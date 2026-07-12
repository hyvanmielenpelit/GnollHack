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
        Microsoft.Maui.Handlers.PickerHandler.Mapper.AppendToMapping(nameof(IPicker.Title), (handler, view) =>
        {
            if (handler.PlatformView is not null && view is Picker pick && !String.IsNullOrWhiteSpace(pick.Title))
            {
                handler.PlatformView.HeaderTemplate = new Microsoft.UI.Xaml.DataTemplate();
                handler.PlatformView.PlaceholderText = pick.Title;
                pick.Title = null;

                pick.TitleColor.ToRgba(out byte r, out byte g, out byte b, out byte a);
                handler.PlatformView.PlaceholderForeground = new XamlUI.SolidColorBrush(WindowsUI.Color.FromArgb(a, r, g, b));
            }
        });
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
#if ANDROID
        Microsoft.Maui.Handlers.ButtonHandler.Mapper.AppendToMapping("NoRippleEffect", (handler, view) =>
        {
            if (handler.PlatformView.Background is Android.Graphics.Drawables.RippleDrawable ripple )
            {
                ripple.SetColor(Android.Content.Res.ColorStateList.ValueOf(Android.Graphics.Color.Transparent));
            };
        });
#endif
#if ANDROID || WINDOWS
        // If UIDesignRequiresCompatibility Info.plist item gets deprecated, you can change the name of the global Switch style to PlatformSwitchStyle, and add it only for right platforms here
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

#if IOS || ANDROID
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
        Microsoft.Maui.Handlers.LabelHandler.Mapper.AppendToMapping(nameof(Label.FormattedText), (handler, view) =>
        {
            if (view is not Label label || label.FormattedText == null || label.FormattedText.Spans.Count <= 1)
                return;

#if IOS
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

            /* Disable extra font padding for precise spacing */
            textView.SetIncludeFontPadding(false);

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
            float baseNaturalHeight = baseFm != null
                ? baseFm.Descent - baseFm.Ascent + System.Math.Max(0, baseFm.Leading)
                : 0;

            if (baseNaturalHeight <= 0)
                return;

            /* Measure each span's font natural line height by iterating MetricAffectingSpan
             * objects in the spannable and applying them to a temporary TextPaint. */
            float maxSpanNaturalHeight = 0;
            if (textView.TextFormatted is Android.Text.ISpannable spannable)
            {
                var fontSpans = spannable.GetSpans(0, spannable.Length(),
                    Java.Lang.Class.FromType(typeof(Android.Text.Style.MetricAffectingSpan)));
                if (fontSpans != null)
                {
                    foreach (var span in fontSpans)
                    {
                        if (span is Android.Text.Style.MetricAffectingSpan metricSpan)
                        {
                            var spanPaint = new Android.Text.TextPaint(textView.Paint);
                            metricSpan.UpdateMeasureState(spanPaint);
                            var fm = spanPaint.GetFontMetrics();
                            if (fm != null)
                            {
                                float h = fm.Descent - fm.Ascent + System.Math.Max(0, fm.Leading);
                                if (h > maxSpanNaturalHeight)
                                    maxSpanNaturalHeight = h;
                            }
                        }
                    }
                }
            }

            /* Compute target line height as the maximum of:
             *   (a) base font natural height × the label's LineHeight multiplier
             *       (or × 1.0 when LineHeight is not set), which reflects the
             *       intended spacing for lines containing only the base font;
             *   (b) the tallest span font's natural line height (no multiplier),
             *       so that a taller span font is never clipped.
             * All lines become uniformly this height. */
            double lineMultiplier = label.LineHeight >= 0 ? label.LineHeight : 1.0;
            float baseTarget = baseNaturalHeight * (float)lineMultiplier;
            float targetLineHeightPx = System.Math.Max(baseTarget, maxSpanNaturalHeight);

            /* Enforce uniform spacing via SetLineSpacing.
             * SetLineSpacing(add, mult) computes: lineHeight = defaultLineHeight * mult + add
             * Setting mult=0 and add=targetLineHeightPx gives an absolute fixed height */
            textView.SetLineSpacing(targetLineHeightPx, 0f);
#endif
        });
#endif

        GHApp.Initialize();
        //MainPage = new AppShell();
    }

    protected override Window CreateWindow(IActivationState activationState)
    {
        Window window = new Window(new AppShell());
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
