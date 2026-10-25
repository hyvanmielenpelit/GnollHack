#if SENTRY
using System;
using System.Globalization;
using System.Linq;
using GnollHackX;
using Sentry;
using Sentry.Protocol;

namespace GnollHackM
{
    internal static class GHSentryEventFallback
    {
        private const string FallbackExtra = "Exception Fallback";
        private const string StackTraceExtra = "Exception Stack Trace";
        private const string UnknownText = "unknown";

        // WORKAROUND(WA-SENTRY-0009): A failing exception processor sends the event without its exception
        /* Called from BeforeSend; never throws. An event that carries an Exception but no SentryExceptions
           gets one built from the exception's type, HResult and message; an event with neither an
           exception nor a message gets a message. Each exception property is read on its own, since a
           property that throws is one way the SDK's processor fails. */
        internal static void Apply(SentryEvent sentryEvent)
        {
            try
            {
                if (sentryEvent == null)
                    return;

                Exception exception = sentryEvent.Exception;
                bool hasSentryExceptions = sentryEvent.SentryExceptions != null && sentryEvent.SentryExceptions.Any();
                if (exception != null && !hasSentryExceptions)
                {
                    string type = UnknownText;
                    string message = UnknownText;
                    string hresult = UnknownText;
                    string stackTrace = null;
                    try
                    {
                        type = exception.GetType().FullName ?? UnknownText;
                    }
                    catch
                    {
                    }
                    try
                    {
                        message = exception.Message ?? UnknownText;
                    }
                    catch
                    {
                    }
                    try
                    {
                        hresult = exception.HResult.ToString("X8", CultureInfo.InvariantCulture);
                    }
                    catch
                    {
                    }
                    try
                    {
                        stackTrace = exception.StackTrace;
                    }
                    catch
                    {
                    }

                    sentryEvent.SentryExceptions = new[]
                    {
                        new SentryException
                        {
                            Type = type,
                            Value = message + " (HResult 0x" + hresult + ")",
                            Mechanism = new Mechanism
                            {
                                Type = "gnollhack.before_send_fallback",
                                Description = "The SDK's exception processor failed; built in BeforeSend"
                            }
                        }
                    };
                    sentryEvent.SetExtra(FallbackExtra, "exception processor failed");
                    if (!string.IsNullOrEmpty(stackTrace))
                    {
                        if (stackTrace.Length > GHConstants.MaxSentryScopeExtraLength)
                            stackTrace = stackTrace.Substring(0, GHConstants.MaxSentryScopeExtraLength);
                        sentryEvent.SetExtra(StackTraceExtra, stackTrace);
                    }
                }
                else if (exception == null && !hasSentryExceptions && sentryEvent.Message == null)
                {
                    sentryEvent.Message = new SentryMessage { Message = "Event without message or exception" };
                    sentryEvent.SetExtra(FallbackExtra, "no message or exception");
                }
            }
            catch
            {
            }
        }
    }
}
#endif
