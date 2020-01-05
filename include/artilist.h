/* GnollHack 4.0  artilist.h      $NHDT-Date: 1433050874 2015/05/31 05:41:14 $  $NHDT-Branch: master $:$NHDT-Revision: 1.16 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2017. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifdef MAKEDEFS_C
/* in makedefs.c, all we care about is the list of names */

#define A(nam, desc, typ, masktyp, aflags, spfx, cspfx, mt, tohitdice, tohitsize, tohitplus, atk, dfn, cry, inv, al, cl, rac, cost, glowclr, objclr) nam

static const char *artifact_names[] = {
#else
/* in artifact.c, set up the actual artifact list structure */

#define A(nam, desc, typ, masktyp, aflags, spfx, cspfx, mt, tohitdice, tohitsize, tohitplus, atk, dfn, cry, inv, al, cl, rac, cost, glowclr, objclr) \
    {                                                                       \
        typ, masktyp, nam, desc, aflags, spfx, cspfx, mt, tohitdice, tohitsize, tohitplus, atk, dfn, cry, inv, al, cl, rac, cost, glowclr, objclr    \
    }

/* clang-format off */
#define     NO_ATTK			{0,0,0,0,0,0}               /* no attack */
#define     PHYS(a,b)		{0,AD_PHYS,a,b,0,0}         /* physical */
#define     PHYSI(a,b,c)	{0,AD_PHYS,a,b,c,0}         /* physical */
#define     DRLI(a,b)		{0,AD_DRLI,a,b,0,0}         /* life drain */
#define     COLD(a,b)		{0,AD_COLD,a,b,0,0}
#define     FIRE(a,b)		{0,AD_FIRE,a,b,0,0}
#define     ELEC(a,b)		{0,AD_ELEC,a,b,0,0}         /* electrical shock */
#define     STUN(a,b)		{0,AD_STUN,a,b,0,0}         /* magical attack */
/* clang-format on */

#define ARTDMG_DOUBLE_DAMAGE -1
#define ARTDMG_TRIPLE_DAMAGE -2
#define ARTDMG_QUADRUPLE_DAMAGE -3

#define None (char *)0

STATIC_OVL NEARDATA struct artifact artilist[] = {
#endif /* MAKEDEFS_C */

    /* Artifact cost rationale:
     * 1.  The more useful the artifact, the better its cost.
     * 2.  Quest artifacts are highly valued.
     * 3.  Chaotic artifacts are inflated due to scarcity (and balance).
     */

    /*  dummy element #0, so that all interesting indices are non-zero */
    A("", None, STRANGE_OBJECT, STRANGE_OBJECT, AF_NONE, SPFX_NONE, SPFX_NONE, 0, 0, 0, 0, NO_ATTK, NO_POWER, NO_POWER, 0, A_NONE,
      NON_PM, NON_PM, 0L, NO_COLOR, NO_COLOR),

    A("Excalibur", "gold-hilted runed long sword", LONG_SWORD, LONG_SWORD, 
  	  AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS | AF_MONSTERS_CAN_TRACK_ARTIFACT | AF_ANGERS_DEMONS,
	  (SPFX_SEEK | SPFX_SEARCH), SPFX_NONE, 0, 
	  1, 5, 0, PHYS(1, 20), DRAIN_RES, NO_POWER, 0, A_LAWFUL, PM_KNIGHT, NON_PM, 4000L, 
	  NO_COLOR, NO_COLOR),
    /*
     *      Stormbringer only has a 5 because it can drain a level,
     *      providing 8 + CON bonus more.
     */
    A("Stormbringer", "black runesword", RUNESWORD, ELVEN_BROADSWORD, 
		(AF_RESTR | AF_INTEL | AF_DRLI | AF_NAME_KNOWN_WHEN_PICKED_UP | AF_BLOODTHIRSTY),
		SPFX_NONE, SPFX_NONE, 0, 
		1, 5, 0, DRLI(1, 5), DRAIN_RES, NO_POWER, NO_POWER, A_CHAOTIC, NON_PM, NON_PM, 8000L,
		NO_COLOR, CLR_BLACK),
    /*
     *      Mjollnir will return to the hand of the wielder when thrown
     *      if the wielder is a Valkyrie wearing Gauntlets of Power.
     */
    A("Mjollnir", "shining runed war hammer", HEAVY_WAR_HAMMER, WAR_HAMMER, /* Mjo:llnir */
	   (AF_RESTR | AF_FAMOUS), SPFX_NONE, SPFX_NONE, 0, 
	   1, 5, 0, ELEC(1, 30), NO_POWER, NO_POWER, NO_POWER,
      A_NEUTRAL, PM_VALKYRIE, NON_PM, 4000L, 
	  NO_COLOR, NO_COLOR),

	/* Note: Battle axe of cleaving is different, it mimicks the effect of Tsurugi of Muramasa --JG */
    A("Cleaver", "runed double-headed axe", BATTLE_AXE, BATTLE_AXE,  AF_RESTR | AF_HITS_ADJACENT_SQUARES, SPFX_NONE, SPFX_NONE, 0,
	  1, 3, 0, PHYS(1, 6), NO_POWER, NO_POWER, NO_POWER, A_NEUTRAL, PM_BARBARIAN, NON_PM, 1500L, 
	  NO_COLOR, NO_COLOR),

    /*
     *      Grimtooth glows in warning when elves are present, but its
     *      damage bonus applies to all targets rather than just elves
     *      (handled in the base weapon).
	 *		Added +1d5/+1d20 bonus against elves to improve the artifact, still does 3d3 to all targets from the base weapon --JG
     */
    A("Grimtooth", "runed crude dagger", ORNAMENTAL_ORCISH_DAGGER, ORCISH_DAGGER, 
	  (AF_RESTR | AF_DFLAG2), SPFX_WARN_OF_MON, SPFX_NONE, M2_ELF, 
	  1, 5, 0, PHYS(1, 20), NO_POWER, NO_POWER, NO_POWER, A_CHAOTIC, NON_PM, PM_ORC, 500L, 
	  CLR_RED, NO_COLOR),
    /*
     *      Orcrist and Sting have same alignment as elves.
     *
     *      The combination of SPFX_WARN_OF_MON+SPFX_DFLAG2+M2_value will trigger
     *      EWarn_of_mon for all monsters that have the M2_value flag.
     *      Sting and Orcrist will warn of M2_ORC monsters.
     */
    A("Orcrist", "ornate runed broadsword", ELVEN_BROADSWORD, ELVEN_BROADSWORD, AF_DFLAG2, SPFX_WARN_OF_MON, SPFX_NONE, M2_ORC,
	  1, 5, 0, PHYS(ARTDMG_TRIPLE_DAMAGE, 0), NO_POWER, NO_POWER, NO_POWER,
	  A_CHAOTIC, NON_PM, PM_ELF, 2000L,
      CLR_BRIGHT_BLUE, NO_COLOR), /* bright blue is actually light blue */

    A("Sting", "ornate runed dagger", ELVEN_RUNEDAGGER, ELVEN_DAGGER, (AF_DFLAG2), SPFX_WARN_OF_MON, SPFX_NONE, M2_ORC,
		 1, 5, 0, PHYS(ARTDMG_DOUBLE_DAMAGE, 0), NO_POWER, NO_POWER, NO_POWER, A_CHAOTIC, NON_PM, PM_ELF, 1000L, CLR_BRIGHT_BLUE, NO_COLOR),
    /*
     *      Magicbane is a bit different!  Its magic fanfare
     *      unbalances victims in addition to doing some damage.
     */
    A("Magicbane", "runed athame", ATHAME, ATHAME, (AF_RESTR | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED | AF_MAGIC_ABSORBING), 
		SPFX_NONE, SPFX_NONE, 0,
		1, 3, 0, STUN(1, 10), ANTIMAGIC, NO_POWER, NO_POWER, A_NEUTRAL, PM_WIZARD, NON_PM, 3500L, 
		NO_COLOR, NO_COLOR),

    A("Frost Brand", "iron-hilted runed long sword", LONG_SWORD, LONG_SWORD, (AF_RESTR | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_NONE, SPFX_NONE, 0,
	  1, 5, 0, COLD(ARTDMG_DOUBLE_DAMAGE, 0), COLD_RES, NO_POWER, NO_POWER, A_NONE, NON_PM, NON_PM, 3000L,
      NO_COLOR, CLR_WHITE),

    A("Fire Brand", "copper-hilted runed long sword", LONG_SWORD, LONG_SWORD, (AF_RESTR | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_NONE, SPFX_NONE, 0,
	  1, 5, 0, FIRE(ARTDMG_DOUBLE_DAMAGE, 0), FIRE_RES, NO_POWER, NO_POWER, A_NONE, NON_PM, NON_PM, 3000L,
      NO_COLOR, CLR_RED),

    A("Dragonbane", "mithril-hilted runed broadsword", SWORD_OF_DRAGON_SLAYING, BROADSWORD,
	  (AF_RESTR | AF_DCLAS | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_REFLECT, SPFX_NONE, S_DRAGON,
	  1, 5, 0, PHYS(1, 20), NO_POWER, NO_POWER, NO_POWER, A_NONE, NON_PM, NON_PM, 500L,
      NO_COLOR, NO_COLOR),

    A("Demonbane", "runed silver long sword", SWORD_OF_DEMON_SLAYING, SILVER_LONG_SWORD, 
	(AF_RESTR | AF_DFLAG2 | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED | AF_ANGERS_DEMONS | AF_PREVENTS_SUMMONING_OF_MON),
	SPFX_NONE, SPFX_NONE, M2_DEMON,
	  1, 5, 0, PHYS(1, 20), NO_POWER, NO_POWER, NO_POWER, A_LAWFUL, NON_PM, NON_PM, 2500L,
      NO_COLOR, NO_COLOR),

    A("Werebane", "runed silver saber", SABER_OF_LYCANTHROPE_SLAYING, SILVER_SABER, (AF_RESTR | AF_DFLAG2 | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_NONE, SPFX_NONE, M2_WERE,
	  1, 5, 0, PHYS(1, 20), LYCANTHROPY_RES, NO_POWER, NO_POWER, A_NONE, NON_PM, NON_PM, 1500L,
      NO_COLOR, NO_COLOR),

    A("Grayswandir", "diamond-encrusted silver saber", SILVER_SABER, SILVER_SABER, (AF_RESTR | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_HALRES, SPFX_NONE, 0,
	  1, 5, 0, PHYS(1, ARTDMG_DOUBLE_DAMAGE), NO_POWER, NO_POWER, NO_POWER, A_LAWFUL, NON_PM, NON_PM, 8000L,
      NO_COLOR, CLR_GRAY),

    A("Giantslayer", "ruby-encrusted long sword", SWORD_OF_GIANT_SLAYING, LONG_SWORD, (AF_RESTR | AF_DFLAG2 | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_NONE, SPFX_NONE, M2_GIANT,
	  1, 5, 0, PHYS(1, 20), NO_POWER, NO_POWER, NO_POWER, A_NEUTRAL, NON_PM, NON_PM, 500L,
      NO_COLOR, NO_COLOR),

    A("Ogresmasher", "gold-hilted war hammer", WAR_HAMMER_OF_OGRE_SLAYING, WAR_HAMMER, (AF_RESTR | AF_DCLAS | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_NONE, SPFX_NONE, S_OGRE,
	  1, 5, 0, PHYS(1, 20), NO_POWER, NO_POWER, NO_POWER, A_NONE, NON_PM, NON_PM, 500L,
      NO_COLOR, NO_COLOR),

	A("Trollsbane", "runed morning star", MORNING_STAR_OF_TROLL_SLAYING, MORNING_STAR, (AF_RESTR | AF_DCLAS | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_REGEN, SPFX_NONE, S_TROLL,
	  1, 5, 0, PHYS(1, 20), NO_POWER, NO_POWER, NO_POWER, A_NONE, NON_PM, NON_PM, 500L,
      NO_COLOR, NO_COLOR),

	A("Gnollbane", "ruby-hilted war hammer", WAR_HAMMER_OF_GNOLL_SLAYING, WAR_HAMMER, (AF_RESTR | AF_DFLAG2 | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_NONE, SPFX_NONE, M2_GNOLL,
		1, 5, 0, PHYS(1, 20), SHOCK_RES, NO_POWER, NO_POWER, A_LAWFUL, NON_PM, NON_PM, 500L,
		NO_COLOR, NO_COLOR),

	A("Crossbow of the Gnoll Lords", "runed repeating heavy crossbow", REPEATING_HEAVY_CROSSBOW, REPEATING_HEAVY_CROSSBOW, (AF_RESTR | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_STLTH , SPFX_NONE, 0,
		1, 5, 0, PHYS(1, 5), NO_POWER, NO_POWER, NO_POWER, A_NONE, NON_PM, PM_GNOLL, 2000L,
		NO_COLOR, NO_COLOR),

	A("Howling Flail", "gold-hilted runed flail", RUNED_FLAIL, FLAIL, (AF_RESTR | AF_NAME_KNOWN_WHEN_INVOKED), SPFX_LUCK, SPFX_NONE, 0,
		1, 5, 0, PHYS(1, 10), NO_POWER, NO_POWER, TAMING, A_CHAOTIC, NON_PM, PM_GNOLL, 6000L,
		NO_COLOR, HI_GOLD),

	A("Wand of Orcus", "skull-headed obsidian mace", MACE_OF_DEATH, MACE_OF_DEATH, (AF_NOGEN | AF_RESTR | AF_NAME_KNOWN_WHEN_INVOKED), SPFX_NONE, SPFX_NONE, 0,
		1, 5, 0, PHYS(1, 10), NO_POWER, NO_POWER, WAND_OF_DEATH, A_CHAOTIC, NON_PM, NON_PM, 6000L, 
		NO_COLOR, CLR_BLACK),

	/*
     *      Two problems:  1) doesn't let trolls regenerate heads,
     *      2) doesn't give unusual message for 2-headed monsters (but
     *      allowing those at all causes more problems than worth the effort).
     */
    A("Vorpal Blade", "immaculate runed long sword", LONG_SWORD, LONG_SWORD, (AF_RESTR | AF_BEHEAD | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED), SPFX_NONE, SPFX_NONE, 0,
		1, 5, 0, PHYSI(0, 0, 1), NO_POWER, NO_POWER, NO_POWER, A_NEUTRAL, NON_PM, NON_PM, 4000L, 
		NO_COLOR, NO_COLOR),
    /*
     *      Ah, never shall I forget the cry,
     *              or the shriek that shrieked he,
     *      As I gnashed my teeth, and from my sheath
     *              I drew my Snickersnee!
     *                      --Koko, Lord high executioner of Titipu
     *                        (From Sir W.S. Gilbert's "The Mikado")
     */
    A("Snickersnee", "gold-hilted runed katana", KATANA, KATANA, AF_RESTR, SPFX_NONE, SPFX_NONE, 0,
		0, 0, 0, PHYS(ARTDMG_DOUBLE_DAMAGE, 0), NO_POWER, NO_POWER, NO_POWER, A_LAWFUL, PM_SAMURAI, NON_PM, 1200L, 
		NO_COLOR, HI_GOLD),

    A("Sunsword", "glowing runed long sword", LONG_SWORD, LONG_SWORD, (AF_RESTR | AF_DFLAG2 | AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED | AF_ANGERS_DEMONS), SPFX_NONE, SPFX_NONE, M2_UNDEAD | M2_DEMON,
		1, 5, 0, PHYS(ARTDMG_QUADRUPLE_DAMAGE, 0), FLASH_RES, NO_POWER, NO_POWER, A_LAWFUL, NON_PM, NON_PM, 1500L, 
		NO_COLOR, HI_GOLD),
		  
    A("The One Ring", "plain golden", RIN_SUPREME_POWER, RIN_SUPREME_POWER, (AF_RESTR | AF_INTEL | AF_MONSTERS_CAN_TRACK_ARTIFACT),
		SPFX_NONE, SPFX_AGGRAVATE_MONSTER | SPFX_UNLUCK, 0,
		0, 0, 0, NO_ATTK, NO_POWER, NO_POWER, CONFLICT, A_NONE, NON_PM, NON_PM, 10000L, 
		NO_COLOR, HI_GOLD),

	/*
     *      The artifacts for the quest dungeon, all self-willed.
     */

    A("The Orb of Detection", "runed crystal ball", CRYSTAL_BALL, CRYSTAL_BALL, //STARTMARKER FOR QUEST ARTIFACTS
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS), SPFX_NONE, (SPFX_ESP | SPFX_HSPDAM), 0,
		0, 0, 0, NO_ATTK, NO_POWER, ANTIMAGIC, INVISIBILITY, A_LAWFUL, PM_ARCHEOLOGIST, NON_PM, 2500L, 
		NO_COLOR, CLR_MAGENTA),

    A("The Heart of Ahriman", "glowing red jewel", LUCKSTONE, LUCKSTONE,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS), SPFX_NONE, SPFX_STLTH, 0,
      /* this stone does double damage if used as a projectile weapon */
		1, 5, 0, PHYS(ARTDMG_DOUBLE_DAMAGE, 0), NO_POWER, NO_POWER, LEVITATION, A_NEUTRAL, PM_BARBARIAN, NON_PM, 2500L, 
		NO_COLOR, CLR_RED),

    A("The Sceptre of Might", "diamond-encrusted sceptre", MACE, MACE,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_DALIGN | AF_FAMOUS), SPFX_NONE, SPFX_NONE, 0,
		1, 5, 0, PHYS(ARTDMG_DOUBLE_DAMAGE, 0), ANTIMAGIC, NO_POWER, CONFLICT, A_LAWFUL, PM_CAVEMAN, NON_PM, 2500L, 
		NO_COLOR, HI_GOLD),

#if 0 /* OBSOLETE */
A("The Palantir of Westernesse",        CRYSTAL_BALL, CRYSTAL_BALL,
	AF_NONE, (SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL),
                (SPFX_ESP|SPFX_REGEN|SPFX_HSPDAM), 0,
        NO_ATTK,        NO_POWER,        NO_POWER,
        TAMING,         A_CHAOTIC, NON_PM , PM_ELF, 8000L, NO_COLOR ),
#endif

    A("The Staff of Aesculapius", "serpent-entwined quarterstaff", STAFF_OF_LIFE, QUARTERSTAFF,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_DRLI | AF_FAMOUS), SPFX_REGEN, SPFX_NONE, 0, 
		0, 0, 0, DRLI(0,0), DRAIN_RES, NO_POWER, HEALING, A_NEUTRAL, PM_HEALER, NON_PM, 5000L, 
		NO_COLOR, CLR_MAGENTA),

    A("The Magic Mirror of Merlin", "runed looking glass", MAGIC_MIRROR, MIRROR,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_SPEAK | AF_FAMOUS), SPFX_NONE, SPFX_ESP, 0,
		0, 0, 0, NO_ATTK, NO_POWER, ANTIMAGIC, 0, A_LAWFUL, PM_KNIGHT, NON_PM, 1500L, 
		NO_COLOR, CLR_MAGENTA),

    A("The Eyes of the Overworld", "runed eyeglasses", LENSES, LENSES,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS), (SPFX_XRAY | SPFX_BLIND_SEEING), SPFX_NONE, 0, 
		0, 0, 0, NO_ATTK, ANTIMAGIC, NO_POWER, ENLIGHTENING, A_NEUTRAL, PM_MONK, NON_PM, 2500L, 
		NO_COLOR, CLR_MAGENTA),

    A("The Mitre of Holiness", "diamond-encrusted helmet", HELM_OF_BRILLIANCE, HELMET,
	  (AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS), SPFX_PROTECT | SPFX_HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS, SPFX_NONE, 0, 
		0, 0, 0, NO_ATTK, NO_POWER, FIRE_RES, ENERGY_BOOST, A_LAWFUL, PM_PRIEST, NON_PM, 2000L, 
		NO_COLOR, CLR_MAGENTA),

    A("The Longbow of Diana", "ornamental long bow", LONG_BOW, LONG_BOW,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS), SPFX_REFLECT, SPFX_ESP, 0,
		1, 10, 0, PHYS(ARTDMG_DOUBLE_DAMAGE, 0), NO_POWER, NO_POWER, CREATE_AMMO, A_CHAOTIC, PM_RANGER, NON_PM, 4000L, 
		NO_COLOR, CLR_MAGENTA),

    /* MKoT has an additional carry property if the Key is not cursed (for
       rogues) or blessed (for non-rogues):  #untrap of doors and chests
       will always find any traps and disarming those will always succeed */
    A("The Master Key of Thievery", "ornamental key", SKELETON_KEY, SKELETON_KEY,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_SPEAK | AF_FAMOUS), SPFX_NONE, (SPFX_WARNING | SPFX_TCTRL | SPFX_HPHDAM), 0, 
		0, 0, 0, NO_ATTK, NO_POWER, NO_POWER, UNTRAP, A_CHAOTIC, PM_ROGUE, NON_PM, 3500L, 
		NO_COLOR, CLR_MAGENTA),

    A("The Tsurugi of Muramasa", "runed tsurugi", TSURUGI, TSURUGI,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_BISECT | AF_FAMOUS), (SPFX_LUCK | SPFX_PROTECT), SPFX_NONE, 0, 
		0, 0, 0, PHYS(1, 8), NO_POWER, NO_POWER, NO_POWER, A_LAWFUL, PM_SAMURAI, NON_PM, 4500L, 
		NO_COLOR, CLR_MAGENTA),

    A("The Platinum Yendorian Express Card", "black credit card", CREDIT_CARD, CREDIT_CARD,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS), SPFX_NONE, (SPFX_ESP | SPFX_HSPDAM), 0, 
		0, 0, 0, NO_ATTK, NO_POWER, ANTIMAGIC, CHARGE_OBJ, A_NEUTRAL, PM_TOURIST, NON_PM, 7000L, 
		NO_COLOR, CLR_BLACK),

    A("The Orb of Fate", "ornamental crystal ball", CRYSTAL_BALL, CRYSTAL_BALL,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS | AF_READABLE), SPFX_NONE, (SPFX_WARNING | SPFX_HSPDAM | SPFX_HPHDAM | SPFX_LUCK), 0, 
		0, 0, 0, NO_ATTK, NO_POWER, NO_POWER, LEV_TELE, A_NEUTRAL, PM_VALKYRIE, NON_PM, 3500L, 
		NO_COLOR, CLR_MAGENTA),

    A("The Eye of the Aethiopica", "eye-shaped amulet", AMULET_OF_ESP, AMULET,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS), SPFX_NONE, (SPFX_EREGEN | SPFX_HSPDAM), 0,
		0, 0, 0, NO_ATTK, ANTIMAGIC, NO_POWER, CREATE_PORTAL, A_NEUTRAL, PM_WIZARD, NON_PM, 4000L, 
		NO_COLOR, CLR_MAGENTA),

	A("The Ark of the Covenant", "gold-covered wooden chest", GOLDEN_CHEST, GOLDEN_CHEST,
		(AF_NOGEN | AF_RESTR | AF_INTEL | AF_FAMOUS | AF_NO_WISH), SPFX_NONE, (SPFX_LUCK | SPFX_PROTECT), 0,
		0, 0, 0, NO_ATTK, NO_POWER, ANTIMAGIC, BLESS_CONTENTS, A_LAWFUL, NON_PM, /* PM_ARCHEOLOGIST */ NON_PM, 5000L, 
		NO_COLOR, HI_GOLD),


    /*
     *  terminator; otyp must be zero
     */
    A(0, 0, 0, 0, AF_NONE, SPFX_NONE, SPFX_NONE, 0, 0, 0, 0, NO_ATTK, NO_POWER, NO_POWER, NO_POWER, A_NONE, NON_PM, NON_PM, 0L,
      0, 0) /* 0 is CLR_BLACK rather than NO_COLOR but it doesn't matter here */

}; /* artilist[] (or artifact_names[]) */

#undef A

#ifndef MAKEDEFS_C
#undef NO_ATTK
#undef NO_POWER
#undef DFNS
#undef PHYS
#undef DRLI
#undef COLD
#undef FIRE
#undef ELEC
#undef STUN
#endif

/*artilist.h*/
