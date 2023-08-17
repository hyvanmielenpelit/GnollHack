/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-07 */

/* GnollHack 4.0    makemon.c    $NHDT-Date: 1556150377 2019/04/24 23:59:37 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.134 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <ctype.h>

/* this assumes that a human quest leader or nemesis is an archetype
   of the corresponding role; that isn't so for some roles (tourist
   for instance) but is for the priests and monks we use it for... */
#define quest_mon_represents_role(mptr, role_pm) \
    (mptr->mlet == S_HUMAN && Role_if(role_pm)   \
     && (mptr->msound == MS_LEADER || mptr->msound == MS_NEMESIS))

STATIC_DCL boolean FDECL(ungeneratable_monster_type, (int));
STATIC_DCL int FDECL(align_shift, (struct permonst *));
STATIC_DCL boolean FDECL(mk_gen_ok, (int, UCHAR_P, unsigned long, BOOLEAN_P));
#if 0
STATIC_DCL void FDECL(m_initgrp, (struct monst *, int, int, int, int));
#endif
STATIC_DCL void FDECL(m_initweap, (struct monst *));
STATIC_DCL void FDECL(m_initinv, (struct monst *));
STATIC_DCL void FDECL(m_init_background, (struct monst*));
STATIC_DCL struct obj* FDECL(m_inityour, (struct monst*, struct obj*)); 
STATIC_DCL boolean FDECL(makemon_rnd_goodpos, (struct monst *,
                                               unsigned long, coord *));

#if 0
#define m_initsgrp(mtmp, x, y, mmf) m_initgrp(mtmp, x, y, 3, mmf)
#define m_initlgrp(mtmp, x, y, mmf) m_initgrp(mtmp, x, y, 10, mmf)
#endif
#define toostrong(monindx, lev) (mons[monindx].difficulty > lev)
#define tooweak(monindx, lev) (mons[monindx].difficulty < lev)


boolean
is_home_elemental(ptr)
struct permonst *ptr;
{
    if (!ptr)
        return FALSE;

    if (ptr->mlet == S_ELEMENTAL)
        switch (monsndx(ptr)) {
        case PM_ELDER_AIR_ELEMENTAL:
        case PM_AIR_ELEMENTAL:
            return Is_airlevel(&u.uz);
        case PM_ELDER_FIRE_ELEMENTAL:
        case PM_FIRE_ELEMENTAL:
            return Is_firelevel(&u.uz);
        case PM_ELDER_EARTH_ELEMENTAL:
        case PM_EARTH_ELEMENTAL:
            return Is_earthlevel(&u.uz);
        case PM_ELDER_WATER_ELEMENTAL:
        case PM_WATER_ELEMENTAL:
            return Is_waterlevel(&u.uz);
        }
    return FALSE;
}

/*
 * Return true if the given monster cannot exist on this elemental level.
 */
boolean
wrong_elem_type(ptr)
struct permonst *ptr;
{
    if (ptr->mlet == S_ELEMENTAL) {
        return (boolean) !is_home_elemental(ptr);
    } else if (Is_earthlevel(&u.uz)) {
        /* no restrictions? */
    } else if (Is_waterlevel(&u.uz)) {
        /* just monsters that can swim */
        if (!is_swimmer(ptr))
            return TRUE;
    } else if (Is_firelevel(&u.uz)) {
        if (!has_innate(ptr, MR_FIRE))
            return TRUE;
    } else if (Is_airlevel(&u.uz)) {
        if (!(is_flyer(ptr) && ptr->mlet != S_TRAPPER) && !is_floater(ptr)
            && !amorphous(ptr) && !is_incorporeal(ptr) && !is_whirly(ptr))
            return TRUE;
    }
    return FALSE;
}

#if 0 /* OBSOLETE, replaced by encounter system --JG */
/* make a group just like mtmp */
STATIC_OVL void
m_initgrp(mtmp, x, y, n, mmflags)
struct monst *mtmp;
int x, y, n, mmflags;
{
    coord mm;
    register int cnt = rnd(n);
    struct monst *mon;
#if defined(__GNUC__) && (defined(HPUX) || defined(DGUX))
    /* There is an unresolved problem with several people finding that
     * the game hangs eating CPU; if interrupted and restored, the level
     * will be filled with monsters.  Of those reports giving system type,
     * there were two DG/UX and two HP-UX, all using gcc as the compiler.
     * hcroft@hpopb1.cern.ch, using gcc 2.6.3 on HP-UX, says that the
     * problem went away for him and another reporter-to-newsgroup
     * after adding this debugging code.  This has almost got to be a
     * compiler bug, but until somebody tracks it down and gets it fixed,
     * might as well go with the "but it went away when I tried to find
     * it" code.
     */
    int cnttmp, cntdiv;

    cnttmp = cnt;
    debugpline4("init group call <%d,%d>, n=%d, cnt=%d.", x, y, n, cnt);
    cntdiv = ((u.ulevel < 3) ? 4 : (u.ulevel < 5) ? 2 : 1);
#endif
    /* Tuning: cut down on swarming at low character levels [mrs] */
    cnt /= (u.ulevel < 3) ? 4 : (u.ulevel < 5) ? 2 : 1;
#if defined(__GNUC__) && (defined(HPUX) || defined(DGUX))
    if (cnt != (cnttmp / cntdiv)) {
        pline("cnt=%d using %d, cnttmp=%d, cntdiv=%d", cnt,
              (u.ulevel < 3) ? 4 : (u.ulevel < 5) ? 2 : 1, cnttmp, cntdiv);
    }
#endif
    if (!cnt)
        cnt++;
#if defined(__GNUC__) && (defined(HPUX) || defined(DGUX))
    if (cnt < 0)
        cnt = 1;
    if (cnt > 10)
        cnt = 10;
#endif

    mm.x = x;
    mm.y = y;
    while (cnt--)
    {
        if (peace_minded(mtmp->data))
            continue;
        /* Don't create groups of peaceful monsters since they'll get
         * in our way.  If the monster has a percentage chance so some
         * are peaceful and some are not, the result will just be a
         * smaller group.
         */
        if (enexto(&mm, mm.x, mm.y, mtmp->data))
        {
            mon = makemon(mtmp->data, mm.x, mm.y, (mmflags | MM_NOGRP));
            if (mon)
            {
                mon->mpeaceful = FALSE;
                mon->mavenge = 0;
                set_mhostility(mon);
                /* Undo the second peace_minded() check in makemon(); if the
                 * monster turned out to be peaceful the first time we
                 * didn't create it at all; we don't want a second check.
                 */
            }
        }
    }
}
#endif

void
m_initthrow(mtmp, otyp, oquan_const, oquan_rnd, poisoned, elemental_enchantment, exceptionality, material)
struct monst *mtmp;
int otyp, oquan_const, oquan_rnd;
boolean poisoned;
int elemental_enchantment, exceptionality, material;
{
    register struct obj *otmp;

    otmp = mksobj_with_flags(otyp, TRUE, FALSE, FALSE, mtmp, material, 0L, 0L, 0UL);
    if(!(oquan_const == 0 && oquan_rnd == 0))
        otmp->quan = (long) rn1(oquan_rnd, oquan_const);
    otmp->owt = weight(otmp);
    if (is_poisonable(otmp) && poisoned)
        otmp->opoisoned = TRUE;
    if(is_elemental_enchantable(otmp) && elemental_enchantment >= 0)
        otmp->elemental_enchantment = elemental_enchantment;
    if (can_have_exceptionality(otmp) && exceptionality >= 0)
        otmp->exceptionality = exceptionality;
    (void) mpickobj(mtmp, otmp);
}

long
mongetsgold(mon, amount)
struct monst* mon;
long amount;
{
    if (amount <= 0)
        return 0;

    struct obj* otmp = mksobj(GOLD_PIECE, FALSE, FALSE, FALSE);
    if (otmp)
    {
        otmp->quan = amount;
        otmp->owt = weight(otmp);
        (void)mpickobj(mon, otmp);
        return amount;
    }
    return 0;
}

STATIC_OVL void
m_init_background(mtmp)
register struct monst* mtmp;
{
    if (!mtmp)
        return;

    char mnamebuf[BUFSZ];

    if (has_enpc(mtmp) && npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_fixed_name != 0)
        christen_monst(mtmp, npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_fixed_name);
    else if (is_mname_proper_name(mtmp->data))
        return;
    else if (mtmp->mnum == PM_ORACLE && !has_mname(mtmp))
        christen_monst(mtmp, "Pythia");
    else if (is_dwarf(mtmp->data) && !(mtmp->data->geno & G_UNIQ) && !has_mname(mtmp))
        christen_monst(mtmp, upstart(randomize_dwarf_name(mnamebuf)));
    else if (is_orc(mtmp->data) && !(mtmp->data->geno & G_UNIQ) && !has_mname(mtmp))
        christen_monst(mtmp, upstart(rndorcname(mnamebuf)));
    else if (is_elf(mtmp->data) && !(mtmp->data->geno & G_UNIQ) && !has_mname(mtmp))
        christen_monst(mtmp, upstart(randomize_elf_name(mnamebuf)));
    else if (is_gnome(mtmp->data) && !(mtmp->data->geno & G_UNIQ) && !has_mname(mtmp))
        christen_monst(mtmp, upstart(randomize_gnome_name(mnamebuf)));
    else if (is_undead(mtmp->data) && (mtmp->data->mlet == S_LICH || mtmp->data == &mons[PM_DEATH_FLAYER] || mtmp->data == &mons[PM_VAMPIRE_MAGE]) && !(mtmp->data->geno & G_UNIQ) && !has_mname(mtmp))
        christen_monst(mtmp, upstart(randomize_undead_spellcaster_name(mnamebuf)));
    else if ((mtmp->data == &mons[PM_ANGEL] || mtmp->data == &mons[PM_ARCHON]) && !has_mname(mtmp))
        christen_monst(mtmp, upstart(randomize_angel_name(mnamebuf)));
    else if (is_modron(mtmp->data) && !has_mname(mtmp))
        christen_monst(mtmp, upstart(randomize_modron_name(mnamebuf)));
    else if (is_gnoll(mtmp->data) && !has_mname(mtmp))
    {
        if (mtmp->data == &mons[PM_FLIND] || mtmp->data == &mons[PM_FLIND_LORD])
            christen_monst(mtmp, upstart(randomize_flind_name(mnamebuf)));
        else
            christen_monst(mtmp, upstart(randomize_gnoll_name(mnamebuf)));
    }
    else if (is_demon(mtmp->data) && !(mtmp->data->geno & G_UNIQ) && !has_mname(mtmp))
        christen_monst(mtmp, upstart(randomize_demon_name(mnamebuf)));
    else if ((is_human(mtmp->data) || is_quantum_mechanic(mtmp->data)) && !has_mname(mtmp))
    {
        if (mtmp->female)
            christen_monst(mtmp, upstart(randomize_female_human_name(mnamebuf)));
        else
            christen_monst(mtmp, upstart(randomize_male_human_name(mnamebuf)));
    }
    else if (mtmp->data == &mons[PM_HALFLING] && !has_mname(mtmp))
        christen_monst(mtmp, upstart(randomize_hobbit_name(mnamebuf)));
}


STATIC_OVL 
struct obj*
m_inityour(mtmp, obj)
struct monst* mtmp;
struct obj* obj;
{
    if (!mtmp || !obj)
        return 0;

    struct obj* otmp = 0;

    int otyp = obj->otyp;
    boolean generate_random_item = FALSE;
    Strcpy(debug_buf_1, "m_inityour");

    if (obj->oartifact)
    {
        if (objects[obj->otyp].oc_prob == 0 || (objects[obj->otyp].oc_flags3 & O3_NO_GENERATION))
        {
            if(!(objects[artilist[obj->oartifact].maskotyp].oc_prob == 0 || (objects[artilist[obj->oartifact].maskotyp].oc_flags3 & O3_NO_GENERATION)))
                otyp = artilist[obj->oartifact].maskotyp;
            else
            {
                generate_random_item = TRUE;
                otyp = random_objectid_from_class(obj->oclass, mtmp, 0UL);
            }
        }
    }
    else
    {
        if (objects[obj->otyp].oc_prob == 0 || (objects[obj->otyp].oc_flags3 & O3_NO_GENERATION))
        {
            generate_random_item = TRUE;
            otyp = random_objectid_from_class(obj->oclass, mtmp, 0UL);
        }
    }

    otmp = mksobj(otyp, FALSE, FALSE, 0);
    if (otmp)
    {
        if (obj->blessed)
            bless(otmp);
        else if (obj->cursed)
            curse(otmp);

        otmp->quan = obj->quan;
        if (!generate_random_item)
        {
            otmp->opoisoned = obj->opoisoned;
            otmp->mythic_prefix = obj->mythic_prefix;
            otmp->mythic_suffix = obj->mythic_suffix;
            otmp->elemental_enchantment = obj->elemental_enchantment;
            otmp->oerodeproof = TRUE;
            otmp->enchantment = obj->enchantment;
            otmp->charges = obj->charges;
            otmp->speflags |= SPEFLAGS_CLONED_ITEM; /* This item will disappear when Aleax dies / is gone */
        }
        otmp->owt = weight(otmp);
        (void)mpickobj(mtmp, otmp);
    }

    return otmp;
}

