/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0    bones.c    $NHDT-Date: 1557092711 2019/05/05 21:45:11 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.75 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985,1993. */
/*-Copyright (c) Robert Patrick Rankin, 2012. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h"

extern char bones[]; /* from files.c */
#ifdef MFLOPPY
extern int64_t bytes_counted;
#endif

STATIC_DCL boolean FDECL(no_bones_level, (d_level *));
STATIC_DCL void FDECL(goodfruit, (int));
STATIC_DCL void FDECL(resetobjs, (struct obj *, BOOLEAN_P));
STATIC_DCL boolean FDECL(fixuporacle, (struct monst *));
STATIC_DCL void FDECL(mark_all_fruits_good, (struct obj*));

STATIC_OVL boolean
no_bones_level(lev)
d_level *lev;
{
    extern d_level save_dlevel; /* in do.c */
    s_level *sptr;

    if (ledger_no(&save_dlevel))
        assign_level(lev, &save_dlevel);

    return (boolean) (((sptr = Is_special(lev)) != 0 && !sptr->boneid)
                      || !dungeons[lev->dnum].boneid
                      /* no bones on the last or multiway branch levels
                         in any dungeon (level 1 isn't multiway) */
                      || Is_botlevel(lev)
                      || (Is_branchlev(lev) && lev->dlevel > 1)
                      || (u.uz.dnum == main_dungeon_dnum && u.uz.dlevel == 1)
                      /* no bones in the invocation level */
                      || (In_hell(lev)
                          && lev->dlevel == dunlevs_in_dungeon(lev) - 1));
}

/* Call this function for each fruit object saved in the bones level: it marks
 * that particular type of fruit as existing (the marker is that that type's
 * ID is positive instead of negative).  This way, when we later save the
 * chain of fruit types, we know to only save the types that exist.
 */
STATIC_OVL void
goodfruit(id)
int id;
{
    struct fruit *f = fruit_from_indx(-id);

    if (f)
        f->fid = id;
}

