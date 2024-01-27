/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    dogmove.c    $NHDT-Date: 1557094801 2019/05/05 22:20:01 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.74 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

#include "mfndpos.h"

extern boolean notonhead;

STATIC_DCL boolean FDECL(dog_hunger, (struct monst *, struct edog *));
STATIC_DCL int FDECL(dog_invent, (struct monst *, struct edog *, int));
STATIC_DCL int FDECL(dog_goal, (struct monst *, struct edog *, int, int, int));
STATIC_DCL struct monst *FDECL(find_targ, (struct monst *, int, int, int));
STATIC_OVL int FDECL(find_friends, (struct monst *, struct monst *, int));
STATIC_DCL struct monst *FDECL(best_target, (struct monst *));
STATIC_DCL long FDECL(score_targ, (struct monst *, struct monst *));
STATIC_DCL boolean FDECL(can_reach_location, (struct monst *, XCHAR_P,
                                              XCHAR_P, XCHAR_P, XCHAR_P));
STATIC_DCL void FDECL(quickmimic, (struct monst *));
STATIC_DCL void FDECL(dog_corpse_after_effect, (struct monst*, struct obj*, UCHAR_P));
STATIC_DCL void FDECL(m_givit, (struct monst*, int, struct permonst*));


/* pick a carried item for pet to drop */
struct obj *
droppables(mon)
struct monst *mon;
{
    struct obj *obj, *wep, dummy, *pickaxe, *unihorn, *key;

    dummy = zeroobj;
    dummy.otyp = GOLD_PIECE; /* not STRANGE_OBJECT or tools of interest */
    dummy.oartifact = 1; /* so real artifact won't override "don't keep it" */
    pickaxe = unihorn = key = (struct obj *) 0;
    wep = MON_WEP(mon);

    if (is_animal(mon->data) || mindless(mon->data)) {
        /* won't hang on to any objects of these types */
        pickaxe = unihorn = key = &dummy; /* act as if already have them */
    } else {
        /* don't hang on to pick-axe if can't use one or don't need one */
        if (!tunnels(mon->data) || !needspick(mon->data))
            pickaxe = &dummy;
        /* don't hang on to key if can't open doors */
        if ((nohands(mon->data) && !is_telekinetic_operator(mon->data)) || verysmall(mon->data))
            key = &dummy;
    }
    if (wep) {
        if (is_pick(wep))
            pickaxe = wep;
        if (wep->otyp == UNICORN_HORN)
            unihorn = wep;
        /* don't need any wielded check for keys... */
    }

    for (obj = mon->minvent; obj; obj = obj->nobj) {
        switch (obj->otyp) {
        case SPADE_OF_COLOSSAL_EXCAVATION:
        case DWARVISH_MATTOCK:
            /* reject mattock if couldn't wield it */
            if (which_armor(mon, W_ARMS))
                break;
            /* keep mattock in preference to pick unless pick is already
               wielded or is an artifact and mattock isn't */
            if (pickaxe && pickaxe->otyp == PICK_AXE && pickaxe != wep
                && (!pickaxe->oartifact || obj->oartifact))
                return pickaxe; /* drop the one we earlier decided to keep */
        /*FALLTHRU*/
        case PICK_AXE:
            if (!pickaxe || (obj->oartifact && !pickaxe->oartifact)) {
                if (pickaxe)
                    return pickaxe;
                pickaxe = obj; /* keep this digging tool */
                continue;
            }
            break;

        case UNICORN_HORN:
            /* reject cursed unicorn horns */
            if (obj->cursed)
                break;
            /* keep artifact unihorn in preference to ordinary one */
            if (!unihorn || (obj->oartifact && !unihorn->oartifact)) {
                if (unihorn)
                    return unihorn;
                unihorn = obj; /* keep this unicorn horn */
                continue;
            }
            break;

        case SKELETON_KEY:
            /* keep key in preference to lock-pick */
            if (key && key->otyp == LOCK_PICK
                && (!key->oartifact || obj->oartifact))
                return key; /* drop the one we earlier decided to keep */
        /*FALLTHRU*/
        case LOCK_PICK:
            /* keep lock-pick in preference to credit card */
            if (key && key->otyp == CREDIT_CARD
                && (!key->oartifact || obj->oartifact))
                return key;
        /*FALLTHRU*/
        case CREDIT_CARD:
            if (!key || (obj->oartifact && !key->oartifact)) {
                if (key)
                    return key;
                key = obj; /* keep this unlocking tool */
                continue;
            }
            break;

        default:
            break;
        }

        if (!obj->owornmask && obj != wep)
            return obj;
    }

    return (struct obj *) 0; /* don't drop anything */
}

struct obj*
m_has_wearable_armor_or_accessory(mon)
struct monst* mon;
{
    for (struct obj* obj = mon->minvent; obj; obj = obj->nobj) 
    {
        if (obj->oclass == ARMOR_CLASS || obj->oclass == AMULET_CLASS || obj->oclass == MISCELLANEOUS_CLASS || obj->oclass == RING_CLASS)
        {
            if ((obj->owornmask & (W_ARMOR | W_ACCESSORY)) == 0)
            {
                if (is_suit(obj) && !can_wear_suit(mon->data))
                    continue;
                if (is_robe(obj) && !can_wear_robe(mon->data))
                    continue;
                if (is_shirt(obj) && !can_wear_shirt(mon->data))
                    continue;
                if (is_cloak(obj) && !can_wear_cloak(mon->data))
                    continue;
                if (is_helmet(obj) && !can_wear_helmet(mon->data))
                    continue;
                if (is_gloves(obj) && !can_wear_gloves(mon->data))
                    continue;
                if (is_bracers(obj) && !can_wear_bracers(mon->data))
                    continue;
                if (is_boots(obj) && !can_wear_boots(mon->data))
                    continue;
                if (is_shield(obj) && !can_wear_shield(mon->data))
                    continue;
                if (is_amulet(obj) && !can_wear_amulet(mon->data))
                    continue;
                if (is_blindfold(obj) && !can_wear_blindfold(mon->data))
                    continue;
                if (obj->oclass == RING_CLASS && !can_wear_rings(mon->data))
                    continue;
                if (obj->oclass == MISCELLANEOUS_CLASS && !can_wear_miscellaneous(mon->data, obj->otyp))
                    continue;
                return obj;
            }
        }
    }

    return (struct obj*) 0;
}

struct obj*
m_has_worn_armor_or_accessory(mon)
struct monst* mon;
{
    for (struct obj* obj = mon->minvent; obj; obj = obj->nobj)
    {
        if (obj->oclass == ARMOR_CLASS || obj->oclass == AMULET_CLASS || obj->oclass == MISCELLANEOUS_CLASS || obj->oclass == RING_CLASS)
        {
            if ((obj->owornmask & (W_ARMOR | W_ACCESSORY)) != 0)
                return obj;
        }
    }

    return (struct obj*) 0;
}



STATIC_VAR NEARDATA const char nofetch[] = { BALL_CLASS, CHAIN_CLASS, ROCK_CLASS,
                                         0 };

STATIC_VAR xchar gtyp, gx, gy; /* type and position of dog's current goal */

STATIC_PTR void FDECL(wantdoor, (int, int, genericptr_t));

boolean
cursed_object_at(x, y)
int x, y;
{
    struct obj *otmp;

    for (otmp = level.objects[x][y]; otmp; otmp = otmp->nexthere)
        if (otmp->cursed)
            return TRUE;
    return FALSE;
}

int
dog_nutrition(mtmp, obj)
struct monst *mtmp;
struct obj *obj;
{
    int nutrit;
    int nutr_size_mult = (int)mon_nutrition_size_multiplier(mtmp);

    /*
     * It is arbitrary that the pet takes the same length of time to eat
     * as a human, but gets more nutritional value.
     */
    if (obj->oclass == FOOD_CLASS) 
    {
        boolean is_veg = FALSE;
        if (obj->otyp == CORPSE)
        {
            mtmp->meating = 3 + (mons[obj->corpsenm].cwt >> 6);
            nutrit = mons[obj->corpsenm].cnutrit;
            if (obj->corpsenm >= LOW_PM && (is_vegetarian_food(&mons[obj->corpsenm]) || is_vegan_food(&mons[obj->corpsenm])))
                is_veg = TRUE;
        } 
        else 
        {
            mtmp->meating = objects[obj->otyp].oc_delay;
            nutrit = objects[obj->otyp].oc_nutrition;
            if(obj->otyp == TIN && (obj->special_quality == SPEQUAL_TIN_CONTAINS_SPINACH || (obj->corpsenm >= LOW_PM && (is_vegetarian_food(&mons[obj->corpsenm]) || is_vegan_food(&mons[obj->corpsenm])))))
                is_veg = TRUE;
            else if(obj->material == MAT_VEGGY)
                is_veg = TRUE;
        }

        nutrit *= nutr_size_mult;

        if(herbivorous(mtmp->data) && is_veg)
            nutrit *= 4;

        if (obj->oeaten)
        {
            mtmp->meating = eaten_stat(mtmp->meating, obj);
            nutrit = eaten_stat(nutrit, obj);
        }
    } 
    else if (obj->oclass == COIN_CLASS) 
    {
        mtmp->meating = (int) (obj->quan / 2000) + 1;
        if (mtmp->meating < 0)
            mtmp->meating = 1;
        nutrit = (int) (obj->quan / 2);
        nutrit *= nutr_size_mult;
    }
    else if (obj->oclass == POTION_CLASS)
    {
        mtmp->meating = 1;
        int nutrdicebuc = (int)objects[obj->otyp].oc_potion_nutrition_dice_buc_multiplier;
        int nutrition = (int)d(max(0, objects[obj->otyp].oc_potion_nutrition_dice + nutrdicebuc * bcsign(obj)), 
            max(1, objects[obj->otyp].oc_potion_nutrition_diesize)) 
            + objects[obj->otyp].oc_potion_nutrition_plus + bcsign(obj) * (int)objects[obj->otyp].oc_potion_nutrition_buc_multiplier;
        nutrit = nutrition * nutr_size_mult;
    }
    else
    {
        /* Unusual pet such as gelatinous cube eating odd stuff.
         * meating made consistent with wild monsters in mon.c.
         * nutrit made consistent with polymorphed player nutrit in
         * eat.c.  (This also applies to pets eating gold.)
         */
        mtmp->meating = obj->owt / 20 + 1;
        nutrit = nutr_size_mult * (int)obj_nutrition(obj, mtmp); // objects[obj->otyp].oc_nutrition;
    }
    if (nutrit < 0)
        nutrit = 0;
    return nutrit;
}

