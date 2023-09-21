/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    sp_lev.c    $NHDT-Date: 1553787633 2019/03/28 15:40:33 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.111 $ */
/*      Copyright (c) 1989 by Jean-Christophe Collet */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * This file contains the various functions that are related to the special
 * levels.
 *
 * It contains also the special level loader.
 */

#include "hack.h"
#include "dlb.h"
#include "sp_lev.h"

#ifdef _MSC_VER
 #pragma warning(push)
 #pragma warning(disable : 4244)
#endif

typedef void FDECL((*select_iter_func), (int, int, genericptr));
typedef void FDECL((*select_iter_func2), (int, int, genericptr, genericptr));
typedef void FDECL((*select_iter_func3), (int, int, genericptr, genericptr, genericptr));
#if 0 /* UNUSED */
typedef void FDECL((*select_iter_func4), (int, int, genericptr, genericptr, genericptr, genericptr));
#endif
typedef void FDECL((*select_iter_func5), (int, int, genericptr, genericptr, genericptr, genericptr, genericptr));

extern void FDECL(mkmap, (lev_init *));

STATIC_DCL void NDECL(solidify_map);
STATIC_DCL void FDECL(splev_stack_init, (struct splevstack *));
STATIC_DCL void FDECL(splev_stack_done, (struct splevstack *));
STATIC_DCL void FDECL(splev_stack_push, (struct splevstack *,
                                         struct opvar *));
STATIC_DCL struct opvar *FDECL(splev_stack_pop, (struct splevstack *));
STATIC_DCL struct splevstack *FDECL(splev_stack_reverse,
                                    (struct splevstack *));
STATIC_DCL struct opvar *FDECL(opvar_new_str, (char *));
STATIC_DCL struct opvar *FDECL(opvar_new_int, (long));
STATIC_DCL struct opvar *FDECL(opvar_new_coord, (int, int));
#if 0
STATIC_DCL struct opvar * FDECL(opvar_new_region, (int,int, int,int));
#endif /*0*/
STATIC_DCL struct opvar *FDECL(opvar_clone, (struct opvar *));
STATIC_DCL struct opvar *FDECL(opvar_var_conversion, (struct sp_coder *,
                                                      struct opvar *));
STATIC_DCL struct splev_var *FDECL(opvar_var_defined, (struct sp_coder *,
                                                       char *));
STATIC_DCL struct opvar *FDECL(splev_stack_getdat, (struct sp_coder *,
                                                    XCHAR_P));
STATIC_DCL struct opvar *FDECL(splev_stack_getdat_any, (struct sp_coder *));
STATIC_DCL void FDECL(variable_list_del, (struct splev_var *));
STATIC_DCL void FDECL(lvlfill_maze_grid, (int, int, int, int, SCHAR_P));
STATIC_DCL void FDECL(lvlfill_solid, (SCHAR_P, SCHAR_P));
STATIC_DCL void FDECL(set_wall_property, (XCHAR_P, XCHAR_P, XCHAR_P, XCHAR_P,
                                          int));
STATIC_DCL void NDECL(shuffle_alignments);
STATIC_DCL void NDECL(count_features);
STATIC_DCL void NDECL(remove_boundary_syms);
STATIC_DCL void FDECL(set_door_orientation, (int, int));
STATIC_DCL void FDECL(maybe_add_door, (int, int, struct mkroom *));
STATIC_DCL void NDECL(link_doors_rooms);
STATIC_DCL void NDECL(fill_rooms);
STATIC_DCL int NDECL(rnddoor);
STATIC_DCL int NDECL(rndtrap);
STATIC_DCL void FDECL(get_location, (schar *, schar *, int, struct mkroom *));
STATIC_DCL boolean FDECL(is_ok_location, (SCHAR_P, SCHAR_P, int));
STATIC_DCL unpacked_coord FDECL(get_unpacked_coord, (long, int));
STATIC_DCL void FDECL(get_location_coord, (schar *, schar *, int,
                                           struct mkroom *, long));
STATIC_DCL void FDECL(get_room_loc, (schar *, schar *, struct mkroom *));
STATIC_DCL void FDECL(get_free_room_loc, (schar *, schar *,
                                          struct mkroom *, packed_coord));
STATIC_DCL boolean FDECL(create_subroom, (struct mkroom *, XCHAR_P, XCHAR_P,
                                          XCHAR_P, XCHAR_P, XCHAR_P, XCHAR_P, int, int, int, int, SHORT_P));
STATIC_DCL void FDECL(create_door, (room_door *, struct mkroom *));
STATIC_DCL void FDECL(create_trap, (spltrap *, struct mkroom *));
STATIC_DCL int FDECL(noncoalignment, (ALIGNTYP_P));
STATIC_DCL boolean FDECL(m_bad_boulder_spot, (int, int));
STATIC_DCL int FDECL(pm_to_humidity, (struct permonst *));
STATIC_DCL void FDECL(create_monster, (monster *, struct mkroom *));
STATIC_DCL void FDECL(create_object, (object *, struct mkroom *));
STATIC_DCL void FDECL(create_lever, (spllever*, struct mkroom*));
STATIC_DCL void FDECL(create_altar, (altar *, struct mkroom *));
STATIC_DCL void FDECL(create_anvil, (anvil*, struct mkroom*));
STATIC_DCL void FDECL(create_decoration, (decoration*, struct mkroom*));
STATIC_DCL void FDECL(create_modron_portal, (modron_portal*, struct mkroom*));
STATIC_DCL void FDECL(create_npc, (npc_create_info*, struct mkroom*));
STATIC_DCL void FDECL(replace_terrain, (replaceterrain *, struct mkroom *));
STATIC_DCL boolean FDECL(search_door, (struct mkroom *,
                                       xchar *, xchar *, XCHAR_P, int));
STATIC_DCL void NDECL(fix_stair_rooms);
STATIC_DCL void FDECL(create_corridor, (corridor *));
STATIC_DCL struct mkroom *FDECL(build_room, (room *, struct mkroom *));
STATIC_DCL void FDECL(light_region, (region *));
STATIC_DCL void FDECL(wallify_map, (int, int, int, int));
STATIC_DCL void FDECL(maze1xy, (coord *, int));
STATIC_DCL void NDECL(fill_empty_maze);
STATIC_DCL boolean FDECL(sp_level_loader, (dlb *, sp_lev *));
STATIC_DCL boolean FDECL(sp_level_free, (sp_lev *));
STATIC_DCL void FDECL(splev_initlev, (lev_init *));
STATIC_DCL struct sp_frame *FDECL(frame_new, (long));
STATIC_DCL void FDECL(frame_del, (struct sp_frame *));
STATIC_DCL void FDECL(spo_frame_push, (struct sp_coder *));
STATIC_DCL void FDECL(spo_frame_pop, (struct sp_coder *));
STATIC_DCL long FDECL(sp_code_jmpaddr, (long, long));
STATIC_DCL void FDECL(spo_call, (struct sp_coder *));
STATIC_DCL void FDECL(spo_return, (struct sp_coder *));
STATIC_DCL void FDECL(spo_end_moninvent, (struct sp_coder *));
STATIC_DCL void FDECL(spo_pop_container, (struct sp_coder *));
STATIC_DCL void FDECL(spo_message, (struct sp_coder *));
STATIC_DCL void FDECL(spo_monster, (struct sp_coder *));
STATIC_DCL void FDECL(spo_object, (struct sp_coder *));
STATIC_DCL void FDECL(spo_lever, (struct sp_coder*));
STATIC_DCL void FDECL(spo_modron_portal, (struct sp_coder*));
STATIC_DCL void FDECL(spo_modron_level_teleporter, (struct sp_coder*));
STATIC_DCL void FDECL(spo_level_flags, (struct sp_coder *));
STATIC_DCL void FDECL(spo_initlevel, (struct sp_coder *));
STATIC_DCL void FDECL(spo_tileset, (struct sp_coder*));
STATIC_DCL void FDECL(spo_forest, (struct sp_coder*));
STATIC_DCL void FDECL(spo_monster_generation, (struct sp_coder*));
STATIC_DCL void FDECL(spo_boundary_type, (struct sp_coder*));
STATIC_DCL void FDECL(spo_engraving, (struct sp_coder *));
STATIC_DCL void FDECL(spo_mineralize, (struct sp_coder *));
STATIC_DCL void FDECL(spo_room, (struct sp_coder *));
STATIC_DCL void FDECL(spo_endroom, (struct sp_coder *));
STATIC_DCL void FDECL(spo_stair, (struct sp_coder *));
STATIC_DCL void FDECL(spo_ladder, (struct sp_coder *));
STATIC_DCL void FDECL(spo_grave, (struct sp_coder *));
STATIC_DCL void FDECL(spo_brazier, (struct sp_coder*));
STATIC_DCL void FDECL(spo_signpost, (struct sp_coder*));
STATIC_DCL void FDECL(spo_tree, (struct sp_coder*));
STATIC_DCL void FDECL(spo_altar, (struct sp_coder *));
STATIC_DCL void FDECL(spo_trap, (struct sp_coder *));
STATIC_DCL void FDECL(spo_gold, (struct sp_coder *));
STATIC_DCL void FDECL(spo_corridor, (struct sp_coder *));
STATIC_DCL void FDECL(selection_setpoint, (int, int, struct opvar *, XCHAR_P));
STATIC_DCL struct opvar *FDECL(selection_not, (struct opvar *));
STATIC_DCL struct opvar *FDECL(selection_logical_oper, (struct opvar *,
                                                     struct opvar *, CHAR_P));
STATIC_DCL struct opvar *FDECL(selection_filter_mapchar, (struct opvar *,
                                                          struct opvar *));
STATIC_DCL void FDECL(selection_filter_percent, (struct opvar *, int));
STATIC_DCL int FDECL(selection_rndcoord, (struct opvar *, schar *, schar *,
                                          BOOLEAN_P));
STATIC_DCL void FDECL(selection_do_grow, (struct opvar *, int));
STATIC_DCL int FDECL(floodfillchk_match_under, (int, int));
STATIC_DCL int FDECL(floodfillchk_match_accessible, (int, int));
STATIC_DCL boolean FDECL(sel_flood_havepoint, (int, int,
                                               xchar *, xchar *, int));
STATIC_DCL void FDECL(selection_do_ellipse, (struct opvar *, int, int,
                                             int, int, int));
STATIC_DCL long FDECL(line_dist_coord, (long, long, long, long, long, long));
STATIC_DCL void FDECL(selection_do_gradient, (struct opvar *, long, long, long,
                                              long, long, long, long, long));
STATIC_DCL void FDECL(selection_do_line, (SCHAR_P, SCHAR_P, SCHAR_P, SCHAR_P,
                                          struct opvar *));
STATIC_DCL void FDECL(selection_do_randline, (SCHAR_P, SCHAR_P, SCHAR_P,
                                              SCHAR_P, SCHAR_P, SCHAR_P,
                                              struct opvar *));
STATIC_DCL void FDECL(selection_iterate, (struct opvar *, select_iter_func,
                                          genericptr_t));
STATIC_DCL void FDECL(selection_iterate2, (struct opvar*, select_iter_func2,
    genericptr_t, genericptr_t));
STATIC_DCL void FDECL(selection_iterate3, (struct opvar*, select_iter_func3,
    genericptr_t, genericptr_t, genericptr_t));
#if 0 /* UNUSED */
STATIC_DCL void FDECL(selection_iterate4, (struct opvar*, select_iter_func4,
    genericptr_t, genericptr_t, genericptr_t, genericptr_t));
#endif
STATIC_DCL void FDECL(selection_iterate5, (struct opvar*, select_iter_func5,
    genericptr_t, genericptr_t, genericptr_t, genericptr_t, genericptr_t));
STATIC_DCL void FDECL(sel_set_ter, (int, int, genericptr_t));
STATIC_DCL void FDECL(sel_set_feature, (int, int, genericptr_t));
STATIC_DCL void FDECL(sel_set_feature2, (int, int, genericptr_t, genericptr_t));
STATIC_DCL void FDECL(sel_set_floor, (int, int, genericptr_t, genericptr_t));
STATIC_DCL void FDECL(sel_set_subtype, (int, int, genericptr_t, genericptr_t));
STATIC_DCL void FDECL(sel_set_carpet_piece, (int, int, genericptr_t, genericptr_t, genericptr_t));
STATIC_DCL void FDECL(sel_set_door, (int, int, genericptr_t, genericptr_t, genericptr_t, genericptr_t, genericptr_t));
STATIC_DCL void FDECL(sel_set_tileset, (int, int, genericptr_t));
STATIC_DCL void FDECL(spo_door, (struct sp_coder *));
STATIC_DCL void FDECL(spo_feature, (struct sp_coder *));
STATIC_DCL void FDECL(spo_fountain, (struct sp_coder*));
STATIC_DCL void FDECL(spo_anvil, (struct sp_coder*));
STATIC_DCL void FDECL(spo_decoration, (struct sp_coder*));
STATIC_DCL void FDECL(spo_floor, (struct sp_coder*));
STATIC_DCL void FDECL(spo_subtype, (struct sp_coder*));
STATIC_DCL void FDECL(spo_carpet, (struct sp_coder*));
STATIC_DCL void FDECL(spo_npc, (struct sp_coder*));
STATIC_DCL void FDECL(spo_terrain, (struct sp_coder *));
STATIC_DCL void FDECL(spo_replace_terrain, (struct sp_coder *));
STATIC_DCL boolean FDECL(generate_way_out_method, (int, int, struct opvar *));
STATIC_DCL void NDECL(ensure_way_out);
STATIC_DCL void FDECL(spo_levregion, (struct sp_coder *));
STATIC_DCL void FDECL(spo_region, (struct sp_coder *));
STATIC_DCL void FDECL(spo_special_region, (struct sp_coder*));
STATIC_DCL void FDECL(spo_special_tileset, (struct sp_coder*));
STATIC_DCL void FDECL(spo_naming, (struct sp_coder*));
STATIC_DCL void FDECL(spo_drawbridge, (struct sp_coder *));
STATIC_DCL void FDECL(spo_mazewalk, (struct sp_coder *));
STATIC_DCL void FDECL(spo_wall_property, (struct sp_coder *));
STATIC_DCL void FDECL(spo_room_door, (struct sp_coder *));
STATIC_DCL void FDECL(sel_set_wallify, (int, int, genericptr_t));
STATIC_DCL void FDECL(spo_wallify, (struct sp_coder *));
STATIC_DCL void FDECL(spo_map, (struct sp_coder *));
STATIC_DCL void FDECL(spo_jmp, (struct sp_coder *, sp_lev *));
STATIC_DCL void FDECL(spo_conditional_jump, (struct sp_coder *, sp_lev *));
STATIC_DCL void FDECL(spo_var_init, (struct sp_coder *));
#if 0
STATIC_DCL long FDECL(opvar_array_length, (struct sp_coder *));
#endif /*0*/
STATIC_DCL void FDECL(spo_shuffle_array, (struct sp_coder *));
STATIC_DCL boolean FDECL(sp_level_coder, (sp_lev *));

#define LEFT 1
#define H_LEFT 2
#define CENTER 3
#define H_RIGHT 4
#define RIGHT 5

#define TOP 1
#define BOTTOM 5

#define sq(x) ((x) * (x))

#define XLIM 4
#define YLIM 3

#define Fread (void) dlb_fread
#define Fgetc (schar) dlb_fgetc
#define New(type) (type *) alloc(sizeof(type))
#define NewTab(type, size) (type **) alloc(sizeof(type *) * (size_t) size)
#define Free(ptr) if (ptr) free((genericptr_t) (ptr))

extern struct engr *head_engr;

extern int min_rx, max_rx, min_ry, max_ry; /* from mkmap.c */

/* positions touched by level elements explicitly defined in the des-file */
STATIC_VAR char SpLev_Map[COLNO][ROWNO];

STATIC_VAR aligntyp ralign[3] = { AM_CHAOTIC, AM_NEUTRAL, AM_LAWFUL };
STATIC_VAR NEARDATA xchar xstart, ystart;
STATIC_VAR NEARDATA char xsize, ysize;

struct lev_msg *lev_message = 0;

lev_region *lregions = 0;
int num_lregions = 0;

STATIC_VAR boolean splev_init_present = FALSE;
STATIC_VAR boolean icedpools = FALSE;
STATIC_VAR int mines_prize_count = 0, soko_prize_count = 0; /* achievements */

STATIC_VAR struct obj *container_obj[MAX_CONTAINMENT];
STATIC_VAR int container_idx = 0;
STATIC_VAR struct monst *invent_carrying_monster = NULL;

#define SPLEV_STACK_RESERVE 128

void
solidify_map()
{
    xchar x, y;

    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (IS_STWALL(levl[x][y].typ) && !SpLev_Map[x][y])
                levl[x][y].wall_info |= (W_NONDIGGABLE | W_NONPASSWALL);
}

void
splev_stack_init(st)
struct splevstack *st;
{
    if (st) {
        st->depth = 0;
        st->depth_alloc = SPLEV_STACK_RESERVE;
        st->stackdata =
           (struct opvar **) alloc((size_t)st->depth_alloc * sizeof (struct opvar *));
    }
}

void
splev_stack_done(st)
struct splevstack *st;
{
    if (st) {
        int i;

        if (st->stackdata && st->depth) {
            for (i = 0; i < st->depth; i++) {
                switch (st->stackdata[i]->spovartyp) {
                default:
                case SPOVAR_NULL:
                case SPOVAR_COORD:
                case SPOVAR_REGION:
                case SPOVAR_MAPCHAR:
                case SPOVAR_MONST:
                case SPOVAR_OBJ:
                case SPOVAR_INT:
                    break;
                case SPOVAR_VARIABLE:
                case SPOVAR_STRING:
                case SPOVAR_SEL:
                    Free(st->stackdata[i]->vardata.str);
                    st->stackdata[i]->vardata.str = NULL;
                    break;
                }
                Free(st->stackdata[i]);
                st->stackdata[i] = NULL;
            }
        }
        Free(st->stackdata);
        st->stackdata = NULL;
        st->depth = st->depth_alloc = 0;
        Free(st);
    }
}

void
splev_stack_push(st, v)
struct splevstack *st;
struct opvar *v;
{
    if (!st || !v)
        return;

    if (!st->stackdata)
    {
        panic("splev_stack_push: no stackdata allocated?");
        return;
    }
    if (st->depth >= st->depth_alloc) 
    {
        struct opvar **tmp = (struct opvar **) alloc(
           ((size_t)st->depth_alloc + SPLEV_STACK_RESERVE) * sizeof (struct opvar *));

        (void) memcpy(tmp, st->stackdata,
            (size_t)st->depth_alloc * sizeof(struct opvar *));
        Free(st->stackdata);
        st->stackdata = tmp;
        st->depth_alloc += SPLEV_STACK_RESERVE;
    }

    st->stackdata[st->depth] = v;
    st->depth++;
}

struct opvar *
splev_stack_pop(st)
struct splevstack *st;
{
    struct opvar *ret = NULL;

    if (!st)
        return ret;

    if (!st->stackdata)
    {
        panic("splev_stack_pop: no stackdata allocated?");
        return (struct opvar *)0;
    }

    if (st->depth)
    {
        st->depth--;
        ret = st->stackdata[st->depth];
        st->stackdata[st->depth] = NULL;
        return ret;
    }
    else
        impossible("splev_stack_pop: empty stack?");
    return ret;
}

struct splevstack *
splev_stack_reverse(st)
struct splevstack *st;
{
    long i;
    struct opvar *tmp;

    if (!st)
        return NULL;
    
    if (!st->stackdata)
    {
        panic("splev_stack_reverse: no stackdata allocated?");
        return (struct splevstack*)0;
    }

    for (i = 0; i < (st->depth / 2); i++) 
    {
        tmp = st->stackdata[i];
        st->stackdata[i] = st->stackdata[st->depth - i - 1];
        st->stackdata[st->depth - i - 1] = tmp;
    }
    return st;
}

#define OV_typ(o) (o->spovartyp)
#define OV_i(o) (o->vardata.l)
#define OV_s(o) (o->vardata.str)

#define OV_pop_i(x) (x = splev_stack_getdat(coder, SPOVAR_INT))
#define OV_pop_c(x) (x = splev_stack_getdat(coder, SPOVAR_COORD))
#define OV_pop_r(x) (x = splev_stack_getdat(coder, SPOVAR_REGION))
#define OV_pop_s(x) (x = splev_stack_getdat(coder, SPOVAR_STRING))
#define OV_pop(x) (x = splev_stack_getdat_any(coder))
#define OV_pop_typ(x, typ) (x = splev_stack_getdat(coder, typ))

struct opvar *
opvar_new_str(s)
char *s;
{
    struct opvar *tmpov = (struct opvar *) alloc(sizeof (struct opvar));

    tmpov->spovartyp = SPOVAR_STRING;
    if (s) {
        size_t len = strlen(s);
        tmpov->vardata.str = (char *) alloc(len + 1);
        (void) memcpy((genericptr_t) tmpov->vardata.str, (genericptr_t) s,
                      len);
        tmpov->vardata.str[len] = '\0';
    } else
        tmpov->vardata.str = NULL;
    return tmpov;
}

struct opvar *
opvar_new_int(i)
long i;
{
    struct opvar *tmpov = (struct opvar *) alloc(sizeof (struct opvar));

    tmpov->spovartyp = SPOVAR_INT;
    tmpov->vardata.l = i;
    return tmpov;
}

struct opvar *
opvar_new_coord(x, y)
int x, y;
{
    struct opvar *tmpov = (struct opvar *) alloc(sizeof (struct opvar));

    tmpov->spovartyp = SPOVAR_COORD;
    tmpov->vardata.l = SP_COORD_PACK(x, y);
    return tmpov;
}

#if 0
struct opvar *
opvar_new_region(x1,y1,x2,y2)
     int x1,y1,x2,y2;
{
    struct opvar *tmpov = (struct opvar *) alloc(sizeof (struct opvar));

    tmpov->spovartyp = SPOVAR_REGION;
    tmpov->vardata.l = SP_REGION_PACK(x1,y1,x2,y2);
    return tmpov;
}
#endif /*0*/

void
opvar_free_x(ov)
struct opvar *ov;
{
    if (!ov)
        return;
    switch (ov->spovartyp) {
    case SPOVAR_COORD:
    case SPOVAR_REGION:
    case SPOVAR_MAPCHAR:
    case SPOVAR_MONST:
    case SPOVAR_OBJ:
    case SPOVAR_INT:
        break;
    case SPOVAR_VARIABLE:
    case SPOVAR_STRING:
    case SPOVAR_SEL:
        Free(ov->vardata.str);
        break;
    default:
        impossible("Unknown opvar value type (%i)!", ov->spovartyp);
    }
    Free(ov);
}

/*
 * Name of current function for use in messages:
 * __func__     -- C99 standard;
 * __FUNCTION__ -- gcc extension, starting before C99 and continuing after;
 *                 picked up by other compilers (or vice versa?);
 * __FUNC__     -- supported by Borland;
 * nhFunc       -- slightly intrusive but fully portable GnollHack construct
 *                 for any version of any compiler.
 */
#define opvar_free(ov)                                    \
    do {                                                  \
        if (ov) {                                         \
            opvar_free_x(ov);                             \
            ov = NULL;                                    \
        } else                                            \
            impossible("opvar_free(), %s", nhFunc);       \
    } while (0)

struct opvar *
opvar_clone(ov)
struct opvar *ov;
{
    struct opvar *tmpov;

    if (!ov)
    {
        panic("no opvar to clone");
        return (struct opvar *)0;
    }

    tmpov = (struct opvar *) alloc(sizeof(struct opvar));
    tmpov->spovartyp = ov->spovartyp;
    switch (ov->spovartyp) {
    case SPOVAR_COORD:
    case SPOVAR_REGION:
    case SPOVAR_MAPCHAR:
    case SPOVAR_MONST:
    case SPOVAR_OBJ:
    case SPOVAR_INT:
        tmpov->vardata.l = ov->vardata.l;
        break;
    case SPOVAR_VARIABLE:
    case SPOVAR_STRING:
    case SPOVAR_SEL:
        tmpov->vardata.str = dupstr(ov->vardata.str);
        break;
    default:
        impossible("Unknown push value type (%i)!", ov->spovartyp);
    }
    return tmpov;
}

struct opvar *
opvar_var_conversion(coder, ov)
struct sp_coder *coder;
struct opvar *ov;
{
    static const char nhFunc[] = "opvar_var_conversion";
    struct splev_var *tmp;
    struct opvar *tmpov;
    struct opvar *array_idx = NULL;

    if (!coder || !ov)
        return NULL;
    if (ov->spovartyp != SPOVAR_VARIABLE)
        return ov;
    tmp = coder->frame->variables;
    while (tmp) {
        if (!strcmp(tmp->name, OV_s(ov))) {
            if ((tmp->svtyp & SPOVAR_ARRAY)) {
                array_idx = opvar_var_conversion(coder,
                                               splev_stack_pop(coder->stack));
                if (!array_idx || OV_typ(array_idx) != SPOVAR_INT)
                {
                    panic("array idx not an int");
                    return (struct opvar*)0;
                }
                if (tmp->array_len < 1)
                {
                    panic("array len < 1");
                    return (struct opvar*)0;
                }
                OV_i(array_idx) = (OV_i(array_idx) % tmp->array_len);
                tmpov = opvar_clone(tmp->data.arrayvalues[OV_i(array_idx)]);
                opvar_free(array_idx);
                return tmpov;
            } else {
                tmpov = opvar_clone(tmp->data.value);
                return tmpov;
            }
        }
        tmp = tmp->next;
    }
    return NULL;
}

