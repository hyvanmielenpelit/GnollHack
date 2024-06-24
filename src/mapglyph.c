/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    mapglyph.c    $NHDT-Date: 1552945095 2019/03/18 21:38:15 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.48 $ */
/* Copyright (c) David Cohrs, 1991                                */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#if defined(TTY_GRAPHICS)
#include "wintty.h" /* for prototype of has_color() only */
#endif
#include "color.h"
#define HI_DOMESTIC CLR_WHITE /* monst.c */

#if !defined(TTY_GRAPHICS)
#define has_color(n) TRUE
#endif

#ifdef TEXTCOLOR
#define zap_color(n) color = iflags.use_color ? zap_type_definitions[n].color : NO_COLOR
#define cmap_color(n,cmap_type_index) color = iflags.use_color ? defsyms[n].color[cmap_type_index] : NO_COLOR
#define cmap_variation_color(n,cmap_type_index) color = iflags.use_color ? defsym_variations[n].color[cmap_type_index] : NO_COLOR
#define otyp_color(n) color = iflags.use_color ? objects[n].oc_color : NO_COLOR
#define obj_color(o, n) color = iflags.use_color ? ((o) && (o)->otyp == n && (o)->material != objects[n].oc_material && material_definitions[(o)->material].color != NO_COLOR ? material_definitions[(o)->material].color : objects[n].oc_color) : NO_COLOR
#define missile_color(l, n) color = iflags.use_color ? ((l).missile_material > MAT_NONE && (l).missile_material != objects[n].oc_material && material_definitions[(l).missile_material].color != NO_COLOR ? material_definitions[(l).missile_material].color : objects[n].oc_color) : NO_COLOR
#define artifact_color(n) color = iflags.use_color ? artilist[n].ocolor : NO_COLOR
#define mon_color(n) color = iflags.use_color ? mons[n].mcolor : NO_COLOR
#define invis_color(n) color = NO_COLOR
#define pet_color(n) color = iflags.use_color ? mons[n].mcolor : NO_COLOR
#define warn_color(n) \
    color = iflags.use_color ? def_warnsyms[n].color : NO_COLOR
#define explode_color(n) color = iflags.use_color ? explosion_type_definitions[n].color : NO_COLOR

#else /* no text color */

#define zap_color(n)
#define cmap_color(n,cmap_idx)
#define otyp_color(n)
#define obj_color(o, n)
#define mon_color(n)
#define invis_color(n)
#define pet_color(c)
#define warn_color(n)
#define explode_color(n)
#endif

#if defined(USE_TILES) && defined(MSDOS)
#define HAS_ROGUE_IBM_GRAPHICS \
    (currentgraphics == ROGUESET && SYMHANDLING(H_IBM) && !iflags.grmode)
#else
#define HAS_ROGUE_IBM_GRAPHICS \
    (currentgraphics == ROGUESET && SYMHANDLING(H_IBM))
#endif

