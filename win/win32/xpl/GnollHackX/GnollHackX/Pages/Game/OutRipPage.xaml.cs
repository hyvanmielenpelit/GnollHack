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
using Xamarin.Forms.PlatformConfiguration;
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
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

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

        private bool _tapHide = false;
#if GNH_MAUI
        private async void TapGestureRecognizer_Tapped(object sender, TappedEventArgs e)
#else
        private async void TapGestureRecognizer_Tapped(object sender, EventArgs e)
#endif
        {
            if(!_playingReplay)
            {
                _tapHide = true;
                OutRipGrid.IsEnabled = false;
                var page = await GHApp.Navigation.PopModalAsync();
                _gamePage.GenericButton_Clicked(sender, e, GHConstants.CancelChar);
                GHApp.DisconnectIViewHandlers(page);
            }
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
            if (!_playingReplay && !_tapHide)
                _gamePage.GenericButton_Clicked(sender, e, GHConstants.CancelChar);
        }

        protected override bool OnBackButtonPressed()
        {
            return true;
        }
    }
}