using System.Runtime.CompilerServices;
using GnollHackM;
using Sentry;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Binds GHSentryOptionsInternals against the Sentry package at GnollHackM's GnhSentryVersion */
    public class GHSentryOptionsInternalsTests
    {
        private sealed class RecordingObserver : IScopeObserver
        {
            public readonly List<string> Breadcrumbs = new List<string>();
            public readonly Dictionary<string, string> Tags = new Dictionary<string, string>();
            public void AddBreadcrumb(Breadcrumb breadcrumb) { lock (Breadcrumbs) Breadcrumbs.Add(breadcrumb.Message); }
            public void SetExtra(string key, object value) { }
            public void SetTag(string key, string value) { lock (Tags) Tags[key] = value; }
            public void UnsetTag(string key) { }
            public void SetUser(SentryUser user) { }
            public void SetTrace(SentryId traceId, SpanId parentSpanId) { }
            public void SetEnvironment(string environment) { }
            public void AddAttachment(SentryAttachment attachment) { }
            public void ClearAttachments() { }
        }

        [UnsafeAccessor(UnsafeAccessorKind.Method, Name = "get_CrashedLastRun")]
        private static extern Func<bool> GetCrashedLastRun(SentryOptions options);

        [Fact]
        public void TrySetNativeHooks_SetsCrashedLastRun()
        {
            SentryOptions options = new SentryOptions();
            Func<bool> crashedLastRun = () => true;

            Assert.True(GHSentryOptionsInternals.TrySetNativeHooks(options, new RecordingObserver(), crashedLastRun));
            Assert.Null(GHSentryOptionsInternals.Failure);
            Assert.Same(crashedLastRun, GetCrashedLastRun(options));
        }

        [Fact]
        public void TrySetNativeHooks_ObserverReceivesScopeChanges()
        {
            RecordingObserver observer = new RecordingObserver();
            using (SentrySdk.Init(options =>
            {
                options.Dsn = "https://00000000000000000000000000000000@o0.ingest.invalid/1";
                options.IsGlobalModeEnabled = true;
                options.AutoSessionTracking = false;
                Assert.True(GHSentryOptionsInternals.TrySetNativeHooks(options, observer, () => false));
            }))
            {
                SentrySdk.AddBreadcrumb("gnh-breadcrumb");
                SentrySdk.ConfigureScope(scope => scope.SetTag("gnh-tag", "1"));
            }

            Assert.Contains("gnh-breadcrumb", observer.Breadcrumbs);
            Assert.Equal("1", observer.Tags["gnh-tag"]);
        }

        [Fact]
        public void TrySetNativeHooks_NullOptions_ReportsFailure()
        {
            Assert.False(GHSentryOptionsInternals.TrySetNativeHooks(null, new RecordingObserver(), () => false));
            Assert.NotNull(GHSentryOptionsInternals.Failure);
        }
    }
}
