/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    quest.c    $NHDT-Date: 1505170343 2017/09/11 22:52:23 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.21 $ */
/*      Copyright 1991, M. Stephenson             */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

/*  quest dungeon branch routines. */

#include "quest.h"
#include "qtext.h"

#define Not_firsttime (on_level(&u.uz0, &u.uz))
#define Qstat(x) (quest_status.x)

static void on_start(void);
static void on_locate(void);
static void on_goal(void);
static void expulsion(boolean);
static boolean chat_with_leader(struct monst*, boolean);
static boolean chat_with_nemesis(struct monst*, boolean);
static boolean chat_with_guardian(struct monst*, boolean);
static boolean prisoner_speaks(struct monst *);

static void
on_start(void)
{
    if (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_FIRST_START)) {
        qt_pager_ex((struct monst*)0, QT_FIRSTTIME, ATR_NONE, CLR_MSG_HINT, FALSE);
        stop_all_immediate_sounds();
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_FIRST_START, TRUE);
    } else if ((u.uz0.dnum != u.uz.dnum) || (u.uz0.dlevel < u.uz.dlevel)) {
        if (Qstat(not_ready) <= 2)
        {
            qt_pager_ex((struct monst*)0, QT_NEXTTIME, ATR_NONE, CLR_MSG_HINT, FALSE);
        }
        else
        {
            qt_pager_ex((struct monst*)0, QT_OTHERTIME, ATR_NONE, CLR_MSG_HINT, FALSE);
        }
    }
}

static void
on_locate(void)
{
    /* the locate messages are phrased in a manner such that they only
       make sense when arriving on the level from above */
    boolean from_above = (u.uz0.dlevel < u.uz.dlevel);

    if (get_flag(quest_status.bitflags, QSCORE_BITFLAGS_KILLED_NEMESIS)) {
        return;
    } else if (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_FIRST_LOCATE)) {
        if (from_above)
        {
            qt_pager_ex((struct monst*)0, QT_FIRSTLOCATE, ATR_NONE, CLR_MSG_HINT, FALSE);
        }
        /* if we've arrived from below this will be a lie, but there won't
           be any point in delivering the message upon a return visit from
           above later since the level has now been seen */
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_FIRST_LOCATE, TRUE);
    } else {
        if (from_above)
        {
            qt_pager_ex((struct monst*)0, QT_NEXTLOCATE, ATR_NONE, CLR_MSG_HINT, FALSE);
        }
    }
}

static void
on_goal(void)
{
    if (get_flag(quest_status.bitflags, QSCORE_BITFLAGS_KILLED_NEMESIS)) {
        return;
    } else if (!Qstat(made_goal)) {
        qt_pager_ex((struct monst*)0, QT_FIRSTGOAL, ATR_NONE, CLR_MSG_HINT, FALSE);
        Qstat(made_goal) = 1;
    } else {
        /*
         * Some QT_NEXTGOAL messages reference the quest artifact;
         * find out if it is still present.  If not, request an
         * alternate message (qt_pager() will revert to delivery
         * of QT_NEXTGOAL if current role doesn't have QT_ALTGOAL).
         * Note: if hero is already carrying it, it is treated as
         * being absent from the level for quest message purposes.
         */
        unsigned whichobjchains = ((1 << OBJ_FLOOR)
                                   | (1 << OBJ_MINVENT)
                                   | (1 << OBJ_BURIED));
        struct obj *qarti = find_quest_artifact(whichobjchains);

        qt_pager((struct monst*)0, qarti ? QT_NEXTGOAL : QT_ALTGOAL);
        if (Qstat(made_goal) < 7)
            Qstat(made_goal)++;
    }
}

void
onquest(void)
{
    if (get_flag(u.uevent.bitflags, UEVENT_BITFLAGS_QCOMPLETED) || Not_firsttime)
        return;
    if (!Is_special(&u.uz))
        return;

    if (Is_qstart(&u.uz))
        on_start();
    else if (Is_qlocate(&u.uz))
        on_locate();
    else if (Is_nemesis(&u.uz))
        on_goal();
    return;
}

void
nemdead(void)
{
    if (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_KILLED_NEMESIS)) {
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_KILLED_NEMESIS, TRUE);
        issue_achievement(GUI_ACHIEVEMENT_DEFEATED_QUEST_NEMESIS);
        qt_pager_ex((struct monst*)0, QT_KILLEDNEM, ATR_NONE, CLR_MSG_HINT, FALSE);
    }
}