/*ARGSUSED*/
int
mapglyph(layers, ochar, ocolor, ospecial, x, y)
struct layer_info layers;
int *ocolor, x, y;
nhsym *ochar;
unsigned long *ospecial;
{
    int signed_glyph = layers.glyph;
    register int offset, idx;
    int color = NO_COLOR;
    nhsym ch;
    unsigned special = 0;
    /* condense multiple tests in macro version down to single */
    boolean has_rogue_ibm_graphics = HAS_ROGUE_IBM_GRAPHICS;
    boolean has_rogue_color = (has_rogue_ibm_graphics
                               && symset[currentgraphics].nocolor == 0);

    int glyph = abs(signed_glyph);
    if(signed_glyph < 0)
        special |= MG_H_FLIP;

    /*
     *  Map the glyph back to a character and color.
     *
     *  Warning:  For speed, this makes an assumption on the order of
     *            offsets.  The order is set in display.h.
     */

    /* Handle no glyph */
    if (glyph == MAX_GLYPH)
        glyph = cmap_to_glyph(S_unexplored);

    if ((offset = (glyph - GLYPH_SPECIAL_EFFECT_OFF)) >= 0 && glyph < MAX_GLYPH)
    {
        /* Should never arrive here, these are UI elements */
        idx = '_';
        color = CLR_GRAY;
    }
    else if (
        (offset = (glyph - GLYPH_PLAYER_DEATH_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_DOOR_USE_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_ITEM_USE_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_SPECIAL_ATTACK_3_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_SPECIAL_ATTACK_2_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_PASSIVE_DEFENSE_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_KICK_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_SPECIAL_ATTACK_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_CAST_NODIR_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_CAST_DIR_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_FIRE_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_THROW_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_ATTACK_OFF)) >= 0
        || (offset = (glyph - GLYPH_PLAYER_OFF)) >= 0
        )
    {
        int mnum = (Upolyd || !flags.showrace ? u.umonnum : urace.monsternum);
        idx = mons[mnum >= LOW_PM ? mnum : PM_HUMAN].mlet + SYM_OFF_M;
        mon_color(mnum);
#ifdef TEXTCOLOR
        /* special case the hero for `showrace' option */
        if (iflags.use_color && x == u.ux && y == u.uy
            && flags.showrace && !Upolyd)
            color = HI_DOMESTIC;
#endif
    }
    else if ((offset = (glyph - GLYPH_ARTIFACT_MISSILE_OFF)) >= 0 
        || (offset = (glyph - GLYPH_ARTIFACT_OFF)) >= 0
        )
    { /* an artifact */
        int artidx = offset + 1;
        int objoffset = artilist[artidx].otyp;
        boolean ismissile = glyph - GLYPH_ARTIFACT_MISSILE_OFF >= 0;
        if (artilist[artidx].maskotyp != STRANGE_OBJECT)
        {
            /* We always use maskotyp for base case if there is one, since the item is specified to look like one */
            objoffset = artilist[artidx].maskotyp;
        }
        /* Select the right symbol */
        idx = objects[objoffset].oc_class + SYM_OFF_O;

        if (has_rogue_color && iflags.use_color)
        {
            switch (objects[objoffset].oc_class)
            {
            case COIN_CLASS:
                color = CLR_YELLOW;
                break;
            case FOOD_CLASS:
                color = CLR_RED;
                break;
            default:
                color = CLR_BRIGHT_BLUE;
                break;
            }
        }
        else
        {
            if (artilist[artidx].ocolor == NO_COLOR)
            {
                if (ismissile)
                {
                    missile_color(layers, artilist[artidx].otyp);
                }
                else
                {
                    struct obj* otmp = cansee(x, y) ? vobj_at(x, y) : level.locations[x][y].hero_memory_layers.memory_objchn;
                    obj_color(otmp, objoffset);
                }
            }
            else
                artifact_color(artidx);
        }
        if (objoffset != BOULDER && is_objpile(x, y))
            special |= MG_OBJPILE;
    }
    else if ((offset = (glyph - GLYPH_FEMALE_BODY_OFF)) >= 0)
    { /* a corpse */
        idx = objects[CORPSE].oc_class + SYM_OFF_O;
        if (has_rogue_color && iflags.use_color)
            color = CLR_RED;
        else
            mon_color(offset);
        special |= MG_FEMALE;
        special |= MG_CORPSE;
        if (is_objpile(x, y))
            special |= MG_OBJPILE;
    }
    else if ((offset = (glyph - GLYPH_BODY_OFF)) >= 0)
    { /* a corpse */
        idx = objects[CORPSE].oc_class + SYM_OFF_O;
        if (has_rogue_color && iflags.use_color)
            color = CLR_RED;
        else
            mon_color(offset);
        special |= MG_CORPSE;
        if (is_objpile(x, y))
            special |= MG_OBJPILE;
    }
    else if ((offset = (glyph - GLYPH_FEMALE_STATUE_OFF)) >= 0)
    { /* a statue */
        if (flags.classic_statue_symbol)
        {
            idx = ROCK_CLASS + SYM_OFF_O;
        }
        else
        {
            idx = mons[offset].mlet + SYM_OFF_M;
        }
        if (has_rogue_color)
            color = CLR_RED;
        else
            otyp_color(STATUE);
        special |= MG_FEMALE;
        special |= MG_STATUE;
        if (is_objpile(x, y))
            special |= MG_OBJPILE;
    }
    else if ((offset = (glyph - GLYPH_STATUE_OFF)) >= 0)
    { /* a statue */
        if (flags.classic_statue_symbol)
        {
            idx = ROCK_CLASS + SYM_OFF_O;
        }
        else
        {
            idx = mons[offset].mlet + SYM_OFF_M;
        }
        if (has_rogue_color)
            color = CLR_RED;
        else
            otyp_color(STATUE);
        special |= MG_STATUE;
        if (is_objpile(x,y))
            special |= MG_OBJPILE;
    }
    else if ((offset = (glyph - GLYPH_WORM_OFF)) >= 0)
    { /* worm */
        /* Should never get here -- These are for GUI drawing */
        idx = S_WORM + SYM_OFF_M;
    }
    else if ((offset = (glyph - GLYPH_WARNING_OFF)) >= 0)
    { /* warn flash */
        idx = offset + SYM_OFF_W;
        if (has_rogue_color)
            color = NO_COLOR;
        else
            warn_color(offset);
    }
    else if ((offset = (glyph - GLYPH_SWALLOW_OFF)) >= 0) 
    { /* swallow */
        /* see swallow_to_glyph() in display.c */
        idx = (S_sw_tl + (offset % MAX_SWALLOW_CHARS)) + SYM_OFF_P;
        if (has_rogue_color && iflags.use_color)
            color = NO_COLOR;
        else
            mon_color(offset / MAX_SWALLOW_CHARS);
    }
    else if ((offset = (glyph - GLYPH_ZAP_OFF)) >= 0)
    { /* zap beam */
        /* see zapdir_to_glyph() in display.c */
        idx = zap_glyph_to_cmap(glyph);

        if (has_rogue_color && iflags.use_color)
            color = NO_COLOR;
        else
            zap_color(offset / NUM_ZAP_CHARS);
    } 
    else if ((offset = (glyph - GLYPH_EXPLODE_OFF)) >= 0)
    { /* explosion */
        idx = ((offset % MAX_EXPLOSION_CHARS) + S_explode1) + SYM_OFF_P;
        explode_color(offset / MAX_EXPLOSION_CHARS);
    }
    else if ((offset = (glyph - GLYPH_BROKEN_CMAP_VARIATION_OFF)) >= 0 
            || (offset = (glyph - GLYPH_CMAP_VARIATION_OFF)) >= 0 
            || (offset = (glyph - GLYPH_BROKEN_CMAP_OFF)) >= 0
            || (offset = (glyph - GLYPH_CMAP_OFF)) >= 0
            )
    { /* cmap */
        boolean is_variation = FALSE;
        if ((glyph - GLYPH_BROKEN_CMAP_VARIATION_OFF) >= 0 || (glyph - GLYPH_CMAP_VARIATION_OFF) >= 0)
            is_variation = TRUE;
        int cmap_type_idx = 0;
        int cmap_offset = 0;
        int variation_index = 0;
        boolean has_decoration = (layers.layer_flags & LFLAGS_C_DECORATION) != 0;
        if(has_decoration && levl[x][y].decoration_typ > 0 && (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_UNDERSCORE) != 0)
            special |= MG_DECORATION;

        if (is_variation)
        {
            cmap_type_idx = max(0, offset / MAX_VARIATIONS);
            variation_index = max(0, offset - cmap_type_idx * MAX_VARIATIONS);
            cmap_offset = defsym_variations[variation_index].base_screen_symbol;
            idx = cmap_offset + SYM_OFF_P;
        }
        else
        {
            cmap_type_idx = offset / NUM_CMAP_TYPE_CHARS;
            cmap_offset = offset - cmap_type_idx * NUM_CMAP_TYPE_CHARS;
            idx = cmap_offset + SYM_OFF_P;
        }

        /* Boulder replacement */
        if (cmap_offset == S_extra_boulder)
            idx = SYM_BOULDER + SYM_OFF_X;

        if (has_rogue_color && iflags.use_color) 
        {
            if (cmap_offset >= S_vwall && cmap_offset <= S_hoportcullis)
                color = CLR_BROWN;
            else if (cmap_offset >= S_arrow_trap && cmap_offset <= S_polymorph_trap)
                color = CLR_MAGENTA;
            else if (cmap_offset == S_corr || cmap_offset == S_litcorr)
                color = CLR_GRAY;
            else if (cmap_offset >= S_room && cmap_offset <= S_water
                     && cmap_offset != S_darkroom && cmap_offset != S_darkgrass && cmap_offset != S_darkground
                )
                color = CLR_GREEN;
            else
                color = NO_COLOR;
        } 
        else if (!iflags.use_color && cmap_offset == S_lava
                   && (showsyms[idx] == showsyms[S_pool + SYM_OFF_P]
                       || showsyms[idx] == showsyms[S_water + SYM_OFF_P]))
        {
            /* try to provide a visible difference between water and lava
               if they use the same symbol and color is disabled */
            special |= MG_BW_LAVA;
        } 
        else
        {
            /* Darken by changing the symbol */
            boolean symbol_darkened_by_change = FALSE;
            boolean dark_exchangeable = (cmap_offset == S_room
                || cmap_offset == S_grass || cmap_offset == S_ground || cmap_offset == S_litcorr);
            int dark_counterpart = (cmap_offset == S_litcorr ? S_corr 
                : cmap_offset == S_room ? S_darkroom 
                : cmap_offset == S_ground ? S_darkground
                : cmap_offset == S_grass ? S_darkgrass : S_unexplored);

            if (dark_exchangeable &&
                ((cmap_offset == S_litcorr && !flags.lit_corridor) || !cansee(x, y) || (layers.layer_flags & LFLAGS_SHOWING_MEMORY)))
            {
                if ((cmap_offset == S_room || cmap_offset == S_grass || cmap_offset == S_ground) && !flags.dark_room)
                {
                    /* Nothing, keep as normal */
                }
                else
                {
                    is_variation = FALSE; /* use base cmap's dark counterpart always */
                    cmap_offset = dark_counterpart;
                    idx = cmap_offset + SYM_OFF_P;
                    symbol_darkened_by_change = TRUE;
                }
            }

            if (iflags.use_color)
            {
                if (!symbol_darkened_by_change && dark_exchangeable && !is_variation
                    && showsyms[idx] != showsyms[dark_counterpart + SYM_OFF_P])
                {
                    /* if showsyms are different, the colors will be as per dark_counterpart for the base cmap (variations can be different colors), otherwise if the same symbols they will use their normal different colors */
                    cmap_color(dark_counterpart, flags.classic_colors ? 0 : cmap_type_idx);
                }
                else
                {
                    if (is_variation && !flags.classic_colors)
                        cmap_variation_color(variation_index, cmap_type_idx);
                    else
                        cmap_color(cmap_offset, flags.classic_colors ? 0 : cmap_type_idx);
                }
            }
        }

        if (iflags.use_color && flags.show_decorations)
        {
            if (has_decoration && levl[x][y].decoration_typ > 0)
            {
                if (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LOOTABLE)
                {
                    if (levl[x][y].decoration_flags & (DECORATION_FLAGS_ITEM_IN_HOLDER | DECORATION_FLAGS_ITEM2_IN_HOLDER | DECORATION_FLAGS_ITEM3_IN_HOLDER))
                    {
                        if (decoration_type_definitions[levl[x][y].decoration_typ].color_filled != NO_COLOR)
                            color = decoration_type_definitions[levl[x][y].decoration_typ].color_filled;

                        if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_UNLIT_FILLED_COLOR) != 0)
                            color = decoration_lit_colors[color].color_unlit;
                        else if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIT_FILLED_COLOR) != 0)
                            color = decoration_lit_colors[color].color_lit;
                        else if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_WOODY_FILLED_COLOR) != 0)
                            color = decoration_lit_colors[color].color_woody;
                        else if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIGHTABLE) != 0)
                        {
                            if (levl[x][y].lamplit)
                                color = decoration_lit_colors[color].color_lit;
                            else
                                color = decoration_lit_colors[color].color_unlit;
                        }
                    }
                    else
                    {
                        if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_UNLIT_COLOR) != 0)
                            color = decoration_lit_colors[color].color_unlit;
                        else if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIT_COLOR) != 0)
                            color = decoration_lit_colors[color].color_lit;
                        else if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_WOODY_COLOR) != 0)
                            color = decoration_lit_colors[color].color_woody;
                        else if (decoration_type_definitions[levl[x][y].decoration_typ].color != NO_COLOR)
                            color = decoration_type_definitions[levl[x][y].decoration_typ].color;
                    }
                }
                else
                {
                    if (decoration_type_definitions[levl[x][y].decoration_typ].color != NO_COLOR)
                        color = decoration_type_definitions[levl[x][y].decoration_typ].color;

                    if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_UNLIT_COLOR) != 0)
                        color = decoration_lit_colors[color].color_unlit;
                    else if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIT_COLOR) != 0)
                        color = decoration_lit_colors[color].color_lit;
                    else if ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIGHTABLE) != 0)
                    {
                        if (levl[x][y].lamplit)
                            color = decoration_lit_colors[color].color_lit;
                        else
                            color = decoration_lit_colors[color].color_unlit;
                    }
                }
            }
        }
    } 
    else if ((offset = (glyph - GLYPH_OBJ_MISSILE_OFF)) >= 0 
               || (offset = (glyph - GLYPH_OBJ_OFF)) >= 0
               )
    { /* object */
        if (glyph - GLYPH_OBJ_MISSILE_OFF >= 0)
            offset = offset / NUM_MISSILE_DIRS;

        idx = objects[offset].oc_class + SYM_OFF_O;
        boolean ismissile = glyph - GLYPH_OBJ_MISSILE_OFF >= 0;

        if (offset == BOULDER)
            idx = SYM_BOULDER + SYM_OFF_X;
        if (has_rogue_color && iflags.use_color) 
        {
            switch (objects[offset].oc_class)
            {
            case COIN_CLASS:
                color = CLR_YELLOW;
                break;
            case FOOD_CLASS:
                color = CLR_RED;
                break;
            default:
                color = CLR_BRIGHT_BLUE;
                break;
            }
        } 
        else
        {
            if (offset == BOULDER)
                cmap_color(S_extra_boulder, flags.classic_colors ? 0 : get_current_cmap_type_index());
            else if (ismissile)
                missile_color(layers, offset);
            else
            {
                struct obj* otmp = cansee(x, y) ? vobj_at(x, y) : level.locations[x][y].hero_memory_layers.memory_objchn;
                obj_color(otmp, offset);
            }
        }
        if (offset != BOULDER && is_objpile(x,y))
            special |= MG_OBJPILE;

    }
    else if ((offset = (glyph - GLYPH_INVIS_OFF)) >= 0)
    { /* invisible */
        idx = SYM_INVISIBLE + SYM_OFF_X;
        if (has_rogue_color)
            color = NO_COLOR; /* no need to check iflags.use_color */
        else
            invis_color(offset);
        special |= MG_INVIS;
    }
    else if (
    (offset = (glyph - GLYPH_FEMALE_DEATH_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_DOOR_USE_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_ITEM_USE_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_SPECIAL_ATTACK_3_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_SPECIAL_ATTACK_2_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_PASSIVE_DEFENSE_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_KICK_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_SPECIAL_ATTACK_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_CAST_NODIR_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_CAST_DIR_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_FIRE_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_THROW_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_ATTACK_OFF)) >= 0
    || (offset = (glyph - GLYPH_FEMALE_MON_OFF)) >= 0
    )
    { /* a female monster */
        special |= MG_FEMALE;
        goto normal_monster_here;
    }
    else if (
    (offset = (glyph - GLYPH_DEATH_OFF)) >= 0
    || (offset = (glyph - GLYPH_DOOR_USE_OFF)) >= 0
    || (offset = (glyph - GLYPH_ITEM_USE_OFF)) >= 0
    || (offset = (glyph - GLYPH_SPECIAL_ATTACK_3_OFF)) >= 0
    || (offset = (glyph - GLYPH_SPECIAL_ATTACK_2_OFF)) >= 0
    || (offset = (glyph - GLYPH_PASSIVE_DEFENSE_OFF)) >= 0
    || (offset = (glyph - GLYPH_KICK_OFF)) >= 0
    || (offset = (glyph - GLYPH_SPECIAL_ATTACK_OFF)) >= 0
    || (offset = (glyph - GLYPH_CAST_NODIR_OFF)) >= 0
    || (offset = (glyph - GLYPH_CAST_DIR_OFF)) >= 0
    || (offset = (glyph - GLYPH_FIRE_OFF)) >= 0
    || (offset = (glyph - GLYPH_THROW_OFF)) >= 0
    || (offset = (glyph - GLYPH_ATTACK_OFF)) >= 0
    )
    { /* mon action */
        goto normal_monster_here;
    }
    else 
    { /* a monster */
        offset = (glyph - GLYPH_MON_OFF);
normal_monster_here:
        {
            boolean ispet = (layers.monster_flags & LMFLAGS_PET) != 0;
            boolean ispeaceful = (layers.monster_flags & LMFLAGS_PEACEFUL) != 0;
            boolean isdetected = ((layers.monster_flags & LMFLAGS_DETECTED) != 0 || (def_monsyms[(int)mons[offset].mlet].sym == ' ' && (layers.layer_flags & LFLAGS_SHOWING_DETECTION) != 0));
            boolean isridden = (layers.monster_flags & LMFLAGS_RIDDEN) != 0;
            boolean issaddled = (layers.status_bits & (1UL << STATUS_MARK_SADDLED)) != 0;

            /* set special flags */
            if(ispet)
                special |= MG_PET;
            if (ispeaceful)
                special |= MG_PEACEFUL;
            if (isdetected)
                special |= MG_DETECT;
            if (isridden)
                special |= MG_RIDDEN;
            if (issaddled)
                special |= MG_SADDLED;

            /* set symbol */
            idx = mons[offset].mlet + SYM_OFF_M;
        
            /* set color */
            if (has_rogue_color && iflags.use_color)
            {
                if (x == u.ux && y == u.uy)
                    /* actually player should be yellow-on-gray if in corridor */
                    color = CLR_YELLOW;
                else
                    color = NO_COLOR;
            }
            else 
            {
                if(ispet)
                    pet_color(offset);
                else
                    mon_color(offset);
    #ifdef TEXTCOLOR
                /* special case the hero for `showrace' option */
                if (iflags.use_color && x == u.ux && y == u.uy
                    && flags.showrace && !Upolyd)
                    color = HI_DOMESTIC;
    #endif
            }
        }
    }

    ch = showsyms[idx];
