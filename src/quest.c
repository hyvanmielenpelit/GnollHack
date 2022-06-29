/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-06-05 */

/* GnollHack 4.0    quest.c    $NHDT-Date: 1505170343 2017/09/11 22:52:23 $  $NHDT-Branch: GnollHack-3.6.0 $:$NHDT-Revision: 1.21 $ */
/*      Copyright 1991, M. Stephenson             */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

/*  quest dungeon branch routines. */

#include "quest.h"
#include "qtext.h"

struct gh_quest_base_data quest_definitions[NUM_QUESTS] = {
    { "Amulet of Yendor",
      "Find the Amulet of Yendor for your god",
      { "Obtain the Silver Bell", "Obtain the Candelabrum of Invocation", "Obtain the Book of the Dead", "Perform the Invocation Ritual", 
        "Kill the High Priest of Moloch", "Pick up the Amulet of Yendor", "Exit the Dungeon", "Find the portal to the next level",
        "Find the High Altar of your alignment and sacrifice the Amulet of Yendor on it", 0, 0, 0},
      0
    },
};


#define Not_firsttime (on_level(&u.uz0, &u.uz))
#define Qstat(x) (quest_status.x)

STATIC_DCL void NDECL(on_start);
STATIC_DCL void NDECL(on_locate);
STATIC_DCL void NDECL(on_goal);
STATIC_DCL boolean NDECL(not_capable);
STATIC_DCL int FDECL(is_pure, (BOOLEAN_P));
STATIC_DCL void FDECL(expulsion, (BOOLEAN_P));
STATIC_DCL boolean FDECL(chat_with_leader, (struct monst*));
STATIC_DCL boolean FDECL(chat_with_nemesis, (struct monst*));
STATIC_DCL boolean FDECL(chat_with_guardian, (struct monst*));
STATIC_DCL boolean FDECL(prisoner_speaks, (struct monst *));

STATIC_OVL void
on_start()
{
    if (!Qstat(first_start)) {
        qt_pager_ex((struct monst*)0, QT_FIRSTTIME, ATR_NONE, CLR_MSG_HINT);
        stop_all_immediate_sounds();
        Qstat(first_start) = TRUE;
    } else if ((u.uz0.dnum != u.uz.dnum) || (u.uz0.dlevel < u.uz.dlevel)) {
        if (Qstat(not_ready) <= 2)
        {
            qt_pager_ex((struct monst*)0, QT_NEXTTIME, ATR_NONE, CLR_MSG_HINT);
        }
        else
        {
            qt_pager_ex((struct monst*)0, QT_OTHERTIME, ATR_NONE, CLR_MSG_HINT);
        }
    }
}

STATIC_OVL void
on_locate()
{
    /* the locate messages are phrased in a manner such that they only
       make sense when arriving on the level from above */
    boolean from_above = (u.uz0.dlevel < u.uz.dlevel);

    if (Qstat(killed_nemesis)) {
        return;
    } else if (!Qstat(first_locate)) {
        if (from_above)
        {
            qt_pager_ex((struct monst*)0, QT_FIRSTLOCATE, ATR_NONE, CLR_MSG_HINT);
        }
        /* if we've arrived from below this will be a lie, but there won't
           be any point in delivering the message upon a return visit from
           above later since the level has now been seen */
        Qstat(first_locate) = TRUE;
    } else {
        if (from_above)
        {
            qt_pager_ex((struct monst*)0, QT_NEXTLOCATE, ATR_NONE, CLR_MSG_HINT);
        }
    }
}

