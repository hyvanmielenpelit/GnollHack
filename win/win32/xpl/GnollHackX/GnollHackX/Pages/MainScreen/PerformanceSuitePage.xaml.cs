using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GnollHackX.Performance;

#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;

namespace GnollHackM
#else
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    /* A single row of the results CollectionView, built fresh from a
       GHPerformanceSuiteInfo every time the list is refreshed. */
    public sealed class PerformanceSuiteListItem
    {
        public string SuiteId { get; set; }
        public string DateText { get; set; }
        public string ScenarioText { get; set; }
        public string LabelText { get; set; }
        public string RunsText { get; set; }
        public string HitchText { get; set; }
        public string SizeText { get; set; }
        public string MarkersText { get; set; }
        public bool HasMarkers { get; set; }
        public Color ItemTextColor { get; set; }
        public Thickness RowPadding { get; set; }
    }

    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class PerformanceSuitePage : CustomModalPage, ICloseablePage, IMessagePopupPage
    {
        /* Backs the Scenario and Page Mode pickers: a display name paired with the
           lower-case value the runner API expects. */
        private sealed class NamedValueItem
        {
            public readonly string DisplayName;
            public readonly string Value;
            public NamedValueItem(string displayName, string value)
            {
                DisplayName = displayName;
                Value = value;
            }
            public override string ToString()
            {
                return DisplayName;
            }
        }

        /* Backs the Runs picker's fixed choice of run counts. */
        private sealed class RunsItem
        {
            public readonly int Value;
            public RunsItem(int value)
            {
                Value = value;
            }
            public override string ToString()
            {
                return Value.ToString();
            }
        }

        /* Backs the Replay picker: a short display name derived from the file name,
           with the full path kept alongside it for use by the runner. */
        private sealed class ReplayItem
        {
            public readonly string DisplayName;
            public readonly string FullPath;
            public ReplayItem(string displayName, string fullPath)
            {
                DisplayName = displayName;
                FullPath = fullPath;
            }
            public override string ToString()
            {
                return DisplayName;
            }
        }

        private string _pendingSelectSuiteId;

        /* Explanation shown in a message box when the keyed label is tapped; the
           label's own text is the message box title. */
        private readonly Dictionary<Label, string> _labelExplanations = new Dictionary<Label, string>();

        public PerformanceSuitePage() : this(null)
        {
        }

        public PerformanceSuitePage(string selectSuiteId)
        {
            InitializeComponent();
            if (GHApp.DarkMode)
            {
                lblHeader.TextColor = GHColors.White;
            }
            SetChildrenDarkModeTextColor(MainLayout, GHApp.DarkMode);

            InitializeInfoLabels();
            InitializePickers();
            LoadPreferences();
            PopulateReplays();
            UpdateEstimatedDuration();

            /* The results list itself is populated from ContentPage_Appearing, once the
               page is actually about to become visible, matching ReplayPage. */
            _pendingSelectSuiteId = selectSuiteId;
        }

        private void InitializeInfoLabels()
        {
            AddInfoLabel(ReplayLabel,
                "The replay each run plays",
                "Lists the replays in the replay folder, newest first. For Minimap, record a wizard-mode game and press Ctrl+F to map the level first. For Playback, record a stretch with fighting and effects.");
            AddInfoLabel(StartTurnLabel,
                "The replay turn each run starts at",
                "The replay turn at which each run starts. Choose a turn with more than warm-up + window seconds of content after it. Early turns seek faster.");
            AddInfoLabel(ScenarioLabel,
                "What is measured: idle, minimap or playback",
                "Idle: the replay is paused at the player's command prompt and only animations run. Minimap: the same in minimap mode. Playback: the replay plays at normal speed through the window.");
            AddInfoLabel(RunsLabel,
                "Measured runs, not counting the warm-up run",
                "The warm-up run is not counted. At least 3 measured runs are needed for a verdict, and 5 or more for a firm one.");
            AddInfoLabel(WarmUpRunLabel,
                "An extra first run, excluded from the results",
                "Run 0 is measured but excluded from the results. It absorbs the loading and garbage-collection storm after the game page opens.");
            AddInfoLabel(PageModeLabel,
                "Reuse one game page, or open a new one per run",
                "Shared page restarts the replay in place, which also collects garbage. Fresh page closes and reopens the game page for each run.");
            AddInfoLabel(ArmLabelLabel,
                "Suites with the same label are compared as one",
                "Suites with the same label pool into one arm when compared. The default is the app version and commit; an edited label is kept until another build is installed.");
            AddInfoLabel(WarmUpSecondsLabel,
                "Seconds played before measuring",
                "Seconds each run plays the scenario before measuring starts.");
            AddInfoLabel(WindowSecondsLabel,
                "Seconds measured per run",
                "Seconds measured in each run.");
            AddInfoLabel(CooldownSecondsLabel,
                "Seconds of rest between runs",
                "Seconds of pause between runs, letting the device cool down before the next run.");
            AddInfoLabel(EstimatedDurationTitleLabel,
                "Approximate time for the whole suite",
                "Every run, including the warm-up run, takes its warm-up, window and cool-down seconds plus an allowance for loading and seeking.");
            AddInfoLabel(ResultsLabel,
                "Suites saved on this device",
                "Suites are saved on this device. "
                + (GHApp.IsWindows
                    ? "Both builds share this store."
                    : "Share suites before reinstalling the app, and import them afterwards.")
                + " Tap suites to select them; Report, Set Baseline and Compare need exactly one.");
        }

        /* The tooltip is a short summary; tapping the label shows the full explanation */
        private void AddInfoLabel(Label label, string tooltip, string explanation)
        {
            _labelExplanations[label] = explanation;
            UIUtils.SetViewCursorOnHandler(label, GameCursorType.Info);
            UIUtils.SetStyledToolTip(label, tooltip);
        }

        private async void InfoLabel_Tapped(object sender, EventArgs e)
        {
            Label label = sender as Label;
            string explanation;
            if (label == null || !_labelExplanations.TryGetValue(label, out explanation))
                return;
            await GHApp.DisplayMessageBox(this, label.Text, explanation, "OK");
        }

        private void InitializePickers()
        {
            List<NamedValueItem> scenarios = new List<NamedValueItem>
            {
                new NamedValueItem("Idle", "idle"),
                new NamedValueItem("Minimap", "minimap"),
                new NamedValueItem("Playback", "playback"),
            };
            ScenarioPicker.ItemsSource = scenarios;

            List<RunsItem> runs = new List<RunsItem>
            {
                new RunsItem(3),
                new RunsItem(5),
                new RunsItem(6),
                new RunsItem(8),
            };
            RunsPicker.ItemsSource = runs;

            List<NamedValueItem> pageModes = new List<NamedValueItem>
            {
                new NamedValueItem("Shared page", "shared"),
                new NamedValueItem("Fresh page per run", "fresh"),
            };
            PageModePicker.ItemsSource = pageModes;
        }

        private void LoadPreferences()
        {
            SelectNamedValuePicker(ScenarioPicker, Preferences.Get("PerformanceSuiteScenario", "idle"), 0);
            SelectRunsPicker(Preferences.Get("PerformanceSuiteRuns", 6));
            WarmUpRunSwitch.IsToggled = Preferences.Get("PerformanceSuiteWarmUpRun", true);
            SelectNamedValuePicker(PageModePicker, Preferences.Get("PerformanceSuitePageMode", "shared"), 0);
            StartTurnEntry.Text = Math.Max(1, Preferences.Get("PerformanceSuiteStartTurn", 1)).ToString();
            WarmUpSecondsEntry.Text = Preferences.Get("PerformanceSuiteWarmUpSeconds", 10).ToString();
            WindowSecondsEntry.Text = Preferences.Get("PerformanceSuiteWindowSeconds", 60).ToString();
            CooldownSecondsEntry.Text = Preferences.Get("PerformanceSuiteCooldownSeconds", 20).ToString();

            /* The saved label is kept only while the build's default label is the one it was
               saved under; another build starts from its own default, so that it never
               pools into the previous build's arm */
            _defaultArmLabel = BuildDefaultArmLabel();
            string savedLabel = Preferences.Get("PerformanceSuiteLabel", "");
            string savedDefaultLabel = Preferences.Get("PerformanceSuiteLabelDefault", "");
            bool sameBuild = string.Equals(savedDefaultLabel, _defaultArmLabel, StringComparison.Ordinal);
            ArmLabelEntry.Text = !sameBuild || string.IsNullOrWhiteSpace(savedLabel) ? _defaultArmLabel : savedLabel;
        }

        private string _defaultArmLabel = "";

        /* "<app version> <short commit>", omitting the commit when it is unknown. The
           commit is the part after '+' of the informational version of the assembly
           GHPerformanceEnvironment reads it from; a full environment capture is avoided
           here, since its first call can block on WMI. */
        private string BuildDefaultArmLabel()
        {
            string version = GHApp.GHVersionString;
            string commit = null;
            try
            {
                System.Reflection.AssemblyInformationalVersionAttribute attribute = System.Attribute.GetCustomAttribute(
                    typeof(GHPerformanceEnvironment).Assembly, typeof(System.Reflection.AssemblyInformationalVersionAttribute))
                    as System.Reflection.AssemblyInformationalVersionAttribute;
                string informational = attribute != null ? attribute.InformationalVersion : null;
                int plus = informational != null ? informational.IndexOf('+') : -1;
                if (plus >= 0 && plus + 1 < informational.Length)
                    commit = informational.Substring(plus + 1);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
            if (!string.IsNullOrEmpty(commit) && commit.Length > 7)
                commit = commit.Substring(0, 7);
            if (string.IsNullOrEmpty(commit))
                return version ?? "";
            return ((version ?? "") + " " + commit).Trim();
        }

        private void SelectNamedValuePicker(Picker picker, string value, int fallbackIndex)
        {
            if (picker.ItemsSource != null)
            {
                int index = 0;
                foreach (object obj in picker.ItemsSource)
                {
                    NamedValueItem item = obj as NamedValueItem;
                    if (item != null && item.Value == value)
                    {
                        picker.SelectedIndex = index;
                        return;
                    }
                    index++;
                }
            }
            picker.SelectedIndex = fallbackIndex;
        }

        private void SelectRunsPicker(int runs)
        {
            int index = 0;
            foreach (object obj in RunsPicker.ItemsSource)
            {
                RunsItem item = obj as RunsItem;
                if (item != null && item.Value == runs)
                {
                    RunsPicker.SelectedIndex = index;
                    return;
                }
                index++;
            }
            RunsPicker.SelectedIndex = 2; /* "6" */
        }

        /* Lists the main replay files in the replay directory, newest first: names
           starting with ReplayFileNamePrefix, never continuation files, accepting the
           plain suffix plus its zip/gzip variants. Mirrors the rules ReplayPage uses to
           tell a main replay file from a continuation or a stray zip. */
        private void PopulateReplays()
        {
            List<ReplayItem> items = new List<ReplayItem>();
            try
            {
                string replayDir = Path.Combine(GHApp.GHPath, GHConstants.ReplayDirectory);
                if (Directory.Exists(replayDir))
                {
                    string[] files = Directory.GetFiles(replayDir);
                    if (files != null)
                    {
                        Array.Sort(files, new FileDateComparer()); /* newest first */
                        foreach (string file in files)
                        {
                            if (!File.Exists(file))
                                continue;
                            FileInfo fi = new FileInfo(file);
                            if (fi == null || string.IsNullOrWhiteSpace(fi.Name))
                                continue;
                            if (!fi.Name.StartsWith(GHConstants.ReplayFileNamePrefix))
                                continue;
                            if (fi.Name.StartsWith(GHConstants.ReplayContinuationFileNamePrefix))
                                continue;

                            bool isGZip = fi.Name.Length > GHConstants.ReplayGZipFileNameSuffix.Length && fi.Name.EndsWith(GHConstants.ReplayGZipFileNameSuffix);
                            bool isZip = fi.Name.Length > GHConstants.ReplayZipFileNameSuffix.Length && fi.Name.EndsWith(GHConstants.ReplayZipFileNameSuffix);
                            bool isPlain = fi.Name.EndsWith(GHConstants.ReplayFileNameSuffix);
                            if (!isGZip && !isZip && !isPlain)
                                continue;

                            items.Add(new ReplayItem(GetReplayDisplayName(fi.Name), fi.FullName));
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }

            ReplayPicker.ItemsSource = items;
            bool hasReplays = items.Count > 0;
            ReplayPicker.IsVisible = hasReplays;
            NoReplaysLabel.IsVisible = !hasReplays;

            string savedPath = Preferences.Get("PerformanceSuiteReplay", "");
            if (!string.IsNullOrEmpty(savedPath))
            {
                for (int i = 0; i < items.Count; i++)
                {
                    if (string.Equals(items[i].FullPath, savedPath, StringComparison.OrdinalIgnoreCase))
                    {
                        ReplayPicker.SelectedIndex = i;
                        break;
                    }
                }
            }
        }

        /* The replay file name without its prefix and suffixes, with the trailing
           timestamp (DateTime.ToBinary, written as an unsigned or, in old names, a
           signed number) shown as a local "yyyy-MM-dd HH:mm" when it decodes to a
           plausible date. */
        private static string GetReplayDisplayName(string fileName)
        {
            string name = fileName ?? "";
            if (name.EndsWith(GHConstants.ReplayGZipFileNameSuffix))
                name = name.Substring(0, name.Length - GHConstants.ReplayGZipFileNameSuffix.Length);
            else if (name.EndsWith(GHConstants.ReplayZipFileNameSuffix))
                name = name.Substring(0, name.Length - GHConstants.ReplayZipFileNameSuffix.Length);
            if (name.EndsWith(GHConstants.ReplayFileNameSuffix))
                name = name.Substring(0, name.Length - GHConstants.ReplayFileNameSuffix.Length);
            if (name.StartsWith(GHConstants.ReplayFileNamePrefix))
                name = name.Substring(GHConstants.ReplayFileNamePrefix.Length);

            int divisorIndex = name.LastIndexOf(GHConstants.ReplayFileNameMiddleDivisor, StringComparison.Ordinal);
            if (divisorIndex > 0 && divisorIndex < name.Length - 1)
            {
                string stampText = name.Substring(divisorIndex + 1);
                int cutIndex = divisorIndex;
                bool negative = name[divisorIndex - 1] == GHConstants.ReplayFileNameMiddleDivisor[0];
                if (negative)
                    cutIndex = divisorIndex - 1;

                long binary = 0;
                bool parsed;
                if (negative)
                {
                    parsed = long.TryParse("-" + stampText, NumberStyles.AllowLeadingSign, CultureInfo.InvariantCulture, out binary);
                }
                else
                {
                    ulong unsignedBinary;
                    parsed = ulong.TryParse(stampText, NumberStyles.None, CultureInfo.InvariantCulture, out unsignedBinary);
                    if (parsed)
                        binary = unchecked((long)unsignedBinary);
                }

                if (parsed && cutIndex > 0)
                {
                    try
                    {
                        DateTime stamp = DateTime.FromBinary(binary).ToLocalTime();
                        if (stamp.Year >= 2000 && stamp.Year < 2200)
                            return name.Substring(0, cutIndex) + " " + stamp.ToString("yyyy-MM-dd HH:mm", CultureInfo.InvariantCulture);
                    }
                    catch (ArgumentException)
                    {
                        /* Not a DateTime binary; keep the name as it is */
                    }
                }
            }
            return name;
        }

        private void Setup_TextChanged(object sender, TextChangedEventArgs e)
        {
            UpdateEstimatedDuration();
        }

        private void Setup_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateEstimatedDuration();
        }

        private void Setup_Toggled(object sender, ToggledEventArgs e)
        {
            UpdateEstimatedDuration();
        }

        private void UpdateEstimatedDuration()
        {
            try
            {
                GHPerformanceSuiteSetup setup = BuildSetupFromFieldsLenient();
                TimeSpan estimate = GHPerformanceSuiteRunner.EstimateDuration(setup);
                EstimatedDurationLabel.Text = FormatDuration(estimate);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
                EstimatedDurationLabel.Text = "-";
            }
        }

        private static string FormatDuration(TimeSpan ts)
        {
            if (ts.TotalHours >= 1)
                return ((int)ts.TotalHours) + "h " + ts.Minutes + "m";
            if (ts.TotalMinutes >= 1)
                return ((int)ts.TotalMinutes) + "m " + ts.Seconds + "s";
            return ((int)ts.TotalSeconds) + "s";
        }

        /* Best-effort setup built from whatever is currently in the fields, used only
           to keep the estimated-duration label live; invalid or missing values fall
           back to defaults rather than blocking the estimate. Run Suite itself performs
           strict validation before building its own setup. */
        private GHPerformanceSuiteSetup BuildSetupFromFieldsLenient()
        {
            GHPerformanceSuiteSetup setup = new GHPerformanceSuiteSetup();
            ReplayItem replay = ReplayPicker.SelectedItem as ReplayItem;
            setup.ReplayPath = replay != null ? replay.FullPath : null;
            setup.StartTurn = ParseIntOrDefault(StartTurnEntry.Text, 1);
            NamedValueItem scenario = ScenarioPicker.SelectedItem as NamedValueItem;
            setup.Scenario = scenario != null ? scenario.Value : "idle";
            RunsItem runsItem = RunsPicker.SelectedItem as RunsItem;
            setup.Runs = runsItem != null ? runsItem.Value : 6;
            setup.ArmLabel = ArmLabelEntry.Text;
            NamedValueItem pageMode = PageModePicker.SelectedItem as NamedValueItem;
            setup.PageMode = pageMode != null ? pageMode.Value : "shared";
            setup.WarmUpRun = WarmUpRunSwitch.IsToggled;
            setup.WarmUpSeconds = ParseIntOrDefault(WarmUpSecondsEntry.Text, 10);
            setup.WindowSeconds = ParseIntOrDefault(WindowSecondsEntry.Text, 60);
            setup.CooldownSeconds = ParseIntOrDefault(CooldownSecondsEntry.Text, 20);
            return setup;
        }

        private static int ParseIntOrDefault(string text, int fallback)
        {
            int value;
            return int.TryParse(text, out value) ? value : fallback;
        }

        private async void btnRunSuite_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();

            ReplayItem replay = ReplayPicker.SelectedItem as ReplayItem;
            if (replay == null)
            {
                await GHApp.DisplayMessageBox(this, "No Replay Selected", "Please select a replay to measure.", "OK");
                return;
            }

            int startTurn;
            if (!int.TryParse(StartTurnEntry.Text, out startTurn) || startTurn < 1)
            {
                await GHApp.DisplayMessageBox(this, "Invalid Start Turn", "Start turn must be a number of at least 1.", "OK");
                return;
            }

            int warmUpSeconds;
            if (!int.TryParse(WarmUpSecondsEntry.Text, out warmUpSeconds) || warmUpSeconds < 0)
            {
                await GHApp.DisplayMessageBox(this, "Invalid Warm-up Seconds", "Warm-up seconds must be zero or a positive number.", "OK");
                return;
            }

            int windowSeconds;
            if (!int.TryParse(WindowSecondsEntry.Text, out windowSeconds) || windowSeconds <= 0)
            {
                await GHApp.DisplayMessageBox(this, "Invalid Window Seconds", "Window seconds must be a positive number.", "OK");
                return;
            }

            int cooldownSeconds;
            if (!int.TryParse(CooldownSecondsEntry.Text, out cooldownSeconds) || cooldownSeconds < 0)
            {
                await GHApp.DisplayMessageBox(this, "Invalid Cool-down Seconds", "Cool-down seconds must be zero or a positive number.", "OK");
                return;
            }

            string label = ArmLabelEntry.Text != null ? ArmLabelEntry.Text.Trim() : "";
            if (string.IsNullOrEmpty(label))
            {
                await GHApp.DisplayMessageBox(this, "Missing Arm Label", "Please enter an arm label.", "OK");
                return;
            }

            NamedValueItem scenarioItem = ScenarioPicker.SelectedItem as NamedValueItem;
            RunsItem runsItem = RunsPicker.SelectedItem as RunsItem;
            NamedValueItem pageModeItem = PageModePicker.SelectedItem as NamedValueItem;
            if (scenarioItem == null || runsItem == null || pageModeItem == null)
            {
                await GHApp.DisplayMessageBox(this, "Incomplete Setup", "Please choose a scenario, a run count, and a page mode.", "OK");
                return;
            }

            GHPerformanceSuiteSetup setup = new GHPerformanceSuiteSetup();
            setup.ReplayPath = replay.FullPath;
            setup.StartTurn = startTurn;
            setup.Scenario = scenarioItem.Value;
            setup.Runs = runsItem.Value;
            setup.ArmLabel = label;
            setup.PageMode = pageModeItem.Value;
            setup.WarmUpRun = WarmUpRunSwitch.IsToggled;
            setup.WarmUpSeconds = warmUpSeconds;
            setup.WindowSeconds = windowSeconds;
            setup.CooldownSeconds = cooldownSeconds;

            SavePreferences(setup, replay.FullPath);

            SetupLayout.IsEnabled = false;
            btnRunSuite.IsEnabled = false;
            string err = null;
            try
            {
                /* onFinished runs on the UI thread once the whole suite (including the
                   game session it pushes on top of this page) has ended and this page
                   is back on top of the modal stack; refresh the results in place
                   instead of pushing another instance of this page. */
                err = await GHPerformanceSuiteRunner.RunAsync(setup, id =>
                {
                    try
                    {
                        RefreshSuiteList(id);
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex);
                    }
                    return Task.CompletedTask;
                });
            }
            finally
            {
                SetupLayout.IsEnabled = true;
                UpdateRunButtonState();
            }

            if (!string.IsNullOrEmpty(err))
            {
                await GHApp.DisplayMessageBox(this, "Cannot Start Performance Suite", err, "OK");
            }
        }

        private void SavePreferences(GHPerformanceSuiteSetup setup, string replayFullPath)
        {
            Preferences.Set("PerformanceSuiteReplay", replayFullPath);
            Preferences.Set("PerformanceSuiteStartTurn", setup.StartTurn);
            Preferences.Set("PerformanceSuiteScenario", setup.Scenario);
            Preferences.Set("PerformanceSuiteRuns", setup.Runs);
            Preferences.Set("PerformanceSuiteWarmUpRun", setup.WarmUpRun);
            Preferences.Set("PerformanceSuitePageMode", setup.PageMode);
            Preferences.Set("PerformanceSuiteLabel", setup.ArmLabel);
            Preferences.Set("PerformanceSuiteLabelDefault", _defaultArmLabel);
            Preferences.Set("PerformanceSuiteWarmUpSeconds", setup.WarmUpSeconds);
            Preferences.Set("PerformanceSuiteWindowSeconds", setup.WindowSeconds);
            Preferences.Set("PerformanceSuiteCooldownSeconds", setup.CooldownSeconds);
        }

        private void UpdateRunButtonState()
        {
            btnRunSuite.IsEnabled = !GHPerformanceSuiteRunner.IsRunning;
        }

        private static PerformanceSuiteListItem BuildListItem(GHPerformanceSuiteInfo info, bool darkMode)
        {
            PerformanceSuiteListItem item = new PerformanceSuiteListItem();
            item.SuiteId = info.SuiteId;
            item.DateText = info.StartedUtc.ToLocalTime().ToString("yyyy-MM-dd HH:mm", CultureInfo.InvariantCulture);
            item.ScenarioText = info.Scenario;
            item.LabelText = info.ArmLabel;
            item.RunsText = info.RunsUsed.ToString(CultureInfo.InvariantCulture) + (info.RunsUsed == 1 ? " run" : " runs");
            item.HitchText = double.IsNaN(info.MedianHitchRatioMsPerSec) ? "n/a" : info.MedianHitchRatioMsPerSec.ToString("0.00", CultureInfo.InvariantCulture) + " ms/s";
            item.SizeText = FormatSize(info.SizeBytes);

            List<string> markers = new List<string>();
            if (info.IsBaseline)
                markers.Add("baseline");
            if (!string.IsNullOrEmpty(info.Status) && info.Status.IndexOf("abort", StringComparison.OrdinalIgnoreCase) >= 0)
                markers.Add("aborted");
            if (!string.IsNullOrEmpty(info.Origin) && info.Origin.IndexOf("import", StringComparison.OrdinalIgnoreCase) >= 0)
                markers.Add("imported");
            if (info.HasBackgroundExclusion)
                markers.Add("bg");
            if (info.EnvironmentChanged)
                markers.Add("env changed");
            item.MarkersText = string.Join(", ", markers);
            item.HasMarkers = markers.Count > 0;

            item.ItemTextColor = darkMode ? GHColors.White : GHColors.Black;
            /* Windows draws a selection check box inside the row's left edge and the list's
               scroll bar over its right edge */
            item.RowPadding = GHApp.IsWindows ? new Thickness(34, 5, 20, 5) : new Thickness(4, 5, 10, 5);
            return item;
        }

        private static string FormatSize(long bytes)
        {
            if (bytes >= 1024 * 1024)
                return (bytes / (1024.0 * 1024.0)).ToString("0.0", CultureInfo.InvariantCulture) + " MB";
            if (bytes >= 1024)
                return (bytes / 1024.0).ToString("0.0", CultureInfo.InvariantCulture) + " KB";
            return bytes.ToString(CultureInfo.InvariantCulture) + " B";
        }

        /* Rebuilds the results list from the store and, when selectSuiteId is given,
           selects and scrolls to that suite (used after a run finishes and after
           Set Baseline, so the affected suite stays visible). */
        private void RefreshSuiteList(string selectSuiteId)
        {
            bool darkMode = GHApp.DarkMode;
            List<GHPerformanceSuiteInfo> suites;
            try
            {
                suites = GHPerformanceSuiteStore.ListSuites();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
                suites = new List<GHPerformanceSuiteInfo>();
            }

            List<PerformanceSuiteListItem> items = new List<PerformanceSuiteListItem>();
            foreach (GHPerformanceSuiteInfo info in suites)
                items.Add(BuildListItem(info, darkMode));

            SuiteCollectionView.SelectedItems.Clear();
            SuiteCollectionView.ItemsSource = items;

            bool hasSuites = items.Count > 0;
            NoSuitesLabel.IsVisible = !hasSuites;
            SuiteCollectionView.IsVisible = hasSuites;

            if (!string.IsNullOrEmpty(selectSuiteId))
            {
                foreach (PerformanceSuiteListItem item in items)
                {
                    if (item.SuiteId == selectSuiteId)
                    {
                        SuiteCollectionView.SelectedItems.Add(item);
                        SuiteCollectionView.ScrollTo(item, position: ScrollToPosition.MakeVisible, animate: false);
                        break;
                    }
                }
            }

            UpdateResultButtons();
            UpdateRunButtonState();
        }

        private void SuiteCollectionView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            UpdateResultButtons();
        }

        private void UpdateResultButtons()
        {
            int count = SuiteCollectionView.SelectedItems != null ? SuiteCollectionView.SelectedItems.Count : 0;
            bool exactlyOne = count == 1;
            bool atLeastOne = count >= 1;
            btnViewReport.IsEnabled = exactlyOne;
            btnSetBaseline.IsEnabled = exactlyOne;
            btnCompareBaseline.IsEnabled = exactlyOne;
            btnShare.IsEnabled = atLeastOne;
            btnDelete.IsEnabled = atLeastOne;
        }

        private PerformanceSuiteListItem SelectedSingleItem()
        {
            if (SuiteCollectionView.SelectedItems == null || SuiteCollectionView.SelectedItems.Count != 1)
                return null;
            return SuiteCollectionView.SelectedItems[0] as PerformanceSuiteListItem;
        }

        private List<string> SelectedSuiteIds()
        {
            List<string> ids = new List<string>();
            if (SuiteCollectionView.SelectedItems != null)
            {
                foreach (object obj in SuiteCollectionView.SelectedItems)
                {
                    PerformanceSuiteListItem item = obj as PerformanceSuiteListItem;
                    if (item != null)
                        ids.Add(item.SuiteId);
                }
            }
            return ids;
        }

        private async void btnViewReport_Clicked(object sender, EventArgs e)
        {
            PerformanceSuiteListItem item = SelectedSingleItem();
            if (item == null)
                return;
            ResultButtonsLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            try
            {
                string path = GHPerformanceSuiteStore.WriteReport(item.SuiteId);
                if (string.IsNullOrEmpty(path))
                {
                    await GHApp.DisplayMessageBox(this, "Report Failed", "GnollHack could not write a report for this suite.", "OK");
                }
                else
                {
                    var displFilePage = new DisplayFilePage(path, "Performance Report - " + item.LabelText, GHPerformanceTextReport.MaxLineWidth, true);
                    string errormsg;
                    if (!displFilePage.ReadFile(out errormsg))
                        await GHApp.DisplayMessageBox(this, "Error Opening Report", "GnollHack cannot open the report file: " + errormsg, "OK");
                    else
                        await GHApp.PushModalPageAsync(displFilePage);
                }
            }
            finally
            {
                ResultButtonsLayout.IsEnabled = true;
                UpdateResultButtons();
            }
        }

        private async void btnSetBaseline_Clicked(object sender, EventArgs e)
        {
            PerformanceSuiteListItem item = SelectedSingleItem();
            if (item == null)
                return;
            ResultButtonsLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            bool ok = false;
            try
            {
                ok = GHPerformanceSuiteStore.SetBaseline(item.SuiteId);
                if (!ok)
                    await GHApp.DisplayMessageBox(this, "Set Baseline Failed", "GnollHack could not set this suite as the baseline.", "OK");
            }
            finally
            {
                ResultButtonsLayout.IsEnabled = true;
                RefreshSuiteList(item.SuiteId);
            }
        }

        private async void btnCompareBaseline_Clicked(object sender, EventArgs e)
        {
            PerformanceSuiteListItem item = SelectedSingleItem();
            if (item == null)
                return;
            ResultButtonsLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            try
            {
                GHComparisonResult result;
                string reportText;
                string refusal;
                if (!GHPerformanceSuiteStore.TryCompareWithBaseline(item.SuiteId, out result, out reportText, out refusal))
                {
                    await GHApp.DisplayMessageBox(this, "Cannot Compare with Baseline", string.IsNullOrEmpty(refusal) ? "The comparison could not be produced." : refusal, "OK");
                }
                else
                {
                    string archiveDir = Path.Combine(GHApp.GHPath, GHConstants.ArchiveDirectory);
                    GHApp.CheckCreateDirectory(archiveDir);
                    string filePath = Path.Combine(archiveDir, "comparison_" + item.SuiteId + ".txt");
                    File.WriteAllText(filePath, reportText, new UTF8Encoding(false));

                    var displFilePage = new DisplayFilePage(filePath, "Comparison - " + item.LabelText, GHPerformanceTextReport.MaxLineWidth, true);
                    string errormsg;
                    if (!displFilePage.ReadFile(out errormsg))
                        await GHApp.DisplayMessageBox(this, "Error Opening Comparison", "GnollHack cannot open the comparison file: " + errormsg, "OK");
                    else
                        await GHApp.PushModalPageAsync(displFilePage);
                }
            }
            catch (Exception ex)
            {
                await GHApp.DisplayMessageBox(this, "Cannot Compare with Baseline", "An error occurred while comparing with the baseline: " + ex.Message, "OK");
            }
            finally
            {
                ResultButtonsLayout.IsEnabled = true;
                UpdateResultButtons();
            }
        }

        private async void btnShare_Clicked(object sender, EventArgs e)
        {
            List<string> ids = SelectedSuiteIds();
            if (ids.Count == 0)
                return;
            ResultButtonsLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.CheckAndRequestWritePermission(this);
            await GHApp.CheckAndRequestReadPermission(this);
            try
            {
                string zip = GHPerformanceSuiteStore.BuildShareZip(ids);
                if (string.IsNullOrEmpty(zip))
                    await GHApp.DisplayMessageBox(this, "Share Failed", "GnollHack could not build a zip archive of the selected performance results.", "OK");
                else
                    await GHApp.ShareFile(this, zip, "GnollHack Performance Results");
            }
            catch (Exception ex)
            {
                await GHApp.DisplayMessageBox(this, "Share Failed", "GnollHack failed to share performance results: " + ex.Message, "OK");
            }
            finally
            {
                ResultButtonsLayout.IsEnabled = true;
                UpdateResultButtons();
            }
        }

        private async void btnDelete_Clicked(object sender, EventArgs e)
        {
            List<string> ids = SelectedSuiteIds();
            if (ids.Count == 0)
                return;
            GHApp.PlayButtonClickedSound();
            bool answer = await GHApp.DisplayMessageBox(this, "Delete Performance Results?",
                "Are you sure you want to delete " + ids.Count + " performance suite" + (ids.Count == 1 ? "" : "s") + "?", "Yes", "No");
            if (!answer)
                return;

            ResultButtonsLayout.IsEnabled = false;
            int deleted = 0;
            try
            {
                foreach (string id in ids)
                {
                    if (GHPerformanceSuiteStore.DeleteSuite(id))
                        deleted++;
                }
                if (deleted < ids.Count)
                    await GHApp.DisplayMessageBox(this, "Delete Incomplete", "Deleted " + deleted + " of " + ids.Count + " selected suites.", "OK");
            }
            finally
            {
                ResultButtonsLayout.IsEnabled = true;
                RefreshSuiteList(null);
            }
        }

        private async void btnImportResults_Clicked(object sender, EventArgs e)
        {
            ResultButtonsLayout.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.CheckAndRequestWritePermission(this);
            await GHApp.CheckAndRequestReadPermission(this);
            try
            {
                FileResult file = await FilePicker.PickAsync();
                if (file != null)
                {
                    GHPerformanceImportSummary summary = GHPerformanceSuiteStore.ImportZip(file.FullPath);
                    string message = "Imported " + summary.Imported.Count + ", skipped " + summary.Skipped.Count + " (already present).";
                    if (summary.Errors.Count > 0)
                        message += Environment.NewLine + summary.Errors.Count + " error" + (summary.Errors.Count == 1 ? "" : "s") + ":" + Environment.NewLine + string.Join(Environment.NewLine, summary.Errors);
                    await GHApp.DisplayMessageBox(this, "Import Results", message, "OK");
                    RefreshSuiteList(null);
                }
            }
            catch (Exception ex)
            {
                await GHApp.DisplayMessageBox(this, "Import Failed", "An error occurred while trying to import performance results: " + ex.Message, "OK");
            }
            finally
            {
                ResultButtonsLayout.IsEnabled = true;
                UpdateResultButtons();
            }
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            await ClosePageAsync(true);
        }

        public void ClosePage()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (PerformanceSuiteGrid.IsEnabled)
                            await ClosePageAsync(true);
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex);
                    }

                });
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }

        private async Task ClosePageAsync(bool playClickSound)
        {
            PerformanceSuiteGrid.IsEnabled = false;
            _backPressed = true;
            if (playClickSound)
                GHApp.PlayButtonClickedSound();
            await GHApp.PopModalPageAsync();
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
            PerformanceSuiteGrid.IsEnabled = true;
            RefreshSuiteList(_pendingSelectSuiteId);
            _pendingSelectSuiteId = null;
            WarmEnvironmentCache();
        }

        /* The first environment capture can block for seconds (WMI); running it once on
           the thread pool lets the later captures on the UI thread (the suite manifest,
           the run records, the share zip) read the cache */
        private static void WarmEnvironmentCache()
        {
            try
            {
                Task.Run(delegate { GHPerformanceEnvironment.CaptureFingerprint(false); });
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
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

        /* Recursively recolors black-on-light setup text (labels, entries, pickers) for
           dark mode, matching SettingsPage's SetChildrenDarkModeTextColor, and gives entries
           and pickers the shaded input background of the current mode. */
        private void SetChildrenDarkModeTextColor(View view, bool darkmode)
        {
            if (view == null)
                return;

            if (view is Grid)
            {
                Grid grid = (Grid)view;
                foreach (View child in grid.Children)
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
                if (darkmode ? l.TextColor == GHColors.Black : l.TextColor == GHColors.White)
                    l.TextColor = darkmode ? GHColors.White : GHColors.Black;
            }
#if GNH_MAUI
            else if (view is Microsoft.Maui.Controls.Entry)
            {
                Microsoft.Maui.Controls.Entry l = (Microsoft.Maui.Controls.Entry)view;
                if (darkmode ? l.TextColor == GHColors.Black : l.TextColor == GHColors.White)
                    l.TextColor = darkmode ? GHColors.White : GHColors.Black;
                l.BackgroundColor = darkmode ? GHColors.PickerDarkModeBkgColor : GHColors.PickerLightModeBkgColor;
            }
            else if (view is Microsoft.Maui.Controls.Picker)
            {
                Microsoft.Maui.Controls.Picker l = (Microsoft.Maui.Controls.Picker)view;
                l.TextColor = darkmode ? GHColors.White : GHColors.Black;
                l.TitleColor = darkmode ? GHColors.White : GHColors.Black;
                l.BackgroundColor = darkmode ? GHColors.PickerDarkModeBkgColor : GHColors.PickerLightModeBkgColor;
            }
#else
            else if (view is Xamarin.Forms.Entry)
            {
                Xamarin.Forms.Entry l = (Xamarin.Forms.Entry)view;
                if (darkmode ? l.TextColor == GHColors.Black : l.TextColor == GHColors.White)
                    l.TextColor = darkmode ? GHColors.White : GHColors.Black;
                l.BackgroundColor = darkmode ? GHColors.PickerDarkModeBkgColor : GHColors.PickerLightModeBkgColor;
            }
            else if (view is Xamarin.Forms.Picker)
            {
                Xamarin.Forms.Picker l = (Xamarin.Forms.Picker)view;
                l.TextColor = darkmode ? GHColors.White : GHColors.Black;
                l.TitleColor = darkmode ? GHColors.White : GHColors.Black;
                l.BackgroundColor = darkmode ? GHColors.PickerDarkModeBkgColor : GHColors.PickerLightModeBkgColor;
            }
#endif
        }

        public bool IsPopupOpen => MessagePopup.IsPopupOpen;
        public void ClosePopup() => MessagePopup.ClosePopup();
        public bool SendKeyToPopup(int key, bool isCtrl, bool isMeta) => MessagePopup.SendKeyToPopup(key, isCtrl, isMeta);
        public bool SendSpecialKeyToPopup(GHSpecialKey spkey, bool isCtrl, bool isMeta, bool isShift) => MessagePopup.SendSpecialKeyToPopup(spkey, isCtrl, isMeta, isShift);
        public Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null,
#if GNH_MAUI
            Color titleColor = null,
#else
            Color? titleColor = null,
#endif
             bool acceptEnterSpaceForOkCancel = false) => MessagePopup.ShowMessagePopupAsync(title, message, okButtonText, cancelButtonText, titleColor, acceptEnterSpaceForOkCancel);
    }
}
