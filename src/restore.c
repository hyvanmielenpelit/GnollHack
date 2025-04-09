/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    restore.c    $NHDT-Date: 1555201698 2019/04/14 00:28:18 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.129 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Michael Allison, 2009. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h"
#include "tcap.h" /* for TERMLIB and ASCIIGRAPH */

#if defined(MICRO)
extern int dotcnt; /* shared with save */
extern int dotrow; /* shared with save */
#endif

#ifdef ZEROCOMP
STATIC_DCL void NDECL(zerocomp_minit);
STATIC_DCL void FDECL(zerocomp_mread, (int, genericptr_t, size_t));
STATIC_DCL int NDECL(zerocomp_mgetc);
#endif

STATIC_DCL void NDECL(def_minit);
STATIC_DCL void FDECL(def_mread, (int, genericptr_t, size_t));

STATIC_DCL int FDECL(check_save_file_tracking, (int64_t));
STATIC_DCL void NDECL(find_lev_obj);
STATIC_DCL void NDECL(find_memory_obj);
STATIC_DCL void FDECL(restlevchn, (int));
STATIC_DCL void FDECL(restdamage, (int, BOOLEAN_P));
STATIC_DCL void FDECL(restobj, (int, struct obj *));
STATIC_DCL struct obj *FDECL(restobjchn, (int, BOOLEAN_P, BOOLEAN_P));
STATIC_OVL void FDECL(restmon, (int, struct monst *));
STATIC_DCL struct monst *FDECL(restmonchn, (int, BOOLEAN_P));
STATIC_DCL struct fruit *FDECL(loadfruitchn, (int));
STATIC_DCL void FDECL(freefruitchn, (struct fruit *));
STATIC_DCL void FDECL(ghostfruit, (struct obj *));
STATIC_DCL boolean FDECL(restgamestate, (int, unsigned int *, unsigned int *));
STATIC_DCL void FDECL(restlevelstate, (unsigned int, unsigned int));
STATIC_DCL int FDECL(restlevelfile, (int, XCHAR_P));
STATIC_OVL void FDECL(restore_msghistory, (int));
STATIC_DCL void FDECL(reset_oattached_mids, (BOOLEAN_P));
STATIC_DCL void FDECL(rest_levl, (int, BOOLEAN_P));
STATIC_DCL void FDECL(restore_gamelog, (int));
STATIC_DCL void FDECL(post_restore_to_forum, (struct u_realtime));

STATIC_VAR struct restore_procs {
    const char *name;
    int mread_flags;
    void NDECL((*restore_minit));
    void FDECL((*restore_mread), (int, genericptr_t, size_t));
    void FDECL((*restore_bclose), (int));
} restoreprocs = {
#if !defined(ZEROCOMP) || (defined(COMPRESS) || defined(ZLIB_COMP))
    "externalcomp", 0, def_minit, def_mread, def_bclose,
#else
    "zerocomp", 0, zerocomp_minit, zerocomp_mread, zerocomp_bclose,
#endif
};

/*
 * Save a mapping of IDs from ghost levels to the current level.  This
 * map is used by the timer routines when restoring ghost levels.
 */
#define N_PER_BUCKET 64
struct bucket {
    struct bucket *next;
    struct {
        unsigned gid; /* ghost ID */
        unsigned nid; /* new ID */
    } map[N_PER_BUCKET];
};

STATIC_DCL void NDECL(clear_id_mapping);
STATIC_DCL void FDECL(add_id_mapping, (unsigned, unsigned));

STATIC_VAR int n_ids_mapped = 0;
STATIC_VAR struct bucket *id_map = 0;

#ifdef AMII_GRAPHICS
void FDECL(amii_setpens, (int)); /* use colors from save file */
extern int amii_numcolors;
#endif

#include "display.h"

boolean restoring = FALSE;
boolean reseting = FALSE;

STATIC_VAR NEARDATA struct fruit *oldfruit;
STATIC_VAR NEARDATA int64_t omoves;

#define Is_IceBox(o) ((o)->otyp == ICE_BOX ? TRUE : FALSE)

/* Recalculate level.objects[x][y], since this info was not saved. */
STATIC_OVL void
find_lev_obj()
{
    register struct obj *fobjtmp = (struct obj *) 0;
    register struct obj *otmp;
    int x, y;

    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            level.objects[x][y] = (struct obj *) 0;

    /*
     * Reverse the entire fobj chain, which is necessary so that we can
     * place the objects in the proper order.  Make all obj in chain
     * OBJ_FREE so place_object will work correctly.
     */
    while ((otmp = fobj) != 0) {
        fobj = otmp->nobj;
        otmp->nobj = fobjtmp;
        otmp->where = OBJ_FREE;
        fobjtmp = otmp;
    }
    /* fobj should now be empty */

    /* Set level.objects (as well as reversing the chain back again) */
    while ((otmp = fobjtmp) != 0) {
        fobjtmp = otmp->nobj;
        place_object(otmp, otmp->ox, otmp->oy);
    }
}

STATIC_OVL void
find_memory_obj()
{
    register struct obj* memoryobjstmp = (struct obj*)0;
    register struct obj* otmp;
    int x, y;

    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            level.locations[x][y].hero_memory_layers.memory_objchn = (struct obj*)0;

    /*
     * Reverse the entire memoryobjs chain, which is necessary so that we can
     * place the objects in the proper order.  Make all obj in chain
     * OBJ_FREE so place_object will work correctly.
     */
    while ((otmp = memoryobjs) != 0) {
        memoryobjs = otmp->nobj;
        otmp->nobj = memoryobjstmp;
        otmp->where = OBJ_FREE;
        memoryobjstmp = otmp;
    }
    /* memoryobjs should now be empty */
    lastmemoryobj = 0;

    /* Set level.locations[x][y].hero_memory_layers.memory_objchn (as well as reversing the chain back again) */
    while ((otmp = memoryobjstmp) != 0) {
        memoryobjstmp = otmp->nobj;
        place_memory_object(otmp, otmp->ox, otmp->oy);
    }
    update_last_memoryobj();
}

/* Things that were marked "in_use" when the game was saved (ex. via the
 * infamous "HUP" cheat) get used up here.
 */
void
inven_inuse(quietly)
boolean quietly;
{
    register struct obj *otmp, *otmp2;

    for (otmp = invent; otmp; otmp = otmp2) 
    {
        otmp2 = otmp->nobj;
        if (otmp->in_use) 
        {
            if (otmp->otyp == AMULET_OF_YENDOR
                || otmp->otyp == CANDELABRUM_OF_INVOCATION
                || otmp->otyp == BELL_OF_OPENING
                || otmp->otyp == SPE_BOOK_OF_THE_DEAD
                || is_quest_artifact(otmp)
                || otmp->oartifact > 0
                || Is_proper_container(otmp)
                )
            {
                otmp->in_use = 0; /* Likely memory corruption; prevent destruction of any critical items */
                if (!quietly)
                    impossible("Mysterious force prevents finishing off %s...", xname(otmp));
                else
                {
                    char dbuf[BUFSZ * 2];
                    Sprintf(dbuf, "Mysterious force prevents finishing off %s...", xname(otmp));
                    issue_debuglog(0, dbuf);
                }
            }
            else
            {
                if (!quietly)
                    pline("Finishing off %s...", xname(otmp));
                useup(otmp);
            }
        }
    }
}

STATIC_OVL void
restlevchn(fd)
register int fd;
{
    int cnt;
    s_level *tmplev, *x;
    Strcpy(debug_buf_4, "restlevchn");

    sp_levchn = (s_level *) 0;
    mread(fd, (genericptr_t) &cnt, sizeof(int));
    for (; cnt > 0; cnt--) 
    {
        tmplev = (s_level *) alloc(sizeof(s_level));
        if (!tmplev)
            return;
        mread(fd, (genericptr_t) tmplev, sizeof(s_level));
        if (!sp_levchn)
            sp_levchn = tmplev;
        else {
            for (x = sp_levchn; x->next; x = x->next)
                ;
            x->next = tmplev;
        }
        tmplev->next = (s_level *) 0;
    }
}

STATIC_OVL void
restdamage(fd, ghostly)
int fd;
boolean ghostly;
{
    int counter;
    struct damage *tmp_dam;
    Strcpy(debug_buf_4, "restdamage");

    mread(fd, (genericptr_t) &counter, sizeof(counter));
    if (!counter)
        return;
    tmp_dam = (struct damage *) alloc(sizeof(struct damage));
    while (--counter >= 0) {
        char damaged_shops[5], *shp = (char *) 0;

        mread(fd, (genericptr_t) tmp_dam, sizeof(*tmp_dam));
        if (ghostly)
            tmp_dam->when += (monstermoves - omoves);
        Strcpy(damaged_shops,
               in_rooms(tmp_dam->place.x, tmp_dam->place.y, SHOPBASE));
        if (u.uz.dlevel) {
            /* when restoring, there are two passes over the current
             * level.  the first time, u.uz isn't set, so neither is
             * shop_keeper().  just wait and process the damage on
             * the second pass.
             */
            for (shp = damaged_shops; *shp; shp++) {
                struct monst *shkp = shop_keeper(*shp);

                if (shkp && inhishop(shkp)
                    && repair_damage(shkp, tmp_dam, (int *) 0, TRUE))
                    break;
            }
        }
        if (!shp || !*shp) {
            tmp_dam->next = level.damagelist;
            level.damagelist = tmp_dam;
            tmp_dam = (struct damage *) alloc(sizeof(*tmp_dam));
        }
    }
    free((genericptr_t) tmp_dam);
}

/* restore one object */
STATIC_OVL void
restobj(fd, otmp)
int fd;
struct obj *otmp;
{
    size_t buflen;
    Strcpy(debug_buf_4, "restobj");

    mread(fd, (genericptr_t) otmp, sizeof(struct obj));

    /* next object pointers are invalid; otmp->cobj needs to be left
       as is--being non-null is key to restoring container contents */
    otmp->nobj = otmp->nexthere = (struct obj *) 0;
    /* non-null oextra needs to be reconstructed */
    if (otmp->oextra) {
        otmp->oextra = newoextra();

        /* oname - object's true name */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) { /* includes terminating '\0' */
            new_oname(otmp, buflen);
            mread(fd, (genericptr_t) ONAME(otmp), buflen);
        }
        /* uoname - object's user-specified name */
        mread(fd, (genericptr_t)&buflen, sizeof(buflen));
        if (buflen > 0) { /* includes terminating '\0' */
            new_uoname(otmp, buflen);
            mread(fd, (genericptr_t)UONAME(otmp), buflen);
        }
        /* omonst - corpse or statue might retain full monster details */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            newomonst(otmp);
            /* this is actually a monst struct, so we
               can just defer to restmon() here */
            restmon(fd, OMONST(otmp));
        }
        /* omid - monster id number, connecting corpse to ghost */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            newomid(otmp);
            mread(fd, (genericptr_t) OMID(otmp), buflen);
        }
        /* olong - temporary gold */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            newolong(otmp);
            mread(fd, (genericptr_t) OLONG(otmp), buflen);
        }
        /* omailcmd - feedback mechanism for scroll of mail */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            char *omailcmd = (char *) alloc(buflen);

            mread(fd, (genericptr_t) omailcmd, buflen);
            new_omailcmd(otmp, omailcmd);
            free((genericptr_t) omailcmd);
        }
    }
}

