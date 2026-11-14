using System;
using System.Globalization;
using System.Text;

namespace GnollHackX
{
    /* The process state snapshot that Android returns with the process's exit record; at most 128 ASCII bytes */
    public sealed class GHExitStateSummary
    {
        public const string Prefix = "gs1";
        public const int MaxBytes = 128;

        /* Indexed by (int)MemoryPressureLevel */
        private static readonly string[] _memoryWarningLevelNames = { "Low", "Medium", "Critical", "Background", "Complete" };

        public long UnixSeconds = -1;              /* u: when the snapshot was written */
        public int GameState = -1;                 /* g: 0 none, 1 game, 2 replay */
        public long SecondsInGame = -1;            /* t */
        public int MemoryWarningLevel = -1;        /* w: last warning; 0 none, else (int)MemoryPressureLevel + 1 */
        public long MemoryWarningAgeSeconds = -1;
        public int HighestMemoryWarningLevel = -1; /* highest since launch, Background excluded; same coding */
        public long RssMB = -1;                    /* r */
        public long SwapMB = -1;                   /* s */
        public long PssMB = -1;                    /* p: last full sample */
        public long GraphicsMB = -1;
        public long NativeHeapMB = -1;
        public long PssSampleAgeSeconds = -1;
        public long GpuCacheUsedMB = -1;           /* c */
        public long GpuCacheLimitMB = -1;
        public long BitmapMB = -1;                 /* b */
        public long SoundMB = -1;                  /* f */
        public long ManagedHeapMB = -1;            /* h */

        public string Format()
        {
            StringBuilder sb = new StringBuilder(MaxBytes);
            bool full = false;
            sb.Append(Prefix);
            AppendField(sb, ref full, "u", UnixSeconds);
            AppendField(sb, ref full, "g", GameState);
            AppendField(sb, ref full, "t", SecondsInGame);
            AppendField(sb, ref full, "w", MemoryWarningLevel, MemoryWarningAgeSeconds, HighestMemoryWarningLevel);
            AppendField(sb, ref full, "r", RssMB);
            AppendField(sb, ref full, "s", SwapMB);
            AppendField(sb, ref full, "p", PssMB, GraphicsMB, NativeHeapMB, PssSampleAgeSeconds);
            AppendField(sb, ref full, "c", GpuCacheUsedMB, GpuCacheLimitMB);
            AppendField(sb, ref full, "b", BitmapMB);
            AppendField(sb, ref full, "f", SoundMB);
            AppendField(sb, ref full, "h", ManagedHeapMB);
            return sb.ToString();
        }

        /* Fields are appended in order of diagnostic value, so the first one that does not fit ends the text */
        private static void AppendField(StringBuilder sb, ref bool full, string key, params long[] values)
        {
            if (full)
                return;
            bool any = false;
            int i;
            for (i = 0; i < values.Length; i++)
            {
                if (values[i] != -1)
                {
                    any = true;
                    break;
                }
            }
            if (!any)
                return;

            StringBuilder field = new StringBuilder();
            field.Append(' ').Append(key).Append('=');
            for (i = 0; i < values.Length; i++)
            {
                if (i > 0)
                    field.Append('/');
                field.Append(values[i].ToString(CultureInfo.InvariantCulture));
            }
            if (sb.Length + field.Length <= MaxBytes)
                sb.Append(field.ToString());
            else
                full = true;
        }

        public static bool TryParse(string text, out GHExitStateSummary summary)
        {
            summary = null;
            if (string.IsNullOrEmpty(text) || !text.StartsWith(Prefix + " ", StringComparison.Ordinal))
                return false;

            GHExitStateSummary res = new GHExitStateSummary();
            string[] tokens = text.Substring(Prefix.Length + 1).Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (string token in tokens)
            {
                int eq = token.IndexOf('=');
                if (eq <= 0)
                    continue;
                string key = token.Substring(0, eq);
                string[] parts = token.Substring(eq + 1).Split('/');
                switch (key)
                {
                    case "u":
                        res.UnixSeconds = ParsePart(parts, 0);
                        break;
                    case "g":
                        res.GameState = ParseIntPart(parts, 0);
                        break;
                    case "t":
                        res.SecondsInGame = ParsePart(parts, 0);
                        break;
                    case "w":
                        res.MemoryWarningLevel = ParseIntPart(parts, 0);
                        res.MemoryWarningAgeSeconds = ParsePart(parts, 1);
                        res.HighestMemoryWarningLevel = ParseIntPart(parts, 2);
                        break;
                    case "r":
                        res.RssMB = ParsePart(parts, 0);
                        break;
                    case "s":
                        res.SwapMB = ParsePart(parts, 0);
                        break;
                    case "p":
                        res.PssMB = ParsePart(parts, 0);
                        res.GraphicsMB = ParsePart(parts, 1);
                        res.NativeHeapMB = ParsePart(parts, 2);
                        res.PssSampleAgeSeconds = ParsePart(parts, 3);
                        break;
                    case "c":
                        res.GpuCacheUsedMB = ParsePart(parts, 0);
                        res.GpuCacheLimitMB = ParsePart(parts, 1);
                        break;
                    case "b":
                        res.BitmapMB = ParsePart(parts, 0);
                        break;
                    case "f":
                        res.SoundMB = ParsePart(parts, 0);
                        break;
                    case "h":
                        res.ManagedHeapMB = ParsePart(parts, 0);
                        break;
                    default:
                        break;
                }
            }
            summary = res;
            return true;
        }

        private static long ParsePart(string[] parts, int index)
        {
            long value;
            if (parts == null || index >= parts.Length)
                return -1;
            if (!long.TryParse(parts[index], NumberStyles.AllowLeadingSign, CultureInfo.InvariantCulture, out value))
                return -1;
            return value;
        }