STATIC_OVL void
m_initweap(mtmp)
register struct monst *mtmp;
{
    if (!mtmp)
        return;

    register struct permonst *ptr = mtmp->data;
    register int mm = mtmp->mnum;
    struct obj *otmp;
    int bias, spe2, w1, w2;

    if (Is_really_rogue_level(&u.uz))
        return;

    if (has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_NO_ITEMS) != 0)
        return;

    /*
     *  First a few special cases:
     *          giants get a boulder to throw sometimes
     *          ettins get clubs
     *          kobolds get darts to throw
     *          centaurs get some sort of bow & arrows or bolts
     *          soldiers get all sorts of things
     *          kops get clubs & cream pies.
     */
    switch (ptr->mlet)
    {
    case S_GNOME:
        /* Moved to init_inv to account for gnomish bankers */
        break;
    case S_GNOLL:
        switch (mm) 
        {
        case PM_GNOLL_SUPREME_WARDEN:
        case PM_GNOLL_WARDEN:
            if (rn2(7))
                (void)mongets(mtmp, GNOLLISH_HAIRCLOTH_ROBE);
            if (!rn2(3))
                (void)mongets(mtmp, FLAIL);
            else if (!rn2(2))
                (void)mongets_with_material(mtmp, DAGGER, MAT_BONE);
            break;
        case PM_GNOLL:
            if (rn2(7))
            {
                if(rn2(3))
                    (void)mongets(mtmp, GNOLLISH_LEATHER_ARMOR);
                else
                    (void)mongets(mtmp, GNOLLISH_STUDDED_LEATHER_ARMOR);
            }
            if (rn2(3))
            {
                (void)mongets(mtmp, GNOLLISH_HOOD);
            }
            if (!rn2(3))
                (void)mongets(mtmp, FLAIL);
            if (!rn2(5))
            {
                (void)mongets(mtmp, CROSSBOW);
                m_initthrow(mtmp, GNOLLISH_QUARREL, 6, 20, !rn2(20), -1, -1, MAT_NONE);
            }
            break;
        case PM_GNOLL_LORD:
            if (rn2(7))
            {
                (void)mongets(mtmp, GNOLLISH_STUDDED_LEATHER_ARMOR);
            }
            if (rn2(7))
            {
                (void)mongets(mtmp, GNOLLISH_HOOD);
            }
            if (!rn2(2))
            {
                if (!rn2(6))
                    (void)mongets(mtmp, DOUBLE_HEADED_FLAIL);
                else
                    (void)mongets(mtmp, FLAIL);
            }
            if (!rn2(3))
            {
                if(!(rn2(4)))
                    (void)mongets(mtmp, HEAVY_CROSSBOW);
                else
                    (void)mongets(mtmp, CROSSBOW);

                m_initthrow(mtmp, GNOLLISH_QUARREL, 11, 15, !rn2(10), -1, -1, MAT_NONE);
            }
            break;
        case PM_GNOLL_KING:
            (void)mongets(mtmp, GNOLLISH_STUDDED_LEATHER_ARMOR);
            if (!rn2(20))
                (void)mongets(mtmp, TRIPLE_HEADED_FLAIL);
            else if (rn2(3))
                (void)mongets(mtmp, DOUBLE_HEADED_FLAIL);
            else
                (void)mongets(mtmp, FLAIL);
            (void)mongets(mtmp, HEAVY_CROSSBOW);
            m_initthrow(mtmp, GNOLLISH_QUARREL, 21, 10, !rn2(5), -1, -1, MAT_NONE);
            break;
        case PM_FLIND:
            if (!rn2(2))
                (void)mongets(mtmp, FLINDBAR);
            else if (!rn2(15))
                (void)mongets(mtmp, TRIPLE_HEADED_FLAIL);
            else
                (void)mongets(mtmp, DOUBLE_HEADED_FLAIL);
            break;
        case PM_FLIND_LORD:
            (void)mongets(mtmp, FLINDBAR);
            break;
        default:
            break;
        }
        break;
    case S_GIANT:
        switch (mm)
        {
        case PM_MINOTAUR:
            break;
        case PM_HELL_BOVINE:
        {
            int bovine_weapons[] = { HALBERD, HALBERD, ANCUS,
                RANSEUR, SPETUM, GLAIVE, BARDICHE, VOULGE, FAUCHARD, GUISARME,
                BILL_GUISARME, LUCERN_HAMMER, BEC_DE_CORBIN };

            (void)mongets(mtmp, bovine_weapons[rn2(SIZE(bovine_weapons))]);
            break;
        }
        case PM_STORM_GIANT:
        {
            otmp = mksobj(JAVELIN, TRUE, FALSE, FALSE);
            if (otmp) 
            {
                otmp->elemental_enchantment = LIGHTNING_ENCHANTMENT;
                otmp->enchantment = rn2(4);
                otmp->quan = rnd(6);
                otmp->blessed = 0;
                otmp->cursed = 0;
                otmp->owt = weight(otmp);
                (void)mpickobj(mtmp, otmp);
            }
            break;
        }
        default:
            if (rn2(2))
                (void)mongets(mtmp, (mm != PM_ETTIN) ? BOULDER : CLUB);

            break;
        }
        break;
    case S_HUMAN:
        if (is_mercenary(ptr))
        {
            w1 = w2 = 0;
            switch (mm) 
            {
            case PM_WATCHMAN:
            case PM_SOLDIER:
                if (!rn2(3))
                {
                    w1 = rn1(BEC_DE_CORBIN - PARTISAN + 1, PARTISAN);
                    w2 = rn2(2) ? DAGGER : KNIFE;
                } else
                    w1 = rn2(2) ? SPEAR : SHORT_SWORD;
                break;
            case PM_SERGEANT:
                w1 = rn2(2) ? FLAIL : MACE;
                break;
            case PM_LIEUTENANT:
                w1 = rn2(2) ? BROADSWORD : LONG_SWORD;
                break;
            case PM_CAPTAIN:
            case PM_WATCH_CAPTAIN:
                w1 = rn2(2) ? LONG_SWORD : SILVER_SABER;
                break;
            default:
                if (!rn2(4))
                    w1 = DAGGER;
                if (!rn2(7))
                    w2 = SPEAR;
                break;
            }
            if (w1)
                (void) mongets(mtmp, w1);
            if (!w2 && w1 != DAGGER && !rn2(4))
                w2 = KNIFE;
            if (w2)
                (void) mongets(mtmp, w2);
        }
        else if (is_elf(ptr)) 
        {
            if (rn2(2))
                (void) mongets(mtmp,
                               rn2(2) ? ELVEN_MITHRIL_COAT : ELVEN_CLOAK);
            if (rn2(2))
                (void) mongets(mtmp, ELVEN_HELM);
            else if (!rn2(4))
                (void) mongets(mtmp, ELVEN_BOOTS);
            if (rn2(2))
                (void) mongets(mtmp, ELVEN_DAGGER);
            switch (rn2(3))
            {
            case 0:
                if (!rn2(4))
                    (void) mongets(mtmp, ELVEN_SHIELD);
                if (rn2(3))
                    (void) mongets(mtmp, ELVEN_SHORT_SWORD);
                (void) mongets(mtmp, ELVEN_LONG_BOW);
                m_initthrow(mtmp, ELVEN_ARROW, 10, 12, FALSE, -1, -1, MAT_NONE);
                break;
            case 1:
                (void) mongets(mtmp, ELVEN_BROADSWORD);
                if (rn2(2))
                    (void) mongets(mtmp, ELVEN_SHIELD);
                break;
            case 2:
                if (rn2(2)) 
                {
                    (void) mongets(mtmp, ELVEN_SPEAR);
                    (void) mongets(mtmp, ELVEN_SHIELD);
                }
                break;
            }
            if (mm == PM_ELVENKING) 
            {
                if (rn2(3) || (in_mklev && Is_earthlevel(&u.uz)))
                    (void) mongets(mtmp, PICK_AXE);
                if (!rn2(50))
                    (void) mongets(mtmp, CRYSTAL_BALL);
            }
        } 
        else if (ptr->msound == MS_PRIEST
                   || quest_mon_represents_role(ptr, PM_PRIEST)) 
        {
            otmp = mksobj_with_flags(MACE, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            if (otmp)
            {
                if (has_epri(mtmp) && mm == PM_HIGH_PRIEST)
                {
                    otmp->enchantment = max(otmp->enchantment, rnd(5) + 5);
                    otmp->exceptionality = EXCEPTIONALITY_ELITE;
                }
                else
                {
                    otmp->enchantment = rnd(3);
                    if (!rn2(2))
                        curse(otmp);
                }
                (void) mpickobj(mtmp, otmp);
            }
        } 
        else if (mm == PM_NINJA)
        { /* extra quest villains */
            (void) mongets(mtmp, rn2(4) ? SHURIKEN : DART);
            (void) mongets(mtmp, rn2(4) ? SHORT_SWORD : AXE);
        }
        else if (ptr->msound == MS_GUARDIAN) 
        {
            /* quest "guardians" */
            switch (mm)
            {
            case PM_STUDENT:
            case PM_ATTENDANT:
            case PM_ABBOT:
            case PM_ACOLYTE:
            case PM_GUIDE:
            case PM_APPRENTICE:
                if (rn2(2))
                    (void) mongets(mtmp, rn2(3) ? DAGGER : KNIFE);
                if (rn2(5))
                    (void) mongets(mtmp, rn2(3) ? LEATHER_JACKET
                                                : LEATHER_CLOAK);
                if (rn2(3))
                    (void) mongets(mtmp, rn2(3) ? LOW_BOOTS : HIGH_BOOTS);
                if (rn2(3))
                    (void) mongets(mtmp, POT_HEALING);
                break;
            case PM_CHIEFTAIN:
            case PM_PAGE:
            case PM_ROSHI:
            case PM_WARRIOR:
                (void) mongets(mtmp, rn2(3) ? LONG_SWORD : SHORT_SWORD);
                (void) mongets(mtmp, rn2(3) ? CHAIN_MAIL : LEATHER_ARMOR);
                if (rn2(2))
                    (void) mongets(mtmp, rn2(2) ? LOW_BOOTS : HIGH_BOOTS);
                if (!rn2(3))
                    (void) mongets(mtmp, LEATHER_CLOAK);
                if (!rn2(3)) 
                {
                    (void) mongets(mtmp, SHORT_BOW);
                    m_initthrow(mtmp, ARROW, 10, 12, FALSE, -1, -1, MAT_NONE);
                }
                break;
            case PM_HUNTER:
                (void) mongets(mtmp, rn2(3) ? SHORT_SWORD : DAGGER);
                if (rn2(2))
                    (void) mongets(mtmp, rn2(2) ? LEATHER_JACKET
                                                : LEATHER_ARMOR);
                (void) mongets(mtmp, LONG_BOW);
                m_initthrow(mtmp, ARROW, 10, 12, FALSE, -1, -1, MAT_NONE);
                break;
            case PM_THUG:
                (void) mongets(mtmp, CLUB);
                (void) mongets(mtmp, rn2(3) ? DAGGER : KNIFE);
                if (rn2(2))
                    (void) mongets(mtmp, LEATHER_GLOVES);
                (void) mongets(mtmp, rn2(2) ? LEATHER_JACKET : LEATHER_ARMOR);
                break;
            case PM_NEANDERTHAL:
                (void) mongets(mtmp, CLUB);
                (void) mongets(mtmp, LEATHER_ARMOR);
                break;
            }
        }
        break;

    case S_ANGEL:
        if (humanoid(ptr)) 
        {
            /* create minion stuff; can't use mongets */
            if (ptr == &mons[PM_ALEAX])
            {
                /* Adapted from UnNetHack, but the items will disappear upon Aleax's death; you can still steal them before that */
                /* mimic your weapon */
                if (uwep && (is_weapon(uwep)))
                    (void)m_inityour(mtmp, uwep);
                else
                {
                    otmp = mongets(mtmp, LONG_SWORD);
                    if (otmp)
                    {
                        bless(otmp);
                        otmp->oerodeproof = TRUE;
                        otmp->elemental_enchantment = FIRE_ENCHANTMENT; /* Make it a bit more street-credible */
                    }
                }

                if (uarms && (is_weapon(uarms) || is_shield(uarms)))
                    (void)m_inityour(mtmp, uarms);
                else if(!(uwep && is_weapon(uwep) && bimanual(uwep)) && !rn2(4))
                    (void)mongets(mtmp, SHIELD_OF_REFLECTION);

                if (uswapwep && (is_weapon(uswapwep)))
                    (void)m_inityour(mtmp, uswapwep);

                if (uswapwep2 && (is_weapon(uswapwep2) || is_shield(uswapwep2)))
                    (void)m_inityour(mtmp, uswapwep2);

                if (uquiver && (is_ammo(uquiver) || is_missile(uquiver)))
                    (void)m_inityour(mtmp, uquiver);

                /* mimic your armor */
                if (uarm) 
                    (void)m_inityour(mtmp, uarm);
                if (uarmc) 
                    (void)m_inityour(mtmp, uarmc);
                if (uarmh) 
                    (void)m_inityour(mtmp, uarmh);
                if (uarms) 
                    (void)m_inityour(mtmp, uarms);
                if (uarmg) 
                    (void)m_inityour(mtmp, uarmg);
                if (uarmf) 
                    (void)m_inityour(mtmp, uarmf);
                if (uarmu) 
                    (void)m_inityour(mtmp, uarmu);
                if (uarmo)
                    (void)m_inityour(mtmp, uarmo);
                if (uarmb)
                    (void)m_inityour(mtmp, uarmb);
            }
            else
            {
                aligntyp alignment = mon_aligntyp(mtmp);
                int aligntyp_major_weapon = LONG_SWORD;
                int aligntyp_minor_weapon = LONG_SWORD;
                unsigned long weapon_flags = MKOBJ_FLAGS_FORCE_BASE_MATERIAL;
                uchar material = MAT_NONE;
                boolean is_major_weapon = !rn2(3) || is_lord(ptr) || is_prince(ptr);
                switch (alignment)
                {
                case A_LAWFUL:
                    aligntyp_major_weapon = SWORD_OF_HOLY_VENGEANCE;
                    aligntyp_minor_weapon = LONG_SWORD;
                    material = is_major_weapon ? MAT_NONE : MAT_SILVER;
                    break;
                case A_NEUTRAL:
                    aligntyp_major_weapon = LONG_SWORD;
                    aligntyp_minor_weapon = LONG_SWORD;
                    weapon_flags = is_major_weapon ? MKOBJ_FLAGS_FORCE_MYTHIC_OR_LEGENDARY : 0UL;
                    material = MAT_SILVER;
                    break;
                case A_CHAOTIC:
                    aligntyp_major_weapon = SWORD_OF_UNHOLY_DESECRATION;
                    aligntyp_minor_weapon = SCIMITAR;
                    break;
                case A_NONE:
                    aligntyp_major_weapon = SCIMITAR;
                    aligntyp_minor_weapon = SCIMITAR;
                    weapon_flags = is_major_weapon ? MKOBJ_FLAGS_FORCE_MYTHIC_OR_LEGENDARY : 0UL;
                    break;
                default:
                    break;
                }
                int weaptype = is_major_weapon ? aligntyp_major_weapon : !rn2(3) ? LONG_SWORD : aligntyp_minor_weapon;
                otmp = mksobj_with_flags(weaptype, FALSE, FALSE, FALSE, mtmp, material, 0L, 0L, weapon_flags);
                if (otmp)
                {
                    if (otmp->oartifact == 0)
                    {
                        if (otmp->exceptionality == EXCEPTIONALITY_NORMAL)
                            otmp->exceptionality = is_prince(ptr) || is_lord(ptr) || !rn2(4) ? EXCEPTIONALITY_CELESTIAL : !rn2(3) ? EXCEPTIONALITY_ELITE : !rn2(2) ? EXCEPTIONALITY_EXCEPTIONAL : EXCEPTIONALITY_NORMAL;

                        if (weaptype != SWORD_OF_HOLY_VENGEANCE && weaptype != SWORD_OF_UNHOLY_DESECRATION
                            && otmp->mythic_prefix == 0 && otmp->mythic_suffix == 0
                            && otmp->elemental_enchantment == 0)
                            otmp->elemental_enchantment = FIRE_ENCHANTMENT;
                    }
                    bless(otmp);
                    otmp->oerodeproof = TRUE;
                    spe2 = rnd(4);
                    otmp->enchantment = max(otmp->enchantment, spe2);
                    (void)mpickobj(mtmp, otmp);
                }

                otmp = mksobj_with_flags(!rn2(4) || is_lord(ptr) ? SHIELD_OF_REFLECTION : !rn2(3) ? SPIKED_SHIELD : LARGE_SHIELD,
                    FALSE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
                
                if (otmp)
                {
                    otmp->cursed = FALSE;
                    otmp->oerodeproof = TRUE;
                    otmp->enchantment = 0;
                    (void)mpickobj(mtmp, otmp);
                }
            }
        }
        break;

    case S_HUMANOID:
        if (mm == PM_HALFLING)
        {
            switch (rn2(3))
            {
            case 0:
                (void) mongets(mtmp, DAGGER);
                break;
            case 1:
            {
                int weaptype = ELVEN_RUNEDAGGER;
                int artifacttype = 0;

                if (!rn2(20))
                {
                    artifacttype = ART_STING;

                }

                otmp = mksobj_with_flags(weaptype, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);

                /* maybe make it special */
                if (artifacttype > 0)
                {
                    otmp = oname(otmp, artiname(artifacttype));
                    if(otmp->oartifact)
                    {
                        otmp->oerodeproof = TRUE;
                        spe2 = rnd(4);
                        otmp->enchantment = max(otmp->enchantment, spe2);
                    }
                }
                (void)mpickobj(mtmp, otmp);
                break;
            }
            case 2:
                (void) mongets(mtmp, !rn2(20) ? STAFF_SLING : SLING);
                (void) mongets(mtmp, SLING_BULLET);
                break;
            }
            if (!rn2(10))
                (void) mongets(mtmp, ELVEN_MITHRIL_COAT);
            if (!rn2(10))
                (void) mongets(mtmp, DWARVISH_CLOAK);
            if (!rn2(100))
            {
                int ringtype = RIN_SUPREME_POWER;
                int artifacttype = ART_RULING_RING_OF_YENDOR;

                otmp = mksobj(ringtype, FALSE, FALSE, FALSE);

                if (otmp)
                {
                    /* try to make it special */
                    otmp = oname(otmp, artiname(artifacttype));
                    if (otmp->oartifact)
                    {
                        (void)mpickobj(mtmp, otmp);
                    }
                    else
                    {
                        /* free object */
                        obfree(otmp, (struct obj*) 0);
                    }
                }
            }
        } 
        else if (is_dwarf(ptr))
        {
            if (rn2(7))
                (void) mongets(mtmp, DWARVISH_CLOAK);
            if (rn2(7))
                (void) mongets(mtmp, SHOES);
            if (!rn2(4))
            {
                (void) mongets(mtmp, DWARVISH_SHORT_SWORD);
                /* note: you can't use a mattock with a shield */
                if (rn2(2))
                    (void) mongets(mtmp, DWARVISH_MATTOCK);
                else 
                {
                    (void) mongets(mtmp, rn2(2) ? AXE : DWARVISH_SPEAR);
                    (void) mongets(mtmp, DWARVISH_ROUNDSHIELD);
                }
                (void) mongets(mtmp, DWARVISH_HELM);
                if (!rn2(3))
                    (void) mongets(mtmp, DWARVISH_MITHRIL_COAT);
            } 
            else 
            {
                (void) mongets(mtmp, !rn2(3) ? PICK_AXE : DAGGER);
            }
        }
        break;
    case S_KOP:
        /* create Keystone Kops with cream pies to
           throw. As suggested by KAA.     [MRS] */
        if (!rn2(4))
            m_initthrow(mtmp, CREAM_PIE, 1, 2, FALSE, -1, -1, MAT_NONE);
        if (!rn2(3))
            (void) mongets(mtmp, (rn2(2)) ? CLUB : RUBBER_HOSE);
        break;
    case S_ORC:
        if (rn2(2))
            (void) mongets(mtmp, ORCISH_HELM);
        switch ((mm != PM_ORC_CAPTAIN) ? mm
                : rn2(2) ? PM_BLACK_ORC : PM_GREAT_ORC)
        {
        case PM_BLACK_ORC:
            if (!rn2(3))
                (void) mongets(mtmp, SCIMITAR);
            if (!rn2(3))
                (void) mongets(mtmp, ORCISH_SHIELD);
            if (!rn2(3))
                (void) mongets(mtmp, KNIFE);
            if (!rn2(3))
                (void) mongets(mtmp, ORCISH_CHAIN_MAIL);
            break;
        case PM_GREAT_ORC:
            if (!rn2(3))
                (void) mongets(mtmp, ORCISH_CLOAK);
            if (!rn2(3))
                (void) mongets(mtmp, ORCISH_SHORT_SWORD);
            if (!rn2(3))
                (void) mongets_with_material(mtmp, SHOES, MAT_IRON);
            if (!rn2(3))
            {
                (void) mongets(mtmp, ORCISH_SHORT_BOW);
                m_initthrow(mtmp, ORCISH_ARROW, 10, 12, TRUE, -1, -1, MAT_NONE);
            }
            if (!rn2(3))
                (void) mongets(mtmp, GREAT_ORCISH_SHIELD);
            break;
        case PM_ORC_SHAMAN:
            if (!rn2(2))
            {
                int n = rn2(3); //0...2
                while (n--)
                    (void)mongets(mtmp, random_reagent_otyp(FALSE, FALSE, 0));
            }
            if(!rn2(2))
                (void)mongets(mtmp, GINSENG_ROOT);
            break;
        case PM_GOBLIN_KING:
            if (!rn2(2))
            {
                (void)mongets(mtmp, !rn2(2) ? STAFF_SLING : SLING);
                m_initthrow(mtmp, SLING_BULLET, 8, 12, FALSE, -1, -1, MAT_NONE);
            }
            (void)mongets(mtmp, SCIMITAR);
            break;
        case PM_GOBLIN:
            if (!rn2(20))
            {
                (void)mongets(mtmp, SLING);
                m_initthrow(mtmp, SLING_BULLET, 6, 8, FALSE, -1, -1, MAT_NONE);
            }
            if (rn2(2))
                (void)mongets(mtmp, ORCISH_DAGGER);
            break;
        case PM_HOBGOBLIN:
            if (!rn2(4))
            {
                (void)mongets(mtmp, !rn2(20)? STAFF_SLING : SLING);
                m_initthrow(mtmp, SLING_BULLET, 6, 8, FALSE, -1, -1, MAT_NONE);
            }
            if (rn2(2))
                (void)mongets(mtmp, !rn2(2) ? ORCISH_DAGGER : SCIMITAR);
            break;
        default:
            if (mm != PM_ORC_SHAMAN && rn2(2))
                (void) mongets(mtmp, (mm == PM_GOBLIN || !rn2(2)) ? ORCISH_DAGGER : SCIMITAR);
        }
        break;
    case S_OGRE:
        if (!rn2(3)) 
        {
            if (ptr != &mons[PM_OGRE] && ptr != &mons[PM_OGRE_MAGE] && ptr != &mons[PM_OGRE_ARCHMAGE]) {
                (void)mongets(mtmp, HEAVY_CROSSBOW);
                m_initthrow(mtmp, CROSSBOW_BOLT, 10, 12, FALSE, -1, -1, MAT_NONE);
            }
            (void)mongets(mtmp, CLUB);
        }
        else
        {
            if (!rn2(mm == PM_OGRE_KING ? 3 : mm == PM_OGRE_LORD || mm == PM_OGRE_OVERLORD ? 6 : 12))
                (void)mongets(mtmp, BATTLE_AXE);
            else
                (void)mongets(mtmp, CLUB);
        }
        break;
    case S_TROLL:
        if (humanoid(mtmp->data) && !rn2(2))
        {
            struct obj* weap = 0;
            switch (rn2(4)) {
            case 0:
                weap = mongets(mtmp, RANSEUR);
                break;
            case 1:
                weap = mongets(mtmp, PARTISAN);
                break;
            case 2:
                weap = mongets(mtmp, GLAIVE);
                break;
            case 3:
                weap = mongets(mtmp, SPETUM);
                break;
            }

            /* Ice trolls get enchanted weapons */
            if (mm == PM_ICE_TROLL && weap && is_elemental_enchantable(weap) && weap->elemental_enchantment == 0 && !rn2(2))
            {
                weap->elemental_enchantment = COLD_ENCHANTMENT;
            }
        }
        break;
    case S_KOBOLD:
        if (!rn2(4))
            m_initthrow(mtmp, DART, 5, 12, !rn2(20), -1, -1, MAT_NONE);
        break;

    case S_CENTAUR:
        if (rn2(2)) 
        {
            if (ptr == &mons[PM_FOREST_CENTAUR]) 
            {
                (void) mongets(mtmp, SHORT_BOW);
                m_initthrow(mtmp, ARROW, 10, 12, FALSE, -1, -1, MAT_NONE);
            } 
            else
            {
                (void) mongets(mtmp, CROSSBOW);
                m_initthrow(mtmp, CROSSBOW_BOLT, 10, 12, FALSE, -1, -1, MAT_NONE);
            }
        }
        break;
    case S_WRAITH:
        if (ptr == &mons[PM_WRAITHLORD])
        {
            (void)mongets(mtmp, WRAITHBLADE);
            otmp = mongets(mtmp, LONG_SWORD);
            if (otmp)
                otmp->mythic_prefix = MYTHIC_PREFIX_VAMPIRIC;
        }
        break;
    case S_LESSER_UNDEAD:
        if (!rn2(4))
            (void) mongets(mtmp, LEATHER_ARMOR);
        if (!rn2(4))
            (void) mongets(mtmp, (rn2(3) ? KNIFE : SHORT_SWORD));
        break;
    case S_LIZARD:
        if (mm == PM_SALAMANDER)
            (void) mongets(mtmp,
                           (rn2(7) ? SPEAR : rn2(3) ? TRIDENT : STILETTO));
        break;
    case S_GOLEM:
        if (mm == PM_SILVER_GOLEM)
        {
            for(int i = 0; i < 4; i++)
            {
                int weaptype = SILVER_SABER;
                short artifacttype = ART_GRAYSWANDIR;
                uchar material = 0UL;

                switch (rn2(6))
                {
                case 0:
                case 1:
                case 2:
                    weaptype = SILVER_SABER;
                    artifacttype = ART_GRAYSWANDIR;
                    break;
                case 3:
                    weaptype = LONG_SWORD;
                    artifacttype = ART_DEMONBANE;
                    break;
                case 4:
                    weaptype = DAGGER;
                    artifacttype = 0;
                    material = MAT_SILVER;
                    break;
                case 5:
                    weaptype = MACE;
                    artifacttype = 0;
                    material = MAT_SILVER;
                    break;
                default:
                    break;
                }
                otmp = mksobj_with_flags(weaptype, TRUE, FALSE, FALSE, mtmp, material, 0L, 0L, 0UL);

                /* maybe make it special */
                if (artifacttype > 0 && !rn2(40))
                    otmp = oname(otmp,
                        artiname(artifacttype));

                (void)mpickobj(mtmp, otmp);
            }
        }
        break;
    case S_MODRON:
        if (mm < PM_MODRON_QUINTON && !rn2(6))
        {
            //(void)mongets(mtmp, !rn2(3) ? STAFF_SLING : SLING);
            //m_initthrow(mtmp, SLING_BULLET, 8, 8, FALSE, -1, -1, MAT_NONE);
            goto default_equipment_here;
        }
        break;
    case S_TREANT:
        break;
    case S_DEMON:
        /* prevent djinn and mail daemons from leaving objects when
         * they vanish
         */
        if (!is_demon(ptr))
            break;

        switch (mm) 
        {
        case PM_BALOR:
        {
            otmp = mksobj_with_flags(BULLWHIP, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            spe2 = rnd(5);
            otmp->enchantment = max(otmp->enchantment, spe2);
            otmp->elemental_enchantment = FIRE_ENCHANTMENT;
            otmp->exceptionality = !rn2(3) ? EXCEPTIONALITY_INFERNAL : EXCEPTIONALITY_ELITE;
            (void)mpickobj(mtmp, otmp);

            if (rn2(2))
            {
                otmp = mksobj_with_flags(BROADSWORD, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
                spe2 = rnd(4);
                otmp->enchantment = max(otmp->enchantment, spe2);
                otmp->elemental_enchantment = FIRE_ENCHANTMENT;
                otmp->exceptionality = !rn2(3) ? EXCEPTIONALITY_INFERNAL : EXCEPTIONALITY_ELITE;
                (void)mpickobj(mtmp, otmp);
            }
            else
            {
                otmp = mksobj_with_flags(AXE, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
                spe2 = 1 + rnd(4);
                otmp->enchantment = max(otmp->enchantment, spe2);
                otmp->exceptionality = !rn2(3) ? EXCEPTIONALITY_INFERNAL : EXCEPTIONALITY_ELITE;
                (void)mpickobj(mtmp, otmp);
            }
            break;
        }
        case PM_HORNED_DEVIL:
            (void) mongets(mtmp, rn2(4) ? TRIDENT : BULLWHIP);
            break;
        case PM_BARBED_DEVIL:
            if (!rn2(4))
            {
                (void)mongets(mtmp, !rn2(3) ? STAFF_SLING : SLING);
                m_initthrow(mtmp, SLING_BULLET, 8, 8, FALSE, -1, -1, MAT_NONE);
            }
            break;
        case PM_MARILITH:
            (void)mongets(mtmp, !rn2(5) ? LONG_SWORD : !rn2(4) ? SCIMITAR : !rn2(3) ? SHORT_SWORD : !rn2(2) ? DAGGER : AXE);
            (void)mongets(mtmp, !rn2(5) ? LONG_SWORD : !rn2(4) ? SCIMITAR : !rn2(3) ? SHORT_SWORD : !rn2(2) ? DAGGER : AXE);
            (void)mongets(mtmp, !rn2(5) ? LONG_SWORD : !rn2(4) ? SCIMITAR : !rn2(3) ? SHORT_SWORD : !rn2(2) ? DAGGER : AXE);
            (void)mongets(mtmp, !rn2(5) ? LONG_SWORD : !rn2(4) ? SCIMITAR : !rn2(3) ? SHORT_SWORD : !rn2(2) ? DAGGER : AXE);
            (void)mongets(mtmp, !rn2(5) ? LONG_SWORD : !rn2(4) ? SCIMITAR : !rn2(3) ? SHORT_SWORD : !rn2(2) ? DAGGER : AXE);
            (void)mongets(mtmp, !rn2(5) ? LONG_SWORD : !rn2(4) ? SCIMITAR : !rn2(3) ? SHORT_SWORD : !rn2(2) ? DAGGER : AXE);
            break;
        case PM_PIT_FIEND:
            otmp = mksobj_with_flags(ANCUS, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            curse(otmp);
            spe2 = 0 + rnd(3);
            otmp->enchantment = max(otmp->enchantment, spe2);
            otmp->exceptionality = !rn2(3) ? EXCEPTIONALITY_INFERNAL : EXCEPTIONALITY_ELITE;
            (void)mpickobj(mtmp, otmp);

            otmp = mksobj_with_flags(JAGGED_TOOTHED_CLUB, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            curse(otmp);
            spe2 = 0 + rnd(3);
            otmp->enchantment = max(otmp->enchantment, spe2);
            otmp->exceptionality = !rn2(3) ? EXCEPTIONALITY_INFERNAL : EXCEPTIONALITY_ELITE;
            (void)mpickobj(mtmp, otmp);
            break;
        case PM_BAPHOMET:
            /* Baphomet's bardiche */
            otmp = mksobj_with_flags(BARDICHE, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            curse(otmp);
            otmp->oerodeproof = TRUE;
            spe2 = 3 + rnd(7);
            otmp->enchantment = max(otmp->enchantment, spe2);
            otmp->exceptionality = EXCEPTIONALITY_INFERNAL;
            (void)mpickobj(mtmp, otmp);

            /* And his nose ring, of course */
            (void)mongets(mtmp, !rn2(2) ? NOSE_RING_OF_BULL_STRENGTH : NOSE_RING_OF_BULLHEADEDNESS);

            break;
        case PM_ORCUS:
            otmp = mksobj_with_flags(MACE_OF_THE_UNDERWORLD, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            if(otmp)
                otmp = oname(otmp, artiname(ART_WAND_OF_ORCUS));

            if (otmp)
            {
                otmp->oerodeproof = TRUE;
                spe2 = 2 + rnd(3);
                otmp->enchantment = max(otmp->enchantment, spe2);
                curse(otmp);
                (void)mpickobj(mtmp, otmp);
            }

            if (!otmp || !otmp->oartifact)
            {
                (void)mongets(mtmp, WAN_DEATH);
            }
            break;
        case PM_YEENAGHU:
            otmp = mksobj_with_flags(TRIPLE_HEADED_FLAIL, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            if (otmp)
                otmp = oname(otmp, artiname(ART_TRIPLE_HEADED_FLAIL_OF_YEENAGHU));

            if (otmp)
            {
                otmp->oerodeproof = TRUE;
                spe2 = 2 + rnd(4);
                otmp->enchantment = max(otmp->enchantment, spe2);
                curse(otmp);
                (void)mpickobj(mtmp, otmp);
            }
            break;
        case PM_ASMODEUS:
            otmp = mksobj_with_flags(DIABOLICAL_SCEPTRE, TRUE, FALSE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            if (otmp)
                otmp = oname(otmp, artiname(ART_RUBY_ROD_OF_ASMODEUS));

            if (otmp)
            {
                otmp->oerodeproof = TRUE;
                spe2 = 2 + rnd(4);
                otmp->enchantment = max(otmp->enchantment, spe2);
                curse(otmp);
                (void)mpickobj(mtmp, otmp);
            }
            break;
        default:
            goto default_equipment_here;
        }
        break;
default_equipment_here:
    default:
        /*
         * Now the general case, some chance of getting some type
         * of weapon for "normal" monsters.  Certain special types
         * of monsters will get a bonus chance or different selections.
         */
        bias = is_lord(ptr) + is_prince(ptr) * 2 + extra_nasty(ptr);
        switch (rnd(14 - (2 * bias)))
        {
        case 1:
            if (strongmonst(ptr))
                (void) mongets(mtmp, BATTLE_AXE);
            else
                m_initthrow(mtmp, DART, 5, 12, !rn2(20), -1, -1, MAT_NONE);
            break;
        case 2:
            if (strongmonst(ptr))
                (void) mongets(mtmp, TWO_HANDED_SWORD);
            else 
            {
                (void) mongets(mtmp, CROSSBOW);
                m_initthrow(mtmp, CROSSBOW_BOLT, 10, 12, FALSE, -1, -1, MAT_NONE);
            }
            break;
        case 3:
            (void) mongets(mtmp, SHORT_BOW);
            m_initthrow(mtmp, ARROW, 10, 12, FALSE, -1, -1, MAT_NONE);
            break;
        case 4:
            if (strongmonst(ptr))
                (void) mongets(mtmp, LONG_SWORD);
            else
                m_initthrow(mtmp, DAGGER, 1, 3, FALSE, -1, -1, MAT_NONE);
            break;
        case 5:
            if (strongmonst(ptr))
                (void) mongets(mtmp, LUCERN_HAMMER);
            else
                (void) mongets(mtmp, AKLYS);
            break;
        default:
            break;
        }
        break;
    }

    if ((int) mtmp->m_lev > rn2(75))
        (void) mongets(mtmp, rnd_offensive_item(mtmp));
}

/*
 *   Makes up money for monster's inventory.
 *   This will change with silver & copper coins
 */
void
mkmonmoney(mtmp, amount)
struct monst *mtmp;
long amount;
{
    struct obj *gold = mksobj(GOLD_PIECE, FALSE, FALSE, FALSE);

    gold->quan = amount;
    add_to_minv(mtmp, gold);
}

STATIC_OVL void
m_initinv(mtmp)
register struct monst *mtmp;
{
    if (!mtmp)
        return;

    register int cnt;
    register struct obj *otmp;
    register struct permonst *ptr = mtmp->data;
    int mndx = mtmp->mnum;
    int n = 0;
    int i;
    Sprintf(debug_buf_2, "m_initinv, mnum=%d", mtmp->mnum);

    if (Is_really_rogue_level(&u.uz))
        return;

    if (has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_NO_ITEMS) != 0)
        return;

    /* Add saddle for relevant monsters */
    if (is_steed(mtmp->data) && may_start_with_saddle(mtmp->data) && !rn2(4))
    {
        otmp = mksobj(SADDLE, TRUE, FALSE, FALSE);
        if (otmp)
            put_saddle_on_mon(otmp, mtmp);
    }

    /*
     *  Soldiers get armour & rations - armour approximates their ac.
     *  Nymphs may get mirror or potion of object detection.
     */
    switch (ptr->mlet) 
    {
    case S_ANGEL:
        break;
    case S_DRAGON:
        if (mndx >= PM_GRAY_DRAGON_HATCHLING && mndx <= PM_YELLOW_DRAGON_HATCHLING)
        {
            if (!rn2(4))
            {
                otmp = mkobj(GEM_CLASS, TRUE, 0);
                (void)mpickobj(mtmp, otmp);
            }
        }
        else if (mndx >= PM_GRAY_DRAGON && mndx <= PM_YELLOW_DRAGON)
        {
            if (!rn2(2))
            {
                int num = rnd(3);
                for (i = 0; i < num; i++)
                {
                    otmp = mkobj(GEM_CLASS, TRUE, 0);
                    (void)mpickobj(mtmp, otmp);
                }
            }
        }
        else if (mndx >= PM_ANCIENT_GRAY_DRAGON && mndx <= PM_ANCIENT_YELLOW_DRAGON)
        {
            int num = rnd(4);
            for (i = 0; i < num; i++)
            {
                if (!rn2(2))
                    otmp = mongets(mtmp, randomtruegem());
                else
                {
                    otmp = mkobj(GEM_CLASS, TRUE, 0);
                    (void)mpickobj(mtmp, otmp);
                }
            }
        }
        break;
    case S_DOG:
        if(!rn2(20))
            (void)mongets_noinit_item(mtmp, BONE, 1);
        
        break;
    case S_HUMAN:
        if (is_mercenary(ptr)) {
            register int mac;

            switch (mndx) {
            case PM_GUARD:
                mac = -1;
                if (!rn2(2))
                    (void)mongetsgold(mtmp, 5 + rn2(21));
                break;
            case PM_SOLDIER:
                mac = 3;
                if (!rn2(2))
                    (void)mongetsgold(mtmp, 5 + rn2(21));
                break;
            case PM_SERGEANT:
                mac = 0;
                if (!rn2(2))
                    (void)mongetsgold(mtmp, rn2(41) + 20);
                break;
            case PM_LIEUTENANT:
                mac = -2;
                if (!rn2(2))
                    (void)mongetsgold(mtmp, rn2(81) + 50);
                break;
            case PM_CAPTAIN:
                mac = -3;
                if (!rn2(2))
                    (void)mongetsgold(mtmp, rn2(151) + 75);
                break;
            case PM_WATCHMAN:
                mac = 3;
                if (!rn2(2))
                    (void)mongetsgold(mtmp, 5 + rn2(21));
                break;
            case PM_WATCH_CAPTAIN:
                mac = -2;
                if (!rn2(2))
                    (void)mongetsgold(mtmp, rn2(81) + 50);
                break;
            default:
                impossible("odd mercenary %d?", mndx);
                mac = 0;
                break;
            }

            if (mac < -1 && rn2(5))
                mac += 7 + mongets_return_enchantment(mtmp, PLATE_MAIL);
            else if (mac < 3 && rn2(5))
                mac +=
                6 + mongets_return_enchantment(mtmp, (rn2(3)) ? SPLINT_MAIL : BANDED_MAIL);
            else if (rn2(5))
                mac += 3 + mongets_return_enchantment(mtmp, (rn2(3)) ? RING_MAIL
                    : STUDDED_LEATHER_ARMOR);
            else
                mac += 2 + mongets_return_enchantment(mtmp, LEATHER_ARMOR);

            if (mac < 10 && rn2(3))
                mac += 1 + mongets_return_enchantment(mtmp, HELMET);
            else if (mac < 10 && rn2(2))
                mac += 1 + mongets_return_enchantment(mtmp, DENTED_POT);
            if (mac < 10 && rn2(3))
                mac += 1 + mongets_return_enchantment(mtmp, SMALL_SHIELD);
            else if (mac < 10 && rn2(2))
                mac += 2 + mongets_return_enchantment(mtmp, LARGE_SHIELD);
            if (mac < 10 && rn2(3))
                mac += 1 + mongets_return_enchantment(mtmp, LOW_BOOTS);
            else if (mac < 10 && rn2(2))
                mac += 2 + mongets_return_enchantment(mtmp, HIGH_BOOTS);
            if (mac < 10 && rn2(3))
                mac += 1 + mongets_return_enchantment(mtmp, LEATHER_GLOVES);
            else if (mac < 10 && rn2(2))
                mac += 1 + mongets_return_enchantment(mtmp, LEATHER_CLOAK);

            nhUse(mac); /* suppress 'dead increment' from static analyzer */

            if (ptr == &mons[PM_WATCH_CAPTAIN]) {
                ; /* better weapon rather than extra gear here */
            }
            else if (ptr == &mons[PM_WATCHMAN]) {
                if (rn2(3)) /* most watchmen carry a whistle */
                    (void)mongets(mtmp, TIN_WHISTLE);
            }
            else if (ptr == &mons[PM_GUARD]) {
                /* if hero teleports out of a vault while being confronted
                   by the vault's guard, there is a shrill whistling sound,
                   so guard evidently carries a cursed whistle */
                otmp = mksobj(TIN_WHISTLE, TRUE, FALSE, FALSE);
                curse(otmp);
                (void)mpickobj(mtmp, otmp);
            }
            else { /* soldiers and their officers */
                if (!rn2(3))
                    (void)mongets(mtmp, K_RATION);
                if (!rn2(2))
                    (void)mongets(mtmp, C_RATION);
                if (ptr != &mons[PM_SOLDIER] && !rn2(3))
                    (void)mongets(mtmp, BUGLE);
            }
        }
        else if (ptr == &mons[PM_SHOPKEEPER]) {
            (void)mongets(mtmp, SKELETON_KEY);
            switch (rn2(4)) {
                /* MAJOR fall through ... */
            case 0:
                (void)mongets(mtmp, WAN_MAGIC_MISSILE);
                /*FALLTHRU*/
            case 1:
                (void)mongets(mtmp, POT_EXTRA_HEALING);
                /*FALLTHRU*/
            case 2:
                (void)mongets(mtmp, POT_HEALING);
                /*FALLTHRU*/
            case 3:
                (void)mongets(mtmp, WAN_STRIKING);
            }
        }
        else if (ptr == &mons[PM_ARCHMAGE]) 
        {
            if (!rn2(10))
                (void)mongets(mtmp, ROBE_OF_THE_ARCHMAGI);
            else if (!rn2(4))
                (void)mongets(mtmp, !rn2(2) ? ROBE_OF_MAGIC_RESISTANCE : !rn2(2) ? ROBE_OF_PROTECTION : ROBE_OF_EYES);
            else
                (void)mongets(mtmp, WIZARD_S_ROBE);

            if(!rn2(4))
                (void)mongets(mtmp, BRACERS_OF_DEFENSE);
            else if (!rn2(4))
                (void)mongets(mtmp, !rn2(2) ? BRACERS_OF_REFLECTION : !rn2(2) ? BRACERS_OF_SPELL_CASTING : BRACERS_AGAINST_MAGIC_MISSILES);
            else
                (void)mongets(mtmp, LEATHER_BRACERS);

            if (!rn2(4))
                (void)mongets(mtmp, STAFF_OF_THE_MAGI);
            else if(!rn2(4))
                (void)mongets(mtmp, !rn2(2) ? STAFF_OF_FIRE : !rn2(2) ? STAFF_OF_THUNDER_AND_LIGHTNING : STAFF_OF_FROST);
            else
                (void)mongets(mtmp, QUARTERSTAFF);

            (void)mongets(mtmp, !rn2(2) ? POT_GREATER_HEALING : POT_EXTRA_HEALING);

        }
        else if (ptr->msound == MS_PRIEST
            || quest_mon_represents_role(ptr, PM_PRIEST)) {
            if (ptr == &mons[PM_HIGH_PRIEST])
            {
                otmp = mongets(mtmp, !rn2(2) ? ROBE_OF_STARRY_WISDOM : GOWN_OF_THE_ARCHBISHOPS);
                otmp->enchantment = max(otmp->enchantment, rn2(3));
            }
            else
            {
                (void)mongets(mtmp, rn2(9) ? CLERICAL_GOWN
                    : GOWN_OF_THE_ARCHBISHOPS);
                (void)mongets(mtmp, rn2(7) ? LEATHER_CLOAK
                    : rn2(3) ? CLOAK_OF_PROTECTION
                    : CLOAK_OF_MAGIC_RESISTANCE);
                (void)mongets(mtmp, SMALL_SHIELD);
                mkmonmoney(mtmp, (long)rn1(10, 20));
            }
        }
        else if (quest_mon_represents_role(ptr, PM_MONK)) {
            (void)mongets(mtmp, SIMPLE_GOWN);
            (void)mongets(mtmp, rn2(11) ? LEATHER_CLOAK : CLOAK_OF_MAGIC_RESISTANCE);
        }
        else if (ptr == &mons[PM_ELVENKING])
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, 250 + rn2(501));
        }
        else if (ptr == &mons[PM_ELF_LORD])
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, 50 + rn2(101));
        }
        else if (ptr->mflags2 & M2_ELF)
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, 5 + rn2(11));
        }
        break;

    case S_NYMPH:
        if (!rn2(2))
            (void)mongets(mtmp, MIRROR);
        if (!rn2(2))
            (void)mongets(mtmp, POT_OBJECT_DETECTION);
        if(!rn2(2))
            (void)mongetsgold(mtmp, 10 + rn2(51));
        break;
    case S_GIANT:
        if (ptr == &mons[PM_MINOTAUR])
        {
            if (!rn2(4))
                (void)mongetsgold(mtmp, rn2(251) + 50);

            if (!rn2(3) || (in_mklev && Is_earthlevel(&u.uz)))
                (void)mongets(mtmp, WAN_DIGGING);

            if (!rn2(4))
                (void)mongets(mtmp, !rn2(2) ? NOSE_RING_OF_BULL_STRENGTH : NOSE_RING_OF_BULLHEADEDNESS);
        }
        else if (ptr == &mons[PM_TITAN]) 
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, rn2(501) + 200);
            //Reagents
            n = rn2(3);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, random_reagent_otyp(TRUE, TRUE, 0));

            //Spellbooks
            n = rn2(3); // 0...2
            for (i = 0; i < n; i++)
            {
                otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
                (void)mpickobj(mtmp, otmp);
            }

        }
        else if (is_giant(ptr)) 
        {
            if (!rn2(3))
                (void)mongetsgold(mtmp, 50 + rn2(101));

            for (cnt = rn2((int)(mtmp->m_lev / 2)); cnt; cnt--) {
                otmp = mksobj(rnd_class(DILITHIUM_CRYSTAL, LUCKSTONE - 1),
                    FALSE, FALSE, FALSE);
                otmp->quan = (long)rn1(2, 3);
                otmp->owt = weight(otmp);
                (void)mpickobj(mtmp, otmp);
            }
        }
        break;
    case S_WRAITH:
        if (ptr == &mons[PM_WRAITHLORD]) {
            if (!rn2(20))
            {
                int ringtype = RIN_SUPREME_POWER;
                short artifacttype = ART_RULING_RING_OF_YENDOR;

                otmp = mksobj(ringtype, FALSE, FALSE, FALSE);

                if (otmp)
                {
                    /* try to make it special */
                    otmp = oname(otmp, artiname(artifacttype));
                    if (otmp->oartifact)
                    {
                        (void)mpickobj(mtmp, otmp);
                    }
                    else
                    {
                        /* free object */
                        obfree(otmp, (struct obj*) 0);
                    }
                }
            }
            else if (!carrying(RIN_SUPREME_POWER))
            {
                otmp = mksobj(RIN_INVISIBILITY, FALSE, FALSE, FALSE);
                curse(otmp);
                (void)mpickobj(mtmp, otmp);
            }
        }
        break;
    case S_VAMPIRE:
        if (ptr == &mons[PM_VAMPIRE_MAGE]) {
            if (!rn2(7))
                (void)mongets(mtmp, RIN_REPLENISHMENT);

            if (!rn2(50))
                (void)mongets(mtmp, ROBE_OF_THE_ARCHMAGI);
            else if (!rn2(4))
                (void)mongets(mtmp, rn2(5) ? WIZARD_S_ROBE : rn2(2) ? ROBE_OF_MAGIC_RESISTANCE : ROBE_OF_PROTECTION);

            if (!rn2(4))
                (void)mongets(mtmp, !rn2(5) ? POT_FULL_ENERGY : !rn2(2) ? POT_GREATER_ENERGY : !rn2(3) ? POT_EXTRA_ENERGY : POT_GAIN_ENERGY);
            
            //Reagents
            n = rn2(3);
            for (i = 0; i < n; i++)
            {
                (void)mongets(mtmp, random_reagent_otyp(TRUE, TRUE, 2));
            }

            //Some spellbooks
            n = rn2(3); // 0...2
            for (i = 0; i < n; i++)
            {
                otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
                (void)mpickobj(mtmp, otmp);
            }

        }
        else if (ptr == &mons[PM_VLAD_THE_IMPALER])
        {
            otmp = mksobj_with_flags(SPEAR, TRUE, FALSE, 0, mtmp, MAT_NONE, EXCEPTIONALITY_INFERNAL, 0L, MKOBJ_FLAGS_FORCE_LEGENDARY | MKOBJ_FLAGS_PARAM_IS_EXCEPTIONALITY);
            if (otmp)
            {
                otmp->enchantment = 4 + rn2(4);
                curse(otmp);
                (void)mpickobj(mtmp, otmp);
            }
            (void) mongets(mtmp, POT_FULL_HEALING);
            (void) mongets(mtmp, POT_SPEED);
            (void) mongets(mtmp, AMULET_OF_REFLECTION);
        }
        break;
    case S_LICH:
        //Weapons
        if (ptr == &mons[PM_MASTER_LICH] && !rn2(13))
            (void)mongets(mtmp, (rn2(7) ? ATHAME : WAN_NOTHING));
        else if (ptr == &mons[PM_ARCH_LICH] && !rn2(3)) {
            otmp = mksobj_with_flags(rn2(3) ? ATHAME : QUARTERSTAFF, TRUE,
                rn2(13) ? FALSE : TRUE, FALSE, mtmp, MAT_NONE, 0L, 0L, 0UL);
            if (otmp->enchantment < 2)
                otmp->enchantment = rnd(3);
            if (!rn2(4))
                otmp->oerodeproof = 1;
            (void)mpickobj(mtmp, otmp);
        }

        //Bracers
        if (!rn2(10))
            (void)mongets(mtmp, !rn2(75) ? BRACERS_OF_REFLECTION : rn2(2) ? BRACERS_OF_DEFENSE : rn2(2) ? BRACERS_AGAINST_MAGIC_MISSILES : LEATHER_BRACERS);

        //Ring
        if (!rn2(10))
            (void)mongets(mtmp, RIN_REPLENISHMENT);

        //Potion of gain energy
        if (!rn2(3))
            (void)mongets(mtmp, !rn2(4) ? POT_FULL_ENERGY : !rn2(2) ? POT_GREATER_ENERGY : !rn2(3) ? POT_EXTRA_ENERGY : POT_GAIN_ENERGY);

        //Reagents
        n = rn2(3) + (ptr == &mons[PM_MASTER_LICH] || ptr == &mons[PM_ARCH_LICH] ? 1 : 0); //1...3 + 2
        for (i = 0; i < n; i++)
            (void)mongets(mtmp, random_reagent_otyp(TRUE, TRUE, 2));

        //Some spellbooks
        if (ptr == &mons[PM_ARCH_LICH])
            n = rn2(5); // 0...4
        else if (ptr == &mons[PM_MASTER_LICH])
            n = rn2(4); // 0...3
        else
            n = rn2(3); // 0...2

        for (i = 0; i < n; i++)
        {
            otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
            (void)mpickobj(mtmp, otmp);
        }

        //Possibly robes
        if (ptr == &mons[PM_MASTER_LICH] || ptr == &mons[PM_ARCH_LICH])
        {
            if (!rn2(20))
                (void)mongets(mtmp, !rn2(2) ? ROBE_OF_THE_ARCHMAGI : ROBE_OF_MAGIC_RESISTANCE);
            else
                (void)mongets(mtmp, rn2(5) ? WIZARD_S_ROBE : ROBE_OF_PROTECTION);

        } else if(!rn2(4))
            (void)mongets(mtmp, rn2(20) ? WIZARD_S_ROBE : ROBE_OF_PROTECTION);

        if (!rn2(4))
            (void)mongetsgold(mtmp, 100 + rn2(701));

        break;
    case S_HUMANOID:
        //Some spellbooks
        n = 0;
        if (ptr == &mons[PM_DEATH_FLAYER])
        {
            //Bracers
            if (!rn2(7))
                (void)mongets(mtmp, rn2(2) ? BRACERS_OF_DEFENSE : rn2(2) ? BRACERS_AGAINST_MAGIC_MISSILES : LEATHER_BRACERS);

            //Ring
            if (!rn2(7))
                (void)mongets(mtmp, RIN_REPLENISHMENT);

            //Reagents
            n = rn2(4);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, random_reagent_otyp(TRUE, TRUE, 2));

            if (!rn2(4))
                (void)mongetsgold(mtmp, 100 + rn2(1001));

            n = rn2(3); // 0...2
        }
        else if (ptr == &mons[PM_ELDER_TENTACLED_ONE])
        {
            if (!rn2(4))
                (void)mongetsgold(mtmp, 100 + rn2(1001));

            n = rn2(4); // 0...3
        }
        else if (ptr == &mons[PM_TENTACLED_ONE])
        {
            if (!rn2(4))
                (void)mongetsgold(mtmp, 50 + rn2(251));

            n = rn2(2); // 0...1
        }
        else if (ptr == &mons[PM_HALFLING])
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, 5 + rn2(11));

            if (!rn2(12))
            {
                otmp = mksobj_with_flags(SPE_MANUAL, TRUE, FALSE, FALSE, mtmp, MAT_NONE, MANUAL_CATALOGUE_OF_COMESTIBLES, 0L, MKOBJ_FLAGS_PARAM_IS_TITLE);
                if (otmp)
                    (void)mpickobj(mtmp, otmp);
            }

        }
        else if (ptr == &mons[PM_DWARF])
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, 5 + rn2(26));

        }
        else if (ptr == &mons[PM_DWARF_LORD])
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, 10 + rn2(51));
        }
        else if (ptr == &mons[PM_DWARF_KING])
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, 100 + rn2(401));
        }
        else if (ptr == &mons[PM_BUGBEAR])
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, 10 + rn2(21));
        }

        if (is_dwarf(ptr))
        {
            /* Essential gear */
            if (!rn2(20))
                (void)mongets(mtmp, WAN_ORE_DETECTION);

            /* Food and drink */
            if (!rn2(4))
                (void)mongets(mtmp, CRAM_RATION);
            if (!rn2(4))
                (void)mongets(mtmp, POT_DWARVEN_MUSHROOM_BREW);
            if (!rn2(12))
                (void)mongets(mtmp, POT_DWARVEN_MUSHROOM_BREW);

        }

        if (n > 0)
        {
            for (i = 0; i < n; i++)
            {
                otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
                (void)mpickobj(mtmp, otmp);
            }
        }

        break;
    case S_KOBOLD:
        /* All kobolds have candles, which they are burning */
        /* Some of them have a back-up candle */
        if (!rn2(2))
        {
            otmp = mksobj(rn2(4) ? TALLOW_CANDLE : WAX_CANDLE, TRUE, FALSE, FALSE);
            otmp->quan = 1;
            otmp->owt = weight(otmp);
            if (!mpickobj(mtmp, otmp) && !levl[mtmp->mx][mtmp->my].lit)
                begin_burn(otmp, FALSE);
        }

        if (!rn2(2))
            (void)mongetsgold(mtmp, ptr == &mons[PM_KOBOLD_LORD] ? 10 + rn2(21) : ptr == &mons[PM_LARGE_KOBOLD] ? 5 + rn2(16) : 5 + rn2(6));


        /* Kobold lords have lots of candles */
        if (ptr == &mons[PM_KOBOLD_LORD] || (!rn2(4) && ptr == &mons[PM_KOBOLD_SHAMAN]))
        {
            /* Maybe more than one as a back-up */
            otmp = mksobj(!rn2(100) ? MAGIC_CANDLE : rn2(4) ? TALLOW_CANDLE : WAX_CANDLE, TRUE, FALSE, FALSE);
            (void)mpickobj(mtmp, otmp);
        }

        if (ptr == &mons[PM_KOBOLD_SHAMAN])
        {
            if (!rn2(6))
                (void)mongets(mtmp, !rn2(50) ? POT_FULL_ENERGY : !rn2(10) ? POT_GREATER_ENERGY : !rn2(3) ? POT_EXTRA_ENERGY : POT_GAIN_ENERGY);
            
            /* Some random reagants */
            n = rn2(2);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, random_reagent_otyp(TRUE, FALSE, 0));

            if (!rn2(4))
                (void)mongets(mtmp, GINSENG_ROOT);
            
            /* Some spellbooks */
            if (!rn2(2))
            {
                n = rn2(2);
                for (i = 0; i < n; i++)
                {
                    otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
                    (void)mpickobj(mtmp, otmp);
                }
            }
        }

        break;
    case S_GREATER_UNDEAD:
        if (rn2(7))
            (void)mongets(mtmp, MUMMY_WRAPPING);

        if (ptr == &mons[PM_GREATER_MUMMY_PRIEST])
        {
            /* Some random reagants */
            n = rn2(5);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, random_reagent_otyp(TRUE, TRUE, 1));

            n = rn2(2);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, GINSENG_ROOT);
        }
        else if (ptr == &mons[PM_GREATER_MUMMY_HIGH_PRIEST])
        {
            /* Some random reagants */
            n = rn2(7);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, random_reagent_otyp(TRUE, TRUE, 1));

            n = rn2(2);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, GINSENG_ROOT);
        }
        break;
    case S_YETI:
        if (ptr == &mons[PM_OWLBEAR_MATRIARCH])
        {
            /* Some random reagants */
            n = rn2(5);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, random_reagent_otyp(TRUE, Inhell, 1));

            n = rn2(2);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, GINSENG_ROOT);
        }
        else if (ptr == &mons[PM_OWLBEAR_SHAMAN])
        {
            /* Some random reagants */
            n = rn2(3);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, random_reagent_otyp(TRUE, Inhell, 1));

            n = rn2(2);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, GINSENG_ROOT);
        }
        break;
    case S_QUANTMECH:
        if (!rn2(5))
        {
            otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
            (void)mpickobj(mtmp, otmp);
        }
        if (!rn2(ptr == &mons[PM_ELDER_QUANTUM_MECHANIC] ? 5: 20)) {
            struct obj* catcorpse;

            otmp = mksobj(LARGE_BOX, FALSE, FALSE, FALSE);
            /* we used to just set the flag, which resulted in weight()
               treating the box as being heavier by the weight of a cat;
               now we include a cat corpse that won't rot; when opening or
               disclosing the box's contents, the corpse might be revived,
               otherwise it's given a rot timer; weight is now ordinary */
            if ((catcorpse = mksobj(CORPSE, TRUE, FALSE, FALSE)) != 0) {
                otmp->speflags |= SPEFLAGS_SCHROEDINGERS_BOX; /* flag for special SchroedingersBox */
                set_corpsenm(catcorpse, PM_HOUSECAT);
                (void)stop_timer(ROT_CORPSE, obj_to_any(catcorpse));
                add_to_container(otmp, catcorpse);
                otmp->owt = weight(otmp);
            }
            (void)mpickobj(mtmp, otmp);
        }
        break;
    case S_LEPRECHAUN:
        mkmonmoney(mtmp, (long)d(level_difficulty(), 30));
        break;
    case S_UNICORN:
        break;
    case S_DEMON:
        /* moved here from m_initweap() because these don't
           have AT_WEAP so m_initweap() is not called for them */
        if (ptr == &mons[PM_ICE_DEVIL] && !rn2(4)) {
            (void)mongets(mtmp, SPEAR);
        }
        else if (ptr == &mons[PM_YACC])
        {
            /* Yacc is normally using an artifact ring as the nose ring */

            int weaptype = NOSE_RING_OF_BULL_STRENGTH;
            short artifacttype = 0;
            boolean three_wishes_ok = TRUE;
            boolean serpent_ring_ok = TRUE;
            boolean conflict_ok = TRUE;
            boolean true_ring_ok = TRUE;

            if (exist_artifact(RIN_THREE_CHARGES, artiname(ART_RING_OF_THREE_WISHES)))
                three_wishes_ok = FALSE;
            if (exist_artifact(RIN_THE_SERPENT_GOD, artiname(ART_SERPENT_RING_OF_SET)))
                serpent_ring_ok = FALSE;
            if (exist_artifact(RIN_SEVEN_CHARGES, artiname(ART_RING_OF_CONFLICT)))
                conflict_ok = FALSE;
            if (exist_artifact(RIN_SUPREME_POWER, artiname(ART_RULING_RING_OF_YENDOR)))
                true_ring_ok = FALSE;

            if (three_wishes_ok)
            {
                weaptype = RIN_THREE_CHARGES;
                artifacttype = ART_RING_OF_THREE_WISHES;
            }

            if (serpent_ring_ok && (!three_wishes_ok || !rn2(2)))
            {
                weaptype = RIN_THE_SERPENT_GOD;
                artifacttype = ART_SERPENT_RING_OF_SET;
            }

            if (conflict_ok && ((!three_wishes_ok && !serpent_ring_ok) || !rn2(1 + three_wishes_ok + serpent_ring_ok)))
            {
                weaptype = RIN_SEVEN_CHARGES;
                artifacttype = ART_RING_OF_CONFLICT;
            }

            /* Ruling Ring of Yendor is rare with Yacc */
            if (true_ring_ok && !three_wishes_ok && !serpent_ring_ok && !conflict_ok && !rn2(2))
            {
                weaptype = RIN_SUPREME_POWER;
                artifacttype = ART_RULING_RING_OF_YENDOR;
            }
            otmp = mksobj(weaptype, TRUE, FALSE, FALSE);

            if (otmp && artifacttype > 0)
                otmp = oname(otmp, artiname(artifacttype));

            if (otmp)
                (void)mpickobj(mtmp, otmp);
        }
        else if ((is_lord(mtmp->data) || is_prince(mtmp->data)) && !rn2(3))
        {
            if (has_neck(mtmp->data) && !nohands(mtmp->data))
                (void)mongets(mtmp, !rn2(2) ? AMULET_OF_REFLECTION : BRACERS_OF_REFLECTION);
            else if(has_neck(mtmp->data))
                (void)mongets(mtmp, AMULET_OF_REFLECTION);
            else if(!nohands(mtmp->data))
                (void)mongets(mtmp, BRACERS_OF_REFLECTION);
        }
        break;
    case S_ORC:
        if (!rn2(2))
            (void)mongetsgold(mtmp, ptr == &mons[PM_ORC_CAPTAIN] ? 20 + rn2(201) : ptr == &mons[PM_GREAT_ORC] ? 10 + rn2(31) : 5 + rn2(16));

        if (ptr == &mons[PM_ORC_SHAMAN])
        {
            if (!rn2(5))
                (void)mongets(mtmp, !rn2(30) ? POT_FULL_ENERGY : !rn2(8) ? POT_GREATER_ENERGY : !rn2(3) ? POT_EXTRA_ENERGY : POT_GAIN_ENERGY);

            if (!rn2(3))
            {
                otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
                (void)mpickobj(mtmp, otmp);
            }
        }
        break;
    case S_GNOLL:
        if (ptr == &mons[PM_FLIND])
        {
            if (!rn2(3))
                (void)mongets(mtmp, WAN_LIGHTNING);
        }
        else if (ptr == &mons[PM_FLIND_LORD])
        {
            if (!rn2(2))
                (void)mongets(mtmp, WAN_LIGHTNING);
        }
        else if (ptr == &mons[PM_GNOLL_WARDEN] || ptr == &mons[PM_GNOLL_SUPREME_WARDEN])
        {
                //Potion of gain energy
                if (!rn2(3))
                    (void)mongets(mtmp, !rn2(10) ? POT_FULL_ENERGY : !rn2(5) ? POT_GREATER_ENERGY : !rn2(3) ? POT_EXTRA_ENERGY : POT_GAIN_ENERGY);

                n = rn2(3);
                for (i = 0; i < n; i++)
                    (void)mongets(mtmp, random_reagent_otyp(FALSE, Inhell, 1));

                if (ptr == &mons[PM_GNOLL_SUPREME_WARDEN])
                {
                    n = rn2(3);
                    for (i = 0; i < n; i++)
                        (void)mongets(mtmp, GINSENG_ROOT);
                }
                else
                {
                    if (!rn2(2))
                        (void)mongets(mtmp, GINSENG_ROOT);

                }
                
                n = rn2(2);
                for (i = 0; i < n; i++)
                {
                    otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
                    (void)mpickobj(mtmp, otmp);
                }

                if (!rn2(6))
                    (void)mongets(mtmp, WAN_MAGIC_MISSILE);
        }
        else if (ptr == &mons[PM_GNOLL] || ptr == &mons[PM_GNOLL_LORD] || ptr == &mons[PM_GNOLL_KING])
        {
            if (!rn2(2))
                (void)mongetsgold(mtmp, ptr == &mons[PM_GNOLL_KING] ? rn2(301) + 75 : ptr == &mons[PM_GNOLL_LORD] ? rn2(41) + 10 : 5 + rn2(11));
        }
        break;

    case S_GNOME:
        if (!rn2(125)) /* Gnomish banker */
        {
            struct obj* sack = mksobj(!rn2(20) ? BAG_OF_TREASURE_HAULING : SACK, FALSE, FALSE, FALSE);
            (void)mpickobj(mtmp, sack);
            otmp = mksobj(GOLD_PIECE, FALSE, FALSE, FALSE);
            otmp->quan =  ptr == &mons[PM_GNOME_KING] ? rn2(1501) + 1000 : ptr == &mons[PM_GNOME_LORD] ? rn2(751) + 500 : rn2(376) + 250;
            otmp->owt = weight(otmp);
            (void)add_to_container(sack, otmp);
            sack->owt = weight(sack);
            boolean gothat = FALSE;
            if (!rn2(2))
            {
                (void)mongets(mtmp, ptr == &mons[PM_GNOME_KING] && rn2(17) ? ROYAL_CROWN : SILK_TOP_HAT);
                gothat = TRUE;
            }
            if (!gothat || !rn2(2))
                (void)mongets(mtmp, TAILORED_SILK_ROBE);
            if(!rn2(4))
                (void)mongets(mtmp, CREDIT_CARD);
        }
        else /* Normal gnome */
        {
            if (!rn2(3))
                (void)mongetsgold(mtmp, ptr == &mons[PM_GNOME_KING] ? rn2(301) + 100 : ptr == &mons[PM_GNOME_LORD] ? 10 + rn2(31) : 5 + rn2(16));

            if (mtmp->mnum == PM_GNOMISH_WIZARD)
            {
                if (!rn2(5))
                {
                    if (!rn2(20))
                        (void)mongets(mtmp, !rn2(2) ? CORNUTHAUM : DUNCE_CAP);
                    else
                        (void)mongets(mtmp, GNOMISH_FELT_HAT);
                }
            }
            else
            {
                if (!rn2(8))
                {
                    if(ptr == &mons[PM_GNOME_KING] && rn2(17))
                        (void)mongets(mtmp, ROYAL_CROWN);
                    else if (!rn2(20))
                        (void)mongets(mtmp, DUNCE_CAP);
                    else
                        (void)mongets(mtmp, GNOMISH_FELT_HAT);
                }
            }
            switch (mtmp->mnum) 
            {
            case PM_GNOMISH_WIZARD:
                if (!rn2(3))
                    (void)mongets(mtmp, DAGGER);
                break;
            case PM_GNOME_KING:
                if (!rn2(5))
                    (void)mongets(mtmp, rnd_offensive_item(mtmp));
                break;
            }
        }
        if (ptr == &mons[PM_GNOMISH_WIZARD])
        {
            n = rn2(3);
            for (i = 0; i < n; i++)
                (void)mongets(mtmp, random_reagent_otyp(TRUE, FALSE, 2));

            if (!rn2(2))
                (void)mongets(mtmp, GINSENG_ROOT);
            
            n = rn2(2);
            for (i = 0; i < n; i++)
            {
                otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
                (void)mpickobj(mtmp, otmp);
            }

            //Potion of gain energy
            if (!rn2(5))
                (void)mongets(mtmp, !rn2(20) ? POT_FULL_ENERGY : !rn2(7) ? POT_GREATER_ENERGY : !rn2(3) ? POT_EXTRA_ENERGY : POT_GAIN_ENERGY);

            if (!rn2(6))
                (void)mongets(mtmp, WAN_CREATE_MONSTER);
        }
        if (!rn2((In_mines(&u.uz) && in_mklev) ? 20 : 60)) 
        {
            otmp = mksobj(rn2(4) ? TALLOW_CANDLE : WAX_CANDLE, TRUE, FALSE, FALSE);
            otmp->quan = 1;
            otmp->owt = weight(otmp);
            if (!mpickobj(mtmp, otmp) && !levl[mtmp->mx][mtmp->my].lit)
                begin_burn(otmp, FALSE);
        }
        break;
    case S_OGRE:
        if (!rn2(2))
            (void)mongetsgold(mtmp, 
                ptr == &mons[PM_OGRE_ARCHMAGE] ? 100 + rn2(401) :
                ptr == &mons[PM_OGRE_KING] ? 200 + rn2(701) :
                ptr == &mons[PM_OGRE_OVERLORD] ? 100 + rn2(201) :
                ptr == &mons[PM_OGRE_MAGE] ? 100 + rn2(201) :
                ptr == &mons[PM_OGRE_LORD] ? 50 + rn2(101) :
                25 + rn2(51));

        //Potion of gain energy
        if (ptr == &mons[PM_OGRE_MAGE])
        {
            if (!rn2(4))
                (void)mongets(mtmp, !rn2(5) ? POT_FULL_ENERGY : !rn2(2) ? POT_GREATER_ENERGY : !rn2(3) ? POT_EXTRA_ENERGY : POT_GAIN_ENERGY);
        }
        else if (ptr == &mons[PM_OGRE_ARCHMAGE])
        {
            if (!rn2(3))
                (void)mongets(mtmp, !rn2(3) ? POT_FULL_ENERGY : !rn2(2) ? POT_GREATER_ENERGY : !rn2(3) ? POT_EXTRA_ENERGY : POT_GAIN_ENERGY);
        }

        break;
    default:
        break;
    }

    switch (mndx)
    {
    case PM_GOBLIN_KING:
    case PM_OGRE_KING:
    case PM_DWARF_KING:
    case PM_ELVENKING:
    case PM_GNOLL_KING:
        if (!rn2(3) && !m_carrying(mtmp, CROWN_OF_RULERSHIP) && !m_carrying(mtmp, ROYAL_CROWN))
            (void)mongets(mtmp, !rn2(25) ? CROWN_OF_RULERSHIP : ROYAL_CROWN);
        if (!rn2(5))
            (void)mongets(mtmp, rnd_offensive_item(mtmp));
        break;
    default:
        break;
    }

    /* ordinary soldiers rarely have access to magic (or gold :-) */
    if (ptr == &mons[PM_SOLDIER] && rn2(13))
        return;

    if (!rn2(2) && (int) mtmp->m_lev > rn2(50))
        (void) mongets(mtmp, rnd_defensive_item(mtmp));
    if ((int) mtmp->m_lev > rn2(100))
        (void) mongets(mtmp, rnd_misc_item(mtmp));
    if (likes_gold(ptr) && !findgold(mtmp->minvent) && !rn2(5))
        mkmonmoney(mtmp,
                   (long) d(level_difficulty(), mtmp->minvent ? 5 : 10));
}

