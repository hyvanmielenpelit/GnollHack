/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-05-22 */

/* GnollHack 4.0    display.c    $NHDT-Date: 1556835736 2019/05/02 22:22:16 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.101 $ */
/* Copyright (c) Dean Luick, with acknowledgements to Kevin Darcy */
/* and Dave Cohrs, 1990.                                          */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 *                      THE NEW DISPLAY CODE
 *
 * The old display code has been broken up into three parts: vision, display,
 * and drawing.  Vision decides what locations can and cannot be physically
 * seen by the hero.  Display decides _what_ is displayed at a given location.
 * Drawing decides _how_ to draw a monster, fountain, sword, etc.
 *
 * The display system uses information from the vision system to decide
 * what to draw at a given location.  The routines for the vision system
 * can be found in vision.c and vision.h.  The routines for display can
 * be found in this file (display.c) and display.h.  The drawing routines
 * are part of the window port.  See doc/window.doc for the drawing
 * interface.
 *
 * The display system deals with an abstraction called a glyph.  Anything
 * that could possibly be displayed has a unique glyph identifier.
 *
 * What is seen on the screen is a combination of what the hero remembers
 * and what the hero currently sees.  Objects and dungeon features (walls
 * doors, etc) are remembered when out of sight.  Monsters and temporary
 * effects are not remembered.  Each location on the level has an
 * associated glyph.  This is the hero's _memory_ of what he or she has
 * seen there before.
 *
 * Display rules:
 *
 *      If the location is in sight, display in order:
 *              visible (or sensed) monsters
 *              visible objects
 *              known traps
 *              background
 *
 *      If the location is out of sight, display in order:
 *              sensed monsters (via telepathy or persistent detection)
 *              warning (partly-sensed monster shown as an abstraction)
 *              memory
 *
 *      "Remembered, unseen monster" is handled like an object rather
 *      than a monster, and stays displayed whether or not it is in sight.
 *      It is removed when a visible or sensed or warned-of monster gets
 *      shown at its location or when searching or fighting reveals that
 *      no monster is there.
 *
 *
 * Here is a list of the major routines in this file to be used externally:
 *
 * newsym
 *
 * Possibly update the screen location (x,y).  This is the workhorse routine.
 * It is always correct --- where correct means following the in-sight/out-
 * of-sight rules.  **Most of the code should use this routine.**  This
 * routine updates the map and displays monsters.
 *
 *
 * map_background
 * map_object
 * map_trap
 * map_invisible
 * unmap_object
 *
 * If you absolutely must override the in-sight/out-of-sight rules, there
 * are two possibilities.  First, you can mess with vision to force the
 * location in sight then use newsym(), or you can  use the map_* routines.
 * The first has not been tried [no need] and the second is used in the
 * detect routines --- detect object, magic mapping, etc.  The map_*
 * routines *change* what the hero remembers.  All changes made by these
 * routines will be sticky --- they will survive screen redraws.  Do *not*
 * use these for things that only temporarily change the screen.  These
 * routines are also used directly by newsym().  unmap_object is used to
 * clear a remembered object when/if detection reveals it isn't there.
 *
 *
 * show_glyph
 *
 * This is direct (no processing in between) buffered access to the screen.
 * Temporary screen effects are run through this and its companion,
 * flush_screen().  There is yet a lower level routine, print_glyph(),
 * but this is unbuffered and graphic dependent (i.e. it must be surrounded
 * by graphic set-up and tear-down routines).  Do not use print_glyph().
 *
 *
 * see_monsters
 * see_objects
 * see_traps
 *
 * These are only used when something affects all of the monsters or
 * objects or traps.  For objects and traps, the only thing is hallucination.
 * For monsters, there are hallucination and changing from/to blindness, etc.
 *
 *
 * tmp_at
 *
 * This is a useful interface for displaying temporary items on the screen.
 * Its interface is different than previously, so look at it carefully.
 *
 *
 *
 * Parts of the rm structure that are used:
 *
 *      typ     - What is really there.
 *      glyph   - What the hero remembers.  This will never be a monster.
 *                Monsters "float" above this.
 *      lit     - True if the position is lit.  An optimization for
 *                lit/unlit rooms.
 *      waslit  - True if the position was *remembered* as lit.
 *      seenv   - A vector of bits representing the directions from which the
 *                hero has seen this position.  The vector's primary use is
 *                determining how walls are seen.  E.g. a wall sometimes looks
 *                like stone on one side, but is seen as wall from the other.
 *                Other uses are for unmapping detected objects and felt
 *                locations, where we need to know if the hero has ever
 *                seen the location.
 *      flags   - Additional information for the typ field.  Different for
 *                each typ.
 *      horizontal - Indicates whether the wall or door is horizontal or
 *                vertical.
 */
#include "hack.h"

STATIC_DCL void FDECL(display_monster,
                      (XCHAR_P, XCHAR_P, struct monst *, int, XCHAR_P, BOOLEAN_P));
STATIC_DCL int FDECL(swallow_to_glyph, (int, int));
STATIC_DCL void FDECL(display_warning, (struct monst *));

STATIC_DCL int FDECL(check_pos, (int, int, int));
STATIC_DCL int FDECL(get_bk_glyph, (XCHAR_P, XCHAR_P));
STATIC_DCL int FDECL(get_floor_layer_glyph, (XCHAR_P, XCHAR_P));
STATIC_DCL int FDECL(get_floor_doodad_layer_glyph, (XCHAR_P, XCHAR_P));
STATIC_DCL int FDECL(get_feature_doodad_layer_glyph, (XCHAR_P, XCHAR_P, signed char*));
STATIC_DCL int FDECL(get_carpet_layer_glyph, (XCHAR_P, XCHAR_P));
STATIC_DCL int FDECL(tether_glyph, (int, int));

/*#define WA_VERBOSE*/ /* give (x,y) locations for all "bad" spots */
#ifdef WA_VERBOSE
STATIC_DCL boolean FDECL(more_than_one, (int, int, int, int, int));
#endif

STATIC_DCL int FDECL(set_twall, (int, int, int, int, int, int, int, int));
STATIC_DCL int FDECL(set_wall, (int, int, int));
STATIC_DCL int FDECL(set_corn, (int, int, int, int, int, int, int, int));
STATIC_DCL int FDECL(set_crosswall, (int, int));
STATIC_DCL void FDECL(set_seenv, (struct rm *, int, int, int, int));
STATIC_DCL void FDECL(t_warn, (struct rm *));

#define remember_topology(x, y) (lastseentyp[x][y] = levl[x][y].typ)


/* ======================================================================== */
/* Glyph Buffering (3rd screen) =========================================== */

typedef struct {
    xchar isnew; /* perhaps move this bit into the rm structure. */
    struct layer_info layers;
} gbuf_entry;

STATIC_VAR gbuf_entry gbuf[ROWNO][COLNO];
STATIC_VAR char gbuf_start[ROWNO];
STATIC_VAR char gbuf_stop[ROWNO];
STATIC_VAR const gbuf_entry nul_gbuf = { 0, nul_layerinfo };
STATIC_VAR boolean in_cls = 0;
STATIC_VAR boolean dela;
STATIC_VAR boolean delagr;
STATIC_VAR xchar lastx, lasty;
STATIC_VAR xchar lastswx, lastswy; /* last swallowed position */
STATIC_VAR int flushing = 0;
STATIC_VAR int delay_flushing = 0;

//{ base_cmap_to_glyph(S_unexplored), NO_GLYPH, { NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH, NO_GLYPH }, 0UL, (genericptr_t)0, 0}

/*
 * magic_map_background()
 *
 * This function is similar to map_background (see below) except we pay
 * attention to and correct unexplored, lit ROOM and CORR spots.
 */
void
magic_map_background(x, y, show)
xchar x, y;
int show;
{
    /* Hero gains knowledge of the lit status of the location */
    struct rm* lev = &levl[x][y];
    lev->waslit = lev->lit;

    map_background(x, y, show);

    remember_topology(x, y);
}

/*
 * The routines map_background(), map_object(), and map_trap() could just
 * as easily be:
 *
 *      map_glyph(x,y,glyph,show)
 *
 * Which is called with the xx_to_glyph() in the call.  Then I can get
 * rid of 3 routines that don't do very much anyway.  And then stop
 * having to create fake objects and traps.  However, I am reluctant to
 * make this change.
 */
/* FIXME: some of these use xchars for x and y, and some use ints.  Make
 * this consistent.
 */

/*
 * map_background()
 *
 * Make the real background part of our map.  This routine assumes that
 * the hero can physically see the location.  Update the screen if directed.
 */
void
map_background(x, y, show)
register xchar x, y;
register int show;
{
    register int glyph = back_to_glyph(x, y);
    int symbol_index = generic_glyph_to_cmap(glyph);
    int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, defsyms[symbol_index].layer, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));

    int new_floor_glyph = NO_GLYPH;
    int new_floor_gui_glyph = NO_GLYPH;
    int new_floor_doodad_glyph = symbol_index <= S_stone ? NO_GLYPH : get_floor_doodad_layer_glyph(x, y);
    int new_floor_doodad_gui_glyph = maybe_get_replaced_glyph(new_floor_doodad_glyph, x, y, data_to_replacement_info(new_floor_doodad_glyph, LAYER_FLOOR_DOODAD, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
    int new_feature_glyph = NO_GLYPH;
    int new_feature_gui_glyph = NO_GLYPH;
    int new_cover_feature_glyph = NO_GLYPH;
    int new_cover_feature_gui_glyph = NO_GLYPH;
    int new_carpet_glyph = symbol_index <= S_stone ? NO_GLYPH : get_carpet_layer_glyph(x, y);
    int new_carpet_gui_glyph = maybe_get_replaced_glyph(new_carpet_glyph, x, y, data_to_replacement_info(new_carpet_glyph, LAYER_CARPET, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
    schar new_feature_doodad_height = 0;
    int new_feature_doodad_glyph = symbol_index <= S_stone ? NO_GLYPH : get_feature_doodad_layer_glyph(x, y, &new_feature_doodad_height);
    int new_feature_doodad_gui_glyph = maybe_get_replaced_glyph(new_feature_doodad_glyph, x, y, data_to_replacement_info(new_feature_doodad_glyph, LAYER_FEATURE_DOODAD, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
    unsigned long new_layer_flags = 0UL;
    if (symbol_index > S_stone && levl[x][y].decoration_typ > 0)
        new_layer_flags |= LFLAGS_C_DECORATION;
    if (symbol_index > S_stone && levl[x][y].carpet_typ > 0)
        new_layer_flags |= LFLAGS_C_CARPET;

    if (defsyms[symbol_index].layer != LAYER_FLOOR)
    {
        new_floor_glyph = get_floor_layer_glyph(x, y);
        new_floor_gui_glyph = maybe_get_replaced_glyph(new_floor_glyph, x, y, data_to_replacement_info(new_floor_glyph, LAYER_FLOOR, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
        if (defsyms[symbol_index].layer == LAYER_FEATURE)
        {
            new_feature_glyph = glyph;
            new_feature_gui_glyph = gui_glyph;
            new_cover_feature_glyph = NO_GLYPH;
            new_cover_feature_gui_glyph = NO_GLYPH;
        }
        else
        {
            new_feature_glyph = NO_GLYPH;
            new_feature_gui_glyph = NO_GLYPH;
            new_cover_feature_glyph = glyph;
            new_cover_feature_gui_glyph = gui_glyph;
        }
    }
    else
    {
        new_floor_glyph = glyph;
        new_floor_gui_glyph = gui_glyph;
        new_feature_glyph = NO_GLYPH;
        new_feature_gui_glyph = NO_GLYPH;
        new_cover_feature_glyph = NO_GLYPH;
        new_cover_feature_gui_glyph = NO_GLYPH;
    }

    if (level.flags.hero_memory)
    {
        levl[x][y].hero_memory_layers.glyph = glyph; /* Ascii only */
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FLOOR] = new_floor_glyph;
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_CARPET] = new_carpet_glyph;
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FLOOR_DOODAD] = new_floor_doodad_glyph;
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FEATURE] = new_feature_glyph;
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FEATURE_DOODAD] = new_feature_doodad_glyph;
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_COVER_FEATURE] = new_cover_feature_glyph;

        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_FLOOR] = new_floor_gui_glyph;
        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_CARPET] = new_carpet_gui_glyph;
        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_FLOOR_DOODAD] = new_floor_doodad_gui_glyph;
        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_FEATURE] = new_feature_gui_glyph;
        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_FEATURE_DOODAD] = new_feature_doodad_gui_glyph;
        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_COVER_FEATURE] = new_cover_feature_gui_glyph;

        levl[x][y].hero_memory_layers.layer_flags = new_layer_flags;
        levl[x][y].hero_memory_layers.special_feature_doodad_layer_height = new_feature_doodad_height;
    }

    if (show)
    {
        int floor_glyph_before = gbuf[y][x].layers.layer_glyphs[LAYER_FLOOR];
        int carpet_glyph_before = gbuf[y][x].layers.layer_glyphs[LAYER_CARPET];
        int floor_doodad_glyph_before = gbuf[y][x].layers.layer_glyphs[LAYER_FLOOR_DOODAD];
        int feature_glyph_before = gbuf[y][x].layers.layer_glyphs[LAYER_FEATURE];
        int feature_doodad_glyph_before = gbuf[y][x].layers.layer_glyphs[LAYER_FEATURE_DOODAD];
        int cover_feature_glyph_before = gbuf[y][x].layers.layer_glyphs[LAYER_COVER_FEATURE];

        int floor_gui_glyph_before = gbuf[y][x].layers.layer_gui_glyphs[LAYER_FLOOR];
        int carpet_gui_glyph_before = gbuf[y][x].layers.layer_gui_glyphs[LAYER_CARPET];
        int floor_doodad_gui_glyph_before = gbuf[y][x].layers.layer_gui_glyphs[LAYER_FLOOR_DOODAD];
        int feature_gui_glyph_before = gbuf[y][x].layers.layer_gui_glyphs[LAYER_FEATURE];
        int feature_doodad_gui_glyph_before = gbuf[y][x].layers.layer_gui_glyphs[LAYER_FEATURE_DOODAD];
        int cover_feature_gui_glyph_before = gbuf[y][x].layers.layer_gui_glyphs[LAYER_COVER_FEATURE];

        //unsigned long flags_before = gbuf[y][x].layers.layer_flags;
        gbuf[y][x].layers.layer_glyphs[LAYER_FLOOR] = new_floor_glyph;
        gbuf[y][x].layers.layer_glyphs[LAYER_CARPET] = new_carpet_glyph;
        gbuf[y][x].layers.layer_glyphs[LAYER_FLOOR_DOODAD] = new_floor_doodad_glyph;
        gbuf[y][x].layers.layer_glyphs[LAYER_FEATURE] = new_feature_glyph;
        gbuf[y][x].layers.layer_glyphs[LAYER_FEATURE_DOODAD] = new_feature_doodad_glyph;
        gbuf[y][x].layers.layer_glyphs[LAYER_COVER_FEATURE] = new_cover_feature_glyph;

        gbuf[y][x].layers.layer_gui_glyphs[LAYER_FLOOR] = new_floor_gui_glyph;
        gbuf[y][x].layers.layer_gui_glyphs[LAYER_CARPET] = new_carpet_gui_glyph;
        gbuf[y][x].layers.layer_gui_glyphs[LAYER_FLOOR_DOODAD] = new_floor_doodad_gui_glyph;
        gbuf[y][x].layers.layer_gui_glyphs[LAYER_FEATURE] = new_feature_gui_glyph;
        gbuf[y][x].layers.layer_gui_glyphs[LAYER_FEATURE_DOODAD] = new_feature_doodad_gui_glyph;
        gbuf[y][x].layers.layer_gui_glyphs[LAYER_COVER_FEATURE] = new_cover_feature_gui_glyph;

        gbuf[y][x].layers.layer_flags = new_layer_flags;
        gbuf[y][x].layers.special_feature_doodad_layer_height = new_feature_doodad_height;

        if (floor_glyph_before != new_floor_glyph || carpet_glyph_before != new_carpet_glyph || floor_doodad_glyph_before != new_floor_doodad_glyph
            || feature_glyph_before != new_feature_glyph || feature_doodad_glyph_before != new_feature_doodad_glyph
            || cover_feature_glyph_before != new_cover_feature_glyph
            || floor_gui_glyph_before != new_floor_gui_glyph || carpet_gui_glyph_before != new_carpet_gui_glyph || floor_doodad_gui_glyph_before != new_floor_doodad_gui_glyph
            || feature_gui_glyph_before != new_feature_gui_glyph || feature_doodad_gui_glyph_before != new_feature_doodad_gui_glyph
            || cover_feature_gui_glyph_before != new_cover_feature_gui_glyph)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }

        show_glyph_ascii(x, y, glyph);
    }
}

/*
 * map_trap()
 *
 * Map the trap and print it out if directed.  This routine assumes that the
 * hero can physically see the location.
 */
void
map_trap(trap, show)
register struct trap *trap;
register int show;
{
    register int x = trap->tx, y = trap->ty;
    register int glyph = trap_to_glyph(trap, newsym_rn2);

    /* Replace */
    int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_TRAP, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));

    struct monst* mtmp = m_at(x, y);
    boolean utrapped = (x == u.ux && y == u.uy && u.utrap > 0);
    boolean mtrapped = (mtmp && mtmp->mtrapped);
    boolean trapped_draw_in_front = (trap->ttyp == STATUE_TRAP || ((utrapped || mtrapped) && (trap->ttyp == WEB || trap->ttyp == BEAR_TRAP)));

    if (level.flags.hero_memory)
    {
        levl[x][y].hero_memory_layers.glyph = glyph;
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_TRAP] = glyph;
        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_TRAP] = gui_glyph;
        if (trapped_draw_in_front)
        {
            levl[x][y].hero_memory_layers.layer_glyphs[LAYER_COVER_TRAP] = glyph; /* Semi-transparent redraw */
            levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_COVER_TRAP] = gui_glyph; /* Semi-transparent redraw */
        }
        if(mtrapped || utrapped)
            levl[x][y].hero_memory_layers.layer_flags |= LFLAGS_T_TRAPPED;
        else
            levl[x][y].hero_memory_layers.layer_flags &= ~LFLAGS_T_TRAPPED;

    }

    if (show)
    {
        show_glyph_ascii(x, y, glyph);
        show_gui_glyph_on_layer(x, y, glyph, gui_glyph, LAYER_TRAP);
        if (trapped_draw_in_front)
            show_gui_glyph_on_layer(x, y, glyph, gui_glyph, LAYER_COVER_TRAP);
        if (mtrapped || utrapped)
            add_glyph_buffer_layer_flags(x, y, LFLAGS_T_TRAPPED);
        else
            remove_glyph_buffer_layer_flags(x, y, LFLAGS_T_TRAPPED);

    }
}

/*
 * map_object()
 *
 * Map the given object.  This routine assumes that the hero can physically
 * see the location of the object.  Update the screen if directed.
 */
void
map_object(obj, show)
register struct obj* obj;
register int show;
{
    map_object_core(obj, show, TRUE, FALSE, FALSE);
}

void
map_object_for_detection(obj, show)
register struct obj* obj;
register int show;
{
    map_object_core(obj, show, TRUE, TRUE, FALSE);
}

void
map_object_no_chain_check(obj, show)
register struct obj* obj;
register int show;
{
    map_object_core(obj, show, FALSE, FALSE, FALSE);
}

void
map_object_no_chain_check_choose_ascii(obj, show, exclude_ascii)
register struct obj* obj;
register int show;
boolean exclude_ascii;
{
    map_object_core(obj, show, FALSE, FALSE, exclude_ascii);
}

void
map_object_core(obj, show, chain_check, add_detection_mark, exclude_ascii)
register struct obj *obj;
register int show;
boolean chain_check;
boolean add_detection_mark;
boolean exclude_ascii;
{
    if (!obj)
        return;

    register int x = obj->ox, y = obj->oy;
    register int glyph = obj_to_glyph(obj, newsym_rn2);
    boolean draw_in_front = is_obj_drawn_in_front(obj);
    enum layer_types layer = draw_in_front ? LAYER_COVER_OBJECT : LAYER_OBJECT;
    boolean in_pit = FALSE;
    struct trap* t = 0;
    if (isok(x, y) && (t = t_at(x, y)) != 0 && t->tseen && (t->ttyp == PIT || t->ttyp == SPIKED_PIT))
        in_pit = TRUE;

    int obj_height = get_obj_height(obj);

    /* Replace */
    int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, layer, obj, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));

    /* Save this object's glyph for showing in here window (ASCII) and in object pile in GUI */
    obj->glyph = glyph;
    obj->gui_glyph = gui_glyph;

    if (level.flags.hero_memory)
    {
        /* MRKR: While hallucinating, statues are seen as random monsters */
        /*       but remembered as random objects.                        */

        int new_glyph = glyph;
        int new_gui_glyph = gui_glyph;
        if (Hallucination && obj->otyp == STATUE)
        {
            new_glyph = random_obj_to_glyph(newsym_rn2);
            new_gui_glyph = maybe_get_replaced_glyph(new_glyph, x, y, data_to_replacement_info(glyph, layer, obj, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
            obj_height = 0;
        }

        if (!exclude_ascii)
        {
            levl[x][y].hero_memory_layers.glyph = new_glyph;
            levl[x][y].hero_memory_layers.layer_glyphs[layer] = new_glyph; /* Object layer glyph should be aligned with ascii glyph */
            levl[x][y].hero_memory_layers.layer_gui_glyphs[layer] = new_gui_glyph; /* Object layer glyph should be aligned with ascii glyph */
            levl[x][y].hero_memory_layers.object_height = obj_height;
        }

        if (obj->ox0 > 0 && obj->oy0 >= 0 && (obj->ox0 != obj->ox || obj->oy0 != obj->oy))
            levl[x][y].hero_memory_layers.o_id = obj->o_id;

        if (in_pit)
            levl[x][y].hero_memory_layers.layer_flags |= LFLAGS_O_IN_PIT;

        struct obj* memobj = o_on_memory(obj->o_id, levl[x][y].hero_memory_layers.memory_objchn);
        if (!chain_check || (chain_check && !memobj))
        {
            struct obj* dummy = memory_dummy_object(obj);
            if(dummy && add_detection_mark)
                dummy->speflags |= SPEFLAGS_DETECTED;
        }
        else if (add_detection_mark && memobj)
        {
            memobj->speflags |= SPEFLAGS_DETECTED;
        }
    }

    if (show)
    {
        if (!exclude_ascii)
        {
            show_glyph_ascii(x, y, glyph);
            show_gui_glyph_on_layer(x, y, glyph, gui_glyph, layer); /* Object layer glyph should be aligned with ascii glyph */
            set_glyph_buffer_object_height(x, y, obj_height);
        }

        if(obj->ox0 > 0 && obj->oy0 >= 0 && (obj->ox0 != obj->ox || obj->oy0 != obj->oy))
            set_glyph_buffer_oid(x, y, obj->o_id); /* Moving object */

        if(in_pit)
            add_glyph_buffer_layer_flags(x, y, LFLAGS_O_IN_PIT);
        if (isok(x, y) && uchain && obj == uchain && uchain->ox == x && uchain->oy == y)
            add_glyph_buffer_layer_flags(x, y, LFLAGS_O_CHAIN);
    }
}

/*
void
map_object_and_update_chain(obj, show)
register struct obj* obj;
register int show;
{
    if (!obj || !isok(obj->ox, obj->oy))
        return;

    map_object(obj, show);

    //if(o_on(obj->o_id, levl[obj->ox][obj->oy].hero_memory_layers.memory_objchn) == 0)
        memory_dummy_object(obj);
}
*/

/*
 * map_invisible()
 *
 * Make the hero remember that a square contains an invisible monster.
 * This is a special case in that the square will continue to be displayed
 * this way even when the hero is close enough to see it.  To get rid of
 * this and display the square's actual contents, use unmap_object() followed
 * by newsym() if necessary.
 */
void
map_invisible(x, y)
register xchar x, y;
{
    if (x != u.ux || y != u.uy) { /* don't display I at hero's location */
        if (level.flags.hero_memory)
        {
            levl[x][y].hero_memory_layers.glyph = GLYPH_INVISIBLE;
            levl[x][y].hero_memory_layers.layer_glyphs[LAYER_MONSTER] = GLYPH_INVISIBLE;
            levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_MONSTER] = GLYPH_INVISIBLE;
            clear_monster_layerinfo(&levl[x][y].hero_memory_layers);
        }
        show_monster_glyph_with_extra_info(x, y, GLYPH_INVISIBLE, (struct monst*)0, 0UL, 0, 0);
    }
}

