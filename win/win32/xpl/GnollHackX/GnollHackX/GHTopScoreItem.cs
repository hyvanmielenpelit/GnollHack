using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Text;
#if GNH_MAUI
using GnollHackX;
namespace GnollHackM
#else
using GnollHackX.Pages.MainScreen;
using Xamarin.Forms;
namespace GnollHackX
#endif
{
    public class GHTopScoreItem : INotifyPropertyChanged
    {
        public int Rank { get; set; }
        public string Name { get; set; }
        public int Score { get; set; }
        public string Outcome { get; set; }
        public string Role { get; set; }
        public string Race { get; set; }
        public string Gender { get; set; }
        public string Alignment { get; set; }
        public int XPLevel { get; set; }
        public int HP { get; set; }
        public int HPMax { get; set; }
        public int Turns { get; set; }
        public long RealTime { get; set; }
        public int dnum { get; set; }
        public int dlevel { get; set; }
        public int Difficulty { get; set; }
        public int UID { get; set; }
        public long StartTime { get; set; }
        public long EndTime { get; set; }

        public string CharacterHPModeString
        {
            get
            {
                return CharacterString + " " + HPString + " " + DifficultyString;
            }
        }

        public string CharacterString
        {
            get
            {
                return Role + " " + Race + " " + Gender + " " + Alignment + (XPLevel > 0 ? " XL:" +XPLevel : ""); 
            }
        }
        public string HPString
        {
            get
            {
                return "HP:" + HP + "/" + HPMax;
            }
        }
        public string TurnTimeString
        {
            get
            {
                return "on " + DeathDateString + " " + TurnString + " (" + RealTimeString + ")";
            }
        }

        public string TurnString
        {
            get
            {
                return "T:" + Turns;
            }
        }

        public string RealTimeString
        {
            get
            {
                long GameDurationTime = RealTime;
                long GameDurationDays = GameDurationTime / (3600 * 24);
                long GameDurationHours = (GameDurationTime / 3600) % 24;
                long GameDurationMinutes = (GameDurationTime % 3600) / 60;
                long GameDurationSeconds = GameDurationTime - GameDurationHours * 3600 - GameDurationMinutes * 60;
                return
                    (GameDurationDays > 0 ? GameDurationDays + "d:" : "") +
                    (GameDurationDays > 0 || GameDurationHours > 0 ? GameDurationHours + "h:" : "") +
                    (GameDurationDays > 0 || GameDurationHours > 0 || GameDurationMinutes > 0 ? GameDurationMinutes + "m:" : "") + GameDurationSeconds + "s";
            }
        }

        public string DeathDateString
        {
            get
            {
                string str = DeathDate.ToString();
                if (str.Length == 8)
                {
                    string res = str.Substring(0,4) + "-" + str.Substring(4, 2) + "-" + str.Substring(6, 2);
                    return res;
                }
                else if(DeathDate > 0)
                {
                    return str;
                }
                else
                    return "";
            }
        }
        public string DifficultyString
        {
            get
            {
                string res = "";
                if (Mode == "wizard" || Mode == "debug")
                    res += "W";
                else if (Mode == "explore")
                    res += "X";
                else if (Mode == "casual")
                    res += "C";
                else if (Mode == "reloadable")
                    res += "R";
                else if (Mode == "modern")
                    res += "M";

                switch (Difficulty)
                {
                    case -4:
                        res += "s";
                        break;
                    case -3:
                        res += "e";
                        break;
                    case -2:
                        res += "a";
                        break;
                    case -1:
                        res += "v";
                        break;
                    case 0:
                        res += "x";
                        break;
                    case 1:
                        res += "m";
                        break;
                    case 2:
                        res += "g";
                        break;
                    default:
                        res += "?";
                        break;
                }
                return "@:" + res;
            }
        }
        public string Mode { get; set; }
        public long BirthDate { get; set; }
        public long DeathDate { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

        public GHTopScoreItem(string line)
        {
            if (string.IsNullOrEmpty(line))
                return;
            string[] lineitems = line.Split('\t');
            if(lineitems == null) 
                return;
            foreach (string lineitem in lineitems)
            {
                AddXlogLineItemData(lineitem);
            }

            string fulltargetpath = Path.Combine(GHApp.GHPath, GHConstants.DumplogDirectory, GetDumplogFileName());
            string fullhtmltargetpath = Path.Combine(GHApp.GHPath, GHConstants.DumplogDirectory, GetHTMLDumplogFileName());
            bool dumplogexists = File.Exists(fulltargetpath);
            bool htmldumplogexists = File.Exists(fullhtmltargetpath);
            _dumplogFileFound = GHApp.UseSingleDumpLog ? (GHApp.UseHTMLDumpLogs ? htmldumplogexists : dumplogexists) : htmldumplogexists || dumplogexists;
        }

        public void AddXlogLineItemData(string xloglineitem)
        {
            if (xloglineitem == null || xloglineitem == "")
                return;

            string[] elements = xloglineitem.Split('=');
            if (elements.Length < 2)
                return;

            int parseint = 0;
            long parselong = 0;
            if (elements[0] == "name")
            {
                Name = elements[1];
            }
            else if (elements[0] == "death")
            {
                Outcome = elements[1];
            }
            else if (elements[0] == "mode")
            {
                Mode = elements[1];
            }
            else if (elements[0] == "points")
            {
                if (int.TryParse(elements[1], out parseint))
                    Score = parseint;
            }
            else if (elements[0] == "hp")
            {
                if (int.TryParse(elements[1], out parseint))
                    HP = parseint;
            }
            else if (elements[0] == "maxhp")
            {
                if (int.TryParse(elements[1], out parseint))
                    HPMax = parseint;
            }
            else if (elements[0] == "difficulty")
            {
                if (int.TryParse(elements[1], out parseint))
                    Difficulty = parseint;
            }
            else if (elements[0] == "role")
            {
                Role = elements[1];
            }
            else if (elements[0] == "race")
            {
                Race = elements[1];
            }
            else if (elements[0] == "gender")
            {
                Gender = elements[1];
            }
            else if (elements[0] == "align")
            {
                Alignment = elements[1];
            }
            else if (elements[0] == "xplvl")
            {
                if (int.TryParse(elements[1], out parseint))
                    XPLevel = parseint;
            }
            else if (elements[0] == "turns")
            {
                if (int.TryParse(elements[1], out parseint))
                    Turns = parseint;
            }
            else if (elements[0] == "realtime")
            {
                if (long.TryParse(elements[1], out parselong))
                    RealTime = parselong;
            }
            else if (elements[0] == "deathdnum")
            {
                if (int.TryParse(elements[1], out parseint))
                    dnum = parseint;
            }
            else if (elements[0] == "deathdlevel")
            {
                if (int.TryParse(elements[1], out parseint))
                    dlevel = parseint;
            }
            else if (elements[0] == "birthdate")
            {
                if (long.TryParse(elements[1], out parselong))
                    BirthDate = parselong;
            }
            else if (elements[0] == "deathdate")
            {
                if (long.TryParse(elements[1], out parselong))
                    DeathDate = parselong;
            }
            else if (elements[0] == "uid")
            {
                if (int.TryParse(elements[1], out parseint))
                    UID = parseint;
            }
            else if (elements[0] == "starttime")
            {
                if (long.TryParse(elements[1], out parselong))
                    StartTime = parselong;
            }
            else if (elements[0] == "endtime")
            {
                if (long.TryParse(elements[1], out parselong))
                    EndTime = parselong;
            }
        }

        public string FormattedName
        {
            get
            {
                string name = Name;
                if (name == null)
                    return "";
                string formatted = name.Replace('\\', '_');
                formatted = formatted.Replace('/', '_');
                formatted = formatted.Replace(' ', '_');
                return formatted;
            }
        }
        public string GetDumplogFileName()
        {
            string startdatestring = GHApp.GnollHackService.DumplogDateString(StartTime);
            return "gnollhack." + FormattedName + "." + startdatestring + ".txt";
        }
        public string GetHTMLDumplogFileName()
        {
            string startdatestring = GHApp.GnollHackService.DumplogDateString(StartTime);
            return "gnollhack." + FormattedName + "." + startdatestring + ".html";
        }

        private bool _dumplogFileFound = false;
        public bool IsDumplogButtonEnabled
        {
            get
            {
                return _dumplogFileFound;
            }
        }

        public Color ItemTextColor
        {
            get
            {
                if(IsDumplogButtonEnabled)
                    return GHApp.DarkMode ? GHColors.LightBlue : GHColors.DarkBlue;
                else
                    return GHApp.DarkMode ? GHColors.White : GHColors.Black;
            }
        }

        private void OnPropertyChanged(string propertyname)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyname));
        }
    }
}
