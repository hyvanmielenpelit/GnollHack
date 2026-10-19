#if GNH_AVALONIA
using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using Avalonia.Media;
using Avalonia.Media.Fonts;
using Avalonia.Platform;
using GnollHackX;
using Microsoft.Maui;

namespace GnollHackM
{
    /* MAUI font registrar on Avalonia, replacing ACM's. Fonts are Avalonia resources under Assets/Fonts/, loaded into
       the fonts:GnollHack collection with the vertical metrics DirectWrite uses, and a registered font resolves to
       that collection with the family name Avalonia reads from the font. ACM's own name table reader decodes UTF-16
       names of the Unicode platform as UTF-8, which breaks diablo_h.ttf and archristy.ttf. */
    internal sealed class GHAvaloniaFontRegistrar : IFontRegistrar
    {
        private const string CollectionKey = "fonts:GnollHack";

        private readonly object _lock = new object();
        private readonly Dictionary<string, string> _fileNames = new Dictionary<string, string>(StringComparer.Ordinal);
        private readonly Dictionary<string, string> _resolved = new Dictionary<string, string>(StringComparer.Ordinal);
        private GHFontCollection _collection;

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

                /* Created on first use, when Avalonia's font manager exists */
                if (_collection == null)
                {
                    _collection = new GHFontCollection(new Uri(CollectionKey, UriKind.Absolute));
                    Avalonia.Media.FontManager.Current.AddFontCollection(_collection);
                }

                resolved = Resolve(_collection, fileName);
                _resolved[font] = resolved;
                return resolved;
            }
        }

        private static string Resolve(GHFontCollection collection, string fileName)
        {
            string uri = "avares://" + typeof(GHAvaloniaFontRegistrar).Assembly.GetName().Name + "/Assets/Fonts/" + fileName;
            try
            {
                byte[] data;
                using (Stream stream = AssetLoader.Open(new Uri(uri)))
                using (MemoryStream memory = new MemoryStream())
                {
                    stream.CopyTo(memory);
                    data = memory.ToArray();
                }

                UseWindowsMetrics(data);

                GlyphTypeface glyphTypeface;
                if (!collection.TryAddGlyphTypeface(new MemoryStream(data, false), out glyphTypeface) || string.IsNullOrEmpty(glyphTypeface.FamilyName))
                {
                    GHApp.MaybeWriteGHLog("GHAvaloniaFontRegistrar: cannot load " + fileName);
                    return null;
                }
                return CollectionKey + "#" + glyphTypeface.FamilyName;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHAvaloniaFontRegistrar: cannot read " + fileName + ": " + ex.Message);
                return null;
            }
        }

        /* Avalonia lays text out with the hhea ascent, descent and line gap unless the font sets USE_TYPO_METRICS;
           DirectWrite, and so WinUI, uses the OS/2 Windows ascent and descent, with only the part of the hhea line
           height beyond them as line gap. Underwood's hhea ascent is much smaller than its Windows one, so centered
           text sat about 0.09 em higher than on Windows. This writes the DirectWrite values into hhea. */
        private static void UseWindowsMetrics(byte[] font)
        {
            int hhea = FindTable(font, "hhea");
            int os2 = FindTable(font, "OS/2");
            if (hhea < 0 || os2 < 0 || hhea + 10 > font.Length || os2 + 78 > font.Length)
                return;

            const int UseTypoMetrics = 1 << 7;
            if ((ReadUInt16(font, os2 + 62) & UseTypoMetrics) != 0)
                return;

            int winAscent = ReadUInt16(font, os2 + 74);
            int winDescent = ReadUInt16(font, os2 + 76);
            if (winAscent == 0 && winDescent == 0)
                return;

            int ascender = ReadInt16(font, hhea + 4);
            int descender = ReadInt16(font, hhea + 6);
            int lineGap = ReadInt16(font, hhea + 8);
            int windowsLineGap = Math.Max(0, ascender - descender + lineGap - winAscent - winDescent);

            WriteInt16(font, hhea + 4, Math.Min(winAscent, short.MaxValue));
            WriteInt16(font, hhea + 6, -Math.Min(winDescent, short.MaxValue));
            WriteInt16(font, hhea + 8, Math.Min(windowsLineGap, short.MaxValue));
        }

        /* The offset of a table in a single-font file, or -1 */
        private static int FindTable(byte[] font, string tag)
        {
            if (font.Length < 12)
                return -1;

            int numTables = ReadUInt16(font, 4);
            int i;
            for (i = 0; i < numTables; i++)
            {
                int record = 12 + 16 * i;
                if (record + 16 > font.Length)
                    return -1;
                if (font[record] == tag[0] && font[record + 1] == tag[1] && font[record + 2] == tag[2] && font[record + 3] == tag[3])
                {
                    long offset = ((long)font[record + 8] << 24) | ((long)font[record + 9] << 16) | ((long)font[record + 10] << 8) | font[record + 11];
                    return offset < font.Length ? (int)offset : -1;
                }
            }
            return -1;
        }

        private static int ReadUInt16(byte[] data, int offset)
        {
            return (data[offset] << 8) | data[offset + 1];
        }

        private static int ReadInt16(byte[] data, int offset)
        {
            return (short)ReadUInt16(data, offset);
        }

        private static void WriteInt16(byte[] data, int offset, int value)
        {
            data[offset] = (byte)((value >> 8) & 0xFF);
            data[offset + 1] = (byte)(value & 0xFF);
        }

        private sealed class GHFontCollection : FontCollectionBase
        {
            private readonly Uri _key;

            public GHFontCollection(Uri key)
            {
                _key = key;
            }

            public override Uri Key
            {
                get { return _key; }
            }
        }
    }
}
#endif
