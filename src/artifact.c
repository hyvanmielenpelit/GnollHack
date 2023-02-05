/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-28 */

/* GnollHack 4.0    artifact.c    $NHDT-Date: 1553363416 2019/03/23 17:50:16 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.129 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2013. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "artilist.h"

/*
 * Note:  both artilist[] and artiexist[] have a dummy element #0,
 *        so loops over them should normally start at #1.  The primary
 *        exception is the save & restore code, which doesn't care about
 *        the contents, just the total size.
 */

extern boolean notonhead; /* for long worms */

/* touch_artifact()'s return value isn't sufficient to tell whether it
   dished out damage, and tracking changes to u.uhp, u.mh, Lifesaved
   when trying to avoid second wounding is too cumbersome */
STATIC_VAR boolean touch_blasted; /* for retouch_object() */

const char* artifact_invoke_names[NUM_ARTINVOKES] = {
    "taming", "healing", "mana replenishment", "untrapping", "charging",
    "level teleportation", "portal creation", "enlightenment", "arrow creation", "arrow of Diana", "death ray", "blessing of contents", "wishing",
    "summon demon", "summon elder air elemental", "recharge itself", "activates the artifact", "time stop"
};

#define get_artifact(o) \
    (((o) && (o)->oartifact) ? &artilist[(int) (o)->oartifact] : 0)

STATIC_DCL boolean FDECL(bane_applies, (const struct artifact *, struct monst *));
STATIC_DCL int FDECL(spec_applies, (const struct artifact *, struct monst *));
STATIC_DCL int FDECL(arti_invoke, (struct obj *));
STATIC_DCL boolean FDECL(Mb_hit, (struct monst * magr, struct monst *mdef, struct obj *, double *, int, BOOLEAN_P, char *));
STATIC_DCL int FDECL(glow_strength, (int));
STATIC_DCL boolean FDECL(untouchable, (struct obj *, BOOLEAN_P));
STATIC_DCL int FDECL(count_surround_traps, (int, int));
STATIC_DCL void FDECL(check_arti_name_discovery, (struct obj*));

/* The amount added to the victim's total hit points to insure that the
   victim will be killed even after damage bonus/penalty adjustments.
   Most such penalties are small, and 200 is plenty; the exception is
   half physical damage.  3.3.1 and previous versions tried to use a very
   large number to account for this case; now, we just compute the fatal
   damage by adding it to 2 times the total hit points instead of 1 time.
   Note: this will still break if they have more than about half the number
   of hit points that will fit in a 15 bit integer. */
#define FATAL_DAMAGE_MODIFIER 200

/* coordinate effects from spec_dbon() with messages in artifact_hit() */
STATIC_VAR int spec_dbon_applies = 0;

/* flags including which artifacts have already been created */
STATIC_VAR boolean artiexist[1 + NUM_ARTIFACTS + 1];
/* and a discovery list for them (no dummy first entry here) */
STATIC_VAR short artidisco[NUM_ARTIFACTS];
STATIC_VAR int nesting = 0; /* recursion control */
STATIC_VAR int mkot_trap_warn_count = 0;

STATIC_DCL void NDECL(hack_artifacts);
STATIC_DCL boolean FDECL(artifact_attack_type, (int, struct obj *));


void
artifact_taken_away(artidx)
int artidx;
{
    artiexist[artidx] = FALSE;
}

/* handle some special cases; must be called after u_init() */
STATIC_OVL void
hack_artifacts()
{
    struct artifact *art;
    int alignmnt = aligns[flags.initalign].value;

    /* Fix up the alignments of "gift" artifacts */
    for (art = artilist + 1; art->otyp; art++)
        if (art->role == Role_switch && art->alignment != A_NONE)
            art->alignment = alignmnt;

    /* Excalibur can be used by any lawful character, not just knights */
    if (!Role_if(PM_KNIGHT))
        artilist[ART_EXCALIBUR].role = NON_PM;

    /* Fix up the quest artifact */
    if (urole.questarti) {
        artilist[urole.questarti].alignment = alignmnt;
        artilist[urole.questarti].role = Role_switch;
    }
    return;
}

/* zero out the artifact existence list */
void
init_artifacts()
{
    (void) memset((genericptr_t) artiexist, 0, sizeof artiexist);
    (void) memset((genericptr_t) artidisco, 0, sizeof artidisco);
    hack_artifacts();
}

void
save_artifacts(fd)
int fd;
{
    bwrite(fd, (genericptr_t) artiexist, sizeof artiexist);
    bwrite(fd, (genericptr_t) artidisco, sizeof artidisco);
}


STATIC_VAR boolean artivaluessaved = FALSE;
STATIC_VAR struct artifact saved_artilist[NUM_ARTIFACTS + 2];

void
save_initial_artifact_values(VOID_ARGS)
{
    if (!artivaluessaved)
    {
        memcpy((genericptr_t)saved_artilist, (genericptr_t)artilist, sizeof(struct artifact) * (NUM_ARTIFACTS + 2));
        artivaluessaved = TRUE;
    }
}

void
reset_artifacts(VOID_ARGS)
{
    memset((genericptr_t)artiexist, 0, sizeof artiexist);
    memset((genericptr_t)artidisco, 0, sizeof artidisco);
    memcpy((genericptr_t)artilist, (genericptr_t)saved_artilist, sizeof(struct artifact) * (NUM_ARTIFACTS + 2));
    mkot_trap_warn_count = 0;
    nesting = 0;
    spec_dbon_applies = 0;
    touch_blasted = 0;
}


void
restore_artifacts(fd)
int fd;
{
    mread(fd, (genericptr_t) artiexist, sizeof artiexist);
    mread(fd, (genericptr_t) artidisco, sizeof artidisco);
    hack_artifacts(); /* redo non-saved special cases */
}

const char *
artiname(artinum)
short artinum;
{
    if (artinum <= 0 || artinum > NUM_ARTIFACTS)
        return empty_string;
    return artilist[artinum].name;
}

/*
   Make an artifact.  If a specific alignment is specified, then an object of
   the appropriate alignment is created from scratch, or 0 is returned if
   none is available.  (If at least one aligned artifact has already been
   given, then unaligned ones also become eligible for this.)
   If no alignment is given, then 'otmp' is converted
   into an artifact of matching type, or returned as-is if that's not
   possible.
   For the 2nd case, caller should use ``obj = mk_artifact(obj, A_NONE);''
   for the 1st, ``obj = mk_artifact((struct obj *)0, some_alignment);''.
 */
struct obj *
mk_artifact(otmp, alignment, mkflags)
struct obj *otmp;   /* existing object; ignored if alignment specified */
aligntyp alignment; /* target alignment, or A_NONE */
uchar mkflags; /* for monks */
{
    const struct artifact *a;
    int m, n, altn;
    boolean by_align = (alignment != A_NONE);
    short o_typ = (by_align || !otmp) ? 0 : otmp->otyp;
    boolean unique = !by_align && otmp && is_otyp_unique(o_typ);
    short eligible[NUM_ARTIFACTS];
    boolean nonweaponsonly = !!(mkflags & MKARTIFACT_FLAGS_NONWEAPONS_ONLY);
    boolean novorpal = !!(mkflags & MKARTIFACT_FLAGS_NO_VORPAL_WEAPONS);
    boolean weaponsonly = !!(mkflags & MKARTIFACT_FLAGS_WEAPONS_ONLY);

    n = altn = 0;    /* no candidates found yet */
    eligible[0] = 0; /* lint suppression */
    /* gather eligible artifacts */
    for (m = 1, a = &artilist[m]; a->otyp; a++, m++) 
    {
        if (artiexist[m])
            continue;
        if ((a->aflags & AF_NOGEN) || unique)
            continue;
        if (nonweaponsonly && objects[a->otyp].oc_class == WEAPON_CLASS)
            continue;
        if (weaponsonly && objects[a->otyp].oc_class != WEAPON_CLASS)
            continue;
        if (novorpal && ((a->aflags & (AF_BEHEAD | AF_BISECT)) != 0 || (objects[a->otyp].oc_aflags & (A1_VORPAL | A1_BISECT)) != 0))
            continue;

        if (!by_align) {
            /* looking for a particular type of item; not producing a
               divine gift so we don't care about role's first choice */
            if (a->otyp == o_typ || o_typ == 0)
                eligible[n++] = m;
            continue; /* move on to next possibility */
        }

        /* we're looking for an alignment-specific item
           suitable for hero's role+race */
        if ((a->alignment == alignment || a->alignment == A_NONE)
            /* avoid enemies' equipment */
            && (a->race == NON_PM || !race_hostile(&mons[a->race])))
        {
            /* when a role-specific first choice is available, use it */
            if (Role_if(a->role))
            {
                /* make this be the only possibility in the list */
                eligible[0] = m;
                n = 1;
                break; /* skip all other candidates */
            }

            /* found something to consider for random selection */
            if (a->alignment != A_NONE || u.ugifts > 0) 
            {
                /* right alignment, or non-aligned with at least 1
                   previous gift bestowed, makes this one viable */
                eligible[n++] = m;
            } 
            else
            {
                /* non-aligned with no previous gifts;
                   if no candidates have been found yet, record
                   this one as a[nother] fallback possibility in
                   case all aligned candidates have been used up
                   (via wishing, naming, bones, random generation) */
                if (!n)
                    eligible[altn++] = m;
                /* [once a regular candidate is found, the list
                   is overwritten and `altn' becomes irrelevant] */
            }
        }
    }

    /* resort to fallback list if main list was empty */
    if (!n)
        n = altn;

    if (n) 
    {
        /* found at least one candidate; pick one at random */
        m = eligible[rn2(n)]; /* [0..n-1] */
        a = &artilist[m];

        /* make an appropriate object if necessary, then christen it */
        if (by_align || !otmp)
            otmp = mksobj((int) a->otyp, TRUE, FALSE, FALSE);

        if (otmp) 
        {
            otmp = oname(otmp, a->name);
            otmp->oartifact = m;
            artiexist[m] = TRUE;
            otmp->exceptionality = artilist[otmp->oartifact].exceptionality;
            otmp->mythic_prefix = artilist[otmp->oartifact].mythic_prefix;
            otmp->mythic_suffix = artilist[otmp->oartifact].mythic_suffix;
            if(artilist[otmp->oartifact].material)
                otmp->material = artilist[otmp->oartifact].material;
        }
    } 
    else 
    {
        /* nothing appropriate could be found; return original object */
        if (by_align)
            otmp = 0; /* (there was no original object) */
    }
    return otmp;
}

/*
 * Returns the full name (with articles and correct capitalization) of an
 * artifact named "name" if one exists, or NULL, it not.
 * The given name must be rather close to the real name for it to match.
 * The object type of the artifact is returned in otyp if the return value
 * is non-NULL.
 */
const char *
artifact_name(name, otyp)
const char *name;
short *otyp;
{
    register const struct artifact *a;
    register const char *aname;

    if (!strncmpi(name, "the ", 4))
        name += 4;

    for (a = artilist + 1; a->otyp; a++) {
        aname = a->name;
        if (!strncmpi(aname, "the ", 4))
            aname += 4;
        if (!strcmpi(name, aname)) {
            *otyp = a->otyp;
            return a->name;
        }
    }

    return (char *) 0;
}

boolean
exist_artifact(otyp, name)
int otyp;
const char *name;
{
    register const struct artifact *a;
    boolean *arex;

    if (otyp && *name)
        for (a = artilist + 1, arex = artiexist + 1; a->otyp; a++, arex++)
            if ((int) a->otyp == otyp && !strcmp(a->name, name))
                return *arex;
    return FALSE;
}

void
artifact_exists(otmp, name, mod)
struct obj *otmp;
const char *name;
boolean mod;
{
    register const struct artifact *a;

    if (otmp && *name)
        for (a = artilist + 1; a->otyp; a++)
            if (a->otyp == otmp->otyp && !strcmp(a->name, name)) 
            {
                register int m = (int) (a - artilist);
                otmp->oartifact = (char) (mod ? m : 0);
                otmp->age = 0;
                if (otmp->otyp == RIN_INCREASE_DAMAGE)
                    otmp->enchantment = 0;
                artiexist[m] = mod;
                if (otmp->oartifact)
                {
                    otmp->exceptionality = artilist[otmp->oartifact].exceptionality;
                    otmp->mythic_prefix = artilist[otmp->oartifact].mythic_prefix;
                    otmp->mythic_suffix = artilist[otmp->oartifact].mythic_suffix;
                    if (artilist[otmp->oartifact].material)
                        otmp->material = artilist[otmp->oartifact].material;

                    if (artilist[otmp->oartifact].aflags & AF_FAMOUS)
                        otmp->nknown = TRUE;

                    if (is_quest_artifact(otmp))
                        otmp->nknown = TRUE;

                    if (otmp->mythic_prefix == MYTHIC_PREFIX_HALLOWED && otmp->cursed)
                    {
                        uncurse(otmp);
                        otmp->enchantment = (short)abs(otmp->enchantment);
                    }
                }
                break;
            }
    return;
}


int
get_artifact_id(otyp, name)
int otyp;
const char* name;
{
    if (otyp > 0 && *name)
    {
        for (int i = 1; i <= NUM_ARTIFACTS; i++)
            if (artilist[i].otyp == otyp && !strcmp(artilist[i].name, name))
            {
                return i;
            }
    }
    return 0;
}

int
nartifact_exist()
{
    int a = 0;
    int n = SIZE(artiexist);

    while (n > 1)
        if (artiexist[--n])
            a++;

    return a;
}

boolean
artifact_has_flag(otmp, abil)
struct obj *otmp;
unsigned long abil;
{
    const struct artifact *arti = get_artifact(otmp);

    return (boolean) (arti && (arti->aflags & abil) != 0L);
}


boolean
artifact_confers_monster_power(mon, otmp, prop_index)
struct monst* mon; /* not used at the moment, but here in case artifact powers will depend on the wielder */
struct obj* otmp;
int prop_index;
{
    if (!otmp)
        return FALSE;

    /* Remove gcc warning */
    if (!mon)
    {
        /* Do nothing, since mon is not being used */
    }

    const struct artifact* arti = get_artifact(otmp);
    if (!arti)
        return FALSE;

    /* Normal wielded and carried properties */
    if ((arti->carried_prop == prop_index)
        || ((arti->worn_prop == prop_index)
            && (is_wielded_item(otmp) ? (otmp->owornmask & W_WIELDED_WEAPON) : (otmp->owornmask & (W_ARMOR | W_ACCESSORY)))
            )
        )
        return TRUE;

    /* Invoked property */
    if (arti->inv_prop == prop_index && otmp->invokeon)
        return TRUE;

    /* Properties from spfx and cspx flags */
    unsigned long abil = prop_to_spfx(prop_index);
    boolean worn_mask_ok = FALSE;
    if (is_wielded_item(otmp))
        worn_mask_ok = !!(otmp->owornmask & W_WIELDED_WEAPON);
    else
        worn_mask_ok = !!(otmp->owornmask & (W_ARMOR | W_ACCESSORY));

    if ((arti->cspfx & abil)
        || ((arti->spfx & abil)    && worn_mask_ok))
        return TRUE;

    return FALSE;
}


/* used so that callers don't need to known about SPFX_ codes */
boolean
confers_luck(uitem)
struct obj *uitem;
{
    if (!uitem)
        return FALSE;

    int otyp = uitem->otyp;
    if (!(objects[otyp].oc_pflags & (P1_CONFERS_LUCK | P1_CONFERS_UNLUCK)) && !has_obj_mythic_luck(uitem))
        return FALSE;

    boolean inappr = inappropriate_character_type(uitem);
    boolean luck = has_obj_mythic_luck(uitem) || (
        ((objects[otyp].oc_pflags & P1_CONFERS_LUCK) && !((objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY) && inappr))
        || (
                (objects[otyp].oc_pflags & P1_CONFERS_UNLUCK) && 
                (
                    ((objects[otyp].oc_pflags & P1_CURSED_ITEM_YIELDS_NEGATIVE) && uitem->cursed) || ((objects[otyp].oc_pflags & P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS) && inappr)
                )
            )
        );
    boolean unluck = (
        ((objects[otyp].oc_pflags & P1_CONFERS_UNLUCK) && !((objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY) && inappr))
        || (
                (objects[otyp].oc_pflags & P1_CONFERS_LUCK) &&
                (
                    ((objects[otyp].oc_pflags & P1_CURSED_ITEM_YIELDS_NEGATIVE) && uitem->cursed) || ((objects[otyp].oc_pflags & P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS) && inappr)
                )
            )
        );
    boolean luck_obtained = (luck && !unluck);
    if (!luck_obtained)
        return FALSE;

    boolean worn = is_obj_worn(uitem);

