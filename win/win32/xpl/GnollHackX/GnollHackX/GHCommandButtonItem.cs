using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;

namespace GnollHackX
{
    public sealed class GHCommandButtonItem
    {
        public readonly string Text;
        public readonly string ImageSourcePath;
        public readonly int Command;
        public readonly string ShortcutText;

        public GHCommandButtonItem(string text, string imgsrcpath, int command)
        {
            Text = text;
            ImageSourcePath = imgsrcpath;
            Command = command;
            ShortcutText = GHUtils.ConstructShortcutText(command);
        }
    }

    public struct GHCommandButtonRect
    {
        public SKRect Rect;
        public readonly GHCommandButtonItem CommandButtonItem;
        public readonly SKImage Bitmap;
        public readonly bool IsWizardModeCommand;
        public GHCommandButtonRect(GHCommandButtonItem commandButtonItem, SKImage bitmap, bool isWizardModeComand)
        {
            Rect = new SKRect();
            CommandButtonItem = commandButtonItem;
            Bitmap = bitmap;
            IsWizardModeCommand = isWizardModeComand;
        }
        public GHCommandButtonRect(SKRect rect, GHCommandButtonItem commandButtonItem, SKImage bitmap, bool isWizardModeComand)
        {
            Rect = rect;
            CommandButtonItem = commandButtonItem;
            Bitmap = bitmap;
            IsWizardModeCommand = isWizardModeComand;
        }
    }
}

