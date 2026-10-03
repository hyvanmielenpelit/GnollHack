#if WINDOWS
#nullable enable

using System;
using System.Runtime.InteropServices;

namespace GnollHackM
{
    /* D3D11, ANGLE EGL and GLES entry points, and raw vtable calls to the composition interop
       interfaces. Slot numbers follow d3d10.h and Microsoft.UI.Composition.Interop.h, with
       IUnknown at 0-2: ID3D10Multithread 3-6 (Enter, Leave, SetMultithreadProtected,
       GetMultithreadProtected); ICompositorInterop 3 (CreateGraphicsDevice);
       ICompositionGraphicsDeviceInterop 3-4 (GetRenderingDevice, SetRenderingDevice);
       ICompositionDrawingSurfaceInterop 3-8 (BeginDraw, EndDraw, Resize, Scroll, ResumeDraw,
       SuspendDraw). */
    internal static unsafe class AngleCompositionInterop
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct POINT
        {
            public int X;
            public int Y;
        }

        private const string LibEgl = "libEGL.dll";
        private const string LibGles = "libGLESv2.dll";

        public static readonly Guid IID_ID3D10Multithread = new Guid("9B7E4E00-342C-4106-A19F-4F2704F689F0");
        public static readonly Guid IID_ID3D11Texture2D = new Guid("6f15aaf2-d208-4e89-9ab4-489535d34f9c");
        public static readonly Guid IID_ICompositorInterop = new Guid("FAB19398-6D19-4D8A-B752-8F096C396069");
        public static readonly Guid IID_ICompositionGraphicsDeviceInterop = new Guid("4AFA8030-BC70-4B0C-B1C7-6E69F933DC83");
        public static readonly Guid IID_ICompositionDrawingSurfaceInterop = new Guid("2D6355C2-AD57-4EAE-92E4-4C3EFF65D578");

        private const int SlotQueryInterface = 0;
        private const int SlotMultithreadSetMultithreadProtected = 5;
        private const int SlotCompositorCreateGraphicsDevice = 3;
        private const int SlotGraphicsDeviceSetRenderingDevice = 4;
        private const int SlotDrawingSurfaceBeginDraw = 3;
        private const int SlotDrawingSurfaceEndDraw = 4;

        public const int E_INVALIDARG = unchecked((int)0x80070057);
        public const int DXGI_ERROR_DEVICE_REMOVED = unchecked((int)0x887A0005);
        public const int DXGI_ERROR_DEVICE_RESET = unchecked((int)0x887A0007);

        private const int D3D_DRIVER_TYPE_HARDWARE = 1;
        private const uint D3D11_CREATE_DEVICE_BGRA_SUPPORT = 0x20;
        private const uint D3D11_SDK_VERSION = 7;
        private const int D3D_FEATURE_LEVEL_11_1 = 0xb100;
        private const int D3D_FEATURE_LEVEL_11_0 = 0xb000;
        private const int D3D_FEATURE_LEVEL_10_1 = 0xa100;
        private const int D3D_FEATURE_LEVEL_10_0 = 0xa000;

        public const int EGL_FALSE = 0;
        public const int EGL_D3D11_DEVICE_ANGLE = 0x33A1;
        public const uint EGL_PLATFORM_DEVICE_EXT = 0x313F;
        public const uint EGL_D3D_TEXTURE_ANGLE = 0x33A3;
        public const int EGL_TEXTURE_OFFSET_X_ANGLE = 0x3490;
        public const int EGL_TEXTURE_OFFSET_Y_ANGLE = 0x3491;
        public const int EGL_SURFACE_ORIENTATION_ANGLE = 0x33A8;
        public const int EGL_SURFACE_ORIENTATION_INVERT_Y_ANGLE = 0x0002;
        public const int EGL_WIDTH = 0x3057;
        public const int EGL_HEIGHT = 0x3056;
        public const int EGL_SURFACE_TYPE = 0x3033;
        public const int EGL_PBUFFER_BIT = 0x0001;
        public const int EGL_RENDERABLE_TYPE = 0x3040;
        public const int EGL_OPENGL_ES2_BIT = 0x0004;
        public const int EGL_RED_SIZE = 0x3024;
        public const int EGL_GREEN_SIZE = 0x3023;
        public const int EGL_BLUE_SIZE = 0x3022;
        public const int EGL_ALPHA_SIZE = 0x3021;
        public const int EGL_DEPTH_SIZE = 0x3025;
        public const int EGL_STENCIL_SIZE = 0x3026;
        public const int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public const int EGL_NONE = 0x3038;
        public const int EGL_BAD_ATTRIBUTE = 0x3004;
        public const int EGL_CONTEXT_LOST = 0x300E;

