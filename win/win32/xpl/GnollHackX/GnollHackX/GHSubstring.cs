using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public
#if USE_SPAN_FOR_SUBSTRING
        ref
#endif
        struct GHSubstring
    {
#if USE_SPAN_FOR_SUBSTRING
        private ReadOnlySpan<char> _internalSpan;
#else
        private string _internalString;
#endif
        public GHSubstring(string str)
        {
#if USE_SPAN_FOR_SUBSTRING
            _internalSpan = str.AsSpan();
#else
            _internalString = str;
#endif
        }
        public GHSubstring(string str, int start) 
        {
#if USE_SPAN_FOR_SUBSTRING
            _internalSpan = str.AsSpan(start);
#else
            _internalString = str.Substring(start);
#endif
        }
        public GHSubstring(string str, int start, int length)
        {
#if USE_SPAN_FOR_SUBSTRING
            _internalSpan = str.AsSpan(start, length);
#else
            _internalString = str.Substring(start, length);
#endif
        }

        public void SetValue(string str)
        {
#if USE_SPAN_FOR_SUBSTRING
            _internalSpan = str.AsSpan();
#else
            _internalString = str;
#endif
        }
        public void SetValue(string str, int start)
        {
#if USE_SPAN_FOR_SUBSTRING
            _internalSpan = str.AsSpan(start);
#else
            _internalString = str.Substring(start);
#endif
        }
        public void SetValue(string str, int start, int length)
        {
#if USE_SPAN_FOR_SUBSTRING
            _internalSpan = str.AsSpan(start, length);
#else
            _internalString = str.Substring(start, length);
#endif
        }

        public GHSubstring Substring(int start)
        {
#if USE_SPAN_FOR_SUBSTRING
            _internalSpan = _internalSpan.Slice(start);
#else
            _internalString = _internalString.Substring(start);
#endif
            return this;
        }
        public GHSubstring Substring(int start, int length)
        {
#if USE_SPAN_FOR_SUBSTRING
            _internalSpan = _internalSpan.Slice(start, length);
#else
            _internalString = _internalString.Substring(start, length);
#endif
            return this;
        }
#if USE_SPAN_FOR_SUBSTRING
        public ReadOnlySpan<char> Value
        {
            get { return _internalSpan; }
        }
#else
        public string Value
        {
            get { return _internalString; }
        }
#endif
        public int Length
        {
            get
            {
#if USE_SPAN_FOR_SUBSTRING
                return _internalSpan.Length;
#else
                return _internalString.Length;
#endif
            }
        }

        public bool IsEqualTo(string str)
        {
#if USE_SPAN_FOR_SUBSTRING
            return MemoryExtensions.CompareTo(_internalSpan, str.AsSpan(), StringComparison.Ordinal) == 0;
#else
            return _internalString == str;
#endif
        }

    }
}