STATIC_OVL struct obj *
restobjchn(fd, ghostly, frozen)
register int fd;
boolean ghostly, frozen;
{
    register struct obj *otmp, *otmp2 = 0;
    register struct obj *first = (struct obj *) 0;
    size_t buflen;
    Strcpy(debug_buf_3, "restobjchn");
    Strcpy(debug_buf_4, "restobjchn");

    while (1) {
        mread(fd, (genericptr_t) &buflen, sizeof buflen);
        if (buflen == 0)
            break;

        otmp = newobj();
        if (!otmp)
            return (struct obj*)0;

        restobj(fd, otmp);
        if (!first)
            first = otmp;
        else
            otmp2->nobj = otmp;

        if (ghostly) {
            unsigned nid = context.ident++;
            add_id_mapping(otmp->o_id, nid);
            otmp->o_id = nid;
        }
        if (ghostly && otmp->otyp == SLIME_MOLD)
            ghostfruit(otmp);
        /* Ghost levels get object age shifted from old player's clock
         * to new player's clock.  Assumption: new player arrived
         * immediately after old player died.
         */
        if (ghostly && !frozen && !age_is_relative(otmp))
            otmp->age = monstermoves - omoves + otmp->age;

        /* get contents of a container or statue */
        if (Has_contents(otmp)) 
        {
            struct obj *otmp3;

            otmp->cobj = restobjchn(fd, ghostly, Is_IceBox(otmp));
            /* restore container back pointers */
            for (otmp3 = otmp->cobj; otmp3; otmp3 = otmp3->nobj)
                otmp3->ocontainer = otmp;
        } 
        else if (SchroedingersBox(otmp)) 
        {
            struct obj *catcorpse;

            /*
             * TODO:  Remove this after 3.6.x save compatibility is dropped.
             *
             * For 3.6.2, SchroedingersBox() always has a cat corpse in it.
             * For 3.6.[01], it was empty and its weight was falsified
             * to have the value it would have had if there was one inside.
             * Put a non-rotting cat corpse in this box to convert to 3.6.2.
             *
             * [Note: after this fix up, future save/restore of this object
             * will take the Has_contents() code path above.]
             */
            if ((catcorpse = mksobj(CORPSE, TRUE, FALSE, FALSE)) != 0) {
                otmp->speflags |= SPEFLAGS_SCHROEDINGERS_BOX;  //otmp->enchantment = 1; /* flag for special SchroedingersBox */
                set_corpsenm(catcorpse, PM_HOUSECAT);
                (void) stop_timer(ROT_CORPSE, obj_to_any(catcorpse));
                add_to_container(otmp, catcorpse);
                otmp->owt = weight(otmp);
            }
        }
        if (otmp->bypass)
            otmp->bypass = 0;
        if (!ghostly) {
            /* fix the pointers */
            if (otmp->o_id == context.victual.o_id)
                context.victual.piece = otmp;
            if (otmp->o_id == context.tin.o_id)
                context.tin.tin = otmp;
            if (otmp->o_id == context.spbook.o_id)
                context.spbook.book = otmp;
        }
        otmp2 = otmp;
    }
    if (first && otmp2->nobj) {
        impossible("Restobjchn: error reading objchn.");
        otmp2->nobj = 0;
    }

    return first;
}

/* restore one monster */
STATIC_OVL void
restmon(fd, mtmp)
int fd;
struct monst *mtmp;
{
    size_t buflen;
    Strcpy(debug_buf_4, "restmon");

    mread(fd, (genericptr_t) mtmp, sizeof(struct monst));

    /* next monster pointer is invalid */
    mtmp->nmon = (struct monst *) 0;
    /* non-null mextra needs to be reconstructed */
    if (mtmp->mextra) {
        mtmp->mextra = newmextra();

        /* mname - monster's name */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) { /* includes terminating '\0' */
            new_mname(mtmp, buflen);
            mread(fd, (genericptr_t) MNAME(mtmp), buflen);
        }
        /* umname - monster's uname */
        mread(fd, (genericptr_t)&buflen, sizeof(buflen));
        if (buflen > 0) { /* includes terminating '\0' */
            new_umname(mtmp, buflen);
            mread(fd, (genericptr_t)UMNAME(mtmp), buflen);
        }
        /* egd - vault guard */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            newegd(mtmp);
            mread(fd, (genericptr_t) EGD(mtmp), sizeof(struct egd));
        }
        /* epri - temple priest */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            newepri(mtmp);
            mread(fd, (genericptr_t) EPRI(mtmp), sizeof(struct epri));
        }
        /* esmi - smith */
        mread(fd, (genericptr_t)&buflen, sizeof(buflen));
        if (buflen > 0) {
            newesmi(mtmp);
            mread(fd, (genericptr_t)ESMI(mtmp), sizeof(struct esmi));
        }
        /* enpc - non-player character */
        mread(fd, (genericptr_t)&buflen, sizeof(buflen));
        if (buflen > 0) {
            newenpc(mtmp);
            mread(fd, (genericptr_t)ENPC(mtmp), sizeof(struct enpc));
        }
        /* eshk - shopkeeper */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            neweshk(mtmp);
            mread(fd, (genericptr_t) ESHK(mtmp), sizeof(struct eshk));
        }
        /* emin - minion */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            newemin(mtmp);
            mread(fd, (genericptr_t) EMIN(mtmp), sizeof(struct emin));
        }
        /* edog - pet */
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen > 0) {
            newedog(mtmp);
            mread(fd, (genericptr_t) EDOG(mtmp), sizeof(struct edog));
        }
        /* mmonst - original form when polymorphed */
        mread(fd, (genericptr_t)&buflen, sizeof(buflen));
        if (buflen > 0) {
            newmmonst(mtmp);
            restmon(fd, MMONST(mtmp));
        }
        /* mobj - object that mimic is posing as */
        mread(fd, (genericptr_t)&buflen, sizeof(buflen));
        if (buflen > 0) {
            newmobj(mtmp);
            restobj(fd, MOBJ(mtmp));
        }
        /* mcorpsenm - obj->corpsenm for mimic posing as corpse or
           statue (inline int rather than pointer to something) */
        mread(fd, (genericptr_t) &MCORPSENM(mtmp), sizeof MCORPSENM(mtmp));
    } /* mextra */

    /* Relink data */
    mtmp->data = &mons[mtmp->mnum];
}

STATIC_OVL struct monst *
restmonchn(fd, ghostly)
register int fd;
boolean ghostly;
{
    register struct monst *mtmp, *mtmp2 = 0;
    register struct monst *first = (struct monst *) 0;
    size_t buflen;
    Strcpy(debug_buf_3, "restmonchn");
    Strcpy(debug_buf_4, "restmonchn");

    while (1) {
        mread(fd, (genericptr_t) &buflen, sizeof(buflen));
        if (buflen == 0)
            break;

        mtmp = newmonst();
        if (!mtmp)
            return (struct monst*) 0;

        restmon(fd, mtmp);
        if (!first)
            first = mtmp;
        else
            mtmp2->nmon = mtmp;

        if (ghostly) {
            unsigned nid = context.ident++;
            add_id_mapping(mtmp->m_id, nid);
            mtmp->m_id = nid;
        }
        if (ghostly) {
            int mndx = monsndx(mtmp->data);
            if (propagate(mndx, TRUE, ghostly) == 0) {
                /* cookie to trigger purge in getbones() */
                mtmp->mbasehpmax = DEFUNCT_MONSTER;
            }
        }
        if (mtmp->minvent) {
            struct obj *obj;
            mtmp->minvent = restobjchn(fd, ghostly, FALSE);
            /* restore monster back pointer */
            for (obj = mtmp->minvent; obj; obj = obj->nobj)
                obj->ocarry = mtmp;
        }
        if (mtmp->mw) {
            struct obj *obj;

            for (obj = mtmp->minvent; obj; obj = obj->nobj)
                if (obj->owornmask & W_WEP)
                    break;
            if (obj)
                mtmp->mw = obj;
            else {
                MON_NOWEP(mtmp);
                impossible("bad monster weapon restore");
            }
        }

        if (mtmp->isshk)
            restshk(mtmp, ghostly);
        if (mtmp->ispriest)
            restpriest(mtmp, ghostly);
        if (mtmp->issmith)
            restsmith(mtmp, ghostly);
        if (mtmp->isnpc)
            restnpc(mtmp, ghostly);

        if (!ghostly) {
            if (mtmp->m_id == context.polearm.m_id)
                context.polearm.hitmon = mtmp;
        }
        mtmp2 = mtmp;
    }
    if (first && mtmp2->nmon) {
        impossible("Restmonchn: error reading monchn.");
        mtmp2->nmon = 0;
    }
    return first;
}

STATIC_OVL struct fruit *
loadfruitchn(fd)
int fd;
{
    register struct fruit *flist, *fnext;
    Strcpy(debug_buf_4, "loadfruitchn");

    flist = 0;
    while (fnext = newfruit(), mread(fd, (genericptr_t) fnext, sizeof *fnext), fnext->fid != 0)
    {
        fnext->nextf = flist;
        flist = fnext;
    }
    dealloc_fruit(fnext);
    return flist;
}

STATIC_OVL void
freefruitchn(flist)
register struct fruit *flist;
{
    register struct fruit *fnext;

    while (flist) 
    {
        fnext = flist->nextf;
        dealloc_fruit(flist);
        flist = fnext;
    }
}

STATIC_OVL void
ghostfruit(otmp)
register struct obj *otmp;
{
    register struct fruit *oldf;

    for (oldf = oldfruit; oldf; oldf = oldf->nextf)
        if (oldf->fid == otmp->special_quality)
            break;

    if (!oldf)
        impossible("no old fruit?");
    else
        otmp->special_quality = fruitadd(oldf->fname, (struct fruit *) 0);
}

#ifdef ANDROID
#define SYSOPT_CHECK_SAVE_UID FALSE
#else
#ifdef SYSCF
#define SYSOPT_CHECK_SAVE_UID sysopt.check_save_uid
#else
#define SYSOPT_CHECK_SAVE_UID TRUE
#endif
#endif


