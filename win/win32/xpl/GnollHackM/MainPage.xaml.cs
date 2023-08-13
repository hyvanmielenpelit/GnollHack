using SkiaSharp;
using GnollHackX;

namespace GnollHackM;

public partial class MainPage : ContentPage
{
	int count = 0;

	public MainPage()
	{
		InitializeComponent();
        App.CurrentMainPage = this;

        canvasView.InvalidateSurface();
        count = GHConstants.AllMessageRows;
        count = 0;
	}

	private void OnCounterClicked(object sender, EventArgs e)
	{
		count++;

		if (count == 1)
			CounterBtn.Text = $"Clicked {count} time";
		else
			CounterBtn.Text = $"Clicked {count} times";

		SemanticScreenReader.Announce(CounterBtn.Text);
	}

    private void canvasView_PaintSurface(object sender, SkiaSharp.Views.Maui.SKPaintSurfaceEventArgs e)
    {
        SKImageInfo info = e.Info;
        SKSurface surface = e.Surface;
        SKCanvas canvas = surface.Canvas;
        string str = "";
        float xText = 0;
        float yText = 100;
        float canvaswidth = canvasView.CanvasSize.Width;
        float canvasheight = canvasView.CanvasSize.Height;

        using (SKPaint textPaint = new SKPaint())
        {
            str = "Test! (" + canvaswidth + ", " + canvasheight + ")";
            textPaint.TextSize = 26;
            textPaint.Color = SKColors.Red;
            canvas.DrawText(str, xText, yText, textPaint);
            xText = 0;
            yText += textPaint.FontSpacing;
            str = App.GnollHackService.GetVersionId();
            canvas.DrawText(str, xText, yText, textPaint);
        }
    }

    public void Suspend()
    {

    }

    public void Resume()
    {

    }

    private async void ContentPage_Appearing(object sender, EventArgs e)
    {
        App.ForceCopyAllBanksToDisk = true;
        App.ReadSecrets();
        await App.GnollHackService.InitializeSecrets(App.CurrentSecrets);
        string gnhpath = App.GnollHackService.GetGnollHackPath();
        App.FmodService.InitializeFmod();
        App.FmodService.AddLoadableSoundBank(Path.Combine(gnhpath, "bank", "Master.strings.bank"), 0, false, false);
        App.FmodService.AddLoadableSoundBank(Path.Combine(gnhpath, "bank", "Master.bank"), 0, false, false);
        App.FmodService.AddLoadableSoundBank(Path.Combine(gnhpath, "bank", "Preliminary.bank"), 0, false, false);
        App.FmodService.AddLoadableSoundBank(Path.Combine(gnhpath, "bank", "Music.bank"), 0, false, false);
        //App.FmodService.AddLoadableSoundBank(App.PlatformService.GetAssetsPath() + "Platforms/Android/banks/Preliminary.bank", 0, true, false);
        //App.FmodService.AddLoadableSoundBank(App.PlatformService.GetAssetsPath() + "Platforms/Android/banks/Master.bank", 0, true, false);
        //App.FmodService.AddLoadableSoundBank(App.PlatformService.GetAssetsPath() + "Platforms/Android/banks/Master.strings.bank", 0, true, false);
        //App.FmodService.AddLoadableSoundBank(App.PlatformService.GetAssetsPath() + "Platforms/Android/banks/Music.bank", 0, true, false);
        App.FmodService.LoadBanks(0);
        App.FmodService.PlayTestSound();
        App.PlayButtonClickedSound();
    }

    private void ContentPage_Disappearing(object sender, EventArgs e)
    {
        App.FmodService.StopTestSound();
    }
}

