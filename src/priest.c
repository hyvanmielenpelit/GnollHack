/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-03-17 */

/* GnollHack 4.0    priest.c    $NHDT-Date: 1545131519 2018/12/18 11:11:59 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.45 $ */
/* Copyright (c) Izchak Miller, Steve Linhart, 1989.              */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "mfndpos.h"

STATIC_DCL boolean FDECL(histemple_at, (struct monst *, XCHAR_P, XCHAR_P));
STATIC_DCL boolean FDECL(has_shrine, (struct monst *));

STATIC_DCL boolean FDECL(hissmithy_at, (struct monst*, XCHAR_P, XCHAR_P));
STATIC_DCL boolean FDECL(has_smithy, (struct monst*));

void
newepri(mtmp)
struct monst *mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();

    if (!EPRI(mtmp)) 
    {
        EPRI(mtmp) = (struct epri *) alloc(sizeof(struct epri));
        (void) memset((genericptr_t) EPRI(mtmp), 0, sizeof(struct epri));
    }
}

void
free_epri(mtmp)
struct monst *mtmp;
{
    if (mtmp->mextra && EPRI(mtmp)) 
    {
        free((genericptr_t) EPRI(mtmp));
        EPRI(mtmp) = (struct epri *) 0;
    }
    mtmp->ispriest = 0;
}

void
newesmi(mtmp)
struct monst* mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();

    if (!ESMI(mtmp)) 
    {
        ESMI(mtmp) = (struct esmi*)alloc(sizeof(struct esmi));
        (void)memset((genericptr_t)ESMI(mtmp), 0, sizeof(struct esmi));
    }
}

void
free_esmi(mtmp)
struct monst* mtmp;
{
    if (mtmp->mextra && ESMI(mtmp)) 
    {
        free((genericptr_t)ESMI(mtmp));
        ESMI(mtmp) = (struct esmi*)0;
    }
    mtmp->issmith = 0;
}


/*
 * Move for priests and shopkeepers.  Called from shk_move() and pri_move().
 * Valid returns are  1: moved  0: didn't  -1: let m_move do it  -2: died.
 */
int
move_special(mtmp, in_his_shop, appr, uondoor, avoid, omx, omy, gx, gy)
register struct monst *mtmp;
boolean in_his_shop;
schar appr;
boolean uondoor, avoid;
register xchar omx, omy, gx, gy;
{
    register xchar nx, ny, nix, niy;
    register schar i;
    schar chcnt, cnt;
    coord poss[9];
    long info[9];
    long allowflags;
#if 0 /* dead code; see below */
    struct obj *ib = (struct obj *) 0;
#endif

    if (omx == gx && omy == gy)
        return 0;

    if (is_confused(mtmp)) 
    {
        avoid = FALSE;
        appr = 0;
    }

    nix = omx;
    niy = omy;
    if (mtmp->isshk)
        allowflags = ALLOW_SSM;
    else
        allowflags = ALLOW_SSM | ALLOW_SANCT;
    if (passes_walls(mtmp->data))
        allowflags |= (ALLOW_ROCK | ALLOW_WALL);
    if (has_pitwalk(mtmp->data))
        allowflags |= ALLOW_PITS;
    if (throws_rocks(mtmp->data))
        allowflags |= ALLOW_ROCK;
    if (tunnels(mtmp->data))
        allowflags |= ALLOW_DIG;

    if (can_operate_objects(mtmp->data))
    {
        allowflags |= OPENDOOR;
        if (monhaskey(mtmp, TRUE))
            allowflags |= UNLOCKDOOR;
    }

    if (is_giant(mtmp->data))
        allowflags |= BUSTDOOR;
    cnt = mfndpos(mtmp, poss, info, allowflags);

    if (mtmp->isshk && avoid && uondoor) 
    { /* perhaps we cannot avoid him */
        for (i = 0; i < cnt; i++)
            if (!(info[i] & NOTONL))
                goto pick_move;
        avoid = FALSE;
    }

#define GDIST(x, y) (dist2(x, y, gx, gy))
pick_move:
    chcnt = 0;
    for (i = 0; i < cnt; i++) 
    {
        nx = poss[i].x;
        ny = poss[i].y;
        if (IS_ROOM(levl[nx][ny].typ)
            || (mtmp->isshk && (!in_his_shop || ESHK(mtmp)->following)))
        {
            if (avoid && (info[i] & NOTONL))
                continue;
            if ((!appr && !rn2(++chcnt))
                || (appr && GDIST(nx, ny) < GDIST(nix, niy))) 
            {
                nix = nx;
                niy = ny;
            }
        }
    }
    if (mtmp->ispriest && avoid && nix == omx && niy == omy
        && onlineu(omx, omy)) 
    {
        /* might as well move closer as long it's going to stay
         * lined up */
        avoid = FALSE;
        goto pick_move;
    }

    if (nix != omx || niy != omy) 
    {
        remove_monster(omx, omy);
        place_monster(mtmp, nix, niy);
        play_movement_sound(mtmp, CLIMBING_TYPE_NONE);
        update_m_facing(mtmp, nix - omx, FALSE);
        newsym(nix, niy);
        if (mtmp->isshk && !in_his_shop && inhishop(mtmp))
            check_special_room(FALSE);
#if 0 /* dead code; maybe someday someone will track down why... */
        if (ib) 
        {
            if (cansee(mtmp->mx, mtmp->my))
                pline("%s picks up %s.", Monnam(mtmp),
                      distant_name(ib, doname));
            obj_extract_self(ib);
            (void) mpickobj(mtmp, ib);
        }
#endif
        return 1;
    }
    return 0;
}

char
temple_occupied(array)
register char *array;
{
    register char *ptr;

    for (ptr = array; *ptr; ptr++)
        if (rooms[*ptr - ROOMOFFSET].rtype == TEMPLE)
            return *ptr;
    return '\0';
}

char
smithy_occupied(array)
register char* array;
{
    register char* ptr;

    for (ptr = array; *ptr; ptr++)
        if (rooms[*ptr - ROOMOFFSET].rtype == SMITHY)
            return *ptr;
    return '\0';
}

STATIC_OVL boolean
histemple_at(priest, x, y)
register struct monst *priest;
register xchar x, y;
{
    return (boolean) (priest && priest->ispriest
                      && (EPRI(priest)->shroom == *in_rooms(x, y, TEMPLE))
                      && on_level(&(EPRI(priest)->shrlevel), &u.uz));
}

boolean
inhistemple(priest)
struct monst *priest;
{
    /* make sure we have a priest */
    if (!priest || !priest->ispriest)
        return FALSE;
    /* priest must be on right level and in right room */
    if (!histemple_at(priest, priest->mx, priest->my))
        return FALSE;
    /* temple room must still contain properly aligned altar */
    return has_shrine(priest);
}


STATIC_OVL boolean
hissmithy_at(smith, x, y)
register struct monst* smith;
register xchar x, y;
{
    return (boolean)(smith && smith->issmith
        && (ESMI(smith)->smithy_room == *in_rooms(x, y, SMITHY))
        && on_level(&(ESMI(smith)->smithy_level), &u.uz));
}

boolean
inhissmithy(smith)
struct monst* smith;
{
    /* make sure we have a priest */
    if (!smith || !smith->issmith)
        return FALSE;
    /* priest must be on right level and in right room */
    if (!hissmithy_at(smith, smith->mx, smith->my))
        return FALSE;
    /* temple room must still contain properly aligned altar */
    return has_smithy(smith);
}


