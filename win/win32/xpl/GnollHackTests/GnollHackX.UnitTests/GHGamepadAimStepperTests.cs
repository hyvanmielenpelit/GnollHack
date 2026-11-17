using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGamepadAimStepper: the immediate step of an aimed press, the first delay and the interval of a held
       press, a new aim restarting the delay, a lost aim or a release ending the hold, and a non-repeating press;
       times in milliseconds */
    public class GHGamepadAimStepperTests
    {
        private static GHGamepadAimStepper Make()
        {
            return new GHGamepadAimStepper(220, 90);
        }

        [Fact]
        public void PressWhileAimed_StepsAtOnce()
        {
            GHGamepadAimStepper s = Make();
            s.SetAim(3, 0);
            Assert.True(s.PressConfirm(0, true));
            Assert.True(s.IsConfirmHeld);
        }

        [Fact]
        public void PressWithoutAim_HoldsNothing()
        {
            GHGamepadAimStepper s = Make();
            Assert.False(s.PressConfirm(0, true));
            Assert.False(s.IsConfirmHeld);
            s.SetAim(2, 10);
            Assert.False(s.IsStepDue(1000));
        }

        [Fact]
        public void Held_RepeatsAfterFirstDelayThenInterval()
        {
            GHGamepadAimStepper s = Make();
            s.SetAim(1, 0);
            s.PressConfirm(0, true);
            Assert.False(s.IsStepDue(219));
            Assert.True(s.IsStepDue(220));
            s.NoteStepSent(220);
            Assert.False(s.IsStepDue(309));
            Assert.True(s.IsStepDue(310));
        }

        [Fact]
        public void NewAim_RestartsFirstDelay()
        {
            GHGamepadAimStepper s = Make();
            s.SetAim(1, 0);
            s.PressConfirm(0, true);
            Assert.True(s.SetAim(5, 200));
            Assert.False(s.IsStepDue(419));
            Assert.True(s.IsStepDue(420));
            Assert.Equal(5, s.Sector);
        }

        [Fact]
        public void LosingAim_EndsTheHold()
        {
            GHGamepadAimStepper s = Make();
            s.SetAim(1, 0);
            s.PressConfirm(0, true);
            s.SetAim(-1, 50);
            s.SetAim(1, 60);
            Assert.False(s.IsConfirmHeld);
            Assert.False(s.IsStepDue(1000));
        }

        [Fact]
        public void Release_Stops()
        {
            GHGamepadAimStepper s = Make();
            s.SetAim(1, 0);
            s.PressConfirm(0, true);
            s.ReleaseConfirm();
            Assert.False(s.IsStepDue(1000));
        }

        [Fact]
        public void NonRepeatingPress_StepsOnce()
        {
            GHGamepadAimStepper s = Make();
            s.SetAim(4, 0);
            Assert.True(s.PressConfirm(0, false));
            Assert.False(s.IsStepDue(1000));
        }

        [Fact]
        public void SetAim_ReportsChangesOnly()
        {
            GHGamepadAimStepper s = Make();
            Assert.True(s.SetAim(2, 0));
            Assert.False(s.SetAim(2, 10));
            Assert.True(s.SetAim(9, 20));
            Assert.False(s.IsAiming);
            Assert.Equal(-1, s.Sector);
        }

        [Fact]
        public void Reset_ClearsAimAndHold()
        {
            GHGamepadAimStepper s = Make();
            s.SetAim(6, 0);
            s.PressConfirm(0, true);
            s.Reset();
            Assert.False(s.IsAiming);
            Assert.False(s.IsConfirmHeld);
            Assert.False(s.IsStepDue(1000));
        }
    }
}
