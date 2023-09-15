/* GnollHack 4.1.0 callback.c */
/* Copyright (c) Janne Gustafsson, 2021. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "callback.h"

int
common_player_selection()
{
    int i, k, n;
    char pick4u = 'n', thisch, lastch = 0;
    char pbuf[QBUFSZ], plbuf[QBUFSZ];
    winid win;
    anything any;
    menu_item* selected = 0;
    int irole = flags.initrole, irace = flags.initrace, igend = flags.initgend, ialign = flags.initalign;
    boolean did_pick_role, did_pick_race, did_pick_gender;
    boolean picksomething = (flags.initrole == ROLE_NONE || flags.initrace == ROLE_NONE
        || flags.initgend == ROLE_NONE || flags.initalign == ROLE_NONE);

back_from_role:
    /* prevent an unnecessary prompt */
    rigid_role_checks();

    /* Should we randomly pick for the player? */
    if (!flags.randomall
        && (flags.initrole == ROLE_NONE || flags.initrace == ROLE_NONE
            || flags.initgend == ROLE_NONE || flags.initalign == ROLE_NONE)
        )
    {
        char* prompt = build_plselection_prompt(
            pbuf, QBUFSZ, flags.initrole, flags.initrace, flags.initgend,
            flags.initalign, FALSE);

        pick4u = ynq2(prompt);
        if (pick4u == 'q')
            pick4u = '\033';

        if (pick4u != 'y' && pick4u != 'n')
        {
give_up: /* Quit */
            if (selected)
                free((genericptr_t)selected);
            return 2; /* Exit Failure */
        }
    }

back_from_race:
    (void)root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
        flags.initrace, flags.initgend,
        flags.initalign);

    /* Select a role, if necessary */
    /* we'll try to be compatible with pre-selected race/gender/alignment,
     * but may not succeed */
    did_pick_role = FALSE, did_pick_race = FALSE, did_pick_gender = FALSE;
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
                flags.initrole = randrole(FALSE);
            }
        }
        else
        {
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
            any.a_int = 0;
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "",
                MENU_UNSELECTED);
            any.a_int = pick_role(flags.initrace, flags.initgend,
                flags.initalign, PICK_RANDOM) + 1;
            if (any.a_int == 0) /* must be non-zero */
                any.a_int = randrole(FALSE) + 1;
            add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, NO_COLOR, "Random",
                MENU_UNSELECTED);
            int back_int = i + 2;
            any.a_int = back_int; /* must be non-zero */
            add_menu(win, NO_GLYPH, &any, '<', 0, ATR_NONE, NO_COLOR, "Back",
                MENU_UNSELECTED);
            int quit_int = i + 1;
            any.a_int = quit_int; /* must be non-zero */
            add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
                MENU_UNSELECTED);
            Sprintf(pbuf, "Pick a role for your %s", plbuf);
            end_menu(win, pbuf);
            n = select_menu(win, PICK_ONE, &selected);
            destroy_nhwindow(win);
            did_pick_role = TRUE;

            /* Process the choice */
            if (n != 1 || selected[0].item.a_int == quit_int)
                goto give_up; /* Selected quit */
            else if (selected[0].item.a_int == back_int)
            {
                free((genericptr_t)selected), selected = 0;
                flags.initrole = irole;
                flags.initrace = irace;
                flags.initgend = igend;
                flags.initalign = ialign;
                goto back_from_role;
            }

            flags.initrole = selected[0].item.a_int - 1;
            free((genericptr_t)selected), selected = 0;
        }
    }

    /* Select a race, if necessary */
    /* force compatibility with role, try for compatibility with
     * pre-selected gender/alignment */
