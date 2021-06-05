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
                    if(!mi.CountSet)
                        mi.Count = -1;

                    if((mi.Count > 0 &&  mi.MaxCount > 0) || mi.MaxCount == 0)
                    {
                        resultlist.Add(mi);
                        mi.Selected = true;
                    }
                }
            }
            else if(MenuView.SelectionMode == SelectionMode.Single)
            {
                if(MenuView.SelectedItem != null)
                {
                    GHMenuItem mi = (GHMenuItem)MenuView.SelectedItem;
                    if (!mi.CountSet)
                        mi.Count = -1;

                    if ((mi.Count > 0 && mi.MaxCount > 0) || mi.MaxCount == 0)
                    {
                        mi.Selected = true;
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

                    if(MenuView.SelectionMode == SelectionMode.Multiple && (mi.MenuFlags & (Int32)MenuFlags.IsGroupHeading) != 0)
                    {
                        foreach (GHMenuItem o in MenuView.ItemsSource)
                        {
                            if(o.GroupAccelerator == mi.HeadingGroupAccelerator)
                            {
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

        private void Slider_ValueChanged(object sender, ValueChangedEventArgs e)
        {
            Slider s = (Slider)sender;
            var newVal = Math.Round(e.NewValue);
            s.Value = newVal;

            var menuitem = s.BindingContext as GHMenuItem;
            if(menuitem != null)
            {
                //menuitem.CountSet = true;
                //menuitem.Count = newVal > menuitem.MaxCount ? -1 : (int)newVal;
                //menuitem.SelectedPickerIndex = menuitem.Count;
                //menuitem.SelectedSliderValue = menuitem.Count;

                if (menuitem.Count != 0)
                {
                    if (MenuView.SelectionMode == SelectionMode.Single && MenuView.SelectedItem != menuitem)
                        MenuView.SelectedItem = menuitem;
                    else if (MenuView.SelectionMode == SelectionMode.Multiple && !MenuView.SelectedItems.Contains(menuitem))
                        MenuView.SelectedItems.Add(menuitem);
                }
                else
                {
                    if (MenuView.SelectionMode == SelectionMode.Single && MenuView.SelectedItem == menuitem)
                        MenuView.SelectedItem = null;
                    else if (MenuView.SelectionMode == SelectionMode.Multiple && MenuView.SelectedItems.Contains(menuitem))
                        MenuView.SelectedItems.Remove(menuitem);
                }
            }
        }

        private void Picker_SelectedIndexChanged(object sender, EventArgs e)
        {
            Picker p = (Picker)sender;
            var menuitem = p.BindingContext as GHMenuItem;
            if(menuitem != null && p.SelectedItem != null)
            {
                /*
                menuitem.CountSet = true;
                menuitem.Count = ((GHNumberPickItem)p.SelectedItem).Number;
                if(menuitem.Count == -1)
                {
                    menuitem.SelectedPickerIndex = menuitem.MaxCount + 1;
                    menuitem.SelectedSliderValue = menuitem.MaxCount + 1;
                }
                else 
                {
                    menuitem.SelectedPickerIndex = menuitem.Count;
                    menuitem.SelectedSliderValue = menuitem.Count;
                }
                */
                if(menuitem.Count != 0)
                {
                    if (MenuView.SelectionMode == SelectionMode.Single && MenuView.SelectedItem != menuitem)
                        MenuView.SelectedItem = menuitem;
                    else if (MenuView.SelectionMode == SelectionMode.Multiple && !MenuView.SelectedItems.Contains(menuitem))
                        MenuView.SelectedItems.Add(menuitem);
                }
                else
                {
                    if (MenuView.SelectionMode == SelectionMode.Single && MenuView.SelectedItem == menuitem)
                        MenuView.SelectedItem = null;
                    else if (MenuView.SelectionMode == SelectionMode.Multiple && MenuView.SelectedItems.Contains(menuitem))
                        MenuView.SelectedItems.Remove(menuitem);
                }

            }
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            var menuitem = ((Button)sender).BindingContext as GHMenuItem;
            if(menuitem != null)
            {
//                menuitem.CountSet = true;
                menuitem.Count = -1;
//                menuitem.SelectedPickerIndex = menuitem.MaxCount + 1;
//                menuitem.SelectedSliderValue = menuitem.MaxCount + 1;
                if (MenuView.SelectionMode == SelectionMode.Single && MenuView.SelectedItem != menuitem)
                    MenuView.SelectedItem = menuitem;
                else if (MenuView.SelectionMode == SelectionMode.Multiple && !MenuView.SelectedItems.Contains(menuitem))
                    MenuView.SelectedItems.Add(menuitem);
            }
        }
    }


    public class DisplayLabelTextConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            string str = "";
            if((double)values[0] + 1 >= (double)values[1])
                str = "All";
            else
                str = String.Format("{0}", values[1]);

            return str;
        }

        public object[] ConvertBack(object values, Type[] targetType, object parameter, CultureInfo culture)
        {
            object[] res = new object[2];
            return res;
        }
    }
    public class CountLabelVisibilityConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)((int)values[0] > 0 && (int)values[1] > 0);
        }

        public object[] ConvertBack(object values, Type[] targetType, object parameter, CultureInfo culture)
        {
            return new object[2];
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
            return (bool)((int)value > 0);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)((int)value > 0);
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