/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    mondata.c    $NHDT-Date: 1550525093 2019/02/18 21:24:53 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.72 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

/*
 *      These routines provide basic data for any type of monster.
 */

struct ability_conversion_table {
    int prop;
    uint64_t innate;
    uint64_t innate2;
    uint64_t conveyed;
    int adtype;
};


STATIC_VAR const NEARDATA struct ability_conversion_table prop2innate[] = {
    { FIRE_IMMUNITY, MR_FIRE, MR2_NONE, MC_NONE, AD_FIRE },
    { COLD_IMMUNITY, MR_COLD, MR2_NONE, MC_NONE, AD_COLD },
    { SLEEP_RESISTANCE, MR_SLEEP, MR2_NONE, MC_SLEEP, AD_SLEE },
    { DISINTEGRATION_RESISTANCE, MR_DISINT, MR2_NONE, MC_DISINT, AD_DISN },
    { SHOCK_IMMUNITY, MR_ELEC, MR2_NONE, MC_NONE, AD_ELEC },
    { POISON_RESISTANCE, MR_POISON, MR2_NONE, MC_POISON, AD_DRST },
    { ACID_IMMUNITY, MR_ACID, MR2_NONE, MC_NONE, AD_ACID  },
    { STONE_RESISTANCE, MR_STONE, MR2_NONE, MC_STONE, AD_STON  },
    { DEATH_RESISTANCE, MR_DEATH, MR2_NONE, MC_DEATH, AD_DETH },
    { LYCANTHROPY_RESISTANCE, MR_LYCANTHROPY, MR2_NONE, MC_NONE, AD_WERE },
    { MAGIC_MISSILE_IMMUNITY, MR_MAGIC_MISSILE, MR2_NONE, MC_NONE, AD_MAGM },
    { STUN_RESISTANCE, MR_STUN, MR2_NONE, MC_NONE, AD_STUN },
    { ANTIMAGIC, MR_MAGIC, MR2_NONE, MC_NONE, -1  },
    { CHARM_RESISTANCE, MR_CHARM, MR2_NONE, MC_CHARM, AD_SSEX },
    { CHARM_RESISTANCE, MR_CHARM, MR2_NONE, MC_CHARM, AD_SEDU }, /* Doubles for AD_SEDU*/
    { HALLUC_RES, MR_NONE, MR2_NONE, MC_NONE, AD_HALU },
    { FEAR_RESISTANCE, MR_FEAR, MR2_NONE, MC_NONE, -1 },
    { SICK_RESISTANCE, MR_SICK, MR2_NONE, MC_SICK, AD_DISE },
    { SICK_RESISTANCE, MR_SICK, MR2_NONE, MC_SICK, AD_ROTS }, /* Doubles for AD_ROTS */
    { DRAIN_RESISTANCE, MR_DRAIN, MR2_NONE, MC_DRAIN, AD_DRLI },
    { FLASH_RESISTANCE, MR_FLASH, MR2_NONE, MC_NONE, AD_BLND },
    { REFLECTING, MR_REFLECTING, MR2_NONE, MC_NONE, -1 },
    { INVISIBILITY, MR_INVISIBLE, MR2_NONE, MC_INVISIBILITY, -1 },
    { SEE_INVISIBLE, MR_SEE_INVISIBLE, MR2_NONE, MC_SEE_INVISIBLE, -1 },
    { REGENERATION, MR_REGENERATION, MR2_NONE, MC_NONE, -1 },
    { TELEPORT, MR_TELEPORT, MR2_NONE, MC_TELEPORT, -1 },
    { TELEPORT_CONTROL, MR_TELEPORT_CONTROL, MR2_NONE, MC_TELEPORT_CONTROL, -1 },
    { BLIND_TELEPATHY, MR_BLIND_TELEPATHY, MR2_NONE, MC_BLIND_TELEPATHY, -1 },
    { TELEPAT, MR_TELEPATHY, MR2_NONE, MC_TELEPATHY, -1 },
    { FREE_ACTION, MR_FREE_ACTION, MR2_NONE, MC_FREE_ACTION, AD_PLYS },
    { LEVITATION_CONTROL, MR_LEVITATION_CONTROL, MR2_NONE, MC_NONE, -1 },
    { FIRE_VULNERABILITY, MR_FIRE_VULNERABILITY, MR2_NONE, MC_NONE, -1 },
    { COLD_VULNERABILITY, MR_COLD_VULNERABILITY, MR2_NONE, MC_NONE, -1 },
    { SHOCK_VULNERABILITY, MR_SHOCK_VULNERABILITY, MR2_NONE, MC_NONE, -1 },
    { MAGIC_MISSILE_VULNERABILITY, MR_MAGIC_MISSILE_VULNERABILITY, MR2_NONE, MC_NONE, -1 },
    { FIRE_RESISTANCE, MR_NONE, MR2_FIRE_RESISTANCE, MC_FIRE, -1 },
    { COLD_RESISTANCE, MR_NONE, MR2_COLD_RESISTANCE, MC_COLD, -1 },
    { SHOCK_RESISTANCE, MR_NONE, MR2_SHOCK_RESISTANCE, MC_ELEC, -1 },
    { MAGIC_MISSILE_RESISTANCE, MR_NONE, MR2_MAGIC_MISSILE_RESISTANCE, MC_NONE, -1 },
    { ACID_RESISTANCE, MR_NONE, MR2_ACID_RESISTANCE, MC_ACID, -1 },
    { CANCELLATION_RESISTANCE, MR_NONE, MR2_CANCELLATION_RESISTANCE, MC_NONE, -1 },
    { BISECTION_RESISTANCE, MR_NONE, MR2_BISECTION_RESISTANCE, MC_NONE, -1 },
    { SLIME_RESISTANCE, MR_NONE, MR2_SLIME_RESISTANCE, MC_NONE, -1 },
    { ENERGY_REGENERATION, MR_NONE, MR2_ENERGY_REGENERATION, MC_NONE, -1 },
    { WOUNDING_RESISTANCE, MR_NONE, MR2_WOUNDING_RESISTANCE, MC_NONE, -1 },
    { POLYMORPH_RESISTANCE, MR_NONE, MR2_POLYMORPH_RESISTANCE, MC_NONE, -1 },
};

struct mflag_description {
    uint64_t mflag_bit;
    const char* singular_adjective;
    const char* plural_noun;
};

STATIC_VAR const NEARDATA struct mflag_description m1flag_descriptions[] = {
    { M1_AMORPHOUS, "amorphous", "amorphous monsters" },
    { M1_AMPHIBIOUS, "amphibious", "amphibious monsters" },
    { M1_BREATHLESS, "breathless", "breathless monsters" },
    { M1_CARNIVORE, "carnivore", "carnivores" },
    { M1_CLING, "clinging", "clingers" },
    { M1_CONCEAL, "concealing", "concealed monster" },
    { M1_FLY, "flying", "flying monsters" },
    { M1_FLOATER, "floater", "floating monsters" },
    { M1_TELEKINETIC_OPERATOR, "telekinetically operating", "telekinetically operating monsters" },
    { M1_HERBIVORE, "herbivore", "herbivores" },
    { M1_HIDE, "hiding", "hidden monster" },
    { M1_HUMANOID, "humanoid", "humanoids" },
    { M1_ANIMAL, "animal", "animals" },
    { M1_STEED, "steed", "steeds" },
    { M1_METALLIVORE, "metallivore", "metallivores" },
    { M1_RUST_CAUSING_IRONVORE, "rust-causing ironvore", "rust-causing ironvores" },
    { M1_MINDLESS, "mindless", "mindless monsters" },
    { M1_NOEYES, "eyeless", "eyeless monsters" },
    { M1_NOHANDS, "handless", "handless monsters" },
    { M1_NONECK, "neckless", "neckless monsters" },
    { M1_NOLIMBS, "limbless", "limbless monsters" },
    { M1_OMNIVORE, "omnivore", "omnivores" },
    { M1_OVIPAROUS, "oviparious", "oviparious monsters" },
    { M1_SLITHY, "slithy", "slithing monsters" },
    { M1_SWIM, "swimming", "swimming monsters" },
    { M1_THICK_HIDE, "thick hide", "thick-hided monsters" },
//    { M1_TUNNEL, "tunneling", "tunneling monsters" },
    { M1_UNSOLID, "unsolid", "unsolid monsters" },
    { M1_WALLWALK, "passes through walls", "monsters that can pass through walls" },
    { 0 , "", "" }
};