STATIC_OVL
boolean
restgamestate(fd, stuckid, steedid)
register int fd;
unsigned int *stuckid, *steedid;
{
    struct flag newgameflags;
#ifdef SYSFLAGS
    struct sysflag newgamesysflags;
#endif
    struct context_info *newgamecontext = (struct context_info*)malloc(sizeof(struct context_info));; /* all 0, but has some pointers */
    struct obj *otmp, *tmp_bc;
    char timebuf[15];
    uint64_t uid;
    boolean defer_perm_invent;
    Strcpy(debug_buf_2, "restgamestate");
    Strcpy(debug_buf_3, "restgamestate");
    Strcpy(debug_buf_4, "restgamestate");

    mread(fd, (genericptr_t) &uid, sizeof uid);
    if (SYSOPT_CHECK_SAVE_UID
        && uid != (uint64_t) getuid()) { /* strange ... */
        /* for wizard mode, issue a reminder; for others, treat it
           as an attempt to cheat and refuse to restore this file */
        pline("Saved game was not yours.");
        if (!wizard)
            return FALSE;
    }

    init_tiledata();
#ifdef USE_TILES
    process_tiledata(1, (const char*)0, glyph2tile, glyphtileflags);
#endif

    *newgamecontext = context; /* copy statically init'd context */
    mread(fd, (genericptr_t) &context, sizeof (struct context_info));
    context.warntype.species = (context.warntype.speciesidx >= LOW_PM)
                                  ? &mons[context.warntype.speciesidx]
                                  : (struct permonst *) 0;
    /* context.victual.piece, .tin.tin, .spellbook.book, and .polearm.hitmon
       are pointers which get set to Null during save and will be recovered
       via corresponding o_id or m_id while objs or mons are being restored */

    /* we want to be able to revert to command line/environment/config
       file option values instead of keeping old save file option values
       if partial restore fails and we resort to starting a new game */
    newgameflags = flags;
    mread(fd, (genericptr_t) &flags, sizeof (struct flag));
    /* avoid keeping permanent inventory window up to date during restore
       (setworn() calls update_inventory); attempting to include the cost
       of unpaid items before shopkeeper's bill is available is a no-no;
       named fruit names aren't accessible yet either
       [3.6.2: moved perm_invent from flags to iflags to keep it out of
       save files; retaining the override here is simpler than trying to
       to figure out where it really belongs now] */
    defer_perm_invent = iflags.perm_invent;
    iflags.perm_invent = FALSE;
    /* wizard and discover are actually flags.debug and flags.explore;
       player might be overriding the save file values for them;
       in the discover case, we don't want to set that for a normal
       game until after the save file has been removed */
    iflags.deferred_X = (newgameflags.explore && !discover);
    if (newgameflags.debug) {
        /* authorized by startup code; wizard mode exists and is allowed */
        wizard = TRUE, discover = iflags.deferred_X = FALSE;
    } else if (wizard) {
        /* specified by save file; check authorization now */
        set_playmode();
    }
#ifdef SYSFLAGS
    newgamesysflags = sysflags;
    mread(fd, (genericptr_t) &sysflags, sizeof(struct sysflag));
#endif
    mread(fd, (genericptr_t)&spl_orderindx, sizeof(spl_orderindx));
    role_init(); /* Reset the initial role, race, gender, and alignment */
#ifdef AMII_GRAPHICS
    amii_setpens(amii_numcolors); /* use colors from save file */
#endif
    mread(fd, (genericptr_t) &u, sizeof(struct you));

#define ReadTimebuf(foo)                   \
    mread(fd, (genericptr_t) timebuf, 14); \
    timebuf[14] = '\0';                    \
    foo = time_from_yyyymmddhhmmss(timebuf);

    ReadTimebuf(ubirthday);
    mread(fd, &urealtime, sizeof urealtime);
    //ReadTimebuf(urealtime.start_timing); /** [not used] **/
    /* current time is the time to use for next urealtime.realtime update */
    urealtime.start_timing = getnow();

    set_uasmon();
#ifdef CLIPPING
    cliparound(u.ux, u.uy, TRUE);
#endif
    if (u.uhp <= 0 && (!Upolyd || u.mh <= 0)) {
        u.ux = u.uy = 0; /* affects pline() [hence You()] */
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "were not healthy enough to survive restoration.");
        /* wiz1_level.dlevel is used by mklev.c to see if lots of stuff is
         * uninitialized, so we only have to set it and not the other stuff.
         */
        wiz1_level.dlevel = 0;
        u.uz.dnum = 0;
        u.uz.dlevel = 1;
        /* revert to pre-restore option settings */
        iflags.deferred_X = FALSE;
        iflags.perm_invent = defer_perm_invent;
        flags = newgameflags;
#ifdef SYSFLAGS
        sysflags = newgamesysflags;
#endif
        context = *newgamecontext;
        free(newgamecontext);
        return FALSE;
    }
    free(newgamecontext);

    /* in case hangup save occurred in midst of level change */
    assign_level(&u.uz0, &u.uz);

    /* this stuff comes after potential aborted restore attempts */
    restore_killers(fd);
    restore_timers(fd, RANGE_GLOBAL, FALSE, 0L);
    restore_light_sources(fd);
    restore_sound_sources(fd);
    invent = restobjchn(fd, FALSE, FALSE);
    /* tmp_bc only gets set here if the ball & chain were orphaned
       because you were swallowed; otherwise they will be on the floor
       or in your inventory */
    tmp_bc = restobjchn(fd, FALSE, FALSE);
    if (tmp_bc) {
        for (otmp = tmp_bc; otmp; otmp = otmp->nobj) {
            if (otmp->owornmask)
                setwornquietly(otmp, otmp->owornmask);
        }
        if (!uball || !uchain)
            impossible("restgamestate: lost ball & chain");
    }

    magic_objs = restobjchn(fd, FALSE, FALSE);
    migrating_objs = restobjchn(fd, FALSE, FALSE);
    migrating_mons = restmonchn(fd, FALSE);
    mread(fd, (genericptr_t) mvitals, sizeof(mvitals));

    /*
     * There are some things after this that can have unintended display
     * side-effects too early in the game.
     * Disable see_monsters() here, re-enable it at the top of moveloop()
     */
    defer_see_monsters = TRUE;

    /* this comes after inventory has been loaded */
    for (otmp = invent; otmp; otmp = otmp->nobj)
        if (otmp->owornmask)
            setwornquietly(otmp, otmp->owornmask);
    /* reset weapon so that player will get a reminder about "bashing"
       during next fight when bare-handed or wielding an unconventional
       item; for pick-axe, we aren't able to distinguish between having
       applied or wielded it, so be conservative and assume the former */
    otmp = uwep;   /* `uwep' usually init'd by setworn() in loop above */
    uwep = 0;      /* clear it and have setuwep() reinit */
    setuwepquietly(otmp, W_WEP); /* (don't need any null check here) */

    /*
    if (!uwep || uwep->otyp == PICK_AXE || uwep->otyp == GRAPPLING_HOOK)
        unweapon = TRUE;
    */

    restore_dungeon(fd);
    restlevchn(fd);
    mread(fd, (genericptr_t) &moves, sizeof moves);
    mread(fd, (genericptr_t) &monstermoves, sizeof monstermoves);
    mread(fd, (genericptr_t) &quest_status, sizeof (struct q_score));
    mread(fd, (genericptr_t) spl_book, (MAXSPELL + 1) * sizeof (struct spell));
    restore_artifacts(fd);
    restore_oracles(fd);
    if (u.ustuck)
        mread(fd, (genericptr_t) stuckid, sizeof *stuckid);
    if (u.usteed)
        mread(fd, (genericptr_t) steedid, sizeof *steedid);
    mread(fd, (genericptr_t) pl_character, sizeof pl_character);

    mread(fd, (genericptr_t) pl_fruit, sizeof pl_fruit);
    freefruitchn(ffruit); /* clean up fruit(s) made by process_options_file() */
    ffruit = loadfruitchn(fd);

    restnames(fd);
    restore_waterlevel(fd);
    restore_msghistory(fd);
    restore_gamelog(fd);

    /* must come after all mons & objs are restored */
    relink_timers(FALSE);
    relink_light_sources(FALSE);
    relink_sound_sources(FALSE);
    /* inventory display is now viable */
    iflags.perm_invent = defer_perm_invent;

    issue_simple_gui_command(GUI_CMD_LOAD_GLYPHS);
#ifdef WHEREIS_FILE
    touch_whereis();
#endif
    return TRUE;
}

/* update game state pointers to those valid for the current level (so we
 * don't dereference a wild u.ustuck when saving the game state, for instance)
 */
STATIC_OVL void
restlevelstate(stuckid, steedid)
unsigned int stuckid, steedid;
{
    register struct monst *mtmp;

    if (stuckid) {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
            if (mtmp->m_id == stuckid)
                break;
        if (!mtmp)
        {
            panic("Cannot find the monster ustuck.");
            return;
        }
        u.ustuck = mtmp;
    }
    if (steedid) {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
            if (mtmp->m_id == steedid)
                break;
        if (!mtmp)
        {
            panic("Cannot find the monster usteed.");
            return;
        }
        u.usteed = mtmp;
        remove_monster(mtmp->mx, mtmp->my);
    }
}

/*ARGSUSED*/
STATIC_OVL int
restlevelfile(fd, ltmp)
int fd; /* fd used in MFLOPPY only */
xchar ltmp;
{
    int nfd;
    char whynot[BUFSZ];

#ifndef MFLOPPY
    nhUse(fd);
#endif
    nfd = create_levelfile(ltmp, whynot);
    if (nfd < 0) {
        /* BUG: should suppress any attempt to write a panic
           save file if file creation is now failing... */
        panic("restlevelfile: %s", whynot);
        return 0;
    }
#ifdef MFLOPPY
    if (!savelev(nfd, ltmp, COUNT_SAVE)) {
        /* The savelev can't proceed because the size required
         * is greater than the available disk space.
         */
        pline("Not enough space on `%s' to restore your game.", levels);

        /* Remove levels and bones that may have been created.
         */
        (void) nhclose(nfd);
#ifdef AMIGA
        clearlocks();
#else /* !AMIGA */
        eraseall(levels, alllevels);
        eraseall(levels, allbones);

        /* Perhaps the person would like to play without a
         * RAMdisk.
         */
        if (ramdisk) {
            /* PlaywoRAMdisk may not return, but if it does
             * it is certain that ramdisk will be 0.
             */
            playwoRAMdisk();
            /* Rewind save file and try again */
            (void) lseek(fd, (off_t) 0, 0);
            (void) validate(fd, (char *) 0); /* skip version etc */
            return dorestore(fd, FALSE);            /* 0 or 1 */
        }
#endif /* ?AMIGA */
        pline("Be seeing you...");
        nh_terminate(EXIT_SUCCESS);
    }
#endif /* MFLOPPY */
    bufon(nfd);
    savelev(nfd, ltmp, WRITE_SAVE | FREE_SAVE);
    bclose(nfd);
    return 2;
}

int
dorestore(fd, is_backup)
register int fd;
boolean is_backup;
{
    int loadres = dorestore0(fd);
    if (!loadres && !is_backup)
    {
        if (!restore_backup_savefile(TRUE))
        {
            pline("Restoring save file failed.  Replaced save file with back-up save file.");
            mark_synch(); /* flush output */
            fd = open_and_validate_saved_game(FALSE, (boolean*)0);
            if (fd >= 0)
            {
                pline("Restoring back-up save file...");
                mark_synch(); /* flush output */
                loadres = dorestore0(fd);
            }
        }
    }
    if (loadres)
    {
        /* Success! */
        delete_excess_levelfiles();

        /* Welcome */
        welcome(FALSE);
        check_special_room(FALSE);
        return 1;
    }
    return 0;
}

