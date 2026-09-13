using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace GnollHackX
{
    /* A growable list whose indexer returns a reference to the stored element, so that a
       large struct element can be mutated and passed on by reference as an array element
       can. A reference obtained from the list points into the backing array that existed
       when it was taken: a growth allocates a new array, after which the old reference
       reads and writes the abandoned one. No reference may be held across Add, AddDefault,
       Insert, EnsureCapacity or a change of Capacity. */
    public sealed class GHRefList<T> : IList<T>, IReadOnlyList<T>
    {
        private const int DefaultCapacity = 4;
        private const int MaxLength = 0X7FFFFFC7;

        private static readonly T[] s_emptyArray = new T[0];

        private T[] _items;
        private int _count;

        public GHRefList()
        {
            _items = s_emptyArray;
        }

        public GHRefList(int capacity)
        {
            if (capacity < 0)
                throw new ArgumentOutOfRangeException(nameof(capacity));

            _items = capacity == 0 ? s_emptyArray : new T[capacity];
        }

        public GHRefList(IEnumerable<T> collection)
        {
            if (collection == null)
                throw new ArgumentNullException(nameof(collection));

            _items = s_emptyArray;
            AddRange(collection);
        }

        public int Count => _count;

        public int Capacity
        {
            get => _items.Length;
            set
            {
                if (value < _count)
                    throw new ArgumentOutOfRangeException(nameof(value));
                if (value == _items.Length)
                    return;

                if (value == 0)
                {
                    _items = s_emptyArray;
                    return;
                }

                T[] destination = new T[value];
                if (_count > 0)
                    Array.Copy(_items, destination, _count);
                _items = destination;
            }
        }

        /* Aliases the stored element; assigning through it modifies the list. */
        public ref T this[int index]
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                if ((uint)index >= (uint)_count)
                    ThrowIndexOutOfRange();

                return ref _items[index];
            }
        }

        /* Spans the live elements only, for ref iteration and bulk operations. */
        public Span<T> AsSpan()
        {
            return new Span<T>(_items, 0, _count);
        }

        public void Add(in T item)
        {
            if (_count == _items.Length)
                Grow(_count + 1);

            _items[_count] = item;
            _count++;
        }

        /* Appends a default element and returns a reference to it, so that a large struct
           can be filled in place rather than built elsewhere and copied in. */
        public ref T AddDefault()
        {
            if (_count == _items.Length)
                Grow(_count + 1);

            _items[_count] = default(T);
            _count++;
            return ref _items[_count - 1];
        }

        public void AddRange(IEnumerable<T> collection)
        {
            if (collection == null)
                throw new ArgumentNullException(nameof(collection));

            ICollection<T> c = collection as ICollection<T>;
            if (c != null)
                EnsureCapacity(_count + c.Count);

            foreach (T item in collection)
                Add(in item);
        }

        public void Insert(int index, in T item)
        {
            if ((uint)index > (uint)_count)
                ThrowIndexOutOfRange();

            /* item is a reference, not a value, and may alias a slot that the shift
               below overwrites, so it is copied out first. */
            T inserted = item;

            if (_count == _items.Length)
                Grow(_count + 1);
            if (index < _count)
                Array.Copy(_items, index, _items, index + 1, _count - index);

            _items[index] = inserted;
            _count++;
        }

        public void EnsureCapacity(int capacity)
        {
            if (capacity > _items.Length)
                Grow(capacity);
        }

        public void TrimExcess()
        {
            if (_count < _items.Length)
                Capacity = _count;
        }

        public void RemoveAt(int index)
        {
            if ((uint)index >= (uint)_count)
                ThrowIndexOutOfRange();

            _count--;
            if (index < _count)
                Array.Copy(_items, index + 1, _items, index, _count - index);
            if (NeedsClearing)
                _items[_count] = default(T);
        }

        /* Moves the last element into the removed slot rather than shifting the tail;
           does not preserve order. */
        public void RemoveAtSwapBack(int index)
        {
            if ((uint)index >= (uint)_count)
                ThrowIndexOutOfRange();

            _count--;
            if (index != _count)
                _items[index] = _items[_count];
            if (NeedsClearing)
                _items[_count] = default(T);
        }

        public bool Remove(in T item)
        {
            int index = IndexOf(in item);
            if (index < 0)
                return false;

            RemoveAt(index);
            return true;
        }

        /* The used range is cleared, not just the count: elements may hold object
           references that would otherwise stay reachable. */
        public void Clear()
        {
            if (_count > 0 && NeedsClearing)
                Array.Clear(_items, 0, _count);
            _count = 0;
        }

        /* Equality falls back to EqualityComparer<T>.Default, which boxes each element for
           a struct that implements neither IEquatable<T> nor Equals. */
        public int IndexOf(in T item)
        {
            return Array.IndexOf(_items, item, 0, _count);
        }

        public int LastIndexOf(in T item)
        {
            return _count == 0 ? -1 : Array.LastIndexOf(_items, item, _count - 1, _count);
        }

        public bool Contains(in T item)
        {
            return IndexOf(in item) >= 0;
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            Array.Copy(_items, 0, array, arrayIndex, _count);
        }

        public void Sort()
        {
            if (_count > 1)
                Array.Sort(_items, 0, _count);
        }

        public void Sort(IComparer<T> comparer)
        {
            if (_count > 1)
                Array.Sort(_items, 0, _count, comparer);
        }

        public void Reverse()
        {
            if (_count > 1)
                Array.Reverse(_items, 0, _count);
        }

        public T[] ToArray()
        {
            if (_count == 0)
                return s_emptyArray;

            T[] result = new T[_count];
            Array.Copy(_items, result, _count);
            return result;
        }

        /* A vacated slot is always overwritten. The probe below would skip that for a T
           that cannot hold a reference, and folds to a constant under a JIT, but it is
           unverified under iOS full AOT; it does not exist at all under netstandard2.0,
           hence the GNH_MAUI guard. GHDrawCommand holds references, so the probe would
           return true for the only current instantiation regardless. */
        private static bool NeedsClearing
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
//#if GNH_MAUI
//                return RuntimeHelpers.IsReferenceOrContainsReferences<T>();
//#else
                return true;
