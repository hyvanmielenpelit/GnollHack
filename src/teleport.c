/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    teleport.c    $NHDT-Date: 1553885439 2019/03/29 18:50:39 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.86 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_DCL boolean FDECL(tele_jump_ok, (int, int, int, int, BOOLEAN_P));
STATIC_DCL boolean FDECL(teleok, (int, int, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL void NDECL(vault_tele);
STATIC_DCL boolean FDECL(rloc_pos_ok, (int, int, struct monst *));
STATIC_DCL void FDECL(mvault_tele, (struct monst *));

/* non-null when teleporting via having read this scroll */
STATIC_VAR struct obj *telescroll = 0;


/*
 * Is (x,y) a good position of mtmp?  If mtmp is NULL, then is (x,y) good
 * for an object?
 *
 * This function will only look at mtmp->mdat, so makemon, mplayer, etc can
 * call it to generate new monster positions with fake monster structures.
 */
boolean
goodpos(x, y, mtmp, gpflags)
int x, y;
struct monst *mtmp;
unsigned long gpflags;
{
    struct permonst *mdat = (struct permonst *) 0;
    boolean ignorewater = ((gpflags & MM_IGNOREWATER) != 0);
    boolean ignoreyou = ((gpflags & GOODPOS_IGNOREYOU) != 0);

    if (!isok(x, y))
        return FALSE;

    /* in many cases, we're trying to create a new monster, which
     * can't go on top of the player or any existing monster.
     * however, occasionally we are relocating engravings or objects,
     * which could be co-located and thus get restricted a bit too much.
     * oh well.
     */
    if (!ignoreyou && mtmp != &youmonst && x == u.ux && y == u.uy
        && (!u.usteed || mtmp != u.usteed))
        return FALSE;

    if (mtmp) 
    {
        struct monst *mtmp2 = m_at(x, y);

        /* Be careful with long worms.  A monster may be placed back in
         * its own location.  Normally, if m_at() returns the same monster
         * that we're trying to place, the monster is being placed in its
         * own location.  However, that is not correct for worm segments,
         * because all the segments of the worm return the same m_at().
         * Actually we overdo the check a little bit--a worm can't be placed
         * in its own location, period.  If we just checked for mtmp->mx
         * != x || mtmp->my != y, we'd miss the case where we're called
         * to place the worm segment and the worm's head is at x,y.
         */
        if (mtmp2 && (mtmp2 != mtmp || mtmp->wormno))
            return FALSE;

        mdat = mtmp->data;
        if (is_pool(x, y) && (Is_waterlevel(&u.uz) || !(mtmp == &youmonst ? Levitation || Flying : is_flying(mtmp) || is_levitating(mtmp))) && !ignorewater)
        {
            if (mtmp == &youmonst)
                return (Walks_on_water || Swimming || Amphibious);
            else
                return ((has_swimming(mtmp) || amphibious(mdat) || is_clinger(mdat)) && !is_mimic(mdat));
        } 
        else if (In_modron_level(&u.uz) && IS_AIR(levl[x][y].typ))
        {
            if (mtmp == &youmonst)
                return (Levitation || Flying || is_incorporeal(youmonst.data));
            else
                return (is_flying(mtmp) || is_levitating(mtmp) || is_incorporeal(mdat));
        }
        else if (mdat->mlet == S_EEL && rn2(13) && !ignorewater)
        {
            return FALSE;
        } 
        else if (is_lava(x, y)) 
        {
            if (mtmp == &youmonst)
                return (Levitation || Flying
                        || (Fire_immunity && Walks_on_water && uarmf
                            && uarmf->oerodeproof)
                        || (Upolyd && likes_lava(youmonst.data)));
            else
                return (is_flying(mtmp) || is_levitating(mtmp)
                        || likes_lava(mdat));
        }
        if (passes_walls(mdat) && may_passwall(x, y))
            return TRUE;
        if (amorphous(mdat) && closed_door(x, y))
            return TRUE;
    }

    if (!accessible(x, y)) 
    {
        if (!(is_pool(x, y) && ignorewater))
            return FALSE;
    }

    if (sobj_at(BOULDER, x, y) && (!mdat || !throws_rocks(mdat)))
        return FALSE;
    return TRUE;
}

/*
 * "entity next to"
 *
 * Attempt to find a good place for the given monster type in the closest
 * position to (xx,yy).  Do so in successive square rings around (xx,yy).
 * If there is more than one valid position in the ring, choose one randomly.
 * Return TRUE and the position chosen when successful, FALSE otherwise.
 */
boolean
enexto(cc, xx, yy, mdat)
coord *cc;
register xchar xx, yy;
struct permonst *mdat;
{
    return enexto_core(cc, xx, yy, mdat, 0);
}

boolean
enexto_core(cc, xx, yy, mdat, entflags)
coord *cc;
register xchar xx, yy;
struct permonst *mdat;
unsigned long entflags;
{
#define MAX_GOOD 15
    coord good[MAX_GOOD], *good_ptr;
    int x, y, range, i;
    int xmin, xmax, ymin, ymax;
    struct monst fakemon; /* dummy monster */

    if (!mdat) {
        debugpline0("enexto() called with null mdat");
        /* default to player's original monster type */
        mdat = &mons[u.umonster];
    }
    fakemon = zeromonst;
    set_mon_data(&fakemon, mdat, 0); /* set up for goodpos */

    good_ptr = good;
    range = 1;
    /*
     * Walk around the border of the square with center (xx,yy) and
     * radius range.  Stop when we find at least one valid position.
     */
    do {
        xmin = max(1, xx - range);
        xmax = min(COLNO - 1, xx + range);
        ymin = max(0, yy - range);
        ymax = min(ROWNO - 1, yy + range);

        for (x = xmin; x <= xmax; x++)
            if (goodpos(x, ymin, &fakemon, entflags)) {
                good_ptr->x = x;
                good_ptr->y = ymin;
                /* beware of accessing beyond segment boundaries.. */
                if (good_ptr++ == &good[MAX_GOOD - 1])
                    goto full;
            }
        for (x = xmin; x <= xmax; x++)
            if (goodpos(x, ymax, &fakemon, entflags)) {
                good_ptr->x = x;
                good_ptr->y = ymax;
                /* beware of accessing beyond segment boundaries.. */
                if (good_ptr++ == &good[MAX_GOOD - 1])
                    goto full;
            }
        for (y = ymin + 1; y < ymax; y++)
            if (goodpos(xmin, y, &fakemon, entflags)) {
                good_ptr->x = xmin;
                good_ptr->y = y;
                /* beware of accessing beyond segment boundaries.. */
                if (good_ptr++ == &good[MAX_GOOD - 1])
                    goto full;
            }
        for (y = ymin + 1; y < ymax; y++)
            if (goodpos(xmax, y, &fakemon, entflags)) {
                good_ptr->x = xmax;
                good_ptr->y = y;
                /* beware of accessing beyond segment boundaries.. */
                if (good_ptr++ == &good[MAX_GOOD - 1])
                    goto full;
            }
        range++;

        /* return if we've grown too big (nothing is valid) */
        if (range > ROWNO && range > COLNO)
            return FALSE;
    } while (good_ptr == good);

full:
    i = rn2((int) (good_ptr - good));
    cc->x = good[i].x;
    cc->y = good[i].y;
    return TRUE;
}

/*
 * Check for restricted areas present in some special levels.  (This might
 * need to be augmented to allow deliberate passage in wizard mode, but
 * only for explicitly chosen destinations.)
 */
STATIC_OVL boolean
tele_jump_ok(x1, y1, x2, y2, isyou)
int x1, y1, x2, y2;
boolean isyou;
{
    if (!isok(x2, y2))
        return FALSE;
    if (dndest.nlx > 0) {
        /* if inside a restricted region, can't teleport outside */
        if (within_bounded_area(x1, y1, dndest.nlx, dndest.nly, dndest.nhx,
                                dndest.nhy)
            && !within_bounded_area(x2, y2, dndest.nlx, dndest.nly,
                                    dndest.nhx, dndest.nhy))
            return (isyou && wizard && (yn_query("Your teleport destination is restricted. Continue?") == 'y')) ? TRUE : FALSE;
        /* and if outside, can't teleport inside */
        if (!within_bounded_area(x1, y1, dndest.nlx, dndest.nly, dndest.nhx,
                                 dndest.nhy)
            && within_bounded_area(x2, y2, dndest.nlx, dndest.nly, dndest.nhx,
                                   dndest.nhy))
            return (isyou && wizard && (yn_query("Your teleport destination is restricted. Continue?") == 'y')) ? TRUE : FALSE;
    }
    if (updest.nlx > 0) { /* ditto */
        if (within_bounded_area(x1, y1, updest.nlx, updest.nly, updest.nhx,
                                updest.nhy)
            && !within_bounded_area(x2, y2, updest.nlx, updest.nly,
                                    updest.nhx, updest.nhy))
            return (isyou && wizard && (yn_query("Your teleport destination is restricted. Continue?") == 'y')) ? TRUE : FALSE;
        if (!within_bounded_area(x1, y1, updest.nlx, updest.nly, updest.nhx,
                                 updest.nhy)
            && within_bounded_area(x2, y2, updest.nlx, updest.nly, updest.nhx,
                                   updest.nhy))
            return (isyou && wizard && (yn_query("Your teleport destination is restricted. Continue?") == 'y')) ? TRUE : FALSE;
    }
    if (noteledest.lx > 0) { /* ditto */
        if (within_bounded_area(x2, y2, noteledest.lx, noteledest.ly, noteledest.hx,
            noteledest.hy)
            && !within_bounded_area(x2, y2, noteledest.nlx, noteledest.nly,
                noteledest.nhx, noteledest.nhy))
            return (isyou && wizard && (yn_query("Your teleport destination is restricted. Continue?") == 'y')) ? TRUE : FALSE;
    }
    return TRUE;
}

STATIC_OVL boolean
teleok(x, y, trapok, verbose)
register int x, y;
boolean trapok, verbose;
{
    if (!trapok) 
    {
        /* allow teleportation onto vibrating square, it's not a real trap */
        struct trap* trap = t_at(x, y);

        if (trap && !(trap_type_definitions[trap->ttyp].tdflags & TRAPDEF_FLAGS_TELEOK))
            return FALSE;
    }
    if (!goodpos(x, y, &youmonst, 0))
        return FALSE;
    if (!tele_jump_ok(u.ux, u.uy, x, y, verbose))
        return FALSE;
    if (!in_out_region(x, y))
        return FALSE;
    return TRUE;
}

void
teleds(nux, nuy, allow_drag, keep_effect_glyphs)
register int nux, nuy;
boolean allow_drag, keep_effect_glyphs;
{
    boolean ball_active, ball_still_in_range;
    struct monst *vault_guard = vault_occupied(u.urooms) ? findgd() : 0;
    int x = nux, y = nuy;
    struct layer_info layers = layers_at(x, y);

    if (u.utraptype == TT_BURIEDBALL) {
        /* unearth it */
        buried_ball_to_punishment();
    }
    ball_active = (Punished && uball->where != OBJ_FREE);
    ball_still_in_range = FALSE;

    /* If they have to move the ball, then drag if allow_drag is true;
     * otherwise they are teleporting, so unplacebc().
     * If they don't have to move the ball, then always "drag" whether or
     * not allow_drag is true, because we are calling that function, not
     * to drag, but to move the chain.  *However* there are some dumb
     * special cases:
     *    0                          0
     *   _X  move east       ----->  X_
     *    @                           @
     * These are permissible if teleporting, but not if dragging.  As a
     * result, drag_ball() needs to know about allow_drag and might end
     * up dragging the ball anyway.  Also, drag_ball() might find that
     * dragging the ball is completely impossible (ball in range but there's
     * rock in the way), in which case it teleports the ball on its own.
     */
    if (ball_active) {
        if (!carried(uball) && distmin(nux, nuy, uball->ox, uball->oy) <= 2)
            ball_still_in_range = TRUE; /* don't have to move the ball */
        else {
            /* have to move the ball */
            if (!allow_drag || distmin(u.ux, u.uy, nux, nuy) > 1) {
                /* we should not have dist > 1 and allow_drag at the same
                 * time, but just in case, we must then revert to teleport.
                 */
                allow_drag = FALSE;
                unplacebc();
            }
        }
    }
    reset_utrap(FALSE);
    u.ustuck = 0;
    u.ux0 = u.ux;
    u.uy0 = u.uy;

    if (!hideunder(&youmonst) && is_mimic(youmonst.data)) {
        /* mimics stop being unnoticed */
        youmonst.m_ap_type = M_AP_NOTHING;
    }

    if (u.uswallow) {
        u.uswldtim = u.uswallow = 0;
        if (Punished && !ball_active) {
            /* ensure ball placement, like unstuck */
            ball_active = TRUE;
            allow_drag = FALSE;
        }
        docrt();
    }
    if (ball_active) {
        if (ball_still_in_range || allow_drag) {
            int bc_control;
            xchar ballx, bally, chainx, chainy;
            boolean cause_delay;

            if (drag_ball(nux, nuy, &bc_control, &ballx, &bally, &chainx,
                          &chainy, &cause_delay, allow_drag))
                move_bc(0, bc_control, ballx, bally, chainx, chainy);
        }
    }
    /* must set u.ux, u.uy after drag_ball(), which may need to know
       the old position if allow_drag is true... */
    u_on_newpos(nux, nuy); /* set u.<x,y>, usteed-><mx,my>; cliparound() */
    fill_pit(u.ux0, u.uy0);
    if (ball_active) {
        if (!ball_still_in_range && !allow_drag)
            placebc();
    }
    initrack(); /* teleports mess up tracking monsters without this */
    update_player_regions();
    /*
     *  Make sure the hero disappears from the old location.  This will
     *  not happen if she is teleported within sight of her previous
     *  location.  Force a full vision recalculation because the hero
     *  is now in a new location.
     */
    newsym_with_flags(u.ux0, u.uy0, keep_effect_glyphs ? NEWSYM_FLAGS_KEEP_OLD_GENERAL_EFFECT_GLYPH | NEWSYM_FLAGS_KEEP_OLD_MONSTER_EFFECT_GLYPH | NEWSYM_FLAGS_KEEP_OLD_BACKGROUND_EFFECT_GLYPH : NEWSYM_FLAGS_NONE);
    see_monsters();
    vision_full_recalc = 1;
    nomul(0);
    vision_recalc(0); /* vision before effects */
    update_hearing_array(0);
    /* if terrain type changes, levitation or flying might become blocked
       or unblocked; might issue message, so do this after map+vision has
       been updated for new location instead of right after u_on_newpos() */
    if (levl[u.ux][u.uy].typ != levl[u.ux0][u.uy0].typ)
        switch_terrain();
    if (telescroll) {
        /* when teleporting by scroll, we need to handle discovery
           now before getting feedback about any objects at our
           destination since we might land on another such scroll */
        if (distu(u.ux0, u.uy0) >= 16 || !couldsee(u.ux0, u.uy0))
            learnscroll(telescroll);
        else
            telescroll = 0; /* no discovery by scrolltele()'s caller */
    }
    /* sequencing issue:  we want guard's alarm, if any, to occur before
       room entry message, if any, so need to check for vault exit prior
       to spoteffects; but spoteffects() sets up new value for u.urooms
       and vault code depends upon that value, so we need to fake it */
    if (vault_guard) {
        char save_urooms[5]; /* [sizeof u.urooms] */

        Strcpy(save_urooms, u.urooms);
        Strcpy(u.urooms, in_rooms(u.ux, u.uy, VAULT));
        /* if hero has left vault, make guard notice */
        if (!vault_occupied(u.urooms))
            uleftvault(vault_guard);
        Strcpy(u.urooms, save_urooms); /* reset prior to spoteffects() */
    }
    /* possible shop entry message comes after guard's shrill whistle */
    spoteffects(TRUE);
    invocation_message();

    reset_monster_origin_coordinates(&youmonst);

    if (keep_effect_glyphs)
    {
        show_gui_glyph_on_layer(x, y, layers.layer_glyphs[LAYER_GENERAL_EFFECT], layers.layer_gui_glyphs[LAYER_GENERAL_EFFECT], LAYER_GENERAL_EFFECT);
        show_gui_glyph_on_layer(x, y, layers.layer_glyphs[LAYER_MONSTER_EFFECT], layers.layer_gui_glyphs[LAYER_MONSTER_EFFECT], LAYER_MONSTER_EFFECT);
        show_gui_glyph_on_layer(x, y, layers.layer_glyphs[LAYER_BACKGROUND_EFFECT], layers.layer_gui_glyphs[LAYER_BACKGROUND_EFFECT], LAYER_BACKGROUND_EFFECT);
        force_redraw_at(x, y);
        flush_screen(1);
    }
}


void
teleds_with_effects(nux, nuy, allow_drag, keep_effect_glyphs)
register int nux, nuy;
boolean allow_drag, keep_effect_glyphs;
{
    play_special_effect_at(SPECIAL_EFFECT_TELEPORT_OUT, 0, u.ux, u.uy, TRUE);
    play_sfx_sound_at_location(SFX_TELEPORT, u.ux, u.uy);
    special_effect_wait_until_action(0);
    show_glyph_on_layer(u.ux, u.uy, NO_GLYPH, LAYER_MONSTER);
    force_redraw_at(u.ux, u.uy);
    flush_screen(1);
    play_special_effect_at(SPECIAL_EFFECT_TELEPORT_IN, 1, nux, nuy, TRUE);
    special_effect_wait_until_action(1);
    teleds(nux, nuy, allow_drag, keep_effect_glyphs);
    special_effect_wait_until_end(0);
    special_effect_wait_until_end(1);
}

boolean
safe_teleds(allow_drag, keep_effect_glyphs)
boolean allow_drag, keep_effect_glyphs;
{
    register int nux, nuy, tcnt = 0;

    do {
        nux = rnd(COLNO - 1);
        nuy = rn2(ROWNO);
    } while (!teleok(nux, nuy, (boolean) (tcnt > 200), FALSE) && ++tcnt <= 400);

    if (tcnt <= 400) {
        teleds(nux, nuy, allow_drag, keep_effect_glyphs);
        return TRUE;
    } else
        return FALSE;
}

boolean
safe_teleds_with_effects(allow_drag, keep_effect_glyphs)
boolean allow_drag, keep_effect_glyphs;
{
    play_special_effect_at(SPECIAL_EFFECT_TELEPORT_OUT, 0, u.ux, u.uy, FALSE);
    play_sfx_sound_at_location(SFX_TELEPORT, u.ux, u.uy);
    special_effect_wait_until_action(0);
    boolean res = safe_teleds(allow_drag, keep_effect_glyphs);
    play_special_effect_at(SPECIAL_EFFECT_TELEPORT_IN, 1, u.ux, u.uy, FALSE);
    special_effect_wait_until_action(1);
    special_effect_wait_until_end(0);
    special_effect_wait_until_end(1);

    return res;
}

STATIC_OVL void
vault_tele()
{
    register struct mkroom *croom = search_special(VAULT);
    coord c;

    if (croom && somexy(croom, &c) && teleok(c.x, c.y, FALSE, FALSE)) {
        teleds_with_effects(c.x, c.y, FALSE, TRUE);
        return;
    }
    tele();
}

boolean
teleport_pet(mtmp, force_it)
register struct monst *mtmp;
boolean force_it;
{
    register struct obj *otmp;

    if (mtmp == u.usteed)
        return FALSE;

    if (mtmp->mleashed) {
        otmp = get_mleash(mtmp);
        if (!otmp) {
            impossible("%s is leashed, without a leash.", Monnam(mtmp));
            goto release_it;
        }
        if (otmp->cursed && !force_it) {
            yelp(mtmp);
            return FALSE;
        } else {
            play_sfx_sound(SFX_LEASH_GOES_SLACK);
            Your("leash goes slack.");
        release_it:
            m_unleash(mtmp, FALSE);
            return TRUE;
        }
    }
    return TRUE;
}

/* teleport the hero via some method other than scroll of teleport */
void
tele()
{
    (void) scrolltele((struct obj *) 0, FALSE, FALSE);
}

/* teleport the hero as though he or she had teleport control */
void
controlled_teleportation()
{
    (void)scrolltele((struct obj*) 0, FALSE, TRUE);
}

/* teleport the hero via some method other than scroll of teleport */
void
wiztele()
{
    (void)scrolltele((struct obj*) 0, TRUE, TRUE);
}


/* teleport the hero; return true if scroll of teleportation should become
   discovered; teleds() will usually do the actual discovery, since the
   outcome sometimes depends upon destination and discovery needs to be
   performed before arrival, in case we land on another teleport scroll */
boolean
scrolltele(scroll, iswizcmd, iscontrolled)
struct obj *scroll;
boolean iswizcmd;
boolean iscontrolled;
{
    coord cc = { 0 };
    boolean result = FALSE; /* don't learn scroll */

    /* Disable teleportation in stronghold && Vlad's Tower */
    if (level.flags.noteleport) 
    {
        if (!wizard || yn_query("Teleportation is not allowed on this level. Override?") != 'y') 
        {
            play_sfx_sound(SFX_MYSTERIOUS_FORCE_PREVENTS);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "A mysterious force prevents you from teleporting!");
            return TRUE;
        }
    }

    /* don't show trap if "Sorry..." */
    if (!Blinded)
        make_blinded(0L, FALSE);

    if ((u.uhave.amulet || On_W_tower_level(&u.uz)) && !rn2(3)) 
    {
        play_sfx_sound(SFX_DISORIENTED_FOR_MOMENT);
        You_feel("disoriented for a moment.");
        if (!wizard || yn_query("Override?") != 'y')
            return FALSE;
    }

    if ((Teleport_control && !Stunned && !Confusion) || iscontrolled || (wizard && (iswizcmd /* || yn_query("Enforce teleport control?") == 'y'*/)))
    {
        if (unconscious() && !iscontrolled && !iswizcmd)
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "Being unconscious, you cannot control your teleport.");
        }
        else 
        {
            char whobuf[BUFSZ];

            Strcpy(whobuf, "you");
            if (u.usteed)
                Sprintf(eos(whobuf), " and %s", mon_nam(u.usteed));
            pline("Where do %s want to be teleported?", whobuf);
            cc.x = u.ux;
            cc.y = u.uy;
            if (getpos(&cc, TRUE, "the desired position", CURSOR_STYLE_TELEPORT_CURSOR) < 0)
                return TRUE; /* abort */
            /* possible extensions: introduce a small error if
               magic power is low; allow transfer to solid rock */
            if (teleok(cc.x, cc.y, FALSE, TRUE)) 
            {
                /* for scroll, discover it regardless of destination */
                if (scroll)
                    learnscroll(scroll);

                teleds_with_effects(cc.x, cc.y, FALSE, TRUE);
                return TRUE;
            }
            pline("Sorry...");
            result = TRUE;
        }
    } 
    else if (scroll && scroll->blessed)
    {
        /* (this used to be handled in seffects()) */
        if (yn_query("Do you wish to teleport?") == 'n')
            return TRUE;
        result = TRUE;
    }

    telescroll = scroll;
    (void) safe_teleds_with_effects(FALSE, TRUE);
    /* teleds() will leave telescroll intact iff random destination
       is far enough away for scroll discovery to be warranted */
    if (telescroll)
        result = TRUE;
    telescroll = 0; /* reset */
    return result;
}

