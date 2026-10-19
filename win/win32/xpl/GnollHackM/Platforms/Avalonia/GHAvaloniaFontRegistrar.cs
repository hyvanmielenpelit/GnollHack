#if GNH_AVALONIA
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using Avalonia.Platform;
using GnollHackX;
using Microsoft.Maui;
using SkiaSharp;

namespace GnollHackM
{
    /* MAUI font registrar on Avalonia, replacing ACM's. Fonts are Avalonia resources under Assets/Fonts/, and
       a registered font resolves to its resource URI with the family name SkiaSharp reads from the font. ACM's own
       name table reader decodes UTF-16 names of the Unicode platform as UTF-8, which breaks diablo_h.ttf and
       archristy.ttf. */
    internal sealed class GHAvaloniaFontRegistrar : IFontRegistrar
    {
        private readonly object _lock = new object();
        private readonly Dictionary<string, string> _fileNames = new Dictionary<string, string>(StringComparer.Ordinal);
        private readonly Dictionary<string, string> _resolved = new Dictionary<string, string>(StringComparer.Ordinal);

        public void Register(string filename, string alias, Assembly assembly)
        {
            Register(filename, alias);
        }

        public void Register(string filename, string alias)
        {
            lock (_lock)
            {
                _fileNames[filename] = filename;
                if (!string.IsNullOrWhiteSpace(alias))
                    _fileNames[alias] = filename;
            }
        }

        public string GetFont(string font)
        {
            if (string.IsNullOrWhiteSpace(font))
                return null;

            lock (_lock)
            {
                string resolved;
                if (_resolved.TryGetValue(font, out resolved))
                    return resolved;

                string fileName;
                if (!_fileNames.TryGetValue(font, out fileName))
                    return null;

                resolved = Resolve(fileName);
                _resolved[font] = resolved;
                return resolved;
            }
        }

        private static string Resolve(string fileName)
        {
            string uri = "avares://" + typeof(GHAvaloniaFontRegistrar).Assembly.GetName().Name + "/Assets/Fonts/" + fileName;
            try
            {
                using (Stream stream = AssetLoader.Open(new Uri(uri)))
                using (SKData data = SKData.Create(stream))
                using (SKTypeface typeface = SKTypeface.FromData(data))
                {
                    string family = typeface?.FamilyName;
                    if (string.IsNullOrEmpty(family))
                    {
                        GHApp.MaybeWriteGHLog("GHAvaloniaFontRegistrar: no family name in " + fileName);
                        return null;
                    }
                    return uri + "#" + family;
                }
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaFontRegistrar: cannot read " + fileName + ": " + ex.Message);
                return null;
            }
        }
    }
}
#endif
