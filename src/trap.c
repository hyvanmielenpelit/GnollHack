/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-07 */

/* GnollHack 4.0    trap.c    $NHDT-Date: 1545259936 2018/12/19 22:52:16 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.313 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2013. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <math.h>

extern const char *const destroy_strings[][3]; /* from zap.c */

STATIC_DCL boolean FDECL(keep_saddle_with_steedcorpse, (unsigned, struct obj *,
                                                        struct obj *));
STATIC_DCL struct obj *FDECL(t_missile, (int, struct trap *));
STATIC_DCL char *FDECL(trapnote, (struct trap *, BOOLEAN_P));
STATIC_DCL int FDECL(steedintrap, (struct trap *, struct obj *));
STATIC_DCL void FDECL(launch_drop_spot, (struct obj *, XCHAR_P, XCHAR_P));
STATIC_DCL int FDECL(mkroll_launch, (struct trap *, XCHAR_P, XCHAR_P,
                                     SHORT_P, long));
STATIC_DCL boolean FDECL(isclearpath, (coord *, int, SCHAR_P, SCHAR_P));
STATIC_DCL void FDECL(dofiretrap, (struct obj *, int));
STATIC_DCL void NDECL(domagictrap);
STATIC_DCL boolean FDECL(emergency_disrobe, (boolean *));
STATIC_DCL int FDECL(succeed_untrap, (int));
STATIC_DCL void FDECL(move_into_trap, (struct trap *));
STATIC_DCL int FDECL(try_disarm, (struct trap *, BOOLEAN_P));
STATIC_DCL void FDECL(reward_untrap, (struct trap *, struct monst *));
STATIC_DCL int FDECL(disarm_holdingtrap, (struct trap *));
STATIC_DCL int FDECL(disarm_magical_trap, (struct trap*));
STATIC_DCL int FDECL(disarm_landmine, (struct trap *));
STATIC_DCL int FDECL(disarm_squeaky_board, (struct trap *));
STATIC_DCL int FDECL(disarm_shooting_trap, (struct trap *, int));
STATIC_DCL void FDECL(clear_conjoined_pits, (struct trap *));
STATIC_DCL boolean FDECL(adj_nonconjoined_pit, (struct trap *));
STATIC_DCL int FDECL(try_lift, (struct monst *, struct trap *, int,
                                BOOLEAN_P));
STATIC_DCL int FDECL(help_monster_out, (struct monst *, struct trap *));
#if 0
STATIC_DCL void FDECL(join_adjacent_pits, (struct trap *));
#endif
STATIC_DCL boolean FDECL(thitm, (int, struct monst *, struct obj *, int,
                                 BOOLEAN_P));
STATIC_DCL void NDECL(maybe_finish_sokoban);

/* mintrap() should take a flags argument, but for time being we use this */
STATIC_VAR int force_mintrap = 0;

STATIC_VAR const char *const a_your[2] = { "a", "your" };
STATIC_VAR const char *const A_Your[2] = { "A", "Your" };
STATIC_VAR const char tower_of_flame[] = "tower of flame";
STATIC_VAR const char *const A_gush_of_water_hits = "A gush of water hits";
STATIC_VAR const char *const blindgas[6] = { "humid",   "odorless",
                                             "pungent", "chilling",
                                             "acrid",   "biting" };

/* Note these names are not currently being used, instead use the explanations in defsyms and defsym_variations */
const struct trap_type_definition trap_type_definitions[TRAPNUM] = {
    {"", "", "", MAT_NONE, SIMPLE_MECHANICAL_TRAP, 0, TRAPDEF_FLAGS_NONE},
    {"arrow trap", "trap", "", MAT_METAL, SIMPLE_MECHANICAL_TRAP, 10, TRAPDEF_FLAGS_NONE},
    {"dart trap", "trap", "", MAT_METAL, SIMPLE_MECHANICAL_TRAP, 10, TRAPDEF_FLAGS_NONE},
    {"falling rock trap", "trap", "", MAT_MINERAL, COMPLEX_MECHANICAL_TRAP, 20, TRAPDEF_FLAGS_NONE},
    {"squeaky board", "trap", "", MAT_WOOD, SIMPLE_MECHANICAL_TRAP, 10, TRAPDEF_FLAGS_NONE},
    {"bear trap", "trap", "", MAT_METAL, SIMPLE_MECHANICAL_TRAP, 10, TRAPDEF_FLAGS_NONE},
    {"land mine", "mine", "", MAT_MINERAL, COMPLEX_MECHANICAL_TRAP, 20, TRAPDEF_FLAGS_NONE},
    {"rolling boulder trap", "trap", "", MAT_MINERAL, COMPLEX_MECHANICAL_TRAP, 20, TRAPDEF_FLAGS_NONE},
    {"sleep gas trap", "trap", "", MAT_MINERAL, COMPLEX_MECHANICAL_TRAP, 20, TRAPDEF_FLAGS_NONE},
    {"rust trap", "trap", "", MAT_MINERAL, COMPLEX_MECHANICAL_TRAP, 20, TRAPDEF_FLAGS_NONE},
    {"fire trap", "trap", "", MAT_MINERAL, COMPLEX_MAGICAL_TRAP, 40, TRAPDEF_FLAGS_NONE},
    {"pit", "trap", "", MAT_MINERAL, NOT_DISARMABLE_TRAP, 0, TRAPDEF_FLAGS_NOT_DISARMABLE},
    {"spiked pit", "trap", "", MAT_MINERAL, NOT_DISARMABLE_TRAP, 0, TRAPDEF_FLAGS_NOT_DISARMABLE},
    {"hole", "hole", "", MAT_MINERAL, NOT_DISARMABLE_TRAP, 0, TRAPDEF_FLAGS_VISIBLE_AT_START | TRAPDEF_FLAGS_NOT_DISARMABLE},
    {"trap door", "trap door", "", MAT_MINERAL, NOT_DISARMABLE_TRAP, 0, TRAPDEF_FLAGS_NOT_DISARMABLE},
    {"teleport trap", "trap", "", MAT_ENERGY, SIMPLE_MAGICAL_TRAP, 30, TRAPDEF_FLAGS_NONE},
    {"level teleport trap", "trap", "", MAT_ENERGY, SIMPLE_MAGICAL_TRAP, 30, TRAPDEF_FLAGS_NONE},
    {"magic portal", "portal", "", MAT_ENERGY, NOT_DISARMABLE_TRAP, 0,
        TRAPDEF_FLAGS_NO_TRY_ESCAPE | TRAPDEF_FLAGS_NOT_OVERRIDEN | TRAPDEF_FLAGS_NOT_GENERATED | TRAPDEF_FLAGS_NOT_DISARMABLE},
    {"web", "web", "", MAT_ORGANIC, SPECIALLY_DISARMABLE_TRAP, 10, TRAPDEF_FLAGS_NONE},
    {"statue", "statue", "", MAT_MINERAL, SIMPLE_MAGICAL_TRAP, 30, TRAPDEF_FLAGS_NO_STEP_CONFIRMATION | TRAPDEF_FLAGS_IGNORED_BY_MONSTERS },
    {"magic trap", "trap", "", MAT_ENERGY, COMPLEX_MAGICAL_TRAP, 40, TRAPDEF_FLAGS_NONE},
    {"anti-magic trap", "trap", "", MAT_ENERGY, SIMPLE_MAGICAL_TRAP, 30, TRAPDEF_FLAGS_NO_TRY_ESCAPE },
    {"polymorph trap", "trap", "", MAT_ENERGY, COMPLEX_MAGICAL_TRAP, 40, TRAPDEF_FLAGS_NONE},
    {"geometric magic portal", "portal", "", MAT_ENERGY, NOT_DISARMABLE_TRAP, 0,
        TRAPDEF_FLAGS_NO_TRY_ESCAPE | TRAPDEF_FLAGS_NOT_OVERRIDEN | TRAPDEF_FLAGS_NOT_GENERATED | TRAPDEF_FLAGS_NOT_DISARMABLE},
    {"lever", "lever", "pull", MAT_METAL, NOT_DISARMABLE_TRAP, 0,
        TRAPDEF_FLAGS_APPLIABLE | TRAPDEF_FLAGS_NO_TRY_ESCAPE | TRAPDEF_FLAGS_TELEOK | TRAPDEF_FLAGS_VISIBLE_AT_START | TRAPDEF_FLAGS_NOT_OVERRIDEN | TRAPDEF_FLAGS_IGNORED_BY_MONSTERS | TRAPDEF_FLAGS_NOT_GENERATED | TRAPDEF_FLAGS_NO_STEP_CONFIRMATION | TRAPDEF_FLAGS_NOT_DISARMABLE},
    {"vibrating square", "vibrating square", "", MAT_MINERAL, NOT_DISARMABLE_TRAP, 0,
        TRAPDEF_FLAGS_NO_TRY_ESCAPE | TRAPDEF_FLAGS_TELEOK | TRAPDEF_FLAGS_NOT_OVERRIDEN | TRAPDEF_FLAGS_NOT_GENERATED | TRAPDEF_FLAGS_NO_STEP_CONFIRMATION | TRAPDEF_FLAGS_IGNORED_BY_MONSTERS | TRAPDEF_FLAGS_NOT_DISARMABLE}
};

/* called when you're hit by fire (dofiretrap,buzz,zapyourself,explode);
   returns TRUE if hit on torso */
boolean
burnarmor(victim)
struct monst *victim;
{
    struct obj *item;
    char buf[BUFSZ];
    int mat_idx, oldspe;
    boolean hitting_u;

    if (!victim)
        return 0;
    hitting_u = (victim == &youmonst);

    /* burning damage may dry wet towel */
    item = hitting_u ? carrying(TOWEL) : m_carrying(victim, TOWEL);
    while (item) {
        if (is_wet_towel(item)) {
            oldspe = item->special_quality;
            dry_a_towel(item, rn2(oldspe + 1), TRUE);
            if (item->special_quality != oldspe)
                break; /* stop once one towel has been affected */
        }
        item = item->nobj;
    }

#define burn_dmg(obj, descr) erode_obj(obj, descr, ERODE_BURN, EF_GREASE)
    do {
        switch (rn2(6)) {
        case 0:
            item = hitting_u ? uarmh : which_armor(victim, W_ARMH);
            if (item) {
                mat_idx = item->material;
                Sprintf(buf, "%s %s", material_definitions[mat_idx].name,
                        helm_simple_name(item));
            }
            if (!burn_dmg(item, item ? buf : "helmet"))
                continue;
            break;
        case 1:
            item = hitting_u ? uarmc : which_armor(victim, W_ARMC);
            if (item) {
                (void) burn_dmg(item, cloak_simple_name(item));
                return TRUE;
            }
            item = hitting_u ? uarmo : which_armor(victim, W_ARMO);
            if (item) {
                (void)burn_dmg(item, robe_simple_name(item));
                return TRUE;
            }
            item = hitting_u ? uarm : which_armor(victim, W_ARM);
            if (item) {
                (void) burn_dmg(item, xname(item));
                return TRUE;
            }
            item = hitting_u ? uarmu : which_armor(victim, W_ARMU);
            if (item)
                (void) burn_dmg(item, "shirt");
            return TRUE;
        case 2:
            item = hitting_u ? uarms : which_armor(victim, W_ARMS);
            if (!burn_dmg(item, (item && (item->owornmask & W_ARMS) ? "wooden shield" : "wooden weapon")))
                continue;
            break;
        case 3:
            item = hitting_u ? uarmg : which_armor(victim, W_ARMG);
            if (!burn_dmg(item, "gloves"))
                continue;
            break;
        case 4:
            item = hitting_u ? uarmf : which_armor(victim, W_ARMF);
            if (!burn_dmg(item, "boots"))
                continue;
            break;
        case 5:
            item = hitting_u ? uarmb : which_armor(victim, W_ARMB);
            if (!burn_dmg(item, "bracers"))
                continue;
            break;
        }
        break; /* Out of while loop */
    } 
    while (!is_obj_protected_by_property((struct obj*)0, &youmonst, AD_FIRE)); //To limit the effect of continue statements when having fire resistance

#undef burn_dmg

    return FALSE;
}

/* Generic erode-item function.
 * "ostr", if non-null, is an alternate string to print instead of the
 *   object's name.
 * "type" is an ERODE_* value for the erosion type
 * "flags" is an or-ed list of EF_* flags
 *
 * Returns an erosion return value (ER_*)
 */
int
erode_obj(otmp, ostr, type, ef_flags)
register struct obj *otmp;
const char *ostr;
int type;
int ef_flags;
{
    static NEARDATA const char
        *const action[] = { "smoulder", "rust", "rot", "corrode" },
        *const msg[] = { "burnt", "rusted", "rotten", "corroded" },
        *const bythe[] = { "heat", "oxidation", "decay", "corrosion" };

    enum object_sound_types obj_erode_sounds[] = { OBJECT_SOUND_TYPE_EROSION_BURNT, OBJECT_SOUND_TYPE_EROSION_RUSTS, OBJECT_SOUND_TYPE_EROSION_ROTS, OBJECT_SOUND_TYPE_EROSION_CORRODES };

    boolean vulnerable = FALSE, is_primary = TRUE,
            check_grease = (ef_flags & EF_GREASE) ? TRUE : FALSE,
            print = (ef_flags & EF_VERBOSE) ? TRUE : FALSE,
            uvictim, vismon, visobj;
    int erosion, cost_type;
    struct monst *victim;
    int adtyp = AD_NONE;

    if (!otmp)
        return ER_NOTHING;

    victim = carried(otmp) ? &youmonst : mcarried(otmp) ? otmp->ocarry : NULL;
    uvictim = (victim == &youmonst);
    vismon = victim && (victim != &youmonst) && canseemon(victim);
    /* Is bhitpos correct here? Ugh. */
    visobj = !victim && cansee(bhitpos.x, bhitpos.y);

    switch (type) 
    {
    case ERODE_BURN:
        vulnerable = is_flammable(otmp);
        check_grease = FALSE;
        cost_type = COST_BURN;
        adtyp = AD_FIRE;
        break;
    case ERODE_RUST:
        vulnerable = is_rustprone(otmp);
        cost_type = COST_RUST;
        break;
    case ERODE_ROT:
        vulnerable = is_rottable(otmp);
        check_grease = FALSE;
        is_primary = FALSE;
        cost_type = COST_ROT;
        break;
    case ERODE_CORRODE:
        vulnerable = is_corrodeable(otmp);
        is_primary = FALSE;
        cost_type = COST_CORRODE;
        break;
    default:
        impossible("Invalid erosion type in erode_obj");
        return ER_NOTHING;
    }
    
    erosion = is_primary ? otmp->oeroded : otmp->oeroded2;

    if (!ostr)
        ostr = cxname(otmp);
    /* 'visobj' messages insert "the"; probably ought to switch to the() */
    if (visobj && !(uvictim || vismon) && !strncmpi(ostr, "the ", 4))
        ostr += 4;

    if (check_grease && otmp->greased)
    {
        grease_protect(otmp, ostr, victim);
        return ER_GREASED;
    }
    else if (!erosion_matters(otmp)) 
    {
        return ER_NOTHING;
    } 
    else if (!vulnerable || (otmp->oerodeproof && otmp->rknown))
    {
        if (flags.verbose && print && (uvictim || vismon))
            pline("%s %s %s not affected by %s.",
                  uvictim ? "Your" : s_suffix(Monnam(victim)),
                  ostr, vtense(ostr, "are"), bythe[type]);
        return ER_NOTHING;
    }
    else if (otmp->oerodeproof || (otmp->blessed && !rnl(4)) || is_obj_protected_by_property(otmp, victim, adtyp))
    {
        if (flags.verbose && (print || otmp->oerodeproof)
            && (uvictim || vismon || visobj))
            pline("Somehow, %s %s %s not affected by the %s.",
                  uvictim ? "your"
                          : !vismon ? "the" /* visobj */
                                    : s_suffix(mon_nam(victim)),
                  ostr, vtense(ostr, "are"), bythe[type]);
        /* We assume here that if the object is protected because it
         * is blessed, it still shows some minor signs of wear, and
         * the hero can distinguish this from an object that is
         * actually proof against damage.
         */
        if (otmp->oerodeproof)
        {
            otmp->rknown = TRUE;
            if (victim == &youmonst)
                update_inventory();
        }

        return ER_NOTHING;
    } 
    else if (erosion < MAX_ERODE) 
    {
        const char *adverb = (erosion + 1 == MAX_ERODE)
                                 ? " completely"
                                 : erosion ? " further" : "";


        if (uvictim || vismon || visobj)
        {
            play_simple_object_sound(otmp, obj_erode_sounds[type]);
            pline_ex(ATR_NONE, uvictim ? CLR_MSG_NEGATIVE : NO_COLOR, "%s %s %s%s!",
                uvictim ? "Your"
                : !vismon ? "The" /* visobj */
                : s_suffix(Monnam(victim)),
                ostr, vtense(ostr, action[type]), adverb);
        }

        if (ef_flags & EF_PAY)
            costly_alteration(otmp, cost_type);

        if (is_primary)
            otmp->oeroded++;
        else
            otmp->oeroded2++;

        if (victim == &youmonst)
            update_inventory();

        return ER_DAMAGED;
    }
    else if (ef_flags & EF_DESTROY) 
    {
        if (uvictim || vismon || visobj)
        {
            play_simple_object_sound(otmp, obj_erode_sounds[type]);
            pline_ex(ATR_NONE, uvictim ? CLR_MSG_NEGATIVE : NO_COLOR, "%s %s %s away!",
                uvictim ? "Your"
                : !vismon ? "The" /* visobj */
                : s_suffix(Monnam(victim)),
                ostr, vtense(ostr, action[type]));
        }

        if (ef_flags & EF_PAY)
            costly_alteration(otmp, cost_type);

        setnotworn(otmp);
        delobj(otmp);
        return ER_DESTROYED;
    } 
    else 
    {
        if (flags.verbose && print) 
        {
            if (uvictim || vismon || visobj)
                play_simple_object_sound(otmp, obj_erode_sounds[type]);

            if (uvictim)
                Your_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s completely %s.",
                     ostr, vtense(ostr, Blind ? "feel" : "look"), msg[type]);
            else if (vismon || visobj)
                pline("%s %s %s completely %s.",
                      !vismon ? "The" : s_suffix(Monnam(victim)),
                      ostr, vtense(ostr, "look"), msg[type]);
        }
        return ER_NOTHING;
    }
}

/* Protect an item from erosion with grease. Returns TRUE if the grease
 * wears off.
 */
boolean
grease_protect(otmp, ostr, victim)
register struct obj *otmp;
const char *ostr;
struct monst *victim;
{
    static const char txt[] = "protected by the layer of grease!";
    boolean vismon = victim && (victim != &youmonst) && canseemon(victim);

    if (ostr) {
        if (victim == &youmonst)
            Your("%s %s %s", ostr, vtense(ostr, "are"), txt);
        else if (vismon)
            pline("%s's %s %s %s", Monnam(victim),
                  ostr, vtense(ostr, "are"), txt);
    } else if (victim == &youmonst || vismon) {
        pline("%s %s", Yobjnam2(otmp, "are"), txt);
    }
    if (!rn2(2)) {
        otmp->greased = 0;
        if (carried(otmp)) {
            pline_The("grease dissolves.");
            update_inventory();
        }
        return TRUE;
    }
    return FALSE;
}

struct trap *
maketrap(x, y, typ, permonstid, mkflags)
int x, y, typ, permonstid;
unsigned long mkflags;
{
    static union vlaunchinfo zero_vl;
    boolean oldplace;
    struct trap *ttmp;
    struct rm *lev = &levl[x][y];

    if ((ttmp = t_at(x, y)) != 0) {
        if (trap_type_definitions[ttmp->ttyp].tdflags & TRAPDEF_FLAGS_NOT_OVERRIDEN)
            return (struct trap *) 0;
        oldplace = TRUE;
        if (u.utrap && x == u.ux && y == u.uy
            && ((u.utraptype == TT_BEARTRAP && typ != BEAR_TRAP)
                || (u.utraptype == TT_WEB && typ != WEB)
                || (u.utraptype == TT_PIT && !is_pit(typ))))
            u.utrap = 0;
        /* old <tx,ty> remain valid */
    } else if (IS_FURNITURE(lev->typ)
               && (!IS_GRAVE(lev->typ) || (typ != PIT && typ != HOLE))) {
        /* no trap on top of furniture (caller usually screens the
           location to inhibit this, but wizard mode wishing doesn't) */
        return (struct trap *) 0;
    } else {
        oldplace = FALSE;
        ttmp = newtrap();
        (void) memset((genericptr_t)ttmp, 0, sizeof(struct trap));
        ttmp->ntrap = 0;
        ttmp->tx = x;
        ttmp->ty = y;
    }
    /* [re-]initialize all fields except ntrap (handled below) and <tx,ty> */
    ttmp->vl = zero_vl;
    ttmp->launch.x = ttmp->launch.y = -1; /* force error if used before set */
    ttmp->dst.dnum = ttmp->dst.dlevel = -1;
    ttmp->madeby_u = (mkflags & MKTRAPFLAG_MADE_BY_U) != 0;
    ttmp->madeby_mon = (mkflags & MKTRAPFLAG_MADE_BY_MON) != 0;
    ttmp->once = 0;
    ttmp->tseen = (trap_type_definitions[typ].tdflags & TRAPDEF_FLAGS_VISIBLE_AT_START);
    ttmp->ttyp = typ;
    ttmp->tsubtyp = 0;
    ttmp->tflags = 0;
    ttmp->activation_count = 0;

    switch (typ) {
    case ARROW_TRAP:
        ttmp->tsubtyp = rn2(2);
        break;
    case LEVER:
        break;
    case SQKY_BOARD: {
        int tavail[12], tpick[12], tcnt = 0, k;
        struct trap *t;

        for (k = 0; k < 12; ++k)
            tavail[k] = tpick[k] = 0;
        for (t = ftrap; t; t = t->ntrap)
            if (t->ttyp == SQKY_BOARD && t != ttmp)
                tavail[t->tnote] = 1;
        /* now populate tpick[] with the available indices */
        for (k = 0; k < 12; ++k)
            if (tavail[k] == 0)
                tpick[tcnt++] = k;
        /* choose an unused note; if all are in use, pick a random one */
        ttmp->tnote = (short) ((tcnt > 0) ? tpick[rn2(tcnt)] : rn2(12));
        break;
    }
    case STATUE_TRAP: { /* create a "living" statue */
        struct monst *mtmp;
        struct obj *otmp, *statue;
        struct permonst *mptr;
        int trycount = 10;
        int mnum = NON_PM;

        if(permonstid <= NON_PM)
        {
            do 
            { /* avoid ultimately hostile co-aligned unicorn */
                mnum = rndmonnum();
                mptr = &mons[mnum];
            } 
            while (--trycount > 0 && is_unicorn(mptr)
                     && sgn(u.ualign.type) == sgn(mptr->maligntyp));
        }
        else
        {
            mnum = permonstid;
            mptr = &mons[mnum];
        }
        mtmp = makemon(mptr, 0, 0, MM_NOCOUNTBIRTH | MM_NO_MONSTER_INVENTORY);
        if (!mtmp)
            break; /* should never happen */

        statue = mkcorpstat(STATUE, mtmp, mptr, x, y, CORPSTAT_NONE);
        /* Note if the statue would have items from mtmp, all light sources and sound sources would need to be switched off first */
        if (mkflags & MKTRAPFLAG_GARDEN_GNOME_ITEMS)
        {
            if(!rn2(2))
            {
                otmp = mksobj((!rn2(20) ? (!rn2(2) ? CORNUTHAUM : DUNCE_CAP) : GNOMISH_FELT_HAT), TRUE, FALSE, FALSE);
                if(otmp)
                    (void)add_to_container(statue, otmp);
            }
        }
        statue->owt = weight(statue);
        mongone(mtmp);
        break;
    }
    case ROLLING_BOULDER_TRAP: /* boulder will roll towards trigger */
        (void) mkroll_launch(ttmp, x, y, BOULDER, 1L);
        break;
    case PIT:
    case SPIKED_PIT:
        ttmp->conjoined = 0;
        /*FALLTHRU*/
    case HOLE:
    case TRAPDOOR:
        if (*in_rooms(x, y, SHOPBASE)
            && (is_hole(typ) || IS_DOOR(lev->typ) || IS_WALL(lev->typ)))
            add_damage(x, y, /* schedule repair */
                       ((IS_DOOR(lev->typ) || IS_WALL(lev->typ))
                        && !context.mon_moving)
                           ? SHOP_HOLE_COST
                           : 0L);
        int ltype = IS_FLOOR(lev->typ) ? lev->typ : lev->floortyp ? lev->floortyp : ROOM;
        int lsubtype = IS_FLOOR(lev->typ) ? lev->subtyp : lev->floorsubtyp ? lev->floorsubtyp : get_initial_location_subtype(ROOM);
        uchar lflags = 0;

        if (lev->typ == STONE || lev->typ == SCORR)
            ltype = CORR, lsubtype = get_initial_location_subtype(CORR);
        else if (IS_WALL_OR_SDOOR(lev->typ))
        {
            ltype = level.flags.is_maze_lev
                ? ROOM
                : level.flags.is_cavernous_lev ? CORR : DOOR;

            lsubtype = get_initial_location_subtype(ltype);
        }
        create_basic_floor_location(x, y, ltype, lsubtype, lflags, FALSE);
        unearth_objs(&youmonst, x, y, FALSE, FALSE);
        break;
    }

    if (!oldplace) {
        ttmp->ntrap = ftrap;
        ftrap = ttmp;
    } else {
        /* oldplace;
           it shouldn't be possible to override a sokoban pit or hole
           with some other trap, but we'll check just to be safe */
        if (Sokoban)
            maybe_finish_sokoban();
    }
    return ttmp;
}

void
fall_through(td)
boolean td; /* td == TRUE : trap door or hole */
{
    d_level dtmp;
    char msgbuf[BUFSZ] = "";
    const char *dont_fall = 0;
    int newlevel, bottom;

    /* we'll fall even while levitating in Sokoban; otherwise, if we
       won't fall and won't be told that we aren't falling, give up now */
    if (Blind && Levitation && !Sokoban)
        return;

    bottom = dunlevs_in_dungeon(&u.uz);
    /* when in the upper half of the quest, don't fall past the
       middle "quest locate" level if hero hasn't been there yet */
    if (In_quest(&u.uz)) {
        int qlocate_depth = qlocate_level.dlevel;

        /* deepest reached < qlocate implies current < qlocate */
        if (dunlev_reached(&u.uz) < qlocate_depth)
            bottom = qlocate_depth; /* early cut-off */
    }
    newlevel = dunlev(&u.uz); /* current level */
    do {
        newlevel++;
    } while (!rn2(4) && newlevel < bottom);

    if (td) {
        struct trap *t = t_at(u.ux, u.uy);

        feeltrap(t);
        if (!Sokoban) {
            if (t->ttyp == TRAPDOOR)
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A trap door opens up under you!");
            else
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "There's a gaping hole under you!");
        }
    } else
        pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s opens up under you!", surface(u.ux, u.uy));

    if (Sokoban && Can_fall_thru(&u.uz))
        ; /* KMH -- You can't escape the Sokoban level traps */
    else if (Levitation || u.ustuck
             || (!Can_fall_thru(&u.uz) && !levl[u.ux][u.uy].candig) || Flying
             || is_clinger(youmonst.data)
             || (Inhell && !u.uevent.invoked && newlevel == bottom)) {
        dont_fall = "don't fall in.";
    } else if (youmonst.data->msize >= MZ_HUGE) {
        dont_fall = "don't fit through.";
    } else if (!next_to_u()) {
        dont_fall = "are jerked back by your pet!";
    }
    if (dont_fall) {
        You1(dont_fall);
        /* hero didn't fall through, but any objects here might */
        impact_drop((struct obj *) 0, u.ux, u.uy, 0);
        if (!td) {
            display_nhwindow(WIN_MESSAGE, FALSE);
            pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "opening under you closes up.");
        }
        return;
    }

    if (*u.ushops)
        shopdig(1);
    if (Is_stronghold(&u.uz)) {
        find_hell(&dtmp);
    } else {
        int dist = newlevel - dunlev(&u.uz);
        dtmp.dnum = u.uz.dnum;
        dtmp.dlevel = newlevel;
        if (dist > 1)
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "fall down a %s%sshaft!", dist > 3 ? "very " : "",
                dist > 2 ? "deep " : "");
    }
    if (!td)
        Sprintf(msgbuf, "The hole in the %s above you closes up.",
                ceiling(u.ux, u.uy));

    schedule_goto(&dtmp, FALSE, TRUE, FALSE, FALSE, 0, (char*)0,
                  !td ? msgbuf : (char *) 0);
}

/*
 * Animate the given statue.  May have been via shatter attempt, trap,
 * or stone to flesh spell.  Return a monster if successfully animated.
 * If the monster is animated, the object is deleted.  If fail_reason
 * is non-null, then fill in the reason for failure (or success).
 *
 * The cause of animation is:
 *
 *      ANIMATE_NORMAL  - hero "finds" the monster
 *      ANIMATE_SHATTER - hero tries to destroy the statue
 *      ANIMATE_SPELL   - stone to flesh spell hits the statue
 *
 * Perhaps x, y is not needed if we can use get_obj_location() to find
 * the statue's location... ???
 *
 * Sequencing matters:
 *      create monster; if it fails, give up with statue intact;
 *      give "statue comes to life" message;
 *      if statue belongs to shop, have shk give "you owe" message;
 *      transfer statue contents to monster (after stolen_value());
 *      delete statue.
 *      [This ordering means that if the statue ends up wearing a cloak of
 *       invisibility or a mummy wrapping, the visibility checks might be
 *       wrong, but to avoid that we'd have to clone the statue contents
 *       first in order to give them to the monster before checking their
 *       shop status--it's not worth the hassle.]
 */