boolean
modronportaltele(ttmp, mtmp)
struct trap* ttmp;
struct monst* mtmp;
{
    if (!ttmp || !mtmp)
        return FALSE;

    boolean isyou = (mtmp == &youmonst);
    const char* portal_color = 
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_SPHERICAL ? "Red" :
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_CYLINDRICAL ? "Green" :
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_TETRAHEDRAL ? "Violet" :
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_CUBICAL ? "Purple" : 
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_OCTAHEDRAL ? "Blue" :
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_DODECAHEDRAL ? "Light blue" :
        "Yellow";

    int portal_object = 
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_SPHERICAL ? MODRONITE_SPHERE :
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_CYLINDRICAL ? MODRONITE_MONOPOLE :
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_TETRAHEDRAL ? MODRONITE_TETRAHEDRON :
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_CUBICAL ? MODRONITE_CUBE : 
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_OCTAHEDRAL ? MODRONITE_OCTAHEDRON :
        ttmp->tsubtyp == MODRON_PORTAL_SUBTYPE_DODECAHEDRAL ? MODRONITE_DODECAHEDRON :
        MODRONITE_ICOSAHEDRON;

    struct obj* otmp = (struct obj*)0;

    if (isyou)
        otmp = carrying(portal_object);
    else
        otmp = m_carrying(mtmp, portal_object);