        public static readonly IntPtr EGL_NO_DISPLAY = IntPtr.Zero;
        public static readonly IntPtr EGL_NO_CONTEXT = IntPtr.Zero;
        public static readonly IntPtr EGL_NO_SURFACE = IntPtr.Zero;
        public static readonly IntPtr EGL_NO_DEVICE_EXT = IntPtr.Zero;

        public const uint GL_STENCIL_BITS = 0x0D57;
        public const uint GL_RGBA8 = 0x8058;

        [DllImport("d3d11.dll")]
        private static extern int D3D11CreateDevice(IntPtr adapter, int driverType, IntPtr software, uint flags,
            int* featureLevels, uint featureLevelCount, uint sdkVersion, IntPtr* device, int* featureLevel,
            IntPtr* immediateContext);

        [DllImport(LibEgl)]
        public static extern IntPtr eglGetPlatformDisplayEXT(uint platform, IntPtr nativeDisplay, int* attribList);

        [DllImport(LibEgl)]
        public static extern uint eglInitialize(IntPtr display, int* major, int* minor);

        [DllImport(LibEgl)]
        public static extern uint eglTerminate(IntPtr display);

        [DllImport(LibEgl)]
        public static extern uint eglChooseConfig(IntPtr display, int* attribList,
            IntPtr* configs, int configSize, int* numConfig);

        [DllImport(LibEgl)]
        public static extern IntPtr eglCreateContext(IntPtr display, IntPtr config, IntPtr shareContext, int* attribList);

        [DllImport(LibEgl)]
        public static extern uint eglDestroyContext(IntPtr display, IntPtr context);

        [DllImport(LibEgl)]
        public static extern IntPtr eglCreatePbufferFromClientBuffer(IntPtr display, uint bufferType,
            IntPtr buffer, IntPtr config, int* attribList);

        [DllImport(LibEgl)]
        public static extern uint eglDestroySurface(IntPtr display, IntPtr surface);

        [DllImport(LibEgl)]
        public static extern uint eglQuerySurface(IntPtr display, IntPtr surface, int attribute, int* value);

        [DllImport(LibEgl)]
        public static extern uint eglMakeCurrent(IntPtr display, IntPtr draw, IntPtr read, IntPtr context);

        [DllImport(LibEgl)]
        public static extern int eglGetError();

        /* attribList is EGLAttrib, which is pointer-sized */
        [DllImport(LibEgl)]
        public static extern IntPtr eglCreateDeviceANGLE(int deviceType, IntPtr nativeDevice, IntPtr* attribList);

        [DllImport(LibEgl)]
        public static extern uint eglReleaseDeviceANGLE(IntPtr device);

        [DllImport(LibGles)]
        public static extern void glFlush();

        [DllImport(LibGles)]
        public static extern void glGetIntegerv(uint pname, int* data);

