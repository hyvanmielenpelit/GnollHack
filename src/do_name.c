/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-07 */

/* GnollHack 4.0    do_name.c    $NHDT-Date: 1555627306 2019/04/18 22:41:46 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.145 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Pasi Kallinen, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_DCL char *NDECL(nextmbuf);
STATIC_DCL void FDECL(getpos_help, (BOOLEAN_P, const char *));
STATIC_DCL int FDECL(CFDECLSPEC cmp_coord_distu, (const void *, const void *));
STATIC_DCL boolean FDECL(gather_locs_interesting, (int, int, int));
STATIC_DCL void FDECL(gather_locs, (coord **, int *, int));
STATIC_DCL int FDECL(gloc_filter_floodfill_matcharea, (int, int));
STATIC_DCL void FDECL(auto_describe, (int, int));
STATIC_DCL void NDECL(do_mname);
STATIC_DCL boolean FDECL(alreadynamed, (struct monst *, char *, char *));
STATIC_DCL void FDECL(do_uoname, (struct obj *));
STATIC_PTR char *FDECL(docall_xname, (struct obj *));
STATIC_DCL void NDECL(namefloorobj);
STATIC_DCL char *FDECL(bogusmon, (char *,char *));
STATIC_DCL void FDECL(print_catalogue, (winid, struct obj*, int, unsigned long));
STATIC_DCL void FDECL(print_artifact_catalogue, (winid, struct obj*));
STATIC_DCL int FDECL(CFDECLSPEC citemsortcmp, (const void*, const void*));
STATIC_DCL int FDECL(CFDECLSPEC artilistsortcmp, (const void*, const void*));
STATIC_DCL const char* FDECL(gettitle, (short*, const char* const*, int, int, unsigned long, unsigned long));

extern const char what_is_an_unknown_object[]; /* from pager.c */

#define NUMMBUF 5

/* manage a pool of BUFSZ buffers, so callers don't have to */
STATIC_OVL char *
nextmbuf()
{
    static char NEARDATA bufs[NUMMBUF][BUFSZ];
    static int bufidx = 0;

    bufidx = (bufidx + 1) % NUMMBUF;
    return bufs[bufidx];
}

/* function for getpos() to highlight desired map locations.
 * parameter value 0 = initialize, 1 = highlight, 2 = done
 */
STATIC_DCL void FDECL((*getpos_hilitefunc), (int)) = (void FDECL((*), (int))) 0;
STATIC_DCL boolean FDECL((*getpos_getvalid), (int, int)) =
                                           (boolean FDECL((*), (int, int))) 0;

void
getpos_sethilite(gp_hilitef, gp_getvalidf)
void FDECL((*gp_hilitef), (int));
boolean FDECL((*gp_getvalidf), (int, int));
{
    getpos_hilitefunc = gp_hilitef;
    getpos_getvalid = gp_getvalidf;
}

STATIC_VAR const char *const gloc_descr[NUM_GLOCS][4] = {
    { "any monsters", "monster", "next/previous monster", "monsters" },
    { "any items", "item", "next/previous object", "objects" },
    { "any doors", "door", "next/previous door or doorway", "doors or doorways" },
    { "any unexplored areas", "unexplored area", "unexplored location",
      "unexplored locations" },
    { "anything interesting", "interesting thing", "anything interesting",
      "anything interesting" },
    { "any valid locations", "valid location", "valid location",
      "valid locations" }
};

STATIC_VAR const char *const gloc_filtertxt[NUM_GFILTER] = {
    "",
    " in view",
    " in this area"
};

void
getpos_help_keyxhelp(tmpwin, k1, k2, gloc)
winid tmpwin;
const char *k1;
const char *k2;
int gloc;
{
    char sbuf[BUFSZ];

    Sprintf(sbuf, "Use '%s'/'%s' to %s%s%s.",
            k1, k2,
            iflags.getloc_usemenu ? "get a menu of "
                                  : "move the cursor to ",
            gloc_descr[gloc][2 + iflags.getloc_usemenu],
            gloc_filtertxt[iflags.getloc_filter]);
    putstr(tmpwin, 0, sbuf);
}

/* the response for '?' help request in getpos() */
STATIC_OVL void
getpos_help(force, goal)
boolean force;
const char *goal;
{
    static const char *const fastmovemode[2] = { "8 units at a time",
                                                 "skipping same glyphs" };
    char sbuf[BUFSZ];
    boolean doing_what_is;
    winid tmpwin = create_nhwindow(NHW_MENU);

    Sprintf(sbuf,
            "Use '%c', '%c', '%c', '%c' to move the cursor to %s.", /* hjkl */
            Cmd.move_W, Cmd.move_S, Cmd.move_N, Cmd.move_E, goal);
    putstr(tmpwin, 0, sbuf);
    Sprintf(sbuf,
            "Use 'H', 'J', 'K', 'L' to fast-move the cursor, %s.",
            fastmovemode[iflags.getloc_moveskip]);
    putstr(tmpwin, 0, sbuf);
    putstr(tmpwin, 0, "Or enter a background symbol (ex. '<').");
    Sprintf(sbuf, "Use '%s' to move the cursor on yourself.",
           visctrl(Cmd.spkeys[NHKF_GETPOS_SELF]));
    putstr(tmpwin, 0, sbuf);
    if (!iflags.terrainmode || (iflags.terrainmode & TER_MON) != 0) {
        getpos_help_keyxhelp(tmpwin,
                             visctrl(Cmd.spkeys[NHKF_GETPOS_MON_NEXT]),
                             visctrl(Cmd.spkeys[NHKF_GETPOS_MON_PREV]),
                             GLOC_MONS);
    }
    if (!iflags.terrainmode || (iflags.terrainmode & TER_OBJ) != 0) {
        getpos_help_keyxhelp(tmpwin,
                             visctrl(Cmd.spkeys[NHKF_GETPOS_OBJ_NEXT]),
                             visctrl(Cmd.spkeys[NHKF_GETPOS_OBJ_PREV]),
                             GLOC_OBJS);
    }
    if (!iflags.terrainmode || (iflags.terrainmode & TER_MAP) != 0) {
        /* these are primarily useful when choosing a travel
           destination for the '_' command */
        getpos_help_keyxhelp(tmpwin,
                             visctrl(Cmd.spkeys[NHKF_GETPOS_DOOR_NEXT]),
                             visctrl(Cmd.spkeys[NHKF_GETPOS_DOOR_PREV]),
                             GLOC_DOOR);
        getpos_help_keyxhelp(tmpwin,
                             visctrl(Cmd.spkeys[NHKF_GETPOS_UNEX_NEXT]),
                             visctrl(Cmd.spkeys[NHKF_GETPOS_UNEX_PREV]),
                             GLOC_EXPLORE);
        getpos_help_keyxhelp(tmpwin,
                             visctrl(Cmd.spkeys[NHKF_GETPOS_INTERESTING_NEXT]),
                             visctrl(Cmd.spkeys[NHKF_GETPOS_INTERESTING_PREV]),
                             GLOC_INTERESTING);
    }
    Sprintf(sbuf, "Use '%s' to change fast-move mode to %s.",
            visctrl(Cmd.spkeys[NHKF_GETPOS_MOVESKIP]),
            fastmovemode[!iflags.getloc_moveskip]);
    putstr(tmpwin, 0, sbuf);
    if (!iflags.terrainmode || (iflags.terrainmode & TER_DETECT) == 0) {
        Sprintf(sbuf, "Use '%s' to toggle menu listing for possible targets.",
                visctrl(Cmd.spkeys[NHKF_GETPOS_MENU]));
        putstr(tmpwin, 0, sbuf);
        Sprintf(sbuf,
                "Use '%s' to change the mode of limiting possible targets.",
                visctrl(Cmd.spkeys[NHKF_GETPOS_LIMITVIEW]));
        putstr(tmpwin, 0, sbuf);
    }
    if (!iflags.terrainmode) {
        char kbuf[BUFSZ];

        if (getpos_getvalid) {
            Sprintf(sbuf, "Use '%s' or '%s' to move to valid locations.",
                    visctrl(Cmd.spkeys[NHKF_GETPOS_VALID_NEXT]),
                    visctrl(Cmd.spkeys[NHKF_GETPOS_VALID_PREV]));
            putstr(tmpwin, 0, sbuf);
        }
        if (getpos_hilitefunc) {
            Sprintf(sbuf, "Use '%s' to display valid locations.",
                    visctrl(Cmd.spkeys[NHKF_GETPOS_SHOWVALID]));
            putstr(tmpwin, 0, sbuf);
        }
        Sprintf(sbuf, "Use '%s' to toggle automatic description.",
                visctrl(Cmd.spkeys[NHKF_GETPOS_AUTODESC]));
        putstr(tmpwin, 0, sbuf);
        if (iflags.cmdassist) { /* assisting the '/' command, I suppose... */
            Sprintf(sbuf,
                    (iflags.getpos_coords == GPCOORDS_NONE)
         ? "(Set 'whatis_coord' option to include coordinates with '%s' text.)"
         : "(Reset 'whatis_coord' option to omit coordinates from '%s' text.)",
                    visctrl(Cmd.spkeys[NHKF_GETPOS_AUTODESC]));
        }
        /* disgusting hack; the alternate selection characters work for any
           getpos call, but only matter for dowhatis (and doquickwhatis) */
        doing_what_is = (goal == what_is_an_unknown_object);
        if (doing_what_is) {
            Sprintf(kbuf, "'%s' or '%s' or '%s' or '%s'",
                    visctrl(Cmd.spkeys[NHKF_GETPOS_PICK]),
                    visctrl(Cmd.spkeys[NHKF_GETPOS_PICK_Q]),
                    visctrl(Cmd.spkeys[NHKF_GETPOS_PICK_O]),
                    visctrl(Cmd.spkeys[NHKF_GETPOS_PICK_V]));
        } else {
            Sprintf(kbuf, "'%s'", visctrl(Cmd.spkeys[NHKF_GETPOS_PICK]));
        }
        Sprintf(sbuf, "Type a %s when you are at the right place.", kbuf);
        putstr(tmpwin, 0, sbuf);
        if (doing_what_is) {
            Sprintf(sbuf,
       "  '%s' describe current spot, show 'more info', move to another spot.",
                    visctrl(Cmd.spkeys[NHKF_GETPOS_PICK_V]));
            putstr(tmpwin, 0, sbuf);
            Sprintf(sbuf,
                    "  '%s' describe current spot,%s move to another spot;",
                    visctrl(Cmd.spkeys[NHKF_GETPOS_PICK]),
                    flags.help ? " prompt if 'more info'," : "");
            putstr(tmpwin, 0, sbuf);
            Sprintf(sbuf,
                    "  '%s' describe current spot, move to another spot;",
                    visctrl(Cmd.spkeys[NHKF_GETPOS_PICK_Q]));
            putstr(tmpwin, 0, sbuf);
            Sprintf(sbuf,
                    "  '%s' describe current spot, stop looking at things;",
                    visctrl(Cmd.spkeys[NHKF_GETPOS_PICK_O]));
            putstr(tmpwin, 0, sbuf);
        }
    }
    if (!force)
        putstr(tmpwin, 0, "Type Space or Escape when you're done.");
    putstr(tmpwin, 0, "");
    display_nhwindow(tmpwin, TRUE);
    destroy_nhwindow(tmpwin);
}

STATIC_OVL int
cmp_coord_distu(a, b)
const void *a;
const void *b;
{
    const coord *c1 = a;
    const coord *c2 = b;
    int dx, dy, dist_1, dist_2;

    dx = u.ux - c1->x;
    dy = u.uy - c1->y;
    dist_1 = max(abs(dx), abs(dy));
    dx = u.ux - c2->x;
    dy = u.uy - c2->y;
    dist_2 = max(abs(dx), abs(dy));

    if (dist_1 == dist_2)
        return (c1->y != c2->y) ? (c1->y - c2->y) : (c1->x - c2->x);

    return dist_1 - dist_2;
}

#define IS_UNEXPLORED_LOC(x,y) \
    (isok((x), (y))                                     \
     && glyph_is_cmap_or_cmap_variation(levl[(x)][(y)].hero_memory_layers.glyph)             \
     && generic_glyph_to_cmap(levl[(x)][(y)].hero_memory_layers.glyph) == S_unexplored  \
     && !levl[(x)][(y)].seenv)

STATIC_VAR struct opvar *gloc_filter_map = (struct opvar *) 0;

#define GLOC_SAME_AREA(x,y)                                     \
    (isok((x), (y))                                             \
     && (selection_getpoint((x),(y), gloc_filter_map)))

STATIC_VAR int gloc_filter_floodfill_match_glyph;

int
gloc_filter_classify_glyph(glyph)
int glyph;
{
    int c;

    if (!glyph_is_cmap_or_cmap_variation(glyph))
        return 0;

    c = generic_glyph_to_cmap(glyph);

    if (is_cmap_room(c) || is_cmap_grass(c) || is_cmap_ground(c) || is_cmap_furniture(c))
        return 1;
    else if (is_cmap_wall(c) || c == S_tree)
        return 2;
    else if (is_cmap_corr(c))
        return 3;
    else if (is_cmap_water(c))
        return 4;
    else if (is_cmap_lava(c))
        return 5;
    return 0;
}

STATIC_OVL int
gloc_filter_floodfill_matcharea(x, y)
int x, y;
{
    int glyph = back_to_glyph(x, y);

    if (!levl[x][y].seenv)
        return FALSE;

    if (glyph == gloc_filter_floodfill_match_glyph)
        return TRUE;

    if (gloc_filter_classify_glyph(glyph)
        == gloc_filter_classify_glyph(gloc_filter_floodfill_match_glyph))
        return TRUE;

    return FALSE;
}

void
gloc_filter_floodfill(x, y)
int x, y;
{
    gloc_filter_floodfill_match_glyph = back_to_glyph(x, y);

    set_selection_floodfillchk(gloc_filter_floodfill_matcharea);
    selection_floodfill(gloc_filter_map, x, y, FALSE);
}

void
gloc_filter_init()
{
    if (iflags.getloc_filter == GFILTER_AREA) {
        if (!gloc_filter_map) {
            gloc_filter_map = selection_opvar((char *) 0);
        }
        /* special case: if we're in a doorway, try to figure out which
           direction we're moving, and use that side of the doorway */
        if (IS_DOOR(levl[u.ux][u.uy].typ)) {
            if (u.dx || u.dy) {
                gloc_filter_floodfill(u.ux + u.dx, u.uy + u.dy);
            } else {
                /* TODO: maybe add both sides of the doorway? */
            }
        } else {
            gloc_filter_floodfill(u.ux, u.uy);
        }
    }
}

void
gloc_filter_done()
{
    if (gloc_filter_map) {
        opvar_free_x(gloc_filter_map);
        gloc_filter_map = (struct opvar *) 0;
    }
}

STATIC_OVL boolean
gather_locs_interesting(x, y, gloc)
int x, y, gloc;
{
    /* TODO: if glyph is a pile glyph, convert to ordinary one
     *       in order to keep tail/boulder/rock check simple.
     */
    int glyph = glyph_at(x, y);

    if (iflags.getloc_filter == GFILTER_VIEW && !cansee(x, y))
        return FALSE;
    if (iflags.getloc_filter == GFILTER_AREA && !GLOC_SAME_AREA(x, y)
        && !GLOC_SAME_AREA(x - 1, y) && !GLOC_SAME_AREA(x, y - 1)
        && !GLOC_SAME_AREA(x + 1, y) && !GLOC_SAME_AREA(x, y + 1))
        return FALSE;

    switch (gloc) {
    default:
    case GLOC_MONS:
        /* unlike '/M', this skips monsters revealed by
           warning glyphs and remembered unseen ones */
        return (glyph_is_monster(glyph)
                && glyph != monnum_to_glyph(PM_LONG_WORM_TAIL) && glyph != monnum_to_glyph(PM_ELDER_LONG_WORM_TAIL));
    case GLOC_OBJS:
        return (glyph_is_object(glyph)
                && glyph != objnum_to_glyph(BOULDER)
                && glyph != objnum_to_glyph(ROCK));
    case GLOC_DOOR:
        return (glyph_is_cmap_or_cmap_variation(glyph)
                && (is_cmap_door(generic_glyph_to_cmap(glyph))
                    || is_cmap_drawbridge(generic_glyph_to_cmap(glyph))
                    || generic_glyph_to_cmap(glyph) == S_ndoor
                    ));
    case GLOC_EXPLORE:
        return (glyph_is_cmap_or_cmap_variation(glyph)
                && (is_cmap_door(generic_glyph_to_cmap(glyph))
                    || is_cmap_drawbridge(generic_glyph_to_cmap(glyph))
                    || generic_glyph_to_cmap(glyph) == S_ndoor
                    || generic_glyph_to_cmap(glyph) == S_hbdoor
                    || generic_glyph_to_cmap(glyph) == S_vbdoor
                    || generic_glyph_to_cmap(glyph) == S_hoportcullis
                    || generic_glyph_to_cmap(glyph) == S_voportcullis
                    || generic_glyph_to_cmap(glyph) == S_room
                    || generic_glyph_to_cmap(glyph) == S_darkroom
                    || generic_glyph_to_cmap(glyph) == S_grass
                    || generic_glyph_to_cmap(glyph) == S_darkgrass
                    || generic_glyph_to_cmap(glyph) == S_ground
                    || generic_glyph_to_cmap(glyph) == S_darkground
                    || generic_glyph_to_cmap(glyph) == S_corr
                    || generic_glyph_to_cmap(glyph) == S_litcorr)
                && (IS_UNEXPLORED_LOC(x + 1, y)
                    || IS_UNEXPLORED_LOC(x - 1, y)
                    || IS_UNEXPLORED_LOC(x, y + 1)
                    || IS_UNEXPLORED_LOC(x, y - 1)));
    case GLOC_VALID:
        if (getpos_getvalid)
            return (*getpos_getvalid)(x,y);
        /*FALLTHRU*/
    case GLOC_INTERESTING:
        return gather_locs_interesting(x,y, GLOC_DOOR)
            || !(glyph_is_cmap_or_cmap_variation(glyph)
                 && (is_cmap_wall(generic_glyph_to_cmap(glyph))
                     || generic_glyph_to_cmap(glyph) == S_tree
                     || generic_glyph_to_cmap(glyph) == S_bars
                     || generic_glyph_to_cmap(glyph) == S_ice
                     || generic_glyph_to_cmap(glyph) == S_air
                     || generic_glyph_to_cmap(glyph) == S_cloud
                     || generic_glyph_to_cmap(glyph) == S_lava
                     || generic_glyph_to_cmap(glyph) == S_water
                     || generic_glyph_to_cmap(glyph) == S_pool
                     || generic_glyph_to_cmap(glyph) == S_ndoor
                     || generic_glyph_to_cmap(glyph) == S_hbdoor
                     || generic_glyph_to_cmap(glyph) == S_vbdoor
                     || generic_glyph_to_cmap(glyph) == S_hoportcullis
                     || generic_glyph_to_cmap(glyph) == S_voportcullis
                     || generic_glyph_to_cmap(glyph) == S_room
                     || generic_glyph_to_cmap(glyph) == S_darkroom
                     || generic_glyph_to_cmap(glyph) == S_grass
                     || generic_glyph_to_cmap(glyph) == S_darkgrass
                     || generic_glyph_to_cmap(glyph) == S_ground
                     || generic_glyph_to_cmap(glyph) == S_darkground
                     || generic_glyph_to_cmap(glyph) == S_corr
                     || generic_glyph_to_cmap(glyph) == S_litcorr
                     || generic_glyph_to_cmap(glyph) == S_unexplored
                     ));
    }
    /*NOTREACHED*/
    return FALSE;
}

/* gather locations for monsters or objects shown on the map */
STATIC_OVL void
gather_locs(arr_p, cnt_p, gloc)
coord **arr_p;
int *cnt_p;
int gloc;
{
    int x, y, pass, idx;

    /*
     * We always include the hero's location even if there is no monster
     * (invisible hero without see invisible) or object (usual case)
     * displayed there.  That way, the count will always be at least 1,
     * and player has a visual indicator (cursor returns to hero's spot)
     * highlighting when successive 'm's or 'o's have cycled all the way
     * through all monsters or objects.
     *
     * Hero's spot will always sort to array[0] because it will always
     * be the shortest distance (namely, 0 units) away from <u.ux,u.uy>.
     */

    gloc_filter_init();

    *cnt_p = idx = 0;
    for (pass = 0; pass < 2; pass++) {
        for (x = 1; x < COLNO; x++)
            for (y = 0; y < ROWNO; y++) {
                if ((x == u.ux && y == u.uy)
                    || gather_locs_interesting(x, y, gloc)) {
                    if (!pass) {
                        ++*cnt_p;
                    } else {
                        (*arr_p)[idx].x = x;
                        (*arr_p)[idx].y = y;
                        ++idx;
                    }
                }
            }

        if (!pass) /* end of first pass */
            *arr_p = (coord *) alloc(*cnt_p * sizeof (coord));
        else /* end of second pass */
            qsort(*arr_p, *cnt_p, sizeof (coord), cmp_coord_distu);
    } /* pass */

    gloc_filter_done();
}

