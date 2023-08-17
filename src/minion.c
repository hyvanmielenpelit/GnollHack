/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    minion.c    $NHDT-Date: 1544998886 2018/12/16 22:21:26 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.40 $ */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/*-Copyright (c) Robert Patrick Rankin, 2008. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

void
newemin(mtmp)
struct monst *mtmp;
{
    if (!mtmp->mextra)
        mtmp->mextra = newmextra();
    if (!EMIN(mtmp))
    {
        EMIN(mtmp) = (struct emin *) alloc(sizeof(struct emin));
        (void) memset((genericptr_t) EMIN(mtmp), 0, sizeof(struct emin));
    }
}

void
free_emin(mtmp)
struct monst *mtmp;
{
    if (has_emin(mtmp)) 
    {
        free((genericptr_t) EMIN(mtmp));
        EMIN(mtmp) = (struct emin *) 0;
    }
    mtmp->isminion = 0;
}

/* count the number of monsters on the level */
int
monster_census(spotted)
boolean spotted; /* seen|sensed vs all */
{
    struct monst *mtmp;
    int count = 0;

    for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
        if (DEADMONSTER(mtmp))
            continue;
        if (spotted && !canspotmon(mtmp))
            continue;
        ++count;
    }
    return count;
}

/* mon summons a monster */
int
msummon(mon)
struct monst *mon;
{
    struct permonst *ptr;
    int dtype = NON_PM, cnt = 0, result = 0, census;
    aligntyp atyp;
    struct monst *mtmp;
    unsigned long mmanimtype = 0UL;

    if (mon) 
    {
        ptr = mon->data;

        if (item_prevents_summoning(mon->mnum)) 
        {
            if (canseemon(mon))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s looks puzzled for a moment.", Monnam(mon));
            return 0;
        }

        atyp = mon->ispriest ? EPRI(mon)->shralign
                             : mon->isminion ? EMIN(mon)->min_align
                                             : (ptr->maligntyp == A_NONE)
                                                   ? A_NONE
                                                   : sgn(ptr->maligntyp);
    }
    else 
    {
        ptr = &mons[PM_WIZARD_OF_YENDOR];
        atyp = (ptr->maligntyp == A_NONE) ? A_NONE : sgn(ptr->maligntyp);
    }

    if (ptr == &mons[PM_FLIND_LORD])
    {
        mmanimtype = MM_CHAOTIC_SUMMON_ANIMATION;

        dtype = (!rn2(80)) ? PM_YEENAGHU : (!rn2(40)) ? monsndx(ptr) : (!rn2(4)) ? ndemon(atyp, FALSE, TRUE) : PM_FLIND;
        if (dtype == PM_YEENAGHU && (mvitals[PM_YEENAGHU].mvflags & MV_GONE))
            dtype = monsndx(ptr);
        if (dtype == PM_FLIND && (mvitals[PM_FLIND].mvflags & MV_GONE))
            dtype = ndemon(atyp, FALSE, TRUE);
        if (dtype == NON_PM)
            return 0;
        cnt = 1;
    }
    else if (ptr == &mons[PM_YACC])
    {
        mmanimtype = MM_CHAOTIC_SUMMON_ANIMATION;

        dtype = PM_HELL_BOVINE;
        cnt = !rn2(2) ? 2 : 1;
    }
    else if (is_dprince(ptr) || (ptr == &mons[PM_WIZARD_OF_YENDOR]))
    {
        mmanimtype = MM_CHAOTIC_SUMMON_ANIMATION;
        
        dtype = (!rn2(20)) ? dlord(atyp, FALSE, TRUE) : ndemon(atyp, FALSE, TRUE); //(!rn2(50)) ? dprince(atyp) : 
        if (dtype == NON_PM)
            return 0;
        cnt = (!rn2(3) && is_ndemon(&mons[dtype])) ? rnd(2) + 1 : 1;
    }
    else if (is_dlord(ptr))
    {
        mmanimtype = MM_CHAOTIC_SUMMON_ANIMATION;
        
        dtype = (!rn2(80)) ? dprince(atyp, FALSE, TRUE) : (!rn2(40)) ? dlord(atyp, FALSE, TRUE)
                                                        : ndemon(atyp, FALSE, TRUE);
        if (dtype == NON_PM)
            return 0;
        cnt = (!rn2(4) && is_ndemon(&mons[dtype])) ? 2 : 1;
    } 
    else if (is_ndemon(ptr)) 
    {
        mmanimtype = MM_CHAOTIC_SUMMON_ANIMATION;
        dtype = (!rn2(80)) ? dlord(atyp, FALSE, TRUE) : (!rn2(6)) ? ndemon(atyp, FALSE, TRUE)
                                                     : monsndx(ptr);
        if (dtype == NON_PM)
            return 0;
        cnt = 1;
    }
    else if (mon && is_lminion(mon))
    {
        mmanimtype = MM_LAWFUL_SUMMON_ANIMATION;
        dtype = (is_lord(ptr) && !rn2(40))
                    ? llord(FALSE, TRUE)
                    : (is_lord(ptr) || !rn2(6)) ? lminion(FALSE, TRUE) : mon->mnum;
        if (dtype == NON_PM)
            return 0;
        cnt = (!rn2(4) && !is_lord(&mons[dtype])) ? 2 : 1;
    }
    else if (ptr == &mons[PM_ANGEL])
    {
        mmanimtype = MM_LAWFUL_SUMMON_ANIMATION;
        /* non-lawful angels can also summon */
        if (!rn2(6))
        {
            switch (atyp)
            { /* see summon_minion */
            case A_NEUTRAL:
                dtype = PM_AIR_ELEMENTAL + rn2(4);
                break;
            case A_CHAOTIC:
            case A_NONE:
                dtype = ndemon(atyp, FALSE, TRUE);
                break;
            }
        }
        else 
        {
            dtype = PM_ANGEL;
        }
        if (dtype == NON_PM)
            return 0;
        cnt = (!rn2(4) && !is_lord(&mons[dtype])) ? 2 : 1;
    }

