using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.IO;
using System.Net.Http.Headers;
using System.Net.Http;

using System.Threading;
using System.Text.RegularExpressions;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using Xamarin.Essentials;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using GnollHackX.Pages.Game;
using GnollHackX.Controls;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SettingsPage : ContentPage
    {
        private GamePage _gamePage;
        private GameMenuPage _gameMenuPage;
        private MainPage _mainPage;
        private bool _doChangeVolume = false;
        public Regex XlogUserNameValidationExpression { get; set; }
        public Regex BonesAllowedUsersValidationExpression { get; set; }

        public SettingsPage(GameMenuPage gameMenuPage, MainPage mainPage)
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);

            _gameMenuPage = gameMenuPage;
            if (_gameMenuPage != null)
                _gamePage = _gameMenuPage._gamePage;
            else
                _gamePage = null;
            _mainPage = mainPage;
            List<string> list = new List<string>
            {
                "20 fps",
                "30 fps",
                "40 fps",
                "60 fps"
            };
            if (GHApp.DisplayRefreshRate >= 80.0f)
                list.Add("80 fps");
            if (GHApp.DisplayRefreshRate >= 90.0f)
                list.Add("90 fps");
            if (GHApp.DisplayRefreshRate >= 120.0f)
                list.Add("120 fps");
            RefreshRatePicker.ItemsSource = list;


            PrimaryGPUCachePicker.ItemsSource = GHApp.GetGPUCacheSizeList(false);
            SecondaryGPUCachePicker.ItemsSource = GHApp.GetGPUCacheSizeList(true);


            SimpleCommandBarButton1Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            SimpleCommandBarButton2Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            SimpleCommandBarButton3Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            SimpleCommandBarButton4Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            SimpleCommandBarButton5Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            SimpleCommandBarButton6Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            SimpleCommandBarButton1Picker.ItemDisplayBinding = new Binding("Name");
            SimpleCommandBarButton2Picker.ItemDisplayBinding = new Binding("Name");
            SimpleCommandBarButton3Picker.ItemDisplayBinding = new Binding("Name");
            SimpleCommandBarButton4Picker.ItemDisplayBinding = new Binding("Name");
            SimpleCommandBarButton5Picker.ItemDisplayBinding = new Binding("Name");
            SimpleCommandBarButton6Picker.ItemDisplayBinding = new Binding("Name");

            XlogUserNameValidationExpression = new Regex(@"^[A-Za-z0-9_]{1,31}$");
            BonesAllowedUsersValidationExpression = new Regex(@"^([A-Za-z0-9_]*[ \,]+)*([A-Za-z0-9_]*)?$");

            SetInitialValues();

            ClassicStatusBarSwitch_Toggled(null, new ToggledEventArgs(ClassicStatusBarSwitch.IsToggled));
            BonesSwitch_Toggled(null, new ToggledEventArgs(BonesSwitch.IsToggled));
            BonesListSwitch_Toggled(null, new ToggledEventArgs(BonesListSwitch.IsToggled));

            if(!GHApp.RecommendedSettingsChecked)
            {
                GHApp.RecommendedSettingsChecked = true;
                Preferences.Set("RecommendedSettingsChecked", true);
                if (!GHApp.PostingXlogEntries || string.IsNullOrWhiteSpace(GHApp.XlogUserName))
                {
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Recommended Settings";
                    PopupLabel.Text = "It is recommended that you set up a GnollHack Account and turn on Post Top Scores and Share Bones Files to enjoy the full GnollHack experience. These settings are found under the Server Posting section.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
            }

            _isManualTogglingEnabled = true;
        }

        private async void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            _doChangeVolume = false;

            if (CursorPicker.SelectedIndex > -1)
            {
                if (_gamePage != null)
                    _gamePage.CursorStyle = (TTYCursorStyle)CursorPicker.SelectedIndex;
                Preferences.Set("CursorStyle", CursorPicker.SelectedIndex);
            }

            if (GraphicsPicker.SelectedIndex > -1)
            {
                if (_gamePage != null)
                    _gamePage.GraphicsStyle = (GHGraphicsStyle)GraphicsPicker.SelectedIndex;
                Preferences.Set("GraphicsStyle", GraphicsPicker.SelectedIndex);
            }

            if (RefreshRatePicker.SelectedIndex > -1)
            {
                if (_gamePage != null)
                    _gamePage.MapRefreshRate = (MapRefreshRateStyle)RefreshRatePicker.SelectedIndex;
                Preferences.Set("MapRefreshRate", RefreshRatePicker.SelectedIndex);
            }

            if (PrimaryGPUCachePicker.SelectedIndex > -1 && PrimaryGPUCachePicker.SelectedItem != null && PrimaryGPUCachePicker.SelectedItem is CacheSizeItem)
            {
                long size = ((CacheSizeItem)PrimaryGPUCachePicker.SelectedItem).Size;
                if (_gamePage != null)
                    _gamePage.SetPrimaryCanvasResourceCacheLimit(size);
                Preferences.Set("PrimaryGPUCacheLimit", size);
                GHApp.PrimaryGPUCacheLimit = size;
            }

            if (SecondaryGPUCachePicker.SelectedIndex > -1 && SecondaryGPUCachePicker.SelectedItem != null && SecondaryGPUCachePicker.SelectedItem is CacheSizeItem)
            {
                long size = ((CacheSizeItem)SecondaryGPUCachePicker.SelectedItem).Size;
                if (_gamePage != null)
                    _gamePage.SetSecondaryCanvasResourceCacheLimit(size);
                Preferences.Set("SecondaryGPUCacheLimit", size);
                GHApp.SecondaryGPUCacheLimit = size;
            }

            if (_gamePage != null)
                _gamePage.ShowMemoryUsage = MemorySwitch.IsToggled;
            Preferences.Set("ShowMemoryUsage", MemorySwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowFPS = FPSSwitch.IsToggled;
            Preferences.Set("ShowFPS", FPSSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowBattery = BatterySwitch.IsToggled;
            Preferences.Set("ShowBattery", BatterySwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowRecording = ShowRecordingSwitch.IsToggled;
            Preferences.Set("ShowRecording", ShowRecordingSwitch.IsToggled);


            if (_gamePage != null)
                _gamePage.UseMainGLCanvas = GPUSwitch.IsToggled;
            Preferences.Set("UseMainGLCanvas", GPUSwitch.IsToggled);

            GHApp.AllowBones = BonesSwitch.IsToggled;
            Preferences.Set("AllowBones", BonesSwitch.IsToggled);

            if (RecordSwitch.IsEnabled)
            {
                GHApp.RecordGame = RecordSwitch.IsToggled;
                Preferences.Set("RecordGame", RecordSwitch.IsToggled);
            }

            GHApp.AutoUploadReplays = AutoUploadReplaysSwitch.IsToggled;
            Preferences.Set("AutoUploadReplays", AutoUploadReplaysSwitch.IsToggled);

            if (GZipSwitch.IsEnabled)
            {
                GHApp.UseGZipForReplays = GZipSwitch.IsToggled;
                Preferences.Set("UseGZipForReplays", GZipSwitch.IsToggled);
            }

            GHApp.EmptyWishIsNothing = EmptyWishIsNothingSwitch.IsToggled;
            Preferences.Set("EmptyWishIsNothing", EmptyWishIsNothingSwitch.IsToggled);

            GHApp.PostingGameStatus = PostGameStatusSwitch.IsToggled;
            Preferences.Set("PostingGameStatus", PostGameStatusSwitch.IsToggled);
            GHApp.PostingDiagnosticData = PostDiagnosticDataSwitch.IsToggled;
            Preferences.Set("PostingDiagnosticData", PostDiagnosticDataSwitch.IsToggled);
            GHApp.PostingXlogEntries = PostXlogSwitch.IsToggled;
            Preferences.Set("PostingXlogEntries", PostXlogSwitch.IsToggled);
            GHApp.PostingReplays = PostReplaysSwitch.IsToggled;
            Preferences.Set("PostingReplays", PostReplaysSwitch.IsToggled);
            GHApp.PostingBonesFiles = PostBonesSwitch.IsToggled;
            Preferences.Set("PostingBonesFiles", PostBonesSwitch.IsToggled);
            GHApp.BonesUserListIsBlack = BonesListSwitch.IsToggled;
            Preferences.Set("BonesUserListIsBlack", BonesListSwitch.IsToggled);
            
            GHApp.CustomGameStatusLink = _customGameStatusLink;
            Preferences.Set("CustomGameStatusLink", _customGameStatusLink);

            GHApp.CustomXlogAccountLink = _customXlogAccountLink;
            Preferences.Set("CustomXlogAccountLink", _customXlogAccountLink);
            GHApp.CustomXlogPostLink = _customXlogPostLink;
            Preferences.Set("CustomXlogPostLink", _customXlogPostLink);
            GHApp.XlogUserName = PostXlogUserNameEntry.Text;
            Preferences.Set("XlogUserName", PostXlogUserNameEntry.Text);
            GHApp.XlogPassword = PostXlogPasswordEntry.Text;
            Preferences.Set("XlogPassword", PostXlogPasswordEntry.Text);
            GHApp.BonesAllowedUsers = BonesAllowedUsersEntry.Text;
            Preferences.Set("BonesAllowedUsers", BonesAllowedUsersEntry.Text);
            GHApp.XlogReleaseAccount = XlogReleaseAccountSwitch.IsToggled;
            Preferences.Set("XlogReleaseAccount", XlogReleaseAccountSwitch.IsToggled);
            if (GHApp.IsDebug)
            {
                GHApp.ForcePostBones = ForcePostBonesSwitch.IsToggled;
                Preferences.Set("ForcePostBones", ForcePostBonesSwitch.IsToggled);
            }

            GHApp.CustomCloudStorageConnectionString = _customCloudStorageConnectionString;
            Preferences.Set("CustomCloudStorageConnectionString", _customCloudStorageConnectionString);

            GHApp.XlogCredentialsIncorrect = false;
            if (!GHApp.AreCredentialsVerified(PostXlogUserNameEntry.Text, PostXlogPasswordEntry.Text))
                GHApp.SetXlogUserNameVerified(false, null, null);
            GHApp.TryVerifyXlogUserName();

            if (_gamePage != null)
                _gamePage.MapGrid = GridSwitch.IsToggled;
            Preferences.Set("MapGrid", GridSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.HitPointBars = HitPointBarSwitch.IsToggled;
            Preferences.Set("HitPointBars", HitPointBarSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ClassicStatusBar = ClassicStatusBarSwitch.IsToggled;
            Preferences.Set("ClassicStatusBar", ClassicStatusBarSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowPets = PetSwitch.IsToggled;
            Preferences.Set("ShowPets", PetSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowOrbs = OrbSwitch.IsToggled;
            Preferences.Set("ShowOrbs", OrbSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowMaxHealthInOrb = MaxHealthInOrbSwitch.IsToggled;
            Preferences.Set("ShowMaxHealthInOrb", MaxHealthInOrbSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowMaxManaInOrb = MaxManaInOrbSwitch.IsToggled;
            Preferences.Set("ShowMaxManaInOrb", MaxManaInOrbSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.PlayerMark = PlayerMarkSwitch.IsToggled;
            Preferences.Set("PlayerMark", PlayerMarkSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.MonsterTargeting = MonsterTargetingSwitch.IsToggled;
            Preferences.Set("MonsterTargeting", MonsterTargetingSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.WalkArrows = WalkArrowSwitch.IsToggled;
            Preferences.Set("WalkArrows", WalkArrowSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.DrawWallEnds = WallEndSwitch.IsToggled;
            Preferences.Set("DrawWallEnds", WallEndSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.BreatheAnimations = BreatheAnimationSwitch.IsToggled;
            Preferences.Set("BreatheAnimations", BreatheAnimationSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.LongerMessageHistory = LongerMessageHistorySwitch.IsToggled;
            //Preferences.Set("LongerMessageHistory", LongerMessageHistorySwitch.IsToggled);
            GHApp.SavedLongerMessageHistory = LongerMessageHistorySwitch.IsToggled;

            //if (_gamePage != null)
            //    _gamePage.ShowPut2BagContextCommand = Put2BagSwitch.IsToggled;
            //Preferences.Set("ShowPut2BagContextCommand", Put2BagSwitch.IsToggled);

            //if (_gamePage != null)
            //    _gamePage.ShowPrevWepContextCommand = PrevWepSwitch.IsToggled;
            //Preferences.Set("ShowPrevWepContextCommand", PrevWepSwitch.IsToggled);

            GHApp.HideAndroidNavigationBar = NavBarSwitch.IsToggled;
            Preferences.Set("HideAndroidNavigationBar", GHApp.HideAndroidNavigationBar);

            GHApp.HideiOSStatusBar = StatusBarSwitch.IsToggled;
            Preferences.Set("HideiOSStatusBar", GHApp.HideiOSStatusBar);

            GHApp.DeveloperMode = DeveloperSwitch.IsToggled;
            Preferences.Set("DeveloperMode", GHApp.DeveloperMode);
            GHApp.DebugLogMessages = LogMessageSwitch.IsToggled;
            Preferences.Set("DebugLogMessages", GHApp.DebugLogMessages);

            Preferences.Set("DefaultMapNoClipMode", !YesClipNormalSwitch.IsToggled);

            for(int i = 0; i < 6; i++)
            {
#if GNH_MAUI
                Microsoft.Maui.Controls.Picker[] pickers = new Microsoft.Maui.Controls.Picker[6] 
#else
                Xamarin.Forms.Picker[] pickers = new Xamarin.Forms.Picker[6] 
#endif
                {
                    SimpleCommandBarButton1Picker,
                    SimpleCommandBarButton2Picker,
                    SimpleCommandBarButton3Picker,
                    SimpleCommandBarButton4Picker,
                    SimpleCommandBarButton5Picker,
                    SimpleCommandBarButton6Picker,
                };
#if GNH_MAUI
                Microsoft.Maui.Controls.Picker targetPicker = pickers[i]; 
#else
                Xamarin.Forms.Picker targetPicker = pickers[i];
#endif
                string keystr = "SimpleUILayoutCommandButton" + (i + 1);
                if (targetPicker.SelectedIndex < 0 || targetPicker.SelectedIndex >= GHApp.SelectableShortcutButtons.Count)
                {
                    if (Preferences.ContainsKey(keystr))
                        Preferences.Remove(keystr);
                }
                else
                {
                    Preferences.Set(keystr, GHApp.SelectableShortcutButtons[targetPicker.SelectedIndex].GetCommand());
                    if (_gamePage != null)
                        _gamePage.SetSimpleLayoutCommandButton(i, targetPicker.SelectedIndex);
                }
            }

            Assembly assembly = GetType().GetTypeInfo().Assembly;
            GHApp.InitializeMoreCommandButtons(assembly, SimpleCmdLayoutSwitch.IsToggled);
            if (_gamePage != null)
            {
                _gamePage.UseSimpleCmdLayout = SimpleCmdLayoutSwitch.IsToggled;
                _gamePage.MoreCmdPage = 1;
            }
            Preferences.Set("UseSimpleCmdLayout", SimpleCmdLayoutSwitch.IsToggled);

            GHApp.SilentMode = SilentModeSwitch.IsToggled;
            Preferences.Set("SilentMode", GHApp.SilentMode);

            Preferences.Set("GeneralVolume", (float)GeneralVolumeSlider.Value);
            Preferences.Set("MusicVolume", (float)MusicVolumeSlider.Value);
            Preferences.Set("AmbientVolume", (float)AmbientVolumeSlider.Value);
            Preferences.Set("DialogueVolume", (float)DialogueVolumeSlider.Value);
            Preferences.Set("EffectsVolume", (float)EffectsVolumeSlider.Value);
            Preferences.Set("UIVolume", (float)UIVolumeSlider.Value);

            if (!GHApp.IsMuted)
                GHApp.FmodService.AdjustVolumes((float)GeneralVolumeSlider.Value, (float)MusicVolumeSlider.Value, (float)AmbientVolumeSlider.Value, (float)DialogueVolumeSlider.Value, (float)EffectsVolumeSlider.Value, (float)UIVolumeSlider.Value);

            int res = GHConstants.DefaultMessageRows, tryres = 0;
            string str = MessageLengthPicker.SelectedItem == null ? res.ToString() : MessageLengthPicker.SelectedItem.ToString();
            if (int.TryParse(str, out tryres))
                res = tryres;

            if (res > 0)
            {
                if (_gamePage != null)
                    _gamePage.NumDisplayedMessages = res;
                Preferences.Set("NumDisplayedMessages", res);
            }

            if (_gamePage != null)
                _gamePage.ForceAllMessages = ForceMaxMessageSwitch.IsToggled;

            res = GHConstants.DefaultPetRows;
            tryres = 0;
            str = PetRowPicker.SelectedItem == null ? res.ToString() : PetRowPicker.SelectedItem.ToString();
            if (str.Length >= 3 && str.Substring(0, 3) == "Max")
                res = 99;
            else if (int.TryParse(str, out tryres))
                res = tryres;

            if (res > 0)
            {
                if (_gamePage != null)
                    _gamePage.NumDisplayedPetRows = res;
                Preferences.Set("NumDisplayedPetRows", res);
            }

            bool unloadbanks = false;
            bool loadbanks = false;
            bool setupfiles = false;

            GetBankLoadingVariables(out unloadbanks, out setupfiles, out loadbanks);

            GHApp.LoadBanks = SoundBankSwitch.IsToggled;
            Preferences.Set("LoadSoundBanks", SoundBankSwitch.IsToggled);

            GHApp.UseHTMLDumpLogs = HTMLDumpLogSwitch.IsToggled;
            Preferences.Set("UseHTMLDumpLogs", HTMLDumpLogSwitch.IsToggled);

            GHApp.UseSingleDumpLog = SingleDumpLogSwitch.IsToggled;
            Preferences.Set("UseSingleDumpLog", SingleDumpLogSwitch.IsToggled);

            GHApp.ReadStreamingBankToMemory = StreamingBankToMemorySwitch.IsToggled;
            Preferences.Set("ReadStreamingBankToMemory", StreamingBankToMemorySwitch.IsToggled);

            GHApp.CopyStreamingBankToDisk = StreamingBankToDiskSwitch.IsToggled;
            Preferences.Set("CopyStreamingBankToDisk", StreamingBankToDiskSwitch.IsToggled);

            if (_mainPage != null)
                _mainPage.PlayCarouselView();

            if (unloadbanks)
            {
                try
                {
                    GHApp.FmodService.ReleaseAllSoundInstances();
                    GHApp.FmodService.UnloadBanks(0);
                    GHApp.FmodService.UnloadBanks(2);
                }
                catch
                {

                }
            }

            if (setupfiles)
            {
                try
                {
                    GHApp.GnollHackService.InitializeSecrets(GHApp.CurrentSecrets);
                }
                catch (Exception ex)
                {
                    await DisplayAlert("Secrets Initialization Failed", "Initializing secrets failed: " + ex.Message, "OK");
                }
                GHApp.SetSoundBanksUpForLoading();
            }

            if (loadbanks)
            {
                try
                {
                    GHApp.FmodService.LoadBanks(0);
                    GHApp.FmodService.LoadBanks(2);
                    if (_gamePage == null)
                    {
                        GHApp.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, 0.5f, 1.0f);
                    }
                }
                catch
                {

                }
            }

            if (_gamePage != null)
                _gamePage.ShowExtendedStatusBar = ShowExtendedStatusBarSwitch.IsToggled;

            if (_gamePage != null)
                _gamePage.LighterDarkening = LighterDarkeningSwitch.IsToggled;
            Preferences.Set("LighterDarkening", LighterDarkeningSwitch.IsToggled);

            if (AlternativeDrawingStackLayout.IsVisible)
            {
                if (_gamePage != null)
                    _gamePage.AlternativeLayerDrawing = AlternativeLayerDrawingSwitch.IsToggled;
                Preferences.Set("AlternativeLayerDrawing", AlternativeLayerDrawingSwitch.IsToggled);
            }

            if (SaveStylePicker.SelectedIndex > -1)
            {
                GHApp.AppSwitchSaveStyle = SaveStylePicker.SelectedIndex;
                Preferences.Set("AppSwitchSaveStyle", SaveStylePicker.SelectedIndex);
            }

            if (_mainPage != null)
            {
                _mainPage.UpdateLayout();
                _mainPage.StartGeneralTimer(); /* Just in case something's changed */
            }

            if (_gameMenuPage != null)
                _gameMenuPage.UpdateLayout();
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
        }

        private void SetInitialValues()
        {
            int cursor = 0, graphics = 0, savestyle = 0, maprefresh = (int)UIUtils.GetDefaultMapFPS(), msgnum = 0, petrows = 0;
            bool mem = false, fps = false, battery = false, showrecording = true, autoupload = false, gpu = GHApp.IsGPUDefault, simplecmdlayout = true, bank = true, navbar = GHConstants.DefaultHideNavigation, statusbar = GHConstants.DefaultHideStatusBar;
            bool allowbones = true, emptywishisnothing = true, recordgame = false, gzip = GHConstants.GZipIsDefaultReplayCompression, lighterdarkening = false, accuratedrawing = GHConstants.DefaultAlternativeLayerDrawing, html = GHConstants.DefaultHTMLDumpLogs, singledumplog = GHConstants.DefaultUseSingleDumpLog, streamingbanktomemory = false, streamingbanktodisk = false, wallends = GHConstants.DefaultDrawWallEnds;
            bool breatheanimations = GHConstants.DefaultBreatheAnimations; //, put2bag = GHConstants.DefaultShowPickNStashContextCommand, prevwep = GHConstants.DefaultShowPrevWepContextCommand;
            bool devmode = GHConstants.DefaultDeveloperMode, logmessages = GHConstants.DefaultLogMessages, hpbars = false, nhstatusbarclassic = GHConstants.IsDefaultStatusBarClassic, pets = true, orbs = true, orbmaxhp = false, orbmaxmana = false, mapgrid = false, playermark = false, monstertargeting = false, walkarrows = true;
            bool forcemaxmsg = false, showexstatus = false, noclipmode = GHConstants.DefaultMapNoClipMode, silentmode = false;
            bool postgamestatus = GHConstants.DefaultPosting, postdiagnostics = GHConstants.DefaultPosting, postxlog = GHConstants.DefaultPosting, postreplays = GHConstants.DefaultPosting, postbones = GHConstants.DefaultPosting, boneslistisblack = false;
            bool longermsghistory = false, xlog_release_account = false, forcepostbones = false;
            long primarygpucache = -2, secondarygpucache = -2;
            float generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume;
            string customlink = "";
            string customxlogaccountlink = "";
            string customxlogpostlink = "";
            string customcloudstorage = "";
            string xlog_username = "";
            string xlog_password = "";
            string bones_allowed_users = "";

            int[] cmdidxs = new int[6];
            for (int i = 0; i < 6; i++)
            {
                string keystr = "SimpleUILayoutCommandButton" + (i + 1);
                int defCmd = GHApp.DefaultShortcutButton(0, i, true).GetCommand();
                int savedCmd = Preferences.Get(keystr, defCmd);
                int listselidx = GHApp.SelectableShortcutButtonIndexInList(savedCmd, defCmd);
                cmdidxs[i] = listselidx;
            }

            silentmode = Preferences.Get("SilentMode", false);
            generalVolume = Preferences.Get("GeneralVolume", GHConstants.DefaultGeneralVolume);
            musicVolume = Preferences.Get("MusicVolume", GHConstants.DefaultMusicVolume);
            ambientVolume = Preferences.Get("AmbientVolume", GHConstants.DefaultAmbientVolume);
            dialogueVolume = Preferences.Get("DialogueVolume", GHConstants.DefaultDialogueVolume);
            effectsVolume = Preferences.Get("EffectsVolume", GHConstants.DefaultEffectsVolume);
            UIVolume = Preferences.Get("UIVolume", GHConstants.DefaultUIVolume);
            navbar = GHApp.HideAndroidNavigationBar;
            statusbar = GHApp.HideiOSStatusBar;
            devmode = GHApp.DeveloperMode;
            logmessages = GHApp.DebugLogMessages;
            bank = Preferences.Get("LoadSoundBanks", true);
            html = Preferences.Get("UseHTMLDumpLogs", GHConstants.DefaultHTMLDumpLogs);
            singledumplog = Preferences.Get("UseSingleDumpLog", GHConstants.DefaultUseSingleDumpLog);
            streamingbanktomemory = Preferences.Get("ReadStreamingBankToMemory", GHApp.DefaultStreamingBankToMemory);
            streamingbanktodisk = Preferences.Get("CopyStreamingBankToDisk", GHConstants.DefaultCopyStreamingBankToDisk);
            postgamestatus = Preferences.Get("PostingGameStatus", GHConstants.DefaultPosting);
            postdiagnostics = Preferences.Get("PostingDiagnosticData", GHConstants.DefaultPosting);
            postxlog = Preferences.Get("PostingXlogEntries", GHConstants.DefaultPosting);
            postreplays = Preferences.Get("PostingReplays", GHConstants.DefaultPosting);
            postbones = Preferences.Get("PostingBonesFiles", GHConstants.DefaultPosting);
            boneslistisblack = Preferences.Get("BonesUserListIsBlack", false);            
            customlink = Preferences.Get("CustomGameStatusLink", "");
            customcloudstorage = Preferences.Get("CustomCloudStorageConnectionString", "");
            customxlogaccountlink = Preferences.Get("CustomXlogAccountLink", "");
            customxlogpostlink = Preferences.Get("CustomXlogPostLink", "");
            xlog_username = Preferences.Get("XlogUserName", "");
            xlog_password = Preferences.Get("XlogPassword", "");
            xlog_release_account = Preferences.Get("XlogReleaseAccount", false);
            bones_allowed_users = Preferences.Get("BonesAllowedUsers", "");
            forcepostbones = Preferences.Get("ForcePostBones", false);
            allowbones = Preferences.Get("AllowBones", true);
            emptywishisnothing = Preferences.Get("EmptyWishIsNothing", true);
            recordgame = Preferences.Get("RecordGame", false);
            gzip = Preferences.Get("UseGZipForReplays", GHConstants.GZipIsDefaultReplayCompression);
            noclipmode = Preferences.Get("DefaultMapNoClipMode", GHConstants.DefaultMapNoClipMode);
            savestyle = Preferences.Get("AppSwitchSaveStyle", 0);
            primarygpucache = Preferences.Get("PrimaryGPUCacheLimit", -2L);
            secondarygpucache = Preferences.Get("SecondaryGPUCacheLimit", -2L);
            if (_gamePage == null)
            {
                cursor = Preferences.Get("CursorStyle", 1);
                graphics = Preferences.Get("GraphicsStyle", 1);
                maprefresh = Preferences.Get("MapRefreshRate", (int)UIUtils.GetDefaultMapFPS());
                mapgrid = Preferences.Get("MapGrid", false);
                forcemaxmsg = false; /* Always starts as false */
                ForceMaxMessageSwitch.IsEnabled = false;
                ForceMaxMessageLabel.TextColor = GHColors.Gray;
                showexstatus = false; /* Always starts as false */
                ShowExtendedStatusBarSwitch.IsEnabled = false;
                ShowExtendedStatusBarLabel.TextColor = GHColors.Gray;
                hpbars = Preferences.Get("HitPointBars", false);
                nhstatusbarclassic = Preferences.Get("ClassicStatusBar", GHConstants.IsDefaultStatusBarClassic);
                pets = Preferences.Get("ShowPets", true);
                orbs = Preferences.Get("ShowOrbs", true);
                orbmaxhp = Preferences.Get("ShowMaxHealthInOrb", false);
                orbmaxmana = Preferences.Get("ShowMaxManaInOrb", false);
                playermark = Preferences.Get("PlayerMark", false);
                monstertargeting = Preferences.Get("MonsterTargeting", false);
                walkarrows = Preferences.Get("WalkArrows", true);
                mem = Preferences.Get("ShowMemoryUsage", false);
                fps = Preferences.Get("ShowFPS", false);
                battery = Preferences.Get("ShowBattery", false);
                showrecording = Preferences.Get("ShowRecording", true);
                autoupload = Preferences.Get("AutoUploadReplays", false);
                gpu = Preferences.Get("UseMainGLCanvas", GHApp.IsGPUDefault);
                simplecmdlayout = Preferences.Get("UseSimpleCmdLayout", true);
                msgnum = Preferences.Get("NumDisplayedMessages", GHConstants.DefaultMessageRows);
                petrows = Preferences.Get("NumDisplayedPetRows", GHConstants.DefaultPetRows);
                lighterdarkening = Preferences.Get("LighterDarkening", GHConstants.DefaultLighterDarkening);
                accuratedrawing = Preferences.Get("AlternativeLayerDrawing", GHConstants.DefaultAlternativeLayerDrawing);
                wallends = Preferences.Get("DrawWallEnds", GHConstants.DefaultDrawWallEnds);
                breatheanimations = Preferences.Get("BreatheAnimations", GHConstants.DefaultBreatheAnimations);
                //put2bag = Preferences.Get("ShowPut2BagContextCommand", GHConstants.DefaultShowPickNStashContextCommand);
                //prevwep = Preferences.Get("ShowPrevWepContextCommand", GHConstants.DefaultShowPrevWepContextCommand);
                longermsghistory = GHApp.SavedLongerMessageHistory; // Preferences.Get("LongerMessageHistory", false);
            }
            else
            {
                cursor = (int)_gamePage.CursorStyle;
                graphics = (int)_gamePage.GraphicsStyle;
                maprefresh = (int)_gamePage.MapRefreshRate;
                mapgrid = _gamePage.MapGrid;
                forcemaxmsg = _gamePage.ForceAllMessages;
                ForceMaxMessageSwitch.IsEnabled = true;
                ForceMaxMessageLabel.TextColor = GHColors.Black;
                showexstatus = _gamePage.ShowExtendedStatusBar;
                ShowExtendedStatusBarSwitch.IsEnabled = true;
                ShowExtendedStatusBarLabel.TextColor = GHColors.Black;
                nhstatusbarclassic = _gamePage.ClassicStatusBar;
                hpbars = _gamePage.HitPointBars;
                pets = _gamePage.ShowPets;
                orbs = _gamePage.ShowOrbs;
                orbmaxhp = _gamePage.ShowMaxHealthInOrb;
                orbmaxmana = _gamePage.ShowMaxManaInOrb;
                playermark = _gamePage.PlayerMark;
                monstertargeting = _gamePage.MonsterTargeting;
                walkarrows = _gamePage.WalkArrows;
                mem = _gamePage.ShowMemoryUsage;
                fps = _gamePage.ShowFPS;
                battery = _gamePage.ShowBattery;
                showrecording = _gamePage.ShowRecording;
                gpu = _gamePage.UseMainGLCanvas;
                simplecmdlayout = _gamePage.UseSimpleCmdLayout;
                msgnum = _gamePage.NumDisplayedMessages;
                petrows = _gamePage.NumDisplayedPetRows;
                lighterdarkening = _gamePage.LighterDarkening;
                accuratedrawing = _gamePage.AlternativeLayerDrawing;
                wallends = _gamePage.DrawWallEnds;
                breatheanimations = _gamePage.BreatheAnimations;
                //put2bag = _gamePage.ShowPut2BagContextCommand;
                //prevwep = _gamePage.ShowPrevWepContextCommand;
                longermsghistory = _gamePage.LongerMessageHistory;
            }
            CursorPicker.SelectedIndex = cursor;
            GraphicsPicker.SelectedIndex = graphics;
            RefreshRatePicker.SelectedIndex = Math.Min(RefreshRatePicker.Items.Count - 1, maprefresh);
            SaveStylePicker.SelectedIndex = savestyle;
            GridSwitch.IsToggled = mapgrid;
            HitPointBarSwitch.IsToggled = hpbars;
            ClassicStatusBarSwitch.IsToggled = nhstatusbarclassic;
            PetSwitch.IsToggled = pets;
            OrbSwitch.IsToggled = orbs;
            MaxHealthInOrbSwitch.IsToggled = orbmaxhp;
            MaxManaInOrbSwitch.IsToggled = orbmaxmana;
            PlayerMarkSwitch.IsToggled = playermark;
            MonsterTargetingSwitch.IsToggled = monstertargeting;
            WalkArrowSwitch.IsToggled = walkarrows;
            YesClipNormalSwitch.IsToggled = !noclipmode;
            SilentModeSwitch.IsToggled = silentmode;
            MemorySwitch.IsToggled = mem;
            FPSSwitch.IsToggled = fps;
            BatterySwitch.IsToggled = battery;
            ShowRecordingSwitch.IsToggled = showrecording;
            AutoUploadReplaysSwitch.IsToggled = autoupload;
            GPUSwitch.IsToggled = gpu;
            SimpleCmdLayoutSwitch.IsToggled = simplecmdlayout;
            NavBarSwitch.IsToggled = navbar;
            if (!GHApp.IsAndroid)
            {
                NavBarSwitch.IsEnabled = false;
                NavBarLabel.IsEnabled = false;
                NavBarLabel.TextColor = GHColors.Gray;
                NavBarStackLayout.IsVisible = false;
            }
            StatusBarSwitch.IsToggled = statusbar;
            if (!GHApp.IsiOS)
            {
                StatusBarSwitch.IsEnabled = false;
                StatusBarLabel.IsEnabled = false;
                StatusBarLabel.TextColor = GHColors.Gray;
                StatusBarStackLayout.IsVisible = false;
            }
            DeveloperSwitch.IsToggled = devmode;
            LogMessageSwitch.IsToggled = logmessages;
            if (!devmode)
            {
                LogMessageSwitch.IsEnabled = false;
                LogMessageLabel.IsEnabled = false;
                LogMessageLabel.TextColor = GHColors.Gray;
            }
            SoundBankSwitch.IsToggled = bank;
            SingleDumpLogSwitch.IsToggled = singledumplog;
            HTMLDumpLogSwitch.IsToggled = html;
            if (!singledumplog)
            {
                HTMLDumpLogSwitch.IsEnabled = false;
                HTMLDumpLogLabel.IsEnabled = false;
                HTMLDumpLogLabel.TextColor = GHColors.Gray;
            }
            StreamingBankToMemorySwitch.IsToggled = streamingbanktomemory;
            StreamingBankToDiskSwitch.IsToggled = streamingbanktodisk;
            BonesSwitch.IsToggled = allowbones;
            RecordSwitch.IsToggled = recordgame;
            if (_gamePage != null || (!RecordSwitch.IsToggled && GHApp.PlatformService.GetDeviceFreeDiskSpaceInBytes() < GHConstants.LowFreeDiskSpaceThresholdInBytes)) /* Cannot turn on or off in the middle of the game; need to save and restart; otherwise either relevant commands are not recorded or things may get prone to bugs */
            {
                RecordSwitch.IsEnabled = false;
                RecordLabel.TextColor = GHColors.Gray;
            }
            GZipSwitch.IsToggled = gzip;
            if (_gamePage != null) /* Cannot turn on or off in the middle of the game; need to save and restart */
            {
                GZipSwitch.IsEnabled = false;
                GZipLabel.TextColor = GHColors.Gray;
            }
            EmptyWishIsNothingSwitch.IsToggled = emptywishisnothing;

            PostGameStatusSwitch.IsToggled = postgamestatus;
            PostDiagnosticDataSwitch.IsToggled = postdiagnostics;
            PostXlogSwitch.IsToggled = postxlog;
            PostReplaysSwitch.IsToggled = postreplays;
            PostBonesSwitch.IsToggled = postbones;
            BonesListSwitch.IsToggled = boneslistisblack;
            _customGameStatusLink = customlink;
            CustomLinkLabel.Text = customlink == "" ? "Default" : "Custom";
            CustomLinkButton.Text = customlink == "" ? "Add" : "Edit";

            _customXlogAccountLink = customxlogaccountlink;
            CustomXlogAccountLinkLabel.Text = customxlogaccountlink == "" ? "Default" : "Custom";
            CustomXlogAccountButton.Text = customxlogaccountlink == "" ? "Link" : "Link";

            _customXlogPostLink = customxlogpostlink;
            CustomXlogPostLinkLabel.Text = customxlogpostlink == "" ? "Default" : "Custom";
            CustomXlogPostButton.Text = customxlogpostlink == "" ? "Link" : "Link";

            PostXlogUserNameEntry.Text = xlog_username;
            PostXlogPasswordEntry.Text = xlog_password;
            BonesAllowedUsersEntry.Text = bones_allowed_users;
            XlogReleaseAccountSwitch.IsToggled = xlog_release_account;
            XlogReleaseAccountStackLayout.IsVisible = GHApp.IsDebug;
            ForcePostBonesSwitch.IsToggled = forcepostbones;
            ForcePostBonesStackLayout.IsVisible = GHApp.IsDebug;

            _customCloudStorageConnectionString = customcloudstorage;
            CustomCloudStorageLabel.Text = customcloudstorage == "" ? "Default" : "Custom";
            CustomCloudStorageButton.Text = customcloudstorage == "" ? "Add" : "Edit";

            SimpleCommandBarButton1Picker.SelectedIndex = cmdidxs[0];
            SimpleCommandBarButton2Picker.SelectedIndex = cmdidxs[1];
            SimpleCommandBarButton3Picker.SelectedIndex = cmdidxs[2];
            SimpleCommandBarButton4Picker.SelectedIndex = cmdidxs[3];
            SimpleCommandBarButton5Picker.SelectedIndex = cmdidxs[4];
            SimpleCommandBarButton6Picker.SelectedIndex = cmdidxs[5];
            SimpleCommandBarButton1Picker.IsEnabled = simplecmdlayout;
            SimpleCommandBarButton2Picker.IsEnabled = simplecmdlayout;
            SimpleCommandBarButton3Picker.IsEnabled = simplecmdlayout;
            SimpleCommandBarButton4Picker.IsEnabled = simplecmdlayout;
            SimpleCommandBarButton5Picker.IsEnabled = simplecmdlayout;
            SimpleCommandBarButton6Picker.IsEnabled = simplecmdlayout;
            SimpleCommandBarButton1Label.TextColor = simplecmdlayout ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton2Label.TextColor = simplecmdlayout ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton3Label.TextColor = simplecmdlayout ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton4Label.TextColor = simplecmdlayout ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton5Label.TextColor = simplecmdlayout ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton6Label.TextColor = simplecmdlayout ? GHColors.Black : GHColors.Gray;

            if (PrimaryGPUCachePicker.ItemsSource != null)
            {
                foreach (object item in PrimaryGPUCachePicker.ItemsSource)
                {
                    if (item is CacheSizeItem)
                    {
                        CacheSizeItem c = (CacheSizeItem)item;
                        if (c.Size == primarygpucache)
                        {
                            PrimaryGPUCachePicker.SelectedItem = c;
                            break;
                        }
                    }
                }
            }
            if (SecondaryGPUCachePicker.ItemsSource != null)
            {
                foreach (object item in SecondaryGPUCachePicker.ItemsSource)
                {
                    if (item is CacheSizeItem)
                    {
                        CacheSizeItem c = (CacheSizeItem)item;
                        if (c.Size == secondarygpucache)
                        {
                            SecondaryGPUCachePicker.SelectedItem = c;
                            break;
                        }
                    }
                }
            }
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
            for (int i = 0; i < PetRowPicker.Items.Count; i++)
            {
                int tryint = 0;
                if (petrows == 99 && PetRowPicker.Items[i].ToString().Length >= 3 && PetRowPicker.Items[i].ToString().Substring(0, 3) == "Max")
                {
                    PetRowPicker.SelectedIndex = i;
                    break;
                }
                else if (int.TryParse(PetRowPicker.Items[i].ToString(), out tryint) && tryint > 0 && tryint == petrows)
                {
                    PetRowPicker.SelectedIndex = i;
                    break;
                }
            }
            ForceMaxMessageSwitch.IsToggled = forcemaxmsg;
            ShowExtendedStatusBarSwitch.IsToggled = showexstatus;
            LighterDarkeningSwitch.IsToggled = lighterdarkening;
            AlternativeLayerDrawingSwitch.IsToggled = accuratedrawing;
#if !DEBUG
            if(!AlternativeLayerDrawingSwitch.IsToggled)
            {
                AlternativeLayerDrawingSwitch.IsEnabled = false;
                AlternativeDrawingLabel.TextColor = GHColors.Gray;
                AlternativeDrawingStackLayout.IsVisible = false;
            }
#endif
            WallEndSwitch.IsToggled = wallends;
            BreatheAnimationSwitch.IsToggled = breatheanimations;
            //Put2BagSwitch.IsToggled = put2bag;
            //PrevWepSwitch.IsToggled = prevwep;
            LongerMessageHistorySwitch.IsToggled = longermsghistory;

            _doChangeVolume = !GHApp.IsMuted;
        }

        private void ClassicStatusBarSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (ClassicStatusBarSwitch.IsToggled)
            {
                OrbSwitch.IsEnabled = true;
                OrbsLabel.TextColor = GHColors.Black;
                PetSwitch.IsEnabled = false;
                PetsLabel.TextColor = GHColors.Gray;
            }
            else
            {
                OrbSwitch.IsEnabled = false;
                OrbsLabel.TextColor = GHColors.Gray;
                PetSwitch.IsEnabled = true;
                PetsLabel.TextColor = GHColors.Black;
            }
            OrbSwitch_Toggled(sender, new ToggledEventArgs(OrbSwitch.IsToggled));
            PetSwitch_Toggled(sender, new ToggledEventArgs(PetSwitch.IsToggled));
        }

        private void OrbSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (!ClassicStatusBarSwitch.IsToggled || OrbSwitch.IsToggled)
            {
                MaxHealthInOrbSwitch.IsEnabled = true;
                MaxHealthInOrbLabel.TextColor = GHColors.Black;
                MaxManaInOrbSwitch.IsEnabled = true;
                MaxManaInOrbLabel.TextColor = GHColors.Black;
            }
            else
            {
                MaxHealthInOrbSwitch.IsEnabled = false;
                MaxHealthInOrbLabel.TextColor = GHColors.Gray;
                MaxManaInOrbSwitch.IsEnabled = false;
                MaxManaInOrbLabel.TextColor = GHColors.Gray;
            }
        }

        private void PetSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (!ClassicStatusBarSwitch.IsToggled && PetSwitch.IsToggled)
            {
                PetRowPicker.IsEnabled = true;
                PetRowsLabel.TextColor = GHColors.Black;
            }
            else
            {
                PetRowPicker.IsEnabled = false;
                PetRowsLabel.TextColor = GHColors.Gray;
            }
        }

        private void SilentModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            GHApp.SilentMode = SilentModeSwitch.IsToggled;
            _doChangeVolume = !GHApp.IsMuted;
        }

        private void StreamingBankToMemorySwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (e.Value && StreamingBankToDiskSwitch.IsToggled)
                StreamingBankToDiskSwitch.IsToggled = false;
        }

        private void StreamingBankToDiskSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (e.Value && StreamingBankToMemorySwitch.IsToggled)
                StreamingBankToMemorySwitch.IsToggled = false;
        }

        private void DeveloperSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (e.Value)
            {
                LogMessageSwitch.IsEnabled = true;
                LogMessageLabel.IsEnabled = true;
                LogMessageLabel.TextColor = GHColors.Black;
            }
            else
            {
                LogMessageSwitch.IsEnabled = false;
                LogMessageSwitch.IsToggled = false;
                LogMessageLabel.IsEnabled = false;
                LogMessageLabel.TextColor = GHColors.Gray;
            }
        }

        private void SingleDumpLogSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (e.Value)
            {
                HTMLDumpLogSwitch.IsEnabled = true;
                HTMLDumpLogLabel.IsEnabled = true;
                HTMLDumpLogLabel.TextColor = GHColors.Black;
            }
            else
            {
                HTMLDumpLogSwitch.IsEnabled = false;
                HTMLDumpLogLabel.IsEnabled = false;
                HTMLDumpLogLabel.TextColor = GHColors.Gray;
            }
        }

        private async Task MaybeShowPleaseWait()
        {
            bool _unloadBanks;
            bool _loadBanks;
            bool _setupFiles;

            GetBankLoadingVariables(out _unloadBanks, out _setupFiles, out _loadBanks);

            if (_unloadBanks || _setupFiles || _loadBanks)
            {
                WaitLayout.IsVisible = true;
                ForceLayout();
                await Task.Delay(50);
            }
        }

        private void GetBankLoadingVariables(out bool unloadBanks, out bool setupFiles, out bool loadBanks)
        {
            unloadBanks = false;
            setupFiles = false;
            loadBanks = false;

            if (SoundBankSwitch.IsToggled != GHApp.LoadBanks)
            {
                if (GHApp.LoadBanks)
                    loadBanks = true;
                else
                    loadBanks = false;
            }

            bool readmem_oldvalue = Preferences.Get("ReadStreamingBankToMemory", GHApp.DefaultStreamingBankToMemory);
            bool copydisk_oldvalue = Preferences.Get("CopyStreamingBankToDisk", GHConstants.DefaultCopyStreamingBankToDisk);

            if (StreamingBankToMemorySwitch.IsToggled != readmem_oldvalue || StreamingBankToDiskSwitch.IsToggled != copydisk_oldvalue)
            {
                unloadBanks = true;
                setupFiles = true;
                loadBanks = true;
            }
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            PostXlogUserNameLabel.TextColor = GHColors.Black;
            BonesAllowedUsersLabel.TextColor = GHColors.Black;
            if (PostXlogUserNameEntry.Text != null && PostXlogUserNameEntry.Text != "")
            {
                if (!XlogUserNameValidationExpression.IsMatch(PostXlogUserNameEntry.Text))
                {
                    PostXlogUserNameLabel.TextColor = GHColors.Red;
                    await MainScrollView.ScrollToAsync(PostXlogUserNameStackLayout.X, PostXlogUserNameStackLayout.Y, true);
                    PostXlogUserNameEntry.Focus();
                    CloseButton.IsEnabled = true;
                    return;
                }
            }
            if (BonesAllowedUsersEntry.Text != null && BonesAllowedUsersEntry.Text != "")
            {
                if (!BonesAllowedUsersValidationExpression.IsMatch(BonesAllowedUsersEntry.Text))
                {
                    BonesAllowedUsersLabel.TextColor = GHColors.Red;
                    await MainScrollView.ScrollToAsync(BonesAllowedUsersStackLayout.X, BonesAllowedUsersStackLayout.Y, true);
                    BonesAllowedUsersEntry.Focus();
                    CloseButton.IsEnabled = true;
                    return;
                }
            }
            await MaybeShowPleaseWait();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }


        private CustomImageButton _linkButtonClicked = null;
        private Label _linkLabel = null;
        private int _linkIndex = 0;

        private string _customGameStatusLink = "";
        private void CustomLinkButton_Clicked(object sender, EventArgs e)
        {
            CustomLinkButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            _linkButtonClicked = CustomLinkButton;
            _linkLabel = CustomLinkLabel;
            _linkIndex = 0;
            TextCaption.Text = "Enter Custom Webhook Link:";
            TextEntry.Placeholder = "Enter the link here";
            TextEntry.Text = _customGameStatusLink;
            TextEntry.IsEnabled = true;
            TextOkButton.IsEnabled = true;
            TextCancelButton.IsEnabled = true;
            TextGrid.IsVisible = true;
        }

        private void TextOkButton_Clicked(object sender, EventArgs e)
        {
            TextOkButton.IsEnabled = false;
            TextCancelButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            string res = TextEntry.Text;
            if (string.IsNullOrWhiteSpace(TextEntry.Text))
            {
                res = "";
            }
            else
            {
                res.Trim();
            }

            bool isValid;
            switch (_linkIndex)
            {
                default:
                case 0:
                case 1:
                case 2:
                    isValid = GHApp.IsValidHttpsURL(res);
                    break;
                case 3:
                    isValid = true; /* any custom connection string does currently */
                    break;
            }

            if (res != "" && !isValid)
            {
                TextFrame.BorderColor = GHColors.Red;
                TextEntry.Focus();
                TextOkButton.IsEnabled = true;
                TextCancelButton.IsEnabled = true;
                return;
            }

            bool isLinkEmpty;
            switch(_linkIndex)
            {
                default:
                case 0:
                    _customGameStatusLink = res;
                    isLinkEmpty = _customGameStatusLink == "";
                    break;
                case 1:
                    _customXlogAccountLink = res;
                    isLinkEmpty = _customXlogAccountLink == "";
                    break;
                case 2:
                    _customXlogPostLink = res;
                    isLinkEmpty = _customXlogPostLink == "";
                    break;
                case 3:
                    _customCloudStorageConnectionString = res;
                    isLinkEmpty = _customCloudStorageConnectionString == "";
                    break;
            }

            if (_linkButtonClicked == null) _linkButtonClicked = CustomLinkButton;
            if (_linkLabel == null) _linkLabel = CustomLinkLabel;

            if (res == "")
                _linkLabel.Text = "Default";
            else
                _linkLabel.Text = "Custom";

            if(_linkIndex != 1)
                _linkButtonClicked.Text = isLinkEmpty ? "Add" : "Edit";

            TextGrid.IsVisible = false;
            TextEntry.Text = "";
            TextEntry.Unfocus();
            TextEntry.IsEnabled = false;
            TextCaption.Text = "";
            TextFrame.BorderColor = GHColors.Black;

            _linkButtonClicked.IsEnabled = true;
        }


        private void TextCancelButton_Clicked(object sender, EventArgs e)
        {
            TextOkButton.IsEnabled = false;
            TextCancelButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            TextGrid.IsVisible = false;
            TextEntry.Text = "";
            TextEntry.Unfocus();
            TextEntry.IsEnabled = false;
            TextCaption.Text = "";
            TextFrame.BorderColor = GHColors.Black;

            if (_linkButtonClicked == null) _linkButtonClicked = CustomLinkButton;
            _linkButtonClicked.IsEnabled = true;
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                await MaybeShowPleaseWait();
                await App.Current.MainPage.Navigation.PopModalAsync();
            }
            return false;
        }

        private void VolumeSlider_ValueChanged(object sender, ValueChangedEventArgs e)
        {
            if (_doChangeVolume)
                GHApp.FmodService.AdjustVolumes((float)GeneralVolumeSlider.Value, (float)MusicVolumeSlider.Value, (float)AmbientVolumeSlider.Value, (float)DialogueVolumeSlider.Value, (float)EffectsVolumeSlider.Value, (float)UIVolumeSlider.Value);
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
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
        }

        private void SimpleCmdLayoutSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            SimpleCommandBarButton1Picker.IsEnabled = e.Value;
            SimpleCommandBarButton2Picker.IsEnabled = e.Value;
            SimpleCommandBarButton3Picker.IsEnabled = e.Value;
            SimpleCommandBarButton4Picker.IsEnabled = e.Value;
            SimpleCommandBarButton5Picker.IsEnabled = e.Value;
            SimpleCommandBarButton6Picker.IsEnabled = e.Value;
            SimpleCommandBarButton1Label.TextColor = e.Value ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton2Label.TextColor = e.Value ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton3Label.TextColor = e.Value ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton4Label.TextColor = e.Value ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton5Label.TextColor = e.Value ? GHColors.Black : GHColors.Gray;
            SimpleCommandBarButton6Label.TextColor = e.Value ? GHColors.Black : GHColors.Gray;
        }

        private async void XlogAccountButton_Clicked(object sender, EventArgs e)
        {
            XlogAccountButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.XlogReleaseAccount = XlogReleaseAccountSwitch.IsToggled;
            await OpenBrowser(new Uri(GHApp.XlogAccountAddress));
            XlogAccountButton.IsEnabled = true;
        }

        private string _customXlogAccountLink = "";
        private void CustomXlogAccountButton_Clicked(object sender, EventArgs e)
        {
            CustomXlogAccountButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            _linkButtonClicked = CustomXlogAccountButton;
            _linkLabel = CustomXlogAccountLinkLabel;
            _linkIndex = 1;
            TextCaption.Text = "Enter Custom Account Link:";
            TextEntry.Placeholder = "Enter the link here";
            TextEntry.Text = _customXlogAccountLink;
            TextEntry.IsEnabled = true;
            TextOkButton.IsEnabled = true;
            TextCancelButton.IsEnabled = true;
            TextGrid.IsVisible = true;
        }

        private string _customXlogPostLink = "";
        private void CustomXlogPostButton_Clicked(object sender, EventArgs e)
        {
            CustomXlogPostButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            _linkButtonClicked = CustomXlogPostButton;
            _linkLabel = CustomXlogPostLinkLabel;
            _linkIndex = 2;
            TextCaption.Text = "Enter Custom Post Link:";
            TextEntry.Placeholder = "Enter the link here";
            TextEntry.Text = _customXlogPostLink;
            TextEntry.IsEnabled = true;
            TextOkButton.IsEnabled = true;
            TextCancelButton.IsEnabled = true;
            TextGrid.IsVisible = true;
        }

        public async Task OpenBrowser(Uri uri)
        {
            try
            {
                await Browser.OpenAsync(uri, BrowserLaunchMode.SystemPreferred);
            }
            catch (Exception ex)
            {
                await DisplayAlert("Cannot Open Web Page", "GnollHack cannot open the webpage at " + uri.OriginalString + ". Error: " + ex.Message, "OK");
            }
        }

        private async void XlogTestButton_Clicked(object sender, EventArgs e)
        {
            XlogTestButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            XlogTestButton.TextColor = GHColors.Yellow;
            XlogTestButton.Text = "Wait";
            GHApp.XlogUserName = PostXlogUserNameEntry.Text;
            GHApp.XlogPassword = PostXlogPasswordEntry.Text;
            GHApp.XlogReleaseAccount = XlogReleaseAccountSwitch.IsToggled;
            GHApp.SetXlogUserNameVerified(false, null, null);
            GHApp.XlogCredentialsIncorrect = false;
            SendResult res = await GHApp.SendXLogEntry("", 1, 0, new List<GHPostAttachment>(), true);
            XlogTestButton.Text = "Test";
            if (res.IsSuccess)
            {
                XlogTestButton.TextColor = GHColors.BrighterGreen;
                PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                PopupTitleLabel.Text = "Connection Success";
                PopupLabel.Text = "Connection to " + (GHApp.IsDebug && !GHApp.XlogReleaseAccount ? "Test " : "") + "GnollHack Server was successful." +
                    (res.HasHttpStatusCode && res.StatusCode != System.Net.HttpStatusCode.OK? " Status Code: " + (int)res.StatusCode + " (" + res.StatusCode.ToString() + ")" : "");
                PopupOkButton.IsEnabled = true;
                PopupGrid.IsVisible = true;
            }
            else
            {
                XlogTestButton.TextColor = GHColors.Red;
                PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                PopupTitleLabel.Text = "Connection Failed";
                PopupLabel.Text = "Connection to " + (GHApp.IsDebug && !GHApp.XlogReleaseAccount ? "Test " : "") + "GnollHack Server failed." +
                    (res.HasHttpStatusCode ? " Status Code: " + (int)res.StatusCode + " (" + res.StatusCode.ToString() + ")" : "") +
                    (res.Message != null ? " Message: " + res.Message : "");
                PopupOkButton.IsEnabled = true;
                PopupGrid.IsVisible = true;
            }
            XlogTestButton.IsEnabled = true;
        }

        private void BonesSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            PostBonesSwitch.IsEnabled = e.Value;
            if (e.Value)
            {
                PostBonesLabel.TextColor = GHColors.Black;
            }
            else
            {
                PostBonesLabel.TextColor = GHColors.Gray;
            }
            PostBonesSwitch_Toggled(sender, new ToggledEventArgs(PostBonesSwitch.IsToggled));
        }

        private void BonesAllowedUsersLabel_TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
            PopupTitleLabel.Text = BonesListSwitch.IsToggled ? "Disallowed Users for Received Bones" : "Allowed Users for Received Bones";
            PopupLabel.Text = (BonesListSwitch.IsToggled ? "You can define users whose bones files you do not want to receive. " : "You can define the group of users from whom you want to receive bones files. ") + "User names are separated by a space or comma. User names are not case-sensitive.";
            PopupOkButton.IsEnabled = true;
            PopupGrid.IsVisible = true;
        }

        private void PopupOkButton_Clicked(object sender, EventArgs e)
        {
            PopupOkButton.IsEnabled = false;
            PopupGrid.IsVisible = false;
            GHApp.PlayButtonClickedSound();
            PopupOkButton.IsEnabled = true;
        }

        private void BonesListSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if(e.Value)
            {
                BonesAllowedUsersLabel.Text = "  Blacklist";
                BonesAllowedUsersEntry.Placeholder = "No users";
            }
            else
            {
                BonesAllowedUsersLabel.Text = "  Whitelist";
                BonesAllowedUsersEntry.Placeholder = "All users";
            }
        }

        private void PostBonesSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            bool val = e.Value && PostBonesSwitch.IsEnabled;
            BonesListSwitch.IsEnabled = val;
            BonesAllowedUsersEntry.IsEnabled = val;
            if (val)
            {
                BonesListLabel.TextColor = GHColors.Black;
                BonesAllowedUsersLabel.TextColor = GHColors.Black;
            }
            else
            {
                BonesListLabel.TextColor = GHColors.Gray;
                BonesAllowedUsersLabel.TextColor = GHColors.Gray;
            }
        }

        private void RecordSwitch_Toggled(object sender, ToggledEventArgs e)
        {

        }

        private void RecordLabel_TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            if(RecordSwitch.IsEnabled)
            {
                //Nothing currently
            }
            else
            {
                if(_gamePage != null)
                {
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Toggling Recording Disallowed";
                    PopupLabel.Text = "Toggling recording on and off is disallowed during the game. Save the game first and then change the setting from the main screen.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
                else
                {
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Insufficient Disk Space";
                    PopupLabel.Text = string.Format("There is too little free disk space for switching on game recording ({0:0.00} GB). Please consider freeing disk space on your device.", (double)GHApp.PlatformService.GetDeviceFreeDiskSpaceInBytes() / (1024 * 1024 * 1024));
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
            }
        }

        private void GZipLabel_TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            if (GZipSwitch.IsEnabled)
            {
                PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                PopupTitleLabel.Text = "Replay Compression Format";
                PopupLabel.Text = "Replay compression format can be either zip (off) or gzip (on).";
                PopupOkButton.IsEnabled = true;
                PopupGrid.IsVisible = true;
            }
            else
            {
                if (_gamePage != null)
                {
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Changing Replay Compression Disallowed";
                    PopupLabel.Text = "Changing the replay compression format is disallowed during the game. Save the game first and then change the setting from the main screen.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
            }
        }
        private void GZipSwitch_Toggled(object sender, ToggledEventArgs e)
        {

        }

        private string _customCloudStorageConnectionString = "";
        private void CustomCloudStorageButton_Clicked(object sender, EventArgs e)
        {
            CustomCloudStorageButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            _linkButtonClicked = CustomCloudStorageButton;
            _linkLabel = CustomCloudStorageLabel;
            _linkIndex = 3;
            TextCaption.Text = "Enter Custom Connection String:";
            TextEntry.Placeholder = "Enter the string here";
            TextEntry.Text = _customCloudStorageConnectionString;
            TextEntry.IsEnabled = true;
            TextOkButton.IsEnabled = true;
            TextCancelButton.IsEnabled = true;
            TextGrid.IsVisible = true;
        }

        private bool _isManualTogglingEnabled = false;

        private void GPUSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if(_isManualTogglingEnabled && e.Value && !GHApp.IsGPUDefault)
            {
                PopupTitleLabel.TextColor = GHColors.Orange;
                PopupTitleLabel.Text = "Unstable GPU Acceleration";
                PopupLabel.Text = "Your device has been detected as potentially causing crashes when GPU acceleration is switched on. Proceed with care.";
                PopupOkButton.IsEnabled = true;
                PopupGrid.IsVisible = true;
            }
        }

        private void PrimaryGPUCachePicker_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_isManualTogglingEnabled && PrimaryGPUCachePicker.SelectedIndex > 1 && PrimaryGPUCachePicker.ItemsSource != null && PrimaryGPUCachePicker.SelectedIndex < PrimaryGPUCachePicker.ItemsSource.Count)
            {
                object item = PrimaryGPUCachePicker.ItemsSource[PrimaryGPUCachePicker.SelectedIndex];
                if (item != null && item is CacheSizeItem)
                {
                    CacheSizeItem c = (CacheSizeItem)item;
                    if (c.Size >= (long)(GHApp.TotalMemory / 2))
                    {
                        PopupTitleLabel.TextColor = GHColors.Orange;
                        PopupTitleLabel.Text = "High GPU Cache";
                        PopupLabel.Text = "Setting primary GPU cache to a high level may potentially crash the game.";
                        PopupOkButton.IsEnabled = true;
                        PopupGrid.IsVisible = true;
                    }
                }
            }
        }

        private void SecondaryGPUCachePicker_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_isManualTogglingEnabled && SecondaryGPUCachePicker.SelectedIndex > 1 && SecondaryGPUCachePicker.ItemsSource != null && SecondaryGPUCachePicker.SelectedIndex < SecondaryGPUCachePicker.ItemsSource.Count)
            {
                object item = SecondaryGPUCachePicker.ItemsSource[SecondaryGPUCachePicker.SelectedIndex];
                if (item != null && item is CacheSizeItem)
                {
                    CacheSizeItem c = (CacheSizeItem)item;
                    if (c.Size >= (long)(GHApp.TotalMemory / 2))
                    {
                        PopupTitleLabel.TextColor = GHColors.Orange;
                        PopupTitleLabel.Text = "High GPU Cache";
                        PopupLabel.Text = "Setting secondary GPU cache to a high level may potentially crash the game.";
                        PopupOkButton.IsEnabled = true;
                        PopupGrid.IsVisible = true;
                    }
                }
            }
        }
    }

}