/*
 * npc.c
 * Copyright (c) Janne Gustafsson, 2020
 */

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
        8, 0,
        5, 1000, 7500,
        NPC_SERVICE_ENCHANT_ACCESSORY | NPC_SERVICE_RECHARGING | NPC_SERVICE_BLESSED_RECHARGING | NPC_SERVICE_IDENTIFY_ACCESSORIES_AND_CHARGED_ITEMS | NPC_SERVICE_BUY_SPELLBOOKS,
        NPC_FLAGS_PARQUET_FLOOR | NPC_FLAGS_DOORS_CLOSED | NPC_FLAGS_LIGHTS_ON
    },
    {
        PM_DWARVEN_GEOLOGIST,
        NPC_GEHENNOM_DWARF_MUMMY,
        "geologist",
        "workshop",
        6, 0,
        10, 1000, 15000,
        NPC_SERVICE_BUY_GEMS_AND_STONES | NPC_SERVICE_IDENTIFY_GEMS_AND_STONES,
        NPC_FLAGS_PARQUET_FLOOR | NPC_FLAGS_DOORS_CLOSED | NPC_FLAGS_LIGHTS_ON | NPC_FLAGS_DOUBLE_MONEY_IN_HELL
    },
    {
        PM_MODRON_QUARTON,
        NPC_GEHENNOM_STANDARD,
        "warp engineer",
        "engineering bay",
        99, 0,
        10, 1000, 30000,
        NPC_SERVICE_BUY_DILITHIUM_CRYSTALS | NPC_SERVICE_IDENTIFY_GEMS_STONES_AND_CHARGED_ITEMS | NPC_SERVICE_RECHARGING | NPC_SERVICE_BLESSED_RECHARGING | NPC_SERVICE_BRANCH_PORTAL,
        NPC_FLAGS_DOORS_CLOSED | NPC_FLAGS_LIGHTS_ON
    },
};

schar
min_npc_appearance_depth()
{
    schar minlvl = 0;
    for (int i = 0; i < MAX_NPC_SUBTYPES; i++)
    {
        if (npc_subtype_definitions[i].min_appearance_depth > minlvl)
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
    const char* msg1, * msg2;
    char buf[BUFSZ];

    /* don't do anything if hero is already in the room */
    if (npc_room_occupied(u.urooms0))
        return;

    if ((npc = findnpc((char)roomno)) != 0)
    {
        /* tended */

        enpc_p = ENPC(npc);
        has_room = has_npc_room(npc);
        can_speak = (mon_can_move(npc));
        if (can_speak && !Deaf && moves >= enpc_p->intone_time)
        {
            pline("%s says:",
                canseemon(npc) ? Monnam(npc) : "A nearby voice");
            enpc_p->intone_time = moves + (long)d(10, 500); /* ~2505 */
            enpc_p->enter_time = 0L;
        }
        msg1 = msg2 = 0;

        if (moves >= enpc_p->enter_time)
        {
            if(has_room)
                Sprintf(buf, "Adventurer, %s %s!", "welcome to my", npc_subtype_definitions[enpc_p->npc_typ].room_name);
            else
                Sprintf(buf, "Adventurer, welcome!");

            msg1 = buf;
        }

        if (msg1 && can_speak && !Deaf)
        {
            verbalize1(msg1);
            if (msg2)
                verbalize1(msg2);
            enpc_p->enter_time = moves + (long)d(10, 100); /* ~505 */
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
int npctype;
int mtype;
{
    struct monst* npc;
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
    if (mtype > NON_PM && mtype < NUM_MONSTERS && !(mvitals[mtype].mvflags & G_GONE))
    {
        npc_montype = mtype;
    }
    else if (sroom->resident_mtype > 0 && sroom->resident_mtype < NUM_MONSTERS && !(mvitals[sroom->resident_mtype].mvflags & G_GONE))
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
                if (!(mvitals[PM_GREATER_DWARVEN_MUMMY_GEOLOGIST].mvflags & G_GONE))
                    npc_montype = PM_GREATER_DWARVEN_MUMMY_GEOLOGIST;
                else if (!(mvitals[PM_DWARF_MUMMY].mvflags & G_GONE))
                    npc_montype = PM_DWARF_MUMMY;
                else if (!(mvitals[PM_DWARF_ZOMBIE].mvflags & G_GONE))
                    npc_montype = PM_DWARF_ZOMBIE;
                else if (!(mvitals[PM_GHOST].mvflags & G_GONE))
                    npc_montype = PM_GHOST;
                else if (!(mvitals[PM_PIT_FIEND].mvflags & G_GONE))
                    npc_montype = PM_PIT_FIEND;
                break;
            default:
                break;
            }
        }
    }

    npc = makemon(&mons[npc_montype], npc_loc_x, npc_loc_y, MM_ENPC);
    if(!npc)
        npc = makemon(&mons[npc_subtype_definitions[npctype].mnum], npc_loc_x, npc_loc_y, MM_ENPC); /* Fallback */
    
    if (npc)
    {
        ENPC(npc)->npc_room = (schar)((sroom - rooms) + ROOMOFFSET);
        ENPC(npc)->npc_typ = npctype;
        assign_level(&(ENPC(npc)->npc_room_level), lvl);
        npc->mtrapseen = ~0; /* traps are known */
        npc->mpeaceful = 1;
        npc->isnpc = 1;
        npc->msleeping = 0;
        set_malign(npc); /* mpeaceful may have changed */

        if ((npc_subtype_definitions[npctype].start_money_d > 0 && npc_subtype_definitions[npctype].start_money_n > 0) || npc_subtype_definitions[npctype].start_money_p > 0)
        {
            long npcmoney = d(npc_subtype_definitions[npctype].start_money_d, npc_subtype_definitions[npctype].start_money_n) + npc_subtype_definitions[npctype].start_money_p;
            
            if (Inhell && (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_DOUBLE_MONEY_IN_HELL))
                npcmoney *= 2;

            (void)mongetsgold(npc, npcmoney);
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