void
clear_monster_layerinfo(linfo)
struct layer_info* linfo;
{
    if (!linfo)
        return;

    linfo->m_id = 0;
    linfo->special_monster_layer_height = 0;
    linfo->monster_origin_x = 0;
    linfo->monster_origin_y = 0;
    linfo->monster_hp = 0;
    linfo->monster_maxhp = 0;
    linfo->rider_glyph = NO_GLYPH;
    linfo->rider_gui_glyph = NO_GLYPH;
    linfo->status_bits = 0UL;
    linfo->condition_bits = 0UL;
    int i;
    for(i = 0; i < NUM_BUFF_BIT_ULONGS; i++)
        linfo->buff_bits[i] = 0UL;
    linfo->wsegdir = 0;
    linfo->reverse_prev_wsegdir = 0;
    linfo->monster_flags = 0UL;
}

boolean
unmap_invisible(x, y)
int x, y;
{
    if (isok(x,y) && glyph_is_invisible(levl[x][y].hero_memory_layers.glyph)) 
    {
        clear_monster_layer_memory_at(x, y);
        unmap_object(x, y);
        newsym(x, y);
        return TRUE;
    }
    return FALSE;
}


/*
 * unmap_object()
 *
 * Remove something from the map when the hero realizes it's not there any
 * more.  Replace it with background or known trap, but not with any other
 * If this is used for detection, a full screen update is imminent anyway;
 * if this is used to get rid of an invisible monster notation, we might have
 * to call newsym().
 */
void
unmap_object(x, y)
register int x, y;
{
    register struct trap *trap;

    if (!level.flags.hero_memory)
        return;

    boolean waslit = levl[x][y].waslit;
    clear_hero_memory_at(x, y);

    if (levl[x][y].seenv)
    {
        levl[x][y].waslit = waslit;
        map_background(x, y, 0);

        if ((trap = t_at(x, y)) != 0 && trap->tseen && !covers_traps(x, y))
        {
            map_trap(trap, 0);
        }
    }
}

/*
 * map_location()
 *
 * Make whatever at this location show up.  This is only for non-living
 * things.  This will not handle feeling invisible objects correctly.
 *
 * Internal to display.c, this is a #define for speed.
 */
/*
#define _map_location(x, y, show) \
    {                                                                       \
        register struct obj *obj;                                           \
        register struct trap *trap;                                         \
                                                                            \
        map_background(x, y, show);                                         \
        if ((trap = t_at(x, y)) && trap->tseen && !covers_traps(x, y))      \
            map_trap(trap, show);                                           \
        if ((obj = vobj_at(x, y)) && !covers_objects(x, y))                 \
            map_object(obj, show);                                          \
                                                                            \
        remember_topology(x, y);                                            \
    }
*/
void
map_location(x, y, show)
int x, y, show;
{
    struct obj* obj;
    struct trap* trap;

    /* Note that this should ordinarily include also waslit, except when one is blind / cannot see the location */

    /* Floor, feature, and doodad layers */
    map_background(x, y, show);

    /* Trap layer */
    if ((trap = t_at(x, y)) && trap->tseen && !covers_traps(x, y))
        map_trap(trap, show);
    else if (level.flags.hero_memory)
    {
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_TRAP] = NO_GLYPH;
        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_TRAP] = NO_GLYPH;
        levl[x][y].hero_memory_layers.layer_glyphs[LAYER_COVER_TRAP] = NO_GLYPH;
        levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_COVER_TRAP] = NO_GLYPH;
    }

    /* Object layer */
    clear_hero_object_memory_at(x, y);
    if (!covers_objects(x, y))
    {
        boolean show_first_object_layer = show;
        boolean show_first_cover_layer = show;
        boolean first = TRUE;

        for (obj = vobj_at(x, y); obj; obj = obj->nexthere)
        {
            boolean draw_in_front = is_obj_drawn_in_front(obj);
            boolean show_this_object = (draw_in_front ? show_first_cover_layer : show_first_object_layer);
            map_object_no_chain_check_choose_ascii(obj, show_this_object, !first);

            if(show_this_object)
                first = FALSE;

            if (draw_in_front)
                show_first_cover_layer = FALSE;
            else
                show_first_object_layer = FALSE;
        }
    }
    if (is_objpile(x, y) && !Hallucination)
        add_glyph_buffer_layer_flags(x, y, LFLAGS_O_PILE);

    remember_topology(x, y);

    //_map_location(x, y, show);
}

#define DETECTED 2
#define PHYSICALLY_SEEN 1
#define is_worm_tail(mon) ((mon) && ((x != (mon)->mx) || (y != (mon)->my)))

/*
 * display_monster()
 *
 * Note that this is *not* a map_XXXX() function!  Monsters sort of float
 * above everything.
 *
 * Yuck.  Display body parts by recognizing that the display position is
 * not the same as the monster position.  Currently the only body part is
 * a worm tail.
 *
 */
STATIC_OVL void
display_monster(x, y, mon, sightflags, worm_tail, dropping_piercer)
register xchar x, y;        /* display position */
register struct monst *mon; /* monster to display */
int sightflags;             /* 1 if the monster is physically seen;
                               2 if detected using Detect_monsters */
xchar worm_tail;            /* mon is actually a worm tail */
boolean dropping_piercer;
{
    if (!mon)
        return;

    boolean mon_mimic = (M_AP_TYPE(mon) != M_AP_NOTHING);
    int sensed = (mon_mimic && (Protection_from_shape_changers || sensemon(mon)));
    /*
     * We must do the mimic check first.  If the mimic is mimicing something,
     * and the location is in sight, we have to change the hero's memory
     * so that when the position is out of sight, the hero remembers what
     * the mimic was mimicing.
     */
    if (mon_mimic && (sightflags == PHYSICALLY_SEEN)) {
        switch (M_AP_TYPE(mon)) {
        default:
            impossible("display_monster:  bad m_ap_type value [ = %d ]",
                       (int) mon->m_ap_type);
            /*FALLTHRU*/
        case M_AP_NOTHING:
            show_monster_glyph_with_extra_info(x, y, any_mon_to_glyph(mon, newsym_rn2),  mon, 0UL, 0, 0);
            clear_monster_layer_memory_at(x, y);
            break;

        case M_AP_FURNITURE: {
            /*
             * This is a poor man's version of map_background().  I can't
             * use map_background() because we are overriding what is in
             * the 'typ' field.  Maybe have map_background()'s parameters
             * be (x,y,glyph) instead of just (x,y).
             *
             * mappearance is currently set to an S_ index value in
             * makemon.c.
             */
            int cmap_type = levl[x][y].use_special_tileset ? levl[x][y].special_tileset : get_current_cmap_type_index();
            int sym = mon->mappearance, glyph = cmap_with_type_to_glyph(sym, cmap_type);

            /* Replace */
            int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_FEATURE, (struct obj*)0, mon, 0UL, 0UL, MAT_NONE, 0));


            //levl[x][y].hero_memory_layers.glyph = glyph;
            if (level.flags.hero_memory)
            {
                levl[x][y].hero_memory_layers.glyph = glyph;
                levl[x][y].hero_memory_layers.layer_glyphs[LAYER_FEATURE] = glyph; /* Override, as otherwise not very credible */
                levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_FEATURE] = gui_glyph; /* Override, as otherwise not very credible */
            }
            if (!sensed) {
                //show_monster_glyph_with_extra_info(x, y, glyph, (struct monst*)0, LFLAGS_M_MIMIC_FURNITURE, 0, 0);
                /* override real topology with mimic's fake one */
                show_glyph_ascii(x, y, glyph);
                show_gui_glyph_on_layer(x, y, glyph, gui_glyph, LAYER_FEATURE);
                lastseentyp[x][y] = cmap_to_type(sym);
            }
            break;
        }

        case M_AP_OBJECT: {
            /* Make a fake object to send to map_object(). */
            struct obj obj;

            obj = zeroobj;
            obj.ox = x;
            obj.oy = y;
            obj.otyp = mon->mappearance;
            /* might be mimicing a corpse or statue */
            obj.corpsenm = has_mcorpsenm(mon) ? MCORPSENM(mon) : PM_TENGU;
            int glyph = obj_to_glyph(&obj, newsym_rn2);
            int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_OBJECT, &obj, mon, 0UL, 0UL, MAT_NONE, 0));
            obj.glyph = glyph;
            obj.gui_glyph = gui_glyph;
            //show_monster_glyph_with_extra_info(x, y,
            //    glyph, mon, LFLAGS_M_MIMIC_OBJECT, 0, 0);
            //map_object(&obj, !sensed);
            if (level.flags.hero_memory)
            {
                int new_glyph = glyph;
                int new_gui_glyph = gui_glyph;
                if (Hallucination && obj.otyp == STATUE)
                {
                    new_glyph = random_obj_to_glyph(newsym_rn2);
                    new_gui_glyph = maybe_get_replaced_glyph(new_glyph, x, y, data_to_replacement_info(new_glyph, LAYER_OBJECT, &obj, mon, 0UL, 0UL, MAT_NONE, 0));
                }
                obj.glyph = new_glyph;
                obj.gui_glyph = new_gui_glyph;
                levl[x][y].hero_memory_layers.glyph = new_glyph;
                levl[x][y].hero_memory_layers.layer_glyphs[LAYER_OBJECT] = new_glyph;
                levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_OBJECT] = new_gui_glyph;

                struct obj* memobj = m_on_memory(mon->m_id, levl[x][y].hero_memory_layers.memory_objchn);
                if (!memobj)
                {
                    struct obj* dummy = memory_dummy_object(&obj);
                    /* o_id is zero in any case */
                    if(dummy)
                        dummy->m_id_memory = mon->m_id;
                }
            }
            show_glyph_ascii(x, y, glyph);
            show_gui_glyph_on_layer(x, y, glyph, gui_glyph, LAYER_OBJECT);

            break;
        }

        case M_AP_MONSTER:
            show_monster_glyph_with_extra_info(x, y, any_monnum_to_glyph(mon->female, what_mon((int)mon->mappearance, rn2_on_display_rng)), mon, 0UL, 0, 0 );
            clear_monster_layer_memory_at(x, y);
            break;
        }
    }

    /* If mimic is unsuccessfully mimicing something, display the monster. */
    if (!mon_mimic || sensed) {
        int num;

        /* [ALI] Only use detected glyphs when monster wouldn't be
         * visible by any other means.
         *
         * There are no glyphs for "detected pets" so we have to
         * decide whether to display such things as detected or as tame.
         * If both are being highlighted in the same way, it doesn't
         * matter, but if not, showing them as pets is preferrable.
         */
        unsigned long extra_flags = 0;

        if (mon->mleashed)
        {
            extra_flags |= LFLAGS_M_TETHERED;
            show_leash_info(x, y, mon->mx, mon->my, u.ux, u.uy);
        }
        if (is_tame(mon) && !Hallucination) {
            if (worm_tail)
                num = monnum_to_glyph(get_worm_tail_mnum(mon->data));
            else
                num = any_mon_to_glyph(mon, rn2_on_display_rng);

            extra_flags |= LFLAGS_M_PET;
        } else if (sightflags == DETECTED) {
            if (worm_tail)
                num = monnum_to_glyph(
                             what_mon(get_worm_tail_mnum(mon->data), rn2_on_display_rng));
            else
                num = any_mon_to_glyph(mon, rn2_on_display_rng);

            extra_flags |= LFLAGS_M_DETECTED;
        } else {
            if (worm_tail)
                num = monnum_to_glyph(
                             what_mon(get_worm_tail_mnum(mon->data), rn2_on_display_rng));
            else
                num = any_mon_to_glyph(mon, rn2_on_display_rng);
        }
        if (worm_tail)
            extra_flags |= (LFLAGS_M_WORM_TAIL | LFLAGS_M_WORM_SEEN);

        if (dropping_piercer)
        {
            extra_flags |= LFLAGS_M_DROPPING_PIERCER;
            show_glyph_on_layer_and_ascii(x, y, num, LAYER_MISSILE);
            show_extra_info(x, y, extra_flags, 0, 0);
        }
        else
            show_monster_glyph_with_extra_info(x, y, num, /*worm_tail ? (struct monst*)0 :*/ mon, extra_flags, 0, 0);
        clear_monster_layer_memory_at(x, y);
    }
}

/*
 * display_warning()
 *
 * This is also *not* a map_XXXX() function!  Monster warnings float
 * above everything just like monsters do, but only if the monster
 * is not showing.
 *
 * Do not call for worm tails.
 */
STATIC_OVL void
display_warning(mon)
register struct monst *mon;
{
    int x = mon->mx, y = mon->my;
    int glyph;

    if (mon_warning(mon)) {
        int wl = Hallucination ?
            rn2_on_display_rng(WARNCOUNT - 1) + 1 : warning_of(mon);
        glyph = warning_to_glyph(wl);
    } else if (MATCH_WARN_OF_MON(mon)) {
        glyph = any_mon_to_glyph(mon, rn2_on_display_rng);
    } else {
        impossible("display_warning did not match warning type?");
        return;
    }
    /* warning glyph is drawn on the monster layer; unseen
       monster glyph is drawn on the object/trap/floor layer;
       if we see a 'warning' move onto 'remembered, unseen' we
       need to explicitly remove that in order for it to not
       reappear when the warned-of monster moves off that spot */
    if (glyph_is_invisible(levl[x][y].hero_memory_layers.glyph))
        unmap_object(x, y);

    show_monster_glyph_with_extra_info(x, y, glyph, (struct monst*)0, 0UL, 0, 0);
}

int
warning_of(mon)
struct monst *mon;
{
    int wl = 0, tmp = 0;

    if (mon_warning(mon)) 
    {
        double mon_difficulty = (double)(mon->cham >= LOW_PM ? max(mons[mon->mnum].difficulty, mons[mon->cham].difficulty) : mons[mon->mnum].difficulty);
        double warning_threshold[WARNCOUNT] = { 0 };
        warning_threshold[0] = max(0.0, (double)u.ulevel * 0.25);
        warning_threshold[1] = max(warning_threshold[0] + 1.0, (double)u.ulevel * 0.50);
        warning_threshold[2] = max(warning_threshold[1] + 1.0, (double)u.ulevel * 0.75);
        warning_threshold[4] = max(warning_threshold[2] + 2.0, max((double)u.ulevel + 1.0, (double)u.ulevel * 1.25));
        warning_threshold[5] = max(warning_threshold[4] + 1.0, max((double)u.ulevel + 2.0, (double)u.ulevel * 1.75));
        warning_threshold[6] = max(warning_threshold[5] + 1.0, max((double)u.ulevel + 3.0, (double)u.ulevel * 2.25));

        tmp = 3;
        if (mon_difficulty <= warning_threshold[0])
            tmp = 0;
        else if (mon_difficulty <= warning_threshold[1])
            tmp = 1;
        else if (mon_difficulty <= warning_threshold[2])
            tmp = 2;
        else if (mon_difficulty >= warning_threshold[6])
            tmp = 6;
        else if (mon_difficulty >= warning_threshold[5])
            tmp = 5;
        else if (mon_difficulty >= warning_threshold[4])
            tmp = 4;

        //tmp = (int) ((ratio + 0.4) / 0.4);    /* match display.h */
        wl = (tmp > WARNCOUNT - 1) ? WARNCOUNT - 1 : tmp;
        if (wl < 0)
            wl = 0;
    }
    return wl;
}


/*
 * feel_newsym()
 *
 * When hero knows what happened to location, even when blind.
 */
void
feel_newsym(x, y)
xchar x, y;
{
    if (Blind)
        feel_location(x, y);
    else
        newsym(x, y);
}


/*
 * feel_location()
 *
 * Feel the given location.  This assumes that the hero is blind and that
 * the given position is either the hero's or one of the eight squares
 * adjacent to the hero (except for a boulder push).
 * If an invisible monster has gone away, that will be discovered.  If an
 * invisible monster has appeared, this will _not_ be discovered since
 * searching only finds one monster per turn so we must check that separately.
 */
void
feel_location(x, y)
xchar x, y;
{
    struct rm *lev;
    struct obj *boulder;
    register struct monst *mon;

    if (!isok(x, y))
        return;

    lev = &(levl[x][y]);

    int orig_glyph = lev->hero_memory_layers.glyph;

    /* The hero can't feel non pool locations while under water
       except for lava and ice. */
    if (Underwater && !Is_waterlevel(&u.uz)
        && !is_pool_or_lava(x, y) && !is_ice(x, y))
        return;

    /* Set the seen vector as if the hero had seen it.
       It doesn't matter if the hero is levitating or not. */
    set_seenv(lev, u.ux, u.uy, x, y);

    if (!can_reach_floor(FALSE)) 
    {
        /*
         * Levitation Rules.  It is assumed that the hero can feel the state
         * of the walls around herself and can tell if she is in a corridor,
         * room, or doorway.  Boulders are felt because they are large enough.
         * Anything else is unknown because the hero can't reach the ground.
         * This makes things difficult.
         *
         * Check (and display) in order:
         *
         *      + Stone, walls, and closed doors.
         *      + Boulders.  [see a boulder before a doorway]
         *      + Doors.
         *      + Room/water positions
         *      + Everything else (hallways!)
         */

        // For simplicity, map always background
        map_background(x, y, 1);

        if ((boulder = sobj_at(BOULDER, x, y)) != 0)
        {
            map_object(boulder, 1);
        }
    } 
    else
    {
        map_location(x, y, 1);

        if (Punished) 
        {
            /*
             * A ball or chain is only felt if it is first on the object
             * location list.  Otherwise, we need to clear the felt bit ---
             * something has been dropped on the ball/chain.  If the bit is
             * not cleared, then when the ball/chain is moved it will drop
             * the wrong glyph.
             */
            if (uchain->ox == x && uchain->oy == y) 
            {
                if (level.objects[x][y] == uchain)
                    u.bc_felt |= BC_CHAIN;
                else
                    u.bc_felt &= ~BC_CHAIN; /* do not feel the chain */
            }

            if (!carried(uball) && uball->ox == x && uball->oy == y) 
            {
                if (level.objects[x][y] == uball)
                    u.bc_felt |= BC_BALL;
                else
                    u.bc_felt &= ~BC_BALL; /* do not feel the ball */
            }
        }
    }

    /* Monster layer */
    /* Clear out hero memory in the case nothing's found below */
    clear_monster_layer_memory_at(x, y);

    /* draw monster on top if we can sense it */
    if ((mon = m_at(x, y)) != 0)
    {
        /* If hero's memory of an invisible monster is accurate, we want to keep
         * him from detecting the same monster over and over again on each turn.
         * OBSOLETE: We must return (so we don't erase the monster).  (We must also, in the
         * search function, be sure to skip over previously detected 'I's.)
         */
        if (glyph_is_invisible(orig_glyph))
        {
            map_invisible(x, y);
        }
        else if (sensemon(mon))
            display_monster(x, y, mon,
                (tp_sensemon(mon) || MATCH_WARN_OF_MON(mon))
                ? PHYSICALLY_SEEN
                : DETECTED,
                is_worm_tail(mon), FALSE);
    }

    /* Environment layer */
    struct nhregion* reg = visible_region_at(x, y);
    if (reg && (ACCESSIBLE(lev->typ) || (reg->typ == REGION_POISON_GAS && is_pool_or_lava(x, y))))
    {
        if ((x == u.ux && y == u.uy && region_type_definitions[reg->typ].sensed_blind_at_location && iflags.using_gui_tiles) 
            ||((x != u.ux || y == u.uy) && region_type_definitions[reg->typ].sensed_by_touching_around)
           )
            show_region(reg, x, y);
    }
}

void
clear_layer_info(layers_ptr)
struct layer_info* layers_ptr;
{
    if (!layers_ptr)
        return;

    struct layer_info nul = nul_layerinfo;
    *layers_ptr = nul;

}

void
clear_object_glyphs_at(x, y)
int x, y;
{
    struct obj* otmp;
    for (otmp = vobj_at(x, y); otmp; otmp = otmp->nexthere)
    {
        otmp->glyph = NO_GLYPH;
        otmp->gui_glyph = NO_GLYPH;
    }
}

void
clear_hero_memory_at(x, y)
int x, y;
{
    struct layer_info* layer_ptr = &levl[x][y].hero_memory_layers;
    clear_hero_object_memory_at(x, y);
    clear_layer_info(layer_ptr);
    levl[x][y].waslit = 0;
}

void
clear_all_glyphs_at(x, y)
int x, y;
{
    clear_layer_info(&gbuf[y][x].layers);
    clear_object_glyphs_at(x, y);
}


/*
 * newsym()
 *
 * Possibly put a new glyph at the given location.
 */
void
newsym(x, y)
register int x, y;
{
    newsym_with_extra_info_and_flags(x, y, 0UL, 0, 0, 0UL);
}

void
newsym_with_flags(x, y, newsym_flags)
register int x, y;
unsigned long newsym_flags;
{
    newsym_with_extra_info_and_flags(x, y, 0UL, 0, 0, newsym_flags);
}

void
newsym_with_extra_info(x, y, disp_flags, hit_tile_id, damage_shown)
register int x, y;
unsigned long disp_flags;
int hit_tile_id, damage_shown;
{
    newsym_with_extra_info_and_flags(x, y, disp_flags, hit_tile_id, damage_shown, 0UL);
}

