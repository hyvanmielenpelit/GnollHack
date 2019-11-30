/* GnollHack 4.0	sounds.c	$NHDT-Date: 1542765362 2018/11/21 01:56:02 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.81 $ */
/*      Copyright (c) 1989 Janet Walz, Mike Threepoint */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"

STATIC_DCL boolean FDECL(mon_is_gecko, (struct monst *));
STATIC_DCL int FDECL(domonnoise, (struct monst *));
STATIC_DCL int NDECL(dochat);
STATIC_DCL int FDECL(do_chat_whoareyou, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_sit, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_givepaw, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_stay, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_follow, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dropitems, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_pickitems, (struct monst*));
STATIC_DCL int FDECL(do_chat_buy_items, (struct monst*));
STATIC_DCL int FDECL(count_sellable_items, (struct monst*));
STATIC_DCL int FDECL(do_chat_oracle_consult, (struct monst*));
STATIC_DCL int FDECL(do_chat_oracle_identify, (struct monst*));
STATIC_DCL int FDECL(do_chat_oracle_enlightenment, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_blesscurse, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_healing, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_chat, (struct monst*));
STATIC_DCL int FDECL(do_chat_shk_pricequote, (struct monst*));
STATIC_DCL int FDECL(do_chat_shk_chat, (struct monst*));
STATIC_DCL int FDECL(do_chat_quest_chat, (struct monst*));
STATIC_DCL int FDECL(mon_in_room, (struct monst *, int));

/* this easily could be a macro, but it might overtax dumb compilers */
STATIC_OVL int
mon_in_room(mon, rmtyp)
struct monst *mon;
int rmtyp;
{
    int rno = levl[mon->mx][mon->my].roomno;
    if (rno >= ROOMOFFSET)
        return rooms[rno - ROOMOFFSET].rtype == rmtyp;
    return FALSE;
}

void
dosounds()
{
    register struct mkroom *sroom;
    register int hallu, vx, vy;
#if defined(AMIGA) && defined(AZTEC_C_WORKAROUND)
    int xx;
#endif
    struct monst *mtmp;

    if (Deaf || !flags.acoustics || u.uswallow || Underwater)
        return;

    hallu = Hallucination ? 1 : 0;

    if (level.flags.nfountains && !rn2(400)) {
        static const char *const fountain_msg[4] = {
            "bubbling water.", "water falling on coins.",
            "the splashing of a naiad.", "a soda fountain!",
        };
        You_hear1(fountain_msg[rn2(3) + hallu]);
    }
    if (level.flags.nsinks && !rn2(300)) {
        static const char *const sink_msg[3] = {
            "a slow drip.", "a gurgling noise.", "dishes being washed!",
        };
        You_hear1(sink_msg[rn2(2) + hallu]);
    }
    if (level.flags.has_court && !rn2(200)) {
        static const char *const throne_msg[4] = {
            "the tones of courtly conversation.",
            "a sceptre pounded in judgment.",
            "Someone shouts \"Off with %s head!\"", "Queen Beruthiel's cats!",
        };
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if ((mtmp->msleeping || is_lord(mtmp->data)
                 || is_prince(mtmp->data)) && !is_animal(mtmp->data)
                && mon_in_room(mtmp, COURT)) {
                /* finding one is enough, at least for now */
                int which = rn2(3) + hallu;

                if (which != 2)
                    You_hear1(throne_msg[which]);
                else
                    pline(throne_msg[2], uhis());
                return;
            }
        }
    }
    if (level.flags.has_swamp && !rn2(200)) {
        static const char *const swamp_msg[3] = {
            "hear mosquitoes!", "smell marsh gas!", /* so it's a smell...*/
            "hear Donald Duck!",
        };
        You1(swamp_msg[rn2(2) + hallu]);
        return;
    }
    if (level.flags.has_vault && !rn2(200)) {
        if (!(sroom = search_special(VAULT))) {
            /* strange ... */
            level.flags.has_vault = 0;
            return;
        }
        if (gd_sound())
            switch (rn2(2) + hallu) {
            case 1: {
                boolean gold_in_vault = FALSE;

                for (vx = sroom->lx; vx <= sroom->hx; vx++)
                    for (vy = sroom->ly; vy <= sroom->hy; vy++)
                        if (g_at(vx, vy))
                            gold_in_vault = TRUE;
#if defined(AMIGA) && defined(AZTEC_C_WORKAROUND)
                /* Bug in aztec assembler here. Workaround below */
                xx = ROOM_INDEX(sroom) + ROOMOFFSET;
                xx = (xx != vault_occupied(u.urooms));
                if (xx)
#else
                if (vault_occupied(u.urooms)
                    != (ROOM_INDEX(sroom) + ROOMOFFSET))
#endif /* AZTEC_C_WORKAROUND */
                {
                    if (gold_in_vault)
                        You_hear(!hallu
                                     ? "someone counting money."
                                     : "the quarterback calling the play.");
                    else
                        You_hear("someone searching.");
                    break;
                }
            }
                /*FALLTHRU*/
            case 0:
                You_hear("the footsteps of a guard on patrol.");
                break;
            case 2:
                You_hear("Ebenezer Scrooge!");
                break;
            }
        return;
    }
    if (level.flags.has_beehive && !rn2(200)) {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if ((mtmp->data->mlet == S_ANT && is_flyer(mtmp->data))
                && mon_in_room(mtmp, BEEHIVE)) {
                switch (rn2(2) + hallu) {
                case 0:
                    You_hear("a low buzzing.");
                    break;
                case 1:
                    You_hear("an angry drone.");
                    break;
                case 2:
                    You_hear("bees in your %sbonnet!",
                             uarmh ? "" : "(nonexistent) ");
                    break;
                }
                return;
            }
        }
    }

	if (level.flags.has_library && !rn2(200)) {
		for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
			if (DEADMONSTER(mtmp))
				continue;
			if ((mtmp->data->mlet == S_LICH || mtmp->data == &mons[PM_IMP] || mtmp->data == &mons[PM_QUASIT]  
				|| mtmp->data == &mons[PM_GNOLL_WARDEN] || mtmp->data == &mons[PM_GNOMISH_WIZARD])
				&& mon_in_room(mtmp, LIBRARY)) {
				switch (rn2(2) + hallu) {
				case 0:
					You("suddenly realize it is quiter than usual.");
					break;
				case 1:
					You_hear("somebody demanding quietness.");
					break;
				case 2:
					You_hear("pages turning in your head.");
					break;
				}
				return;
			}
		}
	}

    if (level.flags.has_morgue && !rn2(200)) {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if ((is_undead(mtmp->data) || is_vampshifter(mtmp))
                && mon_in_room(mtmp, MORGUE)) {
                const char *hair = body_part(HAIR); /* hair/fur/scales */

                switch (rn2(2) + hallu) {
                case 0:
                    You("suddenly realize it is unnaturally quiet.");
                    break;
                case 1:
                    pline_The("%s on the back of your %s %s up.", hair,
                              body_part(NECK), vtense(hair, "stand"));
                    break;
                case 2:
                    pline_The("%s on your %s %s to stand up.", hair,
                              body_part(HEAD), vtense(hair, "seem"));
                    break;
                }
                return;
            }
        }
    }
    if (level.flags.has_barracks && !rn2(200)) {
        static const char *const barracks_msg[4] = {
            "blades being honed.", "loud snoring.", "dice being thrown.",
            "General MacArthur!",
        };
        int count = 0;

        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if (is_mercenary(mtmp->data)
#if 0 /* don't bother excluding these */
                && !strstri(mtmp->data->mname, "watch")
                && !strstri(mtmp->data->mname, "guard")
#endif
                && mon_in_room(mtmp, BARRACKS)
                /* sleeping implies not-yet-disturbed (usually) */
                && (mtmp->msleeping || ++count > 5)) {
                You_hear1(barracks_msg[rn2(3) + hallu]);
                return;
            }
        }
    }
    if (level.flags.has_zoo && !rn2(200)) {
        static const char *const zoo_msg[3] = {
            "a sound reminiscent of an elephant stepping on a peanut.",
            "a sound reminiscent of a seal barking.", "Doctor Dolittle!",
        };
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if ((mtmp->msleeping || is_animal(mtmp->data))
                && mon_in_room(mtmp, ZOO)) {
                You_hear1(zoo_msg[rn2(2) + hallu]);
                return;
            }
        }
    }
    if (level.flags.has_shop && !rn2(200)) {
        if (!(sroom = search_special(ANY_SHOP))) {
            /* strange... */
            level.flags.has_shop = 0;
            return;
        }
        if (tended_shop(sroom)
            && !index(u.ushops, (int) (ROOM_INDEX(sroom) + ROOMOFFSET))) {
            static const char *const shop_msg[3] = {
                "someone cursing shoplifters.",
                "the chime of a cash register.", "Neiman and Marcus arguing!",
            };
            You_hear1(shop_msg[rn2(2) + hallu]);
        }
        return;
    }

	if (level.flags.has_desertedshop && !rn2(200))
	{
		for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
			if (DEADMONSTER(mtmp))
				continue;
			if (mon_in_room(mtmp, DESERTEDSHOP))
			{
				if (hallu)
					pline("For a moment, you thought you heard Neiman and Marcus arguing!");
				else
				{
					if (!rn2(2))
						pline("For a moment, you thought you heard someone cursing.");
					else
						You("hear a faint chime but then it fades.");
				}
				return;
			}
		}
	}

    if (level.flags.has_temple && !rn2(200)
        && !(Is_astralevel(&u.uz) || Is_sanctum(&u.uz))) {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if (mtmp->ispriest && inhistemple(mtmp)
                /* priest must be active */
                && mtmp->mcanmove && !mtmp->msleeping
                /* hero must be outside this temple */
                && temple_occupied(u.urooms) != EPRI(mtmp)->shroom)
                break;
        }
        if (mtmp) {
            /* Generic temple messages; no attempt to match topic or tone
               to the pantheon involved, let alone to the specific deity.
               These are assumed to be coming from the attending priest;
               asterisk means that the priest must be capable of speech;
               pound sign (octathorpe,&c--don't go there) means that the
               priest and the altar must not be directly visible (we don't
               care if telepathy or extended detection reveals that the
               priest is not currently standing on the altar; he's mobile). */
            static const char *const temple_msg[] = {
                "*someone praising %s.", "*someone beseeching %s.",
                "#an animal carcass being offered in sacrifice.",
                "*a strident plea for donations.",
            };
            const char *msg;
            int trycount = 0, ax = EPRI(mtmp)->shrpos.x,
                ay = EPRI(mtmp)->shrpos.y;
            boolean speechless = (mtmp->data->msound <= MS_ANIMAL),
                    in_sight = canseemon(mtmp) || cansee(ax, ay);

            do {
                msg = temple_msg[rn2(SIZE(temple_msg) - 1 + hallu)];
                if (index(msg, '*') && speechless)
                    continue;
                if (index(msg, '#') && in_sight)
                    continue;
                break; /* msg is acceptable */
            } while (++trycount < 50);
            while (!letter(*msg))
                ++msg; /* skip control flags */
            if (index(msg, '%'))
                You_hear(msg, halu_gname(EPRI(mtmp)->shralign));
            else
                You_hear1(msg);
            return;
        }
    }
    if (Is_oracle_level(&u.uz) && !rn2(400)) {
        /* make sure the Oracle is still here */
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if (mtmp->data == &mons[PM_ORACLE])
                break;
        }
        /* and don't produce silly effects when she's clearly visible */
        if (mtmp && (hallu || !canseemon(mtmp))) {
            static const char *const ora_msg[5] = {
                "a strange wind.",     /* Jupiter at Dodona */
                "convulsive ravings.", /* Apollo at Delphi */
                "snoring snakes.",     /* AEsculapius at Epidaurus */
                "someone say \"No more woodchucks!\"",
                "a loud ZOT!" /* both rec.humor.oracle */
            };
            You_hear1(ora_msg[rn2(3) + hallu * 2]);
        }
        return;
    }
}