//#endif
            }
        }

        /* Kept out of its callers so that a bounds check inlines as a compare and a
           call: constructing the exception in place bloats every inlined indexer. */
        [MethodImpl(MethodImplOptions.NoInlining)]
        private static void ThrowIndexOutOfRange()
        {
            throw new ArgumentOutOfRangeException("index");
        }

        private void Grow(int minimum)
        {
            int capacity = _items.Length == 0 ? DefaultCapacity : _items.Length * 2;
            if ((uint)capacity > MaxLength)
                capacity = MaxLength;
            if (capacity < minimum)
                capacity = minimum;

            Capacity = capacity;
        }

        /* The interfaces require value-returning members where the public ones take and
           return references. Both coexist because an explicit implementation is separately
           named; the interface indexer hands out a copy, as its contract demands. */
        T IList<T>.this[int index]
        {
            get => this[index];
            set => this[index] = value;
        }

        T IReadOnlyList<T>.this[int index] => this[index];

        bool ICollection<T>.IsReadOnly => false;

        void ICollection<T>.Add(T item) => Add(in item);

        void IList<T>.Insert(int index, T item) => Insert(index, in item);

        bool ICollection<T>.Remove(T item) => Remove(in item);

        bool ICollection<T>.Contains(T item) => Contains(in item);

        int IList<T>.IndexOf(T item) => IndexOf(in item);

        public Enumerator GetEnumerator()
        {
            return new Enumerator(this);
        }

        IEnumerator<T> IEnumerable<T>.GetEnumerator()
        {
            for (int i = 0; i < _count; i++)
                yield return _items[i];
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return ((IEnumerable<T>)this).GetEnumerator();
        }

        /* Current returns a reference, which permits a ref iteration variable. */
        public struct Enumerator
        {
            private readonly GHRefList<T> _list;
            private int _index;

            internal Enumerator(GHRefList<T> list)
            {
                _list = list;
                _index = -1;
            }

            public ref T Current => ref _list._items[_index];

            public bool MoveNext()
            {
                return ++_index < _list._count;
            }
        }
    }
}
