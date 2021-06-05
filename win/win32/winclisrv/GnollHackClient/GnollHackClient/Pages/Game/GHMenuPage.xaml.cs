using GnollHackCommon;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
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

        private List<GHMenuItem> _GHMenuItems = new List<GHMenuItem>();
        public List<GHMenuItem> MenuItems { get { return _GHMenuItems; } }

        public string Header { get { return HeaderLabel.Text; } set { HeaderLabel.Text = value; } }
        public SelectionMode SelectionHow { get { return MenuView.SelectionMode; } set { MenuView.SelectionMode = value; } }

        private bool _responseSent = false;
        private bool unselect_on_tap = false;

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
                    if((mi.Count > 0 &&  mi.MaxCount > 0) || mi.MaxCount == 0)
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
                    if ((mi.Count > 0 && mi.MaxCount > 0) || mi.MaxCount == 0)
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
            foreach(GHMenuItem mi in MenuItems)
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

                    if (MenuView.SelectionMode == SelectionMode.Multiple && (mi.MenuFlags & (Int32)MenuFlags.IsGroupHeading) != 0)
                    {
                        foreach (GHMenuItem o in MenuView.ItemsSource)
                        {
                            if(o.GroupAccelerator == mi.HeadingGroupAccelerator)
                            {
                                if (!MenuView.SelectedItems.Contains(o))
                                {
                                    MenuView.SelectedItems.Add(o);
                                }
                                else
                                {
                                    MenuView.SelectedItems.Remove(o);
                                }
                            }
                        }
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

        private void CountOKButton_Clicked(object sender, EventArgs e)
        {
            var menuitem = ((Button)sender).BindingContext as GHMenuItem;
            if (menuitem != null)
            {
                string str = menuitem.EntryString;
                int value;
                bool res = int.TryParse(str, out value);
                if(res)
                {
                    if(value < 0 || value > menuitem.MaxCount)
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

        private void CountOKButton_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Text")
            {
                var menuitem = ((Button)sender).BindingContext as GHMenuItem;
                if (menuitem != null)
                {
                    menuitem.EntryTextColor = Color.White;
                }
            }
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
            for(int i = 0; i <= (int)value; i++)
            {
                list.Add(new GHNumberPickItem(i));
            }

            if ((int)value > 0)
                list.Add(new GHNumberPickItem(-1, "All"));

            return list;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            List<GHNumberPickItem> list = new List<GHNumberPickItem>();
            for (int i = 0; i < (int)value; i++)
            {
                list.Add(new GHNumberPickItem(i));
            }

            if((int)value > 0)
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
                return 24;
            }

            return 18;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (((int)value & (int)MenuItemAttributes.Bold) != 0)
            {
                return 24;
            }

            return 18;
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

            if (((UInt32)value & (UInt32)MenuFlags.IsHeading) != 0)
            {
                return BiggerPadding;
            }

            return NormalPadding;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (((UInt32)value & (UInt32)MenuFlags.IsHeading) != 0)
            {
                return BiggerPadding;
            }

            return NormalPadding;
        }
    }

}