static const char *const h_sounds[] = {
    "beep",   "boing",   "sing",   "belche", "creak",   "cough",
    "rattle", "ululate", "pop",    "jingle", "sniffle", "tinkle",
    "eep",    "clatter", "hum",    "sizzle", "twitter", "wheeze",
    "rustle", "honk",    "lisp",   "yodel",  "coo",     "burp",
    "moo",    "boom",    "murmur", "oink",   "quack",   "rumble",
    "twang",  "bellow",  "toot",   "gargle", "hoot",    "warble"
};

const char *
growl_sound(mtmp)
register struct monst *mtmp;
{
    const char *ret;

    switch (mtmp->data->msound) {
    case MS_MEW:
    case MS_HISS:
        ret = "hiss";
        break;
    case MS_BARK:
    case MS_GROWL:
        ret = "growl";
        break;
    case MS_ROAR:
        ret = "roar";
        break;
    case MS_BUZZ:
        ret = "buzz";
        break;
    case MS_SQEEK:
        ret = "squeal";
        break;
    case MS_SQAWK:
        ret = "screech";
        break;
    case MS_NEIGH:
        ret = "neigh";
        break;
    case MS_WAIL:
        ret = "wail";
        break;
    case MS_SILENT:
        ret = "commotion";
        break;
    default:
        ret = "scream";
    }
    return ret;
}

/* the sounds of a seriously abused pet, including player attacking it */
void
growl(mtmp)
register struct monst *mtmp;
{
    register const char *growl_verb = 0;

    if (mtmp->msleeping || !mtmp->mcanmove || !mtmp->data->msound)
        return;

    /* presumably nearness and soundok checks have already been made */
    if (Hallucination)
        growl_verb = h_sounds[rn2(SIZE(h_sounds))];
    else
        growl_verb = growl_sound(mtmp);
    if (growl_verb) {
        pline("%s %s!", Monnam(mtmp), vtense((char *) 0, growl_verb));
        if (context.run)
            nomul(0);
        wake_nearto(mtmp->mx, mtmp->my, mtmp->data->mlevel * 18);
    }
}

/* the sounds of mistreated pets */
void
yelp(mtmp)
register struct monst *mtmp;
{
    register const char *yelp_verb = 0;

    if (mtmp->msleeping || !mtmp->mcanmove || !mtmp->data->msound)
        return;

    /* presumably nearness and soundok checks have already been made */
    if (Hallucination)
        yelp_verb = h_sounds[rn2(SIZE(h_sounds))];
    else
        switch (mtmp->data->msound) {
        case MS_MEW:
            yelp_verb = (!Deaf) ? "yowl" : "arch";
            break;
        case MS_BARK:
        case MS_GROWL:
            yelp_verb = (!Deaf) ? "yelp" : "recoil";
            break;
        case MS_ROAR:
            yelp_verb = (!Deaf) ? "snarl" : "bluff";
            break;
        case MS_SQEEK:
            yelp_verb = (!Deaf) ? "squeal" : "quiver";
            break;
        case MS_SQAWK:
            yelp_verb = (!Deaf) ? "screak" : "thrash";
            break;
        case MS_WAIL:
            yelp_verb = (!Deaf) ? "wail" : "cringe";
            break;
        }
    if (yelp_verb) {
        pline("%s %s!", Monnam(mtmp), vtense((char *) 0, yelp_verb));
        if (context.run)
            nomul(0);
        wake_nearto(mtmp->mx, mtmp->my, mtmp->data->mlevel * 12);
		/* break charm*/
		if(mtmp->mcharmed)
		{
			mtmp->mcharmed = 0;
			mtmp->mcharmed_timer = 0;
			mtmp->mpeaceful = mtmp->morigpeaceful;
			mtmp->mtame = mtmp->morigtame;
		}

    }
}

