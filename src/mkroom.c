/* GnollHack 4.0	mkroom.c	$NHDT-Date: 1446887530 2015/11/07 09:12:10 $  $NHDT-Branch: master $:$NHDT-Revision: 1.24 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * Entry points:
 *      mkroom() -- make and stock a room of a given type
 *      nexttodoor() -- return TRUE if adjacent to a door
 *      has_dnstairs() -- return TRUE if given room has a down staircase
 *      has_upstairs() -- return TRUE if given room has an up staircase
 *      courtmon() -- generate a court monster
 *      save_rooms() -- save rooms into file fd
 *      rest_rooms() -- restore rooms from file fd
 *      cmap_to_type() -- convert S_xxx symbol to XXX topology code
 */

#include "hack.h"

STATIC_DCL boolean FDECL(isbig, (struct mkroom *));
STATIC_DCL struct mkroom *FDECL(pick_room, (BOOLEAN_P));
STATIC_DCL int NDECL(mkshop), NDECL(mkdesertedshop);
STATIC_DCL int FDECL(mkzoo, (int)), NDECL(mkswamp), NDECL(mkgarden), NDECL(mkdragonlair);
STATIC_DCL int NDECL(mktemple);
STATIC_DCL coord *FDECL(shrine_pos, (int));
STATIC_DCL struct permonst *NDECL(morguemon);
STATIC_DCL struct permonst *FDECL(librarymon, (int));
STATIC_DCL struct permonst *NDECL(squadmon);
STATIC_DCL void FDECL(save_room, (int, struct mkroom *));
STATIC_DCL void FDECL(rest_room, (int, struct mkroom *));

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
mkroom(roomtype)
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
        case SWAMP:
            return mkswamp();
            break;
		case GARDEN:
			return mkgarden();
			break;
		case TEMPLE:
			return mktemple();
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
    if (!sroom->rlit) {
        int x, y;

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

	/* set room bits before stocking the shop */
	topologize(sroom);

	/* stock the room with a shopkeeper and artifacts */
	stock_room(i, sroom, TRUE);

	context.made_deserted_shop = 1;

	/* Change back to get the right message */
	sroom->rtype = DESERTEDSHOP;
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

    if ((sroom = pick_room(FALSE)) != 0) {
        sroom->rtype = type;
        fill_zoo(sroom);
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
    struct monst *mon = makemon(&mons[pm], x, y, NO_MM_FLAGS);

    if (mon) {
        mon->msleeping = 1;
        mon->mpeaceful = 0;
        set_malign(mon);
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
	struct permonst* mainlibrarymonst;


    sh = sroom->fdoor;
    switch (type) {
    case COURT:
        if (level.flags.is_maze_lev) {
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
				librarytype = 3; //Mind flayers
		}

		if (librarytype == 0) // Gnomes
		{
			mainlibrarymonst = &mons[PM_GNOMISH_WIZARD];
		}
		else if (librarytype == 1) // Gnolls
		{
			mainlibrarymonst = &mons[PM_GNOLL_WARDEN];
		}
		else if (librarytype == 2) // Liches
		{
			if (Inhell || In_endgame(&u.uz)) {
				mainlibrarymonst = !rn2(4) ? &mons[PM_ARCH_LICH] : &mons[PM_MASTER_LICH];
			}
			else
			{
				if (hd >= 25)
					mainlibrarymonst = &mons[PM_MASTER_LICH];
				if (hd >= 13)
					mainlibrarymonst = &mons[PM_DEMILICH];
				else
					mainlibrarymonst = &mons[PM_LICH];
			}
		}
		else if (librarytype == 3) // Mindflayers
		{
			if ((hd >= 25 || Inhell || In_endgame(&u.uz)) && !rn2(3)) {
				mainlibrarymonst = &mons[PM_MASTER_MIND_FLAYER];
			}
			else
				mainlibrarymonst = &mons[PM_MIND_FLAYER];
		}
		else
			mainlibrarymonst = &mons[PM_GNOMISH_WIZARD];
		//Note in library we place only a monster in one in every 2 squares

		/* FALLTHRU: Here we place the mainlibrarymonst */
	case BEEHIVE:
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
    }

    for (sx = sroom->lx; sx <= sroom->hx; sx++)
        for (sy = sroom->ly; sy <= sroom->hy; sy++) {
            if (sroom->irregular) {
                if ((int) levl[sx][sy].roomno != rmno || levl[sx][sy].edge
                    || (sroom->doorct
                        && distmin(sx, sy, doors[sh].x, doors[sh].y) <= 1))
                    continue;
            } else if (!SPACE_POS(levl[sx][sy].typ)
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

			if (type == LIBRARY && rn2(2)) //LIBRARY gets only one monster in 2 squares, objects in every square
			{
				mon = (struct monst*) 0; //No monster
			}
			else
			{
				mon = makemon((type == COURT)
                           ? courtmon()
                           : (type == BARRACKS)
                              ? squadmon()
                              : (type == MORGUE)
                                 ? morguemon()
                              : (type == LIBRARY)
								? (sx == tx && sy == ty
									? mainlibrarymonst
									: librarymon(librarytype))
                                 : (type == BEEHIVE)
                                     ? (sx == tx && sy == ty
                                         ? &mons[PM_QUEEN_BEE]
                                         : &mons[PM_KILLER_BEE])
                                     : (type == LEPREHALL)
                                         ? &mons[PM_LEPRECHAUN]
                                         : (type == COCKNEST)
                                             ? &mons[PM_COCKATRICE]
                                             : (type == ANTHOLE)
                                                 ? antholemon()
                                                 : (struct permonst *) 0,
                          sx, sy, MM_ASLEEP);
			}

			if (mon) {
                mon->msleeping = 1;
                if (type == COURT && mon->mpeaceful) {
                    mon->mpeaceful = 0;
                    set_malign(mon);
                }
            }
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
                if (!rn2(5))
                    (void) mk_tt_object(CORPSE, sx, sy);
                if (!rn2(10)) /* lots of treasure buried with dead */
                    (void) mksobj_at((rn2(3)) ? LARGE_BOX : CHEST, sx, sy,
                                     TRUE, FALSE);
                if (!rn2(5))
                    make_grave(sx, sy, (char *) 0);
                break;
            case BEEHIVE:
                if (!rn2(3))
                    (void) mksobj_at(LUMP_OF_ROYAL_JELLY, sx, sy, TRUE,
                                     FALSE);
                break;
			case LIBRARY:
				if (!rn2(4)) //Bookshelves are fuller at higher difficulty
					(void)mksobj_at(BOOKSHELF, sx, sy, TRUE, FALSE);
				else if(!rn2(level_difficulty() >= 12 ? 3 : 4))
					(void)mkobj_at(!rn2(4) ? SPBOOK_CLASS : SCROLL_CLASS, sx, sy, FALSE);
				break;
			case BARRACKS:
                if (!rn2(20)) /* the payroll and some loot */
                    (void) mksobj_at((rn2(3)) ? LARGE_BOX : CHEST, sx, sy,
                                     TRUE, FALSE);
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
        levl[tx][ty].typ = THRONE;
        (void) somexy(sroom, &mm);
        gold = mksobj(GOLD_PIECE, TRUE, FALSE, FALSE);
        gold->quan = (long) rn1(50 * level_difficulty(), 10);
        gold->owt = weight(gold);
        /* the royal coffers */
        chest = mksobj_at(CHEST, mm.x, mm.y, TRUE, FALSE);
        add_to_container(chest, gold);
        chest->owt = weight(chest);
        chest->spe = 2; /* so it can be found later */
        level.flags.has_court = 1;
        break;
    }
    case BARRACKS:
        level.flags.has_barracks = 1;
        break;
    case ZOO:
        level.flags.has_zoo = 1;
        break;
    case MORGUE:
        level.flags.has_morgue = 1;
        break;
    case SWAMP:
        level.flags.has_swamp = 1;
        break;
    case BEEHIVE:
        level.flags.has_beehive = 1;
        break;
	case LIBRARY:
		level.flags.has_library = 1;
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

    while (cnt--) {
        mdat = morguemon();
        if (mdat && enexto(&cc, mm->x, mm->y, mdat)
            && (!revive_corpses
                || !(otmp = sobj_at(CORPSE, cc.x, cc.y))
                || !revive(otmp, FALSE, -1)))
            (void) makemon(mdat, cc.x, cc.y, mm_flags);
    }
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
				int ndemon_res = ndemon(A_NONE);
				if (ndemon_res != NON_PM)
					return &mons[ndemon_res];
				/* else do what? As is, it will drop to ghost/wraith/zombie */
			}
        }
    }

    if (hd > 8 && i > 85)
        return mkclass(S_VAMPIRE, 0);

    return ((i < 20) ? &mons[PM_GHOST]
                     : (i < 40) ? &mons[PM_WRAITH]
						: mkclass(S_LESSER_UNDEAD, 0));
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
	else if (type == 3) // Mindflayers
	{
		if ((Inhell || In_endgame(&u.uz)) && !rn2(4)) {
			return &mons[PM_MASTER_MIND_FLAYER];
		}
		else
			return &mons[PM_MIND_FLAYER];
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
    do {
        switch ((indx + trycnt) % 3) {
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
    } while (++trycnt < 3 && (mvitals[mtyp].mvflags & G_GONE));

    return ((mvitals[mtyp].mvflags & G_GONE) ? (struct permonst *) 0
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
                if (!OBJ_AT(sx, sy) && !MON_AT(sx, sy) && !t_at(sx, sy)
                    && !nexttodoor(sx, sy)) {
                    if ((sx + sy) % 2) {
                        levl[sx][sy].typ = POOL;
                        if (!eelct || !rn2(4)) {
                            /* mkclass() won't do, as we might get kraken */
                            (void) makemon(rn2(5)
                                              ? &mons[PM_GIANT_EEL]
                                              : rn2(2)
                                                 ? &mons[PM_PIRANHA]
                                                 : &mons[PM_ELECTRIC_EEL],
                                           sx, sy, NO_MM_FLAGS);
                            eelct++;
                        }
                    } else if (!rn2(4)) /* swamps tend to be moldy */
                        (void) makemon(mkclass(S_FUNGUS, 0), sx, sy,
                                       NO_MM_FLAGS);
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

	if (!sroom)
		return 0;

	/* satisfied; make a garden */
	sroom->rtype = GARDEN;
	for (sx = sroom->lx; sx <= sroom->hx; sx++)
	{
		for (sy = sroom->ly; sy <= sroom->hy; sy++)
		{
			if (!OBJ_AT(sx, sy) && !MON_AT(sx, sy) && !t_at(sx, sy)
				&& !nexttodoor(sx, sy) && !nexttotree(sx, sy) && !rn2(3))
			{
				levl[sx][sy].typ = TREE;
			}
			else
			{
				levl[sx][sy].typ = GRASS;
				/* Buried items */
				if (rn2(100) < 42) /* Half-way between !rn2(2) and !rn2(3) */
				{
					int itemtype = MANDRAKE_ROOT;
					int quan = 1;

					switch (rn2(3))
					{
					case 0:
						itemtype = MANDRAKE_ROOT;
						quan = 1 + !rn2(5) ? 1 : 0;
						break;
					case 1:
						itemtype = GINSENG_ROOT;
						quan = 1 + !rn2(3) ? 1 : 0 + !rn2(5) ? 1 : 0;
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
				if (!rn2(3))
				{
					int itemtype = SPRIG_OF_WOLFSBANE;
					int quan = 1;

					switch (rn2(18))
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
						itemtype = FLY_AGARIS;
						quan = rnd(2);
						break;
					case 12:
						itemtype = MAGIC_MUSHROOM;
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
						itemtype = rn2(4) ? CHANTERELLE : PANTHER_CAP;
						quan = 1;
						break;
					case 16:
						itemtype = rn2(4) ? APPLE : BANANA;
						quan = 1;
						break;
					case 17:
						itemtype = rn2(4) ? PEAR : POMEGRANATE;
						quan = 1;
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

				if (!rn2(8))
				{
					// Garden gnome as statue (with a conical hat)
					maketrap(sx, sy, STATUE_TRAP, !rn2(15) && level_difficulty() > 4 ? PM_GNOME_KING : !rn2(5) && level_difficulty() > 2 ? PM_GNOME_LORD : PM_GNOME, TRAPFLAG_GARDEN_GNOME_ITEMS);
				}
				else if (!rn2(8))
				{
					//Sleepy ogre
					struct permonst* pm = (level_difficulty() > 3 && !rn2(3)) || level_difficulty() > 7 ? mkclass(S_OGRE, 0) : &mons[PM_BUGBEAR];
					struct monst* mon = makemon(!pm ? (&mons[!rn2(5) && level_difficulty() > 10 ? PM_OGRE_LORD : (level_difficulty() > 3 && !rn2(3)) || level_difficulty() > 7 ? PM_OGRE : PM_BUGBEAR]) : pm, sx, sy, NO_MM_FLAGS);
					if(mon)
						mon->msleeping = 1;
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
	register int sx, sy, i;

	for (i = 0; i < nroom; i++) { /* turn up to 1 rooms into a dragon lair */
		sroom = &rooms[rn2(nroom)];
		if (sroom->hx < 0 || sroom->rtype != OROOM || has_upstairs(sroom)
			|| has_dnstairs(sroom))
			continue;
		else
			break;
	}

	if (!sroom)
		return 0;

	/* satisfied; make a dragon lair */
	sroom->rtype = DRAGONLAIR;

	coord c = { 0 };
	int dragons[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int maxdragons = (sroom->hx - sroom->lx + 1) * (sroom->hy - sroom->ly + 1);
	int dragonindex = 0;

	if (!rn2(3))
	{
		dragons[0] = PM_ANCIENT_GRAY_DRAGON + rn2(9);
	}
	else
	{
		int dragontype = rn2(9);
		int adults = min(maxdragons, rnd(2));
		int hatchlings = min(maxdragons - adults, 1 + rnd(4));
		for (int i = 1; i <= adults; i++)
		{
			dragons[dragonindex] = PM_GRAY_DRAGON + dragontype;
			dragonindex++;
		}
		for (int i = 1; i <= hatchlings; i++)
		{
			dragons[dragonindex] = PM_GRAY_DRAGON_HATCHLING + dragontype;
			dragonindex++;
		}
	}
	for (int i = 0; i < dragonindex; i++)
	{
		int trycnt = 0;
		int sx = 0;
		int sy = 0;
		for(int trycnt = 0; trycnt < 100; trycnt++)
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
				struct monst* mon = makemon(&mons[dragons[i]], sx, sy, NO_MM_FLAGS);
				if(mon)
					mon->msleeping = 1;
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
				for (int i = 0; i < gemcnt; i++)
				{
					struct obj* otmp = mksobj_at(randomtruegem(), sx, sy, FALSE, FALSE);
				}

				/* Ancient dragons have 1-6 other items */
				int itemcnt = rnd(6);
				for (int i = 0; i < itemcnt; i++)
				{
					struct obj* otmp = mkobj_at(0, sx, sy, FALSE);
				}

			}
			else if (dragons[i] >= PM_GRAY_DRAGON && dragons[i] <= PM_YELLOW_DRAGON)
			{
				/* Adult dragons have 0-5 gems */
				int gemcnt = rn2(6);
				for (int i = 0; i < gemcnt; i++)
				{
					struct obj* otmp = mkobj_at(GEM_CLASS, sx, sy, FALSE);
				}

				/* Adult dragons have 0-3 other items */
				int itemcnt = rn2(4);
				for (int i = 0; i < itemcnt; i++)
				{
					struct obj* otmp = mkobj_at(0, sx, sy, FALSE);
				}
			}
			else if (dragons[i] >= PM_GRAY_DRAGON_HATCHLING && dragons[i] <= PM_YELLOW_DRAGON_HATCHLING)
			{
				/* Hatchlings dragons have 0-2 gems */
				int gemcnt = rn2(3);
				for (int i = 0; i < gemcnt; i++)
				{
					struct obj* otmp = mkobj_at(GEM_CLASS, sx, sy, FALSE);
				}

				/* Adult dragons have 0-1 other items */
				int itemcnt = rn2(2);
				for (int i = 0; i < itemcnt; i++)
				{
					struct obj* otmp = mkobj_at(0, sx, sy, FALSE);
				}
			}
		}
	}

	for (sx = sroom->lx; sx <= sroom->hx; sx++)
	{
		for (sy = sroom->ly; sy <= sroom->hy; sy++)
		{
			if(rn2(5))
			{
				struct obj* otmp = mksobj_at(GOLD_PIECE, sx, sy, TRUE, FALSE);
				if (otmp)
				{
					otmp->quan = 5 + rn2(101);
					otmp->owt = weight(otmp);
				}
			}

			if(!rn2(3))
			{
				struct obj* otmp2 = mkobj_at(GEM_CLASS, sx, sy, FALSE);
			}

			if (!rn2(4))
			{
				struct obj* otmp2 = mkobj_at(0, sx, sy, FALSE);
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
    shrine_spot = shrine_pos((int) ((sroom - rooms) + ROOMOFFSET));
    lev = &levl[shrine_spot->x][shrine_spot->y];
    lev->typ = ALTAR;
    lev->altarmask = induced_align(80);
    priestini(&u.uz, sroom, shrine_spot->x, shrine_spot->y, FALSE);
    lev->altarmask |= AM_SHRINE;
    level.flags.has_temple = 1;
	return 1;
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
			if (lev->typ == TREE)
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

	for (int i = 0; i < nroom; i++) { /* turn up to 1 rooms gardenlike */
		sroom = &rooms[i];
		if (inside_room(sroom, x, y))
			break;
	}
	return sroom;
}

boolean
somexy(croom, c)
struct mkroom *croom;
coord *c;
{
    int try_cnt = 0;
    int i;

    if (croom->irregular) {
        i = (int) ((croom - rooms) + ROOMOFFSET);

        while (try_cnt++ < 100) {
            c->x = somex(croom);
            c->y = somey(croom);
            if (!levl[c->x][c->y].edge && (int) levl[c->x][c->y].roomno == i)
                return TRUE;
        }
        /* try harder; exhaustively search until one is found */
        for (c->x = croom->lx; c->x <= croom->hx; c->x++)
            for (c->y = croom->ly; c->y <= croom->hy; c->y++)
                if (!levl[c->x][c->y].edge
                    && (int) levl[c->x][c->y].roomno == i)
                    return TRUE;
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

static struct {
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
    if (!(mvitals[mndx].mvflags & G_GONE))
        return &mons[mndx];
    else
        return (struct permonst *) 0;
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
    case S_hcdoor:
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
    case S_grave:
        typ = GRAVE;
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
