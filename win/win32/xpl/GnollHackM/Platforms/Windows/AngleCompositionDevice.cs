#if WINDOWS
#nullable enable

using System;
using System.Collections.Generic;
using System.Diagnostics;
using Microsoft.UI.Composition;
using SkiaSharp;
using WinRT;
using static GnollHackM.AngleCompositionInterop;

namespace GnollHackM
{
    /* The process-wide GL device of the composition panels: an own D3D11 device handed both to
       ANGLE, as the EGL device of a dedicated display, and to composition, as the rendering
       device of each compositor's graphics device, so the textures of the drawing surfaces are
       renderable by ANGLE directly. One EGL context and one GRContext serve every panel.
       UI thread only. Lives for the process; only a device loss tears it down and rebuilds it. */
    internal sealed class AngleCompositionDevice
    {
        private static AngleCompositionDevice? s_instance;
        private static bool s_initializeFailed;
        private static bool s_recreateFailureLogged;
        private static bool s_graphicsDeviceFailureLogged;
        private static bool s_makeCurrentFailureLogged;

        /* Raised before the EGL objects are destroyed, while pbuffers can still be destroyed;
           IsAvailable is already false and GRContext null */
        public static event Action? DeviceLost;
        /* Raised after a successful rebuild */
        public static event Action? DeviceRecreated;

        private IntPtr _d3dDevice;
        private IntPtr _d3dContext;
        private IntPtr _eglDevice;
        private IntPtr _display;
        private IntPtr _config;
        private IntPtr _context;
        private GRGlInterface? _glInterface;
        private GRContext? _grContext;
        private IntPtr _lastSurface;
        private bool _available;
        private int _generation;
        private bool _inDraw;
        private bool _deviceLostPending;
        private bool _handlingDeviceLost;
        private readonly List<KeyValuePair<Compositor, CompositionGraphicsDevice>> _graphicsDevices =
            new List<KeyValuePair<Compositor, CompositionGraphicsDevice>>(1);

        private AngleCompositionDevice()
        {
        }

        public static AngleCompositionDevice? Instance { get { return s_instance; } }

        public bool IsAvailable { get { return _available; } }
        public GRContext? GRContext { get { return _available ? _grContext : null; } }
        public IntPtr Display { get { return _display; } }
        public IntPtr Config { get { return _config; } }
        public IntPtr Context { get { return _context; } }

        /* Changes whenever the EGL objects are destroyed; EGL handles from an older generation
           are dead and must not be passed to EGL */
        public int Generation { get { return _generation; } }

        public static bool TryInitialize()
        {
            Debug.Assert(Microsoft.Maui.ApplicationModel.MainThread.IsMainThread);
            if (s_instance != null)
                return s_instance._available;
            if (s_initializeFailed)
                return false;

            var device = new AngleCompositionDevice();
            string error;
            if (!device.CreateResources(out error))
            {
                s_initializeFailed = true;
                GnollHackX.GHApp.MaybeWriteGHLog("AngleCompositionDevice: initialization failed: " + error,
                    true, GHConstants.SentryGnollHackGeneralCategoryName);
                return false;
            }

            device._available = true;
            s_instance = device;
            GnollHackX.GHApp.MaybeWriteGHLog("AngleCompositionDevice: initialized",
                true, GHConstants.SentryGnollHackGeneralCategoryName);
            return true;
        }

        /* One graphics device per compositor, rendering with this device's D3D11 device */
        public CompositionGraphicsDevice? GetGraphicsDevice(Compositor compositor)
        {
            Debug.Assert(Microsoft.Maui.ApplicationModel.MainThread.IsMainThread);
            if (!_available)
                return null;

            int i;
            for (i = 0; i < _graphicsDevices.Count; i++)
            {
                if (_graphicsDevices[i].Key == compositor)
                    return _graphicsDevices[i].Value;
            }

            IntPtr compositorPtr = ((IWinRTObject)compositor).NativeObject.ThisPtr;
            IntPtr compositorInterop;
            int hr = QueryInterface(compositorPtr, IID_ICompositorInterop, out compositorInterop);
            if (hr < 0)
            {
                LogOnce(ref s_graphicsDeviceFailureLogged,
                    "AngleCompositionDevice: ICompositorInterop query failed: 0x" + hr.ToString("X8"));
                return null;
            }

            IntPtr rawGraphicsDevice;
            hr = CreateGraphicsDevice(compositorInterop, _d3dDevice, out rawGraphicsDevice);
            Release(compositorInterop);
            GC.KeepAlive(compositor);
            if (hr < 0 || rawGraphicsDevice == IntPtr.Zero)
            {
                LogOnce(ref s_graphicsDeviceFailureLogged,
                    "AngleCompositionDevice: CreateGraphicsDevice failed: 0x" + hr.ToString("X8"));
                return null;
            }

            /* FromAbi takes its own reference */
            CompositionGraphicsDevice graphicsDevice = CompositionGraphicsDevice.FromAbi(rawGraphicsDevice);
            Release(rawGraphicsDevice);
            _graphicsDevices.Add(new KeyValuePair<Compositor, CompositionGraphicsDevice>(compositor, graphicsDevice));
            return graphicsDevice;
        }