struct splev_var *
opvar_var_defined(coder, name)
struct sp_coder *coder;
char *name;
{
    struct splev_var *tmp;

    if (!coder)
        return NULL;
    tmp = coder->frame->variables;
    while (tmp) {
        if (!strcmp(tmp->name, name))
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

struct opvar *
splev_stack_getdat(coder, typ)
struct sp_coder *coder;
xchar typ;
{
    static const char nhFunc[] = "splev_stack_getdat";
    if (coder && coder->stack) {
        struct opvar *tmp = splev_stack_pop(coder->stack);
        struct opvar *ret = NULL;

        if (!tmp)
        {
            panic("no value type %i in stack.", typ);
            return (struct opvar*)0;
        }
        if (tmp->spovartyp == SPOVAR_VARIABLE)
        {
            ret = opvar_var_conversion(coder, tmp);
            opvar_free(tmp);
            tmp = ret;
        }
        if (tmp->spovartyp == typ)
            return tmp;
        else opvar_free(tmp);
    }
    return NULL;
}

struct opvar *
splev_stack_getdat_any(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "splev_stack_getdat_any";
    if (coder && coder->stack) {
        struct opvar *tmp = splev_stack_pop(coder->stack);
        if (tmp && tmp->spovartyp == SPOVAR_VARIABLE) {
            struct opvar *ret = opvar_var_conversion(coder, tmp);
            opvar_free(tmp);
            return ret;
        }
        return tmp;
    }
    return NULL;
}

void
variable_list_del(varlist)
struct splev_var *varlist;
{
    static const char nhFunc[] = "variable_list_del";
    struct splev_var *tmp = varlist;

    if (!tmp)
        return;
    while (tmp) {
        Free(tmp->name);
        if ((tmp->svtyp & SPOVAR_ARRAY)) {
            long idx = tmp->array_len;

            while (idx-- > 0) {
                opvar_free(tmp->data.arrayvalues[idx]);
            };
            Free(tmp->data.arrayvalues);
        } else {
            opvar_free(tmp->data.value);
        }
        tmp = varlist->next;
        Free(varlist);
        varlist = tmp;
    }
}

void
lvlfill_maze_grid(x1, y1, x2, y2, filling)
int x1, y1, x2, y2;
schar filling;
{
    int x, y, typ;

    for (x = x1; x <= x2; x++)
        for (y = y1; y <= y2; y++)
        {
            if (level.flags.corrmaze)
                typ = STONE;
            else
                typ = (y < 2 || ((x % 2) && (y % 2))) ? STONE : filling;

            set_initial_location_type_at(x, y, typ);
            initialize_location(&levl[x][y]);
        }
}

void
lvlfill_solid(filling, lit)
schar filling;
schar lit;
{
    int x, y;

    for (x = 1; x < COLNO /*x_maze_max*/; x++)
        for (y = 0; y < ROWNO /*y_maze_max*/; y++) {
            SET_TYPLIT(x, y, filling, -1, lit);
        }
}

/*
 * Make walls of the area (x1, y1, x2, y2) non diggable/non passwall-able
 */
STATIC_OVL void
set_wall_property(x1, y1, x2, y2, prop)
xchar x1, y1, x2, y2;
int prop;
{
    register xchar x, y;
    struct rm *lev;

    x1 = max(x1, 1);
    x2 = min(x2, COLNO - 1);
    y1 = max(y1, 0);
    y2 = min(y2, ROWNO - 1);
    for (y = y1; y <= y2; y++)
        for (x = x1; x <= x2; x++) {
            lev = &levl[x][y];
            if (IS_STWALL(lev->typ) || IS_TREE(lev->typ)
                /* 3.6.2: made iron bars eligible to be flagged nondiggable
                   (checked by chewing(hack.c) and zap_over_floor(zap.c)) */
                || lev->typ == IRONBARS)
                lev->wall_info |= prop;
        }
}

STATIC_OVL void
shuffle_alignments()
{
    int i;
    aligntyp atmp;

    /* shuffle 3 alignments */
    i = rn2(3);
    atmp = ralign[2];
    ralign[2] = ralign[i];
    ralign[i] = atmp;
    if (rn2(2)) {
        atmp = ralign[1];
        ralign[1] = ralign[0];
        ralign[0] = atmp;
    }
}

/*
 * Count the different features (sinks, fountains) in the level.
 */
STATIC_OVL void
count_features()
{
    xchar x, y;

    level.flags.nfountains = level.flags.nsinks = 0;
    for (y = 0; y < ROWNO; y++)
        for (x = 0; x < COLNO; x++) {
            int typ = levl[x][y].typ;
            if (typ == FOUNTAIN)
                level.flags.nfountains++;
            else if (typ == SINK)
                level.flags.nsinks++;
        }
}

void
remove_boundary_syms()
{
    /*
     * If any CROSSWALLs are found, must change to ROOM after REGION's
     * are laid out.  CROSSWALLS are used to specify "invisible"
     * boundaries where DOOR syms look bad or aren't desirable.
     */
    xchar x, y;
    boolean has_bounds = FALSE;

    for (x = 0; x < COLNO - 1; x++)
        for (y = 0; y < ROWNO - 1; y++)
            if (levl[x][y].typ == CROSSWALL) 
            {
                has_bounds = TRUE;
                break;
            }
    if (has_bounds) 
    {
        for (x = 0; x < x_maze_max; x++)
            for (y = 0; y < y_maze_max; y++)
                if ((levl[x][y].typ == CROSSWALL) && SpLev_Map[x][y])
                {
                    set_initial_location_type_at(x, y, IS_FLOOR(level.flags.boundary_type) ? level.flags.boundary_type : ROOM);
                }
    }
}

/* used by sel_set_door() and link_doors_rooms() */
STATIC_OVL void
set_door_orientation(x, y)
int x, y;
{
    boolean wleft, wright, wup, wdown;

    /* If there's a wall or door on either the left side or right
     * side (or both) of this secret door, make it be horizontal.
     *
     * It is feasible to put SDOOR in a corner, tee, or crosswall
     * position, although once the door is found and opened it won't
     * make a lot sense (diagonal access required).  Still, we try to
     * handle that as best as possible.  For top or bottom tee, using
     * horizontal is the best we can do.  For corner or crosswall,
     * either horizontal or vertical are just as good as each other;
     * we produce horizontal for corners and vertical for crosswalls.
     * For left or right tee, using vertical is best.
     *
     * A secret door with no adjacent walls is also feasible and makes
     * even less sense.  It will be displayed as a vertical wall while
     * hidden and become a vertical door when found.  Before resorting
     * to that, we check for solid rock which hasn't been wallified
     * yet (cf lower leftside of leader's room in Cav quest).
     */
    wleft  = (isok(x - 1, y) && (IS_WALL(levl[x - 1][y].typ)
                                 || IS_DOOR(levl[x - 1][y].typ)
                                 || levl[x - 1][y].typ == SDOOR));
    wright = (isok(x + 1, y) && (IS_WALL(levl[x + 1][y].typ)
                                 || IS_DOOR(levl[x + 1][y].typ)
                                 || levl[x + 1][y].typ == SDOOR));
    wup    = (isok(x, y - 1) && (IS_WALL(levl[x][y - 1].typ)
                                 || IS_DOOR(levl[x][y - 1].typ)
                                 || levl[x][y - 1].typ == SDOOR));
    wdown  = (isok(x, y + 1) && (IS_WALL(levl[x][y + 1].typ)
                                 || IS_DOOR(levl[x][y + 1].typ)
                                 || levl[x][y + 1].typ == SDOOR));
    if (!wleft && !wright && !wup && !wdown) {
        /* out of bounds is treated as implicit wall; should be academic
           because we don't expect to have doors so near the level's edge */
        wleft  = (!isok(x - 1, y) || IS_DOORJOIN(levl[x - 1][y].typ));
        wright = (!isok(x + 1, y) || IS_DOORJOIN(levl[x + 1][y].typ));
        wup    = (!isok(x, y - 1) || IS_DOORJOIN(levl[x][y - 1].typ));
        wdown  = (!isok(x, y + 1) || IS_DOORJOIN(levl[x][y + 1].typ));
    }
    levl[x][y].horizontal = ((wleft || wright) && !(wup && wdown)) ? 1 : 0;
}

STATIC_OVL void
maybe_add_door(x, y, droom)
int x, y;
struct mkroom *droom;
{
    if (droom->hx >= 0 && doorindex < DOORMAX && inside_room(droom, x, y))
        add_door(x, y, droom);
}

STATIC_OVL void
link_doors_rooms()
{
    int x, y;
    int tmpi, m;

    for (y = 0; y < ROWNO; y++)
        for (x = 0; x < COLNO; x++)
            if (IS_DOOR(levl[x][y].typ) || levl[x][y].typ == SDOOR) {
                /* in case this door was a '+' or 'S' from the
                   MAP...ENDMAP section without an explicit DOOR
                   directive, set/clear levl[][].horizontal for it */
                set_door_orientation(x, y);

                for (tmpi = 0; tmpi < nroom; tmpi++) {
                    maybe_add_door(x, y, &rooms[tmpi]);
                    for (m = 0; m < rooms[tmpi].nsubrooms; m++) {
                        maybe_add_door(x, y, rooms[tmpi].sbrooms[m]);
                    }
                }
            }
}

void
fill_rooms()
{
    int tmpi, m;

    for (tmpi = 0; tmpi < nroom; tmpi++) {
        if (rooms[tmpi].needfill)
            fill_room(&rooms[tmpi], (rooms[tmpi].needfill == 2));
        for (m = 0; m < rooms[tmpi].nsubrooms; m++)
            if (rooms[tmpi].sbrooms[m]->needfill)
                fill_room(rooms[tmpi].sbrooms[m], FALSE);
    }
}

/*
 * Choose randomly the state (nodoor, open, closed or locked) for a door
 */
STATIC_OVL int
rnddoor()
{
    int i = 1 << rn2(5);

    i >>= 1;
    return i;
}

/*
 * Select a random trap
 */
STATIC_OVL int
rndtrap()
{
    int rtrap;

    do {
        rtrap = rnd(TRAPNUM - 1);
        if (trap_type_definitions[rtrap].tdflags & TRAPDEF_FLAGS_NOT_GENERATED)
        {
            rtrap = NO_TRAP;
        }
        else
        {
            switch (rtrap) {
            case HOLE: /* no random holes on special levels */
                rtrap = NO_TRAP;
                break;
            case TRAPDOOR:
                if (!Can_dig_down(&u.uz))
                    rtrap = NO_TRAP;
                break;
            case LEVEL_TELEP:
            case TELEP_TRAP:
                if (level.flags.noteleport)
                    rtrap = NO_TRAP;
                break;
            case ROLLING_BOULDER_TRAP:
            case ROCKTRAP:
                if (In_endgame(&u.uz))
                    rtrap = NO_TRAP;
                break;
            }
        }
    } while (rtrap == NO_TRAP);
    return rtrap;
}

/*
 * Coordinates in special level files are handled specially:
 *
 *      if x or y is < 0, we generate a random coordinate.
 *      The "humidity" flag is used to insure that engravings aren't
 *      created underwater, or eels on dry land.
 */
STATIC_OVL void
get_location(x, y, humidity, croom)
schar *x, *y;
int humidity;
struct mkroom *croom;
{
    int cpt = 0;
    int mx, my, sx, sy;

    if (croom) {
        mx = croom->lx;
        my = croom->ly;
        sx = croom->hx - mx + 1;
        sy = croom->hy - my + 1;
    } else {
        mx = xstart;
        my = ystart;
        sx = xsize;
        sy = ysize;
    }

    if (*x >= 0) { /* normal locations */
        *x += mx;
        *y += my;
    } else { /* random location */
        do {
            if (croom) { /* handle irregular areas */
                coord tmpc;
                somexy(croom, &tmpc);
                *x = tmpc.x;
                *y = tmpc.y;
            } else {
                *x = mx + rn2((int) sx);
                *y = my + rn2((int) sy);
            }
            if (is_ok_location(*x, *y, humidity))
                break;
        } while (++cpt < 100);
        if (cpt >= 100) {
            register int xx, yy;

            /* last try */
            for (xx = 0; xx < sx; xx++)
                for (yy = 0; yy < sy; yy++) {
                    *x = mx + xx;
                    *y = my + yy;
                    if (is_ok_location(*x, *y, humidity))
                        goto found_it;
                }
            if (!(humidity & NO_LOC_WARN)) {
                impossible("get_location:  can't find a place!");
            } else {
                *x = *y = -1;
            }
        }
    }
found_it:
    ;

    if (!(humidity & ANY_LOC) && !isok(*x, *y)) {
        if (!(humidity & NO_LOC_WARN)) {
            /*warning("get_location:  (%d,%d) out of bounds", *x, *y);*/
            *x = x_maze_max;
            *y = y_maze_max;
        } else {
            *x = *y = -1;
        }
    }
}

STATIC_OVL boolean
is_ok_location(x, y, humidity)
register schar x, y;
register int humidity;
{
    if (Is_waterlevel(&u.uz))
        return TRUE; /* accept any spot */

    /* TODO: Should perhaps check if wall is diggable/passwall? */
    if (humidity & ANY_LOC)
        return TRUE;

    if ((humidity & SOLID) && IS_ROCK(levl[x][y].typ))
        return TRUE;
    if ((humidity & FLOORLOC) && IS_SOLID_FLOOR(levl[x][y].typ))
        return TRUE;
    if ((humidity & AIRLOC) && IS_AIR(levl[x][y].typ))
        return TRUE;
    if ((humidity & ICELOC) && levl[x][y].typ == ICE)
        return TRUE;
    if ((humidity & SPACELOC) && SPACE_POS(levl[x][y].typ))
        return TRUE;
    if ((humidity & WET) && is_pool(x, y))
        return TRUE;
    if ((humidity & HOT) && is_lava(x, y))
        return TRUE;
    return FALSE;
}

unpacked_coord
get_unpacked_coord(loc, defhumidity)
long loc;
int defhumidity;
{
    static unpacked_coord c;

    if (loc & SP_COORD_IS_RANDOM) {
        c.x = c.y = -1;
        c.is_random = 1;
        c.getloc_flags = (loc & ~SP_COORD_IS_RANDOM);
        if (!c.getloc_flags)
            c.getloc_flags = defhumidity;
    } else {
        c.is_random = 0;
        c.getloc_flags = defhumidity;
        c.x = SP_COORD_X(loc);
        c.y = SP_COORD_Y(loc);
    }
    return c;
}

STATIC_OVL void
get_location_coord(x, y, humidity, croom, crd)
schar *x, *y;
int humidity;
struct mkroom *croom;
long crd;
{
    unpacked_coord c;

    c = get_unpacked_coord(crd, humidity);
    *x = c.x;
    *y = c.y;
    get_location(x, y, (int)c.getloc_flags | (c.is_random ? NO_LOC_WARN : 0),
                 croom);
    if (*x == -1 && *y == -1 && c.is_random)
        get_location(x, y, humidity, croom);
}

/*
 * Get a relative position inside a room.
 * negative values for x or y means RANDOM!
 */

STATIC_OVL void
get_room_loc(x, y, croom)
schar *x, *y;
struct mkroom *croom;
{
    coord c;

    if (*x < 0 && *y < 0) 
    {
        if (somexy(croom, &c))
        {
            *x = c.x;
            *y = c.y;
        }
        else
        {
            panic("get_room_loc : can't find a place!");
            return;
        }
    } 
    else
    {
        if (*x < 0)
            *x = rn2(croom->hx - croom->lx + 1);
        if (*y < 0)
            *y = rn2(croom->hy - croom->ly + 1);
        *x += croom->lx;
        *y += croom->ly;
    }
}

/*
 * Get a relative position inside a room.
 * negative values for x or y means RANDOM!
 */
STATIC_OVL void
get_free_room_loc(x, y, croom, pos)
schar *x, *y;
struct mkroom *croom;
packed_coord pos;
{
    schar try_x, try_y;
    register int trycnt = 0;

    get_location_coord(&try_x, &try_y, DRY_NO_ICE, croom, pos);
    if (levl[try_x][try_y].typ != ROOM && levl[try_x][try_y].typ != GRASS && levl[try_x][try_y].typ != GROUND) {
        do {
            try_x = *x, try_y = *y;
            get_room_loc(&try_x, &try_y, croom);
        } while (levl[try_x][try_y].typ != ROOM && levl[try_x][try_y].typ != GRASS && levl[try_x][try_y].typ != GROUND && ++trycnt <= 100);

        if (trycnt > 100)
        {
            panic("get_free_room_loc:  can't find a place!");
            return;
        }
    }
    *x = try_x, *y = try_y;
}

boolean
check_room(lowx, ddx, lowy, ddy, vault)
xchar *lowx, *ddx, *lowy, *ddy;
boolean vault;
{
    register int x, y, hix = *lowx + *ddx, hiy = *lowy + *ddy;
    register struct rm *lev;
    int xlim, ylim, ymax;

    xlim = XLIM + (vault ? 1 : 0);
    ylim = YLIM + (vault ? 1 : 0);

    if (*lowx < 3)
        *lowx = 3;
    if (*lowy < 2)
        *lowy = 2;
    if (hix > COLNO - 3)
        hix = COLNO - 3;
    if (hiy > ROWNO - 3)
        hiy = ROWNO - 3;
chk:
    if (hix <= *lowx || hiy <= *lowy)
        return FALSE;

    /* check area around room (and make room smaller if necessary) */
    for (x = *lowx - xlim; x <= hix + xlim; x++) {
        if (x <= 0 || x >= COLNO)
            continue;
        y = *lowy - ylim;
        ymax = hiy + ylim;
        if (y < 0)
            y = 0;
        if (ymax >= ROWNO)
            ymax = (ROWNO - 1);
        lev = &levl[x][y];
        for (; y <= ymax; y++) {
            if (lev++->typ) {
                if (!vault) {
                    debugpline2("strange area [%d,%d] in check_room.", x, y);
                }
                if (!rn2(3))
                    return FALSE;
                if (x < *lowx)
                    *lowx = x + xlim + 1;
                else
                    hix = x - xlim - 1;
                if (y < *lowy)
                    *lowy = y + ylim + 1;
                else
                    hiy = y - ylim - 1;
                goto chk;
            }
        }
    }
    *ddx = hix - *lowx;
    *ddy = hiy - *lowy;
    return TRUE;
}

/*
 * Create a new room.
 * This is still very incomplete...
 */
boolean
create_room(x, y, w, h, xal, yal, rtype, rlit, floortyp, floorsubtyp, mtype, tileset, decotyp)
xchar x, y;
xchar w, h;
xchar xal, yal;
xchar rtype, rlit;
short decotyp;
int floortyp, floorsubtyp, mtype, tileset;
{
    xchar xabs = 0, yabs = 0;
    int wtmp, htmp, xaltmp, yaltmp, xtmp, ytmp;
    NhRect *r1 = 0, r2;
    int trycnt = 0;
    boolean vault = FALSE;
    int xlim = XLIM, ylim = YLIM;

    if (rtype == -1) /* Is the type random ? */
        rtype = OROOM;

    if (rtype == VAULT) {
        vault = TRUE;
        xlim++;
        ylim++;
    }

    /* on low levels the room is lit (usually) */
    /* some other rooms may require lighting */

    /* is light state random ? */
    if (rlit == -1)
        rlit = (rnd(1 + abs(depth(&u.uz))) < 11 && rn2(77)) ? TRUE : FALSE;

    /*
     * Here we will try to create a room. If some parameters are
     * random we are willing to make several try before we give
     * it up.
     */
    do {
        xchar xborder, yborder;
        wtmp = w;
        htmp = h;
        xtmp = x;
        ytmp = y;
        xaltmp = xal;
        yaltmp = yal;

        /* First case : a totally random room */

        if ((xtmp < 0 && ytmp < 0 && wtmp < 0 && xaltmp < 0 && yaltmp < 0)
            || vault) {
            xchar hx, hy, lx, ly, dx, dy;
            r1 = rnd_rect(); /* Get a random rectangle */

            if (!r1) { /* No more free rectangles ! */
                debugpline0("No more rects...");
                return FALSE;
            }
            hx = r1->hx;
            hy = r1->hy;
            lx = r1->lx;
            ly = r1->ly;
            if (vault)
                dx = dy = 1;
            else {
                dx = 2 + rn2((hx - lx > 28) ? 12 : 8);
                dy = 2 + rn2(4);
                if (dx * dy > 50)
                    dy = 50 / dx;
            }
            xborder = (lx > 0 && hx < COLNO - 1) ? 2 * xlim : xlim + 1;
            yborder = (ly > 0 && hy < ROWNO - 1) ? 2 * ylim : ylim + 1;
            if (hx - lx < dx + 3 + xborder || hy - ly < dy + 3 + yborder) {
                r1 = 0;
                continue;
            }
            xabs = lx + (lx > 0 ? xlim : 3)
                   + rn2(hx - (lx > 0 ? lx : 3) - dx - xborder + 1);
            yabs = ly + (ly > 0 ? ylim : 2)
                   + rn2(hy - (ly > 0 ? ly : 2) - dy - yborder + 1);
            if (ly == 0 && hy >= (ROWNO - 1) && (!nroom || !rn2(nroom))
                && (yabs + dy > ROWNO / 2)) {
                yabs = rn1(3, 2);
                if (nroom < 4 && dy > 1)
                    dy--;
            }
            if (!check_room(&xabs, &dx, &yabs, &dy, vault)) {
                r1 = 0;
                continue;
            }
            wtmp = dx + 1;
            htmp = dy + 1;
            r2.lx = xabs - 1;
            r2.ly = yabs - 1;
            r2.hx = xabs + wtmp;
            r2.hy = yabs + htmp;
        } else { /* Only some parameters are random */
            int rndpos = 0;
            if (xtmp < 0 && ytmp < 0) { /* Position is RANDOM */
                xtmp = rnd(5);
                ytmp = rnd(5);
                rndpos = 1;
            }
            if (wtmp < 0 || htmp < 0) { /* Size is RANDOM */
                wtmp = rn1(15, 3);
                htmp = rn1(8, 2);
            }
            if (xaltmp == -1) /* Horizontal alignment is RANDOM */
                xaltmp = rnd(3);
            if (yaltmp == -1) /* Vertical alignment is RANDOM */
                yaltmp = rnd(3);

            /* Try to generate real (absolute) coordinates here! */

            xabs = (((xtmp - 1) * COLNO) / 5) + 1;
            yabs = (((ytmp - 1) * ROWNO) / 5) + 1;
            switch (xaltmp) {
            case LEFT:
                break;
            case RIGHT:
                xabs += (COLNO / 5) - wtmp;
                break;
            case CENTER:
                xabs += ((COLNO / 5) - wtmp) / 2;
                break;
            }
            switch (yaltmp) {
            case TOP:
                break;
            case BOTTOM:
                yabs += (ROWNO / 5) - htmp;
                break;
            case CENTER:
                yabs += ((ROWNO / 5) - htmp) / 2;
                break;
            }

            if (xabs + wtmp - 1 > COLNO - 2)
                xabs = COLNO - wtmp - 3;
            if (xabs < 2)
                xabs = 2;
            if (yabs + htmp - 1 > ROWNO - 2)
                yabs = ROWNO - htmp - 3;
            if (yabs < 2)
                yabs = 2;

            /* Try to find a rectangle that fit our room ! */

            r2.lx = xabs - 1;
            r2.ly = yabs - 1;
            r2.hx = xabs + wtmp + rndpos;
            r2.hy = yabs + htmp + rndpos;
            r1 = get_rect(&r2);
        }
    } while (++trycnt <= 100 && !r1);
    if (!r1) { /* creation of room failed ? */
        return FALSE;
    }
    split_rects(r1, &r2);

    if (!vault) {
        smeq[nroom] = nroom;
        add_room(xabs, yabs, xabs + wtmp - 1, yabs + htmp - 1, rlit, rtype,
                 FALSE, IS_FLOOR(floortyp) ? floortyp : 0, IS_FLOOR(floortyp) ? floorsubtyp : 0, mtype, tileset, decotyp);
    } else {
        rooms[nroom].lx = xabs;
        rooms[nroom].ly = yabs;
    }
    return TRUE;
}

/*
 * Create a subroom in room proom at pos x,y with width w & height h.
 * x & y are relative to the parent room.
 */
STATIC_OVL boolean
create_subroom(proom, x, y, w, h, rtype, rlit, floortyp, floorsubtyp, mtype, tileset, decotyp)
struct mkroom *proom;
xchar x, y;
xchar w, h;
xchar rtype, rlit;
short decotyp;
int floortyp, floorsubtyp, mtype, tileset;
{
    xchar width, height;

    width = proom->hx - proom->lx + 1;
    height = proom->hy - proom->ly + 1;

    /* There is a minimum size for the parent room */
    if (width < 4 || height < 4)
        return FALSE;

    /* Check for random position, size, etc... */

    if (w == -1)
        w = rnd(width - 3);
    if (h == -1)
        h = rnd(height - 3);
    if (x == -1)
        x = rnd(width - w - 1) - 1;
    if (y == -1)
        y = rnd(height - h - 1) - 1;
    if (x == 1)
        x = 0;
    if (y == 1)
        y = 0;
    if ((x + w + 1) == width)
        x++;
    if ((y + h + 1) == height)
        y++;
    if (rtype == -1)
        rtype = OROOM;
    if (rlit == -1)
        rlit = (rnd(1 + abs(depth(&u.uz))) < 11 && rn2(77)) ? TRUE : FALSE;
    add_subroom(proom, proom->lx + x, proom->ly + y, proom->lx + x + w - 1,
                proom->ly + y + h - 1, rlit, rtype, FALSE, floortyp, floorsubtyp, mtype, tileset, decotyp);
    return TRUE;
}

/*
 * Create a new door in a room.
 * It's placed on a wall (north, south, east or west).
 */
STATIC_OVL void
create_door(dd, broom)
room_door *dd;
struct mkroom *broom;
{
    int x = 0, y = 0;
    int trycnt = 0, wtry = 0;

    if (dd->secret == -1)
        dd->secret = rn2(2);

    if (dd->mask == -1) {
        /* is it a locked door, closed, or a doorway? */
        if (!dd->secret) {
            if (!rn2(3)) {
                if (!rn2(5))
                    dd->mask = D_ISOPEN;
                else if (!rn2(6))
                    dd->mask = D_LOCKED;
                else
                    dd->mask = D_CLOSED;
                if (dd->mask != D_ISOPEN && !rn2(25))
                    dd->mask |= D_TRAPPED;
            } else
                dd->mask = D_NODOOR;
        } else {
            if (!rn2(5))
                dd->mask = D_LOCKED;
            else
                dd->mask = D_CLOSED;

            if (!rn2(20))
                dd->mask |= D_TRAPPED;
        }
    }

    do {
        register int dwall, dpos;

        dwall = dd->wall;
        if (dwall == -1) /* The wall is RANDOM */
            dwall = 1 << rn2(4);

        dpos = dd->pos;

        /* Convert wall and pos into an absolute coordinate! */
        wtry = rn2(4);
        switch (wtry) {
        case 0:
            if (!(dwall & W_NORTH))
                goto redoloop;
            y = broom->ly - 1;
            x = broom->lx
                + ((dpos == -1) ? rn2(1 + (broom->hx - broom->lx)) : dpos);
            if (IS_ROCK(levl[x][y - 1].typ))
                goto redoloop;
            goto outdirloop;
        case 1:
            if (!(dwall & W_SOUTH))
                goto redoloop;
            y = broom->hy + 1;
            x = broom->lx
                + ((dpos == -1) ? rn2(1 + (broom->hx - broom->lx)) : dpos);
            if (IS_ROCK(levl[x][y + 1].typ))
                goto redoloop;
            goto outdirloop;
        case 2:
            if (!(dwall & W_WEST))
                goto redoloop;
            x = broom->lx - 1;
            y = broom->ly
                + ((dpos == -1) ? rn2(1 + (broom->hy - broom->ly)) : dpos);
            if (IS_ROCK(levl[x - 1][y].typ))
                goto redoloop;
            goto outdirloop;
        case 3:
            if (!(dwall & W_EAST))
                goto redoloop;
            x = broom->hx + 1;
            y = broom->ly
                + ((dpos == -1) ? rn2(1 + (broom->hy - broom->ly)) : dpos);
            if (IS_ROCK(levl[x + 1][y].typ))
                goto redoloop;
            goto outdirloop;
        default:
            x = y = 0;
            panic("create_door: No wall for door!");
            return;
            //goto outdirloop;
        }
    outdirloop:
        if (okdoor(x, y))
            break;
    redoloop:
        ;
    } while (++trycnt <= 100);
    if (trycnt > 100) {
        impossible("create_door: Can't find a proper place!");
        return;
    }
    int typ = (dd->secret ? SDOOR : DOOR);

    unsigned short dflags = dd->mask; /* Normal doormask */
    dflags |= dd->dflags;

    set_initial_location_type_at(x, y, typ);
    levl[x][y].doormask = dflags;
    levl[x][y].key_otyp = dd->key_otyp;
    levl[x][y].special_quality = dd->key_special_quality;

    /* Clear off decorations */
    levl[x][y].decoration_typ = 0;
    levl[x][y].decoration_subtyp = 0;
    levl[x][y].decoration_dir = 0;
    levl[x][y].decoration_flags = 0;
}

/*
 * Create a secret door in croom on any one of the specified walls.
 */
void
create_secret_door(croom, walls)
struct mkroom *croom;
xchar walls; /* any of W_NORTH | W_SOUTH | W_EAST | W_WEST (or W_ANY) */
{
    xchar sx, sy; /* location of the secret door */
    int count;

    for (count = 0; count < 100; count++) {
        sx = rn1(croom->hx - croom->lx + 1, croom->lx);
        sy = rn1(croom->hy - croom->ly + 1, croom->ly);

        switch (rn2(4)) {
        case 0: /* top */
            if (!(walls & W_NORTH))
                continue;
            sy = croom->ly - 1;
            break;
        case 1: /* bottom */
            if (!(walls & W_SOUTH))
                continue;
            sy = croom->hy + 1;
            break;
        case 2: /* left */
            if (!(walls & W_EAST))
                continue;
            sx = croom->lx - 1;
            break;
        case 3: /* right */
            if (!(walls & W_WEST))
                continue;
            sx = croom->hx + 1;
            break;
        }

        if (okdoor(sx, sy)) 
        {
            set_initial_location_type_at(sx, sy, SDOOR);
            levl[sx][sy].doormask = D_CLOSED;
            levl[sx][sy].key_otyp = 0;
            levl[sx][sy].special_quality = 0;
            return;
        }
    }

    impossible("couldn't create secret door on any walls 0x%x", walls);
}

/*
 * Create a trap in a room.
 */
STATIC_OVL void
create_trap(t, croom)
spltrap *t;
struct mkroom *croom;
{
    schar x = -1, y = -1;
    coord tm;

    if (croom)
        get_free_room_loc(&x, &y, croom, t->coord);
    else {
        int trycnt = 0;
        do {
            get_location_coord(&x, &y, DRY_NO_ICE, croom, t->coord);
        } while ((levl[x][y].typ == STAIRS || levl[x][y].typ == LADDER)
                 && ++trycnt <= 100);
        if (trycnt > 100)
            return;
    }

    tm.x = x;
    tm.y = y;

    (void)mktrap(t->type, 1, (struct mkroom *) 0, &tm);
}

void
create_carpet(x1, y1, x2, y2, type)
xchar x1, y1, x2, y2;
int type;
{
    if (!isok(x1, y1) || !isok(x2, y2) || type < 0 || type >= MAX_CARPETS)
        return;

    int x, y;
    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)
        {
            levl[x][y].carpet_typ = (schar)type;
            int piece = 0;
            switch (carpet_type_definitions[type].tile_indexation_type)
            {
            default:
            case CARPET_TILE_INDEXATION_TYPE_LONG_CARPET:
                if (x == x1 && y == y1)
                    piece = CARPET_PIECE_LONG_TLCORN;
                else if (x == x1 && y == y2)
                    piece = CARPET_PIECE_LONG_BLCORN;
                else if (x == x2 && y == y1)
                    piece = CARPET_PIECE_LONG_TRCORN;
                else if (x == x2 && y == y2)
                    piece = CARPET_PIECE_LONG_BRCORN;
                else if (x == x1)
                    piece = CARPET_PIECE_LONG_LEFT;
                else if (x == x2)
                    piece = CARPET_PIECE_LONG_RIGHT;
                else if (y == y1)
                    piece = CARPET_PIECE_LONG_TOP;
                else if (y == y2)
                    piece = CARPET_PIECE_LONG_BOTTOM;
                else
                    piece = CARPET_PIECE_LONG_MIDDLE;
                break;
            case CARPET_TILE_INDEXATION_TYPE_3X2_CARPET:
                if (x == x1 && y == y1)
                    piece = CARPET_PIECE_3X2_TLCORN;
                else if (x == x1 && y == y2)
                    piece = CARPET_PIECE_3X2_BLCORN;
                else if (x == x2 && y == y1)
                    piece = CARPET_PIECE_3X2_TRCORN;
                else if (x == x2 && y == y2)
                    piece = CARPET_PIECE_3X2_BRCORN;
                else if (y == y1)
                    piece = CARPET_PIECE_3X2_TOP;
                else if (y == y2)
                    piece = CARPET_PIECE_3X2_BOTTOM;
                else
                    piece = CARPET_PIECE_3X2_BOTTOM; /* should not happen */
                break;
            }
            levl[x][y].carpet_piece = (schar)piece;
            levl[x][y].carpet_flags = 0;
        }
    }
}

/*
 * Create a monster in a room.
 */
STATIC_OVL int
noncoalignment(alignment)
aligntyp alignment;
{
    int k;

    k = rn2(2);
    if (!alignment)
        return (k ? -1 : 1);
    return (k ? -alignment : 0);
}

/* attempt to screen out locations where a mimic-as-boulder shouldn't occur */
STATIC_OVL boolean
m_bad_boulder_spot(x, y)
int x, y;
{
    struct rm *lev;

    /* avoid trap locations */
    if (t_at(x, y))
        return TRUE;
    /* try to avoid locations which already have a boulder (this won't
       actually work; we get called before objects have been placed...) */
    if (sobj_at(BOULDER, x, y))
        return TRUE;
    /* avoid closed doors */
    lev = &levl[x][y];
    if (IS_DOOR(lev->typ) && (lev->doormask & (D_CLOSED | D_LOCKED)) != 0)
        return TRUE;
    /* spot is ok */
    return FALSE;
}

STATIC_OVL int
pm_to_humidity(pm)
struct permonst *pm;
{
    int loc = DRY;
    if (!pm || is_mimic(pm))
        return loc;
    if (pm->mlet == S_EEL || amphibious(pm) || is_swimmer(pm))
        loc = WET;
    if (is_flyer(pm) || is_floater(pm))
        loc |= (HOT | WET);
    if (passes_walls(pm) || is_incorporeal(pm))
        loc |= SOLID;
    if (likes_lava(pm))
        loc |= HOT;
    return loc;
}

STATIC_OVL void
create_monster(m, croom)
monster *m;
struct mkroom *croom;
{
    struct monst *mtmp;
    schar x = -1, y = -1;
    char class;
    aligntyp amask;
    coord cc;
    struct permonst *pm;
    unsigned g_mvflags;

    if (m->class >= 0)
        class = (char) def_char_to_monclass((char) m->class);
    else
        class = 0;

    if (class == MAX_MONSTER_CLASSES)
        panic("create_monster: unknown monster class '%c'", m->class);

    amask = (m->align == AM_SPLEV_CO)
               ? Align2amask(u.ualignbase[A_ORIGINAL])
               : (m->align == AM_SPLEV_NONCO)
                  ? Align2amask(noncoalignment(u.ualignbase[A_ORIGINAL]))
                  : (m->align <= -(MAX_REGISTERS + 1))
                     ? induced_align(80)
                     : (m->align < 0 ? ralign[-m->align - 1] : m->align);

    if (!class)
        pm = (struct permonst *) 0;
    else if (m->id != NON_PM) 
    {
        pm = &mons[m->id];
        g_mvflags = (unsigned) mvitals[m->id].mvflags;
        if ((pm->geno & G_UNIQ) && (g_mvflags & MV_EXTINCT))
            return;
        else if (g_mvflags & MV_GONE)    /* genocided or extinct */
            pm = (struct permonst *) 0; /* make random monster */
    }
    else 
    {
        pm = mkclass(class, 0UL); /* Removed G_NOGEN; did not seem to make a whole lot of sense here. --JG */
        /* if we can't get a specific monster type (pm == 0) then the
           class has been genocided, so settle for a random monster */
    }

    if (In_mines(&u.uz) && pm && your_race(pm)
        && (Race_if(PM_DWARF) || Race_if(PM_GNOME)) && rn2(3))
        pm = (struct permonst *) 0;

    if (pm) 
    {
        int loc = pm_to_humidity(pm);
        /* If water-liking monster, first try is without DRY */
        get_location_coord(&x, &y, loc | NO_LOC_WARN, croom, m->coord);
        if (x == -1 && y == -1)
        {
            loc |= DRY;
            get_location_coord(&x, &y, loc, croom, m->coord);
        }
    }
    else 
    {
        get_location_coord(&x, &y, DRY, croom, m->coord);
    }

    /* try to find a close place if someone else is already there */
    if (MON_AT(x, y) && enexto(&cc, x, y, pm))
        x = cc.x, y = cc.y;

    unsigned long mmflags = 0UL;
    if (m->maxhp)
        mmflags |= MM_MAX_HP;

    if (m->align != -(MAX_REGISTERS + 2))
        mtmp = mk_roamer(pm, Amask2align(amask), x, y, m->peaceful);
    else if (PM_ARCHAEOLOGIST <= m->id && m->id <= PM_WIZARD)
        mtmp = mk_mplayer(pm, x, y, FALSE);
    else
        mtmp = makemon_ex(pm, x, y, mmflags, 0UL, 0, 0, (int)m->level_adjustment);

    if (mtmp)
    {
        x = mtmp->mx, y = mtmp->my; /* sanity precaution */
        m->x = x, m->y = y;
        /* handle specific attributes for some special monsters */
        if (m->name.str)
        {
            mtmp = christen_monst(mtmp, m->name.str);
            mtmp->u_know_mname = 1; /* On special levels you know all generated monsters -- maybe should be specified later */
        }

        mtmp->mon_flags |= MON_FLAGS_SPLEVEL_RESIDENT; /* Created specifically for the special level; don't remove in bones if staying on the level */

        /*
         * This doesn't complain if an attempt is made to give a
         * non-mimic/non-shapechanger an appearance or to give a
         * shapechanger a non-monster shape, it just refuses to comply.
         */
        if (m->appear_as.str
            && ((is_mimic(mtmp->data))
                /* shapechanger (chameleons, et al, and vampires) */
                || (mtmp->cham >= LOW_PM && m->appear == M_AP_MONSTER))
            && !Protection_from_shape_changers) 
        {
            int i;

            switch (m->appear)
            {
            case M_AP_NOTHING:
                impossible(
                 "create_monster: mon has an appearance, \"%s\", but no type",
                           m->appear_as.str);
                break;

            case M_AP_FURNITURE:
                for (i = 0; i < MAX_CMAPPED_CHARS; i++)
                    if (!strcmp(defsyms[i].explanation, m->appear_as.str))
                        break;

                if (i == MAX_CMAPPED_CHARS)
                {
                    impossible("create_monster: can't find feature \"%s\"",
                               m->appear_as.str);
                }
                else 
                {
                    mtmp->m_ap_type = M_AP_FURNITURE;
                    mtmp->mappearance = i;
                }
                break;

            case M_AP_OBJECT:
                for (i = 0; i < NUM_OBJECTS; i++)
                    if (OBJ_NAME(objects[i])
                        && !strcmp(OBJ_NAME(objects[i]), m->appear_as.str))
                        break;
                if (i == NUM_OBJECTS)
                {
                    impossible("create_monster: can't find object \"%s\"",
                               m->appear_as.str);
                } 
                else
                {
                    mtmp->m_ap_type = M_AP_OBJECT;
                    mtmp->mappearance = i;
                    struct obj* otmp = mksobj(i, TRUE, FALSE, 0);
                    if (otmp)
                    {
                        if (has_mobj(mtmp))
                            free_mobj(mtmp);
                        if (!has_mobj(mtmp))
                            newmobj(mtmp);
                        if (has_mobj(mtmp))
                        {
                            *MOBJ(mtmp) = *otmp;
                            MOBJ(mtmp)->oextra = 0;
                            MOBJ(mtmp)->nobj = 0;
                            MOBJ(mtmp)->nexthere = 0;
                            MOBJ(mtmp)->cobj = 0;
                            MOBJ(mtmp)->o_id = context.ident++;
                            if (!MOBJ(mtmp)->o_id) /* ident overflowed */
                                MOBJ(mtmp)->o_id = context.ident++;
                            if (otmp->oextra)
                                copy_oextra(MOBJ(mtmp), otmp);
                        }
                        /* make sure container contents are free'ed */
                        obfree(otmp, (struct obj*)0);
                    }

                    /* try to avoid placing mimic boulder on a trap */
                    if (i == BOULDER && m->x < 0
                        && m_bad_boulder_spot(x, y))
                    {
                        int retrylimit = 10;

                        remove_monster(x, y);
                        do
                        {
                            x = m->x;
                            y = m->y;
                            get_location(&x, &y, DRY, croom);
                            if (MON_AT(x, y) && enexto(&cc, x, y, pm))
                                x = cc.x, y = cc.y;
                        }
                        while (m_bad_boulder_spot(x, y)
                                 && --retrylimit > 0);

                        place_monster(mtmp, x, y);
                        /* if we didn't find a good spot
                           then mimic something else */
                        if (!retrylimit)
                            set_mimic_sym(mtmp);
                    }
                }
                break;

            case M_AP_MONSTER:
            {
                int mndx;

                if (!strcmpi(m->appear_as.str, "random"))
                    mndx = select_newcham_form(mtmp);
                else
                    mndx = name_to_mon(m->appear_as.str);

                if (mndx == NON_PM || (is_vampshifter(mtmp)
                                       && !validvamp(mtmp, &mndx, S_HUMAN)))
                {
                    impossible("create_monster: invalid %s (\"%s\")",
                               (is_mimic(mtmp->data))
                                 ? "mimic appearance"
                                 : (mtmp->data == &mons[PM_WIZARD_OF_YENDOR])
                                     ? "Wizard appearance"
                                     : is_vampshifter(mtmp)
                                         ? "vampire shape"
                                         : "chameleon shape",
                               m->appear_as.str);
                } 
                else if (&mons[mndx] == mtmp->data)
                {
                    /* explicitly forcing a mimic to appear as itself */
                    mtmp->m_ap_type = M_AP_NOTHING;
                    mtmp->mappearance = 0;
                } 
                else if (is_mimic(mtmp->data)
                           || mtmp->data == &mons[PM_WIZARD_OF_YENDOR])
                {
                    /* this is ordinarily only used for Wizard clones
                       and hasn't been exhaustively tested for mimics */
                    mtmp->m_ap_type = M_AP_MONSTER;
                    mtmp->mappearance = mndx;
                }
                else 
                { /* chameleon or vampire */
                    struct permonst *mdat = &mons[mndx];
                    struct permonst *olddata = mtmp->data;

                    mgender_from_permonst(mtmp, mdat);
                    set_mon_data(mtmp, mdat, 0);

                    if (emitted_light_range(olddata) != emitted_light_range(mtmp->data))
                    {
                        /* used to give light, now doesn't, or vice versa,
                           or light's range has changed */
                        if (emitted_light_range(olddata))
                            del_light_source(LS_MONSTER, (genericptr_t) mtmp);
                        if (emitted_light_range(mtmp->data))
                            new_light_source(mtmp->mx, mtmp->my,
                                             emitted_light_range(mtmp->data),
                                             LS_MONSTER, (genericptr_t) mtmp, 0);
                    }

                    if (mon_ambient_sound(olddata) != mon_ambient_sound(mtmp->data))
                    {
                        /* used to give light, now doesn't, or vice versa,
                           or light's range has changed */
                        if (mon_ambient_sound(olddata))
                            del_sound_source(SOUNDSOURCE_MONSTER, (genericptr_t)mtmp);
                        if (mon_ambient_sound(mtmp->data))
                            new_sound_source(mtmp->mx, mtmp->my,
                                mon_ambient_sound(mtmp->data), mon_ambient_volume(mtmp->data),
                                SOUNDSOURCE_MONSTER, mon_ambient_subtype(mtmp->data), (genericptr_t)mtmp);
                    }
                }
                break;
            }
            default:
                impossible(
                  "create_monster: unimplemented mon appear type [%d,\"%s\"]",
                           m->appear, m->appear_as.str);
                break;
            }
            if (does_block(x, y, &levl[x][y]))
                block_vision_and_hearing_at_point(x, y);
        }

        if (m->peaceful >= 0)
        {
            mtmp->mpeaceful = m->peaceful;
            /* changed mpeaceful again; have to reset mhostility */
            set_mhostility(mtmp);
        }

        if (m->asleep >= 0) 
        {
#ifdef UNIXPC
            /* optimizer bug strikes again */
            if (m->asleep)
                mtmp->msleeping = 1;
            else
                mtmp->msleeping = 0;
#else
            mtmp->msleeping = m->asleep;
#endif
        }

        if (m->seentraps)
            mtmp->mtrapseen = m->seentraps;
        if (m->female >= 0)
            mtmp->female = (unsigned int)m->female;
        if (m->waitforu)
            mtmp->mstrategy |= STRAT_WAITFORU;
        if (m->protector)
            mtmp->isprotector = 1;

        if (m->cancelled)
        {
            mtmp->mprops[CANCELLED] = m->cancelled;
        }
        if (m->revived)
            mtmp->mrevived = (short)m->revived;
        if (m->avenge)
            mtmp->mavenge = 1;
        if (m->stunned)
            mtmp->mprops[STUNNED] |= M_INTRINSIC_ACQUIRED;
        if (m->confused)
            mtmp->mprops[CONFUSION] |= M_INTRINSIC_ACQUIRED;
        if (m->invis) 
        {
            mtmp->mprops[INVISIBILITY] |= M_INTRINSIC_ACQUIRED;
        }
        if (m->blinded) 
        {
            mtmp->mprops[BLINDED] |= (m->blinded & M_TIMEOUT);
        }
        if (m->paralyzed)
        {
            mtmp->mprops[PARALYZED] |= (m->paralyzed & M_TIMEOUT);
        }
        if (m->fleeing) 
        {
            /* Assume non-magical fleeing */
            mtmp->mflee = 1;
            mtmp->mflee_timer = m->fleeing;
        }

        if (m->has_invent) {
            if(!m->keep_original_invent)
                discard_minvent(mtmp);
            invent_carrying_monster = mtmp;
        }
    }
}

