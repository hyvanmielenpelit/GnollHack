/* GnollHack 4.0	monflag.h	$NHDT-Date: 1432512778 2015/05/25 00:12:58 $  $NHDT-Branch: master $:$NHDT-Revision: 1.14 $ */
/* Copyright (c) 1989 Mike Threepoint				  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MONFLAG_H
#define MONFLAG_H

#define MS_SILENT 0     /* makes no sound */
#define MS_BARK 1       /* if full moon, may howl */
#define MS_MEW 2        /* mews or hisses */
#define MS_ROAR 3       /* roars */
#define MS_GROWL 4      /* growls */
#define MS_SQEEK 5      /* squeaks, as a rodent */
#define MS_SQAWK 6      /* squawks, as a bird */
#define MS_HISS 7       /* hisses */
#define MS_BUZZ 8       /* buzzes (killer bee) */
#define MS_GRUNT 9      /* grunts (or speaks own language) */
#define MS_NEIGH 10     /* neighs, as an equine */
#define MS_WAIL 11      /* wails, as a tortured soul */
#define MS_GURGLE 12    /* gurgles, as liquid or through saliva */
#define MS_BURBLE 13    /* burbles (jabberwock) */
#define MS_ANIMAL 13    /* up to here are animal noises */
#define MS_SHRIEK 15    /* wakes up others */
#define MS_BONES 16     /* rattles bones (skeleton) */
#define MS_LAUGH 17     /* grins, smiles, giggles, and laughs */
#define MS_MUMBLE 18    /* says something or other */
#define MS_IMITATE 19   /* imitates others (leocrotta) */
#define MS_ORC MS_GRUNT /* intelligent brutes */
#define MS_HUMANOID 20  /* generic traveling companion */
#define MS_ARREST 21    /* "Stop in the name of the law!" (Kops) */
#define MS_SOLDIER 22   /* army and watchmen expressions */
#define MS_GUARD 23     /* "Please drop that gold and follow me." */
#define MS_DJINNI 24    /* "Thank you for freeing me!" */
#define MS_NURSE 25     /* "Take off your shirt, please." */
#define MS_SEDUCE 26    /* "Hello, sailor." (Nymphs) */
#define MS_VAMPIRE 27   /* vampiric seduction, Vlad's exclamations */
#define MS_BRIBE 28     /* asks for money, or berates you */
#define MS_CUSS 29      /* berates (demons) or intimidates (Wiz) */
#define MS_RIDER 30     /* astral level special monsters */
#define MS_LEADER 31    /* your class leader */
#define MS_NEMESIS 32   /* your nemesis */
#define MS_GUARDIAN 33  /* your leader's guards */
#define MS_SELL 34      /* demand payment, complain about shoplifters */
#define MS_ORACLE 35    /* do a consultation */
#define MS_PRIEST 36    /* ask for contribution; do cleansing */
#define MS_SPELL 37     /* spellcaster not matching any of the above */
#define MS_WERE 38      /* lycanthrope in human form */
#define MS_BOAST 39     /* giants */

/* resistances, and all intrinsics and extrinsics the monster can have */
#define MR_NONE			0x00000000L
#define MR_FIRE			0x00000001L			/* resists fire */
#define MR_COLD			0x00000002L			/* resists cold */
#define MR_SLEEP		0x00000004L			/* resists sleep */
#define MR_DISINT		0x00000008L			/* resists disintegration */
#define MR_ELEC			0x00000010L			/* resists electricity */
#define MR_POISON		0x00000020L			/* resists poison */
#define MR_ACID			0x00000040L			/* resists acid */
#define MR_STONE		0x00000080L			/* resists petrification */
#define MR_DEATH		0x00000100L			/* resists death */
#define MR_LYCANTHROPY	0x00000200L			/* resists lycanthropy */
#define MR_MAGICMISSILE 0x00000400L			/* resists magic missiles */
#define MR_MAGIC		0x00000800L			/* resists magic */
#define MR_CHARM		0x00001000L			/* resists charm */
#define MR_DRAIN		0x00002000L			/* resists life draining */
#define MR_FLASH		0x00004000L			/* resists flash */
#define MR_REFLECTING	0x00008000L			/* reflecting */
#define MR_INVISIBLE	0x00010000L			/* invisible */
#define MR_SEE_INVISIBLE 0x00020000L		/* see invisible */
#define MR_REGENERATION 0x00040000L		
#define MR_TELEPORT		0x00080000L		
#define MR_TELEPORT_CONTROL 0x00100000L		
#define MR_BLIND_TELEPATHY	0x00200000L		
#define MR_TELEPATHY	0x00400000L		
#define MR_FEAR			0x00800000L			/* immune to fear */
#define MR_SICK			0x01000000L			/* immune to sickness */


