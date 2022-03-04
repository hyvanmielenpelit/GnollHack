using SkiaSharp;
using SkiaSharp.Views.Forms;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Controls
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SwitchableCanvasView : ContentView, ISKCanvasViewController
    {
        private bool _useGL = false;
        public bool UseGL 
        {   get { return _useGL; }
            set
            {
                _useGL = value;
                internalCanvasView.IsVisible = !_useGL;
                internalGLView.IsVisible = _useGL;
            }
       }

        public static readonly BindableProperty IgnorePixelScalingProperty;
        public static readonly BindableProperty EnableTouchEventsProperty;
        public SKSize CanvasSize { get; }
        public bool IgnorePixelScaling { get; set; }
        public bool EnableTouchEvents { get; set; }
        public event EventHandler<SKPaintSurfaceEventArgs> PaintSurface;
        public event EventHandler<SKTouchEventArgs> Touch;
        public void InvalidateSurface()
        {
            if(UseGL)
                internalGLView.InvalidateSurface();
            else
                internalCanvasView.InvalidateSurface();
        }
        protected override SizeRequest OnMeasure(double widthConstraint, double heightConstraint)
        {
            return new SizeRequest();
        }

        public SwitchableCanvasView()
        {
            InitializeComponent();
            internalCanvasView.PaintSurface += PaintSurface;
            //internalGLView.PaintSurface += PaintSurface;
        }



        public event EventHandler SurfaceInvalidated;
        public event EventHandler<GetPropertyValueEventArgs<SKSize>> GetCanvasSize;

        public void OnPaintSurface(SKPaintSurfaceEventArgs e)
        {

        }

        public void OnTouch(SKTouchEventArgs e)
        {

        }
    }
}