STATIC_OVL void
resetobjs(ochain, restore)
struct obj *ochain;
boolean restore;
{
    struct obj *otmp, *nobj;
    Strcpy(debug_buf_3, "resetobjs");

    for (otmp = ochain; otmp; otmp = nobj)
    {
        nobj = otmp->nobj;
        if (otmp->cobj)
            resetobjs(otmp->cobj, restore);
        
        if (otmp->in_use) 
        {
            Strcpy(debug_buf_2, "resetobjs");
            obj_extract_self(otmp);
            dealloc_obj(otmp);
            continue;
        }

        if (restore)
        {
            if (has_uoname(otmp))
            {
                sanitize_name(UONAME(otmp));
            }
            /* artifact bookkeeping needs to be done during
               restore; other fixups are done while saving */
            if (otmp->oartifact)
            {
                if (exist_artifact(otmp->otyp, safe_oname(otmp)) || is_quest_artifact(otmp)) 
                {
                    /* prevent duplicate--revert to ordinary obj */
                    /* Non-generable base item*/
                    if (objects[otmp->otyp].oc_prob == 0 || (objects[otmp->otyp].oc_flags3 & O3_NO_GENERATION) != 0)
                    {
                        if (artilist[otmp->oartifact].maskotyp && objects[artilist[otmp->oartifact].maskotyp].oc_prob > 0 && !(objects[artilist[otmp->oartifact].maskotyp].oc_flags3 & O3_NO_GENERATION))
                        {
                            otmp->otyp = artilist[otmp->oartifact].maskotyp;
                            /* Keep the artifact's material for weapons or armor; should not create particularly odd situations with artifacts of these classes */
                            if(objects[otmp->otyp].oc_class != WEAPON_CLASS && objects[otmp->otyp].oc_class != ARMOR_CLASS)
                                otmp->material = objects[otmp->otyp].oc_material;
                            if (otmp->charges != 0 && objects[otmp->otyp].oc_charged == CHARGED_NOT_CHARGED)
                            {
                                if (objects[otmp->otyp].oc_enchantable > ENCHTYPE_NO_ENCHANTMENT)
                                {
                                    int maxench = get_max_enchantment(objects[otmp->otyp].oc_enchantable);
                                    otmp->enchantment = max(-maxench, min(maxench, otmp->enchantment + otmp->charges));
                                }
                                otmp->charges = 0;
                            }
                            if (otmp->enchantment != 0 && objects[otmp->otyp].oc_enchantable == ENCHTYPE_NO_ENCHANTMENT)
                            {
                                if (objects[otmp->otyp].oc_charged > CHARGED_NOT_CHARGED)
                                {
                                    short maxcharges = get_max_charge(objects[otmp->otyp].oc_charged);
                                    otmp->charges = max(0, min(maxcharges, otmp->enchantment + otmp->charges));
                                }
                                otmp->enchantment = 0;
                            }
                        }
                        else
                        {
                            otmp->otyp = random_objectid_from_class(otmp->oclass, (struct monst*)0, MKOBJ_FLAGS_ALSO_RARE);
                            otmp->elemental_enchantment = 0;
                            otmp->exceptionality = 0;
                            otmp->material = objects[otmp->otyp].oc_material;
                            otmp->mythic_prefix = 0;
                            otmp->mythic_suffix = 0;
                            otmp->charges = 0;
                            otmp->enchantment = 0;
                        }
                    }
                    else
                    {
                        otmp->material = objects[otmp->otyp].oc_material; /* Base material may have been randomized (using the dead character's randomization) */
                    }
                    otmp->oartifact = 0;
                    otmp->owt = weight(otmp);
                    if (has_oname(otmp))
                        free_oname(otmp);

                } 
                else
                {
                    if (artilist[otmp->oartifact].material == MAT_NONE)
                    {
                        otmp->material = objects[otmp->otyp].oc_material; /* Base material may have been randomized (using the dead character's randomization) */
                        otmp->owt = weight(otmp);
                    }
                    artifact_exists(otmp, safe_oname(otmp), TRUE);
                }
            }
            else
            {
                if (has_oname(otmp))
                {
                    sanitize_name(ONAME(otmp));
                }
                if (objects[otmp->otyp].oc_material_init_type == MATINIT_BASE_MATERIAL)
                {
                    otmp->material = objects[otmp->otyp].oc_material; /* Base material may have been randomized (using the dead character's randomization) */
                    otmp->owt = weight(otmp);
                }
            }
        }
        else 
        { /* saving */
            /* do not zero out o_ids for ghost levels anymore */

            if (objects[otmp->otyp].oc_uses_known)
                otmp->known = 0;
            otmp->dknown = otmp->bknown = 0;
            otmp->rknown = 0;
            otmp->lknown = 0;
            otmp->tknown = 0;
            otmp->cknown = 0;
            otmp->aknown = 0;
            otmp->nknown = 0;
            otmp->mknown = 0;
            otmp->rotknown = 0;
            otmp->invlet = 0;
            otmp->no_charge = 0;
            otmp->was_thrown = 0;

            /* strip user-supplied names */
            /* Statue and some corpse names are left intact,
               presumably in case they came from score file.
               [TODO: this ought to be done differently--names
               which came from such a source or came from any
               stoned or killed monster should be flagged in
               some manner; then we could just check the flag
               here and keep "real" names (dead pets, &c) while
               discarding player notes attached to statues.] */
            free_uoname(otmp);
            if (has_oname(otmp)
                && !(otmp->oartifact || otmp->otyp == STATUE
                     || otmp->otyp == SPE_NOVEL
                     || otmp->otyp == SPE_MANUAL
                     || (otmp->otyp == CORPSE
                         && otmp->corpsenm >= SPECIAL_PM))) 
            {
                free_oname(otmp);
            }

            if (otmp->otyp == SLIME_MOLD)
            {
                goodfruit(otmp->special_quality);
#ifdef MAIL
            }
            else if (otmp->otyp == SCR_MAIL)
            {
                /* 0: delivered in-game via external event;
                   1: from bones or wishing; 2: written with marker */
                if (otmp->special_quality == 0)
                    otmp->special_quality = SPEQUAL_MAIL_FROM_BONES_OR_WISHING;
#endif
            } 
            else if (otmp->otyp == EGG) 
            {
                otmp->speflags &= ~SPEFLAGS_YOURS; /* not "laid by you" in next game */
            } 
            else if (otmp->otyp == TIN) 
            {
                /* make tins of unique monster's meat be empty */
                if (otmp->corpsenm >= LOW_PM
                    && unique_corpstat(&mons[otmp->corpsenm]))
                    otmp->corpsenm = NON_PM;
            }
            else if (otmp->otyp == CORPSE || otmp->otyp == STATUE) 
            {
                int mnum = otmp->corpsenm;

                /* Discard incarnation details of unique monsters
                   (by passing null instead of otmp for object),
                   shopkeepers (by passing false for revival flag),
                   temple priests, and vault guards in order to
                   prevent corpse revival or statue reanimation. */
                if (has_omonst(otmp)
                    && cant_revive(&mnum, FALSE, (struct obj *) 0)) 
                {
                    free_omonst(otmp);
                    /* mnum is now either human_zombie or doppelganger;
                       for corpses of uniques, we need to force the
                       transformation now rather than wait until a
                       revival attempt, otherwise eating this corpse
                       would behave as if it remains unique */
                    if (mnum == PM_DOPPELGANGER && otmp->otyp == CORPSE)
                        set_corpsenm(otmp, mnum);
                }
            }
            else if ((otmp->otyp == iflags.mines_prize_type
                        && !Is_mineend_level(&u.uz))
                       || ((otmp->otyp == iflags.soko_prize_type1
                            || otmp->otyp == iflags.soko_prize_type2)
                           && !Is_sokoend_level(&u.uz))) 
            {
                /* "special prize" in this game becomes ordinary object
                   if loaded into another game */
                otmp->speflags &= ~(SPEFLAGS_MINES_PRIZE | SPEFLAGS_SOKO_PRIZE1 | SPEFLAGS_SOKO_PRIZE2);
            }
            else if (otmp->otyp == AMULET_OF_YENDOR) 
            {
                /* no longer the real Amulet */
                otmp->otyp = FAKE_AMULET_OF_YENDOR;
                otmp->material = objects[otmp->otyp].oc_material;
                curse(otmp);
            }
            else if (otmp->otyp == CANDELABRUM_OF_INVOCATION) 
            {
                if (otmp->lamplit)
                    end_burn(otmp, TRUE);
                otmp->otyp = WAX_CANDLE;
                otmp->age = 50L; /* assume used */
                if (otmp->special_quality > 0)
                    otmp->quan = (int64_t) otmp->special_quality;
                otmp->special_quality = 0;
                otmp->material = objects[otmp->otyp].oc_material;
                curse(otmp);
            }
            else if (otmp->otyp == BELL_OF_OPENING)
            {
                otmp->otyp = BELL;
                curse(otmp);
            }
            else if (otmp->otyp == SPE_BOOK_OF_THE_DEAD) 
            {
                otmp->otyp = SPE_BLANK_PAPER;
                otmp->material = objects[otmp->otyp].oc_material;
                otmp->owt = weight(otmp);
                curse(otmp);
            } 
            else if (otmp->otyp == SPE_BOOK_OF_MODRON) 
            {
                otmp->otyp = SPE_BLANK_PAPER;
                otmp->oartifact = 0;
                otmp->material = objects[otmp->otyp].oc_material;
                otmp->owt = weight(otmp);
                curse(otmp);
            }
            otmp->owt = weight(otmp);
        }
    }
}