/* conveyances: resistances, teleport, teleport control, telepathy, as well as other properties from eating the corpse */
#define MC_NONE			0x00000000L
#define MC_FIRE			0x00000001L			/* confers fire resistance */
#define MC_COLD			0x00000002L   		/* confers cold resistance */
#define MC_SLEEP		0x00000004L  		/* confers sleep resistance */
#define MC_DISINT		0x00000008L 		/* confers disintegration resistance */
#define MC_ELEC			0x00000010L   		/* confers shock resistance */
#define MC_POISON		0x00000020L 		/* confers poison resistance */
#define MC_ACID			0x00000040L   		/* confers acid resistance */
#define MC_STONE		0x00000080L  		/* confers petrification resistance */
#define MC_DEATH		0x00000100L			/* confers death resistance */
#define MC_DRAIN		0x00000200L			/* confers life draining resistance */
#define MC_CHARM		0x00000400L			/* confers charm resistance */
#define MC_FEAR			0x00000800L			/* confers fear resistance */
#define MC_SICK			0x00001000L			/* confers sickness resistance */
/* free bit */
/* free bit */
/* free bit */
#define MC_STRENGTH		0x00010000L			/* confers strength */
#define MC_DEXTERITY	0x00020000L			/* confers dexterity */
#define MC_CONSTITUTION 0x00040000L			/* confers constitution */
#define MC_INTELLIGENCE 0x00080000L			/* confers intelligence */
#define MC_WISDOM		0x00100000L			/* confers wisdom */
#define MC_CHARISMA		0x00200000L			/* confers charisma */
#define MC_LEVEL_GAIN	0x00400000L			/* confers experience levels */
#define MC_TELEPORT			0x00800000L			/* confers teleportiis */
#define MC_TELEPORT_CONTROL 0x01000000L		/* confers teleport control */
#define MC_BLIND_TELEPATHY	0x02000000L		/* confers blind telepathy */
#define MC_TELEPATHY		0x04000000L			/* confers telepathy */
/* edible effects */
#define MC_POISONOUS_CORPSE	0x08000000L		/* the corpse is poisonous to eat */
#define MC_ACIDIC_CORPSE	0x10000000L		/* the corpse is acidic to eat */
#define MC_HALLUCINATING_CORPSE	0x20000000L		/* the corpse is hallucinating to eat */


#define M1_NONE			0x00000000
#define M1_FLY			0x00000001L         /* can fly or float */
#define M1_SWIM			0x00000002L        /* can traverse water */
#define M1_AMORPHOUS	0x00000004L   /* can flow under doors */
#define M1_WALLWALK		0x00000008L    /* can phase thru rock */
#define M1_CLING		0x00000010L       /* can cling to ceiling */
#define M1_TUNNEL		0x00000020L      /* can tunnel thru rock */
#define M1_NEEDPICK		0x00000040L    /* needs pick to tunnel */
#define M1_CONCEAL		0x00000080L     /* hides under objects */
#define M1_HIDE			0x00000100L        /* mimics, blends in with ceiling */
#define M1_AMPHIBIOUS	0x00000200L  /* can survive underwater */
#define M1_BREATHLESS	0x00000400L  /* doesn't need to breathe */
#define M1_NOTAKE		0x00000800L      /* cannot pick up objects */
#define M1_NOEYES		0x00001000L      /* no eyes to gaze into or blind */
#define M1_NOHANDS		0x00002000L     /* no hands to handle things */
#define M1_NOLIMBS		0x00006000L     /* no arms/legs to kick/wear on */
#define M1_NOHEAD		0x00008000L      /* no head to behead */
#define M1_MINDLESS		0x00010000L    /* has no mind--golem, zombie, mold */
#define M1_HUMANOID		0x00020000L    /* has humanoid head/arms/torso */
#define M1_ANIMAL		0x00040000L      /* has animal body */
#define M1_SLITHY		0x00080000L      /* has serpent body */
#define M1_UNSOLID		0x00100000L     /* has no solid or liquid body */
#define M1_THICK_HIDE	0x00200000L  /* has thick hide or scales */
#define M1_OVIPAROUS	0x00400000L   /* can lay eggs */
/* free bit */
/* free bit */
/* free bit */
/* free bit */
/* free bit */
/* free bit */
#define M1_CARNIVORE	0x20000000L   /* eats corpses */
#define M1_HERBIVORE	0x40000000L   /* eats fruits */
#define M1_OMNIVORE		0x60000000L    /* eats both */
#ifdef NHSTDC
#define M1_METALLIVORE	0x80000000UL /* eats metal */
#else
#define M1_METALLIVORE 0x80000000L /* eats metal */
#endif

