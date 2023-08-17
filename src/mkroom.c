/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-07-16 */

/* GnollHack 4.0    mkroom.c    $NHDT-Date: 1446887530 2015/11/07 09:12:10 $  $NHDT-Branch: master $:$NHDT-Revision: 1.24 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * Entry points:
 *      make_room() -- make and stock a room of a given type
 *      nexttodoor() -- return TRUE if adjacent to a door
 *      has_dnstairs() -- return TRUE if given room has a down staircase
 *      has_upstairs() -- return TRUE if given room has an up staircase
 *      courtmon() -- generate a court monster
 *      save_rooms() -- save rooms into file fd
 *      rest_rooms() -- restore rooms from file fd
 *      cmap_to_type() -- convert S_xxx symbol to XXX topology code
 */

#include "hack.h"

NEARDATA struct room_definition room_definitions[NUM_ROOM_TYPES] =
{
    { "ordinary room" , FALSE, 0 },
    { "" , FALSE, 0 },
    { "court", TRUE, CMAP_OPULENT },
    { "swamp", TRUE, CMAP_UNDEAD },
    { "vault", TRUE, CMAP_REINFORCED },
    { "beehive", TRUE, CMAP_NEST },
    { "dragon lair", TRUE, CMAP_NEST },
    { "library", TRUE, CMAP_CITYSCAPE },
    { "garden", TRUE, CMAP_GARDEN },
    { "morgue", TRUE, CMAP_UNDEAD },
    { "barracks", TRUE, CMAP_REINFORCED },
    { "armory", TRUE, CMAP_REINFORCED },
    { "zoo", TRUE, CMAP_NEST },
    { "Delphi", TRUE, CMAP_GARDEN },
    { "temple", TRUE, CMAP_TEMPLE },
    { "leprechaun hall", TRUE, CMAP_CITYSCAPE },
    { "cockatrice nest", TRUE, CMAP_NEST },
    { "anthole", TRUE, CMAP_GNOMISH_MINES },
    { "smithy", TRUE, CMAP_REINFORCED },
    { "residence", TRUE, CMAP_CITYSCAPE },
    { "deserted shop", TRUE, CMAP_CITYSCAPE },
    { "general store", TRUE, CMAP_CITYSCAPE },
    { "armor shop", TRUE, CMAP_CITYSCAPE },
    { "scroll shop", TRUE, CMAP_CITYSCAPE },
    { "potion shop", TRUE, CMAP_CITYSCAPE },
    { "weapon shop", TRUE, CMAP_CITYSCAPE },
    { "food shop", TRUE, CMAP_CITYSCAPE },
    { "ring shop", TRUE, CMAP_CITYSCAPE },
    { "wand shop", TRUE, CMAP_CITYSCAPE },
    { "tool shop", TRUE, CMAP_CITYSCAPE },
    { "book shop", TRUE, CMAP_CITYSCAPE },
    { "reagent shop", TRUE, CMAP_CITYSCAPE },
    { "modron shop", TRUE, CMAP_MODRON },
    { "fodder shop", TRUE, CMAP_CITYSCAPE },
    { "candle shop", TRUE, CMAP_CITYSCAPE },
};

STATIC_DCL boolean FDECL(isbig, (struct mkroom *));
STATIC_DCL struct mkroom *FDECL(pick_room, (BOOLEAN_P));
STATIC_DCL int NDECL(mkshop), NDECL(mkdesertedshop);
STATIC_DCL int FDECL(mkzoo, (int)), NDECL(mkswamp), NDECL(mkgarden), NDECL(mkdragonlair);
STATIC_DCL int NDECL(mktemple);
STATIC_DCL coord* FDECL(shrine_pos, (int));
STATIC_DCL int NDECL(mksmithy);
STATIC_DCL coord* FDECL(anvil_pos, (int));
STATIC_DCL struct permonst *NDECL(morguemon);
STATIC_DCL struct permonst *FDECL(librarymon, (int));
STATIC_DCL struct permonst *NDECL(squadmon);
STATIC_DCL struct permonst* FDECL(armorymon, (BOOLEAN_P));
STATIC_DCL void FDECL(save_room, (int, struct mkroom *));
STATIC_DCL void FDECL(rest_room, (int, struct mkroom *));
STATIC_DCL void FDECL(reset_room, (struct mkroom*));

#define sq(x) ((x) * (x))

extern const struct shclass shtypes[]; /* defined in shknam.c */

STATIC_OVL boolean
isbig(sroom)
register struct mkroom *sroom;
{
    register int area = (sroom->hx - sroom->lx + 1)
                        * (sroom->hy - sroom->ly + 1);

    return (boolean) (area > 20);
}

/* make and stock a room of a given type */
int
make_room(roomtype)
int roomtype;
{
    if (roomtype >= SHOPBASE)
        return mkshop(); /* someday, we should be able to specify shop type */
    else
        switch (roomtype) {
        case COURT:
            return mkzoo(COURT);
            break;
        case ZOO:
            return mkzoo(ZOO);
            break;
        case BEEHIVE:
            return mkzoo(BEEHIVE);
            break;
        case LIBRARY:
            return mkzoo(LIBRARY);
            break;
        case DRAGONLAIR:
            mkdragonlair();
            break;
        case MORGUE:
            return mkzoo(MORGUE);
            break;
        case BARRACKS:
            return mkzoo(BARRACKS);
            break;
        case ARMORY:
            return mkzoo(ARMORY);
            break;
        case SWAMP:
            return mkswamp();
            break;
        case GARDEN:
            return mkgarden();
            break;
        case TEMPLE:
            return mktemple();
            break;
        case SMITHY:
            return mksmithy();
            break;
        case NPCROOM:
            return mknpcroom(MAX_NPC_SUBTYPES);
            break;
        case LEPREHALL:
            return mkzoo(LEPREHALL);
            break;
        case COCKNEST:
            return mkzoo(COCKNEST);
            break;
        case ANTHOLE:
            return mkzoo(ANTHOLE);
            break;
        case DESERTEDSHOP:
            return mkdesertedshop();
            break;
        default:
            impossible("Tried to make a room of type %d.", roomtype);
            return 0;
        }
    return 0;
}