char *
dxdy_to_dist_descr(dx, dy, fulldir)
int dx, dy;
boolean fulldir;
{
    static char buf[30];
    int dst;

    if (!dx && !dy) {
        Sprintf(buf, "here");
    } else if ((dst = xytod(dx, dy)) != -1) {
        /* explicit direction; 'one step' is implicit */
        Sprintf(buf, "%s", directionname(dst));
    } else {
        static const char *dirnames[4][2] = {
            { "n", "north" },
            { "s", "south" },
            { "w", "west" },
            { "e", "east" } };
        buf[0] = '\0';
        /* 9999: protect buf[] against overflow caused by invalid values */
        if (dy) {
            if (abs(dy) > 9999)
                dy = sgn(dy) * 9999;
            Sprintf(eos(buf), "%d%s%s", abs(dy), dirnames[(dy > 0)][fulldir],
                    dx ? "," : "");
        }
        if (dx) {
            if (abs(dx) > 9999)
                dx = sgn(dx) * 9999;
            Sprintf(eos(buf), "%d%s", abs(dx),
                    dirnames[2 + (dx > 0)][fulldir]);
        }
    }
    return buf;
}

/* coordinate formatting for 'whatis_coord' option */
char *
coord_desc(x, y, outbuf, cmode)
int x, y;
char *outbuf, cmode;
{
    static char screen_fmt[16]; /* [12] suffices: "[%02d,%02d]" */
    int dx, dy;

    outbuf[0] = '\0';
    switch (cmode) {
    default:
        break;
    case GPCOORDS_COMFULL:
    case GPCOORDS_COMPASS:
        /* "east", "3s", "2n,4w" */
        dx = x - u.ux;
        dy = y - u.uy;
        Sprintf(outbuf, "(%s)",
                dxdy_to_dist_descr(dx, dy, cmode == GPCOORDS_COMFULL));
        break;
    case GPCOORDS_MAP: /* x,y */
        /* upper left corner of map is <1,0>;
           with default COLNO,ROWNO lower right corner is <79,20> */
        Sprintf(outbuf, "<%d,%d>", x, y);
        break;
    case GPCOORDS_SCREEN: /* y+2,x */
        /* for normal map sizes, force a fixed-width formatting so that
           /m, /M, /o, and /O output lines up cleanly; map sizes bigger
           than Nx999 or 999xM will still work, but not line up like normal
           when displayed in a column setting */
        if (!*screen_fmt)
            Sprintf(screen_fmt, "[%%%sd,%%%sd]",
                    (ROWNO - 1 + 2 < 100) ? "02" :  "03",
                    (COLNO - 1 < 100) ? "02" : "03");
        /* map line 0 is screen row 2;
           map column 0 isn't used, map column 1 is screen column 1 */
        Sprintf(outbuf, screen_fmt, y + 2, x);
        break;
    }
    return outbuf;
}

STATIC_OVL void
auto_describe(cx, cy)
int cx, cy;
{
    coord cc;
    nhsym sym = 0;
    char tmpbuf[BUFSZ];
    const char *firstmatch = "unknown";

    cc.x = cx;
    cc.y = cy;
    if (do_screen_description(cc, TRUE, sym, tmpbuf, &firstmatch,
                              (struct permonst **) 0)) {
        (void) coord_desc(cx, cy, tmpbuf, iflags.getpos_coords);
        custompline(SUPPRESS_HISTORY | STAY_ON_LINE,
                    "%s%s%s%s%s", firstmatch, *tmpbuf ? " " : "", tmpbuf,
                    (iflags.autodescribe
                     && getpos_getvalid && !(*getpos_getvalid)(cx, cy))
                      ? " (illegal)" : "",
                    (iflags.getloc_travelmode && !is_valid_travelpt(cx, cy))
                      ? " (no travel path)" : "");
        curs(WIN_MAP, cx, cy);
        flush_screen(0);
    }
}

boolean
getpos_menu(ccp, gloc)
coord *ccp;
int gloc;
{
    coord *garr = DUMMY;
    int gcount = 0;
    winid tmpwin;
    anything any;
    int i, pick_cnt;
    menu_item *picks = (menu_item *) 0;
    char tmpbuf[BUFSZ];

    gather_locs(&garr, &gcount, gloc);

    if (gcount < 2) { /* gcount always includes the hero */
        free((genericptr_t) garr);
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot %s %s.",
            iflags.getloc_filter == GFILTER_VIEW ? "see" : "detect",
            gloc_descr[gloc][0]);
        return FALSE;
    }

    tmpwin = create_nhwindow(NHW_MENU);
    start_menu_ex(tmpwin, GHMENU_STYLE_CHOOSE_SIMPLE);
    any = zeroany;

    /* gather_locs returns array[0] == you. skip it. */
    for (i = 1; i < gcount; i++) {
        char fullbuf[BUFSZ];
        coord tmpcc;
        const char *firstmatch = "unknown";
        nhsym sym = 0;

        any.a_int = i + 1;
        tmpcc.x = garr[i].x;
        tmpcc.y = garr[i].y;
        if (do_screen_description(tmpcc, TRUE, sym, tmpbuf,
                              &firstmatch, (struct permonst **)0)) {
            (void) coord_desc(garr[i].x, garr[i].y, tmpbuf,
                              iflags.getpos_coords);
            Sprintf(fullbuf, "%s%s%s", firstmatch,
                    (*tmpbuf ? " " : ""), tmpbuf);
            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, fullbuf,
                     MENU_UNSELECTED);
        }
    }

    Sprintf(tmpbuf, "Pick %s%s%s",
            an(gloc_descr[gloc][1]),
            gloc_filtertxt[iflags.getloc_filter],
            iflags.getloc_travelmode ? " for travel destination" : "");
    end_menu(tmpwin, tmpbuf);
    pick_cnt = select_menu(tmpwin, PICK_ONE, &picks);
    destroy_nhwindow(tmpwin);
    if (pick_cnt > 0) {
        ccp->x = garr[picks->item.a_int - 1].x;
        ccp->y = garr[picks->item.a_int - 1].y;
        free((genericptr_t) picks);
    }
    free((genericptr_t) garr);
    return (pick_cnt > 0);
}

int
getpos(ccp, force, goal, cursor_style)
coord *ccp;
boolean force;
const char *goal;
enum game_cursor_types cursor_style;
{
    const char *cp;
    static struct {
        int nhkf, ret;
    } const pick_chars_def[] = {
        { NHKF_GETPOS_PICK, LOOK_TRADITIONAL },
        { NHKF_GETPOS_PICK_Q, LOOK_QUICK },
        { NHKF_GETPOS_PICK_O, LOOK_ONCE },
        { NHKF_GETPOS_PICK_V, LOOK_VERBOSE }
    };
    static const int mMoOdDxX_def[] = {
        NHKF_GETPOS_MON_NEXT,
        NHKF_GETPOS_MON_PREV,
        NHKF_GETPOS_OBJ_NEXT,
        NHKF_GETPOS_OBJ_PREV,
        NHKF_GETPOS_DOOR_NEXT,
        NHKF_GETPOS_DOOR_PREV,
        NHKF_GETPOS_UNEX_NEXT,
        NHKF_GETPOS_UNEX_PREV,
        NHKF_GETPOS_INTERESTING_NEXT,
        NHKF_GETPOS_INTERESTING_PREV,
        NHKF_GETPOS_VALID_NEXT,
        NHKF_GETPOS_VALID_PREV
    };
    char pick_chars[6];
    char mMoOdDxX[13];
    int result = 0;
    int cx = 0, cy = 0, i, c;
    int sidx = 0, tx = 0, ty = 0;
    boolean msg_given = TRUE; /* clear message window by default */
    boolean show_goal_msg = FALSE;
    boolean hilite_state = FALSE;
    coord *garr[NUM_GLOCS] = DUMMY;
    int gcount[NUM_GLOCS] = DUMMY;
    int gidx[NUM_GLOCS] = DUMMY;

    flags.show_cursor_on_u = TRUE;
    flags.force_paint_at_cursor = TRUE;
    flags.active_cursor_style = cursor_style;
    update_cursor(flags.active_cursor_style, flags.show_cursor_on_u, flags.show_cursor_on_u);
    create_context_menu(
        iflags.getloc_travelmode ? CREATE_CONTEXT_MENU_IN_GETPOS_TRAVEL_MODE :
        cursor_style == CURSOR_STYLE_POLEARM_CURSOR || cursor_style == CURSOR_STYLE_PAY_CURSOR ? CREATE_CONTEXT_MENU_IN_GETPOS_MONSTER :
        CREATE_CONTEXT_MENU_IN_GETPOS_GENERAL);

    for (i = 0; i < SIZE(pick_chars_def); i++)
        pick_chars[i] = Cmd.spkeys[pick_chars_def[i].nhkf];

    pick_chars[SIZE(pick_chars_def)] = '\0';

    for (i = 0; i < SIZE(mMoOdDxX_def); i++)
        mMoOdDxX[i] = Cmd.spkeys[mMoOdDxX_def[i]];

    mMoOdDxX[SIZE(mMoOdDxX_def)] = '\0';

    if (!goal)
        goal = "desired location";

    if (flags.verbose) 
    {
        pline("(For instructions type a '%s')",
              visctrl(Cmd.spkeys[NHKF_GETPOS_HELP]));

        msg_given = TRUE;
    }
    cx = (int)ccp->x;
    cy = (int)ccp->y;
    tx = (int)ccp->x;
    ty = (int)ccp->y;

#if defined(CLIPPING) && !defined(ANDROID)
    cliparound(cx, cy, FALSE);
#endif
    curs(WIN_MAP, cx, cy);
    flush_screen(0);
    if(cursor_style != CURSOR_STYLE_TELEPORT_CURSOR)
        issue_simple_gui_command(GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE);
#if defined(MAC) || defined(ANDROID)
    lock_mouse_cursor(TRUE);
#endif

    for (;;) 
    {
        flags.force_paint_at_cursor = TRUE;
        if (show_goal_msg) 
        {
            pline("Move cursor to %s:", goal);
            curs(WIN_MAP, cx, cy);
            flush_screen(0);
            show_goal_msg = FALSE;
        }
        else if (iflags.autodescribe && !msg_given && !hilite_state)
        {
            auto_describe(cx, cy);
        }

        c = converted_nh_poskey(&tx, &ty, &sidx);


        if (hilite_state) 
        {
            (*getpos_hilitefunc)(2);
            hilite_state = FALSE;
            curs(WIN_MAP, cx, cy);
            flush_screen(0);
        }

        if (iflags.autodescribe)
            msg_given = FALSE;

        if (c == Cmd.spkeys[NHKF_ESC]) 
        {
            cx = cy = -10;
            msg_given = TRUE; /* force clear */
            result = -1;
            break;
        }
        if (c == 0) 
        {
            if (!isok(tx, ty))
                continue;
            /* a mouse click event, just assign and return */
            cx = tx;
            cy = ty;
            break;
        }
        if ((cp = index(pick_chars, c)) != 0) 
        {
            /* '.' => 0, ',' => 1, ';' => 2, ':' => 3 */
            result = pick_chars_def[(int) (cp - pick_chars)].ret;
            break;
        }
        for (i = 0; i < 8; i++) 
        {
            int dx, dy;

            if (Cmd.dirchars[i] == c) 
            {
                /* a normal movement letter or digit */
                dx = xdir[i];
                dy = ydir[i];
            } 
            else if (Cmd.alphadirchars[i] == lowc((char) c)
                       || (Cmd.num_pad && Cmd.dirchars[i] == (c & 0177))) 
            {
                /* a shifted movement letter or Meta-digit */
                if (iflags.getloc_moveskip)
                {
                    /* skip same glyphs */
                    int glyph = glyph_at(cx, cy);

                    dx = xdir[i];
                    dy = ydir[i];
                    while (isok(cx + dx, cy + dy)
                           && glyph == glyph_at(cx + dx, cy + dy)
                           && isok(cx + dx + xdir[i], cy + dy + ydir[i])
                           && glyph == glyph_at(cx + dx + xdir[i],
                                                cy + dy + ydir[i]))
                    {
                        dx += xdir[i];
                        dy += ydir[i];
                    }
                }
                else
                {
                    dx = 8 * xdir[i];
                    dy = 8 * ydir[i];
                }
            }
            else
                continue;

            /* truncate at map edge; diagonal moves complicate this... */
            if (cx + dx < 1)
            {
                dy -= sgn(dy) * (1 - (cx + dx));
                dx = 1 - cx; /* so that (cx+dx == 1) */
            }
            else if (cx + dx > COLNO - 1) 
            {
                dy += sgn(dy) * ((COLNO - 1) - (cx + dx));
                dx = (COLNO - 1) - cx;
            }

            if (cy + dy < 0) 
            {
                dx -= sgn(dx) * (0 - (cy + dy));
                dy = 0 - cy; /* so that (cy+dy == 0) */
            }
            else if (cy + dy > ROWNO - 1) 
            {
                dx += sgn(dx) * ((ROWNO - 1) - (cy + dy));
                dy = (ROWNO - 1) - cy;
            }
            cx += dx;
            cy += dy;
            goto nxtc;
        }

        if (c == Cmd.spkeys[NHKF_GETPOS_HELP] || redraw_cmd(c)) 
        {
            if (c == Cmd.spkeys[NHKF_GETPOS_HELP])
                getpos_help(force, goal);
            else /* ^R */
                docrt(); /* redraw */
            /* update message window to reflect that we're still targetting */
            show_goal_msg = TRUE;
            msg_given = TRUE;
        }
        else if (c == Cmd.spkeys[NHKF_GETPOS_SHOWVALID]
                   && getpos_hilitefunc) 
        {
            if (!hilite_state) 
            {
                (*getpos_hilitefunc)(0);
                (*getpos_hilitefunc)(1);
                hilite_state = TRUE;
            }
            goto nxtc;
        } 
        else if (c == Cmd.spkeys[NHKF_GETPOS_AUTODESC]) 
        {
            iflags.autodescribe = !iflags.autodescribe;
            pline("Automatic description %sis %s.",
                  flags.verbose ? "of features under cursor " : "",
                  iflags.autodescribe ? "on" : "off");
            if (!iflags.autodescribe)
                show_goal_msg = TRUE;
            msg_given = TRUE;
            goto nxtc;
        } 
        else if (c == Cmd.spkeys[NHKF_GETPOS_LIMITVIEW])
        {
            static const char *const view_filters[NUM_GFILTER] = {
                "Not limiting targets",
                "Limiting targets to those in sight",
                "Limiting targets to those in same area"
            };

            iflags.getloc_filter = (iflags.getloc_filter + 1) % NUM_GFILTER;
            for (i = 0; i < NUM_GLOCS; i++) 
            {
                if (garr[i]) 
                {
                    free((genericptr_t) garr[i]);
                    garr[i] = NULL;
                }
                gidx[i] = gcount[i] = 0;
            }
            pline("%s.", view_filters[iflags.getloc_filter]);
            msg_given = TRUE;
            goto nxtc;
        } 
        else if (c == Cmd.spkeys[NHKF_GETPOS_MENU]) 
        {
            iflags.getloc_usemenu = !iflags.getloc_usemenu;
            pline("%s a menu to show possible targets%s.",
                  iflags.getloc_usemenu ? "Using" : "Not using",
                  iflags.getloc_usemenu
                      ? " for 'm|M', 'o|O', 'd|D', and 'x|X'" : "");
            msg_given = TRUE;
            goto nxtc;
        }
        else if (c == Cmd.spkeys[NHKF_GETPOS_SELF]) 
        {
            /* reset 'm&M', 'o&O', &c; otherwise, there's no way for player
               to achieve that except by manually cycling through all spots */
            for (i = 0; i < NUM_GLOCS; i++)
                gidx[i] = 0;
            cx = (int)u.ux;
            cy = (int)u.uy;
            goto nxtc;
        } 
        else if (c == Cmd.spkeys[NHKF_GETPOS_MOVESKIP]) 
        {
            iflags.getloc_moveskip = !iflags.getloc_moveskip;
            pline("%skipping over similar terrain when fastmoving the cursor.",
                  iflags.getloc_moveskip ? "S" : "Not s");
        } 
        else if ((cp = index(mMoOdDxX, c)) != 0) 
        { /* 'm|M', 'o|O', &c */
            /* nearest or farthest monster or object or door or unexplored */
            int gtmp = (int) (cp - mMoOdDxX), /* 0..7 */
                gloc = gtmp >> 1;             /* 0..3 */

            if (iflags.getloc_usemenu)
            {
                coord tmpcrd;

                if (getpos_menu(&tmpcrd, gloc)) 
                {
                    cx = (int)tmpcrd.x;
                    cy = (int)tmpcrd.y;
                }
                goto nxtc;
            }

            if (!garr[gloc]) 
            {
                gather_locs(&garr[gloc], &gcount[gloc], gloc);
                gidx[gloc] = 0; /* garr[][0] is hero's spot */
            }
            if (!(gtmp & 1))
            {  /* c=='m' || c=='o' || c=='d' || c=='x') */
                gidx[gloc] = (gidx[gloc] + 1) % gcount[gloc];
            }
            else 
            {            /* c=='M' || c=='O' || c=='D' || c=='X') */
                if (--gidx[gloc] < 0)
                    gidx[gloc] = gcount[gloc] - 1;
            }
            cx = (int)garr[gloc][gidx[gloc]].x;
            cy = (int)garr[gloc][gidx[gloc]].y;
            goto nxtc;
        }
        else
        {
            if (!index(quitchars, c)) 
            {
                char matching[MAX_CMAPPED_CHARS];
                int pass, lo_x, lo_y, hi_x, hi_y, k = 0;

                (void) memset((genericptr_t) matching, 0, sizeof matching);
                for (sidx = 1; sidx < MAX_CMAPPED_CHARS; sidx++) { /* [0] left as 0 */
                    if (IS_DOOR(sidx) || IS_WALL(sidx)
                        || sidx == SDOOR || sidx == SCORR
                        || generic_glyph_to_cmap(k) == S_room
                        || generic_glyph_to_cmap(k) == S_darkroom
                        || generic_glyph_to_cmap(k) == S_grass
                        || generic_glyph_to_cmap(k) == S_darkgrass
                        || generic_glyph_to_cmap(k) == S_ground
                        || generic_glyph_to_cmap(k) == S_darkground
                        || generic_glyph_to_cmap(k) == S_corr
                        || generic_glyph_to_cmap(k) == S_litcorr)
                        continue;
                    if (c == defsyms[sidx].sym || c == (int) showsyms[sidx])
                        matching[sidx] = (char) ++k;
                }
                if (k) 
                {
                    for (pass = 0; pass <= 1; pass++) 
                    {
                        /* pass 0: just past current pos to lower right;
                           pass 1: upper left corner to current pos */
                        lo_y = (pass == 0) ? cy : 0;
                        hi_y = (pass == 0) ? ROWNO - 1 : cy;
                        for (ty = lo_y; ty <= hi_y; ty++) 
                        {
                            lo_x = (pass == 0 && ty == lo_y) ? cx + 1 : 1;
                            hi_x = (pass == 1 && ty == hi_y) ? cx : COLNO - 1;
                            for (tx = lo_x; tx <= hi_x; tx++) 
                            {
                                /* first, look at what is currently visible
                                   (might be monster) */
                                k = glyph_at(tx, ty);
                                if (glyph_is_cmap_or_cmap_variation(k)
                                    && matching[generic_glyph_to_cmap(k)])
                                    goto foundc;
                                /* next, try glyph that's remembered here
                                   (might be trap or object) */
                                if (level.flags.hero_memory
                                    /* !terrainmode: don't move to remembered
                                       trap or object if not currently shown */
                                    && !iflags.terrainmode) 
                                {
                                    k = levl[tx][ty].hero_memory_layers.glyph;
                                    if (glyph_is_cmap_or_cmap_variation(k)
                                        && matching[generic_glyph_to_cmap(k)])
                                        goto foundc;
                                }
                                /* last, try actual terrain here (shouldn't
                                   we be using lastseentyp[][] instead?) */
                                if (levl[tx][ty].seenv)
                                {
                                    k = back_to_glyph(tx, ty);
                                    if (glyph_is_cmap_or_cmap_variation(k)
                                        && matching[generic_glyph_to_cmap(k)])
                                        goto foundc;
                                }
                                continue;
                            foundc:
                                cx = tx, cy = ty;
                                if (msg_given)
                                {
                                    clear_nhwindow(WIN_MESSAGE);
                                    msg_given = FALSE;
                                }
                                goto nxtc;
                            } /* column */
                        }     /* row */
                    }         /* pass */
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    pline_ex(ATR_NONE, CLR_MSG_FAIL, "Can't find dungeon feature '%c'.", c);
                    msg_given = TRUE;
                    goto nxtc;
                } 
                else
                {
                    char note[QBUFSZ];

                    if (!force)
                        Strcpy(note, "aborted");
                    else /* hjkl */
                        Sprintf(note, "use '%c', '%c', '%c', '%c' or '%s'",
                                Cmd.move_W, Cmd.move_S, Cmd.move_N, Cmd.move_E,
                                visctrl(Cmd.spkeys[NHKF_GETPOS_PICK]));
                    pline("Unknown direction: '%s' (%s).", visctrl((char) c),
                          note);
                    msg_given = TRUE;
                } /* k => matching */
            }     /* !quitchars */
            if (force)
                goto nxtc;
            pline("Done.");
            msg_given = FALSE; /* suppress clear */
            cx = -1;
            cy = 0;
            result = 0; /* not -1 */
            break;
        }
    nxtc:
        ;
#ifdef CLIPPING
        cliparound(cx, cy, FALSE);
#endif
        curs(WIN_MAP, cx, cy);
        flush_screen(0);
    }
#if defined(MAC) || defined(ANDROID)
    lock_mouse_cursor(FALSE);
#endif
    if (msg_given)
        clear_nhwindow(WIN_MESSAGE);
    ccp->x = (xchar)cx;
    ccp->y = (xchar)cy;
    for (i = 0; i < NUM_GLOCS; i++)
        if (garr[i])
            free((genericptr_t) garr[i]);
    getpos_hilitefunc = (void FDECL((*), (int))) 0;
    getpos_getvalid = (boolean FDECL((*), (int, int))) 0;