#define M2_NONE			0x00000000
#define M2_NOPOLY		0x00000001L       /* players mayn't poly into one */
#define M2_UNDEAD		0x00000002L       /* is walking dead */
#define M2_WERE			0x00000004L         /* is a lycanthrope */
#define M2_HUMAN		0x00000008L        /* is a human */
#define M2_ELF			0x00000010L          /* is an elf */
#define M2_DWARF		0x00000020L        /* is a dwarf */
#define M2_GNOLL		0x00000040L        /* is a gnoll */
#define M2_ORC			0x00000080L          /* is an orc */
#define M2_DEMON		0x00000100L        /* is a demon */
#define M2_MODRON		0x00000200L		/* is a modron */
#define M2_GNOME		0x00000400L        /* is a gnome */
#define M2_MERC			0x00000800L         /* is a guard or soldier */
#define M2_LORD			0x00001000L         /* is a lord to its kind */
#define M2_PRINCE		0x00002000L       /* is an overlord to its kind */
#define M2_MINION		0x00004000L       /* is a minion of a deity */
#define M2_GIANT		0x00008000L        /* is a giant */
#define M2_SHAPESHIFTER 0x00010000L /* is a shapeshifting species */
#define M2_MALE			0x00020000L         /* always male */
#define M2_FEMALE		0x00040000L       /* always female */
#define M2_NEUTER		0x00080000L       /* neither male nor female */
#define M2_PNAME		0x00100000L        /* monster name is a proper name */
#define M2_HOSTILE		0x00200000L      /* always starts hostile */
#define M2_PEACEFUL		0x00400000L     /* always starts peaceful */
#define M2_DOMESTIC		0x00800000L     /* can be tamed by feeding */
#define M2_WANDER		0x01000000L       /* wanders randomly */
#define M2_STALK		0x02000000L        /* follows you to other levels */
#define M2_NASTY		0x04000000L        /* extra-nasty monster (more xp) */
#define M2_ROCKTHROW	0x08000000L    /* throws boulders */
#define M2_GREEDY		0x10000000L       /* likes gold */
#define M2_JEWELS		0x20000000L       /* likes gems */
#define M2_COLLECT		0x40000000L      /* picks up weapons and food */
#ifdef NHSTDC
#define M2_MAGIC		0x80000000UL /* picks up magic items */
#else
#define M2_MAGIC 0x80000000L /* picks up magic items */
#endif

#define M3_NONE										0x00000000
#define M3_WANTSAMUL								0x00000001 /* would like to steal the amulet */
#define M3_WANTSBELL								0x00000002 /* wants the bell */
#define M3_WANTSBOOK								0x00000004 /* wants the book */
#define M3_WANTSCAND								0x00000008 /* wants the candelabrum */
#define M3_WANTSARTI								0x00000010 /* wants the quest artifact */
#define M3_WANTSALL									0x0000001f  /* wants any major artifact */
#define M3_WAITFORU									0x00000040  /* waits to see you or get attacked */
#define M3_CLOSE									0x00000080     /* lets you close unless attacked */

#define M3_COVETOUS									0x0000000f /* wants something (earlier the same as WANTSALL) */
#define M3_WAITMASK									0x000000c0 /* waiting... */

/* Infravision is currently implemented for players only */
#define M3_INFRAVISION								0x00000100  /* has infravision */
#define M3_INFRAVISIBLE								0x00000200 /* visible by infravision */

#define M3_DISPLACES								0x00000400			/* moves monsters out of its way */
#define M3_NONCORPOREAL								0x00000800      /* is noncorporeal, different from unsolid in the sense that
																	an unsolid monster can be disintegrated, but noncorporeal not */
