/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    dig.c    $NHDT-Date: 1547421446 2019/01/13 23:17:26 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.117 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_VAR NEARDATA boolean did_dig_msg;

STATIC_DCL boolean NDECL(rm_waslit);
STATIC_DCL void FDECL(mkcavepos,
                      (XCHAR_P, XCHAR_P, int, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL void FDECL(mkcavearea, (BOOLEAN_P));
STATIC_DCL int NDECL(dig);
STATIC_DCL void FDECL(dig_up_grave, (coord *));
STATIC_DCL int FDECL(adj_pit_checks, (coord *, char *));
STATIC_DCL void FDECL(pit_flow, (struct trap *, SCHAR_P));
STATIC_OVL void FDECL(create_tree_remnants, (int, int, boolean*));

/* Indices returned by dig_typ() */
enum dig_types {
    DIGTYP_UNDIGGABLE = 0,
    DIGTYP_ROCK,
    DIGTYP_STATUE,
    DIGTYP_BOULDER,
    DIGTYP_DOOR,
    DIGTYP_TREE
};

STATIC_OVL boolean
rm_waslit()
{
    register xchar x, y;

    if (IS_FLOOR(levl[u.ux][u.uy].typ) && levl[u.ux][u.uy].waslit)
        return TRUE;
    for (x = u.ux - 2; x < u.ux + 3; x++)
        for (y = u.uy - 1; y < u.uy + 2; y++)
            if (isok(x, y) && levl[x][y].waslit)
                return TRUE;
    return FALSE;
}

/* Change level topology.  Messes with vision tables and ignores things like
 * boulders in the name of a nice effect.  Vision will get fixed up again
 * immediately after the effect is complete.
 */
STATIC_OVL void
mkcavepos(x, y, dist, waslit, rockit)
xchar x, y;
int dist;
boolean waslit, rockit;
{
    register struct rm *lev;

    if (!isok(x, y))
        return;
    lev = &levl[x][y];

    if (rockit) {
        register struct monst *mtmp;

        if (IS_ROCK(lev->typ))
            return;
        if (t_at(x, y))
            return;                   /* don't cover the portal */
        if ((mtmp = m_at(x, y)) != 0) /* make sure crucial monsters survive */
            if (!passes_walls(mtmp->data))
                (void) rloc(mtmp, TRUE);
    } else if (IS_FLOOR(lev->typ))
        return;

    unblock_vision_and_hearing_at_point(x, y); /* make sure vision knows this location is open */

    /* fake out saved state */
    int rocktyp = (rockit ? STONE : GROUND);
    int rocksubtyp = get_initial_location_subtype(rocktyp);
    int rockvartyp = get_initial_location_vartype(rocktyp, rocksubtyp);
    int rockfloortyp = rockit ? CORR : 0;
    int rockfloorsubtyp = get_initial_location_subtype(rockfloortyp);
    int rockfloorvartyp = get_initial_location_vartype(rockfloortyp, rockfloorsubtyp);

    create_simple_location(x, y, rocktyp, rocksubtyp, rockvartyp, 0, 0, rockfloortyp, rockfloorsubtyp, rockfloorvartyp, FALSE);

    lev->seenv = 0;
    //lev->doormask = 0;
    if (dist < 3)
        lev->lit = (rockit ? FALSE : TRUE);
    if (waslit)
        lev->waslit = (rockit ? FALSE : TRUE);
    //lev->horizontal = FALSE;
    /* short-circuit vision recalc */
    viz_array[y][x] = (dist < 3) ? (IN_SIGHT | COULD_SEE) : COULD_SEE;
    //lev->typ = (rockit ? STONE : ROOM); /* flags set via doormask above */
    if (dist >= 3)
        impossible("mkcavepos called with dist %d", dist);
    feel_newsym(x, y);
}

STATIC_OVL void
mkcavearea(rockit)
register boolean rockit;
{
    int dist;
    xchar xmin = u.ux, xmax = u.ux;
    xchar ymin = u.uy, ymax = u.uy;
    register xchar i;
    register boolean waslit = rm_waslit();

    if (rockit)
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "Crash!  The ceiling collapses around you!");
    else
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "A mysterious force %s cave around you!",
              (levl[u.ux][u.uy].typ == CORR) ? "creates a" : "extends the");
    display_nhwindow(WIN_MESSAGE, TRUE);

    for (dist = 1; dist <= 2; dist++) {
        xmin--;
        xmax++;

        /* top and bottom */
        if (dist < 2) { /* the area is wider that it is high */
            ymin--;
            ymax++;
            for (i = xmin + 1; i < xmax; i++) {
                mkcavepos(i, ymin, dist, waslit, rockit);
                mkcavepos(i, ymax, dist, waslit, rockit);
            }
        }

        /* left and right */
        for (i = ymin; i <= ymax; i++) {
            mkcavepos(xmin, i, dist, waslit, rockit);
            mkcavepos(xmax, i, dist, waslit, rockit);
        }

        flush_screen(1); /* make sure the new glyphs shows up */
        adjusted_delay_output();
    }

    if (!rockit && levl[u.ux][u.uy].typ == CORR)
    {
        levl[u.ux][u.uy].typ = GROUND; /* flags for CORR already 0 */
        levl[u.ux][u.uy].subtyp = get_initial_location_subtype(levl[u.ux][u.uy].typ);
        levl[u.ux][u.uy].vartyp = get_initial_location_vartype(levl[u.ux][u.uy].typ, levl[u.ux][u.uy].subtyp);
        if (waslit)
            levl[u.ux][u.uy].waslit = TRUE;
        newsym(u.ux, u.uy); /* in case player is invisible */
    }

    vision_full_recalc = 1; /* everything changed */
}

/* When digging into location <x,y>, what are you actually digging into? */
int
dig_typ(otmp, x, y)
struct obj *otmp;
xchar x, y;
{
    boolean ispick, isaxe, issaw;

    if (!otmp)
        return DIGTYP_UNDIGGABLE;
    ispick = is_pick(otmp);
    isaxe = is_axe(otmp);
    issaw = is_saw(otmp);
    if (!ispick && !isaxe && !issaw)
        return DIGTYP_UNDIGGABLE;

    return ((ispick && sobj_at(STATUE, x, y))
               ? DIGTYP_STATUE
               : (ispick && sobj_at(BOULDER, x, y))
                  ? DIGTYP_BOULDER
                  : (!issaw && closed_door(x, y))
                     ? DIGTYP_DOOR
                     : IS_TREE(levl[x][y].typ)
                        ? (ispick ? DIGTYP_UNDIGGABLE : DIGTYP_TREE)
                        : (ispick && IS_ROCK(levl[x][y].typ)
                           && (!level.flags.arboreal || (IS_WALL_OR_SDOOR(levl[x][y].typ))))
                           ? DIGTYP_ROCK
                           : DIGTYP_UNDIGGABLE);
}

boolean
is_digging()
{
    if (occupation == dig) {
        return TRUE;
    }
    return FALSE;
}

#define BY_YOU (&youmonst)
#define BY_OBJECT ((struct monst *) 0)

boolean
dig_check(madeby, verbose, x, y)
struct monst *madeby;
boolean verbose;
int x, y;
{
    struct trap *ttmp = t_at(x, y);
    const char *verb =
        (madeby == BY_YOU && uwep && is_axe(uwep)) ? "chop" : 
        (madeby == BY_YOU && uwep && is_saw(uwep)) ? "cut" :
        "dig in";

    if (On_stairs(x, y))
    {
        if (x == xdnladder || x == xupladder)
        {
            if (verbose)
                pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "ladder resists your effort.");
        } 
        else if (verbose)
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "stairs are too hard to %s.", verb);
        return FALSE;
    } 
    else if (IS_THRONE(levl[x][y].typ) && madeby != BY_OBJECT) 
    {
        if (verbose)
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "throne is too hard to break apart.");
        return FALSE;
    }
    else if (IS_ANVIL(levl[x][y].typ)) 
    {
        if (verbose)
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "anvil is too hard to break apart.");
        return FALSE;
    }
    else if (IS_ALTAR(levl[x][y].typ)
               && (madeby != BY_OBJECT || Is_astralevel(&u.uz)
                   || Is_sanctum(&u.uz))) 
    {
        if (verbose)
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "altar is too hard to break apart.");
        return FALSE;
    }
    else if (Is_airlevel(&u.uz)) 
    {
        if (verbose)
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot %s thin air.", verb);
        return FALSE;
    } 
    else if (Is_waterlevel(&u.uz)) 
    {
        if (verbose)
        {
            play_sfx_sound_at_location(SFX_SPLASH_HIT, x, y);
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "%s splashes and subsides.", hliquid("water"));
        }
        return FALSE;
    } 
    else if ((IS_ROCK(levl[x][y].typ) && levl[x][y].typ != SDOOR
                && (levl[x][y].wall_info & W_NONDIGGABLE) != 0)
               || (ttmp && ((trap_type_definitions[ttmp->ttyp].tdflags & TRAPDEF_FLAGS_NOT_OVERRIDEN) || (!Can_dig_down(&u.uz) && !levl[x][y].candig)))
               || (IS_DOOR_OR_SDOOR(levl[x][y].typ) && !is_door_diggable_at(x, y))
              ) 
    {
        if (verbose)
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "%s here is too hard to %s.", surface(x, y), verb);
        return FALSE;
    } 
    else if (sobj_at(BOULDER, x, y)) 
    {
        if (verbose)
            There_ex(ATR_NONE, CLR_MSG_FAIL, "isn't enough room to %s here.", verb);
        return FALSE;
    }
    else if (madeby == BY_OBJECT
               /* the block against existing traps is mainly to
                  prevent broken wands from turning holes into pits */
               && (ttmp || is_pool_or_lava(x, y))) 
    {
        /* digging by player handles pools separately */
        return FALSE;
    }
    return TRUE;
}

