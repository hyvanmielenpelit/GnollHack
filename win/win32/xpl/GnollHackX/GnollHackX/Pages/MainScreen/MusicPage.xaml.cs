using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;
using GnollHackX.Controls;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class MusicPage : ContentPage
    {
        public MusicPage()
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            GHApp.SetPageThemeOnHandler(this, GHApp.DarkMode);
            GHApp.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            if (GHApp.DarkMode)
            {
                lblHeader.TextColor = GHColors.White;
                EmptyLabel.TextColor = GHColors.White;
            }
            AddDiscoveredSoundTracks();
            GHApp.FmodService.StopAllSounds((uint)StopSoundFlags.All, 0);
            GHApp.FmodService.LoadBanks(sound_bank_loading_type.Music);
            GHApp.FmodService.PlayMusic(GHConstants.MusicGHSound, GHConstants.MusicEventPath, 0, 0.3f, 1.0f);
        }

        private void AddDiscoveredSoundTracks()
        {
            long discoBits = GHApp.DiscoveredMusicBits;
            int discoCount = 0;
            for (int i = 0; i < GHSoundTrack.GnollHackSoundTracks.Count && i < 64; i++)
            {
                long bit = 1L << i;
                if((discoBits & bit) != 0)
                {
                    GHSoundTrack track = GHSoundTrack.GnollHackSoundTracks[i];
                    int length = track.Length;
                    int min = length / 60;
                    int sec = length % 60;
                    bool hasSongwriter = !string.IsNullOrWhiteSpace(track.Songwriter);
                    bool hasDuration = length > 0;
                    string durationString = hasDuration ? min + ":" + sec : "";
                    RowImageButton rib = new RowImageButton();
                    rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.soundtrack.png";
                    rib.LblText = track.DisplayName;
                    rib.LblFontSize = 16;
                    rib.LblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.SubLblText = (hasSongwriter ? track.Songwriter : "")
                        + (hasDuration && hasSongwriter ? " (" : "")
                        + (hasDuration ? durationString : "")
                        + (hasDuration && hasSongwriter ? ")" : "");
                    rib.SubLblFontSize = 13;
                    rib.SubLblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.IsSubLblVisible = hasDuration || hasSongwriter;
                    rib.ImgWidth = 80;
                    rib.ImgHeight = 80;
                    rib.GridWidth = 480;
                    rib.GridHeight = 80;
                    rib.WidthRequest = 480;
                    rib.HeightRequest = 80;
                    rib.GridMargin = new Thickness(rib.ImgWidth / 15, 0);
                    rib.BtnCommand = i;
                    rib.BtnClicked += SoundTrackButton_Clicked;
                    SoundTrackLayout.Children.Add(rib); 
                    
                    discoCount++;
                }
            }

            lblSubtitle.Text = "Found " + discoCount + " of " + GHSoundTrack.GnollHackSoundTracks.Count + " sound tracks";
            if (discoCount == 0)
                EmptyLabel.IsVisible = true;
        }

        private void SoundTrackButton_Clicked(object sender, EventArgs e)
        {
            SoundTrackLayout.IsEnabled = false;
            RowImageButton ghbutton = sender as RowImageButton;
            if (ghbutton != null)
            {
                int ghSoundTrackId = ghbutton.BtnCommand;
                GHSoundTrack ghst = GHSoundTrack.GnollHackSoundTracks[ghSoundTrackId];
                int ghsound = -1;
                if (ghst.GHSoundForPlayback >= 0)
                    ghsound = ghst.GHSoundForPlayback;
                else if (ghst.GHSoundList.Count > 0)
                    ghsound = ghst.GHSoundList[0];

                GHApp.FmodService.StopAllSounds((uint)StopSoundFlags.All, 0);
                if (ghsound >= 0)
                {
                    string eventPath = GHApp.GnollHackService.GetEventPathForGHSound(ghsound);
                    float volume = GHApp.GnollHackService.GetVolumeForGHSound(ghsound);
                    if (!string.IsNullOrWhiteSpace(eventPath) && volume > 0)
                    {
                        GHApp.FmodService.PlayMusic(ghsound, eventPath, 0, Math.Min(1.0f, volume * GHConstants.IntroMusicVolume / GHConstants.BackgroundMusicVolume), 1.0f);
                    }
                }
            }
            SoundTrackLayout.IsEnabled = true;
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.FmodService.StopAllSounds((uint)StopSoundFlags.All, 0);
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
            GHApp.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, GHConstants.IntroMusicVolume, 1.0f);
            GHApp.FmodService.UnloadBanks(sound_bank_loading_type.Music);
        }


        private double _currentPageWidth = 0;
        private double _currentPageHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;

                lblHeader.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                SoundTrackLayout.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                GHApp.FmodService.StopAllSounds((uint)StopSoundFlags.All, 0);
                await App.Current.MainPage.Navigation.PopModalAsync();
                GHApp.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, GHConstants.IntroMusicVolume, 1.0f);
                GHApp.FmodService.UnloadBanks(sound_bank_loading_type.Music);
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }

        private void TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            lblHeader.IsEnabled = false;
            GHApp.FmodService.StopAllSounds((uint)StopSoundFlags.All, 0);
            lblHeader.IsEnabled = true;
        }
    }
}