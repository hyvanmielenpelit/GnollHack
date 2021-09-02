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
        private bool _doChangeVolume = false;
        public SettingsPage(GamePage gamePage)
        {
            InitializeComponent();
            _gamePage = gamePage;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            App.BackButtonPressed -= BackButtonPressed;
            _doChangeVolume = false;

            if (_gamePage != null)
                _gamePage.CursorStyle = (TTYCursorStyle)CursorPicker.SelectedIndex;
            Preferences.Set("CursorStyle", CursorPicker.SelectedIndex);

            if (_gamePage != null)
                _gamePage.GraphicsStyle = (GHGraphicsStyle)GraphicsPicker.SelectedIndex;
            Preferences.Set("GraphicsStyle", GraphicsPicker.SelectedIndex);

            if (_gamePage != null)
                _gamePage.ShowMemoryUsage = MemorySwitch.IsToggled;
            Preferences.Set("ShowMemoryUsage", MemorySwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowFPS = FPSSwitch.IsToggled;
            Preferences.Set("ShowFPS", FPSSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.HitPointBars = HitPointBarSwitch.IsToggled;
            Preferences.Set("HitPointBars", HitPointBarSwitch.IsToggled);

            App.HideAndroidNavigatioBar = NavBarSwitch.IsToggled;
            Preferences.Set("HideAndroidNavigationBar", App.HideAndroidNavigatioBar);

            App.DeveloperMode = DeveloperSwitch.IsToggled;
            Preferences.Set("DeveloperMode", App.DeveloperMode);

            Preferences.Set("GeneralVolume", (float)GeneralVolumeSlider.Value);
            Preferences.Set("MusicVolume", (float)MusicVolumeSlider.Value);
            Preferences.Set("AmbientVolume", (float)AmbientVolumeSlider.Value);
            Preferences.Set("DialogueVolume", (float)DialogueVolumeSlider.Value);
            Preferences.Set("EffectsVolume", (float)EffectsVolumeSlider.Value);
            Preferences.Set("UIVolume", (float)UIVolumeSlider.Value);
            App.FmodService.AdjustVolumes((float)GeneralVolumeSlider.Value, (float)MusicVolumeSlider.Value, (float)AmbientVolumeSlider.Value, (float)DialogueVolumeSlider.Value, (float)EffectsVolumeSlider.Value, (float)UIVolumeSlider.Value);

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
            App.BackButtonPressed += BackButtonPressed;

            int cursor = 0, graphics = 0, msgnum = 0;
            bool mem = false, fps = false, navbar = false, devmode = false, hpbars = false;
            float generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume;
            generalVolume = Preferences.Get("GeneralVolume", 1.0f);
            musicVolume = Preferences.Get("MusicVolume", 0.5f);
            ambientVolume = Preferences.Get("AmbientVolume", 0.5f);
            dialogueVolume = Preferences.Get("DialogueVolume", 0.5f);
            effectsVolume = Preferences.Get("EffectsVolume", 0.5f);
            UIVolume = Preferences.Get("UIVolume", 0.5f);
            navbar = App.HideAndroidNavigatioBar;
            devmode = App.DeveloperMode;
            if (_gamePage == null)
            {
                cursor = Preferences.Get("CursorStyle", 1);
                graphics = Preferences.Get("GraphicsStyle", 1);
                hpbars = Preferences.Get("HitPointBars", false);
                mem = Preferences.Get("ShowMemoryUsage", false);
                fps = Preferences.Get("ShowFPS", false);
                msgnum = Preferences.Get("NumDisplayedMessages", GHConstants.DefaultMessageRows);
            }
            else
            {
                cursor = (int)_gamePage.CursorStyle;
                graphics = (int)_gamePage.GraphicsStyle;
                hpbars = _gamePage.HitPointBars;
                mem = _gamePage.ShowMemoryUsage;
                fps = _gamePage.ShowFPS;
                msgnum = _gamePage.NumDisplayedMessages;
            }
            CursorPicker.SelectedIndex = cursor;
            GraphicsPicker.SelectedIndex = graphics;
            HitPointBarSwitch.IsToggled = hpbars;
            MemorySwitch.IsToggled = mem;
            FPSSwitch.IsToggled = fps;
            NavBarSwitch.IsToggled = navbar;
            DeveloperSwitch.IsToggled = devmode;
            GeneralVolumeSlider.Value = (double)generalVolume;
            MusicVolumeSlider.Value = (double)musicVolume;
            AmbientVolumeSlider.Value = (double)ambientVolume;
            DialogueVolumeSlider.Value = (double)dialogueVolume;
            EffectsVolumeSlider.Value = (double)effectsVolume;
            UIVolumeSlider.Value = (double)UIVolume;
            for (int i = 0; i < MessageLengthPicker.Items.Count; i++)
            {
                int tryint = 0;
                if (int.TryParse(MessageLengthPicker.Items[i].ToString(), out tryint) && tryint > 0 && tryint == msgnum)
                {
                    MessageLengthPicker.SelectedIndex = i;
                    break;
                }
            }

            _doChangeVolume = true;
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseGrid.IsEnabled = false;
            App.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
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

        private void VolumeSlider_ValueChanged(object sender, ValueChangedEventArgs e)
        {
            if(_doChangeVolume)
                App.FmodService.AdjustVolumes((float)GeneralVolumeSlider.Value, (float)MusicVolumeSlider.Value, (float)AmbientVolumeSlider.Value, (float)DialogueVolumeSlider.Value, (float)EffectsVolumeSlider.Value, (float)UIVolumeSlider.Value);
        }
    }
}