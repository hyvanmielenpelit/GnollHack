using System;
using System.Diagnostics;
using System.Threading;
using Microsoft.Maui.Devices;

using AvDispatcher = Avalonia.Threading.Dispatcher;
using AvPixelPointEventArgs = Avalonia.Controls.PixelPointEventArgs;
using AvScreen = Avalonia.Platform.Screen;
using AvScreenOrientation = Avalonia.Platform.ScreenOrientation;
using AvScreens = Avalonia.Controls.Screens;
using AvWindow = Avalonia.Controls.Window;

namespace GnollHackM
{
    /* IDeviceDisplay for the Avalonia desktop target. It works before any Avalonia window exists:
       until OnWindowOpened, MainDisplayInfo is 0 x 0 pixels, density 1, 60 Hz, landscape. Afterwards
       it follows the window's screen (pixel bounds, scaling, orientation), and the refresh rate is the
       frame loop's estimate, since Avalonia does not report one; the estimate snaps to the monitor's
       display-mode rate where that is known (Windows). MainDisplayInfoChanged is
       raised on the Avalonia UI thread whenever any value changes, the refresh rate included. */
    public sealed class GHAvaloniaDeviceDisplay : IDeviceDisplay
    {
        private static GHAvaloniaDeviceDisplay _current = null;

        /* The installed instance, set by GHAvaloniaEssentials.InstallEarly */
        public static GHAvaloniaDeviceDisplay Current
        {
            get { return Volatile.Read(ref _current); }
            internal set { Volatile.Write(ref _current, value); }
        }

        private readonly object _lock = new object();
        private double _width = 0;
        private double _height = 0;
        private double _density = 1.0;
        private float _refreshRate = 60.0f;
        private DisplayOrientation _orientation = DisplayOrientation.Landscape;
        private DisplayRotation _rotation = DisplayRotation.Rotation0;
        private bool _keepScreenOn = false;

        /* UI thread only */
        private AvWindow _trackedWindow = null;
        private AvScreens _trackedScreens = null;
        private IntPtr _monitor = IntPtr.Zero;
        private int _monitorRefreshRate = 0;

        public event EventHandler<DisplayInfoChangedEventArgs> MainDisplayInfoChanged;

        /* Stored only; it has no effect in this build */
        public bool KeepScreenOn
        {
            get
            {
                lock (_lock)
                {
                    return _keepScreenOn;
                }
            }
            set
            {
                lock (_lock)
                {
                    _keepScreenOn = value;
                }
            }
        }

        public DisplayInfo MainDisplayInfo
        {
            get
            {
                lock (_lock)
                {
                    return CreateDisplayInfo();
                }
            }
        }

        /* Reads the window's screen and raises MainDisplayInfoChanged; later screen, scaling and
           position changes of the same window are followed until it closes. UI thread only. */
        public void OnWindowOpened(AvWindow window)
        {
            if (window == null)
                return;

            if (!ReferenceEquals(_trackedWindow, window))
            {
                DetachFromWindow();
                _trackedWindow = window;
                window.ScalingChanged += OnWindowScreenChanged;
                window.PositionChanged += OnWindowPositionChanged;
                window.Closed += OnWindowClosed;
                try
                {
                    _trackedScreens = window.Screens;
                    if (_trackedScreens != null)
                        _trackedScreens.Changed += OnWindowScreenChanged;
                }
                catch (Exception ex)
                {
                    _trackedScreens = null;
                    Debug.WriteLine("GHAvaloniaDeviceDisplay: subscribing to screen changes failed: " + ex.Message);
                }
            }

            UpdateMonitorRefreshRate(window, true);
            UpdateFromWindow(window);
        }

        /* Updates the refresh rate from the frame loop's estimate and raises MainDisplayInfoChanged */
        public void OnRefreshRateEstimated(float hz)
        {
            if (float.IsNaN(hz) || float.IsInfinity(hz) || hz <= 0)
                return;

            DisplayInfo info;
            lock (_lock)
            {
                if (Math.Abs(_refreshRate - hz) < 0.001f)
                    return;
                _refreshRate = hz;
                info = CreateDisplayInfo();
            }
            Raise(info);
        }