    if (dtype == NON_PM)
        return 0;

    /* sanity checks */
    if (cnt > 1 && (mons[dtype].geno & G_UNIQ))
        cnt = 1;
    /*
     * If this daemon is unique and being re-summoned (the only way we
     * could get this far with an extinct dtype), try another.
     */
    if (mvitals[dtype].mvflags & MV_GONE)
    {
        dtype = ndemon(atyp, FALSE, TRUE);
        if (dtype == NON_PM)
            return 0;
    }

    /* some candidates can generate a group of monsters, so simple
       count of non-null makemon() result is not sufficient */
    census = monster_census(FALSE);

    context.makemon_spef_idx = 0;
    while (cnt > 0)
    {
        mtmp = makemon_limited(&mons[dtype], u.ux, u.uy, MM_EMIN | (dtype == PM_ANGEL ? MM_ROAMER : 0UL) | MM_PLAY_SUMMON_ANIMATION | mmanimtype | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL), 0UL, 0, 0, 0, 0, 0, dtype == PM_ANGEL ? atyp : 0);
        if (mtmp) 
        {
            context.makemon_spef_idx++;
            mtmp->mdemonsummon_used = 30;
            mtmp->mspecialsummon_used = 30;
            mtmp->mspecialsummon2_used = 30;
            result++;
            /* an angel's alignment should match the summoner */
            //if (dtype == PM_ANGEL)
            //{
            //    mtmp->isminion = 1;
            //    EMIN(mtmp)->min_align = atyp;
            //    /* renegade if same alignment but not peaceful
            //       or peaceful but different alignment */
            //    EMIN(mtmp)->renegade =
            //        (atyp != u.ualign.type) ^ !is_peaceful(mtmp);
            //}
            play_sfx_sound_at_location(SFX_SUMMON_DEMON, mtmp->mx, mtmp->my);
            if(canseemon(mtmp))
            { 
                if (is_demon(ptr))
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears in a cloud of smoke!", Amonnam(mtmp));
                else
                    pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears!", Amonnam(mtmp));
            }
        }
        cnt--;
    }
    makemon_animation_wait_until_end();

    /* how many monsters exist now compared to before? */
    if (result)
        result = monster_census(FALSE) - census;

    return result;
}


/* Yeenaghu summons gnolls */
int
yeenaghu_gnoll_summon(summoner)
struct monst* summoner;
{
    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s summons some gnolls!", Monnam(summoner));

    int dtype = NON_PM, cnt = 0, result = 0, census;
    struct monst* mtmp = (struct monst*) 0;

    cnt = d(1, 3);

    /* some candidates can generate a group of monsters, so simple
       count of non-null makemon() result is not sufficient */
    census = monster_census(FALSE);

    int canseemonnumber = 0;
    struct monst* mtmp2 = (struct monst *) 0;
    context.makemon_spef_idx = 0;

    while (cnt > 0)
    {
        int roll = rn2(15);
        dtype = NON_PM;

        switch (roll)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            dtype = PM_GNOLL;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            dtype = PM_GNOLL_LORD;
            break;
        case 9:
        case 10:
        case 11:
            dtype = PM_GNOLL_KING;
            break;
        case 12:
        case 13:
            dtype = PM_FLIND;
            break;
        case 14:
            dtype = PM_FLIND_LORD;
            break;
        default:
            break;

        }

        if (dtype == NON_PM || mvitals[dtype].mvflags & MV_GONE)
        {
            cnt--;
            continue;
        }