/* while loading bones, strip out text possibly supplied by old player
   that might accidentally or maliciously disrupt new player's display */
void
sanitize_name(namebuf)
char *namebuf;
{
    int c;
    boolean strip_8th_bit = (WINDOWPORT("tty")
                             && !iflags.wc_eight_bit_input);

    /* it's tempting to skip this for single-user platforms, since
       only the current player could have left these bones--except
       things like "hearse" and other bones exchange schemes make
       that assumption false */
    while (*namebuf) {
        c = *namebuf & 0177;
        if (c < ' ' || c == '\177') {
            /* non-printable or undesirable */
            *namebuf = '.';
        } else if (c != *namebuf) {
            /* expected to be printable if user wants such things */
            if (strip_8th_bit)
                *namebuf = '_';
        }
        ++namebuf;
    }
}

/* called by savebones(); also by finish_paybill(shk.c) */
void
drop_upon_death(mtmp, cont, x, y)
struct monst *mtmp; /* monster if hero turned into one (other than ghost) */
struct obj *cont; /* container if hero is turned into a statue */
int x, y;
{
    struct obj *otmp;

    /* Mark all player-specified fruits in inventory good */
    mark_all_fruits_good(invent);

    u.twoweap = 0; /* ensure curse() won't cause swapwep to drop twice */
    while ((otmp = invent) != 0) 
    {
        Strcpy(debug_buf_2, "drop_upon_death");
        Strcpy(debug_buf_3, "drop_upon_death");
        obj_extract_self(otmp);
        /* when turning into green slime, all gear remains held;
           other types "arise from the dead" do aren't holding
           equipment during their brief interval as a corpse */
        if (!mtmp || is_undead(mtmp->data))
            obj_no_longer_held(otmp);

        otmp->owornmask = 0L;
        /* lamps don't go out when dropped */
        if ((cont || artifact_light(otmp) || obj_shines_magical_light(otmp) || has_obj_mythic_magical_light(otmp)) && obj_is_burning(otmp))
            end_burn(otmp, TRUE); /* smother in statue */

        if (rn2(5))
            curse(otmp);
        if (mtmp)
            (void) add_to_minv(mtmp, otmp);
        else if (cont)
            (void) add_to_container(cont, otmp);
        else
            place_object(otmp, x, y);
    }
    if (cont)
        cont->owt = weight(cont);
}

