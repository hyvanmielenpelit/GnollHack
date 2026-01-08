/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    do.c    $NHDT-Date: 1548978604 2019/01/31 23:50:04 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.189 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Derek S. Ray, 2015. */
/* GnollHack may be freely redistributed.  See license for details. */

/* Contains code for 'd', 'D' (drop), '>', '<' (up, down) */

#include "hack.h"
#include "lev.h"
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>

STATIC_DCL void FDECL(trycall, (struct obj *));
STATIC_DCL void NDECL(polymorph_sink);
STATIC_DCL boolean NDECL(teleport_sink);
STATIC_DCL void FDECL(dosinkring, (struct obj *));
STATIC_PTR int NDECL(wipeoff);
STATIC_DCL int FDECL(menu_drop, (int));
STATIC_DCL int FDECL(menu_autostash, (int));
STATIC_DCL int NDECL(currentlevel_rewrite);
STATIC_DCL void NDECL(final_level);
STATIC_DCL void FDECL(print_corpse_properties, (winid, int));
STATIC_DCL void FDECL(revive_handle_magic_chest, (xchar*, struct obj**, int*, struct monst**));
/* STATIC_DCL boolean FDECL(badspot, (XCHAR_P,XCHAR_P)); */
STATIC_PTR int FDECL(CFDECLSPEC item_wiki_cmp, (const genericptr, const genericptr));

extern int n_dgns; /* number of dungeons, from dungeon.c */

STATIC_VAR NEARDATA const char drop_types[] = { ALLOW_COUNT, COIN_CLASS,
                                            ALL_CLASSES, 0 };

STATIC_OVL int CFDECLSPEC
item_wiki_cmp(p, q)
const genericptr p;
const genericptr q;
{
    if (!p || !q)
        return 0;

    short idx1 = *(short*)p;
    short idx2 = *(short*)q;

    const char* name1 = OBJ_NAME(objects[idx1]);
    const char* name2 = OBJ_NAME(objects[idx2]);

    return strcmpi(name1, name2);
}

/* 'd' command: drop one inventory item */
int
dodrop(VOID_ARGS)
{
    int result, i = (invent) ? 0 : (SIZE(drop_types) - 1);

    if (*u.ushops)
        sellobj_state(SELL_DELIBERATE);
    result = drop(getobj(&drop_types[i], "drop", 3, ""));
    if (*u.ushops)
        sellobj_state(SELL_NORMAL);
    if (result)
        reset_occupations();

    return result;
}

/* the '}' command - Character statistics */
int
docharacterstatistics(VOID_ARGS)
{
    int glyph = player_to_glyph_index(urole.rolenum, urace.racenum, Ufemale, u.ualign.type, 0) + GLYPH_PLAYER_OFF;
    int gui_glyph = maybe_get_replaced_glyph(glyph, u.ux, u.uy, data_to_replacement_info(glyph, LAYER_MONSTER, (struct obj*)0, &youmonst, 0UL, 0UL, 0UL, MAT_NONE, 0));

    winid datawin = WIN_ERR;
    datawin = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_CHARACTER_SCREEN, gui_glyph, extended_create_window_info_from_mon_with_flags(&youmonst, WINDOW_CREATE_FLAGS_USE_SPECIAL_SYMBOLS));

    char buf[BUFSZ];
    char buf2[BUFSZ];

    /* Name and Rank*/
    Strcpy(buf, plname);
    *buf = highc(*buf);

    Strcpy(buf2, rank());
    *buf2 = highc(*buf2);
    Sprintf(eos(buf), " the %s", buf2);

    putstr(datawin, ATR_TITLE, buf);

    /* Level and Role */
    Strcpy(buf2, (flags.female && urole.name.f) ? urole.name.f : urole.name.m);
    *buf2 = highc(*buf2);
    Sprintf(buf, "Level %d %s", u.ulevel, buf2);
    
    putstr(datawin, ATR_SUBTITLE, buf);

    /* Empty line */
    Strcpy(buf, "");
    
    putstr(datawin, 0, buf);

    /* Race */
    Strcpy(buf2, urace.noun);
    *buf2 = highc(*buf2);
    if (Upolyd)
        Sprintf(buf, "Original race:     %s", buf2);
    else
        Sprintf(buf, "Race:              %s", buf2);

    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    /* Gender */
    if (Upolyd && u.mfemale != flags.female)
    {
        Sprintf(buf, "Original gender:   %s", u.mfemale ? "Female" : "Male");
        
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    if (Upolyd)
    {
        Strcpy(buf2, pm_monster_name(&mons[u.umonnum], flags.female));
        *buf2 = highc(*buf2);
        Sprintf(buf, "Polymorphed into:  %s", buf2);
        
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }


    Strcpy(buf2, flags.female ? "Female" : "Male");
    if (Upolyd && u.mfemale != flags.female)
        Sprintf(buf, "Polymorph gender:  %s", buf2);
    else
        Sprintf(buf, "Gender:            %s", buf2);
    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    /* Alignment */
    Strcpy(buf2, (u.ualign.type == A_CHAOTIC)
        ? "Chaotic"
        : (u.ualign.type == A_NEUTRAL)
        ? "Neutral"
        : "Lawful");
    *buf2 = highc(*buf2);
    Sprintf(buf, "Alignment:         %s", buf2);
    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    /* God */
    Sprintf(buf, "God:               %s", u_gname());
    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    /* Experience */
    Sprintf(buf, "Experience:        %lld", (long long)u.uexp);

    if (u.ulevel < MAXULEV)
    {
        int ulvl = (int)u.ulevel;
        int64_t exp_for_nxt_lvl = newuexp(ulvl);

        Sprintf(eos(buf), " / %lld", (long long)exp_for_nxt_lvl);
    }
    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    /* Max level */
    if (u.ulevel != u.ulevelmax)
    {
        Sprintf(buf, "Maximum level:     %d", u.ulevelmax);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    /* Empty line, half size since the next line is a heading with its own margin */
    Strcpy(buf, " ");    
    putstr(datawin, ATR_HALF_SIZE, buf);


    /* Current attributes */
    Sprintf(buf, "Current attribute scores:");
    putstr(datawin, ATR_HEADING, buf);

    Sprintf(buf, "  St:%s Dx:%d Co:%d In:%d Wi:%d Ch:%d", get_strength_string(ACURR(A_STR)),
        ACURR(A_DEX), ACURR(A_CON), ACURR(A_INT), ACURR(A_WIS), ACURR(A_CHA));

    putstr(datawin, 0, buf);

    /* Max attributes */
    Sprintf(buf, "Maximum attribute scores:");
    putstr(datawin, ATR_HEADING, buf);

    Sprintf(buf, "  St:%s Dx:%d Co:%d In:%d Wi:%d Ch:%d", get_strength_string(urace.attrmax[A_STR]),
        urace.attrmax[A_DEX], urace.attrmax[A_CON], urace.attrmax[A_INT], urace.attrmax[A_WIS], urace.attrmax[A_CHA]);
    
    putstr(datawin, 0, buf);

    /* Race, role and alignment traits */
    Sprintf(buf, "Character traits:");
    putstr(datawin, ATR_HEADING, buf);

    int trait_count = 0;
    int i;

    for (i = 0; i < MAX_TRAIT_DESCRIPTIONS; i++)
    {
        if (urace.trait_descriptions[i] && strcmp(urace.trait_descriptions[i], ""))
        {
            trait_count++;

            char dbuf[BUFSZ * 2];
            Strcpy(dbuf, urace.trait_descriptions[i]);
            *dbuf = highc(*dbuf);

            Sprintf(buf, " %2d - %s (Race)", trait_count, dbuf);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

        }
        else
            break;
    }

    for (i = 0; i < MAX_TRAIT_DESCRIPTIONS; i++)
    {
        if (urole.trait_descriptions[i] && strcmp(urole.trait_descriptions[i], ""))
        {
            trait_count++;

            char dbuf[BUFSZ * 2];
            Strcpy(dbuf, urole.trait_descriptions[i]);
            *dbuf = highc(*dbuf);

            Sprintf(buf, " %2d - %s (Role)", trait_count, dbuf);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        else
            break;
    }

    if (u.ualign.type == A_LAWFUL || u.ualign.type == A_NEUTRAL)
    {
        trait_count++;
        Sprintf(buf, " %2d - Loses telepathy and luck if commits murder (Alignment)", trait_count);
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (u.ualign.type == A_CHAOTIC)
    {
        trait_count++;
        Sprintf(buf, " %2d - Sacrificing own race is less dangerous (Alignment)", trait_count);
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
    }

    if (trait_count == 0)
    {
        Sprintf(buf, " (None)");
        putstr(datawin, 0, buf);
    }



    /* Current intrinsics */
    Sprintf(buf, "Intrinsic and known extrinsic abilities:");
    putstr(datawin, ATR_HEADING, buf);

    int intrinsic_count = 0;
    for (i = 1; i <= LAST_PROP; i++)
    {
        struct obj* obj = 0;
        int64_t innate_intrinsic = u.uprops[i].intrinsic & (INTRINSIC | FROM_FORM);
        int64_t temporary_intrinsic = u.uprops[i].intrinsic & TIMEOUT;
        int64_t extrinsic = u.uprops[i].extrinsic;
        boolean is_recurring = property_definitions[i].recurring;
        if (extrinsic)
        {
            obj = what_gives(i, TRUE);
        }
        if (innate_intrinsic || obj || (temporary_intrinsic && !is_recurring))
        {
            intrinsic_count++;

            char dbuf2[BUFSZ];
            char dbuf3[BUFSZ];
            struct propname pn = get_property_name_ex(i);
            boolean haspdesc = pn.prop_desc != 0;
            Sprintf(dbuf2, "%s%s%s%s", pn.prop_noun ? pn.prop_noun : "", haspdesc ? " (" : "", haspdesc ? pn.prop_desc : "", haspdesc ? ")" : "");
            //Strcpy(dbuf2, get_property_name(i));
            *dbuf2 = highc(*dbuf2);
            Strcpy(dbuf3, "");

            if (innate_intrinsic & FROM_RACE)
            {
                Sprintf(dbuf3, "race");
            }
            else if (innate_intrinsic & FROM_ROLE)
            {
                if(strcmp(dbuf3, ""))
                    Sprintf(eos(dbuf3), ", ");

                Sprintf(eos(dbuf3), "role");
            }
            else if (innate_intrinsic & FROM_ACQUIRED)
            {
                if (strcmp(dbuf3, ""))
                    Sprintf(eos(dbuf3), ", ");

                Sprintf(eos(dbuf3), "acquired");
            }
            else if (innate_intrinsic & FROM_FORM)
            {
                if (strcmp(dbuf3, ""))
                    Sprintf(eos(dbuf3), ", ");

                Sprintf(eos(dbuf3), "polymorphed form");
            }
            else if (obj)
            {
                if (strcmp(dbuf3, ""))
                    Sprintf(eos(dbuf3), ", ");

                Sprintf(eos(dbuf3), "%s", cxname(obj));
            }
            
            if (temporary_intrinsic && !is_recurring)
            {
                if (strcmp(dbuf3, ""))
                    Sprintf(eos(dbuf3), ", ");

                Sprintf(eos(dbuf3), "temporary effect");
            }

            if (strcmp(dbuf3, ""))
                *dbuf3 = highc(*dbuf3);

            Sprintf(buf, " %2d - %s (%s)", intrinsic_count, dbuf2, dbuf3);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
    }
    if (intrinsic_count == 0)
    {
        Sprintf(buf, " (None)");
        putstr(datawin, 0, buf);
    }

    /* Level-up intrinsics */
    for(i = 1; i <= 2; i++)
    {
        Sprintf(buf, "Abilities to be acquired from %s:", i == 1 ? "race" : "role");
        putstr(datawin, ATR_HEADING, buf);

        const struct innate* intrinsic_ability = (i == 1 ? race_abil(urace.monsternum) : role_abil(urole.monsternum));
        int abil_count = 0;
        int table_index = 0;
        if (intrinsic_ability)
        {
            while (intrinsic_ability[table_index].ulevel > 0)
            {
                if (intrinsic_ability[table_index].ulevel > u.ulevel && intrinsic_ability[table_index].propid > 0)
                {
                    abil_count++;

                    char dbuf2[BUFSZ * 2] = "";
                    struct propname pn = get_property_name_ex(intrinsic_ability[table_index].propid);
                    boolean haspdesc = pn.prop_desc != 0;
                    Sprintf(dbuf2, "%s%s%s%s", pn.prop_noun ? pn.prop_noun : "", haspdesc ? " (" : "", haspdesc ? pn.prop_desc : "", haspdesc ? ")" : "");
                    //Strcpy(dbuf2, get_property_name(intrinsic_ability[table_index].propid));
                    *dbuf2 = highc(*dbuf2);

                    Sprintf(buf, " Level %2d - %s", intrinsic_ability[table_index].ulevel, dbuf2);
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                }
                table_index++;
            }
        }
        if (abil_count == 0)
        {
            Sprintf(buf, " (None)");
            putstr(datawin, 0, buf);
        }
    }

    print_monster_status(datawin, &youmonst);

    display_nhwindow(datawin, FALSE);
    destroy_nhwindow(datawin), datawin = WIN_ERR;

    return 0;
}


STATIC_VAR NEARDATA const char item_description_objects[] = { ALL_CLASSES, ALLOW_NONE, 0 };

/* the M('x') command - Item descriptions */
int
doitemdescriptions(VOID_ARGS)
{
    boolean proceedtoinventory = getobj_autoselect_obj ? TRUE : floorexamine();
    if (!proceedtoinventory)
        return 0;
        
    int i = (invent) ? 0 : (SIZE(item_description_objects) - 1);

    return itemdescription(getobj(&item_description_objects[i], "examine", 1, ""));
}

/* Returns TRUE if we proceed to inventory.
 * Object may be either on floor or in inventory.
 */
boolean
floorexamine(VOID_ARGS)
{
    register struct obj* otmp;
    char qbuf[QBUFSZ];
    char c;
    boolean res = TRUE;
    int cnt = 0;

    if (covers_objects(u.ux, u.uy))
        return TRUE;

    for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
        cnt++;

    if (!cnt)
        return TRUE;

    if (cnt > 1)
    {
        Sprintf(qbuf, "There are several objects here. Do you want to examine them?");
        if ((c = yn_function(qbuf, ynchars, 'n', yndescs)) != 'y')
            return  TRUE;
    }

    for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere) 
    {
        char qsfx[QBUFSZ];
        boolean one = (otmp->quan == 1L);

        /* "There is <an object> here; <verb> it?" or
            "There are <N objects> here; <verb> one?" */
        Sprintf(qbuf, "There %s ", otense(otmp, "are"));
        Sprintf(qsfx, " here; %s %s?", "examine", one ? "it" : "them");
        (void)safe_qbuf(qbuf, qbuf, qsfx, otmp, doname, ansimpleoname,
            one ? something : (const char*) "things");
        if ((c = yn_function(qbuf, ynqchars, 'n', ynqdescs)) == 'y')
        {
            (void)itemdescription(otmp);
            res = FALSE;
        }
        else if (c == 'q')
            return res;
    }

    return res;
}

void
convert_dice_to_ranges(buf)
char* buf;
{
    if (!buf || !*buf || !iflags.show_dice_as_ranges)
        return;

    int len = (int)strlen(buf);
    char* p;
    boolean found = FALSE, useplus = FALSE;
    for (p = buf; (int)(p - buf) < len - 3; p++)
    {
        if ((*p == ' ' || *p == '+') && *(p + 1) >= '0' && *(p + 1) <= '9' && *(p + 2) <= 'd' && *(p + 3) >= '0' && *(p + 3) <= '9')
        {
            found = TRUE;
            useplus = *p == '+';
            break;
        }
    }

    if (found)
    {
        int offset = (int)(p - buf);
        if (offset < len - 3)
        {
            char* qp = strstr(p + 1, "+");
            char* qs = strstr(p + 1, " ");
            if (qs)
            {
                int dice = 0;
                int diesize = 0;
                int plus = 0;
                int res = 0;
                if (qp && qp < qs && *(qp + 1) >= '0' && *(qp + 1) <= '9')
                    res = sscanf(p, useplus ? "+%dd%d+%d" : " %dd%d+%d", &dice, &diesize, &plus);
                else
                    res = sscanf(p, useplus ? "+%dd%d" : " %dd%d", &dice, &diesize);

                if (res >= 2)
                {
                    char* tmpstr = (char*)alloc(len + 32);
                    Strcpy(tmpstr, buf);
                    char* tptr = tmpstr + offset;
                    int minvalue = dice + plus;
                    int maxvalue = dice * diesize + plus;
                    if (minvalue == maxvalue)
                        Sprintf(tptr, useplus ? "+%d" : " %d", minvalue);
                    else
                        Sprintf(tptr, useplus ? "+%d-%d" : " %d-%d", minvalue, maxvalue);
                    Strcat(tptr, qs);
                    Strcpy(buf, tmpstr);
                    free(tmpstr);
                }
            }
        }
    }
}

void
printdice(buf, dice, diesize, plus)
char* buf;
int dice, diesize, plus;
{
    if (!buf)
        return;

    *buf = 0;
    if (iflags.show_dice_as_ranges)
    {
        int minval = (dice > 0 && diesize > 0 ? dice : 0) + plus;
        int maxval = (dice > 0 && diesize > 0 ? dice * diesize : 0) + plus;
        
        if (minval == maxval)
        {
            Sprintf(buf, "%d", minval);
        }
        else
        {
            Sprintf(buf, "%d-%d", minval, maxval);
        }
    }
    else
    {
        char plusbuf[BUFSZ];
        boolean maindiceprinted = FALSE;
        if (dice > 0 && diesize > 0)
        {
            maindiceprinted = TRUE;
            Sprintf(plusbuf, "%dd%d", dice, diesize);
            Strcat(buf, plusbuf);
        }

        if (plus != 0 || !maindiceprinted)
        {
            if (maindiceprinted && plus > 0)
            {
                Sprintf(plusbuf, "+");
                Strcat(buf, plusbuf);
            }
            Sprintf(plusbuf, "%d", plus);
            Strcat(buf, plusbuf);
        }
    }
}

void
printweight(buf, weight_in_ounces, weight_fixed_width, unit_fixed_width, allow_long)
char* buf;
int weight_in_ounces;
boolean weight_fixed_width;
boolean unit_fixed_width;
boolean allow_long;
{
    double weight_in_pounds = ((double)weight_in_ounces) / 16;
    if (flags.metric_system)
    {
        double avoirdupois_pound_in_kg = 0.45359237;
        double weight_in_kg = weight_in_pounds * avoirdupois_pound_in_kg;
        double weight_in_grams = weight_in_kg * 1000;

        if (allow_long && flags.detailed_weights)
        {
            Sprintf(buf, weight_fixed_width ? "%3.0f g" : "%.0f g", weight_in_grams);
            if (unit_fixed_width)
                Strcat(buf, " ");
        }
        else if (weight_in_kg >= 10000)
        {
            Sprintf(buf, weight_fixed_width ? "%3.0f" : "%.0f", weight_in_kg / 1000);
            if (unit_fixed_width)
                Strcat(buf, " t ");
            else
                Sprintf(eos(buf), " ton%s", weight_in_kg == 1000 ? "" : "s");
        }
        else if (weight_in_kg >= 1000)
        {
            Sprintf(buf, weight_fixed_width ? "%1.1f " : "%.1f", weight_in_kg / 1000);
            if (unit_fixed_width)
                Strcat(buf, " t ");
            else
                Sprintf(eos(buf), " ton%s", weight_in_kg == 1000 ? "" : "s");

        }
        else if (weight_in_kg >= 10)
        {
            Sprintf(buf, weight_fixed_width ? "%3.0f kg" : "%.0f kg", weight_in_kg);
        }
        else
        {
            if (flags.detailed_weights && weight_in_grams < 1000)
            {
                Sprintf(buf, weight_fixed_width ? "%3.0f g" : "%.0f g", weight_in_grams);
                if (unit_fixed_width)
                    Strcat(buf, " ");
            }
            else
                Sprintf(buf, weight_fixed_width ? "%1.1f kg" : "%.1f kg", weight_in_kg);
        }
    }
    else
    {
        if (allow_long && flags.detailed_weights)
        {
            Sprintf(buf, weight_fixed_width ? "%3d oz" : "%d oz", weight_in_ounces);
        }
        else if (weight_in_pounds >= 1000)
        {
            Sprintf(buf, weight_fixed_width ? "%3.0f cwt" : "%.0f cwt", weight_in_pounds / 100);
        }
        else if (flags.detailed_weights && weight_in_ounces < 1000)
        {
            Sprintf(buf, weight_fixed_width ? "%3d oz" : "%d oz", weight_in_ounces);
        }
        else if (weight_in_pounds >= 10)
        {
            Sprintf(buf, weight_fixed_width ? "%3.0f lbs" : "%.0f lbs", weight_in_pounds);
        }
        else
        {
            if (weight_in_pounds == 1)
            {
                Sprintf(buf, weight_fixed_width ? "%1.1f lb" : "%.1f lb", weight_in_pounds);
                if (unit_fixed_width)
                    Strcat(buf, " ");
            }
            else
                Sprintf(buf, weight_fixed_width ? "%1.1f lbs" : "%.1f lbs", weight_in_pounds);
        }
    }
}

STATIC_OVL
void print_corpse_properties(datawin, mnum)
winid datawin;
int mnum;
{
    struct permonst* ptr = &mons[mnum];
    char buf[BUFSZ];
    int cnt = 0;

    if (mnum >= LOW_PM)
    {
        if (nonrotting_corpse(mnum))
        {
            cnt++;
            Sprintf(buf, "  %d - Non-rotting corpse", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (is_reviver(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Reviving corpse", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (vegan(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Vegan food", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        else if (vegetarian(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Vegetarian food", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (mnum == PM_LIZARD)
        {
            cnt++;
            Sprintf(buf, "  %d - Cures petrification", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (is_quantum_mechanic(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Causes uncertain velocity", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (mnum == PM_DISENCHANTER)
        {
            cnt++;
            Sprintf(buf, "  %d - Removes random intrinsic", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (mnum == PM_NURSE)
        {
            cnt++;
            Sprintf(buf, "  %d - Cures blindness", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (mnum == PM_CHAMELEON || mnum == PM_DOPPELGANGER || mnum == PM_SANDESTIN)
        {
            cnt++;
            Sprintf(buf, "  %d - Causes polymorph", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (is_rider(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Instantly fatal", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (is_mimic(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Causes mimicking", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (is_were(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Infected with lycanthropy", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (flesh_petrifies(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Touching petrifies", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (mnum == PM_GREEN_SLIME)
        {
            cnt++;
            Sprintf(buf, "  %d - Touching turns into green slime", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (has_sickening_corpse(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Infected with terminal illness", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (has_mummy_rotted_corpse(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Infected with mummy rot", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (has_stunning_corpse(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Stunning", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (has_poisonous_corpse(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Poisonous", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (has_acidic_corpse(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Acidic", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (has_hallucinating_corpse(ptr))
        {
            cnt++;
            Sprintf(buf, "  %d - Hallucinating", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (mon_to_zombie(mnum) > NON_PM)
        {
            cnt++;
            Sprintf(buf, " %2d - Zombifiable", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (mon_to_mummy(mnum) > NON_PM)
        {
            cnt++;
            Sprintf(buf, " %2d - Mummifiable", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (((ptr)->mconveys & MC_STRENGTH) != 0L)
        {
            cnt++;
            Sprintf(buf, "  %d - May confer strength", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (((ptr)->mconveys & MC_DEXTERITY) != 0L)
        {
            cnt++;
            Sprintf(buf, "  %d - May confer dexterity", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (((ptr)->mconveys & MC_CONSTITUTION) != 0L)
        {
            cnt++;
            Sprintf(buf, "  %d - May confer constitution", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (((ptr)->mconveys & MC_INTELLIGENCE) != 0L)
        {
            cnt++;
            Sprintf(buf, "  %d - May confer intelligence", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (((ptr)->mconveys & MC_WISDOM) != 0L)
        {
            cnt++;
            Sprintf(buf, "  %d - May confer wisdom", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (((ptr)->mconveys & MC_CHARISMA) != 0L)
        {
            cnt++;
            Sprintf(buf, "  %d - May confer charisma", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (((ptr)->mconveys & MC_LEVEL_GAIN) != 0L)
        {
            cnt++;
            Sprintf(buf, "  %d - May confer an experiance level", cnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        /* Conveys */
        int i;
        for (i = 0; i < 32; i++)
        {
            uint64_t bit = (uint64_t)1 << i;
            if (((ptr)->mconveys & bit) != 0L)
            {
                int propnum = conveyed_to_prop(bit);
                if (propnum != 0)
                {
                    const char* propname = get_property_name(propnum);
                    if (propname && *propname)
                    {
                        cnt++;
                        Sprintf(buf, " %2d - May confer %s", cnt, propname);
                        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                    }
                }
            }
        }
    }

    if (cnt == 0)
    {
        Sprintf(buf, "  (None)");
        putstr(datawin, 0, buf);
    }
}

int
corpsedescription(obj)
register struct obj* obj;
{
    if (!obj || obj == &zeroobj || !is_obj_rotting_corpse(obj))
        return 0;

    if(obj->corpsenm > NON_PM)
        learn_corpse_type(obj->corpsenm);

    obj->rotknown = 1;
    return itemdescription(obj);
}

int
itemdescription(obj)
register struct obj* obj;
{
    if (!obj || obj == &zeroobj)
        return 0;

    return itemdescription_core(obj, obj->otyp, (struct item_description_stats*)0);
}

int
itemdescription_core(obj, otyp, stats_ptr)
struct obj* obj;
int otyp;
struct item_description_stats* stats_ptr; /* If non-null, only returns item stats without printing anything */
{
    if (otyp <= STRANGE_OBJECT || otyp >= NUM_OBJECTS)
        return 0;
    
    struct window_procs saved_procs = windowprocs;
    if (stats_ptr)
        itemdesc_redirect();

    winid datawin = WIN_ERR;
    int glyph = obj ? obj_to_glyph(obj, rn2_on_display_rng) : otyp + GLYPH_OBJ_OFF;
    int gui_glyph = obj ? maybe_get_replaced_glyph(glyph, obj->ox, obj->oy, data_to_replacement_info(glyph, LAYER_OBJECT, obj, (struct monst*)0, 0UL, 0UL, 0UL, MAT_NONE, 0)) : glyph;

    datawin = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_OBJECT_DESCRIPTION_SCREEN, gui_glyph, extended_create_window_info_from_obj(obj));

    boolean stats_known = obj ? object_stats_known(obj) : TRUE;
    if (obj && obj->oartifact && artilist[obj->oartifact].maskotyp != STRANGE_OBJECT)
    {
        if (!stats_known)
            otyp = artilist[obj->oartifact].maskotyp;
    }
    
    boolean uses_spell_flags = otyp_uses_spellbook_wand_flags(otyp); // object_uses_spellbook_wand_flags_and_properties(obj);
    boolean name_known = (!obj || objects[otyp].oc_name_known);
    boolean desc_known = (!obj || obj->dknown);
    boolean has_conferred_powers = FALSE;
    boolean has_extra_damage = FALSE;
    double wep_avg_dmg = 0;
    double wep_multipliable_avg_dmg = 0; //Multiplied by slaying
    double wep_all_extra_avg_dmg = 0;
    int knownacbonus = 0;
    int knownmcbonus = 0;
    int i;
    char buf[BUFSZ];
    char buf2[BUFSZ];
    char buf3[BUFSZ];
    char plusbuf[BUFSZ];
    char endbuf[BUFSZ];

    /* Name */
    Strcpy(buf, obj ? cxname(obj) : OBJ_NAME(objects[otyp]));
    *buf = highc(*buf);
    
    putstr(datawin, ATR_TITLE, buf);

    /* Type */
    strcpy_capitalized_for_title(buf3, def_oc_syms[(int)objects[otyp].oc_class].name);
    //Sprintf(buf, "Class:                  %s", buf2);
    Sprintf(buf, "%s", makesingular(buf3));
    //
    //putstr(datawin, 0, buf);
    boolean hidemainclass = FALSE;

    Strcpy(buf2, ""); /* Prefix */
    Strcpy(buf3, ""); /* Postfix preceded by : */
    if (objects[otyp].oc_class == WEAPON_CLASS)
    {
        if (is_otyp_ammo(otyp))
        {
            Strcpy(buf2, "Ammunition");
            hidemainclass = TRUE;
        }
        else if (is_otyp_launcher(otyp))
        {
            Strcpy(buf2, "Ranged");
        }
        else if(objects[otyp].oc_flags & O1_THROWN_WEAPON_ONLY)
        {
            Strcpy(buf2, "Thrown");
        }
        else if (objects[otyp].oc_flags & O1_MELEE_AND_THROWN_WEAPON)
        {
            Strcpy(buf2, "Melee and Thrown");
        }
        else
        {
            Strcpy(buf2, "Melee");
        }
        strcpy_capitalized_for_title(buf3, weapon_type_names[objects[otyp].oc_subtyp]);
    }
    else if (objects[otyp].oc_class == ARMOR_CLASS)
    {
        strcpy_capitalized_for_title(buf3, obj ? armor_class_simple_name(obj) : otyp_armor_class_simple_name(otyp));
        if (is_otyp_weapon(otyp))
        {
            Strcat(buf3, ", Melee Weapon");
        }
    }
    else if (objects[otyp].oc_class == MISCELLANEOUS_CLASS && objects[otyp].oc_subtyp > MISC_MULTIPLE_PERMITTED)
    {
        strcpy_capitalized_for_title(buf3, misc_type_names[objects[otyp].oc_subtyp]);
    }
    else if (objects[otyp].oc_class == FOOD_CLASS && objects[otyp].oc_subtyp > FOODTYPE_GENERAL)
    {
        strcpy_capitalized_for_title(buf3, food_type_names[objects[otyp].oc_subtyp]);
    }
    else if (objects[otyp].oc_class == TOOL_CLASS && objects[otyp].oc_subtyp > TOOLTYPE_GENERAL)
    {
        strcpy_capitalized_for_title(buf3, tool_type_names[objects[otyp].oc_subtyp]);
    }
    else if (objects[otyp].oc_class == TOOL_CLASS)
    {
        if (is_otyp_weptool(otyp))
        {
            Strcpy(buf2, "Weapon-like");
        }
        if (is_otyp_candle(otyp))
        {
            Strcpy(buf3, "Candle");
        }
        if (Is_otyp_container(otyp))
        {
            Strcpy(buf2, "Container");
            hidemainclass = TRUE;
        }
    }
    else if (objects[otyp].oc_class == AMULET_CLASS || objects[otyp].oc_class == RING_CLASS || objects[otyp].oc_class == POTION_CLASS)
    {
        if (stats_known)
        {
            if (objects[otyp].oc_magic)
                Strcpy(buf2, "Magical");
            else
                Strcpy(buf2, "Non-magical");
        }
    }
    else if (objects[otyp].oc_class == GEM_CLASS)
    {
        /*
        if (is_graystone(obj))
        {
            Strcpy(buf3, "Gray stone");
        }
        if (is_rock(obj))
        {
            Strcpy(buf3, "Rock");
        }
        if (is_ore(obj))
        {
            Strcpy(buf3, "Ore");
        }
        */
    }

    if (strcmp(buf2, "") != 0)
    {
        char buf4[BUFSZ * 2] = "";
        Strcpy(buf4, buf);
        //Sprintf(buf, "Category:               %s", buf2);
        if (hidemainclass)
            Sprintf(buf, "%s", buf2);
        else
            Sprintf(buf, "%s %s", buf2, buf4);
    }
    if (strcmp(buf3, "") != 0)
    {
        Sprintf(eos(buf), ": %s", buf3);
    }
    if (obj && obj->aknown && obj->oartifact)
    {
        Sprintf(eos(buf), " - Artifact");
        if ((obj->oclass == WEAPON_CLASS || obj->oclass == ARMOR_CLASS) && artilist[obj->oartifact].maskotyp > STRANGE_OBJECT)
        {
            char aobjtypebuf[BUFSZ];
            strcpy_capitalized_for_title(aobjtypebuf, OBJ_NAME(objects[artilist[obj->oartifact].maskotyp]));
            Sprintf(eos(buf), " %s", aobjtypebuf);
        }
    }
    if (stats_known && objects[otyp].oc_class == SPBOOK_CLASS && !(objects[otyp].oc_flags & O1_NON_SPELL_SPELLBOOK))
    {
        print_spell_level_text(buf2, otyp, TRUE, TRUE, FALSE);
        Sprintf(eos(buf), " - %s", buf2);
    }

    putstr(datawin, ATR_SUBTITLE, buf);

    Strcpy(buf, "");
    putstr(datawin, 0, buf);

    /* Weight */
    int objweight = obj ? obj->owt : objects[otyp].oc_weight;

    /* Show loadstone incorrectly if not known and not carried */
    if(obj && otyp == LOADSTONE && !carried(obj) && !objects[otyp].oc_name_known)
        objweight = objects[LUCKSTONE].oc_weight;

    printweight(buf2, objweight, FALSE, FALSE, TRUE);

    Sprintf(buf, "Weight:                 %s", buf2);    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    if(stats_known)
    {
        /* Gold value */
        if (!obj)
            Sprintf(buf2, "%lld gold", (long long)objects[otyp].oc_cost);
        else if(obj->oartifact)
            Sprintf(buf2, "%lld gold", (long long)artilist[obj->oartifact].cost);
        else
            Sprintf(buf2, "%lld gold", (long long)get_object_base_value(obj));

        Sprintf(buf, "Base value:             %s", buf2);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);

        /* Nutritinal value */
        if (obj ? is_edible(obj) : is_otyp_normally_edible(otyp))
        {
            Sprintf(buf2, "%u rounds", !obj ? objects[otyp].oc_nutrition : obj->oeaten ? obj->oeaten : obj_nutrition(obj, &youmonst));
            Sprintf(buf, "Nutritional value:      %s", buf2);            
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
        if (obj && otyp == EGG && obj->corpsenm >= LOW_PM && obj->known && (mvitals[obj->corpsenm].mvflags & MV_KNOWS_EGG))
        {
            Strcpy(buf2, mons[obj->corpsenm].mname);
            *buf2 = highc(*buf2);
            Sprintf(buf, "Egg type:               %s", buf2);            
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
        if (stats_known && is_otyp_normally_edible(otyp))
        {
            if (objects[otyp].oc_edible_subtype > EDIBLETYPE_NORMAL)
            {
                switch (objects[otyp].oc_edible_subtype)
                {
                case EDIBLETYPE_ROTTEN:
                    Strcpy(buf2, "Rotten");
                    break;
                case EDIBLETYPE_SICKENING:
                    Strcpy(buf2, "Sickening");
                    break;
                case EDIBLETYPE_ACIDIC:
                    Strcpy(buf2, "Acidic");
                    break;
                case EDIBLETYPE_POISONOUS:
                    Strcpy(buf2, "Poisonous");
                    break;
                case EDIBLETYPE_TAINTED:
                    Strcpy(buf2, "Tainted");
                    break;
                case EDIBLETYPE_HALLUCINATING:
                    Strcpy(buf2, "Hallucinating");
                    break;
                case EDIBLETYPE_DEADLY_POISONOUS:
                    Strcpy(buf2, "Highly poisonous");
                    break;
                default:
                    Strcpy(buf2, "Normal");
                    break;
                }
                Sprintf(buf, "Comestible type:        %s", buf2);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
            if (objects[otyp].oc_edible_effect != EDIBLEFX_NO_EFFECT
                && objects[otyp].oc_edible_effect != EDIBLEFX_APPLE
                && objects[otyp].oc_edible_effect != EDIBLEFX_EGG
                )
            {
                Strcpy(buf2, "No effect");

                if (objects[otyp].oc_edible_effect > 0)
                {
                    Strcpy(buf2, get_property_name((int)objects[otyp].oc_edible_effect));
                    *buf2 = highc(*buf2);
                }
                else if (objects[otyp].oc_edible_effect < 0)
                {
                    switch (objects[otyp].oc_edible_effect)
                    {
                    case EDIBLEFX_GAIN_STRENGTH:
                        Strcpy(buf2, "Confers strength");
                        break;
                    case EDIBLEFX_GAIN_DEXTERITY:
                        Strcpy(buf2, "Confers dexterity");
                        break;
                    case EDIBLEFX_GAIN_CONSTITUTION:
                        Strcpy(buf2, "Confers constitution");
                        break;
                    case EDIBLEFX_GAIN_INTELLIGENCE:
                        Strcpy(buf2, "Confers intelligence");
                        break;
                    case EDIBLEFX_GAIN_WISDOM:
                        Strcpy(buf2, "Confers wisdom");
                        break;
                    case EDIBLEFX_GAIN_CHARISMA:
                        Strcpy(buf2, "Confers charisma");
                        break;
                    case EDIBLEFX_CURE_LYCANTHROPY:
                        Strcpy(buf2, "Cures lycanthropy");
                        break;
                    case EDIBLEFX_CURE_BLINDNESS:
                        Strcpy(buf2, "Cures blindness");
                        break;
                    case EDIBLEFX_READ_FORTUNE:
                        Strcpy(buf2, "Contains a fortune");
                        break;
                    case EDIBLEFX_CURE_SICKNESS:
                        Strcpy(buf2, "Cures sickness");
                        break;
                    case EDIBLEFX_ROYAL_JELLY:
                        Strcpy(buf2, "Confers strength and other jelly effects");
                        break;
                    case EDIBLEFX_RESTORE_ABILITY:
                        Strcpy(buf2, "Restores abilities");
                        break;
                    case EDIBLEFX_GAIN_LEVEL:
                        Strcpy(buf2, "Confers one level");
                        break;
                    case EDIBLEFX_CURE_PETRIFICATION:
                        Strcpy(buf2, "Cures petrification");
                        break;
                    case EDIBLEFX_CURE_TELEPORTITIS:
                        Strcpy(buf2, "Cures teleportitis");
                        break;
                    default:
                        break;
                    }
                }
                Sprintf(buf, "Comestible effect:      %s", buf2);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            if (obj)
            {
                int mnum = obj->corpsenm;
                if (is_obj_rotting_corpse(obj) && mnum > NON_PM && obj->rotknown)
                {
                    int64_t rotted = get_rotted_status(obj);
                    if (rotted > 5L)
                    {
                        Strcpy(buf2, "Tainted");
                    }
                    else if (obj->orotten || rotted > 3L)
                    {
                        Strcpy(buf2, "Rotten");
                    }
                    else
                    {
                        Strcpy(buf2, "Unrotten");
                    }

                    Sprintf(buf, "Comestible quality:     %s", buf2);
                    putstr(datawin, 0, buf);
                }
            }
        }
    }

    /* Material */
    if (obj)
    {
        if (obj->dknown)
        {
            Strcpy(buf2, material_definitions[obj->material].name);
            *buf2 = highc(*buf2);
            Sprintf(buf, "Material:               %s", buf2);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }
    else if (objects[otyp].oc_material == saved_objects[otyp].oc_material) /* Non-shuffled materials */
    {
        Strcpy(buf2, material_definitions[objects[otyp].oc_material].name);
        *buf2 = highc(*buf2);
        Sprintf(buf, "Base material:          %s", buf2);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    if (name_known && (!obj || obj->oartifact == 0) && !objects[otyp].oc_unique && (objects[otyp].oc_class == SPBOOK_CLASS || objects[otyp].oc_class == SCROLL_CLASS)
        && otyp != SCR_BLANK_PAPER && otyp != SPE_BLANK_PAPER && otyp != SCR_SUPREME_DIABOLISM)
    {
        int ink = otyp_ink_cost(otyp);
        Sprintf(buf, "Base write cost:        %d charge%s", ink, plur(ink));        
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
        Sprintf(buf, "Actual write cost:      %s", (objects[otyp].oc_flags6 & O6_ALWAYS_FULL_INK_COST) != 0 ? "Always base cost" : "From half to full base cost");
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    if (is_otyp_candelabrum(otyp))
    {
        int max_candles = (int)objects[otyp].oc_special_quality;
        Sprintf(buf, "Attachable items:       Up to %d candle%s", max_candles, plur(max_candles));        
        putstr(datawin, ATR_INDENT_AT_COLON, buf);

        if (obj)
        {
            int cur_candles = (int)obj->special_quality;
            Sprintf(buf, "Currently attached:     %d candle%s", cur_candles, plur(cur_candles));
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    /* Skill */
    if (objects[otyp].oc_skill != P_NONE)
    {
        Strcpy(buf2, obj ? weapon_skill_name(obj) : otyp_weapon_skill_name(otyp));
        if (objects[otyp].oc_skill < 0 && objects[otyp].oc_skill != -P_THROWN_WEAPON)
        {
            Sprintf(buf, "Ammunition for:         Weapons using %s skill", buf2);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
        else
        {
            *buf2 = highc(*buf2);
            Sprintf(buf, "Skill:                  %s", buf2);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }        
        if (obj)
        {
            boolean bonusesprinted = FALSE;
            if (objects[otyp].oc_skill == P_SHIELD && is_shield(obj))
            {
                int shieldacbonus = shield_skill_ac_bonus(P_SKILL_LEVEL(P_SHIELD));
                int shieldmcbonus = shield_skill_mc_bonus(P_SKILL_LEVEL(P_SHIELD));
                if (-shieldacbonus == shieldmcbonus)
                    Sprintf(buf, "Skill bonuses:          %s%d to AC and MC",
                        shieldmcbonus >= 0 ? "+" : "",
                        shieldmcbonus);
                else
                    Sprintf(buf, "Skill bonuses:          %s%d to AC and %s%d to MC",
                        shieldacbonus <= 0 ? "+" : "",
                        -shieldacbonus,
                        shieldmcbonus >= 0 ? "+" : "",
                        shieldmcbonus);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
                bonusesprinted = TRUE;
            }
            if (((objects[otyp].oc_skill >= P_FIRST_WEAPON && objects[otyp].oc_skill <= P_LAST_WEAPON) || objects[otyp].oc_skill == -P_THROWN_WEAPON || objects[otyp].oc_skill == P_SHIELD) && is_weapon(obj))
            {
                
                int skilltohitbonus = weapon_skill_hit_bonus(obj, P_NONE, FALSE, FALSE, objects[otyp].oc_skill == -P_THROWN_WEAPON || is_otyp_thrown_weapon_only(otyp), 0, FALSE, objects[otyp].oc_skill == -P_THROWN_WEAPON || is_otyp_thrown_weapon_only(otyp));
                int skilldmgbonus = weapon_skill_dmg_bonus(obj, P_NONE, FALSE, FALSE, objects[otyp].oc_skill == -P_THROWN_WEAPON || is_otyp_thrown_weapon_only(otyp), 0, FALSE, objects[otyp].oc_skill == -P_THROWN_WEAPON || is_otyp_thrown_weapon_only(otyp));
                if (skilltohitbonus == skilldmgbonus)
                    Sprintf(buf, "%s          %s%d to hit and damage", bonusesprinted ? "              " : "Skill bonuses:", skilltohitbonus >= 0 ? "+" : "", skilltohitbonus);
                else
                    Sprintf(buf, "%s          %s%d to hit and %s%d to damage", bonusesprinted ? "              " : "Skill bonuses:", skilltohitbonus >= 0 ? "+" : "", skilltohitbonus, skilldmgbonus >= 0 ? "+" : "", skilldmgbonus);
                putstr(datawin, bonusesprinted ? ATR_INDENT_AT_DOUBLE_SPACE : ATR_INDENT_AT_COLON, buf);

                wep_avg_dmg += skilldmgbonus;
                wep_multipliable_avg_dmg += skilldmgbonus;
            }
        }
    }

    if (stats_known && objects[otyp].oc_class == SPBOOK_CLASS && !(objects[otyp].oc_flags & O1_NON_SPELL_SPELLBOOK))
    {
        print_spell_level_text(buf2, otyp, FALSE, TRUE, FALSE);
        Sprintf(buf, "Level:                  %s", buf2);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    boolean weapon_stats_shown = FALSE;
    if (!uses_spell_flags && desc_known && (is_otyp_weapon(otyp) || is_otyp_pick(otyp) || is_otyp_saw(otyp) || ((is_otyp_gloves(otyp) || is_otyp_boots(otyp) || objects[otyp].oc_class == GEM_CLASS))))
    {
        weapon_stats_shown = TRUE;
        //boolean maindiceprinted = FALSE;
        if (!is_otyp_ammo(otyp) && !is_otyp_gloves(otyp) && !is_otyp_boots(otyp))
        {
            /* Single or two-handed */
            if (is_otyp_bimanual(otyp))
                Strcpy(buf2, "Two-handed");
            else
                Strcpy(buf2, "Single-handed");

            Sprintf(buf, "Hands to use:           %s", buf2);
            
            putstr(datawin, ATR_INDENT_AT_COLON, buf);

            if (obj ? is_appliable_weapon(obj) || is_pick(obj) || is_saw(obj) : is_otyp_appliable_weapon(otyp) || is_otyp_pick(otyp) || is_otyp_saw(otyp))
            {
                /* Single or two-handed */
                if (obj ? is_appliable_pole_type_weapon(obj) : is_otyp_appliable_pole_type_weapon(otyp))
                {
                    if (obj && name_known && stats_known)
                    {
                        int polemin = 1, polemax = 2;
                        get_pole_type_weapon_min_max_distances(obj, &youmonst, &polemin, &polemax);
                        Sprintf(buf2, "Yes, for a ranged attack (min: %.0f\', max: %.0f\')", floor(sqrt((double)polemin) * 5), ceil(sqrt((double)polemax) * 5));
                    }
                    else
                    {
                        Strcpy(buf2, "Yes, for a ranged attack");
                    }
                }
                else if (obj ? is_pick(obj) : is_otyp_pick(otyp))
                {
                    Strcpy(buf2, "Yes, for cutting rock");
                }
                else if (obj ? is_axe(obj) : is_otyp_axe(otyp))
                {
                    Strcpy(buf2, "Yes, for chopping wood");
                }
                else if (obj ? is_saw(obj) : is_otyp_saw(otyp))
                {
                    Strcpy(buf2, "Yes, for cutting wood");
                }
                else if (obj ? is_whip(obj) : is_otyp_whip(otyp))
                {
                    Strcpy(buf2, "Yes, for disarming weapons");
                }
                else
                    Strcpy(buf2, "Yes");

                Sprintf(buf, "Appliable:              %s", buf2);                
                putstr(datawin, ATR_INDENT_AT_COLON, buf);

                if (obj && obj->exceptionality > 0 && name_known && stats_known)
                {
                    if (is_pick(obj) || is_axe(obj) || is_saw(obj))
                    {
                        Sprintf(buf, "Quality bonus:          %d%% %s speed", exceptionality_digging_speed_bonus(obj), is_axe(obj) ? "chopping" : is_saw(obj) ? "cutting" : "digging");
                        putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    }
                    else if (is_whip(obj))
                    {
                        int disarmbonus = exceptionality_weapon_disarm_bonus(obj);
                        Sprintf(buf, "Quality bonus:          %s%d to weapon disarm", disarmbonus >= 0 ? "+" : "", disarmbonus);
                        putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    }
                }
                if (material_definitions[obj ? obj->material : objects[otyp].oc_material].digging_speed_bonus != 0)
                {
                    if (obj)
                    {
                        if (is_pick(obj) || is_axe(obj) || is_saw(obj))
                        {
                            Sprintf(buf, "Material bonus:         %d%% %s speed", material_definitions[obj->material].digging_speed_bonus, is_axe(obj) ? "chopping" : is_saw(obj) ? "cutting" : "digging");
                            putstr(datawin, ATR_INDENT_AT_COLON, buf);
                        }
                    }
                    else
                    {
                        if (is_otyp_pick(otyp) || is_otyp_axe(otyp) || is_otyp_saw(otyp))
                        {
                            Sprintf(buf, "Material bonus:         %d%% %s speed", material_definitions[objects[otyp].oc_material].digging_speed_bonus, is_otyp_axe(otyp) ? "chopping" : is_otyp_saw(otyp) ? "cutting" : "digging");
                            putstr(datawin, ATR_INDENT_AT_COLON, buf);
                        }
                    }
                }
            }
        }

        int baserange = 0;

        /* Ammunition range */
        if (objects[otyp].oc_multishot_style > 0 || is_otyp_launcher(otyp) || is_otyp_nonmelee_throwing_weapon(otyp)) {

            Sprintf(buf, "%s  %s", 
                is_otyp_launcher(otyp) ? "Shots per round:      " : 
                is_otyp_nonmelee_throwing_weapon(otyp) ? "Throws per round:     " : "Attacks per round:    ", 
                multishot_style_names[objects[otyp].oc_multishot_style]);

            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        /* Ammunition range */
        if (is_otyp_launcher(otyp)) {

            baserange = obj ? weapon_range((struct obj*)0, obj) : objects[otyp].oc_range > 0 ? objects[otyp].oc_range : 0;
            if(!obj && objects[otyp].oc_range < 0)
                Strcpy(buf, "Ammunition range:       Based on strength");
            else 
                Sprintf(buf, "Ammunition range:       %d'", max(1, baserange) * 5);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        /* Jousting */
        if (can_otyp_joust(otyp))
        {
            if (obj) 
            {
                int chance = spear_skill_jousting_bonus(P_SKILL_LEVEL(weapon_skill_type(obj))) + riding_skill_jousting_bonus(P_SKILL_LEVEL(P_RIDING));
                Sprintf(buf, "Jousting:               Yes, at %d%% chance when riding", chance);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
            else
            {
                Strcpy(buf, "Jousting:               Yes");
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
        }

        /* Range and throw distance */
        if (obj && !is_gloves(obj) && !is_boots(obj))
        {
            int range = 0;
            boolean thrown = TRUE;

            /* Throw distance and fire distance for ammo */
            if (is_ammo(obj) && uwep && ammo_and_launcher(obj, uwep))
            {
                thrown = FALSE;
                range = weapon_range(obj, uwep);
            }
            else
                range = weapon_range(obj, (struct obj*)0);

            Sprintf(buf2, "%d", max(1, range) * 5);
            if(thrown)
                Sprintf(buf, "Throw distance:         %s'", buf2);
            else
                Sprintf(buf, "Range when fired:       %s'", buf2);
            
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }


        int exceptionality_multiplier = 1;

        if (obj && obj->exceptionality)
        {
                Sprintf(buf, "Weapon quality:         %s", obj->exceptionality == EXCEPTIONALITY_EXCEPTIONAL ? "Exceptional (double base damage)" :
                obj->exceptionality == EXCEPTIONALITY_ELITE ? "Elite (triple base damage)" :
                obj->exceptionality == EXCEPTIONALITY_CELESTIAL ? "Celestial (quadruple base damage)" :
                obj->exceptionality == EXCEPTIONALITY_PRIMORDIAL ? "Primordial (quadruple base damage)" :
                obj->exceptionality == EXCEPTIONALITY_INFERNAL ? "Infernal (quadruple base damage)" :
                "Unknown quality"
            );

            exceptionality_multiplier = get_exceptionality_multiplier(obj->exceptionality);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        /* Add mythic base damage now here if mknown */
        if (obj && obj->mknown)
        {
            if (has_obj_mythic_triple_base_damage(obj))
                exceptionality_multiplier += 2;
            else if (has_obj_mythic_double_base_damage(obj))
                exceptionality_multiplier += 1;
        }

        boolean printmaindmgtype = FALSE;
        boolean doubledamagetopermittedtargets = FALSE;

        /* Damage - Small */
        short wsdice = obj ? get_obj_wsdice(obj) : objects[otyp].oc_wsdice;
        short wsdam = obj ? get_obj_wsdam(obj) : objects[otyp].oc_wsdam;
        short wsdmgplus = obj ? get_obj_wsdmgplus(obj) : objects[otyp].oc_wldmgplus;
        if((wsdice > 0 && wsdam > 0) || wsdmgplus != 0)
        {
            printmaindmgtype = TRUE;
            Sprintf(buf, "Base damage - Small:    ");
            printdice(eos(buf), wsdice * exceptionality_multiplier, wsdam, wsdmgplus * exceptionality_multiplier);
            //if (wsdice > 0 && wsdam > 0)
            //{
            //    maindiceprinted = TRUE;
            //    Sprintf(plusbuf, "%dd%d", wsdice * exceptionality_multiplier, wsdam);
            //    Strcat(buf, plusbuf);
            //}

            //if (wsdmgplus != 0)
            //{
            //    if (maindiceprinted && wsdmgplus > 0)
            //    {
            //        Sprintf(plusbuf, "+");
            //        Strcat(buf, plusbuf);
            //    }
            //    Sprintf(plusbuf, "%d", wsdmgplus * exceptionality_multiplier);
            //    Strcat(buf, plusbuf);
            //}

            Strcpy(endbuf, "");
            double avgdmg = (double)exceptionality_multiplier * ((double)wsdice * (double)(1 + wsdam) / 2.0 + (double)wsdmgplus);
            if (*endbuf)
                Strcat(endbuf, ", ");
            Sprintf(eos(endbuf), "avg %.1f", avgdmg);
            if (stats_known && (objects[otyp].oc_aflags & A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS))
            {
                /* Damage - Doubled */
                if (*endbuf)
                    Strcat(endbuf, ", ");
                Strcat(endbuf, "x2");
                doubledamagetopermittedtargets = TRUE;
            }
            if (objects[otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
            {
                if (objects[otyp].oc_aflags & A1_WOUNDING)
                {
                    if(*endbuf)
                        Strcat(endbuf, ", ");
                    Strcat(endbuf, "permanent");
                }

                if (objects[otyp].oc_aflags & A1_LIFE_LEECH)
                {
                    if (*endbuf)
                        Strcat(endbuf, ", ");
                    Strcat(endbuf, "confers HP");
                }
            }

            if (*endbuf)
            {
                Sprintf(eos(buf), " (%s)", endbuf);
            }

            putstr(datawin, ATR_INDENT_AT_COLON, buf);

        }


        /* Damage - Large */
        short wldice = obj ? get_obj_wldice(obj) : objects[otyp].oc_wldice;
        short wldam = obj ? get_obj_wldam(obj) : objects[otyp].oc_wldam;
        short wldmgplus = obj ? get_obj_wldmgplus(obj) : objects[otyp].oc_wldmgplus;
        if((wldice > 0 && wldam > 0) || wldmgplus != 0)
        {
            printmaindmgtype = TRUE;
            //maindiceprinted = FALSE;
            Sprintf(buf, "Base damage - Large:    ");
            printdice(eos(buf), wldice * exceptionality_multiplier, wldam, wldmgplus * exceptionality_multiplier);

            //if (wldice > 0 && wldam > 0)
            //{
            //    maindiceprinted = TRUE;
            //    Sprintf(plusbuf, "%dd%d", wldice * exceptionality_multiplier, wldam);
            //    Strcat(buf, plusbuf);
            //}

            //if (wldmgplus != 0)
            //{
            //    if (maindiceprinted && wldmgplus > 0)
            //    {
            //        Sprintf(plusbuf, "+");
            //        Strcat(buf, plusbuf);
            //    }
            //    Sprintf(plusbuf, "%d", wldmgplus * exceptionality_multiplier);
            //    Strcat(buf, plusbuf);
            //}

            Strcpy(endbuf, "");
            double avgdmg = (double)exceptionality_multiplier * ((double)wldice * (double)(1 + wldam) / 2.0 + (double)wldmgplus);
            if (*endbuf)
                Strcat(endbuf, ", ");
            Sprintf(eos(endbuf), "avg %.1f", avgdmg);

            if (stats_known && (objects[otyp].oc_aflags & A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS))
            {
                if (*endbuf)
                    Strcat(endbuf, ", ");
                Strcat(endbuf, "x2");
                doubledamagetopermittedtargets = TRUE;
            }

            if(objects[otyp].oc_aflags & A1_USE_FULL_DAMAGE_INSTEAD_OF_EXTRA)
            {
                if (objects[otyp].oc_aflags & A1_WOUNDING)
                {
                    if (*endbuf)
                        Strcat(endbuf, ", ");
                    Strcat(endbuf, "permanent");
                }

                if (objects[otyp].oc_aflags & A1_LIFE_LEECH)
                {
                    if (*endbuf)
                        Strcat(endbuf, ", ");
                    Strcat(endbuf, "confers HP");
                }
            }

            if (*endbuf)
            {
                Sprintf(eos(buf), " (%s)", endbuf);
            }

            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        double base_avg_dmg = 0.5 * ((double)wsdice * (double)exceptionality_multiplier * (1.0 + (double)wsdam) / 2.0 + (double)wsdmgplus * (double)exceptionality_multiplier
            + wldice * (double)exceptionality_multiplier * (1.0 + (double)wldam) / 2.0 + (double)wldmgplus * (double)exceptionality_multiplier);
        /* Add mythic great damage if mknown */
        if (obj && obj->mknown)
        {
            if (has_obj_mythic_great_damage(obj))
            {
                base_avg_dmg += MYTHIC_GREAT_DAMAGE_DICE * (double)(1 + MYTHIC_GREAT_DAMAGE_DIESIZE) / 2;
                if(obj->known && objects[otyp].oc_enchantable)
                    base_avg_dmg += obj->enchantment;
            }
        }
        wep_avg_dmg += base_avg_dmg;
        wep_multipliable_avg_dmg += base_avg_dmg;
        if (doubledamagetopermittedtargets)
        {
            if (objects[otyp].oc_target_permissions)
                wep_all_extra_avg_dmg += base_avg_dmg;
            else
            {
                wep_avg_dmg += base_avg_dmg;
                wep_multipliable_avg_dmg += base_avg_dmg;
            }
        }

        /* Damage type - Main */
        if (printmaindmgtype && objects[otyp].oc_damagetype != AD_PHYS)
        {
            char dmgttext[BUFSZ] = "";
            Strcpy(dmgttext, get_damage_type_text(objects[otyp].oc_damagetype));
            *dmgttext = highc(*dmgttext);
            if (strcmp(dmgttext, "") != 0)
            {
                Sprintf(buf, "Damage type:            %s", dmgttext);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
        }

        if (stats_known
            && ((objects[otyp].oc_wedice > 0 && objects[otyp].oc_wedam > 0) || objects[otyp].oc_wedmgplus != 0))
        {
            has_extra_damage = TRUE;

            /* Damage - Extra */
            //maindiceprinted = FALSE;
            Sprintf(buf, "Extra damage:           ");
            printdice(eos(buf), objects[otyp].oc_wedice, objects[otyp].oc_wedam, objects[otyp].oc_wedmgplus);
            //if (objects[otyp].oc_wedice > 0 && objects[otyp].oc_wedam > 0)
            //{
            //    maindiceprinted = TRUE;
            //    Sprintf(plusbuf, "%dd%d", objects[otyp].oc_wedice, objects[otyp].oc_wedam);
            //    Strcat(buf, plusbuf);
            //}

            //if (objects[otyp].oc_wedmgplus != 0)
            //{
            //    if (maindiceprinted && objects[otyp].oc_wedmgplus > 0)
            //    {
            //        Sprintf(plusbuf, "+");
            //        Strcat(buf, plusbuf);
            //    }
            //    Sprintf(plusbuf, "%d", objects[otyp].oc_wedmgplus);
            //    Strcat(buf, plusbuf);
            //}

            Strcpy(endbuf, "");
            double avgdmg = ((double)objects[otyp].oc_wedice * (double)(1 + objects[otyp].oc_wedam) / 2.0 + (double)objects[otyp].oc_wedmgplus);
            if (*endbuf)
                Strcat(endbuf, ", ");
            Sprintf(eos(endbuf), "avg %.1f", avgdmg);

            if (objects[otyp].oc_aflags & A1_WOUNDING)
            {
                if (*endbuf)
                    Strcat(endbuf, ", ");
                Strcat(endbuf, "permanent");
            }

            if (objects[otyp].oc_aflags & A1_LIFE_LEECH)
            {
                if (*endbuf)
                    Strcat(endbuf, ", ");
                Strcat(endbuf, "confers HP");
            }

            boolean eligiblewielders = FALSE;
            boolean eligibletargets = FALSE;
            if (!(objects[otyp].oc_aflags & A1_EXTRA_DAMAGE_DISRESPECTS_CHARACTERS) && objects[otyp].oc_power_permissions > 0)
            {
                if (*endbuf)
                    Strcat(endbuf, ", ");
                Strcat(endbuf, "by eligible wielders");
                eligiblewielders = TRUE;
            }

            if (!(objects[otyp].oc_aflags & A1_EXTRA_DAMAGE_DISRESPECTS_TARGETS) && objects[otyp].oc_target_permissions > 0)
            {
                if (*endbuf)
                    Strcat(endbuf, ", ");
                Strcat(endbuf, "to eligible targets");
                eligibletargets = TRUE;
            }

            if (*endbuf)
            {
                Sprintf(eos(buf), " (%s)", endbuf);
            }

            putstr(datawin, ATR_INDENT_AT_COLON, buf);

            double extra_avg_dmg = (double)objects[otyp].oc_wedice * (1.0 + (double)objects[otyp].oc_wedam) / 2.0 + (double)objects[otyp].oc_wedmgplus;
            if (eligiblewielders || eligibletargets)
                wep_all_extra_avg_dmg += extra_avg_dmg;
            else
            {
                wep_avg_dmg += extra_avg_dmg;
                wep_multipliable_avg_dmg += extra_avg_dmg;
            }

            /* Damage type - Extra */
            if (objects[otyp].oc_extra_damagetype != AD_PHYS)
            {
                char dmgttext[BUFSZ] = "";
                Strcpy(dmgttext, get_damage_type_text(objects[otyp].oc_extra_damagetype));
                *dmgttext = highc(*dmgttext);
                if (strcmp(dmgttext, "") != 0)
                {
                    Sprintf(buf, "Extra damage type:      %s", dmgttext);
                    
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                }
            }
        }
        else
        {
            double simple_dmg = 0;
            if (objects[otyp].oc_class == POTION_CLASS)
                simple_dmg += 1;
            else if (obj && otyp == CORPSE)
                simple_dmg += (obj->corpsenm >= LOW_PM ? mons[obj->corpsenm].msize : 0) + 1;
            else
                simple_dmg += 0;

            wep_avg_dmg += simple_dmg;
            wep_multipliable_avg_dmg += simple_dmg;
        }

        /* Damage - Silver*/
        if (obj ? obj_counts_as_silver(obj) : objects[otyp].oc_material == MAT_SILVER)
        {
            Sprintf(buf, "Silver bonus damage:    ");
            //maindiceprinted = TRUE;
            printdice(plusbuf, 1, 20, 0);
            //Sprintf(plusbuf, "%dd%d", 1, 20);
            Strcat(buf, plusbuf);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
            wep_all_extra_avg_dmg += (1.0 + 20.0) / 2.0;
        }

        /* Strength bonuses are not applied to ammo (just launchers) to avoid double-counting */
        /* Fixed damage bonus */
        if (is_otyp_launcher(otyp) && (objects[otyp].oc_flags3 & O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH)) 
        {
            Sprintf(buf, "Fixed damage bonus:     %s%d (instead of strength)", 
                objects[otyp].oc_fixed_damage_bonus >= 0 ? "+" : "", objects[otyp].oc_fixed_damage_bonus);
            
            putstr(datawin, ATR_INDENT_AT_COLON, buf);

            double fixed_bonus = (double)objects[otyp].oc_fixed_damage_bonus;
            wep_avg_dmg += fixed_bonus;
            wep_multipliable_avg_dmg += fixed_bonus;
        }
        else if (obj && !is_ammo(obj))
        {
            double dmg_bonus = 0;
            //if (is_ammo(obj) && uwep && is_launcher(uwep) && (objects[uwep->otyp].oc_flags3 & O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH))
            //    dmg_bonus += (double)objects[uwep->otyp].oc_fixed_damage_bonus;
            //else if (is_ammo(obj) && uswapwep && is_launcher(uswapwep) && (objects[uswapwep->otyp].oc_flags3 & O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH))
            //    dmg_bonus += (double)objects[uswapwep->otyp].oc_fixed_damage_bonus;
            //else 
            if(is_otyp_nonmelee_throwing_weapon(otyp) || objects[otyp].oc_skill == P_NONE)
                dmg_bonus += strength_damage_bonus_core(ACURR(A_STR), TRUE) / 2;
            else
                dmg_bonus += strength_damage_bonus_core(ACURR(A_STR), TRUE);

            Sprintf(buf, "Strength damage bonus:  %s%.1f on average",
                dmg_bonus >= 0 ? "+" : "", dmg_bonus);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);

            wep_avg_dmg += dmg_bonus;
            wep_multipliable_avg_dmg += dmg_bonus;
        }
        /* Ammos should not get any strength bonus */
        //else if (obj)
        //{
        //    /* Otherwise get full melee strength damage bonus */
        //    double str_bonus = strength_damage_bonus_core(ACURR(A_STR), TRUE);
        //    Sprintf(buf, "Strength damage bonus:  %s%.1f on average",
        //        str_bonus >= 0 ? "+" : "", str_bonus);
        //    putstr(datawin, ATR_INDENT_AT_COLON, buf);

        //    wep_avg_dmg += str_bonus;
        //    wep_multipliable_avg_dmg += str_bonus;
        //}

        if (objects[otyp].oc_hitbonus != 0)
        {
            if (objects[otyp].oc_hitbonus > 0)
                Sprintf(buf, "To hit bonus:           +%d", objects[otyp].oc_hitbonus);
            else if (objects[otyp].oc_hitbonus < 0)
                Sprintf(buf, "To hit bonus:           %d", objects[otyp].oc_hitbonus);

            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    /* Archery bonus, applied for launcher only to avoid double-counting */
    if(obj && u.uarcherybonus != 0 && uarmb && object_stats_known(uarmb) && uarmb->otyp == BRACERS_OF_ARCHERY && /* Only if bracers of archery is known */
        (is_launcher(obj) && ammo_and_launcher(uquiver, obj))
      )
    {
        Sprintf(buf, "Archery bonus:          %s%d to hit and damage", u.uarcherybonus >= 0 ? "+" : "", u.uarcherybonus);
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

        double archery_avg_dmg = (double)u.uarcherybonus;
        wep_avg_dmg += archery_avg_dmg;
        wep_multipliable_avg_dmg += archery_avg_dmg;

    }

    if (wep_avg_dmg < 0)
        wep_avg_dmg = 0;

    short mcadj = objects[otyp].oc_mc_adjustment + (obj && (objects[otyp].oc_flags & O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT) ? -obj->enchantment : 0);
    if (objects[otyp].oc_mc_adjustment != 0 || mcadj != 0)
    {
        if (mcadj >= 0)
            Sprintf(buf, "Target MC adjustment:   +%d", mcadj);
        else
            Sprintf(buf, "Target MC adjustment:   %d", mcadj);

        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }


    boolean affectsac = ((objects[otyp].oc_class == ARMOR_CLASS && desc_known)
            || (stats_known && (objects[otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED))
            || (obj && has_obj_mythic_defense(obj) && obj->mknown)
            || (stats_known && objects[otyp].oc_class == MISCELLANEOUS_CLASS && objects[otyp].oc_armor_class != 0)
            );

    boolean affectsmc = ((objects[otyp].oc_class == ARMOR_CLASS && desc_known)
            || (stats_known && (objects[otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED))
            || (obj && has_obj_mythic_defense(obj) && obj->mknown)
            || (stats_known && objects[otyp].oc_class == MISCELLANEOUS_CLASS && objects[otyp].oc_magic_cancellation != 0)
            );


    if (obj)
    {
        boolean nonexpeptionalarmor = nonexceptionality_armor(obj);
        if ((((objects[otyp].oc_class == ARMOR_CLASS && desc_known)
            || (stats_known && (objects[otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED))
            || (obj && has_obj_mythic_defense(obj) && obj->mknown))
            && obj->exceptionality) || (nonexpeptionalarmor && name_known))
        {
            const char* excep = nonexpeptionalarmor ? "Cannot have quality" :
                obj->exceptionality == EXCEPTIONALITY_EXCEPTIONAL ? "Exceptional" :
                obj->exceptionality == EXCEPTIONALITY_ELITE ? "Elite" :
                obj->exceptionality == EXCEPTIONALITY_CELESTIAL ? "Celestial" :
                obj->exceptionality == EXCEPTIONALITY_PRIMORDIAL ? "Primordial" :
                obj->exceptionality == EXCEPTIONALITY_INFERNAL ? "Infernal" :
                "Unknown quality";

            Sprintf(buf, "Armor quality:          %s", excep);
            if (!nonexpeptionalarmor)
            {
                int acbon = get_obj_exceptionality_ac_bonus(obj);
                int mcbon = get_obj_exceptionality_mc_bonus(obj);
                if (acbon > 0 || mcbon > 0)
                {
                    Strcat(buf, " (");
                    if (acbon > 0)
                    {
                        Sprintf(eos(buf), "-%d AC", acbon);
                        knownacbonus += acbon;
                    }
                    if (mcbon > 0)
                    {
                        if (acbon > 0)
                            Strcat(buf, ", ");
                        Sprintf(eos(buf), "+%d MC", mcbon);
                        knownmcbonus += mcbon;
                    }
                    Strcat(buf, ")");
                }
            }
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }
    else
    {
        boolean nonexpeptionalarmor = is_otyp_nonexceptionality_armor(otyp);
        if (nonexpeptionalarmor)
        {
            Sprintf(buf, "Armor quality:          %s", "Cannot have quality");
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    if (affectsac)
    {
        int shownacbonus = obj ? -get_object_base_ac(obj) : -(int)objects[otyp].oc_armor_class;
        if (flags.baseacasbonus)
        {
            Sprintf(buf, "Base armor class bonus: %s%d", shownacbonus >= 0 ? "+" : "", shownacbonus);
        }
        else
        {
            Sprintf(buf, "Base armor class:       %d", 10 + shownacbonus);
        }
        knownacbonus += -shownacbonus;
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    if(affectsmc)
    {
        int mc = obj ? get_object_base_mc(obj) : (int)objects[otyp].oc_magic_cancellation;
        Sprintf(buf2, "%s%d", mc >= 0 ? "+" : "", mc);
        Sprintf(buf, "Magic cancellation:     %s", buf2);
        knownmcbonus += mc;
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    boolean use_wand_skill = objects[otyp].oc_class == WAND_CLASS || objects[otyp].oc_skill == P_WAND;
    double skill_multiplier = 1.0;
    double exceptionality_multiplier = 1.0;
    if (objects[otyp].oc_class == WAND_CLASS || (objects[otyp].oc_class == TOOL_CLASS && is_otyp_spelltool(otyp)))
    {
        if (obj && obj->exceptionality)
        {
            exceptionality_multiplier = get_wand_exceptionality_damage_multiplier(obj->exceptionality);
            Sprintf(buf, "Wand quality:           %s", obj->exceptionality == EXCEPTIONALITY_EXCEPTIONAL ? "Exceptional (double base damage)" :
                obj->exceptionality == EXCEPTIONALITY_ELITE ? "Elite (triple base damage)" :
                obj->exceptionality == EXCEPTIONALITY_CELESTIAL ? "Celestial (quadruple base damage)" :
                obj->exceptionality == EXCEPTIONALITY_PRIMORDIAL ? "Primordial (quadruple base damage)" :
                obj->exceptionality == EXCEPTIONALITY_INFERNAL ? "Infernal (quadruple base damage)" :
                "Unknown quality"
            );

            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (obj && use_wand_skill)
        {
            skill_multiplier = get_wand_skill_damage_multiplier(P_SKILL_LEVEL(P_WAND));

            char slnbuf[BUFSZ] = "";
            skill_level_name(P_WAND, slnbuf, FALSE);
            *slnbuf = highc(*slnbuf);
            Sprintf(buf, "Wand skill level:       %s (%.1fx base damage)", slnbuf, skill_multiplier);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    if (stats_known)
    {
        if (objects[otyp].oc_class == WAND_CLASS || objects[otyp].oc_class == SCROLL_CLASS || (objects[otyp].oc_class == TOOL_CLASS && is_otyp_spelltool(otyp)))
        {
            const char *itemname_hc = objects[otyp].oc_class == WAND_CLASS ? "Wand" : objects[otyp].oc_class == SCROLL_CLASS ? "Scroll" : "Item";
            const char *itemname_lc = objects[otyp].oc_class == WAND_CLASS ? "wand" : objects[otyp].oc_class == SCROLL_CLASS ? "scroll" : "item";
            const char* itempadding = objects[otyp].oc_class == SCROLL_CLASS ? "" : "  ";
            if (objects[otyp].oc_spell_dmg_dice > 0 || objects[otyp].oc_spell_dmg_diesize > 0 || objects[otyp].oc_spell_dmg_plus != 0)
            {
                //boolean maindiceprinted = FALSE;
                if(objects[otyp].oc_flags5 & O5_EFFECT_IS_HEALING)
                    Sprintf(buf, "%s healing amount:  %s", itemname_hc, itempadding);
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_DAMAGE)
                    Sprintf(buf, "%s damage:          %s", itemname_hc, itempadding);
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_MANA)
                    Sprintf(buf, "Mana restored by %s:%s", itemname_lc, itempadding);
                else
                    Sprintf(buf, "%s effect damage:   %s", itemname_hc, itempadding);


                printdice(eos(buf), 
                    max(1, (int)((double)objects[otyp].oc_spell_dmg_dice * skill_multiplier * exceptionality_multiplier)), 
                    objects[otyp].oc_spell_dmg_diesize, 
                    objects[otyp].oc_spell_dmg_plus);

                //if (objects[otyp].oc_spell_dmg_dice > 0 && objects[otyp].oc_spell_dmg_diesize > 0)
                //{
                //    maindiceprinted = TRUE;
                //    Sprintf(plusbuf, "%dd%d", max(1, (int)((double)objects[otyp].oc_spell_dmg_dice * skill_multiplier * exceptionality_multiplier)), objects[otyp].oc_spell_dmg_diesize);
                //    Strcat(buf, plusbuf);
                //}

                //if (objects[otyp].oc_spell_dmg_plus != 0)
                //{
                //    if (maindiceprinted && objects[otyp].oc_spell_dmg_plus > 0)
                //    {
                //        Sprintf(plusbuf, "+");
                //        Strcat(buf, plusbuf);
                //    }
                //    Sprintf(plusbuf, "%d", objects[otyp].oc_spell_dmg_plus);
                //    Strcat(buf, plusbuf);
                //}

                if (objects[otyp].oc_flags5 & O5_EFFECT_FOR_BLESSED_ONLY)
                    Strcat(buf, " (blessed only)");

                putstr(datawin, ATR_INDENT_AT_COLON, buf);

            }
            if (objects[otyp].oc_spell_dur_dice > 0 || objects[otyp].oc_spell_dur_diesize > 0 || objects[otyp].oc_spell_dur_plus != 0)
            {
                //boolean maindiceprinted = FALSE;
                Sprintf(buf, "%s effect duration: %s", itemname_hc, itempadding);

                int applied_plus = objects[otyp].oc_spell_dur_plus + (obj && obj->bknown ? bcsign(obj) * objects[otyp].oc_spell_dur_buc_plus : 0);
                printdice(eos(buf), objects[otyp].oc_spell_dur_dice, objects[otyp].oc_spell_dur_diesize, applied_plus);

                //if (objects[otyp].oc_spell_dur_dice > 0 && objects[otyp].oc_spell_dur_diesize > 0)
                //{
                //    maindiceprinted = TRUE;
                //    Sprintf(plusbuf, "%dd%d", objects[otyp].oc_spell_dur_dice, objects[otyp].oc_spell_dur_diesize);
                //    Strcat(buf, plusbuf);
                //}

                //if (applied_plus != 0)
                //{
                //    if (maindiceprinted && applied_plus > 0)
                //    {
                //        Sprintf(plusbuf, "+");
                //        Strcat(buf, plusbuf);
                //    }
                //    Sprintf(plusbuf, "%d", applied_plus);
                //    Strcat(buf, plusbuf);
                //}

                Sprintf(plusbuf, " round%s", (objects[otyp].oc_spell_dur_dice == 0 && objects[otyp].oc_spell_dur_diesize == 0 && applied_plus == 1) ? "" : "s");
                Strcat(buf, plusbuf);

                if (objects[otyp].oc_spell_dur_buc_plus != 0 && obj && !obj->bknown)
                {
                    char bucplusbuf[BUFSZ] = "";
                    int bucplus = objects[otyp].oc_spell_dur_buc_plus;
                    Strcat(buf, " (");
                    if (bucplus >= 0)
                    {
                        Strcpy(bucplusbuf, "-/+");
                    }
                    else
                    {
                        Strcpy(bucplusbuf, "+/");
                    }
                    Sprintf(eos(buf), "%s%d if cursed or blessed)", bucplusbuf, bucplus);
                }

                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
            if (objects[otyp].oc_spell_range > 0)
            {
                Sprintf(buf, "%s effect range:    %s%lld'", itemname_hc, itempadding, (long long)objects[otyp].oc_spell_range * 5L);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
            if (objects[otyp].oc_spell_radius > 0)
            {
                Sprintf(buf, "%s effect radius:   %s%lld'", itemname_hc, itempadding, (long long)objects[otyp].oc_spell_radius * 5L);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            if (objects[otyp].oc_spell_flags & S1_FLAGS_EFFECT_USES_MAGIC_RESISTANCE_MASK)
            {
                boolean applies = (objects[otyp].oc_spell_flags & S1_FLAGS_EFFECT_USES_MAGIC_RESISTANCE) != 0;
                Sprintf(buf, "Magic resistance:       %s", applies ? "Applies" : "Does not apply");
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            if (objects[otyp].oc_spell_flags & S1_FLAGS_EFFECT_USES_SAVING_THROW_MASK)
            {
                Sprintf(buf, "Saving throw:           Against %s", get_otyp_saving_throw_description(otyp));
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            if (objects[otyp].oc_spell_saving_throw_adjustment != 0)
            {
                Sprintf(buf, "Saving throw modifier:  %d", objects[otyp].oc_spell_saving_throw_adjustment);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            /* Flags */
            if (objects[otyp].oc_spell_flags & S1_SPELL_BYPASSES_MAGIC_RESISTANCE)
            {
                if (objects[otyp].oc_spell_flags & S1_SPELL_BYPASSES_UNIQUE_MONSTER_MAGIC_RESISTANCE)
                    Sprintf(buf, "Other:                  %s", "Bypasses magic resistance for all monsters");
                else
                    Sprintf(buf, "Other:                  %s", "Bypasses magic resistance for non-unique monsters");

                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
        }

        if (objects[otyp].oc_class == SPBOOK_CLASS)
        {
            if (objects[otyp].oc_spell_flags & S1_FLAGS_SPELLBOOK_DOES_NOT_YIELD_CASTINGS)
            {
                Sprintf(buf, "Other:                  %s", "Learning does not yield castings");
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
        }

        if (objects[otyp].oc_class == POTION_CLASS)
        {
            if (objects[otyp].oc_potion_normal_dice > 0 || objects[otyp].oc_potion_normal_diesize > 0 || objects[otyp].oc_potion_normal_plus != 0)
            {
                //boolean maindiceprinted = FALSE;
                if(objects[otyp].oc_flags5 & O5_EFFECT_IS_HEALING)
                    Sprintf(buf, "Healing amount:         ");
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_DAMAGE)
                    Sprintf(buf, "Damage:                 ");
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_MANA)
                    Sprintf(buf, "Mana restored:          ");
                else
                    Sprintf(buf, "Effect duration:        ");

                int dice = objects[otyp].oc_potion_normal_dice + (obj && obj->bknown ? (int)objects[otyp].oc_potion_normal_dice_buc_multiplier * bcsign(obj) : 0);
                int plus = objects[otyp].oc_potion_normal_plus + (obj && obj->bknown ? bcsign(obj) * (int)objects[otyp].oc_potion_normal_buc_multiplier : 0);
                printdice(eos(buf), dice, objects[otyp].oc_potion_normal_diesize, plus);

                //if (dice > 0 && objects[otyp].oc_potion_normal_diesize > 0)
                //{
                //    maindiceprinted = TRUE;
                //    Sprintf(plusbuf, "%dd%d", dice, objects[otyp].oc_potion_normal_diesize);
                //    Strcat(buf, plusbuf);
                //}

                //if (plus != 0)
                //{
                //    if (maindiceprinted && plus > 0)
                //    {
                //        Sprintf(plusbuf, "+");
                //        Strcat(buf, plusbuf);
                //    }
                //    Sprintf(plusbuf, "%d", plus);
                //    Strcat(buf, plusbuf);
                //}

                if (objects[otyp].oc_flags5 & O5_EFFECT_IS_HEALING)
                    Sprintf(plusbuf, " hit point%s", (dice == 0 && objects[otyp].oc_potion_normal_diesize == 0 && plus == 1) ? "" : "s");
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_DAMAGE)
                    Strcpy(plusbuf, "");
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_MANA)
                    Strcpy(plusbuf, " mana");
                else
                    Sprintf(plusbuf, " round%s", (dice == 0 && objects[otyp].oc_potion_normal_diesize == 0 && plus == 1) ? "" : "s");
                Strcat(buf, plusbuf);

                if(objects[otyp].oc_flags5 & O5_EFFECT_FOR_BLESSED_ONLY)
                    Strcat(buf, " (blessed only)");

                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
            if (objects[otyp].oc_potion_breathe_dice > 0 || objects[otyp].oc_potion_breathe_diesize > 0 || objects[otyp].oc_potion_breathe_plus != 0)
            {
                //boolean maindiceprinted = FALSE;
                const char* brtype = "duration";
                const char* brtypepadding = "";
                if (objects[otyp].oc_flags5 & O5_EFFECT_IS_HEALING)
                {
                    brtype = "healing";
                    brtypepadding = " ";
                }
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_DAMAGE)
                {
                    brtype = "damage";
                    brtypepadding = "  ";
                }
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_MANA)
                {
                    brtype = "mana";
                    brtypepadding = "    ";
                }

                Sprintf(buf, "Breathe %s:       %s", brtype, brtypepadding);

                int dice = objects[otyp].oc_potion_breathe_dice + (obj && obj->bknown ? (int)objects[otyp].oc_potion_breathe_dice_buc_multiplier * bcsign(obj) : 0);
                int plus = objects[otyp].oc_potion_breathe_plus + (obj && obj->bknown ? bcsign(obj) * (int)objects[otyp].oc_potion_breathe_buc_multiplier : 0);
                printdice(eos(buf), dice, objects[otyp].oc_potion_breathe_diesize, plus);

                //if (dice > 0 && objects[otyp].oc_potion_breathe_diesize > 0)
                //{
                //    maindiceprinted = TRUE;
                //    Sprintf(plusbuf, "%dd%d", dice, objects[otyp].oc_potion_breathe_diesize);
                //    Strcat(buf, plusbuf);
                //}

                //if (plus != 0)
                //{
                //    if (maindiceprinted && plus > 0)
                //    {
                //        Sprintf(plusbuf, "+");
                //        Strcat(buf, plusbuf);
                //    }
                //    Sprintf(plusbuf, "%d", plus);
                //    Strcat(buf, plusbuf);
                //}

                if (objects[otyp].oc_flags5 & O5_EFFECT_IS_HEALING)
                    Sprintf(plusbuf, " hit point%s", (dice == 0 && objects[otyp].oc_potion_breathe_diesize == 0 && plus == 1) ? "" : "s");
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_DAMAGE)
                    Strcpy(plusbuf, "");
                else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_MANA)
                    Strcpy(plusbuf, " mana");
                else
                    Sprintf(plusbuf, " round%s", (dice == 0 && objects[otyp].oc_potion_breathe_diesize == 0 && plus == 1) ? "" : "s");
                Strcat(buf, plusbuf);

                if (objects[otyp].oc_flags5 & O5_EFFECT_FOR_BLESSED_ONLY)
                    Strcat(buf, " (blessed only)");

                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            /* Impact on maximums */
            int extra_data1 = (int)objects[otyp].oc_potion_extra_data1;
            if (objects[otyp].oc_flags5 & O5_EFFECT_IS_HEALING)
            {
                if (extra_data1 > 0)
                {
                    char maxhpbuf[BUFSZ] = "";
                    if (obj && obj->bknown)
                    {
                        if (obj->blessed)
                            Sprintf(maxhpbuf, "+%d (if at max health)", extra_data1);
                        else
                            Sprintf(maxhpbuf, "0 (+%d if blessed at max)", extra_data1);
                    }
                    else
                        Sprintf(maxhpbuf, "+%d if blessed at max", extra_data1);

                    Sprintf(buf, "Maximum health gained:  %s", maxhpbuf);
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                }
            }
            else if (objects[otyp].oc_flags5 & O5_EFFECT_IS_MANA)
            {
                if (extra_data1 > 0)
                {
                    char maxmanabuf[BUFSZ] = "";
                    if (obj && obj->bknown)
                    {
                        if(obj->blessed)
                            Sprintf(maxmanabuf, "+%d (if at max mana)", extra_data1);
                        else if (obj->cursed)
                            Sprintf(maxmanabuf, "-%d (+%d if blessed at max mana)", extra_data1, extra_data1);
                        else
                            Sprintf(maxmanabuf, "0 (+/-%d if blessed at max or cursed)", extra_data1);
                    }
                    else
                        Sprintf(maxmanabuf, "+/-%d if blessed at max or cursed", extra_data1);

                    Sprintf(buf, "Maximum mana gained:    %s", maxmanabuf);
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                }
            }

            /* Special cures on healing */
            if (objects[otyp].oc_flags5 & O5_EFFECT_FLAGS_ARE_HEALING)
            {
                boolean cures_sick_blessed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_SICKNESS);
                boolean cures_sick_uncursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_SICKNESS);
                boolean cures_sick_cursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_SICKNESS);
                boolean cures_sick_noncursed = (objects[otyp].oc_potion_effect_flags & POTFLAGS_NONCURSED_CURE_SICKNESS) == POTFLAGS_NONCURSED_CURE_SICKNESS;
                boolean cures_sick_all = (objects[otyp].oc_potion_effect_flags & POTFLAGS_ALL_CURE_SICKNESS) == POTFLAGS_ALL_CURE_SICKNESS;

                boolean cures_blindness_blessed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_BLINDNESS);
                boolean cures_blindness_uncursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_BLINDNESS);
                boolean cures_blindness_cursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_BLINDNESS);
                boolean cures_blindness_noncursed = (objects[otyp].oc_potion_effect_flags & POTFLAGS_NONCURSED_CURE_BLINDNESS) == POTFLAGS_NONCURSED_CURE_BLINDNESS;
                boolean cures_blindness_all = (objects[otyp].oc_potion_effect_flags & POTFLAGS_ALL_CURE_BLINDNESS) == POTFLAGS_ALL_CURE_BLINDNESS;

                boolean cures_hallucination_blessed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_HALLUCINATION);
                boolean cures_hallucination_uncursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_HALLUCINATION);
                boolean cures_hallucination_cursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_HALLUCINATION);
                boolean cures_hallucination_noncursed = (objects[otyp].oc_potion_effect_flags & POTFLAGS_NONCURSED_CURE_HALLUCINATION) == POTFLAGS_NONCURSED_CURE_HALLUCINATION;
                boolean cures_hallucination_all = (objects[otyp].oc_potion_effect_flags & POTFLAGS_ALL_CURE_HALLUCINATION) == POTFLAGS_ALL_CURE_HALLUCINATION;

                boolean cures_stun_blessed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_STUN);
                boolean cures_stun_uncursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_STUN);
                boolean cures_stun_cursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_STUN);
                boolean cures_stun_noncursed = (objects[otyp].oc_potion_effect_flags & POTFLAGS_NONCURSED_CURE_STUN) == POTFLAGS_NONCURSED_CURE_STUN;
                boolean cures_stun_all = (objects[otyp].oc_potion_effect_flags & POTFLAGS_ALL_CURE_STUN) == POTFLAGS_ALL_CURE_STUN;

                boolean cures_confusion_blessed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_CONFUSION);
                boolean cures_confusion_uncursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_CONFUSION);
                boolean cures_confusion_cursed = !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_CONFUSION);
                boolean cures_confusion_noncursed = (objects[otyp].oc_potion_effect_flags & POTFLAGS_NONCURSED_CURE_CONFUSION) == POTFLAGS_NONCURSED_CURE_CONFUSION;
                boolean cures_confusion_all = (objects[otyp].oc_potion_effect_flags & POTFLAGS_ALL_CURE_CONFUSION) == POTFLAGS_ALL_CURE_CONFUSION;

                if (obj && obj->bknown)
                {
                    boolean cures_sick = FALSE;
                    boolean cures_blind = FALSE;
                    boolean cures_hallucination = FALSE;
                    boolean cures_stun = FALSE;
                    boolean cures_confusion = FALSE;
                    cures_sick = obj->blessed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_SICKNESS) :
                        obj->cursed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_SICKNESS) :
                        !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_SICKNESS);
                    cures_blind = obj->blessed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_BLINDNESS) :
                        obj->cursed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_BLINDNESS) :
                        !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_BLINDNESS);
                    cures_hallucination = obj->blessed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_HALLUCINATION) :
                        obj->cursed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_HALLUCINATION) :
                        !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_HALLUCINATION);
                    cures_stun = obj->blessed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_STUN) :
                        obj->cursed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_STUN) :
                        !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_STUN);
                    cures_confusion = obj->blessed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_BLESSED_CURE_CONFUSION) :
                        obj->cursed ? !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_CURSED_CURE_CONFUSION) :
                        !!(objects[otyp].oc_potion_effect_flags & POTFLAGS_UNCURSED_CURE_CONFUSION);

                    Sprintf(buf, "Cures sickness:         %s (%s)", cures_sick ? "Yes" : "No",
                        cures_sick_all ? "always" : cures_sick_noncursed ? (cures_sick ? "when not cursed" : "yes if not cursed") : 
                        cures_sick_blessed ? (cures_sick ? "when blessed" : "yes if blessed") : cures_sick_uncursed ? (cures_sick ? "when uncursed" : "yes if uncursed") : 
                        cures_sick_cursed ? (cures_sick ? "when cursed" : "yes if cursed") : "never");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Cures blindness:        %s (%s)", cures_blind ? "Yes" : "No",
                        cures_blindness_all ? "always" : cures_blindness_noncursed ? (cures_blind ? "when not cursed" : "yes if not cursed") :
                        cures_blindness_blessed ? (cures_blind ? "when blessed" : "yes if blessed") : cures_blindness_uncursed ? (cures_blind ? "when uncursed" : "yes if uncursed") :
                        cures_blindness_cursed ? (cures_blind ? "when cursed" : "yes if cursed") : "never");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Cures hallucination:    %s (%s)", cures_hallucination ? "Yes" : "No",
                        cures_hallucination_all ? "always" : cures_hallucination_noncursed ? (cures_hallucination ? "when not cursed" : "yes if not cursed") :
                        cures_hallucination_blessed ? (cures_hallucination ? "when blessed" : "yes if blessed") : cures_hallucination_uncursed ? (cures_hallucination ? "when uncursed" : "yes if uncursed") :
                        cures_hallucination_cursed ? (cures_hallucination ? "when cursed" : "yes if cursed") : "never");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Cures stun:             %s (%s)", cures_stun ? "Yes" : "No",
                        cures_stun_all ? "always" : cures_stun_noncursed ? (cures_stun ? "when not cursed" : "yes if not cursed") :
                        cures_stun_blessed ? (cures_stun ? "when blessed" : "yes if blessed") : cures_stun_uncursed ? (cures_stun ? "when uncursed" : "yes if uncursed") :
                        cures_stun_cursed ? (cures_stun ? "when cursed" : "yes if cursed") : "never");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Cures confusion:        %s (%s)", cures_confusion ? "Yes" : "No",
                        cures_confusion_all ? "always" : cures_confusion_noncursed ? (cures_confusion ? "when not cursed" : "yes if not cursed") : 
                        cures_confusion_blessed ? (cures_confusion ? "when blessed" : "yes if blessed") : cures_confusion_uncursed ? (cures_confusion ? "when uncursed" : "yes if uncursed") : 
                        cures_confusion_cursed ? (cures_confusion ? "when cursed" : "yes if cursed") : "never");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                }
                else
                {
                    Sprintf(buf, "Cures sickness:         %s", cures_sick_all ? "Yes" : cures_sick_noncursed ? "If not cursed" : cures_sick_blessed ? "If blessed" : cures_sick_uncursed ? "If uncursed" : cures_sick_cursed ? "If cursed" : "No");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Cures blindness:        %s", cures_blindness_all ? "Yes" : cures_blindness_noncursed ? "If not cursed" : cures_blindness_blessed ? "If blessed" : cures_blindness_uncursed ? "If uncursed" : cures_blindness_cursed ? "If cursed" : "No");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Cures hallucination:    %s", cures_hallucination_all ? "Yes" : cures_hallucination_noncursed ? "If not cursed" : cures_hallucination_blessed ? "If blessed" : cures_hallucination_uncursed ? "If uncursed" : cures_hallucination_cursed ? "If cursed" : "No");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Cures stun:             %s", cures_stun_all ? "Yes" : cures_stun_noncursed ? "If not cursed" : cures_stun_blessed ? "If blessed" : cures_stun_uncursed ? "If uncursed" : cures_stun_cursed ? "If cursed" : "No");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Cures confusion:        %s", cures_confusion_all ? "Yes" : cures_confusion_noncursed ? "If not cursed" : cures_confusion_blessed ? "If blessed" : cures_confusion_uncursed ? "If uncursed" : cures_confusion_cursed ? "If cursed" : "No");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                }
            }


            if (objects[otyp].oc_potion_nutrition_dice > 0 || objects[otyp].oc_potion_nutrition_diesize > 0 || objects[otyp].oc_potion_nutrition_plus != 0)
            {
                //boolean maindiceprinted = FALSE;
                Sprintf(buf, "Nutrition:              ");

                int dice = objects[otyp].oc_potion_nutrition_dice + (obj && obj->bknown ? (int)objects[otyp].oc_potion_nutrition_dice_buc_multiplier * bcsign(obj) : 0);
                int plus = objects[otyp].oc_potion_nutrition_plus + (obj && obj->bknown ? bcsign(obj) * (int)objects[otyp].oc_potion_nutrition_buc_multiplier : 0);
                printdice(eos(buf), dice, objects[otyp].oc_potion_nutrition_diesize, plus);

                //if (dice > 0 && objects[otyp].oc_potion_nutrition_diesize > 0)
                //{
                //    maindiceprinted = TRUE;
                //    Sprintf(plusbuf, "%dd%d", dice, objects[otyp].oc_potion_nutrition_diesize);
                //    Strcat(buf, plusbuf);
                //}

                //if (plus != 0)
                //{
                //    if (maindiceprinted && plus > 0)
                //    {
                //        Sprintf(plusbuf, "+");
                //        Strcat(buf, plusbuf);
                //    }
                //    Sprintf(plusbuf, "%d", plus);
                //    Strcat(buf, plusbuf);
                //}

                Sprintf(plusbuf, " round%s", (dice == 0 && objects[otyp].oc_potion_nutrition_diesize == 0 && plus == 1) ? "" : "s");
                Strcat(buf, plusbuf);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            if (objects[otyp].oc_spell_flags & S1_FLAGS_EFFECT_USES_MAGIC_RESISTANCE_MASK)
            {
                boolean applies = (objects[otyp].oc_spell_flags & S1_FLAGS_EFFECT_USES_MAGIC_RESISTANCE) != 0;
                Sprintf(buf, "Magic resistance:       %s", applies ? "Applies" : "Does not apply");
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            if (objects[otyp].oc_spell_flags & S1_FLAGS_EFFECT_USES_SAVING_THROW_MASK)
            {
                Sprintf(buf, "Saving throw:           Against %s", get_otyp_saving_throw_description(otyp));
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }

            if (objects[otyp].oc_potion_saving_throw_adjustment != 0)
            {
                Sprintf(buf, "Saving throw modifier:  %d", objects[otyp].oc_potion_saving_throw_adjustment);
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
        }

        int64_t splpenalty = obj ? get_object_spell_casting_penalty(obj) : objects[otyp].oc_spell_casting_penalty * ARMOR_SPELL_CASTING_PENALTY_MULTIPLIER;
        if (objects[otyp].oc_class != SPBOOK_CLASS && objects[otyp].oc_class != WAND_CLASS &&
            (objects[otyp].oc_class == ARMOR_CLASS || (objects[otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED) || splpenalty != 0))
        {
            Sprintf(buf2, "%s%lld%%", splpenalty <= 0 ? "+" : "", (long long)-splpenalty);
            if (splpenalty < 0)
                Sprintf(buf, "Spell casting bonus:    %s (somatic spells only)", buf2);
            else
                Sprintf(buf, "Spell casting penalty:  %s (somatic spells only)", buf2);

            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    /* CHARGES */
    if (objects[otyp].oc_charged)
    {
        if (obj && obj->known)
        {
            Sprintf(buf, "Charges left:           %d", obj->charges);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
        else if (stats_known)
        {
            Sprintf(buf, "Charges:                %s", "Yes");
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (stats_known)
        {
            Sprintf(buf, "Maximum charges:        %d", get_max_charge(objects[otyp].oc_charged));
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (obj && obj->known)
        {
            Sprintf(buf, "Rechargings before:     %d", (int)obj->recharged);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (stats_known)
        {
            char rechargebuf[BUFSZ];
            Strcpy(rechargebuf, get_recharge_text(objects[otyp].oc_recharging));
            *rechargebuf = highc(*rechargebuf);

            Sprintf(buf, "Recharging type:        %s", rechargebuf);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    /* ENCHANTMENT */
    if (objects[otyp].oc_enchantable)
    {
        if (obj && obj->known)
        {
            Strcpy(buf, "");

            char bonusbuf[BUFSZ] = "";
            boolean display_ac = affectsac && !(objects[otyp].oc_flags & O1_ENCHANTMENT_DOES_NOT_AFFECT_AC);
            boolean display_mc = affectsmc && !(objects[otyp].oc_flags & O1_ENCHANTMENT_DOES_NOT_AFFECT_MC);

            if (is_weapon(obj)) // obj->oclass == WEAPON_CLASS || is_weptool(obj)
            {
                int enchplus = obj->enchantment;
                int tohitplus = enchplus; // is_launcher(obj) ? (enchplus + 1 * sgn(enchplus)) / 2 : (throwing_weapon(obj) || is_ammo(obj)) ? (enchplus + 0) / 2 : enchplus;
                int dmgplus = enchplus; //  is_launcher(obj) ? (enchplus + 0) / 2 : (throwing_weapon(obj) || is_ammo(obj)) ? (enchplus + 1 * sgn(enchplus)) / 2 : enchplus;

                double ench_bonus = (double)dmgplus;
                wep_avg_dmg += ench_bonus;
                wep_multipliable_avg_dmg += ench_bonus;
                if (wep_avg_dmg < 0)
                    wep_avg_dmg = 0;

                if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS))
                {
                    enchplus *= 2;
                    wep_all_extra_avg_dmg += enchplus;
                }
                if (tohitplus == dmgplus)
                    Sprintf(bonusbuf, " (%s%d to hit and damage", tohitplus >= 0 ? "+" : "", tohitplus);
                else
                    Sprintf(bonusbuf, " (%s%d to hit and %s%d to damage", tohitplus >= 0 ? "+" : "", tohitplus, dmgplus >= 0 ? "+" : "", dmgplus);
            }

            if (display_ac || display_mc)
            {
                if (!*bonusbuf)
                    Strcpy(bonusbuf, " (");
                else
                    Strcat(bonusbuf, ", ");
            }

            if (display_ac && display_mc)
            {
                if (obj->enchantment == 0)
                    Strcat(bonusbuf, "+0 to AC and MC");
                else
                    Sprintf(eos(bonusbuf), "%s%d to AC and %s%d to MC",
                        obj->enchantment <= 0 ? "+" : "",
                        -obj->enchantment,
                        obj->enchantment / 3 >= 0 ? "+" : "",
                        obj->enchantment / 3
                    );
                knownacbonus += obj->enchantment;
                knownmcbonus += obj->enchantment / 3;
            }
            else if (display_ac)
            {
                Sprintf(eos(bonusbuf), "%s%d %s to AC",
                    obj->enchantment <= 0 ? "+" : "",
                    -obj->enchantment,
                    obj->enchantment >= 0 ? "bonus" : "penalty");
                knownacbonus += obj->enchantment;
            }
            else if (display_mc)
            {
                Sprintf(eos(bonusbuf), "%s%d %s to MC",
                    obj->enchantment / 3 >= 0 ? "+" : "",
                    obj->enchantment / 3,
                    obj->enchantment / 3 >= 0 ? "bonus" : "penalty");
                knownmcbonus += obj->enchantment / 3;
            }
            if (*bonusbuf)
                Strcat(bonusbuf, ")");

            Sprintf(buf, "Enchantment status:     %s%d%s", obj->enchantment >= 0 ? "+" : "", obj->enchantment, bonusbuf);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);

        }
        else if (stats_known)
        {
            Sprintf(buf, "Enchantable:            %s", "Yes");
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (stats_known)
        {
            int max_ench = obj ? get_obj_max_enchantment(obj) : get_max_enchantment(objects[otyp].oc_enchantable);
            Sprintf(buf, "Safe enchantable level: %s%d or below", max_ench >= 0 ? "+" : "", max_ench);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    /* Light sources, erosion and other obj-specific properties */
    if (obj)
    {
        /* Light sources */
        if (is_obj_light_source(obj))
        {
            Sprintf(buf, "Light radius:           %d'", 5 * obj_light_radius(obj));
            putstr(datawin, ATR_INDENT_AT_COLON, buf);

            if (stats_known)
            {
                int64_t maxburn = obj_light_maximum_burn_time(obj);
                if (obj_burns_infinitely(obj) || maxburn < 0)
                {
                    Strcpy(buf, "Burning time left:      Infinite");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Strcpy(buf, "Maximum burning time:   Infinite");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                }
                else if ((obj->speflags & SPEFLAGS_HAS_BEEN_PICKED_UP_BY_HERO) != 0) /* Need to weigh the item */
                {
                    int64_t burnleft = obj_light_burn_time_left(obj);
                    Sprintf(buf, "Burning time left:      %lld turn%s", (long long)burnleft, plur(burnleft));
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                    Sprintf(buf, "Maximum burning time:   %lld turn%s", (long long)maxburn, plur(maxburn));
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                }
            }
            else
            {
                if (is_light_source_empty(obj))
                {
                    Strcpy(buf, "Burning time left:      Empty");
                    putstr(datawin, ATR_INDENT_AT_COLON, buf);
                }
            }
        }

        if (is_damageable(obj) && (!is_obj_identified_when_damageable(obj) || (obj->rknown && obj->oerodeproof) || obj->oeroded || obj->oeroded2))
        {
            char erodebuf[BUFSZ] = "";
            if (obj->rknown)
            {
                if (obj->oerodeproof)
                {
                    char proofbuf[BUFSZ] = "";
                    add_erosion_words(obj, proofbuf);
                    Sprintf(erodebuf, "No, %s", proofbuf);
                }
                else
                {
                    Strcpy(erodebuf, "Yes");
                }
            }
            else
            {
                if (obj->oeroded || obj->oeroded2)
                    Strcpy(erodebuf, "Yes"); /* Should not happen */
                else
                    Strcpy(erodebuf, "Potentially");
            }

            Sprintf(buf, "Damaged by erosion:     %s", erodebuf);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (obj->oeroded || obj->oeroded2 || (obj->rknown && obj->oerodeproof && !is_damageable(obj)))
        {
            char erodebuf[BUFSZ] = "";
            char penaltybuf[BUFSZ] = "";
            int penalty = 0;

            add_erosion_words(obj, erodebuf);
            *erodebuf = highc(*erodebuf);

            if (obj->oeroded > 0 || obj->oeroded2 > 0)
            {
                if (obj->oclass == WEAPON_CLASS || is_weptool(obj))
                {
                    penalty = greatest_erosion(obj);
                    Sprintf(penaltybuf, "(%d to damage) ", -penalty);
                    double erosion_bonus = -1.0 * (double)penalty;
                    wep_avg_dmg += erosion_bonus;
                    wep_multipliable_avg_dmg += erosion_bonus;
                    if (wep_avg_dmg < 0)
                        wep_avg_dmg = 0;
                }

                if (obj->oclass == ARMOR_CLASS || (stats_known && (objects[otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED)))
                {
                    penalty = min(greatest_erosion(obj), get_object_base_ac(obj));
                    Sprintf(eos(penaltybuf), "(+%d penalty to AC)", penalty);
                    knownacbonus += -penalty;
                }
            }
            Sprintf(buf, "Erosion status:         %s%s", erodebuf, penaltybuf);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (obj->oclass != COIN_CLASS && obj->otyp != SCR_MAIL)
        {
            Sprintf(buf, "Blessing status:        %s", !obj->bknown ? "Undetermined" : obj->blessed ? "Blessed" : obj->cursed ? "Cursed" : "Uncursed");
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        /* Mythic status */
        boolean nonmythic = (is_weapon(obj) || is_armor(obj)) && otyp_non_mythic(otyp)
            && !obj->oartifact && !objects[otyp].oc_unique && !(objects[otyp].oc_flags3 & O3_UNIQUE);
        if (obj->dknown && name_known && (obj->mythic_prefix || obj->mythic_suffix || nonmythic))
        {
            Sprintf(buf, "Mythic status:          %s", nonmythic ? "Cannot be mythic" : (obj->mythic_prefix && obj->mythic_suffix) ? "Legendary" : "Mythic");
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (obj->opoisoned)
        {
            Sprintf(buf, "Poisoned status:        Poisoned (+2d6 poison damage)");
            convert_dice_to_ranges(buf);
            wep_avg_dmg += 7.0;
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }

        if (obj->elemental_enchantment)
        {
            Sprintf(buf, "Elemental enchantment:  %s", obj->elemental_enchantment == FIRE_ENCHANTMENT ? "Flaming (+4d6 fire damage)" :
                obj->elemental_enchantment == COLD_ENCHANTMENT ? "Freezing (+12d6 cold damage)" :
                obj->elemental_enchantment == LIGHTNING_ENCHANTMENT ? "Electrified (+6d6 lightning damage)" :
                obj->elemental_enchantment == DEATH_ENCHANTMENT ? "Death-magical (kills on hit)" : "Unknown enchantment"
            );

            wep_all_extra_avg_dmg += obj->elemental_enchantment == FIRE_ENCHANTMENT ? 14.0 :
                obj->elemental_enchantment == COLD_ENCHANTMENT ? 42.0 :
                obj->elemental_enchantment == LIGHTNING_ENCHANTMENT ? 21.0 :
                obj->elemental_enchantment == DEATH_ENCHANTMENT ? 0.0 : 0.0;

            convert_dice_to_ranges(buf);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }
    else
    {
        boolean nonmythic = (is_otyp_weapon(otyp) || is_otyp_armor(otyp)) && otyp_non_mythic(otyp) && !objects[otyp].oc_unique && !(objects[otyp].oc_flags3 & O3_UNIQUE);
        if (nonmythic)
        {
            Sprintf(buf, "Mythic status:          %s", "Cannot be mythic");
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    /* Historic status for statues */
    if (obj && otyp == STATUE && obj->dknown)
    {
        if (obj->special_quality == SPEQUAL_STATUE_HISTORIC)
        {
            Sprintf(buf, "Historic statue:        %s", "Yes");
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
        else
        {
            Sprintf(buf, "Historic statue:        %s", "No");
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    /* Identification status */
    boolean notfullyidentified = FALSE;
    if (obj && obj->dknown)
    {
        notfullyidentified = not_fully_identified(obj);
        Sprintf(buf, "Identification status:  %s", stats_known ? (notfullyidentified ? "Known" : "Fully known") : "Unidentified");
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    if (stats_known && objects[otyp].oc_item_cooldown > 0)
    {
        Sprintf(buf, "Cooldown time:          %d rounds", (int)objects[otyp].oc_item_cooldown);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);

        if (obj && obj->cooldownleft > 0)
        {
            Sprintf(buf, "Cooldown left:          %d rounds", (int)obj->cooldownleft);
            putstr(datawin, ATR_INDENT_AT_COLON, buf);
        }
    }

    if (obj && stats_known && obj->repowerleft > 0)
    {
        Sprintf(buf, "Repowering time left:   %d rounds", (int)obj->repowerleft);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    if (obj && stats_known && obj->invokeon)
    {
        Sprintf(buf, "Invoked ability:        Activated");
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    if (obj && stats_known && obj->invokeleft > 0)
    {
        Sprintf(buf, "Invoke time left:       %d rounds", (int)obj->invokeleft);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    /* Extra info is a non-spell-type item is known */
    if (stats_known && !uses_spell_flags)
    {
        if (objects[otyp].oc_oprop > 0
            || objects[otyp].oc_oprop2 > 0
            || objects[otyp].oc_oprop3 > 0
            || (obj && obj->material != objects[otyp].oc_material 
                && ((is_armor(obj) && (material_definitions[obj->material].power_armor[objects[otyp].oc_armor_category] != NO_POWER || material_definitions[obj->material].power2_armor[objects[otyp].oc_armor_category] != NO_POWER))
                    || (is_weapon(obj) && (material_definitions[obj->material].power_weapon != NO_POWER || material_definitions[obj->material].power2_weapon != NO_POWER)))
               )
            || objects[otyp].oc_mana_bonus > 0
            || objects[otyp].oc_hp_bonus > 0
            || objects[otyp].oc_bonus_attributes > 0
            || objects[otyp].oc_aflags > 0
            || objects[otyp].oc_pflags & P1_CONFERS_LUCK
            || objects[otyp].oc_pflags & P1_CONFERS_UNLUCK
            )
        {
            has_conferred_powers = TRUE;

            Sprintf(buf, "Conferred powers:");
            putstr(datawin, ATR_HEADING, buf);

            int powercnt = 0;
            int j;
            for (j = -3; j <= 6; j++)
            {
                int prop = 0;
                if (j == -3)
                    prop = obj && obj->material != objects[otyp].oc_material && is_weapon(obj) ? material_definitions[obj->material].power_weapon : NO_POWER;
                else if (j == -2)
                    prop = obj && obj->material != objects[otyp].oc_material && is_weapon(obj) ? material_definitions[obj->material].power2_weapon : NO_POWER;
                else if (j == -1)
                    prop = obj && obj->material != objects[otyp].oc_material && is_armor(obj) ? material_definitions[obj->material].power_armor[objects[otyp].oc_armor_category] : NO_POWER;
                else if (j == 0)
                    prop = obj && obj->material != objects[otyp].oc_material && is_armor(obj) ? material_definitions[obj->material].power2_armor[objects[otyp].oc_armor_category] : NO_POWER;
                else if (j == 1)
                    prop = objects[otyp].oc_oprop;
                else if (j == 2)
                    prop = objects[otyp].oc_oprop2;
                else if (j == 3)
                    prop = objects[otyp].oc_oprop3;
                else if (j == 4)
                    prop = (int)objects[otyp].oc_mana_bonus;
                else if (j == 5)
                    prop = (int)objects[otyp].oc_hp_bonus;
                else if (j == 6)
                    prop = (int)objects[otyp].oc_bonus_attributes;

                char pwbuf[BUFSZ] = "";
                if (j <= 0)
                {
                    Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");
                }
                else if (j == 1)
                {
                    if (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_WHEN_CARRIED)
                        Sprintf(eos(pwbuf), " when carried");
                    else
                        Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");

                    if (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_ALL_CHARACTERS)
                        Sprintf(eos(pwbuf), " (applies to all)");
                    else if (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
                        Sprintf(eos(pwbuf), " (applies to inappropriate only)");
                }
                else if (j == 2)
                {
                    if (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_WHEN_CARRIED)
                        Sprintf(eos(pwbuf), " when carried");
                    else
                        Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");

                    if (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_ALL_CHARACTERS)
                        Sprintf(eos(pwbuf), " (applies to all)");
                    else if (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
                        Sprintf(eos(pwbuf), " (applies to inappropriate only)");
                }
                else if (j == 3)
                {
                    if (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_WHEN_CARRIED)
                        Sprintf(eos(pwbuf), " when carried");
                    else
                        Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");

                    if (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_ALL_CHARACTERS)
                        Sprintf(eos(pwbuf), " (applies to all)");
                    else if (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
                        Sprintf(eos(pwbuf), " (applies to inappropriate only)");
                }
                else if (j == 4)
                {
                    if (objects[otyp].oc_pflags & P1_HP_BONUS_APPLIES_WHEN_CARRIED)
                        Sprintf(eos(pwbuf), " when carried");
                    else
                        Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");

                    if (objects[otyp].oc_pflags & P1_HP_BONUS_APPLIES_TO_ALL_CHARACTERS)
                        Sprintf(eos(pwbuf), " (applies to all)");
                    else if (objects[otyp].oc_pflags & P1_HP_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
                        Sprintf(eos(pwbuf), " (applies to inappropriate only)");
                    else if (objects[otyp].oc_pflags & P1_HP_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
                        Sprintf(eos(pwbuf), " (negative to inappropriate)");
                }
                else if (j == 5)
                {
                    if (objects[otyp].oc_pflags & P1_MANA_BONUS_APPLIES_WHEN_CARRIED)
                        Sprintf(eos(pwbuf), " when carried");
                    else
                        Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");

                    if (objects[otyp].oc_pflags & P1_MANA_BONUS_APPLIES_TO_ALL_CHARACTERS)
                        Sprintf(eos(pwbuf), " (applies to all)");
                    else if (objects[otyp].oc_pflags & P1_MANA_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
                        Sprintf(eos(pwbuf), " (applies to inappropriate only)");
                    else if (objects[otyp].oc_pflags & P1_MANA_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
                        Sprintf(eos(pwbuf), " (negative to inappropriate)");
                }
                else if (j == 6)
                {
                    if (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_APPLIES_WHEN_CARRIED)
                        Sprintf(eos(pwbuf), " when carried");
                    else
                        Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");

                    if (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_APPLIES_TO_ALL_CHARACTERS)
                        Sprintf(eos(pwbuf), " (applies to all)");
                    else if (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
                        Sprintf(eos(pwbuf), " (applies to inappropriate only)");
                    else if (objects[otyp].oc_pflags & P1_ATTRIBUTE_BONUS_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
                        Sprintf(eos(pwbuf), " (negative to inappropriate)");
                }

                if (prop > 0)
                {
                    if (j < 6)
                    {
                        powercnt++;
                        Strcpy(buf2, "");
                        Sprintf(buf3, " %2d - ", powercnt);
                    }
                    if (j <= 3)
                    {
                        struct propname pn = get_property_name_ex(prop);
                        boolean haspdesc = pn.prop_desc != 0;
                        Sprintf(buf2, "%s%s%s%s", pn.prop_noun ? pn.prop_noun : "", haspdesc ? " (" : "", haspdesc ? pn.prop_desc : "", haspdesc ? ")" : "");
                        //Strcpy(buf2, get_property_name(prop));
                        *buf2 = highc(*buf2);
                    }
                    else if (j == 4)
                    {
                        if (objects[otyp].oc_pflags & P1_MANA_PERCENTAGE_BONUS)
                        {
                            Sprintf(buf2, "Mana pool is increased by %d%%", prop);
                        }
                        else
                        {
                            Sprintf(buf2, "Mana pool increased by %d", prop);
                        }
                    }
                    else if (j == 5)
                    {
                        if (objects[otyp].oc_pflags & P1_HP_PERCENTAGE_BONUS)
                        {
                            Sprintf(buf2, "Maximum hit points are increased by %d%%", prop);
                        }
                        else
                        {
                            Sprintf(buf2, "Maximum hit points increased by %d", prop);
                        }
                    }
                    else if (j == 6)
                    {
                        int k;
                        for (k = 0; k < 14; k++)
                        {
                            Strcpy(buf2, "");
                            int stat = (int)(k == 9 && !(prop & FULL_MC_BONUS) ? /* MC */ objects[otyp].oc_attribute_bonus / 3 : objects[otyp].oc_attribute_bonus);

                            if (obj && obj->cursed && (objects[otyp].oc_pflags & P1_CURSED_ITEM_YIELDS_NEGATIVE))
                                stat = -stat;

                            if (obj && objects[otyp].oc_enchantable && !(prop & IGNORE_ENCHANTMENT))
                            {
                                if (k == 9 && !(prop & FULL_MC_BONUS)) /* MC*/
                                    stat += obj->enchantment / 3;
                                else
                                    stat += obj->enchantment;
                            }
                            if (prop & SETS_FIXED_ATTRIBUTE)
                                stat = min((k == 0 ? STR19(25) : 25), max(1, stat));

                            char raisebuf[BUFSZ];
                            if (prop & FIXED_IS_MAXIMUM)
                                Strcpy(raisebuf, "Lowers");
                            else
                                Strcpy(raisebuf, "Raises");

                            char grantbuf[BUFSZ];
                            if (stat < 0)
                                Strcpy(grantbuf, "Causes");
                            else
                                Strcpy(grantbuf, "Grants");

                            char bonusbuf[BUFSZ];
                            if (stat < 0)
                                Strcpy(bonusbuf, "penalty");
                            else
                                Strcpy(bonusbuf, "bonus");

                            if (k == 0 && (prop & BONUS_TO_STR))
                            {
                                powercnt++;

                                if (prop & SETS_FIXED_ATTRIBUTE)
                                {
                                    char strbuf[BUFSZ] = "";
                                    if (stat <= 18)
                                        Sprintf(strbuf, "%d", stat);
                                    else if (stat < STR18(10))
                                        Sprintf(strbuf, "18/0%d", stat - 18);
                                    else if (stat < STR18(100))
                                        Sprintf(strbuf, "18/%d", stat - 18);
                                    else if (stat == STR18(100))
                                        Sprintf(strbuf, "18/**");
                                    else
                                        Sprintf(strbuf, "%d", stat - 100);
                                    Sprintf(buf2, "%s strength to %s", raisebuf, strbuf);
                                }
                                else
                                    Sprintf(buf2, "%s %s%d %s to strength", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 1 && (prop & BONUS_TO_DEX))
                            {
                                powercnt++;

                                if (prop & SETS_FIXED_ATTRIBUTE)
                                    Sprintf(buf2, "%s dexterity to %d", raisebuf, stat);
                                else
                                    Sprintf(buf2, "%s %s%d %s to dexterity", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 2 && (prop & BONUS_TO_CON))
                            {
                                powercnt++;

                                if (prop & SETS_FIXED_ATTRIBUTE)
                                    Sprintf(buf2, "%s constitution to %d", raisebuf, stat);
                                else
                                    Sprintf(buf2, "%s %s%d %s to constitution", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 3 && (prop & BONUS_TO_INT))
                            {
                                powercnt++;

                                if (prop & SETS_FIXED_ATTRIBUTE)
                                    Sprintf(buf2, "%s intelligence to %d", raisebuf, stat);
                                else
                                    Sprintf(buf2, "%s %s%d %s to intelligence", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 4 && (prop & BONUS_TO_WIS))
                            {
                                powercnt++;

                                if (prop & SETS_FIXED_ATTRIBUTE)
                                    Sprintf(buf2, "%s wisdom to %d", raisebuf, stat);
                                else
                                    Sprintf(buf2, "%s %s%d %s to wisdom", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 5 && (prop & BONUS_TO_CHA))
                            {
                                powercnt++;

                                if (prop & SETS_FIXED_ATTRIBUTE)
                                    Sprintf(buf2, "%s charisma to %d", raisebuf, stat);
                                else
                                    Sprintf(buf2, "%s %s%d %s to charisma", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 6 && (prop & BONUS_TO_AC))
                            {
                                powercnt++;

                                Sprintf(buf2, "%s %s%d %s to armor class", grantbuf, -stat >= 0 ? "+" : "", -stat, bonusbuf);
                                knownacbonus += stat;
                            }
                            if (k == 7 && (prop & BONUS_TO_DAMAGE))
                            {
                                powercnt++;

                                Sprintf(buf2, "%s %s%d %s to damage", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 8 && (prop & BONUS_TO_HIT))
                            {
                                powercnt++;

                                Sprintf(buf2, "%s %s%d %s to hit", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 9 && (prop & BONUS_TO_MC))
                            {
                                powercnt++;

                                Sprintf(buf2, "%s %s%d %s to magic cancellation", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                                knownmcbonus += stat;
                            }
                            if (k == 10 && (prop & BONUS_TO_UNRESTRICTED_SPELL_CASTING))
                            {
                                powercnt++;

                                Sprintf(buf2, "%s %s%d%% %s to casting unrestricted school spells", grantbuf, stat >= 0 ? "+" : "", stat * 5, bonusbuf);
                            }
                            if (k == 11 && (prop & BONUS_TO_EXPERIENCE))
                            {
                                powercnt++;

                                Sprintf(buf2, "%s %s%d%% %s to experience", grantbuf, stat >= 0 ? "+" : "", stat * 10, bonusbuf);
                            }
                            if (k == 12 && (prop & BONUS_TO_ARCHERY))
                            {
                                powercnt++;

                                Sprintf(buf2, "%s %s%d %s to hit and damage of archery weapons", grantbuf, stat >= 0 ? "+" : "", stat, bonusbuf);
                            }
                            if (k == 13 && (prop & BONUS_TO_ALL_SPELL_CASTING))
                            {
                                powercnt++;
                                Sprintf(buf2, "%s %s%d%% %s to casting all spells", grantbuf, stat >= 0 ? "+" : "", stat * 5, bonusbuf);
                            }

                            if (strcmp(buf2, "") != 0) // Something else than ""
                            {
                                Sprintf(buf3, " %2d - ", powercnt);
                                Sprintf(buf, "%s%s%s", buf3, buf2, pwbuf);
                                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                            }

                        }
                    }

                    if (j < 6)
                    {
                        if (strcmp(buf2, "") != 0) // Something else than ""
                        {
                            Sprintf(buf, "%s%s%s", buf3, buf2, pwbuf);
                            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                        }
                    }
                }
            }

            /* Power-like flags here */
            if (objects[otyp].oc_pflags & P1_CONFERS_LUCK)
            {
                char pwbuf[BUFSZ] = "";
                if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_WHEN_CARRIED)
                    Sprintf(eos(pwbuf), " when carried");
                else
                    Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");

                if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_ALL_CHARACTERS)
                    Sprintf(eos(pwbuf), " (applies to all)");
                else if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
                    Sprintf(eos(pwbuf), " (applies to inappropriate only)");
                else if (objects[otyp].oc_pflags & P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
                    Sprintf(eos(pwbuf), " (negative to inappropriate)");

                powercnt++;
                Sprintf(buf, " %2d - Confers luck%s", powercnt, pwbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_pflags & P1_CONFERS_UNLUCK)
            {
                char pwbuf[BUFSZ] = "";
                if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_WHEN_CARRIED)
                    Sprintf(eos(pwbuf), " when carried");
                else
                    Sprintf(eos(pwbuf), " when %s", is_otyp_wieldable_weapon(otyp) ? "wielded" : "worn");

                if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_ALL_CHARACTERS)
                    Sprintf(eos(pwbuf), " (applies to all)");
                else if (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)
                    Sprintf(eos(pwbuf), " (applies to inappropriate only)");
                else if (objects[otyp].oc_pflags & P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)
                    Sprintf(eos(pwbuf), " (negative to inappropriate)");

                powercnt++;
                Sprintf(buf, " %2d - Confers bad luck%s", powercnt, pwbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_DEALS_DOUBLE_DAMAGE_TO_PERMITTED_TARGETS))
            {
                powercnt++;
                Sprintf(buf, " %2d - Deals double damage on hit", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                //wep_all_extra_avg_dmg += wep_multipliable_avg_dmg;
            }
            if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_SVB_MASK) == A1_VORPAL)
            {
                powercnt++;
                Sprintf(buf, " %2d - Beheads target on hit", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_SVB_MASK) == A1_BISECT)
            {
                powercnt++;
                Sprintf(buf, " %2d - Bisects target on hit", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_SVB_MASK) == A1_SHARPNESS)
            {
                powercnt++;
                Sprintf(buf, " %2d - %d%% chance of damage equal to %d%% of target's maximum hit points", powercnt, SHARPNESS_PERCENTAGE_CHANCE, SHARPNESS_MAX_HP_PERCENTAGE_DAMAGE);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            char mcapplybuf[BUFSZ] = "";
            if (!(objects[otyp].oc_aflags & (A1_BYPASSES_MC)) || (objects[otyp].oc_aflags & A1_MAGIC_RESISTANCE_PROTECTS))
            {
                Strcpy(mcapplybuf, " (");
                if (!(objects[otyp].oc_aflags & A1_BYPASSES_MC) && (objects[otyp].oc_aflags & A1_MAGIC_RESISTANCE_PROTECTS))
                    Strcat(mcapplybuf, "MC and MR checks apply");
                else if (objects[otyp].oc_aflags & A1_MAGIC_RESISTANCE_PROTECTS)
                    Strcat(mcapplybuf, "MR check applies");
                else
                    Strcat(mcapplybuf, "MC check applies");
                Strcat(mcapplybuf, ")");
            }

            if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_CRITICAL_STRIKE) && (objects[otyp].oc_aflags & A1_CRITICAL_STRIKE_IS_DEADLY))
            {
                powercnt++;
                int critchance = objects[otyp].oc_critical_strike_percentage;
                char chancebuf[BUFSZ] = "";
                if (critchance < 0)
                {
                    Sprintf(chancebuf, " at chance equal to %s", critical_strike_special_percentage_names[min(MAX_CRITICAL_STRIKE_SPECIAL_PERCENTAGES, -1 * critchance) - 1]);
                }
                else
                    Sprintf(chancebuf, " at %d%% chance", critchance);

                if ((objects[otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)
                    Sprintf(buf, " %2d - Disintegrates the target on hit%s%s", powercnt, critchance < 100 ? chancebuf : "", mcapplybuf);
                else if ((objects[otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK)
                    Sprintf(buf, " %2d - Slays the target on hit%s%s", powercnt, critchance < 100 ? chancebuf : "", mcapplybuf);
                else
                    Sprintf(buf, " %2d - Causes lethal %s damage%s%s", powercnt,
                        get_damage_type_text((objects[otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_USES_EXTRA_DAMAGE_TYPE) ? objects[otyp].oc_extra_damagetype : AD_PHYS),
                        critchance < 100 ? chancebuf : "", mcapplybuf);
                
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

                if (objects[otyp].oc_aflags2 & A2_REQUIRES_ARTIFACT_INVOKE_ON)
                {
                    Strcpy(buf, "      * Active when invoked");
                    putstr(datawin, ATR_INDENT_AT_ASTR, buf);
                }
                if (objects[otyp].oc_aflags2 & A2_REQUIRES_AND_EXPENDS_A_CHARGE)
                {
                    Strcpy(buf, "      * Expends a charge on successful hit");
                    putstr(datawin, ATR_INDENT_AT_ASTR, buf);
                }
            }
            if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_LEVEL_DRAIN))
            {
                powercnt++;
                Sprintf(buf, " %2d - Drains a level on hit%s", powercnt, mcapplybuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_LIFE_LEECH))
            {
                powercnt++;
                Sprintf(buf, " %2d - Transfers hit points to the wielder on hit", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (!uses_spell_flags && (objects[otyp].oc_aflags & A1_WOUNDING))
            {
                powercnt++;
                Sprintf(buf, " %2d - Reduces hit points permanently on hit", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_flags3 & O3_PREVENTS_REVIVAL_OF_PERMITTED_TARGETS)
            {
                powercnt++;
                Sprintf(buf, " %2d - Prevents revival of permitted targets", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_flags3 & O3_PREVENTS_SUMMONING_BY_PERMITTED_TARGETS)
            {
                powercnt++;
                Sprintf(buf, " %2d - Prevents summoning by permitted targets", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_flags4 & O4_EXTENDED_POLEARM_REACH)
            {
                powercnt++;
                Sprintf(buf, " %2d - Extended reach", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_flags3 & O3_DOUBLE_DIGGING_EFFORT)
            {
                powercnt++;
                Sprintf(buf, " %2d - Double digging or cutting speed", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }

            if (powercnt == 0)
            {
                Sprintf(buf, " (None)");
                putstr(datawin, 0, buf);
            }

        }

        /* Power confer limitations */
        if (objects[otyp].oc_power_permissions)
        {
            int powercnt = 0;

            Sprintf(buf, "%s conferred only to:", has_conferred_powers && has_extra_damage ? "Powers and extra damage are" : has_extra_damage ? "Extra damage is" : "Powers are");
            putstr(datawin, ATR_HEADING, buf);

            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_ARCHAEOLOGIST)
            {
                powercnt++;
                Sprintf(buf, " %2d - Archaeologists", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_BARBARIAN)
            {
                powercnt++;
                Sprintf(buf, " %2d - Barbarians", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_CAVEMAN)
            {
                powercnt++;
                Sprintf(buf, " %2d - Cavemen and cavewomen", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_HEALER)
            {
                powercnt++;
                Sprintf(buf, " %2d - Healers", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_KNIGHT)
            {
                powercnt++;
                Sprintf(buf, " %2d - Knights", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_MONK)
            {
                powercnt++;
                Sprintf(buf, " %2d - Monks", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_PRIEST)
            {
                powercnt++;
                Sprintf(buf, " %2d - Priests and priestesses", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_RANGER)
            {
                powercnt++;
                Sprintf(buf, " %2d - Rangers", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_ROGUE)
            {
                powercnt++;
                Sprintf(buf, " %2d - Rogues", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_SAMURAI)
            {
                powercnt++;
                Sprintf(buf, " %2d - Samurais", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_TOURIST)
            {
                powercnt++;
                Sprintf(buf, " %2d - Tourists", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_VALKYRIE)
            {
                powercnt++;
                Sprintf(buf, " %2d - Valkyries", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ROLE_WIZARD)
            {
                powercnt++;
                Sprintf(buf, " %2d - Wizards", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_RACE_DWARF)
            {
                powercnt++;
                Sprintf(buf, " %2d - Dwarves", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_RACE_ELF)
            {
                powercnt++;
                Sprintf(buf, " %2d - Elves", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_RACE_GNOLL)
            {
                powercnt++;
                Sprintf(buf, " %2d - Gnolls", powercnt);
                
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_RACE_GNOME)
            {
                powercnt++;
                Sprintf(buf, " %2d - Gnomes", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_RACE_HUMAN)
            {
                powercnt++;
                Sprintf(buf, " %2d - Humans", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_RACE_ORC)
            {
                powercnt++;
                Sprintf(buf, " %2d - Orcs", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_GENDER_FEMALE)
            {
                powercnt++;
                Sprintf(buf, " %2d - Females", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_GENDER_MALE)
            {
                powercnt++;
                Sprintf(buf, " %2d - Males", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ALIGNMENT_CHAOTIC)
            {
                powercnt++;
                Sprintf(buf, " %2d - Chaotic", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ALIGNMENT_LAWFUL)
            {
                powercnt++;
                Sprintf(buf, " %2d - Lawful", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_power_permissions & PERMITTED_ALIGNMENT_NEUTRAL)
            {
                powercnt++;
                Sprintf(buf, " %2d - Neutral", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (powercnt == 0)
            {
                Sprintf(buf, " (None)");
                putstr(datawin, 0, buf);
            }

        }

        /* Target permissions */
        if (objects[otyp].oc_target_permissions != ALL_TARGETS || (objects[otyp].oc_flags3 & (O3_PERMTTED_TARGET_CHAOTIC | O3_PERMTTED_TARGET_NEUTRAL | O3_PERMTTED_TARGET_LAWFUL)))
        {
            int powercnt = 0;

            Sprintf(buf, "Permitted targets:");
            putstr(datawin, ATR_HEADING, buf);

            /* Flags here */
            if (objects[otyp].oc_target_permissions != ALL_TARGETS)
            {
                if (objects[otyp].oc_flags3 & (O3_TARGET_PERMISSION_IS_M1_FLAG | O3_TARGET_PERMISSION_IS_M2_FLAG | O3_TARGET_PERMISSION_IS_M3_FLAG | O3_TARGET_PERMISSION_IS_M4_FLAG | O3_TARGET_PERMISSION_IS_M5_FLAG))
                {
                    int flag_idx = 1;
                    if (objects[otyp].oc_flags3 & (O3_TARGET_PERMISSION_IS_M2_FLAG))
                        flag_idx = 2;
                    else if (objects[otyp].oc_flags3 & (O3_TARGET_PERMISSION_IS_M3_FLAG))
                        flag_idx = 3;
                    else if (objects[otyp].oc_flags3 & (O3_TARGET_PERMISSION_IS_M4_FLAG))
                        flag_idx = 4;
                    else if (objects[otyp].oc_flags3 & (O3_TARGET_PERMISSION_IS_M5_FLAG))
                        flag_idx = 5;

                    int idx;
                    for (idx = 0; idx < NUM_UNSIGNED_LONG_BITS; idx++)
                    {
                        uint64_t bit = 1;
                        if (idx > 0)
                            bit = bit << idx;

                        if (objects[otyp].oc_target_permissions & bit)
                        {
                            const char* flagname = get_mflag_description(bit, TRUE, flag_idx);
                            if (flagname && strcmp(flagname, ""))
                            {
                                char flagbuf[BUFSZ];
                                Strcpy(flagbuf, flagname);
                                *flagbuf = highc(*flagbuf);
                                powercnt++;
                                Sprintf(buf, " %2d - %s", powercnt, flagbuf);
                                
                                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                            }
                        }
                    }
                }
                else if (objects[otyp].oc_target_permissions > 0 && objects[otyp].oc_target_permissions < MAX_MONSTER_CLASSES)
                {
                    char monsymbuf[BUFSZ];
                    Strcpy(monsymbuf, def_monsyms[objects[otyp].oc_target_permissions].name);
                    *monsymbuf = highc(*monsymbuf);

                    powercnt++;
                    Sprintf(buf, " %2d - %s", powercnt, monsymbuf);
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                }
            }

            if (objects[otyp].oc_flags3 & O3_PERMTTED_TARGET_CHAOTIC)
            {
                powercnt++;
                Sprintf(buf, " %2d - Chaotic creatures", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_flags3 & O3_PERMTTED_TARGET_NEUTRAL)
            {
                powercnt++;
                Sprintf(buf, " %2d - Neutral creatures", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (objects[otyp].oc_flags3 & O3_PERMTTED_TARGET_LAWFUL)
            {
                powercnt++;
                Sprintf(buf, " %2d - Lawful creatures", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (powercnt == 0)
            {
                Sprintf(buf, " (None)");
                putstr(datawin, 0, buf);
            }
        }

        /* Mythic power descriptions for the item. */
        if (obj && stats_known && obj->mknown && (obj->mythic_prefix || obj->mythic_suffix))
        {
            char mythicbuf[BUFSZ] = "";

            if (obj->mythic_prefix)
                Strcpy(mythicbuf, mythic_prefix_qualities[obj->mythic_prefix].name);

            if (obj->mythic_suffix)
            {
                if (obj->mythic_prefix)
                    Strcat(mythicbuf, " and ");

                Strcat(mythicbuf, mythic_suffix_qualities[obj->mythic_suffix].name);
            }

            *mythicbuf = highc(*mythicbuf);

            Sprintf(buf, "Mythic powers - %s:", mythicbuf);
            putstr(datawin, ATR_HEADING, buf);

            int powercnt = 0;
            char mbuf[BUFSZ];
            if (obj->mythic_prefix)
            {
                for (i = 0; i < MAX_MYTHIC_PREFIX_POWERS; i++)
                {
                    uint64_t bit = (uint64_t)1 << (uint64_t)i;
                    if (mythic_prefix_powers[i].description && (mythic_prefix_qualities[obj->mythic_prefix].mythic_powers & bit) && mythic_power_applies_to_obj(obj, mythic_prefix_powers[i].power_flags) && strcmp(mythic_prefix_powers[i].description, ""))
                    {
                        powercnt++;
                        Strcpy(mbuf, mythic_prefix_powers[i].description);
                        if (mythic_prefix_powers[i].power_type == MYTHIC_POWER_TYPE_SLAYING)
                        {
                            if (index(mythic_prefix_powers[i].description, '%'))
                            {
                                double mdmgmult = mythic_prefix_powers[i].parameter2 * (is_ammo(obj) || is_missile(obj) ? 2.0 : 1.0);
                                Sprintf(mbuf, mythic_prefix_powers[i].description, mdmgmult);
                                wep_all_extra_avg_dmg += wep_multipliable_avg_dmg * (mdmgmult - 1);
                            }
                        }

                        convert_dice_to_ranges(mbuf);
                        Sprintf(buf, " %2d - %s", powercnt, mbuf);
                        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                    }
                    if (!mythic_prefix_powers[i].description)
                        break;
                }
            }
            if (obj->mythic_suffix)
            {
                for (i = 0; i < MAX_MYTHIC_SUFFIX_POWERS; i++)
                {
                    uint64_t bit = (uint64_t)1 << (uint64_t)i;
                    if (mythic_suffix_powers[i].description && (mythic_suffix_qualities[obj->mythic_suffix].mythic_powers & bit) && mythic_power_applies_to_obj(obj, mythic_suffix_powers[i].power_flags) && strcmp(mythic_suffix_powers[i].description, ""))
                    {
                        powercnt++;
                        Strcpy(mbuf, mythic_suffix_powers[i].description);
                        if (mythic_suffix_powers[i].power_type == MYTHIC_POWER_TYPE_SLAYING)
                        {
                            if (index(mythic_suffix_powers[i].description, '%'))
                            {
                                double mdmgmult = mythic_suffix_powers[i].parameter2 * (is_ammo(obj) || is_missile(obj) ? 2.0 : 1.0);
                                Sprintf(mbuf, mythic_suffix_powers[i].description, mythic_suffix_powers[i].parameter2 * (is_ammo(obj) || is_missile(obj) ? 2.0 : 1.0));
                                wep_all_extra_avg_dmg += wep_multipliable_avg_dmg * (mdmgmult - 1);
                            }
                        }

                        convert_dice_to_ranges(mbuf);
                        Sprintf(buf, " %2d - %s", powercnt, mbuf);                        
                        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                    }
                    if (!mythic_suffix_powers[i].description)
                        break;
                }
            }
        }
    }

    /* General for all items */
    boolean show_corpse_hint = FALSE;
    if(stats_known)
    {
        uint64_t ocflags = obj ? get_obj_oc_flags(obj) : objects[otyp].oc_flags;
        uint64_t ocflags5 = obj ? get_obj_oc_flags5(obj) : objects[otyp].oc_flags5;
        uint64_t ocflags6 = obj ? get_obj_oc_flags6(obj) : objects[otyp].oc_flags6;
        /* Item properties */
        if (ocflags & ~(O1_THROWN_WEAPON_ONLY | O1_MELEE_AND_THROWN_WEAPON
            | O1_SPELLTOOL | O1_NON_SPELL_SPELLBOOK | O1_EDIBLE_NONFOOD) 
            || (ocflags5 & (O5_MBAG_DESTROYING_ITEM | O5_CANCELLATION_NO_EXPLOSION_BUT_DRAIN | O5_PERMANENTLY_GREASED))
            || (ocflags6 & (O6_THROWING_REQUIRES_STR_18_00))
            || otyp_shines_magical_light(otyp)
            || (obj && has_obj_mythic_fire_resistance(obj)) || (obj && has_obj_mythic_cold_resistance(obj)) || (obj && has_obj_mythic_shock_resistance(obj))
            || is_otyp_special_praying_item(otyp) || otyp_consumes_nutrition_every_20_rounds(otyp)
            || (obj ? is_death_enchantable(obj) : is_otyp_death_enchantable(otyp)) //|| is_otyp_elemental_enchantable(otyp) 
            )
        {
            int powercnt = 0;

            Sprintf(buf, "Item properties:");
            putstr(datawin, ATR_HEADING, buf);

            /* Flags here */
            if (ocflags & O1_INDESTRUCTIBLE)
            {
                powercnt++;
                Sprintf(buf, " %2d - Indestructible", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_DISINTEGRATION_RESISTANT)
            {
                powercnt++;
                Sprintf(buf, " %2d - Disintegration resistant", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if ((ocflags & O1_FIRE_RESISTANT) != 0 || (obj && has_obj_mythic_fire_resistance(obj)))
            {
                powercnt++;
                Sprintf(buf, " %2d - Fire resistant", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if ((ocflags & O1_COLD_RESISTANT) != 0 || (obj && has_obj_mythic_cold_resistance(obj)))
            {
                powercnt++;
                Sprintf(buf, " %2d - Cold resistant", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if ((ocflags & O1_LIGHTNING_RESISTANT) != 0 || (obj && has_obj_mythic_shock_resistance(obj)))
            {
                powercnt++;
                Sprintf(buf, " %2d - Lightning resistant", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_CORROSION_RESISTANT)
            {
                powercnt++;
                Sprintf(buf, " %2d - Corrosion resistant", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_RUST_RESISTANT)
            {
                powercnt++;
                Sprintf(buf, " %2d - Rust-proof", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_ROT_RESISTANT)
            {
                powercnt++;
                Sprintf(buf, " %2d - Rot-resistant", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_POLYMORPH_RESISTANT)
            {
                powercnt++;
                Sprintf(buf, " %2d - Polymorph resistant", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }

            if (obj ? is_death_enchantable(obj) : is_otyp_death_enchantable(otyp))
            {
                powercnt++;
                Sprintf(buf, " %2d - Death-magically enchantable", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }

            if (ocflags & O1_NOT_CURSEABLE)
            {
                powercnt++;
                Sprintf(buf, " %2d - Cannot be cursed", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_BECOMES_CURSED_WHEN_PICKED_UP_AND_DROPPED)
            {
                powercnt++;
                Sprintf(buf, " %2d - Becomes cursed when picked up", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_BECOMES_CURSED_WHEN_WORN)
            {
                powercnt++;
                Sprintf(buf, " %2d - Becomes cursed when worn", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_CANNOT_BE_DROPPED_IF_CURSED)
            {
                powercnt++;
                Sprintf(buf, " %2d - Undroppable when cursed", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }

            if (ocflags & O1_IS_ARMOR_WHEN_WIELDED)
            {
                powercnt++;
                Sprintf(buf, " %2d - Is armor when wielded", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_IS_WEAPON_WHEN_WIELDED)
            {
                powercnt++;
                Sprintf(buf, " %2d - Is weapon when wielded", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags5 & O5_IS_WEAPON_WHEN_WORN)
            {
                powercnt++;
                Sprintf(buf, " %2d - Is weapon when worn", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_RETURNS_TO_HAND_AFTER_THROWING)
            {
                powercnt++;
                Sprintf(buf, " %2d - Returns to hand after throwing", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_WEIGHT_DOES_NOT_REDUCE_RANGE)
            {
                powercnt++;
                Sprintf(buf, " %2d - Weight does not reduce throwing range", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags & O1_CAN_BE_THROWN_ONLY_IF_WIELDED)
            {
                powercnt++;
                Sprintf(buf, " %2d - Can be thrown only if wielded", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags6 & O6_THROWING_REQUIRES_STR_18_00)
            {
                powercnt++;
                Sprintf(buf, " %2d - Throwing requires strength of 18/** or higher", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }

            if (otyp_shines_magical_light(otyp))
            {
                powercnt++;
                Sprintf(buf, " %2d - Shines magical light when wielded", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (is_otyp_special_praying_item(otyp))
            {
                powercnt++;
                Sprintf(buf, " %2d - If blessed, shimmers when it is safe to pray", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (otyp_consumes_nutrition_every_20_rounds(otyp))
            {
                powercnt++;
                Sprintf(buf, " %2d - Consumes nutrition every 20 rounds when worn", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags5 & O5_MBAG_DESTROYING_ITEM)
            {
                powercnt++;
                Sprintf(buf, " %2d - Destroys magic bags if put in", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags5 & O5_CANCELLATION_NO_EXPLOSION_BUT_DRAIN)
            {
                powercnt++;
                Sprintf(buf, " %2d - Strips magic bag destroying items of charges; does not explode", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (ocflags5 & O5_PERMANENTLY_GREASED)
            {
                powercnt++;
                Sprintf(buf, " %2d - Permanently greased", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }

#if 0
            if (is_otyp_elemental_enchantable(otyp))
            {
                powercnt++;
                Sprintf(buf, " %2d - Can be elemental-enchanted", powercnt);
                
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
#endif
            if (powercnt == 0)
            {
                Sprintf(buf, " (None)");
                putstr(datawin, 0, buf);
            }
        }

        /* Corpse properties */
        if (obj && is_obj_rotting_corpse(obj) && obj->corpsenm > NON_PM && obj->corpsenm < NUM_MONSTERS)
        {
            if (mvitals[obj->corpsenm].mvflags & MV_KNOWS_CORPSE)
            {
                Sprintf(buf, "Corpse properties:");
                putstr(datawin, ATR_HEADING, buf);

                print_corpse_properties(datawin, obj->corpsenm);
            }
            else if ((context.game_difficulty <= 0 || flags.force_hint) && flags.max_hint_difficulty >= MIN_DIFFICULTY_LEVEL) /* Shows up on expert, too, just in case, since this is GnollHack-specific */
            {
                show_corpse_hint = TRUE;
            }
        }
    }

    /* Artifact powers */
    if (obj && obj->oartifact && stats_known)
    {
        int powercnt = 0;
        Sprintf(buf, "Artifact properties:");
        putstr(datawin, ATR_HEADING, buf);

        const char* alingstr = align_str(artilist[obj->oartifact].alignment);
        powercnt++;
        Sprintf(buf, " %2d - Alignment is %s", powercnt, alingstr);
        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

        if (artilist[obj->oartifact].role > NON_PM)
        {
            powercnt++;
            Sprintf(buf, " %2d - Artifact is associated with %s", powercnt, pm_plural_name(&mons[artilist[obj->oartifact].role], 4));
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (artilist[obj->oartifact].race > NON_PM)
        {
            powercnt++;
            Sprintf(buf, " %2d - Artifact is associated with %s", powercnt, pm_plural_name(&mons[artilist[obj->oartifact].race], 4));
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (artilist[obj->oartifact].attk.damn != 0 || artilist[obj->oartifact].attk.damd != 0 || artilist[obj->oartifact].attk.damp != 0)
        {
            char dmgttext[BUFSZ] = "";
            Strcpy(dmgttext, get_damage_type_text((short)artilist[obj->oartifact].attk.adtyp));
            powercnt++;
            Sprintf(buf, " %2d - Artifact damage type is %s damage", powercnt, dmgttext);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (artilist[obj->oartifact].tohit_dice != 0 || artilist[obj->oartifact].tohit_diesize != 0 || artilist[obj->oartifact].tohit_plus != 0)
        {
            char tohitbuf[BUFSZ] = "";
            printdice(plusbuf, artilist[obj->oartifact].tohit_dice, artilist[obj->oartifact].tohit_diesize, artilist[obj->oartifact].tohit_plus);
            Sprintf(tohitbuf, "Artifact to-hit bonus is %s", plusbuf);

            //Strcpy(plusbuf, "");
            //if (artilist[obj->oartifact].tohit_plus != 0)
            //    Sprintf(plusbuf, "%s%d", artilist[obj->oartifact].tohit_plus >= 0 ? "+" : "", artilist[obj->oartifact].tohit_plus);

            //if (artilist[obj->oartifact].tohit_dice > 0 && artilist[obj->oartifact].tohit_diesize > 0)
            //{
            //    Sprintf(tohitbuf, "Artifact to-hit bonus is %dd%d%s", artilist[obj->oartifact].tohit_dice, artilist[obj->oartifact].tohit_diesize, plusbuf);
            //}
            //else
            //    Sprintf(tohitbuf, "Artifact to-hit bonus is %s", plusbuf);

            powercnt++;
            Sprintf(buf, " %2d - %s", powercnt, tohitbuf);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (artilist[obj->oartifact].attk.damn != 0 || artilist[obj->oartifact].attk.damd != 0 || artilist[obj->oartifact].attk.damp != 0)
        {
            char dmgbuf[BUFSZ] = "";
            printdice(plusbuf, artilist[obj->oartifact].attk.damn, artilist[obj->oartifact].attk.damd, artilist[obj->oartifact].attk.damp);
            //Strcpy(plusbuf, "");
            //if (artilist[obj->oartifact].attk.damp != 0)
            //    Sprintf(plusbuf, "%s%d", artilist[obj->oartifact].attk.damp >= 0 ? "+" : "", artilist[obj->oartifact].attk.damp);

            //if (artilist[obj->oartifact].attk.damn > 0 && artilist[obj->oartifact].attk.damd > 0)
            //{
            //    Sprintf(dmgbuf, "Artifact damage bonus is %dd%d%s", artilist[obj->oartifact].attk.damn, artilist[obj->oartifact].attk.damd, plusbuf);
            //}
            //else 
            if (artilist[obj->oartifact].attk.damn == ARTDMG_DOUBLE_DAMAGE)
                Sprintf(dmgbuf, "Artifact deals double normal damage");
            else if (artilist[obj->oartifact].attk.damn == ARTDMG_TRIPLE_DAMAGE)
                Sprintf(dmgbuf, "Artifact deals triple normal damage");
            else if (artilist[obj->oartifact].attk.damn == ARTDMG_QUADRUPLE_DAMAGE)
                Sprintf(dmgbuf, "Artifact deals quadruple normal damage");
            else if (artilist[obj->oartifact].attk.damn < 0)
                Sprintf(dmgbuf, "Artifact deals %.2f times normal damage", 1.0 - ((double)artilist[obj->oartifact].attk.damn) / 20.0);
            else
                Sprintf(dmgbuf, "Artifact damage bonus is %s", plusbuf);

            double art_avg_dmg = 0;
            if (artilist[obj->oartifact].attk.damn < 0)
                art_avg_dmg += wep_avg_dmg * (-((double)artilist[obj->oartifact].attk.damn) / 20.0);
            else
                art_avg_dmg += (double)artilist[obj->oartifact].attk.damn * (1.0 + (double)artilist[obj->oartifact].attk.damd) / 2.0 + (double)artilist[obj->oartifact].attk.damp;

            if (artilist[obj->oartifact].aflags & (AF_DMONS | AF_DCLAS | AF_DFLAG1 | AF_DFLAG2))
                wep_all_extra_avg_dmg += art_avg_dmg;
            else
            {
                wep_avg_dmg += art_avg_dmg;
            }
            if (wep_avg_dmg < 0)
                wep_avg_dmg = 0;

            powercnt++;
            Sprintf(buf, " %2d - %s", powercnt, dmgbuf);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        if (artilist[obj->oartifact].worn_prop > 0)
        {
            char defensetext[BUFSZ];
            char descrtext[BUFSZ];
            struct propname pn = get_property_name_ex(artilist[obj->oartifact].worn_prop);
            Strcpy(defensetext, pn.prop_noun ? pn.prop_noun : "");
            *defensetext = highc(*defensetext);
            if (pn.prop_desc)
                Sprintf(descrtext, " (%s)", pn.prop_desc);
            else
                Strcpy(descrtext, "");

            powercnt++;
            if (is_wieldable_weapon(obj))
                Sprintf(buf, " %2d - %s when wielded%s", powercnt, defensetext, descrtext);
            else
                Sprintf(buf, " %2d - %s when worn%s", powercnt, defensetext, descrtext);
            
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (artilist[obj->oartifact].carried_prop > 0)
        {
            char defensetext[BUFSZ];
            char descrtext[BUFSZ];
            struct propname pn = get_property_name_ex(artilist[obj->oartifact].carried_prop);
            Strcpy(defensetext, pn.prop_noun ? pn.prop_noun : "");
            *defensetext = highc(*defensetext);
            if (pn.prop_desc)
                Sprintf(descrtext, " (%s)", pn.prop_desc);
            else
                Strcpy(descrtext, "");

            powercnt++;
            Sprintf(buf, " %2d - %s when carried%s", powercnt, defensetext, descrtext);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (artilist[obj->oartifact].inv_prop > 0)
        {
            char invoketext[BUFSZ];
            char repowertext[BUFSZ];
            char descrtext[BUFSZ];
            Strcpy(repowertext, "");
            Strcpy(descrtext, "");

            if (artilist[obj->oartifact].inv_prop > LAST_PROP)
            {
                const char* invprop = get_artifact_invoke_name(artilist[obj->oartifact].inv_prop);
                Strcpy(invoketext, invprop);
                *invoketext = highc(*invoketext);
            }
            else
            {
                struct propname pn = get_property_name_ex(artilist[obj->oartifact].inv_prop);
                Strcpy(invoketext, pn.prop_noun ? pn.prop_noun : "");
                *invoketext = highc(*invoketext);
                if (pn.prop_desc)
                    Sprintf(descrtext, " (%s)", pn.prop_desc);
                else
                    Strcpy(descrtext, "");
            }

            powercnt++;
            Sprintf(buf, " %2d - %s when invoked%s", powercnt, invoketext, descrtext);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

            if (artilist[obj->oartifact].inv_prop <= LAST_PROP /* switchable property */
                ||
                ((artilist[obj->oartifact].inv_duration_dice > 0 && artilist[obj->oartifact].inv_duration_diesize > 0) ||artilist[obj->oartifact].inv_duration_plus > 0)
                )
            {
                if (artilist[obj->oartifact].inv_prop <= LAST_PROP
                    && artilist[obj->oartifact].inv_duration_dice == 0
                    && artilist[obj->oartifact].inv_duration_diesize == 0 
                    && artilist[obj->oartifact].inv_duration_plus == 0)
                {
                    Strcpy(buf, "      * Effect is switchable on and off");
                }
                else
                {
                    printdice(plusbuf, artilist[obj->oartifact].inv_duration_dice, artilist[obj->oartifact].inv_duration_diesize, artilist[obj->oartifact].inv_duration_plus);
                    Sprintf(buf, "      * Effect duration is %s", plusbuf);

                    //Strcpy(plusbuf, "");
                    //if (artilist[obj->oartifact].inv_duration_plus != 0)
                    //    Sprintf(plusbuf, "%s%d", artilist[obj->oartifact].inv_duration_plus >= 0 ? "+" : "", artilist[obj->oartifact].inv_duration_plus);

                    //if (artilist[obj->oartifact].inv_duration_dice > 0 && artilist[obj->oartifact].inv_duration_diesize > 0)
                    //{
                    //    Sprintf(buf, "      * Effect duration is %dd%d%s rounds", artilist[obj->oartifact].inv_duration_dice, artilist[obj->oartifact].inv_duration_diesize, plusbuf);
                    //}
                    //else
                    //    Sprintf(buf, "      * Effect duration is %s", plusbuf);
                }
                
                putstr(datawin, ATR_INDENT_AT_ASTR, buf);
            }

            if (artilist[obj->oartifact].inv_mana_cost > 0)
            {
                Sprintf(buf, "      * Mana cost is %d", artilist[obj->oartifact].inv_mana_cost);
                putstr(datawin, ATR_INDENT_AT_ASTR, buf);
            }

            if (artilist[obj->oartifact].aflags & AF_INVOKE_EXPENDS_CHARGE)
            {
                Sprintf(buf, "      * Expends one charge");
                putstr(datawin, ATR_INDENT_AT_ASTR, buf);
            }

            if (artilist[obj->oartifact].aflags & AF_INVOKE_REQUIRES_WORN)
            {
                Sprintf(buf, "      * Has to be worn for invocation");
                putstr(datawin, ATR_INDENT_AT_ASTR, buf);
            }

            if (artilist[obj->oartifact].repower_time > 0)
            {
                Sprintf(buf, "      * Repowers over %d round%s", artilist[obj->oartifact].repower_time, plur(artilist[obj->oartifact].repower_time));
                putstr(datawin, ATR_INDENT_AT_ASTR, buf);
            }

        }


        uint64_t aflags = artilist[obj->oartifact].aflags;
        uint64_t aflags2 = artilist[obj->oartifact].aflags2;
        uint64_t mtype = artilist[obj->oartifact].mtype;
        if (aflags & AF_BEHEAD)
        {
            powercnt++;
            Sprintf(buf, " %2d - Has 5%% chance to behead target on hit ", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_BISECT)
        {
            powercnt++;
            Sprintf(buf, " %2d - Has 5%% chance to  bisect target on hit", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_HITS_ADJACENT_SQUARES)
        {
            powercnt++;
            Sprintf(buf, " %2d - Hits also adjacent squares", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_BLOODTHIRSTY)
        {
            powercnt++;
            Sprintf(buf, " %2d - Bloodthirsty", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_PREVENTS_REVIVAL_OF_MON)
        {
            powercnt++;
            Sprintf(buf, " %2d - Prevents revival of %s", powercnt,
                ((aflags & AF_DFLAG2) && (mtype & M2_ORC)) ? "orcs" : 
                ((aflags & AF_DFLAG2) && (mtype & M2_ELF)) ? "elves" :
                ((aflags & AF_DFLAG2) && (mtype & M2_DEMON)) ? "demons" :
                ((aflags & AF_DFLAG2) && (mtype & M2_ANGEL)) ? "angels" :
                ((aflags & AF_DFLAG2) && (mtype & M2_UNDEAD)) ? "undead" :
                ((aflags & AF_DFLAG2) && (mtype & M2_GIANT)) ? "giants" :
                ((aflags & AF_DFLAG2) && (mtype & M2_WERE)) ? "lycanthropes" :
                ((aflags & AF_DCLAS) && mtype < MAX_MONSTER_CLASSES) ? def_monsyms[mtype].name :
                ((aflags & AF_DMONS) && mtype < NUM_MONSTERS) ? pm_plural_name(&mons[mtype], 1) :
                "an unknown type of monsters");
            
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_PREVENTS_SUMMONING_OF_MON)
        {
            powercnt++;
            Sprintf(buf, " %2d - Prevents summoning by %s", powercnt,
                ((aflags & AF_DFLAG2) && (mtype & M2_ORC)) ? "orcs" :
                ((aflags & AF_DFLAG2) && (mtype & M2_ELF)) ? "elves" :
                ((aflags & AF_DFLAG2) && (mtype & M2_DEMON)) ? "demons" :
                ((aflags & AF_DFLAG2) && (mtype & M2_ANGEL)) ? "angels" :
                ((aflags & AF_DFLAG2) && (mtype & M2_UNDEAD)) ? "undead" :
                ((aflags & AF_DFLAG2) && (mtype & M2_GIANT)) ? "giants" :
                ((aflags & AF_DFLAG2) && (mtype & M2_WERE)) ? "lycanthropes" :
                ((aflags & AF_DCLAS) && mtype < MAX_MONSTER_CLASSES) ? def_monsyms[mtype].name :
                ((aflags & AF_DMONS) && mtype < NUM_MONSTERS) ? pm_plural_name(&mons[mtype], 1) :
                "an unknown type of monsters");
            
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_MAGIC_ABSORBING)
        {
            powercnt++;
            Sprintf(buf, " %2d - Magic absorbing", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_MONSTERS_CAN_TRACK_ARTIFACT)
        {
            powercnt++;
            Sprintf(buf, " %2d - Monsters can track the artifact", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_ANGERS_DEMONS)
        {
            powercnt++;
            Sprintf(buf, " %2d - Angers demons on sight", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_SHINES_MAGICAL_LIGHT)
        {
            powercnt++;
            Sprintf(buf, " %2d - Shines magical light when wielded", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_FLICKERS)
        {
            powercnt++;
            Sprintf(buf, " %2d - Flickers in the precence of %s when wielded", powercnt , "applicable monsters");
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_FLICKERS_WHEN_CARRIED)
        {
            powercnt++;
            Sprintf(buf, " %2d - Flickers in the precence of %s when carried", powercnt, "applicable monsters");
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_INTEL)
        {
            powercnt++;
            Sprintf(buf, " %2d - Intelligent", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_SPEAK)
        {
            powercnt++;
            Sprintf(buf, " %2d - Speaking", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags & AF_DRLI)
        {
            powercnt++;
            Sprintf(buf, " %2d - Drains life levels on hit", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags2 & AF2_APPLICABLE_AS_AXE)
        {
            powercnt++;
            Sprintf(buf, " %2d - Can be applied as an axe", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (aflags2 & AF2_DUAL_RUNESWORD_BONUS)
        {
            powercnt++;
            Sprintf(buf, " %2d - Double enchantment and artifact bonuses if wielded with another runesword", powercnt);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }



        for (i = 1; i <= 2; i++)
        {
            uint64_t specialeffect = 0;
            Strcpy(endbuf, "");
            if (i == 1)
            {
                specialeffect = artilist[obj->oartifact].spfx;
                if (is_wieldable_weapon(obj))
                    Strcpy(endbuf, "when wielded");
                else
                    Strcpy(endbuf, "when worn");
            }
            else
            {
                specialeffect = artilist[obj->oartifact].cspfx;
                Strcpy(endbuf, "when carried");
            }
            int propnum = 0;
            int idx;
            for (idx = 0; idx < 32; idx++)
            {
                uint64_t bit = 1;
                if (idx > 0)
                    bit = bit << idx;
                propnum = spfx_to_prop(bit);
                if (propnum > 0 && (specialeffect & bit)
                    && bit != SPFX_WARN_OF_MON
                    && bit != SPFX_STR_25
                    && bit != SPFX_DEX_25
                    && bit != SPFX_CON_25
                    && bit != SPFX_INT_25
                    && bit != SPFX_WIS_25
                    && bit != SPFX_CHA_25
                    )
                {
                    struct propname pn = get_property_name_ex(propnum);
                    if (pn.prop_noun)
                    {
                        char propbuf[BUFSZ];
                        char descrbuf[BUFSZ];
                        Strcpy(descrbuf, "");
                        Strcpy(propbuf, pn.prop_noun);
                        *propbuf = highc(*propbuf);
                        if (pn.prop_desc)
                            Sprintf(descrbuf, " (%s)", pn.prop_desc);
                        else
                            Strcpy(descrbuf, "");

                        powercnt++;
                        Sprintf(buf, " %2d - %s %s%s", powercnt, propbuf, endbuf, descrbuf);
                        
                        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                    }
                }
            }
            if (specialeffect & SPFX_WARN_OF_MON)
            {
                powercnt++;
                Sprintf(buf, " %2d - Warning of the presence of %s %s", powercnt,
                    (context.warntype.obj & M2_ORC) ? "orcs"
                    : (context.warntype.obj & M2_ELF) ? "elves"
                    : (context.warntype.obj & M2_WERE) ? "lycanthropes"
                    : (context.warntype.obj & M2_GIANT) ? "giants"
                    : (context.warntype.obj & M2_ANGEL) ? "angels"
                    : (context.warntype.obj & M2_DEMON) ? "demons" : something, endbuf);
                
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }

            if (specialeffect & SPFX_LUCK)
            {
                powercnt++;
                Sprintf(buf, " %2d - Confers luck %s", powercnt, endbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (specialeffect & SPFX_UNLUCK)
            {
                powercnt++;
                Sprintf(buf, " %2d - Confers bad luck %s", powercnt, endbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (specialeffect & SPFX_STR_25)
            {
                powercnt++;
                Sprintf(buf, " %2d - Raises strength to 25 %s", powercnt, endbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (specialeffect & SPFX_DEX_25)
            {
                powercnt++;
                Sprintf(buf, " %2d - Raises dexterity to 25 %s", powercnt, endbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (specialeffect & SPFX_CON_25)
            {
                powercnt++;
                Sprintf(buf, " %2d - Raises constitution to 25 %s", powercnt, endbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (specialeffect & SPFX_INT_25)
            {
                powercnt++;
                Sprintf(buf, " %2d - Raises intelligence to 25 %s", powercnt, endbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (specialeffect & SPFX_WIS_25)
            {
                powercnt++;
                Sprintf(buf, " %2d - Raises wisdom to 25 %s", powercnt, endbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
            if (specialeffect & SPFX_CHA_25)
            {
                powercnt++;
                Sprintf(buf, " %2d - Raises charisma to 25 %s", powercnt, endbuf);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
        }

        if (artilist[obj->oartifact].mtype > 0)
        {
            Strcpy(endbuf, "");
            if ((artilist[obj->oartifact].aflags & AF_DMONS) && artilist[obj->oartifact].mtype < NUM_MONSTERS)
            {
                Strcpy(endbuf, pm_plural_name(&mons[artilist[obj->oartifact].mtype], 1));
            }
            else if (artilist[obj->oartifact].aflags & (AF_DFLAG1 | AF_DFLAG2))
            {
                char affectbuf[BUFSZ];
                Strcpy(affectbuf, "");

                int cnt = 0;
                for (i = 0; i < NUM_UNSIGNED_LONG_BITS; i++)
                {
                    uint64_t bit = 1UL;

                    if (i > 0)
                        bit = bit << i;

                    if (artilist[obj->oartifact].mtype & bit)
                        cnt++;
                }

                int idx = 0;
                for (i = 0; i < NUM_UNSIGNED_LONG_BITS; i++)
                {
                    uint64_t bit = 1UL;

                    if (i > 0)
                        bit = bit << i;

                    if (artilist[obj->oartifact].mtype & bit)
                    {
                        idx++;
                        const char* affdesc = get_mflag_description(bit, TRUE, (artilist[obj->oartifact].aflags & AF_DFLAG1) ? 1 : 2);

                        if (cnt > 1 && idx > 1)
                        {
                            if (idx == cnt)
                                Sprintf(eos(affectbuf), "%s and %s", cnt > 2 ? "," : "", affdesc);
                            else
                                Sprintf(eos(affectbuf), ", %s", affdesc);
                        }
                        else
                        {
                            Sprintf(affectbuf, "%s", affdesc);
                            if(cnt == 1)
                                break;
                        }
                    }
                }

                Strcpy(endbuf, affectbuf);
            }
            else if ((artilist[obj->oartifact].aflags & AF_DCLAS) && artilist[obj->oartifact].mtype < MAX_MONSTER_CLASSES)
            {
                Strcpy(endbuf, def_monsyms[artilist[obj->oartifact].mtype].name);
            }
            else if (artilist[obj->oartifact].aflags & AF_DALIGN)
            {
                Strcpy(endbuf, "non-aligned creatures");
            }
            powercnt++;
            Sprintf(buf, " %2d - Attack bonus applies to %s", powercnt, endbuf);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
    }


    /* Note if used as a component for a spell */
    if (obj && objects[otyp].oc_name_known && !objects[otyp].oc_unique && !obj->owornmask && !obj->oartifact
        && !is_obj_unremovable_from_the_game(obj) && !is_obj_indestructible(obj))
    {
        int spellcnt = 0;
        for (i = 0; i < MAXSPELL && spellid(i) != NO_SPELL; i++)
        {
            if (is_obj_component_for(i, obj) > 0)
                spellcnt++;
        }
        if (spellcnt > 0)
        {
            Sprintf(buf, "Component for the following spell%s:", plur(spellcnt));            
            putstr(datawin, ATR_HEADING, buf);
            int compcnt = 0;
            const char* splname = 0;
            char sbuf[BUFSZ];
            int splres;
            for (i = 0; i < MAXSPELL && spellid(i) != NO_SPELL; i++)
            {
                splres = is_obj_component_for(i, obj);
                if (splres > 0)
                {
                    compcnt++;
                    splname = OBJ_NAME(objects[spellid(i)]);
                    Strcpy(sbuf, splname);
                    *sbuf = highc(*sbuf);
                    Sprintf(buf, " %2d - %s%s%s", compcnt, splres >= 2 ? "Maybe: " : "", sbuf, splres == 2 ? " (blessedness unknown)" : "");
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                }
            }
        }
    }
    else if (!obj)
    {
        int spellcnt = 0;
        short* spellbook_indices = (short*)alloc((SPE_BLANK_PAPER - FIRST_SPELL) * sizeof(short));
        memset(spellbook_indices, 0, MAXSPELL * sizeof(short));

        for (i = FIRST_SPELL; i < SPE_BLANK_PAPER; i++)
        {
            if (is_otyp_component_for_spellbook(i, otyp, (uint64_t*)0, (int*)0) > 0)
            {
                spellbook_indices[spellcnt] = (short)i;
                spellcnt++;
            }
        }
        if (spellcnt > 0)
        {
            qsort(spellbook_indices, spellcnt, sizeof(short), item_wiki_cmp);

            Sprintf(buf, "Component for the following spell%s:", plur(spellcnt));
            putstr(datawin, ATR_HEADING, buf);
            int compcnt = 0;
            const char* splname = 0;
            char sbuf[BUFSZ];
            char fbuf[BUFSZ];
            int splres;
            for (i = 0; i < spellcnt; i++)
            {
                uint64_t mcflags = 0;
                int mccorpsenm = -1;
                splres = is_otyp_component_for_spellbook((int)spellbook_indices[i], otyp, &mcflags, &mccorpsenm);
                if (splres > 0)
                {
                    compcnt++;
                    splname = OBJ_NAME(objects[spellbook_indices[i]]);
                    Strcpy(sbuf, splname);
                    *sbuf = highc(*sbuf);
                    Strcpy(fbuf, "");
                    if (mccorpsenm >= 0 && mccorpsenm < NUM_MONSTERS)
                    {
                        if (*fbuf)
                            Strcat(fbuf, ", ");
                        Strcat(fbuf, mons[mccorpsenm].mname);
                    }
                    if (mcflags & MATCOMP_NOT_SPENT)
                    {
                        if (*fbuf)
                            Strcat(fbuf, ", ");
                        Strcat(fbuf, "catalyst");
                    }
                    if (mcflags & MATCOMP_NOT_CURSED)
                    {
                        if(*fbuf)
                            Strcat(fbuf, ", ");
                        Strcat(fbuf, "not cursed");
                    }
                    if (mcflags & MATCOMP_BLESSED_REQUIRED)
                    {
                        if (*fbuf)
                            Strcat(fbuf, ", ");
                        Strcat(fbuf, "blessed");
                    }
                    if (mcflags & MATCOMP_CURSED_REQUIRED)
                    {
                        if (*fbuf)
                            Strcat(fbuf, ", ");
                        Strcat(fbuf, "cursed");
                    }
                    if (mcflags & MATCOMP_DEATH_ENCHANTMENT_REQUIRED)
                    {
                        if (*fbuf)
                            Strcat(fbuf, ", ");
                        Strcat(fbuf, "death-enchanted");
                    }
                    Sprintf(buf, " %2d - [[%s]]%s%s%s", compcnt, sbuf, *fbuf ? " (" : "", fbuf, *fbuf ? ")" : "");
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                }
            }
        }

        free(spellbook_indices);
    }

    /* Notable */
    boolean note_your_egg = obj && otyp == EGG && obj->corpsenm >= LOW_PM && obj->known && (mvitals[obj->corpsenm].mvflags & MV_KNOWS_EGG) != 0 && (obj->speflags & SPEFLAGS_YOURS) != 0;
    boolean note_launcher_str_bonus = is_otyp_launcher(otyp) && !(objects[otyp].oc_flags3 & O3_USES_FIXED_DAMAGE_BONUS_INSTEAD_OF_STRENGTH);
    if (note_your_egg || note_launcher_str_bonus)
    {
        int powercnt = 0;

        Sprintf(buf, "Notable:");
        putstr(datawin, ATR_HEADING, buf);

        if (note_your_egg)
        {
            powercnt++;
            Sprintf(buf, " %2d - %s", powercnt, "Laid by you");
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
        if (note_launcher_str_bonus)
        {
            powercnt++;
            Sprintf(buf, " %2d - %s", powercnt, "Adds the user's strength bonus to damage.");
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }
    }

    /* Description */
    if (stats_known && OBJ_ITEM_DESC(otyp) /* && !(obj->oartifact && obj->nknown) */)
    {
        Strcpy(buf, "Description:");
        putstr(datawin, ATR_HEADING, buf);

        size_t desclen = strlen(OBJ_ITEM_DESC(otyp));
        char* descbuf = (char*)alloc(desclen + BUFSZ);
        Strcpy(descbuf, OBJ_ITEM_DESC(otyp));
        convert_dice_to_ranges(descbuf);
        char* bp = descbuf;
        char* ebp;
        while (bp && *bp)
        {
            ebp = strstr(bp, "\n");
            if (ebp)
                *ebp = 0;

            if (strlen(bp) > 0)
            {
                Sprintf(buf, "  %s", bp);                
                putstr(datawin, ATR_INDENT_AT_DOUBLE_SPACE, buf);
            }

            if (!ebp)
                break;
            else
                bp = ebp + 1;
        }
        free((genericptr_t)descbuf);
    }

    boolean weapon_stats_printed = FALSE;
    boolean armor_stats_printed = FALSE;
    boolean wep_ac50pct_stat_set = FALSE;
    if (obj)
    {
        int wep_ac50pct_stat = 0;

        /* Weapon statistics */
        struct obj* applicable_launcher = is_launcher(obj) ? obj : uwep && is_launcher(uwep) ? uwep : uswapwep && is_launcher(uswapwep) ? uswapwep : obj;
        if ((is_weapon(obj)
            || (uwep && obj == uwep) || (uswapwep && obj == uswapwep)
            || (u.twoweap && ((uarms && obj == uarms) || (uswapwep2 && obj == uswapwep2)))
            || weapon_stats_shown
            )
            )
        {
            int powercnt = 0;
            if (is_boots(obj))
                Sprintf(buf, "Statistics in kicking:");
            else if (is_gloves(obj))
                Sprintf(buf, "Statistics in bare-handed combat:");
            else
                Sprintf(buf, "Weapon statistics:");

            putstr(datawin, ATR_HEADING, buf);

            if (is_ammo(obj) && !ammo_and_launcher(obj, applicable_launcher))
            {
                if (!applicable_launcher || obj == applicable_launcher)
                {
                    powercnt++;
                    Sprintf(buf, " %2d - You have not readied a compatible launcher", powercnt);
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                }
                else
                {
                    powercnt++;
                    Sprintf(buf, " %2d - Not compatible with %s", powercnt, yname(applicable_launcher));
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                }
            }
            else
            {
                weapon_stats_printed = TRUE;
                if (!is_launcher(obj) && stats_known && obj->known
                    && (obj == applicable_launcher || !ammo_and_launcher(obj, applicable_launcher)
                        || (ammo_and_launcher(obj, applicable_launcher) && object_stats_known(applicable_launcher) && applicable_launcher->known))
                    )
                {
                    /* we use youmonst as a proxy */
                    /* You hit if rnd(20) < roll_to_hit */
                    int attknum = 1, armorpenalty = 0;
                    int roll_to_hit = 0;
                    if ((is_thrown_weapon_only(obj) || is_ammo(obj)) && !(obj == uwep || obj == uwep2 || obj == uswapwep || obj == uswapwep2))
                    {
                        roll_to_hit = -1 + Luck + u_ranged_strdex_to_hit_bonus() + find_mac(&youmonst) + u.ubasehitinc + u.uhitinc
                            + maybe_polyd((int)youmonst.data->mlevel, u.ulevel);

                        roll_to_hit += omon_adj(&youmonst, obj, FALSE);

                        if (is_ammo(obj))
                        {
                            if (!ammo_and_launcher(obj, applicable_launcher))
                            {
                                roll_to_hit -= 4;
                            }
                            else if (applicable_launcher)
                            {
                                roll_to_hit += weapon_to_hit_value(applicable_launcher, &youmonst, &youmonst, 2);
                                roll_to_hit += weapon_skill_hit_bonus(applicable_launcher, P_NONE, FALSE, FALSE, TRUE, 0, TRUE, FALSE);

                                if ((Race_if(PM_ELF) || Role_if(PM_SAMURAI))
                                    && (!Upolyd || your_race(youmonst.data))
                                    && objects[applicable_launcher->otyp].oc_skill == P_BOW)
                                {
                                    roll_to_hit++;
                                    if (Race_if(PM_ELF) && applicable_launcher->otyp == ELVEN_LONG_BOW)
                                        roll_to_hit++;
                                    else if (Role_if(PM_SAMURAI) && applicable_launcher->otyp == YUMI)
                                        roll_to_hit++;
                                }

                                roll_to_hit += u.uarcherybonus;
                            }
                        }
                        else
                        {
                            if (is_thrown_weapon_only(obj)) /* meant to be thrown */
                                roll_to_hit += 2;

                            roll_to_hit += weapon_skill_hit_bonus(obj, P_NONE, FALSE, FALSE, TRUE, 0, TRUE, TRUE);
                        }
                    }
                    else
                    {
                        roll_to_hit = find_roll_to_hit(&youmonst, AT_WEAP, obj, &attknum, &armorpenalty);
                    }

                    /* This is not accurate for fired weapons since it does not account for launcher properly; also thrown weapons plusses are a bit inaccurate */
                    int youmonstac = find_mac(&youmonst);
                    int chance_to_hit_youmonst = (roll_to_hit - 1) * 5;
                    int chance_to_hit_ac0 = chance_to_hit_youmonst - youmonstac * 5;
                    int diff_to_50_from_ac0 = 50 - chance_to_hit_ac0;
                    int ac_with_50_chance = diff_to_50_from_ac0 / 5;
                    int diff_to_5_from_ac0 = 5 - chance_to_hit_ac0;
                    int ac_with_5_chance = diff_to_5_from_ac0 / 5;
                    wep_ac50pct_stat_set = TRUE;
                    wep_ac50pct_stat = ac_with_50_chance;
                    if (stats_ptr)
                    {
                        stats_ptr->wep_ac50pct_set = TRUE;
                        stats_ptr->ac50pct = ac_with_50_chance;
                    }
                    powercnt++;
                    Sprintf(buf, " %2d - You have ", powercnt);
                    putstr_ex(datawin, buf, ATR_INDENT_AT_DASH, NO_COLOR, 1);
                    putstr_ex(datawin, "50%", ATR_INDENT_AT_DASH | ATR_BOLD, CLR_WHITE, 1);
                    putstr_ex(datawin, " chance to hit AC ", ATR_INDENT_AT_DASH, NO_COLOR, 1);
                    Sprintf(buf, "%d", ac_with_50_chance);
                    putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_BOLD, CLR_WHITE, 1);
                    putstr_ex(datawin, " and ", ATR_INDENT_AT_DASH, NO_COLOR, 1);
                    putstr_ex(datawin, "5%", ATR_INDENT_AT_DASH | ATR_BOLD, CLR_WHITE, 1);
                    putstr_ex(datawin, " AC ", ATR_INDENT_AT_DASH, NO_COLOR, 1);
                    Sprintf(buf, "%d", ac_with_5_chance);
                    putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_BOLD, CLR_WHITE, 0);
                }

                struct multishot_result msres = get_multishot_stats(&youmonst, obj, is_launcher(obj) ? obj : uwep && is_launcher(uwep) ? uwep : uswapwep && is_launcher(uswapwep) ? uswapwep : obj, is_launcher(obj) ? 2 : (is_ammo(obj) || throwing_weapon(obj)));
                double average_multi_shot_times = msres.average;

                powercnt++;
                const char* applicable_verb = is_thrown_weapon_only(obj) ? "throw" : is_launcher(obj) || ammo_and_launcher(obj, applicable_launcher) ? "fire" : "strike";
                if (average_multi_shot_times == 1.0)
                    Sprintf(buf, " %2d - You %s once per round", powercnt, applicable_verb);
                else if (average_multi_shot_times == 2.0)
                    Sprintf(buf, " %2d - You %s twice per round", powercnt, applicable_verb);
                else if (average_multi_shot_times == 3.0)
                    Sprintf(buf, " %2d - You %s three times per round", powercnt, applicable_verb);
                else
                    Sprintf(buf, " %2d - You %s an average of %.1f time%s per round", powercnt, applicable_verb, average_multi_shot_times, plur(average_multi_shot_times));

                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

                wep_avg_dmg *= average_multi_shot_times;
                wep_multipliable_avg_dmg *= average_multi_shot_times;
                wep_all_extra_avg_dmg *= average_multi_shot_times;

                if (stats_ptr)
                {
                    stats_ptr->avg_damage = wep_avg_dmg;
                    stats_ptr->extra_damage = wep_all_extra_avg_dmg;
                }

                powercnt++;
                Sprintf(buf, " %2d - Your basic average damage is %.1f per round", powercnt, wep_avg_dmg);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                if (wep_all_extra_avg_dmg != 0)
                {
                    powercnt++;
                    Sprintf(buf, " %2d - Your average damage with extras is %.1f per round", powercnt, wep_avg_dmg + wep_all_extra_avg_dmg);
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                }

                if (is_launcher(obj) && ammo_and_launcher(uquiver, obj) && !stats_ptr)
                {
                    struct item_description_stats ammo_stats = { 0 };
                    (void)itemdescription_core(uquiver, uquiver->otyp, &ammo_stats);
                    char dmgbuf[BUFSZ];
                    if (ammo_stats.stats_set && ammo_stats.weapon_stats_printed)
                        Sprintf(dmgbuf, "%.1f", ammo_stats.avg_damage);
                    else
                        Strcpy(dmgbuf, "further");
                    powercnt++;
                    Sprintf(buf, " %2d - %s will cause %s damage per round", powercnt, uquiver ? Yname2(uquiver) : "Your ammo", dmgbuf);
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                    if (ammo_stats.stats_set && ammo_stats.weapon_stats_printed)
                    {
                        powercnt++;
                        Sprintf(buf, " %2d - Your average damage with ammo is %.1f per round", powercnt, wep_avg_dmg + ammo_stats.avg_damage);
                        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                        if (wep_all_extra_avg_dmg != 0)
                        {
                            powercnt++;
                            Sprintf(buf, " %2d - Your average damage with ammo and extras is %.1f per round", powercnt, wep_avg_dmg + wep_all_extra_avg_dmg + ammo_stats.avg_damage);
                            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                        }
                    }
                }
                else if (is_ammo(obj) && ammo_and_launcher(obj, applicable_launcher) && !stats_ptr)
                {
                    struct item_description_stats launcher_stats = { 0 };
                    (void)itemdescription_core(applicable_launcher, applicable_launcher->otyp, &launcher_stats);
                    char dmgbuf[BUFSZ];
                    if (launcher_stats.stats_set && launcher_stats.weapon_stats_printed)
                        Sprintf(dmgbuf, "%.1f", launcher_stats.avg_damage);
                    else
                        Strcpy(dmgbuf, "further");
                    powercnt++;
                    Sprintf(buf, " %2d - %s will cause %s damage per round", powercnt, applicable_launcher ? Yname2(applicable_launcher) : "Your launcher", dmgbuf);
                    putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                    if (launcher_stats.stats_set && launcher_stats.weapon_stats_printed)
                    {
                        powercnt++;
                        Sprintf(buf, " %2d - Your average damage with launcher is %.1f per round", powercnt, wep_avg_dmg + launcher_stats.avg_damage);
                        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                        if (wep_all_extra_avg_dmg != 0)
                        {
                            powercnt++;
                            Sprintf(buf, " %2d - Your average damage with launcher and extras is %.1f per round", powercnt, wep_avg_dmg + wep_all_extra_avg_dmg + launcher_stats.avg_damage);
                            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
                        }
                    }
                }
            }
        }

        /* Armor statistics */
        int totalacbonus = 0;
        int totalmcbonus = 0;
        if (notfullyidentified || !stats_known)
        {
            totalacbonus = -knownacbonus;
            totalmcbonus = knownmcbonus;
        }
        else
        {
            totalacbonus = -ARM_AC_BONUS(obj, youmonst.data);
            totalmcbonus = ARM_MC_BONUS(obj, youmonst.data);
        }
        if (is_shield(obj))
        {
            totalacbonus -= shield_skill_ac_bonus(P_SKILL_LEVEL(P_SHIELD));
            totalmcbonus += shield_skill_mc_bonus(P_SKILL_LEVEL(P_SHIELD));
        }
        if (stats_ptr)
        {
            stats_ptr->ac_bonus = totalacbonus;
            stats_ptr->mc_bonus = totalmcbonus;
        }

        if (is_armor(obj) || (objects[(obj)->otyp].oc_flags & O1_IS_ARMOR_WHEN_WIELDED) || (has_obj_mythic_defense(obj) && obj->mknown))
        {
            armor_stats_printed = TRUE;
            int powercnt = 0;
            Sprintf(buf, "Armor statistics:");
            putstr(datawin, ATR_HEADING, buf);

            const char* totalprefix = notfullyidentified ? "Total known" : "Total";
            powercnt++;
            Sprintf(buf, " %2d - %s AC %s %s%d", powercnt, totalprefix, totalacbonus > 0 ? "penalty" : "bonus", totalacbonus >= 0 ? "+" : "", totalacbonus);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            powercnt++;
            Sprintf(buf, " %2d - %s MC %s %s%d", powercnt, totalprefix, totalmcbonus < 0 ? "penalty" : "bonus", totalmcbonus >= 0 ? "+" : "", totalmcbonus);
            putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
        }

        /* Comparison to current item */
        if (!stats_ptr)
        {
            struct obj* launcher = uwep && is_launcher(uwep) ? uwep : uswapwep && is_launcher(uswapwep) ? uswapwep : 0;
            struct obj* cwep = is_boots(obj) && is_weapon(obj) ? uarmf : is_gloves(obj) && is_weapon(obj) ? uarmg : is_ammo(obj) ? (uquiver && launcher && ammo_and_launcher(uquiver, launcher) && ammo_and_launcher(obj, launcher) ? uquiver : 0) :
                is_thrown_weapon_only(obj) ? (uquiver && is_thrown_weapon_only(uquiver) ? uquiver : 0) :
                (is_wieldable_weapon(obj) && uwep && is_launcher(obj) == is_launcher(uwep)) ? uwep :
                (is_wieldable_weapon(obj) && uswapwep && is_launcher(obj) == is_launcher(uswapwep)) ? uswapwep : 0;
            if (cwep && cwep != obj && uwep2 != obj && uswapwep2 != obj && weapon_stats_printed)
            {
                struct item_description_stats cwep_stats = { 0 };
                (void)itemdescription_core(cwep, cwep->otyp, &cwep_stats);
                if (cwep_stats.stats_set && cwep_stats.weapon_stats_printed)
                {
                    int powercnt = 0;
                    if(is_boots(obj) || is_gloves(obj))
                        Sprintf(buf, "Comparison to current %s:", is_boots(obj) ? "weapon boots" : "weapon gloves");
                    else
                        Sprintf(buf, "Comparison to current %s%s:", u.twoweap ? "right-hand " : "", cwep == uwep ? "weapon" : cwep == uswapwep ? "alternative weapon" :
                            cwep == uquiver ? (is_ammo(obj) ? "quivered ammo" : "readied thrown weapon") : "readied item");
                    putstr(datawin, ATR_HEADING, buf);
                    powercnt++;
                    Sprintf(buf, " %2d - Change in average damage is ", powercnt);
                    putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, NO_COLOR, 1);
                    double dmgdiff = wep_avg_dmg - cwep_stats.avg_damage;
                    Sprintf(buf, "%s%.1f", dmgdiff >= 0 ? "+" : "", dmgdiff);
                    putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, dmgdiff > 0 ? CLR_BRIGHT_GREEN : dmgdiff < 0 ? CLR_RED : NO_COLOR, 0);
                    if (wep_ac50pct_stat_set && cwep_stats.wep_ac50pct_set)
                    {
                        powercnt++;
                        Sprintf(buf, " %2d - Change in AC hit at 50%% chance is ", powercnt);
                        putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, NO_COLOR, 1);
                        int ac50pcthitdiff = wep_ac50pct_stat - cwep_stats.ac50pct;
                        Sprintf(buf, "%s%d", ac50pcthitdiff >= 0 ? "+" : "", ac50pcthitdiff);
                        putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, ac50pcthitdiff < 0 ? CLR_BRIGHT_GREEN : ac50pcthitdiff > 0 ? CLR_RED : NO_COLOR, 0);
                    }
                }
            }
            if (u.twoweap && uwep2 && is_wieldable_weapon(uwep2) && is_wieldable_weapon(obj) && !bimanual(obj) && !is_ammo(obj) && !is_launcher(obj) && !is_thrown_weapon_only(obj) && !is_boots(obj) && !is_gloves(obj) && obj != uwep2 && obj != uwep)
            {
                struct obj* cwep2 = uwep2;
                struct item_description_stats cwep2_stats = { 0 };
                (void)itemdescription_core(cwep2, cwep2->otyp, &cwep2_stats);
                if (cwep2_stats.stats_set && cwep2_stats.weapon_stats_printed)
                {
                    int powercnt = 0;
                    Strcpy(buf, "Comparison to current left-hand weapon:");
                    putstr(datawin, ATR_HEADING, buf);
                    powercnt++;
                    Sprintf(buf, " %2d - Change in average damage is ", powercnt);
                    putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, NO_COLOR, 1);
                    double dmgdiff = wep_avg_dmg - cwep2_stats.avg_damage;
                    Sprintf(buf, "%s%.1f", dmgdiff >= 0 ? "+" : "", dmgdiff);
                    putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, dmgdiff > 0 ? CLR_BRIGHT_GREEN : dmgdiff < 0 ? CLR_RED : NO_COLOR, 0);
                    if (wep_ac50pct_stat_set && cwep2_stats.wep_ac50pct_set)
                    {
                        powercnt++;
                        Sprintf(buf, " %2d - Change in AC hit at 50%% chance is ", powercnt);
                        putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, NO_COLOR, 1);
                        int ac50pcthitdiff = wep_ac50pct_stat - cwep2_stats.ac50pct;
                        Sprintf(buf, "%s%d", ac50pcthitdiff >= 0 ? "+" : "", ac50pcthitdiff);
                        putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, ac50pcthitdiff < 0 ? CLR_BRIGHT_GREEN : ac50pcthitdiff > 0 ? CLR_RED : NO_COLOR, 0);
                    }
                }
            }

            if (is_armor(obj) && armor_stats_printed)
            {
                struct obj* current_armor = 0;
                if (is_suit(obj))
                    current_armor = uarm;
                else if (is_cloak(obj))
                    current_armor = uarmc;
                else if (is_robe(obj))
                    current_armor = uarmo;
                else if (is_shirt(obj))
                    current_armor = uarmu;
                else if (is_helmet(obj))
                    current_armor = uarmh;
                else if (is_gloves(obj))
                    current_armor = uarmg;
                else if (is_boots(obj))
                    current_armor = uarmf;
                else if (is_bracers(obj))
                    current_armor = uarmb;
                else if (is_shield(obj))
                    current_armor = uarms;

                if (current_armor && obj != current_armor && is_armor(current_armor))
                {
                    struct item_description_stats armor_stats = { 0 };
                    (void)itemdescription_core(current_armor, current_armor->otyp, &armor_stats);
                    if (armor_stats.stats_set && armor_stats.armor_stats_printed)
                    {
                        int armor_type = objects[current_armor->otyp].oc_subtyp;
                        const char* atypename = armor_type_names[armor_type];
                        int powercnt = 0;
                        Sprintf(buf, "Comparison to current %s", atypename);
                        putstr(datawin, ATR_HEADING, buf);
                        powercnt++;
                        Sprintf(buf, " %2d - Change in AC is ", powercnt);
                        putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, NO_COLOR, 1);
                        int acdiff = totalacbonus - armor_stats.ac_bonus;
                        Sprintf(buf, "%s%d", acdiff >= 0 ? "+" : "", acdiff);
                        putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, acdiff < 0 ? CLR_BRIGHT_GREEN : acdiff > 0 ? CLR_RED : NO_COLOR, 0);
                        powercnt++;
                        Sprintf(buf, " %2d - Change in MC is ", powercnt);
                        putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, NO_COLOR, 1);
                        int mcdiff = totalmcbonus - armor_stats.mc_bonus;
                        Sprintf(buf, "%s%d", mcdiff >= 0 ? "+" : "", mcdiff);
                        putstr_ex(datawin, buf, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, mcdiff > 0 ? CLR_BRIGHT_GREEN : mcdiff < 0 ? CLR_RED : NO_COLOR, 0);
                    }
                }
            }
        }

        /* Hints */
        boolean show_identify_hint = (flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && obj->dknown && (!stats_known || notfullyidentified);
        if (show_identify_hint || show_corpse_hint)
        {
            int powercnt = 0;

            Sprintf(buf, "Hints:");
            putstr(datawin, ATR_HEADING, buf);

            if (show_identify_hint)
            {
                powercnt++;
                if (!stats_known)
                    Sprintf(buf, " %2d - You can identify this item, for example, by using a scroll of identify", powercnt);
                else
                    Sprintf(buf, " %2d - You can fully learn the statistics by identifying this item (e.g., by using a scroll of identify)", powercnt);

                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }

            if (show_corpse_hint)
            {
                powercnt++;
                Sprintf(buf, " %2d - You can determine this corpse's properties by using a wand of probing on it", powercnt);
                putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);
            }
        }
    }
    display_nhwindow(datawin, FALSE);
    destroy_nhwindow(datawin), datawin = WIN_ERR;
    if (stats_ptr)
    {
        stats_ptr->stats_set = TRUE;
        stats_ptr->weapon_stats_printed = weapon_stats_printed;
        stats_ptr->armor_stats_printed = armor_stats_printed;
        stats_ptr->wep_ac50pct_set = wep_ac50pct_stat_set;
        windowprocs = saved_procs;
    }
    return 0;
}




STATIC_VAR const char* damage_type_names[] = {
    "physical", "magic", "fire", "cold", "sleep",
    "disintegrating", "electrical", "poisonous", "acidic", "deadly",
    "petrifying", "blinding", "stunning", "slowing", "paralyzing",
    "life draining", "mana draining", "leg-damaging", "", "sticky",
    "steals gold", "steals item", "seduction", "teleportation", "rust-causing", 
    "confusion", "digestion", "healing", "wraps",  "lycanthropy-infected",
    "drains dexterity", "drains constitution", "drains intelligence", "infected with terminal illness", "decays organics",
    "demonic seduction", "hallucination", "death-causing", "pestilence-infecting", "famine-causing",
    "slime-contracting", "disenchanting", "corroding", "cancelling", "psionic", "system shock",
    "hit point adjustment", "sharpness", "rot-infected",

};

STATIC_VAR const char* damage_type_names_high[] = {
    "fire or poison gas", "disintegration or cold", "ancient gold dragon", "celestial", "magic",
    "random draconic", "demon", "controlled creatures", "gnolls", "ghouls",
    "undead", "minotaurs", "random eye stalk", "random eye stalk", "bison",
    "steals amulet", "curse",
};


const char* get_damage_type_text(damagetype)
short damagetype;
{
    if (damagetype >= AD_RBGD && damagetype - AD_RBGD < SIZE(damage_type_names))
        return damage_type_names_high[damagetype - AD_RBGD];
    else if (damagetype < 0 || damagetype >= SIZE(damage_type_names))
        return empty_string;

    return damage_type_names[damagetype];
}

STATIC_VAR const char* defense_type_names[] = {
    "physical damage resistance", "magic resistance", "fire resistance", "cold resistance", "",
    "", "", "shock resistance", "", "",
    "", "", "stun resistance", "", "",
    "drain resistance",
};

const char* get_defense_type_text(defensetype)
int defensetype;
{
    if (defensetype < 0 || defensetype >= SIZE(defense_type_names))
        return empty_string;

    return defense_type_names[defensetype];
}

int
monsterdescription(mon)
struct monst* mon;
{
    if (!mon)
        return 0;
    struct permonst* ptr = mon->data;
    return monsterdescription_core(mon, ptr);
}

int
monsterdescription_core(mon, ptr)
struct monst* mon;
struct permonst* ptr;
{
    if (!ptr)
        return 0;

    winid datawin = WIN_ERR;
    int glyph = mon ? any_mon_to_glyph(mon, rn2_on_display_rng) : (int)(ptr - &mons[0]) + GLYPH_MON_OFF;
    int gui_glyph = mon ? maybe_get_replaced_glyph(glyph, mon->mx, mon->my, data_to_replacement_info(glyph, LAYER_MONSTER, (struct obj*)0, mon, 0UL, 0UL, 0UL, MAT_NONE, 0)) : glyph;

    datawin = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_MONSTER_DESCRIPTION_SCREEN, gui_glyph, extended_create_window_info_from_mon_with_flags(mon, WINDOW_CREATE_FLAGS_USE_SPECIAL_SYMBOLS));

    //int mnum = mon->mnum;
    boolean is_you = (mon == &youmonst);
    short cham = mon && !is_you ? mon->cham : NON_PM;

    char buf[BUFSZ];
    char buf2[BUFSZ];
    char buf3[BUFSZ];
    char buf4[BUFSZ];

    Strcpy(buf, "");
    Strcpy(buf2, "");
    Strcpy(buf3, "");

    const char* monster_name = mon ? x_monnam(mon, ARTICLE_NONE, (char*)0, (has_mname(mon)) ? (SUPPRESS_SADDLE | SUPPRESS_IT) : SUPPRESS_IT, FALSE) : ptr->mname;
    const char* monster_type_name = mon ? mon_monster_name(mon) : ptr->mname;
    boolean contains_type_name = TRUE;

    Strcpy(buf2, "");
    Strcpy(buf3, "");
    if (strcmp(monster_name, monster_type_name) && !strstr(monster_name, monster_type_name))
    {
        contains_type_name = FALSE;
        Strcpy(buf2, monster_type_name);
        *buf2 = highc(*buf2);
        Sprintf(buf3, " - %s", buf2);
    }

    Strcpy(buf4, "");
    if (mon && has_umname(mon) && has_mname(mon) && mon->u_know_mname && !strcmp(monster_name, UMNAME(mon)) && strcmp(UMNAME(mon), MNAME(mon)))
    {
        Sprintf(buf4, " a.k.a. %s", MNAME(mon));
    }

    /* Name */
    Sprintf(buf, "%s%s%s", 
        mon ? x_monnam(mon, ARTICLE_NONE, (char*)0, (has_mname(mon)) ? (SUPPRESS_SADDLE | SUPPRESS_IT) : SUPPRESS_IT, FALSE) : ptr->mname,
           buf4,
           !contains_type_name ? buf3 : "");
    *buf = highc(*buf);
    if (ptr->mtitle && strcmp(ptr->mtitle, ""))
    {
        Sprintf(eos(buf), ", %s", ptr->mtitle);
    }
    
    putstr(datawin, ATR_TITLE, buf);

    /* Description */
    if (ptr->mdescription && strcmp(ptr->mdescription, ""))
    {
        Sprintf(buf, "Level %d %s", ptr->difficulty, ptr->mdescription);
        //*buf = highc(*buf);
        
        putstr(datawin, ATR_SUBTITLE, buf);
    }
    else
    {
        /* Type */
        Sprintf(buf, "Level %d %s%s", ptr->difficulty, ptr->geno & G_UNIQ ? "unique " : "", def_monsyms[(int)ptr->mlet].explain);
        //*buf = highc(*buf);
        
        putstr(datawin, ATR_SUBTITLE, buf);
    }

    Strcpy(buf, "");
    
    putstr(datawin, 0, buf);

    if (cham >= LOW_PM)
    {
        Strcpy(buf2, mon && mon->female && mons[cham].mfemalename ? mons[cham].mfemalename : mons[cham].mname);
        *buf2 = highc(*buf2);
        Sprintf(buf, "True form:              %s", buf2);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    int relevant_hp = mon ? (is_you ? (Upolyd ? u.mh : u.uhp) : mon->mhp) : 0;
    int relevant_hpmax = mon ? (is_you ? (Upolyd ? u.mhmax : u.uhpmax) : mon->mhpmax) : 0;
    int relevant_level = !mon || is_you ? ptr->mlevel : mon->m_lev;

    Strcpy(buf2, "");
    if (relevant_level != (int)ptr->mlevel)
    {
        Sprintf(buf2, " (base %d)", (int)ptr->mlevel);
    }

    Sprintf(buf, "Hit dice:               %d%s", relevant_level, buf2);    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    if (mon)
    {
        Sprintf(buf, "Hit points:             %d (%d)", relevant_hp, relevant_hpmax);
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }
    Sprintf(buf, "Move:                   %d\"", ptr->mmove);    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    Strcpy(buf2, "");
    int ac = !mon ? ptr->ac : is_you ? u.uac : find_mac(mon);
    if (ac != ptr->ac)
    {
        Sprintf(buf2, " (base %d)", ptr->ac);
    }

    Sprintf(buf, "Armor class:            %d%s", ac, buf2);    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    Strcpy(buf2, "");
    int mc = !mon ? ptr->mc : magic_negation(mon);
    if (mc != ptr->mc)
    {
        Sprintf(buf2, " (base %d)", ptr->mc);
    }

    Sprintf(buf, "Magic cancellation:     %d%s (%d%%)", mc, buf2, magic_negation_percentage(mc));    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    Sprintf(buf, "Magic resistance:       %d%%", ptr->mr);    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    Sprintf(buf, "Alignment:              %s", ptr->maligntyp > 0 ? "Lawful" : ptr->maligntyp < 0 ? "Chaotic" : "Neutral");  
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    if (mon && mon->subtype > 0)
    {
        if ((mons[mon->mnum].mflags6 & M6_USES_DOG_SUBTYPES) != 0 && mon->subtype < NUM_DOG_BREEDS)
        {
            if (dog_breed_definitions[mon->subtype].breed_name)
            {
                Sprintf(buf, "Breed:                  %s", str_upper_start(dog_breed_definitions[mon->subtype].breed_name));
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
            if (dog_breed_definitions[mon->subtype].short_coat_color || dog_breed_definitions[mon->subtype].long_coat_color)
            {
                Sprintf(buf, "Coat color:             %s", str_upper_start(dog_breed_definitions[mon->subtype].long_coat_color ? dog_breed_definitions[mon->subtype].long_coat_color : dog_breed_definitions[mon->subtype].short_coat_color));
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
        }
        else if ((mons[mon->mnum].mflags6 & M6_USES_CAT_SUBTYPES) != 0 && mon->subtype < NUM_CAT_BREEDS)
        {
            if (cat_breed_definitions[mon->subtype].breed_name)
            {
                Sprintf(buf, "Breed:                  %s", str_upper_start(cat_breed_definitions[mon->subtype].breed_name));
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
            if (cat_breed_definitions[mon->subtype].short_coat_color || cat_breed_definitions[mon->subtype].long_coat_color)
            {
                Sprintf(buf, "Coat color:             %s", str_upper_start(cat_breed_definitions[mon->subtype].long_coat_color ? cat_breed_definitions[mon->subtype].long_coat_color : cat_breed_definitions[mon->subtype].short_coat_color));
                putstr(datawin, ATR_INDENT_AT_COLON, buf);
            }
        }
    }

    if (mon && !is_neuter(ptr))
    {
        Sprintf(buf, "Gender:                 %s", (is_you ? flags.female : mon->female) ? "Female" : "Male");
        putstr(datawin, ATR_INDENT_AT_COLON, buf);
    }

    Strcpy(buf2, get_monster_size_text(ptr->msize));
    *buf2 = highc(*buf2);

    Sprintf(buf, "Size:                   %s", buf2);    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    int number_of_attacks = 0;
    int i;
    for (i = 0; i < NATTK; i++)
    {
        if (ptr->mattk[i].aatyp == 0 && ptr->mattk[i].adtyp == 0 && ptr->mattk[i].damd == 0 && ptr->mattk[i].damn == 0 && ptr->mattk[i].damp == 0)
            break;
        else
            number_of_attacks++;
    }

    Sprintf(buf, "Number of attacks:      %d", number_of_attacks);    
    putstr(datawin, ATR_INDENT_AT_COLON, buf);

    for (i = 0; i < number_of_attacks; i++)
    {
        char attypebuf[BUFSZ];
        char adtypebuf[BUFSZ];
        char damagebuf[BUFSZ];
        char specialbuf[BUFSZ];
        char specialbuf1[BUFSZ];
        char specialbuf2[BUFSZ];

        Strcpy(attypebuf, get_attack_type_text(ptr->mattk[i].aatyp));
        *attypebuf = highc(*attypebuf);

        Strcpy(adtypebuf, "");
        const char* adtxt = get_damage_type_text((short)ptr->mattk[i].adtyp);
        if (adtxt && strcmp(adtxt, ""))
        {
            Sprintf(adtypebuf, ", %s", adtxt);
        }

        Strcpy(damagebuf, "");
        if ((ptr->mattk[i].damn > 0 && ptr->mattk[i].damd > 0) || ptr->mattk[i].damp != 0)
        {
            //boolean dpart = FALSE;
            Strcpy(damagebuf, " ");
            printdice(eos(damagebuf), ptr->mattk[i].damn, ptr->mattk[i].damd, ptr->mattk[i].damp);

            //if ((ptr->mattk[i].damn > 0 && ptr->mattk[i].damd > 0))
            //{
            //    dpart = TRUE;
            //    Sprintf(eos(damagebuf), "%dd%d", ptr->mattk[i].damn, ptr->mattk[i].damd);
            //}

            //if (dpart && ptr->mattk[i].damp > 0)
            //    Strcat(damagebuf, "+");

            //if(ptr->mattk[i].damp != 0)
            //    Sprintf(eos(damagebuf), "%d", ptr->mattk[i].damp);
        }
        
        Strcpy(specialbuf, "");
        Strcpy(specialbuf1, "");
        if (ptr->mattk[i].aatyp == AT_SMMN && ptr->mattk[i].mlevel > 0)
        {
            Sprintf(specialbuf1, "success %d%%", ptr->mattk[i].mlevel);
        }
        else if (ptr->mattk[i].aatyp == AT_MAGC && ptr->mattk[i].mlevel > 0)
        {
            Sprintf(specialbuf1, "as level %d caster", ptr->mattk[i].mlevel);
        }
        
        Strcpy(specialbuf2, "");
        if (ptr->mattk[i].mcadj != 0)
        {
            Sprintf(specialbuf2, "%s%d MC %s", ptr->mattk[i].mcadj > 0 ? "+" : "", ptr->mattk[i].mcadj,
                ptr->mattk[i].mcadj <= 0 ? "penalty" : "bonus");
        }

        if (strcmp(specialbuf1, "") || strcmp(specialbuf2, ""))
        {
            if (strcmp(specialbuf1, "") && strcmp(specialbuf2, ""))
            {
                Sprintf(specialbuf, " (%s, %s)", specialbuf1, specialbuf2);
            }
            else
                Sprintf(specialbuf, " (%s)", strcmp(specialbuf1, "") ? specialbuf1 : specialbuf2);
        }

        Sprintf(buf, " %2d - %s%s%s%s", i + 1, attypebuf, adtypebuf, damagebuf, specialbuf);
        
        putstr(datawin, ATR_INDENT_AT_DASH | ATR_ORDERED_LIST, buf);

    }

    print_monster_statistics(datawin, mon, ptr);
    print_monster_intrinsics(datawin, mon, ptr);
    print_monster_wearables(datawin, mon, ptr);
    print_monster_status(datawin, mon);

    display_nhwindow(datawin, FALSE);
    destroy_nhwindow(datawin), datawin = WIN_ERR;

    return 0;
}


STATIC_VAR const char* monster_size_names[] = {
    "tiny", "small", "medium-sized", "large", "huge",
    "gigantic", "gigantic", "gigantic",
};

const char* get_monster_size_text(monsize)
int monsize;
{
    if (monsize < 0 || monsize >= SIZE(monster_size_names))
        return empty_string;

    return monster_size_names[monsize];
}


STATIC_VAR const char* attack_type_names[] = {
    "none", "claw", "bite", "kick", "ram",
    "tail", "butt", "touch", "sting", "grab", "spit",
    "engulf", "breath weapon", "explosion", "explosion",
    "gaze", "tentacle", "eye stalk", "horn", "tusk"
};

const char* get_attack_type_text(aatyp)
int aatyp;
{
    if (aatyp == AT_SMMN)
        return "summoning";
    else if (aatyp == AT_WEAP)
        return "by weapon";
    else if (aatyp == AT_MAGC)
        return "spell casting";
    else if (aatyp == AT_PASV)
        return "passive defense";
    else if (aatyp < 0 || aatyp >= SIZE(attack_type_names))
        return empty_string;

    return attack_type_names[aatyp];
}



/* Called when a boulder is dropped, thrown, or pushed.  If it ends up
 * in a pool, it either fills the pool up or sinks away.  In either case,
 * it's gone for good...  If the destination is not a pool, returns FALSE.
 */
boolean
boulder_hits_pool(otmp, rx, ry, pushing)
struct obj *otmp;
register int rx, ry;
boolean pushing;
{
    if (!otmp || otmp->otyp != BOULDER) 
    {
        impossible("Not a boulder?");
    }
    else if (!Is_waterlevel(&u.uz) && is_pool_or_lava(rx, ry)) 
    {
        boolean lava = is_lava(rx, ry), fills_up;
        const char *what = waterbody_name(rx, ry);
        schar ltyp = levl[rx][ry].typ;
        int chance = rn2(10); /* water: 90%; lava: 10% */
        fills_up = lava ? chance == 0 : chance != 0;

        play_object_floor_sound_at_location(otmp, OBJECT_SOUND_TYPE_DROP, rx, ry, FALSE);

        if (fills_up) 
        {
            struct trap *ttmp = t_at(rx, ry);

            if (ltyp == DRAWBRIDGE_UP)
            {
                levl[rx][ry].drawbridgemask &= ~DB_UNDER; /* clear lava */
                levl[rx][ry].drawbridgemask |= DB_GROUND;
            }
            else
                create_basic_floor_location(rx, ry, GROUND, GROUND_SUBTYPE_SWAMPY, 0, FALSE);
                
            //levl[rx][ry].typ = ROOM, levl[rx][ry].flags = 0;

            if (ttmp)
                (void) delfloortrap(ttmp);

            bury_objs(rx, ry);

            newsym(rx, ry);
            if (pushing) {
                char whobuf[BUFSZ];

                Strcpy(whobuf, "you");
                if (u.usteed)
                    Strcpy(whobuf, y_monnam(u.usteed));
                pline("%s %s %s into the %s.", upstart(whobuf),
                      vtense(whobuf, "push"), the(xname(otmp)), what);
                if (flags.verbose && !Blind)
                    pline("Now you can cross it!");
                /* no splashing in this case */
            }
        }
        if (!fills_up || !pushing) 
        { /* splashing occurs */
            if (!u.uinwater)
            {
                if (pushing ? !Blind : cansee(rx, ry)) 
                {
                    There("is a large splash as %s %s the %s.",
                          the(xname(otmp)), fills_up ? "fills" : "falls into",
                          what);
                } else if (!Deaf)
                    You_hear("a%s splash.", lava ? " sizzling" : "");
                wake_nearto(rx, ry, 40);
            }

            if (fills_up && u.uinwater && distu(rx, ry) == 0) 
            {
                u.uinwater = 0;
                docrt();
                vision_full_recalc = 1;
                play_environment_ambient_sounds();
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "find yourself on dry land again!");
            }
            else if (lava && distu(rx, ry) <= 2)
            {
                int dmg;
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are hit by molten %s%c",
                    hliquid("lava"), Fire_immunity ? '.' : '!');
                burn_away_slime();
                dmg = d(3, 6);
                losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_FIRE, ADFLAGS_NONE), /* lava damage */
                       "molten lava", KILLED_BY);
            }
            else if (!fills_up && flags.verbose
                       && (pushing ? !Blind : cansee(rx, ry)))
                pline("It sinks without a trace!");
        }

        /* boulder is now gone */
        debugprint("boulder_hits_pool: %d", otmp->otyp);
        if (pushing)
            delobj(otmp);
        else
            obfree(otmp, (struct obj *) 0);
        return TRUE;
    }
    return FALSE;
}

/* Used for objects which sometimes do special things when dropped; must be
 * called with the object not in any chain.  Returns TRUE if the object goes
 * away.
 */
boolean
flooreffects(obj, x, y, verb)
struct obj *obj;
int x, y;
const char *verb;
{
    struct trap *t;
    struct monst *mtmp;
    struct obj *otmp;
    boolean tseen;
    int ttyp = NO_TRAP;

    if (obj->where != OBJ_FREE)
    {
        panic("flooreffects: obj not free");
        return FALSE;
    }
    /* make sure things like water_damage() have no pointers to follow */
    obj->nobj = obj->nexthere = (struct obj *) 0;

    if (obj->otyp == BOULDER && boulder_hits_pool(obj, x, y, FALSE))
    {
        return TRUE;
    }
    else if (obj->otyp == BOULDER && (t = t_at(x, y)) != 0
               && (is_pit(t->ttyp) || is_hole(t->ttyp)))
    {
        ttyp = t->ttyp;
        tseen = t->tseen ? TRUE : FALSE;
        if (((mtmp = m_at(x, y)) && mtmp->mtrapped)
            || (u.utrap && u.ux == x && u.uy == y)) 
        {
            if (*verb && (cansee(x, y) || distu(x, y) == 0))
            {
                play_simple_object_sound_at_location(obj, x, y, OBJECT_SOUND_TYPE_TUMBLE_DOWNWARDS);
                pline("%s boulder %s into the pit%s.",
                    Blind ? "A" : "The",
                    vtense((const char*)0, verb),
                    mtmp ? "" : " with you");
            }

            if (mtmp)
            {
                if (!passes_walls(mtmp->data) && !throws_rocks(mtmp->data))
                {
                    /* dieroll was rnd(20); 1: maximum chance to hit
                       since trapped target is a sitting duck */
                    int dmg, dieroll = 1;

                    /* 3.6.2: this was calling hmon() unconditionally
                       so always credited/blamed the hero but the boulder
                       might have been thrown by a giant or launched by
                       a rolling boulder trap triggered by a monster or
                       dropped by a scroll of earth read by a monster */
                    if (context.mon_moving) 
                    {
                        /* normally we'd use ohitmon() but it can call
                           drop_throw() which calls flooreffects() */
                        if (is_launcher(obj))
                            dmg = d(1, 2);
                        else
                            dmg = weapon_total_dmg_value(obj, mtmp, &youmonst, 0);

                        deduct_monster_hp(mtmp, adjust_damage(dmg, (struct monst*)0, mtmp, objects[obj->otyp].oc_damagetype, ADFLAGS_NONE));
                        if (DEADMONSTER(mtmp)) 
                        {
                            if (canspotmon(mtmp))
                                pline("%s is %s!", Monnam(mtmp),
                                      (is_not_living(mtmp->data) || is_vampshifter(mtmp))
                                      ? "destroyed" : "killed");
                            mondied(mtmp);
                        }
                    }
                    else 
                    {
                        boolean obj_destroyed = FALSE;
                        (void) hmon(mtmp, obj, HMON_THROWN, dieroll, &obj_destroyed);
                        if (obj_destroyed)
                            obj = 0;
                    }

                    if (!DEADMONSTER(mtmp) && !is_whirly(mtmp->data))
                        return FALSE; /* still alive */
                }
                mtmp->mtrapped = 0;
            } 
            else 
            {
                if (!Passes_walls && !throws_rocks(youmonst.data)) 
                {
                    losehp(adjust_damage(rnd(15), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE),
                           "squished under a boulder", NO_KILLER_PREFIX);
                    return FALSE; /* player remains trapped */
                } else
                    reset_utrap(TRUE);
            }
        }

        if (*verb)
        {
            if (Blind && (x == u.ux) && (y == u.uy))
            {
                You_hear("a CRASH! beneath you.");
            }
            else if (!Blind && cansee(x, y))
            {
                pline_The("boulder %s%s.",
                    (ttyp == TRAPDOOR && !tseen)
                    ? "triggers and " : "",
                    (ttyp == TRAPDOOR)
                    ? "plugs a trap door"
                    : (ttyp == HOLE) ? "plugs a hole"
                    : "fills a pit");
            }
            else
            {
                You_hear("a boulder %s.", verb);
            }

            if (iflags.using_gui_sounds && !Deaf)
            {
                if ((ttyp == TRAPDOOR && !tseen))
                {
                    play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, x, y);
                }
                play_sfx_sound_at_location(ttyp == TRAPDOOR || ttyp == HOLE ? SFX_BOULDER_PLUGS_HOLE_OR_TRAPDOOR : SFX_BOULDER_FILLS_PIT, x, y);
            }
        }
        /*
         * Note:  trap might have gone away via ((hmon -> killed -> xkilled)
         *  || mondied) -> mondead -> m_detach -> fill_pit.
         */
        if ((t = t_at(x, y)) != 0)
            deltrap(t);
        if (obj)
        {
            debugprint("flooreffects: %d", obj->otyp);
            useupf(obj, 1L);
        }
        bury_objs(x, y);
        newsym(x, y);
        return TRUE;
    } 
    else if (is_lava(x, y))
    {
        return lava_damage(obj, x, y);
    } 
    else if (is_pool(x, y)) 
    {
        /* Reasonably bulky objects (arbitrary) splash when dropped.
         * If you're floating above the water even small things make
         * noise.  Stuff dropped near fountains always misses */
        if ((Blind || Moves_above_ground) && !Deaf
            && ((x == u.ux) && (y == u.uy))) 
        {
            if (!Underwater) 
            {
                play_object_floor_sound_at_location(obj, OBJECT_SOUND_TYPE_DROP, x, y, Underwater);
                if (weight(obj) > 9) 
                {
                    pline("Splash!");
                } 
                else if (Moves_above_ground)
                {
                    pline("Plop!");
                }
            }
            map_background(x, y, 0);
            newsym(x, y);
        }
        return water_damage(obj, NULL, FALSE) == ER_DESTROYED;
    }
    else if (u.ux == x && u.uy == y && (t = t_at(x, y)) != 0
               && uteetering_at_seen_pit(t)) 
    {
        play_simple_object_sound_at_location(obj, x, y, OBJECT_SOUND_TYPE_TUMBLE_DOWNWARDS);
        if (Blind && !Deaf)
        {
            You_hear("%s tumble downwards.", the(xname(obj)));
        }
        else
            pline("%s %s into %s pit.", The(xname(obj)),
                  otense(obj, "tumble"), the_your[t->madeby_u]);
    } 
    else if (obj->globby) 
    {
        /* Globby things like puddings might stick together */
        while (obj && (otmp = obj_nexto_xy(obj, x, y, TRUE)) != 0) 
        {
            pudding_merge_message(obj, otmp);
            /* intentionally not getting the melded object; obj_meld may set
             * obj to null. */
            (void) obj_meld(&obj, &otmp);
        }
        return (boolean) !obj;
    }
    return FALSE;
}

/* obj is an object dropped on an altar */
void
doaltarobj(obj)
register struct obj *obj;
{
    if (Blind)
        return;

    if (obj->oclass != COIN_CLASS) {
        /* KMH, conduct */
        if (!u.uconduct.gnostic++)
            livelog_printf(LL_CONDUCT,
                "eschewed atheism, by dropping %s on an altar",
                doname(obj));
    } else {
        /* coins don't have bless/curse status */
        obj->blessed = obj->cursed = 0;
    }

    if (obj->blessed || obj->cursed) {
        const char* hclr = hcolor_multi_buf1(obj->blessed ? NH_AMBER : NH_BLACK);
        //multicolor_buffer[2] = multicolor_buffer[1];
        There_ex(ATR_NONE, Hallucination ? CLR_MSG_HALLUCINATED : obj->blessed ? CLR_MSG_POSITIVE : CLR_MSG_NEGATIVE, // no_multiattrs, multicolor_buffer, 
            "is %s%s%s as %s %s the altar.",
            an_prefix(hclr), hclr, " flash", doname(obj),
              otense(obj, "hit"));
        if (!Hallucination)
            obj->bknown = 1;
    } else {
        pline("%s %s on the altar.", Doname2(obj), otense(obj, "land"));
        if (obj->oclass != COIN_CLASS)
            obj->bknown = 1;
    }
}

STATIC_OVL void
trycall(obj)
register struct obj *obj;
{
    if (!objects[obj->otyp].oc_name_known && !objects[obj->otyp].oc_uname)
        docall(obj, (char*)0);
}

/* Transforms the sink at the player's position into
   a fountain, throne, altar or grave. */
STATIC_DCL void
polymorph_sink()
{
    uchar sym = S_sink;
    boolean sinklooted;

    if (levl[u.ux][u.uy].typ != SINK)
        return;

    sinklooted = levl[u.ux][u.uy].looted != 0;

    switch (rn2(4)) 
    {
    default:
    case 0:
        sym = S_fountain;
        create_simple_location(u.ux, u.uy, FOUNTAIN, FOUNTAIN_MAGIC + rn2(MAX_FOUNTAIN_SUBTYPES - FOUNTAIN_MAGIC), 0, 0, 0, levl[u.ux][u.uy].floortyp, levl[u.ux][u.uy].floorsubtyp, levl[u.ux][u.uy].floorvartyp, FALSE);
        if (sinklooted)
            SET_FOUNTAIN_LOOTED(u.ux, u.uy);
        break;
    case 1:
        sym = S_throne;
        create_simple_location(u.ux, u.uy, THRONE, 0, 0, sinklooted ? T_LOOTED : 0, 0, levl[u.ux][u.uy].floortyp, levl[u.ux][u.uy].floorsubtyp, levl[u.ux][u.uy].floorvartyp, FALSE);
        break;
    case 2:
        sym = S_altar;
        create_simple_location(u.ux, u.uy, ALTAR, 0, 0, Align2amask(rn2((int)A_LAWFUL + 2) - 1), 0, levl[u.ux][u.uy].floortyp, levl[u.ux][u.uy].floorsubtyp, levl[u.ux][u.uy].floorvartyp, FALSE);
        break;
    case 3:
        if (levl[u.ux][u.uy].floortyp)
        {
            sym = location_type_definitions[levl[u.ux][u.uy].floortyp].base_screen_symbol;
            create_simple_location(u.ux, u.uy, levl[u.ux][u.uy].floortyp, levl[u.ux][u.uy].floorsubtyp, levl[u.ux][u.uy].floorvartyp, 0, 0, 0, 0, 0, FALSE);
        }
        else
        {
            sym = S_room;
            create_basic_floor_location(u.ux, u.uy, ROOM, FLOOR_SUBTYPE_NORMAL, 0, FALSE);
        }
        make_grave(u.ux, u.uy, (char *) 0, FALSE);
        if (levl[u.ux][u.uy].typ == GRAVE)
            sym = S_grave;
        break;
    }

    /* give message even if blind; we know we're not levitating,
       so can feel the outcome even if we can't directly see it */
    if (levl[u.ux][u.uy].typ != ROOM)
        pline_The("sink transforms into %s!", an(defsyms[sym].explanation));
    else
    {
        play_sfx_sound(SFX_VANISHES_IN_PUFF_OF_SMOKE);
        pline_The("sink vanishes.");
    }
    newsym(u.ux, u.uy);
}

/* Teleports the sink at the player's position;
   return True if sink teleported. */
STATIC_DCL boolean
teleport_sink()
{
    int cx, cy;
    int cnt = 0;
    struct trap *trp;
    struct engr *eng;

    do {
        cx = rnd(COLNO - 1);
        cy = rn2(ROWNO);
        trp = t_at(cx, cy);
        eng = engr_at(cx, cy);
    } while (((levl[cx][cy].typ != ROOM && levl[cx][cy].typ != GRASS && levl[cx][cy].typ != GROUND) || trp || eng || cansee(cx, cy))
             && cnt++ < 200);

    if ((levl[cx][cy].typ == ROOM || levl[cx][cy].typ == GRASS || levl[cx][cy].typ == GROUND) && !trp && !eng) {
        /* create sink at new position */
        create_simple_location(cx, cy, SINK, levl[u.ux][u.uy].subtyp, levl[u.ux][u.uy].vartyp, levl[u.ux][u.uy].looted, 0, levl[cx][cy].floortyp, levl[cx][cy].floorsubtyp, levl[cx][cy].floorvartyp, TRUE);
        /* remove old sink */
        create_current_floor_location(u.ux, u.uy, 0, NO_GLYPH, TRUE);
        return TRUE;
    }
    return FALSE;
}

/* obj is a ring being dropped over a kitchen sink */
STATIC_OVL void
dosinkring(obj)
register struct obj *obj;
{
    struct obj *otmp, *otmp2;
    boolean ideed = TRUE;
    boolean nosink = FALSE;

    You("drop %s down the drain.", doname(obj));
    obj->in_use = TRUE;  /* block free identification via interrupt */
    switch (obj->otyp) { /* effects that can be noticed without eyes */
    case RIN_SEARCHING:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "thought %s got lost in the sink, but there it is!", yname(obj));
        goto giveback;
    case RIN_SLOW_DIGESTION:
        pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "ring is regurgitated!");
 giveback:
        obj->in_use = FALSE;
        dropxf(obj);
        trycall(obj);
        return;
    case RIN_LEVITATION:
        pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "sink quivers upward for a moment.");
        break;
    case RIN_POISON_RESISTANCE:
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "smell rotten %s.", makeplural(fruitname(FALSE)));
        break;
    case RIN_AGGRAVATE_MONSTER:
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Several %s buzz angrily around the sink.",
              Hallucination ? makeplural(rndmonnam(NULL)) : "flies");
        break;
    case RIN_SHOCK_RESISTANCE:
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Static electricity surrounds the sink.");
        break;
    case RIN_PROTECTION_FROM_UNDEATH:
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A pillar of smoke arises from the sink.");
        break;
    case RIN_SEVEN_CHARGES: /* Artifact version of ring of conflict */
        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "loud noises coming from the drain.");
        break;
    case RIN_SUSTAIN_ABILITY: /* KMH */
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flow seems fixed.", hliquid("water"));
        break;
    case RIN_GAIN_STRENGTH:
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flow seems %ser now.",
                  hliquid("water"),
                  (obj->enchantment < 0) ? "weak" : "strong");
        break;
    case RIN_GAIN_DEXTERITY:
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flow seems %ser now.",
            hliquid("water"),
            (obj->enchantment < 0) ? "stiff" : "nimbl");
        break;
    case RIN_GAIN_CONSTITUTION:
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flow seems %ser now.",
                  hliquid("water"),
                  (obj->enchantment < 0) ? "less" : "great");
        break;
    case RIN_GAIN_INTELLIGENCE:
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flow seems %ser now.",
            hliquid("water"),
            (obj->enchantment < 0) ? "coars" : "fin");
        break;
    case RIN_GAIN_WISDOM:
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flow seems %s now.",
            hliquid("water"),
            (obj->enchantment < 0) ? "less sensible" : "more sensible");
        break;
    case RIN_POWER:
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flow seems %s now.",
            hliquid("water"),
            (obj->enchantment < 0) ? "much worse" : "much better");
        break;
    case RIN_INCREASE_ACCURACY: /* KMH */
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s flow %s the drain.",
                  hliquid("water"),
                  (obj->enchantment < 0) ? "misses" : "hits");
        break;
    case RIN_INCREASE_DAMAGE:
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "water's force seems %ser now.",
                  (obj->enchantment < 0) ? "small" : "great");
        break;
    case RIN_HUNGER:
        ideed = FALSE;
        for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp2) {
            otmp2 = otmp->nexthere;
            if (otmp != uball && otmp != uchain
                && !obj_resists(otmp, 1, 99)) {
                if (!Blind) {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly, %s %s from the sink!", doname(otmp),
                          otense(otmp, "vanish"));
                    ideed = TRUE;
                }
                debugprint("dosinkring: %d", otmp->otyp);
                delobj(otmp);
            }
        }
        break;
    case MEAT_RING:
        /* Not the same as aggravate monster; besides, it's obvious. */
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Several flies buzz around the sink.");
        break;
    case RIN_TELEPORTATION:
        nosink = teleport_sink();
        /* give message even if blind; we know we're not levitating,
           so can feel the outcome even if we can't directly see it */
        pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "sink %svanishes.", nosink ? "" : "momentarily ");
        ideed = FALSE;
        break;
    case RIN_POLYMORPH:
        polymorph_sink();
        nosink = TRUE;
        /* for S_room case, same message as for teleportation is given */
        ideed = (levl[u.ux][u.uy].typ != ROOM);
        break;
    default:
        ideed = FALSE;
        break;
    }
    if (!Blind && !ideed) {
        ideed = TRUE;
        switch (obj->otyp) { /* effects that need eyes */
        case RIN_ADORNMENT:
            pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "faucets flash brightly for a moment.");
            break;
        case RIN_REGENERATION:
            pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "sink looks as good as new.");
            break;
        case RIN_REPLENISHMENT:
            pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "sink shines with a blue aura for a while.");
            break;
        case RIN_INVISIBILITY:
            You("don't see anything happen to the sink.");
            break;
        case RIN_FREE_ACTION:
            You_see("the ring slide right down the drain!");
            break;
        case RIN_SEE_INVISIBLE:
            You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "some %s in the sink.",
                    Hallucination ? "oxygen molecules" : "air");
            break;
        case RIN_STEALTH:
            pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "sink seems to blend into the floor for a moment.");
            break;
        case RIN_FIRE_RESISTANCE:
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "hot %s faucet flashes brightly for a moment.",
                      hliquid("water"));
            break;
        case RIN_COLD_RESISTANCE:
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "cold %s faucet flashes brightly for a moment.",
                      hliquid("water"));
            break;
        case RIN_PROTECTION_FROM_SHAPE_CHANGERS:
            pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "sink looks nothing like a fountain.");
            break;
        case RIN_PROTECTION:
        {
            pline_The_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "sink glows %s for a moment.",
                hcolor_multi_buf0((obj->enchantment < 0) ? NH_BLACK : NH_SILVER));
            //pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "sink glows %s for a moment.",
            //    hcolor((obj->enchantment < 0) ? NH_BLACK : NH_SILVER));
            break;
        }
        case RIN_WARNING:
            pline_The_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, multicolor_buffer, "sink glows %s for a moment.", hcolor_multi_buf0(NH_WHITE));
            break;
        case RIN_TELEPORT_CONTROL:
            pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "sink looks like it is being beamed aboard somewhere.");
            break;
        case RIN_POLYMORPH_CONTROL:
            pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION,
                  "sink momentarily looks like a regularly erupting geyser.");
            break;
        default:
            break;
        }
    }
    if (ideed)
        trycall(obj);
    else if (!nosink)
        You_hear("the ring bouncing down the drainpipe.");

    if (!rn2(20) && !nosink) {
        pline_The("sink backs up, leaving %s.", doname(obj));
        obj->in_use = FALSE;
        dropxf(obj);
    } else if (!rn2(5)) {
        freeinv(obj);
        obj->in_use = FALSE;
        obj->ox = u.ux;
        obj->oy = u.uy;
        add_to_buried(obj);
    }
    else
    {
        debugprint("dosinkring: %d", obj->otyp);
        useup(obj);
    }
}

/* some common tests when trying to drop or throw items */
boolean
canletgo(obj, word)
struct obj *obj;
const char *word;
{
    if (obj->owornmask & W_WORN_NOT_WIELDED) {
        if (*word)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            Norep_ex(ATR_NONE, CLR_MSG_FAIL, "You cannot %s %s you are wearing.", word, something);
        }
        return FALSE;
    }
    if (objects[obj->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED && obj->cursed) {
        /* getobj() kludge sets corpsenm to user's specified count
           when refusing to split a stack of cursed loadstones */
        if (*word) {
            /* getobj() ignores a count for throwing since that is
               implicitly forced to be 1; replicate its kludge... */
            if (!strcmp(word, "throw") && obj->quan > 1L)
                obj->corpsenm = 1;
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "For some reason, you cannot %s%s %s%s!", word,
                  obj->corpsenm ? " any of" : "", is_graystone(obj) ? "the stone" : "the item", plur(obj->quan));
        }
        obj->corpsenm = 0; /* reset */
        if (!obj->bknown)
        {
            obj->bknown = TRUE;
            update_inventory();
        }
        return FALSE;
    }
    if (obj->otyp == LEASH && obj->leashmon != 0) {
        if (*word)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "leash is tied around your %s.", body_part(HAND));
        }
        return FALSE;
    }
    if (obj->owornmask & W_SADDLE) {
        if (*word)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot %s %s you are sitting on.", word, something);
        }
        return FALSE;
    }
    return TRUE;
}

int
drop(obj)
register struct obj *obj;
{
    if (!obj)
        return 0;
    if (!canletgo(obj, "drop"))
        return 0;
    if (obj == uwep) {
        if (welded(uwep, &youmonst)) {
            weldmsg(obj);
            return 0;
        }
        setuwep((struct obj *) 0, W_WEP);
    }
    if (obj == uarms) {
        if (welded(uarms, &youmonst)) {
            weldmsg(obj);
            return 0;
        }
        setuwep((struct obj*) 0, W_ARMS);
    }
    if (obj == uquiver) {
        setuqwep((struct obj *) 0);
    }
    if (obj == uswapwep) {
        setuswapwep((struct obj *) 0, W_SWAPWEP);
    }
    if (obj == uswapwep2) {
        setuswapwep((struct obj*) 0, W_SWAPWEP2);
    }

    if (u.uswallow) {
        /* barrier between you and the floor */
        if (flags.verbose) {
            char *onam_p, monbuf[BUFSZ];

            /* doname can call s_suffix, reusing its buffer */
            Strcpy(monbuf, s_suffix(mon_nam(u.ustuck)));
            onam_p = is_unpaid(obj) ? yobjnam(obj, (char *) 0) : doname(obj);
            You("drop %s into %s %s.", onam_p, monbuf,
                mbodypart(u.ustuck, STOMACH));
        }
    } else {
        if ((obj->oclass == RING_CLASS || obj->otyp == MEAT_RING)
            && IS_SINK(levl[u.ux][u.uy].typ)) {
            dosinkring(obj);
            return 1;
        }
        if (!can_reach_floor(TRUE)) {
            /* we might be levitating due to #invoke Heart of Ahriman;
               if so, levitation would end during call to freeinv()
               and we want hitfloor() to happen before float_down() */
            //boolean levhack = finesse_ahriman(obj);

            //if (levhack)
             //   ELevitation = W_ARTIFACT_CARRIED; /* other than W_ARTIFACT_INVOKED */
            if (flags.verbose)
                You("drop %s.", doname(obj));
            /* Ensure update when we drop gold objects */
            if (obj->oclass == COIN_CLASS)
                context.botl = 1;
            freeinv(obj);
            hitfloor(obj, TRUE);
            //if (levhack)
            //    float_down(I_SPECIAL | TIMEOUT, W_ARTIFACT_INVOKED | W_ARTIFACT_CARRIED);
            return 1;
        }
        if (!IS_ALTAR(levl[u.ux][u.uy].typ) && flags.verbose)
            You("drop %s.", doname(obj));
    }
    dropx(obj);
    return 1;
}

int
autobag(obj) /* To inventory */
register struct obj* obj;
{
    return auto_bag_in(invent, obj, FALSE);
}

/* dropx - take dropped item out of inventory;
   called in several places - may produce output
   (eg ship_object() and dropy() -> sellobj() both produce output) */
void
dropx(obj)
register struct obj *obj;
{
    /* Ensure update when we drop gold objects */
    if (obj->oclass == COIN_CLASS)
        context.botl = 1;
    freeinv(obj);
    if (!u.uswallow) {
        if (ship_object(obj, u.ux, u.uy, FALSE))
            return;
        if (IS_ALTAR(levl[u.ux][u.uy].typ))
            doaltarobj(obj); /* set bknown */
    }
    dropy(obj);
}

/* dropxf - take dropped item out of inventory;
   called in several places - may produce output
   (eg ship_object() and dropy() -> sellobj() both produce output) */
void
dropxf(obj)
register struct obj* obj;
{
    /* Ensure update when we drop gold objects */
    if (obj->oclass == COIN_CLASS)
        context.botl = 1;
    freeinv(obj);
    obj_set_found(obj);
    if (!u.uswallow) {
        if (ship_object(obj, u.ux, u.uy, FALSE))
            return;
        if (IS_ALTAR(levl[u.ux][u.uy].typ))
            doaltarobj(obj); /* set bknown */
    }
    dropy(obj);
}

/* dropy - put dropped object at destination; called from lots of places */
void
dropy(obj)
struct obj *obj;
{
    dropz(obj, FALSE);
}

/* dropyf - put dropped object at destination; called from lots of places */
void
dropyf(obj)
struct obj* obj;
{
    obj_set_found(obj);
    dropz(obj, FALSE);
}

/* dropz - really put dropped object at its destination... */
void
dropz(obj, with_impact)
struct obj *obj;
boolean with_impact;
{
    if (obj == uwep)
        setuwep((struct obj *) 0, W_WEP);
    if (obj == uarms)
        setuwep((struct obj*) 0, W_ARMS);
    if (obj == uquiver)
        setuqwep((struct obj *) 0);
    if (obj == uswapwep)
        setuswapwep((struct obj *) 0, W_SWAPWEP);
    if (obj == uswapwep2)
        setuswapwep((struct obj*) 0, W_SWAPWEP2);

    if (!u.uswallow && flooreffects(obj, u.ux, u.uy, "drop"))
    {
        if (iflags.using_gui_sounds && !ui_has_input())
        {
            play_object_floor_sound_at_location(obj, OBJECT_SOUND_TYPE_DROP, u.ux, u.uy, Underwater);
            delay_output_milliseconds(ITEM_PICKUP_DROP_DELAY);
        }
        return;
    }
    /* uswallow check done by GAN 01/29/87 */
    if (u.uswallow) 
    {
        boolean could_petrify = FALSE;
        boolean could_poly = FALSE;
        boolean could_slime = FALSE;
        boolean could_grow = FALSE;
        boolean could_heal = FALSE;

        if (obj != uball) 
        { /* mon doesn't pick up ball */
            if (obj->otyp == CORPSE)
            {
                could_petrify = obj->corpsenm >= LOW_PM && touch_petrifies(&mons[obj->corpsenm]);
                could_poly = polyfodder(obj);
                could_slime = (obj->corpsenm == PM_GREEN_SLIME);
                could_grow = (obj->corpsenm == PM_WRAITH || obj->corpsenm == PM_SPECTRE || obj->corpsenm == PM_KING_WRAITH);
                could_heal = (obj->corpsenm == PM_NURSE);
            }

            if (is_unpaid(obj))
                (void) stolen_value(obj, u.ux, u.uy, TRUE, FALSE);

            int was_obj_freed = mpickobj(u.ustuck, obj);

            if (is_animal(u.ustuck->data)) 
            {
                if (could_poly || could_slime) 
                {
                    (void) newcham(u.ustuck, could_poly ? (struct permonst *) 0 : &mons[PM_GREEN_SLIME], 0,
                                   FALSE, could_slime);
                    if (!was_obj_freed)
                    {
                        debugprint("dropz1: %d", obj->otyp);
                        delobj(obj); /* corpse is digested */
                    }
                }
                else if (could_petrify) 
                {
                    int existing_stoning = get_mon_property(u.ustuck, STONED);
                    (void)set_mon_property_verbosely(u.ustuck, STONED, max(1, min(existing_stoning - 1, 5)));
                    //minstapetrify(u.ustuck, TRUE);
                    /* Don't leave a cockatrice corpse in a statue */
                    if (!u.uswallow && !was_obj_freed)
                    {
                        debugprint("dropz2: %d", obj->otyp);
                        delobj(obj);
                    }
                } 
                else if (could_grow)
                {
                    (void) grow_up(u.ustuck, (struct monst *) 0);
                    if (!was_obj_freed)
                    {
                        debugprint("dropz3: %d", obj->otyp);
                        delobj(obj); /* corpse is digested */
                    }
                } 
                else if (could_heal)
                {
                    u.ustuck->mhp = u.ustuck->mhpmax;
                    if (!was_obj_freed)
                    {
                        debugprint("dropz4: %d", obj->otyp);
                        delobj(obj); /* corpse is digested */
                    }
                }
            }
        }
    } 
    else
    {
        place_object(obj, u.ux, u.uy);
        if (iflags.using_gui_sounds && !ui_has_input())
        {
            play_object_floor_sound(obj, OBJECT_SOUND_TYPE_DROP, Underwater);
            delay_output_milliseconds(ITEM_PICKUP_DROP_DELAY);
        }
        if (with_impact)
            container_impact_dmg(obj, u.ux, u.uy);
        if (obj == uball)
            drop_ball(u.ux, u.uy);
        else if (level.flags.has_shop)
            sellobj(obj, u.ux, u.uy);
        stackobj(obj);
        if (Blind && Levitation)
        {
            map_object(obj, 0);
        }
        newsym(u.ux, u.uy); /* remap location under self */
    }
}

/* things that must change when not held; recurse into containers.
   Called for both player and monsters */
void
obj_no_longer_held(obj)
struct obj *obj;
{
    if (!obj) {
        return;
    } else if (Has_contents(obj)) {
        struct obj *contents;

        for (contents = obj->cobj; contents; contents = contents->nobj)
            obj_no_longer_held(contents);
    }
#if 0
    switch (obj->otyp) {
    case CRYSKNIFE:
        /* Normal crysknife reverts to worm tooth when not held by hero
         * or monster; fixed crysknife has only 10% chance of reverting.
         * When a stack of the latter is involved, it could be worthwhile
         * to give each individual crysknife its own separate 10% chance,
         * but we aren't in any position to handle stack splitting here.
         */
        if (!obj->oerodeproof || !rn2(10)) {
            /* if monsters aren't moving, assume player is responsible */
            if (!context.mon_moving && !program_state.gameover)
                costly_alteration(obj, COST_DEGRD);
            obj->otyp = WORM_TOOTH;
            obj->material = objects[obj->otyp].oc_material;
            obj->oerodeproof = 0;
        }
        break;
    }
#endif
}

/* '%' command: drop several things */
int
dodropmany()
{
    int result = 0;

    if (!invent) {
        You("have nothing to drop.");
        return 0;
    }
    add_valid_menu_class(0); /* clear any classes already there */
    if (*u.ushops)
        sellobj_state(SELL_DELIBERATE);

    int n, n_dropped = 0, i;
    int64_t cnt;
    struct obj* otmp, * otmp2;
    menu_item* pick_list = (menu_item*)0;

    /* should coordinate with perm invent, maybe not show worn items */
    n = query_objlist("What would you like to drop?", &invent,
        (USE_INVLET | INVORDER_SORT | OBJECT_COMPARISON), &pick_list, PICK_ANY, allow_all, SHOWWEIGHTS_DROP);

    if (n > 0 && pick_list)
    {
        /*
         * picklist[] contains a set of pointers into inventory, but
         * as soon as something gets dropped, they might become stale
         * (see the drop_everything code above for an explanation).
         * Just checking to see whether one is still in the invent
         * chain is not sufficient validation since destroyed items
         * will be freed and items we've split here might have already
         * reused that memory and put the same pointer value back into
         * invent.  Ditto for using invlet to validate.  So we start
         * by setting bypass on all of invent, then check each pointer
         * to verify that it is in invent and has that bit set.
         */
        bypass_objlist(invent, TRUE);
        for (i = 0; i < n; i++)
        {
            otmp = pick_list[i].item.a_obj;
            for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
                if (otmp2 == otmp)
                    break;
            if (!otmp2 || !otmp2->bypass)
                continue;
            /* found next selected invent item */
            cnt = pick_list[i].count;
            if (cnt < otmp->quan)
            {
                if (welded(otmp, &youmonst))
                {
                    ; /* don't split */
                }
                else if ((objects[otmp->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && otmp->cursed)
                {
                    /* same kludge as getobj(), for canletgo()'s use */
                    otmp->corpsenm = (int)cnt; /* don't split */
                }
                else
                {
                    otmp = splitobj(otmp, cnt);
                }
            }
            n_dropped += drop(otmp);
        }
        bypass_objlist(invent, FALSE); /* reset invent to normal */
        free((genericptr_t)pick_list);
    }
    result = n_dropped;

    if (*u.ushops)
        sellobj_state(SELL_NORMAL);
    if (result)
        reset_occupations();

    return result;
}

/* 'D' command: drop several things */
int
doddrop()
{
    int result = 0;

    if (!invent) {
        You("have nothing to drop.");
        return 0;
    }
    add_valid_menu_class(0); /* clear any classes already there */
    if (*u.ushops)
        sellobj_state(SELL_DELIBERATE);
    if (flags.menu_style != MENU_TRADITIONAL
        || (result = ggetobj("drop", drop, 0, FALSE, (unsigned *) 0, 3)) < -1)
        result = menu_drop(result);
    if (*u.ushops)
        sellobj_state(SELL_NORMAL);
    if (result)
        reset_occupations();

    return result;
}

/* Drop things from the hero's inventory, using a menu. */
STATIC_OVL int
menu_drop(retry)
int retry;
{
    int n, i, n_dropped = 0;
    int64_t cnt;
    struct obj *otmp, *otmp2;
    menu_item* pick_list = (menu_item*)0;
    boolean all_categories = TRUE;
    boolean drop_everything = FALSE;

    if (retry) 
    {
        all_categories = (retry == -2);
    } 
    else if (flags.menu_style == MENU_FULL) 
    {
        all_categories = FALSE;
        n = query_category("Drop what type of items?", invent,
                           UNPAID_TYPES | UNIDENTIFIED_TYPES | UNKNOWN_TYPES | ALL_TYPES | CHOOSE_ALL | BUC_BLESSED
                               | BUC_CURSED | BUC_UNCURSED | BUC_UNKNOWN,
                           &pick_list, PICK_ANY, 0);
        if (!n)
            goto drop_done;
        for (i = 0; i < n; i++) 
        {
            if (pick_list[i].item.a_int == ALL_TYPES_SELECTED)
                all_categories = TRUE;
            else if (pick_list[i].item.a_int == 'A')
            {
                if (ParanoidAutoSelectAll)
                {
                    if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "All Items Selected", "Are you sure to drop all items?") != 'y')
                    {
                        free((genericptr_t)pick_list);
                        return 0;
                    }
                }

                drop_everything = TRUE;
            }
            else
                add_valid_menu_class(pick_list[i].item.a_int);
        }
        free((genericptr_t) pick_list);
    } 
    else if (flags.menu_style == MENU_COMBINATION) 
    {
        unsigned ggoresults = 0;

        all_categories = FALSE;
        /* Gather valid classes via traditional GnollHack method */
        i = ggetobj("drop", drop, 0, TRUE, &ggoresults, 3);
        if (i == -2)
            all_categories = TRUE;
        if (ggoresults & ALL_FINISHED) 
        {
            n_dropped = i;
            goto drop_done;
        }
    }

    if (drop_everything) 
    {
        /*
         * Dropping a burning potion of oil while levitating can cause
         * an explosion which might destroy some of hero's inventory,
         * so the old code
         *      for (otmp = invent; otmp; otmp = otmp2) {
         *          otmp2 = otmp->nobj;
         *          n_dropped += drop(otmp);
         *      }
         * was unreliable and could lead to an "object lost" panic.
         *
         * Use the bypass bit to mark items already processed (hence
         * not droppable) and rescan inventory until no unbypassed
         * items remain.
         */
        bypass_objlist(invent, FALSE); /* clear bypass bit for invent */
        while ((otmp = nxt_unbypassed_obj(invent)) != 0)
            n_dropped += drop(otmp);
        /* we might not have dropped everything (worn armor, welded weapon,
           cursed loadstones), so reset any remaining inventory to normal */
        bypass_objlist(invent, FALSE);
    } 
    else 
    {
        /* should coordinate with perm invent, maybe not show worn items */
        n = query_objlist("What would you like to drop?", &invent,
                          (USE_INVLET | INVORDER_SORT | OBJECT_COMPARISON), &pick_list, PICK_ANY,
                          all_categories ? allow_all : allow_category, SHOWWEIGHTS_DROP);
        if (n > 0 && pick_list)
        {
            /*
             * picklist[] contains a set of pointers into inventory, but
             * as soon as something gets dropped, they might become stale
             * (see the drop_everything code above for an explanation).
             * Just checking to see whether one is still in the invent
             * chain is not sufficient validation since destroyed items
             * will be freed and items we've split here might have already
             * reused that memory and put the same pointer value back into
             * invent.  Ditto for using invlet to validate.  So we start
             * by setting bypass on all of invent, then check each pointer
             * to verify that it is in invent and has that bit set.
             */
            bypass_objlist(invent, TRUE);
            for (i = 0; i < n; i++) 
            {
                otmp = pick_list[i].item.a_obj;
                for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
                    if (otmp2 == otmp)
                        break;
                if (!otmp2 || !otmp2->bypass)
                    continue;
                /* found next selected invent item */
                cnt = pick_list[i].count;
                if (cnt < otmp->quan) 
                {
                    if (welded(otmp, &youmonst))
                    {
                        ; /* don't split */
                    } 
                    else if ((objects[otmp->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && otmp->cursed) 
                    {
                        /* same kludge as getobj(), for canletgo()'s use */
                        otmp->corpsenm = (int) cnt; /* don't split */
                    }
                    else 
                    {
                        otmp = splitobj(otmp, cnt);
                    }
                }
                n_dropped += drop(otmp);
            }
            bypass_objlist(invent, FALSE); /* reset invent to normal */
            free((genericptr_t) pick_list);
        }
    }

 drop_done:
    return n_dropped;
}


/* autostash command: stash several things automatically */
int
doautostash()
{
    int result = 0;

    if (!invent) 
    {
        You("have nothing to stash.");
        return 0;
    }
    if (!count_bags_for_stashing(invent, invent, FALSE, FALSE))
    {
        You("have no appropriate containers to stash your items into.");
        return 0;
    }
    add_valid_menu_class(0); /* clear any classes already there */
    if (flags.menu_style != MENU_TRADITIONAL
        || (result = ggetobj("put in bag", autobag, 0, FALSE, (unsigned*)0, 3)) < -1)
        result = menu_autostash(result);
    if (result)
        reset_occupations();

    return result;
}

/* Drop things from the hero's inventory, using a menu. */
STATIC_OVL int
menu_autostash(retry)
int retry;
{
    int n, i, n_autobagged = 0;
    int64_t cnt;
    struct obj* otmp, * otmp2;
    menu_item* pick_list = (menu_item*)0;
    boolean all_categories = TRUE;
    boolean autobag_everything = FALSE;

    if (retry)
    {
        all_categories = (retry == -2);
    }
    else if (flags.menu_style == MENU_FULL)
    {
        all_categories = FALSE;
        n = query_category("Put what type of items into bag?", invent,
            UNPAID_TYPES | UNIDENTIFIED_TYPES | UNKNOWN_TYPES | ALL_TYPES | CHOOSE_ALL | BUC_BLESSED
            | BUC_CURSED | BUC_UNCURSED | BUC_UNKNOWN,
            &pick_list, PICK_ANY, 0);
        if (!n)
            goto autobag_done;
        for (i = 0; i < n; i++)
        {
            if (pick_list[i].item.a_int == ALL_TYPES_SELECTED)
                all_categories = TRUE;
            else if (pick_list[i].item.a_int == 'A')
            {
                if (ParanoidAutoSelectAll)
                {
                    if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "All Items Selected", "Are you sure to put all items into bag?") != 'y')
                    {
                        free((genericptr_t)pick_list);
                        return 0;
                    }
                }

                autobag_everything = TRUE;
            }
            else
                add_valid_menu_class(pick_list[i].item.a_int);
        }
        free((genericptr_t)pick_list);
    }
    else if (flags.menu_style == MENU_COMBINATION)
    {
        unsigned ggoresults = 0;

        all_categories = FALSE;
        /* Gather valid classes via traditional GnollHack method */
        i = ggetobj("put in bag", autobag, 0, TRUE, &ggoresults, 3);
        if (i == -2)
            all_categories = TRUE;
        if (ggoresults & ALL_FINISHED)
        {
            n_autobagged = i;
            goto autobag_done;
        }
    }

    if (autobag_everything)
    {
        /*
         * Dropping a burning potion of oil while levitating can cause
         * an explosion which might destroy some of hero's inventory,
         * so the old code
         *      for (otmp = invent; otmp; otmp = otmp2) {
         *          otmp2 = otmp->nobj;
         *          n_autobagged += drop(otmp);
         *      }
         * was unreliable and could lead to an "object lost" panic.
         *
         * Use the bypass bit to mark items already processed (hence
         * not autobaggable) and rescan inventory until no unbypassed
         * items remain.
         */
        bypass_objlist(invent, FALSE); /* clear bypass bit for invent */
        while ((otmp = nxt_unbypassed_obj(invent)) != 0)
            n_autobagged += auto_bag_in(invent, otmp, FALSE);
        /* we might not have autobagged everything (worn armor, welded weapon,
           cursed loadstones), so reset any remaining inventory to normal */
        bypass_objlist(invent, FALSE);
    }
    else
    {
        /* should coordinate with perm invent, maybe not show worn items */
        n = query_objlist("What would you like to put in bag?", &invent,
            (USE_INVLET | INVORDER_SORT | OBJECT_COMPARISON), &pick_list, PICK_ANY,
            all_categories ? allow_all : allow_category, SHOWWEIGHTS_DROP);
        if (n > 0 && pick_list)
        {
            /*
             * picklist[] contains a set of pointers into inventory, but
             * as soon as something gets autobagged, they might become stale
             * (see the autobag_everything code above for an explanation).
             * Just checking to see whether one is still in the invent
             * chain is not sufficient validation since destroyed items
             * will be freed and items we've split here might have already
             * reused that memory and put the same pointer value back into
             * invent.  Ditto for using invlet to validate.  So we start
             * by setting bypass on all of invent, then check each pointer
             * to verify that it is in invent and has that bit set.
             */
            bypass_objlist(invent, TRUE);
            for (i = 0; i < n; i++)
            {
                otmp = pick_list[i].item.a_obj;
                for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
                    if (otmp2 == otmp)
                        break;
                if (!otmp2 || !otmp2->bypass)
                    continue;
                /* found next selected invent item */
                cnt = pick_list[i].count;
                if (cnt < otmp->quan)
                {
                    if (welded(otmp, &youmonst))
                    {
                        ; /* don't split */
                    }
                    else if ((objects[otmp->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && otmp->cursed)
                    {
                        /* same kludge as getobj(), for canletgo()'s use */
                        otmp->corpsenm = (int)cnt; /* don't split */
                    }
                    else
                    {
                        otmp = splitobj(otmp, cnt);
                    }
                }
                n_autobagged += auto_bag_in(invent, otmp, FALSE);
            }
            bypass_objlist(invent, FALSE); /* reset invent to normal */
            free((genericptr_t)pick_list);
        }
    }

autobag_done:
    return n_autobagged;
}


/* on a ladder, used in goto_level */
STATIC_VAR NEARDATA boolean at_ladder = FALSE;

/* the '>' command */
int
dodown()
{
    struct trap *trap = 0;
    boolean stairs_down = ((u.ux == xdnstair && u.uy == ydnstair)
                           || (u.ux == sstairs.sx && u.uy == sstairs.sy
                               && !sstairs.up)),
            ladder_down = (u.ux == xdnladder && u.uy == ydnladder);

    if (u_rooted())
        return 1;

    if (stucksteed(TRUE)) {
        return 0;
    }
    /* Levitation might be blocked, but player can still use '>' to
       turn off controlled levitation */
    if (Levitation && !Levitation_control) /* With levitiation control you can essentially fly while floating, so you can go up and down the stairs */
    {
        if ((HLevitation & I_SPECIAL) || (ELevitation & W_ARTIFACT_INVOKED)) 
        {
            /* end controlled levitation */
            if (ELevitation & W_ARTIFACT_INVOKED) {
                struct obj *obj;

                for (obj = invent; obj; obj = obj->nobj) {
                    if (obj->oartifact
                        && artifact_has_invprop(obj, LEVITATION)) {
                        if (obj->age < monstermoves)
                            obj->age = monstermoves;
                        obj->age += rnz(100);
                    }
                }
            }
            if (float_down(I_SPECIAL | TIMEOUT, W_ARTIFACT_INVOKED)) {
                return 1; /* came down, so moved */
            } else if (!HLevitation && !ELevitation) {
                Your("latent levitation ceases.");
                return 1; /* did something, effectively moved */
            }
        }
        if (Blocks_Levitation) {
            ; /* weren't actually floating after all */
        } else if (Blind) {
            /* Avoid alerting player to an unknown stair or ladder.
             * Changes the message for a covered, known staircase
             * too; staircase knowledge is not stored anywhere.
             */
            if (stairs_down)
                stairs_down =
                    (generic_glyph_to_cmap(levl[u.ux][u.uy].hero_memory_layers.glyph) == S_dnstair);
            else if (ladder_down)
                ladder_down =
                    (generic_glyph_to_cmap(levl[u.ux][u.uy].hero_memory_layers.glyph) == S_dnladder);
        }
        if (Is_airlevel(&u.uz))
            You("are floating in the %s.", surface(u.ux, u.uy));
        else if (Is_waterlevel(&u.uz))
            You("are floating in %s.",
                is_pool(u.ux, u.uy) ? "the water" : "a bubble of air");
        else
            floating_above(stairs_down ? "stairs" : ladder_down
                                                    ? "ladder"
                                                    : surface(u.ux, u.uy));
        return 0; /* didn't move */
    }

    if (!stairs_down && !ladder_down)
    {
        trap = t_at(u.ux, u.uy);
        if (trap && uteetering_at_seen_pit(trap))
        {
            dotrap(trap, TOOKPLUNGE);
            return 1;
        } 
        else if (!trap || !is_hole(trap->ttyp)
                   || !Can_fall_thru(&u.uz) || !trap->tseen) 
        {
            
            if (flags.autodig && !context.nopick && uwep && is_pick(uwep)) 
            {
                return use_pick_axe2(uwep);
            }
            else if (flags.autodig && !context.nopick && u.twoweap && uarms && is_pick(uarms))
            {
                return use_pick_axe2(uarms);
            }
            else
            {
                play_sfx_sound(SFX_GENERAL_CANNOT);
                You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "go down here.");
                return 0;
            }
        }
    }

    if (u.ustuck)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are %s, and cannot go down.",
            !u.uswallow ? "being held" : is_animal(u.ustuck->data)
                                             ? "swallowed"
                                             : "engulfed");
        return 1;
    }

    if (/*on_level(&valley_level, &u.uz)*/ Is_stronghold(&u.uz) && !u.uevent.gehennom_entered)
    {
        You_ex(ATR_NONE, CLR_MSG_GOD, "are standing at the gate to Gehennom.");
        pline_ex(ATR_NONE, CLR_MSG_GOD, "Unspeakable cruelty and harm lurk down there.");
        if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Gate to Gehennom", "Are you sure you want to enter?") != 'y')
            return 0;
        else
            pline_ex(ATR_NONE, CLR_MSG_GOD, "So be it.");
        u.uevent.gehennom_entered = 1; /* don't ask again */
    }

    if (!next_to_u())
    {
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are held back by your pet!");
        return 0;
    }

    if (trap) 
    {
        const char *down_or_thru = trap->ttyp == HOLE ? "down" : "through";
        const char *actn = Flying ? "fly" : locomotion(youmonst.data, "jump");

        if (youmonst.data->msize >= MZ_HUGE)
        {
            char qbuf[QBUFSZ], titlebuf[BUFSZ];

            //You("don't fit %s easily.", down_or_thru);
            Sprintf(qbuf, "You don't fit %s easily. Try to squeeze %s?", down_or_thru, down_or_thru);
            Sprintf(titlebuf, "Squeeze %s?", down_or_thru);
            if (yn_query_ex(ATR_NONE, NO_COLOR, titlebuf, qbuf) == 'y')
            {
                if (!rn2(3))
                {
                    actn = "manage to squeeze";
                    losehp(adjust_damage(rnd(4), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE),
                           "contusion from a small passage", KILLED_BY);
                }
                else
                {
                    play_sfx_sound(SFX_GENERAL_TRIED_ACTION_BUT_IT_FAILED);
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "were unable to fit %s.", down_or_thru);
                    return 0;
                }
            } 
            else 
            {
                return 0;
            }
        }
        You("%s %s the %s.", actn, down_or_thru,
            trap->ttyp == HOLE ? "hole" : "trap door");
    }

    if (trap && Is_stronghold(&u.uz)) 
    {
        /* next_level works for branch staircase going to Gehennom, so this is really only for the castle trapdoors but would work also for staircases */
        goto_hell(!trap, TRUE);
    } 
    else
    {
        at_ladder = (boolean) (levl[u.ux][u.uy].typ == LADDER);
        play_movement_sound(&youmonst, at_ladder ? CLIMBING_TYPE_LADDER_DOWN : CLIMBING_TYPE_STAIRS_DOWN);
        next_level(!trap);
        at_ladder = FALSE;
    }
    return 1;
}

/* the '<' command */
int
doup()
{
    if (u_rooted())
        return 1;

    /* "up" to get out of a pit... */
    if (u.utrap && u.utraptype == TT_PIT) {
        climb_pit();
        return 1;
    }

    if ((u.ux != xupstair || u.uy != yupstair)
        && (!xupladder || u.ux != xupladder || u.uy != yupladder)
        && (!sstairs.sx || u.ux != sstairs.sx || u.uy != sstairs.sy
            || !sstairs.up)) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "go up here.");
        return 0;
    }
    if (stucksteed(TRUE)) {
        return 0;
    }
    if (u.ustuck) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are %s, and cannot go up.",
            !u.uswallow ? "being held" : is_animal(u.ustuck->data)
                                             ? "swallowed"
                                             : "engulfed");
        return 1;
    }
    if (near_capacity() > SLT_ENCUMBER) {
        /* No levitation check; inv_weight() already allows for it */
        play_sfx_sound(SFX_GENERAL_TOO_MUCH_ENCUMBRANCE);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "load is too heavy to climb the %s.",
             levl[u.ux][u.uy].typ == STAIRS ? "stairs" : "ladder");
        return 1;
    }
    if (ledger_no(&u.uz) == 1) {
        if (iflags.debug_fuzzer)
            return 0;
        if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Exiting Dungeon", "Beware, there will be no return!  Still climb?") != 'y')
            return 0;
    }
    if (!next_to_u()) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "are held back by your pet!");
        return 0;
    }
    at_ladder = (boolean) (levl[u.ux][u.uy].typ == LADDER);
    play_movement_sound(&youmonst, at_ladder ? CLIMBING_TYPE_LADDER_UP : CLIMBING_TYPE_STAIRS_UP);
    prev_level(TRUE);
    at_ladder = FALSE;
    return 1;
}

d_level save_dlevel = { 0, 0 };

/* check that we can write out the current level */
STATIC_OVL int
currentlevel_rewrite()
{
    register int fd;
    char whynot[BUFSZ];

    /* since level change might be a bit slow, flush any buffered screen
     *  output (like "you fall through a trap door") */
    mark_synch();

    fd = create_levelfile(ledger_no(&u.uz), whynot);
    if (fd < 0) {
        /*
         * This is not quite impossible: e.g., we may have
         * exceeded our quota. If that is the case then we
         * cannot leave this level, and cannot save either.
         * Another possibility is that the directory was not
         * writable.
         */
        pline1(whynot);
        return -1;
    }

#ifdef MFLOPPY
    if (!savelev(fd, ledger_no(&u.uz), COUNT_SAVE)) {
        (void) nhclose(fd);
        delete_levelfile(ledger_no(&u.uz));
        pline("GnollHack is out of disk space for making levels!");
        You("can save, quit, or continue playing.");
        return -1;
    }
#endif
    return fd;
}

#ifdef INSURANCE
void
save_currentstate()
{
    int fd;
    check_pointing = TRUE;
    if (flags.ins_chkpt) {
        /* write out just-attained level, with pets and everything */
        fd = currentlevel_rewrite();
        if (fd < 0)
            return;
        debugprint("save_currentstate (fd=%d)", fd);
        bufon(fd);
        savelev(fd, ledger_no(&u.uz), WRITE_SAVE);
        bclose(fd);
    }

    /* write out non-level state */
    savestateinlock();
    check_pointing = FALSE;
}
#endif

/*
STATIC_OVL boolean
badspot(x, y)
register xchar x, y;
{
    return (boolean) ((levl[x][y].typ != ROOM
                       && levl[x][y].typ != AIR
                       && levl[x][y].typ != CORR)
                      || MON_AT(x, y));
}
*/

/* when arriving on a level, if hero and a monster are trying to share same
   spot, move one; extracted from goto_level(); also used by wiz_makemap() */
void
u_collide_m(mtmp)
struct monst *mtmp;
{
    coord cc;

    if (!mtmp || mtmp == u.usteed || mtmp != m_at(u.ux, u.uy)) {
        impossible("level arrival collision: %s?",
                   !mtmp ? "no monster"
                     : (mtmp == u.usteed) ? "steed is on map"
                       : "monster not co-located");
        return;
    }

    /* There's a monster at your target destination; it might be one
       which accompanied you--see mon_arrive(dogmove.c)--or perhaps
       it was already here.  Randomly move you to an adjacent spot
       or else the monster to any nearby location.  Prior to 3.3.0
       the latter was done unconditionally. */
    if (!rn2(2) && enexto(&cc, u.ux, u.uy, youmonst.data)
        && distu(cc.x, cc.y) <= 2)
        u_on_newpos(cc.x, cc.y); /*[maybe give message here?]*/
    else
        mnexto(mtmp);

    if ((mtmp = m_at(u.ux, u.uy)) != 0) {
        /* there was an unconditional impossible("mnexto failed")
           here, but it's not impossible and we're prepared to cope
           with the situation, so only say something when debugging */
        if (wizard)
            pline("(monster in hero's way)");
        if (!rloc(mtmp, TRUE) || (mtmp = m_at(u.ux, u.uy)) != 0)
            /* no room to move it; send it away, to return later */
            m_into_limbo(mtmp, FALSE);
    }
}

void
goto_level(newlevel, at_location, falling, inside_tower, portal)
d_level *newlevel;
uchar at_location; /* 1 = at stairs, 2 = at altar */
boolean falling, inside_tower;
xchar portal; /* 1 = Magic portal, 2 = Modron portal down (find portal up), 3 = Modron portal up (find portal down), 4 = Modron portal (random destination), 5 = Magic portal to special stairs down, 6 = Magic portal to special stairs up */
{
    int fd, l_idx;
    xchar new_ledger;
    boolean cant_go_back, great_effort,
            up = (depth(newlevel) < depth(&u.uz)),
            newdungeon = (u.uz.dnum != newlevel->dnum),
            was_in_W_tower = In_W_tower(u.ux, u.uy, &u.uz) || inside_tower,
            familiar = FALSE,
            isnew = FALSE; /* made a new level? */
    struct monst *mtmp;
    char whynot[BUFSZ];
    char *annotation;
    boolean play_arrival_teleport_effect = !!(u.utotype & UTOFLAGS_TELEPORT_EFFECT);
    d_level fromlevel = u.uz;

    debugprint("goto_level: dnum=%d, dlevel=%d, portal=%d", (int)newlevel->dnum, (int)newlevel->dlevel, (int)portal);

    if(at_location & 2)
        context.reviving = TRUE;

    if (dunlev(newlevel) > dunlevs_in_dungeon(newlevel))
        newlevel->dlevel = dunlevs_in_dungeon(newlevel);
    if (newdungeon && In_endgame(newlevel)) 
    { /* 1st Endgame Level !!! */
        if (!u.uhave.amulet)
            return;  /* must have the Amulet */
        if (!wizard) /* wizard ^V can bypass Earth level */
            assign_level(newlevel, &earth_level); /* (redundant) */
    }

    new_ledger = ledger_no(newlevel);
    if (new_ledger <= 0)
        done(ESCAPED); /* in fact < 0 is impossible */

#if 0
    /* If you have the amulet and are trying to get out of Gehennom,
     * going up a set of stairs sometimes does some very strange things!
     * Biased against law and towards chaos.  (The chance to be sent
     * down multiple levels when attempting to go up are significantly
     * less than the corresponding comment in older versions indicated
     * due to overlooking the effect of the call to assign_rnd_lvl().)
     *
     * Odds for making it to the next level up, or of being sent down:
     *  "up"    L      N      C
     *   +1   75.0   75.0   75.0
     *    0    6.25   8.33  12.5
     *   -1   11.46  12.50  12.5
     *   -2    5.21   4.17   0.0
     *   -3    2.08   0.0    0.0
     */
    if (Inhell && up && u.uhave.amulet && !newdungeon && !portal
        && (dunlev(&u.uz) < dunlevs_in_dungeon(&u.uz) - 3)) {
        if (!rn2(4)) {
            int odds = 3 + (int) u.ualign.type,   /* 2..4 */
                diff = odds <= 1 ? 0 : rn2(odds); /* paranoia */

            if (diff != 0) {
                assign_rnd_level(newlevel, &u.uz, diff);
                /* if inside the tower, stay inside */
                if (was_in_W_tower && !On_W_tower_level(newlevel))
                    diff = 0;
            }
            if (diff == 0)
                assign_level(newlevel, &u.uz);

            new_ledger = ledger_no(newlevel);

            pline("A mysterious force momentarily surrounds you...");
            if (on_level(newlevel, &u.uz)) {
                (void) safe_teleds(FALSE, FALSE);
                (void) next_to_u();
                return;
            } else
                at_stairs = at_ladder = FALSE;
        }
    }
#endif

    /* Prevent the player from going past the first quest level unless
     * (s)he has been given the go-ahead by the leader.
     */
    if (on_level(&u.uz, &qstart_level) && !newdungeon && !ok_to_quest() && !wizard)
    {
        play_sfx_sound(SFX_MYSTERIOUS_FORCE_PREVENTS);
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "A mysterious force prevents you from descending.");
        if(play_arrival_teleport_effect)
            level_teleport_effect_in(u.ux, u.uy);
        return;
    }

    if (on_level(newlevel, &u.uz))
    {
        if (play_arrival_teleport_effect)
            level_teleport_effect_in(u.ux, u.uy);
        return; /* this can happen */
    }

    /* tethered movement makes level change while trapped feasible */
    if (u.utrap && u.utraptype == TT_BURIEDBALL)
        buried_ball_to_punishment(); /* (before we save/leave old level) */

    /* Player leaves all regions */
    (void)in_out_region(-1, -1);

    fd = currentlevel_rewrite();
    if (fd < 0)
        return;

    /* Fade to black */
    issue_simple_gui_command(GUI_CMD_FADE_TO_BLACK);

    /* discard context which applies to the level we're leaving;
       for lock-picking, container may be carried, in which case we
       keep context; if on the floor, it's about to be saved+freed and
       maybe_reset_pick() needs to do its carried() check before that */
    maybe_reset_pick((struct obj *) 0);
    reset_trapset(); /* even if to-be-armed trap obj is accompanying hero */
    iflags.travelcc.x = iflags.travelcc.y = 0; /* travel destination cache */
    context.polearm.hitmon = (struct monst *) 0; /* polearm target */
    /* digging context is level-aware and can actually be resumed if
       hero returns to the previous level without any intervening dig */

    if (falling) /* assuming this is only trap door or hole */
        impact_drop((struct obj *) 0, u.ux, u.uy, newlevel->dlevel, TRUE);

    check_special_room(TRUE); /* probably was a trap door */
    if (Punished)
        unplacebc();
    reset_utrap(FALSE); /* needed in level_tele */
    fill_pit(u.ux, u.uy);
    u.ustuck = 0; /* idem */
    u.uinwater = 0;
    u.uundetected = 0; /* not hidden, even if means are available */
    move_monsters_to_mydogs(context.reviving, TRUE);
    removealtarsummons();
    if (u.uswallow) /* idem */
        u.uswldtim = u.uswallow = 0;
    recalc_mapseen(); /* recalculate map overview before we leave the level */
    /*
     *  We no longer see anything on the level.  Make sure that this
     *  follows u.uswallow set to null since uswallow overrides all
     *  normal vision.
     */
    vision_recalc(2);

    /*
     * Save the level we're leaving.  If we're entering the endgame,
     * we can get rid of all existing levels because they cannot be
     * reached any more.  We still need to use savelev()'s cleanup
     * for the level being left, to recover dynamic memory in use and
     * to avoid dangling timers and light sources.
     */
    cant_go_back = (newdungeon && In_endgame(newlevel));
    if (!cant_go_back) 
    {
        update_mlstmv(); /* current monsters are becoming inactive */
        debugprint("goto_level (fd=%d)", fd);
        bufon(fd);       /* use buffered output */
    }

    savelev(fd, ledger_no(&u.uz),
            cant_go_back ? FREE_SAVE : (WRITE_SAVE | FREE_SAVE));
    bclose(fd);

    if (cant_go_back)
    {
        /* discard unreachable levels; keep #0 */
        for (l_idx = maxledgerno(); l_idx > 0; --l_idx)
            delete_levelfile(l_idx);
        /* mark #overview data for all dungeon branches as uninteresting */
        for (l_idx = 0; l_idx < n_dgns; ++l_idx)
            remdun_mapseen(l_idx);
    }

    /* Stop all long immediate sounds from the previous level (footsteps going down are still fine) */
    stop_all_long_immediate_sounds();


    /* NEW LEVEL STARTS HERE */
    if (Is_really_rogue_level(newlevel) || Is_really_rogue_level(&u.uz))
        assign_graphics(Is_really_rogue_level(newlevel) ? ROGUESET : PRIMARY);

    check_gold_symbol();
    /* record this level transition as a potential seen branch unless using
     * some non-standard means of transportation (level teleport).
     */

    if ((at_location || falling || portal) && (u.uz.dnum != newlevel->dnum))
        recbranch_mapseen(&u.uz, newlevel);
    assign_level(&u.uz0, &u.uz);
    assign_level(&u.uz, newlevel);
    assign_level(&u.utolev, newlevel);
    u.utotype = 0;

    if (!builds_up(&u.uz))
    { /* usual case */
        if (dunlev(&u.uz) > dunlev_reached(&u.uz))
            dunlev_reached(&u.uz) = dunlev(&u.uz);
    }
    else 
    {
        if (dunlev_reached(&u.uz) == 0
            || dunlev(&u.uz) < dunlev_reached(&u.uz))
            dunlev_reached(&u.uz) = dunlev(&u.uz);
    }
    reset_rndmonst(NON_PM); /* u.uz change affects monster generation */

    /* Clear certain gui texts and effects */
    issue_simple_gui_command(GUI_CMD_CLEAR_CONDITION_TEXTS);
    issue_simple_gui_command(GUI_CMD_CLEAR_FLOATING_TEXTS);
    issue_simple_gui_command(GUI_CMD_CLEAR_GUI_EFFECTS);

    /* It is a good time for the garbage collector to function */
    issue_simple_gui_command(GUI_CMD_COLLECT_GARBAGE);

    /* set default level change destination areas */
    /* the special level code may override these */
    (void) memset((genericptr_t) &updest, 0, sizeof updest);
    (void) memset((genericptr_t) &dndest, 0, sizeof dndest);
    (void) memset((genericptr_t) &noteledest, 0, sizeof noteledest);

    if (!(level_info[new_ledger].flags & LFILE_EXISTS)) 
    {
        /* entering this level for first time; make it now */
        if (level_info[new_ledger].flags & (FORGOTTEN | VISITED))
        {
            impossible("goto_level: returning to discarded level?");
            level_info[new_ledger].flags &= ~(FORGOTTEN | VISITED);
        }
        mklev();
        isnew = TRUE; /* made the level */
    }
    else 
    {
        debugprint("goto_level");
        /* returning to previously visited level; reload it */
        fd = open_levelfile(new_ledger, whynot);
        if (tricked_fileremoved(fd, whynot))
        {
            /* we'll reach here if running in wizard mode */
            error("Cannot continue this game.");
        }
        reseed_random(rn2);
        reseed_random(rn2_on_display_rng);
        minit(); /* ZEROCOMP */
        getlev(fd, hackpid, new_ledger, FALSE);
        (void) nhclose(fd);
        oinit(); /* reassign level dependent obj probabilities */
    }

    reglyph_darkroom();
    /* do this prior to level-change pline messages */
    vision_reset();         /* clear old level's line-of-sight */
    vision_full_recalc = 0; /* don't let that reenable vision yet */
    flush_screen(-1);       /* ensure all map flushes are postponed */
    char wakeupbuf[BUFSZ] = "";

    if (portal == 1 && !In_endgame(&u.uz))
    {
        /* find the portal on the new level */
        register struct trap *ttrap;

        for (ttrap = ftrap; ttrap; ttrap = ttrap->ntrap)
            if (ttrap->ttyp == MAGIC_PORTAL && ttrap->dst.dlevel == fromlevel.dlevel && ttrap->dst.dnum == fromlevel.dnum)
                break;

        if (!ttrap)
        {
            impossible("goto_level: no corresponding portal, portal type is 1. from: %d/%d, to: %d/%d.", fromlevel.dnum, fromlevel.dlevel, u.uz.dnum, u.uz.dlevel);
            u_on_rndspot((up ? 1 : 0) | (was_in_W_tower ? 2 : 0));
        }
        else
        {
            seetrap(ttrap);
            u_on_newpos(ttrap->tx, ttrap->ty);
        }
    } 
    else if ((portal == MAGIC_PORTAL_TARGET_SSTAIRS_DOWN || portal == MAGIC_PORTAL_TARGET_SSTAIRS_UP) && !In_endgame(&u.uz))
    {
        /* find the special stairs on the level */
        u_on_sstairs(portal == MAGIC_PORTAL_TARGET_SSTAIRS_DOWN);
    }
    else if ((portal == MAGIC_PORTAL_TARGET_STAIRS_DOWN) && !In_endgame(&u.uz))
    {
        /* find the stairs on the level */
        u_on_dnstairs();
    }
    else if ((portal == MAGIC_PORTAL_TARGET_STAIRS_UP) && !In_endgame(&u.uz))
    {
        /* find the stairs on the level */
        u_on_upstairs();
    }
    else if ((portal == MAGIC_PORTAL_TARGET_LADDER_DOWN) && !In_endgame(&u.uz))
    {
        u_on_newpos(xdnladder, ydnladder);
    }
    else if ((portal == MAGIC_PORTAL_TARGET_LADDER_UP) && !In_endgame(&u.uz))
    {
        u_on_newpos(xupladder, yupladder);
    }
    else if ((portal == 2 || portal == 3) && !In_endgame(&u.uz))
    {
        /* find the portal on the new level */
        register struct trap* ttrap;

        for (ttrap = ftrap; ttrap; ttrap = ttrap->ntrap)
            if ((portal == 2 && ttrap->ttyp == MODRON_PORTAL && (ttrap->tflags & TRAPFLAGS_LEVEL_TELEPORT_UP))
                || (portal == 3 && ttrap->ttyp == MODRON_PORTAL && (ttrap->tflags & TRAPFLAGS_LEVEL_TELEPORT_DOWN))
                )
                break;

        if (!ttrap)
        {
            impossible("goto_level: no corresponding portal, portal type=%d. from: %d/%d, to: %d/%d.", (int)portal, fromlevel.dnum, fromlevel.dlevel, u.uz.dnum, u.uz.dlevel);
            u_on_rndspot((up ? 1 : 0) | (was_in_W_tower ? 2 : 0));
            return;
        }
        /* Activate the other end */
        ttrap->tflags |= TRAPFLAGS_ACTIVATED;
        seetrap(ttrap);
        u_on_newpos(ttrap->tx, ttrap->ty);
    }
    else if ((at_location & 1) && !In_endgame(&u.uz))
    {
        if (up) 
        {
            if (at_ladder)
                u_on_newpos(xdnladder, ydnladder);
            else if (newdungeon)
                u_on_sstairs(1);
            else
                u_on_dnstairs();
            /* you climb up the {stairs|ladder};
               fly up the stairs; fly up along the ladder */
            great_effort = (Punished && !Levitation);
            if (flags.verbose || great_effort)
                pline("%s %s up%s the %s.",
                      great_effort ? "With great effort, you" : "You",
                      Levitation ? "float" : Flying ? "fly" : "climb",
                      (Flying && at_ladder) ? " along" : "",
                      at_ladder ? "ladder" : "stairs");
        }
        else
        { /* down */
            if (at_ladder)
                u_on_newpos(xupladder, yupladder);
            else if (newdungeon)
                u_on_sstairs(0);
            else
                u_on_upstairs();

            if (!u.dz) 
            {
                ; /* stayed on same level? (no transit effects) */
            } 
            else if (Flying) 
            {
                if (flags.verbose)
                    You("fly down %s.",
                        at_ladder ? "along the ladder" : "the stairs");
            } 
            else if (near_capacity() > UNENCUMBERED
                       || Punished || Fumbling) 
            {
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, "fall down the %s.", at_ladder ? "ladder" : "stairs");
                if (Punished) {
                    drag_down();
                    ballrelease(FALSE);
                }
                /* falling off steed has its own losehp() call */
                if (u.usteed)
                    dismount_steed(DISMOUNT_FELL);
                else
                    losehp(adjust_damage(rnd(3), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE),
                           at_ladder ? "falling off a ladder"
                                     : "tumbling down a flight of stairs",
                           KILLED_BY);
                selftouch("Falling, you");
            } 
            else 
            { /* ordinary descent */
                if (flags.verbose)
                    You("%s.", at_ladder ? "climb down the ladder"
                                         : "descend the stairs");
            }
        }
    }
    else if ((at_location & 2) && !In_endgame(&u.uz))
    {
        revival_at_altar(wakeupbuf);
    }
    else 
    { /* trap door or level_tele or In_endgame */
        if (context.reviving)
            up = TRUE;
        u_on_rndspot((up ? 1 : 0) | (was_in_W_tower ? 2 : 0));
        if (falling) 
        {
            if (Punished)
                ballfall();
            selftouch("Falling, you");
        }
    }

    if (Punished)
        placebc();
    obj_delivery(FALSE);
    arrival_from_mydogs_and_migrating_mons();
    kill_genocided_monsters(); /* for those wiped out while in limbo */
    /*
     * Expire all timers that have gone off while away.  Must be
     * after migrating monsters and objects are delivered
     * (arrival_from_mydogs_and_migrating_mons and obj_delivery).
     */
    run_timers();

    /* hero might be arriving at a spot containing a monster;
       if so, move one or the other to another location */
    if ((mtmp = m_at(u.ux, u.uy)) != 0)
        u_collide_m(mtmp);

    initrack();

    /* initial movement of bubbles just before vision_recalc */
    if (Is_waterlevel(&u.uz) || Is_airlevel(&u.uz))
        movebubbles();
    else if (Is_firelevel(&u.uz))
        fumaroles();

    if (level_info[new_ledger].flags & FORGOTTEN) 
    {
        forget_map(ALL_MAP); /* forget the map */
        forget_traps();      /* forget all traps too */
        forget_engravings();
        familiar = TRUE;
        level_info[new_ledger].flags &= ~FORGOTTEN;
    }

    /* Reset the screen. */
    vision_reset(); /* reset the blockages */
    docrt();        /* does a full vision recalc */
    flush_screen(-1);

    /* Hearing */
    update_hearing_array(0);

    /*
     *  Move all plines beyond the screen reset.
     */

    /* Update music and ambients */
    update_game_music();
    play_level_ambient_sounds();
    play_environment_ambient_sounds();

     /* Fade back from black */
    issue_simple_gui_command(GUI_CMD_FADE_FROM_BLACK);

    if (play_arrival_teleport_effect)
    {
        level_teleport_effect_in(u.ux, u.uy);
    }

    /* Set naming if known on arrival */
    if (level.flags.special_naming_reveal_type == SPECIAL_LEVEL_NAMING_REVEALED_ON_ARRIVAL 
        || In_sokoban(&u.uz) || In_endgame(&u.uz) || In_V_tower(&u.uz) || In_mines(&u.uz))
    {
        set_special_level_seen(&u.uz, TRUE);
    }

    revival_popup_message(wakeupbuf);

    /* special levels can have a custom arrival message */
    deliver_splev_message();

    /* give room entrance message, if any */
    check_special_room(FALSE);

    /* deliver objects traveling with player */
    obj_delivery(TRUE);

    /* Check whether we just entered Gehennom. */
    if (!In_hell(&u.uz0) && Inhell) 
    {
        if (Is_valley(&u.uz))
        {
            You_ex(ATR_NONE, CLR_MSG_WARNING, "arrive at the Valley of the Dead...");
            pline_The_ex1(ATR_NONE, CLR_MSG_WARNING, "odor of burnt flesh and decay pervades the air.");
#ifdef MICRO
            display_nhwindow(WIN_MESSAGE, FALSE);
#endif
            You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "groans and moans everywhere.");
        } 
        else
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "It is hot here.  You smell smoke...");

        if (flags.showscore && !u.uachieve.enter_gehennom)
            context.botl = 1;

        if (!u.uachieve.enter_gehennom)
        {
            achievement_gained("Entered Gehennom");
            livelog_printf(LL_ACHIEVE, "%s", "entered Gehennom");
        }
        u.uachieve.enter_gehennom = 1;
    }

    if (In_mines(&u.uz) && !u.uachieve.entered_gnomish_mines)
    {
        //if (!u.uachieve.entered_gnomish_mines)
        //    achievement_gained("Entered Gnomish Mines");
        livelog_printf(LL_ACHIEVE, "%s", "entered the Gnomish Mines");
        u.uachieve.entered_gnomish_mines = 1;
    }

    if (In_endgame(&u.uz))
    {
        if (Is_astralevel(&u.uz))
        {
            if (!u.uachieve.entered_astral_plane)
                livelog_printf(LL_ACHIEVE, "%s", "entered the Astral Plane");

            u.uachieve.entered_astral_plane = 1;
        }
        else
        {
            if(!u.uachieve.entered_elemental_planes)
                livelog_printf(LL_ACHIEVE, "%s", "entered the Elemental Planes");
            u.uachieve.entered_elemental_planes = 1;
        }
    }

    if (Is_minetown_level(&u.uz) && !u.uachieve.entered_mine_town)
    {
        //    achievement_gained("Entered Mine Town");
        livelog_printf(LL_MINORAC, "%s", "reached Mine Town");
        u.uachieve.entered_mine_town = 1;
    }

    if (In_sokoban(&u.uz) && !u.uachieve.entered_sokoban)
    {
        //    achievement_gained("Entered Sokoban");
        livelog_printf(LL_ACHIEVE, "%s", "entered Sokoban");
        u.uachieve.entered_sokoban = 1;
    }

    if (Is_bigroom(&u.uz) && !u.uachieve.entered_bigroom)
    {
        //    achievement_gained("Entered the Big Room");
        livelog_printf(LL_ACHIEVE, "%s", "entered the Big Room");
        u.uachieve.entered_bigroom = 1;
    }

    /* in case we've managed to bypass the Valley's stairway down */
    if (Inhell && !Is_valley(&u.uz))
        u.uevent.gehennom_entered = 1;
    
    if (In_modron_level(&u.uz))
    {
        if(!u.uachieve.entered_plane_of_modron)
            livelog_printf(LL_ACHIEVE, "%s", "entered the Plane of the Modron");
        u.uevent.modron_plane_entered = 1;
        u.uachieve.entered_plane_of_modron = 1;
    }
    
    if (In_bovine_level(&u.uz))
    {
        if (!u.uachieve.entered_hellish_pastures)
            livelog_printf(LL_ACHIEVE, "%s", "entered the Hellish Pastures");
        u.uevent.hellish_pastures_entered = 1;
        u.uachieve.entered_hellish_pastures = 1;
    }

    if (In_large_circular_dgn_level(&u.uz))
    {
        if (!u.uachieve.entered_large_circular_dungeon)
            livelog_printf(LL_ACHIEVE, "%s", "entered the Large Circular Dungeon");
        u.uevent.large_circular_dgn_entered = 1;
        u.uachieve.entered_large_circular_dungeon = 1;
    }

    if (isnew)
    {
        char dloc[BUFSZ];
        /* Astral is excluded as a major event here because entry to it
           is already one due to that being an achievement */
        boolean major = In_endgame(&u.uz) && !Is_astralevel(&u.uz);
        if (major)
        {
            (void)endgamelevelname(dloc, depth(&u.uz));
            livelog_printf(LL_ACHIEVE, "entered the %s", dloc);
        }
        else
        {
            livelog_printf(LL_DEBUG, "entered new level %d, %s.", dunlev(&u.uz), dungeons[u.uz.dnum].dname);
        }
    }

    if (familiar) 
    {
        STATIC_VAR const char *const fam_msgs[4] = {
            "You have a sense of deja vu.",
            "You feel like you've been here before.",
            "This place %s familiar...", 0 /* no message */
        };
        STATIC_VAR const char *const halu_fam_msgs[4] = {
            "Whoa!  Everything %s different.",
            "You are surrounded by twisty little passages, all alike.",
            "Gee, this %s like uncle Conan's place...", 0 /* no message */
        };
        const char *mesg;
        char buf[BUFSZ];
        int which = rn2(4);

        if (Hallucination)
            mesg = halu_fam_msgs[which];
        else
            mesg = fam_msgs[which];

        if (mesg && index(mesg, '%'))
        {
            Sprintf(buf, mesg, !Blind ? "looks" : "seems");
            mesg = buf;
        }
        if (mesg)
            pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, mesg);
    }

    /* special location arrival messages/events */
    if (In_endgame(&u.uz)) 
    {
        if (isnew && on_level(&u.uz, &astral_level))
            final_level(); /* guardian angel,&c */
        else if (newdungeon && u.uhave.amulet)
            resurrect(); /* force confrontation with Wizard */

#if FALSE
        if (Is_waterlevel(&u.uz))
        {
            issue_simple_gui_command(GUI_CMD_SAVE_AND_DISABLE_TRAVEL_MODE_ON_LEVEL);
            pline_ex1(ATR_NONE, CLR_MSG_HINT, "Beware, it is dangerous to use travel mode on the Plane of Water!");
            pline_ex1(ATR_NONE, CLR_MSG_HINT, "[Travel mode has been disabled]");
        }
        else if (Is_waterlevel(&u.uz0))
        {
            issue_simple_gui_command(GUI_CMD_RESTORE_TRAVEL_MODE_ON_LEVEL);
            pline_ex1(ATR_NONE, CLR_MSG_HINT, "Phew! It is now safe to use the travel mode again!");
            pline_ex1(ATR_NONE, CLR_MSG_HINT, "[Travel mode has been restored]");
        }
#endif
    } 
    else if (In_quest(&u.uz)) 
    {
        onquest(); /* might be reaching locate|goal level */
    } 
    else if (In_V_tower(&u.uz))
    {
        if (newdungeon && In_hell(&u.uz0))
            pline_The_ex1(ATR_NONE, CLR_MSG_ATTENTION, "heat and smoke are gone.");
    } 
    else if (Is_knox(&u.uz))
    {
        /* alarm stops working once Croesus has died */
        if (isnew || !mvitals[PM_CROESUS].died) 
        {
            You_ex(ATR_NONE, CLR_MSG_WARNING, "have penetrated a high security area!");
            play_sfx_sound(SFX_ALARM_SOUNDS);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "An alarm sounds!");
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
            {
                if (DEADMONSTER(mtmp))
                    continue;
                mtmp->msleeping = 0;
                refresh_m_tile_gui_info(mtmp, TRUE);
            }
        }
    } 
    else 
    {
        if (isnew && Is_really_rogue_level(&u.uz))
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "enter what seems to be an older, more primitive world.");

        /* main dungeon message from your quest leader */
        if (!In_quest(&u.uz0) && at_dgn_entrance("The Quest")
            && !(u.uevent.qcompleted || u.uevent.qexpelled
                 || quest_status.leader_is_dead)) 
        {
            if (!u.uevent.qcalled) 
            {
                u.uevent.qcalled = 1;
                com_pager_ex((struct monst*)0, 2, ATR_NONE, CLR_MSG_ATTENTION, FALSE); /* main "leader needs help" message */
            }
            else 
            {          /* reminder message */
                com_pager_ex((struct monst*)0, Role_if(PM_ROGUE) ? 4 : 3, ATR_NONE, CLR_MSG_ATTENTION, FALSE);
            }
        }

        if (!In_modron_level(&u.uz0) && !u.uevent.modron_portal_hint && at_dgn_entrance("Plane of the Modron"))
        {
            u.uevent.modron_portal_hint = 1;
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly feel that angles are here straighter than normal, but then the feeling subsides.");
        }

        if (!Is_bovine_level(&u.uz0) && !u.uevent.bovine_portal_hint && at_dgn_entrance("Hellish Pastures"))
        {
            u.uevent.bovine_portal_hint = 1;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "For a moment, you think you hear distant grunting and bellowing, but then the noises are gone.");
        }

        if (!Is_quantum_tunnel_level(&u.uz0) && !u.uevent.quantum_portal_hint && at_dgn_entrance("The Large Circular Dungeon"))
        {
            u.uevent.quantum_portal_hint = 1;
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "For a moment, you feel as if the fabric of reality stretches back and forth a bit, but then the sensation passes.");
        }

        if ((flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && level_difficulty() >= MINIMUM_DGN_LEVEL_POLY_TRAP && !u.uevent.polymorph_trap_warning)
        {
            u.uevent.polymorph_trap_warning = 1;
            play_sfx_sound(SFX_WARNING);
            custompline_ex_prefix(ATR_NONE, CLR_MSG_WARNING, "WARNING", ATR_NONE, NO_COLOR, " - ", ATR_NONE, CLR_MSG_HIGHLIGHT, 0U, "Polymorph traps can be present on dungeon level %d and below.", MINIMUM_DGN_LEVEL_POLY_TRAP);
        }

        if (!u.uhint.secret_doors_and_corridors && u.uz.dnum == main_dungeon_dnum &&
            u.uz.dlevel > (context.game_difficulty <= NO_SECRET_DOORS_DIFFICULTY_THRESHOLD ? NO_SECRET_DOORS_DUNGEON_LEVEL_THRESHOLD : 1))
        {
            standard_hint("There may be secret doors and corridors. If you are stuck, try searching for hidden passages.", &u.uhint.secret_doors_and_corridors);
        }
    }

    if (windowprocs.wincap2 & WC2_SCREEN_TEXT)
    {
        /* Screen text for entering the level */
        char dngbuf[BUFSZ];
        char lvlbuf[BUFSZ];
        char lvlbuf2[BUFSZ];
        const char* dname = dungeons[u.uz.dnum].dname;
        if (dname && !strncmp(dname, "The ", 4))
            dname += 4;

        Sprintf(dngbuf, "%s", dname ? dname : "Dungeon");

        s_level* slev = Is_special(&u.uz);
        mapseen* mptr = 0;
        if (slev)
            mptr = find_mapseen(&u.uz);

        boolean special_lvl = FALSE;
        if (slev && mptr && mptr->flags.special_level_true_nature_known)
        {
            Sprintf(lvlbuf, "%s", slev->name);
            special_lvl = TRUE;
        }
        else
        {
            Sprintf(lvlbuf, "Level %d", u.uz.dlevel);
        }
        int curdepth = (int)depth(&u.uz);
        if(!In_endgame(&u.uz) && (curdepth != (int)u.uz.dlevel || special_lvl))
            Sprintf(lvlbuf2, "Dungeon Level %d", curdepth);
        else
            Strcpy(lvlbuf2, "");

        display_screen_text(lvlbuf, dngbuf, lvlbuf2, SCREEN_TEXT_ENTER_DUNGEON_LEVEL, 0, 0, 0UL);
    }

    assign_level(&u.uz0, &u.uz); /* reset u.uz0 */
#ifdef INSURANCE
    save_currentstate();
#endif

    if ((annotation = get_annotation(&u.uz)) != 0)
        You("remember this level as %s.", annotation);

    /* assume this will always return TRUE when changing level */
    (void) in_out_region(u.ux, u.uy);
    (void) pickup(1, FALSE);

#ifdef WHEREIS_FILE
    touch_whereis();
#endif

    context.reviving = FALSE;
}


void
revival_at_altar(wakeupbuf)
char* wakeupbuf;
{
    int altar_x = 0, altar_y = 0;

    if (!Is_astralevel(&u.uz))
    {
        int x, y;
        boolean dobreak = FALSE;
        for (x = 1; x < COLNO; x++)
        {
            for (y = 0; y < ROWNO; y++)
            {
                if (IS_ALTAR(levl[x][y].typ))
                {
                    altar_x = x;
                    altar_y = y;
                    if (a_align(x, y) == u.ualign.type)
                    {
                        dobreak = TRUE;
                        break;
                    }
                }
            }
            if (dobreak)
                break;
        }
    }

    set_itimeout(&HInvulnerable, 0L);
    heal_ailments_upon_revival();
    if (wakeupbuf)
    {
        int ux = u.ux, uy = u.uy;
        if (isok(altar_x, altar_y))
        {
            if (MON_AT(altar_x, altar_y))
                mnexto(m_at(altar_x, altar_y));
            u_on_newpos(altar_x, altar_y);
            if (MON_AT(u.ux, u.uy))
                mnexto(m_at(u.ux, u.uy));
            Sprintf(wakeupbuf, "After being dead for a while, you suddenly feel the saving grace of %s, and wake up at %s altar.", u_gname(), u_ghisher());
        }
        else
        {
            u_on_rndspot(FALSE);
            if (MON_AT(u.ux, u.uy))
                mnexto(m_at(u.ux, u.uy));
            Sprintf(wakeupbuf, "After being dead for a while, you suddenly feel the saving grace of %s, and wake up.", u_gname());
        }
        newsym(ux, uy);
    }
}

void
revival_popup_message(wakeupbuf)
char* wakeupbuf;
{
    if (wakeupbuf && *wakeupbuf)
    {
        play_sfx_sound(SFX_REVIVAL);
        pline_ex1(ATR_NONE, CLR_MSG_SUCCESS, wakeupbuf);
        display_popup_text(wakeupbuf, "Revival", POPUP_TEXT_REVIVAL, ATR_NONE, CLR_MSG_SUCCESS, NO_GLYPH, POPUP_FLAGS_NONE);
    }
}

STATIC_OVL void
final_level()
{
    struct monst *mtmp;

    /* reset monster hostility relative to player */
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
    {
        if (DEADMONSTER(mtmp))
            continue;
        reset_hostility(mtmp);
    }

    /* create some player-monsters */
    create_mplayers(rn1(4, 3), TRUE);

    /* create a guardian angel next to player, if worthy */
    gain_guardian_angel(FALSE);
}

STATIC_VAR char *dfr_pre_msg = 0,  /* pline() before level change */
            *dfr_post_msg = 0; /* pline() after level change */

/* change levels at the end of this turn, after monsters finish moving */
void
schedule_goto(tolev, at_location, falling, teleport, inside_tower, portal_flag, pre_msg, post_msg)
d_level *tolev;
uchar at_location; /* 1 = at stairs, 2 = at altar */
boolean falling, teleport, inside_tower;
int64_t portal_flag;
const char *pre_msg, *post_msg;
{
    short typmask = UTOFLAGS_DEFERRED_GOTO; /* non-zero triggers `deferred_goto' */
    debugprint("schedule_goto: dnum=%d, dlevel=%d, portal_flag=%lld", tolev ? tolev->dnum : -1, tolev ? tolev->dlevel : -1, (long long)portal_flag);

    /* destination flags (`goto_level' args) */
    if (at_location == 1)
        typmask |= UTOFLAGS_AT_STAIRS;
    else if (at_location == 2)
        typmask |= UTOFLAGS_AT_ALTAR;

    if (falling)
        typmask |= UTOFLAGS_FALLING;

    //if (portal_flag == 1)
    //    typmask |= UTOFLAGS_PORTAL_1;
    //else if (portal_flag == 2)
    //    typmask |= UTOFLAGS_PORTAL_2;
    //else if (portal_flag == 3)
    //    typmask |= UTOFLAGS_PORTAL_3;
    //else if (portal_flag == 4)
    //    typmask |= UTOFLAGS_PORTAL_4;

    if (portal_flag > 0 && portal_flag < 16)
        typmask |= (short)(portal_flag << UTO_PORTAL_BIT_OFFSET);
    else if (portal_flag < 0)
    {
        typmask |= UTOFLAGS_PORTAL_1; /* The same otherwise as 1 */
        typmask |= UTOFLAGS_REMOVE_PORTAL; /* flag for portal removal */
    }

    if (teleport)
        typmask |= UTOFLAGS_TELEPORT_EFFECT; /* flag for teleport in effect on new level */

    if (inside_tower)
        typmask |= UTOFLAGS_INSIDE_TOWER; /* flag for going inside rather than outside Wizard's tower */

    u.utotype = typmask;
    /* destination level */
    assign_level(&u.utolev, tolev);

    if (pre_msg)
        dfr_pre_msg = dupstr(pre_msg);
    if (post_msg)
        dfr_post_msg = dupstr(post_msg);
}

/* handle something like portal ejection */
void
deferred_goto()
{
    if (!on_level(&u.uz, &u.utolev))
    {
        d_level dest;
        short typmask = u.utotype; /* save it; goto_level zeroes u.utotype */
        debugprint("deferred_goto: dnum=%d, dlevel=%d, typmask=%d", (int)u.utolev.dnum, (int)u.utolev.dlevel, typmask);

        assign_level(&dest, &u.utolev);
        if (dfr_pre_msg)
            pline1(dfr_pre_msg);
        xchar portal_flag = (xchar)((typmask & UTOFLAGS_PORTAL_MASK) >> UTO_PORTAL_BIT_OFFSET); //  (typmask & UTOFLAGS_PORTAL_1) ? 1 : (typmask & UTOFLAGS_PORTAL_2) ? 2 : (typmask & UTOFLAGS_PORTAL_3) ? 3 : (typmask & UTOFLAGS_PORTAL_4) ? 4 : 0;
        uchar at_location = (!!(typmask & UTOFLAGS_AT_STAIRS)) | (2 * (!!(typmask & UTOFLAGS_AT_ALTAR)));
        boolean falling = !!(typmask & UTOFLAGS_FALLING);
        boolean inside_tower = !!(typmask & UTOFLAGS_INSIDE_TOWER);
        goto_level(&dest, at_location, falling, inside_tower, portal_flag);

        /* Remove portal */
        if (typmask & UTOFLAGS_REMOVE_PORTAL) 
        {
            struct trap *t = t_at(u.ux, u.uy);
            if (t)
            {
                deltrap(t);
                newsym(u.ux, u.uy);
            }
        }

        if (dfr_post_msg)
            pline1(dfr_post_msg);
    }

    u.utotype = 0; /* our caller keys off of this */
    if (dfr_pre_msg)
        free((genericptr_t) dfr_pre_msg), dfr_pre_msg = 0;
    if (dfr_post_msg)
        free((genericptr_t) dfr_post_msg), dfr_post_msg = 0;
}

STATIC_OVL void
revive_handle_magic_chest(where_ptr, container_ptr, container_where_ptr, mcarry_ptr)
xchar* where_ptr;
struct obj** container_ptr;
int* container_where_ptr;
struct monst** mcarry_ptr;
{
    if (!where_ptr || !container_where_ptr || !container_ptr || !mcarry_ptr)
        return;

    /* Magic chest handling for revival */
    if (*where_ptr == OBJ_MAGIC)
    {
        boolean found = FALSE;
        struct obj* otmp;
        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (Is_magic_chest(otmp))
            {
                *container_ptr = otmp;
                *container_where_ptr = OBJ_INVENT;
                found = TRUE;
                break;
            }

        if (!found)
        {
            for (otmp = fobj; otmp; otmp = otmp->nobj)
                if (Is_magic_chest(otmp))
                {
                    *container_ptr = otmp;
                    *container_where_ptr = OBJ_FLOOR;
                    found = TRUE;
                    break;
                }
        }

        if (!found)
        {
            struct monst* mtmp;
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
            {
                for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
                    if (Is_magic_chest(otmp))
                    {
                        *container_ptr = otmp;
                        *container_where_ptr = OBJ_MINVENT;
                        *mcarry_ptr = mtmp;
                        found = TRUE;
                        break;
                    }

                if (found)
                    break;
            }
        }

        if (found)
        {
            *where_ptr = OBJ_CONTAINED;
        }
    }
}

/*
 * Return TRUE if we created a monster for the corpse.  If successful, the
 * corpse is gone.
 */
boolean
revive_corpse(corpse)
struct obj *corpse;
{
    struct monst *mtmp, *mcarry;
    boolean is_uwep, chewed;
    xchar where;
    char cname[BUFSZ];
    struct obj *container = (struct obj *) 0;
    int container_where = 0;

    where = corpse->where;
    is_uwep = (corpse == uwep);
    chewed = (corpse->oeaten != 0);
    Strcpy(cname, corpse_xname(corpse,
                               chewed ? "bite-covered" : (const char *) 0,
                               CXN_SINGULAR));
    mcarry = (where == OBJ_MINVENT) ? corpse->ocarry : 0;

    if (where == OBJ_CONTAINED) 
    {
        struct monst *mtmp2;

        container = corpse->ocontainer;
        mtmp2 = get_container_location(container, &container_where, (int *) 0);
        /* container_where is the outermost container's location even if
         * nested */
        if (container_where == OBJ_MINVENT && mtmp2)
            mcarry = mtmp2;
    }
    mtmp = revive(corpse, FALSE, -1, FALSE); /* corpse is gone if successful */

    if (mtmp) 
    {
        revive_handle_magic_chest(&where, &container, &container_where, &mcarry);
        int clr = is_tame(mtmp) || is_peaceful(mtmp) ? CLR_MSG_ATTENTION : CLR_MSG_WARNING;
        switch (where) 
        {
        case OBJ_INVENT:
            if (is_uwep)
                pline_The_ex(ATR_NONE, clr, "%s writhes out of your grasp!", cname);
            else
                You_feel_ex(ATR_NONE, clr, "squirming in your backpack!");

            reviver_hint(mtmp);
            break;

        case OBJ_FLOOR:
            if (cansee(mtmp->mx, mtmp->my))
            {
                pline_ex(ATR_NONE, clr, "%s rises from the dead!",
                    chewed ? Adjmonnam(mtmp, "bite-covered")
                    : Monnam(mtmp));
                reviver_hint(mtmp);
            }
            break;

        case OBJ_MINVENT: /* probably a nymph's */
            if (cansee(mtmp->mx, mtmp->my)) {
                if (canseemon(mcarry))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Startled, %s drops %s as it revives!",
                          mon_nam(mcarry), an(cname));
                else
                    pline_ex(ATR_NONE, clr, "%s suddenly appears!",
                          chewed ? Adjmonnam(mtmp, "bite-covered")
                                 : Monnam(mtmp));
                reviver_hint(mtmp);
            }
            break;
        case OBJ_CONTAINED: {
            char sackname[BUFSZ];

            if (container_where == OBJ_MINVENT && cansee(mtmp->mx, mtmp->my)
                && mcarry && canseemon(mcarry) && container) {
                pline_ex(ATR_NONE, clr, "%s writhes out of %s!", Amonnam(mtmp),
                      yname(container));
                reviver_hint(mtmp);
            } else if (container_where == OBJ_INVENT && container) {
                Strcpy(sackname, an(xname(container)));
                pline_ex(ATR_NONE, clr, "%s %s out of %s in your pack!",
                      Blind ? Something : Amonnam(mtmp),
                      locomotion(mtmp->data, "writhes"), sackname);
                reviver_hint(mtmp);
            } else if (container_where == OBJ_FLOOR && container
                       && cansee(mtmp->mx, mtmp->my)) {
                Strcpy(sackname, an(xname(container)));
                pline_ex(ATR_NONE, clr, "%s escapes from %s!", Amonnam(mtmp), sackname);
                reviver_hint(mtmp);
            }
            break;
        }
        case OBJ_MAGIC:
            /* Should not happen */
            break;
        default:
            /* we should be able to handle the other cases... */
            impossible("revive_corpse: lost corpse @ %d", where);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

/*
 * Return TRUE if we created a monster for the corpse.  If successful, the
 * corpse is gone.
 */
boolean
animate_corpse(corpse, animateintomon)
struct obj* corpse;
int animateintomon; // monstid to be animated into
{
    if (animateintomon < 0 || animateintomon >= NUM_MONSTERS || !corpse || corpse->corpsenm < 0 || corpse->corpsenm >= NUM_MONSTERS)
        return FALSE;

    struct monst* mtmp, * mcarry;
    boolean is_uwep, chewed;
    xchar where;
    char cname[BUFSZ];
    struct obj* container = (struct obj*) 0;
    int container_where = 0;
    int oldcorpsenum = corpse->corpsenm;

    where = corpse->where;
    is_uwep = (corpse == uwep);
    chewed = (corpse->oeaten != 0);
    Strcpy(cname, corpse_xname(corpse,
        chewed ? "bite-covered" : (const char*)0,
        CXN_SINGULAR));
    mcarry = (where == OBJ_MINVENT) ? corpse->ocarry : 0;

    if (where == OBJ_CONTAINED) {
        struct monst* mtmp2;

        container = corpse->ocontainer;
        mtmp2 = get_container_location(container, &container_where, (int*)0);
        /* container_where is the outermost container's location even if
         * nested */
        if (container_where == OBJ_MINVENT && mtmp2)
            mcarry = mtmp2;
    }
    mtmp = revive(corpse, TRUE, animateintomon, FALSE); /* corpse is gone if successful */

    if (mtmp)
    {
        revive_handle_magic_chest(&where, &container, &container_where, &mcarry);
        int clr = is_tame(mtmp) || is_peaceful(mtmp) ? CLR_MSG_ATTENTION : CLR_MSG_WARNING;
        switch (where)
        {
        case OBJ_INVENT:
            if (is_uwep)
                pline_The_ex(ATR_NONE, clr, "%s writhes out of your grasp!", cname);
            else
                You_feel_ex(ATR_NONE, clr, "squirming in your backpack!");
            break;

        case OBJ_FLOOR:
            if (cansee(mtmp->mx, mtmp->my))
                pline_ex(ATR_NONE, clr, "%s rises from the dead as %s!",
                    The(pm_monster_name(&mons[oldcorpsenum], mtmp->female)), an(pm_monster_name(&mons[animateintomon], mtmp->female)));
            break;

        case OBJ_MINVENT: /* probably a nymph's */
            if (cansee(mtmp->mx, mtmp->my)) 
            {
                if (canseemon(mcarry))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Startled, %s drops %s as it gets animated!",
                        mon_nam(mcarry), an(cname));
                else
                    pline_ex(ATR_NONE, clr, "%s rises from the dead as %s!",
                        The(pm_monster_name(&mons[oldcorpsenum], mtmp->female)), an(pm_monster_name(&mons[animateintomon], mtmp->female)));
            }
            break;
        case OBJ_CONTAINED: 
        {
            char sackname[BUFSZ];

            if (container_where == OBJ_MINVENT && cansee(mtmp->mx, mtmp->my)
                && mcarry && canseemon(mcarry) && container)
            {
                pline_ex(ATR_NONE, clr, "%s writhes out of %s!", Amonnam(mtmp),
                    yname(container));
            }
            else if (container_where == OBJ_INVENT && container)
            {
                Strcpy(sackname, an(xname(container)));
                pline_ex(ATR_NONE, clr, "%s %s out of %s in your pack!",
                    Blind ? Something : Amonnam(mtmp),
                    locomotion(mtmp->data, "writhes"), sackname);
            }
            else if (container_where == OBJ_FLOOR && container
                && cansee(mtmp->mx, mtmp->my)) {
                Strcpy(sackname, an(xname(container)));
                pline_ex(ATR_NONE, clr, "%s escapes from %s!", Amonnam(mtmp), sackname);
            }
            break;
        }
        default:
            /* we should be able to handle the other cases... */
            impossible("animate_corpse: lost corpse @ %d", where);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

/* Revive the corpse via a timeout. */
/*ARGSUSED*/
void
revive_mon(arg, timeout)
anything *arg;
int64_t timeout UNUSED;
{
    struct obj *body = arg->a_obj;
    struct permonst *mptr = body->corpsenm >= LOW_PM ? &mons[body->corpsenm] : 0;
    struct monst *mtmp;
    xchar x, y;

    if (!mptr)
        return;

    int body_where = body->where;

    /* corpse will revive somewhere else if there is a monster in the way;
       Riders get a chance to try to bump the obstacle out of their way */
    if ((mptr->mflags3 & M3_DISPLACES) != 0 && body_where == OBJ_FLOOR
        && get_obj_location(body, &x, &y, 0) && (mtmp = m_at(x, y)) != 0) {
        boolean notice_it = canseemon(mtmp); /* before rloc() */
        char *monname = Monnam(mtmp);

        if (rloc2(mtmp, TRUE, TRUE)) {
            if (notice_it && !canseemon(mtmp))
                pline("%s vanishes.", monname);
            else if (!notice_it && canseemon(mtmp))
                pline("%s appears.", Monnam(mtmp)); /* not pre-rloc monname */
            else if (notice_it && dist2(mtmp->mx, mtmp->my, x, y) > 2)
                pline("%s teleports.", monname); /* saw it and still see it */
        }
    }

    /* if we succeed, the corpse is gone */
    if (!revive_corpse(body)) 
    {
        int64_t when;
        int action;

        if (is_rider_or_tarrasque(mptr) && (body_where == OBJ_MAGIC || rn2(99)))  /* Rider usually tries again; and always if in magic chest */
        {
            action = REVIVE_MON;
            if (body_where == OBJ_MAGIC) /* While in a magic chest that cannot be located, try to revive every 9 turns in the case player changed levels, recovered the magic chest, or the like */
                when = 9L;
            else
            {
                for (when = 3L; when < 67L; when++)
                    if (!rn2(3))
                        break;
            }
        } 
        else 
        { /* rot this corpse away */
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "%sless hassled.", is_rider_or_tarrasque(mptr) ? "much " : "");
            action = ROT_CORPSE;
            when = 250L - (monstermoves - body->age);
            if (when < 1L)
                when = 1L;
        }
        (void) start_timer(when, TIMER_OBJECT, action, arg);
    }
}

int
donull()
{
    if (context.first_time_cmd || !occupation)
    {
        display_gui_effect(GUI_EFFECT_WAIT, 0, u.ux, u.uy, 0, 0, 0UL);
    }

    return 1; /* Do nothing, but let other things happen */
}

STATIC_PTR int
wipeoff(VOID_ARGS)
{
    if (u.ucreamed < 4)
        u.ucreamed = 0;
    else
        u.ucreamed -= 4;
    
    if (Blinded < 4)
        Blinded = 0;
    else
        Blinded -= 4;

    if (!Blinded) {
        pline("You've got the glop off.");
        u.ucreamed = 0;
        if (!gulp_blnd_check()) {
            Blinded = 1;
            make_blinded(0L, TRUE);
        }
        return 0;
    } else if (!u.ucreamed) {
        Your("%s feels clean now.", body_part(FACE));
        return 0;
    }
    return 1; /* still busy */
}

int
dowipe()
{
    if (u.ucreamed)
    {
        boolean isfemale = Upolyd ? youmonst.female : flags.female;
        enum monster_soundset_types mss = isfemale ? youmonst.data->female_soundset : youmonst.data->soundset;
        enum object_soundset_types oss = monster_soundsets[mss].attack_soundsets[BAREHANDED_ATTACK_NUMBER];
        enum object_soundset_types used_oss = uarmg ? objects[uarmg->otyp].oc_soundset : oss;

        static NEARDATA char buf[39];
        Sprintf(buf, "wiping off your %s", body_part(FACE));
        set_occupation(wipeoff, buf, ATR_NONE, CLR_MSG_ATTENTION, used_oss, OCCUPATION_WIPING_OFF, OCCUPATION_SOUND_TYPE_START, 0);
        /* Not totally correct; what if they change back after now
         * but before they're finished wiping?
         */
        return 1;
    }
    Your("%s is already clean.", body_part(FACE));
    return 1;
}

void
set_wounded_legs(side, timex)
register int64_t side;
register int timex;
{
    /* KMH -- STEED
     * If you are riding, your steed gets the wounded legs instead.
     * You still call this function, but don't lose hp.
     * Caller is also responsible for adjusting messages.
     */

    /*
    if (!Wounded_legs) {
        ATEMP(A_DEX)--;
        context.botl = 1;
    }
    */

    if (!Wounded_legs || (HWounded_legs & TIMEOUT))
        HWounded_legs = timex;
    EWounded_legs = side;
    (void) encumber_msg();
}

void
heal_legs(how)
int how; /* 0: ordinary, 1: dismounting steed, 2: limbs turn to stone */
{
    if (Wounded_legs) {
        
        /* Moved to updateabon */
        /*
        if (ATEMP(A_DEX) < 0) {
            ATEMP(A_DEX)++;
            context.botl = 1;
        }
        */

        /* when mounted, wounded legs applies to the steed;
           during petrification countdown, "your limbs turn to stone"
           before the final stages and that calls us (how==2) to cure
           wounded legs, but we want to suppress the feel better message */
        if (!u.usteed && how != 2) {
            const char *legs = body_part(LEG);

            if ((EWounded_legs & BOTH_SIDES) == BOTH_SIDES)
                legs = makeplural(legs);
            /* this used to say "somewhat better" but that was
               misleading since legs are being fully healed */
            Your("%s %s better.", legs, vtense(legs, "feel"));
        }

        HWounded_legs = EWounded_legs = 0L;

        /* Wounded_legs reduces carrying capacity, so we want
           an encumbrance check when they're healed.  However,
           while dismounting, first steed's legs get healed,
           then hero is dropped to floor and a new encumbrance
           check is made [in dismount_steed()].  So don't give
           encumbrance feedback during the dismount stage
           because it could seem to be shown out of order and
           it might be immediately contradicted [able to carry
           more when steed becomes healthy, then possible floor
           feedback, then able to carry less when back on foot]. */
        if (how == 0)
            (void) encumber_msg();
    }
}

int
dotogglehpbars()
{
    boolean main_flag = flags.show_tile_u_hp_bar;
    
    flags.show_tile_u_hp_bar = !main_flag;
    flags.show_tile_mon_hp_bar = !main_flag;
    flags.show_tile_pet_hp_bar = !main_flag;

    newsym(u.ux, u.uy);  // force_redraw_at(u.ux, u.uy);

    struct monst* mtmp;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
    {
        if (canseemon(mtmp))
            newsym(mtmp->mx, mtmp->my);  // force_redraw_at(mtmp->mx, mtmp->my);
    }
    flush_screen(1);
//    redraw_map();

    return 0;
}

int
dotogglegrid()
{
    flags.show_grid = !flags.show_grid;
    redraw_map();

    return 0;
}

int
dotogglebufftimers()
{
    flags.show_buff_timer = !flags.show_buff_timer;
    newsym(u.ux, u.uy); //force_redraw_at(u.ux, u.uy);
    struct monst* mtmp;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
    {
        if (canseemon(mtmp) && is_tame(mtmp))
            newsym(mtmp->mx, mtmp->my); //force_redraw_at(mtmp->mx, mtmp->my);
    }
    flush_screen(1);

    return 0;
}

int
dotogglemonstertargeting()
{
    boolean current_flag = flags.show_tile_monster_target;

    flags.show_tile_monster_target = !current_flag;

    struct monst* mtmp;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
    {
        //if (canseemon(mtmp))
        newsym(mtmp->mx, mtmp->my);  //force_redraw_at(mtmp->mx, mtmp->my);
    }
    flush_screen(1);

    return 0;
}

int
dotoggleumark()
{
    boolean current_flag = flags.show_tile_u_mark;

    flags.show_tile_u_mark = !current_flag;

    newsym(u.ux, u.uy); //force_redraw_at(u.ux, u.uy);

    flush_screen(1);

    return 0;
}


void
delete_location(x, y)
xchar x, y;
{
    debugprint("delete_location");

    if (levl[x][y].typ == FOUNTAIN)
        level.flags.nfountains--;
    else if (levl[x][y].typ == SINK)
        level.flags.nsinks--;

    if (levl[x][y].lamplit)
    {
        if(!in_mklev)
            del_light_source(LS_LOCATION, xy_to_any(x, y));
        levl[x][y].lamplit = 0;
    }
    if (levl[x][y].makingsound)
    {
        if (!in_mklev)
            del_sound_source(SOUNDSOURCE_LOCATION, xy_to_any(x, y));
        levl[x][y].makingsound = 0;
    }
    levl[x][y].typ = UNDEFINED_LOCATION;
    levl[x][y].flags = 0;
    levl[x][y].subtyp = 0;
    levl[x][y].vartyp = 0;
    levl[x][y].floor_doodad = 0;
    levl[x][y].carpet_typ = 0;
    levl[x][y].carpet_piece = 0;
    levl[x][y].carpet_flags = 0;
    levl[x][y].decoration_typ = 0;
    levl[x][y].decoration_subtyp = 0;
    levl[x][y].decoration_dir = 0;
    levl[x][y].decoration_flags = 0;
    levl[x][y].key_otyp = 0;
    levl[x][y].special_quality = 0;
    levl[x][y].floortyp = 0;
    levl[x][y].floorsubtyp = 0;
    levl[x][y].floorvartyp = 0;
    levl[x][y].facing_right = 0;
    levl[x][y].horizontal = 0;
}

void
delete_decoration(x, y)
xchar x, y;
{
    if (levl[x][y].decoration_typ)
    {
        debugprint("delete_decoration");
        if (levl[x][y].lamplit)
        {
            if (!in_mklev)
                del_light_source(LS_LOCATION, xy_to_any(x, y));
            levl[x][y].lamplit = 0;
        }
        if (levl[x][y].makingsound)
        {
            if (!in_mklev)
                del_sound_source(SOUNDSOURCE_LOCATION, xy_to_any(x, y));
            levl[x][y].makingsound = 0;
        }
        levl[x][y].decoration_typ = 0;
        levl[x][y].decoration_subtyp = 0;
        levl[x][y].decoration_dir = 0;
        levl[x][y].decoration_flags = 0;
    }
}

void
full_location_transform(x, y, type, subtype, vartype, location_flags, carpet_typ, carpet_piece, carpet_flags, decoration_typ, decoration_subtyp, decoration_dir, decoration_flags, floor_doodad, floortype, floorsubtype, floorvartype, facing_right, horizontal, key_otyp, special_quality, donewsym)
xchar x, y;
int type, subtype, vartype, floor_doodad, floortype, floorsubtype, floorvartype;
schar carpet_typ, carpet_piece, decoration_typ, decoration_subtyp, decoration_dir;
uchar carpet_flags, decoration_flags;
unsigned short location_flags;
boolean facing_right, horizontal, donewsym;
short key_otyp, special_quality;
{
    delete_location(x, y);
    levl[x][y].typ = type;
    levl[x][y].subtyp = subtype;
    levl[x][y].vartyp = vartype;
    levl[x][y].flags = location_flags;
    levl[x][y].floor_doodad = floor_doodad;
    levl[x][y].carpet_typ = carpet_typ;
    levl[x][y].carpet_piece = carpet_piece;
    levl[x][y].carpet_flags = carpet_flags;
    levl[x][y].decoration_typ = decoration_typ;
    levl[x][y].decoration_subtyp = decoration_subtyp;
    levl[x][y].decoration_dir = decoration_dir;
    levl[x][y].decoration_flags = decoration_flags;
    levl[x][y].floortyp = floortype;
    levl[x][y].floorsubtyp = floorsubtype;
    levl[x][y].floorvartyp = floorvartype;
    levl[x][y].facing_right = facing_right;
    levl[x][y].horizontal = horizontal;
    levl[x][y].key_otyp = key_otyp;
    levl[x][y].special_quality = special_quality;

    if(!in_mklev)
        maybe_create_location_light_and_sound_sources(x, y);

    if (levl[x][y].typ == FOUNTAIN)
        level.flags.nfountains++;
    else if (levl[x][y].typ == SINK)
        level.flags.nsinks++;

    if(donewsym && !in_mklev)
        newsym(x, y);
}

void
full_initial_location_transform(x, y, type, location_flags, carpet_typ, carpet_piece, carpet_flags, decoration_typ, decoration_subtyp, decoration_dir, decoration_flags, floor_doodad, floortype, facing_right, horizontal, key_otyp, special_quality, donewsym)
xchar x, y;
int type, floor_doodad, floortype;
schar carpet_typ, carpet_piece, decoration_typ, decoration_subtyp, decoration_dir;
uchar carpet_flags, decoration_flags;
unsigned short location_flags;
boolean facing_right, horizontal, donewsym;
short key_otyp, special_quality;
{
    int subtype = get_initial_location_subtype(type);
    int vartype = get_initial_location_vartype(type, subtype);
    if (IS_FLOOR(type))
        floortype = 0;
    int floorsubtype = floortype == 0 ? 0 : get_initial_location_subtype(floortype);
    int floorvartype = floortype == 0 ? 0 : get_initial_location_vartype(floortype, floorsubtype);
    full_location_transform(x, y, type, subtype, vartype, location_flags, carpet_typ, carpet_piece, carpet_flags, decoration_typ, decoration_subtyp, decoration_dir, decoration_flags, floor_doodad, floortype, floorsubtype, floorvartype, facing_right, horizontal, key_otyp, special_quality, donewsym);
    initialize_location(&levl[x][y]);
}

void
create_simple_location(x, y, type, subtype, vartype, location_flags, floor_doodad, floortype, floorsubtype, floorvartype, donewsym)
xchar x, y;
int type, subtype, vartype, floor_doodad, floortype, floorsubtype, floorvartype;
unsigned short location_flags;
boolean donewsym;
{
    full_location_transform(x, y, type, subtype, vartype, location_flags, 0, 0, 0, 0, 0, 0, 0, floor_doodad, floortype, floorsubtype, floorvartype, FALSE, FALSE, 0, 0, donewsym);
    initialize_location(&levl[x][y]);
}

void
create_simple_location_with_carpet(x, y, type, subtype, vartype, location_flags, carpet_typ, carpet_piece, carpet_flags, floor_doodad, floortype, floorsubtype, floorvartype, donewsym)
xchar x, y;
int type, subtype, vartype, floor_doodad, floortype, floorsubtype, floorvartype;
unsigned short location_flags;
schar carpet_typ, carpet_piece;
uchar carpet_flags;
boolean donewsym;
{
    full_location_transform(x, y, type, subtype, vartype, location_flags, carpet_typ, carpet_piece, carpet_flags, 0, 0, 0, 0, floor_doodad, floortype, floorsubtype, floorvartype, FALSE, FALSE, 0, 0, donewsym);
    initialize_location(&levl[x][y]);
}

void
create_simple_initial_location(x, y, type, location_flags, floor_doodad, floortype, donewsym)
xchar x, y;
int type, floor_doodad, floortype;
unsigned short location_flags;
boolean donewsym;
{
    full_initial_location_transform(x, y, type, location_flags, 0, 0, 0, 0, 0, 0, 0, floor_doodad, floortype, FALSE, FALSE, 0, 0, donewsym);
    initialize_location(&levl[x][y]);
}

void
create_location_with_current_floor(x, y, type, subtype, vartype, location_flags, floor_doodad, donewsym)
xchar x, y;
int type, subtype, vartype, floor_doodad;
unsigned short location_flags;
boolean donewsym;
{
    boolean isfloor = IS_FLOOR(levl[x][y].typ);
    create_simple_location_with_carpet(x, y, type, subtype, vartype, location_flags, levl[x][y].carpet_typ, levl[x][y].carpet_piece, levl[x][y].carpet_flags, floor_doodad, isfloor ? levl[x][y].typ : levl[x][y].floortyp, isfloor ? levl[x][y].subtyp : levl[x][y].floorsubtyp, isfloor ? levl[x][y].vartyp : levl[x][y].floorvartyp, donewsym);
}

void
create_initial_location_with_current_floor(x, y, type, location_flags, floor_doodad, donewsym)
xchar x, y;
int type, floor_doodad;
unsigned short location_flags;
boolean donewsym;
{
    int subtype = get_initial_location_subtype(type);
    int vartype = get_initial_location_vartype(type, subtype);
    create_location_with_current_floor(x, y, type, subtype, vartype, location_flags, floor_doodad, donewsym);
}

void
create_current_floor_location(x, y, location_flags, floor_doodad, donewsym)
xchar x, y;
int floor_doodad;
unsigned short location_flags;
boolean donewsym;
{
    if (IS_FLOOR(levl[x][y].typ))
        return; /* Do nothing, floor already */

    int type, subtype, vartype;
    schar carpet_typ, carpet_piece;
    uchar carpet_flags;
    if (!levl[x][y].floortyp)
    {
        /* Backup */
        type = location_type_definitions[levl[x][y].typ].initial_floor_type;
        subtype = get_initial_location_subtype(type);
        vartype = get_initial_location_vartype(type, subtype);
        carpet_typ = 0;
        carpet_piece = 0;
        carpet_flags = 0;
    }
    else
    {
        type = levl[x][y].floortyp;
        subtype = levl[x][y].floorsubtyp;
        vartype = levl[x][y].floorvartyp;
        carpet_typ = levl[x][y].carpet_typ;
        carpet_piece = levl[x][y].carpet_piece;
        carpet_flags = levl[x][y].carpet_flags;
    }

    create_simple_location_with_carpet(x, y, type, subtype, vartype, location_flags, carpet_typ, carpet_piece, carpet_flags, floor_doodad, 0, 0, 0, donewsym);
}

void
create_basic_floor_location(x, y, type, subtype, location_flags, donewsym)
xchar x, y;
int type, subtype;
unsigned short location_flags;
boolean donewsym;
{
    if (!isok(x, y))
        return;

    int vartype = get_initial_location_vartype(type, subtype);
    full_location_transform(x, y, type, subtype, vartype, location_flags, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, FALSE, FALSE, 0, 0, donewsym);
}

void
transform_location_type(x, y, type, subtype)
xchar x, y;
int type, subtype;
{
    debugprint("transform_location_type");

    /* First, only limited delete */
    if (levl[x][y].typ == FOUNTAIN)
        level.flags.nfountains--;
    else if (levl[x][y].typ == SINK)
        level.flags.nsinks--;

    if (levl[x][y].lamplit)
    {
        del_light_source(LS_LOCATION, xy_to_any(x, y));
        levl[x][y].lamplit = 0;
    }
    if (levl[x][y].makingsound)
    {
        del_sound_source(SOUNDSOURCE_LOCATION, xy_to_any(x, y));
        levl[x][y].makingsound = 0;
    }

    /* Then, change type */
    if (IS_FLOOR(levl[x][y].typ) && !IS_FLOOR(type))
    {
        levl[x][y].floortyp = levl[x][y].typ;
        levl[x][y].floorsubtyp = levl[x][y].subtyp;
        levl[x][y].floorvartyp = levl[x][y].vartyp;
    }
    else if (IS_FLOOR(type))
    {
        levl[x][y].floortyp = 0;
        levl[x][y].floorsubtyp = 0;
        levl[x][y].floorvartyp = 0;
    }

    levl[x][y].typ = type;
    levl[x][y].subtyp = subtype;
    levl[x][y].vartyp = get_initial_location_vartype(type, subtype);

    maybe_create_location_light_and_sound_sources(x, y);
    if (levl[x][y].typ == FOUNTAIN)
        level.flags.nfountains++;
    else if (levl[x][y].typ == SINK)
        level.flags.nsinks++;
}

void
transform_location_type_and_flags(x, y, type, subtype, location_flags)
xchar x, y;
int type, subtype;
unsigned short location_flags;
{
    levl[x][y].flags = location_flags;
    transform_location_type(x, y, type, subtype); /* Does not clear flags */
}

void
transform_location_type_and_flags_and_set_broken(x, y, type, subtype, location_flags, floor_doodad)
xchar x, y;
int type, subtype, floor_doodad;
unsigned short location_flags;
{
    levl[x][y].flags = location_flags;
    levl[x][y].floor_doodad = floor_doodad;
    transform_location_type(x, y, type, subtype); /* Does not clear flags nor floor_doodad */
}

const char*
get_obj_subtype_name(obj)
struct obj* obj;
{
    if (!obj)
        return empty_string;

    int otyp = obj->otyp;
    return get_otyp_subtype_name(otyp);

}

const char*
get_otyp_subtype_name(otyp)
int otyp;
{
    if (otyp < 0 || otyp >= NUM_OBJECTS)
        return empty_string;

    if (objects[otyp].oc_class == WEAPON_CLASS)
    {
        return weapon_type_names[objects[otyp].oc_subtyp];
    }
    else if (objects[otyp].oc_class == ARMOR_CLASS)
    {
        return armor_type_names[objects[otyp].oc_subtyp];
    }
    else if (objects[otyp].oc_class == MISCELLANEOUS_CLASS && objects[otyp].oc_subtyp > MISC_MULTIPLE_PERMITTED)
    {
        return misc_type_names[objects[otyp].oc_subtyp];
    }
    else if (objects[otyp].oc_class == FOOD_CLASS && objects[otyp].oc_subtyp > FOODTYPE_GENERAL)
    {
        return food_type_names[objects[otyp].oc_subtyp];
    }
    else if (objects[otyp].oc_class == TOOL_CLASS && objects[otyp].oc_subtyp > TOOLTYPE_GENERAL)
    {
        return tool_type_names[objects[otyp].oc_subtyp];
    }
    else if (objects[otyp].oc_class == TOOL_CLASS)
    {
        if (is_otyp_candle(otyp))
        {
            return "candle";
        }
        if (is_otyp_key(otyp))
        {
            return "key";
        }
    }
    else if (objects[otyp].oc_class == AMULET_CLASS || objects[otyp].oc_class == RING_CLASS || objects[otyp].oc_class == POTION_CLASS)
    {
        //Nothing
    }
    else if (objects[otyp].oc_class == GEM_CLASS)
    {
        //Nothing
        if (is_otyp_graystone(otyp))
        {
            return "gray stone";
        }
        if (is_otyp_rock(otyp))
        {
            return "rock";
        }
        if (is_otyp_ore(otyp))
        {
            return "ore";
        }
    }
    return empty_string;
}

struct extended_create_window_info
extended_create_window_info_from_obj(obj)
struct obj* obj;
{
    struct extended_create_window_info info = { 0 };
    info.object = obj;
    return info;
}

struct extended_create_window_info
extended_create_window_info_from_mon(mon)
struct monst* mon;
{
    struct extended_create_window_info info = { 0 };
    info.monster = mon;
    return info;
}

struct extended_create_window_info
extended_create_window_info_from_mon_with_flags(mon, cflags)
struct monst* mon;
uint64_t cflags;
{
    struct extended_create_window_info info = { 0 };
    info.monster = mon;
    info.create_flags = cflags;
    return info;
}

void
hint_via_pline(hint_text)
const char* hint_text;
{
    if (hint_text)
    {
        //char hintbuf[BUFSZ];
        //Sprintf(hintbuf, "HINT - %s", hint_text);
        play_sfx_sound(SFX_HINT);
        //pline_ex1(ATR_NONE, CLR_MSG_HINT, hintbuf);
        custompline_ex_prefix(ATR_NONE, CLR_MSG_HINT, "HINT", ATR_NONE, NO_COLOR, " - ", ATR_BOLD, CLR_MSG_HIGHLIGHT, 0, "%s", hint_text);
    }
}

void
pray_hint(pray_what, base_hint, hintflag_ptr)
const char* pray_what;
const char* base_hint;
boolean* hintflag_ptr;
{
    if (!pray_what)
        return;

    if ((flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && (!hintflag_ptr || !*hintflag_ptr))
    {
        if (hintflag_ptr)
            *hintflag_ptr = TRUE;
        boolean canpray = can_pray(FALSE);
        if (!canpray && !base_hint)
            return;
        char buf[BUFSZ];
        Sprintf(buf, "You can %s by %s%s.", pray_what ? pray_what : "", 
            base_hint ? base_hint : "", 
            base_hint && canpray ? " or by praying" : canpray ? "praying" : "");
        hint_via_pline(buf);
    }
}

void
standard_hint(hint_txt, hintflag_ptr)
const char* hint_txt;
boolean* hintflag_ptr;
{
    if (!hint_txt)
        return;

    if ((flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && (!hintflag_ptr || !*hintflag_ptr))
    {
        if (hintflag_ptr)
            *hintflag_ptr = TRUE;
        hint_via_pline(hint_txt);
    }
}

void
reviver_hint(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !is_reviver(mtmp->data))
        return;

    if ((flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && !u.uhint.monster_revived)
    {
        u.uhint.monster_revived = TRUE;

        boolean istinnable = !is_rider(mtmp->data) && !has_monster_type_nontinnable_corpse(mtmp->data);
        boolean isedible = !is_rider(mtmp->data) && !has_monster_type_nontinnable_corpse(mtmp->data) && !is_not_living(mtmp->data);
        char buf[BUFSZ];
        char sbuf[BUFSZ] = "";
        if(istinnable)
            Strcat(sbuf, "tinning");
        if (isedible)
        {
            if (istinnable)
                Strcat(sbuf, " or ");
            Strcat(sbuf, "eating");
        }
        if(istinnable || isedible)
            Strcat(sbuf, " them");

        if(*sbuf)
            Sprintf(buf, "You can get rid of reviving corpses by %s.", sbuf);
        else
            Strcpy(buf, "You can normally get rid of reviving corpses by eating or tinning them, but it looks more difficult here.");

        hint_via_pline(buf);
    }
}

void
item_destruction_hint(adtyp, isray)
int adtyp;
boolean isray;
{
    if (!(flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty))
        return;

    boolean* hint_bool_ptr = 0;
    const char* hintfmt = "%s";
    const char* raytxt = isray ? "reflection or " : "";
    switch (adtyp)
    {
    case AD_FIRE:
        hintfmt = "You can protect your scrolls, books and potions from being destroyed by fire damage by keeping them in a bag, or by acquiring %sfull fire resistance.";
        hint_bool_ptr = &u.uhint.items_destroyed_by_fire;
        break;
    case AD_COLD:
        hintfmt = "You can protect your potions from being destroyed by cold damage by keeping them in a bag, or by acquiring %sfull cold resistance.";
        hint_bool_ptr = &u.uhint.items_destroyed_by_cold;
        break;
    case AD_ELEC:
        hintfmt = "You can protect your rings and wands from being destroyed by electrical damage by keeping them in a bag, or by acquiring %sfull shock resistance.";
        hint_bool_ptr = &u.uhint.items_destroyed_by_shock;
        break;
    default:
        break;
    }
    if (hint_bool_ptr && !*hint_bool_ptr)
    {
        char buf[BUFSZ * 2];
        Sprintf(buf, hintfmt, raytxt);
        standard_hint(buf, hint_bool_ptr);
    }

}

void
brain_hint(mtmp)
struct monst* mtmp;
{
    if ((flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && !u.uhint.brain_got_eaten)
    {
        u.uhint.brain_got_eaten = TRUE;
        hint_via_pline("To protect yourself agains brain-eating attacks, you can wear a nose ring of cerebral safeguarding or wear a helmet, which gives you a high chance of blocking the attacks.");
        if(!Race_if(PM_DWARF) && is_tentacled_one(mtmp->data))
            hint_via_pline("You can also class-genocide all tentacled ones (and all dwarves) by reading a blessed scroll of genocide when you are not a dwarf yourself.");
    }
}

void
grab_hint(mtmp)
struct monst* mtmp;
{
    if (!mtmp || mtmp != u.ustuck)
        return;

    if ((flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && !u.uhint.got_grabbed)
    {
        u.uhint.got_grabbed = TRUE;

        boolean isconstrictor = is_constrictor(mtmp->data);
        boolean hughthrottles = hug_throttles(mtmp->data) && can_be_strangled(&youmonst);

        char buf[BUFSZ];
        char sbuf[BUFSZ] = "";
        if (u.uevent.elbereth_known)
            Strcat(sbuf, "writing Elbereth");
        if(*sbuf)
            Strcat(sbuf, ", ");
        Strcat(sbuf, "dropping a scroll of scare monster");
        if (*sbuf)
            Strcat(sbuf, " or ");
        Strcat(sbuf, "teleporting yourself or the monster away");

        Sprintf(buf, "You can release yourself from the clutches of a %s monster by %s.",
            isconstrictor ? "constricting" : hughthrottles ? "throttling" : "grabbing", sbuf);

        hint_via_pline(buf);
    }
}

void
check_mobbed_hint(VOID_ARGS)
{
    if (!(flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) || u.uhint.got_mobbed)
        return;

    int i, j, x, y;
    struct monst* mtmp = 0;
    int cnt = 0;
    for (i = -1; i <= 1; i++)
    {
        for (j = -1; j <= 1; j++)
        {
            if (!i && !j)
                continue;

            x = u.ux + i;
            y = u.uy + j;
            if(!isok(x, y))
                continue;

            mtmp = m_at(x, y);
            if (mtmp && !DEADMONSTER(mtmp) && !is_peaceful(mtmp) && !is_tame(mtmp) && canspotmon(mtmp))
            {
                cnt++;
            }
        }
    }

    if (cnt >= 3)
    {
        u.uhint.got_mobbed = TRUE;
        char buf[BUFSZ * 2] = "";
        Sprintf(buf, "If you are mobbed by monsters, try to use a wand or scroll of teleportation%s, drop a scroll of scare monster, use an item causing conflict, or read a scroll of taming.",
            u.uevent.elbereth_known ? ", write Elbereth" : "");
        hint_via_pline(buf);
    }
}

void
check_closed_for_inventory_hint(VOID_ARGS)
{
    register struct engr* ep = engr_at(u.ux, u.uy);
    if ((flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && !u.uhint.closed_for_inventory 
        && ep && ep->engr_type == ENGR_SIGNPOST && ep->engr_txt && !strcmp(ep->engr_txt, Closed_for_inventory))
    {
        u.uhint.closed_for_inventory = 1;
        play_sfx_sound(SFX_WARNING);
        char buf[BUFSZ];
        Sprintf(buf, "A \"%s\" sign is adjacent a locked shop door.", Closed_for_inventory);
        custompline_ex_prefix(ATR_NONE, CLR_MSG_HINT, "HINT", ATR_NONE, NO_COLOR, " - ", ATR_BOLD, CLR_MSG_HIGHLIGHT, 0U, "%s", buf);
        custompline_ex_prefix(ATR_NONE, CLR_MSG_WARNING, "WARNING", ATR_NONE, NO_COLOR, " - ", ATR_NONE, CLR_MSG_HIGHLIGHT, 0U, "The shopkeeper will get angry if you break the door.");
        //standard_hint("A \"Closed for inventory\" sign indicates that the door is a locked shop door.  The shopkeeper will get angry if you break the door.", &u.uhint.closed_for_inventory);
    }
}

void
death_hint(VOID_ARGS)
{
    if ((flags.force_hint || context.game_difficulty <= flags.max_hint_difficulty) && killer.name[0] && killer.hint_idx > 0)
    {
        uint64_t kbit = (uint64_t)1 << (killer.hint_idx % 64);
        uint64_t* hints_given = &u.uhint.kill_hints_given[min(NUM_KILL_HINT_ULONGS - 1, killer.hint_idx / 64)];
        if (((*hints_given) & kbit) == 0)
        {
            *hints_given |= kbit;

            char buf[BUFSZ * 2] = "";
            switch (killer.hint_idx)
            {
            case HINT_KILLED_PETRIFICATION_RAY:
                Strcpy(buf, "Acquire petrification resistance or reflection as early on as possible to avoid being killed by petrification rays. To cure petrification, eat a lizard corpse or a dragon fruit, or use a jar of basilisk blood.");
                break;
            case HINT_KILLED_COCKATRICE:
            case HINT_KILLED_PETRIFICATION:
                Strcpy(buf, "To cure petrification, eat a lizard corpse or a dragon fruit, or use a jar of basilisk blood.");
                break;
            case HINT_KILLED_TOUCHED_COCKATRICE_CORPSE:
                Strcpy(buf, "You should wear gloves before touching a cockatrice corpse, cockatrice meat, or similar petrifying item.");
                break;
            case HINT_KILLED_TOUCHED_COCKATRICE:
                Strcpy(buf, "You should wear gloves before touching a cockatrice or similar petrifying monster.");
                break;
            case HINT_KILLED_ATE_COCKATRICE_CORPSE:
                Strcpy(buf, "Always make sure that you are not eating petrifying meat, e.g., by checking it with a wand of probing.");
                break;
            case HINT_KILLED_HIT_BY_COCKATRICE_CORPSE:
                Strcpy(buf, "Keep an eye on monsters wielding a cockatrice corpse, and kill them from a distance.");
                break;
            case HINT_KILLED_DROWNED:
                Strcpy(buf, "Be careful not to enter pools or water, unless you have magical breathing, or you can fly, levitate or waterwalk over them.");
                break;
            case HINT_KILLED_DROWNED_BY_MONSTER:
                Strcpy(buf, "To avoid being drawned by a monster, kill it quickly or teleport either it or yourself away.");
                break;
            case HINT_KILLED_SUFFOCATION:
                Strcpy(buf, "Acquire an amulet of magical breathing to avoid suffocation.");
                break;
            case HINT_KILLED_SUFFOCATION_BY_BEING_BURIED:
                Strcpy(buf, "Acquire an amulet of magical breathing to avoid suffocation by being buried.");
                break;
            case HINT_KILLED_ITEM_STRANGULATION:
                Strcpy(buf, "You can remove a cursed strangulation item by uncursing it first, for example, by praying.");
                break;
            case HINT_KILLED_MONSTER_STRANGULATION:
                Sprintf(buf, "You can stop a monster from strangling you by praying%s or teleporting it or yourself away.", u.uevent.elbereth_known ? ", writing Elbereth," : "");
                break;
            case HINT_KILLED_DISINTEGRATION_RAY:
                Strcpy(buf, "Acquire disintegration resistance or reflection as early on as possible to avoid being killed by disintegration rays.");
                break;
            case HINT_KILLED_MEDUSA_GAZE:
                Strcpy(buf, "Acquire petrification resistance or reflection, or use a blindfold or a towel to make yourself blind, before engaging in combat with Medusa to avoid being turned to stone by her gaze.");
                break;
            case HINT_KILLED_TOUCH_OF_DEATH:
                Strcpy(buf, "Acquire death resistance before engaging in combat with enemies capable of casting Touch of Death, such as greater mummy high priests.");
                break;
            case HINT_KILLED_DEATH_RAY:
                Strcpy(buf, "Acquire death resistance or reflection as early on as possible to avoid being killed by death rays.");
                break;
            case HINT_KILLED_FOOD_POISONING:
                Strcpy(buf, "Acquire sick resistance as early on as possible to avoid being killed by food poisoning. To cure food poisoning, you can eat an eucalyptus leaf or a fig.");
                break;
            case HINT_KILLED_TERMINAL_ILLNESS:
                Strcpy(buf, "Acquire sick resistance as early on as possible to avoid being killed by terminal illness. To cure terminal illness, you can eat an eucalyptus leaf or a fig.");
                break;
            case HINT_KILLED_SLIMED:
                Strcpy(buf, "You can burn slime away to avoid being turned into green slime.");
                break;
            case HINT_KILLED_MUMMY_ROT:
                Strcpy(buf, "Acquire sick resistance as early on as possible to avoid being killed by mummy rot. To cure mummy rot, you can eat an eucalyptus leaf or a fig.");
                break;
            case HINT_KILLED_OLD_CORPSE:
                Strcpy(buf, "Do not eat corpses that have died a long time ago.");
                break;
            case HINT_KILLED_TAINTED_CORPSE:
                Strcpy(buf, "Some corpses are inherently tainted. You can check this, e.g., by using a wand of probing.");
                break;
            case HINT_KILLED_SICKENING_CORPSE:
                Strcpy(buf, "Some corpses are inherently infected by terminal illness. You can check this, e.g., by using a wand of probing.");
                break;
            case HINT_KILLED_POTION_OF_SICKNESS:
                Strcpy(buf, "A potion of sickness can cause terminal illness. Identify potions first, or cure sickness eating a fig or an eucalyptus leaf.");
                break;
            case HINT_KILLED_MUMMY_ROTTED_CORPSE:
                Strcpy(buf, "Some corpses are inherently infected by mummy rot. You can check this, e.g., by using a wand of probing.");
                break;
            case HINT_KILLED_ILLNESS_FROM_CURSED_UNICORN_HORN:
                Strcpy(buf, "A cursed unicorn horn can infect you with terminal illness. Identify it or determine its blessedness on an altar.");
                break;
            case HINT_KILLED_ATE_GREEN_SLIME:
                Strcpy(buf, "Do not eat green slime.");
                break;
            case HINT_KILLED_GENOCIDED_PLAYER:
                Strcpy(buf, "Be careful not to genocide your player race, especially if you are a dwarf, orc, or a gnoll.");
                break;
            case HINT_KILLED_STARVATION:
                Strcpy(buf, "Eat food or pray to avoid being starved to death.");
                break;
            }

            if (*buf)
            {
                hint_via_pline(buf);
                display_popup_text(buf, "Hint", POPUP_TEXT_GENERAL, ATR_NONE, CLR_MSG_HINT, NO_GLYPH, POPUP_FLAGS_NONE);
            }
        }
    }
}

void
heal_ailments_upon_revival(VOID_ARGS)
{
    make_blinded(0L, FALSE);
    make_mummy_rotted(0L, (char*)0, FALSE, 0);
    make_stunned(0L, FALSE);
    make_confused(0L, FALSE);
    make_deaf(0L, FALSE);
    make_mummy_rotted(0L, (char*)0, FALSE, 0);
    make_slimed(0L, (char*)0, 0, FALSE, 0);
    make_sick(0L, (char*)0, FALSE, 0);
    make_food_poisoned(0L, (char*)0, FALSE, 0);
    make_hallucinated(0L, FALSE, 0L);
    u.ubasehpdrain = u.ubaseendrain = u.basemhdrain = 0;
    updatemaxhp();
    updatemaxen();
}


#if !defined (GNH_MOBILE) && defined (DEBUG)
STATIC_DCL winid FDECL(write_create_nhwindow_ex, (int, int, int, struct extended_create_window_info));
STATIC_DCL void FDECL(write_display_nhwindow, (winid, BOOLEAN_P));
STATIC_DCL void FDECL(write_destroy_nhwindow, (winid));
STATIC_DCL void FDECL(write_putstr_ex, (winid, const char*, int, int, int));
STATIC_DCL void FDECL(write_putstr_ex2, (winid, const char*, const char*, const char*, int, int, int));
STATIC_PTR int FDECL(CFDECLSPEC spell_wiki_cmp, (const genericptr, const genericptr));
STATIC_PTR int FDECL(CFDECLSPEC monster_wiki_cmp, (const genericptr, const genericptr));

STATIC_VAR int write_fd = -1;

STATIC_OVL winid
write_create_nhwindow_ex(type, style, glyph, info)
int type UNUSED, style UNUSED, glyph UNUSED;
struct extended_create_window_info info UNUSED;
{
    return 0;
}

STATIC_OVL void
write_display_nhwindow(window, blocking)
winid window UNUSED;
boolean blocking UNUSED; /* with ttys, all windows are blocking */
{
    return;
}

STATIC_OVL void
write_destroy_nhwindow(window)
winid window UNUSED;
{
    return;
}

STATIC_OVL void
write_putstr_ex(window, str, attr, color, app)
winid window UNUSED;
int attr, app, color UNUSED;
const char* str;
{
    if (!str || !*str)
        return;

    char buf[BUFSZ * 4];
    *buf = 0;
    boolean removetrailingcolon = FALSE;
    if ((attr & (ATR_SUBTITLE)) == ATR_SUBTITLE || (attr & (ATR_HEADING)) != 0)
    {
        Strcat(buf, "## ");
        removetrailingcolon = TRUE;
    }
    else if ((attr & (ATR_TITLE)) == ATR_TITLE)
        return; // Strcat(buf, "# "); // No need to print the title twice

    if ((attr & ATR_INDENT_MASK) == ATR_INDENT_AT_COLON)
        Strcat(buf, "- **");

    Strcat(buf, str);
    int slen = (int)strlen(buf);
    if (removetrailingcolon && slen > 0 && buf[slen - 1] == ':')
        buf[slen - 1] = 0;

    if ((attr & ATR_INDENT_MASK) == ATR_INDENT_AT_COLON)
    {
        char* p = strchr(buf, ':');
        if (p)
        {
            char buf2[BUFSZ * 4];
            char* p2 = p + 1;
            while (*p2 && *p2 == ' ')
                p2++;

            Strcpy(buf2, p2);
            Strcpy(p + 1, "** ");
            Strcat(buf, buf2);
        }
    }
    else if ((attr & ATR_INDENT_MASK) == ATR_INDENT_AT_DASH)
    {
        char* p = strchr(buf, '-');
        if (p)
        {
            char buf2[BUFSZ * 4];
            char* p2 = p + 1;
            while (*p2 && *p2 == ' ')
                p2++;

            Strcpy(buf2, p2);
            Strcpy(p - 1, ". ");
            Strcat(buf, buf2);
        }
    }

    if (!app)
    {
        mungspaces(buf);
        Strcat(buf, "\n");
    }

    (void)write(write_fd, buf, strlen(buf));
}

STATIC_OVL void
write_putstr_ex2(window, str, attrs, colors, attr, color, app)
winid window;
int attr, color, app;
const char *str, *attrs UNUSED, *colors UNUSED;
{
    write_putstr_ex(window, str, attr, color, app);
}

STATIC_OVL int CFDECLSPEC
spell_wiki_cmp(p, q)
const genericptr p;
const genericptr q;
{
    if (!p || !q)
        return 0;

    short idx1 = *(short*)p;
    short idx2 = *(short*)q;

    const char* name1 = OBJ_NAME(objects[idx1]);
    const char* name2 = OBJ_NAME(objects[idx2]);

    schar skl1 = objects[idx1].oc_skill;
    schar skl2 = objects[idx2].oc_skill;

    const char* skl_name1 = spelltypemnemonic(skl1);
    const char* skl_name2 = spelltypemnemonic(skl2);

    int64_t lvl1 = objects[idx1].oc_spell_level;
    int64_t lvl2 = objects[idx2].oc_spell_level;

    int skill_res = 0;
    if (skl_name1 && skl_name2)
        skill_res = strcmpi(skl_name1, skl_name2);
    else if (skl_name1)
        return 1;
    else
        return -1;

    if (!skill_res && lvl1 != lvl2)
    {
        return (int)(lvl1 - lvl2);
    }

    if (!skill_res && (name1 || name2))
    {
        if (name1 && name2)
            skill_res = strcmpi(name1, name2);
        else if (name1)
            return 1;
        else
            return -1;
    }

    return skill_res;
}

void
write_spells(VOID_ARGS)
{
    pline("Starting writing spells...");

    const char* spelldir = "spells_for_wiki";
    struct stat st = { 0 };
    if (stat(spelldir, &st) == -1) {
#ifdef WIN32
        (void)mkdir(spelldir);
#else
        (void)mkdir(spelldir, 0700);
#endif
    }
    char fq_save[BUFSIZ];
    char name[BUFSZ * 2];

    struct window_procs saved_windowprocs = windowprocs;
    windowprocs.win_putstr_ex = write_putstr_ex;
    windowprocs.win_putstr_ex2 = write_putstr_ex2;
    windowprocs.win_create_nhwindow_ex = write_create_nhwindow_ex;
    windowprocs.win_display_nhwindow = write_display_nhwindow;
    windowprocs.win_destroy_nhwindow = write_destroy_nhwindow;

    int i, j;
    for (i = FIRST_SPELL; i < SPE_BLANK_PAPER; i++)
    {
        name[0] = '\0';
        (void)Strcpy(name, OBJ_NAME(objects[i]));
        int len = (int)strlen(name);
        for (j = 0; j < len; j++) {

            if (name[j] == ' ')
                name[j] = '-';
        }

        *name = highc(*name);

        fq_save[0] = '\0';
        (void)Strcat(fq_save, spelldir);
        (void)Strcat(fq_save, "/");
        (void)Strcat(fq_save, name);
        (void)Strcat(fq_save, ".md");

        (void)remove(fq_save);

#ifdef MAC
        write_fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
        write_fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
        if (write_fd < 0)
            continue;

        spelldescription_core(-1, i);

        (void)close(write_fd);
    }
    windowprocs = saved_windowprocs;

    short spell_indices[SPE_BLANK_PAPER - FIRST_SPELL] = { 0 };
    for (i = FIRST_SPELL; i < SPE_BLANK_PAPER; i++)
        spell_indices[i - FIRST_SPELL] = (short)(i);

    qsort(spell_indices, SPE_BLANK_PAPER - FIRST_SPELL, sizeof(short), spell_wiki_cmp);

    schar skill_idx = P_NONE;
    schar prev_skill_idx = P_NONE;
    int spell_level = -3;
    int prev_spell_level = -3;
    int spl_idx = 0;
    for (i = 0; i < SPE_BLANK_PAPER - FIRST_SPELL; i++)
    {
        spl_idx = spell_indices[i];
        skill_idx = objects[spl_idx].oc_skill;
        spell_level = (int)objects[spl_idx].oc_spell_level;
        if (prev_skill_idx != skill_idx)
        {
            if (prev_skill_idx != P_NONE && write_fd >= 0)
            {
                (void)close(write_fd);
                write_fd = -1;
            }

            name[0] = '\0';
            Strcpy(name, spelltypemnemonic(skill_idx));
            Strcat(name, " spells");
            int len = (int)strlen(name);
            for (j = 0; j < len; j++) {

                if (name[j] == ' ')
                    name[j] = '-';
            }

            *name = highc(*name);

            fq_save[0] = '\0';
            Strcat(fq_save, spelldir);
            Strcat(fq_save, "/");
            Strcat(fq_save, name);
            Strcat(fq_save, ".md");

            (void)remove(fq_save);

#ifdef MAC
            write_fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
            write_fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
            if (write_fd < 0)
                continue;

            //char buf[BUFSIZ];
            //char buf2[BUFSIZ];
            //Strcpy(buf2, spelltypemnemonic(skill_idx));
            //*buf2 = highc(*buf2);
            //Strcpy(buf, "# ");
            //Strcat(buf, buf2);
            //Strcat(buf, " Spells\n");
            //(void)write(write_fd, buf, strlen(buf));
        }

        if (prev_spell_level != spell_level)
        {
            char buf[BUFSZ * 2];
            char buf2[BUFSZ * 2];
            print_spell_level_text(buf2, spl_idx, FALSE, 2, TRUE);
            Strcpy(buf, "## ");
            Strcat(buf, buf2);
            Strcat(buf, "\n");
            (void)write(write_fd, buf, strlen(buf));
        }

        char buf[BUFSZ * 2];
        char buf2[BUFSZ * 2];
        Strcpy(buf2, OBJ_NAME(objects[spl_idx]));
        *buf2 = highc(*buf2);
        Strcpy(buf, "- [[");
        Strcat(buf, buf2);
        Strcat(buf, "]]\n");
        (void)write(write_fd, buf, strlen(buf));

        prev_skill_idx = skill_idx;
        prev_spell_level = spell_level;
    }
    if (write_fd >= 0)
    {
        (void)close(write_fd);
        write_fd = -1;
    }

    pline("Done!");
}

STATIC_OVL int CFDECLSPEC
monster_wiki_cmp(p, q)
const genericptr p;
const genericptr q;
{
    if (!p || !q)
        return 0;

    short idx1 = *(short*)p;
    short idx2 = *(short*)q;

    const char* name1 = mons[idx1].mname;
    const char* name2 = mons[idx2].mname;

    return strcmpi(name1, name2);
}


void
write_monsters(VOID_ARGS)
{
    pline("Starting writing monsters...");

    const char* monsterdir = "monsters_for_wiki";
    struct stat st = { 0 };
    if (stat(monsterdir, &st) == -1) {
#ifdef WIN32
        (void)mkdir(monsterdir);
#else
        (void)mkdir(monsterdir, 0700);
#endif
    }
    char fq_save[BUFSIZ];
    char name[BUFSZ * 2];
    char buf[BUFSZ * 2];
    char buf2[BUFSZ * 2];

    struct window_procs saved_windowprocs = windowprocs;
    windowprocs.win_putstr_ex = write_putstr_ex;
    windowprocs.win_putstr_ex2 = write_putstr_ex2;
    windowprocs.win_create_nhwindow_ex = write_create_nhwindow_ex;
    windowprocs.win_display_nhwindow = write_display_nhwindow;
    windowprocs.win_destroy_nhwindow = write_destroy_nhwindow;

    int i, j, len;
    for (i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        name[0] = '\0';
        Strcpy(name, mons[i].mname);
        if (mons[i].mlet == S_HUMAN && !strncmp(mons[i].mname, "were", 4))
            Strcat(name, " (Human form)");

        len = (int)strlen(name);
        for (j = 0; j < len; j++)
        {
            if (name[j] == ' ')
                name[j] = '-';
        }
        *name = highc(*name);

        fq_save[0] = '\0';
        Strcat(fq_save, monsterdir);
        Strcat(fq_save, "/");
        Strcat(fq_save, name);
        Strcat(fq_save, ".md");

        (void)remove(fq_save);

#ifdef MAC
        write_fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
        write_fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
        if (write_fd < 0)
            continue;

        monsterdescription_core((struct monst*)0, &mons[i]);

        Strcpy(buf, "Corpse properties:");
        putstr(0, ATR_HEADING, buf);
        print_corpse_properties(0, i);

        Strcpy(buf, "Picture:");
        putstr(0, ATR_HEADING, buf);

        name[0] = 0;
        if (mons[i].mlet == S_HUMAN && !strncmp(mons[i].mname, "were", 4))
            Strcat(name, "human-");
        Strcat(name, mons[i].mname);
        len = (int)strlen(name);
        for (j = 0; j < len; j++)
        {
            if (name[j] == ' ')
                name[j] = '_';

            name[j] = lowc(name[j]);
        }

        Strcpy(buf2, mons[i].mname);
        if (mons[i].mlet == S_HUMAN && !strncmp(mons[i].mname, "were", 4))
            Strcat(buf2, " - Human form");
        *buf2 = highc(*buf2);

        Sprintf(buf, "![%s](https://github.com/hyvanmielenpelit/GnollHackTileSet/blob/main/Monsters/%s/%s.png)", buf2, name, name);
        if (mons[i].mflags5 & M5_FEMALE_TILE)
        {
            putstr_ex(0, buf, ATR_NONE, NO_COLOR, 1);
            putstr_ex(0, " ", ATR_NONE, NO_COLOR, 1);
            if (mons[i].mfemalename)
            {
                Strcpy(buf2, mons[i].mfemalename);
                if (mons[i].mlet == S_HUMAN && !strncmp(mons[i].mname, "were", 4))
                    Strcat(buf2, " - Human form");
                *buf2 = highc(*buf2);
            }
            Sprintf(buf, "![%s](https://github.com/hyvanmielenpelit/GnollHackTileSet/blob/main/Monsters/%s/%s_female.png)", buf2, name, name);
        }
        putstr_ex(0, buf, ATR_NONE, NO_COLOR, 0);

        (void)close(write_fd);
    }
    windowprocs = saved_windowprocs;

    short monster_indices[NUM_MONSTERS] = { 0 };
    for (i = LOW_PM; i < NUM_MONSTERS; i++)
        monster_indices[i - LOW_PM] = (short)(i);

    qsort(monster_indices, NUM_MONSTERS - LOW_PM, sizeof(short), monster_wiki_cmp);

    name[0] = '\0';
    fq_save[0] = '\0';
    Strcat(fq_save, monsterdir);
    Strcat(fq_save, "/");
    Strcat(fq_save, "Monsters");
    Strcat(fq_save, ".md");

    (void)remove(fq_save);

#ifdef MAC
    write_fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
    write_fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
    if (write_fd >= 0)
    {
        for (i = 0; i < 26; i++)
        {
            Sprintf(buf, "- [[Monsters starting with %c]]\n", (char)('A' + i));
            (void)write(write_fd, buf, strlen(buf));
        }
        (void)close(write_fd);
        write_fd = -1;
    }

    short m_idx;
    for (i = 0; i < NUM_MONSTERS - LOW_PM; i++)
    {
        m_idx = monster_indices[i];

        if (i <= 0 || lowc(*mons[m_idx].mname) != lowc(*mons[monster_indices[i - 1]].mname))
        {
            if (i > 0 && write_fd >= 0)
            {
                (void)close(write_fd);
                write_fd = -1;
            }

            name[0] = '\0';
            Sprintf(name, "Monsters-starting-with-%c", highc(*mons[m_idx].mname));
            fq_save[0] = '\0';
            Strcat(fq_save, monsterdir);
            Strcat(fq_save, "/");
            Strcat(fq_save, name);
            Strcat(fq_save, ".md");

            (void)remove(fq_save);

#ifdef MAC
            write_fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
            write_fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
            if (write_fd < 0)
                continue;

            Strncpy(buf2, mons[m_idx].mname, 1);
            buf2[1] = 0;
            *buf2 = highc(*buf2);
            Strcpy(buf, "## ");
            Strcat(buf, buf2);
            Strcat(buf, "\n");
            (void)write(write_fd, buf, strlen(buf));
        }

        if (write_fd < 0)
            continue;

        Strcpy(buf2, mons[m_idx].mname);
        if (mons[m_idx].mlet == S_HUMAN && !strncmp(mons[m_idx].mname, "were", 4))
            Strcat(buf2, " (Human form)");
        *buf2 = highc(*buf2);
        Strcpy(buf, "- [[");
        Strcat(buf, buf2);
        Strcat(buf, "]]\n");

        (void)write(write_fd, buf, strlen(buf));
    }
    if (write_fd >= 0)
    {
        (void)close(write_fd);
        write_fd = -1;
    }

    pline("Done!");
}


void
write_items(VOID_ARGS)
{
    pline("Starting writing items...");

    const char* itemdir = "items_for_wiki";
    struct stat st = { 0 };
    if (stat(itemdir, &st) == -1) {
#ifdef WIN32
        (void)mkdir(itemdir);
#else
        (void)mkdir(itemdir, 0700);
#endif
    }
    char fq_save[BUFSIZ];
    char name[BUFSZ * 2];
    char buf[BUFSZ * 2];
    char buf2[BUFSZ * 2];

    struct window_procs saved_windowprocs = windowprocs;
    windowprocs.win_putstr_ex = write_putstr_ex;
    windowprocs.win_putstr_ex2 = write_putstr_ex2;
    windowprocs.win_create_nhwindow_ex = write_create_nhwindow_ex;
    windowprocs.win_display_nhwindow = write_display_nhwindow;
    windowprocs.win_destroy_nhwindow = write_destroy_nhwindow;

    int i, j, len;
    size_t cnt_objs = 0;
    for (i = STRANGE_OBJECT + 1; i < NUM_OBJECTS; i++)
    {
        if (!OBJ_NAME(objects[i]))
            continue;

        name[0] = '\0';
        switch (objects[i].oc_class)
        {
            case SPBOOK_CLASS:
                Strcat(name, "spellbook of ");
                break;
            case SCROLL_CLASS:
                Strcat(name, "scroll of ");
                break;
            case WAND_CLASS:
                Strcat(name, "wand of ");
                break;
            case RING_CLASS:
                Strcat(name, "ring of ");
                break;
            case POTION_CLASS:
                Strcat(name, "potion of ");
                break;
        }
        Strcat(name, OBJ_NAME(objects[i]));
        len = (int)strlen(name);
        for (j = 0; j < len; j++)
        {
            if (name[j] == ' ')
                name[j] = '-';
        }
        *name = highc(*name);

        fq_save[0] = '\0';
        Strcat(fq_save, itemdir);
        Strcat(fq_save, "/");
        Strcat(fq_save, name);
        Strcat(fq_save, ".md");

        (void)remove(fq_save);

#ifdef MAC
        write_fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
        write_fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
        if (write_fd < 0)
            continue;

        itemdescription_core((struct obj*)0, i, (struct item_description_stats*)0);

        (void)close(write_fd);
        cnt_objs++;
    }
    windowprocs = saved_windowprocs;

    short* item_indices = (short*)alloc(cnt_objs * sizeof(short));
    memset(item_indices, 0, cnt_objs * sizeof(short));
    size_t idx_obj = 0;
    for (i = STRANGE_OBJECT + 1; i < NUM_OBJECTS; i++)
    {
        if (!OBJ_NAME(objects[i]))
            continue;

        item_indices[idx_obj] = (short)(i);
        idx_obj++;
    }

    qsort(item_indices, cnt_objs, sizeof(short), item_wiki_cmp);

    name[0] = '\0';
    fq_save[0] = '\0';
    Strcat(fq_save, itemdir);
    Strcat(fq_save, "/");
    Strcat(fq_save, "Alphabetic-list-of-items");
    Strcat(fq_save, ".md");

    (void)remove(fq_save);

#ifdef MAC
    write_fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
    write_fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
    if (write_fd >= 0)
    {
        for (i = 0; i < 26; i++)
        {
            Sprintf(buf, "- [[Items starting with %c]]\n", (char)('A' + i));
            (void)write(write_fd, buf, strlen(buf));
        }
        (void)close(write_fd);
        write_fd = -1;
    }

    short i_idx;
    for (i = 0; i < (short)cnt_objs; i++)
    {
        i_idx = item_indices[i];

        if (i <= 0 || lowc(*OBJ_NAME(objects[i_idx])) != lowc(*OBJ_NAME(objects[item_indices[i - 1]])))
        {
            if (i > 0 && write_fd >= 0)
            {
                (void)close(write_fd);
                write_fd = -1;
            }

            name[0] = '\0';
            Sprintf(name, "Items-starting-with-%c", highc(*OBJ_NAME(objects[i_idx])));
            fq_save[0] = '\0';
            Strcat(fq_save, itemdir);
            Strcat(fq_save, "/");
            Strcat(fq_save, name);
            Strcat(fq_save, ".md");

            (void)remove(fq_save);

#ifdef MAC
            write_fd = macopen(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, TEXT_TYPE);
#else
            write_fd = open(fq_save, O_WRONLY | O_TEXT | O_CREAT | O_TRUNC, FCMASK);
#endif
            if (write_fd < 0)
                continue;

            Strncpy(buf2, OBJ_NAME(objects[i_idx]), 1);
            buf2[1] = 0;
            *buf2 = highc(*buf2);
            Strcpy(buf, "## ");
            Strcat(buf, buf2);
            Strcat(buf, "\n");
            (void)write(write_fd, buf, strlen(buf));
        }

        if (write_fd < 0)
            continue;

        *buf2 = 0;
        switch (objects[i].oc_class)
        {
        case SPBOOK_CLASS:
            Strcat(buf2, "spellbook of ");
            break;
        case SCROLL_CLASS:
            Strcat(buf2, "scroll of ");
            break;
        case WAND_CLASS:
            Strcat(buf2, "wand of ");
            break;
        case RING_CLASS:
            Strcat(buf2, "ring of ");
            break;
        case POTION_CLASS:
            Strcat(buf2, "potion of ");
            break;
        }
        Strcat(buf2, OBJ_NAME(objects[i_idx]));
        *buf2 = highc(*buf2);
        Strcpy(buf, "- [[");
        Strcat(buf, buf2);
        Strcat(buf, "]]\n");

        (void)write(write_fd, buf, strlen(buf));
    }
    if (write_fd >= 0)
    {
        (void)close(write_fd);
        write_fd = -1;
    }
    free((genericptr_t)item_indices);
    pline("Done!");
}


#endif


/*do.c*/