/* returns 2 if pet dies, otherwise 1 */
int
dog_eat(mtmp, obj, x, y, devour)
register struct monst *mtmp;
register struct obj *obj; /* if unpaid, then thrown or kicked by hero */
int x, y; /* dog's starting location, might be different from current */
boolean devour;
{
    if (!mtmp || !obj)
        return 0;

    register struct edog* edog = EDOG(mtmp);
    boolean poly, grow, heal, eyes, slimer, deadmimic, catavenged, curepetrification;
    int nutrit;
    long oprice;
    char objnambuf[BUFSZ];

    objnambuf[0] = '\0';
    if (edog->hungrytime < monstermoves)
        edog->hungrytime = monstermoves;
    nutrit = dog_nutrition(mtmp, obj);

    deadmimic = (obj->otyp == CORPSE && obj->corpsenm >= LOW_PM && is_mimic(&mons[obj->corpsenm]));
    slimer = (obj->otyp == CORPSE && obj->corpsenm == PM_GREEN_SLIME) && !has_unchanging(mtmp) && !resists_slime(mtmp);
    poly = polyfodder(obj) && !has_unchanging(mtmp);
    grow = mlevelgain(obj);
    heal = mhealup(obj);
    curepetrification = mcurepetrification(obj);
    eyes = (obj->otyp == CARROT);
    catavenged = ((is_domestic(mtmp->data) && mtmp->data->mlet == S_FELINE) && (obj->otyp == CORPSE && obj->corpsenm >= LOW_PM && is_quantum_mechanic(&mons[obj->corpsenm])));

    if (devour) 
    {
        if (mtmp->meating > 1)
            mtmp->meating /= 2;
        if (nutrit > 1)
            nutrit = (nutrit * 3) / 4;
    }
    edog->hungrytime += nutrit;
    mtmp->mprops[CONFUSION] &= ~M_INTRINSIC_ACQUIRED;
    if (edog->mhpmax_penalty) 
    {
        /* no longer starving */
        mtmp->mhpmax += edog->mhpmax_penalty;
        edog->mhpmax_penalty = 0;
    }

    if (mtmp->mflee && mtmp->mflee_timer > 1)
        mtmp->mflee_timer /= 2;
    if (mtmp->mtame > 0 && mtmp->mtame < 20)
        mtmp->mtame++;

    if(canspotmon(mtmp))
        display_nutrition_floating_text(mtmp->mx, mtmp->my, nutrit);

    if (x != mtmp->mx || y != mtmp->my)
    { /* moved & ate on same turn */
        newsym(x, y);
        newsym(mtmp->mx, mtmp->my);
    }

    /* food items are eaten one at a time; entire stack for other stuff */
    if (obj->quan > 1L && obj->oclass == FOOD_CLASS)
        obj = splitobj(obj, 1L);
    
    if (obj->unpaid)
        iflags.suppress_price++;

    if(obj)
        play_occupation_immediate_sound_at_location(objects[obj->otyp].oc_soundset, OCCUPATION_EATING, OCCUPATION_SOUND_TYPE_START, x, y);


    if (is_pool(x, y) && !Underwater)
    {
        /* Don't print obj */
        /* TODO: Reveal presence of sea monster (especially sharks) */
    }
    else 
    {
        /* food is at monster's current location, <mx,my>;
           <x,y> was monster's location at start of this turn;
           they might be the same but will be different when
           the monster is moving+eating on same turn */
        boolean seeobj = cansee(mtmp->mx, mtmp->my),
                sawpet = cansee(x, y) && mon_visible(mtmp);

        /* Observe the action if either the food location or the pet
           itself is in view.  When pet which was in view moves to an
           unseen spot to eat the food there, avoid referring to that
           pet as "it".  However, we want "it" if invisible/unsensed
           pet eats visible food. */
        if (sawpet || (seeobj && canspotmon(mtmp)))
        {
            //if (tunnels(mtmp->data))
            //    pline("%s digs in.", noit_Monnam(mtmp));
            //else
            {
                pline_ex(ATR_NONE, devour ? CLR_MSG_ATTENTION : NO_COLOR, "%s %s %s.", noit_Monnam(mtmp),
                    devour ? "devours" : "eats", distant_name(obj, doname));

                if (catavenged)
                    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "Schroedinger's cat has been avenged.");
            }
        } 
        else if (seeobj)
            pline_ex(ATR_NONE, devour ? CLR_MSG_ATTENTION : NO_COLOR, "It %s %s.", devour ? "devours" : "eats",
                  distant_name(obj, doname));
    }

    if (obj->unpaid)
    {
        Strcpy(objnambuf, xname(obj));
        iflags.suppress_price--;
    }

    /* It's a reward if it's DOGFOOD and the player dropped/threw it.
       We know the player had it if invlet is set. -dlc */
    if (dogfood(mtmp, obj) == DOGFOOD && obj->invlet)
#ifdef LINT
        edog->apport = 0;
#else
        edog->apport += (int) (200L / ((long) edog->dropdist + monstermoves
                                       - edog->droptime));
#endif


    if (rust_causing_and_ironvorous(mtmp->data) && obj->oerodeproof) 
    {
        /* The object's rustproofing is gone now */
        if (obj->unpaid)
            costly_alteration(obj, COST_DEGRD);
        obj->oerodeproof = 0;
        increase_mon_property(mtmp, STUNNED, 5 + rnd(10));
        if (canseemon(mtmp) && flags.verbose) 
        {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s spits %s out in disgust!", Monnam(mtmp),
                  distant_name(obj, doname));
        }
    } 
    else if (obj == uball) 
    {
        unpunish();
        delobj(obj); /* we assume this can't be unpaid */
    } 
    else if (obj == uchain) 
    {
        unpunish();
    } 
    else 
    {
        /* Dog food after effects */
        if (is_obj_rotting_corpse(obj))
            dog_corpse_after_effect(mtmp, obj, (uchar)is_female_corpse_or_statue(obj));
        else
            dog_food_after_effect(mtmp, obj, canseemon(mtmp));

        if (obj->unpaid)
        {
            /* edible item owned by shop has been thrown or kicked
               by hero and caught by tame or food-tameable monst */
            oprice = unpaid_cost(obj, TRUE);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "That %s will cost you %ld %s.", objnambuf, oprice,
                  currency(oprice));
            /* delobj->obfree will handle actual shop billing update */
        }
        if (obj->otyp == STATUE)
            pre_break_statue(obj);

        delobj(obj);
    }

#if 0 /* pet is eating, so slime recovery is not feasible... */
    /* turning into slime might be cureable */
    if (slimer && munslime(mtmp, FALSE)) {
        /* but the cure (fire directed at self) might be fatal */
        if (DEADMONSTER(mtmp))
            return 2;
        slimer = FALSE; /* sliming is avoided, skip polymorph */
    }
#endif


    /* Original food effects */
    if (curepetrification)
    {
        if (has_stoned(mtmp))
        {
            (void)set_mon_property_b(mtmp, STONED, 0, canseemon(mtmp));
            if (canseemon(mtmp))
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s looks limber!", Monnam(mtmp));
        }

        increase_mon_property(mtmp, STONE_RESISTANCE, 13);
        newsym(mtmp->mx, mtmp->my);
    }

    if (poly || slimer)
    {
        struct permonst *ptr = slimer ? &mons[PM_GREEN_SLIME] : 0;
        (void) newcham(mtmp, ptr, 0, FALSE, cansee(mtmp->mx, mtmp->my));
    }

    /* limit "instant" growth to prevent potential abuse */
    if (grow && (int) mtmp->m_lev < (int) mtmp->data->mlevel + 15) 
    {
        if (!grow_up(mtmp, (struct monst *) 0))
            return 2;
    }
    if (heal) {
        mtmp->mhp = mtmp->mhpmax;
        if (iflags.wc2_statuslines > 3 && is_tame(mtmp))
            context.botl = 1;

    }
    if ((eyes || heal) && has_blinded(mtmp))
        mcureblindness(mtmp, canseemon(mtmp));
    if (deadmimic)
        quickmimic(mtmp);

    return 1;
}


/* called after consuming (non-corpse) food */
void
dog_food_after_effect(mtmp, otmp, verbose)
struct monst* mtmp;
struct obj* otmp;
boolean verbose;
{
    if (!mtmp || !otmp)
        return;

    if (!is_obj_normally_edible(otmp))
        return;

    if (rn2(100) >= objects[otmp->otyp].oc_effect_probability)
        return;

    /* Properties */
    int duration = d(objects[otmp->otyp].oc_spell_dur_dice, objects[otmp->otyp].oc_spell_dur_diesize) + objects[otmp->otyp].oc_spell_dur_plus;
    if (objects[otmp->otyp].oc_edible_effect > 0)
    {
        if (duration > 0)
            (void)set_mon_property_b(mtmp, (int)objects[otmp->otyp].oc_edible_effect, duration, verbose);
        else
            (void)set_mon_property_b(mtmp, (int)objects[otmp->otyp].oc_edible_effect, -1, verbose);
    }

    switch (objects[otmp->otyp].oc_edible_effect)
    {
    case EDIBLEFX_CURE_LYCANTHROPY:
        /* Nothing currently */
        break;
    case EDIBLEFX_CURE_BLINDNESS:
        mcureblindness(mtmp, canseemon(mtmp));
        break;
    case EDIBLEFX_READ_FORTUNE:
        /* Nothing currently */
        break;
    case EDIBLEFX_ROYAL_JELLY:
    {
        /* This stuff seems to be VERY healthy! */
        m_gainstr(mtmp, otmp, 1, TRUE);
        mtmp->mhp += otmp->cursed ? -rnd(20) : rnd(20);
        if (mtmp->mhp > mtmp->mhpmax)
        {
            if (!rn2(17))
                mtmp->mbasehpmax++;
            update_mon_maxhp(mtmp);
            mtmp->mhp = mtmp->mhpmax;
        }
        else if (DEADMONSTER(mtmp))
        {
            xkilled(mtmp, XKILL_NOMSG);
        }
        if (!otmp->cursed)
            (void)set_mon_property_b(mtmp, WOUNDED_LEGS, 0, FALSE);
        if (iflags.wc2_statuslines > 3 && is_tame(mtmp))
            context.botl = 1;

        break;
    }
    case EDIBLEFX_GAIN_STRENGTH:
        m_gainstr(mtmp, otmp, 1, TRUE);
        break;
    case EDIBLEFX_GAIN_DEXTERITY:
        (void)m_adjattrib(mtmp, A_DEX, (otmp && otmp->cursed) ? -1 : (otmp && otmp->blessed) ? rnd(2) : 1, TRUE);
        break;
    case EDIBLEFX_GAIN_CONSTITUTION:
        (void)m_adjattrib(mtmp, A_CON, (otmp && otmp->cursed) ? -1 : (otmp && otmp->blessed) ? rnd(2) : 1, TRUE);
        break;
    case EDIBLEFX_GAIN_INTELLIGENCE:
        (void)m_adjattrib(mtmp, A_INT, (otmp && otmp->cursed) ? -1 : (otmp && otmp->blessed) ? rnd(2) : 1, TRUE);
        break;
    case EDIBLEFX_GAIN_WISDOM:
        (void)m_adjattrib(mtmp, A_WIS, (otmp && otmp->cursed) ? -1 : (otmp && otmp->blessed) ? rnd(2) : 1, TRUE);
        break;
    case EDIBLEFX_GAIN_CHARISMA:
        (void)m_adjattrib(mtmp, A_CHA, (otmp && otmp->cursed) ? -1 : (otmp && otmp->blessed) ? rnd(2) : 1, TRUE);
        break;
    case EDIBLEFX_RESTORE_ABILITY:
    {
        if (otmp->cursed)
        {
            if(verbose)
                pline("Ulch!  That made %s feel mediocre!", mon_nam(mtmp));
            break;
        }
        else
        {
            int i, ii, lim;
            pline("Wow!  This made %s feel %s!", mon_nam(mtmp),
                (otmp->blessed)
                ? (unfixable_trouble_count(FALSE) ? "better" : "great")
                : "good");
            i = rn2(A_MAX); /* start at a random point */
            for (ii = 0; ii < A_MAX; ii++) 
            {
                lim = M_AMAX(mtmp, i);
                /* this used to adjust 'lim' for A_STR when u.uhs was
                   WEAK or worse, but that's handled via ATEMP(A_STR) now */
                if (M_ABASE(mtmp, i) < lim)
                {
                    M_ABASE(mtmp, i) = lim;
                    /* only first found if not blessed */
                    if (!otmp->blessed)
                        break;
                }
                if (++i >= A_MAX)
                    i = 0;
            }
        }
        break;
    }
    case EDIBLEFX_GAIN_LEVEL:
    {
        if ((int)mtmp->m_lev < (int)mtmp->data->mlevel + 15)
        {
            (void)grow_up(mtmp, (struct monst*) 0);
        }
        break;
    }
    case EDIBLEFX_EGG:
        if (flesh_petrifies(&mons[otmp->corpsenm]))
        {
            if (!has_petrification_resistance(mtmp))
            {
                if (poly_when_stoned(mtmp->data))
                    mon_to_stone(mtmp);
                else
                    start_delayed_petrification(mtmp, FALSE);
            }
        }
        break;
    case EDIBLEFX_CURE_SICKNESS:
        if(!otmp->cursed)
            mcuresickness(mtmp, TRUE);
        break;
    case EDIBLEFX_APPLE:
        /* Nothing */
        break;
    case EDIBLEFX_CURE_PETRIFICATION:
        if (!otmp->cursed)
        {
            if (has_stoned(mtmp))
            {
                (void)set_mon_property_b(mtmp, STONED, 0, canseemon(mtmp));
                if(canseemon(mtmp))
                    pline("%s looks limber!", Monnam(mtmp));
            }

            increase_mon_property(mtmp, STONE_RESISTANCE, 13);
            newsym(mtmp->mx, mtmp->my);
        }
    }
    return;
}

