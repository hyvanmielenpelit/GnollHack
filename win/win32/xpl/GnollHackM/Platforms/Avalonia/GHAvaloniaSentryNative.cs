#if GNH_AVALONIA && LINUX && SENTRY
using System;
using System.Runtime.InteropServices;
using GnollHackX;

namespace GnollHackM
{
    /* The Linux parts of native crash capture (GHSentryNative, libsentry.so): the signal handler strategy, the
       SA_ONSTACK adjustment and the handler snapshot that CheckHandlers compares with. */
    internal static class GHAvaloniaSentryNative
    {
        private const int SENTRY_HANDLER_STRATEGY_CHAIN_AT_START = 1;

        /* x86_64 Linux signal numbers */
        private const int SIGILL = 4;
        private const int SIGTRAP = 5;
        private const int SIGABRT = 6;
        private const int SIGBUS = 7;
        private const int SIGFPE = 8;
        private const int SIGSEGV = 11;

        /* glibc's x86_64 struct sigaction: handler at 0, sa_mask (128 bytes) at 8, int sa_flags at 136, sa_restorer at
           144; 152 bytes, read into a larger zeroed buffer */
        private const int SigActionBufferSize = 256;
        private const int SigActionFlagsOffset = 136;
        private const int SA_ONSTACK = 0x08000000;

        private static readonly int[] _checkedSignals = new int[] { SIGILL, SIGTRAP, SIGABRT, SIGBUS, SIGFPE, SIGSEGV };
        private static readonly int[] _managedFaultSignals = new int[] { SIGILL, SIGTRAP, SIGBUS, SIGFPE };

        private static IntPtr[] _handlers = null;

        [DllImport("sentry")]
        private static extern void sentry_options_set_handler_strategy(IntPtr options, int handlerStrategy);

        [DllImport("libc", EntryPoint = "sigaction", SetLastError = true)]
        private static extern int SigAction(int signum, IntPtr act, IntPtr oldact);

        /* Before sentry_init */
        internal static void ConfigureOptions(IntPtr options)
        {
            /* CoreCLR turns faults in managed code, such as null references and integer division by zero, into
               exceptions from its own signal handlers. CHAIN_AT_START runs that previous handler first and reports a
               crash only when it returns without having moved the instruction or stack pointer. */
            sentry_options_set_handler_strategy(options, SENTRY_HANDLER_STRATEGY_CHAIN_AT_START);
        }

        /* After a successful sentry_init; an exception makes GHSentryNative close sentry-native again */
        internal static void AfterInit()
        {
            ClearAlternateStackOnManagedFaultSignals();
            _handlers = ReadHandlers();
        }

        /* Inproc installs its handlers with SA_ONSTACK, so CoreCLR's chained handler runs on the small alternate
           signal stack. CoreCLR dispatches managed exceptions from its SIGILL, SIGTRAP, SIGBUS and SIGFPE handlers on
           the current stack, and only its SIGSEGV handler leaves the alternate stack first, so a caught
           DivideByZeroException would overflow it. SIGSEGV keeps SA_ONSTACK for native stack overflows; SIGABRT is
           never chained. */
        private static unsafe void ClearAlternateStackOnManagedFaultSignals()
        {
            byte* buffer = stackalloc byte[SigActionBufferSize];
            foreach (int signal in _managedFaultSignals)
            {
                new Span<byte>(buffer, SigActionBufferSize).Clear();
                if (SigAction(signal, IntPtr.Zero, (IntPtr)buffer) != 0)
                {
                    Console.Error.WriteLine("GHAvaloniaSentryNative: cannot read the handler of signal " + signal + ": errno " + Marshal.GetLastPInvokeError());
                    continue;
                }

                int* flags = (int*)(buffer + SigActionFlagsOffset);
                *flags &= ~SA_ONSTACK;
                if (SigAction(signal, (IntPtr)buffer, IntPtr.Zero) != 0)
                    Console.Error.WriteLine("GHAvaloniaSentryNative: cannot update the handler of signal " + signal + ": errno " + Marshal.GetLastPInvokeError());
            }
        }

        private static unsafe IntPtr[] ReadHandlers()
        {
            IntPtr[] handlers = new IntPtr[_checkedSignals.Length];
            byte* buffer = stackalloc byte[SigActionBufferSize];
            for (int i = 0; i < _checkedSignals.Length; i++)
            {
                new Span<byte>(buffer, SigActionBufferSize).Clear();
                handlers[i] = SigAction(_checkedSignals[i], IntPtr.Zero, (IntPtr)buffer) == 0 ? *(IntPtr*)buffer : IntPtr.Zero;
            }
            return handlers;
        }

        /* Logs every crash signal whose handler is no longer the one installed at initialization; inproc cannot
           reinstall its handlers, so this only reports */
        internal static void CheckHandlers(string context)
        {
            try
            {
                IntPtr[] expected = _handlers;
                if (!GHSentryNative.IsInitialized || GHSentryNative.IsClosed || expected == null)
                    return;

                IntPtr[] current = ReadHandlers();
                for (int i = 0; i < _checkedSignals.Length; i++)
                {
                    if (current[i] != expected[i])
                    {
                        GHApp.MaybeWriteGHLog("GHAvaloniaSentryNative: signal " + _checkedSignals[i] + " handler changed " + context
                            + ": 0x" + expected[i].ToString("x") + " -> 0x" + current[i].ToString("x"), true, "native");
                    }
                }
            }
            catch (Exception ex)
            {
                GHSentryNative.WriteError("GHAvaloniaSentryNative: cannot check the signal handlers: " + ex.Message);
            }
        }
    }
}
#endif