/* the sounds of distressed pets */
void
whimper(mtmp)
register struct monst *mtmp;
{
    register const char *whimper_verb = 0;

    if (mtmp->msleeping || !mtmp->mcanmove || !mtmp->data->msound)
        return;

    /* presumably nearness and soundok checks have already been made */
    if (Hallucination)
        whimper_verb = h_sounds[rn2(SIZE(h_sounds))];
    else
        switch (mtmp->data->msound) {
        case MS_MEW:
        case MS_GROWL:
            whimper_verb = "whimper";
            break;
        case MS_BARK:
            whimper_verb = "whine";
            break;
        case MS_SQEEK:
            whimper_verb = "squeal";
            break;
        }
    if (whimper_verb) {
        pline("%s %s.", Monnam(mtmp), vtense((char *) 0, whimper_verb));
        if (context.run)
            nomul(0);
        wake_nearto(mtmp->mx, mtmp->my, mtmp->data->mlevel * 6);
    }
}

/* pet makes "I'm hungry" noises */
void
beg(mtmp)
register struct monst *mtmp;
{
    if (mtmp->msleeping || !mtmp->mcanmove
        || !(carnivorous(mtmp->data) || herbivorous(mtmp->data)))
        return;

    /* presumably nearness and soundok checks have already been made */
    if (!is_silent(mtmp->data) && mtmp->data->msound <= MS_ANIMAL)
        (void) domonnoise(mtmp);
    else if (mtmp->data->msound >= MS_HUMANOID) {
        if (!canspotmon(mtmp))
            map_invisible(mtmp->mx, mtmp->my);
        verbalize("I'm hungry.");
    }
}

/* return True if mon is a gecko or seems to look like one (hallucination) */
STATIC_OVL boolean
mon_is_gecko(mon)
struct monst *mon;
{
    int glyph;

    /* return True if it is actually a gecko */
    if (mon->data == &mons[PM_GECKO])
        return TRUE;
    /* return False if it is a long worm; we might be chatting to its tail
       (not strictly needed; long worms are MS_SILENT so won't get here) */
    if (mon->data == &mons[PM_LONG_WORM])
        return FALSE;
    /* result depends upon whether map spot shows a gecko, which will
       be due to hallucination or to mimickery since mon isn't one */
    glyph = glyph_at(mon->mx, mon->my);
    return (boolean) (glyph_to_mon(glyph) == PM_GECKO);
}