/*
 * Create an object in a room.
 */
STATIC_OVL void
create_object(o, croom)
object *o;
struct mkroom *croom;
{
    struct obj *otmp = 0;
    schar x, y;
    char c;
    boolean named; /* has a name been supplied in level description? */
    Sprintf(debug_buf_2, "create_object, oclass=%d, id=%d", o->class, o->id);

    named = o->name.str ? TRUE : FALSE;

    get_location_coord(&x, &y, DRY, croom, o->coord);

    if (o->class >= 0)
        c = o->class;
    else
        c = 0;

    struct monst* mowner = 0;
    if ((o->containment & SP_OBJ_CONTENT) != 0 && !container_idx && invent_carrying_monster) 
    {
        mowner = invent_carrying_monster;
    }

    if (o->class == OBJECT_SPECIAL_CREATE_TYPE_CLASS_TREASURE_ARMOR)
    {
        /* class armor treasure */
        if (Role_if(PM_WIZARD))
            otmp = mksobj_at(!rn2(5) ? BRACERS_OF_REFLECTION : !rn2(2) ? BRACERS_OF_DEFENSE : ROBE_OF_PROTECTION, x, y, TRUE, !named);
        else if (Role_if(PM_MONK))
            otmp = mksobj_at(!rn2(5) ? BRACERS_OF_REFLECTION : !rn2(2) ? ROBE_OF_PROTECTION : !rn2(2) ? ROBE_OF_MAGIC_RESISTANCE : CLOAK_OF_MAGIC_RESISTANCE, x, y, TRUE, !named);
        else
            otmp = mksobj_at_with_flags(FULL_PLATE_MAIL, x, y, TRUE, !named, -1, mowner, !rn2(2) ? MAT_MITHRIL : MAT_ADAMANTIUM, 0L, 0L, 0UL);
    }
    else if (o->class == OBJECT_SPECIAL_CREATE_TYPE_CLASS_TREASURE_WEAPON)
    {
        /* class weapon treasure */
        if (Role_if(PM_WIZARD))
        {
            if (!rn2(2))
            {
                otmp = mksobj_at(!rn2(2) ? DAGGER : ATHAME, x, y, TRUE, !named);
                if (!otmp->oartifact)
                {
                    if (can_obj_have_mythic(otmp))
                    {
                        if (!otmp->mythic_prefix && (!rn2(2) || otmp->mythic_suffix))
                        {
                            if (!rn2(2))
                                otmp->mythic_prefix = MYTHIC_PREFIX_VAMPIRIC;
                            else
                                otmp->mythic_prefix = MYTHIC_PREFIX_STYGIAN;
                        }
                        else
                        {
                            if (!rn2(2))
                                otmp->mythic_suffix = MYTHIC_SUFFIX_SHARPNESS;
                            else
                                otmp->mythic_suffix = MYTHIC_SUFFIX_SPEED;
                        }
                    }
                }
                otmp->enchantment += 1;
            }
            else
            {
                otmp = mksobj_at(!rn2(2) ? STAFF_OF_FROST : STAFF_OF_THE_MAGI, x, y, TRUE, !named);
            }
            otmp->enchantment += 2;
            if (can_have_exceptionality(otmp) && otmp->exceptionality < EXCEPTIONALITY_EXCEPTIONAL)
                otmp->exceptionality = EXCEPTIONALITY_EXCEPTIONAL;
        }
        else if (Role_if(PM_MONK))
            otmp = mksobj_at(!rn2(3) ? BELT_OF_FIRE_GIANT_STRENGTH : !rn2(2) ? GAUNTLETS_OF_OGRE_POWER : GLOVES_OF_HASTE, x, y, TRUE, !named);
        else if (Role_if(PM_PRIEST))
        {
            otmp = mksobj_at(Race_if(PM_GNOLL) && rn2(2) ? (!rn2(4) ? DOUBLE_HEADED_FLAIL : FLAIL) : !rn2(2) ? MACE : !rn2(2) ? MORNING_STAR : WAR_HAMMER, x, y, TRUE, !named);
            if (!otmp->oartifact)
            {
                if (can_obj_have_mythic(otmp))
                {
                    if (u.ualign.type == A_CHAOTIC)
                    {
                        if (!otmp->mythic_prefix && (rn2(3) || otmp->mythic_suffix))
                        {
                            if (!rn2(2))
                                otmp->mythic_prefix = MYTHIC_PREFIX_VAMPIRIC;
                            else
                                otmp->mythic_prefix = MYTHIC_PREFIX_STYGIAN;
                        }
                        else
                        {
                            otmp->mythic_suffix = !rn2(2) ? MYTHIC_SUFFIX_FREE_ACTION : MYTHIC_SUFFIX_SPEED;
                        }
                    }
                    else
                    {
                        if (!otmp->mythic_suffix && (rn2(2) || otmp->mythic_prefix))
                        {
                            if (!rn2(3))
                                otmp->mythic_suffix = MYTHIC_SUFFIX_BANISHMENT;
                            else if (!rn2(2))
                                otmp->mythic_suffix = MYTHIC_SUFFIX_DISRUPTION;
                            else
                                otmp->mythic_suffix = MYTHIC_SUFFIX_SPEED;
                        }
                        else
                        {
                            otmp->mythic_prefix = MYTHIC_PREFIX_OLYMPIAN;
                        }
                    }
                }
                otmp->enchantment += 2;
                if (can_have_exceptionality(otmp) && otmp->exceptionality < EXCEPTIONALITY_EXCEPTIONAL)
                    otmp->exceptionality = EXCEPTIONALITY_EXCEPTIONAL;
            }
        }
        else
        {
            int treasuretyp = Race_if(PM_GNOLL) && !P_RESTRICTED(P_FLAIL) && !rn2(2) ? (!rn2(4) ? DOUBLE_HEADED_FLAIL : FLAIL) :
                P_RESTRICTED(P_SWORD) ? (!P_RESTRICTED(P_AXE) && !rn2(2) ? (!rn2(2) ? AXE : BATTLE_AXE) : !P_RESTRICTED(P_BLUDGEONING_WEAPON) && !rn2(2) ? (!rn2(4) ? MACE : WAR_HAMMER) : !P_RESTRICTED(P_DAGGER) && !rn2(2) ? (!rn2(3) ? DAGGER : ATHAME) : QUARTERSTAFF) :
                !rn2(3) ? LONG_SWORD : TWO_HANDED_SWORD;
            otmp = mksobj_at(treasuretyp, x, y, TRUE, !named);
            if (!otmp->oartifact)
            {
                if (can_obj_have_mythic(otmp))
                {
                    if (!otmp->mythic_prefix && (!rn2(2) || otmp->mythic_suffix))
                    {
                        if (!rn2(2))
                            otmp->mythic_prefix = MYTHIC_PREFIX_VAMPIRIC;
                        else
                            otmp->mythic_prefix = MYTHIC_PREFIX_STYGIAN;
                    }
                    else
                    {
                        if (!rn2(2))
                            otmp->mythic_suffix = objects[otmp->otyp].oc_dir == WHACK || !rn2(2) ? MYTHIC_SUFFIX_FREE_ACTION : MYTHIC_SUFFIX_SHARPNESS;
                        else
                            otmp->mythic_suffix = MYTHIC_SUFFIX_SPEED;
                    }
                }
                otmp->enchantment += 2;
                if (can_have_exceptionality(otmp) && otmp->exceptionality < EXCEPTIONALITY_EXCEPTIONAL)
                    otmp->exceptionality = EXCEPTIONALITY_EXCEPTIONAL;
            }
        }
    }
    else if (o->class == OBJECT_SPECIAL_CREATE_TYPE_ARTIFACT)
    {
        otmp = mk_artifact((struct obj*)0, (aligntyp)A_NONE, MKARTIFACT_FLAGS_NONE);
        if(otmp)
            place_object(otmp, x, y);
        else
            otmp = mkobj_at(RANDOM_CLASS, x, y, !named);
    }
    else if (o->class == OBJECT_SPECIAL_CREATE_TYPE_CLASS_ARTIFACT)
    {
        otmp = mk_artifact((struct obj*)0, (aligntyp)A_NONE, Role_if(PM_MONK) ? MKARTIFACT_FLAGS_NONWEAPONS_ONLY : MKARTIFACT_FLAGS_NONE);
        if (otmp)
            place_object(otmp, x, y);
        else
        {
            otmp = mksobj_at(get_artifact_replacement_item_otyp(), x, y, TRUE, TRUE);
            if (!otmp)
                otmp = mkobj_at(RANDOM_CLASS, x, y, !named);
        }
    }
    else if (o->class == OBJECT_SPECIAL_CREATE_TYPE_ORC_ARTIFACT)
    {
        if (!exist_artifact(ORNAMENTAL_ORCISH_DAGGER, artiname(ART_GRIMTOOTH)) && rn2(3))
        {
            otmp = mksobj(ORNAMENTAL_ORCISH_DAGGER, FALSE, FALSE, FALSE);
            otmp = oname(otmp, artiname(ART_GRIMTOOTH));
            otmp->enchantment = 3 + rnd(4);
        }
        else
        {
            otmp = mksobj(ORCISH_DAGGER, FALSE, FALSE, FALSE);
            otmp->enchantment = 2 + rnd(3);
            otmp->mythic_prefix = MYTHIC_PREFIX_VAMPIRIC;
            otmp->exceptionality = EXCEPTIONALITY_ELITE;
        }

        if (otmp)
            place_object(otmp, x, y);
        else
        {
            otmp = mkobj_at(RANDOM_CLASS, x, y, !named);
        }
    }
    else if (o->class == OBJECT_SPECIAL_CREATE_TYPE_RANDOM_CONTENTS)
    {
        otmp = mkobj(RANDOM_CLASS, !named, TRUE);
        if (otmp)
            place_object(otmp, x, y);
        else
            otmp = mkobj_at(RANDOM_CLASS, x, y, !named);
    }
    else if (!c)
        otmp = mkobj_at(RANDOM_CLASS, x, y, !named);
    else if (o->id != -1)
    {
        unsigned long mkflags = o->open ? MKOBJ_FLAGS_OPEN_COFFIN : 0UL;
        mkflags |= o->corpsenm > NON_PM ? MKOBJ_FLAGS_MONSTER_SPECIFIED : 0UL;
        
        otmp = mksobj_at_with_flags(o->id, x, y, TRUE, !named, -1, mowner, MAT_NONE, 0L, 0L, mkflags);
    }
    else 
    {
        /*
         * The special levels are compiled with the default "text" object
         * class characters.  We must convert them to the internal format.
         */
        char oclass = (char) def_char_to_objclass(c);

        if (oclass == MAX_OBJECT_CLASSES)
        {
            panic("create_object:  unexpected object class '%c'", c);
            return;
        }
        /* KMH -- Create piles of gold properly */
        if (oclass == COIN_CLASS)
            otmp = mkgold(0L, x, y);
        else
            otmp = mkobj_at(oclass, x, y, !named);
    }

    if (!otmp)
        return;

    /* Set these first, and then everything else that might affect speflags */
    if (o->speflags > 0)
        otmp->speflags = o->speflags;

    if (o->enchantment != -127) /* That means NOT RANDOM! */
    {
        if(otmp->oclass == WAND_CLASS || objects[otmp->otyp].oc_charged)
            otmp->charges = (schar)o->enchantment;
        else if (otmp->otyp == TIN || otmp->otyp == MAGIC_LAMP || otmp->otyp == MAGIC_CANDLE || otmp->otyp == TOWEL || otmp->otyp == CANDELABRUM_OF_INVOCATION || is_obj_candelabrum(otmp) || otmp->oclass == SCROLL_CLASS)
            otmp->special_quality = (schar)o->enchantment;
        else if (otmp->otyp == CHEST && o->enchantment)
            otmp->speflags |= SPEFLAGS_SCHROEDINGERS_BOX;
        else if (otmp->otyp == STATUE && o->enchantment)
            otmp->speflags |= SPEFLAGS_STATUE_HISTORIC;
        else if (otmp->otyp == EGG && o->enchantment)
            otmp->speflags |= SPEFLAGS_YOURS;
        else
            otmp->enchantment = (schar)o->enchantment;
    }
    switch (o->curse_state) {
    case 1:
        bless(otmp);
        break; /* BLESSED */
    case 2:
        unbless(otmp);
        uncurse(otmp);
        break; /* uncursed */
    case 3:
        curse(otmp);
        break; /* CURSED */
    case 4:
        uncurse(otmp);
        break; /* not cursed */
    case 5:
        unbless(otmp);
        break; /* not blessed */
    default:
        break; /* Otherwise it's random and we're happy
                * with what mkobj gave us! */
    }

    /* corpsenm is "empty" if -1, random if -2, otherwise specific */
    if (o->corpsenm != NON_PM) {
        if (o->corpsenm == NON_PM - 1)
            set_corpsenm(otmp, rndmonnum());
        else
            set_corpsenm(otmp, o->corpsenm);
    }
    /* set_corpsenm() took care of egg hatch and corpse timers */

    if (named)
        otmp = oname(otmp, o->name.str);

    if (o->eroded) {
        if (o->eroded < 0) {
            otmp->oerodeproof = 1;
        } else {
            otmp->oeroded = (o->eroded % 4);
            otmp->oeroded2 = ((o->eroded >> 2) % 4);
        }
    }
    if (o->recharged)
        otmp->recharged = min(RECHARGE_LIMIT, o->recharged);
    if (o->locked) {
        otmp->olocked = 1;
    } else if (o->broken) {
        otmp->obroken = 1;
        otmp->olocked = 0; /* obj generation may set */
    }
    
    if (o->open) {
        otmp->speflags |= SPEFLAGS_LID_OPENED;
        set_corpsenm(otmp, NON_PM);
        otmp->owt = weight(otmp);
    }

    if (o->trapped == 0 || o->trapped == 1)
        otmp->otrapped = o->trapped;
    if (o->material > 0)
        otmp->material = (uchar)o->material;
    if (o->elemental_enchantment >= 0)
        otmp->elemental_enchantment = (uchar)o->elemental_enchantment;
    if (o->exceptionality >= 0)
        otmp->exceptionality = (uchar)o->exceptionality;

    if (o->mythic_type >= 0 && can_obj_have_mythic(otmp) && otmp->oartifact == 0)
    {
        randomize_mythic_quality(otmp, o->mythic_type, &otmp->mythic_prefix, &otmp->mythic_suffix);
    }

    if (o->mythic_prefix >= 0)
        otmp->mythic_prefix = (uchar)o->mythic_prefix;
    if (o->mythic_suffix >= 0)
        otmp->mythic_suffix = (uchar)o->mythic_suffix;
    if (o->age >= 0)
        otmp->age = (long)o->age;
    if (o->special_quality != -127)
    {
        otmp->special_quality = o->special_quality;
        if(is_obj_candelabrum(otmp) && otmp->special_quality > 0)
            otmp->age = MAX_BURN_IN_CANDELABRUM;
        if (otmp->oclass == SPBOOK_CLASS && otmp->special_quality >= -1)
        {
            if (objects[otmp->otyp].oc_subtyp == BOOKTYPE_NOVEL)
            {
                const char* ntitle = noveltitle(&otmp->novelidx, 0UL, 0UL);
                if (ntitle)
                {
                    otmp = oname(otmp, ntitle);
                }
            }
            else if (objects[otmp->otyp].oc_subtyp == BOOKTYPE_MANUAL)
            {
                const char* mtitle = manualtitle(&otmp->manualidx, 0UL, 0UL);
                if (mtitle)
                {
                    otmp = oname(otmp, mtitle);
                }
            }
        }
    }
    if(o->indestructible)
        otmp->speflags |= SPEFLAGS_INDESTRUCTIBLE;
    if (o->uses_up_key)
        otmp->speflags |= SPEFLAGS_USES_UP_KEY;
    if (o->no_pickup)
        otmp->speflags |= SPEFLAGS_NO_PICKUP;
    if (o->greased)
        otmp->greased = 1;
#ifdef INVISIBLE_OBJECTS
    if (o->invis)
        otmp->oinvis = 1;
#endif

    if ((objects[otmp->otyp].oc_flags5 & O5_TILE_IS_TILESET_DEPENDENT) != 0)
    {
        otmp->has_special_tileset = 1;
        otmp->special_tileset = levl[x][y].use_special_tileset ? levl[x][y].special_tileset : get_current_cmap_type_index();
    }

    if (o->quan > 0 && objects[otmp->otyp].oc_merge) {
        otmp->quan = o->quan;
        otmp->owt = weight(otmp);
    }

    /* contents */
    if (o->containment & SP_OBJ_CONTENT) {
        if (!container_idx) {
            if (!invent_carrying_monster) {
                /*impossible("create_object: no container");*/
                /* don't complain, the monster may be gone legally
                   (e.g. unique demon already generated)
                   TODO: In the case of unique demon lords, they should
                   get their inventories even when they get generated
                   outside the des-file.  Maybe another data file that
                   determines what inventories monsters get by default?
                 */
            } else {
                int ci;
                struct obj *objcheck = otmp;
                int inuse = -1;

                for (ci = 0; ci < container_idx; ci++)
                    if (container_obj[ci] == objcheck)
                        inuse = ci;
                remove_object(otmp);
                if (mpickobj(invent_carrying_monster, otmp)) {
                    if (inuse > -1) {
                        impossible(
                     "container given to monster was merged or deallocated.");
                        for (ci = inuse; ci < container_idx - 1; ci++)
                            container_obj[ci] = container_obj[ci + 1];
                        container_obj[container_idx] = NULL;
                        container_idx--;
                    }
                    /* we lost track of it. */
                    return;
                }
            }
        } else {
            struct obj *cobj = container_obj[container_idx - 1];

            remove_object(otmp);
            if (cobj) {
                (void) add_to_container(cobj, otmp);
                cobj->owt = weight(cobj);
            } else {
                obj_extract_self(otmp);
                obfree(otmp, NULL);
                return;
            }
        }
    }
    /* container */
    if (o->containment & SP_OBJ_CONTAINER) {
        delete_contents(otmp);
        if (container_idx < MAX_CONTAINMENT) {
            container_obj[container_idx] = otmp;
            container_idx++;
        } else
            impossible("create_object: too deeply nested containers.");
    }

    /* Medusa level special case: statues are petrified monsters, so they
     * are not stone-resistant and have monster inventory.  They also lack
     * other contents, but that can be specified as an empty container.
     */
    if (o->id == STATUE && Is_medusa_level(&u.uz) && o->corpsenm == NON_PM) {
        struct monst *was = NULL;
        struct obj *obj;
        int wastyp;
        int i = 0; /* prevent endless loop in case makemon always fails */

        /* Named random statues are of player types, and aren't stone-
         * resistant (if they were, we'd have to reset the name as well as
         * setting corpsenm).
         */
        for (wastyp = otmp->corpsenm; i < 1000; i++, wastyp = rndmonnum()) {
            /* makemon without rndmonst() might create a group */
            was = makemon(&mons[wastyp], 0, 0, MM_NOCOUNTBIRTH);
            if (was) {
                if (!resists_ston(was)) {
                    (void) propagate(wastyp, TRUE, FALSE);
                    break;
                }
                mongone(was);
                was = NULL;
            }
        }
        if (was) {
            set_corpsenm(otmp, wastyp);
            while (was->minvent) {
                obj = was->minvent;
                obj->owornmask = 0;
                obj_extract_self(obj);
                (void) add_to_container(otmp, obj);
            }
            otmp->owt = weight(otmp);
            mongone(was);
        }
    }

    if (o->id != -1) {
        static const char prize_warning[] = "multiple prizes on %s level";

        /* if this is a specific item of the right type and it is being
           created on the right level, flag it as the designated item
           used to detect a special achievement (to whit, reaching and
           exploring the target level, although the exploration part
           might be short-circuited if a monster brings object to hero) */
        if (Is_mineend_level(&u.uz)) {
            if (otmp->otyp == iflags.mines_prize_type && otmp->oartifact == iflags.mines_prize_oartifact) {
                otmp->speflags |= SPEFLAGS_MINES_PRIZE;
                /* prevent stacking; cleared when achievement is recorded */
                otmp->nomerge = 1;
                if (++mines_prize_count > 1)
                    impossible(prize_warning, "mines end");
            }
        } else if (Is_sokoend_level(&u.uz)) {
            if (otmp->otyp == iflags.soko_prize_type1) {
                otmp->speflags |= SPEFLAGS_SOKO_PRIZE1;
                otmp->nomerge = 1; /* redundant; Sokoban prizes don't stack */
                if (++soko_prize_count > 1)
                    impossible(prize_warning, "sokoban end");
            } else if (otmp->otyp == iflags.soko_prize_type2) {
                otmp->speflags |= SPEFLAGS_SOKO_PRIZE2;
                otmp->nomerge = 1; /* redundant; Sokoban prizes don't stack */
                if (++soko_prize_count > 1)
                    impossible(prize_warning, "sokoban end");
            }
        }
    }
    otmp->owt = weight(otmp);
    stackobj(otmp);

    if (o->lit) {
        begin_burn(otmp, FALSE);
    }

    if (o->buried) {
        boolean dealloced;

        (void) bury_an_obj(otmp, &dealloced);
        if (dealloced && container_idx) {
            container_obj[container_idx - 1] = NULL;
        }
    }
}

/*
 * Create a lever in a room.
 */
STATIC_OVL void
create_lever(lever, croom)
spllever* lever;
struct mkroom* croom;
{
    schar x = -1, y = -1;
    schar t_x = -1, t_y = -1;

    get_location_coord(&t_x, &t_y, ANY_LOC, croom, lever->target_coord);

    if (croom)
        get_free_room_loc(&x, &y, croom, lever->coord);
    else {
        int trycnt = 0;
        do {
            get_location_coord(&x, &y, DRY_NO_ICE, croom, lever->coord);
        } while ((levl[x][y].typ == STAIRS || levl[x][y].typ == LADDER)
            && ++trycnt <= 100);
        if (trycnt > 100)
            return;
    }

    coord tm;
    tm.x = x;
    tm.y = y;

    struct trap* lvr = mktrap(LEVER, 1, (struct mkroom*)0, &tm);
    if (lvr)
    {
        lvr->lever_effect = lever->lever_effect;
        lvr->effect_param1 = lever->effect_parameter1;
        lvr->effect_param2 = lever->effect_parameter2;
        lvr->effect_param3 = lever->effect_parameter3;
        lvr->effect_param4 = lever->effect_parameter4;
        lvr->effect_flags = lever->effect_flags;
        lvr->tflags = lever->lever_flags;

        if (lvr->lever_effect == LEVER_EFFECT_CREATE_UNCREATE_LOCATION_TYPE)
        {
            if (IS_FLOOR((int)lvr->effect_param1))
            {
                lvr->effect_param2 = (int)lever->effect_subtype2;
            }
            else
            {
                lvr->effect_param2 = lever->effect_subtype;
            }
            if (IS_FLOOR((int)lvr->effect_param3))
            {
                lvr->effect_param3 = lever->effect_subtype2;
            }
            else
            {
                lvr->effect_param3 = lever->effect_subtype;
            }
        }
        else if (lvr->lever_effect == LEVER_EFFECT_CREATE_LOCATION_TYPE)
        {
            if (IS_FLOOR((int)lvr->effect_param1))
            {
                lvr->effect_param2 = (int)lever->effect_subtype2;
            }
            else
            {
                lvr->effect_param2 = lever->effect_subtype;
            }
        }
        else if (lvr->lever_effect == LEVER_EFFECT_CREATE_TRAP)
        {
            lvr->effect_param4 = lever->effect_subtype;
        }

        coord tm2;
        tm2.x = t_x;
        tm2.y = t_y;

        lvr->launch = tm2;
    }

}
/*
 * Create an altar in a room.
 */
STATIC_OVL void
create_altar(a, croom)
altar *a;
struct mkroom *croom;
{
    schar sproom, x = -1, y = -1;
    aligntyp amask;
    boolean croom_is_temple = TRUE;
    int oldtyp;

    if (croom) {
        get_free_room_loc(&x, &y, croom, a->coord);
        if (croom->rtype != TEMPLE)
            croom_is_temple = FALSE;
    } else {
        get_location_coord(&x, &y, DRY_NO_ICE, croom, a->coord);
        if ((sproom = (schar) *in_rooms(x, y, TEMPLE)) != 0)
            croom = &rooms[sproom - ROOMOFFSET];
        else
            croom_is_temple = FALSE;
    }

    /* check for existing features */
    oldtyp = levl[x][y].typ;
    if (oldtyp == STAIRS || oldtyp == LADDER)
        return;

    /* Is the alignment random ?
     * If so, it's an 80% chance that the altar will be co-aligned.
     *
     * The alignment is encoded as amask values instead of alignment
     * values to avoid conflicting with the rest of the encoding,
     * shared by many other parts of the special level code.
     */
    amask = (a->align == AM_SPLEV_CO)
               ? Align2amask(u.ualignbase[A_ORIGINAL])
               : (a->align == AM_SPLEV_NONCO)
                  ? Align2amask(noncoalignment(u.ualignbase[A_ORIGINAL]))
                  : (a->align == -(MAX_REGISTERS + 1))
                     ? induced_align(80)
                     : (a->align < 0 ? ralign[-a->align - 1] : a->align);

    if (IS_FLOOR(oldtyp))
    {
        levl[x][y].floortyp = oldtyp;
        levl[x][y].floorsubtyp = levl[x][y].subtyp;
        levl[x][y].floorvartyp = levl[x][y].vartyp;
    }
    else
    {
        levl[x][y].floortyp = location_type_definitions[ALTAR].initial_floor_type;
        levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
        levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
    }
    levl[x][y].typ = ALTAR;
    levl[x][y].subtyp = a->subtyp;
    levl[x][y].vartyp = 0;
    levl[x][y].special_quality = 0;
    levl[x][y].altarmask = amask;

    if (a->shrine < 0)
        a->shrine = rn2(2); /* handle random case */

    if (!croom_is_temple || !a->shrine)
        return;

    if (a->shrine) { /* Is it a shrine  or sanctum? */
        priestini(&u.uz, croom, x, y, (a->shrine > 1), a->mtype);
        levl[x][y].altarmask |= AM_SHRINE;
        level.flags.has_temple = TRUE;
    }
}

/*
 * Create an anvil in a room.
 */
STATIC_OVL void
create_anvil(a, croom)
anvil* a;
struct mkroom* croom;
{
    schar sproom, x = -1, y = -1;
    boolean croom_is_smithy = TRUE;
    int oldtyp;

    if (croom) {
        get_free_room_loc(&x, &y, croom, a->coord);
        if (croom->rtype != SMITHY)
            croom_is_smithy = FALSE;
    }
    else {
        get_location_coord(&x, &y, DRY_NO_ICE, croom, a->coord);
        if ((sproom = (schar)*in_rooms(x, y, SMITHY)) != 0)
            croom = &rooms[sproom - ROOMOFFSET];
        else
            croom_is_smithy = FALSE;
    }

    /* check for existing features */
    oldtyp = levl[x][y].typ;
    if (oldtyp == STAIRS || oldtyp == LADDER)
        return;

    if (IS_FLOOR(oldtyp))
    {
        levl[x][y].floortyp = oldtyp;
        levl[x][y].floorsubtyp = levl[x][y].subtyp;
        levl[x][y].floorvartyp = levl[x][y].vartyp;
    }
    else
    {
        levl[x][y].floortyp = location_type_definitions[ANVIL].initial_floor_type;
        levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
        levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
    }
    levl[x][y].typ = ANVIL;
    levl[x][y].subtyp = 0;
    levl[x][y].vartyp = 0;
    levl[x][y].special_quality = 0;

    if (!croom_is_smithy)
        return;

    smithini(&u.uz, croom, x, y, 0, a->mtype);
    level.flags.has_smithy = TRUE;
}

/*
 * Create a decoration in a room.
 */
STATIC_OVL void
create_decoration(d, croom)
decoration* d;
struct mkroom* croom;
{
    schar x = -1, y = -1;

    get_location_coord(&x, &y, ANY_LOC, croom, d->coord);

    if (isok(x, y) && d->typ > 0 && d->typ < MAX_DECORATIONS)
    {
        levl[x][y].decoration_typ = d->typ;
        levl[x][y].decoration_subtyp = d->subtyp;
        levl[x][y].decoration_dir = max(0, min(3, d->dir));
        levl[x][y].decoration_flags = 0;
        if ((decoration_type_definitions[d->typ].dflags & DECORATION_TYPE_FLAGS_LOOTABLE) != 0)
        {
            if (d->item_in_holder & 1)
                levl[x][y].decoration_flags |= DECORATION_FLAGS_ITEM_IN_HOLDER;
            if (d->item_in_holder & 2)
                levl[x][y].decoration_flags |= DECORATION_FLAGS_ITEM2_IN_HOLDER;
            if (d->item_in_holder & 4)
                levl[x][y].decoration_flags |= DECORATION_FLAGS_ITEM3_IN_HOLDER;
        }
        if (!d->lit && (decoration_type_definitions[d->typ].dflags & DECORATION_TYPE_FLAGS_LIGHTABLE) != 0)
            levl[x][y].flags |= L_INITIALLY_UNLIT;  //This uses normal flags, since lamplit is shared as well
    }
}


/*
 * Create an anvil in a room.
 */
STATIC_OVL void
create_npc(a, croom)
npc_create_info* a;
struct mkroom* croom;
{
    schar sproom, x = -1, y = -1;
    boolean croom_is_npc_room = TRUE;

    if (croom) {
        get_free_room_loc(&x, &y, croom, a->coord);
        if (croom->rtype != NPCROOM)
            croom_is_npc_room = FALSE;
    }
    else {
        get_location_coord(&x, &y, DRY_NO_ICE, croom, a->coord);
        if ((sproom = (schar)*in_rooms(x, y, NPCROOM)) != 0)
            croom = &rooms[sproom - ROOMOFFSET];
        else
            croom_is_npc_room = FALSE;
    }

    if (!croom_is_npc_room)
        return;

    uchar usedtyp = (a->typ < MAX_NPC_SUBTYPES ? a->typ : rn2(MAX_NPC_SUBTYPES));
    npcini(&u.uz, croom, x, y, usedtyp, a->mtype);
    level.flags.has_npc_room = TRUE;
}

/*
 * Create an modron portal in a room.
 */
STATIC_OVL void
create_modron_portal(a, croom)
modron_portal* a;
struct mkroom* croom;
{
    schar x = -1, y = -1;
    schar t_x = -1, t_y = -1;
    coord tm, portal_tm;
    unsigned long pflags = a->activated ? TRAPFLAGS_ACTIVATED : TRAPFLAGS_NONE;
    if (a->level_teleporter)
    {
        pflags |= TRAPFLAGS_LEVEL_TELEPORTER;
        if (a->tele_direction < 0)
            pflags |= TRAPFLAGS_LEVEL_TELEPORT_UP;
        else if (a->tele_direction > 0)
            pflags |= TRAPFLAGS_LEVEL_TELEPORT_DOWN;

        if (a->end_type != 0)
            pflags |= TRAPFLAGS_LEVEL_TELEPORT_NO_OTHER_END;
    }

    if (croom)
        get_free_room_loc(&x, &y, croom, a->coord);
    else 
    {
        int trycnt = 0;
        do 
        {
            get_location_coord(&x, &y, DRY_NO_ICE, croom, a->coord);
        } while ((levl[x][y].typ == STAIRS || levl[x][y].typ == LADDER)
            && ++trycnt <= 100);

        if (trycnt > 100)
            return;
    }

    get_location_coord(&t_x, &t_y, ANY_LOC, croom, a->t_coord);

    tm.x = x;
    tm.y = y;
    portal_tm.x = t_x;
    portal_tm.y = t_y;

    mkmodronportal(a->typ, &tm, &portal_tm, pflags);
}

void
replace_terrain(terr, croom)
replaceterrain *terr;
struct mkroom *croom;
{
    schar x, y, x1, y1, x2, y2;

    if (terr->toter >= MAX_LEVTYPE)
        return;

    x1 = terr->x1;
    y1 = terr->y1;
    get_location(&x1, &y1, ANY_LOC, croom);

    x2 = terr->x2;
    y2 = terr->y2;
    get_location(&x2, &y2, ANY_LOC, croom);

    for (x = max(x1, 0); x <= min(x2, COLNO - 1); x++)
        for (y = max(y1, 0); y <= min(y2, ROWNO - 1); y++)
            if (levl[x][y].typ == terr->fromter && rn2(100) < terr->chance) {
                SET_TYPLIT(x, y, terr->toter, -1, terr->tolit);
            }
}

/*
 * Search for a door in a room on a specified wall.
 */