/* Note: for long worms, always call cutworm (cutworm calls clone_mon) */
struct monst *
clone_mon(mon, x, y, origin_at_mon)
struct monst *mon;
xchar x, y; /* clone's preferred location or 0 (near mon) */
boolean origin_at_mon;
{
    coord mm;
    struct monst *m2;

    /* may be too weak or have been extinguished for population control */
    if (mon->mhp <= 1 || mon->mbasehpmax <= 1 || mon->mhpmax <= 1 || (mvitals[mon->mnum].mvflags & MV_EXTINCT))
        return (struct monst *) 0;

    if (x == 0)
    {
        mm.x = mon->mx;
        mm.y = mon->my;
        if (!enexto(&mm, mm.x, mm.y, mon->data) || MON_AT(mm.x, mm.y))
            return (struct monst *) 0;
    }
    else if (!isok(x, y))
    {
        return (struct monst *) 0; /* paranoia */
    }
    else
    {
        mm.x = x;
        mm.y = y;
        if (MON_AT(mm.x, mm.y))
        {
            if (!enexto(&mm, mm.x, mm.y, mon->data) || MON_AT(mm.x, mm.y))
                return (struct monst *) 0;
        }
    }
    m2 = newmonst();
    *m2 = *mon; /* copy condition of old monster */
    m2->mextra = (struct mextra *) 0;
    m2->nmon = fmon;
    fmon = m2;
    m2->m_id = context.ident++;
    if (!m2->m_id)
        m2->m_id = context.ident++; /* ident overflowed */
    m2->mx = mm.x;
    m2->my = mm.y;
    if (origin_at_mon)
    {
        m2->mx0 = mon->mx;
        m2->my0 = mon->my;
    }

    m2->mundetected = 0;
    m2->mtrapped = 0;
    m2->mcloned = 1;
    m2->minvent = (struct obj *) 0; /* objects don't clone */
    m2->mleashed = FALSE;
    /* Max HP the same, but current HP halved for both.  The caller
     * might want to override this by halving the max HP also.
     * When current HP is odd, the original keeps the extra point.
     * We know original has more than 1 HP, so both end up with at least 1.
     */
    m2->mhpmax = mon->mhpmax;
    m2->mbasehpmax = mon->mbasehpmax;
    m2->mbasehpdrain = mon->mbasehpdrain;
    m2->mhp = mon->mhp / 2;
    mon->mhp -= m2->mhp;

    for (int i = 0; i < A_MAX; i++)
    {
        m2->acurr.a[i] = mon->acurr.a[i];
        m2->abonus.a[i] = mon->abonus.a[i];
        m2->afixmin.a[i] = mon->afixmin.a[i];
        m2->afixmax.a[i] = mon->afixmax.a[i];
        m2->amin.a[i] = mon->amin.a[i];
        m2->amax.a[i] = mon->amax.a[i];
        m2->atemp.a[i] = mon->atemp.a[i];
        m2->atime.a[i] = mon->atime.a[i];
    }
    m2->mdaminc = mon->mdaminc;
    m2->mhitinc = mon->mhitinc;
    m2->macbonus = mon->macbonus;
    m2->mmcbonus = mon->mmcbonus;

    /* since shopkeepers and guards will only be cloned if they've been
     * polymorphed away from their original forms, the clone doesn't have
     * room for the extra information.  we also don't want two shopkeepers
     * around for the same shop.
     */
    if (mon->isshk)
        m2->isshk = FALSE;
    if (mon->isgd)
        m2->isgd = FALSE;
    if (mon->ispriest)
        m2->ispriest = FALSE;
    if (mon->issmith)
        m2->issmith = FALSE;
    if (mon->isnpc)
        m2->isnpc = FALSE;
    place_monster(m2, m2->mx, m2->my);

    if (emitted_light_range(m2->data))
        new_light_source(m2->mx, m2->my, emitted_light_range(m2->data), LS_MONSTER, monst_to_any(m2), 0);

    if (mon_ambient_sound(m2->data))
        new_sound_source(m2->mx, m2->my, mon_ambient_sound(m2->data), (double)mon_ambient_volume(m2->data), SOUNDSOURCE_MONSTER, mon_ambient_subtype(m2->data),
            monst_to_any(m2));

    if (has_mname(mon))
    {
        m2 = christen_monst(m2, MNAME(mon));
    }
    else if (mon->isshk)
    {
        m2 = christen_monst(m2, shkname(mon));
    }

    /* not all clones caused by player are tame or peaceful */
    if (!context.mon_moving) 
    {
        if (mon->mtame)
            m2->mtame = rn2(max(2 + u.uluck, 2)) ? mon->mtame : 0;
        else if (mon->mpeaceful)
            m2->mpeaceful = rn2(max(2 + u.uluck, 2)) ? 1 : 0;
    }

    newsym(m2->mx, m2->my); /* display the new monster */
    if (m2->mtame)
    {
        if (mon->isminion)
        {
            newemin(m2);
            if (has_emin(mon))
                *(EMIN(m2)) = *(EMIN(mon));
        }
        else
        {
            /* because m2 is a copy of mon it is tame but not init'ed.
             * however, tamedog will not re-tame a tame dog, so m2
             * must be made non-tame to get initialized properly.
             */
            m2->mtame = 0;
            if (tamedog(m2, (struct obj*) 0, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, FALSE, FALSE))
            {
                if(has_edog(m2) && has_edog(mon))
                    *(EDOG(m2)) = *(EDOG(mon));
            }
        }
    }
    else
        m2->isminion = FALSE;

    set_mhostility(m2);

    if (!m2->mtame)
        m2->ispartymember = FALSE;
    else
        m2->ispartymember = mon->ispartymember;


    return m2;
}

