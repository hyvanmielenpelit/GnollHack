using GnollHackCommon;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

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

        private List<GHMenuItem> _GHMenuItems = new List<GHMenuItem>();
        public List<GHMenuItem> MenuItems { get { return _GHMenuItems; } }

        public string Header { get { return HeaderLabel.Text; } set { HeaderLabel.Text = value; } }
        public SelectionMode SelectionHow { get { return MenuView.SelectionMode; } set { MenuView.SelectionMode = value; } }

        private bool _responseSent = false;
        public GHMenuPage(GamePage gamepage, GHWindow ghwindow)
        {
            InitializeComponent();
            MenuView.ItemsSource = MenuItems;
            _clientGame = gamepage.ClientGame;
            _gamePage = gamepage;
            _ghwindow = ghwindow;

            Assembly assembly = GetType().GetTypeInfo().Assembly;

            myImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
            myImage2.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);

        }


        private async void CancelButton_Clicked(object sender, EventArgs e)
        {
            await _gamePage.Navigation.PopModalAsync();
        }

        private async void OKButton_Clicked(object sender, EventArgs e)
        {
            List<GHMenuItem> resultlist = new List<GHMenuItem>();
            if(MenuView.SelectionMode == SelectionMode.Multiple)
            {
                foreach (GHMenuItem mi in MenuView.SelectedItems)
                {
                    resultlist.Add(mi);
                    mi.Count = -1;
                    mi.Selected = true;
                }
            }
            else if(MenuView.SelectionMode == SelectionMode.Single)
            {
                if(MenuView.SelectedItem != null)
                {
                    GHMenuItem mi = (GHMenuItem)MenuView.SelectedItem;
                    mi.Count = -1;
                    mi.Selected = true;
                    resultlist.Add(mi);
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
            foreach(GHMenuItem mi in MenuItems)
            {
                if (mi.Selected)
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

                    if((mi.MenuFlags & (ulong)MenuFlags.IsGroupHeading) != 0)
                    {
                        foreach (GHMenuItem o in MenuView.ItemsSource)
                        {
                            if(o.GroupAccelerator == mi.HeadingGroupAccelerator)
                            {
                                o.Selected = true;
                                if (!MenuView.SelectedItems.Contains(o))
                                    MenuView.SelectedItems.Add(o);
                                else
                                    MenuView.SelectedItems.Remove(o);
                            }
                        }
                    }
                }
                //else if(MenuView.SelectionMode == SelectionMode.Single && mi == MenuView.SelectedItem)
                //{
                //    OKButton_Clicked(sender, e);
                //    return;
                //}
            }
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            if(!_responseSent)
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
            foreach(GHMenuItem o in MenuView.ItemsSource)
            {
                o.Selected = true;
                if(!MenuView.SelectedItems.Contains(o))
                    MenuView.SelectedItems.Add(o);
                else
                    MenuView.SelectedItems.Remove(o);
            }
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
                return 24;
            }

            return 20;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (((int)value & (int)MenuItemAttributes.Bold) != 0)
            {
                return 24;
            }

            return 20;
        }
    }
    public class FontColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return GHUtils.NHColor2Color((nhcolor)value);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return GHUtils.NHColor2Color((nhcolor)value);
        }
    }

    public class PaddingConverter : IValueConverter
    {
        private static Thickness NormalPadding = new Thickness(3, 6, 3, 6);
        private static Thickness BiggerPadding = new Thickness(3, 15, 3, 6);
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {

            if (((int)value & (int)MenuItemAttributes.Bold) != 0)
            {
                return BiggerPadding;
            }

            return NormalPadding;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (((int)value & (int)MenuItemAttributes.Bold) != 0)
            {
                return BiggerPadding;
            }

            return NormalPadding;
        }
    }

}