STATIC_OVL boolean
search_door(croom, x, y, wall, cnt)
struct mkroom *croom;
xchar *x, *y;
xchar wall;
int cnt;
{
    int dx, dy;
    int xx, yy;

    switch (wall) {
    case W_NORTH:
        dy = 0;
        dx = 1;
        xx = croom->lx;
        yy = croom->hy + 1;
        break;
    case W_SOUTH:
        dy = 0;
        dx = 1;
        xx = croom->lx;
        yy = croom->ly - 1;
        break;
    case W_EAST:
        dy = 1;
        dx = 0;
        xx = croom->hx + 1;
        yy = croom->ly;
        break;
    case W_WEST:
        dy = 1;
        dx = 0;
        xx = croom->lx - 1;
        yy = croom->ly;
        break;
    default:
        dx = dy = xx = yy = 0;
        panic("search_door: Bad wall!");
        return FALSE;
        break;
    }
    while (xx <= croom->hx + 1 && yy <= croom->hy + 1) {
        if (IS_DOOR(levl[xx][yy].typ) || levl[xx][yy].typ == SDOOR) {
            *x = xx;
            *y = yy;
            if (cnt-- <= 0)
                return TRUE;
        }
        xx += dx;
        yy += dy;
    }
    return FALSE;
}

/*
 * Dig a corridor between two points.
 */
boolean
dig_corridor(org, dest, nxcor, ftyp, btyp)
coord *org, *dest;
boolean nxcor;
schar ftyp, btyp;
{
    int dx = 0, dy = 0, dix, diy, cct;
    struct rm *crm;
    int tx, ty, xx, yy;

    xx = org->x;
    yy = org->y;
    tx = dest->x;
    ty = dest->y;
    if (xx <= 0 || yy <= 0 || tx <= 0 || ty <= 0 || xx > COLNO - 1
        || tx > COLNO - 1 || yy > ROWNO - 1 || ty > ROWNO - 1) 
    {
        debugpline4("dig_corridor: bad coords <%d,%d> <%d,%d>.",
                    xx, yy, tx, ty);
        return FALSE;
    }
    if (tx > xx)
        dx = 1;
    else if (ty > yy)
        dy = 1;
    else if (tx < xx)
        dx = -1;
    else
        dy = -1;

    xx -= dx;
    yy -= dy;
    cct = 0;
    while (xx != tx || yy != ty) 
    {
        /* loop: dig corridor at [xx,yy] and find new [xx,yy] */
        if (cct++ > 500 || (nxcor && !rn2(35)))
            return FALSE;

        xx += dx;
        yy += dy;

        if (xx >= COLNO - 1 || xx <= 0 || yy <= 0 || yy >= ROWNO - 1)
            return FALSE; /* impossible */

        crm = &levl[xx][yy];
        if (crm->typ == btyp) 
        {
            int normalcorr = (context.game_difficulty <= NO_SECRET_DOORS_DIFFICULTY_THRESHOLD && u.uz.dnum == main_dungeon_dnum && u.uz.dlevel <= NO_SECRET_DOORS_DUNGEON_LEVEL_THRESHOLD) ? TRUE : rn2(100);
            if (ftyp != CORR || normalcorr)
            {
                crm->typ = ftyp;
                crm->subtyp = get_initial_location_subtype(ftyp);
                crm->vartyp = get_initial_location_vartype(crm->typ, crm->subtyp);
                crm->special_quality = 0;
                if (IS_FLOOR(ftyp))
                {
                    crm->floortyp = 0;
                    crm->floorsubtyp = 0;
                    crm->floorvartyp = 0;
                }
                if (nxcor && !rn2(50))
                    (void) mksobj_at(BOULDER, xx, yy, TRUE, FALSE);
            } 
            else 
            {
                crm->typ = SCORR;
                crm->subtyp = get_initial_location_subtype(SCORR);
                crm->vartyp = get_initial_location_vartype(crm->typ, crm->subtyp);
            }
        } 
        else if (crm->typ != ftyp && crm->typ != SCORR) 
        {
            /* strange ... */
            return FALSE;
        }

        /* find next corridor position */
        dix = abs(xx - tx);
        diy = abs(yy - ty);

        if ((dix > diy) && diy && !rn2(dix-diy+1)) 
        {
            dix = 0;
        } 
        else if ((diy > dix) && dix && !rn2(diy-dix+1))
        {
            diy = 0;
        }

        /* do we have to change direction ? */
        if (dy && dix > diy)
        {
            register int ddx = (xx > tx) ? -1 : 1;

            crm = &levl[xx + ddx][yy];
            if (crm->typ == btyp || crm->typ == ftyp || crm->typ == SCORR)
            {
                dx = ddx;
                dy = 0;
                continue;
            }
        } 
        else if (dx && diy > dix) 
        {
            register int ddy = (yy > ty) ? -1 : 1;

            crm = &levl[xx][yy + ddy];
            if (crm->typ == btyp || crm->typ == ftyp || crm->typ == SCORR)
            {
                dy = ddy;
                dx = 0;
                continue;
            }
        }

        /* continue straight on? */
        crm = &levl[xx + dx][yy + dy];
        if (crm->typ == btyp || crm->typ == ftyp || crm->typ == SCORR)
            continue;

        /* no, what must we do now?? */
        if (dx) 
        {
            dx = 0;
            dy = (ty < yy) ? -1 : 1;
        } 
        else 
        {
            dy = 0;
            dx = (tx < xx) ? -1 : 1;
        }
        crm = &levl[xx + dx][yy + dy];
        if (crm->typ == btyp || crm->typ == ftyp || crm->typ == SCORR)
            continue;
        dy = -dy;
        dx = -dx;
    }
    return TRUE;
}

/*
 * Disgusting hack: since special levels have their rooms filled before
 * sorting the rooms, we have to re-arrange the speed values upstairs_room
 * and dnstairs_room after the rooms have been sorted.  On normal levels,
 * stairs don't get created until _after_ sorting takes place.
 */
STATIC_OVL void
fix_stair_rooms()
{
    int i;
    struct mkroom *croom;

    if (xdnstair
        && !((dnstairs_room->lx <= xdnstair && xdnstair <= dnstairs_room->hx)
             && (dnstairs_room->ly <= ydnstair
                 && ydnstair <= dnstairs_room->hy))) {
        for (i = 0; i < nroom; i++) {
            croom = &rooms[i];
            if ((croom->lx <= xdnstair && xdnstair <= croom->hx)
                && (croom->ly <= ydnstair && ydnstair <= croom->hy)) {
                dnstairs_room = croom;
                break;
            }
        }
        if (i == nroom)
        {
            panic("Couldn't find dnstair room in fix_stair_rooms!");
            return;
        }
    }
    if (xupstair
        && !((upstairs_room->lx <= xupstair && xupstair <= upstairs_room->hx)
             && (upstairs_room->ly <= yupstair
                 && yupstair <= upstairs_room->hy))) {
        for (i = 0; i < nroom; i++) {
            croom = &rooms[i];
            if ((croom->lx <= xupstair && xupstair <= croom->hx)
                && (croom->ly <= yupstair && yupstair <= croom->hy)) {
                upstairs_room = croom;
                break;
            }
        }
        if (i == nroom)
        {
            panic("Couldn't find upstair room in fix_stair_rooms!");
            return;
        }
    }
}

/*
 * Corridors always start from a door. But it can end anywhere...
 * Basically we search for door coordinates or for endpoints coordinates
 * (from a distance).
 */
STATIC_OVL void
create_corridor(c)
corridor *c;
{
    coord org, dest;

    if (c->src.room == -1) {
        fix_stair_rooms();
        makecorridors(); /*makecorridors(c->src.door);*/
        return;
    }

    if (!search_door(&rooms[c->src.room], &org.x, &org.y, c->src.wall,
                     c->src.door))
        return;

    if (c->dest.room != -1) {
        if (!search_door(&rooms[c->dest.room], &dest.x, &dest.y, c->dest.wall,
                         c->dest.door))
            return;
        switch (c->src.wall) {
        case W_NORTH:
            org.y--;
            break;
        case W_SOUTH:
            org.y++;
            break;
        case W_WEST:
            org.x--;
            break;
        case W_EAST:
            org.x++;
            break;
        }
        switch (c->dest.wall) {
        case W_NORTH:
            dest.y--;
            break;
        case W_SOUTH:
            dest.y++;
            break;
        case W_WEST:
            dest.x--;
            break;
        case W_EAST:
            dest.x++;
            break;
        }
        (void) dig_corridor(&org, &dest, FALSE, CORR, STONE);
    }
}

/*
 * Fill a room (shop, zoo, etc...) with appropriate stuff.
 */
void
fill_room(croom, prefilled)
struct mkroom *croom;
boolean prefilled;
{
    if (!croom || croom->rtype == OROOM)
        return;

    if (!prefilled) {
        int x, y;

        /* Shop ? */
        if (croom->rtype >= SHOPBASE) {
            stock_room(croom->rtype - SHOPBASE, croom, FALSE);
            level.flags.has_shop = TRUE;
            return;
        }

        switch (croom->rtype) {
        case VAULT:
            for (x = croom->lx; x <= croom->hx; x++)
                for (y = croom->ly; y <= croom->hy; y++)
                    (void) mkgold((long) rn1(abs(depth(&u.uz)) * 100, 51),
                                  x, y);
            break;
        case COURT:
        case ZOO:
        case BEEHIVE:
        case LIBRARY:
        case ANTHOLE:
        case COCKNEST:
        case LEPREHALL:
        case MORGUE:
        case BARRACKS:
        case ARMORY:
            fill_zoo(croom);
            break;
        }
    }
    switch (croom->rtype) {
    case VAULT:
        level.flags.has_vault = TRUE;
        break;
    case ZOO:
        level.flags.has_zoo = TRUE;
        break;
    case COURT:
        level.flags.has_court = TRUE;
        break;
    case MORGUE:
        level.flags.has_morgue = TRUE;
        break;
    case BEEHIVE:
        level.flags.has_beehive = TRUE;
        break;
    case LIBRARY:
        level.flags.has_library = TRUE;
        break;
    case BARRACKS:
        level.flags.has_barracks = TRUE;
        break;
    case ARMORY:
        level.flags.has_armory = TRUE;
        break;
    case TEMPLE:
        level.flags.has_temple = TRUE;
        break;
    case SMITHY:
        level.flags.has_smithy = TRUE;
        break;
    case NPCROOM:
        level.flags.has_npc_room = TRUE;
        break;
    case SWAMP:
        level.flags.has_swamp = TRUE;
        break;
    }
}

struct mkroom *
build_room(r, mkr)
room *r;
struct mkroom *mkr;
{
    boolean okroom;
    struct mkroom *aroom;
    xchar rtype = (!r->chance || rn2(100) < r->chance) ? r->rtype : OROOM;

    if (mkr) {
        aroom = &subrooms[nsubroom];
        okroom = create_subroom(mkr, r->x, r->y, r->w, r->h, rtype, r->rlit, r->rfloortyp, r->rfloorsubtyp, r->mtype, r->tileset, r->decotyp);
    } else {
        aroom = &rooms[nroom];
        okroom = create_room(r->x, r->y, r->w, r->h, r->xalign, r->yalign,
                             rtype, r->rlit, r->rfloortyp, r->rfloorsubtyp, r->mtype, r->tileset, r->decotyp);
    }

    if (okroom) {
#ifdef SPECIALIZATION
        topologize(aroom, FALSE); /* set roomno */
#else
        topologize(aroom); /* set roomno */
#endif
        aroom->needfill = r->filled;
        aroom->needjoining = r->joined;
        return aroom;
    }
    return (struct mkroom *) 0;
}

/*
 * set lighting in a region that will not become a room.
 */
STATIC_OVL void
light_region(tmpregion)
region *tmpregion;
{
    register boolean litstate = tmpregion->rlit ? 1 : 0;
    register int hiy = tmpregion->y2;
    register int x, y;
    register struct rm *lev;
    int lowy = tmpregion->y1;
    int lowx = tmpregion->x1, hix = tmpregion->x2;

    if (litstate) 
    {
        /* adjust region size for walls, but only if lighted */
        lowx = max(lowx - 1, 1);
        hix = min(hix + 1, COLNO - 1);
        lowy = max(lowy - 1, 0);
        hiy = min(hiy + 1, ROWNO - 1);
    }

    for (x = lowx; x <= hix; x++) 
    {
        lev = &levl[x][lowy];
        for (y = lowy; y <= hiy; y++)
        {
            if (lev->typ != LAVAPOOL) /* this overrides normal lighting */
                lev->lit = litstate;

            lev++;
        }
    }
}

STATIC_OVL void
wallify_map(x1, y1, x2, y2)
int x1, y1, x2, y2;
{
    int x, y, xx, yy, lo_xx, lo_yy, hi_xx, hi_yy;

    y1 = max(y1, 0);
    x1 = max(x1, 1);
    y2 = min(y2, ROWNO - 1);
    x2 = min(x2, COLNO - 1);
    for (y = y1; y <= y2; y++) {
        lo_yy = (y > 0) ? y - 1 : 0;
        hi_yy = (y < y2) ? y + 1 : y2;
        for (x = x1; x <= x2; x++) {
            if (levl[x][y].typ != STONE)
                continue;
            lo_xx = (x > 1) ? x - 1 : 1;
            hi_xx = (x < x2) ? x + 1 : x2;
            for (yy = lo_yy; yy <= hi_yy; yy++)
                for (xx = lo_xx; xx <= hi_xx; xx++)
                    if (IS_ROOM(levl[xx][yy].typ) || levl[xx][yy].typ == CORR
                        || levl[xx][yy].typ == CROSSWALL) 
                    {
                        levl[x][y].typ = (yy != y) ? HWALL : VWALL;
                        /* Keep stone subtyp */
                        yy = hi_yy; /* end `yy' loop */
                        break;      /* end `xx' loop */
                    }
        }
    }
}

/*
 * Select a random coordinate in the maze.
 *
 * We want a place not 'touched' by the loader.  That is, a place in
 * the maze outside every part of the special level.
 */
STATIC_OVL void
maze1xy(m, humidity)
coord *m;
int humidity;
{
    register int x, y, tryct = 2000;
    /* tryct:  normally it won't take more than ten or so tries due
       to the circumstances under which we'll be called, but the
       `humidity' screening might drastically change the chances */

    do {
        x = rn1(x_maze_max - 3, 3);
        y = rn1(y_maze_max - 3, 3);
        if (--tryct < 0)
            break; /* give up */
    } while (!(x % 2) || !(y % 2) || SpLev_Map[x][y]
             || !is_ok_location((schar) x, (schar) y, humidity));

    m->x = (xchar) x, m->y = (xchar) y;
}

/*
 * If there's a significant portion of maze unused by the special level,
 * we don't want it empty.
 *
 * Makes the number of traps, monsters, etc. proportional
 * to the size of the maze.
 */
STATIC_OVL void
fill_empty_maze()
{
    int mapcountmax, mapcount, mapfact;
    xchar x, y;
    coord mm;

    mapcountmax = mapcount = (x_maze_max - 2) * (y_maze_max - 2);
    mapcountmax = mapcountmax / 2;

    for (x = 2; x < x_maze_max; x++)
        for (y = 0; y < y_maze_max; y++)
            if (SpLev_Map[x][y])
                mapcount--;

    if ((mapcount > (int) (mapcountmax / 10))) {
        mapfact = (int) ((mapcount * 100L) / mapcountmax);
        for (x = rnd((int) (20 * mapfact) / 100); x; x--) {
            maze1xy(&mm, DRY);
            (void) mkobj_at(rn2(2) ? GEM_CLASS : RANDOM_CLASS, mm.x, mm.y,
                            TRUE);
        }
        for (x = rnd((int) (12 * mapfact) / 100); x; x--) {
            maze1xy(&mm, DRY);
            (void) mksobj_at(BOULDER, mm.x, mm.y, TRUE, FALSE);
        }
        for (x = rn2(2); x; x--) {
            maze1xy(&mm, DRY);
            (void) makemon(&mons[PM_MINOTAUR], mm.x, mm.y, NO_MM_FLAGS);
        }
        for (x = rnd((int) (12 * mapfact) / 100); x; x--) {
            maze1xy(&mm, DRY);
            (void) make_level_monster(mm.x, mm.y, NO_MM_FLAGS, MM2_RANDOMIZE_SUBTYPE);
        }
        for (x = rn2((int) (15 * mapfact) / 100); x; x--) {
            maze1xy(&mm, DRY);
            (void) mkgold(0L, mm.x, mm.y);
        }
        for (x = rn2((int) (15 * mapfact) / 100); x; x--) {
            int trytrap;

            maze1xy(&mm, DRY_NO_ICE); /* Probably should not create on air, but this might exclude a big part of air levels */
            trytrap = rndtrap();
            if (sobj_at(BOULDER, mm.x, mm.y))
                while (is_pit(trytrap) || is_hole(trytrap))
                    trytrap = rndtrap();
            (void) maketrap(mm.x, mm.y, trytrap, NON_PM, MKTRAP_NO_FLAGS);
        }
    }
}

/*
 * special level loader
 */
STATIC_OVL boolean
sp_level_loader(fd, lvl)
dlb *fd;
sp_lev *lvl;
{
    long n_opcode = 0;
    struct opvar *opdat;
    int opcode;

    Fread((genericptr_t) & (lvl->n_opcodes), 1, sizeof(lvl->n_opcodes), fd);
    lvl->opcodes = (_opcode *) alloc(sizeof(_opcode) * (size_t)(lvl->n_opcodes));

    while (n_opcode < lvl->n_opcodes) {
        Fread((genericptr_t) &lvl->opcodes[n_opcode].opcode, 1,
              sizeof(lvl->opcodes[n_opcode].opcode), fd);
        opcode = lvl->opcodes[n_opcode].opcode;

        opdat = NULL;

        if (opcode < SPO_NULL || opcode >= MAX_SP_OPCODES)
        {
            panic("sp_level_loader: impossible opcode %i.", opcode);
            return FALSE;
        }

        if (opcode == SPO_PUSH) {
            size_t nsize;
            struct opvar *ov = (struct opvar *) alloc(sizeof(struct opvar));

            opdat = ov;
            ov->spovartyp = SPO_NULL;
            ov->vardata.l = 0;
            Fread((genericptr_t) & (ov->spovartyp), 1, sizeof(ov->spovartyp),
                  fd);

            switch (ov->spovartyp) {
            case SPOVAR_NULL:
                break;
            case SPOVAR_COORD:
            case SPOVAR_REGION:
            case SPOVAR_MAPCHAR:
            case SPOVAR_MONST:
            case SPOVAR_OBJ:
            case SPOVAR_INT:
                Fread((genericptr_t) & (ov->vardata.l), 1,
                      sizeof(ov->vardata.l), fd);
                break;
            case SPOVAR_VARIABLE:
            case SPOVAR_STRING:
            case SPOVAR_SEL: {
                char *opd;

                Fread((genericptr_t) &nsize, 1, sizeof(nsize), fd);
                opd = (char *) alloc(nsize + 1);

                if (nsize)
                    Fread(opd, 1, nsize, fd);
                opd[nsize] = 0;
                ov->vardata.str = opd;
                break;
            }
            default:
                panic("sp_level_loader: unknown opvar type %i",
                      ov->spovartyp);
            }
        }

        lvl->opcodes[n_opcode].opdat = opdat;
        n_opcode++;
    } /*while*/

    return TRUE;
}

/* Frees the memory allocated for special level creation structs */
STATIC_OVL boolean
sp_level_free(lvl)
sp_lev *lvl;
{
    static const char nhFunc[] = "sp_level_free";
    long n_opcode = 0;

    while (n_opcode < lvl->n_opcodes) {
        int opcode = lvl->opcodes[n_opcode].opcode;
        struct opvar *opdat = lvl->opcodes[n_opcode].opdat;

        if (opcode < SPO_NULL || opcode >= MAX_SP_OPCODES)
            panic("sp_level_free: unknown opcode %i", opcode);

        if (opdat)
            opvar_free(opdat);
        n_opcode++;
    }
    Free(lvl->opcodes);
    lvl->opcodes = NULL;
    return TRUE;
}

void
splev_initlev(linit)
lev_init *linit;
{
    switch (linit->init_style) {
    default:
        impossible("Unrecognized level init style.");
        break;
    case LVLINIT_NONE:
        break;
    case LVLINIT_SOLIDFILL:
        if (linit->lit == -1)
            linit->lit = rn2(2);
        lvlfill_solid(linit->filling, linit->lit);
        break;
    case LVLINIT_MAZEGRID:
        lvlfill_maze_grid(2, 0, x_maze_max, y_maze_max, linit->filling);
        break;
    case LVLINIT_ROGUE:
        makeroguerooms();
        break;
    case LVLINIT_MINES:
        if (linit->lit == -1)
            linit->lit = rn2(2);
        if (linit->filling > -1)
            lvlfill_solid(linit->filling, 0);
        linit->icedpools = icedpools;
        mkmap(linit);
        break;
    }
}

struct sp_frame *
frame_new(execptr)
long execptr;
{
    struct sp_frame *frame =
        (struct sp_frame *) alloc(sizeof(struct sp_frame));

    frame->next = NULL;
    frame->variables = NULL;
    frame->n_opcode = execptr;
    frame->stack = (struct splevstack *) alloc(sizeof(struct splevstack));
    splev_stack_init(frame->stack);
    return frame;
}

void
frame_del(frame)
struct sp_frame *frame;
{
    if (!frame)
        return;
    if (frame->stack) {
        splev_stack_done(frame->stack);
        frame->stack = NULL;
    }
    if (frame->variables) {
        variable_list_del(frame->variables);
        frame->variables = NULL;
    }
    Free(frame);
}

void
spo_frame_push(coder)
struct sp_coder *coder;
{
    struct sp_frame *tmpframe = frame_new(coder->frame->n_opcode);

    tmpframe->next = coder->frame;
    coder->frame = tmpframe;
}

void
spo_frame_pop(coder)
struct sp_coder *coder;
{
    if (coder->frame && coder->frame->next) {
        struct sp_frame *tmpframe = coder->frame->next;

        frame_del(coder->frame);
        coder->frame = tmpframe;
        coder->stack = coder->frame->stack;
    }
}

long
sp_code_jmpaddr(curpos, jmpaddr)
long curpos, jmpaddr;
{
    return (curpos + jmpaddr);
}

void
spo_call(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_call";
    struct opvar *addr;
    struct opvar *params;
    struct sp_frame *tmpframe;

    if (!OV_pop_i(addr) || !OV_pop_i(params))
        return;
    if (OV_i(params) < 0)
        return;

    tmpframe = frame_new(sp_code_jmpaddr(coder->frame->n_opcode,
                                         OV_i(addr) - 1));

    while (OV_i(params)-- > 0) {
        splev_stack_push(tmpframe->stack, splev_stack_getdat_any(coder));
    }
    splev_stack_reverse(tmpframe->stack);

    /* push a frame */
    tmpframe->next = coder->frame;
    coder->frame = tmpframe;

    opvar_free(addr);
    opvar_free(params);
}

void
spo_return(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_return";
    struct opvar *params;

    if (!coder->frame || !coder->frame->next)
    {
        panic("return: no frame.");
        return;
    }
    if (!OV_pop_i(params))
        return;
    if (OV_i(params) < 0)
        return;

    while (OV_i(params)-- > 0) 
    {
        splev_stack_push(coder->frame->next->stack,
                         splev_stack_pop(coder->stack));
    }

    /* pop the frame */
    if (coder->frame->next) 
    {
        struct sp_frame *tmpframe = coder->frame->next;
        frame_del(coder->frame);
        coder->frame = tmpframe;
        coder->stack = coder->frame->stack;
    }

    opvar_free(params);
}

/*ARGUSED*/
void
spo_end_moninvent(coder)
struct sp_coder *coder UNUSED;
{
    if (invent_carrying_monster)
        m_dowear(invent_carrying_monster, TRUE);
    invent_carrying_monster = NULL;
}

/*ARGUSED*/
void
spo_pop_container(coder)
struct sp_coder *coder UNUSED;
{
    if (container_idx > 0) {
        container_idx--;
        container_obj[container_idx] = NULL;
    }
}

void
spo_message(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_message";
    struct opvar *op, * mtype_opvar, * mattr_opvar, *mcolor_opvar, *soundtyp_opvar, *soundid_opvar, *soundparam_opvar, *msgflags_opvar;
    char* msg; // , * levmsg;
    size_t n; //old_n, 
    struct lev_msg* levmsg;
    struct lev_msg zeromsg = { 0 };

    if (!OV_pop_i(mtype_opvar) || !OV_pop_i(mattr_opvar) || !OV_pop_i(mcolor_opvar) || !OV_pop_i(soundtyp_opvar) || !OV_pop_i(soundid_opvar)
        || !OV_pop_i(soundparam_opvar) || !OV_pop_i(msgflags_opvar) || !OV_pop_s(op))
        return;
    msg = OV_s(op);
    if (!msg)
        return;

    n = strlen(msg);
    int mtype = (int)OV_i(mtype_opvar);
    int mattr = (int)OV_i(mattr_opvar);
    int mcolor = (int)OV_i(mcolor_opvar);
    int soundtyp = (int)OV_i(soundtyp_opvar);
    int soundid = (int)OV_i(soundid_opvar);
    int soundparam = (int)OV_i(soundparam_opvar);
    unsigned long msgflags = (unsigned long)OV_i(msgflags_opvar);

    levmsg = (struct lev_msg*)alloc(sizeof(struct lev_msg));
    if (levmsg)
    {
        *levmsg = zeromsg;
        levmsg->msg_type = mtype;
        levmsg->attr = mattr;
        levmsg->color = mcolor;
        levmsg->sound_type = soundtyp;
        levmsg->sound_id = soundid;
        levmsg->sound_param = soundparam;
        levmsg->msgflags = msgflags;
        levmsg->message = (char*)alloc(n + 1);
        Strcpy(levmsg->message, msg);
        if(!lev_message)
            lev_message = levmsg;
        else
        {
            struct lev_msg* lm;
            for (lm = lev_message; lm->next; lm = lm->next)
                ;
            lm->next = levmsg;
        }
    }
    
    opvar_free(op);
    opvar_free(mtype_opvar);
    opvar_free(mattr_opvar);
    opvar_free(mcolor_opvar);
    opvar_free(soundtyp_opvar);
    opvar_free(soundid_opvar);
    opvar_free(soundparam_opvar);
    opvar_free(msgflags_opvar);
}

STATIC_VAR const monster emptymons; /* Initialized to zero automatically */

void
spo_monster(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_monster";
    int nparams = 0;
    struct opvar *varparam;
    struct opvar *id, *mcoord, *has_inv;
    monster tmpmons = emptymons;

    tmpmons.peaceful = -1;
    tmpmons.asleep = -1;
    tmpmons.name.str = (char *) 0;
    tmpmons.appear = 0;
    tmpmons.appear_as.str = (char *) 0;
    tmpmons.align = -MAX_REGISTERS - 2;
    tmpmons.female = -1; /* random */
    tmpmons.invis = 0;
    tmpmons.cancelled = 0;
    tmpmons.revived = 0;
    tmpmons.avenge = 0;
    tmpmons.fleeing = 0;
    tmpmons.blinded = 0;
    tmpmons.paralyzed = 0;
    tmpmons.stunned = 0;
    tmpmons.confused = 0;
    tmpmons.waitforu = 0;
    tmpmons.keep_original_invent = 0;
    tmpmons.protector = 0;
    tmpmons.seentraps = 0;
    tmpmons.has_invent = 0;
    tmpmons.id = 0;
    tmpmons.coord = 0;
    tmpmons.x = 0;
    tmpmons.y = 0;
    tmpmons.class = 0;

    if (!OV_pop_i(has_inv))
        return;

    if (!OV_pop_i(varparam))
        return;

    while ((nparams++ < (SP_M_V_END + 1)) && varparam && (OV_typ(varparam) == SPOVAR_INT)
           && (OV_i(varparam) >= 0) && (OV_i(varparam) < SP_M_V_END)) 
    {
        struct opvar *parm = NULL;

        OV_pop(parm);
        switch (OV_i(varparam)) {
        case SP_M_V_NAME:
            if ((OV_typ(parm) == SPOVAR_STRING) && !tmpmons.name.str)
                tmpmons.name.str = dupstr(OV_s(parm));
            break;
        case SP_M_V_APPEAR:
            if ((OV_typ(parm) == SPOVAR_INT) && !tmpmons.appear_as.str) {
                tmpmons.appear = (xchar)OV_i(parm);
                opvar_free(parm);
                OV_pop(parm);
                tmpmons.appear_as.str = dupstr(OV_s(parm));
            }
            break;
        case SP_M_V_ASLEEP:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.asleep = (schar)OV_i(parm);
            break;
        case SP_M_V_ALIGN:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.align = (aligntyp)OV_i(parm);
            break;
        case SP_M_V_PEACEFUL:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.peaceful = (schar)OV_i(parm);
            break;
        case SP_M_V_FEMALE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.female = OV_i(parm);
            break;
        case SP_M_V_WAITFORU:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.waitforu = OV_i(parm);
            break;
        case SP_M_V_KEEP_ORIGINAL_INVENTORY:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.keep_original_invent = OV_i(parm);
            break;
        case SP_M_V_PROTECTOR:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.protector = OV_i(parm);
            break;
        case SP_M_V_MAXHP:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.maxhp = OV_i(parm);
            break;
        case SP_M_V_LEVEL_ADJUSTMENT:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.level_adjustment = OV_i(parm);
            break;
        case SP_M_V_INVIS:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.invis = OV_i(parm);
            break;
        case SP_M_V_CANCELLED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.cancelled = OV_i(parm);
            break;
        case SP_M_V_REVIVED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.revived = OV_i(parm);
            break;
        case SP_M_V_AVENGE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.avenge = OV_i(parm);
            break;
        case SP_M_V_FLEEING:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.fleeing = OV_i(parm);
            break;
        case SP_M_V_BLINDED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.blinded = OV_i(parm);
            break;
        case SP_M_V_PARALYZED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.paralyzed = OV_i(parm);
            break;
        case SP_M_V_STUNNED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.stunned = OV_i(parm);
            break;
        case SP_M_V_CONFUSED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.confused = OV_i(parm);
            break;
        case SP_M_V_SEENTRAPS:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpmons.seentraps = OV_i(parm);
            break;
        case SP_M_V_END:
            nparams = SP_M_V_END + 1;
            break;
        default:
            impossible("MONSTER with unknown variable param type!");
            break;
        }
        opvar_free(parm);
        if (OV_i(varparam) != SP_M_V_END) {
            opvar_free(varparam);
            OV_pop(varparam);
        }
    }

    if (!OV_pop_c(mcoord)) {
        panic("no monster coord?");
        return;
    }

    if (!OV_pop_typ(id, SPOVAR_MONST)) {
        panic("no mon type");
        return;
    }

    tmpmons.id = SP_MONST_PM(OV_i(id));
    tmpmons.class = SP_MONST_CLASS(OV_i(id));
    tmpmons.coord = OV_i(mcoord);
    tmpmons.has_invent = OV_i(has_inv);

    create_monster(&tmpmons, coder->croom);

    Free(tmpmons.name.str);
    Free(tmpmons.appear_as.str);
    opvar_free(id);
    opvar_free(mcoord);
    opvar_free(has_inv);
    opvar_free(varparam);
}


STATIC_VAR const object emptyobject; /* Initialized to zero automatically */

void
spo_object(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_object";
    int nparams = 0;
    long quancnt;
    struct opvar *varparam;
    struct opvar *id, *containment;
    object tmpobj = emptyobject;

    tmpobj.enchantment = -127; /* random */
    tmpobj.charges = -127; /* random */
    tmpobj.special_quality = -127; /* random */
    tmpobj.speflags = 0;
    tmpobj.curse_state = -1; /* random */
    tmpobj.corpsenm = NON_PM;
    tmpobj.name.str = (char *) 0;
    tmpobj.quan = -1;
    tmpobj.buried = 0;
    tmpobj.lit = 0;
    tmpobj.eroded = 0;
    tmpobj.locked = 0;
    tmpobj.trapped = -1;
    tmpobj.elemental_enchantment = -1; /* random */
    tmpobj.exceptionality = -1;  /* random */
    tmpobj.mythic_type = -1; /* random */
    tmpobj.mythic_prefix = -1;  /* random */
    tmpobj.mythic_suffix = -1;  /* random */
    tmpobj.age = -1;  /* not set */
    tmpobj.recharged = 0;
    tmpobj.invis = 0;
    tmpobj.greased = 0;
    tmpobj.broken = 0;
    tmpobj.open = 0;
    tmpobj.coord = SP_COORD_PACK_RANDOM(0);
    tmpobj.indestructible = 0;
    tmpobj.uses_up_key = 0;
    tmpobj.no_pickup = 0;

    if (!OV_pop_i(containment))
        return;

    if (!OV_pop_i(varparam))
        return;

    while ((nparams++ < (SP_O_V_END + 1)) && varparam && (OV_typ(varparam) == SPOVAR_INT)
           && (OV_i(varparam) >= 0) && (OV_i(varparam) < SP_O_V_END)) 
    {
        struct opvar *parm;

        OV_pop(parm);
        switch (OV_i(varparam)) 
        {
        case SP_O_V_NAME:
            if ((OV_typ(parm) == SPOVAR_STRING) && !tmpobj.name.str)
                tmpobj.name.str = dupstr(OV_s(parm));
            break;
        case SP_O_V_CORPSENM:
            if (OV_typ(parm) == SPOVAR_MONST) {
                char monclass = (char)SP_MONST_CLASS(OV_i(parm));
                int monid = (int)SP_MONST_PM(OV_i(parm));

                if (monid >= LOW_PM && monid < NUM_MONSTERS)
                {
                    tmpobj.corpsenm = monid;
                    break; /* we're done! */
                } 
                else 
                {
                    struct permonst *pm = (struct permonst *) 0;

                    if (def_char_to_monclass(monclass) != MAX_MONSTER_CLASSES)
                    {
                        pm = mkclass(def_char_to_monclass(monclass), 0); /* Removed NOGEN */
                    }
                    else
                    {
                        pm = rndmonst();
                    }

                    if (pm)
                        tmpobj.corpsenm = monsndx(pm);
                }
            }
            break;
        case SP_O_V_CURSE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.curse_state = (schar)OV_i(parm);
            break;
        case SP_O_V_SPE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.enchantment = (short)OV_i(parm);
            break;
        case SP_O_V_QUAN:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.quan = (int)OV_i(parm);
            break;
        case SP_O_V_BURIED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.buried = (short)OV_i(parm);
            break;
        case SP_O_V_LIT:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.lit = (short)OV_i(parm);
            break;
        case SP_O_V_ERODED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.eroded = (short)OV_i(parm);
            break;
        case SP_O_V_LOCKED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.locked = (short)OV_i(parm);
            break;
        case SP_O_V_TRAPPED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.trapped = (short)OV_i(parm);
            break;
        case SP_O_V_RECHARGED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.recharged = (short)OV_i(parm);
            break;
        case SP_O_V_INVIS:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.invis = (short)OV_i(parm);
            break;
        case SP_O_V_GREASED:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.greased = (short)OV_i(parm);
            break;
        case SP_O_V_BROKEN:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.broken = (short)OV_i(parm);
            break;
        case SP_O_V_OPEN:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.open = (short)OV_i(parm);
            break;
        case SP_O_V_COORD:
            if (OV_typ(parm) != SPOVAR_COORD)
                panic("no coord for obj?");
            tmpobj.coord = OV_i(parm);
            break;
        case SP_O_V_ENCHANTMENT:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.enchantment = (short)OV_i(parm);
            break;
        case SP_O_V_CHARGES:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.charges = (short)OV_i(parm);
            break;
        case SP_O_V_SPECIAL_QUALITY:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.special_quality = (int)OV_i(parm);
            break;
        case SP_O_V_AGE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.age = (int)OV_i(parm);
            break;
        case SP_O_V_SPEFLAGS:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.speflags = (unsigned long)OV_i(parm);
            break;
        case SP_O_V_MATERIAL:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.material = (int)OV_i(parm);
            break;
        case SP_O_V_ELEMENTAL_ENCHANTMENT:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.elemental_enchantment = (int)OV_i(parm);
            break;
        case SP_O_V_EXCEPTIONALITY:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.exceptionality = (int)OV_i(parm);
            break;
        case SP_O_V_MYTHIC_TYPE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.mythic_type = (schar)OV_i(parm);
            break;
        case SP_O_V_MYTHIC_PREFIX:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.mythic_prefix = (int)OV_i(parm);
            break;
        case SP_O_V_MYTHIC_SUFFIX:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.mythic_suffix = (int)OV_i(parm);
            break;
        case SP_O_V_INDESTRUCTIBLE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.indestructible = (short)OV_i(parm);
            break;
        case SP_O_V_USES_UP_KEY:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.uses_up_key = (short)OV_i(parm);
            break;
        case SP_O_V_NO_PICKUP:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpobj.no_pickup = (short)OV_i(parm);
            break;
        case SP_O_V_KEY_TYPE:
            if (OV_typ(parm) == SPOVAR_OBJ) {
                //char objclass = SP_OBJ_CLASS(OV_i(parm));
                int otyp = (int)SP_OBJ_TYP(OV_i(parm));

                if (otyp > STRANGE_OBJECT && otyp < NUM_OBJECTS) {
                    tmpobj.keyotyp = is_otyp_key(otyp) ? otyp : STRANGE_OBJECT;
                    break; /* we're done! */
                }
                else {
                    tmpobj.keyotyp = STRANGE_OBJECT;
                }
            }
            break;
        case SP_O_V_END:
            nparams = SP_O_V_END + 1;
            break;
        default:
            impossible("OBJECT with unknown variable param type!");
            break;
        }
        opvar_free(parm);
        if (OV_i(varparam) != SP_O_V_END) {
            opvar_free(varparam);
            OV_pop(varparam);
        }
    }

    if (!OV_pop_typ(id, SPOVAR_OBJ))
    {
        panic("no obj type");
        return;
    }

    tmpobj.id = (short)SP_OBJ_TYP(OV_i(id));
    tmpobj.class = (xchar)SP_OBJ_CLASS(OV_i(id));
    tmpobj.containment = (xchar)OV_i(containment);

    quancnt = (tmpobj.id > STRANGE_OBJECT) ? tmpobj.quan : 0;

    do {
        create_object(&tmpobj, coder->croom);
        quancnt--;
    } while ((quancnt > 0) && ((tmpobj.id > STRANGE_OBJECT)
                               && !objects[tmpobj.id].oc_merge));

    Free(tmpobj.name.str);
    opvar_free(varparam);
    opvar_free(id);
    opvar_free(containment);
}