/*
 * pri_move: return 1: moved  0: didn't  -1: let m_move do it  -2: died
 */
int
pri_move(priest)
register struct monst *priest;
{
    register xchar gx, gy, omx, omy;
    schar temple;
    boolean avoid = TRUE;

    omx = priest->mx;
    omy = priest->my;

    if (!histemple_at(priest, omx, omy))
        return -1;

    temple = EPRI(priest)->shroom;

    gx = EPRI(priest)->shrpos.x;
    gy = EPRI(priest)->shrpos.y;

    gx += rn1(3, -1); /* mill around the altar */
    gy += rn1(3, -1);

    if (!is_peaceful(priest) || is_crazed(priest)
        || (Conflict && !check_ability_resistance_success(priest, A_WIS, 0))) 
    {
        if (monnear(priest, u.ux, u.uy)) 
        {
            if (Displaced)
                Your("displaced image doesn't fool %s!", mon_nam(priest));
            (void) mattacku(priest);
            return 0;
        } 
        else if (index(u.urooms, temple)) 
        {
            /* chase player if inside temple & can see him */
            if (!is_blinded(priest) && m_canseeu(priest)) 
            {
                gx = u.ux;
                gy = u.uy;
            }
            avoid = FALSE;
        }
    }
    else if (Invis)
        avoid = FALSE;

    return move_special(priest, FALSE, TRUE, FALSE, avoid, omx, omy, gx, gy);
}

/* exclusively for mktemple() */
void
priestini(lvl, sroom, sx, sy, sanctum, mtype)
d_level *lvl;
struct mkroom *sroom;
int sx, sy;
boolean sanctum; /* is it the seat of the high priest? */
int mtype;
{
    struct monst *priest;
    struct obj *otmp;

    if (MON_AT(sx + 1, sy))
        (void) rloc(m_at(sx + 1, sy), FALSE); /* insurance */

    int montype = sanctum ? PM_HIGH_PRIEST : PM_ALIGNED_PRIEST;
    if (mtype > NON_PM && mtype < NUM_MONSTERS && !(mvitals[mtype].mvflags & MV_GONE))
    {
        montype = mtype;
    }
    else if (sroom->resident_mtype > 0 && sroom->resident_mtype < NUM_MONSTERS && !(mvitals[sroom->resident_mtype].mvflags & MV_GONE))
    {
        montype = sroom->resident_mtype;
    }
    else
    {
        /* Nothing here currently */
    }

    priest = makemon(&mons[montype], sx + 1, sy, MM_EPRI);
    if(!priest)
        priest = makemon(&mons[sanctum ? PM_HIGH_PRIEST : PM_ALIGNED_PRIEST], sx + 1, sy, MM_EPRI);

    if (priest) 
    {
        EPRI(priest)->shroom = (schar) ((sroom - rooms) + ROOMOFFSET);
        EPRI(priest)->shralign = Amask2align(levl[sx][sy].altarmask);
        EPRI(priest)->shrpos.x = sx;
        EPRI(priest)->shrpos.y = sy;
        assign_level(&(EPRI(priest)->shrlevel), lvl);
        priest->mtrapseen = ~0; /* traps are known */
        priest->ispriest = 1;
        priest->isminion = 0;
        priest->msleeping = 0;
        set_mhostility(priest); /* mpeaceful may have changed */

        /* now his/her goodies... */
        if (montype == PM_HIGH_PRIEST)
        {
            otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, priest, MAT_NONE, MANUAL_MANUAL_OF_THE_PLANES, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
            if (otmp)
                (void)mpickobj(priest, otmp);
        }

        if (sanctum && EPRI(priest)->shralign == A_NONE
            && on_level(&sanctum_level, &u.uz)) 
        {
            priest->mpeaceful = 0;
            (void) mongets(priest, AMULET_OF_YENDOR);

            otmp = mongets(priest, YELLOW_DRAGON_SCALE_MAIL);
            otmp->enchantment = max(otmp->enchantment, rnd(3) + 2);
            otmp = mongets(priest, BRACERS_OF_REFLECTION);
            otmp->enchantment = max(otmp->enchantment, rnd(3) + 1);
            otmp = mongets(priest, CLOAK_OF_MAGIC_RESISTANCE);
            otmp->enchantment = max(otmp->enchantment, rnd(2) + 1);
            otmp = mongets(priest, SPEED_BOOTS);
            otmp->enchantment = max(otmp->enchantment, rnd(2) + 2);
            otmp = mongets(priest, IOUN_STONE_OF_CONSTITUTION);
            otmp->enchantment = max(otmp->enchantment, rnd(2) + 2);

            m_dowear(priest, TRUE);
        }
        else
        {
            priest->mpeaceful = 1;

            if (montype != PM_HIGH_PRIEST)
            {
                otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, priest, MAT_NONE, MANUAL_GUIDE_TO_ALTARS_AND_SACRIFICE, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
                if (otmp)
                    (void)mpickobj(priest, otmp);

                otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, priest, MAT_NONE, MANUAL_GUIDE_TO_PRAYING, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
                if (otmp)
                    (void)mpickobj(priest, otmp);

                otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, priest, MAT_NONE, MANUAL_CATALOGUE_OF_CLERICAL_SPELLS, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
                if (otmp)
                    (void)mpickobj(priest, otmp);

                if (context.game_difficulty < 0)
                {
                    otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, priest, MAT_NONE, MANUAL_INTRODUCTION_TO_BLESSED_AND_CURSED_ITEMS, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
                    if (otmp)
                        (void)mpickobj(priest, otmp);
                }

                /* Reagents */
                int cnt = 10 + rnd(10);
                int i;
                for (i = 0; i < cnt; i++)
                {
                    otmp = mksobj(random_reagent_otyp(TRUE, TRUE, 1), FALSE, FALSE, FALSE);
                    if (otmp)
                        (void)mpickobj(priest, otmp);
                }
            }
        }

        /* robe [via makemon()] */
        if (rn2(2) && (otmp = which_armor(priest, W_ARMC)) != 0) 
        {
            if (p_coaligned(priest))
                uncurse(otmp);
            else
                curse(otmp);
        }
        newsym(priest->mx, priest->my);
    }
}

/*
 * smith_move: return 1: moved  0: didn't  -1: let m_move do it  -2: died
 */
int
smith_move(smith)
register struct monst* smith;
{
    register xchar gx, gy, omx, omy;
    schar smithy;
    boolean avoid = TRUE;

    omx = smith->mx;
    omy = smith->my;

    if (!hissmithy_at(smith, omx, omy))
        return -1;

    smithy = ESMI(smith)->smithy_room;

    gx = ESMI(smith)->anvil_pos.x;
    gy = ESMI(smith)->anvil_pos.y;

    gx += rn1(3, -1); /* mill around the anvil */
    gy += rn1(3, -1);

    if (!is_peaceful(smith) || is_crazed(smith)
        || (Conflict && !check_ability_resistance_success(smith, A_WIS, 0))) 
    {
        if (monnear(smith, u.ux, u.uy)) 
        {
            if (Displaced)
                Your("displaced image doesn't fool %s!", mon_nam(smith));
            (void)mattacku(smith);
            return 0;
        }
        else if (index(u.urooms, smithy))
        {
            /* chase player if inside temple & can see him */
            if (!is_blinded(smith) && m_canseeu(smith)) 
            {
                gx = u.ux;
                gy = u.uy;
            }
            avoid = FALSE;
        }
    }
    else if (Invis)
        avoid = FALSE;

    return move_special(smith, FALSE, TRUE, FALSE, avoid, omx, omy, gx, gy);
}

