using System;

namespace GnollHackX
{
    /* The page-specific labels and actions of GHWebGamepadNavigator */
    public sealed class GHWebGamepadProfile
    {
        /* The Menu hint */
        public string CloseLabel { get; set; }

        /* Run when B has nothing to go back to; null closes the page */
        public Action BackFallback { get; set; }

        /* The B hint when B has nothing to go back to */
        public string BackFallbackLabel { get; set; }

        /* The data-gnh-pad-key name of the element X focuses and activates; null makes X go forward in history */
        public string ComposeKey { get; set; }

        /* The X hint with ComposeKey */
        public string ComposeLabel { get; set; }

        /* The page script call of L3 */
        public string L3Call { get; set; }

        /* The L3 hint; null shows none */
        public string L3Label { get; set; }

        /* The LB/RB hint */
        public string SectionLabel { get; set; }

        /* The Y hint */
        public string SearchLabel { get; set; }

        /* Web pages such as the wiki */
        public static GHWebGamepadProfile CreateDefault()
        {
            GHWebGamepadProfile profile = new GHWebGamepadProfile();
            profile.CloseLabel = "Close";
            profile.BackFallback = null;
            profile.BackFallbackLabel = "Close";
            profile.ComposeKey = null;
            profile.ComposeLabel = null;
            profile.L3Call = "top()";
            profile.L3Label = null;
            profile.SectionLabel = "Section";
            profile.SearchLabel = "Search";
            return profile;
        }

        /* The Overseer chat; the page sets BackFallback */
        public static GHWebGamepadProfile CreateOverseer()
        {
            GHWebGamepadProfile profile = new GHWebGamepadProfile();
            profile.CloseLabel = "Leave";
            profile.BackFallback = null;
            profile.BackFallbackLabel = "Leave";
            profile.ComposeKey = "compose";
            profile.ComposeLabel = "Write";
            profile.L3Call = "bottom('m')";
            profile.L3Label = "Newest";
            profile.SectionLabel = "Message";
            profile.SearchLabel = "Search";
            return profile;
        }
    }
}
