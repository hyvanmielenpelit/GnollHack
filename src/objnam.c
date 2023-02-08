/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-01-06 */

/* GnollHack 4.0    objnam.c    $NHDT-Date: 1551138256 2019/02/25 23:44:16 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.235 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

const char* multishot_style_names[MAX_MULTISHOT_TYPES] = {
    "None",
    "1+1/4 per skill level after basic when firing",
    "2 when firing",
    "1/1.5/2 for basic/skilled/expert when firing",
    "1+1/4 per skill level after expert when thrown",
    "1+1/4 per skill level after skilled when thrown",
    "1+1/4 per skill level after basic when thrown",
    "1/1.5/2 for basic/skilled/expert in melee",
    "1+1/2 per skill level after basic in melee",
};

const char* armor_type_names[MAX_ARMOR_TYPES] = {
    "suit of armor", "shield", "helmet", "gloves", "boots", "cloak", "shirt", "robe", "bracers",
};

const char* weapon_type_names[MAX_WEAPON_TYPES] = {
    "short sword", "long sword", "two-handed sword", "broadsword", "bastard sword", "scimitar", "saber", "katana",  "tsurugi",
    "axe", "throwing axe", "two-handed axe", "dagger", "knife", "mace", "spiked mace", "flail", "morning star", "club", "spiked club",
    "hammer", "spiked hammer",  "staff", "rod", "spear", "javelin", "polearm", "trident",
    "dart", "shuriken", "boomerang", "short bow", "long bow", "hand crossbow", "crossbow", "repeating crossbow", "sling",
    "arrow", "quarrel", "sling-bullet", "pick-axe", "unicorn horn", "horn", "worm tooth",
    "shovel", "hook", "lance", "hose", "fork", "whip", "double-headed flail", "triple-headed flail"
};

const char* food_type_names[MAX_FOOD_TYPES] = {
    "general food", "ration", "fruit", "vegetable", "nut", "seed", "leaf", "kelp",  "bread",
    "cake", "candy", "mushroom", "mold", "glob", "egg", "meat", "jelly", "corpse",  "tin"
};

const char* misc_type_names[MAX_MISC_TYPES] = {
    "general item", "set of wings", "pair of pants", "skirt", "wrist watch",
    "nose ring", "headband", "pair of earrings",
    "ioun stone", "bracelet", "pair of bracers", "belt", "pair of eyeglasses", "blindfold", "scarf", "set of extra arms", "brooch", "mask", "necktie"
};

const char* misc_type_worn_texts[MAX_MISC_TYPES] = {
    "", "attached to back", "", "", "on left wrist",
    "on nose", "", "",
    "orbiting head", "", "", "", "", "", "", "attached to body", "", "", ""
};

const char* tool_type_names[MAX_TOOL_TYPES] = {
    "tool", "box", "chest", "bag", "pick-axe", "shovel", "long club-headed weapon", "hook", "horn", "candelabrum", "bell",
    "candle", "lamp", "lantern", "whistle", "flute", "harp", "drum", "saw", "jar", "can", "grail", "torch"
};

const char* book_type_names[MAX_BOOK_TYPES] = {
    "spellbook", "book", "novel", "manual", "tome", "grimoire", "codex"
};

const char* critical_strike_special_percentage_names[MAX_CRITICAL_STRIKE_SPECIAL_PERCENTAGES] = {
    "100% less 5% per hit die"
};

/* "an uncursed greased partly eaten guardian naga hatchling [corpse]" */
#define PREFIX 80 /* (56) */
#define SCHAR_LIM 127
#define NUMOBUF 12

STATIC_DCL char *FDECL(strprepend, (char *, const char *));
STATIC_DCL short FDECL(rnd_otyp_by_wpnskill, (SCHAR_P));
STATIC_DCL short FDECL(rnd_otyp_by_namedesc, (const char *, CHAR_P, int));
STATIC_DCL boolean FDECL(wishymatch, (const char *, const char *, BOOLEAN_P));
STATIC_DCL void FDECL(releaseobuf, (char *));
STATIC_DCL char *FDECL(minimal_xname, (struct obj *));
STATIC_DCL char *FDECL(doname_base, (struct obj *obj, unsigned));
STATIC_DCL char *FDECL(just_an, (char *str, const char *));
STATIC_DCL boolean FDECL(singplur_lookup, (char *, char *, BOOLEAN_P,
                                           const char *const *));
STATIC_DCL char *FDECL(singplur_compound, (char *));
STATIC_DCL char *FDECL(xname_flags, (struct obj *, unsigned));
STATIC_DCL boolean FDECL(badman, (const char *, BOOLEAN_P));
STATIC_DCL boolean FDECL(material_wish_success, (int, int));

struct Jitem {
    int item;
    const char *name;
};

#define BSTRCMPI(base, ptr, str) ((ptr) < base || strcmpi((ptr), str))
#define BSTRNCMPI(base, ptr, str, num) \
    ((ptr) < base || strncmpi((ptr), str, num))
#define Strcasecpy(dst, src) (void) strcasecpy(dst, src)

STATIC_OVL struct Jitem Japanese_items[] = { { SHORT_SWORD, "wakizashi" },
                                             { BROADSWORD, "ninja-to" },
                                             { FLAIL, "nunchaku" },
                                             { GLAIVE, "naginata" },
                                             { LOCK_PICK, "osaku" },
                                             { WOODEN_HARP, "koto" },
                                             { KNIFE, "shito" },
                                             { PLATE_MAIL, "tanko" },
                                             { HELMET, "kabuto" },
                                             { LEATHER_GLOVES, "yugake" },
                                             { FOOD_RATION, "gunyoki" },
                                             { 0, "" } };

STATIC_DCL const char *FDECL(Japanese_item_name, (int i));

STATIC_OVL char *
strprepend(s, pref)
register char *s;
register const char *pref;
{
    register int i = (int) strlen(pref);

    if (i > PREFIX) {
        impossible("PREFIX too short (for %d).", i);
        return s;
    }
    s -= i;
    (void) strncpy(s, pref, i); /* do not copy trailing 0 */
    return s;
}

/* manage a pool of BUFSZ buffers, so callers don't have to */
STATIC_VAR char NEARDATA obufs[NUMOBUF][BUFSZ];
STATIC_VAR int obufidx = 0;

char *
nextobuf()
{
    obufidx = (obufidx + 1) % NUMOBUF;
    return obufs[obufidx];
}

/* put the most recently allocated buffer back if possible */
STATIC_OVL void
releaseobuf(bufp)
char *bufp;
{
    /* caller may not know whether bufp is the most recently allocated
       buffer; if it isn't, do nothing; note that because of the somewhat
       obscure PREFIX handling for object name formatting by xname(),
       the pointer our caller has and is passing to us might be into the
       middle of an obuf rather than the address returned by nextobuf() */
    if (bufp >= obufs[obufidx]
        && bufp < obufs[obufidx] + sizeof obufs[obufidx]) /* obufs[][BUFSZ] */
        obufidx = (obufidx - 1 + NUMOBUF) % NUMOBUF;
}

char *
obj_typename(otyp)
register int otyp;
{
    char *buf = nextobuf();
    struct objclass *ocl = &objects[otyp];
    const char *actualn = OBJ_NAME(*ocl);
    const char *dn = OBJ_DESCR(*ocl);
    const char *un = ocl->oc_uname;
    int nn = ocl->oc_name_known;

    if (Role_if(PM_SAMURAI) && Japanese_item_name(otyp))
        actualn = Japanese_item_name(otyp);
    switch (ocl->oc_class) {
    case COIN_CLASS:
        Strcpy(buf, "coin");
        break;
    case POTION_CLASS:
        Strcpy(buf, "potion");
        break;
    case SCROLL_CLASS:
        Strcpy(buf, "scroll");
        break;
    case WAND_CLASS:
        Strcpy(buf, "wand");
        break;
    case SPBOOK_CLASS:
        Strcpy(buf, book_type_names[objects[otyp].oc_subtyp]);
        if (objects[otyp].oc_subtyp == BOOKTYPE_NOVEL || objects[otyp].oc_subtyp == BOOKTYPE_MANUAL)
        {
            if(!nn)
                Strcpy(buf, "book");
            nn = 0;
        }
        break;
    case RING_CLASS:
        Strcpy(buf, "ring");
        break;
    case AMULET_CLASS:
        if (nn)
            Strcpy(buf, actualn);
        else
            Strcpy(buf, "amulet");
        if (un)
            Sprintf(eos(buf), " called %s", un);
        if (dn)
            Sprintf(eos(buf), " (%s)", dn);
        return buf;
    default:
        if (nn) {
            Strcpy(buf, actualn);
            if (GemStone(otyp))
                Strcat(buf, " stone");
            if (un)
                Sprintf(eos(buf), " called %s", un);
            if (dn)
                Sprintf(eos(buf), " (%s)", dn);
        } else {
            Strcpy(buf, dn ? dn : actualn);
            if (ocl->oc_class == GEM_CLASS)
                Strcat(buf,
                       (ocl->oc_material == MAT_MINERAL) ? " stone" : " gem");
            if (un)
                Sprintf(eos(buf), " called %s", un);
        }
        return buf;
    }
    /* here for ring/scroll/potion/wand */
    if (nn) {
        if (is_otyp_unique(otyp))
            Strcpy(buf, actualn); /* avoid spellbook of Book of the Dead */
        else
            Sprintf(eos(buf), " of %s", actualn);
    }
    if (un)
        Sprintf(eos(buf), " called %s", un);
    if (dn)
        Sprintf(eos(buf), " (%s)", dn);
    return buf;
}

/* less verbose result than obj_typename(); either the actual name
   or the description (but not both); user-assigned name is ignored */
char *
simple_typename(otyp)
int otyp;
{
    char *bufp, *pp, *save_uname = objects[otyp].oc_uname;

    objects[otyp].oc_uname = 0; /* suppress any name given by user */
    bufp = obj_typename(otyp);
    objects[otyp].oc_uname = save_uname;
    if ((pp = strstri(bufp, " (")) != 0)
        *pp = '\0'; /* strip the appended description */
    return bufp;
}

boolean
obj_is_pname(obj)
struct obj *obj;
{
    if (!obj->oartifact || !has_oname(obj))
        return FALSE;
    if (!program_state.gameover && !iflags.override_ID) {
        if (!obj->nknown || !obj->aknown) //not_fully_identified(obj))
            return FALSE;
    }
    return TRUE;
}

/* used by distant_name() to pass extra information to xname_flags();
   it would be much cleaner if this were a parameter, but that would
   require all of the xname() and doname() calls to be modified */
STATIC_VAR int distantname = 0;

/* Give the name of an object seen at a distance.  Unlike xname/doname,
 * we don't want to set dknown if it's not set already.
 */
char *
distant_name(obj, func)
struct obj *obj;
char *FDECL((*func), (OBJ_P));
{
    char *str;

    /* 3.6.1: this used to save Blind, set it, make the call, then restore
     * the saved value; but the Eyes of the Overworld override blindness
     * and let characters wearing them get dknown set for distant items.
     *
     * TODO? if the hero is wearing those Eyes, figure out whether the
     * object is within X-ray radius and only treat it as distant when
     * beyond that radius.  Logic is iffy but result might be interesting.
     */
    ++distantname;
    str = (*func)(obj);
    --distantname;
    return str;
}

/* convert player specified fruit name into corresponding fruit juice name
   ("slice of pizza" -> "pizza juice" rather than "slice of pizza juice") */
char *
fruitname(juice)
boolean juice; /* whether or not to append " juice" to the name */
{
    char *buf = nextobuf();
    const char *fruit_nam = strstri(pl_fruit, " of ");

    if (fruit_nam)
        fruit_nam += 4; /* skip past " of " */
    else
        fruit_nam = pl_fruit; /* use it as is */

    Sprintf(buf, "%s%s", makesingular(fruit_nam), juice ? " juice" : "");
    return buf;
}

/* look up a named fruit by index (1..127) */
struct fruit *
fruit_from_indx(indx)
int indx;
{
    struct fruit *f;

    for (f = ffruit; f; f = f->nextf)
        if (f->fid == indx)
            break;
    return f;
}

/* look up a named fruit by name */
struct fruit *
fruit_from_name(fname, exact, highest_fid)
const char *fname;
boolean exact; /* False => prefix or exact match, True = exact match only */
int *highest_fid; /* optional output; only valid if 'fname' isn't found */
{
    struct fruit *f, *tentativef;
    char *altfname;
    size_t k;
    /*
     * note: named fruits are case-senstive...
     */

    if (highest_fid)
        *highest_fid = 0;
    /* first try for an exact match */
    for (f = ffruit; f; f = f->nextf)
        if (!strcmp(f->fname, fname))
            return f;
        else if (highest_fid && f->fid > *highest_fid)
            *highest_fid = f->fid;

    /* didn't match as-is; if caller is willing to accept a prefix
       match, try to find one; we want to find the longest prefix that
       matches, not the first */
    if (!exact) {
        tentativef = 0;
        for (f = ffruit; f; f = f->nextf) {
            k = strlen(f->fname);
            if (!strncmp(f->fname, fname, k)
                && (!fname[k] || fname[k] == ' ')
                && (!tentativef || k > strlen(tentativef->fname)))
                tentativef = f;
        }
        f = tentativef;
    }
    /* if we still don't have a match, try singularizing the target;
       for exact match, that's trivial, but for prefix, it's hard */
    if (!f) {
        altfname = makesingular(fname);
        for (f = ffruit; f; f = f->nextf) {
            if (!strcmp(f->fname, altfname))
                break;
        }
        releaseobuf(altfname);
    }
    if (!f && !exact) {
        char fnamebuf[BUFSZ], *p;
        size_t fname_k = strlen(fname); /* length of assumed plural fname */

        tentativef = 0;
        for (f = ffruit; f; f = f->nextf) {
            k = strlen(f->fname);
            /* reload fnamebuf[] each iteration in case it gets modified;
               there's no need to recalculate fname_k */
            Strcpy(fnamebuf, fname);
            /* bug? if singular of fname is longer than plural,
               failing the 'fname_k > k' test could skip a viable
               candidate; unfortunately, we can't singularize until
               after stripping off trailing stuff and we can't get
               accurate fname_k until fname has been singularized;
               compromise and use 'fname_k >= k' instead of '>',
               accepting 1 char length discrepancy without risking
               false match (I hope...) */
            if (fname_k >= k && (p = index(&fnamebuf[k], ' ')) != 0) {
                *p = '\0'; /* truncate at 1st space past length of f->fname */
                altfname = makesingular(fnamebuf);
                k = strlen(altfname); /* actually revised 'fname_k' */
                if (!strcmp(f->fname, altfname)
                    && (!tentativef || k > strlen(tentativef->fname)))
                    tentativef = f;
                releaseobuf(altfname); /* avoid churning through all obufs */
            }
        }
        f = tentativef;
    }
    return f;
}

/* sort the named-fruit linked list by fruit index number */
void
reorder_fruit(forward)
boolean forward;
{
    struct fruit *f, *allfr[1 + 127];
    int i, j, k = SIZE(allfr);

    for (i = 0; i < k; ++i)
        allfr[i] = (struct fruit *) 0;
    for (f = ffruit; f; f = f->nextf) {
        /* without sanity checking, this would reduce to 'allfr[f->fid]=f' */
        j = f->fid;
        if (j < 1 || j >= k) {
            impossible("reorder_fruit: fruit index (%d) out of range", j);
            return; /* don't sort after all; should never happen... */
        } else if (allfr[j]) {
            impossible("reorder_fruit: duplicate fruit index (%d)", j);
            return;
        }
        allfr[j] = f;
    }
    ffruit = 0; /* reset linked list; we're rebuilding it from scratch */
    /* slot [0] will always be empty; must start 'i' at 1 to avoid
       [k - i] being out of bounds during first iteration */
    for (i = 1; i < k; ++i) {
        /* for forward ordering, go through indices from high to low;
           for backward ordering, go from low to high */
        j = forward ? (k - i) : i;
        if (allfr[j]) {
            allfr[j]->nextf = ffruit;
            ffruit = allfr[j];
        }
    }
}

char *
xname(obj)
struct obj *obj;
{
    return xname_flags(obj, CXN_NORMAL);
}

char *
xname_flags(obj, cxn_flags)
register struct obj *obj;
unsigned cxn_flags; /* bitmask of CXN_xxx values */
{
    register char* buf;

    if (!obj)
    {
        buf = nextobuf();
        strcpy(buf, empty_string);
        return buf;
    }

    boolean artifact_description_exists = obj->oartifact && artilist[obj->oartifact].desc && strcmp(artilist[obj->oartifact].desc, "");
    /* Note: maskotyp is relevant here when artifact description does not exist */
    register int typ = ((obj->oartifact && artilist[obj->oartifact].maskotyp != STRANGE_OBJECT) ? artilist[obj->oartifact].maskotyp : obj->otyp);
    register struct objclass *ocl = &objects[typ];
    /* Note: when its description exists, an artifact always uses the unknown name format instead of oc_name_known;
     * if the artifact is known, then it jumps to "the Artifact" format */
    /* Note: artifact uses nknown and aknown to determine if it is in the format of "the Artifact"; this is in obj_is_pname function */
    int nn = artifact_description_exists ? 0 : ocl->oc_name_known;
    int omndx = obj->corpsenm;
    /* Note: this is applied for artifacts only when their own description does not exist */
    const char *actualn = OBJ_NAME(*ocl);
    /* Note: use artifact description for artifacts instead */
    const char *dn = artifact_description_exists ? artilist[obj->oartifact].desc : OBJ_DESCR(*ocl);
    const char *un = ocl->oc_uname;
    boolean pluralize = (obj->quan != 1L) && !(cxn_flags & CXN_SINGULAR);
    boolean known, dknown, bknown, nknown, aknown, mknown, tknown;
    boolean makeThelower = FALSE;

    buf = nextobuf() + PREFIX; /* leave room for "17 -3 " */

    if (Role_if(PM_SAMURAI) && Japanese_item_name(typ))
        actualn = Japanese_item_name(typ);
    /* 3.6.2: this used to be part of 'dn's initialization, but it
       needs to come after possibly overriding 'actualn' */
    if (!dn)
        dn = actualn;

    buf[0] = '\0';
    /*
     * clean up known when it's tied to oc_name_known, eg after AD_DRIN
     * This is only required for unique objects since the article
     * printed for the object is tied to the combination of the two
     * and printing the wrong article gives away information.
     */
    if (!nn && ocl->oc_uses_known && is_otyp_unique(typ))
        obj->known = 0;
    if (!Blind && !distantname)
        obj->dknown = TRUE;
    if (Role_if(PM_PRIEST))
        obj->bknown = TRUE;

    if (iflags.override_ID) {
        known = dknown = bknown = nknown = aknown = mknown = tknown = TRUE;
        nn = 1;
    } else {
        known = obj->known;
        dknown = obj->dknown;
        bknown = obj->bknown;
        nknown = obj->nknown;
        aknown = obj->aknown;
        mknown = obj->mknown;
        tknown = obj->tknown;
    }

    /* Artifacts get just their name */
    if (obj_is_pname(obj))
        goto nameit;

    /* General prefixes */
    if (is_poisonable(obj) && obj->opoisoned && dknown)
        Strcpy(buf, "poisoned ");

    if (dknown)
    {
        if (obj->elemental_enchantment == COLD_ENCHANTMENT)
            Strcat(buf, "freezing ");
        else if (obj->elemental_enchantment == FIRE_ENCHANTMENT)
            Strcat(buf, "flaming ");
        else if (obj->elemental_enchantment == LIGHTNING_ENCHANTMENT)
            Strcat(buf, "electrified ");
        else if (obj->elemental_enchantment == DEATH_ENCHANTMENT)
            Strcat(buf, "death-magical ");
    }

    if (obj->oclass == ARMOR_CLASS && (is_boots(obj) || is_gloves(obj) || is_bracers(obj)))
        Strcat(buf, "pair of ");
    else if (obj->oclass == MISCELLANEOUS_CLASS && (
        objects[obj->otyp].oc_subtyp == MISC_EARRINGS
        || objects[obj->otyp].oc_subtyp == MISC_EYEGLASSES
        || objects[obj->otyp].oc_subtyp == MISC_PANTS
        || objects[obj->otyp].oc_subtyp == MISC_BRACERS
        || objects[obj->otyp].oc_subtyp == MISC_WINGS
        || objects[obj->otyp].oc_subtyp == MISC_EXTRA_ARMS
        ))
        Strcat(buf, "pair of ");
    else if (is_wet_towel(obj))
        Strcat(buf, (obj->special_quality < 3) ? "moist " : "wet ");
    else if (is_key(obj) && obj->special_quality > 0)
    {
        const char* desc = get_key_special_quality_description(obj);
        if (desc && strcmp(desc, ""))
        {
            Strcat(buf, desc);
            Strcat(buf, " ");
        }
    }

    if (dknown && (obj->mythic_prefix || obj->mythic_suffix))
    {
        if (!mknown)
            Strcat(buf, (obj->mythic_prefix && obj->mythic_suffix) ? "legendary " : "mythic ");
        else if (obj->mythic_prefix)
            Strcat(buf, mythic_prefix_qualities[obj->mythic_prefix].mythic_affix);
    }

    if (dknown)
    {
        if (obj->exceptionality == EXCEPTIONALITY_EXCEPTIONAL)
            Strcat(buf, "exceptional ");
        else if (obj->exceptionality == EXCEPTIONALITY_ELITE)
            Strcat(buf, "elite ");
        else if (obj->exceptionality == EXCEPTIONALITY_CELESTIAL)
            Strcat(buf, "celestial ");
        else if (obj->exceptionality == EXCEPTIONALITY_PRIMORDIAL)
            Strcat(buf, "primordial ");
        else if (obj->exceptionality == EXCEPTIONALITY_INFERNAL)
            Strcat(buf, "infernal ");

        if (obj->material != objects[obj->otyp].oc_material)
        {
            Strcat(buf, material_definitions[obj->material].object_prefix);
            Strcat(buf, " ");
        }
    }

    switch (obj->oclass) {
    case AMULET_CLASS:
        if (!dknown)
            Strcat(buf, "amulet");
        else if (typ == AMULET_OF_YENDOR || typ == FAKE_AMULET_OF_YENDOR)
            /* each must be identified individually */
            Strcat(buf, known ? actualn : dn);
        else if (nn)
            Strcat(buf, actualn);
        else if (un)
            Sprintf(eos(buf), "amulet called %s", un);
        else
            Sprintf(eos(buf), "%s amulet", dn);
        break;
    case WEAPON_CLASS:
    case VENOM_CLASS:
    case REAGENT_CLASS:
    case MISCELLANEOUS_CLASS:
    case TOOL_CLASS:
    case GEM_CLASS:
    {
        const char* rock = is_ore(obj) ? "nugget of ore" : is_graystone(obj) ? "stone" : (ocl->oc_material == MAT_MINERAL) ? "stone" : "gem";
        boolean isgem = (obj->oclass == GEM_CLASS);

        if (!dknown)
        {
            if (obj->oclass == WEAPON_CLASS)
                Strcat(buf, weapon_type_names[objects[obj->otyp].oc_subtyp]);
            else if (obj->oclass == MISCELLANEOUS_CLASS)
                Strcat(buf, misc_type_names[objects[obj->otyp].oc_subtyp]);
            else if (obj->oclass == TOOL_CLASS && objects[obj->otyp].oc_subtyp > TOOLTYPE_GENERAL)
                Strcat(buf, tool_type_names[objects[obj->otyp].oc_subtyp]);
            else if (obj->oclass == GEM_CLASS)
                Strcpy(buf, rock);
            else
                Strcat(buf, dn);
        }
        else if (nn)
        {
            Strcat(buf, actualn);
            if (isgem && GemStone(typ))
                Strcat(buf, " stone");
        }
        else if (un)
        {
            Strcat(buf, isgem ? rock : dn);
            Strcat(buf, " called ");
            Strcat(buf, un);
        }
        else
        {
            if (isgem)
                Sprintf(buf, "%s %s", dn, rock);
            else
                Strcat(buf, dn);
        }

        if (typ == FIGURINE && omndx != NON_PM) {
            char anbuf[10]; /* [4] would be enough: 'a','n',' ','\0' */

            Sprintf(eos(buf), " of %s%s",
                just_an(anbuf, mons[omndx].mname),
                mons[omndx].mname);
        }
        else if (is_wet_towel(obj)) {
            if (wizard)
                Sprintf(eos(buf), " (%d)", obj->special_quality);
        }
        break;
    }
    case ARMOR_CLASS:
        /* depends on order of the dragon scales objects -- Special case, ignores the modifiers above */
        if (typ >= GRAY_DRAGON_SCALES && typ <= YELLOW_DRAGON_SCALES) {
            Sprintf(buf, "set of %s", actualn);
            break;
        }

        if (obj->otyp >= ELVEN_SHIELD && obj->otyp <= ORCISH_SHIELD
            && !dknown) {
            Strcpy(buf, "shield");
            break;
        }
        if (obj->otyp == SHIELD_OF_REFLECTION && !dknown) {
            Strcpy(buf, "smooth shield");
            break;
        }

        if (!dknown)
            Strcat(buf, armor_class_simple_name(obj));
        else if (nn) {
            Strcat(buf, actualn);
        } else if (un) {
            Strcat(buf, dn);
            Strcat(buf, " called ");
            Strcat(buf, un);
        } else
            Strcat(buf, dn);
        break;
    case FOOD_CLASS:
        if (typ == SLIME_MOLD) {
            struct fruit *f = fruit_from_indx(obj->special_quality);

            if (!f) {
                impossible("Bad fruit #%d?", obj->special_quality);
                Strcpy(buf, "fruit");
            } else {
                Strcpy(buf, f->fname);
                if (pluralize) {
                    /* ick; already pluralized fruit names
                       are allowed--we want to try to avoid
                       adding a redundant plural suffix */
                    Strcpy(buf, makeplural(makesingular(buf)));
                    pluralize = FALSE;
                }
            }
            break;
        }
        if (obj->globby) {
            Sprintf(buf, "%s%s",
                    (obj->owt <= GLOB_SMALL_MAXIMUM_WEIGHT)
                       ? "small "
                       : (obj->owt > GLOB_LARGE_MAXIMUM_WEIGHT)
                          ? "very large "
                          : (obj->owt > GLOB_MEDIUM_MAXIMUM_WEIGHT)
                             ? "large "
                             : "",
                    actualn);
            break;
        }

        if (!dknown)
            Strcat(buf, food_type_names[objects[obj->otyp].oc_subtyp]);
        else if (nn)
            Strcat(buf, actualn);
        else if (un) {
            Strcat(buf, dn);
            Strcat(buf, " called ");
            Strcat(buf, un);
        }
        else
            Strcat(buf, dn);


        //Strcpy(buf, actualn);
        if (typ == TIN && known)
            tin_details(obj, omndx, buf);
        break;
    case COIN_CLASS:
    case CHAIN_CLASS:
        Strcpy(buf, actualn);
        break;
    case ROCK_CLASS:
        if (typ == STATUE && omndx != NON_PM) 
        {
            char anbuf[10];
            char monbuf[BUFSIZ] = "";
            struct monst* mtmp = get_mtraits(obj, FALSE);
            if (mtmp)
            {
                Strcpy(monbuf, x_monnam(mtmp, ARTICLE_NONE, (char*)0,
                    (has_mname(mtmp)) ? (SUPPRESS_SADDLE | SUPPRESS_IT | SUPPRESS_INVISIBLE)
                    : SUPPRESS_IT | SUPPRESS_INVISIBLE,
                    FALSE));
            }
            else
            {
                Strcpy(monbuf, mons[omndx].mname);
            }

            Sprintf(buf, "%s%s of %s%s",
                    (Role_if(PM_ARCHAEOLOGIST) && (obj->speflags & SPEFLAGS_STATUE_HISTORIC))
                       ? "historic "
                       : "",
                    actualn,
                    is_mname_proper_name(&mons[omndx])
                       ? ""
                       : the_unique_pm(&mons[omndx])
                          ? "the "
                          : just_an(anbuf, monbuf),
                    monbuf);
        }
        else
            Strcpy(buf, actualn);

        break;
    case BALL_CLASS:
        Sprintf(buf, "%sheavy iron ball",
                (obj->owt > ocl->oc_weight) ? "very " : "");
        break;
    case POTION_CLASS:
        if (dknown && obj->odiluted)
            Strcpy(buf, "diluted ");
        if (nn || un || !dknown) {
            Strcat(buf, "potion");
            if (!dknown)
                break;
            if (nn) {
                Strcat(buf, " of ");
                if (typ == POT_WATER && bknown
                    && (obj->blessed || obj->cursed)) {
                    Strcat(buf, obj->blessed ? "holy " : "unholy ");
                }
                Strcat(buf, actualn);
            } else {
                Strcat(buf, " called ");
                Strcat(buf, un);
            }
        } else {
            Strcat(buf, dn);
            Strcat(buf, " potion");
        }
        break;
    case SCROLL_CLASS:
        Strcpy(buf, "scroll");
        if (!dknown)
            break;
        if (nn) {
            Strcat(buf, " of ");
            Strcat(buf, actualn);
        } else if (un) {
            Strcat(buf, " called ");
            Strcat(buf, un);
        } else if (ocl->oc_magic) {
            Strcat(buf, " labeled ");
            Strcat(buf, dn);
        } else {
            Strcpy(buf, dn);
            Strcat(buf, " scroll");
        }
        break;
    case WAND_CLASS:
        if (!dknown)
            Strcpy(buf, "wand");
        else if (nn)
            Sprintf(buf, "wand of %s", actualn);
        else if (un)
            Sprintf(buf, "wand called %s", un);
        else
            Sprintf(buf, "%s wand", dn);
        break;
    case SPBOOK_CLASS:
        if (objects[typ].oc_subtyp == BOOKTYPE_NOVEL || objects[typ].oc_subtyp == BOOKTYPE_MANUAL) { /* 3.6 tribute */
            if (!dknown)
                Strcpy(buf, "book");
            else if (nn)
                Strcpy(buf, actualn);
            else if (un)
                Sprintf(buf, "%s called %s", book_type_names[objects[typ].oc_subtyp], un);
            else
                Sprintf(buf, "%s book", dn);
            break;
            /* end of tribute */
        } else if (!dknown) {
            Strcpy(buf, book_type_names[objects[typ].oc_subtyp]);
        } else if (nn) {
            if (!(objects[typ].oc_flags5 & O5_FULL_NAME)) // typ != SPE_BOOK_OF_THE_DEAD && typ != SPE_BOOK_OF_MODRON)
                Sprintf(buf, "%s of ", book_type_names[objects[typ].oc_subtyp]);
            Strcat(buf, actualn);
        } else if (un) {
            Sprintf(buf, "%s called %s", book_type_names[objects[typ].oc_subtyp], un);
        } else
            Sprintf(buf, "%s %s", dn, book_type_names[objects[typ].oc_subtyp]);
        break;
    case RING_CLASS:
        if (!dknown)
            Strcpy(buf, "ring");
        else if (nn)
            Sprintf(buf, "ring of %s", actualn);
        else if (un)
            Sprintf(buf, "ring called %s", un);
        else
            Sprintf(buf, "%s ring", dn);
        break;
    default:
        Sprintf(buf, "glorkum %d %d %d %d", obj->oclass, typ, obj->enchantment, obj->charges);
    }
    if (pluralize)
        Strcpy(buf, makeplural(buf));

    if (dknown && mknown && obj->mythic_suffix)
    {
        Strcat(buf, mythic_suffix_qualities[obj->mythic_suffix].mythic_affix);
    }

    if (obj->otyp == T_SHIRT && program_state.gameover) {
        char tmpbuf[BUFSZ];

        Sprintf(eos(buf), " with text \"%s\"", tshirt_text(obj, tmpbuf));
    }

    char anamebuf[BUFSZ] = "";
    if (has_oname(obj) && nknown && dknown) {
        if(obj->oclass == SPBOOK_CLASS)
            Strcat(buf, " entitled ");
        else
            Strcat(buf, " named ");
        makeThelower = TRUE;
    nameit:
        strcpy(anamebuf, ONAME(obj));
        if (makeThelower && !strncmpi(anamebuf, "The ", 4))
            *anamebuf = lowc(*anamebuf);
        Strcat(buf, anamebuf);
    }
    else if (has_uoname(obj))
    {
        if(obj->otyp == CORPSE)
            Strcat(buf, " known as "); /* corpses are not labeled */
        else
            Strcat(buf, " labeled ");
        Strcat(buf, UONAME(obj));
    }
    if (!strncmpi(buf, "the ", 4))
        buf += 4;

    /* Corpse names from OMONST */
    if (obj->oextra && OMONST(obj)) 
    {
        if (OMONST(obj)->isshk && OMONST(obj)->mextra && ESHK(OMONST(obj)) && OMONST(obj)->u_know_mname)
        {
            Strcat(buf, " who was named ");
            Strcat(buf, shkname(OMONST(obj)));
        }
        else if(has_mname(OMONST(obj)) && OMONST(obj)->u_know_mname)
        {
            Strcat(buf, " that was named ");
            Strcat(buf, MNAME(OMONST(obj)));
        }
        else if (has_umname(OMONST(obj)))
        {
            Strcat(buf, " that you called ");
            Strcat(buf, UMNAME(OMONST(obj)));
        }
    }

    return buf;
}

