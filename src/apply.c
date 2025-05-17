/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    apply.c    $NHDT-Date: 1553363415 2019/03/23 17:50:15 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.272 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

extern boolean notonhead; /* for long worms */

STATIC_DCL int FDECL(use_cubic_gate, (struct obj*));
STATIC_DCL int FDECL(use_camera, (struct obj *));
STATIC_DCL int FDECL(use_towel, (struct obj *));
STATIC_DCL boolean FDECL(its_dead, (int, int, int *));
STATIC_DCL int FDECL(use_stethoscope, (struct obj *));
STATIC_DCL void FDECL(use_whistle, (struct obj *));
STATIC_DCL int FDECL(use_leash, (struct obj *));
STATIC_DCL int FDECL(use_mirror, (struct obj *));
STATIC_DCL int FDECL(use_holysymbol, (struct obj*));
STATIC_DCL void FDECL(use_bell, (struct obj **));
STATIC_PTR void FDECL(display_jump_positions, (int));
STATIC_DCL void FDECL(use_tinning_kit, (struct obj *));
STATIC_DCL void FDECL(use_figurine, (struct obj **));
STATIC_DCL void FDECL(use_grease, (struct obj *));
STATIC_DCL int FDECL(use_wand_on_object, (struct obj*));
STATIC_DCL void FDECL(use_trap, (struct obj *));
STATIC_DCL void FDECL(use_stone, (struct obj *));
STATIC_PTR int NDECL(set_trap); /* occupation callback */
STATIC_DCL int FDECL(use_whip, (struct obj *));
STATIC_PTR void FDECL(display_polearm_positions, (int));
STATIC_DCL int FDECL(use_pole, (struct obj *));

STATIC_DCL int FDECL(use_cream_pie, (struct obj *));
STATIC_DCL int FDECL(use_watch, (struct obj*));
STATIC_DCL int FDECL(use_grapple, (struct obj *));
STATIC_DCL int FDECL(do_break_wand, (struct obj *));
STATIC_DCL boolean FDECL(figurine_location_checks, (struct obj *,
                                                    coord *, BOOLEAN_P));
STATIC_DCL void FDECL(add_class, (char *, CHAR_P));
STATIC_PTR boolean FDECL(check_jump, (genericptr_t, int, int));
STATIC_DCL boolean FDECL(is_valid_jump_pos, (int, int, int, BOOLEAN_P));
STATIC_DCL int FDECL(get_invalid_jump_position, (int, int));
STATIC_DCL int FDECL(get_invalid_polearm_position, (int, int));
STATIC_DCL boolean FDECL(find_poleable_mon, (coord *, int, int));
STATIC_DCL int FDECL(doapply_core, (int));

#ifdef AMIGA
void FDECL(amii_speaker, (struct obj *, char *, int));
#endif

STATIC_VAR const char no_elbow_room[] =
    "don't have enough elbow-room to maneuver.";

STATIC_OVL int
use_cubic_gate(obj)
struct obj* obj;
{
    if (obj->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s", nothing_happens);
        return 1;
    }

    consume_obj_charge(obj, TRUE);
    level_tele(0, FALSE, zerodlevel, 0);
    makeknown(obj->otyp);
    return 1;
}

int
use_salve(obj, drink_yourself)
struct obj* obj;
boolean drink_yourself;
{
    if (obj->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s empty.", Tobjnam(obj, "are"));
        return 0;
    }

    if (Underwater)
    {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Using %s underwater would spoil it.", yname(obj));
        return 0;
    }

    if (drink_yourself)
        u.dx = u.dy = u.dz = 0;
    else if (!getdir((char*)0))
        return 0;

    update_u_facing(TRUE);

    consume_obj_charge(obj, TRUE);

    const char* contents = (objects[obj->otyp].oc_name_known && OBJ_CONTENT_NAME(obj->otyp) != 0 ? OBJ_CONTENT_NAME(obj->otyp) : OBJ_CONTENT_DESC(obj->otyp) != 0 ? OBJ_CONTENT_DESC(obj->otyp) : "unknown contents");

    if (u.dz) 
    {
        if (u.dz > 0 && !u.ux && !u.uy && u.usteed)
        {
            You("apply some %s on %s.", contents, mon_nam(u.usteed));
            weffects(obj);
        }
        else
        {
            You("throw some %s on the %s.", contents,
                (u.dz > 0) ? surface(u.ux, u.uy) : ceiling(u.ux, u.uy));
        }
    }
    else if (!u.dx && !u.dy) 
    {
        if (drink_yourself)
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_QUAFF);
            You("drink some %s.", contents);
        }
        else
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
            You("apply some %s on yourself.", contents);
        }
        (void)zapyourself(obj, TRUE);
    }
    else
    {
        if (isok(u.ux + u.dx, u.uy + u.dy))
        {
            struct monst* mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
            if (mtmp)
            {
                You("apply some %s on %s.", contents, mon_nam(mtmp));
            }
            else if(IS_WALL_OR_SDOOR(levl[u.ux + u.dx][u.uy + u.dy].typ))
            {
                You("throw some %s on the wall.", contents);
            }
            else
            {
                const char* dfeat = dfeature_at(u.ux + u.dx, u.uy + u.dy);
                if (OBJ_AT(u.ux + u.dx, u.uy + u.dy))
                {
                    if(level.objects[u.ux + u.dx][u.uy + u.dy]->nexthere == 0)
                        You("throw some %s on the %s.", contents, cxname(level.objects[u.ux + u.dx][u.uy + u.dy]));
                    else
                        You("throw some %s on the items on the %s.", contents, dfeat ? dfeat : "floor");
                }
                else
                    You("throw some %s on the %s.", contents, dfeat ? dfeat : "floor");
            }
        }
        else
            You("throw some %s away.", contents);

        weffects(obj);
    }

    if (obj->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s now empty.", Tobjnam(obj, "are"));
    }
    return 1;
}

int
use_grail(obj, drink_yourself)
struct obj* obj;
boolean drink_yourself;
{
    const char* contents = OBJ_CONTENT_DESC(obj->otyp);

    if (obj->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s empty.", Tobjnam(obj, "are"));
        return 0;
    }

    if (Underwater)
    {
        pline("Using %s underwater would dilute the %s.", yname(obj), contents);
        return 0;
    }

    if(drink_yourself)
        u.dx = u.dy = u.dz = 0;
    else if (!getdir((char*)0))
        return 0;

    update_u_facing(TRUE);

    consume_obj_charge(obj, TRUE);

    if (u.dz)
    {
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
        if (u.dz > 0 && !u.ux && !u.uy && u.usteed)
        {
            You("apply some %s on %s.", contents, mon_nam(u.usteed));
            weffects(obj);
        }
        else
        {
            You("pour some %s on the %s.", objects[obj->otyp].oc_name_known ? contents : OBJ_CONTENT_DESC(obj->otyp),
                (u.dz > 0) ? surface(u.ux, u.uy) : ceiling(u.ux, u.uy));
        }
    }
    else if (!u.dx && !u.dy)
    {
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_QUAFF);
        You("drink some %s from %s.", objects[obj->otyp].oc_name_known ? contents : OBJ_CONTENT_DESC(obj->otyp), the(cxname(obj)));
        (void)zapyourself(obj, TRUE);
    }
    else
    {
        if (isok(u.ux + u.dx, u.uy + u.dy))
        {
            struct monst* mtmp = m_at(u.ux + u.dx, u.uy + u.dy);
            if (mtmp)
            {
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
                You("%s some %s %s %s.", is_tame(mtmp) ? "give" : "splash", objects[obj->otyp].oc_name_known ? contents : OBJ_CONTENT_DESC(obj->otyp), is_tame(mtmp) ? "to" : "on", mon_nam(mtmp));
            }
            else if (IS_WALL_OR_SDOOR(levl[u.ux + u.dx][u.uy + u.dy].typ))
            {
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
                You("pour some %s on the wall.", objects[obj->otyp].oc_name_known ? contents : OBJ_CONTENT_DESC(obj->otyp));
            }
            else
            {
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
                const char* dfeat = dfeature_at(u.ux + u.dx, u.uy + u.dy);
                You("pour some %s on the %s.", objects[obj->otyp].oc_name_known ? contents : OBJ_CONTENT_DESC(obj->otyp), dfeat ? dfeat : "floor");
            }
        }
        else
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
            You("pour some %s out of %s.", objects[obj->otyp].oc_name_known ? contents : OBJ_CONTENT_DESC(obj->otyp), the(cxname(obj)));
        }
        weffects(obj);
    }

    if (obj->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s now empty.", Tobjnam(obj, "are"));
    }
    return 1;
}


STATIC_OVL int
use_camera(obj)
struct obj *obj;
{
    struct monst *mtmp;
    boolean action_taken = FALSE;

    if (Underwater) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, NO_COLOR, "Using your camera underwater would void the warranty.");
        return 0;
    }
    if (!getdir((char *) 0))
        return 0;

    update_u_facing(TRUE);

    boolean takeselfie = FALSE;
    mtmp = 0;
    if (!u.dz)
    {
        if (!u.dx && !u.dy)
            mtmp = &youmonst;
        else
            mtmp = m_at(u.ux + u.dx, u.uy + u.dy);

        char selfiebuf[BUFSZ] = "Take a selfie?";
        if (mtmp && mtmp != &youmonst)
            Sprintf(selfiebuf, "Take a selfie with %s?", mon_nam(mtmp));

        if (mtmp && (mtmp == &youmonst || canseemon(mtmp)))
        {
            char ans = ynq(selfiebuf);
            switch (ans)
            {
            case 'y':
                takeselfie = TRUE;
                break;
            case 'n':
                break;
            default:
            case 'q':
                return 0;
            }
        }
    }

    if (obj->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline_ex1(ATR_NONE, CLR_MSG_FAIL, nothing_happens);
        return 1;
    }

    consume_obj_charge(obj, TRUE);

    if (u_action_flags(ACTION_TILE_SPECIAL_ATTACK) & ACTION_SPECIAL_ATTACK_FLAGS_CAMERA)
    {
        update_u_action(ACTION_TILE_SPECIAL_ATTACK);
        action_taken = TRUE;
    }
    else  if (u_action_flags(ACTION_TILE_ITEM_USE) & ACTION_SPECIAL_ATTACK_FLAGS_CAMERA)
    {
        update_u_action(ACTION_TILE_ITEM_USE);
        action_taken = TRUE;
    }

    play_sfx_sound(SFX_CAMERA_CLICK);
    u_wait_until_action();

    if (obj->cursed && !rn2(2)) 
    {
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "camera slips and you take a picture of yourself.");
        (void) zapyourself(obj, TRUE);
    }
    else if (u.uswallow) 
    {
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "take a picture of %s %s.", s_suffix(mon_nam(u.ustuck)),
            mbodypart(u.ustuck, STOMACH));
    }
    else if (u.dz) 
    {
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "take a picture of the %s.",
            (u.dz > 0) ? surface(u.ux, u.uy) : ceiling(u.ux, u.uy));
    }
    else if ((!u.dx && !u.dy) || mtmp == &youmonst)
    {
        if (takeselfie)
        {
            You_ex1(ATR_NONE, CLR_MSG_SUCCESS, "take a selfie.");
            if (!rn2(2))
                You_ex1(ATR_NONE, CLR_MSG_SUCCESS, "quite like it.");
            else
                pline_ex1(ATR_NONE, CLR_MSG_FAIL, "Maybe that wasn't one of your best takes.");
        }
        else
        {
            You_ex1(ATR_NONE, CLR_MSG_SUCCESS, "take a picture of yourself.");
            (void)zapyourself(obj, TRUE);
        }
    } 
    else
    {
        if (takeselfie && mtmp)
        {
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "take a selfie with %s.", mon_nam(mtmp));

            if (Role_if(PM_TOURIST) && (mvitals[mtmp->mnum].mvflags & MV_SELFIE_TAKEN) == 0)
            {
                context.role_score += TOURIST_SELFIE_PER_LEVEL_SCORE * (mons[mtmp->mnum].difficulty + 1);
                pline_ex1(ATR_NONE, CLR_MSG_POSITIVE, "That turned out to be extraordinarily nice.");
                if (mtmp->mnum == PM_DEMOGORGON && !u.uachieve.role_achievement)
                {
                    u.uachieve.role_achievement = 1;
                    achievement_gained("Took a Selfie with Demogorgon");
                    livelog_printf(LL_ACHIEVE, "%s", "took a selfie with Demogorgon");
                }
            }
            else
            {
                if (!rn2(2))
                    You_ex1(ATR_NONE, CLR_MSG_SUCCESS, "quite like it.");
                else
                    pline_ex1(ATR_NONE, CLR_MSG_FAIL, "Maybe that wasn't one of your best takes.");
            }
            mvitals[mtmp->mnum].mvflags |= MV_SELFIE_TAKEN;
            context.botl = context.botlx = 1;
            struct special_view_info info = { 0 };
            info.viewtype = SPECIAL_VIEW_SELFIE;
            (void)open_special_view(info);
        }
        else
        {
            if ((mtmp = bhit(u.dx, u.dy, COLNO, 0, FLASHED_LIGHT,
                (int FDECL((*), (MONST_P, OBJ_P, MONST_P))) 0,
                (int FDECL((*), (OBJ_P, OBJ_P, MONST_P))) 0, 
                (int FDECL((*), (TRAP_P, OBJ_P, MONST_P))) 0, 
                &obj, &youmonst, TRUE, FALSE)) != 0)
            {
                obj->ox = u.ux, obj->oy = u.uy;
                    (void)flash_hits_mon(mtmp, obj);
            }
        }
    }

    if(action_taken)
        update_u_action_revert(ACTION_TILE_NO_ACTION);

    return 1;
}

STATIC_OVL int
use_towel(obj)
struct obj *obj;
{
    boolean drying_feedback = (obj == uwep);

    if (!freehand())
    {
        You_ex(ATR_NONE, CLR_MSG_WARNING, "have no free %s!", body_part(HAND));
        return 0;
    } 
    else if (obj == ublindf)
    {
        You("cannot use it while you're wearing it!");
        return 0;
    } 
    else if (obj->cursed) 
    {
        int64_t old;

        switch (rn2(3)) 
        {
        case 2:
            old = Glib;
            play_sfx_sound(SFX_ACQUIRE_GLIB);
            incr_itimeout(&Glib, rn1(10, 3));
            refresh_u_tile_gui_info(TRUE);
            Your("%s %s!", makeplural(body_part(HAND)),
                 (old ? "are filthier than ever" : "get slimy"));
            if (is_wet_towel(obj))
                dry_a_towel(obj, -1, drying_feedback);
            return 1;
        case 1:
            if (!ublindf) 
            {
                old = u.ucreamed;
                u.ucreamed += rn1(10, 3);
                pline("Yecch!  Your %s %s gunk on it!", body_part(FACE),
                      (old ? "has more" : "now has"));
                make_blinded(Blinded + (int64_t) u.ucreamed - old, TRUE);
            } 
            else
            {
                const char *what;

                what = (obj->otyp == ublindf->otyp) ? "other towel"
                                                          : "blindfold";
                if (ublindf->cursed)
                {
                    You("push your %s %s.", what,
                        rn2(2) ? "cock-eyed" : "crooked");
                } else {
                    struct obj *saved_ublindf = ublindf;
                    You("push your %s off.", what);
                    Blindf_off(ublindf);
                    dropxf(saved_ublindf);
                }
            }
            if (is_wet_towel(obj))
                dry_a_towel(obj, -1, drying_feedback);
            return 1;
        case 0:
            break;
        }
    }

    if (Glib) 
    {
        Glib = 0;
        You("wipe off your %s.", makeplural(body_part(HAND)));
        if (is_wet_towel(obj))
            dry_a_towel(obj, -1, drying_feedback);
        return 1;
    } 
    else if (u.ucreamed) 
    {
        Blinded -= u.ucreamed;
        u.ucreamed = 0;
        if (!Blinded)
        {
            pline("You've got the glop off.");
            if (!gulp_blnd_check()) 
            {
                Blinded = 1;
                make_blinded(0L, TRUE);
            }
        } else {
            Your("%s feels clean now.", body_part(FACE));
        }
        if (is_wet_towel(obj))
            dry_a_towel(obj, -1, drying_feedback);
        return 1;
    }

    Your("%s and %s are already clean.", body_part(FACE),
         makeplural(body_part(HAND)));

    return 0;
}

/* maybe give a stethoscope message based on floor objects */
STATIC_OVL boolean
its_dead(rx, ry, resp)
int rx, ry, *resp;
{
    char buf[BUFSZ];
    boolean more_corpses;
    struct permonst *mptr;
    struct obj *corpse = sobj_at(CORPSE, rx, ry),
               *statue = sobj_at(STATUE, rx, ry);

    if (!can_reach_floor(TRUE)) 
    { /* levitation or unskilled riding */
        corpse = 0;               /* can't reach corpse on floor */
        /* you can't reach tiny statues (even though you can fight
           tiny monsters while levitating--consistency, what's that?) */
        while (statue && mons[statue->corpsenm].msize == MZ_TINY)
            statue = nxtobj(statue, STATUE, TRUE);
    }

    /* when both corpse and statue are present, pick the uppermost one */
    if (corpse && statue) 
    {
        if (nxtobj(statue, CORPSE, TRUE) == corpse)
            corpse = 0; /* corpse follows statue; ignore it */
        else
            statue = 0; /* corpse precedes statue; ignore statue */
    }

    more_corpses = (corpse && nxtobj(corpse, CORPSE, TRUE));

    /* additional stethoscope messages from jyoung@apanix.apana.org.au */
    if (!corpse && !statue)
    {
        ; /* nothing to do */

    }
    else if (Hallucination)
    {
        if (!corpse) 
        {
            /* it's a statue */
            Strcpy(buf, "You're both stoned");
        } 
        else if (corpse->quan == 1L && !more_corpses) 
        {
            int gndr = 2; /* neuter: "it" */
            struct monst *mtmp = get_mtraits(corpse, FALSE);

            /* (most corpses don't retain the monster's sex, so
               we're usually forced to use generic pronoun here) */
            if (mtmp) 
            {
                mptr = mtmp->data = &mons[mtmp->mnum];
                /* TRUE: override visibility check--it's not on the map */
                gndr = pronoun_gender(mtmp, TRUE);
            } 
            else 
            {
                mptr = &mons[corpse->corpsenm];
                if (is_female(mptr))
                    gndr = 1;
                else if (is_male(mptr))
                    gndr = 0;
            }
            Sprintf(buf, "%s's dead", genders[gndr].he); /* "he"/"she"/"it" */
            buf[0] = highc(buf[0]);
        }
        else
        { /* plural */
            Strcpy(buf, "They're dead");
        }
        /* variations on "He's dead, Jim." (Star Trek's Dr McCoy) */
        You_hear("a voice say, \"%s, Jim.\"", buf);
        *resp = 1;
        return TRUE;

    } 
    else if (corpse) 
    {
        boolean here = (rx == u.ux && ry == u.uy),
                one = (corpse->quan == 1L && !more_corpses), reviver = FALSE;
        int visglyph, corpseglyph;

        visglyph = glyph_at(rx, ry);
        corpseglyph = obj_to_glyph(corpse, rn2);

        if (Blind && (visglyph != corpseglyph))
            map_object(corpse, TRUE);

        if (Role_if(PM_HEALER)) 
        {
            /* ok to reset `corpse' here; we're done with it */
            do {
                if (obj_has_timer(corpse, REVIVE_MON))
                    reviver = TRUE;
                else
                    corpse = nxtobj(corpse, CORPSE, TRUE);
            } while (corpse && !reviver);
        }
        You("determine that %s unfortunate being%s %s%s dead.",
            one ? (here ? "this" : "that") : (here ? "these" : "those"),
            one ? "" : "s", one ? "is" : "are", reviver ? " mostly" : "");
        return TRUE;

    }
    else
    { /* statue */
        const char *what, *how;

        mptr = &mons[statue->corpsenm];
        if (Blind)
        { /* ignore statue->dknown; it'll always be set */
            Sprintf(buf, "%s %s",
                    (rx == u.ux && ry == u.uy) ? "This" : "That",
                    humanoid(mptr) ? "person" : "creature");
            what = buf;
        } 
        else 
        {
            what = corpse_monster_name(statue);
            if (!is_mname_proper_name(mptr))
                what = The(what);
        }
        how = "fine";
        if (Role_if(PM_HEALER))
        {
            struct trap *ttmp = t_at(rx, ry);

            if (ttmp && ttmp->ttyp == STATUE_TRAP)
                how = "extraordinary";
            else if (Has_contents(statue))
                how = "remarkable";
        }

        pline("%s is in %s health for a statue.", what, how);
        return TRUE;
    }
    return FALSE; /* no corpse or statue */
}

STATIC_VAR const char hollow_str[] = "a hollow sound.  This must be a secret %s!";

/* Strictly speaking it makes no sense for usage of a stethoscope to
   not take any time; however, unless it did, the stethoscope would be
   almost useless.  As a compromise, one use per turn is free, another
   uses up the turn; this makes curse status have a tangible effect. */
STATIC_OVL int
use_stethoscope(obj)
register struct obj *obj;
{
    struct monst *mtmp;
    struct rm *lev;
    int rx, ry, res;
    boolean interference = (u.uswallow && is_whirly(u.ustuck->data)
                            && !rn2(Role_if(PM_HEALER) ? 10 : 3));

    if (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "have no hands!"); /* not `body_part(HAND)' */
        return 0;
    } else if (Deaf) {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "hear anything!");
        return 0;
    } else if (!freehand() && !is_telekinetic_operator(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "have no free %s.", body_part(HAND));
        return 0;
    }
    if (!getdir((char *) 0))
        return 0;

    update_u_facing(TRUE);

    res = (moves == context.stethoscope_move)
          && (youmonst.movement == context.stethoscope_movement);
    context.stethoscope_move = moves;
    context.stethoscope_movement = youmonst.movement;

    bhitpos.x = u.ux, bhitpos.y = u.uy; /* tentative, reset below */
    notonhead = u.uswallow;
    if (u.usteed && u.dz > 0) {
        if (interference) {
            pline("%s interferes.", Monnam(u.ustuck));
            mstatusline(u.ustuck);
        } else
            mstatusline(u.usteed);
        return res;
    } else if (u.uswallow && (u.dx || u.dy || u.dz)) {
        mstatusline(u.ustuck);
        return res;
    } else if (u.uswallow && interference) {
        pline("%s interferes.", Monnam(u.ustuck));
        mstatusline(u.ustuck);
        return res;
    } else if (u.dz) {
        if (Underwater)
            You_hear("faint splashing.");
        else if (u.dz < 0 || !can_reach_floor(TRUE))
            cant_reach_floor(u.ux, u.uy, (u.dz < 0), TRUE);
        else if (its_dead(u.ux, u.uy, &res))
            ; /* message already given */
        else if (Is_stronghold(&u.uz))
            You_hear("the crackling of hellfire.");
        else
            pline_The("%s seems healthy enough.", surface(u.ux, u.uy));
        return res;
    } else if (obj->cursed && !rn2(2)) {
        You_hear("your heart beat.");
        return res;
    }
    if (Stunned || (Confusion && !rn2(5)))
        confdir();
    if (!u.dx && !u.dy) {
        ustatusline();
        return res;
    }
    rx = u.ux + u.dx;
    ry = u.uy + u.dy;
    if (!isok(rx, ry)) {
        You_hear("a faint typing noise.");
        return 0;
    }
    if ((mtmp = m_at(rx, ry)) != 0) {
        const char *mnm = x_monnam(mtmp, ARTICLE_A, (const char *) 0, SUPPRESS_IT | SUPPRESS_INVISIBLE, FALSE);

        /* bhitpos needed by mstatusline() iff mtmp is a long worm */
        bhitpos.x = rx, bhitpos.y = ry;
        notonhead = (mtmp->mx != rx || mtmp->my != ry);

        if (mtmp->mundetected) {
            if (!canspotmon(mtmp))
            {
                play_sfx_sound(SFX_WAS_HIDING);
                There("is %s hidden there.", mnm);
            }
            mtmp->mundetected = 0;
            newsym(mtmp->mx, mtmp->my);
        } else if (mtmp->mappearance) {
            const char *what = "thing";

            switch (M_AP_TYPE(mtmp)) {
            case M_AP_OBJECT:
                what = simple_typename(mtmp->mappearance);
                break;
            case M_AP_MONSTER: /* ignore Hallucination here */
                what = pm_monster_name(&mons[mtmp->mappearance], mtmp->female);
                break;
            case M_AP_FURNITURE:
                what = defsyms[mtmp->mappearance].explanation;
                break;
            }
            seemimic(mtmp);
            pline("That %s is really %s.", what, mnm);
        } else if (flags.verbose && !canspotmon(mtmp)) {
            There("is %s there.", mnm);
        }

        mstatusline(mtmp);
        if (!canspotmon(mtmp))
            map_invisible(rx, ry);
        return res;
    }
    if (unmap_invisible(rx,ry))
        pline_The("invisible monster must have moved.");

    lev = &levl[rx][ry];
    switch (lev->typ) {
    case SDOOR:
        You_hear(hollow_str, "door");
        cvt_sdoor_to_door_with_animation(rx, ry); /* ->typ = DOOR */
        feel_newsym(rx, ry);
        return res;
    case SCORR:
        You_hear(hollow_str, "passage");
        //create_basic_floor_location(rx, ry, levl[rx][ry].floortyp ? levl[rx][ry].floortyp : CORR, 0, 0, FALSE);
        //unblock_vision_and_hearing_at_point(rx, ry);
        //feel_newsym(rx, ry);
        cvt_scorr_to_corr_with_animation(rx, ry);
        return res;
    }

    if (!its_dead(rx, ry, &res))
        You("hear nothing special."); /* not You_hear()  */
    return res;
}

STATIC_VAR const char whistle_str[] = "produce a %s whistling sound.";

STATIC_OVL void
use_whistle(obj)
struct obj *obj;
{
    if (!obj)
        return;

    if (!can_blow(&youmonst))
    {
        You("are incapable of using the whistle.");
    } 
    else if (Underwater) 
    {
        play_sfx_sound(SFX_WHISTLE_UNDERWATER);
        You("blow bubbles through %s.", yname(obj));
    }
    else
    {
        if (Deaf)
            You_feel("rushing air tickle your %s.",
                        body_part(NOSE));
        else if(obj)
            You(whistle_str, obj->cursed ? "shrill" : "high");

        play_sfx_sound(obj->cursed ? SFX_CURSED_WHISTLE : SFX_WHISTLE);

        wake_nearby();

        /* It does bring your pets to you now! --JG*/
        for (struct monst* mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (!DEADMONSTER(mtmp) && is_tame(mtmp) && !is_deaf(mtmp))
            {
                mtmp->mcomingtou = 100 + rnd(50);
                mtmp->notraveltimer = 0;
                mtmp->yell_x = u.ux;
                mtmp->yell_y = u.uy;
            }
        }

        if (obj->cursed)
            vault_summon_gd();
    }
}

void
use_magic_whistle(obj)
struct obj *obj;
{
    register struct monst *mtmp, *nextmon, *selmon;
    selmon = (struct monst*)0;