void
newsym_with_extra_info_and_flags(x, y, disp_flags, hit_tile_id, damage_shown, specific_newsym_flags)
register int x, y;
unsigned long disp_flags, specific_newsym_flags;
int hit_tile_id, damage_shown;
{
    if (!isok(x, y))
        return;

    /* Add global flags */
    unsigned long newsym_flags = (specific_newsym_flags | context.global_newsym_flags);

    register struct monst *mon;
    register struct rm *lev = &(levl[x][y]);
    register int see_it;
    register xchar worm_tail;
    //int orig_glyph = lev->hero_memory_layers.glyph;

    if (in_mklev)
        return;
#ifdef HANGUPHANDLING
    if (program_state.done_hup)
        return;
#endif

    struct layer_info layers = layers_at(x, y);
    int missile_glyph = NO_GLYPH;
    int missile_gui_glyph = NO_GLYPH;
    if (layers.layer_glyphs[LAYER_MISSILE] != 0 && layers.layer_glyphs[LAYER_MISSILE] != NO_GLYPH /* && (glyph_is_zap(layers.layer_glyphs[LAYER_MISSILE]) || glyph_is_missile(layers.layer_glyphs[LAYER_MISSILE]))*/)
    {
        missile_glyph = layers.layer_glyphs[LAYER_MISSILE];
        missile_gui_glyph = layers.layer_gui_glyphs[LAYER_MISSILE];
    }
    int missile_poisoned = layers.missile_poisoned;
    int missile_material = layers.missile_material;
    int missile_special_quality = layers.missile_special_quality;
    int missile_elemental_enchantment = layers.missile_elemental_enchantment;
    int missile_exceptionality = layers.missile_exceptionality;
    int missile_mythic_prefix = layers.missile_mythic_prefix;
    int missile_mythic_suffix = layers.missile_mythic_suffix;
    int missile_eroded = layers.missile_eroded;
    int missile_eroded2 = layers.missile_eroded2;
    unsigned long missile_flags = layers.missile_flags;
    short missile_height = layers.missile_height;
    xchar missile_origin_x = layers.missile_origin_x;
    xchar missile_origin_y = layers.missile_origin_y;

    int zap_glyph = NO_GLYPH;
    int zap_gui_glyph = NO_GLYPH;
    if (layers.layer_glyphs[LAYER_ZAP] != 0 && layers.layer_glyphs[LAYER_ZAP] != NO_GLYPH)
    {
        zap_glyph = layers.layer_glyphs[LAYER_ZAP];
        zap_gui_glyph = layers.layer_gui_glyphs[LAYER_ZAP];
    }

    int zap_flags = (layers.layer_flags & LFLAGS_ZAP_MASK);

    int background_effect_glyph = NO_GLYPH;
    int background_effect_gui_glyph = NO_GLYPH;
    if (layers.layer_glyphs[LAYER_BACKGROUND_EFFECT] != 0 && layers.layer_glyphs[LAYER_BACKGROUND_EFFECT] != NO_GLYPH)
    {
        background_effect_glyph = layers.layer_glyphs[LAYER_BACKGROUND_EFFECT];
        background_effect_gui_glyph = layers.layer_gui_glyphs[LAYER_BACKGROUND_EFFECT];
    }

    int general_effect_glyph = NO_GLYPH;
    int general_effect_gui_glyph = NO_GLYPH;
    if (layers.layer_glyphs[LAYER_GENERAL_EFFECT] != 0 && layers.layer_glyphs[LAYER_GENERAL_EFFECT] != NO_GLYPH)
    {
        general_effect_glyph = layers.layer_glyphs[LAYER_GENERAL_EFFECT];
        general_effect_gui_glyph = layers.layer_gui_glyphs[LAYER_GENERAL_EFFECT];
    }

    int monster_effect_glyph = NO_GLYPH;
    int monster_effect_gui_glyph = NO_GLYPH;
    if (layers.layer_glyphs[LAYER_MONSTER_EFFECT] != 0 && layers.layer_glyphs[LAYER_MONSTER_EFFECT] != NO_GLYPH)
    {
        monster_effect_glyph = layers.layer_glyphs[LAYER_MONSTER_EFFECT];
        monster_effect_gui_glyph = layers.layer_gui_glyphs[LAYER_MONSTER_EFFECT];
    }

    unsigned long old_flags = layers.layer_flags;

    /* only permit updating the hero when swallowed */
    if (u.uswallow)
    {
        if (x == u.ux && y == u.uy)
            display_self_with_extra_info_choose_ascii(disp_flags, hit_tile_id, damage_shown, FALSE);
        goto new_sym_end_here;
    }

    if (Underwater && !Is_waterlevel(&u.uz))
    {
        /* when underwater, don't do anything unless <x,y> is an
           adjacent water or lava or ice position */
        if (!(is_pool_or_lava(x, y) || is_ice(x, y)) || distu(x, y) > 2)
            goto new_sym_end_here;
    }

    /* First, clear all glyphs in the glyph buffer */
    clear_all_glyphs_at(x, y);

    /* Extra info shown */
    show_extra_info(x, y, disp_flags, hit_tile_id, damage_shown);

    /* Can physically see the location. */
    if (cansee(x, y)) 
    {
        boolean old_glyph_is_invisible = glyph_is_invisible(lev->hero_memory_layers.glyph);
        if(u.uachieve.ascended)
            add_glyph_buffer_layer_flags(x, y, LFLAGS_ASCENSION_RADIANCE);

        //clear_hero_memory_at(x, y);

        /* RECALL FIRST WHETHER THE LOCATION IS LIT OR NOT */
        /*
         * Don't use templit here:  E.g.
         *
         *      lev->waslit = !!(lev->lit || templit(x,y));
         *
         * Otherwise we have the "light pool" problem, where non-permanently
         * lit areas just out of sight stay remembered as lit.  They should
         * re-darken.
         *
         * Perhaps ALL areas should revert to their "unlit" look when
         * out of sight.
         */
        lev->waslit = (lev->lit != 0); /* remember lit condition */

        /* THEN, SHOW THE LOCATION IS AND PUT IT TO MEMORY */
        map_location(x, y, 1);

        /* Monster layer */
        if (x == u.ux && y == u.uy)
        {
            int see_self = canspotself();

#if 0
            /* update map information for <u.ux,u.uy> (remembered topology
               and object/known trap/terrain glyph) but only display it if
               hero can't see him/herself, then show self if appropriate */
            map_location(x, y, !see_self);
#endif
            boolean location_has_boulder = (sobj_at(BOULDER, x, y) != 0);
            unsigned long extra_flags = 0UL;
            if (carrying_leashed_leash())
            {
                extra_flags |= LFLAGS_U_TETHERED;
                struct monst* leashed_mon;
                for (leashed_mon = fmon; leashed_mon; leashed_mon = leashed_mon->nmon)
                {
                    if (leashed_mon->mleashed)
                    {
                        int mx = leashed_mon->mx, my = leashed_mon->my;
                        if (isok(mx, my))
                        {
                            show_leash_info(x, y, mx, my, u.ux, u.uy);
                        }
                    }
                }
            }
            if (see_self)
                display_self_with_extra_info_choose_ascii(disp_flags | extra_flags, hit_tile_id, damage_shown, location_has_boulder);
            else
            {
                show_extra_info(x, y, disp_flags | extra_flags, hit_tile_id, damage_shown);
                refresh_u_tile_gui_info(FALSE);
                gbuf[y][x].layers.monster_origin_x = u.ux0;
                gbuf[y][x].layers.monster_origin_y = u.uy0;
            }

            if (newsym_flags & NEWSYM_FLAGS_SHOW_DROPPING_PIERCER)
            {
                mon = m_at(x, y);
                worm_tail = is_worm_tail(mon);
                see_it = mon && (mon_visible(mon)
                    || (!worm_tail && (tp_sensemon(mon) || MATCH_WARN_OF_MON(mon)))
                    );
                if (mon && (see_it || (!worm_tail && Detect_monsters)))
                {
                    display_monster(x, y, mon,
                        see_it ? PHYSICALLY_SEEN : DETECTED,
                        worm_tail, TRUE);
                }
            }
        }
        else
        {
            mon = m_at(x, y);
            worm_tail = is_worm_tail(mon);
            see_it = mon && (mon_visible(mon)
                             || (!worm_tail && (tp_sensemon(mon) || MATCH_WARN_OF_MON(mon)))
                            );

            if (mon && (see_it || (!worm_tail && Detect_monsters)))
            {
                if (mon->mtrapped)
                {
                    struct trap* trap = t_at(x, y);
                    int tt = trap ? trap->ttyp : NO_TRAP;

                    /* if monster is in a physical trap, you see trap too */
                    if (tt == BEAR_TRAP || is_pit(tt) || tt == WEB)
                    {
                        trap->tseen = 1;
                        if (!covers_traps(x, y))
                            map_trap(trap, 1);
                    }
                }
                //_map_location(x, y, 0); /* map under the monster */
                /* also gets rid of any invisibility glyph */
                display_monster(x, y, mon,
                    see_it ? PHYSICALLY_SEEN : DETECTED,
                    worm_tail, FALSE);

                check_special_level_naming_by_mon(mon);

            }
            else if (mon && mon_warning(mon) && !is_worm_tail(mon))
            {
                display_warning(mon);
            }
            else if (old_glyph_is_invisible)
            {
                map_invisible(x, y);
            }
            else
            {
                /* Clear hero memory of any (invisible) monster from layer */
                if (level.flags.hero_memory)
                {
                    clear_monster_layer_memory_at(x, y);
                }
                clear_monster_layer_at(x, y);
            }
//            else
//                _map_location(x, y, 1); /* map the location */
        }

        /* Environment layer */
        NhRegion *reg = visible_region_at(x, y);
        /* normal region shown only on accessible positions, but poison clouds
         * also shown above lava, pools and moats.
         */
        if (reg && (ACCESSIBLE(lev->typ) || (reg->typ == REGION_POISON_GAS && is_pool_or_lava(x, y))))
        {
            show_region(reg, x, y);
            //return; /* Not needed anymore since last */
        }
    }
    else
    {
        /* Can't see the location. */
        if (x == u.ux && y == u.uy)
        {
            /* Feel location handles all layers up to object layer */
            feel_location(u.ux, u.uy); /* forces an update */

            /* Monster layer */
            if (canspotself())
            {
                display_self_with_extra_info_choose_ascii(disp_flags, hit_tile_id, damage_shown, FALSE);
            }
            else
            {
                gbuf[y][x].layers.monster_origin_x = u.ux0;
                gbuf[y][x].layers.monster_origin_y = u.uy0;
            }
        }
        else
        {
            /*** Show memory from floor to environment layer ***/
            /* Ascii */
            show_glyph_ascii(x, y, lev->hero_memory_layers.glyph);
            add_glyph_buffer_layer_flags(x, y, LFLAGS_SHOWING_MEMORY);

            /* Floor to environment, monster layer replaced below, if needed */
            enum layer_types layer_idx;
            for (layer_idx = LAYER_FLOOR; layer_idx <= LAYER_ENVIRONMENT; layer_idx++)
            {
                show_gui_glyph_on_layer(x, y, lev->hero_memory_layers.layer_glyphs[layer_idx], lev->hero_memory_layers.layer_gui_glyphs[layer_idx], layer_idx);
            }
            /* Add layer flags and object height from memory */
            add_glyph_buffer_layer_flags(x, y, lev->hero_memory_layers.layer_flags);
            set_glyph_buffer_feature_doodad_height(x, y, lev->hero_memory_layers.special_feature_doodad_layer_height);
            set_glyph_buffer_object_height(x, y, lev->hero_memory_layers.object_height);
            set_glyph_buffer_oid(x, y, lev->hero_memory_layers.o_id);

            /* Monster layer */
            mon = m_at(x, y);
            if (mon)
            {
                see_it = (tp_sensemon(mon) || MATCH_WARN_OF_MON(mon) || (see_with_infrared(mon) && mon_visible(mon)));
                if (see_it || Detect_monsters)
                {
                    /* Seen or sensed monsters are printed every time.
                       This also gets rid of any invisibility glyph. */
                    display_monster(x, y, mon, see_it ? 0 : DETECTED,
                        is_worm_tail(mon) ? TRUE : FALSE, FALSE);

                    check_special_level_naming_by_mon(mon);
                }
                else if (mon_warning(mon) && !is_worm_tail(mon))
                {
                    display_warning(mon);
                }
                else if (is_worm_tail(mon))
                {
                    add_glyph_buffer_layer_flags(x, y, LFLAGS_M_WORM_TAIL);
                }
            }
        }
    }

new_sym_end_here:

    if (x == u.ux && y == u.uy)
    {
        add_glyph_buffer_layer_flags(x, y, LFLAGS_UXUY); /* Mark player location */
    }
    
    if (newsym_flags & NEWSYM_FLAGS_KEEP_OLD_MISSILE_GLYPH)
    {
        show_gui_glyph_on_layer(x, y, missile_glyph, missile_gui_glyph, LAYER_MISSILE);
        show_missile_info(x, y, missile_poisoned, missile_material, missile_special_quality, missile_elemental_enchantment, missile_exceptionality, missile_mythic_prefix, missile_mythic_suffix, missile_eroded, missile_eroded2, missile_flags, missile_height, missile_origin_x, missile_origin_y);
    }
    if (newsym_flags & NEWSYM_FLAGS_KEEP_OLD_ZAP_GLYPH)
    {
        show_gui_glyph_on_layer(x, y, zap_glyph, zap_gui_glyph, LAYER_ZAP);
        add_glyph_buffer_layer_flags(x, y, zap_flags);
    }
    if (newsym_flags & NEWSYM_FLAGS_KEEP_OLD_GENERAL_EFFECT_GLYPH)
    {
        show_gui_glyph_on_layer(x, y, general_effect_glyph, general_effect_gui_glyph, LAYER_GENERAL_EFFECT);
    }
    if (newsym_flags & NEWSYM_FLAGS_KEEP_OLD_MONSTER_EFFECT_GLYPH)
    {
        show_gui_glyph_on_layer(x, y, monster_effect_glyph, monster_effect_gui_glyph, LAYER_MONSTER_EFFECT);
    }
    if (newsym_flags & NEWSYM_FLAGS_KEEP_OLD_BACKGROUND_EFFECT_GLYPH)
    {
        show_gui_glyph_on_layer(x, y, background_effect_glyph, background_effect_gui_glyph, LAYER_BACKGROUND_EFFECT);
    }
    if (newsym_flags & NEWSYM_FLAGS_KEEP_OLD_FLAGS)
    {
        add_glyph_buffer_layer_flags(x, y, old_flags);
    }

}

#undef is_worm_tail

/*
 * shieldeff()
 *
 * Put magic shield pyrotechnics at the given location.  This *could* be
 * pulled into a platform dependent routine for fancier graphics if desired.
 */
void
shieldeff(x, y)
xchar x, y;
{
    register int i;

    if (!flags.sparkle)
        return;
    if (cansee(x, y)) { /* Don't see anything if can't see the location */
        int rounds = flags.shield_effect_length > 0 && flags.shield_effect_length <= SHIELD_COUNT ? flags.shield_effect_length : GNH_DEF_SHIELD_COUNT;
        for (i = 0; i < rounds; i++)
        {
            show_glyph(x, y, cmap_to_glyph(shield_static[i]));
            flush_screen(1); /* make sure the glyph shows up */
            adjusted_delay_output();
        }
        newsym_with_flags(x, y, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS); /* restore the old information */
        show_glyph_on_layer(x, y, NO_GLYPH, LAYER_MONSTER_EFFECT);
    }
}

void
talkeff(x, y)
xchar x, y;
{
    register int i;

    if (!flags.sparkle)
        return;
    if (cansee(x, y)) { /* Don't see anything if can't see the location */
        boolean showmon = FALSE;
        int rounds = flags.talk_effect_length > 0 && flags.talk_effect_length <= TALK_COUNT ? flags.talk_effect_length : GNH_DEF_TALK_COUNT;
        for (i = 0; i < rounds; i++) {
            if (showmon)
                newsym(x, y); /* restore the old information */
            else
                show_glyph(x, y, cmap_to_glyph(S_talkeffect));
            flush_screen(1); /* make sure the glyph shows up */
            adjusted_delay_output();
            adjusted_delay_output();
            showmon = !showmon;
        }
        newsym(x, y); /* restore the old information */
    }
}


int
tether_glyph(x, y)
int x, y;
{
    int tdx, tdy;
    tdx = u.ux - x;
    tdy = u.uy - y;
    return zapdir_to_glyph(sgn(tdx),sgn(tdy), 2);
}

/*
 * tmp_at()
 *
 * Temporarily place glyphs on the screen.  Do not call delay_output().  It
 * is up to the caller to decide if it wants to wait [presently, everyone
 * but explode() wants to delay].
 *
 * Call:
 *      (DISP_BEAM,    glyph)   open, initialize glyph
 *      (DISP_FLASH,   glyph)   open, initialize glyph
 *      (DISP_ALWAYS,  glyph)   open, initialize glyph
 *      (DISP_CHANGE,  glyph)   change glyph
 *      (DISP_END,     0)       close & clean up (2nd argument doesn't matter)
 *      (DISP_FREEMEM, 0)       only used to prevent memory leak during exit)
 *      (x, y)                  display the glyph at the location
 *
 * DISP_BEAM   - Display the given glyph at each location, but do not erase
 *               any until the close call.
 * DISP_TETHER - Display a tether glyph at each location, and the tethered
 *               object at the farthest location, but do not erase any
 *               until the return trip or close.
 * DISP_FLASH  - Display the given glyph at each location, but erase the
 *               previous location's glyph.
 * DISP_ALWAYS - Like DISP_FLASH, but vision is not taken into account.
 */

#define TMP_AT_MAX_GLYPHS (COLNO * 2)

STATIC_VAR struct tmp_glyph {
    coord saved[TMP_AT_MAX_GLYPHS]; /* previously updated positions */
    int sidx;                       /* index of next unused slot in saved[] */
    int style; /* either DISP_BEAM or DISP_FLASH or DISP_ALWAYS */
    int glyph; /* glyph to use when printing */
    int gui_glyph; /* gui glyph to use when printing */
    int obj_tile_height;
    struct tmp_glyph *prev;
} tgfirst;

void
tmp_at(x, y)
int x, y;
{
    tmp_at_with_obj(x, y, (struct obj*)0, 0UL, MAT_NONE, 0);
}

void
tmp_at_with_missile_flags(x, y, missile_flags, missile_material, missile_special_quality)
int x, y;
unsigned long missile_flags;
uchar missile_material;
short missile_special_quality;
{
    tmp_at_with_obj(x, y, (struct obj*)0, missile_flags, missile_material, missile_special_quality);
}

void
tmp_at_with_obj(x, y, obj, missile_flags, missile_material, missile_special_quality)
int x, y;
struct obj* obj;
unsigned long missile_flags;
uchar missile_material;
short missile_special_quality;
{
    static struct tmp_glyph *tglyph = (struct tmp_glyph *) 0;
    struct tmp_glyph *tmp;

    switch (x) {
    case DISP_BEAM:
    case DISP_BEAM_DIG:
    case DISP_ALL:
    case DISP_TETHER:
    case DISP_FLASH:
    case DISP_ALWAYS:
        if (!tglyph)
            tmp = &tgfirst;
        else /* nested effect; we need dynamic memory */
            tmp = (struct tmp_glyph *) alloc(sizeof *tmp);
        tmp->prev = tglyph;
        tglyph = tmp;
        tglyph->sidx = 0;
        tglyph->style = x;
        tglyph->glyph = y;
        tglyph->gui_glyph = maybe_get_replaced_glyph(y, 0, 0, data_to_replacement_info(y, LAYER_MISSILE, obj, (struct monst*)0, 0UL, missile_flags, missile_material, missile_special_quality));
        context.tether_x = 0;
        context.tether_y = 0;
        flush_screen(1); /* flush buffered glyphs */
        return;

    case DISP_FREEMEM: /* in case game ends with tmp_at() in progress */
        while (tglyph) {
            tmp = tglyph->prev;
            if (tglyph != &tgfirst)
                free((genericptr_t) tglyph);
            tglyph = tmp;
        }
        context.tether_x = 0;
        context.tether_y = 0;
        return;

    default:
        break;
    }

    if (!tglyph)
    {
        panic("tmp_at: tglyph not initialized");
        return;
    }

    switch (x) {
    case DISP_CHANGE:
        tglyph->glyph = y;
        tglyph->gui_glyph = maybe_get_replaced_glyph(y, 0, 0, data_to_replacement_info(y, LAYER_MISSILE, obj, (struct monst*)0, 0UL, missile_flags, missile_material, missile_special_quality));
        break;

    case DISP_END:
        if (tglyph->style == DISP_BEAM || tglyph->style == DISP_BEAM_DIG || tglyph->style == DISP_ALL) {
            register int i;

            /* Erase (reset) from source to end */
            for (i = 0; i < tglyph->sidx; i++)
                newsym(tglyph->saved[i].x, tglyph->saved[i].y);
        } else if (tglyph->style == DISP_TETHER) {
            int i;

            if (y == BACKTRACK && tglyph->sidx > 1) {
                /* backtrack */
                for (i = tglyph->sidx - 1; i > 0; i--) {
                    context.tether_x = tglyph->saved[i - 1].x;
                    context.tether_y = tglyph->saved[i - 1].y;
                    newsym(tglyph->saved[i].x, tglyph->saved[i].y);
                    show_gui_glyph_on_layer_and_ascii(tglyph->saved[i - 1].x,
                               tglyph->saved[i - 1].y, tglyph->glyph, tglyph->gui_glyph, LAYER_MISSILE);
                    if(obj)
                        show_missile_info(tglyph->saved[i - 1].x, tglyph->saved[i - 1].y, obj->opoisoned, obj->material, obj->special_quality, obj->elemental_enchantment, obj->exceptionality, obj->mythic_prefix, obj->mythic_suffix, obj->oeroded, obj->oeroded2, get_missile_flags(obj, TRUE), get_obj_height(obj), 0, 0);

                    flush_screen(1);   /* make sure it shows up */
                    adjusted_delay_output();

                    if (obj)
                        show_missile_info(tglyph->saved[i - 1].x, tglyph->saved[i - 1].y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0UL, 0, 0, 0);
                }
                tglyph->sidx = 1;
            }
            context.tether_x = 0;
            context.tether_y = 0;
            for (i = 0; i < tglyph->sidx; i++)
                newsym(tglyph->saved[i].x, tglyph->saved[i].y);
        } else {              /* DISP_FLASH or DISP_ALWAYS */
            if (tglyph->sidx) /* been called at least once */
                newsym(tglyph->saved[0].x, tglyph->saved[0].y);
        }
        /* tglyph->sidx = 0; -- about to be freed, so not necessary */
        tmp = tglyph->prev;
        if (tglyph != &tgfirst)
            free((genericptr_t) tglyph);
        tglyph = tmp;
        break;

    default: /* do it */
        if (!isok(x, y))
            break;
        if (tglyph->style == DISP_BEAM || tglyph->style == DISP_BEAM_DIG || tglyph->style == DISP_ALL) {
            if (tglyph->style != DISP_ALL && !cansee(x, y))
                break;
            if (tglyph->sidx >= TMP_AT_MAX_GLYPHS)
                break; /* too many locations */
            /* save pos for later erasing */
            tglyph->saved[tglyph->sidx].x = x;
            tglyph->saved[tglyph->sidx].y = y;
            tglyph->sidx += 1;
        } else if (tglyph->style == DISP_TETHER) {
            if (tglyph->sidx >= TMP_AT_MAX_GLYPHS)
                break; /* too many locations */
            if (tglyph->sidx) {
                int px, py;

                px = tglyph->saved[tglyph->sidx-1].x;
                py = tglyph->saved[tglyph->sidx-1].y;
                show_glyph_on_layer_and_ascii(px, py, tether_glyph(px, py), LAYER_MISSILE);
                if (obj)
                    show_missile_info(px, py, obj->opoisoned, obj->material, obj->special_quality, obj->elemental_enchantment, obj->exceptionality, obj->mythic_prefix, obj->mythic_suffix, obj->oeroded, obj->oeroded2, get_missile_flags(obj, TRUE), get_obj_height(obj), 0, 0);

            }
            /* save pos for later use or erasure */
            context.tether_x = x;
            context.tether_y = y;
            tglyph->saved[tglyph->sidx].x = x;
            tglyph->saved[tglyph->sidx].y = y;
            tglyph->sidx += 1;
        } else {                /* DISP_FLASH/ALWAYS */
            if (tglyph->sidx) { /* not first call, so reset previous pos */
                newsym(tglyph->saved[0].x, tglyph->saved[0].y);
                force_redraw_at(tglyph->saved[0].x, tglyph->saved[0].y);
                tglyph->sidx = 0; /* display is presently up to date */
            }
            if (!cansee(x, y) && tglyph->style != DISP_ALWAYS)
            {
                flush_screen(1);                 /* make sure it shows up */
                break;
            }
            tglyph->saved[0].x = x;
            tglyph->saved[0].y = y;
            tglyph->sidx = 1;
        }

        show_gui_glyph_on_layer_and_ascii(x, y, tglyph->glyph, tglyph->gui_glyph,
            tglyph->style == DISP_BEAM || tglyph->style == DISP_BEAM_DIG || tglyph->style == DISP_ALL ? LAYER_ZAP : LAYER_MISSILE); /* show it */

        if (obj)
            show_missile_info(x, y, obj->opoisoned, obj->material, obj->special_quality, obj->elemental_enchantment, obj->exceptionality, obj->mythic_prefix, obj->mythic_suffix, obj->oeroded, obj->oeroded2, get_missile_flags(obj, TRUE), get_obj_height(obj), 0, 0);

        flush_screen(1);                 /* make sure it shows up */
        break;
    } /* end case */
}