STATIC_OVL int
domonnoise(mtmp)
register struct monst *mtmp;
{
    char verbuf[BUFSZ];
    register const char *pline_msg = 0, /* Monnam(mtmp) will be prepended */
        *verbl_msg = 0,                 /* verbalize() */
        *verbl_msg_mcan = 0;            /* verbalize() if cancelled */
    struct permonst *ptr = mtmp->data;
    int msound = ptr->msound;

    /* presumably nearness and sleep checks have already been made */
	if (Deaf)
	{
		You("cannot hear anything.");
		return 0;
	}
    /* leader might be poly'd; if he can still speak, give leader speech */
    if (mtmp->m_id == quest_status.leader_m_id && msound > MS_ANIMAL)
        msound = MS_LEADER;
    /* make sure it's your role's quest guardian; adjust if not */
    else if (msound == MS_GUARDIAN && ptr != &mons[urole.guardnum])
        msound = mons[genus(monsndx(ptr), 1)].msound;
    /* some normally non-speaking types can/will speak if hero is similar */
    else if (msound == MS_ORC         /* note: MS_ORC is same as MS_GRUNT */
             && (same_race(ptr, youmonst.data)           /* current form, */
                 || same_race(ptr, &mons[Race_switch]))) /* unpoly'd form */
        msound = MS_HUMANOID;
    /* silliness, with slight chance to interfere with shopping */
    else if (Hallucination && mon_is_gecko(mtmp))
        msound = MS_SELL;

    /* be sure to do this before talking; the monster might teleport away, in
     * which case we want to check its pre-teleport position
     */
    if (!canspotmon(mtmp))
        map_invisible(mtmp->mx, mtmp->my);

    switch (msound) {
	case MS_SILENT:
		pline_msg = "does not respond.";
		break;
	case MS_ORACLE:
		if (mtmp->mpeaceful)
			Sprintf(verbuf, "Welcome to Delphi, adventurer!");
		else
			Sprintf(verbuf, "Begone, you fool!");
		verbl_msg = verbuf;
		break;
		//return doconsult(mtmp);
    case MS_PRIEST:
		if(mtmp->mpeaceful)
			Sprintf(verbuf, "Welcome to the temple of %s, adventurer!", (mtmp->ispriest && mtmp->mextra && mtmp->mextra->epri) ? align_gname(mtmp->mextra->epri->shralign) : "our almighty god");
		else
			Sprintf(verbuf, "You shall perish by the divine hand of %s!", (mtmp->ispriest && mtmp->mextra && mtmp->mextra->epri) ? align_gname(mtmp->mextra->epri->shralign) : "our almighty god");
		verbl_msg = verbuf;
		break;
		//priest_talk(mtmp);
    case MS_LEADER:
    case MS_NEMESIS:
    case MS_GUARDIAN:
        quest_chat(mtmp);
        break;
    case MS_SELL: /* pitch, pay, total */
		Sprintf(verbuf, "Welcome, adventurer!");
		if (mtmp->isshk)
		{
			register struct eshk* eshkp = (struct eshk*)0;
			if (mtmp->mextra && mtmp->mextra->eshk)
				eshkp = mtmp->mextra->eshk;

			if (eshkp)
			{
				char shopbuf[BUFSZ] = "";
				Sprintf(shopbuf, "my %s", shoptypename(eshkp->shoptype));
				if (mtmp->mpeaceful)
					Sprintf(verbuf, "Welcome to %s, adventurer!", shopbuf);
				else
					Sprintf(verbuf, "You rotten thief!");
			}
		}
		else
		{
			if (mtmp->mpeaceful)
				Sprintf(verbuf, "Welcome, adventurer!");
			else
				Sprintf(verbuf, "You rotten thief!");
		}
		verbl_msg = verbuf;

#if 0
        if (!Hallucination || (mtmp->isshk && !rn2(2))) {
            shk_chat(mtmp);
        } else {
            /* approximation of GEICO's advertising slogan (it actually
               concludes with "save you 15% or more on car insurance.") */
            Sprintf(verbuf, "15 minutes could save you 15 %s.",
                    currency(15L)); /* "zorkmids" */
            verbl_msg = verbuf;
        }
#endif
        break;
    case MS_VAMPIRE: {
        /* vampire messages are varied by tameness, peacefulness, and time of
         * night */
        boolean isnight = night();
        boolean kindred = (Upolyd && (u.umonnum == PM_VAMPIRE || u.umonnum == PM_VAMPIRE_MAGE
                                      || u.umonnum == PM_VAMPIRE_LORD));
        boolean nightchild =
            (Upolyd && (u.umonnum == PM_WOLF || u.umonnum == PM_WINTER_WOLF
                        || u.umonnum == PM_WINTER_WOLF_CUB));
        const char *racenoun =
            (flags.female && urace.individual.f)
                ? urace.individual.f
                : (urace.individual.m) ? urace.individual.m : urace.noun;

        if (mtmp->mtame) {
            if (kindred) {
                Sprintf(verbuf, "Good %s to you Master%s",
                        isnight ? "evening" : "day",
                        isnight ? "!" : ".  Why do we not rest?");
                verbl_msg = verbuf;
            } else {
                Sprintf(verbuf, "%s%s",
                        nightchild ? "Child of the night, " : "",
                        midnight()
                         ? "I can stand this craving no longer!"
                         : isnight
                          ? "I beg you, help me satisfy this growing craving!"
                          : "I find myself growing a little weary.");
                verbl_msg = verbuf;
            }
        } else if (mtmp->mpeaceful) {
            if (kindred && isnight) {
                Sprintf(verbuf, "Good feeding %s!",
                        flags.female ? "sister" : "brother");
                verbl_msg = verbuf;
            } else if (nightchild && isnight) {
                Sprintf(verbuf, "How nice to hear you, child of the night!");
                verbl_msg = verbuf;
            } else
                verbl_msg = "I only drink... potions.";
        } else {
            int vampindex;
            static const char *const vampmsg[] = {
                /* These first two (0 and 1) are specially handled below */
                "I vant to suck your %s!",
                "I vill come after %s without regret!",
                /* other famous vampire quotes can follow here if desired */
            };
            if (kindred)
                verbl_msg =
                    "This is my hunting ground that you dare to prowl!";
            else if (youmonst.data == &mons[PM_SILVER_DRAGON] || youmonst.data == &mons[PM_ANCIENT_SILVER_DRAGON]
                     || youmonst.data == &mons[PM_SILVER_DRAGON_HATCHLING]) {
                /* Silver dragons are silver in color, not made of silver */
                Sprintf(verbuf, "%s!  Your silver sheen does not frighten me!",
                        youmonst.data == &mons[PM_SILVER_DRAGON] || youmonst.data == &mons[PM_ANCIENT_SILVER_DRAGON]
                            ? "Fool"
                            : "Young Fool");
                verbl_msg = verbuf;
            } else {
                vampindex = rn2(SIZE(vampmsg));
                if (vampindex == 0) {
                    Sprintf(verbuf, vampmsg[vampindex], body_part(BLOOD));
                    verbl_msg = verbuf;
                } else if (vampindex == 1) {
                    Sprintf(verbuf, vampmsg[vampindex],
                            Upolyd ? an(mons[u.umonnum].mname)
                                   : an(racenoun));
                    verbl_msg = verbuf;
                } else
                    verbl_msg = vampmsg[vampindex];
            }
        }
    } break;
    case MS_WERE:
        if (flags.moonphase == FULL_MOON && (night() ^ !rn2(13))) {
            pline("%s throws back %s head and lets out a blood curdling %s!",
                  Monnam(mtmp), mhis(mtmp),
                  ptr == &mons[PM_HUMAN_WERERAT] ? "shriek" : "howl");
            wake_nearto(mtmp->mx, mtmp->my, 11 * 11);
        } else
            pline_msg =
                "whispers inaudibly.  All you can make out is \"moon\".";
        break;
    case MS_BARK:
        if (flags.moonphase == FULL_MOON && night()) {
            pline_msg = "howls.";
        } else if (mtmp->mpeaceful) {
            if (mtmp->mtame
                && (mtmp->mconf || mtmp->mflee || mtmp->mtrapped
                    || moves > EDOG(mtmp)->hungrytime || mtmp->mtame < 5))
                pline_msg = "whines.";
            else if (mtmp->mtame && EDOG(mtmp)->hungrytime > moves + 1000)
                pline_msg = "yips.";
            else {
                    pline_msg = "barks.";
            }
        } else {
            pline_msg = "growls.";
        }
        break;
    case MS_MEW:
        if (mtmp->mtame) {
            if (mtmp->mconf || mtmp->mflee || mtmp->mtrapped
                || mtmp->mtame < 5)
                pline_msg = "yowls.";
            else if (moves > EDOG(mtmp)->hungrytime)
                pline_msg = "meows.";
            else if (EDOG(mtmp)->hungrytime > moves + 1000)
                pline_msg = "purrs.";
            else
                pline_msg = "mews.";
            break;
        }
        /*FALLTHRU*/
    case MS_GROWL:
        pline_msg = mtmp->mpeaceful ? "snarls." : "growls!";
        break;
    case MS_ROAR:
        pline_msg = mtmp->mpeaceful ? "snarls." : "roars!";
        break;
    case MS_SQEEK:
        pline_msg = "squeaks.";
        break;
    case MS_SQAWK:
        if (ptr == &mons[PM_RAVEN] && !mtmp->mpeaceful)
            verbl_msg = "Nevermore!";
        else
            pline_msg = "squawks.";
        break;
    case MS_HISS:
        if (!mtmp->mpeaceful)
            pline_msg = "hisses!";
        else
            return 0; /* no sound */
        break;
    case MS_BUZZ:
        pline_msg = mtmp->mpeaceful ? "drones." : "buzzes angrily.";
        break;
    case MS_GRUNT:
        pline_msg = "grunts.";
        break;
    case MS_NEIGH:
        if (mtmp->mtame < 5)
            pline_msg = "neighs.";
        else if (moves > EDOG(mtmp)->hungrytime)
            pline_msg = "whinnies.";
        else
            pline_msg = "whickers.";
        break;
    case MS_WAIL:
        pline_msg = "wails mournfully.";
        break;
    case MS_GURGLE:
        pline_msg = "gurgles.";
        break;
    case MS_BURBLE:
        pline_msg = "burbles.";
        break;
    case MS_SHRIEK:
        pline_msg = "shrieks.";
        aggravate();
        break;
    case MS_IMITATE:
        pline_msg = "imitates you.";
        break;
    case MS_BONES:
        pline("%s rattles noisily.", Monnam(mtmp));
        You("freeze for a moment.");
        nomul(-2);
        multi_reason = "scared by rattling";
        nomovemsg = 0;
        break;
    case MS_LAUGH: {
        static const char *const laugh_msg[4] = {
            "giggles.", "chuckles.", "snickers.", "laughs.",
        };
        pline_msg = laugh_msg[rn2(4)];
    } break;
    case MS_MUMBLE:
        pline_msg = "mumbles incomprehensibly.";
        break;
    case MS_DJINNI:
        if (mtmp->mtame) {
            verbl_msg = "Sorry, I'm all out of wishes.";
        } else if (mtmp->mpeaceful) {
            if (ptr == &mons[PM_WATER_DEMON])
                pline_msg = "gurgles.";
            else
                verbl_msg = "I'm free!";
        } else {
            if (ptr != &mons[PM_PRISONER])
                verbl_msg = "This will teach you not to disturb me!";
#if 0
            else
                verbl_msg = "??????????";
#endif
        }
        break;
    case MS_BOAST: /* giants */
        if (!mtmp->mpeaceful) {
            switch (rn2(4)) {
            case 0:
                pline("%s boasts about %s gem collection.", Monnam(mtmp),
                      mhis(mtmp));
                break;
            case 1:
                pline_msg = "complains about a diet of mutton.";
                break;
            default:
                pline_msg = "shouts \"Fee Fie Foe Foo!\" and guffaws.";
                wake_nearto(mtmp->mx, mtmp->my, 7 * 7);
                break;
            }
            break;
        }
        /*FALLTHRU*/
    case MS_HUMANOID:
        if (!mtmp->mpeaceful) {
            if (In_endgame(&u.uz) && is_mplayer(ptr))
                mplayer_talk(mtmp);
            else
                pline_msg = "threatens you.";
            break;
        }
        /* Generic peaceful humanoid behaviour. */
        if (mtmp->mflee)
            pline_msg = "wants nothing to do with you.";
        else if (mtmp->mhp < mtmp->mhpmax / 4)
            pline_msg = "moans.";
        else if (mtmp->mconf || mtmp->mstun)
            verbl_msg = !rn2(3) ? "Huh?" : rn2(2) ? "What?" : "Eh?";
        else if (!mtmp->mcansee)
            verbl_msg = "I can't see!";
        else if (mtmp->mtrapped) {
            struct trap *t = t_at(mtmp->mx, mtmp->my);

            if (t)
                t->tseen = 1;
            verbl_msg = "I'm trapped!";
        } else if (mtmp->mhp < mtmp->mhpmax / 2)
            pline_msg = "asks for a potion of healing.";
        else if (mtmp->mtame && !mtmp->isminion
                 && moves > EDOG(mtmp)->hungrytime)
            verbl_msg = "I'm hungry.";
        /* Specific monsters' interests */
        else if (is_elf(ptr))
            pline_msg = "curses orcs.";
        else if (is_dwarf(ptr))
            pline_msg = "talks about mining.";
        else if (likes_magic(ptr))
            pline_msg = "talks about spellcraft.";
        else if (ptr->mlet == S_CENTAUR)
            pline_msg = "discusses hunting.";
        else
            switch (monsndx(ptr)) {
            case PM_HOBBIT:
                pline_msg =
                    (mtmp->mhpmax - mtmp->mhp >= 10)
                        ? "complains about unpleasant dungeon conditions."
                        : "asks you about the One Ring.";
                break;
            case PM_ARCHEOLOGIST:
                pline_msg =
                "describes a recent article in \"Spelunker Today\" magazine.";
                break;
            case PM_TOURIST:
                verbl_msg = "Aloha.";
                break;
            default:
                pline_msg = "discusses dungeon exploration.";
                break;
            }
        break;
    case MS_SEDUCE: {
        int swval;

        if (SYSOPT_SEDUCE) {
            if (ptr->mlet != S_NYMPH
                && could_seduce(mtmp, &youmonst, (struct attack *) 0) == 1) {
                (void) doseduce(mtmp);
                break;
            }
            swval = ((poly_gender() != (int) mtmp->female) ? rn2(3) : 0);
        } else
            swval = ((poly_gender() == 0) ? rn2(3) : 0);
        switch (swval) {
        case 2:
            verbl_msg = "Hello, sailor.";
            break;
        case 1:
            pline_msg = "comes on to you.";
            break;
        default:
            pline_msg = "cajoles you.";
        }
    } break;
    case MS_ARREST:
        if (mtmp->mpeaceful)
            verbalize("Just the facts, %s.", flags.female ? "Ma'am" : "Sir");
        else {
            static const char *const arrest_msg[3] = {
                "Anything you say can be used against you.",
                "You're under arrest!", "Stop in the name of the Law!",
            };
            verbl_msg = arrest_msg[rn2(3)];
        }
        break;
    case MS_BRIBE:
        if (mtmp->mpeaceful && !mtmp->mtame) {
            (void) demon_talk(mtmp);
            break;
        }
    /* fall through */
    case MS_CUSS:
        if (!mtmp->mpeaceful)
            cuss(mtmp);
        else if (is_lminion(mtmp))
            verbl_msg = "It's not too late.";
        else
            verbl_msg = "We're all doomed.";
        break;
    case MS_SPELL:
        /* deliberately vague, since it's not actually casting any spell */
        pline_msg = "seems to mutter a cantrip.";
        break;
    case MS_NURSE:
        verbl_msg_mcan = "I hate this job!";
        if (uwep && (uwep->oclass == WEAPON_CLASS || is_weptool(uwep)))
            verbl_msg = "Put that weapon away before you hurt someone!";
        else if (uarmc || uarmo || uarm || uarmh || uarms || uarmb || uarmg || uarmf)
            verbl_msg = Role_if(PM_HEALER)
                            ? "Doc, I can't help you unless you cooperate."
                            : "Please undress so I can examine you.";
        else if (uarmu)
            verbl_msg = "Take off your shirt, please.";
        else
            verbl_msg = "Relax, this won't hurt a bit.";
        break;
    case MS_GUARD:
        if (money_cnt(invent))
            verbl_msg = "Please drop that gold and follow me.";
        else
            verbl_msg = "Please follow me.";
        break;
    case MS_SOLDIER: {
        static const char
            *const soldier_foe_msg[3] =
                {
                  "Resistance is useless!", "You're dog meat!", "Surrender!",
                },
                   *const soldier_pax_msg[3] = {
                       "What lousy pay we're getting here!",
                       "The food's not fit for Orcs!",
                       "My feet hurt, I've been on them all day!",
                   };
        verbl_msg = mtmp->mpeaceful ? soldier_pax_msg[rn2(3)]
                                    : soldier_foe_msg[rn2(3)];
        break;
    }
    case MS_RIDER: {
        const char *tribtitle;
        struct obj *book = 0;
        boolean ms_Death = (ptr == &mons[PM_DEATH]);

        /* 3.6 tribute */
        if (ms_Death && !context.tribute.Deathnotice
            && (book = u_have_novel()) != 0) {
            if ((tribtitle = noveltitle(&book->novelidx)) != 0) {
                Sprintf(verbuf, "Ah, so you have a copy of /%s/.", tribtitle);
                /* no Death featured in these two, so exclude them */
                if (strcmpi(tribtitle, "Snuff")
                    && strcmpi(tribtitle, "The Wee Free Men"))
                    Strcat(verbuf, "  I may have been misquoted there.");
                verbl_msg = verbuf;
            }
            context.tribute.Deathnotice = 1;
        } else if (ms_Death && rn2(3) && Death_quote(verbuf, sizeof verbuf)) {
            verbl_msg = verbuf;
        /* end of tribute addition */

        } else if (ms_Death && !rn2(10)) {
            pline_msg = "is busy reading a copy of Sandman #8.";
        } else
            verbl_msg = "Who do you think you are, War?";
        break;
    } /* case MS_RIDER */
	default:
		pline_msg = "does not respond.";
		break;
    } /* switch */

    if (pline_msg) {
        pline("%s %s", Monnam(mtmp), pline_msg);
    } else if (mtmp->mcancelled && verbl_msg_mcan) {
        verbalize1(verbl_msg_mcan);
    } else if (verbl_msg) {
        /* more 3.6 tribute */
        if (ptr == &mons[PM_DEATH]) {
            /* Death talks in CAPITAL LETTERS
               and without quotation marks */
            char tmpbuf[BUFSZ];

            pline1(ucase(strcpy(tmpbuf, verbl_msg)));
        } else {
            verbalize1(verbl_msg);
        }
    }
    return 1;
}