    flags.show_cursor_on_u = FALSE;
    flags.force_paint_at_cursor = TRUE;
    flags.active_cursor_style = CURSOR_STYLE_GENERIC_CURSOR;
    update_cursor(flags.active_cursor_style, flags.force_paint_at_cursor, flags.show_cursor_on_u);
    if (cursor_style != CURSOR_STYLE_TELEPORT_CURSOR)
        issue_simple_gui_command(GUI_CMD_RESTORE_TRAVEL_MODE);
    create_context_menu(CREATE_CONTEXT_MENU_NORMAL);

    return result;
}

/* allocate space for a monster's name; removes old name if there is one */
void
new_mname(mon, lth)
struct monst *mon;
size_t lth; /* desired length (caller handles adding 1 for terminator) */
{
    if (lth) {
        /* allocate mextra if necessary; otherwise get rid of old name */
        if (!mon->mextra)
            mon->mextra = newmextra();
        else
            free_mname(mon); /* already has mextra, might also have name */
        MNAME(mon) = (char *) alloc(lth);
    } else {
        /* zero length: the new name is empty; get rid of the old name */
        if (has_mname(mon))
            free_mname(mon);
    }
}

/* release a monster's name; retains mextra even if all fields are now null */
void
free_mname(mon)
struct monst *mon;
{
    if (has_mname(mon)) {
        free((genericptr_t) MNAME(mon));
        MNAME(mon) = (char *) 0;
    }
}

/* allocate space for a monster's uname; removes old uname if there is one */
void
new_umname(mon, lth)
struct monst* mon;
size_t lth; /* desired length (caller handles adding 1 for terminator) */
{
    if (lth) 
    {
        /* allocate mextra if necessary; otherwise get rid of old name */
        if (!mon->mextra)
            mon->mextra = newmextra();
        else
            free_umname(mon); /* already has mextra, might also have name */
        UMNAME(mon) = (char*)alloc(lth);
    }
    else {
        /* zero length: the new name is empty; get rid of the old name */
        if (has_umname(mon))
            free_umname(mon);
    }
}

/* release a monster's uname; retains mextra even if all fields are now null */
void
free_umname(mon)
struct monst* mon;
{
    if (has_umname(mon)) {
        free((genericptr_t)UMNAME(mon));
        UMNAME(mon) = (char*)0;
    }
}

/* allocate space for an object's name; removes old name if there is one */
void
new_oname(obj, lth)
struct obj *obj;
size_t lth; /* desired length (caller handles adding 1 for terminator) */
{
    if (lth) {
        /* allocate oextra if necessary; otherwise get rid of old name */
        if (!obj->oextra)
            obj->oextra = newoextra();
        else
            free_oname(obj); /* already has oextra, might also have name */
        ONAME(obj) = (char *) alloc(lth);
    } else {
        /* zero length: the new name is empty; get rid of the old name */
        if (has_oname(obj))
            free_oname(obj);
    }
}

/* release an object's name; retains oextra even if all fields are now null */
void
free_oname(obj)
struct obj *obj;
{
    if (has_oname(obj)) {
        free((genericptr_t) ONAME(obj));
        ONAME(obj) = (char *) 0;
    }
}

/*  safe_oname() always returns a valid pointer to
 *  a string, either the pointer to an object's name
 *  if it has one, or a pointer to an empty string
 *  if it doesn't.
 */
const char *
safe_oname(obj)
struct obj *obj;
{
    if (has_oname(obj))
        return ONAME(obj);
    return empty_string;
}


/* allocate space for an object's name; removes old name if there is one */
void
new_uoname(obj, lth)
struct obj* obj;
size_t lth; /* desired length (caller handles adding 1 for terminator) */
{
    if (lth) {
        /* allocate oextra if necessary; otherwise get rid of old name */
        if (!obj->oextra)
            obj->oextra = newoextra();
        else
            free_uoname(obj); /* already has oextra, might also have name */
        UONAME(obj) = (char*)alloc(lth);
    }
    else {
        /* zero length: the new name is empty; get rid of the old name */
        if (has_uoname(obj))
            free_uoname(obj);
    }
}

/* release an object's name; retains oextra even if all fields are now null */
void
free_uoname(obj)
struct obj* obj;
{
    if (has_uoname(obj)) {
        free((genericptr_t)UONAME(obj));
        UONAME(obj) = (char*)0;
    }
}

/*  safe_oname() always returns a valid pointer to
 *  a string, either the pointer to an object's name
 *  if it has one, or a pointer to an empty string
 *  if it doesn't.
 */
const char*
safe_uoname(obj)
struct obj* obj;
{
    if (has_uoname(obj))
        return UONAME(obj);
    return empty_string;
}


/* historical note: this returns a monster pointer because it used to
   allocate a new bigger block of memory to hold the monster and its name */
struct monst *
christen_monst(mtmp, name)
struct monst *mtmp;
const char *name;
{
    size_t lth;
    char buf[PL_PSIZ];

    /* dogname & catname are PL_PSIZ arrays; object names have same limit */
    lth = (name && *name) ? (strlen(name) + 1) : 0;
    if (lth > PL_PSIZ) {
        lth = PL_PSIZ;
        name = strncpy(buf, name, PL_PSIZ - 1);
        buf[PL_PSIZ - 1] = '\0';
    }
    new_mname(mtmp, lth); /* removes old name if one is present */
    if (lth)
        Strcpy(MNAME(mtmp), name);
    return mtmp;
}

struct monst*
u_name_monst(mtmp, name)
struct monst* mtmp;
const char* name;
{
    size_t lth;
    char buf[PL_PSIZ];

    /* dogname & catname are PL_PSIZ arrays; object names have same limit */
    lth = (name && *name) ? (strlen(name) + 1) : 0;
    if (lth > PL_PSIZ) {
        lth = PL_PSIZ;
        name = strncpy(buf, name, PL_PSIZ - 1);
        buf[PL_PSIZ - 1] = '\0';
    }
    new_umname(mtmp, lth); /* removes old name if one is present */
    if (lth)
        Strcpy(UMNAME(mtmp), name);
    return mtmp;
}

/* check whether user-supplied name matches or nearly matches an unnameable
   monster's name; if so, give an alternate reject message for do_mname() */
STATIC_OVL boolean
alreadynamed(mtmp, monnambuf, usrbuf)
struct monst *mtmp;
char *monnambuf, *usrbuf;
{
    char pronounbuf[10], *p;

    if (fuzzymatch(usrbuf, monnambuf, " -_", TRUE)
        /* catch trying to name "the Oracle" as "Oracle" */
        || (!strncmpi(monnambuf, "the ", 4)
            && fuzzymatch(usrbuf, monnambuf + 4, " -_", TRUE))
        /* catch trying to name "invisible Orcus" as "Orcus" */
        || ((p = strstri(monnambuf, "invisible ")) != 0
            && fuzzymatch(usrbuf, p + 10, " -_", TRUE))
        /* catch trying to name "the {priest,Angel} of Crom" as "Crom" */
        || ((p = strstri(monnambuf, " of ")) != 0
            && fuzzymatch(usrbuf, p + 4, " -_", TRUE))) {
        pline("%s is already called %s.",
              upstart(strcpy(pronounbuf, mhe(mtmp))), monnambuf);
        return TRUE;
    } else if (mtmp->data == &mons[PM_JUBILEX]
               && strstri(monnambuf, "Jubilex")
               && !strcmpi(usrbuf, "Jubilex")) {
        pline("%s doesn't like being called %s.", upstart(monnambuf), usrbuf);
        return TRUE;
    }
    return FALSE;
}

/* allow player to assign a name to some chosen monster */
STATIC_OVL void
do_mname()
{
    char buf[BUFSZ] = DUMMY, monnambuf[BUFSZ], qbuf[QBUFSZ];
    coord cc;
    int cx, cy;
    struct monst *mtmp = 0;

    if (Hallucination) 
    {
        You("would never recognize it anyway.");
        return;
    }
    cc.x = u.ux;
    cc.y = u.uy;
    if (getpos(&cc, FALSE, "the monster you want to name", CURSOR_STYLE_NAME_CURSOR) < 0
        || (cx = cc.x) < 0)
        return;
    cy = cc.y;

    if (cx == u.ux && cy == u.uy) 
    {
        if (u.usteed && canspotmon(u.usteed)) 
        {
            mtmp = u.usteed;
        } 
        else 
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "This %s creature is called %s and cannot be renamed.",
                  beautiful(), plname);
            return;
        }
    } 
    else
        mtmp = m_at(cx, cy);

    if (!mtmp
        || (!sensemon(mtmp)
            && (!(cansee(cx, cy) || see_with_infrared(mtmp))
                || mtmp->mundetected || M_AP_TYPE(mtmp) == M_AP_FURNITURE
                || M_AP_TYPE(mtmp) == M_AP_OBJECT
                || (is_invisible(mtmp) && !See_invisible)))) 
    {
        pline("I see no monster there.");
        return;
    }
    /* special case similar to the one in lookat() */
    Sprintf(qbuf, "What do you want to call %s?",
            distant_monnam(mtmp, ARTICLE_THE, monnambuf));
    getlin(qbuf, buf);
    if (!*buf || *buf == '\033')
        return;
    /* strip leading and trailing spaces; unnames monster if all spaces */
    (void) mungspaces(buf);

    /* Unique monsters have their own specific names or titles.
     * Shopkeepers, temple priests and other minions use alternate
     * name formatting routines which ignore any user-supplied name.
     *
     * Don't say the name is being rejected if it happens to match
     * the existing name.
     */
    if ((mtmp->data->geno & G_UNIQ) && !mtmp->ispriest) 
    {
        if (!alreadynamed(mtmp, monnambuf, buf))
            pline("%s doesn't like being called names!", upstart(monnambuf));
    } 
    else if (mtmp->isshk
               && !(Deaf || !mon_can_move(mtmp)
                    || mtmp->data->msound <= MS_ANIMAL)) 
    {
        if (!alreadynamed(mtmp, monnambuf, buf))
        {
            mtmp->u_know_mname = 1;
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "I'm %s, not %s.", shkname(mtmp), buf);
        }
    } 
    else if (mtmp->ispriest || mtmp->isminion || mtmp->isshk || mtmp->issmith || mtmp->isnpc) 
    {
        if (!alreadynamed(mtmp, monnambuf, buf))
            pline("%s will not accept the name %s.", upstart(monnambuf), buf);
    }
    else if(((!has_mname(mtmp) || (is_tame(mtmp) && !is_charmed(mtmp))) && is_animal(mtmp->data) && is_peaceful(mtmp)) || (mtmp->mon_flags & MON_FLAGS_YOUR_CHILD))
    {
        /* Peaceful animals who do not have names and tame animals can be named, others do not accept your naming */
        (void)christen_monst(mtmp, buf);
        mtmp->u_know_mname = 1;

        /* Clear out umname */
        if (has_umname(mtmp))
            free_umname(mtmp);
    }
    //else if (has_mname(mtmp))
    //{
    //    if(!mtmp->u_know_mname || !*buf)
    //        (void)u_name_monst(mtmp, buf);
    //    else
    //        pline("%s will not accept the name %s.", upstart(monnambuf), buf);
    //}
    else /* Nickname */
    {
        (void)u_name_monst(mtmp, buf);
    }
}

STATIC_VAR int via_naming = 0;

/*
 * This routine used to change the address of 'obj' so be unsafe if not
 * used with extreme care.  Applying a name to an object no longer
 * allocates a replacement object, so that old risk is gone.
 */
STATIC_OVL
void
do_uoname(obj)
register struct obj *obj;
{
    if (!obj)
        return;

    char buf[BUFSZ] = DUMMY, qbuf[QBUFSZ];
    //char *bufp, buf[BUFSZ] = DUMMY, bufcpy[BUFSZ], qbuf[QBUFSZ];
    //short objtyp;

    /* Do this now because there's no point in even asking for a name */
    if (obj->oclass == SPBOOK_CLASS && (objects[obj->otyp].oc_subtyp == BOOKTYPE_NOVEL || objects[obj->otyp].oc_subtyp == BOOKTYPE_MANUAL))
    {
        pline("%s already has a published name.", Ysimple_name2(obj));
        return;
    }

    if (has_oname(obj) && obj->nknown) {
        pline("%s already has a true name.", Yname2(obj));
        return;
    }

    Sprintf(qbuf, "What do you want to name %s ",
            is_plural(obj) ? "these" : "this");
    (void) safe_qbuf(qbuf, qbuf, "?", obj, xname, simpleonames, "item");
    getlin(qbuf, buf);
    if (!*buf || *buf == '\033')
        return;
    /* strip leading and trailing spaces; unnames item if all spaces */
    (void) mungspaces(buf);

#if 0
    /*
     * We don't violate illiteracy conduct here, although it is
     * arguable that we should for anything other than "X".  Doing so
     * would make attaching player's notes to hero's inventory have an
     * in-game effect, which may or may not be the correct thing to do.
     *
     * We do violate illiteracy in oname() if player creates Sting or
     * Orcrist, clearly being literate (no pun intended...).
     */

    /* relax restrictions over proper capitalization for artifacts */
    if ((aname = artifact_name(buf, &objtyp)) != 0 && objtyp == obj->otyp)
        Strcpy(buf, aname);

    if (obj->oartifact) {
        pline_The("artifact seems to resist the attempt.");
        return;
    } else if (restrict_name(obj, buf) || exist_artifact(obj->otyp, buf)) {
        /* this used to change one letter, substituting a value
           of 'a' through 'y' (due to an off by one error, 'z'
           would never be selected) and then force that to
           upper case if such was the case of the input;
           now, the hand slip scuffs one or two letters as if
           the text had been trodden upon, sometimes picking
           punctuation instead of an arbitrary letter;
           unfortunately, we have to cover the possibility of
           it targetting spaces so failing to make any change
           (we know that it must eventually target a nonspace
           because buf[] matches a valid artifact name) */
        Strcpy(bufcpy, buf);
        /* for "the Foo of Bar", only scuff "Foo of Bar" part */
        bufp = !strncmpi(bufcpy, "the ", 4) ? (buf + 4) : buf;
        do {
            wipeout_text(bufp, rn2_on_display_rng(2), (unsigned) 0);
        } while (!strcmp(buf, bufcpy));
        pline("While engraving, your %s slips.", body_part(HAND));
        display_nhwindow(WIN_MESSAGE, FALSE);
        You("engrave: \"%s\".", buf);
        /* violate illiteracy conduct since hero attempted to write
           a valid artifact name */
        u.uconduct.literate++;
    }
#endif
    //++via_naming; /* This ought to be an argument rather than a static... */
    obj = uoname(obj, buf);
    //--via_naming; /* ...but oname() is used in a lot of places, so defer. */
}

struct obj *
oname(obj, name)
struct obj *obj;
const char *name;
{
    size_t lth;
    char buf[PL_PSIZ];

    lth = *name ? (strlen(name) + 1) : 0;
    if (lth > PL_PSIZ) {
        lth = PL_PSIZ;
        name = strncpy(buf, name, PL_PSIZ - 1);
        buf[PL_PSIZ - 1] = '\0';
    }
    /* If named artifact exists in the game, do not create another.
     * Also trying to create an artifact shouldn't de-artifact
     * it (e.g. Excalibur from prayer). In this case the object
     * will retain its current name. */
    if (obj->oartifact || (lth && exist_artifact(obj->otyp, name)))
        return obj;

    new_oname(obj, lth); /* removes old name if one is present */
    if (lth)
        Strcpy(ONAME(obj), name);

    if (lth)
        artifact_exists(obj, name, TRUE);
    if (obj->oartifact) {
        if (obj->unpaid)
            alter_cost(obj, 0L);
        if (via_naming) {
            /* violate illiteracy conduct since successfully wrote arti-name */
            if (!u.uconduct.literate++)
                livelog_printf(LL_CONDUCT | LL_ARTIFACT, "became literate by naming %s",
                    bare_artifactname(obj));
            else
                livelog_printf(LL_ARTIFACT, "chose %s to be named \"%s\"",
                    ansimpleoname(obj), bare_artifactname(obj));
        }
    }
    if (carried(obj))
        update_inventory();
    return obj;
}


struct obj*
uoname(obj, name)
struct obj* obj;
const char* name;
{
    size_t lth;
    char buf[PL_PSIZ];

    lth = *name ? (strlen(name) + 1) : 0;
    if (lth > PL_PSIZ) {
        lth = PL_PSIZ;
        name = strncpy(buf, name, PL_PSIZ - 1);
        buf[PL_PSIZ - 1] = '\0';
    }
    new_uoname(obj, lth); /* removes old name if one is present */
    if (lth)
        Strcpy(UONAME(obj), name);

    if (carried(obj))
        update_inventory();
    return obj;
}

STATIC_VAR NEARDATA const char callable[] = {
    SCROLL_CLASS, POTION_CLASS, WAND_CLASS,  RING_CLASS, AMULET_CLASS, FOOD_CLASS,
    GEM_CLASS, SPBOOK_CLASS, ARMOR_CLASS, TOOL_CLASS, MISCELLANEOUS_CLASS, WEAPON_CLASS, 0
};

boolean
objtyp_is_callable(i)
int i;
{
    return (boolean) (objects[i].oc_uname
                      || (OBJ_DESCR(objects[i])
                          && index(callable, objects[i].oc_class)));
}

int
doname_specific_object()
{
    struct obj* obj = 0;
    char allowall[2];
    allowall[0] = ALL_CLASSES;
    allowall[1] = '\0';
    obj = getobj(allowall, "name", 0, "");
    if (obj)
        do_uoname(obj);

    return 0;
}

int
doname_type_of_object()
{
    struct obj* obj = 0;
    obj = getobj(callable, "call", 0, "");
    if (obj) {
        /* behave as if examining it in inventory;
           this might set dknown if it was picked up
           while blind and the hero can now see */
        (void)xname(obj);

        if (!obj->dknown) {
            You("would never recognize another one.");
        }
        else {
            docall(obj, (char*)0);
        }

    }
    return 0;
}

