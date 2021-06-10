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
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            CursorPicker.SelectedIndex = (int)_gamePage.CursorStyle;
            GraphicsPicker.SelectedIndex = (int)_gamePage.GraphicsStyle;
        }

    }
}