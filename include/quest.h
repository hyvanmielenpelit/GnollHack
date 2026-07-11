/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    quest.h    $NHDT-Date: 1432512779 2015/05/25 00:12:59 $  $NHDT-Branch: master $:$NHDT-Revision: 1.10 $ */
/* Copyright (c) Mike Stephenson 1991.                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef QUEST_H
#define QUEST_H

struct q_score {              /* Quest "scorecard" */
    uint64_t bitflags;


    uchar not_ready;   /* rejected due to alignment, etc. */
    uchar made_goal;      /* # of times on goal level */

    /* used by questpgr code when messages want to use pronouns
       (set up at game start instead of waiting until monster creation;
       1 bit each would suffice--nobody involved is actually neuter) */
    uchar ldrgend; /* leader's gender: 0=male, 1=female, 2=neuter */
    uchar nemgend; /* nemesis's gender */
    uchar godgend; /* deity's gender */

    unsigned leader_m_id;
};


#define QSCORE_BITFLAG_NONE             0x00000000UL
#define QSCORE_BITFLAG_FIRST_START      0x00000001UL
#define QSCORE_BITFLAG_MET_LEADER       0x00000002UL
#define QSCORE_BITFLAG_PISSED_OFF       0x00000004UL
#define QSCORE_BITFLAG_GOT_QUEST        0x00000008UL
#define QSCORE_BITFLAG_FIRST_LOCATE     0x00000010UL
#define QSCORE_BITFLAG_MET_INTERMED     0x00000020UL
#define QSCORE_BITFLAG_GOT_FINAL        0x00000040UL
#define QSCORE_BITFLAG_MET_NEMESIS      0x00000080UL
#define QSCORE_BITFLAG_KILLED_NEMESIS   0x00000100UL
#define QSCORE_BITFLAG_IN_BATTLE        0x00000200UL
#define QSCORE_BITFLAG_CHEATER          0x00000400UL
#define QSCORE_BITFLAG_TOUCHED_ARTIFACT 0x00000800UL
#define QSCORE_BITFLAG_OFFERED_ARTIFACT 0x00001000UL
#define QSCORE_BITFLAG_GOT_THANKS       0x00002000UL
#define QSCORE_BITFLAG_LEADER_IS_DEAD   0x00004000UL

#define is_qstatus_first_start()      get_flag(quest_status.bitflags, QSCORE_BITFLAG_FIRST_START)
#define set_qstatus_first_start(v)    set_flag(quest_status.bitflags, QSCORE_BITFLAG_FIRST_START, (v))
#define toggle_qstatus_first_start()  toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_FIRST_START)

#define is_qstatus_met_leader()       get_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_LEADER)
#define set_qstatus_met_leader(v)     set_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_LEADER, (v))
#define toggle_qstatus_met_leader()   toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_LEADER)

#define is_qstatus_pissed_off()       get_flag(quest_status.bitflags, QSCORE_BITFLAG_PISSED_OFF)
#define set_qstatus_pissed_off(v)     set_flag(quest_status.bitflags, QSCORE_BITFLAG_PISSED_OFF, (v))
#define toggle_qstatus_pissed_off()   toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_PISSED_OFF)

#define is_qstatus_got_quest()        get_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_QUEST)
#define set_qstatus_got_quest(v)      set_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_QUEST, (v))
#define toggle_qstatus_got_quest()    toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_QUEST)

#define is_qstatus_first_locate()     get_flag(quest_status.bitflags, QSCORE_BITFLAG_FIRST_LOCATE)
#define set_qstatus_first_locate(v)   set_flag(quest_status.bitflags, QSCORE_BITFLAG_FIRST_LOCATE, (v))
#define toggle_qstatus_first_locate() toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_FIRST_LOCATE)

#define is_qstatus_met_intermed()     get_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_INTERMED)
#define set_qstatus_met_intermed(v)   set_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_INTERMED, (v))
#define toggle_qstatus_met_intermed() toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_INTERMED)

#define is_qstatus_got_final()        get_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_FINAL)
#define set_qstatus_got_final(v)      set_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_FINAL, (v))
#define toggle_qstatus_got_final()    toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_FINAL)

#define is_qstatus_met_nemesis()      get_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_NEMESIS)
#define set_qstatus_met_nemesis(v)    set_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_NEMESIS, (v))
#define toggle_qstatus_met_nemesis()  toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_MET_NEMESIS)

#define is_qstatus_killed_nemesis()      get_flag(quest_status.bitflags, QSCORE_BITFLAG_KILLED_NEMESIS)
#define set_qstatus_killed_nemesis(v)    set_flag(quest_status.bitflags, QSCORE_BITFLAG_KILLED_NEMESIS, (v))
#define toggle_qstatus_killed_nemesis()  toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_KILLED_NEMESIS)

#define is_qstatus_in_battle()        get_flag(quest_status.bitflags, QSCORE_BITFLAG_IN_BATTLE)
#define set_qstatus_in_battle(v)      set_flag(quest_status.bitflags, QSCORE_BITFLAG_IN_BATTLE, (v))
#define toggle_qstatus_in_battle()    toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_IN_BATTLE)

#define is_qstatus_cheater()          get_flag(quest_status.bitflags, QSCORE_BITFLAG_CHEATER)
#define set_qstatus_cheater(v)        set_flag(quest_status.bitflags, QSCORE_BITFLAG_CHEATER, (v))
#define toggle_qstatus_cheater()      toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_CHEATER)

#define is_qstatus_touched_artifact()      get_flag(quest_status.bitflags, QSCORE_BITFLAG_TOUCHED_ARTIFACT)
#define set_qstatus_touched_artifact(v)    set_flag(quest_status.bitflags, QSCORE_BITFLAG_TOUCHED_ARTIFACT, (v))
#define toggle_qstatus_touched_artifact()  toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_TOUCHED_ARTIFACT)

#define is_qstatus_offered_artifact()      get_flag(quest_status.bitflags, QSCORE_BITFLAG_OFFERED_ARTIFACT)
#define set_qstatus_offered_artifact(v)    set_flag(quest_status.bitflags, QSCORE_BITFLAG_OFFERED_ARTIFACT, (v))
#define toggle_qstatus_offered_artifact()  toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_OFFERED_ARTIFACT)

#define is_qstatus_got_thanks()       get_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_THANKS)
#define set_qstatus_got_thanks(v)     set_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_THANKS, (v))
#define toggle_qstatus_got_thanks()   toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_GOT_THANKS)

#define is_qstatus_leader_is_dead()      get_flag(quest_status.bitflags, QSCORE_BITFLAG_LEADER_IS_DEAD)
#define set_qstatus_leader_is_dead(v)    set_flag(quest_status.bitflags, QSCORE_BITFLAG_LEADER_IS_DEAD, (v))
#define toggle_qstatus_leader_is_dead()  toggle_flag(quest_status.bitflags, QSCORE_BITFLAG_LEADER_IS_DEAD)

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
