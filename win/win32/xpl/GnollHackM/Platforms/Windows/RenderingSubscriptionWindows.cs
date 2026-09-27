#if WINDOWS && GNH_MAUI && ENABLE_RAW_RENDERING
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading;
using WinRT;

namespace GnollHackM
{
    /* Subscribes to CompositionTarget.Rendering through its ABI with a static native
       delegate, so no managed wrapper is created for the per-frame event arguments.
       Slots follow WinMD order, not the member order of the C# projection; an interface
       whose layout changed would carry a new IID and fail the factory query. */
    internal static unsafe class RenderingSubscriptionWindows
    {
        private static readonly Guid IID_ICompositionTargetStatics = new Guid("12A4BE6F-6DB1-5165-B622-D57AB782745B");
        private static readonly Guid IID_IRenderingEventArgs = new Guid("A67C8F8D-1885-5FC9-975C-901224F79B1E");
        private static readonly Guid IID_EventHandlerOfObject = new Guid("C50898F6-C536-5F47-8583-8B2C2438A13B");
        private static readonly Guid IID_IUnknown = new Guid("00000000-0000-0000-C000-000000000046");
        private static readonly Guid IID_IAgileObject = new Guid("94EA2B94-E9CC-49E0-C0FF-EE64CA8F5B90");

        private const int SlotAddRendering = 6;
        private const int SlotRemoveRendering = 7;
        private const int SlotGetRenderingTime = 6;

        private const int E_FAIL = unchecked((int)0x80004005);
        private const int E_NOINTERFACE = unchecked((int)0x80004002);
        /* Either of these makes XAML remove the handler without a trace */
        private const int RPC_E_DISCONNECTED = unchecked((int)0x80010108);
        private const int RPC_E_SERVER_UNAVAILABLE = unchecked((int)0x800706BA);

        private static IObjectReference _statics;
        private static IntPtr _handler;
        private static long _token;
        private static bool _subscribed;
        private static Action _onFrame;
        private static long _callbackCount;

        /* The arguments of the callback in progress; set only for its duration */
        private static IntPtr _currentArgs;

        public static bool IsSubscribed { get { return _subscribed; } }
        public static long CallbackCount { get { return Interlocked.Read(ref _callbackCount); } }

        /* UI thread only, as for the managed event */
        public static bool TrySubscribe(Action onFrame)
        {
            if (_subscribed)
                return true;

            try
            {
                if (_statics == null)
                    _statics = ActivationFactory.Get("Microsoft.UI.Xaml.Media.CompositionTarget", IID_ICompositionTargetStatics);

                EnsureHandler();
                _onFrame = onFrame;

                IntPtr statics = _statics.ThisPtr;
                long token;
                var add = (delegate* unmanaged[Stdcall]<IntPtr, IntPtr, long*, int>)(*(void***)statics)[SlotAddRendering];
                if (add(statics, _handler, &token) < 0)
                {
                    _onFrame = null;
                    return false;
                }

                _token = token;
                _subscribed = true;
                return true;
            }
            catch (Exception)
            {
                _onFrame = null;
                return false;
            }
        }

        public static void Unsubscribe()
        {
            if (!_subscribed)
                return;

            IntPtr statics = _statics.ThisPtr;
            var remove = (delegate* unmanaged[Stdcall]<IntPtr, long, int>)(*(void***)statics)[SlotRemoveRendering];
            remove(statics, _token);
            _subscribed = false;
            _onFrame = null;
        }

        /* RenderingEventArgs.RenderingTime in 100 ns ticks; valid only inside the callback */
        public static bool TryGetCurrentRenderingTimeTicks(out long ticks)
        {
            ticks = 0;
            IntPtr args = _currentArgs;
            if (args == IntPtr.Zero)
                return false;

            Guid iid = IID_IRenderingEventArgs;
            IntPtr renderingArgs;
            var queryInterface = (delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int>)(*(void***)args)[0];
            if (queryInterface(args, &iid, &renderingArgs) < 0)
                return false;

            try
            {
                long value;
                var getRenderingTime = (delegate* unmanaged[Stdcall]<IntPtr, long*, int>)(*(void***)renderingArgs)[SlotGetRenderingTime];
                if (getRenderingTime(renderingArgs, &value) < 0)
                    return false;
                ticks = value;
                return true;
            }
            finally
            {
                Marshal.Release(renderingArgs);
            }
        }

        /* One object for the process lifetime: a pointer to a four-entry vtable.
           Reference counts are constant because it is never freed. */
        private static void EnsureHandler()
        {
            if (_handler != IntPtr.Zero)
                return;

            IntPtr* vtbl = (IntPtr*)NativeMemory.Alloc((nuint)(4 * sizeof(IntPtr)));
            vtbl[0] = (IntPtr)(delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int>)&QueryInterface;
            vtbl[1] = (IntPtr)(delegate* unmanaged[Stdcall]<IntPtr, uint>)&AddRef;
            vtbl[2] = (IntPtr)(delegate* unmanaged[Stdcall]<IntPtr, uint>)&Release;
            vtbl[3] = (IntPtr)(delegate* unmanaged[Stdcall]<IntPtr, IntPtr, IntPtr, int>)&Invoke;

            IntPtr* obj = (IntPtr*)NativeMemory.Alloc((nuint)sizeof(IntPtr));
            obj[0] = (IntPtr)vtbl;
            _handler = (IntPtr)obj;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static int QueryInterface(IntPtr thisPtr, Guid* riid, IntPtr* ppv)
        {
            Guid iid = *riid;
            if (iid == IID_IUnknown || iid == IID_EventHandlerOfObject || iid == IID_IAgileObject)
            {
                *ppv = thisPtr;
                return 0;
            }
            *ppv = IntPtr.Zero;
            return E_NOINTERFACE;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static uint AddRef(IntPtr thisPtr)
        {
            return 1;
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static uint Release(IntPtr thisPtr)
        {
            return 1;
        }

        /* sender is null for this static event. A failure is reported the way CsWinRT's
           generated invoker reports it, and XAML raises it as an unhandled exception. */
        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static int Invoke(IntPtr thisPtr, IntPtr sender, IntPtr args)
        {
            Interlocked.Increment(ref _callbackCount);
            IntPtr previousArgs = _currentArgs;
            _currentArgs = args;
            try
            {
                Action onFrame = _onFrame;
                if (onFrame != null)
                    onFrame();
                return 0;
            }
            catch (Exception ex)
            {
                ExceptionHelpers.SetErrorInfo(ex);
                int hr = ExceptionHelpers.GetHRForException(ex);
                if (hr >= 0 || hr == RPC_E_DISCONNECTED || hr == RPC_E_SERVER_UNAVAILABLE)
                    hr = E_FAIL;
                return hr;
            }
            finally
            {
                _currentArgs = previousArgs;
            }
        }
    }
}
#endif