/* exclusively for mksmithy() */
void
smithini(lvl, sroom, sx, sy, smithtype, mtype)
d_level* lvl;
struct mkroom* sroom;
int sx, sy;
uchar smithtype;
int mtype;
{
    struct monst* smith;
    int smith_loc_x = sx + 1;
    int smith_loc_y = sy ;
    if (!isok(smith_loc_x, smith_loc_y) || IS_ROCK(levl[smith_loc_x][smith_loc_y].typ))
    {
        smith_loc_x = sx - 1;
        smith_loc_y = sy;
    }
    if (!isok(smith_loc_x, smith_loc_y) || IS_ROCK(levl[smith_loc_x][smith_loc_y].typ))
    {
        smith_loc_x = sx;
        smith_loc_y = sy - 1;
    }
    if (!isok(smith_loc_x, smith_loc_y) || IS_ROCK(levl[smith_loc_x][smith_loc_y].typ))
    {
        smith_loc_x = sx;
        smith_loc_y = sy + 1;
    }
    if (!isok(smith_loc_x, smith_loc_y) || IS_ROCK(levl[smith_loc_x][smith_loc_y].typ))
    {
        smith_loc_x = sx;
        smith_loc_y = sy;
    }

    if (MON_AT(smith_loc_x, smith_loc_y))
        (void)rloc(m_at(smith_loc_x, smith_loc_y), FALSE); /* insurance */

    int smith_montype = PM_SMITH;
    if (mtype > NON_PM && mtype < NUM_MONSTERS && !(mvitals[mtype].mvflags & MV_GONE))
    {
        smith_montype = mtype;
    }
    else if (sroom->resident_mtype > 0 && sroom->resident_mtype < NUM_MONSTERS && !(mvitals[sroom->resident_mtype].mvflags & MV_GONE))
    {
        smith_montype = sroom->resident_mtype;
    }
    else
    {
        /* Nothing here */
    }

    unsigned long extraflags = Inhell ? MM_MALE : 0UL; /* Since there is only one soundset for unusual creature types */

    smith = makemon(&mons[smith_montype], smith_loc_x, smith_loc_y, MM_ESMI | extraflags);
    if(!smith)
        smith = makemon(&mons[PM_SMITH], smith_loc_x, smith_loc_y, MM_ESMI | extraflags); /* Fallback */

    if (smith) 
    {
        ESMI(smith)->smithy_room = (schar)((sroom - rooms) + ROOMOFFSET);
        ESMI(smith)->smith_typ=  smithtype;
        ESMI(smith)->anvil_pos.x = sx;
        ESMI(smith)->anvil_pos.y = sy;
        assign_level(&(ESMI(smith)->smithy_level), lvl);
        smith->mtrapseen = ~0; /* traps are known */
        smith->mpeaceful = 1;
        smith->issmith = 1;
        smith->msleeping = 0;
        set_mhostility(smith); /* mpeaceful may have changed */

        (void)mongetsgold(smith, 3000L + (long)rn2(6) * 500L);
        struct obj* otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, smith, MAT_NONE, MANUAL_GUIDE_TO_DRAGON_SCALE_MAILS, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
        if (otmp)
            (void)mpickobj(smith, otmp);

        otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, smith, MAT_NONE, MANUAL_CATALOGUE_OF_WEAPONS, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
        if (otmp)
            (void)mpickobj(smith, otmp);

        otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, smith, MAT_NONE, MANUAL_CATALOGUE_OF_ARMOR, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
        if (otmp)
            (void)mpickobj(smith, otmp);

        if (context.game_difficulty < 0)
        {
            otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, smith, MAT_NONE, MANUAL_ARMOR_101, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
            if (otmp)
                (void)mpickobj(smith, otmp);
            otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, smith, MAT_NONE, MANUAL_WEAPONS_101, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
            if (otmp)
                (void)mpickobj(smith, otmp);
        }
    }
}

/* get a monster's alignment type without caller needing EPRI & EMIN */
aligntyp
mon_aligntyp(mon)
struct monst *mon;
{
    aligntyp algn = mon->ispriest && has_epri(mon) ? EPRI(mon)->shralign
                                  : mon->isminion && has_emin(mon) ? EMIN(mon)->min_align
                                                  : mon->data->maligntyp;

    if (algn == A_NONE)
        return A_NONE; /* negative but differs from chaotic */
    return (algn > 0) ? A_LAWFUL : (algn < 0) ? A_CHAOTIC : A_NEUTRAL;
}

/*
 * Specially aligned monsters are named specially.
 *      - aligned priests with ispriest and high priests have shrines
 *              they retain ispriest and epri when polymorphed
 *      - aligned priests without ispriest are roamers
 *              they have isminion set and use emin rather than epri
 *      - minions do not have ispriest but have isminion and emin
 *      - caller needs to inhibit Hallucination if it wants to force
 *              the true name even when under that influence
 */
char *
priestname(mon, pname)
register struct monst *mon;
char *pname; /* caller-supplied output buffer */
{
    boolean do_hallu = Hallucination,
            aligned_priest = mon->data == &mons[PM_ALIGNED_PRIEST],
            high_priest = mon->data == &mons[PM_HIGH_PRIEST];
    char whatcode = '\0';
    const char *what = do_hallu ? rndmonnam(&whatcode) : mon_monster_name(mon);

    if (!mon->ispriest && !mon->isminion) /* should never happen...  */
        return strcpy(pname, what);       /* caller must be confused */

    *pname = '\0';
    if (!do_hallu || !bogon_is_pname(whatcode))
        Strcat(pname, "the ");
    if (is_invisible(mon))
        Strcat(pname, "invisible ");
    if (mon->isminion && EMIN(mon)->renegade)
        Strcat(pname, "renegade ");

    if (mon->ispriest || aligned_priest) 
    { /* high_priest implies ispriest */
        if (!aligned_priest && !high_priest) 
        {
            ; /* polymorphed priest; use ``what'' as is */
        } 
        else 
        {
            if (high_priest)
                Strcat(pname, "high ");
            if (Hallucination)
                what = "poohbah";
            else if (mon->female)
                what = "priestess";
            else
                what = "priest";
        }
    } 
    else 
    {
        if (is_tame(mon) && !strcmpi(what, "Angel"))
            Strcat(pname, "guardian ");
    }

    Strcat(pname, what);
    /* same as distant_monnam(), more or less... */
    if (do_hallu || !high_priest || !Is_astralevel(&u.uz)
        || distu(mon->mx, mon->my) <= 2 || program_state.gameover) 
    {
        Strcat(pname, " of ");
        Strcat(pname, halu_gname(mon_aligntyp(mon)));
    }
    return pname;
}

boolean
p_coaligned(priest)
struct monst *priest;
{
    return (boolean) (u.ualign.type == mon_aligntyp(priest));
}

STATIC_OVL boolean
has_shrine(pri)
struct monst *pri;
{
    struct rm *lev;
    struct epri *epri_p;