    if (obj && !can_blow(&youmonst))
    {
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are incapable of using the whistle.");
    } 
    else if (obj && obj->cursed && !rn2(2)) 
    {
        play_sfx_sound(SFX_CURSED_MAGIC_WHISTLE);
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "produce a %shigh-pitched humming noise.",
            Underwater ? "very " : "");
        wake_nearby();
    } 
    else
    {
        int pet_cnt = 0, omx, omy;

        /* it's magic!  it works underwater too (at a higher pitch) */
        if(obj)
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, whistle_str,
                Hallucination ? "normal" : Underwater ? "strange, high-pitched"
                                                  : "strange");
        else
            Your_ex(ATR_NONE, CLR_MSG_SPELL, "spell produces a %s whistling sound.",
                Hallucination ? "normal" : Underwater ? "strange, high-pitched"
                : "strange");
        
        play_sfx_sound(Hallucination ? SFX_WHISTLE : Underwater ? SFX_MAGIC_WHISTLE_UNDERWATER : SFX_MAGIC_WHISTLE);
        if (iflags.using_gui_sounds)
            delay_output_milliseconds(500);

        for (mtmp = fmon; mtmp; mtmp = nextmon)
        {
            nextmon = mtmp->nmon; /* trap might kill mon */
            if (DEADMONSTER(mtmp))
                continue;
            /* steed is already at your location, so not affected;
               this avoids trap issues if you're on a trap location */
            if (mtmp == u.usteed)
                continue;
            if (is_tame(mtmp)) 
            {
                if (mtmp->mtrapped) 
                {
                    /* no longer in previous trap (affects mintrap) */
                    mtmp->mtrapped = 0;
                    fill_pit(mtmp->mx, mtmp->my);
                }
                /* mimic must be revealed before we know whether it
                   actually moves because line-of-sight may change */
                if (M_AP_TYPE(mtmp))
                    seemimic(mtmp);
                omx = mtmp->mx, omy = mtmp->my;
                mnexto2(mtmp, TRUE);
                if (mtmp->mx != omx || mtmp->my != omy) 
                {
                    mtmp->mundetected = 0; /* reveal non-mimic hider */
                    selmon = mtmp;
                    if (canspotmon(mtmp))
                        ++pet_cnt;
                    if (mintrap(mtmp) == 2)
                        change_luck(-1, TRUE);
                }
            }
        }
        if (pet_cnt > 0)
        {
            if(obj)
                makeknown(obj->otyp);
            else
            {
                if(pet_cnt == 1 && selmon)
                    pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s appears in a cloud of smoke!", Ymonnam(selmon));
                else
                    Your_ex(ATR_NONE, CLR_MSG_SUCCESS, "pets appear in a cloud of smoke!");
            }
        }
    }
}

boolean
um_dist(x, y, n)
xchar x, y, n;
{
    return (boolean) (abs(u.ux - x) > n || abs(u.uy - y) > n);
}

int
number_leashed(VOID_ARGS)
{
    int i = 0;
    struct obj *obj;

    for (obj = invent; obj; obj = obj->nobj)
        if (obj->otyp == LEASH && obj->leashmon != 0)
            i++;
    return i;
}

/* otmp is about to be destroyed or stolen */
void
o_unleash(otmp)
register struct obj *otmp;
{
    register struct monst *mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        if (mtmp->m_id == (unsigned) otmp->leashmon)
            mtmp->mleashed = 0;
    otmp->leashmon = 0;
}

/* mtmp is about to die, or become untame */
void
m_unleash(mtmp, feedback)
register struct monst *mtmp;
boolean feedback;
{
    if (!mtmp)
        return;

    register struct obj *otmp;

    if (feedback) {
        if (canseemon(mtmp))
            pline("%s pulls free of %s leash!", Monnam(mtmp), mhis(mtmp));
        else
            Your("leash falls slack.");
    }
    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == LEASH && otmp->leashmon == (int) mtmp->m_id)
            otmp->leashmon = 0;
    mtmp->mleashed = 0;
}

/* player is about to die (for bones) */
void
unleash_all(VOID_ARGS)
{
    register struct obj *otmp;
    register struct monst *mtmp;

    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp->otyp == LEASH)
            otmp->leashmon = 0;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        mtmp->mleashed = 0;
}

/* TODO:
 *  This ought to exclude various other things, such as lights and gas
 *  spore, is_whirly() critters, ethereal creatures, possibly others.
 */
STATIC_OVL boolean
leashable(mtmp)
struct monst *mtmp;
{
    return (boolean) (!is_long_worm_with_tail(mtmp->data) && !is_whirly(mtmp->data) && !is_incorporeal(mtmp->data));
}

/* ARGSUSED */
STATIC_OVL int
use_leash(obj)
struct obj *obj;
{
    coord cc;
    struct monst *mtmp;
    int spotmon;

    if (u.uswallow) 
    {
        /* if the leash isn't in use, assume we're trying to leash
           the engulfer; if it is use, distinguish between removing
           it from the engulfer versus from some other creature
           (note: the two in-use cases can't actually occur; all
           leashes are released when the hero gets engulfed) */
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, (!obj->leashmon
                  ? "leash %s from inside."
                  : (obj->leashmon == (int) u.ustuck->m_id)
                    ? "unleash %s from inside."
                    : "unleash anything from inside %s."),
                 noit_mon_nam(u.ustuck));
        return 0;
    }
    if (!obj->leashmon && number_leashed() >= MAXLEASHED) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot leash any more pets.");
        return 0;
    }

    if (!get_adjacent_loc((char *) 0, (char *) 0, u.ux, u.uy, &cc))
        return 0;

    if (cc.x == u.ux && cc.y == u.uy)
    {
        if (u.usteed && u.dz > 0)
        {
            mtmp = u.usteed;
            spotmon = 1;
            goto got_target;
        }
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Leash yourself?  Very funny...");
        return 0;
    }

    /*
     * From here on out, return value is 1 == a move is used.
     */

    if (!(mtmp = m_at(cc.x, cc.y)))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        There_ex(ATR_NONE, CLR_MSG_FAIL, "is no creature there.");
        (void) unmap_invisible(cc.x, cc.y);
        return 1;
    }

    spotmon = canspotmon(mtmp);
 got_target:

    if (!spotmon && !glyph_is_invisible(levl[cc.x][cc.y].hero_memory_layers.glyph)) 
    {
        /* for the unleash case, we don't verify whether this unseen
           monster is the creature attached to the current leash */
        play_sfx_sound(SFX_FAILS_TO_LEASH);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "fail to %sleash something.", obj->leashmon ? "un" : "");
        /* trying again will work provided the monster is tame
           (and also that it doesn't change location by retry time) */
        map_invisible(cc.x, cc.y);
    } 
    else if (!is_tame(mtmp))
    {
        play_sfx_sound(SFX_FAILS_TO_LEASH);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s leashed!", Monnam(mtmp),
              (!obj->leashmon) ? "cannot be" : "is not");
    }
    else if (!obj->leashmon) 
    {
        /* applying a leash which isn't currently in use */
        if (mtmp->mleashed)
        {
            play_sfx_sound(SFX_GENERAL_ALREADY_DONE);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "This %s is already leashed.",
                  spotmon ? l_monnam(mtmp) : "creature");
        }
        else if (!leashable(mtmp)) 
        {
            play_sfx_sound(SFX_GENERAL_DOES_NOT_FIT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "The leash won't fit onto %s%s.", spotmon ? "your " : "",
                  l_monnam(mtmp));
        }
        else
        {
            play_sfx_sound(SFX_PUT_ON_LEASH);
            You("slip the leash around %s%s.", spotmon ? "your " : "",
                l_monnam(mtmp));
            mtmp->mleashed = 1;
            obj->leashmon = (int) mtmp->m_id;
            mtmp->msleeping = 0;
            refresh_m_tile_gui_info(mtmp, TRUE);
        }
    }
    else 
    {
        /* applying a leash which is currently in use */
        if (obj->leashmon != (int) mtmp->m_id)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "This leash is not attached to that creature.");
        }
        else if (obj->cursed) 
        {
            play_sfx_sound(SFX_GENERAL_WELDED);
            pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "leash would not come off!");
            if (!obj->bknown)
            {
                obj->bknown = TRUE;
                update_inventory();
            }
        }
        else
        {
            mtmp->mleashed = 0;
            obj->leashmon = 0;
            play_sfx_sound(SFX_REMOVE_LEASH);
            You("remove the leash from %s%s.",
                spotmon ? "your " : "", l_monnam(mtmp));
        }
    }
    return 1;
}

/* assuming mtmp->mleashed has been checked */
struct obj *
get_mleash(mtmp)
struct monst *mtmp;
{
    struct obj *otmp;

    otmp = invent;
    while (otmp) {
        if (otmp->otyp == LEASH && otmp->leashmon == (int) mtmp->m_id)
            return otmp;
        otmp = otmp->nobj;
    }
    return (struct obj *) 0;
}

boolean
next_to_u(VOID_ARGS)
{
    register struct monst *mtmp;
    register struct obj *otmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->mleashed) {
            if (distu(mtmp->mx, mtmp->my) > 2)
                mnexto(mtmp);
            if (distu(mtmp->mx, mtmp->my) > 2) {
                for (otmp = invent; otmp; otmp = otmp->nobj)
                    if (otmp->otyp == LEASH
                        && otmp->leashmon == (int) mtmp->m_id) {
                        if (otmp->cursed)
                            return FALSE;

                        play_sfx_sound(SFX_LEASH_GOES_SLACK);
                        You_feel("%s leash go slack.",
                                 (number_leashed() > 1) ? "a" : "the");
                        mtmp->mleashed = 0;
                        otmp->leashmon = 0;
                    }
            }
        }
    }
    /* no pack mules for the Amulet */
    if (u.usteed && mon_has_amulet(u.usteed))
        return FALSE;
    return TRUE;
}

void
check_leash(x, y)
register xchar x, y;
{
    register struct obj *otmp;
    register struct monst *mtmp;

    for (otmp = invent; otmp; otmp = otmp->nobj) 
    {
        if (otmp->otyp != LEASH || otmp->leashmon == 0)
            continue;
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (DEADMONSTER(mtmp))
                continue;
            if ((int) mtmp->m_id == otmp->leashmon)
                break;
        }
        if (!mtmp) 
        {
            impossible("leash in use isn't attached to anything?");
            otmp->leashmon = 0;
            continue;
        }
        if (dist2(u.ux, u.uy, mtmp->mx, mtmp->my)
            > dist2(x, y, mtmp->mx, mtmp->my)) 
        {
            if (!um_dist(mtmp->mx, mtmp->my, 3)) 
            {
                ; /* still close enough */
            } 
            else if (otmp->cursed && !has_innate_breathless(mtmp->data)) 
            {
                if (um_dist(mtmp->mx, mtmp->my, 5)
                    || deduct_monster_hp(mtmp, rnd(2)) <= 0) 
                {
                    int64_t save_pacifism = u.uconduct.killer;

                    Your("leash chokes %s to death!", mon_nam(mtmp));
                    /* hero might not have intended to kill pet, but
                       that's the result of his actions; gain experience,
                       lose pacifism, take alignment and luck hit, make
                       corpse less likely to remain tame after revival */
                    xkilled(mtmp, XKILL_NOMSG);
                    /* life-saving doesn't ordinarily reset this */
                    if (!DEADMONSTER(mtmp))
                        u.uconduct.killer = save_pacifism;
                } 
                else 
                {
                    pline("%s is choked by the leash!", Monnam(mtmp));
                    /* tameness eventually drops to 1 here (never 0) */
                    if (mtmp->mtame/**/ && !mindless(mtmp->data) && rn2(mtmp->mtame/**/))
                        mtmp->mtame--;

                    if (!mtmp->mtame)
                        mtmp->ispartymember = FALSE;

                }
            } 
            else 
            {
                if (um_dist(mtmp->mx, mtmp->my, 5))
                {
                    play_sfx_sound(SFX_LEASH_SNAPS_LOOSE);
                    pline("%s leash snaps loose!", s_suffix(Monnam(mtmp)));
                    m_unleash(mtmp, FALSE);
                } 
                else 
                {
                    You("pull on the leash.");
                    if (mtmp->data->msound != MS_SILENT)
                        switch (rn2(3))
                        {
                        case 0:
                            growl(mtmp);
                            break;
                        case 1:
                            yelp(mtmp);
                            break;
                        default:
                            whimper(mtmp);
                            break;
                        }
                }
            }
        }
    }
}

const char *
beautiful(VOID_ARGS)
{
    return ((ACURR(A_CHA) > 14)
               ? ((poly_gender() == 1)
                     ? "beautiful"
                     : "handsome")
               : "ugly");
}

STATIC_VAR const char look_str[] = "look %s.";

STATIC_OVL int
use_mirror(obj)
struct obj *obj;
{
    const char *mirror, *uvisage;
    struct monst *mtmp;
    unsigned how_seen;
    char mlet;
    boolean vis, invis_mirror, useeit, monable;

    if (!getdir((char *) 0))
        return 0;
    update_u_facing(TRUE);
    invis_mirror = Invis;
    useeit = !Blind && (!invis_mirror || See_invisible);
    uvisage = beautiful();
    mirror = simpleonames(obj); /* "mirror" or "looking glass" */

    if (obj->cursed && !rn2(2))
    {
        if (!Blind)
            pline_The("%s fogs up and doesn't reflect!", mirror);
        return 1;
    }

    if (!u.dx && !u.dy && !u.dz) 
    {
        if (!useeit) 
        {
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "see your %s %s.", uvisage, body_part(FACE));
        } 
        else
        {
            if (u.umonnum == PM_FLOATING_EYE) 
            {
                if (Free_action) 
                {
                    You("stiffen momentarily under your gaze.");
                }
                else
                {
                    if (Hallucination)
                        pline("Yow!  The %s stares back!", mirror);
                    else
                        pline("Yikes!  You've frozen yourself!");
                    if (!Hallucination || !rn2(4)) 
                    {
                        nomul(-rnd(MAXULEV + 6 - u.ulevel));
                        multi_reason = "gazing into a mirror";
                    }
                    nomovemsg = 0; /* default, "you can move again" */
                }
            } 
            else if (youmonst.data->mlet == S_VAMPIRE)
                You("don't have a reflection.");
            else if (u.umonnum == PM_UMBRAL_HULK)
            {
                pline("Huh?  That doesn't look like you!");
                if (!Confusion)
                    play_sfx_sound(SFX_ACQUIRE_CONFUSION);
                make_confused(itimeout_incr(HConfusion, d(3, 4)), FALSE);
            } 
            else if (Hallucination)
                You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolor_buffer, look_str, hcolor_multi_buf0((char *) 0));
            else if (Sick)
                You(look_str, "peaked");
            else if (FoodPoisoned)
                You(look_str, "ill");
            else if (MummyRot)
                You(look_str, "unnaturally gaunt");
            else if (u.uhs >= WEAK)
                You(look_str, "undernourished");
            else
                You("look as %s as ever.", uvisage);
        }
        return 1;
    }

    if (u.uswallow) 
    {
        if (useeit)
            You("reflect %s %s.", s_suffix(mon_nam(u.ustuck)),
                mbodypart(u.ustuck, STOMACH));
        return 1;
    }

    if (Underwater)
    {
        if (useeit)
            You(Hallucination ? "give the fish a chance to fix their makeup."
                              : "reflect the murky water.");
        return 1;
    }

    if (u.dz) 
    {
        if (useeit)
            You("reflect the %s.",
                (u.dz > 0) ? surface(u.ux, u.uy) : ceiling(u.ux, u.uy));
        return 1;
    }
    mtmp = bhit(u.dx, u.dy, COLNO, 0, INVIS_BEAM,
                (int FDECL((*), (MONST_P, OBJ_P, MONST_P))) 0,
                (int FDECL((*), (OBJ_P, OBJ_P, MONST_P))) 0, 
                (int FDECL((*), (TRAP_P, OBJ_P, MONST_P))) 0, 
                &obj, &youmonst, TRUE, FALSE);
    if (!mtmp || !haseyes(mtmp->data) || notonhead)
        return 1;

    /* couldsee(mtmp->mx, mtmp->my) is implied by the fact that bhit()
       targetted it, so we can ignore possibility of X-ray vision */
    vis = canseemon(mtmp);
/* ways to directly see monster (excludes X-ray vision, telepathy,
   extended detection, type-specific warning) */
#define SEENMON (MONSEEN_NORMAL | MONSEEN_SEEINVIS | MONSEEN_INFRAVIS)
    how_seen = vis ? howmonseen(mtmp) : 0;
    /* whether monster is able to use its vision-based capabilities */
    monable = !is_cancelled(mtmp) && (!is_invisible(mtmp) || has_see_invisible(mtmp));
    mlet = mtmp->data->mlet;
    if (is_sleeping(mtmp)) 
    {
        if (vis)
            pline("%s is too tired to look at your %s.", Monnam(mtmp),
                  mirror);
    }
    else if (is_blinded(mtmp))
    {
        if (vis)
            pline("%s can't see anything right now.", Monnam(mtmp));
    } 
    else if (invis_mirror && !has_see_invisible(mtmp)) 
    {
        if (vis)
            pline("%s fails to notice your %s.", Monnam(mtmp), mirror);
        /* infravision doesn't produce an image in the mirror */
    } 
    else if ((how_seen & SEENMON) == MONSEEN_INFRAVIS)
    {
        if (vis) /* (redundant) */
            pline("%s is too far away to see %sself in the dark.",
                  Monnam(mtmp), mhim(mtmp));
        /* some monsters do special things */
    }
    else if (mlet == S_VAMPIRE || mlet == S_GHOST || is_vampshifter(mtmp)) 
    {
        if (vis)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s doesn't have a reflection.", Monnam(mtmp));
    }
    else if (monable && is_medusa(mtmp->data))
    {
        if (mon_reflects(mtmp, "The gaze is reflected away by %s %s!"))
        {
            play_sfx_sound_at_location(SFX_GENERAL_REFLECTS, mtmp->mx, mtmp->my);
            return 1;
        }
        if (vis)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is turned to stone!", Monnam(mtmp));

        play_sfx_sound_at_location(SFX_PETRIFY, mtmp->mx, mtmp->my);
        killed_by_stoning(mtmp);
    } 
    else if (monable && mtmp->data == &mons[PM_FLOATING_EYE]) 
    {
        int tmp = d((int) mtmp->m_lev, (int) mtmp->data->mattk[0].damd);
        if (!rn2(4))
            tmp = 120;
        if (vis)
            pline("%s is frozen by its reflection.", Monnam(mtmp));
        else
            You_hear("%s stop moving.", something);
        paralyze_monst(mtmp, (int) mtmp->mfrozen + tmp, FALSE);
    } 
    else if (monable && mtmp->data == &mons[PM_UMBRAL_HULK]) 
    {
        if (vis)
            pline("%s confuses itself!", Monnam(mtmp));
        nonadditive_increase_mon_property(mtmp, CONFUSION, 10 + rnd(10));
    } 
    else if (monable && (mlet == S_NYMPH || mtmp->data == &mons[PM_INCUBUS])) 
    {
        if (vis) 
        {
            char buf[BUFSZ]; /* "She" or "He" */

            pline("%s admires %sself in your %s.", Monnam(mtmp), mhim(mtmp),
                  mirror);
            pline("%s takes it!", upstart(strcpy(buf, mhe(mtmp))));
        }
        else
            pline("It steals your %s!", mirror);

        setnotworn(obj); /* in case mirror was wielded */
        freeinv(obj);
        (void) mpickobj(mtmp, obj);
        if (!tele_restrict(mtmp))
        {
            (void)rloc2(mtmp, TRUE, TRUE);
        }
    }
    else if (!is_unicorn(mtmp->data) && !humanoid(mtmp->data)
               && (!is_invisible(mtmp) || has_see_invisible(mtmp)) && rn2(5)) 
    {
        boolean do_react = TRUE;

        if (mtmp->mfrozen) 
        {
            if (vis)
                You("discern no obvious reaction from %s.", mon_nam(mtmp));
            else
                You_feel("a bit silly gesturing the mirror in that direction.");
            do_react = FALSE;
        }

        if (do_react)
        {
            if (vis)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is frightened by its reflection.", Monnam(mtmp));

            play_sfx_sound_at_location(SFX_ACQUIRE_FEAR, mtmp->mx, mtmp->my);
            monflee(mtmp, d(2, 4), FALSE, FALSE);
        }
    }
    else if (!Blind)
    {
        if (is_invisible(mtmp) && !See_invisible)
            ;
        else if ((is_invisible(mtmp) && !has_see_invisible(mtmp))
                 /* redundant: can't get here if these are true */
                 || !haseyes(mtmp->data) || notonhead || is_blinded(mtmp))
            pline("%s doesn't seem to notice %s reflection.", Monnam(mtmp),
                  mhis(mtmp));
        else
            pline("%s ignores %s reflection.", Monnam(mtmp), mhis(mtmp));
    }
    return 1;
}

STATIC_OVL int
use_holysymbol(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    if (!getdir((char*)0))
        return 0;

    update_u_facing(TRUE);
    u.uconduct.gnostic++;

    if (obj->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "raise %s high, but nothing happens.", yname(obj));
        return 0;
    }

    consume_obj_charge(obj, TRUE);

    if ((u.ualign.type != A_CHAOTIC
        && (is_demon(youmonst.data) || is_undead(youmonst.data)))
        || u.ugangr > 6 || Inhell) 
    { 
        play_sfx_sound(SFX_FAIL_TO_CAST_CORRECTLY);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "raise %s high, but nothing happens.", yname(obj));
        exercise(A_WIS, FALSE);
        return 0;
    }

    play_simple_player_sound(MONSTER_SOUND_TYPE_CAST);
    You_ex(ATR_NONE, CLR_MSG_HINT, "raise %s high.", yname(obj));
    exercise(A_WIS, TRUE);
    (void)bhit(u.dx, u.dy, obj->blessed ? 4 : 3, 0, ZAPPED_WAND, uthitm, uthito, uthitt,
        &obj, &youmonst, TRUE, FALSE);

    return 1;
}

/* Routines for IMMEDIATE wands and spells. */
/* bhitm: monster mtmp was hit by the effect of wand or spell otmp */
int
uthitm(mtmp, otmp, origmonst)
struct monst* mtmp;
struct obj* otmp;
struct monst* origmonst;
{
    boolean wake = TRUE; /* Most 'zaps' should wake monster */
    boolean reveal_invis = FALSE, learn_it = FALSE;
    boolean helpful_gesture = FALSE;

    if (!otmp || !mtmp)
        return 0;

    if (origmonst) /* Remove gcc warning */
    {
        /* Do nothing */
    }

    if (u.uswallow && mtmp == u.ustuck)
        reveal_invis = FALSE;

    wake = FALSE;

    if (is_undead(mtmp->data) || is_vampshifter(mtmp))  // || is_demon(mtmp->data)
    {

        reveal_invis = TRUE;
        wake = TRUE;

#if 0
        if(otmp->blessed)
            dmg = d(2,6) + Role_if(PM_PRIEST) ? u.ulevel / 2 : 0;
        else
            dmg = d(1,6) + Role_if(PM_PRIEST) ? u.ulevel / 2 : 0;
        context.bypasses = TRUE; /* for make_corpse() */

        //Check if successful, priests have +5 levels to the use of holy symbol, Demons are 5 levels more difficult to turn
        int percentchance = (Role_if(PM_PRIEST) ? 50: 0) + (otmp->blessed ? 20 : 0) + (u.ulevel - mtmp->m_lev - (is_demon(mtmp->data) ? 5 : 0)) * 10;

        if (is_dlord(mtmp->data) || is_dprince(mtmp->data))
            percentchance = 0;
#endif
        int chance = 10 * (10 + u.ulevel - mtmp->m_lev + (otmp->blessed ? 5 : 0) + Luck);
        boolean turn_success = rn2(100) < chance;
        if (!(mtmp->data->geno & G_UNIQ) && turn_success)
        {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s brightly before %s!", Yobjnam2(otmp, "shine"), mon_nam(mtmp));
            if (!DEADMONSTER(mtmp))
            {
                play_sfx_sound_at_location(SFX_ACQUIRE_FEAR, mtmp->mx, mtmp->my);
                monflee(mtmp, 200 + rnd(100), FALSE, TRUE);
            }
#if 0
            if (!otmp->cursed)
            {
                pline("%s shines brightly before %s!", Yname2(otmp), the(mon_nam(mtmp)));
                if (!check_magic_resistance_and_inflict_damage(mtmp, otmp, TRUE, dmg, AD_CLRC, TELL))
                {
                    if (!DEADMONSTER(mtmp))
                        monflee(mtmp, 200 + rnd(100), FALSE, TRUE);
                }
            }
            else
            {
                if (!check_magic_resistance_and_inflict_damage(mtmp, otmp, FALSE, 0, 0, TELL) && !is_dlord(mtmp->data) && !is_dprince(mtmp->data) && !is_tame(mtmp))
                {
                    if (mtmp->m_lev <= 10 && mtmp->m_lev < u.ulevel && rn2(100) < (percentchance - 100))
                    {
                        You("successfully take control of %s.", the(mon_nam(mtmp)));
                        (void)tamedog(mtmp, (struct obj*) 0, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, FALSE);
                    }
                    else
                    {
                        if (!is_peaceful(mtmp))
                        {
                            You("successfully pacify %s.", the(mon_nam(mtmp)));
                            mtmp->mpeaceful = 1;
                        }
                    }
                }
            }
#endif
        }
        else
        {
            if (canseemon(mtmp))
            {
                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_LAUGHTER);
                if (!Deaf)
                    pline("%s laughs at your feeble attempt.", Monnam(mtmp));
                else
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "fail to turn %s.", mon_nam(mtmp));
            }
        }
    }
    else if (is_demon(mtmp->data))
    {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s no effect on %s.", Yobjnam2(otmp, "have"), mon_nam(mtmp));
    }
    else
    {
        pline("%s seems uninterested in %s.", Monnam(mtmp), yname(otmp));
    }


    if (wake) {
        if (!DEADMONSTER(mtmp)) {
            wakeup(mtmp, helpful_gesture ? FALSE : TRUE);
            m_respond(mtmp);
            if (mtmp->isshk && !*u.ushops)
                hot_pursuit(mtmp);
        }
        else if (M_AP_TYPE(mtmp))
            seemimic(mtmp); /* might unblock if mimicing a boulder/door */
    }
    /* note: bhitpos won't be set if swallowed, but that's okay since
     * reveal_invis will be false.  We can't use mtmp->mx, my since it
     * might be an invisible worm hit on the tail.
     */
    if (reveal_invis) {
        if (!DEADMONSTER(mtmp) && cansee(bhitpos.x, bhitpos.y)
            && !canspotmon(mtmp))
            map_invisible(bhitpos.x, bhitpos.y);
    }
    /* if effect was observable then discover the wand type provided
       that the wand itself has been seen */
    if (learn_it)
        learnwand(otmp);
    return 0;
}

int
uthito(obj, otmp, origmonst)
struct obj* obj, * otmp;
struct monst* origmonst;
{
    int res = 0; /* did not affect object by default */
    if(origmonst) /* Remove gcc warning */
    {
        /* Do nothing */
    }

    if (obj == otmp)
        return 0;

    if (obj->bypass) {
        /* The bypass bit is currently only used as follows:
         *
         * UNDEAD_TURNING - When an undead creature gets killed via
         *             undead turning, prevent its corpse from being
         *             immediately revived by the same effect.
         *
         * The bypass bit on all objects is reset each turn, whenever
         * context.bypasses is set.
         *
         * We check the obj->bypass bit above AND context.bypasses
         * as a safeguard against any stray occurrence left in an obj
         * struct someplace, although that should never happen.
         */
        if (context.bypasses) {
            return 0;
        }
        else {
            debugpline1("%s for a moment.", Tobjnam(obj, "pulsate"));
            obj->bypass = 0;
        }
    }

    /*
     * Some parts of this function expect the object to be on the floor
     * obj->{ox,oy} to be valid.  The exception to this (so far) is
     * for the STONE_TO_FLESH spell.
     */
    if (!(obj->where == OBJ_FLOOR))
        impossible("bhito: obj is not floor or Stone To Flesh spell");

    //Do nothing, holy symbol does not revive corpses

    return res;
}

int
uthitt(t, otmp, origmonst)
struct trap* t UNUSED;
struct obj* otmp UNUSED;
struct monst* origmonst UNUSED;
{
    return 0;
}

STATIC_OVL void
use_bell(optr)
struct obj **optr;
{
    register struct obj *obj = *optr;
    struct monst *mtmp;
    boolean wakem = FALSE, learno = FALSE,
            ordinary = (obj->otyp != BELL_OF_OPENING || !obj->charges),
            invoking =
                (obj->otyp == BELL_OF_OPENING && invocation_pos(u.ux, u.uy)
                 && !On_stairs(u.ux, u.uy));

    You("ring %s.", the(xname(obj)));

