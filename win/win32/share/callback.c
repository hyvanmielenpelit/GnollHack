/* GnollHack 4.1.0 callback.c */
/* Copyright (c) Janne Gustafsson, 2021. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "callback.h"

STATIC_DCL int NDECL(common_prompt_for_player_selection);

int
common_player_selection()
{
#if 0
    if (iflags.wc_player_selection == VIA_DIALOG)
    {
        /* pick player type randomly (use pre-selected
         * role/race/gender/alignment) */
        if (flags.randomall)
        {
            if (flags.initrole < 0)
            {
                flags.initrole = pick_role(flags.initrace, flags.initgend,
                    flags.initalign, PICK_RANDOM);
                if (flags.initrole < 0)
                {
                    raw_print("Incompatible role!");
                    flags.initrole = randrole(FALSE);
                }
            }

            if (flags.initrace < 0
                || !validrace(flags.initrole, flags.initrace))
            {
                flags.initrace = pick_race(flags.initrole, flags.initgend,
                    flags.initalign, PICK_RANDOM);
                if (flags.initrace < 0)
                {
                    raw_print("Incompatible race!");
                    flags.initrace = randrace(flags.initrole);
                }
            }

            if (flags.initgend < 0
                || !validgend(flags.initrole, flags.initrace,
                    flags.initgend))
            {
                flags.initgend = pick_gend(flags.initrole, flags.initrace,
                    flags.initalign, PICK_RANDOM);
                if (flags.initgend < 0)
                {
                    raw_print("Incompatible gender!");
                    flags.initgend = randgend(flags.initrole, flags.initrace);
                }
            }

            if (flags.initalign < 0
                || !validalign(flags.initrole, flags.initrace,
                    flags.initalign))
            {
                flags.initalign = pick_align(flags.initrole, flags.initrace,
                    flags.initgend, PICK_RANDOM);
                if (flags.initalign < 0)
                {
                    raw_print("Incompatible alignment!");
                    flags.initalign =
                        randalign(flags.initrole, flags.initrace);
                }
            }
        }
        else
        {
            return 1; /* Do callback */
        }
    }
    else
#endif
    { /* iflags.wc_player_selection == VIA_PROMPTS */
        return common_prompt_for_player_selection();
    }
    return 0;
}

