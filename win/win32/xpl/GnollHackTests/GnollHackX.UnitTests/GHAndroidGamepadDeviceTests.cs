using System;
using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHAndroidGamepadDevice: key edges between Pumps, autorepeat, the D-pad merge of keys and hat,
       L2/R2 keys with analog triggers, the axis layouts, changed-only stick and trigger emission, Resync
       suppression and InputLost latching */
    public class GHAndroidGamepadDeviceTests
    {
        private const int Id = 7;

        private static GHAndroidGamepadDevice NewDevice(GHAndroidAxisLayout axes, bool analogTriggers)
        {
            GHAndroidDeviceLayout layout = new GHAndroidDeviceLayout();
            layout.Axes = axes;
            layout.AnalogTriggers = analogTriggers;
            return new GHAndroidGamepadDevice(Id, layout);
        }

        private static GHAndroidAxisSample Hat(float x, float y)
        {
            GHAndroidAxisSample sample = new GHAndroidAxisSample();
            sample.HatX = x;
            sample.HatY = y;
            return sample;
        }

        private static GHAndroidAxisSample LeftStick(float x, float y)
        {
            GHAndroidAxisSample sample = new GHAndroidAxisSample();
            sample.X = x;
            sample.Y = y;
            return sample;
        }

        private static GHAndroidAxisSample Triggers(float left, float right)
        {
            GHAndroidAxisSample sample = new GHAndroidAxisSample();
            sample.LTrigger = left;
            sample.RTrigger = right;
            return sample;
        }

        private static List<string> Buttons(GHRecordingSink sink)
        {
            return sink.Calls.Where(c => c.StartsWith("button", StringComparison.Ordinal)).ToList();
        }

        [Fact]
        public void Tap_BetweenPumps_ProducesBothEdges()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonA, true, 0, 10));
            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonA, false, 0, 20));
            Assert.Empty(sink.Calls);
            device.Pump(sink);

            Assert.Equal(new[] { "button 7 A down", "button 7 A up" }, sink.Calls);
        }

        [Fact]
        public void RepeatedDown_IsRepeatAndProducesNothing()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnKey(GHAndroidGamepadMapping.KeycodeButtonB, true, 0, 0);
            Assert.Equal(GHAndroidKeyResult.Repeat, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonB, true, 1, 500));
            Assert.Equal(GHAndroidKeyResult.Repeat, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonB, true, 0, 550));
            device.Pump(sink);

            Assert.Equal(new[] { "button 7 B down" }, sink.Calls);
        }

        [Fact]
        public void ButtonMode_IsNotMapped()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            Assert.Equal(GHAndroidKeyResult.NotMapped, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonMode, true, 0, 0));
            device.Pump(sink);
            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void UpWithoutDown_IsUnmatched()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            Assert.Equal(GHAndroidKeyResult.UnmatchedUp, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonX, false, 0, 0));
            device.Pump(sink);
            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void KeyThenHat_OnTheSameDirection_PressAndReleaseOnce()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeDpadRight, true, 0, 0));
            device.OnAxes(Hat(1f, 0f), 1);
            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeDpadRight, false, 0, 2));
            device.OnAxes(Hat(0f, 0f), 3);
            device.Pump(sink);

            Assert.Equal(new[] { "button 7 DPadRight down", "button 7 DPadRight up" }, sink.Calls);
        }

        [Fact]
        public void HatThenKey_OnTheSameDirection_PressAndReleaseOnce()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(Hat(0f, 1f), 0);
            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeDpadDown, true, 0, 1));
            device.OnAxes(Hat(0f, 0f), 2);
            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeDpadDown, false, 0, 3));
            device.Pump(sink);

            Assert.Equal(new[] { "button 7 DPadDown down", "button 7 DPadDown up" }, sink.Calls);
        }

        [Fact]
        public void HatSide_PressesAtHalfAndReleasesBelowThreshold()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(Hat(0f, -0.4f), 0);
            device.OnAxes(Hat(0f, -0.5f), 1);
            device.OnAxes(Hat(0f, -0.35f), 2);
            device.OnAxes(Hat(0f, -0.2f), 3);
            device.OnAxes(Hat(0f, -0.4f), 4);
            device.Pump(sink);

            Assert.Equal(new[] { "button 7 DPadUp down", "button 7 DPadUp up" }, sink.Calls);
        }

        [Fact]
        public void DPadKeyDown_WhileTheHatHoldsTheDirection_QueuesNothing()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(Hat(1f, 0f), 0);
            device.Pump(sink);
            sink.Calls.Clear();

            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeDpadRight, true, 0, 1));
            device.Pump(sink);
            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void TriggerKeys_AreIgnoredWithAnalogTriggers()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, true);

            Assert.Equal(GHAndroidKeyResult.Ignored, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonL2, true, 0, 0));
            Assert.Equal(GHAndroidKeyResult.Ignored, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonR2, true, 0, 0));
            device.Pump(sink);
            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void TriggerKeys_AreButtonsWithoutAnalogTriggers()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonL2, true, 0, 0));
            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonR2, true, 0, 0));
            device.Pump(sink);
            Assert.Equal(new[] { "button 7 LT down", "button 7 RT down" }, sink.Calls);
        }

        [Fact]
        public void Standard_RightStickFromZRz_TriggersTakeTheLargerAxis()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, true);

            GHAndroidAxisSample sample = new GHAndroidAxisSample();
            sample.Z = 0.5f;
            sample.Rz = -0.25f;
            sample.Rx = 0.9f;
            sample.LTrigger = 0.2f;
            sample.Brake = 0.6f;
            sample.RTrigger = 0.9f;
            sample.Gas = 0.1f;
            device.OnAxes(sample, 0);
            device.Pump(sink);

            Assert.Equal(new[] { "stick 7 1 0.500 -0.250", "trigger 7 L 0.600", "trigger 7 R 0.900" }, sink.Calls);
        }

        [Fact]
        public void RxRyStick_RightStickFromRxRy()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.RxRyStick, true);

            GHAndroidAxisSample sample = new GHAndroidAxisSample();
            sample.Z = 0.9f;
            sample.Rz = 0.9f;
            sample.Rx = 0.5f;
            sample.Ry = -0.25f;
            sample.LTrigger = 0.3f;
            device.OnAxes(sample, 0);
            device.Pump(sink);

            Assert.Equal(new[] { "stick 7 1 0.500 -0.250", "trigger 7 L 0.300" }, sink.Calls);
        }

        [Fact]
        public void LinuxGamepad_RightStickFromRxRy_TriggersFromZRzRescaled()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.LinuxGamepad, true);

            GHAndroidAxisSample sample = new GHAndroidAxisSample();
            sample.Z = -1f;
            sample.Rz = -1f;
            sample.Rx = 0.5f;
            sample.Ry = 0.25f;
            sample.LTrigger = 0.8f;
            device.OnAxes(sample, 0);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 1 0.500 0.250" }, sink.Calls);

            sink.Calls.Clear();
            sample.Z = 1f;
            device.OnAxes(sample, 1);
            device.Pump(sink);
            Assert.Equal(new[] { "trigger 7 L 1.000" }, sink.Calls);

            sink.Calls.Clear();
            sample.Z = -1f;
            device.OnAxes(sample, 2);
            device.Pump(sink);
            Assert.Equal(new[] { "trigger 7 L 0.000" }, sink.Calls);
        }

        [Fact]
        public void OnlyChangedSticks_AreEmitted()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(LeftStick(0.5f, 0f), 0);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 0 0.500 0.000" }, sink.Calls);

            sink.Calls.Clear();
            GHAndroidAxisSample sample = LeftStick(0.5f, 0f);
            sample.Z = 0.3f;
            device.OnAxes(sample, 1);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 1 0.300 0.000" }, sink.Calls);

            sink.Calls.Clear();
            device.OnAxes(sample, 2);
            device.Pump(sink);
            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void Resync_KeyHeld_UpIsSilentAndTheNextPressIsNormal()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnKey(GHAndroidGamepadMapping.KeycodeButtonA, true, 0, 0);
            device.OnKey(GHAndroidGamepadMapping.KeycodeButtonY, true, 0, 1);
            device.OnKey(GHAndroidGamepadMapping.KeycodeButtonY, false, 0, 2);
            device.Resync(sink, 10);
            Assert.Equal(new[] { "stick 7 0 0.000 0.000", "stick 7 1 0.000 0.000" }, sink.Calls);
            sink.Calls.Clear();
            device.Pump(sink);
            Assert.Empty(sink.Calls);

            Assert.Equal(GHAndroidKeyResult.Repeat, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonA, true, 3, 20));
            Assert.Equal(GHAndroidKeyResult.Suppressed, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonA, false, 0, 30));
            device.Pump(sink);
            Assert.Empty(sink.Calls);

            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonA, true, 0, 40));
            device.Pump(sink);
            Assert.Equal(new[] { "button 7 A down" }, sink.Calls);
        }

        [Fact]
        public void Resync_LeftStickHeld_EmitsItsValue()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(LeftStick(0.8f, 0f), 0);
            device.Pump(sink);
            sink.Calls.Clear();

            device.Resync(sink, 10);
            Assert.Equal(new[] { "stick 7 0 0.800 0.000", "stick 7 1 0.000 0.000" }, sink.Calls);

            sink.Calls.Clear();
            device.Pump(sink);
            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void Resync_TriggerHeld_EmitsItsValue()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, true);

            device.OnAxes(Triggers(0.7f, 0f), 0);
            device.Pump(sink);
            sink.Calls.Clear();

            device.Resync(sink, 10);
            Assert.Equal(new[] { "stick 7 0 0.000 0.000", "stick 7 1 0.000 0.000", "trigger 7 L 0.700", "trigger 7 R 0.000" },
                sink.Calls);
        }

        [Fact]
        public void Resync_HatHeld_StaysSilentUntilReleased()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(Hat(1f, 0f), 0);
            device.Pump(sink);
            device.Resync(sink, 10);
            sink.Calls.Clear();

            device.OnAxes(Hat(1f, 0f), 11);
            device.OnAxes(Hat(0f, 0f), 12);
            device.Pump(sink);
            Assert.Empty(sink.Calls);

            device.OnAxes(Hat(1f, 0f), 13);
            device.Pump(sink);
            Assert.Equal(new[] { "button 7 DPadRight down" }, sink.Calls);
        }

        [Fact]
        public void InputLost_LeftStickHeld_StaysNeutralUntilInsideTheReleaseThreshold()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(LeftStick(0.8f, 0f), 0);
            device.Pump(sink);
            sink.Calls.Clear();

            device.InputLost(10);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 0 0.000 0.000", "stick 7 1 0.000 0.000" }, sink.Calls);

            sink.Calls.Clear();
            device.OnAxes(LeftStick(0.7f, 0f), 11);
            device.Pump(sink);
            Assert.Empty(sink.Calls);

            device.OnAxes(LeftStick(0.2f, 0f), 12);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 0 0.200 0.000" }, sink.Calls);

            sink.Calls.Clear();
            device.OnAxes(LeftStick(0.9f, 0f), 13);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 0 0.900 0.000" }, sink.Calls);
        }

        [Fact]
        public void InputLost_TriggerHeld_StaysNeutralUntilInsideTheReleaseThreshold()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, true);

            device.OnAxes(Triggers(0f, 0.9f), 0);
            device.Pump(sink);
            sink.Calls.Clear();

            device.InputLost(10);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 0 0.000 0.000", "stick 7 1 0.000 0.000", "trigger 7 L 0.000", "trigger 7 R 0.000" },
                sink.Calls);

            sink.Calls.Clear();
            device.OnAxes(Triggers(0f, 0.8f), 11);
            device.Pump(sink);
            Assert.Empty(sink.Calls);

            device.OnAxes(Triggers(0f, 0.1f), 12);
            device.Pump(sink);
            Assert.Equal(new[] { "trigger 7 R 0.100" }, sink.Calls);

            sink.Calls.Clear();
            device.OnAxes(Triggers(0f, 0.7f), 13);
            device.Pump(sink);
            Assert.Equal(new[] { "trigger 7 R 0.700" }, sink.Calls);
        }

        [Fact]
        public void InputLost_LeftStickAtRest_NextDeflectionIsEmittedAtOnce()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(LeftStick(0f, 0f), 0);
            device.InputLost(10);
            device.Pump(sink);
            sink.Calls.Clear();

            device.OnAxes(LeftStick(0.9f, 0f), 11);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 0 0.900 0.000" }, sink.Calls);
        }

        [Fact]
        public void InputLost_ThenResyncWhileDeflected_EmitsZeroAndKeepsTheLatch()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(LeftStick(0.8f, 0f), 0);
            device.Pump(sink);
            device.InputLost(10);
            device.Pump(sink);
            sink.Calls.Clear();

            device.OnAxes(LeftStick(0.8f, 0f), 11);
            device.Resync(sink, 12);
            Assert.Equal(new[] { "stick 7 0 0.000 0.000", "stick 7 1 0.000 0.000" }, sink.Calls);

            sink.Calls.Clear();
            device.OnAxes(LeftStick(0.8f, 0f), 13);
            device.Pump(sink);
            Assert.Empty(sink.Calls);

            device.OnAxes(LeftStick(0.1f, 0f), 14);
            device.Pump(sink);
            Assert.Equal(new[] { "stick 7 0 0.100 0.000" }, sink.Calls);
        }

        [Fact]
        public void InputLost_HatHeld_ReleasesItAndTheNextPushIsAPress()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(Hat(1f, 0f), 0);
            device.Pump(sink);
            sink.Calls.Clear();

            device.InputLost(10);
            device.Pump(sink);
            Assert.Equal(new[] { "button 7 DPadRight up", "stick 7 0 0.000 0.000", "stick 7 1 0.000 0.000" }, sink.Calls);

            sink.Calls.Clear();
            device.OnAxes(Hat(1f, 0f), 11);
            device.Pump(sink);
            Assert.Equal(new[] { "button 7 DPadRight down" }, sink.Calls);
        }

        [Fact]
        public void InputLost_DPadHeldByKeyAndHat_QueuesNoUp()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnKey(GHAndroidGamepadMapping.KeycodeDpadRight, true, 0, 0);
            device.OnAxes(Hat(1f, 0f), 1);
            device.Pump(sink);
            sink.Calls.Clear();

            device.InputLost(10);
            device.Pump(sink);
            Assert.Empty(Buttons(sink));

            sink.Calls.Clear();
            Assert.Equal(GHAndroidKeyResult.Edge, device.OnKey(GHAndroidGamepadMapping.KeycodeDpadRight, false, 0, 11));
            device.Pump(sink);
            Assert.Equal(new[] { "button 7 DPadRight up" }, sink.Calls);
        }

        [Fact]
        public void InputLost_HatSuppressedByResync_QueuesNoUpAndTheNextPushIsAPress()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnAxes(Hat(1f, 0f), 0);
            device.Pump(sink);
            device.Resync(sink, 5);
            sink.Calls.Clear();

            device.InputLost(10);
            device.Pump(sink);
            Assert.Empty(Buttons(sink));

            sink.Calls.Clear();
            device.OnAxes(Hat(1f, 0f), 11);
            device.Pump(sink);
            Assert.Equal(new[] { "button 7 DPadRight down" }, sink.Calls);
        }

        [Fact]
        public void Clear_ForgetsHeldButtons()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHAndroidGamepadDevice device = NewDevice(GHAndroidAxisLayout.Standard, false);

            device.OnKey(GHAndroidGamepadMapping.KeycodeButtonA, true, 0, 0);
            device.Clear();
            Assert.Equal(GHAndroidKeyResult.UnmatchedUp, device.OnKey(GHAndroidGamepadMapping.KeycodeButtonA, false, 0, 1));
            device.Pump(sink);
            Assert.Empty(sink.Calls);
        }
    }
}