    if (!otmp && !(ttmp->tflags & TRAPFLAGS_ACTIVATED))
    {
        if (isyou)
        {
            play_sfx_sound(SFX_MODRON_GLIMMER_SURROUNDS);
            pline("%s glimmer surrounds you for a while but nothing else happens.", portal_color);
        }
        else
        {
            play_sfx_sound_at_location(SFX_MODRON_GLIMMER_SURROUNDS, mtmp->mx, mtmp->my);
            if (canseemon(mtmp))
                pline("%s glimmer flashes around %s.", portal_color, mon_nam(mtmp));
        }
        return FALSE;
    }

    /* Now do the teleport */
    if (ttmp->tflags & TRAPFLAGS_LEVEL_TELEPORTER)
    {
        d_level destination = { 0 };
        if (ttmp->tflags & TRAPFLAGS_LEVEL_TELEPORT_DOWN)
        {
            destination.dnum = u.uz.dnum;
            destination.dlevel = u.uz.dlevel + 1;
        }
        else if (ttmp->tflags & TRAPFLAGS_LEVEL_TELEPORT_UP)
        {
            destination.dnum = u.uz.dnum;
            destination.dlevel = u.uz.dlevel - 1;
        }
        else
        {
            destination = ttmp->dst;
        }

        ttmp->dst = destination;

        if (isyou || mtmp == u.usteed)
        {
            if (otmp && !(ttmp->tflags & TRAPFLAGS_ACTIVATED))
                pline("%s light envelops %s!", portal_color, yname(otmp));

            domagicportal(ttmp);

            if (otmp && !(ttmp->tflags & TRAPFLAGS_ACTIVATED))
            {
                pline("%s%s has vanished!", otmp->quan > 1 ? "One of " : "", otmp->quan > 1 ? yname(otmp) : Yname2(otmp));
                if (mtmp == u.usteed)
                    m_useup(mtmp, otmp);
                else
                    useup(otmp);
                ttmp->tflags |= TRAPFLAGS_ACTIVATED;
                ttmp->activation_count++;
            }
        }
        else
        {
            (void)mlevel_tele_trap(mtmp, ttmp, TRUE, canseemon(mtmp));

            if (otmp && !(ttmp->tflags & TRAPFLAGS_ACTIVATED))
            {
                m_useup(mtmp, otmp);
                ttmp->tflags |= TRAPFLAGS_ACTIVATED;
                ttmp->activation_count++;
            }
        }

        return TRUE;
    }
    else
    {
        int x = ttmp->launch.x, y = ttmp->launch.y;
        int tcnt = 0;
        if (!isok(x, y))
        {
            /* Arbitrary */
            x = u.ux;
            y = u.uy + 1;
        }
        int nux = x, nuy = y;
        if(isyou ? !teleok(x, y, FALSE, FALSE) : !rloc_pos_ok(x, y, mtmp))
        {
            do {
                if (tcnt < 50)
                {
                    nux = x - 1 + rn2(3);
                    nuy = y - 1 + rn2(3);
                }
                else
                {
                    nux = x - 2 + rn2(5);
                    nuy = y - 2 + rn2(5);
                }
            } while ((isyou ? !teleok(x, y, (tcnt >= 200), FALSE) : !rloc_pos_ok(x, y, mtmp)) && ++tcnt <= 400);
        }

        if (tcnt <= 400)
        {
            if (isyou || mtmp == u.usteed)
            {
                play_sfx_sound(SFX_MODRON_TELEPORT_SUCCESS);
                if(otmp && !(ttmp->tflags & TRAPFLAGS_ACTIVATED))
                    pline("%s light envelops %s!", portal_color, yname(otmp));
                pline("You feel your essence unsolidifying...");
                pline("You reemerge at a new location!");
                teleds_with_effects(nux, nuy, TRUE, TRUE);
                if (otmp && !(ttmp->tflags & TRAPFLAGS_ACTIVATED))
                {
                    pline("%s%s has vanished!", otmp->quan > 1 ? "One of " : "", otmp->quan > 1 ? yname(otmp) : Yname2(otmp));
                    if(mtmp == u.usteed)
                        m_useup(mtmp, otmp);
                    else
                        useup(otmp);
                    ttmp->tflags |= TRAPFLAGS_ACTIVATED;
                    ttmp->activation_count++;
                }
            }
            else
            {
                char colorbuf[BUFSZ];
                Strcpy(colorbuf, portal_color);
                *colorbuf = lowc(*colorbuf);

                play_sfx_sound_at_location(SFX_MODRON_TELEPORT_SUCCESS, mtmp->mx, mtmp->my);
                rloc_to(mtmp, nux, nuy);
                pline("%s disappears in a flash of %s light.", Monnam(mtmp), colorbuf);
                if (otmp && !(ttmp->tflags & TRAPFLAGS_ACTIVATED))
                {
                    m_useup(mtmp, otmp);
                    ttmp->tflags |= TRAPFLAGS_ACTIVATED;
                    ttmp->activation_count++;
                }
            }
            return TRUE;
        }
        else
        {
            if (isyou)
            {
                play_sfx_sound(SFX_MODRON_GLIMMER_SURROUNDS);
                pline("%s glimmer surrounds you for a while but nothing else happens.", portal_color);
            }
            else
            {
                play_sfx_sound_at_location(SFX_MODRON_GLIMMER_SURROUNDS, mtmp->mx, mtmp->my);
                if(canseemon(mtmp))
                    pline("%s glimmer flashes around %s.", portal_color, mon_nam(mtmp));
            }
            return FALSE;
        }
    }
    return FALSE;
}


