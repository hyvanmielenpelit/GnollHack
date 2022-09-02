/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-28 */

/* GnollHack 4.0    dog.c    $NHDT-Date: 1554580624 2019/04/06 19:57:04 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.85 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2011. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_DCL int NDECL(pet_type);

void
newedog(mtmp)
struct monst *mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();
    if (!EDOG(mtmp)) {
        EDOG(mtmp) = (struct edog *) alloc(sizeof(struct edog));
        (void) memset((genericptr_t) EDOG(mtmp), 0, sizeof(struct edog));
    }
}

void
free_edog(mtmp)
struct monst *mtmp;
{
    if (mtmp->mextra && EDOG(mtmp)) {
        free((genericptr_t) EDOG(mtmp));
        EDOG(mtmp) = (struct edog *) 0;
    }
    mtmp->mtame = 0;
    mtmp->ispartymember = 0;
}

void
initedog(mtmp, set_tameness)
register struct monst *mtmp;
boolean set_tameness;
{
    if (set_tameness)
    {
        mtmp->mtame = is_domestic(mtmp->data) ? 10 : 5;
        mtmp->mpeaceful = 1;
    }
    mtmp->ispartymember = 0;
    mtmp->mavenge = 0;
    set_mhostility(mtmp); /* recalc alignment now that it's tamed */
    mtmp->mleashed = 0;
    mtmp->meating = 0;
    EDOG(mtmp)->droptime = 0;
    EDOG(mtmp)->dropdist = 10000;
    EDOG(mtmp)->apport = ACURR(A_CHA);
    EDOG(mtmp)->whistletime = 0;
    EDOG(mtmp)->hungrytime = 1000 + monstermoves;
    EDOG(mtmp)->ogoal.x = -1; /* force error if used before set */
    EDOG(mtmp)->ogoal.y = -1;
    EDOG(mtmp)->abuse = 0;
    EDOG(mtmp)->revivals = 0;
    EDOG(mtmp)->mhpmax_penalty = 0;
    EDOG(mtmp)->killed_by_u = 0;
}

STATIC_OVL int
pet_type()
{
    if (urole.petnum != NON_PM)
    {
        if (urole.petnum == PM_PONY && urace.monsternum == PM_DWARF)
            return PM_RAM;
        else
            return  urole.petnum;
    }
    else if (preferred_pet == 'c')
        return  PM_KITTEN;
    else if (preferred_pet == 'd')
        return  PM_LITTLE_DOG;
    else
        return  rn2(2) ? PM_KITTEN : PM_LITTLE_DOG;
}

struct monst *
make_familiar(otmp, x, y, quietly)
register struct obj *otmp;
xchar x, y;
boolean quietly;
{
    struct permonst *pm;
    struct monst *mtmp = 0;
    int chance, trycnt = 100;

    do 
    {
        if (otmp)
        { /* figurine; otherwise spell */
            int mndx = otmp->corpsenm;

            pm = &mons[mndx];
            /* activating a figurine provides one way to exceed the
               maximum number of the target critter created--unless
               it has a special limit (erinys, Nazgul) */
            if ((mvitals[mndx].mvflags & MV_EXTINCT)
                && mbirth_limit(mndx) != MAXMONNO)
            {
                if (!quietly)
                    /* have just been given "You <do something with>
                       the figurine and it transforms." message */
                    pline("... into a pile of dust.");
                break; /* mtmp is null */
            }
        } 
        else if (!rn2(3))
        {
            pm = &mons[pet_type()];
        }
        else 
        {
            pm = rndmonst();
            if (!pm)
            {
                if (!quietly)
                    There("seems to be nothing available for a familiar.");
                break;
            }
        }

        mtmp = makemon(pm, x, y, MM_EDOG | MM_IGNOREWATER | MM_NO_MONSTER_INVENTORY);
        if (otmp && !mtmp)
        { /* monster was genocided or square occupied */
            if (!quietly)
                pline_The("figurine writhes and then shatters into pieces!");
            break;
        }
    } while (!mtmp && --trycnt > 0);

    if (!mtmp)
        return (struct monst *) 0;

    if (is_pool(mtmp->mx, mtmp->my) && minliquid(mtmp))
        return (struct monst *) 0;

    initedog(mtmp, TRUE);
    mtmp->msleeping = 0;

    if (otmp)
    { /* figurine; resulting monster might not become a pet */
        chance = rn2(10); /* 0==tame, 1==peaceful, 2==hostile */
        if (chance > 2)
            chance = otmp->blessed ? 0 : !otmp->cursed ? 1 : 2;
        /* 0,1,2:  b=80%,10,10; nc=10%,80,10; c=10%,10,80 */
        if (chance > 0)
        {
            mtmp->mtame = 0;   /* not tame after all */
            if (chance == 2)
            { /* hostile (cursed figurine) */
                if (!quietly)
                    You("get a bad feeling about this.");
                mtmp->mpeaceful = 0;
                set_mhostility(mtmp);
                newsym(mtmp->mx, mtmp->my);
            }
        }
        /* if figurine has been named, give same name to the monster */
        if (has_oname(otmp))
        {
            mtmp = christen_monst(mtmp, ONAME(otmp));
            if(otmp->nknown)
                mtmp->u_know_mname = 1;
        }
        if (has_uoname(otmp))
        {
            mtmp = u_name_monst(mtmp, UONAME(otmp));
        }
    }
    set_mhostility(mtmp); /* more alignment changes */
    newsym(mtmp->mx, mtmp->my);

    /* must wield weapon immediately since pets will otherwise drop it */
    if (is_tame(mtmp) && attacktype(mtmp->data, AT_WEAP)) {
        mtmp->weapon_strategy = NEED_HTH_WEAPON;
        (void) mon_wield_item(mtmp, FALSE, 0, 0);
    }
    return mtmp;
}

STATIC_VAR int petname_used = 0;

