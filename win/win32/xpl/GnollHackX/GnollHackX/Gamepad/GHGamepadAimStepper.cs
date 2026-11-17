namespace GnollHackX
{
    /* Aim and Confirm stepping. The confirm button pressed while a direction is aimed steps at once and, held,
       again after the first delay and then at the interval; a press that does not repeat (a run) steps once. A new
       aim restarts the first delay without a step. Releasing the button or losing the aim ends the hold, and a
       press with nothing aimed holds nothing. Times are in ticks of any unit. Free of UI types, so that the unit
       tests compile it. */
    public sealed class GHGamepadAimStepper
    {
        private readonly long _firstDelayTicks;
        private readonly long _intervalTicks;
        private int _sector = -1;
        private bool _held;
        private bool _repeats;
        private long _nextStepTicks;

        public GHGamepadAimStepper(long firstDelayTicks, long intervalTicks)
        {
            _firstDelayTicks = firstDelayTicks;
            _intervalTicks = intervalTicks;
        }

        /* 0-7 clockwise from north; -1 while nothing is aimed */
        public int Sector { get { return _sector; } }
        public bool IsAiming { get { return _sector >= 0; } }
        public bool IsConfirmHeld { get { return _held; } }

        /* sector -1, or outside 0-7, for none; true when the aim changed */
        public bool SetAim(int sector, long now)
        {
            if (sector < 0 || sector > 7)
                sector = -1;
            if (sector == _sector)
                return false;
            _sector = sector;
            if (sector < 0)
                _held = false;
            else if (_held)
                _nextStepTicks = now + _firstDelayTicks;
            return true;
        }

        /* The confirm button went down; true when it steps now */
        public bool PressConfirm(long now, bool repeats)
        {
            if (_sector < 0)
            {
                _held = false;
                return false;
            }
            _held = true;
            _repeats = repeats;
            _nextStepTicks = now + _firstDelayTicks;
            return true;
        }

        public void ReleaseConfirm()
        {
            _held = false;
        }

        /* A held, repeating press whose next step is due */
        public bool IsStepDue(long now)
        {
            return _held && _repeats && _sector >= 0 && now >= _nextStepTicks;
        }

        /* The due step was sent */
        public void NoteStepSent(long now)
        {
            _nextStepTicks = now + _intervalTicks;
        }

        public void Reset()
        {
            _sector = -1;
            _held = false;
            _repeats = false;
            _nextStepTicks = 0;
        }
    }
}