short
get_obj_height(obj)
struct obj* obj;
{
    if (!obj)
        return 0;

    int baseheight = obj->oartifact ? artilist[obj->oartifact].tile_floor_height : OBJ_TILE_HEIGHT(obj->otyp);
    int height = baseheight;
    /* Special variable height for globs */
    if (obj->globby)
    {
        if (obj->owt <= GLOB_SMALL_MAXIMUM_WEIGHT)
            height = baseheight / 2;
        else if(obj->owt <= GLOB_MEDIUM_MAXIMUM_WEIGHT)
            height = (2 * baseheight) / 3;
        else if (obj->owt <= GLOB_LARGE_MAXIMUM_WEIGHT)
            height = (5 * baseheight) / 6;
    }

    return height;
}

boolean
generic_has_floor_tile(obj)
struct obj* obj;
{
    return !obj ? FALSE : obj->oartifact ? has_artifact_floor_tile(obj->oartifact) : has_obj_floor_tile(obj);
}

/*
 * swallowed()
 *
 * The hero is swallowed.  Show a special graphics sequence for this.  This
 * bypasses all of the display routines and messes with buffered screen
 * directly.  This method works because both vision and display check for
 * being swallowed.
 */
void
swallowed(first)
int first;
{
    if (!u.ustuck)
        return;

    int swallower, left_ok, rght_ok;

    if (first)
    {
        cls();
        bot();
    }
    else
    {
        register int x, y;

        /* Clear old location */
        for (y = lastswy - 1; y <= lastswy + 1; y++)
            for (x = lastswx - 1; x <= lastswx + 1; x++)
                if (isok(x, y))
                    clear_glyph_buffer_at(x, y);
    }

    swallower = u.ustuck->mnum;
    /* assume isok(u.ux,u.uy) */
    left_ok = isok(u.ux - 1, u.uy);
    rght_ok = isok(u.ux + 1, u.uy);
    /*
     *  Display the hero surrounded by the monster's stomach.
     */
    if (isok(u.ux, u.uy - 1)) 
    {
        if (left_ok)
            show_glyph_on_layer_and_ascii(u.ux - 1, u.uy - 1,
                       swallow_to_glyph(swallower, S_sw_tl), LAYER_FEATURE);
        show_glyph_on_layer_and_ascii(u.ux, u.uy - 1, swallow_to_glyph(swallower, S_sw_tc), LAYER_FEATURE);
        if (rght_ok)
            show_glyph_on_layer_and_ascii(u.ux + 1, u.uy - 1,
                       swallow_to_glyph(swallower, S_sw_tr), LAYER_FEATURE);
    }

    if (left_ok)
        show_glyph_on_layer_and_ascii(u.ux - 1, u.uy, swallow_to_glyph(swallower, S_sw_ml), LAYER_FEATURE);

    show_glyph_on_layer(u.ux, u.uy, swallow_to_glyph(swallower, S_sw_mc), LAYER_FEATURE);
    display_self();
    if (rght_ok)
        show_glyph_on_layer_and_ascii(u.ux + 1, u.uy, swallow_to_glyph(swallower, S_sw_mr), LAYER_FEATURE);

    if (isok(u.ux, u.uy + 1)) {
        if (left_ok)
            show_glyph_on_layer_and_ascii(u.ux - 1, u.uy + 1,
                       swallow_to_glyph(swallower, S_sw_bl), LAYER_FEATURE);
        show_glyph_on_layer_and_ascii(u.ux, u.uy + 1, swallow_to_glyph(swallower, S_sw_bc), LAYER_FEATURE);
        if (rght_ok)
            show_glyph_on_layer_and_ascii(u.ux + 1, u.uy + 1,
                       swallow_to_glyph(swallower, S_sw_br), LAYER_FEATURE);
    }

    /* Update the swallowed position. */
    lastswx = u.ux;
    lastswy = u.uy;

    play_environment_ambient_sounds();
}

void
clear_glyph_buffer_at(x, y)
int x, y;
{
    if (isok(x, y))
    {
        gbuf[y][x] = nul_gbuf;
        gbuf[y][x].isnew = 1;
        if (gbuf_start[y] > x)
            gbuf_start[y] = x;
        if (gbuf_stop[y] < x)
            gbuf_stop[y] = x;
    }
}

void
reset_display(VOID_ARGS)
{
    dela = delagr = FALSE;
    lastx = lasty = 0;
    lastswx = lastswy = 0;
    memset((genericptr_t)gbuf, 0 , sizeof(gbuf));
    memset((genericptr_t)gbuf_start, 0, sizeof(gbuf_start));
    memset((genericptr_t)gbuf_stop, 0, sizeof(gbuf_stop));
    in_cls = 0;
    flushing = 0;
    delay_flushing = 0;
}

/*
 * under_water()
 *
 * Similar to swallowed() in operation.  Shows hero when underwater
 * except when in water level.  Special routines exist for that.
 */
void
under_water(mode)
int mode;
{
    register int x, y;

    /* swallowing has a higher precedence than under water */
    if (Is_waterlevel(&u.uz) || u.uswallow)
        return;

    /* full update */
    if (mode == 1 || dela) {
        cls();
        dela = FALSE;

    /* delayed full update */
    } else if (mode == 2) {
        dela = TRUE;
        return;

    /* limited update */
    } else {
        for (y = lasty - 1; y <= lasty + 1; y++)
            for (x = lastx - 1; x <= lastx + 1; x++)
                if (isok(x, y))
                    clear_glyph_buffer_at(x, y);
    }

    /*
     * TODO?  Should this honor Xray radius rather than force radius 1?
     */

    for (x = u.ux - 1; x <= u.ux + 1; x++)
        for (y = u.uy - 1; y <= u.uy + 1; y++)
            if (isok(x, y) && (is_pool_or_lava(x, y) || is_ice(x, y))) {
                if (Blind && !(x == u.ux && y == u.uy))
                    clear_glyph_buffer_at(x, y);
                else
                    newsym(x, y);
            }
    lastx = u.ux;
    lasty = u.uy;

    play_environment_ambient_sounds();
}

/*
 *      under_ground()
 *
 *      Very restricted display.  You can only see yourself.
 */
void
under_ground(mode)
int mode;
{
    /* swallowing has a higher precedence than under ground */
    if (u.uswallow)
        return;

    /* full update */
    if (mode == 1 || delagr) {
        cls();
        delagr = FALSE;

    /* delayed full update */
    } else if (mode == 2) {
        delagr = TRUE;
        return;

    /* limited update */
    } else {
        newsym(u.ux, u.uy);
    }
}

/* ======================================================================== */

/*
 * Loop through all of the monsters and update them.  Called when:
 *      + going blind & telepathic
 *      + regaining sight & telepathic
 *      + getting and losing infravision
 *      + hallucinating
 *      + doing a full screen redraw
 *      + see invisible times out or a ring of see invisible is taken off
 *      + when a potion of see invisible is quaffed or a ring of see
 *        invisible is put on
 *      + gaining telepathy when blind [givit() in eat.c, pleased() in pray.c]
 *      + losing telepathy while blind [xkilled() in mon.c, attrcurse() in
 *        sit.c]
 */
void
see_monsters()
{
    register struct monst *mon;
    int new_warn_obj_cnt = 0;
    int new_demon_warn_obj_cnt = 0;
    int new_undead_warn_obj_cnt = 0;
    int new_orc_warn_obj_cnt = 0;
    int new_troll_warn_obj_cnt = 0;
    int new_giant_warn_obj_cnt = 0;
    int new_dragon_warn_obj_cnt = 0;
    int new_elf_warn_obj_cnt = 0;
    int new_dwarf_warn_obj_cnt = 0;
    int new_gnoll_warn_obj_cnt = 0;
    int new_gnome_warn_obj_cnt = 0;
    int new_ogre_warn_obj_cnt = 0;
    int new_human_warn_obj_cnt = 0;
    int new_were_warn_obj_cnt = 0;
    int new_angel_warn_obj_cnt = 0;

    if (defer_see_monsters)
        return;

    for (mon = fmon; mon; mon = mon->nmon) {
        if (DEADMONSTER(mon))
            continue;
        newsym(mon->mx, mon->my);
        if (mon->wormno)
            see_wsegs(mon);

        /* Note: The warning weapons flicker even for peaceful monsters */
        if (Warn_of_mon && (context.warntype.obj & mon->data->mflags2) != 0L)
            new_warn_obj_cnt++;
        if (Demon_warning && (mon->data->mflags2 & M2_DEMON) != 0L)
            new_demon_warn_obj_cnt++;
        if (Undead_warning && (mon->data->mflags2 & M2_UNDEAD) != 0L)
            new_undead_warn_obj_cnt++;
        if (Orc_warning && (mon->data->mflags2 & M2_ORC) != 0L)
            new_orc_warn_obj_cnt++;
        if (Troll_warning && (mon->data->mlet == S_TROLL))
            new_troll_warn_obj_cnt++;
        if (Giant_warning && (mon->data->mflags2 & M2_GIANT) != 0L)
            new_giant_warn_obj_cnt++;
        if (Dragon_warning && (mon->data->mlet == S_DRAGON))
            new_dragon_warn_obj_cnt++;
        if (Elf_warning && (mon->data->mflags2 & M2_ELF) != 0L)
            new_elf_warn_obj_cnt++;
        if (Dwarf_warning && (mon->data->mflags2 & M2_DWARF) != 0L)
            new_dwarf_warn_obj_cnt++;
        if (Gnoll_warning && (mon->data->mflags2 & M2_GNOLL) != 0L)
            new_gnoll_warn_obj_cnt++;
        if (Gnome_warning && (mon->data->mflags2 & M2_GNOME) != 0L)
            new_gnome_warn_obj_cnt++;
        if (Ogre_warning && (mon->data->mlet == S_OGRE))
            new_ogre_warn_obj_cnt++;
        if (Human_warning && (mon->data->mflags2 & M2_HUMAN) != 0L)
            new_human_warn_obj_cnt++;
        if (Lycanthrope_warning && (mon->data->mflags2 & M2_WERE) != 0L)
            new_were_warn_obj_cnt++;
        if (Angel_warning && (mon->data->mflags2 & M2_ANGEL) != 0L)
            new_angel_warn_obj_cnt++;
    }


    /*
     * Make Sting and other similar objects glow blue or stop glowing if required.
     */
    struct obj* uitem;
    for (uitem = invent; uitem; uitem = uitem->nobj)
    {
        if (!object_uses_spellbook_wand_flags_and_properties(uitem))
        {
            int otyp = uitem->otyp;
            boolean inappr = inappropriate_character_type(uitem);
            boolean worn = is_obj_worn(uitem);
            int old_count = uitem->detectioncount;
            int new_count = 0;
            boolean flicker_ability = FALSE;

            if (((worn|| (!worn && (objects[otyp].oc_flags & O1_OFLAG_POWERS_APPLY_WHEN_CARRIED)))
                && ((!inappr && !(objects[otyp].oc_flags & (O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    || (objects[otyp].oc_flags & O1_OFLAG_POWERS_APPLY_TO_ALL_CHARACTERS)
                    || (inappr && (objects[otyp].oc_flags & (O1_OFLAG_POWERS_APPLY_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
                    ))
                || (!worn && uitem->oartifact && artifact_has_flag(uitem, AF_FLICKERS_WHEN_CARRIED))
                )
            {
                if (((objects[otyp].oc_flags & O1_FLICKERS_WHEN_MONSTERS_DETECTED) || (uitem->oartifact && (artifact_has_flag(uitem, AF_FLICKERS) || artifact_has_flag(uitem, AF_FLICKERS_WHEN_CARRIED))) )
                    && item_has_specific_monster_warning(uitem))
                {
                    flicker_ability = TRUE;

                    if (item_is_giving_power(uitem, WARN_OF_MON))
                        new_count += new_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_ORC))
                        new_count += new_orc_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_DEMON))
                        new_count += new_demon_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_UNDEAD))
                        new_count += new_undead_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_TROLL))
                        new_count += new_troll_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_GIANT))
                        new_count += new_giant_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_DRAGON))
                        new_count += new_dragon_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_ELF))
                        new_count += new_elf_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_DWARF))
                        new_count += new_dwarf_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_GNOLL))
                        new_count += new_gnoll_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_GNOME))
                        new_count += new_gnome_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_OGRE))
                        new_count += new_ogre_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_HUMAN))
                        new_count += new_human_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_LYCANTHROPE))
                        new_count += new_were_warn_obj_cnt;

                    if (item_is_giving_power(uitem, WARN_ANGEL))
                        new_count += new_angel_warn_obj_cnt;
                }
            }
            if (flicker_ability)
            {
                if (new_count != old_count)
                    Sting_effects(uitem, new_count);

                uitem->detectioncount = new_count;
            }
        }
    }



    /* when mounted, hero's location gets caught by monster loop */
    if (!u.usteed)
        newsym(u.ux, u.uy);
}

/*
 * Block/unblock light depending on what a mimic is mimicing and if it's
 * invisible or not.  Should be called only when the state of See_invisible
 * changes.
 */
void
set_mimic_blocking()
{
    register struct monst *mon;

    for (mon = fmon; mon; mon = mon->nmon) {
        if (DEADMONSTER(mon))
            continue;
        if (is_invisible(mon) && is_lightblocker_mappear(mon)) {
            if (See_invisible)
                block_point(mon->mx, mon->my);
            else
                unblock_point(mon->mx, mon->my);
        }
    }
    update_hearing_array_and_ambient_sounds();
}

/*
 * Loop through all of the object *locations* and update them.  Called when
 *      + hallucinating.
 */
void
see_objects()
{
    register struct obj *obj;
    for (obj = fobj; obj; obj = obj->nobj)
        if (vobj_at(obj->ox, obj->oy) == obj)
            newsym(obj->ox, obj->oy);
}

/*
 * Update hallucinated traps.
 */
void
see_traps()
{
    struct trap *trap;
    int glyph;

    for (trap = ftrap; trap; trap = trap->ntrap) {
        glyph = glyph_at(trap->tx, trap->ty);
        if (glyph_is_trap(glyph))
            newsym(trap->tx, trap->ty);
    }
}

/*
 * Put the cursor on the hero.  Flush all accumulated glyphs before doing it.
 */
void
curs_on_u()
{
    flush_screen(1); /* Flush waiting glyphs & put cursor on hero */
}

int
doredraw()
{
    docrt();
    return 0;
}

void
docrt()
{
    register int x, y;
    register struct rm *lev;

    if (!u.ux)
        return; /* display isn't ready yet */

    if (u.uswallow) {
        swallowed(1);
        return;
    }
    if (Underwater && !Is_waterlevel(&u.uz)) {
        under_water(1);
        return;
    }
    if (u.uburied) {
        under_ground(1);
        return;
    }

    /* shut down vision */
    vision_recalc(2);

    /* clear detection markers from memory objects */
    clear_memory_object_detection_marks();

    /*
     * This routine assumes that cls() does the following:
     *      + fills the physical screen with the symbol for rock
     *      + clears the glyph buffer
     */
    cls();

    /* display memory */
    for (x = 1; x < COLNO; x++) {
        lev = &levl[x][0];
        for (y = 0; y < ROWNO; y++, lev++)
            if (lev->hero_memory_layers.glyph != cmap_to_glyph(S_unexplored))
            {
                show_glyph_ascii(x, y, lev->hero_memory_layers.glyph);
                add_glyph_buffer_layer_flags(x, y, LFLAGS_SHOWING_MEMORY);
                add_glyph_buffer_layer_flags(x, y, lev->hero_memory_layers.layer_flags);
                enum layer_types layer_idx;
                for (layer_idx = LAYER_FLOOR; layer_idx <= LAYER_GENERAL_UI; layer_idx++)
                    show_gui_glyph_on_layer(x, y, lev->hero_memory_layers.layer_glyphs[layer_idx], lev->hero_memory_layers.layer_gui_glyphs[layer_idx], layer_idx);
            }
    }

    /* see what is to be seen */
    vision_recalc(0);

    /* overlay with monsters */
    see_monsters();

    /* perm_invent */
    update_inventory();

    context.botlx = 1; /* force a redraw of the bottom line */
}

void
clear_memory_object_detection_marks()
{
    struct obj* otmp;
    for (otmp = memoryobjs; otmp; otmp = otmp->nobj)
    {
        otmp->speflags &= ~SPEFLAGS_DETECTED;
    }
}


/* for panning beyond a clipped region; resend the current map data to
   the interface rather than use docrt()'s regeneration of that data */
void
redraw_map()
{
    int x, y;

    /*
     * Not sure whether this is actually necessary; save and restore did
     * used to get much too involved with each dungeon level as it was
     * read and written.
     *
     * !u.ux: display isn't ready yet; (restoring || !on_level()): was part
     * of cliparound() but interface shouldn't access this much internals
     */
    if (!u.ux || restoring || saving || !on_level(&u.uz0, &u.uz))
        return;

    /*
     * This yields sensible clipping when #terrain+getpos is in
     * progress and the screen displays something other than what
     * the map would currently be showing.
     */
    struct layer_info layers;
    issue_gui_command(GUI_CMD_START_FLUSH);
    for (y = 0; y < ROWNO; ++y)
        for (x = 1; x < COLNO; ++x) {
            layers = layers_at(x, y); /* not levl[x][y].hero_memory_layers.glyph */
            print_glyph(WIN_MAP, x, y, layers);
        }
    issue_gui_command(GUI_CMD_FINISH_FLUSH);
    flush_screen(1);
}

/* FIXME: This is a dirty hack, because newsym() doesn't distinguish
 * between object piles and single objects, it doesn't mark the location
 * for update. */
void
newsym_force(x, y)
register int x, y;
{
    newsym(x, y);
    force_redraw_at(x, y);
}

void
force_redraw_at(x, y)
register int x, y;
{
    gbuf[y][x].isnew = 1;
    if (gbuf_start[y] > x)
        gbuf_start[y] = x;
    if (gbuf_stop[y] < x)
        gbuf_stop[y] = x;
}

/*
 * Store the glyph in the 3rd screen for later flushing.
 */