/* similar to simple_typename but minimal_xname operates on a particular
   object rather than its general type; it formats the most basic info:
     potion                     -- if description not known
     brown potion               -- if oc_name_known not set
     potion of object detection -- if discovered
 */
STATIC_OVL char *
minimal_xname(obj)
struct obj *obj;
{
    char *bufp;
    struct obj bareobj;
    struct objclass saveobcls;
    int otyp = obj->otyp;

    /* suppress user-supplied name */
    saveobcls.oc_uname = objects[otyp].oc_uname;
    objects[otyp].oc_uname = 0;
    /* suppress actual name if object's description is unknown */
    saveobcls.oc_name_known = objects[otyp].oc_name_known;
    if (!obj->dknown)
        objects[otyp].oc_name_known = 0;

    /* caveat: this makes a lot of assumptions about which fields
       are required in order for xname() to yield a sensible result */
    bareobj = zeroobj;
    bareobj.otyp = otyp;
    bareobj.oartifact = obj->oartifact;
    bareobj.oclass = obj->oclass;
    bareobj.dknown = obj->dknown;
    /* suppress known except for amulets (needed for fakes and real A-of-Y) */
    bareobj.known = (obj->oclass == AMULET_CLASS)
                        ? obj->known
                        /* default is "on" for types which don't use it */
                        : !objects[otyp].oc_uses_known;
    bareobj.quan = 1L;         /* don't want plural */
    bareobj.corpsenm = NON_PM; /* suppress statue and figurine details */
    /* but suppressing fruit details leads to "bad fruit #0"
       [perhaps we should force "slime mold" rather than use xname?] */
    if (obj->otyp == SLIME_MOLD)
        bareobj.special_quality = obj->special_quality;

    bufp = distant_name(&bareobj, xname); /* xname(&bareobj) */
    if (!strncmp(bufp, "uncursed ", 9))
        bufp += 9; /* Role_if(PM_PRIEST) */

    objects[otyp].oc_uname = saveobcls.oc_uname;
    objects[otyp].oc_name_known = saveobcls.oc_name_known;
    return bufp;
}

/* xname() output augmented for multishot missile feedback */
char *
mshot_xname(obj)
struct obj *obj;
{
    char tmpbuf[BUFSZ];
    char *onm = xname(obj);

    if (m_shot.n > 1 && m_shot.o == obj->otyp) {
        /* "the Nth arrow"; value will eventually be passed to an() or
           The(), both of which correctly handle this "the " prefix */
        Sprintf(tmpbuf, "the %d%s ", m_shot.i, ordin(m_shot.i));
        onm = strprepend(onm, tmpbuf);
    }
    return onm;
}

/* used for naming "the unique_item" instead of "a unique_item" */
boolean
the_unique_obj(obj)
struct obj *obj;
{
    boolean known = (obj->known || iflags.override_ID);

    if (!obj->dknown && !iflags.override_ID)
        return FALSE;
    else if (obj->otyp == FAKE_AMULET_OF_YENDOR && !known)
        return TRUE; /* lie */
    else
        return (boolean) (is_obj_unique(obj)
                          && (known || obj->otyp == AMULET_OF_YENDOR));
}

/* should monster type be prefixed with "the"? (mostly used for corpses) */
boolean
the_unique_pm(ptr)
struct permonst *ptr;
{
    boolean uniq;

    /* even though monsters with personal names are unique, we want to
       describe them as "Name" rather than "the Name" */
    if (is_mname_proper_name(ptr))
        return FALSE;

    uniq = (ptr->geno & G_UNIQ) ? TRUE : FALSE;
    /* high priest is unique if it includes "of <deity>", otherwise not
       (caller needs to handle the 1st possibility; we assume the 2nd);
       worm tail should be irrelevant but is included for completeness */
    if (ptr == &mons[PM_HIGH_PRIEST] || ptr == &mons[PM_LONG_WORM_TAIL] || ptr == &mons[PM_ELDER_LONG_WORM_TAIL])
        uniq = FALSE;
    /* Wizard no longer needs this; he's flagged as unique these days */
    if (ptr == &mons[PM_WIZARD_OF_YENDOR])
        uniq = TRUE;
    return uniq;
}

void
add_erosion_words(obj, prefix)
struct obj *obj;
char *prefix;
{
    boolean iscrys = (obj->otyp == CRYSKNIFE);
    boolean rknown;

    rknown = (iflags.override_ID == 0) ? obj->rknown : TRUE;
        
    /* The only cases where any of these bits do double duty are for
     * rotted food and diluted potions, which are all not is_damageable().
     */
    if (is_damageable(obj) && !iscrys)
    {
        if (obj->oeroded) 
        {
            switch (obj->oeroded)
            {
            case 2:
                Strcat(prefix, "very ");
                break;
            case 3:
                Strcat(prefix, "thoroughly ");
                break;
            }
            Strcat(prefix, is_rustprone(obj) ? "rusty " : "burnt ");
        }

        if (obj->oeroded2)
        {
            switch (obj->oeroded2)
            {
            case 2:
                Strcat(prefix, "very ");
                break;
            case 3:
                Strcat(prefix, "thoroughly ");
                break;
            }
            Strcat(prefix, is_corrodeable(obj) ? "corroded " : "rotted ");
        }
    }

    /* Erodeproof status; now shown also for nondamageable objects, if they happen to be erodeproof */
    if (rknown && obj->oerodeproof)
        Strcat(prefix, iscrys
                          ? "fixed "
                          : is_rustprone(obj)
                             ? "rustproof "
                             : is_corrodeable(obj)
                                ? "corrodeproof " /* "stainless"? */
                                : is_flammable(obj)
                                   ? "fireproof "
                                : is_rottable(obj)
                                   ? "rotproof "
                                   : "erodeproof ");
}

/* used to prevent rust on items where rust makes no difference */
boolean
erosion_matters(obj)
struct obj *obj;
{
    switch (obj->oclass) {
    case TOOL_CLASS:
        /* it's possible for a rusty weptool to be polymorphed into some
           non-weptool iron tool, in which case the rust implicitly goes
           away, but it's also possible for it to be polymorphed into a
           non-iron tool, in which case rust also implicitly goes away,
           so there's no particular reason to try to handle the first
           instance differently [this comment belongs in poly_obj()...] */
        return is_weptool(obj) ? TRUE : FALSE;
    case WEAPON_CLASS:
    case ARMOR_CLASS:
    case BALL_CLASS:
    case CHAIN_CLASS:
        return TRUE;
    default:
        break;
    }
    return FALSE;
}

#define DONAME_WITH_PRICE 0x0001
#define DONAME_VAGUE_QUAN 0x0002
#define DONAME_WITH_WEIGHT_FIRST 0x0004
#define DONAME_WITH_WEIGHT_LAST 0x0008
#define DONAME_LOADSTONE_CORRECTLY 0x0010
#define DONAME_LIT_IN_FRONT  0x0020

STATIC_OVL char *
doname_base(obj, doname_flags)
struct obj *obj;
unsigned doname_flags;
{
    if (!obj)
        return xname(obj); /* Returns an empty string */

    boolean ispoisoned = FALSE,
            with_price = (doname_flags & DONAME_WITH_PRICE) != 0,
            vague_quan = (doname_flags & DONAME_VAGUE_QUAN) != 0,
            weightfirst = (doname_flags & DONAME_WITH_WEIGHT_FIRST) != 0,
            weightlast = (doname_flags & DONAME_WITH_WEIGHT_LAST) != 0,
            loadstonecorrectly = (doname_flags & DONAME_LOADSTONE_CORRECTLY) != 0,
            lit_in_front = (doname_flags & DONAME_LIT_IN_FRONT) != 0;
    boolean known, dknown, cknown, bknown, lknown, tknown;
    int omndx = obj->corpsenm, isenchanted = 0;
    char prefix[PREFIX];
    char tmpbuf[PREFIX + 1]; /* for when we have to add something at
                                the start of prefix instead of the
                                end (Strcat is used on the end) */
    register char *bp = xname(obj);

    if (iflags.override_ID) {
        known = dknown = cknown = bknown = lknown = tknown = TRUE;
    } else {
        known = obj->known;
        dknown = obj->dknown;
        cknown = obj->cknown;
        bknown = obj->bknown;
        lknown = obj->lknown;
        tknown = obj->tknown;
    }

    /* When using xname, we want "poisoned arrow", and when using
     * doname, we want "poisoned +0 arrow".  This kludge is about the only
     * way to do it, at least until someone overhauls xname() and doname(),
     * combining both into one function taking a parameter.
     */
    /* must check opoisoned--someone can have a weirdly-named fruit */
    if (!strncmp(bp, "poisoned ", 9) && obj->opoisoned) {
        bp += 9;
        ispoisoned = TRUE;
    }

    if (!strncmp(bp, "freezing ", 9) && obj->elemental_enchantment == COLD_ENCHANTMENT) {
        bp += 9;
        isenchanted = obj->elemental_enchantment;
    } else if (!strncmp(bp, "flaming ", 8) && obj->elemental_enchantment == FIRE_ENCHANTMENT) {
        bp += 8;
        isenchanted = obj->elemental_enchantment;
    }
    else if (!strncmp(bp, "electrified ", 12) && obj->elemental_enchantment == LIGHTNING_ENCHANTMENT) {
        bp += 12;
        isenchanted = obj->elemental_enchantment;
    }
    else if (!strncmp(bp, "death-magical ", 14) && obj->elemental_enchantment == DEATH_ENCHANTMENT) {
        bp += 14;
        isenchanted = obj->elemental_enchantment;
    }

    if (obj->quan != 1L) {
        if (dknown || !vague_quan)
            Sprintf(prefix, "%ld ", obj->quan);
        else
            Strcpy(prefix, "some ");
    } else if (obj->otyp == CORPSE) {
        /* skip article prefix for corpses [else corpse_xname()
           would have to be taught how to strip it off again] */
        *prefix = '\0';
    } else if (obj_is_pname(obj) || the_unique_obj(obj)) {
        if (!strncmpi(bp, "the ", 4))
            bp += 4;
        Strcpy(prefix, "the ");
    } else {
        Strcpy(prefix, "a ");
    }

    /* "empty" goes at the beginning, but item count goes at the end */
    if ((obj->oclass == TOOL_CLASS && (objects[obj->otyp].oc_subtyp == TOOLTYPE_JAR || objects[obj->otyp].oc_subtyp == TOOLTYPE_CAN || objects[obj->otyp].oc_subtyp == TOOLTYPE_GRAIL) && obj->charges == 0 && !known)
        || (cknown
        /* bag of tricks: include "empty" prefix if it's known to
           be empty but its precise number of charges isn't known
           (when that is known, suffix of "(n:0)" will be appended,
           making the prefix be redundant; note that 'known' flag
           isn't set when emptiness gets discovered because then
           charging magic would yield known number of new charges) */
        && ((obj->otyp == BAG_OF_TRICKS)
             ? (obj->charges == 0 && !known)
             /* not bag of tricks: empty if container which has no contents */
             : ((Is_container(obj) || obj->otyp == STATUE)
                && !Has_contents(obj)))))
        Strcat(prefix, "empty ");

    if (bknown && obj->oclass != COIN_CLASS
        && (obj->otyp != POT_WATER || !objects[POT_WATER].oc_name_known
            || (!obj->cursed && !obj->blessed))) {
        /* allow 'blessed clear potion' if we don't know it's holy water;
         * always allow "uncursed potion of water"
         */
        if (obj->cursed)
            Strcat(prefix, "cursed ");
        else if (obj->blessed)
            Strcat(prefix, "blessed ");
        else if (!iflags.implicit_uncursed
            /* For most items with charges or +/-, if you know how many
             * charges are left or what the +/- is, then you must have
             * totally identified the item, so "uncursed" is unnecessary,
             * because an identified object not described as "blessed" or
             * "cursed" must be uncursed.
             *
             * If the charges or +/- is not known, "uncursed" must be
             * printed to avoid ambiguity between an item whose curse
             * status is unknown, and an item known to be uncursed.
             */
                 || ((!known || !objects[obj->otyp].oc_enchantable
                      || obj->oclass == ARMOR_CLASS
                      || obj->oclass == RING_CLASS
                      || obj->oclass == MISCELLANEOUS_CLASS
                     )
                     && obj->otyp != SCR_MAIL
                     && obj->otyp != FAKE_AMULET_OF_YENDOR
                     && obj->otyp != AMULET_OF_YENDOR
                     //&& !Role_if(PM_PRIEST)
                     ))
            Strcat(prefix, "uncursed ");
    }

    if (tknown && obj->otrapped && Is_box(obj))
        Strcat(prefix, "trapped ");

    if (lknown && Is_box(obj)) 
    {
        if (obj->obroken)
            /* 3.6.0 used "unlockable" here but that could be misunderstood
               to mean "capable of being unlocked" rather than the intended
               "not capable of being locked" */
            Strcat(prefix, "broken ");
        else if (obj->olocked)
        {
            Strcat(prefix, "locked ");
        }
        else
            Strcat(prefix, "unlocked ");
    }
    else if (Is_container_with_lid(obj))
    {
        if(obj->speflags & SPEFLAGS_LID_OPENED)
            Strcat(prefix, "open ");
    }

    if (Is_box(obj))
    {
        print_lock_with_buf(eos(bp), obj->keyotyp, obj->special_quality, FALSE);
    }

    if (obj->greased)
        Strcat(prefix, "greased ");

    /* contents for unknown jars and cans */
    if (obj->oclass == TOOL_CLASS 
        && (objects[obj->otyp].oc_subtyp == TOOLTYPE_JAR || objects[obj->otyp].oc_subtyp == TOOLTYPE_CAN || objects[obj->otyp].oc_subtyp == TOOLTYPE_GRAIL)
        && OBJ_CONTENT_DESC(obj->otyp)
        && objects[obj->otyp].oc_name_known == 0 && !known && obj->charges > 0)
    {
        int lim = get_obj_max_charge(obj);
        const char* fullness = obj->charges > (int)(0.5 * (double)lim) ? "" :
            obj->charges > 1 ? "some " :
            obj->charges > 0 ? "little " : "no ";
        Sprintf(eos(bp), " containing %s%s", fullness, OBJ_CONTENT_DESC(obj->otyp));
    }


    if (cknown && Has_contents(obj)) {
        /* we count the number of separate stacks, which corresponds
           to the number of inventory slots needed to be able to take
           everything out if no merges occur */
        long itemcount = count_contents(obj, FALSE, FALSE, TRUE);

        Sprintf(eos(bp), " containing %ld item%s", itemcount,
                plur(itemcount));
    }

    /* charges */
    if (objects[obj->otyp].oc_charged && known)
    {
        if (flags.long_charge_text)
        {
            Sprintf(eos(bp), " (%d charge%s, %d recharging%s)", (int)obj->charges, plur(obj->charges), (int)obj->recharged, plur(obj->recharged));
        }
        else
            Sprintf(eos(bp), " (%d:%d)", (int)obj->recharged, (int)obj->charges);
    }

    /* post and prefixes */
    switch (is_weptool(obj) ? WEAPON_CLASS : obj->oclass) {
    case AMULET_CLASS:
        if (obj->owornmask & W_AMUL)
            Strcat(bp, " (being worn)");
        break;
    case MISCELLANEOUS_CLASS:
        if (obj->owornmask & W_MISCITEMS)
        {
            if(strcmp(misc_type_worn_texts[objects[obj->otyp].oc_subtyp], "") == 0)
                Strcat(bp, " (being worn)");
            else
            {
                char replacetxt[BUFSZ] = "";
                strcpy(replacetxt, misc_type_worn_texts[objects[obj->otyp].oc_subtyp]);

                /* special replacement for some types */
                if(objects[obj->otyp].oc_subtyp == MISC_IOUN_STONE)
                    Sprintf(replacetxt, "orbiting %s", body_part(HEAD));
                if (objects[obj->otyp].oc_subtyp == MISC_NOSERING)
                    Sprintf(replacetxt, "on %s", body_part(NOSE));

                Strcat(bp, " (");
                Strcat(bp, replacetxt);
                Strcat(bp, ")");
            }
        }
        if (known && objects[obj->otyp].oc_enchantable) 
        {
            Strcat(prefix, sitoa(obj->enchantment));
            Strcat(prefix, " ");
        }
        break;
    case ARMOR_CLASS:
        if (obj->owornmask & W_ARMS)
        {
            if(is_shield(obj))
            {
                if (u.twoweap)
                {
                    if ((objects[obj->otyp].oc_flags & O1_IS_WEAPON_WHEN_WIELDED))
                        Sprintf(eos(bp), " (being worn; weapon in left %s)", body_part(HAND));
                    else
                        Sprintf(eos(bp), " (being worn; wielded in left %s)", body_part(HAND));
                }
                else
                {
                    Strcat(bp, " (being worn)");
                }
            }
            else
            {
                if (u.twoweap)
                {
                    if ((objects[obj->otyp].oc_flags & O1_IS_WEAPON_WHEN_WIELDED))
                        Sprintf(eos(bp), " (weapon in left %s)", body_part(HAND));
                    else
                        Sprintf(eos(bp), " (wielded in left %s)", body_part(HAND));
                }
                else
                {
                    if ((objects[obj->otyp].oc_flags & O1_IS_WEAPON_WHEN_WIELDED))
                        Sprintf(eos(bp), " (unused weapon in another %s)", body_part(HAND));
                    else
                        Sprintf(eos(bp), " (wielded in another %s)", body_part(HAND));
                }
            }
        }
        else if (obj->owornmask & W_ARMOR)
        {
            Strcat(bp, (obj == uskin) ? " (embedded in your skin"
                       /* in case of perm_invent update while Wear/Takeoff
                          is in progress; check doffing() before donning()
                          because donning() returns True for both cases */
                       : doffing(obj) ? " (being doffed"
                         : donning(obj) ? " (being donned"
                           : " (being worn");
            if (uarm && uarmo && (obj == uarm || obj == uarmo))
            {
                boolean ac_tied = (context.suit_yielding_ac_bonus && context.robe_yielding_ac_bonus);
                boolean mc_tied = (context.suit_yielding_mc_bonus && context.robe_yielding_mc_bonus);

                if(ac_tied && mc_tied)
                    Strcat(bp, "; tied");
                else if ((obj == uarm && (!context.suit_yielding_ac_bonus || ac_tied) && (!context.suit_yielding_mc_bonus || mc_tied))
                    || (obj == uarmo && (!context.robe_yielding_ac_bonus || ac_tied) && (!context.robe_yielding_mc_bonus || mc_tied)))
                    Strcat(bp, "; inferior");
                else if ((obj == uarm && (!context.robe_yielding_ac_bonus || ac_tied) && (!context.robe_yielding_mc_bonus || mc_tied))
                    || (obj == uarmo && (!context.suit_yielding_ac_bonus || ac_tied) && (!context.suit_yielding_mc_bonus || mc_tied)))
                    Strcat(bp, "");
                else if((obj == uarm && context.suit_yielding_ac_bonus && !ac_tied)
                    || (obj == uarmo && context.robe_yielding_ac_bonus && !ac_tied))
                    Strcat(bp, "; better AC");
                else if ((obj == uarm && context.suit_yielding_mc_bonus && !mc_tied)
                    || (obj == uarmo && context.robe_yielding_mc_bonus && !mc_tied))
                    Strcat(bp, "; better MC");
            }
            Strcat(bp, ")");
        }
        /*FALLTHRU*/
    case WEAPON_CLASS:
weapon_here:
        if (ispoisoned)
            Strcat(prefix, "poisoned ");
        if (isenchanted)
        {
            switch (isenchanted)
            {
            case COLD_ENCHANTMENT:
                Strcat(prefix, "freezing ");
                break;
            case FIRE_ENCHANTMENT:
                Strcat(prefix, "flaming ");
                break;
            case LIGHTNING_ENCHANTMENT:
                Strcat(prefix, "electrified ");
                break;
            case DEATH_ENCHANTMENT:
                Strcat(prefix, "death-magical ");
                break;
            default:
                break;
            }
        }
        add_erosion_words(obj, prefix);
        if (known) {
            Strcat(prefix, sitoa(obj->enchantment));
            Strcat(prefix, " ");
        }
        break;
    case TOOL_CLASS:
        if (isenchanted)
        {
            switch (isenchanted)
            {
            case COLD_ENCHANTMENT:
                Strcat(prefix, "freezing ");
                break;
            case FIRE_ENCHANTMENT:
                Strcat(prefix, "flaming ");
                break;
            case LIGHTNING_ENCHANTMENT:
                Strcat(prefix, "electrified ");
                break;
            case DEATH_ENCHANTMENT:
                Strcat(prefix, "death-magical ");
                break;
            default:
                break;
            }
        }
        if (obj->owornmask & (W_BLINDFOLD | W_SADDLE)) { /* blindfold */
            Strcat(bp, " (being worn)");
            break;
        }
        if (obj->speflags & SPEFLAGS_AUTOSTASH) {
            Strcat(bp, " (auto-stash)");
            break;
        }
        if (obj->otyp == LEASH && obj->leashmon != 0) {
            struct monst *mlsh = find_mid(obj->leashmon, FM_FMON);

            if (!mlsh) {
                impossible("leashed monster not on this level");
                obj->leashmon = 0;
            } else {
                Sprintf(eos(bp), " (attached to %s)",
                        noit_mon_nam(mlsh));
            }
            break;
        }
        if (is_obj_candelabrum(obj)) 
        {
            if (!obj->special_quality)
                Strcpy(tmpbuf, "no");
            else
                Sprintf(tmpbuf, "%d", obj->special_quality);
            if (lit_in_front)
            {
                if(obj->lamplit)
                    Strcat(prefix, "lit ");

                Sprintf(eos(bp), " with %s candle%s%s", tmpbuf, plur(obj->special_quality),
                    " attached");
            }
            else
                Sprintf(eos(bp), " (%s candle%s%s)", tmpbuf, plur(obj->special_quality),
                        !obj->lamplit ? " attached" : ", lit");
            break;
        } 
        else if (is_lamp(obj) || is_candle(obj) || is_torch(obj))
        {
            if ((is_candle(obj) && obj->otyp != MAGIC_CANDLE
                && obj->age < candle_maximum_burn_time(obj))
                || (obj->otyp == MAGIC_CANDLE && obj->special_quality < 2)
                || (is_torch(obj) && obj->age < torch_maximum_burn_time(obj))
                )
                Strcat(prefix, "partly used ");
            if (obj->lamplit)
            {
                if(lit_in_front)
                    Strcat(prefix, "lit ");
                else
                    Strcat(bp, " (lit)");
            }
            break;
        }
//        if (objects[obj->otyp].oc_charged)
//            goto charges;
        break;
    case WAND_CLASS:
 //charges:
        break;
    case POTION_CLASS:
        if (obj->otyp == POT_OIL && obj->lamplit)
        {
            if (lit_in_front)
                Strcat(prefix, "lit ");
            else
                Strcat(bp, " (lit)");
        }
        break;
    case RING_CLASS:
 ring:
        if (obj->owornmask & W_RINGR)
            Strcat(bp, " (on right ");
        if (obj->owornmask & W_RINGL)
            Strcat(bp, " (on left ");
        if (obj->owornmask & W_RING) {
            Strcat(bp, body_part(HAND));
            Strcat(bp, ")");
        }
        if (known && objects[obj->otyp].oc_enchantable) {
            Strcat(prefix, sitoa(obj->enchantment));
            Strcat(prefix, " ");
        }
        break;
    case FOOD_CLASS:

        if (obj->oeaten)
            Strcat(prefix, "partly eaten ");
        
        if (is_obj_rotting_corpse(obj) && (obj->speflags & SPEFLAGS_ROTTING_STATUS_KNOWN) != 0)
        {
            long rotted = get_rotted_status(obj);
            if (obj->orotten || rotted > 3L)
                Strcat(prefix, rotted > 5L ? "tainted " : "rotten ");
            else
                Strcat(prefix, "unrotten ");
        }

        if (obj->otyp == CORPSE) {
            /* (quan == 1) => want corpse_xname() to supply article,
               (quan != 1) => already have count or "some" as prefix;
               "corpse" is already in the buffer returned by xname() */
            unsigned cxarg = (((obj->quan != 1L) ? 0 : CXN_ARTICLE)
                              | CXN_NOCORPSE);
            char *cxstr = corpse_xname(obj, prefix, cxarg);

            Sprintf(prefix, "%s ", cxstr);
            /* avoid having doname(corpse) consume an extra obuf */
            releaseobuf(cxstr);
        } else if (obj->otyp == EGG) {
#if 0 /* corpses don't tell if they're stale either */
            if (known && stale_egg(obj))
                Strcat(prefix, "stale ");
#endif
            if (omndx >= LOW_PM
                && (known || (mvitals[omndx].mvflags & MV_KNOWS_EGG))) {
                Strcat(prefix, mons[omndx].mname);
                Strcat(prefix, " ");
                if (obj->speflags & SPEFLAGS_YOURS)
                    Strcat(bp, " (laid by you)");
            }
        }
        if (obj->otyp == MEAT_RING)
            goto ring;
        break;
    case BALL_CLASS:
    case CHAIN_CLASS:
        add_erosion_words(obj, prefix);
        if (obj->owornmask & W_BALL)
            Strcat(bp, " (chained to you)");
        break;
    case GEM_CLASS:
        if (objects[obj->otyp].oc_enchantable && (obj->enchantment != 0 || obj->elemental_enchantment != 0 || obj->exceptionality != 0))
            goto weapon_here;
        break;
    }

    const char* hand_s = body_part(HAND);
    const char* hands_s = makeplural(hand_s);
    if ((obj->owornmask & W_WEP) && !mrg_to_wielded) {
        if (obj->quan != 1L || !is_wieldable_weapon(obj))
        {
            if (u.twoweap)
            {
                if(bimanual(obj))
                    Sprintf(eos(bp), " (wielded in %s)", hands_s);
                else
                    Sprintf(eos(bp), " (wielded in right %s)", hand_s);
            }
            else
                Strcat(bp, " (wielded)");
        } else {

            Sprintf(eos(bp), " (%sweapon in %s%s)",
                    is_obj_tethered_weapon(obj, obj->owornmask) ? "tethered " : "", u.twoweap && !bimanual(obj) ? "right " : "", bimanual(obj)? hands_s : hand_s);

        }
    }

    if ((obj->owornmask & W_WEP2) && obj->oclass != ARMOR_CLASS && !mrg_to_wielded) {
        if (obj->quan != 1L || !is_wieldable_weapon(obj))
        {
            if (u.twoweap)
                if (bimanual(obj))
                    Sprintf(eos(bp), " (wielded in %s)", hands_s);
                else
                    Sprintf(eos(bp), " (wielded in left %s)", hand_s);
            else
                Strcat(bp, " (also wielded)");
        }
        else {
            /* note: Sting's glow message, if added, will insert text
               in front of "(weapon in hand)"'s closing paren */
            Sprintf(eos(bp), " (%s%sweapon in %s%s)",
                (!u.twoweap ? "unused " : ""),
                (is_obj_tethered_weapon(obj, obj->owornmask) ? "tethered " : ""),
                (!bimanual(obj) ? (u.twoweap ? "left " : "another ") : ""), 
                (bimanual(obj) ? hands_s : hand_s)
            );
        }
    }

    if (obj->owornmask & W_SWAPWEP) {
        if (u.twoweap && !bimanual(obj))
            Sprintf(eos(bp), " (readied as alternate right %s weapon)", hand_s);
        else
            Strcat(bp, " (readied as alternate weapon)");
    }
    if (obj->owornmask & W_SWAPWEP2) {
        if (is_shield(obj))
            Strcat(bp, " (readied as shield)");
        else
        {
            if (u.twoweap && !bimanual(obj))
                Sprintf(eos(bp), " (readied as alternate left %s weapon)", hand_s);
            else
                Strcat(bp, " (readied as another alternate weapon)");
        }
    }
    if (obj->owornmask & W_QUIVER) {
        switch (obj->oclass) {
        case WEAPON_CLASS:
            if (is_ammo(obj)) {
                if (objects[obj->otyp].oc_skill == -P_BOW) {
                    /* Ammo for a bow */
                    Strcat(bp, " (in quiver)");
                    break;
                } else {
                    /* Ammo not for a bow */
                    Strcat(bp, " (in quiver pouch)");
                    break;
                }
            } else {
                /* Weapons not considered ammo */
                Strcat(bp, " (at the ready)");
                break;
            }
        /* Small things and ammo not for a bow */
        case RING_CLASS:
        case REAGENT_CLASS:
        case AMULET_CLASS:
        case WAND_CLASS:
        case COIN_CLASS:
        case GEM_CLASS:
            Strcat(bp, " (in quiver pouch)");
            break;
        default: /* odd things */
            Strcat(bp, " (at the ready)");
        }
    }

    /* treat 'restoring' like suppress_price because shopkeeper and
       bill might not be available yet while restore is in progress */
    if (iflags.suppress_price || restoring || saving) {
        ; /* don't attempt to obtain any stop pricing, even if 'with_price' */
    } else if (is_unpaid(obj)) { /* in inventory or in container in invent */
        long quotedprice = unpaid_cost(obj, TRUE);

        Sprintf(eos(bp), " (%s, %ld %s)",
                obj->unpaid ? "unpaid" : "contents",
                quotedprice, currency(quotedprice));
    } else if (with_price) { /* on floor or in container on floor */
        int nochrg = 0;
        long price = get_cost_of_shop_item(obj, &nochrg);

        if (price > 0L)
            Sprintf(eos(bp), " (%s, %ld %s)",
                    nochrg ? "contents" : "for sale",
                    price, currency(price));
        else if (nochrg > 0)
            Strcat(bp, " (no charge)");
    }
    if (!strncmp(prefix, "a ", 2)) {
        /* save current prefix, without "a "; might be empty */
        Strcpy(tmpbuf, prefix + 2);
        /* set prefix[] to "", "a ", or "an " */
        (void) just_an(prefix, *tmpbuf ? tmpbuf : bp);
        /* append remainder of original prefix */
        Strcat(prefix, tmpbuf);
    }

    /* show weight for items (debug tourist info)
     * aum is stolen from Crawl's "Arbitrary Unit of Measure" */

    bp = strprepend(bp, prefix);

    /* Mark if invoke, repower, cooling down */
    if (obj->invokeon > 0 || obj->invokeleft > 0 || obj->repowerleft > 0 || obj->cooldownleft > 0)
    {
        boolean addcomma = FALSE;
        Strcat(bp, " (");
        if (obj->invokeon > 0 || obj->invokeleft > 0)
        {
            Strcat(bp, "invoked");
            addcomma = TRUE;
        }
        if (obj->repowerleft > 0)
        {
            if(addcomma)
                Strcat(bp, ", ");

            Strcat(bp, "repowering");
            addcomma = TRUE;
        }
        if (obj->cooldownleft > 0)
        {
            if (addcomma)
                Strcat(bp, ", ");

            Strcat(bp, "cooling down");
            addcomma = TRUE;
        }
        Strcat(bp, ")");
    }

    /* Mark if glowing when detected something */
    if (obj->detectioncount > 0)
    {
        char colorbuf[BUFSZ] = "red";
        if (obj->oartifact)
            strcpy(colorbuf, glow_color(obj->oartifact));

        if (!obj->oartifact || strcmp(colorbuf, "no color") == 0)
        {
            if ((objects[obj->otyp].oc_flags2 & O2_FLICKER_COLOR_MASK) == O2_FLICKER_COLOR_BLACK)
                strcpy(colorbuf, "black");
            else if ((objects[obj->otyp].oc_flags2 & O2_FLICKER_COLOR_MASK) == O2_FLICKER_COLOR_WHITE)
                strcpy(colorbuf, "white");
            else if ((objects[obj->otyp].oc_flags2 & O2_FLICKER_COLOR_MASK) == O2_FLICKER_COLOR_BLUE)
                strcpy(colorbuf, "blue");
            else
                strcpy(colorbuf, "red");
        }
        if (!Blind)
            Sprintf(eos(bp), " (%s %s)",
                glow_verb(obj->detectioncount, TRUE),
                colorbuf);
    }

    /* Mark if glowing when detected something */
    if (is_obj_special_praying_item(obj) && obj->where == OBJ_INVENT && obj->blessed && can_pray(FALSE))
    {
        if (!Blind)
            Sprintf(eos(bp), " (%s)", "shimmering");
    }

    //Weights

    //double objweight = 0;
    int objweight_oz = 0;
    if (!loadstonecorrectly && obj->otyp == LOADSTONE)
        objweight_oz = objects[LUCKSTONE].oc_weight;  
    else
        objweight_oz = obj->owt; 

    //objweight = ((double)objweight_oz) / 16;  //ounces to lbs

    if ((wizard && iflags.wizweight) || weightfirst) 
    {
        char weightbuf[BUFSZ];
        printweight(weightbuf, objweight_oz, TRUE, TRUE);
        char buf[BUFSZ];
        Sprintf(buf, "%s - %s", weightbuf, bp);
        Strcpy(bp, buf);

    }
    
    if (weightlast)
    {
        char weightbuf[BUFSZ];
        printweight(weightbuf, objweight_oz, FALSE, FALSE);
        char buf[BUFSZ];
        Sprintf(buf, "%s (%s)", bp, weightbuf);
        Strcpy(bp, buf);
    }

    return bp;
}