    if (Underwater || (u.uswallow && ordinary)) 
    {
#ifdef AMIGA
        amii_speaker(obj, "AhDhGqEqDhEhAqDqFhGw", AMII_MUFFLED_VOLUME);
#else
        play_sfx_sound(SFX_MUFFLED_BELL_RING);
#endif
        pline("But the sound is muffled.");

    } 
    else if (invoking && ordinary)
    {
        /* needs to be recharged... */
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "But it makes no sound.");
        learno = TRUE; /* help player figure out why */

    } 
    else if (ordinary)
    {
#ifdef AMIGA
        amii_speaker(obj, "ahdhgqeqdhehaqdqfhgw", AMII_MUFFLED_VOLUME);
#endif
        if (iflags.using_gui_sounds)
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
            delay_output_milliseconds(1000);
        }
        if (obj->cursed && !rn2(4)
            /* note: once any of them are gone, we stop all of them */
            && !(mvitals[PM_WOOD_NYMPH].mvflags & MV_GONE)
            && !(mvitals[PM_WATER_NYMPH].mvflags & MV_GONE)
            && !(mvitals[PM_MOUNTAIN_NYMPH].mvflags & MV_GONE)
            && (mtmp = makemon(mkclass(S_NYMPH, 0), u.ux, u.uy, MM_NO_MONSTER_INVENTORY))
                   != 0) {
            You_ex(ATR_NONE, CLR_MSG_WARNING, "summon %s!", a_monnam(mtmp));
            if (!obj_resists(obj, 93, 100)) {
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s shattered!", Tobjnam(obj, "have"));
                useup(obj);
                *optr = 0;
            } else
                switch (rn2(3)) {
                default:
                    break;
                case 1:
                    mtmp->mprops[VERY_FAST] |= M_INTRINSIC_ACQUIRED;
                    break;
                case 2: /* no explanation; it just happens... */
                    nomovemsg = "";
                    multi_reason = NULL;
                    nomul(-rnd(2));
                    break;
                }
        }
        wakem = TRUE;

    } else {
        /* charged Bell of Opening */
        consume_obj_charge(obj, TRUE);
        if (iflags.using_gui_sounds)
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
            delay_output_milliseconds(750);
        }

        if (u.uswallow) 
        {
            if (!obj->cursed)
                (void) openit();
            else
                pline1(nothing_happens);

        } 
        else if (obj->cursed)
        {
            play_sfx_sound(SFX_CURSED_BELL_OF_OPENING_EFFECT);
            coord mm;

            mm.x = u.ux;
            mm.y = u.uy;
            mkundead(&mm, FALSE, MM_NO_MONSTER_INVENTORY | MM_PLAY_SUMMON_ANIMATION | MM_UNDEAD_SUMMON_ANIMATION);
            wakem = TRUE;

        } 
        else if (invoking) 
        {
#ifdef AMIGA
            amii_speaker(obj, "aefeaefeaefeaefeaefe", AMII_LOUDER_VOLUME);
#else
            play_sfx_sound(SFX_BELL_OF_OPENING_UNSETTLING_SHRILL_SOUND);
#endif
            pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s an unsettling shrill sound...", Tobjnam(obj, "issue"));
            obj->age = moves;
            learno = TRUE;
            wakem = TRUE;

        } 
        else if (obj->blessed) 
        {
            int res = 0;

#ifdef AMIGA
            amii_speaker(obj, "ahahahDhEhCw", AMII_SOFT_VOLUME);
#else
            play_sfx_sound(SFX_BLESSED_BELL_OF_OPENING_EFFECT);
#endif
            if (uchain)
            {
                unpunish();
                res = 1;
            }
            else if (u.utrap && u.utraptype == TT_BURIEDBALL) 
            {
                buried_ball_to_freedom();
                res = 1;
            }
            res += openit();
            switch (res)
            {
            case 0:
                pline1(nothing_happens);
                break;
            case 1:
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s opens...", Something);
                learno = TRUE;
                break;
            default:
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Things open around you...");
                learno = TRUE;
                break;
            }

        } 
        else 
        { /* uncursed */
#ifdef AMIGA
            amii_speaker(obj, "AeFeaeFeAefegw", AMII_OKAY_VOLUME);
#else
            play_sfx_sound(SFX_UNCURSED_BELL_OF_OPENING_EFFECT);
#endif
            if (findit() != 0)
                learno = TRUE;
            else
                pline1(nothing_happens);
        }

    } /* charged BofO */

    if (learno)
    {
        makeknown(BELL_OF_OPENING);
        obj->known = 1;
    }
    if (wakem)
        wake_nearby();
}

void
use_candelabrum(obj)
register struct obj *obj;
{
    const char *s = (obj->special_quality != 1) ? "candles" : "candle";

    if (obj->lamplit)
    {
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY2);
        You("snuff the %s.", s);
        Strcpy(debug_buf_3, "use_candelabrum");
        end_burn(obj, TRUE);
        return;
    }
    if (obj->special_quality <= 0) 
    {
        play_sfx_sound(SFX_GENERAL_NOT_IN_THE_RIGHT_CONDITION);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "This %s has no %s.", xname(obj), s);
        return;
    }
    if (Underwater) 
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot make fire under water.");
        return;
    }
    if (u.uswallow || obj->cursed) 
    {
        if (!Blind)
        {
            play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s for a moment, then %s.", s, vtense(s, "flicker"),
                vtense(s, "die"));

        }
        return;
    }


    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
    short max_candles = objects[obj->otyp].oc_special_quality;
    
    if (obj->special_quality < max_candles) 
    {
        There_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s only %d %s in %s.", vtense(s, "are"), obj->special_quality, s,
              the(xname(obj)));
        if (!Blind)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s lit.  %s dimly.", obj->special_quality == 1 ? "It is" : "They are",
                  Tobjnam(obj, "shine"));
    }
    else 
    {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s's %s burn%s", The(xname(obj)), s,
              (Blind ? "." : " brightly!"));
    }

    if (obj->otyp != CANDELABRUM_OF_INVOCATION || !invocation_pos(u.ux, u.uy) || On_stairs(u.ux, u.uy)) 
    {
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s being rapidly consumed!", s, vtense(s, "are"));
        /* this used to be obj->age /= 2, rounding down; an age of
           1 would yield 0, confusing begin_burn() and producing an
           unlightable, unrefillable candelabrum; round up instead */
        obj->age = (obj->age + 1L) / 2L;
    } 
    else 
    {
        if (obj->special_quality == max_candles) 
        {
            if (Blind)
                pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s a strange warmth!", Tobjnam(obj, "radiate"));
            else
                pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "%s with a strange light!", Tobjnam(obj, "glow"));
        }
        obj->known = 1;
    }
    begin_burn(obj, FALSE);
}

int
use_candle(optr)
struct obj **optr;
{
    register struct obj *obj = *optr;
    register struct obj *otmp;
    const char *s = (obj->quan != 1) ? "candles" : "candle";
    char qbuf[QBUFSZ], qsfx[QBUFSZ], *q;
    const char tools[] = { TOOL_CLASS, 0 };

    if (u.uswallow)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_ex(ATR_NONE, CLR_MSG_FAIL, no_elbow_room);
        return 1;
    }

    int candelabrum_cnt = 0;
    struct obj* candelabrum = 0;
    for (otmp = invent; otmp; otmp = otmp->nobj)
    {
        if (is_obj_candelabrum(otmp) && otmp->special_quality < objects[otmp->otyp].oc_special_quality)
        {
            candelabrum_cnt++;
            candelabrum = otmp;
        }
    }

    int floor_cnt = 0;
    struct obj* floor_candelabrum = 0;
    for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
    {
        if (is_obj_candelabrum(otmp) && otmp->special_quality < objects[otmp->otyp].oc_special_quality)
        {
            floor_cnt++;
            floor_candelabrum = otmp;
            if(candelabrum_cnt == 0)
                candelabrum = otmp;
        }
    }

    char ans = 'n';
    if (candelabrum_cnt > 0 || floor_cnt > 0)
    {
        char attachbuf[BUFSZ];
        Sprintf(attachbuf, "Do you want to attach the %s to %s?", cxname(obj),
            candelabrum_cnt == 1 && floor_cnt == 0 && candelabrum ? yname(candelabrum) :
            candelabrum_cnt == 0 && floor_cnt == 1 && floor_candelabrum ? "the candelabrum on the floor here" : 
            "a candelabrum");

        ans = ynq(attachbuf);
        if (ans == 'q')
        {
            pline1(Never_mind);
            return 0;
        }
    }

    otmp = candelabrum;
    //otmp = carrying(CANDELABRUM_OF_INVOCATION);

    if (ans != 'y')
    {
        boolean objsplitted = FALSE;
        struct obj* lightedcandle = (struct obj*)0;
        char qbuf2[QBUFSZ];
        Sprintf(qbuf2, "%s only one?", obj->lamplit ? "Snuff out" : "Light");

        if (obj->quan > 1L && yn_query(qbuf2) == 'y') 
        {
            objsplitted = TRUE;
            if (!carried(obj))
            {
                (void)splitobj(obj, obj->quan - 1);
                lightedcandle = obj;
            }
            else
                lightedcandle = splitobj(obj, 1);
            debugpline0("split object,");
        }
        else
            lightedcandle = obj;

        if (lightedcandle)
        {
            use_lamp(lightedcandle);
        }

        if (lightedcandle && carried(lightedcandle) && objsplitted)
        {
            freeinv(lightedcandle);
            if (inv_cnt(FALSE) >= 52)
            {
                pline("Oops! %s from your %s.", Tobjnam(lightedcandle, "slip"), body_part(HAND));
                sellobj_state(SELL_DONTSELL);
                dropyf(lightedcandle);
                sellobj_state(SELL_NORMAL);
            }
            else
            {
                lightedcandle->nomerge = 1; /* used to prevent merge */
                lightedcandle = addinv(lightedcandle);
                lightedcandle->nomerge = 0;
            }
        }

        return 1;
    }

    ans = 'n';
    if (candelabrum_cnt > 0 && floor_cnt > 0)
    {
        char attachbuf[BUFSZ * 2] = "";
        Sprintf(attachbuf, "There is %s on the floor. Attach the candles to %s?", floor_cnt > 1 ? "candelabra" : "a candelabrum", floor_cnt > 1 ? "one of them" : "it");
        ans = yn_query(attachbuf);
    }

    if (ans == 'y' && floor_cnt > 0)
    {
        if (floor_cnt == 1 && floor_candelabrum)
        {
            otmp = floor_candelabrum;
        }
        else
        {
            int n;
            winid win;
            anything any;
            menu_item* pick_list = (menu_item*)0;

            any.a_void = 0;
            win = create_nhwindow(NHW_MENU);
            start_menu_ex(win, GHMENU_STYLE_PICK_ITEM_LIST_AUTO_OK);

            for (struct obj* cobj = level.objects[u.ux][u.ux]; cobj; cobj = cobj->nexthere)
                if (is_obj_candelabrum(cobj)) 
                {
                    any.a_obj = cobj;
                    add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                        doname(cobj), MENU_UNSELECTED);
                }
            end_menu(win, "Attach candles to which candelabrum?");
            n = select_menu(win, PICK_ONE, &pick_list);
            destroy_nhwindow(win);

            if (n > 0) 
            {
                otmp = pick_list[0].item.a_obj;
                free((genericptr_t)pick_list);
            }
            else
            {
                pline1(Never_mind);
                return 0;
            }
        }
    }
    else
    {
        if (candelabrum_cnt > 1)
        {
            otmp = getobj(tools, "attach candles to", 0, "");
            if (!otmp)
            {
                pline1(Never_mind);
                return 0;
            }
        }

        if (candelabrum_cnt + floor_cnt > 1)
        {
            /* first, minimal candelabrum suffix for formatting candles */
            Sprintf(qsfx, " to\033%s?", thesimpleoname(otmp));
            /* next, format the candles as a prefix for the candelabrum */
            (void)safe_qbuf(qbuf, "Attach ", qsfx, obj, yname, thesimpleoname, s);
            /* strip temporary candelabrum suffix */
            if ((q = strstri(qbuf, " to\033")) != 0)
                Strcpy(q, " to ");

            if (yn_query(safe_qbuf(qbuf, qbuf, "?", otmp, yname, thesimpleoname, "it"))
                == 'n')
            {
                return 0;
#if 0
                use_lamp(obj);
                return 1;
#endif
            }
        }
    }

    if (!otmp)
    {
        pline1(Never_mind);
        return 0;
    }

    if (!is_obj_candelabrum(otmp))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot attach candles to %s.", acxname(otmp));
        return 1;
    }

    int64_t max_candles = (int64_t)objects[otmp->otyp].oc_special_quality;
    if (otmp->special_quality >= max_candles)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is already full of candles.", The(cxname(otmp)));
        return 1;
    }

    if ((int64_t) otmp->special_quality + obj->quan > max_candles)
    {
        obj = splitobj(obj, max_candles - (int64_t) otmp->special_quality);
        /* avoid a grammatical error if obj->quan gets
            reduced to 1 candle from more than one */
        s = (obj->quan != 1) ? "candles" : "candle";
    } 
    else
        *optr = 0;

    play_sfx_sound(SFX_ATTACH_CANDLE);
    You("attach %ld%s %s to %s.", obj->quan, !otmp->special_quality ? "" : " more", s,
        the(xname(otmp)));

    if (!otmp->special_quality || otmp->age > obj->age)
        otmp->age = obj->age;

    otmp->special_quality += (int) obj->quan;

    if (otmp->lamplit && !obj->lamplit)
        pline_The("new %s magically %s!", s, vtense(s, "ignite"));
    else if (!otmp->lamplit && obj->lamplit)
        pline("%s out.", (obj->quan > 1L) ? "They go" : "It goes");

    if (is_unpaid_shop_item(obj, u.ux, u.uy))
    {
        char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
        struct monst* shkp = shop_keeper(*o_shop);
        if (shkp && inhishop(shkp) && (obj->where == OBJ_FLOOR || is_obj_on_shk_bill(obj, shkp)))
        {
            play_voice_shopkeeper_simple_line(shkp, otmp->lamplit ? ((obj->quan > 1L) ? SHOPKEEPER_LINE_BURN_THEM_BOUGHT_THEM : SHOPKEEPER_LINE_BURN_IT_BOUGHT_IT) :
                ((obj->quan > 1L) ? SHOPKEEPER_LINE_USE_THEM_BOUGHT_THEM : SHOPKEEPER_LINE_USE_IT_BOUGHT_IT));
        }
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "You %s %s, you bought %s!",
            otmp->lamplit ? "burn" : "use",
            (obj->quan > 1L) ? "them" : "it",
            (obj->quan > 1L) ? "them" : "it");
    }
    if (obj->quan < max_candles && otmp->special_quality == max_candles)
        pline("%s now has %ld%s candles attached.", The(xname(otmp)), max_candles,
                otmp->lamplit ? " lit" : "");
        
    /* candelabrum's light range might increase */
    if (otmp->lamplit)
        obj_merge_light_sources(otmp, otmp);

    Strcpy(debug_buf_3, "use_candle");
    /* candles are no longer a separate light source */
    if (obj->lamplit)
        end_burn(obj, TRUE);

    /* candles are now gone */
    useupall(obj);
    /* candelabrum's weight is changing */
    otmp->owt = weight(otmp);
    update_inventory();

    return 1;
}

/* call in drop, throw, and put in box, etc. */
boolean
snuff_candle(otmp)
struct obj *otmp;
{
    boolean candle = is_candle(otmp);

    if ((candle || is_obj_candelabrum(otmp))
        && otmp->lamplit) 
    {
        char buf[BUFSZ];
        xchar x, y;
        boolean many = candle ? (otmp->quan > 1L) : (otmp->special_quality > 1);

        (void) get_obj_location(otmp, &x, &y, 0);
        if (otmp->where == OBJ_MINVENT ? cansee(x, y) : !Blind)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%scandle%s flame%s extinguished.", Shk_Your(buf, otmp),
                  (candle ? "" : "candelabrum's "), (many ? "s'" : "'s"),
                  (many ? "s are" : " is"));
        Strcpy(debug_buf_3, "snuff_candle");
        end_burn(otmp, TRUE);
        return TRUE;
    }
    return FALSE;
}

int
use_torch(optr)
struct obj** optr;
{
    register struct obj* obj = *optr;

    if (u.uswallow)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_ex(ATR_NONE, CLR_MSG_FAIL, no_elbow_room);
        return 1;
    }

    boolean objsplitted = FALSE;
    struct obj* lightedcandle = (struct obj*)0;

    if (obj->quan > 1L)
    {
        objsplitted = TRUE;
        if (!carried(obj))
        {
            (void)splitobj(obj, obj->quan - 1);
            lightedcandle = obj;
        }
        else
            lightedcandle = splitobj(obj, 1);
        debugpline0("split object,");
    }
    else
        lightedcandle = obj;

    if (lightedcandle)
    {
        use_lamp(lightedcandle);
    }

    if (lightedcandle && carried(lightedcandle) && objsplitted)
    {
        freeinv(lightedcandle);
        if (inv_cnt(FALSE) >= 52)
        {
            pline("Oops! %s from your %s.", Tobjnam(lightedcandle, "slip"), body_part(HAND));
            sellobj_state(SELL_DONTSELL);
            dropyf(lightedcandle);
            sellobj_state(SELL_NORMAL);
        }
        else
        {
            lightedcandle->nomerge = 1; /* used to prevent merge */
            lightedcandle = addinv(lightedcandle);
            lightedcandle->nomerge = 0;
        }
    }

    update_inventory();
    return 1;
}

/* call in drop, throw, and put in box, etc. */
boolean
snuff_torch(otmp)
struct obj* otmp;
{
    boolean istorch = is_torch(otmp);

    if (istorch && otmp->lamplit)
    {
        char buf[BUFSZ];
        xchar x, y;
        boolean many = otmp->quan > 1L;

        (void)get_obj_location(otmp, &x, &y, 0);
        if (otmp->where == OBJ_MINVENT ? cansee(x, y) : !Blind)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%storch%s flame%s extinguished.", Shk_Your(buf, otmp),
                (many ? "es'" : "'s"), (many ? "s are" : " is"));
        Strcpy(debug_buf_3, "snuff_torch");
        end_burn(otmp, TRUE);
        return TRUE;
    }
    return FALSE;
}

/* called when lit lamp is hit by water or put into a container or
   you've been swallowed by a monster; obj might be in transit while
   being thrown or dropped so don't assume that its location is valid */
boolean
snuff_lit(obj)
struct obj *obj;
{
    xchar x, y;

    if (obj->lamplit) 
    {
        if (is_lamp(obj) || obj->otyp == POT_OIL) 
        {
            (void) get_obj_location(obj, &x, &y, 0);
            if (obj->where == OBJ_MINVENT ? cansee(x, y) : !Blind)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s out!", Yname2(obj), otense(obj, "go"));
            Strcpy(debug_buf_3, "snuff_lit");
            end_burn(obj, TRUE);
            return TRUE;
        }
        else if (is_torch(obj))
        {
            if (snuff_torch(obj))
                return TRUE;
        }
        else
        {
            if (snuff_candle(obj))
                return TRUE;
        }
    }
    return FALSE;
}

/* Called when potentially lightable object is affected by fire_damage().
   Return TRUE if object was lit and FALSE otherwise --ALI */
boolean
catch_lit(obj)
struct obj *obj;
{
    xchar x, y;

    if (!obj->lamplit && is_obj_ignitable(obj))
    {
        if (obj->otyp == MAGIC_LAMP && obj->special_quality == 0)
            return FALSE;
        if ((obj->otyp == MAGIC_CANDLE || is_obj_candelabrum(obj)) && obj->special_quality == 0)
            return FALSE;
        else if (obj->otyp != MAGIC_LAMP && obj->otyp != MAGIC_CANDLE && obj->age == 0)
            return FALSE;
        if (!get_obj_location(obj, &x, &y, 0))
            return FALSE;
        if (is_obj_candelabrum(obj) && obj->cursed)
            return FALSE;
        if (is_lamp(obj) && obj->cursed && !rn2(2))
            return FALSE;
        if (obj->where == OBJ_MINVENT ? cansee(x, y) : !Blind)
            pline("%s %s light!", Yname2(obj), otense(obj, "catch"));
        if (obj->otyp == POT_OIL)
            makeknown(obj->otyp);
        if (carried(obj) && obj->unpaid && costly_spot(u.ux, u.uy)) {
            /* if it catches while you have it, then it's your tough luck */
            check_unpaid(obj);
            if (iflags.using_gui_sounds)
            {
                char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
                struct monst* shkp = shop_keeper(*o_shop);
                if (shkp && inhishop(shkp) && is_obj_on_shk_bill(obj, shkp))
                {
                    play_voice_shopkeeper_simple_line(shkp, obj->quan == 1L ? SHOPKEEPER_LINE_IN_ADDITION_TO_COST_OF_ITEM_ITSELF : SHOPKEEPER_LINE_IN_ADDITION_TO_COST_OF_ITEMS_THEMSELVES);
                    verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "That's in addition to the cost of %s %s, of course.",
                        obj->quan == 1L ? "the item" : "the items", obj->quan == 1L ? "itself" : "themselves");
                }
            }
            else
            {
                verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "That's in addition to the cost of %s %s, of course.",
                    yname(obj), obj->quan == 1L ? "itself" : "themselves");
            }
            bill_dummy_object(obj);
        }
        begin_burn(obj, FALSE);
        return TRUE;
    }
    return FALSE;
}

void
use_lamp(obj)
struct obj *obj;
{
    char buf[BUFSZ];

    if (obj->lamplit) 
    {
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY2);
        if (is_lamp(obj))
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%slamp is now off.", Shk_Your(buf, obj));
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "snuff out %s.", yname(obj));
        }
        Strcpy(debug_buf_3, "use_lamp");
        end_burn(obj, TRUE);
        return;
    }

    if (Underwater)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, !is_candle_or_torch(obj) ? "This is not a diving lamp"
                              : "Sorry, fire and water don't mix.");
        return;
    }

    /* magic lamps with an enchantment == 0 (wished for) cannot be lit */
    if ((!is_candle_or_torch(obj) && obj->age == 0)
        || (obj->otyp == MAGIC_LAMP && obj->special_quality == 0))
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        if (obj->otyp == BRASS_LANTERN)
            Your_ex(ATR_NONE, CLR_MSG_FAIL, "lamp has run out of power.");
        else
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "This %s has no oil.", xname(obj));
        return;
    }
    if ((obj->cursed && !rn2(2)) || (obj->otyp == MAGIC_CANDLE && obj->special_quality == 0))
    {
        play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
        if (!Blind)
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s for a moment, then %s.", Tobjnam(obj, "flicker"),
                  otense(obj, "die"));
    } 
    else
    {
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
        if (is_lamp(obj))
        {
            check_unpaid(obj);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%slamp is now on.", Shk_Your(buf, obj));
        } 
        else 
        { /* candle(s) */
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flame%s %s%s", s_suffix(Yname2(obj)), plur(obj->quan),
                  otense(obj, "burn"), Blind ? "." : " brightly!");
            if (is_unpaid_shop_item(obj, u.ux, u.uy)
                && (obj->age == 30L * (int64_t) objects[obj->otyp].oc_cost || obj->otyp == MAGIC_CANDLE)) 
            {
                const char *ithem = (obj->quan > 1L) ? "them" : "it";

                char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
                struct monst* shkp = shop_keeper(*o_shop);
                if (shkp && inhishop(shkp) && (obj->where == OBJ_FLOOR || is_obj_on_shk_bill(obj, shkp)))
                {
                    play_voice_shopkeeper_simple_line(shkp, (obj->quan > 1L) ? SHOPKEEPER_LINE_BURN_THEM_BOUGHT_THEM : SHOPKEEPER_LINE_BURN_IT_BOUGHT_IT);
                }
                verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "You burn %s, you bought %s!", ithem, ithem);
                bill_dummy_object(obj);
            }
        }
        begin_burn(obj, FALSE);
    }
}

int
use_oil(obj)
struct obj* obj;
{
    if (!obj || obj->otyp != POT_OIL)
        return 0;

    if (obj->lamplit)
    {
        light_cocktail(&obj);
        return 1;
    }
    else
    {
        if (!objects[obj->otyp].oc_name_known)
        {
            makeknown(obj->otyp);
            pline1("This is a potion of oil.");
        }

        winid tmpwin = create_nhwindow(NHW_MENU);
        anything any;
        char buf[BUFSZ];
        menu_item* selected;

        any = zeroany; /* set all bits to zero */
        start_menu_ex(tmpwin, GHMENU_STYLE_CHOOSE_COMMAND);

        any.a_int = 1; /* use index+1 (cant use 0) as identifier */
        Sprintf(buf, "light %s up", obj->quan == 1 ? "it" : "them");
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
            MENU_UNSELECTED);
        any.a_int = 2;
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "refill an item with oil",
            MENU_UNSELECTED);
        Sprintf(buf, "What do you want to do with %s?", the(cxname(obj)));
        end_menu(tmpwin, buf);
        int menures = 0;
        if (select_menu(tmpwin, PICK_ONE, &selected) > 0)
        {
            menures = selected[0].item.a_int;
        }
        free((genericptr_t)selected);
        destroy_nhwindow(tmpwin);

        switch (menures)
        {
        case 1:
            light_cocktail(&obj);
            return 1;
        case 2:
        {
            const char refill_lantern_objects[] = { ALL_CLASSES, TOOL_CLASS, 0 };
            struct obj* target_obj = getobj_ex(refill_lantern_objects, "refill", 0, "", maybe_refillable_with_oil);
            if (!target_obj)
            {
                pline1(Never_mind);
                return 0;
            }
            else if (!is_refillable_with_oil(target_obj))
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot refill %s with oil.", acxname(target_obj));
                return 0;
            }
            else
                return refill_obj_with_oil(target_obj, obj);
        }
        default:
            break;
        }
    }
    return 0;
}

void
light_cocktail(optr)
struct obj **optr;
{
    struct obj *obj = *optr; /* obj is a potion of oil */
    char buf[BUFSZ];
    boolean split1off;

    if (u.uswallow) 
    {
        You(no_elbow_room);
        return;
    }

    if (obj->lamplit) 
    {        
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY2);
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "snuff the lit potion.");
        Strcpy(debug_buf_3, "light_cocktail");
        end_burn(obj, TRUE);
        /*
         * Free & add to re-merge potion.  This will average the
         * age of the potions.  Not exactly the best solution,
         * but its easy.
         */
        if (carried(obj))
        {
            freeinv(obj);
            *optr = addinv(obj);
        }
        return;
    } else if (Underwater) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        There_ex(ATR_NONE, CLR_MSG_FAIL, "is not enough oxygen to sustain a fire.");
        return;
    }

    split1off = (obj->quan > 1L);
    if (split1off)
        obj = splitobj(obj, 1L);

    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);
    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "light %spotion.%s", shk_your(buf, obj),
        Blind ? "" : "  It gives off a dim light.");

    if (is_unpaid_shop_item(obj, u.ux, u.uy)) {
        /* Normally, we shouldn't both partially and fully charge
         * for an item, but (Yendorian Fuel) Taxes are inevitable...
         */
        check_unpaid(obj);
        char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
        struct monst* shkp = shop_keeper(*o_shop);
        if (shkp && inhishop(shkp) && (obj->where == OBJ_FLOOR || is_obj_on_shk_bill(obj, shkp)))
        {
            play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_IN_ADDITION_TO_COST_OF_POTION);
            verbalize_angry1("That's in addition to the cost of the potion, of course.");
        }
        bill_dummy_object(obj);
    }
    makeknown(obj->otyp);

    begin_burn(obj, FALSE); /* after shop billing */
    if (split1off && carried(obj)) 
    {
        Strcpy(debug_buf_2, "light_cocktail");
        obj_extract_self(obj); /* free from inv */
        obj->nomerge = 1;
        obj = hold_another_object(obj, "You drop %s!", doname(obj),
                                  (const char *) 0);
        if (obj)
            obj->nomerge = 0;
    }
    *optr = obj;
}

STATIC_VAR NEARDATA const char cuddly[] = { TOOL_CLASS, GEM_CLASS, 0 };