STATIC_OVL void
on_goal()
{
    if (Qstat(killed_nemesis)) {
        return;
    } else if (!Qstat(made_goal)) {
        qt_pager_ex((struct monst*)0, QT_FIRSTGOAL, ATR_NONE, CLR_MSG_HINT);
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
onquest()
{
    if (u.uevent.qcompleted || Not_firsttime)
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
nemdead()
{
    if (!Qstat(killed_nemesis)) {
        Qstat(killed_nemesis) = TRUE;
        qt_pager_ex((struct monst*)0, QT_KILLEDNEM, ATR_NONE, CLR_MSG_HINT);
    }
}

void
artitouch(obj)
struct obj *obj;
{
    if (!Qstat(touched_artifact)) {
        /* in case we haven't seen the item yet (ie, currently blinded),
           this quest message describes it by name so mark it as seen */
        obj->dknown = 1;
        /* Quest artifacts become identified by the artifact message */
        obj->aknown = 1;
        obj->nknown = 1;
        /* only give this message once */
        Qstat(touched_artifact) = TRUE;
        qt_pager_ex((struct monst*)0, QT_GOTIT, ATR_NONE, CLR_MSG_HINT);
        exercise(A_WIS, TRUE);
    }
}

/* external hook for do.c (level change check) */
boolean
ok_to_quest()
{
    return (boolean) ((Qstat(got_quest) || Qstat(got_thanks) || Qstat(leader_is_dead))
                      && is_pure(FALSE) > 0);
}

STATIC_OVL boolean
not_capable()
{
    return (boolean) (u.ulevel < MIN_QUEST_LEVEL);
}

STATIC_OVL int
is_pure(talk)
boolean talk;
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
STATIC_OVL void
expulsion(seal)
boolean seal;
{
    branch *br;
    d_level *dest;
    struct trap *t;
    long portal_flag;

    br = dungeon_branch("The Quest");
    dest = (br->end1.dnum == u.uz.dnum) ? &br->end2 : &br->end1;
    portal_flag = u.uevent.qexpelled ? 0 /* returned via artifact? */
                                     : !seal ? 1 : -1;
    schedule_goto(dest, FALSE, FALSE, TRUE, portal_flag, (char *) 0, (char *) 0);
    if (seal) { /* remove the portal to the quest - sealing it off */
        int reexpelled = u.uevent.qexpelled;

        u.uevent.qexpelled = 1;
        remdun_mapseen(quest_dnum);
        /* Delete the near portal now; the far (main dungeon side)
           portal will be deleted as part of arrival on that level.
           If monster movement is in progress, any who haven't moved
           yet will now miss out on a chance to wander through it... */
        for (t = ftrap; t; t = t->ntrap)
            if (t->ttyp == MAGIC_PORTAL)
                break;
        if (t)
            deltrap(t); /* (display might be briefly out of sync) */
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
finish_quest(obj)
struct obj *obj; /* quest artifact; possibly null if carrying Amulet */
{
    struct obj *otmp;

    if (u.uhave.amulet) { /* unlikely but not impossible */
        qt_pager_ex((struct monst*)0, QT_HASAMULET, ATR_NONE, CLR_MSG_HINT);
        /* leader IDs the real amulet but ignores any fakes */
        if ((otmp = carrying(AMULET_OF_YENDOR)) != 0)
            fully_identify_obj(otmp);
    } else {
        qt_pager_ex((struct monst*)0, !Qstat(got_thanks) ? QT_OFFEREDIT : QT_OFFEREDIT2, ATR_NONE, CLR_MSG_HINT);
        /* should have obtained bell during quest;
           if not, suggest returning for it now */
        if ((otmp = carrying(BELL_OF_OPENING)) == 0)
        {
            com_pager((struct monst*)0, 5);
        }
    }
    Qstat(got_thanks) = TRUE;

    if (obj) {
        u.uevent.qcompleted = 1; /* you did it! */
        /* behave as if leader imparts sufficient info about the
           quest artifact */
        fully_identify_obj(obj);
        update_inventory();
    }
}

STATIC_OVL boolean
chat_with_leader(mtmp)
struct monst* mtmp;
{
    boolean res = FALSE;
    /*  Rule 0: Cheater checks. */
    if (u.uhave.questart && !Qstat(met_nemesis))
        Qstat(cheater) = TRUE;

    /*  It is possible for you to get the amulet without completing
     *  the quest.  If so, try to induce the player to quest.
     */
    if (Qstat(got_thanks)) 
    {
        res = TRUE;
        /* Rule 1: You've gone back with/without the amulet. */
        if (u.uhave.amulet)
            finish_quest((struct obj *) 0);

        /* Rule 2: You've gone back before going for the amulet. */
        else
        {
            qt_pager(mtmp, QT_POSTHANKS);
        }

    /* Rule 3: You've got the artifact and are back to return it. */
    }
    else if (u.uhave.questart)
    {
        res = TRUE;
        struct obj *otmp;

        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (is_quest_artifact(otmp))
                break;

        finish_quest(otmp);

    /* Rule 4: You haven't got the artifact yet. */
    } 
    else if (Qstat(got_quest))
    {
        res = TRUE;
        qt_pager(mtmp, rn1(10, QT_ENCOURAGE));

    /* Rule 5: You aren't yet acceptable - or are you? */
    } 
    else
    {
        res = TRUE;
        if (!Qstat(met_leader)) 
        {
            qt_pager_ex(mtmp, QT_FIRSTLEADER, ATR_NONE, CLR_MSG_HINT);
            Qstat(met_leader) = TRUE;
            Qstat(not_ready) = 0;
        } 
        else
        {
            qt_pager(mtmp, QT_NEXTLEADER);
        }

        /* the quest leader might have passed through the portal into
           the regular dungeon; none of the remaining make sense there */
        if (!on_level(&u.uz, &qstart_level))
            return res;

        if (not_capable())
        {
            qt_pager_ex(mtmp, QT_BADLEVEL, ATR_NONE, CLR_MSG_HINT);
            exercise(A_WIS, TRUE);
            res = FALSE; // For safety
            expulsion(FALSE);
        }
        else if (is_pure(TRUE) < 0) 
        {
            com_pager_ex((struct monst*)0, QT_BANISHED, ATR_NONE, CLR_MSG_NEGATIVE);
            res = FALSE; // For safety
            expulsion(TRUE);
        }
        else if (is_pure(TRUE) == 0)
        {
            qt_pager_ex(mtmp, QT_BADALIGN, ATR_NONE, CLR_MSG_WARNING);
            if (Qstat(not_ready) == MAX_QUEST_TRIES)
            {
                qt_pager_ex(mtmp, QT_LASTLEADER, ATR_NONE, CLR_MSG_NEGATIVE);
                res = FALSE; // For safety
                expulsion(TRUE);
            }
            else 
            {
                Qstat(not_ready)++;
                exercise(A_WIS, TRUE);
                expulsion(FALSE);
            }
        }
        else 
        { /* You are worthy! */
            qt_pager_ex(mtmp, QT_ASSIGNQUEST, ATR_NONE, CLR_MSG_HINT);
            exercise(A_WIS, TRUE);
            Qstat(got_quest) = TRUE;
        }
    }
    return res;
}

boolean
leader_speaks(mtmp)
struct monst *mtmp;
{
    /* maybe you attacked leader? */
    if (!is_peaceful(mtmp)) 
    {
        Qstat(pissed_off) = TRUE;
        mtmp->mstrategy &= ~STRAT_WAITMASK; /* end the inaction */
    }
    /* the quest leader might have passed through the portal into the
       regular dungeon; if so, mustn't perform "backwards expulsion" */
    if (!on_level(&u.uz, &qstart_level))
        return FALSE;

    if (Qstat(pissed_off)) 
    {
        qt_pager_ex(mtmp, QT_LASTLEADER, ATR_NONE, CLR_MSG_NEGATIVE);
        expulsion(TRUE); // Return FALSE for safety
    }
    else if(!u.uevent.qcompleted)
        return chat_with_leader(mtmp);

    return FALSE;
}

STATIC_OVL boolean
chat_with_nemesis(mtmp)
struct monst* mtmp;
{
    /*  The nemesis will do most of the talking, but... */
    if (!Qstat(met_nemesis))
        qt_pager(mtmp, QT_FIRSTNEMESIS);
    else if (u.uhave.questart && !rn2(2))
        qt_pager(mtmp, QT_NEMWANTSIT);
    else
        qt_pager(mtmp, rn1(10, QT_DISCOURAGE));

    if (!Qstat(met_nemesis))
        Qstat(met_nemesis) = TRUE;

    return TRUE;
}

boolean
nemesis_speaks(mtmp)
struct monst* mtmp;
{
    boolean res = TRUE;
    if (!Qstat(in_battle) || !Qstat(met_nemesis))
    {
        if (u.uhave.questart)
            qt_pager(mtmp, QT_NEMWANTSIT);
        else if (Qstat(made_goal) == 1 || !Qstat(met_nemesis))
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
        Qstat(met_nemesis) = TRUE;
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

STATIC_OVL boolean
chat_with_guardian(mtmp)
struct monst* mtmp;
{
    /*  These guys/gals really don't have much to say... */
    if (u.uhave.questart && Qstat(killed_nemesis))
    {
        qt_pager(mtmp, rn1(5, QT_GUARDTALK2));
    }
    else
    {
        qt_pager(mtmp, rn1(5, QT_GUARDTALK));
    }

    return TRUE;
}

STATIC_OVL boolean
prisoner_speaks(mtmp)
struct monst *mtmp;
{
    if (!mtmp)
        return FALSE;

    if (mtmp->data == &mons[PM_PRISONER]
        && (mtmp->mstrategy & STRAT_WAITMASK)) 
    {
        /* Awaken the prisoner */
        if (canseemon(mtmp))
            pline("%s speaks:", Monnam(mtmp));
        verbalize("I'm finally free!");
        mtmp->mstrategy &= ~STRAT_WAITMASK;
        mtmp->mpeaceful = 1;
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
quest_chat(mtmp)
register struct monst *mtmp;
{
    if (mtmp->m_id == Qstat(leader_m_id)) 
    {
        return chat_with_leader(mtmp);
    }

    switch (mtmp->data->msound) 
    {
    case MS_NEMESIS:
        return chat_with_nemesis(mtmp);
        break;
    case MS_GUARDIAN:
        return chat_with_guardian(mtmp);
        break;
    default:
        impossible("quest_chat: Unknown quest character %s.", mon_nam(mtmp));
    }
    return FALSE;
}

boolean
quest_talk(mtmp, nearby)
struct monst *mtmp;
boolean nearby;
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
        if(nearby || (!Qstat(met_nemesis) && cansee(mtmp->mx, mtmp->my) && distu(mtmp->mx, mtmp->my) <= 64))
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
quest_stat_check(mtmp)
struct monst *mtmp;
{
    if (mtmp->data->msound == MS_NEMESIS)
        Qstat(in_battle) = (mon_can_move(mtmp)
                            && monnear(mtmp, u.ux, u.uy));
}

void
quest_discovered(quest_id)
int quest_id;
{
    if (quest_id < 0 || quest_id >= NUM_QUESTS)
        return;

    if (context.quest_status_info[quest_id].quest_state == QUEST_STATE_UNDISCOVERED)
    {
        //int title_color = CLR_YELLOW;
        //pline_ex1(ATR_BOLD, title_color, quest_definitions[quest_id].quest_type == 0 ? "MAIN QUEST DISCOVERED" : "OPTIONAL QUEST DISCOVERED");
        //pline_ex1(ATR_NONE, CLR_WHITE, quest_definitions[quest_id].title);

        //char buf[BUFSIZ];
        //Sprintf(buf, "- %s", quest_definitions[quest_id].description);
        //pline_ex1(ATR_NONE, CLR_WHITE, buf);

        context.quest_status_info[quest_id].quest_state = QUEST_STATE_DISCOVERED;
    }
}

void
achievement_gained(achievement_name)
const char* achievement_name;
{
    play_sfx_sound(SFX_ACHIEVEMENT);
    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "Achievement Earned - %s", achievement_name);
}

/*quest.c*/