STATIC_OVL int
dig(VOID_ARGS)
{
    register struct rm *lev;
    register xchar dpx = context.digging.pos.x, dpy = context.digging.pos.y;
    boolean use_right = uwep && (is_pick(uwep) || is_axe(uwep) || is_saw(uwep));
    boolean use_left = uarms && (is_pick(uarms) || is_axe(uarms) || is_saw(uarms));
    struct obj* wep = (use_right ? uwep : use_left ? uarms : (struct obj*)0);
    register boolean ispick = wep && is_pick(wep);
    register boolean isaxe = wep && is_axe(wep);
    register boolean issaw = wep && is_saw(wep);
    const char *verb = (!wep || is_pick(wep)) ? "dig into" : isaxe ? "chop through" : issaw ? "cut into" : "dig into";

    lev = &levl[dpx][dpy];
    /* perhaps a nymph stole your pick-axe while you were busy digging */
    /* or perhaps you teleported away */
    if (u.uswallow || !wep || (!ispick && !isaxe && !issaw)
        || !on_level(&context.digging.level, &u.uz)
        || ((context.digging.down ? (dpx != u.ux || dpy != u.uy)
                                  : (distu(dpx, dpy) > 2))))
        return 0;

    if (context.digging.down)
    {
        if (!dig_check(BY_YOU, TRUE, u.ux, u.uy))
            return 0;
    } 
    else 
    { /* !context.digging.down */
        if (IS_TREE(lev->typ) && !may_dig(dpx, dpy)
            && dig_typ(wep, dpx, dpy) == DIGTYP_TREE) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "This tree seems to be petrified.");
            return 0;
        }
        if (IS_ROCK(lev->typ) && !may_dig(dpx, dpy)
            && dig_typ(wep, dpx, dpy) == DIGTYP_ROCK) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "This %s is too hard to %s.",
                  is_db_wall(dpx, dpy) ? "drawbridge" : "wall", verb);
            return 0;
        }
        if (IS_DOOR_OR_SDOOR(lev->typ))
        {
            if (!m_can_destroy_door(&youmonst, lev, FALSE) || !is_door_diggable_at_ptr(lev))
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                pline_ex(ATR_NONE, CLR_MSG_FAIL, "This %s is too hard to %s.",
                    get_door_name_at_ptr(lev), verb);
                return 0;
            }
        }
    }
    if (Fumbling && !rn2(3)) {
        switch (rn2(3)) {
        case 0:
            if (!welded(wep, &youmonst)) {
                You_ex(ATR_NONE, CLR_MSG_WARNING, "fumble and drop %s.", yname(wep));
                dropx(wep);
            } else {
                if (u.usteed)
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s and %s %s!", Yobjnam2(wep, "bounce"),
                          otense(wep, "hit"), mon_nam(u.usteed));
                else
                {
                    play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "Ouch!  %s and %s you!", Yobjnam2(wep, "bounce"),
                        otense(wep, "hit"));
                }
                set_wounded_legs(RIGHT_SIDE, 5 + rnd(5));
            }
            break;
        case 1:
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "Bang!  You hit with the broad side of %s!",
                  the(xname(wep)));
            break;
        default:
            if(issaw)
                Your_ex(ATR_NONE, CLR_MSG_FAIL, "fail to position your saw properly.");
            else
            Your_ex(ATR_NONE, CLR_MSG_FAIL, "swing misses its mark.");
            break;
        }
        return 0;
    }

    context.digging.effort +=
        10 + rn2(5) + u_strdex_to_hit_bonus() + wep->enchantment + material_definitions[wep->material].digging_speed_bonus + exceptionality_digging_speed_bonus(wep) - greatest_erosion(wep) + u.ubasedaminc + u.udaminc;
    
    int speed_bonus = digging_skill_speed_bonus(P_SKILL_LEVEL(objects[wep->otyp].oc_skill));
    context.digging.effort = max(1, (context.digging.effort * (100 + speed_bonus)) / 100);

    if (has_otyp_double_digging_effort(wep->otyp))
        context.digging.effort *= 2;

    if (context.digging.down) 
    {
        struct trap *ttmp = t_at(dpx, dpy);

        if (context.digging.effort > 250 || (ttmp && ttmp->ttyp == HOLE)) {
            (void) dighole(FALSE, FALSE, (coord *) 0);
            (void) memset((genericptr_t) &context.digging, 0,
                          sizeof context.digging);
            return 0; /* done with digging */
        }

        if (context.digging.effort <= 50
            || (ttmp && (ttmp->ttyp == TRAPDOOR || is_pit(ttmp->ttyp)))) {
            return 1;
        } else if (ttmp && (ttmp->ttyp == LANDMINE
                            || (ttmp->ttyp == BEAR_TRAP && !u.utrap))) {
            /* digging onto a set object trap triggers it;
               hero should have used #untrap first */
            dotrap(ttmp, FORCETRAP);
            /* restart completely from scratch if we resume digging */
            (void) memset((genericptr_t) &context.digging, 0,
                          sizeof context.digging);
            return 0;
        } else if (ttmp && ttmp->ttyp == BEAR_TRAP && u.utrap) {
            if (rnl(7) > (Fumbling ? 1 : 4)) {
                char kbuf[BUFSZ];
                int dmg = is_launcher(wep) ? d(1, 2) : weapon_total_dmg_value(wep, &youmonst, &youmonst, 0) + u_str_dmg_bonus();

                if (dmg < 1)
                    dmg = 1;
                else if (uarmf)
                    dmg = (dmg + 1) / 2;
                You("hit yourself in the %s.", body_part(FOOT));
                Sprintf(kbuf, "%s off %s own %s", issaw ? "slicing" : "chopping", uhis(),
                        body_part(FOOT));
                losehp(adjust_damage(dmg, &youmonst, &youmonst, AD_PHYS, ADFLAGS_NONE), kbuf, KILLED_BY);
            } else {
                You("destroy the bear trap with %s.",
                    yobjnam(wep, (const char *) 0));
                deltrap(ttmp);
                reset_utrap(TRUE); /* release from trap, maybe Lev or Fly */
            }
            /* we haven't made any progress toward a pit yet */
            context.digging.effort = 0;
            return 0;
        }

        if (IS_ALTAR(lev->typ)) {
            altar_wrath(dpx, dpy);
            angry_priest();
        }

        /* make pit at <u.ux,u.uy> */
        if (dighole(TRUE, FALSE, (coord *) 0)) {
            use_skill(context.digging.skill, 1);
            context.digging.level.dnum = 0;
            context.digging.level.dlevel = -1;
        }
        return 0;
    }

    if (context.digging.effort > 100) 
    {
        boolean done_feelnewsym = FALSE;
        const char *digtxt, *dmgtxt = (const char *) 0;
        struct obj *obj;
        boolean shopedge = *in_rooms(dpx, dpy, SHOPBASE);

        if ((obj = sobj_at(STATUE, dpx, dpy)) != 0) {
            if (break_statue(obj))
                digtxt = "The statue shatters.";
            else
                /* it was a statue trap; break_statue()
                 * printed a message and updated the screen
                 */
                digtxt = (char *) 0;
        }
        else if ((obj = sobj_at(BOULDER, dpx, dpy)) != 0) 
        {
            struct obj *bobj;

            fracture_rock(obj, TRUE);
            if ((bobj = sobj_at(BOULDER, dpx, dpy)) != 0) 
            {
                /* another boulder here, restack it to the top */
                Strcpy(debug_buf_2, "dig");
                obj_extract_self(bobj);
                place_object(bobj, dpx, dpy);
            }
            digtxt = "The boulder falls apart.";
        } 
        else if (lev->typ == STONE || lev->typ == SCORR || IS_TREE(lev->typ)) 
        {
            if (Is_earthlevel(&u.uz))
            {
                if (wep->blessed && !rn2(3)) 
                {
                    play_simple_location_sound(dpx, dpy, LOCATION_SOUND_TYPE_BREAK);
                    mkcavearea(FALSE);
                    goto cleanup;
                } 
                else if ((wep->cursed && !rn2(4))
                           || (!wep->blessed && !rn2(6))) 
                {
                    play_simple_location_sound(dpx, dpy, LOCATION_SOUND_TYPE_BREAK);
                    mkcavearea(TRUE);
                    goto cleanup;
                }
            }
            if (IS_TREE(lev->typ))
            {
                play_simple_location_sound(dpx, dpy, LOCATION_SOUND_TYPE_BREAK);
                digtxt = "You cut down the tree.";
                struct mkroom* r = which_room(dpx, dpy);

                /* Change the location type */
                int typ = lev->floortyp ? lev->floortyp : r && r->orig_rtype == GARDEN ? GRASS : ROOM;
                int subtyp = lev->floorsubtyp ? lev->floorsubtyp : get_initial_location_subtype(typ);
                int vartyp = lev->floorvartyp ? lev->floorvartyp : get_initial_location_vartype(typ, subtyp);
                create_simple_location(dpx, dpy, typ, subtyp, vartyp, 0, back_to_broken_glyph(dpx, dpy), 0, 0, 0, FALSE);
                uncatch_tree_objects(dpx, dpy);
                create_tree_remnants(dpx, dpy, &done_feelnewsym);
            }
            else 
            {
                digtxt = "You succeed in cutting away some rock.";
                play_simple_location_sound(dpx, dpy, LOCATION_SOUND_TYPE_BREAK);
                int glyph = layers_at(dpx, dpy).layer_gui_glyphs[LAYER_FLOOR];
                create_basic_floor_location(dpx, dpy, levl[dpx][dpy].floortyp ? levl[dpx][dpy].floortyp : CORR, levl[dpx][dpy].floortyp ? levl[dpx][dpy].floorsubtyp : get_initial_location_subtype(levl[dpx][dpy].floortyp), 0, FALSE);
                if (windowprocs.wincap2 & WC2_FADING_ANIMATIONS)
                {
                    feel_newsym(dpx, dpy);
                    done_feelnewsym = TRUE;
                    play_special_effect_with_details_at(0, dpx, dpy, glyph, LAYER_BACKGROUND_EFFECT, -2, 20, 0, 0, TRUE);
                    special_effect_wait_until_action(0);
                    special_effect_wait_until_end(0);
                }
            }
        }
        else if (IS_WALL(lev->typ)) 
        {
            if (shopedge) 
            {
                add_damage(dpx, dpy, SHOP_WALL_DMG);
                dmgtxt = "damage";
            }
            int ltype = 0;
            int lsubtype = 0;
            int lvartype = 0;
            uchar lflags = 0;
            if (level.flags.is_maze_lev)
            {
                ltype = CORR; // ROOM;
                lsubtype = get_initial_location_subtype(ltype);
                lvartype = get_initial_location_vartype(ltype, lsubtype);
            }
            else if (level.flags.is_cavernous_lev && !in_town(dpx, dpy)) 
            {
                ltype = CORR;
                lsubtype = get_initial_location_subtype(ltype);
                lvartype = get_initial_location_vartype(ltype, lsubtype);
            }
            else 
            {
                ltype = DOOR, lsubtype = 0, lflags = D_NODOOR;
            }
            digtxt = "You make an opening in the wall.";
            play_simple_location_sound(dpx, dpy, LOCATION_SOUND_TYPE_BREAK);
            int glyph = layers_at(dpx, dpy).layer_gui_glyphs[lev->typ == STONE ? LAYER_FLOOR : LAYER_FEATURE];
            create_simple_location(dpx, dpy, ltype, lsubtype, lvartype, lflags, back_to_broken_glyph(dpx, dpy), !IS_FLOOR(ltype)? lev->floortyp : 0, !IS_FLOOR(ltype) ? lev->floorsubtyp : 0, !IS_FLOOR(ltype) ? lev->floorvartyp : 0, FALSE);
            if (windowprocs.wincap2 & WC2_FADING_ANIMATIONS)
            {
                feel_newsym(dpx, dpy);
                done_feelnewsym = TRUE;
                play_special_effect_with_details_at(0, dpx, dpy, glyph, LAYER_BACKGROUND_EFFECT, -2, 20, 0, 0, TRUE);
                special_effect_wait_until_action(0);
                special_effect_wait_until_end(0);
            }
        } 
        else if (lev->typ == SDOOR) 
        {
            play_simple_location_sound(dpx, dpy, LOCATION_SOUND_TYPE_BREAK);
            cvt_sdoor_to_door(dpx, dpy); /* ->typ = DOOR */
            digtxt = "You break through a secret door!";
            if (!(lev->doormask & D_TRAPPED))
            {
                lev->doormask &= ~D_MASK;
                lev->doormask |= D_BROKEN;
            }
        } 
        else if (closed_door(dpx, dpy))
        {
            play_simple_location_sound(dpx, dpy, LOCATION_SOUND_TYPE_BREAK);
            digtxt = "You break through the door.";
            if (shopedge)
            {
                add_damage(dpx, dpy, SHOP_DOOR_COST);
                dmgtxt = "break";
            }
            if (!(lev->doormask & D_TRAPPED))
            {
                lev->doormask &= ~D_MASK;
                lev->doormask |= D_BROKEN;
            }
        } else
            return 0; /* statue or boulder got taken */

        if (!does_block(dpx, dpy, &levl[dpx][dpy]))
            unblock_vision_and_hearing_at_point(dpx, dpy); /* vision:  can see through */
        if(!done_feelnewsym)
            feel_newsym(dpx, dpy);

        if (digtxt && !context.digging.quiet)
            pline1(digtxt); /* after newsym */

        if (dmgtxt)
            pay_for_damage(dmgtxt, FALSE);

        if (Is_earthlevel(&u.uz) && !rn2(3))
        {
            register struct monst *mtmp;

            switch (rn2(2)) 
            {
            case 0:
                mtmp = makemon(&mons[!rn2(2) ? PM_ELDER_EARTH_ELEMENTAL : PM_EARTH_ELEMENTAL], dpx, dpy,
                               NO_MM_FLAGS);
                break;
            default:
                mtmp = makemon(&mons[PM_XORN], dpx, dpy, NO_MM_FLAGS);
                break;
            }
            if (mtmp)
                pline_The("debris from your digging comes to life!");
        }
        if (IS_DOOR(lev->typ) && (lev->doormask & D_TRAPPED)) 
        {
            b_trapped(get_door_name_at_ptr(lev), 0, dpx, dpy);
            lev->doormask &= ~D_TRAPPED;
            if (is_door_destroyed_by_booby_trap_at_ptr(lev))
            {
                lev->doormask &= ~D_MASK;
                lev->doormask |= D_NODOOR;
                lev->subtyp = 0;
                newsym(dpx, dpy);
            }
        }
    cleanup:
        use_skill(context.digging.skill, 1);
        context.digging.lastdigtime = moves;
        context.digging.quiet = FALSE;
        context.digging.level.dnum = 0;
        context.digging.level.dlevel = -1;
        return 0;
    } 
    else 
    { /* not enough effort has been spent yet */
        STATIC_VAR const char *const d_target[6] = { "",        "rock", "statue",
                                                 "boulder", "door", "tree" };
        int dig_target = dig_typ(wep, dpx, dpy);

        if (IS_WALL(lev->typ) || dig_target == DIGTYP_DOOR) 
        {
            if (*in_rooms(dpx, dpy, SHOPBASE)) 
            {
                pline("This %s seems too hard to %s.",
                      IS_DOOR(lev->typ) ? "door" : "wall", verb);
                return 0;
            }
        } else if (dig_target == DIGTYP_UNDIGGABLE
                   || (dig_target == DIGTYP_ROCK && !IS_ROCK(lev->typ)))
            return 0; /* statue or boulder got taken */

        if (!did_dig_msg) 
        {
            You("%s the %s with all your might.", issaw ? "cut" : "hit", d_target[dig_target]);
            did_dig_msg = TRUE;
        }
    }
    return 1;
}

/* When will hole be finished? Very rough indication used by shopkeeper. */
int
holetime()
{
    if (occupation != dig || !*u.ushops)
        return -1;
    return ((250 - context.digging.effort) / 20);
}

/* Return typ of liquid to fill a hole with, or GROUND, if no liquid nearby */
schar
fillholetyp(x, y, fill_if_any)
int x, y;
boolean fill_if_any; /* force filling if it exists at all */
{
    register int x1, y1;
    int lo_x = max(1, x - 1), hi_x = min(x + 1, COLNO - 1),
        lo_y = max(0, y - 1), hi_y = min(y + 1, ROWNO - 1);
    int pool_cnt = 0, moat_cnt = 0, lava_cnt = 0;

    for (x1 = lo_x; x1 <= hi_x; x1++)
        for (y1 = lo_y; y1 <= hi_y; y1++)
            if (is_moat(x1, y1))
                moat_cnt++;
            else if (is_pool(x1, y1))
                /* This must come after is_moat since moats are pools
                 * but not vice-versa. */
                pool_cnt++;
            else if (is_lava(x1, y1))
                lava_cnt++;

    if (!fill_if_any)
        pool_cnt /= 3; /* not as much liquid as the others */

    if ((lava_cnt > moat_cnt + pool_cnt && rn2(lava_cnt + 1))
        || (lava_cnt && fill_if_any))
        return LAVAPOOL;
    else if ((moat_cnt > 0 && rn2(moat_cnt + 1)) || (moat_cnt && fill_if_any))
        return MOAT;
    else if ((pool_cnt > 0 && rn2(pool_cnt + 1)) || (pool_cnt && fill_if_any))
        return POOL;
    else if (levl[x][y].floortyp)
        return levl[x][y].floortyp;
    else
        return GROUND;
}

void
digactualhole(x, y, madeby, ttyp)
register int x, y;
struct monst *madeby;
int ttyp;
{
    struct obj *oldobjs, *newobjs;
    register struct trap *ttmp;
    char surface_type[BUFSZ];
    struct rm *lev = &levl[x][y];
    boolean shopdoor;
    struct monst *mtmp = m_at(x, y); /* may be madeby */
    boolean madeby_u = (madeby == BY_YOU);
    boolean madeby_obj = (madeby == BY_OBJECT);
    boolean madeby_mon = madeby && !madeby_u && !madeby_obj;
    boolean at_u = (x == u.ux) && (y == u.uy);
    boolean wont_fall = Moves_above_ground;

    if (at_u && u.utrap) {
        if (u.utraptype == TT_BURIEDBALL)
            buried_ball_to_punishment();
        else if (u.utraptype == TT_INFLOOR)
            reset_utrap(FALSE);
    }

    /* these furniture checks were in dighole(), but wand
       breaking bypasses that routine and calls us directly */
    if (IS_FOUNTAIN(lev->typ)) {
        dogushforth(FALSE);
        SET_FOUNTAIN_WARNED(x, y); /* force dryup */
        dryup(x, y, madeby_u);
        return;
    } else if (IS_SINK(lev->typ)) {
        breaksink(x, y);
        return;
    } else if (lev->typ == DRAWBRIDGE_DOWN
               || (is_drawbridge_wall(x, y) >= 0)) {
        int bx = x, by = y;

        /* if under the portcullis, the bridge is adjacent */
        (void) find_drawbridge(&bx, &by);
        destroy_drawbridge(bx, by, FALSE);
        return;
    }

    if (ttyp != PIT && (!Can_dig_down(&u.uz) && !lev->candig)) {
        impossible("digactualhole: can't dig %s on this level.",
                   defsyms[trap_to_defsym(ttyp)].explanation);
        ttyp = PIT;
    }

    /* maketrap() might change it, also, in this situation,
       surface() returns an inappropriate string for a grave */
    if (IS_GRAVE(lev->typ))
        Strcpy(surface_type, "grave");
    else
        Strcpy(surface_type, surface(x, y));
    shopdoor = IS_DOOR(lev->typ) && *in_rooms(x, y, SHOPBASE);
    oldobjs = level.objects[x][y];
    ttmp = maketrap(x, y, ttyp, NON_PM, (madeby_u ? MKTRAPFLAG_MADE_BY_U : 0UL) | (madeby_mon ? MKTRAPFLAG_MADE_BY_MON : 0UL));
    if (!ttmp)
        return;
    newobjs = level.objects[x][y];
    ttmp->tseen = 0;
    if (cansee(x, y))
        seetrap(ttmp);
    else if (madeby_u)
        feeltrap(ttmp);

    /* Remove doodads */
    levl[x][y].floor_doodad = 0;
    levl[x][y].carpet_typ = 0;
    levl[x][y].carpet_piece = 0;
    levl[x][y].carpet_flags = 0;
    levl[x][y].decoration_typ = 0;
    levl[x][y].decoration_subtyp = 0;
    levl[x][y].decoration_dir = 0;
    levl[x][y].decoration_flags = 0;

    if (ttyp == PIT) {
        if (madeby_u) {
            if (x != u.ux || y != u.uy)
                You("dig an adjacent pit.");
            else
                You("dig a pit in the %s.", surface_type);
            if (shopdoor)
                pay_for_damage("ruin", FALSE);
        } else if (!madeby_obj && canseemon(madeby)) {
            pline("%s digs a pit in the %s.", Monnam(madeby), surface_type);
        } else if (cansee(x, y) && flags.verbose) {
            pline("A pit appears in the %s.", surface_type);
        }
        /* in case we're digging down while encased in solid rock
           which is blocking levitation or flight */
        switch_terrain();
        if (Moves_above_ground)
            wont_fall = TRUE;

        if (at_u) {
            if (!wont_fall) {
                set_utrap(rn1(4, 2), TT_PIT);
                vision_full_recalc = 1; /* vision limits change */
            } else
                reset_utrap(TRUE);
            if (oldobjs != newobjs) /* something unearthed */
                (void) pickup(1, FALSE);   /* detects pit */
        } else if (mtmp) {
            if (is_flying(mtmp) || is_levitating(mtmp)) {
                if (canseemon(mtmp))
                    pline("%s %s over the pit.", Monnam(mtmp),
                          (is_flying(mtmp)) ? "flies" : "floats");
            } else if (mtmp != madeby)
                (void) mintrap(mtmp);
        }
    } else { /* was TRAPDOOR now a HOLE*/

        if (madeby_u)
            You("dig a hole through the %s.", surface_type);
        else if (!madeby_obj && canseemon(madeby))
            pline("%s digs a hole through the %s.", Monnam(madeby),
                  surface_type);
        else if (cansee(x, y) && flags.verbose)
            pline("A hole appears in the %s.", surface_type);

        if (at_u) {
            /* in case we're digging down while encased in solid rock
               which is blocking levitation or flight */
            switch_terrain();
            if (Moves_above_ground)
                wont_fall = TRUE;

            /* check for leashed pet that can't fall right now */
            if (!u.ustuck && !wont_fall && !next_to_u()) {
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are jerked back by your pet!");
                wont_fall = TRUE;
            }

            /* Floor objects get a chance of falling down.  The case where
             * the hero does NOT fall down is treated here.  The case
             * where the hero does fall down is treated in goto_level().
             */
            if (u.ustuck || wont_fall) {
                if (newobjs)
                    impact_drop((struct obj *) 0, x, y, 0);
                if (oldobjs != newobjs)
                    (void) pickup(1, FALSE);
                if (shopdoor && madeby_u)
                    pay_for_damage("ruin", FALSE);

            } else {
                d_level newlevel;

                if (*u.ushops && madeby_u)
                    shopdig(1); /* shk might snatch pack */
                /* handle earlier damage, eg breaking wand of digging */
                else if (!madeby_u)
                    pay_for_damage("dig into", TRUE);

                play_sfx_sound(SFX_TRAP_DOOR_OPENS);
                You("fall through...");
                /* Earlier checks must ensure that the destination
                 * level exists and is in the present dungeon.
                 */
                newlevel.dnum = u.uz.dnum;
                newlevel.dlevel = u.uz.dlevel + 1;
                goto_level(&newlevel, FALSE, TRUE, FALSE, FALSE);
                /* messages for arriving in special rooms */
                spoteffects(FALSE);
            }
        } else {
            if (shopdoor && madeby_u)
                pay_for_damage("ruin", FALSE);
            if (newobjs)
                impact_drop((struct obj *) 0, x, y, 0);
            if (mtmp) {
                /*[don't we need special sokoban handling here?]*/
                if (is_flying(mtmp) || is_levitating(mtmp)
                    || mtmp->data == &mons[PM_WUMPUS]
                    || (mtmp->wormno && count_wsegs(mtmp) > 5)
                    || mtmp->data->msize >= MZ_HUGE)
                    return;
                if (mtmp == u.ustuck) /* probably a vortex */
                    return;           /* temporary? kludge */

                if (teleport_pet(mtmp, FALSE)) {
                    d_level tolevel;

                    if (Is_stronghold(&u.uz)) {
                        assign_level(&tolevel, &valley_level);
                    } else if (Is_botlevel(&u.uz)) {
                        if (canseemon(mtmp))
                            pline("%s avoids the trap.", Monnam(mtmp));
                        return;
                    } else {
                        get_level(&tolevel, depth(&u.uz) + 1);
                    }
                    if (mtmp->isshk)
                        make_angry_shk(mtmp, 0, 0);
                    play_sfx_sound_at_location(SFX_TRAP_DOOR_OPENS, mtmp->mx, mtmp->my);
                    migrate_to_level(mtmp, ledger_no(&tolevel),
                                     MIGR_RANDOM, (coord *) 0);
                }
            }
        }
    }
}