int
dorub(VOID_ARGS)
{
    struct obj *obj = getobj(cuddly, "rub", 0, "");

    if (obj && obj->oclass == GEM_CLASS) {
        if (is_graystone(obj)) {
            use_stone(obj);
            return 1;
        } else {
            play_sfx_sound(SFX_GENERAL_DO_NOT_KNOW_HOW);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "Sorry, I don't know how to use that.");
            return 0;
        }
    }

    if (!obj || !wield_tool(obj, "rub"))
        return 0;

    if (!uwep)
        return 0;

    You("start rubbing %s.", yname(uwep));

    if (iflags.using_gui_sounds && !Deaf)
    {
        play_sfx_sound(SFX_RUB);
        delay_output_milliseconds(1500);
    }

    /* now uwep is obj */
    if (uwep->otyp == MAGIC_LAMP) 
    {
        if ((uwep->special_quality == 1) && !rn2(3)) 
        {
            /* Note: save insurance even though djinni does not guarantee a wish; better than losing wish for sure, and abuse is equally possible by saving before starting rubbing */
            play_sfx_sound(SFX_VANISHES_IN_PUFF_OF_SMOKE);
            check_unpaid_usage(uwep, TRUE); /* unusual item use */
            djinni_from_bottle(uwep);
            makeknown(MAGIC_LAMP);
            update_inventory();
        }
        else if (rn2(2))
        {
            play_special_effect_at(SPECIAL_EFFECT_PUFF_OF_SMOKE, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_VANISHES_IN_PUFF_OF_SMOKE);
            special_effect_wait_until_action(0);
            You("%s smoke.", !Blind ? "see a puff of" : "smell");
            special_effect_wait_until_end(0);
        }
        else
            pline1(nothing_happens);
    } 
    else if (obj->otyp == BRASS_LANTERN) 
    {
        /* message from Adventure */
        pline("Rubbing the electric lamp is not particularly rewarding.");
        pline("Anyway, nothing exciting happens.");
    }
    else
        pline1(nothing_happens);
    return 1;
}

int
dojump(VOID_ARGS)
{
    /* Physical jump */
    return jump(0);
}

enum jump_trajectory {
    jAny  = 0, /* any direction => magical jump */
    jHorz = 1,
    jVert = 2,
    jDiag = 3  /* jHorz|jVert */
};

/* callback routine for walk_path() */
STATIC_PTR boolean
check_jump(arg, x, y)
genericptr arg;
int x, y;
{
    int traj = *(int *) arg;
    struct rm *lev = &levl[x][y];

    if (Passes_walls)
        return TRUE;
    if (IS_STWALL(lev->typ))
        return FALSE;
    if (IS_DOOR(lev->typ)) {
        if (closed_door(x, y))
            return FALSE;
        if ((lev->doormask & D_ISOPEN) != 0 && traj != jAny
            /* reject diagonal jump into or out-of or through open door */
            && (traj == jDiag
                /* reject horizontal jump through horizontal open door
                   and non-horizontal (ie, vertical) jump through
                   non-horizontal (vertical) open door */
                || ((traj & jHorz) != 0) == (lev->horizontal != 0)))
            return FALSE;
        /* empty doorways aren't restricted */
    }
    /* let giants jump over boulders (what about Flying?
       and is there really enough head room for giants to jump
       at all, let alone over something tall?) */
    if (sobj_at(BOULDER, x, y) && !throws_rocks(youmonst.data))
        return FALSE;
    return TRUE;
}

STATIC_OVL boolean
is_valid_jump_pos(x, y, magic, showmsg)
int x, y, magic;
boolean showmsg;
{
    if (!magic && Role_if(PM_KNIGHT) && !(HJumping & ~INTRINSIC) && !EJumping && distu(x, y) != 5) {
        /* The Knight jumping restriction still applies when riding a
         * horse.  After all, what shape is the knight piece in chess?
         */
        if (showmsg)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "Illegal move!");
        }
        return FALSE;
    } else if (distu(x, y) > (magic ? 6 + magic * 3 : 9)) {
        if (showmsg)
        {
            play_sfx_sound(SFX_GENERAL_TOO_FAR);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "Too far!");
        }
        return FALSE;
    } else if (!isok(x, y)) {
        if (showmsg)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot jump there!");
        }
        return FALSE;
    } else if (!cansee(x, y)) {
        if (showmsg)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT_SEE_SPOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot see where to land!");
        }
        return FALSE;
    } else {
        coord uc, tc;
        struct rm *lev = &levl[u.ux][u.uy];
        /* we want to categorize trajectory for use in determining
           passage through doorways: horizonal, vertical, or diagonal;
           since knight's jump and other irregular directions are
           possible, we flatten those out to simplify door checks */
        int diag, traj,
            dx = x - u.ux, dy = y - u.uy,
            ax = abs(dx), ay = abs(dy);

        /* diag: any non-orthogonal destination classifed as diagonal */
        diag = (magic || Passes_walls || (!dx && !dy)) ? jAny
               : !dy ? jHorz : !dx ? jVert : jDiag;
        /* traj: flatten out the trajectory => some diagonals re-classified */
        if (ax >= 2 * ay)
            ay = 0;
        else if (ay >= 2 * ax)
            ax = 0;
        traj = (magic || Passes_walls || (!ax && !ay)) ? jAny
               : !ay ? jHorz : !ax ? jVert : jDiag;
        /* walk_path doesn't process the starting spot;
           this is iffy:  if you're starting on a closed door spot,
           you _can_ jump diagonally from doorway (without needing
           Passes_walls); that's intentional but is it correct? */
        if (diag == jDiag && IS_DOOR(lev->typ)
            && (lev->doormask & D_ISOPEN) != 0
            && (traj == jDiag
                || ((traj & jHorz) != 0) == (lev->horizontal != 0))) {
            if (showmsg)
            {
                play_sfx_sound(SFX_SOMETHING_IN_WAY);
                You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "jump diagonally out of a doorway.");
            }
            return FALSE;
        }
        uc.x = u.ux, uc.y = u.uy;
        tc.x = x, tc.y = y; /* target */
        if (!walk_path(&uc, &tc, check_jump, (genericptr_t) &traj)) {
            if (showmsg)
            {
                play_sfx_sound(SFX_SOMETHING_IN_WAY);
                There("is an obstacle preventing that jump.");
            }
            return FALSE;
        }
    }
    return TRUE;
}

STATIC_VAR int jumping_is_magic;

STATIC_OVL int
get_invalid_jump_position(x, y)
int x, y;
{
    return !isok(x, y) || (!ACCESSIBLE(levl[x][y].typ) && !Passes_walls) ? 1 :
            !is_valid_jump_pos(x, y, jumping_is_magic, FALSE) ? 3 : 0;
}

void
display_jump_positions(state)
int state;
{
    if (state == 0) {
        tmp_at(DISP_BEAM, cmap_to_glyph(S_goodpos));
    } else if (state == 1) {
        int x, y, dx, dy;

        for (dx = -4; dx <= 4; dx++)
            for (dy = -4; dy <= 4; dy++) {
                x = dx + (int) u.ux;
                y = dy + (int) u.uy;
                if (!get_invalid_jump_position(x, y))
                    tmp_at(x, y);
            }
    } else {
        tmp_at(DISP_END, 0);
    }
}

int
jump(magic)
int magic; /* 0=Physical, otherwise skill level */
{
    coord cc;

    /* attempt "jumping" spell if hero has no innate jumping ability */
    if (!magic && !Jumping) 
    {
        int sp_no;

        for (sp_no = 0; sp_no < MAXSPELL; ++sp_no)
            if (spl_book[sp_no].sp_id == NO_SPELL)
                break;
            else if (spl_book[sp_no].sp_id == SPE_JUMPING)
                return spelleffects(sp_no, FALSE, &youmonst);
    }

    if (!magic && (nolimbs(youmonst.data) || slithy(youmonst.data))) 
    {
        /* normally (nolimbs || slithy) implies !Jumping,
           but that isn't necessarily the case for knights */
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "jump; you have no legs!");
        return 0;
    } 
    else if (!magic && !Jumping)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "jump very far.");
        return 0;
    /* if steed is immobile, can't do physical jump but can do spell one */
    }
    else if (!magic && u.usteed && stucksteed(FALSE)) 
    {
        /* stucksteed gave "<steed> won't move" message */
        return 0;
    } 
    else if (u.uswallow) 
    {
        if (magic) 
        {
            You("bounce around a little.");
            return 1;
        }
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "You've got to be kidding!");
        return 0;
    } 
    else if (u.uinwater) 
    {
        if (magic) 
        {
            You("swish around a little.");
            return 1;
        }
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "This calls for swimming, not jumping!");
        return 0;
    }
    else if (u.ustuck) 
    {
        if (is_tame(u.ustuck) && !Conflict && !is_confused(u.ustuck) && !is_crazed(u.ustuck))
        {
            play_sfx_sound(SFX_WRIGGLE_FREE);
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "pull free from %s.", mon_nam(u.ustuck));
            u.ustuck = 0;
            return 1;
        }
        if (magic) {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "writhe a little in the grasp of %s!", mon_nam(u.ustuck));
            return 1;
        }
        play_sfx_sound(SFX_STUCK_IN_TRAP);
        You_ex(ATR_NONE, CLR_MSG_WARNING, "cannot escape from %s!", mon_nam(u.ustuck));
        return 0;
    }
    else if (Levitation || Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)) 
    {
        if (magic) 
        {
            You("flail around a little.");
            return 1;
        }
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough traction to jump.");
        return 0;
    } 
    else if (!magic && near_capacity() > UNENCUMBERED) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are carrying too much to jump!");
        return 0;
    } 
    else if (!magic && (u.uhunger <= 100 || ACURR(A_STR) < 6))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "lack the strength to jump!");
        return 0;
    }
    else if (!magic && Wounded_legs) 
    {
        int64_t wl = (Wounded_legs & BOTH_SIDES);
        const char *bp = body_part(LEG);

        if (wl == BOTH_SIDES)
            bp = makeplural(bp);
        if (u.usteed)
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is in no shape for jumping.", Monnam(u.usteed));
        else
            Your_ex(ATR_NONE, CLR_MSG_FAIL, "%s%s %s in no shape for jumping.",
                 (wl == LEFT_SIDE) ? "left " : (wl == RIGHT_SIDE) ? "right "
                                                                  : "",
                 bp, (wl == BOTH_SIDES) ? "are" : "is");
        return 0;
    } 
    else if (u.usteed && u.utrap) 
    {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is stuck in a trap.", Monnam(u.usteed));
        return 0;
    }

    pline("Where do you want to jump?");
    cc.x = u.ux;
    cc.y = u.uy;
    jumping_is_magic = magic;
    getpos_sethilite(display_jump_positions, get_invalid_jump_position);
    if (getpos(&cc, TRUE, "the desired position", CURSOR_STYLE_JUMP_CURSOR) < 0)
        return 0; /* user pressed ESC */

    if (!is_valid_jump_pos(cc.x, cc.y, magic, TRUE))
    {
        return 0;
    } 
    else 
    {
        coord uc;
        int range, temp;
        u.dx = cc.x - u.ux;
        u.dy = cc.y - u.uy;
        update_u_facing(TRUE);
        play_sfx_sound(SFX_JUMP);

        if (u.utrap)
            switch (u.utraptype) 
            {
            case TT_BEARTRAP:
            {
                int64_t side = rn2(3) ? LEFT_SIDE : RIGHT_SIDE;

                play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
                You("rip yourself free of the bear trap!  Ouch!");
                losehp(adjust_damage(rnd(10), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "jumping out of a bear trap",
                       KILLED_BY);
                set_wounded_legs(side, rn1(1000, 500));
                break;
            }
            case TT_PIT:
                You("leap from the pit!");
                break;
            case TT_WEB:
                You("tear the web apart as you pull yourself free!");
                deltrap(t_at(u.ux, u.uy));
                break;
            case TT_LAVA:
                You("pull yourself above the %s!", hliquid("lava"));
                reset_utrap(TRUE);
                return 1;
            case TT_BURIEDBALL:
            case TT_INFLOOR:
                You("strain your %s, but you're still %s.",
                    makeplural(body_part(LEG)),
                    (u.utraptype == TT_INFLOOR)
                        ? "stuck in the floor"
                        : "attached to the buried ball");
                set_wounded_legs(LEFT_SIDE, rn1(10, 11));
                set_wounded_legs(RIGHT_SIDE, rn1(10, 11));
                return 1;
            }

        /*
         * Check the path from uc to cc, calling hurtle_step at each
         * location.  The final position actually reached will be
         * in cc.
         */
        uc.x = u.ux;
        uc.y = u.uy;
        /* calculate max(abs(dx), abs(dy)) as the range */
        range = cc.x - uc.x;
        if (range < 0)
            range = -range;
        temp = cc.y - uc.y;
        if (temp < 0)
            temp = -temp;
        if (range < temp)
            range = temp;
        (void) walk_path(&uc, &cc, hurtle_jump, (genericptr_t) &range);
        /* hurtle_jump -> hurtle_step results in <u.ux,u.uy> == <cc.x,cc.y>
         * and usually moves the ball if punished, but does not handle all
         * the effects of landing on the final position.
         */
        teleds(cc.x, cc.y, FALSE, FALSE);
        sokoban_guilt();
        nomul(-1);
        multi_reason = "jumping around";
        nomovemsg = "";
        morehungry(rnd(25));
        return 1;
    }
}

boolean
tinnable(corpse)
struct obj *corpse;
{
    if (corpse->oeaten)
        return 0;
    if (!mons[corpse->corpsenm].cnutrit)
        return 0;
    return 1;
}

STATIC_OVL void
use_tinning_kit(obj)
struct obj *obj;
{
    struct obj *corpse, *can;

    /* This takes only 1 move.  If this is to be changed to take many
     * moves, we've got to deal with decaying corpses...
     */
    if (obj->charges <= 0) 
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "seem to be out of tins.");
        return;
    }
    if (!(corpse = floorfood("tin", 2)))
        return;
    if (corpse->oeaten) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot tin %s which is partly eaten.", something);
        return;
    }
    if (corpse->corpsenm < LOW_PM)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "There seems to be something wrong with this corpse.");
        return;
    }

    You("start tinning %s.", the(cxname(corpse)));

    if (iflags.using_gui_sounds && !Deaf)
    {
        play_sfx_sound(SFX_APPLY_TINNING_KIT);
        delay_output_milliseconds(2000);
    }

    if (touch_petrifies(&mons[corpse->corpsenm]) && !Stone_resistance
        && !uarmg) 
    {
        char kbuf[BUFSZ];

        if (poly_when_stoned(youmonst.data))
            You_ex(ATR_NONE, CLR_MSG_WARNING, "tin %s without wearing gloves.",
                an(corpse_monster_name(corpse)));
        else {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Tinning %s without wearing gloves is a fatal mistake...",
                  an(corpse_monster_name(corpse)));
            Sprintf(kbuf, "trying to tin %s without gloves",
                    an(corpse_monster_name(corpse)));
        }
        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
        instapetrify(kbuf);
    }
    if (is_rider(&mons[corpse->corpsenm])) 
    {
        if (revive_corpse(corpse))
        {
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Yes...  But War does not preserve its enemies...");
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "corpse evades your grasp.");
        }
        return;
    }
    if (has_monster_type_nontinnable_corpse(&mons[corpse->corpsenm])) 
    {
        if (is_reviver(&mons[corpse->corpsenm]) && revives_upon_meddling(&mons[corpse->corpsenm]) && revive_corpse(corpse))
        {
            pline("It was a serious mistake to try to tin that...");
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "It seems oddly difficult to tin that.");
        }
        return;
    }
    if (mons[corpse->corpsenm].cnutrit == 0) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "That's too insubstantial to tin.");
        return;
    }
    consume_obj_charge(obj, TRUE);

    if ((can = mksobj(TIN, FALSE, FALSE, FALSE)) != 0) 
    {
        STATIC_VAR const char you_buy_it[] = "You tin it, you bought it!";

        can->corpsenm = corpse->corpsenm;
        can->cursed = obj->cursed;
        can->blessed = obj->blessed;
        can->owt = weight(can);
        can->known = 1;
        can->speflags |= is_female_corpse_or_statue(corpse) ? SPEFLAGS_FEMALE : 0UL;
        /* Mark tinned tins. No spinach allowed... */
        set_tin_variety(can, HOMEMADE_TIN);
        if (carried(corpse)) 
        {
            if (corpse->unpaid)
            {
                char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
                struct monst* shkp = shop_keeper(*o_shop);
                if (shkp && inhishop(shkp) && is_obj_on_shk_bill(corpse, shkp))
                {
                    play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_YOU_TIN_IT_YOU_BOUGHT_IT);
                    verbalize_angry1(you_buy_it);
                }
            }
            useup(corpse);
        } else {
            if (costly_spot(corpse->ox, corpse->oy) && !corpse->no_charge)
                verbalize_angry1(you_buy_it);
            useupf(corpse, 1L);
        }
        (void) hold_another_object(can, "You make, but cannot pick up, %s.",
                                   doname(can), (const char *) 0);
    } else
        impossible("Tinning failed.");
}

void
use_unicorn_horn(obj)
struct obj *obj;
{
    if (!obj)
        return;

#define PROP_COUNT 7           /* number of properties we're dealing with */
#define ATTR_COUNT (A_MAX * 3) /* number of attribute points we might fix */
    int idx, val, val_limit, trouble_count, unfixable_trbl, did_prop,
        did_attr;
    int trouble_list[PROP_COUNT + ATTR_COUNT];


    if (obj->charges <= 0) {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline1(nothing_happens);
        return;
    }
    consume_obj_charge(obj, TRUE);

    if (obj->cursed)
    {
        int64_t lcount = (int64_t) rn1(90, 10);

        switch (rn2(13) / 2) { /* case 6 is half as likely as the others */
        case 0:
            if(!Sick)
                play_sfx_sound(SFX_CATCH_TERMINAL_ILLNESS);
            make_sick((Sick & TIMEOUT) ? (Sick & TIMEOUT) / 3L + 1L
                                       : (int64_t) rn1(ACURR(A_CON), 20),
                      xname(obj), TRUE, HINT_KILLED_ILLNESS_FROM_CURSED_UNICORN_HORN);
            break;
        case 1:
            if(!Blind)
                play_sfx_sound(SFX_ACQUIRE_BLINDNESS);
            make_blinded((Blinded & TIMEOUT) + lcount, TRUE);
            break;
        case 2:
            if (!Confusion)
            {
                You_ex(ATR_NONE, CLR_MSG_WARNING, "suddenly feel %s.",
                    Hallucination ? "trippy" : "confused");
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            }
            make_confused(itimeout_incr(HConfusion, (int)lcount), TRUE);
            break;
        case 3:
            if(!Stunned)
                play_sfx_sound(SFX_ACQUIRE_STUN);
            make_stunned((HStun & TIMEOUT) + lcount, TRUE);
            break;
        case 4:
            play_sfx_sound(SFX_LOSE_ABILITY);
            (void) adjattrib(rn2(A_MAX), -1, FALSE);
            break;
        case 5:
            if (!Hallucination)
                play_sfx_sound(SFX_ACQUIRE_HALLUCINATION);
            (void) make_hallucinated((HHallucination & TIMEOUT) + lcount,
                                     TRUE, 0L);
            break;
        case 6:
            if (Deaf) /* make_deaf() won't give feedback when already deaf */
                pline("Nothing seems to happen.");
            make_deaf((HDeaf & TIMEOUT) + lcount, TRUE);
            break;
        }
        return;
    }

/*
 * Entries in the trouble list use a very simple encoding scheme.
 */
#define prop2trbl(X) ((X) + A_MAX)
#define attr2trbl(Y) (Y)
#define prop_trouble(X) trouble_list[trouble_count++] = prop2trbl(X)
#define attr_trouble(Y) trouble_list[trouble_count++] = attr2trbl(Y)
#define TimedTrouble(P) (((P) && !((P) & ~TIMEOUT)) ? ((P) & TIMEOUT) : 0L)

    trouble_count = unfixable_trbl = did_prop = did_attr = 0;

    /* collect property troubles */
    if (TimedTrouble(Sick))
        prop_trouble(SICK);
    if (TimedTrouble(FoodPoisoned))
        prop_trouble(FOOD_POISONED);
    if (MummyRot)
        prop_trouble(MUMMY_ROT);
    if (TimedTrouble(Blinded) > (int64_t) u.ucreamed
        && !(u.uswallow
             && attacktype_fordmg(u.ustuck->data, AT_ENGL, AD_BLND)))
        prop_trouble(BLINDED);
    if (TimedTrouble(HHallucination))
        prop_trouble(HALLUC);
    if (TimedTrouble(Vomiting))
        prop_trouble(VOMITING);
    if (TimedTrouble(HConfusion))
        prop_trouble(CONFUSION);
    if (TimedTrouble(HStun))
        prop_trouble(STUNNED);
    if (TimedTrouble(HDeaf))
        prop_trouble(DEAF);

    unfixable_trbl = unfixable_trouble_count(TRUE);

#if 0
    /* collect attribute troubles */
    for (idx = 0; idx < A_MAX; idx++) {
        if (ABASE(idx) >= AMAX(idx))
            continue;
        val_limit = AMAX(idx);
        /* this used to adjust 'val_limit' for A_STR when u.uhs was
           WEAK or worse, but that's handled via ATEMP(A_STR) now */
        if (Fixed_abil) {
            /* potion/spell of restore ability override sustain ability
               intrinsic but unicorn horn usage doesn't */
            unfixable_trbl += val_limit - ABASE(idx);
            continue;
        }
        /* don't recover more than 3 points worth of any attribute */
        if (val_limit > ABASE(idx) + 3)
            val_limit = ABASE(idx) + 3;

        for (val = ABASE(idx); val < val_limit; val++)
            attr_trouble(idx);
        /* keep track of unfixed trouble, for message adjustment below */
        unfixable_trbl += (AMAX(idx) - val_limit);
    }
#endif

    if (trouble_count == 0) 
    {
        pline1(nothing_happens);
        return;
    } 
    else if (trouble_count > 1)
    { /* shuffle */
        int i, j, k;

        for (i = trouble_count - 1; i > 0; i--)
            if ((j = rn2(i + 1)) != i) {
                k = trouble_list[j];
                trouble_list[j] = trouble_list[i];
                trouble_list[i] = k;
            }
    }

    /*
     *  Chances for number of troubles to be fixed
     *               0      1      2      3      4      5      6      7
     *   blessed:  22.7%  22.7%  19.5%  15.4%  10.7%   5.7%   2.6%   0.8%
     *  uncursed:  35.4%  35.4%  22.9%   6.3%    0      0      0      0
     */
    val_limit = (obj && obj->blessed) ? 2 : 1; // rn2(d(2, (obj&& obj->blessed) ? 4 : 2));
    if (val_limit > trouble_count)
        val_limit = trouble_count;

    /* fix [some of] the troubles */
    for (val = 0; val < val_limit; val++) {
        idx = trouble_list[val];

        switch (idx) {
        case prop2trbl(SICK):
            play_sfx_sound(SFX_CURE_DISEASE);
            make_sick(0L, (char *) 0, TRUE, 0);
            did_prop++;
            break;
        case prop2trbl(FOOD_POISONED):
            play_sfx_sound(SFX_CURE_DISEASE);
            make_food_poisoned(0L, (char*)0, TRUE, 0);
            did_prop++;
            break;
        case prop2trbl(MUMMY_ROT):
            play_sfx_sound(SFX_CURE_DISEASE);
            make_mummy_rotted(0L, (char*)0, TRUE, 0);
            did_prop++;
            break;
        case prop2trbl(BLINDED):
            play_sfx_sound(SFX_CURE_AILMENT);
            make_blinded((int64_t) u.ucreamed, TRUE);
            did_prop++;
            break;
        case prop2trbl(HALLUC):
            play_sfx_sound(SFX_CURE_AILMENT);
            (void) make_hallucinated(0L, TRUE, 0L);
            did_prop++;
            break;
        case prop2trbl(VOMITING):
            play_sfx_sound(SFX_CURE_DISEASE);
            make_vomiting(0L, TRUE);
            did_prop++;
            break;
        case prop2trbl(CONFUSION):
            play_sfx_sound(SFX_CURE_AILMENT);
            make_confused(0L, TRUE);
            did_prop++;
            break;
        case prop2trbl(STUNNED):
            play_sfx_sound(SFX_CURE_AILMENT);
            make_stunned(0L, TRUE);
            did_prop++;
            break;
        case prop2trbl(DEAF):
            play_sfx_sound(SFX_CURE_AILMENT);
            make_deaf(0L, TRUE);
            did_prop++;
            break;
        default:
            if (idx >= 0 && idx < A_MAX) {
                ABASE(idx) += 1;
                did_attr++;
            }
            else
            {
                panic("use_unicorn_horn: bad trouble? (%d)", idx);
                return;
            }
            break;
        }
    }

    if (did_attr || did_prop)
        context.botl = TRUE;
    if (did_attr)
    {
        play_sfx_sound(SFX_RESTORE_ABILITY);
        pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "This makes you feel %s!",
            (did_prop + did_attr) == (trouble_count + unfixable_trbl)
            ? "great"
            : "better");
    }
    else if (!did_prop)
        pline("Nothing seems to happen.");

#undef PROP_COUNT
#undef ATTR_COUNT
#undef prop2trbl
#undef attr2trbl
#undef prop_trouble
#undef attr_trouble
#undef TimedTrouble
}

/*
 * Timer callback routine: turn figurine into monster
 */
void
fig_transform(arg, timeout)
anything *arg;
int64_t timeout;
{
    struct obj *figurine = arg->a_obj;
    struct monst *mtmp;
    coord cc;
    boolean cansee_spot, silent, okay_spot;
    boolean redraw = FALSE;
    boolean suppress_see = FALSE;
    char monnambuf[BUFSZ], carriedby[BUFSZ];

    if (!figurine) {
        debugpline0("null figurine in fig_transform()");
        return;
    }
    silent = (timeout != monstermoves); /* happened while away */
    okay_spot = get_obj_location(figurine, &cc.x, &cc.y, 0);
    if (figurine->where == OBJ_INVENT || figurine->where == OBJ_MINVENT)
        okay_spot = enexto(&cc, cc.x, cc.y, &mons[figurine->corpsenm]);
    if (!okay_spot || !figurine_location_checks(figurine, &cc, TRUE)) {
        /* reset the timer to try again later */
        (void) start_timer((int64_t) rnd(5000), TIMER_OBJECT, FIG_TRANSFORM,
                           obj_to_any(figurine));
        return;
    }

    cansee_spot = cansee(cc.x, cc.y);
    mtmp = make_familiar(figurine, cc.x, cc.y, TRUE);
    if (mtmp) {
        char and_vanish[BUFSZ];
        struct obj *mshelter = level.objects[mtmp->mx][mtmp->my];

        /* [m_monnam() yields accurate mon type, overriding hallucination] */
        Sprintf(monnambuf, "%s", an(m_monnam(mtmp)));
        and_vanish[0] = '\0';
        if ((is_invisible(mtmp) && !See_invisible)
            || (is_mimic(mtmp->data)
                && M_AP_TYPE(mtmp) != M_AP_NOTHING))
            suppress_see = TRUE;

        if (mtmp->mundetected) {
            if (hides_under(mtmp->data) && mshelter) {
                Sprintf(and_vanish, " and %s under %s",
                        locomotion(mtmp->data, "crawl"), doname(mshelter));
            } else if (is_mimic(mtmp->data) || mtmp->data->mlet == S_EEL) {
                suppress_see = TRUE;
            } else
                Strcpy(and_vanish, " and vanish");
        }

        switch (figurine->where) {
        case OBJ_INVENT:
            if (Blind || suppress_see)
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s from your pack!", something,
                         locomotion(mtmp->data, "drop"));
            else
                You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s out of your pack%s!", monnambuf,
                        locomotion(mtmp->data, "drop"), and_vanish);
            break;

        case OBJ_FLOOR:
            if (cansee_spot && !silent) {
                if (suppress_see)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly vanishes!", an(xname(figurine)));
                else
                    You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "a figurine transform into %s%s!", monnambuf,
                            and_vanish);
                redraw = TRUE; /* update figurine's map location */
            }
            break;

        case OBJ_MINVENT:
            if (cansee_spot && !silent && !suppress_see) {
                struct monst *mon;

                mon = figurine->ocarry;
                /* figurine carrying monster might be invisible */
                if (mon)
                {
                    if (canseemon(mon)
                        && (!mon->wormno || cansee(mon->mx, mon->my)))
                        Sprintf(carriedby, "%s pack", s_suffix(a_monnam(mon)));
                    else if (is_pool(mon->mx, mon->my))
                        Strcpy(carriedby, "empty water");
                    else
                        Strcpy(carriedby, "thin air");
                    You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s out of %s%s!", monnambuf,
                        locomotion(mtmp->data, "drop"), carriedby,
                        and_vanish);
                }
            }
            break;
#if 0
        case OBJ_MIGRATING:
            break;
#endif

        default:
            impossible("figurine came to life where? (%d)",
                       (int) figurine->where);
            break;
        }
    }
    /* free figurine now */
    if (carried(figurine)) {
        useup(figurine);
    } else {
        obj_extract_self(figurine);
        obfree(figurine, (struct obj *) 0);
    }
    if (redraw)
        newsym(cc.x, cc.y);
}