int
dorestore0(fd)
register int fd;
{
    unsigned int stuckid = 0, steedid = 0; /* not a register */
    xchar ltmp;
    int rtmp;
    struct obj *otmp;
    struct save_game_stats game_stats = { 0 };
    struct save_game_stats dummy_stats = { 0 };
    boolean was_corrupted = FALSE;
    Strcpy(debug_buf_1, "dorestore0");
    Strcpy(debug_buf_2, "dorestore0");
    Strcpy(debug_buf_3, "dorestore0");
    Strcpy(debug_buf_4, "dorestore0");

    restoring = TRUE;
    get_plname_from_file(fd, plname);
    get_save_game_stats_from_file(fd, &game_stats);
    getlev(fd, 0, (xchar) 0, FALSE);
    if (!restgamestate(fd, &stuckid, &steedid)) {
        display_nhwindow(WIN_MESSAGE, TRUE);
        savelev(-1, 0, FREE_SAVE); /* discard current level */
        (void) nhclose(fd);
        (void) delete_savefile();
        restoring = FALSE;
        return 0;
    }
    if (!check_save_file_tracking(game_stats.time_stamp)) /* Needs to be here so wizard and other modes have been set */
    {
        savelev(-1, 0, FREE_SAVE); /* discard current level */
        (void)nhclose(fd);
        restoring = FALSE;
        const char* fq_save = fqname(SAVEF, SAVEPREFIX, 1);
        nh_compress(fq_save);
        nh_bail(EXIT_SUCCESS, "Aborting loading the save file due to save file tracking...", TRUE);
        return 0;
    }
    restlevelstate(stuckid, steedid);

    struct u_realtime restored_realtime = urealtime;
#ifdef INSURANCE
    savestateinlock();
#endif
    rtmp = restlevelfile(fd, ledger_no(&u.uz));
    if (rtmp < 2)
        return rtmp; /* dorestore called recursively */

    /* these pointers won't be valid while we're processing the
     * other levels, but they'll be reset again by restlevelstate()
     * afterwards, and in the meantime at least u.usteed may mislead
     * place_monster() on other levels
     */
    u.ustuck = (struct monst *) 0;
    u.usteed = (struct monst *) 0;

#ifdef MICRO
#ifdef AMII_GRAPHICS
    {
        extern struct window_procs amii_procs;
        if (WINDOWPORT("amii") {
            extern winid WIN_BASE;
            clear_nhwindow(WIN_BASE); /* hack until there's a hook for this */
        }
    }
#else
        clear_nhwindow(WIN_MAP);
#endif
    clear_nhwindow(WIN_MESSAGE);
    You("return to level %d in %s%s.", depth(&u.uz),
        dungeons[u.uz.dnum].dname,
        flags.debug ? " while in debug mode"
                    : flags.explore ? " while in explore mode" : "");
    curs(WIN_MAP, 1, 1);
    dotcnt = 0;
    dotrow = 2;
    if (!WINDOWPORT("X11"))
        putstr(WIN_MAP, 0, "Restoring:");
#endif
    while (1) {
        restoreprocs.mread_flags = 1; /* return despite error */
        mread(fd, (genericptr_t) &ltmp, sizeof ltmp);
        if (restoreprocs.mread_flags == -1)
            break;
        restoreprocs.mread_flags = 2; /* return despite error */
        getlev(fd, 0, ltmp, FALSE);
        if (restoreprocs.mread_flags == -2)
            break;
#ifdef MICRO
        curs(WIN_MAP, 1 + dotcnt++, dotrow);
        if (dotcnt >= (COLNO - 1)) {
            dotrow++;
            dotcnt = 0;
        }
        if (!WINDOWPORT("X11")) {
            putstr(WIN_MAP, 0, ".");
        }
        mark_synch();
#endif
        rtmp = restlevelfile(fd, ltmp);
        if (rtmp < 2)
            return rtmp; /* dorestore called recursively */
        if (restoreprocs.mread_flags == -2)
            break;
    }
    if (restoreprocs.mread_flags == -2)
    {
        was_corrupted = TRUE;
        if (query_about_corrupted_savefile())
        {
            (void)nhclose(fd);
            (void)delete_savefile();
            restoring = FALSE;
            return 0;
        }
    }
    restoreprocs.mread_flags = 0;

#ifdef BSD
    (void) lseek(fd, 0L, 0);
#else
    (void) lseek(fd, (off_t) 0, 0);
#endif
    (void) validate(fd, (char *) 0); /* skip version and savefile info */
    get_plname_from_file(fd, plname);
    get_save_game_stats_from_file(fd, &dummy_stats);
    n_game_recoveries = dummy_stats.num_recoveries;

    getlev(fd, 0, (xchar) 0, FALSE);
    (void) nhclose(fd);

    /* Now set the restore settings to match the
     * settings used by the save file output routines
     */
    reset_restpref();

    restlevelstate(stuckid, steedid);
    program_state.something_worth_saving = 1; /* useful data now exists */

    if (!wizard && !discover && !CasualMode)
        (void) delete_savefile();
    if (Is_really_rogue_level(&u.uz))
        assign_graphics(ROGUESET);

#ifdef MFLOPPY
    gameDiskPrompt();
#endif
    max_rank_sz(); /* to recompute mrank_sz (botl.c) */
    /* take care of iron ball & chain */
    for (otmp = fobj; otmp; otmp = otmp->nobj)
        if (otmp->owornmask)
            setwornquietly(otmp, otmp->owornmask);

    update_all_character_properties((struct obj*)0, FALSE);

    /* in_use processing must be after:
     *    + The inventory has been read so that freeinv() works.
     *    + The current level has been restored so billing information
     *      is available.
     */
    inven_inuse(FALSE);
    update_inventory();

    load_qtlist(); /* re-load the quest text info */
    /* Set up the vision internals, after levl[] data is loaded
       but before docrt(). */
    reglyph_darkroom();
    run_timers(); /* expire all timers that have gone off while away */

    vision_reset();
    vision_full_recalc = 1; /* recompute vision (not saved) */
    docrt();
    clear_nhwindow(WIN_MESSAGE);
    status_reassess();

    restoring = FALSE;

    /* Play ambient sounds for the dungeon; check_special_room will play music */
    play_level_ambient_sounds();
    play_environment_ambient_sounds();
    if (!was_corrupted)
        (void)move_tmp_backup_savefile_to_actual_backup_savefile(); /* Restore was successful, update backup savefile */
    else
        (void)delete_tmp_backup_savefile();

    post_restore_to_forum(restored_realtime);
    return 1;
}

STATIC_OVL int
check_save_file_tracking(time_stamp)
int64_t time_stamp;
{
    if (wizard || discover || CasualMode || iflags.save_file_secure)
        return 1;

    if (!iflags.save_file_tracking_supported)
    {
        if (flags.save_file_tracking_value == SAVEFILETRACK_VALID)
        {
            char ans = yn_query("Save file tracking is not supported. Do you want to mark this save file unsuccessfully tracked?");
            if (ans == 'y')
            {
                flags.save_file_tracking_migrated = TRUE;
                flags.save_file_tracking_value = SAVEFILETRACK_INVALID;
                const char* fq_save = fqname(SAVEF, SAVEPREFIX, 0);
                issue_gui_command(GUI_CMD_DELETE_TRACKING_FILE, 0, 0, fq_save);
            }
            else
                return 0; // Return to main menu
        }
    }
    else if (!flags.save_file_tracking_migrated)
    {
        flags.save_file_tracking_migrated = TRUE;
        flags.save_file_tracking_value = SAVEFILETRACK_VALID;
    }
    else if (iflags.save_file_tracking_needed && flags.save_file_tracking_value == SAVEFILETRACK_VALID)
    {
        const char* fq_save = fqname(SAVEF, SAVEPREFIX, 0);
        if (iflags.save_file_tracking_on)
        {
            struct special_view_info info = { 0 };
            info.viewtype = SPECIAL_VIEW_SAVE_FILE_TRACKING_LOAD;
            info.text = fq_save;
            info.time_stamp = time_stamp;
            int errorcode = open_special_view(info);
            if (errorcode > 0)
            {
                //Query if save file tracking should be turned off, turn save file tracking on in settings, or return to main menu
                char ans = yn_query("Save file tracking validation failed. Do you want to mark this save file unsuccessfully tracked?");
                if (ans == 'y')
                {
                    flags.save_file_tracking_value = SAVEFILETRACK_INVALID;
                    issue_gui_command(GUI_CMD_DELETE_TRACKING_FILE, 0, 0, fq_save);
                }
                else
                    return 0; // Return to main menu
            }
        }
        else
        {
            //Save game tracking is needed but is going to be switched off in the save game
            //Query if save file tracking should be turned off, turn save file tracking on in settings, or return to main menu
            char ans = yn_query("Save file tracking is turned off. Do you want to mark this save file unsuccessfully tracked?");
            if (ans == 'y')
            {
                flags.save_file_tracking_value = SAVEFILETRACK_INVALID;
                issue_gui_command(GUI_CMD_DELETE_TRACKING_FILE, 0, 0, fq_save);
            }
            else
                return 0; // Return to main menu
        }
    }
    return 1;
}

STATIC_OVL void
post_restore_to_forum(restored_realtime)
struct u_realtime restored_realtime;
{
    char dgnlvlbuf[BUFSZ * 2];
    print_current_dgnlvl(dgnlvlbuf);
    post_to_forum_rt_printf(LL_GAME_RESTORE, restored_realtime, "resumed %s journey %s", uhis(), dgnlvlbuf);
}

void
restcemetery(fd, cemeteryaddr)
int fd;
struct cemetery **cemeteryaddr;
{
    struct cemetery *bonesinfo, **bonesaddr;
    int flag;
    Strcpy(debug_buf_4, "restcemetery");

    mread(fd, (genericptr_t) &flag, sizeof flag);
    if (flag == 0) {
        bonesaddr = cemeteryaddr;
        do {
            bonesinfo = (struct cemetery *) alloc(sizeof *bonesinfo);
            mread(fd, (genericptr_t) bonesinfo, sizeof *bonesinfo);
            *bonesaddr = bonesinfo;
            bonesaddr = &(*bonesaddr)->next;
        } while (*bonesaddr);
    } else {
        *cemeteryaddr = 0;
    }
}

/*ARGSUSED*/
STATIC_OVL void
rest_levl(fd, rlecomp)
int fd;
boolean rlecomp;
{
#ifdef RLECOMP
    short i, j;
    uchar len;
    struct rm r;

    if (rlecomp) {
        (void) memset((genericptr_t) &r, 0, sizeof(r));
        i = 0;
        j = 0;
        len = 0;
        while (i < ROWNO) {
            while (j < COLNO) {
                if (len > 0) {
                    levl[j][i] = r;
                    len -= 1;
                    j += 1;
                } else {
                    mread(fd, (genericptr_t) &len, sizeof(uchar));
                    mread(fd, (genericptr_t) &r, sizeof(struct rm));
                }
            }
            j = 0;
            i += 1;
        }
        return;
    }
#else /* !RLECOMP */
    nhUse(rlecomp);
#endif /* ?RLECOMP */
    Strcpy(debug_buf_4, "rest_levl");
    mread(fd, (genericptr_t) levl, sizeof levl);
}

void
trickery(reason)
char *reason;
{
    pline("Strange, this map is not as I remember it.");
    pline("Somebody is trying some trickery here...");
    pline("This game is void.");
    Strcpy(killer.name, reason ? reason : "");
    done(TRICKED);
}

void
getlev(fd, pid, lev, ghostly)
int fd, pid;
xchar lev;
boolean ghostly;
{
    register struct trap *trap;
    register struct monst *mtmp;
    int64_t elapsed;
    branch *br;
    int hpid;
    xchar dlvl;
    int x, y;
#ifdef TOS
    short tlev;
#endif
    Strcpy(debug_buf_2, "getlev");
    Strcpy(debug_buf_3, "getlev");
    Strcpy(debug_buf_4, "getlev");

    if (ghostly)
        clear_id_mapping();

#if defined(MSDOS) || defined(OS2)
    setmode(fd, O_BINARY);
#endif
    /* Load the old fruit info.  We have to do it first, so the
     * information is available when restoring the objects.
     */
    if (ghostly)
        oldfruit = loadfruitchn(fd);

    /* First some sanity checks */
    mread(fd, (genericptr_t) &hpid, sizeof(hpid));
/* CHECK:  This may prevent restoration */
#ifdef TOS
    mread(fd, (genericptr_t) &tlev, sizeof(tlev));
    dlvl = tlev & 0x00ff;
#else
    mread(fd, (genericptr_t) &dlvl, sizeof(dlvl));
#endif
    if ((pid && pid != hpid) || (lev && dlvl != lev)) {
        char trickbuf[BUFSZ];

        if (pid && pid != hpid)
            Sprintf(trickbuf, "PID (%d) doesn't match saved PID (%d)!", hpid,
                    pid);
        else
            Sprintf(trickbuf, "This is level %d, not %d!", dlvl, lev);
        if (wizard)
            pline1(trickbuf);
        nhclose(fd);
        trickery(trickbuf);
        return;
    }
    restcemetery(fd, &level.bonesinfo);
    rest_levl(fd,
              (boolean) ((sfrestinfo.sfi1 & SFI1_RLECOMP) == SFI1_RLECOMP));
    mread(fd, (genericptr_t) lastseentyp, sizeof(lastseentyp));
    mread(fd, (genericptr_t) &omoves, sizeof(omoves));
    elapsed = monstermoves - omoves;
    mread(fd, (genericptr_t) &upstair, sizeof(stairway));
    mread(fd, (genericptr_t) &dnstair, sizeof(stairway));
    mread(fd, (genericptr_t) &upladder, sizeof(stairway));
    mread(fd, (genericptr_t) &dnladder, sizeof(stairway));
    mread(fd, (genericptr_t) &sstairs, sizeof(stairway));
    mread(fd, (genericptr_t) &updest, sizeof(dest_area));
    mread(fd, (genericptr_t) &dndest, sizeof(dest_area));
    mread(fd, (genericptr_t) &noteledest, sizeof(dest_area));
    mread(fd, (genericptr_t) &level.flags, sizeof(level.flags));
    mread(fd, (genericptr_t) doors, sizeof(doors));
    rest_rooms(fd); /* No joke :-) */
    if (nroom)
        doorindex = rooms[nroom - 1].fdoor + rooms[nroom - 1].doorct;
    else
        doorindex = 0;

    restore_timers(fd, RANGE_LEVEL, ghostly, elapsed);
    restore_light_sources(fd);
    restore_sound_sources(fd);
    fmon = restmonchn(fd, ghostly);

    rest_worm(fd); /* restore worm information */
    ftrap = 0;
    while (trap = newtrap(),
           mread(fd, (genericptr_t) trap, sizeof(struct trap)),
           trap->tx != 0) { /* need "!= 0" to work around DICE 3.0 bug */
        trap->ntrap = ftrap;
        ftrap = trap;
    }
    dealloc_trap(trap);
    fobj = restobjchn(fd, ghostly, FALSE);
    find_lev_obj();
    /* restobjchn()'s `frozen' argument probably ought to be a callback
       routine so that we can check for objects being buried under ice */
    level.buriedobjlist = restobjchn(fd, ghostly, FALSE);
    billobjs = restobjchn(fd, ghostly, FALSE);
    memoryobjs = restobjchn(fd, ghostly, FALSE);
    find_memory_obj();
    rest_engravings(fd);

    /* reset level.monsters for new level */
    for (x = 0; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++)
            level.monsters[x][y] = (struct monst *) 0;
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (mtmp->isshk)
            set_residency(mtmp, FALSE);
        place_monster(mtmp, mtmp->mx, mtmp->my);
        if (mtmp->wormno)
            place_wsegs(mtmp, NULL);

        /* regenerate monsters while on another level */
        if (!u.uz.dlevel)
            continue;
        if (ghostly) {
            /* reset peaceful/mhostility relative to new character;
               shopkeepers will reset based on name */
            if (!mtmp->isshk)
                mtmp->mpeaceful =
                    (is_unicorn(mtmp->data)
                     && sgn(u.ualign.type) == sgn(mtmp->data->maligntyp))
                        ? TRUE
                        : peace_minded(mtmp->data);
            set_mhostility(mtmp);
            newsym(mtmp->mx, mtmp->my);
        } else if (elapsed > 0L) {
            mon_catchup_elapsed_time(mtmp, elapsed);
        }
        /* update shape-changers in case protection against
           them is different now than when the level was saved */
        restore_cham(mtmp);
        /* give hiders a chance to hide before their next move */
        if (ghostly || elapsed > (int64_t) rnd(10))
            hide_monst(mtmp);
    }

    restdamage(fd, ghostly);
    rest_regions(fd, ghostly);

    if (ghostly) {
        /* Now get rid of all the temp fruits... */
        freefruitchn(oldfruit), oldfruit = 0;

        if (lev > ledger_no(&medusa_level)
            && lev < ledger_no(&stronghold_level) && xdnstair == 0) {
            coord cc;

            mazexy(&cc);
            xdnstair = cc.x;
            ydnstair = cc.y;
            levl[cc.x][cc.y].typ = STAIRS;
        }

        br = Is_branchlev(&u.uz);
        if (br && u.uz.dlevel == 1) {
            d_level ltmp;

            if (on_level(&u.uz, &br->end1))
                assign_level(&ltmp, &br->end2);
            else
                assign_level(&ltmp, &br->end1);

            switch (br->type) {
            case BR_STAIR:
            case BR_NO_END1:
            case BR_NO_END2: /* OK to assign to sstairs if it's not used */
                assign_level(&sstairs.tolev, &ltmp);
                break;
            case BR_PORTAL: /* max of 1 portal per level */
                for (trap = ftrap; trap; trap = trap->ntrap)
                    if (trap->ttyp == MAGIC_PORTAL)
                        break;
                if (!trap)
                {
                    panic("getlev: need portal but none found");
                    return;
                }
                assign_level(&trap->dst, &ltmp);
                break;
            }
        } else if (!br) {
            struct trap *ttmp = 0;

            /* Remove any dangling portals. */
            for (trap = ftrap; trap; trap = ttmp) {
                ttmp = trap->ntrap;
                if (trap->ttyp == MAGIC_PORTAL)
                    deltrap(trap);
            }
        }
    }

    /* must come after all mons & objs are restored */
    relink_timers(ghostly);
    relink_light_sources(ghostly);
    relink_sound_sources(ghostly);
    reset_oattached_mids(ghostly);
    remove_expired_regions(ghostly);

    if (ghostly)
        clear_id_mapping();
}

