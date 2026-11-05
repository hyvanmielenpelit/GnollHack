namespace GnollHackM
{
    internal enum GHMacOptionKeyKind
    {
        /* Routed as before: the meta command of Avalonia's key, or of the key symbol */
        Default,
        /* The meta command of the key's own letter */
        MetaLetter,
        /* The character the layout types with Option, through TextInput */
        Text,
    }

    /* Keyboard text rules of the Avalonia desktop target that need no Avalonia types */
    internal static class GHAvaloniaKeyText
    {
        /* macOS: what an Option combination does. baseCharacter is what the key types with no modifiers in the current
           layout, or '\0' when unknown; keySymbol is what it types with Option, as Avalonia.Native reports it.
           With allowOptionKeys (the Allow Option Keys setting), Option types any printable ASCII character the layout
           puts on the key, such as @ on Option+2 in Finnish or Option+L in German. Otherwise a key whose own character
           is a-z is that letter's meta command, so Option+Z is M-z on QWERTZ and Option+A is M-a on AZERTY. */
        public static GHMacOptionKeyKind ClassifyMacOption(char baseCharacter, string keySymbol, bool allowOptionKeys)
        {
            if (allowOptionKeys && keySymbol != null && keySymbol.Length == 1 && keySymbol[0] >= '!' && keySymbol[0] <= '~')
                return GHMacOptionKeyKind.Text;
            if ((baseCharacter >= 'a' && baseCharacter <= 'z') || (baseCharacter >= 'A' && baseCharacter <= 'Z'))
                return GHMacOptionKeyKind.MetaLetter;
            return GHMacOptionKeyKind.Default;
        }
    }
}