STATIC_OVL void
m_givit(mon, type, ptr)
struct monst* mon;
int type;
register struct permonst* ptr;
{
    if (!mon || type < 1 || type > LAST_PROP)
        return;

    if (ptr)
    {
        register int chance;

        debugpline1("Attempting to give intrinsic %d", type);
        /* some intrinsics are easier to get than others */
        switch (type) {
        case POISON_RESISTANCE:
            if ((ptr == &mons[PM_KILLER_BEE] || ptr == &mons[PM_SCORPION])
                && !rn2(4))
                chance = 1;
            else
                chance = 15;
            break;
        case TELEPORT:
            chance = 10;
            break;
        case TELEPORT_CONTROL:
            chance = 12;
            break;
        case BLIND_TELEPATHY:
            chance = 1;
            break;
        case TELEPAT:
            chance = 15;
            break;
        default:
            chance = 15;
            break;
        }

        if (ptr->mlevel <= rn2(chance))
            return; /* failed die roll */
    }

    if (!(mon->mprops[type] & M_INTRINSIC_ACQUIRED))
        mon->mprops[type] |= M_INTRINSIC_ACQUIRED;
    else
        return;

    if (!canspotmon(mon))
        return;

    switch (type) 
    {
    case FIRE_RESISTANCE:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_FIRE_RESISTANCE, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, Hallucination ? "%s seems to be chillin'." : "%s seems to feel cold for a moment.", Monnam(mon));
        break;
    case SLEEP_RESISTANCE:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_SLEEP_RESISTANCE, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems wide awake.", Monnam(mon));
        break;
    case COLD_RESISTANCE:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_COLD_RESISTANCE, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems hot inside.", Monnam(mon));
        break;
    case ACID_RESISTANCE:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_ACID_RESISTANCE, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems less soluble.", Monnam(mon));
        break;
    case DISINTEGRATION_RESISTANCE:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_DISINTEGRATION_RESISTANCE, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, Hallucination ? "%s seems totally together, man." : "%s seems very firm.", Monnam(mon));
        break;
    case SHOCK_RESISTANCE: /* shock (electricity) resistance */
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_SHOCK_RESISTANCE, mon->mx, mon->my);
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems more grounded in reality.", Monnam(mon));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s health currently seems a bit more amplified!", s_suffix(Monnam(mon)));
        break;
    case DEATH_RESISTANCE: /* death resistance */
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_DEATH_RESISTANCE, mon->mx, mon->my);
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems immortal.", Monnam(mon));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s life force seems firmer!", s_suffix(Monnam(mon)));
        break;
    case CHARM_RESISTANCE: /* charm resistance */
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_CHARM_RESISTANCE, mon->mx, mon->my);
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s understanding of the world seems to have improved!", s_suffix(Monnam(mon)));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems more firm about %s own motivations.", Monnam(mon), mhis(mon));
        break;
    case FEAR_RESISTANCE: /* fear resistance */
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_FEAR_RESISTANCE, mon->mx, mon->my);
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems like Thelma and Louise!", Monnam(mon));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems courageous.", Monnam(mon));
        break;
    case MIND_SHIELDING: /* mind shielding */
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_MIND_SHIELDING, mon->mx, mon->my);
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems more secure!", Monnam(mon));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems more shielded.", Monnam(mon));
        break;
    case LYCANTHROPY_RESISTANCE: /* immunity to lycanthropy */
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_LYCANTHROPY_RESISTANCE, mon->mx, mon->my);
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems unbothered by bugs!", Monnam(mon));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems less lunatic.", Monnam(mon));
        break;
    case CURSE_RESISTANCE: /* protection from curses */
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_CURSE_RESISTANCE, mon->mx, mon->my);
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems unbothered by auras!", Monnam(mon));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems protected from curses.", Monnam(mon));
        break;
    case POISON_RESISTANCE:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_CURSE_RESISTANCE, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, Poison_resistance ? "%s seems especially healthy." : "%s seems healthy.", Monnam(mon));
        break;
    case STONE_RESISTANCE:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_STONE_RESISTANCE, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, Stone_resistance ? "%s seems especially limber." : "%s seems limber.", Monnam(mon));
        break;
    case TELEPORT:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_TELEPORT, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, Hallucination ? "%s seems diffuse." : "%s seems very jumpy.", Monnam(mon));
        break;
    case TELEPORT_CONTROL:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_TELEPORT_CONTROL, mon->mx, mon->my);
        if (Hallucination)
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems centered in %s personal space.", Monnam(mon), mhis(mon));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems in control of %sself.", Monnam(mon), mhim(mon));
        break;
    case BLIND_TELEPATHY:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_BLIND_TELEPATHY, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, Hallucination ? "%s seems to be in touch with the cosmos." : "%s seems to experience a strange mental acuity.", Monnam(mon));
        break;
    case TELEPAT:
        play_sfx_sound_at_location(SFX_INTRINSIC_ACQUIRED_TELEPATHY, mon->mx, mon->my);
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, Hallucination ? "%s seems to be in touch with the cosmos." : "%s seems to experience a peculiar mental acuity.", Monnam(mon));
        break;
    default:
        break;
    }
}


