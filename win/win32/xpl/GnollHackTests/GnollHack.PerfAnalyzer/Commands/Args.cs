using System.Globalization;

namespace GnollHack.PerfAnalyzer.Commands
{
    /* Minimal --name value parser. A flag with no value is a boolean. Repeated names
       accumulate (used for --a and --b file lists). */
    public sealed class Args
    {
        private readonly Dictionary<string, List<string>> _values = new Dictionary<string, List<string>>(StringComparer.OrdinalIgnoreCase);
        public string Command { get; }

        public Args(string[] argv)
        {
            int start = 0;
            if (argv.Length > 0 && !argv[0].StartsWith("--"))
            {
                Command = argv[0];
                start = 1;
            }
            string current = null;
            for (int i = start; i < argv.Length; i++)
            {
                string t = argv[i];
                if (t.StartsWith("--"))
                {
                    current = t.Substring(2);
                    if (!_values.ContainsKey(current))
                        _values[current] = new List<string>();
                }
                else if (current != null)
                {
                    _values[current].Add(t);
                }
            }
        }

        public bool Has(string name) { return _values.ContainsKey(name); }
        public string Get(string name) { return _values.TryGetValue(name, out List<string> v) && v.Count > 0 ? v[0] : null; }
        public List<string> GetAll(string name) { return _values.TryGetValue(name, out List<string> v) ? v : new List<string>(); }
        public string Require(string name)
        {
            string v = Get(name);
            if (v == null)
                throw new ArgumentException("missing required option --" + name);
            return v;
        }
        public double GetDouble(string name, double fallback)
        {
            string v = Get(name);
            return v != null && double.TryParse(v, NumberStyles.Float, CultureInfo.InvariantCulture, out double d) ? d : fallback;
        }
        public int GetInt(string name, int fallback)
        {
            string v = Get(name);
            return v != null && int.TryParse(v, NumberStyles.Integer, CultureInfo.InvariantCulture, out int d) ? d : fallback;
        }
        public IEnumerable<KeyValuePair<string, string>> Prefixed(string prefix)
        {
            foreach (KeyValuePair<string, List<string>> kv in _values)
                if (kv.Key.StartsWith(prefix, StringComparison.OrdinalIgnoreCase) && kv.Value.Count > 0)
                    yield return new KeyValuePair<string, string>(kv.Key.Substring(prefix.Length), kv.Value[0]);
        }
    }
}
