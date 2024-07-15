using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHSoundTrack
    {
        public string DisplayName = "";
        public string Songwriter = "";
        public int Length = 0; /* in seconds */

        public string GameUsageDescription = "";
        public string Notes = "";

        public List<int> GHSoundList = new List<int>();
        public List<string> GameAppearanceList = new List<string>();

        public GHSoundTrack()
        {

        }


        public static List<GHSoundTrack> GnollHackSoundTracks = new List<GHSoundTrack>() 
        { 
            new GHSoundTrack() 
            { 
                DisplayName = "Song", 
                Songwriter = "Avalkis", 
                Length = 0, 
                GameUsageDescription = "",
                GHSoundList = new List<int>()
                { 0, 1 }
            },
            new GHSoundTrack(), 
        };

    }
}