        /* Skia's cached GL state is reset whenever the bound surface changes, because the
           default framebuffer changes with it */
        public bool MakeCurrent(IntPtr surface)
        {
            Debug.Assert(Microsoft.Maui.ApplicationModel.MainThread.IsMainThread);
            if (!_available)
                return false;

            if (eglMakeCurrent(_display, surface, surface, _context) == EGL_FALSE)
            {
                int error = eglGetError();
                if (error == EGL_CONTEXT_LOST)
                    HandleDeviceLost();
                else
                    LogOnce(ref s_makeCurrentFailureLogged,
                        "AngleCompositionDevice: eglMakeCurrent failed: 0x" + error.ToString("X4"));
                return false;
            }

            if (surface != _lastSurface)
            {
                _lastSurface = surface;
                _grContext?.ResetContext();
            }
            return true;
        }

        /* Called before a pbuffer is destroyed; unbinds it if it is the bound one, so it is freed
           at once and a later surface reusing its handle counts as a change */
        public void ForgetSurface(IntPtr surface)
        {
            if (surface == IntPtr.Zero || surface != _lastSurface)
                return;

            _lastSurface = IntPtr.Zero;
            if (_display != IntPtr.Zero && _context != IntPtr.Zero)
                eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, _context);
        }

        /* Bracket a panel's BeginDraw and EndDraw; a device loss detected in between is handled
           after EndDraw */
        public void EnterDraw()
        {
            _inDraw = true;
        }

        public void ExitDraw()
        {
            _inDraw = false;
            if (_deviceLostPending)
            {
                _deviceLostPending = false;
                HandleDeviceLost();
            }
        }

        public void HandleDeviceLost()
        {
            Debug.Assert(Microsoft.Maui.ApplicationModel.MainThread.IsMainThread);
            if (_inDraw)
            {
                _deviceLostPending = true;
                return;
            }
            if (_handlingDeviceLost)
                return;

            _handlingDeviceLost = true;
            try
            {
                GnollHackX.GHApp.MaybeWriteGHLog("AngleCompositionDevice: device lost; recreating",
                    true, GHConstants.SentryGnollHackGeneralCategoryName);

                _available = false;
                DeviceLost?.Invoke();
                DestroyResources();

                string error;
                if (!CreateResources(out error))
                {
                    LogOnce(ref s_recreateFailureLogged, "AngleCompositionDevice: recreation failed: " + error);
                    return;
                }

                int i;
                for (i = 0; i < _graphicsDevices.Count; i++)
                {
                    CompositionGraphicsDevice graphicsDevice = _graphicsDevices[i].Value;
                    IntPtr graphicsDeviceInterop;
                    int hr = QueryInterface(((IWinRTObject)graphicsDevice).NativeObject.ThisPtr,
                        IID_ICompositionGraphicsDeviceInterop, out graphicsDeviceInterop);
                    if (hr >= 0)
                    {
                        hr = SetRenderingDevice(graphicsDeviceInterop, _d3dDevice);
                        Release(graphicsDeviceInterop);
                    }
                    GC.KeepAlive(graphicsDevice);
                    if (hr < 0)
                    {
                        GnollHackX.GHApp.MaybeWriteGHLog(
                            "AngleCompositionDevice: SetRenderingDevice failed: 0x" + hr.ToString("X8"),
                            true, GHConstants.SentryGnollHackGeneralCategoryName);
                    }
                }

                _available = true;
                DeviceRecreated?.Invoke();
            }
            finally
            {
                _handlingDeviceLost = false;
            }
        }

        private unsafe bool CreateResources(out string error)
        {
            IntPtr d3dDevice;
            IntPtr d3dContext;
            int hr = CreateD3D11Device(out d3dDevice, out d3dContext);
            if (hr < 0)
            {
                error = "D3D11CreateDevice failed: 0x" + hr.ToString("X8");
                return false;
            }
            _d3dDevice = d3dDevice;
            _d3dContext = d3dContext;

            hr = AngleCompositionInterop.SetMultithreadProtected(_d3dDevice);
            if (hr < 0)
            {
                error = "ID3D10Multithread query failed: 0x" + hr.ToString("X8");
                DestroyResources();
                return false;
            }

            _eglDevice = eglCreateDeviceANGLE(EGL_D3D11_DEVICE_ANGLE, _d3dDevice, null);
            if (_eglDevice == EGL_NO_DEVICE_EXT)
            {
                error = "eglCreateDeviceANGLE failed: 0x" + eglGetError().ToString("X4");
                DestroyResources();
                return false;
            }

            _display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, _eglDevice, null);
            if (_display == EGL_NO_DISPLAY)
            {
                error = "eglGetPlatformDisplayEXT failed: 0x" + eglGetError().ToString("X4");
                DestroyResources();
                return false;
            }

