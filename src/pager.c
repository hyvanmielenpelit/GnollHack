/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    pager.c    $NHDT-Date: 1555627307 2019/04/18 22:41:47 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.151 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2018. */
/* GnollHack may be freely redistributed.  See license for details. */

/* This file contains the command routines dowhatis() and dohelp() and */
/* a few other help related facilities */

#include "hack.h"
#include "dlb.h"

static boolean is_swallow_sym(nhsym);
static int append_str(char *, const char *);
static void look_at_object(char *, int, int, int);
static void look_at_monster(char *, char *, char*,
                                        struct monst *, int, int);
static struct permonst *lookat(int, int, char *, char*, char *);
static void checkfile(char *, struct permonst *,
                                  boolean, boolean, char *);
static void look_all(boolean,boolean);
static void do_supplemental_info(char *, struct permonst *,
                                             boolean);
static void print_decoration_ending(char*, int, int);
static void whatdoes_help(void);
static void docontact(void);
static void dispfile_help(void);
static void dispfile_shelp(void);
static void dispfile_optionfile(void);
static void dispfile_license(void);
static void dispfile_credits(void);
static void dispfile_debughelp(void);
static void hmenu_doextversion(void);
static void hmenu_dohistory(void);
static void hmenu_dowhatis(void);
static void hmenu_dowhatdoes(void);
static void hmenu_doextlist(void);
#ifdef PORT_HELP
extern void port_help(void);
#endif

/* Returns "true" for characters that could represent a monster's stomach. */
static boolean
is_swallow_sym(nhsym c)
{
    int i;

    for (i = S_sw_tl; i <= S_sw_br; i++)
        if (showsyms[i] == c)
            return TRUE;
    return FALSE;
}

/*
 * Append new_str to the end of buf if new_str doesn't already exist as
 * a substring of buf.  Return 1 if the string was appended, 0 otherwise.
 * It is expected that buf is of size BUFSZ.
 */
static int
append_str(char *buf, const char *new_str)
{
    size_t space_left; /* space remaining in buf */

    if (strstri(buf, new_str))
        return 0;

    space_left = BUFSZ - strlen(buf) - 1;
    if (space_left < 5)
        return 0;
    (void) strncat(buf, " or ", space_left);
    (void) strncat(buf, new_str, space_left - 4);
    return 1;
}

/* shared by monster probing (via query_objlist!) as well as lookat() */
char *
self_lookat(char *outbuf)
{
    char race[QBUFSZ];

    /* include race with role unless polymorphed */
    race[0] = '\0';
    if (!Upolyd)
        Sprintf(race, "%s ", urace.adj);
    Sprintf(outbuf, "%s%s%s called %s",
            /* being blinded may hide invisibility from self */
            (Invis && (senseself() || !Blind)) ? "invisible " : "", race,
            pm_monster_name(&mons[u.umonnum], flags.female), plname);
    if (u.usteed)
        Sprintf(eos(outbuf), ", mounted on %s", y_monnam(u.usteed));
    if (u.uundetected || (Upolyd && U_AP_TYPE))
        mhidden_description(&youmonst, FALSE, eos(outbuf));
    return outbuf;
}

/*
 * Parameters:
 *   altmon: for probing: if mimicking a monster, say so
 */
/* describe a hidden monster; used for look_at during extended monster
   detection and for probing; also when looking at self */
void
mhidden_description(struct monst *mon, boolean altmon, char *outbuf)
{
    struct obj *otmp;
    boolean fakeobj, isyou = (mon == &youmonst);
    int x = isyou ? u.ux : mon->mx, y = isyou ? u.uy : mon->my;
    int glyph = !isok(x, y) ? NO_GLYPH : abs((is_levflag_hero_memory(&level.flags) && !isyou) ? levl[x][y].hero_memory_layers.glyph
                                                    : glyph_at(x, y));

    *outbuf = '\0';
    if (M_AP_TYPE(mon) == M_AP_FURNITURE
        || M_AP_TYPE(mon) == M_AP_OBJECT) {
        Strcpy(outbuf, ", mimicking ");
        if (M_AP_TYPE(mon) == M_AP_FURNITURE) {
            Strcat(outbuf, an(defsyms[mon->mappearance].explanation));
        } else if (M_AP_TYPE(mon) == M_AP_OBJECT
                   /* remembered glyph, not glyph_at() which is 'mon' */
                   && glyph_is_object(glyph)) {
 objfrommap:
            otmp = (struct obj *) 0;
            fakeobj = object_from_map(glyph, x, y, &otmp);
            Strcat(outbuf, (otmp && otmp->otyp != STRANGE_OBJECT)
                              ? ansimpleoname(otmp)
                              : an(obj_descr[STRANGE_OBJECT].oc_name));
            if (fakeobj && otmp) {
                debugprint("mhidden_description");
                otmp->where = OBJ_FREE; /* object_from_map set to OBJ_FLOOR */
                dealloc_obj(otmp);
            }
        } else {
            Strcat(outbuf, something);
        }
    } else if (M_AP_TYPE(mon) == M_AP_MONSTER) {
        if (altmon)
            Sprintf(outbuf, ", masquerading as %s",
                    an(pm_monster_name(&mons[mon->mappearance], is_mon_female(mon))));
    } else if (isyou ? u.uundetected : is_mon_mundetected(mon)) {
        Strcpy(outbuf, ", hiding");
        if (hides_under(mon->data)) {
            Strcat(outbuf, " under ");
            /* remembered glyph, not glyph_at() which is 'mon' */
            if (glyph_is_object(glyph))
                goto objfrommap;
            Strcat(outbuf, something);
        } else if (is_hider(mon->data)) {
            Sprintf(eos(outbuf), " on the %s",
                    (is_flyer(mon->data) || mon->data->mlet == S_PIERCER)
                       ? "ceiling"
                       : surface(x, y)); /* trapper */
        } else {
            if (mon->data->mlet == S_EEL && is_pool(x, y))
                Strcat(outbuf, " in murky water");
        }
    }
}

/* extracted from lookat(); also used by namefloorobj() */
boolean
object_from_map(int glyph, int x, int y, struct obj **obj_p)
{
    boolean fakeobj = FALSE, mimic_obj = FALSE;
    struct monst *mtmp;
    struct obj *otmp;

    *obj_p = (struct obj *) 0;
    if (!isok(x, y))
        return FALSE;

    int glyphotyp = glyph_to_otyp(glyph);
    if (glyphotyp < STRANGE_OBJECT || glyphotyp >= NUM_OBJECTS)
        return FALSE;

    otmp = any_obj_at(glyphotyp, x, y);

    /* there might be a mimic here posing as an object */
    mtmp = m_at(x, y);
    if (mtmp && is_obj_mappear(mtmp, (unsigned) glyphotyp)) 
    {
        if (has_mobj(mtmp))
        {
            otmp = MOBJ(mtmp);
            otmp->ox = x;
            otmp->oy = y;
            //otmp->where = OBJ_FLOOR;
        }
        else
            otmp = 0;
        mimic_obj = TRUE;
    } 
    else
        mtmp = 0;

    if (!otmp || otmp->otyp != glyphotyp) 
    {
        /* this used to exclude STRANGE_OBJECT; now caller deals with it */
        if (!OBJ_NAME(objects[glyphotyp])) /* Hallucinated object, one of the extra name scrolls */
        {
            if (objects[glyphotyp].oc_class > ILLOBJ_CLASS && objects[glyphotyp].oc_class < MAX_OBJECT_CLASSES)
                otmp = mkobj_with_flags(objects[glyphotyp].oc_class, FALSE, FALSE, 0, (struct monst*)0, MAT_NONE, 0L, 0L, 0UL);
            else /* Something strange is going on */
                return FALSE;
        }
        else
            otmp = mksobj(glyphotyp, FALSE, FALSE, FALSE);

        if (!otmp || !OBJ_NAME(objects[otmp->otyp]))
            return FALSE;

        fakeobj = TRUE;
        if (otmp->oclass == COIN_CLASS)
            otmp->quan = 2L; /* to force pluralization */
        else if (otmp->otyp == SLIME_MOLD)
            otmp->special_quality = context.current_fruit; /* give it a type */
        else if (otmp->otyp == EGG)
            otmp->corpsenm = LOW_PM; /* make sure it is not NON_PM */
        
        if (mtmp && has_mcorpsenm(mtmp) && is_obj_mappear(mtmp, (unsigned)otmp->otyp)) /* mimic as corpse/statue */
            otmp->corpsenm = MCORPSENM(mtmp);
        else if (mtmp && has_mobj(mtmp) && MOBJ(mtmp)->otyp == otmp->otyp) /* mimic as corpse/statue via MOBJ; goes here only if otmp->otyp != glyphotyp but still is_obj_mappear(mtmp, (unsigned) glyphotyp) == TRUE above, which indicates that things are not right */
        {
            otmp->quan = MOBJ(mtmp)->quan;
            otmp->corpsenm = MOBJ(mtmp)->corpsenm;
        }
        else if (otmp->otyp == CORPSE && glyph_is_body(glyph))
        {
            otmp->corpsenm = min(NUM_MONSTERS - 1, max(0, abs(glyph) - GLYPH_BODY_OFF) + LOW_PM);
            otmp->speflags |= SPEFLAGS_MALE;
            if (glyph < 0)
                otmp->speflags |= SPEFLAGS_FACING_RIGHT;
        }
        else if (otmp->otyp == CORPSE && glyph_is_female_body(glyph))
        {
            otmp->corpsenm = min(NUM_MONSTERS - 1, max(0, abs(glyph) - GLYPH_FEMALE_BODY_OFF) + LOW_PM);
            otmp->speflags |= SPEFLAGS_FEMALE;
            if (glyph < 0)
                otmp->speflags |= SPEFLAGS_FACING_RIGHT;
        }
        else if (otmp->otyp == STATUE && glyph_is_statue(glyph))
        {
            otmp->corpsenm = min(NUM_MONSTERS - 1, max(0, abs(glyph) - GLYPH_STATUE_OFF) + LOW_PM);
            otmp->speflags |= SPEFLAGS_MALE;
            if (glyph < 0)
                otmp->speflags |= SPEFLAGS_FACING_RIGHT;
        }
        else if (otmp->otyp == STATUE && glyph_is_female_statue(glyph))
        {
            otmp->corpsenm = min(NUM_MONSTERS - 1, max(0, abs(glyph) - GLYPH_FEMALE_STATUE_OFF) + LOW_PM);
            otmp->speflags |= SPEFLAGS_FEMALE;
            if(glyph < 0)
                otmp->speflags |= SPEFLAGS_FACING_RIGHT;
        }

        if (otmp->otyp == LEASH)
            otmp->leashmon = 0;
        /* extra fields needed for shop price with doname() formatting */
        otmp->where = OBJ_FLOOR;
        otmp->ox = x, otmp->oy = y;
        set_obj_no_charge(otmp, (otmp->otyp == STRANGE_OBJECT && costly_spot(x, y)));
    }

    if (otmp && (otmp->otyp == CORPSE || otmp->otyp == STATUE || otmp->otyp == FIGURINE) && (otmp->corpsenm <= NON_PM || otmp->corpsenm >= NUM_MONSTERS)) /* Insurance */
        otmp->corpsenm = LOW_PM;

    /* if located at adjacent spot, mark it as having been seen up close
       (corpse type will be known even if dknown is 0, so we don't need a
       touch check for cockatrice corpse--we're looking without touching) */
    if (otmp && distu(x, y) <= 2 && !Blind && !Hallucination
        /* redundant: we only look for an object which matches current
           glyph among floor and buried objects; when !Blind, any buried
           object's glyph will have been replaced by whatever is present
           on the surface as soon as we moved next to its spot */
        && (fakeobj || otmp->where == OBJ_FLOOR) /* not buried */
        /* terrain mode views what's already known, doesn't learn new stuff */
        && !iflags.terrainmode) /* so don't set dknown when in terrain mode */
        set_obj_dknown(otmp, 1); /* if a pile, clearly see the top item only */

    if (fakeobj && mtmp && mimic_obj &&
        (is_obj_dknown(otmp) || (M_AP_FLAG(mtmp) & M_AP_F_DKNOWN))) 
    {
            mtmp->m_ap_type |= M_AP_F_DKNOWN;
            set_obj_dknown(otmp, 1);
    }
    *obj_p = otmp;
    return fakeobj; /* when True, caller needs to dealloc *obj_p */
}

/*
 * Parameters:
 *   buf: output buffer
 */
static void
look_at_object(char *buf, int x, int y, int glyph)
{
    if (!isok(x, y))
        return;

    struct obj *otmp = 0;
    boolean fakeobj = object_from_map(glyph, x, y, &otmp);

    if (otmp)
    {
        debugprint("look_at_object: otyp=%d, hallu=%d, fakeobj=%d, hasname=%d, hasdescr=%d, dump=%d, dknown=%d", 
            otmp->otyp, Hallucination, (int)fakeobj, 
            otmp->otyp > STRANGE_OBJECT && otmp->otyp < NUM_OBJECTS ? OBJ_NAME(objects[otmp->otyp]) != 0 : -1, 
            otmp->otyp > STRANGE_OBJECT && otmp->otyp < NUM_OBJECTS ? OBJ_DESCR(objects[otmp->otyp]) != 0 : -1,
            (int)iflags.in_dumplog, (int)is_obj_dknown(otmp));
        const char* used_obj_name = (otmp->otyp > STRANGE_OBJECT && otmp->otyp < NUM_OBJECTS && OBJ_NAME(objects[otmp->otyp]))
            ? (iflags.in_dumplog ? aqcxname(otmp) : distant_name(otmp, is_obj_dknown(otmp) ? doname_with_price : doname_vague_quan))
            : obj_descr[STRANGE_OBJECT].oc_name;

        Strcpy(buf, used_obj_name ? used_obj_name : "indescribable object");

        if (fakeobj) 
        {
            otmp->where = OBJ_FREE; /* object_from_map set it to OBJ_FLOOR */
            dealloc_obj(otmp), otmp = 0;
        }
    } 
    else
        Strcpy(buf, something); /* sanity precaution */

    if (otmp && otmp->where == OBJ_BURIED)
        Strcat(buf, " (buried)");
    else if (levl[x][y].typ == STONE || levl[x][y].typ == SCORR)
        Strcat(buf, " embedded in stone");
    else if (IS_WALL_OR_SDOOR(levl[x][y].typ))
        Strcat(buf, " embedded in a wall");
    else if (closed_door(x, y))
        Strcat(buf, " embedded in a door");
    else if (is_pool(x, y))
        Strcat(buf, " in water");
    else if (is_lava(x, y))
        Strcat(buf, " in molten lava"); /* [can this ever happen?] */
    return;
}