/*
 * Propagate a species
 *
 * Once a certain number of monsters are created, don't create any more
 * at random (i.e. make them extinct).  The previous (3.2) behavior was
 * to do this when a certain number had _died_, which didn't make
 * much sense.
 *
 * Returns FALSE propagation unsuccessful
 *         TRUE  propagation successful
 */
boolean
propagate(mndx, tally, ghostly)
int mndx;
boolean tally;
boolean ghostly;
{
    boolean result;
    uchar lim = mbirth_limit(mndx);
    boolean gone = (mvitals[mndx].mvflags & (uchar)MV_GONE) != 0; /* geno'd|extinct */

    result = (((int) mvitals[mndx].born < lim) && !gone) ? TRUE : FALSE;

    /* if it's unique, don't ever make it again; except for revivers */
    if ((mons[mndx].geno & G_UNIQ) && mndx != PM_HIGH_PRIEST)
        mvitals[mndx].mvflags |= (uchar)MV_EXTINCT;

    if (mvitals[mndx].born < 255 && tally
        && (!ghostly || (ghostly && result)))
        mvitals[mndx].born++;
    if ((int) mvitals[mndx].born >= lim && !(mons[mndx].geno & (uchar)G_NOGEN)
        && !(mvitals[mndx].mvflags & (uchar)MV_EXTINCT)) {
        if (wizard) {
            debugpline1("Automatically extinguished %s.",
                        pm_plural_name(&mons[mndx], 1));
        }
        mvitals[mndx].mvflags |= (uchar)MV_EXTINCT;
        reset_rndmonst(mndx);
    }
    return result;
}

