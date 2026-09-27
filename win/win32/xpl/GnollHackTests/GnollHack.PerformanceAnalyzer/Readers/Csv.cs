using System.Globalization;
using System.Text;

namespace GnollHack.PerformanceAnalyzer.Readers
{
    /* Tolerant CSV field handling shared by the readers. Fields may be double-quoted, with
       a doubled quote standing for a literal one; unquoted fields are trimmed. The null
       spellings of trace_processor_shell ("[NULL]") and PresentMon ("NA") read as empty. */
    public static class Csv
    {
        /* Carriage return and line feed, the line ending every file this tool writes uses */
        public static readonly string Crlf = new string(new[] { (char)13, (char)10 });

        /* Any mix of line endings normalized to CRLF */
        public static string ToCrlf(string text)
        {
            string lf = ((char)10).ToString();
            return text.Replace(Crlf, lf).Replace(lf, Crlf);
        }

        public static string[] Split(string line)
        {
            List<string> fields = new List<string>();
            if (line == null)
                return fields.ToArray();
            StringBuilder sb = new StringBuilder();
            bool quoted = false;
            bool wasQuoted = false;
            for (int i = 0; i < line.Length; i++)
            {
                char c = line[i];
                if (quoted)
                {
                    if (c == '"')
                    {
                        if (i + 1 < line.Length && line[i + 1] == '"')
                        {
                            sb.Append('"');
                            i++;
                        }
                        else
                        {
                            quoted = false;
                        }
                    }
                    else
                    {
                        sb.Append(c);
                    }
                }
                else if (c == '"')
                {
                    quoted = true;
                    wasQuoted = true;
                }
                else if (c == ',')
                {
                    fields.Add(wasQuoted ? sb.ToString() : sb.ToString().Trim());
                    sb.Clear();
                    wasQuoted = false;
                }
                else
                {
                    sb.Append(c);
                }
            }
            fields.Add(wasQuoted ? sb.ToString() : sb.ToString().Trim());
            return fields.ToArray();
        }

        public static Dictionary<string, int> HeaderIndex(string[] header)
        {
            Dictionary<string, int> col = new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase);
            for (int i = 0; i < header.Length; i++)
            {
                string name = header[i].Trim();
                if (name.Length > 0 && !col.ContainsKey(name))
                    col[name] = i;
            }
            return col;
        }

        public static bool IsNull(string v)
        {
            if (v == null)
                return true;
            string t = v.Trim();
            return t.Length == 0
                || t.Equals("[NULL]", StringComparison.OrdinalIgnoreCase)
                || t.Equals("NULL", StringComparison.OrdinalIgnoreCase)
                || t.Equals("NA", StringComparison.OrdinalIgnoreCase)
                || t.Equals("N/A", StringComparison.OrdinalIgnoreCase);
        }

        public static string Field(string[] f, int column)
        {
            if (column < 0 || column >= f.Length)
                return null;
            return IsNull(f[column]) ? null : f[column];
        }

        public static bool TryDouble(string v, out double d)
        {
            d = 0;
            if (IsNull(v))
                return false;
            return double.TryParse(v.Trim(), NumberStyles.Float, CultureInfo.InvariantCulture, out d);
        }

        /* Accepts integers and, for tools that print them, integral values in float notation */
        public static bool TryLong(string v, out long l)
        {
            l = 0;
            if (IsNull(v))
                return false;
            string t = v.Trim();
            if (long.TryParse(t, NumberStyles.Integer, CultureInfo.InvariantCulture, out l))
                return true;
            if (double.TryParse(t, NumberStyles.Float, CultureInfo.InvariantCulture, out double d)
                && d >= long.MinValue && d <= long.MaxValue)
            {
                l = (long)Math.Round(d);
                return true;
            }
            return false;
        }

        public static double DoubleOr(string[] f, int column, double fallback)
        {
            return TryDouble(Field(f, column), out double d) ? d : fallback;
        }

        public static long LongOr(string[] f, int column, long fallback)
        {
            return TryLong(Field(f, column), out long l) ? l : fallback;
        }
    }
}