/* called after completely consuming a corpse */
STATIC_OVL void
dog_corpse_after_effect(mon, obj, gender)
struct monst* mon;
struct obj* obj;
uchar gender UNUSED; /* 0 = male, 1 = female, 2 = unknown */
{
    if (!mon || !obj)
        return;

    int tmp = 0;
    int catch_lycanthropy = NON_PM;
    boolean check_intrinsics = FALSE;
    int pm = obj->corpsenm;

    if (pm < LOW_PM || pm >= NUM_MONSTERS)
        return;

    struct permonst* ptr = &mons[pm];
    boolean donotcheckfurther = FALSE;
    if (has_hallucinating_corpse(ptr))
    {
        if (canspotmon(mon))
        {
            play_sfx_sound_at_location(SFX_ACQUIRE_HALLUCINATION, mon->mx, mon->my);
            pline_ex(ATR_NONE, CLR_MSG_HALLUCINATED, "%s seems hallucinated.", Monnam(mon));
        }
        increase_mon_property_b(mon, HALLUC, 200L, canspotmon(mon));
        donotcheckfurther = TRUE;
    }

    if (has_stunning_corpse(ptr) && !has_stun_resistance(mon))
    {
        if (!is_stunned(mon))
            play_sfx_sound_at_location(SFX_ACQUIRE_STUN, mon->mx, mon->my);
        increase_mon_property_b(mon, STUNNED, mons[pm].mlevel * 2 + 5 + rnd(20), canspotmon(mon));
        donotcheckfurther = TRUE;
    }

    if (conveys_see_invisible(ptr))
    {
        if (!has_see_invisible(mon))
        {
            //First temporary
            set_mon_property_b(mon, SEE_INVISIBLE, rn1(100, 50), canspotmon(mon));
        }
        else
        {
            //Then permanent
            set_mon_property_b(mon, SEE_INVISIBLE, -1, canspotmon(mon));
        }
        donotcheckfurther = TRUE;
    }

    if (conveys_invisibility(ptr))
    {
        if (!has_invisibility(mon))
        {
            //First temporary
            set_mon_property_b(mon, INVISIBILITY, rn1(100, 50), canspotmon(mon));
        }
        else
        {
            //Then permanent
            set_mon_property_b(mon, INVISIBILITY, -1, canspotmon(mon));
        }
        newsym(mon->mx, mon->my);
        donotcheckfurther = TRUE;
    }

    if (donotcheckfurther)
        return;

    switch (pm) {
    case PM_NEWT:
        /* Nothing for monsters */
        break;
    case PM_HUMAN_WERERAT:
        catch_lycanthropy = PM_WERERAT;
        break;
    case PM_HUMAN_WEREJACKAL:
        catch_lycanthropy = PM_WEREJACKAL;
        break;
    case PM_HUMAN_WEREWOLF:
        catch_lycanthropy = PM_WEREWOLF;
        break;
    case PM_HUMAN_WEREBEAR:
        catch_lycanthropy = PM_WEREBEAR;
        break;
    case PM_NURSE:
        deduct_monster_hp(mon, -1000);
        mcureblindness(mon, canspotmon(mon));
        refresh_m_tile_gui_info(mon, TRUE);
        check_intrinsics = TRUE; /* might also convey poison resistance */
        break;
    case PM_CHAOS_MIMIC:
        tmp += 10;
        /*FALLTHRU*/
    case PM_GARGANTUAN_MIMIC:
        tmp += 10;
        /*FALLTHRU*/
    case PM_GIANT_MIMIC:
        tmp += 10;
        /*FALLTHRU*/
    case PM_LARGE_MIMIC:
        tmp += 20;
        /*FALLTHRU*/
    case PM_SMALL_MIMIC:
        tmp += 20;
        if (!is_mimic(mon->data) && !has_unchanging(mon)) {
            //Handled in the calling function
            //char buf[BUFSZ];

            //if(canspotmon(mon))
            //    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s can't resist the temptation to mimic %s.", Monnam(mon),
            //        has_hallucination(mon) ? "an orange" : "a pile of gold");
            ///* A pile of gold can't ride. */
            ///* ??? what if this was set before? */
            //newcham(mon, pm, 0, FALSE, FALSE);
            //mon->m_ap_type = M_AP_OBJECT;
            //mon->mappearance = has_hallucination(mon) ? ORANGE : GOLD_PIECE;
            //newsym(mon->mx, mon->my);
        }
        break;
    case PM_QUANTUM_MECHANIC:
    case PM_ELDER_QUANTUM_MECHANIC:
        if (canspotmon(mon))
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "The velocity of %s suddenly seems very uncertain!", mon_nam(mon));
        if (mon->mprops[FAST] & M_INTRINSIC_ACQUIRED) 
        {
            mon->mprops[FAST] &= ~M_INTRINSIC_ACQUIRED;
            if (canspotmon(mon))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems slower.", Monnam(mon));
        }
        else 
        {
            mon->mprops[FAST] |= M_INTRINSIC_ACQUIRED;
            if (canspotmon(mon))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s seems faster.", Monnam(mon));
        }
        break;
    case PM_LIZARD:
        if ((mon->mprops[STUNNED] & M_TIMEOUT) > 2)
            set_mon_property(mon, STUNNED, 2L);
        if ((mon->mprops[CONFUSION] & M_TIMEOUT) > 2)
            set_mon_property(mon, CONFUSION, 2L);
        break;
    case PM_CHAMELEON:
    case PM_DOPPELGANGER:
    case PM_SANDESTIN: /* moot--they don't leave corpses */
        // Handled in the calling function
        //if (canspotmon(mon) && has_unchanging(mon)) 
        //{
        //    pline("%s seems momentarily different.", Monnam(mon)); /* same as poly trap */
        //}
        //else 
        //{
        //    newcham(mon, (struct permonst*)0, 0, FALSE, TRUE);
        //}
        break;
    case PM_DISENCHANTER:
        /* picks an intrinsic at random and removes it; there's
           no feedback if hero already lacks the chosen ability */
        debugpline0("using m_attrcurse to strip an intrinsic");
        m_attrcurse(mon);
        break;
    default:
        check_intrinsics = TRUE;
        break;
    }

    /* Level gain has been moved here, 100% chance */
    // Handled in the calling function
    //if (conveys_level(ptr))
    //{
    //    grow_up(mon, (struct monst*)0);
    //    check_intrinsics = FALSE;
    //}

    /* Possibly convey an intrinsic */
    if (check_intrinsics)
    {
        boolean conveys_STR = conveys_strength(ptr);
        boolean conveys_DEX = conveys_dexterity(ptr);
        boolean conveys_CON = conveys_constitution(ptr);
        boolean conveys_INT = conveys_intelligence(ptr);
        boolean conveys_WIS = conveys_wisdom(ptr);
        boolean conveys_CHA = conveys_charisma(ptr);
        int i, count;

        /* Check the monster for all of the intrinsics.  If this
         * monster can give more than one, pick one to try to give
         * from among all it can give.
         *
         * Strength from giants is now treated like an intrinsic
         * rather than being given unconditionally.
         */
        count = 0; /* number of possible intrinsics */
        tmp = 0;   /* which one we will try to give */

        int mdifficulty = ptr->difficulty;
        int percent = 1;

        if (ptr->mlet == S_NYMPH)
            percent = 25;
        else if (ptr->mlet == S_GIANT)
            percent = 50;
        else if (ptr->mlet == S_OGRE)
            percent = 25;
        else if (ptr->mlet == S_SNAKE)
            percent = 10;
        else if (ptr->mlet == S_YETI)
            percent = 20;
        else if (ptr == &mons[PM_FLOATING_EYE])
            percent = 100;
        else if (is_tentacled_one(ptr))
            percent = 25;
        else
            percent = min(30, max(1, mdifficulty));

        if (conveys_STR && rn2(100) < percent)
        {
            count = 1;
            tmp = -1; /* use -1 as fake prop index for STR */
            debugpline1("\"Intrinsic\" strength, %d", tmp);
        }

        if (conveys_DEX && rn2(100) < percent) {
            if (count == 0 || !rn2(2))
            {
                count = 1;
                tmp = -2;
                debugpline1("\"Intrinsic\" dexterity, %d", tmp);
            }
        }

        if (conveys_CON && rn2(100) < percent)
        {
            if (count == 0 || !rn2(2))
            {
                count = 1;
                tmp = -3;
                debugpline1("\"Intrinsic\" constitution, %d", tmp);
            }
        }

        if (conveys_INT && rn2(100) < percent)
        {
            if (count == 0 || !rn2(2))
            {
                count = 1;
                tmp = -4;
                debugpline1("\"Intrinsic\" intelligence, %d", tmp);
            }
        }

        if (conveys_WIS && rn2(100) < percent)
        {
            if (count == 0 || !rn2(2))
            {
                count = 1;
                tmp = -5;
                debugpline1("\"Intrinsic\" wisdom, %d", tmp);
            }
        }

        if (conveys_CHA && rn2(100) < percent)
        {
            if (count == 0 || !rn2(2))
            {
                count = 1;
                tmp = -6;
                debugpline1("\"Intrinsic\" charisma, %d", tmp);
            }
        }


        for (i = 1; i <= LAST_PROP; i++) {
            if (!intrinsic_possible(i, ptr))
                continue;
            ++count;
            /* a 1 in count chance of replacing the old choice
               with this one, and a count-1 in count chance
               of keeping the old choice (note that 1 in 1 and
               0 in 1 are what we want for the first candidate) */
            if (!rn2(count)) {
                debugpline2("Intrinsic %d replacing %d", i, tmp);
                tmp = i;
            }
        }

        /* if something was chosen, give it now (givit() might fail) */
        if (tmp == -1)
            m_gainstr(mon, (struct obj*)0, 1, canspotmon(mon));
        else if (tmp == -2)
        {
            play_sfx_sound_at_location(SFX_GAIN_ABILITY, mon->mx, mon->my);
            (void)m_adjattrib(mon, A_DEX, 1, canspotmon(mon));
        }
        else if (tmp == -3)
        {
            play_sfx_sound_at_location(SFX_GAIN_ABILITY, mon->mx, mon->my);
            (void)m_adjattrib(mon, A_CON, 1, canspotmon(mon));
        }
        else if (tmp == -4)
        {
            play_sfx_sound_at_location(SFX_GAIN_ABILITY, mon->mx, mon->my);
            (void)m_adjattrib(mon, A_INT, 1, canspotmon(mon));
        }
        else if (tmp == -5)
        {
            play_sfx_sound_at_location(SFX_GAIN_ABILITY, mon->mx, mon->my);
            (void)m_adjattrib(mon, A_WIS, 1, canspotmon(mon));
        }
        else if (tmp == -6)
        {
            play_sfx_sound_at_location(SFX_GAIN_ABILITY, mon->mx, mon->my);
            (void)m_adjattrib(mon, A_CHA, 1, canspotmon(mon));
        }
        else if (tmp > 0)
            m_givit(mon, tmp, ptr);
    } /* check_intrinsics */

    if (catch_lycanthropy >= LOW_PM && !resists_lycanthropy(mon)) {
        //Currently pets cannot catch lycanthropy
    }
    return;
}


void
m_gainstr(mtmp, otmp, num, verbose)
struct monst* mtmp;
struct obj* otmp;
int num;
boolean verbose;
{
    if (!num)
    {
        if (ABASE(A_STR) < 18)
            num = (rn2(4) ? 1 : rnd(6));
        else if (ABASE(A_STR) < STR18(85))
            num = rnd(10);
        else
            num = 1;
    }
    (void)m_adjattrib(mtmp, A_STR, (otmp && otmp->cursed) ? -num : (otmp && otmp->blessed) ? num + rn2(2) : num, verbose);

}



/* hunger effects -- returns TRUE on starvation */
STATIC_OVL boolean
dog_hunger(mtmp, edog)
struct monst *mtmp;
struct edog *edog;
{
    if (!mtmp || !edog)
        return FALSE;

    /* Non-eaters do not get hungry */
    if (is_non_eater(mtmp->data))
    {
        edog->hungrytime = max(edog->hungrytime, monstermoves + PET_NONEATER_NUTRITION);
        return FALSE;
    }

    if (monstermoves >= edog->hungrytime + PET_WEAK_TIME) 
    {
        if (!edog->mhpmax_penalty && !is_not_living(mtmp->data))
        {
            /* starving pets are limited in healing */
            int newmhpmax = mtmp->mhpmax / 3;
            mtmp->mprops[CONFUSION] |= M_INTRINSIC_ACQUIRED;
            edog->mhpmax_penalty = mtmp->mhpmax - newmhpmax;
            update_mon_maxhp(mtmp);
            if (mtmp->mhp > mtmp->mhpmax)
                mtmp->mhp = mtmp->mhpmax;
            if (mtmp->mhp == 1)
                mtmp->mhp = 0; /* since update_mon_maxhp takes it only to 1 */
            if (DEADMONSTER(mtmp))
                goto dog_died;
            if (cansee(mtmp->mx, mtmp->my))
                pline("%s is confused from hunger.", Monnam(mtmp));
            else if (couldsee(mtmp->mx, mtmp->my))
                beg(mtmp);
            else
                You_feel("worried about %s.", y_monnam(mtmp));
            stop_occupation();
        }
        else if ((monstermoves >= edog->hungrytime + PET_STARVING_TIME && !is_not_living(mtmp->data))
                   || DEADMONSTER(mtmp)) 
        {
 dog_died:
            if (mtmp->mleashed && mtmp != u.usteed)
            {
                play_sfx_sound(SFX_LEASH_GOES_SLACK);
                Your("leash goes slack.");
            }
            else if (cansee(mtmp->mx, mtmp->my))
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "%s starves.", Monnam(mtmp));
            else
                You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "%s for a moment.",
                         Hallucination ? "bummed" : "sad");
            mondied(mtmp);
            return  TRUE;
        }
        else if (context.hungry_message_displayed == FALSE && (monstermoves - edog->hungrytime + PET_WEAK_TIME) % 50 == 0)
        {
            context.hungry_message_displayed = TRUE;
            if (cansee(mtmp->mx, mtmp->my))
                pline("%s is %s from hunger.", Monnam(mtmp), 
                    monstermoves >= edog->hungrytime + PET_STARVING_TIME - 50 ? "getting more crazed" : monstermoves >= edog->hungrytime + PET_STARVING_TIME - 100 ? "crazed" : monstermoves >= edog->hungrytime + PET_STARVING_TIME + 50 ? "getting more confused" : "confused");
            else if (couldsee(mtmp->mx, mtmp->my))
                beg(mtmp);
            else
                You_feel("worried about %s.", y_monnam(mtmp));
            stop_occupation();
        }
    }
    else if (monstermoves >= edog->hungrytime && !is_non_eater(mtmp->data))
    {
        if (context.hungry_message_displayed == FALSE && (monstermoves - edog->hungrytime) % 100 == 0)
        {
            context.hungry_message_displayed = TRUE;
            if (cansee(mtmp->mx, mtmp->my))
                pline("%s seems %shungry.", Monnam(mtmp), monstermoves >= edog->hungrytime + PET_EXTREMELY_HUNGRY_TIME ? "extremely " : monstermoves >= edog->hungrytime + PET_VERY_HUNGRY_TIME ? "very " : "");
            else if (couldsee(mtmp->mx, mtmp->my))
                beg(mtmp);
            stop_occupation();
        }
    }

    return FALSE;
}

/* do something with object (drop, pick up, eat) at current position
 * returns 1 if object eaten (since that counts as dog's move), 2 if died
 */