/*
 * Called from dighole(), but also from do_break_wand()
 * in apply.c.
 */
void
liquid_flow(x, y, typ, ttmp, fillmsg)
xchar x, y;
schar typ;
struct trap *ttmp;
const char *fillmsg;
{
    boolean u_spot = (x == u.ux && y == u.uy);

    if (ttmp)
        (void) delfloortrap(ttmp);
    /* if any objects were frozen here, they're released now */
    (void)unearth_objs(&youmonst, x, y, FALSE, FALSE);

    if (fillmsg)
        pline(fillmsg, hliquid(typ == LAVAPOOL ? "lava" : "water"));
    if (u_spot && !Moves_above_ground) {
        if (typ == LAVAPOOL)
            (void) lava_effects();
        else if (!Walks_on_water)
            (void) drown();
    }
}

/* return TRUE if digging succeeded, FALSE otherwise */
boolean
dighole(pit_only, by_magic, cc)
boolean pit_only, by_magic;
coord *cc;
{
    register struct trap *ttmp;
    struct rm *lev;
    struct obj *boulder_here;
    schar typ;
    xchar dig_x, dig_y;
    boolean nohole;

    if (!cc) {
        dig_x = u.ux;
        dig_y = u.uy;
    } else {
        dig_x = cc->x;
        dig_y = cc->y;
        if (!isok(dig_x, dig_y))
            return FALSE;
    }

    ttmp = t_at(dig_x, dig_y);
    lev = &levl[dig_x][dig_y];
    nohole = (!Can_dig_down(&u.uz) && !lev->candig);

    if ((ttmp && ((trap_type_definitions[ttmp->ttyp].tdflags & TRAPDEF_FLAGS_NOT_OVERRIDEN) != 0 || nohole))
        || (IS_ROCK(lev->typ) && lev->typ != SDOOR
            && (lev->wall_info & W_NONDIGGABLE) != 0)) {
        play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
        pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "%s %shere is too hard to dig in.", surface(dig_x, dig_y),
                  (dig_x != u.ux || dig_y != u.uy) ? "t" : "");
    } else if (is_pool_or_lava(dig_x, dig_y)) {
        play_sfx_sound_at_location(SFX_SPLASH_HIT, dig_x, dig_y);
        pline_The("%s sloshes furiously for a moment, then subsides.",
                  hliquid(is_lava(dig_x, dig_y) ? "lava" : "water"));
        wake_nearby(); /* splashing */

    } else if (lev->typ == DRAWBRIDGE_DOWN
               || (is_drawbridge_wall(dig_x, dig_y) >= 0)) {
        /* drawbridge_down is the platform crossing the moat when the
           bridge is extended; drawbridge_wall is the open "doorway" or
           closed "door" where the portcullis/mechanism is located */
        if (pit_only) {
            play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "drawbridge seems too hard to dig through.");
            return FALSE;
        } else {
            int x = dig_x, y = dig_y;
            /* if under the portcullis, the bridge is adjacent */
            (void) find_drawbridge(&x, &y);
            destroy_drawbridge(x, y, FALSE);
            return TRUE;
        }

    } else if ((boulder_here = sobj_at(BOULDER, dig_x, dig_y)) != 0) {
        if (ttmp && is_pit(ttmp->ttyp)
            && rn2(2)) {
            play_sfx_sound(SFX_BOULDER_FILLS_PIT);
            pline_The("boulder settles into the %spit.",
                      (dig_x != u.ux || dig_y != u.uy) ? "adjacent " : "");
            ttmp->ttyp = PIT; /* crush spikes */
            ttmp->tsubtyp = 0;
            ttmp->tflags = 0;
            ttmp->activation_count = 0;
        } else {
            /*
             * digging makes a hole, but the boulder immediately
             * fills it.  Final outcome:  no hole, no boulder.
             */
            play_sfx_sound(SFX_BOULDER_FILLS_PIT);
            pline("KADOOM!  The boulder falls in!");
            (void) delfloortrap(ttmp);
        }
        delobj(boulder_here);
        return TRUE;

    } else if (IS_GRAVE(lev->typ)) {
        digactualhole(dig_x, dig_y, BY_YOU, PIT);
        dig_up_grave(cc);
        return TRUE;
    } else if (lev->typ == DRAWBRIDGE_UP) {
        /* must be floor or ice, other cases handled above */
        /* dig "pit" and let fluid flow in (if possible) */
        typ = fillholetyp(dig_x, dig_y, FALSE);

        if (IS_FLOOR(typ)) {
            /*
             * We can't dig a hole here since that will destroy
             * the drawbridge.  The following is a cop-out. --dlc
             */
            play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "%s %shere is too hard to dig in.",
                      surface(dig_x, dig_y),
                      (dig_x != u.ux || dig_y != u.uy) ? "t" : "");
            return FALSE;
        }

        lev->drawbridgemask &= ~DB_UNDER;
        lev->drawbridgemask |= (typ == LAVAPOOL) ? DB_LAVA : DB_MOAT;
        liquid_flow(dig_x, dig_y, typ, ttmp,
                    "As you dig, the hole fills with %s!");
        return TRUE;

    /* the following two are here for the wand of digging */
    } 
    else if (IS_THRONE(lev->typ)) 
    {
        play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
        pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "throne is too hard to break apart.");

    }
    else if (IS_ANVIL(lev->typ)) 
    {
        play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
        pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "anvil is too hard to break apart.");

    } 
    else if (IS_ALTAR(lev->typ)) 
    {
        play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
        pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "altar is too hard to break apart.");

    } 
    else 
    {
        typ = fillholetyp(dig_x, dig_y, FALSE);

        lev->flags = 0;
        if (!IS_FLOOR(typ))
        {
            create_basic_floor_location(dig_x, dig_y, typ, 0, 0, FALSE);
            liquid_flow(dig_x, dig_y, typ, ttmp,
                        "As you dig, the hole fills with %s!");
            return TRUE;
        }

        /* magical digging disarms settable traps */
        if (by_magic && ttmp
            && (ttmp->ttyp == LANDMINE || ttmp->ttyp == BEAR_TRAP)) {
            int otyp = (ttmp->ttyp == LANDMINE) ? LAND_MINE : BEARTRAP;

            /* convert trap into buried object (deletes trap) */
            cnv_trap_obj(otyp, 1, ttmp, TRUE, FALSE);
        }

        /* finally we get to make a hole */
        if (nohole || pit_only)
            digactualhole(dig_x, dig_y, BY_YOU, PIT);
        else
            digactualhole(dig_x, dig_y, BY_YOU, HOLE);

        return TRUE;
    }

    return FALSE;
}

STATIC_OVL void
dig_up_grave(cc)
coord *cc;
{
    struct obj *otmp = (struct obj*)0;
    xchar dig_x, dig_y;

    if (!cc) {
        dig_x = u.ux;
        dig_y = u.uy;
    } else {
        dig_x = cc->x;
        dig_y = cc->y;
        if (!isok(dig_x, dig_y))
            return;
    }

    /* Grave-robbing is frowned upon... */
    exercise(A_WIS, FALSE);
    if (Role_if(PM_ARCHAEOLOGIST) && (u.ualign.type != A_CHAOTIC)) {
        play_sfx_sound(SFX_GUILTY);
        adjalign(-sgn(u.ualign.type) * 3);
        You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "like a despicable grave-robber!");
    } else if (Role_if(PM_SAMURAI)) {
        play_sfx_sound(SFX_GUILTY);
        adjalign(-sgn(u.ualign.type));
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "disturb the honorable dead!");
    } else if ((u.ualign.type == A_LAWFUL) && (u.ualign.record > -10)) {
        play_sfx_sound(SFX_GUILTY);
        adjalign(-sgn(u.ualign.type));
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "have violated the sanctity of this grave!");
    }

    int itemsfound = unearth_objs(&youmonst, u.ux, u.uy, TRUE, FALSE);
    boolean doautopickup = itemsfound > 0;
    if (!itemsfound)
    {
        switch (rn2(5)) {
        case 0:
            play_sfx_sound(SFX_UNEARTHED_OBJECT_FOUND);
            You("unearth some bones.");
            (void)mksobj_found_at(BONE, dig_x, dig_y, TRUE, FALSE);
            (void)mksobj_found_at(BONE, dig_x, dig_y, TRUE, FALSE);
            if (!rn2(2))
                (void)mksobj_found_at(HUMAN_SKULL, dig_x, dig_y, TRUE, FALSE);
            doautopickup = TRUE;
            break;
        case 1:
            play_sfx_sound(SFX_UNEARTHED_OBJECT_FOUND);
            You("unearth a corpse.");
            if ((otmp = mk_tt_object(CORPSE, dig_x, dig_y)) != 0)
            {
                otmp->age -= 100; /* this is an *OLD* corpse */
                obj_set_found(otmp);
                doautopickup = TRUE;
            }
            break;
        case 2:
            if (!Blind)
            {
                play_sfx_sound(SFX_SURPRISE_ATTACK);
                pline_ex(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_WARNING, Hallucination ? "Dude!  The living dead!"
                    : "The grave's owner is very upset!");
            }
            (void)makemon(mkclass(S_LESSER_UNDEAD, 0), dig_x, dig_y, NO_MM_FLAGS);
            break;
        case 3:
            if (!Blind)
            {
                play_sfx_sound(SFX_SURPRISE_ATTACK);
                pline_ex(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : CLR_MSG_WARNING, Hallucination ? "I want my mummy!"
                    : "You've disturbed a tomb!");
            }
            otmp = mksobj_at_with_flags(SARCOPHAGUS, dig_x, dig_y, FALSE, FALSE, 0, (struct monst*)0, MAT_NONE, 0L, 0L, MKOBJ_FLAGS_OPEN_COFFIN);
            if (otmp)
            {
                boolean dealloc = FALSE;
                (void)bury_an_obj(otmp, &dealloc);
                if (dealloc)
                    otmp = 0;
            }
            (void)makemon(mkclass(S_GREATER_UNDEAD, 0), dig_x, dig_y, NO_MM_FLAGS);
            break;
        default:
            /* No corpse */
            play_sfx_sound(SFX_NOTHING_FOUND);
            pline_The("grave seems unused.  Strange....");
            break;
        }
    }
    del_engr_at(dig_x, dig_y);
    int typ = levl[dig_x][dig_y].floortyp ? levl[dig_x][dig_y].floortyp : GROUND;
    int subtyp = levl[dig_x][dig_y].floorsubtyp ? levl[dig_x][dig_y].floorsubtyp : get_initial_location_subtype(typ);
    int vartyp = levl[dig_x][dig_y].floorvartyp ? levl[dig_x][dig_y].floorvartyp : get_initial_location_vartype(typ, subtyp);
    create_simple_location(dig_x, dig_y, typ, subtyp, vartyp, 0, back_to_broken_glyph(dig_x, dig_y), 0, 0, 0, TRUE);
    if (doautopickup)
        (void)pickup(1, FALSE);
    return;
}

int
use_pick_axe(obj)
struct obj *obj;
{
    if (!obj)
        return 0;

    const char *sdp, *verb;
    char *dsp, dirsyms[12], qbuf[BUFSZ];
    boolean ispick, isaxe, issaw;
    int rx, ry, downok, res = 0;

    ispick = is_pick(obj);
    isaxe = is_axe(obj);
    issaw = is_saw(obj);

    /* Check tool */
    if (!(obj == uwep || (u.twoweap && obj == uarms))) 
    {
        if (!wield_tool(obj, issaw ? "use" : "swing"))
            return 0;
        else
            res = 1;
    }
    verb = ispick ? "dig" : isaxe ? "chop" : issaw ? "cut" : "dig";

    if (u.utrap && u.utraptype == TT_WEB) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s you can't %s while entangled in a web.",
              /* res==0 => no prior message;
                 res==1 => just got "You now wield a pick-axe." message */
              !res ? "Unfortunately," : "But", verb);
        return res;
    }

    /*
    if (isshovel)
    {
        if(!can_reach_floor(FALSE))
        {
            pline("You cannot reach the floor.");
            return res;
        }
        else if (!(levl[u.ux][u.uy].typ == GRASS || IS_GRAVE(levl[u.ux][u.uy].typ)))
        {
            pline("It is too hard to dig here with a shovel.");
            return res;
        }
        else
        {
            u.dx = 0;
            u.dy = 0;
            u.dz = 1;
            return use_pick_axe2(obj);
        }
    }
    */
    /* construct list of directions to show player for likely choices */
    downok = !!can_reach_floor(FALSE);
    dsp = dirsyms;
    for (sdp = Cmd.dirchars; *sdp; ++sdp) {
        /* filter out useless directions */
        if (u.uswallow) {
            ; /* all directions are viable when swallowed */
        } else if (movecmd(*sdp)) {
            /* normal direction, within plane of the level map;
               movecmd() sets u.dx, u.dy, u.dz and returns !u.dz */
            if (!dxdy_moveok())
                continue; /* handle NODIAG */
            rx = u.ux + u.dx;
            ry = u.uy + u.dy;
            if (!isok(rx, ry) || dig_typ(obj, rx, ry) == DIGTYP_UNDIGGABLE)
                continue;
        } else {
            /* up or down; we used to always include down, so that
               there would always be at least one choice shown, but
               it shouldn't be a likely candidate when floating high
               above the floor; include up instead in that situation
               (as a silly candidate rather than a likely one...) */
            if ((u.dz > 0) ^ downok)
                continue;
        }
        /* include this direction */
        *dsp++ = *sdp;
    }
    *dsp = 0;
    Sprintf(qbuf, "In what direction do you want to %s? [%s]", verb, dirsyms);
    if (!getdir(qbuf))
        return res;

    update_u_facing(TRUE);

    return use_pick_axe2(obj);
}