char *
doname(obj)
struct obj *obj;
{
    return doname_base(obj, 0U);
}

char*
doname_in_text(obj)
struct obj* obj;
{
    return doname_base(obj, DONAME_LIT_IN_FRONT);
}

/* Name of object including price. */
char *
doname_with_price(obj)
struct obj *obj;
{
    return doname_base(obj, DONAME_WITH_PRICE);
}

/* Name of object including price. */
char*
doname_with_price_and_weight_last(obj,  loadstonecorrectly)
struct obj* obj;
boolean loadstonecorrectly;
{
    return doname_base(obj, DONAME_WITH_PRICE | DONAME_WITH_WEIGHT_LAST | (loadstonecorrectly ? DONAME_LOADSTONE_CORRECTLY : 0));
}

/* Name of object including price. */
char*
doname_in_text_with_price_and_weight_last(obj)
struct obj* obj;
{
    return doname_base(obj, DONAME_LIT_IN_FRONT | DONAME_WITH_PRICE | DONAME_WITH_WEIGHT_LAST | (objects[LOADSTONE].oc_name_known ? DONAME_LOADSTONE_CORRECTLY : 0));
}

/* Name of object including price. */
char*
doname_with_price_and_weight_first(obj, loadstonecorrectly)
struct obj* obj;
boolean loadstonecorrectly;
{
    return doname_base(obj, DONAME_WITH_PRICE | DONAME_WITH_WEIGHT_FIRST | (loadstonecorrectly ? DONAME_LOADSTONE_CORRECTLY : 0));
}

/* "some" instead of precise quantity if obj->dknown not set */
char *
doname_vague_quan(obj)
struct obj *obj;
{
    /* Used by farlook.
     * If it hasn't been seen up close and quantity is more than one,
     * use "some" instead of the quantity: "some gold pieces" rather
     * than "25 gold pieces".  This is suboptimal, to put it mildly,
     * because lookhere and pickup report the precise amount.
     * Picking the item up while blind also shows the precise amount
     * for inventory display, then dropping it while still blind leaves
     * obj->dknown unset so the count reverts to "some" for farlook.
     *
     * TODO: add obj->qknown flag for 'quantity known' on stackable
     * items; it could overlay obj->cknown since no containers stack.
     */
    return doname_base(obj, DONAME_VAGUE_QUAN);
}

char*
doname_with_weight_first(obj, loadstonecorrectly)
struct obj* obj;
boolean loadstonecorrectly;

{
    return doname_base(obj, DONAME_WITH_WEIGHT_FIRST | (loadstonecorrectly ? DONAME_LOADSTONE_CORRECTLY : 0));
}

char*
doname_with_weight_first_true(obj)
struct obj* obj;
{
    return doname_base(obj, DONAME_WITH_WEIGHT_FIRST | DONAME_LOADSTONE_CORRECTLY);
}

char*
doname_with_weight_last_true(obj)
struct obj* obj;
{
    return doname_base(obj, DONAME_WITH_WEIGHT_LAST | DONAME_LOADSTONE_CORRECTLY);
}



char*
doname_with_weight_last(obj, loadstonecorrectly)
struct obj* obj;
boolean loadstonecorrectly;
{
    return doname_base(obj, DONAME_WITH_WEIGHT_LAST | (loadstonecorrectly ? DONAME_LOADSTONE_CORRECTLY : 0));
}


/* used from invent.c */
boolean
not_fully_identified(otmp)
struct obj *otmp;
{
    /* gold doesn't have any interesting attributes [yet?] */
    if (otmp->oclass == COIN_CLASS)
        return FALSE; /* always fully ID'd */

    if (context.shop_identify_type > 0 && !is_shop_item_type(otmp, context.shop_identify_type - 1))
        return FALSE;

    /* Smith */
    if (context.npc_identify_type == -1 && !(is_weapon(otmp) || otmp->oclass == ARMOR_CLASS))
        return FALSE;

    if (context.npc_identify_type > 0 && !is_npc_item_identification_type(otmp, context.npc_identify_type))
        return FALSE;

    /* check fundamental ID hallmarks first */
    if (!otmp->known || !otmp->dknown
        || (!otmp->bknown && otmp->otyp != SCR_MAIL)
        || !objects[otmp->otyp].oc_name_known)
        return TRUE;
    if ((!otmp->cknown && (Is_container(otmp) || otmp->otyp == STATUE))
        || (!otmp->lknown && Is_box(otmp)) || (!otmp->tknown && Is_box(otmp)))
        return TRUE;
    if (otmp->oartifact && undiscovered_artifact(otmp->oartifact))
        return TRUE;
    if (otmp->oartifact && !otmp->aknown)
        return TRUE;
    if (!otmp->nknown && has_oname(otmp))
        return TRUE;
    if ((otmp->mythic_prefix || otmp->mythic_suffix) && !otmp->mknown)
        return TRUE;
    if (is_obj_rotting_corpse(otmp) && (otmp->speflags & SPEFLAGS_ROTTING_STATUS_KNOWN) == 0)
        return TRUE;
    /* otmp->rknown is the only item of interest if we reach here */
    /*
     *  Note:  if a revision ever allows scrolls to become fireproof or
     *  rings to become shockproof, this checking will need to be revised.
     *  `rknown' ID only matters if xname() will provide the info about it.
     */
    if (otmp->rknown
        || (otmp->oclass != ARMOR_CLASS && otmp->oclass != WEAPON_CLASS
            && !is_weptool(otmp)            /* (redundant) */
            && otmp->oclass != BALL_CLASS)) /* (useless) */
        return FALSE;
    else /* lack of `rknown' only matters for vulnerable objects */
        return (boolean) (is_rustprone(otmp) || is_corrodeable(otmp)
                          || is_flammable(otmp));
}

/* format a corpse name (xname() omits monster type; doname() calls us);
   eatcorpse() also uses us for death reason when eating tainted glob */
char *
corpse_xname(otmp, adjective, cxn_flags)
struct obj *otmp;
const char *adjective;
unsigned cxn_flags; /* bitmask of CXN_xxx values */
{
    char *nambuf = nextobuf();
    int omndx = otmp->corpsenm;
    struct monst* mtmp = get_mtraits(otmp, FALSE);
    boolean isfemale = (mtmp && mtmp->female) || is_female(&mons[omndx]);

    boolean ignore_quan = (cxn_flags & CXN_SINGULAR) != 0,
            /* suppress "the" from "the unique monster corpse" */
        no_prefix = (cxn_flags & CXN_NO_PFX) != 0,
            /* include "the" for "the woodchuck corpse */
        the_prefix = (cxn_flags & CXN_PFX_THE) != 0,
            /* include "an" for "an ogre corpse */
        any_prefix = (cxn_flags & CXN_ARTICLE) != 0,
            /* leave off suffix (do_name() appends "corpse" itself) */
        omit_corpse = (cxn_flags & CXN_NOCORPSE) != 0,
        possessive = FALSE,
        glob = (otmp->otyp != CORPSE && otmp->globby);
    const char *mname;

    if (glob)
    {
        mname = OBJ_NAME(objects[otmp->otyp]); /* "glob of <monster>" */
    }
    else if (omndx == NON_PM)
    { /* paranoia */
        mname = "thing";
        /* [Possible enhancement:  check whether corpse has monster traits
            attached in order to use priestname() for priests and minions.] */
    } 
    else if (omndx == PM_ALIGNED_PRIEST)
    {
        /* avoid "aligned priest"; it just exposes internal details */
        if(isfemale)
            mname = "priestess";
        else
            mname = "priest";
    }
    else
    {
        mname = pm_monster_name(&mons[omndx], isfemale);
        if (the_unique_pm(&mons[omndx]) || is_mname_proper_name(&mons[omndx])) 
        {
            mname = s_suffix(mname);
            possessive = TRUE;
            /* don't precede personal name like "Medusa" with an article */
            if (is_mname_proper_name(&mons[omndx]))
                no_prefix = TRUE;
            /* always precede non-personal unique monster name like
               "Oracle" with "the" unless explicitly overridden */
            else if (the_unique_pm(&mons[omndx]) && !no_prefix)
                the_prefix = TRUE;
        }
    }
    if (no_prefix)
        the_prefix = any_prefix = FALSE;
    else if (the_prefix)
        any_prefix = FALSE; /* mutually exclusive */

    *nambuf = '\0';
    /* can't use the() the way we use an() below because any capitalized
       Name causes it to assume a personal name and return Name as-is;
       that's usually the behavior wanted, but here we need to force "the"
       to precede capitalized unique monsters (pnames are handled above) */
    if (the_prefix)
        Strcat(nambuf, "the ");

    if (!adjective || !*adjective)
    {
        /* normal case:  newt corpse */
        Strcat(nambuf, mname);
    } 
    else 
    {
        /* adjective positioning depends upon format of monster name */
        if (possessive) /* Medusa's cursed partly eaten corpse */
            Sprintf(eos(nambuf), "%s %s", mname, adjective);
        else /* cursed partly eaten troll corpse */
            Sprintf(eos(nambuf), "%s %s", adjective, mname);
        /* in case adjective has a trailing space, squeeze it out */
        mungspaces(nambuf);
        /* doname() might include a count in the adjective argument;
           if so, don't prepend an article */
        if (digit(*adjective))
            any_prefix = FALSE;
    }

    if (glob) {
        ; /* omit_corpse doesn't apply; quantity is always 1 */
    } else if (!omit_corpse) {
        Strcat(nambuf, " corpse");
        /* makeplural(nambuf) => append "s" to "corpse" */
        if (otmp->quan > 1L && !ignore_quan) {
            Strcat(nambuf, "s");
            any_prefix = FALSE; /* avoid "a newt corpses" */
        }
    }

    /* it's safe to overwrite our nambuf after an() has copied
       its old value into another buffer */
    if (any_prefix)
        Strcpy(nambuf, an(nambuf));

    return nambuf;
}

/* xname doesn't include monster type for "corpse"; cxname does */
char *
cxname(obj)
struct obj *obj;
{
    if (obj->otyp == CORPSE)
        return corpse_xname(obj, (const char *) 0, CXN_NORMAL);
    return xname(obj);
}

char*
acxname(obj)
struct obj* obj;
{
    if (obj->otyp == CORPSE)
        return corpse_xname(obj, (const char*)0, CXN_ARTICLE);
    return obj->oartifact && obj->aknown ? the(xname(obj)) : an(xname(obj));
}

/* like cxname, but ignores quantity */
char *
cxname_singular(obj)
struct obj *obj;
{
    if (obj->otyp == CORPSE)
        return corpse_xname(obj, (const char *) 0, CXN_SINGULAR);
    return xname_flags(obj, CXN_SINGULAR);
}

/* treat an object as fully ID'd when it might be used as reason for death */
char*
killer_xname(obj)
struct obj* obj;
{
    return killer_xname_flags(obj, 0);
}

char *
killer_xname_flags(obj, kxnflags)
struct obj *obj;
unsigned kxnflags;
{
    struct obj save_obj;
    unsigned save_ocknown;
    char *buf, *save_ocuname, *save_uoname = (char *) 0;

    if (!obj)
    {
        buf = nextobuf();
        strcpy(buf, empty_string);
        return buf;
    }

    /* bypass object twiddling for artifacts */
    if (obj->oartifact)
        return bare_artifactname(obj);

    /* remember original settings for core of the object;
       oextra structs other than oname don't matter here--since they
       aren't modified they don't need to be saved and restored */
    save_obj = *obj;
    if (has_uoname(obj))
        save_uoname = UONAME(obj);

    /* killer name should be more specific than general xname; however, exact
       info like blessed/cursed and rustproof makes things be too verbose */
    obj->known = obj->dknown = 1;
    obj->bknown = obj->rknown = obj->greased = 0;
    /* if character is a priest[ess], bknown will get toggled back on */
    if (obj->otyp != POT_WATER)
        obj->blessed = obj->cursed = 0;
    else
        obj->bknown = 1; /* describe holy/unholy water as such */
    /* "killed by poisoned <obj>" would be misleading when poison is
       not the cause of death and "poisoned by poisoned <obj>" would
       be redundant when it is, so suppress "poisoned" prefix */
    obj->opoisoned = 0;
    /* strip user-supplied name; artifacts keep theirs */
    if (save_uoname)
        UONAME(obj) = (char *) 0;

    /* temporarily identify the type of object */
    save_ocknown = objects[obj->otyp].oc_name_known;
    objects[obj->otyp].oc_name_known = 1;
    save_ocuname = objects[obj->otyp].oc_uname;
    objects[obj->otyp].oc_uname = 0; /* avoid "foo called bar" */

    /* format the object */
    if (obj->otyp == CORPSE) {
        buf = nextobuf();
        Strcpy(buf, corpse_xname(obj, (const char *) 0, CXN_NORMAL));
    } else if (obj->otyp == SLIME_MOLD) {
        /* concession to "most unique deaths competition" in the annual
           devnull tournament, suppress player supplied fruit names because
           those can be used to fake other objects and dungeon features */
        buf = nextobuf();
        Sprintf(buf, "deadly slime mold%s", plur(obj->quan));
    } else if (obj->oclass == SPBOOK_CLASS && (kxnflags & KXNFLAGS_SPELL) != 0) {
        /* It is a spell rather than the book itself */
        buf = nextobuf();
        Sprintf(buf, "spell of %s", OBJ_NAME(objects[obj->otyp]));
    } else {
        buf = xname(obj);
    }
    /* apply an article if appropriate; caller should always use KILLED_BY */
    if (!(kxnflags & KXNFLAGS_NO_ARTICLE))
    {
        if (obj->quan == 1L && !strstri(buf, "'s ") && !strstri(buf, "s' "))
            buf = (obj_is_pname(obj) || the_unique_obj(obj)) ? the(buf) : an(buf);
    }

    objects[obj->otyp].oc_name_known = save_ocknown;
    objects[obj->otyp].oc_uname = save_ocuname;
    *obj = save_obj; /* restore object's core settings */
    if (save_uoname)
        UONAME(obj) = save_uoname;

    return buf;
}

/* xname,doname,&c with long results reformatted to omit some stuff */
char *
short_oname(obj, func, altfunc, lenlimit)
struct obj *obj;
char *FDECL((*func), (OBJ_P)),    /* main formatting routine */
     *FDECL((*altfunc), (OBJ_P)); /* alternate for shortest result */