void
get_plname_from_file(fd, plbuf)
int fd;
char *plbuf;
{
    int pltmpsiz = 0;
    (void) read(fd, (genericptr_t) &pltmpsiz, (readLenType)sizeof(pltmpsiz));
    (void) read(fd, (genericptr_t) plbuf, (readLenType) pltmpsiz);
    return;
}

void
get_save_game_stats_from_file(fd, stats_ptr)
int fd;
struct save_game_stats* stats_ptr;
{
    (void)read(fd, (genericptr_t)stats_ptr, (readLenType)sizeof(struct save_game_stats));
    return;
}

STATIC_OVL void
restore_gamelog(fd)
register int fd;
{
    int slen = 0;
    struct gamelog_line tmp;
    char* tmpstr = 0;
    Strcpy(debug_buf_4, "restore_gamelog");

    while (1) {
        mread(fd, &slen, sizeof slen);
        if (slen < 0)
            break;
        tmpstr = (char*)alloc((size_t)slen + 1);
        mread(fd, tmpstr, (size_t)slen);
        mread(fd, &tmp, sizeof tmp);
        tmpstr[slen] = '\0';
        gamelog_add(tmp.flags, tmp.turn, tmpstr);
        free(tmpstr);
    }
}


STATIC_OVL void
restore_msghistory(fd)
register int fd;
{
    int msgsize, msgcount = 0;
    char msg[BUFSZ];
    char attrs[BUFSZ];
    char colors[BUFSZ];
    Strcpy(debug_buf_4, "restore_msghistory");

    while (1) {
        mread(fd, (genericptr_t) &msgsize, sizeof(msgsize));
        if (msgsize == -1)
            break;
        if (msgsize > (BUFSZ - 1))
        {
            panic("restore_msghistory: msg too big (%d)", msgsize);
            return;
        }
        mread(fd, (genericptr_t) msg, msgsize);
        msg[msgsize] = '\0';
        mread(fd, (genericptr_t) attrs, msgsize);
        attrs[msgsize] = '\0';
        mread(fd, (genericptr_t) colors, msgsize);
        colors[msgsize] = '\0';
        putmsghistory_ex(msg, attrs, colors, TRUE);
        ++msgcount;
    }
    if (msgcount)
        putmsghistory((char *) 0, TRUE);
    debugpline1("Read %d messages from savefile.", msgcount);
}

/* Clear all structures for object and monster ID mapping. */
STATIC_OVL void
clear_id_mapping()
{
    struct bucket *curr;

    while ((curr = id_map) != 0) {
        id_map = curr->next;
        free((genericptr_t) curr);
    }
    n_ids_mapped = 0;
}