STATIC_OVL int
common_prompt_for_player_selection()
{
    int i, k, n;
    char pick4u = 'n', thisch, lastch = 0;
    char pbuf[QBUFSZ], plbuf[QBUFSZ];
    winid win;
    anything any;
    menu_item* selected = 0;

    /* prevent an unnecessary prompt */
    rigid_role_checks();

    /* Should we randomly pick for the player? */
    if (!flags.randomall
        && (flags.initrole == ROLE_NONE || flags.initrace == ROLE_NONE
            || flags.initgend == ROLE_NONE || flags.initalign == ROLE_NONE)
        )
    {
        /* int echoline; */
        char* prompt = build_plselection_prompt(
            pbuf, QBUFSZ, flags.initrole, flags.initrace, flags.initgend,
            flags.initalign, FALSE);

        /* tty_putstr(BASE_WINDOW, 0, ""); */
        /* echoline = wins[BASE_WINDOW]->cury; */

        pick4u = ynq2(prompt);
        if (pick4u == 'q')
            pick4u = '\033';

        /* tty_putstr(BASE_WINDOW, 0, prompt); */
        //do
        //{
        //    /* pick4u = lowc(readchar()); */
        //    if (index(quitchars, pick4u))
        //        pick4u = 'y';
        //} while (!index(ynqchars, pick4u));

        if ((int)strlen(prompt) + 1 < CO)
        {
            /* Echo choice and move back down line */
            /* tty_putsym(BASE_WINDOW, (int)strlen(prompt)+1, echoline,
             * pick4u); */
             /* tty_putstr(BASE_WINDOW, 0, ""); */
        }
        else
            /* Otherwise it's hard to tell where to echo, and things are
             * wrapping a bit messily anyway, so (try to) make sure the next
             * question shows up well and doesn't get wrapped at the
             * bottom of the window.
             */
            /* tty_clear_nhwindow(BASE_WINDOW) */;

        if (pick4u != 'y' && pick4u != 'n')
        {
        give_up: /* Quit */
            if (selected)
                free((genericptr_t)selected);
            return 2; /* Exit Failure */
        }
    }

    (void)root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
        flags.initrace, flags.initgend,
        flags.initalign);

    /* Select a role, if necessary */
    /* we'll try to be compatible with pre-selected race/gender/alignment,
     * but may not succeed */
    if (flags.initrole < 0)
    {
        char rolenamebuf[QBUFSZ];
        /* Process the choice */
        if (pick4u == 'y' || flags.initrole == ROLE_RANDOM
            || flags.randomall)
        {
            /* Pick a random role */
            flags.initrole = pick_role(flags.initrace, flags.initgend,
                flags.initalign, PICK_RANDOM);
            if (flags.initrole < 0)
            {
                /* tty_putstr(BASE_WINDOW, 0, "Incompatible role!"); */
                flags.initrole = randrole(FALSE);
            }
        }
        else
        {
            /* tty_clear_nhwindow(BASE_WINDOW); */
            /* tty_putstr(BASE_WINDOW, 0, "Choosing Character's Role"); */
            /* Prompt for a role */
            win = create_nhwindow(NHW_MENU);
            start_menu_ex(win, GHMENU_STYLE_CHOOSE_PLAYER);
            any = zeroany; /* zero out all bits */
            for (i = 0; roles[i].name.m; i++)
            {
                if (ok_role(i, flags.initrace, flags.initgend,
                    flags.initalign))
                {
                    any.a_int = i + 1; /* must be non-zero */
                    thisch = lowc(roles[i].name.m[0]);
                    if (thisch == lastch)
                        thisch = highc(thisch);
                    if (flags.initgend != ROLE_NONE
                        && flags.initgend != ROLE_RANDOM)
                    {
                        if (flags.initgend == 1 && roles[i].name.f)
                            Strcpy(rolenamebuf, roles[i].name.f);
                        else
                            Strcpy(rolenamebuf, roles[i].name.m);
                    }
                    else
                    {
                        if (roles[i].name.f)
                        {
                            Strcpy(rolenamebuf, roles[i].name.m);
                            Strcat(rolenamebuf, "/");
                            Strcat(rolenamebuf, roles[i].name.f);
                        }
                        else
                            Strcpy(rolenamebuf, roles[i].name.m);
                    }
                    int player_glyph_index = player_to_glyph_index(i, 
                        flags.initrace >= 0 ? flags.initrace : RACE_HUMAN,
                        flags.initgend >= 0 ? flags.initgend : GENDER_MALE,
                        flags.initalign >= 0 ? aligns[flags.initalign].value : A_NEUTRAL, 
                        0);
                    int glyph = player_glyph_index + GLYPH_PLAYER_OFF;
                    add_menu(win, glyph, &any, thisch, 0, ATR_NONE, NO_COLOR,
                        an(rolenamebuf), MENU_UNSELECTED);
                    lastch = thisch;
                }
            }
            any.a_int = pick_role(flags.initrace, flags.initgend,
                flags.initalign, PICK_RANDOM) + 1;
            if (any.a_int == 0) /* must be non-zero */
                any.a_int = randrole(FALSE) + 1;
            add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, NO_COLOR, "Random",
                MENU_UNSELECTED);
            any.a_int = i + 1; /* must be non-zero */
            add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
                MENU_UNSELECTED);
            Sprintf(pbuf, "Pick a role for your %s", plbuf);
            end_menu(win, pbuf);
            n = select_menu(win, PICK_ONE, &selected);
            destroy_nhwindow(win);

            /* Process the choice */
            if (n != 1 || selected[0].item.a_int == any.a_int)
                goto give_up; /* Selected quit */

            flags.initrole = selected[0].item.a_int - 1;
            free((genericptr_t)selected), selected = 0;
        }
        (void)root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
            flags.initrace, flags.initgend,
            flags.initalign);
    }

    /* Select a race, if necessary */
    /* force compatibility with role, try for compatibility with
     * pre-selected gender/alignment */
    if (flags.initrace < 0 || !validrace(flags.initrole, flags.initrace))
    {
        /* pre-selected race not valid */
        if (pick4u == 'y' || flags.initrace == ROLE_RANDOM
            || flags.randomall) {
            flags.initrace = pick_race(flags.initrole, flags.initgend,
                flags.initalign, PICK_RANDOM);
            if (flags.initrace < 0) {
                /* tty_putstr(BASE_WINDOW, 0, "Incompatible race!"); */
                flags.initrace = randrace(flags.initrole);
            }
        }
        else
        { /* pick4u == 'n' */
            /* Count the number of valid races */
            n = 0; /* number valid */
            k = 0; /* valid race */
            for (i = 0; races[i].noun; i++)
            {
                if (ok_race(flags.initrole, i, flags.initgend,
                    flags.initalign)) {
                    n++;
                    k = i;
                }
            }

            if (n == 0) {
                for (i = 0; races[i].noun; i++)
                {
                    if (validrace(flags.initrole, i))
                    {
                        n++;
                        k = i;
                    }
                }
            }

            /* Permit the user to pick, if there is more than one */
            if (n > 1)
            {
                /* tty_clear_nhwindow(BASE_WINDOW); */
                /* tty_putstr(BASE_WINDOW, 0, "Choosing Race"); */
                win = create_nhwindow(NHW_MENU);
                start_menu_ex(win, GHMENU_STYLE_CHOOSE_PLAYER);
                any = zeroany; /* zero out all bits */
                for (i = 0; races[i].noun; i++)
                    if (ok_race(flags.initrole, i, flags.initgend,
                        flags.initalign))
                    {
                        any.a_int = i + 1; /* must be non-zero */
                        int player_glyph_index = player_to_glyph_index(flags.initrole,
                            i,
                            flags.initgend >= 0 ? flags.initgend : GENDER_MALE,
                            flags.initalign >= 0 ? aligns[flags.initalign].value : A_NEUTRAL,
                            0);
                        int glyph = player_glyph_index + GLYPH_PLAYER_OFF;
                        add_menu(win, glyph, &any, races[i].noun[0], 0,
                            ATR_NONE, NO_COLOR, races[i].noun, MENU_UNSELECTED);
                    }

                any.a_int = pick_race(flags.initrole, flags.initgend,
                    flags.initalign, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randrace(flags.initrole) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, NO_COLOR, "Random",
                    MENU_UNSELECTED);
                any.a_int = i + 1; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
                    MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the race of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                if (n != 1 || selected[0].item.a_int == any.a_int)
                    goto give_up; /* Selected quit */

                k = selected[0].item.a_int - 1;
                free((genericptr_t)selected), selected = 0;
            }
            flags.initrace = k;
        }
        (void)root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
            flags.initrace, flags.initgend,
            flags.initalign);
    }

    /* Select a gender, if necessary */
    /* force compatibility with role/race, try for compatibility with
     * pre-selected alignment */
    if (flags.initgend < 0
        || !validgend(flags.initrole, flags.initrace, flags.initgend))
    {
        /* pre-selected gender not valid */
        if (pick4u == 'y' || flags.initgend == ROLE_RANDOM
            || flags.randomall)
        {
            flags.initgend = pick_gend(flags.initrole, flags.initrace,
                flags.initalign, PICK_RANDOM);
            if (flags.initgend < 0) {
                /* tty_putstr(BASE_WINDOW, 0, "Incompatible gender!"); */
                flags.initgend = randgend(flags.initrole, flags.initrace);
            }
        }
        else
        { /* pick4u == 'n' */
            /* Count the number of valid genders */
            n = 0; /* number valid */
            k = 0; /* valid gender */
            for (i = 0; i < ROLE_GENDERS; i++)
            {
                if (ok_gend(flags.initrole, flags.initrace, i,
                    flags.initalign))
                {
                    n++;
                    k = i;
                }
            }

            if (n == 0)
            {
                for (i = 0; i < ROLE_GENDERS; i++)
                {
                    if (validgend(flags.initrole, flags.initrace, i))
                    {
                        n++;
                        k = i;
                    }
                }
            }

            /* Permit the user to pick, if there is more than one */
            if (n > 1)
            {
                /* tty_clear_nhwindow(BASE_WINDOW); */
                /* tty_putstr(BASE_WINDOW, 0, "Choosing Gender"); */
                win = create_nhwindow(NHW_MENU);
                start_menu_ex(win, GHMENU_STYLE_CHOOSE_PLAYER);
                any = zeroany; /* zero out all bits */
                for (i = 0; i < ROLE_GENDERS; i++)
                    if (ok_gend(flags.initrole, flags.initrace, i,
                        flags.initalign))
                    {
                        any.a_int = i + 1;
                        int player_glyph_index = player_to_glyph_index(flags.initrole,
                            flags.initrace,
                            i,
                            flags.initalign >= 0 ? aligns[flags.initalign].value : A_NEUTRAL,
                            0);
                        int glyph = player_glyph_index + GLYPH_PLAYER_OFF;
                        add_menu(win, glyph, &any, genders[i].adj[0], 0,
                            ATR_NONE, NO_COLOR, genders[i].adj, MENU_UNSELECTED);
                    }

                any.a_int = pick_gend(flags.initrole, flags.initrace,
                    flags.initalign, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randgend(flags.initrole, flags.initrace) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, NO_COLOR, "Random",
                    MENU_UNSELECTED);
                any.a_int = i + 1; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
                    MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the gender of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                if (n != 1 || selected[0].item.a_int == any.a_int)
                    goto give_up; /* Selected quit */

                k = selected[0].item.a_int - 1;
                free((genericptr_t)selected), selected = 0;
            }
            flags.initgend = k;
        }
        (void)root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
            flags.initrace, flags.initgend,
            flags.initalign);
    }

    /* Select an alignment, if necessary */
    /* force compatibility with role/race/gender */
    if (flags.initalign < 0
        || !validalign(flags.initrole, flags.initrace, flags.initalign))
    {
        /* pre-selected alignment not valid */
        if (pick4u == 'y' || flags.initalign == ROLE_RANDOM
            || flags.randomall)
        {
            flags.initalign = pick_align(flags.initrole, flags.initrace,
                flags.initgend, PICK_RANDOM);
            if (flags.initalign < 0)
            {
                /* tty_putstr(BASE_WINDOW, 0, "Incompatible alignment!"); */
                flags.initalign = randalign(flags.initrole, flags.initrace);
            }
        }
        else
        { /* pick4u == 'n' */
            /* Count the number of valid alignments */
            n = 0; /* number valid */
            k = 0; /* valid alignment */
            for (i = 0; i < ROLE_ALIGNS; i++)
            {
                if (ok_align(flags.initrole, flags.initrace, flags.initgend,
                    i))
                {
                    n++;
                    k = i;
                }
            }
            if (n == 0)
            {
                for (i = 0; i < ROLE_ALIGNS; i++)
                {
                    if (validalign(flags.initrole, flags.initrace, i))
                    {
                        n++;
                        k = i;
                    }
                }
            }

            /* Permit the user to pick, if there is more than one */
            if (n > 1)
            {
                /* tty_clear_nhwindow(BASE_WINDOW); */
                /* tty_putstr(BASE_WINDOW, 0, "Choosing Alignment"); */
                win = create_nhwindow(NHW_MENU);
                start_menu_ex(win, GHMENU_STYLE_CHOOSE_PLAYER);
                any = zeroany; /* zero out all bits */
                for (i = 0; i < ROLE_ALIGNS; i++)
                    if (ok_align(flags.initrole, flags.initrace,
                        flags.initgend, i))
                    {
                        any.a_int = i + 1;
                        int player_glyph_index = player_to_glyph_index(flags.initrole,
                            flags.initrace,
                            flags.initgend,
                            aligns[i].value,
                            0);
                        int glyph = player_glyph_index + GLYPH_PLAYER_OFF;
                        add_menu(win, glyph, &any, aligns[i].adj[0], 0,
                            ATR_NONE, NO_COLOR, aligns[i].adj, MENU_UNSELECTED);
                    }

                any.a_int = pick_align(flags.initrole, flags.initrace,
                    flags.initgend, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randalign(flags.initrole, flags.initrace) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, NO_COLOR, "Random",
                    MENU_UNSELECTED);
                any.a_int = i + 1; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
                    MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the alignment of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                if (n != 1 || selected[0].item.a_int == any.a_int)
                    goto give_up; /* Selected quit */

                k = selected[0].item.a_int - 1;
                free((genericptr_t)selected), selected = 0;
            }
            flags.initalign = k;
        }
    }
    /* Success! */
    /* tty_display_nhwindow(BASE_WINDOW, FALSE); */

    return 0;
}

struct objclassdata
get_objclassdata(struct obj* otmp)
{
    struct objclassdata otypdata = { 0 };
    if (!otmp)
        return otypdata;

    otypdata.tile_height = get_obj_height(otmp);
    otypdata.special_quality = objects[otmp->otyp].oc_special_quality;
    otypdata.max_charges = get_obj_max_charge(otmp);

    otypdata.lamplit = otmp->lamplit;
    otypdata.poisoned = otmp->opoisoned;
    otypdata.eroded = otmp->oeroded;
    otypdata.eroded2 = otmp->oeroded2;

    otypdata.corrodeable = is_corrodeable(otmp);
    otypdata.rottable = is_rottable(otmp);
    otypdata.flammable = is_flammable(otmp);
    otypdata.rustprone = is_rustprone(otmp);
    otypdata.poisonable = is_poisonable(otmp);

    return otypdata;
}