size_t lenlimit;
{
    struct obj save_obj;
    char unamebuf[12], onamebuf[12], *save_oname, *save_uname, *outbuf;

    outbuf = (*func)(obj);
    if (strlen(outbuf) <= lenlimit)
        return outbuf;

    /* shorten called string to fairly small amount */
    save_uname = objects[obj->otyp].oc_uname;
    if (save_uname && strlen(save_uname) >= sizeof unamebuf) {
        (void) strncpy(unamebuf, save_uname, sizeof unamebuf - 4);
        Strcpy(unamebuf + sizeof unamebuf - 4, "...");
        objects[obj->otyp].oc_uname = unamebuf;
        releaseobuf(outbuf);
        outbuf = (*func)(obj);
        objects[obj->otyp].oc_uname = save_uname; /* restore called string */
        if (strlen(outbuf) <= lenlimit)
            return outbuf;
    }

    /* shorten named string to fairly small amount */
    save_oname = has_oname(obj) ? ONAME(obj) : 0;
    if (save_oname && strlen(save_oname) >= sizeof onamebuf) {
        (void) strncpy(onamebuf, save_oname, sizeof onamebuf - 4);
        Strcpy(onamebuf + sizeof onamebuf - 4, "...");
        ONAME(obj) = onamebuf;
        releaseobuf(outbuf);
        outbuf = (*func)(obj);
        ONAME(obj) = save_oname; /* restore named string */
        if (strlen(outbuf) <= lenlimit)
            return outbuf;
    }

    /* shorten both called and named strings;
       unamebuf and onamebuf have both already been populated */
    if (save_uname && strlen(save_uname) >= sizeof unamebuf && save_oname
        && strlen(save_oname) >= sizeof onamebuf) {
        objects[obj->otyp].oc_uname = unamebuf;
        ONAME(obj) = onamebuf;
        releaseobuf(outbuf);
        outbuf = (*func)(obj);
        if (strlen(outbuf) <= lenlimit) {
            objects[obj->otyp].oc_uname = save_uname;
            ONAME(obj) = save_oname;
            return outbuf;
        }
    }

    /* still long; strip several name-lengthening attributes;
       called and named strings are still in truncated form */
    save_obj = *obj;
    obj->bknown = obj->rknown = obj->greased = 0;
    obj->oeroded = obj->oeroded2 = 0;
    releaseobuf(outbuf);
    outbuf = (*func)(obj);
    if (altfunc && strlen(outbuf) > lenlimit) {
        /* still long; use the alternate function (usually one of
           the jackets around minimal_xname()) */
        releaseobuf(outbuf);
        outbuf = (*altfunc)(obj);
    }
    /* restore the object */
    *obj = save_obj;
    if (save_oname)
        ONAME(obj) = save_oname;
    if (save_uname)
        objects[obj->otyp].oc_uname = save_uname;

    /* use whatever we've got, whether it's too long or not */
    return outbuf;
}

/*
 * Used if only one of a collection of objects is named (e.g. in eat.c).
 */
const char *
singular(otmp, func)
register struct obj *otmp;
char *FDECL((*func), (OBJ_P));
{
    long savequan;
    char *nam;

    /* using xname for corpses does not give the monster type */
    if (otmp->otyp == CORPSE && func == xname)
        func = cxname;

    savequan = otmp->quan;
    otmp->quan = 1L;
    nam = (*func)(otmp);
    otmp->quan = savequan;
    return nam;
}

/* pick "", "a ", or "an " as article for 'str'; used by an() and doname() */
STATIC_OVL char *
just_an(outbuf, str)
char *outbuf;
const char *str;
{
    char c0;

    *outbuf = '\0';
    c0 = lowc(*str);
    if (!str[1]) {
        /* single letter; might be used for named fruit */
        Strcpy(outbuf, index("aefhilmnosx", c0) ? "an " : "a ");
    } else if (!strncmpi(str, "the ", 4) || !strcmpi(str, "molten lava")
               || !strcmpi(str, "iron bars") || !strcmpi(str, "ice")) {
        ; /* no article */
    } else {
        if ((index(vowels, c0) && strncmpi(str, "one-", 4)
             && strncmpi(str, "eucalyptus", 10) && strncmpi(str, "unicorn", 7)
             && strncmpi(str, "uranium", 7) && strncmpi(str, "useful", 6))
            || (index("x", c0) && !index(vowels, lowc(str[1]))))
            Strcpy(outbuf, "an ");
        else
            Strcpy(outbuf, "a ");
    }
    return outbuf;
}

char *
an(str)
const char *str;
{
    char *buf = nextobuf();

    if (!str || !*str) {
        impossible("Alphabet soup: 'an(%s)'.", str ? "\"\"" : "<null>");
        return strcpy(buf, "an []");
    }
    (void) just_an(buf, str);
    return strcat(buf, str);
}

char *
An(str)
const char *str;
{
    char *tmp = an(str);

    *tmp = highc(*tmp);
    return tmp;
}

/*
 * Prepend "the" if necessary; assumes str is a subject derived from xname.
 * Use is_mname_proper_name() for monster names, not the().  the() is idempotent.
 */
char *
the(str)
const char *str;
{
    char *buf = nextobuf();
    boolean insert_the = FALSE;

    if (!str || !*str) {
        impossible("Alphabet soup: 'the(%s)'.", str ? "\"\"" : "<null>");
        return strcpy(buf, "the []");
    }
    if (!strncmpi(str, "the ", 4)) {
        buf[0] = lowc(*str);
        Strcpy(&buf[1], str + 1);
        return buf;
    } else if (*str < 'A' || *str > 'Z'
               /* treat named fruit as not a proper name, even if player
                  has assigned a capitalized proper name as his/her fruit */
               || fruit_from_name(str, TRUE, (int *) 0)) {
        /* not a proper name, needs an article */
        insert_the = TRUE;
    } else {
        /* Probably a proper name, might not need an article */
        register char *tmp, *named, *called, *entitled, *labeled;
        size_t l;

        /* some objects have capitalized adjectives in their names */
        if (((tmp = rindex(str, ' ')) != 0 || (tmp = rindex(str, '-')) != 0)
            && (tmp[1] < 'A' || tmp[1] > 'Z')) {
            insert_the = TRUE;
        } else if (tmp && index(str, ' ') < tmp) { /* has spaces */
            /* it needs an article if the name contains "of" */
            tmp = strstri(str, " of ");
            named = strstri(str, " named ");
            called = strstri(str, " called ");
            entitled = strstri(str, " entitled ");
            labeled = strstri(str, " labeled ");
            if (!named && entitled)
                named = entitled;
            if (!named && labeled)
                named = labeled;
            if (called && (!named || called < named))
                named = called;

            if (tmp && (!named || tmp < named)) /* found an "of" */
                insert_the = TRUE;
            /* stupid special case: lacks "of" but needs "the" */
            else if (!named && (l = strlen(str)) >= 31
                     && !strcmp(&str[l - 31],
                                "Platinum Yendorian Express Card"))
                insert_the = TRUE;
        }
    }
    if (insert_the)
        Strcpy(buf, "the ");
    else
        buf[0] = '\0';
    Strcat(buf, str);

    return buf;
}

char *
The(str)
const char *str;
{
    char *tmp = the(str);

    *tmp = highc(*tmp);
    return tmp;
}

/* returns "count cxname(otmp)" or just cxname(otmp) if count == 1 */
char *
aobjnam(otmp, verb)
struct obj *otmp;
const char *verb;
{
    char prefix[PREFIX];
    char *bp = cxname(otmp);

    if (otmp->quan != 1L) {
        Sprintf(prefix, "%ld ", otmp->quan);
        bp = strprepend(bp, prefix);
    }
    if (verb) {
        Strcat(bp, " ");
        Strcat(bp, otense(otmp, verb));
    }
    return bp;
}

/* combine yname and aobjnam eg "your count cxname(otmp)" */
char *
yobjnam(obj, verb)
struct obj *obj;
const char *verb;
{
    char *s = aobjnam(obj, verb);

    /* leave off "your" for most of your artifacts, but prepend
     * "your" for unique objects and "foo of bar" quest artifacts */
    if (!carried(obj) || !obj_is_pname(obj)
        || any_quest_artifact(obj)) {
        char *outbuf = shk_your(nextobuf(), obj);
        size_t space_left = BUFSZ - 1 - strlen(outbuf);

        s = strncat(outbuf, s, space_left);
    }
    return s;
}

/* combine Yname2 and aobjnam eg "Your count cxname(otmp)" */
char *
Yobjnam2(obj, verb)
struct obj *obj;
const char *verb;
{
    register char *s = yobjnam(obj, verb);

    *s = highc(*s);
    return s;
}

/* like aobjnam, but prepend "The", not count, and use xname */
char *
Tobjnam(otmp, verb)
struct obj *otmp;
const char *verb;
{
    char *bp = The(xname(otmp));

    if (verb) {
        Strcat(bp, " ");
        Strcat(bp, otense(otmp, verb));
    }
    return bp;
}

/* capitalized variant of doname() */
char *
Doname2(obj)
struct obj *obj;
{
    char *s = doname(obj);

    *s = highc(*s);
    return s;
}

/* returns "[your ]xname(obj)" or "Foobar's xname(obj)" or "the xname(obj)" */
char *
yname(obj)
struct obj *obj;
{
    char *s = cxname(obj);

    /* leave off "your" for most of your artifacts, but prepend
     * "your" for unique objects and "foo of bar" quest artifacts */
    if (!carried(obj) || !obj_is_pname(obj)
        || any_quest_artifact(obj)) 
    {
        char *outbuf = shk_your(nextobuf(), obj);
        size_t space_left = BUFSZ - 1 - strlen(outbuf);

        s = strncat(outbuf, s, space_left);
    }
    else if (obj_is_pname(obj) || the_unique_obj(obj))
    {
        if (strncmpi(s, "the ", 4))
        {
            char tempbuf[BUFSZ];
            strcpy(tempbuf, s);
            Sprintf(s, "the %s", tempbuf);
        }
    }

    return s;
}

/* capitalized variant of yname() */
char *
Yname2(obj)
struct obj *obj;
{
    char *s = yname(obj);

    *s = highc(*s);
    return s;
}

/* returns "your minimal_xname(obj)"
 * or "Foobar's minimal_xname(obj)"
 * or "the minimal_xname(obj)"
 */
char *
ysimple_name(obj)
struct obj *obj;
{
    char *outbuf = nextobuf();
    char *s = shk_your(outbuf, obj); /* assert( s == outbuf ); */
    size_t space_left = BUFSZ - 1 - strlen(s);

    char* min_name = minimal_xname(obj);

    return strncat(s, min_name, space_left);
}

/* capitalized variant of ysimple_name() */
char *
Ysimple_name2(obj)
struct obj *obj;
{
    char *s = ysimple_name(obj);

    *s = highc(*s);
    return s;
}

/* "scroll" or "scrolls" */
char *
simpleonames(obj)
struct obj *obj;
{
    char *simpleoname = minimal_xname(obj);

    if (obj->quan != 1L)
        simpleoname = makeplural(simpleoname);
    return simpleoname;
}

/* "a scroll" or "scrolls"; "a silver bell" or "the Bell of Opening" */
char *
ansimpleoname(obj)
struct obj *obj;
{
    char *simpleoname = simpleonames(obj);
    int otyp = obj->otyp;

    /* prefix with "the" if a unique item, or a fake one imitating same,
       has been formatted with its actual name (we let typename() handle
       any `known' and `dknown' checking necessary) */
    if (otyp == FAKE_AMULET_OF_YENDOR)
        otyp = AMULET_OF_YENDOR;
    if (is_otyp_unique(otyp)
        && !strcmp(simpleoname, OBJ_NAME(objects[otyp])))
        return the(simpleoname);

    /* simpleoname is singular if quan==1, plural otherwise */
    if (obj->quan == 1L)
        simpleoname = an(simpleoname);
    return simpleoname;
}

/* "the scroll" or "the scrolls" */
char *
thesimpleoname(obj)
struct obj *obj;
{
    char *simpleoname = simpleonames(obj);

    return the(simpleoname);
}

/* artifact's name without any object type or known/dknown/&c feedback */
char *
bare_artifactname(obj)
struct obj *obj;
{
    char *outbuf;

    if (obj->oartifact) {
        outbuf = nextobuf();
        Strcpy(outbuf, artiname(obj->oartifact));
        if (!strncmp(outbuf, "The ", 4))
            outbuf[0] = lowc(outbuf[0]);
    } else {
        outbuf = xname(obj);
    }
    return outbuf;
}

STATIC_VAR const char *wrp[] = {
    "wand",   "ring",      "potion",     "scroll", "gem",
    "amulet", "spellbook", "spell book",
    /* for non-specific wishes */
    "weapon", "armor",     "tool",  "reagent",  "miscellaneous item",   "miscellaneous",     "food",   "comestible",
};
STATIC_VAR const char wrpsym[] = { WAND_CLASS,   RING_CLASS,   POTION_CLASS,
                               SCROLL_CLASS, GEM_CLASS,    AMULET_CLASS,
                               SPBOOK_CLASS, SPBOOK_CLASS, WEAPON_CLASS,
                               ARMOR_CLASS,  TOOL_CLASS, REAGENT_CLASS, MISCELLANEOUS_CLASS, MISCELLANEOUS_CLASS,
                               FOOD_CLASS,   FOOD_CLASS };

/* return form of the verb (input plural) if xname(otmp) were the subject */
char *
otense(otmp, verb)
struct obj *otmp;
const char *verb;
{
    char *buf;

    /*
     * verb is given in plural (without trailing s).  Return as input
     * if the result of xname(otmp) would be plural.  Don't bother
     * recomputing xname(otmp) at this time.
     */
    if (!is_plural(otmp))
        return vtense((char *) 0, verb);

    buf = nextobuf();
    Strcpy(buf, verb);
    return buf;
}

/* various singular words that vtense would otherwise categorize as plural;
   also used by makesingular() to catch some special cases */
STATIC_VAR const char *const special_subjs[] = {
    "erinys",  "manes", /* this one is ambiguous */
    "Cyclops", "Hippocrates",     "Pelias",    "aklys",
    "amnesia", "detect monsters", "paralysis", "shape changers",
    "nemesis", 0
    /* note: "detect monsters" and "shape changers" are normally
       caught via "<something>(s) of <whatever>", but they can be
       wished for using the shorter form, so we include them here
       to accommodate usage by makesingular during wishing */
};

/* return form of the verb (input plural) for present tense 3rd person subj */
char *
vtense(subj, verb)
register const char *subj;
register const char *verb;
{
    char *buf = nextobuf(), *bspot;
    size_t len, ltmp;
    const char *sp, *spot;
    const char *const *spec;

    /*
     * verb is given in plural (without trailing s).  Return as input
     * if subj appears to be plural.  Add special cases as necessary.
     * Many hard cases can already be handled by using otense() instead.
     * If this gets much bigger, consider decomposing makeplural.
     * Note: monster names are not expected here (except before corpse).
     *
     * Special case: allow null sobj to get the singular 3rd person
     * present tense form so we don't duplicate this code elsewhere.
     */
    if (subj) {
        if (!strncmpi(subj, "a ", 2) || !strncmpi(subj, "an ", 3))
            goto sing;
        spot = (const char *) 0;
        for (sp = subj; (sp = index(sp, ' ')) != 0; ++sp) {
            if (!strncmpi(sp, " of ", 4) || !strncmpi(sp, " from ", 6)
                || !strncmpi(sp, " called ", 8) || !strncmpi(sp, " named ", 7) || !strncmpi(sp, " entitled ", 10)
                || !strncmpi(sp, " labeled ", 9)) {
                if (sp != subj)
                    spot = sp - 1;
                break;
            }
        }
        len = strlen(subj);
        if (!spot)
            spot = subj + len - 1;

        /*
         * plural: anything that ends in 's', but not '*us' or '*ss'.
         * Guess at a few other special cases that makeplural creates.
         */
        if ((lowc(*spot) == 's' && spot != subj
             && !index("us", lowc(*(spot - 1))))
            || !BSTRNCMPI(subj, spot - 3, "eeth", 4)
            || !BSTRNCMPI(subj, spot - 3, "feet", 4)
            || !BSTRNCMPI(subj, spot - 1, "ia", 2)
            || !BSTRNCMPI(subj, spot - 1, "ae", 2)) {
            /* check for special cases to avoid false matches */
            len = (spot - subj) + 1;
            for (spec = special_subjs; *spec; spec++) {
                ltmp = strlen(*spec);
                if (len == ltmp && !strncmpi(*spec, subj, len))
                    goto sing;
                /* also check for <prefix><space><special_subj>
                   to catch things like "the invisible erinys" */
                if (len > ltmp && *(spot - ltmp) == ' '
                    && !strncmpi(*spec, spot - ltmp + 1, ltmp))
                    goto sing;
            }

            return strcpy(buf, verb);
        }
        /*
         * 3rd person plural doesn't end in telltale 's';
         * 2nd person singular behaves as if plural.
         */
        if (!strcmpi(subj, "they") || !strcmpi(subj, "you"))
            return strcpy(buf, verb);
    }

 sing:
    Strcpy(buf, verb);
    len = (int) strlen(buf);
    bspot = buf + len - 1;

    if (!strcmpi(buf, "are")) {
        Strcasecpy(buf, "is");
    } else if (!strcmpi(buf, "have")) {
        Strcasecpy(bspot - 1, "s");
    } else if (index("zxs", lowc(*bspot))
               || (len >= 2 && lowc(*bspot) == 'h'
                   && index("cs", lowc(*(bspot - 1))))
               || (len == 2 && lowc(*bspot) == 'o')) {
        /* Ends in z, x, s, ch, sh; add an "es" */
        Strcasecpy(bspot + 1, "es");
    } else if (lowc(*bspot) == 'y' && !index(vowels, lowc(*(bspot - 1)))) {
        /* like "y" case in makeplural */
        Strcasecpy(bspot, "ies");
    } else {
        Strcasecpy(bspot + 1, "s");
    }

    return buf;
}

struct sing_plur {
    const char *sing, *plur;
};

/* word pairs that don't fit into formula-based transformations;
   also some suffices which have very few--often one--matches or
   which aren't systematically reversible (knives, staves) */
STATIC_VAR const struct sing_plur one_off[] = {
    { "child",
      "children" },      /* (for wise guys who give their food funny names) */
    { "cubus", "cubi" }, /* in-/suc-cubus */
    { "culus", "culi" }, /* homunculus */
    { "djinni", "djinn" },
    { "erinys", "erinyes" },
    { "foot", "feet" },
    { "fungus", "fungi" },
    { "goose", "geese" },
    { "knife", "knives" },
    { "labrum", "labra" }, /* candelabrum */
    { "louse", "lice" },
    { "mouse", "mice" },
    { "mumak", "mumakil" },
    { "nemesis", "nemeses" },
    { "ovum", "ova" },
    { "ox", "oxen" },
    { "passerby", "passersby" },
    { "rtex", "rtices" }, /* vortex */
    { "serum", "sera" },
    { "staff", "staves" },
    { "tooth", "teeth" },
    { 0, 0 }
};

STATIC_VAR const char *const as_is[] = {
    /* makesingular() leaves these plural due to how they're used */
    "boots",   "shoes",     "gloves",    "lenses", "goggles", "glasses", "eye glasses", "scales",
    "eyes",    "gauntlets", "iron bars", "bracers", "wings", "earrings",
    "pants",   "trousers",  "trunks",
    /* both singular and plural are spelled the same */
    "bison",   "deer",      "elk",       "fish",      "fowl",
    "tuna",    "yaki",      "-hai",      "krill",     "manes",
    "moose",   "ninja",     "sheep",     "ronin",     "roshi",
    "shito",   "tengu",     "ki-rin",    "Nazgul",    "gunyoki",
    "piranha", "royalty",   "samurai",   "shuriken", 0,
    /* Note:  "fish" and "piranha" are collective plurals, suitable
       for "wiped out all <foo>".  For "3 <foo>", they should be
       "fishes" and "piranhas" instead.  We settle for collective
       variant instead of attempting to support both. */
};

/* singularize/pluralize decisions common to both makesingular & makeplural */
STATIC_OVL boolean
singplur_lookup(basestr, endstring, to_plural, alt_as_is)
char *basestr, *endstring;    /* base string, pointer to eos(string) */
boolean to_plural;            /* true => makeplural, false => makesingular */
const char *const *alt_as_is; /* another set like as_is[] */
{
    const struct sing_plur *sp;
    const char *same, *other, *const *as;
    int al;
    size_t baselen = strlen(basestr);

    for (as = as_is; *as; ++as) {
        al = (int) strlen(*as);
        if (!BSTRCMPI(basestr, endstring - al, *as))
            return TRUE;
    }
    if (alt_as_is) {
        for (as = alt_as_is; *as; ++as) {
            al = (int) strlen(*as);
            if (!BSTRCMPI(basestr, endstring - al, *as))
                return TRUE;
        }
    }

    /* avoid false hit on one_off[].plur == "lice" or .sing == "goose";
       if more of these turn up, one_off[] entries will need to flagged
       as to which are whole words and which are matchable as suffices
       then matching in the loop below will end up becoming more complex */
    if (!strcmpi(basestr, "slice")
        || !strcmpi(basestr, "mongoose")) {
        if (to_plural)
            Strcasecpy(endstring, "s");
        return TRUE;
    }
    /* skip "ox" -> "oxen" entry when pluralizing "<something>ox"
       unless it is muskox */
    if (to_plural && baselen > 2 && !strcmpi(endstring - 2, "ox")
        && !(baselen > 5 && !strcmpi(endstring - 6, "muskox"))) {
        /* "fox" -> "foxes" */
        Strcasecpy(endstring, "es");
        return TRUE;
    }
    if (to_plural) {
        if (baselen > 2 && !strcmpi(endstring - 3, "man")
            && badman(basestr, to_plural)) {
            Strcasecpy(endstring, "s");
            return TRUE;
        }
    } else {
        if (baselen > 2 && !strcmpi(endstring - 3, "men")
            && badman(basestr, to_plural))
            return TRUE;
    }
    for (sp = one_off; sp->sing; sp++) {
        /* check whether endstring already matches */
        same = to_plural ? sp->plur : sp->sing;
        al = (int) strlen(same);
        if (!BSTRCMPI(basestr, endstring - al, same))
            return TRUE; /* use as-is */
        /* check whether it matches the inverse; if so, transform it */
        other = to_plural ? sp->sing : sp->plur;
        al = (int) strlen(other);
        if (!BSTRCMPI(basestr, endstring - al, other)) {
            Strcasecpy(endstring - al, same);
            return TRUE; /* one_off[] transformation */
        }
    }
    return FALSE;
}

/* searches for common compounds, ex. lump of royal jelly */
STATIC_OVL char *
singplur_compound(str)
char *str;
{
    /* if new entries are added, be sure to keep compound_start[] in sync */
    static const char *const compounds[] =
        {
          " of ",     " labeled ", " called ", " entitled ",
          " named ",  " above", /* lurkers above */
          " versus ", " from ",    " in ",
          " on ",     " a la ",    " with", /* " with "? */
          " de ",     " d'",       " du ", " against ",
          "-in-",     "-at-",      0
        }, /* list of first characters for all compounds[] entries */
        compound_start[] = " -";

    const char *const *cmpd;
    char *p;

    for (p = str; *p; ++p) {
        /* substring starting at p can only match if *p is found
           within compound_start[] */
        if (!index(compound_start, *p))
            continue;

        /* check current substring against all words in the compound[] list */
        for (cmpd = compounds; *cmpd; ++cmpd)
            if (!strncmpi(p, *cmpd, (int) strlen(*cmpd)))
                return p;
    }
    /* wasn't recognized as a compound phrase */
    return 0;
}

/* Plural routine; once upon a time it may have been chiefly used for
 * user-defined fruits, but it is now used extensively throughout the
 * program.
 *
 * For fruit, we have to try to account for everything reasonable the
 * player has; something unreasonable can still break the code.
 * However, it's still a lot more accurate than "just add an 's' at the
 * end", which Rogue uses...
 *
 * Also used for plural monster names ("Wiped out all homunculi." or the
 * vanquished monsters list) and body parts.  A lot of unique monsters have
 * names which get mangled by makeplural and/or makesingular.  They're not
 * genocidable, and vanquished-mon handling does its own special casing
 * (for uniques who've been revived and re-killed), so we don't bother
 * trying to get those right here.
 *
 * Also misused by muse.c to convert 1st person present verbs to 2nd person.
 * 3.6.0: made case-insensitive.
 */
char *
makeplural(oldstr)
const char *oldstr;
{
    register char *spot;
    char lo_c, *str = nextobuf();
    const char *excess = (char *) 0;
    size_t len;

    if (oldstr)
        while (*oldstr == ' ')
            oldstr++;
    if (!oldstr || !*oldstr) {
        impossible("plural of null?");
        Strcpy(str, "s");
        return str;
    }
    Strcpy(str, oldstr);

    /*
     * Skip changing "pair of" to "pairs of".  According to Webster, usual
     * English usage is use pairs for humans, e.g. 3 pairs of dancers,
     * and pair for objects and non-humans, e.g. 3 pair of boots.  We don't
     * refer to pairs of humans in this game so just skip to the bottom.
     */
    if (!strncmpi(str, "pair of ", 8))
        goto bottom;

    /* look for "foo of bar" so that we can focus on "foo" */
    if ((spot = singplur_compound(str)) != 0) {
        excess = oldstr + (int) (spot - str);
        *spot = '\0';
    } else
        spot = eos(str);

    spot--;
    while (spot > str && *spot == ' ')
        spot--; /* Strip blanks from end */
    *(spot + 1) = '\0';
    /* Now spot is the last character of the string */

    len = strlen(str);

    /* Single letters */
    if (len == 1 || !letter(*spot)) {
        Strcpy(spot + 1, "'s");
        goto bottom;
    }

    /* dispense with some words which don't need pluralization */
    {
        static const char *const already_plural[] = {
            "ae",  /* algae, larvae, &c */
            "matzot", 0,
        };

        /* spot+1: synch up with makesingular's usage */
        if (singplur_lookup(str, spot + 1, TRUE, already_plural))
            goto bottom;

        /* more of same, but not suitable for blanket loop checking */
        if ((len == 2 && !strcmpi(str, "ya"))
            || (len >= 3 && !strcmpi(spot - 2, " ya")))
            goto bottom;
    }

    /* man/men ("Wiped out all cavemen.") */
    if (len >= 3 && !strcmpi(spot - 2, "man")
        /* exclude shamans and humans etc */
        && !badman(str, TRUE)) {
        Strcasecpy(spot - 1, "en");
        goto bottom;
    }
    if (lowc(*spot) == 'f') { /* (staff handled via one_off[]) */
        lo_c = lowc(*(spot - 1));
        if (len >= 3 && !strcmpi(spot - 2, "erf")) {
            /* avoid "nerf" -> "nerves", "serf" -> "serves" */
            ; /* fall through to default (append 's') */
        } else if (index("lr", lo_c) || index(vowels, lo_c)) {
            /* [aeioulr]f to [aeioulr]ves */
            Strcasecpy(spot, "ves");
            goto bottom;
        }
    }
    /* ium/ia (mycelia, baluchitheria) */
    if (len >= 3 && !strcmpi(spot - 2, "ium")) {
        Strcasecpy(spot - 2, "ia");
        goto bottom;
    }
    /* algae, larvae, hyphae (another fungus part) */
    if ((len >= 4 && !strcmpi(spot - 3, "alga"))
        || (len >= 5
            && (!strcmpi(spot - 4, "hypha") || !strcmpi(spot - 4, "larva")))
        || (len >= 6 && !strcmpi(spot - 5, "amoeba"))
        || (len >= 8 && (!strcmpi(spot - 7, "vertebra")))) {
        /* a to ae */
        Strcasecpy(spot + 1, "e");
        goto bottom;
    }
    /* fungus/fungi, homunculus/homunculi, but buses, lotuses, wumpuses */
    if (len > 3 && !strcmpi(spot - 1, "us")
        && !((len >= 5 && !strcmpi(spot - 4, "lotus"))
             || (len >= 6 && !strcmpi(spot - 5, "wumpus")))) {
        Strcasecpy(spot - 1, "i");
        goto bottom;
    }
    /* sis/ses (nemesis) */
    if (len >= 3 && !strcmpi(spot - 2, "sis")) {
        Strcasecpy(spot - 1, "es");
        goto bottom;
    }
    /* matzoh/matzot, possible food name */
    if (len >= 6
        && (!strcmpi(spot - 5, "matzoh") || !strcmpi(spot - 5, "matzah"))) {
        Strcasecpy(spot - 1, "ot"); /* oh/ah -> ot */
        goto bottom;
    }
    if (len >= 5
        && (!strcmpi(spot - 4, "matzo") || !strcmpi(spot - 4, "matza"))) {
        Strcasecpy(spot, "ot"); /* o/a -> ot */
        goto bottom;
    }

    /* note: -eau/-eaux (gateau, bordeau...) */
    /* note: ox/oxen, VAX/VAXen, goose/geese */

    lo_c = lowc(*spot);

    /* Ends in z, x, s, ch, sh; add an "es" */
    if (index("zxs", lo_c)
        || (len >= 2 && lo_c == 'h' && index("cs", lowc(*(spot - 1))) 
            /* Unlike the base rule, words "patriarch" and "matriarch" do not end with "es" */
            && !(len >= 5 && !strcmpi(spot - 4, "iarch")))
        /* Kludge to get "tomatoes" and "potatoes" right */
        || (len >= 4 && !strcmpi(spot - 2, "ato"))
        || (len >= 5 && !strcmpi(spot - 4, "dingo"))) {
        Strcasecpy(spot + 1, "es"); /* append es */
        goto bottom;
    }
    /* Ends in y preceded by consonant (note: also "qu") change to "ies" */
    if (lo_c == 'y' && !index(vowels, lowc(*(spot - 1)))) {
        Strcasecpy(spot, "ies"); /* y -> ies */
        goto bottom;
    }
    /* Default: append an 's' */
    Strcasecpy(spot + 1, "s");

 bottom:
    if (excess)
        Strcat(str, excess);
    return str;
}

