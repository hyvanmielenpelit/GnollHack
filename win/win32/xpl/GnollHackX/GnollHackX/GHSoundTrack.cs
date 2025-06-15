using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public class GHSoundTrack
    {
        public string DisplayName = "";
        public int Length = 0; /* in seconds */
        public string Songwriter = "";

        public string Contact = "";
        public string Notes = "";

        public int GHSoundForPlayback = -1; /* If the first element of GHSoundList is not appropriate (e.g., if it includes many songs) */
        public List<int> GHSoundList = new List<int>();
        public List<string> GameAppearanceList = new List<string>();

        public GHSoundTrack()
        {

        }


        public static List<GHSoundTrack> GnollHackSoundTracks = new List<GHSoundTrack>() 
        { 
            new GHSoundTrack() 
            { 
                DisplayName = "Tower Defense", 
                Length = 114,
                Songwriter = "Eric Matyas", 
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 1, 8 },
                GameAppearanceList = new List<string>()
                { "Main Screen", "Underground Castle" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Mysterious Deep",
                Length = 179,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 3, 5, 149, 668 },
                GameAppearanceList = new List<string>()
                { "Game Introduction", "Oracle", "Plane of Water", "Main Menu" },
            },
            new GHSoundTrack()
            {
                DisplayName = "RPG Map Music",
                Length = 77,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 2 },
                GameAppearanceList = new List<string>()
                { "Player Selection" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Our Mountain",
                Length = 71,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundForPlayback = 2043,
                GHSoundList = new List<int>()
                { 4 },
                GameAppearanceList = new List<string>()
                { "Main Dungeon" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Of Castles, Magic and Spells",
                Length = 31,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 6 },
                GameAppearanceList = new List<string>()
                { "Temple" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Something Freaky This Way Comes",
                Length = 54,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 7 },
                GameAppearanceList = new List<string>()
                { "Medusa" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Haunted Tunnels",
                Length = 84,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 10, 147 },
                GameAppearanceList = new List<string>()
                { "Gnomish Mines", "Plane of Earth" },
            },
            new GHSoundTrack()
            {
                DisplayName = "RPG Theme",
                Length = 90,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 11 },
                GameAppearanceList = new List<string>()
                { "Mine Town" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Dreaming in Puzzles",
                Length = 131,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 12 },
                GameAppearanceList = new List<string>()
                { "Sokoban" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Dream Voyager",
                Length = 141,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 13, 148 },
                GameAppearanceList = new List<string>()
                { "Gehennom", "Plane of Air" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Welcome to the Mansion",
                Length = 141,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 14, 15, 16, 141 },
                GameAppearanceList = new List<string>()
                { "Valley of the Dead", "Orcus Town", "Vlad's Tower", "Sanctum", "Game Over" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Of Legends and Fables 2",
                Length = 58,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 17 },
                GameAppearanceList = new List<string>()
                { "Shop" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Move It Out",
                Length = 48,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 18 },
                GameAppearanceList = new List<string>()
                { "Angry Shopkeeper" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Lurking in the Shadows",
                Length = 94,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 19 },
                GameAppearanceList = new List<string>()
                { "Dead Shopkeeper" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Disturbing Chimes",
                Length = 64,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 20 },
                GameAppearanceList = new List<string>()
                { "Deserted Shop" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Dark Fantasy Open",
                Length = 64,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 142, 146, 150 },
                GameAppearanceList = new List<string>()
                { "Plane of the Modron", "Endgame", "Plane of Fire" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Into Battle",
                Length = 55,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 144 },
                GameAppearanceList = new List<string>()
                { "Hellish Pastures" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Clock-Maker the Hero",
                Length = 94,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 145 },
                GameAppearanceList = new List<string>()
                { "Large Circular Dungeon" },
            },
            new GHSoundTrack()
            {
                DisplayName = "RPG Battle Climax",
                Length = 69,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 152 },
                GameAppearanceList = new List<string>()
                { "Ascension" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Ye Olde Pub",
                Length = 71,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 172 },
                GameAppearanceList = new List<string>()
                { "Smith" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Once Around the Kingdom",
                Length = 100,
                Songwriter = "Eric Matyas",
                Contact = "soundimage.org",
                GHSoundList = new List<int>()
                { 173 },
                GameAppearanceList = new List<string>()
                { "Various NPC Rooms" },
            },

            new GHSoundTrack()
            {
                DisplayName = "The Sword and the Raven",
                Length = 226,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundForPlayback = 2044,
                GHSoundList = new List<int>()
                { 4 },
                GameAppearanceList = new List<string>()
                { "Main Dungeon" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Home",
                Length = 188,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 139 },
                GameAppearanceList = new List<string>()
                { "Quest" },
            },
            new GHSoundTrack()
            {
                DisplayName = "A Dawn in Paradise",
                Length = 188,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 140 },
                GameAppearanceList = new List<string>()
                { "Quest Completed" },
            },
            new GHSoundTrack()
            {
                DisplayName = "A Key to the Mountains",
                Length = 226,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 9, 182 },
                GameAppearanceList = new List<string>()
                { "Court Room", "Fort Ludious" },
            },
            new GHSoundTrack()
            {
                DisplayName = "A Promise of Friendship",
                Length = 284,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 143 },
                GameAppearanceList = new List<string>()
                { "Modron Temple" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Return to the Meadows",
                Length = 168,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 151 },
                GameAppearanceList = new List<string>()
                { "Astral Plane" },
            },
            new GHSoundTrack()
            {
                DisplayName = "A New Path",
                Length = 222,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 174 },
                GameAppearanceList = new List<string>()
                { "Various NPC Rooms" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Forest Celebration",
                Length = 130,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 175 },
                GameAppearanceList = new List<string>()
                { "Various NPC Rooms" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Dance of the Unicorn",
                Length = 130,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 176 },
                GameAppearanceList = new List<string>()
                { "Artificer" },
            },
            new GHSoundTrack()
            {
                DisplayName = "A Light in the Dark",
                Length = 165,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 177 },
                GameAppearanceList = new List<string>()
                { "Warp Engineer" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Troll Festivities",
                Length = 191,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 178 },
                GameAppearanceList = new List<string>()
                { "Various Hermits" },
            },
            new GHSoundTrack()
            {
                DisplayName = "After the Rose",
                Length = 177,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 179 },
                GameAppearanceList = new List<string>()
                { "Izchak's Shop" },
            },
            new GHSoundTrack()
            {
                DisplayName = "The Guiding Stars",
                Length = 164,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 180 },
                GameAppearanceList = new List<string>()
                { "Leprechaun Hall" },
            },
            new GHSoundTrack()
            {
                DisplayName = "Ambush",
                Length = 85,
                Songwriter = "Avalkis",
                Contact = "jukkaeronen.com",
                GHSoundList = new List<int>()
                { 181 },
                GameAppearanceList = new List<string>()
                { "Quantum Core" },
            },

            new GHSoundTrack()
            {
                DisplayName = "A Elbereth Gilthoniel",
                Length = 85,
                Songwriter = "Yolanda Mott",
                Contact = "yolandamott.com",
                GHSoundList = new List<int>()
                { 1839 },
                GameAppearanceList = new List<string>()
                { "Lindisse, Elven Bard" },
            },
        };
    }

    public class SoundTrackComparer : IComparer<GHSoundTrack>
    {
        public SoundTrackComparer() 
        {
            
        }

        public int Compare(GHSoundTrack track1, GHSoundTrack track2)
        {
            return string.Compare(track1.DisplayName, track2.DisplayName);
        }
    }
}