STATIC_OVL int
dog_invent(mtmp, edog, udist)
register struct monst *mtmp;
register struct edog *edog;
int udist;
{
    register int omx, omy, carryamt = 0;
    struct obj *obj, *otmp;

    if (!mon_can_move(mtmp) || !mtmp->mwantstomove)
        return 0;

    omx = mtmp->mx;
    omy = mtmp->my;

    /* If we are carrying something then we drop it (perhaps near @).
     * Note: if apport == 1 then our behaviour is independent of udist.
     * Use udist+1 so steed won't cause divide by zero.
     */
    if (droppables(mtmp)) 
    {
        if ((!rn2(udist + 1) || !rn2(edog->apport)) && mtmp->mwantstodrop && !mtmp->ispartymember && !mtmp->isminion && !is_packmule(mtmp->data))
            if (rn2(10) < edog->apport) 
            {
                mdrop_droppable_objs(mtmp);
                if (edog->apport > 1)
                    edog->apport--;
                edog->dropdist = udist; /* hpscdi!jon */
                edog->droptime = monstermoves;
            }
    } 
    else 
    {
        if ((obj = level.objects[omx][omy]) != 0
            && !index(nofetch, obj->oclass)
#ifdef MAIL
            && obj->otyp != SCR_MAIL
#endif
            )
        {
            int edible = dogfood(mtmp, obj);

            if ((edible <= CADAVER
                 /* starving pet is more aggressive about eating */
                 || (edog->mhpmax_penalty && edible == ACCFOOD))
                && could_reach_item(mtmp, obj->ox, obj->oy) && !shk_chastise_pet(mtmp, obj, TRUE) && !onnopickup(obj->ox, obj->oy, mtmp) && !is_obj_no_pickup(obj) && dog_wants_to_eat(mtmp)
                )
                return dog_eat(mtmp, obj, omx, omy, FALSE);

            carryamt = can_carry(mtmp, obj);
            if (carryamt > 0 && !obj->cursed && !is_obj_unique(obj) && !is_quest_artifact(obj) 
                && !mtmp->issummoned && !mtmp->ispartymember && !mtmp->isminion && !is_packmule(mtmp->data)
                && could_reach_item(mtmp, obj->ox, obj->oy) && !onnopickup(obj->ox, obj->oy, mtmp) && !is_obj_no_pickup(obj))
            {
                if (rn2(20) < edog->apport + 3)
                {
                    if (rn2(udist) || !rn2(edog->apport)) 
                    {
                        int shkpreaction = FALSE;
                        shkpreaction = shk_chastise_pet(mtmp, obj, FALSE);

                        if(!shkpreaction)
                        {
                            otmp = obj;
                            if (carryamt != obj->quan)
                                otmp = splitobj(obj, carryamt);

                            if (flags.verbose)
                            {
                                play_simple_object_sound(otmp, OBJECT_SOUND_TYPE_PICK_UP);

                                if(cansee(omx, omy))
                                    pline("%s picks up %s.", Monnam(mtmp), distant_name(otmp, doname));
                            }

                            Strcpy(debug_buf_2, "dog_invent");
                            obj_extract_self(otmp);
                            newsym(omx, omy);
                            (void) mpickobj(mtmp, otmp);
                            
                            if (attacktype(mtmp->data, AT_WEAP)
                                && mtmp->weapon_strategy == NEED_WEAPON)
                            {
                                mtmp->weapon_strategy = NEED_HTH_WEAPON;
                                (void) mon_wield_item(mtmp, FALSE, 0, 0);
                            }
                            m_dowear(mtmp, FALSE, FALSE);
                        }
                    }
                }
            }
        }
    }
    return 0;
}


boolean
dog_wants_to_eat(mtmp)
struct monst* mtmp;
{
    if(!mtmp || !has_edog(mtmp))
        return FALSE;

    if (is_non_eater(mtmp->data))
        return FALSE;

    if (EDOG(mtmp)->hungrytime > monstermoves && mtmp->mcomingtou)
        return FALSE;

    return (EDOG(mtmp)->hungrytime < monstermoves + PET_SATIATED_NUTRITION); /* twice the satiated amount for the player for convenience */
}


/* set dog's goal -- gtyp, gx, gy;
   returns -1/0/1 (dog's desire to approach player) or -2 (abort move) */
STATIC_OVL int
dog_goal(mtmp, edog, after, udist, whappr)
register struct monst *mtmp;
struct edog *edog;
int after, udist, whappr;
{
    register int omx, omy;
    boolean in_masters_sight, using_yell_position = FALSE, dog_has_minvent;
    register struct obj *obj;
    xchar otyp;
    int appr;

    /* Steeds don't move on their own will */
    if (mtmp == u.usteed)
        return -2;

    omx = mtmp->mx;
    omy = mtmp->my;

    in_masters_sight = couldsee(omx, omy);
  
    dog_has_minvent = (droppables(mtmp) != 0);

    if (!edog || mtmp->mleashed || (mtmp->mcomingtou && in_masters_sight)) { /* he's not going anywhere... */
        gtyp = APPORT;
        gx = u.ux;
        gy = u.uy;
        if (mtmp->mcomingtou && in_masters_sight)
        {
           mtmp->yell_x = 0;
           mtmp->yell_y = 0;
        }
    }
    else if (mtmp->mcomingtou)
    {        
        gtyp = APPORT;
        if (mtmp->yell_x == 0 || mtmp->yell_y == 0)
        {
            gx = u.ux;
            gy = u.uy;
        }
        else
        {
            gx = mtmp->yell_x;
            gy = mtmp->yell_y;
            using_yell_position = TRUE;
        }
    }
    else
    {
#define DDIST(x, y) (dist2(x, y, omx, omy))
#define SQSRCHRADIUS 5
        int min_x, max_x, min_y, max_y;
        register int nx, ny;

        gtyp = UNDEF; /* no goal as yet */
        gx = gy = 0;  /* suppress 'used before set' message */

        if ((min_x = omx - SQSRCHRADIUS) < 1)
            min_x = 1;
        if ((max_x = omx + SQSRCHRADIUS) >= COLNO)
            max_x = COLNO - 1;
        if ((min_y = omy - SQSRCHRADIUS) < 0)
            min_y = 0;
        if ((max_y = omy + SQSRCHRADIUS) >= ROWNO)
            max_y = ROWNO - 1;

        /* nearby food is the first choice, then other objects */
        for (obj = fobj; obj; obj = obj->nobj) 
        {
            nx = obj->ox;
            ny = obj->oy;
            if (nx >= min_x && nx <= max_x && ny >= min_y && ny <= max_y)
            {
                otyp = dogfood(mtmp, obj);

                /* skip inferior goals */
                if (otyp > gtyp || otyp == UNDEF)
                    continue;

                /* avoid cursed items unless starving */
                if (cursed_object_at(nx, ny)
                    && !(edog->mhpmax_penalty && otyp < MANFOOD))
                    continue;

                /* skip completely unreachable goals */
                if (!could_reach_item(mtmp, nx, ny)
                    || !can_reach_location(mtmp, mtmp->mx, mtmp->my, nx, ny)
                    || onnopickup(nx, ny, mtmp) || is_obj_no_pickup(obj))
                    continue;

                if (!dog_wants_to_eat(mtmp) && otyp < MANFOOD)
                    continue;

                /* skip always shop food and other items if chastised */
                if (edog->chastised && (obj->unpaid || (obj->where == OBJ_FLOOR && !obj->no_charge && costly_spot(obj->ox, obj->oy))))
                    continue;

                if (otyp < MANFOOD)
                {
                    if (otyp < gtyp || DDIST(nx, ny) < DDIST(gx, gy))
                    {
                        gx = nx;
                        gy = ny;
                        gtyp = otyp;
                    }
                }
                else if (gtyp == UNDEF && in_masters_sight
                           && !dog_has_minvent
                           && (!levl[omx][omy].lit || levl[u.ux][u.uy].lit)
                           && (otyp == MANFOOD || m_cansee(mtmp, nx, ny))
                           && edog->apport > rn2(8)
                           && can_carry(mtmp, obj) > 0)
                {
                    gx = nx;
                    gy = ny;
                    gtyp = APPORT;
                }
            }
        }
    }

    /* follow player if appropriate */
    if (gtyp == UNDEF || (gtyp != DOGFOOD && gtyp != APPORT
                          && monstermoves < edog->hungrytime)) {
        gx = u.ux;
        gy = u.uy;
        if (after && udist <= 4 && gx == u.ux && gy == u.uy)
            return -2;
        appr = (udist >= 9) ? 1 : is_fleeing(mtmp) ? -1 : 0;
        if (udist > 1) 
        {
            if (!IS_ROOM(levl[u.ux][u.uy].typ) || !rn2(4) || whappr
                || (dog_has_minvent && rn2(edog->apport)))
            {
                appr = 1;

                /* Activate pathing back to player randomly, for guardian angels, or if the distance is large */
                if (!couldsee(mtmp->mx, mtmp->my) && !mtmp->mcomingtou && (!rn2(5) || !has_edog(mtmp) || distu(mtmp->mx, mtmp->my) >= 256))
                {
                    mtmp->mcomingtou = 20 + rn2(20);
                    mtmp->yell_x = 0;
                    mtmp->yell_y = 0;
                }
            }
        }
        /* if you have dog food it'll follow you more closely */
        if (appr == 0)
            for (obj = invent; obj; obj = obj->nobj)
                if (dogfood(mtmp, obj) == DOGFOOD) {
                    appr = 1;
                    break;
                }
    } else
        appr = 1; /* gtyp != UNDEF */
    if (is_confused(mtmp))
        appr = 0;

#define FARAWAY (COLNO + 2) /* position outside screen */

    if (gx == u.ux && gy == u.uy && !in_masters_sight && !using_yell_position) {
        register coord *cp;

        cp = gettrack(omx, omy);
        if (cp) {
            gx = cp->x;
            gy = cp->y;
            if (edog)
                edog->ogoal.x = 0;
        } else {
            /* assume master hasn't moved far, and reuse previous goal */
            if (edog && edog->ogoal.x
                && (edog->ogoal.x != omx || edog->ogoal.y != omy)) {
                gx = edog->ogoal.x;
                gy = edog->ogoal.y;
                edog->ogoal.x = 0;
            } else {
                int fardist = FARAWAY * FARAWAY;
                gx = gy = FARAWAY; /* random */
                do_clear_area(omx, omy, 9, wantdoor, (genericptr_t) &fardist);

                /* here gx == FARAWAY e.g. when dog is in a vault */
                if (gx == FARAWAY || (gx == omx && gy == omy)) {
                    gx = u.ux;
                    gy = u.uy;
                } else if (edog) {
                    edog->ogoal.x = gx;
                    edog->ogoal.y = gy;
                }
            }
        }
    } else if (edog) {
        edog->ogoal.x = 0;
    }
    return appr;
}

STATIC_OVL struct monst *
find_targ(mtmp, dx, dy, maxdist)
register struct monst *mtmp;
int dx, dy;
int maxdist;
{
    struct monst *targ = 0;
    int curx = mtmp->mx, cury = mtmp->my;
    int dist = 0;

    /* Walk outwards */
    for ( ; dist < maxdist; ++dist) {
        curx += dx;
        cury += dy;
        if (!isok(curx, cury))
            break;

        /* FIXME: Check if we hit a wall/door/boulder to
         *        short-circuit unnecessary subsequent checks
         */

        /* If we can't see up to here, forget it - will this
         * mean pets in corridors don't breathe at monsters
         * in rooms? If so, is that necessarily bad?
         */
        if (!m_cansee(mtmp, curx, cury))
            break;

        if (curx == mtmp->mux && cury == mtmp->muy)
            return &youmonst;

        if ((targ = m_at(curx, cury)) != 0) {
            /* Is the monster visible to the pet? */
            if (m_cannotsense_m(mtmp, targ))
            {
                /* If the pet can't see it, it assumes it aint there */
                targ = 0;
            }
            else
                break;
        }
    }
    return targ;
}