        mtmp = makemon(&mons[dtype], u.ux, u.uy, MM_ANGRY | MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL));
        if (mtmp) 
        {
            context.makemon_spef_idx++;
            result++;
            mtmp->mdemonsummon_used = 30;
            mtmp->mspecialsummon_used = 30;
            mtmp->mspecialsummon2_used = 30;
            if (canseemon(mtmp))
            {
                canseemonnumber++;
                mtmp2 = mtmp;
            }
        }

        cnt--;
    }
    makemon_animation_wait_until_end();

    char numberword[BUFSZ] = "Some";
    if(canseemonnumber >= 3)
        Strcpy(numberword, "Several");

    if (result > 0 && canseemonnumber > 0) {
        if (result == 1 && mtmp)
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears out of nowhere!", Amonnam(mtmp));
        else
        {
            if (canseemonnumber == 1 && mtmp2)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears out of nowhere!", Amonnam(mtmp2));
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s gnolls appear out of nowhere!", numberword);
        }
    }

    /* how many monsters exist now compared to before? */
    if (result)
        result = monster_census(FALSE) - census;

    return result;
}

/* Yeenaghu summons ghouls */
int
yeenaghu_ghoul_summon(summoner)
struct monst* summoner;
{
    pline_ex(ATR_NONE, CLR_MSG_SPELL, "%s summons some undead assistance!", Monnam(summoner));

    int dtype = NON_PM, cnt = 0, result = 0, census;
    struct monst* mtmp = (struct monst*) 0;

    cnt = d(1, 3);

    /* some candidates can generate a group of monsters, so simple
       count of non-null makemon() result is not sufficient */
    census = monster_census(FALSE);

    int canseemonnumber = 0;
    struct monst* mtmp2 = (struct monst*) 0;
    int ghoul_cnt = 0;
    int ghast_cnt = 0;
    int thoul_cnt = 0;

    context.makemon_spef_idx = 0;
    while (cnt > 0)
    {
        dtype = !rn2(8) ? PM_THOUL : !rn2(2) ? PM_GHOUL : PM_GHAST;

        if (mvitals[dtype].mvflags & MV_GONE)
        {
            cnt--;
            break;
        }

        mtmp = makemon(&mons[dtype], u.ux, u.uy, MM_ANGRY | MM_PLAY_SUMMON_ANIMATION | MM_UNDEAD_SUMMON_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL));
        if (mtmp) 
        {
            context.makemon_spef_idx++;
            result++;
            mtmp->mdemonsummon_used = 30;
            mtmp->mspecialsummon_used = 30;
            mtmp->mspecialsummon2_used = 30;

            if (dtype == PM_GHOUL)
                ghoul_cnt++;
            else if (dtype == PM_GHAST)
                ghoul_cnt++;
            else
                thoul_cnt++;

            if (canseemon(mtmp))
            {
                canseemonnumber++;
                mtmp2 = mtmp;
            }
        }
        cnt--;
    }
    makemon_animation_wait_until_end();

    char numberword[BUFSZ] = "Some";
    if (canseemonnumber >= 3)
        Strcpy(numberword, "Several");

    if (result > 0 && canseemonnumber > 0)
    {
        if (result == 1 && mtmp)
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s crawls out of nowhere!", Amonnam(mtmp));
        else
        {
            if (canseemonnumber == 1 && mtmp2)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s crawls out of nowhere!", Amonnam(mtmp2));
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s %s crawl out of nowhere!", numberword, thoul_cnt > 0 ? (ghast_cnt == 0 && ghoul_cnt == 0 ? "thouls" : "ghoul-like undead") : ghast_cnt == 0 ? "ghouls" : ghoul_cnt == 0 ? "ghasts" : "ghouls and ghasts");
        }
    }

    /* how many monsters exist now compared to before? */
    if (result)
        result = monster_census(FALSE) - census;

    return result;
}

/* Yacc summons bison */
int
yacc_bison_summon()
{
    int dtype = NON_PM, cnt = 0, result = 0, census;
    struct monst* mtmp = (struct monst*) 0;

    cnt = d(1, 3);

    /* some candidates can generate a group of monsters, so simple
       count of non-null makemon() result is not sufficient */
    census = monster_census(FALSE);

    int canseemonnumber = 0;
    struct monst* mtmp2 = (struct monst*) 0;

    context.makemon_spef_idx = 0;
    while (cnt > 0)
    {
        dtype = PM_BISON;

        if (mvitals[dtype].mvflags & MV_GONE)
        {
            cnt--;
            break;
        }

        mtmp = makemon(&mons[dtype], u.ux, u.uy, MM_ANGRY | MM_PLAY_SUMMON_ANIMATION | MM_SUMMON_MONSTER_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL));
        if (mtmp)
        {
            context.makemon_spef_idx++;
            result++;
            mtmp->mdemonsummon_used = 30;
            mtmp->mspecialsummon_used = 30;
            mtmp->mspecialsummon2_used = 30;
            if (canseemon(mtmp))
            {
                canseemonnumber++;
                mtmp2 = mtmp;
            }
        }
        cnt--;
    }
    makemon_animation_wait_until_end();

    char numberword[BUFSZ] = "Some";
    if (canseemonnumber >= 3)
        Strcpy(numberword, "Several");

    if (result > 0 && canseemonnumber > 0)
    {
        if (result == 1 && mtmp)
        {
            play_sfx_sound_at_location(SFX_SUMMON_DEMON, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears in a cloud of smoke!", Amonnam(mtmp));
        }
        else
        {
            if (canseemonnumber == 1 && mtmp2)
            {
                play_sfx_sound_at_location(SFX_SUMMON_DEMON, mtmp2->mx, mtmp2->my);
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears in a cloud of smoke!", Amonnam(mtmp2));
            }
            else
            {
                play_sfx_sound(SFX_SUMMON_DEMON);
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s bison appear in a cloud of smoke!", numberword);
            }
        }
    }

    /* how many monsters exist now compared to before? */
    if (result)
        result = monster_census(FALSE) - census;

    return result;
}

