using System;
using System.Collections.Generic;
using System.Text;
#if GNH_MAUI
namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX
#endif
{
    public class MeasurableGrid : Grid
    {
        public MeasurableGrid() : base()
        {
            PropertyChanged += MeasurableGrid_PropertyChanged;
            SizeChanged += MeasurableGrid_SizeChanged;
            lock (_propertyLock)
            {
                _threadSafeWidth = Width;
                _threadSafeHeight = Height;
                _threadSafeX = X;
                _threadSafeY = Y;
                _threadSafeIsVisible = IsVisible;
                _threadSafeMargin = Margin;
            }
        }

        private void MeasurableGrid_SizeChanged(object sender, EventArgs e)
        {
            lock (_propertyLock)
            {
                _threadSafeWidth = Width;
                _threadSafeHeight = Height;
            }
        }

        private void MeasurableGrid_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(IsVisible))
            {
                ThreadSafeIsVisible = IsVisible;
            }
            else if (e.PropertyName == nameof(Width))
            {
                ThreadSafeWidth = Width;
            }
            else if (e.PropertyName == nameof(Height))
            {
                ThreadSafeWidth = Height;
            }
            else if (e.PropertyName == nameof(X))
            {
                ThreadSafeX = X;
            }
            else if (e.PropertyName == nameof(Y))
            {
                ThreadSafeY = Y;
            }
            else if (e.PropertyName == nameof(Margin))
            {
                ThreadSafeMargin = Margin;
            }
        }

        private readonly object _propertyLock = new object();
        private double _threadSafeWidth = 0;
        private double _threadSafeHeight = 0;
        private double _threadSafeX = 0;
        private double _threadSafeY = 0;
        private bool _threadSafeIsVisible = true;
        private Thickness _threadSafeMargin = new Thickness();

        public double ThreadSafeWidth { get { lock (_propertyLock) { return _threadSafeWidth; } } private set { lock (_propertyLock) { _threadSafeWidth = value; } } }
        public double ThreadSafeHeight { get { lock (_propertyLock) { return _threadSafeHeight; } } private set { lock (_propertyLock) { _threadSafeHeight = value; } } }
        public double ThreadSafeX { get { lock (_propertyLock) { return _threadSafeX; } } private set { lock (_propertyLock) { _threadSafeX = value; } } }
        public double ThreadSafeY { get { lock (_propertyLock) { return _threadSafeY; } } private set { lock (_propertyLock) { _threadSafeY = value; } } }
        public bool ThreadSafeIsVisible { get { lock (_propertyLock) { return _threadSafeIsVisible; } } private set { lock (_propertyLock) { _threadSafeIsVisible = value; } } }
        public Thickness ThreadSafeMargin { get { lock (_propertyLock) { return _threadSafeMargin; } } private set { lock (_propertyLock) { _threadSafeMargin = value; } } }
    }
}