struct monst *
animate_statue(statue, x, y, cause, fail_reason)
struct obj *statue;
xchar x, y;
int cause;
int *fail_reason;
{
    int mnum = statue->corpsenm;
    struct permonst *mptr = &mons[mnum];
    struct monst *mon = 0, *shkp;
    struct obj *item;
    coord cc;
    boolean historic = (Role_if(PM_ARCHAEOLOGIST) && statue->special_quality == SPEQUAL_STATUE_HISTORIC),
            golem_xform = FALSE, use_saved_traits;
    const char *comes_to_life;
    char statuename[BUFSZ], tmpbuf[BUFSZ];
    static const char historic_statue_is_gone[] =
        "that the historic statue is now gone";

    if (cant_revive(&mnum, TRUE, statue)) 
    {
        /* mnum has changed; we won't be animating this statue as itself */
        if (mnum != PM_DOPPELGANGER)
            mptr = &mons[mnum];
        use_saved_traits = FALSE;
    } 
    else if (is_golem(mptr) && cause == ANIMATE_SPELL)
    {
        /* statue of any golem hit by stone-to-flesh becomes flesh golem */
        golem_xform = (mptr != &mons[PM_FLESH_GOLEM]);
        mnum = PM_FLESH_GOLEM;
        mptr = &mons[PM_FLESH_GOLEM];
        use_saved_traits = (has_omonst(statue) && !golem_xform);
    } 
    else 
    {
        use_saved_traits = has_omonst(statue);
    }

    if (use_saved_traits) 
    {
        /* restore a petrified monster */
        cc.x = x, cc.y = y;
        mon = montraits(statue, &cc, (cause == ANIMATE_SPELL), NON_PM, NON_PM, 0UL);
        if (mon && mon->mtame && !mon->isminion)
            wary_dog(mon, TRUE);
    }
    else 
    {
        /* statues of unique monsters from bones or wishing end
           up here (cant_revive() sets mnum to be doppelganger;
           mptr reflects the original form for use by newcham()) */
        if ((mnum == PM_DOPPELGANGER && mptr != &mons[PM_DOPPELGANGER])
            /* block quest guards from other roles */
            || (mptr->msound == MS_GUARDIAN
                && quest_info(MS_GUARDIAN) != mnum)) 
        {
            mon = makemon(&mons[PM_DOPPELGANGER], x, y,
                          MM_NO_MONSTER_INVENTORY | MM_NOCOUNTBIRTH | MM_ADJACENTOK);
            /* if hero has protection from shape changers, cham field will
               be NON_PM; otherwise, set form to match the statue */
            if (mon && mon->cham >= LOW_PM)
                (void) newcham(mon, mptr, 0, FALSE, FALSE);
        } 
        else
        {
            boolean isfemale = !!(statue->speflags & SPEFLAGS_FEMALE);
            boolean ismale = !!(statue->speflags & SPEFLAGS_FEMALE);

            unsigned long mkflags = MM_NO_MONSTER_INVENTORY;
            if (isfemale)
                mkflags |= MM_FEMALE;
            else if (ismale)
                mkflags |= MM_MALE;

            if(cause == ANIMATE_SPELL)
                mkflags |= MM_ADJACENTOK;

            mon = makemon(mptr, x, y, mkflags);
        }
    }

    if (!mon) 
    {
        if (fail_reason)
            *fail_reason = unique_corpstat(&mons[statue->corpsenm])
                               ? AS_MON_IS_UNIQUE
                               : AS_NO_MON;
        return (struct monst *) 0;
    }

    /* a non-montraits() statue might specify gender */
    if (statue->speflags & SPEFLAGS_FEMALE)
        mon->female = TRUE;
    else if (statue->speflags & SPEFLAGS_MALE)
        mon->female = FALSE;

    /* if statue has been named, give same name to the monster */
    /* //Applies only to nicknames, other stats are recovered from OMONST -- JG
    if (has_oname(statue) && !unique_corpstat(mon->data))
    {
        mon = christen_monst(mon, ONAME(statue));
        if(statue->nknown)
            mon->u_know_mname = 1;
    }
    */
    if (has_uoname(statue) && !unique_corpstat(mon->data))
    {
        mon = u_name_monst(mon, UONAME(statue));
    }
    /* mimic statue becomes seen mimic; other hiders won't be hidden */
    if (M_AP_TYPE(mon))
        seemimic(mon);
    else
        mon->mundetected = FALSE;
    mon->msleeping = 0;
    if (cause == ANIMATE_NORMAL || cause == ANIMATE_SHATTER)
    {
        /* trap always releases hostile monster */
        mon->mtame = 0; /* (might be petrified pet tossed onto trap) */
        mon->mpeaceful = 0;
        mon->ispartymember = FALSE;
        set_mhostility(mon);
    }

    if (isok(mon->mx, mon->my))
        newsym(mon->mx, mon->my);

    comes_to_life = !canspotmon(mon)
                        ? "disappears"
                        : golem_xform
                              ? "turns into flesh"
                              : (is_not_living(mon->data) || is_vampshifter(mon))
                                    ? "moves"
                                    : "comes to life";
    if ((x == u.ux && y == u.uy) || cause == ANIMATE_SPELL) 
    {
        play_sfx_sound(SFX_STATUE_BECOMES_ALIVE);
        /* "the|your|Manlobbi's statue [of a wombat]" */
        shkp = shop_keeper(*in_rooms(mon->mx, mon->my, SHOPBASE));
        Sprintf(statuename, "%s%s", shk_your(tmpbuf, statue),
                (cause == ANIMATE_SPELL
                 /* avoid "of a shopkeeper" if it's Manlobbi himself
                    (if carried, it can't be unpaid--hence won't be
                    described as "Manlobbi's statue"--because there
                    wasn't any living shk when statue was picked up) */
                 && (mon != shkp || carried(statue)))
                   ? xname(statue)
                   : "statue");
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s!", upstart(statuename), comes_to_life);
    } 
    else if (Hallucination) 
    { /* They don't know it's a statue */
        play_sfx_sound(SFX_STATUE_BECOMES_ALIVE);
        pline_The_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s suddenly seems more animated.", rndmonnam((char *) 0));
    } 
    else if (cause == ANIMATE_SHATTER) 
    {
        play_sfx_sound(SFX_STATUE_BECOMES_ALIVE);
        if (cansee(x, y))
            Sprintf(statuename, "%s%s", shk_your(tmpbuf, statue),
                    xname(statue));
        else
            Strcpy(statuename, "a statue");
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Instead of shattering, %s suddenly %s!", statuename,
              comes_to_life);
    } 
    else 
    { /* cause == ANIMATE_NORMAL */
        play_sfx_sound(SFX_TRAP_FOUND);
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "find %s posing as a statue.",
            canspotmon(mon) ? a_monnam(mon) : something);
        if (!canspotmon(mon) && Blind)
            map_invisible(x, y);
        stop_occupation();
    }

    /* if this isn't caused by a monster using a wand of striking,
       there might be consequences for the hero */
    if (!context.mon_moving) 
    {
        /* if statue is owned by a shop, hero will have to pay for it;
           stolen_value gives a message (about debt or use of credit)
           which refers to "it" so needs to follow a message describing
           the object ("the statue comes to life" one above) */
        if (cause != ANIMATE_NORMAL && costly_spot(x, y)
            && (carried(statue) ? statue->unpaid : !statue->no_charge)
            && (shkp = shop_keeper(*in_rooms(x, y, SHOPBASE))) != 0
            /* avoid charging for Manlobbi's statue of Manlobbi
               if stone-to-flesh is used on petrified shopkeep */
            && mon != shkp)
            (void) stolen_value(statue, x, y, is_peaceful(shkp) != 0,
                                FALSE);

        if (historic) {
            play_sfx_sound(SFX_GUILTY);
            You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "guilty %s.", historic_statue_is_gone);
            adjalign(-1);
        }
    } 
    else 
    {
        if (historic && cansee(x, y))
            You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "regret %s.", historic_statue_is_gone);
        /* no alignment penalty */
    }

    /* transfer any statue contents to monster's inventory */
    while ((item = statue->cobj) != 0) {
        obj_extract_self(item);
        (void) mpickobj(mon, item);
    }
    m_dowear(mon, TRUE);

    /* in case statue is wielded and hero zaps stone-to-flesh at self */
    if (statue->owornmask)
        remove_worn_item(statue, TRUE);
    /* statue no longer exists */
    delobj(statue);

    /* avoid hiding under nothing */
    if (x == u.ux && y == u.uy && Upolyd && hides_under(youmonst.data)
        && !OBJ_AT(x, y))
        u.uundetected = 0;

    if (fail_reason)
        *fail_reason = AS_OK;
    return mon;
}

/*
 * You've either stepped onto a statue trap's location or you've triggered a
 * statue trap by searching next to it or by trying to break it with a wand
 * or pick-axe.
 */
struct monst *
activate_statue_trap(trap, x, y, shatter)
struct trap *trap;
xchar x, y;
boolean shatter;
{
    struct monst *mtmp = (struct monst *) 0;
    struct obj *otmp = sobj_at(STATUE, x, y);
    int fail_reason;

    /*
     * Try to animate the first valid statue.  Stop the loop when we
     * actually create something or the failure cause is not because
     * the mon was unique.
     */
    deltrap(trap);
    while (otmp) {
        mtmp = animate_statue(otmp, x, y,
                              shatter ? ANIMATE_SHATTER : ANIMATE_NORMAL,
                              &fail_reason);
        if (mtmp || fail_reason != AS_MON_IS_UNIQUE)
            break;

        otmp = nxtobj(otmp, STATUE, TRUE);
    }

    feel_newsym(x, y);
    return mtmp;
}

STATIC_OVL boolean
keep_saddle_with_steedcorpse(steed_mid, objchn, saddle)
unsigned steed_mid;
struct obj *objchn, *saddle;
{
    if (!saddle)
        return FALSE;
    while (objchn) {
        if (objchn->otyp == CORPSE && has_omonst(objchn)) {
            struct monst *mtmp = OMONST(objchn);

            if (mtmp->m_id == steed_mid) {
                /* move saddle */
                xchar x, y;
                if (get_obj_location(objchn, &x, &y, 0)) {
                    obj_extract_self(saddle);
                    place_object(saddle, x, y);
                    stackobj(saddle);
                }
                return TRUE;
            }
        }
        if (Has_contents(objchn)
            && keep_saddle_with_steedcorpse(steed_mid, objchn->cobj, saddle))
            return TRUE;
        objchn = objchn->nobj;
    }
    return FALSE;
}

/* monster or you go through and possibly destroy a web.
   return TRUE if could go through. */
boolean
mu_maybe_destroy_web(mtmp, domsg, trap)
struct monst *mtmp;
boolean domsg;
struct trap *trap;
{
    boolean isyou = (mtmp == &youmonst);
    struct permonst *mptr = mtmp->data;

    if (amorphous(mptr) || is_whirly(mptr) || flaming(mptr)
        || unsolid(mptr) || is_incorporeal(mptr) || slurps_items(mptr))
    {
        xchar x = trap->tx;
        xchar y = trap->ty;

        if (flaming(mptr) || has_acidic_corpse(mptr)) {
            if (domsg) {
                play_sfx_sound(SFX_BURN_SPIDER_WEB);

                if (isyou)
                    You("%s %s spider web!",
                        (flaming(mptr)) ? "burn" : "dissolve",
                        a_your[trap->madeby_u]);
                else
                    pline("%s %s %s spider web!", Monnam(mtmp),
                          (flaming(mptr)) ? "burns" : "dissolves",
                          a_your[trap->madeby_u]);
            }
            deltrap(trap);
            newsym(x, y);
            return TRUE;
        }
        if (domsg) {
            if (isyou) {
                You("flow through %s spider web.", a_your[trap->madeby_u]);
            } else {
                pline("%s flows through %s spider web.", Monnam(mtmp),
                      a_your[trap->madeby_u]);
                seetrap(trap);
            }
        }
        return TRUE;
    }
    return FALSE;
}

/* make a single arrow/dart/rock for a trap to shoot or drop */
STATIC_OVL struct obj *
t_missile(otyp, trap)
int otyp;
struct trap *trap;
{
    struct obj *otmp = mksobj_with_flags(otyp, TRUE, FALSE, MKOBJ_TYPE_GENERATED, (struct monst*)0, MAT_NONE, 0L, 0L, MKOBJ_FLAGS_FORCE_BASE_MATERIAL);

    otmp->quan = 1L;
    otmp->opoisoned = 0;
    otmp->elemental_enchantment = 0;
    otmp->mythic_prefix = 0;
    otmp->mythic_suffix = 0;
    otmp->ox = trap->tx, otmp->oy = trap->ty;
    otmp->owt = weight(otmp);
    return otmp;
}

void
set_utrap(tim, typ)
unsigned long tim;
uchar typ;
{
    u.utrap = tim;
    u.utraptype = typ;

    float_vs_flight(); /* maybe block Lev and/or Fly */
}

void
reset_utrap(msg)
boolean msg;
{
    boolean was_Lev = (Levitation != 0), was_Fly = (Flying != 0);

    set_utrap(0, 0);

    if (msg) {
        if (!was_Lev && Levitation)
            float_up();
        if (!was_Fly && Flying)
            You("can fly.");
    }
}

void
dotrap(trap, trflags)
register struct trap *trap;
unsigned short trflags;
{
    register int ttype = trap->ttyp;
    const char* trapdesc = get_trap_explanation(trap);
    struct obj *otmp;
    boolean already_seen = trap->tseen,
            forcetrap = ((trflags & FORCETRAP) != 0
                         || (trflags & FAILEDUNTRAP) != 0),
            webmsgok = (trflags & NOWEBMSG) == 0,
            forcebungle = (trflags & FORCEBUNGLE) != 0,
            plunged = (trflags & TOOKPLUNGE) != 0,
            viasitting = (trflags & VIASITTING) != 0,
            conj_pit = conjoined_pits(trap, t_at(u.ux0, u.uy0), TRUE),
            adj_pit = adj_nonconjoined_pit(trap);
    //int oldumort;
    int steed_article = ARTICLE_THE;

    nomul(0);

    /* KMH -- You can't escape the Sokoban level traps */
    if (Sokoban && (is_pit(ttype) || is_hole(ttype))) 
    {
        /* The "air currents" message is still appropriate -- even when
         * the hero isn't flying or levitating -- because it conveys the
         * reason why the player cannot escape the trap with a dexterity
         * check, clinging to the ceiling, etc.
         */
        pline_ex(ATR_NONE, CLR_MSG_MYSTICAL, "Air currents pull you down into %s %s!",
              a_your[trap->madeby_u],
              trapdesc);
        /* then proceed to normal trap effect */
    }
    else if (already_seen && !forcetrap) 
    {
        if ((Levitation || (Flying && !plunged))
            && (is_pit(ttype) || ttype == HOLE || ttype == BEAR_TRAP))
        {
            You("%s over %s %s.", Levitation ? "float" : "fly",
                a_your[trap->madeby_u],
                trapdesc);
            return;
        }

        if (!Fumbling && !(trap_type_definitions[ttype].tdflags & TRAPDEF_FLAGS_NO_TRY_ESCAPE) 
            && !forcebungle && !plunged
            && !conj_pit && !adj_pit
            && (!rn2(5) || (is_pit(ttype)
                            && is_clinger(youmonst.data))))
        {
            play_sfx_sound(SFX_ESCAPE_TRAP);
            if(trap->madeby_u)
                You("escape your %s.", trapdesc);
            else
                You("escape %s.", an(trapdesc));

            return;
        }
    }

    if (u.usteed) 
    {
        u.usteed->mtrapseen |= (1 << (ttype - 1));
        /* suppress article in various steed messages when using its
           name (which won't occur when hallucinating) */
        if (has_mname(u.usteed) && !Hallucination)
            steed_article = ARTICLE_NONE;
    }

    switch (ttype) 
    {
    case ARROW_TRAP:
        if (trap->once && trap->tseen && !rn2(15)) {
            play_sfx_sound(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE);
            You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "a loud click!");
            deltrap(trap);
            newsym(u.ux, u.uy);
            break;
        }
        trap->once = 1;
        seetrap(trap);
        otmp = t_missile(get_shooting_trap_object(trap), trap);
        play_sfx_sound(SFX_ARROW_TRAP_FIRE);
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s shoots out at you!", An(cxname_singular(otmp)));
        if (u.usteed && !rn2(2) && steedintrap(trap, otmp))
        {
            ; /* nothing */
        } 
        else if (thitu(8, weapon_total_dmg_value(otmp, &youmonst, (struct monst*)0, 1), &otmp, (const char*)0, (struct monst*)0, (char*)0))
        {
            if (otmp)
                obfree(otmp, (struct obj *) 0);
        } else {
            place_object(otmp, u.ux, u.uy);
            if (!Blind)
                otmp->dknown = 1;
            stackobj(otmp);
            newsym(u.ux, u.uy);
        }
        break;

    case DART_TRAP:
        if (trap->once && trap->tseen && !rn2(15)) {
            play_sfx_sound(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE);
            You_hear_ex(ATR_NONE, CLR_MSG_WARNING, "a soft click.");
            deltrap(trap);
            newsym(u.ux, u.uy);
            break;
        }
        trap->once = 1;
        seetrap(trap);
        play_sfx_sound(SFX_DART_TRAP_FIRE);
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A little dart shoots out at you!");
        otmp = t_missile(get_shooting_trap_object(trap), trap);
        if (!rn2(6))
            otmp->opoisoned = 1;
        //oldumort = u.umortality;

        if (u.usteed && !rn2(2) && steedintrap(trap, otmp)) 
        {
            ; /* nothing */
        } 
        else if (thitu(7, weapon_total_dmg_value(otmp, &youmonst, (struct monst*)0, 1), &otmp, "little dart", (struct monst*)0, (char*)0)) 
        {
            if (otmp)
            {
                if (otmp->opoisoned)
                    poisoned("dart", A_CON, "little dart",
                             /* if damage triggered life-saving,
                                poison is limited to attrib loss */
                             0, TRUE, 2);
                obfree(otmp, (struct obj *) 0);
            }
        } 
        else 
        {
            place_object(otmp, u.ux, u.uy);
            if (!Blind)
                otmp->dknown = 1;
            stackobj(otmp);
            newsym(u.ux, u.uy);
        }
        break;

    case ROCKTRAP:
        play_special_effect_at(SPECIAL_EFFECT_FALLING_ROCK_TRAP_TRAP_DOOR, 0, trap->tx, trap->ty, FALSE);
        play_sfx_sound(SFX_FALLING_ROCK_TRAP_TRIGGER);
        special_effect_wait_until_action(0);
        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        if (trap->once && trap->tseen && !rn2(15))
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "A trap door in %s opens, but nothing falls out!",
                  the(ceiling(u.ux, u.uy)));
            deltrap(trap);
            newsym(u.ux, u.uy);
        }
        else 
        {
            trap->once = 1;
            feeltrap(trap);

            play_special_effect_at(SPECIAL_EFFECT_FALLING_ROCK_TRAP_FALLING_ROCK, 1, trap->tx, trap->ty, FALSE);

            otmp = t_missile(get_shooting_trap_object(trap), trap);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A trap door in %s opens and %s falls on your %s!",
                the(ceiling(u.ux, u.uy)), an(xname(otmp)), body_part(HEAD));

            special_effect_wait_until_action(1);

            int dmg = d(2, 6); /* should be std ROCK dmg? */
            place_object(otmp, u.ux, u.uy);

            if (uarmh) 
            {
                if (is_hard_helmet(uarmh))
                {
                    if (flags.verbose)
                    {
                        if (iflags.using_gui_sounds)
                        {
                            //delay_output_milliseconds(150);
                            play_sfx_sound(SFX_ROCK_HITS_HARD_HELMET);
                        }
                        pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "Fortunately, you are wearing a hard helmet.");
                    }
                    dmg = 2;
                } 
                else 
                {
                    if (flags.verbose)
                    {
                        if (iflags.using_gui_sounds)
                        {
                            //delay_output_milliseconds(150);
                            play_sfx_sound(SFX_ROCK_HITS_YOU_ON_HEAD);
                        }
                        pline("%s does not protect you.", Yname2(uarmh));
                        if (iflags.using_gui_sounds)
                        {
                            delay_output_milliseconds(150);
                            play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
                        }
                    }
                }
            }
            else
            {
                if (iflags.using_gui_sounds)
                {
                    //delay_output_milliseconds(150);
                    play_sfx_sound(SFX_ROCK_HITS_YOU_ON_HEAD);
                    delay_output_milliseconds(150);
                    play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
                }
            }
            if (!Blind)
                otmp->dknown = 1;
            stackobj(otmp);
            newsym(u.ux, u.uy); /* map the rock */

            losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "falling rock", KILLED_BY_AN);
            exercise(A_STR, FALSE);
            special_effect_wait_until_end(1);
        }
        context.global_newsym_flags = 0UL;
        special_effect_wait_until_end(0);
        break;

    case SQKY_BOARD: /* stepped on a squeaky board */
        if ((Levitation || Flying) && !forcetrap) 
        {
            if (!Blind) 
            {
                seetrap(trap);
                if (Hallucination)
                    You("notice a crease in the linoleum.");
                else
                    You("notice a loose board below you.");
            }
        } 
        else 
        {
            play_sfx_sound(SFX_SQUEAKY_BOARD);
            seetrap(trap);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A board beneath you %s%s%s.",
                  Deaf ? "vibrates" : "squeaks ",
                  Deaf ? "" : trapnote(trap, 0), Deaf ? "" : " loudly");
            wake_nearby();
        }
        break;

    case BEAR_TRAP: 
    {
        int dmg = d(2, 4);

        if ((Levitation || Flying) && !forcetrap)
            break;
        feeltrap(trap);

        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        struct layer_info layers = layers_at(trap->tx, trap->ty);
        int old_glyph = layers.layer_glyphs[LAYER_TRAP];
        int old_gui_glyph = layers.layer_gui_glyphs[LAYER_TRAP];
        int old_cover_glyph = layers.layer_glyphs[LAYER_COVER_TRAP];
        int old_cover_gui_glyph = layers.layer_gui_glyphs[LAYER_COVER_TRAP];
        show_glyph_on_layer(trap->tx, trap->ty, NO_GLYPH, LAYER_TRAP);
        show_glyph_on_layer(trap->tx, trap->ty, NO_GLYPH, LAYER_COVER_TRAP);
        flush_screen(1);
        play_special_effect_at(SPECIAL_EFFECT_BEAR_TRAP_CLOSE, 0, trap->tx, trap->ty, FALSE);
        play_sfx_sound(SFX_BEAR_TRAP_CLOSES);
        special_effect_wait_until_action(0);

        if (amorphous(youmonst.data) || is_whirly(youmonst.data)
            || unsolid(youmonst.data) || is_incorporeal(youmonst.data))
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s bear trap closes harmlessly through you.",
                  A_Your[trap->madeby_u]);

            special_effect_wait_until_end(0);
            play_special_effect_at(SPECIAL_EFFECT_BEAR_TRAP_OPEN, 0, trap->tx, trap->ty, FALSE);
            special_effect_wait_until_action(0);
            special_effect_wait_until_end(0);
            show_gui_glyph_on_layer(trap->tx, trap->ty, old_glyph, old_gui_glyph, LAYER_TRAP);
            show_gui_glyph_on_layer(trap->tx, trap->ty, old_cover_glyph, old_cover_gui_glyph, LAYER_COVER_TRAP);
            flush_screen(1);
            break;
        }

        if (!u.usteed && youmonst.data->msize <= MZ_SMALL) 
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s bear trap closes harmlessly over you.",
                  A_Your[trap->madeby_u]);

            special_effect_wait_until_end(0);
            play_special_effect_at(SPECIAL_EFFECT_BEAR_TRAP_OPEN, 0, trap->tx, trap->ty, FALSE);
            special_effect_wait_until_action(0);
            special_effect_wait_until_end(0);
            show_gui_glyph_on_layer(trap->tx, trap->ty, old_glyph, old_gui_glyph, LAYER_TRAP);
            show_gui_glyph_on_layer(trap->tx, trap->ty, old_cover_glyph, old_cover_gui_glyph, LAYER_COVER_TRAP);
            flush_screen(1);
            break;
        }
        set_utrap((unsigned) rn1(4, 4), TT_BEARTRAP);
        if (u.usteed) 
        {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s bear trap closes on %s %s!", A_Your[trap->madeby_u],
                  s_suffix(mon_nam(u.usteed)), mbodypart(u.usteed, FOOT));
            if (thitm(0, u.usteed, (struct obj*)0, dmg, FALSE))
            {
                special_effect_wait_until_end(0);
                play_special_effect_at(SPECIAL_EFFECT_BEAR_TRAP_OPEN, 0, trap->tx, trap->ty, FALSE);
                special_effect_wait_until_action(0);
                reset_utrap(TRUE); /* steed died, hero not trapped */
            }
        } 
        else 
        {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s bear trap closes on your %s!", A_Your[trap->madeby_u],
                  body_part(FOOT));
            set_wounded_legs(rn2(2) ? RIGHT_SIDE : LEFT_SIDE, rn1(10, 10));
            if (is_bear(&mons[u.umonnum]))
            {
                play_monster_unhappy_sound(&youmonst, MONSTER_UNHAPPY_SOUND_HOWL_IN_ANGER);
                You("howl in anger!");
            }
            losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "bear trap", KILLED_BY_AN);
        }
        special_effect_wait_until_end(0);
        context.global_newsym_flags = 0UL;
        newsym(trap->tx, trap->ty);
        flush_screen(1);
        exercise(A_DEX, FALSE);
        break;
    }

    case SLP_GAS_TRAP:
        seetrap(trap);
        play_special_effect_at(SPECIAL_EFFECT_TRAP_SLEEP_GAS, 0, trap->tx, trap->ty, FALSE);
        play_sfx_sound(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE);
        play_sfx_sound(SFX_ENVELOPED_IN_CLOUD_OF_GAS);
        special_effect_wait_until_action(0);
        if (Sleep_resistance || has_innate_breathless(youmonst.data))
        {
            You_ex(ATR_NONE, CLR_MSG_ATTENTION, "are enveloped in a cloud of gas!");
        } 
        else 
        {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A cloud of gas puts you to sleep!");
            fall_asleep(-rn1(7,8), TRUE);
            standard_hint("You should acquire sleep resistance as early as possible. Keep pets around to protect you while sleeping.", &u.uhint.fell_asleep_by_trap);
        }

        (void) steedintrap(trap, (struct obj *) 0);
        special_effect_wait_until_end(0);
        break;

    case RUST_TRAP:
        seetrap(trap);

        /* Unlike monsters, traps cannot aim their rust attacks at
         * you, so instead of looping through and taking either the
         * first rustable one or the body, we take whatever we get,
         * even if it is not rustable.
         */
        play_special_effect_at(SPECIAL_EFFECT_TRAP_RUST, 0, trap->tx, trap->ty, FALSE);
        play_sfx_sound(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE);
        play_sfx_sound(SFX_GUSH_OF_WATER_HITS);
        special_effect_wait_until_action(0);
        switch (rn2(5))
        {
        case 0:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s you on the %s!", A_gush_of_water_hits, body_part(HEAD));
            (void) water_damage(uarmh, helm_simple_name(uarmh), TRUE);
            break;
        case 1:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s your left %s!", A_gush_of_water_hits, body_part(ARM));
            if (uarms && water_damage(uarms, is_shield(uarms) ? "shield" : "weapon", TRUE) != ER_NOTHING)
                break;
            else if (uwep && bimanual(uwep))
                (void) water_damage(uwep, 0, TRUE);
        glovecheck:
            (void) water_damage(uarmg, "gauntlets", TRUE);
            /* Not "metal gauntlets" since it gets called
             * even if it's leather for the message
             */
            break;
        case 2:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s your right %s!", A_gush_of_water_hits, body_part(ARM));
            (void) water_damage(uwep, 0, TRUE);
            goto glovecheck;
        default:
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s you!", A_gush_of_water_hits);
            for (otmp = invent; otmp; otmp = otmp->nobj)
                if (otmp->lamplit && otmp != uwep && otmp != uarms)
                    (void) snuff_lit(otmp);
            if (uarmc)
                (void) water_damage(uarmc, cloak_simple_name(uarmc), TRUE);
            else if (uarmo)
                (void)water_damage(uarmo, robe_simple_name(uarmo), TRUE);
            else if (uarm)
                (void) water_damage(uarm, suit_simple_name(uarm), TRUE);
            else if (uarmu)
                (void) water_damage(uarmu, "shirt", TRUE);
        }
        update_inventory();

        if (is_iron(youmonst.data)) 
        {
            int dam = u.mhmax;

            play_sfx_sound(SFX_YOU_RUST);
            You("are covered with rust!");
            losehp(adjust_damage(dam, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "rusting away", KILLED_BY);
        }
        else if (u.umonnum == PM_GREMLIN && rn2(3))
        {
            play_sfx_sound(SFX_YOU_SPLIT);
            (void) split_mon(&youmonst, (struct monst *) 0);
        }

        special_effect_wait_until_end(0);
        break;

    case FIRE_TRAP:
        seetrap(trap);
        play_special_effect_at(SPECIAL_EFFECT_TRAP_FIRE, 0, trap->tx, trap->ty, FALSE);
        play_sfx_sound(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE);
        special_effect_wait_until_action(0);
        dofiretrap((struct obj *) 0, In_endgame(&u.uz) ? 25 : max(2, min(20, (level_difficulty() * 2) / 3)));
        special_effect_wait_until_end(0);
        break;

    case PIT:
    case SPIKED_PIT:
        /* KMH -- You can't escape the Sokoban level traps */
        if (!Sokoban && (Levitation || (Flying && !plunged)))
            break;
        feeltrap(trap);
        if (!Sokoban && is_clinger(youmonst.data) && !plunged)
        {
            if (trap->tseen)
            {
                You_see("%s %spit below you.", a_your[trap->madeby_u],
                        ttype == SPIKED_PIT ? "spiked " : "");
            }
            else 
            {
                play_sfx_sound(SFX_TRAP_FOUND);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s pit %sopens up under you!", A_Your[trap->madeby_u],
                      ttype == SPIKED_PIT ? "full of spikes " : "");
                You("don't fall in!");
            }
            break;
        }
        if (!Sokoban)
        {
            char verbbuf[BUFSZ];

            *verbbuf = '\0';
            if (u.usteed) {
                if ((trflags & RECURSIVETRAP) != 0)
                    Sprintf(verbbuf, "and %s fall",
                            x_monnam(u.usteed, steed_article, (char *) 0,
                                     SUPPRESS_SADDLE, FALSE));
                else
                    Sprintf(verbbuf, "lead %s",
                            x_monnam(u.usteed, steed_article, "poor",
                                     SUPPRESS_SADDLE, FALSE));
            }
            else if (conj_pit)
            {
                You("move into an adjacent pit.");
            }
            else if (adj_pit)
            {
                You("stumble over debris%s.",
                    !rn2(5) ? " between the pits" : "");
            } 
            else
            {
                Strcpy(verbbuf,
                       !plunged ? "fall" : (Flying ? "dive" : "plunge"));
            }

            if (*verbbuf)
            {
                play_sfx_sound(SFX_FALL_INTO_PIT);
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s into %s pit!", verbbuf, a_your[trap->madeby_u]);
            }
        }
        /* wumpus reference */
        if (Role_if(PM_RANGER) && !trap->madeby_u && !trap->once
            && In_quest(&u.uz) && Is_qlocate(&u.uz))
        {
            pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "Fortunately it has a bottom after all...");
            trap->once = 1;
        } 
        else if (u.umonnum == PM_PIT_VIPER || u.umonnum == PM_PIT_FIEND)
        {
            pline("How pitiful.  Isn't that the pits?");
        }

        if (ttype == SPIKED_PIT) 
        {
            const char *predicament = "on a set of sharp iron spikes";

            play_sfx_sound(SFX_LAND_ON_SPIKES);
            if (u.usteed) 
            {
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s %s!",
                      upstart(x_monnam(u.usteed, steed_article, "poor",
                                       SUPPRESS_SADDLE, FALSE)),
                      conj_pit ? "steps" : "lands", predicament);
            } 
            else
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s!", conj_pit ? "step" : "land", predicament);
        }

        /* FIXME:
         * if hero gets killed here, setting u.utrap in advance will
         * show "you were trapped in a pit" during disclosure's display
         * of enlightenment, but hero is dying *before* becoming trapped.
         */
        set_utrap((unsigned) rn1(6, 2), TT_PIT);
        if (!steedintrap(trap, (struct obj *) 0)) 
        {
            if (ttype == SPIKED_PIT)
            {
                //oldumort = u.umortality;
                losehp(adjust_damage(rnd(conj_pit ? 4 : adj_pit ? 6 : 10), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE),
                       /* note: these don't need locomotion() handling;
                          if fatal while poly'd and Unchanging, the
                          death reason will be overridden with
                          "killed while stuck in creature form" */
                       plunged
                          ? "deliberately plunged into a pit of iron spikes"
                          : conj_pit
                             ? "stepped into a pit of iron spikes"
                             : adj_pit
                                ? "stumbled into a pit of iron spikes"
                                : "fell into a pit of iron spikes",
                       NO_KILLER_PREFIX);

                if (!rn2(6))
                    poisoned("spikes", A_STR,
                             (conj_pit || adj_pit)
                                ? "stepping on poison spikes"
                                : "fall onto poison spikes",
                             /* if damage triggered life-saving,
                                poison is limited to attrib loss */
                                0, FALSE, 2);
            } 
            else 
            {
                /* plunging flyers take spike damage but not pit damage */
                if (!conj_pit
                    && !(plunged && (Flying || is_clinger(youmonst.data))))
                    losehp(adjust_damage(rnd(adj_pit ? 3 : 6), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE),
                           plunged ? "deliberately plunged into a pit"
                                   : "fell into a pit",
                           NO_KILLER_PREFIX);
            }

            if (Punished && !carried(uball)) 
            {
                unplacebc();
                ballfall();
                placebc();
            }

            if (!conj_pit)
                selftouch("Falling, you");

            vision_full_recalc = 1; /* vision limits change */
            exercise(A_STR, FALSE);
            exercise(A_DEX, FALSE);
        }
        break;

    case HOLE:
    case TRAPDOOR:
        if (!Can_fall_thru(&u.uz))
        {
            seetrap(trap); /* normally done in fall_through */
            impossible("dotrap: %ss cannot exist on this level.",
                       defsyms[trap_to_defsym(ttype)].explanation);
            break; /* don't activate it after all */
        }
        play_sfx_sound(ttype == HOLE ? SFX_HOLE_OPENS : SFX_TRAP_DOOR_OPENS);
        fall_through(TRUE);
        break;

    case TELEP_TRAP:
        seetrap(trap);
        tele_trap(trap);
        break;

    case LEVEL_TELEP:
        seetrap(trap);
        level_tele_trap(trap, trflags);
        break;

    case WEB: /* Our luckless player has stumbled into a web. */
        feeltrap(trap);
        if (mu_maybe_destroy_web(&youmonst, webmsgok, trap))
            break;

        if (webmaker(youmonst.data)) 
        {
            if (webmsgok)
                pline(trap->madeby_u ? "You take a walk on your web."
                                     : "There is a spider web here.");
            break;
        }

        if (webmsgok)
        {
            char verbbuf[BUFSZ];

            play_sfx_sound(SFX_CAUGHT_IN_WEB);
            if (forcetrap || viasitting) 
            {
                Strcpy(verbbuf, "are caught by");
            }
            else if (u.usteed)
            {
                Sprintf(verbbuf, "lead %s into",
                        x_monnam(u.usteed, steed_article, "poor",
                                 SUPPRESS_SADDLE, FALSE));
            } 
            else 
            {
                Sprintf(verbbuf, "%s into",
                        Levitation ? (const char *) "float"
                                   : locomotion(youmonst.data, "stumble"));
            }
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s spider web!", verbbuf, a_your[trap->madeby_u]);
        }

        /* time will be adjusted below */
        set_utrap(1, TT_WEB);

        /* Time stuck in the web depends on your/steed strength. */
        {
            int tim, str = ACURR(A_STR);

            /* If mounted, the steed gets trapped.  Use mintrap
             * to do all the work.  If mtrapped is set as a result,
             * unset it and set utrap instead.  In the case of a
             * strongmonst and mintrap said it's trapped, use a
             * short but non-zero trap time.  Otherwise, monsters
             * have no specific strength, so use player strength.
             * This gets skipped for webmsgok, which implies that
             * the steed isn't a factor.
             */
            if (u.usteed && webmsgok) 
            {
                /* mtmp location might not be up to date */
                u.usteed->mx = u.ux;
                u.usteed->my = u.uy;

                /* mintrap currently does not return 2(died) for webs */
                if (mintrap(u.usteed)) 
                {
                    u.usteed->mtrapped = 0;
                    if (strongmonst(u.usteed->data))
                        str = 17;
                } 
                else 
                {
                    reset_utrap(FALSE);
                    break;
                }

                webmsgok = FALSE; /* mintrap printed the messages */
            }

            if (str <= 3)
                tim = rn1(6, 6);
            else if (str < 6)
                tim = rn1(6, 4);
            else if (str < 9)
                tim = rn1(4, 4);
            else if (str < 12)
                tim = rn1(4, 2);
            else if (str < 15)
                tim = rn1(2, 2);
            else if (str < 18)
                tim = rnd(2);
            else if (str < 69)
                tim = 1;
            else 
            {
                tim = 0;
                if (webmsgok)
                {
                    play_sfx_sound(SFX_TEAR_THROUGH_WEB);
                    You("tear through %s web!", a_your[trap->madeby_u]);
                }
                deltrap(trap);
                newsym(u.ux, u.uy); /* get rid of trap symbol */
            }
            set_utrap((unsigned) tim, TT_WEB);
        }
        break;

    case STATUE_TRAP:
        play_sfx_sound(SFX_GENERIC_MAGICAL_TRAP_ACTIVATE);
        (void) activate_statue_trap(trap, u.ux, u.uy, FALSE);
        break;

    case MAGIC_TRAP: /* A magic trap. */
        seetrap(trap);
        play_sfx_sound(SFX_MAGIC_TRAP_ACTIVATE);
        if (!rn2(30))
        {
            play_special_effect_at(SPECIAL_EFFECT_SMALL_FIERY_EXPLOSION, 0, u.ux, u.uy, FALSE);
            play_sfx_sound(SFX_EXPLOSION_MAGICAL);
            special_effect_wait_until_action(0);
            deltrap(trap);
            newsym(u.ux, u.uy); /* update position */
            You_ex(ATR_NONE, CLR_MSG_WARNING, "are caught in a magical explosion!");
            losehp(adjust_damage(rnd(10), (struct monst*)0, &youmonst, AD_MAGM, ADFLAGS_NONE), "magical explosion", KILLED_BY_AN);
            Your("body absorbs some of the magical energy!");
            u.ubaseenmax += 2;
            u.uen = u.ubaseenmax;
            updatemaxen();
            special_effect_wait_until_end(0);
            break;
        }
        else 
        {
            domagictrap();
        }
        (void) steedintrap(trap, (struct obj *) 0);
        break;

    case ANTI_MAGIC_TRAP:
        seetrap(trap);
        play_special_effect_at(SPECIAL_EFFECT_ANTIMAGIC_TRAP_EFFECT, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_ANTI_MAGIC_TRAP_ACTIVATE);
        special_effect_wait_until_action(0);
        /* hero without magic resistance loses spell energy,
           hero with magic resistance takes damage instead;
           possibly non-intuitive but useful for play balance */
        if (!Antimagic_or_resistance)
        {
            int c = max(0, (u.uenmax * 3) / 10);
            int a = max(2, (u.uenmax * 2) / 10);
            drain_en(rnd(a) + c);
        } 
        else
        {
            int dmgval2 = rnd(4), hp = Upolyd ? u.mh : u.uhp;

            /* Half_XXX_damage has opposite its usual effect (approx)
               but isn't cumulative if hero has more than one */
            if (Half_physical_damage || Half_spell_damage)
                dmgval2 += rnd(4);
            /* give Magicbane wielder dose of own medicine */
            if ((uwep && uwep->oartifact && artifact_has_flag(uwep, AF_MAGIC_ABSORBING)) || (uarms && uarms->oartifact && artifact_has_flag(uarms, AF_MAGIC_ABSORBING)))
                dmgval2 += rnd(4);
            if (Passes_walls)
                dmgval2 = (dmgval2 + 3) / 4;

            play_sfx_sound(SFX_FEELING_LETHARGIC);
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, (dmgval2 >= hp) ? "unbearably torpid!"
                                     : (dmgval2 >= hp / 4) ? "very lethargic."
                                                           : "sluggish.");
            /* opposite of magical explosion */
            losehp(adjust_damage(dmgval2, (struct monst*)0, &youmonst, AD_MAGM, ADFLAGS_NONE), "anti-magical implosion", KILLED_BY_AN);
        }
        special_effect_wait_until_end(0);
        break;

    case POLY_TRAP: {
        char verbbuf[BUFSZ];

        seetrap(trap);
        play_special_effect_at(SPECIAL_EFFECT_POLYMORPH_TRAP_EFFECT, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_POLYMORPH_ACTIVATE);
        special_effect_wait_until_action(0);
        context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;

        if (viasitting)
            Strcpy(verbbuf, "trigger"); /* follows "You sit down." */
        else if (u.usteed)
            Sprintf(verbbuf, "lead %s onto",
                    x_monnam(u.usteed, steed_article, (char *) 0,
                             SUPPRESS_SADDLE, FALSE));
        else
            Sprintf(verbbuf, "%s onto",
                    Levitation ? (const char *) "float"
                               : locomotion(youmonst.data, "step"));
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s a polymorph trap!", verbbuf);
        if (Antimagic_or_resistance || Unchanging) {
            play_sfx_sound(SFX_POLYMORPH_FAIL);
            u_shieldeff();
            You_feel("momentarily different.");
            /* Trap did nothing; don't remove it --KAA */
        } else {
            (void) steedintrap(trap, (struct obj *) 0);
            deltrap(trap);      /* delete trap before polymorph */
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "a change coming over you.");
            polyself(0);
            newsym(u.ux, u.uy); /* get rid of trap symbol */
            standard_hint("You should acquire magic resistance, polymorph control, or an amulet of unchanging before dungeon level 8.", &u.uhint.polymorphed_by_trap);
        }
        context.global_newsym_flags = 0UL;
        special_effect_wait_until_end(0);
        break;
    }
    case LANDMINE: 
    {
        boolean spef_on = FALSE;
        unsigned steed_mid = 0;
        struct obj *saddle = 0;

        if ((Levitation || Flying) && !forcetrap) 
        {
            if (!already_seen && rn2(3))
                break;
            feeltrap(trap);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s in a pile of soil below you.",
                  already_seen ? "There is" : "You discover",
                  trap->madeby_u ? "the trigger of your mine" : "a trigger");

            if (already_seen && rn2(3))
                break;

            flush_screen(1);
            play_special_effect_at(SPECIAL_EFFECT_LAND_MINE_EXPLOSION, 0, trap->tx, trap->ty, FALSE);
            play_sfx_sound(SFX_LAND_MINE_ACTIVATE);
            special_effect_wait_until_action(0);
            play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, trap->tx, trap->ty);
            spef_on = TRUE;
            context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;

            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "KAABLAMM!!!  %s %s%s off!",
                  forcebungle ? "Your inept attempt sets"
                              : "The air currents set",
                  already_seen ? a_your[trap->madeby_u] : "",
                  already_seen ? " land mine" : "it");
        } 
        else 
        {
            /* prevent landmine from killing steed, throwing you to
             * the ground, and you being affected again by the same
             * mine because it hasn't been deleted yet
             */
            static boolean recursive_mine = FALSE;

            if (recursive_mine)
                break;
            feeltrap(trap);

            flush_screen(1);
            play_special_effect_at(SPECIAL_EFFECT_LAND_MINE_EXPLOSION, 0, trap->tx, trap->ty, FALSE);
            play_sfx_sound(SFX_LAND_MINE_ACTIVATE);
            special_effect_wait_until_action(0);
            play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, trap->tx, trap->ty);
            spef_on = TRUE;
            context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;

            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "KAABLAMM!!!  You triggered %s land mine!",
                  a_your[trap->madeby_u]);
            if (u.usteed)
                steed_mid = u.usteed->m_id;
            recursive_mine = TRUE;
            (void) steedintrap(trap, (struct obj *) 0);
            recursive_mine = FALSE;
            saddle = sobj_at(SADDLE, u.ux, u.uy);
            set_wounded_legs(LEFT_SIDE, rn1(35, 41));
            set_wounded_legs(RIGHT_SIDE, rn1(35, 41));
            exercise(A_DEX, FALSE);
        }
        blow_up_landmine(trap);
        if (steed_mid && saddle && !u.usteed)
            (void) keep_saddle_with_steedcorpse(steed_mid, fobj, saddle);
        newsym(u.ux, u.uy); /* update trap symbol */
        losehp(adjust_damage(rnd(16), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "land mine", KILLED_BY_AN);
        /* fall recursively into the pit... */
        if ((trap = t_at(u.ux, u.uy)) != 0)
            dotrap(trap, RECURSIVETRAP);
        fill_pit(u.ux, u.uy);

        if (spef_on)
        {
            special_effect_wait_until_end(0);
            context.global_newsym_flags = 0UL;
        }
        break;
    }

    case ROLLING_BOULDER_TRAP: 
    {
        if ((Levitation || Flying) && !forcetrap) 
        {
            if (!already_seen && rn2(3))
                break;
            feeltrap(trap);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s hidden in the floor below you.",
                already_seen ? "There is" : "You discover",
                trap->madeby_u ? "your rolling boulder trap" : "a rolling boulder trap");
        }
        else
        {
            int style = ROLL | (trap->tseen ? LAUNCH_KNOWN : 0);

            play_sfx_sound(SFX_ROLLING_BOOULDER_TRAP_TRIGGER);
            feeltrap(trap);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Click!  You trigger a rolling boulder trap!");
            if (!launch_obj(BOULDER, trap->launch.x, trap->launch.y,
                trap->launch2.x, trap->launch2.y, style)) {
                deltrap(trap);
                newsym(u.ux, u.uy); /* get rid of trap symbol */
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "Fortunately for you, no boulder was released.");
            }
        }
        break;
    }

    case MAGIC_PORTAL:
        feeltrap(trap);
        play_sfx_sound(SFX_GENERIC_MAGICAL_TRAP_ACTIVATE);
        domagicportal(trap);
        break;

    case VIBRATING_SQUARE:
        feeltrap(trap);
        /* messages handled elsewhere; the trap symbol is merely to mark the
         * square for future reference */
        break;

    case MODRON_PORTAL:
    {
        feeltrap(trap);
        play_sfx_sound(SFX_GENERIC_MAGICAL_TRAP_ACTIVATE);
        (void)modronportaltele(trap, &youmonst);
        break;
    }
    case LEVER:
        break;
    default:
        feeltrap(trap);
        impossible("You hit a trap of type %u", trap->ttyp);
    }
}

