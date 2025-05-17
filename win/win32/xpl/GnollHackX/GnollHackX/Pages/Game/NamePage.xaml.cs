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
    public partial class NamePage : ContentPage
    {
        public Regex ValidationExpression { get; set; }
        private GHGame _currentGame;
        private GamePage _gamePage;
        private string _replayEnteredName = null;

        public NamePage(GamePage gamePage, string modeName, string modeDescription, string replayEnteredPlayerName)
        {
            InitializeComponent();
            On<iOS>().SetUseSafeArea(true);
            UIUtils.AdjustRootLayout(RootGrid);
            UIUtils.SetPageThemeOnHandler(this, GHApp.DarkMode);
            UIUtils.SetViewCursorOnHandler(RootGrid, GameCursorType.Normal);
#if GNH_MAUI
            Loaded += ContentPage_Loaded;
#else
            Appearing += ContentPage_Loaded;
#endif
            ValidationExpression = new Regex(@"^[A-Za-z0-9_]{1,31}$");
            _currentGame = gamePage.CurrentGame;
            _gamePage = gamePage;
            _replayEnteredName = replayEnteredPlayerName;

            if(GHApp.DarkMode)
            {
                WhatNameLabel.TextColor = GHColors.White;
                eName.TextColor = GHColors.White;
            }

            if (!string.IsNullOrWhiteSpace(modeName))
            {
                lblModeName.Text = "Game is in " + modeName;
                lblModeName.IsVisible = true;
                if (!string.IsNullOrWhiteSpace(modeDescription))
                {
                    lblModeDescription.Text = modeDescription;
                    lblModeDescription.IsVisible = true;
                }
                else
                {
                    lblModeDescription.Text = "";
                    lblModeDescription.IsVisible = false;
                }
            }
            else
            {
                lblModeName.Text = "";
                lblModeDescription.Text = "";
                lblModeName.IsVisible = false;
                lblModeDescription.IsVisible = false;
            }
            if(_gamePage.PlayingReplay)
            {
                mainStack.IsEnabled = false;
                btnOK.IsEnabled = false;
                btnCancel.IsEnabled = false;                
            }
        }

        private async void btnOK_Clicked(object sender, EventArgs e)
        {
            await DoPressOk();
        }

        private async Task DoPressOk()
        {
            btnOK.IsEnabled = false;
            btnCancel.IsEnabled = false;
            GHApp.PlayButtonClickedSound();
            if (string.IsNullOrWhiteSpace(eName.Text))
            {
                lblError.TextColor = GHColors.Red;
                lblError.Text = "Please enter a name.";
                btnOK.IsEnabled = true;
                btnCancel.IsEnabled = true;
                return;
            }

            string usedName = eName.Text.Trim();
            if (!ValidationExpression.IsMatch(usedName))
            {
                lblError.TextColor = GHColors.Red;
                lblError.Text = "Name is invalid.";
                btnOK.IsEnabled = true;
                btnCancel.IsEnabled = true;
                return;
            }

            var page = await _gamePage.Navigation.PopModalAsync();
            _currentGame?.ResponseQueue.Enqueue(new GHResponse(_currentGame, GHRequestType.AskName, usedName));
            GHApp.DisconnectIViewHandlers(page);
        }

        private void ReplayDoEnterName()
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                eName.Text = _replayEnteredName;
            });
        }

        private void FocusToEnterName()
        {
            MainThread.BeginInvokeOnMainThread(() =>
            {
                eName.Focus();
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
                eName.Focus();
#endif
            }
        }

        private void ContentPage_Disappearing(object sender, EventArgs e)
        {
            GHApp.BackButtonPressed -= BackButtonPressed;
            eName.Unfocus();
            eName.IsEnabled = false;
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
            btnOK.IsEnabled = false;
            btnCancel.IsEnabled = false;
            GHApp.PlayButtonClickedSound();

            var page = await _gamePage.Navigation.PopModalAsync();
            _currentGame?.ResponseQueue.Enqueue(new GHResponse(_currentGame, GHRequestType.AskName, ""));
            GHApp.DisconnectIViewHandlers(page);
        }

        private async void eName_Completed(object sender, EventArgs e)
        {
            if (btnOK.IsEnabled)
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