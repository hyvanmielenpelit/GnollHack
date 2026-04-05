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
        public bool IsLocked; /* Achievement cannot be gained yet; shows locked instead (normally cannot be achieved before some other achievement is gained) */
        public bool IsKnown; /* If false, unknown what the achievement is; shows question mark instead */
        public bool IsVisible; /* If false, the achievement is not listed */
        public int MadeUnlockedById;
        public int MadeKnownById;
        public int MadeVisibleById;

        public Achievement()
        {

        }

        public Achievement(string name, string description, int categoryId, int subCategoryId, 
            bool isLocked, bool isKnown, bool isVisible,
            int madeUnlockedById, int madeKnownById, int madeVisibleById) : this()
        {
            Name = name;
            Description = description;
            Name = name;
            CategoryId = categoryId;
            SubCategoryId = subCategoryId;
            IsLocked = isLocked;
            IsKnown = isKnown;
            IsVisible = isVisible;
            MadeUnlockedById = madeUnlockedById;
            MadeKnownById = madeKnownById;
            MadeVisibleById = madeVisibleById;
        }

        public Achievement(string name, string description, int categoryId, int subCategoryId) 
            : this(name, description, categoryId, subCategoryId, false, true, true, 0, 0, 0)
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
}