void
artitouch(struct obj *obj)
{
    if (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_TOUCHED_ARTIFACT)) {
        /* in case we haven't seen the item yet (ie, currently blinded),
           this quest message describes it by name so mark it as seen */
        set_obj_dknown(obj, 1);
        /* Quest artifacts become identified by the artifact message */
        set_obj_aknown(obj, 1);
        set_obj_nknown(obj, 1);
        /* only give this message once */
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_TOUCHED_ARTIFACT, TRUE);
        qt_pager_ex((struct monst*)0, QT_GOTIT, ATR_NONE, CLR_MSG_HINT, FALSE);
        exercise(A_WIS, TRUE);
    }
}

/* external hook for do.c (level change check) */
boolean
ok_to_quest(void)
{
    return (boolean) ((get_flag(quest_status.bitflags, QSCORE_BITFLAGS_GOT_QUEST) || get_flag(quest_status.bitflags, QSCORE_BITFLAGS_GOT_THANKS) || get_flag(quest_status.bitflags, QSCORE_BITFLAGS_LEADER_IS_DEAD))
                      && is_pure(FALSE) > 0);
}

boolean
not_capable(void)
{
    return (boolean) (u.ulevel < MIN_QUEST_LEVEL);
}

int
is_pure(boolean talk)
{
    int purity;
    aligntyp original_alignment = u.ualignbase[A_ORIGINAL];

    if (wizard && talk) {
        if (u.ualign.type != original_alignment) {
            You("are currently %s instead of %s.", align_str(u.ualign.type),
                align_str(original_alignment));
        } else if (u.ualignbase[A_CURRENT] != original_alignment) {
            You("have converted.");
        }/* else if (u.ualign.record < MIN_QUEST_ALIGN) {
            You("are currently %d and require %d.", u.ualign.record,
                MIN_QUEST_ALIGN);
            if (yn_function("adjust?", (char *) 0, 'y') == 'y')
                u.ualign.record = MIN_QUEST_ALIGN;
        }*/
    }
    purity = (/*u.ualign.record >= MIN_QUEST_ALIGN
              &&*/ u.ualign.type == original_alignment
              && u.ualignbase[A_CURRENT] == original_alignment)
                 ? 1
                 : (u.ualignbase[A_CURRENT] != original_alignment) ? -1 : 0;
    return purity;
}

/*
 * Expel the player to the stairs on the parent of the quest dungeon.
 *
 * This assumes that the hero is currently _in_ the quest dungeon and that
 * there is a single branch to and from it.
 */
static void
expulsion(boolean seal)
{
    branch *br;
    d_level *dest;
    struct trap *t;
    int64_t portal_flag;
    debugprint("expulsion: seal=%d, qexpelled=%d", (int)seal, (int)get_flag(u.uevent.bitflags, UEVENT_BITFLAGS_QEXPELLED));
    issue_breadcrumb3("Expulsion.", (int)seal, (int)get_flag(u.uevent.bitflags, UEVENT_BITFLAGS_QEXPELLED));

    br = dungeon_branch("The Quest");
    dest = (br->end1.dnum == u.uz.dnum) ? &br->end2 : &br->end1;
    portal_flag = get_flag(u.uevent.bitflags, UEVENT_BITFLAGS_QEXPELLED) ? 0 /* returned via artifact? */
                                     : !seal ? 1 : -1;
    schedule_goto(dest, FALSE, FALSE, TRUE, FALSE, portal_flag, (char *) 0, (char *) 0);
    if (seal) { /* remove the portal to the quest - sealing it off */
        int reexpelled = get_flag(u.uevent.bitflags, UEVENT_BITFLAGS_QEXPELLED);

        set_flag(u.uevent.bitflags, UEVENT_BITFLAGS_QEXPELLED, 1);
        remdun_mapseen(quest_dnum);
        /* Delete the near portal now; the far (main dungeon side)
           portal will be deleted as part of arrival on that level.
           If monster movement is in progress, any who haven't moved
           yet will now miss out on a chance to wander through it... */
        for (t = ftrap; t; t = t->ntrap)
            if (t->ttyp == MAGIC_PORTAL)
                break;
        if (t)
        {
            deltrap(t); /* (display might be briefly out of sync) */
            issue_breadcrumb("Quest near portal deleted.");
        }
        else if (!reexpelled)
            impossible("quest portal already gone?");
    }
}