STATIC_OVL int
find_friends(mtmp, mtarg, maxdist)
struct monst *mtmp, *mtarg;
int    maxdist;
{
    struct monst *pal;
    int dx = sgn(mtarg->mx - mtmp->mx),
        dy = sgn(mtarg->my - mtmp->my);
    int curx = mtarg->mx, cury = mtarg->my;
    int dist = distmin(mtarg->mx, mtarg->my, mtmp->mx, mtmp->my);

    for ( ; dist <= maxdist; ++dist) {
        curx += dx;
        cury += dy;

        if (!isok(curx, cury))
            return 0;

        /* If the pet can't see beyond this point, don't
         * check any farther
         */
        if (!m_cansee(mtmp, curx, cury))
            return 0;

        /* Does pet think you're here? */
        if (mtmp->mux == curx && mtmp->muy == cury)
            return 1;

        pal = m_at(curx, cury);

        if (pal) {
            if (pal->mtame) {
                /* Pet won't notice invisible pets */
                if (!m_cannotsense_m(mtmp, pal)) //!is_invisible(pal) || has_see_invisible(mtmp)
                    return 1;
            } else {
                /* Quest leaders and guardians are always seen */
                if (pal->data->msound == MS_LEADER
                    || pal->data->msound == MS_GUARDIAN)
                    return 1;
            }
        }
    }
    return 0;
}

STATIC_OVL long
score_targ(mtmp, mtarg)
struct monst *mtmp, *mtarg;
{
    long score = 0L;

    /* If the monster is confused, normal scoring is disrupted -
     * anything may happen
     */

    /* Give 1 in 3 chance of safe breathing even if pet is confused or
     * if you're on the quest start level */
    if (!is_confused(mtmp) || !rn2(3) || Is_qstart(&u.uz)) {
        int mtmp_lev;
        aligntyp align1 = A_NONE, align2 = A_NONE; /* For priests, minions */
        boolean faith1 = TRUE,  faith2 = TRUE;

        if (mtmp->isminion)
            align1 = EMIN(mtmp)->min_align;
        else if (mtmp->ispriest)
            align1 = EPRI(mtmp)->shralign;
        else
            faith1 = FALSE;
        if (mtarg->isminion)
            align2 = EMIN(mtarg)->min_align; /* MAR */
        else if (mtarg->ispriest)
            align2 = EPRI(mtarg)->shralign; /* MAR */
        else
            faith2 = FALSE;

        /* Never target quest friendlies */
        if (mtarg->data->msound == MS_LEADER
            || mtarg->data->msound == MS_GUARDIAN)
            return -5000L;
        /* D: Fixed angelic beings using gaze attacks on coaligned priests */
        if (faith1 && faith2 && align1 == align2 && is_peaceful(mtarg)) {
            score -= 5000L;
            return score;
        }
        /* Is monster adjacent? */
        if (distmin(mtmp->mx, mtmp->my, mtarg->mx, mtarg->my) <= 1) {
            score -= 3000L;
            return score;
        }
        /* Is the monster peaceful or tame? */
        if ((is_peaceful(mtarg) && !mon_has_bloodlust(mtmp) /*mtmp->ispacifist*/) || is_tame(mtarg) || mtarg == &youmonst) {
            /* Pets will never be targeted */
            score -= 3000L;
            return score;
        }
        /* Is master/pet behind monster? Check up to 15 squares beyond pet. */
        if (find_friends(mtmp, mtarg, 15)) {
            score -= 3000L;
            return score;
        }
        /* Target hostile monsters in preference to peaceful ones */
        if (!is_peaceful(mtarg))
            score += 10;
        /* Is the monster passive? Don't waste energy on it, if so */
        if (!mon_has_bloodlust(mtmp) && mtarg->data->mattk[0].aatyp == AT_PASV)
            score -= 1000;
        /* Even weak pets with breath attacks shouldn't take on very
           low-level monsters. Wasting breath on lichens is ridiculous. */
        if (!mon_has_bloodlust(mtmp) && ((mtarg->m_lev < 2 && mtmp->m_lev > 5)
            || (mtmp->m_lev > 12 && mtarg->m_lev < mtmp->m_lev - 9
                && u.ulevel > 8 && mtarg->m_lev < u.ulevel - 7)))
            score -= 25;
        /* for strength purposes, a vampshifter in weak form (vampire bat,
           fog cloud, maybe wolf) will attack as if in vampire form;
           otherwise if won't do much and usually wouldn't suffer enough
           damage (from counterattacks) to switch back to vampire form;
           make it be more aggressive by behaving as if stronger */
        mtmp_lev = mtmp->m_lev;
        if (mtmp->cham >= LOW_PM && is_vampshifter(mtmp) && mtmp->data->mlet != S_VAMPIRE) 
        {
            /* is_vampshifter() implies (mtmp->cham >= LOW_PM) */
            mtmp_lev = mons[mtmp->cham].mlevel;
            /* actual vampire level would range from 1.0*mlvl to 1.5*mlvl */
            mtmp_lev += rn2(mtmp_lev / 2 + 1);
            /* we don't expect actual level in weak form to exceed
               base level of strong form, but handle that if it happens */
            if (mtmp->m_lev > mtmp_lev)
                mtmp_lev = mtmp->m_lev;
        }
        /* And pets will hesitate to attack vastly stronger foes.
           This penalty will be discarded if master's in trouble. */
        if (!mon_disregards_enemy_strength(mtmp) && mtarg->m_lev > mtmp_lev + 4L)
            score -= (mtarg->m_lev - mtmp_lev) * 20L;
        /* All things being the same, go for the beefiest monster. This
           bonus should not be large enough to override the pet's aversion
           to attacking much stronger monsters. */
        score += mtarg->m_lev * 2 + mtarg->mhp / 3;
    }
    /* Fuzz factor to make things less predictable when very
       similar targets are abundant. */
    score += rnd(5);
    /* Pet may decide not to use ranged attack when confused */
    if (is_confused(mtmp) && !rn2(3))
        score -= 1000;
    return score;
}

STATIC_OVL struct monst *
best_target(mtmp)
struct monst *mtmp;   /* Pet */
{
    int dx, dy;
    long bestscore = -40000L, currscore;
    struct monst *best_targ = 0, *temp_targ = 0;

    /* Help! */
    if (!mtmp)
        return 0;

    /* If the pet is blind, it's not going to see any target */
    if (is_blinded(mtmp) && !m_has_active_telepathy(mtmp))
        return 0;

    /* Search for any monsters lined up with the pet, within an arbitrary
     * distance from the pet (7 squares, even along diagonals). Monsters
     * are assigned scores and the best score is chosen.
     */
    for (dy = -1; dy < 2; ++dy) {
        for (dx = -1; dx < 2; ++dx) {
            if (!dx && !dy)
                continue;
            /* Traverse the line to find the first monster within 7
             * squares. Invisible monsters are skipped (if the
             * pet doesn't have see invisible).
             */
            temp_targ = find_targ(mtmp, dx, dy, 7);

            /* Nothing in this line? */
            if (!temp_targ)
                continue;

            /* Decide how attractive the target is */
            currscore = score_targ(mtmp, temp_targ);

            if (currscore > bestscore) {
                bestscore = currscore;
                best_targ = temp_targ;
            }
        }
    }

    /* Filter out targets the pet doesn't like */
    if (bestscore < 0L)
        best_targ = 0;

    return best_targ;
}