    if (!pri || !pri->ispriest)
        return FALSE;
    epri_p = EPRI(pri);
    lev = &levl[epri_p->shrpos.x][epri_p->shrpos.y];
    if (!IS_ALTAR(lev->typ) || !(lev->altarmask & AM_SHRINE))
        return FALSE;
    return (boolean) (epri_p->shralign
                      == (Amask2align(lev->altarmask & ~AM_SHRINE)));
}

struct monst *
findpriest(roomno)
char roomno;
{
    register struct monst *mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
    {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->ispriest && (EPRI(mtmp)->shroom == roomno)
            && histemple_at(mtmp, mtmp->mx, mtmp->my))
            return mtmp;
    }
    return (struct monst *) 0;
}

STATIC_OVL boolean
has_smithy(smith)
struct monst* smith;
{
    struct rm* lev;
    struct esmi* esmi_p;

    if (!smith || !smith->issmith)
        return FALSE;

    esmi_p = ESMI(smith);
    lev = &levl[esmi_p->anvil_pos.x][esmi_p->anvil_pos.y];

    if (!IS_ANVIL(lev->typ))
        return FALSE;

    return TRUE;
}

struct monst*
findsmith(roomno)
char roomno;
{
    register struct monst* mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
    {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->issmith && (ESMI(mtmp)->smithy_room == roomno)
            && hissmithy_at(mtmp, mtmp->mx, mtmp->my))
            return mtmp;
    }
    return (struct monst*)0;
}

/* called from check_special_room() when the player enters the temple room */
void
intemple(roomno)
int roomno;
{
    struct monst *priest, *mtmp;
    struct epri *epri_p;
    boolean shrined, sanctum, can_speak;
    long *this_time, *other_time;
    const char *msg1, *msg2;
    int msg1color = NO_COLOR, msg2color = NO_COLOR;
    char buf[BUFSZ];

    /* don't do anything if hero is already in the room */
    if (temple_occupied(u.urooms0))
        return;

    if (!u.uachieve.entered_temple)
    {
        //achievement_gained("Entered a Temple");
        u.uachieve.entered_temple = 1;
    }

    if ((priest = findpriest((char) roomno)) != 0) 
    {
        /* tended */

        epri_p = EPRI(priest);
        shrined = has_shrine(priest);
        sanctum = (priest->data == &mons[PM_HIGH_PRIEST]
                   && (Is_sanctum(&u.uz) || In_endgame(&u.uz)));
        can_speak = (mon_can_move(priest));
        enum priest_special_dialogue_lines spdl_id1 = PRIEST_SPECIAL_DIALOGUE_NONE;
        enum priest_special_dialogue_lines spdl_id2 = PRIEST_SPECIAL_DIALOGUE_NONE;
        if (can_speak && !Deaf && moves >= epri_p->intone_time)
        {
            unsigned save_priest = priest->ispriest;

            /* don't reveal the altar's owner upon temple entry in
               the endgame; for the Sanctum, the next message names
               Moloch so suppress the "of Moloch" for him here too */
            if (sanctum && !Hallucination)
                priest->ispriest = 0;
            pline("%s intones:",
                  canseemon(priest) ? Monnam(priest) : "A nearby voice");
            priest->ispriest = save_priest;
            epri_p->intone_time = moves + (long) d(10, 500); /* ~2505 */
            /* make sure that we don't suppress entry message when
               we've just given its "priest intones" introduction */
            epri_p->enter_time = 0L;
        }
        msg1 = msg2 = 0;
        if (sanctum && Is_sanctum(&u.uz)) 
        {
            if (is_peaceful(priest)) 
            {
                /* first time inside */
                msg1 = "Infidel, you have entered Moloch's Sanctum!";
                msg2 = "Be gone!";
                spdl_id1 = PRIEST_SPECIAL_DIALOGUE_HIGH_ENTERED_SANCTUM;
                spdl_id2 = PRIEST_SPECIAL_DIALOGUE_HIGH_BE_GONE;
                msg1color = CLR_MSG_TALK_ANGRY;
                msg2color = CLR_MSG_TALK_ANGRY;
                priest->mpeaceful = 0;
                /* became angry voluntarily; no penalty for attacking him */
                set_mhostility(priest);
            } 
            else 
            {
                /* repeat visit, or attacked priest before entering */
                msg1 = "You desecrate this place by your presence!";
                spdl_id1 = PRIEST_SPECIAL_DIALOGUE_HIGH_DESECRATE_PLACE_BY_YOUR_PRESENCE;
                msg1color = CLR_MSG_TALK_ANGRY;
            }
        } 
        else if (moves >= epri_p->enter_time) 
        {
            Sprintf(buf, "Pilgrim, you enter a %s place!",
                    !shrined ? "desecrated" : "sacred");
            msg1 = buf;
            spdl_id1 = !shrined ? PRIEST_SPECIAL_DIALOGUE_PILGRIM_ENTER_DESECRATED_PLACE : PRIEST_SPECIAL_DIALOGUE_PILGRIM_ENTER_SACRED_PLACE;
            msg1color = CLR_MSG_TALK;
        }

        if (msg1 && can_speak && !Deaf)
        {
            context.global_minimum_volume = 0.50;
            play_monster_special_dialogue_line(priest, spdl_id1);
            verbalize_ex1(ATR_NONE, msg1color, msg1);
            if (msg2)
            {
                play_monster_special_dialogue_line(priest, spdl_id2);
                verbalize_ex1(ATR_NONE, msg2color, msg2);
            }
            epri_p->enter_time = moves + (long) d(10, 100); /* ~505 */
            context.global_minimum_volume = 0.0;
        }

        if (!sanctum) 
        {
            if (!shrined || !p_coaligned(priest)
                || u.ualign.record <= ALGN_SINNED) 
            {
                msg1 = "have a%s forbidding feeling...";
                msg2 = (!shrined || !p_coaligned(priest)) ? "" : " strange";
                this_time = &epri_p->hostile_time;
                other_time = &epri_p->peaceful_time;
            } 
            else
            {
                msg1 = "experience %s sense of peace.";
                msg2 = (u.ualign.record >= ALGN_PIOUS) ? "a" : "an unusual";
                this_time = &epri_p->peaceful_time;
                other_time = &epri_p->hostile_time;
            }

            /* give message if we haven't seen it recently or
               if alignment update has caused it to switch from
               forbidding to sense-of-peace or vice versa */
            if (moves >= *this_time || *other_time >= *this_time) 
            {
                You_ex(ATR_NONE, CLR_MSG_ATTENTION, msg1, msg2);
                *this_time = moves + (long) d(10, 20); /* ~55 */
                /* avoid being tricked by the RNG:  switch might have just
                   happened and previous random threshold could be larger */
                if (*this_time <= *other_time)
                    *other_time = *this_time - 1L;
            }
        }
        /* recognize the Valley of the Dead and Moloch's Sanctum
           once hero has encountered the temple priest on those levels */
        mapseen_temple(priest);
    }
    else if (!In_quest(&u.uz)) /* Some quest levels have specially occupied temples, so let's not do this (in fact there should be a special handler for those) */
    {
        /* untended */

        switch (rn2(4))
        {
        case 0:
            You_ex(ATR_NONE, CLR_MSG_WARNING, "have an eerie feeling...");
            break;
        case 1:
            You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "like you are being watched.");
            break;
        case 2:
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "A shiver runs down your %s.", body_part(SPINE));
            break;
        default:
            break; /* no message; unfortunately there's no
                      EPRI(priest)->eerie_time available to
                      make sure we give one the first time */
        }
        if (!rn2(5)
            && (mtmp = makemon(&mons[PM_GHOST], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_IN_SMOKE_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END))
                   != 0) 
        {
            int ngen = mvitals[PM_GHOST].born;
            play_sfx_sound(SFX_SURPRISE_ATTACK);
            if (canspotmon(mtmp))
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "A%s ghost appears next to you%c",
                      ngen < 5 ? "n enormous" : "",
                      ngen < 10 ? '!' : '.');
            else
                You_ex(ATR_NONE, CLR_MSG_WARNING, "sense a presence close by!");
            mtmp->mpeaceful = 0;
            set_mhostility(mtmp);
            newsym(mtmp->mx, mtmp->my);
            if (flags.verbose)
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are frightened to death, and unable to move.");
            nomul(-3);
            multi_reason = "being terrified of a ghost";
            nomovemsg = "You regain your composure.";
        }
    }
}

