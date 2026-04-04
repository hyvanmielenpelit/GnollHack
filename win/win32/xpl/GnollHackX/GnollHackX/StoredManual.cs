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
        public string Description;
        public int Id;
        public int CategoryId;
        public int SubCategoryId;
        public bool IsUnknown; /* Visible but unknown what the achievement is */

        public Achievement()
        {

        }

        public Achievement(string name, string description, int id, int categoryId, int subCategoryId, bool isUnknown) : base()
        {
            Name = name;
            Description = description;
            Name = name;
            CategoryId = categoryId;
            SubCategoryId = subCategoryId;
            IsUnknown = isUnknown;
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
