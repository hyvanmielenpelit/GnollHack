using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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
using Xamarin.Essentials;

namespace GnollHackX.Pages.Game
#endif
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class NamePage : CustomModalPage, ICloseablePage, ISpecialKeyPressHandlingPage
    {
        public Regex ValidationExpression { get; set; }
        private GHGame _currentGame;
        private GamePage _gamePage;
        private string _replayEnteredName = null;

        public NamePage(GamePage gamePage, string modeName, string modeDescription, string replayEnteredPlayerName)
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
#if GNH_MAUI
            Loaded += ContentPage_Loaded;
#else
            Appearing += ContentPage_Loaded;
#endif
            ValidationExpression = new Regex(@"^[A-Za-z0-9_]{1,31}$");
            _currentGame = GHApp.CurrentGHGame;
            _gamePage = gamePage;
            _replayEnteredName = replayEnteredPlayerName;

            if(GHApp.DarkMode)
            {
                WhatNameLabel.TextColor = GHColors.White;
                NameEntry.TextColor = GHColors.White;
            }

            if (!string.IsNullOrWhiteSpace(modeName))
            {
                ModeNameLabel.Text = "Game is in " + modeName;
                ModeNameLabel.IsVisible = true;
                if (!string.IsNullOrWhiteSpace(modeDescription))
                {
                    ModeDescriptionLabel.Text = modeDescription;
                    ModeDescriptionLabel.IsVisible = true;
                }
                else
                {
                    ModeDescriptionLabel.Text = "";
                    ModeDescriptionLabel.IsVisible = false;
                }
            }
            else
            {
                ModeNameLabel.Text = "";
                ModeDescriptionLabel.Text = "";
                ModeNameLabel.IsVisible = false;
                ModeDescriptionLabel.IsVisible = false;
            }
            if(_gamePage.PlayingReplay)
            {
                mainStack.IsEnabled = false;
                OKButton.IsEnabled = false;
                CancelButton.IsEnabled = false;                
            }
        }

        private async void btnOK_Clicked(object sender, EventArgs e)
        {
            await DoPressOk();
        }

        private async Task DoPressOk()
        {
            OKButton.IsEnabled = false;
            CancelButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            if (string.IsNullOrWhiteSpace(NameEntry.Text))
            {
                ErrorLabel.TextColor = GHColors.Red;
                ErrorLabel.Text = "Please enter a name.";
                OKButton.IsEnabled = true;
                CancelButton.IsEnabled = true;
                return;
            }

            string usedName = NameEntry.Text.Trim();
            if (!ValidationExpression.IsMatch(usedName))
            {
                ErrorLabel.TextColor = GHColors.Red;
                ErrorLabel.Text = "Name is invalid.";
                OKButton.IsEnabled = true;
                CancelButton.IsEnabled = true;
                return;
            }

            await GHApp.PopModalPageAsync();
            _currentGame?.ResponseQueue.Enqueue(new GHResponse(_currentGame, GHRequestType.AskName, usedName));
        }

        private void ReplayDoEnterName()
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                NameEntry.Text = _replayEnteredName;
            });
        }

        private void FocusToEnterName()
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                NameEntry.Focus();
            });
        }

        private bool _backPressed = false;
        private async Task<bool> BackButtonPressed(object sender, EventArgs e)
        {
            if (!_backPressed)
            {
                _backPressed = true;
                //await GHApp.Navigation.PopModalAsync();
            }
            await Task.FromResult(0);
            return false;
        }

        private void ContentPage_Appearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed += BackButtonPressed;
        }

        private void ContentPage_Loaded(object sender, EventArgs e)
        {
            if (_gamePage.PlayingReplay)
            {
                if (!string.IsNullOrWhiteSpace(_replayEnteredName))
                {
#if GNH_MAUI
                    var timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                    timer.Interval = TimeSpan.FromSeconds(UIUtils.GetWindowHideSecs());
                    timer.IsRepeating = false;
                    timer.Tick += (s, e) => { ReplayDoEnterName(); };
                    timer.Start();
#else
                    Device.StartTimer(TimeSpan.FromMilliseconds(GHConstants.ReplayAskNameDelay1), () =>
                    {
                        ReplayDoEnterName();
                        return false;
                    });
#endif
                }
            }
            else
            {
#if GNH_MAUI
                var timer = Microsoft.Maui.Controls.Application.Current.Dispatcher.CreateTimer();
                timer.Interval = TimeSpan.FromSeconds(GHConstants.KeyboardFocusDelay);
                timer.IsRepeating = false;
                timer.Tick += (s, e) => { FocusToEnterName(); };
                timer.Start();
#else
                NameEntry.Focus();
#endif
            }
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            NameEntry.Unfocus();
            NameEntry.IsEnabled = false;
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

                WhatNameLabel.Margin = UIUtils.GetHeaderMarginWithBorderWithBottom(bkgView.BorderStyle, width, height, 30.0);
            }
        }

        private async void btnCancel_Clicked(object sender, EventArgs e)
        {
            await DoPressCancel();
        }

        public void PressCancel()
        {
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        await DoPressCancel();
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

        public async Task DoPressCancel()
        {
            OKButton.IsEnabled = false;
            CancelButton.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            await GHApp.PopModalPageAsync();
            _currentGame?.ResponseQueue.Enqueue(new GHResponse(_currentGame, GHRequestType.AskName, ""));
        }

        public bool HandleSpecialKeyPress(GHSpecialKey key, bool isCtrl, bool isMeta, bool isShift)
        {
            bool handled = false;
            try
            {
                MainThread.BeginInvokeOnMainThread(async () =>
                {
                    try
                    {
                        if (key == GHSpecialKey.Escape)
                        {
                            if (CancelButton.IsEnabled)
                            {
                                await DoPressCancel();
                                handled = true;
                            }
                        }
                        else if (key == GHSpecialKey.Enter)
                        {
                            if (OKButton.IsEnabled)
                            {
                                await DoPressOk();
                                handled = true;
                            }
                        }
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
            return handled;
        }

        public void ClosePage()
        {
            HandleSpecialKeyPress(GHSpecialKey.Escape, false, false, false);
        }

        private async void eName_Completed(object sender, EventArgs e)
        {
            if (OKButton.IsEnabled)
            {
                await DoPressOk();
            }
        }

        protected override bool OnBackButtonPressed()
        {
            return true;
        }
    }
}