/* #chat command */
int
dotalk()
{
    int result;

    result = dochat();
    return result;
}

STATIC_OVL int
dochat()
{
    struct monst *mtmp;
    int tx, ty;
    struct obj *otmp;

    if (is_silent(youmonst.data)) {
        pline("As %s, you cannot speak.", an(youmonst.data->mname));
        return 0;
    }
    if (Strangled) {
        You_cant("speak.  You're choking!");
        return 0;
    }
    if (u.uswallow) {
        pline("They won't hear you out there.");
        return 0;
    }
    if (Underwater) {
        Your("speech is unintelligible underwater.");
        return 0;
    }
    if (Deaf) {
        pline("How can you hold a conversation when you cannot hear?");
        return 0;
    }

    if (!getdir("Talk to whom? (in what direction)")) {
        /* decided not to chat */
        return 0;
    }

    if (u.usteed && u.dz > 0) {
        if (!u.usteed->mcanmove || u.usteed->msleeping) {
            pline("%s seems not to notice you.", Monnam(u.usteed));
            return 1;
        } else
            return domonnoise(u.usteed);
    }

    if (u.dz) {
        pline("They won't hear you %s there.", u.dz < 0 ? "up" : "down");
        return 0;
    }

    if (u.dx == 0 && u.dy == 0) {
        /*
         * Let's not include this.
         * It raises all sorts of questions: can you wear
         * 2 helmets, 2 amulets, 3 pairs of gloves or 6 rings as a marilith,
         * etc...  --KAA
        if (u.umonnum == PM_ETTIN) {
            You("discover that your other head makes boring conversation.");
            return 1;
        }
         */
        pline("Talking to yourself is a bad habit for a dungeoneer.");
        return 0;
    }

    tx = u.ux + u.dx;
    ty = u.uy + u.dy;

    if (!isok(tx, ty))
        return 0;

    mtmp = m_at(tx, ty);

    if ((!mtmp || mtmp->mundetected)
        && (otmp = vobj_at(tx, ty)) != 0 && otmp->otyp == STATUE) {
        /* Talking to a statue */
        if (!Blind) {
            pline_The("%s seems not to notice you.",
                      /* if hallucinating, you can't tell it's a statue */
                      Hallucination ? rndmonnam((char *) 0) : "statue");
        }
        return 0;
    }

    if (!mtmp || mtmp->mundetected || M_AP_TYPE(mtmp) == M_AP_FURNITURE
        || M_AP_TYPE(mtmp) == M_AP_OBJECT)
        return 0;

    /* sleeping monsters won't talk, except priests (who wake up) */
    if ((!mtmp->mcanmove || mtmp->msleeping) && !mtmp->ispriest) {
        /* If it is unseen, the player can't tell the difference between
           not noticing him and just not existing, so skip the message. */
        if (canspotmon(mtmp))
            pline("%s seems not to notice you.", Monnam(mtmp));
        return 0;
    }

    /* if this monster is waiting for something, prod it into action */
    mtmp->mstrategy &= ~STRAT_WAITMASK;

    if (mtmp->mtame && mtmp->meating) {
        if (!canspotmon(mtmp))
            map_invisible(mtmp->mx, mtmp->my);
        pline("%s is eating noisily.", Monnam(mtmp));
        return 0;
    }


	/* Finally, generate the actual chat menu */
	struct permonst* ptr = mtmp->data;
	int msound = ptr->msound;

	/* leader might be poly'd; if he can still speak, give leader speech */
	if (mtmp->m_id == quest_status.leader_m_id && msound > MS_ANIMAL)
		msound = MS_LEADER;
	/* make sure it's your role's quest guardian; adjust if not */
	else if (msound == MS_GUARDIAN && ptr != &mons[urole.guardnum])
		msound = mons[genus(monsndx(ptr), 1)].msound;
	/* some normally non-speaking types can/will speak if hero is similar */
	else if (msound == MS_ORC         /* note: MS_ORC is same as MS_GRUNT */
		&& (same_race(ptr, youmonst.data)           /* current form, */
			|| same_race(ptr, &mons[Race_switch]))) /* unpoly'd form */
		msound = MS_HUMANOID;
	/* silliness, with slight chance to interfere with shopping */
	else if (Hallucination && mon_is_gecko(mtmp))
		msound = MS_SELL;

	/* be sure to do this before talking; the monster might teleport away, in
	 * which case we want to check its pre-teleport position
	 */
	if (!canspotmon(mtmp))
		map_invisible(mtmp->mx, mtmp->my);

	int i = '\0';

	menu_item* pick_list = (menu_item*)0;
	winid win;
	anything any;

	any = zeroany;
	win = create_nhwindow(NHW_MENU);
	start_menu(win);


#define MAXCHATNUM 100

	struct available_chat_item
	{
		int charnum;
		char name[BUFSZ];
		int (*function_ptr)();
	};
	struct available_chat_item available_chat_list[MAXCHATNUM];
	int chatnum = 0;

	any = zeroany;

	/* Hello! This is the old chat, i.e., domonnoise function */
	strcpy(available_chat_list[chatnum].name, "\"Hello there!\"");
	available_chat_list[chatnum].function_ptr = &domonnoise;
	available_chat_list[chatnum].charnum = 'a' + chatnum;

	any = zeroany;
	any.a_char = available_chat_list[chatnum].charnum;

	add_menu(win, NO_GLYPH, &any,
		any.a_char, 0, ATR_NONE,
		available_chat_list[chatnum].name, MENU_UNSELECTED);

	chatnum++;

	if(msound >= MS_IMITATE)
	{
		/* Who are you? */
		strcpy(available_chat_list[chatnum].name, "\"Who are you?\"");
		available_chat_list[chatnum].function_ptr = &do_chat_whoareyou;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;
	}

	/* Tame dog and cat commands */
	if (has_edog(mtmp) && mtmp->mtame)
	{
		if(mtmp->data->mlet == S_DOG)
		{
			strcpy(available_chat_list[chatnum].name, "\"Sit!\"");
			available_chat_list[chatnum].function_ptr = &do_chat_pet_sit;
			available_chat_list[chatnum].charnum = 'a' + chatnum;

			any = zeroany;
			any.a_char = available_chat_list[chatnum].charnum;

			add_menu(win, NO_GLYPH, &any,
				any.a_char, 0, ATR_NONE,
				available_chat_list[chatnum].name, MENU_UNSELECTED);

			chatnum++;
		}
		if (mtmp->data->mlet == S_DOG)
		{
			strcpy(available_chat_list[chatnum].name, "\"Give paw!\"");
			available_chat_list[chatnum].function_ptr = &do_chat_pet_givepaw;
			available_chat_list[chatnum].charnum = 'a' + chatnum;

			any = zeroany;
			any.a_char = available_chat_list[chatnum].charnum;

			add_menu(win, NO_GLYPH, &any,
				any.a_char, 0, ATR_NONE,
				available_chat_list[chatnum].name, MENU_UNSELECTED);

			chatnum++;
		}

		strcpy(available_chat_list[chatnum].name, "\"Stay!\"");
		available_chat_list[chatnum].function_ptr = &do_chat_pet_stay;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

		strcpy(available_chat_list[chatnum].name, "\"Follow!\"");
		available_chat_list[chatnum].function_ptr = &do_chat_pet_follow;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

		if (mtmp->minvent && !mtmp->issummoned)
		{
			strcpy(available_chat_list[chatnum].name, "\"Drop your items!\"");
			available_chat_list[chatnum].function_ptr = &do_chat_pet_dropitems;
			available_chat_list[chatnum].charnum = 'a' + chatnum;

			any = zeroany;
			any.a_char = available_chat_list[chatnum].charnum;

			add_menu(win, NO_GLYPH, &any,
				any.a_char, 0, ATR_NONE,
				available_chat_list[chatnum].name, MENU_UNSELECTED);

			chatnum++;
		}

		if (OBJ_AT(mtmp->mx, mtmp->my) && !mtmp->issummoned)
		{
			
			strcpy(available_chat_list[chatnum].name, "\"Pick the items on the ground!\"");
			available_chat_list[chatnum].function_ptr = &do_chat_pet_pickitems;
			available_chat_list[chatnum].charnum = 'a' + chatnum;

			any = zeroany;
			any.a_char = available_chat_list[chatnum].charnum;

			add_menu(win, NO_GLYPH, &any,
				any.a_char, 0, ATR_NONE,
				available_chat_list[chatnum].name, MENU_UNSELECTED);

			chatnum++;
		}

	}

	/* Peaceful monster with sellable items */
	if (mtmp->mpeaceful && mtmp->minvent && count_sellable_items(mtmp) > 0)
	{
		strcpy(available_chat_list[chatnum].name, "Buy items");
		available_chat_list[chatnum].function_ptr = &do_chat_buy_items;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

	}

	/* Oracle */
	if (msound == MS_ORACLE)
	{
		strcpy(available_chat_list[chatnum].name, "Consultation");
		available_chat_list[chatnum].function_ptr = &do_chat_oracle_consult;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

		strcpy(available_chat_list[chatnum].name, "Identify items");
		available_chat_list[chatnum].function_ptr = &do_chat_oracle_identify;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

		strcpy(available_chat_list[chatnum].name, "Englightenment");
		available_chat_list[chatnum].function_ptr = &do_chat_oracle_enlightenment;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;
	}

	/* Priest */
	if (msound == MS_PRIEST || mtmp->ispriest)
	{
		strcpy(available_chat_list[chatnum].name, "Healing");
		available_chat_list[chatnum].function_ptr = &do_chat_priest_healing;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

		strcpy(available_chat_list[chatnum].name, "Bless or curse an item");
		available_chat_list[chatnum].function_ptr = &do_chat_priest_blesscurse;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

		strcpy(available_chat_list[chatnum].name, "Contribution to the temple");
		available_chat_list[chatnum].function_ptr = &do_chat_priest_chat;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

	}

	/* Quest */
	if (msound == MS_LEADER || msound == MS_GUARDIAN || msound == MS_NEMESIS)
	{
		strcpy(available_chat_list[chatnum].name, "Talk about your quest");
		available_chat_list[chatnum].function_ptr = &do_chat_quest_chat;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

	}

	/* Shopkeeper */
	if (msound == MS_SELL || mtmp->isshk)
	{
		strcpy(available_chat_list[chatnum].name, "\"How is your business?\"");
		available_chat_list[chatnum].function_ptr = &do_chat_shk_chat;
		available_chat_list[chatnum].charnum = 'a' + chatnum;

		any = zeroany;
		any.a_char = available_chat_list[chatnum].charnum;

		add_menu(win, NO_GLYPH, &any,
			any.a_char, 0, ATR_NONE,
			available_chat_list[chatnum].name, MENU_UNSELECTED);

		chatnum++;

		if (!Blind && (otmp = shop_object(u.ux, u.uy)) != (struct obj*) 0) 
		{
			/* standing on something in a shop and chatting causes the shopkeeper
			   to describe the price(s).  This can inhibit other chatting inside
			   a shop, but that shouldn't matter much.  shop_object() returns an
			   object iff inside a shop and the shopkeeper is present and willing
			   (not angry) and able (not asleep) to speak and the position
			   contains any objects other than just gold.
			*/
			strcpy(available_chat_list[chatnum].name, "Quote items");
			available_chat_list[chatnum].function_ptr = &do_chat_shk_pricequote;
			available_chat_list[chatnum].charnum = 'a' + chatnum;

			any = zeroany;
			any.a_char = available_chat_list[chatnum].charnum;

			add_menu(win, NO_GLYPH, &any,
				any.a_char, 0, ATR_NONE,
				available_chat_list[chatnum].name, MENU_UNSELECTED);

			chatnum++;
		}
	}


	end_menu(win, "What do you want to say?");


	if (chatnum <= 0)
	{
		You("don't have anything to talk about.");
		destroy_nhwindow(win);
		return 0;
	}


	/* Now generate the menu */
	if (select_menu(win, PICK_ONE, &pick_list) > 0) 
	{
		i = pick_list->item.a_char;
		free((genericptr_t)pick_list);
	}
	destroy_nhwindow(win);

	if (i == '\0')
		return 0;

	int res = 0;
	for (int j = 0; j < chatnum; j++)
	{
		if (available_chat_list[j].charnum == i)
		{
			if (i != '\0')
				res = (available_chat_list[j].function_ptr)(mtmp);
			break;
		}
	}

	return res;

}