STATIC_OVL
void
mark_all_fruits_good(objchn)
struct obj* objchn;
{
    if (!objchn)
        return;
    
    for (struct obj* otmp = objchn; otmp; otmp = otmp->nobj)
    {
        if (otmp->otyp == SLIME_MOLD)
            goodfruit(otmp->special_quality);
        else if (Is_proper_container(otmp) && otmp->cobj)
            mark_all_fruits_good(otmp->cobj);
    }

}

/* possibly restore oracle's room and/or put her back inside it; returns
   False if she's on the wrong level and should be removed, True otherwise */
STATIC_OVL boolean
fixuporacle(oracle)
struct monst *oracle;
{
    coord cc;
    int ridx, o_ridx;

    /* oracle doesn't move, but knight's joust or monk's staggering blow
       could push her onto a hole in the floor; at present, traps don't
       activate in such situation hence she won't fall to another level;
       however, that could change so be prepared to cope with such things */
    if (!Is_oracle_level(&u.uz))
        return FALSE;

    oracle->mpeaceful = 1;
    o_ridx = levl[oracle->mx][oracle->my].roomno - ROOMOFFSET;
    if (o_ridx >= 0 && rooms[o_ridx].rtype == DELPHI)
        return TRUE; /* no fixup needed */

    /*
     * The Oracle isn't in DELPHI room.  Either hero entered her chamber
     * and got the one-time welcome message, converting it into an
     * ordinary room, or she got teleported out, or both.  Try to put
     * her back inside her room, if necessary, and restore its type.
     */

    /* find original delphi chamber; should always succeed */
    for (ridx = 0; ridx < SIZE(rooms); ++ridx)
        if (rooms[ridx].orig_rtype == DELPHI)
            break;

    if (o_ridx != ridx && ridx < SIZE(rooms)) {
        /* room found and she's not not in it, so try to move her there */
        cc.x = (rooms[ridx].lx + rooms[ridx].hx) / 2;
        cc.y = (rooms[ridx].ly + rooms[ridx].hy) / 2;
        if (enexto(&cc, cc.x, cc.y, oracle->data)) {
            rloc_to(oracle, cc.x, cc.y);
            o_ridx = levl[oracle->mx][oracle->my].roomno - ROOMOFFSET;
        }
        /* [if her room is already full, she might end up outside;
           that's ok, next hero just won't get any welcome message,
           same as used to happen before this fixup was introduced] */
    }
    if (ridx == o_ridx)
    {/* if she's in her room, mark it as such */
        rooms[ridx].rtype = DELPHI;
    }
    return TRUE; /* keep oracle in new bones file */
}

