/* GnollHack 4.1.0 npc.c */
/* Copyright (c) Janne Gustafsson, 2020. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_DCL boolean FDECL(his_npc_room_at, (struct monst*, XCHAR_P, XCHAR_P));
STATIC_DCL boolean FDECL(has_npc_room, (struct monst*));


struct npc_subtype_definition npc_subtype_definitions[MAX_NPC_SUBTYPES] = 
{
    {
        PM_ARCHMAGE,
        NPC_GEHENNOM_UNDEAD_SPELLCASTER,
        "artificer",
        "laboratory",
        (char*)0,
        (char*)0,
        (char*)0,
        {(char*)0, (char*)0, (char*)0},
        8, 0,
        5, 1000, 7500,
        NPC_SERVICE_ENCHANT_ACCESSORY | NPC_SERVICE_RECHARGING | NPC_SERVICE_BLESSED_RECHARGING | NPC_SERVICE_IDENTIFY_ACCESSORIES_AND_CHARGED_ITEMS | NPC_SERVICE_BUY_SPELLBOOKS | NPC_SERVICE_TEACH_SPELL_CONE_OF_COLD | NPC_SERVICE_TEACH_SPELL_LIGHTNING_BOLT | NPC_SERVICE_TEACH_SPELL_FORCE_BOLT | NPC_SERVICE_TEACH_SPECIAL_SPELLS | NPC_SERVICE_TEACH_RANDOM_ARCANE_SPELLS,
        NPC_FLAGS_PARQUET_FLOOR | NPC_FLAGS_DOORS_CLOSED | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS
    },
    {
        PM_DWARVEN_GEOLOGIST,
        NPC_GEHENNOM_DWARF_MUMMY,
        "geologist",
        "workshop",
        (char*)0,
        (char*)0,
        (char*)0,
        {(char*)0, (char*)0, (char*)0},
        6, 0,
        10, 1000, 15000,
        NPC_SERVICE_BUY_GEMS_AND_STONES | NPC_SERVICE_IDENTIFY_GEMS_AND_STONES | NPC_SERVICE_FORGE_SLING_BULLETS,
        NPC_FLAGS_PARQUET_FLOOR | NPC_FLAGS_DOORS_CLOSED | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_DOUBLE_MONEY_IN_HELL | NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS
    },
    {
        PM_MODRON_QUARTON,
        NPC_GEHENNOM_STANDARD,
        "warp engineer",
        "engineering bay",
        (char*)0,
        (char*)0,
        (char*)0,
        {(char*)0, (char*)0, (char*)0},
        0, 0,
        10, 1000, 30000,
        NPC_SERVICE_BUY_DILITHIUM_CRYSTALS | NPC_SERVICE_IDENTIFY_GEMS_STONES_AND_CHARGED_ITEMS | NPC_SERVICE_RECHARGING | NPC_SERVICE_BLESSED_RECHARGING | NPC_SERVICE_BRANCH_PORTAL,
        NPC_FLAGS_NO_GENERATION | NPC_FLAGS_DOORS_CLOSED | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS
    },
    {
        PM_HERMIT,
        NPC_GEHENNOM_STANDARD,
        "hermit",
        "entrance to the Dungeons of Doom",
        "Durward Cos",
        "I have watched over this entrance to the Dungeons of Doom for many decades now.",
        "Welcome back to the world of the living.",
        {(char*)0, (char*)0, (char*)0},
        0, 0,
        1, 100, 5,
        NPC_SERVICE_GIVE_STARTING_QUESTS,
        NPC_FLAGS_NO_GENERATION | NPC_FLAGS_DISPLAY_NAME_ONLY | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_NO_MY | NPC_FLAGS_NO_ADVICE | NPC_FLAGS_NO_ITEMS | NPC_FLAGS_COMMENTS_ON_REVIVAL | NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS
    },
    {
        PM_HERMIT,
        NPC_GEHENNOM_STANDARD,
        "hermit",
        "simple abode",
        "Eduard Dunley",
        "Long ago, before this place was overrun by evil, I looked after the remains of the dead kings of Yendor.",
        "Welcome back to the world of the living.",
        {(char*)0, (char*)0, (char*)0},
        0, 0,
        1, 100, 5,
        NPC_SERVICE_GIVE_ADVANCED_QUESTS,
        NPC_FLAGS_NO_GENERATION | NPC_FLAGS_DISPLAY_NAME_ONLY | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_NO_ADVICE | NPC_FLAGS_NO_ITEMS | NPC_FLAGS_COMMENTS_ON_REVIVAL | NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS
    }, 
    {
        PM_HERMIT,
        NPC_GEHENNOM_STANDARD,
        "hermit",
        "simple home",
        "Herbert Reed",
        "I study the local gnomes and their ways.",
        "Welcome back to the world of the living.",
        {(char*)0, (char*)0, (char*)0},
        0, 0,
        1, 100, 5,
        NPC_SERVICE_GIVE_GNOMISH_QUESTS,
        NPC_FLAGS_NO_GENERATION | NPC_FLAGS_DISPLAY_NAME_ONLY | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_NO_ADVICE | NPC_FLAGS_NO_ITEMS | NPC_FLAGS_COMMENTS_ON_REVIVAL | NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS
    },
    {
        PM_ORC_SHAMAN,
        NPC_GEHENNOM_STANDARD,
        "orcish hermit",
        "lounge",
        "Haiuun",
        "I'm the seer that led our clan to this place.",
        "Ah, your spirit has returned.",
        {(char*)0, (char*)0, (char*)0},
        0, 0,
        1, 100, 5,
        NPC_SERVICE_GIVE_ORCISH_QUESTS | NPC_SERVICE_TEACH_RANDOM_ARCANE_SPELLS,
        NPC_FLAGS_NO_GENERATION | NPC_FLAGS_DISPLAY_NAME_ONLY | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_NO_ADVICE | NPC_FLAGS_NO_ITEMS | NPC_FLAGS_COMMENTS_ON_REVIVAL | NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS
    },
    {
        PM_ELDER_QUANTUM_MECHANIC,
        NPC_GEHENNOM_UNDEAD_SPELLCASTER,
        "Professor of Quantum Mechanics at the University of Yendor",
        "study",
        "Endicott Whateley",
        "We conduct delicate underground experiments within these mines.",
        (char*)0,
        {(char*)0, (char*)0, (char*)0},
        0, 0,
        5, 50, 500,
        NPC_SERVICE_GIVE_QUANTUM_QUESTS | NPC_SERVICE_SPECIAL_NPC_HINTS,
        NPC_FLAGS_NO_GENERATION | NPC_FLAGS_MALE | NPC_FLAGS_DISPLAY_NAME_ONLY | NPC_FLAGS_DOORS_CLOSED | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_NO_ADVICE | NPC_FLAGS_NO_TITLE_ARTICLE | NPC_FLAGS_MAJORITY_NORMAL_HELLO | NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS
    },
    {
        PM_ELVEN_BARD,
        NPC_GEHENNOM_UNDEAD_SPELLCASTER,
        "elven bard",
        "practice hall",
        "Lindisse",
        "I sing old elven songs to aid adventurers in these dark times.",
        (char*)0,
        {"somebody singing.", "somebody praising elven gods.", "somebody playing a musical instrument."},
        0, 0,
        5, 10, 50,
        NPC_SERVICE_SPECIAL_NPC_HINTS | NPC_SERVICE_SING_SONGS,
        NPC_FLAGS_NO_GENERATION | NPC_FLAGS_FEMALE | NPC_FLAGS_DISPLAY_NAME_ONLY | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_NO_ADVICE | NPC_FLAGS_HAS_ALL_DISTANT_SOUNDS
    },
};

schar
min_npc_appearance_depth()
{
    schar minlvl = -1;
    int i;
    for (i = 0; i < MAX_NPC_SUBTYPES; i++)
    {
        if (!(npc_subtype_definitions[i].general_flags & NPC_FLAGS_NO_GENERATION) && (minlvl < 0 || npc_subtype_definitions[i].min_appearance_depth < minlvl))
            minlvl = npc_subtype_definitions[i].min_appearance_depth;
    }
    return (int)minlvl;
}

void
forget_npc_entry(npc)
struct monst* npc;
{
    struct enpc* enpc_p = npc->isnpc ? ENPC(npc) : 0;

    if (!enpc_p) {
        impossible("attempting to manipulate NPC data for non-NPC?");
        return;
    }
    enpc_p->intone_time = enpc_p->enter_time = enpc_p->peaceful_time =
        enpc_p->hostile_time = 0L;
}

/*
 * npc_move: return 1: moved  0: didn't  -1: let m_move do it  -2: died
 */
