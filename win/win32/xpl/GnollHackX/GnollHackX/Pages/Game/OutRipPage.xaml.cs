using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.Game
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class OutRipPage : ContentPage
    {
        GamePage _gamePage;
        GHWindow _window;
        int _glyph;
        bool _playingReplay = false;

        public OutRipPage(GamePage gamePage, GHWindow window, GHOutRipInfo outripinfo)
        {
            InitializeComponent();
#if GNH_MAUI
            On<iOS>().SetUseSafeArea(true);
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif
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
            _playingReplay = gamePage.PlayingReplay;
        }

        private async void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            if(!_playingReplay)
                await App.Current.MainPage.Navigation.PopModalAsync();
        }

        private GlyphImageSource _glyphImageSource = new GlyphImageSource();

        public GlyphImageSource GlyphImage
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
                return (Math.Abs(_glyph) > 0 && _glyph != GHApp.NoGlyph);
            }
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            if (!_playingReplay)
                _gamePage.GenericButton_Clicked(sender, e, 27);
        }
    }
}