STATIC_OVL boolean
figurine_location_checks(obj, cc, quietly)
struct obj *obj;
coord *cc;
boolean quietly;
{
    xchar x, y;

    if (carried(obj) && u.uswallow) {
        if (!quietly)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough room in here.");
        }
        return FALSE;
    }
    x = cc ? cc->x : u.ux;
    y = cc ? cc->y : u.uy;
    if (!isok(x, y)) {
        if (!quietly)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot put the figurine there.");
        }
        return FALSE;
    }
    if (IS_ROCK(levl[x][y].typ)
        && !(passes_walls(&mons[obj->corpsenm]) && may_passwall(x, y))) {
        if (!quietly)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot place a figurine in %s!",
                IS_TREE(levl[x][y].typ) ? "a tree" : "solid rock");
        }
        return FALSE;
    }
    if (sobj_at(BOULDER, x, y) && !passes_walls(&mons[obj->corpsenm])
        && !throws_rocks(&mons[obj->corpsenm])) {
        if (!quietly)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot fit the figurine on the boulder.");
        }
        return FALSE;
    }
    return TRUE;
}

STATIC_OVL void
use_figurine(optr)
struct obj **optr;
{
    register struct obj *obj = *optr;
    xchar x, y;
    coord cc;

    if (u.uswallow) {
        /* can't activate a figurine while swallowed */
        if (!figurine_location_checks(obj, (coord *) 0, FALSE))
            return;
    }
    if (!getdir((char *) 0)) {
        context.move = multi = 0;
        return;
    }
    update_u_facing(TRUE);
    x = u.ux + u.dx;
    y = u.uy + u.dy;
    cc.x = x;
    cc.y = y;
    /* Passing FALSE arg here will result in messages displayed */
    if (!figurine_location_checks(obj, &cc, FALSE))
        return;
    You("%s and it %stransforms.",
        (u.dx || u.dy) ? "set the figurine beside you"
                       : (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)
                          || is_pool(cc.x, cc.y))
                             ? "release the figurine"
                             : (u.dz < 0 ? "toss the figurine into the air"
                                         : "set the figurine on the ground"),
        Blind ? "supposedly " : "");
    (void) make_familiar(obj, cc.x, cc.y, FALSE);
    (void) stop_timer(FIG_TRANSFORM, obj_to_any(obj));
    useup(obj);
    if (Blind)
        map_invisible(cc.x, cc.y);
    *optr = 0;
}

STATIC_VAR NEARDATA const char lubricables[] = { ALL_CLASSES, ALLOW_NONE, 0 };

STATIC_OVL void
use_grease(obj)
struct obj *obj;
{
    struct obj *otmp;

    if (Glib) {
        pline("%s from your %s.", Tobjnam(obj, "slip"),
              makeplural(body_part(FINGER)));
        dropxf(obj);
        return;
    }

    if (obj->charges > 0)
    {
        if ((obj->cursed || Fumbling) && !rn2(2)) 
        {
            consume_obj_charge(obj, TRUE);

            pline("%s from your %s.", Tobjnam(obj, "slip"),
                  makeplural(body_part(FINGER)));
            dropxf(obj);
            return;
        }

        otmp = getobj(lubricables, objects[obj->otyp].oc_name_known ? "grease" : "apply viscous fluid on", 0, "");
        if (!otmp)
            return;
        if (inaccessible_equipment(otmp, objects[obj->otyp].oc_name_known ? "grease" : "apply viscous fluid on", FALSE, FALSE))
            return;

        consume_obj_charge(obj, TRUE);

        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_APPLY);

        if (otmp != &zeroobj)
        {
            play_sfx_sound(SFX_GREASE_COATING);
            You("cover %s with a thick layer of grease.", yname(otmp));
            otmp->greased = 1;
            if (obj->cursed && !nohands(youmonst.data)) 
            {
                play_sfx_sound(SFX_ACQUIRE_GLIB);
                incr_itimeout(&Glib, rnd(15));
                refresh_u_tile_gui_info(TRUE);
                pline("Some of the grease gets all over your %s.",
                      makeplural(body_part(HAND)));
            }
        }
        else
        {
            play_sfx_sound(SFX_ACQUIRE_GLIB);
            incr_itimeout(&Glib, rnd(15));
            refresh_u_tile_gui_info(TRUE);
            You("coat your %s with grease.", makeplural(body_part(FINGER)));
        }
        makeknown(obj->otyp);
    } 
    else 
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        if (obj->known)
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s empty.", Tobjnam(obj, "are"));
        else
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s to be empty.", Tobjnam(obj, "seem"));
    }
    update_inventory();
}

STATIC_VAR NEARDATA const char wand_application_objects[] = { ALL_CLASSES, ALLOW_NONE, 0 };

STATIC_OVL int 
use_wand_on_object(obj)
struct obj* obj;
{
    struct obj* otmp;
    char buftext[BUFSZ] = "";
    char buftext2[BUFSZ] = "";
    char buftext3[BUFSZ] = "";
    boolean undonned = FALSE, wandknown = FALSE, suggestnamingwand = FALSE;
    int res = 1;

    if (!obj || obj->oclass != WAND_CLASS)
        return 0;

    if (Glib) 
    {
        pline("%s from your %s.", Tobjnam(obj, "slip"),
            makeplural(body_part(FINGER)));
        dropxf(obj);
        return 0;
    }

    if (obj->charges > 0) 
    {
        otmp = getobj(wand_application_objects, "use wand on", 0, "");
        if (!otmp)
            return 0;

        if (otmp == obj)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot use the wand on itself!");
            return 0;
        }

        if (inaccessible_equipment(otmp, "use wand on", FALSE, FALSE))
            return 0;

        consume_obj_charge(obj, TRUE);

        if ((obj->cursed || Fumbling) && !rn2(2)) 
        {
            //Shoot accidently yourself!!

            pline("The wand slips and you accidently zap yourself with it!");
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_ZAP);
            return (zapyourself(obj, TRUE) > 0 ? 1 : 0);
        }

        //Normal effect
        if (otmp && otmp != &zeroobj) 
        {
            play_sfx_sound(SFX_ZAP);
            switch (obj->otyp)
            {
            case WAN_DEATH:
                if (is_death_enchantable(otmp))
                {
                    wandknown = TRUE;
                    You_ex(ATR_NONE, CLR_MSG_POSITIVE, "enchant %s with death magic.", yname(otmp));
                    otmp = elemental_enchant_quan(otmp, 
                        max(1, DEATH_ENCHANTMENT_QUANTITY_NORMAL + bcsign(obj) * DEATH_ENCHANTMENT_QUANTITY_BUC_VARIATION),
                        DEATH_ENCHANTMENT);
                    prinv((char*)0, otmp, 0L);
                    //otmp->elemental_enchantment = DEATH_ENCHANTMENT;
                }
                else if (otmp->oclass == FOOD_CLASS)
                {
                    otmp->orotten = TRUE;
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s %s in black energy for a moment.", The(cxname(otmp)), otense(otmp, "shine"));
                }
                else
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s in black energy for a moment.", Tobjnam(otmp, "flicker"));
                }
                break;
            case WAN_COLD:
                if (otmp->elemental_enchantment == DEATH_ENCHANTMENT)
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s in blue for a moment, but then glows black.", Tobjnam(otmp, "flicker"));
                    break;
                }
                if (otmp->elemental_enchantment == FIRE_ENCHANTMENT)
                {
                    wandknown = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "The cold energies of %s dispel the fire enchantment on %s.", the(xname(obj)), yname(otmp));
                    otmp->elemental_enchantment = 0;
                    break;
                }

                if (is_elemental_enchantable(otmp))
                {
                    wandknown = TRUE;
                    You_ex(ATR_NONE, CLR_MSG_POSITIVE, "enchant %s with cold magic.", yname(otmp));
                    otmp = elemental_enchant_quan(otmp, 
                        max(1, ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL + bcsign(obj) * ELEMENTAL_ENCHANTMENT_QUANTITY_BUC_VARIATION),
                        COLD_ENCHANTMENT);
                    prinv((char*)0, otmp, 0L);
                    //otmp->elemental_enchantment = COLD_ENCHANTMENT;
                }
                else if (otmp->oclass == POTION_CLASS && !oresist_cold(otmp))
                {
                    wandknown = TRUE;
                    destroy_one_item(otmp, otmp->oclass, AD_COLD, TRUE);
                    update_inventory();
                }
                else if (otmp->oclass == FOOD_CLASS)
                {
                    wandknown = TRUE;
                    pline_ex(ATR_NONE, HI_ICE, "%s %s covered in frost, but that's about it.", The(cxname(otmp)), otense(otmp, "are"));
                }
                else
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s in blue for a moment.", Tobjnam(otmp, "flicker"));
                }
                break;
            case WAN_FIRE:
                if (otmp->oartifact == ART_RULING_RING_OF_YENDOR)
                {
                    wandknown = TRUE;
                    play_sfx_sound(SFX_MONSTER_ON_FIRE);
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "Flames emerge from %s and engulf %s!", the(xname(obj)), yname(otmp));
                    read_the_ruling_ring(otmp);
                    break;
                }

                if (otmp->elemental_enchantment == DEATH_ENCHANTMENT)
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s in red for a moment, but then glows black.", Tobjnam(otmp, "flicker"));
                    break;
                }
                if (otmp->elemental_enchantment == COLD_ENCHANTMENT)
                {
                    wandknown = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "The fiery energies of %s dispel the cold enchantment on %s.", the(xname(obj)), yname(otmp));
                    otmp->elemental_enchantment = 0;
                    break;
                }

                if (is_flammable(otmp) && (otmp->oclass == WEAPON_CLASS || otmp->oclass == ARMOR_CLASS))
                {
                    wandknown = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "Flames erupt from %s and engulf %s!", the(xname(obj)), yname(otmp));
                    (void)erode_obj(otmp, xname(otmp), ERODE_BURN,
                        EF_GREASE | EF_VERBOSE);
                }
                else if (is_candle_or_torch(otmp))
                {
                    wandknown = TRUE;
                    if (!otmp->lamplit)
                    {
                        pline_ex(ATR_NONE, CLR_MSG_SPELL, "A flame eminates from %s and lights up %s.", yname(obj), yname(otmp));
                        use_lamp(otmp);
                    }
                    else
                        pline_ex(ATR_NONE, CLR_MSG_SPELL, "A flame eminates from %s, keeping %s alight.", yname(obj), yname(otmp));
                }
                else if(is_elemental_enchantable(otmp))
                {
                        wandknown = TRUE;
                        You_ex(ATR_NONE, CLR_MSG_POSITIVE, "enchant %s with fire magic.", yname(otmp));
                        otmp = elemental_enchant_quan(otmp, 
                            max(1, ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL + bcsign(obj) * ELEMENTAL_ENCHANTMENT_QUANTITY_BUC_VARIATION),
                            FIRE_ENCHANTMENT);
                        prinv((char*)0, otmp, 0L);
                        //otmp->elemental_enchantment = FIRE_ENCHANTMENT;
                }
                else if ((otmp->oclass == POTION_CLASS || otmp->oclass == SCROLL_CLASS || otmp->oclass == SPBOOK_CLASS) && !oresist_fire(otmp))
                {
                    wandknown = TRUE;
                    destroy_one_item(otmp, otmp->oclass, AD_FIRE, TRUE);
                    update_inventory();
                }
                else if (otmp->oclass == FOOD_CLASS)
                {
                    wandknown = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s %s covered in flames, but that's about it.", The(cxname(otmp)), otense(otmp, "are"));
                }
                else
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s in red for a moment.", Tobjnam(otmp, "flicker"));
                }
                break;
            case WAN_LIGHTNING:
                if (otmp->elemental_enchantment == DEATH_ENCHANTMENT)
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s in blue for a moment, but then glows black.", Tobjnam(otmp, "flicker"));
                    break;
                }
                if (is_elemental_enchantable(otmp))
                {
                    wandknown = TRUE;
                    You_ex(ATR_NONE, CLR_MSG_POSITIVE, "enchant %s with lightning magic.", yname(otmp));
                    otmp = elemental_enchant_quan(otmp, 
                        max(1, ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL + bcsign(obj) * ELEMENTAL_ENCHANTMENT_QUANTITY_BUC_VARIATION), 
                        LIGHTNING_ENCHANTMENT);
                    prinv((char*)0, otmp, 0L);
                }
                else if ((otmp->oclass == RING_CLASS || otmp->oclass == WAND_CLASS) && !oresist_elec(otmp))
                {
                    wandknown = TRUE;
                    destroy_one_item(otmp, otmp->oclass, AD_ELEC, TRUE);
                    update_inventory();
                }
                else if (otmp->oclass == FOOD_CLASS)
                {
                    wandknown = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s %s jolted by lightning, but that's about it.", The(cxname(otmp)), otense(otmp, "are"));                }
                else
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s in blue for a moment.", Tobjnam(otmp, "flicker"));
                }
                break;
            case WAN_STRIKING:
                if(hero_breaks(otmp, u.ux, u.uy, TRUE))
                    wandknown = TRUE;
                else
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s for a while, but that's about it.", Tobjnam(otmp, "vibrate"));
                }
                break;
            case WAN_DISJUNCTION:
            case WAN_CANCELLATION:
                if (objects[otmp->otyp].oc_magic || otmp->enchantment != 0 || otmp->charges > (otmp->oclass == WAND_CLASS ? -1 : 0) || otmp->elemental_enchantment > 0 || otmp->blessed || otmp->cursed)
                {
                    suggestnamingwand = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s in gray for a while.", Tobjnam(otmp, "flicker"));
                }
                else
                    pline("Nothing much happens.");

                cancel_item(otmp, TRUE);
                break;
            case WAN_DISINTEGRATION:
                //Blessed enchant weapon for Black Blade
                if (otmp->otyp == BLACK_BLADE_OF_DISINTEGRATION)
                {
                    suggestnamingwand = TRUE;
                    //This will prompt weapon glow
                    play_sfx_sound(SFX_AURA_GLOW);
                    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s black-bluish for a while.", Yobjnam2(otmp, "glow"));
                    otmp->enchantment += 3 - otmp->enchantment / 3;
                    break;
                }

                if (uarmc && uarmc->otyp == CLOAK_OF_INTEGRITY)
                {
                    wandknown = TRUE;
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s the destructive energies of %s.", Yobjnam2(uarmc, "absorb"), the(xname(obj)));
                    makeknown(uarmc->otyp);
                    break;
                }

                if (oresist_disintegration(otmp) || obj->otyp == AMULET_OF_LIFE_SAVING) 
                {
                    if (!Blind)
                    {
                        suggestnamingwand = TRUE;
                        const char* hclr = hcolor_multi_buf2(NH_BLACK);
                        multicolor_buffer[1] = multicolor_buffer[3] = multicolor_buffer[2];
                        pline_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s a %s%s%s, but %s intact.",
                            Tobjnam(otmp, "glow"), "strange ", hclr, " light", otense(otmp, "remain"));
                    }
                    break;
                }

                //Now disintegrate object
                if (otmp->owornmask)
                    remove_worn_item(otmp, TRUE);

                play_sfx_sound(SFX_DISINTEGRATE);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s disintegrated!", Yobjnam2(otmp, "are"));
                wandknown = TRUE;
                //Destroy item;
                useupall(otmp);
                break;
            case WAN_POLYMORPH:
                if (otmp->otyp == WAN_POLYMORPH || otmp->otyp == SPE_POLYMORPH
                    || otmp->otyp == POT_POLYMORPH || obj_resists(otmp, 5, 95)) 
                {
                    if (!Blind)
                    {
                        suggestnamingwand = TRUE;
                        const char* hclr = hcolor_multi_buf2(NH_PURPLE);
                        multicolor_buffer[1] = multicolor_buffer[3] = multicolor_buffer[2];
                        pline_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s a %s%s%s, but %s intact.",
                            Tobjnam(otmp, "glow"), "strange ", hclr, " light", otense(otmp, "remain"));
                    }
                    break;
                }

                //Polymorph it now
                wandknown = TRUE;
                if (otmp->owornmask)
                {
                    remove_worn_item(otmp, TRUE);
                    undonned = TRUE;
                }
                Strcpy(buftext, Yname2(otmp));
                Strcpy(buftext2, otense(otmp, "morph"));
                Strcpy(buftext3, otense(otmp, "undon"));
                Strcat(buftext3, " and ");

                otmp = poly_obj(otmp, STRANGE_OBJECT);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s%s into %s!", buftext, undonned ? buftext3 : "", buftext2, an(xname(otmp)));
                update_inventory();
                break;
            case WAN_EVAPORATION:
                if (otmp->oclass == POTION_CLASS)
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s!", Yobjnam2(otmp, "evaporate"));
                    wandknown = TRUE;
                    useupall(otmp);
                }
                else
                    pline("Nothing much happens.");

                break;
            case WAN_PROBING:
                if (!probe_object(otmp))
                    pline("Nothing much happens.");
                break;
            case WAN_TELEPORTATION:
                if(otmp->owornmask)
                    remove_worn_item(otmp, TRUE);
                if (otmp->otyp == LEASH && otmp->leashmon)
                    o_unleash(otmp);
                freeinv(otmp);
                obj_clear_found(otmp);
                place_object(otmp, u.ux, u.uy);
                char tbuf[BUFSZ * 2];
                Strcpy(tbuf, Tobjnam(otmp, "vanish"));
                boolean stillexists = rloco(otmp);
                play_sfx_sound(SFX_TELEPORT);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s from your person%s.", tbuf, !stillexists ? "" :
                    otmp->ox == u.ux && otmp->oy == u.uy ? " and then appears at your feet" : 
                    cansee(otmp->ox, otmp->oy) && dist2(u.ux, u.uy, otmp->ox, otmp->oy) <= 9 ? " and then appears near you" : 
                    cansee(otmp->ox, otmp->oy) ? " and then appears at a distance from you" :
                    "");
                res = 1;
                break;
            default:
                pline("Nothing much happens.");
                res = 0;
                break;
            }

            //Make the used wand known
            if (!objects[obj->otyp].oc_name_known) 
            {
                if (wandknown)
                    makeknown(obj->otyp);
                else if (suggestnamingwand && !objects[obj->otyp].oc_uname)
                    docall(obj, (char*)0);
            }

        }
    }
    else 
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        res = 0;
        if (obj->known)
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s out of charges.", Tobjnam(obj, "are"));
        else
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s out of charges.", Tobjnam(obj, "seem"));
    }
    update_inventory();
    return res;
}

struct obj*
elemental_enchant_quan(otmp, quan, enchantmenttype)
struct obj* otmp;
int quan;
uchar enchantmenttype;
{
    boolean objsplitted = FALSE;

    if (otmp->quan > 1L && otmp->quan > quan) {
        objsplitted = TRUE;
        if (!carried(otmp))
            (void)splitobj(otmp, otmp->quan - quan);
        else
            otmp = splitobj(otmp, quan);
        debugpline0("split object,");
    }

    if (is_elemental_enchantable(otmp))
    {
        if (enchantmenttype == DEATH_ENCHANTMENT && !is_death_enchantable(otmp))
            otmp->elemental_enchantment = COLD_ENCHANTMENT;
        else
            otmp->elemental_enchantment = enchantmenttype;

        if (otmp->where == OBJ_INVENT)
            update_inventory();
    }

    if (carried(otmp) && objsplitted) {
        freeinv(otmp);
        if (inv_cnt(FALSE) >= 52) {
            sellobj_state(SELL_DONTSELL);
            dropyf(otmp);
            sellobj_state(SELL_NORMAL);
        }
        else {
            //Can be merged with other specially-enchanted items
            //otmp->nomerge = 1; /* used to prevent merge */
            otmp = addinv(otmp);
            //otmp->nomerge = 0;
        }
    }
    return otmp;
}


/* touchstones - by Ken Arnold */
STATIC_OVL void
use_stone(tstone)
struct obj *tstone;
{
    struct obj *obj;
    boolean do_scratch;
    const char *streak_color, *choices;
    char stonebuf[QBUFSZ];
    static const char scritch[] = "\"scritch, scritch\"";
    static const char allowall[3] = { COIN_CLASS, ALL_CLASSES, 0 };
    static const char coins_gems[3] = { COIN_CLASS, GEM_CLASS, 0 };

    /* in case it was acquired while blinded */
    if (!Blind)
        tstone->dknown = 1;
    /* when the touchstone is fully known, don't bother listing extra
       junk as likely candidates for rubbing */
    choices = (tstone->otyp == TOUCHSTONE && tstone->dknown
               && objects[TOUCHSTONE].oc_name_known)
                  ? coins_gems
                  : allowall;
    Sprintf(stonebuf, "rub on the stone%s", plur(tstone->quan));
    if ((obj = getobj(choices, stonebuf, 0, "")) == 0)
        return;

    if (obj == tstone && obj->quan == 1L)
    {
        play_sfx_sound(SFX_GENERAL_THATS_SILLY);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "rub %s on itself.", the(xname(obj)));
        return;
    }

    if (tstone->otyp == TOUCHSTONE && tstone->cursed
        && obj->oclass == GEM_CLASS && !is_graystone(obj)
        && !obj_resists(obj, 80, 100)) 
    {
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BREAK);
        
        if (Blind)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You feel something shatter.");
        else if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "Oh, wow, look at the pretty shards.");
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A sharp crack shatters %s%s.",
                  (obj->quan > 1L) ? "one of " : "", the(xname(obj)));
        useup(obj);
        return;
    }

    if (Blind) 
    {
        pline(scritch);
        return;
    } 
    else if (Hallucination)
    {
        pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "Oh wow, man: Fractals!");
        return;
    }

    do_scratch = FALSE;
    streak_color = 0;

    switch (obj->oclass) 
    {
    case GEM_CLASS: /* these have class-specific handling below */
    case RING_CLASS:
        if (tstone->otyp != TOUCHSTONE || tstone->charges <= 0)
        {
            do_scratch = TRUE;
        } 
        else if (obj->oclass == GEM_CLASS) /* Touchstone */
        {
            tstone->charges--;
            makeknown(TOUCHSTONE);
            makeknown(obj->otyp);
            prinv((char *) 0, obj, 0L);
            play_sfx_sound(SFX_IDENTIFY_SUCCESS);
            update_inventory();
            return;
        } 
        else
        {
            /* either a ring or the touchstone was not effective */
            if (obj->material == MAT_GLASS || obj->material == MAT_CRYSTAL)
            {
                do_scratch = TRUE;
                break;
            }
        }
        streak_color = c_obj_colors[objects[obj->otyp].oc_color];
        break; /* gem or ring */

    default:
        switch (obj->material)
        {
        case MAT_COTTON:
        case MAT_CLOTH:
            pline("%s a little more polished now.", Tobjnam(tstone, "look"));
            return;
        case MAT_LIQUID:
            if (!obj->known) /* note: not "whetstone" */
                You("must think this is a wetstone, do you?");
            else
                pline("%s a little wetter now.", Tobjnam(tstone, "are"));
            return;
        default:
            /* Objects passing the is_flimsy() test will not
               scratch a stone.  They will leave streaks on
               non-touchstones and touchstones alike. */
            if (is_flimsy(obj))
                streak_color = c_obj_colors[objects[obj->otyp].oc_color];
            else
            {
                do_scratch = material_definitions[obj->material].scratchable;
                streak_color = material_definitions[obj->material].streakword;
            }
            break;
        }
        break; /* default oclass */
    }

    Sprintf(stonebuf, "stone%s", plur(tstone->quan));
    if (do_scratch)
    {
        play_simple_object_sound(tstone, OBJECT_SOUND_TYPE_APPLY);
        You("make %s%sscratch marks on the %s.",
            streak_color ? streak_color : (const char*)"",
            streak_color ? " " : "", stonebuf);
    }
    else if (streak_color)
    {
        play_simple_object_sound(tstone, OBJECT_SOUND_TYPE_APPLY);
        You_see("%s streaks on the %s.", streak_color, stonebuf);
    }
    else
    {
        play_simple_object_sound(tstone, OBJECT_SOUND_TYPE_APPLY);
        pline(scritch);
    }
    return;
}

STATIC_VAR struct trapinfo {
    struct obj *tobj;
    xchar tx, ty;
    int time_needed;
    boolean force_bungle;
} trapinfo;

void
reset_trapset(VOID_ARGS)
{
    trapinfo.tobj = 0;
    trapinfo.force_bungle = 0;
}

/* Place a landmine/bear trap.  Helge Hafting */
STATIC_OVL void
use_trap(otmp)
struct obj *otmp;
{
    int ttyp, tmp;
    const char *what = (char *) 0;
    char buf[BUFSZ];
    int levtyp = levl[u.ux][u.uy].typ;
    const char *occutext = "setting the trap";

    if (nohands(youmonst.data) && !is_telekinetic_operator(youmonst.data))
        what = "without hands";
    else if (Stunned)
        what = "while stunned";
    else if (u.uswallow)
        what =
            is_animal(u.ustuck->data) ? "while swallowed" : "while engulfed";
    else if (Underwater)
        what = "underwater";
    else if (Levitation)
        what = "while levitating";
    else if (is_pool(u.ux, u.uy))
        what = "in water";
    else if (is_lava(u.ux, u.uy))
        what = "in lava";
    else if (On_stairs(u.ux, u.uy))
        what = (u.ux == xdnladder || u.ux == xupladder) ? "on the ladder"
                                                        : "on the stairs";
    else if (IS_FURNITURE(levtyp) || IS_ROCK(levtyp)
             || closed_door(u.ux, u.uy) || t_at(u.ux, u.uy))
        what = "here";
    else if (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz))
        what = (levtyp == AIR)
                   ? "in midair"
                   : (levtyp == CLOUD)
                         ? "in a cloud"
                         : "in this place"; /* Air/Water Plane catch-all */
    if (what) {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "set a trap %s!", what);
        reset_trapset();
        return;
    }
    ttyp = (otmp->otyp == LAND_MINE) ? LANDMINE : BEAR_TRAP;
    if (otmp == trapinfo.tobj && u.ux == trapinfo.tx && u.uy == trapinfo.ty) {
        You("resume setting %s%s.", shk_your(buf, otmp),
            defsyms[trap_to_defsym(what_trap(ttyp, rn2))].explanation);
        set_occupation(set_trap, occutext, objects[otmp->otyp].oc_soundset, OCCUPATION_SETTING_TRAP, OCCUPATION_SOUND_TYPE_RESUME, 0);
        return;
    }
    trapinfo.tobj = otmp;
    trapinfo.tx = u.ux, trapinfo.ty = u.uy;
    tmp = ACURR(A_DEX);
    trapinfo.time_needed =
        (tmp > 17) ? 2 : (tmp > 12) ? 3 : (tmp > 7) ? 4 : 5;
    if (Blind)
        trapinfo.time_needed *= 2;
    tmp = ACURR(A_STR);
    if (ttyp == BEAR_TRAP && tmp < 18)
        trapinfo.time_needed += (tmp > 12) ? 1 : (tmp > 7) ? 2 : 4;
    /*[fumbling and/or confusion and/or cursed object check(s)
       should be incorporated here instead of in set_trap]*/
    if (u.usteed && P_SKILL_LEVEL(P_RIDING) < P_BASIC) {
        boolean chance;

        if (Fumbling || otmp->cursed)
            chance = (rnl(10) > 3);
        else
            chance = (rnl(10) > 5);
        You("aren't very skilled at reaching from %s.", mon_nam(u.usteed));
        Sprintf(buf, "Continue your attempt to set %s?",
                the(defsyms[trap_to_defsym(what_trap(ttyp, rn2))]
                    .explanation));
        if (yn_query(buf) == 'y') {
            if (chance) {
                switch (ttyp) {
                case LANDMINE: /* set it off */
                    trapinfo.time_needed = 0;
                    trapinfo.force_bungle = TRUE;
                    break;
                case BEAR_TRAP: /* drop it without arming it */
                    reset_trapset();
                    You("drop %s!",
                        the(defsyms[trap_to_defsym(what_trap(ttyp, rn2))]
                                .explanation));
                    dropxf(otmp);
                    return;
                }
            }
        } else {
            reset_trapset();
            return;
        }
    }
    You("begin setting %s%s.", shk_your(buf, otmp),
        defsyms[trap_to_defsym(what_trap(ttyp, rn2))].explanation);

    set_occupation(set_trap, occutext, objects[otmp->otyp].oc_soundset, OCCUPATION_SETTING_TRAP, OCCUPATION_SOUND_TYPE_START, 0);
    return;
}

