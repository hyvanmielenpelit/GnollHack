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
using Xamarin.Essentials;
using Xamarin.Forms;
using Xamarin.Forms.PlatformConfiguration;
using Xamarin.Forms.PlatformConfiguration.iOSSpecific;
using Xamarin.Forms.Xaml;

namespace GnollHackX.Pages.MainScreen
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class TopScorePage : ContentPage
    {
        private string _fileName;
        private ObservableCollection<GHTopScoreItem> _topScores = null;

        public TopScorePage(string fileName)
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            GHApp.SetPageThemeOnHandler(this, GHApp.DarkMode);
            GHApp.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

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
        }

        public TopScorePage()
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            GHApp.SetPageThemeOnHandler(this, GHApp.DarkMode);
            GHApp.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);

            _fileName = "";
            NoScoresLabel.IsVisible = true;
            ScoresView.IsVisible = false;
            ScoresView.BindingContext = this;
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
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
                    GHTopScoreItem tsi = new GHTopScoreItem(this, line);
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
                ScoresView.Margin = new Thickness(bordermargin, 0, bordermargin, 0);

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
                await DisplayAlert("Cannot Open File", "GnollHack cannot open the file at " + fullPath + " in launcher. Error: " + ex.Message, "OK");
                return false;
            }
        }


        private async void Button_Clicked(object sender, EventArgs e)
        {
            GHApp.PlayButtonClickedSound();
            ScoresView.IsEnabled = false;

            Label btn = sender as Label;
            GHTopScoreItem tsi = null;
            if (btn != null)
                tsi = btn.BindingContext as GHTopScoreItem;

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
                            openhtml = await DisplayAlert("Open HTML DumpLog", "There are both text and HTML dumplogs available. Do you want to open the HTML dumplog?", "Yes", "No");
                        if (openhtml)
                        {
                            //HTMLDumplogDisplayed = await OpenFileInLauncher(fullhtmltargetpath);
                            var displFilePage = new DisplayFilePage(fullhtmltargetpath, "Dumplog - " + tsi.Name, 0, true, true, false);
                            string errormsg = "";
                            if (!displFilePage.ReadFile(out errormsg))
                            {
                                await DisplayAlert("Error Reading HTML Dumplog File", errormsg, "OK");
                            }
                            else
                            {
                                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
                                HTMLDumplogDisplayed = true;
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    await DisplayAlert("Error Reading HTML Dumplog File", "An error occurred when reading HTML dumplog \'" + fullhtmltargetpath + "\' for " + tsi.Name + ": " + ex.Message, "OK");
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
                                await DisplayAlert("Error Reading Dumplog File", errormsg, "OK");
                            }
                            else
                            {
                                await App.Current.MainPage.Navigation.PushModalAsync(displFilePage);
                            }
                        }
                        else
                        {
                            await DisplayAlert("No Dumplog", "Dumplog \'" + fulltargetpath + "\' for " + tsi.Name + " does not exist.", "OK");
                        }
                    }
                }
                catch (Exception ex)
                {
                    await DisplayAlert("Error Reading Dumplog File", "An error occurred when reading dumplog \'" + fulltargetpath + "\' for " + tsi.Name + ": " + ex.Message, "OK");
                }
            }
            else
            {
                await DisplayAlert("Top Score Info Missing", "Selected top score information does not exist.", "OK");
            }
            ScoresView.IsEnabled = true;
        }

        private async void ServerButton_Clicked(object sender, EventArgs e)
        {
            ServerButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await GHApp.OpenBrowser(this, new Uri(GHApp.XlogTopScoreAddress));
            ServerButton.IsEnabled = true;
        }
    }
}