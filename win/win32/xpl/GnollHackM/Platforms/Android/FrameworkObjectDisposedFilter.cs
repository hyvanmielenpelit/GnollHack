#nullable enable
using System;

namespace GnollHackM;

// WORKAROUND(WA-MAUI-0016): Framework teardown raises ObjectDisposedException on Android
/* Classifies an ObjectDisposedException as framework teardown noise when the nearest frame
   outside the JNI peer plumbing, the platform bindings and the base class library belongs to
   .NET MAUI, or when no such frame exists. Every other ObjectDisposedException, including
   one without a stack trace, is treated as GnollHack-originated. Pure .NET, so the unit tests
   can link it. */
internal static class FrameworkObjectDisposedFilter
{
    private const int MaxExceptionDepth = 8;

    public const string WarningMessagePrefix = "Handled framework ObjectDisposedException: ";

    private static readonly string[] PeerPlumbingPrefixes =
    {
        "Java.Interop.",
        "Java.Lang.",
        "Android.",
        "AndroidX.",
        "Google.Android.",
        "Xamarin.",
        "Microsoft.Android.",
        "Microsoft.Extensions.",
        "System.",
    };

    private static readonly string[] FrameworkCallerPrefixes =
    {
        "Microsoft.Maui.",
    };

    /* Returns the framework-teardown ObjectDisposedException in the exception or in its chain
       of transparent wrappers, or null when there is none */
    public static ObjectDisposedException? FindFrameworkTeardownDisposal(Exception? exception)
    {
        Exception? current = exception;
        int depth = 0;
        while (current != null && depth < MaxExceptionDepth)
        {
            if (current is ObjectDisposedException ode)
            {
                return IsFrameworkTeardownDisposal(ode.StackTrace) ? ode : null;
            }
            if (!IsTransparentWrapper(current))
            {
                return null;
            }
            if (current is AggregateException aggregate && aggregate.InnerExceptions.Count != 1)
            {
                return null;
            }
            current = current.InnerException;
            depth++;
        }
        return null;
    }

    /* The Sentry message for a suppressed exception; the object name keeps occurrences of the
       same peer type in one Sentry group */
    public static string Describe(ObjectDisposedException ode)
    {
        return WarningMessagePrefix + (string.IsNullOrEmpty(ode.ObjectName) ? "(no object name)" : ode.ObjectName);
    }

    public static bool IsFrameworkTeardownDisposal(string? stackTrace)
    {
        if (string.IsNullOrEmpty(stackTrace))
        {
            return false;
        }
        bool sawFrame = false;
        string[] lines = stackTrace.Split('\n');
        int i;
        for (i = 0; i < lines.Length; i++)
        {
            string? frame = GetFrameName(lines[i]);
            if (frame == null)
            {
                continue;
            }
            sawFrame = true;
            if (StartsWithAny(frame, PeerPlumbingPrefixes))
            {
                continue;
            }
            return StartsWithAny(frame, FrameworkCallerPrefixes);
        }
        return sawFrame;
    }

    /* "at Ns.Type.Method (args) [0x00012] in <mvid>:0" -> "Ns.Type.Method"; null for separators */
    private static string? GetFrameName(string line)
    {
        string text = line.Trim();
        if (text.Length == 0 || text.StartsWith("---", StringComparison.Ordinal))
        {
            return null;
        }
        int space = text.IndexOf(' ');
        int paren = text.IndexOf('(');
        if (space >= 0 && (paren < 0 || space < paren))
        {
            text = text.Substring(space + 1).TrimStart();
            paren = text.IndexOf('(');
        }
        string name = paren >= 0 ? text.Substring(0, paren).TrimEnd() : text;
        return name.Length == 0 ? null : name;
    }

    private static bool IsTransparentWrapper(Exception exception)
    {
        if (exception is AggregateException || exception is System.Reflection.TargetInvocationException)
        {
            return true;
        }
        string? typeName = exception.GetType().FullName;
        return typeName != null &&
            (typeName.StartsWith("Java.Lang.", StringComparison.Ordinal) ||
             typeName == "Android.Runtime.JavaProxyThrowable");
    }

    private static bool StartsWithAny(string value, string[] prefixes)
    {
        int i;
        for (i = 0; i < prefixes.Length; i++)
        {
            if (value.StartsWith(prefixes[i], StringComparison.Ordinal))
            {
                return true;
            }
        }
        return false;
    }
}