int npc_move(npc)
register struct monst* npc;
{
    register xchar gx, gy, omx, omy;
    schar npc_room;
    boolean avoid = TRUE;

    omx = npc->mx;
    omy = npc->my;

    if (!his_npc_room_at(npc, omx, omy))
        return -1;

    npc_room = ENPC(npc)->npc_room;

    if (rn2(3))
    {
        gx = omx;
        gy = omy;
    }
    else
    {
        gx = somex(&rooms[npc_room - ROOMOFFSET]);
        gy = somey(&rooms[npc_room - ROOMOFFSET]);
    }

    if (!is_peaceful(npc) || is_crazed(npc)
        || (Conflict && !check_ability_resistance_success(npc, A_WIS, 0)))
    {
        if (monnear(npc, u.ux, u.uy))
        {
            if (Displaced)
                Your("displaced image doesn't fool %s!", mon_nam(npc));
            (void)mattacku(npc);
            return 0;
        }
        else if (index(u.urooms, npc_room))
        {
            /* chase player if inside temple & can see him */
            if (!is_blinded(npc) && m_canseeu(npc))
            {
                gx = u.ux;
                gy = u.uy;
            }
            avoid = FALSE;
        }
    }
    else if (Invis)
        avoid = FALSE;

    return move_special(npc, FALSE, TRUE, FALSE, avoid, omx, omy, gx, gy);
}


