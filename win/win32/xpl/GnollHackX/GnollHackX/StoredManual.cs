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
        public string SortName; /* Special name used for sorting */
        public int CategoryId;
        public int SubCategoryId;
        public bool IsVisible; /* If false, the achievement is not listed */
        public int MadeVisibleById;

        public Achievement()
        {

        }

        public Achievement(string name, string description, string sortName, int categoryId, int subCategoryId, 
            bool isVisible, int madeVisibleById) : this()
        {
            Name = name;
            Description = description;
            SortName = sortName != null ? sortName : name;
            CategoryId = categoryId;
            SubCategoryId = subCategoryId;
            IsVisible = isVisible;
            MadeVisibleById = madeVisibleById;
        }

        public Achievement(string name, string description, int categoryId, int subCategoryId, bool isVisible = true) 
            : this(name, description, null, categoryId, subCategoryId, isVisible, 0)
        {

        }

        public Achievement(string name, string description, int categoryId, int subCategoryId, int madeVisibleById)
            : this(name, description, null, categoryId, subCategoryId, false, madeVisibleById)
        {

        }

        public Achievement(string name, string description, string sortName, int categoryId, int subCategoryId, bool isVisible = true)
            : this(name, description, sortName, categoryId, subCategoryId, isVisible, 0)
        {

        }

        public Achievement(string name, string description, string sortName, int categoryId, int subCategoryId, int madeVisibleById)
            : this(name, description, sortName, categoryId, subCategoryId, false, madeVisibleById)
        {

        }
    }

    public class AchievementCategory
    {
        public readonly string Name;

        public AchievementCategory()
        {
            Name = "";
        }

        public AchievementCategory(string name)
        {
            Name = name;
        }
    }

    public class AchievementTier
    {
        public readonly string Name;
        public readonly string SubName;
        public readonly List<List<int>> UnlockAchievements; /* Outer List is OR and inner List AND */

        public AchievementTier()
        {
            Name = "";
            UnlockAchievements = new List<List<int>>();
        }

        public AchievementTier(string name, List<List<int>> unlockAchievements)
        {
            Name = name;
            UnlockAchievements = unlockAchievements;
        }

        public AchievementTier(string name, string subName, List<List<int>> unlockAchievements)
        {
            Name = name;
            SubName = subName;
            UnlockAchievements = unlockAchievements;
        }

        public AchievementTier(string name, int unlockAchievement)
        {
            Name = name;
            UnlockAchievements = new List<List<int>>() { new List<int>() { unlockAchievement } };
        }

        public AchievementTier(string name, string subName, int unlockAchievement)
        {
            Name = name;
            SubName = subName;
            UnlockAchievements = new List<List<int>>() { new List<int>() { unlockAchievement } };
        }
    }

    public class AchievementBundle
    {
        public readonly int AchievementId;
        public readonly List<int> BundleAchievements;

        public AchievementBundle()
        {
            BundleAchievements = new List<int>();
        }

        public AchievementBundle(int achievementId, List<int> bundleAchievements  )
        {
            AchievementId = achievementId;
            BundleAchievements = bundleAchievements;
        }
    }

}