/* amount of HP to lose from level drain (or gain from Stormbringer) */
double
monhp_per_lvl(mon)
struct monst *mon;
{
    //struct permonst *ptr = mon->data;
    double hp = (double)rnd(8) + constitution_hp_bonus(m_acurr(mon, A_CON)); /* default is d8 */

    if (!mon->m_lev) 
    {
        /* level 0 monsters use 1d4 instead of Nd8 */
        hp = rnd(4) + (int)(constitution_hp_bonus(m_acurr(mon, A_CON)) / 2.0);
    }

    if (hp < 1)
        hp = 1;

    return hp;
}

int
monbasehp_per_lvl(mon)
struct monst* mon;
{
    if (!mon)
        return 1;

    int hp = rnd(8); /* default is d8 */

    if (!mon->m_lev) {
        /* level 0 monsters use 1d4 instead of Nd8 */
        hp = rnd(4);
    }

    if (hp < 1)
        hp = 1;

    return hp;
}

double
monhpadj_per_lvl(mon)
struct monst* mon;
{
    double hp = constitution_hp_bonus(m_acurr(mon, A_CON)); /* default is d8 */
    
    if (!mon->m_lev) {
        /* level 0 monsters use 1d4 instead of Nd8 */
        hp = constitution_hp_bonus(m_acurr(mon, A_CON)) / 2;
    }

    if (hp < 0)
        hp = 0;

    return hp;
}

/* set up a new monster's initial level and hit points;
   used by newcham() as well as by makemon() */
void
newmonhp(mon, mndx, level_adjustment, mmflags)
struct monst *mon;
int mndx, level_adjustment;
unsigned long mmflags;
{
    struct permonst *ptr = &mons[mndx];
    boolean use_maxhp = !!(mmflags & MM_MAX_HP);
    boolean use_normalhd = !!(mmflags & MM_NORMAL_HIT_DICE);
    boolean dragonmaxhp = !!(ptr->mlet == S_DRAGON && mndx >= PM_GRAY_DRAGON && In_endgame(&u.uz));
    int hp = 0;
    int basemaxhp = 0;

    mon->m_lev = use_normalhd ? ptr->mlevel : adj_lev(ptr, level_adjustment);

    if (mon->m_lev <= 0) 
    {
        basemaxhp = 4; // +constitution_hp_bonus(m_acurr(mon, A_CON)) / 2;
        hp = use_maxhp || dragonmaxhp ? basemaxhp : rnd(4); // +constitution_hp_bonus(m_acurr(mon, A_CON)) / 2;
    } 
    else 
    {
        basemaxhp = (int)mon->m_lev * 8; // +mon->m_lev * constitution_hp_bonus(m_acurr(mon, A_CON));
        hp = use_maxhp || dragonmaxhp ? basemaxhp : d((int)mon->m_lev, 8); // +mon->m_lev * constitution_hp_bonus(m_acurr(mon, A_CON));
    }

    if (hp < 1)
        hp = 1;

    if (basemaxhp < 1)
        basemaxhp = 1;

    if (is_home_elemental(ptr))
        hp *= 2;

    /* Finally, set mhpmax */
    mon->mbasehpmax = hp;
    if (mon->mbasehpmax < 1)
        mon->mbasehpmax = 1;
    mon->mbaseendrain = 0;
    update_mon_maxhp(mon);

    mon->mhp = mon->mhpmax;

    if (mon->mhp < 1)
        mon->mhp = 1;

    if (mon->mhp > mon->mhpmax)
        mon->mhp = mon->mhpmax;

}

int
monhp_difficulty_adjustment(hp, difficulty_level)
int hp;
int difficulty_level;
{
    int adjhp = hp;
    if (difficulty_level == 2)
        adjhp *= 4;
    else if (difficulty_level == 1)
        adjhp *= 2;
    else if (difficulty_level == -1)
        adjhp = (hp + 1) / 2;
    else if (difficulty_level == -2)
        adjhp = (hp + 3) / 4;

    return adjhp;
}

struct mextra *
newmextra()
{
    struct mextra *mextra;

    mextra = (struct mextra *) alloc(sizeof(struct mextra));
    mextra->mname = 0;
    mextra->umname = 0;
    mextra->egd = 0;
    mextra->epri = 0;
    mextra->esmi = 0;
    mextra->enpc = 0;
    mextra->eshk = 0;
    mextra->emin = 0;
    mextra->edog = 0;
    mextra->mmonst = 0;
    mextra->mobj = 0;
    mextra->mcorpsenm = NON_PM;
    return mextra;
}

boolean
makemon_rnd_goodpos(mon, gpflags, cc)
struct monst *mon;
unsigned long gpflags;
coord *cc;
{
    int tryct = 0;
    int nx,ny;
    boolean good;

    do {
        nx = rn1(COLNO - 3, 2);
        ny = rn2(ROWNO);
        int o_ridx = levl[nx][ny].roomno - ROOMOFFSET;
        boolean specialroom = FALSE;
        if (o_ridx >= 0 && rooms[o_ridx].rtype != OROOM)
            specialroom = TRUE;

        good = (!in_mklev && (abs(u.ux - nx) <= 7 || abs(u.uy - ny) <= 5 || specialroom || cansee(nx,ny))) ? FALSE
                                            : goodpos(nx, ny, mon, gpflags);
    } while ((++tryct < 50) && !good);

    if (!good) {
        /* else go through all map positions, twice, first round
           ignoring positions in sight, and pick first good one.
           skip first round if we're in special level loader or blind */
        int xofs = nx;
        int yofs = ny;
        int dx,dy;
        int bl = (in_mklev || Blind) ? 1 : 0;

        for ( ; bl < 2; bl++) {
            for (dx = 0; dx < COLNO; dx++)
                for (dy = 0; dy < ROWNO; dy++) {
                    nx = ((dx + xofs) % (COLNO - 1)) + 1;
                    ny = ((dy + yofs) % (ROWNO - 1)) + 1;
                    if (bl == 0 && cansee(nx,ny))
                        continue;
                    if (goodpos(nx, ny, mon, gpflags))
                        goto gotgood;
                }
            if (bl == 0 && (!mon || mon->data->mmove)) {
                /* all map positions are visible (or not good),
                   try to pick something logical */
                if (dnstair.sx && !rn2(2)) {
                    nx = dnstair.sx;
                    ny = dnstair.sy;
                } else if (upstair.sx && !rn2(2)) {
                    nx = upstair.sx;
                    ny = upstair.sy;
                } else if (dnladder.sx && !rn2(2)) {
                    nx = dnladder.sx;
                    ny = dnladder.sy;
                } else if (upladder.sx && !rn2(2)) {
                    nx = upladder.sx;
                    ny = upladder.sy;
                }
                if (goodpos(nx, ny, mon, gpflags))
                    goto gotgood;
            }
        }
    } else {
 gotgood:
        cc->x = nx;
        cc->y = ny;
        return TRUE;
    }
    return FALSE;
}


struct monst*
makemon(ptr, x, y, mmflags)
register struct permonst* ptr;
register int x, y;
unsigned long mmflags;
{
    return makemon_limited(ptr, x, y, mmflags, 0UL, 0, 0, 0, 0, 0, 0);
}

struct monst*
makemon2(ptr, x, y, mmflags, mmflags2)
register struct permonst* ptr;
register int x, y;
unsigned long mmflags, mmflags2;
{
    return makemon_limited(ptr, x, y, mmflags, mmflags2, 0, 0, 0, 0, 0, 0);
}

struct monst*
makemon_ex(ptr, x, y, mmflags, mmflags2, subtype, npcsubtype, level_adjustment)
register struct permonst* ptr;
register int x, y;
unsigned long mmflags, mmflags2;
unsigned short subtype;
int npcsubtype, level_adjustment;
{
    return makemon_limited(ptr, x, y, mmflags, mmflags2, subtype, subtype, npcsubtype, 0, level_adjustment, 0);
}

/*
 * called with [x,y] = coordinates;
 *      [0,0] means anyplace
 *      [u.ux,u.uy] means: near player (if !in_mklev)
 *
 *      In case we make a monster group, only return the one at [x,y].
 */
struct monst *
makemon_limited(ptr, x, y, mmflags, mmflags2, subtype, subtype_female, npcsubtype, level_limit, level_adjustment, alignment)
register struct permonst *ptr;
register int x, y;
unsigned long mmflags, mmflags2;
unsigned short subtype, subtype_female;
int npcsubtype;
int level_limit, level_adjustment;
aligntyp alignment;
{
    register struct monst *mtmp;
    int mndx = NON_PM, mcham, ct, mitem;
    //boolean anymon = (!ptr);
    boolean byyou = (x == u.ux && y == u.uy);
    boolean allow_minvent = ((mmflags & MM_NO_MONSTER_INVENTORY) == 0);
    boolean countbirth = ((mmflags & MM_NOCOUNTBIRTH) == 0);
    boolean setorigin = ((mmflags & MM_SET_ORIGIN_COORDINATES) == 0);
    boolean saddled = ((mmflags & MM_SADDLED) != 0);
    boolean maybe_extinct = ((mmflags2 & MM2_MAYBE_ALLOW_EXTINCT) != 0);
    boolean reviving = ((mmflags2 & MM2_REVIVING) != 0);
    boolean randomize_subtype = ((mmflags2 & MM2_RANDOMIZE_SUBTYPE) != 0);
    
    unsigned long gpflags = (mmflags & MM_IGNOREWATER) ? MM_IGNOREWATER : 0;
    int origin_x = x, origin_y = y;
    
    /* if caller wants random location, do it here */
    if (x == 0 && y == 0) 
    {
        coord cc;
        struct monst fakemon;

        cc.x = cc.y = 0; /* lint suppression */
        fakemon.data = ptr; /* set up for goodpos */
        if (!makemon_rnd_goodpos(ptr ? &fakemon : (struct monst *)0,
                                 gpflags, &cc))
            return (struct monst *) 0;
        x = cc.x;
        y = cc.y;
    } 
    else if (byyou && !in_mklev) 
    {
        coord bypos;

        if (enexto_core(&bypos, u.ux, u.uy, ptr, gpflags)) 
        {
            x = bypos.x;
            y = bypos.y;
        }
        else
            return (struct monst *) 0;
    }

    /* Does monster already exist at the position? */
    if (MON_AT(x, y)) 
    {
        if ((mmflags & MM_ADJACENTOK) != 0) 
        {
            coord bypos;
            if (enexto_core(&bypos, x, y, ptr, gpflags)) 
            {
                x = bypos.x;
                y = bypos.y;
            } 
            else
                return (struct monst *) 0;

        } 
        else
            return (struct monst *) 0;
    }

    if (ptr)
    {
        mndx = monsndx(ptr);

        /* if you are to make a specific monster and it has
           already been genocided, return */
        if (mvitals[mndx].mvflags & MV_GENOCIDED)
            return (struct monst *) 0;

        if ((mvitals[mndx].mvflags & MV_EXTINCT) != 0 && !reviving && countbirth)
        {
            if(wizard && maybe_extinct && yn_query("Create an extinct monster?") == 'y')
                debugpline1("Explicitly creating extinct monster %s.", mons[mndx].mname);
            else
                return (struct monst*)0;
        }
    }
    else 
    {
        /* make a random (common) monster that can survive here.
         * (the special levels ask for random monsters at specific
         * positions, causing mass drowning on the medusa level,
         * for instance.)
         */
        int tryct = 0; /* maybe there are no good choices */
        struct monst fakemon;

        do
        {
            if (!(ptr = rndmonst_limited(level_limit))) 
            {
                debugpline0("Warning: no monster.");
                return (struct monst *) 0; /* no more monsters! */
            }
            fakemon.data = ptr; /* set up for goodpos */
        } while (++tryct <= 50
                 /* in Sokoban, don't accept a giant on first try;
                    after that, boulder carriers are fair game */
                 && ((tryct == 1 && throws_rocks(ptr) && In_sokoban(&u.uz))
                     || !goodpos(x, y, &fakemon, gpflags)));

        if (ptr)
            mndx = monsndx(ptr);
        else
            return (struct monst*)0;
    }

    /* Play animation */
    enum special_effect_types spef_type = SPECIAL_EFFECT_SUMMON_MONSTER;
    if (!in_mklev && (mmflags & MM_PLAY_SUMMON_ANIMATION))
    {
        unsigned long atype = (mmflags & MM_SUMMON_ANIMATION_TYPE_MASK);
        enum sfx_sound_types startsound = SFX_GENERAL_SUMMON_START;
        enum sfx_sound_types sfxsound = SFX_SUMMON_MONSTER;
        if (atype == MM_CHAOTIC_SUMMON_ANIMATION)
        {
            spef_type = SPECIAL_EFFECT_SUMMON_DEMON;
            startsound = SFX_INFERNAL_SUMMON_START;
            sfxsound = SFX_SUMMON_DEMON;
        }
        else if (atype == MM_LAWFUL_SUMMON_ANIMATION)
        {
            startsound = SFX_CELESTIAL_SUMMON_START;
            spef_type = SPECIAL_EFFECT_SUMMON_CELESTIAL;
        }
        else if (atype == MM_NEUTRAL_SUMMON_ANIMATION)
        {
            startsound = SFX_NEUTRAL_SUMMON_START;
            spef_type = SPECIAL_EFFECT_SUMMON_NATURE;
        }
        else if (atype == MM_SUMMON_MONSTER_ANIMATION)
        {
            startsound = SFX_GENERAL_SUMMON_START;
            spef_type = SPECIAL_EFFECT_SUMMON_MONSTER;
        }
        else if (atype == MM_SUMMON_NASTY_ANIMATION)
        {
            startsound = SFX_SUMMON_NASTY_START;
            spef_type = SPECIAL_EFFECT_SUMMON_NASTY;
        }
        else if (atype == MM_UNDEAD_SUMMON_ANIMATION)
        {
            startsound = SFX_SUMMON_UNDEAD_START;
            spef_type = SPECIAL_EFFECT_SUMMON_UNDEAD;
        }
        else if (atype == MM_ANIMATE_DEAD_ANIMATION)
        {
            startsound = SFX_ANIMATE_DEAD_START;
            spef_type = SPECIAL_EFFECT_ANIMATE_DEAD;
        }
        else if (atype == MM_SUMMON_IN_SMOKE_ANIMATION)
        {
            startsound = SFX_SMOKE_SUMMON_START;
            spef_type = SPECIAL_EFFECT_SUMMON_IN_SMOKE;
        }

        play_special_effect_at(spef_type, context.makemon_spef_idx, x, y, FALSE);
        if (mmflags & MM_PLAY_SUMMON_SOUND)
            play_sfx_sound_at_location(startsound, x, y);
        special_effect_wait_until_action(context.makemon_spef_idx);
        if (mmflags & MM_PLAY_SUMMON_SOUND)
            play_sfx_sound_at_location(sfxsound, x, y);
    }

    (void) propagate(mndx, countbirth, FALSE);

    mtmp = newmonst();
    *mtmp = zeromonst; /* clear all entries in structure */

    if (mmflags & MM_EGD)
        newegd(mtmp);
    if (mmflags & MM_EPRI)
        newepri(mtmp);
    if (mmflags & MM_ESMI)
        newesmi(mtmp);
    if (mmflags & MM_ENPC)
    {
        newenpc(mtmp);
        if (has_enpc(mtmp))
            ENPC(mtmp)->npc_typ = npcsubtype;
    }
    if (mmflags & MM_ESHK)
        neweshk(mtmp);
    if (mmflags & MM_EMIN)
        newemin(mtmp);
    if (mmflags & MM_EDOG)
        newedog(mtmp);
    if (mmflags & MM_ASLEEP)
        mtmp->msleeping = 1;
    mtmp->nmon = fmon;
    fmon = mtmp;
    mtmp->m_id = context.ident++;
    if (!mtmp->m_id)
        mtmp->m_id = context.ident++; /* ident overflowed */

    set_mon_data(mtmp, ptr, 0); /* mtmp->data = ptr; */
    if (ptr->msound == MS_LEADER && quest_info(MS_LEADER) == mndx)
        quest_status.leader_m_id = mtmp->m_id;
    mtmp->mnum = mndx;
    mtmp->glyph = NO_GLYPH;
    mtmp->gui_glyph = NO_GLYPH;

    /* set up stats*/
    for (int i = 0; i < A_MAX; i++)
    {
        int curscore = 0;
        switch (i)
        {
        case A_STR:
            curscore = ptr->str;
            break;
        case A_DEX:
            curscore = ptr->dex;
            break;
        case A_CON:
            curscore = ptr->con;
            break;
        case A_INT:
            curscore = ptr->intl;
            break;
        case A_WIS:
            curscore = ptr->wis;
            break;
        case A_CHA:
            curscore = ptr->cha;
            break;
        }
        mtmp->acurr.a[i] = curscore;
        mtmp->abonus.a[i] = 0;
        mtmp->afixmin.a[i] = 0;
        mtmp->afixmax.a[i] = i == A_STR ? STR19(25) : 25;
        mtmp->amin.a[i] = curscore; // monster_attribute_minimum(ptr, i);
        mtmp->amax.a[i] = curscore; // monster_attribute_maximum(ptr, i);
        mtmp->atemp.a[i] = 0;
        mtmp->atime.a[i] = 0;
    }
    
    /* set up level and hit points */
    newmonhp(mtmp, mndx, level_adjustment, mmflags);

    /* set up the number of heads */
    mtmp->heads_left = ptr->heads;

    /* set up known rumors */
    mtmp->told_rumor = 0;
    mtmp->rumorsleft = 0;
    if (is_speaking(mtmp->data) && !(mmflags & MM_EGD))
    {
        mtmp->rumorsleft = (!rn2(2) ? 1 : 0) + (((is_lord(mtmp->data) && rn2(2)) || ((is_prince(mtmp->data) && rn2(4))) || ((mtmp->data->mflags6 & M6_ELDER) && rn2(3))) ? 1 : 0) + (ptr->intl > 15 && !rn2(2) ? 1 : 0);
    }

    /* set if faithful */
    if (is_always_faithful(ptr))
        mtmp->isfaithful = TRUE;

    /* set gender */
    if (is_female(ptr))
        mtmp->female = TRUE;
    else if (is_male(ptr))
        mtmp->female = FALSE;
    /* leader and nemesis gender is usually hardcoded in mons[],
       but for ones which can be random, it has already been chosen
       (in role_init(), for possible use by the quest pager code) */
    else if (ptr->msound == MS_LEADER && quest_info(MS_LEADER) == mndx)
        mtmp->female = quest_status.ldrgend;
    else if (ptr->msound == MS_NEMESIS && quest_info(MS_NEMESIS) == mndx)
        mtmp->female = quest_status.nemgend;
    else if (mmflags & MM_MALE)
        mtmp->female = FALSE;
    else if (mmflags & MM_FEMALE)
        mtmp->female = TRUE;
    else
    {
        mtmp->female = randomize_monster_gender(ptr);
    }

    mtmp->subtype = mtmp->female ? subtype_female : subtype;
    if (!mtmp->subtype && randomize_subtype)
    {
        if (mons[mndx].mflags6 & M6_USES_CAT_SUBTYPES)
        {
            if(!rn2(4))
                mtmp->subtype = CAT_BREED_BLACK;
            else if (!rn2(9))
                mtmp->subtype = rn2(NUM_CAT_BREEDS);
        }
        else if (mons[mndx].mflags6 & M6_USES_DOG_SUBTYPES)
        {
            if (!rn2(9))
                mtmp->subtype = rn2(NUM_DOG_BREEDS);
        }
    }

    if ((In_sokoban(&u.uz) && !mindless(ptr)) || (ptr->mflags3 & M3_KNOWS_PITS_AND_HOLES)) /* know about traps here */
        mtmp->mtrapseen = (1L << (PIT - 1)) | (1L << (HOLE - 1));

    /* quest leader and nemesis both know about all trap types */
    if (ptr->msound == MS_LEADER || ptr->msound == MS_NEMESIS)
        mtmp->mtrapseen = ~0;

    if (ptr->mflags3 & M3_KNOWS_TRAPS)
        mtmp->mtrapseen = ~0;

    mtmp->facing_right = (mmflags2 & MM2_FACING_LEFT) ? 0 : (mmflags2 & MM2_FACING_RIGHT) ? 1 : rn2(2);

    place_monster(mtmp, x, y);
    if (setorigin)
    {
        mtmp->mx0 = origin_x;
        mtmp->my0 = origin_y;
    }

    mtmp->mcanmove = mtmp->mwantstomove = mtmp->mwantstodrop = TRUE;
    mtmp->mpeaceful = (mmflags & MM_ANGRY) ? FALSE : (mmflags & MM_PEACEFUL) ? TRUE : peace_minded(ptr);

    switch (ptr->mlet) 
    {
    case S_MIMIC:
        if(is_mimic(ptr))
            set_mimic_sym(mtmp);
        break;
    case S_SPIDER:
    case S_SNAKE:
        if (in_mklev)
            if (x && y)
                (void) mkobj_at(0, x, y, TRUE);
        (void) hideunder(mtmp);
        break;
    case S_LIGHT:
    case S_ELEMENTAL:
        break;
    case S_EEL:
        (void) hideunder(mtmp);
        break;
    case S_LEPRECHAUN:
        //mtmp->msleeping = 1;
        break;
    case S_JABBERWOCK:
    case S_NYMPH:
        //if (rn2(5) && !u.uhave.amulet)
        //    mtmp->msleeping = 1;
        break;
    case S_ORC:
        if (Race_if(PM_ELF))
            mtmp->mpeaceful = FALSE;
        break;
    case S_UNICORN:
        if (is_unicorn(ptr) && sgn(u.ualign.type) == sgn(ptr->maligntyp))
            mtmp->mpeaceful = TRUE;
        break;
    case S_BAT:
        if (Inhell && is_bat(ptr))
            mtmp->mprops[VERY_FAST] |= M_INTRINSIC_ACQUIRED;
        break;
    }

    if ((ct = emitted_light_range(mtmp->data)) != 0)
        new_light_source(mtmp->mx, mtmp->my, ct, LS_MONSTER, monst_to_any(mtmp), 0);

    if ((ct = mon_ambient_sound(mtmp->data)) != 0)
        new_sound_source(mtmp->mx, mtmp->my, ct, (double)mon_ambient_volume(mtmp->data), SOUNDSOURCE_MONSTER, mon_ambient_subtype(mtmp->data),
            monst_to_any(mtmp));

    mitem = 0; /* extra inventory item for this monster */

    if (mndx == PM_VLAD_THE_IMPALER)
        mitem = CANDELABRUM_OF_INVOCATION;
    mtmp->cham = NON_PM; /* default is "not a shapechanger" */
    mtmp->cham_subtype = 0;

    if (!Protection_from_shape_changers
        && (mcham = pm_to_cham(mndx)) != NON_PM) 
    {
        /* this is a shapechanger after all */
        mtmp->cham = mcham;
        /* Vlad stays in his normal shape so he can carry the Candelabrum */
        if (mndx != PM_VLAD_THE_IMPALER
            /* Note:  shapechanger's initial form used to be chosen here
               with rndmonst(), yielding a monster which was appropriate
               to the level's difficulty but ignoring the changer's usual
               type selection, so was inappropriate for vampshifters.
               Let newcham() pick the shape. */
            && newcham(mtmp, (struct permonst *) 0, 0, FALSE, FALSE))
            allow_minvent = FALSE;
    }
    else if (mndx == PM_WIZARD_OF_YENDOR) 
    {
        mtmp->iswiz = TRUE;
        context.no_of_wizards++;
        if (context.no_of_wizards == 1 && Is_earthlevel(&u.uz))
            mitem = SPE_DIG;
    } 
    else if (mndx == PM_GHOST && !(mmflags & MM_NONAME)) 
    {
        mtmp = christen_monst(mtmp, rndghostname());
        mtmp->u_know_mname = TRUE;
    } 
    else if (mndx == PM_CROESUS) 
    {
        mitem = TWO_HANDED_SWORD;
    }
    else if (ptr->msound == MS_NEMESIS)
    {
        mitem = BELL_OF_OPENING;
    }
    else if (mndx == PM_PESTILENCE) 
    {
        mitem = POT_SICKNESS;
    }

    if (mitem && allow_minvent)
        (void) mongets(mtmp, mitem);

    if (in_mklev) 
    {
        if ((/* is_ndemon(ptr) ||mndx == PM_WUMPUS ||  */ 
             is_long_worm_with_tail(&mons[mndx]) || mndx == PM_GIANT_EEL)
            && !u.uhave.amulet && rn2(5))
            mtmp->msleeping = TRUE;
    } 
    else 
    {
        if (byyou) 
        {
            newsym_with_flags(mtmp->mx, mtmp->my, (mmflags& MM_PLAY_SUMMON_ANIMATION) ? NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS : NEWSYM_FLAGS_NONE); /* make sure the mon shows up */
            //newsym(mtmp->mx, mtmp->my);
            set_apparxy(mtmp);
            flush_screen(1);
        }
    }

#if 0
    if (is_dprince(ptr) && ptr->msound == MS_BRIBE) 
    {
        mtmp->mpeaceful = 1;
        increase_mon_property(mtmp, INVISIBILITY, 500);
        mtmp->mavenge = 0;
        if ((uwep && uwep->oartifact && artifact_has_flag(uwep, AF_ANGERS_DEMONS))
            || (uarms && uarms->oartifact && artifact_has_flag(uarms, AF_ANGERS_DEMONS))
            )
            mtmp->mpeaceful = mtmp->mtame = FALSE;
    }
#endif

#ifndef DCC30_BUG
    if (is_long_worm_with_tail(&mons[mndx]) && (mtmp->wormno = get_wormno()) != 0)
#else
    /* DICE 3.0 doesn't like assigning and comparing mtmp->wormno in the
       same expression. */
    if (is_long_worm_with_tail(&mons[mndx])
        && (mtmp->wormno = get_wormno(), mtmp->wormno != 0))
#endif
    {
        /* we can now create worms with tails - 11/91 */
        initworm(mtmp, rn2(5));
        if (count_wsegs(mtmp))
            place_worm_tail_randomly(mtmp, x, y);
    }

    /* Set roamer stuff */
    if ((mmflags & MM_ROAMER) && has_emin(mtmp))
    {
        register boolean coaligned = (u.ualign.type == alignment);
        EMIN(mtmp)->min_align = alignment;
        EMIN(mtmp)->renegade = (mmflags2 & MM2_FORCE_RENEGADE) ? TRUE : (mmflags2 & MM2_FORCE_NONRENEGADE) ? FALSE : (coaligned && !mtmp->mpeaceful);
        mtmp->ispriest = 0;
        mtmp->isminion = 1;
        mtmp->mtrapseen = ~0; /* traps are known */
        mtmp->msleeping = 0;
    }
    /* it's possible to create an ordinary monster of some special
       types; make sure their extended data is initialized to
       something sensible if caller hasn't specified MM_EPRI|MM_EMIN
       (when they're specified, caller intends to handle this itself) */
    else if ((mndx == PM_ALIGNED_PRIEST || mndx == PM_HIGH_PRIEST)
            ? !(mmflags & (MM_EPRI | MM_EMIN))
            : (mndx == PM_ANGEL && !(mmflags & MM_EMIN) && !rn2(3)))
    {
        struct emin *eminp;
        newemin(mtmp);
        eminp = EMIN(mtmp);

        mtmp->isminion = 1;            /* make priest be a roamer */
        eminp->min_align = (mmflags & MM_EMIN_COALIGNED) ? u.ualign.type : rn2(3) - 1; /* no A_NONE */
        eminp->renegade = (boolean) ((mmflags & MM_ANGRY) ? 1 : !rn2(3));
        mtmp->mpeaceful = (eminp->min_align == u.ualign.type)
                              ? !eminp->renegade
                              : eminp->renegade;
    }

    set_mhostility(mtmp); /* having finished peaceful changes */



#if 0
    if (anymon && !(mmflags & MM_NOGRP)) { //Small and large groups deactivated due to new encounter system -- JG
        if ((ptr->geno & G_SGROUP) && rn2(2)) 
        {
            m_initsgrp(mtmp, mtmp->mx, mtmp->my, mmflags);
        } 
        else if (ptr->geno & G_LGROUP)
        {
            if (rn2(3))
                m_initlgrp(mtmp, mtmp->mx, mtmp->my, mmflags);
            else
                m_initsgrp(mtmp, mtmp->mx, mtmp->my, mmflags);
        }
    }
#endif

    m_init_background(mtmp);

    if (allow_minvent)
    {
        if (is_armed(ptr))
            m_initweap(mtmp); /* equip with weapons / armor */
        m_initinv(mtmp); /* add on a few special items incl. more armor */
        m_dowear(mtmp, TRUE);

        if ((saddled || (!rn2(100) && is_domestic(ptr)))
            && can_saddle(mtmp) && !which_armor(mtmp, W_SADDLE)) 
        {
            struct obj *otmp = mksobj(SADDLE, TRUE, FALSE, FALSE);
            put_saddle_on_mon(otmp, mtmp);
        }
    }
    else
    {
        /* no initial inventory is allowed */
        if (mtmp->minvent)
            discard_minvent(mtmp);
        mtmp->minvent = (struct obj *) 0; /* caller expects this */
    }

    /* Add acceleration to lights in Quantum Tunnel */
    if (Is_quantum_tunnel_level(&u.uz) && mons[mtmp->mnum].mlet == S_LIGHT)
    {
        mtmp->mprops[!rn2(3) ? LIGHTNING_FAST : !rn2(2) ? SUPER_FAST : ULTRA_FAST] |= 500 + d(5, 100);
    }

    if (ptr->mflags3 && !(mmflags & MM_NOWAIT)) 
    {
        if (ptr->mflags3 & M3_WAITFORU)
            mtmp->mstrategy |= STRAT_WAITFORU;
        if (ptr->mflags3 & M3_CLOSE)
            mtmp->mstrategy |= STRAT_CLOSE;
        if (ptr->mflags3 & (M3_WAITMASK | M3_COVETOUS))
            mtmp->mstrategy |= STRAT_APPEARMSG;
    }
    
    if (mmflags & MM_WAITFORU)
        mtmp->mstrategy |= STRAT_WAITFORU;

    if (allow_minvent && migrating_objs)
        deliver_obj_to_mon(mtmp, 1, DF_NONE); /* in case of waiting items */

    update_all_mon_statistics(mtmp, TRUE);

    if (!in_mklev)
    {
        newsym_with_flags(mtmp->mx, mtmp->my, (mmflags & MM_PLAY_SUMMON_ANIMATION) ? NEWSYM_FLAGS_KEEP_OLD_EFFECT_GLYPHS : NEWSYM_FLAGS_NONE); /* make sure the mon shows up */

        if (mmflags & MM_ANIMATION_WAIT_UNTIL_END)
        {
            flush_screen(1);
            special_effect_wait_until_end(context.makemon_spef_idx);
        }
    }
    return mtmp;
}

