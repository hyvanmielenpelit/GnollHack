/* GnollHack 4.0  artilist.h      $NHDT-Date: 1433050874 2015/05/31 05:41:14 $  $NHDT-Branch: master $:$NHDT-Revision: 1.16 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2017. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifdef MAKEDEFS_C
/* in makedefs.c, all we care about is the list of names */

#define A(nam, desc, typ, masktyp, s1, s2, mt, atk, dfn, cry, inv, al, cl, rac, cost, clr) nam

static const char *artifact_names[] = {
#else
/* in artifact.c, set up the actual artifact list structure */

#define A(nam, desc, typ, masktyp, s1, s2, mt, atk, dfn, cry, inv, al, cl, rac, cost, clr) \
    {                                                                       \
        typ, masktyp, nam, desc, s1, s2, mt, atk, dfn, cry, inv, al, cl, rac, cost, clr    \
    }

/* clang-format off */
#define     NO_ATTK     {0,0,0,0,0}               /* no attack */
#define     NO_DFNS     {0,0,0,0,0}               /* no defense */
#define     NO_CARY     {0,0,0,0,0}               /* no carry effects */
#define     DFNS(c)     {0,c,0,0,0}
#define     CARY(c)     {0,c,0,0,0}
#define     PHYS(a,b)   {0,AD_PHYS,a,b,0}         /* physical */
#define     DRLI(a,b)   {0,AD_DRLI,a,b,0}         /* life drain */
#define     COLD(a,b)   {0,AD_COLD,a,b,0}
#define     FIRE(a,b)   {0,AD_FIRE,a,b,0}
#define     ELEC(a,b)   {0,AD_ELEC,a,b,0}         /* electrical shock */
#define     STUN(a,b)   {0,AD_STUN,a,b,0}         /* magical attack */
/* clang-format on */

#define ART_DOUBLE_DAMAGE -1
#define ART_TRIPLE_DAMAGE -2
#define ART_QUADRUPLE_DAMAGE -3

#define None (char *)0

STATIC_OVL NEARDATA struct artifact artilist[] = {
#endif /* MAKEDEFS_C */

    /* Artifact cost rationale:
     * 1.  The more useful the artifact, the better its cost.
     * 2.  Quest artifacts are highly valued.
     * 3.  Chaotic artifacts are inflated due to scarcity (and balance).
     */

    /*  dummy element #0, so that all interesting indices are non-zero */
    A("", None, STRANGE_OBJECT, STRANGE_OBJECT, 0, 0, 0, NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE,
      NON_PM, NON_PM, 0L, NO_COLOR),

    A("Excalibur", "gold-hilted runed long sword", LONG_SWORD, LONG_SWORD, (SPFX_NOGEN | SPFX_RESTR | SPFX_SEEK
                                | SPFX_DEFN | SPFX_INTEL | SPFX_SEARCH),
      0, 0, PHYS(5, 20), DRLI(0, 0), NO_CARY, 0, A_LAWFUL, PM_KNIGHT, NON_PM,
      4000L, NO_COLOR),
    /*
     *      Stormbringer only has a 5 because it can drain a level,
     *      providing 8 + CON bonus more.
     */
    A("Stormbringer", "black runesword", RUNESWORD, ELVEN_BROADSWORD,
      (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN | SPFX_INTEL | SPFX_DRLI), 0, 0,
      DRLI(5, 5), DRLI(0, 0), NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM, 8000L,
      NO_COLOR),
    /*
     *      Mjollnir will return to the hand of the wielder when thrown
     *      if the wielder is a Valkyrie wearing Gauntlets of Power.
     */
    A("Mjollnir", "shining runed war hammer", HEAVY_WAR_HAMMER, WAR_HAMMER, /* Mjo:llnir */
      (SPFX_RESTR | SPFX_ATTK), 0, 0, ELEC(5, 30), NO_DFNS, NO_CARY, 0,
      A_NEUTRAL, PM_VALKYRIE, NON_PM, 4000L, NO_COLOR),

	/* Note: Battle axe of cleaving is different, it doubles the effect of Tsurugi of Muramasa --JG */
    A("Cleaver", "runed double-headed axe", BATTLE_AXE, BATTLE_AXE, SPFX_RESTR, 0, 0, PHYS(3, 6), NO_DFNS, NO_CARY,
      0, A_NEUTRAL, PM_BARBARIAN, NON_PM, 1500L, NO_COLOR),

    /*
     *      Grimtooth glows in warning when elves are present, but its
     *      damage bonus applies to all targets rather than just elves
     *      (handled in the base weapon).
	 *		Added +1d5/+1d20 bonus against elves to improve the artifact, still does 3d3 to all targets from the base weapon --JG
     */
    A("Grimtooth", "runed crude dagger", ORNAMENTAL_ORCISH_DAGGER, ORCISH_DAGGER, 
	  (SPFX_RESTR | SPFX_WARN | SPFX_DFLAG2),
      0, M2_ELF, PHYS(5, 20), NO_DFNS,
      NO_CARY, 0, A_CHAOTIC, NON_PM, PM_ORC, 500L, CLR_RED),
    /*
     *      Orcrist and Sting have same alignment as elves.
     *
     *      The combination of SPFX_WARN+SPFX_DFLAG2+M2_value will trigger
     *      EWarn_of_mon for all monsters that have the M2_value flag.
     *      Sting and Orcrist will warn of M2_ORC monsters.
     */
    A("Orcrist", "ornate runed broadsword", ELVEN_BROADSWORD, ELVEN_BROADSWORD, (SPFX_WARN | SPFX_DFLAG2), 0, M2_ORC,
      PHYS(5, ART_TRIPLE_DAMAGE), NO_DFNS, NO_CARY, 0, A_CHAOTIC, NON_PM, PM_ELF, 2000L,
      CLR_BRIGHT_BLUE), /* bright blue is actually light blue */

    A("Sting", "ornate runed dagger", ELVEN_RUNEDAGGER, ELVEN_DAGGER, (SPFX_WARN | SPFX_DFLAG2), 0, M2_ORC, 
		PHYS(5, ART_DOUBLE_DAMAGE), NO_DFNS, NO_CARY, 0, A_CHAOTIC, NON_PM, PM_ELF, 1000L, CLR_BRIGHT_BLUE),
    /*
     *      Magicbane is a bit different!  Its magic fanfare
     *      unbalances victims in addition to doing some damage.
     */
    A("Magicbane", "runed athame", ATHAME, ATHAME, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      STUN(3, 10), DFNS(AD_MAGM), NO_CARY, 0, A_NEUTRAL, PM_WIZARD, NON_PM,
      3500L, NO_COLOR),

    A("Frost Brand", "iron-hilted runed long sword", LONG_SWORD, LONG_SWORD, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      COLD(5, ART_DOUBLE_DAMAGE), COLD(0, 0), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 3000L,
      NO_COLOR),

    A("Fire Brand", "copper-hilted runed long sword", LONG_SWORD, LONG_SWORD, (SPFX_RESTR | SPFX_ATTK | SPFX_DEFN), 0, 0,
      FIRE(5, ART_DOUBLE_DAMAGE), FIRE(0, 0), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 3000L,
      NO_COLOR),

    A("Dragonbane", "mithril-hilted runed broadsword", SWORD_OF_DRAGON_SLAYING, BROADSWORD,
      (SPFX_RESTR | SPFX_DCLAS | SPFX_REFLECT), 0, S_DRAGON,
      PHYS(5, 20), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 500L,
      NO_COLOR),

    A("Demonbane", "runed silver long sword", SWORD_OF_DEMON_SLAYING, SILVER_LONG_SWORD, (SPFX_RESTR | SPFX_DFLAG2), 0, M2_DEMON,
      PHYS(5, 20), NO_DFNS, NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 2500L,
      NO_COLOR),

    A("Werebane", "runed silver saber", SABER_OF_LYCANTHROPE_SLAYING, SILVER_SABER, (SPFX_RESTR | SPFX_DFLAG2), 0, M2_WERE,
      PHYS(5, 20), DFNS(AD_WERE), NO_CARY, 0, A_NONE, NON_PM, NON_PM, 1500L,
      NO_COLOR),

    A("Grayswandir", "diamond-encrusted silver saber", SILVER_SABER, SILVER_SABER, (SPFX_RESTR | SPFX_HALRES), 0, 0,
      PHYS(5, ART_DOUBLE_DAMAGE), NO_DFNS, NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 8000L,
      NO_COLOR),

    A("Giantslayer", "ruby-encrusted long sword", SWORD_OF_GIANT_SLAYING, LONG_SWORD, (SPFX_RESTR | SPFX_DFLAG2), 0, M2_GIANT,
      PHYS(5, 20), NO_DFNS, NO_CARY, 0, A_NEUTRAL, NON_PM, NON_PM, 500L,
      NO_COLOR),

    A("Ogresmasher", "gold-hilted war hammer", WAR_HAMMER_OF_OGRE_SLAYING, WAR_HAMMER, (SPFX_RESTR | SPFX_DCLAS), 0, S_OGRE,
      PHYS(5, 20), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 500L,
      NO_COLOR),

	A("Trollsbane", "runed morning star", MORNING_STAR_OF_TROLL_SLAYING, MORNING_STAR, (SPFX_RESTR | SPFX_REGEN | SPFX_DCLAS), 0, S_TROLL,
      PHYS(5, 20), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 500L,
      NO_COLOR),

	A("Gnollbane", "ruby-hilted war hammer", WAR_HAMMER_OF_GNOLL_SLAYING, WAR_HAMMER, (SPFX_RESTR | SPFX_DFLAG2), 0, M2_GNOLL,
      PHYS(5, 20), DFNS(AD_ELEC), NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 500L,
      NO_COLOR),

	A("Crossbow of the Gnoll Lords", "runed repeating heavy crossbow", REPEATING_HEAVY_CROSSBOW, REPEATING_HEAVY_CROSSBOW, (SPFX_RESTR | SPFX_STLTH), 0, 0,
      PHYS(5, 5), NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, PM_GNOLL, 2000L,
      NO_COLOR),

	A("Howling Flail", "gold-hilted runed flail", RUNED_FLAIL, FLAIL, (SPFX_RESTR | SPFX_LUCK), 0, 0,
		PHYS(5, 10), NO_DFNS, NO_CARY, 0, A_CHAOTIC, NON_PM, PM_GNOLL, 6000L,
		NO_COLOR),

	A("Wand of Orcus", "skull-headed wand-like mace", MACE_OF_DEATH, MACE_OF_DEATH, (SPFX_NOGEN | SPFX_RESTR), 0, 0,
		PHYS(5, 10), NO_DFNS, NO_CARY, WAND_OF_DEATH, A_CHAOTIC, NON_PM, NON_PM, 6000L,
		NO_COLOR),

	/*
     *      Two problems:  1) doesn't let trolls regenerate heads,
     *      2) doesn't give unusual message for 2-headed monsters (but
     *      allowing those at all causes more problems than worth the effort).
     */
    A("Vorpal Blade", "immaculate runed long sword", LONG_SWORD, LONG_SWORD, (SPFX_RESTR | SPFX_BEHEAD), 0, 0,
      PHYS(5, 1), NO_DFNS, NO_CARY, 0, A_NEUTRAL, NON_PM, NON_PM, 4000L,
      NO_COLOR),
    /*
     *      Ah, never shall I forget the cry,
     *              or the shriek that shrieked he,
     *      As I gnashed my teeth, and from my sheath
     *              I drew my Snickersnee!
     *                      --Koko, Lord high executioner of Titipu
     *                        (From Sir W.S. Gilbert's "The Mikado")
     */
    A("Snickersnee", "gold-hilted runed katana", KATANA, KATANA, SPFX_RESTR, 0, 0,
		PHYS(0, ART_DOUBLE_DAMAGE), NO_DFNS, NO_CARY,
      0, A_LAWFUL, PM_SAMURAI, NON_PM, 1200L, NO_COLOR),

    A("Sunsword", "shining long sword", LONG_SWORD, LONG_SWORD, (SPFX_RESTR | SPFX_DFLAG2), 0, M2_UNDEAD | M2_DEMON,
      PHYS(5, ART_QUADRUPLE_DAMAGE), DFNS(AD_BLND), NO_CARY, 0, A_LAWFUL, NON_PM, NON_PM, 1500L,
      NO_COLOR),

	/*
     *      The artifacts for the quest dungeon, all self-willed.
     */

    A("The Orb of Detection", "runed crystal ball", CRYSTAL_BALL, CRYSTAL_BALL,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), (SPFX_ESP | SPFX_HSPDAM), 0,
      NO_ATTK, NO_DFNS, CARY(AD_MAGM), INVISIBILITY, A_LAWFUL, PM_ARCHEOLOGIST,
      NON_PM, 2500L, NO_COLOR),

    A("The Heart of Ahriman", "glowing red jewel", LUCKSTONE, LUCKSTONE,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), SPFX_STLTH, 0,
      /* this stone does double damage if used as a projectile weapon */
      PHYS(5, ART_DOUBLE_DAMAGE), NO_DFNS, NO_CARY, LEVITATION, A_NEUTRAL, PM_BARBARIAN,
      NON_PM, 2500L, NO_COLOR),

    A("The Sceptre of Might", "diamond-encrusted sceptre", MACE, MACE,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_DALIGN), 0, 0, 
		PHYS(5, ART_DOUBLE_DAMAGE),
      DFNS(AD_MAGM), NO_CARY, CONFLICT, A_LAWFUL, PM_CAVEMAN, NON_PM, 2500L,
      NO_COLOR),

