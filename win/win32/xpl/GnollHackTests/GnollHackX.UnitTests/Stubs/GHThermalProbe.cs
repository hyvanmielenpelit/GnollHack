using System;

namespace GnollHackX.Perf
{
    /* Coarse device thermal state, shared across platforms. Android maps its
       PowerManager thermal status one-to-one; iOS maps Nominal/Fair/Serious/Critical
       to Nominal/Light/Moderate/Critical; Windows has no user-mode thermal API and
       stays Unknown. A run measured at a status above Moderate is throttled. */
    public enum GHThermalStatus
    {
        Unknown = 0,
        Nominal = 1,
        Light = 2,
        Moderate = 3,
        Severe = 4,
        Critical = 5
    }

    /* One sample of the device's thermal and power state. Numeric fields are
       float.NaN when the platform does not expose them. */
    public struct GHThermalReading
    {
        public GHThermalStatus Status;
        public float HeadroomFraction;   /* 0..1+, NaN when unavailable (Android API 30+ GetThermalHeadroom) */
        public float BatteryTempC;       /* NaN when unavailable */
        public float CpuPerformancePct;  /* Windows: actual/nominal CPU frequency percent; NaN elsewhere */
        public bool IsCharging;
        public bool IsLowPower;
        public long TimestampTicks;      /* DateTime.UtcNow.Ticks */
        public string Detail;            /* short free text, may be null */
    }

    /* Test-side stand-in for the app's GHThermalProbe. The real one reads through
       the platform service; this one returns whatever a test placed in Reading, and
       defaults to Unknown. The enum and struct above are copies of the app's. */
    public static class GHThermalProbe
    {
        private static readonly string[] _statusNames = new string[]
        {
            "Unknown", "Nominal", "Light", "Moderate", "Severe", "Critical"
        };

        /* The reading Read returns; tests may replace it */
        public static GHThermalReading Reading = Unknown;

        /* A reading with Status Unknown and NaN numeric fields */
        public static GHThermalReading Unknown
        {
            get
            {
                GHThermalReading r = new GHThermalReading();
                r.Status = GHThermalStatus.Unknown;
                r.HeadroomFraction = float.NaN;
                r.BatteryTempC = float.NaN;
                r.CpuPerformancePct = float.NaN;
                r.IsCharging = false;
                r.IsLowPower = false;
                r.TimestampTicks = DateTime.UtcNow.Ticks;
                r.Detail = null;
                return r;
            }
        }

        public static GHThermalReading Read()
        {
            GHThermalReading r = Reading;
            if (r.TimestampTicks == 0)
                r.TimestampTicks = DateTime.UtcNow.Ticks;
            return r;
        }

        public static string StatusName(GHThermalStatus status)
        {
            int i = (int)status;
            if (i < 0 || i >= _statusNames.Length)
                return _statusNames[0];
            return _statusNames[i];
        }
    }
}
