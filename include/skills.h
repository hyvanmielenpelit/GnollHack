/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

/* GnollHack 4.0    skills.h    $NHDT-Date: 1547255911 2019/01/12 01:18:31 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.15 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985-1999. */
/*-Copyright (c) Pasi Kallinen, 2017. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef SKILLS_H
#define SKILLS_H

/* Much of this code was taken from you.h.  It is now
 * in a separate file so it can be included in objects.c.
 */

enum p_skills {
    /* Code to denote that no skill is applicable */
    P_NONE = 0,

/* Weapon Skills -- Stephen White
 * Order matters and are used in macros.
 * Positive values denote hand-to-hand weapons or launchers.
 * Negative values denote ammunition or missiles.
 * Update weapon.c if you amend any skills.
 * Also used for oc_skill.
 */

    P_DAGGER,
    P_AXE,
    P_SWORD,
    P_BLUDGEONING_WEAPON,      /* Heavy-shafted bludgeon */
    P_FLAIL,                   /* Two pieces hinged or chained together */
    P_QUARTERSTAFF,            /* Long-shafted bludgeon */
    P_POLEARM,                 /* attack two or three steps away */
    P_THRUSTING_WEAPON,        /* includes javelin */
    P_BOW,                     /* launchers */
    P_SLING,
    P_CROSSBOW,
    P_THROWN_WEAPON,           /* hand-thrown missiles */
    P_WHIP,                    /* flexible, one-handed */

    /* Spell Skills added by Larry Stewart-Zerba */
    P_ARCANE_SPELL,
    P_CLERIC_SPELL,
    P_HEALING_SPELL,
    P_DIVINATION_SPELL,
    P_ABJURATION_SPELL,
    P_MOVEMENT_SPELL,
    P_TRANSMUTATION_SPELL,
    P_ENCHANTMENT_SPELL,
    P_CONJURATION_SPELL,
    P_CELESTIAL_SPELL,
    P_NATURE_SPELL,
    P_NECROMANCY_SPELL,

    /* Other types of combat */
    P_BARE_HANDED_COMBAT,      /* actually weaponless; gloves are ok */
    P_MARTIAL_ARTS,            /* actually weaponless; gloves are ok */
    P_DUAL_WEAPON_COMBAT,      /* pair of weapons, one in each hand */
    P_TWO_HANDED_WEAPON,       /* two-handed weapons */
    P_DODGE,                   /* increases AC */
    P_SHIELD,                  /* increases AC and MC, and for figthing with weapon shields */
    P_WAND,                    /* shooting rays with wands */

    /* Miscellaneous skills */
    P_RIDING,                  /* how well you control your steed */
    P_DIGGING,                 /* pick-axe and other digging tools */
    P_DISARM_TRAP,             /* disarming traps */
    
    P_NUM_SKILLS
};

//#define P_MARTIAL_ARTS P_BARE_HANDED_COMBAT /* Role distinguishes */

#define P_FIRST_WEAPON P_DAGGER
#define P_LAST_WEAPON P_WHIP

#define P_FIRST_SPELL P_ARCANE_SPELL
#define P_LAST_SPELL P_NECROMANCY_SPELL

#define P_FIRST_H_TO_H P_BARE_HANDED_COMBAT
#define P_LAST_H_TO_H P_WAND

#define P_FIRST_NONCOMBAT P_RIDING
#define P_LAST_NONCOMBAT P_DISARM_TRAP

/*
 * These are the standard weapon skill levels.  It is important that
 * the lowest "valid" skill be be 1.  The code calculates the
 * previous amount to practice by calling  practice_needed_to_advance()
 * with the current skill-1.  To work out for the UNSKILLED case,
 * a value of 0 needed.
 */
enum skill_levels {
    P_ISRESTRICTED = 0, /* unskilled and can't be advanced */
    P_UNSKILLED    = 1, /* unskilled so far but can be advanced */
    /* Skill levels Basic/Advanced/Expert had long been used by
       Heroes of Might and Magic (tm) and its sequels... */
    P_BASIC        = 2,
    P_SKILLED      = 3,
    P_EXPERT       = 4,
    P_MASTER       = 5,        /* Weapon skills and martial arts only */
    P_GRAND_MASTER = 6,        /* Weapon skills and martial arts  only */
    MAX_SKILL_LEVELS
};

#define is_magic_skill(skill) ((skill) >= P_ARCANE_SPELL && (skill) <= P_NECROMANCY_SPELL)
#define practice_needed_to_advance_for_normal_skill(level) ((level) * (level) * 20)
#define RIDING_TURNS_TO_EXERCISE_SKILL 50

/* The hero's skill in various weapons. */
struct skills {
    xchar skill;
    xchar max_skill;
    unsigned short advance;
};

#define P_SKILL_LEVEL(type) (u.weapon_skills[type].skill)
#define P_MAX_SKILL_LEVEL(type) (u.weapon_skills[type].max_skill)
#define P_ADVANCE(type) (u.weapon_skills[type].advance)
#define P_RESTRICTED(type) (u.weapon_skills[type].skill == P_ISRESTRICTED)
#define P_NONTRAINABLE(type) ((type) == P_DODGE)

#define P_SKILL_LIMIT 60 /* Max number of skill advancements */

#define martial_bonus() (adjusted_skill_level(P_MARTIAL_ARTS) >= P_BASIC)


/* Initial skill matrix structure; used in u_init.c and weapon.c */
struct def_skill {
    xchar skill;
    xchar sklvl;
};

#endif /* SKILLS_H */