void
show_layer_glyphs(x, y, layers)
int x, y;
struct layer_info layers;
{
    if (isok(x, y))
    {
        show_glyph_ascii(x, y, layers.glyph);

        boolean layer_glyph_different = FALSE;
        enum layer_types layer_idx;
        for (layer_idx = LAYER_FLOOR; layer_idx < MAX_LAYERS; layer_idx++)
        {
            int layer_glyph_before = gbuf[y][x].layers.layer_glyphs[layer_idx];
            int layer_gui_glyph_before = gbuf[y][x].layers.layer_gui_glyphs[layer_idx];
            gbuf[y][x].layers.layer_glyphs[layer_idx] = layers.layer_glyphs[layer_idx];
            gbuf[y][x].layers.layer_gui_glyphs[layer_idx] = layers.layer_gui_glyphs[layer_idx];
            if (layer_glyph_before != layers.layer_glyphs[layer_idx] || layer_gui_glyph_before != layers.layer_gui_glyphs[layer_idx])
                layer_glyph_different = TRUE;
        }

        if (layer_glyph_different)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}


void
show_glyph(x, y, glyph)
int x, y, glyph;
{
    maybe_clear_and_show_glyph(x, y, glyph, FALSE);
}

void
clear_current_and_show_glyph(x, y, glyph)
int x, y, glyph;
{
    maybe_clear_and_show_glyph(x, y, glyph, TRUE);
}

void
maybe_clear_and_show_glyph(x, y, glyph, clear)
int x, y, glyph;
boolean clear;
{
    if (isok(x, y))
    {
        if(clear)
            remove_current_glyph_from_layer(x, y);
        show_glyph_ascii(x, y, glyph);
        add_glyph_to_layer(x, y, glyph);
    }
}

void
show_glyph_on_layer_and_ascii(x, y, glyph, layer_idx)
int x, y, glyph;
enum layer_types layer_idx;
{
    show_glyph_on_layer(x, y, glyph, layer_idx);
    show_glyph_ascii(x, y, glyph);

}

void
show_gui_glyph_on_layer_and_ascii(x, y, glyph, gui_glyph, layer_idx)
int x, y, gui_glyph, glyph;
enum layer_types layer_idx;
{
    show_gui_glyph_on_layer(x, y, glyph, gui_glyph, layer_idx);
    show_glyph_ascii(x, y, glyph);

}

void
show_glyph_on_layer(x, y, glyph, layer_idx)
int x, y, glyph;
enum layer_types layer_idx;
{
    show_gui_glyph_on_layer(x, y, glyph, glyph, layer_idx);
}

void
show_gui_glyph_on_layer(x, y, glyph, gui_glyph, layer_idx)
int x, y, glyph, gui_glyph;
enum layer_types layer_idx;
{
    if (isok(x, y) && layer_idx < MAX_LAYERS)
    {
        int layer_glyph_before = gbuf[y][x].layers.layer_glyphs[layer_idx];
        int layer_gui_glyph_before = gbuf[y][x].layers.layer_gui_glyphs[layer_idx];

        gbuf[y][x].layers.layer_glyphs[layer_idx] = glyph;
        gbuf[y][x].layers.layer_gui_glyphs[layer_idx] = gui_glyph;

        if (layer_glyph_before != glyph || layer_gui_glyph_before != gui_glyph)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}

void
show_leash_info(x, y, mx, my, ux, uy)
int x, y;
xchar mx, my;
xchar ux, uy;
{
    if (isok(x, y) && isok(ux, uy))
    {
        boolean different = FALSE;

        if (isok(ux, uy))
        {
            if (gbuf[y][x].layers.leash_mon_x[MAXLEASHED] != ux)
                different = TRUE;
            if (gbuf[y][x].layers.leash_mon_y[MAXLEASHED] != uy)
                different = TRUE;
            gbuf[y][x].layers.leash_mon_x[MAXLEASHED] = ux;
            gbuf[y][x].layers.leash_mon_y[MAXLEASHED] = uy;
        }

        if (isok(mx, my))
        {
            int i;
            for (i = 0; i < MAXLEASHED; i++)
            {
                if (gbuf[y][x].layers.leash_mon_x[i] == 0 && gbuf[y][x].layers.leash_mon_y[i] == 0)
                {
                    different = TRUE;
                    gbuf[y][x].layers.leash_mon_x[i] = mx;
                    gbuf[y][x].layers.leash_mon_y[i] = my;
                }
            }
        }

        if (different)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}

void
show_extra_info(x, y, disp_flags, hit_tile_id, damage_displayed)
int x, y;
unsigned long disp_flags;
int hit_tile_id, damage_displayed;
{
    if (isok(x, y))
    {
        change_layer_hit_tile(x, y, hit_tile_id);
        change_layer_damage_displayed(x, y, damage_displayed);

        unsigned long old_flags = gbuf[y][x].layers.layer_flags;
        gbuf[y][x].layers.layer_flags |= disp_flags;

        if (old_flags != gbuf[y][x].layers.layer_flags)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}

void
show_missile_info(x, y, poisoned, material, special_quality, elemental_enchantment, exceptionality, mythic_prefix, mythic_suffix, eroded, eroded2, missile_flags, missile_height, missile_origin_x, missile_origin_y)
int x, y;
uchar poisoned, material, elemental_enchantment, exceptionality, mythic_prefix, mythic_suffix, eroded, eroded2;
unsigned long missile_flags;
short missile_height, special_quality;
xchar missile_origin_x, missile_origin_y;
{
    if (isok(x, y))
    {
        gbuf[y][x].layers.missile_poisoned = poisoned;
        gbuf[y][x].layers.missile_material = material;
        gbuf[y][x].layers.missile_special_quality = special_quality;
        gbuf[y][x].layers.missile_elemental_enchantment = elemental_enchantment;
        gbuf[y][x].layers.missile_exceptionality = exceptionality;
        gbuf[y][x].layers.missile_mythic_prefix = mythic_prefix;
        gbuf[y][x].layers.missile_mythic_suffix = mythic_suffix;
        gbuf[y][x].layers.missile_eroded = eroded;
        gbuf[y][x].layers.missile_eroded2 = eroded2;
        gbuf[y][x].layers.missile_flags = missile_flags;
        gbuf[y][x].layers.missile_height = missile_height;
        gbuf[y][x].layers.missile_origin_x = missile_origin_x;
        gbuf[y][x].layers.missile_origin_y = missile_origin_y;

        gbuf[y][x].isnew = 1;
        if (gbuf_start[y] > x)
            gbuf_start[y] = x;
        if (gbuf_stop[y] < x)
            gbuf_stop[y] = x;
    }
}

unsigned long
get_missile_flags(obj, tethered_weapon)
struct obj* obj;
boolean tethered_weapon;
{
    if (!obj)
        return 0UL;

    unsigned long res = 0UL;
    if (is_corrodeable(obj))
        res |= MISSILE_FLAGS_CORRODEABLE;
    if (is_rottable(obj))
        res |= MISSILE_FLAGS_ROTTABLE;
    if (is_flammable(obj))
        res |= MISSILE_FLAGS_FLAMMABLE;
    if (is_rustprone(obj))
        res |= MISSILE_FLAGS_RUSTPRONE;
    if (is_poisonable(obj))
        res |= MISSILE_FLAGS_POISONABLE;
    if (obj->oerodeproof)
        res |= MISSILE_FLAGS_ERODEPROOF;
    if (tethered_weapon)
        res |= MISSILE_FLAGS_TETHERED;
    if (is_obj_activated(obj))
        res |= MISSILE_FLAGS_LIT;

    return res;
}

void
change_layer_damage_displayed(x, y, damage_displayed)
int x, y;
int damage_displayed;
{
    if (isok(x, y))
    {
        short old_dmg = gbuf[y][x].layers.damage_displayed;
        if (damage_displayed > 0)
            gbuf[y][x].layers.damage_displayed = (short)(Hallucination ? rnd(2 * damage_displayed) : damage_displayed);
        else
            gbuf[y][x].layers.damage_displayed = 0;

        if (old_dmg != gbuf[y][x].layers.damage_displayed)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}

void
change_layer_hit_tile(x, y, hit_tile_id)
int x, y;
int hit_tile_id;
{
    if (isok(x, y))
    {
        short old_hit_tile = gbuf[y][x].layers.hit_tile;
        gbuf[y][x].layers.hit_tile = hit_tile_id;

        if (old_hit_tile != gbuf[y][x].layers.hit_tile)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}

void
add_glyph_buffer_layer_flags(x, y, added_flags)
int x, y;
unsigned long added_flags;
{
    if (isok(x, y))
    {
        unsigned long old_flags = gbuf[y][x].layers.layer_flags;
        gbuf[y][x].layers.layer_flags |= added_flags;

        if (old_flags != gbuf[y][x].layers.layer_flags)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}

void
remove_glyph_buffer_layer_flags(x, y, removed_flags)
int x, y;
unsigned long removed_flags;
{
    if (isok(x, y))
    {
        unsigned long old_flags = gbuf[y][x].layers.layer_flags;
        gbuf[y][x].layers.layer_flags &= ~removed_flags;

        if (old_flags != gbuf[y][x].layers.layer_flags)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}

void
set_glyph_buffer_object_height(x, y, height)
int x, y;
short height;
{
    if (isok(x, y))
    {
        gbuf[y][x].layers.object_height = height;
    }
}

void
set_glyph_buffer_feature_doodad_height(x, y, height)
int x, y;
schar height;
{
    if (isok(x, y))
    {
        gbuf[y][x].layers.special_feature_doodad_layer_height = height;
    }
}

void
set_glyph_buffer_oid(x, y, oid)
int x, y;
int oid;
{
    if (isok(x, y))
    {
        gbuf[y][x].layers.o_id = oid;
    }
}


void
clear_monster_layer_memory_at(x, y)
int x, y;
{
    levl[x][y].hero_memory_layers.layer_glyphs[LAYER_MONSTER] = NO_GLYPH;
    levl[x][y].hero_memory_layers.layer_gui_glyphs[LAYER_MONSTER] = NO_GLYPH;
    clear_monster_layerinfo(&levl[x][y].hero_memory_layers);
}

void
clear_monster_extra_info(x, y)
int x, y;
{
    if (isok(x, y))
    {
        change_layer_damage_displayed(x, y, 0);
        change_layer_hit_tile(x, y, 0);

        unsigned long old_flags = gbuf[y][x].layers.layer_flags;
        gbuf[y][x].layers.layer_flags &= ~(LFLAGS_M_MASK);
        if (old_flags != gbuf[y][x].layers.layer_flags)
        {
            gbuf[y][x].isnew = 1;
            if (gbuf_start[y] > x)
                gbuf_start[y] = x;
            if (gbuf_stop[y] < x)
                gbuf_stop[y] = x;
        }
    }
}


void
clear_monster_layer_at(x, y)
int x, y;
{
    show_glyph_on_layer(x, y, NO_GLYPH, LAYER_MONSTER);
    clear_monster_extra_info(x, y);
    clear_monster_layerinfo(&gbuf[y][x].layers);

}


void
show_monster_glyph_with_extra_info(x, y, glyph, mtmp, disp_flags, hit_tile_id, damage_displayed)
int x, y, glyph;
struct monst* mtmp;
unsigned long disp_flags;
int hit_tile_id, damage_displayed;
{
    boolean isyou = (mtmp == &youmonst);
    show_monster_glyph_with_extra_info_choose_ascii(x, y, glyph, mtmp, mtmp ? (isyou ? u.ux0 : mtmp->mx0) : 0, mtmp ? (isyou ? u.uy0 : mtmp->my0) : 0, disp_flags, hit_tile_id, damage_displayed, FALSE);
}

void
show_monster_glyph_with_extra_info_choose_ascii(x, y, glyph,  mtmp, x0, y0, disp_flags, hit_tile_id, damage_displayed, exclude_ascii)
int x, y, glyph, x0, y0;
struct monst* mtmp;
unsigned long disp_flags;
int hit_tile_id, damage_displayed;
boolean exclude_ascii;
{
    if (isok(x, y))
    {
        boolean loc_is_you = (u.ux == x && u.uy == y);

        /* Replace */
        int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_MONSTER, (struct obj*)0, mtmp, 0UL, 0UL, MAT_NONE, 0));

        if(!exclude_ascii)
            show_glyph_ascii(x, y, glyph);

        show_gui_glyph_on_layer(x, y, glyph, gui_glyph, LAYER_MONSTER);
        clear_monster_extra_info(x, y);
        show_extra_info(x, y, disp_flags, hit_tile_id, damage_displayed);

        clear_monster_layerinfo(&gbuf[y][x].layers);
        gbuf[y][x].layers.monster_origin_x = x0;
        gbuf[y][x].layers.monster_origin_y = y0;
        gbuf[y][x].layers.m_id = mtmp ? mtmp->m_id : 0;

        if (disp_flags & LFLAGS_M_YOU)
        {
            if (u.utrap)
            {
                if (u.utraptype == TT_PIT || u.utraptype == TT_LAVA)
                {
                    gbuf[y][x].layers.special_monster_layer_height = SPECIAL_HEIGHT_IN_PIT;
                }
            }
            else if ((ELevitation || (HLevitation & TIMEOUT)) && !Blocks_Levitation)
            {
                gbuf[y][x].layers.special_monster_layer_height = SPECIAL_HEIGHT_LEVITATION;
            }
        }
        else if (mtmp)
        {
            struct trap* t = 0;
            if (mtmp->mtrapped && (t = t_at(mtmp->mx, mtmp->my)) != 0 && (t->ttyp == PIT || t->ttyp == SPIKED_PIT))
                gbuf[y][x].layers.special_monster_layer_height = SPECIAL_HEIGHT_IN_PIT;
            else if ((mtmp->mprops[LEVITATION] & (M_TIMEOUT | M_EXTRINSIC)) != 0 && mtmp->mprops[BLOCKS_LEVITATION] == 0)
                gbuf[y][x].layers.special_monster_layer_height = SPECIAL_HEIGHT_LEVITATION;
        }

        unsigned long extra_flags = 0UL;


        /* Monster info flags; can be set only in this function */
        if (loc_is_you || mtmp)
        {
            struct monst* used_mtmp = mtmp ? mtmp : &youmonst;

            if (is_semi_transparent(used_mtmp->data) && !Hallucination)
                gbuf[y][x].layers.monster_flags |= LMFLAGS_SEMI_TRANSPARENT;

            if (is_radially_transparent(used_mtmp->data) && !Hallucination)
                gbuf[y][x].layers.monster_flags |= LMFLAGS_RADIAL_TRANSPARENCY;

            if (is_glass_transparent(used_mtmp->data) && !Hallucination)
                gbuf[y][x].layers.monster_flags |= LMFLAGS_GLASS_TRANSPARENCY;

            if (is_long_worm_with_tail(used_mtmp->data))
                gbuf[y][x].layers.monster_flags |= LMFLAGS_LONG_WORM_WITH_TAIL;

            if (is_long_worm_tail(used_mtmp->data))
                gbuf[y][x].layers.monster_flags |= LMFLAGS_LONG_WORM_TAIL;

            if (loc_is_you)
            {
                if (mtmp) /* Steed */
                {
                    if (is_invisible(mtmp) && canspotmon(mtmp))
                        gbuf[y][x].layers.monster_flags |= LMFLAGS_INVISIBLE_TRANSPARENT;
                }
                else
                {
                    if (Invis)
                        gbuf[y][x].layers.monster_flags |= LMFLAGS_INVISIBLE_TRANSPARENT;
                }
                if (canspotself())
                    gbuf[y][x].layers.monster_flags |= LMFLAGS_CAN_SPOT_SELF;
            }
            else if (mtmp)
            {
                if (is_invisible(mtmp) && canspotmon(mtmp))
                    gbuf[y][x].layers.monster_flags |= LMFLAGS_INVISIBLE_TRANSPARENT;
                if (is_boss_monster(mtmp->data) && !is_peaceful(mtmp))
                    gbuf[y][x].layers.monster_flags |= LMFLAGS_BOSS_MONSTER_FIGHT;
            }
        }
       
        if (mtmp)
        {
            if (is_tame(mtmp) && !Hallucination)
                extra_flags |= LFLAGS_M_PET;

            if (is_peaceful(mtmp) && !is_tame(mtmp) && !Hallucination)
                extra_flags |= LFLAGS_M_PEACEFUL;

            if (canspotmon(mtmp))
                extra_flags |= LFLAGS_M_CANSPOTMON;

            /* Worm info */
            int wdir_out = get_wseg_dir_at(mtmp, x, y);
            int wdir_in = get_reverse_prev_wseg_dir_at(mtmp, x, y);
            boolean is_head = is_wseg_head(mtmp, x, y);
            boolean is_tailend = is_wseg_tailend(mtmp, x, y);
            gbuf[y][x].layers.wsegdir = wdir_out;
            gbuf[y][x].layers.reverse_prev_wsegdir = wdir_in;
            if (is_head)
                gbuf[y][x].layers.monster_flags |= LMFLAGS_WORM_HEAD;
            if (is_tailend)
                gbuf[y][x].layers.monster_flags |= LMFLAGS_WORM_TAILEND;
        }

        /* Add to layer */
        if(extra_flags != 0UL)
            add_glyph_buffer_layer_flags(x, y, extra_flags);

        if(loc_is_you || mtmp)
        {
            unsigned long layer_flags = disp_flags | extra_flags;
            update_tile_gui_info(loc_is_you, mtmp, x, y, layer_flags);
        }

    }
}

void
update_tile_gui_info(loc_is_you, mtmp, x, y, layer_flags)
boolean loc_is_you;
struct monst* mtmp;
int x, y;
unsigned long layer_flags;
{
    if (loc_is_you && !mtmp)
        mtmp = &youmonst;

    gbuf[y][x].layers.monster_hp = loc_is_you ? (Upolyd ? u.mh : u.uhp) : (mtmp ? mtmp->mhp : 0);
    gbuf[y][x].layers.monster_maxhp = loc_is_you ? (Upolyd ? u.mhmax : u.uhpmax) : (mtmp ? mtmp->mhpmax : 0);

    /* Status bits*/
    boolean issteed = (mtmp == u.usteed);
    boolean ispeaceful = (layer_flags & LFLAGS_M_PEACEFUL) != 0;
    boolean ispet = (layer_flags & LFLAGS_M_PET) != 0;
    boolean isdetected = (layer_flags & LFLAGS_M_DETECTED) != 0;

    gbuf[y][x].layers.status_bits = get_m_status_bits(mtmp, loc_is_you, ispeaceful, ispet, isdetected);

    /* Conditions */
    gbuf[y][x].layers.condition_bits = get_m_condition_bits(loc_is_you ? &youmonst : mtmp);

    /* Buffs */
    get_m_buff_bits(mtmp, gbuf[y][x].layers.buff_bits, loc_is_you);

    /* Steed mark (you as small) */
    if (loc_is_you && issteed)
    {
        int rider_glyph = u_to_glyph();
        int rider_gui_glyph = maybe_get_replaced_glyph(rider_glyph, x, y, data_to_replacement_info(rider_glyph, LAYER_MONSTER, (struct obj*)0, &youmonst, 0UL, 0UL, MAT_NONE, 0));
        gbuf[y][x].layers.rider_glyph = rider_glyph;
        gbuf[y][x].layers.rider_gui_glyph = rider_gui_glyph;
    }
}

void
refresh_u_tile_gui_info(flush)
boolean flush;
{
    update_tile_gui_info(TRUE, &youmonst, u.ux, u.uy, gbuf[u.uy][u.ux].layers.layer_flags);
    force_redraw_at(u.ux, u.uy);
    if(flush)
        flush_screen(1);
}

void
refresh_m_tile_gui_info(mtmp, flush)
struct monst* mtmp;
boolean flush;
{
    if (!mtmp || !isok(mtmp->mx, mtmp->my))
        return;

    boolean loc_is_you = (u.ux == mtmp->mx && u.uy == mtmp->my);

    if (mtmp == &youmonst)
    {
        refresh_u_tile_gui_info(flush);
    }
    else
    {
        update_tile_gui_info(loc_is_you, mtmp, mtmp->mx, mtmp->my, gbuf[mtmp->my][mtmp->mx].layers.layer_flags);
        force_redraw_at(mtmp->mx, mtmp->my);
        if(flush)
            flush_screen(1);
    }
}

void
remove_current_glyph_from_layer(x, y)
int x, y;
{
    add_or_remove_glyph_to_layer(x, y, gbuf[y][x].layers.glyph, TRUE);
}

void
add_glyph_to_layer(x, y, glyph)
int x, y, glyph;
{
    add_or_remove_glyph_to_layer(x, y, glyph, FALSE);
}

void
add_or_remove_glyph_to_layer(x, y, glyph, remove)
int x, y, glyph;
boolean remove;
{
    if (isok(x, y))
    {
        int gui_glyph = glyph;
        if (glyph_is_cmap_or_cmap_variation(glyph))
        {
            int cmap_idx = generic_glyph_to_cmap(glyph);
            if (!remove)
                gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, defsyms[cmap_idx].layer, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
            gbuf[y][x].layers.layer_glyphs[defsyms[cmap_idx].layer] = remove ? NO_GLYPH : glyph;
            gbuf[y][x].layers.layer_gui_glyphs[defsyms[cmap_idx].layer] = remove ? NO_GLYPH : gui_glyph;
        }
        else if (glyph_is_monster(glyph) || glyph_is_invisible(glyph) || glyph_is_warning(glyph)) /* includes also players */
        {
            if (!remove)
                gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_MONSTER, (struct obj*)0, m_at(x, y), 0UL, 0UL, MAT_NONE, 0));
            gbuf[y][x].layers.layer_glyphs[LAYER_MONSTER] = remove ? NO_GLYPH : glyph;
            gbuf[y][x].layers.layer_gui_glyphs[LAYER_MONSTER] = remove ? NO_GLYPH : gui_glyph;
            clear_monster_extra_info(x, y);
            clear_monster_layerinfo(&gbuf[y][x].layers);
        }
        else if (glyph_is_object(glyph))
        {
            int obj_idx = glyph_to_obj(glyph);
            if (!remove)
                gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, is_otyp_drawn_in_front(obj_idx, x, y) ? LAYER_COVER_OBJECT : LAYER_OBJECT, level.objects[x][y], (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));

            if (is_otyp_drawn_in_front(obj_idx, x, y))
            {
                gbuf[y][x].layers.layer_glyphs[LAYER_COVER_OBJECT] = remove ? NO_GLYPH : glyph;
                gbuf[y][x].layers.layer_gui_glyphs[LAYER_COVER_OBJECT] = remove ? NO_GLYPH : gui_glyph;
            }
            else
            {
                gbuf[y][x].layers.layer_glyphs[LAYER_OBJECT] = remove ? NO_GLYPH : glyph;
                gbuf[y][x].layers.layer_gui_glyphs[LAYER_OBJECT] = remove ? NO_GLYPH : gui_glyph;
            }

            gbuf[y][x].layers.layer_flags &= ~LFLAGS_O_MASK;
        }
        else if (glyph_is_missile(glyph))
        {
            if (!remove)
                gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_MISSILE, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));

            gbuf[y][x].layers.layer_glyphs[LAYER_MISSILE] = remove ? NO_GLYPH : glyph;
            gbuf[y][x].layers.layer_gui_glyphs[LAYER_MISSILE] = remove ? NO_GLYPH : gui_glyph;
        }
        else if (glyph_is_zap(glyph))
        {
            if(!remove)
                gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_ZAP, (struct obj*)0, (struct monst*)0, 0UL, gbuf[y][x].layers.missile_flags, MAT_NONE, 0));

            gbuf[y][x].layers.layer_glyphs[LAYER_ZAP] = remove ? NO_GLYPH : glyph;
            gbuf[y][x].layers.layer_gui_glyphs[LAYER_ZAP] = remove ? NO_GLYPH : gui_glyph;
        }
        else if (glyph_is_swallow(glyph) || glyph_is_explosion(glyph))
        {
            if (!remove)
                gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_GENERAL_EFFECT, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));

            gbuf[y][x].layers.layer_glyphs[LAYER_GENERAL_EFFECT] = remove ? NO_GLYPH : glyph;
            gbuf[y][x].layers.layer_gui_glyphs[LAYER_GENERAL_EFFECT] = remove ? NO_GLYPH : gui_glyph;
        }
        else
        {
            if (!remove)
                gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, LAYER_GENERAL_UI, (struct obj*)0, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
            /* Should not get here */
            gbuf[y][x].layers.layer_glyphs[LAYER_GENERAL_UI] = remove ? NO_GLYPH : glyph;
            gbuf[y][x].layers.layer_gui_glyphs[LAYER_GENERAL_UI] = remove ? NO_GLYPH : gui_glyph;
        }
    }
}

void
show_glyph_ascii(x, y, glyph)
int x, y, glyph;
{

    /*
     * Check for bad positions and glyphs.
     */
    if (!isok(x, y)) 
    {
        const char *text;
        int offset;

        /* column 0 is invalid, but it's often used as a flag, so ignore it */
        if (x == 0)
            return;

        /*
         *  This assumes an ordering of the offsets.  See display.h for
         *  the definition.
         */

        if (glyph >= GLYPH_SPECIAL_EFFECT_OFF
            && glyph < MAX_GLYPH) { /* UI, animation, and other glyphs */
            text = "other glyph";
            offset = glyph - GLYPH_SPECIAL_EFFECT_OFF;
        } else if (glyph >= GLYPH_PLAYER_ATTACK_OFF
            && glyph < GLYPH_SPECIAL_EFFECT_OFF) { /* a player character in action */
            text = "player character in action";
            offset = glyph - GLYPH_PLAYER_ATTACK_OFF;
        } else if (glyph >= GLYPH_PLAYER_OFF
            && glyph < GLYPH_PLAYER_ATTACK_OFF) { /* a player character */
            text = "player character";
            offset = glyph - GLYPH_PLAYER_OFF;
        } else if (glyph >= GLYPH_ARTIFACT_MISSILE_OFF
            && glyph < GLYPH_PLAYER_OFF) { /* an artifact as missile */
            text = "artifact missile";
            offset = glyph - GLYPH_ARTIFACT_MISSILE_OFF;
        } else if (glyph >= GLYPH_ARTIFACT_OFF
            && glyph < GLYPH_ARTIFACT_MISSILE_OFF) { /* an artifact */
            text = "artifact";
            offset = glyph - GLYPH_ARTIFACT_OFF;
        } else if (glyph >= GLYPH_FEMALE_BODY_OFF) { /* female corpse */
            text = "female corpse";
            offset = glyph - GLYPH_FEMALE_BODY_OFF;
        } else if (glyph >= GLYPH_BODY_OFF) { /* a corpse */
            text = "corpse";
            offset = glyph - GLYPH_BODY_OFF;
        } else if (glyph >= GLYPH_FEMALE_STATUE_OFF
            && glyph < GLYPH_BODY_OFF) { /* a female statue */
            text = "female statue";
            offset = glyph - GLYPH_FEMALE_STATUE_OFF;
        } else if (glyph >= GLYPH_STATUE_OFF
            && glyph < GLYPH_FEMALE_STATUE_OFF) { /* a statue */
            text = "statue";
            offset = glyph - GLYPH_STATUE_OFF;
        } else if (glyph >= GLYPH_WORM_OFF && glyph  < GLYPH_STATUE_OFF) { /* worm */
            text = "worm";
            offset = glyph - GLYPH_WORM_OFF;
        } else if (glyph >= GLYPH_WARNING_OFF
            && glyph < GLYPH_WORM_OFF) { /* a warning */
            text = "warning";
            offset = glyph - GLYPH_WARNING_OFF;
        } else if (glyph >= GLYPH_SWALLOW_OFF) { /* swallow border */
            text = "swallow border";
            offset = glyph - GLYPH_SWALLOW_OFF;
        } else if (glyph >= GLYPH_ZAP_OFF) { /* zap beam */
            text = "zap beam";
            offset = glyph - GLYPH_ZAP_OFF;
        } else if (glyph >= GLYPH_EXPLODE_OFF) { /* explosion */
            text = "explosion";
            offset = glyph - GLYPH_EXPLODE_OFF;
        } else if (glyph >= GLYPH_BROKEN_CMAP_VARIATION_OFF) { /* broken cmap variation */
            text = "broken cmap variation";
            offset = glyph - GLYPH_BROKEN_CMAP_VARIATION_OFF;
        } else if (glyph >= GLYPH_CMAP_VARIATION_OFF) { /* cmap variation */
            text = "cmap variation";
            offset = glyph - GLYPH_CMAP_VARIATION_OFF;
        } else if (glyph >= GLYPH_BROKEN_CMAP_OFF) { /* broken cmap */
            text = "broken cmap index";
            offset = glyph - GLYPH_BROKEN_CMAP_OFF;
        } else if (glyph >= GLYPH_CMAP_OFF) { /* cmap */
            text = "cmap index";
            offset = glyph - GLYPH_CMAP_OFF;
        } else if (glyph >= GLYPH_OBJ_MISSILE_OFF) { /* object as missile */
            text = "object missile";
            offset = glyph - GLYPH_OBJ_MISSILE_OFF;
        } else if (glyph >= GLYPH_OBJ_OFF) { /* object */
            text = "object";
            offset = glyph - GLYPH_OBJ_OFF;
        } else if (glyph >= GLYPH_INVIS_OFF) { /* invisible mon */
            text = "invisible mon";
            offset = glyph - GLYPH_INVIS_OFF;
        } else if (glyph >= GLYPH_FEMALE_ATTACK_OFF) { /* female mon in action */
            text = "female mon in action";
            offset = glyph - GLYPH_FEMALE_ATTACK_OFF;
        } else if (glyph >= GLYPH_FEMALE_MON_OFF) { /* female mon */
            text = "female mon";
            offset = glyph - GLYPH_FEMALE_MON_OFF;
        } else if (glyph >= GLYPH_ATTACK_OFF) { /* mon in action */
            text = "mon in action";
            offset = glyph - GLYPH_ATTACK_OFF;
        } else { /* a monster */
            text = "monster";
            offset = glyph;
        }

        impossible("show_glyph:  bad pos %d %d with glyph %d [%s %d].", x, y,
                   glyph, text, offset);
        return;
    }

    if (glyph > MAX_GLYPH) {
        impossible("show_glyph:  bad glyph %d [max %d] at (%d,%d).", glyph,
                   MAX_GLYPH, x, y);
        return;
    }

    //if (gbuf[y][x].layers.glyph != glyph || iflags.use_background_glyph) // Always true
    {
        gbuf[y][x].layers.glyph = glyph;
        gbuf[y][x].layers.bkglyph = get_bk_glyph(x, y);
        gbuf[y][x].isnew = 1;
        if (gbuf_start[y] > x)
            gbuf_start[y] = x;
        if (gbuf_stop[y] < x)
            gbuf_stop[y] = x;
    }
}

