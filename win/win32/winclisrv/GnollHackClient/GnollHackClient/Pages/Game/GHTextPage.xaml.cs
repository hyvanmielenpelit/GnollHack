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
                ProcessAdjustedItems(_adjustedPutStrItems, _putStrItems);
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

        private void ProcessAdjustedItems(List<GHPutStrItem> adjusted_list, List<GHPutStrItem> normal_list)
        {
            adjusted_list.Clear();
            GHPutStrItem newpsi = null;
            
            for (int cnt = 0; cnt < normal_list.Count; cnt++)
            {
                GHPutStrItem psi = normal_list[cnt];
                if (newpsi != null && (psi.Text == ""))
                {
                    adjusted_list.Add(newpsi);
                    newpsi = null;
                }

                if (psi.Text == "")
                {
                    adjusted_list.Add(psi);
                }
                else
                {
                    bool isnewpsi = false;
                    if(newpsi == null)
                    {
                        newpsi = new GHPutStrItem(psi.ReferenceGamePage, psi.Window, "");
                        isnewpsi = true;
                    }

                    if (newpsi.Text != "")
                    {
                        newpsi.Text += " ";
                        newpsi.InstructionList.Add(new GHPutStrInstructions((int)MenuItemAttributes.None, (int)nhcolor.NO_COLOR, 1));
                    }

                    int spacecnt = 0;
                    foreach(char ch in psi.Text)
                    {
                        if (ch == ' ')
                            spacecnt++;
                        else
                            break;
                    }
                    newpsi.Text += psi.Text.TrimStart(' ');
                    if(spacecnt > 0) 
                    {
                        if(isnewpsi)
                            newpsi.PaddingAmount = spacecnt;

                        if (psi.InstructionList.Count > 0)
                        {
                            if (psi.InstructionList[0].PrintLength > spacecnt)
                                newpsi.InstructionList.Add(new GHPutStrInstructions(psi.InstructionList[0].Attributes, psi.InstructionList[0].Color, psi.InstructionList[0].PrintLength - spacecnt));
                            for (int i = 1; i < psi.InstructionList.Count; i++)
                                newpsi.InstructionList.Add(psi.InstructionList[i]);
                        }
                    }
                    else
                        newpsi.InstructionList.AddRange(psi.InstructionList);

                    if (newpsi != null && (cnt == normal_list.Count - 1))
                    {
                        adjusted_list.Add(newpsi);
                        break;
                    }
                }
            }
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
        }
    }
}
