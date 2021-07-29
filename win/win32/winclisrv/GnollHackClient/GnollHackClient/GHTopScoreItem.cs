using System;
using System.Collections.Generic;
using System.Text;

namespace GnollHackClient
{
    public class GHTopScoreItem
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
        public string DifficultyString
        {
            get
            {
                string res = "";
                if (Mode == "wizard")
                    res += "W";
                else if (Mode == "explore")
                    res += "X";

                switch (Difficulty)
                {
                    case -4:
                        res += "E";
                        break;
                    case -3:
                        res += "e";
                        break;
                    case -2:
                        res += "a";
                        break;
                    case -1:
                        res += "h";
                        break;
                    case 0:
                        res += "x";
                        break;
                    case 1:
                        res += "m";
                        break;
                    case 2:
                        res += "M";
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

        public GHTopScoreItem()
        {

        }

        public void AddXlogLineItemData(string xloglineitem)
        {
            if (xloglineitem == null || xloglineitem == "")
                return;

            string[] elements = xloglineitem.Split('=');
            if (elements.Length < 2)
                return;

            int parseint = 0;
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
        }
    }
}