STATIC_VAR const NEARDATA struct mflag_description m2flag_descriptions[] = {
    { M2_DEMON, "demon", "demons" },
    { M2_ANGEL, "angelic being", "angelic beings" },
    { M2_DWARF, "dwarf", "dwarves" },
    { M2_ELF, "elf", "elves" },
    { M2_GIANT, "giant", "giants" },
    { M2_GNOLL, "gnoll", "gnolls" },
    { M2_GNOME, "gnome", "gnomes" },
    { M2_HUMAN, "human", "human beings" },
    { M2_TENTACLED_ONE, "tentacled one", "tentacled ones" },
    { M2_MODRON, "modron", "modrons" },
    { M2_MIMIC, "mimic", "mimics" },
    { M2_ELEMENTAL, "elemental", "elementals" },
    { M2_COCKATRICE, "cockatrice", "cockatrices" },
    { M2_ORC, "orc", "orcs" },
    { M2_SHAPESHIFTER, "shapeshifter", "shapeshifters" },
    { M2_UNDEAD, "undead", "undead" },
    { M2_WERE, "lycanthrope", "lycanthropes" },
    { 0 , "", "" }
};

STATIC_VAR const NEARDATA struct mflag_description m3flag_descriptions[] = {
    { M3_CONSTRICTOR, "constrictor", "constrictors" },
    { M3_INFRAVISION, "infravision", "monsters with infravision" },
    { M3_INCORPOREAL, "incorporeal", "incorporeal monsters" },
    { M3_NONLIVING, "nonliving", "nonliving monster" },
    { M3_REGENERATES_LOST_BODY_PARTS, "regenerates lost body parts", "monsters that regenerate lost body parts" },
    { M3_REVIVES_FROM_DEAD, "revives from dead", "monsters that revive from dead" },
    { M3_SPEAKING, "speaking", "speaking monsters" },
    { 0 , "", "" }
};

STATIC_VAR const NEARDATA struct mflag_description m4flag_descriptions[] = {
    { M4_PITWALK, "pitwalking", "pitwalkers" },
    { M4_SLURPS_ITEMS, "item-engulfing", "item-engulfing monsters" },
    { M4_STONY, "stony", "monsters made of stone" },
    { M4_WOODEN, "wooden", "monsters made of wood" },
    { M4_FLAMING, "flaming", "flaming monsters" },
    { M4_FLAMING, "whirly", "whirly monsters" },
    { M4_BAT, "bat", "bats" },
    { M4_BEAR, "bear or bear-like monster", "bears or bear-like monsters" },
    { M4_BIRD, "bird or bird-like monster", "birds or bird-like monsters" },
    { M4_VORPAL_VULNERABILITY, "vorpal vulnerable", "monsters vulnerable to vorpal weapons" },
    { M4_LORD, "lord", "lords" },
    { M4_PRINCE, "prince", "princes" },
    { M4_IRON, "iron", "monsters made of iron" },
    { M4_METAL, "metal", "monsters made of metal other than iron" },
    { 0 , "", "" }
};

STATIC_VAR const NEARDATA struct mflag_description m5flag_descriptions[] = {
    { M5_HATES_LIGHT, "vulnerable to light", "monsters vulnerable to light" },
    { 0 , "", "" }
};

STATIC_VAR const NEARDATA struct mflag_description m6flag_descriptions[] = {
    { M6_NON_EATER, "non-eating living monster", "non-eating living monsters" },
    { M6_CORPSE_EATER, "corpse-eating monster", "corpse-eating monsters" },
    { M6_LITHOVORE, "lithovore", "lithovores" },
    { M6_SHADE, "impervious to physical weapons", "monsters impervious to physical weapons" },
    { M6_TELEPORT_HEAL_TACTICS, "using teleport and heal tactics", "monsters using teleport and heal tactics" },
    { M6_ABLE_TO_WEAR_OBJECTS, "item wearing and using monster", "item wearing and using monsters" },
    { 0 , "", "" }
};

STATIC_VAR const NEARDATA struct mflag_description m7flag_descriptions[] = {
    { M7_ARCHAEOLOGIST, "archaeologist", "archaeologists" },
    { M7_BARBARIAN, "barbarian", "barbarians" },
    { M7_CAVEMAN, "cavaman", "cavamen" },
    { M7_HEALER, "healer", "healers" },
    { M7_KNIGHT, "knight", "knights" },
    { M7_MONK, "monk", "monks" },
    { M7_PRIEST, "priest", "priests" },
    { M7_ROGUE, "rogue", "rogues" },
    { M7_RANGER, "ranger", "rangers" },
    { M7_SAMURAI, "samurai", "samurai" },
    { M7_TOURIST, "tourist", "tourists" },
    { M7_VALKYRIE, "valkyrie", "valkyries" },
    { M7_WIZARD, "wizard", "wizards" },
    { 0 , "", "" }
};

STATIC_VAR const NEARDATA struct mflag_description m8flag_descriptions[] = {
    { 0 , "", "" }
};

const char*
get_mflag_description(mflag_bit, plural, mindex)
uint64_t mflag_bit;
boolean plural;
uchar mindex;
{
    const struct mflag_description* mtable = m1flag_descriptions;

    if (mindex == 2)
        mtable = m2flag_descriptions;
    else if (mindex == 3)
        mtable = m3flag_descriptions;
    else if (mindex == 4)
        mtable = m4flag_descriptions;
    else if (mindex == 5)
        mtable = m5flag_descriptions;
    else if (mindex == 6)
        mtable = m6flag_descriptions;
    else if (mindex == 7)
        mtable = m7flag_descriptions;
    else if (mindex == 8)
        mtable = m8flag_descriptions;

    for (int i = 0; mtable[i].mflag_bit != 0 ; i++)
    {
        if (mtable[i].mflag_bit == mflag_bit)
            return plural ? mtable[i].plural_noun : mtable[i].singular_adjective;
    }

    return empty_string;
}


uint64_t
prop_to_conveyed(prop_index)
int prop_index;
{
    for (int i = 0; i < SIZE(prop2innate); i++)
    {
        if (prop2innate[i].prop == prop_index)
            return prop2innate[i].conveyed;
    }
    return 0;
}

int
conveyed_to_prop(conveyed_bit)
uint64_t conveyed_bit;
{
    for (int i = 0; i < SIZE(prop2innate); i++)
    {
        if (prop2innate[i].conveyed == conveyed_bit)
            return prop2innate[i].prop;
    }
    return 0;
}



uint64_t
prop_to_innate(prop_index)
int prop_index;
{
    for (int i = 0; i < SIZE(prop2innate); i++)
    {
        if (prop2innate[i].prop == prop_index)
            return prop2innate[i].innate;
    }
    return 0;
}

uint64_t
prop_to_innate2(prop_index)
int prop_index;
{
    for (int i = 0; i < SIZE(prop2innate); i++)
    {
        if (prop2innate[i].prop == prop_index)
            return prop2innate[i].innate2;
    }
    return 0;
}


int
innate_to_prop(innate_bit)
uint64_t innate_bit;
{
    for (int i = 0; i < SIZE(prop2innate); i++)
    {
        if (prop2innate[i].innate == innate_bit)
            return prop2innate[i].prop;
    }
    return 0;
}

int
innate2_to_prop(innate2_bit)
uint64_t innate2_bit;
{
    for (int i = 0; i < SIZE(prop2innate); i++)
    {
        if (prop2innate[i].innate2 == innate2_bit)
            return prop2innate[i].prop;
    }
    return 0;
}


/* set up an individual monster's base type (initial creation, shapechange) */
void
set_mon_data(mon, ptr, subtype)
struct monst *mon;
struct permonst *ptr;
unsigned short subtype;
{
    if (!mon || !ptr)
        return;

    int new_speed, old_speed = mon->data ? mon->data->mmove : 0;

    mon->data = ptr;
    mon->mnum = (short) monsndx(ptr);
    mon->subtype = subtype;
    mon->heads_left = mon->data->heads;

    if (mon->movement) { /* used to adjust poly'd hero as well as monsters */
        new_speed = ptr->mmove;
        /* prorate unused movement if new form is slower so that
           it doesn't get extra moves leftover from previous form;
           if new form is faster, leave unused movement as is */
        if (new_speed < old_speed) {
            /*
             * Some static analysis warns that this might divide by 0
               mon->movement = new_speed * mon->movement / old_speed;
             * so add a redundant test to suppress that.
             */
            mon->movement *= new_speed;
            if (old_speed > 0) /* old > new and new >= 0, so always True */
                mon->movement /= old_speed;
        }
    }
    return;
}

int
does_passive_impact_obj(ptr, obj)
struct permonst* ptr;
struct obj* obj;
{
    if (!obj || !ptr || is_obj_indestructible(obj))
        return 0;