STATIC_OVL boolean
has_npc_room(npc)
struct monst* npc;
{
    struct enpc* enpc_p;

    if (!npc || !npc->isnpc)
        return FALSE;

    enpc_p = ENPC(npc);

    if (enpc_p->npc_room == 0)
        return FALSE;

    /* Extra checks whether the room is still ok (for other types, e.g., has altar or anvil) */

    return TRUE;
}

STATIC_OVL boolean
his_npc_room_at(npc, x, y)
register struct monst* npc;
register xchar x, y;
{
    return (boolean)(npc && npc->isnpc
        && (ENPC(npc)->npc_room == *in_rooms(x, y, NPCROOM))
        && on_level(&(ENPC(npc)->npc_room_level), &u.uz));
}

boolean
in_his_npc_room(npc)
struct monst* npc;
{
    /* make sure we have an npc */
    if (!npc || !npc->isnpc)
        return FALSE;

    /* npc must be on right level and in right room */
    if (!his_npc_room_at(npc, npc->mx, npc->my))
        return FALSE;

    /* room must still contain [ ] */
    return has_npc_room(npc);
}

struct monst*
findnpc(roomno)
char roomno;
{
    register struct monst* mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->isnpc && (ENPC(mtmp)->npc_room == roomno)
            && his_npc_room_at(mtmp, mtmp->mx, mtmp->my))
            return mtmp;
    }
    return (struct monst*)0;
}