/*
 * Reset the changed glyph borders so that none of the 3rd screen has
 * changed.
 */
#define reset_glyph_bbox()             \
    {                                  \
        int i;                         \
                                       \
        for (i = 0; i < ROWNO; i++) {  \
            gbuf_start[i] = COLNO - 1; \
            gbuf_stop[i] = 0;          \
        }                              \
    }

/*
 * Turn the 3rd screen into stone.
 */
void
clear_glyph_buffer()
{
    register int x, y;
    register gbuf_entry *gptr;

    for (y = 0; y < ROWNO; y++) {
        gptr = &gbuf[y][0];
        for (x = COLNO; x > 0; x--) {
            *gptr++ = nul_gbuf;
        }
    }
    reset_glyph_bbox();
}

/*
 * Assumes that the indicated positions are filled with S_unexplored glyphs.
 */
void
row_refresh(start, stop, y)
int start, stop, y;
{
    register int x;

    for (x = start; x <= stop; x++)
        if (gbuf[y][x].layers.glyph != cmap_to_glyph(S_unexplored))
            print_glyph(WIN_MAP, x, y, gbuf[y][x].layers);
}

void
cls()
{
    if (in_cls)
        return;
    in_cls = TRUE;
    display_nhwindow(WIN_MESSAGE, FALSE); /* flush messages */
    context.botlx = 1;                    /* force update of botl window */
    clear_nhwindow(WIN_MAP);              /* clear physical screen */

    clear_glyph_buffer(); /* this is sort of an extra effort, but OK */
    in_cls = FALSE;
}

void
show_memory_everywhere()
{
    int x, y;
    for (x = 1; x < COLNO; x++) 
    {
        for (y = 0; y < ROWNO; y++)
            add_glyph_buffer_layer_flags(x, y, LFLAGS_SHOWING_MEMORY);
    }

}

void
show_detection_everywhere()
{
    int x, y;
    for (x = 1; x < COLNO; x++)
    {
        for (y = 0; y < ROWNO; y++)
            add_glyph_buffer_layer_flags(x, y, LFLAGS_SHOWING_DETECTION);
    }

}

void
show_memory_and_detection_everywhere()
{
    int x, y;
    for (x = 1; x < COLNO; x++)
    {
        for (y = 0; y < ROWNO; y++)
            add_glyph_buffer_layer_flags(x, y, LFLAGS_SHOWING_MEMORY | LFLAGS_SHOWING_DETECTION);
    }

}


/*
 * Synch the third screen with the display.
 */