STATIC_OVL char *
trapnote(trap, noprefix)
struct trap *trap;
boolean noprefix;
{
    static char tnbuf[12];
    const char *tn,
        *tnnames[12] = { "C note",  "D flat", "D note",  "E flat",
                         "E note",  "F note", "F sharp", "G note",
                         "G sharp", "A note", "B flat",  "B note" };

    tnbuf[0] = '\0';
    tn = tnnames[trap->tnote];
    if (!noprefix)
        Sprintf(tnbuf, "%s ",
                (*tn == 'A' || *tn == 'E' || *tn == 'F') ? "an" : "a");
    Sprintf(eos(tnbuf), "%s", tn);
    return tnbuf;
}

STATIC_OVL int
steedintrap(trap, otmp)
struct trap *trap;
struct obj *otmp;
{
    struct monst *steed = u.usteed;
    int tt;
    boolean trapkilled, steedhit;

    if (!steed || !trap)
        return 0;
    tt = trap->ttyp;
    steed->mx = u.ux;
    steed->my = u.uy;
    trapkilled = steedhit = FALSE;

    switch (tt) {
    case ARROW_TRAP:
        if (!otmp) {
            impossible("steed hit by non-existant arrow?");
            return 0;
        }
        trapkilled = thitm(8, steed, otmp, 0, FALSE);
        steedhit = TRUE;
        break;
    case DART_TRAP:
        if (!otmp) {
            impossible("steed hit by non-existant dart?");
            return 0;
        }
        trapkilled = thitm(7, steed, otmp, 0, FALSE);
        steedhit = TRUE;
        break;
    case SLP_GAS_TRAP:
        play_special_effect_at(SPECIAL_EFFECT_TRAP_SLEEP_GAS, 0, trap->tx, trap->ty, FALSE);
        special_effect_wait_until_action(0);
        if (!resists_sleep(steed) && !has_innate_breathless(steed->data)
            && mon_can_move(steed)) {
            if (sleep_monst(steed, (struct obj*)0, (struct monst*)0, rn1(7,8), -100, FALSE))
                /* no in_sight check here; you can feel it even if blind */
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly falls asleep!", Monnam(steed));
        }
        steedhit = TRUE;
        special_effect_wait_until_end(0);
        break;
    case LANDMINE:
    {
        boolean spef_on = FALSE;
        if (cansee(trap->tx, trap->ty))
        {
            flush_screen(1);
            play_special_effect_at(SPECIAL_EFFECT_LAND_MINE_EXPLOSION, 0, trap->tx, trap->ty, FALSE);
            play_sfx_sound_at_location(SFX_LAND_MINE_ACTIVATE, trap->tx, trap->ty);
            special_effect_wait_until_action(0);
            play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, trap->tx, trap->ty);
            spef_on = TRUE;
            context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
        }
        else
            play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, trap->tx, trap->ty);

        trapkilled = thitm(0, steed, (struct obj*)0, rnd(16), FALSE);
        steedhit = TRUE;
        if (spef_on)
        {
            special_effect_wait_until_end(0);
            context.global_newsym_flags = 0UL;
        }
        break;
    }
    case PIT:
    case SPIKED_PIT:
        trapkilled = (DEADMONSTER(steed)
                      || thitm(0, steed, (struct obj *) 0,
                               rnd((tt == PIT) ? 6 : 10), FALSE));
        steedhit = TRUE;
        break;
    case POLY_TRAP:
        if (!resists_magic(steed) && !has_unchanging(steed) && !check_magic_resistance_and_inflict_damage(steed, (struct obj*)0, (struct monst*)0, FALSE, 0, 0, NOTELL)) {
            (void) newcham(steed, (struct permonst *) 0, 0, FALSE, FALSE);
            if (!can_saddle(steed) || !can_ride(steed))
                dismount_steed(DISMOUNT_POLY);
            else
                You("adjust yourself in the saddle on %s.",
                    x_monnam(steed, ARTICLE_A, (char *) 0, SUPPRESS_SADDLE,
                             FALSE));
        }
        steedhit = TRUE;
        break;
    default:
        break;
    }

    if (trapkilled) {
        dismount_steed(DISMOUNT_POLY);
        return 2;
    }
    return steedhit ? 1 : 0;
}

/* some actions common to both player and monsters for triggered landmine */
void
blow_up_landmine(trap)
struct trap *trap;
{
    int x = trap->tx, y = trap->ty, dbx, dby;
    struct rm *lev = &levl[x][y];

    (void) scatter(x, y, 4,
                   MAY_DESTROY | MAY_HIT | MAY_FRACTURE | VIS_EFFECTS,
                   (struct obj *) 0);
    del_engr_at(x, y);
    wake_nearto(x, y, 400);
    if (IS_DOOR(lev->typ) && is_door_destroyed_by_booby_trap_at_ptr(lev))
    {
        lev->doormask &= ~D_MASK;
        lev->doormask |= D_BROKEN;
    }
    /* destroy drawbridge if present */
    if (lev->typ == DRAWBRIDGE_DOWN || is_drawbridge_wall(x, y) >= 0) {
        dbx = x, dby = y;
        /* if under the portcullis, the bridge is adjacent */
        if (find_drawbridge(&dbx, &dby))
            destroy_drawbridge(dbx, dby, FALSE);
        trap = t_at(x, y); /* expected to be null after destruction */
    }
    /* convert landmine into pit */
    if (trap) {
        if (Is_waterlevel(&u.uz) || Is_airlevel(&u.uz)) {
            /* no pits here */
            deltrap(trap);
        } else {
            trap->ttyp = PIT;       /* explosion creates a pit */
            trap->tsubtyp = 0;
            trap->tflags = 0;
            trap->activation_count = 0;
            trap->madeby_u = FALSE; /* resulting pit isn't yours */
            trap->madeby_mon = FALSE; /* resulting pit isn't mons */
            seetrap(trap);          /* and it isn't concealed */
        }
    }
}

/*
 * The following are used to track launched objects to
 * prevent them from vanishing if you are killed. They
 * will reappear at the launchplace in bones files.
 */
STATIC_VAR struct {
    struct obj *obj;
    xchar x, y;
} launchplace;

STATIC_OVL void
launch_drop_spot(obj, x, y)
struct obj *obj;
xchar x, y;
{
    if (!obj) {
        launchplace.obj = (struct obj *) 0;
        launchplace.x = 0;
        launchplace.y = 0;
    } else {
        launchplace.obj = obj;
        launchplace.x = x;
        launchplace.y = y;
    }
}

boolean
launch_in_progress()
{
    if (launchplace.obj)
        return TRUE;
    return FALSE;
}

void
force_launch_placement()
{
    if (launchplace.obj) {
        launchplace.obj->otrapped = 0;
        place_object(launchplace.obj, launchplace.x, launchplace.y);
    }
}

/*
 * Move obj from (x1,y1) to (x2,y2)
 *
 * Return 0 if no object was launched.
 *        1 if an object was launched and placed somewhere.
 *        2 if an object was launched, but used up.
 */