/* C and #name commands - player can name monster or object or type of obj */
int
docallcmd()
{
    struct obj *obj;
    winid win;
    anything any;
    menu_item *pick_list = 0;
    char ch, allowall[2];
    /* if player wants a,b,c instead of i,o when looting, do that here too */
    boolean abc = flags.lootabc;

    if (getobj_autoselect_obj && !invent)
        getobj_autoselect_obj = 0;

    win = create_nhwindow(NHW_MENU);
    start_menu_ex(win, GHMENU_STYLE_CHOOSE_COMMAND);
    any = zeroany;
    if (!getobj_autoselect_obj)
    {
        any.a_char = 'm'; /* group accelerator 'C' */
        add_menu(win, NO_GLYPH, &any, abc ? 0 : any.a_char, 'C', ATR_NONE, NO_COLOR,
            "a monster", MENU_UNSELECTED);
    }
    if (invent) {
        /* we use y and n as accelerators so that we can accept user's
            response keyed to old "name an individual object?" prompt */
        any.a_char = 'i'; /* group accelerator 'y' */
        add_menu(win, NO_GLYPH, &any, abc ? 0 : any.a_char, 'y', ATR_NONE, NO_COLOR,
            "a particular object in inventory", MENU_UNSELECTED);
        any.a_char = 'o'; /* group accelerator 'n' */
        add_menu(win, NO_GLYPH, &any, abc ? 0 : any.a_char, 'n', ATR_NONE, NO_COLOR,
            "the type of an object in inventory", MENU_UNSELECTED);
    }
    if (!getobj_autoselect_obj)
    {
        any.a_char = 'f'; /* group accelerator ',' (or ':' instead?) */
        add_menu(win, NO_GLYPH, &any, abc ? 0 : any.a_char, ',', ATR_NONE, NO_COLOR,
            "the type of an object upon the floor", MENU_UNSELECTED);
        any.a_char = 'd'; /* group accelerator '\' */
        add_menu(win, NO_GLYPH, &any, abc ? 0 : any.a_char, '\\', ATR_NONE, NO_COLOR,
            "the type of an object on discoveries list", MENU_UNSELECTED);
        any.a_char = 'a'; /* group accelerator 'l' */
        add_menu(win, NO_GLYPH, &any, abc ? 0 : any.a_char, 'l', ATR_NONE, NO_COLOR,
            "record an annotation for the current level", MENU_UNSELECTED);
        end_menu(win, "What do you want to name?");
    }
    if (select_menu(win, PICK_ONE, &pick_list) > 0) {
        ch = pick_list[0].item.a_char;
        free((genericptr_t)pick_list);
    }
    else
        ch = 'q';
    destroy_nhwindow(win);

    switch (ch) {
    default:
    case 'q':
        break;
    case 'm': /* name a visible monster */
        do_mname();
        break;
    case 'i': /* name an individual object in inventory */
        allowall[0] = ALL_CLASSES;
        allowall[1] = '\0';
        obj = getobj(allowall, "name", 0, "");
        if (obj)
            do_uoname(obj);
        break;
    case 'o': /* name a type of object in inventory */
        obj = getobj(callable, "call", 0, "");
        if (obj) {
            /* behave as if examining it in inventory;
               this might set dknown if it was picked up
               while blind and the hero can now see */
            (void) xname(obj);

            if (!obj->dknown) {
                You("would never recognize another one.");
#if 0
            } else if (!objtyp_is_callable(obj->otyp)) {
                You("know those as well as you ever will.");
#endif
            } else {
                docall(obj, (char*)0);
            }
        }
        break;
    case 'f': /* name a type of object visible on the floor */
        namefloorobj();
        break;
    case 'd': /* name a type of object on the discoveries list */
        rename_disco();
        break;
    case 'a': /* annotate level */
        donamelevel();
        break;
    }
    return 0;
}

/* for use by safe_qbuf() */
STATIC_PTR char *
docall_xname(obj)
struct obj *obj;
{
    struct obj otemp;

    otemp = *obj;
    otemp.oextra = (struct oextra *) 0;
    otemp.quan = 1L;
    /* in case water is already known, convert "[un]holy water" to "water" */
    otemp.blessed = otemp.cursed = 0;
    /* remove attributes that are doname() caliber but get formatted
       by xname(); most of these fixups aren't really needed because the
       relevant type of object isn't callable so won't reach this far */
    if (otemp.oclass == WEAPON_CLASS)
    {
        otemp.opoisoned = 0; /* not poisoned */
        otemp.elemental_enchantment = 0; /* not specially enchanted */
        otemp.exceptionality = 0; /* not specially crafted */
        otemp.mythic_prefix = 0; /* not mythic */
        otemp.mythic_suffix = 0; /* not mythic */
    }
    else if (otemp.oclass == POTION_CLASS)
        otemp.odiluted = 0; /* not diluted */
    else if (otemp.otyp == TOWEL || otemp.otyp == STATUE)
        otemp.special_quality = 0; /* not wet or historic */
    else if (otemp.otyp == TIN)
        otemp.known = 0; /* suppress tin type (homemade, &c) and mon type */
    else if (otemp.otyp == FIGURINE)
        otemp.corpsenm = NON_PM; /* suppress mon type */
    else if (otemp.otyp == HEAVY_IRON_BALL)
        otemp.owt = objects[HEAVY_IRON_BALL].oc_weight; /* not "very heavy" */
    else if (otemp.oclass == FOOD_CLASS && otemp.globby)
        otemp.owt = 120; /* 6*20, neither a small glob nor a large one */

    return an(xname(&otemp));
}

void
docall(obj, introline)
struct obj *obj;
const char* introline;
{
    char buf[BUFSZ] = DUMMY, qbuf[QBUFSZ];
    char **str1;

    if (!obj->dknown)
        return; /* probably blind */

    flush_screen(1); /* Make sure that the screen shows the effect, if any, before query */

    if (obj->oclass == POTION_CLASS && (obj->speflags & SPEFLAGS_FROM_SINK))
        /* kludge, meaning it's sink water */
        Sprintf(qbuf, "Call a stream of %s fluid:",
                OBJ_DESCR(objects[obj->otyp]));
    else
        (void) safe_qbuf(qbuf, "Call ", ":", obj,
                         docall_xname, simpleonames, "thing");
    /* pointer to old name */
    str1 = &(objects[obj->otyp].oc_uname);
    buf[0] = '\0';
#ifdef EDIT_GETLIN
    /* if there's an existing name, make it be the default answer */
    if (*str1)
    {
        Strncpy(buf, *str1, BUFSZ - 1);
        buf[BUFSZ - 1] = '\0';
    }
#endif
#ifdef ANDROID
    if (1) /* was showlog but that was always TRUE */
        and_getlin_log(qbuf, buf);
    else
#endif

    getlin_ex(GETLINE_GENERAL, ATR_NONE, NO_COLOR, qbuf, buf, (char*)0, (char*)0, introline);

    if (!*buf || *buf == '\033')
        return;

    /* clear old name */
    str1 = &(objects[obj->otyp].oc_uname);
    if (*str1)
        free((genericptr_t) *str1);

    /* strip leading and trailing spaces; uncalls item if all spaces */
    (void) mungspaces(buf);
    if (!*buf) {
        if (*str1) { /* had name, so possibly remove from disco[] */
            /* strip name first, for the update_inventory() call
               from undiscover_object() */
            *str1 = (char *) 0;
            undiscover_object(obj->otyp);
        }
    } else {
        buf[PL_PSIZ - 1] = '\0'; /* Truncate to 62 chars */
        *str1 = dupstr(buf);
        discover_object(obj->otyp, FALSE, TRUE); /* possibly add to disco[] */
    }
}

STATIC_OVL void
namefloorobj()
{
    coord cc;
    int glyph;
    char buf[BUFSZ];
    struct obj *obj = 0;
    boolean fakeobj = FALSE, use_plural;

    cc.x = u.ux, cc.y = u.uy;
    /* "dot for under/over you" only makes sense when the cursor hasn't
       been moved off the hero's '@' yet, but there's no way to adjust
       the help text once getpos() has started */
    Sprintf(buf, "object on map (or '.' for one %s you)",
            (u.uundetected && hides_under(youmonst.data)) ? "over" : "under");
    if (getpos(&cc, FALSE, buf, CURSOR_STYLE_NAME_CURSOR) < 0 || cc.x <= 0)
        return;
    if (cc.x == u.ux && cc.y == u.uy) {
        obj = vobj_at(u.ux, u.uy);
    } else {
        glyph = glyph_at(cc.x, cc.y);
        if (glyph_is_object(glyph))
            fakeobj = object_from_map(glyph, cc.x, cc.y, &obj);
        /* else 'obj' stays null */
    }
    if (!obj) {
        /* "under you" is safe here since there's no object to hide under */
        pline("There doesn't seem to be any object %s.",
              (cc.x == u.ux && cc.y == u.uy) ? "under you" : "there");
        return;
    }
    /* note well: 'obj' might be an instance of STRANGE_OBJECT if target
       is a mimic; passing that to xname (directly or via simpleonames)
       would yield "glorkum" so we need to handle it explicitly; it will
       always fail the Hallucination test and pass the !callable test,
       resulting in the "can't be assigned a type name" message */
    Strcpy(buf, (obj->otyp != STRANGE_OBJECT)
                 ? simpleonames(obj)
                 : obj_descr[STRANGE_OBJECT].oc_name);
    use_plural = (obj->quan > 1L);
    if (Hallucination) {
        const char *unames[6];
        char tmpbuf[BUFSZ];

        /* straight role name */
        unames[0] = ((Upolyd ? u.mfemale : flags.female) && urole.name.f)
                     ? urole.name.f
                     : urole.name.m;
        /* random rank title for hero's role

           note: the 30 is hardcoded in xlev_to_rank, so should be
           hardcoded here too */
        unames[1] = rank_of(rn2_on_display_rng(30) + 1,
                            Role_switch, flags.female);
        /* random fake monster */
        unames[2] = bogusmon(tmpbuf, (char *) 0);
        /* increased chance for fake monster */
        unames[3] = unames[2];
        /* traditional */
        unames[4] = roguename();
        /* silly */
        unames[5] = "Wibbly Wobbly";
        pline("%s %s to call you \"%s.\"",
              The(buf), use_plural ? "decide" : "decides",
              unames[rn2_on_display_rng(SIZE(unames))]);
    } else if (!objtyp_is_callable(obj->otyp)) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s can't be assigned a type name.",
              use_plural ? "Those" : "That", buf);
    } else if (!obj->dknown) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "don't know %s %s well enough to name %s.",
            use_plural ? "those" : "that", buf, use_plural ? "them" : "it");
    } else {
        docall(obj, (char*)0);
    }
    if (fakeobj) {
        obj->where = OBJ_FREE; /* object_from_map() sets it to OBJ_FLOOR */
        dealloc_obj(obj);
    }
}

STATIC_VAR const char *const ghostnames[] = {
    /* these names should have length < PL_NSIZ */
    /* Capitalize the names for aesthetics -dgk */
    "Adri",    "Andries",       "Andreas",     "Bert",    "David",  "Dirk",
    "Emile",   "Frans",         "Fred",        "Greg",    "Hether", "Jay",
    "John",    "Jon",           "Karnov",      "Kay",     "Kenny",  "Kevin",
    "Maud",    "Michiel",       "Mike",        "Peter",   "Robert", "Ron",
    "Tom",     "Wilmar",        "Nick Danger", "Phoenix", "Jiro",   "Mizue",
    "Stephan", "Lance Braccus", "Shadowhawk"
};

/* ghost names formerly set by x_monnam(), now by makemon() instead */
const char *
rndghostname()
{
    return rn2(7) ? ghostnames[rn2(SIZE(ghostnames))] : (const char *) plname;
}

/*
 * Monster naming functions:
 * x_monnam is the generic monster-naming function.
 *                seen        unseen       detected               named
 * mon_nam:     the newt        it      the invisible orc       Fido
 * noit_mon_nam:the newt (as if detected) the invisible orc     Fido
 * l_monnam:    newt            it      invisible orc           dog called Fido
 * Monnam:      The newt        It      The invisible orc       Fido
 * noit_Monnam: The newt (as if detected) The invisible orc     Fido
 * Adjmonnam:   The poor newt   It      The poor invisible orc  The poor Fido
 * Amonnam:     A newt          It      An invisible orc        Fido
 * a_monnam:    a newt          it      an invisible orc        Fido
 * m_monnam:    newt            xan     orc                     Fido
 * y_monnam:    your newt     your xan  your invisible orc      Fido
 * Ymonnam:    Your newt     Your xan  Your invisible orc      Fido
 * noname_monnam(mon,article):
 *              article newt    art xan art invisible orc       art dog
 */

/* Bug: if the monster is a priest or shopkeeper, not every one of these
 * options works, since those are special cases.
 */
char *
x_monnam(mtmp, article, adjective, suppress, called)
register struct monst *mtmp;
int article;
/* ARTICLE_NONE, ARTICLE_THE, ARTICLE_A: obvious
 * ARTICLE_YOUR: "your" on pets, "the" on everything else
 *
 * If the monster would be referred to as "it" or if the monster has a name
 * _and_ there is no adjective, "invisible", "saddled", etc., override this
 * and always use no article.
 */
const char *adjective;
int suppress;
/* SUPPRESS_IT, SUPPRESS_INVISIBLE, SUPPRESS_HALLUCINATION, SUPPRESS_SADDLE.
 * EXACT_NAME: combination of all the above
 * SUPPRESS_NAME: omit monster's assigned name (unless uniq w/ pname).
 */
boolean called;
{
    char *buf = nextmbuf();
    struct permonst *mdat = mtmp->data;
    const char* pm_name = mon_monster_name(mtmp); // mdat->mname;
    boolean do_hallu, do_invis, do_it, do_saddle, do_name;
    boolean name_at_start = FALSE, has_adjectives = FALSE;
    /* note: uname is always at end */
    char *bp;

    /* Remove gcc warning */
    if (called)
    {
        /* Do nothing here */
    }

    if (program_state.gameover)
        suppress |= SUPPRESS_HALLUCINATION;
    if (article == ARTICLE_YOUR && !is_tame(mtmp))
        article = ARTICLE_THE;

    do_hallu = Hallucination && !(suppress & SUPPRESS_HALLUCINATION);
    do_invis = is_invisible(mtmp) && !(suppress & SUPPRESS_INVISIBLE);
    do_it = !canspotmon(mtmp) && article != ARTICLE_YOUR
            && !program_state.gameover && mtmp != u.usteed
            && !(u.uswallow && mtmp == u.ustuck) && !(suppress & SUPPRESS_IT);
    do_saddle = !(suppress & SUPPRESS_SADDLE);
    do_name = !(suppress & SUPPRESS_NAME) || is_mname_proper_name(mdat);

    buf[0] = '\0';

    /* unseen monsters, etc.  Use "it" */
    if (do_it) {
        Strcpy(buf, "it");
        return buf;
    }

    /* priests and minions: don't even use this function */
    if (mtmp->ispriest || mtmp->isminion) {
        char priestnambuf[BUFSZ];
        char *name;
        long save_prop = EHalluc_resistance;
        unsigned short save_invis = mtmp->mprops[INVISIBILITY];

        /* when true name is wanted, explicitly block Hallucination */
        if (!do_hallu)
            EHalluc_resistance = 1L;
        if (!do_invis)
            mtmp->mprops[INVISIBILITY] = 0;
        name = priestname(mtmp, priestnambuf);
        EHalluc_resistance = save_prop;
        mtmp->mprops[INVISIBILITY] = save_invis;
        if (article == ARTICLE_NONE && !strncmp(name, "the ", 4))
            name += 4;
        return strcpy(buf, name);
    }
    /* an "aligned priest" not flagged as a priest or minion should be
       "priest" or "priestess" (normally handled by priestname()) */
    if (mdat == &mons[PM_ALIGNED_PRIEST])
        pm_name = mtmp->female ? "priestess" : "priest";

    /* Shopkeepers: use shopkeeper name.  For normal shopkeepers, just
     * "Asidonhopo"; for unusual ones, "Asidonhopo the invisible
     * shopkeeper" or "Asidonhopo the blue dragon".  If hallucinating,
     * none of this applies.
     */
    if (mtmp->isshk && !do_hallu) 
    {
        if (adjective && article == ARTICLE_THE) 
        {
            /* pathological case: "the angry Asidonhopo the blue dragon"
               sounds silly */
            Strcpy(buf, "the ");
            Strcat(strcat(buf, adjective), " ");
            Strcat(buf, shkname(mtmp));
            return buf;
        }
        Strcat(buf, shkname(mtmp));
        if (mdat == &mons[PM_SHOPKEEPER] && !do_invis)
            return buf;
        Strcat(buf, " the ");
        if (do_invis)
            Strcat(buf, "invisible ");
        Strcat(buf, pm_name);
        return buf;
    }

    /* Put the adjectives in the buffer */
    if (adjective)
        Strcat(strcat(buf, adjective), " ");
    if (do_invis)
        Strcat(buf, "invisible ");
    if (do_saddle && (mtmp->worn_item_flags & W_SADDLE) && !Blind
        && !Hallucination)
        Strcat(buf, "saddled ");



    has_adjectives = (buf[0] != '\0');

    /* Put the actual monster name or type into the buffer now */
    /* Be sure to remember whether the buffer starts with a name */
    if (do_hallu) 
    {
        char rnamecode;
        char *rname = rndmonnam(&rnamecode);

        Strcat(buf, rname);
        name_at_start = bogon_is_pname(rnamecode);
    }
    else if (do_name && has_umname(mtmp))
    {
        char* umname = UMNAME(mtmp);
        Strcat(buf, umname);
        name_at_start = TRUE;
    }
    else if (do_name && has_mname(mtmp) && mtmp->u_know_mname)
    {
        char *name = MNAME(mtmp);

        if (mdat == &mons[PM_GHOST])
        {
            Sprintf(eos(buf), "%s ghost", s_suffix(name));
            name_at_start = TRUE;
        } 
        else if (is_mplayer(mdat) && (bp = strstri(name, " the ")) != 0) 
        {
            /* <name> the <adjective> <invisible> <saddled> <rank> */
            char pbuf[BUFSZ];

            Strcpy(pbuf, name);
            pbuf[bp - name + 5] = '\0'; /* adjectives right after " the " */
            if (has_adjectives)
                Strcat(pbuf, buf);
            Strcat(pbuf, bp + 5); /* append the rest of the name */
            Strcpy(buf, pbuf);
            article = ARTICLE_NONE;
            name_at_start = TRUE;
        } 
        else 
        {
            char tmpbuf[BUFSIZ];
            Strcpy(tmpbuf, buf);
            boolean npc_with_name_only = has_enpc(mtmp) && mtmp->isnpc && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_DISPLAY_NAME_ONLY) != 0;
            if (!is_tame(mtmp) && !npc_with_name_only)
            {
                if ((bp = strstri(name, " the ")) != 0)
                    Sprintf(buf, "%s%s %s", tmpbuf, pm_name, name);
                else
                    Sprintf(buf, "%s%s the %s", tmpbuf, name, pm_name);
            }
            else
                Sprintf(buf, "%s%s", tmpbuf, name);

            name_at_start = TRUE;
        }
    } 
    else if (is_mplayer(mdat) && !In_endgame(&u.uz)) 
    {
        char pbuf[BUFSZ];

        Strcpy(pbuf, rank_of((int) mtmp->m_lev, mtmp->mnum,
                             (boolean) mtmp->female != 0));
        Strcat(buf, lcase(pbuf));
        name_at_start = FALSE;
    } 
    else 
    {
        Strcat(buf, pm_name);
        name_at_start = (boolean) is_mname_proper_name(mdat);
    }

    /* Write article or your in start */
    if (name_at_start && (article == ARTICLE_YOUR || !has_adjectives)) {
        if (mdat == &mons[PM_WIZARD_OF_YENDOR])
            article = ARTICLE_THE;
        else
            article = ARTICLE_NONE;
    } else if ((mdat->geno & G_UNIQ) && article == ARTICLE_A) {
        article = ARTICLE_THE;
    }

    {
        char buf2[BUFSZ];

        switch (article) {
        case ARTICLE_YOUR:
            Strcpy(buf2, "your ");
            Strcat(buf2, buf);
            Strcpy(buf, buf2);
            return buf;
        case ARTICLE_THE:
            Strcpy(buf2, "the ");
            Strcat(buf2, buf);
            Strcpy(buf, buf2);
            return buf;
        case ARTICLE_A:
            return an(buf);
        case ARTICLE_NONE:
        default:
            return buf;
        }
    }
}

char *
l_monnam(mtmp)
struct monst *mtmp;
{
    return x_monnam(mtmp, ARTICLE_NONE, (char *) 0,
                    (has_mname(mtmp)) ? SUPPRESS_SADDLE : 0, TRUE);
}

char *
mon_nam(mtmp)
struct monst *mtmp;
{
    return x_monnam(mtmp, ARTICLE_THE, (char *) 0,
                    (has_mname(mtmp)) ? SUPPRESS_SADDLE : 0, FALSE);
}

/* print the name as if mon_nam() was called, but assume that the player
 * can always see the monster--used for probing and for monsters aggravating
 * the player with a cursed potion of invisibility
 */
char *
noit_mon_nam(mtmp)
struct monst *mtmp;
{
    return x_monnam(mtmp, ARTICLE_THE, (char *) 0,
                    (has_mname(mtmp)) ? (SUPPRESS_SADDLE | SUPPRESS_IT)
                                      : SUPPRESS_IT,
                    FALSE);
}

char*
noittame_mon_nam(mtmp)
struct monst* mtmp;
{
    if (mtmp && is_tame(mtmp))
        return noit_mon_nam(mtmp);
    else
        return mon_nam(mtmp);
}