/*
 * Singularize a string the user typed in; this helps reduce the complexity
 * of readobjnam, and is also used in pager.c to singularize the string
 * for which help is sought.
 *
 * "Manes" is ambiguous: monster type (keep s), or horse body part (drop s)?
 * Its inclusion in as_is[]/special_subj[] makes it get treated as the former.
 *
 * A lot of unique monsters have names ending in s; plural, or singular
 * from plural, doesn't make much sense for them so we don't bother trying.
 * 3.6.0: made case-insensitive.
 */
char *
makesingular(oldstr)
const char *oldstr;
{
    register char *p, *bp;
    const char *excess = 0;
    char *str = nextobuf();

    if (oldstr)
        while (*oldstr == ' ')
            oldstr++;
    if (!oldstr || !*oldstr) {
        impossible("singular of null?");
        str[0] = '\0';
        return str;
    }

    bp = strcpy(str, oldstr);

    /* check for "foo of bar" so that we can focus on "foo" */
    if ((p = singplur_compound(bp)) != 0) {
        excess = oldstr + (int) (p - bp);
        *p = '\0';
    } else
        p = eos(bp);

    /* dispense with some words which don't need singularization */
    if (singplur_lookup(bp, p, FALSE, special_subjs))
        goto bottom;

    /* remove -s or -es (boxes) or -ies (rubies) */
    if (p >= bp + 1 && lowc(p[-1]) == 's') {
        if (p >= bp + 2 && lowc(p[-2]) == 'e') {
            if (p >= bp + 3 && lowc(p[-3]) == 'i') { /* "ies" */
                if (!BSTRCMPI(bp, p - 7, "cookies")
                    || (!BSTRCMPI(bp, p - 4, "pies")
                        /* avoid false match for "harpies" */
                        && (p - 4 == bp || p[-5] == ' '))
                    /* alternate djinni/djinn spelling; not really needed */
                    || (!BSTRCMPI(bp, p - 6, "genies")
                        /* avoid false match for "progenies" */
                        && (p - 6 == bp || p[-7] == ' '))
                    || !BSTRCMPI(bp, p - 5, "mbies") /* zombie */
                    || !BSTRCMPI(bp, p - 5, "yries")) /* valkyrie */
                    goto mins;
                Strcasecpy(p - 3, "y"); /* ies -> y */
                goto bottom;
            }
            /* wolves, but f to ves isn't fully reversible */
            if (p - 4 >= bp && (index("lr", lowc(*(p - 4)))
                                || index(vowels, lowc(*(p - 4))))
                && !BSTRCMPI(bp, p - 3, "ves")) {
                if (!BSTRCMPI(bp, p - 6, "cloves")
                    || !BSTRCMPI(bp, p - 6, "nerves"))
                    goto mins;
                Strcasecpy(p - 3, "f"); /* ves -> f */
                goto bottom;
            }
            /* note: nurses, axes but boxes, wumpuses */
            if (!BSTRCMPI(bp, p - 4, "eses")
                || !BSTRCMPI(bp, p - 4, "oxes") /* boxes, foxes */
                || !BSTRCMPI(bp, p - 4, "nxes") /* lynxes */
                || !BSTRCMPI(bp, p - 4, "ches")
                || !BSTRCMPI(bp, p - 4, "uses") /* lotuses */
                || !BSTRCMPI(bp, p - 4, "sses") /* priestesses */
                || !BSTRCMPI(bp, p - 5, "atoes") /* tomatoes */
                || !BSTRCMPI(bp, p - 7, "dingoes")
                || !BSTRCMPI(bp, p - 7, "Aleaxes")) {
                *(p - 2) = '\0'; /* drop es */
                goto bottom;
            } /* else fall through to mins */

            /* ends in 's' but not 'es' */
        } else if (!BSTRCMPI(bp, p - 2, "us")) { /* lotus, fungus... */
            if (BSTRCMPI(bp, p - 6, "tengus") /* but not these... */
                && BSTRCMPI(bp, p - 7, "hezrous"))
                goto bottom;
        } else if (!BSTRCMPI(bp, p - 2, "ss")
                   || !BSTRCMPI(bp, p - 5, " lens")
                   || (p - 4 == bp && !strcmpi(p - 4, "lens"))) {
            goto bottom;
        }
 mins:
        *(p - 1) = '\0'; /* drop s */

    } else { /* input doesn't end in 's' */

        if (!BSTRCMPI(bp, p - 3, "men")
            && !badman(bp, FALSE)) {
            Strcasecpy(p - 2, "an");
            goto bottom;
        }
        /* matzot -> matzo, algae -> alga */
        if (!BSTRCMPI(bp, p - 6, "matzot") || !BSTRCMPI(bp, p - 2, "ae")) {
            *(p - 1) = '\0'; /* drop t/e */
            goto bottom;
        }
        /* balactheria -> balactherium */
        if (p - 4 >= bp && !strcmpi(p - 2, "ia")
            && index("lr", lowc(*(p - 3))) && lowc(*(p - 4)) == 'e') {
            Strcasecpy(p - 1, "um"); /* a -> um */
        }

        /* here we cannot find the plural suffix */
    }

 bottom:
    /* if we stripped off a suffix (" of bar" from "foo of bar"),
       put it back now [strcat() isn't actually 100% safe here...] */
    if (excess)
        Strcat(bp, excess);

    return bp;
}

boolean
badman(basestr, to_plural)
const char *basestr;
boolean to_plural;            /* true => makeplural, false => makesingular */
{
    /* these are all the prefixes for *man that don't have a *men plural */
    static const char *no_men[] = {
        "albu", "antihu", "anti", "ata", "auto", "bildungsro", "cai", "cay",
        "ceru", "corner", "decu", "des", "dura", "fir", "hanu", "het",
        "infrahu", "inhu", "nonhu", "otto", "out", "prehu", "protohu",
        "subhu", "superhu", "talis", "unhu", "sha",
        "hu", "un", "le", "re", "so", "to", "at", "a",
    };
    /* these are all the prefixes for *men that don't have a *man singular */
    static const char *no_man[] = {
        "abdo", "acu", "agno", "ceru", "cogno", "cycla", "fleh", "grava",
        "hegu", "preno", "sonar", "speci", "dai", "exa", "fla", "sta", "teg",
        "tegu", "vela", "da", "hy", "lu", "no", "nu", "ra", "ru", "se", "vi",
        "ya", "o", "a",
    };
    int i;
    size_t al;
    const char *endstr, *spot;

    if (!basestr || strlen(basestr) < 4)
        return FALSE;

    endstr = eos((char *) basestr);

    if (to_plural) {
        for (i = 0; i < SIZE(no_men); i++) {
            al = strlen(no_men[i]);
            spot = endstr - (al + 3);
            if (!BSTRNCMPI(basestr, spot, no_men[i], al)
                && (spot == basestr || *(spot - 1) == ' '))
                return TRUE;
        }
    } else {
        for (i = 0; i < SIZE(no_man); i++) {
            al = strlen(no_man[i]);
            spot = endstr - (al + 3);
            if (!BSTRNCMPI(basestr, spot, no_man[i], al)
                && (spot == basestr || *(spot - 1) == ' '))
                return TRUE;
        }
    }
    return FALSE;
}

/* compare user string against object name string using fuzzy matching */
STATIC_OVL boolean
wishymatch(u_str, o_str, retry_inverted)
const char *u_str;      /* from user, so might be variant spelling */
const char *o_str;      /* from objects[], so is in canonical form */
boolean retry_inverted; /* optional extra "of" handling */
{
    STATIC_VAR NEARDATA const char detect_SP[] = "detect ",
                               SP_detection[] = " detection";
    char *p, buf[BUFSZ];

    /* ignore spaces & hyphens and upper/lower case when comparing */
    if (fuzzymatch(u_str, o_str, " -", TRUE))
        return TRUE;

    if (retry_inverted) {
        const char *u_of, *o_of;

        /* when just one of the strings is in the form "foo of bar",
           convert it into "bar foo" and perform another comparison */
        u_of = strstri(u_str, " of ");
        o_of = strstri(o_str, " of ");
        if (u_of && !o_of) {
            Strcpy(buf, u_of + 4);
            p = eos(strcat(buf, " "));
            while (u_str < u_of)
                *p++ = *u_str++;
            *p = '\0';
            return fuzzymatch(buf, o_str, " -", TRUE);
        } else if (o_of && !u_of) {
            Strcpy(buf, o_of + 4);
            p = eos(strcat(buf, " "));
            while (o_str < o_of)
                *p++ = *o_str++;
            *p = '\0';
            return fuzzymatch(u_str, buf, " -", TRUE);
        }
    }

    /* [note: if something like "elven speed boots" ever gets added, these
       special cases should be changed to call wishymatch() recursively in
       order to get the "of" inversion handling] */
    if (!strncmp(o_str, "dwarvish ", 9)) {
        if (!strncmpi(u_str, "dwarven ", 8))
            return fuzzymatch(u_str + 8, o_str + 9, " -", TRUE);
    } else if (!strncmp(o_str, "elven ", 6)) {
        if (!strncmpi(u_str, "elvish ", 7))
            return fuzzymatch(u_str + 7, o_str + 6, " -", TRUE);
        else if (!strncmpi(u_str, "elfin ", 6))
            return fuzzymatch(u_str + 6, o_str + 6, " -", TRUE);
    } else if (!strncmp(o_str, detect_SP, sizeof detect_SP - 1)) {
        /* check for "detect <foo>" vs "<foo> detection" */
        if ((p = strstri(u_str, SP_detection)) != 0
            && !*(p + sizeof SP_detection - 1)) {
            /* convert "<foo> detection" into "detect <foo>" */
            *p = '\0';
            Strcat(strcpy(buf, detect_SP), u_str);
            /* "detect monster" -> "detect monsters" */
            if (!strcmpi(u_str, "monster"))
                Strcat(buf, "s");
            *p = ' ';
            return fuzzymatch(buf, o_str, " -", TRUE);
        }
    } else if (strstri(o_str, SP_detection)) {
        /* and the inverse, "<foo> detection" vs "detect <foo>" */
        if (!strncmpi(u_str, detect_SP, sizeof detect_SP - 1)) {
            /* convert "detect <foo>s" into "<foo> detection" */
            p = makesingular(u_str + sizeof detect_SP - 1);
            Strcat(strcpy(buf, p), SP_detection);
            /* caller may be looping through objects[], so avoid
               churning through all the obufs */
            releaseobuf(p);
            return fuzzymatch(buf, o_str, " -", TRUE);
        }
    } else if (strstri(o_str, "ability")) {
        /* when presented with "foo of bar", makesingular() used to
           singularize both foo & bar, but now only does so for foo */
        /* catch "{potion(s),ring} of {gain,restore,sustain} abilities" */
        if ((p = strstri(u_str, "abilities")) != 0
            && !*(p + sizeof "abilities" - 1)) {
            (void) strncpy(buf, u_str, (size_t) (p - u_str));
            Strcpy(buf + (p - u_str), "ability");
            return fuzzymatch(buf, o_str, " -", TRUE);
        }
    } else if (!strcmp(o_str, "aluminum")) {
        /* this special case doesn't really fit anywhere else... */
        /* (note that " wand" will have been stripped off by now) */
        if (!strcmpi(u_str, "aluminium"))
            return fuzzymatch(u_str + 9, o_str + 8, " -", TRUE);
    }

    return FALSE;
}

struct o_range {
    const char *name, oclass;
    int f_o_range, l_o_range;
};

/* wishable subranges of objects */
STATIC_OVL NEARDATA const struct o_range o_ranges[] = {
    { "bag", TOOL_CLASS, LEATHER_BAG, ORIENTAL_SILK_SACK },
    { "lamp", TOOL_CLASS, OIL_LAMP, MAGIC_LAMP },
    { "candle", TOOL_CLASS, TALLOW_CANDLE, MAGIC_CANDLE },
    { "horn", TOOL_CLASS, BRASS_HORN, HORN_OF_PLENTY },
    { "shield", ARMOR_CLASS, SMALL_SHIELD, SHIELD_OF_REFLECTION },
    { "hat", ARMOR_CLASS, FEDORA, DUNCE_CAP },
    { "helm", ARMOR_CLASS, ELVEN_LEATHER_HELM, HELM_OF_TELEPATHY },
    { "crown", ARMOR_CLASS, ROYAL_CROWN, CROWN_OF_RULERSHIP },
    { "conical hat", ARMOR_CLASS, CORNUTHAUM, GNOMISH_FELT_HAT },
    { "gloves", ARMOR_CLASS, LEATHER_GLOVES, GAUNTLETS_OF_DEXTERITY },
    { "gauntlets", ARMOR_CLASS, LEATHER_GLOVES, GAUNTLETS_OF_DEXTERITY },
    { "boots", ARMOR_CLASS, LOW_BOOTS, LEVITATION_BOOTS },
    { "shoes", ARMOR_CLASS, LOW_BOOTS, IRON_SHOES },
    { "cloak", ARMOR_CLASS, ELVEN_CLOAK, CLOAK_OF_DISPLACEMENT },
    { "shirt", ARMOR_CLASS, HAWAIIAN_SHIRT, T_SHIRT },
    { "robe", ARMOR_CLASS, MEDIEVAL_ROBE, MUMMY_WRAPPING },
    { "bracers", ARMOR_CLASS, LEATHER_BRACERS, BRACERS_OF_REFLECTION },
    { "dragon scales", ARMOR_CLASS, GRAY_DRAGON_SCALES, YELLOW_DRAGON_SCALES },
    { "dragon scale mail", ARMOR_CLASS, GRAY_DRAGON_SCALE_MAIL, YELLOW_DRAGON_SCALE_MAIL },
    { "sword", WEAPON_CLASS, SHORT_SWORD, KATANA },
    { "staff", WEAPON_CLASS, QUARTERSTAFF, STAFF_OF_WITHERING },
    { "venom", VENOM_CLASS, BLINDING_VENOM, ACID_VENOM },
    { "reagent", REAGENT_CLASS, THREAD_OF_SPIDER_SILK, FEATHER },
    { "miscellaneous item", MISCELLANEOUS_CLASS, BROOCH_OF_SHIELDING, WINGS_OF_FLYING },
    { "belt", MISCELLANEOUS_CLASS, LEATHER_BELT, BELT_OF_STORM_GIANT_STRENGTH },
    { "nose ring", MISCELLANEOUS_CLASS, NOSE_RING_OF_BULL_STRENGTH, NOSE_RING_OF_CEREBRAL_SAFEGUARDING },
    { "ioun stone", MISCELLANEOUS_CLASS, IOUN_STONE_OF_PROTECTION, IOUN_STONE_OF_SUSTENANCE },
    { "eyeglasses", MISCELLANEOUS_CLASS, LENSES, SUNGLASSES },
    { "goggles", MISCELLANEOUS_CLASS, GOGGLES_OF_NIGHT, GOGGLES_OF_EYE_PROTECTION },
    { "gray stone", GEM_CLASS, LUCKSTONE, FLINT },
    { "grey stone", GEM_CLASS, LUCKSTONE, FLINT },
    { "nugget of ore", GEM_CLASS, NUGGET_OF_IRON_ORE, NUGGET_OF_MITHRIL_ORE },
    { "nugget", GEM_CLASS, NUGGET_OF_IRON_ORE, NUGGET_OF_MITHRIL_ORE },
    { "ore", GEM_CLASS, NUGGET_OF_IRON_ORE, NUGGET_OF_MITHRIL_ORE },
    { "mushroom", FOOD_CLASS, CHAMPIGNON, ORACULAR_TOADSTOOL },
    { "whistle", TOOL_CLASS, TIN_WHISTLE, MAGIC_WHISTLE },
    { "flute", TOOL_CLASS, WOODEN_FLUTE, MAGIC_FLUTE },
    { "harp", TOOL_CLASS, WOODEN_HARP, MAGIC_HARP },
    { "drum", TOOL_CLASS, LEATHER_DRUM, DRUM_OF_EARTHQUAKE },
    { "jar", TOOL_CLASS, JAR_OF_EXTRA_HEALING_SALVE, JAR_OF_BASILISK_BLOOD },
    { "salve", TOOL_CLASS, JAR_OF_EXTRA_HEALING_SALVE, JAR_OF_PRODIGIOUS_HEALING_SALVE },
};

/* alternate spellings; if the difference is only the presence or
   absence of spaces and/or hyphens (such as "pickaxe" vs "pick axe"
   vs "pick-axe") then there is no need for inclusion in this list;
   likewise for ``"of" inversions'' ("boots of speed" vs "speed boots") */
STATIC_VAR const struct alt_spellings {
    const char *sp;
    int ob;
} spellings[] = {
    { "pickax", PICK_AXE },
    { "whip", BULLWHIP },
    { "saber", SILVER_SABER },
    { "silver sabre", SILVER_SABER },
    { "double flail", DOUBLE_HEADED_FLAIL },
    { "two-headed flail", DOUBLE_HEADED_FLAIL },
    { "two headed flail", DOUBLE_HEADED_FLAIL },
    { "double headed flail", DOUBLE_HEADED_FLAIL },
    { "triple flail", TRIPLE_HEADED_FLAIL },
    { "three-headed flail", TRIPLE_HEADED_FLAIL },
    { "three headed flail", TRIPLE_HEADED_FLAIL },
    { "triple headed flail", TRIPLE_HEADED_FLAIL },
    { "quiver of endless arrows", QUIVER_OF_INFINITE_ARROWS },
    { "pouch of infinite bolts", POUCH_OF_ENDLESS_BOLTS },
    { "spellbook of call demogorgon", SPE_CALL_DEMOGORGON },
    { "spellbook of summon demogorgon", SPE_CALL_DEMOGORGON },
    { "spellbook of summon Demogorgon", SPE_CALL_DEMOGORGON },
    { "spellbook of yendorian great summoning", SPE_GREAT_YENDORIAN_SUMMONING },
    { "potion of super heroism", POT_SUPER_HEROISM },
    { "potion of superheroism", POT_SUPER_HEROISM },
    { "smooth shield", SHIELD_OF_REFLECTION },
    { "gauntlets of power", GAUNTLETS_OF_OGRE_POWER },
    { "helmet of telepathy", HELM_OF_TELEPATHY },
    { "helmet of opposite alignment", HELM_OF_OPPOSITE_ALIGNMENT },
    { "helmet of brilliance", HELM_OF_BRILLIANCE },
    { "grey dragon scale mail", GRAY_DRAGON_SCALE_MAIL },
    { "grey dragon scales", GRAY_DRAGON_SCALES },
    { "iron ball", HEAVY_IRON_BALL },
    { "lantern", BRASS_LANTERN },
    { "mattock", DWARVISH_MATTOCK },
    { "longbow", LONG_BOW },
    { "composite longbow", COMPOSITE_LONG_BOW },
    { "elven longbow", ELVEN_LONG_BOW },
    { "elven bow", ELVEN_LONG_BOW },
    { "shortbow", SHORT_BOW },
    { "composite shortbow", COMPOSITE_SHORT_BOW },
    { "orcish shortbow", ORCISH_SHORT_BOW },
    { "orcish bow", ORCISH_SHORT_BOW },
    { "amulet of poison resistance", AMULET_VERSUS_POISON },
    { "potion of sleep", POT_SLEEPING },
    { "potion of mana", POT_GAIN_ENERGY },
    { "potion of extra mana", POT_EXTRA_ENERGY },
    { "potion of greater mana", POT_GREATER_ENERGY },
    { "potion of full mana", POT_FULL_ENERGY },
    { "stone", STONE_PEBBLE },
    { "clay", CLAY_PEBBLE },
    { "sling-bullet", SLING_BULLET },
    { "sling bullet", SLING_BULLET },
    { "sling bullet", SLING_BULLET },
    { "quarrel", CROSSBOW_BOLT },
    { "gnollish crossbow bolt", GNOLLISH_QUARREL },
    { "camera", EXPENSIVE_CAMERA },
    { "tee shirt", T_SHIRT },
    { "can", TIN },
    { "can opener", TIN_OPENER },
    { "kelp", KELP_FROND },
    { "eucalyptus", EUCALYPTUS_LEAF },
    { "royal jelly", LUMP_OF_ROYAL_JELLY },
    { "lembas", ELVEN_WAYBREAD },
    { "cookie", FORTUNE_COOKIE },
    { "pie", CREAM_PIE },
    { "marker", MAGIC_MARKER },
    { "hook", GRAPPLING_HOOK },
    { "grappling iron", GRAPPLING_HOOK },
    { "grapnel", GRAPPLING_HOOK },
    { "grapple", GRAPPLING_HOOK },
    { "spider silk", THREAD_OF_SPIDER_SILK },
    { "silk", THREAD_OF_SPIDER_SILK },
    { "bat guano", CLUMP_OF_BAT_GUANO },
    { "guano", CLUMP_OF_BAT_GUANO },
    { "spore", HEAP_OF_SPORAL_POWDER },
    { "fungal spore", HEAP_OF_SPORAL_POWDER },
    { "sporal powder", HEAP_OF_SPORAL_POWDER },
    { "powder", HEAP_OF_SPORAL_POWDER },
    { "wood", PIECE_OF_WOOD },
    { "skull", HUMAN_SKULL },
    { "ginseng", GINSENG_ROOT },
    { "mandrake", MANDRAKE_ROOT },
    { "wolfsbane", SPRIG_OF_WOLFSBANE },
    { "garlic", CLOVE_OF_GARLIC },
    { "sulfurous ash", PINCH_OF_SULFUROUS_ASH },
    { "sulphurous ash", PINCH_OF_SULFUROUS_ASH },
    { "iron ore", NUGGET_OF_IRON_ORE },
    { "copper ore", NUGGET_OF_COPPER_ORE },
    { "silver ore", NUGGET_OF_SILVER_ORE },
    { "gold ore", NUGGET_OF_GOLD_ORE },
    { "platinum ore", NUGGET_OF_PLATINUM_ORE },
    { "orichalcum ore", NUGGET_OF_ORICHALCUM_ORE },
    { "orihalcon ore", NUGGET_OF_ORICHALCUM_ORE },
    { "nugget of orihacon ore", NUGGET_OF_ORICHALCUM_ORE },
    { "mithril ore", NUGGET_OF_MITHRIL_ORE },
    { "adamantium ore", NUGGET_OF_ADAMANTIUM_ORE },
    { "ash", PINCH_OF_SULFUROUS_ASH },
    { "spinach", HANDFUL_OF_SPINACH_LEAVES },
    { "ring of death resistance", RIN_PROTECTION_FROM_UNDEATH },
    { "ring of drain resistance", RIN_PROTECTION_FROM_UNDEATH },
    { "ring of protection from death", RIN_PROTECTION_FROM_UNDEATH },
    { "ring of protection from shape shifters", RIN_PROTECTION_FROM_SHAPE_CHANGERS },
    { "belt of giant strength", BELT_OF_HILL_GIANT_STRENGTH },
    { "girdle of giant strength", BELT_OF_HILL_GIANT_STRENGTH },
    { "candelabrum", LARGE_FIVE_BRANCHED_CANDELABRUM },
    { "large candelabrum", LARGE_FIVE_BRANCHED_CANDELABRUM },
    /* if we ever add other sizes, move this to o_ranges[] with "bag" */
    { "box", LARGE_BOX },
    /* normally we wouldn't have to worry about unnecessary <space>, but
       " stone" will get stripped off, preventing a wishymatch; that actually
       lets "flint stone" be a match, so we also accept bogus "flintstone" */
    { "luck stone", LUCKSTONE },
    { "load stone", LOADSTONE },
    { "touch stone", TOUCHSTONE },
    { "flintstone", FLINT },
    { "salve", JAR_OF_EXTRA_HEALING_SALVE },
    { "healing salve", JAR_OF_EXTRA_HEALING_SALVE },
    { "extra healing salve", JAR_OF_EXTRA_HEALING_SALVE },
    { "greater healing salve", JAR_OF_GREATER_HEALING_SALVE },
    { "prodigious healing salve", JAR_OF_PRODIGIOUS_HEALING_SALVE },
    { "medicinal salve", JAR_OF_MEDICINAL_SALVE },
    { "medical salve", JAR_OF_MEDICINAL_SALVE },
    { "jar of medical salve", JAR_OF_MEDICINAL_SALVE },
    { "basilisk blood", JAR_OF_BASILISK_BLOOD },
    { "galadhrim bow", GALADHRIM_BOW },
    { "galadhrim boots", GALADHRIM_BOOTS },
    { "bag of gluttony", BAG_OF_THE_GLUTTON },
    { "uruk-hai shield", GREAT_ORCISH_SHIELD },
    { (const char *) 0, 0 },
};

STATIC_OVL short
rnd_otyp_by_wpnskill(skill)
schar skill;
{
    int i, n = 0;
    short otyp = STRANGE_OBJECT;

    for (i = bases[WEAPON_CLASS];
         i < NUM_OBJECTS && objects[i].oc_class == WEAPON_CLASS; i++)
        if (objects[i].oc_skill == skill) {
            n++;
            otyp = i;
        }
    if (n > 0) {
        n = rn2(n);
        for (i = bases[WEAPON_CLASS];
             i < NUM_OBJECTS && objects[i].oc_class == WEAPON_CLASS; i++)
            if (objects[i].oc_skill == skill)
                if (--n < 0)
                    return i;
    }
    return otyp;
}

