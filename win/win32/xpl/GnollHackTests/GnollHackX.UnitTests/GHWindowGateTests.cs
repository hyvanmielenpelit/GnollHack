using GnollHackX.Performance;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHWindowGate's rule table: an open window refuses every begin, a pending
       command refuses a suite's or in-game test's begin but not its own, a running suite
       or test refuses a command's begin and schedule, and a command's cancel or end
       touches only a window a command owns. */
    public class GHWindowGateTests
    {
        [Theory]
        [InlineData(GHWindowOwner.Command)]
        [InlineData(GHWindowOwner.Suite)]
        [InlineData(GHWindowOwner.Diagnostic)]
        public void AnyOpenWindow_RefusesEveryOwner(GHWindowOwner owner)
        {
            Assert.False(GHWindowGate.CanBegin(owner, true, false, false));
            Assert.False(GHWindowGate.CanBegin(owner, true, true, true));
            Assert.NotNull(GHWindowGate.BeginRefusal(owner, true, false, false));
        }

        [Theory]
        [InlineData(GHWindowOwner.Command)]
        [InlineData(GHWindowOwner.Suite)]
        [InlineData(GHWindowOwner.Diagnostic)]
        public void NothingInProgress_EveryOwnerBegins(GHWindowOwner owner)
        {
            Assert.True(GHWindowGate.CanBegin(owner, false, false, false));
            Assert.Null(GHWindowGate.BeginRefusal(owner, false, false, false));
        }

        [Fact]
        public void NoOwner_Refused()
        {
            Assert.False(GHWindowGate.CanBegin(GHWindowOwner.None, false, false, false));
        }

        [Theory]
        [InlineData(GHWindowOwner.Suite)]
        [InlineData(GHWindowOwner.Diagnostic)]
        public void SuiteWhileCommandPending_Refused(GHWindowOwner owner)
        {
            /* The suite or test itself is running while it asks */
            Assert.False(GHWindowGate.CanBegin(owner, false, true, true));
            Assert.False(GHWindowGate.CanBegin(owner, false, true, false));
        }

        [Theory]
        [InlineData(GHWindowOwner.Suite)]
        [InlineData(GHWindowOwner.Diagnostic)]
        public void SuiteWhileItselfRunning_Begins(GHWindowOwner owner)
        {
            Assert.True(GHWindowGate.CanBegin(owner, false, false, true));
        }

        [Fact]
        public void CommandWhileSuiteRunning_Refused()
        {
            Assert.False(GHWindowGate.CanBegin(GHWindowOwner.Command, false, false, true));
            Assert.False(GHWindowGate.CanBegin(GHWindowOwner.Command, false, true, true));
            Assert.False(GHWindowGate.CanSchedule(false, GHWindowOwner.None, true));
            Assert.NotNull(GHWindowGate.ScheduleRefusal(false, GHWindowOwner.None, true));
        }

        [Fact]
        public void CommandSchedule_RefusedByAnotherOwnersWindow()
        {
            Assert.False(GHWindowGate.CanSchedule(true, GHWindowOwner.Suite, false));
            Assert.False(GHWindowGate.CanSchedule(true, GHWindowOwner.Diagnostic, false));
        }

        [Fact]
        public void CommandSchedule_AllowedOverOwnWindowOrNone()
        {
            Assert.True(GHWindowGate.CanSchedule(true, GHWindowOwner.Command, false));
            Assert.True(GHWindowGate.CanSchedule(false, GHWindowOwner.None, false));
            /* A stale owner is ignored when no window is open */
            Assert.True(GHWindowGate.CanSchedule(false, GHWindowOwner.Suite, false));
            Assert.Null(GHWindowGate.ScheduleRefusal(false, GHWindowOwner.None, false));
        }

        [Fact]
        public void CommandCancel_KeepsSuiteContext()
        {
            Assert.False(GHWindowGate.CommandOwnsWindow(GHWindowOwner.Suite));
            Assert.False(GHWindowGate.CommandOwnsWindow(GHWindowOwner.Diagnostic));
            Assert.False(GHWindowGate.CommandOwnsWindow(GHWindowOwner.None));
            Assert.True(GHWindowGate.CommandOwnsWindow(GHWindowOwner.Command));
        }

        [Fact]
        public void OwnCommandPending_DoesNotBlockItsBegin()
        {
            Assert.True(GHWindowGate.CanBegin(GHWindowOwner.Command, false, true, false));
            Assert.Null(GHWindowGate.BeginRefusal(GHWindowOwner.Command, false, true, false));
        }
    }
}