    if ((worn || (!worn && (objects[otyp].oc_pflags & P1_LUCK_APPLIES_WHEN_CARRIED)))
        && ((!inappr && !(objects[otyp].oc_pflags & (P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
            || (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_ALL_CHARACTERS)
            || (inappr && (objects[otyp].oc_pflags & (P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY | P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)))
            )
        )
    {
        return luck_obtained;
    }

    return FALSE;
}

/* used so that callers don't need to known about SPFX_ codes */
boolean
confers_unluck(uitem)
struct obj* uitem;
{
    if (!uitem)
        return FALSE;

    int otyp = uitem->otyp;
    if (!(objects[otyp].oc_pflags & (P1_CONFERS_LUCK | P1_CONFERS_UNLUCK)) && !has_obj_mythic_luck(uitem))
        return FALSE;

    boolean inappr = inappropriate_character_type(uitem);
    boolean luck = has_obj_mythic_luck(uitem) || (
        ((objects[otyp].oc_pflags & P1_CONFERS_LUCK) && !((objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY) && inappr))
        || (
                (objects[otyp].oc_pflags & P1_CONFERS_UNLUCK) &&
                (
                    ((objects[otyp].oc_pflags & P1_CURSED_ITEM_YIELDS_NEGATIVE) && uitem->cursed) || ((objects[otyp].oc_pflags & P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS) && inappr)
                )
            )
        );
    boolean unluck = (
        ((objects[otyp].oc_pflags & P1_CONFERS_UNLUCK) && !((objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY) && inappr))
            || (
                     (objects[otyp].oc_pflags & P1_CONFERS_LUCK) && 
                     (
                        ((objects[otyp].oc_pflags & P1_CURSED_ITEM_YIELDS_NEGATIVE) && uitem->cursed) || ((objects[otyp].oc_pflags & P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS) && inappr)
                     )
               )
        );

    boolean unluck_obtained = (!luck && unluck);
    if (!unluck_obtained)
        return FALSE;

    boolean worn = is_obj_worn(uitem);

    if ((worn || (!worn && (objects[otyp].oc_pflags & P1_LUCK_APPLIES_WHEN_CARRIED)))
        && ((!inappr && !(objects[otyp].oc_pflags & (P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
            || (objects[otyp].oc_pflags & P1_LUCK_APPLIES_TO_ALL_CHARACTERS)
            || (inappr && (objects[otyp].oc_pflags & (P1_LUCK_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY | P1_LUCK_NEGATIVE_TO_INAPPROPRIATE_CHARACTERS)))
            )
        )
    {
        return unluck_obtained;
    }

    return FALSE; 
}

/* obj is assumed to be carried */
boolean
artifact_confers_luck(obj)
struct obj* obj;
{
    return (obj && obj->oartifact && ((obj->owornmask != 0 && (artilist[obj->oartifact].spfx & SPFX_LUCK)) || (artilist[obj->oartifact].cspfx & SPFX_LUCK)));
}

/* obj is assumed to be carried */
boolean
artifact_confers_unluck(obj)
struct obj* obj;
{
    return (obj && obj->oartifact && ((obj->owornmask != 0 && (artilist[obj->oartifact].spfx & SPFX_UNLUCK)) || (artilist[obj->oartifact].cspfx & SPFX_UNLUCK)));
}

/* used to check whether a monster is getting reflection from an artifact */
boolean
arti_reflects(obj)
struct obj *obj;
{
    const struct artifact *arti = get_artifact(obj);

    if (arti) {
        /* while being worn */
        if ((obj->owornmask & ~W_ARTIFACT_CARRIED) && (arti->spfx & SPFX_REFLECT))
            return TRUE;
        /* just being carried */
        if (arti->cspfx & SPFX_REFLECT)
            return TRUE;
    }
    return FALSE;
}

/* decide whether this obj is effective when attacking against shades;
   does not consider the bonus for blessed objects versus undead */
boolean
shade_glare(obj)
struct obj *obj;
{
    if (!obj)
        return FALSE;

    const struct artifact *arti;

    /* any silver object is effective */
    if (obj->material == MAT_SILVER)
        return TRUE;

    /* any blessed object is effective */
    if (obj->blessed)
        return TRUE;

    /* non-silver artifacts with bonus against undead also are effective */
    arti = get_artifact(obj);
    if (arti && (arti->aflags & AF_DFLAG2) && (arti->mtype & M2_UNDEAD))
        return TRUE;

    if (obj->mythic_suffix && has_obj_mythic_suffix_power(obj, MYTHIC_SUFFIX_POWER_INDEX_UNDEAD_DESTRUCTION))
        return TRUE;

    /* [if there was anything with special bonus against noncorporeals,
       it would be effective too] */
    /* otherwise, harmless to shades */
    return FALSE;
}

/* returns 1 if name is restricted for otmp->otyp */
boolean
restrict_name(otmp, name)
struct obj *otmp;
const char *name;
{
    register const struct artifact *a;
    const char *aname, *odesc, *other;
    boolean sametype[NUM_OBJECTS];
    int i, lo, hi, otyp = otmp->otyp, ocls = objects[otyp].oc_class;

    if (!*name)
        return FALSE;
    if (!strncmpi(name, "the ", 4))
        name += 4;

    /* decide what types of objects are the same as otyp;
       if it's been discovered, then only itself matches;
       otherwise, include all other undiscovered objects
       of the same class which have the same description
       or share the same pool of shuffled descriptions */
    (void) memset((genericptr_t) sametype, 0, sizeof sametype); /* FALSE */
    sametype[otyp] = TRUE;
    if (!objects[otyp].oc_name_known
        && (odesc = OBJ_DESCR(objects[otyp])) != 0) {
        obj_shuffle_range(otyp, &lo, &hi);
        for (i = bases[ocls]; i < NUM_OBJECTS; i++) {
            if (objects[i].oc_class != ocls)
                break;
            if (!objects[i].oc_name_known
                && (other = OBJ_DESCR(objects[i])) != 0
                && (!strcmp(odesc, other) || (i >= lo && i <= hi)))
                sametype[i] = TRUE;
        }
    }

    /* Since almost every artifact is AF_RESTR, it doesn't cost
       us much to do the string comparison before the spfx check.
       Bug fix:  don't name multiple elven daggers "Sting".
     */
    for (a = artilist + 1; a->otyp; a++) {
        if (!sametype[a->otyp] && !sametype[a->maskotyp])
            continue;
        aname = a->name;
        if (!strncmpi(aname, "the ", 4))
            aname += 4;
        if (!strcmp(aname, name))
            return (boolean) ((a->aflags & (AF_NOGEN | AF_RESTR)) != 0
                              || otmp->quan > 1L);
    }

    return FALSE;
}

STATIC_OVL boolean
artifact_attack_type(adtyp, otmp)
int adtyp;
struct obj *otmp;
{
    register const struct artifact *weap;

    if ((weap = get_artifact(otmp)) != 0)
        return (boolean) (weap->attk.adtyp == adtyp);
    return FALSE;
}


/*
 * a potential artifact has just been worn/wielded/picked-up or
 * unworn/unwielded/dropped.  Pickup/drop only set/reset the W_ARTIFACT_CARRIED mask.
 */
void
set_artifact_intrinsic(otmp, wp_mask)
struct obj *otmp;
long wp_mask;
{
    long *propptr = 0;
    register const struct artifact *oart = get_artifact(otmp);
    register uchar proptyp;
    register unsigned long spfx;

    if (!oart)
        return;

    /* effects from the worn_prop field */
    proptyp = (wp_mask != W_ARTIFACT_CARRIED) ? oart->worn_prop : oart->carried_prop;

    if(proptyp > 0 && proptyp <= LAST_PROP)
    {
        propptr = &u.uprops[proptyp].extrinsic;
        *propptr |= wp_mask;
    }

    /* intrinsics from the spfx and cspfx fields; there could be more than one */
    spfx = (wp_mask != W_ARTIFACT_CARRIED) ? oart->spfx : oart->cspfx;

    unsigned long bit = 0x00000001UL;
    for (int i = 0; i < 32; i++)
    {
        if (i > 0)
            bit = bit << 1;

        if (spfx & bit)
        {
            int prop = spfx_to_prop(bit);
            if (prop > 0 && prop <= LAST_PROP)
                u.uprops[prop].extrinsic |= wp_mask;
        }
    }

    /* set monster type */
    if (spfx & SPFX_WARN_OF_MON) {
        if (spec_m2(otmp)) 
        {
            context.warntype.obj |= spec_m2(otmp);
        } 
    }

}

/*
 * creature (usually hero) tries to touch (pick up or wield) an artifact obj.
 * Returns 0 if the object refuses to be touched.
 * This routine does not change any object chains.
 * Ignores such things as gauntlets, assuming the artifact is not
 * fooled by such trappings.
 */
int
touch_artifact(obj, mon)
struct obj *obj;
struct monst *mon;
{
    if (!obj || !mon)
        return 1;

    register const struct artifact* oart = get_artifact(obj);
    boolean badclass = FALSE, badalign = FALSE, badappropriate = FALSE, badexceptional = FALSE, self_willed, yours;

    touch_blasted = FALSE;
    if (!oart && !(objects[obj->otyp].oc_flags4 & O4_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS) && obj->exceptionality < EXCEPTIONALITY_CELESTIAL)
        return 1;

    yours = (mon == &youmonst);
    /* all quest artifacts are self-willed; if this ever changes, `badclass'
       will have to be extended to explicitly include quest artifacts */
    if (oart)
    {
        self_willed = ((oart->aflags & AF_INTEL) != 0);
        if (yours) {
            badclass = self_willed
                && ((oart->role != NON_PM && !Role_if(oart->role))
                    || (oart->race != NON_PM && !Race_if(oart->race)));
            badalign = ((oart->aflags & AF_RESTR) != 0
                && oart->alignment != A_NONE
                && (oart->alignment != u.ualign.type
                    || u.ualign.record < 0));

            if (!obj->nknown && (oart->aflags & (AF_FAMOUS | AF_NAME_KNOWN_WHEN_PICKED_UP)))
            {
                play_sfx_sound(SFX_ARTIFACT_NAME_KNOWN);
                pline_ex(ATR_NONE, CLR_MSG_HINT, "As you touch %s, you become aware that it is called %s!", the(cxname(obj)), bare_artifactname(obj));
                obj->nknown = TRUE;
            }
        }
        else if (mon->mnum != PM_WIZARD_OF_YENDOR && !((mon->data->mflags3 & M3_WANTSARTI) && is_quest_artifact(obj)) /*!is_covetous(mon->data)*/ && !is_mplayer(mon->data)) {
            badclass = self_willed && oart->role != NON_PM
                && oart != &artilist[ART_EXCALIBUR];
            badalign = (oart->aflags & AF_RESTR) && oart->alignment != A_NONE
                && (oart->alignment != mon_aligntyp(mon));

        }
        else { /* an M3_WANTSxxx monster or a fake player */
         /* special monsters trying to take the Amulet, invocation tools or
            quest item can touch anything except `spec_applies' artifacts */
            badclass = badalign = FALSE;
        }
        /* weapons which attack specific categories of monsters are
           bad for them even if their alignments happen to match */
        if (!badalign)
            badalign = bane_applies(oart, mon);
    }

    if (objects[obj->otyp].oc_flags4 & O4_DEALS_DAMAGE_TO_INAPPROPRIATE_CHARACTERS)
        badappropriate = inappropriate_monster_character_type(mon, obj);
    
    if (obj->exceptionality >= EXCEPTIONALITY_CELESTIAL)
        badexceptional = inappropriate_exceptionality(mon, obj);

    if (badappropriate || badexceptional ||  (((badclass || badalign) && self_willed)
            || (badalign && (!yours || !rn2(4)))))
    {
            int dmg = 0;
            double damage = 0;
            char buf[BUFSZ];

            if (!yours)
                return 0;

            play_sfx_sound(SFX_BLASTED_BY_POWER);
            if(oart)
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are blasted by %s power!", s_suffix(the(xname(obj))));
            else
                You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "are shocked by %s enchantment!", s_suffix(the(xname(obj))));

            touch_blasted = TRUE;
            if (badappropriate || badexceptional)
                dmg = weapon_total_dmg_value(obj, &youmonst, &youmonst, 0);
            if(badclass || badalign)
                dmg += d((Antimagic_or_resistance ? 2 : 4), (self_willed ? 10 : 4));
            /* add half (maybe quarter) of the usual silver damage bonus */
            if (obj->material == MAT_SILVER && Hate_silver)
                dmg += rnd(10);
            damage = adjust_damage(dmg, (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE);

            Sprintf(buf, "touching %s", (oart ? oart->name : acxname(obj)));
            losehp(damage, buf, KILLED_BY); /* magic damage, not physical */
            exercise(A_WIS, FALSE);
    }


    /* can pick it up unless you're totally non-synch'd with the artifact */
    if (badclass && badalign && self_willed) {
        if (yours) {
            if (!carried(obj))
                pline("%s your grasp!", Tobjnam(obj, "evade"));
            else
                pline("%s beyond your control!", Tobjnam(obj, "are"));
        }
        return 0;
    }

    return 1;
}

/* decide whether an artifact itself is vulnerable to a particular type
   of erosion damage, independent of the properties of its bearer */
boolean
arti_immune(obj, dtyp)
struct obj *obj;
int dtyp;
{
    register const struct artifact *weap = get_artifact(obj);

    if (!weap)
        return FALSE;
    if (dtyp == AD_PHYS)
        return FALSE; /* nothing is immune to phys dmg */
    return (boolean) (weap->attk.adtyp == dtyp
                      || (weap->worn_prop && prop_to_adtyp(weap->worn_prop) == dtyp)
                      || (weap->carried_prop && prop_to_adtyp(weap->carried_prop) == dtyp)
        );
}

STATIC_OVL boolean
bane_applies(oart, mon)
const struct artifact *oart;
struct monst *mon;
{
    struct artifact atmp;

    if (oart && (oart->aflags & AF_DBONUS) != 0) {
        atmp = *oart;
        atmp.aflags &= AF_DBONUS; /* clear other spfx fields */
        if (spec_applies(&atmp, mon))
            return TRUE;
    }
    return FALSE;
}

/* decide whether an artifact's special attacks apply against mtmp */
STATIC_OVL int
spec_applies(weap, mtmp)
register const struct artifact *weap;
struct monst *mtmp;
{
    struct permonst *ptr;
    boolean yours;

    yours = (mtmp == &youmonst);
    ptr = mtmp->data;

    if (weap->aflags & AF_DMONS) 
    {
        return (ptr == &mons[(int) weap->mtype]);
    } 
    else if (weap->aflags & AF_DCLAS) 
    {
        return (weap->mtype == (unsigned long) ptr->mlet);
    } 
    else if (weap->aflags & AF_DFLAG1) 
    {
        return ((ptr->mflags1 & weap->mtype) != 0L);
    } 
    else if (weap->aflags & AF_DFLAG2)
    {
        return ((ptr->mflags2 & weap->mtype)
                || (yours
                    && ((!Upolyd && (urace.selfmask & weap->mtype))
                        || ((weap->mtype & M2_WERE) && u.ulycn >= LOW_PM))));
    } 
    else if (weap->aflags & AF_DALIGN) 
    {
        return yours ? (u.ualign.type != weap->alignment)
                     : (ptr->maligntyp == A_NONE
                        || sgn(ptr->maligntyp) != weap->alignment);
    } 
    else
    {
        switch (weap->attk.adtyp) 
        {
        case AD_FIRE:
            return !(yours ? Fire_immunity : is_mon_immune_to_fire(mtmp));
        case AD_COLD:
            return !(yours ? Cold_immunity : is_mon_immune_to_cold(mtmp));
        case AD_ELEC:
            return !(yours ? Shock_immunity : is_mon_immune_to_elec(mtmp));
        case AD_DISN:
            return !(yours ? Disint_resistance : resists_disint(mtmp));
        case AD_DRAY:
            return !(yours ? Death_resistance : resists_death(mtmp));
        case AD_WERE:
            return !(yours ? Lycanthropy_resistance : resists_lycanthropy(mtmp));
        case AD_MAGM:
            return !check_magic_resistance_and_inflict_damage(mtmp, (struct obj*)0, (struct monst*)0, FALSE, 0, 0, NOTELL);
        case AD_STUN:
            return !(yours ? Stun_resistance : resists_stun(mtmp));
        case AD_DRST:
            return !(yours ? Poison_resistance : resists_poison(mtmp));
        case AD_DRLI:
            return !(yours ? Drain_resistance : resists_drli(mtmp));
        case AD_STON:
            return !(yours ? Stone_resistance : resists_ston(mtmp));
        case AD_DISE:
        case AD_ROTS:
            return !(yours ? Sick_resistance : resists_sickness(mtmp));
        case AD_PLYS:
            return !(yours ? Free_action : resists_paralysis(mtmp));
        case AD_BLND:
            return !(yours ? Flash_resistance : resists_blnd(mtmp) || resists_flash(mtmp));
        case AD_PHYS:
            return 1;
        }
    }
    return 0;
}


boolean
is_immune(mtmp, dmgtype)
struct monst* mtmp;
int dmgtype;
{
    if (!mtmp)
        return FALSE;

    boolean yours = (mtmp == &youmonst);

    switch (dmgtype)
    {
    case AD_MAGM:
        return (yours ? Magic_missile_immunity : is_mon_immune_to_magic_missile(mtmp));
    case AD_FIRE:
        return (yours ? Fire_immunity : is_mon_immune_to_fire(mtmp));
    case AD_COLD:
        return (yours ? Cold_immunity : is_mon_immune_to_cold(mtmp));
    case AD_SLEE:
        return (yours ? Sleep_resistance : resists_sleep(mtmp));
    case AD_DISN:
        return (yours ? Disint_resistance : resists_disint(mtmp));
    case AD_ELEC:
        return (yours ? Shock_immunity : is_mon_immune_to_elec(mtmp));
    case AD_DRST:
        return (yours ? Poison_resistance : resists_poison(mtmp));
    case AD_ACID:
        return (yours ? Acid_immunity : is_mon_immune_to_acid(mtmp));
    case AD_DRAY:
        return (yours ? Death_resistance : resists_death(mtmp));
    case AD_BLND:
        return (yours ? Flash_resistance || Blind || Unaware : resists_blnd(mtmp));
    case AD_WERE:
        return (yours ? Lycanthropy_resistance : resists_lycanthropy(mtmp));
    case AD_STUN:
        return (yours ? Stun_resistance : resists_stun(mtmp));
    case AD_DRLI:
        return (yours ? Drain_resistance : resists_drli(mtmp));
    case AD_STON:
        return (yours ? Stone_resistance : resists_ston(mtmp));
    case AD_DISE:
    case AD_ROTS:
        return (yours ? Sick_resistance : resists_sickness(mtmp));
    case AD_PLYS:
        return (yours ? Free_action : resists_paralysis(mtmp));
    case AD_PHYS:
        return FALSE;
    }

    return FALSE;

}
/* return the M2 flags of monster that an artifact's special attacks apply
 * against */
long
spec_m2(otmp)
struct obj *otmp;
{
    const struct artifact *artifact = get_artifact(otmp);

    if (artifact)
        return artifact->mtype;
    return 0L;
}

/* special attack bonus */
int
spec_abon(otmp, mon)
struct obj *otmp;
struct monst *mon;
{
    const struct artifact *weap = get_artifact(otmp);

    /* no need for an extra check for `NO_ATTK' because this will
       always return 0 for any artifact which has that attribute */
    if (!weap || !spec_applies(weap, mon))
        return 0;

    int dice = weap->tohit_dice;
    int size = weap->tohit_diesize;
    int plus = weap->tohit_plus;

    int abon = plus;
    if (dice > 0 && size > 0)
        abon += d(dice, size);

    if (obj_has_dual_runesword_bonus(otmp))
        abon *= 2;

    return abon;
}

/* special damage bonus */
double
spec_dbon(otmp, mon, damage)
struct obj *otmp;
struct monst *mon;
double damage;
{
    register const struct artifact *weap = get_artifact(otmp);

    if (!weap || (weap->attk.adtyp == AD_PHYS /* check for `NO_ATTK' */
                  && weap->attk.damn == 0 && weap->attk.damd == 0 && weap->attk.damp == 0))
        spec_dbon_applies = FALSE;
    else
        spec_dbon_applies = spec_applies(weap, mon);

    if (spec_dbon_applies)
    {
        double dbon = 0;
        if (weap->attk.damd > 0 && weap->attk.damn > 0)
            dbon += adjust_damage(d(weap->attk.damn, weap->attk.damd) + weap->attk.damp, (struct monst*)0, mon, !weap ? AD_PHYS : weap->attk.adtyp, ADFLAGS_NONE);
        else if(weap->attk.damn < 0)
            dbon += max(-(((double)weap->attk.damn) / 20.0) * damage, 0);

        if (obj_has_dual_runesword_bonus(otmp))
            dbon *= 2;

        return dbon;
    }

    return 0;
}

/* add identified artifact to discoveries list */
void
discover_artifact(m)
short m;
{
    int i;

    /* look for this artifact in the discoveries list;
       if we hit an empty slot then it's not present, so add it */
    for (i = 0; i < NUM_ARTIFACTS; i++)
        if (artidisco[i] == 0 || artidisco[i] == m) {
            artidisco[i] = m;
            return;
        }
    /* there is one slot per artifact, so we should never reach the
       end without either finding the artifact or an empty slot... */
    impossible("couldn't discover artifact (%d)", (int) m);
}

/* used to decide whether an artifact has been fully identified */
boolean
undiscovered_artifact(m)
short m;
{
    int i;

    /* look for this artifact in the discoveries list;
       if we hit an empty slot then it's undiscovered */
    for (i = 0; i < NUM_ARTIFACTS; i++)
        if (artidisco[i] == m)
            return FALSE;
        else if (artidisco[i] == 0)
            break;
    return TRUE;
}

/* display a list of discovered artifacts; return their count */
int
disp_artifact_discoveries(tmpwin)
winid tmpwin; /* supplied by dodiscover() */
{
    int i, otyp;
    short m;
    char buf[BUFSZ];

    for (i = 0; i < NUM_ARTIFACTS; i++) {
        if (artidisco[i] == 0)
            break; /* empty slot implies end of list */
        if (tmpwin == WIN_ERR)
            continue; /* for WIN_ERR, we just count */

        if (i == 0)
            putstr(tmpwin, iflags.menu_headings, "Artifacts");
        m = artidisco[i];
        otyp = (int)artilist[m].otyp;
        Sprintf(buf, "  %s [%s %s]", artiname(m),
                align_str(artilist[m].alignment), simple_typename(otyp));
        putstr(tmpwin, 0, buf);
    }
    return i;
}

/*
 * Magicbane's intrinsic magic is incompatible with normal
 * enchantment magic.  Thus, its effects have a negative
 * dependence on enchantment.  Against low mr victims, it typically
 * does "double athame" damage, 2d4.  Occasionally, it will
 * cast unbalancing magic which effectively averages out to
 * 4d4 damage (3d4 against high mr victims), for enchantment = 0.
 *
 * Prior to 3.4.1, the cancel (aka purge) effect always
 * included the scare effect too; now it's one or the other.
 * Likewise, the stun effect won't be combined with either
 * of those two; it will be chosen separately or possibly
 * used as a fallback when scare or cancel fails.
 *
 * [Historical note: a change to artifact_hit() for 3.4.0
 * unintentionally made all of Magicbane's special effects
 * be blocked if the defender successfully saved against a
 * stun attack.  As of 3.4.1, those effects can occur but
 * will be slightly less likely than they were in 3.3.x.]
 */

enum mb_effect_indices {
    MB_INDEX_PROBE = 0,
    MB_INDEX_STUN,
    MB_INDEX_SCARE,
    MB_INDEX_CANCEL,

    NUM_MB_INDICES
};

#define MB_MAX_DIEROLL 8 /* rolls above this aren't magical */
STATIC_VAR const char *const mb_verb[2][NUM_MB_INDICES] = {
    { "probe", "stun", "scare", "cancel" },
    { "prod", "amaze", "tickle", "purge" },
};

/* called when someone is being hit by Magicbane */
STATIC_OVL boolean
Mb_hit(magr, mdef, mb, dmgptr, dieroll, vis, hittee)
struct monst *magr, *mdef; /* attacker and defender */
struct obj *mb;            /* Magicbane */
double *dmgptr;               /* extra damage target will suffer */
int dieroll;               /* d20 that has already scored a hit */
boolean vis;               /* whether the action can be seen */
char *hittee;              /* target's name: "you" or mon_nam(mdef) */
{
    struct permonst *old_uasmon;
    const char *verb, *fakename;
    boolean youattack = (magr == &youmonst), youdefend = (mdef == &youmonst),
            resisted = FALSE, do_stun, do_confuse, result;
    int attack_indx, scare_dieroll = MB_MAX_DIEROLL / 2;

    result = FALSE; /* no message given yet */
    /* the most severe effects are less likely at higher enchantment */
    if (mb->enchantment >= 3)
        scare_dieroll /= (1 << (mb->enchantment / 3));
    /* if target successfully resisted the artifact damage bonus,
       reduce overall likelihood of the assorted special effects */
    if (!spec_dbon_applies)
        dieroll += 1;

    /* might stun even when attempting a more severe effect, but
       in that case it will only happen if the other effect fails;
       extra damage will apply regardless; 3.4.1: sometimes might
       just probe even when it hasn't been enchanted */
    do_stun = !Stun_resistance && (max(mb->enchantment, 0) < rn2(spec_dbon_applies ? 11 : 7));

    /* the special effects also boost physical damage; increments are
       generally cumulative, but since the stun effect is based on a
       different criterium its damage might not be included; the base
       damage is either 1d4 (athame) or 2d4 (athame+spec_dbon) depending
       on target's resistance check against AD_STUN (handled by caller)
       [note that a successful save against AD_STUN doesn't actually
       prevent the target from ending up stunned] */
    attack_indx = MB_INDEX_PROBE;
    *dmgptr += adjust_damage(rnd(4), magr, mdef, objects[mb->otyp].oc_damagetype, ADFLAGS_NONE); /* (2..3)d4 */
    if (do_stun) {
        attack_indx = MB_INDEX_STUN;
        *dmgptr += adjust_damage(rnd(4), magr, mdef, objects[mb->otyp].oc_damagetype, ADFLAGS_NONE); /* (3..4)d4 */
    }
    if (dieroll <= scare_dieroll) {
        attack_indx = MB_INDEX_SCARE;
        *dmgptr += adjust_damage(rnd(4), magr, mdef, objects[mb->otyp].oc_damagetype, ADFLAGS_NONE); /* (3..5)d4 */
    }
    if (dieroll <= (scare_dieroll / 2)) {
        attack_indx = MB_INDEX_CANCEL;
        *dmgptr += adjust_damage(rnd(4), magr, mdef, objects[mb->otyp].oc_damagetype, ADFLAGS_NONE); /* (4..6)d4 */
    }

    /* give the hit message prior to inflicting the effects */
    verb = mb_verb[!!Hallucination][attack_indx];
    if (youattack || youdefend || vis) {
        result = TRUE;
        pline_The("magic-absorbing blade %s %s!",
                  vtense((const char *) 0, verb), hittee);
        /* assume probing has some sort of noticeable feedback
           even if it is being done by one monster to another */
        if (attack_indx == MB_INDEX_PROBE && !canspotmon(mdef))
            map_invisible(mdef->mx, mdef->my);
    }

    /* now perform special effects */
    switch (attack_indx) {
    case MB_INDEX_CANCEL:
        old_uasmon = youmonst.data;
        /* No is_cancelled(mdef)check: even a cancelled monster can be polymorphed
         * into a golem, and the "cancel" effect acts as if some magical
         * energy remains in spellcasting defenders to be absorbed later.
         */
        if (!cancel_monst(mdef, mb, youattack, FALSE, FALSE, d(3,4)+5)) {
            resisted = TRUE;
        } else {
            do_stun = FALSE;
            if (youdefend) {
                if (youmonst.data != old_uasmon)
                    *dmgptr = 0; /* rehumanized, so no more damage */
                if (u.uenmax > 0) {
                    u.ubaseenmax--;
                    if (u.uen > 0)
                        u.uen--;
                    updatemaxen();
                    context.botl = TRUE;
                    You("lose magical energy!");
                }
            } else {
                if (mdef->data == &mons[PM_CLAY_GOLEM])
                    mdef->mhp = 1; /* cancelled clay golems will die */
                if (youattack && attacktype(mdef->data, AT_MAGC)) {
                    u.ubaseenmax++;
                    u.uen++;
                    updatemaxen();
                    context.botl = TRUE;
                    You("absorb magical energy!");
                }
            }
        }
        break;

    case MB_INDEX_SCARE:
        if (youdefend) {
            if (Antimagic_or_resistance || Fear_resistance) {
                resisted = TRUE;
            } else {
                nomul(-3);
                multi_reason = "being scared stiff";
                nomovemsg = "";
                if (magr && magr == u.ustuck && sticks(youmonst.data)) {
                    u.ustuck = (struct monst *) 0;
                    You("release %s!", mon_nam(magr));
                }
            }
        } else {
            if (rn2(2) && check_ability_resistance_success(mdef, A_WIS, objects[mb->otyp].oc_mc_adjustment))
                resisted = TRUE;
            else
                make_mon_fearful(mdef, 3); // monflee(mdef, 3, FALSE, (mdef->mhp > * dmgptr));
        }
        if (!resisted)
            do_stun = FALSE;
        break;

    case MB_INDEX_STUN:
        do_stun = !Stun_resistance; /* (this is redundant...) */
        break;

    case MB_INDEX_PROBE:
        if (youattack && (mb->enchantment == 0 || !rn2(3 * abs(mb->enchantment)))) {
            pline_The("%s is insightful.", verb);
            /* pre-damage status */
            probe_monster(mdef);
        }
        break;
    }
    /* stun if that was selected and a worse effect didn't occur */
    if (do_stun) {
        if (youdefend)
        {
            if (!Stunned)
                play_sfx_sound(SFX_ACQUIRE_STUN);
            make_stunned(((HStun& TIMEOUT) + 3L), FALSE);
        }
        else
        {
            if (!is_stunned(mdef))
                play_sfx_sound(SFX_ACQUIRE_STUN);

            (void)increase_mon_property_verbosely(mdef, STUNNED, 3);
        }
        /* avoid extra stun message below if we used mb_verb["stun"] above */
        if (attack_indx == MB_INDEX_STUN)
            do_stun = FALSE;
    }
    /* lastly, all this magic can be confusing... */
    do_confuse = !rn2(12);
    if (do_confuse) {
        if (youdefend)
        {
            if (!Confusion)
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            make_confused(itimeout_incr(HConfusion, 4L), FALSE);
        }
        else
        {
            if (!is_confused(mdef))
                play_sfx_sound(SFX_ACQUIRE_CONFUSION);
            (void)increase_mon_property_verbosely(mdef, CONFUSION, 4);
        }
    }

    /* now give message(s) describing side-effects;
       don't let vtense() be fooled by assigned name ending in 's' */
    fakename = youdefend ? "you" : "mon";
    if (youattack || youdefend || vis) {
        (void) upstart(hittee); /* capitalize */
        if (resisted) {
            pline("%s %s!", hittee, vtense(fakename, "resist"));
            if (youdefend)
                u_shieldeff();
            else
                m_shieldeff(mdef);
        }
        if ((do_stun || do_confuse) && flags.verbose) {
            char buf[BUFSZ];

            buf[0] = '\0';
            if (do_stun)
                Strcat(buf, "stunned");
            if (do_stun && do_confuse)
                Strcat(buf, " and ");
            if (do_confuse)
                Strcat(buf, "confused");
            pline("%s %s %s%c", hittee, vtense(fakename, "are"), buf,
                  (do_stun && do_confuse) ? '!' : '.');
        }
    }

    return result;
}

/* Function used when someone attacks someone else with an artifact
 * weapon.  Only adds the special (artifact) damage, and returns a 1 if it
 * did something special (in which case the caller won't print the normal
 * hit message).  This should be called once upon every artifact attack;
 * weapon_dmg_value() no longer takes artifact bonuses into account.  Possible
 * extension: change the killer so that when an orc kills you with
 * Stormbringer it's "killed by Stormbringer" instead of "killed by an orc".
 */
int
artifact_hit(magr, mdef, otmp, dmgptr, instakillptr, dieroll)
struct monst *magr, *mdef;
struct obj *otmp;
double *dmgptr;
boolean* instakillptr;
int dieroll; /* needed for Magicbane and vorpal blades */
{
    boolean youattack = (magr == &youmonst);
    boolean youdefend = (mdef == &youmonst);
    boolean vis = (!youattack && magr && cansee(magr->mx, magr->my))
                  || (!youdefend && cansee(mdef->mx, mdef->my))
                  || (youattack && u.uswallow && mdef == u.ustuck && !Blind);
    boolean realizes_damage;
    char wepdesc[BUFSIZ] ="";
    static const char you[] = "you";
    char hittee[BUFSZ];

    Strcpy(hittee, youdefend ? you : mon_nam(mdef));

    /* The following takes care of most of the damage, but not all--
     * the exception being for level draining, which is specially
     * handled.  Messages are done in this function, however.
     */
    *dmgptr += spec_dbon(otmp, mdef, *dmgptr);

    if (youattack && youdefend)
    {
        impossible("attacking yourself with weapon?");
        return FALSE;
    }

    realizes_damage = (youdefend || vis
                       /* feel the effect even if not seen */
                       || (youattack && mdef == u.ustuck));

    char artifact_hit_desc[BUFSZ] = "";
    if(otmp->oartifact && artilist[otmp->oartifact].hit_desc && strcmp(artilist[otmp->oartifact].hit_desc, ""))
        Strcpy(artifact_hit_desc, artilist[otmp->oartifact].hit_desc);
    else
        Strcpy(artifact_hit_desc, cxname(otmp));

    /* the four basic attacks: fire, cold, shock and missiles */
    if (artifact_attack_type(AD_FIRE, otmp)) 
    {
        if (realizes_damage)
            pline_The("%s %s %s%c", artifact_hit_desc,
                      !spec_dbon_applies
                          ? "hits"
                          : (is_watery(mdef->data))
                                ? "vaporizes part of"
                                : "burns",
                      hittee, !spec_dbon_applies ? '.' : '!');
        if (!rn2(4))
            (void) destroy_mitem(mdef, POTION_CLASS, AD_FIRE);
        if (!rn2(4))
            (void) destroy_mitem(mdef, SCROLL_CLASS, AD_FIRE);
        if (!rn2(7))
            (void) destroy_mitem(mdef, SPBOOK_CLASS, AD_FIRE);
        if (youdefend && Slimed)
            burn_away_slime();
        return realizes_damage;
    }
    if (artifact_attack_type(AD_COLD, otmp)) 
    {
        if (realizes_damage)
            pline_The("%s %s %s%c", artifact_hit_desc,
                      !spec_dbon_applies ? "hits" : "freezes", hittee,
                      !spec_dbon_applies ? '.' : '!');
        if (!rn2(4))
            (void) destroy_mitem(mdef, POTION_CLASS, AD_COLD);
        return realizes_damage;
    }
    if (artifact_attack_type(AD_ELEC, otmp))
    {
        if (spec_dbon_applies)
            play_sfx_sound_at_location(SFX_LIGHTNING_STRIKES, mdef->mx, mdef->my);

        if (realizes_damage)
            pline_The("%s hits%s %s%c", artifact_hit_desc,
                      !spec_dbon_applies ? "" : "!  Lightning strikes",
                      hittee, !spec_dbon_applies ? '.' : '!');

        if (spec_dbon_applies)
            wake_nearto(mdef->mx, mdef->my, 4 * 4);

        if (!rn2(5))
            (void) destroy_mitem(mdef, RING_CLASS, AD_ELEC);
        if (!rn2(5))
            (void) destroy_mitem(mdef, WAND_CLASS, AD_ELEC);
        return realizes_damage;
    }
    if (artifact_attack_type(AD_MAGM, otmp))
    {
        if (realizes_damage)
            pline_The("%s hits%s %s%c", artifact_hit_desc,
                      !spec_dbon_applies
                          ? ""
                          : "!  A hail of magic missiles strikes",
                      hittee, !spec_dbon_applies ? '.' : '!');
        return realizes_damage;
    }

    if (artifact_attack_type(AD_STUN, otmp) && dieroll <= MB_MAX_DIEROLL) 
    {
        /* Magicbane's special attacks (possibly modifies hittee[]) */
        return Mb_hit(magr, mdef, otmp, dmgptr, dieroll, vis, hittee);
    }

    if (!spec_dbon_applies) 
    {
        /* since damage bonus didn't apply, nothing more to do;
           no further attacks have side-effects on inventory */
        return FALSE;
    }

    /* We really want "on a natural 20" but GnollHack does it in */
    /* reverse from AD&D. */
    if (artifact_has_flag(otmp, (AF_BEHEAD | AF_BISECT)))
    {
        if (artifact_has_flag(otmp, AF_BISECT) && dieroll == 1 && (!is_shade(mdef->data) || shade_glare(otmp)) && !(youdefend ? (Bisection_resistance || Invulnerable || is_incorporeal(mdef->data)) : resists_bisection(mdef)))
        {
            strcpy(wepdesc, The(artifact_hit_desc));
            /* not really beheading, but close */
            if (youattack && u.uswallow && mdef == u.ustuck) 
            {
                You("slice %s wide open!", mon_nam(mdef));
                //*dmgptr = 2 * (double)mdef->mhp + FATAL_DAMAGE_MODIFIER;
                //mdef->mhp = 0;
                *instakillptr = TRUE;
                return 2;
            }
            if (!youdefend) 
            {
                /* allow normal cutworm() call to add extra damage */
                if (notonhead)
                    return FALSE;

                if (bigmonst(mdef->data)) 
                {
                    if (youattack)
                        You("slice deeply into %s!", mon_nam(mdef));
                    else if (vis)
                        pline("%s cuts deeply into %s!", Monnam(magr),
                              hittee);
                    *dmgptr *= 2;
                    return 2;
                }
                //*dmgptr = 2 * (double)mdef->mhp + FATAL_DAMAGE_MODIFIER;
                //mdef->mhp = 0;
                *instakillptr = TRUE;
                pline("%s cuts %s in half!", wepdesc, mon_nam(mdef));
                otmp->dknown = TRUE;
                return 2;
            } 
            else
            {
                if (bigmonst(youmonst.data))
                {
                    pline("%s cuts deeply into you!",
                          magr ? Monnam(magr) : wepdesc);
                    *dmgptr *= 2;
                    return 2;
                }

                /* Players with negative AC's take less damage instead
                 * of just not getting hit.  We must add a large enough
                 * value to the damage so that this reduction in
                 * damage does not prevent death.
                 */
                //*dmgptr = 2 * (double)(Upolyd ? u.mh : u.uhp) + FATAL_DAMAGE_MODIFIER;
                //if (Upolyd)
                //    u.mh = 0;
                //else
                //    u.uhp = 0;
                *instakillptr = TRUE;
                pline("%s cuts you in half!", wepdesc);
                otmp->dknown = TRUE;
                return 2;
            }
        } 
        else if (artifact_has_flag(otmp, AF_BEHEAD)
                   && (dieroll == 1 || has_vorpal_vulnerability(mdef->data))) 
        {
            static const char *const behead_msg[2] = { "%s beheads %s!",
                                                       "%s decapitates %s!" };

            if (youattack && u.uswallow && mdef == u.ustuck)
                return FALSE;
            strcpy(wepdesc, artifact_hit_desc);
            if (!youdefend)
            {
                if (!has_neck(mdef->data) || notonhead || u.uswallow || mdef->heads_left == 0)
                {
                    if (youattack)
                        pline("Somehow, you miss %s wildly.", mon_nam(mdef));
                    else if (vis)
                        pline("Somehow, %s misses wildly.", mon_nam(magr));
                    *dmgptr = 0;
                    return (youattack || vis) * 2;
                }
                if (is_incorporeal(mdef->data) || amorphous(mdef->data) || (is_shade(mdef->data) && !shade_glare(otmp)))
                {
                    pline("%s slices through %s %s.", The(wepdesc),
                          s_suffix(mon_nam(mdef)), mbodypart(mdef, NECK));
                    return 2;
                }
                if (mdef->heads_left > 1)
                {
                    mdef->heads_left--;
                    *dmgptr += 0.625 * (double)mdef->mhpmax / (double)max(1, mdef->data->heads); //Adjusted based on Tiamat in AD&D
                    pline("%s cuts one of %s heads off!", The(wepdesc),
                        s_suffix(mon_nam(mdef)));
                    otmp->dknown = TRUE;
                    return 1;
                }
                else
                {
                    if (mdef->heads_left > 0)
                        mdef->heads_left--;
                    //*dmgptr = 2 * (double)mdef->mhp + FATAL_DAMAGE_MODIFIER;
                    //mdef->mhp = 0;
                    *instakillptr = TRUE;

                    if(mdef->data->heads <= 1)
                        pline(behead_msg[rn2(SIZE(behead_msg))], The(wepdesc),mon_nam(mdef));
                    else
                        pline("%s cuts off %s last head!", The(wepdesc), s_suffix(mon_nam(mdef)));

                    if (Hallucination && !flags.female)
                        pline("Good job Henry, but that wasn't Anne.");
                    otmp->dknown = TRUE;
                    return 2;
                }
            }
            else
            {
                if (!has_neck(youmonst.data) || mdef->heads_left == 0) {
                    pline("Somehow, %s misses you wildly.",
                          magr ? mon_nam(magr) : the(wepdesc));
                    *dmgptr = 0;
                    return 2;
                }
                if (is_incorporeal(youmonst.data) || amorphous(youmonst.data) || (is_shade(youmonst.data) && !shade_glare(otmp)))
                {
                    pline("%s slices through your %s.", The(wepdesc),
                          body_part(NECK));
                    return 2;
                }

                if (mdef->heads_left > 1)
                {
                    mdef->heads_left--;
                    *dmgptr += 0.625 * (double)(Upolyd ? u.mh : u.uhp) / (double)max(1, mdef->data->heads); //Adjusted based on Tiamat in AD&D
                    pline("%s cuts one of your %s off!", The(wepdesc), makeplural(body_part(HEAD)));
                    otmp->dknown = TRUE;
                    return 1;
                }
                else
                {
                    if (mdef->heads_left > 0)
                        mdef->heads_left--;
                    //*dmgptr = 2 * (double)(Upolyd ? u.mh : u.uhp) + FATAL_DAMAGE_MODIFIER;
                    //if (Upolyd)
                    //    u.mh = 0;
                    //else
                    //    u.uhp = 0;
                    *instakillptr = TRUE;

                    if (mdef->data->heads <= 1)
                        pline(behead_msg[rn2(SIZE(behead_msg))], The(wepdesc), "you");
                    else
                        pline("%s cuts off %s last %s!", The(wepdesc), "your", body_part(HEAD));

                    otmp->dknown = TRUE;
                    /* Should amulets fall off? */
                    return 2;
                }
            }
        }
    }
    if (artifact_has_flag(otmp, AF_DRLI) && !(youdefend ? Drain_resistance : resists_drli(mdef)))
    {
        /* some non-living creatures (golems, vortices) are
           vulnerable to life drain effects */
        const char *life = is_not_living(mdef->data) ? "animating force" : "life";

        if (!youdefend)
        {
            if (vis)
            {
                if (otmp->oartifact == ART_STORMBRINGER || otmp->oartifact == ART_MOURNBLADE)
                    pline_The("%s draws the %s from %s!",
                        artifact_hit_desc, life, mon_nam(mdef));
                else
                    pline("%s draws the %s from %s!",
                        The(distant_name(otmp, xname)), life,
                        mon_nam(mdef));
            }

            int instakilllevel = obj_has_dual_runesword_bonus(otmp) ? 1 : 0;
            if (mdef->m_lev <= instakilllevel)
            {
                //*dmgptr = 2 * (double)mdef->mhp + FATAL_DAMAGE_MODIFIER;
                //mdef->mhp = 0;
                *instakillptr = TRUE;
            }
            else
            {
                int drain = monbasehp_per_lvl(mdef);
                double mhpadj = monhpadj_per_lvl(mdef);
                int levelloss = 1;
                if (youattack ? obj_has_dual_runesword_bonus(otmp) : monwep_has_dual_runesword_bonus(magr, otmp))
                {
                    drain *= 2;
                    mhpadj *= 2.0;
                    levelloss *= 2;
                }

                *dmgptr += (double)drain + mhpadj;
                mdef->mbasehpmax -= drain;
                mdef->m_lev -= levelloss;
                update_mon_maxhp(mdef);
                drain /= 2;
                if (drain)
                {
                    if (youattack)
                        healup(drain, 0, FALSE, FALSE, FALSE, FALSE, FALSE);
                    else
                        deduct_monster_hp(magr, -drain);
                }
            }
            return vis;
        }
        else
        { /* youdefend */
            int oldhpmax = u.uhpmax;

            if (Blind)
                You_feel("an %s drain your %s!",
                    (otmp->oartifact == ART_STORMBRINGER || otmp->oartifact == ART_MOURNBLADE)
                    ? "unholy blade"
                    : "object",
                    life);
            else if (otmp->oartifact == ART_STORMBRINGER || otmp->oartifact == ART_MOURNBLADE)
                pline_The("%s blade drains your %s!", hcolor(NH_BLACK), life);
            else
                pline("%s drains your %s!", The(distant_name(otmp, xname)),
                    life);
            losexp("life drainage");
            if(monwep_has_dual_runesword_bonus(magr, otmp))
                losexp("life drainage"); /* Lose another level */

            if (magr && magr->mhp < magr->mhpmax)
            {
                magr->mhp += (oldhpmax - u.uhpmax) / 2;
                if (magr->mhp > magr->mhpmax)
                    magr->mhp = magr->mhpmax;
            }
            return TRUE;
        }
    }
    return FALSE;
}

/* Function used when someone attacks someone else with an artifact-like
 * weapon.  
 * Returns extra damage caused, to be added to damage caused by caller (so it can be displayed correctly), if any caused; -1 means that the caller has to kill mdef
 */
int
pseudo_artifact_hit(magr, mdef, otmp, extradmg, dieroll, critstrikeroll, adtyp_ptr)
struct monst* magr, * mdef;
struct obj* otmp;
int extradmg;
int dieroll; /* needed for Magicbane and vorpal blades */
int critstrikeroll; /* need to synchronize critical strike based abilities */
short* adtyp_ptr; /* return value is the type of damage caused */
{
    if (!otmp || !magr || !mdef)
        return 0;

    if (object_uses_spellbook_wand_flags_and_properties(otmp))
        return 0;

    boolean youattack = (magr == &youmonst);
    boolean youdefend = (mdef == &youmonst);
    boolean vis = (!youattack && magr && cansee(magr->mx, magr->my))
        || (!youdefend && cansee(mdef->mx, mdef->my))
        || (youattack && u.uswallow && mdef == u.ustuck && !Blind);
    boolean realizes_damage;
    boolean extradamagedone = (extradmg > 0);
    static const char you[] = "you";
    char hittee[BUFSZ];
    int totaldamagedone = 0;
    boolean lethaldamage = FALSE;
    boolean isdisintegrated = FALSE;
    int criticalstrikeroll = critstrikeroll;

    *adtyp_ptr = objects[otmp->otyp].oc_damagetype;

    Strcpy(hittee, youdefend ? you : mon_nam(mdef));

    /* The following takes care of most of the damage, but not all--
     * the exception being for level draining, which is specially
     * handled.  Messages are done in this function, however.
     */

    if (youattack && youdefend) 
    {
        impossible("attacking yourself with weapon?");
        return 0;
    }

    realizes_damage = (youdefend || vis
        /* feel the effect even if not seen */
        || (youattack && mdef == u.ustuck));

    /* the four basic attacks: fire, cold, shock and missiles will implemented elsewhere */
    if (!(youdefend ? Fire_immunity : is_mon_immune_to_fire(mdef)) && (objects[otmp->otyp].oc_damagetype == AD_FIRE || (extradamagedone && objects[otmp->otyp].oc_extra_damagetype == AD_FIRE)))
    {
        if (realizes_damage)
            pline("%s %s %s%c",
                The(xname(otmp)),
                (is_watery(mdef->data))
                ? "vaporizes part of"
                : "burns",
                hittee, '!');
        if (!rn2(4))
            (void)destroy_mitem(mdef, POTION_CLASS, AD_FIRE);
        if (!rn2(4))
            (void)destroy_mitem(mdef, SCROLL_CLASS, AD_FIRE);
        if (!rn2(7))
            (void)destroy_mitem(mdef, SPBOOK_CLASS, AD_FIRE);
        if (youdefend && Slimed)
            burn_away_slime();
    }
    if (!(youdefend ? Cold_immunity : is_mon_immune_to_cold(mdef)) && (objects[otmp->otyp].oc_damagetype == AD_COLD || (extradamagedone && objects[otmp->otyp].oc_extra_damagetype == AD_COLD)))
    {
        if (realizes_damage)
            pline("%s %s %s%c", The(xname(otmp)),
                "freezes", hittee,
                '!');
        if (!rn2(4))
            (void)destroy_mitem(mdef, POTION_CLASS, AD_COLD);
    }
    if (!(youdefend ? Shock_immunity : is_mon_immune_to_elec(mdef)) && (objects[otmp->otyp].oc_damagetype == AD_ELEC || (extradamagedone && objects[otmp->otyp].oc_extra_damagetype == AD_ELEC)))
    {
        if (realizes_damage)
            pline("The electrical energies of %s jolt %s%c", the(xname(otmp)),
                hittee, '!');
        wake_nearto(mdef->mx, mdef->my, 4 * 4);
        if (!rn2(5))
            (void)destroy_mitem(mdef, RING_CLASS, AD_ELEC);
        if (!rn2(5))
            (void)destroy_mitem(mdef, WAND_CLASS, AD_ELEC);
    }

    if (!check_magic_resistance_and_inflict_damage(mdef, (struct obj*)0, (struct monst*)0, FALSE, 0, 0, NOTELL) && (objects[otmp->otyp].oc_damagetype == AD_MAGM || (extradamagedone && objects[otmp->otyp].oc_extra_damagetype == AD_MAGM)))
    {
        if (realizes_damage)
            pline("A hail of magic missiles strikes from %s hits %s!", the(xname(otmp)), hittee);
    }

    if ((objects[otmp->otyp].oc_damagetype == AD_STUN || (extradamagedone && objects[otmp->otyp].oc_extra_damagetype == AD_STUN)) && dieroll <= MB_MAX_DIEROLL)
    {
        /* Magicbane's special attacks (possibly modifies hittee[]) */
        //(void) Mb_hit(magr, mdef, otmp, &totaldamagedone, dieroll, vis, hittee);
    }

    int crit_strike_probability = get_critical_strike_percentage_chance(otmp, mdef, magr);
    int crit_strike_die_roll_threshold = crit_strike_probability / 5;
    boolean abilityison = (objects[otmp->otyp].oc_aflags2 & A2_REQUIRES_ARTIFACT_INVOKE_ON) != 0 ? (otmp->invokeon || otmp->invokeleft) : TRUE;
    if (abilityison)
    {
        /* We really want "on a natural 20" but GnollHack does it in */
        /* reverse from AD&D. */
        if ((objects[otmp->otyp].oc_aflags & A1_SVB_MASK) == A1_BISECT && !(youdefend ? Bisection_resistance : has_bisection_resistance(mdef)))
        {
            if (
                ((objects[otmp->otyp].oc_aflags & A1_VORPAL_LIKE_DISRESPECTS_TARGETS) || eligible_for_extra_damage(otmp, mdef, magr))
                && ((objects[otmp->otyp].oc_aflags & A1_VORPAL_LIKE_DISRESPECTS_CHARACTERS) || !inappropriate_monster_character_type(magr, otmp))
                && (
                    ((objects[otmp->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && (
                            ((objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && dieroll <= crit_strike_die_roll_threshold)
                            ||
                            (!(objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && criticalstrikeroll < crit_strike_probability))
                        )
                    ||
                    (!(objects[otmp->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && dieroll <= 1)
                    )
                )
            {
                if (youattack && u.uswallow && mdef == u.ustuck) {
                    You("slice %s wide open!", mon_nam(mdef));
                    lethaldamage = TRUE;
                }
                else if (!youdefend)
                {
                    if (is_incorporeal(mdef->data) || amorphous(mdef->data)) {
                        pline("%s through %s body.", Yobjnam2(otmp, "cut"),
                            s_suffix(mon_nam(mdef)));
                    }
                    else if (notonhead)
                        ;
                    else if (bigmonst(mdef->data))
                    {
                        int damagedone = mdef->mhpmax / 2;
                        if (damagedone < 1)
                            damagedone = 1;

                        totaldamagedone += damagedone;

                        if (!does_regenerate_bodyparts(mdef->data))
                        {
                            /* Max HP does not go down if the creature can regenerate the lost body part */
                            mdef->mbasehpmax -= damagedone;
                            mdef->mhpmax -= damagedone;
                            if (mdef->mhpmax < 1)
                                mdef->mhpmax = 1, lethaldamage = TRUE;
                            else
                                update_mon_maxhp(mdef);
                        }
                        pline("%s slices a part of %s off!", The(xname(otmp)),
                            mon_nam(mdef));
                        if (Hallucination && !lethaldamage)
                        {
                            pline("But %s retorts:", mon_nam(mdef));
                            if (rn2(2))
                                verbalize("Hah! It's just a scratch.");
                            else
                                verbalize("Hah! It's just a flesh wound.");
                        }
                        otmp->dknown = TRUE;
                    }
                    else
                    {
                        pline("%s cuts %s in half!", The(xname(otmp)), mon_nam(mdef));
                        otmp->dknown = TRUE;
                        lethaldamage = TRUE;
                    }
                }
                else
                {
                    if (is_incorporeal(youmonst.data) || amorphous(youmonst.data)) {
                        pline("%s slices through your body.", The(xname(otmp)));
                    }
                    else if (bigmonst(youmonst.data))
                    {
                        if (Upolyd)
                        {
                            int damagedone = u.mhmax / 2;
                            if (damagedone < 1)
                                damagedone = 1;

                            totaldamagedone += damagedone;

                            if (!does_regenerate_bodyparts(youmonst.data))
                            {
                                /* Max HP does not go down if the creature can regenerate the lost body part */
                                u.basemhmax -= damagedone;
                                u.mhmax -= damagedone;
                                if (u.mhmax < 1)
                                    u.mhmax = 1, lethaldamage = TRUE;
                            }
                        }
                        else
                        {
                            int damagedone = u.uhpmax / 2;
                            if (damagedone < 1)
                                damagedone = 1;

                            totaldamagedone += damagedone;

                            if (!does_regenerate_bodyparts(youmonst.data))
                            {
                                /* Max HP does not go down if the creature can regenerate the lost body part */
                                u.ubasehpmax -= damagedone;
                                u.uhpmax -= damagedone;
                                if (u.uhpmax < 1)
                                    u.uhpmax = 1, lethaldamage = TRUE;
                            }

                        }
                        pline("%s slices a part of %s off!", The(xname(otmp)), "you");
                        otmp->dknown = TRUE;
                    }
                    else
                    {
                        /* Players with negative AC's take less damage instead
                         * of just not getting hit.  We must add a large enough
                         * value to the damage so that this reduction in
                         * damage does not prevent death.
                         */
                        pline("%s cuts you in half!", The(xname(otmp)));
                        otmp->dknown = TRUE;
                        lethaldamage = TRUE;
                    }
                }
            }
        }
        else if ((objects[otmp->otyp].oc_aflags & A1_SVB_MASK) == A1_SHARPNESS || has_obj_mythic_sharpness(otmp))
        {
            if (has_obj_mythic_sharpness(otmp) ||
                (
                    ((objects[otmp->otyp].oc_aflags & A1_VORPAL_LIKE_DISRESPECTS_TARGETS) || eligible_for_extra_damage(otmp, mdef, magr))
                    && ((objects[otmp->otyp].oc_aflags & A1_VORPAL_LIKE_DISRESPECTS_CHARACTERS) || !inappropriate_monster_character_type(magr, otmp))
                    && (
                        ((objects[otmp->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                            && (
                                ((objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                    && dieroll <= crit_strike_die_roll_threshold)
                                ||
                                (!(objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                    && criticalstrikeroll < crit_strike_probability))
                            )
                        ||
                        (!(objects[otmp->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                            && dieroll <= SHARPNESS_DIE_ROLL_CHANCE)
                        )
                    )
                )
            {
                if (!youdefend)
                {
                    if (is_incorporeal(mdef->data) || amorphous(mdef->data)) {
                        pline("%s through %s %s.", Yobjnam2(otmp, "slice"),
                            s_suffix(mon_nam(mdef)), mbodypart(mdef, NECK));
                    }
                    else
                    {
                        int damagedone = (mdef->mhpmax * SHARPNESS_MAX_HP_PERCENTAGE_DAMAGE) / 100;
                        if (damagedone < 1)
                            damagedone = 1;

                        totaldamagedone += damagedone;

                        pline("%s slices a part of %s off!", The(xname(otmp)),
                            mon_nam(mdef));
                        if (Hallucination && !lethaldamage)
                        {
                            pline("But %s retorts:", mon_nam(mdef));
                            if (rn2(2))
                                verbalize("Hah! It's just a scratch.");
                            else
                                verbalize("Hah! It's just a flesh wound.");
                        }
                        otmp->dknown = TRUE;
                    }
                }
                else
                {
                    if (is_incorporeal(youmonst.data) || amorphous(youmonst.data)) {
                        pline("%s slices through your %s.", The(xname(otmp)),
                            body_part(NECK));
                    }
                    else
                    {
                        if (Upolyd)
                        {
                            int damagedone = (u.mhmax * SHARPNESS_MAX_HP_PERCENTAGE_DAMAGE) / 100;
                            if (damagedone < 1)
                                damagedone = 1;

                            totaldamagedone += damagedone;
                        }
                        else
                        {
                            int damagedone = (u.uhpmax * SHARPNESS_MAX_HP_PERCENTAGE_DAMAGE) / 100;
                            if (damagedone < 1)
                                damagedone = 1;

                            totaldamagedone += damagedone;
                        }
                        pline("%s slices a part of %s off!", The(xname(otmp)), "you");
                        otmp->dknown = TRUE;
                    }
                }
            }
        }
        else if ((objects[otmp->otyp].oc_aflags & A1_SVB_MASK) == A1_VORPAL)
        {
            if (
                ((objects[otmp->otyp].oc_aflags & A1_VORPAL_LIKE_DISRESPECTS_TARGETS) || eligible_for_extra_damage(otmp, mdef, magr))
                && ((objects[otmp->otyp].oc_aflags & A1_VORPAL_LIKE_DISRESPECTS_CHARACTERS) || !inappropriate_monster_character_type(magr, otmp))
                && (
                    ((objects[otmp->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && (
                            ((objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && (dieroll <= crit_strike_die_roll_threshold || has_vorpal_vulnerability(mdef->data)))
                            ||
                            (!(objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                && (criticalstrikeroll < crit_strike_probability || has_vorpal_vulnerability(mdef->data))))
                        )
                    ||
                    (!(objects[otmp->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                        && (dieroll <= 1 || has_vorpal_vulnerability(mdef->data)))
                    )
                )
            {
                static const char* const behead_msg[3] = { "%s beheads %s!",
                                                            "%s decapitates %s!",
                                                            "%s cuts off the last head of %s!" };

                if (youattack && u.uswallow && mdef == u.ustuck)
                    ;
                else if (!youdefend) {
                    if (!has_neck(mdef->data) || notonhead || u.uswallow) {
                        if (youattack)
                            pline("Somehow, you miss %s wildly.", mon_nam(mdef));
                        else if (vis)
                            pline("Somehow, %s misses wildly.", mon_nam(magr));
                    }
                    else if (is_incorporeal(mdef->data) || amorphous(mdef->data)) {
                        pline("%s through %s %s.", Yobjnam2(otmp, "slice"),
                            s_suffix(mon_nam(mdef)), mbodypart(mdef, NECK));
                    }
                    else
                    {
                        if (mdef->heads_left > 1)
                        {
                            mdef->heads_left--;
                            totaldamagedone += (int)(0.625 * (double)mdef->mhpmax / (double)max(1, mdef->data->heads)); //Adjusted based on Tiamat in AD&D
                            pline("%s cuts one of %s heads off!", The(xname(otmp)), s_suffix(mon_nam(mdef)));
                            otmp->dknown = TRUE;
                        }
                        else
                        {
                            if (mdef->heads_left > 0)
                                mdef->heads_left--;

                            pline(behead_msg[rn2(SIZE(behead_msg))], The(xname(otmp)),
                                mon_nam(mdef));
                            if (Hallucination && !flags.female)
                                pline("Good job Henry, but that wasn't Anne.");
                            otmp->dknown = TRUE;
                            lethaldamage = TRUE;
                        }
                    }
                }
                else
                {
                    if (!has_neck(youmonst.data))
                    {
                        pline("Somehow, %s misses you wildly.", (magr ? mon_nam(magr) : the(xname(otmp))));
                    }
                    else if (is_incorporeal(youmonst.data) || amorphous(youmonst.data)) {
                        pline("%s slices through your %s.", The(xname(otmp)),
                            body_part(NECK));
                    }
                    else
                    {
                        if (mdef->heads_left > 1)
                        {
                            mdef->heads_left--;
                            totaldamagedone += (int)(0.625 * (double)(Upolyd ? u.mhmax : u.uhpmax) / (double)max(1, mdef->data->heads)); //Adjusted based on Tiamat in AD&D
                            pline("%s cuts one of your %s off!", The(xname(otmp)), makeplural(body_part(HEAD)));
                            otmp->dknown = TRUE;
                        }
                        else
                        {
                            if (mdef->heads_left > 0)
                                mdef->heads_left--;

                            pline(behead_msg[rn2(SIZE(behead_msg))], The(xname(otmp)), "you");
                            otmp->dknown = TRUE;
                            lethaldamage = TRUE;
                        }
                    }
                }
            }
        }


        if ((objects[otmp->otyp].oc_aflags & A1_LEVEL_DRAIN) || has_obj_mythic_level_drain(otmp))
        {
            if (!is_rider(mdef->data) && !is_undead(mdef->data) //Demons are affected
                && !(youdefend ? Drain_resistance : resists_drli(mdef)))
            {
                if (has_obj_mythic_level_drain(otmp) ||
                    (!((objects[otmp->otyp].oc_aflags & A1_MAGIC_RESISTANCE_PROTECTS) ? check_magic_resistance_and_inflict_damage(mdef, (struct obj*)0, (struct monst*)0, FALSE, 0, 0, NOTELL) : 0)
                        && ((objects[otmp->otyp].oc_aflags & A1_LEVEL_DRAIN_DISRESPECTS_TARGETS) || eligible_for_extra_damage(otmp, mdef, magr))
                        && ((objects[otmp->otyp].oc_aflags & A1_LEVEL_DRAIN_DISRESPECTS_CHARACTERS) || !inappropriate_monster_character_type(magr, otmp))
                        && (
                            ((objects[otmp->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                                && (
                                    ((objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                        && dieroll <= crit_strike_die_roll_threshold)
                                    ||
                                    (!(objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                                        && criticalstrikeroll < crit_strike_probability))
                                )
                            ||
                            (!(objects[otmp->otyp].oc_aflags & A1_USE_CRITICAL_STRIKE_PERCENTAGE_FOR_SPECIAL_ATTACK_TYPES)
                                && 1)
                            )
                        && ((objects[otmp->otyp].oc_aflags & A1_BYPASSES_MC) || !check_magic_cancellation_success(mdef,
                            objects[otmp->otyp].oc_mc_adjustment + (objects[otmp->otyp].oc_flags & O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT ? -otmp->enchantment : 0)))
                        )
                    )
                {
                    /* some non-living creatures (golems, vortices) are
                       vulnerable to life drain effects */
                    const char* life = is_not_living(mdef->data) ? "animating force" : "life energy";

                    if (!youdefend)
                    {
                        if (vis)
                        {
                            pline("%s draws the %s from %s!",
                                The(distant_name(otmp, xname)), life,
                                mon_nam(mdef));
                        }

                        if (mdef->m_lev == 0)
                        {
                            lethaldamage = TRUE;
                        }
                        else
                        {
                            *adtyp_ptr = AD_DRLI;
                            int drain = monbasehp_per_lvl(mdef);
                            int drain2 = (int)monhpadj_per_lvl(mdef);
                            totaldamagedone += drain + drain2;
                            mdef->mbasehpmax -= drain;
                            mdef->mhpmax -= (drain + drain2);
                            if (mdef->mhpmax < 1)
                                mdef->mhpmax = 1, lethaldamage = TRUE;
                            mdef->m_lev--;
                            if (!lethaldamage)
                                update_mon_maxhp(mdef);
                            /* non-artifact level drain does not heal */
                        }
                    }
                    else
                    { /* youdefend */
                        if (Blind)
                            You_feel("an %s drain your %s!",
                                "object",
                                life);
                        else
                            pline("%s drains your %s!", The(distant_name(otmp, xname)),
                                life);
                        losexp("life drainage");
                    }
                }
            }
        }

        if ((objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE) && (objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_IS_DEADLY))
        {
            if (
                ((objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_DISRESPECTS_TARGETS) || eligible_for_extra_damage(otmp, mdef, magr))
                && ((objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_DISRESPECTS_CHARACTERS) || !inappropriate_monster_character_type(magr, otmp))
                )
            {
                if (
                    (
                        ((objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                            && dieroll <= crit_strike_die_roll_threshold)
                        ||
                        (!(objects[otmp->otyp].oc_aflags & A1_CRITICAL_STRIKE_PERCENTAGE_IS_A_DIE_ROLL)
                            && criticalstrikeroll < crit_strike_probability)
                        )
                    && (!(objects[otmp->otyp].oc_aflags2 & A2_REQUIRES_AND_EXPENDS_A_CHARGE) || ((objects[otmp->otyp].oc_aflags2 & A2_REQUIRES_AND_EXPENDS_A_CHARGE) && otmp->charges > 0))
                    )
                {
                    boolean effectsuccessful = FALSE;
                    if (
                        ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_USES_EXTRA_DAMAGE_TYPE
                            && ((objects[otmp->otyp].oc_extra_damagetype == AD_FIRE && (youdefend ? Fire_immunity : is_mon_immune_to_fire(mdef)))
                                || ((objects[otmp->otyp].oc_aflags & A1_MAGIC_RESISTANCE_PROTECTS) ? check_magic_resistance_and_inflict_damage(mdef, (struct obj*)0, (struct monst*)0, FALSE, 0, 0, NOTELL) : 0)
                                || (objects[otmp->otyp].oc_extra_damagetype == AD_COLD && (youdefend ? Cold_immunity : is_mon_immune_to_cold(mdef)))
                                || (objects[otmp->otyp].oc_extra_damagetype == AD_ELEC && (youdefend ? Shock_immunity : is_mon_immune_to_elec(mdef)))))
                        ||
                        ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK
                            && ((youdefend ? Death_resistance : resists_death(mdef))
                                || ((objects[otmp->otyp].oc_aflags & A1_MAGIC_RESISTANCE_PROTECTS) ? check_magic_resistance_and_inflict_damage(mdef, (struct obj*)0, (struct monst*)0, FALSE, 0, 0, NOTELL) : 0)
                                || (!(objects[otmp->otyp].oc_aflags & A1_BYPASSES_MC) && check_magic_cancellation_success(mdef,
                                    objects[otmp->otyp].oc_mc_adjustment + (objects[otmp->otyp].oc_flags & O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT ? -otmp->enchantment : 0)))
                                ))
                        ||
                        ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK
                            && ((youdefend ? (Disint_resistance || Invulnerable) : resists_disint(mdef))
                                || ((objects[otmp->otyp].oc_aflags & A1_MAGIC_RESISTANCE_PROTECTS) ? check_magic_resistance_and_inflict_damage(mdef, (struct obj*)0, (struct monst*)0, FALSE, 0, 0, NOTELL) : 0)
                                || (!(objects[otmp->otyp].oc_aflags & A1_BYPASSES_MC) && check_magic_cancellation_success(mdef,
                                    objects[otmp->otyp].oc_mc_adjustment + (objects[otmp->otyp].oc_flags & O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT ? -otmp->enchantment : 0)))
                                ))
                        )
                    {
                        if (!youdefend)
                        {
                            if ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK)
                            {
                                pline("%s hits %s with death magic!", The(xname(otmp)), mon_nam(mdef));
                            }
                            else if ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)
                            {
                                pline("%s hits %s with annihilating force!", The(xname(otmp)), mon_nam(mdef));
                            }
                            else
                            {
                                pline("%s hits %s with a deadly blow!", The(xname(otmp)), mon_nam(mdef));
                            }

                            if ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK
                                && check_rider_death_absorption(mdef, The(xname(otmp))))
                            {
                                /* Death absorbed the death magics instead of being unaffected */
                                effectsuccessful = TRUE;
                            }
                            else
                            {
                                play_sfx_sound_at_location(SFX_GENERAL_UNAFFECTED, mdef->mx, mdef->my);
                                m_shieldeff(mdef);
                                pline("%s is unaffected!", Monnam(mdef));
                            }
                        }
                        else
                        {
                            if ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK)
                            {
                                pline("%s hits you with death magic!", The(xname(otmp)));
                            }
                            else if ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)
                            {
                                pline("%s hits you with annihilating force!", The(xname(otmp)));
                            }
                            else
                            {
                                pline("%s hits you with a deadly blow!", The(xname(otmp)));
                            }

                            play_sfx_sound(SFX_GENERAL_UNAFFECTED);
                            u_shieldeff();
                            You("are unaffected!");
                        }
                    }
                    else
                    {
                        if ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DISINTEGRATION_ATTACK)
                        {
                            if (!youdefend)
                            {
                                pline("%s hits %s.", The(xname(otmp)), mon_nam(mdef));
                                struct obj* otmp2 = (struct obj*)0;

                                if (resists_disint(mdef))
                                {
                                    /* should never go here */
                                    m_shieldeff(mdef);
                                }
                                else if (mdef->worn_item_flags & W_ARMS)
                                {
                                    effectsuccessful = TRUE;
                                    /* destroy shield; victim survives */
                                    if ((otmp2 = which_armor(mdef, W_ARMS)) != 0)
                                        m_useup(mdef, otmp2);
                                }
                                else if (mdef->worn_item_flags & W_ARM)
                                {
                                    effectsuccessful = TRUE;
                                    /* destroy body armor, also cloak if present */
                                    if ((otmp2 = which_armor(mdef, W_ARM)) != 0)
                                        m_useup(mdef, otmp2);
                                    if ((otmp2 = which_armor(mdef, W_ARMC)) != 0)
                                        m_useup(mdef, otmp2);
                                    if ((otmp2 = which_armor(mdef, W_ARMO)) != 0)
                                        m_useup(mdef, otmp2);
                                }
                                else if (check_rider_disintegration(mdef, (const char*)0))
                                {
                                    effectsuccessful = TRUE;
                                }
                                else
                                {
                                    effectsuccessful = TRUE;
                                    /* no body armor, victim dies; destroy cloak
                                        and shirt now in case target gets life-saved */
                                    if ((otmp2 = which_armor(mdef, W_ARMC)) != 0)
                                        m_useup(mdef, otmp2);
                                    if ((otmp2 = which_armor(mdef, W_ARMU)) != 0)
                                        m_useup(mdef, otmp2);
                                    if ((otmp2 = which_armor(mdef, W_ARMO)) != 0)
                                        m_useup(mdef, otmp2);

                                    if ((mdef->data->geno & G_UNIQ) && (objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ONE_FOURTH_MAX_HP_DAMAGE_TO_UNIQUE_MONSTERS))
                                    {
                                        totaldamagedone += mdef->mhpmax / 4;
                                    }
                                    else
                                    {
                                        lethaldamage = TRUE;
                                        isdisintegrated = TRUE;
                                    }
                                }
                            }
                            else
                            {
                                pline("%s hits you.", The(xname(otmp)));
                                if (Disint_resistance || is_incorporeal(youmonst.data) || Invulnerable
                                    || (!(objects[otmp->otyp].oc_aflags & A1_BYPASSES_MC) && check_magic_cancellation_success(mdef,
                                        objects[otmp->otyp].oc_mc_adjustment + (objects[otmp->otyp].oc_flags & O1_ENCHANTMENT_AFFECTS_MC_ADJUSTMENT ? -otmp->enchantment : 0)))
                                    )
                                {                    // if (abstyp == ZT_BREATH(ZT_DISINTEGRATION)) {
                                    You("are not disintegrated.");
                                }
                                else if (uarms)
                                {
                                    effectsuccessful = TRUE;
                                    /* destroy shield; other possessions are safe */
                                    (void)destroy_arm(uarms);
                                }
                                else if (uarm)
                                {
                                    effectsuccessful = TRUE;
                                    /* destroy suit; if present, cloak and robe go too */
                                    if (uarmc)
                                        (void)destroy_arm(uarmc);
                                    if (uarmo)
                                        (void)destroy_arm(uarmo);
                                    (void)destroy_arm(uarm);
                                }
                                else
                                {
                                    effectsuccessful = TRUE;
                                    /* no shield or suit, you're dead; wipe out cloak
                                        and/or shirt in case of life-saving or bones */
                                    if (uarmc)
                                        (void)destroy_arm(uarmc);
                                    if (uarmo)
                                        (void)destroy_arm(uarmo);
                                    if (uarmu)
                                        (void)destroy_arm(uarmu);
                                    //killer.format = KILLED_BY_AN;
                                    //Strcpy(killer.name, killer_xname(otmp));
                                    /* when killed by disintegration breath, don't leave corpse */
                                    //u.ugrave_arise = -3;
                                    //done(DIED);
                                    lethaldamage = TRUE;
                                    isdisintegrated = TRUE;
                                    //pline("You are disintegrated!");
                                }
                            }
                        }
                        else if ((objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ATTACK_TYPE_MASK) == A1_DEADLY_CRITICAL_STRIKE_IS_DEATH_ATTACK)
                        {
                            effectsuccessful = TRUE;
                            if ((mdef->data->geno & G_UNIQ) && (objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ONE_FOURTH_MAX_HP_DAMAGE_TO_UNIQUE_MONSTERS))
                            {
                                totaldamagedone += mdef->mhpmax / 4;
                            }
                            else
                            {

                                lethaldamage = TRUE;
                                if (!youdefend)
                                {
                                    if (check_rider_death_absorption(mdef, The(xname(otmp))))
                                    {
                                        lethaldamage = FALSE;
                                        totaldamagedone = 0;
                                    }
                                    else
                                    {
                                        pline("%s hits %s. The magic is deadly...", The(xname(otmp)), mon_nam(mdef));
                                    }
                                }
                                else
                                {
                                    pline("%s hits you. The magic is deadly...", The(xname(otmp)));
                                }
                            }
                        }
                        else
                        {
                            effectsuccessful = TRUE;
                            if ((mdef->data->geno & G_UNIQ) && (objects[otmp->otyp].oc_aflags & A1_DEADLY_CRITICAL_STRIKE_ONE_FOURTH_MAX_HP_DAMAGE_TO_UNIQUE_MONSTERS))
                            {
                                totaldamagedone += mdef->mhpmax / 4;
                            }
                            else
                            {
                                lethaldamage = TRUE;
                                if (!youdefend)
                                {
                                    if (is_living(mdef->data))
                                        pline("%s strikes %s dead!", The(xname(otmp)), mon_nam(mdef));
                                    else
                                        pline("%s strikes %s down!", The(xname(otmp)), mon_nam(mdef));
                                }
                                else
                                {
                                    if (is_living(mdef->data))
                                        pline("%s strikes you dead!", The(xname(otmp)));
                                    else
                                        pline("%s strikes you down!", The(xname(otmp)));
                                }
                            }
                        }
                    }

                    /* Expend a charge */
                    if (effectsuccessful && (objects[otmp->otyp].oc_aflags2 & A2_REQUIRES_AND_EXPENDS_A_CHARGE) && otmp->charges > 0)
                    {
                        consume_obj_charge(otmp, TRUE);
                    }
                }
            }
        }
    }
    return (isdisintegrated ? -2 : lethaldamage ? -1 : totaldamagedone);
}

STATIC_VAR NEARDATA const char recharge_type[] = { ALLOW_COUNT, ALL_CLASSES, 0 };
STATIC_VAR NEARDATA const char invoke_types[] = { ALL_CLASSES, 0 };
/* #invoke: an "ugly check" filters out most objects */

/* the #invoke command */
int
doinvoke()
{
    struct obj *obj;

    obj = getobj(invoke_types, "invoke", 0, "");
    if (!obj)
        return 0;

    if (obj->repowerleft > 0)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot invoke %s before its has repowered itself.", the(cxname(obj)));
        return 0;
    }

    if (!retouch_object(&obj, FALSE))
        return 1;
    return arti_invoke(obj);
}

STATIC_OVL int
arti_invoke(obj)
struct obj *obj;
{
    if (!obj) {
        impossible("arti_invoke without obj");
        return 0;
    }

    register const struct artifact *oart = get_artifact(obj);

    /* No artifact or property */
    if (!oart || !oart->inv_prop)
    {
        if (obj->otyp == CRYSTAL_BALL)
            use_crystal_ball(&obj);
        else
            pline1(nothing_happens);
        return 1;
    }

    /* Check requirements */
    if ((oart->aflags & AF_INVOKE_REQUIRES_WORN) && !is_worn_correctly(obj))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "You have to wear %s before invoking.", the(cxname(obj)));
        return 1;
    }
    else if ((oart->aflags & AF_INVOKE_EXPENDS_CHARGE) && obj->charges <= 0)
    {
        play_sfx_sound(SFX_GENERAL_OUT_OF_CHARGES);
        pline("Unfortunately, nothing happens.");
        return 1;
    }
    else if (obj->repowerleft > 0)
    {
        play_sfx_sound(SFX_NOT_READY_YET);
        /* the artifact is tired :-) */
        You_feel("that %s %s ignoring you.", the(xname(obj)),
            otense(obj, "are"));
        /* and just got more so; patience is essential... */
        if(obj->repowerleft < 9900) /* rule out unlimited addition */
            obj->repowerleft += d(3, 10);
        return 1;
    }
    else if (u.uen < artilist[obj->oartifact].inv_mana_cost)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MANA);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "You do not have enough mana to invoke %s.", the(cxname(obj)));
        return 0;
    }


    check_arti_name_discovery(obj);
    
    if (oart->aflags & AF_INVOKE_EXPENDS_CHARGE)
    {
        consume_obj_charge(obj, TRUE);
    }

    if (artilist[obj->oartifact].inv_mana_cost > 0)
    {
        u.uen -= artilist[obj->oartifact].inv_mana_cost;
        context.botl = TRUE;
    }

    if(artilist[obj->oartifact].repower_time > 0) /* Override below if effect failed */
        obj->repowerleft = artilist[obj->oartifact].repower_time;

    int art_inv_dur_dice = artilist[obj->oartifact].inv_duration_dice;
    int art_inv_dur_diesize = artilist[obj->oartifact].inv_duration_diesize;
    int art_inv_dur_plus = artilist[obj->oartifact].inv_duration_plus;
    boolean temporary_effect = ((art_inv_dur_dice > 0 && art_inv_dur_diesize > 0) || art_inv_dur_plus > 0);
    int duration = (art_inv_dur_dice > 0 && art_inv_dur_diesize > 0 ? d(art_inv_dur_dice, art_inv_dur_diesize) : 0) + art_inv_dur_plus;

    if (oart->inv_prop > LAST_PROP)
    {
        switch (oart->inv_prop) 
        {
        case ARTINVOKE_TAMING:
        {
            struct obj pseudo;
            pseudo = zeroobj; /* neither cursed nor blessed, zero oextra too */
            pseudo.otyp = SCR_TAMING;
            boolean effect_happened = 0;
            (void) seffects(&pseudo, &effect_happened, &youmonst);
            break;
        }
        case ARTINVOKE_HEALING:
        {
            int healamt = (u.uhpmax + 1 - u.uhp) / 2;
            long creamed = (long) u.ucreamed;
            play_sfx_sound(SFX_HEALING);
            play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
            special_effect_wait_until_action(0);

            if (Upolyd)
                healamt = (u.mhmax + 1 - u.mh) / 2;
            if (healamt || Sick || FoodPoisoned || MummyRot || Slimed || Blinded > creamed)
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "better.");
            else
                goto nothing_special;
            healup(healamt, 0, TRUE, FALSE, FALSE, FALSE, FALSE);
            //if (healamt > 0) {
            //    if (Upolyd)
            //        u.mh += healamt;
            //    else
            //        u.uhp += healamt;
            //}
            //if (Sick)
            //    make_sick(0L, (char *) 0, FALSE);
            //if (FoodPoisoned)
            //    make_food_poisoned(0L, (char*)0, FALSE);
            //if (MummyRot)
            //    make_mummy_rotted(0L, (char*)0, FALSE);
            if (Slimed)
                make_slimed(0L, (char *) 0, 0, (char*)0, 0);
            if (Blinded > creamed)
                make_blinded(creamed, FALSE);
            special_effect_wait_until_end(0);
            context.botl = TRUE;
            break;
        }
        case ARTINVOKE_ENERGY_BOOST: 
        {
            int epboost = (u.uenmax + 1 - u.uen) / 2;

            if (epboost > 120)
                epboost = 120; /* arbitrary */
            else if (epboost < 12)
                epboost = u.uenmax - u.uen;
            if (epboost) {
                play_sfx_sound(SFX_GAIN_ENERGY);
                play_special_effect_at(SPECIAL_EFFECT_GENERIC_SPELL, 0, u.ux, u.uy, FALSE);
                special_effect_wait_until_action(0);

                int mana_before = u.uen;
                u.uen += epboost;
                int mana_after = u.uen;
                int mana_gain = mana_after - mana_before;
                if (mana_gain > 0)
                {
                    char fbuf[BUFSZ];
                    Sprintf(fbuf, "+%d", mana_gain);
                    display_floating_text(u.ux, u.uy, fbuf, FLOATING_TEXT_MANA_GAIN, ATR_NONE, NO_COLOR, 0UL);
                }
                You_feel_ex(ATR_NONE, CLR_MSG_POSITIVE, "re-energized.");
                special_effect_wait_until_end(0);
                context.botl = TRUE;
            } else
                goto nothing_special;
            break;
        }
        case ARTINVOKE_UNTRAP: 
        {
            if (!untrap(TRUE)) {
                obj->repowerleft = 0;
                return 0;
            }
            break;
        }
        case ARTINVOKE_CHARGE_OBJ:
        {
            struct obj *otmp = getobj(recharge_type, "charge", 0, "");
            boolean b_effect;

            if (!otmp) {
                obj->age = 0;
                return 0;
            }
            b_effect = (obj->blessed && (oart->role == Role_switch
                                         || oart->role == NON_PM));
            recharge(otmp, b_effect ? 1 : obj->cursed ? -1 : 0, TRUE, "Artifact Recharging", FALSE);
            break;
        }
        case ARTINVOKE_LEVEL_TELEPORT:
            level_tele(2, FALSE, zerodlevel);
            break;
        case ARTINVOKE_CREATE_PORTAL:
        {
            int portal_res = create_portal();
            if (!portal_res)
                goto nothing_special;

            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
            break;
        }
        case ARTINVOKE_ENLIGHTENING:
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
            enlightenment(MAGICENLIGHTENMENT, ENL_GAMEINPROGRESS);
            break;
        case ARTINVOKE_CREATE_AMMO:
        {
            struct obj *otmp = mksobj(ARROW, TRUE, FALSE, FALSE);

            if (!otmp)
                goto nothing_special;

            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
            otmp->blessed = obj->blessed;
            otmp->cursed = obj->cursed;
            otmp->bknown = obj->bknown;
            if (obj->blessed) {
                if (otmp->enchantment < 0)
                    otmp->enchantment = 0;
                otmp->quan += rnd(10);
            } else if (obj->cursed) {
                if (otmp->enchantment > 0)
                    otmp->enchantment = 0;
            } else
                otmp->quan += rnd(5);
            otmp->owt = weight(otmp);
            otmp = hold_another_object(otmp, "Suddenly %s out.",
                                       aobjnam(otmp, "fall"), (char *) 0);
            nhUse(otmp);
            break;
        }
        case ARTINVOKE_ARROW_OF_DIANA:
        {
            struct obj pseudo = zeroobj;
            pseudo.otyp = SPE_ARROW_OF_DIANA;
            pseudo.quan = 20L; /* do not let useup get it */
            double damage = 0;

            if (!getdir((char*)0))
            {
                pline1(Never_mind);
                return 0;
            }
            if (!u.dx && !u.dy && !u.dz)
            {
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
                if ((damage = zapyourself(&pseudo, TRUE)) > 0)
                {
                    char buf[BUFSZ];

                    Sprintf(buf, "shot %sself with %s", uhim(), cxname(obj));
                    losehp(damage, buf, NO_KILLER_PREFIX);
                }
            }
            else
            {
                update_u_facing(TRUE);
                weffects(&pseudo);
            }
            break;
        }
        case ARTINVOKE_WAND_OF_DEATH:
        {
            struct obj pseudo = zeroobj;
            pseudo.otyp = WAN_DEATH;
            pseudo.quan = 1L; /* do not let useup get it */
            double damage = 0;

            if (!getdir((char*)0)) 
            {
                pline1(Never_mind);
                return 0;
            }
            if (!u.dx && !u.dy && !u.dz) 
            {
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
                if ((damage = zapyourself(&pseudo, TRUE)) > 0)
                {
                    char buf[BUFSZ];

                    Sprintf(buf, "zapped %sself with %s", uhim(), cxname(obj));
                    losehp(damage, buf, NO_KILLER_PREFIX);
                }
            }
            else
            {
                update_u_facing(TRUE);
                weffects(&pseudo);
            }
            break;
        }
        case ARTINVOKE_BLESS_CONTENTS:
        {
            int cnt = 0;
            for (struct obj* otmp = obj->cobj; otmp; otmp = otmp->nobj)
            {
                cnt++;
            }
            int selected_item = 0;
            
            if (cnt == 0)
            {
                pline("Nothing seems to happen.");
                obj->repowerleft = artilist[obj->oartifact].repower_time;
            }
            else
            {
                if (cnt > 1)
                    selected_item = rn2(cnt);
                int i = 0;
                boolean blessed = FALSE;
                for (struct obj* otmp = obj->cobj; otmp; otmp = otmp->nobj)
                {
                    if (i == selected_item)
                    {
                        bless(otmp);
                        blessed = TRUE;
                        break;
                    }
                    i++;
                }
                if (blessed)
                {
                    check_arti_name_discovery(obj);
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "A light blue aura glows inside %s for a while.", the(cxname(obj)));
                    obj->repowerleft = artilist[obj->oartifact].repower_time;
                }
                else
                {
                    pline("Nothing seems to happen.");
                    obj->repowerleft = artilist[obj->oartifact].repower_time / 30;
                }
            }
            break;
        }
        case ARTINVOKE_WISHING:
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
            makewish(FALSE, TRUE);
            break;
        }
        case ARTINVOKE_DEMON_SUMMON:
        {
            struct monst* mon = (struct monst*)0;
            mon = makemon(&mons[PM_NALFESHNEE], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_CHAOTIC_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
            if (mon)
            {
                mon->issummoned = TRUE;
                (void)tamedog(mon, (struct obj*) 0, TAMEDOG_FORCE_NON_UNIQUE, FALSE, 0, FALSE, FALSE);

                if (temporary_effect)
                {
                    mon->summonduration = d(art_inv_dur_dice, art_inv_dur_diesize) + art_inv_dur_plus;
                    begin_summontimer(mon);
                }
                mon->mprops[SUMMON_FORBIDDEN] |= M_INTRINSIC_ACQUIRED;

                play_sfx_sound_at_location(SFX_SUMMON_DEMON, mon->mx, mon->my);
                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s appears in a puff of smoke!", Amonnam(mon));
            }
            else
            {
                goto nothing_special;
            }
            break;
        }
        case ARTINVOKE_AIR_ELEMENTAL_SUMMON:
        {
            struct monst* mon = (struct monst*)0;
            mon = makemon(&mons[PM_ELDER_AIR_ELEMENTAL], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_NEUTRAL_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
            if(!mon)
                mon = makemon(&mons[PM_AIR_ELEMENTAL], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_NEUTRAL_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);

            if (mon)
            {
                mon->issummoned = TRUE;
                (void)tamedog(mon, (struct obj*)0, TAMEDOG_FORCE_NON_UNIQUE, FALSE, 0, FALSE, FALSE);

                if (temporary_effect)
                {
                    mon->summonduration = d(art_inv_dur_dice, art_inv_dur_diesize) + art_inv_dur_plus;
                    begin_summontimer(mon);
                }
                mon->mprops[SUMMON_FORBIDDEN] |= M_INTRINSIC_ACQUIRED;

                pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "The air around you starts to swirl and forms into %s!", a_monnam(mon));
            }
            else
            {
                goto nothing_special;
            }
            break;
        }
        case ARTINVOKE_RECHARGE_ITSELF:
        {
            int old_recharged = obj->recharged;
            int old_charges = obj->charges;
            obj->recharged = 0;
            obj->charges = get_obj_max_charge(obj);
            if (obj->oartifact == ART_HOLY_GRAIL)
            {
                if (obj->charges > old_charges)
                {
                    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
                    pline_ex(ATR_NONE, CLR_MSG_POSITIVE, "%s itself with %s.", Tobjnam(obj, "fill"), OBJ_CONTENT_DESC(obj->otyp));
                }
                else if (obj->recharged != old_recharged)
                {
                    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE2);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s a bit more shiny.", Tobjnam(obj, "look"));
                }
                else
                {
                    goto nothing_special;
                }
            }
            else
            {
                if (obj->charges > old_charges)
                {
                    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
                    p_glow2(obj, NH_BLUE, ATR_NONE, CLR_MSG_POSITIVE, "Artifact Recharging", FALSE);
                }
                else if (obj->recharged != old_recharged)
                {
                    play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE2);
                    p_glow1(obj, ATR_NONE, CLR_MSG_POSITIVE, "Artifact Recharging", FALSE);
                }
                else
                {
                    goto nothing_special;
                }
            }
            break;
        }
        case ARTINVOKE_TIME_STOP:
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
            timestop(duration);
            break;
        }
        case ARTINVOKE_INVOKE_WITH_TIMER:
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
            char artifact_hit_desc[BUFSZ] = "";
            if (obj->oartifact && artilist[obj->oartifact].hit_desc && strcmp(artilist[obj->oartifact].hit_desc, ""))
                Strcpy(artifact_hit_desc, artilist[obj->oartifact].hit_desc);
            else
                Strcpy(artifact_hit_desc, cxname(obj));

            pline("As you invoke %s, a surge of power surronds %s." , the(cxname(obj)), the(artifact_hit_desc));
            obj->invokeleft = duration;
            break;
        }

        } /* switch */
    } 
    else 
    {
        boolean switch_on = (obj->invokeon == FALSE); // (u.uprops[oart->inv_prop].extrinsic& W_ARTIFACT_INVOKED) == 0;
        boolean noeff = temporary_effect ? (u.uprops[oart->inv_prop].extrinsic || u.uprops[oart->inv_prop].intrinsic) : ((u.uprops[oart->inv_prop].extrinsic & ~W_ARTIFACT_INVOKED) || u.uprops[oart->inv_prop].intrinsic);

        if (temporary_effect)
        {
            play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
            incr_itimeout(&u.uprops[oart->inv_prop].intrinsic, duration);
            refresh_u_tile_gui_info(TRUE);
        }
        else
        {
            if (!switch_on)
            {
                obj->repowerleft = artilist[obj->oartifact].repower_time;
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE);
            }
            else
                play_simple_object_sound(obj, OBJECT_SOUND_TYPE_INVOKE2);

            obj->invokeon = switch_on;
        }

        if (noeff)
        {
        nothing_special:
            /* you had the property from some other source too */
            if (carried(obj))
                You_feel("a surge of power, but nothing seems to happen.");
            return 1;
        }

        /* effect happened, tell it here */
        switch (oart->inv_prop) {
        case CONFLICT:
            if ((!temporary_effect && switch_on) || temporary_effect)
            {
                play_sfx_sound(SFX_CONFLICT);
                You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "like a rabble-rouser.");
            }
            else
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "the tension decrease around you.");
            break;
        case DISPLACED:
            if (obj->oartifact == ART_MAGIC_MIRROR_OF_MERLIN)
            {
                if ((!temporary_effect && switch_on) || temporary_effect)
                {
                    play_sfx_sound(SFX_MIRROR_IMAGE);
                    pline_ex(ATR_NONE, CLR_MSG_SUCCESS, "%s an illusionary image of yourself near you.", Tobjnam(obj, "project"));
                }
                else
                    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "illusionary double disappears.");
            }
            else
            {
                if ((!temporary_effect && switch_on) || temporary_effect)
                {
                    play_sfx_sound(SFX_MIRROR_IMAGE);
                    You_feel_ex(ATR_NONE, CLR_MSG_SUCCESS, "your image becomes displaced.");
                }
                else
                    You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "your image is in its right place again.");
            }
            break;
        case LEVITATION:
#if 0
            if (switch_on) {
                float_up();
                spoteffects(FALSE);
            }
            else
                (void)float_down(I_SPECIAL | TIMEOUT, W_ARTIFACT_INVOKED);
#endif
            break;
        case INVISIBILITY:
#if 0
            if (Blocks_Invisibility || Blind)
                goto nothing_special;
            newsym(u.ux, u.uy);
            if (switch_on)
                Your("body takes on a %s transparency...",
                    Hallucination ? "normal" : "strange");
            else
                Your("body seems to unfade...");
#endif
            break;
        }
    }

    update_inventory();
    return 1;
}

STATIC_OVL
void
check_arti_name_discovery(obj)
struct obj* obj;
{
    if (obj && obj->oartifact && !obj->nknown && (artilist[obj->oartifact].aflags & (AF_FAMOUS | AF_NAME_KNOWN_WHEN_INVOKED)))
    {
        play_sfx_sound(SFX_ARTIFACT_NAME_KNOWN);
        if(obj->oartifact == ART_HOWLING_FLAIL)
            play_sfx_sound(SFX_HOWLING_FLAIL_HOWL);

        pline_ex(ATR_NONE, CLR_MSG_HINT, "As you invoke %s, %syou become aware that %s named %s!", the(cxname(obj)),
            obj->oartifact == ART_HOWLING_FLAIL ? "it lets loose a majestic howl and " : "",
            (pair_of(obj) || obj->quan > 1) ? "they are" : "it is", bare_artifactname(obj));
        obj->nknown = TRUE;
    }
}

int
create_portal()
{
    int i, num_ok_dungeons, last_ok_dungeon = 0;
    d_level newlev;
    extern int n_dgns; /* from dungeon.c */
    winid tmpwin = create_nhwindow(NHW_MENU);
    anything any;

    any = zeroany; /* set all bits to zero */
    start_menu_ex(tmpwin, GHMENU_STYLE_CHOOSE_SIMPLE);

    /* use index+1 (cant use 0) as identifier */
    for (i = num_ok_dungeons = 0; i < n_dgns; i++) 
    {
        if (!dungeons[i].dunlev_ureached)
            continue;
        any.a_int = i + 1;
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE,
            dungeons[i].dname, MENU_UNSELECTED);
        num_ok_dungeons++;
        last_ok_dungeon = i;
    }
    
    end_menu(tmpwin, "Open a portal to which dungeon?");


    if (num_ok_dungeons > 1) 
    {
        /* more than one entry; display menu for choices */
        menu_item* selected;
        int n;

        n = select_menu(tmpwin, PICK_ONE, &selected);
        if (n <= 0)
        {
            destroy_nhwindow(tmpwin);
            return 0;
        }
        i = selected[0].item.a_int - 1;
        free((genericptr_t)selected);
    }
    else
        i = last_ok_dungeon; /* also first & only OK dungeon */

    destroy_nhwindow(tmpwin);

    /*
     * i is now index into dungeon structure for the new dungeon.
     * Find the closest level in the given dungeon, open
     * a use-once portal to that dungeon and go there.
     * The closest level is either the entry or dunlev_ureached.
     */
    newlev.dnum = i;
    if (dungeons[i].depth_start >= depth(&u.uz))
        newlev.dlevel = dungeons[i].entry_lev;
    else
        newlev.dlevel = dungeons[i].dunlev_ureached;

    if (u.uhave.amulet || In_endgame(&u.uz) || In_endgame(&newlev)
        || newlev.dnum == u.uz.dnum || !next_to_u())
    {
        play_sfx_sound(SFX_DISORIENTED_FOR_MOMENT);
        You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "very disoriented for a moment.");
    }
    else 
    {
        play_sfx_sound(SFX_LEVEL_TELEPORT);
        if (!Blind)
            You_ex(ATR_NONE, CLR_MSG_MYSTICAL, "are surrounded by a shimmering sphere!");
        else
            You_feel_ex(ATR_NONE, CLR_MSG_MYSTICAL, "weightless for a moment.");
        goto_level(&newlev, FALSE, FALSE, FALSE);
    }

    return 1;
}

/* will freeing this object from inventory cause levitation to end? */
/* OBSOLETE -- JG */
boolean
finesse_ahriman(obj)
struct obj *obj UNUSED;
{
    return 0;

#if 0
    const struct artifact *oart;
    struct prop save_Lev;
    boolean result;

