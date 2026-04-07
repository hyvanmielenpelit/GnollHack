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

        public StoredManual(string name, int id, string text) : this()
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
        public int CategoryId;
        public int SubCategoryId;
        public bool IsVisible; /* If false, the achievement is not listed */
        public int MadeVisibleById;

        public Achievement()
        {

        }

        public Achievement(string name, string description, int categoryId, int subCategoryId, 
            bool isVisible, int madeVisibleById) : this()
        {
            Name = name;
            Description = description;
            Name = name;
            CategoryId = categoryId;
            SubCategoryId = subCategoryId;
            IsVisible = isVisible;
            MadeVisibleById = madeVisibleById;
        }

        public Achievement(string name, string description, int categoryId, int subCategoryId, bool isVisible = true) 
            : this(name, description, categoryId, subCategoryId, isVisible, 0)
        {

        }

        public Achievement(string name, string description, int categoryId, int subCategoryId, int madeVisibleById)
            : this(name, description, categoryId, subCategoryId, false, madeVisibleById)
        {

        }
    }

    public class AchievementCategory
    {
        public string Name;

        public AchievementCategory()
        {

        }

        public AchievementCategory(string name) : this()
        {
            Name = name;
        }
    }

    public class AchievementBundle
    {
        public int AchievementId;
        public List<int> BundleAchievements;

        public AchievementBundle()
        {
            BundleAchievements = new List<int>();
        }

        public AchievementBundle(int achievementId, List<int> bundleAchievements  ) : this()
        {
            AchievementId = achievementId;
            BundleAchievements = bundleAchievements;
        }
    }

}
