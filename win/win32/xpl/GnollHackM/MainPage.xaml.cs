using SkiaSharp;

namespace GnollHackM;

public partial class MainPage : ContentPage
{
	int count = 0;

	public MainPage()
	{
		InitializeComponent();
        canvasView.InvalidateSurface();
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
        }      
    }
}

