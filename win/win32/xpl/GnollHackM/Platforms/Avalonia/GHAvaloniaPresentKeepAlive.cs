#if GNH_AVALONIA
using Avalonia;
using Avalonia.Controls;
using Avalonia.Media;
using Avalonia.Media.Immutable;

namespace GnollHackM
{
    /* A 1 x 1 control in the window's overlay layer whose render data changes on each Pulse, so that Avalonia
       composes and presents a frame. It draws one black pixel at alpha 1 or 2, alternating. UI thread. */
    public sealed class GHAvaloniaPresentKeepAlive : Control
    {
        private static readonly IImmutableSolidColorBrush _firstBrush =
            new ImmutableSolidColorBrush(Avalonia.Media.Color.FromArgb(1, 0, 0, 0), 1.0, null);
        private static readonly IImmutableSolidColorBrush _secondBrush =
            new ImmutableSolidColorBrush(Avalonia.Media.Color.FromArgb(2, 0, 0, 0), 1.0, null);
        private static readonly Avalonia.Rect _pixel = new Avalonia.Rect(0, 0, 1, 1);

        private bool _useSecondBrush = false;

        public GHAvaloniaPresentKeepAlive()
        {
            Width = 1;
            Height = 1;
            IsHitTestVisible = false;
            Focusable = false;
        }

        /* Never called during Avalonia's render pass */
        public void Pulse()
        {
            _useSecondBrush = !_useSecondBrush;
            InvalidateVisual();
        }

        public override void Render(DrawingContext context)
        {
            context.FillRectangle(_useSecondBrush ? _secondBrush : _firstBrush, _pixel);
        }
    }
}
#endif