#ifdef TEXTCOLOR
    /* Turn off color if no color defined, or rogue level w/o PC graphics. */
    if (!has_color(color) || (Is_really_rogue_level(&u.uz) && !has_rogue_color))
        color = NO_COLOR;
#endif

//    if (ch == ' ' && color != NO_COLOR)  /* Does not look very good */
//        special |= MG_STONE_INVERSE;

    *ochar = ch;
    *ospecial = special;
#ifdef TEXTCOLOR
    *ocolor = color;
#else
    nhUse(ocolor);
#endif
    return idx;
}

char *
encglyph(glyph)
int glyph;
{
    static char encbuf[20]; /* 10+1 would suffice */

    Sprintf(encbuf, "\\G%04X%04X", context.rndencode, abs(glyph));
    return encbuf;
}

char *
decode_mixed(buf, str)
char *buf;
const char *str;
{
    static const char hex[] = "00112233445566778899aAbBcCdDeEfF";
    char *put = buf;

    if (!str)
        return strcpy(buf, "");

    while (*str) {
        if (*str == '\\') {
            int rndchk, dcount, so, gv, oc = 0;
            nhsym ch = 0;
            unsigned long os = 0;
            const char *dp, *save_str;

            save_str = str++;
            switch (*str) {
            case 'G': /* glyph value \GXXXXNNNN*/
                rndchk = dcount = 0;
                for (++str; *str && ++dcount <= 4; ++str)
                    if ((dp = index(hex, *str)) != 0)
                        rndchk = (rndchk * 16) + ((int) (dp - hex) / 2);
                    else
                        break;
                if (rndchk == context.rndencode) {
                    gv = dcount = 0;
                    for (; *str && ++dcount <= 4; ++str)
                        if ((dp = index(hex, *str)) != 0)
                            gv = (gv * 16) + ((int) (dp - hex) / 2);
                        else
                            break;

                    struct layer_info layers = nul_layerinfo;
                    layers.glyph = gv;

                    so = mapglyph(layers, &ch, &oc, &os, 0, 0);
                    *put++ = (char)showsyms[so];
                    /* 'str' is ready for the next loop iteration and '*str'
                       should not be copied at the end of this iteration */
                    continue;
                } else {
                    /* possible forgery - leave it the way it is */
                    str = save_str;
                }
                break;
#if 0
            case 'S': /* symbol offset */
                so = rndchk = dcount = 0;
                for (++str; *str && ++dcount <= 4; ++str)
                    if ((dp = index(hex, *str)) != 0)
                        rndchk = (rndchk * 16) + ((int) (dp - hex) / 2);
                    else
                        break;
                if (rndchk == context.rndencode) {
                    dcount = 0;
                    for (; *str && ++dcount <= 2; ++str)
                        if ((dp = index(hex, *str)) != 0)
                            so = (so * 16) + ((int) (dp - hex) / 2);
                        else
                            break;
                }
                *put++ = showsyms[so];
                break;
#endif
            case '\\':
                break;
            }
        }
        *put++ = *str++;
    }
    *put = '\0';
    return buf;
}

