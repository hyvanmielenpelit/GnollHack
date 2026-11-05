using GnollHackM;
using Xunit;

namespace GnollHackX.UnitTests
{
    /* Covers what macOS Option combinations do; the cases are what the macOS keyboard layouts report */
    public class GHAvaloniaKeyTextTests
    {
        /* Allow Option Keys off. German Option+L is @, Danish Option+I is |, Spanish Option+N is >, AZERTY
           Option+Shift+L is |: all stay the letter's meta command */
        [Theory]
        [InlineData('l', "@")]
        [InlineData('i', "|")]
        [InlineData('n', ">")]
        [InlineData('L', "|")]
        [InlineData('a', "a")]
        public void OffLetterKeysAreMetaEvenWhenTheyTypeAscii(char baseCharacter, string symbol)
        {
            Assert.Equal(GHMacOptionKeyKind.MetaLetter, GHAvaloniaKeyText.ClassifyMacOption(baseCharacter, symbol, false));
        }

        /* Option on a letter key usually types a non-ASCII character, or starts a dead key */
        [Theory]
        [InlineData('z', 0x03A9)]
        [InlineData('e', 0x00B4)]
        [InlineData('q', 0x2021)]
        public void LetterKeysWithNonAsciiSymbolsAreMeta(char baseCharacter, int symbol)
        {
            string text = ((char)symbol).ToString();
            Assert.Equal(GHMacOptionKeyKind.MetaLetter, GHAvaloniaKeyText.ClassifyMacOption(baseCharacter, text, false));
            Assert.Equal(GHMacOptionKeyKind.MetaLetter, GHAvaloniaKeyText.ClassifyMacOption(baseCharacter, text, true));
        }

        /* Allow Option Keys off: Finnish Option+2 is @ but stays the routing of today, meta 2 */
        [Theory]
        [InlineData('2', "@")]
        [InlineData('8', "[")]
        [InlineData((char)0x011B, "@")]
        public void OffOtherKeysStayDefault(char baseCharacter, string symbol)
        {
            Assert.Equal(GHMacOptionKeyKind.Default, GHAvaloniaKeyText.ClassifyMacOption(baseCharacter, symbol, false));
        }

        /* Allow Option Keys on: any key that types printable ASCII types it, letter keys included */
        [Theory]
        [InlineData('2', "@")]
        [InlineData('8', "[")]
        [InlineData('7', "\\")]
        [InlineData('l', "@")]
        [InlineData('i', "|")]
        [InlineData((char)0x011B, "@")]
        [InlineData((char)0x00F6, "\\")]
        [InlineData('\0', "@")]
        public void OnAsciiSymbolsAreText(char baseCharacter, string symbol)
        {
            Assert.Equal(GHMacOptionKeyKind.Text, GHAvaloniaKeyText.ClassifyMacOption(baseCharacter, symbol, true));
        }

        /* Finnish Option+1 is the copyright sign; a space, DEL or a multi-character symbol is never text */
        [Theory]
        [InlineData('1', 0x00A9)]
        [InlineData('1', 0x20)]
        [InlineData('1', 0x7F)]
        [InlineData('1', 0x0D)]
        public void OnNonAsciiSymbolsStayDefault(char baseCharacter, int symbol)
        {
            Assert.Equal(GHMacOptionKeyKind.Default, GHAvaloniaKeyText.ClassifyMacOption(baseCharacter, ((char)symbol).ToString(), true));
        }

        [Theory]
        [InlineData('1', null)]
        [InlineData('1', "")]
        [InlineData('1', "ab")]
        [InlineData('\0', "@")]
        public void UnknownOrEmptyStaysDefault(char baseCharacter, string symbol)
        {
            Assert.Equal(GHMacOptionKeyKind.Default, GHAvaloniaKeyText.ClassifyMacOption(baseCharacter, symbol, false));
        }
    }
}