/* return 0 (no move), 1 (move) or 2 (dead) */
int
dog_move(mtmp, after)
register struct monst *mtmp;
int after; /* this is extra fast monster movement */
{
    int omx, omy; /* original mtmp position */
    int appr, whappr, udist;
    int i, j, k;
    register struct edog *edog = has_edog(mtmp) ? EDOG(mtmp) : 0;
    struct obj *obj = (struct obj *) 0;
    xchar foodtyp;
    boolean cursemsg[9], do_eat = FALSE;
    boolean better_with_displacing = FALSE;
    xchar nix, niy;      /* position mtmp is (considering) moving to */
    register int nx, ny; /* temporary coordinates */
    xchar cnt, uncursedcnt, chcnt;
    int chi = -1, nidist, ndist;
    coord poss[9];
    long info[9], allowflags;
    struct obj* cursedobj[9] = { 0 };
#define GDIST(x, y) (dist2(x, y, gx, gy))

    /*
     * Tame Angels have isminion set and an ispriest structure instead of
     * an edog structure.  Fortunately, guardian Angels need not worry
     * about mundane things like eating and fetching objects, and can
     * spend all their energy defending the player.  (They are the only
     * monsters with other structures that can be tame.)
     */

    /* It seems that tame angels can have both structures --JG */

    omx = mtmp->mx;
    omy = mtmp->my;
    if (dog_hunger(mtmp, edog))
        return 2; /* starved */

    udist = distu(omx, omy);
    /* Let steeds eat and maybe throw rider during Conflict */
    if (mtmp == u.usteed) 
    {
        if (is_crazed(mtmp) || (Conflict && !check_ability_resistance_success(mtmp, A_WIS, 0))) 
        {
            dismount_steed(DISMOUNT_THROWN);
            return 1;
        }
        udist = 1;
    } 
    else if (!udist)
        /* maybe we tamed him while being swallowed --jgm */
        return 0;

    nix = omx; /* set before newdogpos */
    niy = omy;
    cursemsg[0] = FALSE; /* lint suppression */
    cursedobj[0] = 0;
    info[0] = 0;         /* ditto */

    if (has_edog(mtmp))
    {
        if (edog->chastised > 0)
        {
            edog->chastised--;
        }
        j = dog_invent(mtmp, edog, udist);
        if (j == 2)
            return 2; /* died */
        else if (j == 1)
            goto newdogpos; /* eating something */

        whappr = (monstermoves - edog->whistletime < 5);
    }
    else
        whappr = 0;

    appr = dog_goal(mtmp, has_edog(mtmp) ? edog : (struct edog*)0, after, udist, whappr);
    if (appr == -2)
        return 0;

    allowflags = ALLOW_M | ALLOW_TRAPS | ALLOW_SSM | ALLOW_SANCT;
    if (passes_walls(mtmp->data))
        allowflags |= (ALLOW_ROCK | ALLOW_WALL);
    if (passes_bars(mtmp->data))
        allowflags |= ALLOW_BARS;
    if (throws_rocks(mtmp->data))
        allowflags |= ALLOW_ROCK;
    if (has_pitwalk(mtmp->data))
        allowflags |= ALLOW_PITS;
    if (is_displacer(mtmp->data))
        allowflags |= ALLOW_MDISP;
    if (is_crazed(mtmp) || (Conflict && !check_ability_resistance_success(mtmp, A_WIS, 0)))
    {
        allowflags |= (ALLOW_U  | ALLOW_TM);
        if (mtmp->isminion)
        {
            /* Guardian angel refuses to be conflicted; rather,
             * it disappears, angrily, and sends in some nasties
             */
            lose_guardian_angel(mtmp);
            return 2; /* current monster is gone */
        }
    }
#if 0 /* [this is now handled in dochug()] */
    if (!Conflict && !is_crazed(mtmp) && !is_confused(mtmp)
        && mtmp == u.ustuck && !sticks(youmonst.data)) {
        unstuck(mtmp); /* swallowed case handled above */
        You("get released!");
    }
#endif
    if (can_operate_objects(mtmp->data)) 
    {
        allowflags |= OPENDOOR;
        if (monhaskey(mtmp, TRUE))
            allowflags |= UNLOCKDOOR;
        /* note:  the Wizard and Riders can unlock doors without a key;
           they won't use that ability if someone manages to tame them */
    }
    if (is_giant(mtmp->data))
        allowflags |= BUSTDOOR;
    if (tunnels(mtmp->data)
        && !Is_really_rogue_level(&u.uz)) /* same restriction as m_move() */
        allowflags |= ALLOW_DIG;

    cnt = mfndpos(mtmp, poss, info, allowflags);

    boolean pathres = FALSE;
    xchar tx = 0, ty = 0;
    if(!mtmp->notraveltimer)
    {
        /* Find travel path if comingtou or if is a guardian angel that cannot see you */
        if ((mtmp->mcomingtou || (mtmp->isminion && !couldsee(mtmp->mx, mtmp->my))) && !mtmp->notraveltimer)
        {
            xchar mon_dx = 0, mon_dy = 0;

            pathres = m_findtravelpath(mtmp, TRAVP_TRAVEL, &mon_dx, &mon_dy, allowflags);

            if (!pathres)
            {
                mtmp->notraveltimer = 12; /* Check again after 12 rounds */
            }
            
            if (pathres && (mon_dx || mon_dy))
            {
                tx = mtmp->mx + mon_dx;
                ty = mtmp->my + mon_dy;
                if (!isok(tx, ty))
                    pathres = FALSE;
            }
        }
    }

    /* Normally dogs don't step on cursed items, but if they have no
     * other choice they will.  This requires checking ahead of time
     * to see how many uncursed item squares are around.
     */
    uncursedcnt = 0;
    for (i = 0; i < cnt; i++) 
    {
        nx = poss[i].x;
        ny = poss[i].y;
        if (MON_AT(nx, ny) && !((info[i] & ALLOW_M) || ((info[i] & ALLOW_TM) && m_at(nx, ny) && is_tame(m_at(nx, ny))) || info[i] & ALLOW_MDISP))
            continue;
        if (!mtmp->mcomingtou && cursed_object_at(nx, ny))
            continue;
        uncursedcnt++;
    }

    better_with_displacing = should_displace(mtmp, poss, info, cnt, gx, gy);

    chcnt = 0;
    chi = -1;
    nidist = GDIST(nix, niy);

    for (i = 0; i < cnt; i++) 
    {
        nx = poss[i].x;
        ny = poss[i].y;
        cursemsg[i] = FALSE;
        cursedobj[i] = 0;

        /* if leashed, we drag him along. */
        if (mtmp->mleashed && distu(nx, ny) > 4)
            continue;

        /* if a guardian, try to stay close by choice -- Now handled above by activating pathing --JG */
//        if (mtmp->isminion && !mtmp->mcomingtou && (j = distu(nx, ny)) > 16 && j >= udist)
//            continue;

        boolean monatres = MON_AT(nx, ny);
        register struct monst* mtmp2 = m_at(nx, ny);
        boolean allowres =((info[i] & ALLOW_M) && monatres) || ((info[i] & ALLOW_TM) && monatres && mtmp2 && is_tame(mtmp2));

        if (allowres)
        {
            int mstatus;

            if (
                (!mon_disregards_own_health(mtmp) && mtmp2->data->difficulty >= mtmp->data->difficulty * DISREGARD_HEALTH_LEVEL_MULTIPLIER + DISREGARD_HEALTH_LEVEL_DIFFERENCE_THRESHOLD)
                    || (
                        (mtmp2->data == &mons[PM_FLOATING_EYE] && rn2(10)
                        && !is_blinded(mtmp) && haseyes(mtmp->data) && !is_blinded(mtmp2)
                        && (has_see_invisible(mtmp) || !is_invisible(mtmp2)))
                    || (slurps_items(mtmp2->data) && rn2(10))
                    || (is_tame(mtmp2) && !Conflict && !is_crazed(mtmp2))
                    || (max_passive_dmg(mtmp2, mtmp) >= mtmp->mhp)
                    || (is_peaceful(mtmp2) && (mtmp2->data->msound == MS_GUARDIAN
                         || mtmp2->data->msound == MS_LEADER || (!mon_has_bloodlust(mtmp)
                        && !Conflict && !is_crazed(mtmp) && !mon_has_bloodlust(mtmp))))
                    || (touch_petrifies(mtmp2->data) && !resists_ston(mtmp) && !MON_WEP(mtmp))
                    )
                )
                continue;

            if (after)
                return 0; /* hit only once each move */

            bhitpos.x = mtmp2->mx;
            bhitpos.y = mtmp2->my;
            notonhead = 0;
            mstatus = mattackm(mtmp, mtmp2);

            /* aggressor (pet) died */
            if (mstatus & MM_AGR_DIED)
                return 2;

            if ((mstatus & MM_HIT) && !(mstatus & MM_DEF_DIED) && rn2(4)
                && mtmp2->mlstmv != monstermoves
                && !onscary(mtmp->mx, mtmp->my, mtmp2)
                /* monnear check needed: long worms hit on tail */
                && monnear(mtmp2, mtmp->mx, mtmp->my)) 
            {
                bhitpos.x = mtmp->mx;
                bhitpos.y = mtmp->my;
                notonhead = 0;
                mstatus = mattackm(mtmp2, mtmp); /* return attack */
                if (mstatus & MM_DEF_DIED)
                    return 2;
            }
            return 0;
        }
        if ((info[i] & ALLOW_MDISP) && MON_AT(nx, ny)
            && better_with_displacing && !undesirable_disp(mtmp, nx, ny)) {
            int mstatus;
            register struct monst *mtmp3 = m_at(nx, ny);

            mstatus = mdisplacem(mtmp, mtmp3, FALSE); /* displace monster */
            if (mstatus & MM_DEF_DIED)
                return 2;
            return 0;
        }

        {
            /* Dog avoids harmful traps, but perhaps it has to pass one
             * in order to follow player.  (Non-harmful traps do not
             * have ALLOW_TRAPS in info[].)  The dog only avoids the
             * trap if you've seen it, unlike enemies who avoid traps
             * if they've seen some trap of that type sometime in the
             * past.  (Neither behavior is really realistic.)
             */
            struct trap *trap;

            if ((info[i] & ALLOW_TRAPS) && (trap = t_at(nx, ny))) 
            {
                if (mtmp->mleashed) 
                {
                    if (!Deaf)
                        whimper(mtmp);
                }
                else
                {
                    /* 1/40 chance of stepping on it anyway, in case
                     * it has to pass one to follow the player...
                     */
                    if (trap->tseen && rn2(40))
                        continue;
                }
            }
        }

        /* dog eschews cursed objects, but likes dog food */
        /* (minion isn't interested; `cursemsg' stays FALSE) */
        if (!mtmp->isminion)
            for (obj = level.objects[nx][ny]; obj; obj = obj->nexthere) 
            {
                if (obj->blessed && mon_eschews_blessed(mtmp)) /* animals and angels eschew cursed objects */
                {
                    cursemsg[i] = TRUE;
                    cursedobj[i] = obj;
                }
                else if (obj_counts_as_silver(obj) && mon_eschews_silver(mtmp))
                {
                    cursemsg[i] = TRUE;
                    cursedobj[i] = obj;
                }
                else if (obj->cursed && mon_eschews_cursed(mtmp))
                {
                    cursemsg[i] = TRUE;
                    cursedobj[i] = obj;
                }
                else if (obj_sheds_light(obj) && mon_eschews_light(mtmp))
                {
                    cursemsg[i] = TRUE;
                    cursedobj[i] = obj;
                }
                else if ((foodtyp = dogfood(mtmp, obj)) < MANFOOD && !onnopickup(nx, ny, mtmp) && !is_obj_no_pickup(obj) && dog_wants_to_eat(mtmp) && has_edog(mtmp) &&
                    (!EDOG(mtmp)->chastised || (EDOG(mtmp)->chastised && !(obj->unpaid || (obj->where == OBJ_FLOOR && !obj->no_charge && costly_spot(obj->ox, obj->oy)))))
                         && (foodtyp < ACCFOOD || edog->hungrytime <= monstermoves))
                {
                    /* Note: our dog likes the food so much that he
                     * might eat it even when it conceals a cursed object */
                    nix = nx;
                    niy = ny;
                    chi = i;
                    do_eat = TRUE;
                    cursemsg[i] = FALSE; /* not reluctant */
                    cursedobj[i] = 0;
                    goto newdogpos;
                }
            }
        /* didn't find something to eat; if we saw a cursed item and
           aren't being forced to walk on it, usually keep looking */
        if (cursemsg[i] && !mtmp->mleashed && !mtmp->mcomingtou && uncursedcnt > 0
            && rn2(13 * uncursedcnt))
            continue;

        /* lessen the chance of backtracking to previous position(s) */
        /* This causes unintended issues for pets trying to follow
           the hero. Thus, only run it if not leashed and >5 tiles
           away. */
        if (!mtmp->mleashed && !mtmp->mcomingtou && distmin(mtmp->mx, mtmp->my, u.ux, u.uy) > 5)
        {
            k = !mtmp->isminion ? uncursedcnt : cnt;
            for (j = 0; j < MTSZ && j < k - 1; j++)
                if (nx == mtmp->mtrack[j].x && ny == mtmp->mtrack[j].y)
                    if (rn2(MTSZ * (k - j)))
                        goto nxti;
        }


        if (pathres)
        {
            if (poss[i].x == tx && poss[i].y == ty)
            {
                nix = nx;
                niy = ny;
                chi = i;
                break;
            }
        }

        j = ((ndist = GDIST(nx, ny)) - nidist) * appr;
        if ((j == 0 && !rn2(++chcnt)) || j < 0
            || (j > 0 && !whappr
                && ((omx == nix && omy == niy && !rn2(3)) || !rn2(12)))) 
        {
            nix = nx;
            niy = ny;
            nidist = ndist;
            if (j < 0)
                chcnt = 0;
            chi = i;
        }
 nxti:
        ;
    }

    /* Pet hasn't attacked anything but is considering moving -
     * now's the time for ranged attacks. Note that the pet can move
     * after it performs its ranged attack. Should this be changed?
     */
    {
        struct monst *mtarg;
        int hungry = 0;

        /* How hungry is the pet? */
        if (!mtmp->isminion)
        {
            struct edog *dog = EDOG(mtmp);

            hungry = (monstermoves > (dog->hungrytime + 300));
        }

        /* Identify the best target in a straight line from the pet;
         * if there is such a target, we'll let the pet attempt an
         * attack.
         */
        mtarg = best_target(mtmp);

        /* Hungry pets are unlikely to use breath/spit attacks */
        if (mtarg && (!hungry || !rn2(5)))
        {
            int mstatus;

            if (mtarg == &youmonst)
            {
                if (mattacku(mtmp))
                    return 2;
            }
            else
            {
                bhitpos.x = mtarg->mx;
                bhitpos.y = mtarg->my;
                notonhead = 0;
                mstatus = mattackm(mtmp, mtarg);

                /* Shouldn't happen, really */
                if (mstatus & MM_AGR_DIED)
                    return 2;

                /* Allow the targeted nasty to strike back - if
                 * the targeted beast doesn't have a ranged attack,
                 * nothing will happen.
                 */
                if ((mstatus & MM_HIT) && !(mstatus & MM_DEF_DIED)
                    && rn2(4) && mtarg != &youmonst)
                {

                    /* Can monster see? If it can, it can retaliate
                     * even if the pet is invisible, since it'll see
                     * the direction from which the ranged attack came;
                     * if it's blind or unseeing, it can't retaliate
                     */
                    if (!is_blinded(mtarg) && haseyes(mtarg->data))
                    {
                        bhitpos.x = mtmp->mx;
                        bhitpos.y = mtmp->my;
                        notonhead = 0;
                        mstatus = mattackm(mtarg, mtmp);
                        if (mstatus & MM_DEF_DIED)
                            return 2;
                    }
                }
            }
        }
    }

newdogpos:
    if(mtmp->mwantstomove)
    {
        if (nix != omx || niy != omy) 
        {
            boolean wasseen;

            if (info[chi] & ALLOW_U)
            {
                if (mtmp->mleashed)
                { /* play it safe */
                    play_sfx_sound(SFX_PULLS_FREE_OF_LEASH);
                    pline("%s breaks loose of %s leash!", Monnam(mtmp),
                          mhis(mtmp));
                    m_unleash(mtmp, FALSE);
                }
                (void) mattacku(mtmp);
                return 0;
            }
            if (!m_in_out_region(mtmp, nix, niy))
                return 1;
            if (m_digweapon_check(mtmp, nix,niy))
                return 0;

            /* insert a worm_move() if worms ever begin to eat things */
            wasseen = canseemon(mtmp);
            remove_monster(omx, omy);
            place_monster(mtmp, nix, niy);
            play_movement_sound(mtmp, CLIMBING_TYPE_NONE);
            update_m_facing(mtmp, nix - omx, FALSE);

            if (cursemsg[chi] && (wasseen || canseemon(mtmp))) 
            {
#if 0
                /* describe top item of pile, not necessarily cursed item itself;
                   don't use glyph_at() here--it would return the pet but we want
                   to know whether an object is remembered at this map location */
                struct obj *o = (!Hallucination && level.flags.hero_memory
                                 && glyph_is_object(levl[nix][niy].hero_memory_layers.glyph))
                                   ? vobj_at(nix, niy) : 0;
#endif
                const char *what = cursedobj[chi] ? distant_name(cursedobj[chi], doname) : something;
                play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_WARN_CURSED);
                pline_multi_ex(ATR_NONE, CLR_MSG_ATTENTION, no_multiattrs, what != something ? multicolor_orange3 : get_colorless_multicolor_buffer(), "%s %s reluctantly over %s.", noit_Monnam(mtmp),
                      vtense((char *) 0, locomotion(mtmp->data, "step")), what);
            }
            for (j = MTSZ - 1; j > 0; j--)
                mtmp->mtrack[j] = mtmp->mtrack[j - 1];
            mtmp->mtrack[0].x = omx;
            mtmp->mtrack[0].y = omy;
            /* We have to know if the pet's going to do a combined eat and
             * move before moving it, but it can't eat until after being
             * moved.  Thus the do_eat flag.
             */
            if (do_eat && !shk_chastise_pet(mtmp, obj, TRUE) && !onnopickup(obj->ox, obj->oy, mtmp) && !is_obj_no_pickup(obj) && dog_wants_to_eat(mtmp))
            {
                if (dog_eat(mtmp, obj, omx, omy, FALSE) == 2)
                    return 2;
            }
            else
            {
                if (smells_buried_searchable(mtmp->data))
                {
                    int buriedsearchablefound = FALSE;
                    struct obj* otmp2;
                    for (struct obj* otmp = level.buriedobjlist; otmp; otmp = otmp2)
                    {
                        otmp2 = otmp->nobj;
                        if (otmp->ox == nix && otmp->oy == niy && is_otyp_buried_searchable(otmp->otyp))
                        {
                            buriedsearchablefound = TRUE;
                            break;
                        }
                    }
                    if (buriedsearchablefound)
                    {
                        if(canseemon(mtmp))
                            pline("%s starts digging the ground.", Monnam(mtmp));

                        int itemsfound = unearth_objs(mtmp, nix, niy, canseemon(mtmp), TRUE);
                        if (!itemsfound && canseemon(mtmp))
                            pline("%s looks disappointed.", Monnam(mtmp));
                    }
                }
            }
        } 
        else if (mtmp->mleashed && distu(omx, omy) > 4)
        {
            /* an incredible kludge, but the only way to keep pooch near
             * after it spends time eating or in a trap, etc.
             */
            coord cc;

            nx = sgn(omx - u.ux);
            ny = sgn(omy - u.uy);
            cc.x = u.ux + nx;
            cc.y = u.uy + ny;
            if (goodpos(cc.x, cc.y, mtmp, 0))
                goto dognext;

            i = xytod(nx, ny);
            for (j = (i + 7) % 8; j < (i + 1) % 8; j++) 
            {
                dtoxy(&cc, j);
                if (goodpos(cc.x, cc.y, mtmp, 0))
                    goto dognext;
            }
            for (j = (i + 6) % 8; j < (i + 2) % 8; j++)
            {
                dtoxy(&cc, j);
                if (goodpos(cc.x, cc.y, mtmp, 0))
                    goto dognext;
            }
            cc.x = mtmp->mx;
            cc.y = mtmp->my;
        dognext:
            if (!m_in_out_region(mtmp, nix, niy))
                return 1;
            remove_monster(mtmp->mx, mtmp->my);
            place_monster(mtmp, cc.x, cc.y);
            newsym(cc.x, cc.y);
            set_apparxy(mtmp);
        }
    }
    return 1;
}