int
launch_obj(otyp, x1, y1, x2, y2, style)
short otyp;
register int x1, y1, x2, y2;
int style;
{
    register struct monst *mtmp;
    register struct obj *otmp, *otmp2;
    register int dx, dy;
    struct obj *singleobj;
    boolean used_up = FALSE;
    boolean otherside = FALSE;
    int dist;
    int tmp;
    int delaycnt = 0;

    otmp = sobj_at(otyp, x1, y1);
    /* Try the other side too, for rolling boulder traps */
    if (!otmp && otyp == BOULDER) {
        otherside = TRUE;
        otmp = sobj_at(otyp, x2, y2);
    }
    if (!otmp)
        return 0;
    if (otherside) { /* swap 'em */
        int tx, ty;

        tx = x1;
        ty = y1;
        x1 = x2;
        y1 = y2;
        x2 = tx;
        y2 = ty;
    }

    if (otmp->quan == 1L) {
        obj_extract_self(otmp);
        singleobj = otmp;
        otmp = (struct obj *) 0;
    } else {
        singleobj = splitobj(otmp, 1L);
        if(singleobj)
            obj_extract_self(singleobj);
    }
    newsym(x1, y1);

    if (!singleobj)
        return 0;

    /* in case you're using a pick-axe to chop the boulder that's being
       launched (perhaps a monster triggered it), destroy context so that
       next dig attempt never thinks you're resuming previous effort */
    if ((otyp == BOULDER || otyp == STATUE)
        && singleobj->ox == context.digging.pos.x
        && singleobj->oy == context.digging.pos.y)
        (void) memset((genericptr_t) &context.digging, 0,
                      sizeof(struct dig_info));

    dist = distmin(x1, y1, x2, y2);
    bhitpos.x = x1;
    bhitpos.y = y1;
    dx = sgn(x2 - x1);
    dy = sgn(y2 - y1);

    if (singleobj)
    {
        play_immediate_ray_sound_at_location(object_soundsets[objects[singleobj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_CREATE, bhitpos.x, bhitpos.y);
        start_ambient_ray_sound_at_location(object_soundsets[objects[singleobj->otyp].oc_soundset].ray_soundset, bhitpos.x, bhitpos.y);
    }
    switch (style) {
    case ROLL | LAUNCH_UNSEEN:
        if (otyp == BOULDER) {
            You_hear(Hallucination ? "someone bowling."
                                   : "rumbling in the distance.");
        }
        style &= ~LAUNCH_UNSEEN;
        goto roll;
    case ROLL | LAUNCH_KNOWN:
        /* use otrapped as a flag to ohitmon */
        singleobj->otrapped = 1;
        style &= ~LAUNCH_KNOWN;
    /* fall through */
    roll:
    case ROLL:
        delaycnt = 2;
    /* fall through */
    default:
        if (!delaycnt)
            delaycnt = 1;
        if (!cansee(bhitpos.x, bhitpos.y))
            curs_on_u();
        tmp_at(DISP_FLASH, obj_to_missile_glyph(singleobj, get_missile_index(dx, dy), rn2_on_display_rng));
        tmp_at(bhitpos.x, bhitpos.y);
    }
    /* Mark a spot to place object in bones files to prevent
     * loss of object. Use the starting spot to ensure that
     * a rolling boulder will still launch, which it wouldn't
     * do if left midstream. Unfortunately we can't use the
     * target resting spot, because there are some things/situations
     * that would prevent it from ever getting there (bars), and we
     * can't tell that yet.
     */
    launch_drop_spot(singleobj, bhitpos.x, bhitpos.y);

    /* Set the object in motion */
    while (dist-- > 0 && !used_up) {
        struct trap *t;
        tmp_at(bhitpos.x, bhitpos.y);
        tmp = delaycnt;

        /* dstage@u.washington.edu -- Delay only if hero sees it */
        if (cansee(bhitpos.x, bhitpos.y))
            while (tmp-- > 0)
                adjusted_delay_output();

        bhitpos.x += dx;
        bhitpos.y += dy;

        if (singleobj)
            update_ambient_ray_sound_to_location(object_soundsets[objects[singleobj->otyp].oc_soundset].ray_soundset, bhitpos.x, bhitpos.y);

        t = t_at(bhitpos.x, bhitpos.y);

        if ((mtmp = m_at(bhitpos.x, bhitpos.y)) != 0) {
            if (otyp == BOULDER && throws_rocks(mtmp->data)) {
                if (rn2(3)) {
                    if (cansee(bhitpos.x, bhitpos.y))
                        pline("%s snatches the boulder.", Monnam(mtmp));
                    singleobj->otrapped = 0;
                    (void) mpickobj(mtmp, singleobj);
                    used_up = TRUE;
                    launch_drop_spot((struct obj *) 0, 0, 0);
                    break;
                }
            }
            if (singleobj)
                play_immediate_ray_sound_at_location(object_soundsets[objects[singleobj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_HIT_MONSTER, bhitpos.x, bhitpos.y);
            if (ohitmon(mtmp, singleobj, (style == ROLL) ? -1 : dist,
                        FALSE)) {
                used_up = TRUE;
                launch_drop_spot((struct obj *) 0, 0, 0);
                break;
            }
        } else if (bhitpos.x == u.ux && bhitpos.y == u.uy) {
            if (multi)
                nomul(0);
            if(singleobj)
                play_immediate_ray_sound_at_location(object_soundsets[objects[singleobj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_HIT_MONSTER, bhitpos.x, bhitpos.y);
            if (thitu(10 + singleobj->enchantment, weapon_total_dmg_value(singleobj, &youmonst, (struct monst*)0, 1),
                      &singleobj, (char *) 0, (struct monst*)0, (char*)0))
                stop_occupation();
        }
        if (style == ROLL) {
            if (down_gate(bhitpos.x, bhitpos.y) != -1) {
                if (ship_object(singleobj, bhitpos.x, bhitpos.y, FALSE)) {
                    used_up = TRUE;
                    launch_drop_spot((struct obj *) 0, 0, 0);
                    break;
                }
            }
            if (t && otyp == BOULDER)
            {
                switch (t->ttyp) 
                {
                case LANDMINE:
                {
                    if (rn2(10) > 2)
                    {
                        boolean spef_on = FALSE;
                        if (cansee(t->tx, t->ty))
                        {
                            newsym(bhitpos.x, bhitpos.y);
                            flush_screen(1);
                            play_special_effect_at(SPECIAL_EFFECT_LAND_MINE_EXPLOSION, 0, t->tx, t->ty, FALSE);
                            play_sfx_sound_at_location(SFX_LAND_MINE_ACTIVATE, t->tx, t->ty);
                            special_effect_wait_until_action(0);
                            play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, t->tx, t->ty);
                            spef_on = TRUE;
                            context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
                        }
                        else
                            play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, t->tx, t->ty);

                        pline_ex(ATR_NONE, CLR_MSG_WARNING,
                            "KAABLAMM!!!%s",
                            cansee(bhitpos.x, bhitpos.y)
                            ? " The rolling boulder triggers a land mine."
                            : "");
                        deltrap(t);
                        del_engr_at(bhitpos.x, bhitpos.y);
                        place_object(singleobj, bhitpos.x, bhitpos.y);
                        singleobj->otrapped = 0;
                        fracture_rock(singleobj, TRUE);
                        (void)scatter(bhitpos.x, bhitpos.y, 4,
                            MAY_DESTROY | MAY_HIT | MAY_FRACTURE
                            | VIS_EFFECTS,
                            (struct obj*)0);
                        if (cansee(bhitpos.x, bhitpos.y))
                            newsym(bhitpos.x, bhitpos.y);
                        used_up = TRUE;
                        launch_drop_spot((struct obj*)0, 0, 0);
                        if (spef_on)
                        {
                            special_effect_wait_until_end(0);
                            context.global_newsym_flags = 0UL;
                        }
                    }
                    break;
                }
                case LEVEL_TELEP:
                case TELEP_TRAP:
                    if (cansee(bhitpos.x, bhitpos.y))
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Suddenly the rolling boulder disappears!");
                    else
                        You_hear("a rumbling stop abruptly.");
                    singleobj->otrapped = 0;
                    if (t->ttyp == TELEP_TRAP)
                        (void) rloco(singleobj);
                    else {
                        int newlev = random_teleport_level();
                        d_level dest;

                        if (newlev == depth(&u.uz) || In_endgame(&u.uz))
                            continue;
                        add_to_migration(singleobj);
                        get_level(&dest, newlev);
                        singleobj->ox = dest.dnum;
                        singleobj->oy = dest.dlevel;
                        singleobj->owornmask = (long) MIGR_RANDOM;
                    }
                    seetrap(t);
                    used_up = TRUE;
                    launch_drop_spot((struct obj *) 0, 0, 0);
                    break;
                case PIT:
                case SPIKED_PIT:
                case HOLE:
                case TRAPDOOR:
                    /* the boulder won't be used up if there is a
                       monster in the trap; stop rolling anyway */
                    x2 = bhitpos.x, y2 = bhitpos.y; /* stops here */
                    if (flooreffects(singleobj, x2, y2, "fall")) {
                        used_up = TRUE;
                        launch_drop_spot((struct obj *) 0, 0, 0);
                    }
                    dist = -1; /* stop rolling immediately */
                    break;
                }
                if (used_up || dist == -1)
                    break;
            }
            if (flooreffects(singleobj, bhitpos.x, bhitpos.y, "fall")) {
                used_up = TRUE;
                launch_drop_spot((struct obj *) 0, 0, 0);
                break;
            }
            if (otyp == BOULDER
                && (otmp2 = sobj_at(BOULDER, bhitpos.x, bhitpos.y)) != 0) {
                const char *bmsg = " as one boulder sets another in motion";

                if (!isok(bhitpos.x + dx, bhitpos.y + dy) || !dist
                    || IS_ROCK(levl[bhitpos.x + dx][bhitpos.y + dy].typ))
                    bmsg = " as one boulder hits another";

                You_hear("a loud crash%s!",
                         cansee(bhitpos.x, bhitpos.y) ? bmsg : "");
                obj_extract_self(otmp2);
                /* pass off the otrapped flag to the next boulder */
                otmp2->otrapped = singleobj->otrapped;
                singleobj->otrapped = 0;
                place_object(singleobj, bhitpos.x, bhitpos.y);
                singleobj = otmp2;
                otmp2 = (struct obj *) 0;
                wake_nearto(bhitpos.x, bhitpos.y, 10 * 10);
            }
        }
        if (otyp == BOULDER && closed_door(bhitpos.x, bhitpos.y) && is_door_destroyed_by_striking_at(bhitpos.x, bhitpos.y)) {
            if (cansee(bhitpos.x, bhitpos.y))
                pline_The("boulder crashes through a door.");
            levl[bhitpos.x][bhitpos.y].doormask &= ~D_MASK;
            levl[bhitpos.x][bhitpos.y].doormask |= D_BROKEN;
            if (dist)
                unblock_vision_and_hearing_at_point(bhitpos.x, bhitpos.y);
        }

        /* if about to hit iron bars, do so now */
        if (dist > 0 && isok(bhitpos.x + dx, bhitpos.y + dy)
            && levl[bhitpos.x + dx][bhitpos.y + dy].typ == IRONBARS) {
            x2 = bhitpos.x, y2 = bhitpos.y; /* object stops here */
            if (hits_bars(&singleobj,
                          x2, y2, x2+dx, y2+dy,
                          !rn2(20), 0)) {
                if (!singleobj) {
                    used_up = TRUE;
                    launch_drop_spot((struct obj *) 0, 0, 0);
                }
                break;
            }
        }
    }
    tmp_at(DISP_END, 0);
    if (singleobj)
    {
        play_immediate_ray_sound_at_location(object_soundsets[objects[singleobj->otyp].oc_soundset].ray_soundset, RAY_SOUND_TYPE_DESTROY, bhitpos.x, bhitpos.y);
        stop_ambient_ray_sound(object_soundsets[objects[singleobj->otyp].oc_soundset].ray_soundset);
    }
    launch_drop_spot((struct obj *) 0, 0, 0);
    if (!used_up && singleobj) {
        singleobj->otrapped = 0;
        place_object(singleobj, x2, y2);
        newsym(x2, y2);
        return 1;
    } else
        return 2;
}

void
seetrap(trap)
struct trap *trap;
{
    if (!trap->tseen) {
        trap->tseen = 1;
        newsym(trap->tx, trap->ty);
    }
}

/* like seetrap() but overrides vision */
void
feeltrap(trap)
struct trap *trap;
{
    trap->tseen = 1;
    map_trap(trap, 1);
    /* in case it's beneath something, redisplay the something */
    newsym(trap->tx, trap->ty);
}

STATIC_OVL int
mkroll_launch(ttmp, x, y, otyp, ocount)
struct trap *ttmp;
xchar x, y;
short otyp;
long ocount;
{
    struct obj *otmp;
    register int tmp;
    schar dx, dy;
    int distance;
    coord cc;
    coord bcc;
    int trycount = 0;
    boolean success = FALSE;
    int mindist = 4;

    if (ttmp->ttyp == ROLLING_BOULDER_TRAP)
        mindist = 2;
    distance = rn1(5, 4); /* 4..8 away */
    tmp = rn2(8);         /* randomly pick a direction to try first */
    while (distance >= mindist) {
        dx = xdir[tmp];
        dy = ydir[tmp];
        cc.x = x;
        cc.y = y;
        /* Prevent boulder from being placed on water */
        if (ttmp->ttyp == ROLLING_BOULDER_TRAP
            && is_pool_or_lava(x + distance * dx, y + distance * dy))
            success = FALSE;
        else
            success = isclearpath(&cc, distance, dx, dy);
        if (ttmp->ttyp == ROLLING_BOULDER_TRAP) {
            boolean success_otherway;

            bcc.x = x;
            bcc.y = y;
            success_otherway = isclearpath(&bcc, distance, -(dx), -(dy));
            if (!success_otherway)
                success = FALSE;
        }
        if (success)
            break;
        if (++tmp > 7)
            tmp = 0;
        if ((++trycount % 8) == 0)
            --distance;
    }
    if (!success) {
        /* create the trap without any ammo, launch pt at trap location */
        cc.x = bcc.x = x;
        cc.y = bcc.y = y;
    } else {
        otmp = mksobj(otyp, TRUE, FALSE, FALSE);
        otmp->quan = ocount;
        otmp->owt = weight(otmp);
        place_object(otmp, cc.x, cc.y);
        if ((objects[otyp].oc_flags5 & O5_TILE_IS_TILESET_DEPENDENT) != 0)
        {
            otmp->has_special_tileset = 1;
            otmp->special_tileset = levl[cc.x][cc.y].use_special_tileset ? levl[cc.x][cc.y].special_tileset : get_current_cmap_type_index();
        }
        stackobj(otmp);
    }
    ttmp->launch.x = cc.x;
    ttmp->launch.y = cc.y;
    if (ttmp->ttyp == ROLLING_BOULDER_TRAP) {
        ttmp->launch2.x = bcc.x;
        ttmp->launch2.y = bcc.y;
    } else
        ttmp->launch_otyp = otyp;
    newsym(ttmp->launch.x, ttmp->launch.y);
    return 1;
}

STATIC_OVL boolean
isclearpath(cc, distance, dx, dy)
coord *cc;
int distance;
schar dx, dy;
{
    uchar typ;
    xchar x, y;

    x = cc->x;
    y = cc->y;
    while (distance-- > 0) {
        x += dx;
        y += dy;
        typ = levl[x][y].typ;
        if (!isok(x, y) || !ZAP_POS(typ) || closed_door(x, y))
            return FALSE;
    }
    cc->x = x;
    cc->y = y;
    return TRUE;
}

int
mintrap(mtmp)
register struct monst *mtmp;
{
    if (!mtmp)
        return 0;

    register struct trap *trap = t_at(mtmp->mx, mtmp->my);
    boolean trapkilled = FALSE;
    struct permonst *mptr = mtmp->data;
    struct obj *otmp;

    if (!trap)
    {
        mtmp->mtrapped = 0;      /* perhaps teleported? */
    } 
    else if (mtmp->mtrapped)
    { /* is currently in the trap */
        if (!trap->tseen && cansee(mtmp->mx, mtmp->my) && canseemon(mtmp)
            && (is_pit(trap->ttyp) || trap->ttyp == BEAR_TRAP
                || trap->ttyp == HOLE
                || trap->ttyp == WEB)) 
        {
            /* If you come upon an obviously trapped monster, then
             * you must be able to see the trap it's in too.
             */
            seetrap(trap);
        }

        if (!rn2(40) || (is_pit(trap->ttyp) && has_pitwalk(mtmp->data)))
        {
            if (sobj_at(BOULDER, mtmp->mx, mtmp->my)  && is_pit(trap->ttyp))
            {
                if (!rn2(2) || has_pitwalk(mtmp->data))
                {
                    mtmp->mtrapped = 0;
                    if (canseemon(mtmp))
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s rises from the pit...", Monnam(mtmp));
                    fill_pit(mtmp->mx, mtmp->my);
                }
            } 
            else 
            {
                mtmp->mtrapped = 0;
            }
        } 
        else if (metallivorous(mptr)) 
        {
            if (trap->ttyp == BEAR_TRAP) 
            {
                if (canseemon(mtmp))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s eats a bear trap!", Monnam(mtmp));
                deltrap(trap);
                mtmp->meating = 5;
                mtmp->mtrapped = 0;
            } 
            else if (trap->ttyp == SPIKED_PIT)
            {
                if (canseemon(mtmp))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s munches on some spikes!", Monnam(mtmp));
                trap->ttyp = PIT;
                trap->tsubtyp = 0;
                trap->tflags = 0;
                trap->activation_count = 0;
                mtmp->meating = 5;
            }
        }
    } 
    else
    {
        register int tt = trap->ttyp;
        boolean in_sight, tear_web, see_it, can_see_trap,
            inescapable = force_mintrap || ((tt == HOLE || tt == PIT)
                                            && Sokoban && !trap->madeby_u && !trap->madeby_mon);
        const char *fallverb;

        can_see_trap = cansee(trap->tx, trap->ty);

        /* true when called from dotrap, inescapable is not an option */
        if (mtmp == u.usteed)
            inescapable = TRUE;
        
        if (!inescapable && (knows_pits_and_holes(mtmp->data) || knows_traps(mtmp->data) ||
            ((mtmp->mtrapseen & (1 << (tt - 1))) != 0  || (tt == HOLE && !mindless(mptr)))))
        {
            /* it has been in such a trap - perhaps it escapes */
            if ((is_pit(tt) && has_pitwalk(mtmp->data)) || rn2(4))
                return 0;
        }
        else 
        {
            mtmp->mtrapseen |= (1 << (tt - 1));
        }

        /* pitwalking creatures do not care about pits */
        if (is_pit(tt) && has_pitwalk(mtmp->data))
            return 0;

        /* Monster is aggravated by being trapped by you.
           Recognizing who made the trap isn't completely
           unreasonable; everybody has their own style. */
        if (trap->madeby_u && rnl(5))
            setmangry(mtmp, TRUE);

        in_sight = canseemon(mtmp);
        see_it = cansee(mtmp->mx, mtmp->my);
        /* assume hero can tell what's going on for the steed */
        if (mtmp == u.usteed)
            in_sight = TRUE;
        switch (tt) {
        case ARROW_TRAP:
            if (trap->once && trap->tseen && !rn2(15)) {
                play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
                if (in_sight && see_it)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s triggers a trap but nothing happens.",
                          Monnam(mtmp));
                deltrap(trap);
                newsym(mtmp->mx, mtmp->my);
                break;
            }
            play_sfx_sound_at_location(SFX_ARROW_TRAP_FIRE, mtmp->mx, mtmp->my);
            trap->once = 1;
            otmp = t_missile(get_shooting_trap_object(trap), trap);
            if (in_sight)
                seetrap(trap);
            if (thitm(8, mtmp, otmp, 0, FALSE))
                trapkilled = TRUE;
            break;
        case DART_TRAP:
            if (trap->once && trap->tseen && !rn2(15)) {
                play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
                if (in_sight && see_it)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s triggers a trap but nothing happens.",
                          Monnam(mtmp));
                deltrap(trap);
                newsym(mtmp->mx, mtmp->my);
                break;
            }
            play_sfx_sound_at_location(SFX_DART_TRAP_FIRE, mtmp->mx, mtmp->my);
            trap->once = 1;
            otmp = t_missile(get_shooting_trap_object(trap), trap);
            if (!rn2(6))
                otmp->opoisoned = 1;
            if (in_sight)
                seetrap(trap);
            if (thitm(7, mtmp, otmp, 0, FALSE))
                trapkilled = TRUE;
            break;
        case ROCKTRAP:
            if (can_see_trap)
            {
                newsym(trap->tx, trap->ty);
                play_special_effect_at(SPECIAL_EFFECT_FALLING_ROCK_TRAP_TRAP_DOOR, 0, trap->tx, trap->ty, FALSE);
                context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
            }

            play_sfx_sound_at_location(SFX_FALLING_ROCK_TRAP_TRIGGER, mtmp->mx, mtmp->my);
            
            if (can_see_trap)
                special_effect_wait_until_action(0);
            
            if (trap->once && trap->tseen && !rn2(15)) 
            {
                if (in_sight)
                {
                    if(see_it)
                        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A trap door above %s opens, but nothing falls out!", mon_nam(mtmp));
                    else
                        You_see_ex(ATR_NONE, CLR_MSG_ATTENTION, "a trap door open at a distance, but nothing falls out!");
                }
                deltrap(trap);
                newsym(mtmp->mx, mtmp->my);
                if (can_see_trap && see_it)
                    special_effect_wait_until_end(0);
                context.global_newsym_flags = 0UL;
                break;
            }
            trap->once = 1;

            otmp = t_missile(get_shooting_trap_object(trap), trap);
            if (in_sight)
                seetrap(trap);

            if (can_see_trap && otmp)
                play_special_effect_at(SPECIAL_EFFECT_FALLING_ROCK_TRAP_FALLING_ROCK, 1, trap->tx, trap->ty, FALSE);

            if (can_see_trap && otmp)
                special_effect_wait_until_action(1);

            if (thitm(0, mtmp, otmp, d(2, 6), FALSE))
                trapkilled = TRUE;

            if (iflags.using_gui_sounds && can_see_trap && in_sight && see_it && !trapkilled)
            {
                delay_output_milliseconds(100);
                play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_UNHAPPY);
            }

            if (can_see_trap && otmp)
            {
                special_effect_wait_until_end(1);
            }
            if (can_see_trap)
            {
                context.global_newsym_flags = 0UL;
                special_effect_wait_until_end(0);
            }
            break;
        case SQKY_BOARD:
            if (is_flying(mtmp) || is_levitating(mtmp))
                break;
            /* stepped on a squeaky board */
            play_sfx_sound_at_location_with_minimum_volume(SFX_SQUEAKY_BOARD, mtmp->mx, mtmp->my, 0.15);
            if (in_sight) {
                if (!Deaf) {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "A board beneath %s squeaks %s loudly.",
                          mon_nam(mtmp), trapnote(trap, 0));
                    seetrap(trap);
                } else {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s stops momentarily and appears to cringe.",
                          Monnam(mtmp));
                }
            } else {
                /* same near/far threshold as mzapmsg() */
                int range = couldsee(mtmp->mx, mtmp->my) /* 9 or 5 */
                               ? NEARBY_CUTOFF_RANGE_CAN_SEE : NEARBY_CUTOFF_RANGE_CANNOT_SEE;

                You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a %s squeak %s.", trapnote(trap, 1),
                         (distu(mtmp->mx, mtmp->my) <= range * range)
                            ? "nearby" : "in the distance");
            }
            /* wake up nearby monsters */
            wake_nearto(mtmp->mx, mtmp->my, 40);
            break;
        case BEAR_TRAP:
            if (mptr->msize > MZ_SMALL && !amorphous(mptr) && !(is_flying(mtmp) || is_levitating(mtmp))
                && !is_whirly(mptr) && !unsolid(mptr))
            {
                mtmp->mtrapped = 1;
                if (in_sight && can_see_trap)
                {
                    newsym(trap->tx, trap->ty);
                    context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
                    struct layer_info layers = layers_at(trap->tx, trap->ty);
                    int old_glyph = layers.layer_glyphs[LAYER_TRAP];
                    int old_gui_glyph = layers.layer_gui_glyphs[LAYER_TRAP];
                    int old_cover_glyph = layers.layer_glyphs[LAYER_COVER_TRAP];
                    int old_cover_gui_glyph = layers.layer_gui_glyphs[LAYER_COVER_TRAP];
                    show_glyph_on_layer(trap->tx, trap->ty, NO_GLYPH, LAYER_TRAP);
                    show_glyph_on_layer(trap->tx, trap->ty, NO_GLYPH, LAYER_COVER_TRAP);
                    flush_screen(1);
                    play_special_effect_at(SPECIAL_EFFECT_BEAR_TRAP_CLOSE, 0, trap->tx, trap->ty, FALSE);
                    play_sfx_sound_at_location(SFX_BEAR_TRAP_CLOSES, trap->tx, trap->ty);
                    special_effect_wait_until_action(0);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is caught in %s bear trap!", Monnam(mtmp),
                          a_your[trap->madeby_u]);
                    seetrap(trap);
                    special_effect_wait_until_end(0);
                    show_gui_glyph_on_layer(trap->tx, trap->ty, old_glyph, old_gui_glyph, LAYER_TRAP);
                    show_gui_glyph_on_layer(trap->tx, trap->ty, old_cover_glyph, old_cover_gui_glyph, LAYER_COVER_TRAP);
                    flush_screen(1);
                    context.global_newsym_flags = 0UL;
                }
                else 
                {
                    if (is_bear(mptr))
                    {
                        play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_HOWL_IN_ANGER);
                        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "the roaring of an angry bear!");
                    }
                }
            }
            else if (force_mintrap) 
            {
                if (in_sight)
                {
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s evades %s bear trap!", Monnam(mtmp),
                          a_your[trap->madeby_u]);
                    seetrap(trap);
                }
            }
            if (mtmp->mtrapped)
            {
                trapkilled = thitm(0, mtmp, (struct obj*)0, d(2, 4), FALSE);
                if (in_sight && trapkilled)
                {
                    context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
                    struct layer_info layers = layers_at(trap->tx, trap->ty);
                    int old_glyph = layers.layer_glyphs[LAYER_TRAP];
                    int old_gui_glyph = layers.layer_gui_glyphs[LAYER_TRAP];
                    int old_cover_glyph = layers.layer_glyphs[LAYER_COVER_TRAP];
                    int old_cover_gui_glyph = layers.layer_gui_glyphs[LAYER_COVER_TRAP];
                    show_glyph_on_layer(trap->tx, trap->ty, NO_GLYPH, LAYER_TRAP);
                    show_glyph_on_layer(trap->tx, trap->ty, NO_GLYPH, LAYER_COVER_TRAP);
                    flush_screen(1);
                    play_special_effect_at(SPECIAL_EFFECT_BEAR_TRAP_OPEN, 0, trap->tx, trap->ty, FALSE);
                    special_effect_wait_until_action(0);
                    special_effect_wait_until_end(0);
                    show_gui_glyph_on_layer(trap->tx, trap->ty, old_glyph, old_gui_glyph, LAYER_TRAP);
                    show_gui_glyph_on_layer(trap->tx, trap->ty, old_cover_glyph, old_cover_gui_glyph, LAYER_COVER_TRAP);
                    flush_screen(1);
                    context.global_newsym_flags = 0UL;
                }
            }
            break;
        case SLP_GAS_TRAP:
            if (can_see_trap)
            {
                seetrap(trap);
                newsym(trap->tx, trap->ty);
                flush_screen(1);
                play_special_effect_at(SPECIAL_EFFECT_TRAP_SLEEP_GAS, 0, trap->tx, trap->ty, FALSE);
                play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
                play_sfx_sound_at_location(SFX_ENVELOPED_IN_CLOUD_OF_GAS, mtmp->mx, mtmp->my);
                special_effect_wait_until_action(0);

            }
            else
            {
                play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
                play_sfx_sound_at_location(SFX_ENVELOPED_IN_CLOUD_OF_GAS, mtmp->mx, mtmp->my);
            }

            if (!resists_sleep(mtmp) && !has_innate_breathless(mptr) && mon_can_move(mtmp) && sleep_monst(mtmp, (struct obj*)0, (struct monst*)0, rn1(7, 8), -100, FALSE))
            {
                if(in_sight || see_it)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s suddenly falls asleep!", Monnam(mtmp));
            }
            else
            {
                if (in_sight || see_it)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is enveloped in a cloud of gas!", Monnam(mtmp));
            }

            if (can_see_trap)
            {
                special_effect_wait_until_end(0);
            }
            break;
        case RUST_TRAP:
        {
            struct obj *target;
            if (can_see_trap)
            {
                seetrap(trap);
                newsym(mtmp->mx, mtmp->my);
                play_special_effect_at(SPECIAL_EFFECT_TRAP_RUST, 0, mtmp->mx, mtmp->my, FALSE);
            }
            play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
            play_sfx_sound_at_location(SFX_GUSH_OF_WATER_HITS, mtmp->mx, mtmp->my);
            if (can_see_trap)
                special_effect_wait_until_action(0);

            switch (rn2(5)) 
            {
            case 0:
                if (in_sight)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s on the %s!", A_gush_of_water_hits,
                          mon_nam(mtmp), mbodypart(mtmp, HEAD));
                target = which_armor(mtmp, W_ARMH);
                (void) water_damage(target, helm_simple_name(target), TRUE);
                break;
            case 1:
                if (in_sight)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s's left %s!", A_gush_of_water_hits,
                          mon_nam(mtmp), mbodypart(mtmp, ARM));
                target = which_armor(mtmp, W_ARMS);
                if (water_damage(target, (target && (target->oclass == ARMOR_CLASS && objects[target->otyp].oc_armor_category == ARM_SHIELD) ? "shield" : "secondary weapon"), TRUE) != ER_NOTHING)
                    break;
                target = MON_WEP(mtmp);
                if (target && bimanual(target))
                    (void) water_damage(target, 0, TRUE);
            glovecheck:
                target = which_armor(mtmp, W_ARMG);
                (void) water_damage(target, "gauntlets", TRUE);
                break;
            case 2:
                if (in_sight)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s's right %s!", A_gush_of_water_hits,
                          mon_nam(mtmp), mbodypart(mtmp, ARM));
                (void) water_damage(MON_WEP(mtmp), 0, TRUE);
                goto glovecheck;
            default:
                if (in_sight)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s!", A_gush_of_water_hits, mon_nam(mtmp));
                for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
                    if (otmp->lamplit
                        && (otmp->owornmask & (W_WEP | W_SWAPWEP)) == 0)
                        (void) snuff_lit(otmp);
                if ((target = which_armor(mtmp, W_ARMC)) != 0)
                    (void) water_damage(target, cloak_simple_name(target),
                                        TRUE);
                else if ((target = which_armor(mtmp, W_ARM)) != 0)
                    (void) water_damage(target, suit_simple_name(target),
                                        TRUE);
                else if ((target = which_armor(mtmp, W_ARMU)) != 0)
                    (void) water_damage(target, "shirt", TRUE);
            }

            if (is_iron(mptr)) 
            {
                if (in_sight)
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s falls to pieces!", Monnam(mtmp));
                else if (is_tame(mtmp))
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "May %s rust in peace.", mon_nam(mtmp));
                mondied(mtmp);
                if (DEADMONSTER(mtmp))
                    trapkilled = TRUE;
            } 
            else if (mptr == &mons[PM_GREMLIN] && rn2(3)) 
            {
                (void) split_mon(mtmp, (struct monst *) 0);
            }

            if (can_see_trap)
                special_effect_wait_until_end(0);
            break;
        } /* RUST_TRAP */
        case FIRE_TRAP:
        mfiretrap:
            if (can_see_trap)
            {
                newsym(mtmp->mx, mtmp->my);
                play_special_effect_at(SPECIAL_EFFECT_TRAP_FIRE, 0, mtmp->mx, mtmp->my, FALSE);
                play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
                play_sfx_sound_at_location(SFX_TOWER_OF_FLAME_ERUPTS, mtmp->mx, mtmp->my);
                special_effect_wait_until_action(0);
            }
            else
            {
                play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
                play_sfx_sound_at_location(SFX_TOWER_OF_FLAME_ERUPTS, mtmp->mx, mtmp->my);
            }

            if (in_sight && can_see_trap)
                pline("A %s erupts from the %s under %s!", tower_of_flame,
                      surface(mtmp->mx, mtmp->my), mon_nam(mtmp));
            else if (see_it && can_see_trap) /* evidently `mtmp' is invisible */
                You_see("a %s erupt from the %s!", tower_of_flame,
                        surface(mtmp->mx, mtmp->my));

            if ((in_sight || see_it) && can_see_trap)
                seetrap(trap);

            if (is_mon_immune_to_fire(mtmp)) 
            {
                if (in_sight && can_see_trap)
                {
                    m_shieldeff(mtmp);
                    pline("%s is uninjured.", Monnam(mtmp));
                }
            }
            else 
            {
                int dicenum = In_endgame(&u.uz) ? 25 : max(2, min(20, (level_difficulty() * 2) / 3));
                int num = d(dicenum, 6), alt;
                boolean immolate = FALSE;

                /* paper burns very fast, assume straw is tightly
                 * packed and burns a bit slower */
                switch (mtmp->mnum)
                {
                case PM_PAPER_GOLEM:
                    immolate = TRUE;
                    alt = mtmp->mhpmax;
                    break;
                case PM_STRAW_GOLEM:
                    alt = mtmp->mhpmax / 2;
                    break;
                case PM_WOOD_GOLEM:
                    alt = mtmp->mhpmax / 4;
                    break;
                case PM_LEATHER_GOLEM:
                    alt = mtmp->mhpmax / 8;
                    break;
                default:
                    alt = 0;
                    break;
                }
                if (alt > num)
                    num = alt;

                if (thitm(0, mtmp, (struct obj*)0, num, immolate))
                {
                    trapkilled = TRUE;
                }
#if 0
                else
                {
                    /* we know mhp is at least `num' below mhpmax,
                       so no (mhp > mhpmax) check is needed here */
                    mtmp->mbasehpmax -= rn2(num + 1);
                    update_mon_maxhp(mtmp);
                }
#endif
            }

            if (burnarmor(mtmp) || rn2(3))
            {
                (void) destroy_mitem(mtmp, SCROLL_CLASS, AD_FIRE);
                (void) destroy_mitem(mtmp, SPBOOK_CLASS, AD_FIRE);
                (void) destroy_mitem(mtmp, POTION_CLASS, AD_FIRE);
            }

            if (burn_floor_objects(mtmp->mx, mtmp->my, see_it, FALSE)
                && !see_it && distu(mtmp->mx, mtmp->my) <= 3 * 3)
                You("smell smoke.");

            if (is_ice(mtmp->mx, mtmp->my))
                melt_ice(mtmp->mx, mtmp->my, (char *) 0);

            if (see_it && can_see_trap && t_at(mtmp->mx, mtmp->my))
                seetrap(trap);

            if((in_sight || see_it) && can_see_trap)
                special_effect_wait_until_end(0);

            break;
        case PIT:
        case SPIKED_PIT:
            play_sfx_sound_at_location(SFX_FALL_INTO_PIT, mtmp->mx, mtmp->my);
            fallverb = "falls";
            if (is_flying(mtmp) || is_levitating(mtmp)
                || (mtmp->wormno && count_wsegs(mtmp) > 5)
                || is_clinger(mptr))
            {
                if (force_mintrap && !Sokoban) 
                {
                    /* openfallingtrap; not inescapable here */
                    if (in_sight)
                    {
                        seetrap(trap);
                        pline("%s doesn't fall into the pit.", Monnam(mtmp));
                    }
                    break; /* inescapable = FALSE; */
                }

                if (!inescapable)
                    break;               /* avoids trap */

                fallverb = "is dragged"; /* sokoban pit */
            }

            if (!passes_walls(mptr) && !has_pitwalk(mptr))
                mtmp->mtrapped = 1;

            if (in_sight) 
            {
                pline("%s %s into %s pit!", Monnam(mtmp), fallverb,
                      a_your[trap->madeby_u]);
                if (mptr == &mons[PM_PIT_VIPER]
                    || mptr == &mons[PM_PIT_FIEND])
                    pline("How pitiful.  Isn't that the pits?");
                seetrap(trap);
            }

            mselftouch(mtmp, "Falling, ", FALSE);
            if (DEADMONSTER(mtmp) || thitm(0, mtmp, (struct obj *) 0,
                                        rnd((tt == PIT) ? 6 : 10), FALSE))
                trapkilled = TRUE;
            break;
        case HOLE:
        case TRAPDOOR: 
        {
            if (!Can_fall_thru(&u.uz)) 
            {
                impossible("mintrap: %ss cannot exist on this level.",
                    defsyms[trap_to_defsym(tt)].explanation);
                break; /* don't activate it after all */
            }

            if (is_flying(mtmp) || is_levitating(mtmp) || mptr == &mons[PM_WUMPUS]
                || (mtmp->wormno && count_wsegs(mtmp) > 5)
                || mptr->msize >= MZ_HUGE) 
            {
                if (force_mintrap && !Sokoban)
                {
                    /* openfallingtrap; not inescapable here */
                    if (in_sight)
                    {
                        seetrap(trap);
                        if (tt == TRAPDOOR)
                            pline(
                                "A trap door opens, but %s doesn't fall through.",
                                mon_nam(mtmp));
                        else /* (tt == HOLE) */
                            pline("%s doesn't fall through the hole.",
                                Monnam(mtmp));
                    }
                    break; /* inescapable = FALSE; */
                }

                if (inescapable) 
                { /* sokoban hole */
                    if (in_sight) 
                    {
                        pline("%s seems to be yanked down!", Monnam(mtmp));
                        /* suppress message in mlevel_tele_trap() */
                        in_sight = FALSE;
                        seetrap(trap);
                    }
                }
                else
                    break;
            }

            int mlev_res;

            play_sfx_sound_at_location(tt == TRAPDOOR ? SFX_TRAP_DOOR_OPENS : SFX_HOLE_OPENS, mtmp->mx, mtmp->my);
            mlev_res = mlevel_tele_trap(mtmp, trap, inescapable, in_sight);
            if (mlev_res)
                return mlev_res;
            break;
        }

        case LEVEL_TELEP:
        case MAGIC_PORTAL:
        {
            int mlev_res;

            play_sfx_sound_at_location(tt == MAGIC_PORTAL ? SFX_MAGIC_PORTAL_ACTIVATE : SFX_LEVEL_TELEPORT_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
            mlev_res = mlevel_tele_trap(mtmp, trap, inescapable, in_sight);
            if (mlev_res)
                return mlev_res;
            break;
        }
        case TELEP_TRAP:
            mtele_trap(mtmp, trap, in_sight);
            break;
        case WEB:
            /* Monster in a web. */
            if (webmaker(mptr))
                break;
            if (mu_maybe_destroy_web(mtmp, in_sight, trap))
                break;
            tear_web = FALSE;
            switch (mtmp->mnum)
            {
            default:
                if (is_bear(mptr) && !in_sight) 
                {
                    play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_HOWL_IN_ANGER);
                    You_hear("the roaring of a confused bear!");
                    mtmp->mtrapped = 1;
                    break;
                }
                else if (mptr->mlet == S_GIANT
                    /* exclude baby dragons and relatively short worms */
                    || (mptr->mlet == S_DRAGON && extra_nasty(mptr))
                    || (mtmp->wormno && count_wsegs(mtmp) > 5)) 
                {
                    tear_web = TRUE;
                }
                else if (in_sight) 
                {
                    play_sfx_sound_at_location(SFX_CAUGHT_IN_WEB, mtmp->mx, mtmp->my);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is caught in %s spider web.", Monnam(mtmp),
                          a_your[trap->madeby_u]);
                    seetrap(trap);
                }
                mtmp->mtrapped = tear_web ? 0 : 1;
                break;
            /* this list is fairly arbitrary; it deliberately
               excludes wumpus & giant/ettin zombies/mummies */
            case PM_TITANOTHERE:
            case PM_BALUCHITHERIUM:
            case PM_PURPLE_WORM:
            case PM_JABBERWOCK:
            case PM_IRON_GOLEM:
            case PM_BALOR:
            case PM_KRAKEN:
            case PM_MASTODON:
            case PM_ORION:
            case PM_NORN:
            case PM_CYCLOPS:
            case PM_LORD_SURTUR:
                tear_web = TRUE;
                break;
            }

            if (tear_web)
            {
                if (in_sight)
                    pline("%s tears through %s spider web!", Monnam(mtmp),
                          a_your[trap->madeby_u]);
                deltrap(trap);
                newsym(mtmp->mx, mtmp->my);
            }
            else if (force_mintrap && !mtmp->mtrapped) 
            {
                if (in_sight) 
                {
                    pline("%s avoids %s spider web!", Monnam(mtmp),
                          a_your[trap->madeby_u]);
                    seetrap(trap);
                }
            }
            break;
        case STATUE_TRAP:
            break;
        case MAGIC_TRAP:
            /* A magic trap.  Monsters usually immune. */
            if (!rn2(21))
                goto mfiretrap;
            else
            {
                if (can_see_trap)
                {
                    newsym(mtmp->mx, mtmp->my);
                    play_special_effect_at(SPECIAL_EFFECT_MAGIC_TRAP_LIGHT_FLASH, 0, mtmp->mx, mtmp->my, FALSE);
                    play_sfx_sound_at_location(SFX_MAGIC_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
                    special_effect_wait_until_action(0);
                    special_effect_wait_until_end(0);
                }
            }
            break;
        case ANTI_MAGIC_TRAP:
            if (can_see_trap)
            {
                newsym(mtmp->mx, mtmp->my);
                play_special_effect_at(SPECIAL_EFFECT_ANTIMAGIC_TRAP_EFFECT, 0, mtmp->mx, mtmp->my, FALSE);
                play_sfx_sound_at_location(SFX_ANTI_MAGIC_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
                special_effect_wait_until_action(0);
            }
            /* similar to hero's case, more or less */
            if (!resists_magic(mtmp)) { /* lose spell energy */
                if (!is_cancelled(mtmp) && (attacktype(mptr, AT_MAGC)
                                    || attacktype(mptr, AT_BREA))) 
                {
                    mtmp->mspec_used += d(2, 20);
                    if (in_sight && can_see_trap)
                    {
                        seetrap(trap);
                        pline("%s seems lethargic.", Monnam(mtmp));
                    }
                }
            } 
            else
            { /* take some damage */
                int dmgval2 = rnd(4);

                if ((otmp = MON_WEP(mtmp)) != 0
                    && otmp->oartifact && artifact_has_flag(otmp, AF_MAGIC_ABSORBING))
                    dmgval2 += rnd(4);
                if (passes_walls(mptr))
                    dmgval2 = (dmgval2 + 3) / 4;

                if (in_sight)
                    seetrap(trap);

                deduct_monster_hp(mtmp, adjust_damage(dmgval2, (struct monst*)0, mtmp, AD_MAGM, ADFLAGS_NONE));

                if (DEADMONSTER(mtmp))
                    monkilled(mtmp, in_sight ? "compression from an anti-magic field" : (const char *) 0, AD_MAGM, 0);
                if (DEADMONSTER(mtmp))
                    trapkilled = TRUE;
                if (see_it)
                    newsym(trap->tx, trap->ty);
            }
            if(can_see_trap)
                special_effect_wait_until_end(0);
            break;
        case LANDMINE:
        {
            boolean spef_on = FALSE;
            if (rn2(3))
                break; /* monsters usually don't set it off */
            if (is_flying(mtmp) || is_levitating(mtmp)) 
            {
                boolean already_seen = trap->tseen;

                if (in_sight && !already_seen) 
                {
                    pline("A trigger appears in a pile of soil below %s.",
                          mon_nam(mtmp));
                    seetrap(trap);
                }
                if (rn2(3))
                    break;
                if (in_sight)
                {
                    newsym(mtmp->mx, mtmp->my);
                    pline_The("air currents set %s off!",
                              already_seen ? "a land mine" : "it");
                }
            } 
            else if (in_sight) 
            {
                newsym(mtmp->mx, mtmp->my);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s%s triggers %s land mine!",
                      !Deaf ? "KAABLAMM!!!  " : "", Monnam(mtmp),
                      a_your[trap->madeby_u]);
            }

            if (in_sight && can_see_trap)
            {
                flush_screen(1);
                play_special_effect_at(SPECIAL_EFFECT_LAND_MINE_EXPLOSION, 0, trap->tx, trap->ty, FALSE);
                play_sfx_sound_at_location(SFX_LAND_MINE_ACTIVATE, mtmp->mx, mtmp->my);
                special_effect_wait_until_action(0);
                play_sfx_sound_at_location_with_minimum_volume(SFX_EXPLOSION_FIERY, mtmp->mx, mtmp->my, 0.25);
                spef_on = TRUE;
                context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
            }
            else
                play_sfx_sound_at_location_with_minimum_volume(SFX_EXPLOSION_FIERY, mtmp->mx, mtmp->my, 0.25);

            if ((!in_sight || !can_see_trap) && !Deaf)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "Kaablamm!  You hear an explosion in the distance!");

            blow_up_landmine(trap);
            /* explosion might have destroyed a drawbridge; don't
               dish out more damage if monster is already dead */
            if (DEADMONSTER(mtmp)
                || thitm(0, mtmp, (struct obj *) 0, rnd(16), FALSE))
            {
                trapkilled = TRUE;
            } 
            else 
            {
                /* monsters recursively fall into new pit */
                if (mintrap(mtmp) == 2)
                    trapkilled = TRUE;
            }
            /* a boulder may fill the new pit, crushing monster */
            fill_pit(trap->tx, trap->ty);
            if (DEADMONSTER(mtmp))
                trapkilled = TRUE;
            if (unconscious())
            {
                multi = -1;
                nomovemsg = "The explosion awakens you!";
            }
            if (spef_on)
            {
                special_effect_wait_until_end(0);
                context.global_newsym_flags = 0UL;
            }
            break;
        }
        case POLY_TRAP:
            if (can_see_trap)
            {
                newsym(mtmp->mx, mtmp->my);
                play_special_effect_at(SPECIAL_EFFECT_POLYMORPH_TRAP_EFFECT, 0, mtmp->mx, mtmp->my, FALSE);
                play_sfx_sound_at_location(SFX_POLYMORPH_ACTIVATE, mtmp->mx, mtmp->my);
                special_effect_wait_until_action(0);
            }

            if (resists_magic(mtmp) || has_unchanging(mtmp))
            {
                play_sfx_sound_at_location(SFX_POLYMORPH_FAIL, mtmp->mx, mtmp->my);
                m_shieldeff(mtmp);
            } 
            else if (!check_magic_resistance_and_inflict_damage(mtmp, (struct obj*) 0, (struct monst*)0, FALSE, 0, 0, NOTELL))
            {
                if (newcham(mtmp, (struct permonst*)0, 0, FALSE, FALSE))
                {
                    play_sfx_sound_at_location(SFX_POLYMORPH_SUCCESS, mtmp->mx, mtmp->my); // Since msg is FALSE in newcham
                    /* we're done with mptr but keep it up to date */
                    mptr = mtmp->data;
                }

                if (in_sight)
                    seetrap(trap);
            }
            if(can_see_trap)
                special_effect_wait_until_end(0);
            break;
        case ROLLING_BOULDER_TRAP:
            if (!(is_flying(mtmp) || is_levitating(mtmp))) 
            {
                play_sfx_sound_at_location(SFX_ROLLING_BOOULDER_TRAP_TRIGGER, mtmp->mx, mtmp->my);

                int style = ROLL | (in_sight ? 0 : LAUNCH_UNSEEN);

                newsym(mtmp->mx, mtmp->my);
                if (in_sight)
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Click!  %s triggers %s.", Monnam(mtmp),
                          trap->tseen ? "a rolling boulder trap" : something);
                if (launch_obj(BOULDER, trap->launch.x, trap->launch.y,
                               trap->launch2.x, trap->launch2.y, style)) 
                {
                    if (in_sight)
                        trap->tseen = TRUE;
                    if (DEADMONSTER(mtmp))
                        trapkilled = TRUE;
                }
                else 
                {
                    deltrap(trap);
                    newsym(mtmp->mx, mtmp->my);
                }
            }
            break;
        case VIBRATING_SQUARE:
            if (see_it && !Blind) 
            {
                seetrap(trap); /* before messages */
                if (in_sight) {
                    char buf[BUFSZ], *p, *monnm = mon_nam(mtmp);

                    if (nolimbs(mtmp->data)
                        || is_flying(mtmp) || is_levitating(mtmp)) 
                    {
                        /* just "beneath <mon>" */
                        Strcpy(buf, monnm);
                    }
                    else 
                    {
                        Strcpy(buf, s_suffix(monnm));
                        p = eos(strcat(buf, " "));
                        Strcpy(p, makeplural(mbodypart(mtmp, FOOT)));
                        /* avoid "beneath 'rear paws'" or 'rear hooves' */
                        (void) strsubst(p, "rear ", "");
                    }
                    You_see("a strange vibration beneath %s.", buf);
                }
                else 
                {
                    /* notice something (hearing uses a larger threshold
                       for 'nearby') */
                    You_see("the ground vibrate %s.",
                            (distu(mtmp->mx, mtmp->my) <= 2 * 2)
                               ? "nearby" : "in the distance");
                }
            }
            break;
        case MODRON_PORTAL:
        {
            play_sfx_sound_at_location(SFX_GENERIC_MAGICAL_TRAP_ACTIVATE, mtmp->mx, mtmp->my);
            modronportaltele(trap, mtmp);
            break;
        }
        case LEVER:
            break;
        default:
            impossible("Some monster encountered a strange trap of type %d.",
                       tt);
        }
    }
    if (trapkilled)
        return 2;
    return mtmp->mtrapped;
}

