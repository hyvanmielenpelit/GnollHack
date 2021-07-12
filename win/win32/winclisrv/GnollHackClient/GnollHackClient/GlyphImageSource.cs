using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using GnollHackClient.Pages.Game;
using GnollHackCommon;
using SkiaSharp;
using Xamarin.Forms;

namespace GnollHackClient
{
    class GlyphImageSource : StreamImageSource
    {
        public GlyphImageSource()
        {

        }

        public override bool IsEmpty => (GamePage == null || Glyph <= 0 || Glyph >= GamePage.Glyph2Tile.Length);


        protected override void OnPropertyChanged(string propertyName)
        {
            if (propertyName == GlyphProperty.PropertyName ||
                propertyName == GamePageProperty.PropertyName ||
                propertyName == XProperty.PropertyName ||
                propertyName == YProperty.PropertyName ||
                propertyName == WidthProperty.PropertyName ||
                propertyName == HeightProperty.PropertyName)
            {
                OnSourceChanged();
            }
        }

        public override Func<CancellationToken, Task<Stream>> Stream => GetStreamAsync;

        public static readonly BindableProperty GlyphProperty = BindableProperty.Create(
            "Glyph", typeof(int), typeof(GlyphImageSource));

        public int Glyph
        {
            get => (int)GetValue(GlyphProperty);
            set => SetValue(GlyphProperty, value);
        }

        public static readonly BindableProperty XProperty = BindableProperty.Create(
            "X", typeof(int), typeof(GlyphImageSource));

        public int X
        {
            get => (int)GetValue(XProperty);
            set => SetValue(XProperty, value);
        }

        public static readonly BindableProperty YProperty = BindableProperty.Create(
            "Y", typeof(int), typeof(GlyphImageSource));

        public int Y
        {
            get => (int)GetValue(YProperty);
            set => SetValue(YProperty, value);
        }

        public static readonly BindableProperty WidthProperty = BindableProperty.Create(
            "Width", typeof(int), typeof(GlyphImageSource));

        public int Width
        {
            get => (int)GetValue(WidthProperty);
            set => SetValue(WidthProperty, value);
        }

        public static readonly BindableProperty HeightProperty = BindableProperty.Create(
            "Height", typeof(int), typeof(GlyphImageSource));

        public int Height
        {
            get => (int)GetValue(HeightProperty);
            set => SetValue(HeightProperty, value);
        }

        public static readonly BindableProperty GamePageProperty = BindableProperty.Create(
            "GamePage", typeof(GamePage), typeof(GlyphImageSource));

        public GamePage GamePage
        {
            get => (GamePage)GetValue(GamePageProperty);
            set => SetValue(GamePageProperty, value);
        }

        public Task<Stream> GetStreamAsync(CancellationToken userToken = new CancellationToken())
        {
            base.OnLoadingStarted();
            userToken.Register(CancellationTokenSource.Cancel);
            var result = Draw();
            OnLoadingCompleted(CancellationTokenSource.IsCancellationRequested);
            return Task.FromResult(result);

        }

        private Stream Draw()
        {
            var bitmap = new SKBitmap(Width, Height, SKImageInfo.PlatformColorType, SKAlphaType.Premul);
            var canvas = new SKCanvas(bitmap);
            canvas.Clear(SKColors.Transparent);
            int signed_glyph = Glyph;
            int abs_glyph = Math.Abs(signed_glyph);

            if (GamePage != null && abs_glyph > 0 && Width > 0 && Height > 0 && abs_glyph < GamePage.Glyph2Tile.Length)
            {
                int ntile = GamePage.Glyph2Tile[abs_glyph];
                int sheet_idx = GamePage.TileSheetIdx(ntile);
                int tile_x = GamePage.TileSheetX(ntile);
                int tile_y = GamePage.TileSheetY(ntile);

                SKRect sourcerect = new SKRect(tile_x + X, tile_y + Y, tile_x + X + Width, tile_y + Y + Height);

                SKRect targetrect = new SKRect(0, 0, Width, Height);

                canvas.DrawBitmap(GamePage.TileMap[sheet_idx], sourcerect, targetrect);
            }

            var skImage = SKImage.FromBitmap(bitmap);
            var result = (skImage.Encode(SKEncodedImageFormat.Png, 100)).AsStream();
            return result;
        }
    }
}