    struct attack* a;
    for (a = &ptr->mattk[0]; a < &ptr->mattk[NATTK]; a++)
        if (a->aatyp == AT_PASV || a->aatyp == AT_BOOM || a->aatyp == AT_EXPL)
        {
            if (a->aflags & ATTKFLAGS_NO_OBJ_DMG) //Non-corrosive passive for floating eye
                continue;

            switch (a->adtyp)
            {
            case AD_FIRE:
                return !oresist_fire(obj);
            case AD_ELEC:
                return !oresist_elec(obj);
            case AD_COLD:
                return !oresist_cold(obj);
            case AD_ACID:
            case AD_PLYS:
                return !!is_damageable(obj);
            case AD_CORR:
                return !!is_corrodeable(obj);
            case AD_RUST:
                return !!is_rustprone(obj);
            case AD_STON:
                return !!is_organic(obj);
            case AD_SLIM:
                return !!is_organic(obj);
            case AD_DISN:
                return !oresist_disintegration(obj);
            case AD_ENCH:
                return 2; // enchantment does not matter
            case AD_PHYS:
                return 1;
            default:
                break;
            }
        }
    return 0;
}

/* does monster-type have any attack for a specific type of damage? */
struct attack *
attacktype_fordmg(ptr, atyp, dtyp)
struct permonst *ptr;
int atyp, dtyp;
{
    struct attack *a;

    for (a = &ptr->mattk[0]; a < &ptr->mattk[NATTK]; a++)
        if (a->aatyp == atyp && (dtyp == AD_ANY || a->adtyp == dtyp))
            return a;
    return (struct attack *) 0;
}

/* does monster-type have a particular type of attack */
boolean
attacktype(ptr, atyp)
struct permonst *ptr;
int atyp;
{
    return attacktype_fordmg(ptr, atyp, AD_ANY) ? TRUE : FALSE;
}

/* returns True if monster doesn't attack, False if it does */
boolean
noattacks(ptr)
struct permonst *ptr;
{
    int i;
    struct attack *mattk = ptr->mattk;

    for (i = 0; i < NATTK; i++) {
        /* AT_BOOM "passive attack" (gas spore's explosion upon death)
           isn't an attack as far as our callers are concerned */
        if (mattk[i].aatyp == AT_BOOM)
            continue;

        if (mattk[i].aatyp)
            return FALSE;
    }
    return TRUE;
}

/* does monster-type transform into something else when petrified? */
boolean
poly_when_stoned(ptr)
struct permonst *ptr;
{
    /* non-stone golems turn into stone golems unless latter is genocided */
    return (boolean) (is_golem(ptr) && ptr != &mons[PM_STONE_GOLEM]
                      && !(mvitals[PM_STONE_GOLEM].mvflags & MV_GENOCIDED));
    /* allow MV_EXTINCT */
}


/* True iff monster can be blinded by the given attack;
   note: may return True when mdef is blind (e.g. new cream-pie attack) */
boolean
can_blnd(magr, mdef, aatyp, obj)
struct monst *magr; /* NULL == no specific aggressor */
struct monst *mdef;
uchar aatyp;
struct obj *obj; /* aatyp == AT_WEAP, AT_SPIT */
{
    boolean is_you = (mdef == &youmonst);
    boolean check_visor = FALSE;
    struct obj *o;
    const char *s;

    /* no eyes protect against all attacks for now */
    if (!haseyes(mdef->data))
        return FALSE;

    switch (aatyp) {
    case AT_EXPL:
    case AT_BOOM:
    case AT_GAZE:
    case AT_MAGC:
    case AT_EYES: /* assumed to be lightning */
    case AT_BREA: /* assumed to be lightning */
        /* light-based attacks may be cancelled or resisted */
        if (magr && is_cancelled(magr))
            return FALSE;
        return !resists_blnd(mdef);

    case AT_WEAP:
    case AT_SPIT:
    case AT_NONE:
    case AT_PASV:
        /* an object is used (thrown/spit/other) */
        if (obj && (obj->otyp == CREAM_PIE)) {
            if (is_you && Blindfolded)
                return FALSE;
        } else if (obj && (obj->otyp == BLINDING_VENOM)) {
            /* all ublindf protect, cream-pies too */
            if (is_you && (ublindf || u.ucreamed || Eye_protection))
                return FALSE;
            check_visor = TRUE;
        } else if (obj && (obj->otyp == POT_BLINDNESS)) {
            return TRUE; /* no defense */
        } else
            return FALSE; /* other objects cannot cause blindness yet */
        if ((magr == &youmonst) && u.uswallow)
            return FALSE; /* can't affect eyes while inside monster */
        break;

    case AT_ENGL:
        if (is_you && (Blindfolded || Unaware || u.ucreamed))
            return FALSE;
        if (!is_you && mdef->msleeping)
            return FALSE;
        break;

    case AT_CLAW:
        /* e.g. raven: all ublindf protect */
        if (is_you && (ublindf || Eye_protection))
            return FALSE;
        if ((magr == &youmonst) && u.uswallow)
            return FALSE; /* can't affect eyes while inside monster */
        check_visor = TRUE;
        break;

    case AT_TUCH:
    case AT_STNG:
    case AT_TAIL:
        /* some physical, blind-inducing attacks can be cancelled */
        if (magr && is_cancelled(magr))
            return FALSE;
        break;

    default:
        break;
    }

    /* check if wearing a visor (only checked if visor might help) */
    if (check_visor) {
        o = (mdef == &youmonst) ? invent : mdef->minvent;
        for (; o; o = o->nobj)
            if ((o->owornmask & W_ARMH)
                && (s = OBJ_DESCR(objects[o->otyp])) != (char *) 0
                && !strcmp(s, "visored helmet"))
                return FALSE;
    }

    return TRUE;
}

/* returns True if monster can attack at range */
boolean
ranged_attk(ptr)
struct permonst *ptr;
{
    register int i, atyp;
    int64_t atk_mask = ((int64_t)1 << AT_EYES) | ((int64_t)1 << AT_BREA) | ((int64_t)1 << AT_SPIT) | ((int64_t)1 << AT_GAZE);

    /* was: (attacktype(ptr, AT_BREA) || attacktype(ptr, AT_WEAP)
     *       || attacktype(ptr, AT_SPIT) || attacktype(ptr, AT_GAZE)
     *       || attacktype(ptr, AT_MAGC));
     * but that's too slow -dlc
     */
    for (i = 0; i < NATTK; i++) {
        atyp = ptr->mattk[i].aatyp;
        if (atyp >= AT_WEAP)
            return TRUE;
        /* assert(atyp < 32); */
        if ((atk_mask & ((int64_t)1 << atyp)) != 0L)
            return TRUE;
    }
    return FALSE;
}

/* True if specific monster is especially affected by silver weapons */
boolean
mon_hates_silver(mon)
struct monst *mon;
{
    return (boolean) (is_vampshifter(mon) || hates_silver(mon->data));
}

/* True if monster-type is especially affected by silver weapons */
boolean
hates_silver(ptr)
register struct permonst *ptr;
{
    return (boolean) (is_were(ptr) || ptr->mlet == S_VAMPIRE || is_demon(ptr)
                      || is_special_silver_hater(ptr));
}

/* True if specific monster is especially affected by silver weapons */
boolean
mon_hates_cursed(mon)
struct monst* mon;
{
    return hates_cursed(mon->data);
}

/* True if monster-type is especially affected by silver weapons */
boolean
hates_cursed(ptr)
register struct permonst* ptr;
{
    return is_angel(ptr);
}

/* True if monster-type is especially affected by silver weapons */
boolean
hates_blessed(ptr)
register struct permonst* ptr;
{
    return (is_demon(ptr) || is_undead(ptr));
}

boolean
mon_hates_blessed(mon)
struct monst* mon;
{
    return (hates_blessed(mon->data) || is_vampshifter(mon));
}

boolean
eschews_cursed(ptr)
register struct permonst* ptr;
{
    return (innate_eschew_cursed(ptr) || is_animal(ptr) || hates_cursed(ptr));
}

boolean
mon_eschews_cursed(mon)
struct monst* mon;
{
    return (eschews_cursed(mon->data) || mon_hates_cursed(mon));
}

boolean
eschews_blessed(ptr)
register struct permonst* ptr;
{
    return (innate_eschew_blessed(ptr) || hates_blessed(ptr));
}

boolean
mon_eschews_blessed(mon)
struct monst* mon;
{
    return (eschews_blessed(mon->data) || mon_hates_blessed(mon));
}


boolean
eschews_silver(ptr)
register struct permonst* ptr;
{
    return (innate_eschew_silver(ptr) || hates_silver(ptr));
}

boolean
mon_eschews_silver(mon)
struct monst* mon;
{
    return (eschews_silver(mon->data) || mon_hates_silver(mon));
}


/* True if specific monster is especially affected by light-emitting weapons */
boolean
mon_hates_light(mon)
struct monst *mon;
{
    return (boolean) (hates_light(mon->data));
}

boolean
mon_eschews_light(mon)
struct monst* mon;
{
    return (mon_hates_light(mon));
}

/* True iff the type of monster pass through iron bars */
boolean
passes_bars(mptr)
struct permonst *mptr;
{
    return (boolean) (passes_walls(mptr) || amorphous(mptr) || unsolid(mptr)
                      || is_whirly(mptr) || verysmall(mptr)
                      || dmgtype(mptr, AD_CORR) || dmgtype(mptr, AD_RUST)
                      || (slithy(mptr) && !bigmonst(mptr)));
}