STATIC_OVL int
mkshop()
{
    register struct mkroom *sroom;
    int i = -1;
    char *ep = (char *) 0; /* (init == lint suppression) */

    /* first determine shoptype */
    if (wizard) {
#ifndef MAC
        ep = nh_getenv("SHOPTYPE");
        if (ep) {
            if (*ep == 'z' || *ep == 'Z') {
                return mkzoo(ZOO);
            }
            if (*ep == 'm' || *ep == 'M') {
                return mkzoo(MORGUE);
            }
            if (*ep == 'b' || *ep == 'B') {
                return mkzoo(BEEHIVE);
            }
            if (*ep == 'k' || *ep == 'K') { // Kirjasto is library in Finnish -- JG
                return mkzoo(LIBRARY);
            }
            if (*ep == 'd' || *ep == 'D') {
                return mkdragonlair();
                return 1;
            }
            if (*ep == 't' || *ep == 'T' || *ep == '\\') {
                return mkzoo(COURT);
            }
            if (*ep == 's' || *ep == 'S') {
                return mkzoo(BARRACKS);
            }
            if (*ep == 'r' || *ep == 'R') {
                return mkzoo(ARMORY);
            }
            if (*ep == 'a' || *ep == 'A') {
                return mkzoo(ANTHOLE);
            }
            if (*ep == 'c' || *ep == 'C') {
                return mkzoo(COCKNEST);
            }
            if (*ep == 'l' || *ep == 'L') {
                return mkzoo(LEPREHALL);
            }
            if (*ep == '_') {
                return mktemple();
            }
            if (*ep == '}') {
                return mkswamp();
            }
            if (*ep == 'g' || *ep == 'G') { // Garden gnomes
                return mkgarden();
            }
            if (*ep == 'p' || *ep == 'P') { // shoP
                return mkdesertedshop();
            }
            for (i = 0; shtypes[i].name; i++)
                if (*ep == def_oc_syms[(int) shtypes[i].symb].sym)
                    goto gottype;
            if (*ep == 'g' || *ep == 'G')
                i = 0;
            else if (*ep == 'v' || *ep == 'V')
                i = FODDERSHOP - SHOPBASE; /* veggy food */
            else
                i = -1;
        }
#endif
    }
#ifndef MAC
gottype:
#endif
    for (sroom = &rooms[0];; sroom++) {
        if (sroom->hx < 0)
            return 0;
        if (sroom - rooms >= nroom) {
            pline("rooms not closed by -1?");
            return 0;
        }
        if (sroom->rtype != OROOM)
            continue;
        if (has_dnstairs(sroom) || has_upstairs(sroom))
            continue;
        if ((wizard && ep && sroom->doorct != 0) || sroom->doorct == 1)
            break;
    }

    int x, y;
    if (!sroom->rlit) {

        for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
            for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
                levl[x][y].lit = 1;
        sroom->rlit = 1;
    }

    if (i < 0) { /* shoptype not yet determined */
        register int j;

        /* pick a shop type at random */
        for (j = rnd(100), i = 0; (j -= shtypes[i].prob) > 0; i++)
            continue;

        /* big rooms cannot be wand or book shops,
         * - so make them general stores
         */
        if (isbig(sroom) && (shtypes[i].symb == WAND_CLASS
                             || shtypes[i].symb == SPBOOK_CLASS))
            i = 0;
    }
    sroom->rtype = SHOPBASE + i;

    /* Change floor type */
    enum floor_subtypes lsubtype = !rn2(2) ? FLOOR_SUBTYPE_PARQUET : FLOOR_SUBTYPE_MARBLE;
    for (x = sroom->lx; x <= sroom->hx; x++)
        for (y = sroom->ly; y <= sroom->hy; y++)
        {
            if (levl[x][y].typ == ROOM)
            {
                levl[x][y].subtyp = lsubtype;
                levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
            }
            else if (levl[x][y].floortyp == ROOM)
            {
                levl[x][y].floorsubtyp = lsubtype;
                levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
            }
        }

    /* Remove doodads */
    for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
        for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
            if(isok(x, y))
            {
                levl[x][y].floor_doodad = 0;
                delete_decoration(x, y);
            }

    /* Set room tileset */
    set_room_tileset(sroom);

    /* Add a painting */
    schar lowx = sroom->lx;
    schar hix = sroom->hx;
    schar lowy = sroom->ly;
    int roll2 = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
    if (lowx + 1 < hix && !rn2(2))
    {
        if (IS_WALL(levl[lowx + roll2 + 1][lowy - 1].typ))
        {
            levl[lowx + roll2 + 1][lowy - 1].decoration_typ = DECORATION_PAINTING;
            levl[lowx + roll2 + 1][lowy - 1].decoration_subtyp = rn2(MAX_PAINTINGS);
            levl[lowx + roll2 + 1][lowy - 1].decoration_dir = 0;
            levl[lowx + roll2 + 1][lowy - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
        }
    }

    /* set room bits before stocking the shop */
#ifdef SPECIALIZATION
    topologize(sroom, FALSE); /* doesn't matter - this is a special room */
#else
    topologize(sroom);
#endif

    /* stock the room with a shopkeeper and artifacts */
    stock_room(i, sroom, FALSE);
    context.made_shop_count++;
    return 1;
}



STATIC_OVL int
mkdesertedshop()
{
    register struct mkroom* sroom;
    int i = -1;
    char* ep = (char*)0; /* (init == lint suppression) */

    for (sroom = &rooms[0];; sroom++) {
        if (sroom->hx < 0)
            return 0;
        if (sroom - rooms >= nroom) {
            pline("rooms not closed by -1?");
            return 0;
        }
        if (sroom->rtype != OROOM)
            continue;
        if (has_dnstairs(sroom) || has_upstairs(sroom))
            continue;
        if ((wizard && ep && sroom->doorct != 0) || sroom->doorct == 1)
            break;
    }

    /* Lights are off, scary */
    if (sroom->rlit) {
        int x, y;

        for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
            for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
                levl[x][y].lit = 0;
        sroom->rlit = 0;
    }
            

    if (i < 0) { /* shoptype not yet determined */
        register int j;

        /* pick a shop type at random */
        for (j = rnd(100), i = 0; (j -= shtypes[i].prob) > 0; i++)
            continue;

        /* big rooms cannot be wand or book shops,
            * - so make them general stores
            */
        if (isbig(sroom) && (shtypes[i].symb == WAND_CLASS
            || shtypes[i].symb == SPBOOK_CLASS))
            i = 0;
    }

    /* Change temporarily to so mimics look correct */
    sroom->rtype = SHOPBASE + i;

    /* Change floor */
    int x, y;
    enum floor_subtypes lsubtype = !rn2(2) ? FLOOR_SUBTYPE_PARQUET : FLOOR_SUBTYPE_MARBLE;
    for (x = sroom->lx; x <= sroom->hx; x++)
        for (y = sroom->ly; y <= sroom->hy; y++)
        {
            if (levl[x][y].typ == ROOM)
            {
                levl[x][y].subtyp = lsubtype;
                levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
            }
            else if (levl[x][y].floortyp == ROOM)
            {
                levl[x][y].floorsubtyp = lsubtype;
                levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
            }
        }

    /* set room bits before stocking the shop */
    topologize(sroom);

    /* stock the room with a shopkeeper and artifacts */
    stock_room(i, sroom, TRUE);

    context.made_deserted_shop = 1;

    /* Change back to get the right message */
    sroom->rtype = DESERTEDSHOP;

    /* Remove doodads */
    for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
        for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
            if (isok(x, y))
            {
                levl[x][y].floor_doodad = 0;
                delete_decoration(x, y);
            }

    /* Set room tileset */
    set_room_tileset(sroom);

    return 1;
}

/* pick an unused room, preferably with only one door */
STATIC_OVL struct mkroom *
pick_room(strict)
register boolean strict;
{
    register struct mkroom *sroom;
    register int i = nroom;

    for (sroom = &rooms[rn2(nroom)]; i--; sroom++) {
        if (sroom == &rooms[nroom])
            sroom = &rooms[0];
        if (sroom->hx < 0)
            return (struct mkroom *) 0;
        if (sroom->rtype != OROOM)
            continue;
        if (!strict) {
            if (has_upstairs(sroom) || (has_dnstairs(sroom) && rn2(3)))
                continue;
        } else if (has_upstairs(sroom) || has_dnstairs(sroom))
            continue;
        if (sroom->doorct == 1 || !rn2(5) || wizard)
            return sroom;
    }
    return (struct mkroom *) 0;
}

STATIC_OVL int
mkzoo(type)
int type;
{
    register struct mkroom *sroom;
    int x, y;

    if ((sroom = pick_room(FALSE)) != 0) {
        sroom->rtype = type;
        fill_zoo(sroom);
        
        /* Remove doodads */
        for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
            for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
                if (isok(x, y))
                {
                    levl[x][y].floor_doodad = 0;
                    delete_decoration(x, y);
                }

        /* Set room tileset */
        set_room_tileset(sroom);

        /* Change floor for some randomly generated zoo rooms */
        int floorsubtype = 0;
        switch (sroom->rtype)
        {
        case COURT:
            floorsubtype = FLOOR_SUBTYPE_MARBLE;
            break;
        case LIBRARY:
            floorsubtype = FLOOR_SUBTYPE_PARQUET;
            break;
        default:
            break;
        }

        if (floorsubtype > 0)
        {
            int rmno = (int)((sroom - rooms) + ROOMOFFSET);
            for (x = sroom->lx; x <= sroom->hx; x++)
                for (y = sroom->ly; y <= sroom->hy; y++)
                    if (!sroom->irregular || (sroom->irregular && levl[x][y].roomno == rmno))
                    {
                        if (levl[x][y].typ == ROOM)
                        {
                            levl[x][y].subtyp = floorsubtype;
                            levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
                        }
                        else if (levl[x][y].floortyp == ROOM)
                        {
                            levl[x][y].floorsubtyp = floorsubtype;
                            levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
                        }
                    }

        }

        int lowx = sroom->lx;
        int hix = sroom->hx;
        int lowy = sroom->ly;
        switch (sroom->rtype)
        {
        case BARRACKS:
        case ARMORY:
        case COURT:
            if (lowx + 1 < hix)
            {
                int roll = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
                schar bannertype = (schar)rn2(sroom->rtype == COURT ? BANNER_RED_OLD + 1 : MAX_BANNERS);
                if (IS_WALL(levl[lowx + roll + 1][lowy - 1].typ))
                {
                    levl[lowx + roll + 1][lowy - 1].decoration_typ = DECORATION_BANNER;
                    levl[lowx + roll + 1][lowy - 1].decoration_subtyp = bannertype;
                    levl[lowx + roll + 1][lowy - 1].decoration_dir = 0;
                    levl[lowx + roll + 1][lowy - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
                }
                if ((hix - lowx > 5 && rn2(3)) || (hix - lowx >= 2 && !rn2(3)))
                {
                    int roll2 = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
                    if (IS_WALL(levl[lowx + roll2 + 1][lowy - 1].typ))
                    {
                        levl[lowx + roll2 + 1][lowy - 1].decoration_typ = DECORATION_BANNER;
                        levl[lowx + roll2 + 1][lowy - 1].decoration_subtyp = bannertype;
                        levl[lowx + roll2 + 1][lowy - 1].decoration_dir = 0;
                        levl[lowx + roll2 + 1][lowy - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
                    }

                }
                if ((hix - lowx > 5 && !rn2(3)))
                {
                    int roll3 = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
                    if (IS_WALL(levl[lowx + roll3 + 1][lowy - 1].typ))
                    {
                        levl[lowx + roll3 + 1][lowy - 1].decoration_typ = DECORATION_BANNER;
                        levl[lowx + roll3 + 1][lowy - 1].decoration_subtyp = bannertype;
                        levl[lowx + roll3 + 1][lowy - 1].decoration_dir = 0;
                        levl[lowx + roll3 + 1][lowy - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
                    }
                }
                if (lowx + 1 < hix && !rn2(2))
                {
                    int roll4 = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
                    if (IS_WALL(levl[lowx + roll4 + 1][lowy - 1].typ))
                    {
                        levl[lowx + roll4 + 1][lowy - 1].decoration_typ = DECORATION_SHIELD_WITH_SWORDS;
                        levl[lowx + roll4 + 1][lowy - 1].decoration_subtyp = 0;
                        levl[lowx + roll4 + 1][lowy - 1].decoration_dir = 0;
                        levl[lowx + roll4 + 1][lowy - 1].decoration_flags = (rn2(6) ? DECORATION_FLAGS_ITEM_IN_HOLDER : 0UL) | (rn2(5) ? DECORATION_FLAGS_ITEM2_IN_HOLDER : 0UL) | (rn2(5) ? DECORATION_FLAGS_ITEM3_IN_HOLDER : 0UL);
                    }
                }
                if (lowx + 1 < hix && sroom->rtype == COURT && !rn2(2))
                {
                    int roll5 = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
                    if (IS_WALL(levl[lowx + roll5 + 1][lowy - 1].typ))
                    {
                        levl[lowx + roll5 + 1][lowy - 1].decoration_typ = DECORATION_KNIGHT_NICHE;
                        levl[lowx + roll5 + 1][lowy - 1].decoration_subtyp = 0;
                        levl[lowx + roll5 + 1][lowy - 1].decoration_dir = 0;
                        levl[lowx + roll5 + 1][lowy - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
                    }
                }
            }
            break;
        default:
            break;
        }
        /* Decorations */

        return 1;
    }
    return 0;
}

void
mk_zoo_thronemon(x,y)
int x,y;
{
    int i = rnd(level_difficulty());
    int pm = (i > 12) ? PM_OGRE_KING
        : (i > 6) ? PM_ELVENKING
        : (i > 4) ? PM_GNOLL_KING
        : (i > 2) ? PM_DWARF_KING
        : PM_GNOME_KING;
    struct monst *mon = makemon(&mons[pm], x, y, MM_WAITFORU);

    if (mon) {
        //mon->msleeping = 1;
        mon->mpeaceful = 0;
        set_mhostility(mon);
        /* Give him a sceptre to pound in judgment */
        (void) mongets(mon, MACE);
    }
}

void
fill_zoo(sroom)
struct mkroom *sroom;
{
    struct monst *mon;
    register int sx, sy, i;
    int sh, tx = 0, ty = 0, goldlim = 0, type = sroom->rtype;
    int rmno = (int) ((sroom - rooms) + ROOMOFFSET);
    coord mm;
    int librarytype = 0;

    register int roll = rn2(4);
    int hd = level_difficulty();
    struct permonst* main_monster = 0;
    int special_item_chance = 0;
    struct obj* firstbox = 0;;
    struct obj* middlebox = 0;
    struct obj* lastbox = 0;
    boolean anvil_created = FALSE;
    int box_one_in_chance = 5;
    int mon_one_in_chance = 1;
    int box_count = 0;
    int box_threshold = 5;
    boolean make_special_item = FALSE;
    int x, y;

    sh = sroom->fdoor;
    switch (type)
    {
    case ANTHOLE:
    case MORGUE:
        /* Change floor to ground, more sensible for graveyards */
        for (x = sroom->lx; x <= sroom->hx; x++)
            for (y = sroom->ly; y <= sroom->hy; y++)
                if (!sroom->irregular || (sroom->irregular && levl[x][y].roomno == rmno))
                {
                    if (levl[x][y].typ == ROOM)
                    {
                        levl[x][y].typ = GROUND;
                        levl[x][y].subtyp = GROUND_SUBTYPE_NORMAL;
                        levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
                    }
                    else if (levl[x][y].floortyp == ROOM)
                    {
                        levl[x][y].floortyp = GROUND;
                        levl[x][y].floorsubtyp = GROUND_SUBTYPE_NORMAL;
                        levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
                    }
                }
        break;
    case COURT:
        if (level.flags.is_maze_lev)
        {
            for (tx = sroom->lx; tx <= sroom->hx; tx++)
                for (ty = sroom->ly; ty <= sroom->hy; ty++)
                    if (IS_THRONE(levl[tx][ty].typ))
                        goto throne_placed;
        }
        i = 100;
        do { /* don't place throne on top of stairs */
            (void) somexy(sroom, &mm);
            tx = mm.x;
            ty = mm.y;
        } while (occupied((xchar) tx, (xchar) ty) && --i > 0);
    throne_placed:
        mk_zoo_thronemon(tx, ty);
        break;
    case LIBRARY:
        mon_one_in_chance = 2;
        if (hd <= 9)
        {
            if (roll <= 1)
                librarytype = 0; //Gnomish wizards
            else
                librarytype = 1; //Gnoll wardens
        }
        else if (hd <= 12)
        {
            if (roll == 0)
                librarytype = 0; //Gnomish wizards
            else if (roll == 1)
                librarytype = 1; //Gnoll wardens
            else
                librarytype = 2; //Liches
        }
        else
        {
            if (roll <= 2)
                librarytype = 2; //Liches
            else
                librarytype = 3; //Tentacled ones
        }

        if (librarytype == 0) // Gnomes
        {
            main_monster = &mons[PM_GNOMISH_WIZARD];
        }
        else if (librarytype == 1) // Gnolls
        {
            main_monster = &mons[PM_GNOLL_SUPREME_WARDEN];
        }
        else if (librarytype == 2) // Liches
        {
            if (Inhell || In_endgame(&u.uz)) {
                main_monster = !rn2(4) ? &mons[PM_ARCH_LICH] : &mons[PM_MASTER_LICH];
            }
            else
            {
                if (hd >= 25)
                    main_monster = &mons[PM_MASTER_LICH];
                if (hd >= 13)
                    main_monster = &mons[PM_DEMILICH];
                else
                    main_monster = &mons[PM_LICH];
            }
        }
        else if (librarytype == 3) // Tentacled ones
        {
            if ((hd >= 25 || Inhell || In_endgame(&u.uz))) {
                main_monster = &mons[PM_ELDER_TENTACLED_ONE];
            }
            else
                main_monster = &mons[PM_TENTACLED_ONE];
        }
        else
            main_monster = &mons[PM_GNOMISH_WIZARD];
        //Note in library we place only a monster in one in every 2 squares
        goto place_main_monst_here; /* Prevent fall thru warning */
    case COCKNEST:
    case BEEHIVE:
place_main_monst_here:
        tx = sroom->lx + (sroom->hx - sroom->lx + 1) / 2;
        ty = sroom->ly + (sroom->hy - sroom->ly + 1) / 2;
        if (sroom->irregular) {
            /* center might not be valid, so put queen elsewhere */
            if ((int) levl[tx][ty].roomno != rmno || levl[tx][ty].edge) {
                (void) somexy(sroom, &mm);
                tx = mm.x;
                ty = mm.y;
            }
        }
        break;
    case ZOO:
    case LEPREHALL:
        goldlim = 500 * level_difficulty();
        break;
    case ARMORY:
        special_item_chance = (depth(&u.uz) * 3) / 2;
        make_special_item = (rn2(100) < special_item_chance);
        box_threshold = min(5, (sroom->hx - sroom->lx) * (sroom->hy - sroom->ly) / 2);
        box_one_in_chance = max(3, (sroom->hx - sroom->lx) * (sroom->hy - sroom->ly) / 4);
        if (depth(&u.uz) < depth(&oracle_level))
            mon_one_in_chance = max(2, (sroom->hx - sroom->lx) * (sroom->hy - sroom->ly) / 12);
        else if (depth(&u.uz) > depth(&medusa_level))
            mon_one_in_chance = 1;
        else
            mon_one_in_chance = 2;

        if (make_special_item)
            goto place_main_monst_here;
        else
            tx = 0, ty = 0;
        
        break;
    }


    for (sx = sroom->lx; sx <= sroom->hx; sx++)
        for (sy = sroom->ly; sy <= sroom->hy; sy++) 
        {
            if (sroom->irregular)
            {
                if ((int) levl[sx][sy].roomno != rmno || levl[sx][sy].edge
                    || (sroom->doorct
                        && distmin(sx, sy, doors[sh].x, doors[sh].y) <= 1))
                    continue;
            }
            else if (!SPACE_POS(levl[sx][sy].typ)
                       || (sroom->doorct
                           && ((sx == sroom->lx && doors[sh].x == sx - 1)
                               || (sx == sroom->hx && doors[sh].x == sx + 1)
                               || (sy == sroom->ly && doors[sh].y == sy - 1)
                               || (sy == sroom->hy
                                   && doors[sh].y == sy + 1))))
                continue;
            /* don't place monster on explicitly placed throne */
            if (type == COURT && IS_THRONE(levl[sx][sy].typ))
                continue;

            if (mon_one_in_chance > 1 && rn2(mon_one_in_chance))
            {
                mon = (struct monst*) 0; //No monster
            }
            else
            {
                mon = makemon((type == COURT)
                           ? courtmon()
                           : (type == BARRACKS)
                              ? squadmon()
                           : (type == ARMORY)
                              ? armorymon(sx == tx && sy == ty ? TRUE : FALSE)
                              : (type == MORGUE)
                                 ? morguemon()
                              : (type == LIBRARY)
                                ? (sx == tx && sy == ty
                                    ? main_monster
                                    : librarymon(librarytype))
                                 : (type == BEEHIVE)
                                     ? (sx == tx && sy == ty
                                         ? &mons[PM_QUEEN_BEE]
                                         : &mons[PM_KILLER_BEE])
                                     : (type == LEPREHALL)
                                         ? &mons[PM_LEPRECHAUN]
                                         : (type == COCKNEST)
                                            ? (sx == tx && sy == ty
                                                ? (hd >= 23 ? &mons[PM_GARGANTUAN_COCKATRICE] : &mons[PM_GIANT_COCKATRICE])
                                                : (hd >= 23 && !rn2(4) ? &mons[PM_GIANT_COCKATRICE] : &mons[PM_COCKATRICE]))
                                             : (type == ANTHOLE)
                                                 ? antholemon()
                                                 : (struct permonst *) 0,
                          sx, sy, MM_WAITFORU);
            }

            if (mon)
            {
                //mon->msleeping = 1;
                if (type == COURT && is_peaceful(mon))
                {
                    mon->mpeaceful = 0;
                    set_mhostility(mon);
                }
            }

            if (!IS_FLOOR(levl[sx][sy].typ))
                continue;

            switch (type) {
            case ZOO:
            case LEPREHALL:
                if (sroom->doorct) {
                    int distval = dist2(sx, sy, doors[sh].x, doors[sh].y);
                    i = sq(distval);
                } else
                    i = goldlim;
                if (i >= goldlim)
                    i = 5 * level_difficulty();
                goldlim -= i;
                (void) mkgold((long) rn1(i, 10), sx, sy);
                break;
            case MORGUE:
            {
                struct obj* otmp = (struct obj*)0;
                boolean dealloc = FALSE;
                
                if (!rn2(7))
                {
                    otmp = mksobj_at((rn2(7)) ? COFFIN : SARCOPHAGUS, sx, sy,
                        TRUE, FALSE);
                    if (otmp && rn2(2))
                    {
                        (void)bury_an_obj(otmp, &dealloc);
                        if (dealloc)
                            otmp = 0, dealloc = FALSE;

                        if (!rn2(2))
                            make_grave(sx, sy, (char*)0, TRUE);

                    }
                }
                else
                {
                    if (!rn2(5))
                    { 
                        otmp = mk_tt_object(CORPSE, sx, sy);
                        if (otmp && rn2(4))
                        {
                            (void)bury_an_obj(otmp, &dealloc);
                            if (dealloc)
                                otmp = 0, dealloc = FALSE;

                            if (!rn2(3))
                                make_grave(sx, sy, (char*)0, TRUE);
                        }
                    }
                    else if (!rn2(5))
                    {
                        boolean bury = !!rn2(3);
                        otmp = mksobj_at(BONE, sx, sy,
                            TRUE, FALSE);
                        otmp->quan = rnd(5);
                        otmp->owt = weight(otmp);
                        if (otmp && bury)
                        {
                            (void)bury_an_obj(otmp, &dealloc);
                            if (dealloc)
                                otmp = 0, dealloc = FALSE;
                        }
                        if (rn2(3))
                        {
                            otmp = mksobj_at(HUMAN_SKULL, sx, sy,
                                TRUE, FALSE);
                            if (otmp && bury)
                            {
                                (void)bury_an_obj(otmp, &dealloc);
                                if (dealloc)
                                    otmp = 0, dealloc = FALSE;
                            }
                        }
                        if (bury && !rn2(4))
                            make_grave(sx, sy, (char*)0, TRUE);

                    }
                    else
                    {
                        if (!rn2(5))
                            make_grave(sx, sy, (char*)0, TRUE);

                    }

                    if ((otmp && !rn2(5)) || !rn2(12)) /* lots of treasure buried with dead */
                    {
                        otmp = mksobj_at((rn2(3)) ? LARGE_BOX : CHEST, sx, sy,
                            TRUE, FALSE);
                        if (otmp && rn2(4))
                        {
                            (void)bury_an_obj(otmp, &dealloc);
                            if (dealloc)
                                otmp = 0, dealloc = FALSE;
                        }
                    }
                }

                break;
            }
            case BEEHIVE:
                if (!rn2(3))
                    (void) mksobj_at(LUMP_OF_ROYAL_JELLY, sx, sy, TRUE,
                                     FALSE);
                break;
            case LIBRARY:
                if (!rn2(5)) //Bookshelves are fuller at higher difficulty
                    (void)mksobj_at(BOOKSHELF, sx, sy, TRUE, FALSE);
                else if(!rn2(level_difficulty() >= 12 ? 4 : 5))
                    (void)mkobj_at(!rn2(4) ? SPBOOK_CLASS : SCROLL_CLASS, sx, sy, FALSE);
                break;
            case BARRACKS:
                if (!rn2(20)) /* the payroll and some loot */
                    (void) mksobj_at((rn2(3)) ? LARGE_BOX : CHEST, sx, sy,
                                     TRUE, FALSE);
                break;
            case ARMORY:
                if (!rn2(box_one_in_chance * (box_count >= box_threshold ? 2 : 1)))
                {
                    struct obj* box = mksobj_at(rn2(2) ? WEAPON_RACK : rn2(3) ? LARGE_BOX : CHEST, sx, sy,
                        FALSE, FALSE);

                    if (box)
                    {
                        box_count++;
                        lastbox = box;
                        if (!firstbox)
                            firstbox = box;
                        else
                        {
                            if (!rn2(2))
                                middlebox = box;
                        }

                        int nobj = rnd(4) + (rn2(3) ? 1 : 0);
                        for (i = 0; i < nobj; i++)
                        {
                            struct obj* item = 0;
                            item = mkobj(box->otyp == WEAPON_RACK || rn2(2) ? WEAPON_CLASS : ARMOR_CLASS, FALSE, FALSE);

                            if (item)
                            {
                                add_to_container(box, item);
                                box->owt = weight(box);
                            }
                        }
                        box->speflags |= SPEFLAGS_NO_PICKUP;
                        //make_engr_at(sx, sy, Inhell ? Morgoth_word : Gilthoniel_word, 0L, ENGRAVE);
                        context.made_armory_box_count++;
                    }
                }
                else if (!rn2(20))
                {
                    /* Some anvils to add to the atmosphere */
                    if (IS_FLOOR(levl[sx][sy].typ))
                    {
                        levl[sx][sy].floortyp = levl[sx][sy].typ;
                        levl[sx][sy].floorsubtyp = levl[sx][sy].subtyp;
                        levl[sx][sy].floorvartyp = levl[sx][sy].vartyp;
                        levl[sx][sy].typ = ANVIL;
                        levl[sx][sy].subtyp = 0;
                        levl[sx][sy].vartyp = 0;
                        levl[sx][sy].special_quality = 0;
                        anvil_created = TRUE;
                    }
                }
                break;
            case COCKNEST:
                if (!rn2(3)) {
                    struct obj *sobj = mk_tt_object(STATUE, sx, sy);

                    if (sobj) {
                        for (i = rn2(5); i; i--)
                            (void) add_to_container(
                                sobj, mkobj(RANDOM_CLASS, FALSE, TRUE));
                        sobj->owt = weight(sobj);
                    }
                }
                break;
            case ANTHOLE:
                if (!rn2(3))
                    (void) mkobj_at(FOOD_CLASS, sx, sy, FALSE);
                break;
            }
        }



    switch (type) {
    case COURT: {
        struct obj *chest, *gold;
        create_initial_location_with_current_floor(tx, ty, THRONE, 0UL, 0, FALSE);
        (void) somexy(sroom, &mm);
        gold = mksobj(GOLD_PIECE, TRUE, FALSE, FALSE);
        gold->quan = (long) rn1(50 * level_difficulty(), 10);
        gold->owt = weight(gold);
        /* the royal coffers */
        chest = mksobj_at(CHEST, mm.x, mm.y, TRUE, FALSE);
        add_to_container(chest, gold);
        chest->owt = weight(chest);
        chest->speflags |= SPEFLAGS_ROYAL_COFFER; /* so it can be found later */
        level.flags.has_court = 1;
        break;
    }
    case BARRACKS:
        level.flags.has_barracks = 1;
        break;
    case ARMORY:
        if (!box_count)
        {
            for (int try_count = 0; try_count < 20; try_count++)
            {
                (void)somexy(sroom, &mm);
                if (IS_FLOOR(levl[mm.x][mm.y].typ) && !OBJ_AT(mm.x, mm.y))
                {
                    struct obj* box = mksobj_at((rn2(3)) ? LARGE_BOX : CHEST, mm.x, mm.y,
                        FALSE, FALSE);

                    if (box)
                    {
                        lastbox = box;
                        if (!firstbox)
                            firstbox = box;
                        else
                        {
                            if (!rn2(2))
                                middlebox = box;
                        }

                        int nobj = rnd(4) + (rn2(3) ? 1 : 0);
                        for (i = 0; i < nobj; i++)
                        {
                            struct obj* item = 0;
                            item = mkobj(rn2(5) ? WEAPON_CLASS : ARMOR_CLASS, FALSE, FALSE);

                            if (item)
                            {
                                add_to_container(box, item);
                                box->owt = weight(box);
                            }
                        }
                        make_engr_at(mm.x, mm.y, Inhell ? Morgoth_word : Gilthoniel_word, 0L, ENGRAVE, ENGR_FLAGS_NONE);
                        context.made_armory_box_count++;
                    }
                    break;
                }
            }
        }
        if (lastbox && firstbox && rn2(100) < special_item_chance)
        {
            struct obj* box = !middlebox || rn2(3) ? (!rn2(2) ? firstbox : lastbox) : middlebox;
            if (box)
            {
                struct obj* item = mk_artifact((struct obj*)0, A_NONE, MKARTIFACT_FLAGS_WEAPONS_ONLY);
                if (item)
                {
                    add_to_container(box, item);
                    box->owt = weight(box);
                }
            }
        }
        if (!anvil_created)
        {
            for (int try_count = 0; try_count < 20; try_count++)
            {
                (void)somexy(sroom, &mm);
                if (IS_FLOOR(levl[mm.x][mm.y].typ) && !OBJ_AT(mm.x, mm.y))
                {
                    levl[mm.x][mm.y].floortyp = levl[mm.x][mm.y].typ;
                    levl[mm.x][mm.y].floorsubtyp = levl[mm.x][mm.y].subtyp;
                    levl[mm.x][mm.y].typ = ANVIL;
                    levl[mm.x][mm.y].subtyp = 0;
                    break;
                }
            }
        }
        level.flags.has_armory = 1;
        break;
    case ZOO:
        level.flags.has_zoo = 1;
        break;
    case MORGUE:
        level.flags.has_morgue = 1;
        break;
    case SWAMP:
        level.flags.has_swamp = 1;
        // fillzoo is not called for swamp
        break;
    case BEEHIVE:
        level.flags.has_beehive = 1;
        break;
    case LIBRARY:
        level.flags.has_library = 1;
        break;
    }

    /* Shields */
    switch (type)
    {
    case ZOO:
    case BARRACKS:
    case COURT:
    case ARMORY:
    {
        int roll1 = sroom->hx - sroom->lx - 1 <= 1 ? 0 : rn2(sroom->hx - sroom->lx - 1);
        int roll2 = sroom->hx - sroom->lx - 1 <= 1 ? 0 : rn2(sroom->hx - sroom->lx - 1);
        if (sroom->lx + 1 < sroom->hx)
        {
            if (IS_WALL(levl[sroom->lx + roll2 + 1][sroom->ly - 1].typ) && rn2(3))
            {
                levl[sroom->lx + roll2 + 1][sroom->ly - 1].decoration_typ = rn2(2) ? DECORATION_KNIGHT_NICHE : DECORATION_GARGOYLE_NICHE;
                levl[sroom->lx + roll2 + 1][sroom->ly - 1].decoration_subtyp = 0;
                levl[sroom->lx + roll2 + 1][sroom->ly - 1].decoration_dir = 0;
                levl[sroom->lx + roll2 + 1][sroom->ly - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER | DECORATION_FLAGS_ITEM2_IN_HOLDER | DECORATION_FLAGS_ITEM3_IN_HOLDER;
            }
            if (IS_WALL(levl[sroom->lx + roll1 + 1][sroom->ly - 1].typ) && rn2(3))
            {
                levl[sroom->lx + roll1 + 1][sroom->ly - 1].decoration_typ = DECORATION_SHIELD_WITH_SWORDS;
                levl[sroom->lx + roll1 + 1][sroom->ly - 1].decoration_subtyp = 0;
                levl[sroom->lx + roll1 + 1][sroom->ly - 1].decoration_dir = 0;
                levl[sroom->lx + roll1 + 1][sroom->ly - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER | DECORATION_FLAGS_ITEM2_IN_HOLDER | DECORATION_FLAGS_ITEM3_IN_HOLDER;
            }
        }
        break;
    }
    default:
        break;
    }

    /* Paintings */
    switch (type)
    {
    case LIBRARY:
    case COURT:
    {
        int roll1 = sroom->hx - sroom->lx - 1 <= 1 ? 0 : rn2(sroom->hx - sroom->lx - 1);
        if (sroom->lx + 1 < sroom->hx)
        {
            if (IS_WALL(levl[sroom->lx + roll1 + 1][sroom->ly - 1].typ) && rn2(3))
            {
                levl[sroom->lx + roll1 + 1][sroom->ly - 1].decoration_typ = DECORATION_PAINTING;
                levl[sroom->lx + roll1 + 1][sroom->ly - 1].decoration_subtyp = rn2(MAX_PAINTINGS);
                levl[sroom->lx + roll1 + 1][sroom->ly - 1].decoration_dir = 0;
                levl[sroom->lx + roll1 + 1][sroom->ly - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
            }
        }
        break;
    }
    default:
        break;
    }
}

/* make a swarm of undead around mm */
void
mkundead(mm, revive_corpses, mm_flags)
coord *mm;
boolean revive_corpses;
int mm_flags;
{
    int cnt = (level_difficulty() + 1) / 10 + rnd(5);
    struct permonst *mdat;
    struct obj *otmp;
    coord cc;
    struct monst* mtmp;

    context.makemon_spef_idx = 0;
    while (cnt--) {
        mdat = morguemon();
        if (mdat && enexto(&cc, mm->x, mm->y, mdat)
            && (!revive_corpses
                || !(otmp = sobj_at(CORPSE, cc.x, cc.y))
                || !revive(otmp, FALSE, -1, FALSE)))
        {
            mtmp = makemon(mdat, cc.x, cc.y, mm_flags);
            if (mtmp)
                context.makemon_spef_idx++;

        }
    }
    makemon_animation_wait_until_end();
    level.flags.graveyard = TRUE; /* reduced chance for undead corpse */
}

STATIC_OVL struct permonst *
morguemon()
{
    register int i = rn2(100), hd = rn2(level_difficulty());

    if (hd > 15 && i < 3)
        return &mons[PM_KING_WRAITH];
    else if (hd > 10 && i < 10) 
    {
        if (Inhell || In_endgame(&u.uz)) {
            return mkclass(S_DEMON, 0);
        } 
        else 
        {
            if(!rn2(2))
                return &mons[PM_SPECTRE];
            else
            {
                int ndemon_res = ndemon(A_NONE, FALSE);
                if (ndemon_res != NON_PM)
                    return &mons[ndemon_res];
                /* else do what? As is, it will drop to ghost/wraith/zombie */
            }
        }
    }

    if (hd > 8 && i > 85)
        return mkclass_core(S_VAMPIRE, 0, A_NONE, -rn2(5), 0UL);

    return ((i < 20) ? &mons[PM_GHOST]
                     : (i < 30) ? &mons[PM_WRAITH]
                     : (i < 50) ? mkclass_core(S_GREATER_UNDEAD, 0, A_NONE, -rn2(5), 0UL)
                        : mkclass(S_LESSER_UNDEAD, -rn2(5)));
}


STATIC_OVL struct permonst*
librarymon(type)
int type;
{
    int hd = level_difficulty();
    register int i = rn2(100);

    if(type == 0) // Gnomes
    {
        return &mons[PM_GNOMISH_WIZARD];
    }
    else if (type == 1) // Gnolls
    {
        return &mons[PM_GNOLL_WARDEN];
    }
    else if (type == 2) // Liches
    {
        if (i <= 5) // Another main 
        {
            if ((Inhell || In_endgame(&u.uz)) && !rn2(4))
                return &mons[PM_MASTER_LICH];
            else if((hd >= 13 || Inhell || In_endgame(&u.uz)))
                return &mons[PM_DEMILICH];
            else
                return &mons[PM_LICH];
        }
        else if(i <= (hd >= 17 ? 40 : hd >= 13 ? 30 : 20))
        {
            if (Inhell || In_endgame(&u.uz))
                return (!rn2(2) ? &mons[PM_DEMILICH] : &mons[PM_LICH]);
            else
                return &mons[PM_LICH];
        }
        else if (i <= 65)
            return &mons[PM_QUASIT];
        else if (i <= 100)
            return &mons[PM_IMP];
    }
    else if (type == 3) // Tentacled ones
    {
        if ((Inhell || In_endgame(&u.uz)) && !rn2(4)) {
            return &mons[PM_ELDER_TENTACLED_ONE];
        }
        else
            return &mons[PM_TENTACLED_ONE];
    }

    return (struct permonst*) 0;
}


struct permonst *
antholemon()
{
    int mtyp, indx, trycnt = 0;

    /* casts are for dealing with time_t */
    indx = (int) ((long) ubirthday % 3L);
    indx += level_difficulty();
    /* Same monsters within a level, different ones between levels */
    do 
    {
        switch ((indx + trycnt) % 3) 
        {
        case 0:
            mtyp = PM_SOLDIER_ANT;
            break;
        case 1:
            mtyp = PM_FIRE_ANT;
            break;
        default:
            mtyp = PM_GIANT_ANT;
            break;
        }
        /* try again if chosen type has been genocided or used up */
    } while (++trycnt < 3 && (mvitals[mtyp].mvflags & MV_GONE));

    return ((mvitals[mtyp].mvflags & MV_GONE) ? (struct permonst *) 0
                                             : &mons[mtyp]);
}

STATIC_OVL int
mkswamp() /* Michiel Huisjes & Fred de Wilde */
{
    register struct mkroom *sroom;
    register int sx, sy, i, eelct = 0;
    int swampnumber = 0;

    for (i = 0; i < 5; i++) { /* turn up to 5 rooms swampy */
        sroom = &rooms[rn2(nroom)];
        if (sroom->hx < 0 || sroom->rtype != OROOM || has_upstairs(sroom)
            || has_dnstairs(sroom))
            continue;

        /* satisfied; make a swamp */
        sroom->rtype = SWAMP;
        for (sx = sroom->lx; sx <= sroom->hx; sx++)
            for (sy = sroom->ly; sy <= sroom->hy; sy++)
            {
                if (IS_FLOOR(levl[sx][sy].typ))
                {
                    levl[sx][sy].typ = GRASS;
                    levl[sx][sy].subtyp = GRASS_SUBTYPE_SWAMPY;
                    levl[sx][sy].vartyp = get_initial_location_vartype(levl[sx][sy].typ, levl[sx][sy].subtyp);
                }
                else
                {
                    levl[sx][sy].floortyp = GRASS;
                    levl[sx][sy].floorsubtyp = GRASS_SUBTYPE_SWAMPY;
                    levl[sx][sy].floorvartyp = get_initial_location_vartype(levl[sx][sy].floortyp, levl[sx][sy].floorsubtyp);
                }

                if (!OBJ_AT(sx, sy) && !MON_AT(sx, sy) && !t_at(sx, sy) && !nexttodoor(sx, sy))
                {
                    if ((sx + sy) % 2)
                    {
                        levl[sx][sy].typ = POOL;
                        levl[sx][sy].subtyp = 0;
                        levl[sx][sy].vartyp = get_initial_location_vartype(levl[sx][sy].typ, levl[sx][sy].subtyp);
                        levl[sx][sy].special_quality = 0;
                        levl[sx][sy].floortyp = location_type_definitions[POOL].initial_floor_type;
                        levl[sx][sy].floorsubtyp = get_initial_location_subtype(levl[sx][sy].floortyp);
                        levl[sx][sy].floorvartyp = get_initial_location_vartype(levl[sx][sy].floortyp, levl[sx][sy].floorsubtyp);
                        if (!eelct || !rn2(4))
                        {
                            /* mkclass() won't do, as we might get kraken */
                            (void)makemon(rn2(5)
                                ? &mons[PM_GIANT_EEL]
                                : rn2(2)
                                ? &mons[PM_PIRANHA]
                                : &mons[PM_ELECTRIC_EEL],
                                sx, sy, NO_MM_FLAGS);
                            eelct++;
                        }
                    }
                    else if (!rn2(4)) /* swamps tend to be moldy */
                        (void)makemon(mkclass(S_FUNGUS, 0), sx, sy, NO_MM_FLAGS);
                    else if (!rn2(8)) /* swamps may have cloudberries */
                        (void)mksobj_at(DRAGON_FRUIT, sx, sy, TRUE, FALSE);
                    else if (!rn2(10)) /* swamps may have cloudberries */
                        (void)mksobj_at(CLOUDBERRY, sx, sy, TRUE, FALSE);
                    else if (!rn2(10)) /* swamps may have cloudberries */
                        (void)mksobj_at(PANTHER_CAP, sx, sy, TRUE, FALSE);
                    else if (!rn2(10)) /* swamps may have cloudberries */
                        (void)mksobj_at(DEATH_CAP, sx, sy, TRUE, FALSE);
                    else if (!rn2(8)) /* swamps may have cloudberries */
                        (void)mksobj_at(!rn2(4) ? CHANTERELLE : !rn2(3) ? PENNY_BUN : !rn2(2) ? CHAMPIGNON : FLY_AGARIC, sx, sy, TRUE, FALSE);
                    else if (!rn2(50)) /* swamps may have phantomberries */
                        (void)mksobj_at(PHANTOMBERRY, sx, sy, TRUE, FALSE);
                }

            }


        level.flags.has_swamp = 1;
        swampnumber++;
    }
    return swampnumber;
}


STATIC_OVL int
mkgarden() 
{
    register struct mkroom* sroom = (struct mkroom*)0;
    register int sx, sy, i;

    for (i = 0; i < nroom; i++) { /* turn up to 1 rooms gardenlike */
        sroom = &rooms[rn2(nroom)];
        if (sroom->hx < 0 || sroom->rtype != OROOM || has_upstairs(sroom)
            || has_dnstairs(sroom))
            continue;
        else
            break;
    }

    if (i >= nroom)
        return 0;

    /* satisfied; make a garden */
    int levdiff = level_difficulty();
    boolean has_statues = rn2(2);
    int statue_base_type = levdiff >= 16 && rn2(2) ? PM_WINGED_GARGOYLE :
        levdiff >= 13 && rn2(3) ? PM_ROCK_TROLL : levdiff >= 7 && rn2(9) ? PM_GARGOYLE : PM_GNOME;
    sroom->rtype = GARDEN;

    /* Remove doodads */
    for (sx = sroom->lx - 1; sx <= sroom->hx + 1; sx++)
        for (sy = sroom->ly - 1; sy <= sroom->hy + 1; sy++)
            if (isok(sx, sy))
            {
                levl[sx][sy].floor_doodad = 0;
                delete_decoration(sx, sy);
            }

    /* Set room tileset */
    set_room_tileset(sroom);

    for (sx = sroom->lx; sx <= sroom->hx; sx++)
    {
        for (sy = sroom->ly; sy <= sroom->hy; sy++)
        {
            if (levl[sx][sy].typ == STAIRS || levl[sx][sy].typ == LADDER)
            {
                /* Insurance */
                levl[sx][sy].floortyp = GRASS;
                levl[sx][sy].floorsubtyp = GRASS_SUBTYPE_NORMAL;
                levl[sx][sy].floorvartyp = get_initial_location_vartype(levl[sx][sy].floortyp, levl[sx][sy].floorsubtyp);
            }
            else if (!OBJ_AT(sx, sy) && !MON_AT(sx, sy) && !t_at(sx, sy)
                && !nexttodoor(sx, sy) && !nexttotree(sx, sy) && !rn2(3))
            {
                levl[sx][sy].typ = TREE;
                levl[sx][sy].subtyp = get_initial_tree_subtype(FOREST_TYPE_GARDEN);
                levl[sx][sy].vartyp = get_initial_location_vartype(levl[sx][sy].typ, levl[sx][sy].subtyp);
                levl[sx][sy].special_quality = 0;
                levl[sx][sy].floortyp = GRASS;
                levl[sx][sy].floorsubtyp = GRASS_SUBTYPE_NORMAL;
                levl[sx][sy].floorvartyp = get_initial_location_vartype(levl[sx][sy].floortyp, levl[sx][sy].floorsubtyp);
                initialize_location(&levl[sx][sy]);
            }
            else
            {
                levl[sx][sy].typ = GRASS;
                levl[sx][sy].subtyp = GRASS_SUBTYPE_NORMAL;
                levl[sx][sy].vartyp = get_initial_location_vartype(levl[sx][sy].typ, levl[sx][sy].subtyp);
                levl[sx][sy].special_quality = 0;
                /* Buried items */
                if (!rn2(5))
                {
                    int itemtype = MANDRAKE_ROOT;
                    int quan = 1;

                    switch (rn2(3))
                    {
                    case 0:
                        itemtype = MANDRAKE_ROOT;
                        quan = 1 + (!rn2(5) ? 1 : 0);
                        break;
                    case 1:
                        itemtype = GINSENG_ROOT;
                        quan = 1 + (!rn2(3) ? 1 : 0) + (!rn2(5) ? 1 : 0);
                        break;
                    case 2:
                        itemtype = CARROT;
                        quan = rnd(3);
                        break;
                    default:
                        break;
                    }
                    struct obj* otmp = mksobj(itemtype, TRUE, FALSE, FALSE);
                    if (otmp)
                    {
                        otmp->quan = quan;
                        otmp->owt = weight(otmp);
                        otmp->ox = sx;
                        otmp->oy = sy;
                        add_to_buried(otmp);
                    }
                }

                /* Non-buried items */
                if (!rn2(4))
                {
                    int itemtype = SPRIG_OF_WOLFSBANE;
                    int quan = 1;

                    switch (rn2(26))
                    {
                    case 0:
                        itemtype = CLOVE_OF_GARLIC;
                        quan = rnd(2);
                        break;
                    case 1:
                        itemtype = SPRIG_OF_WOLFSBANE;
                        quan = rnd(3);
                        break;
                    case 2:
                        itemtype = rn2(3) ? CHAMPIGNON : HANDFUL_OF_SPINACH_LEAVES;
                        quan = 1;
                        break;
                    case 3:
                        itemtype = rn2(4) ? PENNY_BUN : AVOCADO;
                        quan = 1;
                        break;
                    case 4:
                        itemtype = APPLE;
                        quan = rnd(5);
                        break;
                    case 5:
                        itemtype = PEAR;
                        quan = rnd(5);
                        break;
                    case 6:
                        itemtype = MELON;
                        quan = rnd(2);
                        break;
                    case 7:
                        itemtype = CHAMPIGNON;
                        quan = rnd(2);
                        break;
                    case 8:
                        itemtype = PENNY_BUN;
                        quan = rnd(2);
                        break;
                    case 9:
                        itemtype = CHANTERELLE;
                        quan = rnd(2);
                        break;
                    case 10:
                        itemtype = DEATH_CAP;
                        quan = rnd(2);
                        break;
                    case 11:
                        itemtype = FLY_AGARIC;
                        quan = rnd(2);
                        break;
                    case 12:
                        itemtype = ORACULAR_TOADSTOOL;
                        quan = rnd(2);
                        break;
                    case 13:
                        itemtype = HEALING_MUSHROOM;
                        quan = rnd(2);
                        break;
                    case 14:
                        itemtype = MANA_MUSHROOM;
                        quan = rnd(2);
                        break;
                    case 15:
                        itemtype = CHANTERELLE;
                        quan = 1;
                        break;
                    case 16:
                        itemtype = PANTHER_CAP;
                        quan = 1;
                        break;
                    case 17:
                        itemtype = rn2(4) ? APPLE : BANANA;
                        quan = 1;
                        break;
                    case 18:
                        itemtype = rn2(4) ? PEAR : POMEGRANATE;
                        quan = 1;
                        break;
                    case 19:
                    case 20:
                        itemtype = CLOUDBERRY;
                        quan = rnd(2);
                        break;
                    case 21:
                    case 22:
                        itemtype = FIG;
                        quan = rnd(2);
                        break;
                    case 23:
                    case 24:
                        itemtype = DRAGON_FRUIT;
                        quan = rnd(2);
                        break;
                    case 25: /* second time */
                        itemtype = HEALING_MUSHROOM;
                        quan = rnd(2);
                        break;
                    default:
                        break;
                    }
                    struct obj* otmp = mksobj_at(itemtype, sx, sy, TRUE, FALSE);
                    if (otmp)
                    {
                        otmp->quan = quan;
                        otmp->owt = weight(otmp);
                    }
                }

                if (has_statues && !rn2(8))
                {
                    // Garden gnome as statue (with a conical hat)
                    int monster_type = statue_base_type;
                    unsigned long statueflags = 0UL;
                    switch (statue_base_type)
                    {
                    case PM_GNOME:
                        if (levdiff > 9 || (levdiff > 4 && !rn2(15)))
                            monster_type = PM_GNOME_KING;
                        else if (levdiff > 2 && !rn2(5))
                            monster_type = PM_GNOME_LORD;
                        statueflags = MKTRAPFLAG_GARDEN_GNOME_ITEMS;
                        break;
                    case PM_GARGOYLE:
                        if (levdiff >= 15 && rn2(2))
                            monster_type = PM_WINGED_GARGOYLE;
                        else if (levdiff >= 13 && !rn2(15))
                            monster_type = PM_WINGED_GARGOYLE;
                        break;
                    default:
                        break;
                    }

                    maketrap(sx, sy, STATUE_TRAP, monster_type, statueflags);
                }
                else if (!rn2(8))
                {
                    /* Random ogre at high levels */
                    struct permonst* pm = levdiff >= mons[PM_OGRE_OVERLORD].difficulty ? mkclass(S_OGRE, 0) : (struct permonst*)0;
                    
                    /* Otherwise, hobgoblins, bugbears or normal ogres + some bosses */
                    (void)makemon(!pm ? (&mons[
                         levdiff >= mons[PM_OGRE_LORD].difficulty && !rn2(4) ? PM_OGRE_LORD :
                        (levdiff >= mons[PM_OGRE].difficulty && !rn2(4)) || levdiff >= (mons[PM_OGRE].difficulty + 5) ? PM_OGRE :
                        (levdiff >= mons[PM_BUGBEAR].difficulty && !rn2(4)) || levdiff >= 7 ? PM_BUGBEAR :
                        PM_HOBGOBLIN]) : pm, sx, sy, MM_ASLEEP);

                }
            }

        }
    }
    level.flags.has_garden = 1;
    return 1;
}

STATIC_OVL int
mkdragonlair()
{
    register struct mkroom* sroom = (struct mkroom*)0;
    register int sx, sy, i, j;

    for (i = 0; i < nroom; i++) { /* turn up to 1 rooms into a dragon lair */
        sroom = &rooms[rn2(nroom)];
        if (sroom->hx < 0 || sroom->rtype != OROOM || has_upstairs(sroom)
            || has_dnstairs(sroom))
            continue;
        else
            break;
    }

    if (i >= nroom)
        return 0;

    /* satisfied; make a dragon lair */
    sroom->rtype = DRAGONLAIR;

    coord c = { 0 };
    int dragons[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int maxdragons = min(10, max(2, (sroom->hx - sroom->lx + 1) * (sroom->hy - sroom->ly + 1)));
    int dragonindex = 0;

    if (!rn2(3))
    {
        dragons[0] = PM_ANCIENT_GRAY_DRAGON + rn2(9);
        dragonindex++;
    }
    else
    {
        int dragontype = rn2(9);
        int adults = min(maxdragons, rnd(2));
        int hatchlings = min(maxdragons - adults, 1 + rnd(4));
        for (i = 1; i <= adults; i++)
        {
            dragons[dragonindex] = PM_GRAY_DRAGON + dragontype;
            dragonindex++;
        }
        for (i = 1; i <= hatchlings; i++)
        {
            dragons[dragonindex] = PM_GRAY_DRAGON_HATCHLING + dragontype;
            dragonindex++;
        }
    }
    for (i = 0; i < dragonindex; i++)
    {
        int trycnt = 0;
        sx = 0;
        sy = 0;
        for(trycnt = 0; trycnt < 100; trycnt++)
        {
            boolean success = somexy(sroom, &c);
            if (!success)
                continue;

            sx = c.x;
            sy = c.y;
            
            if (trycnt < 50 && !MON_AT(sx, sy) && !t_at(sx, sy))
                break;
            else if (!MON_AT(sx, sy))
                break;
        }
        if (trycnt < 100)
        {
            if(dragons[i] > 0)
            {
                (void)makemon(&mons[dragons[i]], sx, sy, !rn2(2) ? MM_ASLEEP | MM_WAITFORU : MM_WAITFORU);
            }

            struct obj* otmp = mksobj_at(GOLD_PIECE, sx, sy, FALSE, FALSE);
            if (otmp)
            {
                if(dragons[i] >= PM_GRAY_DRAGON_HATCHLING && dragons[i] <= PM_YELLOW_DRAGON_HATCHLING)
                    otmp->quan = 100 + rn2(201);
                else if (dragons[i] >= PM_GRAY_DRAGON && dragons[i] <= PM_YELLOW_DRAGON)
                    otmp->quan = 1000 + rn2(2001);
                else if (dragons[i] >= PM_ANCIENT_GRAY_DRAGON && dragons[i] <= PM_ANCIENT_YELLOW_DRAGON)
                    otmp->quan = 3000 + rn2(4001);
                otmp->owt = weight(otmp);
            }
            if (dragons[i] >= PM_ANCIENT_GRAY_DRAGON && dragons[i] <= PM_ANCIENT_YELLOW_DRAGON)
            {
                /* Ancient dragons have 5-10 true gems */
                int gemcnt = 5 + rn2(6);
                for (j = 0; j < gemcnt; j++)
                {
                    (void)mksobj_at(randomtruegem(), sx, sy, FALSE, FALSE);
                }

                /* Ancient dragons have 1-6 other items */
                int itemcnt = rnd(6);
                for (j = 0; j < itemcnt; j++)
                {
                    (void)mkobj_at(0, sx, sy, FALSE);
                }

            }
            else if (dragons[i] >= PM_GRAY_DRAGON && dragons[i] <= PM_YELLOW_DRAGON)
            {
                /* Adult dragons have 0-5 gems */
                int gemcnt = rn2(6);
                for (j = 0; j < gemcnt; j++)
                {
                    (void)mkobj_at(GEM_CLASS, sx, sy, FALSE);
                }

                /* Adult dragons have 0-3 other items */
                int itemcnt = rn2(4);
                for (j = 0; j < itemcnt; j++)
                {
                    (void)mkobj_at(0, sx, sy, FALSE);
                }
            }
            else if (dragons[i] >= PM_GRAY_DRAGON_HATCHLING && dragons[i] <= PM_YELLOW_DRAGON_HATCHLING)
            {
                /* Hatchlings dragons have 0-2 gems */
                int gemcnt = rn2(3);
                for (j = 0; j < gemcnt; j++)
                {
                    (void)mkobj_at(GEM_CLASS, sx, sy, FALSE);
                }

                /* Adult dragons have 0-1 other items */
                int itemcnt = rn2(2);
                for (j = 0; j < itemcnt; j++)
                {
                    (void)mkobj_at(0, sx, sy, FALSE);
                }
            }
        }
    }


    int roomsize = (sroom->hx - sroom->lx) * (sroom->hy - sroom->ly);
    int averagegold = roomsize > 4 ? 3000 + rn2(2000) : 2000;
    int goldpersquare = roomsize > 0 ? averagegold / roomsize : 100;
    int gemoneinchance = max(3, roomsize / 4);
    int itemoneinchance = max(4, roomsize / 3);

    for (sx = sroom->lx; sx <= sroom->hx; sx++)
    {
        for (sy = sroom->ly; sy <= sroom->hy; sy++)
        {
            if(rn2(6))
            {
                struct obj* otmp = mksobj_at(GOLD_PIECE, sx, sy, TRUE, FALSE);
                if (otmp)
                {
                    otmp->quan = 1 + rn2(2 * goldpersquare);
                    otmp->owt = weight(otmp);
                }
            }

            if(!rn2(gemoneinchance))
            {
                (void)mkobj_at(GEM_CLASS, sx, sy, FALSE);
            }

            if (!rn2(itemoneinchance))
            {
                (void)mkobj_at(RANDOM_CLASS, sx, sy, FALSE);
            }
        }
    }

    level.flags.has_dragonlair = 1;
    return 1;
}

STATIC_OVL coord *
shrine_pos(roomno)
int roomno;
{
    static coord buf;
    int delta;
    struct mkroom *troom = &rooms[roomno - ROOMOFFSET];

    /* if width and height are odd, placement will be the exact center;
       if either or both are even, center point is a hypothetical spot
       between map locations and placement will be adjacent to that */
    delta = troom->hx - troom->lx;
    buf.x = troom->lx + delta / 2;
    if ((delta % 2) && rn2(2))
        buf.x++;
    delta = troom->hy - troom->ly;
    buf.y = troom->ly + delta / 2;
    if ((delta % 2) && rn2(2))
        buf.y++;
    return &buf;
}

STATIC_OVL coord*
anvil_pos(roomno)
int roomno;
{
    static coord buf;
    int delta;
    struct mkroom* troom = &rooms[roomno - ROOMOFFSET];

    /* if width and height are odd, placement will be the exact center;
       if either or both are even, center point is a hypothetical spot
       between map locations and placement will be adjacent to that */
    delta = troom->hx - troom->lx;
    buf.x = troom->lx + delta / 2;
    if ((delta % 2) && rn2(2))
        buf.x++;
    delta = troom->hy - troom->ly;
    buf.y = troom->ly + delta / 2;
    if ((delta % 2) && rn2(2))
        buf.y++;
    return &buf;
}

STATIC_OVL int
mktemple()
{
    register struct mkroom *sroom;
    coord *shrine_spot;
    register struct rm *lev;

    if (!(sroom = pick_room(TRUE)))
        return 0;

    /* set up Priest and shrine */
    sroom->rtype = TEMPLE;
    /*
     * In temples, shrines are blessed altars
     * located in the center of the room
     */
    int x, y;
    for (x = sroom->lx; x <= sroom->hx; x++)
        for (y = sroom->ly; y <= sroom->hy; y++)
        {
            if (levl[x][y].typ == ROOM)
            {
                levl[x][y].subtyp = FLOOR_SUBTYPE_MARBLE;
                levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
            }
            else if (levl[x][y].floortyp == ROOM)
            {
                levl[x][y].floorsubtyp = FLOOR_SUBTYPE_MARBLE;
                levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
            }
        }

    /* Remove doodads */
    for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
        for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
            if (isok(x, y))
            {
                levl[x][y].floor_doodad = 0;
                delete_decoration(x, y);
            }

    /* Set room tileset */
    set_room_tileset(sroom);

     /* Altar */
    shrine_spot = shrine_pos((int) ((sroom - rooms) + ROOMOFFSET));
    lev = &levl[shrine_spot->x][shrine_spot->y];
    if (IS_FLOOR(lev->typ))
    {
        lev->floortyp = lev->typ;
        lev->floorsubtyp = lev->subtyp;
        lev->floorvartyp = lev->vartyp;
    }
    lev->typ = ALTAR;
    lev->subtyp = 0;
    lev->vartyp = 0;
    lev->special_quality = 0;
    lev->altarmask = induced_align(80);

    /* Four candelabra */
    for (int i = 0; i <= 1; i++)
    {
        for (int j = 0; j <= 1; j++)
        {
            int x1 = i == 0 ? sroom->lx : sroom->hx;
            int y1 = j == 0 ? sroom->ly : sroom->hy;
            if (isok(x1, y1) && IS_FLOOR(levl[x1][y1].typ))
            {
                struct obj* otmp = mksobj_at(LARGE_FIVE_BRANCHED_CANDELABRUM, x1, y1, TRUE, TRUE);
                if (otmp)
                {
                    otmp->special_quality = objects[otmp->otyp].oc_special_quality;
                    otmp->age = MAX_BURN_IN_CANDELABRUM;
                    otmp->owt = weight(otmp);
                    if (!otmp->lamplit)
                        begin_burn(otmp, FALSE);
                }
            }
        }
    }

    /* Priest statue */
    int lowx = sroom->lx;
    int hix = sroom->hx;
    int lowy = sroom->ly;
    int hiy = sroom->hy;
    int roll1 = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
    if (lowx + 1 < hix && rn2(5))
    {
        if (IS_WALL(levl[lowx + roll1 + 1][lowy - 1].typ))
        {
            levl[lowx + roll1 + 1][lowy - 1].decoration_typ = DECORATION_PRIEST_NICHE;
            levl[lowx + roll1 + 1][lowy - 1].decoration_subtyp = 0;
            levl[lowx + roll1 + 1][lowy - 1].decoration_dir = 0;
            levl[lowx + roll1 + 1][lowy - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
        }
    }
    int roll2 = hiy - lowy - 1 <= 1 ? 0 : rn2(hiy - lowy - 1);
    if (lowy + 1 < hiy && rn2(5))
    {
        if (IS_WALL(levl[lowx - 1][lowy + roll2 + 1].typ))
        {
            levl[lowx - 1][lowy + roll2 + 1].decoration_typ = DECORATION_PRIEST_NICHE;
            levl[lowx - 1][lowy + roll2 + 1].decoration_subtyp = 0;
            levl[lowx - 1][lowy + roll2 + 1].decoration_dir = 1;
            levl[lowx - 1][lowy + roll2 + 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
        }
    }
    int roll3 = hiy - lowy - 1 <= 1 ? 0 : rn2(hiy - lowy - 1);
    if (lowy + 1 < hiy && rn2(5))
    {
        if (IS_WALL(levl[hix + 1][lowy + roll3 + 1].typ))
        {
            levl[hix + 1][lowy + roll3 + 1].decoration_typ = DECORATION_PRIEST_NICHE;
            levl[hix + 1][lowy + roll3 + 1].decoration_subtyp = 0;
            levl[hix + 1][lowy + roll3 + 1].decoration_dir = 2;
            levl[hix + 1][lowy + roll3 + 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
        }
    }

    /* Priest */
    priestini(&u.uz, sroom, shrine_spot->x, shrine_spot->y, FALSE, NON_PM);
    lev->altarmask |= AM_SHRINE;
    level.flags.has_temple = 1;
    context.made_temple_count++;
    return 1;
}

STATIC_OVL int
mksmithy()
{
    register struct mkroom* sroom;
    coord* anvil_spot;
    register struct rm* lev;

    if (!(sroom = pick_room(TRUE)))
        return 0;

    /* set up the smith and the smithy */
    sroom->rtype = SMITHY;
    /*
     * In smithies, anvils are
     * located in the center of the room
     */
    anvil_spot = anvil_pos((int)((sroom - rooms) + ROOMOFFSET));
    lev = &levl[anvil_spot->x][anvil_spot->y];
    if (IS_FLOOR(lev->typ))
    {
        lev->floortyp = lev->typ;
        lev->floorsubtyp = lev->subtyp;
        lev->floorvartyp = lev->vartyp;
    }
    lev->typ = ANVIL;
    lev->subtyp = 0;
    lev->vartyp = 0;
    lev->special_quality = 0;
    smithini(&u.uz, sroom, anvil_spot->x, anvil_spot->y, 0, NON_PM);
    level.flags.has_smithy = 1;

    /* The smith has lights turned on */
    int x, y;
    if (!sroom->rlit) {

        for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
            for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
                levl[x][y].lit = 1;
        sroom->rlit = 1;
    }

    /* Remove doodads */
    for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
        for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
            if (isok(x, y))
            {
                levl[x][y].floor_doodad = 0;
                delete_decoration(x, y);
            }

    /* Set room tileset */
    set_room_tileset(sroom);

    /* Add a fireplace */
    schar lowx = sroom->lx;
    schar hix = sroom->hx;
    schar lowy = sroom->ly;
    int roll2 = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
    if (lowx + 1 < hix && !rn2(2))
    {
        if (IS_WALL(levl[lowx + roll2 + 1][lowy - 1].typ))
        {
            levl[lowx + roll2 + 1][lowy - 1].decoration_typ = !rn2(2) ? DECORATION_FIREPLACE : DECORATION_ANOTHER_FIREPLACE;
            levl[lowx + roll2 + 1][lowy - 1].decoration_subtyp = 0;
            levl[lowx + roll2 + 1][lowy - 1].decoration_dir = 0;
            levl[lowx + roll2 + 1][lowy - 1].decoration_flags = 0;
        }
    }

    return 1;
}


int
mknpcroom(npctyp)
int npctyp;
{
    register struct mkroom* sroom;

    if (!(sroom = pick_room(TRUE)))
        return 0;

    sroom->rtype = NPCROOM;

    schar u_depth = depth(&u.uz);
    uchar npctype = NPC_ARTIFICER;
    if (npctyp >= 0 && npctyp < MAX_NPC_SUBTYPES)
    {
        npctype = (uchar)npctyp;
    }
    else
    {
        /* Select random NPC type */
        int ok_npc_cnt = 0;
        boolean is_npc_ok[MAX_NPC_SUBTYPES] = { 0 };
        for (int i = 0; i < MAX_NPC_SUBTYPES; i++)
        {
            is_npc_ok[i] = !(npc_subtype_definitions[i].general_flags & NPC_FLAGS_NO_GENERATION);
            if (npc_subtype_definitions[i].min_appearance_depth > 0 && u_depth < npc_subtype_definitions[i].min_appearance_depth)
                is_npc_ok[i] = FALSE;
            if (npc_subtype_definitions[i].max_appearance_depth > 0 && u_depth > npc_subtype_definitions[i].max_appearance_depth)
                is_npc_ok[i] = FALSE;

            if (is_npc_ok[i])
                ok_npc_cnt++;
        }

        if (ok_npc_cnt > 0)
        {
            int cnt = -1;
            int rndidx = (ok_npc_cnt == 1 ? 1 : rn2(ok_npc_cnt));
            for (int i = 0; i < MAX_NPC_SUBTYPES; i++)
            {
                if (is_npc_ok[i])
                    cnt++;
                if (cnt == rndidx)
                {
                    npctype = i;
                    break;
                }
            }
        }
    }

    sroom->rsubtype = npctype;

    if (sroom->doorct > 0 && sroom->fdoor >= 0 && (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_DOORS_CLOSED))
    {
        for (int i = 0; i < sroom->doorct; i++)
        {
            if(sroom->fdoor + i >= DOORMAX)
                break;

            int doorx = doors[sroom->fdoor + i].x;
            int doory = doors[sroom->fdoor + i].y;
            if (isok(doorx, doory) && levl[doorx][doory].typ == DOOR)
            {
                if ((levl[doorx][doory].doormask & D_MASK) == D_NODOOR || (levl[doorx][doory].doormask & D_MASK) == D_BROKEN)
                {
                    levl[doorx][doory].doormask &= ~D_MASK;
                    levl[doorx][doory].doormask |= D_CLOSED;
                }
            }
        }
    }


    /* Change floor type */
    if (npc_subtype_definitions[npctype].general_flags & (NPC_FLAGS_PARQUET_FLOOR | NPC_FLAGS_MARBLE_FLOOR))
    {
        enum floor_subtypes lsubtype = (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_PARQUET_FLOOR) ? FLOOR_SUBTYPE_PARQUET : FLOOR_SUBTYPE_MARBLE;
        for (int x = sroom->lx; x <= sroom->hx; x++)
            for (int y = sroom->ly - 1; y <= sroom->hy; y++)
            {
                if (levl[x][y].typ == ROOM)
                {
                    levl[x][y].subtyp = lsubtype;
                    levl[x][y].vartyp = get_initial_location_vartype(levl[x][y].typ, levl[x][y].subtyp);
                }
                else if (levl[x][y].floortyp == ROOM)
                {
                    levl[x][y].floorsubtyp = lsubtype;
                    levl[x][y].floorvartyp = get_initial_location_vartype(levl[x][y].floortyp, levl[x][y].floorsubtyp);
                }
            }
    }

    npcini(&u.uz, sroom, somex(sroom), somey(sroom), npctype, NON_PM);
    level.flags.has_npc_room = 1;
    context.npc_made |= (1UL << (unsigned long)npctype);

    int x, y;
    if (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_LIGHTS_ON)
    {
        /* The NPC has lights turned on */
        if (!sroom->rlit) {

            for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
                for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
                    levl[x][y].lit = 1;
            sroom->rlit = 1;
        }
    }

    /* Remove doodads */
    for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
        for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
            if (isok(x, y))
            {
                levl[x][y].floor_doodad = 0;
                delete_decoration(x, y);
                if (room_definitions[sroom->rtype].has_special_tileset || npc_subtype_definitions[npctype].has_special_tileset)
                {
                    levl[x][y].use_special_tileset = 1;
                    levl[x][y].special_tileset = npc_subtype_definitions[npctype].has_special_tileset ? npc_subtype_definitions[npctype].special_tileset : room_definitions[sroom->rtype].special_tileset;
                }
            }

    if (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_MAY_HAVE_PAINTINGS)
    {
        /* Add a painting */
        schar lowx = sroom->lx;
        schar hix = sroom->hx;
        schar lowy = sroom->ly;
        //schar hiy = sroom->hy;
        int roll2 = hix - lowx - 1 <= 1 ? 0 : rn2(hix - lowx - 1);
        if (lowx + 1 < hix && ((npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_ALWAYS_HAS_PAINTING) || !rn2(2)))
        {
            if (IS_WALL(levl[lowx + roll2 + 1][lowy - 1].typ))
            {
                levl[lowx + roll2 + 1][lowy - 1].decoration_typ = DECORATION_PAINTING;
                levl[lowx + roll2 + 1][lowy - 1].decoration_subtyp = rn2(MAX_PAINTINGS);
                levl[lowx + roll2 + 1][lowy - 1].decoration_dir = 0;
                levl[lowx + roll2 + 1][lowy - 1].decoration_flags = DECORATION_FLAGS_ITEM_IN_HOLDER;
            }
        }
    }


    return 1;
}

void
set_room_tileset(sroom)
struct mkroom* sroom;
{
    if (!sroom)
        return;

    if (room_definitions[sroom->rtype].has_special_tileset)
    {
        int x, y;
        for (x = sroom->lx - 1; x <= sroom->hx + 1; x++)
            for (y = sroom->ly - 1; y <= sroom->hy + 1; y++)
                if (isok(x, y))
                {
                    levl[x][y].use_special_tileset = 1;
                    levl[x][y].special_tileset = room_definitions[sroom->rtype].special_tileset;
                }

    }
}

boolean
nexttodoor(sx, sy)
register int sx, sy;
{
    register int dx, dy;
    register struct rm *lev;

    for (dx = -1; dx <= 1; dx++)
        for (dy = -1; dy <= 1; dy++) {
            if (!isok(sx + dx, sy + dy))
                continue;
            lev = &levl[sx + dx][sy + dy];
            if (IS_DOOR(lev->typ) || lev->typ == SDOOR)
                return TRUE;
        }
    return FALSE;
}

boolean
nexttotree(sx, sy)
register int sx, sy;
{
    register int dx, dy;
    register struct rm* lev;

    for (dx = -1; dx <= 1; dx++)
        for (dy = -1; dy <= 1; dy++) {
            if (!isok(sx + dx, sy + dy))
                continue;
            lev = &levl[sx + dx][sy + dy];
            if (IS_TREE(lev->typ))
                return TRUE;
        }
    return FALSE;
}

boolean
has_dnstairs(sroom)
register struct mkroom *sroom;
{
    if (sroom == dnstairs_room)
        return TRUE;
    if (sstairs.sx && !sstairs.up)
        return (boolean) (sroom == sstairs_room);
    return FALSE;
}

boolean
has_upstairs(sroom)
register struct mkroom *sroom;
{
    if (sroom == upstairs_room)
        return TRUE;
    if (sstairs.sx && sstairs.up)
        return (boolean) (sroom == sstairs_room);
    return FALSE;
}

int
somex(croom)
register struct mkroom *croom;
{
    return rn1(croom->hx - croom->lx + 1, croom->lx);
}

int
somey(croom)
register struct mkroom *croom;
{
    return rn1(croom->hy - croom->ly + 1, croom->ly);
}

boolean
inside_room(croom, x, y)
struct mkroom *croom;
xchar x, y;
{
    return (boolean) (x >= croom->lx - 1 && x <= croom->hx + 1
                      && y >= croom->ly - 1 && y <= croom->hy + 1);
}

struct mkroom* which_room(x, y)
xchar x, y;
{
    struct mkroom* sroom = (struct mkroom*)0;
    struct mkroom* irregular_room = (struct mkroom*)0;

    for (int i = 0; i < nroom; i++) { /* turn up to 1 rooms gardenlike */
        sroom = &rooms[i];
        if (inside_room(sroom, x, y))
        {
            if (sroom->irregular)
            {
                irregular_room = sroom;
            }
            else
            {
                if (sroom->nsubrooms > 0)
                {
                    struct mkroom* subroom = which_room_by_list(sroom->sbrooms, sroom->nsubrooms, x, y);
                    if (subroom)
                        return subroom;
                }
            return sroom;
            }
        }
    }
    return irregular_room;
}

struct mkroom* which_room_by_list(room_list_ptr, list_length, x, y)
struct mkroom** room_list_ptr;
int list_length;
xchar x, y;
{
    if (!room_list_ptr || *room_list_ptr == (struct mkroom*)0)
        return (struct mkroom*)0;

    struct mkroom* sroom = (struct mkroom*)0;
    for (int i = 0; i < list_length; i++) { /* turn up to 1 rooms gardenlike */
        sroom = room_list_ptr[i];
        if (inside_room(sroom, x, y))
        {
            if (sroom->nsubrooms > 0)
            {
                struct mkroom* subroom = which_room_by_list(sroom->sbrooms, sroom->nsubrooms, x, y);
                if (subroom)
                    return subroom;
            }
            return sroom;
        }
    }
    return (struct mkroom*)0;
}


boolean
somexy_within_distance(croom, cpoint, distance, c)
struct mkroom* croom;
coord cpoint;
int distance;
coord* c;
{
    int try_cnt = 0;
    int i = (int)((croom - rooms) + ROOMOFFSET);

    if (!isok(cpoint.x, cpoint.y))
        return FALSE;
    if (distance < 0)
        return somexy(croom, c);

    if (distance == 0)
    {
        c->x = cpoint.x;
        c->y = cpoint.y;
        return TRUE;
    }
    else if (distance <= 10 && ((!croom->irregular && !croom->nsubrooms) || (croom->irregular)))
    {
        int x_min = max(1, cpoint.x - distance);
        int x_max = min(COLNO - 1, cpoint.x + distance);
        int y_min = max(0, cpoint.y - distance);
        int y_max = min(ROWNO - 1, cpoint.y + distance);
        int x_rand = x_max - x_min + 1;
        int y_rand = y_max - y_min + 1;
        if(x_rand <= 1 && y_rand <= 1)
        {
            c->x = cpoint.x;
            c->y = cpoint.y;
            return TRUE;
        }

        while (try_cnt++ < 100) 
        {
            c->x = x_min + (x_rand > 1 ? rn2(x_rand) : 0);
            c->y = y_min + (y_rand > 1 ? rn2(y_rand) : 0);
            if (!IS_WALL(levl[c->x][c->y].typ) && !IS_ROCK(levl[c->x][c->y].typ) && (int)levl[c->x][c->y].roomno == i &&
                ((!croom->irregular && !croom->nsubrooms) || (croom->irregular && !levl[c->x][c->y].edge))
                && dist2(cpoint.x, cpoint.y, c->x, c->y) <= distance * distance)
                return TRUE;
        }
    }

    try_cnt = 0;
    if (croom->irregular) 
    {
        while (try_cnt++ < 100) 
        {
            c->x = somex(croom);
            c->y = somey(croom);
            if (!levl[c->x][c->y].edge && (int)levl[c->x][c->y].roomno == i
                && dist2(cpoint.x, cpoint.y, c->x, c->y) <= distance * distance)
                return TRUE;
        }
        /* try harder; exhaustively search until one is found */
        for (c->x = croom->lx; c->x <= croom->hx; c->x++)
            for (c->y = croom->ly; c->y <= croom->hy; c->y++)
                if (!levl[c->x][c->y].edge
                    && (int)levl[c->x][c->y].roomno == i
                    && dist2(cpoint.x, cpoint.y, c->x, c->y) <= distance * distance)
                    return TRUE;
        return FALSE;
    }

    if (!croom->nsubrooms)
    {
        while (try_cnt++ < 100) {
            c->x = somex(croom);
            c->y = somey(croom);
            if (dist2(cpoint.x, cpoint.y, c->x, c->y) <= distance * distance)
                return TRUE;
        }
        c->x = somex(croom);
        c->y = somey(croom);
        return TRUE;
    }

    /* Check that coords doesn't fall into a subroom or into a wall */
    while (try_cnt++ < 100) 
    {
        c->x = somex(croom);
        c->y = somey(croom);
        if (IS_WALL(levl[c->x][c->y].typ) || dist2(cpoint.x, cpoint.y, c->x, c->y) > distance * distance)
            continue;
        for (int j = 0; j < croom->nsubrooms; j++)
            if (inside_room(croom->sbrooms[j], c->x, c->y))
                goto you_lose;
        break;
    you_lose:
        ;
    }
    if (try_cnt >= 100)
        return FALSE;
    return TRUE;
}

boolean
somexy(croom, c)
struct mkroom *croom;
coord *c;
{
    int try_cnt = 0;
    int i;

    if (croom->irregular)
    {
        i = (int) ((croom - rooms) + ROOMOFFSET);
        struct rm* lev;

        while (try_cnt++ < 100) {
            c->x = somex(croom);
            c->y = somey(croom);
            lev = &levl[c->x][c->y];
            if (!lev->edge && (int)lev->roomno == i)
                return TRUE;
        }
        /* try harder; exhaustively search until one is found */
        for (c->x = croom->lx; c->x <= croom->hx; c->x++)
            for (c->y = croom->ly; c->y <= croom->hy; c->y++)
            {
                lev = &levl[c->x][c->y];
                if (!lev->edge && (int)lev->roomno == i)
                    return TRUE;

            }

        return FALSE;
    }

    if (!croom->nsubrooms) {
        c->x = somex(croom);
        c->y = somey(croom);
        return TRUE;
    }

    /* Check that coords doesn't fall into a subroom or into a wall */

    while (try_cnt++ < 100) {
        c->x = somex(croom);
        c->y = somey(croom);
        if (IS_WALL(levl[c->x][c->y].typ))
            continue;
        for (i = 0; i < croom->nsubrooms; i++)
            if (inside_room(croom->sbrooms[i], c->x, c->y))
                goto you_lose;
        break;
    you_lose:
        ;
    }
    if (try_cnt >= 100)
        return FALSE;
    return TRUE;
}

/*
 * Search for a special room given its type (zoo, court, etc...)
 *      Special values :
 *              - ANY_SHOP
 *              - ANY_TYPE
 */
struct mkroom *
search_special(type)
schar type;
{
    register struct mkroom *croom;

    for (croom = &rooms[0]; croom->hx >= 0; croom++)
        if ((type == ANY_TYPE && croom->rtype != OROOM)
            || (type == ANY_SHOP && croom->rtype >= SHOPBASE)
            || croom->rtype == type)
            return croom;
    for (croom = &subrooms[0]; croom->hx >= 0; croom++)
        if ((type == ANY_TYPE && croom->rtype != OROOM)
            || (type == ANY_SHOP && croom->rtype >= SHOPBASE)
            || croom->rtype == type)
            return croom;
    return (struct mkroom *) 0;
}

struct permonst *
courtmon()
{
    int i = rn2(60) + rn2(3 * level_difficulty());

    if (i > 100)
        return mkclass(S_DRAGON, 0);
    else if (i > 95)
        return mkclass(S_GIANT, 0);
    else if (i > 85)
        return mkclass(S_TROLL, 0);
    else if (i > 75)
        return mkclass(S_CENTAUR, 0);
    else if (i > 60)
        return mkclass(S_ORC, 0);
    else if (i > 45)
        return &mons[PM_BUGBEAR];
    else if (i > 30)
        return &mons[PM_HOBGOBLIN];
    else if (i > 15)
        return mkclass(S_GNOLL, 0);
    else
        return mkclass(S_KOBOLD, 0);
}

#define NSTYPES (PM_CAPTAIN - PM_SOLDIER + 1)

STATIC_VAR const struct {
    unsigned pm;
    unsigned prob;
} squadprob[NSTYPES] = { { PM_SOLDIER, 80 },
                         { PM_SERGEANT, 15 },
                         { PM_LIEUTENANT, 4 },
                         { PM_CAPTAIN, 1 } };

/* return soldier types. */
STATIC_OVL struct permonst *
squadmon()
{
    int sel_prob, i, cpro, mndx;

    sel_prob = rnd(80 + level_difficulty());

    cpro = 0;
    for (i = 0; i < NSTYPES; i++) {
        cpro += squadprob[i].prob;
        if (cpro > sel_prob) {
            mndx = squadprob[i].pm;
            goto gotone;
        }
    }
    mndx = squadprob[rn2(NSTYPES)].pm;
gotone:
    if (!(mvitals[mndx].mvflags & MV_GONE))
        return &mons[mndx];
    else
        return (struct permonst *) 0;
}

/* return armory monster types. */
STATIC_OVL struct permonst*
armorymon(doboss)
boolean doboss;
{
    int u_depth = depth(&u.uz);
    int ldif = u_depth + (doboss ? u_depth / 2: 0);
    int mndx = 0;
    if (u_depth >= depth(&medusa_level))
    {
        return squadmon();
    }
    else if (ldif >= 13 && rn2(2))
    {
        /* Soldiers */
        mndx = PM_SOLDIER;
        if (!rn2(3) || doboss)
        {
            mndx = PM_SERGEANT;
            if(ldif >= 13 && (!rn2(2) || doboss))
                mndx = PM_LIEUTENANT;
            else if (ldif >= 16 && (!rn2(2) || doboss))
                mndx = PM_CAPTAIN;
        }
    }
    else
    {
        if (u.ualign.type == A_CHAOTIC)
        {
            /* Dwarfs and gnomes for chaotic */
            mndx = ldif >= 11 ? PM_DWARF_LORD : ldif >= 7 ? PM_DWARF : ldif >= 5 ? (!rn2(2) ? PM_DWARF : PM_GNOME) : (!rn2(2) ? PM_GNOME : PM_HALFLING);
            if (ldif <= 3 && (!rn2(6) || doboss))
                mndx = PM_DWARF;
            else if (ldif < 11 && ldif >= 6 && (!rn2(5) || doboss))
                mndx = PM_DWARF_LORD;
            else if (ldif >= 9 && (!rn2(4) || doboss))
                mndx = PM_DWARF_KING;
        }
        else
        {
            /* Goblins or gnolls for non-chaotic */
            if (!Race_if(PM_GNOLL) && (ldif >= 11 || (!rn2(2) || doboss)))
            {
                mndx = ldif >= 12 ? PM_GNOLL_LORD : ldif >= 7 ? PM_GNOLL : ldif >= 5 ? (rn2(2) ? PM_GNOLL : PM_GOBLIN) : (rn2(3) ? PM_GOBLIN : PM_GIANT_RAT);
                if (ldif <= 3 && (!rn2(6) || doboss))
                    mndx = PM_GNOLL;
                else if (ldif < 12 && ldif >= 6 && (!rn2(6) || doboss))
                    mndx = PM_GNOLL_LORD;
                else if (ldif >= 12 && (!rn2(6) || doboss))
                    mndx = PM_GNOLL_KING;
                else if (ldif >= 14 && (!rn2(6) || doboss))
                    mndx = PM_FLIND;
                else if (ldif >= 20 && (!rn2(6) || doboss))
                    mndx = PM_FLIND_LORD;
            }
            else
            {
                mndx = ldif >= 13 ? PM_OGRE : ldif >= 9 ? PM_BUGBEAR : ldif >= 7 ? PM_HOBGOBLIN : ldif >= 5 ? (rn2(2) ? PM_HOBGOBLIN : PM_GOBLIN) : (rn2(3) ? PM_GOBLIN : PM_GIANT_RAT);
                if (ldif <= 3 && (!rn2(6) || doboss))
                    mndx = PM_HOBGOBLIN;
                else if (ldif < 9 && ldif >= 5 && (!rn2(6) || doboss))
                    mndx = PM_BUGBEAR;
                else if (ldif < 13 && ldif >= 7 && (!rn2(6) || doboss))
                    mndx = PM_OGRE;
                else if (ldif >= 10 && (!rn2(6) || doboss))
                    mndx = PM_OGRE_LORD;
                else if (ldif >= 13 && (!rn2(6) || doboss))
                    mndx = PM_OGRE_OVERLORD;
                else if (ldif >= 23 && (!rn2(6) || doboss))
                    mndx = PM_OGRE_KING;
            }
        }
    }

    if (!(mvitals[mndx].mvflags & MV_GONE))
        return &mons[mndx];
    else
        return (struct permonst*)0;
}

/*
 * save_room : A recursive function that saves a room and its subrooms
 * (if any).
 */
STATIC_OVL void
save_room(fd, r)
int fd;
struct mkroom *r;
{
    short i;

    /*
     * Well, I really should write only useful information instead
     * of writing the whole structure. That is I should not write
     * the subrooms pointers, but who cares ?
     */
    bwrite(fd, (genericptr_t) r, sizeof (struct mkroom));
    for (i = 0; i < r->nsubrooms; i++)
        save_room(fd, r->sbrooms[i]);
}

/*
 * save_rooms : Save all the rooms on disk!
 */
void
save_rooms(fd)
int fd;
{
    short i;

    /* First, write the number of rooms */
    bwrite(fd, (genericptr_t) &nroom, sizeof(nroom));
    for (i = 0; i < nroom; i++)
        save_room(fd, &rooms[i]);
}

STATIC_OVL void
rest_room(fd, r)
int fd;
struct mkroom *r;
{
    short i;

    mread(fd, (genericptr_t) r, sizeof(struct mkroom));
    for (i = 0; i < r->nsubrooms; i++) {
        r->sbrooms[i] = &subrooms[nsubroom];
        rest_room(fd, &subrooms[nsubroom]);
        subrooms[nsubroom++].resident = (struct monst *) 0;
    }
}


STATIC_OVL void
reset_room(r)
struct mkroom* r;
{
    short i;

    /*
     * Well, I really should write only useful information instead
     * of writing the whole structure. That is I should not write
     * the subrooms pointers, but who cares ?
     */
    for (i = 0; i < r->nsubrooms; i++)
        reset_room(r->sbrooms[i]);

    memset((genericptr_t)r, 0, sizeof(struct mkroom));
}

void
reset_rooms(VOID_ARGS)
{
    short i;

    /* First, write the number of rooms */
    for (i = 0; i < nroom; i++)
        reset_room(&rooms[i]);

    nroom = 0;
}


/*
 * rest_rooms : That's for restoring rooms. Read the rooms structure from
 * the disk.
 */
void
rest_rooms(fd)
int fd;
{
    short i;

    mread(fd, (genericptr_t) &nroom, sizeof(nroom));
    nsubroom = 0;
    for (i = 0; i < nroom; i++) {
        rest_room(fd, &rooms[i]);
        rooms[i].resident = (struct monst *) 0;
    }
    rooms[nroom].hx = -1; /* restore ending flags */
    subrooms[nsubroom].hx = -1;
}

/* convert a display symbol for terrain into topology type;
   used for remembered terrain when mimics pose as furniture */
int
cmap_to_type(sym)
int sym;
{
    int typ = STONE; /* catchall */

    switch (sym) {
    case S_unexplored:
        typ = UNDEFINED_LOCATION;
        break;
    case S_stone:
        typ = STONE;
        break;
    case S_vwall:
        typ = VWALL;
        break;
    case S_hwall:
        typ = HWALL;
        break;
    case S_tlcorn:
        typ = TLCORNER;
        break;
    case S_trcorn:
        typ = TRCORNER;
        break;
    case S_blcorn:
        typ = BLCORNER;
        break;
    case S_brcorn:
        typ = BRCORNER;
        break;
    case S_crwall:
        typ = CROSSWALL;
        break;
    case S_tuwall:
        typ = TUWALL;
        break;
    case S_tdwall:
        typ = TDWALL;
        break;
    case S_tlwall:
        typ = TLWALL;
        break;
    case S_trwall:
        typ = TRWALL;
        break;
    case S_ndoor:  /* no door (empty doorway) */
    case S_vodoor: /* open door in vertical wall */
    case S_hodoor: /* open door in horizontal wall */
    case S_vcdoor: /* closed door in vertical wall */
    case S_hcdoor: /* closed door in horizontal wall */
    case S_vbdoor: /* broken door in vertical wall */
    case S_hbdoor: /* broken door in horizontal wall */
    case S_voportcullis: /* open portcullis in vertical wall */
    case S_hoportcullis: /* open portcullis in horizontal wall */
        typ = DOOR;
        break;
    case S_bars:
        typ = IRONBARS;
        break;
    case S_tree:
        typ = TREE;
        break;
    case S_room:
        typ = ROOM;
        break;
    case S_corr:
    case S_litcorr:
        typ = CORR;
        break;
    case S_upstair:
    case S_dnstair:
        typ = STAIRS;
        break;
    case S_upladder:
    case S_dnladder:
        typ = LADDER;
        break;
    case S_altar:
        typ = ALTAR;
        break;
    case S_anvil:
        typ = ANVIL;
        break;
    case S_grave:
        typ = GRAVE;
        break;
    case S_brazier:
        typ = BRAZIER;
        break;
    case S_signpost:
        typ = SIGNPOST;
        break;
    case S_throne:
        typ = THRONE;
        break;
    case S_sink:
        typ = SINK;
        break;
    case S_fountain:
        typ = FOUNTAIN;
        break;
    case S_pool:
        typ = POOL;
        break;
    case S_ice:
        typ = ICE;
        break;
    case S_lava:
        typ = LAVAPOOL;
        break;
    case S_vodbridge: /* open drawbridge spanning north/south */
    case S_hodbridge:
        typ = DRAWBRIDGE_DOWN;
        break;        /* east/west */
    case S_vcdbridge: /* closed drawbridge in vertical wall */
    case S_hcdbridge:
        typ = DBWALL;
        break;
    case S_air:
        typ = AIR;
        break;
    case S_cloud:
        typ = CLOUD;
        break;
    case S_water:
        typ = WATER;
        break;
    default:
        break; /* not a cmap symbol? */
    }
    return typ;
}

/*mkroom.c*/