/* ^T command; 'm ^T' == choose among several teleport modes */
int
dotelecmd()
{
    long save_HTele, save_ETele;
    int res, added, hidden;
    boolean ignore_restrictions = FALSE;
/* also defined in spell.c */
#define NOOP_SPELL  0
#define HIDE_SPELL  1
#define ADD_SPELL   2
#define UNHIDESPELL 3
#define REMOVESPELL 4

    /* normal mode; ignore 'm' prefix if it was given */
    if (!wizard)
        return dotele(FALSE);

    added = hidden = NOOP_SPELL;
    save_HTele = HTeleportation, save_ETele = ETeleportation;
    if (!iflags.menu_requested)
    {
        ignore_restrictions = TRUE;
    }
    else 
    {
        static const struct tporttypes {
            char menulet;
            const char *menudesc;
        } tports[] = {
            /*
             * Potential combinations:
             *  1) attempt ^T without intrinsic, not know spell;
             *  2) via intrinsic, not know spell, obey restrictions;
             *  3) via intrinsic, not know spell, ignore restrictions;
             *  4) via intrinsic, know spell, obey restrictions;
             *  5) via intrinsic, know spell, ignore restrictions;
             *  6) via spell, not have intrinsic, obey restrictions;
             *  7) via spell, not have intrinsic, ignore restrictions;
             *  8) force, obey other restrictions;
             *  9) force, ignore restrictions.
             * We only support the 1st (t), 2nd (n), 6th (s), and 9th (w).
             *
             * This ignores the fact that there is an experience level
             * (or poly-form) requirement which might make normal ^T fail.
             */
            { 'n', "normal ^T on demand; no spell, obey restrictions" },
            { 's', "via spellcast; no intrinsic teleport" },
            { 't', "try ^T without having it; no spell" },
            { 'w', "debug mode; ignore restrictions" }, /* trad wizard mode */
        };
        menu_item *picks = (menu_item *) 0;
        anything any;
        winid win;
        int i, tmode;

        win = create_nhwindow(NHW_MENU);
        start_menu_ex(win, GHMENU_STYLE_CHOOSE_COMMAND);
        any = zeroany;

        for (i = 0; i < SIZE(tports); ++i)
        {
            any.a_int = (int) tports[i].menulet;
            add_menu(win, NO_GLYPH, &any, (char) any.a_int, 0, ATR_NONE, NO_COLOR,
                     tports[i].menudesc,
                     (tports[i].menulet == 'w') ? MENU_SELECTED
                                                : MENU_UNSELECTED);
        }

        end_menu(win, "Which way do you want to teleport?");
        i = select_menu(win, PICK_ONE, &picks);
        destroy_nhwindow(win);
        if (i > 0)
        {
            tmode = picks[0].item.a_int;
            /* if we got 2, use the one which wasn't preselected */
            if (i > 1 && tmode == 'w')
                tmode = picks[1].item.a_int;
            free((genericptr_t) picks);
        } 
        else if (i == 0)
        {
            /* preselected one was explicitly chosen and got toggled off */
            tmode = 'w';
        }
        else 
        { /* ESC */
            return 0;
        }

        switch (tmode) 
        {
        case 'n':
            HTeleportation |= I_SPECIAL; /* confer intrinsic teleportation */
            hidden = tport_spell(HIDE_SPELL); /* hide teleport-away */
            break;
        case 's':
            HTeleportation = ETeleportation = 0L; /* suppress intrinsic */
            added = tport_spell(ADD_SPELL); /* add teleport-away */
            break;
        case 't':
            HTeleportation = ETeleportation = 0L; /* suppress intrinsic */
            hidden = tport_spell(HIDE_SPELL); /* hide teleport-away */
            break;
        case 'w':
            ignore_restrictions = TRUE;
            break;
        }
    }

    /* if dotele() can be fatal, final disclosure might lie about
       intrinsic teleportation; we should be able to live with that
       since the menu finagling is only applicable in wizard mode */
    res = dotele(ignore_restrictions);

    HTeleportation = save_HTele;
    ETeleportation = save_ETele;
    if (added != NOOP_SPELL || hidden != NOOP_SPELL)
        /* can't both be non-NOOP so addition will yield the non-NOOP one */
        (void) tport_spell(added + hidden - NOOP_SPELL);

    return res;
}

int
dotele(break_the_rules)
boolean break_the_rules; /* True: wizard mode ^T */
{
    struct trap *trap;
    const char *cantdoit;
    boolean trap_once = FALSE;

    trap = t_at(u.ux, u.uy);
    if (trap && (!trap->tseen || trap->ttyp != TELEP_TRAP))
        trap = 0;

    if (trap) 
    {
        trap_once = trap->once; /* trap may get deleted, save this */
        if (trap->once)
        {
            pline("This is a vault teleport, usable once only.");
            if (yn_query("Jump in?") == 'n')
            {
                trap = 0;
            } 
            else 
            {
                deltrap(trap);
                newsym(u.ux, u.uy);
            }
        }
        if (trap)
            You("%s onto the teleportation trap.",
                locomotion(youmonst.data, "jump"));
    }

    if (!trap)
    {
        boolean castit = FALSE;
        int sp_no = 0, energy = 0;
        boolean can_teleport = Teleportation;
        boolean can_teleport_at_will = can_teleport && Teleport_control;
        boolean is_able_to_teleport_at_will = can_teleport_at_will && !Stunned; /* Confusion just randomly teleports you around but stunned prevents from teleporting at will */

        if (!break_the_rules && !is_able_to_teleport_at_will)
        {
            /* Try to use teleport away spell.
               3.6.2: this used to require that you know the spellbook
               (probably just intended as an optimization to skip the
               lookup loop) but it is possible to know and cast a spell
               after forgetting its book due to amnesia. */
            for (sp_no = 0; sp_no < MAXSPELL; sp_no++)
                if (spl_book[sp_no].sp_id == SPE_TELEPORT_SELF)
                    break;

            boolean has_teleport_self_spell = sp_no < MAXSPELL;

            castit = (has_teleport_self_spell && !Confusion && !Stunned);
            if (castit && yn_query("Cast the 'teleport self' spell?") == 'y')
            {
                return spelleffects(sp_no, FALSE, &youmonst);
            }
            else if (!castit) 
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "%s.",
                    can_teleport_at_will ? (has_teleport_self_spell ? "are not in a condition to teleport at will or cast the 'teleport self' spell" : "are not in a condition to teleport at will or able to cast the 'teleport self' spell") 
                    : (has_teleport_self_spell ? "are not able to teleport at will or in a condition to cast the 'teleport self' spell" : "are not able to teleport at will or cast the 'teleport self' spell"));
                return 0;
            }
        }

        cantdoit = 0;
        energy = TELEPORT_AT_WILL_MANA_COST;

        if (break_the_rules) 
        {
            if (!castit)
                energy = 0;
            /* spell will cost more if carrying the Amulet, but the
               amount is rnd(2 * energy) so we can't know by how much;
               average is twice the normal cost, but could be triple;
               the extra energy is spent even if that results in not
               having enough to cast (which also uses the move) */
            else if (u.uen < energy)
                u.uen = energy;
        } 
        else if (u.uhunger <= 10)
        {
            play_sfx_sound(SFX_GENERAL_NOT_ENOUGH_STAMINA);
            cantdoit = "are too weak from hunger";
        } 
        else if (ACURR(A_STR) < 4) 
        {
            play_sfx_sound(SFX_GENERAL_NOT_ENOUGH_STAMINA);
            cantdoit = "lack the strength";
        }
        else if (energy > u.uen)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MANA);
            cantdoit = "lack the energy";
        }

        if (cantdoit) 
        {
            You_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s.", cantdoit,
                castit ? "for a teleport spell" : "to teleport");
            return 0;
        } 
        else if (check_capacity(
                       "Your concentration falters from carrying so much."))
        {
            play_sfx_sound(SFX_GENERAL_TOO_MUCH_ENCUMBRANCE);
            return 1; /* this failure in spelleffects() also uses the move */
        }

        u.uen -= energy;
        context.botl = 1;
    }

    if (next_to_u()) 
    {
        if (trap && trap_once)
            vault_tele();
        else if (break_the_rules)
            wiztele();
        else
            tele();
        (void) next_to_u();
    }
    else 
    {
        play_simple_player_sound(MONSTER_SOUND_TYPE_SHUDDER);
        You_ex1(ATR_NONE, CLR_MSG_ATTENTION, shudder_for_moment);
        return 0;
    }

    if (!trap && !break_the_rules)
        morehungry(100);

    return 1;
}

