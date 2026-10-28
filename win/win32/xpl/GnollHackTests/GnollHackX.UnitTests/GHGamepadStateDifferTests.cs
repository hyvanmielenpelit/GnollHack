using System;
using System.Collections.Generic;
using System.Globalization;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Records sink calls as text, and the last stick and trigger values as numbers */
    internal sealed class GHRecordingSink : IGHGamepadInputSink
    {
        public readonly List<string> Calls = new List<string>();
        public readonly List<GHGamepadControllerInfo> Added = new List<GHGamepadControllerInfo>();
        public readonly float[] LastStick = new float[4];
        public readonly float[] LastTrigger = new float[2];

        public void ControllerAdded(GHGamepadControllerInfo info)
        {
            Added.Add(info);
            Calls.Add("added " + info.Id);
        }

        public void ControllerRemoved(int controllerId)
        {
            Calls.Add("removed " + controllerId);
        }

        public void Button(int controllerId, GHGamepadButton button, bool down, long timestamp)
        {
            Calls.Add("button " + controllerId + " " + button + (down ? " down" : " up"));
        }

        public void Stick(int controllerId, int stick, float x, float y, long timestamp)
        {
            LastStick[2 * stick] = x;
            LastStick[2 * stick + 1] = y;
            Calls.Add("stick " + controllerId + " " + stick + " " + x.ToString("F3", CultureInfo.InvariantCulture)
                + " " + y.ToString("F3", CultureInfo.InvariantCulture));
        }

        public void Trigger(int controllerId, bool right, float value, long timestamp)
        {
            LastTrigger[right ? 1 : 0] = value;
            Calls.Add("trigger " + controllerId + (right ? " R " : " L ") + value.ToString("F3", CultureInfo.InvariantCulture));
        }
    }

    /* Covers GHGamepadStateDiffer: edges in button order, changed sticks and triggers only, snapshots */
    public class GHGamepadStateDifferTests
    {
        [Fact]
        public void Commit_EmitsEdgesInButtonOrderThenChangedAxes()
        {
            GHGamepadStateDiffer differ = new GHGamepadStateDiffer(true);
            GHRecordingSink sink = new GHRecordingSink();
            differ.SetButton(GHGamepadButton.Menu, true);
            differ.SetButton(GHGamepadButton.A, true);
            differ.SetStick(1, 0.5f, -0.25f);
            differ.SetTrigger(true, 1f);

            differ.Commit(3, 100, sink);

            Assert.Equal(new[] { "button 3 A down", "button 3 Menu down", "stick 3 1 0.500 -0.250", "trigger 3 R 1.000" }, sink.Calls);
        }

        [Fact]
        public void Commit_UnchangedStateEmitsNothing()
        {
            GHGamepadStateDiffer differ = new GHGamepadStateDiffer(true);
            GHRecordingSink sink = new GHRecordingSink();
            differ.SetButton(GHGamepadButton.B, true);
            differ.SetStick(0, 0.1f, 0.2f);
            differ.Commit(1, 0, sink);
            sink.Calls.Clear();

            differ.SetButton(GHGamepadButton.B, true);
            differ.SetStick(0, 0.1f, 0.2f);
            differ.Commit(1, 1, sink);

            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void Commit_ReleaseIsAnUpEdge()
        {
            GHGamepadStateDiffer differ = new GHGamepadStateDiffer(false);
            GHRecordingSink sink = new GHRecordingSink();
            differ.SetButton(GHGamepadButton.X, true);
            differ.Commit(1, 0, sink);
            differ.SetButton(GHGamepadButton.X, false);
            differ.Commit(1, 1, sink);

            Assert.Equal(new[] { "button 1 X down", "button 1 X up" }, sink.Calls);
        }

        [Fact]
        public void Triggers_OnlyWhenTheControllerHasThem()
        {
            GHGamepadStateDiffer differ = new GHGamepadStateDiffer(false);
            GHRecordingSink sink = new GHRecordingSink();
            differ.SetTrigger(false, 0.8f);
            differ.Commit(1, 0, sink);

            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void VirtualButtons_AreNotStored()
        {
            GHGamepadStateDiffer differ = new GHGamepadStateDiffer(false);
            GHRecordingSink sink = new GHRecordingSink();
            differ.SetButton(GHGamepadButton.LeftStickUp, true);
            differ.Commit(1, 0, sink);

            Assert.Empty(sink.Calls);
            Assert.False(differ.IsPendingDown(GHGamepadButton.LeftStickUp));
        }

        [Fact]
        public void SilentSnapshot_NoButtonEdgesButEveryAxis()
        {
            GHGamepadStateDiffer differ = new GHGamepadStateDiffer(true);
            GHRecordingSink sink = new GHRecordingSink();
            differ.SetButton(GHGamepadButton.A, true);
            differ.SetTrigger(false, 0.9f);

            differ.CommitSnapshot(1, 0, sink, true);

            Assert.Equal(new[] { "stick 1 0 0.000 0.000", "stick 1 1 0.000 0.000", "trigger 1 L 0.900", "trigger 1 R 0.000" }, sink.Calls);
            sink.Calls.Clear();
            differ.SetButton(GHGamepadButton.A, false);
            differ.Commit(1, 1, sink);
            Assert.Equal(new[] { "button 1 A up" }, sink.Calls);
        }

        [Fact]
        public void LoudSnapshot_EmitsTheDifferences()
        {
            GHGamepadStateDiffer differ = new GHGamepadStateDiffer(false);
            GHRecordingSink sink = new GHRecordingSink();
            differ.SetButton(GHGamepadButton.LB, true);
            differ.Commit(1, 0, sink);
            sink.Calls.Clear();

            differ.SetButton(GHGamepadButton.LB, false);
            differ.SetButton(GHGamepadButton.RB, true);
            differ.CommitSnapshot(1, 1, sink, false);

            Assert.Equal(new[] { "button 1 LB up", "button 1 RB down", "stick 1 0 0.000 0.000", "stick 1 1 0.000 0.000" }, sink.Calls);
        }

        [Fact]
        public void RevertPending_DropsChanges()
        {
            GHGamepadStateDiffer differ = new GHGamepadStateDiffer(true);
            GHRecordingSink sink = new GHRecordingSink();
            differ.SetButton(GHGamepadButton.Y, true);
            differ.SetStick(0, 1f, 0f);
            differ.RevertPending();
            differ.Commit(1, 0, sink);

            Assert.Empty(sink.Calls);
        }
    }
}
