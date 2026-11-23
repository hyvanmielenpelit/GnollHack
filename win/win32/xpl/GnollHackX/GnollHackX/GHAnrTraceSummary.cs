using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace GnollHackX
{
    /* Summary of the thread dump in an Android ANR trace (ApplicationExitInfo.getTraceInputStream), read from
       the line after "----- pid ..." up to "----- end ...": the main thread's native frames, its wait
       signature, and the other threads that have native frames inside an APK */
    public sealed class GHAnrTraceSummary
    {
        public const int MaxMainFrames = 32;
        public const int MaxApkThreads = 48;
        public const int MaxSignatureLength = 200; /* Sentry's tag value limit */

        private static readonly Regex _threadHeaderRegex = new Regex("^\"(?<name>[^\"]*)\"", RegexOptions.CultureInvariant);
        private static readonly Regex _threadStateRegex = new Regex(@"\btid=\d+ (?<state>\S+)", RegexOptions.CultureInvariant);
        private static readonly Regex _nativeFrameRegex = new Regex(@"^\s+(?:native: )?#(?<idx>\d+) pc (?<pc>[0-9a-fA-F]+)\s+(?<rest>\S.*)$",
            RegexOptions.CultureInvariant);
        private static readonly Regex _installDirRegex = new Regex("/data/app/~~[^/]+/[^/]+/", RegexOptions.CultureInvariant);
        private static readonly Regex _trailingOffsetRegex = new Regex(@"\+\d+$", RegexOptions.CultureInvariant);

        public int ThreadCount { get; private set; }
        public string MainSignature { get; private set; }     /* "no-main" when no thread is named main */
        public string MainNativeFrames { get; private set; }  /* compact frames, one per line; "" when none */
        public string ApkThreads { get; private set; }        /* one line per non-main thread with an APK frame; "" when none */
        public int SameWaitThreadCount { get; private set; }  /* non-main threads whose signature equals the main thread's */

        private sealed class Frame
        {
            public string Path;
            public string Label;
            public string Function;
            public bool IsLibc;
            public bool IsApk;
            public string Compact;
        }

        private sealed class ThreadInfo
        {
            public string Name;
            public string State;
            public readonly List<Frame> Frames = new List<Frame>();
            public string Signature;
        }

        private GHAnrTraceSummary()
        {
            MainSignature = "no-main";
            MainNativeFrames = "";
            ApkThreads = "";
        }

        public static GHAnrTraceSummary Parse(TextReader reader, int maxLines)
        {
            List<ThreadInfo> threads = new List<ThreadInfo>();
            ThreadInfo current = null;
            if (reader != null)
            {
                try
                {
                    int lineCount;
                    for (lineCount = 0; lineCount < maxLines; lineCount++)
                    {
                        string line = reader.ReadLine();
                        if (line == null)
                            break;
                        if (line.StartsWith("----- end ", StringComparison.Ordinal))
                            break;
                        if (line.Length > 0 && line[0] == '"')
                        {
                            Match header = _threadHeaderRegex.Match(line);
                            if (header.Success)
                            {
                                current = new ThreadInfo();
                                current.Name = header.Groups["name"].Value;
                                current.State = ThreadState(line);
                                threads.Add(current);
                            }
                            continue;
                        }
                        if (current == null)
                            continue;
                        Match frame = _nativeFrameRegex.Match(line);
                        if (frame.Success)
                            current.Frames.Add(ParseFrame(line, frame.Groups["rest"].Value));
                    }
                }
                catch (IOException)
                {
                    /* A truncated trace is summarized as far as it was read */
                }
            }
            return Build(threads);
        }

        public static string SameWaitBucket(int count)
        {
            if (count <= 0)
                return "0";
            if (count <= 2)
                return "1-2";
            if (count <= 9)
                return "3-9";
            return "10+";
        }

        private static string ThreadState(string header)
        {
            Match state = _threadStateRegex.Match(header);
            if (state.Success)
                return state.Groups["state"].Value;
            if (header.IndexOf("(not attached)", StringComparison.Ordinal) >= 0)
                return "not-attached";
            return "unknown";
        }

        private static Frame ParseFrame(string line, string rest)
        {
            Frame frame = new Frame();
            int pathEnd = 0;
            while (pathEnd < rest.Length && !char.IsWhiteSpace(rest[pathEnd]))
                pathEnd++;
            frame.Path = rest.Substring(0, pathEnd);

            int bang = frame.Path.IndexOf('!');
            string filePath = bang >= 0 ? frame.Path.Substring(0, bang) : frame.Path;
            string fileName = FileName(filePath);
            frame.IsLibc = fileName == "libc.so";
            frame.IsApk = filePath.EndsWith(".apk", StringComparison.Ordinal);
            if (bang >= 0)
                frame.Label = frame.Path.Substring(bang + 1);
            else if (frame.IsApk)
                frame.Label = "apk";
            else
                frame.Label = fileName;

            frame.Function = ParseFunction(rest.Substring(pathEnd));

            string compact = line.Trim();
            if (compact.StartsWith("native: ", StringComparison.Ordinal))
                compact = compact.Substring(8);
            frame.Compact = _installDirRegex.Replace(compact, "<app>/");
            return frame;
        }

        private static string FileName(string path)
        {
            int slash = path.LastIndexOf('/');
            return slash >= 0 ? path.Substring(slash + 1) : path;
        }

        /* The function from the first top-level parenthesized group that is not an offset, a BuildId,
           "???" or "deleted"; "" when there is none */
        private static string ParseFunction(string tail)
        {
            int i = 0;
            while (i < tail.Length)
            {
                if (tail[i] != '(')
                {
                    i++;
                    continue;
                }
                int depth = 0;
                int j;
                for (j = i; j < tail.Length; j++)
                {
                    if (tail[j] == '(')
                    {
                        depth++;
                    }
                    else if (tail[j] == ')')
                    {
                        depth--;
                        if (depth == 0)
                            break;
                    }
                }
                if (j >= tail.Length)
                    return "";
                string content = tail.Substring(i + 1, j - i - 1);
                if (!content.StartsWith("offset ", StringComparison.Ordinal)
                    && !content.StartsWith("BuildId:", StringComparison.Ordinal)
                    && content != "???"
                    && content != "deleted")
                    return FunctionName(content);
                i = j + 1;
            }
            return "";
        }

        private static string FunctionName(string content)
        {
            string withoutOffset = _trailingOffsetRegex.Replace(content, "");
            int paren = withoutOffset.IndexOf('(');
            string name = paren >= 0 ? withoutOffset.Substring(0, paren).Trim() : withoutOffset.Trim();
            return name.Length > 0 ? name : withoutOffset.Trim();
        }

        /* "none"; or the outermost function of the leading libc run ("running" without one), then
           " < " and the first frame after the run as label[:function], and " x" and the frames after the run */
        private static string Signature(List<Frame> frames)
        {
            if (frames.Count == 0)
                return "none";
            int run = 0;
            while (run < frames.Count && frames[run].IsLibc)
                run++;
            string top = run == 0 ? "running" : (frames[run - 1].Function.Length > 0 ? frames[run - 1].Function : "libc");
            if (run == frames.Count)
                return Cut(top);
            Frame first = frames[run];
            string label = first.Function.Length > 0 ? first.Label + ":" + first.Function : first.Label;
            return Cut(top + " < " + label + " x" + (frames.Count - run).ToString(System.Globalization.CultureInfo.InvariantCulture));
        }

        private static string Cut(string text)
        {
            return text.Length <= MaxSignatureLength ? text : text.Substring(0, MaxSignatureLength);
        }

        private static bool IsWaitSignature(string signature)
        {
            return signature != "none" && signature != "no-main" && !signature.StartsWith("running", StringComparison.Ordinal);
        }

        private static GHAnrTraceSummary Build(List<ThreadInfo> threads)
        {
            GHAnrTraceSummary summary = new GHAnrTraceSummary();
            summary.ThreadCount = threads.Count;
            ThreadInfo main = null;
            foreach (ThreadInfo thread in threads)
            {
                thread.Signature = Signature(thread.Frames);
                if (main == null && thread.Name == "main")
                    main = thread;
            }

            if (main != null)
            {
                summary.MainSignature = main.Signature;
                StringBuilder frames = new StringBuilder();
                int i;
                for (i = 0; i < main.Frames.Count && i < MaxMainFrames; i++)
                {
                    if (i > 0)
                        frames.Append('\n');
                    frames.Append(main.Frames[i].Compact);
                }
                summary.MainNativeFrames = frames.ToString();
            }

            bool countSameWait = IsWaitSignature(summary.MainSignature);
            int sameWait = 0;
            int apkLines = 0;
            int apkOmitted = 0;
            StringBuilder apk = new StringBuilder();
            foreach (ThreadInfo thread in threads)
            {
                if (thread == main)
                    continue;
                if (countSameWait && thread.Signature == summary.MainSignature)
                    sameWait++;
                Frame firstApk = null;
                Frame firstNonLibc = null;
                foreach (Frame frame in thread.Frames)
                {
                    if (firstNonLibc == null && !frame.IsLibc)
                        firstNonLibc = frame;
                    if (firstApk == null && frame.IsApk)
                        firstApk = frame;
                }
                if (firstApk == null)
                    continue;
                if (apkLines >= MaxApkThreads)
                {
                    apkOmitted++;
                    continue;
                }
                if (apkLines > 0)
                    apk.Append('\n');
                apk.Append(thread.Name).Append(" [").Append(thread.State).Append("] ").Append(thread.Signature)
                    .Append(" | ").Append((firstNonLibc ?? firstApk).Compact);
                apkLines++;
            }
            if (apkOmitted > 0)
                apk.Append("\n... and ").Append(apkOmitted.ToString(System.Globalization.CultureInfo.InvariantCulture)).Append(" more");
            summary.ApkThreads = apk.ToString();
            summary.SameWaitThreadCount = sameWait;
            return summary;
        }
    }
}