        private static int ParseIntPart(string[] parts, int index)
        {
            long value = ParsePart(parts, index);
            return value < int.MinValue || value > int.MaxValue ? -1 : (int)value;
        }

        public static string GameStateName(int gameState)
        {
            switch (gameState)
            {
                case 0: return "none";
                case 1: return "game";
                case 2: return "replay";
                default: return "unknown";
            }
        }

        /* "none" for 0, "unknown" for anything not a level */
        public static string MemoryWarningName(int level)
        {
            if (level == 0)
                return "none";
            if (level >= 1 && level <= _memoryWarningLevelNames.Length)
                return _memoryWarningLevelNames[level - 1];
            return "unknown";
        }

        public string Describe(long exitUnixSeconds)
        {
            StringBuilder sb = new StringBuilder();
            if (GameState != -1)
            {
                sb.Append(GameStateName(GameState));
                if (SecondsInGame >= 0)
                    sb.Append(", ").Append(SecondsInGame.ToString(CultureInfo.InvariantCulture)).Append(" s in");
            }
            if (MemoryWarningLevel == 0)
            {
                AppendSeparator(sb);
                sb.Append("no memory warning");
            }
            else if (MemoryWarningLevel > 0)
            {
                AppendSeparator(sb);
                sb.Append("warning ").Append(MemoryWarningName(MemoryWarningLevel));
                if (MemoryWarningAgeSeconds != -1)
                    sb.Append(' ').Append(Age(MemoryWarningAgeSeconds)).Append(" s before snapshot");
                if (HighestMemoryWarningLevel > 0)
                    sb.Append(", highest ").Append(MemoryWarningName(HighestMemoryWarningLevel));
            }
            if (RssMB != -1 || SwapMB != -1)
            {
                AppendSeparator(sb);
                sb.Append("rss ").Append(Mb(RssMB)).Append(" MB, swap ").Append(Mb(SwapMB)).Append(" MB");
            }
            if (PssMB != -1)
            {
                AppendSeparator(sb);
                sb.Append("pss ").Append(Mb(PssMB)).Append(" MB (graphics ").Append(Mb(GraphicsMB))
                    .Append(", native ").Append(Mb(NativeHeapMB)).Append(')');
                if (PssSampleAgeSeconds != -1)
                    sb.Append(' ').Append(Age(PssSampleAgeSeconds)).Append(" s before snapshot");
            }
            if (GpuCacheUsedMB != -1 || GpuCacheLimitMB != -1)
            {
                AppendSeparator(sb);
                sb.Append("GPU cache ").Append(Mb(GpuCacheUsedMB)).Append(" / ").Append(Mb(GpuCacheLimitMB)).Append(" MB");
            }
            if (BitmapMB != -1)
            {
                AppendSeparator(sb);
                sb.Append("bitmaps ").Append(Mb(BitmapMB)).Append(" MB");
            }
            if (SoundMB != -1)
            {
                AppendSeparator(sb);
                sb.Append("sound ").Append(Mb(SoundMB)).Append(" MB");
            }
            if (ManagedHeapMB != -1)
            {
                AppendSeparator(sb);
                sb.Append("managed heap ").Append(Mb(ManagedHeapMB)).Append(" MB");
            }
            if (exitUnixSeconds > 0 && UnixSeconds != -1)
            {
                AppendSeparator(sb);
                sb.Append("snapshot ").Append(Age(exitUnixSeconds - UnixSeconds)).Append(" s before exit");
            }
            return sb.ToString();
        }

        private static void AppendSeparator(StringBuilder sb)
        {
            if (sb.Length > 0)
                sb.Append("; ");
        }

        private static string Age(long seconds)
        {
            return (seconds < 0 ? 0 : seconds).ToString(CultureInfo.InvariantCulture);
        }

        private static string Mb(long value)
        {
            return value < 0 ? "?" : value.ToString(CultureInfo.InvariantCulture);
        }

        /* Every field except UnixSeconds */
        public bool ContentEquals(GHExitStateSummary other)
        {
            if (other == null)
                return false;
            return GameState == other.GameState
                && SecondsInGame == other.SecondsInGame
                && MemoryWarningLevel == other.MemoryWarningLevel
                && MemoryWarningAgeSeconds == other.MemoryWarningAgeSeconds
                && HighestMemoryWarningLevel == other.HighestMemoryWarningLevel
                && RssMB == other.RssMB
                && SwapMB == other.SwapMB
                && PssMB == other.PssMB
                && GraphicsMB == other.GraphicsMB
                && NativeHeapMB == other.NativeHeapMB
                && PssSampleAgeSeconds == other.PssSampleAgeSeconds
                && GpuCacheUsedMB == other.GpuCacheUsedMB
                && GpuCacheLimitMB == other.GpuCacheLimitMB
                && BitmapMB == other.BitmapMB
                && SoundMB == other.SoundMB
                && ManagedHeapMB == other.ManagedHeapMB;
        }

        /* Approximate rated RAM from ActivityManager.MemoryInfo.TotalMem, which is below the rated size;
           thresholds are in GiB with inclusive lower bounds */
        public static string MemoryBucket(ulong totalMemoryBytes)
        {
            if (totalMemoryBytes == 0)
                return "unknown";
            double gib = totalMemoryBytes / (1024.0 * 1024.0 * 1024.0);
            if (gib < 2.0)
                return "2 GB or less";
            if (gib < 3.0)
                return "3 GB";
            if (gib < 4.0)
                return "4 GB";
            if (gib < 6.0)
                return "6 GB";
            if (gib < 8.0)
                return "8 GB";
            if (gib < 11.5)
                return "12 GB";
            return "16 GB+";
        }
    }
}
