namespace GnollHackX.Performance
{
    /* What the map showed when the in-game performance test's window began. Plain data:
       no GHApp, MAUI or Xamarin types. Must compile under C# 7.3 (the legacy
       netstandard2.0 project). */
    public sealed class GHDiagnosisScene
    {
        public const string ZoomNormal = "normal";
        public const string ZoomAlternate = "alternate";
        public const string ZoomMinimap = "minimap";

        public string LevelText;                /* the status line's level description; null when unknown */
        public string ZoomMode;                 /* ZoomNormal, ZoomAlternate or ZoomMinimap; null when unknown */
        public float MapFontSize = float.NaN;   /* the normal zoom's map font size */
    }
}