/* returns True if monster can blow (whistle, etc) */
boolean
can_blow(mtmp)
struct monst *mtmp;
{
    if ((is_silent(mtmp->data) || mtmp->data->msound == MS_BUZZ)
        && (has_innate_breathless(mtmp->data) || verysmall(mtmp->data)
            || !has_head(mtmp->data) || mtmp->data->mlet == S_EEL))
        return FALSE;
    if ((mtmp == &youmonst) && Strangled)
        return FALSE;
    return TRUE;
}

/* for casting spells and reading scrolls while blind */
boolean
can_chant(mtmp)
struct monst *mtmp;
{
    if ((mtmp == &youmonst && (Strangled || Silenced))
        || is_silenced(mtmp) || is_being_strangled(mtmp)
        || is_silent(mtmp->data) || (!has_head(mtmp->data) && !is_speaking(mtmp->data))
        || mtmp->data->msound == MS_BUZZ || mtmp->data->msound == MS_BURBLE)
        return FALSE;
    return TRUE;
}

/* True if mon is vulnerable to strangulation */
boolean
can_be_strangled(mon)
struct monst *mon;
{
    //boolean nonbreathing, nobrainer;

    /* For amulet of strangulation support:  here we're considering
       strangulation to be loss of blood flow to the brain due to
       constriction of the arteries in the neck, so all headless
       creatures are immune (no neck) as are mindless creatures
       who don't need to breathe (brain, if any, doesn't care).
       Mindless creatures who do need to breath are vulnerable, as
       are non-breathing creatures which have higher brain function. */
    if (!has_neck(mon->data))
        return FALSE;

    return TRUE;

#if 0
    if (mon == &youmonst)
    {
        /* hero can't be mindless but poly'ing into mindless form can
           confer strangulation protection */
        nobrainer = mindless(youmonst.data);
        nonbreathing = Breathless;
    }
    else
    {
        nobrainer = mindless(mon->data);
        /* monsters don't wear amulets of magical breathing,
           so second part doesn't achieve anything useful... */
        nonbreathing = is_breathless(mon);
    }
    return (boolean) (!nobrainer || !nonbreathing);
#endif
}

/* returns True if monster can track well */
boolean
can_track(ptr)
register struct permonst *ptr;
{
    /* All monsters can track Excalibur */
    if ((uwep && uwep->oartifact && artifact_has_flag(uwep, AF_MONSTERS_CAN_TRACK_ARTIFACT)) 
        || (uarms && uarms->oartifact && artifact_has_flag(uarms, AF_MONSTERS_CAN_TRACK_ARTIFACT))
        )
        return TRUE;
    else
        return (boolean) haseyes(ptr);
}

/* creature will slide out of armor */
boolean
sliparm(ptr)
register struct permonst *ptr;
{
    return (boolean) (is_whirly(ptr) || ptr->msize <= MZ_SMALL);
}

/* creature will break out of armor */
boolean
breakarm(ptr)
register struct permonst *ptr;
{
    if (sliparm(ptr))
        return FALSE;

    return (boolean) (bigmonst(ptr)
                      || (ptr->msize > MZ_SMALL && !humanoid(ptr))
                      /* special cases of humanoids that cannot wear suits */
                      || ptr == &mons[PM_MARILITH] || ptr == &mons[PM_GLABREZU]
                      || ptr == &mons[PM_WINGED_GARGOYLE]);
}

/* creature sticks other creatures it hits */
boolean
sticks(ptr)
register struct permonst *ptr;
{
    return (boolean) (dmgtype(ptr, AD_STCK) || dmgtype(ptr, AD_WRAP)
                      || attacktype(ptr, AT_HUGS));
}

/* some monster-types can't vomit */
boolean
cantvomit(ptr)
struct permonst *ptr;
{
    /* rats and mice are incapable of vomiting;
       which other creatures have the same limitation? */
    if (ptr->mlet == S_RODENT && ptr != &mons[PM_ROCK_MOLE]
        && ptr != &mons[PM_WOODCHUCK])
        return TRUE;
    return FALSE;
}

/* number of horns this type of monster has on its head */
int
num_horns(ptr)
struct permonst *ptr;
{
    if (!ptr)
        return 0;

    switch (monsndx(ptr)) {
    case PM_HORNED_DEVIL: /* ? "more than one" */
    case PM_MINOTAUR:
    case PM_ASMODEUS:
    case PM_DISPATER:
    case PM_BALOR:
        return 2;
    case PM_WHITE_UNICORN:
    case PM_GRAY_UNICORN:
    case PM_BLACK_UNICORN:
    case PM_KI_RIN:
        return 1;
    default:
        break;
    }
    return 0;
}

/* does monster-type deal out a particular type of damage from a particular
   type of attack? */
struct attack *
dmgtype_fromattack(ptr, dtyp, atyp)
struct permonst *ptr;
int dtyp, atyp;
{
    struct attack *a;

    for (a = &ptr->mattk[0]; a < &ptr->mattk[NATTK]; a++)
        if (a->adtyp == dtyp && (atyp == AT_ANY || a->aatyp == atyp))
            return a;
    return (struct attack *) 0;
}

/* does monster-type deal out a particular type of damage from any attack */
boolean
dmgtype(ptr, dtyp)
struct permonst *ptr;
int dtyp;
{
    return dmgtype_fromattack(ptr, dtyp, AT_ANY) ? TRUE : FALSE;
}

/* returns the maximum damage a defender can do to the attacker via
   a passive defense */
int
max_passive_dmg(mdef, magr)
register struct monst *mdef, *magr;
{
    int i, dmg = 0, multi2 = 0;
    uchar adtyp;

    /* each attack by magr can result in passive damage */
    for (i = 0; i < NATTK; i++)
        switch (magr->data->mattk[i].aatyp) {
        case AT_CLAW:
        case AT_BITE:
        case AT_TUSK:
        case AT_HORN:
        case AT_RAMS:
        case AT_KICK:
        case AT_BUTT:
        case AT_TAIL:
        case AT_TUCH:
        case AT_STNG:
        case AT_HUGS:
        case AT_ENGL:
        case AT_TENT:
        case AT_WEAP:
            multi2++;
            break;
        default:
            break;
        }

    for (i = 0; i < NATTK; i++)
        if (mdef->data->mattk[i].aatyp == AT_PASV
            || mdef->data->mattk[i].aatyp == AT_BOOM) {
            adtyp = mdef->data->mattk[i].adtyp;
            if ((adtyp == AD_ACID && !is_mon_immune_to_acid(magr))
                || (adtyp == AD_COLD && !is_mon_immune_to_cold(magr))
                || (adtyp == AD_FIRE && !is_mon_immune_to_fire(magr))
                || (adtyp == AD_ELEC && !is_mon_immune_to_elec(magr))
                || adtyp == AD_PHYS) {
                dmg = max(0, (mdef->data->mattk[i].damn > 0 ? mdef->data->mattk[i].damn : (int)mdef->data->mlevel / 2 + 2) * (mdef->data->mattk[i].damd > 0 ? mdef->data->mattk[i].damd : 6) + mdef->data->mattk[i].damp);
            } else
                dmg = 0;

            return dmg * multi2;
        }
    return 0;
}

/* determine whether two monster types are from the same species */
boolean
same_race(pm1, pm2)
struct permonst *pm1, *pm2;
{
    char let1 = pm1->mlet, let2 = pm2->mlet;

    if (pm1 == pm2)
        return TRUE; /* exact match */

