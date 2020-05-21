/* GnollHack 4.0 tiledata.c */
/* Copyright (c) Janne Gustafsson 2020.                           */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "artifact.h"
#include "artilist.h"

#ifdef USE_TILES
short glyph2tile[MAX_GLYPH] = { 0 }; /* moved here from tile.c */
short tile2animation[MAX_GLYPH] = { 0 }; /* maximum of one tile per glyph */
short tile2enlargement[MAX_GLYPH] = { 0 }; /* maximum of one tile per glyph */
#endif

NEARDATA struct tileset_definition default_tileset_definition =
{
    2, 0, 0, 2, 0, 1, 2,
    0, 2, 2,
    2, 0, 1,
    1,
    {"dungeon-normal", (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0, (char*)0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    0, 0, 3
};

NEARDATA struct ui_component_definition ui_tile_component_array[MAX_UI_TILES] = {
    {"death",                   1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"hit",                     1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"hit-text-1",              1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"hit-text-2",              1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"hit-text-3",              1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"hit-text-4",              1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"hit-text-5",              1, 64, 96, {"whole", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"general-ui",              3, 16, 16, {"checkbox-unchecked", "checkbox-checked", "checkbox-count", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"status",                  14, 16, 16, {"petmark", "detectmark", "pilemark", "hungry",  "weak", "faint", "burdened", "stressed",  "strained", "overtaxed", "overloaded", "two-weapon",  "skill", "saddled", "", "",  "", "", "", "",  "", "", "", ""} },
    {"conditions",              min(24, BL_MASK_BITS), 16, 16, {"", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"main-window-borders",     6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"message-window-borders",  6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"status-window-borders",   6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"map-window-borders",      6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"menu-window-borders",     6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
    {"text-window-borders",     6, 32, 32, {"top-left", "top", "middle-left", "middle-center",  "bottom-left",  "bottom-center", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", "",  "", "", "", ""} },
};

void
init_tiledata()
{
    /* fill out condition names*/
    for (int i = 0; i < BL_MASK_BITS; i++)
    {
        unsigned long bit = 1 << i;
        const char* cond_name = get_condition_name(bit);
        ui_tile_component_array[CONDITION_MARKS].component_names[i] = cond_name;
    }

}

int
process_tiledata(tsd, process_style, save_file_name, tilemaparray) /* Save tile data / read tile data / count tiles */
struct tileset_definition* tsd;
int process_style;  /* 0 = save data to file, 1 = read data from file, 2 = count tiles */
const char* save_file_name;
short* tilemaparray;
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
    const char* monster_set_name_array[7] = { "normal", "pet", "detect", "body", "ridden", "attack", "statue" };

    for (int gender = 0; gender <= 1; gender++)
    {
        if (gender == 1)
        {
            if (tsd->female_tile_style == 0)
                continue;
        }
        const char* gender_name = (gender == 0 ? "base" : "female");

        for (int spset = 0; spset < 7; spset++)
        {
            if (spset == 1 && !tsd->has_pet_tiles)
                continue;
            if (spset == 2 && !tsd->has_detect_tiles)
                continue;
            if (spset == 3 && !tsd->body_tile_style)
                continue;
            if (spset == 4 && !tsd->ridden_tile_style)
                continue;
            if (spset == 5 && !tsd->attack_tile_style)
                continue;
            if (spset == 6 && !tsd->has_statue_tiles)
                continue;

            set_name = monster_set_name_array[spset];
            for (int i = LOW_PM; i < NUM_MONSTERS; i++)
            {
                if (gender == 1)
                {
                    if (tsd->female_tile_style == 2 && !(mons[i].mflags5 & M5_FEMALE_TILE))
                        continue;
                }

                if (spset == 3)
                {
                    if (tsd->body_tile_style == 2 && !(mons[i].mflags5 & M5_CORPSE_TILE))
                        continue;
                }

                if (spset == 4)
                {
                    if (tsd->ridden_tile_style == 2 && !(mons[i].mflags1 & M1_STEED))
                        continue;
                }

                if (spset == 5)
                {
                    if (tsd->attack_tile_style == 2 && !(mons[i].mflags5 & M5_ATTACK_TILE))
                        continue;
                }

                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,%s,%s", tile_section_name, gender_name, set_name, mons[i].mname);
                    if(gender == 0 && mons[i].enlargement > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[mons[i].enlargement].width_in_tiles, enlargements[mons[i].enlargement].height_in_tiles, enlargements[mons[i].enlargement].main_tile_x_coordinate);
                    else if (gender == 1 && mons[i].female_enlargement > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[mons[i].female_enlargement].width_in_tiles, enlargements[mons[i].female_enlargement].height_in_tiles, enlargements[mons[i].female_enlargement].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    int glyph_offset = (spset == 0 ? GLYPH_MON_OFF : spset == 1 ? GLYPH_PET_OFF : spset == 2 ? GLYPH_DETECT_OFF :
                        spset == 3 ? GLYPH_BODY_OFF : spset == 4 ? GLYPH_RIDDEN_OFF : spset == 5 ? GLYPH_ATTACK_OFF : GLYPH_STATUE_OFF);
                    int female_glyph_offset = (spset == 0 ? GLYPH_FEMALE_MON_OFF : spset == 1 ? GLYPH_FEMALE_PET_OFF : spset == 2 ? GLYPH_FEMALE_DETECT_OFF :
                        spset == 3 ? GLYPH_FEMALE_BODY_OFF : spset == 4 ? GLYPH_FEMALE_RIDDEN_OFF : spset == 5 ? GLYPH_FEMALE_ATTACK_OFF : GLYPH_FEMALE_STATUE_OFF);

                    if (gender == 0)
                    {
                        tilemaparray[i + glyph_offset] = tile_count;
                        if (spset == 0)
                        {
                            if (!tsd->has_pet_tiles)
                                tilemaparray[i + GLYPH_PET_OFF] = tile_count;
                            if (!tsd->has_detect_tiles)
                                tilemaparray[i + GLYPH_DETECT_OFF] = tile_count;
                            if (tsd->ridden_tile_style != 1)
                                tilemaparray[i + GLYPH_RIDDEN_OFF] = tile_count;
                            if (tsd->attack_tile_style != 1)
                                tilemaparray[i + GLYPH_ATTACK_OFF] = tile_count;
                        }
                    }

                    /* write female versions twice just in case, first as base monster, and then override as female, if we get here */
                    tilemaparray[i + female_glyph_offset] = tile_count;
                    if (spset == 0)
                    {
                        if (!tsd->has_pet_tiles)
                            tilemaparray[i + GLYPH_FEMALE_PET_OFF] = tile_count;
                        if (!tsd->has_detect_tiles)
                            tilemaparray[i + GLYPH_FEMALE_DETECT_OFF] = tile_count;
                        if (tsd->ridden_tile_style != 1)
                            tilemaparray[i + GLYPH_FEMALE_RIDDEN_OFF] = tile_count;
                        if (tsd->attack_tile_style != 1)
                            tilemaparray[i + GLYPH_FEMALE_ATTACK_OFF] = tile_count;
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
    for (int j = 0; j <= 4; j++)
    {
        /* j == 0 -> Normal objs */
        /* j == 1 -> Inventory objs */
        /* j == 2 -> Lit objs */
        /* j == 3 -> Lit inventory objs */
        /* j == 4 -> Missile objs */

        if (j == 1 && !tsd->inventory_tile_style)
            continue;
        if (j == 2 && !tsd->lit_tile_style)
            continue;
        if (j == 3 && (!tsd->lit_tile_style || !tsd->inventory_tile_style))
            continue;
        if (j == 4 && !tsd->missile_tile_style)
            continue;

        boolean first_scroll_found = FALSE;
        int nameless_idx = 0;
        set_name = (j == 0 ? "normal" : j == 1 ? "inventory" : j == 2 ? "lit" : j == 3 ? "inventory lit" : "missile");
        glyph_offset =
            (j == 0 ? GLYPH_OBJ_OFF :
                j == 1 ? GLYPH_OBJ_INVENTORY_OFF :
                j == 2 ? GLYPH_OBJ_LIT_OFF :
                j == 3 ? GLYPH_OBJ_INVENTORY_LIT_OFF :
                GLYPH_OBJ_MISSILE_OFF);

        for (int i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
        {
            int missile_tile_num = (objects[i].oc_flags4 & O4_SINGLE_MISSILE_TILE) ? 1 : NUM_MISSILE_DIRS;

            if (j == 1)
            {
                if (tsd->inventory_tile_style == 2 && !(objects[i].oc_flags4 & O4_INVENTORY_TILE))
                    continue;
            }
            else if (j == 2)
            {
                if (tsd->lit_tile_style == 2 && !(objects[i].oc_flags4 & O4_LIT_TILE))
                    continue;
            }
            else if (j == 3)
            {
                if (tsd->inventory_tile_style == 2 && !(objects[i].oc_flags4 & O4_INVENTORY_TILE))
                    continue;
                if (tsd->lit_tile_style == 2 && !(objects[i].oc_flags4 & O4_LIT_TILE))
                    continue;
            }
            else if (j == 4)
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

                /* First, generic scroll tile */
                if (process_style == 0)
                {
                    if (j == 4)
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
                        else if (j == 2 && obj_descr[i].lit_enlargement > 0)
                            Sprintf(eos(buf), ",%d,%d,%d", enlargements[obj_descr[i].lit_enlargement].width_in_tiles, enlargements[obj_descr[i].lit_enlargement].height_in_tiles, enlargements[obj_descr[i].lit_enlargement].main_tile_x_coordinate);
                        else
                            Sprintf(eos(buf), ",1,1,0");
                        Sprintf(eos(buf), "\n");
                        (void)write(fd, buf, strlen(buf));
                        tile_count++;
                    }
                }
                else if (process_style == 1)
                {
                    if (j == 4)
                    {
                        for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                        {
                            /* Found scroll */
                            tilemaparray[i * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
                            
                            /* Add the tile to all scrolls */
                            for (int m = STRANGE_OBJECT; m < NUM_OBJECTS; m++)
                            {
                                if (objects[m].oc_class == SCROLL_CLASS)
                                {
                                    tilemaparray[m * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
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

                        /* Add the tile to all scrolls */
                        for (int m = STRANGE_OBJECT; m < NUM_OBJECTS; m++)
                        {
                            if (objects[m].oc_class == SCROLL_CLASS)
                            {
                                /* Other scroll's main tile */
                                tilemaparray[m + glyph_offset] = tile_count;

                                /* Add others if they do not have their own */
                                if (j == 0)
                                {
                                    if (tsd->inventory_tile_style != 1)
                                    {
                                        int glyph_offset3 = GLYPH_OBJ_INVENTORY_OFF;
                                        tilemaparray[m + glyph_offset3] = tile_count;
                                        if (tsd->lit_tile_style != 1)
                                        {
                                            int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                            tilemaparray[m + glyph_offset4] = tile_count;
                                        }
                                    }
                                    if (tsd->lit_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_LIT_OFF;
                                        tilemaparray[m + glyph_offset4] = tile_count;
                                    }
                                    if (tsd->missile_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_MISSILE_OFF;
                                        for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                                        {
                                            tilemaparray[m * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                                        }
                                    }
                                }
                                else if (j == 1)
                                {
                                    if (tsd->lit_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                        tilemaparray[m + glyph_offset4] = tile_count;
                                    }
                                }
                                else if (j == 2)
                                {
                                    if (tsd->inventory_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                        tilemaparray[m + glyph_offset4] = tile_count;
                                    }
                                }
                            }
                        }

                        tile_count++;
                    }
                }
                else
                {
                    if (j == 4)
                        tile_count += missile_tile_num;
                    else
                        tile_count++;
                }

                /* Second, scroll of mail */
                if (process_style == 0)
                {
                    if (j == 4)
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
                    if (j == 4)
                    {
                        for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                        {
#ifdef MAIL
                            tilemaparray[SCR_MAIL * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;
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
                        /* Main tile */
                        tilemaparray[SCR_MAIL + glyph_offset] = tile_count;

                        /* Add to others, if they have not tiles of their own */
                        if (j == 0)
                        {
                            if (tsd->inventory_tile_style != 1)
                            {
                                int glyph_offset3 = GLYPH_OBJ_INVENTORY_OFF;
                                tilemaparray[SCR_MAIL + glyph_offset3] = tile_count;
                                if (tsd->lit_tile_style != 1)
                                {
                                    int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                    tilemaparray[SCR_MAIL + glyph_offset4] = tile_count;
                                }
                            }
                            if (tsd->lit_tile_style != 1)
                            {
                                int glyph_offset4 = GLYPH_OBJ_LIT_OFF;
                                tilemaparray[SCR_MAIL + glyph_offset4] = tile_count;
                            }
                            if (tsd->missile_tile_style != 1)
                            {
                                int glyph_offset4 = GLYPH_OBJ_MISSILE_OFF;
                                for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                                {
                                    tilemaparray[SCR_MAIL * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                                }
                            }
                        }
                        else if (j == 1)
                        {
                            if (tsd->lit_tile_style != 1)
                            {
                                int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                tilemaparray[SCR_MAIL + glyph_offset4] = tile_count;
                            }
                        }
                        else if (j == 2)
                        {
                            if (tsd->inventory_tile_style != 1)
                            {
                                int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                tilemaparray[SCR_MAIL + glyph_offset4] = tile_count;
                            }
                        }

#endif
                        tile_count++;
                    }
                }
                else
                {
                    if (j == 4)
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

            if (process_style == 0)
            {
                if (j == 4)
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
                    else if (j == 2 && obj_descr[i].lit_enlargement > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[obj_descr[i].lit_enlargement].width_in_tiles, enlargements[obj_descr[i].lit_enlargement].height_in_tiles, enlargements[obj_descr[i].lit_enlargement].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                    tile_count++;
                }
            }
            else if (process_style == 1)
            {
                if (j == 4)
                {
                    for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                    {
                        /* Write to the tile to the main glyph */
                        tilemaparray[i * NUM_MISSILE_DIRS + n + glyph_offset] = tile_count;

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

                    /* Write to the tile to the inventory, lit, and inventory lit glyphs if they do not have their own */
                    if (j == 0)
                    {
                        if (tsd->inventory_tile_style != 1)
                        {
                            int glyph_offset3 = GLYPH_OBJ_INVENTORY_OFF;
                            tilemaparray[i + glyph_offset3] = tile_count;
                            if (tsd->lit_tile_style != 1)
                            {
                                int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                tilemaparray[i + glyph_offset4] = tile_count;
                            }
                        }
                        if (tsd->lit_tile_style != 1)
                        {
                            int glyph_offset4 = GLYPH_OBJ_LIT_OFF;
                            tilemaparray[i + glyph_offset4] = tile_count;
                        }
                        if (tsd->missile_tile_style != 1)
                        {
                            int glyph_offset4 = GLYPH_OBJ_MISSILE_OFF;
                            for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                            {
                                tilemaparray[i * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                            }
                        }
                    }
                    else if (j == 1)
                    {
                        if (tsd->lit_tile_style != 1)
                        {
                            int glyph_offset3 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                            tilemaparray[i + glyph_offset3] = tile_count;
                        }
                    }
                    else if (j == 2)
                    {
                        if (tsd->inventory_tile_style != 1)
                        {
                            int glyph_offset3 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                            tilemaparray[i + glyph_offset3] = tile_count;
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

                                /* Write to the tile to the inventory, lit, and inventory lit glyphs if they do not have their own */
                                if (j == 0)
                                {
                                    if (tsd->inventory_tile_style != 1)
                                    {
                                        int glyph_offset3 = GLYPH_OBJ_INVENTORY_OFF;
                                        tilemaparray[m + glyph_offset3] = tile_count;
                                        if (tsd->lit_tile_style != 1)
                                        {
                                            int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                            tilemaparray[m + glyph_offset4] = tile_count;
                                        }
                                    }
                                    if (tsd->lit_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_LIT_OFF;
                                        tilemaparray[m + glyph_offset4] = tile_count;
                                    }
                                    if (tsd->missile_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_MISSILE_OFF;
                                        for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                                        {
                                            tilemaparray[m * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                                        }
                                    }
                                }
                                else if (j == 1)
                                {
                                    if (tsd->lit_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                        tilemaparray[m + glyph_offset4] = tile_count;
                                    }
                                }
                                else if (j == 2)
                                {
                                    if (tsd->inventory_tile_style != 1)
                                    {
                                        int glyph_offset4 = GLYPH_OBJ_INVENTORY_LIT_OFF;
                                        tilemaparray[m + glyph_offset4] = tile_count;
                                    }
                                }
                            }
                        }
                    }

                    /* Write generic corpse and statue tiles */
                    if (j == 0 &&
                        (tsd->body_tile_style != 1 && i == CORPSE)
                        || (!tsd->has_statue_tiles && i == STATUE)
                        )
                    {
                        for (int gender = 0; gender <= 1; gender++)
                        {
                            for (int k = LOW_PM; k < NUM_MONSTERS; k++)
                            {
                                if (i == CORPSE && tsd->body_tile_style == 2 && (mons[k].mflags5 & M5_CORPSE_TILE))
                                    continue;

                                int glyph_offset2 = (gender == 0 ? (i == CORPSE ? GLYPH_BODY_OFF : GLYPH_STATUE_OFF) : (i == CORPSE ? GLYPH_FEMALE_BODY_OFF : GLYPH_FEMALE_STATUE_OFF));
                                tilemaparray[k + glyph_offset2] = tile_count;
                            }
                        }
                    }


                    tile_count++;
                }
            }
            else
            {
                if (j == 4)
                    tile_count += missile_tile_num;
                else
                    tile_count++;
            }
        }
    }

    /* Artifact tiles */
    tile_section_name = "artifacts";
    for (int j = 0; j <= 4; j++)
    {
        if (j == 1 && !tsd->inventory_tile_style)
            continue;
        if (j == 2 && !tsd->lit_tile_style)
            continue;
        if (j == 3 && (!tsd->lit_tile_style || !tsd->inventory_tile_style))
            continue;
        if (j == 4 && !tsd->missile_tile_style)
            continue;

        set_name = (j == 0 ? "normal" : j == 1 ? "inventory" : j == 2 ? "lit" : j == 3 ? "inventory lit" : "missile");
        glyph_offset = (j == 0 ? GLYPH_ARTIFACT_OFF : 
            j == 1 ? GLYPH_ARTIFACT_INVENTORY_OFF :
            j == 2 ? GLYPH_ARTIFACT_LIT_OFF :
            j == 3 ? GLYPH_ARTIFACT_INVENTORY_LIT_OFF :
            GLYPH_ARTIFACT_MISSILE_OFF);

        for (int i = 1; i <= NUM_ARTIFACTS; i++)
        {
            int missile_tile_num = (artilist[i].aflags2 & AF2_SINGLE_MISSILE_TILE) ? 1 : NUM_MISSILE_DIRS;

            if (j == 1)
            {
                if (tsd->inventory_tile_style == 2 && !(artilist[i].aflags2 & AF2_INVENTORY_TILE))
                    continue;
            }
            else if (j == 2)
            {
                if (tsd->lit_tile_style == 2 && !(artilist[i].aflags2 & AF2_LIT_TILE))
                    continue;
            }
            else if (j == 3)
            {
                if (tsd->inventory_tile_style == 2 && !(artilist[i].aflags2 & AF2_INVENTORY_TILE))
                    continue;
                if (tsd->lit_tile_style == 2 && !(artilist[i].aflags2 & AF2_LIT_TILE))
                    continue;
            }
            else if (j == 4)
            {
                if (tsd->missile_tile_style == 2 && !(artilist[i].aflags2 & AF2_MISSILE_TILE))
                    continue;
            }


            int base_item = artilist[i].otyp;
            boolean no_description = !artilist[i].desc;
            boolean no_base_item_name = !OBJ_NAME(objects[base_item]);
            boolean no_base_item_description = !obj_descr[objects[base_item].oc_name_idx].oc_descr;
            if (process_style == 0)
            {
                if (j == 4)
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
                    else if (j == 2 && artilist[i].lit_enlargement > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[artilist[i].lit_enlargement].width_in_tiles, enlargements[artilist[i].lit_enlargement].height_in_tiles, enlargements[artilist[i].lit_enlargement].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                    tile_count++;

                }
            }
            else if (process_style == 1)
            {
                if (j == 4)
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
                    if (j == 0)
                    {
                        if (tsd->inventory_tile_style != 1)
                        {
                            int glyph_offset3 = GLYPH_ARTIFACT_INVENTORY_OFF;
                            tilemaparray[(i - 1) + glyph_offset3] = tile_count;
                            if (tsd->lit_tile_style != 1)
                            {
                                int glyph_offset4 = GLYPH_ARTIFACT_INVENTORY_LIT_OFF;
                                tilemaparray[(i - 1) + glyph_offset4] = tile_count;
                            }
                        }
                        if (tsd->lit_tile_style != 1)
                        {
                            int glyph_offset4 = GLYPH_ARTIFACT_LIT_OFF;
                            tilemaparray[(i - 1) + glyph_offset4] = tile_count;
                        }
                        if (tsd->missile_tile_style != 1)
                        {
                            int glyph_offset4 = GLYPH_ARTIFACT_MISSILE_OFF;
                            for (int n = 0; n < NUM_MISSILE_DIRS; n++)
                            {
                                tilemaparray[(i - 1) * NUM_MISSILE_DIRS + n + glyph_offset4] = tile_count;
                            }
                        }
                    }
                    else if (j == 1)
                    {
                        if (tsd->lit_tile_style != 1)
                        {
                            int glyph_offset3 = GLYPH_ARTIFACT_INVENTORY_LIT_OFF;
                            tilemaparray[(i - 1) + glyph_offset3] = tile_count;
                        }
                    }
                    else if (j == 2)
                    {
                        if (tsd->inventory_tile_style != 1)
                        {
                            int glyph_offset3 = GLYPH_ARTIFACT_INVENTORY_LIT_OFF;
                            tilemaparray[(i - 1) + glyph_offset3] = tile_count;
                        }
                    }
                    tilemaparray[(i - 1) + glyph_offset] = tile_count;
                    tile_count++;
                }
            }
            else
            {
                if (j == 4)
                    tile_count += missile_tile_num;
                else
                    tile_count++;
            }
        }
    }

    /* CMAP tiles */
    tile_section_name = "cmap";
    int bottom_end_variations[NUM_BOTTOM_ENDS] = { S_vwall, S_tlcorn, S_trcorn, S_crwall, S_tdwall, S_tlwall, S_trwall };
    int num_cmaps = (tsd->has_full_cmap_set ? CMAP_TYPE_MAX : max(1, tsd->number_of_cmaps));

    for (int cmap_idx = 0; cmap_idx < num_cmaps; cmap_idx++)
    {
        char namebuf[BUFSZ];
        if (tsd->cmap_names[cmap_idx] && strcmp(tsd->cmap_names[cmap_idx], ""))
            Sprintf(namebuf, "%s", tsd->cmap_names[cmap_idx]);
        else
            Sprintf(namebuf, "unnamed-cmap-%d", cmap_idx);

        for (int i = 0; i < CMAP_TYPE_CHAR_NUM; i++)
        {
            if (cmap_idx > 0)
            {
                if (tsd->cmap_limitation_style[cmap_idx] == 1 && (i < S_vwall || i > S_trwall))
                    continue;
                if (tsd->cmap_limitation_style[cmap_idx] == 2 && (i < S_stone || i > S_dnladder))
                    continue;
            }

            if (tsd->cmap_wall_style[cmap_idx] == 1 && (i < S_vwall || i > S_hwall))
                continue;

            if (process_style == 0)
            {
                Sprintf(buf, "%s,%s,%s,%s", tile_section_name, tsd->has_full_cmap_set ? cmap_type_names[cmap_idx] : namebuf, get_cmap_tilename(i),
                    (defsyms[i].explanation && strcmp(defsyms[i].explanation, "")) ? defsyms[i].explanation : "no description");
                if (defsyms[i].enlargement[cmap_idx] > 0)
                    Sprintf(eos(buf), ",%d,%d,%d", enlargements[defsyms[i].enlargement[cmap_idx]].width_in_tiles, enlargements[defsyms[i].enlargement[cmap_idx]].height_in_tiles, enlargements[defsyms[i].enlargement[cmap_idx]].main_tile_x_coordinate);
                else
                    Sprintf(eos(buf), ",1,1,0");
                Sprintf(eos(buf), "\n");
                (void)write(fd, buf, strlen(buf));
            }
            else if (process_style == 1)
            {
                if (tsd->has_full_cmap_set)
                {
                    glyph_offset = GLYPH_CMAP_OFF + cmap_idx * CMAP_TYPE_CHAR_NUM;
                    tilemaparray[i + glyph_offset] = tile_count;

                    /* Copy 2 simple wall tile to all other relevant tiles */
                    if (tsd->cmap_wall_style[cmap_idx] == 1 )
                    {
                        if (i == S_vwall)
                        {
                            for (int j = S_tlcorn; j <= S_trwall; j++)
                            {
                                tilemaparray[j + glyph_offset] = tile_count;
                            }
                        }
                        else if (i == S_hwall)
                        {
                            int glyph_offset2 = GLYPH_CMAP_VARIATION_OFF + cmap_idx * MAX_VARIATIONS;
                            for (int j = 0; j < NUM_BOTTOM_ENDS; j++)
                            {
                                int variation_offset = defsyms[bottom_end_variations[j]].variation_offset;
                                tilemaparray[GWALL_BOTTOM_END + variation_offset + glyph_offset2] = tile_count;
                            }
                        }
                    }

                    if (!tsd->has_variations && defsyms[i].variations > 0)
                    {
                        for (int m = 0; m < defsyms[i].variations; m++)
                        {
                            glyph_offset = GLYPH_CMAP_VARIATION_OFF + cmap_idx * MAX_VARIATIONS;
                            tilemaparray[m + defsyms[i].variation_offset+ glyph_offset] = tile_count;
                        }
                    }
                }
                else
                {
                    /* Go through all internal cmaps */
                    for (int k = 0; k < CMAP_TYPE_MAX; k++)
                    {
                        /* Write this cmap_idx for all internal CMAPs it is used for */
                        if (tsd->cmap_mapping[k] == cmap_idx)
                        {
                            glyph_offset = GLYPH_CMAP_OFF + k * CMAP_TYPE_CHAR_NUM;
                            tilemaparray[i + glyph_offset] = tile_count;

                            /* Copy 2 simple wall tile to all other relevant tiles */
                            if (tsd->cmap_wall_style[k] == 1)
                            {
                                if (i == S_vwall)
                                {
                                    for (int j = S_tlcorn; j <= S_trwall; j++)
                                    {
                                        tilemaparray[j + glyph_offset] = tile_count;
                                    }
                                }
                                else if (i == S_hwall)
                                {
                                    int glyph_offset2 = GLYPH_CMAP_VARIATION_OFF + cmap_idx * MAX_VARIATIONS;
                                    for (int j = 0; j < NUM_BOTTOM_ENDS; j++)
                                    {
                                        int variation_offset = defsyms[bottom_end_variations[j]].variation_offset;
                                        tilemaparray[GWALL_BOTTOM_END + variation_offset + glyph_offset2] = tile_count;
                                    }
                                }
                            }

                            if (!tsd->has_variations && defsyms[i].variations > 0)
                            {
                                for (int m = 0; m < defsyms[i].variations; m++)
                                {
                                    glyph_offset = GLYPH_CMAP_VARIATION_OFF + cmap_idx * MAX_VARIATIONS;
                                    tilemaparray[m + defsyms[i].variation_offset + glyph_offset] = tile_count;
                                }
                            }
                        }
                    }
                }

                if (cmap_idx == 0 && num_cmaps > 1)
                {
                    /* copy an out-limited tiles to all limited cmaps */
                    for (int k = 1; k < num_cmaps; k++)
                    {
                        if (
                            (tsd->cmap_limitation_style[k] == 1 && (i < S_vwall || i > S_trwall))
                            || (tsd->cmap_limitation_style[k] == 2 && (i < S_stone || i > S_dnladder))
                            )
                        {
                            int glyph_offset2 = GLYPH_CMAP_OFF + k * CMAP_TYPE_CHAR_NUM;
                            tilemaparray[i + glyph_offset2] = tile_count;
                            if (!tsd->has_variations && defsyms[i].variations > 0)
                            {
                                for (int m = 0; m < defsyms[i].variations; m++)
                                {
                                    glyph_offset = GLYPH_CMAP_VARIATION_OFF + cmap_idx * MAX_VARIATIONS;
                                    tilemaparray[m + defsyms[i].variation_offset + glyph_offset] = tile_count;
                                }
                            }
                        }
                    }
                }
            }
            tile_count++;
        }
    }

    if (tsd->has_variations)
    {
        /* CMAP variation tiles */
        tile_section_name = "cmap-variations";
        int num_cmaps = (tsd->has_full_cmap_set ? CMAP_TYPE_MAX : max(1, tsd->number_of_cmaps));
        for (int cmap_idx = 0; cmap_idx < num_cmaps; cmap_idx++)
        {
            char namebuf[BUFSZ];
            if (tsd->cmap_names[cmap_idx] && strcmp(tsd->cmap_names[cmap_idx], ""))
                Sprintf(namebuf, "%s", tsd->cmap_names[cmap_idx]);
            else
                Sprintf(namebuf, "unnamed-cmap-%d", cmap_idx);

            for (int i = 0; i < MAX_VARIATIONS; i++)
            {
                if (cmap_idx > 0)
                {
                    if (tsd->cmap_limitation_style[cmap_idx] == 1 && !is_wall_variation(i))
                        continue;
                    if (tsd->cmap_limitation_style[cmap_idx] == 2 && !is_base_cmap_variation(i))
                        continue;
                }

                /* Already read in CMAP */
                if (tsd->cmap_wall_style[cmap_idx] == 1 && is_bottom_end_variation(i))
                    continue;

                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,%s", tile_section_name, tsd->has_full_cmap_set ? cmap_type_names[cmap_idx] : namebuf,
                        (defsym_variations[i].explanation && strcmp(defsym_variations[i].explanation, "")) ? defsym_variations[i].explanation : "no description");
                    if (defsym_variations[i].enlargement[cmap_idx] > 0)
                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[defsym_variations[i].enlargement[cmap_idx]].width_in_tiles, enlargements[defsym_variations[i].enlargement[cmap_idx]].height_in_tiles, enlargements[defsym_variations[i].enlargement[cmap_idx]].main_tile_x_coordinate);
                    else
                        Sprintf(eos(buf), ",1,1,0");
                    Sprintf(eos(buf), "\n");
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    if (tsd->has_full_cmap_set)
                    {
                        glyph_offset = GLYPH_CMAP_VARIATION_OFF + cmap_idx * MAX_VARIATIONS;
                        tilemaparray[i + glyph_offset] = tile_count;
                    }
                    else
                    {
                        /* Go through all internal cmaps */
                        for (int k = 0; k < CMAP_TYPE_MAX; k++)
                        {
                            /* Write this cmap_idx for all internal CMAPs it is used for */
                            if (tsd->cmap_mapping[k] == cmap_idx)
                            {
                                glyph_offset = GLYPH_CMAP_VARIATION_OFF + k * MAX_VARIATIONS;
                                tilemaparray[i + glyph_offset] = tile_count;
                            }
                        }
                    }

                    
                    if (cmap_idx == 0 && num_cmaps > 1)
                    {
                        /* copy an out-limited tile to all limited cmaps */
                        for (int k = 1; k < num_cmaps; k++)
                        {
                            if (
                                (tsd->cmap_limitation_style[k] == 1 && !is_wall_variation(i))
                                || (tsd->cmap_limitation_style[k] == 2 && !is_base_cmap_variation(i))
                                )
                            {
                                int glyph_offset2 = GLYPH_CMAP_VARIATION_OFF + k * MAX_VARIATIONS;
                                tilemaparray[i + glyph_offset2] = tile_count;
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

                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s,1,1,0\n", tile_section_name, set_name, explosion_direction_name);
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
                    const char* explosion_name = explosion_type_names[j];
                    for (int i = 0; i < MAX_EXPLOSION_CHARS; i++)
                    {
                        const char* explosion_direction_name = explosion_direction_name_array[i];
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s,1,1,0\n", tile_section_name, set_name, explosion_name, explosion_direction_name);
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
                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s,1,1,0\n", tile_section_name, set_name, zap_direction_name);
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
                    const char* zap_name_array[NUM_ZAP] = {
                        "magic", "fire", "frost", "sleep", "disintegration",
                        "lightning", "poison", "acid", "death",
                        "petrification" };
                    const char* zap_name = zap_name_array[j];

                    for (int i = 0; i < MAX_ZAP_CHARS; i++)
                    {
                        const char* zap_direction_name = zap_direction_name_array[i];
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s,1,1,0\n", tile_section_name, set_name, zap_name, zap_direction_name);
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
                    "middle-left", "middle-right",
                    "bottom-left", "bottom-center", "bottom-right" };

            if (tsd->swallow_tile_style == 0)
            {
                for (int i = 0; i < MAX_SWALLOW_CHARS; i++)
                {
                    const char* swallow_direction_name = swallow_direction_name_array[i];
                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s,1,1,0\n", tile_section_name, set_name, swallow_direction_name);
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
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s,1,1,0\n", tile_section_name, set_name, mons[j].mname, swallow_direction_name);
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
    const char* player_set_name_array[2] = { "normal", "attack" };
    for (int spset = 0; spset < 2; spset++)
    {
        set_name = player_set_name_array[spset];

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
                                    int player_glyph = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                    tilemaparray[player_glyph] = tilemaparray[role_as_monster + (spset == 0 ? ((gender == 0) ? GLYPH_MON_OFF : GLYPH_FEMALE_MON_OFF) : ((gender == 0) ? GLYPH_ATTACK_OFF : GLYPH_FEMALE_ATTACK_OFF))];

                                    if (spset == 0 && !player_has_attack_tile(roleidx, raceidx, gender, alignment + 1, level))
                                    {
                                        int player_attack_glyph = player_to_attack_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                        tilemaparray[player_attack_glyph] = tile_count;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (tsd->player_tile_style == 1)
        {
            if (process_style == 0)
            {
                if (spset == 1 && !GENERIC_PLAYER_HAS_ATTACK_TILE)
                    continue;

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
                                    int player_glyph = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                    tilemaparray[player_glyph] = tile_count;

                                    if (spset == 0 && !player_has_attack_tile(roleidx, raceidx, gender, alignment + 1, level))
                                    {
                                        int player_attack_glyph = player_to_attack_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                        tilemaparray[player_attack_glyph] = tile_count;
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
                                if (process_style == 0)
                                {
                                    if (spset == 1 && !player_has_attack_tile(roleidx, raceidx, gender, alignment + 1, level))
                                        continue;

                                    Sprintf(buf, "%s,%s,%s,%s,%s,%s,level-%d", tile_section_name, set_name, role_name, race_name, gender_name, align_name, level);
                                    int pl_enl = get_player_enlargement(roleidx, raceidx, gender, alignment + 1, level);
                                    if (pl_enl > 0)
                                        Sprintf(eos(buf), ",%d,%d,%d", enlargements[pl_enl].width_in_tiles, enlargements[pl_enl].height_in_tiles, enlargements[pl_enl].main_tile_x_coordinate);
                                    else
                                        Sprintf(eos(buf), ",1,1,0");
                                    Sprintf(eos(buf), "\n");
                                    (void)write(fd, buf, strlen(buf));
                                }
                                else if (process_style == 1)
                                {
                                    int player_glyph = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                    tilemaparray[player_glyph] = tile_count;

                                    if (spset == 0 && !player_has_attack_tile(roleidx, raceidx, gender, alignment + 1, level))
                                    {
                                        int player_attack_glyph = player_to_attack_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                        tilemaparray[player_attack_glyph] = tile_count;
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
                                if (process_style == 0)
                                {
                                    if (spset == 1 && !player_has_attack_tile(roleidx, raceidx, gender, alignment + 1, level))
                                        continue;

                                    Sprintf(buf, "%s,%s,%s,%s,%s,%s,level-%d", tile_section_name, set_name, role_name, race_name, gender_name, align_name, level);
                                    int pl_enl = get_player_enlargement(roleidx, raceidx, gender, alignment + 1, level);
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
                                            int player_glyph = player_to_glyph(roleidx, raceidx, gender, k, level);
                                            tilemaparray[player_glyph] = tile_count;

                                            if (spset == 0 && !player_has_attack_tile(roleidx, raceidx, gender, alignment + 1, level))
                                            {
                                                int player_attack_glyph = player_to_attack_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                                tilemaparray[player_attack_glyph] = tile_count;
                                            }

                                        }
                                    }
                                    else
                                    {
                                        int player_glyph = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                        tilemaparray[player_glyph] = tile_count;

                                        if (spset == 0 && !player_has_attack_tile(roleidx, raceidx, gender, alignment + 1, level))
                                        {
                                            int player_attack_glyph = player_to_attack_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                            tilemaparray[player_attack_glyph] = tile_count;
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
    const char* cursor_name_array[MAX_CURSORS] = {
        "generic", "look", "teleport",
        "jump", "polearm", "spell", "pay" };

    for (int i = 0; i < MAX_CURSORS; i++)
    {
        if (process_style == 0)
        {
            Sprintf(buf, "%s,%s,%s,1,1,0\n", tile_section_name, set_name, cursor_name_array[i]);
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

    /* Animation tiles */
    tile_section_name = "animation";
    for (int i = 1; i <= NUM_ANIMATIONS; i++)  /* animation number, starts at 1 */
    {
        for (int j = 0; j < max(0, min(animations[i].number_of_tiles, MAX_TILES_PER_ANIMATION)); j++) /* tile number */
        {
            if (process_style == 0)
            {
                Sprintf(buf, "%s,%s,tile-%d,%d", tile_section_name, 
                    animations[i].animation_name ? animations[i].animation_name : "unknown animation",
                    j,
                    glyph2tile[animations[i].base_glyph_id]
                );
                int enl = animations[i].tile_enlargement[j];
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
                for (int k = 0; k < MAX_FRAMES_PER_ANIMATION; k++)  /* frame number */
                {
                    if(animations[i].frame2tile[k] == j)
                        tilemaparray[k + animations[i].glyph_offset + GLYPH_ANIMATION_OFF] = tile_count;
                }
            }
            tile_count++;
        }
    }


    /* Enlargement tiles */
    tile_section_name = "enlargement";
    for (int i = 1; i <= NUM_ENLARGEMENTS; i++)  /* enlargement number, starts at 1 */
    {
        for (int j = 0; j < max(0, min(MAX_TILES_PER_ENLARGEMENT, enlargements[i].number_of_tiles)); j++) /* tile number */
        {
            const char* pos_name = "unknown";
            const char* position_names[MAX_TILES_PER_ENLARGEMENT] = { "top-left", "top", "top-right", "left", "right" };

            int position = -1;
            for (int k = 0; k < MAX_ENLARGEMENT_FRAMES; k++) /* frame number */
            {
                if (enlargements[i].frame2tile[k] == j)
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
                Sprintf(buf, "%s,%s,%s,%d,%d,%d,%d,%d,%d\n", tile_section_name, 
                    enlargements[i].enlargement_name ? enlargements[i].enlargement_name : "unknown enlargement", 
                    pos_name,
                    glyph2tile[enlargements[i].base_glyph_id], 
                    enlargements[i].width_in_tiles,
                    enlargements[i].height_in_tiles,
                    enlargements[i].main_tile_x_coordinate,
                    enlargements[i].number_of_tiles,
                    position
                    );
                (void)write(fd, buf, strlen(buf));
            }
            else if (process_style == 1)
            {
                glyph_offset = GLYPH_ENLARGEMENT_OFF;
                for (int k = 0; k < MAX_FRAMES_PER_ENLARGEMENT; k++)  /* frame number */
                {
                    if (animations[i].frame2tile[k] == j)
                        tilemaparray[k + enlargements[i].glyph_offset + GLYPH_ENLARGEMENT_OFF] = tile_count;
                }
            }
            tile_count++;
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
         *Set tile2animation 
         */

        /* Monsters */
        for (int i = 0; i < NUM_MONSTERS; i++)
        {
            if (mons[i].stand_animation)
            {
                int glyph = monnum_to_glyph(i);
                short tile = tilemaparray[glyph];
                tile2animation[tile] = mons[i].stand_animation;
            }
            if (mons[i].female_stand_animation)
            {
                int glyph = female_monnum_to_glyph(i);
                short tile = tilemaparray[glyph];
                tile2animation[tile] = mons[i].stand_animation;
            }
            if (mons[i].attack_animation)
            {
                int glyph = i + GLYPH_ATTACK_OFF;
                short tile = tilemaparray[glyph];
                tile2animation[tile] = mons[i].attack_animation;
            }
            if (mons[i].female_attack_animation)
            {
                int glyph = i + GLYPH_FEMALE_ATTACK_OFF;
                short tile = tilemaparray[glyph];
                tile2animation[tile] = mons[i].female_attack_animation;
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
            if (obj_descr[i].lit_animation)
            {
                int glyph = lit_objnum_to_glyph(i);
                short tile = glyph2tile[glyph];
                tile2animation[tile] = obj_descr[i].lit_animation;
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
            if (artilist[i].lit_animation)
            {
                int glyph = lit_objnum_to_glyph(i);
                short tile = glyph2tile[glyph];
                tile2animation[tile] = artilist[i].lit_animation;
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
                            int stand_anim_idx = get_player_stand_animation(roleidx, raceidx, gender, alignment, level);
                            if (stand_anim_idx)
                            {
                                int player_stand_glyph = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                short tile = glyph2tile[player_stand_glyph];
                                tile2animation[tile] = stand_anim_idx;
                            }
                            int attack_anim_idx = get_player_attack_animation(roleidx, raceidx, gender, alignment, level);
                            if (attack_anim_idx)
                            {
                                int player_attack_glyph = player_to_attack_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                short tile = glyph2tile[player_attack_glyph];
                                tile2animation[tile] = attack_anim_idx;
                            }
                        }
                    }
                }
            }
        }


        /*
         * Set tile2enlargement
         */

        /* Monsters */
        for (int i = 0; i < NUM_MONSTERS; i++)
        {
            if (mons[i].enlargement)
            {
                int glyph = monnum_to_glyph(i);
                short tile = tilemaparray[glyph];
                tile2enlargement[tile] = mons[i].enlargement;
            }
            if (mons[i].female_enlargement)
            {
                int glyph = female_monnum_to_glyph(i);
                short tile = tilemaparray[glyph];
                tile2enlargement[tile] = mons[i].female_enlargement;
            }
            if (mons[i].attack_enlargement)
            {
                int glyph = i + GLYPH_ATTACK_OFF;
                short tile = tilemaparray[glyph];
                tile2enlargement[tile] = mons[i].attack_enlargement;
            }
            if (mons[i].female_attack_enlargement)
            {
                int glyph = i + GLYPH_FEMALE_ATTACK_OFF;
                short tile = tilemaparray[glyph];
                tile2enlargement[tile] = mons[i].female_attack_enlargement;
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
            if (obj_descr[i].lit_enlargement)
            {
                int glyph = lit_objnum_to_glyph(i);
                short tile = glyph2tile[glyph];
                tile2enlargement[tile] = obj_descr[i].lit_enlargement;
            }
        }

        /* Artfacts */
        for (int i = 1; i <= NUM_ARTIFACTS; i++)
        {
            if (artilist[i].enlargement)
            {
                int glyph = i + GLYPH_ARTIFACT_OFF;
                short tile = glyph2tile[glyph];
                tile2enlargement[tile] = artilist[i].enlargement;
            }
            if (artilist[i].lit_enlargement)
            {
                int glyph = i + GLYPH_ARTIFACT_LIT_OFF;
                short tile = glyph2tile[glyph];
                tile2enlargement[tile] = artilist[i].lit_enlargement;
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
                            int enlargement_idx = get_player_enlargement(roleidx, raceidx, gender, alignment, level);
                            if (enlargement_idx)
                            {
                                int player_glyph = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                short tile = glyph2tile[player_glyph];
                                tile2enlargement[tile] = enlargement_idx;
                            }
                            int attack_enlargement_idx = get_player_attack_enlargement(roleidx, raceidx, gender, alignment, level);
                            if (attack_enlargement_idx)
                            {
                                int player_attack_glyph = player_to_attack_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                short tile = glyph2tile[player_attack_glyph];
                                tile2enlargement[tile] = attack_enlargement_idx;
                            }
                        }
                    }
                }
            }
        }

        /* Animations */
        for (int i = 1; i <= NUM_ANIMATIONS; i++)
        {
            for (int j = 0; j < max(0, min(MAX_FRAMES_PER_ANIMATION, animations[i].number_of_frames)); j++)
            {
                char animation_tile_glyph_index = animations[i].frame2tile[j];
                if (animation_tile_glyph_index >= 0 && animations[i].tile_enlargement[animation_tile_glyph_index])
                {
                    int glyph = (int)animation_tile_glyph_index + animations[i].glyph_offset + GLYPH_ANIMATION_OFF;
                    short tile = glyph2tile[glyph];
                    tile2enlargement[tile] = animations[i].tile_enlargement[animation_tile_glyph_index];
                }
            }
        }
    }

    return tile_count;
}


/*tiledata.c*/
