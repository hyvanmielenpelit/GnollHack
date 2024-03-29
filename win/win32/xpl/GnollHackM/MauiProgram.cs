using Microsoft.Extensions.Logging;
using SkiaSharp.Views.Maui.Controls.Hosting;
#if IOS
using GnollHackM.Platforms.iOS;
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
#if IOS
               handlers.AddHandler(typeof(Shell), typeof(CustomShellRenderer));  
#endif
            })
            .ConfigureFonts(fonts =>
			{
				fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
				fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
                fonts.AddFont("archristy.ttf", "ARChristy");
                fonts.AddFont("DejaVuSansMono.woff", "DejaVuSansMono");
                fonts.AddFont("DejaVuSansMono-Bold.woff", "DejaVuSansMono-Bold");
                fonts.AddFont("DejaVuSansMono-BoldOblique.woff", "DejaVuSansMono-BoldOblique");
                fonts.AddFont("DejaVuSansMono-Oblique.woff", "DejaVuSansMono-Oblique");
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
