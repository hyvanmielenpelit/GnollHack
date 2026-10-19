using System;
using System.Globalization;
using System.Reflection;
using System.Threading;
using Microsoft.Maui.ApplicationModel;

using AvApplication = Avalonia.Application;
using AvDispatcher = Avalonia.Threading.Dispatcher;
using AvThemeVariant = Avalonia.Styling.ThemeVariant;

namespace GnollHackM
{
    /* IAppInfo for the Avalonia desktop target: an unpackaged app whose version comes from the entry
       assembly. BuildString is the version's build component, the number GHApp reads on Windows. */
    public sealed class GHAvaloniaAppInfo : IAppInfo
    {
        private readonly Version _version;
        private readonly string _versionString;
        private readonly string _buildString;

        /* The last theme read on the UI thread, returned to other threads */
        private int _lastTheme = (int)Microsoft.Maui.ApplicationModel.AppTheme.Unspecified;

        public GHAvaloniaAppInfo()
        {
            Assembly assembly = Assembly.GetEntryAssembly() ?? typeof(GHAvaloniaAppInfo).Assembly;
            Version version = assembly.GetName().Version ?? new Version(1, 0, 0, 0);
            _version = version;
            _versionString = version.ToString();
            _buildString = (version.Build >= 0 ? version.Build : 0).ToString(CultureInfo.InvariantCulture);
        }

        public string PackageName => "com.soundmindentertainment.gnollhack";

        public string Name => "GnollHack";

        public string VersionString => _versionString;

        public Version Version => _version;

        public string BuildString => _buildString;

        /* No settings UI to open on the desktop */
        public void ShowSettingsUI()
        {
        }

        /* Avalonia's ActualThemeVariant, which can be read only on the UI thread */
        public Microsoft.Maui.ApplicationModel.AppTheme RequestedTheme
        {
            get
            {
                if (!AvDispatcher.UIThread.CheckAccess())
                    return (Microsoft.Maui.ApplicationModel.AppTheme)Volatile.Read(ref _lastTheme);

                Microsoft.Maui.ApplicationModel.AppTheme theme = Microsoft.Maui.ApplicationModel.AppTheme.Unspecified;
                AvApplication application = AvApplication.Current;
                if (application != null)
                {
                    AvThemeVariant variant = application.ActualThemeVariant;
                    while (variant != null)
                    {
                        if (variant == AvThemeVariant.Dark)
                        {
                            theme = Microsoft.Maui.ApplicationModel.AppTheme.Dark;
                            break;
                        }
                        if (variant == AvThemeVariant.Light)
                        {
                            theme = Microsoft.Maui.ApplicationModel.AppTheme.Light;
                            break;
                        }
                        variant = variant.InheritVariant;
                    }
                }
                Volatile.Write(ref _lastTheme, (int)theme);
                return theme;
            }
        }

        public AppPackagingModel PackagingModel => AppPackagingModel.Unpackaged;

        public Microsoft.Maui.ApplicationModel.LayoutDirection RequestedLayoutDirection
            => Microsoft.Maui.ApplicationModel.LayoutDirection.LeftToRight;
    }
}
