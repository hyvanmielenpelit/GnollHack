using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Text;
using static System.Net.Mime.MediaTypeNames;

namespace GnollHackX
{
    public class StoredManual
    {
        public string Name;
        public int Id;
        public string Text;

        public StoredManual() 
        {
            
        }

        public StoredManual(string name, int id, string text) : base()
        {
            Name = name;
            Id = id;
            Text = text;
        }
    }

    public class Achievement
    {
        public string Name;
        public int Id;
        public int CategoryId;

        public Achievement()
        {

        }

        public Achievement(string name, int id, int categoryId) : base()
        {
            Name = name;
            Id = id;
            CategoryId = categoryId;
        }
    }

    public class AchievementCategory
    {
        public string Name;
        public int Id;

        public AchievementCategory()
        {

        }

        public AchievementCategory(string name, int id) : base()
        {
            Name = name;
            Id = id;
        }
    }
}
