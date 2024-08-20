using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
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
    public partial class SnapshotPage : ContentPage
    {
        public SnapshotPage()
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
        }

        List<GHSnapshot> _snapshots = new List<GHSnapshot>();

        public void LoadSnapshots()
        {
            _snapshots.Clear();
            string datadir = Path.Combine(GHApp.GHPath, GHConstants.SnapshotDirectory);
            if (Directory.Exists(datadir))
            {
                string[] files = Directory.GetFiles(datadir);
                foreach (string file in files)
                {
                    bool fileexists = File.Exists(file);
                    FileInfo fileinfo = new FileInfo(file);
                    string suffix = ".json";
                    if (fileinfo.Name.Length > suffix.Length && fileinfo.Name.EndsWith(suffix) && fileexists)
                    {
                        GHSnapshot snapshot = null;
                        try
                        {
                            using (FileStream fs = File.OpenRead(file))
                            {
                                using (StreamReader sr = new StreamReader(fs))
                                {
                                    string json = sr.ReadToEnd();
                                    snapshot = JsonConvert.DeserializeObject<GHSnapshot>(json);
                                }
                            }
                        }
                        catch (Exception ex)
                        {
                            System.Diagnostics.Debug.WriteLine(ex);
                        }
                        if (snapshot != null)
                            _snapshots.Add(snapshot);
                    }
                }
            }

            lblSubtitle.Text = _snapshots.Count + " snapshot" + (_snapshots.Count != 1 ? "s" : "") + " taken";
            if (_snapshots.Count > 0)
            {
                Comparison<GHSnapshot> comp = new Comparison<GHSnapshot>((snap1, snap2) => { return (int)(snap1.timestamp - snap2.timestamp); });
                _snapshots.Sort(comp);
                int i = -1;
                foreach (GHSnapshot snap in _snapshots)
                {
                    i++;
                    RowImageButton rib = new RowImageButton();
                    if(GHApp.Glyph2Tile != null && GHApp._tileMap[0] != null && snap.gnh_compatibility <= GHApp.GHVersionNumber)
                    {
                        GlyphImageSource gis = new GlyphImageSource();
                        gis.Glyph = snap.gui_glyph;
                        gis.AutoSize = true;
                        rib.ImgGlyphImageSource = gis;
                    }
                    else
                    {
                        rib.ImgSourcePath = "resource://" + GHApp.AppResourceName + ".Assets.UI.you.png";
                    }
                    rib.ImgHighFilterQuality = true;
                    rib.LblText = snap.name;
                    rib.LblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.LblFontSize = 19;
                    rib.SubLblText = snap.character;
                    rib.SubLbl2Text = snap.location;
                    rib.SubLbl3Text = snap.gamemode;
                    rib.SubLbl4Text =  snap.timing;
                    rib.SubLblTextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.SubLbl2TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.SubLbl3TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.SubLbl4TextColor = GHApp.DarkMode ? GHColors.White : GHColors.Black;
                    rib.SubLblFontSize = 15;
                    rib.SubLbl2FontSize = 15;
                    rib.SubLbl3FontSize = 15;
                    rib.SubLbl4FontSize = 15;
                    rib.IsSubLblVisible = true;
                    rib.IsSubLbl2Visible = true;
                    rib.IsSubLbl3Visible = true;
                    rib.IsSubLbl4Visible = true;
                    rib.ImgWidth = 80;
                    rib.ImgHeight = 80;
                    rib.GridWidth = 480;
                    rib.GridHeight = 80;
#if GNH_MAUI
                    rib.MaximumWidthRequest = 480;
#else
                    rib.WidthRequest = 480;
#endif
                    rib.HeightRequest = 80;
                    rib.GridMargin = new Thickness(rib.ImgWidth / 15, 0);
                    rib.BtnCommand = i;
                    rib.BtnClicked += SnapshotButton_Clicked;
                    SnapshotLayout.Children.Add(rib);
                }
            }
            else
                EmptyLabel.IsVisible = true;

        }

        private async void SnapshotButton_Clicked(object sender, EventArgs e)
        {
            SnapshotLayout.IsEnabled = false;
            RowImageButton ghbutton = sender as RowImageButton;
            if (ghbutton != null)
            {
                if (ghbutton.BtnCommand >= 0 && ghbutton.BtnCommand < _snapshots.Count)
                {
                    GHSnapshot snap = _snapshots[ghbutton.BtnCommand];
                    string logdir = Path.GetDirectoryName(snap.dumplog);
                    string htmldir = Path.GetDirectoryName(snap.dumphtml);
                    string logfile = Path.GetFileName(snap.dumplog);
                    string htmlfile = Path.GetFileName(snap.dumphtml);
                    string logFilePath = string.IsNullOrWhiteSpace(logfile) ? snap.dumplog : !string.IsNullOrWhiteSpace(logdir) ? Path.Combine(GHApp.GHPath, logdir, logfile) : Path.Combine(GHApp.GHPath, logfile);
                    string htmlFilePath = string.IsNullOrWhiteSpace(htmlfile) ? snap.dumphtml : !string.IsNullOrWhiteSpace(htmldir) ? Path.Combine(GHApp.GHPath, htmldir, htmlfile) : Path.Combine(GHApp.GHPath, htmlfile);
                    string usedFilePath = GHApp.UseHTMLDumpLogs && !string.IsNullOrWhiteSpace(htmlFilePath) && File.Exists(htmlFilePath) ? htmlFilePath :
                         !string.IsNullOrWhiteSpace(logFilePath) && File.Exists(logFilePath) ? logFilePath : "";

                    if (!string.IsNullOrWhiteSpace(usedFilePath))
                    {
                        bool usingHTML = GHApp.UseHTMLDumpLogs && !string.IsNullOrWhiteSpace(htmlFilePath) && usedFilePath == htmlFilePath;
                        var dispfilepage = new DisplayFilePage(usedFilePath, snap.name, 0, true, usingHTML, false);
                        dispfilepage.FontSize = 15;
                        string errormsg;
                        if (dispfilepage.ReadFile(out errormsg))
                            await App.Current.MainPage.Navigation.PushModalAsync(dispfilepage);
                        else
                            await DisplayAlert("Error Reading Snapshot", "Error reading snapshot at " + usedFilePath + ": " + errormsg, "OK");
                    }
                }
            }
            SnapshotLayout.IsEnabled = true;
        }

        private async void Button_Clicked(object sender, EventArgs e)
        {
            CloseButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            await App.Current.MainPage.Navigation.PopModalAsync();
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
                SnapshotLayout.Margin = UIUtils.GetMiddleElementMarginWithBorder(bkgView.BorderStyle, width, height);
                CloseButton.Margin = UIUtils.GetFooterMarginWithBorder(bkgView.BorderStyle, width, height);
            }
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

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
        }
        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
        }
    }
}