/* Combine cockatrice checks into single functions to avoid repeating code. */
void
instapetrify(str)
const char *str;
{
    if (Stone_resistance)
        return;
    if (poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM))
        return;
    play_sfx_sound(SFX_PETRIFY);
    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "turn to stone...");
    killer.format = KILLED_BY;
    if (str != killer.name)
        Strcpy(killer.name, str ? str : "");
    done(STONING);
}

void
minstapetrify(mon, byplayer)
struct monst *mon;
boolean byplayer;
{
    if (resists_ston(mon))
        return;
    if (poly_when_stoned(mon->data)) {
        mon_to_stone(mon);
        return;
    }
    if (!vamp_stone(mon))
        return;

    /* give a "<mon> is slowing down" message and also remove
       intrinsic speed (comparable to similar effect on the hero) */
    /* message is not here, since instapetrify */

    play_sfx_sound_at_location(SFX_PETRIFY, mon->mx, mon->my);
    if (cansee(mon->mx, mon->my))
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s turns to stone.", Monnam(mon));
    if (byplayer) {
        xkilled(mon, XKILL_NOMSG | XKILL_STONED);
    } else
        monstone(mon);
}

void start_delayed_petrification(mtmp, by_you)
struct monst* mtmp;
boolean by_you;
{
    mtmp->delayed_killer_by_you = by_you;

    int existing_stoning = get_mon_property(mtmp, STONED);
    (void)set_mon_property_verbosely(mtmp, STONED, existing_stoning == 0 ? 5 : max(1, existing_stoning - 1));

    /* unstoned is checked every round in a delayed fashion */
}

void start_delayed_sliming(mtmp, by_you)
struct monst* mtmp;
boolean by_you;
{
    mtmp->delayed_killer_by_you = by_you;

    int existing_sliming = get_mon_property(mtmp, SLIMED);
    (void)set_mon_property_verbosely(mtmp, SLIMED, existing_sliming == 0 ? 10 : max(1, existing_sliming - 1));

}


void
selftouch(arg)
const char *arg;
{
    char kbuf[BUFSZ];

    if (uwep && uwep->otyp == CORPSE && uwep->corpsenm >= LOW_PM && touch_petrifies(&mons[uwep->corpsenm])
        && !Stone_resistance) {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s touch the %s corpse.", arg, corpse_monster_name(uwep));
        Sprintf(kbuf, "%s corpse", an(corpse_monster_name(uwep)));
        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
        instapetrify(kbuf);
        /* life-saved; unwield the corpse if we can't handle it */
        if (!uarmg && !Stone_resistance)
            uwepgone();
    }
    if (uarms && uarms->otyp == CORPSE
        && touch_petrifies(&mons[uarms->corpsenm]) && !Stone_resistance) {
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s touch the %s corpse.", arg, corpse_monster_name(uarms));
        Sprintf(kbuf, "%s corpse", an(corpse_monster_name(uarms)));
        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE_CORPSE;
        instapetrify(kbuf);
        /* life-saved; unwield the corpse */
        if (!uarmg && !Stone_resistance)
            uwep2gone();
    }
}

void
mselftouch(mon, arg, byplayer)
struct monst *mon;
const char *arg;
boolean byplayer;
{
    struct obj *mwep = MON_WEP(mon);

    if (mwep && mwep->otyp == CORPSE && touch_petrifies(&mons[mwep->corpsenm])
        && !resists_ston(mon)) {
        if (cansee(mon->mx, mon->my)) {
            pline("%s%s touches %s.", arg ? arg : "",
                  arg ? mon_nam(mon) : Monnam(mon),
                  corpse_xname(mwep, (const char *) 0, CXN_PFX_THE));
        }

        start_delayed_petrification(mon, byplayer);

        //minstapetrify(mon, byplayer);
        /* if life-saved, might not be able to continue wielding */
        //if (!DEADMONSTER(mon) && !which_armor(mon, W_ARMG) && !resists_ston(mon))
        //    mwepgone(mon);
    }
}

/* start levitating */
void
float_up()
{
    play_movement_sound(&youmonst, CLIMBING_TYPE_STAIRS_UP);
    context.botl = context.botlx = TRUE;
    if (u.utrap) {
        if (u.utraptype == TT_PIT) {
            reset_utrap(FALSE);
            You("float up, out of the pit!");
            vision_full_recalc = 1; /* vision limits change */
            fill_pit(u.ux, u.uy);
        } else if (u.utraptype == TT_LAVA /* molten lava */
                   || u.utraptype == TT_INFLOOR) { /* solidified lava */
            Your("body pulls upward, but your %s are still stuck.",
                 makeplural(body_part(LEG)));
        } else if (u.utraptype == TT_BURIEDBALL) { /* tethered */
            coord cc;

            cc.x = u.ux, cc.y = u.uy;
            /* caveat: this finds the first buried iron ball within
               one step of the specified location, not necessarily the
               buried [former] uball at the original anchor point */
            (void) buried_ball(&cc);
            /* being chained to the floor blocks levitation from floating
               above that floor but not from enhancing carrying capacity */
            You("feel lighter, but your %s is still chained to the %s.",
                body_part(LEG),
                IS_ROOM(levl[cc.x][cc.y].typ) ? "floor" : "ground");
        } else if (u.utraptype == WEB) {
            You("float up slightly, but you are still stuck in the web.");
        } else { /* bear trap */
            You("float up slightly, but your %s is still stuck.",
                body_part(LEG));
        }
        /* when still trapped, float_vs_flight() below will block levitation */
#if 0
    } else if (Is_waterlevel(&u.uz)) {
        pline("It feels as though you've lost some weight.");
#endif
    } else if (u.uinwater) {
        spoteffects(TRUE);
    } else if (u.uswallow) {
        You(is_animal(u.ustuck->data) ? "float away from the %s."
                                      : "spiral up into %s.",
            is_animal(u.ustuck->data) ? surface(u.ux, u.uy)
                                      : mon_nam(u.ustuck));
    } else if (Hallucination) {
        pline("Up, up, and awaaaay!  You're walking on air!");
    } else if (Is_airlevel(&u.uz)) {
        You("gain control over your movements.");
    } else {
        You("start to float in the air!");
    }
    if (u.usteed && !is_levitating(u.usteed) && !is_flying(u.usteed)) {
        if (Lev_at_will) {
            pline("%s magically floats up!", Monnam(u.usteed));
        } else {
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "cannot stay on %s.", mon_nam(u.usteed));
            dismount_steed(DISMOUNT_GENERIC);
        }
    }
    if (Flying)
        You("are no longer able to control your flight.");
    float_vs_flight(); /* set BFlying, also BLevitation if still trapped */
    /* levitation gives maximum carrying capacity, so encumbrance
       state might be reduced */
    (void) encumber_msg();
    newsym(u.ux, u.uy);

    return;
}

void
fill_pit(x, y)
int x, y;
{
    struct obj *otmp;
    struct trap *t;

    if ((t = t_at(x, y)) && is_pit(t->ttyp)
        && (otmp = sobj_at(BOULDER, x, y))) {
        obj_extract_self(otmp);
        (void) flooreffects(otmp, x, y, "settle");
    }
}

/* stop levitating */
int
float_down(hmask, emask)
long hmask, emask; /* might cancel timeout */
{
    register struct trap *trap = (struct trap *) 0;
    d_level current_dungeon_level;
    boolean no_msg = FALSE;

    play_movement_sound(&youmonst, CLIMBING_TYPE_STAIRS_DOWN);

    HLevitation &= ~hmask;
    ELevitation &= ~emask;
    if (Levitation)
        return 0; /* maybe another ring/potion/boots */
    if (Blocks_Levitation) {
        /* if blocked by terrain, we haven't actually been levitating so
           we don't give any end-of-levitation feedback or side-effects,
           but if blocking is solely due to being trapped in/on floor,
           do give some feedback but skip other float_down() effects */
        boolean trapped = (HBlocks_Levitation == I_SPECIAL);

        float_vs_flight();
        if (trapped && u.utrap) /* u.utrap => paranoia */
            You("are no longer trying to float up from the %s.",
                (u.utraptype == TT_BEARTRAP) ? "trap's jaws"
                  : (u.utraptype == TT_WEB) ? "web"
                      : (u.utraptype == TT_BURIEDBALL) ? "chain"
                          : (u.utraptype == TT_LAVA) ? "lava"
                              : "ground"); /* TT_INFLOOR */
        (void) encumber_msg(); /* carrying capacity might have changed */
        return 0;
    }
    context.botl = context.botlx = TRUE;
    nomul(0); /* stop running or resting */
    if (Blocks_Flying) {
        /* controlled flight no longer overridden by levitation */
        float_vs_flight(); /* clears BFlying & I_SPECIAL
                            * unless hero is stuck in floor */
        if (Flying) {
            You("have stopped levitating and are now flying.");
            (void) encumber_msg(); /* carrying capacity might have changed */
            return 1;
        }
    }

    if (u.uswallow) {
        You("float down, but you are still %s.",
            is_animal(u.ustuck->data) ? "swallowed" : "engulfed");
        (void) encumber_msg();
        return 1;
    }

    if (Punished && !carried(uball)
        && (is_pool(uball->ox, uball->oy)
            || ((trap = t_at(uball->ox, uball->oy))
                && (is_pit(trap->ttyp) || is_hole(trap->ttyp))))) {
        u.ux0 = u.ux;
        u.uy0 = u.uy;
        u.ux = uball->ox;
        u.uy = uball->oy;
        movobj(uchain, uball->ox, uball->oy);
        newsym(u.ux0, u.uy0);
        vision_full_recalc = 1; /* in case the hero moved. */
    }
    /* check for falling into pool - added by GAN 10/20/86 */
    if (!Flying) {
        if (!u.uswallow && u.ustuck) {
            if (sticks(youmonst.data))
                You("aren't able to maintain your hold on %s.",
                    mon_nam(u.ustuck));
            else
                pline("Startled, %s can no longer hold you!",
                      mon_nam(u.ustuck));
            u.ustuck = 0;
        }
        /* kludge alert:
         * drown() and lava_effects() print various messages almost
         * every time they're called which conflict with the "fall
         * into" message below.  Thus, we want to avoid printing
         * confusing, duplicate or out-of-order messages.
         * Use knowledge of the two routines as a hack -- this
         * should really be handled differently -dlc
         */
        if (is_pool(u.ux, u.uy) && !Walks_on_water && !Swimming && !u.uinwater)
            no_msg = drown();

        if (is_lava(u.ux, u.uy)) {
            (void) lava_effects();
            no_msg = TRUE;
        }
    }
    if (!trap) {
        trap = t_at(u.ux, u.uy);
        if (Is_airlevel(&u.uz)) {
            play_sfx_sound(SFX_TUMBLE_IN_AIR);
            You("begin to tumble in place.");
        } else if (Is_waterlevel(&u.uz) && !no_msg) {
            You_feel("heavier.");
        /* u.uinwater msgs already in spoteffects()/drown() */
        } else if (!u.uinwater && !no_msg) {
            if (!(emask & W_SADDLE)) {
                if (Sokoban && trap) {
                    /* Justification elsewhere for Sokoban traps is based
                     * on air currents.  This is consistent with that.
                     * The unexpected additional force of the air currents
                     * once levitation ceases knocks you off your feet.
                     */
                    if (Hallucination)
                        pline("Bummer!  You've crashed.");
                    else
                        You("fall over.");
                    losehp(adjust_damage(rnd(2), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "dangerous winds", KILLED_BY);
                    if (u.usteed)
                        dismount_steed(DISMOUNT_FELL);
                    selftouch("As you fall, you");
                } else if (u.usteed && (is_levitating(u.usteed)
                                        || is_flying(u.usteed))) {
                    You("settle more firmly in the saddle.");
                } else if (Hallucination) {
                    pline("Bummer!  You've %s.",
                          is_pool(u.ux, u.uy)
                             ? "splashed down"
                             : "hit the ground");
                } else {
                    You("float gently to the %s.", surface(u.ux, u.uy));
                }
            }
        }
    }

    /* levitation gives maximum carrying capacity, so having it end
       potentially triggers greater encumbrance; do this after
       'come down' messages, before trap activation or autopickup */
    (void) encumber_msg();

    /* can't rely on u.uz0 for detecting trap door-induced level change;
       it gets changed to reflect the new level before we can check it */
    assign_level(&current_dungeon_level, &u.uz);
    if (trap) {
        switch (trap->ttyp) {
        case STATUE_TRAP:
            break;
        case HOLE:
        case TRAPDOOR:
            if (!Can_fall_thru(&u.uz) || u.ustuck)
                break;
            /*FALLTHRU*/
        default:
            if (!u.utrap) /* not already in the trap */
                dotrap(trap, 0);
        }
    }
    if (!Is_airlevel(&u.uz) && !Is_waterlevel(&u.uz) && !u.uswallow
        /* falling through trap door calls goto_level,
           and goto_level does its own pickup() call */
        && on_level(&u.uz, &current_dungeon_level))
        (void) pickup(1, FALSE);

    newsym(u.ux, u.uy);

    return 1;
}

/* shared code for climbing out of a pit */
void
climb_pit()
{
    if (!u.utrap || u.utraptype != TT_PIT)
        return;

    if (Passes_walls) {
        /* marked as trapped so they can pick things up */
        You("ascend from the pit.");
        reset_utrap(FALSE);
        fill_pit(u.ux, u.uy);
        vision_full_recalc = 1; /* vision limits change */
    } else if (!rn2(2) && sobj_at(BOULDER, u.ux, u.uy)) {
        Your("%s gets stuck in a crevice.", body_part(LEG));
        display_nhwindow(WIN_MESSAGE, FALSE);
        clear_nhwindow(WIN_MESSAGE);
        You("free your %s.", body_part(LEG));
    } else if ((Flying || is_clinger(youmonst.data)) && !Sokoban) {
        /* eg fell in pit, then poly'd to a flying monster;
           or used '>' to deliberately enter it */
        You("%s from the pit.", Flying ? "fly" : "climb");
        reset_utrap(FALSE);
        fill_pit(u.ux, u.uy);
        vision_full_recalc = 1; /* vision limits change */
    } else if (!(--u.utrap)) {
        reset_utrap(FALSE);
        play_movement_sound(&youmonst, CLIMBING_TYPE_STAIRS_UP);
        You("%s to the edge of the pit.",
            (Sokoban && Levitation)
                ? "struggle against the air currents and float"
                : u.usteed ? "ride" : "crawl");
        fill_pit(u.ux, u.uy);
        vision_full_recalc = 1; /* vision limits change */
    } else if (u.dz || flags.verbose) {
        play_sfx_sound(SFX_STUCK_IN_TRAP);
        if (u.usteed)
            Norep("%s is still in a pit.", upstart(y_monnam(u.usteed)));
        else
            Norep((Hallucination && !rn2(5))
                      ? "You've fallen, and you can't get up."
                      : "You are still in a pit.");
    }

    newsym(u.ux, u.uy);
}

STATIC_OVL void
dofiretrap(box, dice)
struct obj *box; /* null for floor trap */
int dice; /* of d6 */
{
    boolean see_it = !Blind;
    int num, alt;

    if (dice < 1)
        dice = 1;

    /* Bug: for box case, the equivalent of burn_floor_objects() ought
     * to be done upon its contents.
     */

    if ((box && !carried(box)) ? is_pool(box->ox, box->oy) : Underwater) {
        play_sfx_sound(SFX_STEAMY_BUBBLES);
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A cascade of steamy bubbles erupts from %s!",
              the(box ? xname(box) : surface(u.ux, u.uy)));
        if (Fire_immunity)
            You("are uninjured.");
        else
            losehp(adjust_damage(rnd(3), (struct monst*)0, &youmonst, AD_FIRE, ADFLAGS_NONE), "boiling water", KILLED_BY);
        return;
    }
    play_sfx_sound(SFX_TOWER_OF_FLAME_ERUPTS);
    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A %s %s from %s!", tower_of_flame, box ? "bursts" : "erupts",
          the(box ? xname(box) : surface(u.ux, u.uy)));

    if (Fire_immunity) 
    {
        u_shieldeff();
        num = 0;
    }
    else if (Upolyd) 
    {
        num = d(dice, 6);
        switch (u.umonnum) 
        {
        case PM_PAPER_GOLEM:
            alt = u.mhmax;
            break;
        case PM_STRAW_GOLEM:
            alt = u.mhmax / 2;
            break;
        case PM_WOOD_GOLEM:
            alt = u.mhmax / 4;
            break;
        case PM_LEATHER_GOLEM:
            alt = u.mhmax / 8;
            break;
        default:
            alt = 0;
            break;
        }
        if (alt > num)
            num = alt;
#if 0
        if (u.mhmax > mons[u.umonnum].mlevel)
        {
            u.basemhmax -= rn2(min(u.mhmax, num + 1));
            updatemaxhp();
            context.botl = 1;
        }
#endif
    }
    else 
    {
        num = d(dice, 6);
#if 0
        if (u.uhpmax > u.ulevel)
        {
            u.ubasehpmax -= rn2(min(u.uhpmax, num + 1));
            updatemaxhp();
            context.botl = 1;
        }
#endif
    }

    double damage = adjust_damage(num, (struct monst*)0, &youmonst, AD_FIRE, ADFLAGS_NONE);
    if (damage <= 0.0)
        You("are uninjured.");
    else
    {
        int hp_before = Upolyd ? u.mh : u.uhp;
        losehp(damage, tower_of_flame, KILLED_BY_AN); /* fire damage */
        int hp_after = Upolyd ? u.mh : u.uhp;
        int damage_done = hp_before - hp_after;
        if (damage_done > 0)
        {
            int multicolors[2] = { CLR_RED, NO_COLOR };
            You_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolors, "sustain %d damage%s", damage_done, exclam(damage_done));
        }
    }

    burn_away_slime();

    if (burnarmor(&youmonst) || rn2(3))
    {
        destroy_item(SCROLL_CLASS, AD_FIRE);
        destroy_item(SPBOOK_CLASS, AD_FIRE);
        destroy_item(POTION_CLASS, AD_FIRE);

        item_destruction_hint(AD_FIRE, FALSE);

        /* Check for the Ruling Ring of Yendor */
        for (struct obj* oring = invent; oring; oring = oring->nobj)
        {
            if (oring->oartifact == ART_RULING_RING_OF_YENDOR)
            {
                read_the_ruling_ring(oring);
                break;
            }
        }
    }


    if (!box && burn_floor_objects(u.ux, u.uy, see_it, TRUE) && !see_it)
        You("smell paper burning.");
    if (is_ice(u.ux, u.uy))
        melt_ice(u.ux, u.uy, (char *) 0);
}

STATIC_OVL void
domagictrap()
{
    register int fate = rnd(20);

    /* What happened to the poor sucker? */

    if (fate < 10) 
    {
        /* Most of the time, it creates some monsters. */
        int cnt = rnd(4);

        play_special_effect_at(SPECIAL_EFFECT_MAGIC_TRAP_LIGHT_FLASH, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_FLASH_AND_ROAR);
        special_effect_wait_until_action(0);
        /* blindness effects */
        if (!resists_blnd(&youmonst) && !Flash_resistance)
        {
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are momentarily blinded by a flash of light!");
            make_blinded((long) rn1(5, 10), FALSE);
            if (!Blind)
                Your1(vision_clears);
        } else if (!Blind) {
            You_see_ex(ATR_NONE, CLR_MSG_NEGATIVE, "a flash of light!");
        }

        /* deafness effects */
        if (!Deaf) 
        {
            You_hear_ex(ATR_NONE, CLR_MSG_NEGATIVE, "a deafening roar!");
            incr_itimeout(&HDeaf, rn1(20, 30));
            context.botl = context.botlx = TRUE;
            refresh_u_tile_gui_info(TRUE);
            play_environment_ambient_sounds();
        } 
        else
        {
            /* magic vibrations still hit you */
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "rankled.");
            incr_itimeout(&HDeaf, rn1(5, 15));
            context.botl = context.botlx = TRUE;
            refresh_u_tile_gui_info(TRUE);
            play_environment_ambient_sounds();
        }
        context.makemon_spef_idx = 0;
        while (cnt--)
        {
            struct monst* mtmp = makemon((struct permonst*)0, u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL));
            if (mtmp)
                context.makemon_spef_idx++;
        }
        makemon_animation_wait_until_end();
        /* roar: wake monsters in vicinity, after placing trap-created ones */
        wake_nearto(u.ux, u.uy, 7 * 7);
        /* [flash: should probably also hit nearby gremlins with light] */
        special_effect_wait_until_end(0);
    }
    else
    {
        play_special_effect_at(fate == 12 ? SPECIAL_EFFECT_TRAP_FIRE : SPECIAL_EFFECT_MAGIC_TRAP_EFFECT, 0, u.ux, u.uy, FALSE);
        play_sfx_sound(SFX_GENERIC_MAGICAL_TRAP_ACTIVATE);
        special_effect_wait_until_action(0);

        switch (fate)
        {
        case 10:
        case 11:
            /* sometimes nothing happens */
            break;
        case 12: /* a flash of fire */
            dofiretrap((struct obj*)0, In_endgame(&u.uz) ? 25 : max(2, min(20, (level_difficulty() * 2) / 3)));
            break;

            /* odd feelings */
        case 13:
            //play_sfx_sound(SFX_SHIVER_RUNS_DOWN_SPINE);
            play_simple_player_sound(MONSTER_SOUND_TYPE_SHUDDER);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "A shiver runs up and down your %s!", body_part(SPINE));
            break;
        case 14:
            play_sfx_sound(SFX_DISTANT_HOWLING);
            You_hear_ex(ATR_NONE, CLR_MSG_WARNING, Hallucination ? "the moon howling at you."
                : "distant howling.");
            break;
        case 15:
            play_sfx_sound(SFX_MAGIC_TRAP_WEIRD_EFFECT);
            if (on_level(&u.uz, &qstart_level))
                You_feel_ex(ATR_NONE, CLR_MSG_WARNING,
                    "%slike the prodigal son.",
                    (flags.female || (Upolyd && is_neuter(youmonst.data)))
                    ? "oddly "
                    : "");
            else
                You_ex(ATR_NONE, CLR_MSG_WARNING, "suddenly yearn for %s.",
                    Hallucination
                    ? "Cleveland"
                    : (In_quest(&u.uz) || at_dgn_entrance("The Quest"))
                    ? "your nearby homeland"
                    : "your distant homeland");
            break;
        case 16:
            play_sfx_sound(SFX_PACK_SHAKING);
            Your_ex(ATR_NONE, CLR_MSG_WARNING, "pack shakes violently!");
            break;
        case 17:
            play_sfx_sound(SFX_MAGIC_TRAP_WEIRD_EFFECT);
            You_ex(ATR_NONE, CLR_MSG_WARNING, Hallucination ? "smell hamburgers." : "smell charred flesh.");
            break;
        case 18:
            play_sfx_sound(SFX_MAGIC_TRAP_WEIRD_EFFECT);
            You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "tired.");
            break;

            /* very occasionally something nice happens. */
        case 19: { /* tame nearby monsters */
            int i, j;
            struct monst* mtmp;

            play_sfx_sound(SFX_GAIN_ABILITY);
            (void)adjattrib(A_CHA, 1, FALSE);
            int cnt = 1;
            for (i = -1; i <= 1; i++)
            {
                for (j = -1; j <= 1; j++)
                {
                    if (!isok(u.ux + i, u.uy + j))
                        continue;
                    mtmp = m_at(u.ux + i, u.uy + j);
                    if (mtmp)
                    {
                        if (tamedog(mtmp, (struct obj*)0, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, FALSE, FALSE))
                        {
                            if (canseemon(mtmp))
                            {
                                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, cnt, mtmp->mx, mtmp->my, FALSE);
                                cnt++;
                                play_sfx_sound(SFX_TAMING);
                                special_effect_wait_until_action(cnt);
                                if (iflags.using_gui_sounds)
                                    delay_output_milliseconds(100);
                            }
                        }
                    }
                }
            }
            for (int idx = 1; idx < cnt; idx++)
            {
                special_effect_wait_until_end(idx);
            }
            break;
        }
        case 20: { /* uncurse stuff */
            struct obj pseudo;
            long save_conf = HConfusion;

            play_sfx_sound(SFX_UNCURSE_ITEM_SUCCESS);
            pseudo = zeroobj; /* neither cursed nor blessed,
                                 and zero out oextra */
            pseudo.otyp = SCR_REMOVE_CURSE;
            HConfusion = 0L;
            boolean effect_happened = 0;
            (void)seffects(&pseudo, &effect_happened, &youmonst);
            HConfusion = save_conf;
            break;
        }
        default:
            break;
        }

        special_effect_wait_until_end(0);
    }
}

/* Set an item on fire.
 *   "force" means not to roll a luck-based protection check for the
 *     item.
 *   "x" and "y" are the coordinates to dump the contents of a
 *     container, if it burns up.
 *
 * Return whether the object was destroyed.
 */
boolean
fire_damage(obj, force, x, y)
struct obj *obj;
boolean force;
xchar x, y;
{
    int chance;
    struct obj *otmp, *ncobj;
    int in_sight = !Blind && couldsee(x, y); /* Don't care if it's lit */
    int dindx;

    /* object might light in a controlled manner */
    if (catch_lit(obj))
        return FALSE;

    if (Is_container(obj))
    {
        switch (obj->otyp) 
        {
        case ICE_BOX:
            return FALSE; /* Immune */
        case CHEST:
            chance = 40;
            break;
        case LARGE_BOX:
            chance = 30;
            break;
        case BOOKSHELF:
            chance = 50;
            break;
        default:
            chance = 20;
            break;
        }
        if ((!force && (Luck + 5) > rn2(chance))
            || (is_flammable(obj) && obj->oerodeproof))
            return FALSE;
        /* Container is burnt up - dump contents out */
        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BURNT);
        if (in_sight)
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s catches fire and burns.", Yname2(obj));
        if (Has_contents(obj))
        {
            if (in_sight)
                pline("Its contents fall out.");
            for (otmp = obj->cobj; otmp; otmp = ncobj) 
            {
                ncobj = otmp->nobj;
                obj_extract_self(otmp);
                if (!flooreffects(otmp, x, y, ""))
                    place_object(otmp, x, y);
            }
        }
        setnotworn(obj);
        delobj(obj);
        return TRUE;
    } 
    else if (!force && (Luck + 5) > rn2(20)) 
    {
        /*  chance per item of sustaining damage:
          *     max luck (Luck==13):    10%
          *     avg luck (Luck==0):     75%
          *     awful luck (Luck<-4):  100%
          */
        return FALSE;
    }
    else if (obj->oclass == SCROLL_CLASS || obj->oclass == SPBOOK_CLASS) 
    {
        if (obj->otyp == SPE_BOOK_OF_THE_DEAD) 
        {
            if (in_sight)
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Smoke rises from %s.", the(xname(obj)));
            return FALSE;
        }

        if (oresist_fire(obj))
            return FALSE;

        dindx = (obj->oclass == SCROLL_CLASS) ? 3 : 4;

        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BURNT);
        if (in_sight)
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s.", Yname2(obj),
                  destroy_strings[dindx][(obj->quan > 1L)]);
        setnotworn(obj);
        delobj(obj);
        return TRUE;
    } 
    else if (obj->oclass == POTION_CLASS)
    {
        if (oresist_fire(obj))
            return FALSE;

        play_simple_object_sound(obj, OBJECT_SOUND_TYPE_BURNT);
        dindx = (obj->otyp != POT_OIL) ? 1 : 2;
        if (in_sight)
            pline("%s %s.", Yname2(obj),
                  destroy_strings[dindx][(obj->quan > 1L)]);
        setnotworn(obj);
        delobj(obj);
        return TRUE;
    }
    else if (erode_obj(obj, (char *) 0, ERODE_BURN, EF_DESTROY)
               == ER_DESTROYED)
    {
        return TRUE;
    }
    return FALSE;
}

/*
 * Apply fire_damage() to an entire chain.
 *
 * Return number of objects destroyed. --ALI
 */
int
fire_damage_chain(chain, force, here, x, y)
struct obj *chain;
boolean force, here;
xchar x, y;
{
    struct obj *obj, *nobj;
    int num = 0;

    for (obj = chain; obj; obj = nobj) {
        nobj = here ? obj->nexthere : obj->nobj;
        if (fire_damage(obj, force, x, y))
            ++num;
    }

    if (num && (Blind && !couldsee(x, y)))
        You("smell smoke.");
    return num;
}

/* obj has been thrown or dropped into lava; damage is worse than mere fire */
boolean
lava_damage(obj, x, y)
struct obj *obj;
xchar x, y;
{
    int otyp = obj->otyp, ocls = obj->oclass;

    /* the Amulet, invocation items, and Rider corpses are never destroyed
       (let Book of the Dead fall through to fire_damage() to get feedback) */
    if (obj_resists(obj, 0, 0) && otyp != SPE_BOOK_OF_THE_DEAD)
        return FALSE;
    /* destroy liquid (venom), wax, veggy, flesh, paper (except for scrolls
       and books--let fire damage deal with them), cloth, leather, wood, bone
       unless it's inherently or explicitly fireproof or contains something;
       note: potions are glass so fall through to fire_damage() and boil */
    if (melts_in_lava(obj)
        && ocls != SCROLL_CLASS && ocls != SPBOOK_CLASS
        && !is_obj_indestructible(obj)
        && !oresist_fire(obj)
        /* assumes oerodeproof isn't overloaded for some other purpose on
           non-eroding items */
        && !obj->oerodeproof
        /* fire_damage() knows how to deal with containers and contents */
        && !Has_contents(obj)) {
        if (cansee(x, y)) {
            /* this feedback is pretty clunky and can become very verbose
               when former contents of a burned container get here via
               flooreffects() */
            if (obj == thrownobj || obj == kickedobj)
                pline("%s %s up!", is_plural(obj) ? "They" : "It",
                      otense(obj, "burn"));
            else
                You_see("%s hit lava and burn up!", doname(obj));
        }
        if (carried(obj)) { /* shouldn't happen */
            remove_worn_item(obj, TRUE);
            useupall(obj);
        } else
            delobj(obj);
        return TRUE;
    }
    return fire_damage(obj, TRUE, x, y);
}

void
acid_damage(obj)
struct obj *obj;
{
    /* Scrolls but not spellbooks can be erased by acid. */
    struct monst *victim;
    boolean vismon;

    if (!obj)
        return;

    victim = carried(obj) ? &youmonst : mcarried(obj) ? obj->ocarry : NULL;
    vismon = victim && (victim != &youmonst) && canseemon(victim);

    if (obj->greased) {
        grease_protect(obj, (char *) 0, victim);
    } else if (obj->oclass == SCROLL_CLASS && obj->otyp != SCR_BLANK_PAPER) {
        if (obj->otyp != SCR_BLANK_PAPER
            && obj->otyp != SCR_MAIL
            ) {
            if (!Blind) {
                if (victim == &youmonst)
                    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Your %s.", aobjnam(obj, "fade"));
                else if (vismon)
                    pline("%s %s.", s_suffix(Monnam(victim)),
                          aobjnam(obj, "fade"));
            }
        }
        obj->otyp = SCR_BLANK_PAPER;
        obj->special_quality = 0;
        obj->dknown = 0;
    } else
        erode_obj(obj, (char *) 0, ERODE_CORRODE, EF_GREASE | EF_VERBOSE);
}

/* context for water_damage(), managed by water_damage_chain();
   when more than one stack of potions of acid explode while processing
   a chain of objects, use alternate phrasing after the first message */
STATIC_VAR struct h2o_ctx {
    int dkn_boom, unk_boom; /* track dknown, !dknown separately */
    boolean ctx_valid;
} acid_ctx = { 0, 0, FALSE };

/* Get an object wet and damage it appropriately.
 *   "ostr", if present, is used instead of the object name in some
 *     messages.
 *   "force" means not to roll luck to protect some objects.
 * Returns an erosion return value (ER_*)
 */