void
level_tele(teletype, controltype, target_level, tele_flags)
int teletype; /* 0 = scroll or other involuntary, 1 = wizard mode command, 2 = spell */
int controltype; /* 0 = uncontrolled, 1 = controlled, 2 = town portal / other using target_level */
d_level target_level;
uchar tele_flags; /* 1 = teleport inside Wizard's Tower  */
{
    register int newlev;
    d_level newlevel;
    const char *escape_by_flying = 0; /* when surviving dest of -N */
    char buf[BUFSZ];
    boolean force_dest = FALSE;
    int tcnt = 0;

    if (iflags.debug_fuzzer)
        goto random_levtport;

    if ((u.uhave.amulet || In_endgame(&u.uz) || (In_sokoban(&u.uz) && controltype != 2)) && !wizard)
    {
        play_sfx_sound(SFX_DISORIENTED_FOR_MOMENT);
        You_feel("very disoriented for a moment.");
        return;
    }

    if (controltype == 2)
    {
        newlevel = target_level;
        newlev = target_level.dlevel;
    }
    else if ((Teleport_control && !Stunned && !Confusion) || controltype == 1 || (wizard && (teletype == 1 /* || yn_query("Enforce teleport control?") == 'y'*/)))
    {
        char qbuf[BUFSZ];
        char phbuf[BUFSZ] = "";
        int trycnt = 0;

        Strcpy(qbuf, "To what level do you want to teleport?");
        do {
            if (iflags.menu_requested) {
                /* wizard mode 'm ^V' skips prompting on first pass
                   (note: level Tport via menu won't have any second pass) */
                iflags.menu_requested = FALSE;
                if (wizard)
                    goto levTport_menu;
            }
            if (++trycnt == 2) {
                if (wizard)
                    Strcpy(phbuf, "type a number, name, or ? for a menu");
                else
                    Strcpy(phbuf, "type a number or name");
            }
            *buf = '\0'; /* EDIT_GETLIN: if we're on second or later pass,
                            the previous input was invalid so don't use it
                            as getlin()'s preloaded default answer */
            getlin_ex(wizard ? GETLINE_WIZ_LEVELPORT : GETLINE_LEVELPORT, ATR_NONE, NO_COLOR, qbuf, buf, *phbuf ? phbuf : (char*)0, (char*)0, (char*)0);
            if (!strcmp(buf, "\033")) { /* cancelled */
                if (teletype != 1 && Confusion && rnl(5)) {
                    pline("Oops...");
                    goto random_levtport;
                }
                return;
            } else if (!strcmp(buf, "*")) {
                goto random_levtport;
            } else if (teletype != 1 && Confusion && rnl(5)) {
                pline("Oops...");
                goto random_levtport;
            }
            if (wizard && !strcmp(buf, "?")) {
                schar destlev;
                xchar destdnum;

            levTport_menu:
                destlev = 0;
                destdnum = 0;
                newlev = (int) print_dungeon(TRUE, &destlev, &destdnum);
                if (!newlev)
                    return;

                newlevel.dnum = destdnum;
                newlevel.dlevel = destlev;
                if (In_endgame(&newlevel) && !In_endgame(&u.uz)) {
                    struct obj *amu;

                    if (!u.uhave.amulet
                        && (amu = mksobj(AMULET_OF_YENDOR, TRUE, FALSE, FALSE))
                               != 0) {
                        /* ordinarily we'd use hold_another_object()
                           for something like this, but we don't want
                           fumbling or already full pack to interfere */
                        amu = addinv(amu);
                        prinv_ex("Endgame prerequisite:", amu, 0L, ATR_NONE, CLR_MSG_GOD, ATR_NONE, NO_COLOR, TRUE, FALSE);
                    }
                }
                force_dest = TRUE;
            } 
            else if ((newlev = lev_by_name(buf)) == 0)
                newlev = atoi(buf);
        
        }
        while (!newlev && !digit(buf[0])
                 && (buf[0] != '-' || !digit(buf[1])) && trycnt < 10);

        /* no dungeon escape via this route */
        if (newlev == 0) 
        {
            if (trycnt >= 10)
                goto random_levtport;
            if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Going Nowhere", "Go to Nowhere.  Are you sure?") != 'y')
                return;

            play_special_effect_at(SPECIAL_EFFECT_LEVEL_TELEPORT_OUT, 0, u.ux, u.uy, TRUE);
            play_sfx_sound(SFX_LEVEL_TELEPORT);
            special_effect_wait_until_action(0);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s in agony as your body begins to warp...",
                is_silent(youmonst.data) ? "writhe" : "scream");
            display_nhwindow(WIN_MESSAGE, FALSE);
            show_glyph_on_layer(u.ux, u.uy, NO_GLYPH, LAYER_MONSTER);
            force_redraw_at(u.ux, u.uy);
            flush_screen(1);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "cease to exist.");
            if (invent)
                Your("possessions land on the %s with a thud.",
                     surface(u.ux, u.uy));
            killer.format = NO_KILLER_PREFIX;
            Strcpy(killer.name, "committed suicide");
            done(DIED);
            pline("An energized cloud of dust begins to coalesce.");
            Your("body rematerializes%s.",
                 invent ? ", and you gather up all your possessions" : "");
            return;
        }

        /* if in Knox and the requested level > 0, stay put.
         * we let negative values requests fall into the "heaven" loop.
         */
        if (Is_knox(&u.uz) && newlev > 0 && !force_dest)
        {
            play_simple_player_sound(MONSTER_SOUND_TYPE_SHUDDER);
            You_ex1(ATR_NONE, CLR_MSG_ATTENTION, shudder_for_moment);
            return;
        }
        /* if in Quest, the player sees "Home 1", etc., on the status
         * line, instead of the logical depth of the level.  controlled
         * level teleport request is likely to be relativized to the
         * status line, and consequently it should be incremented to
         * the value of the logical depth of the target level.
         *
         * we let negative values requests fall into the "heaven" loop.
         */
        if (In_quest(&u.uz) && newlev > 0)
            newlev = newlev + dungeons[u.uz.dnum].depth_start - 1;
    } 
    else
    { /* involuntary level tele */
random_levtport:
        tcnt = 0;
        do
        {
            tcnt++;
            newlev = random_teleport_level();

        } while (newlev == depth(&u.uz) && tcnt < (teletype == 0 ? 1 : 10));

        if (newlev == depth(&u.uz))
        {
            play_simple_player_sound(MONSTER_SOUND_TYPE_SHUDDER);
            You_ex1(ATR_NONE, CLR_MSG_ATTENTION, shudder_for_moment);
            return;
        }
    }

    if (u.utrap && u.utraptype == TT_BURIEDBALL)
        buried_ball_to_punishment();

    if (!next_to_u() && !force_dest) 
    {
        play_simple_player_sound(MONSTER_SOUND_TYPE_SHUDDER);
        You_ex1(ATR_NONE, CLR_MSG_ATTENTION, shudder_for_moment);
        return;
    }

    if (In_endgame(&u.uz)) 
    { /* must already be wizard */
        int llimit = dunlevs_in_dungeon(&u.uz);

        if (newlev >= 0 || newlev <= -llimit) 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "get there from here.");
            return;
        }
        newlevel.dnum = u.uz.dnum;
        newlevel.dlevel = llimit + newlev;
        level_teleport_effect_out(u.ux, u.uy);
        schedule_goto(&newlevel, FALSE, FALSE, TRUE, FALSE, 0, (char *) 0, (char *) 0);
        if(isok(u.ux, u.uy))
            newsym_with_flags(u.ux, u.uy, NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS);
        return;
    }

    killer.name[0] = 0; /* still alive, so far... */
    killer.hint_idx = 0;

    if (iflags.debug_fuzzer && newlev < 0)
        goto random_levtport;

    level_teleport_effect_out(u.ux, u.uy);

    if (controltype != 2)
    {
        if (newlev < 0 && !force_dest)
        {
            if (*u.ushops0) {
                /* take unpaid inventory items off of shop bills */
                in_mklev = TRUE; /* suppress map update */
                u_left_shop(u.ushops0, TRUE);
                /* you're now effectively out of the shop */
                *u.ushops0 = *u.ushops = '\0';
                in_mklev = FALSE;
            }
            if (newlev <= -10) {
                You("arrive in heaven.");
                play_voice_god_simple_line_by_align(u.ualign.type, GOD_LINE_THOU_ART_EARLY_BUT_WELL_ADMIT_THEE);
                verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou art early, but we'll admit thee.");
                killer.format = NO_KILLER_PREFIX;
                Strcpy(killer.name, "went to heaven prematurely");
            }
            else if (newlev == -9) {
                You_feel("deliriously happy.");
                pline("(In fact, you're on Cloud 9!)");
                display_nhwindow(WIN_MESSAGE, FALSE);
            }
            else
                You("are now high above the clouds...");

            if (killer.name[0]) {
                ; /* arrival in heaven is pending */
            }
            else if (Levitation) {
                escape_by_flying = "float gently down to earth";
            }
            else if (Flying) {
                escape_by_flying = "fly down to the ground";
            }
            else {
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Unfortunately, you don't know how to fly.");
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "plummet a few thousand feet to your death.");
                Sprintf(killer.name,
                    "teleported out of the dungeon and fell to %s death",
                    uhis());
                killer.format = NO_KILLER_PREFIX;
            }
        }

        if (killer.name[0]) { /* the chosen destination was not survivable */
            d_level lsav;

            /* set specific death location; this also suppresses bones */
            lsav = u.uz;   /* save current level, see below */
            u.uz.dnum = 0; /* main dungeon */
            u.uz.dlevel = (newlev <= -10) ? -10 : 0; /* heaven or surface */
            done(DIED);
            /* can only get here via life-saving (or declining to die in
               explore|debug mode); the hero has now left the dungeon... */
            escape_by_flying = "find yourself back on the surface";
            u.uz = lsav; /* restore u.uz so escape code works */
        }

        /* calls done(ESCAPED) if newlevel==0 */
        if (escape_by_flying) {
            You("%s.", escape_by_flying);
            newlevel.dnum = 0;   /* specify main dungeon */
            newlevel.dlevel = 0; /* escape the dungeon */
            /* [dlevel used to be set to 1, but it doesn't make sense to
                teleport out of the dungeon and float or fly down to the
                surface but then actually arrive back inside the dungeon] */
        }
        else if (u.uz.dnum == main_dungeon_dnum
            && newlev >= dungeons[u.uz.dnum].depth_start
            + dunlevs_in_dungeon(&u.uz))
        {
            if (!(wizard && force_dest))
            {
                get_level_in_dungeon(&newlevel, newlev, gehennom_dnum);
                if (!find_mapseen(&newlevel))
                    find_hell(&newlevel);
            }
        }
        else
        {
            /* if invocation did not yet occur, teleporting into
             * the last level of Gehennom is forbidden.
             */
            if (!wizard && Inhell && !u.uevent.invoked
                && newlev >= (dungeons[u.uz.dnum].depth_start
                    + dunlevs_in_dungeon(&u.uz) - 1))
            {
                newlev = dungeons[u.uz.dnum].depth_start
                    + dunlevs_in_dungeon(&u.uz) - 2;
                pline("Sorry...");
            }
            /* no teleporting out of quest dungeon */
            if (In_quest(&u.uz) && newlev < depth(&qstart_level))
                newlev = depth(&qstart_level);
            /* the player thinks of levels purely in logical terms, so
             * we must translate newlev to a number relative to the
             * current dungeon.
             */
            if (!(wizard && force_dest))
                get_level(&newlevel, newlev);
        }
    }

    schedule_goto(&newlevel, FALSE, FALSE, TRUE, !!(tele_flags & 1), 0, (char*)0, (char*)0);
    /* in case player just read a scroll and is about to be asked to
       call it something, we can't defer until the end of the turn */
    if (u.utotype && !context.mon_moving)
        deferred_goto();

    if (isok(u.ux, u.uy))
        newsym_with_flags(u.ux, u.uy, NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS);
}

