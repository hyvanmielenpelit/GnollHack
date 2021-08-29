using GnollHackCommon;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class GHMenuPage : ContentPage
    {
        private ClientGame _clientGame;
        private GamePage _gamePage;
        private GHWindow _ghwindow;
        private ghmenu_styles _style;
        private int _glyph;

        private ObservableCollection<GHMenuItem> _GHMenuItems = null;
        public ObservableCollection<GHMenuItem> MenuItems { get { return _GHMenuItems; } set { _GHMenuItems = value;  }  }

        public string Header { get { return HeaderLabel.Text; } set { HeaderLabel.Text = value; } }
        public bool HeaderVisible { get { return HeaderLabel.IsVisible; } set { HeaderLabel.IsVisible = value; } }

        public string Subtitle { get { return SubtitleLabel.Text; } set { SubtitleLabel.Text = value; } }
        public bool SubtitleVisible { get { return SubtitleLabel.IsVisible; } set { SubtitleLabel.IsVisible = value; } }

        public SelectionMode SelectionHow { get { return MenuView.SelectionMode; } set { MenuView.SelectionMode = value; } }

        private bool _responseSent = false;
        private bool unselect_on_tap = false;

        public GHMenuPage(GamePage gamePage, GHWindow ghwindow, ghmenu_styles style)
        {
            InitializeComponent();

            _clientGame = gamePage.ClientGame;
            _gamePage = gamePage;
            _ghwindow = ghwindow;
            _style = style;
            _glyph = _ghwindow.Glyph;
            MenuView.BindingContext = this;
            HeaderLabel.BindingContext = this;
            SubtitleLabel.BindingContext = this;

            _glyphImageSource.ReferenceGamePage = gamePage;
            _glyphImageSource.AutoSize = true;
            _glyphImageSource.ObjData = _ghwindow.ObjData;
            _glyphImageSource.Glyph = _ghwindow.Glyph;
            WindowGlyphImage.BindingContext = this;

            Assembly assembly = GetType().GetTypeInfo().Assembly;

            OKButtonImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            CancelButtonImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            //BackgroundImage.Source = ImageSource.FromResource("GnollHackClient.Assets.UI.menubackground.png", assembly);
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

        public double MenuViewWidthRequest
        {
            get
            {
                return ClientUtils.MenuViewWidthRequest(_style);
            }            
        }

        public string MenuHeaderFontFamily
        {
            get
            {
                return ClientUtils.MenuHeaderFontFamily(_style);
            }
        }

        public double MenuHeaderFontSize
        {
            get
            {
                return ClientUtils.MenuHeaderFontSize(_style);
            }
        }

        public string MenuSubtitleFontFamily
        {
            get
            {
                return ClientUtils.MenuSubtitleFontFamily(_style);
            }
        }

        public double MenuSubtitleFontSize
        {
            get
            {
                return ClientUtils.MenuSubtitleFontSize(_style);
            }
        }

        private async void CancelButton_Clicked(object sender, EventArgs e)
        {
            CancelGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await _gamePage.Navigation.PopModalAsync();
        }

        private async void OKButton_Clicked(object sender, EventArgs e)
        {
            OKGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            List<GHMenuItem> resultlist = new List<GHMenuItem>();
            if(MenuView.SelectionMode == SelectionMode.Multiple)
            {
                foreach (GHMenuItem mi in MenuView.SelectedItems)
                {
                    if(mi.Count != 0)
                    {
                        resultlist.Add(mi);
                    }
                }
            }
            else if(MenuView.SelectionMode == SelectionMode.Single)
            {
                if(MenuView.SelectedItem != null)
                {
                    GHMenuItem mi = (GHMenuItem)MenuView.SelectedItem;
                    if (mi.Count != 0)
                    {
                        resultlist.Add(mi);
                    }
                }
            }

            ConcurrentQueue<GHResponse> queue;
            if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
            {
                queue.Enqueue(new GHResponse(_clientGame, GHRequestType.ShowMenuPage, _ghwindow, resultlist));
            }
            _responseSent = true;
            await _gamePage.Navigation.PopModalAsync();
        }

        public void Process()
        {
            MenuView.ItemsSource = MenuItems;
            foreach (GHMenuItem mi in MenuItems)
            {
                if (mi.Count != 0)
                    MenuView.SelectedItems.Add(mi);
            }

        }

        private void MenuView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

            foreach(GHMenuItem mi in e.CurrentSelection)
            {
                if(mi.Identifier == 0)
                {
                    if (mi == MenuView.SelectedItem)
                    {
                        MenuView.SelectedItem = null;
                    }
                    else
                    {
                        MenuView.SelectedItems.Remove(mi);
                    }

                    if (MenuView.SelectionMode == SelectionMode.Multiple && (mi.MenuFlags & (ulong)MenuFlags.IsGroupHeading) != 0)
                    {
                        foreach (GHMenuItem o in MenuView.ItemsSource)
                        {
                            if(o.GroupAccelerator == mi.HeadingGroupAccelerator)
                            {
                                if (!mi.HeadingUnselectGroup)
                                {
                                    if (!MenuView.SelectedItems.Contains(o))
                                        MenuView.SelectedItems.Add(o);
                                }
                                else
                                {
                                    if (MenuView.SelectedItems.Contains(o))
                                        MenuView.SelectedItems.Remove(o);
                                }
                            }
                        }
                        mi.HeadingUnselectGroup = !mi.HeadingUnselectGroup;
                    }
                }
                else
                {
                    if (mi.Count == 0)
                        mi.Count = -1;
                }
                //else if(MenuView.SelectionMode == SelectionMode.Single && mi == MenuView.SelectedItem)
                //{
                //    OKButton_Clicked(sender, e);
                //    return;
                //}
            }

            foreach (GHMenuItem pmi in e.PreviousSelection)
            {
                if(!e.CurrentSelection.Contains(pmi))
                {
                    if (pmi.Count != 0)
                        pmi.Count = 0;
                }
            }

        }

        private static double _currentPageWidth = 0;
        private static double _currentPageHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);

            if(width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;
                //Refresh MenuView here
            }
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
            _gamePage.ShowWaitIcon = false;
            if (!_responseSent)
            {
                ConcurrentQueue<GHResponse> queue;
                if (ClientGame.ResponseDictionary.TryGetValue(_clientGame, out queue))
                {
                    queue.Enqueue(new GHResponse(_clientGame, GHRequestType.ShowMenuPage, _ghwindow, new List<GHMenuItem>()));
                }
            }
        }

        private void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            if(MenuView.SelectionMode == SelectionMode.Multiple)
            {
                foreach (GHMenuItem o in MenuView.ItemsSource)
                {
                    if(o.Identifier != 0)
                    {
                        if(!unselect_on_tap)
                        {
                            if (!MenuView.SelectedItems.Contains(o))
                            {
                                MenuView.SelectedItems.Add(o);
                            }
                        }
                        else
                        {
                            if (MenuView.SelectedItems.Contains(o))
                            {
                                MenuView.SelectedItems.Remove(o);
                            }
                        }
                    }
                }
                unselect_on_tap = !unselect_on_tap;
            }
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            var menuitem = ((Button)sender).BindingContext as GHMenuItem;

            if(menuitem != null)
            {
                menuitem.Count = -1;
                
                if (MenuView.SelectionMode == SelectionMode.Single && MenuView.SelectedItem != menuitem)
                    MenuView.SelectedItem = menuitem;
                else if (MenuView.SelectionMode == SelectionMode.Multiple && !MenuView.SelectedItems.Contains(menuitem))
                    MenuView.SelectedItems.Add(menuitem);
            }
        }

        private void SwipeView_SwipeStarted(object sender, SwipeStartedEventArgs e)
        {

        }

        private void SwipeView_SwipeEnded(object sender, SwipeEndedEventArgs e)
        {

        }

        private void Entry_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            var menuitem = ((Entry)sender).BindingContext as GHMenuItem;
            if (menuitem != null)
            {
                menuitem.EntryTextColor = Color.White;
            }
        }

        private void Entry_Completed(object sender, EventArgs e)
        {
            var menuitem = ((Entry)sender).BindingContext as GHMenuItem;
            if (menuitem != null)
            {
                string str = menuitem.EntryString;
                int value;
                bool res = int.TryParse(str, out value);
                if (res)
                {
                    if (value < 0 || value > menuitem.MaxCount)
                        menuitem.Count = -1;
                    else
                        menuitem.Count = value;

                    menuitem.EntryTextColor = Color.Green;
                }
                else
                {
                    menuitem.EntryTextColor = Color.Red;
                }
            }
        }

        private void menuItemCanvasView_PaintSurface(object sender, SkiaSharp.Views.Forms.SKPaintSurfaceEventArgs e)
        {
            if (sender == null)
                return;

            SKCanvasView canvasView = (SKCanvasView)sender;
            GHMenuItem menuitem = (GHMenuItem)canvasView.BindingContext;
            if(menuitem != null)
            {
                //SKImageInfo info = e.Info;
                SKSurface surface = e.Surface;
                SKCanvas canvas = surface.Canvas;

                canvas.Clear(SKColors.Transparent);

                //float width = info.Width;
                //float height = info.Height;
                float canvaswidth = canvasView.CanvasSize.Width;
                float canvasheight = canvasView.CanvasSize.Height;

                int signed_glyph = menuitem.Glyph;
                int glyph = Math.Abs(signed_glyph);
                //bool hflip = (signed_glyph < 0);
                //menuitem.GlyphImageSource.Width = (int)canvaswidth;
                //menuitem.GlyphImageSource.Height = (int)canvasheight;
                //menuitem.GlyphImageSource.DrawOnCanvas(canvas);
                if (glyph < _gamePage.Glyph2Tile.Length)
                {
                    int ntile = _gamePage.Glyph2Tile[glyph];
                    int sheet_idx = _gamePage.TileSheetIdx(ntile);
                    int tile_x = _gamePage.TileSheetX(ntile);
                    int tile_y = _gamePage.TileSheetY(ntile);
                    int tile_extra_y = GHConstants.TileHeight / 2;

                    SKRect sourcerect = new SKRect(tile_x, tile_y + tile_extra_y, tile_x + GHConstants.TileWidth, tile_y + GHConstants.TileHeight);
                    SKRect targetrect = new SKRect(0, 0, canvaswidth, canvasheight);

                    canvas.DrawBitmap(_gamePage.TileMap[sheet_idx], sourcerect, targetrect);
                }
            }
        }

        private void menuItemCanvasView_SizeChanged(object sender, EventArgs e)
        {
            if (sender == null)
                return;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            App.BackButtonPressed += BackButtonPressed;
            App.DebugWriteProfilingStopwatchTimeAndStop("ShowMenuPage Appearing");
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

    }

    public class ColumnSpanConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (int)((bool)value ? 1 : 2);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (int)((bool)value ? 1 : 2);
        }
    }

    public class MaxCountConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (int)value + 1;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (int)value + 1;
        }
    }

    public class EnableCountConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)((int)value > 1);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)((int)value > 1);
        }
    }
    public class EnablePickerConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)((int)value > 0 && (int)value <= 100);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)((int)value > 0 && (int)value <= 100);
        }
    }
    public class EnableSliderConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)((int)value > 100);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)((int)value > 100);
        }
    }
    public class ItemSourceConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            List<GHNumberPickItem> list = new List<GHNumberPickItem>();
            list.Add(new GHNumberPickItem(-1, "All"));
            for (int i = 0; i <= (int)value; i++)
            {
                list.Add(new GHNumberPickItem(i));
            }
            return list;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            List<GHNumberPickItem> list = new List<GHNumberPickItem>();
            for (int i = 0; i <= (int)value; i++)
            {
                list.Add(new GHNumberPickItem(i));
            }

            list.Add(new GHNumberPickItem(-1, "All"));

            return list;
        }
    }

    public class FontConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            if (((int)value & (int)MenuItemAttributes.Bold) != 0)
            {
                return "Diablo";
            }

            return "Underwood";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (((int)value & (int)MenuItemAttributes.Bold) != 0)
            {
                return "Diablo";
            }

            return "Underwood";
        }
    }


    public class FontSizeConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            if (((int)value & (int)MenuItemAttributes.Bold) != 0)
            {
                return 18;
            }

            return 15;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (((int)value & (int)MenuItemAttributes.Bold) != 0)
            {
                return 18;
            }

            return 15;
        }
    }
    public class FontColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ClientUtils.NHColor2XColor((int)value);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ClientUtils.NHColor2XColor((int)value);
        }
    }

    public class GrayedFontColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            Color clr = ClientUtils.NHColor2XColor((int)value);
            Color gray = Color.Black;
            Color res = new Color((clr.R + gray.R) / 2, (clr.G + gray.G) / 2, (clr.B + gray.B) / 2, clr.A);
            return res;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            Color clr = ClientUtils.NHColor2XColor((int)value);
            Color gray = Color.Black;
            Color res = new Color((clr.R + gray.R) / 2, (clr.G + gray.G) / 2, (clr.B + gray.B) / 2, clr.A);
            return res;
        }
    }

    public class PaddingConverter : IValueConverter
    {
        private static Thickness NormalPadding = new Thickness(3, 3, 2, 3);
        private static Thickness BiggerPadding = new Thickness(3, 12, 2, 3);
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            if (((ulong)value & (ulong)MenuFlags.IsHeading) != 0)
            {
                return BiggerPadding;
            }

            return NormalPadding;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (((ulong)value & (ulong)MenuFlags.IsHeading) != 0)
            {
                return BiggerPadding;
            }

            return NormalPadding;
        }
    }

}