STATIC_OVL short
rnd_otyp_by_namedesc(name, oclass, xtra_prob)
const char *name;
char oclass;
int xtra_prob; /* to force 0% random generation items to also be considered */
{
    int i, n = 0;
    short validobjs[NUM_OBJECTS];
    register const char *zn;
    int prob, maxprob = 0;

    if (!name || !*name)
        return STRANGE_OBJECT;

    memset((genericptr_t) validobjs, 0, sizeof validobjs);

    /* FIXME:
     * When this spans classes (the !oclass case), the item
     * probabilities are not very useful because they don't take
     * the class generation probability into account.  [If 10%
     * of spellbooks were blank and 1% of scrolls were blank,
     * "blank" would have 10/11 chance to yield a blook even though
     * scrolls are supposed to be much more common than books.]
     */
    for (i = oclass ? bases[(int) oclass] : STRANGE_OBJECT + 1;
         i < NUM_OBJECTS && (!oclass || objects[i].oc_class == oclass);
         ++i) {
        /* don't match extra descriptions (w/o real name) */
        if ((zn = OBJ_NAME(objects[i])) == 0)
            continue;
        if (wishymatch(name, zn, TRUE)
            || ((zn = OBJ_DESCR(objects[i])) != 0
                && wishymatch(name, zn, FALSE))
//            || ((zn = objects[i].oc_uname) != 0
//                && wishymatch(name, zn, FALSE))
            ) 
        {
            validobjs[n++] = (short) i;
            maxprob += (objects[i].oc_prob + xtra_prob);
        }
    }

    if (n > 0 && maxprob) {
        prob = rn2(maxprob);
        for (i = 0; i < n - 1; i++)
            if ((prob -= (objects[validobjs[i]].oc_prob + xtra_prob)) < 0)
                break;
        return validobjs[i];
    }
    return STRANGE_OBJECT;
}

int
shiny_obj(oclass)
char oclass;
{
    return (int) rnd_otyp_by_namedesc("shiny", oclass, 0);
}

/*
 * Return something wished for.  Specifying a null pointer for
 * the user request string results in a random object.  Otherwise,
 * if asking explicitly for "nothing" (or "nil") return no_wish;
 * if not an object return &zeroobj; if an error (no matching object),
 * return null.
 */