/* reset the move counters used to limit temple entry feedback;
   leaving the level and then returning yields a fresh start */
void
forget_temple_entry(priest)
struct monst *priest;
{
    struct epri *epri_p = priest->ispriest ? EPRI(priest) : 0;

    if (!epri_p) 
    {
        impossible("attempting to manipulate shrine data for non-priest?");
        return;
    }

    epri_p->intone_time = epri_p->enter_time = epri_p->peaceful_time =
        epri_p->hostile_time = 0L;
}

void
forget_smithy_entry(smith)
struct monst* smith;
{
    struct esmi* esmi_p = smith->issmith ? ESMI(smith) : 0;

    if (!esmi_p) 
    {
        impossible("attempting to manipulate smithy data for non-smith?");
        return;
    }

    esmi_p->intone_time = esmi_p->enter_time = esmi_p->peaceful_time =
        esmi_p->hostile_time = 0L;
}

/* called from check_special_room() when the player enters the smithy */
void
insmithy(roomno)
int roomno;
{
    struct monst* smith;
    struct esmi* esmi_p;
    boolean smithied, can_speak;
    const char* msg1, * msg2;
    char buf[BUFSZ];

    /* don't do anything if hero is already in the room */
    if (smithy_occupied(u.urooms0))
        return;

    if ((smith = findsmith((char)roomno)) != 0) 
    {
        /* tended */

        esmi_p = ESMI(smith);
        smithied = has_smithy(smith);
        can_speak = (mon_can_move(smith));
        if (can_speak && !Deaf && moves >= esmi_p->intone_time)
        {
            pline("%s says:",
                canseemon(smith) ? Monnam(smith) : "A nearby voice");
            esmi_p->intone_time = moves + (long)d(10, 500); /* ~2505 */
            esmi_p->enter_time = 0L;
        }
        msg1 = msg2 = 0;

        if (moves >= esmi_p->enter_time) 
        {
            context.global_minimum_volume = 0.5;
            play_monster_special_dialogue_line(smith, !smithied ? SMITH_LINE_ADVENTURER_WELCOME : SMITH_LINE_ADVENTURER_WELCOME_TO_THE_SMITHY);
            context.global_minimum_volume = 0.0;
            Sprintf(buf, "Adventurer, %s!",
                !smithied ? "welcome" : "welcome to the smithy");

            msg1 = buf;
        }

        if (msg1 && can_speak && !Deaf) 
        {
            verbalize_ex1(ATR_NONE, CLR_MSG_TALK, msg1);
            if (msg2)
                verbalize_ex1(ATR_NONE, CLR_MSG_TALK, msg2);
            esmi_p->enter_time = moves + (long)d(10, 100); /* ~505 */
        }
    }
    else 
    {
        /* untended */
        switch (rn2(4))
        {
        case 0:
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "have an oddly wishful feeling...");
            break;
        case 1:
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "something is out of place.");
            break;
        case 2:
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "strangely worried.");
            break;
        default:
            break; 
        }
    }
}



void
priest_talk(priest)
register struct monst *priest;
{
    boolean coaligned = p_coaligned(priest);
    boolean strayed = (u.ualign.record < 0);

    /* KMH, conduct */
    u.uconduct.gnostic++;

    if (is_fleeing(priest) || (!priest->ispriest && coaligned && strayed)) 
    {
        pline("%s doesn't want anything to do with you!", Monnam(priest));
        priest->mpeaceful = 0;
        return;
    }

    /* priests don't chat unless peaceful and in their own temple */
    if (!inhistemple(priest) || !is_peaceful(priest)
        || !mon_can_move(priest)) 
    {
        static const char *cranky_msg[3] = {
            "Thou wouldst have words, eh?  I'll give thee a word or two!",
            "Talk?  Here is what I have to say!",
            "Pilgrim, I would speak no longer with thee."
        };

        if (!mon_can_move(priest)) 
        {
            pline("%s breaks out of %s reverie!", Monnam(priest),
                  mhis(priest));
            priest->mfrozen = priest->msleeping = 0;
            priest->mcanmove = 1;
            refresh_m_tile_gui_info(priest, TRUE);
        }
        int roll = rn2(3);
        priest->mpeaceful = 0;
        play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_WORD_OR_TWO + roll);
        verbalize1(cranky_msg[roll]);
        return;
    }

    /* you desecrated the temple and now you want to chat? */
    if (is_peaceful(priest) && *in_rooms(priest->mx, priest->my, TEMPLE)
        && !has_shrine(priest)) 
    {
        play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_BEGONE_DESECRATE_HOLY_PLACE);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY,
              "Begone!  Thou desecratest this holy place with thy presence.");
        priest->mpeaceful = 0;
        return;
    }

    if (!money_cnt(invent)) 
    {
        if (coaligned && !strayed) 
        {
            long pmoney = money_cnt(priest->minvent);
            if (pmoney > 0L) 
            {
                play_monster_special_dialogue_line(priest, (pmoney == 1L) ? PRIEST_SPECIAL_DIALOGUE_TAKE_THIS_HAVE_ALE : PRIEST_SPECIAL_DIALOGUE_TAKE_THESE_HAVE_ALE);
                /* Note: two bits is actually 25 cents.  Hmm. */
                pline("%s gives you %s for an ale. (A ginger ale, to be exact.)", Monnam(priest),
                      (pmoney == 1L) ? "one bit" : "two bits");
                money2u(priest, pmoney > 1L ? 2 : 1);
                bot();
            }
            else
            {
                play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_VIRTUES_OF_POVERTY);
                pline("%s preaches the virtues of poverty.", Monnam(priest));
            }

            exercise(A_WIS, TRUE);
        } 
        else
            pline("%s is not interested.", Monnam(priest));

        return;
    } 
    else 
    {
        long offer;

        play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_CONSIDER_CONTRIBUTION);
        pline("%s asks you for a contribution for the temple.",
              Monnam(priest));
        if ((offer = bribe(priest)) == 0) 
        {
            play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_REGRET_ACTION);
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "Thou shalt regret thine action!");
            if (coaligned)
                adjalign(-1);
        }
        else if (offer < (u.ulevel * 200))
        {
            if (money_cnt(invent) > (offer * 2L)) 
            {
                play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_CHEAPSKATE);
                verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "Cheapskate.");
            } 
            else 
            {
                play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_THANK_FOR_CONTRIBUTION);
                verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "I thank thee for thy contribution.");
                /* give player some token */
                exercise(A_WIS, TRUE);
            }
        } 
        else if (offer < (u.ulevel * 400)) 
        {
            play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_PIOUS_INDIVIDUAL);
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "Thou art indeed a pious individual.");
            if (money_cnt(invent) < (offer * 2L)) {
                if (coaligned && u.ualign.record <= ALGN_SINNED)
                    adjalign(1);
                play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_BESTOW_BLESSING);
                verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "I bestow upon thee a blessing.");
                incr_itimeout(&HClairvoyant, rn1(500, 500));
                refresh_u_tile_gui_info(TRUE);
            }
        }
        else if (offer < (u.ulevel * 600)
                   /* u.ublessed is only active when Protection is
                      enabled via something other than worn gear
                      (theft by gremlin clears the intrinsic but not
                      its former magnitude, making it recoverable) */
                   && (u.ublessed == 0
                       || (u.ublessed < 12
                           && (u.ublessed < 8 || !rn2(u.ublessed)))))
        {
            play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_DEVOTION_REWARDED);
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "Thy devotion has been rewarded.");
            if (u.ublessed == 0)
                u.ublessed = rnd(3);
            else
                u.ublessed++;
        }
        else
        {
            play_monster_special_dialogue_line(priest, PRIEST_SPECIAL_DIALOGUE_GENEROSITY_APPRECIATED);
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "Thy selfless generosity is deeply appreciated.");
            if (money_cnt(invent) < (offer * 2L) && coaligned)
            {
                if (strayed && (moves - u.ucleansed) > 5000L) 
                {
                    u.ualign.record = 0; /* cleanse thee */
                    u.ucleansed = moves;
                } 
                else 
                {
                    adjalign(2);
                }
            }
        }
    }
    find_ac();
    find_mc();
    context.botl = 1;
}