#if 0 /* OBSOLETE */
A("The Palantir of Westernesse",        CRYSTAL_BALL, CRYSTAL_BALL,
        (SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL),
                (SPFX_ESP|SPFX_REGEN|SPFX_HSPDAM), 0,
        NO_ATTK,        NO_DFNS,        NO_CARY,
        TAMING,         A_CHAOTIC, NON_PM , PM_ELF, 8000L, NO_COLOR ),
#endif

    A("The Staff of Aesculapius", "serpent-entwined quarterstaff", STAFF_OF_LIFE, QUARTERSTAFF,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_ATTK | SPFX_INTEL | SPFX_DRLI
       | SPFX_REGEN),
      0, 0, DRLI(0, 0), DRLI(0, 0), NO_CARY, HEALING, A_NEUTRAL, PM_HEALER,
      NON_PM, 5000L, NO_COLOR),

    A("The Magic Mirror of Merlin", "runed looking glass", MAGIC_MIRROR, MIRROR,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_SPEAK), SPFX_ESP, 0,
      NO_ATTK, NO_DFNS, CARY(AD_MAGM), 0, A_LAWFUL, PM_KNIGHT, NON_PM, 1500L,
      NO_COLOR),

    A("The Eyes of the Overworld", "runed eyeglasses", LENSES, LENSES,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_XRAY), 0, 0, NO_ATTK,
      DFNS(AD_MAGM), NO_CARY, ENLIGHTENING, A_NEUTRAL, PM_MONK, NON_PM,
      2500L, NO_COLOR),

    A("The Mitre of Holiness", "diamond-encrusted helmet", HELM_OF_BRILLIANCE, HELMET,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_DFLAG2 | SPFX_INTEL | SPFX_PROTECT), 0,
      M2_UNDEAD, NO_ATTK, NO_DFNS, CARY(AD_FIRE), ENERGY_BOOST, A_LAWFUL,
      PM_PRIEST, NON_PM, 2000L, NO_COLOR),

    A("The Longbow of Diana", "ornamental long bow", LONG_BOW, LONG_BOW,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_REFLECT), SPFX_ESP, 0,
      PHYS(10, ART_DOUBLE_DAMAGE), NO_DFNS, NO_CARY, CREATE_AMMO, A_CHAOTIC, PM_RANGER, NON_PM,
      4000L, NO_COLOR),

    /* MKoT has an additional carry property if the Key is not cursed (for
       rogues) or blessed (for non-rogues):  #untrap of doors and chests
       will always find any traps and disarming those will always succeed */
    A("The Master Key of Thievery", "ornamental key", SKELETON_KEY, SKELETON_KEY,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_SPEAK),
      (SPFX_WARN | SPFX_TCTRL | SPFX_HPHDAM), 0, NO_ATTK, NO_DFNS, NO_CARY,
      UNTRAP, A_CHAOTIC, PM_ROGUE, NON_PM, 3500L, NO_COLOR),

    A("The Tsurugi of Muramasa", "runed tsurugi", TSURUGI, TSURUGI,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_BEHEAD | SPFX_LUCK
       | SPFX_PROTECT),
      0, 0, PHYS(0, 8), NO_DFNS, NO_CARY, 0, A_LAWFUL, PM_SAMURAI, NON_PM,
      4500L, NO_COLOR),

    A("The Platinum Yendorian Express Card", "black credit card", CREDIT_CARD, CREDIT_CARD,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_DEFN),
      (SPFX_ESP | SPFX_HSPDAM), 0, NO_ATTK, NO_DFNS, CARY(AD_MAGM),
      CHARGE_OBJ, A_NEUTRAL, PM_TOURIST, NON_PM, 7000L, NO_COLOR),

    A("The Orb of Fate", "ornamental crystal ball", CRYSTAL_BALL, CRYSTAL_BALL,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL | SPFX_LUCK),
      (SPFX_WARN | SPFX_HSPDAM | SPFX_HPHDAM), 0, NO_ATTK, NO_DFNS, NO_CARY,
      LEV_TELE, A_NEUTRAL, PM_VALKYRIE, NON_PM, 3500L, NO_COLOR),

    A("The Eye of the Aethiopica", "eye-shaped amulet", AMULET_OF_ESP, AMULET,
      (SPFX_NOGEN | SPFX_RESTR | SPFX_INTEL), (SPFX_EREGEN | SPFX_HSPDAM), 0,
      NO_ATTK, DFNS(AD_MAGM), NO_CARY, CREATE_PORTAL, A_NEUTRAL, PM_WIZARD,
      NON_PM, 4000L, NO_COLOR),

    /*
     *  terminator; otyp must be zero
     */
    A(0, 0, 0, 0, 0, 0, 0, NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 0L,
      0) /* 0 is CLR_BLACK rather than NO_COLOR but it doesn't matter here */

}; /* artilist[] (or artifact_names[]) */

#undef A

#ifndef MAKEDEFS_C
#undef NO_ATTK
#undef NO_DFNS
#undef DFNS
#undef PHYS
#undef DRLI
#undef COLD
#undef FIRE
#undef ELEC
#undef STUN
#endif

/*artilist.h*/
