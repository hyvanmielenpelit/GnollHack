/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    quest.h    $NHDT-Date: 1432512779 2015/05/25 00:12:59 $  $NHDT-Branch: master $:$NHDT-Revision: 1.10 $ */
/* Copyright (c) Mike Stephenson 1991.                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef QUEST_H
#define QUEST_H

struct q_score {              /* Quest "scorecard" */
    /* Bitfield flags converted to portable explicit bitmask flags */
    uint64_t bitflags;
#define QSCORE_BITFLAGS_NONE             0x00000000UL
#define QSCORE_BITFLAGS_FIRST_START      0x00000001UL
#define QSCORE_BITFLAGS_MET_LEADER       0x00000002UL
#define QSCORE_BITFLAGS_PISSED_OFF       0x00000004UL
#define QSCORE_BITFLAGS_GOT_QUEST        0x00000008UL
#define QSCORE_BITFLAGS_FIRST_LOCATE     0x00000010UL
#define QSCORE_BITFLAGS_MET_INTERMED     0x00000020UL
#define QSCORE_BITFLAGS_GOT_FINAL        0x00000040UL
#define QSCORE_BITFLAGS_MET_NEMESIS      0x00000080UL
#define QSCORE_BITFLAGS_KILLED_NEMESIS   0x00000100UL
#define QSCORE_BITFLAGS_IN_BATTLE        0x00000200UL
#define QSCORE_BITFLAGS_CHEATER          0x00000400UL
#define QSCORE_BITFLAGS_TOUCHED_ARTIFACT 0x00000800UL
#define QSCORE_BITFLAGS_OFFERED_ARTIFACT 0x00001000UL
#define QSCORE_BITFLAGS_GOT_THANKS       0x00002000UL
#define QSCORE_BITFLAGS_LEADER_IS_DEAD   0x00004000UL

    uchar not_ready;
    uchar made_goal;
    uchar ldrgend;
    uchar nemgend;
    uchar godgend;

    unsigned leader_m_id;
};

#define MAX_QUEST_TRIES 7  /* exceed this and you "fail" */
#define MIN_QUEST_ALIGN 20 /* at least this align.record to start */
/* note: align 20 matches "pious" as reported by enlightenment (cmd.c) */
#define MIN_QUEST_LEVEL 14 /* at least this u.ulevel to start */
/* note: exp.lev. 14 is threshold level for 5th rank (class title, role.c) */

#define ModeAllowsPostToForum (!wizard && !CasualMode && !discover)
//#if !defined(DEBUG)
#define IfModeAllowsPostToForum if (ModeAllowsPostToForum)
//#else
//#define IfModeAllowsPostToForum 
//#endif
#endif /* QUEST_H */
