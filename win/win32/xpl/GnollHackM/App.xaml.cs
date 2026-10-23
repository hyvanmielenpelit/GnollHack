using GnollHackX;
using Microsoft.Maui.Controls.PlatformConfiguration;
using Microsoft.Maui.Platform;
using Newtonsoft.Json.Linq;

#if WINDOWS
using WindowsUI = Windows.UI;
using XamlUI = Microsoft.UI.Xaml.Media;
#endif

namespace GnollHackM;

public partial class App : Application
{
	public App()
	{
		InitializeComponent();

#if WINDOWS
        Microsoft.Maui.Handlers.PickerHandler.Mapper.AppendToMapping(nameof(IPicker.Title), MapPickerTitleToPlaceholder);
        Microsoft.Maui.Handlers.PickerHandler.Mapper.AppendToMapping(nameof(IPicker.TitleColor), MapPickerTitleToPlaceholder);
        Microsoft.Maui.Handlers.SwitchHandler.Mapper.AppendToMapping("NoLabel", (handler, View) =>
        {
            handler.PlatformView.OnContent = null;
            handler.PlatformView.OffContent = null;
            handler.PlatformView.MinWidth = 0;
            handler.PlatformView.MinHeight = 0;
        });
        Microsoft.Maui.Handlers.ButtonHandler.Mapper.AppendToMapping("NoFocusBorder", (handler, View) =>
        {
            handler.PlatformView.BorderThickness = new Microsoft.UI.Xaml.Thickness(0);
        });
#endif
#if GNH_AVALONIA
        Avalonia.Controls.Maui.Handlers.PickerHandler.Mapper.AppendToMapping(nameof(IPicker.Title), MapAvaloniaPickerTitleToPlaceholder);
        Avalonia.Controls.Maui.Handlers.PickerHandler.Mapper.AppendToMapping(nameof(IPicker.TitleColor), MapAvaloniaPickerTitleToPlaceholder);
#endif
#if ANDROID
        Microsoft.Maui.Handlers.ButtonHandler.Mapper.AppendToMapping("NoRippleEffect", (handler, view) =>
        {
            if (handler.PlatformView.Background is Android.Graphics.Drawables.RippleDrawable ripple )
            {
                ripple.SetColor(Android.Content.Res.ColorStateList.ValueOf(Android.Graphics.Color.Transparent));
            };
        });
#endif
#if ANDROID || WINDOWS || GNH_AVALONIA
        // iOS uses the native Liquid Glass switch colors; PlatformSwitchStyle is the implicit Switch style elsewhere
        if (Application.Current.Resources.TryGetValue("PlatformSwitchStyle", out var styleObj) && styleObj is Style baseStyle)
        {
            var switchStyle = new Style(typeof(Switch));
            foreach (var setter in baseStyle.Setters)
            {
                switchStyle.Setters.Add(setter);
            }
            Application.Current.Resources.Add(switchStyle);
        }
#endif
#if IOS
        // WORKAROUND(WA-MAUI-0003): IsOn mapping recolors and re-animates the Liquid Glass UISwitch mid-toggle
        /* Liquid Glass UISwitch (iOS 26+): MAUI's default IsOn mapping forces a TrackColor update,
         * which writes OnTintColor and a private track subview, and re-issues an animated SetState
         * in the middle of UIKit's own toggle animation (dotnet/maui#34560). Toggling must leave
         * the switch alone; colors are written only when one is set. */
        Microsoft.Maui.Handlers.SwitchHandler.Mapper.ModifyMapping(nameof(ISwitch.IsOn), (handler, view, previous) =>
        {
            if (handler.PlatformView is UIKit.UISwitch uiSwitch && uiSwitch.On != view.IsOn)
                uiSwitch.SetState(view.IsOn, true);
        });
        Microsoft.Maui.Handlers.SwitchHandler.Mapper.ModifyMapping(nameof(ISwitch.TrackColor), (handler, view, previous) =>
        {
            if (view.TrackColor == null)
            {
                if (handler.PlatformView is UIKit.UISwitch uiSwitch && uiSwitch.OnTintColor != null)
                    uiSwitch.OnTintColor = null;
                return;
            }
            previous?.Invoke(handler, view);
        });
#endif

#if IOS || ANDROID
        // WORKAROUND(WA-MAUI-0028): FormattedText lines get uneven heights when spans mix fonts
        /* Custom Label handler for consistent line spacing when mixing fonts in FormattedText.
         * 
         * Problem: iOS (CoreText) and Android (StaticLayout) compute each line's height from
         * the tallest font's metrics on that line. When a Label mixes fonts (e.g., Underwood
         * for body text and DejaVuSansMono for inline code), lines containing the taller font
         * end up with a different height, producing visually uneven spacing.
         *
         * Solution: After MAUI sets the platform view's attributed/spannable text, we
         * post-process it to enforce a uniform absolute line height based on the tallest
         * font present in the string.
         *
         * On iOS: Set MinimumLineHeight = MaximumLineHeight on all paragraph styles,
         *         preserving existing Alignment and LineBreakMode.
         * On Android: Disable IncludeFontPadding and use SetLineSpacing(absolute, 0f)
         *             to enforce a fixed line height.
         */
        /* Shared handler logic for consistent line spacing when mixing fonts.
         * Registered for both FormattedText and LineHeight property mappings because
         * MAUI's MapLineHeight fires independently and calls SetLineSpacing(0, mult)
         * with a per-line relative multiplier, which would override our absolute fix. */
        Action<Microsoft.Maui.Handlers.LabelHandler, Microsoft.Maui.ILabel> uniformLineHeightHandler = (handler, view) =>
        {
#if IOS
            if (view is not Label label || label.FormattedText == null || label.FormattedText.Spans.Count <= 1)
                return;

            if (handler.PlatformView is not UIKit.UILabel uiLabel || uiLabel.AttributedText == null || uiLabel.AttributedText.Length == 0)
                return;

            var attributedText = uiLabel.AttributedText;
            var mutableAttr = new Foundation.NSMutableAttributedString(attributedText);

            /* Resolve the base font from the MAUI label's FontFamily/FontSize/FontAttributes.
             * MapFont is skipped for FormattedText labels, so uiLabel.Font stays at the
             * UILabel default system font and cannot be used as the reference here. */
            var fontManager = handler.MauiContext?.Services.GetService<Microsoft.Maui.IFontManager>();
            if (fontManager == null)
                return;
            var basePlatformFont = fontManager.GetFont(label.ToFont());
            if (basePlatformFont == null)
                return;
            nfloat baseFontNaturalLineHeight = basePlatformFont.LineHeight;

            /* Find the maximum natural line height across all span fonts in the
             * attributed string. UIFont.LineHeight = ascent + descent + leading. */
            nfloat maxSpanLineHeight = 0;
            attributedText.EnumerateAttribute(
                UIKit.UIStringAttributeKey.Font,
                new Foundation.NSRange(0, attributedText.Length),
                Foundation.NSAttributedStringEnumeration.None,
                (Foundation.NSObject value, Foundation.NSRange range, ref bool stop) =>
                {
                    if (value is UIKit.UIFont font && font.LineHeight > maxSpanLineHeight)
                        maxSpanLineHeight = font.LineHeight;
                });

            if (maxSpanLineHeight <= 0)
                return;

            /* Compute target uniform line height as the maximum of:
             *   (a) base font natural height × the label's LineHeight multiplier
             *       (or × 1.0 when LineHeight is not set), which reflects the
             *       intended spacing for lines that contain only the base font;
             *   (b) the tallest span font's natural line height (no multiplier),
             *       so that a taller span font never gets clipped.
             * This guarantees all lines are at least as tall as (a), and grow to
             * accommodate any span font that exceeds that floor. */
            double lineMultiplier = label.LineHeight >= 0 ? label.LineHeight : 1.0;
            nfloat baseTarget = baseFontNaturalLineHeight * (nfloat)lineMultiplier;
            nfloat targetLineHeight = (nfloat)System.Math.Max((double)baseTarget, (double)maxSpanLineHeight);

            /* Enumerate existing paragraph styles and patch them in-place.
             * For sub-ranges with no ParagraphStyle attribute, 'value' will be null. */
            attributedText.EnumerateAttribute(
                UIKit.UIStringAttributeKey.ParagraphStyle,
                new Foundation.NSRange(0, attributedText.Length),
                Foundation.NSAttributedStringEnumeration.None,
                (Foundation.NSObject value, Foundation.NSRange range, ref bool stop) =>
                {
                    UIKit.NSMutableParagraphStyle mutableStyle;
                    if (value is UIKit.NSParagraphStyle existingStyle)
                    {
                        /* Clone existing style to preserve Alignment, LineBreakMode, etc. */
                        mutableStyle = (UIKit.NSMutableParagraphStyle)existingStyle.MutableCopy();
                    }
                    else
                    {
                        /* No paragraph style on this range; create a minimal one */
                        mutableStyle = new UIKit.NSMutableParagraphStyle();
                    }

                    mutableStyle.MinimumLineHeight = targetLineHeight;
                    mutableStyle.MaximumLineHeight = targetLineHeight;

                    mutableAttr.AddAttribute(
                        UIKit.UIStringAttributeKey.ParagraphStyle,
                        mutableStyle,
                        range);
                });

            uiLabel.AttributedText = mutableAttr;

#elif ANDROID
            if (handler.PlatformView is not Android.Widget.TextView textView)
                return;

            if (view is not Label label || label.FormattedText == null || label.FormattedText.Spans.Count <= 1)
            {
                /* State leak fix: If this label was previously FormattedText, we wiped out its
                 * native SetLineSpacing multiplier. If it is now converted back to plain Text,
                 * MAUI doesn't automatically re-map LineHeight since the property value didn't
                 * change. We must restore it manually. */
                if (view is Label plainLabel && plainLabel.LineHeight >= 0)
                {
                    textView.SetLineSpacing(0, (float)plainLabel.LineHeight);
                }
                return;
            }

            /* Resolve the base font from the MAUI label's FontFamily/FontSize/FontAttributes.
             * MapFont is skipped for FormattedText labels, so textView.Paint does not
             * reflect the MAUI FontFamily and cannot be used as the reference here. */
            var fontManager = handler.MauiContext?.Services.GetService<Microsoft.Maui.IFontManager>();
            if (fontManager == null)
                return;
            var baseFont = label.ToFont();
            var basePaint = new Android.Text.TextPaint(textView.Paint);
            if (!baseFont.IsDefault)
            {
                var typeface = fontManager.GetTypeface(baseFont);
                if (typeface != null)
                    basePaint.SetTypeface(typeface);
                var fontSize = fontManager.GetFontSize(baseFont);
                basePaint.TextSize = Android.Util.TypedValue.ApplyDimension(
                    fontSize.Unit,
                    fontSize.Value,
                    textView.Resources.DisplayMetrics);
            }
            var baseFm = basePaint.GetFontMetrics();
            /* Base natural height = ascent + descent (no Leading).
             * This matches iOS UIFont.LineHeight and is the metric we multiply
             * by the LineHeight multiplier. */
            float baseNaturalHeight = baseFm != null
                ? baseFm.Descent - baseFm.Ascent
                : 0;

            if (baseNaturalHeight <= 0)
                return;

            /* Measure the maximum natural height of the span fonts, STRICTLY IGNORING Leading.
             * We iterate through the span transition points and cumulatively apply all
             * MetricAffectingSpans (e.g., AbsoluteSizeSpan + TypefaceSpan) to a TextPaint.
             * This prevents fonts with massive native Leading (like DejaVu) from artificially
             * inflating the line height, while still allowing intentionally large fonts
             * (via FontSize) to correctly expand the line. */
            float maxSpanNaturalHeight = 0;
            if (textView.TextFormatted is Android.Text.ISpanned spanned)
            {
                int len = spanned.Length();
                int next;
                for (int i = 0; i < len; i = next)
                {
                    next = spanned.NextSpanTransition(i, len, Java.Lang.Class.FromType(typeof(Android.Text.Style.MetricAffectingSpan)));

                    var spansAtIdx = spanned.GetSpans(i, next, Java.Lang.Class.FromType(typeof(Android.Text.Style.MetricAffectingSpan)));
                    if (spansAtIdx != null && spansAtIdx.Length > 0)
                    {
                        var spanPaint = new Android.Text.TextPaint(textView.Paint);
                        foreach (var s in spansAtIdx)
                        {
                            if (s is Android.Text.Style.MetricAffectingSpan metricSpan)
                            {
                                metricSpan.UpdateMeasureState(spanPaint);
                            }
                        }
                        var fm = spanPaint.GetFontMetrics();
                        if (fm != null)
                        {
                            float h = fm.Descent - fm.Ascent; /* NO LEADING */
                            if (h > maxSpanNaturalHeight)
                                maxSpanNaturalHeight = h;
                        }
                    }
                }
            }

            /* Compute target line height based on the base font's natural height
             * (descent − ascent) × the LineHeight multiplier.
             * We take the Max with maxSpanNaturalHeight so that intentionally large fonts
             * expand the line, but at 1.5x multiplier, baseTarget usually dominates
             * normal-sized fonts like DejaVu. */
            double lineMultiplier = label.LineHeight >= 0 ? label.LineHeight : 1.0;
            float baseTarget = baseNaturalHeight * (float)lineMultiplier;
            float targetLineHeightPx = System.Math.Max(baseTarget, maxSpanNaturalHeight);

            /* Since MAUI invokes its native LineHeight mapping initially when the label
             * is empty, textView.SetLineSpacing(0, label.LineHeight) may already be set.
             * Later, when FormattedText is populated, MAUI's mapper skips LineHeight,
             * leaving the multiplier intact. If we don't reset it, Android's StaticLayout
             * will multiply our UniformLineHeightSpan by label.LineHeight AGAIN! */
            textView.SetLineSpacing(0, 1.0f);

            /* Apply a LineHeightSpan to the entire text to force uniform line heights.
             * A LineHeightSpan.chooseHeight() callback fires for EACH line during
             * StaticLayout and directly controls the FontMetricsInt, guaranteeing
             * uniform height regardless of which fonts appear on that line.
             *
             * We create a NEW SpannableString because textView.TextFormatted returns
             * SpannedString (immutable — no SetSpan). The constructor copies all
             * existing spans from the source. We strip any previously-added
             * UniformLineHeightSpan to prevent accumulation when this handler fires
             * multiple times (registered for both FormattedText and LineHeight). */
            if (textView.TextFormatted != null)
            {
                var newSpannable = new Android.Text.SpannableString(textView.TextFormatted);

                /* Remove any existing UniformLineHeightSpan from previous handler runs */
                var existingSpans = newSpannable.GetSpans(0, newSpannable.Length(),
                    Java.Lang.Class.FromType(typeof(UniformLineHeightSpan)));
                if (existingSpans != null)
                {
                    foreach (var old in existingSpans)
                        newSpannable.RemoveSpan(old);
                }

                int targetHeightInt = (int)System.Math.Ceiling(targetLineHeightPx);
                int baseAscentInt = baseFm != null ? (int)System.Math.Floor(baseFm.Ascent) : -targetHeightInt;
                var uniformSpan = new UniformLineHeightSpan(targetHeightInt, baseAscentInt);
                newSpannable.SetSpan(
                    uniformSpan,
                    0,
                    newSpannable.Length(),
                    Android.Text.SpanTypes.InclusiveInclusive);

                textView.TextFormatted = newSpannable;
            }
#endif
        };

        /* Register for both FormattedText and LineHeight mappings.
         * On Android, MAUI's MapLineHeight calls SetLineSpacing(0, label.LineHeight) with a
         * relative multiplier that fires after FormattedText and overrides our absolute fix.
         * By also appending to LineHeight, we re-apply our fix after MAUI's own handler. */
        Microsoft.Maui.Handlers.LabelHandler.Mapper.AppendToMapping(nameof(Label.FormattedText), uniformLineHeightHandler);
        Microsoft.Maui.Handlers.LabelHandler.Mapper.AppendToMapping(nameof(Label.LineHeight), uniformLineHeightHandler);
#endif

        GHApp.Initialize();
#if IOS
        GnollHackM.Platforms.iOS.StatusBarVisibility.Register();
#endif
        //MainPage = new AppShell();
    }

#if WINDOWS
    /* Shows a picker's Title as ComboBox placeholder text in TitleColor instead of as a header */
    private static void MapPickerTitleToPlaceholder(Microsoft.Maui.Handlers.IPickerHandler handler, IPicker picker)
    {
        var comboBox = handler.PlatformView;
        if (comboBox == null)
            return;

        comboBox.Header = null;
        comboBox.HeaderTemplate = null;

        string title = picker.Title;
        if (string.IsNullOrWhiteSpace(title))
        {
            comboBox.ClearValue(Microsoft.UI.Xaml.Controls.ComboBox.PlaceholderTextProperty);
            comboBox.ClearValue(Microsoft.UI.Xaml.Controls.ComboBox.PlaceholderForegroundProperty);
            return;
        }

        comboBox.PlaceholderText = title;
        Color titleColor = picker.TitleColor;
        if (titleColor != null)
        {
            titleColor.ToRgba(out byte r, out byte g, out byte b, out byte a);
            comboBox.PlaceholderForeground = new XamlUI.SolidColorBrush(WindowsUI.Color.FromArgb(a, r, g, b));
        }
        else
        {
            comboBox.ClearValue(Microsoft.UI.Xaml.Controls.ComboBox.PlaceholderForegroundProperty);
        }
    }
#endif

#if GNH_AVALONIA
    /* Shows a picker's Title as ComboBox placeholder text in TitleColor instead of as a header */
    private static void MapAvaloniaPickerTitleToPlaceholder(Avalonia.Controls.Maui.Handlers.PickerHandler handler, IPicker picker)
    {
        Avalonia.Controls.Maui.MauiComboBox comboBox = ((IElementHandler)handler).PlatformView as Avalonia.Controls.Maui.MauiComboBox;
        if (comboBox == null)
            return;

        comboBox.Header = null;
        comboBox.HeaderTemplate = null;

        string title = picker.Title;
        if (string.IsNullOrWhiteSpace(title))
        {
            comboBox.ClearValue(Avalonia.Controls.ComboBox.PlaceholderTextProperty);
            comboBox.ClearValue(Avalonia.Controls.ComboBox.PlaceholderForegroundProperty);
            return;
        }

        comboBox.PlaceholderText = title;
        Color titleColor = picker.TitleColor;
        if (titleColor != null)
        {
            titleColor.ToRgba(out byte r, out byte g, out byte b, out byte a);
            comboBox.PlaceholderForeground = new Avalonia.Media.SolidColorBrush(Avalonia.Media.Color.FromArgb(a, r, g, b));
        }
        else
        {
            comboBox.ClearValue(Avalonia.Controls.ComboBox.PlaceholderForegroundProperty);
        }
    }
#endif