/* Orcus summons undead */
int
orcus_undead_summon()
{
    int dtype = NON_PM, cnt = 0, result = 0, census;
    struct monst* mtmp = (struct monst*) 0;

    cnt = d(2, 3);

    /* some candidates can generate a group of monsters, so simple
       count of non-null makemon() result is not sufficient */
    census = monster_census(FALSE);

    int canseemonnumber = 0;
    struct monst* mtmp2 = (struct monst*) 0;

    context.makemon_spef_idx = 0;
    while (cnt > 0) {
        int roll = rn2(29);
        dtype = NON_PM;

        switch (roll)
        {
        case 0:
        case 1:
            dtype = PM_GNOME_ZOMBIE;
            break;
        case 2:
            dtype = PM_ELF_ZOMBIE;
            break;
        case 3:
            dtype = PM_ORC_ZOMBIE;
            break;
        case 4:
            dtype = PM_HUMAN_ZOMBIE;
            break;
        case 5:
            dtype = PM_GNOME_MUMMY;
            break;
        case 6:
            dtype = PM_ELF_MUMMY;
            break;
        case 7:
            dtype = PM_ORC_MUMMY;
            break;
        case 8:
            dtype = PM_HUMAN_MUMMY;
            break;
        case 9:
            dtype = PM_GIANT_MUMMY;
            break;
        case 10:
            dtype = PM_ETTIN_MUMMY;
            break;
        case 11:
            dtype = PM_VAMPIRE;
            break;
        case 12:
            dtype = PM_VAMPIRE_MAGE;
            break;
        case 13:
            dtype = PM_VAMPIRE_LORD;
            break;
        case 14:
            dtype = PM_WRAITH;
            break;
        case 15:
            dtype = PM_LICH;
            break;
        case 16:
            dtype = PM_DEMILICH;
            break;
        case 17:
            dtype = PM_GIANT_ZOMBIE;
            break;
        case 18:
            dtype = PM_DWARF_ZOMBIE;
            break;
        case 19:
            dtype = PM_DWARF_MUMMY;
            break;
        case 20:
            dtype = PM_SPECTRE;
            break;
        case 21:
            dtype = PM_KING_WRAITH;
            break;
        case 22:
            dtype = PM_SKELETON_WARRIOR;
            break;
        case 23:
            dtype = PM_SKELETON_LORD;
            break;
        case 24:
            dtype = PM_SKELETON_KING;
            break;
        case 25:
            dtype = PM_GREATER_MUMMY;
            break;
        case 26:
            dtype = PM_GREATER_MUMMY_PRIEST;
            break;
        case 27:
            dtype = PM_GREATER_MUMMY_PHARAOH;
            break;
        case 28:
            dtype = PM_VAMPIRE_KING;
            break;
        default:
            break;
        }

        if (dtype == NON_PM || mvitals[dtype].mvflags & MV_GONE)
        {
            cnt--;
            continue;
        }

        mtmp = makemon(&mons[dtype], u.ux, u.uy, MM_ANGRY | MM_PLAY_SUMMON_ANIMATION | MM_UNDEAD_SUMMON_ANIMATION | (context.makemon_spef_idx == 0 ? MM_PLAY_SUMMON_SOUND : 0UL));
        if (mtmp) 
        {
            context.makemon_spef_idx++;
            result++;
            mtmp->mdemonsummon_used = 30;
            mtmp->mspecialsummon_used = 30;
            mtmp->mspecialsummon2_used = 30;
            if (canseemon(mtmp))
            {
                canseemonnumber++;
                mtmp2 = mtmp;
            }
        }

        cnt--;
    }
    makemon_animation_wait_until_end();

    char numberword[BUFSZ] = "Some";
    if (canseemonnumber >= 3)
        Strcpy(numberword, "Several");

    if (result > 0 && canseemonnumber > 0) {
        if (result == 1 && mtmp)
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears out of nowhere!", Amonnam(mtmp));
        else
        {
            if (canseemonnumber == 1 && mtmp2)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears out of nowhere!", Amonnam(mtmp2));
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s undead appear out of nowhere!", numberword);
        }
    }

    /* how many monsters exist now compared to before? */
    if (result)
        result = monster_census(FALSE) - census;

    return result;
}