        /* Hardware device with BGRA support; 11_1 is retried without where the runtime rejects it */
        public static int CreateD3D11Device(out IntPtr device, out IntPtr immediateContext)
        {
            int* levels = stackalloc int[4];
            levels[0] = D3D_FEATURE_LEVEL_11_1;
            levels[1] = D3D_FEATURE_LEVEL_11_0;
            levels[2] = D3D_FEATURE_LEVEL_10_1;
            levels[3] = D3D_FEATURE_LEVEL_10_0;

            IntPtr dev = IntPtr.Zero;
            IntPtr ctx = IntPtr.Zero;
            int level = 0;
            int hr = D3D11CreateDevice(IntPtr.Zero, D3D_DRIVER_TYPE_HARDWARE, IntPtr.Zero, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                levels, 4, D3D11_SDK_VERSION, &dev, &level, &ctx);
            if (hr == E_INVALIDARG)
            {
                hr = D3D11CreateDevice(IntPtr.Zero, D3D_DRIVER_TYPE_HARDWARE, IntPtr.Zero, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                    levels + 1, 3, D3D11_SDK_VERSION, &dev, &level, &ctx);
            }

            device = hr >= 0 ? dev : IntPtr.Zero;
            immediateContext = hr >= 0 ? ctx : IntPtr.Zero;
            return hr;
        }

        public static int QueryInterface(IntPtr unknown, Guid iid, out IntPtr result)
        {
            IntPtr ppv = IntPtr.Zero;
            var queryInterface =
                (delegate* unmanaged[Stdcall]<IntPtr, Guid*, IntPtr*, int>)(*(void***)unknown)[SlotQueryInterface];
            int hr = queryInterface(unknown, &iid, &ppv);
            result = hr >= 0 ? ppv : IntPtr.Zero;
            return hr;
        }

        public static void AddRef(IntPtr unknown)
        {
            if (unknown != IntPtr.Zero)
                Marshal.AddRef(unknown);
        }

        public static void Release(IntPtr unknown)
        {
            if (unknown != IntPtr.Zero)
                Marshal.Release(unknown);
        }

        public static int SetMultithreadProtected(IntPtr d3dDevice)
        {
            IntPtr multithread;
            int hr = QueryInterface(d3dDevice, IID_ID3D10Multithread, out multithread);
            if (hr < 0)
                return hr;

            var setProtected =
                (delegate* unmanaged[Stdcall]<IntPtr, int, int>)(*(void***)multithread)[SlotMultithreadSetMultithreadProtected];
            setProtected(multithread, 1);
            Release(multithread);
            return 0;
        }

        public static int CreateGraphicsDevice(IntPtr compositorInterop, IntPtr renderingDevice, out IntPtr graphicsDevice)
        {
            IntPtr result = IntPtr.Zero;
            var create =
                (delegate* unmanaged[Stdcall]<IntPtr, IntPtr, IntPtr*, int>)(*(void***)compositorInterop)[SlotCompositorCreateGraphicsDevice];
            int hr = create(compositorInterop, renderingDevice, &result);
            graphicsDevice = hr >= 0 ? result : IntPtr.Zero;
            return hr;
        }

        public static int SetRenderingDevice(IntPtr graphicsDeviceInterop, IntPtr renderingDevice)
        {
            var set =
                (delegate* unmanaged[Stdcall]<IntPtr, IntPtr, int>)(*(void***)graphicsDeviceInterop)[SlotGraphicsDeviceSetRenderingDevice];
            return set(graphicsDeviceInterop, renderingDevice);
        }

        /* Whole-surface update; the texture is returned with a reference the caller releases */
        public static int BeginDraw(IntPtr drawingSurfaceInterop, out IntPtr texture, out POINT offset)
        {
            Guid iid = IID_ID3D11Texture2D;
            IntPtr updateObject = IntPtr.Zero;
            POINT updateOffset = default;
            var beginDraw =
                (delegate* unmanaged[Stdcall]<IntPtr, IntPtr, Guid*, IntPtr*, POINT*, int>)(*(void***)drawingSurfaceInterop)[SlotDrawingSurfaceBeginDraw];
            int hr = beginDraw(drawingSurfaceInterop, IntPtr.Zero, &iid, &updateObject, &updateOffset);
            texture = hr >= 0 ? updateObject : IntPtr.Zero;
            offset = updateOffset;
            return hr;
        }

        public static int EndDraw(IntPtr drawingSurfaceInterop)
        {
            var endDraw = (delegate* unmanaged[Stdcall]<IntPtr, int>)(*(void***)drawingSurfaceInterop)[SlotDrawingSurfaceEndDraw];
            return endDraw(drawingSurfaceInterop);
        }
    }
}
#endif