void
spo_lever(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_lever";
    int nparams = 0;
    struct opvar* varparam, *effect_type_opvar, *coord_opvar;
    spllever tmplever = { 0 };

    if (!OV_pop_i(effect_type_opvar))
        return;

    tmplever.lever_effect = (int)OV_i(effect_type_opvar);

    if (!OV_pop_i(varparam))
        return;

    while ((nparams++ < (SP_L_V_END + 1)) && varparam && (OV_typ(varparam) == SPOVAR_INT)
        && (OV_i(varparam) >= 0) && (OV_i(varparam) < SP_L_V_END))
    {
        struct opvar* parm;

        OV_pop(parm);
        switch (OV_i(varparam))
        {
        case SP_L_V_MONSTER:
            if (OV_typ(parm) == SPOVAR_MONST) {
                char monclass = (char)SP_MONST_CLASS(OV_i(parm));
                int monid = (int)SP_MONST_PM(OV_i(parm));

                if (monid >= LOW_PM && monid < NUM_MONSTERS) {
                    tmplever.effect_parameter1 = monid;
                    tmplever.effect_parameter2 = monclass;
                    break; /* we're done! */
                }
                else {
                    tmplever.effect_parameter1 = NON_PM;
                    tmplever.effect_parameter2 = monclass;
                }
            }
            break;
        case SP_L_V_OBJECT:
            if (OV_typ(parm) == SPOVAR_OBJ) {
                char objclass = (char)SP_OBJ_CLASS(OV_i(parm));
                int otyp = (int)SP_OBJ_TYP(OV_i(parm));

                if (otyp > STRANGE_OBJECT && otyp < NUM_OBJECTS) {
                    tmplever.effect_parameter1 = otyp;
                    tmplever.effect_parameter2 = objclass;
                    break; /* we're done! */
                }
                else {
                    tmplever.effect_parameter1 = STRANGE_OBJECT;
                    tmplever.effect_parameter2 = objclass;
                }
            }
            break;
        case SP_L_V_TRAP:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.effect_parameter3 = OV_i(parm);
            break;
        case SP_L_V_SPECIAL_QUALITY:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.effect_parameter4 = OV_i(parm);
            break;
        case SP_L_V_SUBTYPE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.effect_subtype = OV_i(parm);
            break;
        case SP_L_V_FLOOR_SUBTYPE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.effect_subtype2 = OV_i(parm);
            break;
        case SP_L_V_TERRAIN:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.effect_parameter1 = OV_i(parm);
            break;
        case SP_L_V_TERRAIN2:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.effect_parameter3 = OV_i(parm);
            break;
        case SP_L_V_ACTIVE:
            if (OV_typ(parm) == SPOVAR_INT && OV_i(parm) == 1)
                tmplever.lever_flags |= TRAPFLAGS_ACTIVATED;
            break;

        case SP_L_V_CONTINUOUS:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.lever_flags |= TRAPFLAGS_CONTINUOUSLY_SWITCHABLE;
            break;

        case SP_L_V_SWITCHABLE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.lever_flags |= TRAPFLAGS_SWITCHABLE_BETWEEN_STATES;
            break;

        case SP_L_V_EFFECT_FLAG:
            if (OV_typ(parm) == SPOVAR_INT)
                tmplever.effect_flags = (unsigned long)OV_i(parm);
            break;

        case SP_L_V_COORD:
            if (OV_typ(parm) != SPOVAR_COORD)
                panic("no target coord for a lever?");
            tmplever.target_coord = OV_i(parm);
            break;

        case SP_L_V_END:
            nparams = SP_L_V_END + 1;
            break;
        default:
            impossible("LEVER with unknown variable param type!");
            break;
        }
        opvar_free(parm);
        if (OV_i(varparam) != SP_L_V_END) {
            opvar_free(varparam);
            OV_pop(varparam);
        }
    }

    if (!OV_pop_c(coord_opvar))
        return;

    tmplever.coord = OV_i(coord_opvar);

    create_lever(&tmplever, coder->croom);

    opvar_free(varparam);
    opvar_free(effect_type_opvar);
    opvar_free(coord_opvar);

}

void
spo_level_flags(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_level_flags";
    struct opvar *flagdata;
    long lflags;

    if (!OV_pop_i(flagdata))
        return;
    lflags = OV_i(flagdata);

    if (lflags & NOTELEPORT)
        level.flags.noteleport = 1;
    if (lflags & HARDFLOOR)
        level.flags.hardfloor = 1;
    if (lflags & NOMMAP)
        level.flags.nommap = 1;
    if (lflags & SHORTSIGHTED)
        level.flags.shortsighted = 1;
    if (lflags & ARBOREAL)
        level.flags.arboreal = 1;
    if (lflags & DESERT)
    {
        level.flags.desert = 1;
        int i, j;
        for(i = 1; i < COLNO; i++)
            for (j = 0; j < ROWNO; j++)
            {
                if (IS_TREE(levl[i][j].typ))
                {
                    levl[i][j].floortyp = GROUND;
                    levl[i][j].floorsubtyp = get_initial_location_subtype(levl[i][j].floortyp);
                    levl[i][j].floorvartyp = get_initial_location_vartype(levl[i][j].floortyp, levl[i][j].floorsubtyp);
                }
                else if (IS_FLOOR(levl[i][j].typ))
                {
                    levl[i][j].typ = GROUND;
                    levl[i][j].subtyp = get_initial_location_subtype(levl[i][j].typ);
                    levl[i][j].vartyp = get_initial_location_vartype(levl[i][j].typ, levl[i][j].subtyp);
                    levl[i][j].special_quality = 0;
                }
            }
    }
    if (lflags & SWAMPY)
        level.flags.swampy = 1;
    if (lflags & THRONE_ON_GROUND)
        coder->throne_on_ground = 1;
    if (lflags & FOUNTAIN_ON_GRASS)
        coder->fountain_on_grass = 1;
    if (lflags & FOUNTAIN_ON_GROUND)
        coder->fountain_on_ground = 1;
    if (lflags & TREE_ON_GROUND)
        coder->tree_on_ground = 1;
    if (lflags & MAPPING_DOES_NOT_REVEAL_SPECIAL)
        level.flags.mapping_does_not_reveal_special = 1;
    if (lflags & MAZELEVEL)
        level.flags.is_maze_lev = 1;
    if (lflags & PREMAPPED)
        coder->premapped = TRUE;
    if (lflags & SHROUD)
        level.flags.hero_memory = 0;
    if (lflags & GRAVEYARD)
        level.flags.graveyard = 1;
    if (lflags & ICEDPOOLS)
        icedpools = TRUE;
    if (lflags & SOLIDIFY)
        coder->solidify = TRUE;
    if (lflags & CORRMAZE)
        level.flags.corrmaze = TRUE;
    if (lflags & CHECK_INACCESSIBLES)
        coder->check_inaccessibles = TRUE;
    if (lflags & NO_MAP_PADDING)
        coder->no_map_padding = TRUE;

    opvar_free(flagdata);
}

void
spo_tileset(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_tileset";
    struct opvar* tilesetdata;
    long tilesetid = 0;

    if (!OV_pop_i(tilesetdata))
        return;
    tilesetid = OV_i(tilesetdata);

    if (tilesetid < 0 || tilesetid >= MAX_CMAP_TYPES)
    {
        level.flags.has_tileset = 0;
        level.flags.tileset = 0;
    }
    else
    {
        level.flags.has_tileset = 1;
        level.flags.tileset = tilesetid;
    }
    opvar_free(tilesetdata);
}

void
spo_forest(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_forest";
    struct opvar* forest_type, *initialize_type;
    schar forest_id = 0;
    int initialize_forest = 0;

    if (!OV_pop_i(forest_type) || !OV_pop_i(initialize_type))
        return;

    forest_id = (schar)OV_i(forest_type);
    initialize_forest = (int)OV_i(initialize_type);

    if (forest_id < 0 || forest_id >= MAX_FOREST_TYPES)
    {
        level.flags.forest_type = 0;
    }
    else
    {
        level.flags.forest_type = forest_id;
    }

    int x, y;
    for (x = 1; x < COLNO; x++)
    {
        for (y = 0; y < ROWNO; y++)
        {
            if (IS_TREE(levl[x][y].typ))
            {
                levl[x][y].subtyp = get_initial_tree_subtype(level.flags.forest_type);
                levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
                levl[x][y].special_quality = 0;
                if (initialize_forest)
                    initialize_location(&levl[x][y]);
            }
        }
    }

    opvar_free(forest_type);
    opvar_free(initialize_type);
}

void
spo_monster_generation(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_monster_generation";
    struct opvar* prob_var, *monnum_var, *id;
    int probability = 0, monsternum = 0;

    if (!OV_pop_i(prob_var) || !OV_pop_i(monnum_var))
        return;

    probability = (int)OV_i(prob_var);
    monsternum = (int)OV_i(monnum_var);

    short mnum = 0;
    schar mclass = 0;
    char usedclass = 0;
    for (int i = 0; i < monsternum; i++)
    {
        if (!OV_pop_typ(id, SPOVAR_MONST)) {
            panic("no mon type");
            return;
        }

        mnum = (short)SP_MONST_PM(OV_i(id));
        mclass = (schar)SP_MONST_CLASS(OV_i(id));

        if (mclass >= 0)
            usedclass = (char)def_char_to_monclass((char)mclass);
        else
            usedclass = 0;

        level.flags.mon_gen_infos[level.flags.nmgeninfos].probability = (xchar)probability;
        if (!usedclass)
        {
            level.flags.mon_gen_infos[level.flags.nmgeninfos].mnum = NON_PM;
            level.flags.mon_gen_infos[level.flags.nmgeninfos].mclass = RANDOM_CLASS;
        }
        else if (mnum >= LOW_PM)
        {
            level.flags.mon_gen_infos[level.flags.nmgeninfos].mnum = mnum;
            level.flags.mon_gen_infos[level.flags.nmgeninfos].mclass = usedclass;
        }
        else
        {
            level.flags.mon_gen_infos[level.flags.nmgeninfos].mnum = NON_PM;
            level.flags.mon_gen_infos[level.flags.nmgeninfos].mclass = usedclass;
        }

        if (level.flags.nmgeninfos < MAX_MON_GEN_INFOS - 1)
            level.flags.nmgeninfos++;

        opvar_free(id);
    }

    opvar_free(prob_var);
    opvar_free(monnum_var);
}

void
spo_boundary_type(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_boundary_type";
    struct opvar* bt_opvar;
    schar boundary_location_type = 0;

    if (!OV_pop_i(bt_opvar))
        return;
    boundary_location_type = (schar)OV_i(bt_opvar);

    if (boundary_location_type < 0 || boundary_location_type >= MAX_LEVTYPE || !IS_FLOOR(boundary_location_type))
    {
        level.flags.boundary_type = ROOM;
    }
    else
    {
        level.flags.boundary_type = boundary_location_type;
    }
    opvar_free(bt_opvar);
}

void
spo_initlevel(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_initlevel";
    lev_init init_lev;
    struct opvar *init_style, *fg, *bg, *smoothed, *joined, *lit, *walled,
        *filling;

    if (!OV_pop_i(fg) || !OV_pop_i(bg) || !OV_pop_i(smoothed)
        || !OV_pop_i(joined) || !OV_pop_i(lit) || !OV_pop_i(walled)
        || !OV_pop_i(filling) || !OV_pop_i(init_style))
        return;

    splev_init_present = TRUE;

    init_lev.init_style = (xchar)OV_i(init_style);
    init_lev.fg = (char)OV_i(fg);
    init_lev.bg = (char)OV_i(bg);
    init_lev.smoothed = (boolean)OV_i(smoothed);
    init_lev.joined = (boolean)OV_i(joined);
    init_lev.lit = (xchar)OV_i(lit);
    init_lev.walled = (xchar)OV_i(walled);
    init_lev.filling = (schar)OV_i(filling);
    init_lev.tree_on_ground = coder->tree_on_ground;
    init_lev.throne_on_ground = coder->throne_on_ground;
    init_lev.fountain_on_ground = coder->fountain_on_ground;
    init_lev.fountain_on_grass = coder->fountain_on_grass;
    coder->lvl_is_joined = (int)OV_i(joined);

    splev_initlev(&init_lev);

    opvar_free(init_style);
    opvar_free(fg);
    opvar_free(bg);
    opvar_free(smoothed);
    opvar_free(joined);
    opvar_free(lit);
    opvar_free(walled);
    opvar_free(filling);
}

void
spo_engraving(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_engraving";
    struct opvar *etyp, *txt, *ecoord;
    xchar x, y;

    if (!OV_pop_i(etyp) || !OV_pop_s(txt) || !OV_pop_c(ecoord))
        return;

    get_location_coord(&x, &y, FLOORLOC, coder->croom, OV_i(ecoord));
    make_engr_at(x, y, OV_s(txt), 0L, (xchar)OV_i(etyp), ENGR_FLAGS_NONE);

    opvar_free(etyp);
    opvar_free(txt);
    opvar_free(ecoord);
}

void
spo_mineralize(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_mineralize";
    struct opvar *kelp_pool, *kelp_moat, *gold_prob, *gem_prob;

    if (!OV_pop_i(gem_prob) || !OV_pop_i(gold_prob) || !OV_pop_i(kelp_moat)
        || !OV_pop_i(kelp_pool))
        return;

    mineralize((int)OV_i(kelp_pool), (int)OV_i(kelp_moat), (int)OV_i(gold_prob),
        (int)OV_i(gem_prob), TRUE);

    opvar_free(gem_prob);
    opvar_free(gold_prob);
    opvar_free(kelp_moat);
    opvar_free(kelp_pool);
}

void
spo_room(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_room";

    if (coder->n_subroom > MAX_NESTED_ROOMS) {
        panic("Too deeply nested rooms?!");
        return;
    } else {
        struct opvar *rflags, *h, *w, *yalign, *xalign, *y, *x, *rlit,
            *chance, *rtype, * rfloorsubtyp, *rfloortyp, * rtileset, * rdecotyp, *mtype;
        room tmproom;
        struct mkroom *tmpcr;

        if (!OV_pop_i(h) || !OV_pop_i(w) || !OV_pop_i(y) || !OV_pop_i(x)
            || !OV_pop_i(yalign) || !OV_pop_i(xalign) || !OV_pop_i(rflags) || !OV_pop_i(rfloortyp) || !OV_pop_i(rfloorsubtyp) || !OV_pop_i(rtileset) || !OV_pop_i(rdecotyp) || !OV_pop(mtype)
            || !OV_pop_i(rlit) || !OV_pop_i(chance) || !OV_pop_i(rtype))
            return;

        int flmt = (int)OV_i(rfloortyp);
        int flt = (int)OV_i(rfloorsubtyp);
        short decotyp = (short)OV_i(rdecotyp);
        tmproom.x = (xchar)OV_i(x);
        tmproom.y = (xchar)OV_i(y);
        tmproom.w = (xchar)OV_i(w);
        tmproom.h = (xchar)OV_i(h);
        tmproom.xalign = (xchar)OV_i(xalign);
        tmproom.yalign = (xchar)OV_i(yalign);
        tmproom.rtype = (xchar)OV_i(rtype);
        tmproom.chance = (xchar)OV_i(chance);
        tmproom.rlit = (xchar)OV_i(rlit);
        tmproom.filled = (xchar)(OV_i(rflags) & (1 << 0));
        /*tmproom.irregular = (OV_i(rflags) & (1 << 1));*/
        tmproom.joined = (xchar)!(OV_i(rflags) & (1 << 2));
        tmproom.rfloortyp = (xchar)(flmt >= 0 && IS_FLOOR(flmt) ? flmt : ROOM); /* Rooms have default flooring as ROOM, rather than nothing */
        tmproom.rfloorsubtyp = (xchar)(IS_FLOOR(tmproom.rfloortyp) && flt >= 0 ? flt : 0);
        tmproom.tileset = (short)OV_i(rtileset);
        tmproom.decotyp = decotyp < 0 ? (coder->croom ? 0 : 1) : decotyp;
        tmproom.mtype = NON_PM;
        if (OV_typ(mtype) == SPOVAR_MONST)
        {
            //char monclass = SP_MONST_CLASS(OV_i(mtype));
            int monid = (int)SP_MONST_PM(OV_i(mtype));

            if (monid >= LOW_PM && monid < NUM_MONSTERS)
            {
                tmproom.mtype = monid;
            }
            else
            {
                tmproom.mtype = NON_PM;
            }
        }

        opvar_free(x);
        opvar_free(y);
        opvar_free(w);
        opvar_free(h);
        opvar_free(xalign);
        opvar_free(yalign);
        opvar_free(rtype);
        opvar_free(rfloortyp);
        opvar_free(rfloorsubtyp);
        opvar_free(rtileset);
        opvar_free(rdecotyp);
        opvar_free(mtype);
        opvar_free(chance);
        opvar_free(rlit);
        opvar_free(rflags);

        if (!coder->failed_room[coder->n_subroom - 1]) {
            tmpcr = build_room(&tmproom, coder->croom);
            if (tmpcr) {
                coder->tmproomlist[coder->n_subroom] = tmpcr;
                coder->failed_room[coder->n_subroom] = FALSE;
                coder->n_subroom++;
                return;
            }
        } /* failed to create parent room, so fail this too */
    }
    coder->tmproomlist[coder->n_subroom] = (struct mkroom *) 0;
    coder->failed_room[coder->n_subroom] = TRUE;
    coder->n_subroom++;
}

void
spo_endroom(coder)
struct sp_coder *coder;
{
    if (coder->n_subroom > 1) {
        coder->n_subroom--;
        coder->tmproomlist[coder->n_subroom] = NULL;
        coder->failed_room[coder->n_subroom] = TRUE;
    } else {
        /* no subroom, get out of top-level room */
        /* Need to ensure xstart/ystart/xsize/ysize have something sensible,
           in case there's some stuff to be created outside the outermost
           room,
           and there's no MAP.
        */
        if (xsize <= 1 && ysize <= 1) {
            xstart = 1;
            ystart = 0;
            xsize = COLNO - 1;
            ysize = ROWNO;
        }
    }
}

void
spo_stair(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_stair";
    xchar x, y;
    struct opvar *up, *scoord;
    struct trap *badtrap;

    if (!OV_pop_i(up) || !OV_pop_c(scoord))
        return;

    get_location_coord(&x, &y, DRY_NO_ICE, coder->croom, OV_i(scoord));
    if ((badtrap = t_at(x, y)) != 0)
        deltrap(badtrap);
    mkstairs(x, y, (char) OV_i(up), coder->croom, STAIRCASE_NORMAL);
    SpLev_Map[x][y] = 1;

    opvar_free(scoord);
    opvar_free(up);
}

void
spo_ladder(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_ladder";
    xchar x, y;
    struct opvar *up, *lcoord;

    if (!OV_pop_i(up) || !OV_pop_c(lcoord))
        return;

    get_location_coord(&x, &y, DRY_NO_ICE, coder->croom, OV_i(lcoord));

    if (IS_FLOOR(levl[x][y].typ))
    {
        levl[x][y].floortyp = levl[x][y].typ;
        levl[x][y].floorsubtyp = levl[x][y].subtyp;
        levl[x][y].floorvartyp = levl[x][y].vartyp;
    }
    else
    {
        levl[x][y].floortyp = location_type_definitions[LADDER].initial_floor_type;
        levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
        levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
    }

    levl[x][y].typ = LADDER;
    levl[x][y].subtyp = get_initial_location_subtype(levl[x][y].typ);
    levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
    levl[x][y].special_quality = 0;

    SpLev_Map[x][y] = 1;
    if (OV_i(up)) {
        xupladder = x;
        yupladder = y;
        levl[x][y].ladder = LA_UP;
    } else {
        xdnladder = x;
        ydnladder = y;
        levl[x][y].ladder = LA_DOWN;
    }
    opvar_free(lcoord);
    opvar_free(up);
}

void
spo_grave(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_grave";
    struct opvar *gcoord, *typ, *txt;
    schar x, y;

    if (!OV_pop_i(typ) || !OV_pop_s(txt) || !OV_pop_c(gcoord))
        return;

    get_location_coord(&x, &y, FLOORLOC, coder->croom, OV_i(gcoord));

    if (isok(x, y) && !t_at(x, y))
    {
        if (IS_FLOOR(levl[x][y].typ))
        {
            levl[x][y].floortyp = levl[x][y].typ;
            levl[x][y].floorsubtyp = levl[x][y].subtyp;
            levl[x][y].floorvartyp = levl[x][y].vartyp;
        }
        else
        {
            levl[x][y].floortyp = location_type_definitions[GRAVE].initial_floor_type;
            levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
            levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
        }

        levl[x][y].typ = GRAVE;
        levl[x][y].subtyp = get_initial_location_subtype(levl[x][y].typ);
        levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
        levl[x][y].special_quality = 0;

        switch (OV_i(typ)) {
        case 2:
            make_grave(x, y, OV_s(txt), TRUE);
            break;
        case 1:
            make_grave(x, y, NULL, TRUE);
            break;
        default:
            del_engr_at(x, y);
            break;
        }
    }

    opvar_free(gcoord);
    opvar_free(typ);
    opvar_free(txt);
}

void
spo_signpost(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_signpost";
    struct opvar* gcoord, * typ, * txt, *subtyp;
    schar x, y;

    if (!OV_pop_i(subtyp) || !OV_pop_i(typ) || !OV_pop_s(txt) || !OV_pop_c(gcoord))
        return;

    get_location_coord(&x, &y, DRY, coder->croom, OV_i(gcoord));

    if (isok(x, y) && !t_at(x, y))
    {
        if (IS_FLOOR(levl[x][y].typ))
        {
            levl[x][y].floortyp = levl[x][y].typ;
            levl[x][y].floorsubtyp = levl[x][y].subtyp;
            levl[x][y].floorvartyp = levl[x][y].vartyp;
        }
        else
        {
            levl[x][y].floortyp = location_type_definitions[SIGNPOST].initial_floor_type;
            levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
            levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
        }

        levl[x][y].typ = SIGNPOST;
        levl[x][y].subtyp = (schar)OV_i(subtyp);
        levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
        levl[x][y].special_quality = 0;

        switch (OV_i(typ)) {
        case 2:
            make_signpost(x, y, OV_s(txt), TRUE);
            break;
        case 1:
            make_signpost(x, y, NULL, TRUE);
            break;
        default:
            del_engr_at(x, y);
            break;
        }
    }

    opvar_free(gcoord);
    opvar_free(typ);
    opvar_free(txt);
    opvar_free(subtyp);
}

void
spo_brazier(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_brazier";
    struct opvar* gcoord, * lamplit, * subtyp;
    schar x, y;

    if (!OV_pop_i(subtyp) || !OV_pop_i(lamplit) || !OV_pop_c(gcoord))
        return;

    get_location_coord(&x, &y, DRY, coder->croom, OV_i(gcoord));

    if (isok(x, y) && !t_at(x, y))
    {
        if (IS_FLOOR(levl[x][y].typ))
        {
            levl[x][y].floortyp = levl[x][y].typ;
            levl[x][y].floorsubtyp = levl[x][y].subtyp;
            levl[x][y].floorvartyp = levl[x][y].vartyp;
        }
        else
        {
            levl[x][y].floortyp = location_type_definitions[BRAZIER].initial_floor_type;
            levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
            levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
        }

        levl[x][y].typ = BRAZIER;
        long stval = OV_i(subtyp);
        if (stval >= 0)
            levl[x][y].subtyp = (schar)stval;
        else
            levl[x][y].subtyp = get_initial_location_subtype(levl[x][y].typ);

        levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
        levl[x][y].special_quality = 0;

        long val = OV_i(lamplit);
        if (val >= 0)
            levl[x][y].lamplit = (unsigned int)val;
        else
            levl[x][y].lamplit = rn2(2);

        if (!levl[x][y].lamplit)
            levl[x][y].flags |= L_INITIALLY_UNLIT;
    }

    opvar_free(gcoord);
    opvar_free(subtyp);
    opvar_free(lamplit);
}

void
spo_tree(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_tree";
    struct opvar* gcoord, * subtyp, * foresttyp;
    schar x, y;

    if (!OV_pop_i(subtyp) || !OV_pop_i(foresttyp) || !OV_pop_c(gcoord))
        return;

    get_location_coord(&x, &y, DRY_NO_ICE, coder->croom, OV_i(gcoord));

    if (isok(x, y) && !t_at(x, y))
    {
        if (IS_FLOOR(levl[x][y].typ))
        {
            levl[x][y].floortyp = levl[x][y].typ;
            levl[x][y].floorsubtyp = levl[x][y].subtyp;
            levl[x][y].floorvartyp = levl[x][y].vartyp;
        }
        else
        {
            levl[x][y].floortyp = location_type_definitions[TREE].initial_floor_type;
            levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
            levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
        }

        levl[x][y].typ = TREE;
        int forest_id = (int)OV_i(foresttyp);
        long val = OV_i(subtyp);
        if (val >= 0)
            levl[x][y].subtyp = (schar)val;
        else
            levl[x][y].subtyp = get_initial_tree_subtype(forest_id >= 0 ? forest_id : -1);

        levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
        levl[x][y].special_quality = 0;
        initialize_location(&levl[x][y]);
    }

    opvar_free(gcoord);
    opvar_free(subtyp);
}

void
spo_altar(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_altar";
    struct opvar *al, *shrine, *acoord, *mtype, *subtype;
    altar tmpaltar;

    if (!OV_pop_i(al) || !OV_pop_i(shrine) || !OV_pop_i(subtype) || !OV_pop(mtype) || !OV_pop_c(acoord))
        return;

    tmpaltar.coord = OV_i(acoord);
    tmpaltar.align = (aligntyp)OV_i(al);
    tmpaltar.shrine = (xchar)OV_i(shrine);
    tmpaltar.subtyp = (xchar)OV_i(subtype);
    tmpaltar.mtype = NON_PM;
    if (OV_typ(mtype) == SPOVAR_MONST) 
    {
        //char monclass = SP_MONST_CLASS(OV_i(mtype));
        int monid = (int)SP_MONST_PM(OV_i(mtype));

        if (monid >= LOW_PM && monid < NUM_MONSTERS) 
        {
            tmpaltar.mtype = monid;
        }
        else 
        {
            tmpaltar.mtype = NON_PM;
        }
    }

    create_altar(&tmpaltar, coder->croom);

    opvar_free(acoord);
    opvar_free(shrine);
    opvar_free(subtype);
    opvar_free(al);
    opvar_free(mtype);
}

void
spo_trap(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_trap";
    struct opvar *type;
    struct opvar *tcoord;
    spltrap tmptrap;

    if (!OV_pop_i(type) || !OV_pop_c(tcoord))
        return;

    tmptrap.coord = OV_i(tcoord);
    tmptrap.type = (xchar)OV_i(type);

    create_trap(&tmptrap, coder->croom);
    opvar_free(tcoord);
    opvar_free(type);
}

void
spo_gold(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_gold";
    struct opvar *gcoord, *amt;
    schar x, y;
    long amount;

    if (!OV_pop_c(gcoord) || !OV_pop_i(amt))
        return;
    amount = OV_i(amt);
    get_location_coord(&x, &y, DRY, coder->croom, OV_i(gcoord));
    if (amount == -1)
        amount = rnd(200);
    mkgold(amount, x, y);
    opvar_free(gcoord);
    opvar_free(amt);
}

void
spo_corridor(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_corridor";
    struct opvar *deswall, *desdoor, *desroom, *srcwall, *srcdoor, *srcroom;
    corridor tc;

    if (!OV_pop_i(deswall) || !OV_pop_i(desdoor) || !OV_pop_i(desroom)
        || !OV_pop_i(srcwall) || !OV_pop_i(srcdoor) || !OV_pop_i(srcroom))
        return;

    tc.src.room = (xchar)OV_i(srcroom);
    tc.src.door = (xchar)OV_i(srcdoor);
    tc.src.wall = (xchar)OV_i(srcwall);
    tc.dest.room = (xchar)OV_i(desroom);
    tc.dest.door = (xchar)OV_i(desdoor);
    tc.dest.wall = (xchar)OV_i(deswall);

    create_corridor(&tc);

    opvar_free(deswall);
    opvar_free(desdoor);
    opvar_free(desroom);
    opvar_free(srcwall);
    opvar_free(srcdoor);
    opvar_free(srcroom);
}

struct opvar *
selection_opvar(nbuf)
char *nbuf;
{
    struct opvar *ov;
    char buf[(COLNO * ROWNO) + 1];

    if (!nbuf) {
        (void) memset(buf, 1, sizeof(buf));
        buf[(COLNO * ROWNO)] = '\0';
        ov = opvar_new_str(buf);
    } else {
        ov = opvar_new_str(nbuf);
    }
    ov->spovartyp = SPOVAR_SEL;
    return ov;
}

xchar
selection_getpoint(x, y, ov)
int x, y;
struct opvar *ov;
{
    if (!ov || ov->spovartyp != SPOVAR_SEL)
        return 0;
    if (x < 0 || y < 0 || x >= COLNO || y >= ROWNO)
        return 0;

    return (ov->vardata.str[COLNO * y + x] - 1);
}

void
selection_setpoint(x, y, ov, c)
int x, y;
struct opvar *ov;
xchar c;
{
    if (!ov || ov->spovartyp != SPOVAR_SEL)
        return;
    if (x < 0 || y < 0 || x >= COLNO || y >= ROWNO)
        return;

    ov->vardata.str[COLNO * y + x] = (char) (c + 1);
}

struct opvar *
selection_not(s)
struct opvar *s;
{
    struct opvar *ov;
    int x, y;

    ov = selection_opvar((char *) 0);
    if (!ov)
        return NULL;

    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (!selection_getpoint(x, y, s))
                selection_setpoint(x, y, ov, 1);

    return ov;
}

struct opvar *
selection_logical_oper(s1, s2, oper)
struct opvar *s1, *s2;
char oper;
{
    struct opvar *ov;
    int x, y;

    ov = selection_opvar((char *) 0);
    if (!ov)
        return NULL;

    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++) {
            switch (oper) {
            default:
            case '|':
                if (selection_getpoint(x, y, s1)
                    || selection_getpoint(x, y, s2))
                    selection_setpoint(x, y, ov, 1);
                break;
            case '&':
                if (selection_getpoint(x, y, s1)
                    && selection_getpoint(x, y, s2))
                    selection_setpoint(x, y, ov, 1);
                break;
            }
        }

    return ov;
}

struct opvar *
selection_filter_mapchar(ov, mc)
struct opvar *ov;
struct opvar *mc;
{
    int x, y;
    schar mapc;
    xchar lit;
    struct opvar *ret = selection_opvar((char *) 0);

    if (!ov || !mc || !ret)
        return NULL;
    mapc = (schar)SP_MAPCHAR_TYP(OV_i(mc));
    lit = (xchar)SP_MAPCHAR_LIT(OV_i(mc));
    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (selection_getpoint(x, y, ov) && (levl[x][y].typ == mapc)) {
                switch (lit) {
                default:
                case -2:
                    selection_setpoint(x, y, ret, 1);
                    break;
                case -1:
                    selection_setpoint(x, y, ret, rn2(2));
                    break;
                case 0:
                case 1:
                    if (levl[x][y].lit == lit)
                        selection_setpoint(x, y, ret, 1);
                    break;
                }
            }
    return ret;
}

void
selection_filter_percent(ov, percent)
struct opvar *ov;
int percent;
{
    int x, y;

    if (!ov)
        return;
    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (selection_getpoint(x, y, ov) && (rn2(100) >= percent))
                selection_setpoint(x, y, ov, 0);
}

