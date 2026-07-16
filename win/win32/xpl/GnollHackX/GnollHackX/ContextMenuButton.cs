using SkiaSharp;

namespace GnollHackX
{
    public class ContextMenuButton
    {
        public readonly string LblText;
        public readonly string ImgSourcePath;
        public readonly SKImage Bitmap;
        public readonly int BtnCommand;
        public readonly string ShortcutText;
        
        public ContextMenuButton(string lblText, string imgSourcePath, SKImage bitmap, int btnCommand)
        {
            LblText = lblText;
            ImgSourcePath = imgSourcePath;
            Bitmap = bitmap;
            BtnCommand = btnCommand;
            ShortcutText = GHUtils.ConstructShortcutText(btnCommand);
        }
    }
}
