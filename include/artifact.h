/* GnollHack 4.0	artifact.h	$NHDT-Date: 1433050871 2015/05/31 05:41:11 $  $NHDT-Branch: master $:$NHDT-Revision: 1.11 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef ARTIFACT_H
#define ARTIFACT_H

/* general and attack-based flags */
#define AF_NONE		0x00000000L  /* no special effects, just a bonus */
#define AF_NOGEN	0x00000001L  /* item is special, bequeathed by gods */
#define AF_RESTR	0x00000002L  /* item is restricted - can't be named */
#define AF_INTEL	0x00000004L  /* item is self-willed - intelligent */
#define AF_SPEAK	0x00000008L  /* item can speak (finally implemented) */
#define AF_DRLI		0x00000010L  /* drains a level from monsters */
#define AF_BEHEAD	0x00000020L  /* beheads monsters */
#define AF_BISECT	0x00000040L  /* bisects monsters */
#define AF_HITS_ADJACENT_SQUARES			0x00000080L  /* hits adjacent squares (like Cleaver) */
#define AF_BLOODTHIRSTY						0x00000100L  /* no attack confirmation (like Stormbringer) */
#define AF_MAGIC_ABSORBING					0x00000200L  /* absorbs curses (like Magicbane) */
#define AF_PREVENTS_REVIVAL_OF_MON			0x00000400L  /* prevents revival of specified monsters (like Trollsbane) */
#define AF_PREVENTS_SUMMONING_OF_MON		0x00000800L  /* prevents summoning by specified monsters (like Demonbane) */
#define AF_MONSTERS_CAN_TRACK_ARTIFACT		0x00001000L  /* Ditto */
#define AF_ANGERS_DEMONS					0x00002000L  /* Ditto */
#define AF_SHINES_MAGICAL_LIGHT				0x00004000L  /* shines like Sunsword when wielded */
#define AF_FLICKERS							0x00008000L  /* flickers like Sting */
#define AF_FLICKERS_WHEN_CARRIED			0x00010000L  /* flickers also when just carried */
/* free bit */
/* free bit */
/* free bit */

/* monster class flags and mask here */
#define AF_DMONS							0x00100000L  /* attack bonus on one monster type */
#define AF_DCLAS							0x00200000L  /* attack bonus on monsters w/ symbol mtype */
#define AF_DFLAG1							0x00400000L  /* attack bonus on monsters w/ mflags1 flag */
#define AF_DFLAG2							0x00800000L  /* attack bonus on monsters w/ mflags2 flag */
#define AF_DALIGN							0x01000000L  /* attack bonus on non-aligned monsters  */
#define AF_DBONUS							0x01F00000L  /* attack bonus mask */

#define AF_FAMOUS							0x02000000L  /* Name is always known */
#define AF_NAME_KNOWN_WHEN_PICKED_UP		0x04000000L  /* Ditto */
#define AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED	0x08000000L  /* Ditto */
#define AF_NAME_KNOWN_WHEN_INVOKED			0x10000000L  /* Ditto */
/* free bit */
#define AF_READABLE							0x40000000L  /* readable even if base item is not */
#define AF_NO_WISH							0x80000000L  /* not wishable, not that for artifacts the base item's no wish does not apply */


/* wielded or carried special effects */
#define SPFX_NONE				0x00000000L  /* No special effects, just a bonus */
#define SPFX_SEEK				0x00000010L  /* Item helps you search for things */
#define SPFX_WARN_OF_MON		0x00000020L  /* Item warns you of a specific monster type; also always flickers */
#define SPFX_SEARCH				0x00000040L  /* Helps searching */
#define SPFX_HALRES				0x00000080L  /* Blocks hallucinations */
#define SPFX_ESP				0x00000100L  /* ESP (like amulet of ESP) */
#define SPFX_STLTH				0x00000200L  /* Stealth */
#define SPFX_REGEN				0x00000400L  /* Regeneration */
#define SPFX_EREGEN				0x00000800L  /* Energy Regeneration */
#define SPFX_HSPDAM				0x00001000L  /* 1/2 spell damage (on player) in combat */
#define SPFX_HPHDAM				0x00002000L  /* 1/2 physical damage (on player) in combat */
#define SPFX_TCTRL				0x00004000L  /* Teleportation Control */
#define SPFX_LUCK				0x00008000L  /* Increase Luck (like Luckstone) */
#define SPFX_XRAY				0x00010000L  /* Gives X-RAY vision to player */
#define SPFX_REFLECT			0x00020000L  /* Reflection */
#define SPFX_PROTECT			0x00040000L  /* Protection */
#define SPFX_AGGRAVATE_MONSTER	0x00080000L  /* Gives aggravate monster */
#define SPFX_UNLUCK				0x00100000L  /* Gives brings bad luck */
#define SPFX_BLIND_SEEING		0x00200000L  /* Blocks blindness */
#define SPFX_HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS \
								0x00400000L  /* Ditto, for Mitre of Holiness */
#define SPFX_WARNING			0x00800000L  /* Item warns you of danger */
#define SPFX_ORC_WARNING		0x01000000L  /* Item warns you of orcs */
#define SPFX_ELF_WARNING		0x02000000L  /* Item warns you of elves */

struct artifact {
    short otyp;
	short maskotyp;
	const char *name;
	const char* desc;	/* unidentified name */
	unsigned long aflags;  /* artifact flags */
	unsigned long spfx;  /* special effect from wielding/wearing */
    unsigned long cspfx; /* special effect just from carrying obj */
    unsigned long mtype; /* monster type, symbol, or flag */
	int tohit_dice;
	int tohit_diesize;
	int tohit_plus;
	struct attack attk;
	int worn_prop;		/* property conferred when wielded/worn */
	int carried_prop;	/* property conferred when carried */
	int inv_prop;       /* property obtained by invoking artifact */
    aligntyp alignment; /* alignment of bequeathing gods */
    short role;         /* character role associated with */
    short race;         /* character race associated with */
    long cost; /* price when sold to hero (default 100 x base cost) */
    char acolor;        /* color to use if artifact 'glows' */
	char ocolor;		/* color used for object instead of its normal color */
};

/* invoked properties with special powers */
enum invoke_prop_types {
    TAMING = (LAST_PROP + 1),
    HEALING,
    ENERGY_BOOST,
    UNTRAP,
    CHARGE_OBJ,
    LEV_TELE,
    CREATE_PORTAL,
    ENLIGHTENING,
    CREATE_AMMO,
	WAND_OF_DEATH,
	BLESS_CONTENTS
};

#endif /* ARTIFACT_H */