    protected override Window CreateWindow(IActivationState activationState)
    {
        Window window = new Window(new AppShell());
#if GNH_AVALONIA
        window.Title = "GnollHack";
#endif
        window.Activated += WindowActivated;
        window.Deactivated += WindowDeactivated;
        window.Destroying += WindowDestroying;
        window.Stopped += Window_Stopped;
        window.Resumed += Window_Resumed;
        window.Created += Window_Created;
        return window;  
    }

    private static void Window_Created(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been created.", true, "Window Information");
        GHApp.OnStart();
    }

    private static void Window_Resumed(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been resumed.", true, "Window Information");
        GHApp.OnResume();
    }

    private static void Window_Stopped(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been stopped / entered background.", true, "Window Information");
        GHApp.OnSleep();
    }

    private static void WindowActivated(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been activated / focused.", true, "Window Information");
        GHApp.OnFocus();
    }
    private static void WindowDeactivated(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window has been deactivated / unfocused.", true, "Window Information");
        GHApp.OnUnfocus();
    }
    private static void WindowDestroying(object sender, EventArgs e)
    {
        GHApp.MaybeWriteGHLog("MAUI Window's native window is being destroyed.", true, "Window Information");
        //Window window = sender as Window;
        //if (window != null)
        //{
        //    window.Activated -= WindowActivated;
        //    window.Deactivated -= WindowDeactivated;
        //    window.Destroying -= WindowDestroying;
        //    window.Stopped -= Window_Stopped;
        //    window.Resumed -= Window_Resumed;
        //    window.Created -= Window_Created;
        //}
    }

