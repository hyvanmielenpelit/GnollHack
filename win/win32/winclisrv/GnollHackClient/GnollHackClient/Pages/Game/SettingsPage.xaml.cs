using GnollHackCommon;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using Xamarin.Essentials;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SettingsPage : ContentPage
    {
        private GamePage _gamePage;
        public SettingsPage(GamePage gamePage)
        {
            InitializeComponent();
            _gamePage = gamePage;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            if(_gamePage != null)
                _gamePage.CursorStyle = (TTYCursorStyle)CursorPicker.SelectedIndex;
            Preferences.Set("CursorStyle", CursorPicker.SelectedIndex);

            if (_gamePage != null)
                _gamePage.GraphicsStyle = (GHGraphicsStyle)GraphicsPicker.SelectedIndex;
            Preferences.Set("GraphicsStyle", GraphicsPicker.SelectedIndex);

            if (_gamePage != null)
                _gamePage.ShowFPS = FPSSwitch.IsToggled;
            Preferences.Set("ShowFPS", FPSSwitch.IsToggled);

            App.HideAndroidNavigatioBar = NavBarSwitch.IsToggled;
            Preferences.Set("HideAndroidNavigationBar", App.HideAndroidNavigatioBar);

            App.DeveloperMode = DeveloperSwitch.IsToggled;
            Preferences.Set("DeveloperMode", App.DeveloperMode);

            int res = 4, tryres = 0;
            string str = MessageLengthPicker.SelectedItem.ToString();
            if (int.TryParse(str, out tryres))
                res = tryres;

            if (res > 0)
            {
                if (_gamePage != null)
                    _gamePage.NumDisplayedMessages = res;
                Preferences.Set("NumDisplayedMessages", res);
            }
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            int cursor = 0, graphics = 0, msgnum = 0;
            bool fps = false, navbar = false, devmode = false;

            navbar = App.HideAndroidNavigatioBar;
            devmode = App.DeveloperMode;
            if (_gamePage == null)
            {
                cursor = Preferences.Get("CursorStyle", 1);
                graphics = Preferences.Get("GraphicsStyle", 1);
                fps = Preferences.Get("ShowFPS", false);
                msgnum = Preferences.Get("NumDisplayedMessages", GHConstants.DefaultMessageRows);
            }
            else
            {
                cursor = (int)_gamePage.CursorStyle;
                graphics = (int)_gamePage.GraphicsStyle;
                fps = _gamePage.ShowFPS;
                msgnum = _gamePage.NumDisplayedMessages;
            }
            CursorPicker.SelectedIndex = cursor;
            GraphicsPicker.SelectedIndex = graphics;
            FPSSwitch.IsToggled = fps;
            NavBarSwitch.IsToggled = navbar;
            DeveloperSwitch.IsToggled = devmode;
            for (int i = 0; i < MessageLengthPicker.Items.Count; i++)
            {
                int tryint = 0;
                if (int.TryParse(MessageLengthPicker.Items[i].ToString(), out tryint) && tryint > 0 && tryint == msgnum)
                {
                    MessageLengthPicker.SelectedIndex = i;
                    break;
                }
            }
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

    }
}