back_from_gender:
    (void)root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
        flags.initrace, flags.initgend,
        flags.initalign);
    did_pick_race = FALSE, did_pick_gender = FALSE;
    if (flags.initrace < 0 || !validrace(flags.initrole, flags.initrace))
    {
        /* pre-selected race not valid */
        if (pick4u == 'y' || flags.initrace == ROLE_RANDOM
            || flags.randomall) {
            flags.initrace = pick_race(flags.initrole, flags.initgend,
                flags.initalign, PICK_RANDOM);
            if (flags.initrace < 0) {
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

                any.a_int = 0;
                add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "",
                    MENU_UNSELECTED);
                any.a_int = pick_race(flags.initrole, flags.initgend,
                    flags.initalign, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randrace(flags.initrole) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, NO_COLOR, "Random",
                    MENU_UNSELECTED);
                int back_int = i + 2;
                any.a_int = back_int; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, '<', 0, ATR_NONE, NO_COLOR, "Back",
                    MENU_UNSELECTED);
                int quit_int = i + 1;
                any.a_int = quit_int; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
                    MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the race of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                did_pick_race = TRUE;

                if (n != 1 || selected[0].item.a_int == quit_int)
                    goto give_up; /* Selected quit */
                else if (selected[0].item.a_int == back_int)
                {
                    free((genericptr_t)selected), selected = 0;
                    flags.initrole = irole;
                    flags.initrace = irace;
                    flags.initgend = igend;
                    flags.initalign = ialign;
                    if (!did_pick_role)
                        goto back_from_role;
                    else
                        goto back_from_race;
                }

                k = selected[0].item.a_int - 1;
                free((genericptr_t)selected), selected = 0;
            }
            flags.initrace = k;
        }
    }

    /* Select a gender, if necessary */
    /* force compatibility with role/race, try for compatibility with
     * pre-selected alignment */