void
summon_minion(alignment, talk)
aligntyp alignment;
boolean talk;
{
    register struct monst *mon;
    int mnum;

    switch ((int) alignment)
    {
    case A_LAWFUL:
        mnum = lminion(TRUE, TRUE);
        break;
    case A_NEUTRAL:
    {
        int minions[8] = { PM_AIR_ELEMENTAL, PM_EARTH_ELEMENTAL, PM_FIRE_ELEMENTAL, PM_WATER_ELEMENTAL, PM_ELDER_AIR_ELEMENTAL, PM_ELDER_EARTH_ELEMENTAL, PM_ELDER_FIRE_ELEMENTAL, PM_ELDER_WATER_ELEMENTAL };
        mnum = minions[rn2(8)];
        break;
    }
    case A_CHAOTIC:
    case A_NONE:
        mnum = ndemon(alignment, TRUE, TRUE);
        break;
    default:
        impossible("unaligned player?");
        mnum = ndemon(A_NONE, TRUE, TRUE);
        break;
    }

    if (mnum == NON_PM) 
    {
        mon = 0;
    }
    else if (mnum == PM_ANGEL) 
    {
        mon = makemon_limited(&mons[mnum], u.ux, u.uy, MM_EMIN | MM_ROAMER | MM_PLAY_SUMMON_ANIMATION | MM_LAWFUL_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END, MM2_FORCE_NONRENEGADE, 0, 0, 0, 0, 0, alignment);
        //if (mon)
        //{
        //    mon->isminion = 1;
        //    EMIN(mon)->min_align = alignment;
        //    EMIN(mon)->renegade = FALSE;
        //}
    }
    else if (mnum != PM_SHOPKEEPER && mnum != PM_SMITH && mnum != PM_GUARD
               && mnum != PM_ALIGNED_PRIEST && mnum != PM_HIGH_PRIEST) 
    {
        /* This was mons[mnum].pxlth == 0 but is this restriction
           appropriate or necessary now that the structures are separate? */
        mon = makemon_limited(&mons[mnum], u.ux, u.uy, MM_EMIN | MM_ROAMER | MM_PLAY_SUMMON_ANIMATION | MM_LAWFUL_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END, MM2_FORCE_NONRENEGADE, 0, 0, 0, 0, 0, alignment);
        //if (mon)
        //{
        //    mon->isminion = 1;
        //    EMIN(mon)->min_align = alignment;
        //    EMIN(mon)->renegade = FALSE;
        //}
    }
    else
    {
        mon = makemon(&mons[mnum], u.ux, u.uy, MM_PLAY_SUMMON_ANIMATION | MM_LAWFUL_SUMMON_ANIMATION | MM_PLAY_SUMMON_SOUND | MM_ANIMATION_WAIT_UNTIL_END);
    }

    if (mon) 
    {
        if (talk)
        {
            pline_The("voice of %s booms:", align_gname(alignment));
            play_voice_god_simple_line_by_align(alignment, GOD_LINE_THOU_SHALT_PAY_FOR_THINE_INDISCRETION);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou shalt pay for thine indiscretion!");
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s appears before you.", Amonnam(mon));
            mon->mstrategy &= ~STRAT_APPEARMSG;
        }
        mon->mpeaceful = FALSE;
        /* don't call set_mhostility(); player was naughty */
    }
}

#define Athome (Inhell && (mtmp->cham == NON_PM))

