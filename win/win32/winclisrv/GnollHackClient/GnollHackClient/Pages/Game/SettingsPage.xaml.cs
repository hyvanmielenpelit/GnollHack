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
            _gamePage.CursorStyle = (TTYCursorStyle)CursorPicker.SelectedIndex;
            Preferences.Set("CursorStyle", ((int)_gamePage.CursorStyle).ToString());

            _gamePage.GraphicsStyle = (GHGraphicsStyle)GraphicsPicker.SelectedIndex;
            Preferences.Set("GraphicsStyle", ((int)_gamePage.GraphicsStyle).ToString());

            _gamePage.ShowFPS = FPSSwitch.IsToggled;
            Preferences.Set("ShowFPS", _gamePage.ShowFPS ? "1" : "0");

            int res = 4, tryres = 0;
            string str = MessageLengthPicker.SelectedItem.ToString();
            if (int.TryParse(str, out tryres))
                res = tryres;

            if (res > 0)
            {
                _gamePage.NumDisplayedMessages = res;
                Preferences.Set("NumDisplayedMessages", res.ToString());
            }
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            CursorPicker.SelectedIndex = (int)_gamePage.CursorStyle;
            GraphicsPicker.SelectedIndex = (int)_gamePage.GraphicsStyle;
            FPSSwitch.IsToggled = _gamePage.ShowFPS;
            for (int i = 0; i < MessageLengthPicker.Items.Count; i++)
            {
                int tryint = 0;
                if (int.TryParse(MessageLengthPicker.Items[i].ToString(), out tryint) && tryint > 0 && tryint == _gamePage.NumDisplayedMessages)
                {
                    MessageLengthPicker.SelectedIndex = i;
                    break;
                }
            }
        }

    }
}