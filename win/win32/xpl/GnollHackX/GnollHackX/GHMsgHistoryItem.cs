using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using System.Text;
#if GNH_MAUI
using GnollHackM;
#endif

namespace GnollHackX
{
    public sealed class GHMsgHistoryItem
    {
        public static readonly string[] _emptyTextSplit = { "" };

        private readonly string _text = "";
        private readonly string[] _textSplit = null;

        public string Text => _text;
        public string[] TextSplit => _textSplit ?? _emptyTextSplit;

        public readonly int Attribute = 0;
        public readonly int NHColor = (int)NhColor.NO_COLOR;

        public readonly byte[] Attributes = null;
        public readonly byte[] Colors = null;

        /* This is accessed only by the PaintSurface thread */
        public readonly List<string> WrappedTextRows = new List<string>();

        /* There are protected by Interlocked */
        private string _filter = null;
        public string Filter { get { return Interlocked.CompareExchange(ref _filter, null, null); } set { Interlocked.Exchange(ref _filter, value); Interlocked.Exchange(ref _isMatchCalculated, 0); } }
        private int _isMatchCalculated = 0;
        private int _calculatedMatch = 0;
        public bool MatchFilter
        {
            get
            {
                if (Interlocked.CompareExchange(ref _isMatchCalculated, 0, 0) != 0)
                {
                    return Interlocked.CompareExchange(ref _calculatedMatch, 0, 0) != 0; 
                }
                else
                {
                    string filter = Filter;
                    bool isCalcMatch;
                    if (string.IsNullOrEmpty(filter))
                        isCalcMatch = true;
                    else
                        isCalcMatch = Text.IndexOf(filter, StringComparison.OrdinalIgnoreCase) >= 0;
                    Interlocked.Exchange(ref _calculatedMatch, isCalcMatch ? 1 : 0);
                    Interlocked.Exchange(ref _isMatchCalculated, 1);
                    return isCalcMatch;
                }
            }
        }

        public GHMsgHistoryItem()
        {

        }
        public GHMsgHistoryItem(string text)
        {
            _text = text;
            _textSplit = _text?.Split(' ') ?? null;
        }
        public GHMsgHistoryItem(string text, int attr, int color)
        {
            _text = text;
            _textSplit = _text?.Split(' ') ?? null;
            Attribute = attr;
            NHColor = color;
        }
        public GHMsgHistoryItem(string text, byte[] attrs, byte[] colors, int attr, int color)
        {
            _text = text;
            _textSplit = _text?.Split(' ') ?? null;
            Attributes = attrs;
            Colors = colors;
            Attribute = attr;
            NHColor = color;
        }
    }

    public sealed class GHMsgHistoryList : IEnumerable<GHMsgHistoryItem>
    {
        private readonly GHMsgHistoryItem[] _items;
        private readonly int _capacity;
        private readonly int _excess;
        private int _start;
        private int _count;
        public int Capacity { get { return _capacity; } }
        public int Excess { get { return _excess; } }
        public int Start { get { return _start; } }
        public int Count { get { return _count; } }
        public int Length { get { return _count; } }

        public GHMsgHistoryList(int capacity, int excess)
        {
            _capacity = Math.Max(1, capacity);
            _excess = Math.Max(0, Math.Min(_capacity, excess));
            _items = new GHMsgHistoryItem[_capacity + _excess];
            _start = 0;
            _count = 0;
        }

        public GHMsgHistoryList(GHMsgHistoryList oldList)
        {
            _capacity = Math.Max(1, oldList.Capacity);
            _excess = Math.Max(0, Math.Min(_capacity, oldList.Excess));
            _items = new GHMsgHistoryItem[_capacity + _excess];
            _start = 0;
            _count = oldList.Count;
            for (int i = 0; i < _count; i++)
            {
                _items[i] = oldList[i];
            }
        }

        /* Copies # of last items from oldList equal to Math.Min(capacity, oldList.Count) */
        public GHMsgHistoryList(GHMsgHistoryList oldList, int capacity, int excess)
        {
            _capacity = Math.Max(1, capacity);
            _excess = Math.Max(0, Math.Min(_capacity, excess));
            _items = new GHMsgHistoryItem[_capacity + _excess];
            _start = 0;
            _count = Math.Min(_capacity, oldList.Count);
            int start = oldList.Count - _count;
            for (int i = 0; i < _count; i++)
            {
                _items[i] = oldList[i + start];
            }
        }

        public GHMsgHistoryItem this[int index] { get { return _items[_start + index]; } }

        public bool TryAdd(GHMsgHistoryItem item)
        {
            if (IsFull)
                return false;

            _items[_start + _count] = item;
            if (_count == _capacity)
                _start++;
            else
                _count++;

            return true;
        }

        public bool IsFull { get { return _start + _count >= _capacity + _excess - 1;  } }
        public GHMsgHistorySpan CurrentSpan
        {
            get { return new GHMsgHistorySpan(_items, _start, _count); }
        }
        public IEnumerator<GHMsgHistoryItem> GetEnumerator()
        {
            for (int i = 0; i < _count; i++)
                yield return _items[i];
        }

        IEnumerator IEnumerable.GetEnumerator() // Non-generic version
        {
            return GetEnumerator(); // Delegate to generic version
        }
    }

    public sealed class GHMsgHistorySpan : IEnumerable<GHMsgHistoryItem>
    {
        private readonly GHMsgHistoryItem[] _items;
        private readonly int _start;
        private readonly int _count;
        public int Count { get { return _count; } }
        public int Length { get { return _count; } }

        public GHMsgHistorySpan(GHMsgHistoryItem[] items, int start, int count) 
        {
            _items = items;
            _start = start;
            _count = count;
        }
        public GHMsgHistoryItem this[int index] { get { return _items[_start + index]; } }

        public IEnumerator<GHMsgHistoryItem> GetEnumerator()
        {
            for (int i = 0; i < _count; i++)
                yield return _items[i];
        }

        IEnumerator IEnumerable.GetEnumerator() // Non-generic version
        {
            return GetEnumerator(); // Delegate to generic version
        }
    }
}
