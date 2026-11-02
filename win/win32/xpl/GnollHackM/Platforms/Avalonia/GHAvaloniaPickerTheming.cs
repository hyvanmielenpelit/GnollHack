#if GNH_AVALONIA
using Microsoft.Maui;
using Microsoft.Maui.Graphics;
using GnollHackX;
using AcmPickerHandler = Avalonia.Controls.Maui.Handlers.PickerHandler;

namespace GnollHackM
{
    /* A picker's own dropdown list in the settings popups' panel color, its items white and the current one light
       green, in Dark Mode and light mode alike; the closed picker's arrow and focused text in its text color, and its
       pointer-over and pressed background in the text fields' veil that shows against its text color. Otherwise the
       Fluent theme draws them in the system's theme variant. Applies to every picker in the app. */
    internal static class GHAvaloniaPickerTheming
    {
        private static readonly Color ItemPointerOverColor = Color.FromRgba(1.0, 1.0, 1.0, 0.10);
        private static readonly Color ItemSelectedColor = Color.FromRgba(1.0, 1.0, 1.0, 0.15);
        private static readonly Color ItemSelectedPointerOverColor = Color.FromRgba(1.0, 1.0, 1.0, 0.22);
        private static readonly Color ItemPressedColor = Color.FromRgba(1.0, 1.0, 1.0, 0.25);

        private static readonly string[] TextColorResourceKeys =
        {
            "ComboBoxDropDownGlyphForeground",
            "ComboBoxDropDownGlyphForegroundFocused",
            "ComboBoxDropDownGlyphForegroundFocusedPressed",
            "ComboBoxForegroundFocused",
            "ComboBoxForegroundFocusedPressed",
        };

        /* Before any handler is created */
        public static void Register()
        {
            AcmPickerHandler.Mapper.AppendToMapping(nameof(ITextStyle.TextColor), MapPickerColors);
        }

        /* When the picker connects and when its text color changes, as a Dark Mode change does */
        private static void MapPickerColors(AcmPickerHandler handler, IPicker picker)
        {
            Avalonia.Controls.ComboBox comboBox = ((IElementHandler)handler).PlatformView as Avalonia.Controls.ComboBox;
            if (comboBox == null)
                return;

            bool darkMode = GHApp.DarkMode;
            Avalonia.Controls.IResourceDictionary resources = comboBox.Resources;
            resources["ComboBoxDropDownBackground"] = GHAvaloniaControlTheming.ToBrush(darkMode ? GHColors.MsgBoxDarkModeBkgColor : GHColors.MsgBoxLightModeBkgColor);
            resources["ComboBoxDropDownBorderBrush"] = GHAvaloniaControlTheming.ToBrush(GHColors.Black);
            SetBrush(resources, GHColors.White, "ComboBoxItemForeground", "ComboBoxItemForegroundPointerOver", "ComboBoxItemForegroundPressed");
            SetBrush(resources, GHColors.LightGreen, "ComboBoxItemForegroundSelected", "ComboBoxItemForegroundSelectedPointerOver", "ComboBoxItemForegroundSelectedPressed");
            SetBrush(resources, GHColors.Gray, "ComboBoxItemForegroundDisabled", "ComboBoxItemForegroundSelectedDisabled");
            SetBrush(resources, ItemPointerOverColor, "ComboBoxItemBackgroundPointerOver");
            SetBrush(resources, ItemSelectedColor, "ComboBoxItemBackgroundSelected");
            SetBrush(resources, ItemSelectedPointerOverColor, "ComboBoxItemBackgroundSelectedPointerOver");
            SetBrush(resources, ItemPressedColor, "ComboBoxItemBackgroundPressed", "ComboBoxItemBackgroundSelectedPressed");

            /* Light text sits on a dark background, whatever the mode */
            Color textColor = ((ITextStyle)picker).TextColor;
            bool lightText = textColor != null ? textColor.GetLuminosity() > 0.5f : darkMode;
            SetBrush(resources, lightText ? GHColors.PickerDarkModeBkgColor : GHColors.PickerLightModeBkgColor, "ComboBoxBackgroundPointerOver", "ComboBoxBackgroundPressed");

            foreach (string key in TextColorResourceKeys)
            {
                if (textColor == null)
                    resources.Remove(key);
                else
                    resources[key] = GHAvaloniaControlTheming.ToBrush(textColor);
            }
        }

        private static void SetBrush(Avalonia.Controls.IResourceDictionary resources, Color color, params string[] keys)
        {
            Avalonia.Media.IBrush brush = GHAvaloniaControlTheming.ToBrush(color);
            foreach (string key in keys)
                resources[key] = brush;
        }
    }
}
#endif