/*
 * Parameters:
 *   buf, simplebuf, extrabuf: buf: output, simplebuf: for searching encyclopedia, extrabuf: optional output
 */
static void
look_at_monster(char *buf, char *simplebuf, char *extrabuf, struct monst *mtmp, int x, int y)
{
    char *name, monnambuf[BUFSZ], headbuf[BUFSZ], tmpbuf[BUFSZ];
    struct permonst *pm = (M_AP_TYPE(mtmp) == M_AP_MONSTER && mtmp->mappearance < NUM_MONSTERS && !Can_detect_mimic(mtmp)) ? &mons[mtmp->mappearance] : mtmp->data;
    boolean accurate = !Hallucination;
    boolean show_monster_type = accurate && (is_mon_isshk(mtmp) || has_umname(mtmp) || (has_mname(mtmp) && is_mon_u_know_mname(mtmp))) && !is_mname_proper_name(pm);

    name = (pm == &mons[PM_COYOTE] && accurate)
              ? coyotename(mtmp, monnambuf)
              : distant_monnam(mtmp, ARTICLE_NONE, monnambuf);

    if (simplebuf)
    {
        if (show_monster_type)
            Strcpy(simplebuf, pm_monster_name(pm, is_mon_female(mtmp)));
        else
            Strcpy(simplebuf, name);
    }

    Strcpy(headbuf, "");

    if ((pm->heads > 3 && !(pm->geno & G_UNIQ) && !is_mname_proper_name(pm)) || (pm->heads > 1 && mtmp->heads_left != pm->heads))
        Sprintf(headbuf, "%d-headed ", mtmp->heads_left);

    Strcpy(tmpbuf, "");
    Sprintf(tmpbuf, "%s%s%s",
        (is_tame(mtmp) && accurate)
        ? (call_mon_tame(mtmp) ? "tame " : "allied ")
        : (is_peaceful(mtmp) && accurate)
        ? "peaceful "
        : "",
        headbuf,
        name);

    Sprintf(buf, "level %d %s%s", accurate ? pm->difficulty : rn2(3) ? rnd(30) : rnd(80),
            (mtmp->mx != x || mtmp->my != y)
                ? "tail of "
                : "",
        (mtmp->mx != x || mtmp->my != y) && !((is_mon_isshk(mtmp) || has_umname(mtmp) || (has_mname(mtmp) && is_mon_u_know_mname(mtmp)) || (is_mplayer(pm) && strstri(name, " the ") != 0) || is_mname_proper_name(pm)) && accurate) ? an(tmpbuf) : tmpbuf);

    if (show_monster_type)
        Sprintf(eos(buf), ", %s", an(pm_monster_name(pm, is_mon_female(mtmp))));

    if (u.ustuck == mtmp) {
        if (u.uswallow || iflags.save_uswallow) /* monster detection */
            Strcat(buf, is_animal(pm)
                          ? ", swallowing you" : ", engulfing you");
        else
            Strcat(buf, (Upolyd && sticks(youmonst.data))
                          ? ", being held" : ", holding you");
    }
    if (is_mon_mleashed(mtmp))
        Strcat(buf, ", leashed to you");

    if (is_mon_mtrapped(mtmp) && cansee(mtmp->mx, mtmp->my)) {
        struct trap *t = t_at(mtmp->mx, mtmp->my);
        int tt = t ? t->ttyp : NO_TRAP;

        /* newsym lets you know of the trap, so mention it here */
        if (tt == BEAR_TRAP || is_pit(tt) || tt == WEB)
            Sprintf(eos(buf), ", trapped in %s",
                    an(get_trap_explanation(t)));
    }

    /* we know the hero sees a monster at this location, but if it's shown
       due to persistant monster detection he might remember something else */
    if (is_mon_mundetected(mtmp) || M_AP_TYPE(mtmp))
        mhidden_description(mtmp, FALSE, eos(buf));

    if (is_tame(mtmp))
    {
        Sprintf(eos(buf), ", %d/%d HP", mtmp->mhp, mtmp->mhpmax);
        if (has_edog(mtmp))
        {
            if (monstermoves >= EDOG(mtmp)->hungrytime + 500)
                Sprintf(eos(buf), ", starving");
            else if (monstermoves >= EDOG(mtmp)->hungrytime)
                    Sprintf(eos(buf), ", hungry");
        }
    }
    if (extrabuf) {
        unsigned how_seen = howmonseen(mtmp);

        extrabuf[0] = '\0';
        if (how_seen != 0 && how_seen != MONSEEN_NORMAL) {
            if (how_seen & MONSEEN_NORMAL) {
                Strcat(extrabuf, "normal vision");
                how_seen &= ~MONSEEN_NORMAL;
                /* how_seen can't be 0 yet... */
                if (how_seen)
                    Strcat(extrabuf, ", ");
            }
            if (how_seen & MONSEEN_SEEINVIS) {
                Strcat(extrabuf, "see invisible");
                how_seen &= ~MONSEEN_SEEINVIS;
                if (how_seen)
                    Strcat(extrabuf, ", ");
            }
            if (how_seen & MONSEEN_INFRAVIS) {
                Strcat(extrabuf, "infravision");
                how_seen &= ~MONSEEN_INFRAVIS;
                if (how_seen)
                    Strcat(extrabuf, ", ");
            }
            if (how_seen & MONSEEN_TELEPAT) {
                Strcat(extrabuf, "telepathy");
                how_seen &= ~MONSEEN_TELEPAT;
                if (how_seen)
                    Strcat(extrabuf, ", ");
            }
            if (how_seen & MONSEEN_XRAYVIS) {
                /* Eyes of the Overworld */
                Strcat(extrabuf, "X-ray vision");
                how_seen &= ~MONSEEN_XRAYVIS;
                if (how_seen)
                    Strcat(extrabuf, ", ");
            }
            if (how_seen & MONSEEN_ASTRALVIS) {
                /* Eyes of the Overworld */
                Strcat(extrabuf, "astral vision");
                how_seen &= ~MONSEEN_ASTRALVIS;
                if (how_seen)
                    Strcat(extrabuf, ", ");
            }
            if (how_seen & MONSEEN_DETECT) {
                Strcat(extrabuf, "monster detection");
                how_seen &= ~MONSEEN_DETECT;
                if (how_seen)
                    Strcat(extrabuf, ", ");
            }
            if (how_seen & MONSEEN_WARNMON) {
                if (Hallucination) {
                    Strcat(extrabuf, "paranoid delusion");
                } else {
                    uint64_t mW = (context.warntype.obj
                                        | context.warntype.polyd),
                                  m2 = pm->mflags2;
                    const char *whom = ((mW & M2_HUMAN & m2) ? "humans"
                                        : (mW & M2_ELF & m2) ? "elves"
                                          : (mW & M2_ORC & m2) ? "orcs"
                                          : (mW & M2_ANGEL & m2) ? "angels"
                                            : (mW & M2_DEMON & m2) ? "demons"
                                              : mon_common_name(mtmp));

                    Sprintf(eos(extrabuf), "warned of %s", makeplural(whom));
                }
                how_seen &= ~MONSEEN_WARNMON;
                if (how_seen)
                    Strcat(extrabuf, ", ");
            }
            /* should have used up all the how_seen bits by now */
            if (how_seen) {
                impossible("lookat: unknown method of seeing monster");
                Sprintf(eos(extrabuf), "(%u)", how_seen);
            }
        } /* seen by something other than normal vision */
    } /* extrabuf is non-null */
}

static
void print_decoration_ending(char *buf, int x, int y)
{
    if (levl[x][y].decoration_typ > 0 && (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_ADD_OTHER_ITEM_DESCRIPTIONS) != 0)
    {
        const char* dec2 = 0;
        const char* dec3 = 0;
        if (decoration_type_definitions[levl[x][y].decoration_typ].lootable_item2 != STRANGE_OBJECT && (levl[x][y].decoration_flags & DECORATION_FLAGS_ITEM2_IN_HOLDER) != 0)
        {
            dec2 = OBJ_NAME(objects[decoration_type_definitions[levl[x][y].decoration_typ].lootable_item2]);
        }
        if (decoration_type_definitions[levl[x][y].decoration_typ].lootable_item3 != STRANGE_OBJECT && (levl[x][y].decoration_flags & DECORATION_FLAGS_ITEM3_IN_HOLDER) != 0)
        {
            dec3 = OBJ_NAME(objects[decoration_type_definitions[levl[x][y].decoration_typ].lootable_item3]);
        }
        boolean same_item = dec2 && dec3 && !strcmp(dec2, dec3);
        if (same_item)
        {
            Sprintf(eos(buf), " with two %s", makeplural(dec2));
        }
        else if (dec2 && dec3)
        {
            Sprintf(eos(buf), " with %s and %s", an(dec2), an(dec3));
        }
        else if (dec2 || dec3)
        {
            Sprintf(eos(buf), " with %s", an(dec2 ? dec2 : dec3));
        }
    }
}

/*
 * Return the name of the glyph found at (x,y).
 * If not hallucinating and the glyph is a monster, also monster data.
 */
