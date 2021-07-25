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
        private List<GHPutStrItem> _putStrItems = new List<GHPutStrItem>();

        public List<GHPutStrItem> PutStrItems { get; }
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
            TextView.ItemsSource = list;
            _glyphImageSource.ReferenceGamePage = gamePage;
            _glyphImageSource.AutoSize = true;
            _glyphImageSource.Glyph = _window.Glyph;
            //_glyphImageSource.Width = GHConstants.TileWidth * 3;
            //_glyphImageSource.Height = GHConstants.TileHeight * 2;
            WindowGlyphImage.BindingContext = this;

        }

        private void GHTextPage_Disappearing(object sender, EventArgs e)
        {
            _gamePage.GenericButton_Clicked(sender, e, 27);
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

        private bool IsGlyphVisible 
        { get 
            { 
                return (_glyph > 0 && _glyph < _gamePage.NoGlyph); 
            } 
        }
    }
}