/* called from check_special_room() when the player enters the residence */
void
in_npc_room(roomno)
int roomno;
{
    struct monst* npc;
    struct enpc* enpc_p;
    boolean has_room, can_speak;
    char buf[BUFSZ];
    int npctype = 0;

    /* don't do anything if hero is already in the room */
    if (npc_room_occupied(u.urooms0))
        return;

    if ((npc = findnpc((char)roomno)) != 0)
    {
        /* tended */
        if (!has_enpc(npc))
            return;

        enpc_p = ENPC(npc);
        has_room = has_npc_room(npc);
        can_speak = (mon_can_move(npc));
        npctype = enpc_p->npc_typ;

        if (can_speak && !Deaf)
        {
            if (moves >= enpc_p->intone_time && moves >= enpc_p->enter_time)
            {
                pline("%s says:",
                    canseemon(npc) ? Monnam(npc) : "A nearby voice");
                enpc_p->intone_time = moves + (long)d(10, 500); /* ~2505 */
                enpc_p->enter_time = 0L;
            }

            if (context.reviving && (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_COMMENTS_ON_REVIVAL))
            {
                int dialogueline = NPC_LINE_REVIVAL;
                context.global_minimum_volume = 0.5;
                play_monster_special_dialogue_line(npc, dialogueline);
                context.global_minimum_volume = 0.0;
                Sprintf(buf, "%s", npc_subtype_definitions[npctype].revival_line);
                verbalize1(buf);
            }
            else if (moves >= enpc_p->enter_time)
            {
                int dialogueline = !has_room ? NPC_LINE_ADVENTURER_WELCOME : NPC_LINE_ADVENTURER_WELCOME_TO_MY_RESIDENCE;
                context.global_minimum_volume = 0.5;
                play_monster_special_dialogue_line(npc, dialogueline);
                context.global_minimum_volume = 0.0;

                if (has_room)
                    Sprintf(buf, "Adventurer, %s %s %s!", "welcome to", (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_NO_MY) ? "the" : "my", npc_subtype_definitions[enpc_p->npc_typ].room_name);
                else
                    Sprintf(buf, "Adventurer, welcome!");

                verbalize1(buf);
                enpc_p->enter_time = moves + (long)d(5, 50); /* ~125 */
            }
        }

    }
    else
    {
        /* untended */
        switch (rn2(4))
        {
        case 0:
            You("have an oddly wishful feeling...");
            break;
        case 1:
            You_feel("something is out of place.");
            break;
        case 2:
            You_feel("strangely worried.");
            break;
        default:
            break;
        }
    }
}

char
npc_room_occupied(array)
register char* array;
{
    register char* ptr;

    for (ptr = array; *ptr; ptr++)
        if (rooms[*ptr - ROOMOFFSET].rtype == NPCROOM)
            return *ptr;
    return '\0';
}

