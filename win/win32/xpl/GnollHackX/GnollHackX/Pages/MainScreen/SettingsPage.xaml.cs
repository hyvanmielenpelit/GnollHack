using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Collections.Concurrent;
#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using Xamarin.Forms;
using Xamarin.Forms.Xaml;
using Xamarin.Essentials;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using GnollHackX.Pages.Game;

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
        public SettingsPage(GameMenuPage gameMenuPage, MainPage mainPage)
        {
            InitializeComponent();
#if GNH_MAUI
        On<iOS>().SetUseSafeArea(true);
#else
            On<Xamarin.Forms.PlatformConfiguration.iOS>().SetUseSafeArea(true);
#endif
            _gameMenuPage = gameMenuPage;
            if (_gameMenuPage != null)
                _gamePage = _gameMenuPage._gamePage;
            else
                _gamePage = null;
            _mainPage = mainPage;
            List<string> list = new List<string>();
            list.Add("20 fps");
            list.Add("30 fps");
            list.Add("40 fps");
            if (GHApp.DisplayRefreshRate >= 60.0f)
                list.Add("60 fps");
            if (GHApp.DisplayRefreshRate >= 80.0f)
                list.Add("80 fps");
            if (GHApp.DisplayRefreshRate >= 90.0f)
                list.Add("90 fps");
            if (GHApp.DisplayRefreshRate >= 120.0f)
                list.Add("120 fps");
            RefreshRatePicker.ItemsSource = list;

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

            SetInitialValues();

            ClassicStatusBarSwitch_Toggled(null, new ToggledEventArgs(ClassicStatusBarSwitch.IsToggled));
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

            if (_gamePage != null)
                _gamePage.ShowMemoryUsage = MemorySwitch.IsToggled;
            Preferences.Set("ShowMemoryUsage", MemorySwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowFPS = FPSSwitch.IsToggled;
            Preferences.Set("ShowFPS", FPSSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.UseMainGLCanvas = GPUSwitch.IsToggled;
            Preferences.Set("UseMainGLCanvas", GPUSwitch.IsToggled);

            Preferences.Set("AllowBones", BonesSwitch.IsToggled);

            GHApp.PostingGameStatus = PostGameStatusSwitch.IsToggled;
            Preferences.Set("PostingGameStatus", PostGameStatusSwitch.IsToggled);
            GHApp.PostingDiagnosticData = PostDiagnosticDataSwitch.IsToggled;
            Preferences.Set("PostingDiagnosticData", PostDiagnosticDataSwitch.IsToggled);

            GHApp.CustomGameStatusLink = _customGameStatusLink;
            Preferences.Set("CustomGameStatusLink", _customGameStatusLink);

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

            if (MainSection.Contains(AlternativeDrawingViewCell))
            {
                if (_gamePage != null)
                    _gamePage.AlternativeLayerDrawing = AlternativeLayerDrawingSwitch.IsToggled;
                Preferences.Set("AlternativeLayerDrawing", AlternativeLayerDrawingSwitch.IsToggled);
            }

            if (_mainPage != null)
                _mainPage.UpdateLayout();

            if (_gameMenuPage != null)
                _gameMenuPage.UpdateLayout();
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
        }

        private void SetInitialValues()
        {
            int cursor = 0, graphics = 0, maprefresh = (int)UIUtils.GetDefaultMapFPS(), msgnum = 0, petrows = 0;
            bool mem = false, fps = false, gpu = GHApp.IsGPUDefault, simplecmdlayout = true, bank = true, navbar = GHConstants.DefaultHideNavigation, statusbar = GHConstants.DefaultHideStatusBar;
            bool allowbones = true, lighterdarkening = false, accuratedrawing = GHConstants.DefaultAlternativeLayerDrawing, html = GHConstants.DefaultHTMLDumpLogs, singledumplog = GHConstants.DefaultUseSingleDumpLog, streamingbanktomemory = false, streamingbanktodisk = false, wallends = GHConstants.DefaultDrawWallEnds, breatheanimations = GHConstants.DefaultBreatheAnimations;
            bool devmode = GHConstants.DefaultDeveloperMode, logmessages = GHConstants.DefaultLogMessages, hpbars = false, nhstatusbarclassic = GHConstants.IsDefaultStatusBarClassic, pets = true, orbs = true, orbmaxhp = false, orbmaxmana = false, mapgrid = false, playermark = false, monstertargeting = false, walkarrows = true;
            bool forcemaxmsg = false, showexstatus = false, noclipmode = GHConstants.DefaultMapNoClipMode, silentmode = false;
            bool postgamestatus = GHConstants.DefaultPosting, postdiagnostics = GHConstants.DefaultPosting;
            float generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume;
            string customlink = "";
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
            streamingbanktomemory = Preferences.Get("ReadStreamingBankToMemory", GHConstants.DefaultReadStreamingBankToMemory);
            streamingbanktodisk = Preferences.Get("CopyStreamingBankToDisk", GHConstants.DefaultCopyStreamingBankToDisk);
            postgamestatus = Preferences.Get("PostingGameStatus", GHConstants.DefaultPosting);
            postdiagnostics = Preferences.Get("PostingDiagnosticData", GHConstants.DefaultPosting);
            customlink = Preferences.Get("CustomGameStatusLink", "");
            allowbones = Preferences.Get("AllowBones", true);
            noclipmode = Preferences.Get("DefaultMapNoClipMode", GHConstants.DefaultMapNoClipMode);
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
                gpu = Preferences.Get("UseMainGLCanvas", GHApp.IsGPUDefault);
                simplecmdlayout = Preferences.Get("UseSimpleCmdLayout", true);
                msgnum = Preferences.Get("NumDisplayedMessages", GHConstants.DefaultMessageRows);
                petrows = Preferences.Get("NumDisplayedPetRows", GHConstants.DefaultPetRows);
                lighterdarkening = Preferences.Get("LighterDarkening", GHConstants.DefaultLighterDarkening);
                accuratedrawing = Preferences.Get("AlternativeLayerDrawing", GHConstants.DefaultAlternativeLayerDrawing);
                wallends = Preferences.Get("DrawWallEnds", GHConstants.DefaultDrawWallEnds);
                breatheanimations = Preferences.Get("BreatheAnimations", GHConstants.DefaultBreatheAnimations);
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
                gpu = _gamePage.UseMainGLCanvas;
                simplecmdlayout = _gamePage.UseSimpleCmdLayout;
                msgnum = _gamePage.NumDisplayedMessages;
                petrows = _gamePage.NumDisplayedPetRows;
                lighterdarkening = _gamePage.LighterDarkening;
                accuratedrawing = _gamePage.AlternativeLayerDrawing;
                wallends = _gamePage.DrawWallEnds;
                breatheanimations = _gamePage.BreatheAnimations;
            }
            CursorPicker.SelectedIndex = cursor;
            GraphicsPicker.SelectedIndex = graphics;
            RefreshRatePicker.SelectedIndex = Math.Min(RefreshRatePicker.Items.Count - 1, maprefresh);
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
            GPUSwitch.IsToggled = gpu;
            SimpleCmdLayoutSwitch.IsToggled = simplecmdlayout;
            NavBarSwitch.IsToggled = navbar;
            if (!GHApp.IsAndroid)
            {
                NavBarSwitch.IsEnabled = false;
                NavBarLabel.IsEnabled = false;
                NavBarLabel.TextColor = GHColors.Gray;
                if (MainSection.Contains(NavBarViewCell))
                    MainSection.Remove(NavBarViewCell);
            }
            StatusBarSwitch.IsToggled = statusbar;
            if (!GHApp.IsiOS)
            {
                StatusBarSwitch.IsEnabled = false;
                StatusBarLabel.IsEnabled = false;
                StatusBarLabel.TextColor = GHColors.Gray;
                if (MainSection.Contains(StatusBarViewCell))
                    MainSection.Remove(StatusBarViewCell);
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
            PostGameStatusSwitch.IsToggled = postgamestatus;
            PostDiagnosticDataSwitch.IsToggled = postdiagnostics;
            _customGameStatusLink = customlink;
            CustomLinkLabel.Text = customlink == "" ? "Default" : "Custom";
            CustomLinkButton.Text = customlink == "" ? "Add" : "Edit";

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
            AlternativeDrawingLabel.TextColor = Color.Gray;
            if (MainSection.Contains(AlternativeDrawingViewCell))
                MainSection.Remove(AlternativeDrawingViewCell);    
        }
#endif
            WallEndSwitch.IsToggled = wallends;
            BreatheAnimationSwitch.IsToggled = breatheanimations;

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

            bool readmem_oldvalue = Preferences.Get("ReadStreamingBankToMemory", false);
            bool copydisk_oldvalue = Preferences.Get("CopyStreamingBankToDisk", false);

            if (StreamingBankToMemorySwitch.IsToggled != readmem_oldvalue || StreamingBankToDiskSwitch.IsToggled != copydisk_oldvalue)
            {
                unloadBanks = true;
                setupFiles = true;
                loadBanks = true;
            }
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await MaybeShowPleaseWait();
            await App.Current.MainPage.Navigation.PopModalAsync();
        }



        private string _customGameStatusLink = "";
        private void CustomLinkButton_Clicked(object sender, EventArgs e)
        {
            CustomLinkButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            TextCaption.Text = "Enter Custom Webhook Link:";
            TextEntry.Text = _customGameStatusLink;
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

            if (res != "" && !GHApp.IsValidHttpsURL(res))
            {
                TextFrame.BorderColor = GHColors.Red;
                TextEntry.Focus();
                TextOkButton.IsEnabled = true;
                TextCancelButton.IsEnabled = true;
                return;
            }

            _customGameStatusLink = res;
            if (res == "")
                CustomLinkLabel.Text = "Default";
            else
                CustomLinkLabel.Text = "Custom";

            CustomLinkButton.Text = _customGameStatusLink == "" ? "Add" : "Edit";

            TextGrid.IsVisible = false;
            TextEntry.Text = "";
            TextCaption.Text = "";
            TextFrame.BorderColor = GHColors.Black;
            CustomLinkButton.IsEnabled = true;
        }


        private void TextCancelButton_Clicked(object sender, EventArgs e)
        {
            TextOkButton.IsEnabled = false;
            TextCancelButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            TextGrid.IsVisible = false;
            TextEntry.Text = "";
            TextCaption.Text = "";
            TextFrame.BorderColor = GHColors.Black;
            CustomLinkButton.IsEnabled = true;
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
    }
}