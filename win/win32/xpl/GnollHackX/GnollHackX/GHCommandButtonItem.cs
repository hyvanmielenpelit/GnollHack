using System;
using System.Collections.Generic;
using System.Text;
using SkiaSharp;

namespace GnollHackX
{
    public sealed class GHCommandButtonItem
    {
        public string Text;
        public string ImageSourcePath;
        public int Command;
        public string ShortcutText;

        public GHCommandButtonItem()
        {

        }
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
        public GHCommandButtonItem CommandButtonItem;
        public SKImage Bitmap;
        public GHCommandButtonRect(GHCommandButtonItem commandButtonItem, SKImage bitmap)
        {
            Rect = new SKRect();
            CommandButtonItem = commandButtonItem;
            Bitmap = bitmap;
        }
        public GHCommandButtonRect(SKRect rect, GHCommandButtonItem commandButtonItem, SKImage bitmap)
        {
            Rect = rect;
            CommandButtonItem = commandButtonItem;
            Bitmap = bitmap;
        }
    }
}

