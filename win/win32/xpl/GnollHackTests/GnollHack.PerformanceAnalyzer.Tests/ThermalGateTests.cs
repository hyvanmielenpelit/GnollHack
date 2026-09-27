using GnollHack.PerformanceAnalyzer.Model;
using Xunit;

namespace GnollHack.PerformanceAnalyzer.Tests
{
    public class ThermalGateTests
    {
        private static RunRecord RunWith(ThermalReading before, ThermalReading after)
        {
            RunRecord r = new RunRecord();
            r.Thermal.Before = before;
            r.Thermal.After = after;
            return r;
        }

        [Theory]
        [InlineData("Severe")]
        [InlineData("Critical")]
        public void Apply_StatusAboveModerate_Throttles(string afterStatus)
        {
            RunRecord r = RunWith(new ThermalReading { Status = "Nominal" }, new ThermalReading { Status = afterStatus });
            ThermalGate.Apply(r);
            Assert.True(r.Thermal.Throttled);
            Assert.Equal("status", r.Thermal.GateSignal);
        }

        [Fact]
        public void Apply_WindowsBefore95After60_ThrottlesWithThePowerPlanNamed()
        {
            RunRecord r = RunWith(
                new ThermalReading { Status = "Nominal", CpuPerformancePct = 95, PowerPlan = "Balanced" },
                new ThermalReading { Status = "Nominal", CpuPerformancePct = 60 });
            ThermalGate.Apply(r);
            Assert.True(r.Thermal.Throttled);
            Assert.Equal("cpuPerformancePct", r.Thermal.GateSignal);
            Assert.Contains("Balanced", r.Thermal.ThrottleReason);
        }

        [Fact]
        public void Apply_IdleBalancedBefore60After58_DoesNotThrottle()
        {
            RunRecord r = RunWith(
                new ThermalReading { Status = "Nominal", CpuPerformancePct = 60, PowerPlan = "Balanced" },
                new ThermalReading { Status = "Nominal", CpuPerformancePct = 58, PowerPlan = "Balanced" });
            ThermalGate.Apply(r);
            Assert.False(r.Thermal.Throttled);
        }

        [Fact]
        public void Apply_OnlyABeforeReadingUnder90_DoesNotThrottle()
        {
            RunRecord r = RunWith(new ThermalReading { Status = "Nominal", CpuPerformancePct = 60, PowerPlan = "Balanced" }, null);
            ThermalGate.Apply(r);
            Assert.False(r.Thermal.Throttled);
        }

        [Fact]
        public void Apply_AfterUnder90WithNoBefore_Throttles()
        {
            RunRecord r = RunWith(null, new ThermalReading { Status = "Nominal", CpuPerformancePct = 60, PowerPlan = "Balanced" });
            ThermalGate.Apply(r);
            Assert.True(r.Thermal.Throttled);
            Assert.Equal("cpuPerformancePct", r.Thermal.GateSignal);
            Assert.Contains("Balanced", r.Thermal.ThrottleReason);
        }
    }
}