    /* player races have their own predicates */
    if (is_human(pm1))
        return is_human(pm2);
    if (is_elf(pm1))
        return is_elf(pm2);
    if (is_dwarf(pm1))
        return is_dwarf(pm2);
    if (is_gnoll(pm1))
        return is_gnoll(pm2);
    if (is_gnome(pm1))
        return is_gnome(pm2);
    if (is_orc(pm1))
        return is_orc(pm2);
    /* other creatures are less precise */
    if (is_giant(pm1))
        return is_giant(pm2); /* open to quibbling here */
    if (is_golem(pm1))
        return is_golem(pm2); /* even moreso... */
    if (is_tentacled_one(pm1))
        return is_tentacled_one(pm2);
    if (let1 == S_KOBOLD || pm1 == &mons[PM_KOBOLD_ZOMBIE]
        || pm1 == &mons[PM_KOBOLD_MUMMY])
        return (let2 == S_KOBOLD || pm2 == &mons[PM_KOBOLD_ZOMBIE]
                || pm2 == &mons[PM_KOBOLD_MUMMY]);
    if (let1 == S_OGRE)
        return (let2 == S_OGRE);
    if (let1 == S_NYMPH)
        return (let2 == S_NYMPH);
    if (let1 == S_CENTAUR)
        return (let2 == S_CENTAUR);
    if (is_unicorn(pm1))
        return is_unicorn(pm2);
    if (let1 == S_DRAGON)
        return (let2 == S_DRAGON);
    if (let1 == S_NAGA)
        return (let2 == S_NAGA);
    /* other critters get steadily messier */
    if (is_rider(pm1))
        return is_rider(pm2); /* debatable */
    if (is_minion(pm1))
        return is_minion(pm2); /* [needs work?] */
    /* tengu don't match imps (first test handled case of both being tengu) */
    if (pm1 == &mons[PM_TENGU] || pm2 == &mons[PM_TENGU])
        return FALSE;
    if (let1 == S_IMP)
        return (let2 == S_IMP);
    /* and minor demons (imps) don't match major demons */
    else if (let2 == S_IMP)
        return FALSE;
    if (is_demon(pm1))
        return is_demon(pm2);
    if (is_undead(pm1)) {
        if (let1 == S_LESSER_UNDEAD)
            return (let2 == S_LESSER_UNDEAD);
        if (let1 == S_GREATER_UNDEAD)
            return (let2 == S_GREATER_UNDEAD);
        if (let1 == S_VAMPIRE)
            return (let2 == S_VAMPIRE);
        if (let1 == S_LICH)
            return (let2 == S_LICH);
        if (let1 == S_WRAITH)
            return (let2 == S_WRAITH);
        if (let1 == S_GHOST)
            return (let2 == S_GHOST);
    } else if (is_undead(pm2))
        return FALSE;

    /* check for monsters which grow into more mature forms */
    if (let1 == let2) {
        int m1 = monsndx(pm1), m2 = monsndx(pm2), prv, nxt;

        /* we know m1 != m2 (very first check above); test all smaller
           forms of m1 against m2, then all larger ones; don't need to
           make the corresponding tests for variants of m2 against m1 */
        for (prv = m1, nxt = big_to_little(m1); nxt != prv;
             prv = nxt, nxt = big_to_little(nxt))
            if (nxt == m2)
                return TRUE;
        for (prv = m1, nxt = little_to_big(m1); nxt != prv;
             prv = nxt, nxt = little_to_big(nxt))
            if (nxt == m2)
                return TRUE;
    }
    /* not caught by little/big handling */
    if (pm1 == &mons[PM_GARGOYLE] || pm1 == &mons[PM_WINGED_GARGOYLE])
        return (pm2 == &mons[PM_GARGOYLE]
                || pm2 == &mons[PM_WINGED_GARGOYLE]);
    if (pm1 == &mons[PM_KILLER_BEE] || pm1 == &mons[PM_QUEEN_BEE])
        return (pm2 == &mons[PM_KILLER_BEE] || pm2 == &mons[PM_QUEEN_BEE]);

    if (is_long_worm(pm1))
        return is_long_worm(pm2); /* handles tail */

    if (is_long_worm(pm1))
        return is_long_worm(pm2); /* handles tail */

    if (is_cockatrice(pm1))
        return is_cockatrice(pm2);

    if (is_hell_hound(pm1))
        return is_hell_hound(pm2);

    /* [currently there's no reason to bother matching up
        assorted bugs and blobs with their closest variants] */
    /* didn't match */
    return FALSE;
}

/* return an index into the mons array */
int
monsndx(ptr)
struct permonst *ptr;
{
    register int i;

    i = (int) (ptr - &mons[0]);
    if (i < LOW_PM || i >= NUM_MONSTERS) 
    {
        panic("monsndx - could not index monster (%s, %s, %d)",
            fmt_ptr((genericptr_t)&mons[0]), fmt_ptr((genericptr_t) ptr), i);
        return NON_PM; /* will not get here */
    }
    return i;
}

/* for handling alternate spellings */
struct alt_spl {
    const char *name;
    short pm_val;
    uchar gender; //0 = both, 1 = male only, 2 = female only
};

/* figure out what type of monster a user-supplied string is specifying */
int
name_to_mon(in_str)
const char* in_str;
{
    int fem = 0;
    return name_to_mon_ex(in_str, &fem);
}