STATIC_OVL
int
set_trap(VOID_ARGS)
{
    struct obj *otmp = trapinfo.tobj;
    struct trap *ttmp;
    int ttyp;

    if (!otmp || !carried(otmp) || u.ux != trapinfo.tx
        || u.uy != trapinfo.ty) {
        /* ?? */
        reset_trapset();
        return 0;
    }

    if (--trapinfo.time_needed > 0)
        return 1; /* still busy */

    ttyp = (otmp->otyp == LAND_MINE) ? LANDMINE : BEAR_TRAP;
    ttmp = maketrap(u.ux, u.uy, ttyp, NON_PM, MKTRAPFLAG_MADE_BY_U);
    if (ttmp) {
        feeltrap(ttmp);
        if (*in_rooms(u.ux, u.uy, SHOPBASE)) {
            add_damage(u.ux, u.uy, 0L); /* schedule removal */
        }
        if (!trapinfo.force_bungle)
        {

            if (ttyp == LANDMINE)
                play_sfx_sound(SFX_LAND_MINE_SET_UP);
            else
                play_sfx_sound(SFX_BEAR_TRAP_SET_UP);

            You("finish arming %s.",
                the(defsyms[trap_to_defsym(what_trap(ttyp, rn2))].explanation));
        }
        if (((otmp->cursed || Fumbling) && (rnl(10) > 5))
            || trapinfo.force_bungle)
            dotrap(ttmp,
                   (unsigned short) (trapinfo.force_bungle ? FORCEBUNGLE : 0));
    } else {
        /* this shouldn't happen */
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "trap setting attempt fails.");
    }
    useup(otmp);
    reset_trapset();
    return 0;
}

STATIC_OVL int
use_whip(obj)
struct obj *obj;
{
    char buf[BUFSZ];
    struct monst *mtmp;
    struct obj *otmp;
    int rx, ry, proficient, res = 0;
    const char *msg_slipsfree = "The bullwhip slips free.";
    const char *msg_snap = "Snap!";

    if (obj != uwep) {
        if (!wield_tool(obj, "lash"))
            return 0;
        else
            res = 1;
    }
    if (!getdir((char *) 0))
        return res;

    update_u_facing(TRUE);

    if (u.uswallow) {
        mtmp = u.ustuck;
        rx = mtmp->mx;
        ry = mtmp->my;
    } else {
        if (Stunned || (Confusion && !rn2(5)))
            confdir();
        rx = u.ux + u.dx;
        ry = u.uy + u.dy;
        if (!isok(rx, ry)) 
        {
            You("miss.");
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            return res;
        }
        mtmp = m_at(rx, ry);
    }

    proficient = whip_skill_weapon_disarm_bonus(P_SKILL_LEVEL(P_WHIP)) + exceptionality_weapon_disarm_bonus(obj);
    if (Fumbling)
        proficient -= 3;
    if (proficient < 0)
        proficient = 0;

    if (u.uswallow && attack(u.ustuck)) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        There_ex(ATR_NONE, CLR_MSG_FAIL, "is not enough room to flick your bullwhip.");

    } else if (Underwater) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        There_ex(ATR_NONE, CLR_MSG_FAIL, "is too much resistance to flick your bullwhip.");

    } else if (u.dz < 0) {
        You("flick a bug off of the %s.", ceiling(u.ux, u.uy));

    } else if ((!u.dx && !u.dy) || (u.dz > 0)) {
        update_u_action(ACTION_TILE_ATTACK);
        play_monster_simple_weapon_sound(&youmonst, 0, obj, OBJECT_SOUND_TYPE_SWING_MELEE);
        u_wait_until_action();

        int dam;

        /* Sometimes you hit your steed by mistake */
        if (u.usteed && proficient < 3 && !rn2(proficient + 2)) {
            You("whip %s!", mon_nam(u.usteed));
            kick_steed();
            update_u_action_revert(ACTION_TILE_NO_ACTION);
            return 1;
        }
        if (Levitation || u.usteed) {
            /* Have a shot at snaring something on the floor */
            otmp = level.objects[u.ux][u.uy];
            if (otmp && otmp->otyp == CORPSE && otmp->corpsenm == PM_HORSE) {
                pline("Why beat a dead horse?");
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            }
            if (otmp && proficient) {
                You("wrap your bullwhip around %s on the %s.",
                    an(singular(otmp, xname)), surface(u.ux, u.uy));
                if (rnl(6) || pickup_object(otmp, 1L, TRUE, FALSE, (uchar*)0) < 1)
                    pline1(msg_slipsfree);
                update_u_action_revert(ACTION_TILE_NO_ACTION);
                return 1;
            }
        }
        dam = rnd(2) + u_str_dmg_bonus() + obj->enchantment;
        if (dam <= 0)
            dam = 1;
        You("hit your %s with your bullwhip.", body_part(FOOT));
        Sprintf(buf, "killed %sself with %s bullwhip", uhim(), uhis());
        losehp(adjust_damage(dam, &youmonst, &youmonst, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE), buf, NO_KILLER_PREFIX);
        update_u_action_revert(ACTION_TILE_NO_ACTION);
        return 1;

    } else if ((Fumbling || Glib) && !rn2(5)) {
        pline_The("bullwhip slips out of your %s.", body_part(HAND));
        dropxf(obj);

    } else if (u.utrap && u.utraptype == TT_PIT) {
        /*
         * Assumptions:
         *
         * if you're in a pit
         *    - you are attempting to get out of the pit
         * or, if you are applying it towards a small monster
         *    - then it is assumed that you are trying to hit it
         * else if the monster is wielding a weapon
         *    - you are attempting to disarm a monster
         * else
         *    - you are attempting to hit the monster.
         *
         * if you're confused (and thus off the mark)
         *    - you only end up hitting.
         *
         */
        const char *wrapped_what = (char *) 0;

        if (mtmp) {
            if (bigmonst(mtmp->data)) {
                wrapped_what = strcpy(buf, mon_nam(mtmp));
            } else if (proficient) {
                if (attack(mtmp))
                {
                    return 1;
                }
                else
                    pline1(msg_snap);
            }
        }
        if (!wrapped_what) {
            if (IS_FURNITURE(levl[rx][ry].typ))
                wrapped_what = something;
            else if (sobj_at(BOULDER, rx, ry))
                wrapped_what = "a boulder";
        }
        if (wrapped_what) {
            coord cc;

            cc.x = rx;
            cc.y = ry;
            You("wrap your bullwhip around %s.", wrapped_what);
            if (proficient && rn2(proficient + 2)) {
                if (!mtmp || enexto(&cc, rx, ry, youmonst.data)) {
                    You("yank yourself out of the pit!");
                    teleds(cc.x, cc.y, TRUE, FALSE);
                    reset_utrap(TRUE);
                    vision_full_recalc = 1;
                }
            } else {
                pline1(msg_slipsfree);
            }
            if (mtmp)
                wakeup(mtmp, TRUE);
        } else
            pline1(msg_snap);

    } 
    else if (mtmp) 
    {
        if (!canspotmon(mtmp) && !glyph_is_invisible(levl[rx][ry].hero_memory_layers.glyph))
        {
            pline("A monster is there that you couldn't see.");
            map_invisible(rx, ry);
        }
        otmp = MON_WEP(mtmp); /* can be null */
        if (otmp) {
            char onambuf[BUFSZ];
            const char *mon_hand;
            int gotitroll = rn2(proficient * 2 + 1);
            boolean gotit = gotitroll && (!Fumbling || !rn2(10));

            update_u_action(ACTION_TILE_ATTACK);
            play_monster_simple_weapon_sound(&youmonst, 0, obj, OBJECT_SOUND_TYPE_SWING_MELEE);
            u_wait_until_action();

            Strcpy(onambuf, cxname(otmp));
            if (gotit) {
                mon_hand = mbodypart(mtmp, HAND);
                if (bimanual(otmp))
                    mon_hand = makeplural(mon_hand);
            } else
                mon_hand = 0; /* lint suppression */

            You("wrap your bullwhip around %s.", yname(otmp));
            if (gotit && mwelded(otmp, mtmp)) {
                pline("%s welded to %s %s%c",
                      (otmp->quan == 1L) ? "It is" : "They are", mhis(mtmp),
                      mon_hand, !otmp->bknown ? '!' : '.');
                otmp->bknown = 1;
                gotit = FALSE; /* can't pull it free */
            }
            if (gotit) {
                Strcpy(debug_buf_2, "use_whip");
                obj_extract_self(otmp);
                possibly_unwield(mtmp, FALSE);
                setmnotwielded(mtmp, otmp);
                int roll = rn2(proficient + 1);
                if (roll / 2)
                {
                    /* right into your inventory */
                    You("snatch %s!", yname(otmp));
                    if (otmp->otyp == CORPSE
                        && touch_petrifies(&mons[otmp->corpsenm]) && !uarmg
                        && !Stone_resistance
                        && !(poly_when_stoned(youmonst.data)
                            && polymon(PM_STONE_GOLEM))) {
                        char kbuf[BUFSZ];

                        Sprintf(kbuf, "%s corpse",
                            an(corpse_monster_name(otmp)));
                        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Snatching %s is a fatal mistake.", kbuf);
                        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
                        instapetrify(kbuf);
                    }
                    (void)hold_another_object(otmp, "You drop %s!",
                        doname(otmp), (const char*)0);
                }
                else
                {
                    int another_roll = rn2(proficient + 2);
                    if (another_roll)
                    {
                        /* to floor near you */
                        You("yank %s to the %s!", yname(otmp),
                            surface(u.ux, u.uy));
                        place_object(otmp, u.ux, u.uy);
                        stackobj(otmp);
                    }
                    else
                    {
                        /* to floor beneath mon */
                        You("yank %s from %s %s!", the(onambuf),
                            s_suffix(mon_nam(mtmp)), mon_hand);
                        obj_no_longer_held(otmp);
                        place_object(otmp, mtmp->mx, mtmp->my);
                        stackobj(otmp);
                    }
                }
#if 0
                switch (rn2(proficient + 1)) {
                case 2:
                    /* to floor near you */
                    You("yank %s to the %s!", yname(otmp),
                        surface(u.ux, u.uy));
                    place_object(otmp, u.ux, u.uy);
                    stackobj(otmp);
                    break;
                case 3:
#if 0
                    /* right to you */
                    if (!rn2(25)) {
                        /* proficient with whip, but maybe not
                           so proficient at catching weapons */
                        int hitu, hitvalu;

                        hitvalu = 8 + otmp->enchantment;
                        hitu = thitu(hitvalu, weapon_total_dmg_value(otmp, &youmonst, mtmp, 0),
                                     &otmp, (char *)0, (struct monst*)0, (char*)0);
                        if (hitu) {
                            pline_The("%s hits you as you try to snatch it!",
                                      the(onambuf));
                        }
                        place_object(otmp, u.ux, u.uy);
                        stackobj(otmp);
                        break;
                    }
#endif /* 0 */
                    /* right into your inventory */
                    You("snatch %s!", yname(otmp));
                    if (otmp->otyp == CORPSE
                        && touch_petrifies(&mons[otmp->corpsenm]) && !uarmg
                        && !Stone_resistance
                        && !(poly_when_stoned(youmonst.data)
                             && polymon(PM_STONE_GOLEM))) {
                        char kbuf[BUFSZ];

                        Sprintf(kbuf, "%s corpse",
                                an(corpse_monster_name(otmp)));
                        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Snatching %s is a fatal mistake.", kbuf);
                        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
                        instapetrify(kbuf);
                    }
                    (void) hold_another_object(otmp, "You drop %s!",
                                               doname(otmp), (const char *) 0);
                    break;
                default:
                    /* to floor beneath mon */
                    You("yank %s from %s %s!", the(onambuf),
                        s_suffix(mon_nam(mtmp)), mon_hand);
                    obj_no_longer_held(otmp);
                    place_object(otmp, mtmp->mx, mtmp->my);
                    stackobj(otmp);
                    break;
                }
#endif /* 0 */
            } else {
                pline1(msg_slipsfree);
            }
            wakeup(mtmp, TRUE);
            update_u_action_revert(ACTION_TILE_NO_ACTION);
        } else {
            if (M_AP_TYPE(mtmp) && !Protection_from_shape_changers
                && !sensemon(mtmp))
                stumble_onto_mimic(mtmp);
            else
                You("flick your bullwhip towards %s.", mon_nam(mtmp));
            if (proficient) {
                if (attack(mtmp))
                {
                    return 1;
                }
                else
                    pline1(msg_snap);
            }
        }

    } else if (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)) {
        /* it must be air -- water checked above */
        update_u_action(ACTION_TILE_ATTACK);
        play_monster_simple_weapon_sound(&youmonst, 0, obj, OBJECT_SOUND_TYPE_SWING_MELEE);
        u_wait_until_action();

        You("snap your whip through thin air.");

        update_u_action_revert(ACTION_TILE_NO_ACTION);

    } else {
        pline1(msg_snap);
    }
    return 1;
}

STATIC_VAR const char
    not_enough_room[] = "There's not enough room here to use that.",
    where_to_hit[] = "Where do you want to hit?",
    cant_see_spot[] = "won't hit anything if you can't see that spot.",
    cant_reach[] = "can't reach that spot from here.";

/* find pos of monster in range, if only one monster */
boolean
find_poleable_mon(pos, min_range, max_range)
coord *pos;
int min_range, max_range;
{
    struct monst *mtmp;
    coord mpos;
    boolean impaired;
    int x, y, lo_x, hi_x, lo_y, hi_y, rt, glyph;

    if (Blind)
        return FALSE; /* must be able to see target location */
    impaired = (Confusion || Stunned || Hallucination);
    mpos.x = mpos.y = 0; /* no candidate location yet */
    rt = isqrt(max_range);
    lo_x = max(u.ux - rt, 1), hi_x = min(u.ux + rt, COLNO - 1);
    lo_y = max(u.uy - rt, 0), hi_y = min(u.uy + rt, ROWNO - 1);
    for (x = lo_x; x <= hi_x; ++x) {
        for (y = lo_y; y <= hi_y; ++y) {
            if (distu(x, y) < min_range || distu(x, y) > max_range
                || !isok(x, y) || !cansee(x, y))
                continue;
            glyph = glyph_at(x, y);
            if (!impaired
                && glyph_is_monster(glyph)
                && (mtmp = m_at(x, y)) != 0
                && (is_tame(mtmp) || (is_peaceful(mtmp) && flags.confirm)))
                continue;
            if (glyph_is_monster(glyph)
                || glyph_is_warning(glyph)
                || glyph_is_invisible(glyph)
                || (glyph_is_any_statue(glyph) && impaired)) 
            {
                if (mpos.x)
                    return FALSE; /* more than one candidate location */
                mpos.x = x, mpos.y = y;
            }
        }
    }
    if (!mpos.x)
        return FALSE; /* no candidate location */
    *pos = mpos;
    return TRUE;
}

STATIC_VAR int polearm_range_min = -1;
STATIC_VAR int polearm_range_max = -1;

STATIC_OVL int
get_invalid_polearm_position(x, y)
int x, y;
{
    return !isok(x, y) || !POLEARM_ACCESSIBLE(levl[x][y].typ) ? 1 :
            distu(x, y) < polearm_range_min ? 2 :
            distu(x, y) > polearm_range_max ? 3 : 0;
}

void
display_polearm_positions(state)
int state;
{
    if (state == 0) {
        tmp_at(DISP_BEAM, cmap_to_glyph(S_goodpos));
    } else if (state == 1) {
        int x, y, dx, dy;

        for (dx = -4; dx <= 4; dx++)
            for (dy = -4; dy <= 4; dy++) {
                x = dx + (int) u.ux;
                y = dy + (int) u.uy;
                if (!get_invalid_polearm_position(x, y)) {
                    tmp_at(x, y);
                }
            }
    } else {
        tmp_at(DISP_END, 0);
    }
}

/* Note that ranges are squared distances */
void
get_pole_type_weapon_min_max_distances(obj, mon, min_range, max_range)
struct obj* obj;
struct monst* mon;
int* min_range;
int* max_range;
{
    if (!obj || !mon)
        return;

    boolean is_you = (mon == &youmonst);
    int skill_level = is_you ? P_SKILL_LEVEL(objects[obj->otyp].oc_skill) : is_prince(mon->data) ? P_GRAND_MASTER : is_lord(mon->data) ? P_EXPERT : P_BASIC;

    if (is_pole(obj))
    {
        *min_range = POLEARM_MIN_DISTANCE;
        *max_range = (has_otyp_extended_polearm_reach(obj->otyp) || has_obj_mythic_reach(obj) ? POLEARM_EXTENDED_MAX_DISTANCE : POLEARM_NORMAL_MAX_DISTANCE);

    }
    else if (is_spear(obj) || is_trident(obj))
    {
        *min_range = SPEAR_MIN_DISTANCE;
        if (has_otyp_extended_polearm_reach(obj->otyp) || has_obj_mythic_reach(obj))
        {
            *max_range = POLEARM_EXTENDED_MAX_DISTANCE;
        }
        else
        {
            switch (skill_level)
            {
            case P_BASIC:
                *max_range = SPEAR_BASIC_MAX_DISTANCE;
                break;
            case P_SKILLED:
                *max_range = SPEAR_SKILLED_MAX_DISTANCE;
                break;
            case P_EXPERT:
                *max_range = SPEAR_EXPERT_MAX_DISTANCE;
                break;
            case P_MASTER:
                *max_range = SPEAR_MASTER_MAX_DISTANCE;
                break;
            case P_GRAND_MASTER:
                *max_range = SPEAR_GRAND_MASTER_MAX_DISTANCE;
                break;
            default:
                *max_range = SPEAR_UNSKILLED_MAX_DISTANCE;
                break;
            }
        }
    }
    else if (is_lance(obj))
    {
        *min_range = LANCE_MIN_DISTANCE;
        if (has_otyp_extended_polearm_reach(obj->otyp) || has_obj_mythic_reach(obj))
        {
            *max_range = POLEARM_EXTENDED_MAX_DISTANCE;
        }
        else
        {
            switch (skill_level)
            {
            case P_BASIC:
                *max_range = LANCE_BASIC_MAX_DISTANCE;
                break;
            case P_SKILLED:
                *max_range = LANCE_SKILLED_MAX_DISTANCE;
                break;
            case P_EXPERT:
                *max_range = LANCE_EXPERT_MAX_DISTANCE;
                break;
            case P_MASTER:
                *max_range = LANCE_MASTER_MAX_DISTANCE;
                break;
            case P_GRAND_MASTER :
                *max_range = LANCE_GRAND_MASTER_MAX_DISTANCE;
                break;
            default:
                *max_range = LANCE_UNSKILLED_MAX_DISTANCE;
                break;
            }
        }
    }
    else
    {
        /* Should not happen */
        *min_range = 1;
        *max_range = 2;
    }
}

//boolean
//can_pole_hit_in_melee(obj, mon, min_range, max_range)
//struct obj* obj;
//struct monst* mon;
//{
//    int min_range = 0, max_range = 0;
//    get_pole_type_weapon_min_max_distances(obj, mon, &min_range, &max_range);
//    return (min_range <= 1 && max_range >= 1);
//}

/* Distance attacks by pole-weapons */
STATIC_OVL int
use_pole(obj)
struct obj *obj;
{
    if (!obj)
        return 0;

    int res = 0, max_range = 2, min_range = 1, glyph;
    coord cc;
    struct monst *mtmp;
    struct monst *hitm = context.polearm.hitmon;

    /* Are you allowed to use the pole? */
    if (u.uswallow) {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, not_enough_room);
        return 0;
    }
    if (obj != uwep) {
        if (!wield_tool(obj, "swing"))
            return 0;
        else
            res = 1;
    }
    /* assert(obj == uwep); */

    /*
     * Calculate allowable range (pole's reach is always 2 steps):
     *  unskilled and basic: orthogonal direction, 4..4;
     *  skilled: as basic, plus knight's jump position, 4..5;
     *  expert: as skilled, plus diagonal, 4..8.
     *      ...9...
     *      .85458.
     *      .52125.
     *      9410149
     *      .52125.
     *      .85458.
     *      ...9...
     *  (Note: no roles in GnollHack can become expert or better
     *  for polearm skill; Yeoman in slash'em can become expert.)
     */

    get_pole_type_weapon_min_max_distances(obj, &youmonst, &min_range, &max_range);
    //min_range = POLEARM_MIN_DISTANCE;
    //max_range = (has_otyp_extended_polearm_reach(obj->otyp) ? POLEARM_EXTENDED_MAX_DISTANCE : POLEARM_NORMAL_MAX_DISTANCE);

#if 0
    typ = uwep_skill_type();
    if (typ == P_NONE || P_SKILL_LEVEL(typ) <= P_BASIC)
        max_range = 4;
    else if (P_SKILL_LEVEL(typ) == P_SKILLED)
        max_range = 5;
    else
        max_range = 8; /* (P_SKILL_LEVEL(typ) >= P_EXPERT) */
#endif

    polearm_range_min = min_range;
    polearm_range_max = max_range;

    /* Prompt for a location */
    pline(where_to_hit);
    cc.x = u.ux;
    cc.y = u.uy;
    if (!find_poleable_mon(&cc, min_range, max_range) && hitm
        && !DEADMONSTER(hitm) && couldsee(hitm->mx, hitm->my) && canspotmon(hitm)
        && distu(hitm->mx, hitm->my) <= max_range
        && distu(hitm->mx, hitm->my) >= min_range) {
        cc.x = hitm->mx;
        cc.y = hitm->my;
    }
    getpos_sethilite(display_polearm_positions, get_invalid_polearm_position);
    if (getpos(&cc, TRUE, "the spot to hit", CURSOR_STYLE_POLEARM_CURSOR) < 0)
        return res; /* ESC; uses turn iff polearm became wielded */

    glyph = glyph_at(cc.x, cc.y);
    if (distu(cc.x, cc.y) > max_range)
    {
        play_sfx_sound(SFX_GENERAL_TOO_FAR);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Too far!");
        return res;
    } 
    else if (distu(cc.x, cc.y) < min_range) 
    {
        play_sfx_sound(SFX_GENERAL_TOO_CLOSE);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Too close!");
        return res;
    } 
    else if (!cansee(cc.x, cc.y) && !glyph_is_monster(glyph)
               && !glyph_is_invisible(glyph) && !glyph_is_any_statue(glyph))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT_SEE_SPOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, cant_see_spot);
        return res;
    } 
    else if (!couldsee(cc.x, cc.y)) 
    { /* Eyes of the Overworld */
        play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
        You_ex(ATR_NONE, CLR_MSG_FAIL, cant_reach);
        return res;
    }

    context.polearm.hitmon = (struct monst *) 0;
    /* Attack the monster there */
    bhitpos = cc;
    uchar hitres = 0;
    if ((mtmp = m_at(bhitpos.x, bhitpos.y)) != (struct monst *) 0) 
    {
        if (attack_checks(mtmp, uwep))
            return res;
        if (overexertion())
            return 1; /* burn nutrition; maybe pass out */

        play_monster_simple_weapon_sound(&youmonst, 0, obj, OBJECT_SOUND_TYPE_SWING_MELEE);
        if(dist2(u.ux, u.uy, bhitpos.x, bhitpos.y) > 0)
            display_gui_effect(GUI_EFFECT_POLEARM, is_spear(obj) ? GUI_POLEARM_SPEAR : is_lance(obj) ? GUI_POLEARM_LANCE : is_trident(obj) ? GUI_POLEARM_SPEAR : GUI_POLEARM_THRUSTED, u.ux, u.uy, bhitpos.x, bhitpos.y, 0UL);
        context.polearm.hitmon = mtmp;
        check_caitiff(mtmp);
        notonhead = (bhitpos.x != mtmp->mx || bhitpos.y != mtmp->my);
        (void) thitmonst(mtmp, uwep, FALSE, &hitres);
    } 
    else if (glyph_is_any_statue(glyph) /* might be hallucinatory */
               && sobj_at(STATUE, bhitpos.x, bhitpos.y)) 
    {
        play_monster_simple_weapon_sound(&youmonst, 0, obj, OBJECT_SOUND_TYPE_SWING_MELEE);
        if (dist2(u.ux, u.uy, bhitpos.x, bhitpos.y) > 0)
            display_gui_effect(GUI_EFFECT_POLEARM, is_spear(obj) ? GUI_POLEARM_SPEAR : is_lance(obj) ? GUI_POLEARM_LANCE : is_trident(obj) ? GUI_POLEARM_SPEAR : GUI_POLEARM_THRUSTED, u.ux, u.uy, bhitpos.x, bhitpos.y, 0UL);

        struct trap *t = t_at(bhitpos.x, bhitpos.y);

        if (t && t->ttyp == STATUE_TRAP
            && activate_statue_trap(t, t->tx, t->ty, FALSE)) 
        {
            ; /* feedback has been give by animate_statue() */
        } 
        else 
        {
            /* Since statues look like monsters now, we say something
               different from "you miss" or "there's nobody there".
               Note:  we only do this when a statue is displayed here,
               because the player is probably attempting to attack it;
               other statues obscured by anything are just ignored. */
            play_monster_simple_weapon_sound(&youmonst, 0, obj, OBJECT_SOUND_TYPE_GENERAL_EFFECT);
            pline("Thump!  Your blow bounces harmlessly off the statue.");
            wake_nearto(bhitpos.x, bhitpos.y, 25);
        }
    } 
    else 
    {
        play_monster_simple_weapon_sound(&youmonst, 0, obj, OBJECT_SOUND_TYPE_SWING_MELEE);
        if (dist2(u.ux, u.uy, bhitpos.x, bhitpos.y) > 0)
            display_gui_effect(GUI_EFFECT_POLEARM, is_spear(obj) ? GUI_POLEARM_SPEAR : is_lance(obj) ? GUI_POLEARM_LANCE : is_trident(obj) ? GUI_POLEARM_SPEAR : GUI_POLEARM_THRUSTED, u.ux, u.uy, bhitpos.x, bhitpos.y, 0UL);

        /* no monster here and no statue seen or remembered here */
        (void) unmap_invisible(bhitpos.x, bhitpos.y);
        You("miss; there is no one there to hit.");
    }
    u_wipe_engr(2); /* same as for melee or throwing */
    return 1;
}

STATIC_OVL int
use_cream_pie(obj)
struct obj *obj;
{
    boolean wasblind = Blind;
    boolean wascreamed = u.ucreamed;
    boolean several = FALSE;

    if (obj->quan > 1L) {
        several = TRUE;
        obj = splitobj(obj, 1L);
    }
    if (Hallucination)
        You("give yourself a facial.");
    else
        pline("You immerse your %s in %s%s.", body_part(FACE),
              several ? "one of " : "",
              several ? makeplural(the(xname(obj))) : the(xname(obj)));
    if (can_blnd((struct monst *) 0, &youmonst, AT_WEAP, obj)) {
        int blindinc = rnd(25);
        u.ucreamed += blindinc;
        make_blinded((Blinded& TIMEOUT) + (int64_t) blindinc, FALSE);
        if (!Blind || (Blind && wasblind))
            pline("There's %ssticky goop all over your %s.",
                  wascreamed ? "more " : "", body_part(FACE));
        else /* Blind  && !wasblind */
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "see through all the sticky goop on your %s.",
                     body_part(FACE));
    }

    setnotworn(obj);
    /* useup() is appropriate, but we want costly_alteration()'s message */
    costly_alteration(obj, COST_SPLAT);
    Strcpy(debug_buf_2, "use_cream_pie");
    obj_extract_self(obj);
    delobj(obj);
    return 0;
}

STATIC_OVL int
use_watch(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    if(midnight())
        You("check your watch. It is midnight!");
    else if (night())
        You("check your watch. It is nighttime!");
    else
        You("check your watch. It is daytime.");

    return 0;
}

STATIC_OVL int
use_grapple(obj)
struct obj *obj;
{
    int res = 0, max_range = 4, tohit;
    enum p_skills typ;
    boolean save_confirm;
    coord cc;
    struct monst *mtmp;
    struct obj *otmp;

    /* Are you allowed to use the hook? */
    if (u.uswallow) {
        pline(not_enough_room);
        return 0;
    }
    if (obj != uwep) {
        if (!wield_tool(obj, "cast"))
            return 0;
        else
            res = 1;
    }
    /* assert(obj == uwep); */

