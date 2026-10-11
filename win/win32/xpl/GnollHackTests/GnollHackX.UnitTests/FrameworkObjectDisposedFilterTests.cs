using System;
using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers FrameworkObjectDisposedFilter: the nearest stack frame outside the JNI peer
       plumbing, the platform bindings and the base class library decides, a MAUI caller or no
       caller at all means framework teardown, any other caller or a missing stack means the
       exception is reported, and only transparent wrappers are unwrapped. */
    public class FrameworkObjectDisposedFilterTests
    {
        private const string MonoAssertSelf =
            "  at Java.Interop.JniPeerMembers.AssertSelf (Java.Interop.IJavaPeerable self) [0x00029] in <abc>:0";
        private const string MonoInvokeVirtual =
            "  at Java.Interop.JniPeerMembers+JniInstanceMethods.InvokeVirtualVoidMethod (System.String encodedMember, Java.Interop.IJavaPeerable self, Java.Interop.JniArgumentValue* parameters) [0x0001e] in <abc>:0";
        private const string MonoInvalidate =
            "  at Android.Views.View.Invalidate () [0x0000a] in <abc>:0";
        private const string MonoMauiCaller =
            "  at Microsoft.Maui.Platform.WrapperView.Foo () [0x00000] in <abc>:0";
        private const string MonoGnollHackCaller =
            "  at GnollHackM.AndroidSKGLViewSurfaceHandler.DisconnectHandler (SkiaSharp.Views.Android.SKGLSurfaceView platformView) [0x00010] in <abc>:0";
        private const string MonoMainActivityOnDestroy =
            "  at GnollHackM.MainActivity.OnDestroy () [0x00002] in <abc>:0";

        private static string Stack(params string[] frames)
        {
            return string.Join("\n", frames);
        }

        [Fact]
        public void MonoStack_MauiCaller_IsFramework()
        {
            string stack = Stack(MonoAssertSelf, MonoInvokeVirtual, MonoInvalidate, MonoMauiCaller);
            Assert.True(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(stack));
        }

        [Fact]
        public void MonoStack_GnollHackCaller_IsNotFramework()
        {
            string stack = Stack(MonoAssertSelf, MonoInvokeVirtual, MonoInvalidate, MonoGnollHackCaller);
            Assert.False(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(stack));
        }

        [Fact]
        public void NearestCallerDecides_MauiBeforeGnollHack_IsFramework()
        {
            string stack = Stack(MonoAssertSelf, MonoInvokeVirtual, MonoInvalidate, MonoMauiCaller, MonoMainActivityOnDestroy);
            Assert.True(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(stack));
        }

        [Fact]
        public void CoreClrStack_MauiCaller_IsFramework()
        {
            string stack = string.Join("\r\n",
                "   at Microsoft.Extensions.DependencyInjection.ServiceLookup.ThrowHelper.ThrowObjectDisposedException()",
                "   at Microsoft.Maui.MauiContext.get_Services() in /_/src/Core/src/MauiContext.cs:line 12");
            Assert.True(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(stack));
        }

        [Fact]
        public void OnlyPlumbingFrames_IsFramework()
        {
            string stack = Stack(
                MonoAssertSelf,
                MonoInvokeVirtual,
                "  at Android.Views.View+IOnClickListenerInvoker.n_OnClick_Landroid_view_View_ (System.IntPtr jnienv, System.IntPtr native__this, System.IntPtr native_v) [0x00011] in <abc>:0");
            Assert.True(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(stack));
        }

        [Fact]
        public void EndOfStackTraceSeparator_IsSkipped()
        {
            string separator = "--- End of stack trace from previous location ---";
            Assert.True(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(
                Stack(MonoAssertSelf, MonoInvalidate, separator, MonoMauiCaller)));
            Assert.False(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(
                Stack(MonoAssertSelf, MonoInvalidate, separator, MonoGnollHackCaller)));
        }

        [Fact]
        public void SkiaSharpCaller_IsNotFramework()
        {
            string stack = Stack(
                MonoAssertSelf,
                MonoInvokeVirtual,
                MonoInvalidate,
                "  at SkiaSharp.Views.Maui.Handlers.SKCanvasViewHandler.OnInvalidateSurface () [0x00000] in <abc>:0");
            Assert.False(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(stack));
        }

        [Theory]
        [InlineData(null)]
        [InlineData("")]
        [InlineData("   ")]
        [InlineData(" \r\n \n ")]
        public void MissingStack_IsNotFramework(string stack)
        {
            Assert.False(FrameworkObjectDisposedFilter.IsFrameworkTeardownDisposal(stack));
        }

        [Fact]
        public void FindFrameworkTeardownDisposal_Null_ReturnsNull()
        {
            Assert.Null(FrameworkObjectDisposedFilter.FindFrameworkTeardownDisposal(null));
        }

        [Fact]
        public void FindFrameworkTeardownDisposal_NonWrapperOuterException_ReturnsNull()
        {
            Exception exception = new InvalidOperationException("x", new ObjectDisposedException("y"));
            Assert.Null(FrameworkObjectDisposedFilter.FindFrameworkTeardownDisposal(exception));
        }

        [Fact]
        public void FindFrameworkTeardownDisposal_UnthrownDisposal_ReturnsNull()
        {
            ObjectDisposedException ode = new ObjectDisposedException("Android.Views.View");
            Assert.Null(ode.StackTrace);
            Assert.Null(FrameworkObjectDisposedFilter.FindFrameworkTeardownDisposal(ode));
            Assert.Null(FrameworkObjectDisposedFilter.FindFrameworkTeardownDisposal(new AggregateException(ode)));
        }

        [Fact]
        public void FindFrameworkTeardownDisposal_AggregateOfTwo_ReturnsNull()
        {
            AggregateException aggregate = new AggregateException(
                new ObjectDisposedException("Android.Views.View"),
                new ObjectDisposedException("Android.Views.ViewGroup"));
            Assert.Null(FrameworkObjectDisposedFilter.FindFrameworkTeardownDisposal(aggregate));
        }

        [Fact]
        public void Describe_UsesObjectName()
        {
            Assert.Equal(FrameworkObjectDisposedFilter.WarningMessagePrefix + "Android.Views.View",
                FrameworkObjectDisposedFilter.Describe(new ObjectDisposedException("Android.Views.View")));
        }
    }
}