/* returns 1 if it won't attack. */
int
demon_talk(mtmp, stop_chat_ptr)
struct monst *mtmp;
boolean* stop_chat_ptr;
{
    long cash, demand, offer;

    if (stop_chat_ptr)
        *stop_chat_ptr = FALSE;

    if ((uwep && uwep->oartifact && artifact_has_flag(uwep, AF_ANGERS_DEMONS))
        || (uarms && uarms->oartifact && artifact_has_flag(uarms, AF_ANGERS_DEMONS))
        ) 
    {
        pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s looks very angry.", Amonnam(mtmp));
        mtmp->mpeaceful = mtmp->mtame = 0;
        if (!mtmp->mtame)
            mtmp->ispartymember = FALSE;

        set_mhostility(mtmp);
        newsym(mtmp->mx, mtmp->my);
        if (stop_chat_ptr)
            *stop_chat_ptr = TRUE;
        return 0;
    }

    if (is_fainted()) {
        reset_faint(); /* if fainted - wake up */
    } else {
        stop_occupation();
        if (multi > 0) {
            nomul(0);
            unmul((char *) 0);
        }
    }

    /* Slight advantage given. */
    if (is_dprince(mtmp->data) && is_invisible(mtmp))
    {
        boolean wasunseen = !canspotmon(mtmp);

        set_mon_property(mtmp, INVISIBILITY, 0);

        if (wasunseen && canspotmon(mtmp)) {
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s appears before you.", Amonnam(mtmp));
            mtmp->mstrategy &= ~STRAT_APPEARMSG;
        }
        newsym(mtmp->mx, mtmp->my);
    }
    if (youmonst.data->mlet == S_DEMON) 
    { /* Won't blackmail their own. */
        /* play sound is missing from here */
        pline("%s says, \"Good hunting, %s.\"", Amonnam(mtmp),
              flags.female ? "Sister" : "Brother");
        if (!tele_restrict(mtmp))
        {
            (void)rloc2(mtmp, TRUE, TRUE);
            if (stop_chat_ptr)
                *stop_chat_ptr = TRUE;
        }
        return 1;
    }
    else if (mtmp->data == &mons[PM_YEENAGHU] && maybe_polyd(is_gnoll(youmonst.data), Race_if(PM_GNOLL)))
    {
        if (canspotmon(mtmp))
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s, the Demon Lord of Gnolls, stands towering before you.", Amonnam(mtmp));
        else
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You hear the thundering voice of %s, the Demon Lord of Gnolls.", Amonnam(mtmp));

        pline("%s speaks.", Amonnam(mtmp));
        boolean has_howling_flail = FALSE;
        struct obj* flailobj;
        for(flailobj = invent; flailobj; flailobj = flailobj->nobj)
            if (flailobj->oartifact == ART_HOWLING_FLAIL)
            {
                has_howling_flail = TRUE;
                break;
            }

        if ((u.ualign.type == A_CHAOTIC && (u.ualign.record >= 14 || !(2-rnl(3)))) || (u.ualign.type == A_NEUTRAL && !rnl(3))
            || has_howling_flail)
        {
            struct monst tmpmon = *mtmp; /* Save Yeenaghu's data */
            play_monster_special_dialogue_line(mtmp, YEENAGHU_LINE_PLEASED);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "You have pleased me, my minion. I will grant one wish!");
            /* give a wish and discard the monster (mtmp set to null) */
            mongrantswish(&mtmp);
            //mongrantswish removes the monster (calls mongone)
            play_simple_monster_sound(&tmpmon, MONSTER_SOUND_TYPE_LAUGHTER);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "The demon lord laughs, then vanishes.");
            if (stop_chat_ptr)
                *stop_chat_ptr = TRUE;
            return 1;
        }
        if (u.ualign.type != A_LAWFUL)
        {
            play_monster_special_dialogue_line(mtmp, YEENAGHU_LINE_NOT_PLEASED);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "You have not pleased me! Prove your allegiance to the Abyss, and you shall be rewarded.");
            play_monster_special_dialogue_line(mtmp, YEENAGHU_LINE_SCOWLS);
            play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s scowls at you, then vanishes.", Amonnam(mtmp));
        }
        else
        {
            play_monster_special_dialogue_line(mtmp, YEENAGHU_LINE_DISPLEASE);
            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Your ways displease me. Follow the path of the Abyss, and you shall be rewarded.");
            play_monster_special_dialogue_line(mtmp, YEENAGHU_LINE_SCOWLS);
            play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s scowls at you menacingly, then vanishes.", Amonnam(mtmp));
        }
    }
    else if (mtmp->data == &mons[PM_DEMOGORGON] || mtmp->data == &mons[PM_YEENAGHU] || mtmp->data == &mons[PM_JUBILEX] || mtmp->data == &mons[PM_ORCUS])
    {
        //Demon lords and princes get angry for all but devout (and some random less devout) chaotic characters
        if ((u.ualign.type == A_CHAOTIC && (u.ualign.record >= 14 || !rnl(4))))
        {
            play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_LAUGHTER);
            play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mtmp->mx, mtmp->my);
            pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s laughs menacingly, then vanishes.", Amonnam(mtmp));
        }
        else 
        {
            play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_GET_ANGRY);
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "Seeing you, %s gets angry...", Amonnam(mtmp));
            mtmp->mpeaceful = 0;
            set_mhostility(mtmp);
            newsym(mtmp->mx, mtmp->my);
            update_game_music();
            if (stop_chat_ptr)
                *stop_chat_ptr = TRUE;
            return 0;
        }
    }
    else //An Arch-Devil demands payment
    {
        cash = money_cnt(invent);
        demand =
            (cash * (rnd(80) + 20 * Athome))
            / (100 * (1 + (sgn(u.ualign.type) == sgn(mtmp->data->maligntyp))));

        if (!demand || multi < 0 || Sleeping || Paralyzed_or_immobile) { /* you have no gold or can't move */
            mtmp->mpeaceful = 0;
            set_mhostility(mtmp);
            newsym(mtmp->mx, mtmp->my);
            if (stop_chat_ptr)
                *stop_chat_ptr = TRUE;
            return 0;
        }
        else 
        {
            /* make sure that the demand is unmeetable if the monster
               has the Amulet, preventing monster from being satisfied
               and removed from the game (along with said Amulet...) */
            if (mon_has_amulet(mtmp))
                demand = cash + (long) rn1(1000, 40);

            if (canspotmon(mtmp))
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s stands towering before you.", Amonnam(mtmp));
            else
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "You hear the thundering voice of %s.", Amonnam(mtmp));

            verbalize_ex(ATR_NONE, CLR_MSG_GOD, "For your life, we, %s, demand a gift of %ld %s.", Amonnam(mtmp), demand,
                  currency(demand));

            if ((offer = bribe(mtmp)) >= demand) 
            {
                play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mtmp->mx, mtmp->my);
                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_LAUGHTER);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s vanishes, laughing about cowardly mortals.", Amonnam(mtmp));
            }
            else if (offer > 0L
                       && (long) rnd(5 * ACURR(A_CHA)) > (demand - offer)) 
            {
                play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mtmp->mx, mtmp->my);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s scowls at you menacingly, then vanishes.", Amonnam(mtmp));
            }
            else 
            {
                play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_GET_ANGRY);
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s gets angry...", Amonnam(mtmp));
                mtmp->mpeaceful = 0;
                set_mhostility(mtmp);
                newsym(mtmp->mx, mtmp->my);
                update_game_music();
                if (stop_chat_ptr)
                    *stop_chat_ptr = TRUE;
                return 0;
            }
        }
    }
    mongone(mtmp);
    if (stop_chat_ptr)
        *stop_chat_ptr = TRUE;
    return 1;
}