/* MRKR: use_pick_axe() is split in two to allow autodig to bypass */
/*       the "In what direction do you want to dig?" query.        */
/*       use_pick_axe2() uses the existing u.dx, u.dy and u.dz    */
int
use_pick_axe2(obj)
struct obj *obj;
{
    register int rx, ry;
    register struct rm *lev;
    struct trap *trap, *trap_with_u;
    int dig_target;
    boolean ispick = is_pick(obj);
    boolean isaxe = is_axe(obj);
    boolean issaw = is_saw(obj);
    const char *verbing = ispick ? "digging" : isaxe ? "chopping" : "cutting";
    boolean resume = FALSE;
    boolean action_taken = FALSE;

    if (u.uswallow && attack(u.ustuck))
    {
        ; /* return 1 */
    } 
    else if (Underwater)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "Turbulence torpedoes your %s attempts.", verbing);
    } 
    else if (u.dz < 0) 
    {
        if (Levitation)
        {
            play_sfx_sound(SFX_GENERAL_NOT_ENOUGH_LEVERAGE);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough leverage.");
        }
        else
        {
            play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "reach the %s.", ceiling(u.ux, u.uy));
        }
    } 
    else if (!u.dx && !u.dy && !u.dz)
    {
        char buf[BUFSZ];
        int dam;

        if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
        {
            update_u_action(ACTION_TILE_ATTACK);
            action_taken = TRUE;
        }
        dam = rnd(2) + u_str_dmg_bonus() + obj->enchantment;
        if (dam <= 0)
            dam = 1;
        play_object_hit_sound(obj, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(&youmonst), dam, HMON_MELEE);
        You("hit yourself with %s.", yname(obj));
        Sprintf(buf, "%s own %s", uhis(), OBJ_NAME(objects[obj->otyp]));
        losehp(adjust_damage(dam, &youmonst, &youmonst, AD_PHYS, ADFLAGS_NONE), buf, KILLED_BY);
        if (action_taken)
            update_u_action_revert(ACTION_TILE_NO_ACTION);
        context.botl = 1;
        return 1;
    }
    else if (u.dz == 0)
    {
        if (Stunned || (Confusion && !rn2(5)))
            confdir();
        rx = u.ux + u.dx;
        ry = u.uy + u.dy;
        if (!isok(rx, ry))
        {
            if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
            {
                update_u_action(ACTION_TILE_ATTACK);
                action_taken = TRUE;
            }

            play_occupation_immediate_sound(objects[obj->otyp].oc_soundset, issaw ? OCCUPATION_CUTTING_TREE : OCCUPATION_DIGGING_ROCK, OCCUPATION_SOUND_TYPE_START);
            pline("Clash!");

            if (action_taken)
                update_u_action_revert(ACTION_TILE_NO_ACTION);
            return 1;
        }
        lev = &levl[rx][ry];
        if (MON_AT(rx, ry) && attack(m_at(rx, ry)))
            return 1;
        dig_target = dig_typ(obj, rx, ry);
        if (dig_target == DIGTYP_UNDIGGABLE) 
        {
            /* ACCESSIBLE or POOL */
            trap = t_at(rx, ry);
            if (trap && trap->ttyp == WEB)
            {
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_SWING_MELEE);
                if (!trap->tseen)
                {
                    seetrap(trap);
                    There("is a spider web there!");
                }
                pline("%s entangled in the web.", Yobjnam2(obj, "become"));
                /* you ought to be able to let go; tough luck */
                /* (maybe `move_into_trap()' would be better) */
                nomul(-d(2, 2));
                multi_reason = "stuck in a spider web";
                nomovemsg = "You pull free.";
            } 
            else if (lev->typ == IRONBARS) 
            {
                play_object_hit_sound(obj, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(rx, ry), 0, HMON_MELEE);
                pline("Clang!");
                wake_nearby();
            }
            else if (IS_TREE(lev->typ)) 
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "need an axe or a saw to cut down a tree.");
            }
            else if (IS_ROCK(lev->typ)) 
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "need a pick to dig rock.");
            }
            else if (!ispick && ((sobj_at(STATUE, rx, ry) && !issaw) || sobj_at(BOULDER, rx, ry)))
            {
                if (isaxe)
                {
                    update_u_action(ACTION_TILE_ATTACK);

                    boolean vibrate = !rn2(3);
                    play_object_hit_sound(obj, HIT_SURFACE_SOURCE_OBJECT, obj_to_any(sobj_at(STATUE, rx, ry)), vibrate ? 2 : 0, HMON_MELEE);
                    pline("Sparks fly as you whack the %s.%s",
                        sobj_at(STATUE, rx, ry) ? "statue" : "boulder",
                        vibrate ? " The axe-handle vibrates violently!" : "");
                    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
                    if (vibrate)
                        losehp(adjust_damage(2, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "axing a hard object",
                            KILLED_BY);

                    update_u_action_revert(ACTION_TILE_NO_ACTION);
                }
                else if(issaw)
                {
                    play_object_hit_sound(obj, HIT_SURFACE_SOURCE_OBJECT, obj_to_any(sobj_at(STATUE, rx, ry)), 0, HMON_MELEE);
                    pline("Sparks fly as you cut the %s, but nothing much else happens.",
                        sobj_at(STATUE, rx, ry) ? "statue" : "boulder");
                    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_GENERAL_EFFECT2);
                }
            }
            else if (u.utrap && u.utraptype == TT_PIT && trap
                       && (trap_with_u = t_at(u.ux, u.uy))
                       && is_pit(trap->ttyp)
                       && !conjoined_pits(trap, trap_with_u, FALSE)) 
            {
                int idx;
                if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
                {
                    update_u_action(ACTION_TILE_ATTACK);
                    action_taken = TRUE;
                }
                play_occupation_immediate_sound(objects[obj->otyp].oc_soundset, issaw ? OCCUPATION_CUTTING_TREE : OCCUPATION_DIGGING_ROCK, OCCUPATION_SOUND_TYPE_START);

                for (idx = 0; idx < 8; idx++) 
                {
                    if (xdir[idx] == u.dx && ydir[idx] == u.dy)
                        break;
                }
                /* idx is valid if < 8 */
                if (idx < 8) {
                    int adjidx = (idx + 4) % 8;

                    trap_with_u->conjoined |= (1 << idx);
                    trap->conjoined |= (1 << adjidx);
                    pline("You clear some debris from between the pits.");
                    play_occupation_immediate_sound(objects[obj->otyp].oc_soundset, issaw ? OCCUPATION_CUTTING_TREE : OCCUPATION_DIGGING_ROCK, OCCUPATION_SOUND_TYPE_FINISH);
                }
                if (action_taken)
                    update_u_action_revert(ACTION_TILE_NO_ACTION);
            } 
            else if (u.utrap && u.utraptype == TT_PIT
                       && (trap_with_u = t_at(u.ux, u.uy)) != 0)
            {
                if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
                {
                    update_u_action(ACTION_TILE_ATTACK);
                    action_taken = TRUE;
                }
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_SWING_MELEE);
                You("swing %s, but the rubble has no place to go.",
                    yobjnam(obj, (char *) 0));
                play_occupation_immediate_sound(objects[obj->otyp].oc_soundset, issaw ? OCCUPATION_CUTTING_TREE : OCCUPATION_DIGGING_ROCK, OCCUPATION_SOUND_TYPE_START);
                if (action_taken)
                    update_u_action_revert(ACTION_TILE_NO_ACTION);
            }
            else 
            {
                if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
                {
                    update_u_action(ACTION_TILE_ATTACK);
                    action_taken = TRUE;
                }
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_SWING_MELEE);
                You("swing %s through thin air.", yobjnam(obj, (char *) 0));
                if (action_taken)
                    update_u_action_revert(ACTION_TILE_NO_ACTION);
            }
        }
        else 
        {
            STATIC_VAR const char *const d_action[6] = { "swinging", 
                                                     "digging",
                                                     "chipping the statue",
                                                     "hitting the boulder",
                                                     "chopping at the door",
                                                     "cutting the tree" };

            STATIC_VAR const char* const d_action_saw[6] = { "positioning the saw",
                                         "cutting",
                                         "cutting the statue",
                                         "cutting the boulder",
                                         "cutting at the door",
                                         "cutting the tree" };

            STATIC_VAR const int /*enum object_occupation_types*/ d_action_soundset[6] = { OCCUPATION_SWINGING,
                                         OCCUPATION_DIGGING_ROCK,
                                         OCCUPATION_DIGGING_ROCK,
                                         OCCUPATION_DIGGING_ROCK,
                                         OCCUPATION_HITTING_DOOR,
                                         OCCUPATION_CUTTING_TREE };

            did_dig_msg = FALSE;
            context.digging.quiet = FALSE;
            if (context.digging.pos.x != rx || context.digging.pos.y != ry
                || !on_level(&context.digging.level, &u.uz)
                || context.digging.down) 
            {
                if (flags.autodig && dig_target == DIGTYP_ROCK
                    && !context.digging.down
                    && context.digging.pos.x == u.ux
                    && context.digging.pos.y == u.uy
                    && (moves <= context.digging.lastdigtime + 2
                        && moves >= context.digging.lastdigtime)) 
                {
                    /* avoid messages if repeated autodigging */
                    did_dig_msg = TRUE;
                    context.digging.quiet = TRUE;
                }
                context.digging.down = context.digging.chew = FALSE;
                context.digging.warned = FALSE;
                context.digging.pos.x = rx;
                context.digging.pos.y = ry;
                assign_level(&context.digging.level, &u.uz);
                context.digging.effort = 0;
                context.digging.skill = (int)weapon_skill_type(obj);
                if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
                {
                    update_u_action(ACTION_TILE_ATTACK);
                    action_taken = TRUE;
                }
                if (!context.digging.quiet)
                {
                    You("start %s.", issaw ? d_action_saw[dig_target] : d_action[dig_target]);
                }
                if (action_taken)
                    update_u_action_revert(ACTION_TILE_NO_ACTION);
            }
            else 
            {
                if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
                {
                    update_u_action(ACTION_TILE_ATTACK);
                    action_taken = TRUE;
                }
                You("%s %s.", context.digging.chew ? "begin" : "continue",
                    issaw ? d_action_saw[dig_target] : d_action[dig_target]);
                context.digging.chew = FALSE;
                if (action_taken)
                    update_u_action_revert(ACTION_TILE_NO_ACTION);
            }
            set_occupation(dig, verbing, objects[obj->otyp].oc_soundset, 
                d_action_soundset[dig_target],
                context.digging.chew ? OCCUPATION_SOUND_TYPE_START : OCCUPATION_SOUND_TYPE_RESUME, 0);
        }
    } 
    else if (Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)) 
    {
        /* it must be air -- water checked above */
        You("swing %s through thin air.", yobjnam(obj, (char *) 0));
        if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
        {
            update_u_action(ACTION_TILE_ATTACK);
            action_taken = TRUE;
        }
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_SWING_MELEE);
        if (action_taken)
            update_u_action_revert(ACTION_TILE_NO_ACTION);
    }
    else if (!can_reach_floor(FALSE)) 
    {
        cant_reach_floor(u.ux, u.uy, FALSE, FALSE);
    }
    else if (is_pool_or_lava(u.ux, u.uy)) 
    {
        /* Monsters which swim also happen not to be able to dig */
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot stay under%s long enough.",
            is_pool(u.ux, u.uy) ? "water" : " the lava");
    }
    else if ((trap = t_at(u.ux, u.uy)) != 0
               && uteetering_at_seen_pit(trap)) 
    {
        dotrap(trap, FORCEBUNGLE);
        /* might escape trap and still be teetering at brink */
        if (!u.utrap)
            cant_reach_floor(u.ux, u.uy, FALSE, TRUE);
    } 
    else if (!ispick
               /* can only dig down with an axe when doing so will
                  trigger or disarm a trap here */
               && (!trap || (trap->ttyp != LANDMINE
                             && trap->ttyp != BEAR_TRAP))) 
    {
        update_u_action(ACTION_TILE_ATTACK);
        pline("%s merely scratches the %s.", Yobjnam2(obj, (char *) 0),
              surface(u.ux, u.uy));
        u_wipe_engr(3);

        if(trap)
            play_object_hit_sound(obj, HIT_SURFACE_SOURCE_TRAP, trap_to_any(trap), 0, HMON_MELEE);
        else
            play_object_hit_sound(obj, HIT_SURFACE_SOURCE_LOCATION, xy_to_any(u.ux, u.uy), 0, HMON_MELEE);

        update_u_action_revert(ACTION_TILE_NO_ACTION);

    } 
    else 
    {
        if (context.digging.pos.x != u.ux || context.digging.pos.y != u.uy
            || !on_level(&context.digging.level, &u.uz)
            || !context.digging.down) 
        {
            context.digging.chew = FALSE;
            context.digging.down = TRUE;
            context.digging.warned = FALSE;
            context.digging.pos.x = u.ux;
            context.digging.pos.y = u.uy;
            assign_level(&context.digging.level, &u.uz);
            context.digging.effort = 0;
            context.digging.skill = (int)weapon_skill_type(obj);
            if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
            {
                update_u_action(ACTION_TILE_ATTACK);
                action_taken = TRUE;
            }
            You("start %s downward.", verbing);
            if (*u.ushops)
                shopdig(0);
            if (action_taken)
                update_u_action_revert(ACTION_TILE_NO_ACTION);
        }
        else
        {
            if (u_action_flags(ACTION_TILE_ATTACK) & ACTION_ATTACK_FLAGS_PICK_AXE)
            {
                update_u_action(ACTION_TILE_ATTACK);
                action_taken = TRUE;
            }
            You("continue %s downward.", verbing);
            if (action_taken)
                update_u_action_revert(ACTION_TILE_NO_ACTION);
            resume = TRUE;
        }
        did_dig_msg = FALSE;
        set_occupation(dig, verbing, objects[obj->otyp].oc_soundset,
            (context.digging.down && (levl[u.ux][u.uy].typ == GROUND || levl[u.ux][u.uy].typ == GRASS ||
                levl[u.ux][u.uy].floortyp == GROUND || levl[u.ux][u.uy].floortyp == GRASS)) 
            ? OCCUPATION_DIGGING_GROUND : OCCUPATION_DIGGING_ROCK, 
            resume ? OCCUPATION_SOUND_TYPE_RESUME : OCCUPATION_SOUND_TYPE_START, 0);
    }
    return 1;
}

/*
 * Town Watchmen frown on damage to the town walls, trees or fountains.
 * It's OK to dig holes in the ground, however.
 * If mtmp is assumed to be a watchman, a watchman is found if mtmp == 0
 * zap == TRUE if wand/spell of digging, FALSE otherwise (chewing)
 */
