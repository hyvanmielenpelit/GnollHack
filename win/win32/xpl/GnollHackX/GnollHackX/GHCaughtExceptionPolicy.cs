using System;
using System.Collections.Generic;

namespace GnollHackX
{
    /* How GHApp.ReportCaughtException reports an exception caught at the top of an async void method or
       lambda: the source name, which exceptions are cancellations, and one report per source and exception
       type. Free of UI and Sentry types, so that the unit tests compile it. */
    public sealed class GHCaughtExceptionPolicy
    {
        public const int MaxReportedKeys = 256;

        private readonly object _reportedKeysLock = new object();
        private readonly HashSet<string> _reportedKeys = new HashSet<string>(StringComparer.Ordinal);

        /* "MainPage.SettingsButton_Clicked" from the caller's file path, with either separator, and member */
        public static string FormatSource(string callerFilePath, string memberName)
        {
            string file = callerFilePath ?? "";
            int separator = Math.Max(file.LastIndexOf('/'), file.LastIndexOf('\\'));
            if (separator >= 0)
                file = file.Substring(separator + 1);
            if (file.EndsWith(".xaml.cs", StringComparison.OrdinalIgnoreCase))
                file = file.Substring(0, file.Length - ".xaml.cs".Length);
            else if (file.EndsWith(".cs", StringComparison.OrdinalIgnoreCase))
                file = file.Substring(0, file.Length - ".cs".Length);
            string member = string.IsNullOrEmpty(memberName) ? "?" : memberName;
            return file.Length == 0 ? member : file + "." + member;
        }

        /* A cancellation, or an AggregateException holding nothing but cancellations */
        public static bool IsCancellation(Exception ex)
        {
            if (ex is OperationCanceledException)
                return true;
            AggregateException aggregate = ex as AggregateException;
            if (aggregate == null)
                return false;
            IList<Exception> inner = aggregate.Flatten().InnerExceptions;
            if (inner.Count == 0)
                return false;
            int i;
            for (i = 0; i < inner.Count; i++)
            {
                if (!(inner[i] is OperationCanceledException))
                    return false;
            }
            return true;
        }

        /* True the first time a source and exception type are seen, for at most MaxReportedKeys keys */
        public bool TryClaimReport(string source, Exception ex)
        {
            string key = (source ?? "") + "|" + (ex == null ? "" : ex.GetType().FullName);
            lock (_reportedKeysLock)
            {
                if (_reportedKeys.Count >= MaxReportedKeys)
                    return false;
                return _reportedKeys.Add(key);
            }
        }
    }
}