long
bribe(mtmp)
struct monst *mtmp;
{
    char buf[BUFSZ] = DUMMY;
    long offer;
    long umoney = money_cnt(invent);

    getlin_ex(GETLINE_NUMBERS_ONLY, ATR_NONE, NO_COLOR, "How much will you offer?", buf, (char*)0, (char*)0, (char*)0);
    if (sscanf(buf, "%ld", &offer) != 1)
        offer = 0L;

    /*Michael Paddon -- fix for negative offer to monster*/
    /*JAR880815 - */
    if (offer < 0L) {
        You("try to shortchange %s, but fumble.", mon_nam(mtmp));
        return 0L;
    } else if (offer == 0L) {
        You("refuse.");
        return 0L;
    } else if (offer >= umoney) {
        You("give %s all your gold.", mon_nam(mtmp));
        offer = umoney;
    } else {
        You("give %s %ld %s.", mon_nam(mtmp), offer, currency(offer));
    }
    (void) money2mon(mtmp, offer);
    context.botl = 1;
    return offer;
}

int
dprince(atyp, ignore_difficulty, is_summon)
aligntyp atyp;
boolean ignore_difficulty, is_summon;
{
    int tryct, pm;

    for (tryct = !In_endgame(&u.uz) ? 20 : 0; tryct > 0; --tryct) {
        pm = rn1(PM_DEMOGORGON + 1 - PM_BAALZEBUB, PM_BAALZEBUB);
        if (!(mvitals[pm].mvflags & MV_GONE)
            && (atyp == A_NONE || sgn(mons[pm].maligntyp) == sgn(atyp)))
            return pm;
    }
    return dlord(atyp, ignore_difficulty, is_summon); /* approximate */
}

int
dlord(atyp, ignore_difficulty, is_summon)
aligntyp atyp;
boolean ignore_difficulty, is_summon;
{
    int tryct, pm;

    for (tryct = !In_endgame(&u.uz) ? 20 : 0; tryct > 0; --tryct) {
        pm = rn1(PM_BAPHOMET + 1 - PM_JUBILEX, PM_JUBILEX);
        if (!(mvitals[pm].mvflags & MV_GONE)
            && (atyp == A_NONE || sgn(mons[pm].maligntyp) == sgn(atyp)))
            return pm;
    }
    return ndemon(atyp, ignore_difficulty, is_summon); /* approximate */
}

/* create lawful (good) lord */
int
llord(ignore_difficulty, is_summon)
boolean ignore_difficulty, is_summon;
{
    if (!(mvitals[PM_ARCHON].mvflags & MV_GONE))
        return PM_ARCHON;

    return lminion(ignore_difficulty, is_summon); /* approximate */
}

int
lminion(ignore_difficulty, is_summon)
boolean ignore_difficulty, is_summon;
{
    int tryct;
    struct permonst *ptr;

    for (tryct = 0; tryct < 20; tryct++) {
        ptr = mkclass_core(S_ANGEL, 0, A_NONE, 0, (ignore_difficulty ? (MKCLASS_FLAGS_IGNORE_DIFFICULTY) : 0UL) | (is_summon ? (MKCLASS_FLAGS_SUMMON) : 0UL));
        if (ptr && !is_lord(ptr))
            return monsndx(ptr);
    }

    return NON_PM;
}