/* Add a mapping to the ID map. */
STATIC_OVL void
add_id_mapping(gid, nid)
unsigned gid, nid;
{
    int idx;

    idx = n_ids_mapped % N_PER_BUCKET;
    /* idx is zero on first time through, as well as when a new bucket is */
    /* needed */
    if (idx == 0) {
        struct bucket *gnu = (struct bucket *) alloc(sizeof(struct bucket));
        gnu->next = id_map;
        id_map = gnu;
    }

    id_map->map[idx].gid = gid;
    id_map->map[idx].nid = nid;
    n_ids_mapped++;
}

/*
 * Global routine to look up a mapping.  If found, return TRUE and fill
 * in the new ID value.  Otherwise, return false and return -1 in the new
 * ID.
 */
boolean
lookup_id_mapping(gid, nidp)
unsigned gid, *nidp;
{
    int i;
    struct bucket *curr;

    if (n_ids_mapped)
    {
        for (curr = id_map; curr; curr = curr->next) 
        {
            /* first bucket might not be totally full */
            if (curr == id_map) 
            {
                i = n_ids_mapped % N_PER_BUCKET;
                if (i == 0)
                    i = N_PER_BUCKET;
            } 
            else
                i = N_PER_BUCKET;

            while (--i >= 0)
            {
                if (gid == curr->map[i].gid)
                {
                    *nidp = curr->map[i].nid;
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

STATIC_OVL void
reset_oattached_mids(ghostly)
boolean ghostly;
{
    struct obj *otmp;
    unsigned oldid, nid;
    for (otmp = fobj; otmp; otmp = otmp->nobj) {
        if (ghostly && has_omonst(otmp)) {
            struct monst *mtmp = OMONST(otmp);

            mtmp->m_id = 0;
            mtmp->mpeaceful = mtmp->mtame = 0; /* pet's owner died! */
            mtmp->ispartymember = FALSE;
        }
        if (ghostly && has_omid(otmp)) {
            (void) memcpy((genericptr_t) &oldid, (genericptr_t) OMID(otmp),
                          sizeof(oldid));
            if (lookup_id_mapping(oldid, &nid))
                (void) memcpy((genericptr_t) OMID(otmp), (genericptr_t) &nid,
                              sizeof(nid));
            else
                free_omid(otmp);
        }
    }
}

void
print_current_dgnlvl(buf)
char* buf;
{
    if (!buf)
        return;

    const char* lvl_name = 0;
    s_level* slev = Is_special(&u.uz);
    mapseen* mptr = 0;
    if (slev)
        mptr = find_mapseen(&u.uz);

    if (slev && mptr && mptr->flags.special_level_true_nature_known)
        lvl_name = slev->name;

    char lvlbuf[BUFSZ], dgnbuf[BUFSZ];
    print_dgnlvl_buf(lvlbuf, dgnbuf, lvl_name, dungeons[u.uz.dnum].dname, u.uz.dlevel, (boolean*)0);
    Sprintf(buf, "%s%s", lvlbuf, dgnbuf);
}

void
print_dgnlvl_buf(lvlbuf, dgnbuf, lvl_name, dgn_name, dlevel, has_lvl_name_ptr)
char* lvlbuf, * dgnbuf;
const char* lvl_name, *dgn_name;
int dlevel;
boolean* has_lvl_name_ptr;
{
    if (!lvlbuf || !dgnbuf)
        return;

    Sprintf(dgnbuf, "%s", dgn_name && *dgn_name ? dgn_name : "Dungeon");
    if (!strncmp(dgnbuf, "The ", 4))
        *dgnbuf = lowc(*dgnbuf);

    if(has_lvl_name_ptr)
        *has_lvl_name_ptr = FALSE;
    if (lvl_name && *lvl_name)
    {
        char namedlvlbuf[BUFSZ];
        Strcpy(namedlvlbuf, lvl_name);

        const char* lvlconjunction = "in";
        const char* dgnconjunction = "in";
        boolean lvl_has_level = strstr(lvl_name, "Level") != 0;
        boolean lvl_has_plane = strstr(lvl_name, "Plane") != 0;
        boolean lvl_has_island = strstr(lvl_name, "Island") != 0;
        if (lvl_has_level)
            dgnconjunction = "of";
        if (lvl_has_level || lvl_has_plane || lvl_has_island)
            lvlconjunction = "on";

        const char* addedthe = "";
        if (strncmpi(namedlvlbuf, "the ", 4) && lvl_has_plane)
            addedthe = "the ";
        if (!strncmp(namedlvlbuf, "The ", 4))
            *namedlvlbuf = lowc(*namedlvlbuf);

        Sprintf(lvlbuf, "%s %s%s %s ", lvlconjunction, addedthe, namedlvlbuf, dgnconjunction);
        if(has_lvl_name_ptr)
            *has_lvl_name_ptr = TRUE;
    }
    else
        Sprintf(lvlbuf, "on level %d of ", dlevel);
}

void
print_character_description(characterbuf, ulevel, rolenum, racenum, gender, aligntype, prefix)
char* characterbuf;
short ulevel;
short rolenum;
short racenum;
boolean gender;
aligntyp aligntype;
const char* prefix;
{
    if (!characterbuf)
        return;

    if (!prefix)
        prefix = "";

    char alignbuf[BUFSZ], genderwithspacebuf[BUFSZ], racebuf[BUFSZ], rolebuf[BUFSZ], tmpbuf[BUFSZ];

    strcpy_capitalized_for_title(alignbuf, aligns[1 - aligntype].adj);
    strcpy_capitalized_for_title(racebuf, races[racenum].adj);
    if (roles[rolenum].name.f)
    {
        Strcpy(genderwithspacebuf, "");
        if (gender)
        {
            strcpy_capitalized_for_title(rolebuf, roles[rolenum].name.f);
        }
        else
            strcpy_capitalized_for_title(rolebuf, roles[rolenum].name.m);
    }
    else
    {
        Sprintf(tmpbuf, "%s ", genders[gender].adj);
        strcpy_capitalized_for_title(genderwithspacebuf, tmpbuf);
        strcpy_capitalized_for_title(rolebuf, roles[rolenum].name.m);
    }

    Sprintf(characterbuf, "%sLevel %d %s %s%s %s", prefix, ulevel, alignbuf, genderwithspacebuf, racebuf, rolebuf);
}

void
print_location_description(adventuringbuf, level_name, dgn_name, dlevel, dgn_depth, prefix)
char* adventuringbuf;
const char* level_name;
const char* dgn_name;
int dlevel;
schar dgn_depth;
const char* prefix;
{
    if (!adventuringbuf)
        return;

    if (!prefix)
        prefix = "";

    char lvlbuf[BUFSZ] = "", dgnbuf[BUFSZ] = "", totallevelbuf[BUFSZ] = "";
    boolean has_lvl_name = FALSE;
    print_dgnlvl_buf(lvlbuf, dgnbuf, level_name, dgn_name, dlevel, &has_lvl_name);

    if (!has_lvl_name && dgn_depth != (schar)dlevel)
        Sprintf(totallevelbuf, ", which is dungeon level %d", dgn_depth);
    Sprintf(adventuringbuf, "%sAdventuring %s%s%s", prefix, lvlbuf, dgnbuf, totallevelbuf);
}

void
print_mode_duration_description(playingbuf, difficulty, umoves, debug_mode, explore_mode, modern_mode, casual_mode, is_non_scoring, tournament_mode, prefix)
char* playingbuf;
schar difficulty;
int64_t umoves;
boolean debug_mode, explore_mode, modern_mode, casual_mode, is_non_scoring, tournament_mode;
const char* prefix;
{
    if (!playingbuf)
        return;

    if (!prefix)
        prefix = "";

    Sprintf(playingbuf, "%sPlaying at %s difficulty in %s mode for %lld turns", prefix, get_game_difficulty_text(difficulty),
        get_game_mode_text_core(debug_mode, explore_mode, modern_mode, casual_mode, is_non_scoring, tournament_mode, TRUE),
        (long long)umoves);
}

void
print_timestamp_description(savedbuf, descr, stamp, prefix)
char* savedbuf;
time_t stamp;
const char* descr, * prefix;
{
    if (!savedbuf)
        return;

    if (!prefix)
        prefix = "";

    char timebuf[BUFSZ] = "";
    char* timestr = ctime(&stamp);
    if (timestr && *timestr)
    {
        Strncpy(timebuf, timestr, strlen(timestr) - 1);
        timebuf[strlen(timestr) - 1] = 0;
    }
    else
    {
        Strcpy(timebuf, "unknown date");
    }

    Sprintf(savedbuf, "%s%s %s", prefix, descr, timebuf);
}


#ifdef SELECTSAVED
/* put up a menu listing each character from this player's saved games;
   returns 1: use plname[], 0: new game, -1: quit */
int
restore_menu(bannerwin)
winid bannerwin; /* if not WIN_ERR, clear window and show copyright in menu */
{
    winid tmpwin;
    anything any;
    struct save_game_data *saved;
    menu_item *chosen_action = (menu_item *) 0;
    int ch = 0, ret = 0; /* ch: 0 => new game */
    boolean repeat = TRUE;
    boolean firsttime = TRUE;
    int k;

    do
    {
        *plname = '\0';
        saved = get_saved_games(); /* array of character names */
        repeat = TRUE;
        ch = ret = 0;

        if (firsttime && (!saved || !saved[0].playername))
            return 2; /* start new game straight; there's no menu to return to */

        firsttime = FALSE;

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu_ex(tmpwin, GHMENU_STYLE_START_GAME_MENU);
        any = zeroany; /* no selection */

        if (bannerwin != WIN_ERR)
        {
            /* for tty; erase copyright notice and redo it in the menu */
            clear_nhwindow(bannerwin);
        }

#ifndef GNH_MOBILE
        /* COPYRIGHT_BANNER_[ABCD] */
        for (k = 1; k <= 4; ++k)
            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
                copyright_banner_line(k), MENU_UNSELECTED);
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, "",
            MENU_UNSELECTED);
#endif
        int ccg_saveno = -1;
        if (*recovery_plname && saved)
        {
            char ccgbuf[BUFSZ] = "", rnbuf[BUFSZ] = "";
            boolean found_rplname = FALSE;
            for (k = 0; saved[k].playername && saved[k].playername[0]; k++)
            {
                if (!strcmp(recovery_plname, saved[k].playername) && !saved[k].is_error_save_file && !saved[k].is_imported_save_file)
                {
                    found_rplname = TRUE;
                    ccg_saveno = k;
                    if (saved[k].is_running)
                        break; /* prefer crashed file */
                }
            }
            if (found_rplname)
            {
#ifdef GNH_MOBILE
                char rolebuf[BUFSZ], racebuf[BUFSZ];
                strcpy_capitalized_for_title(racebuf, races[saved[ccg_saveno].gamestats.racenum].adj);
                strcpy_capitalized_for_title(rolebuf, saved[ccg_saveno].gamestats.gender && roles[saved[ccg_saveno].gamestats.rolenum].name.f ? roles[saved[ccg_saveno].gamestats.rolenum].name.f : roles[saved[ccg_saveno].gamestats.rolenum].name.m);

                size_t totlen = strlen(recovery_plname) + strlen(racebuf) + strlen(rolebuf);
                if(totlen >= PL_NSIZ)
                    Sprintf(rnbuf, " (%s)", recovery_plname);
                else
                    Sprintf(rnbuf, " (%s, %s %s)", recovery_plname, racebuf, rolebuf);
#endif
                Sprintf(ccgbuf, "Continue Current Game%s", rnbuf);
                any.a_int = 3;
                add_menu(tmpwin, NO_GLYPH, &any, 'c', 0, ATR_HEADING, 
                    saved[ccg_saveno].is_running ? CLR_MAGENTA : saved[ccg_saveno].is_error_save_file ? CLR_BROWN : saved[ccg_saveno].is_imported_save_file ? CLR_BLUE : NO_COLOR,
                    ccgbuf,
                    MENU_UNSELECTED);
            }
        }

        char ngbuf[BUFSZ] = "", modebuf[BUFSZ] = "", descbuf[BUFSZ] = "";
        strcpy_capitalized_for_title(modebuf, get_game_mode_text(FALSE));
#ifdef GNH_MOBILE
        char dtmpbuf[BUFSZ];
        Strcpy(dtmpbuf, get_game_mode_description());
        *dtmpbuf = highc(*dtmpbuf);
        Sprintf(descbuf, " (%s)", dtmpbuf);
#endif
        if (TournamentMode)
            Sprintf(ngbuf, "New %s Game%s", modebuf, descbuf);
        else
            Sprintf(ngbuf, "New Game in %s Mode%s", modebuf, descbuf);

        any.a_int = -1;
        add_menu(tmpwin, NO_GLYPH, &any, 'n', 0, ATR_HEADING, NO_COLOR, ngbuf,
            MENU_UNSELECTED);

        if (saved && saved[0].playername)
        {
            any.a_int = 1;
            add_menu(tmpwin, NO_GLYPH, &any, 'l', 0, ATR_HEADING, NO_COLOR, "Load Saved Game",
                MENU_UNSELECTED);

            any.a_int = 2;
            add_menu(tmpwin, NO_GLYPH, &any, 'd', 0, ATR_HEADING, NO_COLOR, "Delete Saved Game",
                MENU_UNSELECTED);
        }

        any.a_int = -2;
        add_menu(tmpwin, NO_GLYPH, &any, 'q', 0, ATR_HEADING, NO_COLOR, "Quit",
            MENU_UNSELECTED);

#ifdef GNH_MOBILE
        end_menu(tmpwin, "GnollHack");
#else
        end_menu(tmpwin, (char*)0);
#endif

        if (select_menu(tmpwin, PICK_ONE, &chosen_action) > 0)
        {
            ret = chosen_action->item.a_int;
            free((genericptr_t)chosen_action);
        }
        else
            ret = -2;

        destroy_nhwindow(tmpwin);

        if (bannerwin != WIN_ERR)
        {
            /* for tty; clear the menu away and put subset of copyright back
                */
            clear_nhwindow(bannerwin);
        }

        switch (ret)
        {
        case 1: /* Load Saved Game */
        case 2: /* Delete Saved Game */
            ch = select_saved_game(bannerwin, ret - 1, saved);
            if (ch > 0)
                repeat = FALSE;
            break;
        case 3: /* Continue Current Game */
            if (ccg_saveno >= 0 && saved[ccg_saveno].playername > 0)
            {
                ch = ccg_saveno + 1;
                Strcpy(plname, saved[ccg_saveno].playername);
                plname_from_error_savefile = saved[ccg_saveno].is_error_save_file;
                plname_from_imported_savefile = saved[ccg_saveno].is_imported_save_file;
                repeat = FALSE;
            }
            else /* Start new game; something's wrong */
            {
                ch = 0;
                repeat = FALSE;
            }
            break;
        case -1: /* New Game */
            ch = 0;
            repeat = FALSE;
            break;
        case -2:
        default: /* Quit */
            ch = -1;
            repeat = FALSE;
            break;
        }

        if (bannerwin != WIN_ERR)
        {
            /* for tty; clear the menu away and put subset of copyright back
                */
            clear_nhwindow(bannerwin);
            /* COPYRIGHT_BANNER_A, preceding "Who are you?" prompt */
            if (ch == 0)
            {
                int i;
                for (i = 1; i <= 4; ++i)
                    putstr(bannerwin, 0, copyright_banner_line(i));
                putstr(bannerwin, 0, "");
            }
        }

        free_saved_games(saved);
    } while (repeat);
    return (ch > 0) ? 1 : ch;
}

int
select_saved_game(bannerwin, style, saved)
winid bannerwin; /* if not WIN_ERR, clear window and show copyright in menu */
uchar style;
struct save_game_data* saved;
{
    if (!saved)
        return 0;

    boolean repeat = FALSE;
    int ch = 0;
    do
    {
        repeat = FALSE;
        ch = 0;
        winid tmpwin;
        anything any;
        menu_item* chosen_game = (menu_item*)0;
        int k, clet; /* ch: 0 => new game */
        const char* titlestr = style == 0 ? "Load a Saved Game" : "Delete a Saved Game";

        tmpwin = create_nhwindow(NHW_MENU);
        start_menu_ex(tmpwin, style == 0 ? GHMENU_STYLE_CHOOSE_SAVED_GAME : GHMENU_STYLE_DELETE_SAVED_GAME);
        any = zeroany; /* no selection */

    #ifndef GNH_MOBILE
        add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR,
            titlestr, MENU_UNSELECTED);
    #endif

#if defined(TTY_GRAPHICS) || defined(CURSES_GRAPHICS)
        char prefix[8] = "    ";
#else
        char prefix[8] = "";
#endif
        for (k = 0; saved[k].playername; ++k)
        {
            char namebuf[BUFSZ], characterbuf[BUFSZ], adventuringbuf[BUFSZ];
            char playingbuf[BUFSZ], savedbuf[BUFSZ];
            //char adventuringbuf[BUFSZ], lvlbuf[BUFSZ], dgnbuf[BUFSZ], totallevelbuf[BUFSZ] = "";
            //boolean has_lvl_name = FALSE;

            Sprintf(namebuf, "%s%s%s%s", saved[k].playername, saved[k].is_running ? " [Crashed]" : "", saved[k].is_error_save_file ? " [Saved upon Error]" : "", saved[k].is_imported_save_file ? " [Imported]" : "");
            print_character_description(characterbuf, saved[k].gamestats.ulevel, saved[k].gamestats.rolenum, saved[k].gamestats.racenum, saved[k].gamestats.gender, saved[k].gamestats.alignment, prefix);
            print_location_description(adventuringbuf, saved[k].gamestats.level_name, saved[k].gamestats.dgn_name, (int)saved[k].gamestats.dlevel, saved[k].gamestats.depth, prefix);
            print_mode_duration_description(playingbuf, saved[k].gamestats.game_difficulty, saved[k].gamestats.umoves, saved[k].gamestats.debug_mode, saved[k].gamestats.explore_mode, saved[k].gamestats.modern_mode, saved[k].gamestats.casual_mode, (boolean)((saved[k].gamestats.save_flags& SAVEFLAGS_NON_SCORING) != 0), (boolean)((saved[k].gamestats.save_flags& SAVEFLAGS_TOURNAMENT_MODE) != 0), prefix);
            print_timestamp_description(savedbuf, "Game was saved on", (time_t)saved[k].gamestats.time_stamp, prefix);

            //print_dgnlvl_buf(lvlbuf, dgnbuf, saved[k].gamestats.level_name, saved[k].gamestats.dgn_name, (int)saved[k].gamestats.dlevel, &has_lvl_name);
            //if (!has_lvl_name && saved[k].gamestats.depth != (schar)saved[k].gamestats.dlevel)
            //    Sprintf(totallevelbuf, ", which is dungeon level %d", saved[k].gamestats.depth);

            //Sprintf(adventuringbuf, "%sAdventuring %s%s%s", prefix, lvlbuf, dgnbuf, totallevelbuf);
            //Sprintf(playingbuf, "%sPlaying at %s difficulty in %s mode for %lld turns", prefix, get_game_difficulty_text(saved[k].gamestats.game_difficulty),
            //    get_game_mode_text_core(saved[k].gamestats.debug_mode, saved[k].gamestats.explore_mode, saved[k].gamestats.modern_mode, saved[k].gamestats.casual_mode, (boolean)((saved[k].gamestats.save_flags & SAVEFLAGS_NON_SCORING) != 0), (boolean)((saved[k].gamestats.save_flags & SAVEFLAGS_TOURNAMENT_MODE) != 0), TRUE),
            //    (long long)saved[k].gamestats.umoves);
            //time_t stamp = (time_t)saved[k].gamestats.time_stamp;
            //char* timestr = ctime(&stamp);
            //if (timestr && *timestr)
            //{
            //    Strncpy(timebuf, timestr, strlen(timestr) - 1);
            //    timebuf[strlen(timestr) - 1] = 0;
            //}
            //else
            //{
            //    Strcpy(timebuf, "unknown date");
            //}

            //Sprintf(savedbuf, "%sGame was saved on %s", prefix, timebuf);

            //int glyph = saved[k].gamestats.glyph;
            int gui_glyph = saved[k].gamestats.gui_glyph;
            any.a_int = k + 1;

            add_menu(tmpwin, gui_glyph, &any, 0, 0, ATR_HEADING | ATR_BOLD, 
                style == 1 ? CLR_RED : saved[k].is_running ? CLR_MAGENTA : saved[k].is_error_save_file ? CLR_BROWN : saved[k].is_imported_save_file ? CLR_BLUE : NO_COLOR, 
                namebuf, MENU_UNSELECTED);

            any.a_int = 0;
            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, characterbuf,
                MENU_UNSELECTED);

            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, adventuringbuf,
                MENU_UNSELECTED);

            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, playingbuf,
                MENU_UNSELECTED);

            add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_NONE, NO_COLOR, savedbuf,
                MENU_UNSELECTED);
        }

        clet = (k + 1 <= 'q' - 'a') ? 'q' : 0; /* back to menu */
        any.a_int = -1;
        add_menu(tmpwin, NO_GLYPH, &any, clet, 0, ATR_HEADING, NO_COLOR,
            "Quit back to menu", MENU_SELECTED);

    #ifdef GNH_MOBILE
        end_menu(tmpwin, titlestr);
    #else
        /* no prompt on end_menu, as we've done our own at the top */
        end_menu(tmpwin, (char*)0);
    #endif

        int n = select_menu(tmpwin, PICK_ONE, &chosen_game);
        boolean shoulddelete = FALSE;
        boolean is_running = FALSE;
        if (n > 0)
        {
            ch = chosen_game->item.a_int;
            if (ch > 0 && saved[ch - 1].playername > 0)
            {
                Strcpy(plname, saved[ch - 1].playername);
                plname_from_error_savefile = saved[ch - 1].is_error_save_file;
                plname_from_imported_savefile = saved[ch - 1].is_imported_save_file;
                is_running = saved[ch - 1].is_running;
                if (style == 1)
                {
                    shoulddelete = TRUE;
                }
            }
            else if (ch < 0)
                ch = 0; // ++ch; /* -1 -> 0 (new game), -2 -> -1 (quit) */

            free((genericptr_t)chosen_game);
        }
        else
        {
            ch = 0; /* back to menu */
        }

        if (shoulddelete)
        {
            if (bannerwin != WIN_ERR)
                clear_nhwindow(bannerwin);
            char qbuf[QBUFSZ], tbuf[BUFSZ];
            if (is_running)
            {
                Sprintf(qbuf, "Are you sure to delete the crashed game files for \'%s\'?", plname);
                Strcpy(tbuf, "Delete Crashed Game?");
            }
            else
            {
                Sprintf(qbuf, "Are you sure to delete the saved game for \'%s\'?", plname);
                Strcpy(tbuf, "Delete Saved Game?");
            }
            int ans = yn_query_ex(ATR_NONE, CLR_RED, tbuf, qbuf);
            if (ans == 'y')
            {
                set_savefile_name(TRUE);
                if (is_running)
                {
                    delete_running_files();
                    /* Save file exists */
                    boolean save_exists = check_existing_save_file();
                    boolean error_save_exists = check_existing_error_save_file();
                    if (!save_exists && !error_save_exists) /* Saved game can be deleted separately from the menu */
                    {
                        boolean backup_save_exists = check_has_backup_savefile();
                        if (backup_save_exists)
                        {
                            Strcpy(tbuf, "Restore Backup?");
                            Sprintf(qbuf, "Do you want to restore the backup saved game for \'%s\'? Answering \'No\' will delete the backup.", plname);
                            ans = yn_query_ex(ATR_NONE, CLR_GREEN, tbuf, qbuf);
                            switch (ans)
                            {
                            default:
                            case 'y':
                                restore_backup_savefile(TRUE);
                                break;
                            case 'n':
                                delete_tmp_backup_savefile();
                                delete_backup_savefile();
                                break;
                            }
                        }
                    }
                }
                else
                {
                    delete_tmp_backup_savefile();
                    delete_backup_savefile();
                    delete_error_savefile();
                    delete_savefile();
                }
            }
            else
                repeat = TRUE;

            *plname = 0;
            ch = 0;
        }

        destroy_nhwindow(tmpwin);

    } while (repeat);

    if (bannerwin != WIN_ERR)
    {
        /* for tty; clear the menu away and put subset of copyright back
         */
        clear_nhwindow(bannerwin);
        /* COPYRIGHT_BANNER_A, preceding "Who are you?" prompt */
        int i;
        for (i = 1; i <= 4; ++i)
            putstr(bannerwin, 0, copyright_banner_line(i));
        putstr(bannerwin, 0, "");
    }
    return ch;
}