STATIC_OVL int
do_chat_whoareyou(mtmp)
struct monst* mtmp;
{
	if (!mtmp)
		return 0;

	char ansbuf[BUFSZ] = "";
	int msound = mtmp->data->msound;

	if (mtmp->isshk)
	{
		register struct eshk* eshkp = (struct eshk*)0;
		if (mtmp->mextra && mtmp->mextra->eshk)
			eshkp = mtmp->mextra->eshk;

		Sprintf(ansbuf, "My name is %s.", shkname(mtmp));

		if (eshkp)
		{
			char shopbuf[BUFSZ] = "";
			Sprintf(shopbuf, "this %s", shoptypename(eshkp->shoptype));
			Sprintf(eos(ansbuf), " I run %s.", shopbuf);
		}
	
		verbalize(ansbuf);
	}
	else if (mtmp->ispriest || msound == MS_PRIEST)
	{
		Sprintf(ansbuf, "I am %s.", mon_nam(mtmp));
		verbalize(ansbuf);
	}
	else if (mtmp->mnum == PM_ORACLE || msound == MS_ORACLE)
	{
		Sprintf(ansbuf, "I am the Oracle of Delphi.");
		verbalize(ansbuf);
	}
	else if (mtmp->m_id == quest_status.leader_m_id && msound > MS_ANIMAL)
	{
		Sprintf(ansbuf, "I am %s, your quest leader.", mon_nam(mtmp));
		verbalize(ansbuf);

	}
	else if (msound == MS_GUARDIAN)
	{
		if(mtmp->mnum == urole.guardnum)
			Sprintf(ansbuf, "I am your quest guardian.");
		else
			Sprintf(ansbuf, "I am a quest guardian.");

		verbalize(ansbuf);
	}
	else if (msound == MS_NEMESIS)
	{
		Sprintf(ansbuf, "I am %s, your quest nemesis. Tremble before me!", mon_nam(mtmp));
		verbalize(ansbuf);
	}
	else
	{
		domonnoise(mtmp);
	}