    //protected override void OnStart()
    //{
    //    base.OnStart();
    //    GHApp.OnStart();
    //}

    //protected override void OnSleep()
    //{
    //    base.OnSleep();
    //    GHApp.OnSleep();
    //}

    //protected override void OnResume()
    //{
    //    base.OnResume();
    //    GHApp.OnResume();
    //}

    protected override void CleanUp()
    {
        GHApp.RevertScreenResolution();
        base.CleanUp();
    }


}

#if ANDROID
/// <summary>
/// A LineHeightSpan that forces every line to a uniform absolute height in pixels.
/// Applied to the entire SpannableString, its chooseHeight() callback fires for each
/// line during StaticLayout and directly adjusts the FontMetricsInt to produce the
/// target height, overriding per-line font metric differences.
/// </summary>
internal class UniformLineHeightSpan : Java.Lang.Object, Android.Text.Style.ILineHeightSpan
{
    private readonly int _targetHeight;
    private readonly int _targetAscent;

    /// <summary>
    /// Creates a span that forces every line to a uniform height.
    /// </summary>
    /// <param name="targetHeightPx">The desired total line height in pixels.</param>
    /// <param name="targetAscentPx">
    /// The ascent to use for all lines (negative value, e.g. -40).
    /// This should be the base font's ascent so baseline positioning is consistent.
    /// </param>
    public UniformLineHeightSpan(int targetHeightPx, int targetAscentPx)
    {
        _targetHeight = targetHeightPx;
        _targetAscent = targetAscentPx;
    }

    public void ChooseHeight(
        Java.Lang.ICharSequence text,
        int start,
        int end,
        int spanstartv,
        int lineHeight,
        Android.Graphics.Paint.FontMetricsInt fm)
    {
        if (fm == null)
            return;

        /* Force all four font metrics to fixed values so every line has
         * exactly the same height AND the same baseline position.
         * Without this, fm.Ascent varies per line based on the tallest
         * font on that line, causing subtle vertical shifts even when
         * the total height (Descent - Ascent) is the same.
         *
         * Ascent = _targetAscent (fixed baseline from base font)
         * Descent = Ascent + _targetHeight
         * Top = Ascent, Bottom = Descent (no extra leading) */
        fm.Ascent = _targetAscent;
        fm.Descent = _targetAscent + _targetHeight;
        fm.Top = fm.Ascent;
        fm.Bottom = fm.Descent;
    }
}
#endif