int
ndemon(atyp, ignore_difficulty, is_summon)
aligntyp atyp; /* A_NONE is used for 'any alignment' */
boolean ignore_difficulty, is_summon;
{
    struct permonst *ptr;

    /*
     * 3.6.2:  [fix #H2204, 22-Dec-2010, eight years later...]
     * pick a correctly aligned demon in one try.  This used to
     * use mkclass() to choose a random demon type and keep trying
     * (up to 20 times) until it got one with the desired alignment.
     * mkclass_aligned() skips wrongly aligned potential candidates.
     * [The only neutral demons are djinni and mail daemon and
     * mkclass() won't pick them, but call it anyway in case either
     * aspect of that changes someday.]
     */
#if 0
    if (atyp == A_NEUTRAL)
        return NON_PM;
#endif
    ptr = mkclass_aligned(S_DEMON, 0, atyp, (ignore_difficulty ? (MKCLASS_FLAGS_IGNORE_DIFFICULTY) : 0UL) | (is_summon ? (MKCLASS_FLAGS_SUMMON) : 0UL));
    if (ptr && is_ndemon(ptr))
        return monsndx(ptr);
    else
        return NON_PM;
}

/* guardian angel has been affected by conflict so is abandoning hero */
void
lose_guardian_angel(mon)
struct monst *mon; /* if null, angel hasn't been created yet */
{
    coord mm;
    int i;

    if (mon) {
        if (canspotmon(mon)) 
        {
            if (!Deaf) 
            {
                pline("%s rebukes you, saying:", Monnam(mon));
                verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Since you desire conflict, have some more!");
            } 
            else 
            {
                play_sfx_sound_at_location(SFX_VANISHES_IN_PUFF_OF_SMOKE, mon->mx, mon->my);
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s vanishes!", Monnam(mon));
            }
        }
        mongone(mon);
    }
    /* create 2 to 4 hostile angels to replace the lost guardian */
    for (i = rn1(3, 2); i > 0; --i) {
        mm.x = u.ux;
        mm.y = u.uy;
        if (enexto(&mm, mm.x, mm.y, &mons[PM_ANGEL]))
            (void) mk_roamer(&mons[PM_ANGEL], u.ualign.type, mm.x, mm.y,
                             FALSE);
    }
}

/* just entered the Astral Plane; receive tame guardian angel if worthy */
void
gain_guardian_angel(fromspell)
boolean fromspell;
{
    struct monst *mtmp;
    struct obj *otmp;
    coord mm;

    Hear_again(); /* attempt to cure any deafness now (divine
                     message will be heard even if that fails) */
    if (!fromspell && Conflict) 
    {
        play_voice_god_simple_line_by_align(u.ualign.type, GOD_LINE_THY_DESIRE_FOR_CONFLICT_SHALL_BE_FULFILLED);
        pline("A voice booms:");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thy desire for conflict shall be fulfilled!");
        /* send in some hostile angels instead */
        lose_guardian_angel((struct monst *) 0);
    }
    else if (fromspell || u.ualign.record > 8) 
    { /* fervent */
        play_voice_god_simple_line_by_align(u.ualign.type, GOD_LINE_THOU_HAST_BEEN_WORTHY_OF_ME);
        pline("A voice whispers:");
        verbalize_ex(ATR_NONE, CLR_MSG_GOD, "Thou hast been worthy of me!");
        mm.x = u.ux;
        mm.y = u.uy;
        if (enexto(&mm, mm.x, mm.y, &mons[PM_ANGEL])
            && (mtmp = mk_roamer(&mons[PM_ANGEL], u.ualign.type, mm.x, mm.y,
                                 TRUE)) != 0) {
            mtmp->mstrategy &= ~STRAT_APPEARMSG;
            if (!Blind)
                pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "An angel appears near you.");
            else
                You_feel_ex(ATR_NONE, CLR_MSG_ATTENTION, "the presence of a friendly angel near you.");
            /* guardian angel -- the one case mtame doesn't
             * imply an edog structure, so we don't want to
             * call tamedog().
             */
            mtmp->mtame = 10;
            mtmp->isprotector = TRUE;
            newsym(mtmp->mx, mtmp->my);
            /* make him strong enough vs. endgame foes */
            mtmp->m_lev = rn1(8, 15);
            mtmp->mbasehpmax =
                d((int) mtmp->m_lev, 10) + 30 + rnd(30);
            mtmp->mbaseendrain = 0;
            update_mon_maxhp(mtmp);
            mtmp->mhp = mtmp->mhpmax;
            if ((otmp = select_hwep(mtmp, TRUE, 0, 0)) == 0) {
                otmp = mksobj(SILVER_SABER, FALSE, FALSE, FALSE);
                if (mpickobj(mtmp, otmp))
                {
                    panic("merged weapon?");
                    return;
                }
            }
            bless(otmp);
            if (otmp->enchantment < 4)
                otmp->enchantment += rnd(4);
            if ((otmp = which_armor(mtmp, W_ARMS)) == 0
                || otmp->otyp != SHIELD_OF_REFLECTION) {
                (void) mongets(mtmp, AMULET_OF_REFLECTION);
                m_dowear(mtmp, TRUE);
            }
        }
    }
}

/*minion.c*/