struct monst *
makedog()
{
    register struct monst *mtmp;
    register struct obj *otmp;
    const char *petname;
    const char* petname_female = "";
    int pettype;

    if (preferred_pet == 'n')
        return ((struct monst *) 0);

    pettype = pet_type();
    if (pettype == PM_LITTLE_DOG)
        petname = dogname;
    else if (pettype == PM_PONY)
        petname = horsename;
    else if (pettype == PM_RAM)
        petname = ramname;
    else if (pettype == PM_SMALL_LUGGAGE)
        petname = luggagename;
    else if (pettype == PM_DIREWOLF_CUB)
        petname = wolfname;
    else
        petname = catname;

    /* default pet names */
    if (!*petname && pettype == PM_LITTLE_DOG)
    {
        /* All of these names were for dogs. */
        petname_female = rn2(2) ? "Fifi" : "Lassie";

        if (Race_if(PM_GNOLL))
        {
            if(!rn2(2))                        /* Warcraft III: Dwarf Campaign Chapter 3 -- JG */
                petname = "Spot";
            else
                petname = "Rover";
        }
        else
        {
            if (Role_if(PM_CAVEMAN))
                petname = "Slasher";        /* The Warrior */
            if (Role_if(PM_SAMURAI))
                petname = "Hachiko";        /* Shibuya Station */
            if (Role_if(PM_BARBARIAN))
                petname = "Idefix";            /* Obelix */
            if (Role_if(PM_TOURIST))
            {
                petname = "Pepe";            /* Tribute to a male Welsh springer spaniel -- JG */
                petname_female = "Luna";    /* Tribute to a female Finnish Lapphund -- JG */
            }
            if (Role_if(PM_RANGER))
                petname = "Sirius";            /* Orion's dog */
        }
    }
    else if(!*petname && pettype == PM_DIREWOLF_CUB) 
    {
        if (Role_if(PM_VALKYRIE))
        {
            petname = "Ghost";                /* Game of Thrones */
            petname_female = "Nymeria";        /* Game of Thrones */
        }
    }

    mtmp = makemon(&mons[pettype], u.ux, u.uy, MM_EDOG | MM_NORMAL_HIT_DICE | MM_NO_MONSTER_INVENTORY);

    if (!mtmp)
        return ((struct monst *) 0); /* pets were genocided */

    if (pettype == PM_LITTLE_DOG && Role_if(PM_SAMURAI))
    {
        mtmp->female = FALSE;
        mtmp->isfaithful = 1; /* Hachiko is well-known for its faithfulness -- JG */
    }

    context.startingpet_mid = mtmp->m_id;

    /* Horses and rams already wear a saddle */
    if ((pettype == PM_PONY || pettype == PM_RAM ) && !!(otmp = mksobj(SADDLE, TRUE, FALSE, FALSE)))
    {
        otmp->dknown = otmp->bknown = otmp->rknown = otmp->nknown = 1;
        put_saddle_on_mon(otmp, mtmp);
    }

    if (!petname_used++)
    {
        if (*petname_female && mtmp->female)
            mtmp = christen_monst(mtmp, petname_female);
        else if (*petname)
            mtmp = christen_monst(mtmp, petname);

        mtmp->u_know_mname = 1;
    }
    initedog(mtmp, TRUE);
    return  mtmp;
}

/* record `last move time' for all monsters prior to level save so that
   mon_arrive() can catch up for lost time when they're restored later */
void
update_mlstmv()
{
    struct monst *mon;

    /* monst->mlstmv used to be updated every time `monst' actually moved,
       but that is no longer the case so we just do a blanket assignment */
    for (mon = fmon; mon; mon = mon->nmon) {
        if (DEADMONSTER(mon))
            continue;
        mon->mlstmv = monstermoves;
    }
}

void
losedogs()
{
    register struct monst *mtmp, *mtmp0, *mtmp2;
    int dismissKops = 0;

    /*
     * First, scan migrating_mons for shopkeepers who want to dismiss Kops,
     * and scan mydogs for shopkeepers who want to retain kops.
     * Second, dismiss kops if warranted, making more room for arrival.
     * Third, place monsters accompanying the hero.
     * Last, place migrating monsters coming to this level.
     *
     * Hero might eventually be displaced (due to the third step, but
     * occurring later), which is the main reason to do the second step
     * sooner (in turn necessitating the first step, rather than combining
     * the list scans with monster placement).
     */

    /* check for returning shk(s) */
    for (mtmp = migrating_mons; mtmp; mtmp = mtmp->nmon) {
        if (mtmp->mux != u.uz.dnum || mtmp->muy != u.uz.dlevel)
            continue;
        if (mtmp->isshk) {
            if (ESHK(mtmp)->dismiss_kops) {
                if (dismissKops == 0)
                    dismissKops = 1;
                ESHK(mtmp)->dismiss_kops = FALSE; /* reset */
            } else if (!is_peaceful(mtmp)) {
                /* an unpacified shk is returning; don't dismiss kops
                   even if another pacified one is willing to do so */
                dismissKops = -1;
                /* [keep looping; later monsters might need ESHK reset] */
            }
        }
    }
    /* make the same check for mydogs */
    for (mtmp = mydogs; mtmp && dismissKops >= 0; mtmp = mtmp->nmon) {
        if (mtmp->isshk) {
            /* hostile shk might accompany hero [ESHK(mtmp)->dismiss_kops
               can't be set here; it's only used for migrating_mons] */
            if (!is_peaceful(mtmp))
                dismissKops = -1;
        }
    }

    /* when a hostile shopkeeper chases hero to another level
       and then gets paid off there, get rid of summoned kops
       here now that he has returned to his shop level */
    if (dismissKops > 0)
        make_happy_shoppers(TRUE);

    /* place pets and/or any other monsters who accompany hero */
    while ((mtmp = mydogs) != 0) {
        mydogs = mtmp->nmon;
        mon_arrive(mtmp, TRUE);
    }

    /* time for migrating monsters to arrive;
       monsters who belong on this level but fail to arrive get put
       back onto the list (at head), so traversing it is tricky */
    for (mtmp = migrating_mons; mtmp; mtmp = mtmp2) {
        mtmp2 = mtmp->nmon;
        if (mtmp->mux == u.uz.dnum && mtmp->muy == u.uz.dlevel) {
            /* remove mtmp from migrating_mons list */
            if (mtmp == migrating_mons) {
                migrating_mons = mtmp->nmon;
            } else {
                for (mtmp0 = migrating_mons; mtmp0; mtmp0 = mtmp0->nmon)
                    if (mtmp0->nmon == mtmp) {
                        mtmp0->nmon = mtmp->nmon;
                        break;
                    }
                if (!mtmp0)
                {
                    panic("losedogs: can't find migrating mon");
                    return;
                }
            }
            mon_arrive(mtmp, FALSE);
        }
    }
}

