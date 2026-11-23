using System.IO;
using System.Text;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHAnrTraceSummary: thread headers, native frame parsing, wait signatures, install directory
       compaction, the stop conditions and the caps, on synthetic ANR thread dumps. */
    public class GHAnrTraceSummaryTests
    {
        private const string App = "/data/app/~~AAAA==/com.example.app-BBBB==/";
        private const string Apk = App + "split_config.arm64_v8a.apk";
        private const string Libc = "/apex/com.android.runtime/lib64/bionic/libc.so";

        private static GHAnrTraceSummary Parse(string text, int maxLines = 50000)
        {
            return GHAnrTraceSummary.Parse(new StringReader(text), maxLines);
        }

        private static string CondWaitThread(string header)
        {
            return header + "\n"
                + "  | group=\"main\" sCount=1 ucsCount=0 flags=1 obj=0x72a0 self=0xb400\n"
                + "  native: #00 pc 000000000005c0cc  " + Libc + " (syscall+28) (BuildId: 0a1b)\n"
                + "  native: #01 pc 0000000000060b40  " + Libc + " (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: 0a1b)\n"
                + "  native: #02 pc 00000000000c1b4c  " + Libc + " (pthread_cond_wait+76) (BuildId: 0a1b)\n"
                + "  native: #03 pc 00000000001f2e10  " + Apk + " (offset 0x1a2000)\n"
                + "  at mono.java.lang.Runnable.n_run(Native method)\n"
                + "\n";
        }

        private static string JrShape()
        {
            return CondWaitThread("\"main\" prio=5 tid=1 Native")
                + CondWaitThread("\"Thread-7\" prio=5 tid=21 Native")
                + CondWaitThread("\"Thread-35\" prio=5 tid=40 Native")
                + "\"Thread-8\" prio=5 tid=22 Native\n"
                + "  native: #00 pc 0000000000301a40  " + Apk + " (offset 0x1a2000)\n"
                + "\n"
                + "\"Thread-18\" prio=5 tid=30 Native\n"
                + "  native: #00 pc 000000000005c0d0  " + Libc + " (syscall+32) (BuildId: 0a1b)\n"
                + "  native: #01 pc 0000000000060b40  " + Libc + " (__futex_wait_ex(void volatile*, bool, int, bool, timespec const*)+144) (BuildId: 0a1b)\n"
                + "  native: #02 pc 00000000000c1c88  " + Libc + " (pthread_cond_timedwait+136) (BuildId: 0a1b)\n"
                + "  native: #03 pc 0000000000012340  " + Apk + " (SystemNative_LowLevelMonitor_TimedWait+128)\n"
                + "  native: #04 pc 0000000000a00000  " + Apk + " (offset 0x2b3000)\n"
                + "\n"
                + "\"HeapTaskDaemon\" daemon prio=5 tid=7 WaitingForTaskProcessor\n"
                + "  at dalvik.system.VMRuntime.runHeapTasks(Native method)\n"
                + "\n"
                + "----- end 3594 -----\n";
        }

        [Fact]
        public void JrShape_SignaturesAndSameWaitCount()
        {
            GHAnrTraceSummary s = Parse(JrShape());
            Assert.Equal(6, s.ThreadCount);
            Assert.Equal("pthread_cond_wait < apk x1", s.MainSignature);
            Assert.Equal(2, s.SameWaitThreadCount);
            Assert.Equal("1-2", GHAnrTraceSummary.SameWaitBucket(s.SameWaitThreadCount));
            Assert.Contains("Thread-8 [Native] running < apk x1 | #00 pc 0000000000301a40  <app>/split_config.arm64_v8a.apk (offset 0x1a2000)", s.ApkThreads);
            Assert.Contains("Thread-18 [Native] pthread_cond_timedwait < apk:SystemNative_LowLevelMonitor_TimedWait x2", s.ApkThreads);
            Assert.DoesNotContain("HeapTaskDaemon", s.ApkThreads);
        }

        [Fact]
        public void MainNativeFrames_AreCompactAndInOrder()
        {
            GHAnrTraceSummary s = Parse(JrShape());
            string[] lines = s.MainNativeFrames.Split('\n');
            Assert.Equal(4, lines.Length);
            Assert.StartsWith("#00 pc 000000000005c0cc  " + Libc + " (syscall+28)", lines[0]);
            Assert.Equal("#03 pc 00000000001f2e10  <app>/split_config.arm64_v8a.apk (offset 0x1a2000)", lines[3]);
            Assert.DoesNotContain("~~AAAA==", s.MainNativeFrames);
            Assert.DoesNotContain("~~AAAA==", s.ApkThreads);
        }

        [Fact]
        public void SonameForm_LabelsLibraryAndKeepsOffsetAndBuildId()
        {
            string text = "\"main\" prio=5 tid=1 Native\n"
                + "  native: #00 pc 00000000000c1b4c  " + Libc + " (pthread_cond_wait+76) (BuildId: 0a1b)\n"
                + "  native: #01 pc 00000000004a3c10  " + Apk + "!libcoreclr.so (offset 0x1a2000) (BuildId: 9f8e7d)\n"
                + "  native: #02 pc 00000000004a1000  " + Apk + "!libcoreclr.so (offset 0x1a2000) (BuildId: 9f8e7d)\n";
            GHAnrTraceSummary s = Parse(text);
            Assert.Equal("pthread_cond_wait < libcoreclr.so x2", s.MainSignature);
            Assert.Contains("<app>/split_config.arm64_v8a.apk!libcoreclr.so (offset 0x1a2000) (BuildId: 9f8e7d)", s.MainNativeFrames);
        }

        [Fact]
        public void DaemonAndUnattachedHeaders_StartThreads()
        {
            string text = "\"HeapTaskDaemon\" daemon prio=5 tid=7 WaitingForTaskProcessor\n"
                + "  native: #00 pc 000000000005c0cc  " + Libc + " (syscall+28) (BuildId: 0a1b)\n"
                + "\"FMOD mixer thread\" prio=5 (not attached)\n"
                + "  | sysTid=4120 nice=0 cgrp=top-app\n"
                + "  #00 pc 0000000000123400  " + Apk + " (offset 0x3c4000)\n"
                + "  #01 pc 0000000000123500  " + Apk + " (offset 0x3c4000)\n"
                + "\"main\" prio=5 tid=1 Native\n";
            GHAnrTraceSummary s = Parse(text);
            Assert.Equal(3, s.ThreadCount);
            Assert.Equal("none", s.MainSignature);
            Assert.Contains("FMOD mixer thread [not-attached] running < apk x2", s.ApkThreads);
            Assert.DoesNotContain("HeapTaskDaemon", s.ApkThreads);
        }

        [Fact]
        public void FunctionNames_ZeroOffsetUnknownAndDeleted()
        {
            string text = "\"main\" prio=5 tid=1 Native\n"
                + "  native: #00 pc 0000000000001000  /apex/com.android.art/lib64/libart.so (art::Foo) (BuildId: 77)\n"
                + "\"Thread-2\" prio=5 tid=2 Native\n"
                + "  native: #00 pc 0000000000002000  " + Apk + " (???)\n"
                + "\"Thread-3\" prio=5 tid=3 Native\n"
                + "  native: #00 pc 0000000000003000  /memfd:jit-cache (deleted) (offset 0x2000000)\n";
            GHAnrTraceSummary s = Parse(text);
            Assert.Equal("running < libart.so:art::Foo x1", s.MainSignature);
            Assert.Contains("Thread-2 [Native] running < apk x1", s.ApkThreads);
            Assert.DoesNotContain("Thread-3", s.ApkThreads);
        }

        [Fact]
        public void StopsAtEndLineAndAtLineCap()
        {
            string afterEnd = "\"main\" prio=5 tid=1 Native\n"
                + "----- end 3594 -----\n"
                + "  native: #00 pc 00000000000c1b4c  " + Libc + " (pthread_cond_wait+76)\n";
            Assert.Equal("none", Parse(afterEnd).MainSignature);

            string capped = "\"main\" prio=5 tid=1 Native\n"
                + "  native: #00 pc 00000000000c1b4c  " + Libc + " (pthread_cond_wait+76)\n"
                + "  native: #01 pc 00000000001f2e10  " + Apk + " (offset 0x1a2000)\n";
            Assert.Equal("pthread_cond_wait", Parse(capped, 2).MainSignature);
        }

        [Fact]
        public void EmptyAndMalformedInput_DoNotThrow()
        {
            GHAnrTraceSummary empty = Parse("");
            Assert.Equal(0, empty.ThreadCount);
            Assert.Equal("no-main", empty.MainSignature);
            Assert.Equal("", empty.MainNativeFrames);
            Assert.Equal("", empty.ApkThreads);
            Assert.Equal(0, empty.SameWaitThreadCount);

            GHAnrTraceSummary nullReader = GHAnrTraceSummary.Parse(null, 100);
            Assert.Equal("no-main", nullReader.MainSignature);

            GHAnrTraceSummary noMain = Parse("\"Thread-1\" prio=5 tid=2 Native\n  native: #00 pc 12  " + Libc + " (syscall\n");
            Assert.Equal("no-main", noMain.MainSignature);
            Assert.Equal(0, noMain.SameWaitThreadCount);
        }

        [Fact]
        public void RunningMain_CountsNoSameWaitThreads()
        {
            string text = "\"main\" prio=5 tid=1 Runnable\n"
                + "  native: #00 pc 0000000000301a40  " + Apk + " (offset 0x1a2000)\n"
                + "\"Thread-8\" prio=5 tid=22 Native\n"
                + "  native: #00 pc 0000000000301a40  " + Apk + " (offset 0x1a2000)\n";
            GHAnrTraceSummary s = Parse(text);
            Assert.Equal("running < apk x1", s.MainSignature);
            Assert.Equal(0, s.SameWaitThreadCount);
        }

        [Fact]
        public void Caps_MainFramesAndApkThreads()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("\"main\" prio=5 tid=1 Native\n");
            int i;
            for (i = 0; i < 40; i++)
                sb.Append("  native: #").Append(i.ToString("00")).Append(" pc 0000000000001000  " + Apk + " (offset 0x1000)\n");
            for (i = 0; i < 60; i++)
            {
                sb.Append("\"Thread-").Append(i + 100).Append("\" prio=5 tid=").Append(i + 100).Append(" Native\n");
                sb.Append("  native: #00 pc 0000000000001000  " + Apk + " (offset 0x1000)\n");
            }
            GHAnrTraceSummary s = Parse(sb.ToString());
            Assert.Equal(GHAnrTraceSummary.MaxMainFrames, s.MainNativeFrames.Split('\n').Length);
            string[] apkLines = s.ApkThreads.Split('\n');
            Assert.Equal(GHAnrTraceSummary.MaxApkThreads + 1, apkLines.Length);
            Assert.Equal("... and 12 more", apkLines[apkLines.Length - 1]);
        }

        [Fact]
        public void SignatureIsCutAtMaxLength()
        {
            string longName = new string('f', 300);
            string text = "\"main\" prio=5 tid=1 Native\n"
                + "  native: #00 pc 00000000000c1b4c  " + Libc + " (" + longName + "+4)\n";
            Assert.Equal(GHAnrTraceSummary.MaxSignatureLength, Parse(text).MainSignature.Length);
        }

        [Theory]
        [InlineData(0, "0")]
        [InlineData(1, "1-2")]
        [InlineData(2, "1-2")]
        [InlineData(3, "3-9")]
        [InlineData(9, "3-9")]
        [InlineData(10, "10+")]
        [InlineData(50, "10+")]
        public void SameWaitBucket_Boundaries(int count, string expected)
        {
            Assert.Equal(expected, GHAnrTraceSummary.SameWaitBucket(count));
        }
    }
}
