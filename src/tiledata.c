/* GnollHack 4.0 tiledata.c */
/* Copyright (c) Janne Gustafsson, 2020.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "artifact.h"
#include "artilist.h"

#ifdef USE_TILES
short glyph2tile[MAX_GLYPH] = { 0 }; /* moved here from tile.c */
uchar glyphtileflags[MAX_GLYPH] = { 0 }; /* specifies how to use the tile and operations applied to the tile before use */
short tile2replacement[MAX_GLYPH] = { 0 }; /* maximum of one tile per glyph */
short tile2animation[MAX_GLYPH] = { 0 }; /* maximum of one tile per glyph */
short tile2enlargement[MAX_GLYPH] = { 0 }; /* maximum of one tile per glyph */
#endif

NEARDATA struct tileset_definition default_tileset_definition =
{
    2,
    {TRUE, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, 1, 2,
    2,
    2,
    5,
    {"dungeon-normal", "gnomish-mines", "gehennom", "dungeon-undead", "sokoban", (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0},
    {0, 1, 2, 0, 4, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    1, 1, 3
};

NEARDATA struct ui_component_definition ui_tile_component_array[MAX_UI_TILES] = {
    {"death",                   NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"hit",                     NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"extra-hit-tile-1",        NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"extra-hit-tile-2",        NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"extra-hit-tile-3",        NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"extra-hit-tile-4",        NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"extra-hit-tile-5",        NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"extra-hit-tile-6",        NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"extra-hit-tile-7",        NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"general-ui",              NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 3, 16, 16, {"checkbox-unchecked", "checkbox-checked", "checkbox-count", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"status",                  NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, MAX_STATUS_MARKS, 16, 16, {"petmark", "peacemark", "detectmark", "pilemark",  "hungry", "weak", "faint", "burdened",  "stressed", "strained", "overtaxed", "overloaded",  "two-weapon", "skill", "saddled", "low-hp",  "critical-hp", "spec-used", "", "",  "", "", "", ""} },
    {"conditions",              NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, min(24, BL_MASK_BITS), 16, 16, {"", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"main_tile_mark",          NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"main-window-borders",     NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"message-window-borders",  NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"status-window-borders",   NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"map-window-borders",      NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"menu-window-borders",     NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"text-window-borders",     NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT, 6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
};


boolean
has_generic_player_action_tile(action)
enum action_tile_types action;
{
    return TRUE;
}


void
init_tiledata()
{
    /* fill out condition names*/
    for (int i = 0; i < BL_MASK_BITS; i++)
    {
        unsigned long bit = 1UL << i;
        const char* cond_name = get_condition_name(bit);
        ui_tile_component_array[CONDITION_MARKS].component_names[i] = cond_name;
    }

}

int
process_tiledata(tsd, process_style, save_file_name, tilemaparray, tilemapflags) /* Save tile data / read tile data / count tiles */
struct tileset_definition* tsd;
int process_style;  /* 0 = save data to file, 1 = read data from file, 2 = count tiles */
const char* save_file_name;
short* tilemaparray;
uchar* tilemapflags;
{
    const char* fq_save = save_file_name;
    const char* tile_section_name;
    const char* set_name;
    int fd;
    short tile_count = 0;
    char buf[BUFSIZ];
    strcpy(buf, "");
    int glyph_offset = 0;

    if (process_style == 0)
    {
        (void)remove(fq_save);

#ifdef MAC
        fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
        fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
    }

    /* Monster tiles */
    tile_section_name = "monsters";
    
    /* spsets must be in same order as enum action_tile_types */
    const char* monster_set_name_array[MAX_ACTION_TILES + 2] = { "normal", "attack", "throw", "fire", "cast", "special-attack", "kick", "passive-defense", "defend", "receive-damage", "item-use", "door-use", "death", "statue", "body" };

    for (int gender = 0; gender <= 1; gender++)
    {
        if (gender == 1)
        {
            if (tsd->female_tile_style == 0)
                continue;
        }
        const char* gender_name = (gender == 0 ? "base" : "female");

        for (int spset = 0; spset < MAX_ACTION_TILES + 2; spset++)
        {
            if (spset > ACTION_TILE_NO_ACTION && spset < MAX_ACTION_TILES)
            {
                if (!tsd->action_tile_style[spset])
                    continue;
            }

            if (spset == MAX_ACTION_TILES && !tsd->statue_tile_style)
                continue;
            if (spset == MAX_ACTION_TILES + 1 && !tsd->corpse_tile_style)
                continue;

            set_name = monster_set_name_array[spset];
            for (int i = LOW_PM; i < NUM_MONSTERS; i++)
            {
                uchar fullsizedflag = 0;
                if (gender == 1)
                {
                    if (tsd->female_tile_style == 2 && !(mons[i].mflags5 & M5_FEMALE_TILE))
                        continue;
                }

                if (spset == MAX_ACTION_TILES + 1)
                {
                    if (tsd->corpse_tile_style == 2 && !(mons[i].mflags5 & M5_CORPSE_TILE))
                        continue;

                    if (objects[CORPSE].oc_flags4 & O4_FULL_SIZED_BITMAP)
                        fullsizedflag = GLYPH_TILE_FLAG_FULL_SIZED_ITEM;
                    else
                        fullsizedflag = 0;
                }

                if (spset > ACTION_TILE_NO_ACTION && spset < MAX_ACTION_TILES)
                {
                    unsigned long m5_action_flag = M5_ATTACK_TILE << (spset - 1);
                    if (tsd->action_tile_style[spset] == 2 && !(mons[i].mflags5 & m5_action_flag))
                        continue;
                }

                if (spset == MAX_ACTION_TILES)
                {
                    if (objects[STATUE].oc_flags4 & O4_FULL_SIZED_BITMAP)
                        fullsizedflag = GLYPH_TILE_FLAG_FULL_SIZED_ITEM;
                    else
                        fullsizedflag = 0;
                }


                short enlargement = 0;
                if (gender == 0)
                {
                    if (spset < MAX_ACTION_TILES)
                        enlargement = mons[i].enlargement.actions[spset];
                    else if (spset == MAX_ACTION_TILES)
                        enlargement = mons[i].enlargement.statue;
                    else if (spset == MAX_ACTION_TILES + 1)
                        enlargement = mons[i].enlargement.corpse;
                }
                else
                {
                    if (spset < MAX_ACTION_TILES)
                        enlargement = mons[i].female_enlargement.actions[spset];
                    else if (spset == MAX_ACTION_TILES)
                        enlargement = mons[i].female_enlargement.statue;
                    else if (spset == MAX_ACTION_TILES + 1)
                        enlargement = mons[i].female_enlargement.corpse;
                }

                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,%s,%s", tile_section_name, gender_name, set_name, mons[i].mname);
                    if(enlargement > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[enlargement].width_in_tiles, enlargements[enlargement].height_in_tiles, enlargements[enlargement].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    int glyph_offset = (spset < MAX_ACTION_TILES) ? get_monster_action_glyph_offset(spset, 0) : spset == MAX_ACTION_TILES ? GLYPH_STATUE_OFF : GLYPH_BODY_OFF; // mon_glyph_offset_array[spset];
                    int female_glyph_offset = (spset < MAX_ACTION_TILES) ? get_monster_action_glyph_offset(spset, 1) : spset == MAX_ACTION_TILES ? GLYPH_FEMALE_STATUE_OFF : GLYPH_FEMALE_BODY_OFF;  //female_mon_glyph_offset_array[spset];

                    if (gender == 0)
                    {
                        tilemaparray[i + glyph_offset] = tile_count;
                        if(spset == MAX_ACTION_TILES || spset == MAX_ACTION_TILES + 1)
                            tilemapflags[i + glyph_offset] |= fullsizedflag;

                        if (spset == ACTION_TILE_NO_ACTION)
                        {
                            for (enum action_tile_types action = ACTION_TILE_ATTACK; action < MAX_ACTION_TILES; action++)
                            {
                                if (tsd->action_tile_style[action] != 1)
                                    tilemaparray[i + get_monster_action_glyph_offset(action, 0)] = tile_count;
                            }
                        }
                        else if (spset == ACTION_TILE_ATTACK)
                        {
                            /* Write these again if we get here */
                            enum action_tile_types action_array[6] = { ACTION_TILE_THROW, ACTION_TILE_FIRE, ACTION_TILE_CAST, ACTION_TILE_SPECIAL_ATTACK, ACTION_TILE_KICK, ACTION_TILE_ITEM_USE };
                            for (int idx = 0; idx < 6; idx++)
                            {
                                enum action_tile_types action = action_array[idx];
                                if (tsd->action_tile_style[action] != 1)
                                    tilemaparray[i + get_monster_action_glyph_offset(action, 0)] = tile_count;
                            }
                        }
                        else if (spset == ACTION_TILE_DOOR_USE)
                        {
                            enum action_tile_types action_array[3] = { ACTION_TILE_THROW, ACTION_TILE_CAST, ACTION_TILE_ITEM_USE };
                            for (int idx = 0; idx < 3; idx++)
                            {
                                enum action_tile_types action = action_array[idx];
                                if (tsd->action_tile_style[action] != 1)
                                    tilemaparray[i + get_monster_action_glyph_offset(action, 0)] = tile_count;
                            }
                        }
                    }

                    /* write female versions twice just in case, first as base monster, and then override as female, if we get here */
                    tilemaparray[i + female_glyph_offset] = tile_count;
                    if (spset == MAX_ACTION_TILES || spset == MAX_ACTION_TILES + 1)
                        tilemapflags[i + female_glyph_offset] |= fullsizedflag;

                    if (spset == ACTION_TILE_NO_ACTION)
                    {
                        for (enum action_tile_types action = ACTION_TILE_ATTACK; action < MAX_ACTION_TILES; action++)
                        {
                            if (tsd->action_tile_style[action] != 1)
                                tilemaparray[i + get_monster_action_glyph_offset(action, 1)] = tile_count;
                        }
                    }
                    else if (spset == ACTION_TILE_ATTACK)
                    {
                        /* Write these again if we get here */
                        enum action_tile_types action_array[6] = { ACTION_TILE_THROW, ACTION_TILE_FIRE, ACTION_TILE_CAST, ACTION_TILE_SPECIAL_ATTACK, ACTION_TILE_KICK, ACTION_TILE_ITEM_USE };
                        for (int idx = 0; idx < 6; idx++)
                        {
                            enum action_tile_types action = action_array[idx];
                            if (tsd->action_tile_style[action] != 1)
                                tilemaparray[i + get_monster_action_glyph_offset(action, 1)] = tile_count;
                        }
                    }
                    else if (spset == ACTION_TILE_DOOR_USE)
                    {
                        enum action_tile_types action_array[3] = { ACTION_TILE_THROW, ACTION_TILE_CAST, ACTION_TILE_ITEM_USE };
                        for (int idx = 0; idx < 3; idx++)
                        {
                            enum action_tile_types action = action_array[idx];
                            if (tsd->action_tile_style[action] != 1)
                                tilemaparray[i + get_monster_action_glyph_offset(action, 1)] = tile_count;
                        }
                    }
                }
                tile_count++;
            }
        }
    }

    const char* missile_direction_name_array[NUM_MISSILE_DIRS] = {
            "top-left", "top-center", "top-right",
            "middle-left",  "middle-right",
            "bottom-left", "bottom-center", "bottom-right" };

    /* Object tiles */
    tile_section_name = "objects";
    for (int j = 0; j <= 1; j++)
    {
        /* j == 0 -> Normal objs */
        /* j == 1 -> Missile objs */

        if (j == 1 && !tsd->missile_tile_style)
            continue;

        boolean first_scroll_found = FALSE;
        int nameless_idx = 0;
        set_name = (j == 0 ? "normal" : "missile");
        glyph_offset =
            (j == 0 ? GLYPH_OBJ_OFF :
                GLYPH_OBJ_MISSILE_OFF);

        for (int i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
        {
            int missile_tile_num = (objects[i].oc_flags4 & O4_SINGLE_MISSILE_TILE) ? 1 : NUM_MISSILE_DIRS;

            if (j == 1)
            {
                if (tsd->missile_tile_style == 2 && !(objects[i].oc_flags4 & O4_MISSILE_TILE))
                    continue;
            }


            const char* oclass_name = def_oc_syms[objects[i].oc_class].name;
            /* Jump over gems that all look the same, just take pieces of glass and luckstone */
            if (objects[i].oc_class == GEM_CLASS)
            {
                if (i <= LAST_GEM || (i > LUCKSTONE && i <= FLINT))
                    continue;
            }
            else if (objects[i].oc_class == SCROLL_CLASS)
            {
                /* Special case for scrolls - they all look the same, except mail, so only two special cases */
                if (first_scroll_found)
                    continue;

                first_scroll_found = TRUE;
                uchar fullsizedflag = !!(objects[i].oc_flags4 & O4_FULL_SIZED_BITMAP);

                /* First, generic scroll tile */
                if (process_style == 0)
                {
                    if (j == 1)
                    {
                        for (int n = 0; n < missile_tile_num; n++)
                        {
                            Sprintf(buf, "%s,%s,%s,%s,%s,%s,1,1,0\n", tile_section_name, set_name, oclass_name, 
                                "generic", "scroll", 
                                missile_tile_num == 1 ? "generic" : missile_direction_name_array[n]);
                            (void)write(fd, buf, strlen(buf));
                            tile_count++;
                        }
                    }
                    else
                    {
                        Sprintf(buf, "%s,%s,%s,%s,%s", tile_section_name, set_name, oclass_name, "generic", "scroll");

                        if (objects[i].oc_flags4 & O4_FULL_SIZED_BITMAP)
                            Sprintf(eos(buf), ",1");
                        else
                            Sprintf(eos(buf), ",0");

                        if (j == 0 && obj_descr[i].enlargement > 0)
                            Sprintf(eos(buf), ",%d,%d,%d", enlargements[obj_descr[i].enlargement].width_in_tiles, enlargements[obj_descr[i].enlargement].height_in_tiles, enlargements[obj_descr[i].enlargement].main_tile_x_coordinate);
                        else
                            Sprintf(eos(buf), ",1,1,0");
                        Sprintf(eos(buf), "\n");
                        (void)write(fd, buf, strlen(buf));
                        tile_count++;
                    }
                }
                else if (process_style == 1)
                {
                    if (j == 1)
                    {
                        for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                        {
                            /* Found scroll */
                            tilemaparray[i * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
                            tilemapflags[i * NUM_MISSILE_DIRS + n + glyph_offset] &= ~GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;

                            /* Add the tile to all scrolls */
                            for (int m = STRANGE_OBJECT; m < NUM_OBJECTS; m++)
                            {
                                if (objects[m].oc_class == SCROLL_CLASS)
                                {
                                    tilemaparray[m * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
                                    tilemapflags[m * NUM_MISSILE_DIRS + n + glyph_offset] &= ~GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;
                                }
                            }
                            if(missile_tile_num != 1)
                                tile_count++;
                        }
                        if (missile_tile_num == 1)
                            tile_count++;
                    }
                    else
                    {
                        tilemaparray[i + glyph_offset] = tile_count;
                        tilemapflags[i + glyph_offset] |= fullsizedflag;

                        /* Add the tile to all scrolls */
                        for (int m = STRANGE_OBJECT; m < NUM_OBJECTS; m++)
                        {
                            if (objects[m].oc_class == SCROLL_CLASS)
                            {
                                /* Other scroll's main tile */
                                tilemaparray[m + glyph_offset] = tile_count;
                                tilemapflags[m + glyph_offset] |= fullsizedflag;

                                /* Add others if they do not have their own */
                                if (j == 0)
                                {
                                    if (tsd->missile_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_MISSILE_OFF;
                                        for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                                        {
                                            tilemaparray[m * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                                            tilemapflags[m * NUM_MISSILE_DIRS + n + glyph_offset4] |= GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;
                                        }
                                    }
                                }
                            }
                        }

                        tile_count++;
                    }
                }
                else
                {
                    if (j == 1)
                        tile_count += missile_tile_num;
                    else
                        tile_count++;
                }

                /* Second, scroll of mail */
                if (process_style == 0)
                {
                    if (j == 1)
                    {
                        for (int n = 0; n < missile_tile_num; n++)
                        {
                            Sprintf(buf, "%s,%s,%s,%s,%s,%s,1,1,0\n", tile_section_name, set_name, oclass_name, 
                                "mail", "envelope", 
                                missile_tile_num == 1 ? "generic" : missile_direction_name_array[n]);
                            (void)write(fd, buf, strlen(buf));
                            if(missile_tile_num != 1)
                                tile_count++;
                        }
                        if(missile_tile_num == 1)
                            tile_count++;
                    }
                    else
                    {
                        Sprintf(buf, "%s,%s,%s,%s,%s,0,1,1,0\n", tile_section_name, set_name, oclass_name, "mail", "envelope");
                        (void)write(fd, buf, strlen(buf));
                        tile_count++;
                    }
                }
                else if (process_style == 1)
                {
                    /* Add the tile the scroll "mail" */
                    if (j == 1)
                    {
                        for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                        {
#ifdef MAIL
                            tilemaparray[SCR_MAIL * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
                            tilemapflags[SCR_MAIL * NUM_MISSILE_DIRS + n + glyph_offset] &= ~GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;
#endif
                            if(missile_tile_num != 1)
                                tile_count++;
                        }
                        if (missile_tile_num == 1)
                            tile_count++;
                    }
                    else
                    {
#ifdef MAIL
                        fullsizedflag = !!(objects[SCR_MAIL].oc_flags4 & O4_FULL_SIZED_BITMAP);

                        /* Main tile */
                        tilemaparray[SCR_MAIL + glyph_offset] = tile_count;
                        tilemapflags[SCR_MAIL + glyph_offset] |= fullsizedflag;

                        /* Add to others, if they have not tiles of their own */
                        if (j == 0)
                        {
                            if (tsd->missile_tile_style != 1)
                            {
                                int glyph_offset4 = GLYPH_OBJ_MISSILE_OFF;
                                for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                                {
                                    tilemaparray[SCR_MAIL * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                                    tilemapflags[SCR_MAIL * NUM_MISSILE_DIRS + n + glyph_offset4] |= GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;
                                }
                            }
                        }
#endif
                        tile_count++;
                    }
                }
                else
                {
                    if (j == 1)
                        tile_count += missile_tile_num;
                    else
                        tile_count++;
                }

                /* Move to next item after having found the first scroll */
                continue;
            }

            /* Normal item case starts here */
            boolean nameless = !OBJ_NAME(objects[i]);
            boolean no_description = !obj_descr[objects[i].oc_name_idx].oc_descr;
            char nameless_name[BUFSZ];
            Sprintf(nameless_name, "nameless-%d", nameless_idx);
            if (!OBJ_NAME(objects[i]))
            {
                nameless = TRUE;
                nameless_idx++;
            }

            /* Full-sized item */
            uchar fullsizedflag = (objects[i].oc_flags4 & O4_FULL_SIZED_BITMAP) ? GLYPH_TILE_FLAG_FULL_SIZED_ITEM : 0;

            if (process_style == 0)
            {
                if (j == 1)
                {
                    for (int n = 0; n < missile_tile_num; n++)
                    {
                        Sprintf(buf, "%s,%s,%s,%s,%s,%s,1,1,0\n", tile_section_name, set_name, oclass_name,
                            nameless ? nameless_name : OBJ_NAME(objects[i]),
                            no_description ? "no description" : obj_descr[objects[i].oc_name_idx].oc_descr,
                            missile_tile_num == 1 ? "generic" : missile_direction_name_array[n]);
                        (void)write(fd, buf, strlen(buf));
                        tile_count++;
                    }
                }
                else
                {
                    Sprintf(buf, "%s,%s,%s,%s,%s", tile_section_name, set_name, oclass_name,
                        nameless ? nameless_name : OBJ_NAME(objects[i]),
                        no_description ? "no description" : obj_descr[objects[i].oc_name_idx].oc_descr
                    );

                    if (objects[i].oc_flags4 & O4_FULL_SIZED_BITMAP)
                        Sprintf(eos(buf), ",1");
                    else
                        Sprintf(eos(buf), ",0");

                    if (j == 0 && obj_descr[i].enlargement > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[obj_descr[i].enlargement].width_in_tiles, enlargements[obj_descr[i].enlargement].height_in_tiles, enlargements[obj_descr[i].enlargement].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                    tile_count++;
                }
            }
            else if (process_style == 1)
            {
                if (j == 1)
                {
                    for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                    {
                        /* Write to the tile to the main glyph */
                        tilemaparray[i * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
                        tilemapflags[i * NUM_MISSILE_DIRS + n + glyph_offset] &= ~GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;

                        /* If this is a piece of glass or luckstone, add the tile to all other gems with the same color; others have been skipped */
                        if (objects[i].oc_class == GEM_CLASS && (i > LAST_GEM && i <= LUCKSTONE))
                        {
                            for (int m = STRANGE_OBJECT; m < NUM_OBJECTS; m++)
                            {
                                if (objects[m].oc_class == GEM_CLASS
                                    && (m <= LAST_GEM || (m > LUCKSTONE && m <= FLINT))
                                    && objects[m].oc_color == objects[i].oc_color
                                    )
                                {
                                    /* Write to the tile to the main glyph */
                                    tilemaparray[m * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
                                    tilemapflags[m * NUM_MISSILE_DIRS + n + glyph_offset] &= ~GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;
                                }
                            }
                        }
                        if(missile_tile_num != 1)
                            tile_count++;
                    }
                    if (missile_tile_num == 1)
                        tile_count++;
                }
                else
                {
                    /* Write to the tile to the main glyph */
                    tilemaparray[i + glyph_offset] = tile_count;
                    tilemapflags[i + glyph_offset] |= fullsizedflag;

                    /* Write to the tile to the inventory, lit, and inventory lit glyphs if they do not have their own */
                    if (j == 0)
                    {
                        if (tsd->missile_tile_style != 1)
                        {
                            int glyph_offset4 = GLYPH_OBJ_MISSILE_OFF;
                            for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                            {
                                tilemaparray[i * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                                tilemapflags[i * NUM_MISSILE_DIRS + n + glyph_offset4] |= GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;
                            }
                        }
                    }

                    /* If this is a piece of glass or luckstone, add the tile to all other gems with the same color; others have been skipped */
                    if (objects[i].oc_class == GEM_CLASS && (i > LAST_GEM && i <= LUCKSTONE))
                    {
                        for (int m = STRANGE_OBJECT; m < NUM_OBJECTS; m++)
                        {
                            if (objects[m].oc_class == GEM_CLASS
                                && (m <= LAST_GEM || (m > LUCKSTONE && m <= FLINT))
                                && objects[m].oc_color == objects[i].oc_color
                                )
                            {
                                /* Write to the tile to the main glyph */
                                tilemaparray[m + glyph_offset] = tile_count;
                                tilemapflags[m + glyph_offset] |= fullsizedflag;

                                /* Write to the tile to the inventory, lit, and inventory lit glyphs if they do not have their own */
                                if (j == 0)
                                {
                                    if (tsd->missile_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_MISSILE_OFF;
                                        for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                                        {
                                            tilemaparray[m * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                                            tilemapflags[m * NUM_MISSILE_DIRS + n + glyph_offset4] |= GLYPH_TILE_FLAG_NORMAL_ITEM_AS_MISSILE;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    /* Write generic corpse and statue tiles */
                    if (j == 0 &&
                        (tsd->corpse_tile_style != 1 && i == CORPSE)
                        || (!tsd->statue_tile_style && i == STATUE)
                        )
                    {
                        for (int gender = 0; gender <= 1; gender++)
                        {
                            for (int k = LOW_PM; k < NUM_MONSTERS; k++)
                            {
                                if (i == CORPSE && tsd->corpse_tile_style == 2 && (mons[k].mflags5 & M5_CORPSE_TILE))
                                    continue;

                                int glyph_offset2 = (gender == 0 ? (i == CORPSE ? GLYPH_BODY_OFF : GLYPH_STATUE_OFF) : (i == CORPSE ? GLYPH_FEMALE_BODY_OFF : GLYPH_FEMALE_STATUE_OFF));
                                tilemaparray[k + glyph_offset2] = tile_count;
                                tilemapflags[k + glyph_offset2] |= fullsizedflag;
                            }
                        }
                    }


                    tile_count++;
                }
            }
            else
            {
                if (j == 1)
                    tile_count += missile_tile_num;
                else
                    tile_count++;
            }
        }
    }

    /* Artifact tiles */
    tile_section_name = "artifacts";
    for (int j = 0; j <= 1; j++)
    {
        if (j == 1 && !tsd->missile_tile_style)
            continue;

        set_name = (j == 0 ? "normal" :"missile");
        glyph_offset = (j == 0 ? GLYPH_ARTIFACT_OFF : 
            GLYPH_ARTIFACT_MISSILE_OFF);

        for (int i = 1; i <= NUM_ARTIFACTS; i++)
        {
            int missile_tile_num = (artilist[i].aflags2 & AF2_SINGLE_MISSILE_TILE) ? 1 : NUM_MISSILE_DIRS;

            if (j == 1)
            {
                if (tsd->missile_tile_style == 2 && !(artilist[i].aflags2 & AF2_MISSILE_TILE))
                    continue;
            }


            int base_item = artilist[i].otyp;
            boolean no_description = !artilist[i].desc;
            boolean no_base_item_name = !OBJ_NAME(objects[base_item]);
            boolean no_base_item_description = !obj_descr[objects[base_item].oc_name_idx].oc_descr;

            uchar fullsizedflag = !!(objects[base_item].oc_flags4 & O4_FULL_SIZED_BITMAP);

            if (process_style == 0)
            {
                if (j == 1)
                {
                    for (int n = 0; n < missile_tile_num; n++)
                    {
                        Sprintf(buf, "%s,%s,%s,%s,%s,%s,%s,1,1,0\n", tile_section_name, set_name,
                            artilist[i].name,
                            no_description ? "no artifact description" : artilist[i].desc,
                            no_base_item_name ? "nameless base item" : OBJ_NAME(objects[base_item]),
                            no_base_item_description ? "no base item description" : obj_descr[objects[base_item].oc_name_idx].oc_descr,
                            missile_tile_num == 1 ? "generic " : missile_direction_name_array[n]
                        );
                        (void)write(fd, buf, strlen(buf));
                        tile_count++;
                    }
                }
                else
                {
                    Sprintf(buf, "%s,%s,%s,%s,%s,%s", tile_section_name, set_name,
                        artilist[i].name,
                        no_description ? "no artifact description" : artilist[i].desc,
                        no_base_item_name ? "nameless base item" : OBJ_NAME(objects[base_item]),
                        no_base_item_description ? "no base item description" : obj_descr[objects[base_item].oc_name_idx].oc_descr
                    );

                    if (objects[i].oc_flags4 & O4_FULL_SIZED_BITMAP)
                        Sprintf(eos(buf), ",1");
                    else
                        Sprintf(eos(buf), ",0");

                    if (j == 0 && artilist[i].enlargement > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[artilist[i].enlargement].width_in_tiles, enlargements[artilist[i].enlargement].height_in_tiles, enlargements[artilist[i].enlargement].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                    tile_count++;

                }
            }
            else if (process_style == 1)
            {
                if (j == 1)
                {
                    for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                    {
                        tilemaparray[(i - 1) * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
                        if(missile_tile_num != 1)
                            tile_count++;
                    }
                    if (missile_tile_num == 1)
                        tile_count++;
                }
                else
                {
                    tilemaparray[(i - 1) + glyph_offset] = tile_count;
                    tilemapflags[(i - 1) + glyph_offset] |= fullsizedflag;

                    if (j == 0)
                    {
                        if (tsd->missile_tile_style != 1)
                        {
                            int glyph_offset4 = GLYPH_ARTIFACT_MISSILE_OFF;
                            for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                            {
                                tilemaparray[(i - 1) * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                            }
                        }
                    }
                    tile_count++;
                }
            }
            else
            {
                if (j == 1)
                    tile_count += missile_tile_num;
                else
                    tile_count++;
            }
        }
    }

    /* CMAP tiles */
    for (int spset = 0; spset <= 1; spset++)
    {
        tile_section_name = (spset == 0 ? "cmap" : "broken-cmap");
        int base_glyph_offset = (spset == 0 ? GLYPH_CMAP_OFF : GLYPH_BROKEN_CMAP_OFF);
        int base_variation_glyph_offset = (spset == 0 ? GLYPH_CMAP_VARIATION_OFF : GLYPH_BROKEN_CMAP_VARIATION_OFF);
        int num_cmaps = min(CMAP_TYPE_MAX, max(1, tsd->number_of_cmaps));

        for (int tileset_cmap_idx = 0; tileset_cmap_idx < num_cmaps; tileset_cmap_idx++)
        {
            char namebuf[BUFSZ];
            if (tsd->cmap_names[tileset_cmap_idx] && strcmp(tsd->cmap_names[tileset_cmap_idx], ""))
                Sprintf(namebuf, "%s", tsd->cmap_names[tileset_cmap_idx]);
            else
                Sprintf(namebuf, "unnamed-cmap-%d", tileset_cmap_idx);

            for (int i = 0; i < CMAP_TYPE_CHAR_NUM; i++)
            {
                if (tileset_cmap_idx > 0)
                {
                    if (tsd->cmap_limitation_style[tileset_cmap_idx] == 1 && !defsyms[i].included_in_default_tileset_cmap[tileset_cmap_idx])
                        continue;
                    if (tsd->cmap_limitation_style[tileset_cmap_idx] == 2 && (i < S_vwall || i > S_trwall))
                        continue;
                    if (tsd->cmap_limitation_style[tileset_cmap_idx] == 3 && (i < S_stone || i > S_dnladder))
                        continue;
                }

                if (spset == 1 && defsyms[i].has_broken_tile == FALSE)
                    continue;

                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,%s,%s", tile_section_name, namebuf, get_cmap_tilename(i),
                        (defsyms[i].explanation && strcmp(defsyms[i].explanation, "")) ? defsyms[i].explanation : "no description");
                    int enl_idx = (spset == 0 ? defsyms[i].enlargement[tileset_cmap_idx] : defsyms[i].broken_enlargement[tileset_cmap_idx]);
                    if (enl_idx > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[enl_idx].width_in_tiles, enlargements[enl_idx].height_in_tiles, enlargements[enl_idx].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    /* Go through all internal cmaps */
                    for (int k = 0; k < CMAP_TYPE_MAX; k++)
                    {
                        /* Write this tileset_cmap_idx for all internal CMAPs it is used for */
                        if (tsd->cmap_mapping[k] == tileset_cmap_idx)
                        {
                            glyph_offset = base_glyph_offset + k * CMAP_TYPE_CHAR_NUM;
                            tilemaparray[i + glyph_offset] = tile_count;
                            if (i == S_extra_boulder)
                            {
                                if (objects[BOULDER].oc_flags4 & O4_FULL_SIZED_BITMAP)
                                    tilemapflags[i + glyph_offset] |= GLYPH_TILE_FLAG_FULL_SIZED_ITEM;
                            }
                        }
                    }

                    /*

                        for (int k = 1; k < CMAP_TYPE_MAX; k++)
                        {
                            int tileset_cmap_idx2 = tsd->cmap_mapping[k];
                            if (tileset_cmap_idx2 > 0 && tileset_cmap_idx2 != tileset_cmap_idx)
                            {
                                boolean included_in_this_tileset_cmap = defsym_variations[i].included_in_default_tileset_cmap[tileset_cmap_idx2];
                                int base_tileset_cmap = defsym_variations[i].base_default_tileset_cmap[tileset_cmap_idx2];

                    */
                    if (num_cmaps > 1)
                    {
                        /* copy an out-limited tiles to all limited cmaps */
                        for (int k = 0; k < CMAP_TYPE_MAX; k++)
                        {
                            int tileset_cmap_idx2 = tsd->cmap_mapping[k];
                            if (tileset_cmap_idx2 > 0 && tileset_cmap_idx2 != tileset_cmap_idx)
                            {
                                boolean included_in_this_tileset_cmap = defsyms[i].included_in_default_tileset_cmap[tileset_cmap_idx2];
                                int base_tileset_cmap = defsyms[i].base_default_tileset_cmap[tileset_cmap_idx2];
                                if (
                                    (tsd->cmap_limitation_style[tileset_cmap_idx2] == 1 && !included_in_this_tileset_cmap && base_tileset_cmap == tileset_cmap_idx)
                                    || (tsd->cmap_limitation_style[tileset_cmap_idx2] == 2 && (i < S_vwall || i > S_trwall) && tileset_cmap_idx == 0)
                                    || (tsd->cmap_limitation_style[tileset_cmap_idx2] == 3 && (i < S_stone || i > S_dnladder) && tileset_cmap_idx == 0)
                                    )
                                {
                                    int glyph_offset2 = base_glyph_offset + k * CMAP_TYPE_CHAR_NUM;
                                    tilemaparray[i + glyph_offset2] = tile_count;
                                    if (i == S_extra_boulder)
                                    {
                                        if (objects[BOULDER].oc_flags4 & O4_FULL_SIZED_BITMAP)
                                            tilemapflags[i + glyph_offset2] |= GLYPH_TILE_FLAG_FULL_SIZED_ITEM;
                                    }
                                }
                            }
                        }
                    }
                }
                tile_count++;
            }
        }
    }

    /* CMAP variation tiles */
    for (int spset = 0; spset <= 1; spset++)
    {
        tile_section_name = (spset == 0 ? "cmap-variation" : "broken-cmap-variation");
        int base_glyph_offset = (spset == 0 ? GLYPH_CMAP_VARIATION_OFF : GLYPH_BROKEN_CMAP_VARIATION_OFF);
        int num_cmaps = (min(CMAP_TYPE_MAX, max(1, tsd->number_of_cmaps)));
        for (int tileset_cmap_idx = 0; tileset_cmap_idx < num_cmaps; tileset_cmap_idx++)
        {
            char namebuf[BUFSZ];
            if (tsd->cmap_names[tileset_cmap_idx] && strcmp(tsd->cmap_names[tileset_cmap_idx], ""))
                Sprintf(namebuf, "%s", tsd->cmap_names[tileset_cmap_idx]);
            else
                Sprintf(namebuf, "unnamed-cmap-%d", tileset_cmap_idx);

            for (int i = 0; i < MAX_VARIATIONS; i++)
            {
                if (tileset_cmap_idx > 0)
                {
                    if (tsd->cmap_limitation_style[tileset_cmap_idx] == 1 && !defsym_variations[i].included_in_default_tileset_cmap[tileset_cmap_idx])
                        continue;
                    if (tsd->cmap_limitation_style[tileset_cmap_idx] == 2 && !is_wall_variation(i))
                        continue;
                    if (tsd->cmap_limitation_style[tileset_cmap_idx] == 3 && !is_base_cmap_variation(i))
                        continue;
                }

                if (spset == 1 && defsym_variations[i].has_broken_tile == FALSE)
                    continue;

                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,%s", tile_section_name, namebuf,
                        (defsym_variations[i].variation_name && strcmp(defsym_variations[i].variation_name, "")) ? defsym_variations[i].variation_name : "no-variation-name");
                    int enl_idx = (spset == 0 ? defsym_variations[i].enlargement[tileset_cmap_idx] : defsym_variations[i].broken_enlargement[tileset_cmap_idx]);
                    if (enl_idx > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[enl_idx].width_in_tiles, enlargements[enl_idx].height_in_tiles, enlargements[enl_idx].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    /* Go through all internal cmaps */
                    for (int k = 0; k < CMAP_TYPE_MAX; k++)
                    {
                        /* Write this tileset_cmap_idx for all internal CMAPs it is used for */
                        if (tsd->cmap_mapping[k] == tileset_cmap_idx)
                        {
                            glyph_offset = base_glyph_offset + k * MAX_VARIATIONS;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                    }

                    if (num_cmaps > 1)
                    {
                        /* copy an out-limited tile to all limited cmaps */
                        for (int k = 0; k < CMAP_TYPE_MAX; k++)
                        {
                            int tileset_cmap_idx2 = tsd->cmap_mapping[k];
                            if (tileset_cmap_idx2 > 0 && tileset_cmap_idx2 != tileset_cmap_idx)
                            {
                                boolean included_in_this_tileset_cmap = defsym_variations[i].included_in_default_tileset_cmap[tileset_cmap_idx2];
                                int base_tileset_cmap = defsym_variations[i].base_default_tileset_cmap[tileset_cmap_idx2];

                                if (
                                    (tsd->cmap_limitation_style[tileset_cmap_idx2] == 1 && !included_in_this_tileset_cmap && base_tileset_cmap == tileset_cmap_idx)
                                    || (tsd->cmap_limitation_style[tileset_cmap_idx2] == 2 && !is_wall_variation(i) && tileset_cmap_idx == 0)
                                    || (tsd->cmap_limitation_style[tileset_cmap_idx2] == 3 && !is_base_cmap_variation(i) && tileset_cmap_idx == 0)
                                    )
                                {
                                    int glyph_offset2 = base_glyph_offset + k * MAX_VARIATIONS;
                                    tilemaparray[i + glyph_offset2] = tile_count;
                                }
                            }
                        }
                    }
                }
                tile_count++;
            }
        }
    }

    /* Miscellaneous tiles */
    tile_section_name = "misc";
    for (int misc_idx = 0; misc_idx < 5; misc_idx++)
    {
        set_name = (misc_idx == 0 ? "invisible" : misc_idx == 1 ? "explode" : misc_idx == 2 ? "zap" :
            misc_idx == 3 ? "swallow" : "warning");

        if (misc_idx == 0)
        {
            if (NUM_INVIS_GLYPHS == 1)
            {
                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,generic,1,1,0\n", tile_section_name, set_name);
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    tilemaparray[GLYPH_INVIS_OFF] = tile_count;
                }
                tile_count++;
            }
            else
            {
                for (int i = 0; i < NUM_INVIS_GLYPHS; i++)
                {
                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,tile-%d,1,1,0\n", tile_section_name, set_name, i);
                        (void)write(fd, buf, strlen(buf));
                    }
                    else if (process_style == 1)
                    {
                        glyph_offset = GLYPH_INVIS_OFF;
                        tilemaparray[i + glyph_offset] = tile_count;
                    }
                    tile_count++;
                }
            }
        }
        else if (misc_idx == 1)
        {
            const char* explosion_direction_name_array[MAX_EXPLOSION_CHARS] = {
                    "top-left", "top-center", "top-right",
                    "middle-left", "middle-center", "middle-right",
                    "bottom-left", "bottom-center", "bottom-right" };

            if (tsd->has_all_explode_tiles == 0)
            {
                for (int i = 0; i < MAX_EXPLOSION_CHARS; i++)
                {
                    const char* explosion_direction_name = explosion_direction_name_array[i];
                    int x_coord = i % 3;
                    int y_coord = i / 3;

                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s,%d,%d,3,3,1,1,0\n", tile_section_name, set_name, explosion_direction_name, x_coord, y_coord);
                        (void)write(fd, buf, strlen(buf));
                    }
                    else if (process_style == 1)
                    {
                        for (int j = 0; j < EXPL_MAX; j++)
                        {
                            glyph_offset = GLYPH_EXPLODE_OFF + MAX_EXPLOSION_CHARS * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                    }
                    tile_count++;
                }
            }
            else
            {
                for (int j = 0; j < EXPL_MAX; j++)
                {
                    const char* explosion_name = explosion_type_definitions[j].name;
                    for (int i = 0; i < MAX_EXPLOSION_CHARS; i++)
                    {
                        const char* explosion_direction_name = explosion_direction_name_array[i];
                        int x_coord = i % 3;
                        int y_coord = i / 3;
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s,%d,%d,3,3,1,1,0\n", tile_section_name, set_name, explosion_name, explosion_direction_name, x_coord, y_coord);
                            (void)write(fd, buf, strlen(buf));
                        }
                        else if (process_style == 1)
                        {
                            glyph_offset = GLYPH_EXPLODE_OFF + MAX_EXPLOSION_CHARS * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                        tile_count++;
                    }
                }
            }
        }
        else if (misc_idx == 2)
        {
            const char* zap_direction_name_array[MAX_ZAP_CHARS] = {
                "vertical", "horizontal", "diagonal-top-left-to-bottom-right", "diagonal-bottom-left-to-top-right" };

            if (tsd->has_all_zap_tiles == 0)
            {
                for (int i = 0; i < MAX_ZAP_CHARS; i++)
                {
                    const char* zap_direction_name = zap_direction_name_array[i];
                    int x_coord = i % 2;
                    int y_coord = i / 2;
                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s,%d,%d,2,2,1,1,0\n", tile_section_name, set_name, zap_direction_name, x_coord, y_coord);
                        (void)write(fd, buf, strlen(buf));
                    }
                    else if (process_style == 1)
                    {
                        for (int j = 0; j < NUM_ZAP; j++)
                        {
                            glyph_offset = GLYPH_ZAP_OFF + MAX_ZAP_CHARS * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                    }
                    tile_count++;
                }
            }
            else
            {
                for (int j = 0; j < NUM_ZAP; j++)
                {
                    const char* zap_name = zap_type_definitions[j].name;

                    for (int i = 0; i < MAX_ZAP_CHARS; i++)
                    {
                        const char* zap_direction_name = zap_direction_name_array[i];
                        int x_coord = i % 2;
                        int y_coord = i / 2;
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s,%d,%d,2,2,1,1,0\n", tile_section_name, set_name, zap_name, zap_direction_name, x_coord, y_coord);
                            (void)write(fd, buf, strlen(buf));
                        }
                        else if (process_style == 1)
                        {
                            glyph_offset = GLYPH_ZAP_OFF + MAX_ZAP_CHARS * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                        tile_count++;
                    }
                }
            }
        }
        else if (misc_idx == 3)
        {
            const char* swallow_direction_name_array[MAX_SWALLOW_CHARS] = {
                    "top-left", "top-center", "top-right",
                    "middle-left", "middle-center", "middle-right",
                    "bottom-left", "bottom-center", "bottom-right" };

            if (tsd->swallow_tile_style == 0)
            {
                for (int i = 0; i < MAX_SWALLOW_CHARS; i++)
                {
                    const char* swallow_direction_name = swallow_direction_name_array[i];
                    int x_coord = i % 3;
                    int y_coord = i / 3;
                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s,%d,%d,3,3,1,1,0\n", tile_section_name, set_name, swallow_direction_name, x_coord, y_coord);
                        (void)write(fd, buf, strlen(buf));
                    }
                    else if (process_style == 1)
                    {
                        for (int j = 0; j < NUM_MONSTERS; j++)
                        {
                            glyph_offset = GLYPH_SWALLOW_OFF + MAX_SWALLOW_CHARS * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                    }
                    tile_count++;
                }
            }
            else
            {
                boolean first_found = FALSE;
                for (int j = 0; j < NUM_MONSTERS; j++)
                {
                    if (tsd->swallow_tile_style == 2)
                    {
                        if (!attacktype(&mons[j], AT_ENGL))
                            continue;
                    }

                    for (int i = 0; i < MAX_SWALLOW_CHARS; i++)
                    {
                        const char* swallow_direction_name = swallow_direction_name_array[i];
                        int x_coord = i % 3;
                        int y_coord = i / 3;
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s,%d,%d,3,3,1,1,0\n", tile_section_name, set_name, mons[j].mname, swallow_direction_name, x_coord, y_coord);
                            (void)write(fd, buf, strlen(buf));
                        }
                        else if (process_style == 1)
                        {
                            glyph_offset = GLYPH_SWALLOW_OFF + MAX_SWALLOW_CHARS * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                        tile_count++;
                    }

                    /* Write the first found swallow tile set for all monsters, just in case */
                    if (tsd->swallow_tile_style == 2 && !first_found && process_style == 1)
                    {
                        first_found = TRUE;
                        for (int k = 0; k < NUM_MONSTERS; k++)
                        {
                            for (int m = 0; m < MAX_SWALLOW_CHARS; m++)
                            {
                                glyph_offset = GLYPH_SWALLOW_OFF + MAX_SWALLOW_CHARS * k;
                                tilemaparray[m + glyph_offset] = tile_count - MAX_SWALLOW_CHARS + m;
                            }
                        }
                    }
                }
            }
        }
        else if (misc_idx == 4)
        {
            for (int i = 0; i < WARNCOUNT; i++)
            {
                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,warn-level-%d,1,1,0\n", tile_section_name, set_name, i);
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    glyph_offset = GLYPH_WARNING_OFF;
                    tilemaparray[i + glyph_offset] = tile_count;
                }
                tile_count++;
            }
        }
    }


    /* Player tiles */
    tile_section_name = "player";
    const char* player_set_name_array[MAX_ACTION_TILES] = { "normal", "attack", "throw", "fire", "cast", "special-attack", "kick", "passive-defense", "defend", "receive-damage", "item-use", "door-use", "death" };

    /* Initialize glyphs first with monster tiles */
    for (enum action_tile_types spset = 0; spset < MAX_ACTION_TILES; spset++)
    {
        set_name = player_set_name_array[spset];
        int player_glyph_offset = get_player_action_glyph_offset(spset);
        if (tsd->player_tile_style == 0 || tsd->player_tile_style == 3) /* For style 3, fill out all cases with monster tiles, and override below */
        {
            /* Use player monster icons */
            if (process_style == 1)
            {
                for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
                {
                    int role_as_monster = roles[roleidx].monsternum;
                    for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
                    {
                        for (int gender = 0; gender <= 1; gender++)
                        {
                            for (int alignment = -1; alignment <= 1; alignment++)
                            {
                                for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                                {
                                    int player_glyph = player_to_glyph_index(roleidx, raceidx, gender, alignment + 1, level) + player_glyph_offset;
                                    tilemaparray[player_glyph] = get_monster_action_glyph_offset(spset, gender); // tilemaparray[role_as_monster + ((gender == 0) ? player_as_mon_glyph_offset_array[spset] : player_as_female_mon_glyph_offset_array[spset])];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /* Now main player tiles */
    for (enum action_tile_types spset = ACTION_TILE_NO_ACTION; spset < MAX_ACTION_TILES; spset++)
    {
        set_name = player_set_name_array[spset];
        int player_glyph_offset = get_player_action_glyph_offset(spset);

        if (tsd->player_tile_style == 1)
        {
            if (!has_generic_player_action_tile(spset))
                continue;

            if (process_style == 0)
            {
                Sprintf(buf, "%s,%s,generic,1,1,0\n", tile_section_name, set_name);
                (void)write(fd, buf, strlen(buf));
            }
            else if (process_style == 1)
            {
                for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
                {
                    for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
                    {
                        for (int gender = 0; gender <= 1; gender++)
                        {
                            for (int alignment = -1; alignment <= 1; alignment++)
                            {
                                for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                                {
                                    int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, alignment + 1, level);
                                    int player_glyph = player_glyph_index + player_glyph_offset;
                                    tilemaparray[player_glyph] = tile_count;

                                    if (spset == ACTION_TILE_NO_ACTION)
                                    {
                                        for (enum action_tile_types spset2 = ACTION_TILE_ATTACK; spset2 < MAX_ACTION_TILES; spset2++)
                                        {
                                            if (!has_generic_player_action_tile(spset2))
                                            {
                                                int player_glyph2 = get_player_action_glyph_offset(spset2);
                                                tilemaparray[player_glyph2] = tile_count;
                                            }
                                        }
                                    }
                                    else if (spset == ACTION_TILE_ATTACK)
                                    {
                                        enum action_tile_types action_array[5] = { ACTION_TILE_THROW, ACTION_TILE_FIRE, ACTION_TILE_CAST, ACTION_TILE_SPECIAL_ATTACK, ACTION_TILE_KICK };
                                        for (int idx = 0; idx < 5; idx++)
                                        {
                                            enum action_tile_types action = action_array[idx];
                                            if (!has_generic_player_action_tile(idx))
                                            {
                                                int player_glyph2 = get_player_action_glyph_offset(idx);
                                                tilemaparray[player_glyph2] = tile_count;
                                            }
                                        }
                                    }
                                    else if (spset == ACTION_TILE_DOOR_USE)
                                    {
                                        enum action_tile_types action_array[3] = { ACTION_TILE_THROW, ACTION_TILE_CAST, ACTION_TILE_ITEM_USE };
                                        for (int idx = 0; idx < 3; idx++)
                                        {
                                            enum action_tile_types action = action_array[idx];
                                            if (!has_generic_player_action_tile(idx))
                                            {
                                                int player_glyph2 = get_player_action_glyph_offset(idx);
                                                tilemaparray[player_glyph2] = tile_count;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            tile_count++;
        }
        else if (tsd->player_tile_style == 2)
        {
            for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
            {
                const char* role_name = roles[roleidx].name.m;
                for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
                {
                    const char* race_name = races[raceidx].noun;
                    for (int gender = 0; gender <= 1; gender++)
                    {
                        const char* gender_name = (gender == 0 ? "male" : "female");
                        for (int alignment = -1; alignment <= 1; alignment++)
                        {
                            const char* align_name = (alignment == -1 ? "chaotic" : alignment == 0 ? "neutral" : alignment == 1 ? "lawful" : "unspecified");
                            for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                            {
                                if (!player_has_action_tile(spset, roleidx, raceidx, gender, alignment + 1, level))
                                    continue;

                                if (process_style == 0)
                                {
                                    Sprintf(buf, "%s,%s,%s,%s,%s,%s,level-%d", tile_section_name, set_name, role_name, race_name, gender_name, align_name, level);
                                    int pl_enl = get_player_enlargement(spset, roleidx, raceidx, gender, alignment + 1, level);
                                    if (pl_enl > 0)
                                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[pl_enl].width_in_tiles, enlargements[pl_enl].height_in_tiles, enlargements[pl_enl].main_tile_x_coordinate);
                                    else
                                        Sprintf(eos(buf), ",1,1,0");
                                    Sprintf(eos(buf), "\n");
                                    (void)write(fd, buf, strlen(buf));
                                }
                                else if (process_style == 1)
                                {
                                    int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, alignment + 1, level);
                                    int player_glyph = player_glyph_index + player_glyph_offset;
                                    tilemaparray[player_glyph] = tile_count;

                                    if (spset == 0)
                                    {
                                        for (enum action_tile_types action = ACTION_TILE_ATTACK; action < MAX_ACTION_TILES; action++)
                                        {
                                            if (!player_has_action_tile(action, roleidx, raceidx, gender, alignment + 1, level))
                                            {
                                                int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                tilemaparray[player_glyph2] = tile_count;
                                            }
                                        }
                                    }
                                    else if (spset == ACTION_TILE_ATTACK)
                                    {
                                        enum action_tile_types action_array[5] = {ACTION_TILE_THROW, ACTION_TILE_FIRE, ACTION_TILE_CAST, ACTION_TILE_SPECIAL_ATTACK, ACTION_TILE_KICK };
                                        for (int idx = 0; idx < 5; idx++)
                                        {
                                            enum action_tile_types action = action_array[idx];
                                            if (!player_has_action_tile(action, roleidx, raceidx, gender, alignment + 1, level))
                                            {
                                                int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                tilemaparray[player_glyph2] = tile_count;
                                            }
                                        }
                                    }
                                    else if (spset == ACTION_TILE_DOOR_USE)
                                    {
                                        enum action_tile_types action_array[3] = { ACTION_TILE_THROW, ACTION_TILE_CAST, ACTION_TILE_ITEM_USE };
                                        for (int idx = 0; idx < 3; idx++)
                                        {
                                            enum action_tile_types action = action_array[idx];
                                            if (!player_has_action_tile(action, roleidx, raceidx, gender, alignment + 1, level))
                                            {
                                                int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                tilemaparray[player_glyph2] = tile_count;
                                            }
                                        }
                                    }
                                }
                                tile_count++;
                            }
                        }
                    }
                }
            }
        }
        else if (tsd->player_tile_style == 3)
        {
            for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
            {
                const char* role_name = roles[roleidx].name.m;
                for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
                {
                    if (!validrace(roleidx, raceidx))
                        continue;

                    const char* race_name = races[raceidx].noun;
                    for (int gender = 0; gender <= 1; gender++)
                    {
                        const char* gender_name = (gender == 0 ? "male" : "female");
                        for (int alignment = -1; alignment <= 1; alignment++)
                        {
                            if (alignment > -1 && !(roles[roleidx].allow & ROLE_ALIGNMENT_TILES))
                                continue;

                            const char* align_name = (roles[roleidx].allow & ROLE_ALIGNMENT_TILES) ? (alignment == -1 ? "chaotic" : alignment == 0 ? "neutral" : alignment == 1 ? "lawful" : "unspecified") : "any";
                            for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                            {
                                if (!player_has_action_tile(spset, roleidx, raceidx, gender, alignment + 1, level))
                                    continue;

                                if (process_style == 0)
                                {
                                    Sprintf(buf, "%s,%s,%s,%s,%s,%s,level-%d", tile_section_name, set_name, role_name, race_name, gender_name, align_name, level);
                                    int pl_enl = get_player_enlargement(spset, roleidx, raceidx, gender, alignment + 1, level);
                                    if (pl_enl > 0)
                                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[pl_enl].width_in_tiles, enlargements[pl_enl].height_in_tiles, enlargements[pl_enl].main_tile_x_coordinate);
                                    else
                                        Sprintf(eos(buf), ",1,1,0");
                                    Sprintf(eos(buf), "\n");
                                    (void)write(fd, buf, strlen(buf));
                                }
                                else if (process_style == 1)
                                {
                                    if (alignment == -1 && !(roles[roleidx].allow & ROLE_ALIGNMENT_TILES))
                                    {
                                        for (int k = 0; k <= 2; k++)
                                        {
                                            int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, k, level);
                                            int player_glyph = player_glyph_index + player_glyph_offset;
                                            tilemaparray[player_glyph] = tile_count;
                                            if (spset == ACTION_TILE_NO_ACTION)
                                            {
                                                for (enum action_tile_types action = ACTION_TILE_ATTACK; action < MAX_ACTION_TILES; action++)
                                                {
                                                    if (!player_has_action_tile(action, roleidx, raceidx, gender, k, level))
                                                    {
                                                        int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                        tilemaparray[player_glyph2] = tile_count;
                                                    }
                                                }
                                            }
                                            else if (spset == ACTION_TILE_ATTACK)
                                            {
                                                enum action_tile_types action_array[5] = { ACTION_TILE_THROW, ACTION_TILE_FIRE, ACTION_TILE_CAST, ACTION_TILE_SPECIAL_ATTACK, ACTION_TILE_KICK };
                                                for (int idx = 0; idx < 5; idx++)
                                                {
                                                    enum action_tile_types action = action_array[idx];
                                                    if (!player_has_action_tile(action, roleidx, raceidx, gender, k, level))
                                                    {
                                                        int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                        tilemaparray[player_glyph2] = tile_count;
                                                    }
                                                }
                                            }
                                            else if (spset == ACTION_TILE_DOOR_USE)
                                            {
                                                enum action_tile_types action_array[3] = { ACTION_TILE_THROW, ACTION_TILE_CAST, ACTION_TILE_ITEM_USE };
                                                for (int idx = 0; idx < 3; idx++)
                                                {
                                                    enum action_tile_types action = action_array[idx];
                                                    if (!player_has_action_tile(action, roleidx, raceidx, gender, k, level))
                                                    {
                                                        int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                        tilemaparray[player_glyph2] = tile_count;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, alignment + 1, level);
                                        int player_glyph = player_glyph_index + player_glyph_offset;
                                        tilemaparray[player_glyph] = tile_count;
                                        if (spset == ACTION_TILE_NO_ACTION)
                                        {
                                            for (enum action_tile_types action = ACTION_TILE_ATTACK; action < MAX_ACTION_TILES; action++)
                                            {
                                                if (!player_has_action_tile(action, roleidx, raceidx, gender, alignment + 1, level))
                                                {
                                                    int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                    tilemaparray[player_glyph2] = tile_count;
                                                }
                                            }
                                        }
                                        else if (spset == ACTION_TILE_ATTACK)
                                        {
                                            enum action_tile_types action_array[5] = { ACTION_TILE_THROW, ACTION_TILE_FIRE, ACTION_TILE_CAST, ACTION_TILE_SPECIAL_ATTACK, ACTION_TILE_KICK };
                                            for (int idx = 0; idx < 5; idx++)
                                            {
                                                enum action_tile_types action = action_array[idx];
                                                if (!player_has_action_tile(action, roleidx, raceidx, gender, alignment + 1, level))
                                                {
                                                    int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                    tilemaparray[player_glyph2] = tile_count;
                                                }
                                            }
                                        }
                                        else if (spset == ACTION_TILE_DOOR_USE)
                                        {
                                            enum action_tile_types action_array[3] = { ACTION_TILE_THROW, ACTION_TILE_CAST, ACTION_TILE_ITEM_USE };
                                            for (int idx = 0; idx < 3; idx++)
                                            {
                                                enum action_tile_types action = action_array[idx];
                                                if (!player_has_action_tile(action, roleidx, raceidx, gender, alignment + 1, level))
                                                {
                                                    int player_glyph2 = player_glyph_index + get_player_action_glyph_offset(action);
                                                    tilemaparray[player_glyph2] = tile_count;
                                                }
                                            }
                                        }
                                    }
                                }
                                tile_count++;
                            }
                        }
                    }
                }
            }
        }
    }


    /* User interface tiles */
    tile_section_name = "user-interface";
    set_name = "cursor";

    for (int i = 0; i < MAX_CURSORS; i++)
    {
        if (process_style == 0)
        {
            Sprintf(buf, "%s,%s,%s,1,1,0\n", tile_section_name, set_name, game_cursors[i].game_cursor_name);
            (void)write(fd, buf, strlen(buf));
        }
        else if (process_style == 1)
        {
            glyph_offset = GLYPH_CURSOR_OFF;
            tilemaparray[i + GLYPH_CURSOR_OFF] = tile_count;
        }
        tile_count++;
    }

    set_name = "ui-tile";
    for (int i = 0; i < MAX_UI_TILES; i++)
    {
        if (process_style == 0)
        {
            Sprintf(buf, "%s,%s,%s,%d,%d,%d", tile_section_name, set_name, 
                ui_tile_component_array[i].name, 
                ui_tile_component_array[i].number,
                ui_tile_component_array[i].width,
                ui_tile_component_array[i].height
            );
            for (int j = 0; j < ui_tile_component_array[i].number; j++)
            {
                Sprintf(eos(buf), ",%s", ui_tile_component_array[i].component_names[j]);
            }
            Sprintf(eos(buf), "\n");
            (void)write(fd, buf, strlen(buf));
        }
        else if (process_style == 1)
        {
            glyph_offset = GLYPH_UI_TILE_OFF;
            tilemaparray[i + GLYPH_UI_TILE_OFF] = tile_count;
        }
        tile_count++;
    }

    /* Replacement tiles */
    tile_section_name = "replacement";
    for (int i = 1; i <= NUM_REPLACEMENTS; i++)  /* replacement number, starts at 1 */
    {
        short base_tile = get_replacement_base_tile(i);
        for (int j = 0; j < max(0, min(replacements[i].number_of_tiles, MAX_TILES_PER_REPLACEMENT)); j++) /* tile number */
        {
            if (process_style == 0)
            {
                Sprintf(buf, "%s,%s,%s,%d", tile_section_name,
                    replacements[i].replacement_name ? replacements[i].replacement_name : "unknown replacement",
                    replacements[i].tile_names[j],
                    base_tile
                );
                int enl = replacements[i].tile_enlargement[j];
                if (enl > 0)
                    Sprintf(eos(buf), ",%d,%d,%d", enlargements[enl].width_in_tiles, enlargements[enl].height_in_tiles, enlargements[enl].main_tile_x_coordinate);
                else
                    Sprintf(eos(buf), ",1,1,0");
                Sprintf(eos(buf), "\n");
                (void)write(fd, buf, strlen(buf));
            }
            else if (process_style == 1)
            {
                tilemaparray[j + replacements[i].glyph_offset + GLYPH_REPLACEMENT_OFF] = tile_count;
            }
            tile_count++;
        }
    }

    /* Animation tiles */
    tile_section_name = "animation";
    for (int i = 1; i <= NUM_ANIMATIONS; i++)  /* animation number, starts at 1 */
    {
        short base_tile = get_animation_base_tile(i);
        int contained_anims = max(1, animations[i].number_of_tile_animations);
        for (int m = 0; m < contained_anims; m++)
        {
            for (int j = 0; j < max(0, min(animations[i].number_of_tiles, MAX_TILES_PER_ANIMATION)); j++) /* tile number */
            {
                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,tile-%d,%d", tile_section_name,
                        animations[i].animation_name ? animations[i].animation_name : "unknown animation",
                        j,
                        base_tile + m
                    );
                    int enl = animations[i].tile_enlargement;
                    if (enl > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[enl].width_in_tiles, enlargements[enl].height_in_tiles, enlargements[enl].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    glyph_offset = GLYPH_ANIMATION_OFF;
                    int n_frames = min(animations[i].number_of_frames, MAX_FRAMES_PER_ANIMATION);
                    for (int k = 0; k < n_frames; k++)  /* frame number */
                    {
                        if (animations[i].frame2tile[k] == j)
                            tilemaparray[k + n_frames * m + animations[i].glyph_offset + GLYPH_ANIMATION_OFF] = tile_count;
                        else if (animations[i].frame2tile[k] == -1)
                            tilemaparray[k + n_frames * m + animations[i].glyph_offset + GLYPH_ANIMATION_OFF] = base_tile;
                    }
                }
                tile_count++;
            }
        }
    }


    /* Enlargement tiles */
    tile_section_name = "enlargement";
    for (int i = 1; i <= NUM_ENLARGEMENTS; i++)  /* enlargement number, starts at 1 */
    {
        short enl_anim_tiles = enlargements[i].number_of_animation_tiles ? enlargements[i].number_of_animation_tiles : 1;
        for (int m = 0; m < enl_anim_tiles; m++)
        {
            short enl_anim_tile_idx = enlargements[i].number_of_animation_tiles ? m : -1;
            short base_tile = get_enlargement_base_tile(i, enl_anim_tile_idx);
            for (int j = 0; j < enlargements[i].number_of_enlargement_tiles; j++)
            {
                /* j is enlargement glyph index number, the same as position tile number */
                const char* pos_name = "unknown";
                const char* position_names[NUM_POSITIONS_IN_ENLARGEMENT] = { "top-left", "top", "top-right", "left", "right" };

                int position = -1;
                for (int k = 0; k < NUM_POSITIONS_IN_ENLARGEMENT; k++) /* position number */
                {
                    if (enlargements[i].position2tile[k] == j)
                    {
                        position = k;
                        break;
                    }
                }
                if (position >= 0)
                {
                    pos_name = position_names[position];

                }

                if (process_style == 0)
                {
                    char tilebuf[BUFSIZ];
                    strcpy(tilebuf, "");
//                    if (enl_anim_tile_idx >= 0)
//                        Sprintf(tilebuf, "_tile-%d", enl_anim_tile_idx);

                    char enlbuf[BUFSIZ];
                    Sprintf(enlbuf, "%s%s", enlargements[i].enlargement_name ? enlargements[i].enlargement_name : "unknown enlargement",
                        tilebuf);

                    Sprintf(buf, "%s,%s,%s,%d,%d,%d,%d,%d,%d,%d\n", tile_section_name,
                        enlbuf,
                        pos_name,
                        base_tile,
                        enlargements[i].width_in_tiles,
                        enlargements[i].height_in_tiles,
                        enlargements[i].main_tile_x_coordinate,
                        enlargements[i].number_of_enlargement_tiles,
                        position,
                        enl_anim_tile_idx
                    );
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    glyph_offset = GLYPH_ENLARGEMENT_OFF;
                    int addedindex = enl_anim_tile_idx >= 0 ?
                        enl_anim_tile_idx * enlargements[i].number_of_enlargement_tiles
                        : 0;
                    tilemaparray[j + addedindex + enlargements[i].glyph_offset + GLYPH_ENLARGEMENT_OFF] = tile_count;
#if 0
                    for (int k = 0; k < min(enlargements[i].number_of_enlargement_tiles, NUM_POSITIONS_IN_ENLARGEMENT); k++)  /* frame number */
                    {
                        if (enlargements[i].position2tile[k] == j)
                            tilemaparray[k + addedindex + enlargements[i].glyph_offset + GLYPH_ENLARGEMENT_OFF] = tile_count;
                    }
#endif
                }
                tile_count++;
            }
        }
    }


    /* Finalize */
    if (process_style == 0)
    {
        /* Finished */
        (void)close(fd);
    }
    else if (process_style == 1)
    {
        /*
         *Set tile2replacement
         */

         /* Monsters */
        for (int i = 0; i < NUM_MONSTERS; i++)
        {
            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
            {
                if (mons[i].replacement.actions[action])
                {
                    int glyph = i + get_monster_action_glyph_offset(action, 0);
                    short tile = tilemaparray[glyph];
                    tile2replacement[tile] = mons[i].replacement.actions[action];
                }
            }
            if (mons[i].replacement.corpse)
            {
                int glyph = i + GLYPH_BODY_OFF;
                short tile = tilemaparray[glyph];
                tile2replacement[tile] = mons[i].replacement.corpse;
            }
            if (mons[i].replacement.statue)
            {
                int glyph = i + GLYPH_STATUE_OFF;
                short tile = tilemaparray[glyph];
                tile2replacement[tile] = mons[i].replacement.statue;
            }


            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
            {
                if (mons[i].female_replacement.actions[action])
                {
                    int glyph = i + get_monster_action_glyph_offset(action, 1);
                    short tile = tilemaparray[glyph];
                    tile2replacement[tile] = mons[i].replacement.actions[action];
                }
            }
            if (mons[i].female_replacement.corpse)
            {
                int glyph = i + GLYPH_FEMALE_BODY_OFF;
                short tile = tilemaparray[glyph];
                tile2replacement[tile] = mons[i].female_replacement.corpse;
            }
            if (mons[i].female_replacement.statue)
            {
                int glyph = i + GLYPH_FEMALE_STATUE_OFF;
                short tile = tilemaparray[glyph];
                tile2replacement[tile] = mons[i].female_replacement.statue;
            }
        }

        /* Objects */
        for (int i = 0; i < NUM_OBJECTS; i++)
        {
            if (obj_descr[i].replacement)
            {
                int glyph = objnum_to_glyph(i);
                short tile = glyph2tile[glyph];
                tile2replacement[tile] = obj_descr[i].replacement;
            }
        }

        /* Artifacts */
        for (int i = 1; i <= NUM_ARTIFACTS; i++)
        {
            if (artilist[i].replacement)
            {
                int glyph = objnum_to_glyph(i);
                short tile = glyph2tile[glyph];
                tile2replacement[tile] = artilist[i].replacement;
            }
        }

        /* CMAP */
        for (int cmap_type_idx = 0; cmap_type_idx < CMAP_TYPE_MAX; cmap_type_idx++)
        {
            for (int i = 0; i < MAX_CMAPPED_CHARS; i++)
            {
                if (defsyms[i].replacement[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * CMAP_TYPE_CHAR_NUM + GLYPH_CMAP_OFF;
                    short tile = glyph2tile[glyph];
                    tile2replacement[tile] = defsyms[i].replacement[cmap_type_idx];
                }
                if (defsyms[i].broken_replacement[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * CMAP_TYPE_CHAR_NUM + GLYPH_BROKEN_CMAP_OFF;
                    short tile = glyph2tile[glyph];
                    tile2replacement[tile] = defsyms[i].broken_replacement[cmap_type_idx];
                }
            }
        }

        /* CMAP Variation */
        for (int cmap_type_idx = 0; cmap_type_idx < CMAP_TYPE_MAX; cmap_type_idx++)
        {
            for (int i = 0; i < MAX_VARIATIONS; i++)
            {
                if (defsym_variations[i].replacement[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF;
                    short tile = glyph2tile[glyph];
                    tile2replacement[tile] = defsym_variations[i].replacement[cmap_type_idx];
                }
                if (defsym_variations[i].broken_replacement[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * MAX_VARIATIONS + GLYPH_BROKEN_CMAP_VARIATION_OFF;
                    short tile = glyph2tile[glyph];
                    tile2replacement[tile] = defsym_variations[i].broken_replacement[cmap_type_idx];
                }
            }
        }

        /* Player */
        for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
        {
            for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
            {
                for (int gender = 0; gender <= 1; gender++)
                {
                    for (int alignment = -1; alignment <= 1; alignment++)
                    {
                        for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                        {
                            int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, alignment + 1, level);
                            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                            {
                                int anim_idx = get_player_replacement(action, roleidx, raceidx, gender, alignment, level);
                                if (anim_idx)
                                {
                                    int player_glyph = player_glyph_index + get_player_action_glyph_offset(action);
                                    short tile = glyph2tile[player_glyph];
                                    tile2replacement[tile] = anim_idx;
                                }
                            }
                        }
                    }
                }
            }
        }

        /* Cursors */
        for (int i = 0; i < MAX_CURSORS; i++)
        {
            if (game_cursors[i].replacement)
            {
                int glyph = i + GLYPH_CURSOR_OFF;
                short tile = glyph2tile[glyph];
                tile2replacement[tile] = game_cursors[i].replacement;
            }
        }

        /* UI Tiles */
        for (int i = 0; i < MAX_UI_TILES; i++)
        {
            if (ui_tile_component_array[i].replacement)
            {
                int glyph = i + GLYPH_UI_TILE_OFF;
                short tile = glyph2tile[glyph];
                tile2replacement[tile] = ui_tile_component_array[i].replacement;
            }
        }



        /*
         *Set tile2animation 
         */

        /* Monsters */
        for (int i = 0; i < NUM_MONSTERS; i++)
        {
            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
            {
                if (mons[i].animation.actions[action])
                {
                    int glyph = i + get_monster_action_glyph_offset(action, 0);
                    short tile = tilemaparray[glyph];
                    tile2animation[tile] = mons[i].animation.actions[action];
                }
            }
            if (mons[i].animation.corpse)
            {
                int glyph = i + GLYPH_BODY_OFF;
                short tile = tilemaparray[glyph];
                tile2animation[tile] = mons[i].animation.corpse;
            }
            if (mons[i].animation.statue)
            {
                int glyph = i + GLYPH_STATUE_OFF;
                short tile = tilemaparray[glyph];
                tile2animation[tile] = mons[i].animation.statue;
            }


            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
            {
                if (mons[i].female_animation.actions[action])
                {
                    int glyph = i + get_monster_action_glyph_offset(action, 1);
                    short tile = tilemaparray[glyph];
                    tile2animation[tile] = mons[i].animation.actions[action];
                }
            }
            if (mons[i].female_animation.corpse)
            {
                int glyph = i + GLYPH_FEMALE_BODY_OFF;
                short tile = tilemaparray[glyph];
                tile2animation[tile] = mons[i].female_animation.corpse;
            }
            if (mons[i].female_animation.statue)
            {
                int glyph = i + GLYPH_FEMALE_STATUE_OFF;
                short tile = tilemaparray[glyph];
                tile2animation[tile] = mons[i].female_animation.statue;
            }
        }

        /* Objects */
        for (int i = 0; i < NUM_OBJECTS; i++)
        {
            if (obj_descr[i].stand_animation)
            {
                int glyph = objnum_to_glyph(i);
                short tile = glyph2tile[glyph];
                tile2animation[tile] = obj_descr[i].stand_animation;
            }
        }

        /* Artifacts */
        for (int i = 1; i <= NUM_ARTIFACTS; i++)
        {
            if (artilist[i].stand_animation)
            {
                int glyph = objnum_to_glyph(i);
                short tile = glyph2tile[glyph];
                tile2animation[tile] = artilist[i].stand_animation;
            }
        }

        /* CMAP */
        for (int cmap_type_idx = 0; cmap_type_idx < CMAP_TYPE_MAX; cmap_type_idx++)
        {
            for (int i = 0; i < MAX_CMAPPED_CHARS; i++)
            {
                if (defsyms[i].stand_animation[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * CMAP_TYPE_CHAR_NUM + GLYPH_CMAP_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = defsyms[i].stand_animation[cmap_type_idx];
                }
                if (defsyms[i].broken_animation[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * CMAP_TYPE_CHAR_NUM + GLYPH_BROKEN_CMAP_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = defsyms[i].broken_animation[cmap_type_idx];
                }
            }
        }

        /* CMAP Variation */
        for (int cmap_type_idx = 0; cmap_type_idx < CMAP_TYPE_MAX; cmap_type_idx++)
        {
            for (int i = 0; i < MAX_VARIATIONS; i++)
            {
                if (defsym_variations[i].stand_animation[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = defsym_variations[i].stand_animation[cmap_type_idx];
                }
                if (defsym_variations[i].broken_animation[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * MAX_VARIATIONS + GLYPH_BROKEN_CMAP_VARIATION_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = defsym_variations[i].broken_animation[cmap_type_idx];
                }
            }
        }

        /* Player */
        for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
        {
            for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
            {
                for (int gender = 0; gender <= 1; gender++)
                {
                    for (int alignment = -1; alignment <= 1; alignment++)
                    {
                        for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                        {
                            int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, alignment + 1, level);
                            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                            {
                                int anim_idx = get_player_animation(action, roleidx, raceidx, gender, alignment, level);
                                if (anim_idx)
                                {
                                    int player_glyph = player_glyph_index + get_player_action_glyph_offset(action);
                                    short tile = glyph2tile[player_glyph];
                                    tile2animation[tile] = anim_idx;
                                }
                            }
                        }
                    }
                }
            }
        }

        /* Explosion */
        for (int i = 0; i < EXPL_MAX; i++)
        {
            if (explosion_type_definitions[i].animation)
            {
                for (int j = 0; j < MAX_EXPLOSION_CHARS; j++)
                {
                    int glyph = j + i * MAX_EXPLOSION_CHARS + GLYPH_EXPLODE_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = explosion_type_definitions[i].animation;
                }
            }
        }

        /* Zap */
        for (int i = 0; i < NUM_ZAP; i++)
        {
            if (zap_type_definitions[i].animation)
            {
                for (int j = 0; j < MAX_ZAP_CHARS; j++)
                {
                    int glyph = j + i * MAX_ZAP_CHARS + GLYPH_ZAP_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = zap_type_definitions[i].animation;
                }
            }
        }

        /* Swallow */
        for (int i = 0; i < NUM_MONSTERS; i++)
        {
            if (mons[i].animation.swallow)
            {
                for (int j = 0; j < MAX_SWALLOW_CHARS; j++)
                {
                    int glyph = j + i * MAX_SWALLOW_CHARS + GLYPH_SWALLOW_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = mons[i].animation.swallow;
                }
            }
            if (mons[i].female_animation.swallow)
            {
                for (int j = 0; j < MAX_SWALLOW_CHARS; j++)
                {
                    int glyph = j + i * MAX_SWALLOW_CHARS + GLYPH_SWALLOW_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = mons[i].female_animation.swallow;
                }
            }
        }

        /* Cursors */
        for (int i = 0; i < MAX_CURSORS; i++)
        {
            if (game_cursors[i].animation)
            {
                int glyph = i + GLYPH_CURSOR_OFF;
                short tile = glyph2tile[glyph];
                tile2animation[tile] = game_cursors[i].animation;
            }
        }

        /* UI Tiles */
        for (int i = 0; i < MAX_UI_TILES; i++)
        {
            if (ui_tile_component_array[i].animation)
            {
                int glyph = i + GLYPH_UI_TILE_OFF;
                short tile = glyph2tile[glyph];
                tile2animation[tile] = ui_tile_component_array[i].animation;
            }
        }


        /* Replacements */
        for (int i = 1; i <= NUM_REPLACEMENTS; i++)
        {
            for (int j = 0; j < replacements[i].number_of_tiles; j++)
            {
                int anim_idx = replacements[i].tile_animation[j];
                if (anim_idx)
                {
                    int glyph = j + replacements[i].glyph_offset + GLYPH_REPLACEMENT_OFF;
                    short tile = glyph2tile[glyph];
                    tile2animation[tile] = anim_idx;
                }
            }
        }


        /*
         * Set tile2enlargement
         */

        /* Monsters */
        for (int i = 0; i < NUM_MONSTERS; i++)
        {
            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
            {
                if (mons[i].enlargement.actions[action])
                {
                    int glyph = i + get_monster_action_glyph_offset(action, 0);
                    short tile = tilemaparray[glyph];
                    tile2enlargement[tile] = mons[i].enlargement.actions[action];
                }
            }
            if (mons[i].enlargement.corpse)
            {
                int glyph = i + GLYPH_BODY_OFF;
                short tile = tilemaparray[glyph];
                tile2enlargement[tile] = mons[i].enlargement.corpse;
            }
            if (mons[i].enlargement.statue)
            {
                int glyph = i + GLYPH_STATUE_OFF;
                short tile = tilemaparray[glyph];
                tile2enlargement[tile] = mons[i].enlargement.statue;
            }


            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
            {
                if (mons[i].female_enlargement.actions[action])
                {
                    int glyph = i + get_monster_action_glyph_offset(action, 1);
                    short tile = tilemaparray[glyph];
                    tile2enlargement[tile] = mons[i].female_enlargement.actions[action];
                }
            }
            if (mons[i].female_enlargement.corpse)
            {
                int glyph = i + GLYPH_FEMALE_BODY_OFF;
                short tile = tilemaparray[glyph];
                tile2enlargement[tile] = mons[i].female_enlargement.corpse;
            }
            if (mons[i].female_enlargement.statue)
            {
                int glyph = i + GLYPH_FEMALE_STATUE_OFF;
                short tile = tilemaparray[glyph];
                tile2enlargement[tile] = mons[i].female_enlargement.statue;
            }
        }
        /* Objects */
        for (int i = 0; i < NUM_OBJECTS; i++)
        {
            if (obj_descr[i].enlargement)
            {
                int glyph = objnum_to_glyph(i);
                short tile = glyph2tile[glyph];
                tile2enlargement[tile] = obj_descr[i].enlargement;
            }
        }

        /* Artifacts */
        for (int i = 1; i <= NUM_ARTIFACTS; i++)
        {
            if (artilist[i].enlargement)
            {
                int glyph = i + GLYPH_ARTIFACT_OFF;
                short tile = glyph2tile[glyph];
                tile2enlargement[tile] = artilist[i].enlargement;
            }
        }

        /* CMAP */
        for (int cmap_type_idx = 0; cmap_type_idx < CMAP_TYPE_MAX; cmap_type_idx++)
        {
            for (int i = 0; i < MAX_CMAPPED_CHARS; i++)
            {
                if (defsyms[i].enlargement[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * CMAP_TYPE_CHAR_NUM + GLYPH_CMAP_OFF;
                    short tile = glyph2tile[glyph];
                    tile2enlargement[tile] = defsyms[i].enlargement[cmap_type_idx];
                }
                if (defsyms[i].broken_enlargement[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * CMAP_TYPE_CHAR_NUM + GLYPH_BROKEN_CMAP_OFF;
                    short tile = glyph2tile[glyph];
                    tile2enlargement[tile] = defsyms[i].broken_enlargement[cmap_type_idx];
                }
            }
        }

        /* CMAP Variation */
        for (int cmap_type_idx = 0; cmap_type_idx < CMAP_TYPE_MAX; cmap_type_idx++)
        {
            for (int i = 0; i < MAX_VARIATIONS; i++)
            {
                if (defsym_variations[i].enlargement[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * MAX_VARIATIONS + GLYPH_CMAP_VARIATION_OFF;
                    short tile = glyph2tile[glyph];
                    tile2enlargement[tile] = defsym_variations[i].enlargement[cmap_type_idx];
                }
                if (defsym_variations[i].broken_enlargement[cmap_type_idx])
                {
                    int glyph = i + cmap_type_idx * MAX_VARIATIONS + GLYPH_BROKEN_CMAP_VARIATION_OFF;
                    short tile = glyph2tile[glyph];
                    tile2enlargement[tile] = defsym_variations[i].broken_enlargement[cmap_type_idx];
                }
            }
        }

        /* Player */
        for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
        {
            for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
            {
                for (int gender = 0; gender <= 1; gender++)
                {
                    for (int alignment = -1; alignment <= 1; alignment++)
                    {
                        for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                        {
                            int player_glyph_index = player_to_glyph_index(roleidx, raceidx, gender, alignment + 1, level);
                            for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                            {
                                int enlargement_idx = get_player_enlargement(action, roleidx, raceidx, gender, alignment, level);
                                if (enlargement_idx)
                                {
                                    int player_glyph = player_glyph_index + get_player_action_glyph_offset(action);
                                    short tile = glyph2tile[player_glyph];
                                    tile2enlargement[tile] = enlargement_idx;
                                }
                            }
                        }
                    }
                }
            }
        }

        /* Cursors */
        for (int i = 0; i < MAX_CURSORS; i++)
        {
            if (game_cursors[i].enlargement)
            {
                int glyph = i + GLYPH_CURSOR_OFF;
                short tile = glyph2tile[glyph];
                tile2enlargement[tile] = game_cursors[i].enlargement;
            }
        }

        /* UI Tiles */
        for (int i = 0; i < MAX_UI_TILES; i++)
        {
            if (ui_tile_component_array[i].enlargement)
            {
                int glyph = i + GLYPH_UI_TILE_OFF;
                short tile = glyph2tile[glyph];
                tile2enlargement[tile] = ui_tile_component_array[i].enlargement;
            }
        }

        /* Replacements */
        for (int i = 1; i <= NUM_REPLACEMENTS; i++)
        {
            for (int j = 0; j < replacements[i].number_of_tiles; j++)
            {
                int enl_idx = replacements[i].tile_enlargement[j];
                if (enl_idx)
                {
                    int glyph = j + replacements[i].glyph_offset + GLYPH_REPLACEMENT_OFF;
                    short tile = glyph2tile[glyph];
                    tile2enlargement[tile] = enl_idx;
                }
            }
        }

        /* Animations */
        for (int i = 1; i <= NUM_ANIMATIONS; i++)
        {
            for (int j = 0; j < max(0, min(MAX_FRAMES_PER_ANIMATION, animations[i].number_of_frames)); j++)
            {
                char animation_tile_glyph_index = animations[i].frame2tile[j];
                if (animation_tile_glyph_index >= 0 && animations[i].tile_enlargement)
                {
                    int glyph = (int)animation_tile_glyph_index + animations[i].glyph_offset + GLYPH_ANIMATION_OFF;
                    short tile = glyph2tile[glyph];
                    tile2enlargement[tile] = animations[i].tile_enlargement;
                }
            }
        }
    }

    return tile_count;
}


/*tiledata.c*/