void
flush_screen(cursor_on_u)
int cursor_on_u;
{
    /* Prevent infinite loops on errors:
     *      flush_screen->print_glyph->impossible->pline->flush_screen
     */

    register int x, y;

    if (cursor_on_u == -1)
        delay_flushing = !delay_flushing;
    if (delay_flushing)
        return;
    if (flushing)
        return; /* if already flushing then return */
    flushing = 1;
#ifdef HANGUPHANDLING
    if (program_state.done_hup)
        return;
#endif

#if (defined(GNH_MOBILE) || defined(WIN32)) && defined(USE_TILES)
    char saved_gbuf_start[ROWNO];
    char saved_gbuf_stop[ROWNO];
    memcpy(saved_gbuf_start, gbuf_start, ROWNO);
    memcpy(saved_gbuf_stop, gbuf_stop, ROWNO);
    int layer_idx, signed_glyph, repl;
    register gbuf_entry* gptr_adj;
    for (y = 0; y < ROWNO; y++)
    {
        register gbuf_entry* gptr = &gbuf[y][x = saved_gbuf_start[y]];

        for (; x <= saved_gbuf_stop[y]; gptr++, x++)
        {
            if (gptr->isnew)
            {
                int dir;
                for (dir = 0; dir < 4; dir++)
                {
                    int rx = 0;
                    int ry = 0;
                    unsigned long dir_bit = 0;
                    switch (dir)
                    {
                    case 0:
                        rx = x - 1;
                        ry = y;
                        dir_bit = REPLACEMENT_EVENT_UPDATE_FROM_RIGHT;
                        break;
                    case 1:
                        rx = x + 1;
                        ry = y;
                        dir_bit = REPLACEMENT_EVENT_UPDATE_FROM_LEFT;
                        break;
                    case 2:
                        rx = x;
                        ry = y - 1;
                        dir_bit = REPLACEMENT_EVENT_UPDATE_FROM_BELOW;
                        break;
                    case 3:
                        rx = x;
                        ry = y + 1;
                        dir_bit = REPLACEMENT_EVENT_UPDATE_FROM_TOP;
                        break;
                    default:
                        break;
                    }

                    if (isok(rx, ry))
                    {
                        gptr_adj = &gbuf[ry][rx];
                        for (layer_idx = 0; layer_idx < MAX_LAYERS; layer_idx++)
                        {
                            signed_glyph = gptr_adj->layers.layer_glyphs[layer_idx];
                            if (signed_glyph != NO_GLYPH)
                            {
                                repl = glyph2replacement[abs(signed_glyph)];
                                if (repl)
                                {
                                    if (replacements[repl].replacement_events & dir_bit)
                                    {
                                        newsym_with_flags(rx, ry, NEWSYM_FLAGS_KEEP_OLD_EFFECT_MISSILE_ZAP_GLYPHS_AND_FLAGS);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (y = 0; y < ROWNO; y++)
    {
        for (x = gbuf_start[y]; x <= gbuf_stop[y]; x++)
        {
            if (cansee(x, y))
            {
                add_glyph_buffer_layer_flags(x, y, LFLAGS_CAN_SEE);
            }

            boolean is_lit_unknown_wall = (levl[x][y].waslit && IS_NON_STONE_WALL(levl[x][y].typ) && wall_angle(&levl[x][y]) == S_stone);
            if (!levl[x][y].waslit || is_lit_unknown_wall)
            {
                add_glyph_buffer_layer_flags(x, y, LFLAGS_APPEARS_UNLIT);
            }
        }
    }

    for (y = 0; y < ROWNO; y++)
    {
        int x_start = gbuf_start[y];
        if (y > 0 && gbuf_start[y - 1] < x_start)
            x_start = gbuf_start[y - 1];
        if (y < ROWNO - 1 && gbuf_start[y + 1] < x_start)
            x_start = gbuf_start[y + 1];

        int x_stop = gbuf_stop[y];
        if (y > 0 && gbuf_stop[y - 1] > x_stop)
            x_stop = gbuf_stop[y - 1];
        if (y < ROWNO - 1 && gbuf_stop[y + 1] > x_stop)
            x_stop = gbuf_stop[y + 1];

        if (x_start > x_stop)
            continue;

        for (x = x_start - 1; x <= x_stop + 1; x++)
        {
            if (isok(x, y))
            {
                boolean nwead = NO_WALL_END_AUTODRAW(x, y);
                boolean cur = (gbuf[y][x].layers.layer_flags & LFLAGS_NO_WALL_END_AUTODRAW) != 0;
                if (nwead != cur)
                {
                    gbuf[y][x].isnew = 1;
                    gbuf[y][x].layers.layer_flags &= ~LFLAGS_NO_WALL_END_AUTODRAW;
                    if (nwead)
                        gbuf[y][x].layers.layer_flags |= LFLAGS_NO_WALL_END_AUTODRAW;
                }
            }
        }
    }
#endif
    issue_gui_command(Is_really_rogue_level(&u.uz) ? GUI_CMD_FORCE_ASCII : GUI_CMD_UNFORCE_ASCII);
    issue_gui_command(GUI_CMD_START_FLUSH);
    for (y = 0; y < ROWNO; y++) {
        register gbuf_entry *gptr = &gbuf[y][x = gbuf_start[y]];

        for (; x <= gbuf_stop[y]; gptr++, x++)
            if (gptr->isnew) {
                print_glyph(WIN_MAP, x, y, gptr->layers);
                gptr->isnew = 0;
            }
    }
    issue_gui_command(GUI_CMD_FINISH_FLUSH);

    if (cursor_on_u)
        curs(WIN_MAP, u.ux, u.uy); /* move cursor to the hero */
    display_nhwindow(WIN_MAP, FALSE);
    reset_glyph_bbox();
    flushing = 0;
    if (context.botl || context.botlx)
        bot();
    else if (iflags.time_botl)
        timebot();
}

/* ======================================================================== */

/*
 * back_to_glyph()
 *
 * Use the information in the rm structure at the given position to create
 * a glyph of a background.
 *
 * I had to add a field in the rm structure (horizontal) so that we knew
 * if open doors and secret doors were horizontal or vertical.  Previously,
 * the screen symbol had the horizontal/vertical information set at
 * level generation time.
 *
 * I used the 'ladder' field (really doormask) for deciding if stairwells
 * were up or down.  I didn't want to check the upstairs and dnstairs
 * variables.
 */
int
back_to_glyph(x, y)
xchar x, y;
{
    int idx;
    struct rm *ptr = &(levl[x][y]);
    boolean is_variation = FALSE;
    boolean facing_right = (ptr->facing_right != 0);
    int multiplier = facing_right ? -1 : 1;
    int cmap_type = levl[x][y].use_special_tileset ? levl[x][y].special_tileset : get_current_cmap_type_index();

    switch (ptr->typ) {
    case UNDEFINED_LOCATION:
        idx = level.flags.arboreal ? S_tree : S_unexplored;
        break;
    case SCORR:
    case STONE:
    {
    stone_here:
        {
            idx = S_stone;
            int used_vartyp = (ptr->typ == STONE ? ptr->vartyp : (ptr->vartyp % NUM_NORMAL_STONE_VARTYPES));
            if ((used_vartyp > 0 || ptr->subtyp > 0) && ptr->typ != SDOOR)
            { /* Walls use also stone subtypes if looked from outside, but they can have a larger subtype than what is possible for walls */
                is_variation = TRUE;
                int var_offset = defsyms[idx].variation_offset;
                idx = var_offset + stone_subtype_definitions[ptr->subtyp].variation_offset + used_vartyp - 1;
            }
        }
    }
    break;
    case ROOM:
        idx = /*(!ptr->waslit || flags.dark_room) && !cansee(x, y) ? DARKROOMSYM: */ S_room;

        if (ptr->subtyp > 0 || ptr->vartyp > 0)
        {
            is_variation = TRUE;
            int var_offset = defsyms[idx].variation_offset;
            idx = var_offset + floor_subtype_definitions[ptr->subtyp].variation_offset + ptr->vartyp - 1;
        }

        break;
    case GRASS:
        idx = /*(!ptr->waslit || flags.dark_room) && !cansee(x, y) ? DARKGRASSSYM :*/ S_grass;
        if (ptr->subtyp > 0 || ptr->vartyp > 0)
        {
            is_variation = TRUE;
            int var_offset = defsyms[idx].variation_offset;
            idx = var_offset + grass_subtype_definitions[ptr->subtyp].variation_offset + ptr->vartyp - 1;
        }
        break;
    case GROUND:
        idx = S_ground;
        if (ptr->subtyp > 0 || ptr->vartyp > 0)
        {
            is_variation = TRUE;
            int var_offset = defsyms[idx].variation_offset;
            idx = var_offset + ground_subtype_definitions[ptr->subtyp].variation_offset + ptr->vartyp - 1;
        }
        break;
    case CORR:
        idx = /* (ptr->waslit || flags.lit_corridor) ? */ S_litcorr /* : S_corr */;
        if (ptr->subtyp > 0 || ptr->vartyp > 0)
        {
            is_variation = TRUE;
            int var_offset = defsyms[idx].variation_offset;
            idx = var_offset + corridor_subtype_definitions[ptr->subtyp].variation_offset + ptr->vartyp - 1;
        }
        break;
    case HWALL:
    {
        if (ptr->seenv)
        {
            idx = wall_angle(ptr);
            
            if (idx == S_stone)
                break;

        hwall_here:
            {
                if (idx == S_hwall && (ptr->subtyp > 0 || ptr->vartyp > 0) && ptr->typ != SDOOR)
                {
                    is_variation = TRUE;
                    int var_offset = defsyms[idx].variation_offset;
                    idx = var_offset + wall_subtype_definitions[ptr->subtyp].variation_offset + ptr->vartyp - 1;
                }
            }
        }
        else
            goto stone_here;
        break;
    }
    case VWALL:
        if (ptr->seenv)
        {
            idx = wall_angle(ptr);

            if (idx == S_stone)
                break;

            if (idx == S_hwall)
                goto hwall_here;

            if (idx == S_vwall && (ptr->subtyp > 0 || ptr->vartyp > 0))
            {
                is_variation = TRUE;
                int var_offset = defsyms[idx].variation_offset;
                idx = var_offset + wall_subtype_definitions[ptr->subtyp].variation_offset + ptr->vartyp - 1;
            }
        }
        else
            goto stone_here;
        break;
    case TLCORNER:
    case TRCORNER:
    case CROSSWALL:
    case BLCORNER:
    case BRCORNER:
    case TUWALL:
    case TDWALL:
    case TLWALL:
    case TRWALL:
    case SDOOR:
        if (ptr->seenv)
        {
            idx = wall_angle(ptr);

            if (idx == S_hwall)
                goto hwall_here;
        }
        else
            goto stone_here;
        break;
    case DOOR:
    {
        int sym_idx = S_ndoor;
        int subtyp = ptr->subtyp;

        if ((ptr->doormask & D_MASK)) {
            if (ptr->doormask & D_BROKEN)
                sym_idx = (ptr->horizontal) ? S_hbdoor : S_vbdoor;
            else if (ptr->doormask & D_ISOPEN)
                sym_idx = (ptr->horizontal) ? S_hodoor : S_vodoor;
            else if (ptr->doormask & D_PORTCULLIS)
                sym_idx = (ptr->horizontal) ? S_hoportcullis : S_voportcullis;
            else /* else is closed */
                sym_idx = (ptr->horizontal) ? S_hcdoor : S_vcdoor;
        }
        else
            sym_idx = S_ndoor;

        if ((subtyp == 0) || sym_idx == S_ndoor)
        {
            idx = sym_idx;
        }
        else
        {
            is_variation = TRUE;
            int var_offset = defsyms[sym_idx].variation_offset;
            idx = var_offset + subtyp - 1;
        }
        break;
    }
    case IRONBARS:
        idx = S_bars;
        break;
    case TREE:
    {
        int subtyp_idx = ptr->subtyp;
        int vartyp_idx = ptr->vartyp;
        int sym_idx = S_tree;
        if (subtyp_idx == 0 && vartyp_idx == 0)
        {
            idx = sym_idx;
        }
        else
        {
            is_variation = TRUE;
            int var_offset = defsyms[sym_idx].variation_offset;
            idx = var_offset + tree_subtype_definitions[ptr->subtyp].variation_offset + vartyp_idx - 1;
        }
        break;
    }
    case POOL:
    case MOAT:
        idx = S_pool;
        break;
    case STAIRS:
    {
        //boolean is_branch_staircase = (sstairs.sx  && x == sstairs.sx && y == sstairs.sy);
        //boolean is_extra_staircase = use_extra_special_staircase();
        int subtyp_idx = ptr->subtyp; // is_branch_staircase ? (is_extra_staircase ? STAIRCASE_TO_DEEPER: STAIRCASE_BRANCH) : 0;
        int sym_idx = (ptr->ladder & LA_DOWN) ? S_dnstair : S_upstair;
        if (subtyp_idx == 0)
        {
            idx = sym_idx;
        }
        else
        {
            is_variation = TRUE;
            int var_offset = defsyms[sym_idx].variation_offset;
            idx = var_offset + subtyp_idx - 1;
        }
        break;
    }
    case LADDER:
        idx = (ptr->ladder & LA_DOWN) ? S_dnladder : S_upladder;
        break;
    case FOUNTAIN:
    {
        int ftyp = levl[x][y].subtyp; // (levl[x][y].fountainmask& FOUNTAIN_TYPE_MASK);
        if (ftyp > 0)
        {
            is_variation = TRUE;
            idx = context.used_fountain_subtype[ftyp] - 1 + defsyms[S_fountain].variation_offset;
        }
        else
            idx = S_fountain;

        break;
    }
    case SINK:
        idx = S_sink;
        break;
    case ANVIL:
        idx = S_anvil;
        break;
    case ALTAR:
    {
        aligntyp alignment_mask = (ptr->altarmask & ~AM_SHRINE);
        aligntyp alignment = Amask2align(alignment_mask);
        int var_idx = -1;
        
        if (ptr->subtyp > 0 || alignment != A_NONE)
        {
            if (ptr->subtyp == 0)
            {
                var_idx = altar_subtype_definitions[ptr->subtyp].variation_offset + 
                    (alignment == A_LAWFUL ? 0 :
                        alignment == A_NEUTRAL ? 1 :
                        alignment == A_CHAOTIC ? 2 : -1
                        );
            }
            else
            {
                var_idx = altar_subtype_definitions[ptr->subtyp].variation_offset - 1; /* Just one variation here with any alignment */
            }
        }

        int sym_idx = S_altar;
        if (var_idx == -1)
        {
            idx = sym_idx;
        }
        else
        {
            is_variation = TRUE;
            int var_offset = defsyms[sym_idx].variation_offset;
            idx = var_offset + var_idx;
        }
        break;
    }
    case GRAVE:
        idx = S_grave;
        break;
    case BRAZIER:
        idx = S_brazier;
        break;
    case SIGNPOST:
        idx = S_signpost;
        break;
    case THRONE:
        idx = S_throne;
        break;
    case LAVAPOOL:
        idx = S_lava;
        break;
    case ICE:
        idx = S_ice;
        break;
    case AIR:
        idx = S_air;
        break;
    case CLOUD:
        idx = S_cloud;
        break;
    case WATER:
        idx = S_water;
        break;
    case DBWALL:
        idx = (ptr->horizontal) ? S_hcdbridge : S_vcdbridge;
        break;
    case DRAWBRIDGE_UP:
        switch (ptr->drawbridgemask & DB_UNDER) {
        case DB_MOAT:
            idx = S_pool;
            break;
        case DB_LAVA:
            idx = S_lava;
            break;
        case DB_ICE:
            idx = S_ice;
            break;
        case DB_GROUND:
            idx = S_ground; //  S_room;
            break;
        default:
            impossible("Strange db-under: %d",
                       ptr->drawbridgemask & DB_UNDER);
            idx = S_room; /* something is better than nothing */
            break;
        }
        break;
    case DRAWBRIDGE_DOWN:
        idx = (ptr->horizontal) ? S_hodbridge : S_vodbridge;
        break;
    default:
        impossible("back_to_glyph:  unknown level type [ = %d ]", ptr->typ);
        idx = S_room;
        break;
    }

    if(is_variation)
        return multiplier * cmap_variation_with_type_to_glyph(idx, cmap_type);
    else
        return multiplier * cmap_with_type_to_glyph(idx, cmap_type);
}


boolean
use_extra_special_staircase()
{
    return (depth(&u.uz) == dungeons[u.uz.dnum].depth_start
        || depth(&u.uz) == dungeons[u.uz.dnum].depth_start + dungeons[u.uz.dnum].num_dunlevs - 1);
}

int
back_to_broken_glyph(x, y)
xchar x, y;
{
    int glyph = back_to_glyph(x, y);
    int multiplier = glyph < 0 ? -1 : 1;
    int absglyph = abs(glyph);

    if (glyph_is_cmap(absglyph))
    {
        int cmap = glyph_to_cmap(absglyph);
        if (defsyms[cmap].has_broken_tile == FALSE)
            return NO_GLYPH;

        return multiplier * (absglyph - GLYPH_CMAP_OFF + GLYPH_BROKEN_CMAP_OFF);
    }
    else if (glyph_is_cmap_variation(absglyph))
    {
        int cmap_var_idx = glyph_to_cmap_variation(absglyph);
        if (defsym_variations[cmap_var_idx].has_broken_tile == FALSE)
            return NO_GLYPH;

        return multiplier * (absglyph - GLYPH_CMAP_VARIATION_OFF + GLYPH_BROKEN_CMAP_VARIATION_OFF);
    }
    else
        return NO_GLYPH;
}

int
get_location_light_range(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return 0;

    struct rm* lev = &levl[x][y];

    /* Altars have candles */
    if (IS_ALTAR(lev->typ))
        return 1;

    /* Braziers are even bigger */
    if (IS_BRAZIER(lev->typ))
        return 5;

    if (lev->decoration_typ > 0 
        && (decoration_type_definitions[lev->decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIGHTABLE) != 0 
        && ((decoration_type_definitions[lev->decoration_typ].dflags & DECORATION_TYPE_FLAGS_LOOTABLE) == 0 || (lev->decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0)
       )
    {
        return 2;
    }

    return 0;
}

int
get_location_light_sidedness(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return 0;

    struct rm* lev = &levl[x][y];

    /* Altars have candles */
    if (IS_ALTAR(lev->typ))
        return 0;

    /* Braziers are even bigger */
    if (IS_BRAZIER(lev->typ))
        return 0;

    if (lev->decoration_typ > 0
        && (decoration_type_definitions[lev->decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIGHTABLE) != 0
        && ((decoration_type_definitions[lev->decoration_typ].dflags & DECORATION_TYPE_FLAGS_LOOTABLE) == 0 || (lev->decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0)
        )
    {
        return 1 + lev->decoration_dir;
    }

    return 0;
}


/*
 * swallow_to_glyph()
 *
 * Convert a monster number and a swallow location into the correct glyph.
 * If you don't want a patchwork monster while hallucinating, decide on
 * a random monster in swallowed() and don't use what_mon() here.
 */
STATIC_OVL int
swallow_to_glyph(mnum, loc)
int mnum;
int loc;
{
    if (loc < S_sw_tl || S_sw_br < loc) {
        impossible("swallow_to_glyph: bad swallow location");
        loc = S_sw_br;
    }
    return ((int) (what_mon(mnum, rn2_on_display_rng) * MAX_SWALLOW_CHARS) +
            (loc - S_sw_tl)) + GLYPH_SWALLOW_OFF;
}

/*
 * zapdir_to_glyph()
 *
 * Change the given zap direction and beam type into a glyph.  Each beam
 * type has four glyphs, one for each of the symbols below.  The order of
 * the zap symbols [0-3] as defined in rm.h are:
 *
 *      |  S_vbeam      ( 0, 1) or ( 0,-1)
 *      -  S_hbeam      ( 1, 0) or (-1, 0)
 *      \  S_lslant     ( 1, 1) or (-1,-1)
 *      /  S_rslant     (-1, 1) or ( 1,-1)
 */
int
zapdir_to_glyph(dx, dy, beam_type)
register int dx, dy;
int beam_type;
{
    if (beam_type >= MAX_ZAP_TYPES) {
        impossible("zapdir_to_glyph:  illegal beam type");
        beam_type = 0;
    }
    int zapdir_glyph_index = dir_to_beam_index(dx, dy);
    return ((int) ((beam_type * NUM_ZAP_CHARS) + zapdir_glyph_index)) + GLYPH_ZAP_OFF;
}

int
dir_to_beam_index(dx, dy)
register int dx, dy;
{
    boolean is_reverse = (dy > 0 || (dy == 0 && dx > 0));
    int dir_glyph_index = (dx == dy) ? 2 : (dx && dy) ? 3 : dx ? 1 : 0;

    return dir_glyph_index + (is_reverse ? 4 : 0);
}

int
zapbounce_to_glyph(orig_dx, orig_dy, new_dx, new_dy, beam_type)
register int orig_dx, orig_dy, new_dx, new_dy;
int beam_type;
{
    if (beam_type >= MAX_ZAP_TYPES) {
        impossible("zapbounce_to_glyph:  illegal beam type");
        beam_type = 0;
    }

    int zapbounce_glyph_index = 8;

    if (orig_dx == -new_dx && orig_dy == -new_dy)
    {
        zapbounce_glyph_index += 8;
        zapbounce_glyph_index += dir_to_beam_index(orig_dx, orig_dy);
    }
    else
    {
        if (orig_dx > 0 && orig_dy < 0 && new_dy > 0)
            zapbounce_glyph_index += 0;
        else if (orig_dx > 0 && orig_dy > 0 && new_dx < 0)
            zapbounce_glyph_index += 1;
        else if (orig_dx < 0 && orig_dy > 0 && new_dy < 0)
            zapbounce_glyph_index += 2;
        else if (orig_dx < 0 && orig_dy < 0 && new_dx > 0)
            zapbounce_glyph_index += 3;
        else if (orig_dx < 0 && orig_dy < 0 && new_dy > 0)
            zapbounce_glyph_index += 4;
        else if (orig_dx > 0 && orig_dy < 0 && new_dx < 0)
            zapbounce_glyph_index += 5;
        else if (orig_dx > 0 && orig_dy > 0 && new_dy < 0)
            zapbounce_glyph_index += 6;
        else if (orig_dx < 0 && orig_dy > 0 && new_dx > 0)
            zapbounce_glyph_index += 7;
    }
    return ((int)((beam_type * NUM_ZAP_CHARS) + zapbounce_glyph_index)) + GLYPH_ZAP_OFF;
}

int
zap_glyph_to_corner_glyph(orig_glyph, layer_flags, zap_source_idx)
int orig_glyph;
unsigned long layer_flags;
int zap_source_idx;
/* Orig_glyph is from a location relative to X:
   1 2 3 
   8 X 4
   7 6 5 */
{
    if (!glyph_is_zap(orig_glyph))
        return NO_GLYPH;

    int zap_index = (orig_glyph - GLYPH_ZAP_OFF) / NUM_ZAP_CHARS;
    int dir_index = (orig_glyph - GLYPH_ZAP_OFF) % NUM_ZAP_CHARS;

    if(dir_index >= 24)
        return NO_GLYPH;

    int res = -1;
    int used_zap_source_idx = (zap_source_idx - 1) % NUM_ZAP_SOURCE_BASE_DIRS + 1;
    boolean check_leading = TRUE;
    boolean check_trailing = TRUE;

    if (zap_source_idx > NUM_ZAP_SOURCE_BASE_DIRS)
        check_trailing = FALSE;
    else
        check_leading = FALSE;

    /* Leading cases */
    if (check_leading)
    {
        switch (dir_index)
        {
        case 0:
        case 20:
            if (used_zap_source_idx == 6 && (layer_flags & LFLAGS_ZAP_LEADING_EDGE))
                res = 48;
            break;
        case 1:
        case 21:
            if (used_zap_source_idx == 4 && (layer_flags & LFLAGS_ZAP_LEADING_EDGE))
                res = 51;
            break;
        case 2:
        case 10:
        case 13:
        case 22:
            if (used_zap_source_idx == 4)
                res = 25;
            else if (used_zap_source_idx == 6)
                res = 24;
            else if (used_zap_source_idx == 5 && (layer_flags & LFLAGS_ZAP_LEADING_EDGE))
                res = 32;
            break;
        case 3:
        case 11:
        case 14:
        case 23:
            if (used_zap_source_idx == 8)
                res = 27;
            else if (used_zap_source_idx == 6)
                res = 26;
            else if (used_zap_source_idx == 7 && (layer_flags & LFLAGS_ZAP_LEADING_EDGE))
                res = 33;
            break;
        case 4:
        case 16:
            if (used_zap_source_idx == 2 && (layer_flags & LFLAGS_ZAP_LEADING_EDGE))
                res = 50;
            break;
        case 5:
        case 17:
            if (used_zap_source_idx == 8 && (layer_flags & LFLAGS_ZAP_LEADING_EDGE))
                res = 49;
            break;
        case 6:
        case 15:
        case 18:
        case 8:
            if (used_zap_source_idx == 8)
                res = 29;
            else if (used_zap_source_idx == 2)
                res = 28;
            else if (used_zap_source_idx == 1 && (layer_flags & LFLAGS_ZAP_LEADING_EDGE))
                res = 34;
            break;
        case 7:
        case 9:
        case 12:
        case 19:
            if (used_zap_source_idx == 4)
                res = 31;
            else if (used_zap_source_idx == 2)
                res = 30;
            else if (used_zap_source_idx == 3 && (layer_flags & LFLAGS_ZAP_LEADING_EDGE))
                res = 35;
            break;
        default:
            break;
        }
    }

    if (check_trailing)
    {
        /* Trailing cases */
        switch (dir_index)
        {
        case 0:
        case 16:
            if (used_zap_source_idx == 2 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 52;
            break;
        case 1:
        case 17:
            if (used_zap_source_idx == 8 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 55;
            break;
        case 2:
        case 11:
        case 12:
        case 18:
            if (used_zap_source_idx == 2 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 36;
            else if (used_zap_source_idx == 8 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 37;
            else if (used_zap_source_idx == 1 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 44;
            break;
        case 3:
        case 8:
        case 13:
        case 19:
            if (used_zap_source_idx == 2 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 38;
            else if (used_zap_source_idx == 4 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 39;
            else if (used_zap_source_idx == 3 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 45;
            break;
        case 4:
        case 20:
            if (used_zap_source_idx == 6 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 54;
            break;
        case 5:
        case 21:
            if (used_zap_source_idx == 4 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 53;
            break;
        case 6:
        case 9:
        case 14:
        case 22:
            if (used_zap_source_idx == 6 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 40;
            else if (used_zap_source_idx == 4 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 41;
            else if (used_zap_source_idx == 5 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 46;
            break;
        case 7:
        case 10:
        case 15:
        case 23:
            if (used_zap_source_idx == 6 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 42;
            else if (used_zap_source_idx == 8 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 43;
            else if (used_zap_source_idx == 7 && (layer_flags & LFLAGS_ZAP_TRAILING_EDGE))
                res = 47;
            break;
        default:
            break;
        }
    }

#if 0
    /* Trailing version */
    switch (dir_index)
    {
    case 2:
    case 11:
    case 12:
    case 18:
        if (zap_source_idx == 4)
            res = 24;
        else if (zap_source_idx == 1)
            res = 25;
        break;
    case 3:
    case 8:
    case 13:
    case 19:
        if (zap_source_idx == 2)
            res = 26;
        else if (zap_source_idx == 1)
            res = 27;
        break;
    case 6:
    case 9:
    case 14:
    case 22:
        if (zap_source_idx == 2)
            res = 28;
        else if (zap_source_idx == 3)
            res = 29;
        break;
    case 7:
    case 10:
    case 15:
    case 23:
        if (zap_source_idx == 4)
            res = 30;
        else if (zap_source_idx == 3)
            res = 31;
        break;
    default:
        break;
    }
#endif
    if(res == -1)
        return NO_GLYPH;
    else
        return res + zap_index * NUM_ZAP_CHARS + GLYPH_ZAP_OFF;
}

/*
 * Utility routine for dowhatis() used to find out the glyph displayed at
 * the location.  This isn't necessarily the same as the glyph in the levl
 * structure, so we must check the "third screen".
 */
int
glyph_at(x, y)
xchar x, y;
{
    if (x < 0 || y < 0 || x >= COLNO || y >= ROWNO)
        return cmap_to_glyph(S_unexplored); /* XXX */
    return gbuf[y][x].layers.glyph;
}

struct layer_info
layers_at(x, y)
xchar x, y;
{
    if (x < 0 || y < 0 || x >= COLNO || y >= ROWNO)
    {
        struct layer_info layers = nul_layerinfo;
        return layers;
    }
    return gbuf[y][x].layers;
}

/*
 * This will be used to get the glyph for the background so that
 * it can potentially be merged into graphical window ports to
 * improve the appearance of stuff on dark room squares and the
 * plane of air etc.
 *
 * Until that is working correctly in the branch, however, for now
 * we just return NO_GLYPH as an indicator to ignore it.
 *
 * [This should be using background as recorded for #overview rather
 * than current data from the map.]
 */

STATIC_OVL int
get_bk_glyph(x, y)
xchar x, y;
{
    if (glyph_is_specific_cmap_or_its_variation(gbuf[y][x].layers.glyph, S_unexplored))
        return NO_GLYPH;
    else
        return back_to_glyph(x, y);

}

/* Floor layer glyph for possibly transparent feature layer glyphs, actual opaque floor layer level location types return NO_GLYPH  */
STATIC_OVL int
get_floor_layer_glyph(x, y)
xchar x, y;
{
    int idx;
    struct rm* ptr = &(levl[x][y]);
    int cmap_type = levl[x][y].use_special_tileset ? levl[x][y].special_tileset : get_current_cmap_type_index();

    if (ptr->floortyp && IS_FLOOR(ptr->floortyp))
    {
        idx = location_type_definitions[ptr->floortyp].base_screen_symbol;
        if (ptr->floorsubtyp > 0 || ptr->floorvartyp > 0)
        {
            int subtyp_offset = 0;
            if (ptr->floortyp == ROOM)
                subtyp_offset = floor_subtype_definitions[ptr->floorsubtyp].variation_offset;
            else if (ptr->floortyp == GRASS)
                subtyp_offset = grass_subtype_definitions[ptr->floorsubtyp].variation_offset;
            else if (ptr->floortyp == GROUND)
                subtyp_offset = ground_subtype_definitions[ptr->floorsubtyp].variation_offset;
            else if (ptr->floortyp == CORR)
                subtyp_offset = corridor_subtype_definitions[ptr->floorsubtyp].variation_offset;
            int var_offset = defsyms[idx].variation_offset;
            idx = var_offset + subtyp_offset + ptr->floorvartyp - 1;
            return cmap_variation_with_type_to_glyph(idx, cmap_type);
        }

        return cmap_with_type_to_glyph(idx, cmap_type);
    }

    switch (ptr->typ) 
    {
    case UNDEFINED_LOCATION:
    case SCORR:
    case STONE:
    case ROOM:
    case GRASS:
    case GROUND:
    case CORR:
    case HWALL:
    case VWALL:
    case TLCORNER:
    case TRCORNER:
    case BLCORNER:
    case BRCORNER:
    case CROSSWALL:
    case TUWALL:
    case TDWALL:
    case TLWALL:
    case TRWALL:
    case SDOOR:
    case LAVAPOOL:
    case ICE:
    case AIR:
    case CLOUD:
    case WATER:
    case DBWALL:
    case DRAWBRIDGE_UP:
    case MOAT:
    case POOL:
        return NO_GLYPH;
    case DOOR:
    case IRONBARS:
        idx = /* ptr->waslit ? */ S_room /* : DARKROOMSYM */;
        break;
    case TREE:
        idx = /* ptr->waslit ? */ S_grass /*: DARKGRASSSYM */;
        break;
    case LADDER:
    case STAIRS:
    case FOUNTAIN:
    case SINK:
    case ALTAR:
    case GRAVE:
    case BRAZIER:
    case SIGNPOST:
    case THRONE:
        idx = /* ptr->waslit ? */ S_room /*: DARKROOMSYM*/;
        break;
    case DRAWBRIDGE_DOWN:
        switch (ptr->drawbridgemask & DB_UNDER) 
        {
        case DB_MOAT:
            idx = S_pool;
            break;
        case DB_LAVA:
            idx = S_lava;
            break;
        case DB_ICE:
            idx = S_ice;
            break;
        case DB_GROUND:
            idx = S_ground; // /* ptr->waslit ? */ S_room /*: DARKROOMSYM*/;
            break;
        default:
            impossible("Strange db-under: %d",
                ptr->drawbridgemask & DB_UNDER);
            idx = /* ptr->waslit ? */ S_room /*: DARKROOMSYM*/; /* something is better than nothing */
            break;
        }
        break;
    default:
        impossible("get_floor_layer_glyph:  unknown level type [ = %d ]", ptr->typ);
        idx =/*  ptr->waslit ? */ S_room /* : DARKROOMSYM */;
        break;
    }

    return cmap_with_type_to_glyph(idx, cmap_type);
}

/* Floor doodad layer glyph  */
STATIC_OVL int
get_floor_doodad_layer_glyph(x, y)
xchar x, y;
{
    if (levl[x][y].floor_doodad)
        return levl[x][y].floor_doodad;

    return NO_GLYPH;
}

/* Feature doodad layer glyph  */
STATIC_OVL int
get_feature_doodad_layer_glyph(x, y, height_ptr)
xchar x, y;
schar* height_ptr;
{
    if (!isok(x, y))
        return NO_GLYPH;

    if (levl[x][y].decoration_typ > 0)
    {
        boolean decoration_seen = levl[x][y].decoration_dir < 0
            || (levl[x][y].decoration_dir == 0 && (levl[x][y].seenv & (SV3 | SV4 | SV5 | SV6 | SV7)))
            || (levl[x][y].decoration_dir == 1 && (levl[x][y].seenv & (SV1 | SV2 | SV3 | SV4 | SV5)))
            || (levl[x][y].decoration_dir == 2 && (levl[x][y].seenv & (SV5 | SV6 | SV7 | SV0 | SV1)))
            || (levl[x][y].decoration_dir == 3 && (levl[x][y].seenv & (SV7 | SV0 | SV1 | SV2 | SV3)))
            ;
        int glyph = NO_GLYPH;
        if (decoration_seen)
        {
            if (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_MIRRORABLE)
            {
                glyph = levl[x][y].decoration_dir + (decoration_type_definitions[levl[x][y].decoration_typ].first_doodad[levl[x][y].decoration_dir] + levl[x][y].decoration_subtyp) * NUM_DOODAD_MIRRORINGS + GLYPH_MIRRORABLE_DOODAD_OFF;
                if (height_ptr)
                    *height_ptr = mirrorable_doodads[decoration_type_definitions[levl[x][y].decoration_typ].first_doodad[levl[x][y].decoration_dir] + levl[x][y].decoration_subtyp].special_height;
            }
            else
            {
                if (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_NO_SUBTYP_OFFSET)
                {
                    glyph = decoration_type_definitions[levl[x][y].decoration_typ].first_doodad[levl[x][y].decoration_dir] + GLYPH_SIMPLE_DOODAD_OFF;
                    if (height_ptr)
                        *height_ptr = simple_doodads[decoration_type_definitions[levl[x][y].decoration_typ].first_doodad[levl[x][y].decoration_dir]].special_height;
                }
                else
                {
                    glyph = decoration_type_definitions[levl[x][y].decoration_typ].first_doodad[levl[x][y].decoration_dir] + levl[x][y].decoration_subtyp + GLYPH_SIMPLE_DOODAD_OFF;
                    if (height_ptr)
                        *height_ptr = simple_doodads[decoration_type_definitions[levl[x][y].decoration_typ].first_doodad[levl[x][y].decoration_dir] + levl[x][y].decoration_subtyp].special_height;
                }
            }
        }
        return glyph;
    }
    else
    {
        if (height_ptr)
            *height_ptr = 0;
        return NO_GLYPH;
    }
}

/* Carpet layer glyph  */
STATIC_OVL int
get_carpet_layer_glyph(x, y)
xchar x, y;
{
    if (!isok(x, y))
        return NO_GLYPH;

    if (levl[x][y].carpet_typ > 0 && levl[x][y].carpet_typ < MAX_CARPETS)
    {
        int glyph = NO_GLYPH;
        glyph = carpet_type_definitions[levl[x][y].carpet_typ].first_doodad + levl[x][y].carpet_piece;
        return glyph;
    }
    else
    {
        return NO_GLYPH;
    }
}

/* ------------------------------------------------------------------------ */
/* Wall Angle ------------------------------------------------------------- */

#ifdef WA_VERBOSE

STATIC_DCL const char *FDECL(type_to_name, (int));
STATIC_DCL void FDECL(error4, (int, int, int, int, int, int));

STATIC_VAR int bad_count[MAX_TYPE]; /* count of positions flagged as bad */

STATIC_DCL const char *
type_to_name(type)
int type;
{
    return (type < 0 || type >= MAX_TYPE) ? "unknown" : location_type_definitions[type].name;
}

STATIC_DCL void
error4(x, y, a, b, c, dd)
int x, y, a, b, c, dd;
{
    pline("set_wall_state: %s @ (%d,%d) %s%s%s%s",
          type_to_name(levl[x][y].typ), x, y,
          a ? "1" : "", b ? "2" : "", c ? "3" : "", dd ? "4" : "");
    bad_count[levl[x][y].typ]++;
}
#endif /* WA_VERBOSE */

/*
 * Return 'which' if position is implies an unfinished exterior.  Return
 * zero otherwise.  Unfinished implies outer area is rock or a corridor.
 *
 * Things that are ambiguous: lava
 */
STATIC_OVL int
check_pos(x, y, which)
int x, y, which;
{
    int type;

    if (!isok(x, y))
        return which;
    type = levl[x][y].typ;
    if ((IS_ROCK(type) && !IS_TREE(type)) || type == CORR || type == SCORR)
        return which;
    return 0;
}

/* Return TRUE if more than one is non-zero. */
/*ARGSUSED*/
#ifdef WA_VERBOSE
STATIC_OVL boolean
more_than_one(x, y, a, b, c)
int x, y, a, b, c;
{
    if ((a && (b | c)) || (b && (a | c)) || (c && (a | b))) {
        error4(x, y, a, b, c, 0);
        return TRUE;
    }
    return FALSE;
}
#else
#define more_than_one(x, y, a, b, c) \
    (((a) && ((b) | (c))) || ((b) && ((a) | (c))) || ((c) && ((a) | (b))))
#endif

/* Return the wall mode for a T wall. */
STATIC_OVL int
set_twall(x0, y0, x1, y1, x2, y2, x3, y3)
int x0, y0; /* used #if WA_VERBOSE */
int x1, y1, x2, y2, x3, y3;
{
    int wmode, is_1, is_2, is_3;

#ifndef WA_VERBOSE
    /* non-verbose more_than_one() doesn't use these */
    nhUse(x0);
    nhUse(y0);
#endif

    is_1 = check_pos(x1, y1, WM_T_LONG);
    is_2 = check_pos(x2, y2, WM_T_BL);
    is_3 = check_pos(x3, y3, WM_T_BR);
    if (more_than_one(x0, y0, is_1, is_2, is_3)) {
        wmode = 0;
    } else {
        wmode = is_1 + is_2 + is_3;
    }
    return wmode;
}

/* Return wall mode for a horizontal or vertical wall. */
STATIC_OVL int
set_wall(x, y, horiz)
int x, y, horiz;
{
    int wmode, is_1, is_2;

    if (horiz) {
        is_1 = check_pos(x, y - 1, WM_W_TOP);
        is_2 = check_pos(x, y + 1, WM_W_BOTTOM);
    } else {
        is_1 = check_pos(x - 1, y, WM_W_LEFT);
        is_2 = check_pos(x + 1, y, WM_W_RIGHT);
    }
    if (more_than_one(x, y, is_1, is_2, 0)) {
        wmode = 0;
    } else {
        wmode = is_1 + is_2;
    }
    return wmode;
}

/* Return a wall mode for a corner wall. (x4,y4) is the "inner" position. */
STATIC_OVL int
set_corn(x1, y1, x2, y2, x3, y3, x4, y4)
int x1, y1, x2, y2, x3, y3, x4, y4;
{
    int wmode, is_1, is_2, is_3, is_4;

    is_1 = check_pos(x1, y1, 1);
    is_2 = check_pos(x2, y2, 1);
    is_3 = check_pos(x3, y3, 1);
    is_4 = check_pos(x4, y4, 1); /* inner location */

    /*
     * All 4 should not be true.  So if the inner location is rock,
     * use it.  If all of the outer 3 are true, use outer.  We currently
     * can't cover the case where only part of the outer is rock, so
     * we just say that all the walls are finished (if not overridden
     * by the inner section).
     */
    if (is_4) {
        wmode = WM_C_INNER;
    } else if (is_1 && is_2 && is_3)
        wmode = WM_C_OUTER;
    else
        wmode = 0; /* finished walls on all sides */

    return wmode;
}

/* Return mode for a crosswall. */
STATIC_OVL int
set_crosswall(x, y)
int x, y;
{
    int wmode, is_1, is_2, is_3, is_4;

    is_1 = check_pos(x - 1, y - 1, 1);
    is_2 = check_pos(x + 1, y - 1, 1);
    is_3 = check_pos(x + 1, y + 1, 1);
    is_4 = check_pos(x - 1, y + 1, 1);

    wmode = is_1 + is_2 + is_3 + is_4;
    if (wmode > 1) {
        if (is_1 && is_3 && (is_2 + is_4 == 0)) {
            wmode = WM_X_TLBR;
        } else if (is_2 && is_4 && (is_1 + is_3 == 0)) {
            wmode = WM_X_BLTR;
        } else {
#ifdef WA_VERBOSE
            error4(x, y, is_1, is_2, is_3, is_4);
#endif
            wmode = 0;
        }
    } else if (is_1)
        wmode = WM_X_TL;
    else if (is_2)
        wmode = WM_X_TR;
    else if (is_3)
        wmode = WM_X_BR;
    else if (is_4)
        wmode = WM_X_BL;

    return wmode;
}

/* Called from mklev.  Scan the level and set the wall modes. */
void
set_wall_state()
{
    int x, y;
    int wmode;
    struct rm *lev;

#ifdef WA_VERBOSE
    for (x = 0; x < MAX_TYPE; x++)
        bad_count[x] = 0;
#endif

    for (x = 0; x < COLNO; x++)
        for (lev = &levl[x][0], y = 0; y < ROWNO; y++, lev++) {
            switch (lev->typ) {
            case SDOOR:
                wmode = set_wall(x, y, (int) lev->horizontal);
                break;
            case VWALL:
                wmode = set_wall(x, y, 0);
                break;
            case HWALL:
                wmode = set_wall(x, y, 1);
                break;
            case TDWALL:
                wmode = set_twall(x, y, x, y - 1, x - 1, y + 1, x + 1, y + 1);
                break;
            case TUWALL:
                wmode = set_twall(x, y, x, y + 1, x + 1, y - 1, x - 1, y - 1);
                break;
            case TLWALL:
                wmode = set_twall(x, y, x + 1, y, x - 1, y - 1, x - 1, y + 1);
                break;
            case TRWALL:
                wmode = set_twall(x, y, x - 1, y, x + 1, y + 1, x + 1, y - 1);
                break;
            case TLCORNER:
                wmode =
                    set_corn(x - 1, y - 1, x, y - 1, x - 1, y, x + 1, y + 1);
                break;
            case TRCORNER:
                wmode =
                    set_corn(x, y - 1, x + 1, y - 1, x + 1, y, x - 1, y + 1);
                break;
            case BLCORNER:
                wmode =
                    set_corn(x, y + 1, x - 1, y + 1, x - 1, y, x + 1, y - 1);
                break;
            case BRCORNER:
                wmode =
                    set_corn(x + 1, y, x + 1, y + 1, x, y + 1, x - 1, y - 1);
                break;
            case CROSSWALL:
                wmode = set_crosswall(x, y);
                break;

            default:
                wmode = -1; /* don't set wall info */
                break;
            }

            if (wmode >= 0)
                lev->wall_info = (lev->wall_info & ~WM_MASK) | wmode;
        }

#ifdef WA_VERBOSE
    /* check if any bad positions found */
    for (x = y = 0; x < MAX_TYPE; x++)
        if (bad_count[x]) {
            if (y == 0) {
                y = 1; /* only print once */
                pline("set_wall_type: wall mode problems with: ");
            }
            pline("%s %d;", location_type_definitions[x].name, bad_count[x]);
        }
#endif /* WA_VERBOSE */
}

/* ------------------------------------------------------------------------ */
/* This matrix is used here and in vision.c. */
unsigned char seenv_matrix[3][3] = { { SV2, SV1, SV0 },
                                     { SV3, SVALL, SV7 },
                                     { SV4, SV5, SV6 } };

#define sign(z) ((z) < 0 ? -1 : ((z) > 0 ? 1 : 0))

/* Set the seen vector of lev as if seen from (x0,y0) to (x,y). */
STATIC_OVL void
set_seenv(lev, x0, y0, x, y)
struct rm *lev;
int x0, y0, x, y; /* from, to */
{
    int dx = x - x0, dy = y0 - y;

    lev->seenv |= seenv_matrix[sign(dy) + 1][sign(dx) + 1];
}

/* Called by blackout(vault.c) when vault guard removes temporary corridor,
   turning spot <x0,y0> back into stone; <x1,y1> is an adjacent spot. */
void
unset_seenv(lev, x0, y0, x1, y1)
struct rm *lev;     /* &levl[x1][y1] */
int x0, y0, x1, y1; /* from, to; abs(x1-x0)==1 && abs(y0-y1)==1 */
{
    int dx = x1 - x0, dy = y0 - y1;

    lev->seenv &= ~seenv_matrix[dy + 1][dx + 1];
}

/* ------------------------------------------------------------------------ */

/* T wall types, one for each row in wall_matrix[][]. */
#define T_d 0
#define T_l 1
#define T_u 2
#define T_r 3

/*
 * These are the column names of wall_matrix[][].  They are the "results"
 * of a tdwall pattern match.  All T walls are rotated so they become
 * a tdwall.  Then we do a single pattern match, but return the
 * correct result for the original wall by using different rows for
 * each of the wall types.
 */
#define T_stone 0
#define T_tlcorn 1
#define T_trcorn 2
#define T_hwall 3
#define T_tdwall 4

STATIC_VAR const int wall_matrix[4][5] = {
    { S_stone, S_tlcorn, S_trcorn, S_hwall, S_tdwall }, /* tdwall */
    { S_stone, S_trcorn, S_brcorn, S_vwall, S_tlwall }, /* tlwall */
    { S_stone, S_brcorn, S_blcorn, S_hwall, S_tuwall }, /* tuwall */
    { S_stone, S_blcorn, S_tlcorn, S_vwall, S_trwall }, /* trwall */
};

/* Cross wall types, one for each "solid" quarter.  Rows of cross_matrix[][].
 */
#define C_bl 0
#define C_tl 1
#define C_tr 2
#define C_br 3

/*
 * These are the column names for cross_matrix[][].  They express results
 * in C_br (bottom right) terms.  All crosswalls with a single solid
 * quarter are rotated so the solid section is at the bottom right.
 * We pattern match on that, but return the correct result depending
 * on which row we'ere looking at.
 */
#define C_trcorn 0
#define C_brcorn 1
#define C_blcorn 2
#define C_tlwall 3
#define C_tuwall 4
#define C_crwall 5

STATIC_VAR const int cross_matrix[4][6] = {
    { S_brcorn, S_blcorn, S_tlcorn, S_tuwall, S_trwall, S_crwall },
    { S_blcorn, S_tlcorn, S_trcorn, S_trwall, S_tdwall, S_crwall },
    { S_tlcorn, S_trcorn, S_brcorn, S_tdwall, S_tlwall, S_crwall },
    { S_trcorn, S_brcorn, S_blcorn, S_tlwall, S_tuwall, S_crwall },
};

/* Print out a T wall warning and all interesting info. */
STATIC_OVL void
t_warn(lev)
struct rm *lev;
{
    STATIC_VAR const char warn_str[] = "wall_angle: %s: case %d: seenv = 0x%x";
    const char *wname;

    if (lev->typ == TUWALL)
        wname = "tuwall";
    else if (lev->typ == TLWALL)
        wname = "tlwall";
    else if (lev->typ == TRWALL)
        wname = "trwall";
    else if (lev->typ == TDWALL)
        wname = "tdwall";
    else
        wname = "unknown";
    impossible(warn_str, wname, lev->wall_info & WM_MASK,
               (unsigned int) lev->seenv);
}

/*
 * Return the correct graphics character index using wall type, wall mode,
 * and the seen vector.  It is expected that seenv is non zero.
 *
 * All T-wall vectors are rotated to be TDWALL.  All single crosswall
 * blocks are rotated to bottom right.  All double crosswall are rotated
 * to W_X_BLTR.  All results are converted back.
 *
 * The only way to understand this is to take out pen and paper and
 * draw diagrams.  See rm.h for more details on the wall modes and
 * seen vector (SV).
 */
int
wall_angle(lev)
struct rm *lev;
{
    register unsigned int seenv = lev->seenv & 0xff;
    const int *row;
    int col, idx;

#define only(sv, bits) (((sv) & (bits)) && !((sv) & ~(bits)))
    switch (lev->typ) {
    case TUWALL:
        row = wall_matrix[T_u];
        seenv = (seenv >> 4 | seenv << 4) & 0xff; /* rotate to tdwall */
        goto do_twall;
    case TLWALL:
        row = wall_matrix[T_l];
        seenv = (seenv >> 2 | seenv << 6) & 0xff; /* rotate to tdwall */
        goto do_twall;
    case TRWALL:
        row = wall_matrix[T_r];
        seenv = (seenv >> 6 | seenv << 2) & 0xff; /* rotate to tdwall */
        goto do_twall;
    case TDWALL:
        row = wall_matrix[T_d];
 do_twall:
        switch (lev->wall_info & WM_MASK) {
        case 0:
            if (seenv == SV4) {
                col = T_tlcorn;
            } else if (seenv == SV6) {
                col = T_trcorn;
            } else if (seenv & (SV3 | SV5 | SV7)
                       || ((seenv & SV4) && (seenv & SV6))) {
                col = T_tdwall;
            } else if (seenv & (SV0 | SV1 | SV2)) {
                col = (seenv & (SV4 | SV6) ? T_tdwall : T_hwall);
            } else {
                t_warn(lev);
                col = T_stone;
            }
            break;
        case WM_T_LONG:
            if (seenv & (SV3 | SV4) && !(seenv & (SV5 | SV6 | SV7))) {
                col = T_tlcorn;
            } else if (seenv & (SV6 | SV7) && !(seenv & (SV3 | SV4 | SV5))) {
                col = T_trcorn;
            } else if ((seenv & SV5)
                       || ((seenv & (SV3 | SV4)) && (seenv & (SV6 | SV7)))) {
                col = T_tdwall;
            } else {
                /* only SV0|SV1|SV2 */
                if (!only(seenv, SV0 | SV1 | SV2))
                    t_warn(lev);
                col = T_stone;
            }
            break;
        case WM_T_BL:
#if 0  /* older method, fixed */
            if (only(seenv, SV4 | SV5)) {
                col = T_tlcorn;
            } else if ((seenv & (SV0 | SV1 | SV2))
                       && only(seenv, SV0 | SV1 | SV2 | SV6 | SV7)) {
                col = T_hwall;
            } else if ((seenv & SV3)
                       || ((seenv & (SV0 | SV1 | SV2))
                           && (seenv & (SV4 | SV5)))) {
                col = T_tdwall;
            } else {
                if (seenv != SV6)
                    t_warn(lev);
                col = T_stone;
            }
#endif /* 0 */
            if (only(seenv, SV4 | SV5))
                col = T_tlcorn;
            else if ((seenv & (SV0 | SV1 | SV2 | SV7))
                     && !(seenv & (SV3 | SV4 | SV5)))
                col = T_hwall;
            else if (only(seenv, SV6))
                col = T_stone;
            else
                col = T_tdwall;
            break;
        case WM_T_BR:
#if 0  /* older method, fixed */
            if (only(seenv, SV5 | SV6)) {
                col = T_trcorn;
            } else if ((seenv & (SV0 | SV1 | SV2))
                       && only(seenv, SV0 | SV1 | SV2 | SV3 | SV4)) {
                col = T_hwall;
            } else if ((seenv & SV7)
                       || ((seenv & (SV0 | SV1 | SV2))
                           && (seenv & (SV5 | SV6)))) {
                col = T_tdwall;
            } else {
                if (seenv != SV4)
                    t_warn(lev);
                col = T_stone;
            }
#endif /* 0 */
            if (only(seenv, SV5 | SV6))
                col = T_trcorn;
            else if ((seenv & (SV0 | SV1 | SV2 | SV3))
                     && !(seenv & (SV5 | SV6 | SV7)))
                col = T_hwall;
            else if (only(seenv, SV4))
                col = T_stone;
            else
                col = T_tdwall;

            break;
        default:
            impossible("wall_angle: unknown T wall mode %d",
                       lev->wall_info & WM_MASK);
            col = T_stone;
            break;
        }
        idx = row[col];
        break;

    case SDOOR:
        if (lev->horizontal)
            goto horiz;
        /*FALLTHRU*/
    case VWALL:
        switch (lev->wall_info & WM_MASK) {
        case 0:
            idx = seenv ? S_vwall : S_stone;
            break;
        case 1:
            idx = seenv & (SV1 | SV2 | SV3 | SV4 | SV5) ? S_vwall : S_stone;
            break;
        case 2:
            idx = seenv & (SV0 | SV1 | SV5 | SV6 | SV7) ? S_vwall : S_stone;
            break;
        default:
            impossible("wall_angle: unknown vwall mode %d",
                       lev->wall_info & WM_MASK);
            idx = S_stone;
            break;
        }
        break;

    case HWALL:
 horiz:
        switch (lev->wall_info & WM_MASK) {
        case 0:
            idx = seenv ? S_hwall : S_stone;
            break;
        case 1:
            idx = seenv & (SV3 | SV4 | SV5 | SV6 | SV7) ? S_hwall : S_stone;
            break;
        case 2:
            idx = seenv & (SV0 | SV1 | SV2 | SV3 | SV7) ? S_hwall : S_stone;
            break;
        default:
            impossible("wall_angle: unknown hwall mode %d",
                       lev->wall_info & WM_MASK);
            idx = S_stone;
            break;
        }
        break;

#define set_corner(idx, lev, which, outer, inner, name)    \
    switch ((lev)->wall_info & WM_MASK) {                  \
    case 0:                                                \
        idx = which;                                       \
        break;                                             \
    case WM_C_OUTER:                                       \
        idx = seenv & (outer) ? which : S_stone;           \
        break;                                             \
    case WM_C_INNER:                                       \
        idx = seenv & ~(inner) ? which : S_stone;          \
        break;                                             \
    default:                                               \
        impossible("wall_angle: unknown %s mode %d", name, \
                   (lev)->wall_info &WM_MASK);             \
        idx = S_stone;                                     \
        break;                                             \
    }

    case TLCORNER:
        set_corner(idx, lev, S_tlcorn, (SV3 | SV4 | SV5), SV4, "tlcorn");
        break;
    case TRCORNER:
        set_corner(idx, lev, S_trcorn, (SV5 | SV6 | SV7), SV6, "trcorn");
        break;
    case BLCORNER:
        set_corner(idx, lev, S_blcorn, (SV1 | SV2 | SV3), SV2, "blcorn");
        break;
    case BRCORNER:
        set_corner(idx, lev, S_brcorn, (SV7 | SV0 | SV1), SV0, "brcorn");
        break;

    case CROSSWALL:
        switch (lev->wall_info & WM_MASK) {
        case 0:
            if (seenv == SV0)
                idx = S_brcorn;
            else if (seenv == SV2)
                idx = S_blcorn;
            else if (seenv == SV4)
                idx = S_tlcorn;
            else if (seenv == SV6)
                idx = S_trcorn;
            else if (!(seenv & ~(SV0 | SV1 | SV2))
                     && (seenv & SV1 || seenv == (SV0 | SV2)))
                idx = S_tuwall;
            else if (!(seenv & ~(SV2 | SV3 | SV4))
                     && (seenv & SV3 || seenv == (SV2 | SV4)))
                idx = S_trwall;
            else if (!(seenv & ~(SV4 | SV5 | SV6))
                     && (seenv & SV5 || seenv == (SV4 | SV6)))
                idx = S_tdwall;
            else if (!(seenv & ~(SV0 | SV6 | SV7))
                     && (seenv & SV7 || seenv == (SV0 | SV6)))
                idx = S_tlwall;
            else
                idx = S_crwall;
            break;

        case WM_X_TL:
            row = cross_matrix[C_tl];
            seenv = (seenv >> 4 | seenv << 4) & 0xff;
            goto do_crwall;
        case WM_X_TR:
            row = cross_matrix[C_tr];
            seenv = (seenv >> 6 | seenv << 2) & 0xff;
            goto do_crwall;
        case WM_X_BL:
            row = cross_matrix[C_bl];
            seenv = (seenv >> 2 | seenv << 6) & 0xff;
            goto do_crwall;
        case WM_X_BR:
            row = cross_matrix[C_br];
 do_crwall:
            if (seenv == SV4) {
                idx = S_stone;
            } else {
                seenv = seenv & ~SV4; /* strip SV4 */
                if (seenv == SV0) {
                    col = C_brcorn;
                } else if (seenv & (SV2 | SV3)) {
                    if (seenv & (SV5 | SV6 | SV7))
                        col = C_crwall;
                    else if (seenv & (SV0 | SV1))
                        col = C_tuwall;
                    else
                        col = C_blcorn;
                } else if (seenv & (SV5 | SV6)) {
                    if (seenv & (SV1 | SV2 | SV3))
                        col = C_crwall;
                    else if (seenv & (SV0 | SV7))
                        col = C_tlwall;
                    else
                        col = C_trcorn;
                } else if (seenv & SV1) {
                    col = seenv & SV7 ? C_crwall : C_tuwall;
                } else if (seenv & SV7) {
                    col = seenv & SV1 ? C_crwall : C_tlwall;
                } else {
                    impossible("wall_angle: bottom of crwall check");
                    col = C_crwall;
                }

                idx = row[col];
            }
            break;

        case WM_X_TLBR:
            if (only(seenv, SV1 | SV2 | SV3))
                idx = S_blcorn;
            else if (only(seenv, SV5 | SV6 | SV7))
                idx = S_trcorn;
            else if (only(seenv, SV0 | SV4))
                idx = S_stone;
            else
                idx = S_crwall;
            break;

        case WM_X_BLTR:
            if (only(seenv, SV0 | SV1 | SV7))
                idx = S_brcorn;
            else if (only(seenv, SV3 | SV4 | SV5))
                idx = S_tlcorn;
            else if (only(seenv, SV2 | SV6))
                idx = S_stone;
            else
                idx = S_crwall;
            break;

        default:
            impossible("wall_angle: unknown crosswall mode");
            idx = S_stone;
            break;
        }
        break;

    default:
        impossible("wall_angle: unexpected wall type %d", lev->typ);
        idx = S_stone;
    }
    return idx;
}

int
get_current_cmap_type_index()
{
    if (level.flags.has_tileset)
        return level.flags.tileset;
    else if (dungeons[u.uz.dnum].flags.has_tileset)
        return dungeons[u.uz.dnum].flags.tileset;
    else if (In_mines(&u.uz) || Is_quantum_level(&u.uz))
        return CMAP_GNOMISH_MINES;
    else if (Is_valley(&u.uz) || Is_orcus_level(&u.uz) || Is_sanctum(&u.uz) || In_V_tower(&u.uz))
        return CMAP_UNDEAD;
    else if (In_modron_level(&u.uz))
        return CMAP_MODRON;
    else if (In_sokoban(&u.uz))
        return CMAP_SOKOBAN;
    else if (On_W_tower_level(&u.uz))
        return CMAP_TOWER;
    else if (Is_astralevel(&u.uz))
        return CMAP_ASTRAL;
    else if (In_endgame(&u.uz))
        return CMAP_ELEMENTAL_PLANES;
    else if (Inhell)
        return CMAP_GEHENNOM;
    else
        return CMAP_NORMAL;
}

int
get_missile_index(dx, dy)
int dx, dy;
{
    if (!dx && !dy)
        return 0; /* really invalid */

    if (dy < 0)
        return sgn(dx) + 1;
    else if (dy == 0 && dx < 0)
        return 3;
    else if (dy == 0 && dx > 0)
        return 4;
    else if (dy > 0)
    return sgn(dx) + 6;

    /* Not reached */
    return 0;
}


void
display_self_with_extra_info_choose_ascii(displayed_flags, hit_tile_id, dmg_received, exclude_ascii)
unsigned long displayed_flags;
int hit_tile_id, dmg_received;
boolean exclude_ascii;
{

    int cmap_type = levl[u.ux][u.uy].use_special_tileset ? levl[u.ux][u.uy].special_tileset : get_current_cmap_type_index();
    show_monster_glyph_with_extra_info_choose_ascii(u.ux, u.uy,
        maybe_display_usteed((U_AP_TYPE == M_AP_NOTHING)
            ? u_to_glyph() /*hero_glyph*/
            : (U_AP_TYPE == M_AP_FURNITURE)
            ? cmap_with_type_to_glyph(youmonst.mappearance, cmap_type)
            : (U_AP_TYPE == M_AP_OBJECT)
            ? objnum_to_glyph(youmonst.mappearance)
            /* else U_AP_TYPE == M_AP_MONSTER */
            : any_monnum_to_glyph(flags.female, youmonst.mappearance)
        ),
        u.usteed, u.ux0, u.uy0,
        displayed_flags | LFLAGS_M_YOU | (u.usteed && mon_visible(u.usteed) ? LFLAGS_M_RIDDEN : 0UL),
        hit_tile_id, dmg_received, exclude_ascii);

}

void
u_shieldeff()
{
    //enum action_tile_types action_before = u.action;
    //update_u_action_and_wait(ACTION_TILE_PASSIVE_DEFENSE);
    shieldeff(u.ux, u.uy);
    //update_u_action_revert(action_before);
}

void
m_shieldeff(mon)
struct monst* mon;
{
    if (!mon || !cansee(mon->mx, mon->my)) /* Blanket check that shield effect is no played for nothing */
        return;

    //enum action_tile_types action_before = mon->action;
    //update_m_action_and_wait(mon, ACTION_TILE_PASSIVE_DEFENSE);
    shieldeff(mon->mx, mon->my);
    //update_m_action_revert(mon, action_before);
}

void
set_obj_glyph(otmp)
struct obj* otmp;
{
    if (!otmp)
        return;

    xchar x = 0, y = 0;
    get_obj_location(otmp, &x, &y, CONTAINED_TOO | BURIED_TOO);
    int glyph = obj_to_glyph(otmp, rn2_on_display_rng);
    int gui_glyph = maybe_get_replaced_glyph(glyph, x, y, data_to_replacement_info(glyph, is_obj_drawn_in_front(otmp) ? LAYER_COVER_OBJECT : LAYER_OBJECT, otmp, (struct monst*)0, 0UL, 0UL, MAT_NONE, 0));
    otmp->glyph = glyph;
    otmp->gui_glyph = gui_glyph;
}

int
get_seen_monster_glyph(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return NO_GLYPH;

    int glyph = any_seen_mon_to_glyph(mtmp, rn2_on_display_rng);
    int gui_glyph = maybe_get_replaced_glyph(glyph, mtmp->mx, mtmp->my, data_to_replacement_info(glyph, LAYER_MONSTER, (struct obj*)0, mtmp, 0UL, 0UL, MAT_NONE, 0));

    return iflags.using_gui_tiles ? gui_glyph : glyph;
}

const char*
get_decoration_description(x, y)
int x, y;
{
    static char decoration_buf[BUFSZ] = "";;
    if (isok(x, y))
    {
        if ((gbuf[y][x].layers.layer_flags & LFLAGS_C_DECORATION) != 0 && levl[x][y].decoration_typ > 0)
        {
            if ((levl[x][y].decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0 && decoration_type_definitions[levl[x][y].decoration_typ].description_filled != 0)
            {
                Strcpy(decoration_buf, decoration_type_definitions[levl[x][y].decoration_typ].description_filled);
                if (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_PAINTING_DESCR)
                {
                    if (levl[x][y].decoration_subtyp >= 0 && levl[x][y].decoration_subtyp < MAX_PAINTINGS)
                    {
                        if ((levl[x][y].decoration_flags & DECORATION_FLAGS_SEEN) != 0 || cansee(x, y))
                        {
                            levl[x][y].decoration_flags |= DECORATION_FLAGS_SEEN;
                            if (painting_definitions[levl[x][y].decoration_subtyp].description)
                            {
                                Strcat(decoration_buf, " of ");
                                Strcat(decoration_buf, painting_definitions[levl[x][y].decoration_subtyp].description);
                            }
                            if (painting_definitions[levl[x][y].decoration_subtyp].artist)
                            {
                                Strcat(decoration_buf, " by ");
                                Strcat(decoration_buf, painting_definitions[levl[x][y].decoration_subtyp].artist);
                            }
                        }
                    }
                }           
                else if (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_BANNER_DESCR)
                {
                    if (levl[x][y].decoration_subtyp >= 0 && levl[x][y].decoration_subtyp < MAX_BANNERS)
                    {
                        if ((levl[x][y].decoration_flags & DECORATION_FLAGS_SEEN) != 0 || cansee(x, y))
                        {
                            levl[x][y].decoration_flags |= DECORATION_FLAGS_SEEN;
                            if (banner_definitions[levl[x][y].decoration_subtyp].description)
                            {
                                Strcpy(decoration_buf, banner_definitions[levl[x][y].decoration_subtyp].description);
                            }
                        }
                    }
                }
            }
            else if (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_WALL_SCULPTURE_DESCR)
            {
                if (levl[x][y].decoration_subtyp >= 0 && levl[x][y].decoration_subtyp < MAX_WALL_SCULPTURES)
                {
                    if (wall_sculpture_definitions[levl[x][y].decoration_subtyp].description)
                    {
                        Strcpy(decoration_buf, wall_sculpture_definitions[levl[x][y].decoration_subtyp].description);
                    }
                }
            }
            else
            {
                Strcpy(decoration_buf, decoration_type_definitions[levl[x][y].decoration_typ].description);
            }
            return decoration_buf;
        }
    }
    return 0;
}

const char*
get_carpet_description(x, y)
int x, y;
{
    if (isok(x, y))
    {
        if ((gbuf[y][x].layers.layer_flags & LFLAGS_C_CARPET) != 0 && levl[x][y].carpet_typ > 0)
        {
            return carpet_type_definitions[levl[x][y].carpet_typ].description;
        }
    }
    return 0;
}

/*display.c*/
