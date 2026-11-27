using System;
using System.IO;
using System.Reflection;
using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers GHApplicationControlBlock: a FileLoadException carrying one of the two Win32 policy errors is
       found directly or inside wrapping exceptions, its file name is shortened, and every other load
       error or exception type is ignored. */
    public class GHApplicationControlBlockTests
    {
        private const int HrSystemIntegrityPolicyViolation = unchecked((int)0x800711C7);
        private const int HrAccessDisabledByPolicy = unchecked((int)0x800704EC);
        private const int HrFusionRefDefMismatch = unchecked((int)0x80131040);

        private static FileLoadException Blocked(string fileName, int hResult)
        {
            return new FileLoadException("Blocked", fileName) { HResult = hResult };
        }

        [Fact]
        public void SmartAppControlBlock_FullPath_IsFound()
        {
            Exception ex = Blocked(@"c:\program files (x86)\steam\steamapps\common\GnollHack\SkiaSharp.Views.Windows.dll", HrSystemIntegrityPolicyViolation);

            bool found = GHApplicationControlBlock.TryFind(ex, out string fileName, out int error);

            Assert.True(found);
            Assert.Equal("SkiaSharp.Views.Windows.dll", fileName);
            Assert.Equal(4551, error);
        }

        [Fact]
        public void AppLockerBlock_IsFound()
        {
            Exception ex = Blocked(@"C:\Games\GnollHack\Microsoft.Maui.dll", HrAccessDisabledByPolicy);

            bool found = GHApplicationControlBlock.TryFind(ex, out string fileName, out int error);

            Assert.True(found);
            Assert.Equal("Microsoft.Maui.dll", fileName);
            Assert.Equal(1260, error);
        }

        [Fact]
        public void BlockInsideTypeInitializationException_IsFound()
        {
            Exception ex = new TypeInitializationException("Some.Type", Blocked(@"C:\Games\GnollHack\SkiaSharp.dll", HrSystemIntegrityPolicyViolation));

            Assert.True(GHApplicationControlBlock.TryFind(ex, out string fileName, out int error));
            Assert.Equal("SkiaSharp.dll", fileName);
            Assert.Equal(4551, error);
        }

        [Fact]
        public void BlockInsideTargetInvocationException_IsFound()
        {
            Exception ex = new TargetInvocationException(Blocked(@"C:\Games\GnollHack\WinRT.Runtime.dll", HrSystemIntegrityPolicyViolation));

            Assert.True(GHApplicationControlBlock.TryFind(ex, out string fileName, out _));
            Assert.Equal("WinRT.Runtime.dll", fileName);
        }

        [Fact]
        public void OtherLoadError_IsIgnored()
        {
            Exception ex = Blocked(@"C:\Games\GnollHack\SkiaSharp.dll", HrFusionRefDefMismatch);

            Assert.False(GHApplicationControlBlock.TryFind(ex, out string fileName, out int error));
            Assert.Null(fileName);
            Assert.Equal(0, error);
        }

        [Fact]
        public void OtherExceptionType_WithPolicyHResult_IsIgnored()
        {
            Exception ex = new InvalidOperationException("Not a load") { HResult = HrSystemIntegrityPolicyViolation };

            Assert.False(GHApplicationControlBlock.TryFind(ex, out _, out _));
        }

        [Fact]
        public void AssemblyDisplayName_IsShortenedToDllName()
        {
            Exception ex = Blocked("Foo.Bar, Version=1.0.0.0, Culture=neutral", HrSystemIntegrityPolicyViolation);

            Assert.True(GHApplicationControlBlock.TryFind(ex, out string fileName, out _));
            Assert.Equal("Foo.Bar.dll", fileName);
        }

        [Fact]
        public void NullException_IsNotABlock()
        {
            Assert.False(GHApplicationControlBlock.TryFind(null, out string fileName, out int error));
            Assert.Null(fileName);
            Assert.Equal(0, error);
        }
    }
}