void
watch_dig(mtmp, x, y, zap)
struct monst *mtmp;
xchar x, y;
boolean zap;
{
    struct rm *lev = &levl[x][y];

    if (in_town(x, y)
        && (closed_door(x, y) || IS_WALL_OR_SDOOR(lev->typ)
            || IS_FOUNTAIN(lev->typ) || IS_TREE(lev->typ))) {
        if (!mtmp) {
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
                if (DEADMONSTER(mtmp))
                    continue;
                if (is_watch(mtmp->data) && !is_blinded(mtmp) && m_canseeu(mtmp)
                    && couldsee(mtmp->mx, mtmp->my) && is_peaceful(mtmp))
                    break;
            }
        }

        if (mtmp) {
            if (zap || context.digging.warned) {
                play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_HALT_VANDAL_YOURE_UNDER_ARREST);
                context.global_minimum_volume = 0.25f;
                verbalize_angry1("Halt, vandal!  You're under arrest!");
                context.global_minimum_volume = 0.0f;
                (void) angry_guards(!!Deaf);
            } else {
                const char *str;
                int dialogueline = 0;

                if (IS_DOOR(lev->typ))
                {
                    str = "door";
                    dialogueline = WATCHMAN_LINE_HEY_STOP_DAMAGING_THAT_DOOR;
                }
                else if (IS_TREE(lev->typ))
                {
                    str = "tree";
                    dialogueline = WATCHMAN_LINE_HEY_STOP_DAMAGING_THAT_TREE;
                }
                else if (IS_ROCK(lev->typ))
                {
                    str = "wall";
                    dialogueline = WATCHMAN_LINE_HEY_STOP_DAMAGING_THAT_WALL;
                }
                else
                {
                    str = "fountain";
                    dialogueline = WATCHMAN_LINE_HEY_STOP_DAMAGING_THAT_FOUNTAIN;
                }


                context.global_minimum_volume = 0.25f;
                play_monster_special_dialogue_line(mtmp, dialogueline);
                context.global_minimum_volume = 0.0f;
                verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "Hey, stop damaging that %s!", str);
                context.digging.warned = TRUE;
            }
            if (is_digging())
                stop_occupation();
        }
    }
}

/* Return TRUE if monster died, FALSE otherwise.  Called from m_move(). */
boolean
mdig_tunnel(mtmp)
register struct monst *mtmp;
{
    register struct rm *here;
    int pile = rnd(12);

    here = &levl[mtmp->mx][mtmp->my];
    if (here->typ == SDOOR)
        cvt_sdoor_to_door(mtmp->mx, mtmp->my); /* ->typ = DOOR */

    /* Eats away door if present & closed or locked */
    if (closed_door(mtmp->mx, mtmp->my)) 
    {
        if (!m_can_destroy_door(mtmp, here, TRUE) || !is_door_diggable_at_ptr(here))
            return FALSE;

        if (*in_rooms(mtmp->mx, mtmp->my, SHOPBASE))
            add_damage(mtmp->mx, mtmp->my, 0L);

        unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my); /* vision */

        if (here->doormask & D_TRAPPED) 
        {
            here->doormask &= ~D_MASK;
            if (is_door_destroyed_by_booby_trap_at_ptr(here))
            {
                here->doormask |= D_NODOOR;
                here->subtyp = 0;
            }
            else
            {
                here->doormask |= D_BROKEN;
            }
            if (mb_trapped(mtmp)) { /* mtmp is killed */
                newsym(mtmp->mx, mtmp->my);
                return TRUE;
            }
        }
        else 
        {
            if (!rn2(3) && flags.verbose) /* not too often.. */
                draft_message(TRUE); /* "You feel an unexpected draft." */
            here->doormask &= ~D_MASK;
            here->doormask |= D_BROKEN;
        }
        newsym(mtmp->mx, mtmp->my);
        return FALSE;
    } 
    else if (here->typ == SCORR) 
    {
        create_basic_floor_location(mtmp->mx, mtmp->my, levl[mtmp->mx][mtmp->my].floortyp ? levl[mtmp->mx][mtmp->my].floortyp : CORR, levl[mtmp->mx][mtmp->my].floortyp ? levl[mtmp->mx][mtmp->my].floorsubtyp : get_initial_location_subtype(CORR), 0, FALSE);
        unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my);
        newsym(mtmp->mx, mtmp->my);
        draft_message(FALSE); /* "You feel a draft." */
        return FALSE;
    }
    else if (!IS_ROCK(here->typ) && !IS_TREE(here->typ))
    { /* no dig */
        return FALSE;
    }

    /* Only rock, trees, and walls fall through to this point. */
    if ((here->wall_info & W_NONDIGGABLE) != 0) {
        impossible("mdig_tunnel:  %s at (%d,%d) is undiggable",
                   (IS_WALL(here->typ) ? "wall"
                    : IS_TREE(here->typ) ? "tree" : "stone"),
                   (int) mtmp->mx, (int) mtmp->my);
        return FALSE; /* still alive */
    }

    if (IS_WALL(here->typ))
    {
        /* KMH -- Okay on arboreal levels (room walls are still stone) */
        if (flags.verbose && !rn2(5))
        {
            context.global_minimum_volume = 0.15f;
            play_simple_location_sound(mtmp->mx, mtmp->my, LOCATION_SOUND_TYPE_BREAK);
            context.global_minimum_volume = 0.0f;
            You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "crashing rock.");
        }
        if (*in_rooms(mtmp->mx, mtmp->my, SHOPBASE))
            add_damage(mtmp->mx, mtmp->my, 0L);

        int ltype = 0;
        int lsubtype = 0;
        int lvartype = 0;
        int lflags = 0;

//        if (here->floortyp)
//            ltype = here->floortyp, lsubtype = here->floorsubtyp, lvartype = here->floorvartyp;
//        else
        {
            if (level.flags.is_maze_lev)
            {
                ltype = CORR, lflags = 0; //ROOM
            }
            else if (level.flags.is_cavernous_lev
                && !in_town(mtmp->mx, mtmp->my))
            {
                ltype = CORR, lflags = 0;
            }
            else
            {
                ltype = DOOR, lflags = D_NODOOR;
            }

            lsubtype = get_initial_location_subtype(ltype);
            lvartype = get_initial_location_vartype(ltype, lsubtype);
        }

        create_simple_location(mtmp->mx, mtmp->my, ltype, lsubtype, lvartype, lflags, back_to_broken_glyph(mtmp->mx, mtmp->my), !IS_FLOOR(ltype) ? here->floortyp : 0, !IS_FLOOR(ltype) ? here->floorsubtyp : 0, !IS_FLOOR(ltype) ? here->floorvartyp : 0, FALSE);
    }
    else if (IS_TREE(here->typ))
    {
        struct mkroom* r = which_room(mtmp->mx, mtmp->my);
        int ltype = 0;
        int lsubtype = 0;
        int lvartype = 0;

        uncatch_tree_objects(mtmp->mx, mtmp->my);

        if(here->floortyp)
            ltype = here->floortyp, lsubtype = here->floorsubtyp, lvartype = here->floorvartyp;
        else if (r && r->orig_rtype == GARDEN)
        {
            ltype = GRASS;
            lsubtype = get_initial_location_subtype(ltype);
            lvartype = get_initial_location_vartype(ltype, lsubtype);
        }
        else
        {
            ltype = ROOM;
            lsubtype = get_initial_location_subtype(ltype);
            lvartype = get_initial_location_vartype(ltype, lsubtype);
        }

        if (here->special_quality > 0 && tree_subtype_definitions[here->subtyp].fruit_type > STRANGE_OBJECT)
        {
            struct obj* otmp = mksobj_at(tree_subtype_definitions[here->subtyp].fruit_type, mtmp->mx, mtmp->my, TRUE, FALSE); //rnd_treefruit_at(mtmp->mx, mtmp->my);
            otmp->quan = here->special_quality;
            otmp->owt = weight(otmp);
        }

        create_simple_location(mtmp->mx, mtmp->my, ltype, lsubtype, lvartype, 0, back_to_broken_glyph(mtmp->mx, mtmp->my), 0, 0, 0, FALSE);

#if 0
        if (pile && pile < 5)
        {
            struct obj* otmp = rnd_treefruit_at(mtmp->mx, mtmp->my);
            if (otmp && (objects[otmp->otyp].oc_edible_effect == EDIBLEFX_NO_EFFECT || objects[otmp->otyp].oc_edible_effect == EDIBLEFX_APPLE))
            {
                otmp->quan = rnd(16) + 4;
                otmp->owt = weight(otmp);
            }
        }
#endif
    }
    else
    {
        create_basic_floor_location(mtmp->mx, mtmp->my, levl[mtmp->mx][mtmp->my].floortyp ? levl[mtmp->mx][mtmp->my].floortyp : CORR, levl[mtmp->mx][mtmp->my].floortyp ? levl[mtmp->mx][mtmp->my].floorsubtyp : get_initial_location_subtype(CORR), 0, FALSE);

        if (pile && pile < 5)
            (void) mksobj_at((pile == 1) ? BOULDER : ROCK, mtmp->mx, mtmp->my,
                             TRUE, FALSE);
    }
    newsym(mtmp->mx, mtmp->my);
    if (!sobj_at(BOULDER, mtmp->mx, mtmp->my))
        unblock_vision_and_hearing_at_point(mtmp->mx, mtmp->my); /* vision */

    return FALSE;
}

#define STRIDENT 4 /* from pray.c */

/* draft refers to air currents, but can be a pun on "draft" as conscription
   for military service (probably not a good pun if it has to be explained) */
void
draft_message(unexpected)
boolean unexpected;
{
    /*
     * [Bug or TODO?  Have caller pass coordinates and use the travel
     * mechanism to determine whether there is a path between
     * destroyed door (or exposed secret corridor) and hero's location.
     * When there is no such path, no draft should be felt.]
     */

    if (unexpected) {
        if (!Hallucination)
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "an unexpected draft.");
        else
            /* U.S. classification system uses 1-A for eligible to serve
               and 4-F for ineligible due to physical or mental defect;
               some intermediate values exist but are rarely seen */
            You_feel_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "like you are %s.",
                     (ACURR(A_STR) < 6 || ACURR(A_DEX) < 6
                      || ACURR(A_CON) < 6 || ACURR(A_CHA) < 6
                      || ACURR(A_INT) < 6 || ACURR(A_WIS) < 6) ? "4-F"
                                                               : "1-A");
    } else {
        if (!Hallucination) {
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "a draft.");
        } else {
            /* "marching" is deliberately ambiguous; it might mean drills
                after entering military service or mean engaging in protests */
            STATIC_VAR const char *draft_reaction[] = {
                "enlisting", "marching", "protesting", "fleeing",
            };
            int dridx;

            /* Lawful: 0..1, Neutral: 1..2, Chaotic: 2..3 */
            dridx = rn1(2, 1 - sgn(u.ualign.type));
            if (u.ualign.record < STRIDENT)
                /* L: +(0..2), N: +(-1..1), C: +(-2..0); all: 0..3 */
                dridx += rn1(3, sgn(u.ualign.type) - 1);
            You_feel_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "like %s.", draft_reaction[dridx]);
        }
    }
}

STATIC_OVL void
create_tree_remnants(x, y, done_feelnewsym_ptr)
int x, y;
boolean* done_feelnewsym_ptr;
{
    if (!isok(x, y))
        return;

    struct rm* lev = &levl[x][y];
    short special_quality = lev->special_quality;
    int fruittype = tree_subtype_definitions[lev->subtyp].fruit_type;
    int glyph = layers_at(x, y).layer_gui_glyphs[LAYER_COVER_FEATURE];

    /* Wood */
    struct obj* otmp_wood = mksobj_found_at(PIECE_OF_WOOD, x, y, FALSE, FALSE);
    otmp_wood->quan = d(1, 3);
    otmp_wood->owt = weight(otmp_wood);

    /* Possibly some fruits */
    if (special_quality > 0 && fruittype > STRANGE_OBJECT)
    {
        struct obj* otmp = mksobj_found_at(fruittype, x, y, TRUE, FALSE); //rnd_treefruit_at(dpx, dpy);
        otmp->quan = special_quality;
        otmp->owt = weight(otmp);
        lev->special_quality = 0;
    }
    if (windowprocs.wincap2 & WC2_FADING_ANIMATIONS)
    {
        feel_newsym(x, y);
        if(done_feelnewsym_ptr)
            *done_feelnewsym_ptr = TRUE;
        play_special_effect_with_details_at(0, x, y, glyph, LAYER_GENERAL_EFFECT, -2, 20, 0, 0, TRUE);
        special_effect_wait_until_action(0);
        special_effect_wait_until_end(0);
        clear_found_this_turn_at(x, y);
    }
}

void
zap_try_destroy_tree(x, y)
int x, y;
{
    if (!isok(x, y) || !IS_TREE(levl[x][y].typ))
        return;

    if (!(levl[x][y].wall_info & W_NONDIGGABLE))
    {
        play_simple_location_sound((xchar)x, (xchar)y, LOCATION_SOUND_TYPE_BREAK);
        uncatch_tree_objects(x, y);
        create_current_floor_location((xchar)x, (xchar)y, 0, back_to_broken_glyph((xchar)x, (xchar)y), FALSE);
        unblock_vision_and_hearing_at_point(x, y); /* vision */
        create_tree_remnants(x, y, (boolean*)0);
        newsym(x, y);
        force_redraw_at(x, y);
        pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "tree splinters into pieces!");
    }
    else if (!Blind)
    {
        play_sfx_sound(SFX_WALL_GLOWS_THEN_FADES);
        pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "tree shudders but is unharmed.");
    }
}

/* digging via wand zap or spell cast */
void
zap_dig(origobj)
struct obj* origobj;
{
    struct rm *room;
    struct monst *mtmp;
    struct obj *otmp;
    struct trap *trap_with_u = (struct trap *) 0;
    int zx, zy, lzx, lzy, diridx = 8, digdepth, flow_x = -1, flow_y = -1;
    boolean shopdoor, shopwall, maze_dig, pitdig = FALSE, pitflow = FALSE;

    /*
     * Original effect (approximately):
     * from CORR: dig until we pierce a wall
     * from ROOM: pierce wall and dig until we reach
     * an ACCESSIBLE place.
     * Currently: dig for digdepth positions;
     * also down on request of Lennart Augustsson.
     * 3.6.0: from a PIT: dig one adjacent pit.
     */

