using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class OutRipPage : ContentPage
    {
        GamePage _gamePage;
        GHWindow _window;
        int _glyph;

        public OutRipPage(GamePage gamePage, GHWindow window, GHOutRipInfo outripinfo)
        {
            InitializeComponent();
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
            _gamePage = gamePage;
            _window = window;
            _glyph = _window.Glyph;
            NameLabel.Text = outripinfo.PlayerName;
            PointsLabel.Text = outripinfo.Points + " points";
            KillerLabel.Text = outripinfo.Killer;
            _glyphImageSource.ReferenceGamePage = gamePage;
            _glyphImageSource.UseUpperSide = false;
            _glyphImageSource.AutoSize = true;
            _glyphImageSource.Grayed = true;
            _glyphImageSource.Opacity = 0.5f;
            _glyphImageSource.Glyph = _window.Glyph;
            PlayerImage.BindingContext = this;
        }

        private async void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private GlyphImageSource _glyphImageSource = new GlyphImageSource();

        public ImageSource GlyphImage
        {
            get
            {
                return _glyphImageSource;
            }
        }

        public bool IsGlyphVisible
        {
            get
            {
                return (Math.Abs(_glyph) > 0 && _glyph != _gamePage.NoGlyph);
            }
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            _gamePage.GenericButton_Clicked(sender, e, 27);
        }
    }
}