	return 1;
}


STATIC_OVL int
do_chat_pet_sit(mtmp)
struct monst* mtmp;
{
	if (mtmp->mtame > 5 || (mtmp->mtame > 0 && rn2(mtmp->mtame + 1)))
	{
		pline("%s sits down!", Monnam(mtmp));
		mtmp->mstaying = 2+ rn2(5);
		mtmp->mwantstomove = 0;
	}
	else
		pline("%s stares at you but does nothing.", Monnam(mtmp));

	return 1;
}


STATIC_OVL int
do_chat_pet_givepaw(mtmp)
struct monst* mtmp;
{
	if (!mtmp)
		return 0;

	boolean givepawsuccess = FALSE;
	if (mtmp->mtame >= 1 && mtmp->mtame <= 4)
		givepawsuccess = !rn2(6 - mtmp->mtame);
	else if (mtmp->mtame >= 5)
		givepawsuccess = TRUE;

	if(givepawsuccess)
	{
		pline("%s gives you the paw!", Monnam(mtmp));
		if (mtmp->mtame < 5)
			mtmp->mtame++;
	}
	else
		pline("%s stares at you but does nothing.", Monnam(mtmp));

	return 1;
}


STATIC_OVL int
do_chat_pet_stay(mtmp)
struct monst* mtmp;
{
	if (mtmp->mtame > 5 || (mtmp->mtame > 0 && rn2(mtmp->mtame + 1)))
	{
		pline("%s sits down and looks determined not to move anywhere.", Monnam(mtmp));
		mtmp->mstaying = 25 + rn2(20);
		mtmp->mwantstomove = 0;
	}
	else
		pline("%s stares at you but does nothing.", Monnam(mtmp));

	return 1;
}


STATIC_OVL int
do_chat_pet_follow(mtmp)
struct monst* mtmp;
{
	if (mtmp->mtame > 0 && mtmp->mstaying)
	{
		pline("%s stands up and seems ready to follow you!", Monnam(mtmp));
		mtmp->mstaying = 0;
		mtmp->mwantstomove = 1;
	}
	else
		pline("%s stares at you but does nothing.", Monnam(mtmp));

	return 1;
}


STATIC_OVL int
do_chat_pet_dropitems(mtmp)
struct monst* mtmp;
{
	if (!mtmp)
		return 0;

	struct edog* edog = (struct edog*)0;
	boolean has_edog = !mtmp->isminion;

	int omx = mtmp->mx;
	int omy = mtmp->my;
	int udist = distu(omx, omy);

	if(mtmp->mextra && mtmp->mextra->edog)
		edog = mtmp->mextra->edog;

	if(has_edog && edog && droppables(mtmp))
	{
		relobj(mtmp, (int)mtmp->minvis, TRUE);
		if (edog->apport > 1)
			edog->apport--;
		edog->dropdist = udist;
		edog->droptime = monstermoves;
	}
	else
	{
		pline("%s stares at you but does nothing.", Monnam(mtmp));
	}


	return 1;
}

STATIC_OVL int
do_chat_pet_pickitems(mtmp)
struct monst* mtmp;
{
	if (!mtmp)
		return 0;

	struct edog* edog = (struct edog*)0;
	boolean has_edog = !mtmp->isminion;

	int omx = mtmp->mx;
	int omy = mtmp->my;
	int udist = distu(omx, omy);

	if (mtmp->mextra && mtmp->mextra->edog)
		edog = mtmp->mextra->edog;


	if (has_edog && edog)
	{
		int itemspicked = 0;
		boolean chastised = FALSE;
		struct obj* obj = level.objects[omx][omy];
		for (int i = 0; obj && i < 20; i++, obj = level.objects[omx][omy])
		{
			int carryamt = can_carry(mtmp, obj);
			if (carryamt > 0 && !obj->cursed && !mtmp->issummoned
				&& could_reach_item(mtmp, obj->ox, obj->oy))
			{
				struct monst* shkp = (struct monst*)0;
				if (obj && obj->unpaid || (obj->where == OBJ_FLOOR && !obj->no_charge && costly_spot(omx, omy)))
				{
					shkp = shop_keeper(inside_shop(omx, omy));
					char shopkeeper_name[BUFSZ] = "";
					if (shkp)
					{
						strcpy(shopkeeper_name, shkname(shkp));
						if (!edog->chastised)
						{
							edog->chastised = 20 + rn2(1000);
							if (cansee(omx, omy) && flags.verbose)
							{
								pline("%s tries to pick up %s.", Monnam(mtmp),
									distant_name(obj, doname));

								pline("However, %s glances at %s menacingly.", shopkeeper_name,
									mon_nam(mtmp));

								verbalize("Drop that, now!");

								pline("%s drops %s.", Monnam(mtmp),
									the(cxname(obj)));
								chastised = TRUE;
							}
						}
					}
				}

				if (!shkp)
				{
					struct obj* otmp = obj;
					if (carryamt != obj->quan)
						otmp = splitobj(obj, carryamt);

					if (cansee(omx, omy) && flags.verbose)
						pline("%s picks up %s.", Monnam(mtmp),
							distant_name(otmp, doname));

					obj_extract_self(otmp);
					newsym(omx, omy);
					(void)mpickobj(mtmp, otmp);
					itemspicked++;
				}
			}
		}
		if(itemspicked == 0 && !chastised)
		{
			pline("%s stares at you but does nothing.", Monnam(mtmp));
		}
	}
	else
	{
		pline("%s stares at you but does nothing.", Monnam(mtmp));
	}