    /* Prompt for a location */
    pline(where_to_hit);
    cc.x = u.ux;
    cc.y = u.uy;
    if (getpos(&cc, TRUE, "the spot to hit", CURSOR_STYLE_GRAPPLE_CURSOR) < 0)
        return res; /* ESC; uses turn iff grapnel became wielded */

    /* Calculate range; unlike use_pole(), there's no minimum for range */
    typ = uwep_skill_type();
    if (typ == P_NONE || P_SKILL_LEVEL(typ) <= P_BASIC)
        max_range = 4;
    else if (P_SKILL_LEVEL(typ) == P_SKILLED)
        max_range = 5;
    else
        max_range = 8;
    if (distu(cc.x, cc.y) > max_range) {
        play_sfx_sound(SFX_GENERAL_TOO_FAR);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Too far!");
        return res;
    } else if (!cansee(cc.x, cc.y)) {
        play_sfx_sound(SFX_GENERAL_CANNOT_SEE_SPOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, cant_see_spot);
        return res;
    } else if (!couldsee(cc.x, cc.y)) { /* Eyes of the Overworld */
        play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
        You_ex(ATR_NONE, CLR_MSG_FAIL, cant_reach);
        return res;
    }

    /* What do you want to hit? */
    tohit = rn2(5);
    if (typ != P_NONE && P_SKILL_LEVEL(typ) >= P_SKILLED) {
        winid tmpwin = create_nhwindow(NHW_MENU);
        anything any;
        char buf[BUFSZ];
        menu_item *selected;

        any = zeroany; /* set all bits to zero */
        any.a_int = 1; /* use index+1 (cant use 0) as identifier */
        start_menu_ex(tmpwin, GHMENU_STYLE_CHOOSE_COMMAND);
        any.a_int++;
        Sprintf(buf, "an object on the %s", surface(cc.x, cc.y));
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                 MENU_UNSELECTED);
        any.a_int++;
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "a monster",
                 MENU_UNSELECTED);
        any.a_int++;
        Sprintf(buf, "the %s", surface(cc.x, cc.y));
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                 MENU_UNSELECTED);
        end_menu(tmpwin, "Aim for what?");
        tohit = rn2(4);
        if (select_menu(tmpwin, PICK_ONE, &selected) > 0
            && rn2(P_SKILL_LEVEL(typ) > P_SKILLED ? 20 : 2))
            tohit = selected[0].item.a_int - 1;
        free((genericptr_t) selected);
        destroy_nhwindow(tmpwin);
    }

    /* possibly scuff engraving at your feet;
       any engraving at the target location is unaffected */
    if (tohit == 2 || !rn2(2))
        u_wipe_engr(rnd(2));

    /* What did you hit? */
    uchar hitres = 0;
    switch (tohit) {
    case 0: /* Trap */
        /* FIXME -- untrap needs to deal with non-adjacent traps */
        break;
    case 1: /* Object */
        if ((otmp = level.objects[cc.x][cc.y]) != 0) {
            You("snag an object from the %s!", surface(cc.x, cc.y));
            (void) pickup_object(otmp, 1L, FALSE, FALSE, (uchar*)0);
            /* If pickup fails, leave it alone */
            newsym(cc.x, cc.y);
            return 1;
        }
        break;
    case 2: /* Monster */
        bhitpos = cc;
        if ((mtmp = m_at(cc.x, cc.y)) == (struct monst *) 0)
            break;
        notonhead = (bhitpos.x != mtmp->mx || bhitpos.y != mtmp->my);
        save_confirm = flags.confirm;
        if (verysmall(mtmp->data) && !rn2(4)
            && enexto(&cc, u.ux, u.uy, (struct permonst *) 0)) {
            flags.confirm = FALSE;
            (void) attack_checks(mtmp, uwep);
            flags.confirm = save_confirm;
            check_caitiff(mtmp); /* despite fact there's no damage */
            You("pull in %s!", mon_nam(mtmp));
            mtmp->mundetected = 0;
            rloc_to(mtmp, cc.x, cc.y);
            return 1;
        } else if ((!bigmonst(mtmp->data) && !strongmonst(mtmp->data))
                   || rn2(4)) {
            flags.confirm = FALSE;
            (void) attack_checks(mtmp, uwep);
            flags.confirm = save_confirm;
            check_caitiff(mtmp);
            (void) thitmonst(mtmp, uwep, FALSE, &hitres);
            return 1;
        }
    /*FALLTHRU*/
    case 3: /* Surface */
        if (IS_AIR(levl[cc.x][cc.y].typ) || is_pool(cc.x, cc.y))
            pline_The("hook slices through the %s.", surface(cc.x, cc.y));
        else {
            You("are yanked toward the %s!", surface(cc.x, cc.y));
            hurtle(sgn(cc.x - u.ux), sgn(cc.y - u.uy), 1, FALSE);
            spoteffects(TRUE);
        }
        return 1;
    default: /* Yourself (oops!) */
        if (P_SKILL_LEVEL(typ) <= P_BASIC) {
            You("hook yourself!");
            losehp(adjust_damage(rn1(10, 10), &youmonst, &youmonst, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE), "a grappling hook",
                   KILLED_BY);
            return 1;
        }
        break;
    }
    pline1(nothing_happens);
    return 1;
}

#define BY_OBJECT ((struct monst *) 0)

/* return 1 if the wand is broken, hence some time elapsed */
STATIC_OVL int
do_break_wand(obj)
struct obj *obj;
{
    static const char nothing_else_happens[] = "But nothing else happens...";
    register int i, x, y;
    register struct monst *mon;
    int dmg_n = 0, dmg_d = 6;
    double damage = 0;
    boolean affects_objects;
    boolean shop_damage = FALSE;
    boolean fillmsg = FALSE;
    int expltype = EXPL_MAGICAL;
    char confirm[QBUFSZ], buf[BUFSZ];
    boolean is_fragile = (!strcmp(OBJ_DESCR(objects[obj->otyp]), "balsa"));
    context.bhitcount = 0;

    if (!paranoid_query(ParanoidBreak,
                       safe_qbuf(confirm,
                                 "Are you really sure you want to break ",
                                 "?", obj, yname, ysimple_name, "the wand")))
        return 0;

    if (nohands(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "break %s without hands!", yname(obj));
        return 0;
    } else if (ACURR(A_STR) < (is_fragile ? 5 : 10)) {
        You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have the strength to break %s!", yname(obj));
        return 0;
    } else if (obj->oartifact > 0) {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is too hard to break!", Yname2(obj));
        return 0;
    }

    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BREAK);
    pline("Raising %s high above your %s, you %s it in two!", yname(obj),
          body_part(HEAD), is_fragile ? "snap" : "break");

    /* [ALI] Do this first so that wand is removed from bill. Otherwise,
     * the freeinv() below also hides it from setpaid() which causes problems.
     */
    if (obj->unpaid) {
        check_unpaid(obj); /* Extra charge for use */
        costly_alteration(obj, COST_DSTROY);
    }

    current_wand = obj; /* destroy_item might reset this */
    freeinv(obj);       /* hide it from destroy_item instead... */
    setnotworn(obj);    /* so we need to do this ourselves */

    if (!zappable(obj)) {
        pline(nothing_else_happens);
        goto discard_broken_wand;
    }
    /* successful call to zappable() consumes a charge; put it back */
    obj->charges++;
    /* might have "wrested" a final charge, taking it from 0 to -1;
       if so, we just brought it back up to 0, which wouldn't do much
       below so give it 1..3 charges now, usually making it stronger
       than an ordinary last charge (the wand is already gone from
       inventory, so perm_invent can't accidentally reveal this) */
    if (!obj->charges)
        obj->charges = rnd(3);

    obj->ox = u.ux;
    obj->oy = u.uy;
    dmg_n = obj->charges;
    affects_objects = FALSE;
    obj->speflags |= SPEFLAGS_BEING_BROKEN;

    uchar hit_only_one = 1;
    if (objects[obj->otyp].oc_dir == IMMEDIATE_MULTIPLE_TARGETS)
        hit_only_one = 0;
    else if (objects[obj->otyp].oc_dir == IMMEDIATE_ONE_TO_THREE_TARGETS)
        hit_only_one = 2; /* 1- 3 targets based on BUC status */
    else if (objects[obj->otyp].oc_dir == IMMEDIATE_ONE_TO_SEVEN_TARGETS)
        hit_only_one = 3; /* 1- 7 targets based on BUC status */
    else if (objects[obj->otyp].oc_dir == IMMEDIATE_TWO_TO_SIX_TARGETS)
        hit_only_one = 4; /* 2- 6 targets based on BUC status */

    switch (obj->otyp) {
    case WAN_WISHING:
    case WAN_IDENTIFY:
    case WAN_NOTHING:
    case WAN_LOCKING:
    case WAN_PROBING:
    case WAN_ENLIGHTENMENT:
    case WAN_OPENING:
    case WAN_UNDEAD_TURNING:
    case WAN_SECRET_DOOR_DETECTION:
    case WAN_TRAP_DETECTION:
    case WAN_ORE_DETECTION:
    case WAN_TOWN_PORTAL:
        pline(nothing_else_happens);
        goto discard_broken_wand;
    case WAN_DEATH:
    case WAN_DISINTEGRATION:
    case WAN_LIGHTNING:
        dmg_n *= 4;
        goto wanexpl;
    case WAN_FIRE:
        expltype = EXPL_FIERY;
        /*FALLTHRU*/
    case WAN_COLD:
        if (expltype == EXPL_MAGICAL)
            expltype = EXPL_FROSTY;
        dmg_n *= 2;
        /*FALLTHRU*/
    case WAN_MAGIC_MISSILE:
    wanexpl:
        explode(u.ux, u.uy, objects[obj->otyp].oc_dir_subtype, &youmonst, dmg_n, dmg_d, 0, obj->otyp, WAND_CLASS, expltype);
        makeknown(obj->otyp); /* explode describes the effect */
        goto discard_broken_wand;
    case WAN_STRIKING:
        /* we want this before the explosion instead of at the very end */
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A wall of force smashes down around you!");
        dmg_n = 1 + obj->charges;
        dmg_d = 6; /* normally 2d12 */
        goto wanexpl;
    case WAN_DISJUNCTION:
    case WAN_CANCELLATION:
    case WAN_POLYMORPH:
    case WAN_TELEPORTATION:
    case WAN_RESURRECTION:
        affects_objects = TRUE;
        break;
    default:
        break;
    }

    /* magical explosion and its visual effect occur before specific effects
     */
    /* [TODO?  This really ought to prevent the explosion from being
       fatal so that we never leave a bones file where none of the
       surrounding targets (or underlying objects) got affected yet.] */
    explode(obj->ox, obj->oy, 
        objects[obj->otyp].oc_dir == RAY && objects[obj->otyp].oc_dir_subtype <= RAY_WND_PETRIFICATION ? objects[obj->otyp].oc_dir_subtype : 0, 
        &youmonst, dmg_n, dmg_d, 0, obj->otyp, WAND_CLASS,
            EXPL_MAGICAL);

    /* prepare for potential feedback from polymorph... */
    zapsetup();

    /* this makes it hit us last, so that we can see the action first */
    for (i = 0; i <= 8; i++) {
        bhitpos.x = x = obj->ox + xdir[i];
        bhitpos.y = y = obj->oy + ydir[i];
        if (!isok(x, y))
            continue;

        if (obj->otyp == WAN_DIGGING) {
            schar typ;

            if (dig_check(BY_OBJECT, FALSE, x, y)) {
                if (IS_WALL(levl[x][y].typ) || IS_DOOR(levl[x][y].typ)) {
                    /* normally, pits and holes don't anger guards, but they
                     * do if it's a wall or door that's being dug */
                    watch_dig((struct monst *) 0, x, y, TRUE);
                    if (*in_rooms(x, y, SHOPBASE))
                        shop_damage = TRUE;
                }
                /*
                 * Let liquid flow into the newly created pits.
                 * Adjust corresponding code in music.c for
                 * drum of earthquake if you alter this sequence.
                 */
                typ = fillholetyp(x, y, FALSE);
                if (!IS_FLOOR(typ)) {
                    levl[x][y].typ = typ, levl[x][y].flags = 0;
                    liquid_flow(x, y, typ, t_at(x, y),
                                fillmsg
                                  ? (char *) 0
                                  : "Some holes are quickly filled with %s!");
                    fillmsg = TRUE;
                } else
                    digactualhole(x, y, BY_OBJECT, (rn2(obj->charges) < 3
                                                    || (!Can_dig_down(&u.uz)
                                                        && !levl[x][y].candig))
                                                      ? PIT
                                                      : HOLE);
            }
            continue;
        } else if (obj->otyp == WAN_CREATE_MONSTER) {
            /* u.ux,u.uy creates it near you--x,y might create it in rock */
            (void) makemon2((struct permonst *) 0, u.ux, u.uy, NO_MM_FLAGS, MM2_RANDOMIZE_SUBTYPE);
            continue;
        } else if (x != u.ux || y != u.uy) {
            /*
             * Wand breakage is targetting a square adjacent to the hero,
             * which might contain a monster or a pile of objects or both.
             * Handle objects last; avoids having undead turning raise an
             * undead's corpse and then attack resulting undead monster.
             * obj->bypass in bhitm() prevents the polymorphing of items
             * dropped due to monster's polymorph and prevents undead
             * turning that kills an undead from raising resulting corpse.
             */
            if ((mon = m_at(x, y)) != 0) {
                (void) bhitm(mon, obj, (struct monst*)0);
                /* if (context.botl) bot(); */
            }
            if (affects_objects && level.objects[x][y]) {
                (void) bhitpile(obj, (struct monst*)0, bhito, x, y, 0, hit_only_one, FALSE);
                if (context.botl)
                    bot(); /* potion effects */
            }
        } else {
            /*
             * Wand breakage is targetting the hero.  Using xdir[]+ydir[]
             * deltas for location selection causes this case to happen
             * after all the surrounding squares have been handled.
             * Process objects first, in case damage is fatal and leaves
             * bones, or teleportation sends one or more of the objects to
             * same destination as hero (lookhere/autopickup); also avoids
             * the polymorphing of gear dropped due to hero's transformation.
             * (Unlike with monsters being hit by zaps, we can't rely on use
             * of obj->bypass in the zap code to accomplish that last case
             * since it's also used by retouch_equipment() for polyself.)
             */
            if (affects_objects && level.objects[x][y]) {
                (void) bhitpile(obj, (struct monst*)0, bhito, x, y, 0, hit_only_one, FALSE);
                if (context.botl)
                    bot(); /* potion effects */
            }
            damage = zapyourself(obj, FALSE);
            if (damage > 0) 
            {
                Sprintf(buf, "killed %sself by breaking a wand", uhim());
                losehp(damage, buf, NO_KILLER_PREFIX);
            }
            if (context.botl)
                bot(); /* blindness */
        }
    }

    /* potentially give post zap/break feedback */
    zapwrapup();

    /* Note: if player fell thru, this call is a no-op.
       Damage is handled in digactualhole in that case */
    if (shop_damage)
        pay_for_damage("dig into", FALSE);

    if (obj->otyp == WAN_LIGHT)
        litroom(TRUE, obj); /* only needs to be done once */

discard_broken_wand:
    obj = current_wand; /* [see dozap() and destroy_item()] */
    current_wand = 0;
    if (obj)
        delobj(obj);
    nomul(0);
    return 1;
}

STATIC_OVL void
add_class(cl, class)
char *cl;
char class;
{
    char tmp[2];

    tmp[0] = class;
    tmp[1] = '\0';
    Strcat(cl, tmp);
}

STATIC_VAR const char tools[] = { TOOL_CLASS, WEAPON_CLASS, WAND_CLASS, 0 };

/* augment tools[] if various items are carried */
void
setapplyclasses(class_list)
char class_list[];
{
    register struct obj *otmp;
    int otyp;
    boolean knowoil, knowtouchstone, addpotions, addstones, addfood, addmisc;

    knowoil = objects[POT_OIL].oc_name_known;
    knowtouchstone = objects[TOUCHSTONE].oc_name_known;
    addpotions = addstones = addfood = addmisc = FALSE;
    for (otmp = invent; otmp; otmp = otmp->nobj) {
        otyp = otmp->otyp;
        if (otyp == POT_OIL
            || (otmp->oclass == POTION_CLASS
                && (!otmp->dknown
                    || (!knowoil && !objects[otyp].oc_name_known))))
            addpotions = TRUE;
        if (otyp == TOUCHSTONE
            || (is_graystone(otmp)
                && (!otmp->dknown
                    || (!knowtouchstone && !objects[otyp].oc_name_known))))
            addstones = TRUE;
        if (otyp == CREAM_PIE || otyp == EUCALYPTUS_LEAF)
            addfood = TRUE;
        if (otmp->oclass == MISCELLANEOUS_CLASS && is_obj_appliable(otmp))
            addmisc = TRUE;
    }

    class_list[0] = '\0';
    if (addpotions || addstones)
        add_class(class_list, ALL_CLASSES);
    Strcat(class_list, tools);
    if (addpotions)
        add_class(class_list, POTION_CLASS);
    if (addstones)
        add_class(class_list, GEM_CLASS);
    if (addfood)
        add_class(class_list, FOOD_CLASS);
    if (addmisc)
        add_class(class_list, MISCELLANEOUS_CLASS);
}


void
setbreakclasses(class_list)
char* class_list;
{
    register struct obj* otmp;

    for (otmp = invent; otmp; otmp = otmp->nobj) {
        if ((breaktest(otmp) || otmp->oclass == WAND_CLASS) && !strchr(class_list, otmp->oclass))
            add_class(class_list, otmp->oclass);
    }
}

/* the C('b') command */
int
dobreak(VOID_ARGS)
{
    int res = 1;
    char class_list[MAX_OBJECT_CLASSES + 2];
    Strcpy(class_list, "");

    //Cannot break when overloaded?
    if (check_capacity((char*)0))
        return 0;

    setbreakclasses(class_list);

    struct obj* obj = getobj(class_list, "break", 0, "");
    if (!obj)
        return 0;

    if (!retouch_object(&obj, FALSE))
        return 1; /* evading your grasp costs a turn; just be
                     grateful that you don't drop it as well */

    if (obj->oclass == WAND_CLASS)
        do_break_wand(obj);
    else if (breaktest(obj))
    {
        char confirm[QBUFSZ];

        if (!paranoid_query(ParanoidBreak, //Use this wall all breaking
            safe_qbuf(confirm,
                "Are you really sure you want to break ",
                "?", obj, yname, ysimple_name, "the item")))
            return 0;

        if (nohands(youmonst.data)) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "break %s without hands!", yname(obj));
            return 0;
        }
        res = hero_breaks(obj, u.ux, u.uy, TRUE);
    }
    else
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot break that!");
    }

    return res; 
}

/* the 'a' command */
int
doapply(VOID_ARGS)
{
    return doapply_core(0);
}

/* the 'b' command */
int
dotakeitemsout(VOID_ARGS)
{
    return doapply_core(1);
}

/* the 'B' command */
int
doputitemsin(VOID_ARGS)
{
    return doapply_core(2);
}

STATIC_OVL int
doapply_core(applymode)
int applymode; /* 0 = normal, 1 = take out items, 2 = put in items */
{
    struct obj *obj;
    register int res = 1;
    char class_list[MAX_OBJECT_CLASSES + 2];

    if (check_capacity((char *) 0))
        return 0;

    if (!applymode)
    {
        int fres = 0;
        if ((fres = floorapply()) > -1) /* -1 = nothing to floor apply or selected not to do it */
        {
            return fres;
        }
    }

    setapplyclasses(class_list); /* tools[] */
    const char* getobjverb = applymode == 1 ? "take items out of" : applymode == 2 ? "put items in" : "use or apply";
    obj = getobj(class_list, getobjverb, 0, "");
    if (!obj)
        return 0;

    if (applymode && !Is_container(obj))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "cannot %s %s.", getobjverb, thecxname(obj));
        return 0;
    }

    if (obj->cooldownleft > 0)
    {
        play_sfx_sound(SFX_NOT_READY_YET);
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "cannot apply %s before its cooldown has expired.", thecxname(obj));
        return 0;
    }

    if (!retouch_object(&obj, FALSE))
        return 1; /* evading your grasp costs a turn; just be
                     grateful that you don't drop it as well */

    if (obj->oclass == WAND_CLASS)
    {
        return use_wand_on_object(obj); // do_break_wand(obj);
    }

    if (Is_proper_container(obj))
    {
        /* Regular containers */
        res = use_container(&obj, 1, FALSE, applymode);
    }
    else if (is_key(obj))
    {
        res = (pick_lock(obj) != 0);
    }
    else if (is_obj_candelabrum(obj))
    {
        use_candelabrum(obj);
    }
    else
    {
        /* All others */
        switch (obj->otyp)
        {
        case BLINDFOLD:
            if (obj == ublindf)
            {
                if (!cursed(obj, FALSE))
                    Blindf_off(obj);
            }
            else if (!ublindf)
            {
                Blindf_on(obj);
            }
            else
            {
                You("are already %s.", ublindf->otyp == TOWEL
                    ? "covered by a towel"
                    : "wearing a blindfold");
            }
            break;
        case CREAM_PIE:
            res = use_cream_pie(obj);
            break;
        case EXPENSIVE_WATCH:
            res = use_watch(obj);
            break;
        case BULLWHIP:
            res = use_whip(obj);
            break;
        case GRAPPLING_HOOK:
            res = use_grapple(obj);
            break;
        case BAG_OF_TRICKS:
            (void)bagotricks(obj, FALSE, (int*)0);
            break;
        case POUCH_OF_ENDLESS_BOLTS:
            (void)endlessarrows(obj, CROSSBOW_BOLT, rnd(10) + 10);
            break;
        case BAG_OF_INFINITE_SLING_BULLETS:
            (void)endlessarrows(obj, SLING_BULLET, rnd(10) + 10);
            break;
        case QUIVER_OF_INFINITE_ARROWS:
            (void)endlessarrows(obj, ARROW, rnd(10) + 10);
            break;
        case CAN_OF_GREASE:
            use_grease(obj);
            break;
        case LOCK_PICK:
        case CREDIT_CARD:
        case SKELETON_KEY:
            res = (pick_lock(obj) != 0);
            break;
        case PICK_AXE:
        case DWARVISH_MATTOCK:
        case SPADE_OF_COLOSSAL_EXCAVATION:
        case SAW_OF_MIGHTY_CUTTING:
            res = use_pick_axe(obj);
            break;
        case GOLF_CLUB:
            res = use_golf_club(obj);
            break;
        case TINNING_KIT:
            use_tinning_kit(obj);
            break;
        case LEASH:
            res = use_leash(obj);
            break;
        case SADDLE:
            res = use_saddle(obj);
            break;
        case MAGIC_WHISTLE:
            use_magic_whistle(obj);
            break;
        case TIN_WHISTLE:
            use_whistle(obj);
            break;
        case EUCALYPTUS_LEAF:
            /* MRKR: Every Australian knows that a gum leaf makes an excellent
             * whistle, especially if your pet is a tame kangaroo named Skippy.
             */
            if (obj->blessed)
            {
                use_magic_whistle(obj);
                /* sometimes the blessing will be worn off */
                if (!rn2(49))
                {
                    play_sfx_sound(SFX_AURA_GLOW);
                    if (!Blind)
                    {
                        pline_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "%s %s.", Yobjnam2(obj, "glow"), hcolor_multi_buf1(NH_BROWN));
                        obj->bknown = 1;
                    }
                    unbless(obj);
                    update_inventory();
                }
            }
            else
            {
                use_whistle(obj);
            }
            break;
        case STETHOSCOPE:
            res = use_stethoscope(obj);
            break;
        case MIRROR:
        case MAGIC_MIRROR:
            res = use_mirror(obj);
            break;
        case HOLY_SYMBOL:
            res = use_holysymbol(obj);
            break;
        case BELL:
        case BELL_OF_OPENING:
            use_bell(&obj);
            break;
        case CANDELABRUM_OF_INVOCATION:
            use_candelabrum(obj);
            break;
        case WAX_CANDLE:
        case TALLOW_CANDLE:
        case MAGIC_CANDLE:
            res = use_candle(&obj);
            break;
        case TORCH:
            res = use_torch(&obj);
            break;
        case OIL_LAMP:
        case MAGIC_LAMP:
        case BRASS_LANTERN:
            use_lamp(obj);
            break;
        case POT_OIL:
            res = use_oil(obj);
            break;
        case EXPENSIVE_CAMERA:
            res = use_camera(obj);
            break;
        case TOWEL:
            res = use_towel(obj);
            break;
        case CRYSTAL_BALL:
            use_crystal_ball(&obj);
            break;
        case MAGIC_MARKER:
            res = dowrite(obj);
            break;
        case TIN_OPENER:
            res = use_tin_opener(obj);
            break;
        case FIGURINE:
            use_figurine(&obj);
            break;
        case UNICORN_HORN:
            use_unicorn_horn(obj);
            break;
        case WOODEN_FLUTE:
        case MAGIC_FLUTE:
        case BRASS_HORN:
        case TOOLED_HORN:
        case FROST_HORN:
        case FIRE_HORN:
        case HORN_OF_CHAOS:
        case WOODEN_HARP:
        case MAGIC_HARP:
        case BUGLE:
        case LEATHER_DRUM:
        case DRUM_OF_EARTHQUAKE:
            res = do_play_instrument(obj);
            break;
        case JAR_OF_EXTRA_HEALING_SALVE:
        case JAR_OF_GREATER_HEALING_SALVE:
        case JAR_OF_PRODIGIOUS_HEALING_SALVE:
        case JAR_OF_MEDICINAL_SALVE:
        case JAR_OF_BASILISK_BLOOD:
            res = use_salve(obj, FALSE);
            break;
        case GRAIL_OF_HEALING:
            res = use_grail(obj, FALSE);
            break;
        case HORN_OF_PLENTY: /* not a musical instrument */
            (void)hornoplenty(obj, FALSE);
            break;
        case LAND_MINE:
        case BEARTRAP:
            use_trap(obj);
            break;
        case FLINT:
        case LUCKSTONE:
        case LOADSTONE:
        case JINXSTONE:
        case TOUCHSTONE:
            use_stone(obj);
            break;
        case CUBIC_GATE:
            res = use_cubic_gate(obj);
            break;
        case DIABOLICAL_SCEPTRE:
        {
            obj->special_quality++;
            if (obj->special_quality > 2)
                obj->special_quality = 0;

            const char* glowcolor = "gray";
            switch (obj->special_quality)
            {
            case 0:
                glowcolor = "blue";
                break;
            case 1:
                glowcolor = "white";
                break;
            case 2:
                glowcolor = "red";
                break;
            default:
                break;
            }
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "The head of %s rotates.", yname(obj));
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s.", Tobjnam(obj, "glow"), glowcolor);
            break;
        }
        default:
            /* Pole-type-weapons can strike at a distance */
            if (is_appliable_pole_type_weapon(obj))
            {
                res = use_pole(obj);
                break;
            }
            else if (is_pick(obj) || is_axe(obj) || is_saw(obj))
            {
                res = use_pick_axe(obj);
                break;
            }
            play_sfx_sound(SFX_GENERAL_DO_NOT_KNOW_HOW);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "Sorry, I don't know how to use that.");
            nomul(0);
            return 0;
        }
    }

    if (res && obj && obj->oartifact)
        arti_speak(obj);
    nomul(0);
    return res;
}

int
count_other_containers(objchain, this_container, last_container_ptr, usenexthere)
struct obj* objchain;
struct obj* this_container;
struct obj** last_container_ptr;
boolean usenexthere;
{
    if (!objchain)
    {
        *last_container_ptr = (struct obj*)0;
        return 0;
    }
    
    int cnt = 0;
    for (struct obj* otmp = objchain; otmp; otmp = usenexthere ? otmp->nexthere : otmp->nobj)
    {
        if ((Is_proper_container(otmp) || (Is_container(otmp) && !objects[otmp->otyp].oc_name_known)) && otmp != this_container && !otmp->olocked)
        {
            cnt++;
            *last_container_ptr = otmp;
        }
    }

    return cnt;
}