int
name_to_mon_ex(in_str, fem_ptr)
const char *in_str;
int* fem_ptr;
{
    /* Be careful.  We must check the entire string in case it was
     * something such as "ettin zombie corpse".  The calling routine
     * doesn't know about the "corpse" until the monster name has
     * already been taken off the front, so we have to be able to
     * read the name with extraneous stuff such as "corpse" stuck on
     * the end.
     * This causes a problem for names which prefix other names such
     * as "ettin" on "ettin zombie".  In this case we want the _longest_
     * name which exists.
     * This also permits plurals created by adding suffixes such as 's'
     * or 'es'.  Other plurals must still be handled explicitly.
     */
    register int i;
    register int mntmp = NON_PM;
    register char *s, *str, *term;
    char buf[BUFSZ];
    size_t len, slen;

    str = strcpy(buf, in_str);

    if (!strncmp(str, "a ", 2))
        str += 2;
    else if (!strncmp(str, "an ", 3))
        str += 3;
    else if (!strncmp(str, "the ", 4))
        str += 4;

    slen = strlen(str);
    term = str + slen;

    if ((s = strstri(str, "vortices")) != 0)
        Strcpy(s + 4, "ex");
    /* be careful with "ies"; "priest", "zombies" */
    else if (slen > 3 && !strcmpi(term - 3, "ies")
             && (slen < 7 || strcmpi(term - 7, "zombies")))
        Strcpy(term - 3, "y");
    /* luckily no monster names end in fe or ve with ves plurals */
    else if (slen > 3 && !strcmpi(term - 3, "ves"))
        Strcpy(term - 3, "f");

    slen = strlen(str); /* length possibly needs recomputing */

    {
        static const struct alt_spl names[] = {
            /* Alternate spellings */
            { "grey dragon", PM_GRAY_DRAGON, 0 },
            { "grey dragon hatchling", PM_GRAY_DRAGON_HATCHLING, 0 },
            { "baby gray dragon", PM_GRAY_DRAGON_HATCHLING, 0 },
            { "baby grey dragon", PM_GRAY_DRAGON_HATCHLING, 0 },
            { "baby silver dragon", PM_SILVER_DRAGON_HATCHLING, 0 },
            { "baby red dragon", PM_RED_DRAGON_HATCHLING, 0 },
            { "baby white dragon", PM_WHITE_DRAGON_HATCHLING, 0 },
            { "baby black dragon", PM_BLACK_DRAGON_HATCHLING, 0 },
            { "baby blue dragon", PM_BLUE_DRAGON_HATCHLING, 0 },
            { "baby green dragon", PM_GREEN_DRAGON_HATCHLING, 0 },
            { "baby yellow dragon", PM_YELLOW_DRAGON_HATCHLING, 0 },
            { "grey unicorn", PM_GRAY_UNICORN, 0 },
            { "grey ooze", PM_GRAY_OOZE, 0 },
            { "gray-elf", PM_GREY_ELF, 0 },
            { "mindflayer", PM_TENTACLED_ONE, 0 },
            { "master mindflayer", PM_ELDER_TENTACLED_ONE, 0 },
            { "mind flayer", PM_TENTACLED_ONE, 0 },
            { "master mind flayer", PM_ELDER_TENTACLED_ONE, 0 },
            { "umber hulk", PM_UMBRAL_HULK, 0 },
            /* More alternates; priest and priestess are separate monster
               types but that isn't the case for {aligned,high} priests */
            { "aligned priestess", PM_ALIGNED_PRIEST, 2 },
            { "high priestess", PM_HIGH_PRIEST, 2 },
            /* Inappropriate singularization by -ves check above */
            { "master of thief", PM_MASTER_OF_THIEVES, 0 },
            /* Potential misspellings where we want to avoid falling back
               to the rank title prefix (input has been singularized) */
            { "master thief", PM_MASTER_OF_THIEVES, 0 },
            { "master of assassin", PM_MASTER_ASSASSIN, 0 },
            /* Outdated names */
            { "invisible stalker", PM_STALKER, 0 },
            { "high-elf", PM_ELVENKING, 0 }, /* PM_HIGH_ELF is obsolete */
            /* other misspellings or incorrect words */
            { "wood-elf", PM_WOODLAND_ELF, 0 },
            { "wood elf", PM_WOODLAND_ELF, 0 },
            { "woodland nymph", PM_WOOD_NYMPH, 0 },
            { "halfling", PM_HALFLING, 0 },    /* potential guess for polyself */
            { "genie", PM_DJINNI, 0 }, /* potential guess for ^G/#wizgenesis */
            /* Hyphenated names -- it would be nice to handle these via
               fuzzymatch() but it isn't able to ignore trailing stuff */
            { "ki rin", PM_KI_RIN, 0 },
            { "uruk hai", PM_GREAT_ORC, 0 },
            { "orc captain", PM_ORC_CAPTAIN, 0 },
            { "woodland elf", PM_WOODLAND_ELF, 0 },
            { "green elf", PM_GREEN_ELF, 0 },
            { "grey elf", PM_GREY_ELF, 0 },
            { "gray elf", PM_GREY_ELF, 0 },
            { "elf lord", PM_ELF_LORD, 0 },
            { "olog hai", PM_WAR_TROLL, 0 },
            { "arch lich", PM_ARCH_LICH, 0 },
            { "archlich", PM_ARCH_LICH, 0 },
            /* Some irregular plurals */
            { "incubi", PM_INCUBUS, 1 },
            { "succubi", PM_INCUBUS, 2 },
            { "foocubi", PM_INCUBUS, 0 },
            { "violet fungi", PM_VIOLET_FUNGUS, 0 },
            { "homunculi", PM_HOMUNCULUS, 0 },
            { "baluchitheria", PM_BALUCHITHERIUM, 0 },
            { "lurkers above", PM_LURKER_ABOVE, 0 },
            { "cavemen", PM_CAVEMAN, 1 },
            { "cavewomen", PM_CAVEMAN, 2 },
            { "cavepersons", PM_CAVEMAN, 0 },
            { "cavepeople", PM_CAVEMAN, 0 },
            { "watchmen", PM_WATCHMAN, 1 },
            { "watchwomen", PM_WATCHMAN, 2 },
            { "watchpersons", PM_WATCHMAN, 0 },
            { "djinn", PM_DJINNI, 0 },
            { "mumak", PM_OLIPHANT, 0 },
            { "mumakil", PM_OLIPHANT, 0 },
            { "erinyes", PM_ERINYS, 0 },
            { "mummy pharaoh", PM_GREATER_MUMMY_PHARAOH, 0 },
            { "mummy king", PM_GREATER_MUMMY_PHARAOH, 0 },
            { "mummy priest", PM_GREATER_MUMMY_PRIEST, 0 },
            { "mummy high priest", PM_GREATER_MUMMY_HIGH_PRIEST, 0 },
            { "leocrotta", PM_LEUCROTTA, 0 },
            { "archeologist", PM_ARCHAEOLOGIST, 0 },
            { "dire wolf", PM_DIREWOLF, 0 },
            { "dire wolf cub", PM_DIREWOLF_CUB, 0 },
            { "hobbit", PM_HALFLING, 0 },
            { "balrog", PM_BALOR, 0 },
            { "ent", PM_TREANT, 0 },
            { "elder ent", PM_ELDER_TREANT, 0 },
            { "mordor orc", PM_BLACK_ORC, 0 },
            { "Mordor orc", PM_BLACK_ORC, 0 },
            { "uruk-hai", PM_GREAT_ORC, 0 },
            { "Uruk-hai", PM_GREAT_ORC, 0 },
            { "olog-hai", PM_WAR_TROLL, 0 },
            { "Olog-hai", PM_WAR_TROLL, 0 },
            { "elven king", PM_ELVENKING, 1 },
            { "elven-king", PM_ELVENKING, 1 },
            { "elf king", PM_ELVENKING, 1 },
            { "elf-king", PM_ELVENKING, 1 },
            { "elven queen", PM_ELVENKING, 2 },
            { "elven-queen", PM_ELVENKING, 2 },
            { "elf queen", PM_ELVENKING, 2 },
            { "elf-queen", PM_ELVENKING, 2 },
            { "Yeenoghu", PM_YEENAGHU, 2 },
            { "yeenoghu", PM_YEENAGHU, 2 },
            { "Juiblex", PM_JUBILEX, 2 },
            { "juiblex", PM_JUBILEX, 2 },
            /* end of list */
            { 0, NON_PM }
        };
        register const struct alt_spl *namep;

        for (namep = names; namep->name; namep++)
            if (!strncmpi(str, namep->name, strlen(namep->name)))
            {
                if(namep->gender == 1 && fem_ptr)
                    *fem_ptr = 0;
                if (namep->gender == 2 && fem_ptr)
                    *fem_ptr = 1;
                return namep->pm_val;
            }
    }

    boolean found = FALSE;
    for (int j = 1; j <= 3; j++)
    {
        for (len = 0, i = LOW_PM; i < NUM_MONSTERS; i++)
        {
            const char* relevant_name = (const char*)0;
            if (j == 1)
                relevant_name = mons[i].mname;
            else if (j == 2)
                relevant_name = mons[i].mfemalename;
            else if (j == 3)
                relevant_name = mons[i].mcommonname;

            if (!relevant_name)
                continue;

            size_t m_i_len = relevant_name ? strlen(relevant_name) : 0;

            if (m_i_len > len && !strncmpi(relevant_name, str, m_i_len)) {
                if (m_i_len == slen) {
                    mntmp = i;
                    found = TRUE;
                    break; /* exact match */
                } else if (slen > m_i_len
                           && (str[m_i_len] == ' '
                               || !strcmpi(&str[m_i_len], "s")
                               || !strncmpi(&str[m_i_len], "s ", 2)
                               || !strcmpi(&str[m_i_len], "'")
                               || !strncmpi(&str[m_i_len], "' ", 2)
                               || !strcmpi(&str[m_i_len], "'s")
                               || !strncmpi(&str[m_i_len], "'s ", 3)
                               || !strcmpi(&str[m_i_len], "es")
                               || !strncmpi(&str[m_i_len], "es ", 3))) {
                    mntmp = i;
                    len = m_i_len;
                }
            }
        }

        if (found)
        {
            if (j == 1 && fem_ptr && mntmp >= LOW_PM && mntmp < NUM_MONSTERS && mons[mntmp].mfemalename && *fem_ptr == -1)
                *fem_ptr = 0;
            if (j == 2 && fem_ptr)
                *fem_ptr = 1;
            break;
        }
    }

    if (mntmp == NON_PM)
        mntmp = title_to_mon(str, (int *) 0, (size_t *) 0);
    
    return mntmp;
}

/* monster class from user input; used for genocide and controlled polymorph;
   returns 0 rather than MAX_MONSTER_CLASSES if no match is found */
int
name_to_monclass(in_str, mndx_p)
const char *in_str;
int *mndx_p;
{
    /* Single letters are matched against def_monsyms[].sym; words
       or phrases are first matched against def_monsyms[].explain
       to check class description; if not found there, then against
       mons[].mname to test individual monster types.  Input can be a
       substring of the full description or mname, but to be accepted,
       such partial matches must start at beginning of a word.  Some
       class descriptions include "foo or bar" and "foo or other foo"
       so we don't want to accept "or", "other", "or other" there. */
    STATIC_VAR NEARDATA const char *const falsematch[] = {
        /* multiple-letter input which matches any of these gets rejected */
        "an", "the", "or", "other", "or other", 0
    };
    /* positive pm_val => specific monster; negative => class */
    STATIC_VAR NEARDATA const struct alt_spl truematch[] = {
        /* "long worm" won't match "worm" class but would accidentally match
           "long worm tail" class before the comparison with monster types */
        { "long worm", PM_LONG_WORM, 0 },
        /* matches wrong--or at least suboptimal--class */
        { "demon", -S_DEMON, 0 }, /* hits "imp or minor demon" */
        /* matches specific monster (overly restrictive) */
        { "devil", -S_DEMON, 0 }, /* always "horned devil" */
        /* some plausible guesses which need help */
        { "bug", -S_XAN, 0 },  /* would match bugbear... */
        { "fish", -S_EEL, 0 }, /* wouldn't match anything */
        /* end of list */
        { 0, NON_PM }
    };
    const char *p, *x;
    int i, len;

    if (mndx_p)
        *mndx_p = NON_PM; /* haven't [yet] matched a specific type */

    if (!in_str || !in_str[0]) {
        /* empty input */
        return 0;
    } else if (!in_str[1]) {
        /* single character */
        i = def_char_to_monclass(*in_str);
        if (i == S_MIMIC_DEF) { /* ']' -> 'm' */
            i = S_MIMIC;
        } else if (i == S_WORM_TAIL) { /* '~' -> 'w' */
            i = S_WORM;
            if (mndx_p)
                *mndx_p = PM_LONG_WORM;
        } else if (i == MAX_MONSTER_CLASSES) /* maybe 'I' */
            i = (*in_str == DEF_INVISIBLE) ? S_invisible : 0;
        return i;
    } else {
        /* multiple characters */
        if (!strcmpi(in_str, "long")) /* not enough to match "long worm" */
            return 0; /* avoid false whole-word match with "long worm tail" */
        in_str = makesingular(in_str);
        /* check for special cases */
        for (i = 0; falsematch[i]; i++)
            if (!strcmpi(in_str, falsematch[i]))
                return 0;
        for (i = 0; truematch[i].name; i++)
            if (!strcmpi(in_str, truematch[i].name)) {
                i = truematch[i].pm_val;
                if (i < 0)
                    return -i; /* class */
                if (mndx_p)
                    *mndx_p = i; /* monster */
                return mons[i].mlet;
            }
        /* check monster class descriptions */
        len = (int) strlen(in_str);
        for (i = 1; i < MAX_MONSTER_CLASSES; i++) {
            x = def_monsyms[i].explain;
            if ((p = strstri(x, in_str)) != 0 && (p == x || *(p - 1) == ' ')
                && ((int) strlen(p) >= len
                    && (p[len] == '\0' || p[len] == ' ')))
                return i;
        }
        /* check individual species names */
        i = name_to_mon(in_str);
        if (i != NON_PM) {
            if (mndx_p)
                *mndx_p = i;
            return mons[i].mlet;
        }
    }
    return 0;
}