struct monst *
mk_roamer(ptr, alignment, x, y, peaceful)
register struct permonst *ptr;
aligntyp alignment;
xchar x, y;
boolean peaceful;
{
    register struct monst *roamer;
    //register boolean coaligned = (u.ualign.type == alignment);

#if 0 /* this was due to permonst's pxlth field which is now gone */
    if (ptr != &mons[PM_ALIGNED_PRIEST] && ptr != &mons[PM_ANGEL])
        return (struct monst *) 0;
#endif

    if (MON_AT(x, y))
        (void) rloc(m_at(x, y), FALSE); /* insurance */

    if (!(roamer = makemon_limited(ptr, x, y, MM_ADJACENTOK | MM_EMIN | MM_ROAMER | (peaceful ? MM_PEACEFUL : MM_ANGRY), 0UL, 0, 0, 0, 0, alignment)))
        return (struct monst *) 0;

    //EMIN(roamer)->min_align = alignment;
    //EMIN(roamer)->renegade = (coaligned && !peaceful);
    //roamer->ispriest = 0;
    //roamer->isminion = 1;
    //roamer->mtrapseen = ~0; /* traps are known */
    //roamer->mpeaceful = peaceful;
    //roamer->msleeping = 0;
    //set_mhostility(roamer); /* peaceful may have changed */

    /* MORE TO COME */
    return roamer;
}

void
reset_hostility(roamer)
register struct monst *roamer;
{
    if (!roamer || !roamer->isminion || !has_emin(roamer))
        return;

    if (roamer->data != &mons[PM_ALIGNED_PRIEST]
        && roamer->data != &mons[PM_ANGEL])
        return;

    if (EMIN(roamer)->min_align != u.ualign.type)
    {
        roamer->mpeaceful = roamer->mtame = 0;
        if (!roamer->mtame)
            roamer->ispartymember = FALSE;
        set_mhostility(roamer);
    }
    newsym(roamer->mx, roamer->my);
}

boolean
in_your_sanctuary(mon, x, y)
struct monst *mon; /* if non-null, <mx,my> overrides <x,y> */
xchar x, y;
{
    register char roomno;
    register struct monst *priest;

    if (mon) 
    {
        if (is_minion(mon->data) || is_rider(mon->data))
            return FALSE;
        x = mon->mx, y = mon->my;
    }

    if (u.ualign.record <= ALGN_SINNED) /* sinned or worse */
        return FALSE;
    if ((roomno = temple_occupied(u.urooms)) == 0
        || roomno != *in_rooms(x, y, TEMPLE))
        return FALSE;
    if ((priest = findpriest(roomno)) == 0)
        return FALSE;
    return (boolean) (has_shrine(priest) && p_coaligned(priest)
                      && priest->mpeaceful);
}

/* when attacking "priest" in his temple */
void
ghod_hitsu(priest)
struct monst *priest;
{
    int x, y, ax, ay, roomno = (int) temple_occupied(u.urooms);
    struct mkroom *troom;

    if (!roomno || !has_shrine(priest))
        return;

    ax = x = EPRI(priest)->shrpos.x;
    ay = y = EPRI(priest)->shrpos.y;
    troom = &rooms[roomno - ROOMOFFSET];

    if ((u.ux == x && u.uy == y) || !linedup(u.ux, u.uy, x, y, 1, FALSE, FALSE, FALSE, 0, FALSE, M_SHOOT_RANGE))
    {
        if (IS_DOOR(levl[u.ux][u.uy].typ))
        {
            if (u.ux == troom->lx - 1) 
            {
                x = troom->hx;
                y = u.uy;
            } 
            else if (u.ux == troom->hx + 1) 
            {
                x = troom->lx;
                y = u.uy;
            }
            else if (u.uy == troom->ly - 1)
            {
                x = u.ux;
                y = troom->hy;
            }
            else if (u.uy == troom->hy + 1) 
            {
                x = u.ux;
                y = troom->ly;
            }
        }
        else
        {
            switch (rn2(4))
            {
            case 0:
                x = u.ux;
                y = troom->ly;
                break;
            case 1:
                x = u.ux;
                y = troom->hy;
                break;
            case 2:
                x = troom->lx;
                y = u.uy;
                break;
            default:
                x = troom->hx;
                y = u.uy;
                break;
            }
        }
        if (!linedup(u.ux, u.uy, x, y, 1, FALSE, FALSE, FALSE, 0, FALSE, M_SHOOT_RANGE))
            return;
    }

    switch (rn2(3)) 
    {
    case 0:
        play_voice_god_simple_line_at(ax, ay, GOD_LINE_THOU_SHALT_SUFFER);
        pline_ex(ATR_NONE, CLR_MSG_GOD, "%s roars in anger:  \"Thou shalt suffer!\"",
              a_gname_at(ax, ay));
        break;
    case 1:
        play_voice_god_simple_line_at(ax, ay, GOD_LINE_HOW_DAREST_THOU_HARM_MY_SERVANT);
        pline_ex(ATR_NONE, CLR_MSG_GOD, "%s voice booms:  \"How darest thou harm my servant!\"",
              s_suffix(a_gname_at(ax, ay)));
        break;
    default:
        play_voice_god_simple_line_at(ax, ay, GOD_LINE_THOU_DOST_PROFANE_MY_SHRINE);
        pline_ex(ATR_NONE, CLR_MSG_GOD, "%s roars:  \"Thou dost profane my shrine!\"",
              a_gname_at(ax, ay));
        break;
    }