#endif /* SELECTSAVED */

boolean
check_saved_game_exists(void)
{
    struct save_game_data* saved = get_saved_games(); /* array of character names */
    if (saved)
    {
        int i = 0;
        while (saved[i].playername)
        {
            if (!strcmp(saved[i].playername, plname) && !saved[i].is_running)
            {
                free_saved_games(saved);
                return TRUE;
            }
            i++;
        }
    }
    free_saved_games(saved);
    return FALSE;
}


void
minit()
{
    (*restoreprocs.restore_minit)();
    return;
}

void
mread(fd, buf, len)
register int fd;
register genericptr_t buf;
register size_t len;
{
    (*restoreprocs.restore_mread)(fd, buf, len);
    return;
}

/* examine the version info and the savefile_info data
   that immediately follows it.
   Return 0 if it passed the checks.
   Return 1 if it failed the version check.
   Return 2 if it failed the savefile feature check.
   Return -1 if it failed for some unknown reason.
 */
int
validate(fd, name)
int fd;
const char *name;
{
    int rlen;
    struct savefile_info sfi;
    uint64_t compatible;
    boolean verbose = name ? TRUE : FALSE, reslt = FALSE;

    if (!(reslt = uptodate(fd, name)))
        return 1;

    rlen = (int)read(fd, (genericptr_t) &sfi, (readLenType)sizeof sfi);
    minit(); /* ZEROCOMP */
    if (rlen == 0) {
        if (verbose) {
            pline("File \"%s\" is empty during save file feature check?",
                  name);
            wait_synch();
        }
        return -1;
    }

    compatible = (sfi.sfi1 & sfcap.sfi1);

    if ((sfi.sfi1 & SFI1_ZEROCOMP) == SFI1_ZEROCOMP) {
        if ((compatible & SFI1_ZEROCOMP) != SFI1_ZEROCOMP) {
            if (verbose) {
                pline("File \"%s\" has incompatible ZEROCOMP compression.",
                      name);
                wait_synch();
            }
            return 2;
        } else if ((sfrestinfo.sfi1 & SFI1_ZEROCOMP) != SFI1_ZEROCOMP) {
            set_restpref("zerocomp");
        }
    }

    if ((sfi.sfi1 & SFI1_EXTERNALCOMP) == SFI1_EXTERNALCOMP) {
        if ((compatible & SFI1_EXTERNALCOMP) != SFI1_EXTERNALCOMP) {
            if (verbose) {
                pline("File \"%s\" lacks required internal compression.",
                      name);
                wait_synch();
            }
            return 2;
        } else if ((sfrestinfo.sfi1 & SFI1_EXTERNALCOMP)
                   != SFI1_EXTERNALCOMP) {
            set_restpref("externalcomp");
        }
    }

    /* RLECOMP check must be last, after ZEROCOMP or INTERNALCOMP adjustments
     */
    if ((sfi.sfi1 & SFI1_RLECOMP) == SFI1_RLECOMP) {
        if ((compatible & SFI1_RLECOMP) != SFI1_RLECOMP) {
            if (verbose) {
                pline("File \"%s\" has incompatible run-length compression.",
                      name);
                wait_synch();
            }
            return 2;
        } else if ((sfrestinfo.sfi1 & SFI1_RLECOMP) != SFI1_RLECOMP) {
            set_restpref("rlecomp");
        }
    }
    /* savefile does not have RLECOMP level location compression, so adjust */
    else
        set_restpref("!rlecomp");

    return 0;
}