/* returns 3 values (0=male, 1=female, 2=none) */
int
gender(mtmp)
register struct monst *mtmp;
{
    if (is_neuter(mtmp->data))
        return 2;
    return mtmp->female;
}

/* Like gender(), but lower animals and such are still "it".
   This is the one we want to use when printing messages. */
int
pronoun_gender(mtmp, override_vis)
register struct monst *mtmp;
boolean override_vis; /* if True then 'no it' unless neuter */
{
    if (!override_vis && !canspotmon(mtmp))
        return 2;
    if (is_neuter(mtmp->data))
        return 2;
    return (humanoid(mtmp->data) || (mtmp->data->geno & G_UNIQ)
            || is_mname_proper_name(mtmp->data)) ? (int) mtmp->female : 2;
}

/* used for nearby monsters when you go to another level */
boolean
levl_follower(mtmp)
struct monst *mtmp;
{
    if (mtmp == u.usteed)
        return TRUE;

    /* Wizard with Amulet won't bother trying to follow across levels */
    if (mtmp->iswiz && mon_has_amulet(mtmp))
        return FALSE;

    /* Special NPCs do not follow you */
    if (mtmp->isshk || mtmp->ispriest || mtmp->isnpc || mtmp->issmith)
        return FALSE;
    
    /* some monsters will follow even while intending to flee from you */
    if (is_tame(mtmp) || mtmp->iswiz || is_fshk(mtmp))
        return TRUE;

    /* stalking types follow, but won't when fleeing unless you hold
       the Amulet */
    return (boolean) ((mtmp->data->mflags2 & M2_STALK)
                      && (!is_fleeing(mtmp) || u.uhave.amulet));
}

STATIC_VAR const short grownups[][2] = {
    { PM_CHICKATRICE, PM_COCKATRICE },
    { PM_COCKATRICE, PM_GIANT_COCKATRICE },
    { PM_GIANT_COCKATRICE, PM_GARGANTUAN_COCKATRICE },
    { PM_LITTLE_DOG, PM_DOG },
    { PM_DOG, PM_LARGE_DOG },
    { PM_DIREWOLF_CUB, PM_DIREWOLF },
    { PM_DIREWOLF, PM_LARGE_DIREWOLF },
    { PM_HELL_HOUND_PUP, PM_HELL_HOUND },
    { PM_WINTER_WOLF_CUB, PM_WINTER_WOLF },
    { PM_KITTEN, PM_HOUSECAT },
    { PM_HOUSECAT, PM_LARGE_CAT },
    { PM_PONY, PM_HORSE },
    { PM_HORSE, PM_WARHORSE },
    { PM_RAM, PM_LARGE_RAM },
    { PM_LARGE_RAM, PM_WARRAM },
    { PM_SMALL_LUGGAGE, PM_MEDIUM_LUGGAGE },
    { PM_MEDIUM_LUGGAGE, PM_LARGE_LUGGAGE },
    { PM_LARGE_LUGGAGE, PM_GIANT_LUGGAGE },
    { PM_KOBOLD, PM_LARGE_KOBOLD },
    { PM_LARGE_KOBOLD, PM_KOBOLD_LORD },
    { PM_GNOME, PM_GNOME_LORD },
    { PM_GNOME_LORD, PM_GNOME_KING },
    { PM_DWARF, PM_DWARF_LORD },
    { PM_DWARF_LORD, PM_DWARF_KING },
    { PM_GNOLL, PM_GNOLL_LORD },
    { PM_GNOLL_LORD, PM_GNOLL_KING },
    { PM_GNOLL_WARDEN, PM_GNOLL_SUPREME_WARDEN },
    { PM_FLIND, PM_FLIND_LORD },
    { PM_TENTACLED_ONE, PM_ELDER_TENTACLED_ONE },
    { PM_ORC, PM_ORC_CAPTAIN },
    { PM_HILL_ORC, PM_ORC_CAPTAIN },
    { PM_BLACK_ORC, PM_ORC_CAPTAIN },
    { PM_GREAT_ORC, PM_ORC_CAPTAIN },
    { PM_SEWER_RAT, PM_GIANT_RAT },
    { PM_CAVE_SPIDER, PM_GIANT_SPIDER },
    { PM_GIANT_SPIDER, PM_PHASE_SPIDER },
    { PM_OGRE, PM_OGRE_LORD },
    { PM_OGRE_LORD, PM_OGRE_OVERLORD },
    { PM_OGRE_OVERLORD, PM_OGRE_KING },
    { PM_ELF, PM_ELF_LORD },
    { PM_WOODLAND_ELF, PM_ELF_LORD },
    { PM_GREEN_ELF, PM_ELF_LORD },
    { PM_GREY_ELF, PM_ELF_LORD },
    { PM_ELF_LORD, PM_ELVENKING },
    { PM_LICH, PM_DEMILICH },
    { PM_DEMILICH, PM_MASTER_LICH },
    { PM_MASTER_LICH, PM_ARCH_LICH },
    { PM_VAMPIRE, PM_VAMPIRE_LORD },
    { PM_VAMPIRE_LORD, PM_VAMPIRE_KING },
    { PM_BAT, PM_GIANT_BAT },
    { PM_GRAY_DRAGON_HATCHLING, PM_GRAY_DRAGON },
    { PM_SILVER_DRAGON_HATCHLING, PM_SILVER_DRAGON },
    { PM_RED_DRAGON_HATCHLING, PM_RED_DRAGON },
    { PM_WHITE_DRAGON_HATCHLING, PM_WHITE_DRAGON },
    { PM_ORANGE_DRAGON_HATCHLING, PM_ORANGE_DRAGON },
    { PM_BLACK_DRAGON_HATCHLING, PM_BLACK_DRAGON },
    { PM_BLUE_DRAGON_HATCHLING, PM_BLUE_DRAGON },
    { PM_GREEN_DRAGON_HATCHLING, PM_GREEN_DRAGON },
    { PM_YELLOW_DRAGON_HATCHLING, PM_YELLOW_DRAGON },
    { PM_GRAY_DRAGON, PM_ANCIENT_GRAY_DRAGON },
    { PM_SILVER_DRAGON, PM_ANCIENT_SILVER_DRAGON },
    { PM_RED_DRAGON, PM_ANCIENT_RED_DRAGON },
    { PM_WHITE_DRAGON, PM_ANCIENT_WHITE_DRAGON },
    { PM_ORANGE_DRAGON, PM_ANCIENT_ORANGE_DRAGON },
    { PM_BLACK_DRAGON, PM_ANCIENT_BLACK_DRAGON },
    { PM_BLUE_DRAGON, PM_ANCIENT_BLUE_DRAGON },
    { PM_GREEN_DRAGON, PM_ANCIENT_GREEN_DRAGON },
    { PM_YELLOW_DRAGON, PM_ANCIENT_YELLOW_DRAGON },
    { PM_DRACOLICH, PM_ELDER_DRACOLICH },
    { PM_RED_NAGA_HATCHLING, PM_RED_NAGA },
    { PM_BLACK_NAGA_HATCHLING, PM_BLACK_NAGA },
    { PM_GOLDEN_NAGA_HATCHLING, PM_GOLDEN_NAGA },
    { PM_GUARDIAN_NAGA_HATCHLING, PM_GUARDIAN_NAGA },
    { PM_SMALL_MIMIC, PM_LARGE_MIMIC },
    { PM_LARGE_MIMIC, PM_GIANT_MIMIC },
    { PM_GIANT_MIMIC, PM_CHAOS_MIMIC },
    { PM_BABY_LONG_WORM, PM_LONG_WORM },
    { PM_LONG_WORM, PM_ELDER_LONG_WORM },
    { PM_BABY_PURPLE_WORM, PM_PURPLE_WORM },
    { PM_BABY_CROCODILE, PM_CROCODILE },
    { PM_SOLDIER, PM_SERGEANT },
    { PM_SERGEANT, PM_LIEUTENANT },
    { PM_LIEUTENANT, PM_CAPTAIN },
    { PM_WATCHMAN, PM_WATCH_CAPTAIN },
    { PM_ALIGNED_PRIEST, PM_HIGH_PRIEST },
    { PM_STUDENT, PM_ARCHAEOLOGIST },
    { PM_ATTENDANT, PM_HEALER },
    { PM_PAGE, PM_KNIGHT },
    { PM_ACOLYTE, PM_PRIEST },
    { PM_APPRENTICE, PM_WIZARD },
    { PM_MANES, PM_LEMURE },
    { PM_KEYSTONE_KOP, PM_KOP_SERGEANT },
    { PM_KOP_SERGEANT, PM_KOP_LIEUTENANT },
    { PM_KOP_LIEUTENANT, PM_KOP_KAPTAIN },
    { PM_MODRON_MONODRONE, PM_MODRON_DUODRONE },
    { PM_MODRON_DUODRONE, PM_MODRON_TRIDRONE },
    { PM_MODRON_TRIDRONE, PM_MODRON_QUADRONE },
    { PM_MODRON_QUADRONE, PM_MODRON_PENTADRONE },
    { PM_MODRON_QUINTON, PM_MODRON_QUARTON },
    { PM_MODRON_QUARTON, PM_MODRON_TERTIAN },
    { PM_MODRON_TERTIAN, PM_MODRON_SECUNDUS },
    { PM_TREANT, PM_ELDER_TREANT },
    { PM_QUANTUM_MECHANIC, PM_ELDER_QUANTUM_MECHANIC },
    { NON_PM, NON_PM }
};