void
makemon_animation_wait_until_end()
{
    int num = min(MAX_PLAYED_SPECIAL_EFFECTS, max(1, context.makemon_spef_idx));
    for (int i = 0; i < num; i++)
    {
        if (context.special_effect_animation_counter_on[i])
        {
            special_effect_wait_until_end(i);
        }
    }
    context.makemon_spef_idx = 0;
}


boolean
randomize_monster_gender(ptr)
struct permonst* ptr;
{
    if (is_female(ptr))
        return TRUE;
    else if (is_male(ptr))
        return FALSE;
    else if (is_neuter(ptr))
        return FALSE;

    unsigned long genderfrequence = (ptr->geno & G_GENDER_GEN_MASK);
    boolean malerare = ((ptr->geno & G_GENDER_MALE_RARE) != 0);
    int chancepower = genderfrequence != 0UL ? (int)(genderfrequence - G_GENDER_ONE_FOURTH + 1UL) : 0;
    int oneinchance = 2 << chancepower; /* 2 to the power of chancepower */

    return malerare ? rn2(max(2, oneinchance)) : !rn2(max(2, oneinchance)); /* ignored for neuters */
}

int
mbirth_limit(mndx)
int mndx;
{
    /* There is an implicit limit of 4 for "high priest of <deity>",
     * but aligned priests can grow into high priests, thus they aren't
     * really limited to 4, so leave the default amount in place for them.
     */

    /* assert(MAXMONNO < 255); */
    int blimit = MAXMONNO;
    switch (mndx)
    {
    case PM_WRAITHLORD:
        blimit = 9;
        break;
    case PM_MODRON_QUINTON:
        blimit = 25;
        break;
    case PM_MODRON_QUARTON:
        blimit = 16;
        break;
    case PM_MODRON_TERTIAN:
        blimit = 9;
        break;
    case PM_MODRON_SECUNDUS:
        blimit = 4;
        break;
    default:
        break;
    }
    
    return blimit;
}

/* used for wand/scroll/spell of create monster */
/* returns TRUE iff you know monsters have been created */
boolean
create_critters(cnt, mptr, neverask)
int cnt;
struct permonst *mptr; /* usually null; used for confused reading */
boolean neverask;
{
    coord c;
    int x, y;
    struct monst *mon;
    boolean known = FALSE;
    boolean ask = (wizard && !neverask);

    while (cnt--) 
    {
        if (ask)
        {
            if (create_particular()) 
            {
                known = TRUE;
                continue;
            }
            else
                ask = FALSE; /* ESC will shut off prompting */
        }
        x = u.ux, y = u.uy;
        /* if in water, try to encourage an aquatic monster
           by finding and then specifying another wet location */
        if (!mptr && u.uinwater && enexto(&c, x, y, &mons[PM_GIANT_EEL]))
            x = c.x, y = c.y;

        mon = makemon2(mptr, x, y,  MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_IN_SMOKE_ANIMATION | MM_PLAY_SUMMON_SOUND, MM2_RANDOMIZE_SUBTYPE);
        if (mon)
        {
            //play_sfx_sound_at_location(SFX_SUMMON_MONSTER, mon->mx, mon->my);
            if(canspotmon(mon))
                known = TRUE;
        }
    }
    makemon_animation_wait_until_end();
    return known;
}

STATIC_OVL boolean
ungeneratable_monster_type(mndx)
int mndx;
{
    return !mk_gen_ok(mndx, MV_GONE, (G_NOGEN | G_UNIQ), FALSE);

    //if (mons[mndx].geno & (G_NOGEN | G_UNIQ))
    //    return TRUE;
    //if (mvitals[mndx].mvflags & MV_GONE)
    //    return TRUE;
    //if (Inhell)
    //    return (boolean) ((mons[mndx].maligntyp > A_NEUTRAL));
    //else
    //    return (boolean) ((mons[mndx].geno & G_HELL) != 0);
}

/*
 *      shift the probability of a monster's generation by
 *      comparing the dungeon alignment and monster alignment.
 *      return an integer in the range of 0-5.
 */
STATIC_VAR NEARDATA long oldmoves = 0L; /* != 1, starting value of moves */
STATIC_VAR NEARDATA s_level* align_lev;

STATIC_OVL int
align_shift(ptr)
register struct permonst *ptr;
{
    register int alshift;

    if (oldmoves != moves) {
        align_lev = Is_special(&u.uz);
        oldmoves = moves;
    }
    switch ((align_lev) ? align_lev->flags.align : dungeons[u.uz.dnum].flags.align) {
    default: /* just in case */
    case AM_NONE:
        alshift = 0;
        break;
    case AM_LAWFUL:
        alshift = (ptr->maligntyp + 20) / (2 * ALIGNWEIGHT);
        break;
    case AM_NEUTRAL:
        alshift = (20 - abs(ptr->maligntyp)) / ALIGNWEIGHT;
        break;
    case AM_CHAOTIC:
        alshift = (-(ptr->maligntyp - 20)) / (2 * ALIGNWEIGHT);
        break;
    }
    return alshift;
}

STATIC_VAR NEARDATA struct {
    int choice_count;
    char mchoices[SPECIAL_PM]; /* value range is 0..127 */
} rndmonst_state = { -1, { 0 } };


/* select a random monster type */
struct permonst*
rndmonst()
{
    return rndmonst_limited(0);
}

/* select a random monster type */
struct permonst*
rndmonst_limited(level_limit)
int level_limit;
{
    register struct permonst *ptr;
    register int mndx, ct;

    /* Special dungeons; note that these influence only wand of create monster, summon monster spells, and shapeshifters; random monster generation is handled in create_monster_or_encounter */
    if (u.uz.dnum == quest_dnum && rn2(7) && (ptr = qt_montype()) != 0)
        return ptr;

    if (u.uz.dnum == modron_dnum && (ptr = mkclass(S_MODRON, 0)) != (struct permonst*)0)
        return ptr;

    if (u.uz.dnum == bovine_dnum)
    {
        ptr = (struct permonst*)0;
        if(!(mons[PM_HELL_BOVINE].geno & MV_GONE))
            ptr = &mons[PM_HELL_BOVINE];
        
        if (!(mons[PM_MINOTAUR].geno & MV_GONE) && (!ptr || !rn2(2)))
            ptr = &mons[PM_MINOTAUR];

        if (!(mons[PM_BISON].geno & MV_GONE) && (!ptr || !rn2(6)))
            ptr = &mons[PM_BISON];

        return ptr;
    }


    /* Normal case */
    if (rndmonst_state.choice_count < 0) 
    { /* need to recalculate */
        int minmlev = 0, maxmlev = 0;
        boolean upper;

        for (int i = 1; i <= 3; i++)
        {
            rndmonst_state.choice_count = 0;
            /* look for first common monster */
            for (mndx = LOW_PM; mndx < SPECIAL_PM; mndx++) 
            {
                if (!ungeneratable_monster_type(mndx))
                    break;
                rndmonst_state.mchoices[mndx] = 0;
            }
            if (mndx == SPECIAL_PM) 
            {
                /* evidently they've all been exterminated */
                debugpline0("rndmonst: no common mons!");
                return (struct permonst *) 0;
            } /* else `mndx' now ready for use below */
            /* determine the level of the weakest monster to make. */
            /* determine the level of the strongest monster to make. */
            get_generated_monster_minmax_levels(i, &minmlev, &maxmlev, 0);

            if (level_limit > 0)
            {
                if (maxmlev > level_limit)
                    maxmlev = level_limit;
                if (minmlev > (int)((double)level_limit / 2.5))
                    minmlev = (int)((double)level_limit / 2.5);
            }

            upper = Is_really_rogue_level(&u.uz);

            /*
             * Find out how many monsters exist in the range we have selected.
             */
            for ( ; mndx < SPECIAL_PM; mndx++)
            { /* (`mndx' initialized above) */
                ptr = &mons[mndx];
                rndmonst_state.mchoices[mndx] = 0;
                if (tooweak(mndx, minmlev) || toostrong(mndx, maxmlev))
                    continue;
                if (upper && !isupper((uchar) def_monsyms[(int) ptr->mlet].sym)) /* Left here, because influences the whole class; you can thus force lower case monsters with mkclass on rogue level */
                    continue;
                if (ungeneratable_monster_type(mndx))
                    continue;
                ct = (int) (ptr->geno & G_FREQ) + align_shift(ptr);
                if (ct < 0 || ct > 127)
                {
                    panic("rndmonst: bad count [#%d: %d]", mndx, ct);
                    return (struct permonst*) 0;
                }
                rndmonst_state.choice_count += ct;
                rndmonst_state.mchoices[mndx] = (char) ct;
            }
            /*
             *      Possible modification:  if choice_count is "too low",
             *      expand minmlev..maxmlev range and try again.
             */
            if (rndmonst_state.choice_count > 0)
                break;
        }
    } /* choice_count+mchoices[] recalc */

    if (rndmonst_state.choice_count <= 0) 
    {
        /* maybe no common mons left, or all are too weak or too strong */
        debugpline1("rndmonst: choice_count=%d", rndmonst_state.choice_count);
        return (struct permonst *) 0;
    }

    /*
     *  Now, select a monster at random.
     */
    ct = rnd(rndmonst_state.choice_count);
    for (mndx = LOW_PM; mndx < SPECIAL_PM; mndx++)
        if ((ct -= (int) rndmonst_state.mchoices[mndx]) <= 0)
            break;

    if (mndx == SPECIAL_PM || ungeneratable_monster_type(mndx) || mndx < LOW_PM || mndx >= NUM_MONSTERS) 
    { /* shouldn't happen */
        impossible("rndmonst: bad `mndx' [#%d]", mndx);
        return (struct permonst *) 0;
    }

    return &mons[mndx];
}


void
get_generated_monster_minmax_levels(attempt, minlvl, maxlvl, difficulty_level_adjustment)
int attempt;
int* minlvl;
int* maxlvl;
int difficulty_level_adjustment;
{
    /* Initial adjustment */
    double max_multiplier = 0.50;
    double min_multiplier = 0.50;

    double zlevel = (double)level_difficulty();
    double zlevel_formin = min(MAXULEV, zlevel);
    double zlevel_formax = zlevel;
    int i = attempt;

    int minmlev = 0;
    int maxmlev = 0;
    
    /* game difficulty deactivated here for the time being */
    int applied_difficulty = 0 /*(int)context.game_difficulty*/ + difficulty_level_adjustment;
    if (applied_difficulty < -4)
        applied_difficulty = -4;
    if (applied_difficulty > 4)
        applied_difficulty = 4;

    /* Difficult level adjustments */
    switch (applied_difficulty)
    {
    case -4:
        min_multiplier /= 2.0;
        max_multiplier /= 2.0;
        break;
    case -3:
        min_multiplier /= 1.682;
        max_multiplier /= 1.682;
        break;
    case -2:
        min_multiplier /= 1.414;
        max_multiplier /= 1.414;
        break;
    case -1:
        min_multiplier /= 1.189;
        max_multiplier /= 1.189;
        break;
    case 0:
        min_multiplier *= 1.0;
        max_multiplier *= 1.0;
        break;
    case 1:
        min_multiplier *= 1.189;
        max_multiplier *= 1.189;
        break;
    case 2:
        min_multiplier *= 1.414;
        max_multiplier *= 1.414;
        break;
    case 3:
        min_multiplier *= 1.682;
        max_multiplier *= 1.682;
        break;
    case 4:
        min_multiplier *= 2.0;
        max_multiplier *= 2.0;
        break;
    }

    if (i == 1)
    {
        /* Try first with a tighter range */
        minmlev = (int)min(25.0, max(0.0, (zlevel_formin + (double)u.ulevel) * min_multiplier / 1.66 - 1.0));
        maxmlev = (int)max(1.0, (zlevel_formax + (double)u.ulevel) * max_multiplier + 0.5);
    }
    else if (i == 2)
    {
        minmlev = (int)min(15.0, max(0.0, (zlevel_formin + (double)u.ulevel) * min_multiplier / 2.5 - 1.0));
        maxmlev = (int)max(1.0, (zlevel_formax + (double)u.ulevel) * 1.189 * max_multiplier + 0.5);
    }
    else
    {
        minmlev = 0;
        maxmlev = (int)max(1.0, (zlevel_formax + (double)u.ulevel) * 1.414 * max_multiplier + 0.5);
    }

    *minlvl = minmlev;
    *maxlvl = maxmlev;

}
/* called when you change level (experience or dungeon depth) or when
   monster species can no longer be created (genocide or extinction) */
void
reset_rndmonst(mndx)
int mndx; /* particular species that can no longer be created */
{
    /* cached selection info is out of date */
    if (mndx == NON_PM) {
        rndmonst_state.choice_count = -1; /* full recalc needed */
    } else if (mndx < SPECIAL_PM) {
        rndmonst_state.choice_count -= rndmonst_state.mchoices[mndx];
        rndmonst_state.mchoices[mndx] = 0;
    } /* note: safe to ignore extinction of unique monsters */
}