        private void UpdateFromWindow(AvWindow window)
        {
            AvScreen screen = null;
            try
            {
                AvScreens screens = window.Screens;
                if (screens != null)
                    screen = screens.ScreenFromWindow(window) ?? screens.Primary;
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaDeviceDisplay: reading the window's screen failed: " + ex.Message);
                return;
            }

            if (screen == null)
                return;

            double width = screen.Bounds.Width;
            double height = screen.Bounds.Height;
            double density = screen.Scaling > 0 ? screen.Scaling : 1.0;
            DisplayOrientation orientation;
            DisplayRotation rotation;
            switch (screen.CurrentOrientation)
            {
            case AvScreenOrientation.Landscape:
                orientation = DisplayOrientation.Landscape;
                rotation = DisplayRotation.Rotation0;
                break;
            case AvScreenOrientation.Portrait:
                orientation = DisplayOrientation.Portrait;
                rotation = DisplayRotation.Rotation90;
                break;
            case AvScreenOrientation.LandscapeFlipped:
                orientation = DisplayOrientation.Landscape;
                rotation = DisplayRotation.Rotation180;
                break;
            case AvScreenOrientation.PortraitFlipped:
                orientation = DisplayOrientation.Portrait;
                rotation = DisplayRotation.Rotation270;
                break;
            default:
                orientation = height > width ? DisplayOrientation.Portrait : DisplayOrientation.Landscape;
                rotation = DisplayRotation.Rotation0;
                break;
            }

            DisplayInfo info;
            lock (_lock)
            {
                if (_width == width && _height == height && _density == density
                    && _orientation == orientation && _rotation == rotation)
                    return;
                _width = width;
                _height = height;
                _density = density;
                _orientation = orientation;
                _rotation = rotation;
                info = CreateDisplayInfo();
            }
            Raise(info);
        }

        // WORKAROUND(WA-AVALONIA-0015): Avalonia's Screen reports no refresh rate
        /* Passes the monitor's display-mode refresh rate to the frame loop. Unless forced, it is read only
           when the window has moved to another monitor: a drag raises many position changes. */
        private void UpdateMonitorRefreshRate(AvWindow window, bool force)
        {
            IntPtr monitor = GHAvaloniaMonitorRefreshRate.GetMonitor(window);
            if (!force && monitor == _monitor)
                return;
            _monitor = monitor;

            int hz = GHAvaloniaMonitorRefreshRate.GetRefreshRate(monitor);
            if (hz == _monitorRefreshRate)
                return;
            _monitorRefreshRate = hz;
            GHAvaloniaFrameLoop.SetMonitorRefreshRate(hz);
            if (hz > 0)
                GnollHackX.GHApp.MaybeWriteGHLog("Avalonia display: monitor refresh rate " + hz + " Hz");
        }

        private void OnWindowScreenChanged(object sender, EventArgs e)
        {
            AvWindow window = _trackedWindow;
            if (window != null)
            {
                UpdateMonitorRefreshRate(window, true);
                UpdateFromWindow(window);
            }
        }

        private void OnWindowPositionChanged(object sender, AvPixelPointEventArgs e)
        {
            AvWindow window = _trackedWindow;
            if (window != null)
            {
                UpdateMonitorRefreshRate(window, false);
                UpdateFromWindow(window);
            }
        }

        private void OnWindowClosed(object sender, EventArgs e)
        {
            if (ReferenceEquals(sender, _trackedWindow))
                DetachFromWindow();
        }

        private void DetachFromWindow()
        {
            AvWindow window = _trackedWindow;
            if (window != null)
            {
                window.ScalingChanged -= OnWindowScreenChanged;
                window.PositionChanged -= OnWindowPositionChanged;
                window.Closed -= OnWindowClosed;
            }
            if (_trackedScreens != null)
            {
                try
                {
                    _trackedScreens.Changed -= OnWindowScreenChanged;
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("GHAvaloniaDeviceDisplay: unsubscribing from screen changes failed: " + ex.Message);
                }
            }
            _trackedWindow = null;
            _trackedScreens = null;
            _monitor = IntPtr.Zero;
        }

        /* Called with _lock held */
        private DisplayInfo CreateDisplayInfo()
        {
            return new DisplayInfo(_width, _height, _density, _orientation, _rotation, _refreshRate);
        }

        private void Raise(DisplayInfo info)
        {
            AvDispatcher dispatcher = AvDispatcher.UIThread;
            if (dispatcher.CheckAccess())
                InvokeChanged(info);
            else
                dispatcher.Post(() => InvokeChanged(info));
        }

        private void InvokeChanged(DisplayInfo info)
        {
            EventHandler<DisplayInfoChangedEventArgs> handler = MainDisplayInfoChanged;
            handler?.Invoke(null, new DisplayInfoChangedEventArgs(info));
        }
    }
}