/* Either you've returned to quest leader while carrying the quest
   artifact or you've just thrown it to/at him or her.  If quest
   completion text hasn't been given yet, give it now.  Otherwise
   give another message about the character keeping the artifact
   and using the magic portal to return to the dungeon. */
void
finish_quest(struct obj *obj)
{
    struct obj *otmp;

    if (get_flag(u.uhave.bitflags, UHAVE_BITFLAGS_AMULET)) { /* unlikely but not impossible */
        qt_pager_ex((struct monst*)0, QT_HASAMULET, ATR_NONE, CLR_MSG_HINT, FALSE);
        /* leader IDs the real amulet but ignores any fakes */
        if ((otmp = carrying(AMULET_OF_YENDOR)) != 0)
            fully_identify_obj(otmp);
    } else {
        qt_pager_ex((struct monst*)0, !get_flag(quest_status.bitflags, QSCORE_BITFLAGS_GOT_THANKS) ? QT_OFFEREDIT : QT_OFFEREDIT2, ATR_NONE, CLR_MSG_HINT, FALSE);
        /* should have obtained bell during quest;
           if not, suggest returning for it now */
        if ((otmp = carrying(BELL_OF_OPENING)) == 0)
        {
            com_pager((struct monst*)0, 5);
            context.quest_flags |= QUEST_FLAGS_HEARD_OF_BELL | QUEST_FLAGS_HEARD_OF_BELL_OWNER | QUEST_FLAGS_HEARD_OF_BELL_OWNER_IS_NEMESIS;
        }
    }
    set_flag(quest_status.bitflags, QSCORE_BITFLAGS_GOT_THANKS, TRUE);

    if (obj) {
        set_flag(u.uevent.bitflags, UEVENT_BITFLAGS_QCOMPLETED, 1); /* you did it! */
        /* behave as if leader imparts sufficient info about the
           quest artifact */
        fully_identify_obj(obj);
        update_inventory();
        livelog_printf(LL_ACHIEVE, "completed %s quest without incident", uhis());
        issue_achievement(GUI_ACHIEVEMENT_COMPLETED_THE_QUEST);
        update_game_music();
    }
}

static boolean
chat_with_leader(struct monst *mtmp, boolean dopopup)
{
    boolean res = FALSE;
    /*  Rule 0: Cheater checks. */
    if (get_flag(u.uhave.bitflags, UHAVE_BITFLAGS_QUESTART) && !get_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_NEMESIS))
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_CHEATER, TRUE);

    /*  It is possible for you to get the amulet without completing
     *  the quest.  If so, try to induce the player to quest.
     */
    if (get_flag(quest_status.bitflags, QSCORE_BITFLAGS_GOT_THANKS)) 
    {
        res = TRUE;
        /* Rule 1: You've gone back with/without the amulet. */
        if (get_flag(u.uhave.bitflags, UHAVE_BITFLAGS_AMULET))
            finish_quest((struct obj *) 0);

        /* Rule 2: You've gone back before going for the amulet. */
        else
        {
            qt_pager_ex(mtmp, QT_POSTHANKS, ATR_NONE, NO_COLOR, dopopup);
        }

    /* Rule 3: You've got the artifact and are back to return it. */
    }
    else if (get_flag(u.uhave.bitflags, UHAVE_BITFLAGS_QUESTART))
    {
        res = TRUE;
        struct obj *otmp;

        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (is_quest_artifact(otmp))
                break;

        finish_quest(otmp);

    /* Rule 4: You haven't got the artifact yet. */
    } 
    else if (get_flag(quest_status.bitflags, QSCORE_BITFLAGS_GOT_QUEST))
    {
        res = TRUE;
        qt_pager_ex(mtmp, rn1(10, QT_ENCOURAGE), ATR_NONE, NO_COLOR, dopopup);

    /* Rule 5: You aren't yet acceptable - or are you? */
    } 
    else
    {
        res = TRUE;
        if (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_LEADER)) 
        {
            qt_pager_ex(mtmp, QT_FIRSTLEADER, ATR_NONE, CLR_MSG_HINT, dopopup);
            set_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_LEADER, TRUE);
            Qstat(not_ready) = 0;
        } 
        else
        {
            qt_pager_ex(mtmp, QT_NEXTLEADER, ATR_NONE, NO_COLOR, dopopup);
        }

        /* the quest leader might have passed through the portal into
           the regular dungeon; none of the remaining make sense there */
        if (!on_level(&u.uz, &qstart_level))
            return res;

        if (not_capable())
        {
            qt_pager_ex(mtmp, QT_BADLEVEL, ATR_NONE, CLR_MSG_HINT, dopopup);
            exercise(A_WIS, TRUE);
            res = FALSE; // For safety
            debugprint_pos();
            expulsion(FALSE);
        }
        else if (is_pure(TRUE) < 0) 
        {
            com_pager_ex((struct monst*)0, QT_BANISHED, ATR_NONE, CLR_MSG_NEGATIVE, dopopup);
            res = FALSE; // For safety
            debugprint_pos();
            expulsion(TRUE);
        }
        else if (is_pure(TRUE) == 0)
        {
            qt_pager_ex(mtmp, QT_BADALIGN, ATR_NONE, CLR_MSG_WARNING, dopopup);
            if (Qstat(not_ready) == MAX_QUEST_TRIES)
            {
                qt_pager_ex(mtmp, QT_LASTLEADER, ATR_NONE, CLR_MSG_NEGATIVE, dopopup);
                res = FALSE; // For safety
                debugprint_pos();
                expulsion(TRUE);
            }
            else 
            {
                Qstat(not_ready)++;
                exercise(A_WIS, TRUE);
                debugprint_pos();
                expulsion(FALSE);
            }
        }
        else 
        { /* You are worthy! */
            qt_pager_ex(mtmp, QT_ASSIGNQUEST, ATR_NONE, CLR_MSG_HINT, dopopup);
            exercise(A_WIS, TRUE);
            set_flag(quest_status.bitflags, QSCORE_BITFLAGS_GOT_QUEST, TRUE);
        }
    }
    return res;
}