    play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, RAY_SOUND_TYPE_CREATE, u.ux, u.uy);

    if (u.uswallow) {
        mtmp = u.ustuck;

        if (!is_whirly(mtmp->data)) {
            if (is_animal(mtmp->data))
                You("pierce %s %s wall!", s_suffix(mon_nam(mtmp)),
                    mbodypart(mtmp, STOMACH));
            mtmp->mhp = 1; /* almost dead */
            expels(mtmp, mtmp->data, !is_animal(mtmp->data));
        }
        return;
    } /* swallowed */

    if (u.dz)
    {
        if (!Is_airlevel(&u.uz) && !Is_waterlevel(&u.uz) && !Underwater) 
        {
            if (u.dz < 0 || On_stairs(u.ux, u.uy)) 
            {
                int dmg;
                if (On_stairs(u.ux, u.uy))
                {
                    play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, RAY_SOUND_TYPE_BOUNCE, u.ux, u.uy);
                    pline_The("beam bounces off the %s and hits the %s.",
                        (u.ux == xdnladder || u.ux == xupladder)
                        ? "ladder"
                        : "stairs",
                        ceiling(u.ux, u.uy));
                }
                You("loosen a rock from the %s.", ceiling(u.ux, u.uy));
                context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
                play_special_effect_at(SPECIAL_EFFECT_FALLING_ROCK_TRAP_FALLING_ROCK, 0, u.ux, u.uy, FALSE);
                special_effect_wait_until_action(0);
                play_sfx_sound(SFX_ROCK_HITS_YOU_ON_HEAD);
                pline("It falls on your %s!", body_part(HEAD));
                dmg = rnd((uarmh && is_hard_helmet(uarmh)) ? 2 : 6);
                losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "falling rock", KILLED_BY_AN);
                if (iflags.using_gui_sounds)
                {
                    delay_output_milliseconds(150);
                    play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
                }
                otmp = mksobj_at(ROCK, u.ux, u.uy, FALSE, FALSE);
                if (otmp) {
                    (void) xname(otmp); /* set dknown, maybe bknown */
                    stackobj(otmp);
                }
                newsym(u.ux, u.uy);
                flush_screen(1);
                special_effect_wait_until_end(0);
                context.global_newsym_flags = 0UL;
            }
            else 
            {
                watch_dig((struct monst *) 0, u.ux, u.uy, TRUE);
                (void) dighole(FALSE, TRUE, (coord *) 0);
            }
        }
        return;
    } /* up or down */

    /* normal case: digging across the level */
    int framenum = 1;
    int anim_intervals = 0;
    context.zap_aggregate_intervals_to_wait_until_end = 0UL;
    context.zap_aggregate_intervals_to_wait_until_action = 0UL;
    int i;
    for (i = 0; i < MAX_PLAYED_ZAP_ANIMATIONS; i++)
    {
        toggle_animation_timer(ANIMATION_TIMER_ZAP, i, 0, 0, 0, 0, 0UL);
        context.zap_animation_counter_on[i] = FALSE;
        //context.zap_animation_counter[i] = 0L;
        context.zap_animation_x[i] = 0;
        context.zap_animation_y[i] = 0;
    }
    enum animation_types anim = zap_type_definitions[ZAP_SPECIAL_DIGGING].animation;
    boolean playing_anim = (iflags.using_gui_tiles && anim > 0 && animations[anim].play_type == ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY);
    if (playing_anim)
    {
        framenum = animations[anim].number_of_frames + (animations[anim].main_tile_use_style != ANIMATION_MAIN_TILE_IGNORE ? 1 : 0);
        anim_intervals = framenum * animations[anim].intervals_between_frames;
    }
    int zap_tile_count = 0;
    boolean first_tile_found = FALSE;
    //int prev_anim_counter_idx = -1;

    shopdoor = shopwall = FALSE;
    maze_dig = level.flags.is_maze_lev && !Is_earthlevel(&u.uz);
    lzx = u.ux;
    lzy = u.uy;
    zx = u.ux + u.dx;
    zy = u.uy + u.dy;
    if (u.utrap && u.utraptype == TT_PIT
        && (trap_with_u = t_at(u.ux, u.uy))) 
    {
        pitdig = TRUE;
        for (diridx = 0; diridx < 8; diridx++) 
        {
            if (xdir[diridx] == u.dx && ydir[diridx] == u.dy)
                break;
            /* diridx is valid if < 8 */
        }
    }

    start_ambient_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, zx, zy);
    tmp_at(DISP_BEAM_DIG, zapdir_to_glyph(u.dx, u.dy, ZAP_SPECIAL_DIGGING)); // cmap_to_glyph(S_digbeam) + dir_to_beam_index(u.dx, u.dy));

    digdepth = (origobj && objects[origobj->otyp].oc_spell_range > 0) ? (int)objects[origobj->otyp].oc_spell_range : rn1(18, 8);
    context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_ZAP_GLYPH;
    while (--digdepth >= 0)
    {
        if (!isok(zx, zy))
            break;
        room = &levl[zx][zy];

        if (playing_anim)
        {
            int used_count = min(zap_tile_count, MAX_PLAYED_ZAP_ANIMATIONS);
            int idx = 0;
            boolean use_old = FALSE;
            for (idx = 0; idx < used_count; idx++)
            {
                if (context.zap_animation_counter_on[idx] == TRUE && context.zap_animation_x[idx] == zx && context.zap_animation_y[idx] == zy)
                {
                    use_old = TRUE;
                    break;
                }
            }

            context.zap_animation_x[idx] = zx;
            context.zap_animation_y[idx] = zy;
            //context.zap_animation_counter[idx] = 0L;
            context.zap_animation_counter_on[idx] = TRUE;
            context.zap_aggregate_intervals_to_wait_until_action = 0UL;
            context.zap_aggregate_intervals_to_wait_until_end = anim_intervals;
            toggle_animation_timer(ANIMATION_TIMER_ZAP, idx, TRUE, zx, zy, 0, 0UL);

            if (animations[anim].action_execution_frame > 0)
            {
                int64_t intervals_to_execution = (int64_t)(animations[anim].action_execution_frame * animations[anim].intervals_between_frames);
#if 0
                if (prev_anim_counter_idx > -1 && context.zap_animation_counter_on[prev_anim_counter_idx])
                {
                    int64_t diff = context.zap_animation_counter[prev_anim_counter_idx] - intervals_to_execution /* - 1*/;
                    if (abs((int)diff) <= 3) /* Extra check that something else is not going on */
                    {
                        context.zap_animation_counter[prev_anim_counter_idx] -= diff;
                    }
                }
#endif

                context.zap_aggregate_intervals_to_wait_until_action = (uint64_t)intervals_to_execution;
            }

            //prev_anim_counter_idx = idx;

            if (!use_old)
                zap_tile_count++;
        }
        remove_glyph_buffer_layer_flags(zx, zy, LFLAGS_ZAP_TRAILING_EDGE, 0UL);
        if (!first_tile_found)
        {
            add_glyph_buffer_layer_flags(zx, zy, LFLAGS_ZAP_TRAILING_EDGE, 0UL);
            first_tile_found = TRUE;
        }
        remove_glyph_buffer_layer_flags(lzx, lzy, LFLAGS_ZAP_LEADING_EDGE, 0UL);
        add_glyph_buffer_layer_flags(zx, zy, LFLAGS_ZAP_LEADING_EDGE, 0UL);
        tmp_at(zx, zy);
        force_redraw_at(zx, zy);
        update_ambient_ray_sound_to_location(OBJECT_RAY_SOUNDSET_DIGBEAM, zx, zy);
        
        /* wait a little bit */
        if (iflags.using_gui_tiles)
        {
            if (playing_anim)
            {
                delay_output_intervals((int)context.zap_aggregate_intervals_to_wait_until_action);
                context.zap_aggregate_intervals_to_wait_until_action = 0UL;
            }
            else
                adjusted_delay_output();
        }
        else
        {
            adjusted_delay_output(); /* wait a little */
        }

        if (pitdig) 
        { /* we are already in a pit if this is true */
            coord cc;
            struct trap *adjpit = t_at(zx, zy);
            if ((diridx < 8) && !conjoined_pits(adjpit, trap_with_u, FALSE)) 
            {
                digdepth = 0; /* limited to the adjacent location only */
                if (!(adjpit && is_pit(adjpit->ttyp))) 
                {
                    char buf[BUFSZ];
                    cc.x = zx;
                    cc.y = zy;
                    if (!adj_pit_checks(&cc, buf)) 
                    {
                        if (buf[0])
                            pline1(buf);
                    } else {
                        /* this can also result in a pool at zx,zy */
                        play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
                        dighole(TRUE, TRUE, &cc);
                        adjpit = t_at(zx, zy);
                        force_redraw_at(zx, zy);
                    }
                }
                if (adjpit
                    && is_pit(adjpit->ttyp)) 
                {
                    int adjidx = (diridx + 4) % 8;
                    trap_with_u->conjoined |= (1 << diridx);
                    adjpit->conjoined |= (1 << adjidx);
                    flow_x = zx;
                    flow_y = zy;
                    pitflow = TRUE;
                }
                if (is_pool(zx, zy) || is_lava(zx, zy)) 
                {
                    flow_x = zx - u.dx;
                    flow_y = zy - u.dy;
                    pitflow = TRUE;
                }
                break;
            }
        } 
        else if ((closed_door(zx, zy) || room->typ == SDOOR)) 
        {
            if (is_door_diggable_at_ptr(room))
            {
                if (*in_rooms(zx, zy, SHOPBASE))
                {
                    add_damage(zx, zy, SHOP_DOOR_COST);
                    shopdoor = TRUE;
                }
                play_simple_location_sound(zx, zy, LOCATION_SOUND_TYPE_BREAK);
                int otherflags = room->doormask & ~D_MASK;
                if (room->typ == SDOOR)
                    transform_location_type(zx, zy, DOOR, 0);  /* doormask set below */
                else if (cansee(zx, zy))
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is razed!", get_door_name_at_ptr(room));
                watch_dig((struct monst*)0, zx, zy, TRUE);
                room->doormask = (D_NODOOR | otherflags);
                room->subtyp = 0;
                unblock_vision_and_hearing_at_point(zx, zy); /* vision */
                newsym(zx, zy);
                force_redraw_at(zx, zy);
            }
            else
            {
                play_sfx_sound(SFX_WALL_GLOWS_THEN_FADES);
                pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s glows then fades.", get_door_name_at_ptr(room));
                break;
            }
            digdepth -= 2;
            if (maze_dig)
                break;
        } 
        else if (maze_dig) 
        {
            int ltype = 0;
            unsigned short lflags = 0;

            if (IS_WALL(room->typ))
            {
                play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
                if (!(room->wall_info & W_NONDIGGABLE))
                {
                    if (*in_rooms(zx, zy, SHOPBASE))
                    {
                        add_damage(zx, zy, SHOP_WALL_COST);
                        shopwall = TRUE;
                    }

                    ltype = CORR;
                    play_simple_location_sound(zx, zy, LOCATION_SOUND_TYPE_BREAK);
                    create_simple_initial_location(zx, zy, ltype, lflags, back_to_broken_glyph(zx, zy), 0, FALSE);
                    unblock_vision_and_hearing_at_point(zx, zy); /* vision */
                    newsym(zx, zy);
                    force_redraw_at(zx, zy);
                }
                else if (!Blind)
                {
                    play_sfx_sound(SFX_WALL_GLOWS_THEN_FADES);
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "wall glows then fades.");
                }
                break;
            } 
            else if (IS_TREE(room->typ)) 
            { /* check trees before stone */
                play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
                zap_try_destroy_tree(zx, zy);
                break;
            }
            else if (room->typ == STONE || room->typ == SCORR)
            {
                play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
                if (!(room->wall_info & W_NONDIGGABLE))
                {
                    play_simple_location_sound(zx, zy, LOCATION_SOUND_TYPE_BREAK);
                    struct rm* lev = &levl[zx][zy];
                    create_basic_floor_location(zx, zy, lev->floortyp ? lev->floortyp : CORR, lev->floortyp ? lev->floorsubtyp : get_initial_location_subtype(CORR), 0, FALSE);
                    unblock_vision_and_hearing_at_point(zx, zy); /* vision */
                    newsym(zx, zy);
                    force_redraw_at(zx, zy);
                }
                else if (!Blind)
                {
                    play_sfx_sound(SFX_WALL_GLOWS_THEN_FADES);
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "rock glows then fades.");
                }
                break;
            }
        } 
        else if (IS_ROCK(room->typ)) 
        {
            play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
            if (!may_dig(zx, zy))
                break;

            int ltype = 0;
            int lsubtype = 0;
            int lvartype = 0;
            uchar lflags = 0;

            if (IS_WALL_OR_SDOOR(room->typ))
            {
                if (*in_rooms(zx, zy, SHOPBASE)) 
                {
                    add_damage(zx, zy, SHOP_WALL_COST);
                    shopwall = TRUE;
                }
                watch_dig((struct monst *) 0, zx, zy, TRUE);
                if (level.flags.is_cavernous_lev && !in_town(zx, zy)) 
                {
                    ltype = CORR;
                    lsubtype = get_initial_location_subtype(CORR);
                } 
                else 
                {
                    ltype = DOOR, lflags = D_NODOOR;
                }
                digdepth -= 2;
            } 
            else if (IS_TREE(room->typ)) 
            {
                ltype = room->floortyp;
                lsubtype = room->floorsubtyp;
                lvartype = room->floorvartyp;
                digdepth -= 2;
                uncatch_tree_objects(zx, zy);
            } 
            else 
            { /* IS_ROCK but not IS_WALL or SDOOR */
                ltype = CORR;
                lsubtype = get_initial_location_subtype(CORR);
                lvartype = get_initial_location_vartype(ltype, lsubtype);
                digdepth--;
            }
            play_simple_location_sound(zx, zy, LOCATION_SOUND_TYPE_BREAK);
            create_simple_location(zx, zy, ltype, lsubtype, lvartype, lflags, back_to_broken_glyph(zx, zy), !IS_FLOOR(ltype) ? room->floortyp : 0, !IS_FLOOR(ltype) ? room->floorsubtyp : 0, !IS_FLOOR(ltype) ? room->floorvartyp : 0, FALSE);
            unblock_vision_and_hearing_at_point(zx, zy); /* vision */
            newsym(zx, zy);
            force_redraw_at(zx, zy);
        }
        flush_screen(1);
        lzx = zx;
        lzy = zy;
        zx += u.dx;
        zy += u.dy;
    }                    /* while */
    if (context.zap_aggregate_intervals_to_wait_until_end > 0)
    {
        delay_output_intervals((int)context.zap_aggregate_intervals_to_wait_until_end);
        context.zap_aggregate_intervals_to_wait_until_end = 0UL;
    }
    context.global_newsym_flags = 0;
    tmp_at(DISP_END, 0); /* closing call */
    stop_ambient_ray_sound(OBJECT_RAY_SOUNDSET_DIGBEAM);
    play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_DIGBEAM, RAY_SOUND_TYPE_DESTROY, zx, zy);

    if (pitflow && isok(flow_x, flow_y)) {
        struct trap *ttmp = t_at(flow_x, flow_y);

        if (ttmp && is_pit(ttmp->ttyp)) {
            schar filltyp = fillholetyp(ttmp->tx, ttmp->ty, TRUE);

            if (!IS_FLOOR(filltyp))
                pit_flow(ttmp, filltyp);
        }
    }

    if (shopdoor || shopwall)
        pay_for_damage(shopdoor ? "destroy" : "dig into", FALSE);
    return;
}

/* evaporation via wand zap or spell cast */
void
zap_evaporation(origobj)
struct obj* origobj;
{
    struct monst* mtmp;
    int zx, zy, lzx, lzy, digdepth;