char *
Monnam(mtmp)
struct monst *mtmp;
{
    register char *bp = mon_nam(mtmp);

    *bp = highc(*bp);
    return  bp;
}

char *
noit_Monnam(mtmp)
struct monst *mtmp;
{
    register char *bp = noit_mon_nam(mtmp);

    *bp = highc(*bp);
    return  bp;
}

char*
noittame_Monnam(mtmp)
struct monst* mtmp;
{
    if (mtmp && is_tame(mtmp))
        return noit_Monnam(mtmp);
    else
        return Monnam(mtmp);
}

/* return "a dog" rather than "Fido", honoring hallucination and visibility */
char *
noname_monnam(mtmp, article)
struct monst *mtmp;
int article;
{
    return x_monnam(mtmp, article, (char *) 0, SUPPRESS_NAME, FALSE);
}

/* monster's own name -- overrides hallucination and [in]visibility
   so shouldn't be used in ordinary messages (mainly for disclosure) */
char *
m_monnam(mtmp)
struct monst *mtmp;
{
    return x_monnam(mtmp, ARTICLE_NONE, (char *) 0, EXACT_NAME, FALSE);
}

/* pet name: "your little dog" */
char *
y_monnam(mtmp)
struct monst *mtmp;
{
    int prefix, suppression_flag;

    prefix = is_tame(mtmp) ? ARTICLE_YOUR : ARTICLE_THE;
    suppression_flag = (has_mname(mtmp)
                        /* "saddled" is redundant when mounted */
                        || mtmp == u.usteed)
                           ? SUPPRESS_SADDLE
                           : 0;

    return x_monnam(mtmp, prefix, (char *) 0, suppression_flag, FALSE);
}

/* pet name: "Your little dog" */
char*
Ymonnam(mtmp)
struct monst* mtmp;
{
    return upstart(y_monnam(mtmp));
}

char *
Adjmonnam(mtmp, adj)
struct monst *mtmp;
const char *adj;
{
    char *bp = x_monnam(mtmp, ARTICLE_THE, adj,
                        has_mname(mtmp) ? SUPPRESS_SADDLE : 0, FALSE);

    *bp = highc(*bp);
    return  bp;
}

char *
a_monnam(mtmp)
struct monst *mtmp;
{
    return x_monnam(mtmp, ARTICLE_A, (char *) 0,
                    has_mname(mtmp) ? SUPPRESS_SADDLE : 0, FALSE);
}

char *
Amonnam(mtmp)
struct monst *mtmp;
{
    char *bp = a_monnam(mtmp);

    *bp = highc(*bp);
    return  bp;
}

/* used for monster ID by the '/', ';', and 'C' commands to block remote
   identification of the endgame altars via their attending priests */
char *
distant_monnam(mon, article, outbuf)
struct monst *mon;
int article; /* only ARTICLE_NONE and ARTICLE_THE are handled here */
char *outbuf;
{
    /* high priest(ess)'s identity is concealed on the Astral Plane,
       unless you're adjacent (overridden for hallucination which does
       its own obfuscation) */
    if (mon->data == &mons[PM_HIGH_PRIEST] && !Hallucination
        && Is_astralevel(&u.uz) && distu(mon->mx, mon->my) > 2) {
        Strcpy(outbuf, article == ARTICLE_THE ? "the " : "");
        Strcat(outbuf, mon->female ? "high priestess" : "high priest");
    } else {
        Strcpy(outbuf, x_monnam(mon, article, (char *) 0, 0, TRUE));
    }

    if (mon->data->mtitle && strcmp(mon->data->mtitle, ""))
    {
        Sprintf(eos(outbuf), ", %s", mon->data->mtitle);
    }

    if (mon->data->msound == MS_LEADER && quest_info(MS_LEADER) == mon->mnum)
    {
        strcat(outbuf, ", quest leader");
    }
    else if (mon->data->msound == MS_NEMESIS && quest_info(MS_NEMESIS) == mon->mnum)
    {
        strcat(outbuf, ", quest nemesis");
    }

    return outbuf;
}

/* fake monsters used to be in a hard-coded array, now in a data file */
STATIC_OVL char *
bogusmon(buf, code)
char *buf, *code;
{
    char *mname = buf;

    get_rnd_text(BOGUSMONFILE, buf, rn2_on_display_rng);
    /* strip prefix if present */
    if (!letter(*mname)) {
        if (code)
            *code = *mname;
        ++mname;
    } else {
        if (code)
            *code = '\0';
    }
    return mname;
}

/* return a random monster name, for hallucination */
char *
rndmonnam(code)
char *code;
{
    static char buf[BUFSZ];
    char *mname;
    int name;
#define BOGUSMONSIZE 100 /* arbitrary */

    if (code)
        *code = '\0';

    do {
        name = rn2_on_display_rng(SPECIAL_PM + BOGUSMONSIZE - LOW_PM) + LOW_PM;
    } while (name < SPECIAL_PM
             && (is_mname_proper_name(&mons[name]) || (mons[name].geno & G_NOGEN)));

    if (name >= SPECIAL_PM) {
        mname = bogusmon(buf, code);
    } else {
        mname = strcpy(buf, pm_monster_name(&mons[name], !rn2(2)));
    }
    return mname;
#undef BOGUSMONSIZE
}

/* check bogusmon prefix to decide whether it's a personal name */
boolean
bogon_is_pname(code)
char code;
{
    if (!code)
        return FALSE;
    return index("-+=", code) ? TRUE : FALSE;
}

/* name of a Rogue player */
const char *
roguename()
{
    char *i, *opts;

    if ((opts = nh_getenv("ROGUEOPTS")) != 0) {
        for (i = opts; *i; i++)
            if (!strncmp("name=", i, 5)) {
                char *j;
                if ((j = index(i + 5, ',')) != 0)
                    *j = (char) 0;
                return i + 5;
            }
    }
    return rn2(3) ? (rn2(2) ? "Michael Toy" : "Kenneth Arnold")
                  : "Glenn Wichman";
}

#define NUM_HCOLORS 34
STATIC_VAR NEARDATA const char *const hcolors[NUM_HCOLORS] = {
    "ultraviolet", "infrared", "bluish-orange", "reddish-green", "dark white",
    "light black", "sky blue-pink", "salty", "sweet", "sour", "bitter",
    "striped", "spiral", "swirly", "plaid", "checkered", "argyle", "paisley",
    "blotchy", "guernsey-spotted", "polka-dotted", "square", "round",
    "triangular", "cabernet", "sangria", "fuchsia", "wisteria", "lemon-lime",
    "strawberry-banana", "peppermint", "romantic", "incandescent",
    "octarine", /* Discworld: the Colour of Magic */
};

STATIC_VAR NEARDATA uchar const hcolors2nhcolor[NUM_HCOLORS] = {
    CLR_MAGENTA, CLR_RED, CLR_ORANGE, CLR_GREEN, CLR_GRAY,
    CLR_GRAY, CLR_BRIGHT_MAGENTA, CLR_MAX, CLR_MAX, CLR_MAX, CLR_MAX,
    CLR_MAX, CLR_MAX, CLR_MAX, CLR_MAX, CLR_MAX, CLR_MAX, CLR_MAX,
    CLR_MAX, CLR_MAX, CLR_MAX, CLR_MAX, CLR_MAX,
    CLR_MAX, CLR_RED, CLR_RED, CLR_MAX, CLR_MAX, CLR_BRIGHT_GREEN,
    CLR_ORANGE, CLR_MAX, CLR_RED, CLR_BRIGHT_MAGENTA,
    CLR_MAX, /* Discworld: the Colour of Magic */
};

const char *
hcolor(colorpref)
const char *colorpref;
{
    return hcolor_multi(colorpref, (int*)0, 0);
    //return (Hallucination || !colorpref)
    //    ? hcolors[rn2_on_display_rng(NUM_HCOLORS)]
    //    : colorpref;
}

const char*
hcolor_multi_buf0(colorpref)
const char* colorpref;
{
    return hcolor_multi(colorpref, get_colorless_multicolor_buffer(), 0);
}

const char*
hcolor_multi_buf1(colorpref)
const char* colorpref;
{
    return hcolor_multi(colorpref, get_colorless_multicolor_buffer(), 1);
}

const char*
hcolor_multi_buf2(colorpref)
const char* colorpref;
{
    return hcolor_multi(colorpref, get_colorless_multicolor_buffer(), 2);
}

const char*
hcolor_multi_buf3(colorpref)
const char* colorpref;
{
    return hcolor_multi(colorpref, get_colorless_multicolor_buffer(), 3);
}

const char*
hcolor_multi_buf4(colorpref)
const char* colorpref;
{
    return hcolor_multi(colorpref, get_colorless_multicolor_buffer(), 4);
}

const char*
hcolor_multi(colorpref, multicolors_buf, multicolor_idx)
const char* colorpref;
int* multicolors_buf;
int multicolor_idx;
{
    if (colorpref && !*colorpref)
    {
        return colorpref;
    }
    else if (Hallucination || !colorpref)
    {
        int random_color = rn2_on_display_rng(NUM_HCOLORS);
        if (multicolors_buf)
        {
            int nhcolor = (int)hcolors2nhcolor[random_color];
            if (nhcolor == CLR_MAX)
            {
                nhcolor = rn2(CLR_MAX - 1); /* All colors are equally likely, and NO_COLOR implies CLR_WHITE */
                if (nhcolor == NO_COLOR)
                    nhcolor = CLR_WHITE;
            }
            multicolors_buf[multicolor_idx] = nhcolor;
        }
        return hcolors[random_color] ? hcolors[random_color] : "colorless";
    }
    else
    {
        if (multicolors_buf)
            multicolors_buf[multicolor_idx] = color_name_to_nhcolor(colorpref);
        return colorpref;
    }
}

int
color_name_to_nhcolor(colorpref)
const char* colorpref;
{
    int nhcolor;
    if (!colorpref || !*colorpref || colorpref == NH_COLORLESS)
        nhcolor = NO_COLOR;
    else if (colorpref == NH_BLACK)
        nhcolor = CLR_BLACK;
    else if (colorpref == NH_AMBER)
        nhcolor = CLR_YELLOW;
    else if (colorpref == NH_GOLDEN)
        nhcolor = HI_GOLD;
    else if (colorpref == NH_LIGHT_BLUE)
        nhcolor = CLR_BRIGHT_BLUE;
    else if (colorpref == NH_RED)
        nhcolor = CLR_RED;
    else if (colorpref == NH_GREEN)
        nhcolor = CLR_GREEN;
    else if (colorpref == NH_SILVER)
        nhcolor = HI_SILVER;
    else if (colorpref == NH_BLUE)
        nhcolor = CLR_BLUE;
    else if (colorpref == NH_PURPLE)
        nhcolor = CLR_MAGENTA;
    else if (colorpref == NH_WHITE)
        nhcolor = CLR_WHITE;
    else if (colorpref == NH_ORANGE)
        nhcolor = CLR_ORANGE;
    else if (colorpref == NH_BROWN)
        nhcolor = CLR_BROWN;
    else if (colorpref == NH_GRAY)
        nhcolor = CLR_GRAY;
    else if (colorpref == NH_DARK_RED)
        nhcolor = CLR_RED;
    else
    {
        nhcolor = clrnameptr2color(colorpref);
    }
    return nhcolor;
}

/* return a random real color unless hallucinating */
const char *
rndcolor()
{
    int k = rn2(CLR_MAX);

    return Hallucination ? hcolor((char *) 0)
                         : (k == NO_COLOR) ? "colorless"
                                           : c_obj_colors[k];
}

STATIC_VAR NEARDATA const char *const hliquids[] = {
    "yoghurt", "oobleck", "clotted blood", "diluted water", "purified water",
    "instant coffee", "tea", "herbal infusion", "liquid rainbow",
    "creamy foam", "mulled wine", "bouillon", "nectar", "grog", "flubber",
    "ketchup", "slow light", "oil", "vinaigrette", "liquid crystal", "honey",
    "caramel sauce", "ink", "aqueous humour", "milk substitute",
    "fruit juice", "glowing lava", "gastric acid", "mineral water",
    "cough syrup", "quicksilver", "sweet vitriol", "grey goo", "pink slime",
};

const char *
hliquid(liquidpref)
const char *liquidpref;
{
    return (Hallucination || !liquidpref) ? hliquids[rn2(SIZE(hliquids))]
                                          : liquidpref;
}

/* Aliases for road-runner nemesis
 */
STATIC_VAR const char *const coynames[] = {
    "Carnivorous Vulgaris", "Road-Runnerus Digestus", "Eatibus Anythingus",
    "Famishus-Famishus", "Eatibus Almost Anythingus", "Eatius Birdius",
    "Famishius Fantasticus", "Eternalii Famishiis", "Famishus Vulgarus",
    "Famishius Vulgaris Ingeniusi", "Eatius-Slobbius", "Hardheadipus Oedipus",
    "Carnivorous Slobbius", "Hard-Headipus Ravenus", "Evereadii Eatibus",
    "Apetitius Giganticus", "Hungrii Flea-Bagius", "Overconfidentii Vulgaris",
    "Caninus Nervous Rex", "Grotesques Appetitus", "Nemesis Ridiculii",
    "Canis latrans"
};

char *
coyotename(mtmp, buf)
struct monst *mtmp;
char *buf;
{
    if (mtmp && buf) {
        Sprintf(buf, "%s - %s",
                x_monnam(mtmp, ARTICLE_NONE, (char *) 0, 0, TRUE),
                is_cancelled(mtmp) ? coynames[SIZE(coynames) - 1]
                           : coynames[mtmp->m_id % (SIZE(coynames) - 1)]);
    }
    return buf;
}

char *
rndorcname(s)
char *s;
{
    static const char *v[] = { "a", "ai", "og", "u" };
    static const char *snd[] = { "gor", "gris", "un", "bane", "ruk",
                                 "oth","ul", "z", "thos","akh","hai" };
    int i, iend = rn1(2, 3), vstart = rn2(2);

    if (s) {
        *s = '\0';
        for (i = 0; i < iend; ++i) {
            vstart = 1 - vstart;                /* 0 -> 1, 1 -> 0 */
            Sprintf(eos(s), "%s%s", (i > 0 && !rn2(30)) ? "-" : "",
                    vstart ? v[rn2(SIZE(v))] : snd[rn2(SIZE(snd))]);
        }
    }
    return s;
}