/* check whether bones are feasible */
boolean
can_make_bones(VOID_ARGS)
{
    register struct trap *ttmp;

    /* don't let multiple restarts generate multiple copies of objects
       in bones files */
    if (discover || ModernMode || CasualMode || flags.non_scoring) // In ModernMode bones files could work, but the player is not supposed to die in that mode, so something odd would have happened to get here
        return FALSE;
#if !defined(DEBUG) && defined(GNH_MOBILE)
    if (wizard)
        return FALSE;
#endif
    if (!flags.bones)
        return FALSE;
    if (ledger_no(&u.uz) <= 0 || ledger_no(&u.uz) > maxledgerno())
        return FALSE;
    if (no_bones_level(&u.uz))
        return FALSE; /* no bones for specific levels */
    if (u.uswallow) 
        return FALSE; /* no bones when swallowed */

    if (!Is_branchlev(&u.uz)) {
        /* no bones on non-branches with portals */
        for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap)
            if (ttmp->ttyp == MAGIC_PORTAL)
                return FALSE;
    }

    if (depth(&u.uz) <= 0                 /* bulletproofing for endgame */
        || (!rn2(1 + (depth(&u.uz) >> 2)) /* fewer ghosts on low levels */
            && !wizard))
        return FALSE;
    return TRUE;
}

/* save bones and possessions of a deceased adventurer */
void
savebones(how, when, corpse)
int how;
time_t when;
struct obj *corpse;
{
    int fd, x, y;
    struct trap *ttmp;
    struct monst *mtmp;
    struct permonst *mptr;
    struct fruit *f;
    struct cemetery *newbones;
    char c, *bonesid;
    char whynot[BUFSZ];

    /* caller has already checked `can_make_bones()' */

