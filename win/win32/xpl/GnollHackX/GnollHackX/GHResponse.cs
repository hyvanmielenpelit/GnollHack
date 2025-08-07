using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackX
{
    public sealed class GHResponse
    {
        public readonly GHRequestType RequestType;
        public readonly string ResponseStringValue;
        public readonly int ResponseIntValue;
        public readonly uint ResponseUIntValue;
        public readonly bool ResponseBoolValue;
        public readonly List<GHMenuItem> SelectedMenuItems;
        public GHGame RequestingGame { get; }
        public GHWindow RequestingGHWindow { get; }
        public readonly int X;
        public readonly int Y;
        public readonly int Mod;

        public readonly float GeneralVolume, MusicVolume, AmbientVolume, DialogueVolume, EffectsVolume, GameUIVolume;

        public GHResponse()
        {

        }

        public GHResponse(GHGame ghGame, GHRequestType requesttype)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
        }

        public GHResponse(GHGame ghGame, GHRequestType requesttype, string responsestringvalue)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ResponseStringValue = responsestringvalue;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, int responseintvalue)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ResponseIntValue = responseintvalue;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, uint responseuintvalue)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ResponseUIntValue = responseuintvalue;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, bool responseboolvalue)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            ResponseBoolValue = responseboolvalue;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, GHWindow window, List<GHMenuItem> responselist, bool wasCancelled)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            RequestingGHWindow = window;
            SelectedMenuItems = responselist;
            ResponseBoolValue = wasCancelled;
        }
        public GHResponse(GHGame ghGame, GHRequestType requesttype, int x, int y, int mod)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            X = x;
            Y = y;
            Mod = mod;
        }

        public GHResponse(GHGame ghGame, GHRequestType requesttype, float generalVolume, float musicVolume, float ambientVolume, float dialogueVolume, float effectsVolume, float gameUIVolume)
        {
            RequestingGame = ghGame;
            RequestType = requesttype;
            GeneralVolume = generalVolume;
            MusicVolume = musicVolume;
            AmbientVolume = ambientVolume;
            DialogueVolume = dialogueVolume;
            EffectsVolume = effectsVolume;
            GameUIVolume = gameUIVolume;
        }
    }
}
