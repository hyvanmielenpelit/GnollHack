#if GNH_AVALONIA
using Microsoft.Maui;
using Microsoft.Maui.Graphics;
using AcmButtonHandler = Avalonia.Controls.Maui.Handlers.ButtonHandler;
using AcmCheckBoxHandler = Avalonia.Controls.Maui.Handlers.CheckBoxHandler;
using AcmEditorHandler = Avalonia.Controls.Maui.Handlers.EditorHandler;
using AcmEntryHandler = Avalonia.Controls.Maui.Handlers.EntryHandler;

namespace GnollHackM
{
    /* Colors of Avalonia controls that ACM leaves to the Fluent theme, set as the WinUI build shows them */
    internal static class GHAvaloniaControlTheming
    {
        /* The check box's box: stroke in every state and fill when checked, as MAUI's WinUI handler sets them.
           The Fluent theme draws the box from these resources, not from the control's BorderBrush. */
        private static readonly string[] CheckBoxColorResourceKeys =
        {
            "CheckBoxCheckBackgroundStrokeUnchecked",
            "CheckBoxCheckBackgroundStrokeUncheckedPointerOver",
            "CheckBoxCheckBackgroundStrokeUncheckedPressed",
            "CheckBoxCheckBackgroundStrokeUncheckedDisabled",
            "CheckBoxCheckBackgroundStrokeChecked",
            "CheckBoxCheckBackgroundStrokeCheckedPointerOver",
            "CheckBoxCheckBackgroundStrokeCheckedPressed",
            "CheckBoxCheckBackgroundStrokeCheckedDisabled",
            "CheckBoxCheckBackgroundStrokeIndeterminate",
            "CheckBoxCheckBackgroundStrokeIndeterminatePointerOver",
            "CheckBoxCheckBackgroundStrokeIndeterminatePressed",
            "CheckBoxCheckBackgroundStrokeIndeterminateDisabled",
            "CheckBoxCheckBackgroundFillChecked",
            "CheckBoxCheckBackgroundFillCheckedPointerOver",
            "CheckBoxCheckBackgroundFillCheckedPressed",
            "CheckBoxCheckBackgroundFillCheckedDisabled",
        };

        /* Before any handler is created */
        public static void Register()
        {
            AcmEntryHandler.Mapper.AppendToMapping(nameof(ITextStyle.TextColor), (handler, entry) => MapCaret(handler, entry));
            AcmEditorHandler.Mapper.AppendToMapping(nameof(ITextStyle.TextColor), (handler, editor) => MapCaret(handler, editor));
            AcmCheckBoxHandler.Mapper.AppendToMapping(nameof(ICheckBox.Foreground), MapCheckBoxColor);
            AcmCheckBoxHandler.Mapper.AppendToMapping("Color", MapCheckBoxColor);
            AcmButtonHandler.Mapper.AppendToMapping(nameof(ITextStyle.TextColor), MapButtonDisabledTextColor);
        }

        // WORKAROUND(WA-AVALONIA-0008): ACM leaves some control colors to the Fluent theme
        /* A disabled button's text in its text color, as MAUI's WinUI handler sets it. ACM sets the pointer-over and
           pressed foregrounds only, which left the Fluent theme's faint disabled foreground. */
        private static void MapButtonDisabledTextColor(AcmButtonHandler handler, IButton button)
        {
            Avalonia.Controls.Button platformButton = ((IElementHandler)handler).PlatformView as Avalonia.Controls.Button;
            if (platformButton == null)
                return;

            Color color = (button as ITextStyle)?.TextColor;
            if (color == null)
                platformButton.Resources.Remove("ButtonForegroundDisabled");
            else
                platformButton.Resources["ButtonForegroundDisabled"] = ToBrush(color);
        }

        // WORKAROUND(WA-AVALONIA-0008): ACM leaves some control colors to the Fluent theme
        /* The caret in the text color. Without a CaretBrush, Avalonia draws the inverse of the background,
           which for GnollHack's transparent text boxes is black. */
        private static void MapCaret(IElementHandler handler, ITextStyle textStyle)
        {
            Avalonia.Controls.TextBox textBox = handler.PlatformView as Avalonia.Controls.TextBox;
            if (textBox == null)
                return;

            Color color = textStyle.TextColor;
            if (color == null)
                textBox.ClearValue(Avalonia.Controls.TextBox.CaretBrushProperty);
            else
                textBox.CaretBrush = ToBrush(color);
        }

        // WORKAROUND(WA-AVALONIA-0008): ACM leaves some control colors to the Fluent theme
        private static void MapCheckBoxColor(AcmCheckBoxHandler handler, ICheckBox checkBox)
        {
            Avalonia.Controls.CheckBox box = ((IElementHandler)handler).PlatformView as Avalonia.Controls.CheckBox;
            if (box == null)
                return;

            Color color = (checkBox.Foreground as SolidPaint)?.Color;
            Avalonia.Media.IBrush brush = color == null ? null : ToBrush(color);
            foreach (string key in CheckBoxColorResourceKeys)
            {
                if (brush == null)
                    box.Resources.Remove(key);
                else
                    box.Resources[key] = brush;
            }
        }

        private static Avalonia.Media.IBrush ToBrush(Color color)
        {
            color.ToRgba(out byte r, out byte g, out byte b, out byte a);
            return new Avalonia.Media.SolidColorBrush(Avalonia.Media.Color.FromArgb(a, r, g, b));
        }
    }
}
#endif