    clear_bypasses();
    fd = open_bonesfile(&u.uz, &bonesid);
    if (fd >= 0) {
        (void) nhclose(fd);
        if (wizard) {
            if (yn_query("Bones file already exists.  Replace it?") == 'y') {
                if (delete_bonesfile(&u.uz))
                    goto make_bones;
                else
                    pline("Cannot unlink old bones.");
            }
        }
        /* compression can change the file's name, so must
           wait until after any attempt to delete this file */
        compress_bonesfile();
        return;
    }

make_bones:
    program_state.in_bones = 1; /* Stays on until program termination */
    unleash_all();
    /* in case these characters are not in their home bases */
    int minmlev = 0, maxmlev = 0;
    get_generated_monster_minmax_levels(0, &minmlev, &maxmlev, 0);
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        mptr = mtmp->data;
        if (((mptr->difficulty > maxmlev || (mptr->geno & G_UNIQ) != 0)
            && !(mtmp->mon_flags & MON_FLAGS_SPLEVEL_RESIDENT)
            && !(Invocation_lev(&u.uz) && mptr == &mons[PM_BAPHOMET])
            && !(Is_sanctum(&u.uz) && mptr == &mons[PM_HIGH_PRIEST])
            && !(Inhell && mptr == &mons[PM_DEMOGORGON])
            && !(Inhell && mptr == &mons[PM_ASMODEUS])
            && !((mtmp->mpeaceful && (mptr->geno & G_UNIQ) == 0) || mtmp->isshk || mtmp->issmith || mtmp->isnpc || mtmp->ispriest)
            )
            || mtmp->iswiz 
            || (is_medusa(mptr) && !Is_medusa_level(&u.uz))
            || mptr->msound == MS_NEMESIS || mptr->msound == MS_LEADER
            || mptr == &mons[PM_VLAD_THE_IMPALER]
            || (mptr == &mons[PM_ORACLE] && !fixuporacle(mtmp)))
            mongone(mtmp);
        else if (!mtmp->mpeaceful && (is_watch(mptr) || mptr->msound == MS_LEADER || mtmp->isshk || mtmp->issmith || mtmp->isnpc || mtmp->ispriest)) /* Shopkeepers are pacified upon loading */
            mtmp->mpeaceful = 1;
    }
    if (u.usteed)
        dismount_steed(DISMOUNT_BONES);
    dmonsfree(); /* discard dead or gone monsters */

    /* mark all fruits as nonexistent; when we come to them we'll mark
     * them as existing (using goodfruit())
     */
    for (f = ffruit; f; f = f->nextf)
        f->fid = -f->fid;

    /* check iron balls separately--maybe they're not carrying it */
    if (uball)
        uball->owornmask = uchain->owornmask = 0L;

    /* dispose of your possessions, usually cursed */
    if (u.ugrave_arise == (NON_PM - 1)) {
        struct obj *otmp;

        /* embed your possessions in your statue */
        otmp = mk_named_object(STATUE, &mons[u.umonnum], u.ux, u.uy, plname);
        otmp->nknown = 1;

        drop_upon_death((struct monst *) 0, otmp, u.ux, u.uy);
        if (!otmp)
            return; /* couldn't make statue */
        mtmp = (struct monst *) 0;
    } else if (u.ugrave_arise < LOW_PM) {
        /* drop everything */
        drop_upon_death((struct monst *) 0, (struct obj *) 0, u.ux, u.uy);
        /* trick makemon() into allowing monster creation
         * on your location
         */
        in_mklev = TRUE;
        mtmp = makemon(&mons[PM_GHOST], u.ux, u.uy, MM_NONAME);
        mtmp->u_know_mname = 1;
        in_mklev = FALSE;
        if (!mtmp)
            return;
        mtmp = christen_monst(mtmp, plname);
        if (corpse)
            (void) obj_attach_mid(corpse, mtmp->m_id);
    } else {
        /* give your possessions to the monster you become */
        in_mklev = TRUE; /* use <u.ux,u.uy> as-is */
        mtmp = makemon(&mons[u.ugrave_arise], u.ux, u.uy, MM_NO_MONSTER_INVENTORY);
        in_mklev = FALSE;
        if (!mtmp) { /* arise-type might have been genocided */
            drop_upon_death((struct monst *) 0, (struct obj *) 0, u.ux, u.uy);
            u.ugrave_arise = NON_PM; /* in case caller cares */
            return;
        }
        mtmp = christen_monst(mtmp, plname);
        mtmp->u_know_mname = 1;
        newsym(u.ux, u.uy);
        /* ["Your body rises from the dead as an <mname>..." used
           to be given here, but it has been moved to done() so that
           it gets delivered even when savebones() isn't called] */
        drop_upon_death(mtmp, (struct obj *) 0, u.ux, u.uy);
        /* 'mtmp' now has hero's inventory; if 'mtmp' is a mummy, give it
           a wrapping unless already carrying one */
        if (mtmp->data->mlet == S_GREATER_UNDEAD && !m_carrying(mtmp, MUMMY_WRAPPING))
            (void) mongets(mtmp, MUMMY_WRAPPING);
        m_dowear(mtmp, TRUE, FALSE);
    }
    if (mtmp) {
        mtmp->m_lev = (u.ulevel ? u.ulevel : 1);
        mtmp->mbasehpmax = u.ubasehpmax;
        mtmp->mbasehpdrain = 0; /* Undead are cured on hp drain */
        update_mon_maxhp(mtmp);
        mtmp->mhp = mtmp->mhpmax;
        mtmp->female = flags.female;
        mtmp->heads_left = mtmp->data->heads;
        mtmp->msleeping = 1;
    }
    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        resetobjs(mtmp->minvent, FALSE);
        /* do not zero out m_ids for bones levels any more */
        mtmp->mlstmv = 0L;
        if (mtmp->mtame)
        {
            mtmp->mtame = mtmp->mpeaceful = 0;
            mtmp->ispartymember = FALSE;
        }
    }
    for (ttmp = ftrap; ttmp; ttmp = ttmp->ntrap) {
        ttmp->madeby_u = 0;
        //ttmp->madeby_mon = 0; //Keep madeby_mon flags to indicate that the traps are not original (mostly relevant in Sokoban)
        ttmp->tseen = (ttmp->ttyp == HOLE);
    }
    unsee_engravings();
    resetobjs(fobj, FALSE);
    resetobjs(level.buriedobjlist, FALSE);

    /* Hero is no longer on the map. */
    u.ux0 = u.ux, u.uy0 = u.uy;
    u.ux = u.uy = 0;

    /* Clear all memory from the level. */
    clear_memoryobjs(); /* Blanket removal just in case */
    for (x = 1; x < COLNO; x++)
        for (y = 0; y < ROWNO; y++) {
            levl[x][y].seenv = 0;
            clear_hero_memory_at(x, y);
            lastseentyp[x][y] = UNDEFINED_LOCATION;
        }

    /* Attach bones info to the current level before saving. */
    newbones = (struct cemetery *) alloc(sizeof *newbones);
    /* entries are '\0' terminated but have fixed length allocations,
       so pre-fill with spaces to initialize any excess room */
    (void) memset((genericptr_t) newbones, ' ', sizeof *newbones);
    /* format name+role,&c, death reason, and date+time;
       gender and alignment reflect final values rather than what the
       character started out as, same as topten and logfile entries */
