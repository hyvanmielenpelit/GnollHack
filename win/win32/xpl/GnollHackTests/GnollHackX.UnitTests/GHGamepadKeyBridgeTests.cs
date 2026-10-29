using System;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHGamepadKeyBridge: edges, the autorepeat measured on the Deck (repeated KeyDown without KeyUp),
       a KeyUp without a KeyDown, unowned codes, Resync suppression and the auxiliary controller it adds */
    public class GHGamepadKeyBridgeTests
    {
        private const int F2 = 102;
        private const int F3 = 103;
        private const int F4 = 104;
        private const int F5 = 105;
        private const int Escape = 27;

        private static GHGamepadKeyBridge NewBridge(GHRecordingSink sink)
        {
            GHGamepadKeyBridge bridge = new GHGamepadKeyBridge(new[] { F2, F3, F4, F5 },
                new[] { GHGamepadButton.L4, GHGamepadButton.L5, GHGamepadButton.R4, GHGamepadButton.R5 });
            string error;
            Assert.True(bridge.Start(sink, out error));
            sink.Calls.Clear();
            return bridge;
        }

        [Fact]
        public void Start_AddsTheAuxiliaryGripController()
        {
            GHRecordingSink sink = new GHRecordingSink();
            NewBridge(sink);

            GHGamepadControllerInfo info = sink.Added[0];
            Assert.Equal(1000, info.Id);
            Assert.True(info.IsAuxiliary);
            Assert.True(info.Capabilities.HasGrips);
            Assert.False(info.Capabilities.HasAnalogSticks);
        }

        [Fact]
        public void Edges_ArriveOnPump()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHGamepadKeyBridge bridge = NewBridge(sink);

            Assert.Equal(GHGamepadKeyBridgeResult.Edge, bridge.OnKey(F2, true, 10));
            Assert.Equal(GHGamepadKeyBridgeResult.Edge, bridge.OnKey(F2, false, 20));
            Assert.Empty(sink.Calls);
            bridge.Pump(30);

            Assert.Equal(new[] { "button 1000 L4 down", "button 1000 L4 up" }, sink.Calls);
        }

        [Fact]
        public void Autorepeat_IsIgnored()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHGamepadKeyBridge bridge = NewBridge(sink);

            bridge.OnKey(F5, true, 0);
            Assert.Equal(GHGamepadKeyBridgeResult.Repeat, bridge.OnKey(F5, true, 600));
            Assert.Equal(GHGamepadKeyBridgeResult.Repeat, bridge.OnKey(F5, true, 640));
            bridge.OnKey(F5, false, 700);
            bridge.Pump(800);

            Assert.Equal(new[] { "button 1000 R5 down", "button 1000 R5 up" }, sink.Calls);
        }

        [Fact]
        public void UpWithoutDown_IsIgnored()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHGamepadKeyBridge bridge = NewBridge(sink);

            Assert.Equal(GHGamepadKeyBridgeResult.UnmatchedUp, bridge.OnKey(F3, false, 0));
            bridge.Pump(1);
            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void UnownedCodes_AreNotTaken()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHGamepadKeyBridge bridge = NewBridge(sink);

            Assert.False(bridge.Owns(Escape));
            Assert.True(bridge.Owns(F4));
            Assert.Equal(GHGamepadKeyBridgeResult.NotOwned, bridge.OnKey(Escape, false, 0));
            bridge.Pump(1);
            Assert.Empty(sink.Calls);
        }

        [Fact]
        public void Resync_DropsQueuedEdgesAndSuppressesHeldKeys()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHGamepadKeyBridge bridge = NewBridge(sink);

            bridge.OnKey(F2, true, 0);
            bridge.OnKey(F3, true, 0);
            bridge.OnKey(F3, false, 5);
            bridge.Resync(10);
            bridge.Pump(11);
            Assert.Empty(sink.Calls);

            /* F2 is still held: silent until its KeyUp, then normal again */
            Assert.Equal(GHGamepadKeyBridgeResult.Repeat, bridge.OnKey(F2, true, 20));
            Assert.Equal(GHGamepadKeyBridgeResult.Suppressed, bridge.OnKey(F2, false, 30));
            bridge.Pump(31);
            Assert.Empty(sink.Calls);

            bridge.OnKey(F2, true, 40);
            bridge.Pump(41);
            Assert.Equal(new[] { "button 1000 L4 down" }, sink.Calls);
        }

        [Fact]
        public void Stop_RemovesTheController()
        {
            GHRecordingSink sink = new GHRecordingSink();
            GHGamepadKeyBridge bridge = NewBridge(sink);
            bridge.OnKey(F4, true, 0);
            bridge.Stop();
            bridge.Pump(1);
            Assert.Equal(new[] { "removed 1000" }, sink.Calls);
        }

        [Fact]
        public void Constructor_RejectsMismatchedArrays()
        {
            Assert.Throws<ArgumentException>(() => new GHGamepadKeyBridge(new[] { F2 }, new GHGamepadButton[0]));
        }
    }
}