int
water_damage(obj, ostr, force)
struct obj *obj;
const char *ostr;
boolean force;
{
    if (!obj)
        return ER_NOTHING;

    if (snuff_lit(obj))
        return ER_DAMAGED;

    boolean erodeprotection = FALSE;

    xchar x = 0, y = 0;
    get_obj_location(obj, &x, &y, 0);

    if (!ostr)
        ostr = cxname(obj);

    if (obj->otyp == CAN_OF_GREASE && obj->charges > 0)
    {
        return ER_NOTHING;
    } 
    else if (obj->otyp == TOWEL && obj->special_quality < 7)
    {
        wet_a_towel(obj, rnd(7), TRUE);
        return ER_NOTHING;
    }
    else if (obj->greased) 
    {
        if (!rn2(2))
            obj->greased = 0;
        if (carried(obj))
            update_inventory();
        return ER_GREASED;
    }
    else if (Is_container(obj) && !Is_box(obj)
        && (obj->otyp != OILSKIN_SACK || (obj->cursed && !rn2(3)))) 
    {
        if (carried(obj))
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Water gets into your %s!", ostr);

        water_damage_chain(obj->cobj, FALSE);
        return ER_DAMAGED; /* contents were damaged */
    } 
    else if (obj->otyp == OILSKIN_SACK)
    {
        if (carried(obj))
            pline("Some water slides right off your %s.", ostr);
        makeknown(OILSKIN_SACK);
        /* not actually damaged, but because we /didn't/ get the "water
           gets into!" message, the player now has more information and
           thus we need to waste any potion they may have used (also,
           flavourwise the water is now on the floor) */
        return ER_DAMAGED;
    } 
    else if (!force && (Luck + 5) > rn2(20)) 
    {
        /*  chance per item of sustaining damage:
            *   max luck:               10%
            *   avg luck (Luck==0):     75%
            *   awful luck (Luck<-4):  100%
            */
        return ER_NOTHING;
    } 
    else if (obj->oclass == SCROLL_CLASS)
    {
        if (obj->otyp == SCR_BLANK_PAPER
            || obj->otyp == SCR_MAIL
           ) return 0;
        else if (is_obj_indestructible(obj))
        {
            return 0;
        }

        if (carried(obj))
        {
            play_sfx_sound_at_location(SFX_SCROLL_FADES, x, y);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Your %s %s.", ostr, vtense(ostr, "fade"));
        }
        obj->otyp = SCR_BLANK_PAPER;
        obj->dknown = 0;
        obj->special_quality = 0;
        if (carried(obj))
            update_inventory();
        return ER_DAMAGED;
    } 
    else if (obj->oclass == SPBOOK_CLASS)
    {
        if (obj->otyp == SPE_BOOK_OF_THE_DEAD)
        {
            play_sfx_sound_at_location(SFX_STEAMY_BUBBLES, x, y);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Steam rises from %s.", the(xname(obj)));
            return 0;
        }
        else if (is_obj_indestructible(obj))
        {
            return 0;
        }
        else if (obj->otyp == SPE_BLANK_PAPER)
        {
            return 0;
        }
        if (carried(obj))
        {
            play_sfx_sound_at_location(SFX_SCROLL_FADES, x, y);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Your %s %s.", ostr, vtense(ostr, "fade"));
        }
        if (objects[obj->otyp].oc_subtyp == BOOKTYPE_NOVEL) 
        {
            obj->novelidx = 0;
            free_oname(obj);
        }
        else if (objects[obj->otyp].oc_subtyp == BOOKTYPE_MANUAL)
        {
            obj->manualidx = 0;
            free_oname(obj);
        }

        obj->otyp = SPE_BLANK_PAPER;
        obj->material = objects[obj->otyp].oc_material;
        obj->dknown = 0;
        if (carried(obj))
            update_inventory();
        return ER_DAMAGED;
    } 
    else if (obj->oclass == POTION_CLASS)
    {
        if (obj->otyp == POT_ACID) {
            char *bufp;
            boolean one = (obj->quan == 1L), update = carried(obj),
                    exploded = FALSE;

            if (Blind && !carried(obj))
                obj->dknown = 0;
            if (acid_ctx.ctx_valid)
                exploded = ((obj->dknown ? acid_ctx.dkn_boom
                                         : acid_ctx.unk_boom) > 0);
            /* First message is
             * "a [potion|<color> potion|potion of acid] explodes"
             * depending on obj->dknown (potion has been seen) and
             * objects[POT_ACID].oc_name_known (fully discovered),
             * or "some {plural version} explode" when relevant.
             * Second and subsequent messages for same chain and
             * matching dknown status are
             * "another [potion|<color> &c] explodes" or plural
             * variant.
             */
            bufp = simpleonames(obj);
            play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, x, y);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s %s %s!", /* "A potion explodes!" */
                  !exploded ? (one ? "A" : "Some")
                            : (one ? "Another" : "More"),
                  bufp, vtense(bufp, "explode"));
            if (acid_ctx.ctx_valid) {
                if (obj->dknown)
                    acid_ctx.dkn_boom++;
                else
                    acid_ctx.unk_boom++;
            }
            setnotworn(obj);
            delobj(obj);
            if (update)
                update_inventory();
            return ER_DESTROYED;
        }
        else if (obj->odiluted)
        {
            if (carried(obj))
            {
                play_sfx_sound_at_location(SFX_POTION_DILUTES, x, y);
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Your %s %s further.", ostr, vtense(ostr, "dilute"));
            }
            obj->otyp = POT_WATER;
            obj->dknown = 0;
            obj->blessed = obj->cursed = 0;
            obj->odiluted = 0;
            if (carried(obj))
                update_inventory();
            return ER_DAMAGED;
        } 
        else if (obj->otyp != POT_WATER)
        {
            if (carried(obj))
            {
                play_sfx_sound_at_location(SFX_POTION_DILUTES, x, y);
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Your %s %s.", ostr, vtense(ostr, "dilute"));
            }
            obj->odiluted++;
            if (carried(obj))
                update_inventory();
            return ER_DAMAGED;
        }
    }
    else 
    {
        if (is_poisonable(obj) && obj->opoisoned)
        {
            obj->opoisoned = 0;
            play_sfx_sound_at_location(SFX_POISON_DISSOLVES, x, y);
            pline("The poison on %s dissolves off.", yname(obj));
        }
        else if (is_elemental_enchantable(obj) && obj->elemental_enchantment)
        {
            if (obj->elemental_enchantment == FIRE_ENCHANTMENT)
            {
                obj->elemental_enchantment = 0;
                play_sfx_sound_at_location(SFX_FLAMES_EXTINQUISHED, x, y);
                pline("The flames on %s are extinguished.", yname(obj));
            }
            else if (obj->elemental_enchantment == LIGHTNING_ENCHANTMENT)
            {
                play_sfx_sound_at_location(SFX_ELECTRICITY_CRACKLES, x, y);
                pline("The electricity on %s crackles.", yname(obj));
            }
            else if (obj->elemental_enchantment == COLD_ENCHANTMENT)
            {
                play_sfx_sound_at_location(SFX_ICY_COATING_FORMS, x, y);
                pline("An icy coating forms on %s.", yname(obj));
                erodeprotection = TRUE;
            }
        }
        return (erodeprotection ? ER_NOTHING : erode_obj(obj, ostr, ERODE_RUST, EF_NONE));
    }
    return ER_NOTHING;
}

void
water_damage_chain(obj, here)
struct obj *obj;
boolean here;
{
    struct obj *otmp;

    /* initialize acid context: so far, neither seen (dknown) potions of
       acid nor unseen have exploded during this water damage sequence */
    acid_ctx.dkn_boom = acid_ctx.unk_boom = 0;
    acid_ctx.ctx_valid = TRUE;

    for (; obj; obj = otmp) {
        otmp = here ? obj->nexthere : obj->nobj;
        water_damage(obj, (char *) 0, FALSE);
    }

    /* reset acid context */
    acid_ctx.dkn_boom = acid_ctx.unk_boom = 0;
    acid_ctx.ctx_valid = FALSE;
}

/*
 * This function is potentially expensive - rolling
 * inventory list multiple times.  Luckily it's seldom needed.
 * Returns TRUE if disrobing made player unencumbered enough to
 * crawl out of the current predicament.
 */
STATIC_OVL boolean
emergency_disrobe(lostsome)
boolean *lostsome;
{
    int invc = inv_cnt(TRUE);

    while (near_capacity() > (Punished ? UNENCUMBERED : SLT_ENCUMBER)) {
        register struct obj *obj, *otmp = (struct obj *) 0;
        register int i;

        /* Pick a random object */
        if (invc > 0) {
            i = rn2(invc);
            for (obj = invent; obj; obj = obj->nobj) {
                /*
                 * Undroppables are: body armor, boots, gloves,
                 * amulets, and rings because of the time and effort
                 * in removing them + loadstone and other cursed stuff
                 * for obvious reasons.
                 */
                if (!(((objects[obj->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && obj->cursed) || obj == uamul
                      || obj == uleft || obj == uright || obj == ublindf
                      || obj == uarm || obj == uarmc || obj == uarmg
                      || obj == uarmf || obj == uarmu || obj == uarmo || obj == uarmb
                      || obj == umisc || obj == umisc2 || obj == umisc3 || obj == umisc4 || obj == umisc5
                      || (obj->cursed && (obj == uarmh || obj == uarms))
                      || welded(obj, &youmonst)))
                    otmp = obj;
                /* reached the mark and found some stuff to drop? */
                if (--i < 0 && otmp)
                    break;

                /* else continue */
            }
        }
        if (!otmp)
            return FALSE; /* nothing to drop! */
        if (otmp->owornmask)
            remove_worn_item(otmp, FALSE);
        *lostsome = TRUE;
        dropx(otmp);
        invc--;
    }
    return TRUE;
}


/*  return TRUE iff player relocated */
boolean
drown()
{
    const char *pool_of_water;
    boolean inpool_ok = FALSE, crawl_ok;
    int i, x, y;

    /* happily wading in the same contiguous pool */
    if (u.uinwater && is_pool(u.ux - u.dx, u.uy - u.dy)
        && (Swimming || Amphibious)) {
        /* water effects on objects every now and then */
        if (!rn2(5))
            inpool_ok = TRUE;
        else
            return FALSE;
    }

    if (!u.uinwater) {
        play_sfx_sound(SFX_FALL_INTO_WATER);
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s into the %s%c", Is_waterlevel(&u.uz) ? "plunge" : "fall",
            hliquid("water"),
            Amphibious || Swimming ? '.' : '!');
        if (!Swimming && !Is_waterlevel(&u.uz))
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "sink like %s.", Hallucination ? "the Titanic" : "a rock");
    }

    water_damage_chain(invent, FALSE);

    if (u.umonnum == PM_GREMLIN && rn2(3))
        (void) split_mon(&youmonst, (struct monst *) 0);
    else if (is_iron(youmonst.data)) 
    {
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "rust!");
        double damage = adjust_damage(d(2, 6), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE);
        i = (int)floor(damage);
        if (u.basemhmax > i)
            u.basemhmax -= i;
        updatemaxhp();
        losehp(damage, "rusting away", KILLED_BY);
    }
    if (inpool_ok)
        return FALSE;

    if ((i = number_leashed()) > 0) {
        play_sfx_sound(SFX_LEASH_GOES_SLACK);
        pline_The("leash%s slip%s loose.", (i > 1) ? "es" : "",
                  (i > 1) ? "" : "s");
        unleash_all();
    }

    if (Amphibious || Swimming) {
        if (Amphibious) {
            if (flags.verbose)
                pline("But you aren't drowning.");
            if (!Is_waterlevel(&u.uz)) {
                if (Hallucination)
                    Your("keel hits the bottom.");
                else
                    You("touch bottom.");
            }
        }
        if (Punished) {
            unplacebc();
            placebc();
        }
        vision_recalc(2); /* unsee old position */
        u.uinwater = 1;
        under_water(1);
        vision_full_recalc = 1;
        return FALSE;
    }

    if ((Teleportation || has_innate_teleportation(youmonst.data)) && !Unaware
        && (Teleport_control || rn2(3) < Luck + 2)) {
        You_ex(ATR_NONE, CLR_MSG_SPELL, "attempt a teleport spell."); /* utcsri!carroll */
        if (!level.flags.noteleport) {
            (void) dotele(FALSE);
            if (!is_pool(u.ux, u.uy))
                return TRUE;
        } else
            pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "attempted teleport spell fails.");
    }
    if (u.usteed) {
        dismount_steed(DISMOUNT_GENERIC);
        if (!is_pool(u.ux, u.uy))
            return TRUE;
    }
    crawl_ok = FALSE;
    x = y = 0; /* lint suppression */
    /* if sleeping, wake up now so that we don't crawl out of water
       while still asleep; we can't do that the same way that waking
       due to combat is handled; note unmul() clears u.usleep */
    if (u.usleep)
        unmul("Suddenly you wake up!");
    /* being doused will revive from fainting */
    if (is_fainted())
        reset_faint();
    /* can't crawl if unable to move (crawl_ok flag stays false) */
    if (multi < 0 || (Upolyd && !youmonst.data->mmove) || Sleeping || Paralyzed_or_immobile)
        goto crawl;
    /* look around for a place to crawl to */
    for (i = 0; i < 100; i++) {
        x = rn1(3, u.ux - 1);
        y = rn1(3, u.uy - 1);
        if (crawl_destination(x, y)) {
            crawl_ok = TRUE;
            goto crawl;
        }
    }
    /* one more scan */
    for (x = u.ux - 1; x <= u.ux + 1; x++)
        for (y = u.uy - 1; y <= u.uy + 1; y++)
            if (crawl_destination(x, y)) {
                crawl_ok = TRUE;
                goto crawl;
            }
crawl:
    if (crawl_ok) {
        boolean lost = FALSE;
        /* time to do some strip-tease... */
        boolean succ = Is_waterlevel(&u.uz) ? TRUE : emergency_disrobe(&lost);

        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "try to crawl out of the %s.", hliquid("water"));
        if (lost)
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "dump some of your gear to lose weight...");
        if (succ) {
            pline("Pheew!  That was close.");
            teleds(x, y, TRUE, FALSE);
            return TRUE;
        }
        /* still too much weight */
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "But in vain.");
    }
    u.uinwater = 1;
    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "drown.");
    for (i = 0; i < 5; i++) { /* arbitrary number of loops */
        /* killer format and name are reconstructed every iteration
           because lifesaving resets them */
        pool_of_water = waterbody_name(u.ux, u.uy);
        killer.format = KILLED_BY_AN;
        /* avoid "drowned in [a] water" */
        if (!strcmp(pool_of_water, "water"))
            pool_of_water = "deep water", killer.format = KILLED_BY;
        Strcpy(killer.name, pool_of_water);
        done(DROWNING);
        /* oops, we're still alive.  better get out of the water. */
        if (safe_teleds(TRUE, FALSE))
            break; /* successful life-save */
        /* nowhere safe to land; repeat drowning loop... */
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "You're still drowning.");
    }
    if (u.uinwater) {
        u.uinwater = 0;
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "find yourself back %s.",
            Is_waterlevel(&u.uz) ? "in an air bubble" : "on land");
    }
    play_environment_ambient_sounds();
    return TRUE;
}

void
drain_en(n)
int n;
{
    int mana_before = u.uen;
    play_sfx_sound(SFX_DRAIN_ENERGY);
    if (!u.uenmax)
    {
        /* energy is completely gone */
        You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "momentarily lethargic.");
    } 
    else if (n == -1)
    {
        You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "your magical energy drain away!");
        u.uen = 0;
        context.botl = 1;
    }
    else 
    {
        /* throttle further loss a bit when there's not much left to lose */
        //if (n > u.uenmax || n > u.ulevel)
        //    n = rnd(n);

        You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "your magical energy drain away%c", (n > u.uen) ? '!' : '.');
        u.uen -= n;
        if (u.uen < 0) {
            /*
            u.ubaseenmax -= rnd(-u.uen);
            updatemaxen();
            if (u.uenmax < 0)
                u.uenmax = 0;
            */
            u.uen = 0;
        }
        context.botl = 1;
    }

    int mana_after = u.uen;
    int mana_loss = mana_before - mana_after;
    if (mana_loss > 0 && isok(u.ux, u.uy))
    {
        char fbuf[BUFSZ];
        Sprintf(fbuf, "%d", -mana_loss);
        display_floating_text(u.ux, u.uy, fbuf, FLOATING_TEXT_MANA_LOSS, ATR_NONE, NO_COLOR, 0UL);
    }
}

/* disarm a trap */
int
dountrap()
{
    if (near_capacity() >= HVY_ENCUMBER) {
        play_sfx_sound(SFX_GENERAL_TOO_MUCH_ENCUMBRANCE);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "You're too strained to do that.");
        return 0;
    }
    if ((nohands(youmonst.data) && !webmaker(youmonst.data))
        || !youmonst.data->mmove) {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "And just how do you expect to do that?");
        return 0;
    } else if (u.ustuck && sticks(youmonst.data)) {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "You'll have to let go of %s first.", mon_nam(u.ustuck));
        return 0;
    }
    if (u.ustuck || (welded(uwep, &youmonst) && bimanual(uwep))) {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        Your_ex(ATR_NONE, CLR_MSG_WARNING, "%s seem to be too busy for that.", makeplural(body_part(HAND)));
        return 0;
    }
    return untrap(FALSE);
}

/* Did the untrap succeed? */
STATIC_OVL int
succeed_untrap(ttyp)
int ttyp;
{
    if (ttyp < 0 || ttyp >= TRAPNUM)
        return FALSE;

    //boolean res = FALSE;
    //int probability = 0;
    int usedskilllevel = max(P_UNSKILLED, min(P_EXPERT, P_SKILL_LEVEL(P_DISARM_TRAP) + (Enhanced_untrap ? 1 : 0))); /* (Role_if(PM_ROGUE) && u.uhave.questart) */

    //probability = untrap_probability(ttmp->ttyp, usedskilllevel, &youmonst);
    if (Fumbling & !rn2(2))
        return -1;

    int trap_dif = trap_difficulty(ttyp, &youmonst);
    int skill_lvl = max(0, usedskilllevel - 1);
    int checkdiff = skill_lvl - trap_dif;
    if (Confusion || Hallucination)
        checkdiff -= 2;
    if (Stunned)
        checkdiff -= 2;
    if (Blind)
        checkdiff -= 1;
    if (Fumbling)
        checkdiff -= 1;

    switch (checkdiff)
    {
    case -1:
        return !rn2(2) ? -1 : 0;
    case 0:
        return !rn2(3) ? -1 : !!rn2(2);
    case 1:
        return !rn2(6) ? -1 : !!rn2(5);
    default:
        if (checkdiff <= -2)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    /* Only spiders know how to deal with webs reliably */
    //if (ttmp->ttyp == WEB && !webmaker(youmonst.data))
    //    probability = 3;
    //if (Confusion || Hallucination)
    //    probability = probability / 2;
    //if (Blind)
    //    probability = (probability * 2) / 3;
    //if (Stunned)
    //    probability = probability / 2;
    //
    ///* Your own traps are better known than others. */
    //if (ttmp && ttmp->madeby_u)
    //    probability = probability * 2;

    //res = (rn2(100) < probability);
    //return res;
}

int
trap_difficulty(trap_type, mon)
int trap_type;
struct monst* mon;
{
    if (trap_type < 0 || trap_type >= TRAPNUM)
        return NOT_DISARMABLE_TRAP;

    int diflevel = trap_type_definitions[trap_type].tdifficulty;
    if (diflevel == SPECIALLY_DISARMABLE_TRAP)
    {
        switch (trap_type)
        {
        case WEB:
            diflevel = webmaker(mon->data) ? SIMPLE_MECHANICAL_TRAP : COMPLEX_MAGICAL_TRAP;
            break;
        default:
            diflevel = NOT_DISARMABLE_TRAP;
            break;
        }
    }
    return diflevel;
}

int
untrap_probability(trap_type, skill_level, mon)
int trap_type;
int skill_level;
struct monst* mon;
{
    //int probability = 0;
    //switch (skill_level)
    //{
    //case P_UNSKILLED:
    //{
    //    switch (trap_type)
    //    {
    //    case WEB:
    //        probability = webmaker(youmonst.data) ? 50 : 1;
    //        break;
    //    case ANTI_MAGIC_TRAP:
    //    case POLY_TRAP:
    //    case TELEP_TRAP:
    //    case LEVEL_TELEP:
    //    case RUST_TRAP:
    //    case FIRE_TRAP:
    //    case SLP_GAS_TRAP:
    //    case MAGIC_TRAP:
    //        probability = 0;
    //        break;
    //    case ROCKTRAP:
    //    case ROLLING_BOULDER_TRAP:
    //        probability = 10;
    //        break;
    //    default:
    //        probability = 33;
    //        break;
    //    }
    //    break;
    //}
    //case P_BASIC:
    //{
    //    switch (trap_type)
    //    {
    //    case WEB:
    //        probability = webmaker(youmonst.data) ? 100 : 3;
    //        break;
    //    case ANTI_MAGIC_TRAP:
    //    case POLY_TRAP:
    //    case TELEP_TRAP:
    //    case LEVEL_TELEP:
    //    case RUST_TRAP:
    //    case FIRE_TRAP:
    //    case SLP_GAS_TRAP:
    //    case MAGIC_TRAP:
    //        probability = 33;
    //        break;
    //    case ROCKTRAP:
    //    case ROLLING_BOULDER_TRAP:
    //        probability = 50;
    //        break;        
    //    default:
    //        probability = 66;
    //        break;
    //    }
    //    break;
    //}
    //case P_SKILLED:
    //{
    //    switch (trap_type)
    //    {
    //    case WEB:
    //        probability = webmaker(youmonst.data) ? 200 : 10;
    //        break;
    //    case ANTI_MAGIC_TRAP:
    //    case POLY_TRAP:
    //    case TELEP_TRAP:
    //    case LEVEL_TELEP:
    //    case RUST_TRAP:
    //    case FIRE_TRAP:
    //    case SLP_GAS_TRAP:
    //    case MAGIC_TRAP:
    //        probability = 66;
    //        break;
    //    case ROCKTRAP:
    //    case ROLLING_BOULDER_TRAP:
    //        probability = 75;
    //        break;
    //    default:
    //        probability = 83;
    //        break;
    //    }
    //    break;
    //}
    //case P_EXPERT:
    //{
    //    switch (trap_type)
    //    {
    //    case WEB:
    //        probability = webmaker(youmonst.data) ? 400 : 33;
    //        break;
    //    case ANTI_MAGIC_TRAP:
    //    case POLY_TRAP:
    //    case TELEP_TRAP:
    //    case LEVEL_TELEP:
    //    case RUST_TRAP:
    //    case FIRE_TRAP:
    //    case SLP_GAS_TRAP:
    //    case MAGIC_TRAP:
    //        probability = 83;
    //        break;
    //    case ROCKTRAP:
    //    case ROLLING_BOULDER_TRAP:
    //        probability = 88;
    //        break;
    //    default:
    //        probability = 92;
    //        break;
    //    }
    //    break;
    //}
    //case P_MASTER:
    //{
    //    switch (trap_type)
    //    {
    //    case WEB:
    //        probability = webmaker(youmonst.data) ? 800 : 66;
    //        break;
    //    case ANTI_MAGIC_TRAP:
    //    case POLY_TRAP:
    //    case TELEP_TRAP:
    //    case LEVEL_TELEP:
    //    case RUST_TRAP:
    //    case FIRE_TRAP:
    //    case SLP_GAS_TRAP:
    //    case MAGIC_TRAP:
    //        probability = 90;
    //        break;
    //    case ROCKTRAP:
    //    case ROLLING_BOULDER_TRAP:
    //        probability = 95;
    //        break;
    //    default:
    //        probability = 97;
    //        break;
    //    }
    //    break;
    //}
    //case P_GRAND_MASTER:
    //{
    //    switch (trap_type)
    //    {
    //    case WEB:
    //        probability = webmaker(youmonst.data) ? 1600 : 90;
    //        break;
    //    case ANTI_MAGIC_TRAP:
    //    case POLY_TRAP:
    //    case TELEP_TRAP:
    //    case LEVEL_TELEP:
    //    case RUST_TRAP:
    //    case FIRE_TRAP:
    //    case SLP_GAS_TRAP:
    //    case MAGIC_TRAP:
    //        probability = 95;
    //        break;
    //    case ROCKTRAP:
    //    case ROLLING_BOULDER_TRAP:
    //        probability = 100;
    //        break;
    //    default:
    //        probability = 100;
    //        break;
    //    }
    //    break;
    //}
    //default:
    //    break;
    //}

    int trap_dif = trap_difficulty(trap_type, mon);
    int skill_lvl = max(0, skill_level - 1);
    int checkdiff = skill_lvl - trap_dif;

    switch (checkdiff)
    {
    case 0:
        return 33;
    case 1:
        return 66;
    default:
        if (checkdiff <= -1)
        {
            return 0;
        }
        else
        {
            return 100;
        }
    }    
}

/* Replace trap with object(s).  Helge Hafting */
void
cnv_trap_obj(otyp, cnt, ttmp, bury_it, set_found)
int otyp;
int cnt;
struct trap *ttmp;
boolean bury_it, set_found;
{
    struct obj *otmp = mksobj(otyp, TRUE, FALSE, FALSE);

    otmp->quan = cnt;
    otmp->owt = weight(otmp);
    /* Only dart traps are capable of being poisonous */
    if (otyp != DART)
        otmp->opoisoned = 0;
    if(set_found)
        obj_set_found(otmp);
    place_object(otmp, ttmp->tx, ttmp->ty);

    if (bury_it)
    {
        /* magical digging first disarms this trap, then will unearth it */
        (void) bury_an_obj(otmp, (boolean *) 0);
    } 
    else 
    {
        /* Sell your own traps only... */
        if (ttmp->madeby_u)
            sellobj(otmp, ttmp->tx, ttmp->ty);
        stackobj(otmp);
    }
    int tx = ttmp->tx, ty = ttmp->ty;
    if (u.utrap && ttmp->tx == u.ux && ttmp->ty == u.uy)
        reset_utrap(TRUE);
    deltrap(ttmp);
    newsym(tx, ty);
}

/* while attempting to disarm an adjacent trap, we've fallen into it */
STATIC_OVL void
move_into_trap(ttmp)
struct trap *ttmp;
{
    int bc = 0;
    xchar x = ttmp->tx, y = ttmp->ty, bx, by, cx, cy;
    boolean unused;

    bx = by = cx = cy = 0; /* lint suppression */
    /* we know there's no monster in the way, and we're not trapped */
    if (!Punished
        || drag_ball(x, y, &bc, &bx, &by, &cx, &cy, &unused, TRUE))
    {
        u.ux0 = u.ux, u.uy0 = u.uy;
        u.ux = x, u.uy = y;
        u.umoved = TRUE;
        newsym(u.ux0, u.uy0);
        vision_recalc(1);
        check_leash(u.ux0, u.uy0);
        if (Punished)
            move_bc(0, bc, bx, by, cx, cy);
        /* marking the trap unseen forces dotrap() to treat it like a new
           discovery and prevents pickup() -> look_here() -> check_here()
           from giving a redundant "there is a <trap> here" message when
           there are objects covering this trap */
        ttmp->tseen = 0; /* hack for check_here() */
        /* trigger the trap */
        iflags.failing_untrap++; /* spoteffects() -> dotrap(,FAILEDUNTRAP) */
        spoteffects(TRUE); /* pickup() + dotrap() */
        iflags.failing_untrap--;
        /* this should no longer be necessary; before the failing_untrap
           hack, Flying hero would not trigger an unseen bear trap and
           setting it not-yet-seen above resulted in leaving it hidden */
        if ((ttmp = t_at(u.ux, u.uy)) != 0)
            ttmp->tseen = 1;
        exercise(A_WIS, FALSE);
    }
}

/* 0: doesn't even try
 * 1: tries and fails
 * 2: succeeds
 */
STATIC_OVL int
try_disarm(ttmp, force_failure)
struct trap *ttmp;
boolean force_failure;
{
    struct monst *mtmp = m_at(ttmp->tx, ttmp->ty);
    int ttype = ttmp->ttyp;
    const char* trapdesc = get_trap_explanation(ttmp);
    boolean under_u = (!u.dx && !u.dy);
    boolean holdingtrap = (ttype == BEAR_TRAP || ttype == WEB);

    /* Test for monster first, monsters are displayed instead of trap. */
    if (mtmp && (!mtmp->mtrapped || !holdingtrap)) 
    {
        play_sfx_sound(SFX_SOMETHING_IN_WAY);
        pline("%s is in the way.", Monnam(mtmp));
        return 0;
    }

    /* We might be forced to move onto the trap's location. */
    if (sobj_at(BOULDER, ttmp->tx, ttmp->ty) && !Passes_walls && !under_u) 
    {
        play_sfx_sound(SFX_SOMETHING_IN_WAY);
        There("is a boulder in your way.");
        return 0;
    }

    /* duplicate tight-space checks from test_move */
    if (u.dx && u.dy && bad_rock(youmonst.data, u.ux, ttmp->ty)
        && bad_rock(youmonst.data, ttmp->tx, u.uy)) 
    {
        if ((invent && (inv_weight() + weight_cap() > 600))
            || bigmonst(youmonst.data)) {
            /* don't allow untrap if they can't get thru to it */
            play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "are unable to reach the %s!",
                trapdesc);
            return 0;
        }
    }

    /* untrappable traps are located on the ground. */
    if (!can_reach_floor(under_u))
    {
        if (u.usteed && P_SKILL_LEVEL(P_RIDING) < P_BASIC)
            rider_cant_reach();
        else
        {
            play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "are unable to reach the %s!",
                trapdesc);
        }
        return 0;
    }

    /* Will our hero succeed? */
    int success = succeed_untrap(ttmp->ttyp);
    if (force_failure || success <= 0)
    {
        play_sfx_sound(SFX_DISARM_TRAP_FAIL);
        if (success == -1) //rnl(5))
        {
            pline("Whoops...");
            if (mtmp) 
            { /* must be a trap that holds monsters */
                if (ttype == BEAR_TRAP) 
                {
                    if (mtmp->mtame)
                        abuse_dog(mtmp);

                    deduct_monster_hp(mtmp, adjust_damage(rnd(4), (struct monst*)0, mtmp, AD_PHYS, ADFLAGS_NONE));

                    if (DEADMONSTER(mtmp))
                        killed(mtmp);
                }
                else if (ttype == WEB) 
                {
                    if (!webmaker(youmonst.data))
                    {
                        struct trap *ttmp2 = maketrap(u.ux, u.uy, WEB, NON_PM, MKTRAP_NO_FLAGS);

                        if (ttmp2)
                        {
                            pline_The(
                                "webbing sticks to you.  You're caught too!");
                            dotrap(ttmp2, NOWEBMSG);
                            if (u.usteed && u.utrap) 
                            {
                                /* you, not steed, are trapped */
                                dismount_steed(DISMOUNT_FELL);
                            }
                        }
                    } 
                    else
                        pline("%s remains entangled.", Monnam(mtmp));
                }
            } 
            else if (under_u)
            {
                /* [don't need the iflags.failing_untrap hack here] */
                dotrap(ttmp, FAILEDUNTRAP);
            }
            else 
            {
                move_into_trap(ttmp);
            }
        } 
        else
        {
            pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s is difficult to %s.",
                  ttmp->madeby_u ? "Your" : under_u ? "This" : "That",
                  trapdesc,
                  (ttype == WEB) ? "remove" : "disarm");
        }
        return 1;
    }
    return 2;
}

STATIC_OVL void
reward_untrap(ttmp, mtmp)
struct trap *ttmp;
struct monst *mtmp;
{
    if (!ttmp->madeby_u) 
    {
        if (rnl(10) < 8 && !is_peaceful(mtmp) && mon_can_move(mtmp) && !mindless(mtmp->data)
            && mtmp->data->mlet != S_HUMAN)
        {
            mtmp->mpeaceful = 1;
            set_mhostility(mtmp); /* reset alignment */
            newsym(mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s is grateful.", Monnam(mtmp));
        }

        /* Helping someone out of a trap is a nice thing to do,
         * A lawful may be rewarded, but not too often.  */
        if (!rn2(3) && !rnl(8) && u.ualign.type == A_LAWFUL)
        {
            adjalign(1);
            You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "that you did the right thing.");
        }
    }
}

STATIC_OVL int
disarm_holdingtrap(ttmp) /* Helge Hafting */
struct trap *ttmp;
{
    struct monst *mtmp;
    int fails = try_disarm(ttmp, FALSE);
    if (fails < 2)
    {
        return fails;
    }

    /* ok, disarm it. */

    boolean madebyu = ttmp->madeby_u;
    uchar ttyp = ttmp->ttyp;
    play_sfx_sound(SFX_DISARM_TRAP_SUCCESS);

    /* untrap the monster, if any.
       There's no need for a cockatrice test, only the trap is touched */
    if ((mtmp = m_at(ttmp->tx, ttmp->ty)) != 0) 
    {
        mtmp->mtrapped = 0;
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "remove %s %s from %s.", the_your[ttmp->madeby_u ? 1 : 0],
            (ttmp->ttyp == BEAR_TRAP) ? "bear trap" : "webbing",
            mon_nam(mtmp));
        reward_untrap(ttmp, mtmp);
    }
    else 
    {
        if (ttmp->ttyp == BEAR_TRAP)
        {
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "disarm %s bear trap.", the_your[ttmp->madeby_u ? 1 : 0]);
            cnv_trap_obj(BEARTRAP, 1, ttmp, FALSE, FALSE);
        }
        else /* if (ttmp->ttyp == WEB) */
        {
            You_ex(ATR_NONE, CLR_MSG_SUCCESS, "succeed in removing %s web.", the_your[ttmp->madeby_u ? 1 : 0]);
            deltrap(ttmp);
        }
    }
    newsym(u.ux + u.dx, u.uy + u.dy);

    /* gain skill for untrap */
    if(!madebyu)
        use_skill(P_DISARM_TRAP, trap_type_definitions[ttyp].training);

    return 1;
}

STATIC_OVL int
disarm_magical_trap(ttmp) /* Janne Gustafsson */
struct trap* ttmp;
{
    int fails = try_disarm(ttmp, FALSE);
    if (fails < 2)
    {
        return fails;
    }

