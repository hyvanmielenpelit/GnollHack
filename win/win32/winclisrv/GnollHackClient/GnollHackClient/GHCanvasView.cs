using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using GnollHackClient.Pages.Game;
using GnollHackCommon;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using Xamarin.Forms;

namespace GnollHackClient
{

    class GHCanvasView : SKCanvasView
    {
        public ClientGame _clientGame;
        public GamePage _gamePage;
        public int _glyph;

        public GHWindow GHWindow {get; set;}
        public ghmenu_styles MenuStyle { get; set; }
        public object MenuItemLock = new object();

        private ObservableCollection<GHMenuItem> _GHMenuItems = null;
        public ObservableCollection<GHMenuItem> MenuItems { get { return _GHMenuItems; } set { _GHMenuItems = value; } }

        public SelectionMode SelectionHow { get; set; }
        public int SelectionIndex { get; set; }

        private bool _responseSent = false;
        private bool unselect_on_tap = false;


        public GHCanvasView() : base()
        {

        }



        public BindableProperty GeneralAnimationCounterProperty =
            BindableProperty.Create(nameof(GeneralAnimationCounter), typeof(long), typeof(GHCanvasView), 0L);

        public long GeneralAnimationCounter
        {
            get { return (long)GetValue(GeneralAnimationCounterProperty); }
            set { SetValue(GeneralAnimationCounterProperty, value); }
        }


        protected override void OnPropertyChanged(string propertyName = null)
        {
            base.OnPropertyChanged(propertyName);

            if(_gamePage != null)
            {
                bool refresh = true;
                lock (_gamePage.RefreshScreenLock)
                {
                    refresh = _gamePage.RefreshScreen;
                }

                _gamePage.IncrementCounters();

                if (refresh)
                    InvalidateSurface();

            }
        }
    }
}