    play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_CREATE, u.ux, u.uy);

    if (u.uswallow) 
    {
        mtmp = u.ustuck;

        if (mtmp && !is_whirly(mtmp->data)) 
        {
            if (is_animal(mtmp->data))
                You("dehydrate %s %s wall!", s_suffix(mon_nam(mtmp)),
                    mbodypart(mtmp, STOMACH));
            mtmp->mhp = 1; /* almost dead */
            expels(mtmp, mtmp->data, !is_animal(mtmp->data));
        }
        return;
    } /* swallowed */

    if (u.dz) 
    {
        zx = u.ux;
        zy = u.uy;
        struct rm* lev = &levl[zx][zy];
        boolean see_it = cansee(zx, zy);

        if (Is_waterlevel(&u.uz) || u.dz < 0)
        {
            pline("Nothing happens.");
        }
        else
        {
            if (is_pool(zx, zy)) 
            {
                play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
                const char* msgtxt = "You hear hissing gas.";
                play_simple_location_sound(zx, zy, LOCATION_SOUND_TYPE_BREAK);

                if (lev->typ == DRAWBRIDGE_UP && (lev->drawbridgemask & DB_UNDER) == DB_MOAT)
                {
                    lev->drawbridgemask &= ~DB_UNDER; /* clear water */
                    lev->drawbridgemask |= DB_GROUND;
                    if (see_it)
                        msgtxt = "The water evaporates.";
                }
                else if (lev->typ == POOL || lev->typ == MOAT)  /* Wand of evaportaion is powerful enough to dry out a moat */
                { // Leave no pits, evaporation gives a walkable route
                    create_basic_floor_location(zx, zy, levl[zx][zy].floortyp ? levl[zx][zy].floortyp : GROUND, levl[zx][zy].floortyp ? levl[zx][zy].floorsubtyp : get_initial_location_subtype(GROUND), 0, FALSE);
                    if (see_it)
                        msgtxt = "The water evaporates.";
                }
                else
                {
                    if (see_it)
                        msgtxt = "Some water evaporates.";
                }
                Norep("%s", msgtxt);
                newsym(zx, zy);
            }
            else if (IS_FOUNTAIN(lev->typ))
            {
                play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
                play_simple_location_sound(zx, zy, LOCATION_SOUND_TYPE_DRY_UP);
                /* replace the fountain with ordinary floor */
                create_current_floor_location(zx, zy, 0, back_to_broken_glyph(zx, zy), TRUE);
                newsym(zx, zy);
                if (see_it)
                    pline_The1("fountain dries up!");
                /* The location is seen if the hero/monster is invisible
                   or felt if the hero is blind. */
            }
        }
        return;
    } /* up or down */

    int framenum = 1;
    int anim_intervals = 0;
    context.zap_aggregate_intervals_to_wait_until_end = 0UL;
    context.zap_aggregate_intervals_to_wait_until_action = 0UL;
    int i;
    for (i = 0; i < MAX_PLAYED_ZAP_ANIMATIONS; i++)
    {
        toggle_animation_timer(ANIMATION_TIMER_ZAP, i, FALSE, 0, 0, 0, 0UL);
        context.zap_animation_counter_on[i] = FALSE;
        //context.zap_animation_counter[i] = 0L;
        context.zap_animation_x[i] = 0;
        context.zap_animation_y[i] = 0;
    }
    enum animation_types anim = zap_type_definitions[ZAP_SPECIAL_DIGGING].animation;
    boolean playing_anim = (iflags.using_gui_tiles && anim > 0 && animations[anim].play_type == ANIMATION_PLAY_TYPE_PLAYED_SEPARATELY);
    if (playing_anim)
    {
        framenum = animations[anim].number_of_frames + (animations[anim].main_tile_use_style != ANIMATION_MAIN_TILE_IGNORE ? 1 : 0);
        anim_intervals = framenum * animations[anim].intervals_between_frames;
    }
    int zap_tile_count = 0;
    boolean first_tile_found = FALSE;
    //int prev_anim_counter_idx = -1;

    /* normal case: evaporating across the level */
    lzx = u.ux;
    lzy = u.uy;
    zx = u.ux + u.dx;
    zy = u.uy + u.dy;

    start_ambient_ray_sound_at_location(ZAP_SPECIAL_EVAPORATION, zx, zy);
    tmp_at(DISP_BEAM_DIG, zapdir_to_glyph(u.dx, u.dy, ZAP_SPECIAL_EVAPORATION)); // cmap_to_glyph(S_digbeam) + dir_to_beam_index(u.dx, u.dy));

    digdepth = (origobj && objects[origobj->otyp].oc_spell_range > 0) ? (int)objects[origobj->otyp].oc_spell_range : rn1(18, 8);
    context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_ZAP_GLYPH;
    while (--digdepth >= 0)
    {
        if (!isok(zx, zy))
            break;

        if (playing_anim)
        {
            int used_count = min(zap_tile_count, MAX_PLAYED_ZAP_ANIMATIONS);
            int idx = 0;
            boolean use_old = FALSE;
            for (idx = 0; idx < used_count; idx++)
            {
                if (context.zap_animation_counter_on[idx] == TRUE && context.zap_animation_x[idx] == zx && context.zap_animation_y[idx] == zy)
                {
                    use_old = TRUE;
                    break;
                }
            }

            context.zap_animation_x[idx] = zx;
            context.zap_animation_y[idx] = zy;
            //context.zap_animation_counter[idx] = 0L;
            context.zap_animation_counter_on[idx] = TRUE;
            context.zap_aggregate_intervals_to_wait_until_action = 0UL;
            context.zap_aggregate_intervals_to_wait_until_end = anim_intervals;
            toggle_animation_timer(ANIMATION_TIMER_ZAP, idx, TRUE, zx, zy, 0, 0UL);

            if (animations[anim].action_execution_frame > 0)
            {
                int64_t intervals_to_execution = (int64_t)(animations[anim].action_execution_frame * animations[anim].intervals_between_frames);
#if 0
                if (prev_anim_counter_idx > -1 && context.zap_animation_counter_on[prev_anim_counter_idx])
                {
                    int64_t diff = context.zap_animation_counter[prev_anim_counter_idx] - intervals_to_execution /* - 1*/;
                    if (abs((int)diff) <= 3) /* Extra check that something else is not going on */
                    {
                        context.zap_animation_counter[prev_anim_counter_idx] -= diff;
                    }
                }
#endif
                context.zap_aggregate_intervals_to_wait_until_action = (uint64_t)intervals_to_execution;
            }

            //prev_anim_counter_idx = idx;

            if (!use_old)
                zap_tile_count++;
        }
        remove_glyph_buffer_layer_flags(zx, zy, LFLAGS_ZAP_TRAILING_EDGE, 0UL);
        if (!first_tile_found)
        {
            add_glyph_buffer_layer_flags(zx, zy, LFLAGS_ZAP_TRAILING_EDGE, 0UL);
            first_tile_found = TRUE;
        }
        remove_glyph_buffer_layer_flags(lzx, lzy, LFLAGS_ZAP_LEADING_EDGE, 0UL);
        add_glyph_buffer_layer_flags(zx, zy, LFLAGS_ZAP_LEADING_EDGE, 0UL);
        tmp_at(zx, zy);

        /* wait a little bit */
        if (iflags.using_gui_tiles)
        {
            if (playing_anim)
            {
                delay_output_intervals((int)context.zap_aggregate_intervals_to_wait_until_action);
                context.zap_aggregate_intervals_to_wait_until_action = 0UL;
            }
            else
                adjusted_delay_output();
        }
        else
        {
            adjusted_delay_output(); /* wait a little */
        }

        struct rm* lev = &levl[zx][zy];
        boolean see_it = cansee(zx, zy);

        /* Kill water elementals */
        mtmp = m_at(zx, zy);
        if (mtmp)
        {
            if (is_watery(mtmp->data))
            {
                play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_HIT_MONSTER, zx, zy);
                mtmp->mhp = 0;
                if (DEADMONSTER(mtmp))
                    killed(mtmp);
            }
        }

        /* Destroy potions */
        struct obj* otmp, *otmp2;
        for (otmp = level.objects[zx][zy]; otmp; otmp = otmp2) {
            otmp2 = otmp->nexthere; //Save nextthere to otmp2, because otmp may get deleted below
            if(otmp->oclass == POTION_CLASS)
            {
                play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_HIT_OBJECT, zx, zy);
                play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_BREAK);
                if(see_it)
                    pline("%s!", Tobjnam(otmp, "evaporate"));

                delobj(otmp);
            }
        }

        /* Destroy water and fountains */
        if (is_ice(zx, zy)) 
        {
            //melt_ice(zx, zy, (char*)0);
        }
        else if (is_pool(zx, zy)) 
        {
            const char* msgtxt = "You hear hissing gas.";
            play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
            play_simple_location_sound(zx, zy, LOCATION_SOUND_TYPE_BREAK);

            if (lev->typ == DRAWBRIDGE_UP && (lev->drawbridgemask & DB_UNDER) == DB_MOAT)
            {
                digdepth -= 1;
                lev->drawbridgemask &= ~DB_UNDER; /* clear water */
                lev->drawbridgemask |= DB_GROUND;
                if (see_it)
                    msgtxt = "The water evaporates.";
            }
            else if (lev->typ == POOL || lev->typ == MOAT) /* Wand of evaportaion is powerful enough to dry out a moat */
            { // Leave no pits, evaporation gives a walkable route
                digdepth -= 1;
                create_basic_floor_location(zx, zy, levl[zx][zy].floortyp ? levl[zx][zy].floortyp : GROUND, levl[zx][zy].floortyp ? levl[zx][zy].floorsubtyp : get_initial_location_subtype(GROUND), 0, FALSE);
                if (see_it)
                    msgtxt = "The water evaporates.";
            }
            else
            {
                digdepth -= 1;
                if (see_it)
                    msgtxt = "Some water evaporates.";
            }
            Norep("%s", msgtxt);
            newsym(zx, zy);
        }
        else if (IS_FOUNTAIN(lev->typ))
        {
            /* replace the fountain with ordinary floor */
            play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
            play_simple_location_sound(zx, zy, LOCATION_SOUND_TYPE_DRY_UP);
            create_current_floor_location(zx, zy, 0, back_to_broken_glyph(zx, zy), TRUE);
            newsym(zx, zy);
            if (see_it)
                pline_The1("fountain dries up!");
            /* The location is seen if the hero/monster is invisible
               or felt if the hero is blind. */
            digdepth -= 1;
        }

        if (IS_ROCK(lev->typ))
        {
            play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_HIT_LOCATION, zx, zy);
            if (!IS_TREE(lev->typ) || lev->typ == SDOOR){
                break;
            }
        }
        flush_screen(1);
        lzx = zx;
        lzy = zy;
        zx += u.dx;
        zy += u.dy;
    }                    /* while */
    if (context.zap_aggregate_intervals_to_wait_until_end > 0)
    {
        delay_output_intervals((int)context.zap_aggregate_intervals_to_wait_until_end);
        context.zap_aggregate_intervals_to_wait_until_end = 0UL;
    }
    context.global_newsym_flags = 0;
    tmp_at(DISP_END, 0); /* closing call */
    stop_ambient_ray_sound(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM);
    play_immediate_ray_sound_at_location(OBJECT_RAY_SOUNDSET_EVAPORATION_BEAM, RAY_SOUND_TYPE_DESTROY, zx, zy);

    return;
}

/*
 * This checks what is on the surface above the
 * location where an adjacent pit might be created if
 * you're zapping a wand of digging laterally while
 * down in the pit.
 */
STATIC_OVL int
adj_pit_checks(cc, msg)
coord *cc;
char *msg;
{
    int ltyp;
    struct rm *room;
    const char *foundation_msg =
                 "The foundation is too hard to dig through from this angle.";

    if (!cc)
        return FALSE;
    if (!isok(cc->x, cc->y))
        return FALSE;
    *msg = '\0';
    room = &levl[cc->x][cc->y];
    ltyp = room->typ, room->flags = 0;

    if (is_pool(cc->x, cc->y) || is_lava(cc->x, cc->y)) {
        /* this is handled by the caller after we return FALSE */
        return FALSE;
    } else if (closed_door(cc->x, cc->y) || room->typ == SDOOR) {
        /* We reject this here because dighole() isn't
           prepared to deal with this case */
        Strcpy(msg, foundation_msg);
        return FALSE;
    } else if (IS_WALL(ltyp)) {
        /* if (room->wall_info & W_NONDIGGABLE) */
        Strcpy(msg, foundation_msg);
        return FALSE;
    } else if (IS_TREE(ltyp)) { /* check trees before stone */
        /* if (room->wall_info & W_NONDIGGABLE) */
        Strcpy(msg, "The tree's roots glow then fade.");
        return FALSE;
    } else if (ltyp == STONE || ltyp == SCORR) {
        if (room->wall_info & W_NONDIGGABLE) {
            Strcpy(msg, "The rock glows then fades.");
            return FALSE;
        }
    } else if (ltyp == IRONBARS) {
        /* "set of iron bars" */
        Strcpy(msg, "The bars go much deeper than your pit.");
#if 0
    } else if (is_lava(cc->x, cc->y)) {
    } else if (is_ice(cc->x, cc->y)) {
    } else if (is_pool(cc->x, cc->y)) {
    } else if (IS_GRAVE(ltyp)) {
#endif
    } else if (IS_SINK(ltyp)) {
        Strcpy(msg, "A tangled mass of plumbing remains below the sink.");
        return FALSE;
    } else if ((cc->x == xupladder && cc->y == yupladder) /* ladder up */
               || (cc->x == xdnladder && cc->y == ydnladder)) { /* " down */
        Strcpy(msg, "The ladder is unaffected.");
        return FALSE;
    } else {
        const char *supporting = (const char *) 0;

        if (IS_FOUNTAIN(ltyp))
            supporting = "fountain";
        else if (IS_THRONE(ltyp))
            supporting = "throne";
        else if (IS_ALTAR(ltyp))
            supporting = "altar";
        else if (IS_ANVIL(ltyp))
            supporting = "anvil";
        else if ((cc->x == xupstair && cc->y == yupstair)
                 || (cc->x == sstairs.sx && cc->y == sstairs.sy
                     && sstairs.up))
            /* "staircase up" */
            supporting = "stairs";
        else if ((cc->x == xdnstair && cc->y == ydnstair)
                 || (cc->x == sstairs.sx && cc->y == sstairs.sy
                     && !sstairs.up))
            /* "staircase down" */
            supporting = "stairs";
        else if (ltyp == DRAWBRIDGE_DOWN   /* "lowered drawbridge" */
                 || ltyp == DBWALL)        /* "raised drawbridge" */
            supporting = "drawbridge";

        if (supporting) {
            Sprintf(msg, "The %s%ssupporting structures remain intact.",
                    supporting ? s_suffix(supporting) : "",
                    supporting ? " " : "");
            return FALSE;
        }
    }
    return TRUE;
}

/*
 * Ensure that all conjoined pits fill up.
 */
STATIC_OVL void
pit_flow(trap, filltyp)
struct trap *trap;
schar filltyp;
{
    if (trap && !IS_FLOOR(filltyp) && is_pit(trap->ttyp)) {
        struct trap t;
        int idx;

        t = *trap;
        levl[t.tx][t.ty].typ = filltyp, levl[t.tx][t.ty].flags = 0;
        liquid_flow(t.tx, t.ty, filltyp, trap,
                    (t.tx == u.ux && t.ty == u.uy)
                        ? "Suddenly %s flows in from the adjacent pit!"
                        : (char *) 0);
        for (idx = 0; idx < 8; ++idx) {
            if (t.conjoined & (1 << idx)) {
                int x, y;
                struct trap *t2;

                x = t.tx + xdir[idx];
                y = t.ty + ydir[idx];
                t2 = t_at(x, y);
#if 0
                /* cannot do this back-check; liquid_flow()
                 * called deltrap() which cleaned up the
                 * conjoined fields on both pits.
                 */
                if (t2 && (t2->conjoined & (1 << ((idx + 4) % 8))))
#endif
                /* recursion */
                pit_flow(t2, filltyp);
            }
        }
    }
}

struct obj *
buried_ball(cc)
coord *cc;
{
    int odist, bdist = COLNO;
    struct obj *otmp, *ball = 0;

    /* FIXME:
     *  This is just approximate; if multiple buried balls meet the
     *  criterium (within 2 steps of tethered hero's present location)
     *  it will find an arbitrary one rather than the one which used
     *  to be uball.  Once 3.6.{0,1} save file compatibility is broken,
     *  we should add context.buriedball_oid and then we can find the
     *  actual former uball, which might be extra heavy or christened
     *  or not the one buried directly underneath the target spot.
     *
     *  [Why does this search within a radius of two when trapmove()
     *  only lets hero get one step away from the buried ball?]
     */

    if (u.utrap && u.utraptype == TT_BURIEDBALL)
        for (otmp = level.buriedobjlist; otmp; otmp = otmp->nobj) {
            if (otmp->otyp != HEAVY_IRON_BALL)
                continue;
            /* if found at the target spot, we're done */
            if (otmp->ox == cc->x && otmp->oy == cc->y)
                return otmp;
            /* find nearest within allowable vicinity: +/-2
             *  4 5 8
             *  1 2 5
             *  0 1 4
             */
            odist = dist2(otmp->ox, otmp->oy, cc->x, cc->y);
            if (odist <= 8 && (!ball || odist < bdist)) {
                /* remember nearest buried ball but keep checking others */
                ball = otmp;
                bdist = odist;
            }
        }
    if (ball) {
        /* found, but not at < cc->x, cc->y > */
        cc->x = ball->ox;
        cc->y = ball->oy;
    }
    return ball;
}

void
buried_ball_to_punishment()
{
    coord cc;
    struct obj *ball;

    cc.x = u.ux;
    cc.y = u.uy;
    ball = buried_ball(&cc);
    if (ball) {
        Strcpy(debug_buf_2, "buried_ball_to_punishment");
        obj_extract_self(ball);
#if 0
        /* rusting buried metallic objects is not implemented yet */
        if (ball->timed)
            (void) stop_timer(RUST_METAL, obj_to_any(ball));
#endif
        punish(ball); /* use ball as flag for unearthed buried ball */
        reset_utrap(FALSE);
        del_engr_at(cc.x, cc.y);
        newsym(cc.x, cc.y);
    }
}