#ifdef ANDROID
    Sprintf(newbones->who, "%s-%.3s-%.3s-%.3s-%.3s", wizard ? "wizard" : plname, urole.filecode,
#else
    Sprintf(newbones->who, "%s-%.3s-%.3s-%.3s-%.3s", plname, urole.filecode,
#endif
        urace.filecode, genders[flags.female].filecode,
            aligns[1 - u.ualign.type].filecode);
    formatkiller(newbones->how, sizeof newbones->how, how, TRUE);
    Strcpy(newbones->when, yyyymmddhhmmss(when));
    /* final resting place, used to decide when bones are discovered */
    newbones->frpx = u.ux0, newbones->frpy = u.uy0;
    newbones->bonesknown = FALSE;
    /* if current character died on a bones level, the cemetery list
       will have multiple entries, most recent (this dead hero) first */
    newbones->next = level.bonesinfo;
    level.bonesinfo = newbones;
    /* flag these bones if they are being created in wizard mode;
       they might already be flagged as such, even when we're playing
       in normal mode, if this level came from a previous bones file */
    if (wizard)
        level.flags.wizard_bones = 1;

    fd = create_bonesfile(&u.uz, &bonesid, whynot);
    if (fd < 0) {
        if (wizard)
            pline1(whynot);
        /* bones file creation problems are silent to the player.
         * Keep it that way, but place a clue into the paniclog.
         */
        paniclog("savebones", whynot);
        return;
    }
    c = (char) (strlen(bonesid) + 1);

#ifdef MFLOPPY /* check whether there is room */
    if (iflags.checkspace) {
        savelev(fd, ledger_no(&u.uz), COUNT_SAVE);
        /* savelev() initializes bytes_counted to 0, so it must come
         * first here even though it does not in the real save.  the
         * resulting extra bflush() at the end of savelev() may increase
         * bytes_counted by a couple over what the real usage will be.
         *
         * note it is safe to call store_version() here only because
         * bufon() is null for ZEROCOMP, which MFLOPPY uses -- otherwise
         * this code would have to know the size of the version
         * information itself.
         */
        store_version(fd);
        store_savefileinfo(fd);
        bwrite(fd, (genericptr_t) &c, sizeof c);
        bwrite(fd, (genericptr_t) bonesid, (size_t) c); /* DD.nnn */
        savefruitchn(fd, COUNT_SAVE);
        bflush(fd);
        if (bytes_counted > freediskspace(bones)) { /* not enough room */
            if (wizard)
                pline("Insufficient space to create bones file.");
            (void) nhclose(fd);
            cancel_bonesfile();
            return;
        }
        co_false(); /* make sure stuff before savelev() gets written */
    }
#endif /* MFLOPPY */

    store_version(fd);
    store_savefileinfo(fd);
    bwrite(fd, (genericptr_t) &c, sizeof c);
    bwrite(fd, (genericptr_t) bonesid, (size_t) c); /* DD.nnn */
    savefruitchn(fd, WRITE_SAVE);
    update_mlstmv(); /* update monsters for eventual restoration */
    savelev(fd, ledger_no(&u.uz), WRITE_SAVE);
    bclose(fd);
    commit_bonesfile(&u.uz);
    compress_bonesfile();
#if !defined(COMPRESS) && !defined(ZLIB_COMP)
    if (issue_gui_command && !flags.non_scoring && flags.save_file_tracking_value)
    {
        const char* fq_bones = fqname(bones, BONESPREFIX, 0);
        issue_gui_command(GUI_CMD_POST_BONES_FILE, context.game_difficulty - MIN_DIFFICULTY_LEVEL, 0, fq_bones);
    }
#endif
}

