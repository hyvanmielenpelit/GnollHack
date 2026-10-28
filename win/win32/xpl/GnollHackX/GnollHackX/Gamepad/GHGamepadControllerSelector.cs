using System;
using System.Collections.Generic;

namespace GnollHackX
{
    /* The registered controllers and which one is active. The last controller with a button press or a
       virtual-direction press wins; stick drift does not count. An auxiliary controller never becomes
       active and never causes a switch; its buttons belong to the active controller. */
    public sealed class GHGamepadControllerSelector
    {
        private readonly List<GHGamepadControllerInfo> _controllers = new List<GHGamepadControllerInfo>();
        private int _activeId = -1;

        public int Count { get { return _controllers.Count; } }

        /* -1 when none */
        public int ActiveId { get { return _activeId; } }

        /* null when none */
        public GHGamepadControllerInfo Active { get { return Find(_activeId); } }

        /* The active controller's style; Generic when none */
        public GHGamepadStyle Style
        {
            get
            {
                GHGamepadControllerInfo active = Active;
                return active != null ? active.Style : GHGamepadStyle.Generic;
            }
        }

        /* The active controller's capabilities, with HasGrips also set by any auxiliary controller */
        public GHGamepadCapabilities Capabilities
        {
            get
            {
                GHGamepadControllerInfo active = Active;
                GHGamepadCapabilities caps = active != null ? active.Capabilities : new GHGamepadCapabilities();
                foreach (GHGamepadControllerInfo info in _controllers)
                {
                    if (info.IsAuxiliary && info.Capabilities.HasGrips)
                        caps.HasGrips = true;
                }
                return caps;
            }
        }

        public GHGamepadControllerInfo Find(int controllerId)
        {
            foreach (GHGamepadControllerInfo info in _controllers)
            {
                if (info.Id == controllerId)
                    return info;
            }
            return null;
        }

        public bool IsAuxiliary(int controllerId)
        {
            GHGamepadControllerInfo info = Find(controllerId);
            return info != null && info.IsAuxiliary;
        }

        /* A non-auxiliary controller added while none is active becomes active; a re-added id is replaced */
        public void Add(GHGamepadControllerInfo info)
        {
            if (info == null)
                return;
            GHGamepadControllerInfo existing = Find(info.Id);
            if (existing != null)
                _controllers.Remove(existing);
            _controllers.Add(info);
            if (!info.IsAuxiliary && Find(_activeId) == null)
                _activeId = info.Id;
        }

        /* True when the removed controller was active; the next active one is chosen by input or by Add */
        public bool Remove(int controllerId)
        {
            GHGamepadControllerInfo info = Find(controllerId);
            if (info == null)
                return false;
            _controllers.Remove(info);
            if (_activeId != controllerId)
                return false;
            _activeId = -1;
            return true;
        }

        /* A press on controllerId; true when it switched the active controller away from another one */
        public bool NoteActivity(int controllerId)
        {
            GHGamepadControllerInfo info = Find(controllerId);
            if (info == null || info.IsAuxiliary || controllerId == _activeId)
                return false;
            bool hadActive = Find(_activeId) != null;
            _activeId = controllerId;
            return hadActive;
        }

        /* Whether input from controllerId reaches the tracker: the active controller and auxiliary ones */
        public bool Delivers(int controllerId)
        {
            GHGamepadControllerInfo info = Find(controllerId);
            return info != null && (info.IsAuxiliary || controllerId == _activeId);
        }

        public void Clear()
        {
            _controllers.Clear();
            _activeId = -1;
        }
    }
}