back_from_align:
    (void)root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
        flags.initrace, flags.initgend,
        flags.initalign);
    did_pick_gender = FALSE;
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

                any.a_int = 0;
                add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "",
                    MENU_UNSELECTED);
                any.a_int = pick_gend(flags.initrole, flags.initrace,
                    flags.initalign, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randgend(flags.initrole, flags.initrace) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, NO_COLOR, "Random",
                    MENU_UNSELECTED);
                int back_int = i + 2;
                any.a_int = back_int; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, '<', 0, ATR_NONE, NO_COLOR, "Back",
                    MENU_UNSELECTED);
                int quit_int = i + 1;
                any.a_int = quit_int; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
                    MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the gender of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                did_pick_gender = TRUE;
                if (n != 1 || selected[0].item.a_int == quit_int)
                    goto give_up; /* Selected quit */
                else if (selected[0].item.a_int == back_int)
                {
                    free((genericptr_t)selected), selected = 0;
                    flags.initrace = irace;
                    flags.initgend = igend;
                    flags.initalign = ialign;
                    if (!did_pick_race && !did_pick_role)
                    {
                        flags.initrole = irole;
                        goto back_from_role;
                    }
                    else if (!did_pick_race)
                    {
                        flags.initrole = irole;
                        goto back_from_race;
                    }
                    else
                        goto back_from_gender;
                }

                k = selected[0].item.a_int - 1;
                free((genericptr_t)selected), selected = 0;
            }
            flags.initgend = k;
        }
    }

    (void)root_plselection_prompt(plbuf, QBUFSZ - 1, flags.initrole,
        flags.initrace, flags.initgend,
        flags.initalign);

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

                any.a_int = 0;
                add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "",
                    MENU_UNSELECTED);
                any.a_int = pick_align(flags.initrole, flags.initrace,
                    flags.initgend, PICK_RANDOM) + 1;
                if (any.a_int == 0) /* must be non-zero */
                    any.a_int = randalign(flags.initrole, flags.initrace) + 1;
                add_menu(win, NO_GLYPH, &any, '*', 0, ATR_NONE, NO_COLOR, "Random",
                    MENU_UNSELECTED);
                int back_int = i + 2;
                any.a_int = back_int; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, '<', 0, ATR_NONE, NO_COLOR, "Back",
                    MENU_UNSELECTED);
                int quit_int = i + 1;
                any.a_int = quit_int; /* must be non-zero */
                add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
                    MENU_UNSELECTED);
                Sprintf(pbuf, "Pick the alignment of your %s", plbuf);
                end_menu(win, pbuf);
                n = select_menu(win, PICK_ONE, &selected);
                destroy_nhwindow(win);
                if (n != 1 || selected[0].item.a_int == quit_int)
                    goto give_up; /* Selected quit */
                else if (selected[0].item.a_int == back_int)
                {
                    free((genericptr_t)selected), selected = 0;
                    flags.initgend = igend;
                    flags.initalign = ialign;
                    if (!did_pick_role && !did_pick_race && !did_pick_gender)
                    {
                        flags.initrole = irole;
                        flags.initrace = irace;
                        goto back_from_role;
                    }
                    else if (!did_pick_race && !did_pick_gender)
                    {
                        flags.initrole = irole;
                        flags.initrace = irace;
                        goto back_from_race;
                    }
                    else if (!did_pick_gender)
                    {
                        flags.initrace = irace;
                        goto back_from_gender;
                    }
                    else
                        goto back_from_align;
                }

                k = selected[0].item.a_int - 1;
                free((genericptr_t)selected), selected = 0;
            }
            flags.initalign = k;
        }
    }

    boolean getconfirmation = (picksomething && pick4u != 'a' && !flags.randomall);
    while (getconfirmation) 
    {
        int player_glyph_index = flags.initrole >= 0 && flags.initrace >= 0 && flags.initgend >= 0 && flags.initalign >= 0 ? 
            player_to_glyph_index(flags.initrole,
                flags.initrace,
                flags.initgend,
                aligns[flags.initalign].value,
                0) 
            : NO_GLYPH;
        int glyph = player_glyph_index + GLYPH_PLAYER_OFF;
        win = create_nhwindow_ex(NHW_MENU, 0, glyph, zerocreatewindowinfo);
        start_menu_ex(win, GHMENU_STYLE_ACCEPT_PLAYER);
        any = zeroany; /* zero out all bits */
        any.a_int = 1;
        add_menu(win, NO_GLYPH, &any, 'y', 0, ATR_NONE, NO_COLOR, "Yes; start game",
            MENU_SELECTED);
        any.a_int = 2;
        add_menu(win, NO_GLYPH, &any, 'n', 0, ATR_NONE, NO_COLOR,
            "No; choose role again", MENU_UNSELECTED);
        if (iflags.renameallowed) {
            any.a_int = 3;
            add_menu(win, NO_GLYPH, &any, 'a', 0, ATR_NONE, NO_COLOR,
                "Not yet; choose another name", MENU_UNSELECTED);
        }
        any.a_int = -1;
        add_menu(win, NO_GLYPH, &any, 'q', 0, ATR_NONE, NO_COLOR, "Quit",
            MENU_UNSELECTED);

        any.a_int = 0;
        add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "",
            MENU_UNSELECTED);

        char buf[BUFSZ];
        add_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_TITLE, NO_COLOR, "Character Abilities",
            MENU_UNSELECTED);

        add_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_HEADING, NO_COLOR, "Traits",
            MENU_UNSELECTED);
        for (i = 0; i < MAX_TRAIT_DESCRIPTIONS && races[flags.initrace].trait_descriptions[i] != 0 && *(races[flags.initrace].trait_descriptions[i]) != 0; i++)
        {
            Sprintf(buf, "%s", races[flags.initrace].trait_descriptions[i]);
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
        }
        for (i = 0; i < MAX_TRAIT_DESCRIPTIONS && roles[flags.initrole].trait_descriptions[i] != 0 && *(roles[flags.initrole].trait_descriptions[i]) != 0; i++)
        {
            Sprintf(buf, "%s", roles[flags.initrole].trait_descriptions[i]);
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_INDENT_AT_DASH, NO_COLOR, buf,
                MENU_UNSELECTED);
        }
        if (aligns[flags.initalign].value == A_LAWFUL || aligns[flags.initalign].value == A_NEUTRAL)
        {
            Sprintf(buf, "%s", "Loses telepathy and luck if commits murder");
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
        }
        if (aligns[flags.initalign].value == A_CHAOTIC)
        {
            Sprintf(buf, "%s", "Can sacrifice own race");
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
        }

        add_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_HEADING, NO_COLOR, "Ability Scores",
            MENU_UNSELECTED);
        char statbuf[BUFSZ], minstr[BUFSZ], maxstr[BUFSZ];
        int sumbaseatrs = 0;
        for (i = A_STR; i < A_MAX; i++)
        {
            sumbaseatrs += roles[flags.initrole].attrbase[i];
            switch (i)
            {
            case A_STR:
                Strcpy(statbuf, "Str");
                break;
            case A_DEX:
                Strcpy(statbuf, "Dex");
                break;
            case A_CON:
                Strcpy(statbuf, "Con");
                break;
            case A_INT:
                Strcpy(statbuf, "Int");
                break;
            case A_WIS:
                Strcpy(statbuf, "Wis");
                break;
            case A_CHA:
                Strcpy(statbuf, "Cha");
                break;
            default:
                Strcpy(statbuf, "");
                break;
            }
            if (i == A_STR)
            {
                Sprintf(minstr, "%s", get_strength_string(races[flags.initrace].attrmin[i]));
                Sprintf(maxstr, "%s", get_strength_string(races[flags.initrace].attrmax[i]));
            }
            else
            {
                Sprintf(minstr, "%d", races[flags.initrace].attrmin[i]);
                Sprintf(maxstr, "%d", races[flags.initrace].attrmax[i]);
            }
            Sprintf(buf, "%s: Base:%2d Dist:%2d%% Min:%s Max:%s",
                statbuf,
                roles[flags.initrole].attrbase[i],
                roles[flags.initrole].attrdist[i],
                minstr, maxstr);
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
        }
        Sprintf(buf, "(%d randomly distributed ability scores)", SUM_INIT_ATTRIBUTES - sumbaseatrs);
        add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
            MENU_UNSELECTED);

        add_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_HEADING, NO_COLOR, "Hit Point Advancement",
            MENU_UNSELECTED);
        if (roles[flags.initrole].xlev < MAXULEV)
        {
            Sprintf(buf, "Initial:         %s", get_advancement_description(flags.initrole, flags.initrace, 0, FALSE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
            Sprintf(buf, "Threshold level: %d", roles[flags.initrole].xlev);
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
            Sprintf(buf, "Below threshold: %s per level", get_advancement_description(flags.initrole, flags.initrace, 1, FALSE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
            Sprintf(buf, "After threshold: %s per level", get_advancement_description(flags.initrole, flags.initrace, 2, FALSE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
        }
        else
        {
            Sprintf(buf, "Initial:   %s", get_advancement_description(flags.initrole, flags.initrace, 0, FALSE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
            Sprintf(buf, "Per level: %s", get_advancement_description(flags.initrole, flags.initrace, 1, FALSE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
        }

        add_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_HEADING, NO_COLOR, "Mana Advancement",
            MENU_UNSELECTED);
        if (roles[flags.initrole].xlev < MAXULEV)
        {
            Sprintf(buf, "Initial:         %s", get_advancement_description(flags.initrole, flags.initrace, 0, TRUE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
            Sprintf(buf, "Threshold level: %d", roles[flags.initrole].xlev);
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
            Sprintf(buf, "Below threshold: %s per level", get_advancement_description(flags.initrole, flags.initrace, 1, TRUE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
            Sprintf(buf, "After threshold: %s per level", get_advancement_description(flags.initrole, flags.initrace, 2, TRUE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
        }
        else
        {
            Sprintf(buf, "Initial:   %s", get_advancement_description(flags.initrole, flags.initrace, 0, TRUE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
            Sprintf(buf, "Per level: %s", get_advancement_description(flags.initrole, flags.initrace, 1, TRUE));
            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, buf,
                MENU_UNSELECTED);
        }

        /* Level-up intrinsics */
        for (i = 1; i <= 2; i++)
        {
            const struct innate* intrinsic_ability = (i == 1 ? race_abil(races[flags.initrace].monsternum) : role_abil(roles[flags.initrole].monsternum));
            if (intrinsic_ability)
            {
                int abil_count = 0;
                int table_index;
                for (table_index = 0; intrinsic_ability[table_index].ulevel > 0; table_index++)
                {
                    if (intrinsic_ability[table_index].propid > 0)
                    {
                        abil_count++;
                    }
                }
                if (abil_count > 0)
                {
                    Sprintf(buf, "Abilities from %s:", i == 1 ? "Race" : "Role");
                    add_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings | ATR_HEADING, NO_COLOR, buf,
                        MENU_UNSELECTED);
                    for (table_index = 0;intrinsic_ability[table_index].ulevel > 0; table_index++)
                    {
                        if (intrinsic_ability[table_index].propid > 0)
                        {
                            char dbuf2[BUFSZ];
                            Strcpy(dbuf2, get_property_name(intrinsic_ability[table_index].propid));
                            *dbuf2 = highc(*dbuf2);

                            Sprintf(buf, "Level %2d - %s", intrinsic_ability[table_index].ulevel, dbuf2);
                            add_menu(win, NO_GLYPH, &any, 0, 0, ATR_INDENT_AT_DASH, NO_COLOR, buf,
                                MENU_UNSELECTED);
                        }
                    }
                }
            }
        }

        if (!roles[flags.initrole].name.f
            && (roles[flags.initrole].allow & ROLE_GENDMASK)
            == (ROLE_MALE | ROLE_FEMALE))
            Sprintf(plbuf, " %s", genders[flags.initgend].adj);
        else
            *plbuf = '\0'; /* omit redundant gender */
        Sprintf(pbuf, "%s, %s%s %s %s", plname, aligns[flags.initalign].adj, plbuf,
            races[flags.initrace].adj,
            (flags.initgend == 1 && roles[flags.initrole].name.f) ? roles[flags.initrole].name.f
            : roles[flags.initrole].name.m);

        end_menu_ex(win, "Accept this character?", pbuf);
        n = select_menu(win, PICK_ONE, &selected);
        /* [pick-one menus with a preselected entry behave oddly...] */
        int choice = (n > 0) ? selected[n - 1].item.a_int : (n == 0) ? 1 : -1;
        if (selected)
            free((genericptr_t)selected), selected = 0;
        destroy_nhwindow(win);

        switch (choice) {
        default:          /* 'q' or ESC */
            goto give_up; /* quit */
            break;
        case 3: { /* 'a' */
            /*
             * TODO: what, if anything, should be done if the name is
             * changed to or from "wizard" after port-specific startup
             * code has set flags.debug based on the original name?
             */
            int saveROLE, saveRACE, saveGEND, saveALGN;

            iflags.renameinprogress = TRUE;
            /* plnamesuffix() can change any or all of ROLE, RACE,
               GEND, ALGN; we'll override that and honor only the name */
            saveROLE = flags.initrole, saveRACE = flags.initrace, saveGEND = flags.initgend,
                saveALGN = flags.initalign;
            *plname = '\0';
            plnamesuffix(); /* calls askname() when plname[] is empty */
            flags.initrole = saveROLE, flags.initrace = saveRACE, flags.initgend = saveGEND,
                flags.initalign = saveALGN;
            iflags.renameinprogress = FALSE;
            break; /* getconfirmation is still True */
        }
        case 2:    /* 'n' */
            /* start fresh, but bypass "shall I pick everything for you?"
               step; any partial role selection via config file, command
               line, or name suffix is discarded this time */
            pick4u = 'n';
            flags.initrole = flags.initrace = flags.initgend = flags.initalign = ROLE_NONE;
            goto back_from_race;
            break;
        case 1: /* 'y' or Space or Return/Enter */
            /* success; drop out through end of function */
            getconfirmation = FALSE;
            break;
        }
    }
    /* Success! */
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
    otypdata.nhcolor = objects[otmp->otyp].oc_color;

    otypdata.lamplit = otmp->lamplit;
    otypdata.poisoned = otmp->opoisoned;
    otypdata.eroded = otmp->oeroded;
    otypdata.eroded2 = otmp->oeroded2;

    otypdata.corrodeable = is_corrodeable(otmp);
    otypdata.rottable = is_rottable(otmp);
    otypdata.flammable = is_flammable(otmp);
    otypdata.rustprone = is_rustprone(otmp);
    otypdata.poisonable = is_poisonable(otmp);

    if (otmp == uchain || otmp == uball)
    {
        otypdata.is_uchain = otmp == uchain;
        otypdata.is_uball = otmp == uball;

        xchar ox = 0, oy = 0;
        (void)get_obj_location(otmp, &ox, &oy, BURIED_TOO);
        otypdata.obj_loc_x = ox;
        otypdata.obj_loc_y = oy;
    }

    return otypdata;
}