	return 1;
}


STATIC_OVL int
do_chat_buy_items(mtmp)
struct monst* mtmp;
{
	int result = 0;


	return 1;
}

STATIC_OVL int
count_sellable_items(mtmp)
struct monst* mtmp;
{
	if (!mtmp || !mtmp->minvent)
		return 0;

	int cnt = 0;
	for (struct obj* otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
	{
		if (!otmp->owornmask)
			cnt++;
	}
	return cnt;
}

STATIC_OVL int
do_chat_oracle_consult(mtmp)
struct monst* mtmp;
{
	return doconsult(mtmp);
}

STATIC_OVL int
do_chat_oracle_identify(mtmp)
struct monst* mtmp;
{
	return do_oracle_identify(mtmp);
}

STATIC_OVL int
do_chat_oracle_enlightenment(mtmp)
struct monst* mtmp;
{
	return do_oracle_enlightenment(mtmp);
}


STATIC_OVL int
do_chat_priest_blesscurse(mtmp)
struct monst* mtmp;
{

	long umoney = money_cnt(invent);
	int u_pay, bless_cost = 200 + 10 * u.ulevel, curse_cost = 100 + 5 * u.ulevel;
	int priest_action = 0;
	char qbuf[QBUFSZ];

	if (!mtmp) {
		There("is no one here to talk to.");
		return 0;
	}
	else if (!mtmp->mpeaceful) 
	{
		pline("%s is in no mood for doing any services.", Monnam(mtmp));
		return 0;
	}
	else if (!umoney) 
	{
		You("have no money.");
		return 0;
	}

	Sprintf(qbuf, "\"Would you like to bless an item?\" (%d %s)", bless_cost, currency((long)bless_cost));
	switch (ynq(qbuf)) {
	default:
	case 'q':
		return 0;
	case 'y':
			if (umoney < (long)bless_cost) {
				You("don't have enough money for that!");
				return 0;
			}
			u_pay = bless_cost;
			priest_action = 1;
			break;
		break;
	case 'n':
		Sprintf(qbuf, "\"Then would you like to curse one?\" (%d %s)",
			curse_cost, currency((long)curse_cost));
		if (yn(qbuf) != 'y')
			return 0;
		if (umoney < (long)curse_cost)
		{
			You("don't have enough money for that!");
			return 0;
		}
		u_pay = curse_cost;
		priest_action = 2;
		break;
	}


	money2mon(mtmp, (long)u_pay);
	context.botl = 1;

	int otyp = SPE_CURSE;

	switch (priest_action) {
	case 1: /* bless */
		otyp = SPE_BLESS;
		break;
	case 2: /* curse */
		otyp = SPE_CURSE;
		break;
	default:
		break;
	}

	struct obj* pseudo = mksobj(otyp, FALSE, FALSE, FALSE);
	pseudo->blessed = pseudo->cursed = 0;
	pseudo->quan = 20L; /* do not let useup get it */
	seffects(pseudo);
	obfree(pseudo, (struct obj*)0);

	return 1;
}


STATIC_OVL int
do_chat_priest_healing(mtmp)
struct monst* mtmp;
{

	long umoney = money_cnt(invent);
	int u_pay, fullhealing_cost = 250 + 5 * u.ulevel, extrahealing_cost = 50;
	int priest_action = 0;
	char qbuf[QBUFSZ];

	if (!mtmp) 
	{
		There("is no one here to talk to.");
		return 0;
	}
	else if (!mtmp->mpeaceful)
	{
		pline("%s is in no mood for doing any services.", Monnam(mtmp));
		return 0;
	}
	else if (!umoney)
	{
		You("have no money.");
		return 0;
	}

	Sprintf(qbuf, "\"Would you like to have a full healing?\" (%d %s)", fullhealing_cost, currency((long)fullhealing_cost));
	switch (ynq(qbuf)) {
	default:
	case 'q':
		return 0;
	case 'y':
		if (umoney < (long)fullhealing_cost) {
			You("don't have enough money for that!");
			return 0;
		}
		u_pay = fullhealing_cost;
		priest_action = 1;
		break;
		break;
	case 'n':
		Sprintf(qbuf, "\"Then would you like to have a normal one?\" (%d %s)",
			extrahealing_cost, currency((long)extrahealing_cost));
		if (yn(qbuf) != 'y')
			return 0;
		if (umoney < (long)extrahealing_cost)
		{
			You("don't have enough money for that!");
			return 0;
		}
		u_pay = extrahealing_cost;
		priest_action = 2;
		break;
	}
	money2mon(mtmp, (long)u_pay);
	context.botl = 1;

	int otyp = POT_HEALING;

	switch (priest_action) {
	case 1: 
		otyp = POT_FULL_HEALING;
		break;
	case 2: 
		otyp = POT_EXTRA_HEALING;
		break;
	default:
		break;
	}

	struct obj* pseudo = mksobj(otyp, FALSE, FALSE, FALSE);
	pseudo->blessed = pseudo->cursed = 0;
	pseudo->quan = 20L; /* do not let useup get it */
	peffects(pseudo);
	obfree(pseudo, (struct obj*)0);

	return 1;
}

STATIC_OVL int
do_chat_priest_chat(mtmp)
struct monst* mtmp;
{
	priest_talk(mtmp);
	return 1;
}


STATIC_OVL int
do_chat_shk_pricequote(mtmp)
struct monst* mtmp;
{
	struct obj* otmp = shop_object(u.ux, u.uy);
	if(otmp)
		price_quote(otmp);

	return 1;
}

STATIC_OVL int
do_chat_shk_chat(mtmp)
struct monst* mtmp;
{
	shk_chat(mtmp);
	return 1;
}

STATIC_OVL int
do_chat_quest_chat(mtmp)
struct monst* mtmp;
{
	quest_chat(mtmp);
	return 1;
}


#ifdef USER_SOUNDS

extern void FDECL(play_usersound, (const char *, int));

typedef struct audio_mapping_rec {
    struct nhregex *regex;
    char *filename;
    int volume;
    struct audio_mapping_rec *next;
} audio_mapping;

static audio_mapping *soundmap = 0;

char *sounddir = ".";

/* adds a sound file mapping, returns 0 on failure, 1 on success */
int
add_sound_mapping(mapping)
const char *mapping;
{
    char text[256];
    char filename[256];
    char filespec[256];
    int volume;

    if (sscanf(mapping, "MESG \"%255[^\"]\"%*[\t ]\"%255[^\"]\" %d", text,
               filename, &volume) == 3) {
        audio_mapping *new_map;

        if (strlen(sounddir) + strlen(filename) > 254) {
            raw_print("sound file name too long");
            return 0;
        }
        Sprintf(filespec, "%s/%s", sounddir, filename);

        if (can_read_file(filespec)) {
            new_map = (audio_mapping *) alloc(sizeof(audio_mapping));
            new_map->regex = regex_init();
            new_map->filename = dupstr(filespec);
            new_map->volume = volume;
            new_map->next = soundmap;

            if (!regex_compile(text, new_map->regex)) {
                raw_print(regex_error_desc(new_map->regex));
                regex_free(new_map->regex);
                free(new_map->filename);
                free(new_map);
                return 0;
            } else {
                soundmap = new_map;
            }
        } else {
            Sprintf(text, "cannot read %.243s", filespec);
            raw_print(text);
            return 0;
        }
    } else {
        raw_print("syntax error in SOUND");
        return 0;
    }

    return 1;
}

void
play_sound_for_message(msg)
const char *msg;
{
    audio_mapping *cursor = soundmap;

    while (cursor) {
        if (regex_match(msg, cursor->regex)) {
            play_usersound(cursor->filename, cursor->volume);
        }
        cursor = cursor->next;
    }
}

#endif /* USER_SOUNDS */

/*sounds.c*/