int
little_to_big(montype)
int montype;
{
    register int i;

    for (i = 0; grownups[i][0] >= LOW_PM; i++)
        if (montype == grownups[i][0]) {
            montype = grownups[i][1];
            break;
        }
    return montype;
}

int
big_to_little(montype)
int montype;
{
    register int i;

    for (i = 0; grownups[i][0] >= LOW_PM; i++)
        if (montype == grownups[i][1]) {
            montype = grownups[i][0];
            break;
        }
    return montype;
}

/* determine whether two permonst indices are part of the same progression;
   existence of progressions with more than one step makes it a bit tricky */
boolean
big_little_match(montyp1, montyp2)
int montyp1, montyp2;
{
    int l, b;

    /* simplest case: both are same pm */
    if (montyp1 == montyp2)
        return TRUE;
    /* assume it isn't possible to grow from one class letter to another */
    if (mons[montyp1].mlet != mons[montyp2].mlet)
        return FALSE;
    /* check whether montyp1 can grow up into montyp2 */
    for (l = montyp1; (b = little_to_big(l)) != l; l = b)
        if (b == montyp2)
            return TRUE;
    /* check whether montyp2 can grow up into montyp1 */
    for (l = montyp2; (b = little_to_big(l)) != l; l = b)
        if (b == montyp1)
            return TRUE;
    /* neither grows up to become the other; no match */
    return FALSE;
}

/*
 * Return the permonst ptr for the race of the monster.
 * Returns correct pointer for non-polymorphed and polymorphed
 * player.  It does not return a pointer to player role character.
 */
const struct permonst *
raceptr(mtmp)
struct monst *mtmp;
{
    if (mtmp == &youmonst && !Upolyd)
        return &mons[urace.monsternum];
    else
        return mtmp->data;
}

STATIC_VAR const char *levitate[4] = { "float", "Float", "wobble", "Wobble" };
STATIC_VAR const char *flys[4] = { "fly", "Fly", "flutter", "Flutter" };
STATIC_VAR const char *flyl[4] = { "fly", "Fly", "stagger", "Stagger" };
STATIC_VAR const char *slither[4] = { "slither", "Slither", "falter", "Falter" };
STATIC_VAR const char *ooze[4] = { "ooze", "Ooze", "tremble", "Tremble" };
STATIC_VAR const char *immobile[4] = { "wiggle", "Wiggle", "pulsate", "Pulsate" };
STATIC_VAR const char *crawl[4] = { "crawl", "Crawl", "falter", "Falter" };

const char *
locomotion(ptr, def)
const struct permonst *ptr;
const char *def;
{
    int capitalize = (*def == highc(*def));

    return (is_floater(ptr) ? levitate[capitalize]
            : (is_flyer(ptr) && ptr->msize <= MZ_SMALL) ? flys[capitalize]
              : (is_flyer(ptr) && ptr->msize > MZ_SMALL) ? flyl[capitalize]
                : slithy(ptr) ? slither[capitalize]
                  : amorphous(ptr) ? ooze[capitalize]
                    : !ptr->mmove ? immobile[capitalize]
                      : nolimbs(ptr) ? crawl[capitalize]
                        : def);
}

const char *
stagger(ptr, def)
const struct permonst *ptr;
const char *def;
{
    int capitalize = 2 + (*def == highc(*def));

    return (is_floater(ptr) ? levitate[capitalize]
            : (is_flyer(ptr) && ptr->msize <= MZ_SMALL) ? flys[capitalize]
              : (is_flyer(ptr) && ptr->msize > MZ_SMALL) ? flyl[capitalize]
                : slithy(ptr) ? slither[capitalize]
                  : amorphous(ptr) ? ooze[capitalize]
                    : !ptr->mmove ? immobile[capitalize]
                      : nolimbs(ptr) ? crawl[capitalize]
                        : def);
}

/* return phrase describing the effect of fire attack on a type of monster */
const char *
on_fire(mptr, mattk)
struct permonst *mptr;
struct attack *mattk;
{
    if (!mptr || !mattk)
        return "on fire";

    const char *what;

    switch (monsndx(mptr)) {
    case PM_FLAMING_SPHERE:
    case PM_FIRE_VORTEX:
    case PM_FIRE_ELEMENTAL:
    case PM_ELDER_FIRE_ELEMENTAL:
    case PM_SALAMANDER:
        what = "already on fire";
        break;
    case PM_WATER_ELEMENTAL:
    case PM_ELDER_WATER_ELEMENTAL:
    case PM_FOG_CLOUD:
    case PM_STEAM_VORTEX:
        what = "boiling";
        break;
    case PM_ICE_VORTEX:
    case PM_GLASS_GOLEM:
        what = "melting";
        break;
    case PM_STONE_GOLEM:
    case PM_CLAY_GOLEM:
    case PM_GOLD_GOLEM:
    case PM_AIR_ELEMENTAL:
    case PM_EARTH_ELEMENTAL:
    case PM_ELDER_AIR_ELEMENTAL:
    case PM_ELDER_EARTH_ELEMENTAL:
    case PM_DUST_VORTEX:
    case PM_ENERGY_VORTEX:
        what = "heating up";
        break;
    default:
        what = (mattk->aatyp == AT_HUGS) ? "being roasted" : "on fire";
        break;
    }
    return what;
}

/*
 * Returns:
 *      True if monster is presumed to have a sense of smell.
 *      False if monster definitely does not have a sense of smell.
 *
 * Do not base this on presence of a head or nose, since many
 * creatures sense smells other ways (feelers, forked-tongues, etc.)
 * We're assuming all insects can smell at a distance too.
 */
boolean
olfaction(mdat)
struct permonst *mdat;
{
    if (is_golem(mdat)
        || mdat->mlet == S_EYE /* spheres  */
        || mdat->mlet == S_JELLY || mdat->mlet == S_PUDDING
        || mdat->mlet == S_BLOB || mdat->mlet == S_VORTEX
        || mdat->mlet == S_ELEMENTAL
        || mdat->mlet == S_FUNGUS /* mushrooms and fungi */
        || mdat->mlet == S_LIGHT)
        return FALSE;
    return TRUE;
}

int
get_worm_tail_mnum(ptr)
struct permonst* ptr;
{
    if (ptr == &mons[PM_ELDER_LONG_WORM])
        return PM_ELDER_LONG_WORM_TAIL;
    else if (ptr == &mons[PM_LONG_WORM])
        return PM_LONG_WORM_TAIL;
    else
        return 0; /* Should not get here */
}


boolean
befriend_with_obj(ptr, obj)
struct permonst* ptr;
struct obj* obj;
{
    if (!ptr || !obj)
        return FALSE;

    if ((ptr->mflags4 & M4_LOVES_BANANAS) && (obj)->otyp == BANANA)
        return TRUE;

    if (is_hell_hound(ptr) && (obj)->oclass == FOOD_CLASS && (obj)->cursed)
    {
        if (!(obj->material == MAT_VEGGY
            || ((obj)->otyp == CORPSE && (obj)->corpsenm >= LOW_PM && vegetarian(&mons[(obj)->corpsenm]))))
            return TRUE;
    }

    if (is_domestic(ptr) && (obj)->oclass == FOOD_CLASS)
    {
        if ((ptr)->mlet == S_UNICORN)
        {
            if (obj->material == MAT_VEGGY
                || ((obj)->otyp == CORPSE && (obj)->corpsenm >= LOW_PM && vegetarian(&mons[(obj)->corpsenm])))
                return TRUE;
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;

}
/*mondata.c*/
