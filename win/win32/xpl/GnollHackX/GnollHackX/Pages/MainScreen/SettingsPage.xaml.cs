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
            UIUtils.AdjustRootLayout(RootGrid);
            GHApp.SetPageThemeOnHandler(this, GHApp.DarkMode);
            GHApp.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            GHApp.SetViewCursorOnHandler(TournamentLabel, GameCursorType.Info);
            GHApp.SetViewCursorOnHandler(BonesAllowedUsersLabel, GameCursorType.Info);
            GHApp.SetViewCursorOnHandler(RecordLabel, GameCursorType.Info);
            GHApp.SetViewCursorOnHandler(GZipLabel, GameCursorType.Info);

            _gameMenuPage = gameMenuPage;
            if (_gameMenuPage != null)
                _gamePage = _gameMenuPage._gamePage;
            else
                _gamePage = null;
            _mainPage = mainPage;

            lblHeader.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            SetTournamentModeLabelColors(GHApp.TournamentMode);
            SetChildrenDarkModeTextColor(RootLayout, GHApp.DarkMode);

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

            List<MouseCommandItem> mouseCommandItems = new List<MouseCommandItem>();
            mouseCommandItems.AddRange(GHApp.MouseCommandItems);

            if(_gamePage == null)
            {
                mouseCommandItems.Add(new MouseCommandItem("By Role", (int)NhGetPosMods.DefClickRole));
            }

            RightMousePicker.ItemsSource = mouseCommandItems;
            MiddleMousePicker.ItemsSource = mouseCommandItems;

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

            FullCommandBarButton1Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton2Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton3Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton4Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton5Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton6Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton7Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton8Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton9Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton10Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton11Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton12Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton13Picker.ItemsSource = GHApp.SelectableShortcutButtons;
            FullCommandBarButton1Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton2Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton3Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton4Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton5Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton6Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton7Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton8Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton9Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton10Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton11Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton12Picker.ItemDisplayBinding = new Binding("Name");
            FullCommandBarButton13Picker.ItemDisplayBinding = new Binding("Name");

            XlogUserNameValidationExpression = new Regex(@"^[A-Za-z0-9_]{1,31}$");
            BonesAllowedUsersValidationExpression = new Regex(@"^([A-Za-z0-9_]*[ \,]+)*([A-Za-z0-9_]*)?$");

            SetInitialValues();

            ClassicStatusBarSwitch_Toggled(null, new ToggledEventArgs(ClassicStatusBarSwitch.IsToggled));
            AllowBonesSwitch_Toggled(null, new ToggledEventArgs(AllowBonesSwitch.IsToggled));
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

        private void SetChildrenDarkModeTextColor(View view, bool darkmode)
        {
            if (view == null) 
                return; 

            if(view is Grid)
            {
                Grid grid = (Grid)view;
                foreach(View child in grid.Children)
                {
                    SetChildrenDarkModeTextColor(child, darkmode);
                }
            }
            else if (view is StackLayout)
            {
                StackLayout layout = (StackLayout)view;
                foreach (View child in layout.Children)
                {
                    SetChildrenDarkModeTextColor(child, darkmode);
                }
            }
            else if (view is Label)
            {
                Label l = (Label)view;
                if(darkmode ? l.TextColor == GHColors.Black : l.TextColor == GHColors.White)
                    l.TextColor = darkmode ? GHColors.White : GHColors.Black;
                else if (darkmode ? l.TextColor == GHColors.Green : l.TextColor == GHColors.LightGreen)
                    l.TextColor = darkmode ? GHColors.LightGreen : GHColors.Green;
            }
#if GNH_MAUI
            else if (view is Microsoft.Maui.Controls.Entry)
            {
                Microsoft.Maui.Controls.Entry l = (Microsoft.Maui.Controls.Entry)view;
                if (darkmode ? l.TextColor == GHColors.Black : l.TextColor == GHColors.White)
                    l.TextColor = darkmode ? GHColors.White : GHColors.Black;
            }
            else if (view is Microsoft.Maui.Controls.Picker)
            {
                Microsoft.Maui.Controls.Picker l = (Microsoft.Maui.Controls.Picker)view;
                l.TextColor = darkmode ? GHColors.White : GHColors.Black;
                l.TitleColor = darkmode ? GHColors.White : GHColors.Black;
            }
#else
            else if (view is Xamarin.Forms.Entry)
            {
                Xamarin.Forms.Entry l = (Xamarin.Forms.Entry)view;
                if (darkmode ? l.TextColor == GHColors.Black : l.TextColor == GHColors.White)
                    l.TextColor = darkmode ? GHColors.White : GHColors.Black;
            }
            else if (view is Xamarin.Forms.Picker)
            {
                Xamarin.Forms.Picker l = (Xamarin.Forms.Picker)view;
                l.TextColor = darkmode ? GHColors.White : GHColors.Black;
                l.TitleColor = darkmode ? GHColors.White : GHColors.Black;
            }
#endif
        }

        private void SetTournamentModeLabelColors(bool isTournament)
        {
            Color offColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            Color onColor = GHApp.DarkMode ? GHColors.LightGreen : GHColors.Green;
            Color usedColor = isTournament ? onColor : offColor;

            RecordLabel.TextColor = RecordSwitch.IsEnabled ? usedColor : GHColors.Gray;
            AutoUploadReplaysLabel.TextColor = AutoUploadReplaysSwitch.IsEnabled ? usedColor : GHColors.Gray;
            SaveStyleLabel.TextColor = SaveStylePicker.IsEnabled ? usedColor : GHColors.Gray;
            PostGameStatusLabel.TextColor = PostGameStatusSwitch.IsEnabled ? usedColor : GHColors.Gray;
            PostXlogLabel.TextColor = PostXlogSwitch.IsEnabled ? usedColor : GHColors.Gray;
            PostReplaysLabel.TextColor = PostReplaysSwitch.IsEnabled ? usedColor : GHColors.Gray;
            PostBonesLabel.TextColor = PostBonesSwitch.IsEnabled ? usedColor : GHColors.Gray;
            AllowBonesLabel.TextColor= AllowBonesSwitch.IsEnabled ? usedColor : GHColors.Gray;
            GZipLabel.TextColor = GZipSwitch.IsEnabled ? usedColor : GHColors.Gray;

            CustomWebHookLinkTitleLabel.TextColor = usedColor;
            AccountTitleLabel.TextColor = usedColor;
            PostingTitleLabel.TextColor = usedColor;
            PostXlogUserNameLabel.TextColor = usedColor;
            PostXlogPasswordLabel.TextColor = usedColor;
            CloudStorageTitleLabel.TextColor = usedColor;
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }

        private async Task SetSettingValues()
        {
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
                _gamePage.ShowMemory = MemorySwitch.IsToggled;
            Preferences.Set("ShowMemory", MemorySwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowFPS = FPSSwitch.IsToggled;
            Preferences.Set("ShowFPS", FPSSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowBattery = BatterySwitch.IsToggled;
            Preferences.Set("ShowBattery", BatterySwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowZoom = ZoomSwitch.IsToggled;
            Preferences.Set("ShowZoom", ZoomSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowRecording = ShowRecordingSwitch.IsToggled;
            Preferences.Set("ShowRecording", ShowRecordingSwitch.IsToggled);

            GHApp.UseGPU = GPUSwitch.IsToggled;
            if (_gamePage != null)
                _gamePage.UseMainGLCanvas = GPUSwitch.IsToggled;
            Preferences.Set("UseMainGLCanvas", GPUSwitch.IsToggled);

            GHApp.DisableAuxGPU = DisableAuxGPUSwitch.IsToggled;
            Preferences.Set("DisableAuxiliaryGLCanvas", DisableAuxGPUSwitch.IsToggled);
            bool useAuxGPU = GPUSwitch.IsToggled && !DisableAuxGPUSwitch.IsToggled;
            GHApp.UseAuxGPU = useAuxGPU;
            if (_gamePage != null)
                _gamePage.UseAuxiliaryGLCanvas = useAuxGPU;
            Preferences.Set("UseAuxiliaryGLCanvas", useAuxGPU);

            GHApp.AllowBones = AllowBonesSwitch.IsToggled;
            Preferences.Set("AllowBones", AllowBonesSwitch.IsToggled);

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
            if (FixRectsSwitch.IsEnabled)
            {
                GHApp.FixRects = FixRectsSwitch.IsToggled;
                Preferences.Set("FixRects", FixRectsSwitch.IsToggled);
            }

            GHApp.EmptyWishIsNothing = EmptyWishIsNothingSwitch.IsToggled;
            Preferences.Set("EmptyWishIsNothing", EmptyWishIsNothingSwitch.IsToggled);

            GHApp.MirroredCharacterClickAction = CharacterClickActionSwitch.IsToggled;
            if (_gamePage != null) /* During game only doubles as the option; outside of game sets the default */
                _gamePage.SetCharacterClickAction(CharacterClickActionSwitch.IsToggled);
            else
                Preferences.Set("CharacterClickAction", CharacterClickActionSwitch.IsToggled);

            if (RightMousePicker.SelectedIndex > -1 && RightMousePicker.SelectedItem != null && RightMousePicker.SelectedItem is MouseCommandItem)
            {
                GHApp.MirroredRightMouseCommand = ((MouseCommandItem)RightMousePicker.SelectedItem).Value;
                if (_gamePage != null)  /* During game only doubles as the option; outside of game sets the default */
                    _gamePage.SetRightMouseCommand(((MouseCommandItem)RightMousePicker.SelectedItem).Value);
                else
                    Preferences.Set("RightMouseCommand", ((MouseCommandItem)RightMousePicker.SelectedItem).Value);
            }

            if (MiddleMousePicker.SelectedIndex > -1 && MiddleMousePicker.SelectedItem != null && MiddleMousePicker.SelectedItem is MouseCommandItem)
            {
                GHApp.MirroredMiddleMouseCommand = ((MouseCommandItem)MiddleMousePicker.SelectedItem).Value;
                if (_gamePage != null)  /* During game only doubles as the option; outside of game sets the default */
                    _gamePage.SetMiddleMouseCommand(((MouseCommandItem)MiddleMousePicker.SelectedItem).Value);
                else
                    Preferences.Set("MiddleMouseCommand", ((MouseCommandItem)MiddleMousePicker.SelectedItem).Value);
            }

            GHApp.OkOnDoubleClick = DoubleClickSwitch.IsToggled;
            Preferences.Set("OkOnDoubleClick", DoubleClickSwitch.IsToggled);

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
                _gamePage.DesktopStatusBar = DesktopStatusBarSwitch.IsToggled;
            Preferences.Set("DesktopStatusBar", DesktopStatusBarSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.DesktopButtons = DesktopButtonsSwitch.IsToggled;
            Preferences.Set("DesktopButtons", DesktopButtonsSwitch.IsToggled);

            if(MenuFadeEffectsGrid.IsVisible)
            {
                if (_gamePage != null)
                    _gamePage.MenuFadeEffects = MenuFadeEffectsSwitch.IsToggled;
                Preferences.Set("MenuFadeEffects", MenuFadeEffectsSwitch.IsToggled);
            }

            if (_gamePage != null)
                _gamePage.MenuHighFilterQuality = MenuHighFilterQualitySwitch.IsToggled;
            Preferences.Set("MenuHighFilterQuality", MenuHighFilterQualitySwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowScore = ShowScoreSwitch.IsToggled;
            Preferences.Set("ShowScore", ShowScoreSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowXP = ShowXPSwitch.IsToggled;
            Preferences.Set("ShowXP", ShowXPSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.RightAligned2ndRow = RightAligned2ndRowSwitch.IsToggled;
            Preferences.Set("RightAligned2ndRow", RightAligned2ndRowSwitch.IsToggled);

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
            GHApp.TournamentMode = TournamentSwitch.IsToggled;
            Preferences.Set("TournamentMode", GHApp.TournamentMode);

            Preferences.Set("DefaultMapNoClipMode", !YesClipNormalSwitch.IsToggled);

#if GNH_MAUI
            Microsoft.Maui.Controls.Picker[] simplePickers = new Microsoft.Maui.Controls.Picker[6] 
#else
            Xamarin.Forms.Picker[] simplePickers = new Xamarin.Forms.Picker[6]
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
            Microsoft.Maui.Controls.Picker[] fullPickers = new Microsoft.Maui.Controls.Picker[13] 
#else
            Xamarin.Forms.Picker[] fullPickers = new Xamarin.Forms.Picker[13]
#endif
            {
                    FullCommandBarButton1Picker,
                    FullCommandBarButton2Picker,
                    FullCommandBarButton3Picker,
                    FullCommandBarButton4Picker,
                    FullCommandBarButton5Picker,
                    FullCommandBarButton6Picker,
                    FullCommandBarButton7Picker,
                    FullCommandBarButton8Picker,
                    FullCommandBarButton9Picker,
                    FullCommandBarButton10Picker,
                    FullCommandBarButton11Picker,
                    FullCommandBarButton12Picker,
                    FullCommandBarButton13Picker,
            };
            for (int i = 0; i < 6; i++)
            {
#if GNH_MAUI
                Microsoft.Maui.Controls.Picker targetPicker = simplePickers[i];
#else
                Xamarin.Forms.Picker targetPicker = simplePickers[i];
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
            for (int i = 0; i < 13; i++)
            {
#if GNH_MAUI
                Microsoft.Maui.Controls.Picker targetPicker = fullPickers[i]; 
#else
                Xamarin.Forms.Picker targetPicker = fullPickers[i];
#endif
                string keystr = "FullUILayoutCommandButton" + (i + 1);
                if (targetPicker.SelectedIndex < 0 || targetPicker.SelectedIndex >= GHApp.SelectableShortcutButtons.Count)
                {
                    if (Preferences.ContainsKey(keystr))
                        Preferences.Remove(keystr);
                }
                else
                {
                    Preferences.Set(keystr, GHApp.SelectableShortcutButtons[targetPicker.SelectedIndex].GetCommand());
                    if (_gamePage != null)
                        _gamePage.SetFullLayoutCommandButton(i, targetPicker.SelectedIndex);
                }
            }

            GHApp.InitializeMoreCommandButtons(SimpleCmdLayoutSwitch.IsToggled);
            if (_gamePage != null)
            {
                _gamePage.UseSimpleCmdLayout = SimpleCmdLayoutSwitch.IsToggled;
                _gamePage.MoreCmdPage = 1;
            }
            Preferences.Set("UseSimpleCmdLayout", SimpleCmdLayoutSwitch.IsToggled);

            GHApp.DarkMode = DarkModeSwitch.IsToggled;
            Preferences.Set("DarkMode", GHApp.DarkMode);

            GHApp.SilentMode = SilentModeSwitch.IsToggled;
            Preferences.Set("SilentMode", GHApp.SilentMode);

            if(WindowedModeStackLayout.IsVisible && WindowedModeSwitch.IsEnabled)
            {
                /* Do not update GHApp.WindowedMode, because it needs a restart */
                Preferences.Set("WindowedMode", WindowedModeSwitch.IsToggled);
            }

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
                    GHApp.FmodService.UnloadBanks(sound_bank_loading_type.Master);
                    GHApp.FmodService.UnloadBanks(sound_bank_loading_type.Preliminary);
                    if(_gamePage != null)
                    {
                        GHApp.FmodService.UnloadBanks(sound_bank_loading_type.Music);
                        GHApp.FmodService.UnloadBanks(sound_bank_loading_type.Game);
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
            }

            if (setupfiles)
            {
                try
                {
                    await GHApp.GnollHackService.InitializeSecrets(GHApp.CurrentSecrets);
                }
                catch (Exception ex)
                {
                    await DisplayAlert("Secrets Initialization Failed", "Initializing secrets failed: " + ex.Message, "OK");
                }
                await GHApp.SetSoundBanksUpForLoading();
            }

            if (loadbanks)
            {
                try
                {
                    GHApp.FmodService.LoadBanks(sound_bank_loading_type.Master);
                    GHApp.FmodService.LoadBanks(sound_bank_loading_type.Preliminary);
                    if(_gamePage != null)
                    {
                        GHApp.FmodService.LoadBanks(sound_bank_loading_type.Music);
                        GHApp.FmodService.LoadBanks(sound_bank_loading_type.Game);
                    }
                    if (_gamePage == null)
                    {
                        GHApp.FmodService.PlayMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, GHConstants.IntroMusicVolume, 1.0f);
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
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
            bool mem = false, fps = false, zoom = false, battery = false, showrecording = true, autoupload = false, gpu = GHApp.IsGPUDefault, disableauxgpu = false, simplecmdlayout = GHConstants.DefaultSimpleCmdLayout, darkmode = false, windowedmode = false, bank = true, navbar = GHConstants.DefaultHideNavigation, statusbar = GHConstants.DefaultHideStatusBar;
            bool allowbones = true, emptywishisnothing = true, doubleclick = GHApp.IsDesktop, recordgame = false, gzip = GHConstants.GZipIsDefaultReplayCompression, lighterdarkening = false, accuratedrawing = GHConstants.DefaultAlternativeLayerDrawing, html = GHConstants.DefaultHTMLDumpLogs, singledumplog = GHConstants.DefaultUseSingleDumpLog, streamingbanktomemory = false, streamingbanktodisk = false, wallends = GHConstants.DefaultDrawWallEnds;
            bool breatheanimations = GHConstants.DefaultBreatheAnimations; //, put2bag = GHConstants.DefaultShowPickNStashContextCommand, prevwep = GHConstants.DefaultShowPrevWepContextCommand;
            bool devmode = GHConstants.DefaultDeveloperMode, logmessages = GHConstants.DefaultLogMessages, tournament = false, hpbars = false, nhstatusbarclassic = GHConstants.IsDefaultStatusBarClassic, desktopstatusbar = false, rightaligned2ndrow = false, showscore = false, showxp = false, desktopbuttons = false, menufadeeffects = false, menuhighfilterquality = true, pets = true, orbs = true, orbmaxhp = false, orbmaxmana = false, mapgrid = false, playermark = false, monstertargeting = false, walkarrows = true;
            bool forcemaxmsg = false, showexstatus = false, noclipmode = GHConstants.DefaultMapNoClipMode, silentmode = false, characterclickaction = false;
            bool postgamestatus = GHConstants.DefaultPosting, postdiagnostics = GHConstants.DefaultPosting, postxlog = GHConstants.DefaultPosting, postreplays = GHConstants.DefaultPosting, postbones = GHConstants.DefaultPosting, boneslistisblack = false;
            bool longermsghistory = false, xlog_release_account = false, forcepostbones = false, fixrects = false;
            long primarygpucache = -2, secondarygpucache = -2;
            int rightmouse = GHConstants.DefaultRightMouseCommand, middlemouse = GHConstants.DefaultMiddleMouseCommand;
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

            int[] fullCmdIdxs = new int[13];
            for (int i = 0; i < 13; i++)
            {
                string keystr = "FullUILayoutCommandButton" + (i + 1);
                int defCmd = GHApp.DefaultShortcutButton(0, i, false).GetCommand();
                int savedCmd = Preferences.Get(keystr, defCmd);
                int listselidx = GHApp.SelectableShortcutButtonIndexInList(savedCmd, defCmd);
                fullCmdIdxs[i] = listselidx;
            }

            darkmode = GHApp.DarkMode; // Preferences.Get("DarkMode", false);
            windowedmode = Preferences.Get("WindowedMode", false);
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
            tournament = GHApp.TournamentMode;
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
            doubleclick = Preferences.Get("OkOnDoubleClick", GHApp.IsDesktop);
            recordgame = Preferences.Get("RecordGame", false);
            gzip = Preferences.Get("UseGZipForReplays", GHConstants.GZipIsDefaultReplayCompression);
            fixrects = Preferences.Get("FixRects", GHApp.IsFixRectsDefault);
            noclipmode = Preferences.Get("DefaultMapNoClipMode", GHConstants.DefaultMapNoClipMode);
            savestyle = Preferences.Get("AppSwitchSaveStyle", 0);
            primarygpucache = Preferences.Get("PrimaryGPUCacheLimit", -2L);
            secondarygpucache = Preferences.Get("SecondaryGPUCacheLimit", -2L);
            disableauxgpu = Preferences.Get("DisableAuxiliaryGLCanvas", false);
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
                desktopstatusbar = Preferences.Get("DesktopStatusBar", GHApp.IsDesktop);
                desktopbuttons = Preferences.Get("DesktopButtons", GHApp.IsDesktop);
                menufadeeffects = Preferences.Get("MenuFadeEffects", GHConstants.AreMenuFadeEffectsDefault);
                menuhighfilterquality = Preferences.Get("MenuHighFilterQuality", GHConstants.IsMenuHighFilterQualityDefault);
                showscore = Preferences.Get("ShowScore", GHApp.IsDesktop);
                showxp = Preferences.Get("ShowXP", GHApp.IsDesktop);
                rightaligned2ndrow = Preferences.Get("RightAligned2ndRow", false);
                pets = Preferences.Get("ShowPets", true);
                orbs = Preferences.Get("ShowOrbs", true);
                orbmaxhp = Preferences.Get("ShowMaxHealthInOrb", false);
                orbmaxmana = Preferences.Get("ShowMaxManaInOrb", false);
                playermark = Preferences.Get("PlayerMark", false);
                monstertargeting = Preferences.Get("MonsterTargeting", false);
                walkarrows = Preferences.Get("WalkArrows", true);
                mem = Preferences.Get("ShowMemory", false);
                fps = Preferences.Get("ShowFPS", false);
                zoom = Preferences.Get("ShowZoom", false);
                battery = Preferences.Get("ShowBattery", false);
                showrecording = Preferences.Get("ShowRecording", true);
                autoupload = Preferences.Get("AutoUploadReplays", false);
                gpu = Preferences.Get("UseMainGLCanvas", GHApp.IsGPUDefault && GHApp.IsGPUAvailable);
                simplecmdlayout = Preferences.Get("UseSimpleCmdLayout", GHConstants.DefaultSimpleCmdLayout);
                msgnum = Preferences.Get("NumDisplayedMessages", GHConstants.DefaultMessageRows);
                petrows = Preferences.Get("NumDisplayedPetRows", GHConstants.DefaultPetRows);
                lighterdarkening = Preferences.Get("LighterDarkening", GHConstants.DefaultLighterDarkening);
                accuratedrawing = Preferences.Get("AlternativeLayerDrawing", GHConstants.DefaultAlternativeLayerDrawing);
                wallends = Preferences.Get("DrawWallEnds", GHConstants.DefaultDrawWallEnds);
                breatheanimations = Preferences.Get("BreatheAnimations", GHConstants.DefaultBreatheAnimations);
                //put2bag = Preferences.Get("ShowPut2BagContextCommand", GHConstants.DefaultShowPickNStashContextCommand);
                //prevwep = Preferences.Get("ShowPrevWepContextCommand", GHConstants.DefaultShowPrevWepContextCommand);
                longermsghistory = GHApp.SavedLongerMessageHistory; // Preferences.Get("LongerMessageHistory", false);
                characterclickaction = Preferences.Get("CharacterClickAction", GHConstants.DefaultCharacterClickAction); /* Default value */
                rightmouse = Preferences.Get("RightMouseCommand", GHConstants.DefaultRightMouseCommand);
                middlemouse = Preferences.Get("MiddleMouseCommand", GHConstants.DefaultMiddleMouseCommand);
            }
            else
            {
                cursor = (int)_gamePage.CursorStyle;
                graphics = (int)_gamePage.GraphicsStyle;
                maprefresh = (int)_gamePage.MapRefreshRate;
                mapgrid = _gamePage.MapGrid;
                forcemaxmsg = _gamePage.ForceAllMessages;
                ForceMaxMessageSwitch.IsEnabled = true;
                ForceMaxMessageLabel.TextColor = darkmode ? GHColors.White : GHColors.Black;
                showexstatus = _gamePage.ShowExtendedStatusBar;
                ShowExtendedStatusBarSwitch.IsEnabled = true;
                ShowExtendedStatusBarLabel.TextColor = darkmode ? GHColors.White : GHColors.Black;
                nhstatusbarclassic = _gamePage.ClassicStatusBar;
                desktopstatusbar = _gamePage.DesktopStatusBar;
                desktopbuttons = _gamePage.DesktopButtons;
                menufadeeffects = _gamePage.MenuFadeEffects;
                menuhighfilterquality = _gamePage.MenuHighFilterQuality;
                showscore = _gamePage.ShowScore;
                showxp = _gamePage.ShowXP;
                rightaligned2ndrow = _gamePage.RightAligned2ndRow;
                hpbars = _gamePage.HitPointBars;
                pets = _gamePage.ShowPets;
                orbs = _gamePage.ShowOrbs;
                orbmaxhp = _gamePage.ShowMaxHealthInOrb;
                orbmaxmana = _gamePage.ShowMaxManaInOrb;
                playermark = _gamePage.PlayerMark;
                monstertargeting = _gamePage.MonsterTargeting;
                walkarrows = _gamePage.WalkArrows;
                mem = _gamePage.ShowMemory;
                fps = _gamePage.ShowFPS;
                zoom = _gamePage.ShowZoom;
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
                characterclickaction = GHApp.MirroredCharacterClickAction; // _gamePage.GetCharacterClickAction(); /* Value of the option in the (saved) game */
                rightmouse = GHApp.MirroredRightMouseCommand; //_gamePage.GetRightMouseCommand();
                middlemouse = GHApp.MirroredMiddleMouseCommand; //_gamePage.GetMiddleMouseCommand();
            }

            CursorPicker.SelectedIndex = cursor;
            GraphicsPicker.SelectedIndex = graphics;
            RefreshRatePicker.SelectedIndex = Math.Min(RefreshRatePicker.Items.Count - 1, maprefresh);
            SaveStylePicker.SelectedIndex = savestyle;
            GridSwitch.IsToggled = mapgrid;
            HitPointBarSwitch.IsToggled = hpbars;
            ClassicStatusBarSwitch.IsToggled = nhstatusbarclassic;
            DesktopStatusBarSwitch.IsToggled = desktopstatusbar;
            ShowScoreSwitch.IsToggled = showscore;
            ShowXPSwitch.IsToggled = showxp;
            RightAligned2ndRowSwitch.IsToggled = rightaligned2ndrow;
            DesktopButtonsSwitch.IsToggled = desktopbuttons;
            if (GHApp.IsiOS)
            {
                MenuFadeEffectsGrid.IsVisible = false;
                MenuFadeEffectsSwitch.IsEnabled = false;
                MenuFadeEffectsLabel.TextColor = GHColors.Gray;
            }
            else
            {
                MenuFadeEffectsSwitch.IsToggled = menufadeeffects;
            }
            MenuHighFilterQualitySwitch.IsToggled = menuhighfilterquality;
            PetSwitch.IsToggled = pets;
            OrbSwitch.IsToggled = orbs;
            MaxHealthInOrbSwitch.IsToggled = orbmaxhp;
            MaxManaInOrbSwitch.IsToggled = orbmaxmana;
            PlayerMarkSwitch.IsToggled = playermark;
            MonsterTargetingSwitch.IsToggled = monstertargeting;
            WalkArrowSwitch.IsToggled = walkarrows;
            YesClipNormalSwitch.IsToggled = !noclipmode;
            MemorySwitch.IsToggled = mem;
            FPSSwitch.IsToggled = fps;
            ZoomSwitch.IsToggled = zoom;
            BatterySwitch.IsToggled = battery;
            ShowRecordingSwitch.IsToggled = showrecording;
            AutoUploadReplaysSwitch.IsToggled = autoupload;
            DisableAuxGPUSwitch.IsToggled = disableauxgpu;
            FixRectsSwitch.IsToggled = fixrects;
            if (GHApp.IsGPUAvailable)
            {
                GPUSwitch.IsToggled = gpu;
                DisableAuxGPUSwitch.IsEnabled = gpu;
                DisableAuxGPULabel.TextColor = gpu ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
                FixRectsSwitch.IsEnabled = gpu;
                FixRectsLabel.TextColor = gpu ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            }
            else
            {
                GPUSwitch.IsToggled = false;
                GPUSwitch.IsEnabled = false;
                GPULabel.IsEnabled = false;
                GPULabel.TextColor = GHColors.Gray;

                FixRectsSwitch.IsEnabled = false;
                FixRectsLabel.TextColor = GHColors.Gray;

                DisableAuxGPUSwitch.IsEnabled = false;
                DisableAuxGPULabel.TextColor = GHColors.Gray;
            }
            SimpleCmdLayoutSwitch.IsToggled = simplecmdlayout;
            DarkModeSwitch.IsToggled = darkmode;
            SilentModeSwitch.IsToggled = silentmode;
            WindowedModeSwitch.IsToggled = windowedmode;
            if (!GHApp.IsDesktop)
            {
                WindowedModeSwitch.IsEnabled = false;
                WindowedModeLabel.IsEnabled = false;
                WindowedModeLabel.TextColor = GHColors.Gray;
                WindowedModeStackLayout.IsVisible = false;
            }
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
            TournamentSwitch.IsToggled = tournament;
            if (_gamePage != null) /* Cannot turn on or off in the middle of the game */
            {
                TournamentSwitch.IsEnabled = false;
                TournamentLabel.TextColor = GHColors.Gray;
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
            AllowBonesSwitch.IsToggled = allowbones;
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
            CharacterClickActionSwitch.IsToggled = characterclickaction;
            DoubleClickSwitch.IsToggled = doubleclick;

            if(RightMousePicker.ItemsSource != null)
            {
                for (int i = 0; i < RightMousePicker.ItemsSource.Count; i++)
                {
                    object o = RightMousePicker.ItemsSource[i];
                    if (o is MouseCommandItem)
                    {
                        if (((MouseCommandItem)o).Value == rightmouse)
                        {
                            RightMousePicker.SelectedIndex = i;
                            break;
                        }
                    }
                }
            }

            if (MiddleMousePicker.ItemsSource != null)
            {
                for (int i = 0; i < MiddleMousePicker.ItemsSource.Count; i++)
                {
                    object o = MiddleMousePicker.ItemsSource[i];
                    if (o is MouseCommandItem)
                    {
                        if (((MouseCommandItem)o).Value == middlemouse)
                        {
                            MiddleMousePicker.SelectedIndex = i;
                            break;
                        }
                    }
                }
            }

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

            SimpleCommandBarButtonsLayout.IsVisible = simplecmdlayout;
            SimpleCommandBarButton1Picker.SelectedIndex = cmdidxs[0];
            SimpleCommandBarButton2Picker.SelectedIndex = cmdidxs[1];
            SimpleCommandBarButton3Picker.SelectedIndex = cmdidxs[2];
            SimpleCommandBarButton4Picker.SelectedIndex = cmdidxs[3];
            SimpleCommandBarButton5Picker.SelectedIndex = cmdidxs[4];
            SimpleCommandBarButton6Picker.SelectedIndex = cmdidxs[5];
            //SimpleCommandBarButton1Picker.IsEnabled = simplecmdlayout;
            //SimpleCommandBarButton2Picker.IsEnabled = simplecmdlayout;
            //SimpleCommandBarButton3Picker.IsEnabled = simplecmdlayout;
            //SimpleCommandBarButton4Picker.IsEnabled = simplecmdlayout;
            //SimpleCommandBarButton5Picker.IsEnabled = simplecmdlayout;
            //SimpleCommandBarButton6Picker.IsEnabled = simplecmdlayout;
            //SimpleCommandBarButton1Label.TextColor = simplecmdlayout ? (darkmode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            //SimpleCommandBarButton2Label.TextColor = simplecmdlayout ? (darkmode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            //SimpleCommandBarButton3Label.TextColor = simplecmdlayout ? (darkmode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            //SimpleCommandBarButton4Label.TextColor = simplecmdlayout ? (darkmode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            //SimpleCommandBarButton5Label.TextColor = simplecmdlayout ? (darkmode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            //SimpleCommandBarButton6Label.TextColor = simplecmdlayout ? (darkmode ? GHColors.White : GHColors.Black) : GHColors.Gray;

            FullCommandBarButtonsLayout.IsVisible = !simplecmdlayout;
            FullCommandBarButton1Picker.SelectedIndex = fullCmdIdxs[0];
            FullCommandBarButton2Picker.SelectedIndex = fullCmdIdxs[1];
            FullCommandBarButton3Picker.SelectedIndex = fullCmdIdxs[2];
            FullCommandBarButton4Picker.SelectedIndex = fullCmdIdxs[3];
            FullCommandBarButton5Picker.SelectedIndex = fullCmdIdxs[4];
            FullCommandBarButton6Picker.SelectedIndex = fullCmdIdxs[5];
            FullCommandBarButton7Picker.SelectedIndex = fullCmdIdxs[6];
            FullCommandBarButton8Picker.SelectedIndex = fullCmdIdxs[7];
            FullCommandBarButton9Picker.SelectedIndex = fullCmdIdxs[8];
            FullCommandBarButton10Picker.SelectedIndex = fullCmdIdxs[9];
            FullCommandBarButton11Picker.SelectedIndex = fullCmdIdxs[10];
            FullCommandBarButton12Picker.SelectedIndex = fullCmdIdxs[11];
            FullCommandBarButton13Picker.SelectedIndex = fullCmdIdxs[12];

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
                OrbsLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                PetSwitch.IsEnabled = false;
                PetsLabel.TextColor = GHColors.Gray;
                DesktopStatusBarSwitch.IsEnabled = false;
                DesktopStatusBarLabel.TextColor = GHColors.Gray;
                ShowScoreSwitch.IsEnabled = false;
                ShowScoreLabel.TextColor = GHColors.Gray;
                ShowXPSwitch.IsEnabled = false;
                ShowXPLabel.TextColor = GHColors.Gray;
                RightAligned2ndRowSwitch.IsEnabled = false;
                RightAligned2ndRowLabel.TextColor = GHColors.Gray;
            }
            else
            {
                OrbSwitch.IsEnabled = false;
                OrbsLabel.TextColor = GHColors.Gray;
                PetSwitch.IsEnabled = true;
                PetsLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                DesktopStatusBarSwitch.IsEnabled = true;
                DesktopStatusBarLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                ShowScoreSwitch.IsEnabled = true;
                ShowScoreLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                ShowXPSwitch.IsEnabled = true;
                ShowXPLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                RightAligned2ndRowSwitch.IsEnabled = true;
                RightAligned2ndRowLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            }
            OrbSwitch_Toggled(sender, new ToggledEventArgs(OrbSwitch.IsToggled));
            PetSwitch_Toggled(sender, new ToggledEventArgs(PetSwitch.IsToggled));
        }

        private void OrbSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (!ClassicStatusBarSwitch.IsToggled || OrbSwitch.IsToggled)
            {
                MaxHealthInOrbSwitch.IsEnabled = true;
                MaxHealthInOrbLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                MaxManaInOrbSwitch.IsEnabled = true;
                MaxManaInOrbLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
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
                PetRowsLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
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
                LogMessageLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
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
                HTMLDumpLogLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
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
                await Task.Delay(100);
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
            SetTournamentModeLabelColors(TournamentSwitch.IsToggled);
            //PostXlogUserNameLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            BonesAllowedUsersLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
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
            if(TournamentSwitch.IsToggled)
            {
                if(!GHApp.XlogUserNameVerified)
                {
                    PopupTitleLabel.TextColor = GHColors.Orange;
                    PopupTitleLabel.Text = "Tournament Verification";
                    PopupLabel.Text = "User name and password in the Server Posting section must be verified for the Tournament Mode.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                    CloseButton.IsEnabled = true;
                    await MainScrollView.ScrollToAsync(0, PostXlogUserNameStackLayout.Y, true);
                    return;
                }
            }
            await MaybeShowPleaseWait();
            await SetSettingValues();
            GHApp.CurrentMainPage?.InvalidateCarousel();
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
                await SetSettingValues();
                GHApp.CurrentMainPage?.InvalidateCarousel();
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
            SimpleCommandBarButtonsLayout.IsVisible = e.Value;
            FullCommandBarButtonsLayout.IsVisible = !e.Value;
        }

        private async void XlogAccountButton_Clicked(object sender, EventArgs e)
        {
            XlogAccountButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            GHApp.XlogReleaseAccount = XlogReleaseAccountSwitch.IsToggled;
            await GHApp.OpenBrowser(this, "GnollHack Account", new Uri(GHApp.XlogAccountAddress));
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

        private void AllowBonesSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            PostBonesSwitch.IsEnabled = e.Value;
            //if (e.Value)
            //{
            //    PostBonesLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            //}
            //else
            //{
            //    PostBonesLabel.TextColor = GHColors.Gray;
            //}
            PostBonesSwitch_Toggled(sender, new ToggledEventArgs(PostBonesSwitch.IsToggled));
            SetTournamentModeLabelColors(TournamentSwitch.IsToggled);
        }

#if GNH_MAUI
        private void BonesAllowedUsersLabel_TapGestureRecognizer_Tapped(object sender, TappedEventArgs e)
#else
        private void BonesAllowedUsersLabel_TapGestureRecognizer_Tapped(object sender, EventArgs e)
#endif
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
                BonesListLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                BonesAllowedUsersLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
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

#if GNH_MAUI
        private void RecordLabel_TapGestureRecognizer_Tapped(object sender, TappedEventArgs e)
#else
        private void RecordLabel_TapGestureRecognizer_Tapped(object sender, EventArgs e)
#endif
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

#if GNH_MAUI
        private void GZipLabel_TapGestureRecognizer_Tapped(object sender, TappedEventArgs e)
#else
        private void GZipLabel_TapGestureRecognizer_Tapped(object sender, EventArgs e)
#endif
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
            FixRectsSwitch.IsEnabled = e.Value;
            FixRectsLabel.TextColor = e.Value ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            DisableAuxGPUSwitch.IsEnabled = e.Value;
            DisableAuxGPULabel.TextColor = e.Value ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;

            if (_isManualTogglingEnabled && e.Value && !GHApp.IsGPUDefault)
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

        private void ShowTournamentInfoPopup()
        {
            PopupTitleLabel.TextColor = GHColors.Orange;
            PopupTitleLabel.Text = "Tournament Mode";
            PopupLabel.Text = "Tournament Mode will force on Post Game Progress, Post Top Scores, Allow Ghost Levels, Share Bones Files, Record Game, and Auto-Upload to Cloud settings. The mode will also disable all special game play modes, custom links, and custom webhooks." + (GHApp.XlogUserNameVerified ? "" : Environment.NewLine + Environment.NewLine + "Please make sure that your user name and password for Server Posting are verified before proceeding.");
            PopupOkButton.IsEnabled = true;
            PopupGrid.IsVisible = true;
        }

        private void TournamentSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if(e.Value && !GHApp.TournamentMode)
            {
                ShowTournamentInfoPopup();
            }
            SetTournamentModeLabelColors(e.Value);
        }

        private void PostDiagnosticDataSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (e.Value && !GHApp.PostingDiagnosticData && GHApp.IsiOS)
            {
                PopupTitleLabel.TextColor = GHColors.Orange;
                PopupTitleLabel.Text = "Share With App Developers";
                PopupLabel.Text = "To enable us to get information on app crashes, please make sure that also Share With App Developers is on in the Setting app under Privacy -> Analytics.";
                PopupOkButton.IsEnabled = true;
                PopupGrid.IsVisible = true;
            }
        }

        private void DarkModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (_isManualTogglingEnabled)
            {
                GHApp.DarkMode = e.Value;
                bkgView.InvalidateSurface();
                lblHeader.TextColor = e.Value ? GHColors.White : GHColors.Black;
                SetChildrenDarkModeTextColor(RootLayout, e.Value);
                if (_gameMenuPage != null)
                {
                    _gameMenuPage.UpdateDarknessMode();
                }
            }
        }

        private void TextEntry_Completed(object sender, EventArgs e)
        {
            if (TextOkButton.IsEnabled)
            {
                TextOkButton_Clicked(sender, e);
            }
        }

        private void TournamentLabel_TapGestureRecognizer_Tapped(object sender, EventArgs e)
        {
            ShowTournamentInfoPopup();
        }

        private void WindowedModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if(_isManualTogglingEnabled)
            {
                bool oldValue = GHApp.WindowedMode;
                if (oldValue != e.Value)
                {
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Restart Required";
                    PopupLabel.Text = "Please restart GnollHack for the changes in Windowed Mode to take effect.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
            }
        }
    }


}