    buzz(-10 - (AD_ELEC - 1), (struct obj*)0, (struct monst*)0, 6, 8, 0, x, y, sgn(tbx),
         sgn(tby)); /* bolt of lightning */
    exercise(A_WIS, FALSE);
}

void
angry_priest()
{
    register struct monst *priest;
    struct rm *lev;

    if ((priest = findpriest(temple_occupied(u.urooms))) != 0)
    {
        struct epri *eprip = EPRI(priest);

        wakeup(priest, FALSE);
        setmangry(priest, FALSE);
        /*
         * If the altar has been destroyed or converted, let the
         * priest run loose.
         * (When it's just a conversion and there happens to be
         * a fresh corpse nearby, the priest ought to have an
         * opportunity to try converting it back; maybe someday...)
         */
        lev = &levl[eprip->shrpos.x][eprip->shrpos.y];
        if (!IS_ALTAR(lev->typ)
            || ((aligntyp) Amask2align(lev->altarmask & AM_MASK)
                != eprip->shralign))
        {
            if (!has_emin(priest))
                newemin(priest);
            if (has_emin(priest))
            {
                priest->ispriest = 0; /* now a roaming minion */
                priest->isminion = 1;
                EMIN(priest)->min_align = eprip->shralign;
                EMIN(priest)->renegade = FALSE;
                /* discard priest's memory of his former shrine;
                   if we ever implement the re-conversion mentioned
                   above, this will need to be removed */
                free_epri(priest);
            }
        }
    }
}

/*
 * When saving bones, find priests that aren't on their shrine level,
 * and remove them.  This avoids big problems when restoring bones.
 * [Perhaps we should convert them into roamers instead?]
 */
void
clearpriests()
{
    struct monst *mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
    {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->ispriest && !on_level(&(EPRI(mtmp)->shrlevel), &u.uz))
            mongone(mtmp);
    }
}

/* munge priest-specific structure when restoring -dlc */
void
restpriest(mtmp, ghostly)
register struct monst *mtmp;
boolean ghostly;
{
    if (u.uz.dlevel) 
    {
        if (ghostly)
            assign_level(&(EPRI(mtmp)->shrlevel), &u.uz);
    }
}

void
clearsmiths()
{
    struct monst* mtmp;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) 
    {
        if (DEADMONSTER(mtmp))
            continue;
        if (mtmp->issmith && !on_level(&(ESMI(mtmp)->smithy_level), &u.uz))
            mongone(mtmp);
    }
}

/* munge smith-specific structure when restoring -dlc */
void
restsmith(mtmp, ghostly)
register struct monst* mtmp;
boolean ghostly;
{
    if (u.uz.dlevel) 
    {
        if (ghostly)
            assign_level(&(ESMI(mtmp)->smithy_level), &u.uz);
    }
}

/*
 * align_str(), piousness(), mstatusline() and ustatusline() used to be
 * in pline.c, presumeably because the latter two generate one line of
 * output.  The USE_OLDARGS config gets warnings from 2016ish-vintage
 * gcc (for -Wint-to-pointer-cast, activated by -Wall or -W) when they
 * follow pline() itself.  Fixing up the variadic calls like is done for
 * lev_comp would be needlessly messy there.
 *
 * They don't belong here.  If/when enlightenment ever gets split off
 * from cmd.c (which definitely doesn't belong there), they should go
 * with it.
 */

const char *
align_str(alignment)
aligntyp alignment;
{
    switch ((int) alignment) 
    {
    case A_CHAOTIC:
        return "chaotic";
    case A_NEUTRAL:
        return "neutral";
    case A_LAWFUL:
        return "lawful";
    case A_NONE:
        return "unaligned";
    }
    return "unknown";
}

/* used for self-probing */
char *
piousness(showneg, suffix)
boolean showneg;
const char *suffix;
{
    static char buf[32]; /* bigger than "insufficiently neutral" */
    const char *pio;

    /* note: piousness 20 matches MIN_QUEST_ALIGN (quest.h) */
    if (u.ualign.record >= 20)
        pio = "piously";
    else if (u.ualign.record > 13)
        pio = "devoutly";
    else if (u.ualign.record > 8)
        pio = "fervently";
    else if (u.ualign.record > 3)
        pio = "stridently";
    else if (u.ualign.record == 3)
        pio = "";
    else if (u.ualign.record > 0)
        pio = "haltingly";
    else if (u.ualign.record == 0)
        pio = "nominally";
    else if (!showneg)
        pio = "insufficiently";
    else if (u.ualign.record >= -3)
        pio = "strayed";
    else if (u.ualign.record >= -8)
        pio = "sinned";
    else
        pio = "transgressed";

    Sprintf(buf, "%s", pio);
    if (suffix && (!showneg || u.ualign.record >= 0)) 
    {
        if (u.ualign.record != 3)
            Strcat(buf, " ");
        Strcat(buf, suffix);
    }
    return buf;
}

/* stethoscope or probing applied to monster -- one-line feedback */
void
mstatusline(mtmp)
struct monst *mtmp;
{
    char buf[BUFSZ];
    print_mstatusline(buf, mtmp, ARTICLE_THE, FALSE);
    pline("Status of %s", buf);
}