void
reset_restpref(VOID_ARGS)
{
#ifdef ZEROCOMP
    if (iflags.zerocomp)
        set_restpref("zerocomp");
    else
#endif
        set_restpref("externalcomp");
#ifdef RLECOMP
    if (iflags.rlecomp)
        set_restpref("rlecomp");
    else
#endif
        set_restpref("!rlecomp");
}

void
set_restpref(suitename)
const char *suitename;
{
    if (!strcmpi(suitename, "externalcomp")) {
        restoreprocs.name = "externalcomp";
        restoreprocs.restore_mread = def_mread;
        restoreprocs.restore_minit = def_minit;
        sfrestinfo.sfi1 |= SFI1_EXTERNALCOMP;
        sfrestinfo.sfi1 &= ~SFI1_ZEROCOMP;
        def_minit();
    }
    if (!strcmpi(suitename, "!rlecomp")) {
        sfrestinfo.sfi1 &= ~SFI1_RLECOMP;
    }
#ifdef ZEROCOMP
    if (!strcmpi(suitename, "zerocomp")) {
        restoreprocs.name = "zerocomp";
        restoreprocs.restore_mread = zerocomp_mread;
        restoreprocs.restore_minit = zerocomp_minit;
        sfrestinfo.sfi1 |= SFI1_ZEROCOMP;
        sfrestinfo.sfi1 &= ~SFI1_EXTERNALCOMP;
        zerocomp_minit();
    }
#endif
#ifdef RLECOMP
    if (!strcmpi(suitename, "rlecomp")) {
        sfrestinfo.sfi1 |= SFI1_RLECOMP;
    }
#endif
}

#ifdef ZEROCOMP
#define RLESC '\0' /* Leading character for run of RLESC's */

#ifndef ZEROCOMP_BUFSIZ
#define ZEROCOMP_BUFSIZ BUFSZ
#endif
STATIC_VAR NEARDATA unsigned char inbuf[ZEROCOMP_BUFSIZ];
STATIC_VAR NEARDATA unsigned short inbufp = 0;
STATIC_VAR NEARDATA unsigned short inbufsz = 0;
STATIC_VAR NEARDATA short inrunlength = -1;
STATIC_VAR NEARDATA int mreadfd;

STATIC_OVL int
zerocomp_mgetc()
{
    if (inbufp >= inbufsz) {
        inbufsz = read(mreadfd, (genericptr_t) inbuf, sizeof inbuf);
        if (!inbufsz) {
            if (inbufp > sizeof inbuf)
                error("EOF on file #%d.\n", mreadfd);
            inbufp = 1 + sizeof inbuf; /* exactly one warning :-) */
            return -1;
        }
        inbufp = 0;
    }
    return inbuf[inbufp++];
}

STATIC_OVL void
zerocomp_minit()
{
    inbufsz = 0;
    inbufp = 0;
    inrunlength = -1;
}

STATIC_OVL void
zerocomp_mread(fd, buf, len)
int fd;
genericptr_t buf;
register size_t len;
{
    /*register int readlen = 0;*/
    if (fd < 0)
        error("Restore error; mread attempting to read file %d.", fd);
    mreadfd = fd;
    while (len--) {
        if (inrunlength > 0) {
            inrunlength--;
            *(*((char **) &buf))++ = '\0';
        } else {
            register short ch = zerocomp_mgetc();
            if (ch < 0) {
                restoreprocs.mread_flags = -1;
                return;
            }
            if ((*(*(char **) &buf)++ = (char) ch) == RLESC) {
                inrunlength = zerocomp_mgetc();
            }
        }
        /*readlen++;*/
    }
}
#endif /* ZEROCOMP */

STATIC_OVL void
def_minit()
{
    return;
}

STATIC_OVL void
def_mread(fd, buf, len)
register int fd;
register genericptr_t buf;
register size_t len;
{
    register int rlen;

    rlen = (int)read(fd, buf, (readLenType) len);
    if ((readLenType) rlen != (readLenType) len) {
        if (restoreprocs.mread_flags == 1) { /* means "return anyway", no corruption */
            restoreprocs.mread_flags = -1;
            return;
        } else if (restoreprocs.mread_flags == 2) { /* means "return anyway", apparent corruption */
            restoreprocs.mread_flags = -2;
            return;
        } else {
            pline("Read %d instead of %zu bytes.", rlen, len);
            if (restoring) {
                (void) nhclose(fd);
                (void) delete_tmp_backup_savefile();
                (void) ask_delete_invalid_savefile("corrupted", TRUE);
                error("Error restoring old game.");
            }
            panic("Error reading level file: buf1=%s, buf2=%s, buf3=%s, buf4=%s", debug_buf_1, debug_buf_2, debug_buf_3, debug_buf_4);
            return;
        }
    }
}

/*restore.c*/
