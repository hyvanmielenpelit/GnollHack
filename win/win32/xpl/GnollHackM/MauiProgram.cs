using Microsoft.Extensions.Logging;
using Microsoft.Maui.LifecycleEvents;
using SkiaSharp.Views.Maui.Controls.Hosting;
using System.Runtime.Intrinsics.Arm;
using GnollHackX;

#if IOS
using GnollHackM.Platforms.iOS;
#endif
#if WINDOWS
using GnollHackM.Platforms.Windows;
#endif

namespace GnollHackM;

public static class MauiProgram
{
	public static MauiApp CreateMauiApp()
	{
		var builder = MauiApp.CreateBuilder();
		builder
			.UseMauiApp<App>()
            .UseSkiaSharp()
            .ConfigureMauiHandlers((handlers) => {
                handlers.AddHandler(typeof(CustomLabel), typeof(AutoSizeSKCanvasViewHandler));
#if IOS
                handlers.AddHandler(typeof(Shell), typeof(CustomShellRenderer));  
#endif
#if WINDOWS
                handlers.AddHandler(typeof(NoTabStopButton), typeof(CustomButtonHandler));
                handlers.AddHandler(typeof(PointableButton), typeof(PointableButtonHandler));
#endif
            })
#if WINDOWS
            .ConfigureLifecycleEvents(events =>
            {
                // Make sure to add "using Microsoft.Maui.LifecycleEvents;" in the top of the file 
                events.AddWindows(windowsLifecycleBuilder =>
                {
                    windowsLifecycleBuilder.OnWindowCreated(window =>
                    {
                        window.ExtendsContentIntoTitleBar = false;
                        GHApp.WindowsXamlWindow = window;
                        var handle = WinRT.Interop.WindowNative.GetWindowHandle(window);
                        var id = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(handle);
                        var appWindow = Microsoft.UI.Windowing.AppWindow.GetFromWindowId(id);
                        switch (appWindow.Presenter)
                        {
                            case Microsoft.UI.Windowing.OverlappedPresenter overlappedPresenter:
                                overlappedPresenter.SetBorderAndTitleBar(false, false);
                                overlappedPresenter.Maximize();
                                break;
                        }
                    });
                });
            })
#endif            
            .ConfigureFonts(fonts =>
            {
                fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
				fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
                fonts.AddFont("archristy.ttf", "ARChristy");
                fonts.AddFont("DejaVuSansMono.ttf", "DejaVuSansMono");
                fonts.AddFont("DejaVuSansMono-Bold.ttf", "DejaVuSansMono-Bold");
                fonts.AddFont("DejaVuSansMono-BoldOblique.ttf", "DejaVuSansMono-BoldOblique");
                fonts.AddFont("DejaVuSansMono-Oblique.ttf", "DejaVuSansMono-Oblique");
                fonts.AddFont("diablo_h.ttf", "Diablo");
                fonts.AddFont("endr.ttf", "Endor");
                fonts.AddFont("Immortal-Regular.ttf", "Immortal");
                fonts.AddFont("Lato-Regular.ttf", "Lato-Regular");
                fonts.AddFont("Lato-Bold.ttf", "Lato-Bold");
                fonts.AddFont("shxi.ttf", "Xizor");
                fonts.AddFont("uwch.ttf", "Underwood");
            });

#if DEBUG
		builder.Logging.AddDebug();
#endif

		return builder.Build();
	}
}