/* check if a monster could pick up objects from a location */
boolean
could_reach_item(mon, nx, ny)
struct monst *mon;
xchar nx, ny;
{
    if ((!is_pool(nx, ny) || is_swimmer(mon->data))
        && (!is_lava(nx, ny) || likes_lava(mon->data))
        && (!sobj_at(BOULDER, nx, ny) || throws_rocks(mon->data))
        && mon_can_reach_floor(mon)
        )
        return TRUE;
    return FALSE;
}

/* Hack to prevent a dog from being endlessly stuck near an object that
 * it can't reach, such as caught in a teleport scroll niche.  It recursively
 * checks to see if the squares in between are good.  The checking could be
 * a little smarter; a full check would probably be useful in m_move() too.
 * Since the maximum food distance is 5, this should never be more than 5
 * calls deep.
 */
STATIC_OVL boolean
can_reach_location(mon, mx, my, fx, fy)
struct monst *mon;
xchar mx, my, fx, fy;
{
    int i, j;
    int dist;

    if (mx == fx && my == fy)
        return TRUE;
    if (!isok(mx, my))
        return FALSE; /* should not happen */

    dist = dist2(mx, my, fx, fy);
    for (i = mx - 1; i <= mx + 1; i++) {
        for (j = my - 1; j <= my + 1; j++) {
            if (!isok(i, j))
                continue;
            if (dist2(i, j, fx, fy) >= dist)
                continue;
            if (IS_ROCK(levl[i][j].typ) && !passes_walls(mon->data)
                && (!may_dig(i, j) || !tunnels(mon->data)))
                continue;
            if (IS_DOOR(levl[i][j].typ)
                && (levl[i][j].doormask & (D_CLOSED | D_LOCKED)))
                continue;
            if (!could_reach_item(mon, i, j))
                continue;
            if (can_reach_location(mon, i, j, fx, fy))
                return TRUE;
        }
    }
    return FALSE;
}

/* do_clear_area client */
STATIC_PTR void
wantdoor(x, y, distance)
int x, y;
genericptr_t distance;
{
    int ndist, *dist_ptr = (int *) distance;

    if (*dist_ptr > (ndist = distu(x, y))) {
        gx = x;
        gy = y;
        *dist_ptr = ndist;
    }
}

STATIC_VAR const struct qmchoices {
    int mndx;             /* type of pet, 0 means any  */
    char mlet;            /* symbol of pet, 0 means any */
    unsigned mappearance; /* mimic this */
    uchar m_ap_type;      /* what is the thing it is mimicing? */
} qm[] = {
    /* Things that some pets might be thinking about at the time */
    { PM_LITTLE_DOG, 0, PM_KITTEN, M_AP_MONSTER },
    { PM_DOG, 0, PM_HOUSECAT, M_AP_MONSTER },
    { PM_LARGE_DOG, 0, PM_LARGE_CAT, M_AP_MONSTER },
    { PM_KITTEN, 0, PM_LITTLE_DOG, M_AP_MONSTER },
    { PM_HOUSECAT, 0, PM_DOG, M_AP_MONSTER },
    { PM_LARGE_CAT, 0, PM_LARGE_DOG, M_AP_MONSTER },
    { PM_HOUSECAT, 0, PM_GIANT_RAT, M_AP_MONSTER },
    { 0, S_DOG, SINK,
      M_AP_FURNITURE }, /* sorry, no fire hydrants in GnollHack */
    { 0, 0, TRIPE_RATION, M_AP_OBJECT }, /* leave this at end */
};

void
finish_meating(mtmp)
struct monst *mtmp;
{
    mtmp->meating = 0;
    if (!is_mimic(mtmp->data) && M_AP_TYPE(mtmp) && mtmp->mappearance && mtmp->cham == NON_PM) 
    {
        /* was eating a mimic and now appearance needs resetting */
        mtmp->m_ap_type = 0;
        mtmp->mappearance = 0;
        if (has_mobj(mtmp))
            free_mobj(mtmp);
        newsym(mtmp->mx, mtmp->my);
    }
}

STATIC_OVL void
quickmimic(mtmp)
struct monst *mtmp;
{
    int idx = 0, trycnt = 5, spotted;
    char buf[BUFSZ];

    if (Protection_from_shape_changers || !mtmp->meating)
        return;

    /* with polymorph, the steed's equipment would be re-checked and its
       saddle would come off, triggering DISMOUNT_FELL, but mimicking
       doesn't impact monster's equipment; normally DISMOUNT_POLY is for
       rider taking on an unsuitable shape, but its message works fine
       for this and also avoids inflicting damage during forced dismount;
       do this before changing so that dismount refers to original shape */
    if (mtmp == u.usteed)
        dismount_steed(DISMOUNT_POLY);

    do {
        idx = rn2(SIZE(qm));
        if (qm[idx].mndx != 0 && mtmp->mnum == qm[idx].mndx)
            break;
        if (qm[idx].mlet != 0 && mtmp->data->mlet == qm[idx].mlet)
            break;
        if (qm[idx].mndx == 0 && qm[idx].mlet == 0)
            break;
    } while (--trycnt > 0);
    if (trycnt == 0)
        idx = SIZE(qm) - 1;

    Strcpy(buf, mon_nam(mtmp));
    spotted = canspotmon(mtmp);

    mtmp->m_ap_type = qm[idx].m_ap_type;
    mtmp->mappearance = qm[idx].mappearance;
    if (has_mobj(mtmp))
        free_mobj(mtmp);

    if (spotted || cansee(mtmp->mx, mtmp->my) || canspotmon(mtmp)) {
        /* this isn't quite right; if sensing a monster without being
           able to see its location, you really shouldn't be told you
           sense it becoming furniture or an object that you can't see
           (on the other hand, perhaps you're sensing a brief glimpse
           of its mind as it changes form) */
        newsym(mtmp->mx, mtmp->my);
        You("%s %s %sappear%s where %s was!",
            cansee(mtmp->mx, mtmp->my) ? "see" : "sense that",
            (M_AP_TYPE(mtmp) == M_AP_FURNITURE)
                ? an(defsyms[mtmp->mappearance].explanation)
                : (M_AP_TYPE(mtmp) == M_AP_OBJECT
                   && OBJ_DESCR(objects[mtmp->mappearance]))
                      ? an(OBJ_DESCR(objects[mtmp->mappearance]))
                      : (M_AP_TYPE(mtmp) == M_AP_OBJECT
                         && OBJ_NAME(objects[mtmp->mappearance]))
                            ? an(OBJ_NAME(objects[mtmp->mappearance]))
                            : (M_AP_TYPE(mtmp) == M_AP_MONSTER)
                                  ? an(pm_monster_name(&mons[mtmp->mappearance], mtmp->female))
                                  : something,
            cansee(mtmp->mx, mtmp->my) ? "" : "has ",
            cansee(mtmp->mx, mtmp->my) ? "" : "ed",
            buf);
        create_context_menu(CREATE_CONTEXT_MENU_BLOCKING_WINDOW);
        display_nhwindow(WIN_MAP, TRUE);
        create_context_menu(CREATE_CONTEXT_MENU_NORMAL);
    }
}

void
reset_dogmove(VOID_ARGS)
{
    gtyp = gx = gy = 0;
}

/*dogmove.c*/
