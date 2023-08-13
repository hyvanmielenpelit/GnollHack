using GnollHackX.Pages.MainScreen;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Text;

namespace GnollHackX
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
        public int HP { get; set; }
        public int HPMax { get; set; }
        public int Turns { get; set; }
        public int dnum { get; set; }
        public int dlevel { get; set; }
        public int Difficulty { get; set; }
        public int UID { get; set; }
        public long StartTime { get; set; }
        public long EndTime { get; set; }

        public string DifficultyString
        {
            get
            {
                string res = "";
                if (Mode == "wizard")
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
                return res;
            }
        }
        public string Mode { get; set; }
        public int BirthDate { get; set; }

        TopScorePage _page = null;

        public event PropertyChangedEventHandler PropertyChanged;

        public GHTopScoreItem(TopScorePage topScorePage)
        {
            _page = topScorePage;
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
            else if (elements[0] == "turns")
            {
                if (int.TryParse(elements[1], out parseint))
                    Turns = parseint;
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
                if (int.TryParse(elements[1], out parseint))
                    BirthDate = parseint;
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

        public bool IsDumplogButtonEnabled
        {
            get
            {
                //string filename = GetDumplogFileName();
                //string fulltargetpath = Path.Combine(GHApp.GHPath, "dumplog", filename);
                //bool res = File.Exists(fulltargetpath);
                //return res;
                return true;
            }
        }

        private bool _raceVisible;
        public bool IsRaceVisible
        {
            get
            {
                bool newvalue = _page.IsRaceVisible;
                if (newvalue != _raceVisible)
                {
                    _raceVisible = newvalue;
                    OnPropertyChanged("IsRaceVisible");
                }
                return _raceVisible;
            }
        }

        private bool _genderVisible;
        public bool IsGenderVisible
        {
            get
            {
                bool newvalue = _page.IsGenderVisible;
                if (newvalue != _genderVisible)
                {
                    _genderVisible = newvalue;
                    OnPropertyChanged("IsGenderVisible");
                }
                return _genderVisible;
            }
        }

        private bool _alignmentVisible;
        public bool IsAlignmentVisible
        {
            get
            {
                bool newvalue = _page.IsAlignmentVisible;
                if (newvalue != _alignmentVisible)
                {
                    _alignmentVisible = newvalue;
                    OnPropertyChanged("IsAlignmentVisible");
                }
                return _alignmentVisible;
            }
        }

        private bool _hpVisible;
        public bool IsHPVisible
        {
            get
            {
                bool newvalue = _page.IsHPVisible;
                if (newvalue != _hpVisible)
                {
                    _hpVisible = newvalue;
                    OnPropertyChanged("IsHPVisible");
                }
                return _hpVisible;
            }
        }

        private bool _hpMaxVisible;
        public bool IsHPMaxVisible
        {
            get
            {
                bool newvalue = _page.IsHPMaxVisible;
                if (newvalue != _hpMaxVisible)
                {
                    _hpMaxVisible = newvalue;
                    OnPropertyChanged("IsHPMaxVisible");
                }
                return _hpMaxVisible;
            }
        }


        private bool _turnsVisible;
        public bool IsTurnsVisible
        {
            get
            {
                bool newvalue = _page.IsTurnsVisible;
                if (newvalue != _turnsVisible)
                {
                    _turnsVisible = newvalue;
                    OnPropertyChanged("IsTurnsVisible");
                }
                return _turnsVisible;
            }
        }

        private bool _birthDateVisible;
        public bool IsBirthDateVisible
        {
            get
            {
                bool newvalue = _page.IsBirthDateVisible;
                if (newvalue != _birthDateVisible)
                {
                    _birthDateVisible = newvalue;
                    OnPropertyChanged("IsBirthDateVisible");
                }
                return _birthDateVisible;
            }
        }

        private bool _outcomeVisible;
        public bool IsOutcomeVisible
        {
            get
            {
                bool newvalue = _page.IsOutcomeVisible;
                if (newvalue != _outcomeVisible)
                {
                    _outcomeVisible = newvalue;
                    OnPropertyChanged("IsOutcomeVisible");
                }
                return _outcomeVisible;
            }
        }

        private void OnPropertyChanged(string propertyname)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyname));
        }
    }
}