boolean
leader_speaks(struct monst *mtmp)
{
    /* maybe you attacked leader? */
    if (!is_peaceful(mtmp)) 
    {
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_PISSED_OFF, TRUE);
        mtmp->mstrategy &= ~STRAT_WAITMASK; /* end the inaction */
    }
    /* the quest leader might have passed through the portal into the
       regular dungeon; if so, mustn't perform "backwards expulsion" */
    if (!on_level(&u.uz, &qstart_level))
        return FALSE;

    if (get_flag(quest_status.bitflags, QSCORE_BITFLAGS_PISSED_OFF)) 
    {
        qt_pager_ex(mtmp, QT_LASTLEADER, ATR_NONE, CLR_MSG_NEGATIVE, FALSE);
        debugprint_pos();
        expulsion(TRUE); // Return FALSE for safety
    }
    else if(!get_flag(u.uevent.bitflags, UEVENT_BITFLAGS_QCOMPLETED))
        return chat_with_leader(mtmp, FALSE);

    return FALSE;
}

static boolean
chat_with_nemesis(struct monst *mtmp, boolean dopopup)
{
    /*  The nemesis will do most of the talking, but... */
    if (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_NEMESIS))
        qt_pager_ex(mtmp, QT_FIRSTNEMESIS, ATR_NONE, NO_COLOR, dopopup);
    else if (get_flag(u.uhave.bitflags, UHAVE_BITFLAGS_QUESTART) && !rn2(2))
        qt_pager_ex(mtmp, QT_NEMWANTSIT, ATR_NONE, NO_COLOR, dopopup);
    else
        qt_pager_ex(mtmp, rn1(10, QT_DISCOURAGE), ATR_NONE, NO_COLOR, dopopup);

    if (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_NEMESIS))
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_NEMESIS, TRUE);

    return TRUE;
}

