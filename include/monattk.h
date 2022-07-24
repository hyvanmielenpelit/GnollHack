/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-04-16 */

/* GnollHack 4.0    monattk.h    $NHDT-Date: 1432512775 2015/05/25 00:12:55 $  $NHDT-Branch: master $:$NHDT-Revision: 1.11 $ */
/* GnollHack may be freely redistributed.  See license for details. */
/* Copyright 1988, M. Stephenson */

#ifndef MONATTK_H
#define MONATTK_H

/*    Add new attack types below - ordering affects experience (exper.c).
 *    Attacks > AT_BUTT are worth extra experience.
 */
#define AT_ANY (-1) /* fake attack; dmgtype_fromattack wildcard */
#define AT_NONE 0   /* no attack, passive moved to 252 */
#define AT_CLAW 1   /* claw (punch, hit, etc.) */
#define AT_BITE 2   /* bite */
#define AT_KICK 3   /* kick */
#define AT_RAMS 4   /* trample (e.g., juggernaut) */
#define AT_TAIL 5   /* lashes tail (e.g., Tarrasque) */
#define AT_BUTT 6   /* head butt (ex. a unicorn) */
#define AT_TUCH 7   /* touches */
#define AT_STNG 8   /* sting */
#define AT_HUGS 9   /* crushing bearhug */
#define AT_SPIT 10  /* spits substance - ranged */
#define AT_ENGL 11  /* engulf (swallow or by a cloud) */
#define AT_BREA 12  /* breath - ranged */
#define AT_EXPL 13  /* explodes - proximity */
#define AT_BOOM 14  /* explodes when killed */
#define AT_GAZE 15  /* gaze - ranged */
#define AT_TENT 16  /* tentacles */
#define AT_EYES 17  /* gazer eye stalk - ranged */
#define AT_HORN 18  /* horn */
#define AT_TUSK 19  /* tusk */

#define AT_PASV 252 /* passive defense */
#define AT_SMMN 253 /* uses demon special abilities */
#define AT_WEAP 254 /* uses weapon */
#define AT_MAGC 255 /* uses magic spell(s) */

/*    Add new damage types below.
 *
 *    Note that 1-10 correspond to the types of attack used in buzz().
 *    Please don't disturb the order unless you rewrite the buzz() code.
 */
#define AD_NONE (-1)   /* None */
#define AD_ANY (-1) /* fake damage; attacktype_fordmg wildcard */
#define AD_PHYS 0   /* ordinary physical */
#define AD_MAGM 1   /* magic missiles FIRST RAY */
#define AD_FIRE 2   /* fire damage */
#define AD_COLD 3   /* frost damage */
#define AD_SLEE 4   /* sleep ray */
#define AD_DISN 5   /* disintegration */
#define AD_ELEC 6   /* shock damage */
#define AD_DRST 7   /* drains str (poison) */
#define AD_ACID 8   /* acid damage */
#define AD_DRAY 9   /* death ray */
#define AD_STON 10  /* petrifies (Medusa, cockatrice, gorgon) LAST RAY */
#define AD_BLND 11  /* blinds (yellow light) */
#define AD_STUN 12  /* stuns */
#define AD_SLOW 13  /* slows */
#define AD_PLYS 14  /* paralyses */
#define AD_DRLI 15  /* drains life levels (Vampire) */
#define AD_DREN 16  /* drains magic energy */
#define AD_LEGS 17  /* damages legs (xan) */
#define AD_SPEC 18  /* special */
#define AD_STCK 19  /* sticks to you (mimic) */
#define AD_SGLD 20  /* steals gold (leppie) */
#define AD_SITM 21  /* steals item (nymphs) */
#define AD_SEDU 22  /* seduces & steals multiple items */
#define AD_TLPT 23  /* teleports you (Quantum Mech.) */
#define AD_RUST 24  /* rusts armour (Rust Monster)*/
#define AD_CONF 25  /* confuses (Umbral Hulk) */
#define AD_DGST 26  /* digests opponent (trapper, etc.) */
#define AD_HEAL 27  /* heals opponent's wounds (nurse) */
#define AD_WRAP 28  /* special "stick" for eels */
#define AD_WERE 29  /* confers lycanthropy */
#define AD_DRDX 30  /* drains dexterity (quasit) */
#define AD_DRCO 31  /* drains constitution */
#define AD_DRIN 32  /* drains intelligence (tentacled one) */
#define AD_DISE 33  /* confers terminal disease */
#define AD_DCAY 34  /* decays organics (brown Pudding) */
#define AD_SSEX 35  /* Succubus seduction (extended) */
#define AD_HALU 36  /* causes hallucination */
#define AD_DETH 37  /* for Death only */
#define AD_PEST 38  /* for Pestilence only */
#define AD_FAMN 39  /* for Famine only */
#define AD_SLIM 40  /* turns you into green slime */
#define AD_ENCH 41  /* remove enchantment (disenchanter) */
#define AD_CORR 42  /* corrode armor (black pudding) */
#define AD_CNCL 43  /* cancel target */
#define AD_PSIO 44  /* psionic damage */
#define AD_SHOC 45  /* system shock */
#define AD_ADJU 46  /* hit point adjustment, e.g. due to size shrinkage */
#define AD_SHRP 47  /* sharp as sword of sharpness */
#define AD_ROTS 48  /* confers mummy rot */

#define AD_RBGD 237 /* random breath weapon, gold dragon */
#define AD_RBPD 238 /* random breath weapon, Bahamut */
#define AD_GDRA 239 /* summons an ancient gold dragon, Bahamut */
#define AD_CLRC 240 /* random clerical spell */
#define AD_SPEL 241 /* random magic spell */
#define AD_RBRE 242 /* random breath weapon */
#define AD_DMNS 243 /* Summons demons */
#define AD_LYCA 244 /* Lycanthrope summon */
#define AD_GNOL 245 /* summons gnolls, Yeenaghu */
#define AD_GHUL 246 /* summons ghouls, Yeenaghu */
#define AD_UNDO 247 /* summons undead, Orcus */
#define AD_MINO 248 /* summons a minotaur, Baphomet and Geryon */
#define AD_REY1 249 /* random eyestalk 1 */
#define AD_REY2 250 /* random eyestalk 2 */
#define AD_BISN 251 /* summons bison, Yacc */

#define AD_SAMU 252 /* hits, may steal Amulet (Wizard) */
#define AD_CURS 253 /* random curse (ex. gremlin) */

/*
 *  Monster to monster attacks.  When a monster attacks another (mattackm),
 *  any or all of the following can be returned.  See mattackm() for more
 *  details.
 */
#define MM_MISS 0x0     /* aggressor missed */
#define MM_HIT 0x1      /* aggressor hit defender */
#define MM_DEF_DIED 0x2 /* defender died */
#define MM_AGR_DIED 0x4 /* aggressor died */

#endif /* MONATTK_H */