/* free bit */
#define M3_NONLIVING								0x00002000			/* is nonliving, such as a golem or other construct or a vortex (unaffected by death attacks)*/
#define M3_MULTIWEAPON								0x00004000		/* fights with more than one weapon (i.e., the wielded weapon), if available*/
#define M3_CONSTRICTOR								0x00008000       /* grabbing / sticking itself causes constriction */

#define M3_WIZARD									0x00010000			/* monster can use wizard items */
#define M3_PRIEST									0x00020000			/* monster can use priest items */
#define M3_KNIGHT									0x00040000			/* monster can use knight items */
#define M3_HEALER									0x00080000			/* monster can use healer items */

/* free bit */
/* free bit */
#define M3_REGENERATES_LOST_BODY_PARTS				0x00400000		/* does not lose max hitpoint because of sharpness or bisection */
#define M3_REVIVES_FROM_DEAD						0x00800000		/* is resurrects itself after a while */

#define M3_SPEAKING									0x01000000		/* can be spoken to using the chat command */
#define M3_CHAT_CAN_JOIN_PARTY						0x02000000		/* can join the player's party */

#define M3_WEBMAKER									0x04000000		/* is a webmaker */
#define M3_KNOWS_TRAPS								0x08000000		/* knows traps on the level */
#define M3_KNOWS_PITS_AND_HOLES						0x10000000		/* knows pits and holes on the level */

#define M3_RANDOM_CORPSE							0x20000000		/* leaves a corpse at random intervals */
#define M3_CORPSE_CRUMBLES_TO_DUST					0x40000000		/* corpse crumbles to dust, also leaving no corpse */
#define M3_HUG_HITS_IF_TWO_FIRST_ATTACKS_SUCCEEDED	0x80000000		/* corpse crumbles to dust, also leaving no corpse */


#define M4_NONE						0x00000000
#define M4_BRAVE					0x00000001		/* disregards enemy strength */
#define M4_FEARLESS					0x00000002		/* disregards own health */
#define M4_BLOODLUST				0x00000004		/* attacks peaceful */

#define M4_SPLITS_UPON_HIT			0x00000008		/* brown and black puddings */
#define M4_VEGETARIAN_FOOD			0x00000010		/* puddings and other monsters for a vegetarian */
#define M4_VEGAN_FOOD				0x00000020		/* monsters for a vegan */
#define M4_NONROTTING_CORPSE		0x00000040		/* corpse does not rot */

#define M4_ALTERNATIVE_PASSIVE_DEFENSE_TEXT	0x00000080		/* e.g. engulfed in X's flames rather than feel very hot */

#define M4_PITWALK					0x00000100		/* can walk in and out of pits without trouble */


#define MZ_TINY 0          /* < 2' */
#define MZ_SMALL 1         /* 2-4' */
#define MZ_MEDIUM 2        /* 4-7' */
#define MZ_HUMAN MZ_MEDIUM /* human-sized */
#define MZ_LARGE 3         /* 7-12' */
#define MZ_HUGE 4          /* 12-25' */
#define MZ_GIGANTIC 7      /* off the scale */

/* Monster races -- must stay within ROLE_RACEMASK */
/* Eventually this may become its own field */
#define MH_HUMAN M2_HUMAN
#define MH_ELF M2_ELF
#define MH_DWARF M2_DWARF
#define MH_GNOLL M2_GNOLL
#define MH_ORC M2_ORC
#define MH_GNOME M2_GNOME

/* for mons[].geno (constant during game) */
#define G_UNIQ 0x1000     /* generated only once */
#define G_NOHELL 0x0800   /* not generated in "hell" */
#define G_HELL 0x0400     /* generated only in "hell" */
#define G_NOGEN 0x0200    /* generated only specially */
#define G_SGROUP 0x0080   /* appear in small groups normally */
#define G_LGROUP 0x0040   /* appear in large groups normally */
#define G_GENO 0x0020     /* can be genocided */
#define G_NOCORPSE 0x0010 /* no corpse left ever */
#define G_FREQ 0x0007     /* creation frequency mask */

/* for mvitals[].mvflags (variant during game), along with G_NOCORPSE */
#define G_KNOWN 0x0004 /* have been encountered */
#define G_GONE (G_GENOD | G_EXTINCT)
#define G_GENOD 0x0002 /* have been genocided */
#define G_EXTINCT                       \
    0x0001 /* have been extinguished as \
              population control */
#define MV_KNOWS_EGG                        \
    0x0008 /* player recognizes egg of this \
              monster type */

#endif /* MONFLAG_H */