/* decide whether it's ok to generate a candidate monster by mkclass() */
STATIC_OVL boolean
mk_gen_ok(mndx, mvflagsmask, genomask, ispoly)
int mndx;
uchar mvflagsmask;
unsigned long genomask;
boolean ispoly;
{
    struct permonst *ptr = &mons[mndx];

    if (mvitals[mndx].mvflags & mvflagsmask)
        return FALSE;
    if (ispoly)
    {
        if (!polyok(ptr))
            return FALSE;
    }
    else
    {
        if (is_placeholder(ptr))
            return FALSE;
        if ((mons[mndx].geno & G_STRAYED) && u.ualign.type == u.ualignbase[A_ORIGINAL]) /* Does not appear if you are on the right path */
            return FALSE;
        if ((ptr->geno & G_MODRON) && (ptr->geno & G_NOGEN) && u.uz.dnum != modron_dnum)
            return FALSE;
        if ((ptr->geno & G_YACC) && (ptr->geno & G_NOGEN) && u.uz.dnum != bovine_dnum)
            return FALSE;
        if ((ptr->geno & G_MODRON) && u.uz.dnum == modron_dnum) /* Overrides G_NOGEN */
            return TRUE;
        if ((ptr->geno & G_YACC) && u.uz.dnum == bovine_dnum) /* Overrides G_NOGEN */
            return TRUE;
        if ((ptr->geno & G_NOMINES) && In_mines(&u.uz))
            return FALSE;
        if ((ptr->geno & G_NOHELL) && Inhell)
            return FALSE;
        if ((ptr->geno & G_HELL) && !Inhell)
            return FALSE;
        if (In_endgame(&u.uz) && !Is_astralevel(&u.uz) && wrong_elem_type(ptr))
            return FALSE;
        //if (Inhell && (ptr->maligntyp <= A_NEUTRAL))
        //    return FALSE;
    }

    /* Note that G_MODRON and G_YACC override G_NOGEN */
    if (ptr->geno & genomask)
        return FALSE;

    /* special levels might ask for random demon type; reject this one */
    if (ptr == &mons[PM_MAIL_DAEMON])
        return FALSE;

    return TRUE;
}

/* Make one of the multiple types of a given monster class.
   The second parameter specifies a special casing bit mask
   to allow the normal genesis masks to be deactivated.
   Returns Null if no monsters in that class can be made. */
struct permonst *
mkclass(mclass, spc)
char mclass;
int spc;
{
    return mkclass_core(mclass, spc, A_NONE, 0, 0UL);
}

/* mkclass() with alignment restrictions; used by ndemon() */
struct permonst*
mkclass_aligned(mclass, spc, atyp, mflags)
char mclass;
int spc;
aligntyp atyp;
unsigned long mflags;
{
    return mkclass_core(mclass, spc, atyp, 0, mflags);
}

struct permonst *
mkclass_core(mclass, spc, atyp, difficulty_adj, mflags)
char mclass;
int spc;
aligntyp atyp;
int difficulty_adj;
unsigned long mflags;
{
    register int first = 0, last = 0, num = 0;
    int k, nums[SPECIAL_PM + 1]; /* +1: insurance for final return value */
    int minmlev = 0, maxmlev = 0, mask = (G_NOGEN | G_UNIQ) & ~spc;

    if (mclass < 1 || mclass >= MAX_MONSTER_CLASSES) {
        impossible("mkclass called with bad class!");
        return (struct permonst*)0;
    }

    (void) memset((genericptr_t) nums, 0, sizeof nums);

    for(int i = 1; i <= 3; i++)
    {
        get_generated_monster_minmax_levels(i, &minmlev, &maxmlev, difficulty_adj);
    
        /*  Assumption #1:  monsters of a given class are contiguous in the
         *                  mons[] array.  Player monsters and quest denizens
         *                  are an exception; mkclass() won't pick them.
         *                  SPECIAL_PM is long worm tail and separates the
         *                  regular monsters from the exceptions.
         */
        first = NON_PM;
        boolean foundfirst = FALSE;
        for (int firstindex = LOW_PM; firstindex < SPECIAL_PM; firstindex++)
            if (mons[firstindex].mlet == mclass)
            {
                if(first == NON_PM)
                    first = firstindex;

                if((mflags & MKCLASS_FLAGS_IGNORE_DIFFICULTY) != 0 || !tooweak(firstindex,minmlev))
                {
                    first = firstindex;
                    foundfirst = TRUE;
                    break;
                }
            }

        if (foundfirst)
            break;
    }

    if (first == NON_PM) //SPECIAL_PM)
    {
        //impossible("mkclass found no class %d monsters", mclass);
        return (struct permonst *) 0;
    }

    /*  Assumption #2:  monsters of a given class are presented in ascending
     *                  order of strength.
     */
    for (last = first; last < SPECIAL_PM && mons[last].mlet == mclass; last++)
    {
        if (atyp != A_NONE && sgn(mons[last].maligntyp) != sgn(atyp))
            continue;
        if (mk_gen_ok(last, MV_GONE, mask, FALSE))
        {
            /* consider it; don't reject a toostrong() monster if we
               don't have anything yet (num==0) or if it is the same
               (or lower) difficulty as preceding candidate (non-zero
               'num' implies last > first so mons[last-1] is safe);
               sometimes accept it even if high difficulty */
            if (num && toostrong(last, maxmlev) && (mflags & MKCLASS_FLAGS_IGNORE_DIFFICULTY) == 0
                && last > first && mons[last].difficulty > mons[last - 1].difficulty)
                break;

            k = (int)(mons[last].geno & G_FREQ);
            if (k > 0)
            {
                /* skew towards lower value monsters at lower exp. levels
                   (this used to be done in the next loop, but that didn't
                   work well when multiple species had the same level and
                   were followed by one that was past the bias threshold;
                   cited example was succubus and incubus, where the bias
                   against picking the next demon resulted in incubus
                   being picked nearly twice as often as succubus);
                   we need the '+1' in case the entire set is too high
                   level (really low level hero) */
                nums[last] = k + 1 - (adj_lev(&mons[last], 0) > (u.ulevel * 2));
                num += nums[last];
            }
        }
    }
    if (!num)
        return (struct permonst *) 0;

    /* the hard work has already been done; 'num' should hit 0 before
       first reaches last (which is actually one past our last candidate) */
    for (num = rnd(num); first < last; first++)
        if ((num -= nums[first]) <= 0)
            break;

    if (first < LOW_PM || first >= NUM_MONSTERS)
    {
        impossible("mkclass: bad `first' [#%d]", first);
        return (struct permonst*)0;
    }

    return nums[first] ? &mons[first] : (struct permonst *) 0;
}

/* like mkclass(), but excludes difficulty considerations; used when
   player with polycontrol picks a class instead of a specific type;
   genocided types are avoided but extinct ones are acceptable; we don't
   check polyok() here--caller accepts some choices !polyok() would reject */
int
mkclass_poly(mclass)
int mclass;
{
    register int first, last, num = 0;

    for (first = LOW_PM; first < SPECIAL_PM; first++)
        if (mons[first].mlet == mclass)
            break;
    if (first == SPECIAL_PM)
        return NON_PM;

    for (last = first; last < SPECIAL_PM && mons[last].mlet == mclass; last++)
        if (mk_gen_ok(last, MV_GENOCIDED, (G_NOGEN | G_UNIQ), TRUE))
            num += (int)(mons[last].geno & G_FREQ);
    if (!num)
        return NON_PM;

    for (num = rnd(num); num > 0; first++)
        if (mk_gen_ok(first, MV_GENOCIDED, (G_NOGEN | G_UNIQ), TRUE))
            num -= (int)(mons[first].geno & G_FREQ);
    first--; /* correct an off-by-one error */

    return first;
}

/* adjust strength of monsters based on u.uz and u.ulevel */
int
adj_lev(ptr, manual_adj)
register struct permonst *ptr;
int manual_adj;
{
    int tmp, tmp2;

    if (ptr == &mons[PM_WIZARD_OF_YENDOR]) {
        /* does not depend on other strengths, but does get stronger
         * every time he is killed
         */
        tmp = ptr->mlevel + mvitals[PM_WIZARD_OF_YENDOR].died;
        if (tmp > MAX_MONSTER_LEVEL)
            tmp = MAX_MONSTER_LEVEL;
        return tmp;
    }

    tmp = ptr->mlevel + manual_adj;
    if (tmp > MAX_MONSTER_LEVEL)
        return MAX_MONSTER_LEVEL; /* "special" demons/devils */
    tmp2 = (level_difficulty() - tmp);
    if (tmp2 < 0)
        tmp--; /* if mlevel > u.uz decrement tmp */
    else
        tmp += (tmp2 / 5); /* else increment 1 per five diff */

    tmp2 = (u.ulevel - ptr->mlevel); /* adjust vs. the player */
    if (tmp2 > 0)
        tmp += (tmp2 / 4); /* level as well */

    tmp2 = (3 * ((int) ptr->mlevel)) / 2 + manual_adj; /* crude upper limit */
    if (tmp2 > MAX_MONSTER_LEVEL)
        tmp2 = MAX_MONSTER_LEVEL;  /* hard upper limit */
    if (tmp2 < 0)
        tmp2 = 0;  /* hard lower limit */
    return ((tmp > tmp2) ? tmp2 : (tmp > 0 ? tmp : 0)); /* 0 lower limit */
}

/* monster earned experience and will gain some hit points; it might also
   grow into a bigger monster (baby to adult, soldier to officer, etc) */
struct permonst *
grow_up(mtmp, victim)
struct monst *mtmp, *victim;
{
    int oldtype, newtype, max_increase, cur_increase, lev_limit, hp_threshold;
    unsigned fem;
    struct permonst *ptr = mtmp->data;

    /* monster died after killing enemy but before calling this function */
    /* currently possible if killing a gas spore */
    if (DEADMONSTER(mtmp))
        return (struct permonst *) 0;

    /* note:  none of the monsters with special hit point calculations
       have both little and big forms */
    oldtype = monsndx(ptr);
    newtype = little_to_big(oldtype);

    /* growth limits differ depending on method of advancement */
    if (victim)
    {                       /* killed a monster */
        /*
         * The HP threshold is the maximum number of hit points for the
         * current level; once exceeded, a level will be gained.
         * Possible bug: if somehow the hit points are already higher
         * than that, monster will gain a level without any increase in HP.
         */
        hp_threshold = mtmp->m_lev * 8; /* normal limit */
        if (!mtmp->m_lev)
            hp_threshold = 4;

        lev_limit = 3 * (int) ptr->mlevel / 2; /* same as adj_lev() */
        /* If they can grow up, be sure the level is high enough for that */
        if (oldtype != newtype && mons[newtype].mlevel > lev_limit)
            lev_limit = (int) mons[newtype].mlevel;
        /* number of hit points to gain; unlike for the player, we put
           the limit at the bottom of the next level rather than the top */
        max_increase = rnd((int) victim->m_lev + 1);
        if (mtmp->mbasehpmax + max_increase > hp_threshold + 1)
            max_increase = max((hp_threshold + 1) - mtmp->mbasehpmax, 0);
        cur_increase = (max_increase > 1) ? rn2(max_increase) : 0;
    } 
    else
    {
        /* a gain level potion or wraith corpse; always go up a level
           unless already at maximum (MAX_MONSTER_LEVEL is hard upper limit) */
        max_increase = cur_increase = rnd(8);
        hp_threshold = 0; /* smaller than `mhpmax + max_increase' */
        lev_limit = MAX_MONSTER_LEVEL;   /* recalc below */
    }


    mtmp->mbasehpmax += max_increase;
    mtmp->mhp += cur_increase;
    update_mon_maxhp(mtmp);
    if (mtmp->mbasehpmax <= hp_threshold)
        return ptr; /* doesn't gain a level */

    if (is_mplayer(ptr))
        lev_limit = MAXULEV; /* same as player */
    else if (lev_limit < 5)
        lev_limit = 5; /* arbitrary */
    else if (lev_limit > MAX_MONSTER_LEVEL)
        lev_limit = MAX_MONSTER_LEVEL; //(ptr->mlevel > 49 ? 50 : 49);

    if ((int) ++mtmp->m_lev >= mons[newtype].mlevel && newtype != oldtype)
    {
        ptr = &mons[newtype];
        /* new form might force gender change */
        fem = is_male(ptr) ? 0 : is_female(ptr) ? 1 : mtmp->female;

        if (mvitals[newtype].mvflags & MV_GENOCIDED)
        { /* allow MV_EXTINCT */
            if (canspotmon(mtmp))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "As %s grows up into %s, %s %s!", mon_nam(mtmp),
                      an(pm_monster_name(ptr, mtmp->female)), mhe(mtmp),
                      is_not_living(ptr) ? "expires" : "dies");
            set_mon_data(mtmp, ptr, mtmp->subtype); /* keep mvitals[] accurate */
            change_mon_ability_scores(mtmp, oldtype, newtype);
            mondied(mtmp);
            return (struct permonst *) 0;
        } 
        else if (canspotmon(mtmp))
        {
            char buf[BUFSZ];

            /* 3.6.1:
             * Temporary (?) hack to fix growing into opposite gender.
             */
            play_sfx_sound_at_location(SFX_GAIN_LEVEL, mtmp->mx, mtmp->my);
            Sprintf(buf, "%s%s",
                    /* deal with female gnome becoming a gnome lord */
                    (mtmp->female && !fem) ? "male "
                        /* or a male gnome becoming a gnome lady
                           (can't happen with 3.6.0 mons[], but perhaps
                           slightly less sexist if prepared for it...) */
                      : (fem && !mtmp->female) ? "female " : "",
                    pm_monster_name(ptr, !!fem));
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s %s.", upstart(y_monnam(mtmp)),
                  (fem != mtmp->female) ? "changes into"
                                        : humanoid(ptr) ? "becomes"
                                                        : "grows up into",
                  an(buf));
        }
        set_mon_data(mtmp, ptr, mtmp->subtype);
        change_mon_ability_scores(mtmp, oldtype, newtype);
        newsym(mtmp->mx, mtmp->my);    /* color may change */
        lev_limit = (int) mtmp->m_lev; /* never undo increment */

        mtmp->female = fem; /* gender might be changing */
    }

    /* sanity checks */
    if ((int) mtmp->m_lev > lev_limit) 
    {
        mtmp->m_lev--; /* undo increment */
        /* HP might have been allowed to grow when it shouldn't */
        if (mtmp->mbasehpmax == hp_threshold + 1)
        {
            mtmp->mbasehpmax--;
            update_mon_maxhp(mtmp);
        }
    }
//    if (mtmp->mhpmax > 50 * 8)
//        mtmp->mhpmax = 50 * 8; /* absolute limit */
    if (mtmp->mhp > mtmp->mhpmax)
        mtmp->mhp = mtmp->mhpmax;

    return ptr;
}

void
change_mon_ability_scores(mtmp, oldtype, newtype)
struct monst* mtmp;
int oldtype, newtype;
{
    /* Adjust monster ability scores to the new form */
    if (oldtype != newtype)
    {
        int strdiff = mons[newtype].str - mons[oldtype].str;
        int dexdiff = mons[newtype].dex - mons[oldtype].dex;
        int condiff = mons[newtype].con - mons[oldtype].con;
        int intdiff = mons[newtype].intl - mons[oldtype].intl;
        int wisdiff = mons[newtype].wis - mons[oldtype].wis;
        int chadiff = mons[newtype].cha - mons[oldtype].cha;

        if (strdiff)
            m_gainstr(mtmp, (struct obj*)0, strdiff, FALSE);
        if (dexdiff)
            (void)m_adjattrib(mtmp, A_DEX, dexdiff, FALSE);
        if (condiff)
            (void)m_adjattrib(mtmp, A_CON, condiff, FALSE);
        if (intdiff)
            (void)m_adjattrib(mtmp, A_INT, intdiff, FALSE);
        if (wisdiff)
            (void)m_adjattrib(mtmp, A_WIS, wisdiff, FALSE);
        if (chadiff)
            (void)m_adjattrib(mtmp, A_CHA, chadiff, FALSE);
    }


}

int
mongets_return_enchantment(mtmp, otyp)
register struct monst* mtmp;
int otyp;
{
    struct obj* otmp = mongets(mtmp, otyp);
    if (!otmp)
        return 0;
    else
        return otmp->enchantment;
}

struct obj*
mongets(mtmp, otyp)
register struct monst* mtmp;
int otyp;
{
    return mongets_with_material(mtmp, otyp, MAT_NONE);
}

struct obj*
mongets_with_material(mtmp, otyp, material)
register struct monst *mtmp;
int otyp;
uchar material;
{
    register struct obj *otmp;
    if (!otyp || !mtmp)
        return (struct obj*)0;

    otmp = mksobj_with_flags(otyp, TRUE, FALSE, FALSE, mtmp, material, 0L, 0L, 0UL);
    if (otmp) 
    {
        if (mtmp->data->mlet == S_DEMON) 
        {
            /* demons never get blessed objects */
            if (otmp->blessed)
                curse(otmp);
        } 
        else if (is_lminion(mtmp)) 
        {
            /* lawful minions don't get cursed, bad, or rusting objects */
            otmp->cursed = FALSE;
            if (otmp->enchantment < 0)
                otmp->enchantment = 0;
            otmp->oerodeproof = TRUE;
        } 
        else if (is_mplayer(mtmp->data) && is_sword(otmp)) 
        {
            otmp->enchantment = (3 + rn2(4));
        }

        if (otmp->otyp == CANDELABRUM_OF_INVOCATION)
        {
            otmp->enchantment = 0;
            otmp->special_quality = 0;
            otmp->age = 0L;
            otmp->lamplit = FALSE;
            otmp->blessed = otmp->cursed = FALSE;
        }
        else if (otmp->otyp == BELL_OF_OPENING)
        {
            otmp->blessed = otmp->cursed = FALSE;
        } 
        else if (otmp->otyp == SPE_BOOK_OF_THE_DEAD)
        {
            otmp->blessed = FALSE;
            otmp->cursed = TRUE;
        }

        /* leaders and boss monsters don't tolerate inferior quality battle gear */
        if (mtmp->data->msound == MS_NEMESIS)
        {
            if (is_weapon(otmp))
            {
                if (can_have_exceptionality(otmp) && otmp->oartifact == 0 && otmp->exceptionality < EXCEPTIONALITY_ELITE)
                    otmp->exceptionality = EXCEPTIONALITY_ELITE;
                if (otmp->enchantment < 2)
                    otmp->enchantment = 2;
            }

            if (otmp->oclass == ARMOR_CLASS && otmp->enchantment < 1)
                otmp->enchantment = 1;
        }
        else if (is_demon(mtmp->data) && (is_prince(mtmp->data) || is_lord(mtmp->data)))
        {
            if (is_weapon(otmp))
            {
                if (can_have_exceptionality(otmp) && otmp->oartifact == 0 && otmp->exceptionality != EXCEPTIONALITY_INFERNAL)
                    otmp->exceptionality = EXCEPTIONALITY_INFERNAL;
                if (otmp->enchantment < 3)
                    otmp->enchantment = 3;
            }

            if (otmp->oclass == ARMOR_CLASS && otmp->enchantment < 2)
                otmp->enchantment = 2;
        }
        else if (mtmp->data->geno & G_UNIQ)
        {
            if (is_weapon(otmp))
            {
                if (can_have_exceptionality(otmp) && otmp->oartifact == 0)
                {
                    if (mtmp->m_lev >= 30 && mtmp->data->maligntyp < 0 && Inhell && otmp->exceptionality != EXCEPTIONALITY_INFERNAL)
                        otmp->exceptionality = EXCEPTIONALITY_INFERNAL;
                    else if (mtmp->m_lev >= 20 && otmp->exceptionality < EXCEPTIONALITY_ELITE)
                        otmp->exceptionality = EXCEPTIONALITY_ELITE;
                    else if (mtmp->m_lev >= 10 && otmp->exceptionality < EXCEPTIONALITY_EXCEPTIONAL)
                        otmp->exceptionality = EXCEPTIONALITY_EXCEPTIONAL;
                }
                if (mtmp->m_lev >= 30 && otmp->enchantment < 3)
                    otmp->enchantment = 3;
                else if (mtmp->m_lev >= 20 && otmp->enchantment < 2)
                    otmp->enchantment = 2;
                else if (mtmp->m_lev >= 10 && otmp->enchantment < 1)
                    otmp->enchantment = 1;
            }

            if (otmp->oclass == ARMOR_CLASS && otmp->enchantment < 0)
                otmp->enchantment = 0;
        }
        else if (is_prince(mtmp->data))
        {
            if (is_weapon(otmp))
            {
                if (can_have_exceptionality(otmp) && otmp->oartifact == 0 && otmp->exceptionality < EXCEPTIONALITY_EXCEPTIONAL)
                    otmp->exceptionality = EXCEPTIONALITY_EXCEPTIONAL;
                if (otmp->enchantment < 1)
                    otmp->enchantment = 1;
            }

            if (otmp->oclass == ARMOR_CLASS && otmp->enchantment < 0)
                otmp->enchantment = 0;
        }

        if (isok(mtmp->mx, mtmp->my) && (objects[otyp].oc_flags5 & O5_TILE_IS_TILESET_DEPENDENT))
        {
            otmp->has_special_tileset = 1;
            otmp->special_tileset = levl[mtmp->mx][mtmp->my].use_special_tileset ? levl[mtmp->mx][mtmp->my].special_tileset : get_current_cmap_type_index();
        }

        (void) mpickobj(mtmp, otmp); /* might free otmp */
    }

    return otmp;
}

struct obj*
mongets_noinit_item(mtmp, otyp, number)
struct monst* mtmp;
int otyp;
int number;
{
    struct obj* otmp;

    if (!otyp || !mtmp || number <= 0)
        return (struct obj*)0;

    otmp = mksobj(otyp, FALSE, FALSE, FALSE);
    
    if (otmp) 
    {
        if(objects[otmp->otyp].oc_merge && number > 1)
        {
            otmp->quan = number;
            otmp->owt = weight(otmp);
        }
        (void)mpickobj(mtmp, otmp); /* might free otmp */
        return otmp;
    }

    return (struct obj*)0;
}


struct obj*
mongets_spellbook(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return (struct obj*)0;
    struct obj* otmp = mkobj(SPBOOK_CLASS, FALSE, FALSE);
    if (otmp)
        (void)mpickobj(mtmp, otmp);
    return otmp;
}



int
golemhp(type)
int type;
{
    switch (type) {
    case PM_STRAW_GOLEM:
        return 20;
    case PM_PAPER_GOLEM:
        return 20;
    case PM_ROPE_GOLEM:
        return 30;
    case PM_LEATHER_GOLEM:
        return 40;
    case PM_GOLD_GOLEM:
        return 40;
    case PM_WOOD_GOLEM:
        return 50;
    case PM_FLESH_GOLEM:
        return 40;
    case PM_CLAY_GOLEM:
        return 50;
    case PM_STONE_GOLEM:
        return 60;
    case PM_GLASS_GOLEM:
        return 60;
    case PM_GEMSTONE_GOLEM:
        return 70;
    case PM_IRON_GOLEM:
        return 80;
    case PM_BONE_GOLEM:
        return 90;
    case PM_SILVER_GOLEM:
        return 160;
    default:
        return 0;
    }
}

