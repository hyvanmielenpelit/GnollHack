using GnollHackX;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public sealed class GHMsgHistoryItem
    {
        private string _text = "";
        public string Text { get { return _text; } set { if (value != null) { _text = value; _textSplit = _text.Split(' '); } } }

        private string[] _textSplit = null;
        public string[] TextSplit 
        { get
            {
                if (_textSplit == null)
                    return new string[1] { "" };
                else
                    return _textSplit;
            } 
        }
        public List<string> WrappedTextRows = new List<string>();
        public int Attribute { get; set; }
        public int NHColor { get; set; }

        public byte[] Attributes { get; set; } = null;
        public byte[] Colors { get; set; } = null;

        public bool IsLast { get; set; }

        private string _filter = null;
        public string Filter { get { return _filter; } set { _filter = value; _isMatchCalculated = false; } }
        private bool _isMatchCalculated = false;
        private bool _calculatedMatch = false;
        public bool MatchFilter
        {
            get
            {
                if (_isMatchCalculated)
                {
                    return _calculatedMatch; 
                }
                else
                {
                    if (Filter == null || Filter == "")
                    {
                        _calculatedMatch = true;
                        _isMatchCalculated = true;
                    }
                    else
                    {
                        _calculatedMatch = Text.IndexOf(Filter, StringComparison.OrdinalIgnoreCase) >= 0; // Text.Contains(Filter);
                        _isMatchCalculated = true;
                    }
                    return _calculatedMatch;
                }
            }
        }

        public GHMsgHistoryItem()
        {
            IsLast = false;
        }
        public GHMsgHistoryItem(string text)
        {
            Text = text;
            IsLast = false;
        }
        public GHMsgHistoryItem(string text, int attr, int color)
        {
            Text = text;
            Attribute = attr;
            NHColor = color;
            IsLast = false;
        }
        public GHMsgHistoryItem(string text, byte[] attrs, byte[] colors, int attr, int color)
        {
            Text = text;
            Attributes = attrs;
            Colors = colors;
            Attribute = attr;
            NHColor = color;
            IsLast = false;
        }
    }

    public class GHMsgHistoryList : IEnumerable<GHMsgHistoryItem>
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