/*
 * This differs from putstr() because the str parameter can
 * contain a sequence of characters representing:
 *        \GXXXXNNNN    a glyph value, encoded by encglyph().
 *
 * For window ports that haven't yet written their own
 * XXX_putmixed() routine, this general one can be used.
 * It replaces the encoded glyph sequence with a single
 * showsyms[] char, then just passes that string onto
 * putstr().
 */

void
genl_putmixed_ex(window,  str, attr, color, app)
winid window;
int attr, app, color;
const char *str;
{
    char buf[BUFSZ];

    /* now send it to the normal putstr */
    putstr_ex(window, decode_mixed(buf, str), attr, color, app);
}

int
zap_glyph_to_cmap(glyph)
int glyph;
{
    if (!glyph_is_zap(glyph))
        return 0;

    int idx = 0;
    int offset = glyph - GLYPH_ZAP_OFF;
    int zapnum = offset / NUM_ZAP_CHARS;
    int zidx = (offset % NUM_ZAP_CHARS);
    if (zapnum == ZAP_SPECIAL_DIGGING || zapnum == ZAP_SPECIAL_EVAPORATION)
        idx = S_digbeam + SYM_OFF_P;
    else if (zapnum == ZAP_SPECIAL_FLASHED_LIGHT)
        idx = S_flashbeam + SYM_OFF_P;
    else if (zidx < 2 * NUM_ASCII_ZAP_CHARS)
        idx = (S_vbeam + (zidx % NUM_ASCII_ZAP_CHARS)) + SYM_OFF_P;
    else if (zidx < 3 * NUM_ASCII_ZAP_CHARS)
        idx = ((zidx % 2) == 0 ? S_rslant : S_lslant) + SYM_OFF_P;
    else  if (zidx < 4 * NUM_ASCII_ZAP_CHARS)
        idx = ((zidx % 2) == 1 ? S_rslant : S_lslant) + SYM_OFF_P;
    else if (zidx < 6 * NUM_ASCII_ZAP_CHARS)
        idx = (S_vbeam + (zidx % NUM_ASCII_ZAP_CHARS)) + SYM_OFF_P;
    else
        idx = S_unexplored; /* Illegal */

    return idx;
}


/*mapglyph.c*/