STATIC_OVL int
selection_rndcoord(ov, x, y, removeit)
struct opvar *ov;
schar *x, *y;
boolean removeit;
{
    int idx = 0;
    int c;
    int dx, dy;

    for (dx = 0; dx < COLNO; dx++)
        for (dy = 0; dy < ROWNO; dy++)
            if (isok(dx, dy) && selection_getpoint(dx, dy, ov))
                idx++;

    if (idx) {
        c = rn2(idx);
        for (dx = 0; dx < COLNO; dx++)
            for (dy = 0; dy < ROWNO; dy++)
                if (isok(dx, dy) && selection_getpoint(dx, dy, ov)) {
                    if (!c) {
                        *x = dx;
                        *y = dy;
                        if (removeit) selection_setpoint(dx, dy, ov, 0);
                        return 1;
                    }
                    c--;
                }
    }
    *x = *y = -1;
    return 0;
}

void
selection_do_grow(ov, dir)
struct opvar *ov;
int dir;
{
    int x, y;
    char tmp[COLNO][ROWNO];

    if (ov->spovartyp != SPOVAR_SEL)
        return;
    if (!ov)
        return;

    (void) memset(tmp, 0, sizeof tmp);

    for (x = 1; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++) {
            /* note:  dir is a mask of multiple directions, but the only
               way to specify diagonals is by including the two adjacent
               orthogonal directions, which effectively specifies three-
               way growth [WEST|NORTH => WEST plus WEST|NORTH plus NORTH] */
            if (((dir & W_WEST) && selection_getpoint(x + 1, y, ov))
                || (((dir & (W_WEST | W_NORTH)) == (W_WEST | W_NORTH))
                    && selection_getpoint(x + 1, y + 1, ov))
                || ((dir & W_NORTH) && selection_getpoint(x, y + 1, ov))
                || (((dir & (W_NORTH | W_EAST)) == (W_NORTH | W_EAST))
                    && selection_getpoint(x - 1, y + 1, ov))
                || ((dir & W_EAST) && selection_getpoint(x - 1, y, ov))
                || (((dir & (W_EAST | W_SOUTH)) == (W_EAST | W_SOUTH))
                    && selection_getpoint(x - 1, y - 1, ov))
                || ((dir & W_SOUTH) && selection_getpoint(x, y - 1, ov))
                ||  (((dir & (W_SOUTH | W_WEST)) == (W_SOUTH | W_WEST))
                     && selection_getpoint(x + 1, y - 1, ov))) {
                tmp[x][y] = 1;
            }
        }

    for (x = 1; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (tmp[x][y])
                selection_setpoint(x, y, ov, 1);
}

STATIC_VAR int FDECL((*selection_flood_check_func), (int, int));
STATIC_VAR schar floodfillchk_match_under_typ;

void
set_selection_floodfillchk(f)
int FDECL((*f), (int, int));
{
    selection_flood_check_func = f;
}

STATIC_OVL int
floodfillchk_match_under(x,y)
int x,y;
{
    return (floodfillchk_match_under_typ == levl[x][y].typ);
}

STATIC_OVL int
floodfillchk_match_accessible(x, y)
int x, y;
{
    return (ACCESSIBLE(levl[x][y].typ)
            || levl[x][y].typ == SDOOR
            || levl[x][y].typ == SCORR);
}

/* check whethere <x,y> is already in xs[],ys[] */
STATIC_OVL boolean
sel_flood_havepoint(x, y, xs, ys, n)
int x, y;
xchar xs[], ys[];
int n;
{
    xchar xx = (xchar) x, yy = (xchar) y;

    while (n > 0) {
        --n;
        if (xs[n] == xx && ys[n] == yy)
            return TRUE;
    }
    return FALSE;
}

void
selection_floodfill(ov, x, y, diagonals)
struct opvar *ov;
int x, y;
boolean diagonals;
{
    static const char nhFunc[] = "selection_floodfill";
    struct opvar *tmp = selection_opvar((char *) 0);
#define SEL_FLOOD_STACK (COLNO * ROWNO)
#define SEL_FLOOD(nx, ny) \
    do {                                      \
        if (idx < SEL_FLOOD_STACK) {          \
            dx[idx] = (nx);                   \
            dy[idx] = (ny);                   \
            idx++;                            \
        } else                                \
            panic(floodfill_stack_overrun);   \
    } while (0)
#define SEL_FLOOD_CHKDIR(mx, my, sel) \
    do {                                                        \
        if (isok((mx), (my))                                    \
            && (*selection_flood_check_func)((mx), (my))        \
            && !selection_getpoint((mx), (my), (sel))           \
            && !sel_flood_havepoint((mx), (my), dx, dy, idx))   \
            SEL_FLOOD((mx), (my));                              \
    } while (0)
    static const char floodfill_stack_overrun[] = "floodfill stack overrun";
    int idx = 0;
    xchar dx[SEL_FLOOD_STACK];
    xchar dy[SEL_FLOOD_STACK];

    if (selection_flood_check_func == (int FDECL((*), (int, int))) 0) {
        opvar_free(tmp);
        return;
    }
    SEL_FLOOD(x, y);
    do {
        idx--;
        x = dx[idx];
        y = dy[idx];
        if (isok(x, y)) {
            selection_setpoint(x, y, ov, 1);
            selection_setpoint(x, y, tmp, 1);
        }
        SEL_FLOOD_CHKDIR((x + 1), y, tmp);
        SEL_FLOOD_CHKDIR((x - 1), y, tmp);
        SEL_FLOOD_CHKDIR(x, (y + 1), tmp);
        SEL_FLOOD_CHKDIR(x, (y - 1), tmp);
        if (diagonals) {
            SEL_FLOOD_CHKDIR((x + 1), (y + 1), tmp);
            SEL_FLOOD_CHKDIR((x - 1), (y - 1), tmp);
            SEL_FLOOD_CHKDIR((x - 1), (y + 1), tmp);
            SEL_FLOOD_CHKDIR((x + 1), (y - 1), tmp);
        }
    } while (idx > 0);
#undef SEL_FLOOD
#undef SEL_FLOOD_STACK
#undef SEL_FLOOD_CHKDIR
    opvar_free(tmp);
}

/* McIlroy's Ellipse Algorithm */
void
selection_do_ellipse(ov, xc, yc, a, b, filled)
struct opvar *ov;
int xc, yc, a, b, filled;
{ /* e(x,y) = b^2*x^2 + a^2*y^2 - a^2*b^2 */
    int x = 0, y = b;
    long a2 = (long) a * a, b2 = (long) b * b;
    long crit1 = -(a2 / 4 + a % 2 + b2);
    long crit2 = -(b2 / 4 + b % 2 + a2);
    long crit3 = -(b2 / 4 + b % 2);
    long t = -a2 * y; /* e(x+1/2,y-1/2) - (a^2+b^2)/4 */
    long dxt = 2 * b2 * x, dyt = -2 * a2 * y;
    long d2xt = 2 * b2, d2yt = 2 * a2;
    int width = 1;
    int i;

    if (!ov)
        return;

    filled = !filled;

    if (!filled) {
        while (y >= 0 && x <= a) {
            selection_setpoint(xc + x, yc + y, ov, 1);
            if (x != 0 || y != 0)
                selection_setpoint(xc - x, yc - y, ov, 1);
            if (x != 0 && y != 0) {
                selection_setpoint(xc + x, yc - y, ov, 1);
                selection_setpoint(xc - x, yc + y, ov, 1);
            }
            if (t + b2 * x <= crit1       /* e(x+1,y-1/2) <= 0 */
                || t + a2 * y <= crit3) { /* e(x+1/2,y) <= 0 */
                x++;
                dxt += d2xt;
                t += dxt;
            } else if (t - a2 * y > crit2) { /* e(x+1/2,y-1) > 0 */
                y--;
                dyt += d2yt;
                t += dyt;
            } else {
                x++;
                dxt += d2xt;
                t += dxt;
                y--;
                dyt += d2yt;
                t += dyt;
            }
        }
    } else {
        while (y >= 0 && x <= a) {
            if (t + b2 * x <= crit1       /* e(x+1,y-1/2) <= 0 */
                || t + a2 * y <= crit3) { /* e(x+1/2,y) <= 0 */
                x++;
                dxt += d2xt;
                t += dxt;
                width += 2;
            } else if (t - a2 * y > crit2) { /* e(x+1/2,y-1) > 0 */
                for (i = 0; i < width; i++)
                    selection_setpoint(xc - x + i, yc - y, ov, 1);
                if (y != 0)
                    for (i = 0; i < width; i++)
                        selection_setpoint(xc - x + i, yc + y, ov, 1);
                y--;
                dyt += d2yt;
                t += dyt;
            } else {
                for (i = 0; i < width; i++)
                    selection_setpoint(xc - x + i, yc - y, ov, 1);
                if (y != 0)
                    for (i = 0; i < width; i++)
                        selection_setpoint(xc - x + i, yc + y, ov, 1);
                x++;
                dxt += d2xt;
                t += dxt;
                y--;
                dyt += d2yt;
                t += dyt;
                width += 2;
            }
        }
    }
}

/* distance from line segment (x1,y1, x2,y2) to point (x3,y3) */
long
line_dist_coord(x1, y1, x2, y2, x3, y3)
long x1, y1, x2, y2, x3, y3;
{
    long px = x2 - x1;
    long py = y2 - y1;
    long s = px * px + py * py;
    long x, y, dx, dy, dist = 0;
    float lu = 0;

    if (x1 == x2 && y1 == y2)
        return (long)isqrt(dist2((int)x1, (int)y1, (int)x3, (int)y3));

    lu = ((x3 - x1) * px + (y3 - y1) * py) / (float) s;
    if (lu > 1)
        lu = 1;
    else if (lu < 0)
        lu = 0;

    x = x1 + lu * px;
    y = y1 + lu * py;
    dx = x - x3;
    dy = y - y3;
    dist = (long)isqrt((int)(dx * dx + dy * dy));

    return dist;
}

void
selection_do_gradient(ov, x, y, x2, y2, gtyp, mind, maxd, limit)
struct opvar *ov;
long x, y, x2, y2, gtyp, mind, maxd, limit;
{
    long dx, dy, dofs;

    if (mind > maxd) {
        long tmp = mind;
        mind = maxd;
        maxd = tmp;
    }

    dofs = maxd - mind;
    if (dofs < 1)
        dofs = 1;

    switch (gtyp) {
    default:
    case SEL_GRADIENT_RADIAL: {
        for (dx = 0; dx < COLNO; dx++)
            for (dy = 0; dy < ROWNO; dy++) {
                long d0 = line_dist_coord(x, y, x2, y2, dx, dy);
                if (d0 >= mind && (!limit || d0 <= maxd)) {
                    if (d0 - mind > (long)rn2((int)dofs))
                        selection_setpoint((int)dx, (int)dy, ov, 1);
                }
            }
        break;
    }
    case SEL_GRADIENT_SQUARE: {
        for (dx = 0; dx < COLNO; dx++)
            for (dy = 0; dy < ROWNO; dy++) {
                long d1 = line_dist_coord(x, y, x2, y2, x, dy);
                long d2 = line_dist_coord(x, y, x2, y2, dx, y);
                long d3 = line_dist_coord(x, y, x2, y2, x2, dy);
                long d4 = line_dist_coord(x, y, x2, y2, dx, y2);
                long d5 = line_dist_coord(x, y, x2, y2, dx, dy);
                long d0 = min(d5, min(max(d1, d2), max(d3, d4)));

                if (d0 >= mind && (!limit || d0 <= maxd)) {
                    if (d0 - mind > (long)rn2((int)dofs))
                        selection_setpoint((int)dx, (int)dy, ov, 1);
                }
            }
        break;
    } /*case*/
    } /*switch*/
}

/* bresenham line algo */
void
selection_do_line(x1, y1, x2, y2, ov)
schar x1, y1, x2, y2;
struct opvar *ov;
{
    int d0, dx, dy, ai, bi, xi, yi;

    if (x1 < x2) {
        xi = 1;
        dx = x2 - x1;
    } else {
        xi = -1;
        dx = x1 - x2;
    }

    if (y1 < y2) {
        yi = 1;
        dy = y2 - y1;
    } else {
        yi = -1;
        dy = y1 - y2;
    }

    selection_setpoint(x1, y1, ov, 1);

    if (dx > dy) {
        ai = (dy - dx) * 2;
        bi = dy * 2;
        d0 = bi - dx;
        do {
            if (d0 >= 0) {
                y1 += yi;
                d0 += ai;
            } else
                d0 += bi;
            x1 += xi;
            selection_setpoint(x1, y1, ov, 1);
        } while (x1 != x2);
    } else {
        ai = (dx - dy) * 2;
        bi = dx * 2;
        d0 = bi - dy;
        do {
            if (d0 >= 0) {
                x1 += xi;
                d0 += ai;
            } else
                d0 += bi;
            y1 += yi;
            selection_setpoint(x1, y1, ov, 1);
        } while (y1 != y2);
    }
}

void
selection_do_randline(x1, y1, x2, y2, rough, rec, ov)
schar x1, y1, x2, y2, rough, rec;
struct opvar *ov;
{
    int mx, my;
    int dx, dy;

    if (rec < 1) {
        return;
    }

    if ((x2 == x1) && (y2 == y1)) {
        selection_setpoint(x1, y1, ov, 1);
        return;
    }

    if (rough > max(abs(x2 - x1), abs(y2 - y1)))
        rough = max(abs(x2 - x1), abs(y2 - y1));

    if (rough < 2) {
        mx = ((x1 + x2) / 2);
        my = ((y1 + y2) / 2);
    } else {
        do {
            dx = rn2(rough) - (rough / 2);
            dy = rn2(rough) - (rough / 2);
            mx = ((x1 + x2) / 2) + dx;
            my = ((y1 + y2) / 2) + dy;
        } while ((mx > COLNO - 1 || mx < 0 || my < 0 || my > ROWNO - 1));
    }

    selection_setpoint(mx, my, ov, 1);

    rough = (rough * 2) / 3;

    rec--;

    selection_do_randline(x1, y1, mx, my, rough, rec, ov);
    selection_do_randline(mx, my, x2, y2, rough, rec, ov);
}

void
selection_iterate(ov, func, arg)
struct opvar *ov;
select_iter_func func;
genericptr_t arg;
{
    int x, y;

    /* yes, this is very naive, but it's not _that_ expensive. */
    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (selection_getpoint(x, y, ov))
                (*func)(x, y, arg);
}

void
selection_iterate2(ov, func, arg, arg2)
struct opvar* ov;
select_iter_func2 func;
genericptr_t arg, arg2;
{
    int x, y;

    /* yes, this is very naive, but it's not _that_ expensive. */
    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (selection_getpoint(x, y, ov))
                (*func)(x, y, arg, arg2);
}

void
selection_iterate3(ov, func, arg, arg2, arg3)
struct opvar* ov;
select_iter_func3 func;
genericptr_t arg, arg2, arg3;
{
    int x, y;

    /* yes, this is very naive, but it's not _that_ expensive. */
    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (selection_getpoint(x, y, ov))
                (*func)(x, y, arg, arg2, arg3);
}

#if 0 /* UNUSED */
void
selection_iterate4(ov, func, arg, arg2, arg3, arg4)
struct opvar* ov;
select_iter_func4 func;
genericptr_t arg, arg2, arg3, arg4;
{
    int x, y;

    /* yes, this is very naive, but it's not _that_ expensive. */
    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (selection_getpoint(x, y, ov))
                (*func)(x, y, arg, arg2, arg3, arg4);
}
#endif

void
selection_iterate5(ov, func, arg, arg2, arg3, arg4, arg5)
struct opvar* ov;
select_iter_func5 func;
genericptr_t arg, arg2, arg3, arg4, arg5;
{
    int x, y;

    /* yes, this is very naive, but it's not _that_ expensive. */
    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            if (selection_getpoint(x, y, ov))
                (*func)(x, y, arg, arg2, arg3, arg4, arg5);
}


void
sel_set_ter(x, y, arg)
int x, y;
genericptr_t arg;
{
    terrain terr;
    terr = *(terrain *) arg;
    SET_TYPLIT(x, y, terr.ter, terr.subter, terr.tlit);

    
    if (x >= COLNO)
        x = COLNO - 1;
    if (x < 0)
        x = 0;
    if (y >= ROWNO)
        y = ROWNO - 1;
    if (y < 0)
        y = 0;

    /* handle doors and secret doors */
    if (levl[x][y].typ == SDOOR || IS_DOOR(levl[x][y].typ)) {
        if (levl[x][y].typ == SDOOR)
            levl[x][y].doormask = D_CLOSED;
        if (x && (IS_WALL(levl[x - 1][y].typ) || levl[x - 1][y].horizontal))
            levl[x][y].horizontal = 1;
    }
}

void
sel_set_feature(x, y, arg)
int x, y;
genericptr_t arg;
{
    /* Do nothing if there is already a feature */
    if (IS_FURNITURE(levl[x][y].typ))
        return;

    /* Feature sounds etc. are genereated after the level creation is complee */
    if (IS_FLOOR((*(int*)arg)))
    {
        levl[x][y].floortyp = 0;
        levl[x][y].floorsubtyp = 0;
        levl[x][y].floorvartyp = 0;
    }
    else if (IS_FLOOR(levl[x][y].typ))
    {
        levl[x][y].floortyp = levl[x][y].typ;
        levl[x][y].floorsubtyp = levl[x][y].subtyp;
        levl[x][y].floorvartyp = levl[x][y].vartyp;
    }
    else
    {
        levl[x][y].floortyp = location_type_definitions[(*(int*)arg)].initial_floor_type;
        levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
        levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
    }

    levl[x][y].typ = (*(int *) arg);
    levl[x][y].subtyp = get_initial_location_subtype(levl[x][y].typ);
    levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
    levl[x][y].special_quality = 0;

}

void
sel_set_feature2(x, y, arg, arg2)
int x, y;
genericptr_t arg, arg2;
{
    /* Do nothing if there is already a feature */
    if (IS_FURNITURE(levl[x][y].typ))
        return;

    /* Feature sounds etc. are genereated after the level creation is complee */
    if (IS_FLOOR((*(int*)arg)))
    {
        levl[x][y].floortyp = 0;
        levl[x][y].floorsubtyp = 0;
        levl[x][y].floorvartyp = 0;
    }
    else if (IS_FLOOR(levl[x][y].typ))
    {
        levl[x][y].floortyp = levl[x][y].typ;
        levl[x][y].floorsubtyp = levl[x][y].subtyp;
        levl[x][y].floorvartyp = levl[x][y].vartyp;
    }
    else
    {
        levl[x][y].floortyp = location_type_definitions[(*(int*)arg)].initial_floor_type;
        levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
        levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
    }

    levl[x][y].typ = (*(int*)arg);
    levl[x][y].subtyp = (*(int*)arg2);
    levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
    levl[x][y].special_quality = 0;

}

void
sel_set_floor(x, y, arg, arg2)
int x, y;
genericptr_t arg, arg2;
{
    if (IS_FLOOR(levl[x][y].typ))
    {
        /* Command can change a floor tile set earlier to another type, in order to ease the re-flooring rooms */
        levl[x][y].typ = *(int*)arg;
        levl[x][y].subtyp = *(int*)arg2;
        levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
        levl[x][y].special_quality = 0;
        levl[x][y].floortyp = 0;
        levl[x][y].floorsubtyp = 0;
        levl[x][y].floorvartyp = 0;
    }
    else
    {
        levl[x][y].floortyp = *(int*)arg;
        levl[x][y].floorsubtyp = *(int*)arg2;
        levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
    }
}

void
sel_set_subtype(x, y, arg1, arg2)
int x, y;
genericptr_t arg1, arg2;
{
    int typ = (*(int*)arg1);
    if (typ < 0 || levl[x][y].typ == typ)
    {
        levl[x][y].subtyp = (*(int*)arg2);
        levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
    }
}

void
sel_set_carpet_piece(x, y, arg1, arg2, arg3)
int x, y;
genericptr_t arg1, arg2, arg3;
{
    schar typ = (schar)(*(int*)arg1);
    if(typ >= 0 && typ < MAX_CARPETS)
        levl[x][y].carpet_typ = typ;
    
    schar piece = (schar)(*(int*)arg2);
    if(piece >= 0)
        levl[x][y].carpet_piece = piece;
    
    levl[x][y].carpet_flags = (uchar)(*(int*)arg3);
}

void
sel_set_door(dx, dy, arg, arg2, arg3, arg4, arg5)
int dx, dy;
genericptr_t arg, arg2, arg3, arg4, arg5;
{
    unsigned short dmask = *(unsigned short*) arg;
    int subtyp = *(int*)arg2;
    long key_otyp = *(long*)arg3;
    long key_spe_quality = *(long*)arg4;
    unsigned short dflags = *(unsigned short*)arg5;

    xchar x = dx, y = dy;
    int settyp = (dmask & D_SECRET) ? SDOOR : DOOR;

    if (levl[x][y].typ != settyp)
    {
        if (IS_FLOOR(levl[x][y].typ))
        {
            levl[x][y].floortyp = levl[x][y].typ;
            levl[x][y].floorsubtyp = levl[x][y].subtyp;
            levl[x][y].floorvartyp = levl[x][y].vartyp;
        }
        else if (IS_FLOOR(levl[x][y].floortyp))
        {
            levl[x][y].floortyp = 0;
            levl[x][y].floorsubtyp = 0;
            levl[x][y].floorvartyp = 0;
        }
        else
        {
            levl[x][y].floortyp = location_type_definitions[settyp].initial_floor_type;
            levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
            levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
        }

        levl[x][y].typ = settyp;
    }

    if (dmask & D_SECRET)
    {
        dmask &= ~D_SECRET;
        if (dmask < D_CLOSED)
            dmask = D_CLOSED;
    }

    dmask |= dflags;

    set_door_orientation(x, y); /* set/clear levl[x][y].horizontal */
    levl[x][y].subtyp = subtyp;
    levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
    levl[x][y].doormask = dmask;
    levl[x][y].key_otyp = key_otyp;
    levl[x][y].special_quality = key_spe_quality;

    /* Clear out any possible decoration */
    levl[x][y].decoration_typ = 0;
    levl[x][y].decoration_subtyp = 0;
    levl[x][y].decoration_dir = 0;
    levl[x][y].decoration_flags = 0;

    SpLev_Map[x][y] = 1;
}

void
spo_door(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_door";
    struct opvar *msk, *sel;
    struct opvar* varparam;
    unsigned short dmask = 0;
    int subtyp = 0;
    unsigned short dflags = 0;
    xchar secret = 0;
    long kotyp = 0, kspeq = 0;
    int nparams = 0;

    if (!OV_pop_i(msk))
        return;

    if (!OV_pop_i(varparam))
        return;

    while ((nparams++ < (SP_D_V_END + 1)) && varparam && (OV_typ(varparam) == SPOVAR_INT)
        && (OV_i(varparam) >= 0) && (OV_i(varparam) < SP_D_V_END))
    {
        struct opvar* parm;

        OV_pop(parm);
        switch (OV_i(varparam))
        {
        case SP_D_V_SUBTYPE:
            if (OV_typ(parm) == SPOVAR_INT)
                subtyp = (int)OV_i(parm);
            break;
        case SP_D_V_KEY_TYPE:
            if (OV_typ(parm) == SPOVAR_OBJ) {
                //char objclass = SP_OBJ_CLASS(OV_i(parm));
                int otyp = (int)SP_OBJ_TYP(OV_i(parm));

                if (otyp > STRANGE_OBJECT && otyp < NUM_OBJECTS) {
                    kotyp = is_otyp_key(otyp) ? otyp : STRANGE_OBJECT;
                    break; /* we're done! */
                }
                else {
                    kotyp = STRANGE_OBJECT;
                }
            }
            break;
        case SP_D_V_SPECIAL_QUALITY:
            if (OV_typ(parm) == SPOVAR_INT)
                kspeq = OV_i(parm);
            break;

        case SP_D_V_INDESTRUCTIBLE:
            if (OV_typ(parm) == SPOVAR_INT && OV_i(parm))
                dflags |= L_INDESTRUCTIBLE;
            break;

        case SP_D_V_SECRET_DOOR:
            if (OV_typ(parm) == SPOVAR_INT)
                secret = (xchar)OV_i(parm);
            break;

        case SP_D_V_USES_UP_KEY:
            if (OV_typ(parm) == SPOVAR_INT && OV_i(parm))
                dflags |= L_USES_UP_KEY;
            break;

        case SP_D_V_NON_PASSDOOR:
            if (OV_typ(parm) == SPOVAR_INT && OV_i(parm))
                dflags |= L_NON_PASSDOOR;
            break;

        case SP_D_V_END:
            nparams = SP_D_V_END + 1;
            break;
        default:
            impossible("DOOR with unknown variable param type!");
            break;
        }
        opvar_free(parm);
        if (OV_i(varparam) != SP_D_V_END) {
            opvar_free(varparam);
            OV_pop(varparam);
        }
    }

    if (!OV_pop_typ(sel, SPOVAR_SEL))
        return;

    dmask = OV_i(msk) == -1 ? (unsigned short)rnddoor() : (unsigned short) OV_i(msk);
    if (secret)
        dmask |= D_SECRET;

    selection_iterate5(sel, sel_set_door, (genericptr_t) &dmask, (genericptr_t)&subtyp, (genericptr_t)&kotyp, (genericptr_t)&kspeq, (genericptr_t)&dflags);

    opvar_free(varparam);
    opvar_free(sel);
    opvar_free(msk);
}

void
spo_feature(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_feature";
    struct opvar *sel;
    int typ;

    if (!OV_pop_typ(sel, SPOVAR_SEL))
        return;

    switch (coder->opcode) {
    default:
        impossible("spo_feature called with wrong opcode %i.", coder->opcode);
        break;
    case SPO_THRONE:
        typ = THRONE;
        break;
    case SPO_SINK:
        typ = SINK;
        break;
    case SPO_POOL:
        typ = POOL;
        break;
    }
    selection_iterate(sel, sel_set_feature, (genericptr_t) &typ);

    opvar_free(sel);
}

void spo_fountain(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_fountain";
    struct opvar* sel;
    struct opvar* subtyp_opvar;

    if (!OV_pop_i(subtyp_opvar) || !OV_pop_typ(sel, SPOVAR_SEL))
        return;

    int typ = FOUNTAIN;
    int subtyp = (int)OV_i(subtyp_opvar);
    
    if (subtyp >= MAX_FOUNTAIN_SUBTYPES || subtyp < 0)
        subtyp = 1 + rn2(MAX_FOUNTAIN_SUBTYPES - 1); /* Natural fountain does not appear randomly */

    selection_iterate2(sel, sel_set_feature2, (genericptr_t)&typ, (genericptr_t)&subtyp);

    opvar_free(subtyp_opvar);
    opvar_free(sel);

}

void spo_anvil(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_anvil";
    struct opvar* acoord, *mtype;
    anvil tmpanvil;

    if (!OV_pop(mtype) || !OV_pop_c(acoord))
        return;

    tmpanvil.coord = OV_i(acoord);
    tmpanvil.mtype = NON_PM;
    if (OV_typ(mtype) == SPOVAR_MONST)
    {
        //char monclass = SP_MONST_CLASS(OV_i(mtype));
        int monid = (int)SP_MONST_PM(OV_i(mtype));

        if (monid >= LOW_PM && monid < NUM_MONSTERS)
        {
            tmpanvil.mtype = monid;
        }
        else
        {
            tmpanvil.mtype = NON_PM;
        }
    }

    create_anvil(&tmpanvil, coder->croom);

    opvar_free(mtype);
    opvar_free(acoord);
}

void spo_decoration(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_decoration";
    struct opvar* acoord, *typ_opvar, *subtyp_opvar, *dir_opvar, *flags_opvar, *lit_opvar;
    decoration tmpdecoration;

    if (!OV_pop_i(typ_opvar) || !OV_pop_i(subtyp_opvar) || !OV_pop_i(dir_opvar) || !OV_pop_i(flags_opvar) || !OV_pop_i(lit_opvar) || !OV_pop_c(acoord))
        return;

    tmpdecoration.coord = OV_i(acoord);
    tmpdecoration.typ = (schar)OV_i(typ_opvar);
    tmpdecoration.subtyp = (schar)OV_i(subtyp_opvar);
    tmpdecoration.dir = (schar)OV_i(dir_opvar);
    tmpdecoration.item_in_holder = (uchar)OV_i(flags_opvar);
    tmpdecoration.lit = (boolean)OV_i(lit_opvar);

    create_decoration(&tmpdecoration, coder->croom);

    opvar_free(typ_opvar);
    opvar_free(subtyp_opvar);
    opvar_free(dir_opvar);
    opvar_free(flags_opvar);
    opvar_free(lit_opvar);
    opvar_free(acoord);
}

void
spo_floor(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_floor";
    struct opvar* sel;
    struct opvar* typ_opvar, *subtyp_opvar;
    int typ, subtyp;

    if (!OV_pop_i(typ_opvar) || !OV_pop_i(subtyp_opvar) || !OV_pop_typ(sel, SPOVAR_SEL))
        return;

    typ = (int)OV_i(typ_opvar);
    subtyp = (int)OV_i(subtyp_opvar);
    selection_iterate2(sel, sel_set_floor, (genericptr_t)&typ, (genericptr_t)&subtyp);

    opvar_free(typ_opvar);
    opvar_free(subtyp_opvar);
    opvar_free(sel);
}


void
spo_subtype(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_subtype";
    struct opvar* sel;
    struct opvar* subtyp_opvar, * typ_opvar;
    int typ, subtyp;

    if (!OV_pop_i(typ_opvar) || !OV_pop_i(subtyp_opvar) || !OV_pop_typ(sel, SPOVAR_SEL))
        return;

    typ = (int)OV_i(typ_opvar);
    subtyp = (int)OV_i(subtyp_opvar);
    selection_iterate2(sel, sel_set_subtype, (genericptr_t)&typ, (genericptr_t)&subtyp);

    opvar_free(typ_opvar);
    opvar_free(subtyp_opvar);
    opvar_free(sel);
}


void
spo_carpet(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_carpet";
    struct opvar* carpet_type, * area;
    xchar dx1, dy1, dx2, dy2;
    int typ;

    if (!OV_pop_i(carpet_type) || !OV_pop_r(area))
        return;

    dx1 = (xchar)SP_REGION_X1(OV_i(area));
    dy1 = (xchar)SP_REGION_Y1(OV_i(area));
    dx2 = (xchar)SP_REGION_X2(OV_i(area));
    dy2 = (xchar)SP_REGION_Y2(OV_i(area));
    typ = (int)OV_i(carpet_type);

    get_location(&dx1, &dy1, ANY_LOC, coder->croom);
    get_location(&dx2, &dy2, ANY_LOC, coder->croom);

    create_carpet(dx1, dy1, dx2, dy2, typ);

    opvar_free(area);
    opvar_free(carpet_type);
}

void
spo_carpet_piece(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_carpet_piece";
    struct opvar* sel;
    struct opvar* subtyp_opvar, *typ_opvar, * flags_opvar;
    int typ, subtyp, cflags;

    if (!OV_pop_i(typ_opvar) || !OV_pop_i(subtyp_opvar) || !OV_pop_i(flags_opvar) || !OV_pop_typ(sel, SPOVAR_SEL))
        return;

    typ = (int)OV_i(typ_opvar);
    subtyp = (int)OV_i(subtyp_opvar);
    cflags = (int)OV_i(flags_opvar);
    selection_iterate3(sel, sel_set_carpet_piece, (genericptr_t)&typ, (genericptr_t)&subtyp, (genericptr_t)&cflags);

    opvar_free(typ_opvar);
    opvar_free(subtyp_opvar);
    opvar_free(flags_opvar);
    opvar_free(sel);
}

void spo_npc(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_npc";
    struct opvar *acoord, *typ, *mtype;
    npc_create_info tmpnpc;

    if (!OV_pop_i(typ) || !OV_pop(mtype) || !OV_pop_c(acoord))
        return;

    tmpnpc.coord = OV_i(acoord);
    tmpnpc.typ = (uchar)OV_i(typ);
    tmpnpc.mtype = NON_PM;
    if (OV_typ(mtype) == SPOVAR_MONST)
    {
        //char monclass = SP_MONST_CLASS(OV_i(mtype));
        int monid = (int)SP_MONST_PM(OV_i(mtype));

        if (monid >= LOW_PM && monid < NUM_MONSTERS)
        {
            tmpnpc.mtype = monid;
        }
        else
        {
            tmpnpc.mtype = NON_PM;
        }
    }

    create_npc(&tmpnpc, coder->croom);

    opvar_free(mtype);
    opvar_free(typ);
    opvar_free(acoord);
}

void spo_modron_portal(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_modron_portal";
    struct opvar* acoord, *tcoord, *activated, *typ;
    modron_portal tmpportal;

    if (!OV_pop_i(activated) || !OV_pop_i(typ) || !OV_pop_c(tcoord) || !OV_pop_c(acoord))
        return;

    tmpportal.coord = OV_i(acoord);
    tmpportal.t_coord = OV_i(tcoord);
    tmpportal.typ = (uchar)OV_i(typ);
    tmpportal.activated = (boolean)OV_i(activated);
    tmpportal.level_teleporter = FALSE;
    tmpportal.tele_direction = 0;
    tmpportal.end_type = FALSE;

    create_modron_portal(&tmpportal, coder->croom);

    opvar_free(activated);
    opvar_free(typ);
    opvar_free(tcoord);
    opvar_free(acoord);
}

void spo_modron_level_teleporter(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_modron_level_teleporter";
    struct opvar* acoord, *activated, *typ, *dir, *endt;
    modron_portal tmpportal;

    if (!OV_pop_i(activated) || !OV_pop_i(typ) || !OV_pop_i(endt) || !OV_pop_i(dir) || !OV_pop_c(acoord))
        return;

    int tele_dir = (int)OV_i(dir);
    tmpportal.coord = OV_i(acoord);
    tmpportal.t_coord = 0L;
    tmpportal.typ = (uchar)OV_i(typ);
    tmpportal.activated = (boolean)OV_i(activated);
    tmpportal.level_teleporter = TRUE;
    tmpportal.tele_direction = tele_dir;
    tmpportal.end_type = (boolean)OV_i(endt);

    create_modron_portal(&tmpportal, coder->croom);

    opvar_free(activated);
    opvar_free(typ);
    opvar_free(dir);
    opvar_free(endt);
    opvar_free(acoord);
}

void
spo_terrain(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_terrain";
    terrain tmpterrain;
    struct opvar *ter, *subter, *sel;

    if (!OV_pop_i(subter) || !OV_pop_typ(ter, SPOVAR_MAPCHAR) || !OV_pop_typ(sel, SPOVAR_SEL))
        return;

    tmpterrain.ter = (xchar)SP_MAPCHAR_TYP(OV_i(ter));
    tmpterrain.subter = (xchar)OV_i(subter);
    tmpterrain.tlit = (xchar)SP_MAPCHAR_LIT(OV_i(ter));
    selection_iterate(sel, sel_set_ter, (genericptr_t) &tmpterrain);

    opvar_free(ter);
    opvar_free(subter);
    opvar_free(sel);
}

void
spo_replace_terrain(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_replace_terrain";
    replaceterrain rt;
    struct opvar *reg, *from_ter, *to_ter, *chance;

    if (!OV_pop_i(chance) || !OV_pop_typ(to_ter, SPOVAR_MAPCHAR)
        || !OV_pop_typ(from_ter, SPOVAR_MAPCHAR) || !OV_pop_r(reg))
        return;

    rt.chance = (xchar)OV_i(chance);
    rt.tolit = (xchar)SP_MAPCHAR_LIT(OV_i(to_ter));
    rt.toter = (xchar)SP_MAPCHAR_TYP(OV_i(to_ter));
    rt.fromter = (xchar)SP_MAPCHAR_TYP(OV_i(from_ter));
    /* TODO: use SP_MAPCHAR_LIT(OV_i(from_ter)) too */
    rt.x1 = (xchar)SP_REGION_X1(OV_i(reg));
    rt.y1 = (xchar)SP_REGION_Y1(OV_i(reg));
    rt.x2 = (xchar)SP_REGION_X2(OV_i(reg));
    rt.y2 = (xchar)SP_REGION_Y2(OV_i(reg));

    replace_terrain(&rt, coder->croom);

    opvar_free(reg);
    opvar_free(from_ter);
    opvar_free(to_ter);
    opvar_free(chance);
}

STATIC_OVL boolean
generate_way_out_method(nx,ny, ov)
int nx,ny;
struct opvar *ov;
{
    static const char nhFunc[] = "generate_way_out_method";
    const int escapeitems[] = { PICK_AXE,
                                DWARVISH_MATTOCK,
                                SPADE_OF_COLOSSAL_EXCAVATION,
                                WAN_DIGGING,
                                WAN_TELEPORTATION,
                                SCR_TELEPORTATION,
                                RIN_TELEPORTATION };
    struct opvar *ov2 = selection_opvar((char *) 0), *ov3;
    schar x, y;
    boolean res = TRUE;

    selection_floodfill(ov2, nx, ny, TRUE);
    ov3 = opvar_clone(ov2);

    /* try to make a secret door */
    while (selection_rndcoord(ov3, &x, &y, TRUE)) 
    {
        if (isok(x+1, y) && !selection_getpoint(x+1, y, ov)
            && IS_WALL(levl[x+1][y].typ)
            && isok(x+2, y) &&  selection_getpoint(x+2, y, ov)
            && ACCESSIBLE(levl[x+2][y].typ))
        {
            if (IS_FLOOR(levl[x + 1][y].typ))
            {
                levl[x + 1][y].floortyp = levl[x + 1][y].typ;
                levl[x + 1][y].floorsubtyp = levl[x + 1][y].subtyp;
                levl[x + 1][y].floorvartyp = levl[x + 1][y].vartyp;
            }
            else
            {
                levl[x + 1][y].floortyp = location_type_definitions[SDOOR].initial_floor_type;
                levl[x + 1][y].floorsubtyp = get_initial_location_subtype(levl[x + 1][y].floortyp);
                levl[x + 1][y].floorvartyp = get_initial_location_vartype(levl[x + 1][y].floortyp, levl[x + 1][y].floorsubtyp);
            }
            levl[x + 1][y].typ = SDOOR;
            levl[x + 1][y].subtyp = 0;
            levl[x + 1][y].vartyp = 0;
            levl[x + 1][y].special_quality = 0;
            goto gotitdone;
        }
        if (isok(x-1, y) && !selection_getpoint(x-1, y, ov)
            && IS_WALL(levl[x-1][y].typ)
            && isok(x-2, y) &&  selection_getpoint(x-2, y, ov)
            && ACCESSIBLE(levl[x-2][y].typ)) 
        {
            if (IS_FLOOR(levl[x - 1][y].typ))
            {
                levl[x - 1][y].floortyp = levl[x - 1][y].typ;
                levl[x - 1][y].floorsubtyp = levl[x - 1][y].subtyp;
                levl[x - 1][y].floorvartyp = levl[x - 1][y].vartyp;
            }
            else
            {
                levl[x - 1][y].floortyp = location_type_definitions[SDOOR].initial_floor_type;
                levl[x - 1][y].floorsubtyp = get_initial_location_subtype(levl[x - 1][y].floortyp);
                levl[x - 1][y].floorvartyp = get_initial_location_vartype(levl[x - 1][y].floortyp, levl[x - 1][y].floorsubtyp);

            }
            levl[x - 1][y].typ = SDOOR;
            levl[x - 1][y].subtyp = get_initial_location_subtype(levl[x - 1][y].typ);
            levl[x - 1][y].vartyp = get_initial_location_vartype(levl[x - 1][y].typ, levl[x - 1][y].subtyp);
            levl[x - 1][y].special_quality = 0;
            goto gotitdone;
        }
        if (isok(x, y+1) && !selection_getpoint(x, y+1, ov)
            && IS_WALL(levl[x][y+1].typ)
            && isok(x, y+2) &&  selection_getpoint(x, y+2, ov)
            && ACCESSIBLE(levl[x][y+2].typ)) 
        {
            if (IS_FLOOR(levl[x][y + 1].typ))
            {
                levl[x][y + 1].floortyp = levl[x][y + 1].typ;
                levl[x][y + 1].floorsubtyp = levl[x][y + 1].subtyp;
                levl[x][y + 1].floorvartyp = levl[x][y + 1].vartyp;
            }
            else
            {
                levl[x][y + 1].floortyp = location_type_definitions[SDOOR].initial_floor_type;
                levl[x][y + 1].floorsubtyp = get_initial_location_subtype(levl[x][y + 1].floortyp);
                levl[x][y + 1].floorvartyp = get_initial_location_vartype(levl[x][y + 1].floortyp, levl[x][y + 1].floorsubtyp);
            }
            levl[x][y + 1].typ = SDOOR;
            levl[x][y + 1].subtyp = get_initial_location_subtype(levl[x][y + 1].typ);
            levl[x][y + 1].vartyp = get_initial_location_vartype(levl[x][y + 1].typ, levl[x][y + 1].subtyp);
            levl[x][y + 1].special_quality = 0;
            goto gotitdone;
        }
        if (isok(x, y-1) && !selection_getpoint(x, y-1, ov)
            && IS_WALL(levl[x][y-1].typ)
            && isok(x, y-2) &&  selection_getpoint(x, y-2, ov)
            && ACCESSIBLE(levl[x][y-2].typ)) 
        {
            if (IS_FLOOR(levl[x][y - 1].typ))
            {
                levl[x][y - 1].floortyp = levl[x][y - 1].typ;
                levl[x][y - 1].floorsubtyp = levl[x][y - 1].subtyp;
                levl[x][y - 1].floorvartyp = levl[x][y - 1].vartyp;
            }
            else
            {
                levl[x][y - 1].floortyp = location_type_definitions[SDOOR].initial_floor_type;
                levl[x][y - 1].floorsubtyp = get_initial_location_subtype(levl[x][y - 1].floortyp);
                levl[x][y - 1].floorvartyp = get_initial_location_vartype(levl[x][y - 1].floortyp, levl[x][y - 1].floorsubtyp);
            }
            levl[x][y - 1].typ = SDOOR;
            levl[x][y - 1].subtyp = get_initial_location_subtype(levl[x][y - 1].typ);
            levl[x][y - 1].vartyp = get_initial_location_vartype(levl[x][y - 1].typ, levl[x][y - 1].subtyp);
            levl[x][y - 1].special_quality = 0;
            goto gotitdone;
        }
    }

    /* try to make a hole or a trapdoor */
    if (Can_fall_thru(&u.uz)) {
        opvar_free(ov3);
        ov3 = opvar_clone(ov2);
        while (selection_rndcoord(ov3, &x, &y, TRUE)) {
            if (maketrap(x,y, rn2(2) ? HOLE : TRAPDOOR, NON_PM, MKTRAP_NO_FLAGS))
                goto gotitdone;
        }
    }

    /* generate one of the escape items */
    if (selection_rndcoord(ov2, &x, &y, FALSE)) {
        mksobj_at(escapeitems[rn2(SIZE(escapeitems))], x, y, TRUE, FALSE);
        goto gotitdone;
    }

    res = FALSE;
 gotitdone:
    opvar_free(ov2);
    opvar_free(ov3);
    return res;
}

STATIC_OVL void
ensure_way_out()
{
    static const char nhFunc[] = "ensure_way_out";
    struct opvar *ov = selection_opvar((char *) 0);
    struct trap *ttmp = ftrap;
    int x,y;
    boolean ret = TRUE;

    set_selection_floodfillchk(floodfillchk_match_accessible);

    if (xupstair && !selection_getpoint(xupstair, yupstair, ov))
        selection_floodfill(ov, xupstair, yupstair, TRUE);
    if (xdnstair && !selection_getpoint(xdnstair, ydnstair, ov))
        selection_floodfill(ov, xdnstair, ydnstair, TRUE);
    if (xupladder && !selection_getpoint(xupladder, yupladder, ov))
        selection_floodfill(ov, xupladder, yupladder, TRUE);
    if (xdnladder && !selection_getpoint(xdnladder, ydnladder, ov))
        selection_floodfill(ov, xdnladder, ydnladder, TRUE);

    while (ttmp) {
        if (((trap_type_definitions[ttmp->ttyp].tdflags & TRAPDEF_FLAGS_NOT_OVERRIDEN)
            || is_hole(ttmp->ttyp))
            && !selection_getpoint(ttmp->tx, ttmp->ty, ov))
            selection_floodfill(ov, ttmp->tx, ttmp->ty, TRUE);
        ttmp = ttmp->ntrap;
    }

    do {
        ret = TRUE;
        for (x = 0; x < COLNO; x++)
            for (y = 0; y < ROWNO; y++)
                if (ACCESSIBLE(levl[x][y].typ)
                    && !selection_getpoint(x, y, ov)) {
                    if (generate_way_out_method(x,y, ov))
                        selection_floodfill(ov, x,y, TRUE);
                    ret = FALSE;
                    goto outhere;
                }
    outhere: ;
    } while (!ret);
    opvar_free(ov);
}

void
spo_levregion(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_levregion";
    struct opvar *rname, *padding, *rtype, *del_islev, *dy2, *dx2, *dy1, *dx1,
        *in_islev, *iy2, *ix2, *iy1, *ix1;

    lev_region *tmplregion;

    if (!OV_pop_s(rname) || !OV_pop_i(padding) || !OV_pop_i(rtype)
        || !OV_pop_i(del_islev) || !OV_pop_i(dy2) || !OV_pop_i(dx2)
        || !OV_pop_i(dy1) || !OV_pop_i(dx1) || !OV_pop_i(in_islev)
        || !OV_pop_i(iy2) || !OV_pop_i(ix2) || !OV_pop_i(iy1)
        || !OV_pop_i(ix1))
        return;

    tmplregion = (lev_region *) alloc(sizeof(lev_region));

    tmplregion->inarea.x1 = (xchar)OV_i(ix1);
    tmplregion->inarea.y1 = (xchar)OV_i(iy1);
    tmplregion->inarea.x2 = (xchar)OV_i(ix2);
    tmplregion->inarea.y2 = (xchar)OV_i(iy2);

    tmplregion->delarea.x1 = (xchar)OV_i(dx1);
    tmplregion->delarea.y1 = (xchar)OV_i(dy1);
    tmplregion->delarea.x2 = (xchar)OV_i(dx2);
    tmplregion->delarea.y2 = (xchar)OV_i(dy2);

    tmplregion->in_islev = (boolean)OV_i(in_islev);
    tmplregion->del_islev = (boolean)OV_i(del_islev);
    tmplregion->rtype = (xchar)OV_i(rtype);
    tmplregion->padding = (xchar)OV_i(padding);
    tmplregion->rname.str = dupstr(OV_s(rname));

    if (!tmplregion->in_islev) {
        get_location(&tmplregion->inarea.x1, &tmplregion->inarea.y1, ANY_LOC,
                     (struct mkroom *) 0);
        get_location(&tmplregion->inarea.x2, &tmplregion->inarea.y2, ANY_LOC,
                     (struct mkroom *) 0);
    }

    if (!tmplregion->del_islev) {
        get_location(&tmplregion->delarea.x1, &tmplregion->delarea.y1,
                     ANY_LOC, (struct mkroom *) 0);
        get_location(&tmplregion->delarea.x2, &tmplregion->delarea.y2,
                     ANY_LOC, (struct mkroom *) 0);
    }
    if (num_lregions) {
        /* realloc the lregion space to add the new one */
        lev_region *newl = (lev_region *) alloc(
            sizeof(lev_region) * (size_t) (1 + (size_t)num_lregions));

        (void) memcpy((genericptr_t) (newl), (genericptr_t) lregions,
                      sizeof(lev_region) * num_lregions);
        Free(lregions);
        num_lregions++;
        lregions = newl;
    } else {
        num_lregions = 1;
        lregions = (lev_region *) alloc(sizeof(lev_region));
    }
    (void) memcpy(&lregions[num_lregions - 1], tmplregion,
                  sizeof(lev_region));
    free(tmplregion);

    opvar_free(dx1);
    opvar_free(dy1);
    opvar_free(dx2);
    opvar_free(dy2);

    opvar_free(ix1);
    opvar_free(iy1);
    opvar_free(ix2);
    opvar_free(iy2);

    opvar_free(del_islev);
    opvar_free(in_islev);
    opvar_free(rname);
    opvar_free(rtype);
    opvar_free(padding);
}

void
spo_region(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_region";
    struct opvar *rtype, *rlit, *rflags, *area, *rfloortyp, * rfloorsubtyp, * rtileset, * rdecotyp, *mtype;
    xchar dx1, dy1, dx2, dy2;
    register struct mkroom *troom;
    boolean prefilled, room_not_needed, irregular, joined;
    int roommontype = NON_PM, roomtileset = -1;
    short decotyp;

    if (!OV_pop_i(rdecotyp) || !OV_pop_i(rtileset) || !OV_pop_i(rfloorsubtyp) || !OV_pop_i(rfloortyp) || !OV_pop_i(rflags) || !OV_pop_i(rtype) || !OV_pop_i(rlit) || !OV_pop(mtype)
        || !OV_pop_r(area))
        return;

    roomtileset = (int)OV_i(rtileset);
    decotyp = (short)OV_i(rdecotyp);
    if (decotyp < 0)
        decotyp = 0;
    prefilled = (boolean)!(OV_i(rflags) & (1 << 0));
    irregular = (boolean)(OV_i(rflags) & (1 << 1));
    joined = (boolean)!(OV_i(rflags) & (1 << 2));
    xchar rawfloortype = (xchar)OV_i(rfloortyp);
    xchar rawfloorsubtype = (xchar)OV_i(rfloorsubtyp);
    xchar usedfloortype = IS_FLOOR(rawfloortype) ? rawfloortype : rawfloorsubtype >= 0 ? ROOM : 0; /* If only subtype is defined, assume that we speak of room floors */
    xchar usedfloorsubtype = rawfloorsubtype >= 0 ? rawfloorsubtype : 0;

    if (OV_i(rtype) > MAXRTYPE) {
        OV_i(rtype) -= MAXRTYPE + 1;
        prefilled = TRUE;
    } else
        prefilled = FALSE;

    if (OV_i(rlit) < 0)
        OV_i(rlit) =
            (rnd(1 + abs(depth(&u.uz))) < 11 && rn2(77)) ? TRUE : FALSE;

    dx1 = (xchar)SP_REGION_X1(OV_i(area));
    dy1 = (xchar)SP_REGION_Y1(OV_i(area));
    dx2 = (xchar)SP_REGION_X2(OV_i(area));
    dy2 = (xchar)SP_REGION_Y2(OV_i(area));

    get_location(&dx1, &dy1, ANY_LOC, (struct mkroom *) 0);
    get_location(&dx2, &dy2, ANY_LOC, (struct mkroom *) 0);

    if (OV_typ(mtype) == SPOVAR_MONST)
    {
        //char monclass = SP_MONST_CLASS(OV_i(mtype));
        int monid = (int)SP_MONST_PM(OV_i(mtype));

        if (monid >= LOW_PM && monid < NUM_MONSTERS)
        {
            roommontype = monid;
        }
        else
        {
            roommontype = NON_PM;
        }
    }

    /* for an ordinary room, `prefilled' is a flag to force
       an actual room to be created (such rooms are used to
       control placement of migrating monster arrivals) */
    room_not_needed = (OV_i(rtype) == OROOM && !irregular && !prefilled);
    if (room_not_needed || nroom >= MAXNROFROOMS) {
        region tmpregion;
        if (!room_not_needed)
            impossible("Too many rooms on new level!");
        tmpregion.rlit = (xchar)OV_i(rlit);
        tmpregion.rfloortyp = usedfloortype; /* If only subtype is defined, assume that we speak of room floors */
        tmpregion.rfloorsubtyp = usedfloorsubtype;
        tmpregion.x1 = dx1;
        tmpregion.y1 = dy1;
        tmpregion.x2 = dx2;
        tmpregion.y2 = dy2;
        tmpregion.mtype = NON_PM;
        light_region(&tmpregion);

        opvar_free(area);
        opvar_free(rflags);
        opvar_free(rlit);
        opvar_free(rtype);
        opvar_free(rfloorsubtyp);
        opvar_free(rfloortyp);
        opvar_free(rtileset);
        opvar_free(rdecotyp);
        opvar_free(mtype);

        return;
    }

    troom = &rooms[nroom];

    /* mark rooms that must be filled, but do it later */
    if (OV_i(rtype) != OROOM)
        troom->needfill = (prefilled ? 2 : 1);

    troom->needjoining = joined;

    if (irregular) 
    {
        min_rx = max_rx = dx1;
        min_ry = max_ry = dy1;
        smeq[nroom] = nroom;
        flood_fill_rm(dx1, dy1, nroom + ROOMOFFSET, (boolean)OV_i(rlit), TRUE);
        add_room(min_rx, min_ry, max_rx, max_ry, FALSE, (schar)OV_i(rtype), TRUE, usedfloortype, usedfloorsubtype, roommontype, roomtileset, decotyp);
        troom->rlit = (schar)OV_i(rlit);
        troom->irregular = TRUE;
    }
    else 
    {
        add_room(dx1, dy1, dx2, dy2, (boolean)OV_i(rlit), (schar)OV_i(rtype), TRUE, usedfloortype, usedfloorsubtype, roommontype, roomtileset, decotyp);
#ifdef SPECIALIZATION
        topologize(troom, FALSE); /* set roomno */
#else
        topologize(troom); /* set roomno */
#endif
    }

    if (!room_not_needed)
    {
        if (coder->n_subroom > 1)
            impossible("region as subroom");
        else
        {
            coder->tmproomlist[coder->n_subroom] = troom;
            coder->failed_room[coder->n_subroom] = FALSE;
            coder->n_subroom++;
        }
    }

    opvar_free(area);
    opvar_free(rflags);
    opvar_free(rlit);
    opvar_free(rtype);
    opvar_free(rfloorsubtyp);
    opvar_free(rfloortyp);
    opvar_free(rtileset);
    opvar_free(rdecotyp);
    opvar_free(mtype);
}

void
spo_special_region(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_special_region";
    struct opvar* rtype, * area;
    xchar dx1, dy1, dx2, dy2;
    int typ;

    if (!OV_pop_i(rtype) || !OV_pop_r(area))
        return;

    dx1 = (xchar)SP_REGION_X1(OV_i(area));
    dy1 = (xchar)SP_REGION_Y1(OV_i(area));
    dx2 = (xchar)SP_REGION_X2(OV_i(area));
    dy2 = (xchar)SP_REGION_Y2(OV_i(area));
    typ = (int)OV_i(rtype);

    get_location(&dx1, &dy1, ANY_LOC, coder->croom);
    get_location(&dx2, &dy2, ANY_LOC, coder->croom);

    create_simple_permanent_region(dx1, dy1, dx2, dy2, typ);

    opvar_free(area);
    opvar_free(rtype);
}

void
spo_special_tileset(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_special_tileset";
    struct opvar*rtype, *sel;
    int typ;

    if (!OV_pop_i(rtype) || !OV_pop_typ(sel, SPOVAR_SEL))
        return;

    typ = (int)OV_i(rtype);

    selection_iterate(sel, sel_set_tileset, (genericptr_t)&typ);

    opvar_free(sel);
    opvar_free(rtype);
}

void
sel_set_tileset(x, y, arg)
int x, y;
genericptr_t arg;
{
    levl[x][y].use_special_tileset = 1;
    levl[x][y].special_tileset = (*(int*)arg);
}

void
spo_naming(coder)
struct sp_coder* coder;
{
    static const char nhFunc[] = "spo_naming";
    struct opvar* typ_opvar, *montype_opvar, *name_opvar;
    short mnum;
    xchar mclass;

    if (!OV_pop_s(name_opvar))
        return;

    if (!OV_pop_i(typ_opvar))
        return;

    if (!OV_pop_typ(montype_opvar, SPOVAR_MONST))
        return;

    mnum = (short)SP_MONST_PM(OV_i(montype_opvar));
    mclass = (xchar)SP_MONST_CLASS(OV_i(montype_opvar));

    Strcpy(level.flags.special_description, OV_s(name_opvar));

    int typ;
    typ = (int)OV_i(typ_opvar);
    level.flags.special_naming_reveal_type = typ;
    level.flags.special_naming_seen_monster_type = mnum;
    level.flags.special_naming_seen_monster_class = mclass;

    opvar_free(typ_opvar);
    opvar_free(montype_opvar);
    opvar_free(name_opvar);
}

void
spo_drawbridge(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_drawbridge";
    xchar x, y;
    int dopen;
    struct opvar *dir, *db_open, *dcoord;

    if (!OV_pop_i(dir) || !OV_pop_i(db_open) || !OV_pop_c(dcoord))
        return;

    get_location_coord(&x, &y, DRY | WET | HOT, coder->croom, OV_i(dcoord));
    if ((dopen = (int)OV_i(db_open)) == -1)
        dopen = !rn2(2);
    if (!create_drawbridge(x, y, (int)OV_i(dir), dopen ? TRUE : FALSE))
        impossible("Cannot create drawbridge.");
    SpLev_Map[x][y] = 1;

    opvar_free(dcoord);
    opvar_free(db_open);
    opvar_free(dir);
}

void
spo_mazewalk(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_mazewalk";
    xchar x, y;
    struct opvar *ftyp, *fstocked, *fdir, *mcoord;
    long dir;

    if (!OV_pop_i(ftyp) || !OV_pop_i(fstocked) || !OV_pop_i(fdir)
        || !OV_pop_c(mcoord))
        return;

    dir = OV_i(fdir);

    get_location_coord(&x, &y, ANY_LOC, coder->croom, OV_i(mcoord));
    if (!isok(x, y))
        return;

    if (OV_i(ftyp) < 1) {
        OV_i(ftyp) = level.flags.corrmaze ? CORR : level.flags.arboreal ? GRASS : level.flags.swampy ? GRASS : level.flags.desert ? GROUND : ROOM;
    }

    /* don't use move() - it doesn't use W_NORTH, etc. */
    switch (dir) {
    case W_NORTH:
        --y;
        break;
    case W_SOUTH:
        y++;
        break;
    case W_EAST:
        x++;
        break;
    case W_WEST:
        --x;
        break;
    default:
        impossible("spo_mazewalk: Bad MAZEWALK direction");
    }

    if (!IS_DOOR(levl[x][y].typ)) 
    {
        set_initial_location_type_at(x, y, (int)OV_i(ftyp));
    }

    /*
     * We must be sure that the parity of the coordinates for
     * walkfrom() is odd.  But we must also take into account
     * what direction was chosen.
     */
    if (!(x % 2)) {
        if (dir == W_EAST)
            x++;
        else
            x--;

        /* no need for IS_DOOR check; out of map bounds */
        set_initial_location_type_at(x, y, (int)OV_i(ftyp));
    }

    if (!(y % 2)) {
        if (dir == W_SOUTH)
            y++;
        else
            y--;
    }

    walkfrom(x, y, (schar)OV_i(ftyp));
    if (OV_i(fstocked))
        fill_empty_maze();

    opvar_free(mcoord);
    opvar_free(fdir);
    opvar_free(fstocked);
    opvar_free(ftyp);
}

void
spo_wall_property(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_wall_property";
    struct opvar *r;
    xchar dx1, dy1, dx2, dy2;
    int wprop = (coder->opcode == SPO_NON_DIGGABLE)
                   ? W_NONDIGGABLE
                   : W_NONPASSWALL;

    if (!OV_pop_r(r))
        return;

    dx1 = (xchar)SP_REGION_X1(OV_i(r));
    dy1 = (xchar)SP_REGION_Y1(OV_i(r));
    dx2 = (xchar)SP_REGION_X2(OV_i(r));
    dy2 = (xchar)SP_REGION_Y2(OV_i(r));

    get_location(&dx1, &dy1, ANY_LOC, (struct mkroom *) 0);
    get_location(&dx2, &dy2, ANY_LOC, (struct mkroom *) 0);

    set_wall_property(dx1, dy1, dx2, dy2, wprop);

    opvar_free(r);
}

void
spo_room_door(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_room_door";
    struct opvar *wall, *secret, *mask, *pos, *varparam;
    int nparams = 0;
    xchar secret_door_var = 0;
    room_door tmpd = { 0 };

    if (!OV_pop_i(wall) || !OV_pop_i(secret) || !OV_pop_i(mask)
        || !OV_pop_i(pos) || !coder->croom)
        return;

    if (!OV_pop_i(varparam))
        return;

    while ((nparams++ < (SP_D_V_END + 1)) && varparam && (OV_typ(varparam) == SPOVAR_INT)
        && (OV_i(varparam) >= 0) && (OV_i(varparam) < SP_D_V_END))
    {
        struct opvar* parm;

        OV_pop(parm);
        switch (OV_i(varparam))
        {
        case SP_D_V_SUBTYPE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpd.subtype = (int)OV_i(parm);
            break;
        case SP_D_V_KEY_TYPE:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpd.key_otyp = (int)OV_i(parm);
            break;
        case SP_D_V_SPECIAL_QUALITY:
            if (OV_typ(parm) == SPOVAR_INT)
                tmpd.key_special_quality = (int)OV_i(parm);
            break;
        case SP_D_V_INDESTRUCTIBLE:
            if (OV_typ(parm) == SPOVAR_INT && OV_i(parm))
                tmpd.dflags |= L_INDESTRUCTIBLE;
            break;
        case SP_D_V_SECRET_DOOR:
            if (OV_typ(parm) == SPOVAR_INT)
                secret_door_var = (xchar)OV_i(parm);
            break;
        case SP_D_V_USES_UP_KEY:
            if (OV_typ(parm) == SPOVAR_INT && OV_i(parm))
                tmpd.dflags |= L_USES_UP_KEY;
            break;
        case SP_D_V_NON_PASSDOOR:
            if (OV_typ(parm) == SPOVAR_INT && OV_i(parm))
                tmpd.dflags |= L_NON_PASSDOOR;
            break;

        case SP_D_V_END:
            nparams = SP_D_V_END + 1;
            break;
        default:
            impossible("DOOR in ROOM_DOOR with unknown variable param type!");
            break;
        }
        opvar_free(parm);
        if (OV_i(varparam) != SP_D_V_END) {
            opvar_free(varparam);
            OV_pop(varparam);
        }
    }

    tmpd.secret = (xchar)(OV_i(secret) || secret_door_var);
    tmpd.mask = (short)OV_i(mask);
    tmpd.pos = (xchar)OV_i(pos);
    tmpd.wall = (xchar)OV_i(wall);

    create_door(&tmpd, coder->croom);

    opvar_free(varparam);
    opvar_free(wall);
    opvar_free(secret);
    opvar_free(mask);
    opvar_free(pos);
}

/*ARGSUSED*/
void
sel_set_wallify(x, y, arg)
int x, y;
genericptr_t arg UNUSED;
{
    wallify_map(x, y, x, y);
}

void
spo_wallify(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_wallify";
    struct opvar *typ, *r;
    int dx1, dy1, dx2, dy2;

    if (!OV_pop_i(typ))
        return;
    switch (OV_i(typ)) {
    default:
    case 0:
        if (!OV_pop_r(r))
            return;
        dx1 = (xchar) SP_REGION_X1(OV_i(r));
        dy1 = (xchar) SP_REGION_Y1(OV_i(r));
        dx2 = (xchar) SP_REGION_X2(OV_i(r));
        dy2 = (xchar) SP_REGION_Y2(OV_i(r));
        wallify_map(dx1 < 0 ? (xstart - 1) : dx1,
                    dy1 < 0 ? (ystart - 1) : dy1,
                    dx2 < 0 ? (xstart + xsize + 1) : dx2,
                    dy2 < 0 ? (ystart + ysize + 1) : dy2);
        break;
    case 1:
        if (!OV_pop_typ(r, SPOVAR_SEL))
            return;
        selection_iterate(r, sel_set_wallify, NULL);
        break;
    case 2:
        if (!OV_pop_r(r))
            return;
        wallify_entire_map();
        break;
    }
    opvar_free(r);
    opvar_free(typ);
}

void
spo_map(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_map";
    mazepart tmpmazepart;
    struct opvar *mpxs, *mpys, *mpmap, *mpa, *mpkeepr, *mpzalign;
    xchar halign, valign;
    xchar tmpxstart, tmpystart, tmpxsize, tmpysize;
    unpacked_coord upc;

    if (!OV_pop_i(mpxs) || !OV_pop_i(mpys) || !OV_pop_s(mpmap)
        || !OV_pop_i(mpkeepr) || !OV_pop_i(mpzalign) || !OV_pop_c(mpa))
        return;

    tmpmazepart.xsize = (char)OV_i(mpxs);
    tmpmazepart.ysize = (char)OV_i(mpys);
    tmpmazepart.zaligntyp = (schar)OV_i(mpzalign);

    upc = get_unpacked_coord(OV_i(mpa), ANY_LOC);
    tmpmazepart.halign = upc.x;
    tmpmazepart.valign = upc.y;

    tmpxsize = xsize;
    tmpysize = ysize;
    tmpxstart = xstart;
    tmpystart = ystart;

    halign = tmpmazepart.halign;
    valign = tmpmazepart.valign;
    xsize = tmpmazepart.xsize;
    ysize = tmpmazepart.ysize;
    switch (tmpmazepart.zaligntyp) {
    default:
    case 0:
        break;
    case 1:
        switch ((int) halign) {
        case LEFT:
            xstart = coder->no_map_padding ? 1 : splev_init_present ? 1 : 3;
            break;
        case H_LEFT:
            xstart = (coder->no_map_padding ? 1 : 2) + ((x_maze_max - (coder->no_map_padding ? 1 : 2) - xsize) / 4);
            break;
        case CENTER:
            xstart = (coder->no_map_padding ? 1 : 2) + ((x_maze_max - (coder->no_map_padding ? 1 : 2) - xsize) / 2);
            break;
        case H_RIGHT:
            xstart = (coder->no_map_padding ? 1 : 2) + ((x_maze_max - (coder->no_map_padding ? 1 : 2) - xsize) * 3 / 4);
            break;
        case RIGHT:
            xstart = x_maze_max - xsize;
            break;
        }
        switch ((int) valign) {
        case TOP:
            ystart = coder->no_map_padding ? 0 : 3;
            break;
        case CENTER:
            ystart = (coder->no_map_padding ? 0 : 2) + ((y_maze_max - (coder->no_map_padding ? 0 : 2) - ysize) / 2);
            break;
        case BOTTOM:
            ystart = y_maze_max - ysize - 1;
            break;
        }
        if (!(xstart % 2))
            xstart++;
        if (!(ystart % 2))
            ystart++;
        break;
    case 2:
        if (!coder->croom) {
            xstart = 1;
            ystart = 0;
            xsize = COLNO - 1 - tmpmazepart.xsize;
            ysize = ROWNO - tmpmazepart.ysize;
        }
        get_location_coord(&halign, &valign, ANY_LOC, coder->croom,
                           OV_i(mpa));
        xsize = tmpmazepart.xsize;
        ysize = tmpmazepart.ysize;
        xstart = halign;
        ystart = valign;
        break;
    }
    if (ystart < 0 || ystart + ysize > ROWNO) {
        /* try to move the start a bit */
        ystart += (ystart > 0) ? -2 : 2;
        if (ysize == ROWNO)
            ystart = 0;
        if (ystart < 0 || ystart + ysize > ROWNO)
            panic("reading special level with ysize too large");
    }
    if (xsize <= 1 && ysize <= 1) {
        xstart = 1;
        ystart = 0;
        xsize = COLNO - 1;
        ysize = ROWNO;
    } else {
        xchar x, y, mptyp;

        /* Load the map */
        for (y = ystart; y < ystart + ysize; y++)
            for (x = xstart; x < xstart + xsize; x++) {
                mptyp = (mpmap->vardata.str[(y - ystart) * xsize
                                                  + (x - xstart)] - 1);
                if (mptyp >= MAX_LEVTYPE)
                    continue;

                struct rm* lev = &levl[x][y];
                lev->typ = mptyp;
                lev->subtyp = get_initial_location_subtype(lev->typ);
                lev->vartyp = get_initial_location_vartype(lev->typ, lev->subtyp);
                lev->special_quality = 0;
                set_initial_location_floortype(lev, coder->fountain_on_grass, coder->fountain_on_ground, coder->tree_on_ground, coder->throne_on_ground);
                initialize_location(lev);

                lev->lit = FALSE;
                /* clear out levl: load_common_data may set them */
                lev->flags = 0;
                lev->horizontal = 0;
                lev->roomno = 0;
                lev->edge = 0;
                SpLev_Map[x][y] = 1;
                /*
                 *  Set secret doors to closed (why not trapped too?).  Set
                 *  the horizontal bit.
                 */
                if (levl[x][y].typ == SDOOR || IS_DOOR(levl[x][y].typ)) 
                {
                    if (levl[x][y].typ == SDOOR)
                        levl[x][y].doormask = D_CLOSED;
                    /*
                     *  If there is a wall to the left that connects to a
                     *  (secret) door, then it is horizontal.  This does
                     *  not allow (secret) doors to be corners of rooms.
                     */
                    if (x != xstart && (IS_WALL(levl[x - 1][y].typ)
                                        || levl[x - 1][y].horizontal))
                        levl[x][y].horizontal = 1;
                } 
                else if (levl[x][y].typ == HWALL)
                    levl[x][y].horizontal = 1;
                else if (levl[x][y].typ == IRONBARS)
                {
                    if(!isok(x, y - 1) || IS_DOORJOIN(levl[x][y - 1].typ) || IS_DOOR(levl[x][y - 1].typ))
                        levl[x][y].horizontal = 0;
                    else
                    {
                        levl[x][y].horizontal = 1;
                        if (isok(x, y - 1) && IS_FLOOR(levl[x][y - 1].typ))
                        {
                            levl[x][y].floortyp = levl[x][y - 1].typ;
                            levl[x][y].floorsubtyp = get_initial_location_subtype(levl[x][y].floortyp);
                            levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
                        }

                    }
                }
                else if (levl[x][y].typ == LAVAPOOL)
                    levl[x][y].lit = 1;
                else if (splev_init_present && levl[x][y].typ == ICE)
                    levl[x][y].icedpool = icedpools ? ICED_POOL : ICED_MOAT;
            }
        if (coder->lvl_is_joined)
            remove_rooms(xstart, ystart, xstart + xsize, ystart + ysize);
    }
    if (!OV_i(mpkeepr)) {
        xstart = tmpxstart;
        ystart = tmpystart;
        xsize = tmpxsize;
        ysize = tmpysize;
    }

    opvar_free(mpxs);
    opvar_free(mpys);
    opvar_free(mpmap);
    opvar_free(mpa);
    opvar_free(mpkeepr);
    opvar_free(mpzalign);
}

void
spo_jmp(coder, lvl)
struct sp_coder *coder;
sp_lev *lvl;
{
    static const char nhFunc[] = "spo_jmp";
    struct opvar *tmpa;
    long a;

    if (!OV_pop_i(tmpa))
        return;
    a = sp_code_jmpaddr(coder->frame->n_opcode, (OV_i(tmpa) - 1));
    if ((a >= 0) && (a < lvl->n_opcodes) && (a != coder->frame->n_opcode))
        coder->frame->n_opcode = a;
    opvar_free(tmpa);
}

void
spo_conditional_jump(coder, lvl)
struct sp_coder *coder;
sp_lev *lvl;
{
    static const char nhFunc[] = "spo_conditional_jump";
    struct opvar *oa, *oc;
    long a, c;
    long test = 0;

    if (!OV_pop_i(oa) || !OV_pop_i(oc))
        return;

    a = sp_code_jmpaddr(coder->frame->n_opcode, (OV_i(oa) - 1));
    c = OV_i(oc);

    switch (coder->opcode) {
    default:
        impossible("spo_conditional_jump: illegal opcode");
        break;
    case SPO_JL:
        test = (c & SP_CPUFLAG_LT);
        break;
    case SPO_JLE:
        test = (c & (SP_CPUFLAG_LT | SP_CPUFLAG_EQ));
        break;
    case SPO_JG:
        test = (c & SP_CPUFLAG_GT);
        break;
    case SPO_JGE:
        test = (c & (SP_CPUFLAG_GT | SP_CPUFLAG_EQ));
        break;
    case SPO_JE:
        test = (c & SP_CPUFLAG_EQ);
        break;
    case SPO_JNE:
        test = (c & ~SP_CPUFLAG_EQ);
        break;
    }

    if ((test) && (a >= 0) && (a < lvl->n_opcodes)
        && (a != coder->frame->n_opcode))
        coder->frame->n_opcode = a;

    opvar_free(oa);
    opvar_free(oc);
}

void
spo_var_init(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_var_init";
    struct opvar *vname;
    struct opvar *arraylen;
    struct opvar *vvalue;
    struct splev_var *tmpvar;
    struct splev_var *tmp2;
    long idx;

    OV_pop_s(vname);
    OV_pop_i(arraylen);

    if (!vname || !arraylen)
    {
        panic("no values for SPO_VAR_INIT");
        return;
    }

    tmpvar = opvar_var_defined(coder, OV_s(vname));

    if (tmpvar) {
        /* variable redefinition */
        if (OV_i(arraylen) < 0) {
            /* copy variable */
            if (tmpvar->array_len) {
                idx = tmpvar->array_len;
                while (idx-- > 0) {
                    opvar_free(tmpvar->data.arrayvalues[idx]);
                }
                Free(tmpvar->data.arrayvalues);
            } else {
                opvar_free(tmpvar->data.value);
            }
            tmpvar->data.arrayvalues = NULL;
            goto copy_variable;
        } else if (OV_i(arraylen)) {
            /* redefined array */
            idx = tmpvar->array_len;
            while (idx-- > 0) {
                opvar_free(tmpvar->data.arrayvalues[idx]);
            }
            Free(tmpvar->data.arrayvalues);
            tmpvar->data.arrayvalues = NULL;
            goto create_new_array;
        } else {
            /* redefined single value */
            OV_pop(vvalue);
            if (tmpvar->svtyp != vvalue->spovartyp)
            {
                panic("redefining variable as different type");
                return;
            }
            opvar_free(tmpvar->data.value);
            tmpvar->data.value = vvalue;
            tmpvar->array_len = 0;
        }
    } else {
        /* new variable definition */
        tmpvar = (struct splev_var *) alloc(sizeof(struct splev_var));
        tmpvar->next = coder->frame->variables;
        tmpvar->name = dupstr(OV_s(vname));
        coder->frame->variables = tmpvar;

        if (OV_i(arraylen) < 0) {
        /* copy variable */
        copy_variable:
            OV_pop(vvalue);
            tmp2 = opvar_var_defined(coder, OV_s(vvalue));
            if (!tmp2)
            {
                panic("no copyable var");
                return;
            }
            tmpvar->svtyp = tmp2->svtyp;
            tmpvar->array_len = tmp2->array_len;
            if (tmpvar->array_len) {
                idx = tmpvar->array_len;
                tmpvar->data.arrayvalues =
                    (struct opvar **) alloc(sizeof(struct opvar *) * (size_t)idx);
                while (idx-- > 0) {
                    tmpvar->data.arrayvalues[idx] =
                        opvar_clone(tmp2->data.arrayvalues[idx]);
                }
            } else {
                tmpvar->data.value = opvar_clone(tmp2->data.value);
            }
            opvar_free(vvalue);
        } else if (OV_i(arraylen)) {
        /* new array */
        create_new_array:
            idx = OV_i(arraylen);
            tmpvar->array_len = idx;
            tmpvar->data.arrayvalues =
                (struct opvar **) alloc(sizeof(struct opvar *) * (size_t)idx);
            while (idx-- > 0) {
                OV_pop(vvalue);
                if (!vvalue)
                {
                    panic("no value for arrayvariable");
                    return;
                }
                tmpvar->data.arrayvalues[idx] = vvalue;
            }
            tmpvar->svtyp = SPOVAR_ARRAY;
        } else {
            /* new single value */
            OV_pop(vvalue);
            if (!vvalue)
            {
                panic("no value for variable");
                return;
            }
            tmpvar->svtyp = OV_typ(vvalue);
            tmpvar->data.value = vvalue;
            tmpvar->array_len = 0;
        }
    }

    opvar_free(vname);
    opvar_free(arraylen);

}

#if 0
STATIC_OVL long
opvar_array_length(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "opvar_array_length";
    struct opvar *vname;
    struct splev_var *tmp;
    long len = 0;

    if (!coder)
        return 0;

    vname = splev_stack_pop(coder->stack);
    if (!vname)
        return 0;
    if (vname->spovartyp != SPOVAR_VARIABLE)
        goto pass;

    tmp = coder->frame->variables;
    while (tmp) {
        if (!strcmp(tmp->name, OV_s(vname))) {
            if ((tmp->svtyp & SPOVAR_ARRAY)) {
                len = tmp->array_len;
                if (len < 1)
                    len = 0;
            }
            goto pass;
        }
        tmp = tmp->next;
    }

pass:
    opvar_free(vname);
    return len;
}
#endif /*0*/

void
spo_shuffle_array(coder)
struct sp_coder *coder;
{
    static const char nhFunc[] = "spo_shuffle_array";
    struct opvar *vname;
    struct splev_var *tmp;
    struct opvar *tmp2;
    long i, j;

    if (!OV_pop_s(vname))
        return;

    tmp = opvar_var_defined(coder, OV_s(vname));
    if (!tmp || (tmp->array_len < 1)) {
        opvar_free(vname);
        return;
    }
    for (i = tmp->array_len - 1; i > 0; i--) {
        if ((j = (long)rn2((int)i + 1)) == i)
            continue;
        tmp2 = tmp->data.arrayvalues[j];
        tmp->data.arrayvalues[j] = tmp->data.arrayvalues[i];
        tmp->data.arrayvalues[i] = tmp2;
    }

    opvar_free(vname);
}

/* Special level coder, creates the special level from the sp_lev codes.
 * Does not free the allocated memory.
 */
STATIC_OVL boolean
sp_level_coder(lvl)
sp_lev *lvl;
{
    static const char nhFunc[] = "sp_level_coder";
    unsigned long exec_opcodes = 0;
    int tmpi;
    long room_stack = 0;
    unsigned long max_execution = SPCODER_MAX_RUNTIME;
    struct sp_coder *coder =
        (struct sp_coder *) alloc(sizeof (struct sp_coder));

    coder->frame = frame_new(0);
    coder->stack = NULL;
    coder->premapped = FALSE;
    coder->solidify = FALSE;
    coder->check_inaccessibles = FALSE;
    coder->fountain_on_grass = FALSE;
    coder->fountain_on_ground = FALSE;
    coder->tree_on_ground = FALSE;
    coder->throne_on_ground = FALSE;
    coder->no_map_padding = FALSE;
    coder->croom = NULL;
    coder->n_subroom = 1;
    coder->exit_script = FALSE;
    coder->lvl_is_joined = 0;

    splev_init_present = FALSE;
    icedpools = FALSE;
    /* achievement tracking; static init would suffice except we need to
       reset if #wizmakemap is used to recreate mines' end or sokoban end;
       once either level is created, these values can be forgotten */
    mines_prize_count = soko_prize_count = 0;

    if (wizard) {
        char *met = nh_getenv("SPCODER_MAX_RUNTIME");

        if (met && met[0] == '1')
            max_execution = (1 << 30) - 1;
    }

    for (tmpi = 0; tmpi <= MAX_NESTED_ROOMS; tmpi++) {
        coder->tmproomlist[tmpi] = (struct mkroom *) 0;
        coder->failed_room[tmpi] = FALSE;
    }

    shuffle_alignments();

    for (tmpi = 0; tmpi < MAX_CONTAINMENT; tmpi++)
        container_obj[tmpi] = NULL;
    container_idx = 0;

    invent_carrying_monster = NULL;

    (void) memset((genericptr_t) &SpLev_Map[0][0], 0, sizeof SpLev_Map);

    level.flags.is_maze_lev = 0;

    xstart = 1;
    ystart = 0;
    xsize = COLNO - 1;
    ysize = ROWNO;

    while (coder->frame->n_opcode < lvl->n_opcodes && !coder->exit_script) {
        coder->opcode = lvl->opcodes[coder->frame->n_opcode].opcode;
        coder->opdat = lvl->opcodes[coder->frame->n_opcode].opdat;

        coder->stack = coder->frame->stack;

        if (exec_opcodes++ > max_execution) {
            impossible("Level script is taking too much time, stopping.");
            coder->exit_script = TRUE;
        }

        if (coder->failed_room[coder->n_subroom - 1]
            && coder->opcode != SPO_ENDROOM && coder->opcode != SPO_ROOM
            && coder->opcode != SPO_SUBROOM)
            goto next_opcode;

        coder->croom = coder->tmproomlist[coder->n_subroom - 1];

        switch (coder->opcode) {
        case SPO_NULL:
            break;
        case SPO_EXIT:
            coder->exit_script = TRUE;
            break;
        case SPO_FRAME_PUSH:
            spo_frame_push(coder);
            break;
        case SPO_FRAME_POP:
            spo_frame_pop(coder);
            break;
        case SPO_CALL:
            spo_call(coder);
            break;
        case SPO_RETURN:
            spo_return(coder);
            break;
        case SPO_END_MONINVENT:
            spo_end_moninvent(coder);
            break;
        case SPO_POP_CONTAINER:
            spo_pop_container(coder);
            break;
        case SPO_POP: {
            struct opvar *ov = splev_stack_pop(coder->stack);

            opvar_free(ov);
            break;
        }
        case SPO_PUSH:
            splev_stack_push(coder->stack, opvar_clone(coder->opdat));
            break;
        case SPO_MESSAGE:
            spo_message(coder);
            break;
        case SPO_MONSTER:
            spo_monster(coder);
            break;
        case SPO_OBJECT:
            spo_object(coder);
            break;
        case SPO_LEVER:
            spo_lever(coder);
            break;
        case SPO_LEVEL_FLAGS:
            spo_level_flags(coder);
            break;
        case SPO_INITLEVEL:
            spo_initlevel(coder);
            break;
        case SPO_TILESET:
            spo_tileset(coder);
            break;
        case SPO_FOREST:
            spo_forest(coder);
            break;
        case SPO_MONSTER_GENERATION:
            spo_monster_generation(coder);
            break;
        case SPO_BOUNDARY_TYPE:
            spo_boundary_type(coder);
            break;
        case SPO_ENGRAVING:
            spo_engraving(coder);
            break;
        case SPO_MINERALIZE:
            spo_mineralize(coder);
            break;
        case SPO_SUBROOM:
        case SPO_ROOM:
            if (!coder->failed_room[coder->n_subroom - 1]) {
                spo_room(coder);
            } else
                room_stack++;
            break;
        case SPO_ENDROOM:
            if (coder->failed_room[coder->n_subroom - 1]) {
                if (!room_stack)
                    spo_endroom(coder);
                else
                    room_stack--;
            } else {
                spo_endroom(coder);
            }
            break;
        case SPO_DOOR:
            spo_door(coder);
            break;
        case SPO_STAIR:
            spo_stair(coder);
            break;
        case SPO_LADDER:
            spo_ladder(coder);
            break;
        case SPO_GRAVE:
            spo_grave(coder);
            break;
        case SPO_BRAZIER:
            spo_brazier(coder);
            break;
        case SPO_SIGNPOST:
            spo_signpost(coder);
            break;
        case SPO_TREE:
            spo_tree(coder);
            break;
        case SPO_ALTAR:
            spo_altar(coder);
            break;
        case SPO_SINK:
        case SPO_POOL:
        case SPO_THRONE:
            spo_feature(coder);
            break;
        case SPO_FOUNTAIN:
            spo_fountain(coder);
            break;
        case SPO_ANVIL:
            spo_anvil(coder);
            break;
        case SPO_DECORATION:
            spo_decoration(coder);
            break;
        case SPO_FLOOR:
            spo_floor(coder);
            break;
        case SPO_SUBTYPE:
            spo_subtype(coder);
            break;
        case SPO_CARPET:
            spo_carpet(coder);
            break;
        case SPO_CARPET_PIECE:
            spo_carpet_piece(coder);
            break;
        case SPO_NPC:
            spo_npc(coder);
            break;
        case SPO_MODRON_PORTAL:
            spo_modron_portal(coder);
            break;
        case SPO_MODRON_LEVEL_TELEPORTER:
            spo_modron_level_teleporter(coder);
            break;
        case SPO_TRAP:
            spo_trap(coder);
            break;
        case SPO_GOLD:
            spo_gold(coder);
            break;
        case SPO_CORRIDOR:
            spo_corridor(coder);
            break;
        case SPO_TERRAIN:
            spo_terrain(coder);
            break;
        case SPO_REPLACETERRAIN:
            spo_replace_terrain(coder);
            break;
        case SPO_LEVREGION:
            spo_levregion(coder);
            break;
        case SPO_REGION:
            spo_region(coder);
            break;
        case SPO_SPECIAL_REGION:
            spo_special_region(coder);
            break;
        case SPO_SPECIAL_TILESET:
            spo_special_tileset(coder);
            break;
        case SPO_NAMING:
            spo_naming(coder);
            break;
        case SPO_DRAWBRIDGE:
            spo_drawbridge(coder);
            break;
        case SPO_MAZEWALK:
            spo_mazewalk(coder);
            break;
        case SPO_NON_PASSWALL:
        case SPO_NON_DIGGABLE:
            spo_wall_property(coder);
            break;
        case SPO_ROOM_DOOR:
            spo_room_door(coder);
            break;
        case SPO_WALLIFY:
            spo_wallify(coder);
            break;
        case SPO_COPY: {
            struct opvar *a = splev_stack_pop(coder->stack);

            splev_stack_push(coder->stack, opvar_clone(a));
            splev_stack_push(coder->stack, opvar_clone(a));
            opvar_free(a);
            break;
        }
        case SPO_DEC: {
            struct opvar *a;

            if (!OV_pop_i(a))
                break;
            OV_i(a)--;
            splev_stack_push(coder->stack, a);
            break;
        }
        case SPO_INC: {
            struct opvar *a;

            if (!OV_pop_i(a))
                break;
            OV_i(a)++;
            splev_stack_push(coder->stack, a);
            break;
        }
        case SPO_MATH_SIGN: {
            struct opvar *a;

            if (!OV_pop_i(a))
                break;
            OV_i(a) = ((OV_i(a) < 0) ? -1 : ((OV_i(a) > 0) ? 1 : 0));
            splev_stack_push(coder->stack, a);
            break;
        }
        case SPO_MATH_ADD: {
            struct opvar *a, *b;

            if (!OV_pop(b) || !OV_pop(a))
                break;
            if (OV_typ(b) == OV_typ(a)) {
                if (OV_typ(a) == SPOVAR_INT) {
                    OV_i(a) = OV_i(a) + OV_i(b);
                    splev_stack_push(coder->stack, a);
                    opvar_free(b);
                } else if (OV_typ(a) == SPOVAR_STRING) {
                    struct opvar *c;
                    char *tmpbuf = (char *) alloc(strlen(OV_s(a))
                                                  + strlen(OV_s(b)) + 1);

                    (void) sprintf(tmpbuf, "%s%s", OV_s(a), OV_s(b));
                    c = opvar_new_str(tmpbuf);
                    splev_stack_push(coder->stack, c);
                    opvar_free(a);
                    opvar_free(b);
                    Free(tmpbuf);
                } else {
                    splev_stack_push(coder->stack, a);
                    opvar_free(b);
                    impossible("adding weird types");
                }
            } else {
                splev_stack_push(coder->stack, a);
                opvar_free(b);
                impossible("adding different types");
            }
            break;
        }
        case SPO_MATH_SUB: {
            struct opvar *a, *b;

            if (!OV_pop_i(b) || !OV_pop_i(a))
                break;
            OV_i(a) = OV_i(a) - OV_i(b);
            splev_stack_push(coder->stack, a);
            opvar_free(b);
            break;
        }
        case SPO_MATH_MUL: {
            struct opvar *a, *b;

            if (!OV_pop_i(b) || !OV_pop_i(a))
                break;
            OV_i(a) = OV_i(a) * OV_i(b);
            splev_stack_push(coder->stack, a);
            opvar_free(b);
            break;
        }
        case SPO_MATH_DIV: {
            struct opvar *a, *b;

            if (!OV_pop_i(b) || !OV_pop_i(a))
                break;
            if (OV_i(b) >= 1) {
                OV_i(a) = OV_i(a) / OV_i(b);
            } else {
                OV_i(a) = 0;
            }
            splev_stack_push(coder->stack, a);
            opvar_free(b);
            break;
        }
        case SPO_MATH_MOD: {
            struct opvar *a, *b;

            if (!OV_pop_i(b) || !OV_pop_i(a))
                break;
            if (OV_i(b) > 0) {
                OV_i(a) = OV_i(a) % OV_i(b);
            } else {
                OV_i(a) = 0;
            }
            splev_stack_push(coder->stack, a);
            opvar_free(b);
            break;
        }
        case SPO_CMP: {
            struct opvar *a;
            struct opvar *b;
            struct opvar *c;
            long val = 0;

            OV_pop(b);
            OV_pop(a);
            if (!a || !b) {
                impossible("spo_cmp: no values in stack");
                break;
            }
            if (OV_typ(a) != OV_typ(b)) {
                impossible("spo_cmp: trying to compare differing datatypes");
                break;
            }
            switch (OV_typ(a)) {
            case SPOVAR_COORD:
            case SPOVAR_REGION:
            case SPOVAR_MAPCHAR:
            case SPOVAR_MONST:
            case SPOVAR_OBJ:
            case SPOVAR_INT:
                if (OV_i(b) > OV_i(a))
                    val |= SP_CPUFLAG_LT;
                if (OV_i(b) < OV_i(a))
                    val |= SP_CPUFLAG_GT;
                if (OV_i(b) == OV_i(a))
                    val |= SP_CPUFLAG_EQ;
                c = opvar_new_int(val);
                break;
            case SPOVAR_STRING:
                c = opvar_new_int(!strcmp(OV_s(b), OV_s(a))
                                     ? SP_CPUFLAG_EQ
                                     : 0);
                break;
            default:
                c = opvar_new_int(0);
                break;
            }
            splev_stack_push(coder->stack, c);
            opvar_free(a);
            opvar_free(b);
            break;
        }
        case SPO_JMP:
            spo_jmp(coder, lvl);
            break;
        case SPO_JL:
        case SPO_JLE:
        case SPO_JG:
        case SPO_JGE:
        case SPO_JE:
        case SPO_JNE:
            spo_conditional_jump(coder, lvl);
            break;
        case SPO_RN2: {
            struct opvar *tmpv;
            struct opvar *t;

            if (!OV_pop_i(tmpv))
                break;
            t = opvar_new_int((OV_i(tmpv) > 1) ? rn2((int)OV_i(tmpv)) : 0);
            splev_stack_push(coder->stack, t);
            opvar_free(tmpv);
            break;
        }
        case SPO_DICE: {
            struct opvar *a, *b, *t;

            if (!OV_pop_i(b) || !OV_pop_i(a))
                break;
            if (OV_i(b) < 1)
                OV_i(b) = 1;
            if (OV_i(a) < 1)
                OV_i(a) = 1;
            t = opvar_new_int(d((int)OV_i(a), (int)OV_i(b)));
            splev_stack_push(coder->stack, t);
            opvar_free(a);
            opvar_free(b);
            break;
        }
        case SPO_MAP:
            spo_map(coder);
            break;
        case SPO_VAR_INIT:
            spo_var_init(coder);
            break;
        case SPO_SHUFFLE_ARRAY:
            spo_shuffle_array(coder);
            break;
        case SPO_SEL_ADD: /* actually, logical or */
        {
            struct opvar *sel1, *sel2, *pt;

            if (!OV_pop_typ(sel1, SPOVAR_SEL))
                panic("no sel1 for add");
            if (!OV_pop_typ(sel2, SPOVAR_SEL))
                panic("no sel2 for add");
            pt = selection_logical_oper(sel1, sel2, '|');
            opvar_free(sel1);
            opvar_free(sel2);
            splev_stack_push(coder->stack, pt);
            break;
        }
        case SPO_SEL_COMPLEMENT: {
            struct opvar *sel, *pt;

            if (!OV_pop_typ(sel, SPOVAR_SEL))
                panic("no sel for not");
            pt = selection_not(sel);
            opvar_free(sel);
            splev_stack_push(coder->stack, pt);
            break;
        }
        case SPO_SEL_FILTER: /* sorta like logical and */
        {
            struct opvar *filtertype;

            if (!OV_pop_i(filtertype))
                panic("no sel filter type");
            switch (OV_i(filtertype)) {
            case SPOFILTER_PERCENT: {
                struct opvar *tmp1, *sel;

                if (!OV_pop_i(tmp1))
                    panic("no sel filter percent");
                if (!OV_pop_typ(sel, SPOVAR_SEL))
                    panic("no sel filter");
                selection_filter_percent(sel, (int)OV_i(tmp1));
                splev_stack_push(coder->stack, sel);
                opvar_free(tmp1);
                break;
            }
            case SPOFILTER_SELECTION: /* logical and */
            {
                struct opvar *pt, *sel1, *sel2;

                if (!OV_pop_typ(sel1, SPOVAR_SEL))
                    panic("no sel filter sel1");
                if (!OV_pop_typ(sel2, SPOVAR_SEL))
                    panic("no sel filter sel2");
                pt = selection_logical_oper(sel1, sel2, '&');
                splev_stack_push(coder->stack, pt);
                opvar_free(sel1);
                opvar_free(sel2);
                break;
            }
            case SPOFILTER_MAPCHAR: {
                struct opvar *pt, *tmp1, *sel;

                if (!OV_pop_typ(sel, SPOVAR_SEL))
                    panic("no sel filter");
                if (!OV_pop_typ(tmp1, SPOVAR_MAPCHAR))
                    panic("no sel filter mapchar");
                pt = selection_filter_mapchar(sel, tmp1);
                splev_stack_push(coder->stack, pt);
                opvar_free(tmp1);
                opvar_free(sel);
                break;
            }
            default:
                panic("unknown sel filter type");
            }
            opvar_free(filtertype);
            break;
        }
        case SPO_SEL_POINT: {
            struct opvar *tmp;
            struct opvar *pt = selection_opvar((char *) 0);
            schar x, y;

            if (!OV_pop_c(tmp))
                panic("no ter sel coord");
            get_location_coord(&x, &y, ANY_LOC, coder->croom, OV_i(tmp));
            selection_setpoint(x, y, pt, 1);
            splev_stack_push(coder->stack, pt);
            opvar_free(tmp);
            break;
        }
        case SPO_SEL_RECT:
        case SPO_SEL_FILLRECT: {
            struct opvar *tmp, *pt = selection_opvar((char *) 0);
            schar x, y, x1, y1, x2, y2;

            if (!OV_pop_r(tmp))
                panic("no ter sel region");
            x1 = min(SP_REGION_X1(OV_i(tmp)), SP_REGION_X2(OV_i(tmp)));
            y1 = min(SP_REGION_Y1(OV_i(tmp)), SP_REGION_Y2(OV_i(tmp)));
            x2 = max(SP_REGION_X1(OV_i(tmp)), SP_REGION_X2(OV_i(tmp)));
            y2 = max(SP_REGION_Y1(OV_i(tmp)), SP_REGION_Y2(OV_i(tmp)));
            get_location(&x1, &y1, ANY_LOC, coder->croom);
            get_location(&x2, &y2, ANY_LOC, coder->croom);
            x1 = (x1 < 0) ? 0 : x1;
            y1 = (y1 < 0) ? 0 : y1;
            x2 = (x2 >= COLNO) ? COLNO - 1 : x2;
            y2 = (y2 >= ROWNO) ? ROWNO - 1 : y2;
            if (coder->opcode == SPO_SEL_RECT) {
                for (x = x1; x <= x2; x++) {
                    selection_setpoint(x, y1, pt, 1);
                    selection_setpoint(x, y2, pt, 1);
                }
                for (y = y1; y <= y2; y++) {
                    selection_setpoint(x1, y, pt, 1);
                    selection_setpoint(x2, y, pt, 1);
                }
            } else {
                for (x = x1; x <= x2; x++)
                    for (y = y1; y <= y2; y++)
                        selection_setpoint(x, y, pt, 1);
            }
            splev_stack_push(coder->stack, pt);
            opvar_free(tmp);
            break;
        }
        case SPO_SEL_LINE: {
            struct opvar *tmp = NULL, *tmp2 = NULL,
                *pt = selection_opvar((char *) 0);
            schar x1, y1, x2, y2;

            if (!OV_pop_c(tmp))
                panic("no ter sel linecoord1");
            if (!OV_pop_c(tmp2))
                panic("no ter sel linecoord2");
            get_location_coord(&x1, &y1, ANY_LOC, coder->croom, OV_i(tmp));
            get_location_coord(&x2, &y2, ANY_LOC, coder->croom, OV_i(tmp2));
            x1 = (x1 < 0) ? 0 : x1;
            y1 = (y1 < 0) ? 0 : y1;
            x2 = (x2 >= COLNO) ? COLNO - 1 : x2;
            y2 = (y2 >= ROWNO) ? ROWNO - 1 : y2;
            selection_do_line(x1, y1, x2, y2, pt);
            splev_stack_push(coder->stack, pt);
            opvar_free(tmp);
            opvar_free(tmp2);
            break;
        }
        case SPO_SEL_RNDLINE: {
            struct opvar *tmp = NULL, *tmp2 = NULL, *tmp3,
                *pt = selection_opvar((char *) 0);
            schar x1, y1, x2, y2;

            if (!OV_pop_i(tmp3))
                panic("no ter sel randline1");
            if (!OV_pop_c(tmp))
                panic("no ter sel randline2");
            if (!OV_pop_c(tmp2))
                panic("no ter sel randline3");
            get_location_coord(&x1, &y1, ANY_LOC, coder->croom, OV_i(tmp));
            get_location_coord(&x2, &y2, ANY_LOC, coder->croom, OV_i(tmp2));
            x1 = (x1 < 0) ? 0 : x1;
            y1 = (y1 < 0) ? 0 : y1;
            x2 = (x2 >= COLNO) ? COLNO - 1 : x2;
            y2 = (y2 >= ROWNO) ? ROWNO - 1 : y2;
            selection_do_randline(x1, y1, x2, y2, (schar)OV_i(tmp3), 12, pt);
            splev_stack_push(coder->stack, pt);
            opvar_free(tmp);
            opvar_free(tmp2);
            opvar_free(tmp3);
            break;
        }
        case SPO_SEL_GROW: {
            struct opvar *dirs, *pt;

            if (!OV_pop_i(dirs))
                panic("no dirs for grow");
            if (!OV_pop_typ(pt, SPOVAR_SEL))
                panic("no selection for grow");
            selection_do_grow(pt, (int)OV_i(dirs));
            splev_stack_push(coder->stack, pt);
            opvar_free(dirs);
            break;
        }
        case SPO_SEL_FLOOD: {
            struct opvar *tmp;
            schar x, y;

            if (!OV_pop_c(tmp))
                panic("no ter sel flood coord");
            get_location_coord(&x, &y, ANY_LOC, coder->croom, OV_i(tmp));
            if (isok(x, y)) {
                struct opvar *pt = selection_opvar((char *) 0);

                set_selection_floodfillchk(floodfillchk_match_under);
                floodfillchk_match_under_typ = levl[x][y].typ;
                selection_floodfill(pt, x, y, FALSE);
                splev_stack_push(coder->stack, pt);
            }
            opvar_free(tmp);
            break;
        }
        case SPO_SEL_RNDCOORD: {
            struct opvar *pt;
            schar x, y;

            if (!OV_pop_typ(pt, SPOVAR_SEL))
                panic("no selection for rndcoord");
            if (selection_rndcoord(pt, &x, &y, FALSE)) {
                x -= xstart;
                y -= ystart;
            }
            splev_stack_push(coder->stack, opvar_new_coord(x, y));
            opvar_free(pt);
            break;
        }
        case SPO_SEL_ELLIPSE: {
            struct opvar *filled, *xaxis, *yaxis, *pt;
            struct opvar *sel = selection_opvar((char *) 0);
            schar x, y;

            if (!OV_pop_i(filled))
                panic("no filled for ellipse");
            if (!OV_pop_i(yaxis))
                panic("no yaxis for ellipse");
            if (!OV_pop_i(xaxis))
                panic("no xaxis for ellipse");
            if (!OV_pop_c(pt))
                panic("no pt for ellipse");
            get_location_coord(&x, &y, ANY_LOC, coder->croom, OV_i(pt));
            selection_do_ellipse(sel, x, y, (int)OV_i(xaxis), (int)OV_i(yaxis),
                (int)OV_i(filled));
            splev_stack_push(coder->stack, sel);
            opvar_free(filled);
            opvar_free(yaxis);
            opvar_free(xaxis);
            opvar_free(pt);
            break;
        }
        case SPO_SEL_GRADIENT: {
            struct opvar *gtyp, *glim, *mind, *maxd, *gcoord, *coord2;
            struct opvar *sel;
            schar x, y, x2, y2;

            if (!OV_pop_i(gtyp))
                panic("no gtyp for grad");
            if (!OV_pop_i(glim))
                panic("no glim for grad");
            if (!OV_pop_c(coord2))
                panic("no coord2 for grad");
            if (!OV_pop_c(gcoord))
                panic("no coord for grad");
            if (!OV_pop_i(maxd))
                panic("no maxd for grad");
            if (!OV_pop_i(mind))
                panic("no mind for grad");
            get_location_coord(&x, &y, ANY_LOC, coder->croom, OV_i(gcoord));
            get_location_coord(&x2, &y2, ANY_LOC, coder->croom, OV_i(coord2));

            sel = selection_opvar((char *) 0);
            selection_do_gradient(sel, x, y, x2, y2, OV_i(gtyp), OV_i(mind),
                                  OV_i(maxd), OV_i(glim));
            splev_stack_push(coder->stack, sel);

            opvar_free(gtyp);
            opvar_free(glim);
            opvar_free(gcoord);
            opvar_free(coord2);
            opvar_free(maxd);
            opvar_free(mind);
            break;
        }
        default:
            panic("sp_level_coder: Unknown opcode %i", coder->opcode);
        }

    next_opcode:
        coder->frame->n_opcode++;
    } /*while*/

    link_doors_rooms();
    fill_rooms();
    remove_boundary_syms();

    if (coder->check_inaccessibles)
        ensure_way_out();
    /* FIXME: Ideally, we want this call to only cover areas of the map
     * which were not inserted directly by the special level file (see
     * the insect legs on Baalzebub's level, for instance). Since that
     * is currently not possible, we overload the corrmaze flag for this
     * purpose.
     */

    //if (!level.flags.corrmaze)
    wallification(1, 0, COLNO - 1, ROWNO - 1);

    count_features();

    if (coder->solidify)
        solidify_map();

    /* This must be done before sokoban_detect(),
     * otherwise branch stairs won't be premapped. */
    fixup_special();

    create_level_light_sources();
    create_level_sound_sources();
    define_level_location_floor_types(coder->fountain_on_grass, coder->fountain_on_ground, coder->tree_on_ground, coder->throne_on_ground);

    if (coder->premapped)
        sokoban_detect();

    if (coder->frame) {
        struct sp_frame *tmpframe;
        do {
            tmpframe = coder->frame->next;
            frame_del(coder->frame);
            coder->frame = tmpframe;
        } while (coder->frame);
    }
    Free(coder);

    return TRUE;
}

/*
 * General loader
 */
boolean
load_special(name)
const char *name;
{
    dlb *fd;
    sp_lev *lvl = NULL;
    boolean result = FALSE;
    struct version_info vers_info;

    fd = dlb_fopen(name, RDBMODE);
    if (!fd)
        return FALSE;
    
    Fread((genericptr_t) &vers_info, sizeof vers_info, 1, fd);
    
    if (!check_version(&vers_info, name, TRUE)) {
        (void) dlb_fclose(fd);
        goto give_up;
    }

    lvl = (sp_lev *) alloc(sizeof (sp_lev));
    result = sp_level_loader(fd, lvl);
    (void) dlb_fclose(fd);
    if (result)
        result = sp_level_coder(lvl);
    sp_level_free(lvl);
    Free(lvl);

give_up:
    return result;
}

void
reset_splev(VOID_ARGS)
{
    selection_flood_check_func = 0;
    floodfillchk_match_under_typ = 0;
    xsize = ysize = 0;
    xstart = ystart = 0;
    memset((genericptr_t)&SpLev_Map, 0, sizeof(SpLev_Map));
    ralign[0] = AM_CHAOTIC;
    ralign[1] = AM_NEUTRAL;
    ralign[2] = AM_LAWFUL;

    lev_message = 0;
    lregions = 0;
    num_lregions = 0;

    splev_init_present = FALSE;
    icedpools = FALSE;
    mines_prize_count = 0, soko_prize_count = 0; /* achievements */

    memset((genericptr_t)&container_obj, 0, sizeof(container_obj));
    container_idx = 0;
    invent_carrying_monster = NULL;
}


#ifdef _MSC_VER
 #pragma warning(pop)
#endif

/*sp_lev.c*/
