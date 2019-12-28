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
#define AF_ATTK		0x00000010L  /* item has a special attack (attk) */
#define AF_DEFN		0x00000020L  /* item has a special defence (defn) */
#define AF_DRLI		0x00000040L  /* drains a level from monsters */
#define AF_BEHEAD	0x00000400L  /* beheads monsters */
#define AF_BISECT	0x00000800L  /* bisects monsters */
#define AF_FAMOUS							0x00001000L  /* Name is always known */
#define AF_NAME_KNOWN_WHEN_PICKED_UP		0x00002000L  /* Ditto */
#define AF_NAME_KNOWN_WHEN_WORN_OR_WIELDED	0x00004000L  /* Ditto */

/* monster class flags and mask here */
#define AF_DMONS	0x00100000L  /* attack bonus on one monster type */
#define AF_DCLAS	0x00200000L  /* attack bonus on monsters w/ symbol mtype */
#define AF_DFLAG1	0x00400000L  /* attack bonus on monsters w/ mflags1 flag */
#define AF_DFLAG2	0x00800000L  /* attack bonus on monsters w/ mflags2 flag */
#define AF_DALIGN	0x01000000L  /* attack bonus on non-aligned monsters  */
#define AF_DBONUS	0x01F00000L  /* attack bonus mask */

/* wielded or carried special effects */
#define SPFX_NONE				0x00000000L  /* no special effects, just a bonus */
#define SPFX_SEEK				0x00000010L  /* item helps you search for things */
#define SPFX_WARN				0x00000020L  /* item warns you of danger */
#define SPFX_SEARCH				0x00000040L  /* helps searching */
#define SPFX_HALRES				0x00000080L  /* blocks hallucinations */
#define SPFX_ESP				0x00000100L  /* ESP (like amulet of ESP) */
#define SPFX_STLTH				0x00000200L  /* Stealth */
#define SPFX_REGEN				0x00000400L  /* Regeneration */
#define SPFX_EREGEN				0x00000800L  /* Energy Regeneration */
#define SPFX_HSPDAM				0x00001000L  /* 1/2 spell damage (on player) in combat */
#define SPFX_HPHDAM				0x00002000L  /* 1/2 physical damage (on player) in combat */
#define SPFX_TCTRL				0x00004000L  /* Teleportation Control */
#define SPFX_LUCK				0x00008000L  /* Increase Luck (like Luckstone) */
#define SPFX_XRAY				0x00010000L  /* gives X-RAY vision to player */
#define SPFX_REFLECT			0x00020000L  /* Reflection */
#define SPFX_PROTECT			0x00040000L  /* Protection */
#define SPFX_AGGRAVATE_MONSTER	0x00080000L  /* Gives aggravate monster */
#define SPFX_UNLUCK				0x00100000L  /* Gives brings bad luck */
#define SPFX_BLIND_SEEING		0x00200000L  /* Blocks blindness */

struct artifact {
    short otyp;
	short maskotyp;
	const char *name;
	const char* desc;	/* unidentified name */
	unsigned long aflags;  /* artifact flags */
	unsigned long spfx;  /* special effect from wielding/wearing */
    unsigned long cspfx; /* special effect just from carrying obj */
    unsigned long mtype; /* monster type, symbol, or flag */
    struct attack attk, defn, cary;
    uchar inv_prop;     /* property obtained by invoking artifact */
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
	WAND_OF_DEATH
};

#endif /* ARTIFACT_H */