/* called from resurrect() in addition to losedogs() */
void
mon_arrive(mtmp, with_you)
struct monst *mtmp;
boolean with_you;
{
    struct trap *t;
    xchar xlocale, ylocale, xyloc, xyflags, wander;
    int num_segs;
    boolean failed_to_place = FALSE;

    mtmp->nmon = fmon;
    fmon = mtmp;
    if (mtmp->isshk)
        set_residency(mtmp, FALSE);

    num_segs = mtmp->wormno;

    /* baby long worms have no tail so don't use is_long_worm() */
    if (is_long_worm_with_tail(mtmp->data))
    {
        mtmp->wormno = get_wormno();
        if (mtmp->wormno)
            initworm(mtmp, num_segs);
    } 
    else
        mtmp->wormno = 0;

    /* some monsters might need to do something special upon arrival
       _after_ the current level has been fully set up; see dochug() */
    mtmp->mstrategy |= STRAT_ARRIVE;

    /* make sure mnexto(rloc_to(set_apparxy())) doesn't use stale data */
    mtmp->mux = u.ux, mtmp->muy = u.uy;
    xyloc = mtmp->mtrack[0].x;
    xyflags = mtmp->mtrack[0].y;
    xlocale = mtmp->mtrack[1].x;
    ylocale = mtmp->mtrack[1].y;
    memset(mtmp->mtrack, 0, sizeof mtmp->mtrack);

    if (mtmp == u.usteed)
        return; /* don't place steed on the map */
    
    if (with_you) 
    {
        /* When a monster accompanies you, sometimes it will arrive
           at your intended destination and you'll end up next to
           that spot.  This code doesn't control the final outcome;
           goto_level(do.c) decides who ends up at your target spot
           when there is a monster there too. */
        if (!MON_AT(u.ux, u.uy) && !is_tame(mtmp) && !is_peaceful(mtmp) && !rn2(2))
            rloc_to(mtmp, u.ux, u.uy);
        else
            mnexto(mtmp);
        return;
    }
    /*
     * The monster arrived on this level independently of the player.
     * Its coordinate fields were overloaded for use as flags that
     * specify its final destination.
     */

    long nmv = monstermoves - 1L - mtmp->mlstmv;
    if (nmv > 0) 
    {
        /* mtmp->mlstmv < monstermoves - 1L */
        /* heal monster for time spent in limbo */
        mon_catchup_elapsed_time(mtmp, nmv);
        mtmp->mlstmv = monstermoves - 1L;

        /* let monster move a bit on new level (see placement code below) */
        wander = (xchar) min(nmv, 8);
    } 
    else
        wander = 0;

    switch (xyloc) 
    {
    case MIGR_APPROX_XY: /* {x,y}locale set above */
        break;
    case MIGR_EXACT_XY:
        wander = 0;
        break;
    case MIGR_WITH_HERO:
        xlocale = u.ux, ylocale = u.uy;
        break;
    case MIGR_STAIRS_UP:
        xlocale = xupstair, ylocale = yupstair;
        break;
    case MIGR_STAIRS_DOWN:
        xlocale = xdnstair, ylocale = ydnstair;
        break;
    case MIGR_LADDER_UP:
        xlocale = xupladder, ylocale = yupladder;
        break;
    case MIGR_LADDER_DOWN:
        xlocale = xdnladder, ylocale = ydnladder;
        break;
    case MIGR_SSTAIRS:
        xlocale = sstairs.sx, ylocale = sstairs.sy;
        break;
    case MIGR_MODRON_PORTAL_UP:
        xlocale = 0, ylocale = 0;
        for (t = ftrap; t; t = t->ntrap)
            if (t->ttyp == MODRON_PORTAL && (t->tflags & TRAPFLAGS_LEVEL_TELEPORT_UP))
                break;
        if (t)
        {
            xlocale = t->tx, ylocale = t->ty;
        }
        else
        {
            impossible("mon_arrive: no corresponding portal?");
        }
        break;
    case MIGR_MODRON_PORTAL_DOWN:
        xlocale = 0, ylocale = 0;
        for (t = ftrap; t; t = t->ntrap)
            if (t->ttyp == MODRON_PORTAL && (t->tflags & TRAPFLAGS_LEVEL_TELEPORT_DOWN))
                break;
        if (t)
        {
            xlocale = t->tx, ylocale = t->ty;
        }
        else
        {
            impossible("mon_arrive: no corresponding portal?");
        }
        break;
    case MIGR_PORTAL:
        if (In_endgame(&u.uz)) 
        {
            /* there is no arrival portal for endgame levels */
            /* BUG[?]: for simplicity, this code relies on the fact
               that we know that the current endgame levels always
               build upwards and never have any exclusion subregion
               inside their TELEPORT_REGION settings. */
            xlocale = rn1(updest.hx - updest.lx + 1, updest.lx);
            ylocale = rn1(updest.hy - updest.ly + 1, updest.ly);
            break;
        }
        /* find the arrival portal */
        for (t = ftrap; t; t = t->ntrap)
            if (t->ttyp == MAGIC_PORTAL)
                break;
        if (t) 
        {
            xlocale = t->tx, ylocale = t->ty;
            break;
        }
        else 
        {
            impossible("mon_arrive: no corresponding portal?");
        } /*FALLTHRU*/
    default:
    case MIGR_RANDOM:
        xlocale = ylocale = 0;
        break;
    }

    if ((mtmp->mspare1 & MIGR_LEFTOVERS) != 0L) 
    {
        /* Pick up the rest of the MIGR_TO_SPECIES objects */
        if (migrating_objs)
            deliver_obj_to_mon(mtmp, 0, DF_ALL);
    }

    if (xlocale && wander) 
    {
        /* monster moved a bit; pick a nearby location */
        /* mnearto() deals w/stone, et al */
        char *r = in_rooms(xlocale, ylocale, 0);

        if (r && *r) 
        {
            coord c;

            /* somexy() handles irregular rooms */
            coord point = { 0 };
            point.x = xlocale;
            point.y = ylocale;
            if (somexy_within_distance(&rooms[*r - ROOMOFFSET], point, wander, &c))
                xlocale = c.x, ylocale = c.y;
            else
                xlocale = ylocale = 0;
        } 
        else 
        { /* not in a room */
            int i, j;

            i = max(1, xlocale - wander);
            j = min(COLNO - 1, xlocale + wander);
            xlocale = rn1(j - i, i);
            i = max(0, ylocale - wander);
            j = min(ROWNO - 1, ylocale + wander);
            ylocale = rn1(j - i, i);
        }
    } /* moved a bit */

    mtmp->mx = 0; /*(already is 0)*/
    mtmp->my = xyflags;
    if (xlocale)
        failed_to_place = !mnearto(mtmp, xlocale, ylocale, FALSE, FALSE);
    else
        failed_to_place = !rloc(mtmp, TRUE);

    if (failed_to_place)
        m_into_limbo(mtmp, FALSE); /* try again next time hero comes to this level */
}