    boolean madebyu = ttmp->madeby_u;
    uchar ttyp = ttmp->ttyp;
    play_sfx_sound(SFX_DISARM_TRAP_SUCCESS);
    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "disarm %s %s.", the_your[ttmp->madeby_u], get_trap_explanation(ttmp));
    
    ///* Skills gained */
    //int skillgained = 5;
    //boolean ismagictrap = FALSE;

    //switch (ttmp->ttyp)
    //{
    //case POLY_TRAP:
    //    skillgained = 32;
    //    ismagictrap = TRUE;
    //    break;
    //case MAGIC_TRAP:
    //    skillgained = 28;
    //    ismagictrap = TRUE;
    //    break;
    //case LEVEL_TELEP:
    //    skillgained = 26;
    //    ismagictrap = TRUE;
    //    break;
    //case ANTI_MAGIC_TRAP:
    //    skillgained = 24;
    //    ismagictrap = TRUE;
    //    break;
    //case TELEP_TRAP:
    //    skillgained = 22;
    //    ismagictrap = TRUE;
    //    break;
    //case SLP_GAS_TRAP:
    //    skillgained = 20;
    //    ismagictrap = TRUE;
    //    break;
    //case RUST_TRAP:
    //    skillgained = 18;
    //    break;
    //case FIRE_TRAP:
    //    skillgained = 16;
    //    ismagictrap = TRUE;
    //    break;
    //case ROLLING_BOULDER_TRAP:
    //    skillgained = 14;
    //    break;
    //case ROCKTRAP:
    //    skillgained = 12;
    //    break;
    //default:
    //    break;
    //}
    if (!madebyu)
        use_skill(P_DISARM_TRAP, trap_type_definitions[ttyp].training);

    int skill_level = P_SKILL_LEVEL(P_DISARM_TRAP);
    int getitem_normal = (skill_level <= P_UNSKILLED ? !rn2(3) :
        skill_level == P_BASIC ? rn2(3) :
        skill_level == P_SKILLED ? rn2(6) : rn2(12));
    int getitem_magic = (skill_level <= P_UNSKILLED ? 0 :
        skill_level == P_BASIC ? !rn2(3) :
        skill_level == P_SKILLED ? rn2(3) : rn2(6));
    int getitem = trap_type_definitions[ttyp].tdifficulty == SIMPLE_MAGICAL_TRAP || trap_type_definitions[ttyp].tdifficulty == COMPLEX_MAGICAL_TRAP ? getitem_magic : getitem_normal;

    int genotyp = STRANGE_OBJECT;
    if (getitem)
    {
        /* Items gained */
        switch (ttmp->ttyp)
        {
        case POLY_TRAP:
            genotyp = WAN_POLYMORPH;
            break;
        case MAGIC_TRAP:
            Strcpy(debug_buf_1, "disarm_magical_trap");
            genotyp = random_objectid_from_class(WAND_CLASS, (struct monst*)0, 0UL);
            break;
        case LEVEL_TELEP:
            genotyp = CUBIC_GATE;
            break;
        case ANTI_MAGIC_TRAP:
            genotyp = POT_MAGIC_RESISTANCE;
            break;
        case TELEP_TRAP:
            genotyp = WAN_TELEPORTATION;
            break;
        case SLP_GAS_TRAP:
            genotyp = WAN_SLEEP;
            break;
        case RUST_TRAP:
            genotyp = POT_WATER;
            break;
        case FIRE_TRAP:
            genotyp = WAN_FIRE;
            break;
        case ROCKTRAP:
            genotyp = get_shooting_trap_object(ttmp);
            break;
        default:
            break;
        }
    }

    if (genotyp > STRANGE_OBJECT)
        cnv_trap_obj(genotyp, 1, ttmp, FALSE, TRUE);
    else
    {
        deltrap(ttmp);
        newsym(u.ux + u.dx, u.uy + u.dy);
    }

    return 1;
}

 
/*
 * Find the type of a trap in the table, knowing its name.
 */
/* OBSOLETE - JG */
const char*
get_trap_name(trapid)
int trapid;
{
    return trap_type_definitions[trapid].name;
}



STATIC_OVL int
disarm_landmine(ttmp) /* Helge Hafting */
struct trap *ttmp;
{
    int fails = try_disarm(ttmp, FALSE);
    boolean madebyu = ttmp->madeby_u;

    if (fails < 2)
    {
        if (fails == 1)
            play_sfx_sound(SFX_DISARM_TRAP_FAIL);
        return fails;
    }

    uchar ttyp = ttmp->ttyp;
    play_sfx_sound(SFX_DISARM_TRAP_SUCCESS);
    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "disarm %s land mine.", the_your[ttmp->madeby_u]);
    cnv_trap_obj(LAND_MINE, 1, ttmp, FALSE, FALSE);

    /* gain skill for untrap */
    if (!madebyu)
        use_skill(P_DISARM_TRAP, trap_type_definitions[ttyp].training);

    return 1;
}

/* getobj will filter down to cans of grease and known potions of oil */
#if 0
STATIC_VAR NEARDATA const char oil[] = { ALL_CLASSES, TOOL_CLASS, POTION_CLASS,
                                     0 };
#endif

/* it may not make much sense to use grease on floor boards, but so what? */
STATIC_OVL int
disarm_squeaky_board(ttmp)
struct trap *ttmp;
{
    int fails;
    boolean madebyu = ttmp->madeby_u;

#if 0
    struct obj* obj;
    boolean bad_tool;
    obj = getobj(oil, "untrap with", 0, "");
    if (!obj)
        return 0;

    bad_tool = (obj->cursed
                || ((obj->otyp != POT_OIL || obj->lamplit)
                    && (obj->otyp != CAN_OF_GREASE || !obj->charges)));

    fails = try_disarm(ttmp,  bad_tool);
    if (fails < 2)
        return fails;

    /* successfully used oil or grease to fix squeaky board */
    if (obj->otyp == CAN_OF_GREASE)
    {
        consume_obj_charge(obj, TRUE);
    } 
    else 
    {
        useup(obj); /* oil */
        makeknown(POT_OIL);
    }
#endif
    fails = try_disarm(ttmp, FALSE);
    if (fails < 2)
    {
        return fails;
    }

    uchar ttyp = ttmp->ttyp;
    play_sfx_sound(SFX_DISARM_TRAP_SUCCESS);
    You("repair the squeaky board."); /* no madeby_u */
    deltrap(ttmp);
    newsym(u.ux + u.dx, u.uy + u.dy);

    /* gain skill for untrap */
    if (!madebyu)
        use_skill(P_DISARM_TRAP, trap_type_definitions[ttyp].training);

    more_experienced(1, 0); // 5);
    newexplevel();
    return 1;
}

/* removes traps that shoot arrows, darts, etc. */
STATIC_OVL int
disarm_shooting_trap(ttmp, otyp)
struct trap *ttmp;
int otyp;
{
    int fails = try_disarm(ttmp, FALSE);
    boolean madebyu = ttmp->madeby_u;

    if (fails < 2)
    {
        return fails;
    }

    uchar ttyp = ttmp->ttyp;
    play_sfx_sound(SFX_DISARM_TRAP_SUCCESS);
    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "disarm %s %s.", the_your[ttmp->madeby_u], get_trap_explanation(ttmp));

    int skill_level = P_SKILL_LEVEL(P_DISARM_TRAP);
    int getitem = (skill_level <= P_UNSKILLED ? !rn2(3) :
        skill_level == P_BASIC ? rn2(3) :
        skill_level == P_SKILLED ? rn2(6) : rn2(12));

    if (getitem)
        cnv_trap_obj(otyp, 50 - rnl(50), ttmp, FALSE, TRUE);
    else
    {
        deltrap(ttmp);
        newsym(u.ux + u.dx, u.uy + u.dy);
    }

    /* gain skill for untrap */
    if (!madebyu)
        use_skill(P_DISARM_TRAP, trap_type_definitions[ttyp].training);

    return 1;
}

/* Is the weight too heavy?
 * Formula as in near_capacity() & check_capacity() */
STATIC_OVL int
try_lift(mtmp, ttmp, wt, stuff)
struct monst *mtmp;
struct trap *ttmp;
int wt;
boolean stuff;
{
    int wc = weight_cap();

    if (((wt * 2) / wc) >= HVY_ENCUMBER) 
    {
        play_sfx_sound(SFX_DISARM_TRAP_FAIL);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is %s for you to lift.", Monnam(mtmp),
              stuff ? "carrying too much" : "too heavy");
        if (!ttmp->madeby_u && !is_peaceful(mtmp) && mon_can_move(mtmp)
            && !mindless(mtmp->data) && mtmp->data->mlet != S_HUMAN
            && rnl(10) < 3) 
        {
            mtmp->mpeaceful = 1;
            set_mhostility(mtmp); /* reset alignment */
            newsym(mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s thinks it was nice of you to try.", Monnam(mtmp));
        }
        return 0;
    }
    return 1;
}

/* Help trapped monster (out of a (spiked) pit) */
STATIC_OVL int
help_monster_out(mtmp, ttmp)
struct monst *mtmp;
struct trap *ttmp;
{
    int wt;
    struct obj *otmp;
    int untrap_result;

    /*
     * This works when levitating too -- consistent with the ability
     * to hit monsters while levitating.
     *
     * Should perhaps check that our hero has arms/hands at the
     * moment.  Helping can also be done by engulfing...
     *
     * Test the monster first - monsters are displayed before traps.
     */
    if (!mtmp->mtrapped)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s isn't trapped.", Monnam(mtmp));
        return 0;
    }
    /* Do you have the necessary capacity to lift anything? */
    if (check_capacity((char *) 0))
        return 1;

    /* Will our hero succeed? */
    untrap_result = succeed_untrap(ttmp->ttyp);
    if (untrap_result > 0 && mon_can_move(mtmp) && !is_tame(mtmp) && !is_peaceful(mtmp) && rn2(3))
    {
        play_sfx_sound(SFX_MONSTER_DOES_NOT_ALLOW);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "try to reach out your %s, but %s backs away skeptically.",
            makeplural(body_part(ARM)), mon_nam(mtmp));
        return 1;
    }

    /* is it a cockatrice?... */
    if (touch_petrifies(mtmp->data) && !uarmg && !Stone_resistance)
    {
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "grab the trapped %s using your bare %s.", mon_monster_name(mtmp),
            makeplural(body_part(HAND)));

        if (poly_when_stoned(youmonst.data) && polymon(PM_STONE_GOLEM)) 
        {
            display_nhwindow(WIN_MESSAGE, FALSE);
        }
        else
        {
            char kbuf[BUFSZ];

            Sprintf(kbuf, "trying to help %s out of a pit",
                    an(mon_monster_name(mtmp)));
            killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE;
            instapetrify(kbuf);
            return 1;
        }
    }

    /* need to do cockatrice check first if sleeping or paralyzed */
    if (untrap_result <= 0)
    {
        play_sfx_sound(SFX_DISARM_TRAP_FAIL);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "try to grab %s, but cannot get a firm grasp.", mon_nam(mtmp));
        if (mtmp->msleeping)
        {
            mtmp->msleeping = 0;
            refresh_m_tile_gui_info(mtmp, TRUE);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s awakens.", Monnam(mtmp));
        }
        return 1;
    }

    You("reach out your %s and grab %s.", makeplural(body_part(ARM)),
        mon_nam(mtmp));

    if (mtmp->msleeping) 
    {
        mtmp->msleeping = 0;
        refresh_m_tile_gui_info(mtmp, TRUE);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s awakens.", Monnam(mtmp));
    }
    else if (mtmp->mfrozen && !rn2(mtmp->mfrozen))
    {
        /* After such manhandling, perhaps the effect wears off */
        mtmp->mcanmove = 1;
        mtmp->mfrozen = 0;
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s stirs.", Monnam(mtmp));
    }

    /* is the monster too heavy? */
    wt = inv_weight() + mtmp->data->cwt;
    if (!try_lift(mtmp, ttmp, wt, FALSE))
        return 1;

    /* is the monster with inventory too heavy? */
    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
        wt += otmp->owt;
    if (!try_lift(mtmp, ttmp, wt, TRUE))
        return 1;

    play_sfx_sound(SFX_DISARM_TRAP_SUCCESS);
    You_ex(ATR_NONE, CLR_MSG_SUCCESS, "pull %s out of the pit.", mon_nam(mtmp));
    mtmp->mtrapped = 0;
    fill_pit(mtmp->mx, mtmp->my);
    reward_untrap(ttmp, mtmp);
    return 1;
}

int
untrap(force)
boolean force;
{
    register struct obj *otmp;
    register int x, y;
    int ch;
    struct trap *ttmp;
    struct monst *mtmp;
    const char *trapdescr;
    boolean here, useplural, deal_with_floor_trap,
            confused = (Confusion || Hallucination),
            trap_skipped = FALSE;
    int boxcnt = 0;
    char the_trap[BUFSZ], qbuf[QBUFSZ];

    if (!getdir((char *) 0))
        return 0;

    update_u_facing(TRUE);

    x = u.ux + u.dx;
    y = u.uy + u.dy;

    if (!isok(x, y)) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_The_ex(ATR_NONE, CLR_MSG_FAIL, "perils lurking there are beyond your grasp.");
        return 0;
    }

    /* 'force' is true for #invoke; make it be true for #untrap if
       carrying MKoT */
    if (!force && has_magic_key(&youmonst))
        force = TRUE;

    ttmp = t_at(x, y);
    if (ttmp && !ttmp->tseen)
        ttmp = 0;
    trapdescr = ttmp ? get_trap_explanation(ttmp) : 0;
    here = (x == u.ux && y == u.uy); /* !u.dx && !u.dy */

    if (here) /* are there are one or more containers here? */
        for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
            if (Is_box(otmp)) 
            {
                if (++boxcnt > 1)
                    break;
            }

    deal_with_floor_trap = can_reach_floor(FALSE);

    if (!deal_with_floor_trap)
    {
        *the_trap = '\0';
        if (ttmp)
            Strcat(the_trap, an(trapdescr));
        if (ttmp && boxcnt)
            Strcat(the_trap, " and ");
        if (boxcnt)
            Strcat(the_trap, (boxcnt == 1) ? "a container" : "containers");
        useplural = ((ttmp && boxcnt > 0) || boxcnt > 1);

        /* note: boxcnt and useplural will always be 0 for !here case */
        if (ttmp || boxcnt)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
            There_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s %s but you can't reach %s%s.",
                useplural ? "are" : "is", the_trap, here ? "here" : "there",
                useplural ? "them" : "it",
                u.usteed ? " while mounted" : "");
        }
        trap_skipped = (ttmp != 0);
    }
    else 
    { /* deal_with_floor_trap */

        if (ttmp) 
        {
            Strcpy(the_trap, the(trapdescr));
            if (boxcnt) 
            {
                if (is_pit(ttmp->ttyp)) 
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "do much about %s%s.", the_trap,
                             u.utrap ? " that you're stuck in"
                                     : " while standing on the edge of it");
                    trap_skipped = TRUE;
                    deal_with_floor_trap = FALSE;
                } 
                else
                {
                    Sprintf(
                        qbuf, "There %s and %s here.  %s %s?",
                        (boxcnt == 1) ? "is a container" : "are containers",
                        an(trapdescr),
                        (ttmp->ttyp == WEB) ? "Remove" : "Disarm", the_trap);

                    switch (ynq(qbuf)) 
                    {
                    case 'q':
                        return 0;
                    case 'n':
                        trap_skipped = TRUE;
                        deal_with_floor_trap = FALSE;
                        break;
                    }
                }
            }

            if (deal_with_floor_trap)
            {
                if (u.utrap) 
                {
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot deal with %s while trapped%s!", the_trap,
                        (x == u.ux && y == u.uy) ? " in it" : "");
                    return 1;
                }

                if ((mtmp = m_at(x, y)) != 0
                    && (M_AP_TYPE(mtmp) == M_AP_FURNITURE
                        || M_AP_TYPE(mtmp) == M_AP_OBJECT)) 
                {
                    stumble_onto_mimic(mtmp);
                    return 1;
                }

                boolean trap_to_help_out = is_pit(ttmp->ttyp);
                if (mtmp && !here && trap_to_help_out)
                {
                    return help_monster_out(mtmp, ttmp);
                }
                else if (trap_type_definitions[ttmp->ttyp].tdflags & TRAPDEF_FLAGS_NOT_DISARMABLE)
                {
                    play_sfx_sound(SFX_CANNOT_DISARM_TRAP);
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot disarm %s.", an(trap_type_definitions[ttmp->ttyp].name));
                    return 0;
                }
                else
                {
                    switch (ttmp->ttyp)
                    {
                    case BEAR_TRAP:
                    case WEB:
                        return disarm_holdingtrap(ttmp);
                    case LANDMINE:
                        return disarm_landmine(ttmp);
                    case SQKY_BOARD:
                        return disarm_squeaky_board(ttmp);
                    case DART_TRAP:
                        return disarm_shooting_trap(ttmp, get_shooting_trap_object(ttmp));
                    case ARROW_TRAP:
                        return disarm_shooting_trap(ttmp, get_shooting_trap_object(ttmp));
                    case ROCKTRAP:
                        return disarm_shooting_trap(ttmp, get_shooting_trap_object(ttmp));
                    case ROLLING_BOULDER_TRAP:
                    case ANTI_MAGIC_TRAP:
                    case POLY_TRAP:
                    case TELEP_TRAP:
                    case LEVEL_TELEP:
                    case RUST_TRAP:
                    case FIRE_TRAP:
                    case SLP_GAS_TRAP:
                    case MAGIC_TRAP:
                        return disarm_magical_trap(ttmp);
                    case PIT:
                    case SPIKED_PIT:
                        if (here)
                        {
                            play_sfx_sound(SFX_CANNOT_DISARM_TRAP);
                            You_ex(ATR_NONE, CLR_MSG_FAIL, "are already on the edge of the pit.");
                            return 0;
                        }

                        if (!mtmp)
                        {
                            play_sfx_sound(SFX_CANNOT_DISARM_TRAP);
                            pline_ex(ATR_NONE, CLR_MSG_FAIL, "Try filling the pit instead.");
                            return 0;
                        }

                        return help_monster_out(mtmp, ttmp);
                    default:
                        play_sfx_sound(SFX_CANNOT_DISARM_TRAP);
                        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot disable %s trap.", !here ? "that" : "this");
                        return 0;
                    }
                }
            }
        } /* end if */

        if (boxcnt)
        {
            for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
                if (Is_box(otmp)) 
                {
                    (void) safe_qbuf(qbuf, "There is ",
                        otmp->tknown && otmp->otrapped ? " here.  Investigate the trap further?" : " here.  Check it for traps?", otmp,
                                     doname, ansimpleoname, "a box");

                    switch (ynq(qbuf)) 
                    {
                    case 'q':
                        nomul(0);
                        return 0;
                    case 'n':
                        continue;
                    }

                    int res = check_box_trap(otmp, force);

                    if (res == 1)
                        return 1;
                    else if (res == 2)
                        break; /* Stops the whole process, no turn taken */
                    else
                    {
                        trap_skipped = TRUE;
                        continue;
                    }
                }

            You(trap_skipped ? "find no other traps here."
                             : "know of no traps here.");
            return 0;
        }

        if (stumble_on_door_mimic(x, y))
            return 1;

    } /* deal_with_floor_trap */

      
    /* doors can be manipulated even while levitating/unskilled riding */
    play_simple_player_sound(MONSTER_SOUND_TYPE_SEARCH);
    if (iflags.using_gui_sounds)
        delay_output_milliseconds(10 * ANIMATION_FRAME_INTERVAL);

    if (!IS_DOOR(levl[x][y].typ)) 
    {
        if (!trap_skipped)
        {
            play_sfx_sound(SFX_NO_TRAPS_FOUND);
            You("know of no traps there.");
        }
        return 0;
    }

    switch ((levl[x][y].doormask & D_MASK)) 
    {
    case D_NODOOR:
    case D_PORTCULLIS:
        play_sfx_sound(SFX_CANNOT_DISARM_TRAP);
        You("%s no door there.", Blind ? "feel" : "see");
        return 0;
    case D_ISOPEN:
        play_sfx_sound(SFX_CANNOT_DISARM_TRAP);
        pline("This door is safely open.");
        return 0;
    case D_BROKEN:
        play_sfx_sound(SFX_CANNOT_DISARM_TRAP);
        pline("This door is broken.");
        return 0;
    }

    if ((((levl[x][y].doormask & D_TRAPPED) & D_MASK) != 0
         && (force || (!confused && rn2(MAXULEV - u.ulevel + 11) < 10)))
        || (!force && confused && !rn2(3)))
    {
        play_sfx_sound(SFX_TRAP_FOUND);
        You_ex(ATR_NONE, CLR_MSG_WARNING, "find a trap on the door!");
        exercise(A_WIS, TRUE);

        if (yn_query_ex(ATR_NONE, CLR_MSG_WARNING, "Trap Found", "Disarm it?") != 'y')
            return 1;

        if (levl[x][y].doormask & D_TRAPPED) 
        {
            ch = succeed_untrap(ARROW_TRAP);  /*untrap_probability(ARROW_TRAP, P_SKILL_LEVEL(P_DISARM_TRAP), &youmonst); */ // ACURR(A_DEX) + (Role_if(PM_ROGUE) ? u.ulevel * 3 : u.ulevel);
            //exercise(A_DEX, TRUE);
            if (!force && ch < 0) 
            {
                play_sfx_sound(SFX_DISARM_TRAP_FAIL);
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "set it off!");
                b_trapped(get_short_door_name_at(x, y), FINGER, x, y);
                levl[x][y].doormask &= ~D_TRAPPED;
                if (is_door_destroyed_by_booby_trap_at(x, y))
                {
                    levl[x][y].doormask &= ~D_MASK;
                    levl[x][y].doormask |= D_NODOOR;
                    levl[x][y].subtyp = 0;
                    unblock_vision_and_hearing_at_point(x, y);
                }
                newsym(x, y);
                /* (probably ought to charge for this damage...) */
                if (*in_rooms(x, y, SHOPBASE))
                    add_damage(x, y, 0L);
            }
            else if (!force && !ch)
            {
                play_sfx_sound(SFX_DISARM_TRAP_FAIL);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "fail to disarm it!");
            }
            else 
            {
                play_sfx_sound(SFX_DISARM_TRAP_SUCCESS);
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "disarm it!");
                levl[x][y].doormask &= ~D_TRAPPED;
                use_skill(P_DISARM_TRAP, trap_type_definitions[ARROW_TRAP].training);
            }
        } 
        else
        {
            play_sfx_sound(SFX_NO_TRAPS_FOUND);
            pline("This door was not trapped.");
        }
        return 1;
    } 
    else 
    {
        play_sfx_sound(SFX_NO_TRAPS_FOUND);
        You("find no traps on the door.");
        return 1;
    }
}

int
check_all_box_traps(force)
boolean force;
{
    struct obj* otmp;
    int boxcnt = 0;
    boolean deal_with_floor_trap = can_reach_floor(FALSE);
    boolean useplural = FALSE;
    char the_trap[BUFSIZ], qbuf[BUFSIZ];

    for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
        if (Is_box(otmp))
        {
            if (++boxcnt > 1)
                break;
        }

    if (!deal_with_floor_trap)
    {
        *the_trap = '\0';
        if (boxcnt)
            Strcat(the_trap, (boxcnt == 1) ? "a container" : "containers");

        useplural = (boxcnt > 1);

        /* note: boxcnt and useplural will always be 0 for !here case */
        if (boxcnt)
        {
            play_sfx_sound(SFX_GENERAL_CANNOT_REACH);
            There_ex(ATR_NONE, CLR_MSG_FAIL, "%s %s %s but you can't reach %s%s.",
                useplural ? "are" : "is", the_trap, "here",
                useplural ? "them" : "it",
                u.usteed ? " while mounted" : "");
        }
    }
    else
    { /* deal_with_floor_trap */
        if (boxcnt)
        {
            for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp->nexthere)
                if (Is_box(otmp) && !(otmp->tknown && !otmp->otrapped))
                {
                    (void)safe_qbuf(qbuf, "There is ",
                        otmp->tknown && otmp->otrapped ? " here.  Investigate the trap further?" : " here.  Check it for traps?", otmp,
                        doname, ansimpleoname, "a box");

                    switch (ynq(qbuf))
                    {
                    case 'q':
                        nomul(0);
                        return 0;
                    case 'n':
                        continue;
                    }

                    int res = check_box_trap(otmp, force);

                    if (res == 1)
                        return 1; /* Takes a turn */
                    else if (res == 2)
                        return 0; /* Stops the whole process, no turn taken */
                    else
                    {
                        continue;
                    }
                }

            return 0;
        }
    }

    return 0;
}

/* Return values 0 = do not take a turn, 1 = take turn, 2 = do not take a turn and stop checking other traps (q pressed) */
int
check_box_trap(otmp, force)
struct obj* otmp;
boolean force;
{
    boolean confused = (Confusion || Hallucination);
    int ch = 0;

    if (!otmp->tknown)
    {
        play_simple_player_sound(MONSTER_SOUND_TYPE_SEARCH);
        if (iflags.using_gui_sounds)
            delay_output_milliseconds(10 * ANIMATION_FRAME_INTERVAL);
    }

    if ((otmp->otrapped
        && (force || otmp->tknown || (!confused
            && rn2(MAXULEV + 1 - u.ulevel) < (MAXULEV / 3))))
        || (!force && confused && !rn2(3)))
    {
        play_sfx_sound(SFX_TRAP_FOUND);
        if (otmp->tknown)
        {
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "You confirm that there is an elaborately constructed trap on %s.", the(xname(otmp)));
        }
        else
        {
            You_ex(ATR_NONE, CLR_MSG_WARNING, "find a trap on %s!", the(xname(otmp)));
            otmp->tknown = 1;
        }
        if (!confused)
            exercise(A_WIS, TRUE);

        switch (dnq("Disarm or neglect it?"))
        {
        case 'q':
            return 2;
        case 'n':
            return 0;
        }

        if (otmp->otrapped)
        {
            ch = succeed_untrap(SLP_GAS_TRAP);
            //ch = untrap_probability(SLP_GAS_TRAP, P_SKILL_LEVEL(P_DISARM_TRAP), &youmonst); // ACURR(A_DEX) + (Role_if(PM_ROGUE) ? u.ulevel * 3 : u.ulevel);
            //exercise(A_DEX, TRUE);

            //if (Role_if(PM_ROGUE))
            //    ch *= 2;

            if (!force && ch < 0) // && (confused || Fumbling || rnd(100) > ch))
            {
                play_sfx_sound(SFX_DISARM_TRAP_FAIL);
                (void)chest_trap(otmp, FINGER, TRUE);
            }
            else if (!force && !ch)
            {
                play_sfx_sound(SFX_DISARM_TRAP_FAIL);
                You_ex(ATR_NONE, CLR_MSG_FAIL, "fail to disarm it!");
            }
            else
            {
                play_sfx_sound(SFX_DISARM_TRAP_SUCCESS);
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "disarm it!");
                otmp->otrapped = 0;
                use_skill(P_DISARM_TRAP, trap_type_definitions[SLP_GAS_TRAP].training);
            }
        }
        else
            pline("That %s was not trapped.", xname(otmp));

        return 1;
    }
    else
    {
        play_sfx_sound(SFX_NO_TRAPS_FOUND);
        You("find no traps on %s.", the(xname(otmp)));
        return 1;
    }

    return 0;
}

/* for magic unlocking; returns true if targetted monster (which might
   be hero) gets untrapped; the trap remains intact */
boolean
openholdingtrap(mon, noticed)
struct monst *mon;
boolean *noticed; /* set to true iff hero notices the effect; */
{                 /* otherwise left with its previous value intact */
    struct trap *t;
    char buf[BUFSZ], whichbuf[20];
    const char *trapdescr = 0, *which = 0;
    boolean ishero = (mon == &youmonst);

    if (!mon)
        return FALSE;
    if (mon == u.usteed)
        ishero = TRUE;

    t = t_at(ishero ? u.ux : mon->mx, ishero ? u.uy : mon->my);

    if (ishero && u.utrap)
    { /* all u.utraptype values are holding traps */
        which = "";
        switch (u.utraptype) 
        {
        case TT_LAVA:
            trapdescr = "molten lava";
            break;
        case TT_INFLOOR:
            /* solidified lava, so not "floor" even if within a room */
            trapdescr = "ground";
            break;
        case TT_BURIEDBALL:
            trapdescr = "your anchor";
            break;
        case TT_BEARTRAP:
        case TT_PIT:
        case TT_WEB:
            trapdescr = 0; /* use defsyms[].explanation */
            break;
        default:
            /* lint suppression in case 't' is unexpectedly Null
               or u.utraptype has new value we don't know about yet */
            trapdescr = "trap";
            break;
        }
    }
    else
    {
        /* if no trap here or it's not a holding trap, we're done */
        if (!t || (t->ttyp != BEAR_TRAP && t->ttyp != WEB))
            return FALSE;
    }

    if (!trapdescr)
        trapdescr = get_trap_explanation(t);
    if (!which)
        which = t->tseen ? the_your[t->madeby_u ? 1 : 0]
                         : index(vowels, *trapdescr) ? "an" : "a";
    if (*which)
        which = strcat(strcpy(whichbuf, which), " ");

    if (ishero) 
    {
        if (!u.utrap)
            return FALSE;
        *noticed = TRUE;
        if (u.usteed)
            Sprintf(buf, "%s is", noit_Monnam(u.usteed));
        else
            Strcpy(buf, "You are");

        play_sfx_sound(SFX_WRIGGLE_FREE);
        pline("%s released from %s%s.", buf, which, trapdescr);
        reset_utrap(TRUE);
    } 
    else 
    {
        if (!mon->mtrapped)
            return FALSE;

        mon->mtrapped = 0;
        if (canspotmon(mon)) 
        {
            *noticed = TRUE;
            if (t->ttyp == BEAR_TRAP)
            {
                context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
                struct layer_info layers = layers_at(t->tx, t->ty);
                int old_glyph = layers.layer_glyphs[LAYER_TRAP];
                int old_gui_glyph = layers.layer_gui_glyphs[LAYER_TRAP];
                int old_cover_glyph = layers.layer_glyphs[LAYER_COVER_TRAP];
                int old_cover_gui_glyph = layers.layer_gui_glyphs[LAYER_COVER_TRAP];
                show_glyph_on_layer(t->tx, t->ty, NO_GLYPH, LAYER_TRAP);
                show_glyph_on_layer(t->tx, t->ty, NO_GLYPH, LAYER_COVER_TRAP);
                flush_screen(1);
                play_special_effect_at(SPECIAL_EFFECT_BEAR_TRAP_OPEN, 0, t->tx, t->ty, FALSE);
                play_sfx_sound_at_location(SFX_BEAR_TRAP_SET_UP, t->tx, t->ty);
                special_effect_wait_until_action(0);
                pline("%s is released from %s%s.", Monnam(mon), which,
                    trapdescr);
                special_effect_wait_until_end(0);
                show_gui_glyph_on_layer(t->tx, t->ty, old_glyph, old_gui_glyph, LAYER_TRAP);
                show_gui_glyph_on_layer(t->tx, t->ty, old_cover_glyph, old_cover_gui_glyph, LAYER_COVER_TRAP);
                flush_screen(1);
                context.global_newsym_flags = 0UL;
            }
            else
                pline("%s is released from %s%s.", Monnam(mon), which,
                    trapdescr);
        }
        else if (cansee(t->tx, t->ty) && t->tseen) 
        {
            *noticed = TRUE;
            if (t->ttyp == WEB)
                pline("%s is released from %s%s.", Something, which,
                      trapdescr);
            else /* BEAR_TRAP */
            {
                context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
                struct layer_info layers = layers_at(t->tx, t->ty);
                int old_glyph = layers.layer_glyphs[LAYER_TRAP];
                int old_gui_glyph = layers.layer_gui_glyphs[LAYER_TRAP];
                int old_cover_glyph = layers.layer_glyphs[LAYER_COVER_TRAP];
                int old_cover_gui_glyph = layers.layer_gui_glyphs[LAYER_COVER_TRAP];
                show_glyph_on_layer(t->tx, t->ty, NO_GLYPH, LAYER_TRAP);
                show_glyph_on_layer(t->tx, t->ty, NO_GLYPH, LAYER_COVER_TRAP);
                flush_screen(1);
                play_special_effect_at(SPECIAL_EFFECT_BEAR_TRAP_OPEN, 0, t->tx, t->ty, FALSE);
                play_sfx_sound_at_location(SFX_BEAR_TRAP_SET_UP, t->tx, t->ty);
                special_effect_wait_until_action(0);
                pline("%s%s opens.", upstart(strcpy(buf, which)), trapdescr);
                special_effect_wait_until_end(0);
                show_gui_glyph_on_layer(t->tx, t->ty, old_glyph, old_gui_glyph, LAYER_TRAP);
                show_gui_glyph_on_layer(t->tx, t->ty, old_cover_glyph, old_cover_gui_glyph, LAYER_COVER_TRAP);
                flush_screen(1);
                context.global_newsym_flags = 0UL;
            }
        }
        newsym(t->tx, t->ty);
        /* might pacify monster if adjacent */
        if (rn2(2) && distu(mon->mx, mon->my) <= 2)
            reward_untrap(t, mon);
    }
    return TRUE;
}

/* for magic locking; returns true if targetted monster (which might
   be hero) gets hit by a trap (might avoid actually becoming trapped) */