int
getbones(VOID_ARGS)
{
    register int fd;
    register int ok;
    char c, *bonesid, oldbonesid[40]; /* was [10]; more should be safer */

    if (discover || ModernMode || CasualMode) /* save bones files for classic mode games; note that in ModernMode bones files could work but since the player does not die, bones files are a bit pointless */
        return 0;

    if (!flags.bones)
        return 0;

    /* wizard check added by GAN 02/05/87 */
    if (rn2(3) /* only once in three times do we find bones */
        && !wizard)
        return 0;
    if (no_bones_level(&u.uz))
        return 0;
    fd = open_bonesfile(&u.uz, &bonesid);
    if (fd < 0)
        return 0;

    if (validate(fd, bones) != 0) {
        if (!wizard)
            pline("Discarding unuseable bones; no need to panic...");
        ok = FALSE;
        (void)nhclose(fd);
    } else {
        ok = TRUE;
        if (wizard) {
            if (yn_query("Get bones?") == 'n') {
                (void) nhclose(fd);
                compress_bonesfile();
                return 0;
            }
        }
        Strcpy(debug_buf_1, "getbones");
        Strcpy(debug_buf_2, "getbones");
        Strcpy(debug_buf_3, "getbones");
        Strcpy(debug_buf_4, "getbones");
        mread(fd, (genericptr_t) &c, sizeof c); /* length incl. '\0' */
        mread(fd, (genericptr_t) oldbonesid, (size_t) c); /* DD.nnn */
        if (strcmp(bonesid, oldbonesid) != 0
            /* from 3.3.0 through 3.6.0, bones in the quest branch stored
               a bogus bonesid in the file; 3.6.1 fixed that, but for
               3.6.0 bones to remain compatible, we need an extra test;
               once compatibility with 3.6.x goes away, this can too
               (we don't try to make this conditional upon the value of
               VERSION_COMPATIBILITY because then we'd need patchlevel.h) */
            && (strlen(bonesid) <= 2
                || strcmp(bonesid + 2, oldbonesid) != 0)) {
            char errbuf[BUFSZ];

            Sprintf(errbuf, "This is bones level '%s', not '%s'!", oldbonesid,
                    bonesid);
            if (wizard) {
                pline1(errbuf);
                ok = FALSE; /* won't die of trickery */
            }
            (void)nhclose(fd);
            trickery(errbuf);
        } else {
            register struct monst *mtmp;

            getlev(fd, 0, 0, TRUE);

            /* Note that getlev() now keeps tabs on unique
             * monsters such as demon lords, and tracks the
             * birth counts of all species just as makemon()
             * does.  If a bones monster is extinct or has been
             * subject to genocide, their mhpmax will be
             * set to the magic DEFUNCT_MONSTER cookie value.
             */
            for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
                if (has_mname(mtmp))
                    sanitize_name(MNAME(mtmp));
                if (has_umname(mtmp))
                    sanitize_name(UMNAME(mtmp));
                if (mtmp->mbasehpmax == DEFUNCT_MONSTER) {
                    if (wizard) {
                        debugpline1("Removing defunct monster %s from bones.",
                                    mon_monster_name(mtmp));
                    }
                    mongone(mtmp);
                } else
                    /* to correctly reset named artifacts on the level */
                    resetobjs(mtmp->minvent, TRUE);
            }
            resetobjs(fobj, TRUE);
            resetobjs(level.buriedobjlist, TRUE);
        }
        (void)nhclose(fd);
    }
    sanitize_engravings();
    u.uroleplay.numbones++;

    if (wizard) {
        if (yn_query("Unlink bones?") == 'n') {
            compress_bonesfile();
            return ok;
        }
    }
    if (!delete_bonesfile(&u.uz)) {
        /* When N games try to simultaneously restore the same
         * bones file, N-1 of them will fail to delete it
         * (the first N-1 under AmigaDOS, the last N-1 under UNIX).
         * So no point in a mysterious message for a normal event
         * -- just generate a new level for those N-1 games.
         */
        /* pline("Cannot unlink bones."); */
        return 0;
    }
    return ok;
}

/*bones.c*/