void
domagicportal(ttmp)
register struct trap *ttmp;
{
    struct d_level target_level;

    if (u.utrap && u.utraptype == TT_BURIEDBALL)
        buried_ball_to_punishment();

    if (!next_to_u()) {
        play_simple_player_sound(MONSTER_SOUND_TYPE_SHUDDER);
        You_ex1(ATR_NONE, CLR_MSG_ATTENTION, shudder_for_moment);
        return;
    }

    /* if landed from another portal, do nothing */
    /* problem: level teleport landing escapes the check */
    if (!on_level(&u.uz, &u.uz0))
        return;

    play_sfx_sound(SFX_MAGIC_PORTAL_ACTIVATE);
    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "activated a magic portal!");

    /* prevent the poor shnook, whose amulet was stolen while in
     * the endgame, from accidently triggering the portal to the
     * next level, and thus losing the game
     */
    if (In_endgame(&u.uz) && !u.uhave.amulet) {
        You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "dizzy for a moment, but nothing happens...");
        return;
    }

    int portal_flags = (ttmp->ttyp == MODRON_PORTAL ? 
        ((ttmp->tflags & TRAPFLAGS_LEVEL_TELEPORT_NO_OTHER_END) != 0 ? 4 : 
            (ttmp->tflags & TRAPFLAGS_LEVEL_TELEPORT_UP) != 0 ? 3 : 
            (ttmp->tflags & TRAPFLAGS_LEVEL_TELEPORT_DOWN) != 0 ? 2 : 4) : 1);

    target_level = ttmp->dst;

    level_teleport_effect_out(u.ux, u.uy);

    schedule_goto(&target_level, FALSE, FALSE, TRUE, FALSE, portal_flags,
                  "You feel dizzy for a moment, but the sensation passes.",
                  (char *) 0);
}

void
level_teleport_effect_out(x, y)
int x, y;
{
    boolean isyou = (x == u.ux && y == u.uy);

    play_special_effect_at(SPECIAL_EFFECT_LEVEL_TELEPORT_OUT, 0, x, y, isyou);
    play_sfx_sound(SFX_LEVEL_TELEPORT);
    special_effect_wait_until_action(0);
    show_glyph_on_layer(x, y, NO_GLYPH, LAYER_MONSTER);
    force_redraw_at(x, y);
    flush_screen(1);
    special_effect_wait_until_end(0);
}

void
level_teleport_effect_in(x, y)
int x, y;
{
    boolean isyou = (x == u.ux && y == u.uy);
    struct layer_info layers = layers_at(x, y);
    show_glyph_on_layer(x, y, NO_GLYPH, LAYER_MONSTER);
    force_redraw_at(x, y);
    flush_screen(1);
    play_special_effect_at(SPECIAL_EFFECT_LEVEL_TELEPORT_IN, 0, x, y, isyou);
    play_sfx_sound(SFX_LEVEL_TELEPORT);
    special_effect_wait_until_action(0);
    show_gui_glyph_on_layer(x, y, layers.layer_glyphs[LAYER_MONSTER], layers.layer_gui_glyphs[LAYER_MONSTER], LAYER_MONSTER);
    force_redraw_at(x, y);
    flush_screen(1);
    special_effect_wait_until_end(0);
}

void
tele_trap(trap)
struct trap *trap;
{
    play_sfx_sound(SFX_TELEPORT_TRAP_ACTIVATE);
    if (In_endgame(&u.uz) || Antimagic) {
        play_sfx_sound(SFX_WRENCHING_SENSATION);
        if (Antimagic)
            u_shieldeff();
        You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "a wrenching sensation.");
    } else if (!next_to_u()) {
        play_simple_player_sound(MONSTER_SOUND_TYPE_SHUDDER);
        You_ex1(ATR_NONE, CLR_MSG_ATTENTION, shudder_for_moment);
    } else if (trap->once) {
        deltrap(trap);
        newsym(u.ux, u.uy); /* get rid of trap symbol */
        vault_tele();
    } else
        tele();
}

void
level_tele_trap(trap, trflags)
struct trap *trap;
unsigned trflags;
{
    char verbbuf[BUFSZ];

    if ((trflags & VIASITTING) != 0)
        Strcpy(verbbuf, "trigger"); /* follows "You sit down." */
    else
        Sprintf(verbbuf, "%s onto",
                Levitation ? (const char *) "float"
                           : locomotion(youmonst.data, "step"));

    play_sfx_sound(SFX_LEVEL_TELEPORT_TRAP_ACTIVATE);
    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s a level teleport trap!", verbbuf);

    if (Antimagic) {
        u_shieldeff();
    }
    if (Antimagic || In_endgame(&u.uz)) {
        play_sfx_sound(SFX_WRENCHING_SENSATION);
        You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "a wrenching sensation.");
        return;
    }
    if (!Blind && !Flash_resistance)
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are momentarily blinded by a flash of light.");
    else
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are momentarily disoriented.");
    deltrap(trap);
    newsym(u.ux, u.uy); /* get rid of trap symbol */
    level_tele(0, FALSE, zerodlevel, 0);
}

/* check whether monster can arrive at location <x,y> via Tport (or fall) */
STATIC_OVL boolean
rloc_pos_ok(x, y, mtmp)
register int x, y; /* coordinates of candidate location */
struct monst *mtmp;
{
    register int xx, yy;

    if (!goodpos(x, y, mtmp, 0))
        return FALSE;
    /*
     * Check for restricted areas present in some special levels.
     *
     * `xx' is current column; if 0, then `yy' will contain flag bits
     * rather than row:  bit #0 set => moving upwards; bit #1 set =>
     * inside the Wizard's tower.
     */
    xx = mtmp->mx;
    yy = mtmp->my;
    if (!xx) {
        /* no current location (migrating monster arrival) */
        if (dndest.nlx && On_W_tower_level(&u.uz))
            return (((yy & 2) != 0)
                    /* inside xor not within */
                    ^ !within_bounded_area(x, y, dndest.nlx, dndest.nly,
                                           dndest.nhx, dndest.nhy));
        if (updest.lx && (yy & 1) != 0) /* moving up */
            return (within_bounded_area(x, y, updest.lx, updest.ly,
                                        updest.hx, updest.hy)
                    && (!updest.nlx
                        || !within_bounded_area(x, y, updest.nlx, updest.nly,
                                                updest.nhx, updest.nhy)));
        if (dndest.lx && (yy & 1) == 0) /* moving down */
            return (within_bounded_area(x, y, dndest.lx, dndest.ly,
                                        dndest.hx, dndest.hy)
                    && (!dndest.nlx
                        || !within_bounded_area(x, y, dndest.nlx, dndest.nly,
                                                dndest.nhx, dndest.nhy)));
    } else {
        /* [try to] prevent a shopkeeper or temple priest from being
           sent out of his room (caller might resort to goodpos() if
           we report failure here, so this isn't full prevention) */
        if (mtmp->isshk && inhishop(mtmp)) {
            if (levl[x][y].roomno != ESHK(mtmp)->shoproom)
                return FALSE;
        } else if (mtmp->ispriest && inhistemple(mtmp)) {
            if (levl[x][y].roomno != EPRI(mtmp)->shroom)
                return FALSE;
        } else if (mtmp->issmith && inhissmithy(mtmp)) {
            if (levl[x][y].roomno != ESMI(mtmp)->smithy_room)
                return FALSE;
        } else if (mtmp->isnpc && in_his_npc_room(mtmp)) {
            if (levl[x][y].roomno != ENPC(mtmp)->npc_room)
                return FALSE;
        }
        /* current location is <xx,yy> */
        if (!tele_jump_ok(xx, yy, x, y, FALSE))
            return FALSE;
    }
    /* <x,y> is ok */
    return TRUE;
}

