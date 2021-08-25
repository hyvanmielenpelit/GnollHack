using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GHTextPage : ContentPage
    {
        private List<GHPutStrItem> _putStrItems = null;

        public List<GHPutStrItem> PutStrItems { get { return _putStrItems; } }
        private List<GHPutStrItem> _adjustedPutStrItems = null;
        public List<GHPutStrItem> AdjustedPutStrItems { get { return _adjustedPutStrItems; } }

        private GamePage _gamePage;
        private ClientGame _clientGame;
        private GHWindow _window;
        private int _glyph;

        public GHTextPage(GamePage gamePage, GHWindow window, List<GHPutStrItem> list)
        {
            InitializeComponent();

            _gamePage = gamePage;
            _clientGame = _gamePage.ClientGame;
            _window = window;
            _glyph = _window.Glyph;
            _putStrItems = list;
            if (window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_GENERAL || window.WindowStyle == ghwindow_styles.GHWINDOW_STYLE_PAGER_SPEAKER)
            {
                _adjustedPutStrItems = new List<GHPutStrItem>();
                ClientUtils.ProcessAdjustedItems(_adjustedPutStrItems, _putStrItems);
                TextView.ItemsSource = _adjustedPutStrItems;
            }
            else
                TextView.ItemsSource = _putStrItems;

            _glyphImageSource.ReferenceGamePage = gamePage;
            _glyphImageSource.AutoSize = true;
            _glyphImageSource.ObjData = _window.ObjData;
            _glyphImageSource.Glyph = _window.Glyph;
            WindowGlyphImage.BindingContext = this;

        }

        private void GHTextPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
            _gamePage.GenericButton_Clicked(sender, e, 27);
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
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
        { get 
            { 
                return (Math.Abs(_glyph) > 0 && _glyph != _gamePage.NoGlyph); 
            } 
        }


        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
        }
    }
}