struct obj *
readobjnam(bp, no_wish, is_wiz_wish)
register char *bp;
struct obj *no_wish;
boolean is_wiz_wish;
{
    register char *p;
    register int i;
    register struct obj *otmp;
    int cnt, enchantment, spesgn, typ, very, rechrg;
    int charges, chargesfound;
    int blessed, uncursed, iscursed, ispoisoned, isgreased;
    int eroded, eroded2, erodeproof, locked, unlocked, broken, open;
    int halfeaten, mntmp, contents;
    int islit, unlabeled, ishistoric, isdiluted, trapped, elemental_enchantment, material, exceptionality, mythic_prefix, mythic_suffix, key_special_quality, key_otyp, is_switchable;
    int tmp, tinv, tvariety;
    int wetness, gsize = 0;
    struct fruit *f;
    int ftype = context.current_fruit;
    char fruitbuf[BUFSZ], globbuf[BUFSZ];
    /* Fruits may not mess up the ability to wish for real objects (since
     * you can leave a fruit in a bones file and it will be added to
     * another person's game), so they must be checked for last, after
     * stripping all the possible prefixes and seeing if there's a real
     * name in there.  So we have to save the full original name.  However,
     * it's still possible to do things like "uncursed burnt Alaska",
     * or worse yet, "2 burned 5 course meals", so we need to loop to
     * strip off the prefixes again, this time stripping only the ones
     * possible on food.
     * We could get even more detailed so as to allow food names with
     * prefixes that _are_ possible on food, so you could wish for
     * "2 3 alarm chilis".  Currently this isn't allowed; options.c
     * automatically sticks 'candied' in front of such names.
     */
    char oclass;
    char *un, *dn, *actualn, *origbp = bp;
    const char *name = 0;
    boolean isartifact = FALSE;
    boolean wiz_wishing = (wizard && is_wiz_wish);

    cnt = enchantment = charges = chargesfound = spesgn = typ = 0;
    very = rechrg = blessed = uncursed = iscursed = ispoisoned = elemental_enchantment = material = exceptionality = mythic_prefix = mythic_suffix =
        isgreased = eroded = eroded2 = erodeproof = halfeaten =
        islit = unlabeled = ishistoric = isdiluted = trapped =
        locked = unlocked = open = broken = key_special_quality = key_otyp = is_switchable = 0;
    tvariety = RANDOM_TIN;
    mntmp = NON_PM;
#define CONTAINER_UNDEFINED 0
#define CONTAINER_EMPTY 1
#define CONTAINER_SPINACH 2
    contents = CONTAINER_UNDEFINED;
    oclass = 0;
    actualn = dn = un = 0;
    wetness = 0;

    if (!bp)
        goto any;
    /* first, remove extra whitespace they may have typed */
    (void) mungspaces(bp);
    /* allow wishing for "nothing" to preserve wishless conduct...
       [now requires "wand of nothing" if that's what was really wanted] */
    if (!strcmpi(bp, "nothing") || !strcmpi(bp, "nil")
        || !strcmpi(bp, "none"))
        return no_wish;
    /* save the [nearly] unmodified choice string */
    Strcpy(fruitbuf, bp);



    int foundkey = FALSE;
    for (;;) 
    {
        register int l;

        if (!bp || !*bp)
            goto any;
        if (!strncmpi(bp, "an ", l = 3) || !strncmpi(bp, "a ", l = 2)) {
            cnt = 1;
        } else if (!strncmpi(bp, "the ", l = 4)) {
            ; /* just increment `bp' by `l' below */
        } else if (!cnt && digit(*bp) && strcmp(bp, "0")) {
            cnt = atoi(bp);
            while (digit(*bp))
                bp++;
            while (*bp == ' ')
                bp++;
            l = 0;
        } else if (*bp == '+' || *bp == '-') {
            spesgn = (*bp++ == '+') ? 1 : -1;
            enchantment = atoi(bp);
            while (digit(*bp))
                bp++;
            while (*bp == ' ')
                bp++;
            l = 0;
        } else if (!strncmpi(bp, "blessed ", l = 8)
                   || !strncmpi(bp, "holy water", l = 9)) {
            blessed = 1;
        } else if (!strncmpi(bp, "moist ", l = 6)
                   || !strncmpi(bp, "wet ", l = 4)) {
            if (!strncmpi(bp, "wet ", 4))
                wetness = rn2(3) + 3;
            else
                wetness = rnd(2);
        } else if (!strncmpi(bp, "cursed ", l = 7)
                   || !strncmpi(bp, "unholy water ", l = 12)) {
            iscursed = 1;
        } else if (!strncmpi(bp, "uncursed ", l = 9)) {
            uncursed = 1;
        } else if (!strncmpi(bp, "rustproof ", l = 10)
                   || !strncmpi(bp, "erodeproof ", l = 11)
                   || !strncmpi(bp, "corrodeproof ", l = 13)
                   || !strncmpi(bp, "fixed ", l = 6)
                   || !strncmpi(bp, "fireproof ", l = 10)
                   || !strncmpi(bp, "rotproof ", l = 9)) {
            erodeproof = 1;
        } else if (!strncmpi(bp, "lit ", l = 4)
                   || !strncmpi(bp, "burning ", l = 8)) {
            islit = 1;
        } else if (!strncmpi(bp, "unlit ", l = 6)
                   || !strncmpi(bp, "extinguished ", l = 13)) {
            islit = 0;
            /* "unlabeled" and "blank" are synonymous */
        } else if (!strncmpi(bp, "unlabeled ", l = 10)
                   || !strncmpi(bp, "unlabelled ", l = 11)
                   || !strncmpi(bp, "blank ", l = 6)) {
            unlabeled = 1;
        } else if (!strncmpi(bp, "poisoned ", l = 9)) {
            ispoisoned = 1;
            /* "trapped" recognized but not honored outside wizard mode */
        }
        else if (!strncmpi(bp, "freezing ", l = 9))
        {
            elemental_enchantment = COLD_ENCHANTMENT;
        }
        else if (!strncmpi(bp, "flaming ", l = 8))
        {
            elemental_enchantment = FIRE_ENCHANTMENT;
        }
        else if (!strncmpi(bp, "electrified ", l = 12))
        {
            elemental_enchantment = LIGHTNING_ENCHANTMENT;
        }
        else if (!strncmpi(bp, "death-magical ", l = 14))
        {
            elemental_enchantment = DEATH_ENCHANTMENT;
        }
        else if (!strncmpi(bp, "exceptional ", l = 12))
        {
            exceptionality = EXCEPTIONALITY_EXCEPTIONAL;
        }
        else if (!strncmpi(bp, "elite ", l = 6))
        {
            exceptionality = EXCEPTIONALITY_ELITE;
        }
        else if (!strncmpi(bp, "celestial ", l = 10))
        {
            exceptionality = EXCEPTIONALITY_CELESTIAL;
        }
        else if (!strncmpi(bp, "primordial ", l = 11))
        {
            exceptionality = EXCEPTIONALITY_PRIMORDIAL;
        }
        else if (!strncmpi(bp, "infernal ", l = 9))
        {
            exceptionality = EXCEPTIONALITY_INFERNAL;
        }
        else if (!strncmpi(bp, "legendary ", l = 10))
        {
            mythic_prefix = -2;
            mythic_suffix = -2;
        }
        else if (!strncmpi(bp, "mythic ", l = 7))
        {
            mythic_prefix = -1;
            mythic_suffix = -1;
        }
        else if (!strncmpi(bp, "trapped ", l = 8)) {
            trapped = 0; /* undo any previous "untrapped" */
            if (wiz_wishing)
                trapped = 1;
        } else if (!strncmpi(bp, "untrapped ", l = 10)) {
            trapped = 2; /* not trapped */
        /* locked, unlocked, broken: box/chest lock states */
        } else if (!strncmpi(bp, "locked ", l = 7)) {
            locked = 1, unlocked = broken = 0;
        } else if (!strncmpi(bp, "unlocked ", l = 9)) {
            unlocked = 1, locked = broken = 0;
        } else if (!strncmpi(bp, "open ", l = 5)) {
            open = unlocked = 1, locked = broken = 0;
        } else if (!strncmpi(bp, "broken ", l = 7)) {
            broken = 1, locked = unlocked = 0;
        } else if (!strncmpi(bp, "greased ", l = 8)) {
            isgreased = 1;
        } else if (!strncmpi(bp, "very ", l = 5)) {
            /* very rusted very heavy iron ball */
            very = 1;
        } else if (!strncmpi(bp, "thoroughly ", l = 11)) {
            very = 2;
        } else if (!strncmpi(bp, "rusty ", l = 6)
                   || !strncmpi(bp, "rusted ", l = 7)
                   || !strncmpi(bp, "burnt ", l = 6)
                   || !strncmpi(bp, "burned ", l = 7)) {
            eroded = 1 + very;
            very = 0;
        } else if (!strncmpi(bp, "corroded ", l = 9)
                   || !strncmpi(bp, "rotted ", l = 7)) {
            eroded2 = 1 + very;
            very = 0;
        } else if (!strncmpi(bp, "partly eaten ", l = 13)
                   || !strncmpi(bp, "partially eaten ", l = 16)) {
            halfeaten = 1;
        } else if (!strncmpi(bp, "historic ", l = 9)) {
            ishistoric = 1;
        } else if (!strncmpi(bp, "switchable ", l = 11)) {
            is_switchable = 1;
        } else if (!strncmpi(bp, "diluted ", l = 8)) {
            isdiluted = 1;
        } else if (!strncmpi(bp, "empty ", l = 6)) {
            contents = CONTAINER_EMPTY;
        } else if (!strncmpi(bp, "small ", l = 6)) { /* glob sizes */
            /* "small" might be part of monster name (mimic, if wishing
               for its corpse) rather than prefix for glob size; when
               used for globs, it might be either "small glob of <foo>" or
               "small <foo> glob" and user might add 's' even though plural
               doesn't accomplish anything because globs don't stack */
            if (strncmpi(bp + l, "glob", 4) && !strstri(bp + l, " glob"))
                break;
            gsize = 1;
        } else if (!strncmpi(bp, "medium ", l = 7)) {
            /* xname() doesn't display "medium" but without this
               there'd be no way to ask for the intermediate size
               ("glob" without size prefix yields smallest one) */
            gsize = 2;
        } else if (!strncmpi(bp, "large ", l = 6)) {
            /* "large" might be part of monster name (dog, cat, koboold,
               mimic) or object name (box, round shield) rather than
               prefix for glob size */
            if (strncmpi(bp + l, "glob", 4) && !strstri(bp + l, " glob"))
                break;
            /* "very large " had "very " peeled off on previous iteration */
            gsize = (very != 1) ? 3 : 4;
        }
        else
        {
            boolean anythingfound = FALSE;
            int mythic_idx;
            for (mythic_idx = 1; mythic_idx < MAX_MYTHIC_PREFIXES; mythic_idx++)
            {
                if (!strncmpi(bp, mythic_prefix_qualities[mythic_idx].mythic_affix, l = (int)strlen(mythic_prefix_qualities[mythic_idx].mythic_affix)))
                {
                    mythic_prefix = mythic_idx;
                    anythingfound = TRUE;
                    break;
                }
            }

            if (!anythingfound)
            {
                foundkey = find_key_otyp_by_description(bp, &key_otyp, &key_special_quality);
                break;
            }
        }
        bp += l;
    }
    if (!cnt)
        cnt = 1; /* will be changed to 2 if makesingular() changes string */

    if (foundkey)
    {
        typ = key_otyp;
        goto typfnd;
    }

    if (strlen(bp) > 1 && (p = rindex(bp, '(')) != 0) {
        boolean keeptrailingchars = TRUE;

        p[(p > bp && p[-1] == ' ') ? -1 : 0] = '\0'; /*terminate bp */
        ++p; /* advance past '(' */
        if (!strncmpi(p, "lit)", 4)) {
            islit = 1;
            p += 4 - 1; /* point at ')' */
        } else {
            charges = atoi(p);
            while (digit(*p))
                p++;
            if (*p == ':') {
                p++;
                rechrg = charges;
                charges = atoi(p);
                while (digit(*p))
                    p++;
            }
            if (*p != ')') {
                charges = rechrg = 0;
                /* mis-matched parentheses; rest of string will be ignored
                 * [probably we should restore everything back to '('
                 * instead since it might be part of "named ..."]
                 */
                keeptrailingchars = FALSE;
            } else {
                chargesfound = 1;
            }
        }
        if (keeptrailingchars) {
            char *pp = eos(bp);

            /* 'pp' points at 'pb's terminating '\0',
               'p' points at ')' and will be incremented past it */
            do {
                *pp++ = *++p;
            } while (*p);
        }
    }
    /*
     * otmp->enchantment is type schar, so we don't want enchantment to be any bigger or
     * smaller.  Also, enchantment should always be positive --some cheaters may
     * try to confuse atoi().
     */
    if (enchantment < 0) {
        spesgn = -1; /* cheaters get what they deserve */
        enchantment = abs(enchantment);
    }
    if (enchantment > SCHAR_LIM)
        enchantment = SCHAR_LIM;

    if (charges > SCHAR_LIM)
        charges = SCHAR_LIM;

    if (rechrg < 0 || rechrg > RECHARGE_LIMIT)
        rechrg = RECHARGE_LIMIT;

    /* now we have the actual name, as delivered by xname, say
     *  green potions called whisky
     *  scrolls labeled "QWERTY"
     *  egg
     *  fortune cookies
     *  very heavy iron ball named hoei
     *  wand of wishing
     *  elven cloak
     */
    if ((p = strstri(bp, " named ")) != 0) {
        *p = 0;
        name = p + 7;
    }
    if ((p = strstri(bp, " entitled ")) != 0) {
        *p = 0;
        name = p + 10;
    }
    if ((p = strstri(bp, " called ")) != 0) {
        *p = 0;
        un = p + 8;
        /* "helmet called telepathy" is not "helmet" (a specific type)
         * "shield called reflection" is not "shield" (a general type)
         */
        for (i = 0; i < SIZE(o_ranges); i++)
            if (!strcmpi(bp, o_ranges[i].name)) {
                oclass = o_ranges[i].oclass;
                goto srch;
            }
    }
    if ((p = strstri(bp, " labeled ")) != 0) {
        *p = 0;
        dn = p + 9;
    } else if ((p = strstri(bp, " labelled ")) != 0) {
        *p = 0;
        dn = p + 10;
    }
    if ((p = strstri(bp, " of spinach")) != 0 && strstri(bp, " of spinach leaves") == 0) {
        *p = 0;
        contents = CONTAINER_SPINACH;
    }



    /*
     * Skip over "pair of ", "pairs of", "set of" and "sets of".
     *
     * Accept "3 pair of boots" as well as "3 pairs of boots".  It is
     * valid English either way.  See makeplural() for more on pair/pairs.
     *
     * We should only double count if the object in question is not
     * referred to as a "pair of".  E.g. We should double if the player
     * types "pair of spears", but not if the player types "pair of
     * lenses".  Luckily (?) all objects that are referred to as pairs
     * -- boots, gloves, and lenses -- are also not mergable, so cnt is
     * ignored anyway.
     */
    if (!strncmpi(bp, "pair of ", 8)) {
        bp += 8;
        cnt *= 2;
    } else if (!strncmpi(bp, "pairs of ", 9)) {
        bp += 9;
        if (cnt > 1)
            cnt *= 2;
    } else if (!strncmpi(bp, "set of ", 7)) {
        bp += 7;
    } else if (!strncmpi(bp, "sets of ", 8)) {
        bp += 8;
    }

    /* Mythic suffixes */
    char* mythic_sng = makesingular(bp);
    if (mythic_sng != 0)
    {
        //make all lower case
        char* lp;
        for (lp = mythic_sng; *lp; lp++)
        {
            *lp = lowc(*lp);
        }
    }
    int mythic_idx;
    for (mythic_idx = 1; mythic_idx < MAX_MYTHIC_SUFFIXES; mythic_idx++)
    {
        if (mythic_sng != 0 && (p = strstri(mythic_sng, mythic_suffix_qualities[mythic_idx].mythic_affix)) != 0
            && strlen(p) == strlen(mythic_suffix_qualities[mythic_idx].mythic_affix))
        {
            char ringbuf[BUFSZ], robebuf[BUFSZ], bootsbuf[BUFSZ], glovesbuf[BUFSZ], gauntletsbuf[BUFSZ], bracersbuf[BUFSZ], amuletbuf[BUFSZ], cloakbuf[BUFSZ], shirtbuf[BUFSZ];
            char potionbuf[BUFSZ], wandbuf[BUFSZ], scrollbuf[BUFSZ], spellbookbuf[BUFSZ];
            Sprintf(ringbuf, "ring%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(robebuf, "robe%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(bootsbuf, "boots%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(cloakbuf, "cloak%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(shirtbuf, "shirt%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(glovesbuf, "gloves%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(amuletbuf, "amulet%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(bracersbuf, "bracers%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(gauntletsbuf, "gauntlets%s", mythic_suffix_qualities[mythic_idx].mythic_affix);

            Sprintf(wandbuf, "wand%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(potionbuf, "potion%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(scrollbuf, "scroll%s", mythic_suffix_qualities[mythic_idx].mythic_affix);
            Sprintf(spellbookbuf, "spellbook%s", mythic_suffix_qualities[mythic_idx].mythic_affix);

            if (!(p == mythic_sng + 4 && !strcmp(p - 4, ringbuf)) /* No rings and other standard items etc. */
                && !(p == mythic_sng + 4 && !strcmp(p - 4, robebuf))
                && !(p == mythic_sng + 5 && !strcmp(p - 5, bootsbuf))
                && !(p == mythic_sng + 5 && !strcmp(p - 5, cloakbuf))
                && !(p == mythic_sng + 5 && !strcmp(p - 5, shirtbuf))
                && !(p == mythic_sng + 6 && !strcmp(p - 6, glovesbuf))
                && !(p == mythic_sng + 6 && !strcmp(p - 6, amuletbuf))
                && !(p == mythic_sng + 7 && !strcmp(p - 7, bracersbuf))
                && !(p == mythic_sng + 9 && !strcmp(p - 9, gauntletsbuf))

                && !(p == mythic_sng + 4 && !strcmp(p - 4, wandbuf))
                && !(p == mythic_sng + 6 && !strcmp(p - 6, potionbuf))
                && !(p == mythic_sng + 6 && !strcmp(p - 6, scrollbuf))
                && !(p == mythic_sng + 9 && !strcmp(p - 9, spellbookbuf))
                )
            {
                p = strstri(bp, mythic_suffix_qualities[mythic_idx].mythic_affix);
                if (p)
                {
                    *p = 0;
                    mythic_suffix = mythic_idx;
                }
            }
            break;
        }
    }

    /* intercept pudding globs here; they're a valid wish target,
     * but we need them to not get treated like a corpse.
     *
     * also don't let player wish for multiple globs.
     */
    i = (int) strlen(bp);
    p = (char *) 0;
    /* check for "glob", "<foo> glob", and "glob of <foo>" */
    if (!strcmpi(bp, "glob") || !BSTRCMPI(bp, bp + i - 5, " glob")
        || !strcmpi(bp, "globs") || !BSTRCMPI(bp, bp + i - 6, " globs")
        || (p = strstri(bp, "glob of ")) != 0
        || (p = strstri(bp, "globs of ")) != 0) {
        mntmp = name_to_mon(!p ? bp : (strstri(p, " of ") + 4));
        /* if we didn't recognize monster type, pick a valid one at random */
        if (mntmp == NON_PM)
            mntmp = rn1(PM_BLACK_PUDDING - PM_GRAY_OOZE, PM_GRAY_OOZE);
        /* construct canonical spelling in case name_to_mon() recognized a
           variant (grey ooze) or player used inverted syntax (<foo> glob);
           if player has given a valid monster type but not valid glob type,
           object name lookup won't find it and wish attempt will fail */
        Sprintf(globbuf, "glob of %s", mons[mntmp].mname);
        bp = globbuf;
        mntmp = NON_PM; /* not useful for "glob of <foo>" object lookup */
        cnt = 0; /* globs don't stack */
        oclass = FOOD_CLASS;
        actualn = bp, dn = 0;
        goto srch;
    } else {
        /*
         * Find corpse type using "of" (figurine of an orc, tin of orc meat)
         * Don't check if it's a wand or spellbook.
         * (avoid "wand/finger of death" confusion).
         */
        if (!strstri(bp, " guano") // rule out bat guano
            && !strstri(bp, "wand ") && !strstri(bp, "spellbook ") && !strstri(bp, "bracers ")
            && !strstri(bp, "sword ") && !strstri(bp, "dagger ") && !strstri(bp, "arrow ") && !strstri(bp, "arrows ")
            && !strstri(bp, "axe ") && !strstri(bp, "bolt ") && !strstri(bp, "quarrel ") && !strstri(bp, "sling-bullet ")
            && !strstri(bp, "bolts ") && !strstri(bp, "quarrels ") && !strstri(bp, "sling-bullets ")
            && !strstri(bp, "mace ") && !strstri(bp, "flail ") && !strstri(bp, "hammer ") && !strstri(bp, "morning star ")
            && !strstri(bp, "staff ") && !strstri(bp, "bow ") && !strstri(bp, "crossbow ")
            && !strstri(bp, "robe ") && !strstri(bp, "cloak ") && !strstri(bp, "gloves ")
            && !strstri(bp, "gauntlets ") && !strstri(bp, "belt ") && !strstri(bp, "girdle ")
            && !strstri(bp, "boots ") && !strstri(bp, "ring ")
            && !strstri(bp, "potion ") && !strstri(bp, "scroll ")
            && !strstri(bp, "potions ") && !strstri(bp, "scrolls ")
            && !strstri(bp, "finger ")) {
            if ((p = strstri(bp, "tin of ")) != 0) {
                if (!strcmpi(p + 7, "spinach")) {
                    contents = CONTAINER_SPINACH;
                    mntmp = NON_PM;
                } else {
                    tmp = tin_variety_txt(p + 7, &tinv);
                    tvariety = tinv;
                    mntmp = name_to_mon(p + 7 + tmp);
                }
                typ = TIN;
                goto typfnd;
            } else if ((p = strstri(bp, " of ")) != 0
                       && (mntmp = name_to_mon(p + 4)) >= LOW_PM)
                *p = 0;
        }
    }
    /* Find corpse type w/o "of" (red dragon scale mail, yeti corpse) */
    if (strncmpi(bp, "samurai sword", 13)  /* not the "samurai" monster! */
        && strncmpi(bp, "wizard lock", 11) /* not the "wizard" monster! */
        && strncmpi(bp, "wizard's robe", 13) /* not the "wizard" monster! */
        && strncmpi(bp, "bat guano", 9) /* not the "bat" monster! */
        && strncmpi(bp, "ninja-to", 8)     /* not the "ninja" rank */
        && strncmpi(bp, "master key", 10)  /* not the "master" rank */
        && strncmpi(bp, "death cap", 9)  /* not the "death" monster */
        && strncmpi(bp, "magenta", 7))   /* not the "mage" rank */
    {
        if (mntmp < LOW_PM && strlen(bp) > 2
            && (mntmp = name_to_mon(bp)) >= LOW_PM) {
            int mntmptoo;
            size_t mntmplen; /* double check for rank title */
            char *obp = bp;

            mntmptoo = title_to_mon(bp, (int *) 0, &mntmplen);
            bp += (mntmp != mntmptoo) ? strlen(mons[mntmp].mname)
                                      : mntmplen;
            if (*bp == ' ') {
                bp++;
            } else if (!strncmpi(bp, "s ", 2)) {
                bp += 2;
            } else if (!strncmpi(bp, "es ", 3)) {
                bp += 3;
            } else if (!*bp && !actualn && !dn && !un && !oclass) {
                /* no referent; they don't really mean a monster type */
                bp = obp;
                mntmp = NON_PM;
            }
        }
    }

    /* first change to singular if necessary */
    if (*bp) {
        char *sng = makesingular(bp);

        if (strcmp(bp, sng)) {
            if (cnt == 1)
                cnt = 2;
            Strcpy(bp, sng);
        }
    }

    /* Alternate spellings (pick-ax, silver sabre, &c) */
    {
        const struct alt_spellings *as = spellings;

        while (as->sp) {
            if (fuzzymatch(bp, as->sp, " -", TRUE)) {
                typ = as->ob;
                goto typfnd;
            }
            as++;
        }
        /* can't use spellings list for this one due to shuffling */
        if (!strncmpi(bp, "grey spell", 10))
            *(bp + 2) = 'a';

        if ((p = strstri(bp, "armour")) != 0) {
            /* skip past "armo", then copy remainder beyond "u" */
            p += 4;
            while ((*p = *(p + 1)) != '\0')
                ++p; /* self terminating */
        }
    }

    /* dragon scales - assumes order of dragons */
    if (!strcmpi(bp, "scales") && mntmp >= PM_GRAY_DRAGON
        && mntmp <= PM_YELLOW_DRAGON) {
        typ = GRAY_DRAGON_SCALES + mntmp - PM_GRAY_DRAGON;
        mntmp = NON_PM; /* no monster */
        goto typfnd;
    }

    p = eos(bp);
    if (!BSTRCMPI(bp, p - 10, "holy water")) {
        typ = POT_WATER;
        if ((p - bp) >= 12 && *(p - 12) == 'u')
            iscursed = 1; /* unholy water */
        else
            blessed = 1;
        goto typfnd;
    }
    if (unlabeled && !BSTRCMPI(bp, p - 6, "scroll")) {
        typ = SCR_BLANK_PAPER;
        goto typfnd;
    }
    if (unlabeled && !BSTRCMPI(bp, p - 9, "spellbook")) {
        typ = SPE_BLANK_PAPER;
        goto typfnd;
    }
    /* specific food rather than color of gem/potion/spellbook[/scales] */
    if (!BSTRCMPI(bp, p - 6, "orange") && mntmp == NON_PM) {
        typ = ORANGE;
        goto typfnd;
    }
    /*
     * NOTE: Gold pieces are handled as objects nowadays, and therefore
     * this section should probably be reconsidered as well as the entire
     * gold/money concept.  Maybe we want to add other monetary units as
     * well in the future. (TH)
     */
    if (!BSTRCMPI(bp, p - 10, "gold piece")
        || !BSTRCMPI(bp, p - 7, "zorkmid")
        || !strcmpi(bp, "gold") || !strcmpi(bp, "money")
        || !strcmpi(bp, "coin") || *bp == GOLD_SYM) {
        if (cnt > 5000 && !wiz_wishing)
            cnt = 5000;
        else if (cnt < 1)
            cnt = 1;
        otmp = mksobj(GOLD_PIECE, FALSE, FALSE, 2);
        otmp->quan = (long) cnt;
        otmp->owt = weight(otmp);
        context.botl = 1;
        return otmp;
    }

    /* check for single character object class code ("/" for wand, &c) */
    if (strlen(bp) == 1 && (i = def_char_to_objclass(*bp)) < MAX_OBJECT_CLASSES
        && i > ILLOBJ_CLASS && (i != VENOM_CLASS || wiz_wishing)) {
        oclass = i;
        goto any;
    }

    /* Search for class names: XXXXX potion, scroll of XXXXX.  Avoid */
    /* false hits on, e.g., rings for "ring mail". */
    if (strncmpi(bp, "enchant ", 8)
        && strncmpi(bp, "destroy ", 8)
        && strncmpi(bp, "detect food", 11)
        && strncmpi(bp, "food detection", 14)
        && strncmpi(bp, "ring mail", 9)
        && strncmpi(bp, "staff of withering", 18)
        && strncmpi(bp, "one ring", 8)
        && strncmpi(bp, "wand of orcus", 13)
        && strncmpi(bp, "studded leather armor", 21)
        && strncmpi(bp, "leather armor", 13)
        && strncmpi(bp, "tooled horn", 11)
        && strncmpi(bp, "food ration", 11)
        && strncmpi(bp, "weapon rack", 11)
        && strncmpi(bp, "Ring of Three Wishes", 20)
        && strncmpi(bp, "ring of three wishes", 20)
        && strncmpi(bp, "Serpent Ring of Set", 19)
        && strncmpi(bp, "serpent ring of set", 19)
        && strncmpi(bp, "Ring of Conflict", 16)
        && strncmpi(bp, "ring of conflict", 16)
        && strncmpi(bp, "oracular toadstool", 18)
        && strncmpi(bp, "meat ring", 9))
        for (i = 0; i < (int) (sizeof wrpsym); i++)
        {
            register size_t j = strlen(wrp[i]);

            if (!strncmpi(bp, wrp[i], j))
            {
                oclass = wrpsym[i];
                if (oclass != AMULET_CLASS) 
                {
                    bp += j;
                    if (!strncmpi(bp, " of ", 4))
                        actualn = bp + 4;
                    /* else if(*bp) ?? */
                } else
                    actualn = bp;
                goto srch;
            }
            
            if (!strncmpi("ring", wrp[i], j) && strncmpi(bp, "enchant ring", 12))
                continue;

            if (!BSTRCMPI(bp, p - j, wrp[i])) {
                oclass = wrpsym[i];
                p -= j;
                *p = 0;
                if (p > bp && p[-1] == ' ')
                    p[-1] = 0;
                actualn = dn = bp;
                goto srch;
            }
        }

    /* Wishing in wizard mode can create traps and furniture.
     * Part I:  distinguish between trap and object for the two
     * types of traps which have corresponding objects:  bear trap
     * and land mine.  "beartrap" (object) and "bear trap" (trap)
     * have a difference in spelling which we used to exploit by
     * adding a special case in wishymatch(), but "land mine" is
     * spelled the same either way so needs different handing.
     * Since we need something else for land mine, we've dropped
     * the bear trap hack so that both are handled exactly the
     * same.  To get an armed trap instead of a disarmed object,
     * the player can prefix either the object name or the trap
     * name with "trapped " (which ordinarily applies to chests
     * and tins), or append something--anything at all except for
     * " object", but " trap" is suggested--to either the trap
     * name or the object name.
     */
    if (wiz_wishing && (!strncmpi(bp, "bear", 4) || !strncmpi(bp, "land", 4))) {
        boolean beartrap = (lowc(*bp) == 'b');
        char *zp = bp + 4; /* skip "bear"/"land" */

        if (*zp == ' ')
            ++zp; /* embedded space is optional */
        if (!strncmpi(zp, beartrap ? "trap" : "mine", 4)) {
            zp += 4;
            if (trapped == 2 || !strcmpi(zp, " object")) {
                /* "untrapped <foo>" or "<foo> object" */
                typ = beartrap ? BEARTRAP : LAND_MINE;
                goto typfnd;
            } else if (trapped == 1 || *zp != '\0') {
                /* "trapped <foo>" or "<foo> trap" (actually "<foo>*") */
                int idx = trap_to_defsym(beartrap ? BEAR_TRAP : LANDMINE);

                /* use canonical trap spelling, skip object matching */
                Strcpy(bp, defsyms[idx].explanation);
                goto wiztrap;
            }
            /* [no prefix or suffix; we're going to end up matching
               the object name and getting a disarmed trap object] */
        }
    }

 retry:
    /* "grey stone" check must be before general "stone" */
    for (i = 0; i < SIZE(o_ranges); i++)
        if (!strcmpi(bp, o_ranges[i].name)) {
            typ = rnd_class(o_ranges[i].f_o_range, o_ranges[i].l_o_range);
            goto typfnd;
        }

    if (!BSTRCMPI(bp, p - 6, " stone") || !BSTRCMPI(bp, p - 4, " gem")) {
        p[!strcmpi(p - 4, " gem") ? -4 : -6] = '\0';
        oclass = GEM_CLASS;
        dn = actualn = bp;
        goto srch;
    } else if (!strcmpi(bp, "looking glass")) {
        ; /* avoid false hit on "* glass" */
    } else if (!BSTRCMPI(bp, p - 6, " glass") || !strcmpi(bp, "glass")) {
        register char *g = bp;

        /* treat "broken glass" as a non-existent item; since "broken" is
           also a chest/box prefix it might have been stripped off above */
        if (broken || strstri(g, "broken"))
            return (struct obj *) 0;
        if (!strncmpi(g, "worthless ", 10))
            g += 10;
        if (!strncmpi(g, "piece of ", 9))
            g += 9;
        if (!strncmpi(g, "colored ", 8))
            g += 8;
        else if (!strncmpi(g, "coloured ", 9))
            g += 9;
        if (!strcmpi(g, "glass")) { /* choose random color */
            /* 9 different kinds */
            typ = LAST_GEM + rnd(9);
            if (objects[typ].oc_class == GEM_CLASS)
                goto typfnd;
            else
                typ = 0; /* somebody changed objects[]? punt */
        } else { /* try to construct canonical form */
            char tbuf[BUFSZ];

            Strcpy(tbuf, "worthless piece of ");
            Strcat(tbuf, g); /* assume it starts with the color */
            Strcpy(bp, tbuf);
        }
    }

    actualn = bp;
    if (!dn)
        dn = actualn; /* ex. "skull cap" */
 srch:
    /* check real names of gems first */
    if (!oclass && actualn) {
        for (i = bases[GEM_CLASS]; i <= LAST_GEM; i++) {
            register const char *zn;

            if ((zn = OBJ_NAME(objects[i])) != 0 && !strcmpi(actualn, zn)) {
                typ = i;
                goto typfnd;
            }
        }
        /* "tin of foo" would be caught above, but plain "tin" has
           a random chance of yielding "tin wand" unless we do this */
        if (!strcmpi(actualn, "tin")) {
            typ = TIN;
            goto typfnd;
        }
    }

    if (wiz_wishing && actualn && wishymatch(actualn, OBJ_NAME(objects[AMULET_OF_YENDOR]), TRUE))
    {
        typ = AMULET_OF_YENDOR;
        goto typfnd;
    }

    if (((typ = rnd_otyp_by_namedesc(actualn, oclass, 1)) != STRANGE_OBJECT)
        || ((typ = rnd_otyp_by_namedesc(dn, oclass, 1)) != STRANGE_OBJECT)
        || ((typ = rnd_otyp_by_namedesc(un, oclass, 1)) != STRANGE_OBJECT)
        || ((typ = rnd_otyp_by_namedesc(origbp, oclass, 1)) != STRANGE_OBJECT))
        goto typfnd;
    typ = 0;

    if (actualn) {
        struct Jitem *j = Japanese_items;

        while (j->item) {
            if (actualn && !strcmpi(actualn, j->name)) {
                typ = j->item;
                goto typfnd;
            }
            j++;
        }
    }
    /* if we've stripped off "armor" and failed to match anything
       in objects[], append "mail" and try again to catch misnamed
       requests like "plate armor" and "yellow dragon scale armor" */
    if (oclass == ARMOR_CLASS && !strstri(bp, "mail")) {
        /* modifying bp's string is ok; we're about to resort
           to random armor if this also fails to match anything */
        Strcat(bp, " mail");
        goto retry;
    }
    if (!strcmpi(bp, "spinach")) {
        contents = CONTAINER_SPINACH;
        typ = TIN;
        goto typfnd;
    }
    /* Note: not strcmpi.  2 fruits, one capital, one not, are possible.
       Also not strncmp.  We used to ignore trailing text with it, but
       that resulted in "grapefruit" matching "grape" if the latter came
       earlier than the former in the fruit list. */
    {
        char *fp;
        int l, cntf;
        int blessedf, iscursedf, uncursedf, halfeatenf;

        blessedf = iscursedf = uncursedf = halfeatenf = 0;
        cntf = 0;

        fp = fruitbuf;
        for (;;) {
            if (!fp || !*fp)
                break;
            if (!strncmpi(fp, "an ", l = 3) || !strncmpi(fp, "a ", l = 2)) {
                cntf = 1;
            } else if (!cntf && digit(*fp)) {
                cntf = atoi(fp);
                while (digit(*fp))
                    fp++;
                while (*fp == ' ')
                    fp++;
                l = 0;
            } else if (!strncmpi(fp, "blessed ", l = 8)) {
                blessedf = 1;
            } else if (!strncmpi(fp, "cursed ", l = 7)) {
                iscursedf = 1;
            } else if (!strncmpi(fp, "uncursed ", l = 9)) {
                uncursedf = 1;
            } else if (!strncmpi(fp, "partly eaten ", l = 13)
                       || !strncmpi(fp, "partially eaten ", l = 16)) {
                halfeatenf = 1;
            } else
                break;
            fp += l;
        }

        for (f = ffruit; f; f = f->nextf) {
            /* match type: 0=none, 1=exact, 2=singular, 3=plural */
            int ftyp = 0;

            if (!strcmp(fp, f->fname))
                ftyp = 1;
            else if (!strcmp(fp, makesingular(f->fname)))
                ftyp = 2;
            else if (!strcmp(fp, makeplural(f->fname)))
                ftyp = 3;
            if (ftyp) {
                typ = SLIME_MOLD;
                blessed = blessedf;
                iscursed = iscursedf;
                uncursed = uncursedf;
                halfeaten = halfeatenf;
                /* adjust count if user explicitly asked for
                   singular amount (can't happen unless fruit
                   has been given an already pluralized name)
                   or for plural amount */
                if (ftyp == 2 && !cntf)
                    cntf = 1;
                else if (ftyp == 3 && !cntf)
                    cntf = 2;
                cnt = cntf;
                ftype = f->fid;
                goto typfnd;
            }
        }
    }

    if (!oclass && actualn) {
        short objtyp;

        /* Perhaps it's an artifact specified by name, not type */
        name = artifact_name(actualn, &objtyp);
        if (name) {
            typ = objtyp;
            isartifact = TRUE;
            goto typfnd;
        }
    }
/* Let wizards wish for traps and furniture.
 * Must come after objects check so wizards can still wish for
 * trap objects like beartraps.
 * Disallow such topology tweaks for WIZKIT startup wishes.
 */
 wiztrap:
    if (wiz_wishing && !program_state.wizkit_wishing) {
        struct rm *lev;
        int trap, x = u.ux, y = u.uy;

        for (trap = NO_TRAP + 1; trap < TRAPNUM; trap++) {
            struct trap *t;
            const char *tname;

            tname = defsyms[trap_to_defsym(trap)].explanation;
            if (strncmpi(tname, bp, strlen(tname)))
                continue;
            /* found it; avoid stupid mistakes */
            if (is_hole(trap) && !Can_fall_thru(&u.uz))
                trap = ROCKTRAP;
            if ((t = maketrap(x, y, trap, NON_PM, MKTRAP_NO_FLAGS)) != 0) {
                trap = t->ttyp;
                if (trap == LEVER && is_switchable)
                {
                    t->tflags |= TRAPFLAGS_SWITCHABLE_BETWEEN_STATES | TRAPFLAGS_CONTINUOUSLY_SWITCHABLE;
                }
                tname = defsyms[trap_to_defsym(trap)].explanation;
                pline("%s%s.", An(tname),
                      (trap != MAGIC_PORTAL) ? "" : " to nowhere");
                newsym(x, y);
            } else
                pline("Creation of %s failed.", an(tname));
            return (struct obj *) &zeroobj;
        }

        /* furniture and terrain */
        lev = &levl[x][y];
        p = eos(bp);
        if (!BSTRCMPI(bp, p - 8, "fountain")) 
        {
            int lsubtype = 0;
            int lvartype = 0;
            uchar lflags = 0;
            boolean lhorizontal = 0;
            if (!strncmpi(bp, "enchanted magic ", 16))
            {
                lhorizontal = 1;
                lsubtype = FOUNTAIN_MAGIC;
            }
            if (!strncmpi(bp, "natural ", 7))
                lsubtype = FOUNTAIN_NATURAL;
            if (!strncmpi(bp, "magic ", 6))
                lsubtype = FOUNTAIN_MAGIC;
            if (!strncmpi(bp, "healing ", 8))
                lsubtype = FOUNTAIN_HEALING;
            if (!strncmpi(bp, "mana ", 5))
                lsubtype = FOUNTAIN_MANA;
            if (!strncmpi(bp, "power ", 6))
                lsubtype = FOUNTAIN_POWER;
            if (!strncmpi(bp, "water ", 6))
                lsubtype = FOUNTAIN_WATER;
            if (!strncmpi(bp, "poison ", 7))
                lsubtype = FOUNTAIN_POISON;
            
            int floortype = IS_FLOOR(lev->typ) ? lev->typ : ROOM;
            int floorsubtype = IS_FLOOR(lev->typ) ? lev->subtyp : get_initial_location_subtype(ROOM);
            int floorvartype = IS_FLOOR(lev->typ) ? lev->vartyp : get_initial_location_vartype(ROOM, floorsubtype);

            full_location_transform(x, y, FOUNTAIN, lsubtype, lvartype, lflags, 0, 0, 0, 0, 0, 0, 0, 0, floortype, floorsubtype, floorvartype, FALSE, lhorizontal, 0, 0, FALSE);

            int ftyp = lev->subtyp; // (lev->fountainmask & FOUNTAIN_TYPE_MASK);
            pline("A %s.", ftyp == FOUNTAIN_MAGIC && lev->blessedftn ? "enchanted fountain" : fountain_type_text(ftyp));
            newsym(x, y);
            return (struct obj *) &zeroobj;
        }
        if (!BSTRCMPI(bp, p - 6, "throne")) 
        {
            create_simple_location(x, y, THRONE, 0, 0, 0, 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, FALSE);
            pline("A throne.");
            newsym(x, y);
            return (struct obj *) &zeroobj;
        }
        if (!BSTRCMPI(bp, p - 5, "anvil"))
        {
            create_simple_location(x, y, ANVIL, 0, 0, 0, 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, FALSE);
            pline("An anvil.");
            newsym(x, y);
            return (struct obj*)&zeroobj;
        }
        if (!BSTRCMPI(bp, p - 4, "sink")) {
            create_simple_location(x, y, SINK, 0, 0, 0, 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, FALSE);
            pline("A sink.");
            newsym(x, y);
            return (struct obj *) &zeroobj;
        }
        /* ("water" matches "potion of water" rather than terrain) */
        if (!BSTRCMPI(bp, p - 4, "pool") || !BSTRCMPI(bp, p - 4, "moat")) {
            create_simple_location(x, y, !BSTRCMPI(bp, p - 4, "pool") ? POOL : MOAT, 0, 0, 0, 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, FALSE);
            del_engr_at(x, y);
            pline("A %s.", (lev->typ == POOL) ? "pool" : "moat");
            /* Must manually make kelp! */
            water_damage_chain(level.objects[x][y], TRUE);
            newsym(x, y);
            return (struct obj *) &zeroobj;
        }
        if (!BSTRCMPI(bp, p - 4, "lava")) { /* also matches "molten lava" */
            create_simple_location(x, y, LAVAPOOL, 0, 0, 0, 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, FALSE);
            del_engr_at(x, y);
            pline("A pool of molten lava.");
            if (!(Levitation || Flying))
                (void) lava_effects();
            newsym(x, y);
            return (struct obj *) &zeroobj;
        }

        if (!BSTRCMPI(bp, p - 5, "altar")) {
            aligntyp al;

            if (!strncmpi(bp, "chaotic ", 8))
                al = A_CHAOTIC;
            else if (!strncmpi(bp, "neutral ", 8))
                al = A_NEUTRAL;
            else if (!strncmpi(bp, "lawful ", 7))
                al = A_LAWFUL;
            else if (!strncmpi(bp, "unaligned ", 10))
                al = A_NONE;
            else /* -1 - A_CHAOTIC, 0 - A_NEUTRAL, 1 - A_LAWFUL */
                al = (!rn2(6)) ? A_NONE : rn2((int) A_LAWFUL + 2) - 1;

            create_simple_location(x, y, ALTAR, 0, 0, Align2amask(al), 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, FALSE);
            pline("%s altar.", An(align_str(al)));
            newsym(x, y);
            return (struct obj *) &zeroobj;
        }

        if (!BSTRCMPI(bp, p - 5, "grave")
            || !BSTRCMPI(bp, p - 9, "headstone")) {
            make_grave(x, y, (char *) 0, FALSE);
            pline("%s.", IS_GRAVE(lev->typ) ? "A grave"
                                            : "Can't place a grave here");
            newsym(x, y);
            return (struct obj *) &zeroobj;
        }

        if (!BSTRCMPI(bp, p - 7, "brazier")) {
            create_simple_location(x, y, BRAZIER, 0, 0, 0, 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, FALSE);
            pline("%s.", IS_BRAZIER(lev->typ) ? "A brazier"
                : "Can't place a brazier here");
            newsym(x, y);
            return (struct obj*)&zeroobj;
        }

        if (!BSTRCMPI(bp, p - 8, "signpost")) {
            make_signpost(x, y, "Here's the signpost you wished for", FALSE);
            pline("%s.", IS_SIGNPOST(lev->typ) ? "A signpost"
                : "Can't place a signpost here");
            newsym(x, y);
            return (struct obj*)&zeroobj;
        }

        for (i = 1; i < MAX_TREE_SUBTYPES; i++)
        {
            if (!tree_subtype_definitions[i].description)
                continue;

            int len = (int)strlen(tree_subtype_definitions[i].description);

            if (len == 0)
                continue;

            if (!BSTRCMPI(bp, p - len, tree_subtype_definitions[i].description)) {
                int subtype = i;
                int vartype = get_initial_location_vartype(TREE, subtype);
                create_location_with_current_floor(x, y, TREE, subtype, vartype, 0, 0, FALSE);
                pline("%s.", An(tree_subtype_definitions[i].description));
                newsym(x, y);
                block_vision_and_hearing_at_point(x, y);
                return (struct obj*)&zeroobj;
            }

        }

        if (!BSTRCMPI(bp, p - 4, "tree")) {
            int subtype = get_initial_location_subtype(TREE);
            int vartype = get_initial_location_vartype(TREE, subtype);
            create_simple_location(x, y, TREE, subtype, vartype, 0, 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].vartyp : levl[x][y].floorvartyp, FALSE);
            pline("A tree.");
            newsym(x, y);
            block_vision_and_hearing_at_point(x, y);
            return (struct obj*)&zeroobj;
        }



        if (!BSTRCMPI(bp, p - 4, "bars")) {
            create_simple_location(x, y, IRONBARS, 0, 0, 0, 0, IS_FLOOR(levl[x][y].typ) ? levl[x][y].typ : levl[x][y].floortyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].subtyp : levl[x][y].floorsubtyp, IS_FLOOR(levl[x][y].typ) ? levl[x][y].vartyp : levl[x][y].floorvartyp, FALSE);
            pline("Iron bars.");
            newsym(x, y);
            return (struct obj *) &zeroobj;
        }
    }

    if (!oclass && !typ) {
        if (!strncmpi(bp, "polearm", 7)) {
            typ = rnd_otyp_by_wpnskill(P_POLEARM);
            goto typfnd;
        } else if (!strncmpi(bp, "hammer", 6)) {
            typ = rnd_otyp_by_wpnskill(P_BLUDGEONING_WEAPON);
            goto typfnd;
        }
    }

    if (!material)
    {
        int m;
        /* Check for materials */
        for (m = MAT_NONE + 1; m < MAX_MATERIAL_TYPES; m++)
        {
            if (material_definitions[m].wishable)
            {
                size_t bplen = strlen(bp);
                size_t mlen = strlen(material_definitions[m].object_prefix);
                size_t mlena = material_definitions[m].adjective ? strlen(material_definitions[m].adjective) : 0;
                if ((!strncmpi(bp, material_definitions[m].object_prefix, mlen) && bplen >= mlen + 1 && *(bp + mlen) == ' ')
                    || (material_definitions[m].adjective && !strncmpi(bp, material_definitions[m].adjective, mlena) && bplen >= mlena + 1 && *(bp + mlena) == ' '))
                {
                    bp += mlen + 1;
                    material = m;
                    goto retry;
                }
            }
        }
    }

    if (!oclass)
        return ((struct obj *) 0);
 any:
    if (!oclass)
        oclass = wrpsym[rn2((int) sizeof wrpsym)];
 typfnd:
    if (typ)
        oclass = objects[typ].oc_class;

    /* handle some objects that are only allowed in wizard mode */
    if (typ && (!wiz_wishing || (wiz_wishing && (
           typ == AMULET_OF_YENDOR 
        || typ == CANDELABRUM_OF_INVOCATION
        || typ == BELL_OF_OPENING
        || typ == SPE_BOOK_OF_THE_DEAD
        || typ == MAGIC_LAMP
        || (!isartifact && is_otyp_nowish(typ))
        || (isartifact && (artilist[get_artifact_id(typ, name)].aflags & AF_NO_WISH))
        )
        && yn_query("That item is nonwishable. Force it anyway?") != 'y'))) {
        switch (typ) {
        case AMULET_OF_YENDOR:
            typ = FAKE_AMULET_OF_YENDOR;
            break;
        case CANDELABRUM_OF_INVOCATION:
            typ = !rn2(2) ? LARGE_FIVE_BRANCHED_CANDELABRUM : rnd_class(TALLOW_CANDLE, WAX_CANDLE);
            break;
        case BELL_OF_OPENING:
            typ = BELL;
            break;
        case SPE_BOOK_OF_THE_DEAD:
            typ = SPE_BLANK_PAPER;
            break;
        case SPE_BOOK_OF_MODRON:
            typ = SPE_BLANK_PAPER;
            isartifact = FALSE;
            name = 0;
            break;
        case MAGIC_LAMP:
            typ = OIL_LAMP;
            break;
        default:
            /* catch any other non-wishable objects (venom) */
            if (is_otyp_nowish(typ) && !isartifact)
                return (struct obj *) 0;

            if (isartifact && (artilist[get_artifact_id(typ, name)].aflags & AF_NO_WISH))
                return (struct obj*) 0;
            break;
        }
    }

    /*
     * Create the object, then fine-tune it.
     */
    unsigned long mkflags = 0UL;
    if (open && typ > 0 && Is_otyp_container_with_lid(typ))
        mkflags |= MKOBJ_FLAGS_OPEN_COFFIN;

    otmp = typ ? mksobj_with_flags(typ, TRUE, FALSE, 2, (struct monst*)0, MAT_NONE, 0L, 0L, mkflags) : mkobj(oclass, FALSE, 2);
    typ = otmp->otyp, oclass = otmp->oclass; /* what we actually got */

    if (islit && (is_lamp(otmp) || is_candle(otmp) || is_torch(otmp) || is_obj_candelabrum(otmp) || typ == POT_OIL))
    {
        place_object(otmp, u.ux, u.uy); /* make it viable light source */
        begin_burn(otmp, FALSE);
        obj_extract_self(otmp); /* now release it for caller's use */
    }

    /* if player specified a reasonable count, maybe honor it */
    if (cnt > 0 && objects[typ].oc_merge
        && (wiz_wishing || cnt < rnd(6) || (cnt <= 7 && is_candle(otmp))
            || (cnt <= 20 && ((oclass == WEAPON_CLASS && is_ammo(otmp))
                              || is_rock(otmp) || is_missile(otmp)))))
        otmp->quan = (long) cnt;

    if (oclass == GEM_CLASS && !wiz_wishing)
        otmp->quan = 1;


    /* set enchantment */
    if (spesgn == 0) {
        enchantment = otmp->enchantment;
    }
    else if (wiz_wishing)
    {
        ; /* no alteration to enchantment */
    }
    else if (oclass == ARMOR_CLASS || oclass == WEAPON_CLASS
               || is_weptool(otmp)
               || (oclass == RING_CLASS && objects[typ].oc_enchantable)
               || objects[typ].oc_enchantable
        ) 
    {
        if (enchantment > get_obj_init_enchantment(otmp) && enchantment > otmp->enchantment)
            enchantment = 0;
        if (enchantment > get_obj_max_enchantment(otmp) / 3 && Luck < 0)
            spesgn = -1;
    }
    else 
    {
        if (enchantment > 0 && spesgn == -1)
            enchantment = 0;
        if (enchantment > otmp->enchantment)
            enchantment = otmp->enchantment;
    }

    if (spesgn == -1)
        enchantment = -enchantment;

    if (!objects[typ].oc_enchantable)
        enchantment = 0;

    otmp->enchantment = enchantment;


    /* set charges */
    if (!objects[typ].oc_charged)
        charges = 0;
    else if (!chargesfound)
        charges = otmp->charges;
    else if (wiz_wishing)
    {
        ; /* no alteration to charges */
    }
    else if (oclass == WAND_CLASS
        || objects[typ].oc_charged
        )
    {
        int maxcharges = get_obj_max_charge(otmp);
        if (charges > maxcharges)
            charges = maxcharges;

        if (charges < 0)
            charges = 0;

        if (typ == WAN_WISHING && !wiz_wishing)
        {
            charges = (rn2(10) ? -1 : 0);
        }
    }
        
    otmp->charges = charges;


    /* set special_quality */
    if (is_otyp_key(typ) && typ == key_otyp)
    {
        otmp->special_quality = key_special_quality;
    }

    switch (typ) {
    case TIN:
        if (contents == CONTAINER_EMPTY) {
            otmp->corpsenm = NON_PM;
            otmp->special_quality = 0;
        } else if (contents == CONTAINER_SPINACH) {
            otmp->corpsenm = NON_PM;
            otmp->special_quality = 1;
        }
        break;
    case TOWEL:
        if (wetness)
            otmp->special_quality = wetness;
        break;
    case SKELETON_KEY:
        break;
    case SLIME_MOLD:
        otmp->special_quality = ftype;
    /* Fall through */
    case CHEST:
    case LARGE_BOX:
    case HEAVY_IRON_BALL:
    case IRON_CHAIN:
    case STATUE:
        /* otmp->cobj already done in mksobj() */
        break;
    case SCR_MAIL:
        /* 0: delivered in-game via external event (or randomly for fake mail);
           1: from bones or wishing; 2: written with marker */
        otmp->special_quality = 1;
        break;
    default:
        break;
    }

    /* set speflags */
    if (oclass == VENOM_CLASS)
        otmp->speflags |= SPEFLAGS_YOURS;


    /* set otmp->corpsenm or dragon scale [mail] */
    if (mntmp >= LOW_PM) {
        if (mntmp == PM_LONG_WORM_TAIL)
            mntmp = PM_LONG_WORM;
        else if (mntmp == PM_ELDER_LONG_WORM_TAIL)
            mntmp = PM_ELDER_LONG_WORM;

        switch (typ) {
        case TIN:
            otmp->special_quality = 0; /* No spinach */
            if (dead_species(mntmp, FALSE)) {
                otmp->corpsenm = NON_PM; /* it's empty */
            } else if ((!(mons[mntmp].geno & G_UNIQ) || wiz_wishing)
                       && !(mvitals[mntmp].mvflags & MV_NOCORPSE)
                       && mons[mntmp].cnutrit != 0) {
                otmp->corpsenm = mntmp;
            }
            break;
        case CORPSE:
            if ((!(mons[mntmp].geno & G_UNIQ) || wiz_wishing)
                && !(mvitals[mntmp].mvflags & MV_NOCORPSE)) {
                if (mons[mntmp].msound == MS_GUARDIAN)
                    mntmp = genus(mntmp, 1);
                set_corpsenm(otmp, mntmp);
            }
            break;
        case EGG:
            mntmp = can_be_hatched(mntmp);
            /* this also sets hatch timer if appropriate */
            set_corpsenm(otmp, mntmp);
            break;
        case FIGURINE:
            if (!(mons[mntmp].geno & G_UNIQ) && !is_human(&mons[mntmp])
                && mntmp != PM_MAIL_DAEMON
                )
                otmp->corpsenm = mntmp;
            break;
        case STATUE:
            otmp->corpsenm = mntmp;
            if (Has_contents(otmp) && verysmall(&mons[mntmp]))
                delete_contents(otmp); /* no spellbook */
            
            if(ishistoric)
                otmp->speflags |= SPEFLAGS_STATUE_HISTORIC;
            else
                otmp->speflags &= ~SPEFLAGS_STATUE_HISTORIC;

            break;
        case SCALE_MAIL:
            /* Dragon mail - depends on the order of objects & dragons. */
            if (mntmp >= PM_GRAY_DRAGON && mntmp <= PM_YELLOW_DRAGON)
                otmp->otyp = GRAY_DRAGON_SCALE_MAIL + mntmp - PM_GRAY_DRAGON;
            break;
        }
    }

    /* set material */
    if (material > 0)
    {
        if (material_wish_success(typ, material))
            otmp->material = material;
    }

    /* set eroded and erodeproof */
    if (erosion_matters(otmp)) {
        if (eroded && (is_flammable(otmp) || is_rustprone(otmp)))
            otmp->oeroded = eroded;
        if (eroded2 && (is_corrodeable(otmp) || is_rottable(otmp)))
            otmp->oeroded2 = eroded2;
        /*
         * 3.6.1: earlier versions included `&& !eroded && !eroded2' here,
         * but damageproof combined with damaged is feasible (eroded
         * armor modified by confused reading of cursed destroy armor)
         * so don't prevent player from wishing for such a combination.
         */
        if (erodeproof && (is_damageable(otmp) || otmp->otyp == CRYSKNIFE))
            otmp->oerodeproof = (Luck >= 0 || wiz_wishing);
    }

    /* set otmp->recharged */
    if (oclass == WAND_CLASS) {
        /* prevent wishing abuse */
        if (otmp->otyp == WAN_WISHING && !wiz_wishing)
            rechrg = 1;
        otmp->recharged = (char) rechrg;
    }

    /* set poisoned */
    if (ispoisoned) {
        if (is_poisonable(otmp))
            otmp->opoisoned = (Luck >= 0);
        else if (oclass == FOOD_CLASS)
            /* try to taint by making it as old as possible */
            otmp->age = 1L;
    }
    /* and [un]trapped */
    if (trapped) {
        if (Is_box(otmp) || typ == TIN)
            otmp->otrapped = (trapped == 1);
    }

    /* Set elemental enchantment */
    if (elemental_enchantment) 
    {
        if (is_elemental_enchantable(otmp))
            if(Luck >= 0)
                otmp->elemental_enchantment = (uchar)elemental_enchantment;

        if(otmp->elemental_enchantment == DEATH_ENCHANTMENT && !is_death_enchantable(otmp))
            otmp->elemental_enchantment = LIGHTNING_ENCHANTMENT;
    }

    /* Set exceptionality */
    if (exceptionality)
    {
        if (can_have_exceptionality(otmp) && otmp->oartifact == 0)
        {
            if(wiz_wishing)
                otmp->exceptionality = (uchar)exceptionality;
            else
            {
                boolean halfchance = !!(objects[otmp->otyp].oc_flags5 & O5_HALF_EXCEPTIONALITY_CHANCE);
                boolean doublechance = !!(objects[otmp->otyp].oc_flags5 & O5_DOUBLE_EXCEPTIONALITY_CHANCE);
                if (is_weapon(otmp))
                {
                    if (exceptionality < EXCEPTIONALITY_ELITE)
                        otmp->exceptionality = (uchar)exceptionality;
                    else if (exceptionality == EXCEPTIONALITY_ELITE)
                        otmp->exceptionality = (uchar)(((rn2(100) < (halfchance ? 40 : doublechance ? 100 : 80)) && (Luck >= 0 || doublechance)) ? exceptionality : EXCEPTIONALITY_NORMAL);
                    else
                        otmp->exceptionality = (uchar)((!rn2(halfchance ? 8 : doublechance ? 2 : 4) && Luck >= 0) ? exceptionality : EXCEPTIONALITY_NORMAL);
                }
                else
                {
                    if (exceptionality < EXCEPTIONALITY_ELITE)
                        otmp->exceptionality = (uchar)((doublechance || (!rn2(2) && Luck >= 0)) ? exceptionality : EXCEPTIONALITY_NORMAL);
                    else if (exceptionality == EXCEPTIONALITY_ELITE)
                        otmp->exceptionality = (uchar)((!rn2(halfchance ? 8 : doublechance ? 2 : 4) && Luck >= 0) ? exceptionality : EXCEPTIONALITY_NORMAL);
                    else
                        otmp->exceptionality = (uchar)((!rn2(halfchance ? 40 : doublechance ? 10 : 20) && Luck >= 0) ? exceptionality : EXCEPTIONALITY_NORMAL);
                }
            }

            if (((objects[otmp->otyp].oc_flags5 & O5_CANNOT_BE_CELESTIAL) || (objects[otmp->otyp].oc_flags2 & (O2_DEMON_ITEM | O2_UNDEAD_ITEM))) && otmp->exceptionality == EXCEPTIONALITY_CELESTIAL)
                otmp->exceptionality = EXCEPTIONALITY_ELITE;
            else if (((objects[otmp->otyp].oc_flags5 & O5_CANNOT_BE_PRIMORDIAL) || (objects[otmp->otyp].oc_flags2 & (O2_DEMON_ITEM | O2_ANGELIC_ITEM))) && otmp->exceptionality == EXCEPTIONALITY_PRIMORDIAL)
                otmp->exceptionality = EXCEPTIONALITY_ELITE;
            else if (((objects[otmp->otyp].oc_flags5 & O5_CANNOT_BE_INFERNAL) || (objects[otmp->otyp].oc_flags2 & (O2_ANGELIC_ITEM)) || otmp->material == MAT_SILVER) && otmp->exceptionality == EXCEPTIONALITY_INFERNAL)
                otmp->exceptionality = EXCEPTIONALITY_ELITE;
        }
    }

    /* Set mythic quality */
    if (can_obj_have_mythic(otmp) && (mythic_prefix || mythic_suffix) && otmp->oartifact == 0)
    {
        if (mythic_prefix < 0 || mythic_suffix < 0)
        {
            uchar dummy_prefix = 0, dummy_suffix = 0;
            boolean force_legendary = ((mythic_prefix == -2 || mythic_suffix == -2) && (wiz_wishing || (!rn2(3) && Luck >= 0)));
            randomize_mythic_quality(otmp, force_legendary ? 2 : 1, &dummy_prefix, &dummy_suffix);
            if(mythic_prefix < 0)
                mythic_prefix = (int)dummy_prefix;
            if (mythic_suffix < 0)
                mythic_suffix = (int)dummy_suffix;
        }
        else
        {
            if (mythic_prefix && mythic_suffix && !(wiz_wishing || (!rn2(3) && Luck >= 0)))
            {
                if (!rn2(2))
                    mythic_prefix = 0;
                else
                    mythic_suffix = 0;
            }
        }

        if (mythic_prefix < 0)
            mythic_prefix = 0;
        if (mythic_suffix < 0)
            mythic_suffix = 0;
        
        uchar affix_idx;
        for (affix_idx = 0; affix_idx <= 1; affix_idx++)
        {
            struct mythic_definition* mythic_definitions = (affix_idx == 0 ? mythic_prefix_qualities : mythic_suffix_qualities);
            int mythic_quality = (affix_idx == 0 ? mythic_prefix : mythic_suffix);
            uchar* otmp_mythic_quality_ptr = (affix_idx == 0 ? &otmp->mythic_prefix : &otmp->mythic_suffix);

            if (is_mythic_affix_ok(affix_idx, (uchar)mythic_quality, otmp, wiz_wishing ? 0 : 1))
            {
                if (wiz_wishing || (mythic_definitions[mythic_quality].mythic_flags & MYTHIC_FLAG_DIRECTLY_WISHABLE))
                    *otmp_mythic_quality_ptr = (uchar)mythic_quality;
                else
                {
                    if ((mythic_definitions[mythic_quality].mythic_flags & MYTHIC_FLAG_NON_WISHABLE))
                        *otmp_mythic_quality_ptr = 0;
                    else if (!rn2(3) && Luck >= 0)
                        *otmp_mythic_quality_ptr = (uchar)mythic_quality;
                    else
                        *otmp_mythic_quality_ptr = 0;
                }
            }
        }
    }

    /* set blessed/cursed -- setting the fields directly is safe
 * since weight() is called below and addinv() will take care
 * of luck */
    if (iscursed) {
        curse(otmp);
    }
    else if (uncursed) {
        otmp->blessed = 0;
        otmp->cursed = (Luck < 0 && !wiz_wishing && !is_obj_uncurseable(otmp));
    }
    else if (blessed) {
        otmp->blessed = (Luck >= 0 || wiz_wishing);
        otmp->cursed = (Luck < 0 && !wiz_wishing && !is_obj_uncurseable(otmp));
    }
    else if (spesgn < 0) {
        curse(otmp);
    }

    /* empty for containers rather than for tins */
    if (contents == CONTAINER_EMPTY) {
        if (otmp->otyp == BAG_OF_TRICKS || otmp->otyp == HORN_OF_PLENTY) {
            if (otmp->charges > 0)
                otmp->charges = 0;
        } else if (Has_contents(otmp)) {
            /* this assumes that artifacts can't be randomly generated
               inside containers */
            delete_contents(otmp);
            otmp->owt = weight(otmp);
        }
    }
    /* set locked/unlocked/broken */
    if (Is_box(otmp)) {
        if (locked) {
            otmp->olocked = 1, otmp->obroken = 0;
        } else if (unlocked) {
            otmp->olocked = 0, otmp->obroken = 0;
        } else if (broken) {
            otmp->olocked = 0, otmp->obroken = 1;
        }
    }

    if (isgreased)
        otmp->greased = 1;

    if (isdiluted && otmp->oclass == POTION_CLASS && otmp->otyp != POT_WATER)
        otmp->odiluted = 1;

    /* set tin variety */
    if (otmp->otyp == TIN && tvariety >= 0 && (rn2(4) || wiz_wishing))
        set_tin_variety(otmp, tvariety);

    if (name) {
        const char *aname;
        short objtyp;

        /* an artifact name might need capitalization fixing */
        aname = artifact_name(name, &objtyp);
        if (aname && objtyp == otmp->otyp)
            name = aname;

        /* 3.6 tribute - fix up novel */
        if (objects[otmp->otyp].oc_class == SPBOOK_CLASS && objects[otmp->otyp].oc_subtyp == BOOKTYPE_NOVEL) {
            const char *novelname;

            novelname = lookup_novel(name, &otmp->novelidx);
            if (novelname)
                name = novelname;

            otmp = oname(otmp, name);
        }
        else if (objects[otmp->otyp].oc_class == SPBOOK_CLASS && objects[otmp->otyp].oc_subtyp == BOOKTYPE_MANUAL) {
            const char* manualname;

            manualname = lookup_manual(name, &otmp->manualidx);
            if (manualname)
                name = manualname;

            otmp = oname(otmp, name);
        }
        else if(name == aname)
        {
            otmp = oname(otmp, name);
            /* name==aname => wished for artifact (otmp->oartifact => got it) */
            if (otmp->oartifact) {
                otmp->quan = 1L;
                u.uconduct.wisharti++; /* KMH, conduct */
            }
        }
        else
        {
            otmp = uoname(otmp, name);
        }
    }

    /* more wishing abuse: don't allow wishing for certain artifacts */
    /* and make them pay; charge them for the wish anyway! */
    if ((is_quest_artifact(otmp)
         || (otmp->oartifact && rn2(nartifact_exist()) > 1)) && !wiz_wishing)
    {
        artifact_exists(otmp, safe_oname(otmp), FALSE);
        obfree(otmp, (struct obj *) 0);
        otmp = (struct obj *) &zeroobj;
        pline("For a moment, you feel %s in your %s, but it disappears!",
              something, makeplural(body_part(HAND)));
        return otmp;
    }

    if (halfeaten && otmp->oclass == FOOD_CLASS) {
        if (otmp->otyp == CORPSE)
            otmp->oeaten = mons[otmp->corpsenm].cnutrit;
        else
            otmp->oeaten = objects[otmp->otyp].oc_nutrition;
        /* (do this adjustment before setting up object's weight) */
        consume_oeaten(otmp, 1);
    }
    otmp->owt = weight(otmp);
    if (very && otmp->otyp == HEAVY_IRON_BALL)
        otmp->owt += IRON_BALL_W_INCR;
    else if (gsize > 1 && otmp->globby)
        /* 0: unspecified => small; 1: small => keep default owt of 20;
           2: medium => 120; 3: large => 320; 4: very large => 520 */
        otmp->owt += 100 + (gsize - 2) * 200;

    return otmp;
}

int
rnd_class(first, last)
int first, last;
{
    int i, x, sum = 0;

    if (first == last)
        return first;
    for (i = first; i <= last; i++)
        sum += objects[i].oc_prob;
    if (!sum) /* all zero */
        return first + rn2(last - first + 1);
    x = rnd(sum);
    for (i = first; i <= last; i++)
        if (objects[i].oc_prob && (x -= objects[i].oc_prob) <= 0)
            return i;
    return 0;
}

STATIC_OVL const char *
Japanese_item_name(i)
int i;
{
    struct Jitem *j = Japanese_items;

    while (j->item) {
        if (i == j->item)
            return j->name;
        j++;
    }
    return (const char *) 0;
}

const char *
suit_simple_name(suit)
struct obj *suit;
{
    const char *suitnm, *esuitp;

    if (suit) {
        if (is_dragon_mail(suit))
            return "dragon mail"; /* <color> dragon scale mail */
        else if (is_dragon_scales(suit))
            return "dragon scales";
        suitnm = OBJ_NAME(objects[suit->otyp]);
        esuitp = eos((char *) suitnm);
        if (strlen(suitnm) > 5 && !strcmp(esuitp - 5, " mail"))
            return "mail"; /* most suits fall into this category */
        else if (strlen(suitnm) > 7 && !strcmp(esuitp - 7, " jacket"))
            return "jacket"; /* leather jacket */
    }
    /* "suit" is lame but "armor" is ambiguous and "body armor" is absurd */
    return "suit";
}

const char *
cloak_simple_name(cloak)
struct obj *cloak;
{
    if (cloak) {
        switch (cloak->otyp) {
        case ALCHEMY_SMOCK:
            return (objects[cloak->otyp].oc_name_known && cloak->dknown)
                       ? "smock"
                       : "apron";
        default:
            break;
        }
    }
    return "cloak";
}

const char*
robe_simple_name(robe)
struct obj* robe;
{
    if (robe) {
        switch (robe->otyp) {
        case MEDIEVAL_ROBE: //special types here
            return "robe";
        case MUMMY_WRAPPING:
            return "wrapping";
        default:
            break;
        }
    }
    return "robe";
}

const char*
miscitem_simple_name(miscitem)
struct obj* miscitem;
{
    if (!miscitem)
        return "invalid miscellaneous item";

    return "miscellaneous item";
}


const char*
armor_class_simple_name(item)
struct obj* item;
{
    if (!item)
        return "armor";

    if (is_suit(item))
        return suit_simple_name(item);
    else if (is_robe(item))
        return robe_simple_name(item);
    else if (is_cloak(item))
        return cloak_simple_name(item);
    else if (is_helmet(item))
        return helm_simple_name(item);
    else if(objects[item->otyp].oc_subtyp >= 0 && objects[item->otyp].oc_subtyp < MAX_ARMOR_TYPES)
        return armor_type_names[objects[item->otyp].oc_subtyp];
    else
        return "armor";

}


/* helm vs hat for messages */
const char *
helm_simple_name(helmet)
struct obj *helmet;
{
    /*
     *  There is some wiggle room here; the result has been chosen
     *  for consistency with the "protected by hard helmet" messages
     *  given for various bonks on the head:  headgear that provides
     *  such protection is a "helm", that which doesn't is a "hat".
     *
     *      elven leather helm / leather hat    -> hat
     *      dwarvish iron helm / hard hat       -> helm
     *  The rest are completely straightforward:
     *      fedora, cornuthaum, dunce cap       -> hat
     *      all other types of helmets          -> helm
     */
    return (helmet && !is_metallic(helmet)) ? "hat" : "helm";
}

const char *
mimic_obj_name(mtmp)
struct monst *mtmp;
{
    if (M_AP_TYPE(mtmp) == M_AP_OBJECT) {
        if (mtmp->mappearance == GOLD_PIECE)
            return "gold";
        if (mtmp->mappearance != STRANGE_OBJECT)
            return simple_typename(mtmp->mappearance);
    }
    return "whatcha-may-callit";
}

/*
 * Construct a query prompt string, based around an object name, which is
 * guaranteed to fit within [QBUFSZ].  Takes an optional prefix, three
 * choices for filling in the middle (two object formatting functions and a
 * last resort literal which should be very short), and an optional suffix.
 */
char *
safe_qbuf(qbuf, qprefix, qsuffix, obj, func, altfunc, lastR)
char *qbuf; /* output buffer */
const char *qprefix, *qsuffix;
struct obj *obj;
char *FDECL((*func), (OBJ_P)), *FDECL((*altfunc), (OBJ_P));
const char *lastR;
{
    if (!qbuf)
        return qbuf;

    char *bufp;
    /* convert size_t (or int for ancient systems) to ordinary unsigned */
    size_t len, lenlimit,
        len_qpfx = (qprefix ? strlen(qprefix) : 0),
        len_qsfx = (qsuffix ? strlen(qsuffix) : 0),
        len_lastR = strlen(lastR);

    lenlimit = QBUFSZ - 1;
    /* sanity check, aimed mainly at paniclog (it's conceivable for
       the result of short_oname() to be shorter than the length of
       the last resort string, but we ignore that possibility here) */
    if (len_qpfx > lenlimit)
        impossible("safe_qbuf: prefix too long (%zu characters).", len_qpfx);
    else if (len_qpfx + len_qsfx > lenlimit)
        impossible("safe_qbuf: suffix too long (%zu + %zu characters).",
                   len_qpfx, len_qsfx);
    else if (len_qpfx + len_lastR + len_qsfx > lenlimit)
        impossible("safe_qbuf: filler too long (%zu + %zu + %zu characters).",
                   len_qpfx, len_lastR, len_qsfx);

    /* the output buffer might be the same as the prefix if caller
       has already partially filled it */
    if (qbuf == qprefix) 
    {
        /* prefix is already in the buffer */
        qbuf[lenlimit] = '\0';
    } 
    else if (qprefix) 
    {
        /* put prefix into the buffer */
        (void) strncpy(qbuf, qprefix, lenlimit);
        qbuf[lenlimit] = '\0';
    }
    else 
    {
        /* no prefix; output buffer starts out empty */
        qbuf[0] = '\0';
    }


    len = strlen(qbuf);

    if (len + len_lastR + len_qsfx > lenlimit)
    {
        /* too long; skip formatting, last resort output is truncated */
        if (len < lenlimit) 
        {
            (void) strncpy(&qbuf[len], lastR, lenlimit - len);
            qbuf[lenlimit] = '\0';
            len = strlen(qbuf);
            if (qsuffix && len < lenlimit)
            {
                (void) strncpy(&qbuf[len], qsuffix, lenlimit - len);
                qbuf[lenlimit] = '\0';
                /* len = (unsigned) strlen(qbuf); */
            }
        }
    } 
    else 
    {
        /* suffix and last resort are guaranteed to fit */
        len += len_qsfx; /* include the pending suffix */
        /* format the object */
        bufp = short_oname(obj, func, altfunc, lenlimit - len);
        if (len + strlen(bufp) <= lenlimit)
            Strcat(qbuf, bufp); /* formatted name fits */
        else
            Strcat(qbuf, lastR); /* use last resort */
        releaseobuf(bufp);

        if (qsuffix)
            Strcat(qbuf, qsuffix);
    }
    /* assert( strlen(qbuf) < QBUFSZ ); */
    return qbuf;
}


const char*
get_key_special_quality_description(obj)
struct obj* obj;
{
    if (!obj || !is_key(obj) || obj->special_quality == 0)
        return "";

    int otyp = obj->otyp;
    int sq = obj->special_quality;

    return get_key_special_quality_description_by_otyp(otyp, sq);
}


struct key_special_description {
    int otyp;
    int special_quality;
    const char* description;
    const char* lock_description;
};

struct key_special_description key_special_descriptions[] =
{
    {MAGIC_KEY, 1, "enchanted", "enchanted"},
    {MAGIC_KEY, 2, "scintillating", "scintillating"},
    {MAGIC_KEY, 3, "glowing", "glowing"},
    {MAGIC_KEY, 4, "shining", "shining"},
    {MAGIC_KEY, 5, "iridescent", "iridescent"},
    {GEOMETRIC_KEY, 1, "spherical", "spherical"},
    {GEOMETRIC_KEY, 2, "linear", "linear"},
    {GEOMETRIC_KEY, 3, "triangular", "triangular"},
    {GEOMETRIC_KEY, 4, "square", "square"},
    {GEOMETRIC_KEY, 5, "pentagonal", "pentagonal"},
    {GEOMETRIC_KEY, 6, "hexagonal", "hexagonal"},
    {GEOMETRIC_KEY, 7, "heptagonal", "heptagonal"},
    {GEOMETRIC_KEY, 8, "octagonal", "octagonal"},
    {ORNAMENTAL_KEY, 1, "runed", "runed"},
    {ORNAMENTAL_KEY, 2, "decorated", "decorated"},
    {ORNAMENTAL_KEY, 3, "grim", "grim"},         /* Dispater */
    {ORNAMENTAL_KEY, 4, "skull-headed", "skull-shaped"}, /* Orcus */
    {ORNAMENTAL_KEY, 5, "fly-shaped", "fly-shaped"},   /* Baalzebub */
    {ORNAMENTAL_KEY, 6, "hyena-headed", "hyena-shaped"}, /* Yeenaghu */
    {0, 0, 0}
};

boolean
find_key_otyp_by_description(inbuf, otyp_ptr, sq_ptr)
char * inbuf;
int *otyp_ptr, *sq_ptr;
{
    if (strstr(inbuf, " key") == 0)
        return FALSE;

    int i;
    for (i = 0; key_special_descriptions[i].otyp > STRANGE_OBJECT; i++)
    {
        char buf[BUFSZ];
        size_t ln = 0;
        strcpy(buf, key_special_descriptions[i].description);
        Strcat(buf, " ");
        ln = strlen(buf);
        if (!strncmpi(inbuf, buf, ln) && strstr(inbuf, OBJ_NAME(objects[key_special_descriptions[i].otyp])) != 0)
        {
            *otyp_ptr = key_special_descriptions[i].otyp;
            *sq_ptr = key_special_descriptions[i].special_quality;
            return TRUE;
        }
    }
    return FALSE;
}

const char*
get_key_special_quality_description_by_otyp(otyp, sq)
int otyp, sq;
{
    int i;
    for (i = 0; key_special_descriptions[i].otyp > STRANGE_OBJECT; i++)
        if (key_special_descriptions[i].otyp == otyp && key_special_descriptions[i].special_quality == sq)
            return key_special_descriptions[i].description;

    return "";
}

const char*
get_lock_description_by_otyp(otyp, sq, normally_without_lock)
int otyp, sq;
boolean normally_without_lock;
{
    if (Blind)
        return "";

    if (otyp < NON_PM || otyp >= NUM_OBJECTS) /* No lock, STRANGE_OBJECT indicates normal door */
    {
        if (normally_without_lock)
            return "";
        else
            return "no";
    }

    if (otyp == NON_PM)
        otyp = 0;

    if (!is_otyp_key(otyp))
        return "";

    if (sq > 0)
    {
        int i;
        for (i = 0; key_special_descriptions[i].otyp > STRANGE_OBJECT; i++)
            if (key_special_descriptions[i].otyp == otyp && key_special_descriptions[i].special_quality == sq)
                return key_special_descriptions[i].lock_description;

        return "";
    }
    else
        return OBJ_CONTENT_NAME(otyp);
}

void
print_lock_with_buf(lbuf, key_otyp, special_quality, normally_without_lock)
char* lbuf;
int key_otyp, special_quality;
boolean normally_without_lock;
{
    if (!lbuf)
        return;

    const char* ldesc = get_lock_description_by_otyp(key_otyp, special_quality, normally_without_lock);
    if (ldesc && strcmp(ldesc, ""))
    {
        if (!strcmp(ldesc, "no"))
            Sprintf(lbuf, " with no lock");
        else
            Sprintf(lbuf, " with %s lock", an(ldesc));
    }
}

STATIC_OVL
boolean material_wish_success(otyp, material)
int otyp, material;
{
    if (otyp <= STRANGE_OBJECT || otyp >= NUM_OBJECTS)
        return FALSE;

    if (material == objects[otyp].oc_material)
        return TRUE; /* base material always works */

    int mit = objects[otyp].oc_material_init_type;
    if (mit <= MATINIT_NORMAL || mit >= MAX_MATINIT_TYPES)
        return FALSE;

    int i;
    for (i = 0; i < MAX_WISHING_MATERIALS && material_wishing_definitions[mit].material[i] > 0; i++)
    {
        if (material == material_wishing_definitions[mit].material[i])
        {
            return wizard || (rn2(100) < material_wishing_definitions[mit].probability[i]);
        }
    }
    return FALSE;
}

/*objnam.c*/
