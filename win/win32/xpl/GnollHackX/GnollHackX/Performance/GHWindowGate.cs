namespace GnollHackX.Performance
{
    /* Who opened the measurement window: a window command (window.cmd or an Android
       broadcast), a performance suite, or the in-game performance test */
    public enum GHWindowOwner
    {
        None = 0,
        Command,
        Suite,
        Diagnostic
    }

    /* The rules deciding whether a measurement window may begin, so no measurement
       replaces or discards another's window:
         - no window begins while any window is open;
         - a suite or in-game test window does not begin while a window command is
           pending (scheduled, waiting to begin);
         - a command window does not begin while a suite or in-game test is running,
           and its own pending command does not block it;
         - a command is not scheduled while a suite or in-game test is running, or while
           a window not owned by a command is open;
         - cancelling or ending a command is always allowed, but touches the window only
           when a command owns it.
       Pure functions of plain inputs: no GHApp, MAUI or Xamarin types. Must compile
       under C# 7.3 (the legacy netstandard2.0 project). */
    public static class GHWindowGate
    {
        /* Why a window for owner may not begin now, or null when it may */
        public static string BeginRefusal(GHWindowOwner owner, bool windowOpen, bool commandPending,
            bool suiteOrDiagnosticRunning)
        {
            if (owner == GHWindowOwner.None)
                return "no window owner";
            if (windowOpen)
                return "another measurement window is open";
            if ((owner == GHWindowOwner.Suite || owner == GHWindowOwner.Diagnostic) && commandPending)
                return "a window command is pending";
            if (owner == GHWindowOwner.Command && suiteOrDiagnosticRunning)
                return "a performance suite or in-game test is running";
            return null;
        }

        public static bool CanBegin(GHWindowOwner owner, bool windowOpen, bool commandPending,
            bool suiteOrDiagnosticRunning)
        {
            return BeginRefusal(owner, windowOpen, commandPending, suiteOrDiagnosticRunning) == null;
        }

        /* Why a window command may not be scheduled now, or null when it may. openOwner
           is the owner of the open window, ignored when none is open. */
        public static string ScheduleRefusal(bool windowOpen, GHWindowOwner openOwner, bool suiteOrDiagnosticRunning)
        {
            if (suiteOrDiagnosticRunning)
                return "a performance suite or in-game test is running";
            if (windowOpen && openOwner != GHWindowOwner.Command)
                return "another measurement window is open";
            return null;
        }

        public static bool CanSchedule(bool windowOpen, GHWindowOwner openOwner, bool suiteOrDiagnosticRunning)
        {
            return ScheduleRefusal(windowOpen, openOwner, suiteOrDiagnosticRunning) == null;
        }

        /* Whether a command's cancel or end may end or discard the open window, and clear
           its run context */
        public static bool CommandOwnsWindow(GHWindowOwner openOwner)
        {
            return openOwner == GHWindowOwner.Command;
        }
    }
}
