using System;

namespace GnollHackX
{
    /* A polled controller state (the physical buttons, two sticks, two triggers) turned into sink calls.
       Set* updates the pending state; Commit emits the button edges in button order, then one Stick per
       stick and one Trigger per trigger that changed, and makes the pending state the committed one. */
    public sealed class GHGamepadStateDiffer
    {
        private const int PhysicalButtonCount = (int)GHGamepadButton.LeftStickUp;

        private readonly bool _hasTriggers;
        private uint _pendingButtons;
        private uint _committedButtons;
        private readonly float[] _pendingAxes = new float[6];
        private readonly float[] _committedAxes = new float[6];

        /* hasTriggers: the controller reports analog triggers through Trigger */
        public GHGamepadStateDiffer(bool hasTriggers)
        {
            _hasTriggers = hasTriggers;
        }

        public uint CommittedButtons { get { return _committedButtons; } }

        /* Physical buttons only; the virtual stick directions are derived by the service */
        public void SetButton(GHGamepadButton button, bool down)
        {
            if (button < 0 || (int)button >= PhysicalButtonCount)
                return;
            uint bit = 1u << (int)button;
            if (down)
                _pendingButtons |= bit;
            else
                _pendingButtons &= ~bit;
        }

        public bool IsPendingDown(GHGamepadButton button)
        {
            if (button < 0 || (int)button >= PhysicalButtonCount)
                return false;
            return (_pendingButtons & (1u << (int)button)) != 0;
        }

        /* 0 left, 1 right; -1..1 raw, +Y down */
        public void SetStick(int stick, float x, float y)
        {
            if (stick != 0 && stick != 1)
                return;
            _pendingAxes[2 * stick] = x;
            _pendingAxes[2 * stick + 1] = y;
        }

        /* 0..1 raw */
        public void SetTrigger(bool right, float value)
        {
            _pendingAxes[right ? 5 : 4] = value;
        }

        public void Commit(int controllerId, long timestamp, IGHGamepadInputSink sink)
        {
            uint changed = _pendingButtons ^ _committedButtons;
            if (changed != 0)
            {
                int i;
                for (i = 0; i < PhysicalButtonCount; i++)
                {
                    uint bit = 1u << i;
                    if ((changed & bit) != 0)
                        sink.Button(controllerId, (GHGamepadButton)i, (_pendingButtons & bit) != 0, timestamp);
                }
            }
            EmitAxes(controllerId, timestamp, sink, false);
            CopyPendingToCommitted();
        }

        /* A snapshot after open or resync: buttons become committed without edges when silentButtons is set;
           every stick and trigger value is emitted */
        public void CommitSnapshot(int controllerId, long timestamp, IGHGamepadInputSink sink, bool silentButtons)
        {
            if (silentButtons)
                _committedButtons = _pendingButtons;
            uint changed = _pendingButtons ^ _committedButtons;
            int i;
            for (i = 0; i < PhysicalButtonCount && changed != 0; i++)
            {
                uint bit = 1u << i;
                if ((changed & bit) != 0)
                    sink.Button(controllerId, (GHGamepadButton)i, (_pendingButtons & bit) != 0, timestamp);
            }
            EmitAxes(controllerId, timestamp, sink, true);
            CopyPendingToCommitted();
        }

        /* Drops the pending changes */
        public void RevertPending()
        {
            _pendingButtons = _committedButtons;
            Array.Copy(_committedAxes, _pendingAxes, _pendingAxes.Length);
        }

        private void EmitAxes(int controllerId, long timestamp, IGHGamepadInputSink sink, bool all)
        {
            int stick;
            for (stick = 0; stick < 2; stick++)
            {
                int x = 2 * stick;
                if (all || _pendingAxes[x] != _committedAxes[x] || _pendingAxes[x + 1] != _committedAxes[x + 1])
                    sink.Stick(controllerId, stick, _pendingAxes[x], _pendingAxes[x + 1], timestamp);
            }
            if (!_hasTriggers)
                return;
            if (all || _pendingAxes[4] != _committedAxes[4])
                sink.Trigger(controllerId, false, _pendingAxes[4], timestamp);
            if (all || _pendingAxes[5] != _committedAxes[5])
                sink.Trigger(controllerId, true, _pendingAxes[5], timestamp);
        }

        private void CopyPendingToCommitted()
        {
            _committedButtons = _pendingButtons;
            Array.Copy(_pendingAxes, _committedAxes, _pendingAxes.Length);
        }
    }
}
