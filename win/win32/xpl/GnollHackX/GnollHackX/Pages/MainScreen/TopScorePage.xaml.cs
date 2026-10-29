using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Controls.PlatformConfiguration.iOSSpecific;

namespace GnollHackM
#else
using GnollHackX.Controls;
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    public partial class TopScorePage : CustomModalPage, ICloseablePage, IGHGamepadHandlingPage, IMessagePopupPage, IGHGamepadPopupPage
    {
        private string _fileName;
        private ObservableCollection<GHTopScoreItem> _topScores = null;

        public TopScorePage(string fileName)
        {
            InitializeComponent();
//#if GNH_MAUI
//            SafeAreaEdges = SafeAreaEdges.All;
//#else
//            On<iOS>().SetUseSafeArea(true);
//#endif
            //UIUtils.AdjustRootLayout(RootGrid);
            //UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            //UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

            _fileName = fileName;
            ScoresView.BindingContext = this;
            MainGrid.BindingContext = this;
            if(GHApp.DarkMode)
            {
                HeaderLabel.TextColor = GHColors.White;
                NoScoresLabel.TextColor = GHColors.White;
                foreach(View view in HeaderGrid.Children)
                {
                    if(view is Label)
                    {
                        Label l = (Label)view;
                        l.TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    }
                }
            }
            if(GHApp.HasInternetAccess)
            {
                CloseButton.IsVisible = false;
                CloseGrid.IsVisible = true;
            }
            InitializeGamepadNavigation();
        }

        public TopScorePage()
        {
            InitializeComponent();
//#if GNH_MAUI
//            SafeAreaEdges = SafeAreaEdges.All;
//#else
//            On<iOS>().SetUseSafeArea(true);
//#endif
//            UIUtils.AdjustRootLayout(RootGrid);
            //UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            //UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

            _fileName = "";
            NoScoresLabel.IsVisible = true;
            ScoresView.IsVisible = false;
            ScoresView.BindingContext = this;
            InitializeGamepadNavigation();
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            await ClosePageAsync(true);
        }

        private async Task ClosePageAsync(bool playClickSound)
        {
            CloseButton.IsEnabled = false;
            _backPressed = true;
            if (playClickSound)
                GHApp.PlayButtonClickedSound();
            await GHApp.PopModalPageAsync();
        }

        public void ClosePage()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (MessagePopup.IsPopupOpen)
                            MessagePopup.ClosePopup();
                        else if (CloseButton.IsEnabled)
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
        public bool ReadFile(out string errorMessage)
        {
            string res = "";
            try
            {
                string[] lines = File.ReadAllLines(_fileName);
                List<GHTopScoreItem> newTopScores = new List<GHTopScoreItem>();
                foreach (string line in lines)
                {
                    GHTopScoreItem tsi = new GHTopScoreItem(line);
                    newTopScores.Add(tsi);
                }

                /* Sort top scores */
                var q = from s in newTopScores orderby s.Score descending /* where (GHApp.DeveloperMode || s.Mode == "normal") */ select s;
                List<GHTopScoreItem> sortedList = q.ToList();
                _topScores = new ObservableCollection<GHTopScoreItem>();
                foreach (GHTopScoreItem sorteditem in sortedList)
                    _topScores.Add(sorteditem);

                int rank = 1;
                int rankadd = 0;
                for (int i = 0; i < _topScores.Count; i++)
                {
                    if (i > 0 && _topScores[i - 1].Score > _topScores[i].Score)
                    {
                        rank += rankadd;
                        rankadd = 1;
                    }
                    else
                        rankadd++;

                    _topScores[i].Rank = rank;
                }

                /* Display in collection view */
                ScoresView.ItemsSource = _topScores;
                NoScoresLabel.IsVisible = (_topScores.Count == 0);
                ScoresView.IsVisible = (_topScores.Count != 0);
            }
            catch (Exception e)
            {
                errorMessage = e.Message;
                return false;
            }
            errorMessage = res;
            return true;
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

                HeaderLabel.Margin = UIUtils.GetHeaderMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorderWithTop(bkgView.BorderStyle, width, height, 20.0);
                double bordermargin = UIUtils.GetBorderWidth(bkgView.BorderStyle, width, height);
                ScoresView.Margin = new Thickness(bordermargin + 6, 0, bordermargin + 6, 0);

                if (ScoresView.ItemsSource != null)
                {
                    ScoresView.ItemsSource = null;
                    ScoresView.ItemsSource = _topScores;
                }
            }
        }

        private double AvailableWidth { get { return Width - ScoresView.Margin.Left - ScoresView.Margin.Right; } }

        private bool _raceVisible = false;
        public bool IsRaceVisible
        {
            get
            {
                bool newvalue = (AvailableWidth >= 620);
                if (_raceVisible != newvalue)
                {
                    _raceVisible = newvalue;
                    OnPropertyChanged("IsRaceVisible");
                }
                return _raceVisible;
            }
        }

        private bool _genderVisible = false;
        public bool IsGenderVisible
        {
            get
            {
                bool newvalue = (AvailableWidth >= 680);
                if (_genderVisible != newvalue)
                {
                    _genderVisible = newvalue;
                    OnPropertyChanged("IsGenderVisible");
                }
                return _genderVisible;
            }
        }

        private bool _alignmentVisible = false;
        public bool IsAlignmentVisible
        {
            get
            {
                bool newvalue = (AvailableWidth >= 730);
                if (_alignmentVisible != newvalue)
                {
                    _alignmentVisible = newvalue;
                    OnPropertyChanged("IsAlignmentVisible");
                }
                return _alignmentVisible;
            }
        }

        private bool _hpVisible = false;
        public bool IsHPVisible
        {
            get
            {
                bool newvalue = (AvailableWidth >= 840);
                if (_hpVisible != newvalue)
                {
                    _hpVisible = newvalue;
                    OnPropertyChanged("IsHPVisible");
                }
                return _hpVisible;
            }
        }

        private bool _hpMaxVisible = false;
        public bool IsHPMaxVisible
        {
            get
            {
                bool newvalue = (AvailableWidth >= 780);
                if (_hpMaxVisible != newvalue)
                {
                    _hpMaxVisible = newvalue;
                    OnPropertyChanged("IsHPMaxVisible");
                }
                return _hpMaxVisible;
            }
        }

        private bool _turnsVisible = false;
        public bool IsTurnsVisible
        {
            get
            {
                bool newvalue = (AvailableWidth >= 905);
                if (_turnsVisible != newvalue)
                {
                    _turnsVisible = newvalue;
                    OnPropertyChanged("IsTurnsVisible");
                }
                return _turnsVisible;
            }
        }

        private bool _birthDateVisible = false;
        public bool IsBirthDateVisible
        {
            get
            {
                bool newvalue = (AvailableWidth >= 990);
                if (_birthDateVisible != newvalue)
                {
                    _birthDateVisible = newvalue;
                    OnPropertyChanged("IsBirthDateVisible");
                }
                return _birthDateVisible;
            }
        }

        private bool _outcomeVisible = false;
        public bool IsOutcomeVisible
        {
            get
            {
                bool newvalue = (AvailableWidth >= 570);
                if (_outcomeVisible != newvalue)
                {
                    _outcomeVisible = newvalue;
                    OnPropertyChanged("IsOutcomeVisible");
                }
                return _outcomeVisible;
            }
        }

        public async Task<bool> OpenFileInLauncher(string fullPath)
        {
            try
            {
                await Launcher.OpenAsync(new OpenFileRequest
                {
                    File = new ReadOnlyFile(fullPath)
                }); 
                return true;
            }
            catch (Exception ex)
            {
                await GHApp.DisplayMessageBox(this, "Cannot Open File", "GnollHack cannot open the file at " + fullPath + " in launcher. Error: " + ex.Message, "OK");
                return false;
            }
        }


        private async void Button_Clicked(object sender, EventArgs e)
        {
            Label btn = sender as Label;
            GHTopScoreItem tsi = null;
            if (btn != null)
                tsi = btn.BindingContext as GHTopScoreItem;

            await OpenDumplogAsync(tsi);
        }

        /* Shows the dumplog of tsi, the HTML one if preferred and present */
        private async Task OpenDumplogAsync(GHTopScoreItem tsi)
        {
            GHApp.PlayButtonClickedSound();
            ScoresView.IsEnabled = false;

            if (tsi != null)
            {
                string fulltargetpath = Path.Combine(GHApp.GHPath, GHConstants.DumplogDirectory, tsi.GetDumplogFileName());
                string fullhtmltargetpath = Path.Combine(GHApp.GHPath, GHConstants.DumplogDirectory, tsi.GetHTMLDumplogFileName());
                bool dumplogexists = File.Exists(fulltargetpath);
                bool htmldumplogexists = File.Exists(fullhtmltargetpath);

                bool HTMLDumplogDisplayed = false;
                try
                {
                    if(GHApp.UseHTMLDumpLogs && htmldumplogexists)
                    {
                        bool openhtml = true;
                        if (dumplogexists && htmldumplogexists && !GHApp.UseSingleDumpLog)
                            openhtml = await GHApp.DisplayMessageBox(this, "Open HTML DumpLog", "There are both text and HTML dumplogs available. Do you want to open the HTML dumplog?", "Yes", "No");
                        if (openhtml)
                        {
                            //HTMLDumplogDisplayed = await OpenFileInLauncher(fullhtmltargetpath);
                            var displFilePage = new DisplayFilePage(fullhtmltargetpath, "Dumplog - " + tsi.Name, 0, true, true, false);
                            string errormsg = "";
                            if (!displFilePage.ReadFile(out errormsg))
                            {
                                await GHApp.DisplayMessageBox(this, "Error Reading HTML Dumplog File", errormsg, "OK");
                            }
                            else
                            {
                                await GHApp.PushModalPageAsync(displFilePage);
                                HTMLDumplogDisplayed = true;
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    await GHApp.DisplayMessageBox(this, "Error Reading HTML Dumplog File", "An error occurred when reading HTML dumplog \'" + fullhtmltargetpath + "\' for " + tsi.Name + ": " + ex.Message, "OK");
                }

                try
                {
                    if (!HTMLDumplogDisplayed)
                    {
                        if (dumplogexists)
                        {
                            var displFilePage = new DisplayFilePage(fulltargetpath, "Dumplog - " + tsi.Name, 0, true);
                            string errormsg = "";
                            if (!displFilePage.ReadFile(out errormsg))
                            {
                                await GHApp.DisplayMessageBox(this, "Error Reading Dumplog File", errormsg, "OK");
                            }
                            else
                            {
                                await GHApp.PushModalPageAsync(displFilePage);
                            }
                        }
                        else
                        {
                            await GHApp.DisplayMessageBox(this, "No Dumplog", "Dumplog \'" + fulltargetpath + "\' for " + tsi.Name + " does not exist.", "OK");
                        }
                    }
                }
                catch (Exception ex)
                {
                    await GHApp.DisplayMessageBox(this, "Error Reading Dumplog File", "An error occurred when reading dumplog \'" + fulltargetpath + "\' for " + tsi.Name + ": " + ex.Message, "OK");
                }
            }
            else
            {
                await GHApp.DisplayMessageBox(this, "Top Score Info Missing", "Selected top score information does not exist.", "OK");
            }
            ScoresView.IsEnabled = true;
        }

        private async void ServerButton_Clicked(object sender, EventArgs e)
        {
            ServerButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, "Top Scores", new Uri(GHApp.XlogTopScoreAddress));
            ServerButton.IsEnabled = true;
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (MessagePopup.IsPopupOpen)
            {
                MessagePopup.ClosePopup();
                return false;
            }
            if (!_backPressed)
            {
                await ClosePageAsync(false);
            }
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
            if (GHGamepadService.IsActive)
                EnsureGamepadListSelectionMode();
            _gamepadNavigator.Attach();
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            _gamepadNavigator.Detach();
        }

        /* Gamepad focus: the score list, whose Up/Down move the selected row and A opens its dumplog, then Close
           or Server and Close; B closes the page. The first focus is set when the page first appears, after
           ReadFile has filled the list. */
        private readonly GHGamepadPageNavigator _gamepadNavigator = new GHGamepadPageNavigator();

        private void InitializeGamepadNavigation()
        {
            GHGamepadPageNavigator.Layer main = _gamepadNavigator.Main;
            main.AddCollectionView(ScoresView, () => _topScores, GamepadOpenDumplog);
            main.AddButton(CloseButton);
            main.AddButton(ServerButton);
            main.AddButton(CloseButton2);
            main.HorizontalMoves = true;
            main.SetInitialFocus(ScoresView, CloseButton, CloseButton2);
            main.Back = () =>
            {
                ClosePage();
                return true;
            };
        }

        /* A on the list: rows without a dumplog have no tap target either */
        private async void GamepadOpenDumplog(object item)
        {
            try
            {
                GHTopScoreItem tsi = item as GHTopScoreItem;
                if (tsi != null && tsi.IsDumplogButtonEnabled && ScoresView.IsEnabled)
                    await OpenDumplogAsync(tsi);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }

        /* The list shows the gamepad focus only as its selection, which needs SelectionMode Single; it is set on the
           first gamepad input, so touch keeps the list without selection */
        private void EnsureGamepadListSelectionMode()
        {
            if (ScoresView.IsVisible && ScoresView.SelectionMode != SelectionMode.Single)
                ScoresView.SelectionMode = SelectionMode.Single;
        }

        /* Selects the first row when the list has the gamepad focus without a selected row */
        private void EnsureGamepadListSelection()
        {
            if (!GHGamepadService.IsActive || _topScores == null || _topScores.Count == 0)
                return;
            if (!ReferenceEquals(_gamepadNavigator.Main.Focus.FocusedView, ScoresView)
                || !GHGamepadPageNavigator.IsViewAvailable(ScoresView))
                return;
            object selected = ScoresView.SelectedItem;
            if (selected != null && _topScores.Contains(selected as GHTopScoreItem))
                return;
            ScoresView.SelectedItem = _topScores[0];
            try
            {
                ScoresView.ScrollTo(0, -1, ScrollToPosition.MakeVisible, false);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }

        public bool HandleGamepadButton(GHGamepadButtonEvent e)
        {
            EnsureGamepadListSelectionMode();
            bool handled = _gamepadNavigator.HandleButton(e);
            EnsureGamepadListSelection();
            return handled;
        }

        public void UpdateGamepadSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            _gamepadNavigator.UpdateSticks(sticks, dtSeconds);
        }

        public bool SendGamepadButtonToPopup(GHGamepadButtonEvent e) => MessagePopup.HandleGamepadButton(e);

        public bool IsPopupOpen => MessagePopup.IsPopupOpen;
        public void ClosePopup() => MessagePopup.ClosePopup();
        public bool SendKeyToPopup(int key, bool isCtrl, bool isMeta) => MessagePopup.SendKeyToPopup(key, isCtrl, isMeta);
        public bool SendSpecialKeyToPopup(GHSpecialKey spkey, bool isCtrl, bool isMeta, bool isShift) => MessagePopup.SendSpecialKeyToPopup(spkey, isCtrl, isMeta, isShift);
        public Task<bool> ShowMessagePopupAsync(string title, string message, string okButtonText, string cancelButtonText = null,
#if GNH_MAUI
            Microsoft.Maui.Graphics.Color titleColor = null, bool acceptEnterSpaceForOkCancel = false
#else
            Color? titleColor = null, bool acceptEnterSpaceForOkCancel = false
#endif
            ) => MessagePopup.ShowMessagePopupAsync(title, message, okButtonText, cancelButtonText, titleColor, acceptEnterSpaceForOkCancel);
    }
}