struct obj*
select_other_container(objchain, this_container, usenexthere)
struct obj* objchain;
struct obj* this_container;
boolean usenexthere;
{
    if (!objchain)
    {
        return (struct obj*)0;
    }

    int n = 0;
    winid win;
    
    win = create_nhwindow(NHW_MENU);
    start_menu_ex(win, GHMENU_STYLE_PICK_ITEM_LIST_AUTO_OK);
    int cnt = 0;

    for (struct obj* otmp = objchain; otmp; otmp = usenexthere ? otmp->nexthere : otmp->nobj)
    {
        if (cnt >= 52)
            break;

        if ((Is_proper_container(otmp) || (Is_container(otmp) && !objects[otmp->otyp].oc_name_known)) && otmp != this_container && !otmp->olocked)
        {
            anything any = zeroany;
            any.a_obj = otmp;
            char applied_invlet = usenexthere ? 'a' + cnt :  otmp->invlet;
            char applied_group_accelerator = 0; // def_oc_syms[(int)objects[otmp->otyp].oc_class].sym;

            xchar x = 0, y = 0;
            get_obj_location(otmp, &x, &y, CONTAINED_TOO | BURIED_TOO);
            int glyph = obj_to_glyph(otmp, rn2_on_display_rng);
            int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_OBJECT, otmp, (struct monst*)0, 0UL, 0UL, 0UL, MAT_NONE, 0));

            add_menu(win, gui_glyph, &any,
                applied_invlet,
                applied_group_accelerator,
                ATR_NONE, NO_COLOR,
                doname(otmp), 
                MENU_UNSELECTED);

            cnt++;
        }
    }

    char cbuf[BUFSZ];
    Sprintf(cbuf, "Choose %s container:", this_container ? "another" : "a");

    end_menu(win, cbuf);
    menu_item* selected = (menu_item*)0;
    n = select_menu(win, PICK_ONE, &selected);
    destroy_nhwindow(win);

    if (n > 0) 
    {
        return selected->item.a_obj;
    }
    return (struct obj*)0;
}

/* Keep track of unfixable troubles for purposes of messages saying you feel
 * great.
 */
int
unfixable_trouble_count(is_horn)
boolean is_horn;
{
    int unfixable_trbl = 0;

    if (Stoned)
        unfixable_trbl++;
    if (Strangled)
        unfixable_trbl++;
    if (Wounded_legs && !u.usteed)
        unfixable_trbl++;
    if (Slimed)
        unfixable_trbl++;
    /* lycanthropy is undesirable, but it doesn't actually make you feel bad */

    if (!is_horn || (Confusion & ~TIMEOUT))
        unfixable_trbl++;
    if (!is_horn || (Sick & ~TIMEOUT))
        unfixable_trbl++;
    if (!is_horn || (FoodPoisoned & ~TIMEOUT))
        unfixable_trbl++;
    if (!is_horn || (MummyRot & ~TIMEOUT))
        unfixable_trbl++;
    if (!is_horn || (HHallucination & ~TIMEOUT))
        unfixable_trbl++;
    if (!is_horn || (Vomiting & ~TIMEOUT))
        unfixable_trbl++;
    if (!is_horn || (HStun & ~TIMEOUT))
        unfixable_trbl++;
    if (!is_horn || (HDeaf & ~TIMEOUT))
        unfixable_trbl++;

    return unfixable_trbl;
}

/* release monster from bag of tricks; return number of monsters created */
int
endlessarrows(bag, arrowtype, quan)
struct obj* bag;
int arrowtype, quan; //ObjID and quantity
{
    struct obj* otmp;
    otmp = mksobj(arrowtype, FALSE, FALSE, FALSE);
    if (otmp) 
    {
        otmp->quan = quan;
        otmp->owt = weight(otmp);

        if (bag->blessed)
        {
            otmp->blessed = TRUE;
            otmp->enchantment = rnd(3);
        }
        if (bag->cursed)
        {
            otmp->cursed = TRUE;
            otmp->enchantment = -rnd(3);
        }
        if (bag->elemental_enchantment)
        {
            if (bag->elemental_enchantment != DEATH_ENCHANTMENT || (bag->elemental_enchantment == DEATH_ENCHANTMENT && is_death_enchantable(otmp)))
                otmp->elemental_enchantment = bag->elemental_enchantment;

            if (otmp->elemental_enchantment == DEATH_ENCHANTMENT && quan > 1)
                otmp->quan = min(quan, max(1, DEATH_ENCHANTMENT_QUANTITY_NORMAL + bcsign(bag) * DEATH_ENCHANTMENT_QUANTITY_BUC_VARIATION));

            if (otmp->elemental_enchantment == LIGHTNING_ENCHANTMENT && quan > 1)
                otmp->quan = min(quan, max(1, ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL + bcsign(bag) * ELEMENTAL_ENCHANTMENT_QUANTITY_BUC_VARIATION));

            if (otmp->elemental_enchantment == FIRE_ENCHANTMENT && quan > 1)
                otmp->quan = min(quan, max(1, ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL + bcsign(bag) * ELEMENTAL_ENCHANTMENT_QUANTITY_BUC_VARIATION));

            if (otmp->elemental_enchantment == COLD_ENCHANTMENT && quan > 1)
                otmp->quan = min(quan, max(1, ELEMENTAL_ENCHANTMENT_QUANTITY_NORMAL + bcsign(bag) * ELEMENTAL_ENCHANTMENT_QUANTITY_BUC_VARIATION));

            otmp->owt = weight(otmp);
        }

        if ((bag->unpaid || (bag->where == OBJ_FLOOR && !bag->no_charge)) && costly_spot(u.ux, u.uy))
        {
            char* o_shop = in_rooms(u.ux, u.uy, SHOPBASE);
            struct monst* shkp = shop_keeper(*o_shop);
            if (shkp && inhishop(shkp) && (bag->where == OBJ_FLOOR || is_obj_on_shk_bill(bag, shkp)))
            {
                add_one_tobill(otmp, FALSE, shkp);
            }
        }

        play_sfx_sound(SFX_ITEM_APPEARS);
        You("pull %s out of %s.", doname(otmp), yname(bag));
        makeknown(bag->otyp);
        bag->cooldownleft = objects[bag->otyp].oc_item_cooldown;
        const char
            * verb = ((Is_airlevel(&u.uz) || u.uinwater) ? "slip" : "drop"),
            * oops_msg = (u.uswallow
                ? "Oops!  %s out of your reach!"
                : (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)
                    || levl[u.ux][u.uy].typ < IRONBARS
                    || levl[u.ux][u.uy].typ >= ICE)
                ? "Oops!  %s away from you!"
                : "Oops!  %s to the floor!");

        /* The(aobjnam()) is safe since otmp is unidentified -dlc */
        (void)hold_another_object(otmp, oops_msg,
            The(aobjnam(otmp, verb)),
            (const char*)0);
    }
    else
    {
        pline("%s is empty.", The(cxname(bag)));
    }

    if (bag->elemental_enchantment)
    {
        if (!rn2(ELEMENTAL_ENCHANTMENT_BAG_WEAR_OFF_ONE_PER_CHANCE))
        {
            bag->elemental_enchantment = 0;
            pline("%s no longer %s.", Yobjnam2(bag, "are"), bag->elemental_enchantment == FIRE_ENCHANTMENT ? "flaming" :
                bag->elemental_enchantment == COLD_ENCHANTMENT ? "freezing" :
                bag->elemental_enchantment == LIGHTNING_ENCHANTMENT ? "electrified" :
                bag->elemental_enchantment == DEATH_ENCHANTMENT ? "death-magical" : "enchanted");

            if (bag->where == OBJ_INVENT)
                update_inventory();
        }

    }

    if (otmp)
        return (int)otmp->quan;
    else
        return 0;
}


int
use_golf_club(obj)
struct obj* obj;
{
    int x, y;
    //int avrg_attrib;
    int glyph, oldglyph = -1;
    register struct monst* mtmp;
    boolean no_golf_swing = FALSE;
    char kickobjnam[BUFSZ];

    kickobjnam[0] = '\0';
    if (!uwep || !(uwep == obj && !uarms)) 
    {
        no_golf_swing = TRUE;
        if (!uwep || uwep != obj)
        {
            You_ex(ATR_NONE, CLR_MSG_FAIL, "must wield %s first.", the(cxname(obj)));
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_FAIL, "must have both hands free to use %s.", the(cxname(obj)));
        }
    }
    else if (near_capacity() > SLT_ENCUMBER) 
    {
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "load is too heavy to balance yourself for a golf swing.");
        no_golf_swing = TRUE;
    }
    else if (u.uinwater && !rn2(2)) 
    {
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "slow motion golf swing doesn't hit anything.");
        no_golf_swing = TRUE;
    }
    else if (u.utrap) 
    {
        no_golf_swing = TRUE;
        switch (u.utraptype) 
        {
        case TT_PIT:
            if (!Passes_walls)
                pline_ex(ATR_NONE, CLR_MSG_FAIL, "There's not enough room for a golf swing down here.");
            else
                no_golf_swing = FALSE;
            break;
        case TT_WEB:
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "move your %s!", makeplural(body_part(ARM)));
            break;
        default:
            break;
        }
    }

    if (no_golf_swing) 
    {
        /* ignore direction typed before player notices kick failed */
        play_sfx_sound(SFX_GENERAL_CANNOT);
        display_nhwindow(WIN_MESSAGE, TRUE); /* --More-- */
        return 0;
    }

    if (!getdir((char*)0))
        return 0;
    if (!u.dx && !u.dy)
        return 0;

    update_u_facing(TRUE);
    
    x = u.ux + u.dx;
    y = u.uy + u.dy;

    /* KMH -- Kicking boots always succeed */
    /*
    if (Magical_kicking)
        avrg_attrib = 99;
    else
        avrg_attrib = (ACURRSTR + ACURR(A_DEX) + ACURR(A_CON)) / 3;
    */
    if (u.uswallow) 
    {
        switch (rn2(3)) 
        {
        case 0:
            play_sfx_sound(SFX_STUCK_IN_TRAP);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "move your %s!", makeplural(body_part(ARM)));
            break;
        case 1:
            if (is_animal(u.ustuck->data)) 
            {
                play_sfx_sound(SFX_BURP);
                pline("%s burps loudly.", Monnam(u.ustuck));
                break;
            }
            /*FALLTHRU*/
        default:
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_SWING_MELEE);
            Your("feeble swing has no effect.");
            break;
        }
        return 1;
    }
    else if (u.utrap && u.utraptype == TT_PIT) 
    {
        /* must be Passes_walls */
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_SWING_MELEE);
        You("swing at the side of the pit.");
        return 1;
    }

    if (Levitation) 
    {
        int xx, yy;

        xx = u.ux - u.dx;
        yy = u.uy - u.dy;
        /* doors can be opened while levitating, so they must be
         * reachable for bracing purposes
         * Possible extension: allow bracing against stuff on the side?
         */
        if (isok(xx, yy) && !IS_ROCK(levl[xx][yy].typ)
            && !IS_DOOR(levl[xx][yy].typ)
            && (!Is_airlevel(&u.uz) || !OBJ_AT(xx, yy))) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "have nothing to brace yourself against.");
            return 0;
        }
    }

    mtmp = isok(x, y) ? m_at(x, y) : 0;
    /* might not kick monster if it is hidden and becomes revealed,
       if it is peaceful and player declines to attack, or if the
       hero passes out due to encumbrance with low hp; context.move
       will be 1 unless player declines to kick peaceful monster */
    if (mtmp) 
    {
        oldglyph = glyph_at(x, y);
        if (!attack(mtmp))
            return context.move;
    }

    wake_nearby();

    if (!isok(x, y)) 
    {
        goto thump;
    }
    struct rm *maploc = &levl[x][y];

    /*
     * The next five tests should stay in their present order:
     * monsters, pools, objects, non-doors, doors.
     *
     * [FIXME:  Monsters who are hidden underneath objects or
     * in pools should lead to hero kicking the concealment
     * rather than the monster, probably exposing the hidden
     * monster in the process.  And monsters who are hidden on
     * ceiling shouldn't be kickable (unless hero is flying?);
     * kicking toward them should just target whatever is on
     * the floor at that spot.]
     */

    if (mtmp) 
    {
        /* save mtmp->data (for recoil) in case mtmp gets killed */
        struct permonst* mdat = mtmp->data;

        attack(mtmp);
        glyph = glyph_at(x, y);
        /* see comment in attack_checks() */
        if (DEADMONSTER(mtmp))
        { /* DEADMONSTER() */
            /* if we mapped an invisible monster and immediately
               killed it, we don't want to forget what we thought
               was there before the kick */
            if (glyph != oldglyph && glyph_is_invisible(glyph))
                clear_current_and_show_glyph(x, y, oldglyph);
        }
        else if (!canspotmon(mtmp)
            /* check <x,y>; monster that evades kick by jumping
               to an unseen square doesn't leave an I behind */
            && mtmp->mx == x && mtmp->my == y
            && !glyph_is_invisible(glyph)
            && !(u.uswallow && mtmp == u.ustuck)) 
        {
            map_invisible(x, y);
        }

        /* recoil if floating */
        if ((Is_airlevel(&u.uz) || Levitation) && context.move) 
        {
            int range;

            range =
                ((int)youmonst.data->cwt + (weight_cap() + inv_weight()));
            if (range < 1)
                range = 1; /* divide by zero avoidance */
            range = (3 * (int)mdat->cwt) / range;

            if (range < 1)
                range = 1;
            hurtle(-u.dx, -u.dy, range, TRUE);
        }
        return 1;
    }

    (void)unmap_invisible(x, y);

    if (is_pool(x, y) ^ !!u.uinwater) 
    {
        /* objects normally can't be removed from water by kicking */
        play_sfx_sound_at_location(SFX_SPLASH_HIT, x, y);
        You("splash some %s around.", hliquid("water"));
        return 1;
    }

    if (OBJ_AT(x, y) && (!Levitation || Is_airlevel(&u.uz)
        || Is_waterlevel(&u.uz) || sobj_at(BOULDER, x, y))) 
    {
        if (kick_object(x, y, kickobjnam, TRUE)) 
        {
            if (Is_airlevel(&u.uz))
                hurtle(-u.dx, -u.dy, 1, TRUE); /* assume it's light */
            return 1;
        }
        return 1;
    }

    if (!IS_DOOR(maploc->typ)) 
    {
        if (maploc->typ == SDOOR) 
        {
            cvt_sdoor_to_door_with_animation(x, y); /* ->typ = DOOR */
            play_sfx_sound(SFX_THUMP_HIT);
            pline("Thump!  Your swing uncovers a secret door!");
            return 1;
        }

        if (maploc->typ == SCORR) 
        {
            goto thump;
        }

        if (IS_THRONE(maploc->typ)) 
        {
            if (Levitation)
                goto dumb;

            goto thump;
        }

        if (IS_ALTAR(maploc->typ)) 
        {
            if (Levitation)
                goto dumb;
            You("swing your %s at %s.", cxname(obj), (Blind ? something : "the altar"));
            play_sfx_sound(SFX_THUMP_HIT);
            altar_wrath(x, y);
            return 1;
        }
        if (IS_ANVIL(maploc->typ))
        {
            if (Levitation)
                goto dumb;
            You("swing your %s at %s.", cxname(obj), (Blind ? something : "the altar"));
            play_sfx_sound(SFX_THUMP_HIT);
            pline("THUD!");
            return 1;
        }
        if (IS_FOUNTAIN(maploc->typ))
        {
            if (Levitation)
                goto dumb;
            You("swing at %s.", (Blind ? something : "the fountain"));
            play_sfx_sound_at_location(SFX_SPLASH_HIT, x, y);
            pline("%s wet.", Yobjnam2(obj, "get"));
            return 1;
        }

        if (IS_GRAVE(maploc->typ))
        {
            if (Levitation)
                goto dumb;

            if (Role_if(PM_ARCHAEOLOGIST) || Role_if(PM_SAMURAI)
                || ((u.ualign.type == A_LAWFUL) && (u.ualign.record > -10))) 
            {
                adjalign(-sgn(u.ualign.type));
            }

            goto thump;
        }

        if (maploc->typ == IRONBARS)
        {
            play_sfx_sound(SFX_SINK_KLUNK);
            pline("Klunk!");
            return 1;

        }

        if (IS_TREE(maploc->typ)) 
        {
            goto thump;
        }

        if (IS_SINK(maploc->typ)) 
        {
            play_sfx_sound(SFX_SINK_KLUNK);
            pline("Klunk!");
            return 1;
        }

        if (maploc->typ == STAIRS || maploc->typ == LADDER || IS_STWALL(maploc->typ))
        {
            if (!IS_STWALL(maploc->typ) && maploc->ladder == LA_DOWN)
                goto dumb;

            goto thump;
        }

        goto dumb;
    }

    if ((maploc->doormask & D_MASK) == D_ISOPEN || (maploc->doormask & D_MASK) == D_BROKEN || (maploc->doormask & D_MASK) == D_NODOOR || (maploc->doormask & D_MASK) == D_PORTCULLIS)
    {
    dumb:
        You("swing at empty space.");
        if (Blind)
            feel_location(x, y);
        return 1; /* uses a turn */
    }

    /* door is known to be CLOSED or LOCKED */
    if (Blind)
        feel_location(x, y); /* we know we hit it */
thump:
    play_sfx_sound(SFX_THUMP_HIT);
    pline("Thump!");
    return 1;
}

/* -1 to continue to inventory, 0 = did not take a turn, 1 = take a turn */
int
floorapply(VOID_ARGS)
{
    register struct trap* ttmp = t_at(u.ux, u.uy);
    char qbuf[QBUFSZ];
    char c;
    int res = -1;

    if (!ttmp || !(trap_type_definitions[ttmp->ttyp].tdflags & TRAPDEF_FLAGS_APPLIABLE))
        return -1;

    /* "There is <an object> here; <verb> it?" or
        "There are <N objects> here; <verb> one?" */
    Sprintf(qbuf, "There is %s here; %s it?", an(trap_type_definitions[ttmp->ttyp].name), trap_type_definitions[ttmp->ttyp].apply_verb);
    if ((c = yn_function_es(YN_STYLE_GENERAL, ATR_NONE, CLR_MSG_ATTENTION, (const char*)0, qbuf, ynqchars, 'n', ynqdescs, (const char*)0)) == 'y')
    {
        res = use_lever(ttmp);
    }
    else if (c == 'q')
        return 0;

    return res;
}

int use_lever(lever)
struct trap* lever;
{
    if (!lever)
        return 0;

    if (lever->activation_count > 0 && !(lever->tflags & TRAPFLAGS_CONTINUOUSLY_SWITCHABLE))
    {
        play_sfx_sound_at_location(SFX_LEVER_SWITCH_FAIL, lever->tx, lever->ty);
        pline("The lever does not move.");
        return 1;
    }

    if (lever->tflags & TRAPFLAGS_SWITCHABLE_BETWEEN_STATES)
    {
        lever->activation_count++;
        if (lever->tflags & TRAPFLAGS_ACTIVATED)
        {
            play_sfx_sound_at_location(SFX_LEVER_SWITCH_OFF_SUCCESS, lever->tx, lever->ty);
            lever->tflags &= ~TRAPFLAGS_ACTIVATED;
            You("pull the lever right.");
        }
        else
        {
            play_sfx_sound_at_location(SFX_LEVER_SWITCH_ON_SUCCESS, lever->tx, lever->ty);
            lever->tflags |= TRAPFLAGS_ACTIVATED;
            You("pull the lever left.");
        }
    }
    else
    {
        play_sfx_sound_at_location(SFX_LEVER_SWITCH_SUCCESS, lever->tx, lever->ty);
        lever->activation_count++;
        You("pull the lever.");
    }
    newsym(lever->tx, lever->ty);

    /* Lever effect here */
    int target_x = lever->launch.x;
    int target_y = lever->launch.y;
    switch (lever->lever_effect)
    {
    case LEVER_EFFECT_OPEN_DOOR:
    {
        if (isok(target_x, target_y) && IS_DOOR(levl[target_x][target_y].typ) && levl[target_x][target_y].doormask & (D_CLOSED | D_LOCKED))
        {
            levl[target_x][target_y].doormask &= ~(D_CLOSED | D_LOCKED);
            levl[target_x][target_y].doormask |= D_ISOPEN;
            if (door_subtype_definitions[levl[target_x][target_y].subtyp].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND)
                unblock_vision_and_hearing_at_point(target_x, target_y);
            play_simple_location_sound(target_x, target_y, LOCATION_SOUND_TYPE_OPEN);
            newsym(target_x, target_y);
        }
        break;
    }
    case LEVER_EFFECT_LOCK_DOOR:
    {
        if (isok(target_x, target_y) && IS_DOOR(levl[target_x][target_y].typ) && levl[target_x][target_y].doormask & (D_CLOSED | D_ISOPEN))
        {
            levl[target_x][target_y].doormask &= ~(D_CLOSED | D_ISOPEN);
            levl[target_x][target_y].doormask |= D_LOCKED;
            if(door_subtype_definitions[levl[target_x][target_y].subtyp].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND)
                block_vision_and_hearing_at_point(target_x, target_y);
            play_simple_location_sound(target_x, target_y, LOCATION_SOUND_TYPE_LOCK);
            newsym(target_x, target_y);
        }
        break;
    }
    case LEVER_EFFECT_OPEN_LOCK_DOOR:
    {
        if (isok(target_x, target_y) && IS_DOOR(levl[target_x][target_y].typ))
        {
            if (levl[target_x][target_y].doormask & (D_CLOSED | D_ISOPEN) && (lever->tflags & TRAPFLAGS_STATE_MASK) == 0)
            {
                levl[target_x][target_y].doormask &= ~(D_CLOSED | D_ISOPEN);
                levl[target_x][target_y].doormask |= D_LOCKED;
                if (door_subtype_definitions[levl[target_x][target_y].subtyp].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND)
                    block_vision_and_hearing_at_point(target_x, target_y);
                play_simple_location_sound(target_x, target_y, LOCATION_SOUND_TYPE_LOCK);
            }
            else if (levl[target_x][target_y].doormask & (D_CLOSED | D_LOCKED) && (lever->tflags & TRAPFLAGS_STATE_MASK) > 0)
            {
                levl[target_x][target_y].doormask &= ~(D_CLOSED | D_LOCKED);
                levl[target_x][target_y].doormask |= D_ISOPEN;
                if (door_subtype_definitions[levl[target_x][target_y].subtyp].flags & DSTFLAGS_BLOCKS_VISION_AND_SOUND)
                    unblock_vision_and_hearing_at_point(target_x, target_y);
                play_simple_location_sound(target_x, target_y, LOCATION_SOUND_TYPE_OPEN);
            }
            newsym(target_x, target_y);
        }
        break;
    }
    case LEVER_EFFECT_CREATE_CLOSED_DOOR:
    {
        if (isok(target_x, target_y) && IS_ROCK(levl[target_x][target_y].typ))
        {
            create_simple_location(target_x, target_y, DOOR, 0, 0, D_CLOSED, 0, levl[target_x][target_y].floortyp, levl[target_x][target_y].floorsubtyp, levl[target_x][target_y].floorvartyp, TRUE);
            play_sfx_sound_at_location(SFX_SUMMON_MONSTER, target_x, target_y);
        }
        break;
    }
    case LEVER_EFFECT_CREATE_LOCATION_TYPE:
    {
        int loctyp = (int)lever->effect_param1;
        int locsubtyp = (int)lever->effect_param2;
        int locvartyp = (int)lever->effect_param3;
        int locflags = (uchar)lever->effect_flags;
        if (isok(target_x, target_y) && loctyp >= 0 && loctyp < MAX_LEVTYPE)
        {
            create_simple_location(target_x, target_y, loctyp, locsubtyp, locvartyp, locflags, 0, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floortyp, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floorsubtyp, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floorvartyp, TRUE);
            play_sfx_sound_at_location(SFX_SUMMON_MONSTER, target_x, target_y);
        }
        break;
    }
    case LEVER_EFFECT_CREATE_UNCREATE_LOCATION_TYPE:
    {
        int loctyp = (int)lever->effect_param1;
        int locsubtyp = (int)lever->effect_param2;
        int loctyp2 = (int)lever->effect_param3;
        int locsubtyp2 = (int)lever->effect_param4;
        if (isok(target_x, target_y))
        {
            if (loctyp2 >= 0 && loctyp2 < MAX_LEVTYPE && (lever->tflags & TRAPFLAGS_STATE_MASK) == 0)
            {
                create_simple_location(target_x, target_y, loctyp2, locsubtyp2, 0, 0, 0, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floortyp, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floorsubtyp, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floorvartyp, TRUE);
            }
            else if (loctyp >= 0 && loctyp < MAX_LEVTYPE && (lever->tflags & TRAPFLAGS_STATE_MASK) > 0)
            {
                create_simple_location(target_x, target_y, loctyp, locsubtyp, 0, 0, 0, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floortyp, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floorsubtyp, IS_FLOOR(loctyp) ? 0 : levl[target_x][target_y].floorvartyp, TRUE);
            }
            play_sfx_sound_at_location(SFX_SUMMON_MONSTER, target_x, target_y);
        }
        break;
    }
    case LEVER_EFFECT_CREATE_TRAP:
    {
        struct trap* t = 0;
        if (isok(target_x, target_y))
        {
            if (lever->effect_param3 > NO_TRAP && lever->effect_param3 < TRAPNUM)
            {
                int montyp = (int)lever->effect_param1;
                if (lever->effect_param3 == STATUE_TRAP && montyp == NON_PM && (char)lever->effect_param2 > 0)
                {
                    struct permonst* pm = mkclass((char)lever->effect_param2, 0);
                    if(pm)
                        montyp = monsndx(pm);
                }
                t = maketrap(target_x, target_y, (int)lever->effect_param3, montyp, lever->effect_flags);
            }
        }
        if (t)
        {
            t->tsubtyp = (uchar)lever->effect_param4;
            play_sfx_sound_at_location(SFX_SUMMON_MONSTER, target_x, target_y);
            newsym(target_x, target_y);
            if (cansee(target_x, target_y) && (t->tseen || t->ttyp == STATUE_TRAP))
            {
                pline("%s appears in a puff of smoke!", t->ttyp == STATUE_TRAP ? "A statue" : An(get_trap_explanation(t)));
            }
        }

        break;
    }
    case LEVER_EFFECT_CREATE_OBJECT:
    {
        struct obj* otmp = 0;
        if(isok(target_x, target_y))
        { 
            if (lever->effect_param1 > STRANGE_OBJECT && lever->effect_param1 < NUM_OBJECTS)
            {
                otmp = mksobj_at((int)lever->effect_param1, target_x, target_y, TRUE, FALSE);
                if (otmp)
                    otmp->special_quality = (short)lever->effect_param4;
            }
            else if (lever->effect_param2 >= 0)
            {
                otmp = mkobj_at((char)lever->effect_param2, target_x, target_y, TRUE);
            }
        }
        if (otmp)
        {
            play_sfx_sound_at_location(SFX_SUMMON_MONSTER, target_x, target_y);
            newsym(otmp->ox, otmp->oy);
            if (cansee(target_x, target_y))
            {
                pline("%s in a puff of smoke!", upstart(aobjnam(otmp, "appear")));
            }
        }

        break;
    }
    case LEVER_EFFECT_CREATE_MONSTER:
    {
        if (isok(target_x, target_y))
        {
            struct monst* mtmp = 0;
            if (lever->effect_param1 > NON_PM && lever->effect_param1 < NUM_MONSTERS)
            {
                mtmp = makemon(&mons[lever->effect_param1], target_x, target_y, lever->effect_flags | MM_ADJACENTOK);
            }
            else if (lever->effect_param2 >= 0)
            {
                mtmp = makemon(lever->effect_param2 == 0 ? (struct permonst*)0 : mkclass((char)lever->effect_param2, 0), target_x, target_y, lever->effect_flags | MM_ADJACENTOK);
            }
            if (mtmp)
            {
                play_sfx_sound_at_location(SFX_SUMMON_MONSTER, target_x, target_y);
                newsym(mtmp->mx, mtmp->my);
                if (canseemon(mtmp))
                {
                    pline("%s appears in a puff of smoke!", Amonnam(mtmp));
                }
            }
        }
        break;
    }
    default:
        break;
    }

    if (!(lever->tflags & TRAPFLAGS_SWITCHABLE_BETWEEN_STATES))
    {
        pline("The lever springs back to its original position.");
    }
    return 1;
}

/*apply.c*/