char*
randomize_dwarf_name(s)
char* s;
{
    static const char* start[] = { "th", "m", "d", "thr", "g",
        "d", "t", "l", "b", "", "n", "f", "fr", "gr", "aggr" };
    static const char* v[] = { "a", "e", "i", "o", "u", "u", "eo", "y" };
    static const char* middle_vowel_end[] = { "l", "r", "g", "n" };
    static const char* mv[] = { "a", "e", "i", "o", "u" };
    static const char* middle_novowel_end[] = { "rog", "rg" };
    static const char* end_previous_vowel[] = { "din", "ddin", "rdin", "rin", "in", "mli", "lin", "nin", "sek", "rek", "rim", "im" , "r", "ri", "gas", "m", "dis", "di" };
    static const char* end_previous_novowel[] = { "din", "rin", "in", "lin", "rim", "im", "ri", "as", "dis" };

    if (s) {
        strcpy(s, "");
        if (!rn2(4))
        {
            /* with middle syllable */
            /* with no vowel end*/
            if(!rn2(3))
                Sprintf(s, "%s%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                    middle_novowel_end[rn2(SIZE(middle_novowel_end))], end_previous_novowel[rn2(SIZE(end_previous_novowel))]);
            else
                Sprintf(s, "%s%s%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                    middle_vowel_end[rn2(SIZE(middle_vowel_end))], mv[rn2(SIZE(mv))], end_previous_vowel[rn2(SIZE(end_previous_vowel))]);
        }
        else
            Sprintf(s, "%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                end_previous_vowel[rn2(SIZE(end_previous_vowel))]);
    }
    return s;
}

char*
randomize_elf_name(s)
char* s;
{
    static const char* start[] = { "", "", "", "g", "gal", "b", "c", "cel",
        "d", "l", "b", "n", "f", "f", "gl", "gw" };
    static const char* v[] = { "a", "e", "i", "o", "ae", "ea", "eo" };
    static const char* middle_vowel_end[] = { "l", "m", "g", "n", "br", "dr", "lemm", "ld", "lr", "r", "ng", "lg" };
    static const char* mv[] = { "a", "e", "i", "o" };
    static const char* middle_novowel_end[] = { "lad", "len", "m", "r", "l", "ren", "n", "galm", "galn", "ngr", "ng", "nr", "in", "th", "dhr", "gl", 
        "ht", "bl", "thr", "rd", "mr", "nd", "nn" };
    static const char* end_previous_vowel[] = { "riel", "rie", "drie", "driel", "dras", "dros", "drod", "dras", "ros", "rod", 
        "nael", "dhel", "n", "d", "r", "wen", "brian", "bor", "mbrian", "mbor", "rdan", "nor", "ron", 
        "lin", "dil", "hil", "dhil", "rdhil", "kil", "re", "we", "nd", "l", "lye", "dhon", "thil", "dor", "l-galad", "ndor", "las", "dir", "dreth",
        "phin", "lodh" };
    static const char* end_previous_novowel[] = { "iel", "ie", "as", "os", "od", "ael", "an", "on", "arfin", "olfin", "ion", "ien", "or",
        "in", "un", "ellas", "anel", "odel", "uil" };

    if (s) {
        strcpy(s, "");
        if (rn2(3))
        {
            /* with middle syllable */
            /* with no vowel end*/
            if (!rn2(2))
                Sprintf(s, "%s%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                    middle_novowel_end[rn2(SIZE(middle_novowel_end))], end_previous_novowel[rn2(SIZE(end_previous_novowel))]);
            else
                Sprintf(s, "%s%s%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                    middle_vowel_end[rn2(SIZE(middle_vowel_end))], mv[rn2(SIZE(mv))], end_previous_vowel[rn2(SIZE(end_previous_vowel))]);
        }
        else
            Sprintf(s, "%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                end_previous_vowel[rn2(SIZE(end_previous_vowel))]);
    }
    return s;
}

char*
randomize_gnome_name(s)
char* s;
{
    static const char* start[] = { "", "", "", "g", "h", "b", "p",
        "d", "l", "br", "n", "f", "sh", "w", "r", "z", "t", "fr", "gl", "v", "wr", "sch", "gn" };
    static const char* v[] = { "a", "e", "i", "o", "u", "u", "y", "ee", "oo", "uu" };
    static const char* middle_vowel_end[] = { "l", "lg", "m", "n", "lst", "lv", "dd", "mpn", "r", "ld", "rk", "nk", "rl", "d", "v", "rr", "pp", "shl" };
    static const char* mv[] = { "a", "e", "i", "o", "u", "y" };
    static const char* middle_novowel_end[] = { "m", "r", "l", "n" "ss", "s", "ll", 
        "mbl", "rn", "bl", "thr", "rd", "mr", "nd", "nn", "mp", "p", "rg", "pp", "lb", "shl", "rb", "lfr", "nk" };
    static const char* end_previous_vowel[] = { "cc", "ck", "x", "b", "n", "m", "ll", "nock", "wick", "wock", "mil",
        "k", "nn", "nottin", "mottin", "noodle", "lla", "ddo", "mip", "bell" };
    static const char* end_previous_novowel[] = { "a", "e", "o", "i", "y", "ey", "in", "yn", "en", 
        "oodle" "ottin", "owick", "owock", "iwick", "iwock", "wick", "wock", "ell", "ag", "ed" };

    if (s) 
    {
        strcpy(s, "");
        if (!rn2(3))
        {
            /* with middle syllable */
            /* with no vowel end*/
            if (!rn2(2))
                Sprintf(s, "%s%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                    middle_novowel_end[rn2(SIZE(middle_novowel_end))], end_previous_novowel[rn2(SIZE(end_previous_novowel))]);
            else
                Sprintf(s, "%s%s%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                    middle_vowel_end[rn2(SIZE(middle_vowel_end))], mv[rn2(SIZE(mv))], end_previous_vowel[rn2(SIZE(end_previous_vowel))]);
        }
        else
            Sprintf(s, "%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                end_previous_vowel[rn2(SIZE(end_previous_vowel))]);
    }
    return s;
}


char*
randomize_hobbit_name(s)
char* s;
{
    static const char* start[] = { "", "b", "bl", "s", "br",
        "d", "dr", "f", "fl", "fr", "g", "gr", "h", "l", "m", "p", "pr", "s", "t" };
    static const char* v[] = { "a", "e", "i", "o", "u" };
    static const char* middle_vowel_end[] = { "g", "l", "lg", "m", "n", "st", "ld" };
    static const char* mv[] = { "a", "e", "i", "o", "u", "y", "ia" };
    static const char* middle_novowel_end[] = { "g", "d" "m", "lb", "lgr", "nd", "ng", "cc", "n", "lc", "l", "lm"
        "sc", "ff", "th", "rr", "rtl", "nt", "lv", "pp", "rt", "nch" };
    static const char* end_previous_vowel[] = { "b", "c", "nd", "rd", "nta", "ca", "ll", "lac", "lbo", "do", "lba", "ll", "lla", 
        "mwise", "bert", "bart",  "mbert", "mbart", "bald",
        "fast", "mfast", "mfred", "grim", "brand", "gard", "fons", "bson", "mson", "mac", "doc", "dic", "mas", "gilda", "lot", "mrose" };
    static const char* end_previous_novowel[] = { "a", "e", "o", "o", "i", "y", "ida", "ard", "anta", "old", "ald"
        "obert", "ibert", "obart", "ibart", "an", "ibald",  "ibald" };

    if (s)
    {
        strcpy(s, "");
        if (rn2(3))
        {
            /* with middle syllable */
            /* with no vowel end*/
            if (rn2(4))
                Sprintf(s, "%s%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                    middle_novowel_end[rn2(SIZE(middle_novowel_end))], end_previous_novowel[rn2(SIZE(end_previous_novowel))]);
            else
                Sprintf(s, "%s%s%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                    middle_vowel_end[rn2(SIZE(middle_vowel_end))], mv[rn2(SIZE(mv))], end_previous_vowel[rn2(SIZE(end_previous_vowel))]);
        }
        else
            Sprintf(s, "%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                end_previous_vowel[rn2(SIZE(end_previous_vowel))]);
    }
    return s;
}

char*
randomize_modron_name(s)
char* s;
{
    static const char* start[] = { "alph", "bet", "gamm", "thet", "kapp", "zet", "iot", "delt", "et", "sigm", "omeg", "ypsil", "omicr" };
    static const char* v[] = { "a", "i", "o", "ati", "adri", "ahe" };
    static const char* end[] = { "gon", "dron", "tron" };

    if (s)
    {
        strcpy(s, "");
        Sprintf(s, "%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
            end[rn2(SIZE(end))]);
    }
    return s;
}

char*
randomize_gnoll_name(s)
char* s;
{
    static const char* start[] = { "bh", "gh", "gn", "ghn", "gr" "th", "dh", "dr", "dhr", "br", "bhr", "rr", "rh" };
    static const char* v[] = { "a", "e", "o", "u", "y"  };
    static const char* end[] = { "rk", "rp", "rl", "wl", "rnok", "rr", "rrg", "rror", "ryc", "ryn", "rryn", "rn", "gnol", "garr"  };

    if (s)
    {
        strcpy(s, "");
            Sprintf(s, "%s%s%s", start[rn2(SIZE(start))], v[rn2(SIZE(v))],
                end[rn2(SIZE(end))]);
    }
    return s;
}

char*
randomize_flind_name(s)
char* s;
{
    if (s)
    {
        strcpy(s, "");
        char ns[BUFSIZ];
        (void)randomize_gnoll_name(ns);
        Sprintf(s, "yee%s", ns);
    }
    return s;
}


char*
randomize_male_human_name(s)
char* s;
{
    static const char* names[] = { 
        "Ackley",
        "Acton",
        "Adger",
        "Aiken",
        "Alcott",
        "Alder",
        "Aldrich",
        "Alfred",
        "Allard",
        "Allston",
        "Alton",
        "Alvertos",
        "Alvin",
        "Arledge",
        "Arley",
        "Arlo",
        "Arundel",
        "Ashley",
        "Athelstan",
        "Averill",
        "Awarnach",
        "Ballard",
        "Bancroft",
        "Barclay",
        "Barnett",
        "Beacher",
        "Beardsley",
        "Bede",
        "Beldon",
        "Birch",
        "Blake",
        "Booth",
        "Borden",
        "Bradley",
        "Brandon",
        "Brent",
        "Brigham",
        "Brinley",
        "Brock",
        "Bromley",
        "Brook",
        "Buckley",
        "Burgess",
        "Burne",
        "Burt",
        "Buster",
        "Cade",
        "Calder",
        "Caldwell",
        "Calhoun",
        "Calvert",
        "Cameron",
        "Carden",
        "Carleton",
        "Carlyle",
        "Carter",
        "Carvell",
        "Catcher",
        "Cedric",
        "Chad",
        "Channing",
        "Charles",
        "Chevy",
        "Chilton",
        "Claiborne",
        "Clark",
        "Clifford",
        "Colbert",
        "Colborn",
        "Colter",
        "Courtland",
        "Crawford",
        "Creighton",
        "Cromwell",
        "Currier",
        "Dale",
        "Darren",
        "Darwin",
        "Dayton",
        "Dean",
        "Digby",
        "Donald",
        "Dorset",
        "Douglas",
        "Dover",
        "Doyle",
        "Dudley",
        "Duncan",
        "Durward",
        "Dustin",
        "Dwight",
        "Earl",
        "Edgar",
        "Edmund",
        "Edsel",
        "Edward",
        "Edwin",
        "Egerton",
        "Elder",
        "Eldon",
        "Eldridge",
        "Elmer",
        "Elton",
        "Emerson",
        "Erskine",
        "Esmond",
        "Fairfax",
        "Farley",
        "Farrell",
        "Fielding",
        "Ford",
        "Fuller",
        "Fulton",
        "Gilford",
        "Goldman",
        "Gordon",
        "Gower",
        "Gray",
        "Hadden",
        "Hadley",
        "Hagley",
        "Halbert",
        "Haley",
        "Hall",
        "Hallam",
        "Halsey",
        "Hamilton",
        "Hardy",
        "Harlan",
        "Harmon",
        "Harry",
        "Hawk",
        "Hawthorne",
        "Hayden",
        "Hayes",
        "Haywood",
        "Hedley",
        "Hendrick",
        "Henley",
        "Herbert",
        "Hilton",
        "Holden",
        "Hollis",
        "Horton",
        "Hudson",
        "Humphrey",
        "Hunt",
        "Hutton",
        "Hyatt",
        "Irving",
        "Ives",
        "Kenelm",
        "Kenley",
        "Kent",
        "Kenton",
        "Keyon",
        "Kinsey",
        "Landon",
        "Lang",
        "Lawson",
        "Layton",
        "Leland",
        "Lind",
        "Litton",
        "Llewellyn",
        "Lyndon",
        "Lyre",
        "Maitland",
        "Manley",
        "Manning",
        "Marden",
        "Marlow",
        "Marsden",
        "Mather",
        "Mead",
        "Millard",
        "Miller",
        "Milton",
        "Morven",
        "Nash",
        "Nedes",
        "Nelson",
        "Newell",
        "Newman",
        "North",
        "Nyle",
        "Oakley",
        "Ogden",
        "Olin",
        "Orman",
        "Osbert",
        "Osmond",
        "Oswald",
        "Oswin",
        "Palmer",
        "Parker",
        "Parr",
        "Payton",
        "Pell",
        "Penley",
        "Penn",
        "Pierson",
        "Preston",
        "Putnam",
        "Rabbit",
        "Radcliff",
        "Raleigh",
        "Ralph",
        "Ramsey",
        "Ransford",
        "Ransley",
        "Ransom",
        "Raven",
        "Ravinger",
        "Rawlins",
        "Rayburn",
        "Raymond",
        "Read",
        "Redford",
        "Reginald",
        "Remington",
        "Ridley",
        "Rishley",
        "Robert",
        "Rochester",
        "Rodman",
        "Rodney",
        "Rowan",
        "Rudyard",
        "Rutherford",
        "Rylan",
        "Sandon",
        "Sanford",
        "Scott",
        "Seabert",
        "Seward",
        "Shaw",
        "Shelley",
        "Shepherd",
        "Sherlock",
        "Sherman",
        "Sherwood",
        "Shipley",
        "Siddel",
        "Snowden",
        "Spencer",
        "Spike",
        "Stanley",
        "Stokley",
        "Stroud",
        "Studs",
        "Tanner",
        "Tate",
        "Terrel",
        "Thane",
        "Thatcher",
        "Thorne",
        "Thorpe",
        "Thurlow",
        "Tomkin",
        "Tostig",
        "Townsend",
        "Tranter",
        "Trent",
        "Tripp",
        "Truman",
        "Tye",
        "Tyne",
        "Wakefield",
        "Wallace",
        "Walton",
        "Waverly",
        "Wayland",
        "Wayne",
        "Wetherby",
        "Whitfield",
        "Wilfred",
        "Winifred",
        "Winslow",
        "Winston",
        "Winthrop",
        "Woodrow",
        "Wright",
        "Wyatt",
        "Wylie",
        "Wyndam",
        "York"
    };

    if (s)
    {
        strcpy(s, "");
        Sprintf(s, "%s", names[rn2(SIZE(names))]);
    }
    return s;
}


char*
randomize_female_human_name(s)
char* s;
{
    static const char* names[] = {
        "Abby",
        "Abigail",
        "Ada",
        "Addison",
        "Adelaide",
        "Adele",
        "Agatha",
        "Agnes",
        "Alaina",
        "Alanna",
        "Alberta",
        "Albina",
        "Alexandria",
        "Alice",
        "Alicia",
        "Alisha",
        "Alison",
        "Alma",
        "Alvina",
        "Amanda",
        "Amber",
        "Amelia",
        "Amy",
        "Ana",
        "Andrea",
        "Andy",
        "Angel",
        "Angela",
        "Angie",
        "Anna",
        "Annabelle",
        "Annabeth",
        "Anne",
        "Annie",
        "Antonia",
        "April",
        "Arabella",
        "Arda",
        "Ashley",
        "Astrid",
        "Aubrey",
        "Audrey",
        "Autumn",
        "Averil",
        "Avis",
        "Aviva",
        "Barbara",
        "Beatrice",
        "Becki",
        "Belinda",
        "Bella",
        "Berenice",
        "Bertha",
        "Blanche",
        "Brenda",
        "Bridget",
        "Bronwen",
        "Bronwyn",
        "Bryony",
        "Carlene",
        "Carlie",
        "Carmelita",
        "Carol",
        "Carol Ann",
        "Carol Anne",
        "Carole",
        "Caroline",
        "Carolyn",
        "Cassandra",
        "Cathleen",
        "Cathy",
        "Cecilia",
        "Cecily",
        "Celestia",
        "Celia",
        "Celinda",
        "Charisse",
        "Charity",
        "Charla",
        "Charlene",
        "Charlotte",
        "Charlyne",
        "Charmaine",
        "Chelsea",
        "Cheryl",
        "Chloe",
        "Christabel",
        "Christina",
        "Christine",
        "Cindy",
        "Claire",
        "Clara",
        "Clare",
        "Claribel",
        "Clarissa",
        "Claudia",
        "Clementine",
        "Colette",
        "Colleen",
        "Cordelia",
        "Crystal",
        "Cynthia",
        "Danielle",
        "Daphne",
        "Darlene",
        "Davina",
        "Dawn",
        "Deanna",
        "Deanne",
        "Deborah",
        "Delia",
        "Denise",
        "Devon",
        "Dora",
        "Doreen",
        "Dorothy",
        "Drew",
        "Drusilla",
        "Dulcie",
        "Edith",
        "Edna",
        "Edwina",
        "Eleanor",
        "Elizabeth",
        "Ella",
        "Ellen",
        "Ellie",
        "Emily",
        "Emma",
        "Enid",
        "Erika",
        "Erin",
        "Estelle",
        "Esther",
        "Esty",
        "Ethel",
        "Ethelreda",
        "Eudora",
        "Eva",
        "Eve",
        "Evelyn",
        "Faith",
        "Felicity",
        "Fleur",
        "Flora",
        "Florence",
        "Francie",
        "Frida",
        "Gail",
        "Gemma",
        "Genevieve",
        "Georgia",
        "Georgiana",
        "Gertie",
        "Gertrude",
        "Gia",
        "Giselle",
        "Glenda",
        "Grace",
        "Gwenda",
        "Gwendolen",
        "Gwendoline",
        "Gwendolyn",
        "Gwyneth",
        "Hannah",
        "Harriet",
        "Heather",
        "Heidi",
        "Helen",
        "Helena",
        "Helene",
        "Henrietta",
        "Hester",
        "Hilary",
        "Hilda",
        "Hodierna",
        "Holly",
        "Honor",
        "Hope",
        "Ida",
        "Imelda",
        "Imogen",
        "Iona",
        "Irene",
        "Iris",
        "Isabella",
        "Isla",
        "Ivy",
        "Jacqueline",
        "Jasmine",
        "Jennifer",
        "Joan",
        "Joanna",
        "Joanne",
        "Joelle",
        "Josephine",
        "Judith",
        "Julia",
        "Julianne",
        "Julie",
        "Karen",
        "Karina",
        "Karlene",
        "Karolyn",
        "Kathleen",
        "Kaylee",
        "Kierra",
        "Lana",
        "Lanna",
        "Lara",
        "Laura",
        "Lauren",
        "Lauretta",
        "Leah",
        "Leanne",
        "Leila",
        "Leisha",
        "Lena",
        "Lenna",
        "Leonora",
        "Leslie",
        "Lettice",
        "Liana",
        "Lila",
        "Lilla",
        "Lillian",
        "Lily",
        "Linda",
        "Lindsay",
        "Lisa",
        "Liza",
        "Lois",
        "Lorelei",
        "Loretta",
        "Lorinda",
        "Lorna",
        "Lorraine",
        "Louella",
        "Louisa",
        "Louise",
        "Lucia",
        "Lucinda",
        "Lucy",
        "Lynnette",
        "Lysette",
        "Mabel",
        "Madelaine",
        "Madge",
        "Maggie",
        "Mandy",
        "Marcia",
        "Marcie",
        "Margaret",
        "Mariah",
        "Marian",
        "Marianne",
        "Marie",
        "Marilyn",
        "Marina",
        "Marissa",
        "Marjorie",
        "Marsha",
        "Marta",
        "Mary",
        "Matilda",
        "Maud",
        "Maude",
        "Maureen",
        "Mavis",
        "May",
        "Maya",
        "Mayola",
        "Medea",
        "Megan",
        "Melissa",
        "Merle",
        "Michelle",
        "Mildred",
        "Millicent",
        "Minna",
        "Minnie",
        "Miranda",
        "Moira",
        "Morgan",
        "Myra",
        "Myrna",
        "Myrtle",
        "Nadine",
        "Naila",
        "Nancy",
        "Narcissa",
        "Natalie",
        "Nena",
        "Nettie",
        "Nia",
        "Nicola",
        "Nicole",
        "Nina",
        "Nora",
        "Odette",
        "Olivia",
        "Opal",
        "Patrice",
        "Paula",
        "Paulina",
        "Pearl",
        "Penelope",
        "Persis",
        "Philippa",
        "Priscilla",
        "Rachel",
        "Reba",
        "Rhiannon",
        "Rhoda",
        "Rhonda",
        "Richeldis",
        "Rita",
        "Roberta",
        "Rosamund",
        "Rose",
        "Rosemary",
        "Ruth",
        "Sabrina",
        "Salma",
        "Samantha",
        "Sandra",
        "Sarah",
        "Serena",
        "Shania",
        "Shannon",
        "Sharla",
        "Sharleen",
        "Sharlene",
        "Sharon",
        "Shawna",
        "Sheryl",
        "Sibyl",
        "Simone",
        "Sophia",
        "Sophie",
        "Stella",
        "Susan",
        "Susanna",
        "Susanne",
        "Suzanne",
        "Sylvia",
        "Talitha",
        "Tara",
        "Teresa",
        "Thelma",
        "Thomasina",
        "Thurza",
        "Tiffany",
        "Tina",
        "Tracy",
        "Trisha",
        "Tyra",
        "Urith",
        "Valerie",
        "Vanessa",
        "Venetia",
        "Vera",
        "Victoria",
        "Vilma",
        "Viola",
        "Virginia",
        "Wanda",
        "Wendy",
        "Whitney",
        "Wilma",
        "Winifred",
        "Winnie",
        "Winnifred",
        "Yasmin",
        "Yvette",
        "Yvonne"
    };


    if (s)
    {
        strcpy(s, "");
        Sprintf(s, "%s", names[rn2(SIZE(names))]);
    }
    return s;
}

char*
randomize_demon_name(s)
char* s;
{
    static const char* names[] = {
        "Baalzephon",
        "Dagos",
        "Furcas",
        "Pearza",
        "Zaebos",
        "Zapan",
        "Zimimar",
        "Koraboros",
        "Abarax",
        "Tanetal",
        "Abbatorru",
        "Adacher",
        "Adaddon",
        "Adadese",
        "Akinurb",
        "Anurnac",
        "Anur-Shub",
        "Arbelah",
        "Babzidu",
        "Bal-Balah",
        "Barbagg",
        "Bis",
        "Bisrib",
        "Chuh-Arbalma",
        "Damasze",
        "Dimgidu",
        "Dingidnin",
        "Dinimabi",
        "Dukiana",
        "Dur",
        "Ehur-Shu-Dur",
        "Ehurtar",
        "Elamash",
        "Enian",
        "Enlilil",
        "Esagala",
        "Esarhal",
        "Esarruk",
        "Gaszarsir",
        "Gismudurug",
        "Gissz",
        "Giszagnabi",
        "Haburta",
        "Harch-sschwat",
        "Harrapi",
        "Hisszagu",
        "Hursag",
        "Huszi",
        "Ibza",
        "Iduduamnaa",
        "Igaam",
        "Igalil",
        "Ilile",
        "Imhiakaam",
        "Jebel",
        "Jebelat",
        "Jezirpa",
        "Karduki",
        "Kikabi",
        "Kilzecru",
        "Klo-Thort",
        "Kulzu",
        "Kuninin",
        "Maam",
        "Maradaraam",
        "Matukal",
        "Meteneg",
        "Mitanu",
        "Muibzur",
        "Muigude",
        "Muisze",
        "Muninaam",
        "Mununaa",
        "Nab-kin",
        "Naszsir",
        "Nimrisr",
        "Nin",
        "Nutae",
        "Pisipad",
        "Pismurda",
        "Riasir",
        "Sag",
        "Sagrata",
        "Shaddon",
        "Shalbal",
        "Shalih",
        "Sharrar",
        "Sharruk",
        "Sigsir",
        "Sineser",
        "Szekaabsuk",
        "Szerinsuk",
        "Szumuzabi",
        "Szusumu",
        "T'aah'ukin",
        "Tho'Daah-Nugh",
        "Tigla",
        "Tubidu",
        "Tumunnu",
        "Turtush",
        "Tur-uk",
        "Ubienlnaa",
        "Udnaa",
        "Urah",
        "Zaburra",
        "Zaganit",
        "Zalidamsir",
        "Zamanni",
        "Zananaa"
    };

    if (s)
    {
        strcpy(s, "");
        Sprintf(s, "%s", names[rn2(SIZE(names))]);
    }
    return s;
}

char*
randomize_undead_spellcaster_name(s)
char* s;
{
    static const char* names[] = {
        "Arklem Greeth",
        "Asberdies",
        "Aumvor",
        "Azalin",
        "Boretti",
        "Zrie Prakis",
        "Erandis Vol",
        "Tordynnar Rhaevaern",
        "Sammaster",
        "Redeye",
        "Thessalar",
        "Szass Tam",
        "Skall",
        "Larloch",
        "Harthoon",
        "Dregoth",
        "Dragotha"
    };

    if (s)
    {
        strcpy(s, "");
        Sprintf(s, "%s", names[rn2(SIZE(names))]);
    }
    return s;
}

char*
randomize_angel_name(s)
char* s;
{
    static const char* names[] = {
        "Aglibol",
        "Ananiel",
        "Arakiel",
        "Arariel",
        "Armaros",
        "Artiya'il",
        "Asbeel",
        "Azazel",
        "Azrael",
        "Barachiel",
        "Baraqiel",
        "Batariel",
        "Bezaliel",
        "Camael",
        "Cassiel",
        "Chazaqiel",
        "Daniel",
        "Dadrail",
        "Dumah",
        "Eleleth",
        "Gabriel",
        "Gadreel",
        "Hadraniel",
        "Hahasiah",
        "Hanibal",
        "Haniel",
        "Harut",
        "Hashmal",
        "Hofniel",
        "Jegudiel",
        "Jehoel",
        "Jequn",
        "Jerahmeel",
        "Jophiel",
        "Kalka'il",
        "Kepharel",
        "Kerubiel",
        "Kokabiel",
        "Kushiel",
        "Lailah",
        "Mebahiah",
        "Munkar",
        "Muriel",
        "Nakir",
        "Nanael",
        "Nathaniel",
        "Netzach",
        "Nithael",
        "Nuriel",
        "Ophaniel",
        "Pahaliah",
        "Penemue",
        "Phanuel",
        "Poyel",
        "Pravuil",
        "Puriel",
        "Radueriel",
        "Raguel",
        "Ramiel",
        "Raphael",
        "Raziel",
        "Rikbiel",
        "Sabriel",
        "Sachiel",
        "Sahaquiel",
        "Samael",
        "Samyaza",
        "Sandalphon",
        "Sarathiel",
        "Sariel",
        "Schemhampharae",
        "Selaphiel",
        "Seraphiel",
        "Shamnail",
        "Shamsiel",
        "Sidriel",
        "Tamiel",
        "Temeluchus",
        "Tennin",
        "Turail",
        "Turiel",
        "Uriel",
        "Uziel",
        "Vasiariah",
        "Vehuel",
        "Yarhibol",
        "Yomiel",
        "Zachariel",
        "Zadkiel",
        "Zaphkiel",
        "Zaqiel",
        "Zephaniel",
        "Zephon",
        "Araciel",
        "Arakiba",
        "Araqael",
        "Araqiel",
        "Aretstikapha",
        "Arkas",
        "Arkiel",
        "Arqael",
        "Sarquael",
        "Armoniel",
        "Cafziel",
        "Cafzyel",
        "Caphziel",
        "Casiel",
        "Cassael",
        "Castiel",
        "Casziel",
        "Kafziel",
        "Kasiel",
        "Qafsiel",
        "Qaphsiel",
        "Qaspiel",
        "Qephetzial",
        "Quaphsiel",
        "Cameel",
        "Camiel",
        "Camniel",
        "Kamael",
        "Kemuel",
        "Khamael",
        "Iophiel",
        "Iofiel",
        "Jofiel",
        "Yofiel",
        "Youfiel",
        "Zophiel",
        "Zuriel",
        "Akrasiel",
        "Raguil",
        "Rakul",
        "Raquel",
        "Rasuil",
        "Reuel",
        "Rufael",
        "Sarakiel",
        "Saraqael",
        "Sauriel",
        "Seriel",
        "Sourial",
        "Suriel",
        "Suriyel",
        "Suruel",
        "Surufel",
        "Tzaphkiel",
        "Tzaphqiel",
        "Zafchial",
        "Zafkiel",
        "Zaphchial",
        "Zaphiel",
        "Zelel",
        "Hesediel",
        "Tzadkiel",
        "Zadakiel",
        "Zedekiel",
        "Zedekul"
    };

    if (s)
    {
        strcpy(s, "");
        Sprintf(s, "%s", names[rn2(SIZE(names))]);
    }
    return s;
}


struct monst *
christen_orc(mtmp, gang, other)
struct monst *mtmp;
const char *gang, *other;
{
    int sz = 0;
    char buf[BUFSZ], buf2[BUFSZ], *orcname;

    orcname = rndorcname(buf2);
    sz = (int) strlen(orcname);
    if (gang)
        sz += (int) (strlen(gang) + sizeof " of " - sizeof "");
    else if (other)
        sz += (int) strlen(other);

    if (sz < BUFSZ) {
        char gbuf[BUFSZ];
        boolean nameit = FALSE;

        if (gang && orcname) {
            Sprintf(buf, "%s of the %s clan", upstart(orcname),
                    upstart(strcpy(gbuf, gang)));
            nameit = TRUE;
        } else if (other && orcname) {
            Sprintf(buf, "%s%s", upstart(orcname), other);
            nameit = TRUE;
        }
        if (nameit)
            mtmp = christen_monst(mtmp, buf);
    }
    return mtmp;
}

/* make sure "The Colour of Magic" remains the first entry in here */
STATIC_VAR const char *const sir_Terry_novels[] = {
    "The Colour of Magic", "The Light Fantastic", "Equal Rites", "Mort",
    "Sourcery", "Wyrd Sisters", "Pyramids", "Guards! Guards!", "Eric",
    "Moving Pictures", "Reaper Man", "Witches Abroad", "Small Gods",
    "Lords and Ladies", "Men at Arms", "Soul Music", "Interesting Times",
    "Maskerade", "Feet of Clay", "Hogfather", "Jingo", "The Last Continent",
    "Carpe Jugulum", "The Fifth Elephant", "The Truth", "Thief of Time",
    "The Last Hero", "The Amazing Maurice and His Educated Rodents",
    "Night Watch", "The Wee Free Men", "Monstrous Regiment",
    "A Hat Full of Sky", "Going Postal", "Thud!", "Wintersmith",
    "Making Money", "Unseen Academicals", "I Shall Wear Midnight", "Snuff",
    "Raising Steam", "The Shepherd's Crown"
};

STATIC_OVL
const char*
gettitle(titleidx, titlearray, arraysize, numrandomized, excludedtitles, excludedtitles2)
short* titleidx;
const char* const* titlearray;
int arraysize;
int numrandomized; /* Only this first elements are included in randomization */
unsigned long excludedtitles, excludedtitles2; /* Requires a 64-bit long to work for more than 32 novels */
{
    short num = (short)min(arraysize, numrandomized); /* num is the titles randomized before exclusions */
    short j, k = num; /* k is the titles randomized after exclusions */
    if (excludedtitles)
    {
        int i;
        for (i = 0; i < 32 && i < num; i++)
        {
            unsigned long bit = 1UL << i;
            if (excludedtitles & bit)
            {
                k--;
            }
        }
    }
    if (excludedtitles2)
    {
        int i;
        for (i = 32; i < 64 && i < num; i++)
        {
            unsigned long bit = 1UL << (i - 32);
            if (excludedtitles2 & bit)
            {
                k--;
            }
        }
    }

    if (k < 1)
        j = 0;
    else
    {
        short roll = (short)rn2((int)k);
        if (excludedtitles || excludedtitles2)
        {
            for (j = 0; j < num; j++)
            {
                unsigned long bit = 0UL;
                if (j < 32)
                {
                    bit = 1UL << j;
                    if (excludedtitles & bit)
                        continue;
                }
                else if (j < 64)
                {
                    bit = 1UL << (j - 32);
                    if (excludedtitles2 & bit)
                        continue;
                }
                if (!roll)
                    break;
                roll--;
            }
        }
        else
            j = roll;
    }

    if (titleidx)  /* Randomized or non-randomized if titleidx != 0 */
    {
        if (*titleidx == -1)
            *titleidx = j; /* Randomized, set titleidx to the randomized index */
        else if (*titleidx >= 0 && *titleidx < arraysize)
            j = *titleidx;  /* Set to value determined by titleidx */
    }
    return titlearray[j];
}

const char *
noveltitle(novidx, excludedtitles, excludedtitles2)
short* novidx;
unsigned long excludedtitles, excludedtitles2;
{
    return gettitle(novidx, sir_Terry_novels, SIZE(sir_Terry_novels), SIZE(sir_Terry_novels), excludedtitles, excludedtitles2);
}


STATIC_VAR const char* const manual_names[MAX_MANUAL_TYPES] = {
    /* Manuals */
    "Wands 101", "Armor 101", "Weapons 101", "Gray Stones 101",
    "Basics of Kicking", "Basics of Enchantment", "Basics of Eating and Drinking", "Introduction to Dangerous Monsters",
    "Introduction to Blessed and Cursed Items", "Guide to Praying", "Essential Survival Tips",
    "Assorted Tips and Tricks", "Elementary Mechanics", "Principles of Magic", "Understanding Pets and Hirelings",
    "Item Identification 101", "Item Identification 102", "Guide to Essential Resistances, Vol. I",
    "Guide to Essential Resistances, Vol. II",
    /* Never randomly generated */
    "Guide to Dragon Scale Mails", "Guide to Altars and Sacrifice",
    "Secrets of Scare Monster", "Gurathul's Guide to Ascension", "Master Class in Wands", "Infernal Inhabitants of Gehennom",
    "Advanced Reading in Known Monsters", "Manual of the Planes",
    /* Catalogues */
    "Catalogue of Weapons", "Catalogue of Armor", "Catalogue of Rings", "Catalogue of Potions", "Catalogue of Scrolls", "Catalogue of Wands",
    "Catalogue of Miscellaneous Magic Items", "Catalogue of Tools", "Catalogue of Magic Spells", "Catalogue of Clerical Spells",
    "Catalogue of Comestibles", "Catalogue of Gems and Stones", "Catalogue of Artifacts",
};

const char*
manualtitle(mnlidx, excludedtitles, excludedtitles2)
short* mnlidx;
unsigned long excludedtitles, excludedtitles2;
{
    return gettitle(mnlidx, manual_names, SIZE(manual_names), NUM_RANDOM_MANUALS, excludedtitles, excludedtitles2);
}

/* qsort comparison routine */
STATIC_OVL int
citemsortcmp(p, q)
const void* p;
const void* q;
{
    short sp = *(short*)p;
    short sq = *(short*)q;
    int oclsp = objects[sp].oc_class;
    int oclsq = objects[sq].oc_class;
    int ocldiff = oclsp - oclsq;
    int skillp = objects[sp].oc_class == ARMOR_CLASS ? 0 : objects[sp].oc_skill;
    int skillq = objects[sq].oc_class == ARMOR_CLASS ? 0 : objects[sq].oc_skill;
    int skilldiff = skillp - skillq;
    int subtypp = objects[sp].oc_subtyp;
    int subtypq = objects[sq].oc_subtyp;
    int subtypdiff = subtypp - subtypq;
    const char* namep = OBJ_NAME(objects[sp]);
    const char* nameq = OBJ_NAME(objects[sq]);
    int namediff = namep && nameq ? strcmpi(namep, nameq) : namep ? 1 : nameq ? -1 : 0;

    return ocldiff ? ocldiff : skilldiff ? skilldiff : subtypdiff ? subtypdiff : namediff;
}

/* qsort comparison routine */
STATIC_OVL int
artilistsortcmp(p, q)
const void* p;
const void* q;
{
    short sp = *(short*)p;
    short sq = *(short*)q;
    const char* namep = artilist[sp].name;
    const char* nameq = artilist[sq].name;
    if (namep && !strncmpi(namep, "The ", 4))
        namep += 4;
    if (nameq && !strncmpi(nameq, "The ", 4))
        nameq += 4;
    int namediff = namep && nameq ? strcmpi(namep, nameq) : namep ? 1 : nameq ? -1 : 0;

    return namediff;
}

STATIC_VAR short sorted_citems[NUM_OBJECTS];

#define CATALOGUE_MAGICAL 1
#define CATALOGUE_CLERICAL 2

STATIC_OVL void
print_catalogue(datawin, obj, objectclass, cflags)
winid datawin;
struct obj* obj;
int objectclass;
unsigned long cflags;
{
    short i, cnt = 0;
    char buf[BUFSZ];
    char objbuf[BUFSZ];
    const char magicschools[8] = {
        P_ARCANE_SPELL,
        P_MOVEMENT_SPELL,
        P_TRANSMUTATION_SPELL,
        P_ENCHANTMENT_SPELL,
        P_CONJURATION_SPELL,
        0
    };

    for (i = bases[objectclass]; i < NUM_OBJECTS; i++) 
    {
        if (objects[i].oc_class != objectclass)
            break;

        if (!OBJ_NAME(objects[i]) || !*OBJ_NAME(objects[i]))
            continue;

        if(((objects[i].oc_flags3 & (O3_NO_WISH | O3_NO_GENERATION)) || (objects[i].oc_flags5 & O5_NO_CATALOGUE)))
            continue;
        
        if (objectclass == SPBOOK_CLASS && objects[i].oc_skill == P_NONE)
            continue;

        if ((cflags & CATALOGUE_MAGICAL) && index(magicschools, objects[i].oc_skill) == 0)
            continue;

        if ((cflags & CATALOGUE_CLERICAL) && index(magicschools, objects[i].oc_skill))
            continue;

        if (obj && obj->cursed && (
            (i % 3) == (short)(obj->o_id % 3)
            || (((unsigned int)i + obj->o_id) % 7) == 0
            || (((unsigned int)i + obj->o_id + 1) % 11) == 0
            ))
            continue;

        sorted_citems[cnt] = i;
        cnt++;

    }

    qsort((genericptr_t)sorted_citems, cnt, sizeof(short), citemsortcmp);

    int subtyp = -1;
    int skill = -1;
    int magiccnt = 0;
    boolean checkmagic = FALSE;
    if (objectclass == WEAPON_CLASS || objectclass == ARMOR_CLASS)
        checkmagic = TRUE;

    for (i = 0; i < cnt; i++) 
    {
        if ((objectclass == WEAPON_CLASS || objectclass == SPBOOK_CLASS) && objects[sorted_citems[i]].oc_skill != skill && objects[sorted_citems[i]].oc_skill != P_NONE)
        {
            strcpy_capitalized_for_title(objbuf, get_skill_plural_name(objects[sorted_citems[i]].oc_skill));
            Sprintf(buf, "%s%s", abs(objects[sorted_citems[i]].oc_skill) != P_THROWN_WEAPON && objects[sorted_citems[i]].oc_skill < 0 ? "Projectiles for " : "", objbuf);
            putstr(datawin, ATR_HEADING, buf);
        }
        else if (objectclass == ARMOR_CLASS && objects[sorted_citems[i]].oc_subtyp != subtyp)
        {
            strcpy_capitalized_for_title(objbuf, makeplural(armor_type_names[objects[sorted_citems[i]].oc_subtyp]));
            Sprintf(buf, "%s", objbuf);
            putstr(datawin, ATR_HEADING, buf);
        }
        else if (objectclass == FOOD_CLASS && objects[sorted_citems[i]].oc_subtyp != subtyp)
        {
            strcpy_capitalized_for_title(objbuf, makeplural(food_type_names[objects[sorted_citems[i]].oc_subtyp]));
            Sprintf(buf, "%s", objbuf);
            putstr(datawin, ATR_HEADING, buf);
        }
        else if (objectclass == TOOL_CLASS && objects[sorted_citems[i]].oc_subtyp != subtyp)
        {
            strcpy_capitalized_for_title(objbuf, makeplural(tool_type_names[objects[sorted_citems[i]].oc_subtyp]));
            Sprintf(buf, "%s", objbuf);
            putstr(datawin, ATR_HEADING, buf);
        }
        else if (objectclass == MISCELLANEOUS_CLASS && objects[sorted_citems[i]].oc_subtyp != subtyp)
        {
            strcpy_capitalized_for_title(objbuf, makeplural(misc_type_names[objects[sorted_citems[i]].oc_subtyp]));
            Sprintf(buf, "%s", objbuf);
            putstr(datawin, ATR_HEADING, buf);
        }
        else if (objectclass == GEM_CLASS && objects[sorted_citems[i]].oc_skill != skill && objects[sorted_citems[i]].oc_skill == P_NONE)
        {
            strcpy_capitalized_for_title(buf, "Non-Projectile Rocks");
            putstr(datawin, ATR_HEADING, buf);
        }
        skill = objects[sorted_citems[i]].oc_skill;
        subtyp = objects[sorted_citems[i]].oc_subtyp;
        magiccnt += (int)objects[sorted_citems[i]].oc_magic;
        Sprintf(objbuf, "%s", OBJ_NAME(objects[sorted_citems[i]]));
        *objbuf = highc(*objbuf);
        Sprintf(buf, "%4d - %s%s", i + 1, objbuf, checkmagic && objects[sorted_citems[i]].oc_magic ? "*" : "");
        putstr(datawin, ATR_INDENT_AT_DASH, buf);
    }
    if (checkmagic && magiccnt > 0)
    {
        putstr(datawin, 0, "");
        putstr(datawin, ATR_INDENT_AT_DASH, "* Magical item");
    }
}

STATIC_OVL void
print_artifact_catalogue(datawin, obj)
winid datawin;
struct obj* obj;
{
    short i, cnt = 0;
    char buf[BUFSZ];
    char objbuf[BUFSZ];

    for (i = 1; i < NUM_ARTIFACTS; i++)
    {
        if (artilist[i].aflags2 & AF2_NO_CATALOGUE)
            continue;

        if (obj && obj->cursed && (
            (i % 3) == (short)(obj->o_id % 3)
            || (((unsigned int)i + obj->o_id) % 7) == 0
            || (((unsigned int)i + obj->o_id + 1) % 11) == 0
            ))
            continue;

        sorted_citems[cnt] = i;
        cnt++;

    }

    qsort((genericptr_t)sorted_citems, cnt, sizeof(short), artilistsortcmp);

    int nowishcnt = 0;
    for (i = 0; i < cnt; i++)
    {
        const char* aname = artilist[sorted_citems[i]].name;
        if (aname && !strncmpi(aname, "The ", 4))
            aname += 4;
        Strcpy(objbuf, aname);
        if (artilist[sorted_citems[i]].aflags & AF_NO_WISH)
            nowishcnt++;
        Sprintf(buf, "%3d - %s%s", i + 1, objbuf, (artilist[sorted_citems[i]].aflags & AF_NO_WISH) ? "*" : "");
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }
    if (nowishcnt > 0)
    {
        putstr(datawin, 0, "");
        putstr(datawin, ATR_INDENT_AT_DASH, "* Not wishable");
    }
}

void
read_manual(obj)
struct obj* obj;
{
    if (!obj)
        return;

    short mnlidx = obj->manualidx;

    if (mnlidx < 0)
    {
        pline("%s unintelligibly scribbled.", Tobjnam(obj, "be"));
        return;
    }

    if (!u.uconduct.literate++)
        livelog_printf(LL_CONDUCT | LL_ARTIFACT, "became literate by reading %s",
            acxname(obj));

    winid datawin;
    char buf[BUFSZ];

    if (mnlidx >= FIRST_CATALOGUE)
    {
        datawin = create_nhwindow_ex(NHW_TEXT, GHWINDOW_STYLE_GENERAL, NO_GLYPH, zerocreatewindowinfo);
        if (mnlidx == MANUAL_CATALOGUE_OF_ARTIFACTS)
        {
            putstr(datawin, ATR_HEADING, "The manual contains a list of artifacts found in Yendor:");
            putstr(datawin, 0, "");
            print_artifact_catalogue(datawin, obj);

        }
        else
        {
            int itemclass = ILLOBJ_CLASS;
            unsigned long cflags = 0UL;
            switch (mnlidx)
            {
            case MANUAL_CATALOGUE_OF_WEAPONS:
                itemclass = WEAPON_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_ARMOR:
                itemclass = ARMOR_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_RINGS:
                itemclass = RING_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_POTIONS:
                itemclass = POTION_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_WANDS:
                itemclass = WAND_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_SCROLLS:
                itemclass = SCROLL_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_MISCELLANEOUS_MAGIC_ITEMS:
                itemclass = MISCELLANEOUS_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_TOOLS:
                itemclass = TOOL_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_MAGIC_SPELLS:
                itemclass = SPBOOK_CLASS;
                cflags = 1;
                break;
            case MANUAL_CATALOGUE_OF_CLERICAL_SPELLS:
                itemclass = SPBOOK_CLASS;
                cflags = 2;
                break;
            case MANUAL_CATALOGUE_OF_COMESTIBLES:
                itemclass = FOOD_CLASS;
                break;
            case MANUAL_CATALOGUE_OF_GEMS_AND_STONES:
                itemclass = GEM_CLASS;
                break;
            default:
                break;
            }
            char tbuf[BUFSZ];
            Sprintf(tbuf, "The manual contains a list of %s found in Yendor:", 
                itemclass == GEM_CLASS ? "gems and rocks" :
                itemclass == SPBOOK_CLASS ? (cflags & 1 ? "wizard spells" : cflags & 2 ? "clerical spells" : "spells") : def_oc_syms[itemclass].name);
            putstr(datawin, ATR_HEADING, tbuf);
            putstr(datawin, 0, "");
            print_catalogue(datawin, obj, itemclass, cflags);
        }        
    }
    else
    {
        datawin = create_nhwindow_ex(NHW_TEXT, GHWINDOW_STYLE_PAGER_KEEP_LINE_BREAKS, NO_GLYPH, zerocreatewindowinfo);
        putstr(datawin, 0, "The manual contains several instructions:");
        putstr(datawin, 0, "");
        switch (mnlidx)
        {
        case MANUAL_WANDS_101:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can zap wands at monsters and items.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. You can zap wands even at yourself, as well as upwards and downwards.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You can also apply wands on an item. It applies the wand's effect on to the item.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. For example, you can use a wand of cancellation to remove a curse from an item.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. If you put a wand of cancellation or a Rod of Disjunction in a bag of holding, it will explode.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. It is advisable to put wands of cancellation into ordinary bags to avoid putting them accidently into a magic bag.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. You can zap a wand of digging downwards to create a hole to flee from a dangerous situation.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. If you are engulfed by a monster, you can zap a wand of digging at it. It will spit you out and it is reduced to 1 HP.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. A wand of create monster is useful for generating monsters for sacrificing at an altar.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. Wands make a good offensive ranged weapon choice for classes that can use them well.");
            break;
        case MANUAL_ARMOR_101:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Armor gives Armor Class (AC), which reduces the chance of a monster in hitting you. The lower the AC the better the armor.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Total AC below 0 also reduces damage.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Armor also gives Magic Cancellation (MC), which reduces the success chance of touch-based special attacks. The higher the MC the better the protection.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. Robe is a special armor slot that does not stack with body armor. The game uses the superior AC and MC of the body armor and the robe.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. Armor may also give you spell casting penalty, which reduces the chance of casting somatic spells. It does not affect non-somatic spells.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. A crystal plate mail gives you reflection.");
            break;
        case MANUAL_WEAPONS_101:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can ready an alternate weapon by swapping weapons and then readying the alternate weapon.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Switching between weapons by swapping two readied sets of weapons does not cost you a turn.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You can also fight with two weapons at once, but this comes a penalty to hit and damage, which depends on your skill level.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. You gain weapon skill points by hitting with the weapon successfully.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. You gain two-weapon-fighting skill points when you it hit successfully with your off-hand weapon.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Ranged weapons incur a -30 to hit penalty when thrown or fired in melee combat.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Weapons may be enchanted to be flaming, freezing, and electrified, which all cost extra damage. The enchantment may dissipate upon hit.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Glass and bone weapons can be enchanted to be death-magical, which slays the target instantly upon hit. The enchantment is consequently dispelled.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Weapons may have exceptional, elite, and celestial/primordial/infernal qualities.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. Exceptional weapons do double base damage, elite weapons triple base damage, and celestial/primordial/infernal weapons quadruple base damage.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "11. Celestial weapons can only be used by lawful characters, primordial weapons only by neutral characters, and infernal weapons only by chaotic characters.");
            break;
        case MANUAL_GRAY_STONES_101:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can identify any gem or stone by applying an uncursed or blessed touchstone (one of the gray stones) unto it.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Beware of the cursed loadstone (one of the gray stones). It is cursed and you cannot drop it once you have picked it up. And it's very heavy.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You can identify a loadstone by kicking it while on the floor. If it does not move, it is a loadstone.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. If you have unidentified gray stones in a container on the floor, you may pick it up and tip it so that its contents drop on the ground.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. This way you do not need to put its contents - and a possible loadstone - into your inventory.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Beware of the cursed jinxstone. It will cause mishaps to happen and once you have picked it up, you cannot drop it.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Jinxstones are best identified by letting your pet stand upon it. If the pet does not step upon the stone, it is cursed.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. A luckstone, one of the gray stones, increase your luck if kept in the open inventory.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Luck will disappear over time, but a blessed luckstone will prevent this.");
            break;
        case MANUAL_BASICS_OF_KICKING:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can kick closed doors open.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Do not kick shop doors, or the shopkeeper will get angry.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You can break the locks of locked chests by kicking them. This may destroy glass items in the chest, though.");
            break;
        case MANUAL_BASICS_OF_ENCHANTMENT:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can enchant weapons and armor to make them better.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. If you enchant an armor or a weapon too much, it may explode.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You may examine a fully identified item to determine its maximum safe enchantable level.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. You can magically protect weapons and armor to prevent them from corroding.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. Mythic weapons and armor have one special power. The special powers are named as a prefix and suffix to the name of the weapon or armor.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Legendary weapons and armor have two special powers, one prefix ability and one suffix ability.");
            break;
        case MANUAL_BASICS_OF_EATING_AND_DRINKING:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Do not eat too much, or you may choke to death.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Corpses rot very fast. Do not eat rotten corpses or you will be food poisoned and die.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Gnolls can smell which corpses are safe to eat and which are not.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. Some corpses may give you intrinsic abilities when eaten, or increase your ability scores.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. You should eat corpses that are safe to eat and which give good abilities. Do not eat dangerous corpses.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Do not eat corpses left by undead creatures, except wraith corpses.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Undead corpses are rotten, except for wraith corpses, which will give you an experience level.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Some fruits give you ability score bonuses, when eaten.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. You can probe monsters and corpses with a wand of probing to determine what abilities they have and if the corpses are safe to eat.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. Rings and amulets consume nutrition periodically when worn. If you are low on food, it may be wise not to wear them.");
            break;
        case MANUAL_INTRODUCTION_TO_DANGEROUS_MONSTERS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Hitting a floating eye in melee will paralyse you. Do not do it.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Watch out for nymphs and harpies. They can steal your items.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Leprechauns will steal your money that is in open inventory.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. There are many dangerous monsters that should be killed from far away.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. For example, werecreatures may bite you and cause you to contract lycanthropy.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Mummies cause mummy rot on touch. It prevents you from healing over time and reduces your constitution and charisma slowly.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Some snakes can constrict you. It is often a good idea to kill them from a distance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Some monsters, such as owlbears, can grab you. You need to kill the grabbed monster first before doing anything else.");
            break;
        case MANUAL_ESSENTIAL_SURVIVAL_TIPS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can cure a disease, such as mummy rot or food poisoning, by eating a fig.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Also, a priest can cure your disease for a fee. Furthermore, praying can cure a disease as well.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You can cure lycanthropy by drinking a potion of holy water or by eating a sprig of wolfsbane.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. Blessed potions of healing increase your maximum hit points if you drink them at full health.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. It is a good practice to drink many blessed potions of healing early on to maximise your hit points.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. You cannot swim. Do not try to enter deep water, such as moat, or you may drown.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Monsters cannot engulf and digest you, if you are wearing a ring of slow digestion.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Tentacled ones cannot eat your brain, if you are wearing a ring of sustain ability. They also cannot cause amnesia.");
            break;
        case MANUAL_INTRODUCTION_TO_BLESSED_AND_CURSED_ITEMS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Items can be blessed, uncursed, or cursed. You cannot remove cursed items if you equip them.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. You can remove a curse from an item if you bless them item using a potion of holy water or by paying to a priest to do so.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You can also do so by reading a blessed or uncursed scroll of remove curse.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. It is good practice to identify items before using them. At least you should make sure that the item is not cursed, when you equip it.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. An uncursed scroll of remove curse will remove curses from equipped items.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. A blessed scroll will remove curses from all items in the open inventory.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Dropping items on the altar will reveal their blessed/uncursed/cursed status.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Dipping items into holy water will make uncursed items blessed and cursed items uncursed.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Dipping items into unholy water will make uncursed items cursed and blessed items uncursed.");
            break;
        case MANUAL_GUIDE_TO_DRAGON_SCALE_MAILS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. If you polymorph while wearing a dragon scale mail, you will turn into a dragon and your dragon scale mail will revert to mere scales again.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. You can turn dragon scales into a dragon scale mail by enchanting them or by asking a smith to forge them into a mail.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. An orange dragon scale mail gives you sleep resistance and free action.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. A green dragon scale mail gives you poison resistance and sickness resistance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. A yellow dragon scale mail gives you acid resistance and petrification resistance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. A gray dragon scale mail gives you magic resistance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. A silver dragon scale mail gives you reflection.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. A red dragon scale mail gives you full fire resistance, protection from green slime, and regeneration.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. A white dragon scale mail gives you full cold resistance, protection from bisection, and energy regeneration.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. A blue dragon scale mail gives you full shock resistance and very fast speed.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "11. A black dragon scale mail gives you disintegration, death, and drain resistances.");
            break;
        case MANUAL_GUIDE_TO_PRAYING:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You may pray on turn 301 earliest.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Praying at low hit points or in a severe condition will heal you, if your God is not angry with you.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. After successfully praying, it is good practice to wait 1000 turns before praying again.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. If you pray too often, your God gets angry and your adventure is ruined.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. A blessed prayerstone will shimmer when it is safe to pray.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. You can make a prayerstone blessed by sacrificing on the altar while the stone is in open inventory.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Praying does not work in Gehennom.");
            break;
        case MANUAL_GUIDE_TO_ALTARS_AND_SACRIFICE:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Dropping items on the altar will reveal their blessed/uncursed/cursed status.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Sacrificing a corpse on the altar may increase your luck. The corpse needs to be a high level monster.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Sacrificing a corpse on the altar may grant you an artifact.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. If you drop potions of water on the altar and pray, they will be converted into holy water.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. You can sacrifice on an altar of another god than yours to convert it to your deity. The success chance depends on your level.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Do not sacrifice on an altar of another god than yours, if it is attended by a priest. Doing so will anger him or her.");
            break;
        case MANUAL_ASSORTED_TIPS_AND_TRICKS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can dip potions twice into a fountain, moat, or a sink to dilute them into potions of water.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. You can write scrolls and spellbooks with a magic marker.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You can wash scrolls and spellbooks clean by dipping them into a fountain, moat, or a sink.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. You can use a pick-axe or a similar tool to dig through walls. You can also dig through the floor.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. Disarming traps may generate useful items, such as darts or wands.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Searching can reveal secret doors. But it does not succeed always, so you should do it several times.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. You can search chests for traps.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Engraving with a dagger or another weapon will make it dull, that is, reduce its enchantment.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Engraving a text with a wand may produce effects on bugs on the ground, which may help you identify the wand.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. Engraving text with fingers is not permanent. The text will disappear soon.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "11. Engraving with a dagger is a good way to make a permanent engraving. Just dispose the dagger afterwards.");
            break;
        case MANUAL_ELEMENTARY_MECHANICS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Your encumbrance limit is based on your strength and constitution.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. If you are burdened, your speed will drop by 25%.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Your speed determines how fast you can move and attack.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. Skills increase your chance of hitting with an armor or casting a spell. You receive a skill slot for each level above 1.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. Skills typically require 1/2/3/4/5 skill slots to increase them to the next level at Unskilled/Basic/Skilled/Expert/Master level.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. You can reassign your skill points if you read a scroll of retraining.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. High strength gives you bonus to hit and to damage and more carrying capacity.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. High dexterity gives you bonus to hit and to AC.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. High consitution gives you bonus hit points and more carrying capacity.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. High intelligence gives you more mana and helps you to cast some spells better.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "11. High wisdom gives you more mana and helps you to cast some spells better.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "12. High charisma allows you to get discount on sold items and services. It also helps you to cast some spells better.");
            break;
        case MANUAL_PRINCIPLES_OF_MAGIC:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You need to mix spells before you can cast them.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Mixing spells requires material components, which you will find in the dungeon.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Spells come in 14 levels: Minor cantrip (c), major cantrip (C), and levels 1-12. Higher level spells are more difficult to cast.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. To cast spells well, you need to put skill points into magic schools and acquire items that increase your chance of casting spells.");
            break;
        case MANUAL_UNDERSTANDING_PETS_AND_HIRELINGS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Starting pets do not step voluntarily on cursed items.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. This is a great way to identify whether an item is cursed or not.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. If a pet steps relucatantly on an item, that item is cursed.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. You can yell for your pet, if it is far away, and it will come to you.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. When you go up or down the stairs, your pet needs to be at your side or at most two squares away for it to join you.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. You can chat to your pet to give it commands or to feed it with food.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. You can hire some peaceful monsters to join your party for a fee.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. You can equip your hirelings with items by chatting to them.");
            break;
        case MANUAL_ITEM_IDENTIFICATION_101:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You need to identify items before you know what they do.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. The items can be blessed, uncursed, or cursed. Uncursed items are normal.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Blessed items usually grant an additional bonus.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. Cursed items have adverse effects and you cannot remove them once equipped.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. Equipment can also have positive or negative enchantment bonuses.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Equipment with positive enchantment bonuses work better, while those with negative enchantment work worse.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. A scroll of identify identifies 1-3 items; 3 if the scroll is blessed, 2 if uncursed, and 1 if cursed.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. You can drop items on an altar to identify their blessed/uncursed/cursed status.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Some pets do not step on cursed items.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. You can talk to shopkeepers and the Oracle to identify items for a fee.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "11. You can talk to many NPCs, such as artificers, to identify some item classes for a fee.");
            break;
        case MANUAL_ITEM_IDENTIFICATION_102:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can use a touchstone to identify gems.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. You can use a wand of identify to identify items, 1 per charge.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. You can cast the Identify spell to identify items.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. You can cast the Detect Blessedness spell to determine the blessed/uncursed/cursed status of an item.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. You can try to kick a gray stone, to see if it moves. If not, it is a cursed loadstone.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. You can engrave with a wand a single letter on the ground, to see what it does.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. You can see what the price of an item is in a shop and then try to figure it out, what the item is.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. You can equip blessed and uncursed items to see what they do. Sometimes it is not obvious.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. You can zap a wand of probing at corpses to see what they do when eaten.");
            break;
        case MANUAL_GUIDE_TO_ESSENTIAL_RESISTANCES_VOL_I:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You should acquire various resistances to better withstand the challenges ahead.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Most critical resistances are magic resistance, reflection, and free action.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Also poison, sleep, sickness, petrification, and death resistances are important for survival.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. Certain magic items may bestow you with some of these resistances.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. Your role and race can also confer you resistances as you advance in experience levels.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. A suitable cloak, robe, or ioun stone will confer you magic resistance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. You can obtain reflection from an appropriate amulet, pair of bracers, or shield.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Free action is conferred only by a ring or a mythic weapon or armor of a suitable type.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Get poison resistance early on. Look for a ring or an amulet, or eat a suitable monster corpse.");
            break;
        case MANUAL_GUIDE_TO_ESSENTIAL_RESISTANCES_VOL_II:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You should acquire various resistances to better withstand the challenges ahead.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Some resistances such as sleep, sickness, and petrification resistances can be somewhat hard to come by.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. It is important to get sleep resistance early on. Look for an elf or gelatinous cube corpse.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. A healer's beak mask can confer you sickness resistance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. An amulet versus petrification will grant you petrification resistance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Look for an appropriate amulet, a ring, or a gown to get death resistance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. There are also resistances against brain damaging and blinding attacks.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. A nose ring of cerebral safeguarding will protect your brain.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Certain eyeglasses and goggles can confer you protection against blinding attacks.");
            break;
        case MANUAL_SECRETS_OF_SCARE_MONSTER:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can stand on a scroll of scare monster and monsters cannot attack you.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. A cursed scroll of scare monster will turn to dust if picked up.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. A scroll of scare monster will lose one level of the blessed/uncursed/cursed status when dropped on the ground.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. If you attack while standing on a scroll of scare monster, the scroll will become cursed.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. It is good practice to bless scrolls of scare monster, so that you can drop them on the ground and pick up again.");
            break;
        case MANUAL_GURATHULS_GUIDE_TO_ASCENSION:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. It is good practice to genocide dangerous monsters with a blessed scroll of genocide. This will prevent them appearing later in the game.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. You can create multiple monsters of your choice with a cursed scroll of genocide.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Wizard of Yendor may replace your amulet with a cheap plastic imitation. It is good practice to name the real amulet as soon as you get it.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. It is essential for survival that you get good protective equipment.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. You need at least magic resistance, reflection, free action, sleep resistance, and death resistance to survive some nasty attacks.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Magic resistance will protect you from polymorph traps and some spell caster spells.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Reflection will protect you from rays.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Free action will protect you from thrown potion of paralysis and paralysing monsters, such as gelatinous cube.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Sleep resistance will protect you from sleep gas traps and thrown potions of sleep.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. Death resistance will protect you from some instant death attacks, such as the touch of death spell of a greater mummy high priest.");
            break;
        case MANUAL_MASTER_CLASS_IN_WANDS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. You can charge a wand of wishing only once.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Be sure to use a blessed scroll of charging or a fully charging service on the wand to get maximum number of wishes.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. It is often useful to wish for a wand of disintegration, a wand of petrification, or a wand of death to deal with arch-devils and the Wizard of Yendor in Gehennom.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. If you are being slimed by a green slime, you can zap a wand of fire at yourself, and it will cure your sliming. Alternatively, you can use a scroll of fire.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. You can polymorph objects with a wand of polymorph. One charge affects 7/4/1 items based on the wand's blessed/uncursed/cursed status.");
            break;
        case MANUAL_INFERNAL_INHABITANTS_OF_GEHENNOM:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Demons and devils are immune to death attacks.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Some devils can spawn with a bullwhip that can disarm you from your weapon. It is best to kill them from a distance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Demons and devils can gate in more of their kind with their melee attack. It is often best to kill them from a distance.");
            break;
        case MANUAL_ADVANCED_READING_IN_KNOWN_MONSTERS:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. Some sea monsters, such as eels, can drown you. It is often a good idea to kill them from a distance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. Some monsters, such as trolls, regenerate and revive after being killed. It is often best to use a tinning kit on them to get rid of their corpses. Eating the corpses is another possibility.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. If you are bare-handed and wearing no armor, nurses will heal you. They can also increase your maximum hit points.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. Cockatrices and chickatrices can petrify you if they touch you successfully.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. Touching bare-handed or eating a cockatrice or chickatrice corpse will stone you.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. Eating a lizard corpse or a dragon fruit will cure stoning. They will also grant 13-turn protection from further petrification.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. If you fall into a pit while holding a cockatrice or chickatrice corpse you will stone immediately.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. It is good practice to keep one lizard corpse or dragon fruit in open inventory in case you will start petrification.");
            break;
        case MANUAL_MANUAL_OF_THE_PLANES:
            putstr(datawin, ATR_INDENT_AT_PERIOD, "1. To reach the Astral Plane and the High Temples there, you must pass through the four Elemental Planes.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "2. These are the Elemental Planes of Earth, Air, Fire, and Water.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "3. Within each of these Elemental Planes, there is a secret portal to the next Elemental Plane you must find.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "4. You must be sufficiently prepared to survive the alien conditions within each such Elemental Plane.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "5. Within the Elemental Plane of Earth, tools and spells to cut off rock are critical.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "6. So are the means to detect small cavities within the rock, where the portal to the next Elemental Plane may be located.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "7. Within the Elemental Plane of Air, you must possess the ability to levitate or fly, for it is impossible to move otherwise.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "8. Travelling through the Elemental Plane of Fire will be greatly assisted by fire resistance.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "9. Further, you must be capable of crossing pools of lava that will block your way.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "10. The last Elemental Plane is Water. The portal will create a magical air bubble, which is essential for survival.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "11. Take utmost care to remain within the bubble and not to be dragged into the water by aquatic creatures.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "12. Finally, the Astral Plane is accessed through a portal from the Elemental Plane of Water.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "13. The High Temples of the Aligned Gods are located on the Astral Plane, along with a great number of minions of the gods.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "14. However, the Riders of the Apocalypse, terrifying beings personifying ancient evils, also inhabit the Plane.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "15. The three Riders are Death, Pestilence, and Famine.");
            putstr(datawin, ATR_INDENT_AT_PERIOD, "16. The Riders must be passed to reach the inner fanes of the High Temples. Great many have fallen in the attempt.");
            break;
        default:
            putstr(datawin, 0, "(This manual seems impossible.)");
            break;
        }
    }
    putstr(datawin, 0, "");

    Sprintf(buf, "[%s]", manual_names[mnlidx]);
    putstr(datawin, ATR_HEADING, buf);
    issue_gui_command(GUI_CMD_LIBRARY_MANUAL, datawin, mnlidx, manual_names[mnlidx]);
    display_nhwindow(datawin, TRUE);
    destroy_nhwindow(datawin);

    Sprintf(buf, "[%s]", manual_names[mnlidx]);
    putmsghistory(buf, FALSE);

}

const char *
lookup_novel(lookname, idx)
const char *lookname;
short* idx;
{
    short k;

    /* Take American or U.K. spelling of this one */
    if (!strcmpi(The(lookname), "The Color of Magic"))
        lookname = sir_Terry_novels[0];

    for (k = 0; k < SIZE(sir_Terry_novels); ++k) {
        if (!strcmpi(lookname, sir_Terry_novels[k])
            || !strcmpi(The(lookname), sir_Terry_novels[k])) {
            if (idx)
                *idx = k;
            return sir_Terry_novels[k];
        }
    }
    /* name not found; if novelidx is already set, override the name */
    if (idx && *idx >= 0 && *idx < SIZE(sir_Terry_novels))
        return sir_Terry_novels[*idx];

    return (const char *) 0;
}

const char*
lookup_manual(lookname, idx)
const char* lookname;
short* idx;
{
    short k;

    for (k = 0; k < SIZE(manual_names); ++k) {
        if (!strcmpi(lookname, manual_names[k])
            || !strcmpi(The(lookname), manual_names[k])) {
            if (idx)
                *idx = k;
            return manual_names[k];
        }
    }
    /* name not found; if idx is already set, override the name */
    if (idx && *idx >= 0 && *idx < SIZE(manual_names))
        return manual_names[*idx];

    return (const char*)0;
}

void
reset_doname(VOID_ARGS)
{
    via_naming = 0;
}

/*do_name.c*/