boolean
nemesis_speaks(struct monst *mtmp)
{
    boolean res = TRUE;
    if (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_IN_BATTLE) || !get_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_NEMESIS))
    {
        if (get_flag(u.uhave.bitflags, UHAVE_BITFLAGS_QUESTART))
            qt_pager(mtmp, QT_NEMWANTSIT);
        else if (Qstat(made_goal) == 1 || !get_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_NEMESIS))
            qt_pager(mtmp, QT_FIRSTNEMESIS);
        else if (Qstat(made_goal) < 4)
            qt_pager(mtmp, QT_NEXTNEMESIS);
        else if (Qstat(made_goal) < 7)
            qt_pager(mtmp, QT_OTHERNEMESIS);
        else if (!rn2(5))
            qt_pager(mtmp, rn1(10, QT_DISCOURAGE));
        else
            res = FALSE;

        if (Qstat(made_goal) < 7)
            Qstat(made_goal)++;
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_NEMESIS, TRUE);
    } 
    else
    {/* he will spit out random maledictions */
        if (!rn2(5))
        {
            qt_pager(mtmp, rn1(10, QT_DISCOURAGE));
        }
        else
            res = FALSE;
    }
    return res;
}

static boolean
chat_with_guardian(struct monst *mtmp, boolean dopopup)
{
    /*  These guys/gals really don't have much to say... */
    if (get_flag(u.uhave.bitflags, UHAVE_BITFLAGS_QUESTART) && get_flag(quest_status.bitflags, QSCORE_BITFLAGS_KILLED_NEMESIS))
    {
        qt_pager_ex(mtmp, rn1(5, QT_GUARDTALK2), ATR_NONE, NO_COLOR, dopopup);
    }
    else
    {
        qt_pager_ex(mtmp, rn1(5, QT_GUARDTALK), ATR_NONE, NO_COLOR, dopopup);
    }

    return TRUE;
}

static boolean
prisoner_speaks(struct monst *mtmp)
{
    if (!mtmp)
        return FALSE;

    if (mtmp->data == &mons[PM_PRISONER]
        && (mtmp->mstrategy & STRAT_WAITMASK)) 
    {
        /* Awaken the prisoner */
        if (canseemon(mtmp))
            pline("%s speaks:", Monnam(mtmp));
        verbalize_happy1("I'm finally free!");
        mtmp->mstrategy &= ~STRAT_WAITMASK;
        set_mon_peaceful(mtmp, 1);
        newsym(mtmp->mx, mtmp->my);

        /* Your god is happy... */
        adjalign(3);

        /* ...But the guards are not */
        (void) angry_guards(FALSE);
        return TRUE;
    }
    return FALSE;
}

boolean
quest_chat(struct monst *mtmp, boolean dopopup)
{
    if (mtmp->m_id == Qstat(leader_m_id)) 
    {
        return chat_with_leader(mtmp, dopopup);
    }

    switch (mtmp->data->msound) 
    {
    case MS_NEMESIS:
        return chat_with_nemesis(mtmp, dopopup);
        break;
    case MS_GUARDIAN:
        return chat_with_guardian(mtmp, dopopup);
        break;
    default:
        impossible("quest_chat: Unknown quest character %s.", mon_nam(mtmp));
    }
    return FALSE;
}

boolean
quest_talk(struct monst *mtmp, boolean nearby)
{
    if (!mtmp || !mon_can_move(mtmp))
        return FALSE;

    if (mtmp->m_id == Qstat(leader_m_id) && nearby) 
    {
        return leader_speaks(mtmp);
    }

    switch (mtmp->data->msound)
    {
    case MS_NEMESIS:
        if(nearby || (!get_flag(quest_status.bitflags, QSCORE_BITFLAGS_MET_NEMESIS) && cansee(mtmp->mx, mtmp->my) && distu(mtmp->mx, mtmp->my) <= 64))
            return nemesis_speaks(mtmp);
        break;
    case MS_PRISONER:
        if (nearby)
            return prisoner_speaks(mtmp);
        break;
    default:
        break;
    }

    return FALSE;
}

void
quest_stat_check(struct monst *mtmp)
{
    if (mtmp->data->msound == MS_NEMESIS)
        set_flag(quest_status.bitflags, QSCORE_BITFLAGS_IN_BATTLE, (mon_can_move(mtmp))
                            && monnear(mtmp, u.ux, u.uy));
}

void
achievement_gained(const char *achievement_name)
{
    play_sfx_sound(SFX_ACHIEVEMENT);
    custompline_ex_prefix(ATR_NONE, CLR_MSG_HINT, "ACHIEVEMENT", ATR_NONE, NO_COLOR, " - ", ATR_BOLD, CLR_MSG_HIGHLIGHT, 0U, "%s", achievement_name);
    if(flags.showscore)
        context.botl = context.botlx = 1; /* Make sure score get updated when visible on the status bar */
}

/*quest.c*/
