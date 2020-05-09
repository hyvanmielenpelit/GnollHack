/* GnollHack 4.0 tiledata.c */
/* Copyright (c) Janne Gustafsson 2020.                           */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "artifact.h"
#include "artilist.h"



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
    int tile_count = 0;
    char buf[BUFSIZ];
    strcpy(buf, "");

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
    for (int gender = 0; gender <= 1; gender++)
    {
        if (gender == 1)
        {
            if (tsd->female_tile_style == 0)
                continue;
        }
        const char* gender_name = (gender == 0 ? "base" : "female");

        for (int spset = 0; spset < 6; spset++)
        {
            if (spset == 1 && !tsd->has_pet_tiles)
                continue;
            if (spset == 2 && !tsd->has_detect_tiles)
                continue;
            if (spset == 3 && !tsd->has_body_tiles)
                continue;
            if (spset == 4 && !tsd->has_ridden_tiles)
                continue;
            if (spset == 5 && !tsd->has_statue_tiles)
                continue;

            set_name = (spset == 0 ? "normal" : spset == 1 ? "pet" : spset == 2 ? "detect" :
                spset == 3 ? "body" : spset == 4 ? "ridden" : "statue");
            for (int i = LOW_PM; i < NUMMONS; i++)
            {
                if (gender == 1)
                {
                    if (tsd->female_tile_style == 2 && !(mons[i].geno & G_FEMALE_TILE))
                        continue;
                }

                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,%s,%s\n", tile_section_name, gender_name, set_name, mons[i].mname);
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    int glyph_offset = (spset == 0 ? GLYPH_MON_OFF : spset == 1 ? GLYPH_PET_OFF : spset == 2 ? GLYPH_DETECT_OFF :
                        spset == 3 ? GLYPH_BODY_OFF : spset == 4 ? GLYPH_RIDDEN_OFF : GLYPH_STATUE_OFF);
                    int female_glyph_offset = (spset == 0 ? GLYPH_FEMALE_MON_OFF : spset == 1 ? GLYPH_FEMALE_PET_OFF : spset == 2 ? GLYPH_FEMALE_DETECT_OFF :
                        spset == 3 ? GLYPH_FEMALE_BODY_OFF : spset == 4 ? GLYPH_FEMALE_RIDDEN_OFF : GLYPH_FEMALE_STATUE_OFF);

                    if (gender == 0)
                    {
                        tilemaparray[i + glyph_offset] = tile_count;
                        if (spset == 0)
                        {
                            if (!tsd->has_pet_tiles)
                                tilemaparray[i + GLYPH_PET_OFF] = tile_count;
                            if (!tsd->has_detect_tiles)
                                tilemaparray[i + GLYPH_DETECT_OFF] = tile_count;
                            if (!tsd->has_ridden_tiles)
                                tilemaparray[i + GLYPH_RIDDEN_OFF] = tile_count;
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
                        if (!tsd->has_ridden_tiles)
                            tilemaparray[i + GLYPH_FEMALE_RIDDEN_OFF] = tile_count;
                    }
                }
                tile_count++;
            }
        }
    }

    /* Object tiles */
    tile_section_name = "objects";
    for (int j = 0; j <= 2; j++)
    {
        if (j > 0 && !tsd->has_right_and_left_hand_objects)
            break;

        int nameless_idx = 0;
        set_name = (j == 0 ? "normal" : j == 1 ? "right-hand" : "left-hand");
        for (int i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
        {
            /* Jump over gems that all look the same, just take pieces of glass and luckstone */
            if (objects[i].oc_class == GEM_CLASS)
            {
                if (i <= LAST_GEM || (i > LUCKSTONE && i <= FLINT))
                    continue;
            }

            boolean nameless = !OBJ_NAME(objects[i]);
            boolean no_description = !obj_descr[objects[i].oc_name_idx].oc_descr;
            char nameless_name[BUFSZ];
            Sprintf(nameless_name, "nameless-%d", nameless_idx);
            if (!OBJ_NAME(objects[i]))
            {
                nameless = TRUE;
                nameless_idx++;
            }
            const char* oclass_name = def_oc_syms[objects[i].oc_class].name;

            if (process_style == 0)
            {
                Sprintf(buf, "%s,%s,%s,%s,%s\n", tile_section_name, set_name, oclass_name,
                    nameless ? nameless_name : OBJ_NAME(objects[i]),
                    no_description ? "no description" : obj_descr[objects[i].oc_name_idx].oc_descr
                );
                (void)write(fd, buf, strlen(buf));
            }
            else if (process_style == 1)
            {
                if (j == 0 && !tsd->has_right_and_left_hand_objects)
                {
                    for (int k = 1; k <= 2; k++)
                    {
                        int glyph_offset3 = (k == 0 ? GLYPH_OBJ_OFF : k == 1 ? GLYPH_OBJ_RIGHT_HAND_OFF : GLYPH_OBJ_LEFT_HAND_OFF);
                        tilemaparray[i + glyph_offset3] = tile_count;
                    }
                }

                int glyph_offset = (j == 0 ? GLYPH_OBJ_OFF : j == 1 ? GLYPH_OBJ_RIGHT_HAND_OFF : GLYPH_OBJ_LEFT_HAND_OFF);
                tilemaparray[i + glyph_offset] = tile_count;

                /* Write generic corpse and statue tiles */
                if (j == 0 &&
                    (!tsd->has_body_tiles && i == CORPSE)
                    || (!tsd->has_statue_tiles && i == STATUE)
                    )
                {
                    for (int gender = 0; gender <= 1; gender++)
                    {
                        for (int k = LOW_PM; k < NUMMONS; k++)
                        {
                            int glyph_offset2 = (gender == 0 ?(i == CORPSE ? GLYPH_BODY_OFF : GLYPH_STATUE_OFF) : (i == CORPSE ? GLYPH_FEMALE_BODY_OFF : GLYPH_FEMALE_STATUE_OFF));
                            tilemaparray[k + glyph_offset2] = tile_count;
                        }
                    }
                }
            }
            tile_count++;
        }
    }

    /* Artifact tiles */
    tile_section_name = "artifacts";
    for (int j = 0; j <= 2; j++)
    {
        if (j > 0 && !tsd->has_right_and_left_hand_objects)
            break;

        set_name = (j == 0 ? "normal" : j == 1 ? "right-hand" : "left-hand");
        for (int i = 1; i <= NROFARTIFACTS; i++)
        {
            int base_item = artilist[i].otyp;
            boolean no_description = !artilist[i].desc;
            boolean no_base_item_name = !OBJ_NAME(objects[base_item]);
            boolean no_base_item_description = !obj_descr[objects[base_item].oc_name_idx].oc_descr;
            if (process_style == 0)
            {
                Sprintf(buf, "%s,%s,%s,%s,%s,%s\n", tile_section_name, set_name,
                    artilist[i].name,
                    no_description ? "no artifact description" : artilist[i].desc,
                    no_base_item_name ? "nameless base item" : OBJ_NAME(objects[base_item]),
                    no_base_item_description ? "no base item description" : obj_descr[objects[base_item].oc_name_idx].oc_descr
                );
                (void)write(fd, buf, strlen(buf));
            }
            else if (process_style == 1)
            {
                if (j == 0 && !tsd->has_right_and_left_hand_objects)
                {
                    for (int k = 1; k <= 2; k++)
                    {
                        int glyph_offset3 = (k == 0 ? GLYPH_ARTIFACT_OFF : k == 1 ? GLYPH_ARTIFACT_RIGHT_HAND_OFF : GLYPH_ARTIFACT_LEFT_HAND_OFF);
                        tilemaparray[i + glyph_offset3] = tile_count;
                    }
                }

                int glyph_offset = (j == 0 ? GLYPH_ARTIFACT_OFF : j == 1 ? GLYPH_ARTIFACT_RIGHT_HAND_OFF : GLYPH_ARTIFACT_LEFT_HAND_OFF);
                tilemaparray[i + glyph_offset] = tile_count;
            }
            tile_count++;
        }
    }

    /* CMAP tiles */
    tile_section_name = "cmap";
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
                if (tsd->nonzero_cmap_style == 1 && (i < S_vwall || i > S_trwall))
                    continue;
            }
            if (process_style == 0)
            {
                Sprintf(buf, "%s,%s,%s\n", tile_section_name, tsd->has_full_cmap_set ? cmap_type_names[cmap_idx] : namebuf, get_cmap_tilename(i));
                (void)write(fd, buf, strlen(buf));
            }
            else if (process_style == 1)
            {
                if (tsd->has_full_cmap_set)
                {
                    int glyph_offset = GLYPH_CMAP_OFF + cmap_idx * CMAP_TYPE_CHAR_NUM;
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
                            int glyph_offset = GLYPH_CMAP_OFF + k * CMAP_TYPE_CHAR_NUM;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                    }
                }

                if (tsd->nonzero_cmap_style == 1 && cmap_idx == 0 && num_cmaps > 1 && (i < S_vwall || i > S_trwall))
                {
                    /* copy non-walls to all other cmaps */
                    for (int k = 1; k < num_cmaps; k++)
                    {
                        int glyph_offset2 = GLYPH_CMAP_OFF + k * CMAP_TYPE_CHAR_NUM;
                        tilemaparray[i + glyph_offset2] = tile_count;
                    }
                }
            }
            tile_count++;
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
            if (NUM_INVIS_TILES == 1)
            {
                if (process_style == 0)
                {
                    Sprintf(buf, "%s,%s,generic\n", tile_section_name, set_name);
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
                for (int i = 0; i < NUM_INVIS_TILES; i++)
                {
                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,tile-%d\n", tile_section_name, set_name, i);
                        (void)write(fd, buf, strlen(buf));
                    }
                    else if (process_style == 1)
                    {
                        int glyph_offset = GLYPH_INVIS_OFF;
                        tilemaparray[i + glyph_offset] = tile_count;
                    }
                    tile_count++;
                }
            }
        }
        else if (misc_idx == 1)
        {
            const char* explosion_direction_name_array[MAXEXPCHARS] = {
                    "top-left", "top-center", "top-right",
                    "middle-right", "middle-center", "middle-left",
                    "bottom-left", "bottom-center", "bottom-right" };

            if (tsd->has_all_explode_tiles == 0)
            {
                for (int i = 0; i < MAXEXPCHARS; i++)
                {
                    const char* explosion_direction_name = explosion_direction_name_array[i];

                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s\n", tile_section_name, set_name, explosion_direction_name);
                        (void)write(fd, buf, strlen(buf));
                    }
                    else if (process_style == 1)
                    {
                        for (int j = 0; j < EXPL_MAX; j++)
                        {
                            int glyph_offset = GLYPH_EXPLODE_OFF + MAXEXPCHARS * j;
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
                    for (int i = 0; i < MAXEXPCHARS; i++)
                    {
                        const char* explosion_direction_name = explosion_direction_name_array[i];
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s\n", tile_section_name, set_name, explosion_name, explosion_direction_name);
                            (void)write(fd, buf, strlen(buf));
                        }
                        else if (process_style == 1)
                        {
                            int glyph_offset = GLYPH_EXPLODE_OFF + MAXEXPCHARS * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                        tile_count++;
                    }
                }
            }
        }
        else if (misc_idx == 2)
        {
            const char* zap_direction_name_array[4] = {
                "vertical", "horizontal", "diagonal-top-left-to-bottom-right", "diagonal-bottom-left-to-top-right" };

            if (tsd->has_all_zap_tiles == 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    const char* zap_direction_name = zap_direction_name_array[i];
                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s\n", tile_section_name, set_name, zap_direction_name);
                        (void)write(fd, buf, strlen(buf));
                    }
                    else if (process_style == 1)
                    {
                        for (int j = 0; j < NUM_ZAP; j++)
                        {
                            int glyph_offset = GLYPH_ZAP_OFF + 4 * j;
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

                    for (int i = 0; i < 4; i++)
                    {
                        const char* zap_direction_name = zap_direction_name_array[i];
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s\n", tile_section_name, set_name, zap_name, zap_direction_name);
                            (void)write(fd, buf, strlen(buf));
                        }
                        else if (process_style == 1)
                        {
                            int glyph_offset = GLYPH_ZAP_OFF + 4 * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                        tile_count++;
                    }
                }
            }
        }
        else if (misc_idx == 3)
        {
            const char* swallow_direction_name_array[8] = {
                    "top-left", "top-center", "top-right",
                    "middle-right", "middle-left",
                    "bottom-left", "bottom-center", "bottom-right" };

            if (tsd->swallow_tile_style == 0)
            {
                for (int i = 0; i < 8; i++)
                {
                    const char* swallow_direction_name = swallow_direction_name_array[i];
                    if (process_style == 0)
                    {
                        Sprintf(buf, "%s,%s,generic,%s\n", tile_section_name, set_name, swallow_direction_name);
                        (void)write(fd, buf, strlen(buf));
                    }
                    else if (process_style == 1)
                    {
                        for (int j = 0; j < NUMMONS; j++)
                        {
                            int glyph_offset = GLYPH_SWALLOW_OFF + 8 * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                    }
                    tile_count++;
                }
            }
            else
            {
                boolean first_found = FALSE;
                for (int j = 0; j < NUMMONS; j++)
                {
                    if (tsd->swallow_tile_style == 2)
                    {
                        if (!attacktype(&mons[j], AT_ENGL))
                            continue;
                    }

                    for (int i = 0; i < 8; i++)
                    {
                        const char* swallow_direction_name = swallow_direction_name_array[i];
                        if (process_style == 0)
                        {
                            Sprintf(buf, "%s,%s,%s,%s\n", tile_section_name, set_name, mons[j].mname, swallow_direction_name);
                            (void)write(fd, buf, strlen(buf));
                        }
                        else if (process_style == 1)
                        {
                            int glyph_offset = GLYPH_SWALLOW_OFF + 8 * j;
                            tilemaparray[i + glyph_offset] = tile_count;
                        }
                        tile_count++;
                    }

                    /* Write the first found swallow tile set for all monsters, just in case */
                    if (tsd->swallow_tile_style == 2 && !first_found && process_style == 1)
                    {
                        first_found = TRUE;
                        for (int k = 0; k < NUMMONS; k++)
                        {
                            for (int m = 0; m < 8; m++)
                            {
                                int glyph_offset = GLYPH_SWALLOW_OFF + 8 * k;
                                tilemaparray[m + glyph_offset] = tile_count - 8 + m;
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
                    Sprintf(buf, "%s,%s,warn-level-%d\n", tile_section_name, set_name, i);
                    (void)write(fd, buf, strlen(buf));
                }
                else if (process_style == 1)
                {
                    int glyph_offset = GLYPH_WARNING_OFF;
                    tilemaparray[i + glyph_offset] = tile_count;
                }
                tile_count++;
            }
        }
    }


    /* Player tiles */
    tile_section_name = "player";
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
                                int player_idx = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                tilemaparray[player_idx] = tilemaparray[role_as_monster + ((gender == 0) ? GLYPH_MON_OFF : GLYPH_FEMALE_MON_OFF)];
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
            Sprintf(buf, "%s,generic\n", tile_section_name);
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
                                int player_idx = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                tilemaparray[player_idx] = tile_count;
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
                                Sprintf(buf, "%s,%s,%s,%s,%s,level-%d\n", tile_section_name, role_name, race_name, gender_name, align_name, level);
                                (void)write(fd, buf, strlen(buf));
                            }
                            else if (process_style == 1)
                            {
                                int player_idx = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                tilemaparray[player_idx] = tile_count;
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
                                Sprintf(buf, "%s,%s,%s,%s,%s,level-%d\n", tile_section_name, role_name, race_name, gender_name, align_name, level);
                                (void)write(fd, buf, strlen(buf));
                            }
                            else if (process_style == 1)
                            {
                                if (alignment == -1 && !(roles[roleidx].allow & ROLE_ALIGNMENT_TILES))
                                {
                                    for (int k = 0; k <= 2; k++)
                                    {
                                        int player_idx = player_to_glyph(roleidx, raceidx, gender, k, level);
                                        tilemaparray[player_idx] = tile_count;
                                    }
                                }
                                else
                                {
                                    int player_idx = player_to_glyph(roleidx, raceidx, gender, alignment + 1, level);
                                    tilemaparray[player_idx] = tile_count;
                                }
                            }
                            tile_count++;
                        }
                    }
                }
            }
        }
    }

    if (process_style == 0)
    {
        /* Finished */
        (void)close(fd);
    }
    /* Nothing for other process_styles */

    return tile_count;
}


/*tiledata.c*/
