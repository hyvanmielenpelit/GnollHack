using System.Threading;
using SkiaSharp;

namespace GnollHackM
{
    /* One SKImage shared by a raster control and the draw operations showing it. Created with one
       reference; the image is disposed when the last reference is released. Thread-safe. */
    public sealed class GHRefCountedImage
    {
        private SKImage _image;
        private int _refCount;

        public GHRefCountedImage(SKImage image)
        {
            _image = image;
            _refCount = 1;
        }

        /* Null once the last reference has been released */
        public SKImage Image => Volatile.Read(ref _image);

        /* False when the image has already been released for good */
        public bool AddRef()
        {
            while (true)
            {
                int count = Volatile.Read(ref _refCount);
                if (count <= 0)
                    return false;
                if (Interlocked.CompareExchange(ref _refCount, count + 1, count) == count)
                    return true;
            }
        }

        public void Release()
        {
            if (Interlocked.Decrement(ref _refCount) == 0)
            {
                SKImage image = Interlocked.Exchange(ref _image, null);
                image?.Dispose();
            }
        }
    }
}