/* stethoscope or probing applied to monster -- one-line feedback */
void
print_mstatusline(buf, mtmp, monsternamearticle, showheads)
char* buf;
struct monst* mtmp;
int monsternamearticle;
boolean showheads;
{
    aligntyp alignment = mon_aligntyp(mtmp);
    char info[BUFSZ], monnambuf[BUFSZ];

    info[0] = 0;
    if (showheads && mtmp->data->heads > 1)
    {
        Sprintf(eos(info), "  Heads %d(%d)", mtmp->heads_left, mtmp->data->heads);
    }

    if (is_long_worm_with_tail(mtmp->data)) 
    {
        int segndx, nsegs = count_wsegs(mtmp);

        /* the worm code internals don't consider the head of be one of
           the worm's segments, but we count it as such when presenting
           worm feedback to the player */
        if (!nsegs) 
        {
            Strcat(info, ", single segment");
        }
        else 
        {
            ++nsegs; /* include head in the segment count */
            segndx = wseg_at(mtmp, bhitpos.x, bhitpos.y);
            Sprintf(eos(info), ", %d%s of %d segments",
                segndx, ordin(segndx), nsegs);
        }
    }
    if (mtmp->cham >= LOW_PM && mtmp->data != &mons[mtmp->cham])
        /* don't reveal the innate form (chameleon, vampire, &c),
           just expose the fact that this current form isn't it */
        Strcat(info, ", shapechanger");

    if (is_tame(mtmp) && !is_non_eater(mtmp->data) && !mtmp->isminion && mtmp->mextra && EDOG(mtmp) && monstermoves >= EDOG(mtmp)->hungrytime)
    {
        struct edog* edog = EDOG(mtmp);

        if (monstermoves >= edog->hungrytime + 500)
            Sprintf(eos(info), ", %s from hunger", monstermoves >= edog->hungrytime + 650 ? "crazed" : "confused");
        else
            Sprintf(eos(info), ", %shungry", monstermoves >= edog->hungrytime + 400 ? "extremely " : monstermoves >= edog->hungrytime + 200 ? "very " : "");
    }

    if (is_tame(mtmp))
    {
        if (wizard && flags.wiz_mstatusline)
        {
            Sprintf(eos(info), " (%d", is_tame(mtmp));
            if (!mtmp->isminion)
                Sprintf(eos(info), "; hungry %ld; apport %d",
                    EDOG(mtmp)->hungrytime, EDOG(mtmp)->apport);
            Strcat(info, ")");
        }
    }

    /* pets eating mimic corpses mimic while eating, so this comes first */
    if (mtmp->meating)
        Strcat(info, ", eating");
    /* a stethoscope exposes mimic before getting here so this
       won't be relevant for it, but wand of probing doesn't */
    if (mtmp->mundetected || mtmp->m_ap_type)
        mhidden_description(mtmp, TRUE, eos(info));
    if (is_cancelled(mtmp))
        Strcat(info, ", cancelled");
    if (is_silenced(mtmp))
        Strcat(info, ", silenced");
    if (has_no_magic_resistance(mtmp))
        Strcat(info, ", no magic resistance");
    else if (has_half_magic_resistance(mtmp))
        Strcat(info, ", halved magic resistance");
    if (has_summon_forbidden(mtmp))
        Strcat(info, ", unable to summon");
    if (is_confused(mtmp))
        Strcat(info, ", confused");
    if (is_blinded(mtmp))
        Strcat(info, ", blind");
    if (is_sick(mtmp))
        Strcat(info, ", terminally ill");
    if (is_stoning(mtmp))
        Strcat(info, ", solidifying");
    if (is_turning_into_slime(mtmp))
        Strcat(info, ", becoming slimy");
    if (is_being_strangled(mtmp))
        Strcat(info, ", being strangled");
    if (is_paralyzed(mtmp))
        Strcat(info, ", paralyzed");
    if (is_stunned(mtmp))
        Strcat(info, ", stunned");
    if (is_sleeping(mtmp))
        Strcat(info, ", asleep");
    if (mtmp->mfrozen || !mtmp->mcanmove)
        Strcat(info, ", can't move");
    else if (mtmp->mstrategy & STRAT_WAITMASK)
        Strcat(info, ", meditating");
    if (is_fleeing(mtmp))
        Strcat(info, ", scared");
    if (mtmp->mtrapped)
        Strcat(info, ", trapped");
    if (is_slow(mtmp))
        Strcat(info, ", slow");
    else if (is_lightning_fast(mtmp))
        Strcat(info, ", lightning fast");
    else if (is_super_fast(mtmp))
        Strcat(info, ", super fast");
    else if (is_ultra_fast(mtmp))
        Strcat(info, ", ultra fast");
    else if (is_very_fast(mtmp))
        Strcat(info, ", very fast");
    else if (is_fast(mtmp))
        Strcat(info, ", fast");
    if (is_invisible(mtmp))
        Strcat(info, ", invisible");
    if (mtmp == u.ustuck)
        Strcat(info, sticks(youmonst.data) ? ", held by you"
            : !u.uswallow ? ", holding you"
            : attacktype_fordmg(u.ustuck->data, AT_ENGL, AD_DGST)
            ? ", digesting you"
            : is_animal(u.ustuck->data) ? ", swallowing you"
            : ", engulfing you");
    if (mtmp == u.usteed)
        Strcat(info, ", carrying you");

    /* avoid "Status of the invisible newt ..., invisible" */
    /* and unlike a normal mon_nam, use "saddled" even if it has a name */
    char adjbuf[BUFSZ], alignbuf[BUFSZ];
    Sprintf(adjbuf, "level %d%s", mtmp->data->difficulty, is_tame(mtmp) ? (call_mon_tame(mtmp) ? " tame" : " allied") : is_peaceful(mtmp) ? " peaceful" : "");
    strcpy(alignbuf, align_str(alignment));
    *alignbuf = highc(*alignbuf);

    Strcpy(monnambuf, x_monnam(mtmp, monsternamearticle, adjbuf,
        (SUPPRESS_IT | SUPPRESS_INVISIBLE), FALSE));

    Sprintf(buf, "%s, HP:%d(%d) AC:%d %s%s", monnambuf,
        mtmp->mhp, mtmp->mhpmax, find_mac(mtmp), alignbuf, info);
}

/* stethoscope or probing applied to hero -- one-line feedback */
void
ustatusline()
{
    char info[BUFSZ];

    info[0] = '\0';

    if (youmonst.data->heads > 1)
    {
        Sprintf(eos(info), "  %d(%d) heads", youmonst.heads_left, youmonst.data->heads);
    }

    if (Sick || FoodPoisoned) 
    {
        Strcat(info, ", dying from");
        if (FoodPoisoned)
            Strcat(info, " food poisoning");
        if (FoodPoisoned)
            Strcat(info, " food poisoning");
        if (Sick) 
        {
            if (FoodPoisoned)
                Strcat(info, " and");
            Strcat(info, " illness");
        }
    }

    if (MummyRot) 
    {
        Strcat(info, ", mummy rot");
    }

    if (Stoned)
        Strcat(info, ", solidifying");
    if (Slimed)
        Strcat(info, ", becoming slimy");
    if (Strangled)
        Strcat(info, ", being strangled");
    if (Vomiting)
        Strcat(info, ", nauseated"); /* !"nauseous" */
    if (Confusion)
        Strcat(info, ", confused");
    if (Fearful)
        Strcat(info, ", fearful");
    if (Cancelled)
        Strcat(info, ", cancelled");
    if (Silenced)
        Strcat(info, ", silenced");
    if (Paralyzed_or_immobile)
        Strcat(info, ", paralyzed");
    if (Sleeping)
        Strcat(info, ", sleeping");
    if (Blind) 
    {
        Strcat(info, ", blind");
        if (u.ucreamed) 
        {
            if ((long) u.ucreamed < Blinded || Blindfolded
                || !haseyes(youmonst.data))
                Strcat(info, ", cover");
            Strcat(info, "ed by sticky goop");
        } /* note: "goop" == "glop"; variation is intentional */
    }
    if (Stunned)
        Strcat(info, ", stunned");
    if (!u.usteed && Wounded_legs) 
    {
        const char *what = body_part(LEG);
        if ((Wounded_legs & BOTH_SIDES) == BOTH_SIDES)
            what = makeplural(what);
        Sprintf(eos(info), ", injured %s", what);
    }
    if (Glib)
        Sprintf(eos(info), ", slippery %s", makeplural(body_part(HAND)));
    if (u.utrap)
        Strcat(info, ", trapped");

    if (Lightning_fast)
        Strcat(info, ", lightning fast");
    else if (Super_fast)
        Strcat(info, ", super fast");
    else if (Ultra_fast)
        Strcat(info, ", ultra fast");
    else if (Very_fast)
        Strcat(info, ", very fast");
    else if (Fast)
        Strcat(info,  ", fast");

    if (u.uundetected)
        Strcat(info, ", concealed");
    if (Invis)
        Strcat(info, ", invisible");
    if (u.ustuck) 
    {
        if (sticks(youmonst.data))
            Strcat(info, ", holding ");
        else
            Strcat(info, ", held by ");
        Strcat(info, mon_nam(u.ustuck));
    }

    pline("Status of %s (%s):  Level %d  HP %d(%d)  AC %d%s.", plname,
          piousness(FALSE, align_str(u.ualign.type)),
          Upolyd ? mons[u.umonnum].mlevel : u.ulevel, Upolyd ? u.mh : u.uhp,
          Upolyd ? u.mhmax : u.uhpmax, u.uac, info);
}

/*priest.c*/
