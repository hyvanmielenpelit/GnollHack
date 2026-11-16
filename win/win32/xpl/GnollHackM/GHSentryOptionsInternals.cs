#if SENTRY
using System;
using System.Runtime.CompilerServices;
using Sentry;

namespace GnollHackM
{
    // WORKAROUND(WA-SENTRY-0013): Sentry .NET 7 makes the native scope-sync and crashed-last-run options internal
    /* Sets SentryOptions.ScopeObserver, EnableScopeSync and CrashedLastRun, which Sentry .NET declares internal.
       Never throws: a member the Sentry release in use lacks is named in Failure, and the others stay set. */
    internal static class GHSentryOptionsInternals
    {
        public static string Failure { get; private set; }

        public static bool TrySetNativeHooks(SentryOptions options, IScopeObserver scopeObserver, Func<bool> crashedLastRun)
        {
            string failure = null;
            if (options == null)
            {
                failure = "no options";
            }
            else
            {
                try
                {
                    SetScopeObserver(options, scopeObserver);
                    SetEnableScopeSync(options, true);
                }
                catch (Exception ex)
                {
                    failure = "scope sync: " + ex.GetType().Name + ": " + ex.Message;
                }

                try
                {
                    SetCrashedLastRun(options, crashedLastRun);
                }
                catch (Exception ex)
                {
                    failure = (failure == null ? "" : failure + "; ") + "CrashedLastRun: " + ex.GetType().Name + ": " + ex.Message;
                }
            }

            Failure = failure;
            return failure == null;
        }

        [UnsafeAccessor(UnsafeAccessorKind.Method, Name = "set_ScopeObserver")]
        private static extern void SetScopeObserver(SentryOptions options, IScopeObserver value);

        [UnsafeAccessor(UnsafeAccessorKind.Method, Name = "set_EnableScopeSync")]
        private static extern void SetEnableScopeSync(SentryOptions options, bool value);

        [UnsafeAccessor(UnsafeAccessorKind.Method, Name = "set_CrashedLastRun")]
        private static extern void SetCrashedLastRun(SentryOptions options, Func<bool> value);
    }
}
#endif