    /* if we aren't levitating or this isn't an artifact which confers
       levitation via #invoke then freeinv() won't toggle levitation */
    if (!Levitation || (oart = get_artifact(obj)) == 0
        || oart->inv_prop != LEVITATION || !(ELevitation & W_ARTIFACT_INVOKED))
        return FALSE;

    /* arti_invoke(off) -> float_down() clears I_SPECIAL|TIMEOUT & W_ARTIFACT_INVOKED;
       probe ahead to see whether that actually results in floating down;
       (this assumes that there aren't two simultaneously invoked artifacts
       both conferring levitation--safe, since if there were two of them,
       invoking the 2nd would negate the 1st rather than stack with it) */
    save_Lev = u.uprops[LEVITATION];
    HLevitation &= ~(I_SPECIAL | TIMEOUT);
    ELevitation &= ~W_ARTIFACT_INVOKED;
    result = (boolean) !Levitation;
    u.uprops[LEVITATION] = save_Lev;
    return result;
#endif
}

/* WAC return TRUE if artifact is always lit */
boolean
artifact_light(obj)
struct obj *obj;
{
    return (boolean) (obj && obj->oartifact && artifact_has_flag(obj, AF_SHINES_MAGICAL_LIGHT));
}

/* KMH -- Talking artifacts are finally implemented */
void
arti_speak(obj)
struct obj *obj;
{
    register const struct artifact *oart = get_artifact(obj);
    const char *line;
    char buf[BUFSZ];

