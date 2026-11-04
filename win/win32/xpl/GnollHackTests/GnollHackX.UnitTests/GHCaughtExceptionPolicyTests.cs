using System;
using System.Threading.Tasks;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHCaughtExceptionPolicy: the source name from either path separator and the .xaml.cs and .cs
       suffixes, cancellations found through AggregateException, and one report per source and exception
       type up to MaxReportedKeys. */
    public class GHCaughtExceptionPolicyTests
    {
        [Theory]
        [InlineData(@"C:\hmp\GnollHack\win\win32\xpl\GnollHackX\GnollHackX\MainPage.xaml.cs", "SettingsButton_Clicked", "MainPage.SettingsButton_Clicked")]
        [InlineData("/_/win/win32/xpl/GnollHackX/GnollHackX/GHApp.cs", "PushModalPageAsync", "GHApp.PushModalPageAsync")]
        [InlineData(@"C:\src\Pages\MainScreen\SettingsPage.Pickers.cs", "ScrollPickerListAfterLayout", "SettingsPage.Pickers.ScrollPickerListAfterLayout")]
        [InlineData("", "Handler", "Handler")]
        [InlineData(null, null, "?")]
        public void FormatSource_NamesFileAndMember(string callerFilePath, string memberName, string expected)
        {
            Assert.Equal(expected, GHCaughtExceptionPolicy.FormatSource(callerFilePath, memberName));
        }

        [Fact]
        public void IsCancellation_RecognizesCancellations()
        {
            Assert.True(GHCaughtExceptionPolicy.IsCancellation(new OperationCanceledException()));
            Assert.True(GHCaughtExceptionPolicy.IsCancellation(new TaskCanceledException()));
            Assert.True(GHCaughtExceptionPolicy.IsCancellation(
                new AggregateException(new TaskCanceledException(), new AggregateException(new OperationCanceledException()))));
        }

        [Fact]
        public void IsCancellation_RejectsEverythingElse()
        {
            Assert.False(GHCaughtExceptionPolicy.IsCancellation(null));
            Assert.False(GHCaughtExceptionPolicy.IsCancellation(new InvalidOperationException()));
            Assert.False(GHCaughtExceptionPolicy.IsCancellation(new AggregateException()));
            Assert.False(GHCaughtExceptionPolicy.IsCancellation(
                new AggregateException(new TaskCanceledException(), new System.IO.IOException())));
        }

        [Fact]
        public void TryClaimReport_OncePerSourceAndType()
        {
            GHCaughtExceptionPolicy policy = new GHCaughtExceptionPolicy();
            Assert.True(policy.TryClaimReport("MainPage.A", new InvalidOperationException("first")));
            Assert.False(policy.TryClaimReport("MainPage.A", new InvalidOperationException("second")));
            Assert.True(policy.TryClaimReport("MainPage.A", new System.IO.IOException()));
            Assert.True(policy.TryClaimReport("MainPage.B", new InvalidOperationException()));
        }

        [Fact]
        public void TryClaimReport_StopsAtMaxReportedKeys()
        {
            GHCaughtExceptionPolicy policy = new GHCaughtExceptionPolicy();
            int i;
            for (i = 0; i < GHCaughtExceptionPolicy.MaxReportedKeys; i++)
                Assert.True(policy.TryClaimReport("Source" + i, new InvalidOperationException()));
            Assert.False(policy.TryClaimReport("OneMore", new InvalidOperationException()));
        }
    }
}