/* exclusively for mknpcroom() */
void
npcini(lvl, sroom, sx, sy, npctype, mtype)
d_level* lvl;
struct mkroom* sroom;
int sx, sy;
uchar npctype;
int mtype;
{
    struct monst* npc;
    int i;
    int npc_loc_x = sx;
    int npc_loc_y = sy;
    if (!isok(npc_loc_x, npc_loc_y) || IS_ROCK(levl[npc_loc_x][npc_loc_y].typ))
    {
        npc_loc_x = sx + 1;
        npc_loc_y = sy;
    }
    if (!isok(npc_loc_x, npc_loc_y) || IS_ROCK(levl[npc_loc_x][npc_loc_y].typ))
    {
        npc_loc_x = sx - 1;
        npc_loc_y = sy;
    }
    if (!isok(npc_loc_x, npc_loc_y) || IS_ROCK(levl[npc_loc_x][npc_loc_y].typ))
    {
        npc_loc_x = sx;
        npc_loc_y = sy - 1;
    }
    if (!isok(npc_loc_x, npc_loc_y) || IS_ROCK(levl[npc_loc_x][npc_loc_y].typ))
    {
        npc_loc_x = sx;
        npc_loc_y = sy + 1;
    }

    if (MON_AT(npc_loc_x, npc_loc_y))
        (void)rloc(m_at(npc_loc_x, npc_loc_y), FALSE); /* insurance */

    int npc_montype = npc_subtype_definitions[npctype].mnum;
    if (mtype > NON_PM && mtype < NUM_MONSTERS && !(mvitals[mtype].mvflags & MV_GONE))
    {
        npc_montype = mtype;
    }
    else if (sroom->resident_mtype > 0 && sroom->resident_mtype < NUM_MONSTERS && !(mvitals[sroom->resident_mtype].mvflags & MV_GONE))
    {
        npc_montype = sroom->resident_mtype;
    }
    else
    {
        if (Inhell)
        {
            switch (npc_subtype_definitions[npctype].npc_gehennom_type)
            {
            case NPC_GEHENNOM_STANDARD:
                break;
            case NPC_GEHENNOM_UNDEAD_SPELLCASTER:
                npc_montype = get_gehennom_undead_spellcaster(npc_montype);
                break;
            case NPC_GEHENNOM_DWARF_MUMMY:
                if (!(mvitals[PM_GREATER_DWARVEN_MUMMY_GEOLOGIST].mvflags & MV_GONE))
                    npc_montype = PM_GREATER_DWARVEN_MUMMY_GEOLOGIST;
                else if (!(mvitals[PM_DWARF_MUMMY].mvflags & MV_GONE))
                    npc_montype = PM_DWARF_MUMMY;
                else if (!(mvitals[PM_DWARF_ZOMBIE].mvflags & MV_GONE))
                    npc_montype = PM_DWARF_ZOMBIE;
                else if (!(mvitals[PM_GHOST].mvflags & MV_GONE))
                    npc_montype = PM_GHOST;
                else if (!(mvitals[PM_PIT_FIEND].mvflags & MV_GONE))
                    npc_montype = PM_PIT_FIEND;
                break;
            default:
                break;
            }
        }
    }

    unsigned long extraflags = (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_FEMALE) != 0  ? MM_FEMALE : Inhell || (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_MALE) != 0 ? MM_MALE : 0UL; /* Since there is only one soundset for unusual creature types */

    npc = makemon_ex(&mons[npc_montype], npc_loc_x, npc_loc_y, MM_ENPC | extraflags, npctype, 0);
    if(!npc)
        npc = makemon_ex(&mons[npc_subtype_definitions[npctype].mnum], npc_loc_x, npc_loc_y, MM_ENPC | extraflags, npctype, 0); /* Fallback */
    
    if (npc)
    {
        ENPC(npc)->npc_room = (schar)((sroom - rooms) + ROOMOFFSET);
        ENPC(npc)->npc_typ = npctype;
        assign_level(&(ENPC(npc)->npc_room_level), lvl);
        npc->mtrapseen = ~0; /* traps are known */
        npc->mpeaceful = 1;
        npc->isnpc = 1;
        npc->msleeping = 0;
        set_mhostility(npc); /* mpeaceful may have changed */

        if ((npc_subtype_definitions[npctype].start_money_d > 0 && npc_subtype_definitions[npctype].start_money_n > 0) || npc_subtype_definitions[npctype].start_money_p > 0)
        {
            long npcmoney = d(npc_subtype_definitions[npctype].start_money_d, npc_subtype_definitions[npctype].start_money_n) + npc_subtype_definitions[npctype].start_money_p;
            
            if (Inhell && (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_DOUBLE_MONEY_IN_HELL))
                npcmoney *= 2;

            (void)mongetsgold(npc, npcmoney);
        }

        switch (npctype)
        {
        case NPC_GEOLOGIST:
        {
            /* High chance of at least one ore of each type */
            for (i = NUGGET_OF_IRON_ORE; i <= NUGGET_OF_MITHRIL_ORE; i++)
            {
                if (rn2(9))
                    mongets(npc, i);
            }

            /* Random ores */
            for (i = 0; i < 24; i++)
            {
                mongets(npc, rnd_class(NUGGET_OF_IRON_ORE, NUGGET_OF_MITHRIL_ORE));
            }

            /* Random gems */
            for (i = 0; i < 24; i++)
            {
                mongets(npc, rnd_class(FIRST_GEM, LAST_GEM));
            }

            /* Sling bullets */
            mongets(npc, LEADEN_SLING_BULLET);
            mongets(npc, IRON_SLING_BULLET);
            for (i = 0; i < 3; i++)
            {
                if(!rn2(2))
                    mongets(npc, rnd_class(LEADEN_SLING_BULLET, SILVER_SLING_BULLET));
            }
            break;
        }
        case NPC_QUANTUM_MECHANIC:
        {
            int cnt = rnd(3);
            for (i = 0; i < cnt; i++)
            {
                mongets(npc, WAN_TOWN_PORTAL);
            }

            cnt = rnd(3);
            for (i = 0; i < cnt; i++)
            {
                mongets(npc, WAN_TELEPORTATION);
            }
            mongets(npc, CUBIC_GATE);
            if (!rn2(2))
                mongets(npc, SPE_TELEPORT_MONSTER);
            if (!rn2(2))
                mongets(npc, SPE_TELEPORT_SELF);
            if (!rn2(3))
                mongets(npc, SPE_CIRCLE_OF_TELEPORTATION);
            if (!rn2(4))
                mongets(npc, SPE_LEVEL_TELEPORT);
            if (!rn2(5))
                mongets(npc, SPE_PORTAL);
            if (!rn2(7))
                mongets(npc, SPE_DISINTEGRATE);
            if (!rn2(9))
                mongets(npc, SPE_SPHERE_OF_ANNIHILATION);
            if (!rn2(11))
                mongets(npc, SPE_TIME_STOP);
            if (!rn2(13))
                mongets(npc, SPE_BLACK_BLADE_OF_DISASTER);
            break;
        }
        case NPC_ELVEN_BARD:
        {
            int roll = rnd(3);
            if (roll & 1)
                mongets(npc, !rn2(10) ? MAGIC_HARP : WOODEN_HARP);
            if (roll & 2)
                mongets(npc, !rn2(10) ? MAGIC_FLUTE : WOODEN_FLUTE);
            mongets(npc, ELVEN_WAYBREAD);
            break;
        }
        case NPC_HERMIT:
        {
            int cnt = 1 + rnd(2);
            for (i = 0; i < cnt; i++)
            {
                mongets(npc, SPE_MANUAL);
            }
            break;
        }
        case NPC_HERMIT2:
        {
            struct obj* otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, MANUAL_GURATHULS_GUIDE_TO_ASCENSION, MKOBJ_FLAGS_PARAM_IS_TITLE);
            if (otmp)
                (void)mpickobj(npc, otmp);

            int cnt = 1 + rnd(2);
            for (i = 0; i < cnt; i++)
            {
                mongets(npc, SPE_MANUAL);
            }

            mongets(npc, RIN_LEVITATION);
            mongets(npc, RIN_FIRE_RESISTANCE);
            mongets(npc, RIN_PROTECTION_FROM_UNDEATH);
            mongets(npc, WAN_UNDEAD_TURNING);
            break;
        }
        case NPC_ORC_HERMIT3:
        {
            struct obj* otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, MANUAL_ADVANCED_READING_IN_KNOWN_MONSTERS, MKOBJ_FLAGS_PARAM_IS_TITLE);
            if (otmp)
                (void)mpickobj(npc, otmp);

            int cnt = 1 + rnd(2);
            for (i = 0; i < cnt; i++)
            {
                mongets(npc, SPE_MANUAL);
            }

            break;
        }
        case NPC_HERMIT3:
        {
            struct obj* otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, MANUAL_ADVANCED_READING_IN_KNOWN_MONSTERS, MKOBJ_FLAGS_PARAM_IS_TITLE);
            if (otmp)
                (void)mpickobj(npc, otmp);

            int cnt = 1 + rnd(2);
            for (i = 0; i < cnt; i++)
            {
                mongets(npc, SPE_MANUAL);
            }

            mongets(npc, GNOMISH_FELT_HAT);
            cnt = 2 + rnd(2);
            for (i = 0; i < cnt; i++)
            {
                otmp = mksobj_with_flags(GNOMISH_FELT_HAT, TRUE, FALSE, FALSE, 0, MKOBJ_FLAGS_FORCE_MYTHIC_OR_LEGENDARY);
                if (otmp)
                    (void)mpickobj(npc, otmp);
            }

            break;
        }
        case NPC_ARTIFICER:
        {
            struct obj* otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, depth(&u.uz) > depth(&medusa_level) ? MANUAL_SECRETS_OF_SCARE_MONSTER : MANUAL_MASTER_CLASS_IN_WANDS, MKOBJ_FLAGS_PARAM_IS_TITLE);
            if (otmp)
                (void)mpickobj(npc, otmp);

            otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, MANUAL_PRINCIPLES_OF_MAGIC, MKOBJ_FLAGS_PARAM_IS_TITLE);
            if (otmp)
                (void)mpickobj(npc, otmp);

            mongets(npc, WAN_TOWN_PORTAL);
            otmp = mkobj(WAND_CLASS, TRUE, TRUE);
            if (otmp)
                (void)mpickobj(npc, otmp);
            if (!rn2(2))
            {
                otmp = mkobj(WAND_CLASS, TRUE, TRUE);
                if (otmp)
                    (void)mpickobj(npc, otmp);
            }
            if (!rn2(2))
            {
                otmp = mkobj(WAND_CLASS, TRUE, TRUE);
                if (otmp)
                    (void)mpickobj(npc, otmp);
            }
            break;
        }
        default:
            break;
        }


        if (npc_subtype_definitions[npctype].service_flags & NPC_SERVICE_TEACH_RANDOM_ARCANE_SPELLS)
        {
            int cnt = 0;

            if (level_difficulty() < 15)
            {
                ENPC(npc)->special_teach_spells[cnt] = SPE_MAGIC_ARROW;
                cnt++;
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_SHOCKING_TOUCH;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_MAGIC_MISSILE;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_CIRCLE_OF_FIRE;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_CIRCLE_OF_MAGIC;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_CIRCLE_OF_FROST;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_CIRCLE_OF_LIGHTNING;
                    cnt++;
                }
                if ((!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS) || cnt == 0)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_FIRE_BOLT;
                    cnt++;
                }
            }
            else if (Inhell)
            {
                if (!rn2(2) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = rn2(2) ? SPE_INCINERATE : SPE_FLAME_BURST;
                    cnt++;
                }
                if (!rn2(2) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = rn2(2) ? SPE_GLACIAL_GRASP : SPE_GREATER_FROST_RAY;
                    cnt++;
                }
                if (!rn2(2) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_GREATER_ANIMATE_FIRE;
                    cnt++;
                }
                if (!rn2(2) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_SUMMON_DEMON;
                    cnt++;
                }
                if (!rn2(2) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = rn2(2) ? SPE_FINGER_OF_DEATH : SPE_DEATHSPELL;
                    cnt++;
                }
                if ((!rn2(2) && cnt < MAX_SPECIAL_TEACH_SPELLS) || cnt == 0)
                {
                    ENPC(npc)->special_teach_spells[cnt] = rn2(2) ? SPE_RAISE_SKELETON_KING : SPE_RAISE_SKELETON_LORD;
                    cnt++;
                }
                if (!rn2(6) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_CREATE_DRACOLICH;
                    cnt++;
                }
                if (!rn2(6) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_CREATE_ELDER_DRACOLICH;
                    cnt++;
                }
            }
            else
            {
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_FORCE_STRIKE;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_GREATER_MAGIC_MISSILE;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_FIRE_STORM;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_THUNDERSTORM;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_ICE_STORM;
                    cnt++;
                }
                if (!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_MAGIC_STORM;
                    cnt++;
                }
                if ((!rn2(3) && cnt < MAX_SPECIAL_TEACH_SPELLS) || cnt == 0)
                {
                    ENPC(npc)->special_teach_spells[cnt] = SPE_METEOR_SWARM;
                    cnt++;
                }
            }
        }
    }
}

void
newenpc(mtmp)
struct monst* mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();
    if (!ENPC(mtmp)) {
        ENPC(mtmp) = (struct enpc*)alloc(sizeof(struct enpc));
        (void)memset((genericptr_t)ENPC(mtmp), 0, sizeof(struct enpc));
    }
}

void
free_enpc(mtmp)
struct monst* mtmp;
{
    if (mtmp->mextra && ENPC(mtmp))
    {
        free((genericptr_t)ENPC(mtmp));
        ENPC(mtmp) = (struct enpc*)0;
    }
    mtmp->isnpc = 0;
}

/* munge NPC-specific structure when restoring -dlc */
void
restnpc(mtmp, ghostly)
register struct monst* mtmp;
boolean ghostly;
{
    if (u.uz.dlevel) {
        if (ghostly)
            assign_level(&(ENPC(mtmp)->npc_room_level), &u.uz);
    }
}

void
clearnpcs()
{
    struct monst* mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->isnpc && !on_level(&(ENPC(mtmp)->npc_room_level), &u.uz))
            mongone(mtmp);
    }
}


/* npc.c */