/* heal monster for time spent elsewhere */
void
mon_catchup_elapsed_time(mtmp, nmv)
struct monst *mtmp;
long nmv; /* number of moves */
{
    int imv = 0; /* avoid zillions of casts and lint warnings */

#if defined(DEBUG) || defined(BETA)
    if (nmv < 0L) 
    { /* crash likely... */
        panic("catchup from future time?");
        /*NOTREACHED*/
        return;
    }
    else if (nmv == 0L) 
    { /* safe, but should'nt happen */
        impossible("catchup from now?");
    } 
    else
#endif
        if (nmv >= LARGEST_INT) /* paranoia */
        imv = LARGEST_INT - 1;
    else
        imv = (int) nmv;

    /* might stop being afraid, blind or frozen */
    /* set to 1 and allow final decrement in movemon() */
    if (is_blinded(mtmp)) 
    {
        if (imv >= (int)is_blinded(mtmp))
            set_mon_property(mtmp, BLINDED, 0);
        else
            increase_mon_property(mtmp, BLINDED, -imv);
    }

    if (mtmp->mfrozen)
    {
        if (imv >= (int) mtmp->mfrozen)
            mtmp->mfrozen = 1;
        else
            mtmp->mfrozen -= imv;
    }

    if (mtmp->mstaying) 
    {
        if (imv >= (int)mtmp->mstaying)
            mtmp->mstaying = 1;
        else
            mtmp->mstaying -= imv;
    }

    if (mtmp->mcarrying) 
    {
        if (imv >= (int)mtmp->mcarrying)
            mtmp->mcarrying = 1;
        else
            mtmp->mcarrying -= imv;
    }

    if (mtmp->mcomingtou) 
    {
        if (imv >= (int)mtmp->mcomingtou)
            mtmp->mcomingtou = 1;
        else
            mtmp->mcomingtou -= imv;
    }

    if (mtmp->notraveltimer)
    {
        if (imv >= (int)mtmp->notraveltimer)
            mtmp->notraveltimer = 1;
        else
            mtmp->notraveltimer -= imv;
    }

    if (mtmp->mflee_timer)
    {
        if (imv >= (int) mtmp->mflee_timer)
            mtmp->mflee_timer = 1;
        else
            mtmp->mflee_timer -= imv;
    }

    /* might recover from temporary trouble */
    if (mtmp->mtrapped && rn2(imv + 1) > 40 / 2)
        mtmp->mtrapped = 0;
    if (is_confused(mtmp) && rn2(imv + 1) > 50 / 2)
        mtmp->mprops[CONFUSION] = 0;
    if (is_stunned(mtmp) && rn2(imv + 1) > 10 / 2)
        mtmp->mprops[STUNNED] = 0;

    /* might finish eating or be able to use special ability again */
    if (imv > mtmp->meating)
        finish_meating(mtmp);
    else
        mtmp->meating -= imv;

    /* reduce spec_used */
    /* recover lost energy */
    int repidest_enreg_dur = !has_rapidest_regeneration(mtmp) ? 0 : (mtmp->mprops[RAPIDEST_REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[RAPIDEST_REGENERATION] & M_TIMEOUT) : imv;
    int repider_enreg_dur = !has_rapider_regeneration(mtmp) ? 0 : (mtmp->mprops[RAPIDER_REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[RAPIDER_REGENERATION] & M_TIMEOUT) : imv;
    int repid_enreg_dur = !has_rapider_regeneration(mtmp) ? 0 : (mtmp->mprops[RAPID_REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[RAPID_REGENERATION] & M_TIMEOUT) : imv;
    int enreg_dur = !has_regeneration(mtmp) ? 0 : (mtmp->mprops[REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[REGENERATION] & M_TIMEOUT) : imv;
    int eff_repidest_enreg_dur = max(0, repidest_enreg_dur);
    int eff_repider_enreg_dur = max(0, repider_enreg_dur - repidest_enreg_dur);
    int eff_repid_enreg_dur = max(0, repid_enreg_dur - repider_enreg_dur - repidest_enreg_dur);
    int eff_enreg_dur = max(0, enreg_dur - repid_enreg_dur - repider_enreg_dur - repidest_enreg_dur);
    int eff_normal_enreg_dur = max(0, imv - enreg_dur - repid_enreg_dur - repider_enreg_dur - repidest_enreg_dur);

    int enrecovered = eff_normal_enreg_dur * 1;
    enrecovered += (eff_repidest_enreg_dur ? eff_repidest_enreg_dur * 5 : 0);
    enrecovered += (eff_repider_enreg_dur ? eff_repider_enreg_dur * 4 : 0);
    enrecovered += (eff_repid_enreg_dur ? eff_repid_enreg_dur * 3 : 0);
    enrecovered += (eff_enreg_dur ? eff_enreg_dur * 2 : 0);

    /* Using enrecovered */
    if (enrecovered > mtmp->mspec_used)
        mtmp->mspec_used = 0;
    else
        mtmp->mspec_used -= enrecovered;

    if (enrecovered > mtmp->mmagespell_used)
        mtmp->mmagespell_used = 0;
    else
        mtmp->mmagespell_used -= enrecovered;

    if (enrecovered > mtmp->mmageintermediate_used)
        mtmp->mmageintermediate_used = 0;
    else
        mtmp->mmageintermediate_used -= enrecovered;

    if (enrecovered > mtmp->mmageultimate_used)
        mtmp->mmageultimate_used = 0;
    else
        mtmp->mmageultimate_used -= enrecovered;

    if (enrecovered > mtmp->mclericspell_used)
        mtmp->mclericspell_used = 0;
    else
        mtmp->mclericspell_used -= enrecovered;

    if (enrecovered > mtmp->mclericintermediate_used)
        mtmp->mclericintermediate_used = 0;
    else
        mtmp->mclericintermediate_used -= enrecovered;

    if (enrecovered > mtmp->mclericultimate_used)
        mtmp->mclericultimate_used = 0;
    else
        mtmp->mclericultimate_used -= enrecovered;

    /* Using imv only */
    if (imv > mtmp->mdemonsummon_used)
        mtmp->mdemonsummon_used = 0;
    else
        mtmp->mdemonsummon_used -= imv;

    if (imv > mtmp->mspecialsummon_used)
        mtmp->mspecialsummon_used = 0;
    else
        mtmp->mspecialsummon_used -= imv;

    if (imv > mtmp->mspecialsummon2_used)
        mtmp->mspecialsummon2_used = 0;
    else
        mtmp->mspecialsummon2_used -= imv;

    /* reduce tameness for every 300 moves you are separated */
    if (mtmp->mtame && !mtmp->isfaithful) 
    {
        int wilder = (imv + 0) / 300;
        if (mtmp->mtame > wilder)
            mtmp->mtame -= wilder; /* less tame */
        else if (mtmp->mtame > rn2(wilder))
            mtmp->mtame = 0; /* untame */
        else
            mtmp->mtame = mtmp->mpeaceful = 0; /* hostile! */

        if (!mtmp->mtame)
            mtmp->ispartymember = FALSE;
    }

    /* check to see if it would have died as a pet; if so, go wild instead
     * of dying the next time we call dog_move()
     */
    if (mtmp->mtame && has_edog(mtmp) && !mtmp->isminion && !is_non_eater(mtmp->data))
    {
        struct edog *edog = EDOG(mtmp);

        if ((monstermoves > edog->hungrytime + 500 && mtmp->mhp < 3)
            || (monstermoves > edog->hungrytime + 750))
            mtmp->mtame = mtmp->mpeaceful = 0;

        if(!mtmp->mtame)
            mtmp->ispartymember = 0;
    }

    if (!mtmp->mtame && mtmp->mleashed) 
    {
        /* leashed monsters should always be with hero, consequently
           never losing any time to be accounted for later */
        impossible("catching up for leashed monster?");
        m_unleash(mtmp, FALSE);
    }

    /* recover lost hit points */
    int divine_reg_dur = !has_divine_regeneration(mtmp) ? 0 : (mtmp->mprops[DIVINE_REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[DIVINE_REGENERATION] & M_TIMEOUT) : imv;
    int repidest_reg_dur = !has_rapidest_regeneration(mtmp) ? 0 : (mtmp->mprops[RAPIDEST_REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[RAPIDEST_REGENERATION] & M_TIMEOUT) : imv;
    int repider_reg_dur = !has_rapider_regeneration(mtmp) ? 0 : (mtmp->mprops[RAPIDER_REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[RAPIDER_REGENERATION] & M_TIMEOUT) : imv;
    int repid_reg_dur = !has_rapider_regeneration(mtmp) ? 0 : (mtmp->mprops[RAPID_REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[RAPID_REGENERATION] & M_TIMEOUT) : imv;
    int reg_dur = !has_regeneration(mtmp) ? 0 : (mtmp->mprops[REGENERATION] & M_TIMEOUT) ? (mtmp->mprops[REGENERATION] & M_TIMEOUT) : imv;
    int eff_divine_reg_dur = divine_reg_dur;
    int eff_repidest_reg_dur = max(0, repidest_reg_dur - divine_reg_dur);
    int eff_repider_reg_dur = max(0, repider_reg_dur - repidest_reg_dur - divine_reg_dur);
    int eff_repid_reg_dur = max(0, repid_reg_dur - repider_reg_dur - repidest_reg_dur - divine_reg_dur);
    int eff_reg_dur = max(0, reg_dur - repid_reg_dur - repider_reg_dur - repidest_reg_dur - divine_reg_dur);
    int eff_normal_reg_dur = max(0, imv - reg_dur - repid_reg_dur - repider_reg_dur - repidest_reg_dur - divine_reg_dur);

    int hprecovered = eff_normal_reg_dur / 20;
    hprecovered += (eff_divine_reg_dur ? eff_divine_reg_dur * max(1, min(mtmp->mhpmax / 16, 10)) : 0);
    hprecovered += (eff_repidest_reg_dur ? eff_repidest_reg_dur * max(1, min(mtmp->mhpmax / 8, 20)) : 0);
    hprecovered += (eff_repider_reg_dur ? eff_repider_reg_dur * max(1, min(mtmp->mhpmax / 4, 40)) : 0);
    hprecovered += (eff_repid_reg_dur ? eff_repid_reg_dur * max(1, min(mtmp->mhpmax / 2, 80)) : 0);
    hprecovered += (eff_reg_dur ? eff_reg_dur * max(1, min(mtmp->mhpmax, 160)) : 0);

    if (mtmp->mhp + hprecovered >= mtmp->mhpmax)
        mtmp->mhp = mtmp->mhpmax;
    else
        mtmp->mhp += hprecovered;

    int i;
    for (i = 1; i <= LAST_PROP; i++)
    {
        unsigned short tim = (mtmp->mprops[i] & M_TIMEOUT);
        if (tim)
        {
            if (imv > tim)
                mtmp->mprops[i] = (mtmp->mprops[i] & ~M_TIMEOUT) | 1;
            else
                mtmp->mprops[i] = (mtmp->mprops[i] & ~M_TIMEOUT) | tim;
        }
    }

    if (imv != 0 && iflags.wc2_statuslines > 3 && is_tame(mtmp))
        context.botl = 1;

}

/* called when you move to another level */
void
keepdogs(pets_only)
boolean pets_only; /* true for ascension or final escape */
{
    register struct monst *mtmp, *mtmp2;
    register struct obj *obj;
    int num_segs;
    boolean stay_behind;

    for (mtmp = fmon; mtmp; mtmp = mtmp2) {
        mtmp2 = mtmp->nmon;
        if (DEADMONSTER(mtmp))
            continue;
        if (pets_only) {
            if (!is_tame(mtmp))
                continue; /* reject non-pets */
            /* don't block pets from accompanying hero's dungeon
               escape or ascension simply due to mundane trifles;
               unlike level change for steed, don't bother trying
               to achieve a normal trap escape first */
            mtmp->mtrapped = 0;
            mtmp->meating = 0;
            mtmp->msleeping = 0;
            mtmp->mfrozen = 0;
            mtmp->mstaying = 0;
            mtmp->mcarrying = 0;
            mtmp->mcomingtou = 0;
            mtmp->notraveltimer = 0;
            mtmp->yell_x = 0;
            mtmp->yell_y = 0;
            mtmp->mcanmove = 1;
            mtmp->mwantstomove = 1;
            mtmp->mwantstodrop = 1;
        }
        if ((((monnear(mtmp, u.ux, u.uy) || (is_tame(mtmp) && mon_somewhat_near(mtmp, u.ux, u.uy))) && levl_follower(mtmp))
             /* the wiz will level t-port from anywhere to chase
                the amulet; if you don't have it, will chase you
                only if in range. -3. */
             || (u.uhave.amulet && mtmp->iswiz))
            && ((mon_can_move(mtmp))
                /* eg if level teleport or new trap, steed has no control
                   to avoid following */
                || (mtmp == u.usteed))
            /* monster won't follow if it hasn't noticed you yet */
            && !(mtmp->mstrategy & STRAT_WAITFORU))
        {
            stay_behind = FALSE;
            if (mtmp->mtrapped)
                (void) mintrap(mtmp); /* try to escape */
            if (mtmp == u.usteed) {
                /* make sure steed is eligible to accompany hero */
                mtmp->mtrapped = 0;       /* escape trap */
                mtmp->meating = 0;        /* terminate eating */
                mdrop_special_objs(mtmp); /* drop Amulet */
            } else if (mtmp->meating || mtmp->mtrapped) {
                if (canseemon(mtmp))
                    pline("%s is still %s.", Monnam(mtmp),
                          mtmp->meating ? "eating" : "trapped");
                stay_behind = TRUE;
            } else if (mon_has_amulet(mtmp)) {
                if (canseemon(mtmp))
                    pline("%s seems very disoriented for a moment.",
                          Monnam(mtmp));
                stay_behind = TRUE;
            }
            if (stay_behind) {
                if (mtmp->mleashed) {
                    play_sfx_sound(SFX_LEASH_GOES_SLACK);
                    pline("%s leash suddenly comes loose.",
                          humanoid(mtmp->data)
                              ? (mtmp->female ? "Her" : "His")
                              : "Its");
                    m_unleash(mtmp, FALSE);
                }
                if (mtmp == u.usteed) {
                    /* can't happen unless someone makes a change
                       which scrambles the stay_behind logic above */
                    impossible("steed left behind?");
                    dismount_steed(DISMOUNT_GENERIC);
                }
                continue;
            }
            if (mtmp->isshk)
                set_residency(mtmp, TRUE);

            if (mtmp->wormno) {
                register int cnt;
                /* NOTE: worm is truncated to # segs = max wormno size */
                cnt = count_wsegs(mtmp);
                num_segs = min(cnt, MAX_NUM_WORMS - 1);
                wormgone(mtmp);
                place_monster(mtmp, mtmp->mx, mtmp->my);
            } else
                num_segs = 0;

            /* set minvent's obj->no_charge to 0 */
            for (obj = mtmp->minvent; obj; obj = obj->nobj) {
                if (Has_contents(obj))
                    picked_container(obj); /* does the right thing */
                obj->no_charge = 0;
            }

            relmon(mtmp, &mydogs);   /* move it from map to mydogs */
            mtmp->mx = mtmp->my = 0; /* avoid mnexto()/MON_AT() problem */
            mtmp->wormno = num_segs;
            mtmp->mlstmv = monstermoves;
        } else if (mtmp->iswiz) {
            /* we want to be able to find him when his next resurrection
               chance comes up, but have him resume his present location
               if player returns to this level before that time */
            migrate_to_level(mtmp, ledger_no(&u.uz), MIGR_EXACT_XY,
                             (coord *) 0);
        } else if (mtmp->mleashed) {
            /* this can happen if your quest leader ejects you from the
               "home" level while a leashed pet isn't next to you */
            play_sfx_sound(SFX_LEASH_GOES_SLACK);
            pline("%s leash goes slack.", s_suffix(Monnam(mtmp)));
            m_unleash(mtmp, FALSE);
        }
    }
}

void
migrate_to_level(mtmp, tolev, xyloc, cc)
register struct monst *mtmp;
xchar tolev; /* destination level */
xchar xyloc; /* MIGR_xxx destination xy location: */
coord *cc;   /* optional destination coordinates */
{
    struct obj *obj;
    d_level new_lev;
    xchar xyflags;
    int num_segs = 0; /* count of worm segments */

    if (mtmp->isshk)
        set_residency(mtmp, TRUE);

    if (mtmp->wormno) {
        int cnt = count_wsegs(mtmp);

        /* **** NOTE: worm is truncated to # segs = max wormno size **** */
        num_segs = min(cnt, MAX_NUM_WORMS - 1); /* used below */
        wormgone(mtmp); /* destroys tail and takes head off map */
        /* there used to be a place_monster() here for the relmon() below,
           but it doesn't require the monster to be on the map anymore */
    }

    /* set minvent's obj->no_charge to 0 */
    for (obj = mtmp->minvent; obj; obj = obj->nobj) {
        if (Has_contents(obj))
            picked_container(obj); /* does the right thing */
        obj->no_charge = 0;
    }

    if (mtmp->mleashed) {
        mtmp->mtame--;
        if (!mtmp->mtame)
            mtmp->ispartymember = 0;
        m_unleash(mtmp, TRUE);
    }
    relmon(mtmp, &migrating_mons); /* move it from map to migrating_mons */

    new_lev.dnum = ledger_to_dnum((xchar) tolev);
    new_lev.dlevel = ledger_to_dlev((xchar) tolev);
    /* overload mtmp->[mx,my], mtmp->[mux,muy], and mtmp->mtrack[] as */
    /* destination codes (setup flag bits before altering mx or my) */
    xyflags = (depth(&new_lev) < depth(&u.uz)); /* 1 => up */
    if (In_W_tower(mtmp->mx, mtmp->my, &u.uz))
        xyflags |= 2;
    mtmp->wormno = num_segs;
    mtmp->mlstmv = monstermoves;
    mtmp->mtrack[1].x = cc ? cc->x : mtmp->mx;
    mtmp->mtrack[1].y = cc ? cc->y : mtmp->my;
    mtmp->mtrack[0].x = xyloc;
    mtmp->mtrack[0].y = xyflags;
    mtmp->mux = new_lev.dnum;
    mtmp->muy = new_lev.dlevel;
    mtmp->mx = mtmp->my = 0; /* this implies migration */
    mtmp->mon_flags &= ~MON_FLAGS_SPLEVEL_RESIDENT; /* Not level's original resident anymore */
    if (mtmp == context.polearm.hitmon)
        context.polearm.hitmon = (struct monst *) 0;
}

/* return quality of food; the lower the better */
/* fungi will eat even tainted food */
int
dogfood(mon, obj)
struct monst *mon;
register struct obj *obj;
{
    struct permonst *mptr = mon->data, *fptr = 0;
    boolean carni = carnivorous(mptr), herbi = herbivorous(mptr),
            starving, mblind;

    if (is_quest_artifact(obj) || obj_resists(obj, 0, 95))
        return obj->cursed ? TABU : APPORT;

    if (is_non_eater(mptr))
        return TABU;

    if (is_corpse_eater(mptr) && obj->otyp != CORPSE)
        return TABU;

    switch (obj->oclass) 
    {
    case FOOD_CLASS:
        if (obj->otyp == CORPSE || obj->otyp == TIN || obj->otyp == EGG)
            fptr = &mons[obj->corpsenm];

        if (obj->otyp == CORPSE && is_rider(fptr))
            return TABU;

        if ((obj->otyp == CORPSE || obj->otyp == EGG) && fptr && touch_petrifies(fptr)
            && !resists_ston(mon))
            return POISON;

        if ((obj->otyp == CORPSE || obj->otyp == EGG) &&
            (polyfodder(obj) || obj->corpsenm == PM_GREEN_SLIME || (obj->corpsenm >= LOW_PM && is_mimic(&mons[obj->corpsenm]))))
            return POISON;

        if (objects[obj->otyp].oc_edible_subtype > EDIBLETYPE_NORMAL)
            return POISON;

        if (!carni && !herbi)
            return obj->cursed ? UNDEF : APPORT;

        /* a starving pet will eat almost anything */
        starving = (mon->mtame && mon->mextra && EDOG(mon) && !mon->isminion
                    && EDOG(mon)->mhpmax_penalty);

        /* even carnivores will eat carrots if they're temporarily blind */
        mblind = (is_blinded(mon) && haseyes(mon->data));

        /* ghouls prefer old corpses and unhatchable eggs, yum!
           they'll eat fresh non-veggy corpses and hatchable eggs
           when starving; they never eat stone-to-flesh'd meat */
        if (is_corpse_eater(mptr))
        {
            if (obj->otyp == CORPSE)
                return (peek_at_iced_corpse_age(obj) + 50L <= monstermoves
                        && !nonrotting_corpse_ptr(fptr))
                           ? DOGFOOD
                           : (starving && !vegan(fptr))
                              ? ACCFOOD
                              : POISON;

            if (obj->otyp == EGG)
                return stale_egg(obj) ? CADAVER : starving ? ACCFOOD : POISON;

            return TABU;
        }

        switch (obj->otyp) 
        {
        case TRIPE_RATION:
        case MEATBALL:
        case MEAT_RING:
        case MEAT_STICK:
        case HUGE_CHUNK_OF_MEAT:
            return carni ? DOGFOOD : MANFOOD;
        case EGG:
            return carni ? CADAVER : MANFOOD;
        case CORPSE:
            if ((peek_at_iced_corpse_age(obj) + 50L <= monstermoves
                 && !nonrotting_corpse(obj->corpsenm)
                 && mptr->mlet != S_FUNGUS)
                || (has_acidic_corpse(fptr) && !(is_mon_immune_to_acid(mon) || mon_resists_acid_weakly(mon)))
                || (has_poisonous_corpse(fptr) && !resists_poison(mon))
                || (has_stunning_corpse(fptr) && !resists_stun(mon))
                || (has_sickening_corpse(fptr) && !resists_sickness(mon))
                || (has_mummy_rotted_corpse(fptr) && !resists_sickness(mon))
                )
                return POISON;
            /* turning into slime is preferable to starvation */
            else if (fptr == &mons[PM_GREEN_SLIME] && !slimeproof(mon->data))
                return starving ? ACCFOOD : POISON;
            else if (incorporeal_food(fptr))
                return MANFOOD;
            else if (vegan(fptr))
                return herbi ? CADAVER : MANFOOD;
            /* most humanoids will avoid cannibalism unless starving;
               arbitrary: elves won't eat other elves even then */
            else if (humanoid(mptr) && same_race(mptr, fptr)
                     && (!is_undead(mptr) && fptr->mlet != S_KOBOLD
                         && fptr->mlet != S_ORC && fptr->mlet != S_OGRE))
                return (starving && carni && !is_elf(mptr)) ? ACCFOOD : TABU;
            else
                return carni ? CADAVER : MANFOOD;
        case CLOVE_OF_GARLIC:
            return (is_undead(mptr) || is_vampshifter(mon))
                      ? TABU
                      : (herbi || starving)
                         ? ACCFOOD
                         : MANFOOD;
        case TIN:
            return metallivorous(mptr) ? ACCFOOD : MANFOOD;
        case APPLE:
            return herbi ? DOGFOOD : starving ? ACCFOOD : MANFOOD;
        case CARROT:
            return (herbi || mblind) ? DOGFOOD : starving ? ACCFOOD : MANFOOD;
        case BANANA:
            return (mptr->mflags4 & M4_LOVES_BANANAS)
                      ? DOGFOOD /* for monkey and ape (tameable), sasquatch */
                      : (herbi || starving)
                         ? ACCFOOD
                         : MANFOOD;
        default:
            if (starving)
                return ACCFOOD;
            return (obj->otyp > SLIME_MOLD) ? (carni ? ACCFOOD : MANFOOD)
                                            : (herbi ? ACCFOOD : MANFOOD);
        }
    case REAGENT_CLASS:
    {
        /*FALLTHRU*/
    }
    default:
        if (obj->otyp == AMULET_OF_STRANGULATION
            || obj->otyp == RIN_SLOW_DIGESTION)
            return TABU;
        if (mon_hates_silver(mon) && objects[obj->otyp].oc_material == MAT_SILVER)
            return TABU;
        if (slurps_items(mptr) && is_slurpable(obj))
            return ACCFOOD;
        if (metallivorous(mptr) && is_metallic(obj)
            && (is_rustprone(obj) || !rust_causing_and_ironvorous(mptr)))
        {
            /* Non-rustproofed ferrous based metals are preferred. */
            return (is_rustprone(obj) && !obj->oerodeproof) ? DOGFOOD
                                                            : ACCFOOD;
        }
        if (lithovore(mptr) && is_obj_stony(obj))
            return (obj->speflags & SPEFLAGS_NO_PICKUP) || (In_sokoban(&u.uz) && obj->otyp == BOULDER) ? TABU : obj->cursed ? ACCFOOD : DOGFOOD;
        if (!obj->cursed
            && obj->oclass != BALL_CLASS
            && obj->oclass != CHAIN_CLASS)
            return APPORT;
        /*FALLTHRU*/
    case ROCK_CLASS:
        return UNDEF;
    }
}

/*
 * With the separate mextra structure added in 3.6.x this always
 * operates on the original mtmp. It now returns TRUE if the taming
 * succeeded.
 */
boolean
tamedog(mtmp, obj, forcetaming, charm_type, duration, verbose, thrown)
register struct monst *mtmp;
register struct obj *obj;
uchar forcetaming; /* 0 = no force, 1 = force non-uniques, 2 = force all */
int charm_type; /* 0 = permanent, 1 = charm, 2 = control undead */
unsigned short duration;
boolean verbose;
boolean thrown;
{
    /* The Wiz, Medusa and the quest nemeses aren't even made peaceful. */
    if (!mtmp || mtmp->iswiz || mtmp->data == &mons[PM_MEDUSA]
        || (mtmp->data->mflags3 & M3_WANTSARTI))
        return FALSE;

    boolean was_tame = is_tame(mtmp);
    boolean has_edog = has_edog(mtmp);

    if (!charm_type)
    {
        /* worst case, at least it'll be peaceful. */
        mtmp->mpeaceful = 1;
        set_mhostility(mtmp);
        newsym_with_flags(mtmp->mx, mtmp->my, NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS);
    }

    if (!forcetaming && thrown && flags.moonphase == FULL_MOON && night() && rn2(6) && obj
        && mtmp->data->mlet == S_DOG)
        return FALSE;

    /* If we cannot tame it, at least it's no longer afraid. */
    mtmp->mflee = 0;
    mtmp->mflee_timer = 0;

    /* make grabber let go now, whether it becomes tame or not */
    if (mtmp == u.ustuck)
    {
        if (u.uswallow)
            expels(mtmp, mtmp->data, TRUE);
        else if (!(Upolyd && sticks(youmonst.data)))
            unstuck(mtmp);
    }

    /* feeding it treats makes it tamer */
    if (mtmp->mtame && obj && mtmp->mextra && EDOG(mtmp))
    {
        int tasty;

        if (!thrown || (mon_can_move(mtmp) && !is_confused(mtmp) && !mtmp->meating
            && (((tasty = dogfood(mtmp, obj)) == DOGFOOD && dog_wants_to_eat(mtmp))
                || (tasty <= ACCFOOD && EDOG(mtmp)->hungrytime <= monstermoves)))) 
        {
            /* pet will "catch" and eat this thrown food */
            if(verbose && thrown)
            {
                if (canseemon(mtmp))
                {
                    boolean big_corpse =
                        (obj->otyp == CORPSE && obj->corpsenm >= LOW_PM
                         && mons[obj->corpsenm].msize > mtmp->data->msize);

                    pline("%s catches %s%s", Monnam(mtmp), the(xname(obj)),
                          !big_corpse ? "." : ", or vice versa!");
                }
                else if (cansee(mtmp->mx, mtmp->my))
                    pline("%s.", Tobjnam(obj, "stop"));
            }
            /* dog_eat expects a floor object */
            place_object(obj, mtmp->mx, mtmp->my);

            (void) dog_eat(mtmp, obj, mtmp->mx, mtmp->my, FALSE);
            /* eating might have killed it, but that doesn't matter here;
               a non-null result suppresses "miss" message for thrown
               food and also implies that the object has been deleted */
            return TRUE;
        }
        else
            return FALSE;
    }

    if ((mtmp->mtame) ||  (!forcetaming && (mtmp->data->geno & G_UNIQ) && mtmp->data->mlet != S_DOG) /* Unique monsters cannot be tamed, except Cerberos -- JG */
        /* monsters with conflicting structures cannot be tamed */
        || mtmp->isshk || mtmp->isgd || mtmp->ispriest || mtmp->issmith || mtmp->isnpc /* shopkeepers, guards, and priests cannot be forced to be tame for now -- JG */
        || (!forcetaming && 
            (mtmp->isminion /* minions cannot be tamed, not sure why this is --JG */
            || is_covetous(mtmp->data)
            || is_human(mtmp->data)
            || (is_demon(mtmp->data) && !is_demon(youmonst.data))
            || (obj && dogfood(mtmp, obj) >= MANFOOD)
            )
           )
       )
        return FALSE;

    if (mtmp->m_id == quest_status.leader_m_id)
        return FALSE;

    /* add the pet extension */
    if (obj && mtmp->data == &mons[PM_CERBERUS] && mtmp->heads_tamed < mtmp->heads_left)
    {
        mtmp->heads_tamed++;
    }
    else if(!has_edog)
    {
        newedog(mtmp);
        initedog(mtmp, !charm_type);
    }
    else if (!charm_type)
    {
        mtmp->mtame = is_domestic(mtmp->data) ? 10 : 5;
        mtmp->mpeaceful = 1;

    }

    if (charm_type == 1)
    {
        (void)set_mon_property_b(mtmp, CHARMED, !duration ? -1 : duration, verbose);
    }
    else if (charm_type == 2)
    {
        (void)set_mon_property_b(mtmp, UNDEAD_CONTROL, !duration ? -1 : duration, verbose);
    }
    else if(is_tame(mtmp) && !was_tame)
    {
        newsym_with_flags(mtmp->mx, mtmp->my, NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS);
    }

    if (obj) 
    { /* thrown food */
        int headnum = (int)min(mtmp->heads_left, mtmp->heads_tamed);
        if (mtmp->data == &mons[PM_CERBERUS] && (headnum > 0 && headnum <= 3))
        {
            const char* headstr[3] = { "first", "second", "third" };
            pline("%s %s head %s %s and seems to appreciate it a lot.", s_suffix(Monnam(mtmp)), headstr[headnum - 1],
                thrown ? "catches" : "takes", yname(obj));
        }
        else if (!thrown)
        {
            pline("%s takes %s and seems to appreciate it a lot.", Monnam(mtmp), yname(obj));
        }

        /* defer eating until the edog extension has been set up */
        place_object(obj, mtmp->mx, mtmp->my); /* put on floor */
        /* devour the food (might grow into larger, genocided monster) */
        if (dog_eat(mtmp, obj, mtmp->mx, mtmp->my, TRUE) == 2)
            return TRUE; /* oops, it died... */
        /* `obj' is now obsolete */
    }

    newsym_with_flags(mtmp->mx, mtmp->my, NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS);
    if (attacktype(mtmp->data, AT_WEAP))
    {
        mtmp->weapon_strategy = NEED_HTH_WEAPON;
        (void) mon_wield_item(mtmp, FALSE, 0, 0);
    }
    return TRUE;
}

void
break_charm(mtmp, verbose)
struct monst* mtmp;
boolean verbose;
{
    /* break charm */
    if (has_charmed(mtmp))
    {
        if(verbose)
            (void)set_mon_property_verbosely(mtmp, CHARMED, -3);
        else
            (void)set_mon_property(mtmp, CHARMED, -3);
    }
    if (has_undead_control(mtmp))
    {
        if (verbose)
            (void)set_mon_property_verbosely(mtmp, UNDEAD_CONTROL, -3);
        else
            (void)set_mon_property(mtmp, UNDEAD_CONTROL, -3);
    }
}

/*
 * Called during pet revival or pet life-saving.
 * If you killed the pet, it revives wild.
 * If you abused the pet a lot while alive, it revives wild.
 * If you abused the pet at all while alive, it revives untame.
 * If the pet wasn't abused and was very tame, it might revive tame.
 */
void
wary_dog(mtmp, was_dead)
struct monst *mtmp;
boolean was_dead;
{
    struct edog *edog;
    boolean quietly = was_dead;

    finish_meating(mtmp);

    if (!mtmp->mtame)
        return;
    edog = !mtmp->isminion ? EDOG(mtmp) : 0;

    /* if monster was starving when it died, undo that now */
    if (edog && edog->mhpmax_penalty) 
    {
        edog->mhpmax_penalty = 0;
        update_mon_maxhp(mtmp);
    }

    if (edog && (edog->killed_by_u == 1 || edog->abuse > 2)) 
    {
        mtmp->mpeaceful = mtmp->mtame = 0;
        if (!mtmp->mtame)
            mtmp->ispartymember = 0;

        if (edog->abuse >= 0 && edog->abuse < 10)
            if (!rn2(edog->abuse + 1))
                mtmp->mpeaceful = 1;

        if (!quietly && cansee(mtmp->mx, mtmp->my)) 
        {
            if (haseyes(youmonst.data)) 
            {
                if (haseyes(mtmp->data))
                    pline("%s %s to look you in the %s.", Monnam(mtmp),
                          is_peaceful(mtmp) ? "seems unable" : "refuses",
                          body_part(EYE));
                else
                    pline("%s avoids your gaze.", Monnam(mtmp));
            }
        }
    }
    else
    {
        /* chance it goes wild anyway - Pet Sematary */
        if(mtmp->mtame)
            mtmp->mtame = rn2(mtmp->mtame + 1);
        if (!mtmp->mtame)
            mtmp->mpeaceful = rn2(2);
    }

    if (!mtmp->mtame)
    {
        if (!quietly && canspotmon(mtmp))
            pline("%s %s.", Monnam(mtmp),
                  is_peaceful(mtmp) ? "is no longer tame" : "has become feral");
        newsym(mtmp->mx, mtmp->my);
        /* a life-saved monster might be leashed;
           don't leave it that way if it's no longer tame */
        if (mtmp->mleashed)
            m_unleash(mtmp, TRUE);
        if (mtmp == u.usteed)
            dismount_steed(DISMOUNT_THROWN);
    }
    else if (edog)
    {
        /* it's still a pet; start a clean pet-slate now */
        edog->revivals++;
        edog->killed_by_u = 0;
        edog->abuse = 0;
        edog->ogoal.x = edog->ogoal.y = -1;
        if (was_dead || edog->hungrytime < monstermoves + 500L)
            edog->hungrytime = monstermoves + 500L;
        if (was_dead)
        {
            edog->droptime = 0L;
            edog->dropdist = 10000;
            edog->whistletime = 0L;
            edog->apport = 5;
        } /* else lifesaved, so retain current values */
    }
}

void
abuse_dog(mtmp)
struct monst *mtmp;
{
    if (!mtmp->mtame)
        return;

    if (Aggravate_monster || Conflict || is_crazed(mtmp))
        mtmp->mtame /= 2;
    else
        mtmp->mtame--;

    if (!mtmp->mtame)
        mtmp->ispartymember = FALSE;
    
    if (mtmp->mtame && !mtmp->isminion)
        EDOG(mtmp)->abuse++;

    if (!mtmp->mtame && mtmp->mleashed)
        m_unleash(mtmp, TRUE);

    /* don't make a sound if pet is in the middle of leaving the level */
    /* newsym isn't necessary in this case either */
    if (mtmp->mx != 0) {
        if (mtmp->mtame && rn2(mtmp->mtame))
            yelp(mtmp);
        else
            growl(mtmp); /* give them a moment's worry */

        if (!mtmp->mtame)
            newsym(mtmp->mx, mtmp->my);
    }
}

void
reset_dogs(VOID_ARGS)
{
    petname_used = 0;
}

/*dog.c*/