static struct permonst *
lookat(int x, int y, char *buf, char *simplebuf, char *extrabuf)
{
    if (!buf || !simplebuf || !extrabuf)
        return (struct permonst*)0;

    struct monst* mtmp = (struct monst*)0;
    struct permonst* pm = (struct permonst*)0;
    int glyph;
    boolean noarticle = FALSE;

    buf[0] = simplebuf[0] = extrabuf[0] = '\0';
    if (!isok(x, y))
        return (struct permonst*)0;

    glyph = glyph_at(x, y);
    if (u.ux == x && u.uy == y && canspotself()
        && !(iflags.save_uswallow &&
            glyph == any_mon_to_glyph(u.ustuck, rn2_on_display_rng))
        && (!iflags.terrainmode || (iflags.terrainmode & TER_MON) != 0))
    {
        /* fill in buf[] */
        (void)self_lookat(buf);

        /* file lookup can't distinguish between "gnomish wizard" monster
           and correspondingly named player character, always picking the
           former; force it to find the general "wizard" entry instead */
        if (Role_if(PM_WIZARD) && Race_if(PM_GNOME) && !Upolyd)
            pm = &mons[PM_WIZARD];

        /* When you see yourself normally, no explanation is appended
           (even if you could also see yourself via other means).
           Sensing self while blind or swallowed is treated as if it
           were by normal vision (cf canseeself()). */
        if ((Invisib || u.uundetected) && !Blind
            && !(u.uswallow || iflags.save_uswallow))
        {
            unsigned how = 0;

            if (Infravision)
                how |= 1;
            if (Unblind_telepat)
                how |= 2;
            if (Detect_monsters)
                how |= 4;

            if (how)
                Sprintf(eos(buf), " [seen: %s%s%s%s%s]",
                    (how & 1) ? "infravision" : "",
                    /* add comma if telep and infrav */
                    ((how & 3) > 2) ? ", " : "",
                    (how & 2) ? "telepathy" : "",
                    /* add comma if detect and (infrav or telep or both) */
                    ((how & 7) > 4) ? ", " : "",
                    (how & 4) ? "monster detection" : "");
        }
    }
    else if (u.uswallow)
    {
        /* when swallowed, we're only called for spots adjacent to hero,
           and blindness doesn't prevent hero from feeling what holds him */
        Sprintf(buf, "interior of %s", a_monnam(u.ustuck));
        pm = u.ustuck->data;
    }
    else if (glyph_is_monster(glyph))
    {
        bhitpos.x = x;
        bhitpos.y = y;
        if ((mtmp = m_at(x, y)) != 0)
        {
            if (is_tame(mtmp))
            {
                print_mstatusline(buf, mtmp, ARTICLE_NONE, TRUE);
                Strcpy(simplebuf, pm_monster_name(mtmp->data, is_mon_female(mtmp)));
            }
            else
            {
                look_at_monster(buf, simplebuf, extrabuf, mtmp, x, y);
            }
            pm = (M_AP_TYPE(mtmp) == M_AP_MONSTER && mtmp->mappearance < NUM_MONSTERS && !Can_detect_mimic(mtmp)) ? &mons[mtmp->mappearance] : mtmp->data;
            if(has_umname(mtmp) || (has_mname(mtmp) && is_mon_u_know_mname(mtmp)))
                noarticle = TRUE;
        }
        else if (Hallucination)
        {
            /* 'monster' must actually be a statue */
            Strcpy(buf, rndmonnam((char*)0));
        }
    }
    else if (glyph_is_object(glyph))
    {
        noarticle = TRUE;
        look_at_object(buf, x, y, glyph); /* fill in buf[] */
    }
    else if (glyph_is_trap(glyph))
    {
        int tnum = what_trap(glyph_to_trap(glyph), rn2_on_display_rng);
        int tsubtyp = 0;
        if (glyph_is_cmap_variation(glyph) && !Hallucination)
            tsubtyp = max(0, min(defsyms[trap_to_defsym(tnum)].variations, glyph_to_cmap_variation(glyph) - defsyms[trap_to_defsym(tnum)].variation_offset + 1));

        /* Trap detection displays a bear trap at locations having
         * a trapped door or trapped container or both.
         * TODO: we should create actual trap types for doors and
         * chests so that they can have their own glyphs and tiles.
         */
        if (trapped_chest_at(tnum, x, y))
            Strcpy(buf, "trapped chest"); /* might actually be a large box */
        else if (trapped_door_at(tnum, x, y))
            Strcpy(buf, "trapped door"); /* not "trap door"... */
        else
            Strcpy(buf, tsubtyp ? defsym_variations[max(0, tsubtyp - 1 + defsyms[trap_to_defsym(tnum)].variation_offset)].explanation : defsyms[trap_to_defsym(tnum)].explanation);

        if (tnum == LEVER && !Hallucination)
        {
            struct trap* ttmp = t_at(x, y);
            if (ttmp && ttmp->ttyp == LEVER && (ttmp->tflags & TRAPFLAGS_SWITCHABLE_BETWEEN_STATES))
            {
                if ((ttmp->tflags & TRAPFLAGS_STATE_MASK) > 0UL)
                    Sprintf(eos(buf), " turned left");
                else
                    Sprintf(eos(buf), " turned right");
            }
        }

    }
    else if (glyph_is_warning(glyph))
    {
        int warnindx = glyph_to_warning(glyph);

        Strcpy(buf, def_warnsyms[warnindx].explanation);
    }
    else if (!glyph_is_cmap_or_cmap_variation(glyph))
    {
        Strcpy(buf, "unexplored area");
    }
    else
    {
        int gl = generic_glyph_to_cmap(glyph);
        switch (gl)
        {
        case S_altar:
            if(levl[x][y].subtyp == ALTAR_SUBTYPE_MOLOCH)
                Sprintf(buf, "%saltar to %s",
                    ((levl[x][y].altarmask & AM_SHRINE)
                        && (Is_astralevel(&u.uz) || Is_sanctum(&u.uz)))
                    ? "high "
                    : "" , Moloch);
            else if (levl[x][y].subtyp == ALTAR_SUBTYPE_HIGH)
                Sprintf(buf, "%s %saltar",
                    /* like endgame high priests, endgame high altars
                       are only recognizable when immediately adjacent */
                    (Is_astralevel(&u.uz) && distu(x, y) > 2)
                    ? "aligned"
                    : align_str(Amask2align(levl[x][y].altarmask & ~AM_SHRINE)),
                    "high ");
            else
                Sprintf(buf, "%s %saltar",
                    /* like endgame high priests, endgame high altars
                       are only recognizable when immediately adjacent */
                    (Is_astralevel(&u.uz) && distu(x, y) > 2)
                    ? "aligned"
                    : align_str(Amask2align(levl[x][y].altarmask & ~AM_SHRINE)),
                    ((levl[x][y].altarmask & AM_SHRINE)
                        && (Is_astralevel(&u.uz) || Is_sanctum(&u.uz)))
                    ? "high "
                    : "");
            break;
        case S_ndoor:
            if (is_drawbridge_wall(x, y) >= 0)
                Strcpy(buf, "open drawbridge portcullis");
            else if (((levl[x][y].doormask & ~D_TRAPPED) & D_MASK) == D_BROKEN)
                Strcpy(buf, "broken door");
            else
                Strcpy(buf, "doorway");
            break;
        case S_cloud:
            Strcpy(buf, Is_airlevel(&u.uz) ? "cloudy area" : "fog/vapor cloud");
            break;
        case S_fountain:
            Strcpy(buf, get_fountain_name(x, y));
            break;
        case S_unexplored:
            noarticle = TRUE;
            Strcpy(buf, "unexplored");
            break;
        case S_stone:
            noarticle = TRUE;
            if (!levl[x][y].seenv)
            {
                Strcpy(buf, "unexplored stone");
                break;
            }
            else if (Underwater && !Is_waterlevel(&u.uz))
            {
                /* "unknown" == previously mapped but not visible when
                   submerged; better terminology appreciated... */
                Strcpy(buf, (distu(x, y) <= 2) ? "land" : "unknown");
                break;
            }
            else if (levl[x][y].typ == STONE || levl[x][y].typ == SCORR)
            {
                Strcpy(buf, "stone");
                break;
            }
            /*FALLTHRU*/
        default:
        {
            //int cmap = generic_glyph_to_cmap(glyph);

            const char* explanation = iflags.using_gui_tiles ? get_floor_doodad_explanation_at(x, y) : (const char*)0;
            if (!explanation)
                explanation = get_cmap_or_cmap_variation_glyph_explanation(glyph);

            const char* dec_descr = 0;
            const char* carpet_descr = 0;
            if (explanation && *explanation)
            {
                carpet_descr = get_carpet_description(x, y);
                dec_descr = get_decoration_description(x, y);
                if (dec_descr && *dec_descr && carpet_descr && *carpet_descr)
                    Sprintf(buf, "%s on %s", dec_descr, an(carpet_descr));
                else if (dec_descr && *dec_descr)
                {
                    char dbuf[BUFSZ];
                    Strcpy(dbuf, dec_descr);
                    print_decoration_ending(dbuf, x, y);
                    Sprintf(buf, "%s on %s", dbuf, an(explanation));
                }
                else if (carpet_descr && *carpet_descr && strstr(explanation, "floor"))
                    Sprintf(buf, "%s on %s", carpet_descr, an(explanation));
                else
                    Strcpy(buf, explanation); // defsyms[cmap].explanation);
            }

            if (is_cmap_door(gl) && IS_DOOR(levl[x][y].typ))
            {
                print_lock_with_buf(eos(buf), levl[x][y].key_otyp, levl[x][y].special_quality, is_door_normally_without_lock_at(x, y));
            }
            if ((
                (is_cmap_brazier(gl) && IS_BRAZIER(levl[x][y].typ)) || 
                    (dec_descr && levl[x][y].decoration_typ > 0 
                        && (decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIGHTABLE) != 0
                        && ((decoration_type_definitions[levl[x][y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LOOTABLE) == 0 || (levl[x][y].decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0)
                    )
                ) && cansee(x, y))
            {
                char buf2[BUFSZ * 2];
                Sprintf(buf2, "%s%s", is_levl_lamplit(&levl[x][y]) ? "lit " : "unlit ", buf);
                Strcpy(buf, buf2);
            }
            break;
        }
        }
    }

    char exbuf[BUFSZ * 2];
    Strcpy(exbuf, buf);
    int article = strstri(exbuf, " of a room")? 2 :
        (!noarticle && pm && (pm->geno & G_UNIQ)) ? (is_mname_proper_name(pm) ? 0 : 2) : /* for unique monsters have no article if the name is a proper name, otherwise they have the */
        !(noarticle == TRUE
            || strcmp(exbuf, "air") == 0
            || strcmp(exbuf, "land") == 0
            || strcmp(exbuf, "water") == 0);

    Strcpy(buf, article == 2 ? the(exbuf)
        : article == 1 ? an(exbuf) : exbuf);
    
    if (!*simplebuf)
        Strcpy(simplebuf, buf);
    return (pm && !Hallucination) ? pm : (struct permonst *) 0;
}

/*
 * Look in the "data" file for more info.  Called if the user typed in the
 * whole name (user_typed_name == TRUE), or we've found a possible match
 * with a character/glyph and flags.help is TRUE.
 *
 * NOTE: when (user_typed_name == FALSE), inp is considered read-only and
 *       must not be changed directly, e.g. via lcase(). We want to force
 *       lcase() for data.base lookup so that we can have a clean key.
 *       Therefore, we create a copy of inp _just_ for data.base lookup.
 */
static void
checkfile(char *inp, struct permonst *pm, boolean user_typed_name, boolean without_asking, char *supplemental_name)
{
    dlb *fp;
    char buf[BUFSZ], newstr[BUFSZ], givenname[BUFSZ];
    char *ep, *dbase_str;
    long txt_offset = 0L;
    winid datawin = WIN_ERR;

    fp = dlb_fopen(DATAFILE, "r");
    if (!fp) {
        pline("Cannot open 'data' file!");
        return;
    }
    /* If someone passed us garbage, prevent fault. */
    if (!inp || strlen(inp) > (BUFSZ - 1)) {
        impossible("bad do_look buffer passed (%s)!",
                   !inp ? "null" : "too long");
        goto checkfile_done;
    }

    /* To prevent the need for entries in data.base like *ngel to account
     * for Angel and angel, make the lookup string the same for both
     * user_typed_name and picked name.
     */
    if (pm != (struct permonst *) 0 && !user_typed_name)
        dbase_str = strcpy(newstr, pm->mname);
    else
        dbase_str = strcpy(newstr, inp);
    (void) lcase(dbase_str);

    /*
     * TODO:
     * The switch from xname() to doname_vague_quan() in look_at_obj()
     * had the unintendded side-effect of making names picked from
     * pointing at map objects become harder to simplify for lookup.
     * We should split the prefix and suffix handling used by wish
     * parsing and also wizmode monster generation out into separate
     * routines and use those routines here.  This currently lacks
     * erosion handling and probably lots of other bits and pieces
     * that wishing already understands and most of this duplicates
     * stuff already done for wish handling or monster generation.
     */
    if (!strncmp(dbase_str, "interior of ", 12))
        dbase_str += 12;
    if (!strncmp(dbase_str, "a ", 2))
        dbase_str += 2;
    else if (!strncmp(dbase_str, "an ", 3))
        dbase_str += 3;
    else if (!strncmp(dbase_str, "the ", 4))
        dbase_str += 4;
    else if (!strncmp(dbase_str, "some ", 5))
        dbase_str += 5;
    else if (digit(*dbase_str)) {
        /* remove count prefix ("2 ya") which can come from looking at map */
        while (digit(*dbase_str))
            ++dbase_str;
        if (*dbase_str == ' ')
            ++dbase_str;
    }
    if (!strncmp(dbase_str, "pair of ", 8))
        dbase_str += 8;
    if (!strncmp(dbase_str, "tame ", 5))
        dbase_str += 5;
    else if (!strncmp(dbase_str, "allied ", 7))
        dbase_str += 7;
    else if (!strncmp(dbase_str, "peaceful ", 9))
        dbase_str += 9;
    if (!strncmp(dbase_str, "invisible ", 10))
        dbase_str += 10;
    if (!strncmp(dbase_str, "saddled ", 8))
        dbase_str += 8;
    if (!strncmp(dbase_str, "blessed ", 8))
        dbase_str += 8;
    else if (!strncmp(dbase_str, "uncursed ", 9))
        dbase_str += 9;
    else if (!strncmp(dbase_str, "cursed ", 7))
        dbase_str += 7;
    if (!strncmp(dbase_str, "empty ", 6))
        dbase_str += 6;
    if (!strncmp(dbase_str, "partly used ", 12))
        dbase_str += 12;
    else if (!strncmp(dbase_str, "partly eaten ", 13))
        dbase_str += 13;
    if (!strncmp(dbase_str, "statue of ", 10))
        dbase_str[6] = '\0';
    else if (!strncmp(dbase_str, "figurine of ", 12))
        dbase_str[8] = '\0';
    /* remove enchantment ("+0 aklys"); [for 3.6.0 and earlier, this wasn't
       needed because looking at items on the map used xname() rather than
       doname() hence known enchantment was implicitly suppressed] */
    if (*dbase_str && index("+-", dbase_str[0]) && digit(dbase_str[1])) {
        ++dbase_str; /* skip sign */
        while (digit(*dbase_str))
            ++dbase_str;
        if (*dbase_str == ' ')
            ++dbase_str;
    }
    /* "towel", "wet towel", and "moist towel" share one data.base entry;
       for "wet towel", we keep prefix so that the prompt will ask about
       "wet towel"; for "moist towel", we also want to ask about "wet towel".
       (note: strncpy() only terminates output string if the specified
       count is bigger than the length of the substring being copied) */
    if (!strncmp(dbase_str, "moist towel", 11))
        Strncpy(dbase_str += 2, "wet", 3); /* skip "mo" replace "ist" */

    /* Make sure the name is non-empty. */
    if (*dbase_str) {
        long pass1offset = -1L;
        int chk_skip, pass = 1;
        boolean yes_to_moreinfo, found_in_file, pass1found_in_file,
                skipping_entry;
        char *sp, *ap, *alt = 0; /* alternate description */

        /* adjust the input to remove "named " and "called " */
        if ((ep = strstri(dbase_str, " named ")) != 0) {
            alt = ep + 7;
            if ((ap = strstri(dbase_str, " called ")) != 0 && ap < ep)
                ep = ap; /* "named" is alt but truncate at "called" */
        } 
        else if ((ep = strstri(dbase_str, " entitled ")) != 0) {
            alt = ep + 10;
            if ((ap = strstri(dbase_str, " called ")) != 0 && ap < ep)
                ep = ap; /* "named" is alt but truncate at "called" */
        }
        else if ((ep = strstri(dbase_str, " labeled ")) != 0) {
            alt = ep + 9;
            if ((ap = strstri(dbase_str, " called ")) != 0 && ap < ep)
                ep = ap; /* "named" is alt but truncate at "called" */
        }
        else if ((ep = strstri(dbase_str, " called ")) != 0) {
            copynchars(givenname, ep + 8, BUFSZ - 1);
            alt = givenname;
            if (supplemental_name && (sp = strstri(inp, " called ")) != 0)
                copynchars(supplemental_name, sp + 8, BUFSZ - 1);
        } else
            ep = strstri(dbase_str, ", ");
        if (ep && ep > dbase_str)
            *ep = '\0';
        /* remove article from 'alt' name ("a pair of lenses named
           The Eyes of the Overworld" simplified above to "lenses named
           The Eyes of the Overworld", now reduced to "The Eyes of the
           Overworld", skip "The" as with base name processing) */
        if (alt && (!strncmpi(alt, "a ", 2)
                    || !strncmpi(alt, "an ", 3)
                    || !strncmpi(alt, "the ", 4)))
            alt = index(alt, ' ') + 1;
        /* remove charges or "(lit)" or wizmode "(N aum)" */
        if ((ep = strstri(dbase_str, " (")) != 0 && ep > dbase_str)
            *ep = '\0';
        if (alt && (ap = strstri(alt, " (")) != 0 && ap > alt)
            *ap = '\0';

        /*
         * If the object is named, then the name is the alternate description;
         * otherwise, the result of makesingular() applied to the name is.
         * This isn't strictly optimal, but named objects of interest to the
         * user will usually be found under their name, rather than under
         * their object type, so looking for a singular form is pointless.
         */
        if (!alt)
            alt = makesingular(dbase_str);

        pass1found_in_file = FALSE;
        for (pass = !strcmp(alt, dbase_str) ? 0 : 1; pass >= 0; --pass) {
            found_in_file = skipping_entry = FALSE;
            txt_offset = 0L;
            if (dlb_fseek(fp, txt_offset, SEEK_SET) < 0 ) {
                impossible("can't get to start of 'data' file");
                goto checkfile_done;
            }
            /* skip first record; read second */
            if (!dlb_fgets(buf, BUFSZ, fp) || !dlb_fgets(buf, BUFSZ, fp)) {
                impossible("can't read 'data' file");
                goto checkfile_done;
            } else if (sscanf(buf, "%8lx\n", &txt_offset) < 1
                       || txt_offset == 0L)
                goto bad_data_file;

            /* look for the appropriate entry */
            while (dlb_fgets(buf, BUFSZ, fp)) {
                if (*buf == '.')
                    break; /* we passed last entry without success */

                if (digit(*buf)) {
                    /* a number indicates the end of current entry */
                    skipping_entry = FALSE;
                } else if (!skipping_entry) {
                    if (!(ep = index(buf, '\n')))
                        goto bad_data_file;
                    (void) strip_newline((ep > buf) ? ep - 1 : ep);
                    /* if we match a key that begins with "~", skip
                       this entry */
                    chk_skip = (*buf == '~') ? 1 : 0;
                    if ((pass == 0 && pmatch(&buf[chk_skip], dbase_str))
                        || (pass == 1 && alt && pmatch(&buf[chk_skip], alt))) {
                        if (chk_skip) {
                            skipping_entry = TRUE;
                            continue;
                        } else {
                            found_in_file = TRUE;
                            if (pass == 1)
                                pass1found_in_file = TRUE;
                            break;
                        }
                    }
                }
            }
            if (found_in_file) {
                long entry_offset, fseekoffset;
                int entry_count;
                int i;

                /* skip over other possible matches for the info */
                do {
                    if (!dlb_fgets(buf, BUFSZ, fp))
                        goto bad_data_file;
                } while (!digit(*buf));
                if (sscanf(buf, "%ld,%d\n", &entry_offset, &entry_count) < 2)
                    goto bad_data_file;
                fseekoffset = txt_offset + entry_offset;
                if (pass == 1)
                    pass1offset = fseekoffset;
                else if (fseekoffset == pass1offset)
                    goto checkfile_done;

                yes_to_moreinfo = FALSE;
                if (!user_typed_name && !without_asking) {
                    char *entrytext = pass ? alt : dbase_str;
                    char question[QBUFSZ];

                    Strcpy(question, "More info about \"");
                    /* +2 => length of "\"?" */
                    copynchars(eos(question), entrytext,
                               (int) (sizeof question - 1
                                      - (strlen(question) + 2)));
                    Strcat(question, "\"?");
                    if (yn_query(question) == 'y')
                        yes_to_moreinfo = TRUE;
                }

                if (user_typed_name || without_asking || yes_to_moreinfo) {
                    if (dlb_fseek(fp, fseekoffset, SEEK_SET) < 0) {
                        pline("? Seek error on 'data' file!");
                        goto checkfile_done;
                    }
                    datawin = create_nhwindow(NHW_MENU);
                    for (i = 0; i < entry_count; i++) {
                        if (!dlb_fgets(buf, BUFSZ, fp))
                            goto bad_data_file;
                        (void) strip_newline(buf);
                        if (index(buf + 1, '\t') != 0)
                            (void) tabexpand(buf + 1);
                        putstr(datawin, 0, buf + 1);
                    }
                    display_nhwindow(datawin, FALSE);
                    destroy_nhwindow(datawin), datawin = WIN_ERR;
                }
            } else if (user_typed_name && pass == 0 && !pass1found_in_file)
                pline("I don't have any information on those things.");
        }
    }
    goto checkfile_done; /* skip error feedback */

 bad_data_file:
    impossible("'data' file in wrong format or corrupted");
 checkfile_done:
    if (datawin != WIN_ERR)
        destroy_nhwindow(datawin);
    (void) dlb_fclose(fp);
    return;
}

int
do_screen_description(coord cc, boolean looked, nhsym sym, char *out_str, const char **firstmatch, struct permonst **for_supplement)
{
    static const char mon_interior[] = "the interior of a monster",
                      unreconnoitered[] = "unreconnoitered";
    static char look_buf[BUFSZ * 5] = "";
    static char simple_buf[BUFSZ * 2] = "";
    static char x_buf[BUFSZ * 2] = "";
    char prefix[BUFSZ * 2] = "";
    int i = 0, alt_i = 0, glyph = NO_GLYPH,
        skipped_venom = 0, found = 0; /* count of matching syms found */
    boolean hit_trap = FALSE, need_to_look = FALSE,
            submerged = (Underwater && !Is_waterlevel(&u.uz));
    const char *x_str = 0;

    if (iflags.using_gui_tiles)
    {
        Strcpy(prefix, "");
    }
    else
    {
        if (looked)
        {
            int oc = 0;
            uint64_t os = 0;
            struct layer_info layers = layers_at(cc.x, cc.y);
            glyph = abs(layers.glyph);
            /* Convert glyph at selected position to a symbol for use below. */
            (void)mapglyph(layers, &sym, &oc, &os, cc.x, cc.y);
            Sprintf(prefix, "%c - ", (char)sym);
            //Sprintf(prefix, "%s - ", encglyph(glyph));
        }
        else
            Sprintf(prefix, "%c - ", (char)sym);
    }

    /*
     * Check all the possibilities, saving all explanations in a buffer.
     * When all have been checked then the string is printed.
     */

    /*
     * Handle restricted vision range (limited to adjacent spots when
     * swallowed or underwater) cases first.
     *
     * 3.6.0 listed anywhere on map, other than self, as "interior
     * of a monster" when swallowed, and non-adjacent water or
     * non-water anywhere as "dark part of a room" when underwater.
     * "unreconnoitered" is an attempt to convey "even if you knew
     * what was there earlier, you don't know what is there in the
     * current circumstance".
     *
     * (Note: 'self' will always be visible when swallowed so we don't
     * need special swallow handling for <ux,uy>.
     * Another note: for '#terrain' without monsters, u.uswallow and
     * submerged will always both be False and skip this code.)
     */
    x_str = 0;
    if (!looked) 
    {
        ; /* skip special handling */
    }
    else if (((u.uswallow || submerged) && distu(cc.x, cc.y) > 2)
               /* detection showing some category, so mostly background */
               || ((iflags.terrainmode & (TER_DETECT | TER_MAP)) == TER_DETECT
                   && glyph_is_specific_cmap_or_its_variation(glyph, S_unexplored)))
    {
        x_str = unreconnoitered;
        need_to_look = FALSE;
    }
    else if (is_swallow_sym(sym))
    {
        x_str = mon_interior;
        need_to_look = TRUE; /* for specific monster type */
    }

    if (x_str)

    {
        /* we know 'found' is zero here, but guard against some other
           special case being inserted ahead of us someday */
        if (!found) 
        {
            Sprintf(out_str, "%s%s", prefix, x_str);
            *firstmatch = x_str;
            found++;
        }
        else 
        {
            found += append_str(out_str, x_str); /* not 'an(x_str)' */
        }

        /* for is_swallow_sym(), we want to list the current symbol's
           other possibilities (wand for '/', throne for '\\', &c) so
           don't jump to the end for the x_str==mon_interior case */
        if (x_str == unreconnoitered)
            goto didlook;
    }

    /* Check for monsters */
    if (!iflags.terrainmode || (iflags.terrainmode & TER_MON) != 0)
    {
        for (i = 1; i < MAX_MONSTER_CLASSES; i++)
        {
            if (sym == (looked ? showsyms[i + SYM_OFF_M] : def_monsyms[i].sym)
                && def_monsyms[i].explain && *def_monsyms[i].explain)
            {
                need_to_look = TRUE;
                if (!found) 
                {
                    Sprintf(out_str, "%s%s",
                            prefix, an(def_monsyms[i].explain));
                    *firstmatch = def_monsyms[i].explain;
                    found++;
                }
                else 
                {
                    found += append_str(out_str, an(def_monsyms[i].explain));
                }
            }
        }
        /* handle '@' as a special case if it refers to you and you're
           playing a character which isn't normally displayed by that
           symbol; firstmatch is assumed to already be set for '@' */
        if ((looked ? (sym == showsyms[S_HUMAN + SYM_OFF_M]
                       && cc.x == u.ux && cc.y == u.uy)
                    : (sym == def_monsyms[S_HUMAN].sym && !flags.showrace))
            && !(Race_if(PM_HUMAN) || Race_if(PM_ELF)) && !Upolyd)
            found += append_str(out_str, "you"); /* tack on "or you" */
    }

    /* Now check for objects */
    if (!iflags.terrainmode || (iflags.terrainmode & TER_OBJ) != 0) 
    {
        for (i = 1; i < MAX_OBJECT_CLASSES; i++)
        {
            if (sym == (looked ? showsyms[i + SYM_OFF_O]
                               : def_oc_syms[i].sym)
                || (looked && i == ROCK_CLASS && glyph_is_any_statue(glyph))) 
            {
                need_to_look = TRUE;
                if (looked && i == VENOM_CLASS
                    ) 
                {
                    skipped_venom++;
                    continue;
                }
                if (!found)
                {
                    Sprintf(out_str, "%s%s",
                            prefix, an(def_oc_syms[i].explain));
                    *firstmatch = def_oc_syms[i].explain;
                    found++;
                } 
                else 
                {
                    found += append_str(out_str, an(def_oc_syms[i].explain));
                }
            }
        }
    }

    if (sym == DEF_INVISIBLE) 
    {
        extern const char altinvisexplain[]; /* drawing.c */
        /* for active clairvoyance, use alternate "unseen creature" */
        boolean usealt = (EDetect_monsters & I_SPECIAL) != 0L;
        const char *unseen_explain = !usealt ? invisexplain : altinvisexplain;

        if (!found) 
        {
            Sprintf(out_str, "%s%s", prefix, an(unseen_explain));
            *firstmatch = unseen_explain;
            found++;
        }
        else 
        {
            found += append_str(out_str, an(unseen_explain));
        }
    }

    /* Now check for graphics symbols */
    alt_i = (sym == (looked ? showsyms[0] : defsyms[0].sym)) ? 0 : (2 + 1);
    for (hit_trap = FALSE, i = 0; i < MAX_CMAPPED_CHARS; i++) 
    {
        /* when sym is the default background character, we process
           i == 0 three times: unexplored, stone, dark part of a room */
        if (alt_i < 2) 
        {
            x_str = !alt_i++ ? "unexplored" : submerged ? "unknown" : "stone";
            i = 0; /* for second iteration, undo loop increment */
            /* alt_i is now 1 or 2 */
        } 
        else 
        {
            if (alt_i++ == 2)
                i = 0; /* undo loop increment */
            
            if (i == S_fountain)
                x_str = get_fountain_name(cc.x, cc.y);
            else
            {
                x_str = defsyms[i].explanation;
                if (defsyms[i].explanation && *defsyms[i].explanation)
                {
                    static char decoration_buf[BUFSZ * 2] = "";
                    const char* dec_descr = get_decoration_description(cc.x, cc.y);
                    const char* carpet_descr = get_carpet_description(cc.x, cc.y);
                    if (dec_descr && *dec_descr)
                    {
                        if(carpet_descr && *carpet_descr)
                            Sprintf(decoration_buf, "%s on %s", dec_descr, an(carpet_descr));
                        else
                        {
                            char dbuf[BUFSZ];
                            Strcpy(dbuf, dec_descr);
                            print_decoration_ending(dbuf, cc.x, cc.y);
                            Sprintf(decoration_buf, "%s on %s", dbuf, an(defsyms[i].explanation));
                        }
                        if ((levl[cc.x][cc.y].decoration_typ > 0
                            && (decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LIGHTABLE) != 0
                            && ((decoration_type_definitions[levl[cc.x][cc.y].decoration_typ].dflags & DECORATION_TYPE_FLAGS_LOOTABLE) == 0 || (levl[cc.x][cc.y].decoration_flags & DECORATION_FLAGS_ITEM_IN_HOLDER) != 0)
                            ) && cansee(cc.x, cc.y))
                        {
                            char buf2[BUFSZ * 2];
                            Sprintf(buf2, "%s%s", is_levl_lamplit(&levl[cc.x][cc.y]) ? "lit " : "unlit ", decoration_buf);
                            Strcpy(decoration_buf, buf2);
                        }
                        x_str = decoration_buf;
                    }
                    else if (carpet_descr && *carpet_descr && strstr(defsyms[i].explanation, "floor"))
                    {
                        Sprintf(decoration_buf, "%s on %s", carpet_descr, an(defsyms[i].explanation));
                        x_str = decoration_buf;
                    }
                }
            }
            if (submerged && !strcmp(x_str, defsyms[0].explanation))
                x_str = "land"; /* replace "dark part of a room" */
            /* alt_i is now 3 or more and no longer of interest */
        }

        if (sym == (looked ? showsyms[i] : defsyms[i].sym) && x_str && *x_str)
        {
            /* avoid "an unexplored", "an stone", "an air", "a water",
               "a floor of a room", "a dark part of a room";
               article==2 => "the", 1 => "an", 0 => (none) */
            int article = strstri(x_str, " of a room") ? 2
                          : !(alt_i <= 2
                              || strcmp(x_str, "air") == 0
                              || strcmp(x_str, "land") == 0
                              || strcmp(x_str, "parquet") == 0
                              || strcmp(x_str, "marble") == 0
                              || strcmp(x_str, "water") == 0);

            if (!found)
            {
                if (is_cmap_trap(i))
                {
                    Sprintf(out_str, "%sa trap", prefix);
                    hit_trap = TRUE;
                }
                else 
                {
                    Sprintf(out_str, "%s%s", prefix,
                            article == 2 ? the(x_str)
                            : article == 1 ? an(x_str) : x_str);
                }
                Strcpy(x_buf, article == 2 ? the(x_str)
                    : article == 1 ? an(x_str) : x_str);
                *firstmatch = x_buf;
                found++;
            } 
            else if (!(hit_trap && is_cmap_trap(i))
                       && !(found >= 3 && is_cmap_drawbridge(i))
                       /* don't mention vibrating square outside of Gehennom
                          unless this happens to be one (hallucination?) */
                       && (i != S_vibrating_square || Inhell
                           || (looked && glyph_is_trap(glyph)
                               && glyph_to_trap(glyph) == VIBRATING_SQUARE))) 
            {
                found += append_str(out_str, (article == 2) ? the(x_str)
                                             : (article == 1) ? an(x_str)
                                               : x_str);
                if (is_cmap_trap(i))
                    hit_trap = TRUE;
            }

            if (i == S_altar || is_cmap_trap(i) || is_cmap_door(i))
                need_to_look = TRUE;
        }
    }

    /* Now check for warning symbols */
    for (i = 1; i < WARNCOUNT; i++) 
    {
        x_str = def_warnsyms[i].explanation;
        if (sym == (looked ? warnsyms[i] : def_warnsyms[i].sym)) 
        {
            if (!found) 
            {
                Sprintf(out_str, "%s%s", prefix, def_warnsyms[i].explanation);
                *firstmatch = def_warnsyms[i].explanation;
                found++;
            }
            else 
            {
                found += append_str(out_str, def_warnsyms[i].explanation);
            }

            /* Kludge: warning trumps boulders on the display.
               Reveal the boulder too or player can get confused */
            if (looked && sobj_at(BOULDER, cc.x, cc.y))
                Strcat(out_str, " co-located with a boulder");
            break; /* out of for loop*/
        }
    }

    /* if we ignored venom and list turned out to be short, put it back */
    if (skipped_venom && found < 2) 
    {
        x_str = def_oc_syms[VENOM_CLASS].explain;
        if (!found)
        {
            Sprintf(out_str, "%s%s", prefix, an(x_str));
            *firstmatch = x_str;
            found++;
        }
        else 
        {
            found += append_str(out_str, an(x_str));
        }
    }

    /* handle optional boulder symbol as a special case */
    if (iflags.bouldersym && sym == iflags.bouldersym) 
    {
        if (!found) 
        {
            *firstmatch = "a boulder";
            Sprintf(out_str, "%s%s", prefix, *firstmatch);
            found++;
        } 
        else 
        {
            found += append_str(out_str, "boulder");
        }
    }

    /*
     * If we are looking at the screen, follow multiple possibilities or
     * an ambiguous explanation by something more detailed.
     */

    if (found > 4)
        Sprintf(out_str, "%s", "That can be many things");

 didlook:
    if (looked)
    {
        struct permonst *pm = (struct permonst *)0;

        if (found > 1 || need_to_look)
        {
            char temp_buf[BUFSZ * 5];
            char extrabuf[BUFSZ * 2];

            pm = lookat(cc.x, cc.y, look_buf, simple_buf, extrabuf);
            if (pm && for_supplement)
                *for_supplement = pm;
            *firstmatch = simple_buf;

            if (*look_buf)
            {
                char mdescbuf[BUFSZ * 2];
                Strcpy(mdescbuf, "");
                if (!Hallucination && pm && pm->mdescription && strcmp(pm->mdescription, ""))
                {
                    char mdescbuf2[BUFSZ * 2];
                    Strcpy(mdescbuf2, pm->mdescription);
                    //*mdescbuf2 = lowc(*mdescbuf2);
                    Sprintf(mdescbuf, ", %s", mdescbuf2);
                }

                if (iflags.using_gui_tiles)
                {
                    Sprintf(temp_buf, "%s%s", look_buf, mdescbuf);
                    (void)strncpy(out_str, temp_buf, BUFSZ - 1);
                    out_str[BUFSZ - 1] = 0;
                }
                else
                {
                    Sprintf(temp_buf, " (%s%s)", look_buf, mdescbuf);

                    int clen = BUFSZ - (int)strlen(out_str) - 1;
                    if (clen > 0)
                    {
                        (void)strncat(out_str, temp_buf, (size_t)clen);
                        out_str[BUFSZ - 1] = 0;
                    }
                }
                found = 1; /* we have something to look up */
            }

            if (extrabuf[0])
            {
                Sprintf(temp_buf, " [seen: %s]", extrabuf);
                int clen = BUFSZ - (int)strlen(out_str) - 1;
                if (clen > 0)
                {
                    (void)strncat(out_str, temp_buf, clen);
                    out_str[BUFSZ - 1] = 0;
                }
            }
        }
    }
    if (iflags.using_gui_tiles)
    {
        *out_str = highc(*out_str);
    }
    return found;
}

/* also used by getpos hack in do_name.c */
const char what_is_an_unknown_object[] = "an unknown object";

int
do_look(int mode, coord *click_cc)
{
    boolean quick = (mode == 1); /* use cursor; don't search for "more info" */
    boolean clicklook = (mode == 2); /* right mouse-click method */
    char out_str[BUFSZ * 5] = DUMMY;
    const char *firstmatch = "unknown";
    struct permonst *pm = 0, *supplemental_pm = 0;
    int i = '\0', ans = 0;
    nhsym sym = 0;              /* typed symbol or converted glyph */
    int found = FALSE;            /* count of matching syms found */
    coord cc = DUMMY;             /* screen pos of unknown glyph */
    boolean save_verbose = FALSE; /* saved value of flags.verbose */
    boolean from_screen = FALSE;  /* question from the screen */

    cc.x = 0;
    cc.y = 0;

    if (!clicklook) {
        if (quick) {
            from_screen = TRUE; /* yes, we want to use the cursor */
            i = 'y';
        } else {
            menu_item *pick_list = (menu_item *) 0;
            winid win;
            anything any;

            any = zeroany;
            win = create_nhwindow(NHW_MENU);
            start_menu_style(win, GHMENU_STYLE_CHOOSE_COMMAND);
            any.a_char = '/';
            /* 'y' and 'n' to keep backwards compatibility with previous
               versions: "Specify unknown object by cursor?" */
            add_menu(win, NO_GLYPH, &any,
                     flags.lootabc ? 0 : any.a_char, 'y', ATR_NONE, NO_COLOR,
                     "something on the map", MENU_UNSELECTED);
            any.a_char = 'i';
            add_menu(win, NO_GLYPH, &any,
                     flags.lootabc ? 0 : any.a_char, 0, ATR_NONE, NO_COLOR,
                     "something you're carrying", MENU_UNSELECTED);
            any.a_char = '?';
            add_menu(win, NO_GLYPH, &any,
                     flags.lootabc ? 0 : any.a_char, 'n', ATR_NONE, NO_COLOR,
                     "something else (by symbol or name)", MENU_UNSELECTED);
            if (!u.uswallow && !Hallucination) {
                any = zeroany;
                add_menu(win, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                         "", MENU_UNSELECTED);
                /* these options work sensibly for the swallowed case,
                   but there's no reason for the player to use them then;
                   objects work fine when hallucinating, but screen
                   symbol/monster class letter doesn't match up with
                   bogus monster type, so suppress when hallucinating */
                any.a_char = 'm';
                add_menu(win, NO_GLYPH, &any,
                         flags.lootabc ? 0 : any.a_char, 0, ATR_NONE, NO_COLOR,
                         "nearby monsters", MENU_UNSELECTED);
                any.a_char = 'M';
                add_menu(win, NO_GLYPH, &any,
                         flags.lootabc ? 0 : any.a_char, 0, ATR_NONE, NO_COLOR,
                         "all monsters shown on map", MENU_UNSELECTED);
                any.a_char = 'o';
                add_menu(win, NO_GLYPH, &any,
                         flags.lootabc ? 0 : any.a_char, 0, ATR_NONE, NO_COLOR,
                         "nearby objects", MENU_UNSELECTED);
                any.a_char = 'O';
                add_menu(win, NO_GLYPH, &any,
                         flags.lootabc ? 0 : any.a_char, 0, ATR_NONE, NO_COLOR,
                         "all objects shown on map", MENU_UNSELECTED);
            }
            end_menu(win, "What do you want to look at:");
            if (select_menu(win, PICK_ONE, &pick_list) > 0) {
                i = pick_list->item.a_char;
                free((genericptr_t) pick_list);
            }
            destroy_nhwindow(win);
        }

        switch (i) {
        default:
        case 'q':
            return 0;
        case 'y':
        case '/':
            from_screen = TRUE;
            sym = 0;
            cc.x = u.ux;
            cc.y = u.uy;
            break;
        case 'i':
          {
            char invlet;
            struct obj *invobj;

            invlet = display_inventory((const char *) 0, TRUE, SHOWWEIGHTS_INVENTORY, TRUE);
            if (!invlet || invlet == CANCEL_LET || invlet == SWAP_LET)
                return 0;
            *out_str = '\0';
            for (invobj = invent; invobj; invobj = invobj->nobj)
                if (invobj->invlet == invlet) {
                    Strcpy(out_str, singular(invobj, xname));
                    break;
                }
            if (*out_str)
                checkfile(out_str, pm, TRUE, TRUE, (char *) 0);
            return 0;
          }
        case '?':
            from_screen = FALSE;
            getlin_ex(GETLINE_GENERAL, ATR_NONE, NO_COLOR, "Specify what?", out_str, "type the word", (char*)0, (char*)0);
            if (strcmp(out_str, " ")) /* keep single space as-is */
                /* remove leading and trailing whitespace and
                   condense consecutive internal whitespace */
                mungspaces(out_str);
            if (out_str[0] == '\0' || out_str[0] == '\033')
                return 0;

            if (out_str[1]) { /* user typed in a complete string */
                checkfile(out_str, pm, TRUE, TRUE, (char *) 0);
                return 0;
            }
            sym = (nhsym)out_str[0];
            break;
        case 'm':
            look_all(TRUE, TRUE); /* list nearby monsters */
            return 0;
        case 'M':
            look_all(FALSE, TRUE); /* list all monsters */
            return 0;
        case 'o':
            look_all(TRUE, FALSE); /* list nearby objects */
            return 0;
        case 'O':
            look_all(FALSE, FALSE); /* list all objects */
            return 0;
        }
    } else { /* clicklook */
        cc.x = click_cc->x;
        cc.y = click_cc->y;
        sym = 0;
        from_screen = FALSE;
    }

    /* Save the verbose flag, we change it later. */
    save_verbose = flags.verbose;
    flags.verbose = flags.verbose && !quick;
    /*
     * The user typed one letter, or we're identifying from the screen.
     */
    do {
        /* Reset some variables. */
        pm = (struct permonst *) 0;
        found = 0;
        out_str[0] = '\0';

        if (from_screen || clicklook) {
            if (from_screen) {
                if (flags.verbose)
                    pline("Please move the cursor to %s.",
                          what_is_an_unknown_object);
                else
                    pline("Pick an object.");

                ans = getpos(&cc, quick, what_is_an_unknown_object, CURSOR_STYLE_LOOK_CURSOR);
                if (ans < 0 || cc.x < 0)
                    break; /* done */
                flags.verbose = FALSE; /* only print long question once */
            }
        }

        found = do_screen_description(cc, (from_screen || clicklook), sym,
                                      out_str, &firstmatch, &supplemental_pm);

        /* Finally, print out our explanation. */
        if (found) {
            /* use putmixed() because there may be an encoded glyph present */
            putmixed(WIN_MESSAGE, 0, out_str);
#if defined (DUMPLOG) || defined (DUMPHTML)
            {
                char dmpbuf[BUFSZ];

                /* putmixed() bypasses pline() so doesn't write to DUMPLOG;
                   tty puts it into ^P recall, so it ought to be there;
                   DUMPLOG is plain text, so override graphics character;
                   at present, force space, but we ought to use defsyms[]
                   value for the glyph the graphics character came from */
                (void) decode_mixed(dmpbuf, out_str);
                if (dmpbuf[0] < ' ' || dmpbuf[0] >= 127) /* ASCII isprint() */
                    dmpbuf[0] = ' ';
                dumplogmsg(dmpbuf, (char*)0, (char*)0, ATR_NONE, NO_COLOR);
            }
#endif

            /* check the data file for information about this thing */
            if (found == 1 && ans != LOOK_QUICK && ans != LOOK_ONCE
                && (ans == LOOK_VERBOSE || (flags.help && !quick))
                && !clicklook) {
                char temp_buf[BUFSZ], supplemental_name[BUFSZ];

                supplemental_name[0] = '\0';
                Strcpy(temp_buf, firstmatch);
                checkfile(temp_buf, pm, FALSE,
                          (boolean) (ans == LOOK_VERBOSE), supplemental_name);
                if (supplemental_pm)
                    do_supplemental_info(supplemental_name, supplemental_pm,
                                         (boolean) (ans == LOOK_VERBOSE));
            }
        } else {
            pline("I've never heard of such things.");
        }
    } while (from_screen && !quick && ans != LOOK_ONCE && !clicklook);

    flags.verbose = save_verbose;

    return 0;
}

/*
 * Parameters:
 *   nearby: True => within BOLTLIM, False => entire map
 *   do_mons: True => monsters, False => objects
 */
static void
look_all(boolean nearby, boolean do_mons)
{
    winid win;
    int x, y, lo_x, lo_y, hi_x, hi_y, glyph, count = 0;
    char lookbuf[BUFSZ], outbuf[BUFSZ];

    win = create_nhwindow(NHW_TEXT);
    lo_y = nearby ? max(u.uy - NEARBY_CUTOFF_RANGE_CAN_SEE, 0) : 0;
    lo_x = nearby ? max(u.ux - NEARBY_CUTOFF_RANGE_CAN_SEE, 1) : 1;
    hi_y = nearby ? min(u.uy + NEARBY_CUTOFF_RANGE_CAN_SEE, ROWNO - 1) : ROWNO - 1;
    hi_x = nearby ? min(u.ux + NEARBY_CUTOFF_RANGE_CAN_SEE, COLNO - 1) : COLNO - 1;
    for (y = lo_y; y <= hi_y; y++) {
        for (x = lo_x; x <= hi_x; x++) {
            lookbuf[0] = '\0';
            glyph = glyph_at(x, y);
            if (do_mons) {
                if (glyph_is_monster(glyph)) {
                    struct monst *mtmp;

                    bhitpos.x = x; /* [is this actually necessary?] */
                    bhitpos.y = y;
                    if (x == u.ux && y == u.uy && canspotself()) {
                        (void) self_lookat(lookbuf);
                        ++count;
                    } else if ((mtmp = m_at(x, y)) != 0) {
                        look_at_monster(lookbuf, (char*)0, (char *)0, mtmp, x, y);
                        ++count;
                    }
                } else if (glyph_is_invisible(glyph)) {
                    /* remembered, unseen, creature */
                    Strcpy(lookbuf, invisexplain);
                    ++count;
                } else if (glyph_is_warning(glyph)) {
                    int warnindx = glyph_to_warning(glyph);

                    Strcpy(lookbuf, def_warnsyms[warnindx].explanation);
                    ++count;
                }
            } else { /* !do_mons */
                if (glyph_is_object(glyph)) {
                    look_at_object(lookbuf, x, y, glyph);
                    ++count;
                }
            }
            if (*lookbuf) {
                char coordbuf[20], which[12], cmode;

                cmode = (iflags.getpos_coords != GPCOORDS_NONE)
                           ? iflags.getpos_coords : GPCOORDS_MAP;
                if (count == 1) {
                    Strcpy(which, do_mons ? "monsters" : "objects");
                    if (nearby)
                        Sprintf(outbuf, "%s currently shown near %s:",
                                upstart(which),
                                (cmode != GPCOORDS_COMPASS)
                                  ? coord_desc(u.ux, u.uy, coordbuf, cmode)
                                  : !canspotself() ? "your position" : "you");
                    else
                        Sprintf(outbuf, "All %s currently shown on the map:",
                                which);
                    putstr(win, 0, outbuf);
                    putstr(win, 0, "");
                }
                /* prefix: "coords  C  " where 'C' is mon or obj symbol */
                Sprintf(outbuf, (cmode == GPCOORDS_SCREEN) ? "%s  "
                                  : (cmode == GPCOORDS_MAP) ? "%8s  "
                                      : "%12s  ",
                        coord_desc(x, y, coordbuf, cmode));
                Sprintf(eos(outbuf), "%s  ", encglyph(glyph));
                /* guard against potential overflow */
                lookbuf[sizeof lookbuf - 1 - strlen(outbuf)] = '\0';
                Strcat(outbuf, lookbuf);
                putmixed(win, 0, outbuf);
            }
        }
    }
    if (count)
        display_nhwindow(win, TRUE);
    else
        pline("No %s are currently shown %s.",
              do_mons ? "monsters" : "objects",
              nearby ? "nearby" : "on the map");
    destroy_nhwindow(win);
}

static const char *suptext1[] = {
    "%s is a member of a marauding horde of orcs",
    "rumored to have brutally attacked and plundered",
    "the ordinarily sheltered town that is located ",
    "deep within The Gnomish Mines.",
    "",
    "The members of that vicious horde proudly and ",
    "defiantly acclaim their allegiance to their",
    "leader %s in their names.",
    (char *) 0,
};

static const char *suptext2[] = {
    "\"%s\" is the common dungeon name of",
    "a nefarious orc who is known to acquire property",
    "from thieves and sell it off for profit.",
    "",
    "The perpetrator was last seen hanging around the",
    "stairs leading to the Gnomish Mines.",
    (char *) 0,
};

void
do_supplemental_info(char *name, struct permonst *pm, boolean without_asking)
{
    const char **textp;
    winid datawin = WIN_ERR;
    char *entrytext = name, *bp = (char *) 0, *bp2 = (char *) 0;
    char question[QBUFSZ];
    boolean yes_to_moreinfo = FALSE;
    boolean is_marauder = (name && pm && is_orc(pm));

    /*
     * Provide some info on some specific things
     * meant to support in-game mythology, and not
     * available from data.base or other sources.
     */
    if (is_marauder && (strlen(name) < (BUFSZ - 1))) {
        char fullname[BUFSZ];

        bp = strstri(name, " of ");
        bp2 = strstri(name, " the Fence");

        if (bp || bp2) {
            Strcpy(fullname, name);
            if (!without_asking) {
                Strcpy(question, "More info about \"");
                /* +2 => length of "\"?" */
                copynchars(eos(question), entrytext,
                    (int) (sizeof question - 1 - (strlen(question) + 2)));
                Strcat(question, "\"?");
                if (yn_query(question) == 'y')
                yes_to_moreinfo = TRUE;
            }
            if (yes_to_moreinfo) {
                int i, subs = 0;
                const char *gang = (char *) 0;

                if (bp) {
                    textp = suptext1;
                    gang = bp + 4;
                    *bp = '\0';
                } else {
                    textp = suptext2;
                    gang = "";
                }
                datawin = create_nhwindow(NHW_MENU);
                for (i = 0; textp[i]; i++) {
                    char buf[BUFSZ];
                    const char *txt;

                    if (strstri(textp[i], "%s") != 0) {
                        Sprintf(buf, textp[i], subs++ ? gang : fullname);
                        txt = buf;
                    } else
                        txt = textp[i];
                    putstr(datawin, 0, txt);
                }
                display_nhwindow(datawin, FALSE);
                destroy_nhwindow(datawin), datawin = WIN_ERR;
            }
        }
    }
}

/* the '/' command */
int
dowhatis(void)
{
    return do_look(0, (coord *) 0);
}

/* the ';' command */
int
doquickwhatis(void)
{
    return do_look(1, (coord *) 0);
}

/* the '^' command */
int
doidtrap(void)
{
    struct trap *trap;
    int x, y, tt, glyph;

    debugprint_pos();
    if (!getdir("^"))
        return 0;
    x = u.ux + u.dx;
    y = u.uy + u.dy;

    /* check fake bear trap from confused gold detection */
    glyph = glyph_at(x, y);
    if (glyph_is_trap(glyph) && (tt = glyph_to_trap(glyph)) == BEAR_TRAP) 
    {
        boolean chesttrap = trapped_chest_at(tt, x, y);

        if (chesttrap || trapped_door_at(tt, x, y))
        {
            pline("That is a trapped %s.", chesttrap ? "chest" : "door");
            return 0; /* trap ID'd, but no time elapses */
        }
    }

    for (trap = ftrap; trap; trap = trap->ntrap)
        if (trap->tx == x && trap->ty == y) 
        {
            if (!trap->tseen)
                break;
            tt = trap->ttyp;
            if (u.dz)
            {
                if (u.dz < 0 ? is_hole(tt) : tt == ROCKTRAP)
                    break;
            }
            tt = what_trap(tt, rn2_on_display_rng);
            pline("That is %s%s%s.",
                  an(!Hallucination ? get_trap_explanation(trap) : defsyms[trap_to_defsym(tt)].explanation),
                  !trap->madeby_u
                     ? ""
                     : (tt == WEB)
                        ? " woven"
                        /* trap doors & spiked pits can't be made by
                           player, and should be considered at least
                           as much "set" as "dug" anyway */
                        : (tt == HOLE || tt == PIT)
                           ? " dug"
                           : " set",
                  !trap->madeby_u ? "" : " by you");

            return 0;
        }
    pline("I can't see a trap there.");
    return 0;
}

/*
    Implements a rudimentary if/elif/else/endif interpretor and use
    conditionals in dat/cmdhelp to describe what command each keystroke
    currently invokes, so that there isn't a lot of "(debug mode only)"
    and "(if number_pad is off)" cluttering the feedback that the user
    sees.  (The conditionals add quite a bit of clutter to the raw data
    but users don't see that.  number_pad produces a lot of conditional
    commands:  basic letters vs digits, 'g' vs 'G' for '5', phone
    keypad vs normal layout of digits, and QWERTZ keyboard swap between
    y/Y/^Y/M-y/M-Y/M-^Y and z/Z/^Z/M-z/M-Z/M-^Z.)

    The interpretor understands
     '&#' for comment,
     '&? option' for 'if' (also '&? !option'
                           or '&? option=value[,value2,...]'
                           or '&? !option=value[,value2,...]'),
     '&: option' for 'elif' (with argument variations same as 'if';
                             any number of instances for each 'if'),
     '&:' for 'else' (also '&: #comment';
                      0 or 1 instance for a given 'if'), and
     '&.' for 'endif' (also '&. #comment'; required for each 'if').

    The option handling is a bit of a mess, with no generality for
    which options to deal with and only a comma separated list of
    integer values for the '=value' part.  number_pad is the only
    supported option that has a value; the few others (wizard/debug,
    rest_on_space, #if SHELL, #if SUSPEND) are booleans.
*/

static void
whatdoes_help(void)
{
    dlb *fp;
    char *p, buf[BUFSZ];
    winid tmpwin = create_nhwindow(NHW_TEXT);

    fp = dlb_fopen(KEYHELP, "r");
    if (!fp) {
        pline("Cannot open \"%s\" data file!", KEYHELP);
        display_nhwindow(WIN_MESSAGE, TRUE);
        return;
    }
    while (dlb_fgets(buf, (int) sizeof buf, fp)) {
        if (*buf == '#')
            continue;
        for (p = buf; *p; p++)
            if (*p != ' ' && *p != '\t')
                break;
        putstr(tmpwin, 0, p);
    }
    (void) dlb_fclose(fp);
    display_nhwindow(tmpwin, TRUE);
    destroy_nhwindow(tmpwin);
}

#if 0
#define WD_STACKLIMIT 5
struct wd_stack_frame {
    boolean active;
    boolean been_true;
    boolean else_seen;
};

static boolean whatdoes_cond(char *, struct wd_stack_frame *,
                                         int *, int);

static boolean
whatdoes_cond(char *buf, struct wd_stack_frame *stack, int *depth, int lnum)
{
    const char badstackfmt[] = "cmdhlp: too many &%c directives at line %d.";
    boolean newcond, neg, gotopt;
    char *p, *q, act = buf[1];
    int np = 0;

    newcond = (act == '?' || !stack[*depth].been_true);
    buf += 2;
    mungspaces(buf);
    if (act == '#' || *buf == '#' || !*buf || !newcond) {
        gotopt = (*buf && *buf != '#');
        *buf = '\0';
        neg = FALSE; /* lint suppression */
        p = q = (char *) 0;
    } else {
        gotopt = TRUE;
        if ((neg = (*buf == '!')) != 0)
            if (*++buf == ' ')
                ++buf;
        p = index(buf, '='), q = index(buf, ':');
        if (!p || (q && q < p))
            p = q;
        if (p) { /* we have a value specified */
            /* handle a space before or after (or both) '=' (or ':') */
            if (p > buf && p[-1] == ' ')
                p[-1] = '\0'; /* end of keyword in buf[] */
            *p++ = '\0'; /* terminate keyword, advance to start of value */
            if (*p == ' ')
                p++;
        }
    }
    if (*buf && (act == '?' || act == ':')) {
        if (!strcmpi(buf, "number_pad")) {
            if (!p) {
                newcond = iflags.num_pad;
            } else {
                /* convert internal encoding (separate yes/no and 0..3)
                   back to user-visible one (-1..4) */
                np = iflags.num_pad ? (1 + iflags.num_pad_mode) /* 1..4 */
                                    : (-1 * iflags.num_pad_mode); /* -1..0 */
                newcond = FALSE;
                for (; p; p = q) {
                    q = index(p, ',');
                    if (q)
                        *q++ = '\0';
                    if (atoi(p) == np) {
                        newcond = TRUE;
                        break;
                    }
                }
            }
        } else if (!strcmpi(buf, "rest_on_space")) {
            newcond = flags.rest_on_space;
        } else if (!strcmpi(buf, "debug") || !strcmpi(buf, "wizard")) {
            newcond = flags.debug; /* == wizard */
        } else if (!strcmpi(buf, "shell")) {
#ifdef SHELL
            /* should we also check sysopt.shellers? */
            newcond = TRUE;
#else
            newcond = FALSE;
#endif
        } else if (!strcmpi(buf, "suspend")) {
#ifdef SUSPEND
            /* sysopt.shellers is also used for dosuspend()... */
            newcond = TRUE;
#else
            newcond = FALSE;
#endif
        } else {
            impossible(
                "cmdhelp: unrecognized &%c conditional at line %d: \"%.20s\"",
                       act, lnum, buf);
            neg = FALSE;
        }
        /* this works for number_pad too: &? !number_pad:-1,0
           would be true for 1..4 after negation */
        if (neg)
            newcond = !newcond;
    }
    switch (act) {
    default:
    case '#': /* comment */
        break;
    case '.': /* endif */
        if (--*depth < 0) {
            impossible(badstackfmt, '.', lnum);
            *depth = 0;
        }
        break;
    case ':': /* else or elif */
        if (*depth == 0 || stack[*depth].else_seen) {
            impossible(badstackfmt, ':', lnum);
            *depth = 1; /* so that stack[*depth - 1] is a valid access */
        }
        if (stack[*depth].active || stack[*depth].been_true
            || !stack[*depth - 1].active)
            stack[*depth].active = 0;
        else if (newcond)
            stack[*depth].active = stack[*depth].been_true = 1;
        if (!gotopt)
            stack[*depth].else_seen = 1;
        break;
    case '?': /* if */
        if (++*depth >= WD_STACKLIMIT) {
            impossible(badstackfmt, '?', lnum);
            *depth = WD_STACKLIMIT - 1;
        }
        stack[*depth].active = (newcond && stack[*depth - 1].active) ? 1 : 0;
        stack[*depth].been_true = stack[*depth].active;
        stack[*depth].else_seen = 0;
        break;
    }
    return stack[*depth].active ? TRUE : FALSE;
}
#endif /* 0 */

char *
dowhatdoes_core(char q, char *cbuf)
{
    char buf[BUFSZ];
#if 0
    dlb *fp;
    struct wd_stack_frame stack[WD_STACKLIMIT];
    boolean cond;
    int ctrl, meta, depth = 0, lnum = 0;
#endif /* 0 */
    const char *ec_desc;

    if ((ec_desc = key2extcmddesc(q)) != NULL) {
        char keybuf[QBUFSZ];

        Sprintf(buf, "%-8s%s.", key2txt(q, keybuf), ec_desc);
        Strcpy(cbuf, buf);
        return cbuf;
    }
    return 0;
#if 0
    fp = dlb_fopen(CMDHELPFILE, "r");
    if (!fp) {
        pline("Cannot open \"%s\" data file!", CMDHELPFILE);
        return 0;
    }

    meta = (0x80 & (uchar) q) != 0;
    if (meta)
        q &= 0x7f;
    ctrl = (0x1f & (uchar) q) == (uchar) q;
    if (ctrl)
        q |= 0x40; /* NUL -> '@', ^A -> 'A', ... ^Z -> 'Z', ^[ -> '[', ... */
    else if (q == 0x7f)
        ctrl = 1, q = '?';

    (void) memset((genericptr_t) stack, 0, sizeof stack);
    cond = stack[0].active = 1;
    while (dlb_fgets(buf, sizeof buf, fp)) {
        ++lnum;
        if (buf[0] == '&' && buf[1] && index("?:.#", buf[1])) {
            cond = whatdoes_cond(buf, stack, &depth, lnum);
            continue;
        }
        if (!cond)
            continue;
        if (meta ? (buf[0] == 'M' && buf[1] == '-'
                    && (ctrl ? buf[2] == '^' && highc(buf[3]) == q
                             : buf[2] == q))
                 : (ctrl ? buf[0] == '^' && highc(buf[1]) == q
                         : buf[0] == q)) {
            (void) strip_newline(buf);
            if (index(buf, '\t'))
                (void) tabexpand(buf);
            if (meta && ctrl && buf[4] == ' ') {
                Strncpy(buf, "M-^?    ", 8);
                buf[3] = q;
            } else if (meta && buf[3] == ' ') {
                Strncpy(buf, "M-?     ", 8);
                buf[2] = q;
            } else if (ctrl && buf[2] == ' ') {
                Strncpy(buf, "^?      ", 8);
                buf[1] = q;
            } else if (buf[1] == ' ') {
                Strncpy(buf, "?       ", 8);
                buf[0] = q;
            }
            (void) dlb_fclose(fp);
            Strcpy(cbuf, buf);
            return cbuf;
        }
    }
    (void) dlb_fclose(fp);
    if (depth != 0)
        impossible("cmdhelp: mismatched &? &: &. conditionals.");
    return (char *) 0;
#endif /* 0 */
}

int
dowhatdoes(void)
{
    static boolean once = FALSE;
    char bufr[BUFSZ];
    char q, *reslt;

    if (!once) {
        pline("Ask about '&' or '?' to get more info.%s",
#ifdef ALTMETA
              iflags.altmeta ? "  (For ESC, type it twice.)" :
#endif
              "");
        once = TRUE;
    }
#if defined(UNIX) || defined(VMS)
    introff(); /* disables ^C but not ^\ */
#endif
    q = yn_function("What command?", (char *)0, '\0', (char*)0);
#ifdef ALTMETA
    if (q == '\033' && iflags.altmeta) {
        /* in an ideal world, we would know whether another keystroke
           was already pending, but this is not an ideal world...
           if user typed ESC, we'll essentially hang until another
           character is typed */
        q = yn_function("]", (char *)0, '\0', (char*)0);
        if (q != '\033')
            q = (char) ((uchar) q | 0200);
    }
#endif /*ALTMETA*/
#if defined(UNIX) || defined(VMS)
    intron(); /* reenables ^C */
#endif
    reslt = dowhatdoes_core(q, bufr);
    if (reslt) {
        if (q == '&' || q == '?')
            whatdoes_help();
        pline("%s", reslt);
    } else {
        pline("No such command '%s', char code %d (0%03o or 0x%02x).",
              visctrl(q), (uchar) q, (uchar) q, (uchar) q);
    }
    return 0;
}

static void
docontact(void)
{
    winid cwin = create_nhwindow(NHW_TEXT);
    char buf[BUFSZ];

    if (sysopt.support) {
        /*XXX overflow possibilities*/
        Sprintf(buf, "To contact local support, %s", sysopt.support);
        putstr(cwin, 0, buf);
        putstr(cwin, 0, "");
    } else if (sysopt.fmtd_wizard_list) { /* formatted SYSCF WIZARDS */
        Sprintf(buf, "To contact local support, contact %s.",
                sysopt.fmtd_wizard_list);
        putstr(cwin, 0, buf);
        putstr(cwin, 0, "");
    }
    putstr(cwin, 0, "To contact the GnollHack development team directly,");
    /*XXX overflow possibilities*/
    Sprintf(buf, "see the 'Contact' form on our website or email <%s>.",
            DEVTEAM_EMAIL);
    putstr(cwin, 0, buf);
    putstr(cwin, 0, "");
    putstr(cwin, 0, "For more information on GnollHack, or to report a bug,");
    Sprintf(buf, "visit our website \"%s\".", DEVTEAM_URL);
    putstr(cwin, 0, buf);
    display_nhwindow(cwin, FALSE);
    destroy_nhwindow(cwin);
}

void
dispfile_help(void)
{
    display_file(HELP, TRUE);
}

void
dispfile_shelp(void)
{
    display_file(SHELP, TRUE);
}

void
dispfile_optionfile(void)
{
    display_file(OPTIONFILE, TRUE);
}

void
dispfile_license(void)
{
    display_file(LICENSE, TRUE);
}

void
dispfile_credits(void)
{
    display_file(CREDITSFILE, TRUE);
}


void
dispfile_debughelp(void)
{
    display_file(DEBUGHELP, TRUE);
}

void
hmenu_doextversion(void)
{
    (void) doextversion();
}

void
hmenu_dohistory(void)
{
    (void) dohistory();
}

void
hmenu_dowhatis(void)
{
    (void) dowhatis();
}

void
hmenu_dowhatdoes(void)
{
    (void) dowhatdoes();
}

void
hmenu_doextlist(void)
{
    (void) doextlist();
}

void
domenucontrols(void)
{
    winid cwin = create_nhwindow(NHW_TEXT);
    show_menu_controls(cwin, FALSE);
    display_nhwindow(cwin, FALSE);
    destroy_nhwindow(cwin);
}

/* data for dohelp() */
static const struct {
    void (*f)(void);
    const char *text;
} help_menu_items[] = {
    { hmenu_doextversion, "About GnollHack (version information)." },
    { dispfile_help, "Long description of the game and commands." },
    { dispfile_shelp, "List of game commands." },
    { hmenu_dohistory, "Concise history of GnollHack and NetHack." },
    { hmenu_dowhatis, "Info on a character in the game display." },
    { hmenu_dowhatdoes, "Info on what a given key does." },
    { option_help, "List of game options." },
    { dispfile_optionfile, "Longer explanation of game options." },
    { dokeylist, "Full list of keyboard commands" },
    { hmenu_doextlist, "List of extended commands." },
    { domenucontrols, "List menu control keys" },
    { dispfile_license, "The GnollHack license." },
    { dispfile_credits, "Credits." },
    { docontact, "Support information." },
#ifdef PORT_HELP
    { port_help, "%s-specific help and commands." },
#endif
    { dispfile_debughelp, "List of wizard-mode commands." },
    { (void (*)(void)) 0, (char *) 0 }
};

/* the '?' command */
int
dohelp(void)
{
    winid tmpwin = create_nhwindow(NHW_MENU);
    char helpbuf[QBUFSZ];
    int i, n;
    menu_item *selected;
    anything any;
    int sel;

    any = zeroany; /* zero all bits */
    start_menu_style(tmpwin, GHMENU_STYLE_HELP);

    for (i = 0; help_menu_items[i].text; i++) {
        if (!wizard && help_menu_items[i].f == dispfile_debughelp)
            continue;
        if (help_menu_items[i].text[0] == '%') {
            Sprintf(helpbuf, help_menu_items[i].text, PORT_ID);
        } else {
            Strcpy(helpbuf, help_menu_items[i].text);
        }
        any.a_int = i + 1;
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                 helpbuf, MENU_UNSELECTED);
    }
    end_menu(tmpwin, "Select one item:");
    n = select_menu(tmpwin, PICK_ONE, &selected);
    destroy_nhwindow(tmpwin);
    if (n > 0) {
        sel = selected[0].item.a_int - 1;
        free((genericptr_t) selected);
        (void) (*help_menu_items[sel].f)();
    }
    return 0;
}

/* the 'V' command; also a choice for '?' */
int
dohistory(void)
{
    display_file(HISTORY, TRUE);
    return 0;
}

#if defined(DUMPLOG) || defined(DUMPHTML)

/*
 * AI snapshot map legend.
 *
 * The Overseer AI reads the snapshot's ASCII map as plain text, with no
 * tileset and none of the hover tooltips html_dump_glyph() gives a human
 * reading the HTML dumplog.  On its own that map is close to unreadable: a
 * corridor and a sink are both '#', every trap is '^', and a blank cell looks
 * like empty floor when it really means "never seen, or solid rock".
 *
 * dump_map_legend_ai() is printed between the "Map:" heading and the map
 * itself, in three sections:
 *   - reading notes: the coordinate system, what a blank cell means, that the
 *     map is hero memory rather than live vision, plus caveats for
 *     hallucination, blindness, being engulfed, and arboreal levels
 *   - the symbols actually present on this map, with generic meanings from the
 *     same tables do_screen_description() uses, and a cell count each
 *   - notable positions with symbol, color, the description lookat() would
 *     give, and a compass bearing from the hero
 *
 * It works in three stages: walk every map cell once and accumulate two flat
 * lists (one entry per distinct symbol seen, one per notable position), sort
 * them, then print.  Everything either list needs to know about a kind of
 * thing - what to call it, and whether it earns coordinate lines at all -
 * lives in the single legend_kinds[] table.
 *
 * Bulk terrain deliberately gets a cell count but no coordinate lines, which
 * is what keeps the legend short on water levels and arboreal ones.
 *
 * Snapshot-only, like the format_for_ai parts of list_discoveries(): a human
 * reading a dumplog has the tileset, the tooltips and the game itself in front
 * of them and needs none of this.
 */

/* maximum coordinate lines per kind, so that an item-strewn level cannot push
   the rest of the snapshot past the reader's size limit */
#define LEGEND_MAX_PER_KIND 40

/* One classification for both lists, in print order. */
enum legend_kinds_types {
    LEGEND_KIND_CREATURE = 0,
    LEGEND_KIND_OBJECT,
    LEGEND_KIND_TRAP,
    LEGEND_KIND_FEATURE,
    LEGEND_KIND_TERRAIN,
    LEGEND_KIND_OTHER,

    LEGEND_MAX_KINDS
};

static const struct legend_kind {
    const char *label;    /* prefixes a coordinate line */
    const char *section;  /* prefixes a symbol line */
    const char *plural;   /* names the kind in the "and N more" line */
    boolean has_positions; /* FALSE for bulk terrain: count it, do not list it */
} legend_kinds[LEGEND_MAX_KINDS] = {
    { "Creature", "Creatures",        "creatures",        TRUE  },
    { "Object",   "Objects",          "objects",          TRUE  },
    { "Trap",     "Traps",            "traps",            TRUE  },
    { "Feature",  "Dungeon features", "dungeon features", TRUE  },
    { "Terrain",  "Terrain",          "terrain cells",    FALSE },
    { "Other",    "Other",            "other things",     FALSE }
};

/* one entry per distinct screen symbol found on the map */
struct legend_sym_entry {
    short sym;   /* screen symbol index, showsyms[] numbering */
    short kind;  /* enum legend_kinds_types */
    nhsym ch;    /* the character the map actually printed for it */
    int count;   /* how many cells hold it */
};

/* one entry per notable position */
struct legend_pos_entry {
    xchar x, y;
    short sym;
    short kind;
    nhsym ch;
    schar color;
    boolean hidden; /* a feature currently covered by something on top */
};

/* File-static rather than automatic: this runs on the UI thread of a mobile
   app, and the two lists plus the slot map are a few kilobytes. */
static struct legend_sym_entry legend_syms[SYM_MAX];
static int legend_symcnt;
static short legend_symslot[SYM_MAX]; /* sym -> legend_syms[] index, or -1 */
static struct legend_pos_entry legend_positions[LEGEND_MAX_KINDS
                                                * LEGEND_MAX_PER_KIND];
static int legend_poscnt;
static int legend_kept[LEGEND_MAX_KINDS];  /* lines kept, for the cap */
static int legend_wanted[LEGEND_MAX_KINDS]; /* lines wanted, for "and N more" */

/* Is this screen symbol a "point feature", that is, something worth its own
   coordinate line?  Bulk terrain - floor, corridor, grass, ground, walls,
   stone, unexplored, water, lava, ice, air, cloud, tree - is not: it gets a
   cell count in the symbol list instead, and coordinate lines for it would
   swamp the legend on a water level or an arboreal one.
   Traps are excluded here because they are their own kind; that includes the
   trap-range portals, the lever and the vibrating square. */
static boolean
legend_is_point_feature(int sym)
{
    if (sym < 0 || sym >= MAX_CMAPPED_CHARS)
        return FALSE;

    if (is_cmap_door(sym) || is_cmap_drawbridge(sym))
        return TRUE;

    switch (sym)
    {
    case S_ndoor:      /* doorway or broken door */
    case S_bars:
    case S_upstair:
    case S_dnstair:
    case S_upladder:
    case S_dnladder:
    case S_anvil:
    case S_altar:
    case S_grave:
    case S_brazier:
    case S_signpost:
    case S_throne:
    case S_sink:
    case S_fountain:
        return TRUE;
    default:
        break;
    }

    return FALSE;
}

/* Which kind of thing this cell holds.  Both lists classify through here.
 *
 * Parameters:
 *   glyph: the displayed glyph, or NO_GLYPH to classify by symbol alone
 *   sym:   screen symbol index from map_ai_glyph_char()
 *
 * The glyph is consulted first because it knows things the symbol index does
 * not: a statue and a live monster share a monster class letter (unless
 * flags.classic_statue_symbol is set), and only the glyph says which is
 * which.  Passing NO_GLYPH gives the stable symbol-only answer, which is what
 * the symbol list wants - one symbol index must always land in one section,
 * however many different things happen to be drawn with it.
 */
static int
legend_kind_of(int glyph, int sym)
{
    if (glyph != NO_GLYPH)
    {
        if (glyph_is_monster(glyph) || glyph_is_warning(glyph)
            || glyph_is_invisible(glyph))
            return LEGEND_KIND_CREATURE;
        if (glyph_is_object(glyph))
            return LEGEND_KIND_OBJECT;
        if (glyph_is_trap(glyph))
            return LEGEND_KIND_TRAP;
    }

    if (sym >= SYM_OFF_X)
        return (sym - SYM_OFF_X) == SYM_INVISIBLE ? LEGEND_KIND_CREATURE
                                                  : LEGEND_KIND_OTHER;
    if (sym >= SYM_OFF_M) /* covers SYM_OFF_W too: both are creatures */
        return LEGEND_KIND_CREATURE;
    if (sym >= SYM_OFF_O)
        return LEGEND_KIND_OBJECT;
    if (is_cmap_trap(sym))
        return LEGEND_KIND_TRAP;
    if (legend_is_point_feature(sym))
        return LEGEND_KIND_FEATURE;

    return LEGEND_KIND_TERRAIN;
}

/* Generic meaning of a screen symbol index, from the same tables that
   do_screen_description() consults when it is not looking at the map. */
static const char *
legend_sym_explanation(int sym)
{
    int idx;

    if (sym >= SYM_OFF_X)
    {
        idx = sym - SYM_OFF_X;
        if (idx == SYM_BOULDER)
            return "boulder";
        if (idx == SYM_INVISIBLE)
            return invisexplain;
        return "unknown";
    }
    if (sym >= SYM_OFF_W)
    {
        idx = sym - SYM_OFF_W;
        if (idx > 0 && idx < WARNCOUNT && def_warnsyms[idx].explanation)
            return def_warnsyms[idx].explanation;
        return "warning of an unseen creature";
    }
    if (sym >= SYM_OFF_M)
    {
        idx = sym - SYM_OFF_M;
        if (idx > 0 && idx < MAX_MONSTER_CLASSES && def_monsyms[idx].explain
            && *def_monsyms[idx].explain)
            return def_monsyms[idx].explain;
        return "creature";
    }
    if (sym >= SYM_OFF_O)
    {
        idx = sym - SYM_OFF_O;
        if (idx > 0 && idx < MAX_OBJECT_CLASSES && def_oc_syms[idx].explain
            && *def_oc_syms[idx].explain)
            return def_oc_syms[idx].explain;
        return "object";
    }

    /* cmap.  S_unexplored and S_stone are both blank on screen, and the
       reader has no way to tell them apart, so describe them as one thing. */
    if (sym == S_unexplored || sym == S_stone)
        return "never seen, or solid rock";
    if (sym >= 0 && sym < MAX_CMAPPED_CHARS && defsyms[sym].explanation
        && *defsyms[sym].explanation)
        return defsyms[sym].explanation;

    return "unknown";
}

/* Count one cell against its symbol, appending an entry the first time that
   symbol is seen. */
static void
legend_tally_sym(int sym, nhsym ch)
{
    struct legend_sym_entry *e;

    if (sym < 0 || sym >= SYM_MAX)
        return;

    if (legend_symslot[sym] < 0)
    {
        legend_symslot[sym] = (short) legend_symcnt;
        e = &legend_syms[legend_symcnt++];
        e->sym = (short) sym;
        e->kind = (short) legend_kind_of(NO_GLYPH, sym);
        e->ch = ch;
        e->count = 0;
    }
    legend_syms[legend_symslot[sym]].count++;
}

/* Remember one notable position.  'wanted' is counted even past the cap, so
   that the "and N more" line can be honest about what was dropped. */
static void
legend_add_pos(int kind, int x, int y, nhsym ch, int sym, int color,
               boolean hidden)
{
    struct legend_pos_entry *e;

    if (kind < 0 || kind >= LEGEND_MAX_KINDS
        || !legend_kinds[kind].has_positions)
        return;

    legend_wanted[kind]++;
    if (legend_kept[kind] >= LEGEND_MAX_PER_KIND
        || legend_poscnt >= SIZE(legend_positions))
        return;
    legend_kept[kind]++;

    e = &legend_positions[legend_poscnt++];
    e->x = (xchar) x;
    e->y = (xchar) y;
    e->sym = (short) sym;
    e->kind = (short) kind;
    e->ch = ch;
    e->color = (schar) color;
    e->hidden = hidden;
}

/* qsort comparison routine: kind, then commonest first, then symbol index so
   that the order is stable when two symbols are equally common */
static int
legend_symcmp(const void *p, const void *q)
{
    const struct legend_sym_entry *a = (const struct legend_sym_entry *) p;
    const struct legend_sym_entry *b = (const struct legend_sym_entry *) q;

    if (a->kind != b->kind)
        return a->kind - b->kind;
    if (a->count != b->count)
        return b->count - a->count;

    return a->sym - b->sym;
}

/* qsort comparison routine: kind, then reading order */
static int
legend_poscmp(const void *p, const void *q)
{
    const struct legend_pos_entry *a = (const struct legend_pos_entry *) p;
    const struct legend_pos_entry *b = (const struct legend_pos_entry *) q;

    if (a->kind != b->kind)
        return a->kind - b->kind;
    if (a->y != b->y)
        return a->y - b->y;

    return a->x - b->x;
}

void
dump_map_legend_ai(void)
{
    static char descbuf[BUFSZ * 5], simplebuf[BUFSZ * 2], extrabuf[BUFSZ * 2];
    static char buf[BUFSZ * 6];
    char coordbuf[BUFSZ];
    int x, y, i, kind, sym, color, glyph, terrain_glyph;
    int default_glyph;
    int hero_sym = -1;
    int saved_terrainmode;
    coord saved_bhitpos;
    nhsym ch, hero_ch = 0;
    uint64_t special;
    int subset = TER_MAP | TER_TRP | TER_OBJ | TER_MON;

    legend_symcnt = 0;
    legend_poscnt = 0;
    for (i = 0; i < SYM_MAX; i++)
        legend_symslot[i] = -1;
    for (i = 0; i < LEGEND_MAX_KINDS; i++)
        legend_kept[i] = legend_wanted[i] = 0;

    default_glyph = base_cmap_to_glyph(is_levflag_arboreal(&level.flags)
                                           ? S_tree : S_unexplored);

    /* lookat() -> look_at_object() -> object_from_map() marks an adjacent
       object dknown unless terrain mode is active.  Taking a snapshot must
       not teach the hero anything, so borrow the mechanism #terrain uses:
       a non-zero terrainmode means "view what is already known".  All four
       bits are set so that nothing is filtered out of the descriptions.
       lookat() also writes bhitpos; save and restore it. */
    saved_terrainmode = iflags.terrainmode;
    saved_bhitpos = bhitpos;
    iflags.terrainmode = subset;

    /* Stage 1: walk the map once, in the same order and with the same
       arguments dump_map_ai() uses, so that the legend describes the map that
       is actually printed.  Each cell is counted against its symbol, and
       classified for the coordinate list.

       The second reveal_terrain_getglyph() call asks what the terrain alone
       would show.  Where that differs from the displayed glyph, a point
       feature is hidden under a creature, an item or a trap, and "there is a
       staircase under that dragon" is exactly what the reader needs.  The
       same kind test applies to it, so bulk terrain cannot leak in. */
    for (y = 0; y < ROWNO; y++)
    {
        for (x = 1; x < COLNO; x++)
        {
            glyph = reveal_terrain_getglyph(x, y, FALSE, u.uswallow,
                                            default_glyph, subset);
            color = NO_COLOR;
            special = 0;
            sym = 0;
            ch = map_ai_glyph_char(glyph, x, y, &sym, &color, &special);

            legend_tally_sym(sym, ch);

            /* Remember which symbol the hero's own cell produced, so that the
               symbol list can point the reader at it.  Only when the hero is
               actually drawn there: when it cannot sense itself the cell
               shows terrain, and when engulfed it shows the engulfer.  Keyed
               on the symbol index rather than on '@' so that a polymorphed
               hero tags the right line. */
            if (x == u.ux && y == u.uy && !u.uswallow
                && (glyph_is_monster(glyph) || glyph_is_invisible(glyph)))
            {
                hero_sym = sym;
                hero_ch = ch;
            }

            legend_add_pos(legend_kind_of(glyph, sym), x, y, ch, sym, color,
                           FALSE);

            terrain_glyph = reveal_terrain_getglyph(x, y, FALSE, u.uswallow,
                                                    default_glyph, TER_MAP);
            if (terrain_glyph != glyph)
            {
                color = NO_COLOR;
                special = 0;
                sym = 0;
                ch = map_ai_glyph_char(terrain_glyph, x, y, &sym, &color,
                                       &special);
                if (legend_kind_of(NO_GLYPH, sym) == LEGEND_KIND_FEATURE)
                    legend_add_pos(LEGEND_KIND_FEATURE, x, y, ch, sym, color,
                                   TRUE);
            }
        }
    }

    /* Stage 2: sort both lists into print order. */
    if (legend_symcnt > 1)
        qsort((genericptr_t) legend_syms, legend_symcnt,
              sizeof *legend_syms, legend_symcmp);
    if (legend_poscnt > 1)
        qsort((genericptr_t) legend_positions, legend_poscnt,
              sizeof *legend_positions, legend_poscmp);

    /* Stage 3: print.
       Section 1: how to read the map. */
    Sprintf(buf,
            "Reading this map: the two lines above the map are a column"
            " ruler, and the second of them numbers every column. Every map"
            " row starts with a %d character gutter holding its row number"
            " and \": \", then exactly %d map characters, and ends at its"
            " line break.",
            MAP_AI_GUTTER_WIDTH, COLNO - 1);
    putstr(0, ATR_NONE, buf);
    Sprintf(buf,
            "Columns are x = 1 to %d, left to right. Rows are y = 0 to %d,"
            " top to bottom, and the number in the gutter is y. To find"
            " <x,y>, take row y and read the map character x positions after"
            " the gutter. Coordinates below are written <x,y>.",
            COLNO - 1, ROWNO - 1);
    putstr(0, ATR_NONE, buf);
    Sprintf(buf,
            "There is no end-of-row marker character, because every"
            " printable character is already a map symbol in this game. If a"
            " row looks shorter than %d characters, its rightmost cells were"
            " blank and something in transit trimmed them; the column ruler"
            " is the authoritative scale.",
            COLNO - 1);
    putstr(0, ATR_NONE, buf);
    if (u.uswallow)
        Sprintf(buf,
                "The hero is at <%d,%d>, inside the creature drawn there.",
                (int) u.ux, (int) u.uy);
    else if (hero_ch)
        Sprintf(buf, "The hero is at <%d,%d>, shown as '%c'.",
                (int) u.ux, (int) u.uy, (char) hero_ch);
    else
        Sprintf(buf,
                "The hero is at <%d,%d>, but is not drawn there: it cannot"
                " currently sense itself, so that cell shows the terrain"
                " instead.",
                (int) u.ux, (int) u.uy);
    putstr(0, ATR_NONE, buf);
    putstr(0, ATR_NONE,
           "A blank cell is NOT open floor: it is either area the hero has"
           " never seen or solid rock. Blank margins to the left, right,"
           " above and below the drawn area are simply unvisited parts of the"
           " level, not empty rooms.");
    putstr(0, ATR_NONE,
           "This map is the hero's memory, not live vision: creatures and"
           " items are drawn where they were last seen and may have moved or"
           " been taken since.");
    if (Hallucination)
        putstr(0, ATR_NONE,
               "The hero is hallucinating, so every description below is"
               " unreliable.");
    if (Blind)
        putstr(0, ATR_NONE,
               "The hero is blind; the map shows only what is remembered or"
               " felt.");
    if (u.uswallow)
        putstr(0, ATR_NONE,
               "The hero has been engulfed; the map shows the interior of the"
               " engulfing creature, not the level.");
    if (is_levflag_arboreal(&level.flags))
        putstr(0, ATR_NONE,
               "This level is arboreal: cells the hero has not seen default"
               " to trees rather than blank.");
    putstr(0, ATR_NONE,
           "For the full GnollHack symbol reference and map-reading guidance,"
           " read knowledge base topic `reading_the_game_map`.");
    /* blank separator; win 0 so it reaches the AI snapshot file too
       (NHW_DUMPTXT is filtered out of it by dump_putstr_ex) */
    putstr(0, 0, "");

    /* Section 2: the symbols present on this map.  The section name is
       repeated on every line rather than used as an indented heading,
       because the reader's HTML-to-text step collapses runs of spaces and
       indentation would be lost. */
    putstr(0, ATR_HEADING, "Symbols on this map:");
    for (i = 0; i < legend_symcnt; i++)
    {
        sym = (int) legend_syms[i].sym;
        ch = legend_syms[i].ch;
        kind = (int) legend_syms[i].kind;

        Sprintf(buf, "%s: '%c'%s %s, %d cell%s",
                legend_kinds[kind].section, (char) ch,
                ch == ' ' ? " (blank)" : "",
                legend_sym_explanation(sym), legend_syms[i].count,
                legend_syms[i].count == 1 ? "" : "s");
        if (sym == hero_sym)
            Strcat(buf, legend_syms[i].count == 1
                            ? " (this is the hero)"
                            : " (one of them is the hero)");
        putstr(0, ATR_NONE, buf);
    }
    putstr(0, 0, "");

    /* Section 3: notable positions. */
    putstr(0, ATR_HEADING, "Notable locations:");
    if (!legend_poscnt)
        putstr(0, ATR_NONE,
               "Nothing notable is currently shown on the map: no creatures,"
               " items, traps or dungeon features.");
    for (i = 0; i < legend_poscnt; i++)
    {
        x = (int) legend_positions[i].x;
        y = (int) legend_positions[i].y;
        ch = legend_positions[i].ch;
        sym = (int) legend_positions[i].sym;
        kind = (int) legend_positions[i].kind;
        color = (int) legend_positions[i].color;

        Sprintf(buf, "%s <%d,%d> '%c'", legend_kinds[kind].label, x, y,
                (char) ch);
        /* NO_COLOR is 8, which is a real index into c_obj_colors[], so this
           test has to come first or a colorless cell would be labelled
           "transparent". */
        if (color != NO_COLOR && color >= 0 && color < CLR_MAX)
            Sprintf(eos(buf), " [%s]", c_obj_colors[color]);

        if (legend_positions[i].hidden)
        {
            /* Described from the terrain tables: lookat() reads the live
               display and would describe whatever is standing on top. */
            Sprintf(eos(buf), " %s, currently hidden under something",
                    legend_sym_explanation(sym));
        }
        else
        {
            descbuf[0] = simplebuf[0] = extrabuf[0] = '\0';
            (void) lookat(x, y, descbuf, simplebuf, extrabuf);
            if (!*descbuf)
                Strcpy(descbuf, legend_sym_explanation(sym));
            if (x == u.ux && y == u.uy)
                Sprintf(eos(buf), " you: %s", descbuf);
            else
                Sprintf(eos(buf), " %s", descbuf);
        }

        if (!(x == u.ux && y == u.uy))
        {
            *coordbuf = '\0';
            (void) coord_desc(x, y, coordbuf, GPCOORDS_COMPASS);
            if (*coordbuf)
                Sprintf(eos(buf), " %s", coordbuf);
        }
        putstr(0, ATR_NONE, buf);

        /* the overflow note follows the last kept line of its kind */
        if ((i + 1 == legend_poscnt
             || (int) legend_positions[i + 1].kind != kind)
            && legend_wanted[kind] > legend_kept[kind])
        {
            Sprintf(buf,
                    "... and %d more %s on the map, not listed (legend size"
                    " limit).",
                    legend_wanted[kind] - legend_kept[kind],
                    legend_kinds[kind].plural);
            putstr(0, ATR_NONE, buf);
        }
    }
    putstr(0, 0, "");

    iflags.terrainmode = saved_terrainmode;
    bhitpos = saved_bhitpos;
}

#endif /* DUMPLOG || DUMPHTML */

/*pager.c*/