/*
 *      Alignment vs. yours determines monster's attitude to you.
 *      (Some "animal" types are co-aligned, but also hungry.)
 */
boolean
peace_minded(ptr)
register struct permonst *ptr;
{
    aligntyp mal = ptr->maligntyp, ual = u.ualign.type;

    if (ptr == &mons[PM_YEENAGHU] && maybe_polyd(is_gnoll(youmonst.data), Race_if(PM_GNOLL)))
    {
        return TRUE;
    }

    if (always_peaceful(ptr))
        return TRUE;
    if (always_hostile(ptr))
        return FALSE;
    if (ptr->msound == MS_LEADER || ptr->msound == MS_GUARDIAN)
        return TRUE;
    if (ptr->msound == MS_NEMESIS)
        return FALSE;

    if (race_peaceful(ptr))
        return TRUE;
    if (race_hostile(ptr))
        return FALSE;

    /* the monster is hostile if its alignment is different from the
     * player's */
    if (sgn(mal) != sgn(ual))
        return FALSE;

    /* Negative monster hostile to player with Amulet. */
    if (mal < A_NEUTRAL && u.uhave.amulet)
        return FALSE;

    /* minions are hostile to players that have strayed at all */
    if (is_minion(ptr))
        return (boolean) (u.ualign.record >= 0);

    /* Last case:  a chance of a co-aligned monster being
     * hostile.  This chance is greater if the player has strayed
     * (u.ualign.record negative) or the monster is not strongly aligned.
     */
    return (boolean) (!!rn2(16 + (u.ualign.record < -15 ? -15 : u.ualign.record))
                      && !!rn2(2 + abs(mal)));
}

/* Set mhostility to have the proper effect on player alignment if monster is
 * killed.  Negative numbers mean it's bad to kill this monster; positive
 * numbers mean it's good.  Since there are more hostile monsters than
 * peaceful monsters, the penalty for killing a peaceful monster should be
 * greater than the bonus for killing a hostile monster to maintain balance.
 * Rules:
 *   it's bad to kill peaceful monsters, potentially worse to kill always-
 *      peaceful monsters;
 *   it's never bad to kill a hostile monster, although it may not be good.
 */
void
set_mhostility(mtmp)
struct monst *mtmp;
{
    schar mal = mtmp->data->maligntyp;
    boolean coaligned;

    if (mtmp->ispriest || mtmp->isminion) 
    {
        /* some monsters have individual alignments; check them */
        if (mtmp->ispriest && has_epri(mtmp))
            mal = EPRI(mtmp)->shralign;
        else if (mtmp->isminion && has_emin(mtmp))
            mal = EMIN(mtmp)->min_align;
        /* unless alignment is none, set mal to -5,0,5 */
        /* (see align.h for valid aligntyp values)     */
        if (mal != A_NONE)
            mal *= 5;
    }

    coaligned = (sgn(mal) == sgn(u.ualign.type));

    int absmal = abs(mal) + 1;
    if (mtmp->data->msound == MS_LEADER)
    {
        mtmp->mhostility = -20;
    } 
    else if (mal == A_NONE) 
    {
        if (is_peaceful(mtmp))
            mtmp->mhostility = 1;
        else
            mtmp->mhostility = 20; /* really hostile */
    }
    else if (always_peaceful(mtmp->data))
    {
        if (is_peaceful(mtmp))
            mtmp->mhostility = -3 * max(5, absmal);
        else
            mtmp->mhostility = 3 * max(5, absmal); /* renegade */
    } 
    else if (always_hostile(mtmp->data)) 
    {
        if (coaligned)
            mtmp->mhostility = 1;
        else
            mtmp->mhostility = max(5, absmal);
    }
    else if (coaligned) 
    {
        if (is_peaceful(mtmp))
            mtmp->mhostility = -3 * max(3, absmal);
        else /* renegade */
            mtmp->mhostility = max(3, absmal);
    } 
    else /* not coaligned and therefore hostile */
        mtmp->mhostility = absmal;
}

/* allocate a new mcorpsenm field for a monster; only need mextra itself */
void
newmcorpsenm(mtmp)
struct monst *mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();
    MCORPSENM(mtmp) = NON_PM; /* not initialized yet */
}

/* release monster's mcorpsenm field; basically a no-op */
void
freemcorpsenm(mtmp)
struct monst *mtmp;
{
    if (has_mcorpsenm(mtmp))
        MCORPSENM(mtmp) = NON_PM;
}

STATIC_VAR const NEARDATA char syms[] = {
    MAX_OBJECT_CLASSES,  MAX_OBJECT_CLASSES + 1, RING_CLASS,   WAND_CLASS,   WEAPON_CLASS,
    FOOD_CLASS,   COIN_CLASS,      SCROLL_CLASS, POTION_CLASS, ARMOR_CLASS,
    AMULET_CLASS, TOOL_CLASS,      ROCK_CLASS,   GEM_CLASS,    SPBOOK_CLASS,  
    REAGENT_CLASS, MISCELLANEOUS_CLASS, ART_CLASS,
    S_MIMIC_DEF,  S_MIMIC_DEF,
};

void
set_mimic_new_mobj(mtmp, otyp)
struct monst* mtmp;
int otyp;
{
    if (!mtmp)
        return;

    mtmp->m_ap_type = M_AP_OBJECT;
    mtmp->mappearance = otyp;
    struct obj* otmp = mksobj(otyp, TRUE, FALSE, 0);
    if (otmp)
    {
        if (otyp == GOLD_PIECE)
            set_random_gold_amount(otmp);
        if (has_mobj(mtmp))
            free_mobj(mtmp);
        if (!has_mobj(mtmp))
            newmobj(mtmp);
        if (has_mobj(mtmp))
        {
            *MOBJ(mtmp) = *otmp;
            MOBJ(mtmp)->oextra = 0;
            MOBJ(mtmp)->nobj = 0;
            MOBJ(mtmp)->nexthere = 0;
            MOBJ(mtmp)->cobj = 0;
            MOBJ(mtmp)->o_id = context.ident++;
            if (!MOBJ(mtmp)->o_id) /* ident overflowed */
                MOBJ(mtmp)->o_id = context.ident++;
            if (otmp->oextra)
                copy_oextra(MOBJ(mtmp), otmp);
        }
        /* make sure container contents are free'ed */
        obfree(otmp, (struct obj*)0);
    }
}

void
set_mimic_existing_mobj(mtmp, otmp)
struct monst* mtmp;
struct obj* otmp;
{
    if (!mtmp || !otmp)
        return;

    mtmp->m_ap_type = M_AP_OBJECT;
    mtmp->mappearance = otmp->otyp;
    if (has_mobj(mtmp))
        free_mobj(mtmp);
    if (!has_mobj(mtmp))
        newmobj(mtmp);
    if (has_mobj(mtmp))
    {
        *MOBJ(mtmp) = *otmp;
        MOBJ(mtmp)->oextra = 0;
        MOBJ(mtmp)->nobj = 0;
        MOBJ(mtmp)->nexthere = 0;
        MOBJ(mtmp)->cobj = 0;
        MOBJ(mtmp)->o_id = context.ident++;
        if (!MOBJ(mtmp)->o_id) /* ident overflowed */
            MOBJ(mtmp)->o_id = context.ident++;
        if (otmp->oextra)
            copy_oextra(MOBJ(mtmp), otmp);
        if (MOBJ(mtmp)->otyp == GOLD_PIECE)
            set_random_gold_amount(MOBJ(mtmp));
    }
}

void
set_mimic_sym(mtmp)
register struct monst *mtmp;
{
    int typ, roomno, rt;
    unsigned appear, ap_type;
    int s_sym;
    struct obj *otmp;
    int mx, my;
    int roll = -1;

    if (!mtmp || Protection_from_shape_changers)
        return;
    mx = mtmp->mx;
    my = mtmp->my;
    typ = levl[mx][my].typ;
    /* only valid for INSIDE of room */
    roomno = levl[mx][my].roomno - ROOMOFFSET;
    if (roomno >= 0)
        rt = rooms[roomno].rtype;
#ifdef SPECIALIZATION
    else if (IS_ROOM(typ))
        rt = OROOM, roomno = 0;
#endif
    else
        rt = 0; /* roomno < 0 case for GCC_WARN */

    if (OBJ_AT(mx, my)) {
        ap_type = M_AP_OBJECT;
        appear = level.objects[mx][my]->otyp;
        set_mimic_existing_mobj(mtmp, level.objects[mx][my]);
    } else if (IS_DOOR(typ) || IS_WALL(typ) || typ == SDOOR || typ == SCORR) {
        ap_type = M_AP_FURNITURE;
        /*
         *  If there is a wall to the left that connects to this
         *  location, then the mimic mimics a horizontal closed door.
         *  This does not allow doors to be in corners of rooms.
         *  Since rogue has no closed doors, mimic a wall there
         *  (yes, mimics can end up on this level by various means).
         */
        if (mx != 0 && (levl[mx - 1][my].typ == HWALL
                        || levl[mx - 1][my].typ == TLCORNER
                        || levl[mx - 1][my].typ == TRWALL
                        || levl[mx - 1][my].typ == BLCORNER
                        || levl[mx - 1][my].typ == TDWALL
                        || levl[mx - 1][my].typ == CROSSWALL
                        || levl[mx - 1][my].typ == TUWALL))
            appear = Is_really_rogue_level(&u.uz) ? S_hwall : S_hcdoor;
        else
            appear = Is_really_rogue_level(&u.uz) ? S_vwall : S_vcdoor;
    } else if (level.flags.is_maze_lev && !In_sokoban(&u.uz) && rn2(2)) {
        ap_type = M_AP_OBJECT;
        appear = STATUE;
    } else if (roomno < 0 && !t_at(mx, my)) {
        ap_type = M_AP_OBJECT;
        appear = BOULDER;
    } else if (rt == ZOO || rt == VAULT) {
        ap_type = M_AP_OBJECT;
        appear = GOLD_PIECE;
        set_mimic_new_mobj(mtmp, appear);
    } else if (rt == DELPHI) {
        if (rn2(2)) {
            ap_type = M_AP_OBJECT;
            appear = STATUE;
        } else {
            ap_type = M_AP_FURNITURE;
            appear = S_fountain;
        }
    } else if (rt == TEMPLE) {
        ap_type = M_AP_FURNITURE;
        appear = S_altar;
        /*
         * We won't bother with beehives, morgues, barracks, throne rooms
         * since they shouldn't contain too many mimics anyway...
         */
    }
    else if (rt == SMITHY) {
        ap_type = M_AP_FURNITURE;
        appear = S_anvil;
    } else if (rt >= SHOPBASE) {
        s_sym = get_shop_item(rt - SHOPBASE);
        if (s_sym < 0) {
            ap_type = M_AP_OBJECT;
            appear = -s_sym;
            set_mimic_new_mobj(mtmp, appear);
        } else {
            if (s_sym == RANDOM_CLASS)
            {
                roll = rn2((int)sizeof(syms) - 2) + 2;
                s_sym = syms[roll];
            }
            goto assign_sym;
        }
    } else {
        roll = rn2((int)sizeof(syms));
        s_sym = syms[roll];
 assign_sym:
        if (s_sym == MAX_OBJECT_CLASSES || s_sym == MAX_OBJECT_CLASSES + 1) {
            ap_type = M_AP_FURNITURE;
            appear = (s_sym == MAX_OBJECT_CLASSES) ? S_upstair : S_dnstair;
        } else {
            ap_type = M_AP_OBJECT;
            if (s_sym == S_MIMIC_DEF) {
                appear = STRANGE_OBJECT;
            } else if (s_sym == COIN_CLASS) {
                appear = GOLD_PIECE;
            } else {
                Sprintf(debug_buf_2, "set_mimic_sym, s_sym=%d,roll=%d", s_sym, roll);
                otmp = mkobj((char) s_sym, FALSE, FALSE);
                appear = otmp->otyp;
                set_mimic_existing_mobj(mtmp, otmp);
                if (has_mobj(mtmp))
                    free_mobj(mtmp);
                if(!has_mobj(mtmp))
                    newmobj(mtmp);
                if (has_mobj(mtmp))
                {
                    *MOBJ(mtmp) = *otmp;
                    MOBJ(mtmp)->oextra = 0;
                    MOBJ(mtmp)->nobj = 0;
                    MOBJ(mtmp)->nexthere = 0;
                    MOBJ(mtmp)->cobj = 0;
                    MOBJ(mtmp)->o_id = context.ident++;
                    if (!MOBJ(mtmp)->o_id) /* ident overflowed */
                        MOBJ(mtmp)->o_id = context.ident++;
                    if (otmp->oextra)
                        copy_oextra(MOBJ(mtmp), otmp);
                }
                /* make sure container contents are free'ed */
                obfree(otmp, (struct obj *) 0);
            }
        }
    }
    mtmp->m_ap_type = ap_type;
    mtmp->mappearance = appear;
    /* when appearing as an object based on a monster type, pick a shape */
    if (ap_type == M_AP_OBJECT
        && (appear == STATUE || appear == FIGURINE
            || appear == CORPSE || appear == EGG || appear == TIN)) 
    {
        int mndx;
        if (has_mobj(mtmp) && MOBJ(mtmp)->otyp == (short)appear)
            mndx = MOBJ(mtmp)->corpsenm;
        else
        {
            mndx = rndmonnum();
            int nocorpse_ndx = (mvitals[mndx].mvflags & MV_NOCORPSE) != 0;

            if (appear == CORPSE && nocorpse_ndx)
                mndx = rn1(PM_WIZARD - PM_ARCHAEOLOGIST + 1, PM_ARCHAEOLOGIST);
            else if ((appear == EGG && !can_be_hatched(mndx))
                || (appear == TIN && nocorpse_ndx))
                mndx = NON_PM; /* revert to generic egg or empty tin */
        }
        newmcorpsenm(mtmp);
        if(has_mcorpsenm(mtmp))
            MCORPSENM(mtmp) = mndx;
    }

    if (does_block(mx, my, &levl[mx][my]))
        block_vision_and_hearing_at_point(mx, my);
}

/* release monster from bag of tricks; return number of monsters created */
int
bagotricks(bag, tipping, seencount)
struct obj *bag;
boolean tipping; /* caller emptying entire contents; affects shop handling */
int *seencount;  /* secondary output */
{
    int moncount = 0;

    if (!bag || bag->otyp != BAG_OF_TRICKS) {
        impossible("bad bag o' tricks");
    } else if (bag->charges < 1) {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        /* if tipping known empty bag, give normal empty container message */
        pline1((tipping && bag->cknown) ? "It's empty." : nothing_happens);
        /* now known to be empty if sufficiently discovered */
        if (bag->dknown && objects[bag->otyp].oc_name_known)
            bag->cknown = 1;
    } else {
        struct monst *mtmp;
        int creatcnt = 1, seecount = 0;

        consume_obj_charge(bag, !tipping);

        if (!rn2(23))
            creatcnt += rnd(7);
        do {
            mtmp = makemon2((struct permonst *) 0, u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_IN_SMOKE_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END, MM2_RANDOMIZE_SUBTYPE);
            if (mtmp) {
                ++moncount;
                if (canspotmon(mtmp))
                    ++seecount;
            }
        } while (--creatcnt > 0);
        if (seecount) {
            if (seencount)
                *seencount += seecount;
            if (bag->dknown)
                makeknown(BAG_OF_TRICKS);
        } else if (!tipping) {
            pline1(!moncount ? nothing_happens : "Nothing seems to happen.");
        }
    }
    return moncount;
}


struct monst*
make_level_monster(x, y, mmflags, mmflags2)
register int x, y;
unsigned long mmflags, mmflags2; 
{
    if (level.flags.nmgeninfos <= 0)
    {
        return makemon2((struct permonst*)0, x, y, mmflags, mmflags2);
    }

    int i, totalprob = 0;
    for (i = 0; i < level.flags.nmgeninfos; i++)
    {
        totalprob += level.flags.mon_gen_infos[i].probability;
    }
    
    if (totalprob <= 0)
    {
        /* Do not generate anything */
        return (struct monst*)0;
    }

    int roll, trycnt = 0, sel_index = -1;
    struct monst* mtmp = 0;
    do
    {
        if (totalprob < 2)
            roll = 0;
        else
            roll = rn2(totalprob);

        sel_index = -1;
        for (i = level.flags.nmgeninfos - 1; i >= 0; i--)
        {
            roll -= level.flags.mon_gen_infos[i].probability;
            if (roll < 0)
            {
                sel_index = i;
                break;
            }
        }

        if (sel_index >= 0)
        {
            if (level.flags.mon_gen_infos[sel_index].mnum >= LOW_PM)
            {
                mtmp = makemon2(&mons[level.flags.mon_gen_infos[sel_index].mnum], x, y, mmflags, mmflags2);
            }
            else if (level.flags.mon_gen_infos[sel_index].mclass > 0 && level.flags.mon_gen_infos[sel_index].mclass < MAX_MONSTER_CLASSES)
            {
                struct permonst* pm = mkclass(level.flags.mon_gen_infos[sel_index].mclass, 0);
                if(pm)
                    mtmp = makemon2(pm, x, y, mmflags, mmflags2);
            }
            else
            {
                mtmp = makemon2((struct permonst*)0, x, y, mmflags, mmflags2);
            }

            if (mtmp)
            {
                return mtmp;
            }
        }

        trycnt++;
    } while (trycnt < 50);

    return (struct monst*)0;
}

struct monst*
make_level_monster_anywhere(VOID_ARGS)
{
    return make_level_monster(0, 0, NO_MM_FLAGS, MM2_RANDOMIZE_SUBTYPE);
}

void
reset_makemon(VOID_ARGS)
{
    oldmoves = 0L;
    align_lev = 0;
    reset_rndmonst(NON_PM);
}

void
newmmonst(mtmp)
struct monst* mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();
    if (!MMONST(mtmp)) {
        MMONST(mtmp) = (struct monst*)alloc(sizeof(struct monst));
        (void)memset((genericptr_t)MMONST(mtmp), 0, sizeof(struct monst));
    }
}

void
free_mmonst(mtmp)
struct monst* mtmp;
{
    if (has_mmonst(mtmp)) {
        if (MMONST(mtmp)->mextra)
            dealloc_mextra(MMONST(mtmp));
        free((genericptr_t)MMONST(mtmp));
        MMONST(mtmp) = (struct monst*)0;
    }
}

void
newmobj(mtmp)
struct monst* mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();
    if (!MOBJ(mtmp)) {
        MOBJ(mtmp) = (struct obj*)alloc(sizeof(struct obj));
        (void)memset((genericptr_t)MOBJ(mtmp), 0, sizeof(struct obj));
    }
}

void
free_mobj(mtmp)
struct monst* mtmp;
{
    if (has_mobj(mtmp)) {
        if (MOBJ(mtmp)->oextra)
            dealloc_oextra(MOBJ(mtmp));
        free((genericptr_t)MOBJ(mtmp));
        MOBJ(mtmp) = (struct obj*)0;
    }
}

void
save_mmonst(mon, mon_mmonst)
struct monst* mon; /* Save to this mon's mextra */
struct monst* mon_mmonst;
{
    if (mon_mmonst->ispriest)
        forget_temple_entry(mon_mmonst); /* EPRI() */
    if (mon_mmonst->issmith)
        forget_smithy_entry(mon_mmonst); /* ESMI() */
    if (mon_mmonst->isnpc)
        forget_npc_entry(mon_mmonst); /* ENPC() */
    if (!has_mmonst(mon))
        newmmonst(mon);
    if (has_mmonst(mon))
    {
        struct monst* mtmp2 = MMONST(mon);

        *mtmp2 = *mon_mmonst;
        mtmp2->mextra = (struct mextra*)0;

        /* invalidate pointers */
        /* m_id is needed to know if this is a revived quest leader */
        /* but m_id must be cleared when loading bones */
        mtmp2->nmon = (struct monst*)0;
        //mtmp2->data = (struct permonst *) 0; /* This sounds very dangerous to set to zero */
        mtmp2->minvent = (struct obj*)0;
        if (mon_mmonst->mextra)
            copy_mextra(mtmp2, mon_mmonst);
    }
}

/* returns a pointer to a new monst structure based on
 * the one contained within the obj.
 */
struct monst*
get_mmonst(mon, copyof)
struct monst* mon;
boolean copyof;
{
    struct monst* mon_mmonst = (struct monst*)0;
    struct monst* mnew = (struct monst*)0;

    if (has_mmonst(mon))
        mon_mmonst = MMONST(mon);
    if (mon_mmonst) {
        if (copyof) {
            mnew = newmonst();
            *mnew = *mon_mmonst;
            mnew->mextra = (struct mextra*)0;
            if (mon_mmonst->mextra)
                copy_mextra(mnew, mon_mmonst);
        }
        else {
            /* Never insert this returned pointer into mon chains! */
            mnew = mon_mmonst;
        }

        if (mnew && !mnew->data)
            mnew->data = &mons[mnew->mnum];
    }

    return mnew;
}

void
save_mobj(mon, obj_mobj)
struct monst* mon; /* Save to this mon's mextra */
struct obj* obj_mobj;
{
    if (!has_mobj(mon))
        newmobj(mon);
    if (has_mobj(mon))
    {
        struct obj* obj2 = MOBJ(mon);

        *obj2 = *obj_mobj;
        obj2->oextra = (struct oextra*)0;

        /* invalidate pointers */
        /* m_id is needed to know if this is a revived quest leader */
        /* but m_id must be cleared when loading bones */
        obj2->nobj = (struct obj*)0;
        obj2->nexthere = (struct obj*)0;
        //mtmp2->data = (struct permonst *) 0; /* This sounds very dangerous to set to zero */
        obj2->cobj = (struct obj*)0;
        if (obj_mobj->oextra)
            copy_oextra(obj2, obj_mobj);
    }
}

/* returns a pointer to a new monst structure based on
 * the one contained within the obj.
 */
struct obj*
get_mobj(mon, copyof)
struct monst* mon;
boolean copyof;
{
    struct obj* obj_mobj = (struct obj*)0;
    struct obj* onew = (struct obj*)0;

    if (has_mobj(mon))
        obj_mobj = MOBJ(mon);
    if (obj_mobj) {
        if (copyof) {
            onew = newobj();
            *onew = *obj_mobj;
            onew->oextra = (struct oextra*)0;
            if (obj_mobj->oextra)
                copy_oextra(onew, obj_mobj);
        }
        else {
            /* Never insert this returned pointer into obj chains! */
            onew = obj_mobj;
        }
    }

    return onew;
}

/*makemon.c*/