/*
 * rloc_to()
 *
 * Pulls a monster from its current position and places a monster at
 * a new x and y.  If oldx is 0, then the monster was not in the
 * levels.monsters array.  However, if oldx is 0, oldy may still have
 * a value because mtmp is a migrating_mon.  Worm tails are always
 * placed randomly around the head of the worm.
 */
void
rloc_to(mtmp, x, y)
struct monst *mtmp;
register int x, y;
{
    register int oldx = mtmp->mx, oldy = mtmp->my;
    boolean resident_shk = mtmp->isshk && inhishop(mtmp);

    if (x == mtmp->mx && y == mtmp->my && m_at(x, y) == mtmp)
    return; /* that was easy */

    if (oldx) { /* "pick up" monster */
        if (mtmp->wormno) {
            remove_worm(mtmp);
        }
        else {
            remove_monster(oldx, oldy);
            newsym(oldx, oldy); /* update old location */
        }
    }

    memset(mtmp->mtrack, 0, sizeof mtmp->mtrack);
    place_monster(mtmp, x, y); /* put monster down */
    update_monster_region(mtmp);

    if (mtmp->wormno) /* now put down tail */
    place_worm_tail_randomly(mtmp, x, y);

    if (u.ustuck == mtmp) {
        if (u.uswallow) {
            u.ux = x;
            u.uy = y;
            docrt();
        }
        else
            u.ustuck = 0;
    }

    newsym(x, y);      /* update new location */
    set_apparxy(mtmp); /* orient monster */

    /* shopkeepers will only teleport if you zap them with a wand of
       teleportation or if they've been transformed into a jumpy monster;
       the latter only happens if you've attacked them with polymorph */
    if (resident_shk && !inhishop(mtmp))
    make_angry_shk(mtmp, oldx, oldy);
}

/* place a monster at a random location, typically due to teleport */
/* return TRUE if successful, FALSE if not */
boolean
rloc(mtmp, suppress_impossible)
struct monst* mtmp; /* mx==0 implies migrating monster arrival */
boolean suppress_impossible;
{
    register int x, y, trycount;

    if (mtmp == u.usteed) {
        tele();
        return TRUE;
    }

    if (mtmp->iswiz && mtmp->mx) { /* Wizard, not just arriving */
        if (!In_W_tower(u.ux, u.uy, &u.uz))
            x = xupstair, y = yupstair;
        else if (!xdnladder) /* bottom level of tower */
            x = xupladder, y = yupladder;
        else
            x = xdnladder, y = ydnladder;
        /* if the wiz teleports away to heal, try the up staircase,
           to block the player's escaping before he's healed
           (deliberately use `goodpos' rather than `rloc_pos_ok' here) */
        if (goodpos(x, y, mtmp, 0))
            goto found_xy;
    }

    trycount = 0;
    do {
        x = rn1(COLNO - 3, 2);
        y = rn2(ROWNO);
        if ((trycount < 500) ? rloc_pos_ok(x, y, mtmp)
            : goodpos(x, y, mtmp, 0))
            goto found_xy;
    } while (++trycount < 1000);

    /* last ditch attempt to find a good place */
    for (x = 2; x < COLNO - 1; x++)
        for (y = 0; y < ROWNO; y++)
            if (goodpos(x, y, mtmp, 0))
                goto found_xy;

    /* level either full of monsters or somehow faulty */
    if (!suppress_impossible)
        impossible("rloc(): couldn't relocate monster");
    return FALSE;

found_xy:
    rloc_to(mtmp, x, y);
    return TRUE;
}

boolean
rloc_with_effects(mtmp, suppress_impossible)
struct monst* mtmp; /* mx==0 implies migrating monster arrival */
boolean suppress_impossible;
{
    if (!mtmp)
        return FALSE;

    boolean res = FALSE;
    boolean spef1ok = FALSE;
    boolean spef2ok = FALSE;
    if (isok(mtmp->mx, mtmp->my) && canspotmon(mtmp))
    {
        spef1ok = TRUE;
        play_special_effect_at(SPECIAL_EFFECT_TELEPORT_OUT, 0, mtmp->mx, mtmp->my, FALSE);
        play_sfx_sound_at_location(SFX_TELEPORT, mtmp->mx, mtmp->my);
        special_effect_wait_until_action(0);
        show_glyph_on_layer(mtmp->mx, mtmp->my, NO_GLYPH, LAYER_MONSTER);
        force_redraw_at(mtmp->mx, mtmp->my);
        flush_screen(1);
    }
    res = rloc(mtmp, suppress_impossible);
    if (isok(mtmp->mx, mtmp->my) && canspotmon(mtmp))
    {
        spef2ok = TRUE;
        play_special_effect_at(SPECIAL_EFFECT_TELEPORT_IN, 1, mtmp->mx, mtmp->my, FALSE);
        play_sfx_sound_at_location(SFX_TELEPORT, mtmp->mx, mtmp->my);
        special_effect_wait_until_action(1);
    }
    if(spef1ok)
        special_effect_wait_until_end(0);
    if(spef2ok)
        special_effect_wait_until_end(1);

    return res;
}

boolean
rloc2(mtmp, suppress_impossible, has_effects)
struct monst* mtmp; /* mx==0 implies migrating monster arrival */
boolean suppress_impossible, has_effects;
{
    if (has_effects)
        return rloc_with_effects(mtmp, suppress_impossible);
    else
        return rloc(mtmp, suppress_impossible);
}