            if (eglInitialize(_display, null, null) == EGL_FALSE)
            {
                error = "eglInitialize failed: 0x" + eglGetError().ToString("X4");
                DestroyResources();
                return false;
            }

            int* configAttributes = stackalloc int[17];
            configAttributes[0] = EGL_RED_SIZE;
            configAttributes[1] = 8;
            configAttributes[2] = EGL_GREEN_SIZE;
            configAttributes[3] = 8;
            configAttributes[4] = EGL_BLUE_SIZE;
            configAttributes[5] = 8;
            configAttributes[6] = EGL_ALPHA_SIZE;
            configAttributes[7] = 8;
            configAttributes[8] = EGL_DEPTH_SIZE;
            configAttributes[9] = 8;
            configAttributes[10] = EGL_STENCIL_SIZE;
            configAttributes[11] = 8;
            configAttributes[12] = EGL_SURFACE_TYPE;
            configAttributes[13] = EGL_PBUFFER_BIT;
            configAttributes[14] = EGL_RENDERABLE_TYPE;
            configAttributes[15] = EGL_OPENGL_ES2_BIT;
            configAttributes[16] = EGL_NONE;
            IntPtr config = IntPtr.Zero;
            int configCount = 0;
            if (eglChooseConfig(_display, configAttributes, &config, 1, &configCount) == EGL_FALSE || configCount < 1)
            {
                error = "eglChooseConfig failed: 0x" + eglGetError().ToString("X4");
                DestroyResources();
                return false;
            }
            _config = config;

            int* contextAttributes = stackalloc int[3];
            contextAttributes[0] = EGL_CONTEXT_CLIENT_VERSION;
            contextAttributes[1] = 2;
            contextAttributes[2] = EGL_NONE;
            _context = eglCreateContext(_display, _config, EGL_NO_CONTEXT, contextAttributes);
            if (_context == EGL_NO_CONTEXT)
            {
                error = "eglCreateContext failed: 0x" + eglGetError().ToString("X4");
                DestroyResources();
                return false;
            }

            if (eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, _context) == EGL_FALSE)
            {
                error = "eglMakeCurrent failed: 0x" + eglGetError().ToString("X4");
                DestroyResources();
                return false;
            }
            _lastSurface = IntPtr.Zero;

            _glInterface = GRGlInterface.Create();
            if (_glInterface == null)
            {
                error = "GRGlInterface.Create returned null";
                DestroyResources();
                return false;
            }

            _grContext = GRContext.CreateGl(_glInterface);
            if (_grContext == null)
            {
                error = "GRContext.CreateGl returned null";
                DestroyResources();
                return false;
            }

            error = "";
            return true;
        }

        /* Resources are released only while this context is current; otherwise Skia's GL deletes
           would reach whichever context is current on the thread */
        private void DestroyResources()
        {
            _generation++;
            if (_grContext != null)
            {
                bool current = _display != IntPtr.Zero && _context != IntPtr.Zero
                    && eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, _context) != EGL_FALSE;
                _grContext.AbandonContext(current);
                _grContext.Dispose();
                _grContext = null;
            }
            if (_glInterface != null)
            {
                _glInterface.Dispose();
                _glInterface = null;
            }
            if (_display != IntPtr.Zero)
            {
                eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                if (_context != IntPtr.Zero)
                    eglDestroyContext(_display, _context);
                eglTerminate(_display);
            }
            if (_eglDevice != IntPtr.Zero)
                eglReleaseDeviceANGLE(_eglDevice);
            Release(_d3dContext);
            Release(_d3dDevice);

            _context = IntPtr.Zero;
            _config = IntPtr.Zero;
            _display = IntPtr.Zero;
            _eglDevice = IntPtr.Zero;
            _d3dContext = IntPtr.Zero;
            _d3dDevice = IntPtr.Zero;
            _lastSurface = IntPtr.Zero;
        }

        private static void LogOnce(ref bool logged, string message)
        {
            if (logged)
                return;
            logged = true;
            GnollHackX.GHApp.MaybeWriteGHLog(message, true, GHConstants.SentryGnollHackGeneralCategoryName);
        }
    }
}
#endif