void
buried_ball_to_freedom()
{
    coord cc;
    struct obj *ball;

    cc.x = u.ux;
    cc.y = u.uy;
    ball = buried_ball(&cc);
    if (ball) {
        Strcpy(debug_buf_2, "buried_ball_to_freedom");
        obj_extract_self(ball);
#if 0
        /* rusting buried metallic objects is not implemented yet */
        if (ball->timed)
            (void) stop_timer(RUST_METAL, obj_to_any(ball));
#endif
        place_object(ball, cc.x, cc.y);
        stackobj(ball);
        reset_utrap(TRUE);
        del_engr_at(cc.x, cc.y);
        newsym(cc.x, cc.y);
    }
}

/* move objects from fobj/nexthere lists to buriedobjlist, keeping position
   information */
struct obj *
bury_an_obj(otmp, dealloced)
struct obj *otmp;
boolean *dealloced;
{
    struct obj *otmp2;
    boolean under_ice;
    xchar x = 0, y = 0;
    get_obj_location(otmp, &x, &y, CONTAINED_TOO | BURIED_TOO);

    debugpline1("bury_an_obj: %s", xname(otmp));
    if (dealloced)
        *dealloced = FALSE;
    if (otmp == uball) {
        unpunish();
        u.utrap = rn1(50, 20);
        u.utraptype = TT_BURIEDBALL;
        pline_The("iron ball gets buried!");
    }
    /* after unpunish(), or might get deallocated chain */

    otmp2 = otmp->nexthere;
    if (otmp->otyp == LEASH && otmp->leashmon != 0)
        o_unleash(otmp);

    if (otmp->lamplit && otmp->otyp != POT_OIL)
        end_burn(otmp, TRUE);

    Strcpy(debug_buf_2, "bury_an_obj");
    obj_extract_self(otmp);

    if (is_obj_unburiable(otmp)) //(otmp == uchain || obj_resists(otmp, 0, 0))
    {
        /*
         * obj_resists(,0,0) prevents Rider corpses from being buried.
         * It also prevents The Amulet and invocation tools from being
         * buried.  Since they can't be confined to bags and statues,
         * it makes sense that they can't be buried either, even though
         * the real reason there (direct accessibility when carried) is
         * completely different.
         */
        if(isok(x,y))
            place_object(otmp, x, y); /* Makes sure it gets placed back */
        else
            place_object(otmp, u.ux, u.uy); /* Fallback */

        return otmp2;
    }

    under_ice = is_ice(otmp->ox, otmp->oy);
    if ((is_rock(otmp) || is_ore(otmp)) && !under_ice) {
        /* merges into burying material */
        if (dealloced)
            *dealloced = TRUE;
        obfree(otmp, (struct obj *) 0);
        return otmp2;
    }
    /*
     * Start a rot on organic material.  Not corpses -- they
     * are already handled.
     */
    if (otmp->otyp == CORPSE) {
        ; /* should cancel timer if under_ice */
    } else if ((under_ice ? otmp->oclass == POTION_CLASS : is_rottable(otmp))
               && !obj_resists(otmp, 5, 95)) {
        (void) start_timer((under_ice ? 0L : 250L) + (int64_t) rnd(250),
                           TIMER_OBJECT, ROT_ORGANIC, obj_to_any(otmp));
#if 0
    /* rusting of buried metal not yet implemented */
    } else if (is_rustprone(otmp)) {
        (void) start_timer((int64_t) rnd((otmp->otyp == HEAVY_IRON_BALL)
                                         ? 1500
                                         : 250),
                           TIMER_OBJECT, RUST_METAL, obj_to_any(otmp));
#endif
    }
    add_to_buried(otmp);
    return  otmp2;
}

void
bury_objs(x, y)
int x, y;
{
    struct obj *otmp, *otmp2;
    struct monst *shkp;
    int64_t loss = 0L;
    boolean costly;

    costly = ((shkp = shop_keeper(*in_rooms(x, y, SHOPBASE)))
              && costly_spot(x, y));

    if (level.objects[x][y] != (struct obj *) 0) {
        debugpline2("bury_objs: at <%d,%d>", x, y);
    }
    for (otmp = level.objects[x][y]; otmp; otmp = otmp2) {
        if (costly) {
            loss += stolen_value(otmp, x, y, is_peaceful(shkp), TRUE);
            if (otmp->oclass != COIN_CLASS)
                otmp->no_charge = 1;
        }
        otmp2 = bury_an_obj(otmp, (boolean *) 0);
    }

    /* don't expect any engravings here, but just in case */
    del_engr_at(x, y);
    newsym(x, y);

    if (costly && loss) {
        You("owe %s %ld %s for burying merchandise.", mon_nam(shkp), loss,
            currency(loss));
    }
}

/* move objects from buriedobjlist to fobj/nexthere lists */
int
unearth_objs(mtmp, x, y, verbose, buriedsearchableonly)
struct monst *mtmp;
int x, y;
boolean verbose, buriedsearchableonly;
{
    struct obj *otmp, *otmp2, *bball;
    coord cc;
    int cnt = 0;
    boolean is_you = (mtmp == &youmonst);

    debugpline2("unearth_objs: at <%d,%d>", x, y);
    cc.x = x;
    cc.y = y;
    bball = buried_ball(&cc);
    for (otmp = level.buriedobjlist; otmp; otmp = otmp2) 
    {
        otmp2 = otmp->nobj;
        if (otmp->ox == x && otmp->oy == y && (!buriedsearchableonly || (buriedsearchableonly && is_otyp_buried_searchable(otmp->otyp))))
        {
            cnt++;
            if (verbose)
            {
                play_sfx_sound_at_location(SFX_UNEARTHED_OBJECT_FOUND, x, y);
                pline_ex(ATR_NONE, is_you || (mtmp && is_tame(mtmp)) ? CLR_MSG_SUCCESS : CLR_MSG_ATTENTION, "%s %s %s%s.", is_you ? "You" : Monnam(mtmp), is_you ? "find" : "finds", doname(otmp), buriedsearchableonly ? " buried close to the surface of the ground" : "");
            }
            if (bball && otmp == bball
                && u.utrap && u.utraptype == TT_BURIEDBALL) 
            {
                buried_ball_to_punishment();
            }
            else
            {
                Strcpy(debug_buf_2, "unearth_objs");
                obj_extract_self(otmp);
                if (otmp->timed)
                    (void) stop_timer(ROT_ORGANIC, obj_to_any(otmp));

                obj_set_found(otmp);
                place_object(otmp, x, y);
                stackobj(otmp);
            }
        }
    }
    if(!buriedsearchableonly)
        del_engr_at(x, y);
    newsym(x, y);
    return cnt;
}

/*
 * The organic material has rotted away while buried.  As an expansion,
 * we could add add partial damage.  A damage count is kept in the object
 * and every time we are called we increment the count and reschedule another
 * timeout.  Eventually the object rots away.
 *
 * This is used by buried objects other than corpses.  When a container rots
 * away, any contents become newly buried objects.
 */
/* ARGSUSED */
void
rot_organic(arg, timeout)
anything *arg;
int64_t timeout UNUSED;
{
    struct obj *obj = arg->a_obj;

    while (Has_contents(obj)) 
    {
        struct obj* cobj_to_bury = obj->cobj;

        /* We don't need to place contained object on the floor
           first, but we do need to update its map coordinates. */
        cobj_to_bury->ox = obj->ox, cobj_to_bury->oy = obj->oy;
        /* Everything which can be held in a container can also be
           buried, so bury_an_obj's use of obj_extract_self insures
           that Has_contents(obj) will eventually become false. */
        (void) bury_an_obj(cobj_to_bury, (boolean *) 0);
        if (obj->cobj == cobj_to_bury)
            break; /* Something's wrong, avoid infine loop */
    }
    Strcpy(debug_buf_2, "rot_organic");
    obj_extract_self(obj);
    obfree(obj, (struct obj *) 0);
}

/*
 * Called when a corpse has rotted completely away.
 */
void
rot_corpse(arg, timeout)
anything *arg;
int64_t timeout;
{
    xchar x = 0, y = 0;
    struct obj *obj = arg->a_obj;
    boolean on_floor = obj->where == OBJ_FLOOR,
            in_invent = obj->where == OBJ_INVENT;

    if (on_floor) {
        x = obj->ox;
        y = obj->oy;
    } else if (in_invent) {
        if (flags.verbose) {
            char *cname = corpse_xname(obj, (const char *) 0, CXN_NO_PFX);

            Your("%s%s %s away%c", obj == uwep || obj == uarms ? "wielded " : "", cname,
                 otense(obj, "rot"), obj == uwep || obj == uarms ? '!' : '.');
        }
        if (obj == uwep) {
            uwepgone(); /* now bare handed */
            stop_occupation();
        }
        else if (obj == uarms) {
            uwep2gone();
            stop_occupation();
        }
        else if (obj == uswapwep)
        {
            uswapwepgone();
            stop_occupation();
        }
        else if (obj == uswapwep2)
        {
            uswapwep2gone();
            stop_occupation();
        }
        else if (obj == uquiver) {
            uqwepgone();
            stop_occupation();
        }
    } else if (obj->where == OBJ_MINVENT && obj->owornmask) {
        if (obj == MON_WEP(obj->ocarry))
            setmnotwielded(obj->ocarry, obj);
    } else if (obj->where == OBJ_MIGRATING) {
        /* clear destination flag so that obfree()'s check for
           freeing a worn object doesn't get a false hit */
        obj->owornmask = 0L;
    }
    rot_organic(arg, timeout);
    if (on_floor) {
        struct monst *mtmp = m_at(x, y);

        /* a hiding monster may be exposed */
        if (mtmp && !OBJ_AT(x, y) && mtmp->mundetected
            && hides_under(mtmp->data)) {
            mtmp->mundetected = 0;
        } else if (x == u.ux && y == u.uy && u.uundetected && hides_under(youmonst.data))
            (void) hideunder(&youmonst);
        newsym(x, y);
    } else if (in_invent)
        update_inventory();
}

#if 0
void
bury_monst(mtmp)
struct monst *mtmp;
{
    debugpline1("bury_monst: %s", mon_nam(mtmp));
    if (canseemon(mtmp)) {
        if (is_flying(mtmp) || is_levitating(mtmp)) {
            pline_The("%s opens up, but %s is not swallowed!",
                      surface(mtmp->mx, mtmp->my), mon_nam(mtmp));
            return;
        } else
            pline_The("%s opens up and swallows %s!",
                      surface(mtmp->mx, mtmp->my), mon_nam(mtmp));
    }

    mtmp->mburied = TRUE;
    wakeup(mtmp, FALSE);       /* at least give it a chance :-) */
    newsym(mtmp->mx, mtmp->my);
}

void
bury_you()
{
    debugpline0("bury_you");
    if (!Levitation && !Flying) {
        if (u.uswallow)
            You_feel("a sensation like falling into a trap!");
        else
            pline_The("%s opens beneath you and you fall in!",
                      surface(u.ux, u.uy));

        u.uburied = TRUE;
        if (!Strangled && !Breathless)
            Strangled = 6;
        under_ground(1);
    }
}

void
unearth_you()
{
    debugpline0("unearth_you");
    u.uburied = FALSE;
    under_ground(0);
    if (!uamul || uamul->otyp != AMULET_OF_STRANGULATION)
        Strangled = 0;
    vision_recalc(0);
}

void
escape_tomb()
{
    debugpline0("escape_tomb");
    if ((Teleportation || has_innate_teleportation(youmonst.data))
        && (Teleport_control || rn2(3) < Luck+2)) {
        You("attempt a teleport spell.");
        (void) dotele(FALSE);        /* calls unearth_you() */
    } else if (u.uburied) { /* still buried after 'port attempt */
        boolean good;

        if (amorphous(youmonst.data) || Passes_walls
            || is_incorporeal(youmonst.data)
            || (unsolid(youmonst.data)
                && !is_watery(youmonst.data))
            || (tunnels(youmonst.data) && !needspick(youmonst.data))) {
            You("%s up through the %s.",
                (tunnels(youmonst.data) && !needspick(youmonst.data))
                   ? "try to tunnel"
                   : (amorphous(youmonst.data))
                      ? "ooze"
                      : "phase",
                surface(u.ux, u.uy));

            good = (tunnels(youmonst.data) && !needspick(youmonst.data))
                      ? dighole(TRUE, FALSE, (coord *)0) : TRUE;
            if (good)
                unearth_you();
        }
    }
}

void
bury_obj(otmp)
struct obj *otmp;
{
    debugpline0("bury_obj");
    if (cansee(otmp->ox, otmp->oy))
        pline_The("objects on the %s tumble into a hole!",
                  surface(otmp->ox, otmp->oy));

    bury_objs(otmp->ox, otmp->oy);
}
#endif /*0*/

#ifdef DEBUG
/* bury everything at your loc and around */
int
wiz_debug_cmd_bury()
{
    int x, y;

    for (x = u.ux - 1; x <= u.ux + 1; x++)
        for (y = u.uy - 1; y <= u.uy + 1; y++)
            if (isok(x, y))
                bury_objs(x, y);
    return 0;
}

#endif /* DEBUG */


/* the C('d') command - Dig ground with hands or your weapon */
int
dodig()
{
    if (uwep && (uwep->otyp == PICK_AXE || uwep->otyp == DWARVISH_MATTOCK || uwep->otyp == SPADE_OF_COLOSSAL_EXCAVATION))
    {
        u.dx = u.dy = 0;
        u.dz = 1;
        int res = use_pick_axe2(uwep);
        return res;
    }
    else if (u.twoweap && uarms && (uarms->otyp == PICK_AXE || uarms->otyp == DWARVISH_MATTOCK || uarms->otyp == SPADE_OF_COLOSSAL_EXCAVATION))
    {
        u.dx = u.dy = 0;
        u.dz = 1;
        int res = use_pick_axe2(uarms);
        return res;
    }


    char digbuf[BUFSIZ];
    enum object_soundset_types oss = 0;
    if (uwep)
    {
        Strcpy(digbuf, yname(uwep));
        oss = objects[uwep->otyp].oc_soundset;

    }
    else if (u.twoweap && uarms)
    {
        Strcpy(digbuf, yname(uarms));
        oss = objects[uarms->otyp].oc_soundset;
    }
    else
    {
        Sprintf(digbuf, "your %s", makeplural(body_part(HAND)));
        if (!Upolyd || u.umonnum < LOW_PM)
        {
            enum player_soundset_types pss = get_player_soundset();
            oss = player_soundsets[pss].attack_soundsets[PLAYER_ATTACK_SOUNDSET_BAREHANDED];
        }
        else
        {
            oss = monster_soundsets[flags.female ? mons[u.umonnum].female_soundset : mons[u.umonnum].soundset].attack_soundsets[0];
        }
    }

    struct rm* lev = &levl[u.ux][u.uy];

    if (!(lev->typ == GRASS || lev->typ == GROUND || IS_GRAVE(lev->typ)))
    {
        play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "It is too hard to dig here with %s.", digbuf);
        return 0;
    }

    play_occupation_immediate_sound(oss, OCCUPATION_DIGGING_GROUND, OCCUPATION_SOUND_TYPE_START);

    if (IS_GRAVE(lev->typ))
    {
        You("dig the grave with %s.", digbuf);
        coord cc;
        cc.x = u.ux;
        cc.y = u.uy;
        dig_up_grave(&cc);
        return 1;
    }


    /* Normal digging */
    You("dig the ground with %s.", digbuf);

    int itemsfound = unearth_objs(&youmonst, u.ux, u.uy, TRUE, FALSE);
    if (itemsfound)
    {
        (void)pickup(1, FALSE);
    }
    else
    {
        pline("However, you do not find anything.");
    }
    return 1;
}


void
uncatch_tree_objects(x, y)
int x, y;
{
    if (!isok(x, y))
        return;

    /* Things are not caught in leaves anymore */
    struct obj* otmp_caught;
    for (otmp_caught = level.objects[x][y]; otmp_caught; otmp_caught = otmp_caught->nexthere)
    {
        otmp_caught->speflags &= ~SPEFLAGS_CAUGHT_IN_LEAVES; /* Not caught anymore */
        otmp_caught->speflags |= SPEFLAGS_FOUND_THIS_TURN; /* But now found this turn */
    }
}

void
reset_dig(VOID_ARGS)
{
    did_dig_msg = 0;
}
/*dig.c*/