void
rloc_to_with_effects(mtmp, x, y)
struct monst* mtmp;
register int x, y;
{
    if (!mtmp)
        return;

    boolean spef1ok = FALSE;
    boolean spef2ok = FALSE;
    if (isok(mtmp->mx, mtmp->my) && canspotmon(mtmp))
    {
        spef1ok = TRUE;
        play_sfx_sound_at_location(SFX_TELEPORT, mtmp->mx, mtmp->my);
        play_special_effect_at(SPECIAL_EFFECT_TELEPORT_OUT, 0, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(0);
        show_glyph_on_layer(mtmp->mx, mtmp->my, NO_GLYPH, LAYER_MONSTER);
        force_redraw_at(mtmp->mx, mtmp->my);
        flush_screen(1);
    }
    rloc_to(mtmp, x, y);
    if (isok(mtmp->mx, mtmp->my) && canspotmon(mtmp))
    {
        spef2ok = TRUE;
        play_sfx_sound_at_location(SFX_TELEPORT, mtmp->mx, mtmp->my);
        play_special_effect_at(SPECIAL_EFFECT_TELEPORT_IN, 1, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(1);
    }
    if (spef1ok)
        special_effect_wait_until_end(0);
    if (spef2ok)
        special_effect_wait_until_end(1);
}

STATIC_OVL void
mvault_tele(mtmp)
struct monst *mtmp;
{
    register struct mkroom *croom = search_special(VAULT);
    coord c;

    if (croom && somexy(croom, &c) && goodpos(c.x, c.y, mtmp, 0)) {
        rloc_to(mtmp, c.x, c.y);
        return;
    }
    (void)rloc2(mtmp, TRUE, TRUE);
}

boolean
tele_restrict(mon)
struct monst *mon;
{
    if (level.flags.noteleport)
    {
        if (canseemon(mon))
        {
            play_sfx_sound_at_location(SFX_MYSTERIOUS_FORCE_PREVENTS, mon->mx, mon->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A mysterious force prevents %s from teleporting!",
                mon_nam(mon));
        }
        return TRUE;
    }
    return FALSE;
}

void
mtele_trap(mtmp, trap, in_sight)
struct monst *mtmp;
struct trap *trap;
int in_sight;
{
    char *monname;

    if (tele_restrict(mtmp))
        return;

    play_sfx_sound_at_location(SFX_TELEPORT_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
    if (teleport_pet(mtmp, FALSE)) {
        /* save name with pre-movement visibility */
        monname = Monnam(mtmp);

        /* Note: don't remove the trap if a vault.  Other-
         * wise the monster will be stuck there, since
         * the guard isn't going to come for it...
         */
        if (trap->once)
            mvault_tele(mtmp);
        else
            (void)rloc2(mtmp, TRUE, TRUE);

        if (in_sight) {
            play_special_effect_at(SPECIAL_EFFECT_TELEPORT_IN, 1, mtmp->mx, mtmp->my, FALSE);
            special_effect_wait_until_action(1);
            if (canseemon(mtmp))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems disoriented.", monname);
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly disappears!", monname);
            seetrap(trap);
            special_effect_wait_until_end(1);
        }
        special_effect_wait_until_end(0);
    }
}

/* return 0 if still on level, 3 if not */
int
mlevel_tele_trap(mtmp, trap, force_it, in_sight)
struct monst *mtmp;
struct trap *trap;
boolean force_it;
int in_sight;
{
    int tt = (trap ? trap->ttyp : NO_TRAP);

    if (mtmp == u.ustuck) /* probably a vortex */
        return 0;         /* temporary? kludge */
    if (teleport_pet(mtmp, force_it)) 
    {
        d_level tolevel;
        int migrate_typ = MIGR_RANDOM;

        if (is_hole(tt))
        {
            if (Is_stronghold(&u.uz)) 
            {
                assign_level(&tolevel, &valley_level);
            } 
            else if (Is_botlevel(&u.uz))
            {
                if (in_sight && trap->tseen)
                    pline("%s avoids the %s.", Monnam(mtmp), trap_type_definitions[tt].type_name);
                return 0;
            } 
            else
            {
                get_level(&tolevel, depth(&u.uz) + 1);
            }
        }
        else if (tt == MAGIC_PORTAL || tt == MODRON_PORTAL)
        {
            if (In_endgame(&u.uz)
                && (mon_has_amulet(mtmp) || is_home_elemental(mtmp->data))) 
            {
                if (in_sight && mtmp->data->mlet != S_ELEMENTAL) {
                    pline("%s seems to shimmer for a moment.", Monnam(mtmp));
                    seetrap(trap);
                }
                return 0;
            } 
            else
            {
                if (tt == MODRON_PORTAL)
                {
                    if (trap->tflags & TRAPFLAGS_LEVEL_TELEPORT_DOWN)
                    {
                        trap->dst.dlevel = u.uz.dlevel + 1;
                        trap->dst.dnum = u.uz.dnum;
                        migrate_typ = MIGR_MODRON_PORTAL_UP;
                    }
                    else if (trap->tflags & TRAPFLAGS_LEVEL_TELEPORT_UP)
                    {
                        trap->dst.dlevel = u.uz.dlevel - 1;
                        trap->dst.dnum = u.uz.dnum;
                        migrate_typ = MIGR_MODRON_PORTAL_DOWN;
                    }
                    else
                        migrate_typ = MIGR_RANDOM;
                }
                else
                    migrate_typ = MIGR_PORTAL;
                assign_level(&tolevel, &trap->dst);
            }
            if(cansee(mtmp->mx, mtmp->my))
                level_teleport_effect_out(mtmp->mx, mtmp->my);
        }
        else if (tt == LEVEL_TELEP || tt == NO_TRAP)
        {
            int nlev;

            if (mon_has_amulet(mtmp) || In_endgame(&u.uz)
                /* NO_TRAP is used when forcing a monster off the level;
                   onscary(0,0,) is true for the Wizard, Riders, lawful
                   minions, Angels of any alignment, shopkeeper or priest
                   currently inside his or her own special room */
                || (tt == NO_TRAP && onscary(0, 0, mtmp)))
            {
                if (in_sight)
                    pline("%s seems very disoriented for a moment.",
                          Monnam(mtmp));
                return 0;
            }

            if (tt == NO_TRAP)
            {
                /* creature is being forced off the level to make room;
                   it will try to return to this level (at a random spot
                   rather than its current one) if the level is left by
                   the hero and then revisited */
                assign_level(&tolevel, &u.uz);
            } 
            else
            {
                nlev = random_teleport_level();
                if (nlev == depth(&u.uz))
                {
                    if (in_sight)
                        pline("%s shudders for a moment.", Monnam(mtmp));
                    return 0;
                }
                get_level(&tolevel, nlev);
            }
            if (cansee(mtmp->mx, mtmp->my))
                level_teleport_effect_out(mtmp->mx, mtmp->my);
        }
        else
        {
            impossible("mlevel_tele_trap: unexpected trap type (%d)", tt);
            return 0;
        }

        if (in_sight)
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly, %s disappears out of sight.", mon_nam(mtmp));
            if (trap)
                seetrap(trap);
        }
        migrate_to_level(mtmp, ledger_no(&tolevel), migrate_typ, (coord *) 0);
        return 3; /* no longer on this level */
    }
    return 0;
}

/* place object randomly, returns False if it's gone (eg broken) */
boolean
rloco(obj)
register struct obj *obj;
{
    register xchar tx, ty, otx, oty;
    boolean restricted_fall;
    int try_limit = 4000;

    if (obj->otyp == CORPSE && obj->corpsenm >= LOW_PM && is_rider(&mons[obj->corpsenm])) {
        if (revive_corpse(obj))
            return FALSE;
    }

    obj->speflags &= ~SPEFLAGS_CAUGHT_IN_LEAVES;
    obj_clear_found(obj);

    Strcpy(debug_buf_2, "rloco");
    obj_extract_self(obj);
    otx = obj->ox;
    oty = obj->oy;
    restricted_fall = (otx == 0 && dndest.lx);
    do {
        tx = rn1(COLNO - 3, 2);
        ty = rn2(ROWNO);
        if (!--try_limit)
            break;
    } while (!goodpos(tx, ty, (struct monst *) 0, 0)
             || (restricted_fall
                 && (!within_bounded_area(tx, ty, dndest.lx, dndest.ly,
                                          dndest.hx, dndest.hy)
                     || (dndest.nlx
                         && within_bounded_area(tx, ty,
                                                dndest.nlx, dndest.nly,
                                                dndest.nhx, dndest.nhy))))
             /* on the Wizard Tower levels, objects inside should
                stay inside and objects outside should stay outside */
             || (dndest.nlx && On_W_tower_level(&u.uz)
                 && within_bounded_area(tx, ty, dndest.nlx, dndest.nly,
                                        dndest.nhx, dndest.nhy)
                    != within_bounded_area(otx, oty, dndest.nlx, dndest.nly,
                                           dndest.nhx, dndest.nhy)));

    if (flooreffects(obj, tx, ty, "fall")) {
        return FALSE;
    } else if (otx == 0 && oty == 0) {
        ; /* fell through a trap door; no update of old loc needed */
    } else {
        if (costly_spot(otx, oty)
            && (!costly_spot(tx, ty)
                || !index(in_rooms(tx, ty, 0), *in_rooms(otx, oty, 0)))) {
            if (costly_spot(u.ux, u.uy)
                && index(u.urooms, *in_rooms(otx, oty, 0)))
                addtobill(obj, FALSE, FALSE, FALSE);
            else
                (void) stolen_value(obj, otx, oty, FALSE, FALSE);
        }
        newsym(otx, oty); /* update old location */
    }
    place_object(obj, tx, ty);
    newsym(tx, ty);
    return TRUE;
}

/* Returns an absolute depth */
int
random_teleport_level()
{
    int nlev, max_depth, min_depth, cur_depth = (int) depth(&u.uz);

    /* [the endgame case can only occur in wizard mode] */
    if (!rn2(5) || Is_knox(&u.uz) || In_endgame(&u.uz))
        return cur_depth;

    /* What I really want to do is as follows:
     * -- If in a dungeon that goes down, the new level is to be restricted
     *    to [top of parent, bottom of current dungeon]
     * -- If in a dungeon that goes up, the new level is to be restricted
     *    to [top of current dungeon, bottom of parent]
     * -- If in a quest dungeon or similar dungeon entered by portals,
     *    the new level is to be restricted to [top of current dungeon,
     *    bottom of current dungeon]
     * The current behavior is not as sophisticated as that ideal, but is
     * still better what we used to do, which was like this for players
     * but different for monsters for no obvious reason.  Currently, we
     * must explicitly check for special dungeons.  We check for Knox
     * above; endgame is handled in the caller due to its different
     * message ("disoriented").
     * --KAA
     * 3.4.2: explicitly handle quest here too, to fix the problem of
     * monsters sometimes level teleporting out of it into main dungeon.
     * Also prevent monsters reaching the Sanctum prior to invocation.
     */
    if (In_quest(&u.uz)) {
        int bottom = dunlevs_in_dungeon(&u.uz),
            qlocate_depth = qlocate_level.dlevel;

        /* if hero hasn't reached the middle locate level yet,
           no one can randomly teleport past it */
        if (dunlev_reached(&u.uz) < qlocate_depth)
            bottom = qlocate_depth;
        min_depth = dungeons[u.uz.dnum].depth_start;
        max_depth = bottom + (dungeons[u.uz.dnum].depth_start - 1);
    } else {
        min_depth = 1;
        max_depth =
            dunlevs_in_dungeon(&u.uz) + (dungeons[u.uz.dnum].depth_start - 1);
        /* can't reach Sanctum if the invocation hasn't been performed */
        if (Inhell && !u.uevent.invoked)
            max_depth -= 1;
    }

    /* Get a random value relative to the current dungeon */
    /* Range is 1 to current+3, current not counting */
    nlev = rn2(cur_depth + 3 - min_depth) + min_depth;
    if (nlev >= cur_depth)
        nlev++;

    if (nlev > max_depth) {
        nlev = max_depth;
        /* teleport up if already on bottom */
        if (Is_botlevel(&u.uz))
            nlev -= rnd(3);
    }
    if (nlev < min_depth) {
        nlev = min_depth;
        if (nlev == cur_depth) {
            nlev += rnd(3);
            if (nlev > max_depth)
                nlev = max_depth;
        }
    }
    return nlev;
}

/* you teleport a monster (via wand, spell, or poly'd q.mechanic attack);
   return false iff the attempt fails */
boolean
u_teleport_mon(mtmp, give_feedback)
struct monst *mtmp;
boolean give_feedback;
{
    if (!mtmp)
        return FALSE;

    coord cc;

    if((mtmp->data->geno & G_UNIQ) && tele_restrict(mtmp))
        return FALSE;

    if (mtmp->ispriest && *in_rooms(mtmp->mx, mtmp->my, TEMPLE)) 
    {
        if (give_feedback)
        {
            play_sfx_sound_at_location(SFX_GENERAL_RESISTS, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s resists your magic!", Monnam(mtmp));
        }
        return FALSE;
    }
    else
    {
        play_special_effect_at(SPECIAL_EFFECT_TELEPORT_OUT, 0, mtmp->mx, mtmp->my, FALSE);
        play_sfx_sound_at_location(SFX_TELEPORT, mtmp->mx, mtmp->my);
        special_effect_wait_until_action(0);
        if (level.flags.noteleport && u.uswallow && mtmp == u.ustuck)
        {
            if (give_feedback)
                You("are no longer inside %s!", mon_nam(mtmp));
            unstuck(mtmp);
            (void)rloc(mtmp, TRUE);
        }
        else if (is_rider(mtmp->data) && rn2(13)
            && enexto(&cc, u.ux, u.uy, mtmp->data))
        {
            rloc_to(mtmp, cc.x, cc.y);
        }
        else
        {
            (void)rloc(mtmp, TRUE);
        }
        play_special_effect_at(SPECIAL_EFFECT_TELEPORT_IN, 1, mtmp->mx, mtmp->my, FALSE);
        special_effect_wait_until_action(1);
        special_effect_wait_until_end(0);
        special_effect_wait_until_end(1);
        return TRUE;
    }
}

void
reset_teleport(VOID_ARGS)
{
    telescroll = 0;
}

/*teleport.c*/