boolean
closeholdingtrap(mon, noticed)
struct monst *mon;
boolean *noticed; /* set to true iff hero notices the effect; */
{                 /* otherwise left with its previous value intact */
    struct trap *t;
    unsigned short dotrapflags;
    boolean ishero = (mon == &youmonst), result;

    if (!mon)
        return FALSE;
    if (mon == u.usteed)
        ishero = TRUE;
    t = t_at(ishero ? u.ux : mon->mx, ishero ? u.uy : mon->my);
    /* if no trap here or it's not a holding trap, we're done */
    if (!t || (t->ttyp != BEAR_TRAP && t->ttyp != WEB))
        return FALSE;

    if (ishero) 
    {
        if (u.utrap)
            return FALSE; /* already trapped */
        *noticed = TRUE;
        dotrapflags = FORCETRAP;
        /* dotrap calls mintrap when mounted hero encounters a web */
        if (u.usteed)
            dotrapflags |= NOWEBMSG;
        ++force_mintrap;
        dotrap(t, dotrapflags);
        --force_mintrap;
        result = (u.utrap != 0);
    }
    else 
    {
        if (mon->mtrapped)
            return FALSE; /* already trapped */
        /* you notice it if you see the trap close/tremble/whatever
           or if you sense the monster who becomes trapped */
        *noticed = cansee(t->tx, t->ty) || canspotmon(mon);
        ++force_mintrap;
        result = (mintrap(mon) != 0);
        --force_mintrap;
    }
    return result;
}

/* for magic unlocking; returns true if targetted monster (which might
   be hero) gets hit by a trap (target might avoid its effect) */
boolean
openfallingtrap(mon, trapdoor_only, noticed)
struct monst *mon;
boolean trapdoor_only;
boolean *noticed; /* set to true iff hero notices the effect; */
{                 /* otherwise left with its previous value intact */
    struct trap *t;
    boolean ishero = (mon == &youmonst), result;

    if (!mon)
        return FALSE;
    if (mon == u.usteed)
        ishero = TRUE;
    t = t_at(ishero ? u.ux : mon->mx, ishero ? u.uy : mon->my);
    /* if no trap here or it's not a falling trap, we're done
       (note: falling rock traps have a trapdoor in the ceiling) */
    if (!t || ((t->ttyp != TRAPDOOR && t->ttyp != ROCKTRAP)
               && (trapdoor_only || (t->ttyp != HOLE && !is_pit(t->ttyp)))))
        return FALSE;

    if (ishero)
    {
        if (u.utrap)
            return FALSE; /* already trapped */
        *noticed = TRUE;
        dotrap(t, FORCETRAP);
        result = (u.utrap != 0);
    }
    else
    {
        if (mon->mtrapped)
            return FALSE; /* already trapped */
        /* you notice it if you see the trap close/tremble/whatever
           or if you sense the monster who becomes trapped */
        *noticed = cansee(t->tx, t->ty) || canspotmon(mon);
        /* monster will be angered; mintrap doesn't handle that */
        wakeup(mon, TRUE);
        ++force_mintrap;
        result = (mintrap(mon) != 0);
        --force_mintrap;
        /* mon might now be on the migrating monsters list */
    }
    return result;
}

/* only called when the player is doing something to the chest directly */
boolean
chest_trap(obj, bodypart, disarm)
register struct obj *obj;
register int bodypart;
boolean disarm;
{
    register struct obj *otmp = obj, *otmp2;
    char buf[80];
    const char *msg;
    enum sfx_sound_types sfx = 0;
    enum special_effect_types spef = MAX_SPECIAL_EFFECTS;
    coord cc;

    if (get_obj_location(obj, &cc.x, &cc.y, 0)) /* might be carried */
        obj->ox = cc.x, obj->oy = cc.y;

    otmp->tknown = 1; /* You found it by triggering it! */
    otmp->otrapped = 0; /* trap is one-shot; clear flag first in case
                           chest kills you and ends up in bones file */
    play_sfx_sound(SFX_CHEST_TRAP_TRIGGER);
    You_ex(ATR_NONE, CLR_MSG_WARNING, disarm ? "set it off!" : "trigger a trap!");
    if (iflags.using_gui_sounds)
        delay_output_milliseconds(100);

    display_nhwindow(WIN_MESSAGE, FALSE);

    if (Luck > -13 && rn2(13 + Luck) > 7) 
    { /* saved by luck */
        /* trap went off, but good luck prevents damage */
        switch (rn2(13))
        {
        case 12:
        case 11:
            msg = "explosive charge is a dud";
            sfx = 0;
            spef = SPECIAL_EFFECT_GENERAL_EXCLAMATION;
            break;
        case 10:
        case 9:
            msg = "electric charge is grounded";
            sfx = SFX_ELECTRICITY_CRACKLES;
            spef = SPECIAL_EFFECT_GENERAL_EXCLAMATION;
            break;
        case 8:
        case 7:
            msg = "flame fizzles out";
            sfx = SFX_TRAP_FLAME_FIZZLES_OUT;
            spef = SPECIAL_EFFECT_GENERAL_EXCLAMATION;
            break;
        case 6:
        case 5:
        case 4:
            msg = "poisoned needle misses";
            sfx = SFX_TRAP_NEEDLE_MISSES;
            spef = SPECIAL_EFFECT_TRAP_NEEDLE;
            break;
        case 3:
        case 2:
        case 1:
        case 0:
            msg = "gas cloud blows away";
            sfx = SFX_TRAP_CLOUD_BLOWS_AWAY;
            spef = SPECIAL_EFFECT_TRAP_SLEEP_GAS;
            break;
        default:
            impossible("chest disarm bug");
            msg = (char *) 0;
            break;
        }
        if (msg)
        {
            if(spef < MAX_SPECIAL_EFFECTS)
                play_special_effect_at(spef, 0, obj->ox, obj->oy, FALSE);
            if(sfx > 0)
                play_sfx_sound_at_location(sfx, obj->ox, obj->oy);
            if (spef < MAX_SPECIAL_EFFECTS)
                special_effect_wait_until_action(0);
            pline("But luckily the %s!", msg);
            if (spef < MAX_SPECIAL_EFFECTS)
                special_effect_wait_until_end(0);

        }
    }
    else
    {
        switch (rn2(20) ? ((Luck >= 13) ? 0 : rn2(13 - Luck)) : rn2(26))
        {
        case 25:
        case 24:
        case 23:
        case 22:
        case 21:
        {
            struct monst *shkp = 0;
            long loss = 0L;
            boolean costly, insider;
            register xchar ox = obj->ox, oy = obj->oy;

            /* the obj location need not be that of player */
            costly = (costly_spot(ox, oy)
                      && (shkp = shop_keeper(*in_rooms(ox, oy, SHOPBASE)))
                             != (struct monst *) 0);
            insider = (*u.ushops && inside_shop(u.ux, u.uy)
                       && *in_rooms(ox, oy, SHOPBASE) == *u.ushops);

            play_special_effect_at(SPECIAL_EFFECT_SMALL_FIERY_EXPLOSION, 0, ox, oy, FALSE);
            play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, ox, oy);
            special_effect_wait_until_action(0);
            context.global_newsym_flags = NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS;
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s!", Tobjnam(obj, "explode"));
            Sprintf(buf, "exploding %s", xname(obj));

            if (costly)
                loss += stolen_value(obj, ox, oy, is_peaceful(shkp) != 0,
                                     TRUE);
            delete_contents(obj);
            /* unpunish() in advance if either ball or chain (or both)
               is going to be destroyed */
            if (Punished && ((uchain->ox == u.ux && uchain->oy == u.uy)
                             || (uball->where == OBJ_FLOOR
                                 && uball->ox == u.ux && uball->oy == u.uy)))
                unpunish();

            for (otmp = level.objects[u.ux][u.uy]; otmp; otmp = otmp2)
            {
                otmp2 = otmp->nexthere;
                if (costly)
                    loss += stolen_value(otmp, otmp->ox, otmp->oy,
                                         is_peaceful(shkp) != 0, TRUE);
                delobj(otmp);
            }
            wake_nearby();
            losehp(adjust_damage(d(6, 6), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), buf, KILLED_BY_AN);
            exercise(A_STR, FALSE);
            if (costly && loss)
            {
                if (insider)
                    You("owe %ld %s for objects destroyed.", loss,
                        currency(loss));
                else 
                {
                    You("caused %ld %s worth of damage!", loss,
                        currency(loss));
                    make_angry_shk(shkp, ox, oy);
                }
            }
            context.global_newsym_flags = 0UL;
            special_effect_wait_until_end(0);

            return TRUE;
        } /* case 21 */
        case 20:
        case 19:
        case 18:
        case 17:
            play_special_effect_at(SPECIAL_EFFECT_TRAP_SLEEP_GAS, 0, obj->ox, obj->oy, FALSE);
            play_sfx_sound(SFX_ENVELOPED_IN_CLOUD_OF_GAS);
            special_effect_wait_until_action(0);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A cloud of noxious gas billows from %s.", the(xname(obj)));
            poisoned("gas cloud", A_STR, "cloud of poison gas", 0, FALSE, 2);
            exercise(A_CON, FALSE);
            special_effect_wait_until_end(0);
            break;
        case 16:
        case 15:
        case 14:
        case 13:
            play_special_effect_at(SPECIAL_EFFECT_TRAP_NEEDLE, 0, obj->ox, obj->oy, FALSE);
            play_sfx_sound_at_location(SFX_TRAP_NEEDLE_PRICKS, obj->ox, obj->oy);
            special_effect_wait_until_action(0);
            You_feel_ex(ATR_NONE, CLR_MSG_NEGATIVE, "a needle prick your %s.", body_part(bodypart));
            if (iflags.using_gui_sounds)
            {
                delay_output_milliseconds(100);
                play_player_ouch_sound(MONSTER_OUCH_SOUND_OUCH);
            }
            display_u_being_hit(HIT_POISONED, 0, 0UL);
            poisoned("needle", A_CON, "poisoned needle", 0, FALSE, 2);

            exercise(A_CON, FALSE);
            special_effect_wait_until_end(0);
            break;
        case 12:
        case 11:
        case 10:
        case 9:
            play_special_effect_at(SPECIAL_EFFECT_TRAP_FIRE, 0, obj->ox, obj->oy, FALSE);
            play_sfx_sound_at_location(SFX_GENERIC_PHYSICAL_TRAP_ACTIVATE, obj->ox, obj->oy);
            special_effect_wait_until_action(0);
            dofiretrap(obj, 3);
            special_effect_wait_until_end(0);
            break;
        case 8:
        case 7:
        case 6:
        {
            int dmg;

            play_sfx_sound(SFX_ELECTRIC_SHOCK);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are jolted by a surge of electricity!");
            display_u_being_hit(HIT_ELECTROCUTED, 0, 0UL);
            if (Shock_immunity)
            {
                play_sfx_sound(SFX_GENERAL_UNAFFECTED);
                u_shieldeff();
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "don't seem to be affected.");
                dmg = 0;
            }
            else
            {
                dmg = d(4, 4);
            }
            double damage = adjust_damage(dmg, (struct monst*)0, &youmonst, AD_ELEC, ADFLAGS_NONE);

            destroy_item(RING_CLASS, AD_ELEC);
            destroy_item(WAND_CLASS, AD_ELEC);
            if (damage > 0.0)
            {
                int hp_before = Upolyd ? u.mh : u.uhp;
                losehp(damage, "electric shock", KILLED_BY_AN);
                int hp_after = Upolyd ? u.mh : u.uhp;
                display_u_being_hit(HIT_ELECTROCUTED, hp_before - hp_after, 0UL);
            }
            item_destruction_hint(AD_ELEC, FALSE);
            break;
        } /* case 6 */
        case 5:
        case 4:
        case 3:
            play_special_effect_at(SPECIAL_EFFECT_GENERAL_EXCLAMATION, 0, obj->ox, obj->oy, FALSE);
            special_effect_wait_until_action(0);
            if (!Free_action)
            {
                play_sfx_sound(SFX_CAUSE_PARALYSIS);
                incr_itimeout(&HParalyzed, d(5, 6));
                context.botl = context.botlx = 1;
                refresh_u_tile_gui_info(TRUE);
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Suddenly you are frozen in place!");
#if 0
                nomul(-d(5, 6));
                multi_reason = "frozen by a trap";
                exercise(A_DEX, FALSE);
#endif
                nomovemsg = You_can_move_again;
                nomovemsg_color = CLR_MSG_SUCCESS;
            }
            else
            {
                play_sfx_sound(SFX_GENERAL_UNAFFECTED);
                You_ex(ATR_NONE, CLR_MSG_SUCCESS, "momentarily stiffen.");
            }
            special_effect_wait_until_end(0);
            break;
        case 2:
        case 1:
        case 0:
            play_special_effect_at(SPECIAL_EFFECT_TRAP_SLEEP_GAS, 0, obj->ox, obj->oy, FALSE);
            play_sfx_sound(SFX_ENVELOPED_IN_CLOUD_OF_GAS);
            special_effect_wait_until_action(0);
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "A cloud of %s gas billows from %s.",
                  Blind ? blindgas[rn2(SIZE(blindgas))] : rndcolor(),
                  the(xname(obj)));
            if (!Stunned) {
                if (Hallucination)
                    pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "What a groovy feeling!");
                else
                    You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s%s...", stagger(youmonst.data, "stagger"),
                        Halluc_resistance ? ""
                                          : Blind ? " and get dizzy"
                                                  : " and your vision blurs");
            }
            play_sfx_sound(SFX_ACQUIRE_HALLUCINATION);
            make_stunned((HStun & TIMEOUT) + (long) rn1(7, 16), FALSE);
            (void) make_hallucinated(
                (HHallucination & TIMEOUT) + (long) rn1(5, 16), FALSE, 0L);
            special_effect_wait_until_end(0);
            break;
        default:
            impossible("bad chest trap");
            break;
        }
        bot(); /* to get immediate botl re-display */
    }

    return FALSE;
}

struct trap *
t_at(x, y)
register int x, y;
{
    register struct trap *trap = ftrap;

    while (trap) 
    {
        if (trap->tx == x && trap->ty == y)
            return trap;
        trap = trap->ntrap;
    }
    return (struct trap *) 0;
}

void
deltrap(trap)
register struct trap *trap;
{
    register struct trap *ttmp;

    clear_conjoined_pits(trap);
    if (trap == ftrap)
    {
        ftrap = ftrap->ntrap;
    } 
    else
    {
        for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap)
            if (ttmp->ntrap == trap)
                break;

        if (!ttmp)
        {
            panic("deltrap: no preceding trap!");
            return;
        }
        ttmp->ntrap = trap->ntrap;
    }

    if (Sokoban && (trap->ttyp == PIT || trap->ttyp == HOLE))
        maybe_finish_sokoban();

    dealloc_trap(trap);
}

boolean
conjoined_pits(trap2, trap1, u_entering_trap2)
struct trap *trap2, *trap1;
boolean u_entering_trap2;
{
    int dx, dy, diridx, adjidx;

    if (!trap1 || !trap2)
        return FALSE;
    if (!isok(trap2->tx, trap2->ty) || !isok(trap1->tx, trap1->ty)
        || !is_pit(trap2->ttyp)
        || !is_pit(trap1->ttyp)
        || (u_entering_trap2 && !(u.utrap && u.utraptype == TT_PIT)))
        return FALSE;
    dx = sgn(trap2->tx - trap1->tx);
    dy = sgn(trap2->ty - trap1->ty);
    for (diridx = 0; diridx < 8; diridx++)
        if (xdir[diridx] == dx && ydir[diridx] == dy)
            break;
    /* diridx is valid if < 8 */
    if (diridx < 8)
    {
        adjidx = (diridx + 4) % 8;
        if ((trap1->conjoined & (1 << diridx))
            && (trap2->conjoined & (1 << adjidx)))
            return TRUE;
    }
    return FALSE;
}

STATIC_OVL void
clear_conjoined_pits(trap)
struct trap *trap;
{
    int diridx, adjidx, x, y;
    struct trap *t;

    if (trap && is_pit(trap->ttyp))
    {
        for (diridx = 0; diridx < 8; ++diridx) 
        {
            if (trap->conjoined & (1 << diridx)) 
            {
                x = trap->tx + xdir[diridx];
                y = trap->ty + ydir[diridx];

                if (isok(x, y)
                    && (t = t_at(x, y)) != 0
                    && is_pit(t->ttyp))
                {
                    adjidx = (diridx + 4) % 8;
                    t->conjoined &= ~(1 << adjidx);
                }

                trap->conjoined &= ~(1 << diridx);
            }
        }
    }
}

STATIC_OVL boolean
adj_nonconjoined_pit(adjtrap)
struct trap *adjtrap;
{
    struct trap *trap_with_u = t_at(u.ux0, u.uy0);

    if (trap_with_u && adjtrap && u.utrap && u.utraptype == TT_PIT
        && is_pit(trap_with_u->ttyp) && is_pit(adjtrap->ttyp))
    {
        int idx;

        for (idx = 0; idx < 8; idx++)
        {
            if (xdir[idx] == u.dx && ydir[idx] == u.dy)
                return TRUE;
        }
    }
    return FALSE;
}

#if 0
/*
 * Mark all neighboring pits as conjoined pits.
 * (currently not called from anywhere)
 */
STATIC_OVL void
join_adjacent_pits(trap)
struct trap *trap;
{
    struct trap *t;
    int diridx, x, y;

    if (!trap)
        return;
    for (diridx = 0; diridx < 8; ++diridx) 
    {
        x = trap->tx + xdir[diridx];
        y = trap->ty + ydir[diridx];
        if (isok(x, y)) 
        {
            if ((t = t_at(x, y)) != 0 && is_pit(t->ttyp))
            {
                trap->conjoined |= (1 << diridx);
                join_adjacent_pits(t);
            } else
                trap->conjoined &= ~(1 << diridx);
        }
    }
}
#endif /*0*/

/*
 * Returns TRUE if you escaped a pit and are standing on the precipice.
 */
boolean
uteetering_at_seen_pit(trap)
struct trap *trap;
{
    if (trap && trap->tseen && (!u.utrap || u.utraptype != TT_PIT)
        && is_pit(trap->ttyp))
        return TRUE;
    else
        return FALSE;
}

/* Destroy a trap that emanates from the floor. */
boolean
delfloortrap(ttmp)
register struct trap *ttmp;
{
    /* some of these are arbitrary -dlc */
    if (ttmp && ((ttmp->ttyp == SQKY_BOARD) || (ttmp->ttyp == BEAR_TRAP)
                 || (ttmp->ttyp == LANDMINE) || (ttmp->ttyp == FIRE_TRAP)
                 || is_pit(ttmp->ttyp)
                 || is_hole(ttmp->ttyp)
                 || (ttmp->ttyp == TELEP_TRAP) || (ttmp->ttyp == LEVEL_TELEP)
                 || (ttmp->ttyp == WEB) || (ttmp->ttyp == MAGIC_TRAP)
                 || (ttmp->ttyp == ANTI_MAGIC_TRAP)))
    {
        register struct monst *mtmp;

        if (ttmp->tx == u.ux && ttmp->ty == u.uy)
        {
            if (u.utraptype != TT_BURIEDBALL)
                reset_utrap(TRUE);
        } 
        else if ((mtmp = m_at(ttmp->tx, ttmp->ty)) != 0)
        {
            mtmp->mtrapped = 0;
        }
        deltrap(ttmp);
        return TRUE;
    }
    return FALSE;
}

/* used for doors (also tins).  can be used for anything else that opens. */
void
b_trapped(item, bodypart, x, y)
const char *item;
int bodypart;
xchar x, y;
{
    int lvl = level_difficulty(),
        dmg = rnd(5 + (lvl < 5 ? lvl : 2 + lvl / 2));

    play_special_effect_at(SPECIAL_EFFECT_SMALL_FIERY_EXPLOSION, 0, x, y, FALSE);
    play_sfx_sound_at_location(SFX_EXPLOSION_FIERY, x, y);
    special_effect_wait_until_action(0);

    pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "KABOOM!!  %s was booby-trapped!", The(item));
    wake_nearby();
    losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE), "explosion", KILLED_BY_AN);
    exercise(A_STR, FALSE);
    if (bodypart)
        exercise(A_CON, FALSE);
    make_stunned((HStun & TIMEOUT) + (long) dmg, TRUE);
    special_effect_wait_until_end(0);
}

/* Monster is hit by trap. */
/* Note: doesn't work if both obj and d_override are null */
STATIC_OVL boolean
thitm(tlev, mon, obj, d_override, nocorpse)
int tlev;
struct monst *mon;
struct obj *obj;
int d_override;
boolean nocorpse;
{
    if (!mon)
        return FALSE;

    int strike;
    boolean trapkilled = FALSE;

    if (d_override)
        strike = 1;
    else if (obj)
        strike = (find_mac(mon) + tlev + obj->enchantment <= rnd(20));
    else
        strike = (find_mac(mon) + tlev <= rnd(20));

    /* Actually more accurate than thitu, which doesn't take
     * obj->enchantment into account.
     */
    if (!strike) 
    {
        if (obj && cansee(mon->mx, mon->my))
            pline("%s is almost hit by %s!", Monnam(mon), doname(obj));
    } 
    else 
    {
        int dam = 1;

        if (obj && cansee(mon->mx, mon->my))
            pline("%s is hit by %s!", Monnam(mon), doname(obj));
        
        if (d_override)
            dam = d_override;
        else if (obj) 
        {
            if (is_launcher(obj))
                dam = d(1, 2);
            else
                dam = weapon_total_dmg_value(obj, mon, (struct monst*)0, 1);

            if (dam < 1)
                dam = 1;
        }
        
        if(obj)
            play_object_hit_sound_at_location(obj, HIT_SURFACE_SOURCE_MONSTER, monst_to_any(mon), dam, HMON_THROWN, mon->mx, mon->my);

        int hp_before = mon->mhp;
        deduct_monster_hp(mon, adjust_damage(dam, (struct monst*)0, mon, obj ? objects[obj->otyp].oc_damagetype : AD_PHYS, ADFLAGS_NONE));
        int hp_after = mon->mhp;
        int damage_done = hp_before - hp_after;
        if (damage_done > 0 && canseemon(mon))
        {
            int multicolors[3] = { NO_COLOR, CLR_ORANGE, NO_COLOR };
            pline_multi_ex(ATR_NONE, NO_COLOR, no_multiattrs, multicolors, "%s sustains %d damage%s", Monnam(mon), damage_done, exclam(damage_done));
        }

        if (DEADMONSTER(mon)) 
        {
            int xx = mon->mx, yy = mon->my;

            monkilled(mon, "", obj ? objects[obj->otyp].oc_damagetype : AD_PHYS, nocorpse ? XKILL_NOCORPSE : 0);
            if (DEADMONSTER(mon)) 
            {
                newsym(xx, yy);
                trapkilled = TRUE;
            }
        }
    }

    if (obj && (!strike || d_override))
    {
        place_object(obj, mon->mx, mon->my);
        stackobj(obj);
    }
    else if (obj)
        obfree(obj, (struct obj*)0);

    return trapkilled;
}

boolean
unconscious()
{
    if (multi >= 0)
        return FALSE;

    return (boolean) (u.usleep
                      || (nomovemsg
                          && (!strncmp(nomovemsg, "You awake", 9)
                              || !strncmp(nomovemsg, "You regain con", 14)
                              || !strncmp(nomovemsg, "You are consci", 14))));
}

STATIC_VAR const char lava_killer[] = "molten lava";

boolean
lava_effects()
{
    register struct obj *obj, *obj2;
    int dmg = d(6, 6); /* only applicable for water walking */
    boolean usurvive, boil_away;

    burn_away_slime();
    if (likes_lava(youmonst.data))
        return FALSE;

    usurvive = Fire_immunity || (Walks_on_water && dmg < u.uhp);
    /*
     * A timely interrupt might manage to salvage your life
     * but not your gear.  For scrolls and potions this
     * will destroy whole stacks, where fire resistant hero
     * survivor only loses partial stacks via destroy_item().
     *
     * Flag items to be destroyed before any messages so
     * that player causing hangup at --More-- won't get an
     * emergency save file created before item destruction.
     */
    if (!usurvive)
        for (obj = invent; obj; obj = obj->nobj)
            if ((melts_in_lava(obj) || obj->oclass == POTION_CLASS)
                && !obj->oerodeproof
                && !oresist_fire(obj)
                ) /* for invocation items */
                obj->in_use = 1;

    /* Check whether we should burn away boots *first* so we know whether to
     * make the player sink into the lava. Assumption: water walking only
     * comes from boots.
     */
    if (uarmf && melts_in_lava(uarmf) && !uarmf->oerodeproof)
    {
        obj = uarmf;
        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s into flame!", Yobjnam2(obj, "burst"));
        iflags.in_lava_effects++; /* (see above) */
        (void) Boots_off();
        useup(obj);
        iflags.in_lava_effects--;
    }

    if (!Fire_immunity)
    {
        if (Walks_on_water)
        {
            pline_The("%s here burns you!", hliquid("lava"));
            if (usurvive) 
            {
                losehp(adjust_damage(dmg, (struct monst*)0, &youmonst, AD_FIRE, ADFLAGS_NONE), lava_killer, KILLED_BY); /* lava damage */
                goto burn_stuff;
            }
        }
        else
            You("fall into the %s!", hliquid("lava"));

        usurvive = Lifesaved || discover;
        if (wizard)
            usurvive = TRUE;

        /* prevent remove_worn_item() -> Boots_off(WATER_WALKING_BOOTS) ->
           spoteffects() -> lava_effects() recursion which would
           successfully delete (via useupall) the no-longer-worn boots;
           once recursive call returned, we would try to delete them again
           here in the outer call (and access stale memory, probably panic) */
        iflags.in_lava_effects++;

        for (obj = invent; obj; obj = obj2)
        {
            obj2 = obj->nobj;
            /* above, we set in_use for objects which are to be destroyed */
            if (obj->otyp == SPE_BOOK_OF_THE_DEAD && !Blind) 
            {
                if (usurvive)
                {
                    const char* hclr = hcolor_multi_buf2(NH_DARK_RED);
                    multicolor_buffer[1] = multicolor_buffer[3] = multicolor_buffer[2];
                    pline_multi_ex(ATR_NONE, CLR_MSG_WARNING, no_multiattrs, multicolor_buffer, "%s glows a %s%s%s, but remains intact.",
                        The(xname(obj)), "strange ", hclr, " light");
                }
            }
            else if (obj->oartifact == ART_RULING_RING_OF_YENDOR)
            {
                read_the_ruling_ring(obj);
            }
            else if (obj->in_use)
            {
                if (obj->owornmask) 
                {
                    if (usurvive)
                        pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s into flame!", Yobjnam2(obj, "burst"));
                    remove_worn_item(obj, TRUE);
                }
                useupall(obj);
            }
        }

        iflags.in_lava_effects--;

        /* s/he died... */
        boil_away = (is_watery(&mons[u.umonnum])
                     || u.umonnum == PM_STEAM_VORTEX
                     || u.umonnum == PM_FOG_CLOUD);
        for (;;)
        {
            u.uhp = -1;
            /* killer format and name are reconstructed every iteration
               because lifesaving resets them */
            killer.format = KILLED_BY;
            Strcpy(killer.name, lava_killer);
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s...", boil_away ? "boil away" : "burn to a crisp");
            done(BURNING);
            if (safe_teleds(TRUE, FALSE))
                break; /* successful life-save */
            /* nowhere safe to land; repeat burning loop */
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "You're still burning.");
        }
        You_ex(ATR_NONE, CLR_MSG_SUCCESS, "find yourself back on solid %s.", surface(u.ux, u.uy));
        return TRUE;
    } 
    else if (!Walks_on_water && (!u.utrap || u.utraptype != TT_LAVA))
    {
        boil_away = !Fire_immunity;
        /* if not fire resistant, sink_into_lava() will quickly be fatal;
           hero needs to escape immediately */
        set_utrap((unsigned) (rn1(4, 4) + ((boil_away ? 2 : rn1(4, 12)) << 8)),
                  TT_LAVA);
        You_ex(ATR_NONE, !boil_away ? CLR_ORANGE : CLR_RED, "sink into the %s%s!", hliquid("lava"),
            !boil_away ? ", but it only burns slightly"
                       : " and are about to be immolated");
        if (u.uhp > 1)
            losehp(adjust_damage(!boil_away ? 1 : (u.uhp / 2), (struct monst*)0, &youmonst, AD_FIRE, ADFLAGS_NONE), lava_killer,
                   KILLED_BY); /* lava damage */
    }

burn_stuff:
    destroy_item(SCROLL_CLASS, AD_FIRE);
    destroy_item(SPBOOK_CLASS, AD_FIRE);
    destroy_item(POTION_CLASS, AD_FIRE);
    item_destruction_hint(AD_FIRE, FALSE);
    return FALSE;
}

/* called each turn when trapped in lava */
void
sink_into_lava()
{
    static const char sink_deeper[] = "You sink deeper into the lava.";

    if (!u.utrap || u.utraptype != TT_LAVA) {
        ; /* do nothing; this shouldn't happen */
    } else if (!is_lava(u.ux, u.uy)) {
        reset_utrap(FALSE); /* this shouldn't happen either */
    } else if (!u.uinvulnerable) {
        /* ordinarily we'd have to be fire resistant to survive long
           enough to become stuck in lava, but it can happen without
           resistance if water walking boots allow survival and then
           get burned up; u.utrap time will be quite short in that case */
        if (!Fire_immunity)
            u.uhp = (u.uhp + 2) / 3;

        u.utrap -= (1 << 8);
        if (u.utrap < (1 << 8)) {
            killer.format = KILLED_BY;
            Strcpy(killer.name, "molten lava");
            You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "sink below the surface and die.");
            burn_away_slime(); /* add insult to injury? */
            done(DISSOLVED);
            /* can only get here via life-saving; try to get away from lava */
            reset_utrap(TRUE);
            /* levitation or flight have become unblocked, otherwise Tport */
            if (!Levitation && !Flying)
                (void) safe_teleds(TRUE, FALSE);
        } else if (!u.umoved) {
            /* can't fully turn into slime while in lava, but might not
               have it be burned away until you've come awfully close */
            if (Slimed && rnd(10 - 1) >= (int) (Slimed & TIMEOUT)) {
                pline_ex(ATR_NONE, CLR_MSG_WARNING, sink_deeper);
                burn_away_slime();
            } else {
                Norep(sink_deeper);
            }
            u.utrap += rnd(4);
        }
    }
}

/* called when something has been done (breaking a boulder, for instance)
   which entails a luck penalty if performed on a sokoban level */
void
sokoban_guilt()
{
    if (Sokoban) {
        change_luck(-1, TRUE);
        /* TODO: issue some feedback so that player can learn that whatever
           he/she just did is a naughty thing to do in sokoban and should
           probably be avoided in future....
           Caveat: doing this might introduce message sequencing issues,
           depending upon feedback during the various actions which trigger
           Sokoban luck penalties. */
    }
}

/* called when a trap has been deleted or had its ttyp replaced */
STATIC_OVL void
maybe_finish_sokoban()
{
    struct trap *t;

    if (Sokoban && !in_mklev) {
        /* scan all remaining traps, ignoring any created by the hero (and by monsters);
           if this level has no more pits or holes, the current sokoban
           puzzle has been solved */
        for (t = ftrap; t; t = t->ntrap) {
            if (t->madeby_u || t->madeby_mon)
                continue;
            if (t->ttyp == PIT || t->ttyp == HOLE)
                break;
        }
        if (!t) {
            int sokonum = dungeons[u.uz.dnum].entry_lev - u.uz.dlevel + 1;

            /* we've passed the last trap without finding a pit or hole;
               clear the sokoban_rules flag so that luck penalties for
               things like breaking boulders or jumping will no longer
               be given, and restrictions on diagonal moves are lifted */
            Sokoban = 0; /* clear gl.level.flags.sokoban_rules */
            /*
             * TODO: give some feedback about solving the sokoban puzzle
             * (perhaps say "congratulations" in Japanese?).
             */

             /* log the completion event regardless of whether or not
                any normal in-game feedback has just been given */
            livelog_printf(LL_MINORAC | LL_DUMP,
                "completed %d%s Sokoban level",
                sokonum, ordin(sokonum));
        }
    }
}

const char*
get_trap_explanation(trap)
struct trap* trap;
{
    if (!Hallucination && trap->tsubtyp && defsyms[trap_to_defsym(trap->ttyp)].variations >= trap->tsubtyp)
    {
        return  defsym_variations[trap->tsubtyp - 1 +defsyms[trap_to_defsym(trap->ttyp)].variation_offset].explanation;
    }
    else
    {
        return defsyms[trap_to_defsym(what_trap(trap->ttyp, rn2_on_display_rng))].explanation;
    }
}

int
get_shooting_trap_object(trap)
struct trap* trap;
{
    if (!trap)
        return 0;

    if (trap->ttyp == ARROW_TRAP)
    {
        if (trap->tsubtyp == ARROW_TRAP_NORMAL)
            return ARROW;
        else if (trap->tsubtyp == CROSSBOW_BOLT_TRAP)
            return CROSSBOW_BOLT;
    }
    else if (trap->ttyp == DART_TRAP)
        return DART;
    else if (trap->ttyp == ROCKTRAP)
        return ROCK;

    return 0;
}

void
reset_traps(VOID_ARGS)
{
    memset((genericptr_t)&launchplace, 0, sizeof(launchplace));
    memset((genericptr_t)&acid_ctx, 0, sizeof(acid_ctx));
    force_mintrap = 0;
}

/*trap.c*/