    /* Is this a speaking artifact? */
    if (!oart || !(oart->aflags & AF_SPEAK))
        return;

    line = getrumor(bcsign(obj), buf, TRUE);
    if (!*line)
        line = "GnollHack rumors file closed for renovation.";
    pline_ex(ATR_NONE, CLR_MSG_GOD, "%s:", Tobjnam(obj, "whisper"));
    verbalize1(line);
    display_popup_text(line, cxname(obj), POPUP_TEXT_DIALOGUE, ATR_NONE, NO_COLOR, obj_to_glyph(obj, rn2_on_display_rng), POPUP_FLAGS_ADD_QUOTES);
    return;
}

boolean
artifact_has_invprop(otmp, inv_prop)
struct obj *otmp;
uchar inv_prop;
{
    const struct artifact *arti = get_artifact(otmp);

    return (boolean) (arti && (arti->inv_prop == inv_prop));
}

/* Return the price sold to the hero of a given artifact or unique item */
long
arti_cost(otmp)
struct obj *otmp;
{
    if (!otmp->oartifact)
        return objects[otmp->otyp].oc_cost;
    else if (artilist[(int) otmp->oartifact].cost)
        return artilist[(int) otmp->oartifact].cost;
    else
        return (20L * (objects[otmp->otyp].oc_cost + 75L));
}

struct abil2adtyp_tag {
    int prop;
    uchar adtyp;
} abil2adtyp[] = {
    { FIRE_IMMUNITY, AD_FIRE },
    { COLD_IMMUNITY, AD_COLD },
    { SHOCK_IMMUNITY, AD_ELEC },
    { ACID_IMMUNITY, AD_ACID },
    { DEATH_RESISTANCE, AD_DRAY },
    { LYCANTHROPY_RESISTANCE, AD_WERE },
    { MAGIC_MISSILE_IMMUNITY, AD_MAGM },
    { DISINTEGRATION_RESISTANCE, AD_DISN },
    { POISON_RESISTANCE, AD_DRST },
    { DRAIN_RESISTANCE, AD_DRLI },
    { FLASH_RESISTANCE, AD_BLND },
    { STONE_RESISTANCE, AD_STON },
    { STUN_RESISTANCE, AD_STUN },
    { FIRE_RESISTANCE, AD_FIRE },
    { COLD_RESISTANCE, AD_COLD },
    { SHOCK_RESISTANCE, AD_ELEC },
    { MAGIC_MISSILE_RESISTANCE, AD_MAGM },
    { ACID_RESISTANCE, AD_ACID },
    { IMPROVED_FIRE_RESISTANCE, AD_FIRE },
    { IMPROVED_COLD_RESISTANCE, AD_COLD },
    { IMPROVED_SHOCK_RESISTANCE, AD_ELEC },
    { IMPROVED_MAGIC_MISSILE_RESISTANCE, AD_MAGM },
    { IMPROVED_ACID_RESISTANCE, AD_ACID },
};

uchar
prop_to_adtyp(prop_index)
int prop_index;
{
    int k;

    for (k = 0; k < SIZE(abil2adtyp); k++) {
        if (abil2adtyp[k].prop == prop_index)
            return abil2adtyp[k].adtyp;
    }
    return 0;
}

int
adtyp_to_prop(adtyp_index)
uchar adtyp_index;
{
    int k;

    for (k = 0; k < SIZE(abil2adtyp); k++) {
        if (abil2adtyp[k].adtyp == adtyp_index)
            return abil2adtyp[k].prop;
    }
    return 0;
}

STATIC_VAR const struct abil2spfx_tag {
    int prop;
    unsigned long spfx;
} abil2spfx[] = {
    { DEATH_RESISTANCE, SPFX_DEATH_RES },
    { DRAIN_RESISTANCE, SPFX_DRAIN_RES },
    { ANTIMAGIC, SPFX_ANTIMAGIC },
    { WARN_OF_MON, SPFX_WARN_OF_MON },
    { SEARCHING, SPFX_SEARCH },
    { HALLUC_RES, SPFX_HALRES },
    { TELEPAT, SPFX_ESP },
    { STEALTH, SPFX_STLTH },
    { REGENERATION, SPFX_REGEN },
    { ENERGY_REGENERATION, SPFX_EREGEN },
    { HALF_SPELL_DAMAGE, SPFX_HSPDAM },
    { HALF_PHYSICAL_DAMAGE, SPFX_HPHDAM },
    { TELEPORT_CONTROL, SPFX_TCTRL },
    /* SPFX_LUCK not here */
    { XRAY_VISION, SPFX_XRAY },
    { REFLECTING, SPFX_REFLECT },
    { MAGICAL_PROTECTION, SPFX_PROTECT },
    { AGGRAVATE_MONSTER, SPFX_AGGRAVATE_MONSTER },
    /* SPFX_UNLUCK not here */
    { BLOCKS_BLINDNESS, SPFX_BLIND_SEEING },
    { HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS, SPFX_HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS },
    { WARNING, SPFX_WARNING },
    { WARN_ORC, SPFX_ORC_WARNING },
    { WARN_ELF, SPFX_ELF_WARNING },
    { WARN_DEMON, SPFX_DEMON_WARNING },
    { WARN_UNDEAD, SPFX_UNDEAD_WARNING },
    { TITAN_STRENGTH, SPFX_STR_25 },
    { DIVINE_DEXTERITY, SPFX_DEX_25 },
    { DIVINE_ENDURANCE, SPFX_CON_25 },
    { DIVINE_INTELLECT, SPFX_INT_25 },
    { DIVINE_WISDOM, SPFX_WIS_25 },
    { DIVINE_CHARISMA, SPFX_CHA_25 },
};

unsigned long
prop_to_spfx(prop_index)
int prop_index;
{

    int k;

    for (k = 0; k < SIZE(abil2spfx); k++) {
        if (abil2spfx[k].prop == prop_index)
            return abil2spfx[k].spfx;
    }
    return 0L;
}

int
spfx_to_prop(spfx_bit)
unsigned long spfx_bit;
{

    int k;

    for (k = 0; k < SIZE(abil2spfx); k++) {
        if (abil2spfx[k].spfx == spfx_bit)
            return abil2spfx[k].prop;
    }
    return 0;
}

/*
 * Return the first item that is conveying a particular extrinsic.
 */
struct obj *
what_gives(prop_index)
int prop_index;
{
    struct obj *obj;
    long wornbits;
    long spfx = prop_to_spfx(prop_index);

    wornbits = u.uprops[prop_index].extrinsic;

    for (obj = invent; obj; obj = obj->nobj) 
    {
        if ((wornbits & W_ARTIFACT_CARRIED) && obj->oartifact && (artilist[obj->oartifact].carried_prop == prop_index || (artilist[obj->oartifact].cspfx & spfx)))
            return obj;

        if ((wornbits & W_ARTIFACT_INVOKED) && obj->oartifact && artilist[obj->oartifact].inv_prop == prop_index && obj->invokeon)
            return obj;

        if (wornbits & W_CARRIED)
        {
            if (carried_item_is_giving_monster_power(&youmonst, obj, prop_index))
                return obj;
        }

        if (wornbits & obj->owornmask)
            return obj;
    }
    return (struct obj *) 0;
}

/* assumes obj is in mon inventory */
boolean
carried_item_is_giving_monster_power(mon, obj, prop_index)
struct monst* mon;
struct obj* obj;
int prop_index;
{
    if (!obj)
        return FALSE;

    int otyp = obj->otyp;
    boolean inappr = inappropriate_monster_character_type(mon,obj);

    if (objects[otyp].oc_oprop == prop_index
        && (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_WHEN_CARRIED)
        && ((!inappr && !(objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
            || (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_ALL_CHARACTERS)
            || (inappr && (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
        )
        return TRUE;

    if (objects[otyp].oc_oprop2 == prop_index
        && (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_WHEN_CARRIED)
        && ((!inappr && !(objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
            || (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_ALL_CHARACTERS)
            || (inappr && (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
        )
        return TRUE;

    if (objects[otyp].oc_oprop3 == prop_index
        && (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_WHEN_CARRIED)
        && ((!inappr && !(objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
            || (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_ALL_CHARACTERS)
            || (inappr && (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
        )
        return TRUE;

    return FALSE;
}

/* assumes obj is in mon inventory */
boolean
carried_artifact_is_giving_monster_power(mon, obj, prop_index)
struct monst* mon;
struct obj* obj;
int prop_index;
{
    if (!mon)
    {
        /* Do nothing, since mon is not being used */
    }

    long spfx = prop_to_spfx(prop_index);

    return (obj && obj->oartifact && (artilist[obj->oartifact].carried_prop == prop_index || (artilist[obj->oartifact].cspfx & spfx)));
}

boolean
worn_item_is_giving_monster_power(mon, obj, prop_index)
struct monst* mon;
struct obj* obj;
int prop_index;
{
    if (!obj)
        return FALSE;

    if (!obj->owornmask)
        return carried_item_is_giving_monster_power(mon, obj, prop_index);

    if ((obj->owornmask & W_WEP) && !is_wielded_item(obj))
        return carried_item_is_giving_monster_power(mon, obj, prop_index);

    if ((obj->owornmask & W_WEP2) && !is_wielded_item(obj))
        return carried_item_is_giving_monster_power(mon, obj, prop_index);

    int otyp = obj->otyp;
    boolean inappr = inappropriate_monster_character_type(mon, obj);

    if (objects[otyp].oc_oprop == prop_index
        && ((!inappr && !(objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
            || (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_ALL_CHARACTERS)
            || (inappr && (objects[otyp].oc_pflags & P1_POWER_1_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
        )
        return TRUE;

    if (objects[otyp].oc_oprop2 == prop_index
        && ((!inappr && !(objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
            || (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_ALL_CHARACTERS)
            || (inappr && (objects[otyp].oc_pflags & P1_POWER_2_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
        )
        return TRUE;

    if (objects[otyp].oc_oprop3 == prop_index
        && ((!inappr && !(objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY))
            || (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_ALL_CHARACTERS)
            || (inappr && (objects[otyp].oc_pflags & P1_POWER_3_APPLIES_TO_INAPPROPRIATE_CHARACTERS_ONLY)))
        )
        return TRUE;

    return FALSE;
}

boolean
item_is_giving_monster_power(mon, obj, prop_index)
struct monst* mon;
struct obj* obj;
int prop_index;
{

    if (worn_item_is_giving_monster_power(mon, obj, prop_index))
        return TRUE;

    if (obj->oartifact && artifact_confers_monster_power(mon, obj, prop_index))
        return TRUE;

    return FALSE;
}

boolean
item_is_giving_power(obj, prop_index)
struct obj* obj;
int prop_index;
{
    return item_is_giving_monster_power(&youmonst, obj, prop_index);
}


const char *
glow_color(arti_indx)
int arti_indx;
{
    int colornum = artilist[arti_indx].acolor;
    const char *colorstr = clr2colorname(colornum);

    return hcolor(colorstr);
}

/* glow verb; [0] holds the value used when blind */
STATIC_VAR const char *glow_verbs[] = {
    "quiver", "flicker", "glimmer", "gleam"
};

/* relative strength that Sting is glowing (0..3), to select verb */
STATIC_OVL int
glow_strength(count)
int count;
{
    /* glow strength should also be proportional to proximity and
       probably difficulty, but we don't have that information and
       gathering it is more trouble than this would be worth */
    return (count > 12) ? 3 : (count > 4) ? 2 : (count > 0);
}

const char *
glow_verb(count, ingsfx)
int count; /* 0 means blind rather than no applicable creatures */
boolean ingsfx;
{
    static char resbuf[20];

    Strcpy(resbuf, glow_verbs[glow_strength(count)]);
    /* ing_suffix() will double the last consonant for all the words
       we're using and none of them should have that, so bypass it */
    if (ingsfx)
        Strcat(resbuf, "ing");
    return resbuf;
}

boolean
item_has_specific_monster_warning(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    boolean res = (
        item_is_giving_power(otmp, WARN_OF_MON)
        || item_is_giving_power(otmp, WARN_ORC)
        || item_is_giving_power(otmp, WARN_DEMON)
        || item_is_giving_power(otmp, WARN_UNDEAD)
        || item_is_giving_power(otmp, WARN_TROLL)
        || item_is_giving_power(otmp, WARN_GIANT)
        || item_is_giving_power(otmp, WARN_DRAGON)
        || item_is_giving_power(otmp, WARN_ELF)
        || item_is_giving_power(otmp, WARN_DWARF)
        || item_is_giving_power(otmp, WARN_GNOME)
        || item_is_giving_power(otmp, WARN_GNOLL)
        || item_is_giving_power(otmp, WARN_OGRE)
        || item_is_giving_power(otmp, WARN_HUMAN)
        || item_is_giving_power(otmp, WARN_LYCANTHROPE)
        || item_is_giving_power(otmp, WARN_ANGEL)
        );

    return res;
}

/* use for warning "glow" for Sting, Orcrist, and Grimtooth */
void
Sting_effects(otmp, orc_count)
struct obj* otmp;
int orc_count; /* new count, new count is in the items; OBSOLETE: (warn_obj_cnt is old count); -1 is a flag value */
{
    if (!otmp)
        return;

    int otyp = otmp->otyp;

    if (otmp && item_has_specific_monster_warning(otmp))
    {
        int oldstr = glow_strength(otmp->detectioncount),
            newstr = glow_strength(orc_count);

        char colorbuf[BUFSZ] = "red";

        if (otmp->oartifact)
            strcpy(colorbuf, glow_color(otmp->oartifact));

        if (!otmp->oartifact || strcmp(colorbuf, "no color") == 0)
        {
            if ((objects[otyp].oc_flags2 & O2_FLICKER_COLOR_MASK) == O2_FLICKER_COLOR_BLACK)
                strcpy(colorbuf, "black");
            else if ((objects[otyp].oc_flags2 & O2_FLICKER_COLOR_MASK) == O2_FLICKER_COLOR_WHITE)
                strcpy(colorbuf, "white");
            else if ((objects[otyp].oc_flags2 & O2_FLICKER_COLOR_MASK) == O2_FLICKER_COLOR_BLUE)
                strcpy(colorbuf, "blue");
            else
                strcpy(colorbuf, "red");
        }


        char weapbuf[BUFSZ] = "";
        strcpy(weapbuf, Yname2(otmp));

        if (orc_count == -1 && otmp->detectioncount > 0) {
            /* -1 means that blindness has just been toggled; give a
               'continue' message that eventual 'stop' message will match */
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s is %s.", weapbuf,
                  glow_verb(Blind ? 0 : otmp->detectioncount, TRUE));
        } else if (newstr > 0 && newstr != oldstr) {
            /* 'start' message */
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s %s%c", weapbuf,
                      otense(otmp, glow_verb(orc_count, FALSE)),
                      colorbuf,
                      (newstr > oldstr) ? '!' : '.');
            else if (oldstr == 0) /* quivers */
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s %s slightly.", weapbuf,
                      otense(otmp, glow_verb(0, FALSE)));
        } else if (orc_count == 0 && otmp->detectioncount > 0) {
            /* 'stop' message */
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s stops %s.", weapbuf,
                  glow_verb(Blind ? 0 : otmp->detectioncount, TRUE));
        }
    }
}

/* called when hero is wielding/applying/invoking a carried item, or
   after undergoing a transformation (alignment change, lycanthropy,
   polymorph) which might affect item access */
int
retouch_object(objp, loseit)
struct obj **objp; /* might be destroyed or unintentionally dropped */
boolean loseit;    /* whether to drop it if hero can longer touch it */
{
    if (!objp || !*objp)
        return 0;
    
    struct obj *obj = *objp;

    if (touch_artifact(obj, &youmonst)) {
        char buf[BUFSZ];
        double damage = 0;
        boolean ag = (obj->material == MAT_SILVER && Hate_silver),
            bane = bane_applies(get_artifact(obj), &youmonst),
            inappr_character = ((objects[obj->otyp].oc_flags4 & O4_INAPPROPRIATE_CHARACTERS_CANT_HANDLE) != 0 && inappropriate_monster_character_type(&youmonst, obj)),
            inappr_exceptionality = inappropriate_exceptionality(&youmonst, obj);

        /* nothing else to do if hero can successfully handle this object */
        if (!ag && !bane && !inappr_exceptionality && !inappr_character)
            return 1;

        /* hero can't handle this object, but didn't get touch_artifact()'s
           "<obj> evades your grasp|control" message; give an alternate one */
        You_cant_ex(ATR_NONE, CLR_MSG_WARNING, "handle %s%s!", yname(obj),
                 obj->owornmask ? " anymore" : "");
        /* also inflict damage unless touch_artifact() already did so */
        if (!touch_blasted) {
            /* damage is somewhat arbitrary; half the usual 1d20 physical
               for silver, 1d10 magical for <foo>bane, potentially both */
            if (ag)
                damage = adjust_damage(rnd(10), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE);
            if (bane)
                damage += adjust_damage(rnd(10), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE);
            if (inappr_character)
                damage += adjust_damage(rnd(5), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE);
            if (inappr_exceptionality)
                damage += adjust_damage(rnd(5), (struct monst*)0, &youmonst, AD_PHYS, ADFLAGS_NONE);
            Sprintf(buf, "handling %s", killer_xname(obj));
            losehp(damage, buf, KILLED_BY);
            exercise(A_CON, FALSE);
        }
    }

    /* removing a worn item might result in loss of levitation,
       dropping the hero onto a polymorph trap or into water or
       lava and potentially dropping or destroying the item */
    if (obj->owornmask) {
        struct obj *otmp;

        remove_worn_item(obj, FALSE);
        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (otmp == obj)
                break;
        if (!otmp)
            *objp = obj = 0;
    }

    /* if we still have it and caller wants us to drop it, do so now */
    if (loseit && obj) {
        if (Levitation) {
            freeinv(obj);
            hitfloor(obj, TRUE);
        } else {
            /* dropx gives a message iff item lands on an altar */
            if (!IS_ALTAR(levl[u.ux][u.uy].typ))
                pline("%s to the %s.", Tobjnam(obj, "fall"),
                      surface(u.ux, u.uy));
            dropx(obj);
        }
        *objp = obj = 0; /* no longer in inventory */
    }
    return 0;
}

/* an item which is worn/wielded or an artifact which conveys
   something via being carried or which has an #invoke effect
   currently in operation undergoes a touch test; if it fails,
   it will be unworn/unwielded and revoked but not dropped */
STATIC_OVL boolean
untouchable(obj, drop_untouchable)
struct obj *obj;
boolean drop_untouchable;
{
    struct artifact *art;
    boolean beingworn, carryeffect, invoked;
    long wearmask = ~(W_QUIVER | W_SWAP_WEAPON | W_BALL);

    beingworn = ((obj->owornmask & wearmask) != 0L
                 /* some items in use don't have any wornmask setting */
                 || (obj->oclass == TOOL_CLASS
                     && (obj->lamplit || (obj->otyp == LEASH && obj->leashmon)
                         || (Is_container(obj) && Has_contents(obj)))));

    if ((art = get_artifact(obj)) != 0) {
        carryeffect = (art->carried_prop || art->cspfx);
        invoked = (art->inv_prop > 0 && art->inv_prop <= LAST_PROP
                   && (u.uprops[art->inv_prop].extrinsic & W_ARTIFACT_INVOKED) != 0L);
    } else {
        carryeffect = invoked = FALSE;
    }

    if (beingworn || carryeffect || invoked) {
        if (!retouch_object(&obj, drop_untouchable)) {
            /* "<artifact> is beyond your control" or "you can't handle
               <object>" has been given and it is now unworn/unwielded
               and possibly dropped (depending upon caller); if dropped,
               carried effect was turned off, else we leave that alone;
               we turn off invocation property here if still carried */
            if (invoked && obj)
                arti_invoke(obj); /* reverse #invoke */
            return TRUE;
        }
    }
    return FALSE;
}


/* check all items currently in use (mostly worn) for touchability */
void
retouch_equipment(dropflag)
int dropflag; /* 0==don't drop, 1==drop all, 2==drop weapon */
{
    struct obj *obj;
    boolean dropit, had_gloves = (uarmg != 0);
    int had_rings = (!!uleft + !!uright);
    /*
     * We can potentially be called recursively if losing/unwearing
     * an item causes worn helm of opposite alignment to come off or
     * be destroyed.
     *
     * BUG: if the initial call was due to putting on a helm of
     * opposite alignment and it does come off to trigger recursion,
     * after the inner call executes, the outer call will finish
     * using the non-helm alignment rather than the helm alignment
     * which triggered this in the first place.
     */
    if (!nesting++)
        clear_bypasses(); /* init upon initial entry */

    dropit = (dropflag > 0); /* drop all or drop weapon */
    /* check secondary weapon first, before possibly unwielding primary */
    if (u.twoweap && uarms) {
        bypass_obj(uarms); /* so loop below won't process it again */
        (void) untouchable(uarms, dropit);
    }
    /* check primary weapon next so that they're handled together */
    if (uwep) {
        bypass_obj(uwep); /* so loop below won't process it again */
        (void) untouchable(uwep, dropit);
    }

    /* in case someone is daft enough to add artifact or silver saddle */
    if (u.usteed && (obj = which_armor(u.usteed, W_SADDLE)) != 0) {
        /* untouchable() calls retouch_object() which expects an object in
           hero's inventory, but remove_worn_item() will be harmless for
           saddle and we're suppressing drop, so this works as intended */
        if (untouchable(obj, FALSE))
            dismount_steed(DISMOUNT_THROWN);
    }
    /*
     * TODO?  Force off gloves if either or both rings are going to
     * become unworn; force off cloak [suit] before suit [shirt].
     * The torso handling is hypothetical; the case for gloves is
     * not, due to the possibility of unwearing silver rings.
     */

    dropit = (dropflag == 1); /* all untouchable items */
    /* loss of levitation (silver ring, or Heart of Ahriman invocation)
       might cause hero to lose inventory items (by dropping into lava,
       for instance), so inventory traversal needs to rescan the whole
       invent chain each time it moves on to another object; we use bypass
       handling to keep track of which items have already been processed */
    while ((obj = nxt_unbypassed_obj(invent)) != 0)
        (void) untouchable(obj, dropit);

    if (had_rings != (!!uleft + !!uright) && uarmg && uarmg->cursed)
        uncurse(uarmg); /* temporary? hack for ring removal plausibility */
    if (had_gloves && !uarmg)
        selftouch("After losing your gloves, you");

    if (!--nesting)
        clear_bypasses(); /* reset upon final exit */
}


STATIC_OVL int
count_surround_traps(x, y)
int x, y;
{
    struct rm *levp;
    struct obj *otmp;
    struct trap *ttmp;
    int dx, dy, glyph, ret = 0;

    for (dx = x - 1; dx < x + 2; ++dx)
        for (dy = y - 1; dy < y + 2; ++dy) {
            if (!isok(dx, dy))
                continue;
            /* If a trap is shown here, don't count it; the hero
             * should be expecting it.  But if there is a trap here
             * that's not shown, either undiscovered or covered by
             * something, do count it.
             */
            glyph = glyph_at(dx, dy);
            if (glyph_is_trap(glyph))
                continue;
            if ((ttmp = t_at(dx, dy)) != 0) {
                ++ret;
                continue;
            }
            levp = &levl[dx][dy];
            if (IS_DOOR(levp->typ) && (levp->doormask & D_TRAPPED) != 0) {
                ++ret;
                continue;
            }
            for (otmp = level.objects[dx][dy]; otmp; otmp = otmp->nexthere)
                if (Is_container(otmp) && otmp->otrapped) {
                    ++ret; /* we're counting locations, so just */
                    break; /* count the first one in a pile     */
                }
        }
    /*
     * [Shouldn't we also check inventory for a trapped container?
     * Even if its trap has already been found, there's no 'tknown'
     * flag to help hero remember that so we have nothing comparable
     * to a shown glyph to justify skipping it.]
     */
    return ret;
}

/* sense adjacent traps if wielding MKoT without wearing gloves */
void
mkot_trap_warn()
{
    static const char *const heat[7] = {
        "cool", "slightly warm", "warm", "very warm",
        "hot", "very hot", "like fire"
    };

    if (!uarmg && uwep && uwep->oartifact == ART_MASTER_KEY_OF_THIEVERY) {
        int idx, ntraps = count_surround_traps(u.ux, u.uy);

        if (ntraps != mkot_trap_warn_count) {
            idx = min(ntraps, SIZE(heat) - 1);
            pline_The("Key feels %s%c", heat[idx], (ntraps > 3) ? '!' : '.');
        }
        mkot_trap_warn_count = ntraps;
    } else
        mkot_trap_warn_count = 0;
}

/* Master Key is magic key if its bless/curse state meets our criteria:
   not cursed for rogues or blessed for non-rogues */
boolean
is_magic_key(mon, obj)
struct monst *mon; /* if null, non-rogue is assumed */
struct obj *obj;
{
    if (!obj)
        return FALSE;

    if (((obj && obj->oartifact == ART_MASTER_KEY_OF_THIEVERY)
         && ((mon == &youmonst) ? Role_if(PM_ROGUE)
                                : (mon && mon->data == &mons[PM_ROGUE])))
        ? !obj->cursed : obj->blessed)
        return TRUE;
    return FALSE;
}

/* figure out whether 'mon' (usually youmonst) is carrying the magic key */
struct obj *
has_magic_key(mon)
struct monst *mon; /* if null, hero assumed */
{
    struct obj *o;
    short key = artilist[ART_MASTER_KEY_OF_THIEVERY].otyp;

    if (!mon)
        mon = &youmonst;

    for (o = ((mon == &youmonst) ? invent : mon->minvent); o;
         o = nxtobj(o, key, FALSE)) 
    {
        if (is_magic_key(mon, o))
            return o;
    }
    return (struct obj *) 0;
}


const char* get_artifact_invoke_name(specialpropindex)
int specialpropindex;
{
    if (specialpropindex < ARTINVOKE_TAMING || specialpropindex >= ARTINVOKE_TAMING + SIZE(artifact_invoke_names))
        return empty_string;

    return artifact_invoke_names[specialpropindex - ARTINVOKE_TAMING];
}

boolean
is_artifact_applicable_as_axe(struct obj* obj)
{
    return (obj && obj->oartifact && (artilist[obj->oartifact].aflags2 & AF2_APPLICABLE_AS_AXE) != 0);
}

int
artifact_to_obj(artifactid)
int artifactid;
{
    if (artifactid <= 0 || artifactid > NUM_ARTIFACTS)
        return STRANGE_OBJECT;
    return (int)artilist[artifactid].otyp;

}
/*artifact.c*/
