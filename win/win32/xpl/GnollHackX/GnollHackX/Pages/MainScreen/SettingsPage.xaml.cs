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
using static System.Net.Mime.MediaTypeNames;


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
    public partial class SettingsPage : CustomModalPage, ISpecialKeyPressHandlingPage
    {
        private GamePage _gamePage;
        private GameMenuPage _gameMenuPage;
        private MainPage _mainPage;
        private bool _doChangeVolume = false;
        public Regex EngraveQuickTextValidationExpression { get; set; }
        public Regex XlogUserNameValidationExpression { get; set; }
        public Regex BonesAllowedUsersValidationExpression { get; set; }

        private static readonly Dictionary<string, string> _settingDescriptions = new Dictionary<string, string>
        {
            { "GPU Acceleration", "Enables and disables the GPU acceleration of the game. If disabled, the game uses CPU for rendering graphics. Note that GPU acceleration can cause crashes on some systems." },
            { "Graphics", "Shows the game in 2D tile graphics or in ASCII text." },
            { "Map FPS", "Determines the max refresh rate of the game. This can be used to save battery or to make the game smoother. The default refresh rate varies by device (typically 60 FPS)." },
            { "Screen Resolution", "Determines the rendering resolution of the game map." },
            { "Screen Scale", "Custom scale to make UI components larger or smaller than normal." },
            { "Windowed Mode", "Determines if the game runs in windowed or fullscreen mode." },
            { "Edge to Edge", "Determines whether the game screen extends over notches or similar areas." },
            { "Cursor Style", "*(ASCII graphics only)* Determines the way how the player character is indicated in the game." },
            { "Hide Navigation", "Determines whether to hide the operating system navigation buttons in the bottom of the screen." },
            { "Hide Status Bar", "Determines if the top status bar of the operating system is hidden." },
            { "Show Battery", "Displays a battery icon in the status bar indicating the current battery level." },
            { "Show FPS", "Displays a frames-per-second icon in the status bar indicating the current refresh rate of the game." },
            { "Show Zoom", "Displays a zoom icon in the status bar indicating the current zoom level of the map." },
            { "Silent Mode", "Game sounds and music are not played if turned on." },
            { "Dark Mode", "**Off (Light mode):** Menu and text window backgrounds are light and texts are black.\n**On (Dark mode):** Menu and text window backgrounds are dark and texts are white." },
            { "Tournament Mode", "Toggles several other settings to values required by tournaments like [[Junethack]]." },
            { "Metric System", "Enables the metric system, displaying weights in kilograms (`kg`), grams (`g`), or tons. If disabled (default), the game uses the imperial system, displaying weights in pounds (`lbs` / `lb`), ounces (`oz`), or hundredweights (`cwt`). Mirrors the `metric_system` option." },
            { "Show Dice as Ranges", "Converts traditional D&D-style dice notation (like `2d6`) into a simple range (like `2-12`) in item descriptions. If disabled, traditional XdY notation is used. Mirrors the `show_dice_as_ranges` option." },
            { "Show Damage Formula", "Shows the underlying dice formula (e.g., `1d6+2`) used to calculate an item's damage when examining it. Mirrors the `show_damage_formula` option." },
            { "Menu Fade Effects", "Determines whether the texts in the menu and text pages fade in and out when opening and closing the page. Only available on Android." },
            { "Improved Menu Images", "**Off:** Nearest neighbour (faster but worse quality).\n**On:** Bilinear interpolation (slower but better quality)." },
            { "Highlighted Menu Keys", "**Off:** Hotkeys are grayed.\n**On:** Hotkeys are colored black/white based on light/dark mode." },
            { "Show Equipment Icons", "Displays equipment slot icons in the inventory menu." },
            { "Equipment Flip Animation", "Enables a 3D flip animation when moving between inventory and equipment screens." },
            { "Worn Shows Equipment", "Determines whether the worn item command (`]`) shows a full graphical equipment screen with all body slots or a plain text list of currently worn items. Mirrors the `worn_shows_equipment` option." },
            { "Starting and Gifted Pets", "Determines whether the player starts with a pet and receives gifted pets in special situations. Turn off the setting (or enable the option) if you are attempting a petless conduct. Mirrors the `pets_not_gifted` option (with inverted value)." },
            { "Allow Ghost Levels", "Determines if the game uses so-called bones files of dead characters." },
            { "Simple Command Layout", "**Off:** Shows two rows of commands in the bottom of the screen, and extra commands in more commands.\n**On:** Shows one row of commands and only the most important extra commands in more commands." },
            { "Alternative Zoom Button", "Displays the alternative zoom toggle button in the top-right corner of the map." },
            { "Travel Mode Button", "Displays the travel mode toggle button in the top-right corner of the map." },
            { "Auto-Dig Button", "Displays the auto-dig toggle button in the top-right corner of the map." },
            { "Ignore Stopping Button", "Displays the ignore stopping toggle button in the top-right corner of the map." },
            { "Desktop Buttons", "Determines whether Stats and Equipment buttons are shown on the left and right side of command buttons." },
            { "Skill Context Button", "Determines whether to show the skill context button on the left side of the screen." },
            { "Polearm Context Button", "Determines whether to show the polearm context button on the left side of the screen." },
            { "Single Commands Page", "Uses a single large commands page instead of several categorized pages." },
            { "Classic Status Bar", "**Off:** Shows the modern graphical game status bar.\n**On:** Shows the game status bar in text in the NetHack style." },
            { "Desktop Status Bar", "Displays ability scores and other extra information on the status bar." },
            { "Show Score", "Displays the game score on the status bar." },
            { "Show XP", "Displays experience points on the status bar." },
            { "Right-Aligned on 2nd Row", "Moves the display location of score, experience points and gold to the 2nd status bar row." },
            { "Show Status Screen", "Toggle for showing the status screen. The same as tapping the middle area of the status bar." },
            { "Grid", "Determines if the game shows grid lines in the game for easier tile position discernment." },
            { "Grid Opacity", "Adjusts the opacity of the grid lines if they are shown." },
            { "Hit Point Bars", "Determines if the game shows hit point bars under the player character, pets, and monsters." },
            { "Player Mark", "Determines if a green targeting icon is displayed above the player character." },
            { "Targeting", "Determines if a red targeting icon is displayed above hostile monsters." },
            { "Show Pets", "Determines if the game shows pet icons in the top of the game screen (under the game status bar)." },
            { "Pet Rows", "Determines how many rows of pet icons are allowed in the top of the screen." },
            { "Orbs", "Determines if health and mana orbs are shown in the top left corner of the screen." },
            { "Show Max Hit Points", "Determines if maximum hit points are shown under the current hit points in the health orb." },
            { "Show Max Mana", "Determines if maximum mana is shown under the current mana in the mana orb." },
            { "Messages", "Determines the number of messages shown in the bottom left corner of the screen." },
            { "Show All", "Toggle for showing all messages. The same as tapping the message area in the game screen." },
            { "Walk Arrows", "Determines if there are walk arrows when Travel Mode is disabled." },
            { "Default Auto-Center", "Determines if the Auto-Center button in the game screen is enabled by default." },
            { "Show Keyboard Shortcuts", "Determines if keyboard shortcuts are shown in menus." },
            { "Lighter Unlit Areas", "**Off:** Unlit areas are darker.\n**On:** Unlit areas are lighter." },
            { "Draw Wall Ends", "Determines if the game draws wall end graphics. Can be disabled to save processor time." },
            { "Breathing Animations", "Determines if the game shows the breathing animations of various creatures." },
            { "Empty Wish is Nothing", "**Off:** A random item is received if the wish is left blank.\n**On:** Nothing is received. Keep on to preserve wishless conduct." },
            { "Character Click Action", "Binds clicking or tapping on the player character to execute an action appropriate to the location (e.g., descending stairs or resting). Mirrors the `self_click_action` option." },
            { "OK on Double Click", "Double-clicking a menu item also presses OK button automatically in menus." },
            { "Traditional Get Position", "When asked to select a location, the location is selected by moving around a tile-based game cursor using arrow icons or keyboard, rather than clicking a location on the map." },
            { "Auto-Dig", "Automatically attempts to dig through solid rock or walls when moving into, clicking, or tapping them if wielding a suitable digging tool (like a pick-axe or mattock). Mirrors the `autodig` option." },
            { "Ignore Stopping", "Prevents automated pathfinding travel from stopping when your character passes over items, closed doors, or engravings. Mirrors the `ignore_stopping` option." },
            { "Right Mouse Button", "Configures the game command bound to a right mouse button click. The default \"by role\" setting triggers role-specific default actions. Mirrors the `right_click_command` option." },
            { "Middle Mouse Button", "Configures the game command bound to a middle mouse button click. The default \"by role\" setting triggers role-specific default actions. Mirrors the `middle_click_command` option." },
            { "Quick Engrave Text", "The default text written when using the quick engrave command. Mirrors the `engrave_quicktext` option." },
            { "Quick Engrave Style", "Selects which engraving method or tool is automatically chosen when executing the quick engrave command: ask every time, always use finger, or use last item. Mirrors the `engrave_quickstyle` option." },
            { "Master", "Changes the volume of all sounds and music." },
            { "Music", "Changes the volume of music." },
            { "Ambient", "Changes the volume of ambient sounds." },
            { "Dialogue", "Changes the volume of voice overs." },
            { "Effects", "Changes the volume of sound effects." },
            { "Interface", "Changes the volume of interface sounds, such as button clicks." },
            { "Post Game Progress", "Posts updates of new events of your journey to a selected channel on a Discord Server." },
            { "Webhook Link", "Specifies the webhook link to the channel on a Discord server where your game progress is posted." },
            { "Post Diagnostic Data", "Enables posting of anonymous game diagnostics and crash logs to the developer server." },
            { "Account", "Enables you to access and specify the GnollHack Server web address." },
            { "User Name", "Your user name on the GnollHack server." },
            { "Password", "Your password on the GnollHack server." },
            { "Post Top Scores", "Posts your score to the GnollHack Server when your game has ended." },
            { "Share Bones Files", "Posts bones files to the server, and allows you to encounter other players' dead characters' ghosts (if Allow Ghost Levels is on)." },
            { "Use Blacklist", "Determines whether to use blacklist or whitelist in blocking unwanted users' bones files." },
            { "Whitelist/Blacklist", "List of allowed or blocked user names on the GnollHack server for bones files." },
            { "Save File Tracking", "Determines whether the save files are tracked by the GnollHack Server." },
            { "Record Game", "Determines whether the game records a replay of your game." },
            { "Show Recording", "Determines whether a red dot appears on the status bar to indicate that game recording is on." },
            { "Auto-Upload to Cloud", "Determines whether the saved replay is automatically uploaded to an Azure cloud storage." },
            { "Cloud Storage", "Specifies a custom Azure cloud storage connection string to upload replays to." },
            { "Developer Mode", "Activates Developer Mode, which enables [[Wizard Mode]] and editing the options file." },
            { "Debug Logging", "Debug information is written in the app log." },
            { "Low-Level Logging", "Extensive logging of various low-level events. Can clog the app log quickly." },
            { "Screen Logging", "Toggles printing of log messages directly on the game screen." },
            { "Debug Post Channel", "Use an alternative post channel instead of the one specified under Post Game Progress." },
            { "Show Memory", "Shows the current managed memory usage on the game screen." },
            { "Low Disk Space Warning", "Displays a warning if free disk space is low (less than 5 GB) to prevent save game corruption." },
            { "Load Sound Banks", "Determines if the sound banks are loaded. Can be disabled to save memory, but the game will have no sounds." },
            { "Streaming Banks to Memory", "**Off:** Streamed from their default location (or to disk).\n**On:** Read to memory and streamed from there (uses more memory)." },
            { "Streaming Banks to Disk", "**Off:** Streamed from their default location (or to memory).\n**On:** Copied to the storage and streamed from there (uses more storage space)." },
            { "Longer Message History", "Shows 16,384 last messages instead of 250, and adds a search bar. Switches off automatically for performance reasons upon adding new messages or restarting." },
            { "Hide Message History", "The game will not show latest messages at all. Useful for taking cleaner screenshots." },
            { "Use Single Dumplog", "**Off:** The game asks whether to open plain text or HTML dumplog.\n**On:** Opens the dumplog based on the **Use HTML Dumplog** setting." },
            { "Use HTML Dumplog", "If **Use Single Dumplog** is **On**, determines if the dumplog is plain text (**Off**) or HTML (**On**)." },
            { "GZip Replay Compression", "**Off:** Zip format is used.\n**On:** GZip format is used." },
            { "Platform Render Loop", "**Off:** UI framework specific animation system is used.\n**On:** Platform-specific render loop linking to display refresh rate is used." },
            { "Runtime GL Effects", "*(Experimental)* Enables runtime GL shaders for advanced visual effects on the map." },
            { "GL Only on Map", "**Off:** Skia GL rendering is used on map, menus, and more commands page.\n**On:** Skia GL rendering is used only on map. Other pages use CPU-based rendering." },
            { "Mipmapping on Map", "Toggles whether mipmapping is used in map rendering. Mostly obsolete." },
            { "Adjust Rectangles", "Toggles whether the game adjusts tile rectangles to prevent Skia from drawing non-existing lines between tiles." },
            { "Fix Filtering", "Adjusts texture coordinates to prevent graphics filtering artifacts on some devices." },
            { "Disable Windows Key", "Prevents the Windows key from opening the Start menu, to avoid accidental focus loss during gameplay." },
            { "Default Vi-Keys", "**Off**: The default setting for the `number_pad` option is `2` (numbers for movement).\n**On**: The default setting is `0` (vi-keys for movement)." },
            { "On Switching Apps", "**Save Game**: The game is automatically saved and restored when the player returns. Menus close.\n**Checkpoint**: The game creates a checkpoint and doesn't close menus, but recovers to checkpoint if terminated." },
            { "Map GPU Cache", "Determines the cache size for Skia GPU rendering for the map. Large numbers can lead to out of memory." },
            { "Menu GPU Cache", "Determines the cache size for Skia GPU rendering for the menu and more commands pages." },
        };

        public SettingsPage(GameMenuPage gameMenuPage, MainPage mainPage)
        {
            _mainPage = mainPage;
            if(_mainPage != null)
            {
                Disappearing += (s, e) => { _mainPage.StartCarouselViewAndEnableButtons(); };
            }

            InitializeComponent();
//#if GNH_MAUI
//            SafeAreaEdges = SafeAreaEdges.All;
//#else
//            On<iOS>().SetUseSafeArea(true);
//#endif
            //UIUtils.AdjustRootLayout(RootGrid);
            //UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            //UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
            UIUtils.SetViewCursorOnHandler(TournamentLabel, GameCursorType.Info);
            UIUtils.SetViewCursorOnHandler(RecordLabel, GameCursorType.Info);
            UIUtils.SetViewCursorOnHandler(GZipLabel, GameCursorType.Info);

            _gameMenuPage = gameMenuPage;
            if (_gameMenuPage != null)
                _gamePage = _gameMenuPage._gamePage;
            else
                _gamePage = null;

            bool isDarkMode = GHApp.DarkMode;
            lblHeader.TextColor = isDarkMode ? GHColors.White : GHColors.Black;
            SetTournamentModeLabelColors(GHApp.TournamentMode);
            SetChildrenDarkModeTextColor(RootLayout, isDarkMode);
            if (isDarkMode)
            {
                PopupFrame.BackgroundColor = Color.FromRgba(0.15, 0.15, 0.15, 0.85);
            }

            MapRefreshRateStyle defaultRefresh = UIUtils.GetDefaultMapFPS();
            List<MapRefreshRateItem> list = new List<MapRefreshRateItem>
            {
                new MapRefreshRateItem("Default (" + UIUtils.GetMainCanvasAnimationFrequency(defaultRefresh) + " FPS)", true, defaultRefresh),
                new MapRefreshRateItem("20 FPS", false, MapRefreshRateStyle.MapFPS20),
                new MapRefreshRateItem("30 FPS", false, MapRefreshRateStyle.MapFPS30),
                new MapRefreshRateItem("40 FPS", false, MapRefreshRateStyle.MapFPS40),
                new MapRefreshRateItem("60 FPS", false, MapRefreshRateStyle.MapFPS60),
            };

            float screenRefreshRate = GHApp.DisplayRefreshRate;
            if (screenRefreshRate >= 72.0f)
                list.Add(new MapRefreshRateItem("72 FPS", false, MapRefreshRateStyle.MapFPS72));
            if (screenRefreshRate >= 80.0f)
                list.Add(new MapRefreshRateItem("80 FPS", false, MapRefreshRateStyle.MapFPS80));
            //if (screenRefreshRate >= 90.0f)
            //    list.Add("90 fps");
            //if (screenRefreshRate >= 120.0f)
            //    list.Add("120 fps");
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

            EngraveQuickEntry.MaxLength = GHConstants.MaxEngraveQuickTextLength;

            ScreenScalePicker.ItemsSource = GHApp.ScreenScaleItems;
            ScreenResolutionPicker.ItemsSource = GHApp.ScreenResolutionItems;
            if (!GHApp.IsWindows)
            {
                ScreenResolutionGrid.IsVisible = false;
            }
            if (!GHApp.IsPlatformRenderLoopAvailable)
            {
                PlatformRenderLoopGrid.IsVisible = false;
            }
            if (!GHApp.IsGPUAvailable || !GHConstants.EnableExperimentalFeatures)
            {
                RuntimeEffectsGrid.IsVisible = false;
            }


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

            EngraveQuickTextValidationExpression = new Regex(@"^[\x20-\x7E]{0," + GHConstants.MaxEngraveQuickTextLength + "}$");
            XlogUserNameValidationExpression = new Regex(@"^[A-Za-z0-9_]{1," + GHConstants.MaxXlogUserNameLength + "}$");
            BonesAllowedUsersValidationExpression = new Regex(@"^([A-Za-z0-9_]*[ \,]+)*([A-Za-z0-9_]*)?$");

            SetInitialValues();

            ClassicStatusBarSwitch_Toggled(null, new ToggledEventArgs(ClassicStatusBarSwitch.IsToggled));
            AllowBonesSwitch_Toggled(null, new ToggledEventArgs(AllowBonesSwitch.IsToggled));
            BonesListSwitch_Toggled(null, new ToggledEventArgs(BonesListSwitch.IsToggled));
            if(SaveFileTrackingGrid.IsVisible && !SaveFileTrackingSwitch.IsToggled)
                UpdateServerPostingEnabled(SaveFileTrackingSwitch.IsToggled);

            if (!GHApp.RecommendedSettingsChecked)
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

#if GNH_MAUI && ANDROID
            /* Workaround for ScrollView bug in .NET MAUI 9.0 GA */
            PostXlogUserNameEntry.IsEnabled = false;
            PostXlogPasswordEntry.IsEnabled = false;
            IDispatcherTimer timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
            timer.Interval = TimeSpan.FromSeconds(0.5);
            timer.IsRepeating = false;
            timer.Tick += (s, e) => 
            {
                PostXlogUserNameEntry.IsEnabled = true;
                PostXlogPasswordEntry.IsEnabled = true;
            };
            timer.Start();
#endif
            SetupSettingLabels(RootLayout);
            _isManualTogglingEnabled = true;
        }

        private void SetupSettingLabels(object container)
        {
            if (container == null) return;
            
            System.Collections.IEnumerable children = null;
#if GNH_MAUI
            if (container is Microsoft.Maui.Controls.Layout layout)
            {
                children = layout.Children;
            }
#else
            if (container is Xamarin.Forms.Layout<Xamarin.Forms.View> layout)
            {
                children = layout.Children;
            }
#endif

            if (children != null)
            {
                foreach (var child in children)
                {
                    if (child is Label label)
                    {
                        string text = label.Text?.Trim();
                        if (!string.IsNullOrEmpty(text) && _settingDescriptions.ContainsKey(text))
                        {
                            if (label.FontSize >= SaveStyleLabel.FontSize && label.FontSize <= GPULabel.FontSize)
                            {
                                UIUtils.SetViewCursorOnHandler(label, GameCursorType.Info);
                                if (!label.GestureRecognizers.Any(g => g is TapGestureRecognizer))
                                {
                                    var tapGesture = new TapGestureRecognizer();
                                    string baseFontFamily = label.FontFamily; /* We need to set since there is a bug that without it iOS sets the normal font to something else in FormattedText */
                                    tapGesture.Tapped += (s, e) =>
                                    {
                                        ShowSettingInfoPopup(text, _settingDescriptions[text], baseFontFamily);
                                    };
                                    label.GestureRecognizers.Add(tapGesture);
                                }
                            }
                        }
                    }
                    else
                    {
                        SetupSettingLabels(child);
                    }
                }
            }
#if GNH_MAUI
            else if (container is Microsoft.Maui.Controls.ScrollView scrollView)
            {
                SetupSettingLabels(scrollView.Content);
            }
            else if (container is Microsoft.Maui.Controls.ContentView contentView)
            {
                SetupSettingLabels(contentView.Content);
            }
#else
            else if (container is Xamarin.Forms.ScrollView scrollView)
            {
                SetupSettingLabels(scrollView.Content);
            }
            else if (container is Xamarin.Forms.ContentView contentView)
            {
                SetupSettingLabels(contentView.Content);
            }
#endif
        }

        private void ShowSettingInfoPopup(string title, string description, string baseFontFamily)
        {
            PopupTitleLabel.TextColor = GHColors.TitleGoldColor;
            PopupTitleLabel.Text = title;
            UIUtils.SetMarkdownText(PopupLabel, description, baseFontFamily, true); /* Popups have always a dark background */
            PopupOkButton.IsEnabled = true;
            PopupGrid.IsVisible = true;
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
                else if (darkmode ? l.TextColor == GHColors.Blue : l.TextColor == GHColors.LightBlue)
                    l.TextColor = darkmode ? GHColors.LightBlue : GHColors.Blue;
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
            Color normalColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            Color offColor = GHApp.DarkMode ? GHColors.BrighterRed : GHColors.Red;
            Color defColor = GHApp.DarkMode ? GHColors.LightPink : GHColors.Magenta;
            Color onColor = GHApp.DarkMode ? GHColors.LightGreen : GHColors.Green;
            Color usedOnColor = isTournament ? onColor : normalColor;
            Color usedOffColor = isTournament ? offColor : normalColor;
            Color usedDefColor = isTournament ? defColor : normalColor;

            TournamentLabel.TextColor = TournamentSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            RecordLabel.TextColor = RecordSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            AutoUploadReplaysLabel.TextColor = AutoUploadReplaysSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            SaveStyleLabel.TextColor = SaveStylePicker.IsEnabled ? usedDefColor : GHColors.Gray;
            PostGameStatusLabel.TextColor = PostGameStatusSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            PostXlogLabel.TextColor = PostXlogSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            PostReplaysLabel.TextColor = PostReplaysSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            PostBonesLabel.TextColor = PostBonesSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            AllowBonesLabel.TextColor = AllowBonesSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            GZipLabel.TextColor = GZipSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            SaveFileTrackingLabel.TextColor = SaveFileTrackingSwitch.IsEnabled ? usedOnColor : GHColors.Gray;
            DebugPostChannelLabel.TextColor = DebugPostChannelSwitch.IsEnabled ? usedOffColor : GHColors.Gray;

            CustomWebHookLinkTitleLabel.TextColor = usedDefColor;
            AccountTitleLabel.TextColor = usedDefColor;
            PostingTitleLabel.TextColor = usedDefColor;
            PostXlogUserNameLabel.TextColor = usedOnColor;
            PostXlogPasswordLabel.TextColor = usedOnColor;
            CloudStorageTitleLabel.TextColor = usedDefColor;
        }

        private void UpdateDefaultMapRefreshRate()
        {
            int selIdx = RefreshRatePicker.SelectedIndex;
            var source = RefreshRatePicker.ItemsSource;
            RefreshRatePicker.ItemsSource = null;
            if (source != null && source.Count > 0)
            {
                var item = source[0] as MapRefreshRateItem;
                if (item != null)
                {
#if WINDOWS
                    float refreshRate = (float)DisplaySettingsHelper.GetCurrentResolution()?.RefreshRate;
#else
                    float refreshRate = GHApp.DisplayRefreshRate;
#endif
                    item.Value = UIUtils.GetDefaultMapFPS(refreshRate, PlatformRenderLoopGrid.IsVisible && PlatformRenderLoopSwitch.IsToggled);
                    item.DisplayName = "Default (" + UIUtils.GetMainCanvasAnimationFrequency(item.Value) + " FPS)";
                }
            }
            RefreshRatePicker.ItemsSource = source;
            RefreshRatePicker.SelectedIndex = selIdx;
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

            if (GridOpacityPicker.SelectedIndex > -1 && GridOpacityPicker.SelectedItem != null)
            {
                if (_gamePage != null)
                    _gamePage.GridOpacity = GridOpacityPicker.SelectedIndex;
                Preferences.Set("GridOpacity", GridOpacityPicker.SelectedIndex);
            }

            if (RefreshRatePicker.SelectedIndex > -1 && RefreshRatePicker.SelectedItem != null && RefreshRatePicker.SelectedItem is MapRefreshRateItem)
            {
                if (_gamePage != null)
                    _gamePage.MapRefreshRate = ((MapRefreshRateItem)RefreshRatePicker.SelectedItem).Value;
                Preferences.Set("MapRefreshRate", (int)((MapRefreshRateItem)RefreshRatePicker.SelectedItem).Value);
            }

            if (ScreenScalePicker.SelectedIndex > -1 && ScreenScalePicker.SelectedItem != null && ScreenScalePicker.SelectedItem is ScreenScaleItem)
            {
                float val = ((ScreenScaleItem)ScreenScalePicker.SelectedItem).Value;
                GHApp.CustomScreenScale = val;
                Preferences.Set("CustomScreenScale", val);
            }

            if (ScreenResolutionGrid.IsVisible && ScreenResolutionPicker.SelectedIndex >= 0 && ScreenResolutionPicker.SelectedItem != null)
            {
                ScreenResolutionItem item = ScreenResolutionPicker.SelectedItem as ScreenResolutionItem;
                if (item != null)
                {
                    GHApp.CustomScreenResolutionWidth = item.Width;
                    GHApp.CustomScreenResolutionHeight = item.Height;
                    GHApp.CustomScreenResolutionRefreshRate = item.RefreshRate;
                    GHApp.CustomScreenResolutionPriority = item.ListPriority;
                    Preferences.Set("CustomScreenResolutionWidth", (int)item.Width);
                    Preferences.Set("CustomScreenResolutionHeight", (int)item.Height);
                    Preferences.Set("CustomScreenResolutionRefreshRate", (int)item.RefreshRate);
                    Preferences.Set("CustomScreenResolutionPriority", (int)item.ListPriority);
                }
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
            {
                _gamePage.WarnLowDiskSpace = WarnLowDiskSpaceSwitch.IsToggled;
                if (WarnLowDiskSpaceSwitch.IsToggled)
                    GHApp.UpdateFreeDiskSpace();
            }
            Preferences.Set("WarnLowDiskSpace", WarnLowDiskSpaceSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowZoom = ZoomSwitch.IsToggled;
            Preferences.Set("ShowZoom", ZoomSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowRecording = ShowRecordingSwitch.IsToggled;
            Preferences.Set("ShowRecording", ShowRecordingSwitch.IsToggled);

            if(MipMapGrid.IsVisible && MipMapSwitch.IsEnabled)
            {
                GHApp.UseMipMap = MipMapSwitch.IsToggled;
                if (_gamePage != null)
                    _gamePage.UseMainMipMap = MipMapSwitch.IsToggled;
                Preferences.Set("UseMainMipMap", MipMapSwitch.IsToggled);
            }

            GHApp.UseGPU = GPUSwitch.IsToggled;
            if (_gamePage != null)
                _gamePage.UseMainGLCanvas = GPUSwitch.IsToggled;
            Preferences.Set("UseMainGLCanvas", GPUSwitch.IsToggled);

            if (PlatformRenderLoopGrid.IsVisible && PlatformRenderLoopSwitch.IsEnabled)
            {
                bool usingBefore = GHApp.UsePlatformRenderLoop;
                GHApp.UsePlatformRenderLoop = PlatformRenderLoopSwitch.IsToggled;
                Preferences.Set("UsePlatformRenderLoop", PlatformRenderLoopSwitch.IsToggled);
                if (_gamePage != null)
                    _gamePage.PlatformRenderLoopToggled = true;
                if (PlatformRenderLoopSwitch.IsToggled != usingBefore)
                {
                    if (GHApp.IsAndroid)
                    {
                        if (PlatformRenderLoopSwitch.IsToggled)
                            GHApp.PlatformService.RevertAnimatorDuration(false);
                        else
                            GHApp.PlatformService.OverrideAnimatorDuration();
                    }
                    if (!PlatformRenderLoopSwitch.IsToggled)
                        GHApp.StopRenderingStopwatch();
                }
            }

            GHApp.DisableAuxGPU = DisableAuxGPUSwitch.IsToggled;
            Preferences.Set("DisableAuxiliaryGLCanvas", DisableAuxGPUSwitch.IsToggled);
            bool useAuxGPU = GPUSwitch.IsToggled && !DisableAuxGPUSwitch.IsToggled;
            GHApp.UseAuxGPU = useAuxGPU;
            if (_gamePage != null)
                _gamePage.UseAuxiliaryGLCanvas = useAuxGPU;
            Preferences.Set("UseAuxiliaryGLCanvas", useAuxGPU);

            GHApp.AllowBones = AllowBonesSwitch.IsToggled;
            Preferences.Set("AllowBones", AllowBonesSwitch.IsToggled);
            //GHApp.AllowPet = AllowPetSwitch.IsToggled; //Use MirroredPetsNotGifted below instead
            //Preferences.Set("AllowPet", AllowPetSwitch.IsToggled);

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
            if (FixVerticalSwitch.IsEnabled)
            {
                GHApp.FixFiltering = FixVerticalSwitch.IsToggled;
                Preferences.Set("FixFiltering", FixVerticalSwitch.IsToggled);
            }
            if (RuntimeEffectsSwitch.IsEnabled)
            {
                GHApp.RuntimeEffects = RuntimeEffectsSwitch.IsToggled;
                Preferences.Set("RuntimeEffects", RuntimeEffectsSwitch.IsToggled);
            }

            GHApp.EmptyWishIsNothing = EmptyWishIsNothingSwitch.IsToggled;
            Preferences.Set("EmptyWishIsNothing", EmptyWishIsNothingSwitch.IsToggled);

            /* There's an iflag but not a corresponding option; iflag just need to be set always to the same value as here */
            GHApp.GetPositionArrows = GetPositionArrowsSwitch.IsToggled;
            Preferences.Set("GetPositionArrows", GetPositionArrowsSwitch.IsToggled);
            if (_gamePage != null)
                _gamePage.SetGetPositionArrows(GetPositionArrowsSwitch.IsToggled);

            /* This is flags, saved in save file */
            GHApp.MirroredCharacterClickAction = CharacterClickActionSwitch.IsToggled;
            if (_gamePage != null) /* During game only doubles as the option; outside of game sets the default */
                _gamePage.SetCharacterClickAction(CharacterClickActionSwitch.IsToggled);
            else
                Preferences.Set("CharacterClickAction", CharacterClickActionSwitch.IsToggled);

            /* This is flags, saved in save file */
            GHApp.MirroredMetricSystem = MetricSystemSwitch.IsToggled;
            if (_gamePage != null) /* During game only doubles as the option; outside of game sets the default */
                _gamePage.SetMetricSystem(MetricSystemSwitch.IsToggled);
            else
                Preferences.Set("MetricSystem", MetricSystemSwitch.IsToggled);

            /* There's an iflag but not a corresponding option; iflag just need to be set always to the same value as here */
            GHApp.MirroredDiceAsRanges = DiceAsRangesSwitch.IsToggled;
            Preferences.Set("DiceAsRanges", DiceAsRangesSwitch.IsToggled);
            if (_gamePage != null)
                _gamePage.SetDiceAsRanges(DiceAsRangesSwitch.IsToggled);

            GHApp.MirroredDamageFormula = DamageFormulaSwitch.IsToggled;
            Preferences.Set("DamageFormula", DamageFormulaSwitch.IsToggled);
            if (_gamePage != null)
                _gamePage.SetDamageFormula(DamageFormulaSwitch.IsToggled);

            /* There's an iflag but not a corresponding option; iflag just need to be set always to the same value as here */
            GHApp.MirroredWornShowsEquipment = WornShowsEquipmentSwitch.IsToggled;
            Preferences.Set("WornShowsEquipment", WornShowsEquipmentSwitch.IsToggled);
            if (_gamePage != null)
                _gamePage.SetWornShowsEquipment(WornShowsEquipmentSwitch.IsToggled);

            /* This is flags, saved in save file */
            GHApp.MirroredPetsNotGifted = !AllowPetSwitch.IsToggled;
            if (_gamePage != null) /* During game only doubles as the option; outside of game sets the default */
                _gamePage.SetNoPetsPreference(!AllowPetSwitch.IsToggled);
            else
                Preferences.Set("AllowPet", AllowPetSwitch.IsToggled);

            /* This is flags, saved in save file */
            GHApp.MirroredAutoDig = AutoDigSwitch.IsToggled;
            if (_gamePage != null) /* During game only doubles as the option; outside of game sets the default */
                _gamePage.SetAutoDig(AutoDigSwitch.IsToggled);
            else
                Preferences.Set("AutoDig", AutoDigSwitch.IsToggled);

            /* This is flags, saved in save file */
            GHApp.MirroredIgnoreStopping = IgnoreStoppingSwitch.IsToggled;
            if (_gamePage != null) /* During game only doubles as the option; outside of game sets the default */
                _gamePage.SetIgnoreStopping(IgnoreStoppingSwitch.IsToggled);
            else
                Preferences.Set("IgnoreStopping", IgnoreStoppingSwitch.IsToggled);

            if (RightMouseButtonGrid.IsVisible)
            {
                if (RightMousePicker.SelectedIndex > -1 && RightMousePicker.SelectedItem != null && RightMousePicker.SelectedItem is MouseCommandItem)
                {
                    GHApp.MirroredRightMouseCommand = ((MouseCommandItem)RightMousePicker.SelectedItem).Value;
                    if (_gamePage != null)  /* During game only doubles as the option; outside of game sets the default */
                        _gamePage.SetRightMouseCommand(((MouseCommandItem)RightMousePicker.SelectedItem).Value);
                    else
                        Preferences.Set("RightMouseCommand", ((MouseCommandItem)RightMousePicker.SelectedItem).Value);
                }
            }

            if (MiddleMouseButtonGrid.IsVisible)
            {
                if (MiddleMousePicker.SelectedIndex > -1 && MiddleMousePicker.SelectedItem != null && MiddleMousePicker.SelectedItem is MouseCommandItem)
                {
                    GHApp.MirroredMiddleMouseCommand = ((MouseCommandItem)MiddleMousePicker.SelectedItem).Value;
                    if (_gamePage != null)  /* During game only doubles as the option; outside of game sets the default */
                        _gamePage.SetMiddleMouseCommand(((MouseCommandItem)MiddleMousePicker.SelectedItem).Value);
                    else
                        Preferences.Set("MiddleMouseCommand", ((MouseCommandItem)MiddleMousePicker.SelectedItem).Value);
                }
            }

            GHApp.MirroredEngraveQuickText = EngraveQuickEntry.Text;
            Preferences.Set("EngraveQuickText", EngraveQuickEntry.Text);
            if (_gameMenuPage != null)
                _gamePage.SetEngraveQuickText(EngraveQuickEntry.Text);

            if (EngraveQuickPicker.SelectedIndex >= 0)
            {
                GHApp.MirroredEngraveQuickStyle = EngraveQuickPicker.SelectedIndex;
                Preferences.Set("EngraveQuickStyle", EngraveQuickPicker.SelectedIndex);
                if (_gameMenuPage != null)
                    _gamePage.SetEngraveQuickStyle(EngraveQuickPicker.SelectedIndex);
            }

            GHApp.OkOnDoubleClick = DoubleClickSwitch.IsToggled;
            Preferences.Set("OkOnDoubleClick", DoubleClickSwitch.IsToggled);

            GHApp.PostingGameStatus = PostGameStatusSwitch.IsToggled;
            Preferences.Set("PostingGameStatus", PostGameStatusSwitch.IsToggled);
#if !SENTRY
            GHApp.PostingDiagnosticData = PostDiagnosticDataSwitch.IsToggled;
            Preferences.Set("PostingDiagnosticData", PostDiagnosticDataSwitch.IsToggled);
#endif
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
            if (SaveFileTrackingGrid.IsVisible)
            {
                GHApp.SaveFileTracking = SaveFileTrackingSwitch.IsToggled;
                Preferences.Set("SaveFileTracking", SaveFileTrackingSwitch.IsToggled);
            }

            GHApp.CustomCloudStorageConnectionString = _customCloudStorageConnectionString;
            Preferences.Set("CustomCloudStorageConnectionString", _customCloudStorageConnectionString);

            GHApp.XlogCredentialsIncorrect = false;
            if (!GHApp.AreCredentialsVerified(PostXlogUserNameEntry.Text, PostXlogPasswordEntry.Text))
                GHApp.SetXlogUserNameVerified(false, null, null);

            if (!GHApp.XlogUserNameVerified && !string.IsNullOrEmpty(GHApp.XlogUserName))
            {
                PopupTitleLabel.TextColor = GHColors.TitleGoldColor;
                PopupTitleLabel.Text = "Credentials Verification";
                PopupLabel.Text = "Verifying credentials... Please wait.";
                PopupOkButton.IsVisible = false;
                Thickness oldThickness = PopupFrame.Padding;
                PopupFrame.Padding = new Thickness(12, 18, 12, 30);
                PopupGrid.IsVisible = true;
                await GHApp.TryVerifyXlogUserNameAsync(true);
                if (!GHApp.XlogUserNameVerified)
                {
                    PopupTitleLabel.TextColor = GHColors.BrighterRed;
                    PopupTitleLabel.Text = "Login Failed";
                    PopupLabel.Text = "Logging in to " + (GHApp.IsDebug && !GHApp.XlogReleaseAccount ? "Test " : "") + "GnollHack Server failed.";
                    await Task.Delay(500);
                }
                else
                {
                    PopupTitleLabel.TextColor = GHColors.BrighterGreen;
                    PopupTitleLabel.Text = "Login Success";
                    PopupLabel.Text = "Logging in to " + (GHApp.IsDebug && !GHApp.XlogReleaseAccount ? "Test " : "") + "GnollHack Server was successful.";
                    await Task.Delay(350);
                }
                PopupTitleLabel.TextColor = GHColors.TitleGoldColor;
                PopupOkButton.IsVisible = true;
                PopupFrame.Padding = oldThickness;
                PopupGrid.IsVisible = false;
            }

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
                _gamePage.MenuHighlightedKeys = MenuHighlightedKeysSwitch.IsToggled;
            Preferences.Set("MenuHighlightedKeys", MenuHighlightedKeysSwitch.IsToggled);

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

            GHApp.DrawWallEnds = WallEndSwitch.IsToggled;
            if (_gamePage != null)
                _gamePage.DrawWallEnds = WallEndSwitch.IsToggled;
            Preferences.Set("DrawWallEnds", WallEndSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.BreatheAnimations = BreatheAnimationSwitch.IsToggled;
            Preferences.Set("BreatheAnimations", BreatheAnimationSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.LongerMessageHistory = LongerMessageHistorySwitch.IsToggled;
            GHApp.SavedLongerMessageHistory = LongerMessageHistorySwitch.IsToggled;

            if (_gamePage != null)
                _gamePage.HideMessageHistory = HideMessageHistorySwitch.IsToggled;
            GHApp.SavedHideMessageHistory = HideMessageHistorySwitch.IsToggled;

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
            GHApp.LowLevelLogging = LowLevelLogSwitch.IsToggled;
            Preferences.Set("LowLevelLogging", GHApp.LowLevelLogging);
            GHApp.ScreenLogging = ScreenLogSwitch.IsToggled;
            Preferences.Set("ScreenLogging", GHApp.ScreenLogging);
            GHApp.DebugPostChannel = DebugPostChannelSwitch.IsToggled;
            Preferences.Set("DebugPostChannel", GHApp.DebugPostChannel);
            GHApp.TournamentMode = TournamentSwitch.IsToggled;
            Preferences.Set("TournamentMode", GHApp.TournamentMode);

            Preferences.Set("DefaultMapNoClipMode", !YesClipNormalSwitch.IsToggled);

            if (DisableWindowsKeyGrid.IsVisible && DisableWindowsKeySwitch.IsEnabled)
            {
                GHApp.DisableWindowsKey = DisableWindowsKeySwitch.IsToggled;
                Preferences.Set("DisableWindowsKey", DisableWindowsKeySwitch.IsToggled);
            }

            GHApp.DefaultVIKeys = DefaultVIKeysSwitch.IsToggled;
            Preferences.Set("DefaultVIKeys", DefaultVIKeysSwitch.IsToggled);

            GHApp.ShowKeyboardShortcuts = ShowKeyboardShortcutsSwitch.IsToggled;
            Preferences.Set("ShowKeyboardShortcuts", ShowKeyboardShortcutsSwitch.IsToggled);
            
            GHApp.UseSingleMoreCommandsPage = UseSingleMoreCommandsPageSwitch.IsToggled;
            Preferences.Set("UseSingleMoreCommandsPage", UseSingleMoreCommandsPageSwitch.IsToggled);

            GHApp.ShowSkillContextButton = ShowSkillContextButtonSwitch.IsToggled;
            Preferences.Set("ShowSkillContextButton", ShowSkillContextButtonSwitch.IsToggled);

            GHApp.ShowPolearmContextButton = ShowPolearmContextButtonSwitch.IsToggled;
            Preferences.Set("ShowPolearmContextButton", ShowPolearmContextButtonSwitch.IsToggled);

            GHApp.EquipmentFlipAnimation = EquipmentFlipAnimationSwitch.IsToggled;
            Preferences.Set("EquipmentFlipAnimation", EquipmentFlipAnimationSwitch.IsToggled);

            GHApp.ShowEquipmentIcons = ShowEquipmentIconsSwitch.IsToggled;
            Preferences.Set("ShowEquipmentIcons", ShowEquipmentIconsSwitch.IsToggled);

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
                    Preferences.Set(keystr, GHApp.SelectableShortcutButtons[targetPicker.SelectedIndex].GHCommand);
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
                    Preferences.Set(keystr, GHApp.SelectableShortcutButtons[targetPicker.SelectedIndex].GHCommand);
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

            if (_gamePage != null)
                _gamePage.ShowAltZoomButton = ShowAltZoomButtonSwitch.IsToggled;
            Preferences.Set("ShowAltZoomButton", ShowAltZoomButtonSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowTravelModeButton = ShowTravelModeButtonSwitch.IsToggled;
            Preferences.Set("ShowTravelModeButton", ShowTravelModeButtonSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowAutoDigButton = ShowAutoDigButtonSwitch.IsToggled;
            Preferences.Set("ShowAutoDigButton", ShowAutoDigButtonSwitch.IsToggled);

            if (_gamePage != null)
                _gamePage.ShowIgnoreStoppingButton = ShowIgnoreStoppingButtonSwitch.IsToggled;
            Preferences.Set("ShowIgnoreStoppingButton", ShowIgnoreStoppingButtonSwitch.IsToggled);

            bool oldDarkMode = GHApp.DarkMode;
            Preferences.Set("DarkMode", DarkModeSwitch.IsToggled);
            if (oldDarkMode != DarkModeSwitch.IsToggled)
            {
                GHApp.DarkMode = DarkModeSwitch.IsToggled;
                UpdateDarkModeOnAllPages(DarkModeSwitch.IsToggled);
            }

            GHApp.SilentMode = SilentModeSwitch.IsToggled;
            Preferences.Set("SilentMode", GHApp.SilentMode);

            if(WindowedModeStackLayout.IsVisible && WindowedModeSwitch.IsEnabled)
            {
                /* Do not update GHApp.WindowedMode, because it needs a restart */
                Preferences.Set("WindowedMode", WindowedModeSwitch.IsToggled);
            }

            if (Edge2EdgeStackLayout.IsVisible && Edge2EdgeSwitch.IsEnabled)
            {
                /* Do not update GHApp.Edge2Edge, because it needs a restart */
                Preferences.Set("Edge2Edge", Edge2EdgeSwitch.IsToggled);
                //UpdateEdge2EdgeOnAllPages(Edge2EdgeSwitch.IsToggled);
            }


            float generalVolume = (float)GeneralVolumeSlider.Value;
            float musicVolume = (float)MusicVolumeSlider.Value;
            float ambientVolume = (float)AmbientVolumeSlider.Value;
            float dialogueVolume = (float)DialogueVolumeSlider.Value;
            float effectsVolume = (float)EffectsVolumeSlider.Value;
            float uiVolume = (float)UIVolumeSlider.Value;

            Preferences.Set("GeneralVolume", generalVolume);
            Preferences.Set("MusicVolume", musicVolume);
            Preferences.Set("AmbientVolume", ambientVolume);
            Preferences.Set("DialogueVolume", dialogueVolume);
            Preferences.Set("EffectsVolume", effectsVolume);
            Preferences.Set("UIVolume", uiVolume);

            ////if (!GHApp.IsMuted)
            //    GHApp.FmodService?.AdjustVolumes((float)GeneralVolumeSlider.Value, (float)MusicVolumeSlider.Value, (float)AmbientVolumeSlider.Value, (float)DialogueVolumeSlider.Value, (float)EffectsVolumeSlider.Value, (float)UIVolumeSlider.Value);

            /* Adjust first UI volumes */
            GHApp.FmodService?.AdjustUIVolumes(generalVolume, musicVolume, uiVolume);
            GHGame curGame = GHApp.CurrentGHGame;
            if (curGame != null)
            {
                /* Game is ongoing, so ask GHGame to handle this */
                curGame.ResponseQueue.Enqueue(new GHResponse(curGame, GHRequestType.SetVolume, generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, uiVolume));
            }
            else
            {
                /* Adjust directly, since there's no game thread */
                GHApp.FmodService?.AdjustGameVolumes(generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, uiVolume);
            }

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

            if (unloadbanks && GHApp.FmodService != null)
            {
                try
                {
                    GHApp.FmodService.ReleaseAllUISoundInstances();
                    GHApp.FmodService.ReleaseAllGameSoundInstances(); /* Needs to be done immediately, so hopefully no conflict with the game thread */
                    GHApp.FmodService.ResetGameState();
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
                    await GHApp.GnollHackService.InitializeFilesInSettings(GHApp.CurrentSettings);
                }
                catch (Exception ex)
                {
                    await GHApp.DisplayMessageBox(this, "Secrets Initialization Failed", "Initializing secrets failed: " + ex.Message, "OK");
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
                        GHApp.FmodService.PlayUIMusic(GHConstants.IntroGHSound, GHConstants.IntroEventPath, GHConstants.IntroBankId, GHConstants.IntroMusicVolume, 1.0f);
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
                await _mainPage.StartGeneralTimerAsync(); /* Just in case something's changed */
            }

            if (_gameMenuPage != null)
                _gameMenuPage.UpdateLayout();

            if(_gamePage != null)
                _gamePage.UpdateButtonAndUISizes();
        }

        private void SetInitialValues()
        {
            int cursor = 0, graphics = 0, gridopacity = 0, savestyle = 0, maprefresh = -1, msgnum = 0, petrows = 0;
            bool mem = false, fps = false, zoom = false, battery = false, lowdiskspace = true, showrecording = true, autoupload = false, gpu = GHApp.IsGPUDefault, disableauxgpu = false, platformloop = false, mipmap = false, simplecmdlayout = GHConstants.DefaultSimpleCmdLayout, showaltzoom = !GHConstants.DefaultSimpleCmdLayout, showtravelmode = !GHConstants.DefaultSimpleCmdLayout, showautodig = false, showignore = false, darkmode = false, windowedmode = false, edge2edge = false, bank = true, navbar = GHConstants.DefaultHideNavigation, statusbar = GHConstants.DefaultHideStatusBar;
            bool allowbones = true, allowpet = true, emptywishisnothing = true, doubleclick = GHApp.IsDesktop, getpositionarrows = false, recordgame = false, gzip = GHConstants.GZipIsDefaultReplayCompression, lighterdarkening = false, accuratedrawing = GHConstants.DefaultAlternativeLayerDrawing, html = GHConstants.DefaultHTMLDumpLogs, singledumplog = GHConstants.DefaultUseSingleDumpLog, streamingbanktomemory = false, streamingbanktodisk = false, wallends = GHConstants.DefaultDrawWallEnds;
            bool breatheanimations = GHConstants.DefaultBreatheAnimations; //, put2bag = GHConstants.DefaultShowPickNStashContextCommand, prevwep = GHConstants.DefaultShowPrevWepContextCommand;
            bool devmode = GHConstants.DefaultDeveloperMode, logmessages = GHConstants.DefaultLogMessages, lowlevellogging = false, screenlogging = false, debugpostchannel = GHConstants.DefaultDebugPostChannel, tournament = false, hpbars = false, nhstatusbarclassic = GHConstants.IsDefaultStatusBarClassic, desktopstatusbar = false, rightaligned2ndrow = false, showscore = false, showxp = false, desktopbuttons = false, menufadeeffects = false, menuhighfilterquality = true, menuhighlightedkeys = false, pets = true, orbs = true, orbmaxhp = false, orbmaxmana = false, mapgrid = false, playermark = false, monstertargeting = false, walkarrows = true;
            bool forcemaxmsg = false, showexstatus = false, noclipmode = GHConstants.DefaultMapNoClipMode, silentmode = false, characterclickaction = false, metricsystem = false, diceasranges = true, damageformula = false, wornshowsequipment = true, autodig = false, ignorestopping = false;
            bool postgamestatus = GHConstants.DefaultPosting, postxlog = GHConstants.DefaultPosting, postreplays = GHConstants.DefaultPosting, postbones = GHConstants.DefaultPosting, boneslistisblack = false, showkeyboardshortcuts = false, singlecmdpage = false, skillbutton = false, polearmbutton = false, equipmentflipanimation = true, showequipmenticons = true;
#if !SENTRY
            bool postdiagnostics = GHConstants.DefaultPosting;
#endif
            bool longermsghistory = false, hidemsghistory = false, xlog_release_account = false, forcepostbones = false, fixrects = false, fixvertical = true, runtimeeffects = GHConstants.DefaultRuntimeEffects, save_file_tracking = false, disablewindowskey = false, defaultvikeys = false;
            bool experimental = GHConstants.EnableExperimentalFeatures;
            long primarygpucache = -2, secondarygpucache = -2;
            int rightmouse = GHConstants.DefaultRightMouseCommand, middlemouse = GHConstants.DefaultMiddleMouseCommand;
            float screenscale = 0.0f;
            uint screenresolutionwidth = 0, screenresolutionheight = 0, screenresolutionrefreshrate = 0, screenresolutionpriority = 1;
            float generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, UIVolume;
            string customlink = "";
            string customxlogaccountlink = "";
            string customxlogpostlink = "";
            string customcloudstorage = "";
            string xlog_username = "";
            string xlog_password = "";
            string bones_allowed_users = "";
            string engravequick = "";
            int engraveqstyle = 0;

            int[] cmdidxs = new int[6];
            for (int i = 0; i < 6; i++)
            {
                string keystr = "SimpleUILayoutCommandButton" + (i + 1);
                int defCmd = GHApp.DefaultShortcutButton(0, i, true).GHCommand;
                int savedCmd = Preferences.Get(keystr, defCmd);
                int listselidx = GHApp.SelectableShortcutButtonIndexInList(savedCmd, defCmd);
                cmdidxs[i] = listselidx;
            }

            int[] fullCmdIdxs = new int[13];
            for (int i = 0; i < 13; i++)
            {
                string keystr = "FullUILayoutCommandButton" + (i + 1);
                int defCmd = GHApp.DefaultShortcutButton(0, i, false).GHCommand;
                int savedCmd = Preferences.Get(keystr, defCmd);
                int listselidx = GHApp.SelectableShortcutButtonIndexInList(savedCmd, defCmd);
                fullCmdIdxs[i] = listselidx;
            }

            darkmode = GHApp.DarkMode; // Preferences.Get("DarkMode", false);
            windowedmode = Preferences.Get("WindowedMode", false);
            edge2edge = Preferences.Get("Edge2Edge", false);
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
            lowlevellogging = GHApp.LowLevelLogging;
            screenlogging = GHApp.ScreenLogging;
            debugpostchannel = GHApp.DebugPostChannel;
            tournament = GHApp.TournamentMode;
            bank = Preferences.Get("LoadSoundBanks", true);
            html = Preferences.Get("UseHTMLDumpLogs", GHConstants.DefaultHTMLDumpLogs);
            singledumplog = Preferences.Get("UseSingleDumpLog", GHConstants.DefaultUseSingleDumpLog);
            streamingbanktomemory = Preferences.Get("ReadStreamingBankToMemory", GHApp.DefaultStreamingBankToMemory);
            streamingbanktodisk = Preferences.Get("CopyStreamingBankToDisk", GHConstants.DefaultCopyStreamingBankToDisk);
            postgamestatus = Preferences.Get("PostingGameStatus", GHConstants.DefaultPosting);
#if !SENTRY
            postdiagnostics = Preferences.Get("PostingDiagnosticData", GHConstants.DefaultPosting);
#endif
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
            fixvertical = Preferences.Get("FixFiltering", GHApp.IsFixFilteringDefault);
            runtimeeffects = Preferences.Get("RuntimeEffects", GHConstants.DefaultRuntimeEffects);
            noclipmode = Preferences.Get("DefaultMapNoClipMode", GHConstants.DefaultMapNoClipMode);
            savestyle = Preferences.Get("AppSwitchSaveStyle", GHApp.IsDesktop ? 1 : 0);
            primarygpucache = Preferences.Get("PrimaryGPUCacheLimit", -2L);
            secondarygpucache = Preferences.Get("SecondaryGPUCacheLimit", -2L);
            disableauxgpu = Preferences.Get("DisableAuxiliaryGLCanvas", GHApp.IsDisableAuxGPUDefault);
            platformloop = GHApp.IsPlatformRenderLoopAvailable && Preferences.Get("UsePlatformRenderLoop", GHConstants.IsPlatformRenderLoopDefault);
            screenscale = Preferences.Get("CustomScreenScale", 0.0f);
            screenresolutionwidth = (uint)Preferences.Get("CustomScreenResolutionWidth", 0);
            screenresolutionheight = (uint)Preferences.Get("CustomScreenResolutionHeight", 0);
            screenresolutionrefreshrate = (uint)Preferences.Get("CustomScreenResolutionRefreshRate", 0);
            screenresolutionpriority = (uint)Preferences.Get("CustomScreenResolutionPriority", 1);
            save_file_tracking = GHApp.SaveFileTracking;
            disablewindowskey = Preferences.Get("DisableWindowsKey", false);
            defaultvikeys = Preferences.Get("DefaultVIKeys", false);
            showkeyboardshortcuts = Preferences.Get("ShowKeyboardShortcuts", GHApp.IsDesktop);
            singlecmdpage = Preferences.Get("UseSingleMoreCommandsPage", GHApp.IsDesktop);
            skillbutton = Preferences.Get("ShowSkillContextButton", true);
            polearmbutton = Preferences.Get("ShowPolearmContextButton", GHConstants.DefaultShowPolearmContextButton);
            equipmentflipanimation = Preferences.Get("EquipmentFlipAnimation", true);
            showequipmenticons = Preferences.Get("ShowEquipmentIcons", true);
            maprefresh = Preferences.Get("MapRefreshRate", -1);
            if (_gamePage == null)
            {
                cursor = Preferences.Get("CursorStyle", 1);
                graphics = Preferences.Get("GraphicsStyle", 1);
                mapgrid = Preferences.Get("MapGrid", false);
                gridopacity = Preferences.Get("GridOpacity", 0);
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
                menuhighfilterquality = Preferences.Get("MenuHighFilterQuality", GHApp.IsMenuHighFilterQualityDefault);
                menuhighlightedkeys = Preferences.Get("MenuHighlightedKeys", GHApp.IsMenuHighlightedKeysDefault);
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
                lowdiskspace = Preferences.Get("WarnLowDiskSpace", true);
                showrecording = Preferences.Get("ShowRecording", true);
                autoupload = Preferences.Get("AutoUploadReplays", false);
                mipmap = Preferences.Get("UseMainMipMap", GHApp.IsUseMainMipMapDefault);
                gpu = Preferences.Get("UseMainGLCanvas", GHApp.IsUseMainGPUDefault);
                simplecmdlayout = Preferences.Get("UseSimpleCmdLayout", GHConstants.DefaultSimpleCmdLayout);
                showaltzoom = Preferences.Get("ShowAltZoomButton", !simplecmdlayout);
                showtravelmode = Preferences.Get("ShowTravelModeButton", !simplecmdlayout);
                showautodig = Preferences.Get("ShowAutoDigButton", false);
                showignore = Preferences.Get("ShowIgnoreStoppingButton", false);
                msgnum = Preferences.Get("NumDisplayedMessages", GHConstants.DefaultMessageRows);
                petrows = Preferences.Get("NumDisplayedPetRows", GHConstants.DefaultPetRows);
                lighterdarkening = Preferences.Get("LighterDarkening", GHConstants.DefaultLighterDarkening);
                accuratedrawing = Preferences.Get("AlternativeLayerDrawing", GHConstants.DefaultAlternativeLayerDrawing);
                wallends = Preferences.Get("DrawWallEnds", GHConstants.DefaultDrawWallEnds);
                breatheanimations = Preferences.Get("BreatheAnimations", GHConstants.DefaultBreatheAnimations);
                //put2bag = Preferences.Get("ShowPut2BagContextCommand", GHConstants.DefaultShowPickNStashContextCommand);
                //prevwep = Preferences.Get("ShowPrevWepContextCommand", GHConstants.DefaultShowPrevWepContextCommand);
                longermsghistory = GHApp.SavedLongerMessageHistory; // Preferences.Get("LongerMessageHistory", false);
                hidemsghistory = GHApp.SavedHideMessageHistory; // Preferences.Get("HideMessageHistory", false);
                getpositionarrows = Preferences.Get("GetPositionArrows", false);
                characterclickaction = Preferences.Get("CharacterClickAction", GHConstants.DefaultCharacterClickAction); /* Default value */
                metricsystem = Preferences.Get("MetricSystem", GHConstants.DefaultMetricSystem); /* Default value */
                diceasranges = Preferences.Get("DiceAsRanges", GHConstants.DefaultDiceAsRanges); /* Default value */
                damageformula = Preferences.Get("DamageFormula", GHConstants.DefaultDamageFormula); /* Default value */
                wornshowsequipment = Preferences.Get("WornShowsEquipment", GHConstants.DefaultWornShowsEquipment); /* Default value */
                autodig = Preferences.Get("AutoDig", GHConstants.DefaultAutoDig); /* Default value */
                ignorestopping = Preferences.Get("IgnoreStopping", GHConstants.DefaultIgnoreStopping); /* Default value */
                allowpet = Preferences.Get("AllowPet", true);
                rightmouse = Preferences.Get("RightMouseCommand", GHConstants.DefaultRightMouseCommand);
                middlemouse = Preferences.Get("MiddleMouseCommand", GHConstants.DefaultMiddleMouseCommand);
                engravequick = Preferences.Get("EngraveQuickText", "");
                engraveqstyle = Preferences.Get("EngraveQuickStyle", 0);
            }
            else
            {
                cursor = (int)_gamePage.CursorStyle;
                graphics = (int)_gamePage.GraphicsStyle;
                gridopacity = (int)_gamePage.GridOpacity;
                //maprefresh = (int)_gamePage.MapRefreshRate;
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
                menuhighlightedkeys = _gamePage.MenuHighlightedKeys;
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
                lowdiskspace = _gamePage.WarnLowDiskSpace;
                showrecording = _gamePage.ShowRecording;
                mipmap = _gamePage.UseMainMipMap;
                gpu = _gamePage.UseMainGLCanvas;
                simplecmdlayout = _gamePage.UseSimpleCmdLayout;
                showaltzoom = _gamePage.ShowAltZoomButton;
                showtravelmode = _gamePage.ShowTravelModeButton;
                showautodig = _gamePage.ShowAutoDigButton;
                showignore = _gamePage.ShowIgnoreStoppingButton;
                msgnum = _gamePage.NumDisplayedMessages;
                petrows = _gamePage.NumDisplayedPetRows;
                lighterdarkening = _gamePage.LighterDarkening;
                accuratedrawing = _gamePage.AlternativeLayerDrawing;
                wallends = _gamePage.DrawWallEnds;
                breatheanimations = _gamePage.BreatheAnimations;
                //put2bag = _gamePage.ShowPut2BagContextCommand;
                //prevwep = _gamePage.ShowPrevWepContextCommand;
                longermsghistory = _gamePage.LongerMessageHistory;
                hidemsghistory = _gamePage.HideMessageHistory;
                getpositionarrows = GHApp.GetPositionArrows; /* Not mirrored, but there is an iflag */
                characterclickaction = GHApp.MirroredCharacterClickAction; // _gamePage.GetCharacterClickAction(); /* Value of the option in the (saved) game */
                metricsystem = GHApp.MirroredMetricSystem;
                diceasranges = GHApp.MirroredDiceAsRanges;
                damageformula = GHApp.MirroredDamageFormula;
                wornshowsequipment = GHApp.MirroredWornShowsEquipment;
                autodig = GHApp.MirroredAutoDig;
                ignorestopping = GHApp.MirroredIgnoreStopping;
                allowpet = GHApp.AllowPet;
                rightmouse = GHApp.MirroredRightMouseCommand; //_gamePage.GetRightMouseCommand();
                middlemouse = GHApp.MirroredMiddleMouseCommand; //_gamePage.GetMiddleMouseCommand();
                engravequick = GHApp.MirroredEngraveQuickText;
                engraveqstyle = GHApp.MirroredEngraveQuickStyle;
            }

            CursorPicker.SelectedIndex = cursor;
            GraphicsPicker.SelectedIndex = graphics;
            GridOpacityPicker.SelectedIndex = gridopacity;
            //RefreshRatePicker.SelectedIndex = Math.Min(RefreshRatePicker.Items.Count - 1, maprefresh);
            if (RefreshRatePicker.ItemsSource != null)
            {
                for (int i = 0; i < RefreshRatePicker.ItemsSource.Count; i++)
                {
                    object o = RefreshRatePicker.ItemsSource[i];
                    if (o is MapRefreshRateItem)
                    {
                        if (maprefresh == -1 ? ((MapRefreshRateItem)o).IsDefault : (int)((MapRefreshRateItem)o).Value == maprefresh)
                        {
                            RefreshRatePicker.SelectedIndex = i;
                            break;
                        }
                    }
                }
            }

            if (ScreenScalePicker.ItemsSource != null)
            {
                for (int i = 0; i < ScreenScalePicker.ItemsSource.Count; i++)
                {
                    object o = ScreenScalePicker.ItemsSource[i];
                    if (o is ScreenScaleItem)
                    {
                        if (((ScreenScaleItem)o).Value == screenscale)
                        {
                            ScreenScalePicker.SelectedIndex = i;
                            break;
                        }
                    }
                }
            }

            if (ScreenResolutionGrid.IsVisible && ScreenResolutionPicker.ItemsSource != null && ScreenResolutionPicker.ItemsSource.Count > 0)
            {
                if (ScreenResolutionPicker.ItemsSource.Count == 1 || screenresolutionpriority == 2)
                {
                    ScreenResolutionPicker.SelectedIndex = 0;
                }
                else if (GHApp.RecommendedScreenResolution != null && screenresolutionpriority == 1 && ScreenResolutionPicker.ItemsSource.Count >= 2 && (ScreenResolutionPicker.ItemsSource[1] as ScreenResolutionItem)?.ListPriority == 1)
                {
                    ScreenResolutionPicker.SelectedIndex = 1;
                }
                else
                {
                    for (int i = 0; i < ScreenResolutionPicker.ItemsSource.Count; i++)
                    {
                        ScreenResolutionItem item = ScreenResolutionPicker.ItemsSource[i] as ScreenResolutionItem;
                        if (item != null)
                        {
                            if (item.Width == screenresolutionwidth && item.Height == screenresolutionheight && item.RefreshRate == screenresolutionrefreshrate)
                            {
                                ScreenResolutionPicker.SelectedIndex = i;
                                break;
                            }
                        }
                    }
                    if (ScreenResolutionPicker.SelectedIndex < 0)
                        ScreenResolutionPicker.SelectedIndex = 0;
                }
            }

            SaveStylePicker.SelectedIndex = savestyle;
            GridSwitch.IsToggled = mapgrid;
            HitPointBarSwitch.IsToggled = hpbars;
            ClassicStatusBarSwitch.IsToggled = nhstatusbarclassic;
            DesktopStatusBarSwitch.IsToggled = desktopstatusbar;
            ShowScoreSwitch.IsToggled = showscore;
            ShowXPSwitch.IsToggled = showxp;
            RightAligned2ndRowSwitch.IsToggled = rightaligned2ndrow;
            DesktopButtonsSwitch.IsToggled = desktopbuttons;
            if (GHApp.IsiOS || GHApp.IsWindows)
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
            MenuHighlightedKeysSwitch.IsToggled = menuhighlightedkeys;
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
            WarnLowDiskSpaceSwitch.IsToggled = lowdiskspace;
            ShowRecordingSwitch.IsToggled = showrecording;
            AutoUploadReplaysSwitch.IsToggled = autoupload;
            DisableAuxGPUSwitch.IsToggled = disableauxgpu;
            RuntimeEffectsSwitch.IsToggled = runtimeeffects;
            PlatformRenderLoopSwitch.IsToggled = platformloop;
            FixRectsSwitch.IsToggled = fixrects;
            FixVerticalSwitch.IsToggled = fixvertical;
            MipMapSwitch.IsToggled = mipmap;
            if (!GHApp.IsMaui)
            {
                MipMapGrid.IsVisible = false;
            }
            if (GHApp.IsGPUAvailable)
            {
                GPUSwitch.IsToggled = gpu;
                DisableAuxGPUSwitch.IsEnabled = gpu;
                DisableAuxGPULabel.TextColor = gpu ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
                //FixRectsSwitch.IsEnabled = gpu;
                //FixRectsLabel.TextColor = gpu ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
                MipMapSwitch.IsEnabled = gpu;
                MipMapLabel.TextColor = gpu ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
                RuntimeEffectsSwitch.IsEnabled = gpu && experimental; /* Currently experimental so disabled unless experimental is true to avoid setting the setting value before the feature is fully developed */
                RuntimeEffectsLabel.TextColor = gpu && experimental ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            }
            else
            {
                GPUSwitch.IsToggled = false;
                GPUSwitch.IsEnabled = false;
                GPULabel.IsEnabled = false;
                GPULabel.TextColor = GHColors.Gray;

                //FixRectsSwitch.IsEnabled = false;
                //FixRectsLabel.TextColor = GHColors.Gray;

                DisableAuxGPUSwitch.IsEnabled = false;
                DisableAuxGPULabel.TextColor = GHColors.Gray;

                MipMapSwitch.IsEnabled = false;
                MipMapLabel.TextColor = GHColors.Gray;
                RuntimeEffectsSwitch.IsEnabled = false;
                RuntimeEffectsLabel.TextColor = GHColors.Gray;
            }
            SimpleCmdLayoutSwitch.IsToggled = simplecmdlayout;
            ShowAltZoomButtonSwitch.IsToggled = showaltzoom;
            ShowTravelModeButtonSwitch.IsToggled = showtravelmode;
            ShowAutoDigButtonSwitch.IsToggled = showautodig;
            ShowIgnoreStoppingButtonSwitch.IsToggled = showignore;
            DarkModeSwitch.IsToggled = darkmode;
            SilentModeSwitch.IsToggled = silentmode;
            WindowedModeSwitch.IsToggled = windowedmode;
            Edge2EdgeSwitch.IsToggled = edge2edge;
            if (!GHApp.IsDesktop)
            {
                WindowedModeSwitch.IsEnabled = false;
                WindowedModeLabel.IsEnabled = false;
                WindowedModeLabel.TextColor = GHColors.Gray;
                WindowedModeStackLayout.IsVisible = false;
            }
            else /* Disabled until works on Android */
            {
                Edge2EdgeSwitch.IsEnabled = false;
                Edge2EdgeLabel.IsEnabled = false;
                Edge2EdgeLabel.TextColor = GHColors.Gray;
                Edge2EdgeStackLayout.IsVisible = false;
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
            if (devmode)
            {
                LogMessageSwitch.IsToggled = logmessages;
                LowLevelLogSwitch.IsToggled = lowlevellogging;
                ScreenLogSwitch.IsToggled = screenlogging;
                DebugPostChannelSwitch.IsToggled = debugpostchannel;
            }
            else
            {
                LogMessageSwitch.IsToggled = false;
                LogMessageSwitch.IsEnabled = false;
                LogMessageLabel.IsEnabled = false;
                LogMessageLabel.TextColor = GHColors.Gray;
                LowLevelLogSwitch.IsToggled = false;
                LowLevelLogSwitch.IsEnabled = false;
                LowLevelLogLabel.IsEnabled = false;
                LowLevelLogLabel.TextColor = GHColors.Gray;
                ScreenLogSwitch.IsToggled = false;
                ScreenLogSwitch.IsEnabled = false;
                ScreenLogLabel.IsEnabled = false;
                ScreenLogLabel.TextColor = GHColors.Gray;
                DebugPostChannelSwitch.IsToggled = false;
                DebugPostChannelSwitch.IsEnabled = false;
                DebugPostChannelLabel.IsEnabled = false;
                DebugPostChannelLabel.TextColor = GHColors.Gray;
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
            if (_gamePage != null) /* Cannot turn on or off in the middle of the game */
            {
                AllowBonesSwitch.IsEnabled = false;
                AllowBonesLabel.TextColor = GHColors.Gray;
            }
            AllowPetSwitch.IsToggled = allowpet;
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
            MetricSystemSwitch.IsToggled = metricsystem;
            DiceAsRangesSwitch.IsToggled = diceasranges;
            DamageFormulaSwitch.IsToggled = damageformula;
            WornShowsEquipmentSwitch.IsToggled = wornshowsequipment;
            AutoDigSwitch.IsToggled = autodig;
            IgnoreStoppingSwitch.IsToggled = ignorestopping;
            DoubleClickSwitch.IsToggled = doubleclick;
            GetPositionArrowsSwitch.IsToggled = getpositionarrows;

            if (RightMousePicker.ItemsSource != null)
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

            EngraveQuickEntry.Text = engravequick;
            if (engraveqstyle >= 0 && engraveqstyle < EngraveQuickPicker.Items?.Count)
                EngraveQuickPicker.SelectedIndex = engraveqstyle;

            if(!GHApp.IsWindows)
            {
                RightMouseButtonGrid.IsVisible = false;
                MiddleMouseButtonGrid.IsVisible = false;
            }

            PostGameStatusSwitch.IsToggled = postgamestatus;
#if !SENTRY
            PostDiagnosticDataSwitch.IsToggled = postdiagnostics;
#else
            PostDiagnosticDataGrid.IsVisible = false;
#endif
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
            XlogReleaseAccountGrid.IsVisible = GHApp.IsDebug;
            ForcePostBonesSwitch.IsToggled = forcepostbones;
            ForcePostBonesGrid.IsVisible = GHApp.IsDebug;
            if (GHApp.IsSaveFileTrackingNeeded)
            {
                SaveFileTrackingSwitch.IsToggled = save_file_tracking;
                if (_gamePage != null)
                {
                    SaveFileTrackingSwitch.IsEnabled = false;
                    SaveFileTrackingLabel.TextColor = GHColors.Gray;
                }
            }
            else
            {
                SaveFileTrackingGrid.IsVisible = false;
                SaveFileTrackingSwitch.IsToggled = false;
            }

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
            HideMessageHistorySwitch.IsToggled = hidemsghistory;
            DisableWindowsKeySwitch.IsToggled = disablewindowskey;
            DisableWindowsKeyGrid.IsVisible = GHApp.IsWindows;
            DefaultVIKeysSwitch.IsToggled = defaultvikeys;
            ShowKeyboardShortcutsSwitch.IsToggled = showkeyboardshortcuts;
            UseSingleMoreCommandsPageSwitch.IsToggled = singlecmdpage;
            ShowSkillContextButtonSwitch.IsToggled = skillbutton;
            ShowPolearmContextButtonSwitch.IsToggled = polearmbutton;
            EquipmentFlipAnimationSwitch.IsToggled = equipmentflipanimation;
            ShowEquipmentIconsSwitch.IsToggled = showequipmenticons;
            _doChangeVolume = true; // !GHApp.IsMuted;
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
            //_doChangeVolume = !GHApp.IsMuted;
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
                LowLevelLogSwitch.IsEnabled = true;
                LowLevelLogLabel.IsEnabled = true;
                LowLevelLogLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                ScreenLogSwitch.IsEnabled = true;
                ScreenLogLabel.IsEnabled = true;
                ScreenLogLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                DebugPostChannelSwitch.IsEnabled = true;
                DebugPostChannelLabel.IsEnabled = true;
                DebugPostChannelLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            }
            else
            {
                LogMessageSwitch.IsEnabled = false;
                LogMessageSwitch.IsToggled = false;
                LogMessageLabel.IsEnabled = false;
                LogMessageLabel.TextColor = GHColors.Gray;
                LowLevelLogSwitch.IsEnabled = false;
                LowLevelLogSwitch.IsToggled = false;
                LowLevelLogLabel.IsEnabled = false;
                LowLevelLogLabel.TextColor = GHColors.Gray;
                ScreenLogSwitch.IsEnabled = false;
                ScreenLogSwitch.IsToggled = false;
                ScreenLogLabel.IsEnabled = false;
                ScreenLogLabel.TextColor = GHColors.Gray;
                DebugPostChannelSwitch.IsEnabled = false;
                DebugPostChannelSwitch.IsToggled = false;
                DebugPostChannelLabel.IsEnabled = false;
                DebugPostChannelLabel.TextColor = GHColors.Gray;
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
            await ClosePageAsync(true);
        }

        private async Task ClosePageAsync(bool playClickedSound)
        {
            CloseButton.IsEnabled = false;
            _backPressed = true;
            if (playClickedSound)
                GHApp.PlayButtonClickedSound();
            SetTournamentModeLabelColors(TournamentSwitch.IsToggled);
            //PostXlogUserNameLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            BonesAllowedUsersLabel.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
            if (EngraveQuickEntry.Text != null && EngraveQuickEntry.Text != "")
            {
                if (!EngraveQuickTextValidationExpression.IsMatch(EngraveQuickEntry.Text))
                {
                    EngraveQuickLabel.TextColor = GHColors.Red;
                    await MainScrollView.ScrollToAsync(EngraveQuickGrid.X, EngraveQuickGrid.Y, true);
                    EngraveQuickEntry.Focus();
                    CloseButton.IsEnabled = true;
                    _backPressed = false;
                    return;
                }
            }
            if (PostXlogUserNameEntry.Text != null && PostXlogUserNameEntry.Text != "")
            {
                if (!XlogUserNameValidationExpression.IsMatch(PostXlogUserNameEntry.Text))
                {
                    PostXlogUserNameLabel.TextColor = GHColors.Red;
                    await MainScrollView.ScrollToAsync(PostXlogUserNameGrid.X, PostXlogUserNameGrid.Y, true);
                    PostXlogUserNameEntry.Focus();
                    CloseButton.IsEnabled = true;
                    _backPressed = false;
                    return;
                }
            }
            if (BonesAllowedUsersEntry.Text != null && BonesAllowedUsersEntry.Text != "")
            {
                if (!BonesAllowedUsersValidationExpression.IsMatch(BonesAllowedUsersEntry.Text))
                {
                    BonesAllowedUsersLabel.TextColor = GHColors.Red;
                    await MainScrollView.ScrollToAsync(BonesAllowedUsersGrid.X, BonesAllowedUsersGrid.Y, true);
                    BonesAllowedUsersEntry.Focus();
                    CloseButton.IsEnabled = true;
                    _backPressed = false;
                    return;
                }
            }
            if (TournamentSwitch.IsToggled)
            {
                if (!GHApp.XlogUserNameVerified || !GHApp.AreCredentialsVerified(PostXlogUserNameEntry.Text, PostXlogPasswordEntry.Text))
                {
                    GHApp.SetXlogUserNameVerified(false, null, null);
                    bool hasNoUserName = string.IsNullOrEmpty(PostXlogUserNameEntry.Text);
                    bool hasNoPassword = string.IsNullOrEmpty(PostXlogPasswordEntry.Text);
                    if (!hasNoUserName && !hasNoPassword)
                    {
                        PopupTitleLabel.TextColor = GHColors.TitleGoldColor;
                        PopupTitleLabel.Text = "Credentials Verification";
                        PopupLabel.Text = "Verifying credentials... Please wait.";
                        PopupOkButton.IsEnabled = true;
                        PopupGrid.IsVisible = true;
                        GHApp.XlogUserName = PostXlogUserNameEntry.Text;
                        GHApp.XlogPassword = PostXlogPasswordEntry.Text;
                        GHApp.XlogReleaseAccount = XlogReleaseAccountSwitch.IsToggled;
                        GHApp.XlogCredentialsIncorrect = false;
                        await GHApp.SendXLogEntry("", 1, 0, new List<GHPostAttachment>(), true);
                        PopupGrid.IsVisible = false;
                    }
                    if (!GHApp.XlogUserNameVerified)
                    {
                        PopupTitleLabel.TextColor = GHColors.Orange;
                        PopupTitleLabel.Text = "Tournament Verification";
                        PopupLabel.Text = "User name and password in the Server Posting section must be verified for the Tournament Mode.";
                        PopupOkButton.IsEnabled = true;
                        PopupGrid.IsVisible = true;
                        CloseButton.IsEnabled = true;
                        _backPressed = false;
                        await MainScrollView.ScrollToAsync(0, PostXlogUserNameGrid.Y, true);
                        return;
                    }
                }
            }
            await MaybeShowPleaseWait();
            await SetSettingValues();
            GHApp.CurrentMainPage?.InvalidateCarousel();
            await GHApp.PopModalPageAsync();
            GHApp.UpdateFreeDiskSpace();
            GHApp.UpdateUsedMemory();
        }

        public bool HandleSpecialKeyPress(GHSpecialKey key, bool isCtrl, bool isMeta, bool isShift)
        {
            if (GHApp.PushingModalPage || GHApp.IsSystemBrowserOpen) /* Ignore key presses when opening a page or using a system browser */
                return true;

            bool handled = false;
            if (WaitLayout.IsVisible)
                return true;

            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (key == GHSpecialKey.Escape || key == GHSpecialKey.Enter)
                        {
                            if (PopupGrid.IsVisible)
                            {
                                if (PopupOkButton.IsEnabled && PopupOkButton.IsVisible)
                                {
                                    PopupOkButton_Clicked(PopupOkButton, EventArgs.Empty);
                                }
                            }
                            else if (TextGrid.IsVisible)
                            {
                                if (TextCancelButton.IsEnabled && key == GHSpecialKey.Escape)
                                {
                                    TextCancelButton_Clicked(TextCancelButton, EventArgs.Empty);
                                }
                                else if (TextOkButton.IsEnabled && key == GHSpecialKey.Enter)
                                {
                                    TextOkButton_Clicked(TextOkButton, EventArgs.Empty);
                                }
                            }
                            else
                            {
                                if (CloseButton.IsEnabled)
                                    await ClosePageAsync(true);
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                });
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            return handled;
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
#if GNH_MAUI
                TextFrame.Stroke = SolidColorBrush.Red;
#else
                TextFrame.BorderColor = GHColors.Red;
#endif
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
#if GNH_MAUI
            TextFrame.Stroke = SolidColorBrush.Black;
#else
            TextFrame.BorderColor = GHColors.Black;
#endif

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
#if GNH_MAUI
            TextFrame.Stroke = SolidColorBrush.Black;
#else
            TextFrame.BorderColor = GHColors.Black;
#endif

            if (_linkButtonClicked == null) _linkButtonClicked = CustomLinkButton;
            _linkButtonClicked.IsEnabled = true;
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                await ClosePageAsync(false);
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

        private void VolumeSlider_ValueChanged(object sender, ValueChangedEventArgs e)
        {
            float generalVolume = (float)GeneralVolumeSlider.Value;
            float musicVolume = (float)MusicVolumeSlider.Value;
            float ambientVolume = (float)AmbientVolumeSlider.Value;
            float dialogueVolume = (float)DialogueVolumeSlider.Value;
            float effectsVolume = (float)EffectsVolumeSlider.Value;
            float uiVolume = (float)UIVolumeSlider.Value;

            //if (_doChangeVolume)
            //    GHApp.FmodService?.AdjustVolumes((float)GeneralVolumeSlider.Value, (float)MusicVolumeSlider.Value, (float)AmbientVolumeSlider.Value, (float)DialogueVolumeSlider.Value, (float)EffectsVolumeSlider.Value, (float)UIVolumeSlider.Value);
            if (_doChangeVolume)
            {
                /* Adjust first UI volumes */
                GHApp.FmodService?.AdjustUIVolumes(generalVolume, musicVolume, uiVolume);
                GHGame curGame = GHApp.CurrentGHGame;
                if (curGame != null)
                {
                    /* Game is ongoing, so ask GHGame to handle this */
                    curGame.ResponseQueue.Enqueue(new GHResponse(curGame, GHRequestType.SetVolume, generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, uiVolume));
                }
                else
                {
                    /* Adjust directly, since there's no game thread */
                    GHApp.FmodService?.AdjustGameVolumes(generalVolume, musicVolume, ambientVolume, dialogueVolume, effectsVolume, uiVolume);
                }
            }
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
            bool hasNoUserName = string.IsNullOrEmpty(PostXlogUserNameEntry.Text);
            bool hasNoPassword = string.IsNullOrEmpty(PostXlogPasswordEntry.Text);
            if (hasNoUserName || hasNoPassword)
            {
                PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                PopupTitleLabel.Text = "Credentials Not Set";
                PopupLabel.Text = (hasNoUserName && hasNoPassword ? "User name and password have" : hasNoUserName ? "User name has" : "Password has") +" not been set.";
                PopupOkButton.IsEnabled = true;
                PopupGrid.IsVisible = true;
            }
            else
            {
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
                        (res.HasHttpStatusCode && res.StatusCode != System.Net.HttpStatusCode.OK ? " Status Code: " + (int)res.StatusCode + " (" + res.StatusCode.ToString() + ")" : "");
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
                else if (res.StatusCode == System.Net.HttpStatusCode.Forbidden)
                {
                    XlogTestButton.TextColor = GHColors.Red;
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Login Failed";
                    PopupLabel.Text = "Logging in to " + (GHApp.IsDebug && !GHApp.XlogReleaseAccount ? "Test " : "") + "GnollHack Server failed. Check your user name and password.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
#if GNH_MAUI
                else if (res.StatusCode == System.Net.HttpStatusCode.Locked)
                {
                    XlogTestButton.TextColor = GHColors.Red;
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Locked Out";
                    PopupLabel.Text = "You have been locked out of your account on " + (GHApp.IsDebug && !GHApp.XlogReleaseAccount ? "Test " : "") + "GnollHack Server. Please contact customer support.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
#endif
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
                string text = RecordLabel.Text;
                if (!string.IsNullOrEmpty(text) && _settingDescriptions.ContainsKey(text))
                {
                    ShowSettingInfoPopup(text, _settingDescriptions[text], RecordLabel.FontFamily);
                }
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
            if (GZipSwitch.IsEnabled || _gamePage == null)
            {
                string text = GZipLabel.Text;
                if (!string.IsNullOrEmpty(text) && _settingDescriptions.ContainsKey(text))
                {
                    ShowSettingInfoPopup(text, _settingDescriptions[text], GZipLabel.FontFamily);
                }
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
            //FixRectsSwitch.IsEnabled = e.Value;
            //FixRectsLabel.TextColor = e.Value ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            DisableAuxGPUSwitch.IsEnabled = e.Value;
            DisableAuxGPULabel.TextColor = e.Value ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
            MipMapSwitch.IsEnabled = e.Value;
            MipMapLabel.TextColor = e.Value ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;

            if (_isManualTogglingEnabled)
            {
                if (e.Value && !GHApp.IsGPUDefault)
                {
                    PopupTitleLabel.TextColor = GHColors.Orange;
                    PopupTitleLabel.Text = "Unstable GPU Acceleration";
                    PopupLabel.Text = "Your device has been detected as potentially causing crashes when GPU acceleration is switched on. Proceed with care.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
                if (ScreenResolutionGrid.IsVisible)
                {
                    GHApp.UpdateRecommendedScreenResolution(e.Value);
                    _isManualTogglingEnabled = false;
                    int selIdx = ScreenResolutionPicker.SelectedIndex;
                    ScreenResolutionPicker.ItemsSource = null;
                    ScreenResolutionPicker.ItemsSource = GHApp.ScreenResolutionItems;
                    if (selIdx >= 0 && selIdx < ScreenResolutionPicker.ItemsSource.Count)
                        ScreenResolutionPicker.SelectedIndex = selIdx;
                    _isManualTogglingEnabled = true;
                    ScreenResolutionPicker_SelectedIndexChanged(this, EventArgs.Empty);
                }
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
            PopupLabel.Text = "Use Tournament Mode only when playing in a tournament." + Environment.NewLine + Environment.NewLine + "Tournament Mode will force on Post Game Progress, Post Top Scores, Allow Ghost Levels, Share Bones Files, " + 
                (SaveFileTrackingGrid.IsVisible ? "Save File Tracking, " : "") + 
                "Record Game, and Auto-Upload to Cloud settings. The mode will also disable all special game play modes, custom links, and custom webhooks." + 
                (GHApp.XlogUserNameVerified ? "" : Environment.NewLine + Environment.NewLine + 
                "Please make sure that your user name and password for Server Posting are verified before proceeding.");
            PopupOkButton.IsEnabled = true;
            PopupGrid.IsVisible = true;
        }

        private void TournamentSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if(e.Value && !GHApp.TournamentMode)
            {
                ShowTournamentInfoPopup();
            }
            GHApp.TournamentMode = e.Value;
            SetTournamentModeLabelColors(e.Value);
        }

        private void PostDiagnosticDataSwitch_Toggled(object sender, ToggledEventArgs e)
        {
#if !SENTRY
            if (e.Value && !GHApp.PostingDiagnosticData && GHApp.IsiOS)
            {
                PopupTitleLabel.TextColor = GHColors.Orange;
                PopupTitleLabel.Text = "Share With App Developers";
                PopupLabel.Text = "To enable us to get information on app crashes, please make sure that also Share With App Developers is on in the Setting app under Privacy -> Analytics.";
                PopupOkButton.IsEnabled = true;
                PopupGrid.IsVisible = true;
            }
#endif
        }

        private void DarkModeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (_isManualTogglingEnabled)
            {
                GHApp.DarkMode = e.Value;
                SetDarkMode(e.Value);
                bkgView.InvalidateSurface();
                lblHeader.TextColor = e.Value ? GHColors.White : GHColors.Black;
                SetChildrenDarkModeTextColor(RootLayout, e.Value);
                if (e.Value)
                    PopupFrame.BackgroundColor = Color.FromRgba(0.15, 0.15, 0.15, 0.85);
                else
                    PopupFrame.BackgroundColor = Color.FromRgba(0.0, 0.0, 0.0, (double)0x99 / (double)0xFF);

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

        private void MipMapSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (_isManualTogglingEnabled)
            {
                bool oldValue = GHApp.UseMipMap;
                if (oldValue != e.Value)
                {
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.CLR_ORANGE, 0, false, true);
                    PopupTitleLabel.Text = "Resource Usage Warning";
                    //ulong memreq = GHConstants.UseMipMapThresholdInBytes / 1024 / 1024;
                    //ulong mem = GHApp.TotalMemory / 1024 / 1024;
                    PopupLabel.Text = "Changing the mipmap setting may require more memory or CPU power than normal.";
                    //"It is recommended for the device to have at least " + 
                    //    memreq + " MB of memory. Your device has " + mem + " MB of memory.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
            }
        }

        //protected override bool OnBackButtonPressed()
        //{
        //    return true;
        //}

        private void SaveFileTrackingSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if(_isManualTogglingEnabled)
            {
                UpdateServerPostingEnabled(e.Value);
            }
        }

        private void UpdateServerPostingEnabled(bool trackSaveFiles) 
        {
            if(SaveFileTrackingGrid.IsVisible)
            {
                PostXlogUserNameEntry.IsEnabled = trackSaveFiles;
                PostXlogPasswordEntry.IsEnabled = trackSaveFiles;
                PostXlogSwitch.IsEnabled = trackSaveFiles;
                PostReplaysSwitch.IsEnabled = trackSaveFiles;
                PostBonesSwitch.IsEnabled = trackSaveFiles;
                BonesListSwitch.IsEnabled = trackSaveFiles;
                BonesAllowedUsersEntry.IsEnabled = trackSaveFiles;
                ForcePostBonesSwitch.IsEnabled = trackSaveFiles;

                Color labelColor = trackSaveFiles ? (GHApp.DarkMode ? GHColors.White : GHColors.Black) : GHColors.Gray;
                PostXlogUserNameLabel.TextColor = labelColor;
                PostXlogPasswordLabel.TextColor = labelColor;
                PostXlogLabel.TextColor = labelColor;
                PostReplaysLabel.TextColor = labelColor;
                PostBonesLabel.TextColor = labelColor;
                BonesListLabel.TextColor = labelColor;
                BonesAllowedUsersLabel.TextColor = labelColor;
                ForcePostBonesLabel.TextColor = labelColor;

                if (trackSaveFiles)
                {
                    AllowBonesSwitch_Toggled(null, new ToggledEventArgs(AllowBonesSwitch.IsToggled));
                    BonesListSwitch_Toggled(null, new ToggledEventArgs(BonesListSwitch.IsToggled));
                }
            }
        }

        private void DebugPostChannelSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            GHApp.DebugPostChannel = e.Value;
        }

        private void ScreenResolutionPicker_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_isManualTogglingEnabled && ScreenResolutionGrid.IsVisible && ScreenResolutionPicker.SelectedIndex >= 0)
            {
                ScreenResolutionItem item = ScreenResolutionPicker.SelectedItem as ScreenResolutionItem;
                if (item != null)
                {
                    GHApp.CustomScreenResolutionWidth = item.Width;
                    GHApp.CustomScreenResolutionHeight = item.Height;
                    GHApp.CustomScreenResolutionRefreshRate = item.RefreshRate;
                    GHApp.CustomScreenResolutionPriority = item.ListPriority;
                    if (item.ListPriority == 2 || item.Width == 0 || item.Height == 0 || item.RefreshRate == 0)
                        GHApp.RevertScreenResolution();
                    else
                        GHApp.ChangeScreenResolution(item.Width, item.Height, item.RefreshRate);
                    UpdateDefaultMapRefreshRate();
                }
            }
        }

        private void PlatformRenderLoopSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (_isManualTogglingEnabled)
            {
                UpdateDefaultMapRefreshRate();
                if (GHApp.IsAndroid && !e.Value && (GHApp.PlatformService?.IsRemoveAnimationsOn() ?? false))
                {
                    string animationSettingName = GHApp.OneUIAnimationSettingName;
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Invalid Animation Settings";
                    PopupLabel.Text = "GnollHack has detected animation settings that are incompatible with Platform Render Loop being turned off."
                            + (GHApp.IsSamsung ? " In the Android Settings app, please switch off \"" + animationSettingName + "\" under Accessibility > Visibility Enhancements." : " In the Android settings app, please adjust the value of \"Animator duration scale\" to 1x under Developer Options > Animator duration scale.");
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
            }
        }

        private void Edge2EdgeSwitch_Toggled(object sender, ToggledEventArgs e)
        {
            if (_isManualTogglingEnabled)
            {
                //UpdateEdge2EdgeOnAllPages(e.Value);
                bool oldValue = GHApp.Edge2Edge;
                if (oldValue != e.Value)
                {
                    PopupTitleLabel.TextColor = UIUtils.NHColor2XColor((int)NhColor.NO_COLOR, 0, false, true);
                    PopupTitleLabel.Text = "Restart Required";
                    PopupLabel.Text = "Please restart GnollHack for the changes in Edge to Edge to take effect.";
                    PopupOkButton.IsEnabled = true;
                    PopupGrid.IsVisible = true;
                }
            }
        }

        private void UpdateEdge2EdgeOnAllPages(bool newValue)
        {
            bool oldEdge2Edge = GHApp.Edge2Edge;
            if (oldEdge2Edge != newValue)
            {
                GHApp.Edge2Edge = newValue;
                if (GHApp.Navigation?.ModalStack?.Count > 0)
                {
                    foreach (var page in GHApp.Navigation.ModalStack)
                    {
                        if (page is CustomContentPage customPage)
                        {
                            customPage?.SetEdge2Edge(newValue);
                        }
                    }
                }
                _mainPage?.SetEdge2Edge(newValue);
            }
        }
        private void UpdateDarkModeOnAllPages(bool newValue)
        {
            bool oldDarkMode = GHApp.DarkMode;
            if (oldDarkMode != newValue)
            {
                GHApp.DarkMode = newValue;
                if (GHApp.Navigation?.ModalStack?.Count > 0)
                {
                    foreach (var page in GHApp.Navigation.ModalStack)
                    {
                        if (page is CustomContentPage customPage)
                        {
                            customPage?.SetDarkMode(newValue);
                        }
                    }
                }
                _mainPage?.SetDarkMode(newValue);
            }
        }
    }
}