/* GnollHack 4.0    sounds.c    $NHDT-Date: 1542765362 2018/11/21 01:56:02 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.81 $ */
/*      Copyright (c) 1989 Janet Walz, Mike Threepoint */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <math.h>

STATIC_DCL boolean FDECL(mon_is_gecko, (struct monst *));
STATIC_DCL int FDECL(domonnoise, (struct monst *));
STATIC_DCL boolean NDECL(speak_check);
STATIC_DCL boolean NDECL(yell_check);
STATIC_DCL boolean FDECL(m_speak_check, (struct monst*));
STATIC_DCL boolean FDECL(m_general_talk_check, (struct monst*, const char*));
STATIC_DCL int NDECL(dochat);
STATIC_DCL int FDECL(do_chat_whoareyou, (struct monst*));
STATIC_DCL int FDECL(do_chat_rumors, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_sit, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_givepaw, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_pet, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_good_boy_girl, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_stay, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_standup, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_follow, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_unfollow, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_display_inventory, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dropitems, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_pickitems, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_giveitems, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_takeitems, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dowear, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dotakeoff, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dowield_hth, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dowield_ranged, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dowield_pickaxe, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dowield_axe, (struct monst*));
STATIC_DCL int FDECL(do_chat_pet_dounwield, (struct monst*));
STATIC_DCL int FDECL(do_chat_feed, (struct monst*));
STATIC_DCL int FDECL(do_chat_buy_items, (struct monst*));
STATIC_DCL int FDECL(do_chat_join_party, (struct monst*));
STATIC_DCL int FDECL(do_chat_explain_statistics, (struct monst*));
STATIC_DCL int FDECL(do_chat_oracle_consult, (struct monst*));
STATIC_DCL int FDECL(do_chat_oracle_identify, (struct monst*));
STATIC_DCL int FDECL(do_chat_oracle_enlightenment, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_blesscurse, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_full_healing, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_normal_healing, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_cure_sickness, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_chat, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_divination, (struct monst*));
STATIC_DCL int FDECL(do_chat_priest_teach_spells, (struct monst*));
STATIC_DCL int FDECL(do_chat_shk_payitems, (struct monst*));
STATIC_DCL int FDECL(do_chat_shk_pricequote, (struct monst*));
STATIC_DCL int FDECL(do_chat_shk_chat, (struct monst*));
STATIC_DCL int FDECL(do_chat_shk_identify, (struct monst*));
STATIC_DCL int FDECL(do_chat_shk_reconciliation, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_reconciliation, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_enchant_armor, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_enchant_weapon, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_repair_armor, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_repair_weapon, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_protect_armor, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_protect_weapon, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_refill_lantern, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_forge_special_armor, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_identify, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_sell_ore, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_reconciliation, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_identify_gems_and_stones, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_identify_accessories_and_charged_items, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_identify_gems_stones_and_charged_items, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_sell_gems_and_stones, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_sell_dilithium_crystals, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_sell_spellbooks, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_branch_portal, (struct monst*));
STATIC_DCL int FDECL(sell_to_npc, (struct obj*, struct monst*));
STATIC_DCL int FDECL(do_chat_npc_enchant_accessory, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_recharge, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_blessed_recharge, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_teach_spells, (struct monst*));
STATIC_DCL int FDECL(do_chat_watchman_reconciliation, (struct monst*));
STATIC_DCL int FDECL(do_chat_quest_chat, (struct monst*));
STATIC_DCL int FDECL(mon_in_room, (struct monst *, int));
STATIC_DCL int FDECL(spell_service_query, (struct monst*, int, int, const char*, int, const char*));
STATIC_DCL int FDECL(general_service_query, (struct monst*, int (*)(struct monst*), const char*, long, const char*));
STATIC_DCL int FDECL(general_service_query_with_components, (struct monst*, int (*)(struct monst*), const char*, long, const char*, const char*));
STATIC_DCL int FDECL(repair_armor_func, (struct monst*));
STATIC_DCL int FDECL(repair_weapon_func, (struct monst*));
STATIC_DCL int FDECL(refill_lantern_func, (struct monst*));
STATIC_DCL int FDECL(forge_special_func, (struct monst*, const char*, boolean (*)(struct obj*), char, int, int, int));
STATIC_DCL int FDECL(forge_dragon_scale_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_adamantium_full_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_mithril_full_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_orichalcum_full_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(learn_spell_func, (struct monst*));
STATIC_DCL int FDECL(spell_teaching, (struct monst*, int*));
STATIC_DCL boolean FDECL(maybe_dilithium_crystal, (struct obj*));
STATIC_DCL boolean FDECL(maybe_ore, (struct obj*));
STATIC_DCL boolean FDECL(maybe_adamantium_ore, (struct obj*));
STATIC_DCL boolean FDECL(maybe_mithril_ore, (struct obj*));
STATIC_DCL boolean FDECL(maybe_orichalcum_ore, (struct obj*));
STATIC_DCL boolean FDECL(maybe_dragon_scales, (struct obj*));

extern const struct shclass shtypes[]; /* defined in shknam.c */


int
ask_shk_reconciliation(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isshk || !mtmp->mextra || !ESHK(mtmp))
        return 0;

    You("try to appease %s by offering %s some compensation.", mon_nam(mtmp), mhim(mtmp));
    pline("%s says:", Monnam(mtmp));
    int res = do_chat_shk_reconciliation(mtmp);
    return res;
}

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
            if ((is_sleeping(mtmp) || is_lord(mtmp->data)
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
                    {
                        play_sfx_sound(!hallu ? SFX_LEVEL_SOMEONE_COUNTING_MONEY : SFX_LEVEL_QUARTERBACK_CALLING_PLAY);
                        You_hear(!hallu
                            ? "someone counting money."
                            : "the quarterback calling the play.");
                    }
                    else
                    {
                        play_sfx_sound(SFX_LEVEL_SOMEONE_SEARCHING);
                        You_hear("someone searching.");
                    }
                    break;
                }
            }
                /*FALLTHRU*/
            case 0:
                play_sfx_sound(SFX_LEVEL_FOOTSTEPS_OF_GUARD_PATROL);
                You_hear("the footsteps of a guard on patrol.");
                break;
            case 2:
                play_sfx_sound(SFX_LEVEL_EBENEZER_SCROOGE);
                You_hear("Ebenezer Scrooge!");
                break;
            }
        return;
    }
    if (level.flags.has_beehive && !rn2(200)) 
    {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (DEADMONSTER(mtmp))
                continue;
            if ((mtmp->data->mlet == S_ANT && is_flyer(mtmp->data))
                && mon_in_room(mtmp, BEEHIVE)) 
            {
                switch (rn2(2) + hallu) 
                {
                case 0:
                    play_sfx_sound(SFX_LEVEL_LOW_BUZZING);
                    You_hear("a low buzzing.");
                    break;
                case 1:
                    play_sfx_sound(SFX_LEVEL_ANGRY_DRONE);
                    You_hear("an angry drone.");
                    break;
                case 2:
                    play_sfx_sound(SFX_LEVEL_BEES_IN_BONNET);
                    You_hear("bees in your %sbonnet!",
                             uarmh ? "" : "(nonexistent) ");
                    break;
                }
                return;
            }
        }
    }

    if (level.flags.has_library && !rn2(200)) 
    {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (DEADMONSTER(mtmp))
                continue;
            if ((mtmp->data->mlet == S_LICH || mtmp->data == &mons[PM_IMP] || mtmp->data == &mons[PM_QUASIT]  
                || mtmp->data == &mons[PM_GNOLL_WARDEN] || mtmp->data == &mons[PM_GNOLL_SUPREME_WARDEN] || mtmp->data == &mons[PM_GNOMISH_WIZARD])
                && mon_in_room(mtmp, LIBRARY))
            {
                switch (rn2(2) + hallu)
                {
                case 0:
                    You("suddenly realize it is quiter than usual.");
                    break;
                case 1:
                    play_sfx_sound(SFX_LEVEL_SOMEONE_DEMANDING_QUIETNESS);
                    You_hear("somebody demanding quietness.");
                    break;
                case 2:
                    play_sfx_sound(SFX_LEVEL_PAGES_TURNING_IN_HEAD);
                    You_hear("pages turning in your head.");
                    break;
                }
                return;
            }
        }
    }

    if (level.flags.has_dragonlair && !rn2(200))
    {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (DEADMONSTER(mtmp))
                continue;
            if ((mtmp->data->mlet == S_DRAGON)
                && mon_in_room(mtmp, DRAGONLAIR))
            {
                switch (rn2(2) + hallu) 
                {
                case 0:
                    if (is_sleeping(mtmp))
                        You_feel("ominiously threatened.");
                    else
                        You_hear("coins being assembled.");
                    break;
                case 1:
                    if (is_sleeping(mtmp))
                    {
                        play_sfx_sound(SFX_LEVEL_LOUD_DRAGON_SNORING);
                        You_hear("loud snoring.");
                    }
                    else
                    {
                        play_sfx_sound(SFX_LEVEL_LOUD_DRAGON_ROAR);
                        You_hear("a loud roar.");
                    }
                    break;
                case 2:
                    play_sfx_sound(SFX_LEVEL_SOMEONE_CLAIMING_TO_BE_FIRE_AND_DEATH);
                    You_hear("somebody claiming to be fire and death.");
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
        static enum sfx_sound_types barracks_sound[4] = {
            SFX_LEVEL_BLADES_BEING_HONED, SFX_LEVEL_LOUD_HUMAN_SNORING, SFX_LEVEL_DICE_BEING_THROWN,
            SFX_LEVEL_GENERAL_MCARTHUR,
        };
        int count = 0;

        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if (is_mercenary(mtmp->data)
                && mon_in_room(mtmp, BARRACKS)
                /* sleeping implies not-yet-disturbed (usually) */
                && (is_sleeping(mtmp) || ++count > 5)) 
            {
                int roll = rn2(3) + hallu;

                play_sfx_sound(barracks_sound[roll]);
                You_hear1(barracks_msg[roll]);
                return;
            }
        }
    }

    if (level.flags.has_armory && !rn2(200)) {
        static const char* const armory_msg[4] = {
            "swords being unsheated.", "armors being worn.", "weapons being assembled.",
            "Master Chief!",
        };

        static enum sfx_sound_types armory_sound[4] = {
            SFX_LEVEL_SWORDS_BEING_UNSHEATED, SFX_LEVEL_ARMORS_BEING_WORN, SFX_LEVEL_WEAPONS_BEING_ASSEMBLED,
            SFX_LEVEL_MASTER_CHIEF,
        };

        int count = 0;

        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if (mon_in_room(mtmp, ARMORY)
                /* sleeping implies not-yet-disturbed (usually) */
                && (is_sleeping(mtmp) || ++count > 5))
            {
                int roll = rn2(3) + hallu;
                play_sfx_sound(armory_sound[roll]);
                You_hear1(armory_msg[roll]);
                return;
            }
        }
    }   

    if (level.flags.has_zoo && !rn2(200)) {
        static const char *const zoo_msg[3] = {
            "a sound reminiscent of an elephant stepping on a peanut.",
            "a sound reminiscent of a seal barking.", "Doctor Dolittle!",
        };
        static enum sfx_sound_types zoo_sound[3] = {
            SFX_LEVEL_ELEPHANT_STEPPING_ON_PEANUT, SFX_LEVEL_SEAL_BREAKING, SFX_LEVEL_DOCTOR_DOLITTLE,
        };
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon) {
            if (DEADMONSTER(mtmp))
                continue;
            if ((is_sleeping(mtmp) || is_animal(mtmp->data))
                && mon_in_room(mtmp, ZOO))
            {
                int roll = rn2(2) + hallu;
                play_sfx_sound(zoo_sound[roll]);
                You_hear1(zoo_msg[roll]);
                return;
            }
        }
    }
    if (level.flags.has_shop && !rn2(200)) 
    {
        if (!(sroom = search_special(ANY_SHOP))) {
            /* strange... */
            level.flags.has_shop = 0;
            return;
        }

        if (tended_shop(sroom)
            && !index(u.ushops, (int) (ROOM_INDEX(sroom) + ROOMOFFSET)))
        {
            static const char *const shop_msg[3] = {
                "someone cursing shoplifters.",
                "the chime of a cash register.", "Neiman and Marcus arguing!",
            };

            static enum sfx_sound_types shop_sound[3] = {
                SFX_LEVEL_SOMEONE_CURSING_SHOPLIFTERS, SFX_LEVEL_CHIME_OF_CASH_REGISTER, SFX_LEVEL_NEIMAN_AND_MARCUS_ARGUING,
            };

            int roll = rn2(2) + hallu;
            struct monst* shkp = sroom->resident;
            if (roll == 0 && shkp) /* Should be in his shop, since it is tended */
            {
                play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_CURSING_SHOPLIFTERS);
            }
            else
                play_sfx_sound(shop_sound[roll]);

            You_hear1(shop_msg[roll]);
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
                    {
                        play_sfx_sound(SFX_LEVEL_FAINT_CHIME);
                        You("hear a faint chime but then it fades.");
                    }
                }
                return;
            }
        }
    }

    if (level.flags.has_temple && !rn2(200)
        && !(Is_astralevel(&u.uz) || Is_sanctum(&u.uz))) 
    {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (DEADMONSTER(mtmp))
                continue;
            if (mtmp->ispriest && inhistemple(mtmp)
                /* priest must be active */
                && mon_can_move(mtmp)
                /* hero must be outside this temple */
                && temple_occupied(u.urooms) != EPRI(mtmp)->shroom)
                break;
        }
        if (mtmp) 
        {
            /* Generic temple messages; no attempt to match topic or tone
               to the pantheon involved, let alone to the specific deity.
               These are assumed to be coming from the attending priest;
               asterisk means that the priest must be capable of speech;
               pound sign (octathorpe,&c--don't go there) means that the
               priest and the altar must not be directly visible (we don't
               care if telepathy or extended detection reveals that the
               priest is not currently standing on the altar; he's mobile). */
            static const char *const temple_msg[4] = {
                "*someone praising %s.", "*someone beseeching %s.",
                "#an animal carcass being offered in sacrifice.",
                "*a strident plea for donations.",
            };
            static enum sfx_sound_types temple_sound[4] = {
                SFX_LEVEL_SOMEONE_PRAISING_GOD, SFX_LEVEL_SOMEONE_BESEECHING, SFX_LEVEL_ANIMAL_CARCASS_BEING_OFFERED, SFX_LEVEL_PLEA_FOR_DONATIONS
            };
            const char *msg;
            int trycount = 0, ax = EPRI(mtmp)->shrpos.x,
                ay = EPRI(mtmp)->shrpos.y;
            boolean speechless = (mtmp->data->msound <= MS_ANIMAL),
                    in_sight = canseemon(mtmp) || cansee(ax, ay);

            int roll = 0;
            do {
                roll = rn2(3 + hallu);
                msg = temple_msg[roll];
                if (index(msg, '*') && speechless)
                    continue;
                if (index(msg, '#') && in_sight)
                    continue;
                break; /* msg is acceptable */
            } while (++trycount < 50);

            while (!letter(*msg))
                ++msg; /* skip control flags */

            play_sfx_sound(temple_sound[roll]);
            if (index(msg, '%'))
                You_hear(msg, halu_gname(EPRI(mtmp)->shralign));
            else
                You_hear1(msg);
            return;
        }
    }

    if (level.flags.has_smithy && !rn2(200) && !Deaf)
    {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (DEADMONSTER(mtmp))
                continue;
            if (mtmp->issmith && inhissmithy(mtmp)
                /* priest must be active */
                && mon_can_move(mtmp)
                /* hero must be outside this temple */
                && smithy_occupied(u.urooms) != ESMI(mtmp)->smithy_room)
                break;
        }
        if (mtmp)
        {
            static const char* const smithy_msg[3] = {
                "iron being forged.", "loud clanging.",
                "water being vaporized.",
            };
            static enum sfx_sound_types smithy_sound[3] = {
                SFX_LEVEL_IRON_BEING_FORGED, SFX_LEVEL_LOUD_CLANGING, SFX_LEVEL_WATER_BEING_VAPORIZED,
            };

            int roll = rn2(3);
            play_sfx_sound(smithy_sound[roll]);
            You_hear1(smithy_msg[roll]);
            return;
        }
    }

    if (level.flags.has_npc_room && !rn2(200) && !Deaf)
    {
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (DEADMONSTER(mtmp))
                continue;
            if (mtmp->isnpc && in_his_npc_room(mtmp)
                /* npc must be active */
                && mon_can_move(mtmp)
                /* hero must be outside this temple */
                && npc_room_occupied(u.urooms) != ENPC(mtmp)->npc_room)
                break;
        }
        if (mtmp)
        {
            static const char* const npc_msg[3] = {
                "somebody mumbling.", "distant chitchat.",
                "footsteps at a distance.",
            };
            static enum sfx_sound_types npc_sound[3] = {
                SFX_LEVEL_SOMEONE_MUMBLING, SFX_LEVEL_DISTANT_CHITCHAT, SFX_LEVEL_FOOTSTEPS_AT_DISTANCE,
            };
            int roll = rn2(3);
            play_sfx_sound(npc_sound[roll]);
            You_hear1(npc_msg[roll]);
            return;
        }
    }

    if (Is_oracle_level(&u.uz) && !rn2(400))
    {
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
            static enum sfx_sound_types ora_sound[5] = {
                SFX_LEVEL_STRANGE_WIND, SFX_LEVEL_CONVULSIVE_RAVINGS, SFX_LEVEL_SNORING_SNAKES,
                SFX_LEVEL_NO_MORE_WOODCHUCKS, SFX_LEVEL_LOUD_ZOT,
            };
            int roll = rn2(3) + hallu * 2;
            play_sfx_sound(ora_sound[roll]);
            You_hear1(ora_msg[roll]);
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
    case MS_BLEAT:
        ret = "bleat";
        break;
    case MS_MOO:
        ret = "moo";
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

    if (!mon_can_move(mtmp) || !mtmp->data->msound)
        return;

    /* presumably nearness and soundok checks have already been made */
    if (Hallucination)
        growl_verb = h_sounds[rn2(SIZE(h_sounds))];
    else
        growl_verb = growl_sound(mtmp);
    if (growl_verb) 
    {
        play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_GROWL);
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

    if (!mon_can_move(mtmp) || !mtmp->data->msound)
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
    if (yelp_verb)
    {
        play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_UNHAPPY);
        pline("%s %s!", Monnam(mtmp), vtense((char *) 0, yelp_verb));
        if (context.run)
            nomul(0);
        wake_nearto(mtmp->mx, mtmp->my, mtmp->data->mlevel * 12);
        break_charm(mtmp, TRUE);

    }
}

/* the sounds of distressed pets */
void
whimper(mtmp)
register struct monst *mtmp;
{
    register const char *whimper_verb = 0;

    if (!mon_can_move(mtmp) || !mtmp->data->msound)
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
        play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_WHIMPER);
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
    if (!mon_can_move(mtmp)
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
    if (is_long_worm_with_tail(mon->data))
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

    if (is_silenced(mtmp))
        msound = MS_SILENT;

    if (mtmp->isshk)
        msound = MS_SELL;
    else if (mtmp->issmith)
        msound = MS_SMITH;
    else if (mtmp->isnpc)
        msound = MS_NPC;
    else if (mtmp->ispriest)
        msound = MS_PRIEST;

    int chat_line = -1;

    switch (msound) {
    case MS_SILENT:
        pline_msg = "does not respond.";
        break;
    case MS_ORACLE:
        if (is_peaceful(mtmp))
        {
            Sprintf(verbuf, "Welcome to Delphi, adventurer!");
            chat_line = 0;
        }
        else
        {
            Sprintf(verbuf, "Begone, you fool!");
            chat_line = 1;
        }
        verbl_msg = verbuf;
        break;
        //return doconsult(mtmp);
    case MS_PRIEST:
        if (is_peaceful(mtmp))
        {
            Sprintf(verbuf, "Welcome to the temple of %s, adventurer!", (mtmp->ispriest && mtmp->mextra && mtmp->mextra->epri) ? align_gname(mtmp->mextra->epri->shralign) : "our almighty god");
            chat_line = 0;
        }
        else
        {
            Sprintf(verbuf, "You shall perish by the divine hand of %s!", (mtmp->ispriest && mtmp->mextra && mtmp->mextra->epri) ? align_gname(mtmp->mextra->epri->shralign) : "our almighty god");
            chat_line = 1;
        }
        verbl_msg = verbuf;
        break;
        //priest_talk(mtmp);
    case MS_LEADER:
    case MS_NEMESIS:
    case MS_GUARDIAN:
        quest_chat(mtmp);
        break;
    case MS_SELL: /* pitch, pay, total */
        if (mtmp->isshk)
        {
            register struct eshk* eshkp = (struct eshk*)0;
            if (mtmp->mextra && mtmp->mextra->eshk)
                eshkp = mtmp->mextra->eshk;

            if (eshkp && !Deaf && !muteshk(mtmp))
            {
                char shopbuf[BUFSZ] = "";
                Sprintf(shopbuf, "my %s", shoptypename(eshkp->shoptype));
                if (is_peaceful(mtmp))
                {
                    if (eshkp->visitct > 1)
                    {
                        play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_WELCOME_BACK_TO_MY_STORE);
                        Sprintf(verbuf, "Welcome back to my store!");
                    }
                    else
                    {
                        play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_WELCOME_TO_MY_STORE);
                        Sprintf(verbuf, "Welcome to %s!", shopbuf);
                    }
                }
                else
                {
                    play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_YOU_DARE_TO_RETURN_TO_MY_STORE);
                    Sprintf(verbuf, "So, adventurer, you dare to return to my store!");
                }
            }
        }
        else
        {
            if (is_peaceful(mtmp))
            {
                Sprintf(verbuf, "Welcome, adventurer!");
                chat_line = 0;
            }
            else
            {
                Sprintf(verbuf, "You rotten thief!");
                chat_line = 1;
            }
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
    case MS_SMITH: /* pitch, pay, total */
        if (is_peaceful(mtmp))
        {
            Sprintf(verbuf, "Welcome to my smithy, adventurer!");
            chat_line = 0;
        }
        else
        {
            Sprintf(verbuf, "Begone, you rotten vandal!");
            chat_line = 1;
        }
        verbl_msg = verbuf;
        break;
    case MS_NPC: /* pitch, pay, total */
        if (is_peaceful(mtmp))
        {
            if (mtmp->isnpc && has_enpc(mtmp))
            {
                Sprintf(verbuf, "Welcome to my %s, adventurer!", npc_subtype_definitions[ENPC(mtmp)->npc_typ].room_name);
                chat_line = 0;
            }
            else
            {
                Sprintf(verbuf, "Welcome to my residence, adventurer!");
                chat_line = 1;
            }
        }
        else
        {
            Sprintf(verbuf, "Begone, you rotten vandal!");
            chat_line = 2;
        }

        verbl_msg = verbuf;
        break;
    case MS_MODRON:
        if (is_peaceful(mtmp))
        {
            if (mtmp->isnpc && has_enpc(mtmp))
            {
                Sprintf(verbuf, "Welcome to my %s on the Plane of the Modron, adventurer!", npc_subtype_definitions[ENPC(mtmp)->npc_typ].room_name);
                chat_line = 0;
            }
            else if (mtmp->issmith)
            {
                Sprintf(verbuf, "Welcome to my smithy on the Plane of the Modron, adventurer!");
                chat_line = 1;
            }
            else
            {
                Sprintf(verbuf, "Welcome to the Plane of the Modron!");
                chat_line = 2;
            }
        }
        else
        {
            Sprintf(verbuf, "Begone, you geometric anomaly!");
            chat_line = 3;
        }

        verbl_msg = verbuf;
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

        if (is_tame(mtmp)) {
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
        } else if (is_peaceful(mtmp)) {
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
                            Upolyd ? an(pm_monster_name(&mons[u.umonnum], flags.female))
                                   : an(racenoun));
                    verbl_msg = verbuf;
                } else
                    verbl_msg = vampmsg[vampindex];
            }
        }
    } break;
    case MS_WERE:
        if (flags.moonphase == FULL_MOON && (night() ^ !rn2(13))) {
            chat_line = 1;
            pline("%s throws back %s head and lets out a blood curdling %s!",
                  Monnam(mtmp), mhis(mtmp),
                  ptr == &mons[PM_HUMAN_WERERAT] ? "shriek" : ptr == &mons[PM_HUMAN_WEREBEAR] ? "growl" : "howl");
            wake_nearto(mtmp->mx, mtmp->my, 11 * 11);
        }
        else
        {
            pline_msg =
                "whispers inaudibly.  All you can make out is \"moon\".";
            chat_line = 0;
        }
        break;
    case MS_BARK:
        if (flags.moonphase == FULL_MOON && night())
        {
            pline_msg = "howls.";
            chat_line = 4;
        }
        else if (is_peaceful(mtmp)) 
        {
            if (mtmp->mtame
                && (is_confused(mtmp) || is_fleeing(mtmp) || mtmp->mtrapped
                    || moves > EDOG(mtmp)->hungrytime || mtmp->mtame < 5))
            {
                pline_msg = "whines.";
                chat_line = 2;
            }
            else if (mtmp->mtame && EDOG(mtmp)->hungrytime > moves + 1000)
            {
                pline_msg = "yips.";
                chat_line = 1;
            }
            else 
            {
                pline_msg = "barks.";
                chat_line = 0;
            }
        } 
        else
        {
            pline_msg = "growls.";
            chat_line = 3;
        }
        break;
    case MS_MEW:
        if (is_tame(mtmp)) {
            if (is_confused(mtmp) || is_fleeing(mtmp) || mtmp->mtrapped
                || mtmp->mtame < 5)
            {
                pline_msg = "yowls.";
                chat_line = 3;
            }
            else if (moves > EDOG(mtmp)->hungrytime)
            {
                pline_msg = "meows.";
                chat_line = 2;
            }
            else if (EDOG(mtmp)->hungrytime > moves + 1000)
            {
                pline_msg = "purrs.";
                chat_line = 1;
            }
            else
            {
                pline_msg = "mews.";
                chat_line = 0;
            }
            break;
        }
        /*FALLTHRU*/
    case MS_GROWL:
        pline_msg = is_peaceful(mtmp) ? "snarls." : "growls!";
        chat_line = is_peaceful(mtmp) ? 1 : 0;
        break;
    case MS_ROAR:
        pline_msg = is_peaceful(mtmp) ? "snarls." : "roars!";
        chat_line = is_peaceful(mtmp) ? 1 : 0;
        break;
    case MS_SQEEK:
        pline_msg = "squeaks.";
        chat_line = 0;
        break;
    case MS_SQAWK:
        if (ptr == &mons[PM_RAVEN] && !is_peaceful(mtmp))
        {
            verbl_msg = "Nevermore!";
            chat_line = 1;
        }
        else
            pline_msg = "squawks.";
        break;
    case MS_HISS:
        if (!is_peaceful(mtmp))
        {
            pline_msg = "hisses!";
            chat_line = 1;
        }
        else
        {
            pline_msg = "hisses gently.";
            chat_line = 0;
        }
        break;
    case MS_BUZZ:
        pline_msg = is_peaceful(mtmp) ? "drones." : "buzzes angrily.";
        chat_line = is_peaceful(mtmp) ? 0 : 1;
        break;
    case MS_GRUNT:
        pline_msg = "grunts.";
        chat_line = 0;
        break;
    case MS_NEIGH:
        if (mtmp->mtame < 5)
        {
            pline_msg = "neighs.";
            chat_line = 2;
        }
        else if (moves > EDOG(mtmp)->hungrytime)
        {
            pline_msg = "whinnies.";
            chat_line = 1;
        }
        else
        {
            pline_msg = "whickers.";
            chat_line = 0;
        }
        break;
    case MS_BLEAT:
        pline_msg = "bleats.";
        chat_line = 0;
        break;
    case MS_MOO:
        pline_msg = is_peaceful(mtmp) ? "moos." : "moos threateningly."; 
        chat_line = is_peaceful(mtmp) ? 0 : 1;
        break;
    case MS_WAIL:
        pline_msg = "wails mournfully.";
        chat_line = 0;
        break;
    case MS_GURGLE:
        pline_msg = "gurgles.";
        chat_line = 0;
        break;
    case MS_BURBLE:
        pline_msg = "burbles.";
        chat_line = 0;
        break;
    case MS_SHRIEK:
        pline_msg = "shrieks.";
        chat_line = 0;
        aggravate();
        break;
    case MS_IMITATE:
        pline_msg = "imitates you.";
        chat_line = 0;
        break;
    case MS_BONES:
        pline("%s rattles noisily.", Monnam(mtmp));
        chat_line = 0;
        You("freeze for a moment.");
        nomul(-2);
        multi_reason = "scared by rattling";
        nomovemsg = 0;
        break;
    case MS_LAUGH: {
        static const char *const laugh_msg[4] = {
            "giggles.", "chuckles.", "snickers.", "laughs.",
        };
        int roll = rn2(4);
        pline_msg = laugh_msg[roll];
        chat_line = roll;
    } break;
    case MS_MUMBLE:
        pline_msg = "mumbles incomprehensibly.";
        chat_line = 0;
        break;
    case MS_DJINNI:
        if (is_tame(mtmp)) {
            verbl_msg = "Sorry, I'm all out of wishes.";
            chat_line = 2;
        } else if (is_peaceful(mtmp)) {
            if (ptr == &mons[PM_WATER_DEMON])
            {
                pline_msg = "gurgles.";
                chat_line = 1;
            }
            else
            {
                verbl_msg = "I'm free!";
                chat_line = 1;
            }
        } else {
            if (ptr != &mons[PM_PRISONER])
            {
                verbl_msg = "This will teach you not to disturb me!";
                chat_line = 2;
            }
#if 0
            else
                verbl_msg = "??????????";
#endif
        }
        break;
    case MS_BOAST: /* giants */
        if (!is_peaceful(mtmp)) {
            switch (rn2(4)) {
            case 0:
                pline("%s boasts about %s gem collection.", Monnam(mtmp),
                      mhis(mtmp));
                chat_line = 2;
                break;
            case 1:
                pline_msg = "complains about a diet of mutton.";
                chat_line = 1;
                break;
            default:
                pline_msg = "shouts \"Fee Fie Foe Foo!\" and guffaws.";
                chat_line = 0;
                wake_nearto(mtmp->mx, mtmp->my, 7 * 7);
                break;
            }
            break;
        }
        /*FALLTHRU*/
    case MS_HUMANOID:
        if (!is_peaceful(mtmp)) {
            if (In_endgame(&u.uz) && is_mplayer(ptr))
                mplayer_talk(mtmp);
            else
            {
                pline_msg = "threatens you.";
                chat_line = 0;
            }
            break;
        }
        /* Generic peaceful humanoid behaviour. */
        if (is_fleeing(mtmp))
        {
            pline_msg = "wants nothing to do with you.";
            chat_line = 1;
        }
        else if (mtmp->mhp < mtmp->mhpmax / 4)
        {
            pline_msg = "moans.";
            chat_line = 2;
        }
        else if (is_confused(mtmp) || is_stunned(mtmp))
        {
            const char* conf_msg[3] = { "Huh?", "What?", "Eh?" };
        
            int roll = rn2(3);
            verbl_msg = conf_msg[roll];
            chat_line = 3 + roll;
        }
        else if (is_blinded(mtmp))
        {
            verbl_msg = "I can't see!";
            chat_line = 6;
        }
        else if (mtmp->mtrapped) 
        {
            struct trap *t = t_at(mtmp->mx, mtmp->my);

            if (t)
                t->tseen = 1;
            verbl_msg = "I'm trapped!";
            chat_line = 7;
        } 
        else if (mtmp->mhp < mtmp->mhpmax / 2)
        {
            pline_msg = "asks for a potion of healing.";
            chat_line = 8;
        }
        else if (mtmp->mtame && !mtmp->isminion
            && moves > EDOG(mtmp)->hungrytime)
        {
            verbl_msg = "I'm hungry.";
            chat_line = 9;
        }
        /* Specific monsters' interests */
        else if (is_elf(ptr))
        {
            pline_msg = "curses orcs.";
            chat_line = 10;
        }
        else if (is_dwarf(ptr))
        {
            pline_msg = "talks about mining.";
            chat_line = 11;
        }
        else if (likes_magic(ptr))
        {
            pline_msg = "talks about spellcraft.";
            chat_line = 12;
        }
        else if (ptr->mlet == S_CENTAUR)
        {
            pline_msg = "discusses hunting.";
            chat_line = 13;
        }
        else
            switch (monsndx(ptr)) 
            {
            case PM_HOBBIT:
                pline_msg =
                    (mtmp->mhpmax - mtmp->mhp >= 10)
                        ? "complains about unpleasant dungeon conditions."
                        : "asks you about the One Ring.";

                chat_line = 14 + ((mtmp->mhpmax - mtmp->mhp >= 10) ? 1 : 0);
                break;
            case PM_ARCHAEOLOGIST:
                pline_msg =
                "describes a recent article in \"Spelunker Today\" magazine.";
                chat_line = 16;
                break;
            case PM_TOURIST:
                verbl_msg = "Aloha.";
                chat_line = 17;
                break;
            case PM_ABBOT:
            case PM_MONK:
                if (In_modron_level(&u.uz))
                {
                    pline_msg = "discusses the ways of the Modron.";
                    chat_line = 18;
                }
                else
                {
                    pline_msg = "advises you on meditation.";
                    chat_line = 19;
                }
                break;
            default:
                pline_msg = "discusses dungeon exploration.";
                chat_line = 20;
                break;
            }
        break;
    case MS_SEDUCE: {
        int swval;

        if (SYSOPT_SEDUCE)
        {
            if (ptr->mlet != S_NYMPH
                && could_seduce(mtmp, &youmonst, (struct attack *) 0) == 1) {
                (void) doseduce(mtmp);
                break;
            }
            swval = ((poly_gender() != (int) mtmp->female) ? rn2(3) : 0);
        }
        else
        {
            swval = ((poly_gender() == 0) ? rn2(3) : 0);
        }

        switch (swval) 
        {
        case 2:
            verbl_msg = "Hello, sailor.";
            break;
        case 1:
            pline_msg = "comes on to you.";
            break;
        default:
            pline_msg = "cajoles you.";
        }
        chat_line = swval;
    } break;
    case MS_ARREST:
        if (is_peaceful(mtmp))
        {
            verbalize("Just the facts, %s.", flags.female ? "Ma'am" : "Sir");
            chat_line = flags.female ? 4 : 3;
        }
        else 
        {
            static const char *const arrest_msg[3] = {
                "Anything you say can be used against you.",
                "You're under arrest!", "Stop in the name of the Law!",
            };
            int roll = rn2(3);
            verbl_msg = arrest_msg[roll];
            chat_line = roll;
        }
        break;
    case MS_BRIBE:
        if (is_peaceful(mtmp) && !is_tame(mtmp)) {
            (void) demon_talk(mtmp);
            break;
        }
    /* fall through */
    case MS_CUSS:
        if (!is_peaceful(mtmp))
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
        {
            verbl_msg = "Put that weapon away before you hurt someone!";
            chat_line = 1;
        }
        else if (uarmc || uarmo || uarm || uarmh || uarms || uarmb || uarmg || uarmf)
        {
            verbl_msg = Role_if(PM_HEALER)
                ? "Doc, I can't help you unless you cooperate."
                : "Please undress so I can examine you.";

            chat_line = Role_if(PM_HEALER) ? 2 : 3;

        }
        else if (uarmu)
        {
            verbl_msg = "Take off your shirt, please.";
            chat_line = 4;
        }
        else
        {
            verbl_msg = "Relax, this won't hurt a bit.";
            chat_line = 0;
        }
        if (is_cancelled(mtmp))
            chat_line = 5;

        break;
    case MS_GUARD:
        if (money_cnt(invent))
        {
            verbl_msg = "Please drop that gold and follow me.";
            chat_line = 0;
        }
        else
        {
            verbl_msg = "Please follow me.";
            chat_line = 1;
        }
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
        int roll = rn2(3);
        chat_line = is_peaceful(mtmp) ? roll + 3 : roll;
        verbl_msg = is_peaceful(mtmp) ? soldier_pax_msg[roll]
                                    : soldier_foe_msg[roll];
        break;
    }
    case MS_RIDER:
    {
        const char *tribtitle;
        struct obj *book = 0;
        boolean ms_Death = (ptr == &mons[PM_DEATH]);

        /* 3.6 tribute */
        if (ms_Death && !context.tribute.Deathnotice
            && (book = u_have_novel()) != 0) 
        {
            if ((tribtitle = noveltitle(&book->novelidx)) != 0) 
            {
                Sprintf(verbuf, "Ah, so you have a copy of /%s/.", tribtitle);
                /* no Death featured in these two, so exclude them */
                if (strcmpi(tribtitle, "Snuff")
                    && strcmpi(tribtitle, "The Wee Free Men"))
                    Strcat(verbuf, "  I may have been misquoted there.");
                verbl_msg = verbuf;
                chat_line = 2;
            }
            context.tribute.Deathnotice = 1;
        } 
        else if (ms_Death && rn2(3) && Death_quote(verbuf, sizeof verbuf)) 
        {
            verbl_msg = verbuf;
            /* end of tribute addition */
            chat_line = -1; /* This needs to be activated later */
        }
        else if (ms_Death && !rn2(10)) 
        {
            pline_msg = "is busy reading a copy of Sandman #8.";
            chat_line = 1;
        }
        else
        {
            verbl_msg = "Who do you think you are, War?";
            chat_line = 0;
        }
        break;
    } /* case MS_RIDER */
    default:
        pline_msg = "does not respond.";
        break;
    } /* switch */


    if(chat_line >= 0)
       play_monster_chat_sound(mtmp, chat_line);

    if (pline_msg) 
    {
        pline("%s %s", Monnam(mtmp), pline_msg);
    }
    else if (is_cancelled(mtmp) && verbl_msg_mcan)
    {
        verbalize1(verbl_msg_mcan);
    } 
    else if (verbl_msg) 
    {
        /* more 3.6 tribute */
        if (ptr == &mons[PM_DEATH]) 
        {
            /* Death talks in CAPITAL LETTERS
               and without quotation marks */
            char tmpbuf[BUFSZ];

            pline1(ucase(strcpy(tmpbuf, verbl_msg)));
        }
        else 
        {
            verbalize1(verbl_msg);
        }
    }
    return 1;
}

/* #yell command */
int
doyell()
{
    int result = 0;

    if (yell_check())
    {
        play_simple_player_sound(MONSTER_SOUND_TYPE_YELL);
        wake_nearby();
        boolean petfound = FALSE;
        for (struct monst* mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (!DEADMONSTER(mtmp) && is_tame(mtmp))
            {
                petfound = TRUE;
                if (!is_deaf(mtmp))
                {
                    mtmp->mcomingtou = 100 + rnd(50);
                    mtmp->yell_x = u.ux;
                    mtmp->yell_y = u.uy;
                }
            }
        }
        You("yell loudly%s!", petfound ? " for your companions" : "");
    }
    return result;
}


/* #chat command */
int
dotalk()
{
    int result;

    result = dochat();
    return result;
}

STATIC_OVL boolean
speak_check()
{
    if (is_silent(youmonst.data) || !can_speak_language(youmonst.data))
    {
        pline("As %s, you cannot speak.", an(mon_monster_name(&youmonst)));
        return 0;
    }
    if (Strangled)
    {
        You_cant("speak.  You're choking!");
        return 0;
    }
    if (Silenced)
    {
        You_cant("speak.  Your voice is gone!");
        return 0;
    }
    if (Underwater)
    {
        Your("speech is unintelligible underwater.");
        return 0;
    }
    if (Deaf)
    {
        pline("How can you hold a conversation when you cannot hear?");
        return 0;
    }
    return 1;
}

STATIC_OVL boolean
yell_check()
{
    if (is_silent(youmonst.data) || !can_speak_language(youmonst.data))
    {
        pline("As %s, you cannot yell.", an(mon_monster_name(&youmonst)));
        return 0;
    }
    if (Strangled)
    {
        You_cant("yell.  You're choking!");
        return 0;
    }
    if (Silenced)
    {
        You_cant("yell.  Your voice is gone!");
        return 0;
    }
    if (Underwater)
    {
        You_cant("yell underwater.");
        return 0;
    }
    return 1;
}

STATIC_OVL boolean
m_speak_check(mtmp)
struct monst* mtmp;
{
    if (is_silenced(mtmp))
    {
        char Mhis[BUFSIZ];
        strcpy(Mhis, mhis(mtmp));
        *Mhis = highc(*Mhis);
        pline("%s cannot answer you. %s voice is gone!", Monnam(mtmp), Mhis);
        return 0;
    }

    return 1;
}

STATIC_OVL boolean
m_general_talk_check(mtmp, nomoodstr)
struct monst* mtmp;
const char* nomoodstr;
{
    if (!nomoodstr || !mtmp)
        return 0;

    if (!mtmp) 
    {
        There("is no one here to talk to.");
        return 0;
    }
    else if (!is_peaceful(mtmp)) 
    {
        pline("%s is in no mood for %s.", Monnam(mtmp), nomoodstr);
        return 0;
    }

    return 1;
}

void
genl_chat_message()
{
    while (1)
    {
        char buf[BUFSIZ] = "";
        getlin("Chat>", buf);
        if (!*buf || *buf == '\033')
            return;

        pline("> %s", buf);
    }
}

STATIC_OVL int
dochat()
{
    struct monst *mtmp;
    int tx, ty;
    struct obj *otmp;

    if (!getdir("Talk to whom? (in what direction)")) 
    {
        /* decided not to chat */
        return 0;
    }

    if (u.dx == 0 && u.dy == 0)
    {
        struct special_view_info info = { 0 };
        info.viewtype = SPECIAL_VIEW_CHAT_MESSAGE;

        /* Chat message functionality */
        open_special_view(info);
        return 0;
    }

    if (u.uswallow)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline("They won't hear you out there.");
        return 0;
    }

    if (u.usteed && u.dz > 0)
    {
        if (!mon_can_move(u.usteed)) 
        {
            play_sfx_sound(SFX_MONSTER_DOES_NOT_NOTICE);
            pline("%s seems not to notice you.", Monnam(u.usteed));
            return 1;
        } else
            return domonnoise(u.usteed);
    }

    if (u.dz)
    {
        play_sfx_sound(SFX_GENERAL_THAT_DID_NOTHING);
        pline("They won't hear you %s there.", u.dz < 0 ? "up" : "down");
        return 0;
    }

    if (u.dx == 0 && u.dy == 0) 
    {
        /* Note: Used above for chat message --JG */

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
        play_sfx_sound(SFX_GENERAL_THATS_SILLY);
        pline("Talking to yourself is a bad habit for a dungeoneer.");
        return 0;
    }

    update_u_facing(2);

    tx = u.ux + u.dx;
    ty = u.uy + u.dy;

    if (!isok(tx, ty))
        return 0;

    mtmp = m_at(tx, ty);

    if ((!mtmp || mtmp->mundetected)
        && (otmp = vobj_at(tx, ty)) != 0 && otmp->otyp == STATUE)
    {
        /* Talking to a statue */
        if (!Blind) 
        {
            play_sfx_sound(SFX_MONSTER_DOES_NOT_NOTICE);
            pline_The("%s seems not to notice you.",
                      /* if hallucinating, you can't tell it's a statue */
                      Hallucination ? rndmonnam((char *) 0) : "statue");
        }
        return 0;
    }

    if (!mtmp || mtmp->mundetected || !canspotmon(mtmp) || M_AP_TYPE(mtmp) == M_AP_FURNITURE
        || M_AP_TYPE(mtmp) == M_AP_OBJECT)
    {
        play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        pline("There is no-one to talk to.");
        return 0;
    }

#if 0
    /* Non-speaking monster */
    if (!is_speaking_monster(mtmp->data) && !is_tame(mtmp))
    {
        if (canspotmon(mtmp))
        {
            if (mtmp->data->msound <= MS_ANIMAL)
                domonnoise(mtmp);
            else
                pline("%s does not seem to be of the type that engages in conversation.", Monnam(mtmp));
        }

        return 0;
    }
    if (is_silenced(mtmp) && !is_tame(mtmp))
    {
        if (canspotmon(mtmp))
            pline("%s voice is gone and cannot answer you!", s_suffix(Monnam(mtmp)));

        return 0;
    }
#endif

    /* sleeping monsters won't talk, except priests (who wake up) */
    if (!mon_can_move(mtmp) && !mtmp->ispriest) {
        /* If it is unseen, the player can't tell the difference between
           not noticing him and just not existing, so skip the message. */
        if (canspotmon(mtmp))
        {
            play_sfx_sound(SFX_MONSTER_DOES_NOT_NOTICE);
            pline("%s seems not to notice you.", Monnam(mtmp));
        }
        return 0;
    }

    /* if this monster is waiting for something, prod it into action */
    mtmp->mstrategy &= ~STRAT_WAITMASK;

    if (is_tame(mtmp) && mtmp->meating) {
        if (!canspotmon(mtmp))
            map_invisible(mtmp->mx, mtmp->my);
        play_sfx_sound(SFX_MONSTER_IS_BUSY);
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


#define MAXCHATNUM 50

    struct available_chat_item
    {
        int charnum;
        char name[80];
        int (*function_ptr)();
    };
    struct available_chat_item available_chat_list[MAXCHATNUM] = { {0} };
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

    if(is_speaking_monster(mtmp->data) && is_peaceful(mtmp))
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


        if (is_izchak(mtmp, TRUE) || mtmp->rumorsleft >= 0)
        {
            if(is_izchak(mtmp, TRUE))
                strcpy(available_chat_list[chatnum].name, mtmp->told_rumor ? "Ask what is further on his mind" : "Ask what is on his mind");
            else
                strcpy(available_chat_list[chatnum].name, mtmp->told_rumor ? "Ask for further adventuring advice" : "Ask for adventuring advice");

            available_chat_list[chatnum].function_ptr = &do_chat_rumors;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }
    }

    /* Tame dog and cat commands */
    if (has_edog(mtmp) && is_tame(mtmp) && is_peaceful(mtmp))
    {
        if (mtmp->data->mlet == S_DOG)
        {
            Sprintf(available_chat_list[chatnum].name, "\"Good %s!\"", mtmp->female ? "girl" : "boy");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_good_boy_girl;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (mtmp->data->mlet == S_DOG && !mtmp->mstaying && mtmp->mwantstomove)
        {
            strcpy(available_chat_list[chatnum].name, "Command to sit down");
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
            strcpy(available_chat_list[chatnum].name, "Command to give paw");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_givepaw;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

        }

        if (!mtmp->mstaying && mtmp->mwantstomove)
        {

            if (is_animal(mtmp->data))
                strcpy(available_chat_list[chatnum].name, "Command to stay put");
            else if (is_speaking_monster(mtmp->data))
                strcpy(available_chat_list[chatnum].name, "Command to hold position");
            else
                strcpy(available_chat_list[chatnum].name, "Command to hold position");

            available_chat_list[chatnum].function_ptr = &do_chat_pet_stay;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }


        if (mtmp->mstaying || !mtmp->mwantstomove)
        {
            if (is_animal(mtmp->data))
                strcpy(available_chat_list[chatnum].name, "Command to stop staying put");
            else if (is_speaking_monster(mtmp->data))
                strcpy(available_chat_list[chatnum].name, "Command to stop holding position");
            else
                strcpy(available_chat_list[chatnum].name, "Command to stop holding position");

            available_chat_list[chatnum].function_ptr = &do_chat_pet_standup;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }


        if (!mtmp->mcomingtou)
        {

            strcpy(available_chat_list[chatnum].name, "Command to follow you");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_follow;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (mtmp->mcomingtou)
        {
            strcpy(available_chat_list[chatnum].name, "Command to stop following you");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_unfollow;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }



        if (mtmp->minvent)
        {
            strcpy(available_chat_list[chatnum].name, "Display inventory");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_display_inventory;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (mtmp->minvent && droppables(mtmp) && !mtmp->issummoned && !mtmp->ispartymember)
        {
            strcpy(available_chat_list[chatnum].name, "Command to drop items");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_dropitems;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (OBJ_AT(mtmp->mx, mtmp->my) && !mtmp->issummoned && !mtmp->ispartymember)
        {
            
            strcpy(available_chat_list[chatnum].name, "Command to pick the items on the ground");
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

    /* These are available also for hostile creatures */
    if (is_animal(mtmp->data))
    {
        /* Petting */
        Sprintf(available_chat_list[chatnum].name, "Pet %s", mon_nam(mtmp));
        available_chat_list[chatnum].function_ptr = &do_chat_pet_pet;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;
    }

    if ((is_domestic(mtmp->data) || mtmp->data->mlet == S_DOG || mtmp->data->mlet == S_FELINE || mtmp->data->mlet == S_YETI || mtmp->data->mlet == S_UNICORN || is_tame(mtmp)) && (carnivorous(mtmp->data) || herbivorous(mtmp->data)))
    {
        /* Feeding */
        Sprintf(available_chat_list[chatnum].name, "Feed %s", mon_nam(mtmp));
        available_chat_list[chatnum].function_ptr = &do_chat_feed;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;
    }

    if (is_tame(mtmp) && invent && is_peaceful(mtmp)) /*  && !mtmp->issummoned */
    {
        Sprintf(available_chat_list[chatnum].name, "Give items to %s", mon_nam(mtmp));
        available_chat_list[chatnum].function_ptr = &do_chat_pet_giveitems;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        if (is_packmule(mtmp->data) && mtmp->minvent)
        {
            Sprintf(available_chat_list[chatnum].name, "Take items from %s", mon_nam(mtmp));
            available_chat_list[chatnum].function_ptr = &do_chat_pet_takeitems;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

        }
    }

    if (is_tame(mtmp) && mtmp->minvent && is_peaceful(mtmp)) /*  && !mtmp->issummoned */
    {
        if(m_has_wearable_armor_or_accessory(mtmp))
        {
            Sprintf(available_chat_list[chatnum].name, "Ask to wear a piece of armor or accessory");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_dowear;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (m_has_worn_armor_or_accessory(mtmp))
        {
            Sprintf(available_chat_list[chatnum].name, "Ask to take off a piece of armor or accessory");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_dotakeoff;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }
    }

    if (is_tame(mtmp) && mtmp->minvent && can_operate_objects(mtmp->data) && attacktype(mtmp->data, AT_WEAP) && is_peaceful(mtmp)) /*  && !mtmp->issummoned */
    {
        if (select_hwep(mtmp))
        {
            Sprintf(available_chat_list[chatnum].name, "Ask to wield a hand-to-hand weapon");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_dowield_hth;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }
        if (select_rwep(mtmp) && !(MON_WEP(mtmp) && is_launcher(MON_WEP(mtmp))))
        {
            Sprintf(available_chat_list[chatnum].name, "Ask to wield a ranged weapon");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_dowield_ranged;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (m_carrying(mtmp, PICK_AXE) && !(MON_WEP(mtmp) && MON_WEP(mtmp)->otyp == PICK_AXE))
        {
            Sprintf(available_chat_list[chatnum].name, "Ask to wield a pick axe");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_dowield_pickaxe;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }
        if(m_carrying(mtmp, AXE) || ((m_carrying(mtmp, BATTLE_AXE)) && !which_armor(mtmp, W_ARMS)))
        {
            Sprintf(available_chat_list[chatnum].name, "Ask to wield an axe");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_dowield_axe;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }
        if(MON_WEP(mtmp))
        {
            Sprintf(available_chat_list[chatnum].name, "Ask to unwield the current weapon");
            available_chat_list[chatnum].function_ptr = &do_chat_pet_dounwield;
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
    if (is_peaceful(mtmp) && !(is_tame(mtmp) && !mtmp->ispartymember)
        && !mtmp->isgd
        && !mtmp->ispriest
        && !is_watch(mtmp->data)
        && msound != MS_ORACLE
        && msound != MS_ARREST
        && !(msound == MS_LEADER || msound == MS_GUARDIAN || msound == MS_NEMESIS)
        && mtmp->minvent && count_sellable_items(mtmp) > 0)
    {
        strcpy(available_chat_list[chatnum].name, "Check items for sale");
        available_chat_list[chatnum].function_ptr = &do_chat_buy_items;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

    }

    /* Ask a suitable (speaking) peaceful monster to join */
    if (is_peaceful(mtmp) && !is_tame(mtmp)
        && (mtmp->data->mflags3 & M3_CHAT_CAN_JOIN_PARTY)
        && !(mtmp->data->geno & G_UNIQ)
        && !mtmp->isshk
        && !mtmp->isgd
        && !is_watch(mtmp->data)
        && !mtmp->ispriest
        && !mtmp->issmith
        && !mtmp->isnpc
        && msound != MS_ORACLE
        && msound != MS_ARREST
        && !(msound == MS_LEADER || msound == MS_GUARDIAN || msound == MS_NEMESIS)
        )
    {
        strcpy(available_chat_list[chatnum].name, "Ask to join the party");
        available_chat_list[chatnum].function_ptr = &do_chat_join_party;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        strcpy(available_chat_list[chatnum].name, "Explain current statistics");
        available_chat_list[chatnum].function_ptr = &do_chat_explain_statistics;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;
    }


    /* Oracle */
    if (is_peaceful(mtmp) && msound == MS_ORACLE)
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

        strcpy(available_chat_list[chatnum].name, "Identify an item");
        available_chat_list[chatnum].function_ptr = &do_chat_oracle_identify;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        strcpy(available_chat_list[chatnum].name, "Enlightenment");
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
    if (is_peaceful(mtmp) && (msound == MS_PRIEST || mtmp->ispriest))
    {
        strcpy(available_chat_list[chatnum].name, "Standard healing");
        available_chat_list[chatnum].function_ptr = &do_chat_priest_normal_healing;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        strcpy(available_chat_list[chatnum].name, "Full healing");
        available_chat_list[chatnum].function_ptr = &do_chat_priest_full_healing;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        strcpy(available_chat_list[chatnum].name, "Cure sickness");
        available_chat_list[chatnum].function_ptr = &do_chat_priest_cure_sickness;
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

        strcpy(available_chat_list[chatnum].name, "Divination");
        available_chat_list[chatnum].function_ptr = &do_chat_priest_divination;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        strcpy(available_chat_list[chatnum].name, "Teach spells");
        available_chat_list[chatnum].function_ptr = &do_chat_priest_teach_spells;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        if (mtmp->ispriest && inhistemple(mtmp))
        {
            strcpy(available_chat_list[chatnum].name, "Chat about a monetary contribution to the temple");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_chat;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

    }
    else if (is_peaceful(mtmp) && is_priest(mtmp->data) && msound != MS_ORACLE)
    {
        /* Non-priest monster priests here */
        strcpy(available_chat_list[chatnum].name, "Healing");
        available_chat_list[chatnum].function_ptr = &do_chat_priest_normal_healing;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        strcpy(available_chat_list[chatnum].name, "Cure sickness");
        available_chat_list[chatnum].function_ptr = &do_chat_priest_cure_sickness;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;


        strcpy(available_chat_list[chatnum].name, "Divination");
        available_chat_list[chatnum].function_ptr = &do_chat_priest_divination;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;
    }

    /* Quest */
    if ((is_peaceful(mtmp) && (msound == MS_LEADER || msound == MS_GUARDIAN)) || msound == MS_NEMESIS)
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
        if(1)
        {
            if(is_peaceful(mtmp))
                strcpy(available_chat_list[chatnum].name, "Ask about the state of business");
            else
                Sprintf(available_chat_list[chatnum].name, "Ask about what's getting on %s nerves", mhis(mtmp));
            available_chat_list[chatnum].function_ptr = &do_chat_shk_chat;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }
        if (!is_peaceful(mtmp))
        {
            strcpy(available_chat_list[chatnum].name, "Ask for reconciliation");
            available_chat_list[chatnum].function_ptr = &do_chat_shk_reconciliation;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }
        if(invent && count_unpaid(invent))
        {
            strcpy(available_chat_list[chatnum].name, "Pay items");
            available_chat_list[chatnum].function_ptr = &do_chat_shk_payitems;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }


        int shp_indx = 0;
        if(is_peaceful(mtmp) && mtmp->mextra && ESHK(mtmp))
        {
            shp_indx = ESHK(mtmp)->shoptype - SHOPBASE;
            const struct shclass* shp = &shtypes[shp_indx];
            char itembuf[BUFSIZ] = "";
            strcpy(itembuf, shp->identified_item_description);
            
            Sprintf(available_chat_list[chatnum].name, "Identify %s", itembuf);
            available_chat_list[chatnum].function_ptr = &do_chat_shk_identify;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }


        if (is_peaceful(mtmp) && !Blind && (otmp = shop_object(u.ux, u.uy)) != (struct obj*) 0)
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

    /* Smith */
    if (msound == MS_SMITH || mtmp->issmith)
    {
        if (!is_peaceful(mtmp))
        {
            strcpy(available_chat_list[chatnum].name, "Ask for reconciliation");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_reconciliation;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (is_peaceful(mtmp) && mtmp->mextra && ESMI(mtmp) &&!mtmp->mrevived) /* no mrivived here to prevent abuse*/
        {
            strcpy(available_chat_list[chatnum].name, "Identify a weapon or armor");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_identify;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            char sbuf[BUFSIZ];
            Sprintf(sbuf, "Sell nuggets of ore to %s", mon_nam(mtmp));
            strcpy(available_chat_list[chatnum].name, sbuf);
            available_chat_list[chatnum].function_ptr = &do_chat_smith_sell_ore;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Enchant an armor");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_enchant_armor;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Enchant a weapon");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_enchant_weapon;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Repair a piece of armor");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_repair_armor;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Repair a weapon");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_repair_weapon;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Protect a piece of armor");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_protect_armor;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Protect a weapon");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_protect_weapon;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Forge a special armor");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_forge_special_armor;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Refill oil for a lamp or lantern");
            available_chat_list[chatnum].function_ptr = &do_chat_smith_refill_lantern;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

        }

    }

    /* NPCs */
    if (msound == MS_NPC || mtmp->isnpc)
    {
        if (!is_peaceful(mtmp))
        {
            strcpy(available_chat_list[chatnum].name, "Ask for reconciliation");
            available_chat_list[chatnum].function_ptr = &do_chat_npc_reconciliation;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (is_peaceful(mtmp) && has_enpc(mtmp) && !mtmp->mrevived) /* no mrivived here to prevent abuse*/
        {
            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_ENCHANT_ACCESSORY)
            {
                Sprintf(available_chat_list[chatnum].name, "Enchant an accessory");
                available_chat_list[chatnum].function_ptr = &do_chat_npc_enchant_accessory;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_RECHARGING)
            {
                Sprintf(available_chat_list[chatnum].name, "Recharge an item");
                available_chat_list[chatnum].function_ptr = &do_chat_npc_recharge;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BLESSED_RECHARGING)
            {
                Sprintf(available_chat_list[chatnum].name, "Fully recharge an item");
                available_chat_list[chatnum].function_ptr = &do_chat_npc_blessed_recharge;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_IDENTIFY_GEMS_AND_STONES)
            {
                char sbuf[BUFSIZ];
                Sprintf(sbuf, "Identify gems and stones");
                strcpy(available_chat_list[chatnum].name, sbuf);
                available_chat_list[chatnum].function_ptr = &do_chat_npc_identify_gems_and_stones;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_GEMS_AND_STONES)
            {
                char sbuf[BUFSIZ];
                Sprintf(sbuf, "Sell gems and stones to %s", mon_nam(mtmp));
                strcpy(available_chat_list[chatnum].name, sbuf);
                available_chat_list[chatnum].function_ptr = &do_chat_npc_sell_gems_and_stones;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_DILITHIUM_CRYSTALS)
            {
                char sbuf[BUFSIZ];
                Sprintf(sbuf, "Sell dilithium crystals to %s", mon_nam(mtmp));
                strcpy(available_chat_list[chatnum].name, sbuf);
                available_chat_list[chatnum].function_ptr = &do_chat_npc_sell_dilithium_crystals;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_SPELLBOOKS)
            {
                char sbuf[BUFSIZ];
                Sprintf(sbuf, "Sell spellbooks to %s", mon_nam(mtmp));
                strcpy(available_chat_list[chatnum].name, sbuf);
                available_chat_list[chatnum].function_ptr = &do_chat_npc_sell_spellbooks;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_IDENTIFY_ACCESSORIES_AND_CHARGED_ITEMS)
            {
                char sbuf[BUFSIZ];
                Sprintf(sbuf, "Identify accessories and charged items");
                strcpy(available_chat_list[chatnum].name, sbuf);
                available_chat_list[chatnum].function_ptr = &do_chat_npc_identify_accessories_and_charged_items;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_IDENTIFY_GEMS_STONES_AND_CHARGED_ITEMS)
            {
                char sbuf[BUFSIZ];
                Sprintf(sbuf, "Identify gems, stones and charged items");
                strcpy(available_chat_list[chatnum].name, sbuf);
                available_chat_list[chatnum].function_ptr = &do_chat_npc_identify_gems_stones_and_charged_items;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BRANCH_PORTAL)
            {
                Sprintf(available_chat_list[chatnum].name, "Open a branch portal");
                available_chat_list[chatnum].function_ptr = &do_chat_npc_branch_portal;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_TEACH_SPELLS_MASK)
            {
                char sbuf[BUFSIZ];
                Sprintf(sbuf, "Teach spells");
                strcpy(available_chat_list[chatnum].name, sbuf);
                available_chat_list[chatnum].function_ptr = &do_chat_npc_teach_spells;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

        }

    }


    /* Watchmen */
    if (is_watch(mtmp->data))
    {
        if(!is_peaceful(mtmp))
        {
            strcpy(available_chat_list[chatnum].name, "Ask for reconciliation");
            available_chat_list[chatnum].function_ptr = &do_chat_watchman_reconciliation;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }
    }


    /* Finish the menu */
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
            {
                res = (available_chat_list[j].function_ptr)(mtmp);

                if (res == 2) /* Changed level or the like and mtmp does not exist anymore */
                    return 1;

                if(mtmp->talkstate == 0)
                    mtmp->talkstate = 1;
                mtmp->notalktimer = 100 + rnd(200);
            }
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

    if (!m_general_talk_check(mtmp, "chatting") || !m_speak_check(mtmp))
        return 0;

    char ansbuf[BUFSZ];
    int msound = mtmp->data->msound;

    if (mtmp->isshk)
    {
        register struct eshk* eshkp = (struct eshk*)0;
        if (mtmp->mextra && mtmp->mextra->eshk)
            eshkp = mtmp->mextra->eshk;

        if (iflags.using_gui_sounds)
        {
            play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_IM_THE_SHOPKEEPER);
            Sprintf(ansbuf, "I'm the shopkeeper.");
        }
        else
            Sprintf(ansbuf, "My name is %s.", shkname(mtmp));

        if (eshkp)
        {
            char shopbuf[BUFSZ] = "";
            Sprintf(shopbuf, "this %s", shoptypename(eshkp->shoptype));
            Sprintf(eos(ansbuf), " I run %s.", shopbuf);
            play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_I_RUN_THIS_STORE);
        }
        verbalize("%s", ansbuf);
        if (iflags.using_gui_sounds && !mtmp->u_know_mname)
            pline("The name tag shows that %s name is %s.", mhis(mtmp), shkname(mtmp));

        mtmp->u_know_mname = 1;
    }
    else if (mtmp->ispriest || msound == MS_PRIEST)
    {
        if (has_mname(mtmp))
        {
            Sprintf(ansbuf, "I am %s, %s.", MNAME(mtmp), mon_nam(mtmp));
            mtmp->u_know_mname = 1;
        }
        else
            Sprintf(ansbuf, "I am %s.", mon_nam(mtmp));
        verbalize("%s", ansbuf);
    }
    else if (mtmp->mnum == PM_ORACLE || msound == MS_ORACLE)
    {
        Sprintf(ansbuf, "My name is %s.", MNAME(mtmp));
        verbalize("%s", ansbuf);
        mtmp->u_know_mname = 1;

        Sprintf(ansbuf, "I am the Oracle of Delphi.");
        verbalize("%s", ansbuf);
    }
    else if (msound == MS_ARREST)
    {
        if(!is_peaceful(mtmp))
        {
            Sprintf(ansbuf, "Hah, I'm the DDPD officer who is going to arrest you, scum!");
            verbalize("%s", ansbuf);
        }
        else
        {
            Sprintf(ansbuf, "My name is %s.", MNAME(mtmp));
            verbalize("%s", ansbuf);
            mtmp->u_know_mname = 1;

            Sprintf(ansbuf, "I work for the DDPD.");
            verbalize("%s", ansbuf);
        }
    }
    else if (is_watch(mtmp->data))
    {
        if (!is_peaceful(mtmp))
        {
            if(uwep && is_weapon(uwep))
                Sprintf(ansbuf, "Hah, drop your weapon first, scum!");
            else
                Sprintf(ansbuf, "The question is who are you, scum?");

            verbalize("%s", ansbuf);
        }
        else
        {
            if (has_mname(mtmp))
                Sprintf(ansbuf, "I am %s, a local %s.", MNAME(mtmp), mon_monster_name(mtmp));
            else
                Sprintf(ansbuf, "I am a local %s.", mon_monster_name(mtmp));

            mtmp->u_know_mname = 1;
            verbalize("%s", ansbuf);
        }
    }
    else if (mtmp->m_id == quest_status.leader_m_id && msound > MS_ANIMAL)
    {
        switch (urole.rolenum)
        {
        case ROLE_ARCHAEOLOGIST:
            Sprintf(ansbuf, "I am %s, %s.", mon_nam(mtmp), mtmp->data->mtitle);
            break;
        default:
            Sprintf(ansbuf, "I am %s, your quest leader.", mon_nam(mtmp));
            break;
        }
        play_voice_quest_leader_whoareyou(mtmp);
        verbalize("%s", ansbuf);

    }
    else if (mtmp->issmith)
    {
        if (has_mname(mtmp))
            Sprintf(ansbuf, "I am %s, a local smith.", MNAME(mtmp));
        else
            Sprintf(ansbuf, "I am a local smith.");

        mtmp->u_know_mname = 1;
        verbalize("%s", ansbuf);
    }
    else if (mtmp->isnpc && has_enpc(mtmp))
    {
        if (has_mname(mtmp))
            Sprintf(ansbuf, "I am %s, %s.", MNAME(mtmp), an(npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_role_name));
        else
            Sprintf(ansbuf, "I am a local %s.", npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_role_name);

        mtmp->u_know_mname = 1;
        verbalize("%s", ansbuf);
    }
    else if (msound == MS_GUARDIAN)
    {
        char namebuf[BUFSIZ] = "";
        if (has_mname(mtmp))
        {
            Sprintf(namebuf, "%s, ", MNAME(mtmp));
            mtmp->u_know_mname = 1;
        }

        if(mtmp->mnum == urole.guardnum)
            Sprintf(ansbuf, "I am %syour quest guardian.", namebuf);
        else
            Sprintf(ansbuf, "I am %sa quest guardian.", namebuf);

        verbalize("%s", ansbuf);
    }
    else if (msound == MS_NEMESIS)
    {
        Sprintf(ansbuf, "I am %s, your quest nemesis. Tremble before me!", mon_nam(mtmp));
        verbalize("%s", ansbuf);
    }
    else if (has_mname(mtmp))
    {
        Sprintf(ansbuf, "My name is %s.", MNAME(mtmp));
        verbalize("%s", ansbuf);
        mtmp->u_know_mname = 1;
    }
    else if (is_mname_proper_name(mtmp->data))
    {
        char titlebuf[BUFSZ];
        strcpy(titlebuf, "");
        if(mtmp->data->mtitle && strcmp(mtmp->data->mtitle, "")) 
            Sprintf(titlebuf, ", %s", mtmp->data->mtitle);

        Sprintf(ansbuf, "I am %s%s.", mon_monster_name(mtmp), titlebuf);
        verbalize("%s", ansbuf);
    }
    else
    {
        Sprintf(ansbuf, "My name is none of your business.");
        verbalize("%s", ansbuf);
    }

    return 1;
}


STATIC_OVL int
do_chat_rumors(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    if (!m_general_talk_check(mtmp, "giving any advice") || !m_speak_check(mtmp))
        return 0;

    if (mtmp->data->msound == MS_LEADER)
    {
        return do_chat_quest_chat(mtmp);
    }

    if (mtmp->data->msound == MS_ORACLE || mtmp->data == &mons[PM_ORACLE])
    {
        pline("%s answers: \"The wisdom of Delphi shall be conveyed to thee by consultation.\"", Monnam(mtmp));
        mtmp->rumorsleft = -1;
        mtmp->told_rumor = TRUE;
        return 1;
    }
    else if (is_izchak(mtmp, TRUE))
    {
        izchak_talk(mtmp);
        mtmp->told_rumor = TRUE;
        return 1;
    }

    char ansbuf[BUFSZ];
    char* rumor = getrumor(0, ansbuf, TRUE);
        
    if (mtmp->rumorsleft == 0 || !rumor)
    {
        play_voice_monster_advice(mtmp, FALSE);
        pline("%s answers: \"Unfortunately, I don't have any %s advice for you.\"", Monnam(mtmp), mtmp->told_rumor ? "further" : "useful");
        mtmp->rumorsleft = 0;
    }
    else
    {
        play_voice_monster_advice(mtmp, TRUE);
        if (mtmp->told_rumor)
            pline("%s answers: \"Let me think. Maybe keep this in mind%s\"", Monnam(mtmp), iflags.using_gui_sounds || Deaf ? "." : ":");
        else
            pline("%s answers: \"Yes, here's a piece of advice for you%s\"", Monnam(mtmp), iflags.using_gui_sounds || Deaf ? "." : ":");

        /* Tell a rumor */
        if (iflags.using_gui_sounds || Deaf)
        {
            pline("(%s hands a note over to you.)  It reads:", Monnam(mtmp));
            u.uconduct.literate++;
        }
        verbalize("%s", rumor);

        mtmp->told_rumor = TRUE;
    }

    mtmp->rumorsleft--;

    return 1;
}


STATIC_OVL int
do_chat_pet_sit(mtmp)
struct monst* mtmp;
{
    if (mtmp->mtame > 5 || (mtmp->mtame > 0 && rn2(mtmp->mtame + 1)))
    {
        pline("%s sits down!", Monnam(mtmp));
        mtmp->mstaying = 2 + rn2(5);
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
    givepawsuccess = mtmp->mtame > 1 ? rn2(mtmp->mtame) : FALSE;

    if (givepawsuccess)
    {
        pline("%s gives you the paw!", Monnam(mtmp));
        if (mtmp->mtame > 0 && mtmp->mtame < 20 && !rn2(mtmp->mtame + 20))
            mtmp->mtame++;
    }
    else
        pline("%s stares at you but does nothing.", Monnam(mtmp));

    return 1;
}

STATIC_OVL int
do_chat_pet_good_boy_girl(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    if (is_animal(mtmp->data) && mtmp->mtame > 2 && rn2(mtmp->mtame) && mon_can_move(mtmp) && (!has_edog(mtmp) || (has_edog(mtmp) && (EDOG(mtmp)->abuse <= 0 || !rn2(EDOG(mtmp)->abuse + 2)))))
    {
        play_monster_happy_sound(mtmp, MONSTER_HAPPY_SOUND_NORMAL);

        switch (mtmp->data->msound)
        {
        case MS_BARK:
            pline("%s woofs.", Monnam(mtmp));
            break;
        case MS_MEW:
            pline("%s mews softly.", Monnam(mtmp));
            break;
        case MS_NEIGH:
            pline("%s snorts.", Monnam(mtmp));
            break;
        case MS_BLEAT:
            pline("%s baas.", Monnam(mtmp));
            break;
        default:
            pline("%s seems to appreciate your kind words!", Monnam(mtmp));
            break;
        }

        if (mtmp->mtame > 0 && mtmp->mtame < 20 && !rn2(10 + mtmp->mtame))
            mtmp->mtame++;

        if (mtmp->mtame >= 15 && !mtmp->isfaithful && !rn2(max(2, 25 - mtmp->mtame)))
            mtmp->isfaithful = 1;
    }
    else if(rn2(4) && mon_can_move(mtmp))
        domonnoise(mtmp);
    else
        pline("%s does not seem to react to your words.", Monnam(mtmp));

    return 1;
}

STATIC_OVL int
do_chat_pet_pet(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    if (is_animal(mtmp->data) && !is_peaceful(mtmp))
    {
        if (!rn2(is_domestic(mtmp->data) ? 20 : 200) && !(mtmp->data->mflags2 & M2_HOSTILE) && !(mtmp->data->geno & G_UNIQ) && !mtmp->iswiz && mtmp->cham < LOW_PM)
        {
            mtmp->mpeaceful = 1;
            pline("%s seems to appreciate your gesture!", Monnam(mtmp));
        }
        else
            pline("%s does not seem to appreciate your gesture!", Monnam(mtmp));
    }
    else if (is_animal(mtmp->data) && is_peaceful(mtmp) && !is_tame(mtmp))
    {
        if (!rn2(is_domestic(mtmp->data) ? 20 : 200) && !(mtmp->data->mflags2 & M2_HOSTILE) && !(mtmp->data->geno & G_UNIQ) && !mtmp->iswiz && mtmp->cham < LOW_PM)
        {
            mtmp->mpeaceful = 1;
            tamedog(mtmp, (struct obj*)0, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, TRUE, FALSE);
            pline("%s seems to appreciate your gesture!", Monnam(mtmp));
        }
        else
        {
            pline("%s does not seem to appreciate your gesture!", Monnam(mtmp));
            if (!rn2(is_domestic(mtmp->data) ? 20 : 4) && !(mtmp->data->mflags2 & M2_PEACEFUL))
            {
                setmangry(mtmp, FALSE);
            }
        }
    }
    else if (is_animal(mtmp->data) && mtmp->mtame > 0 && rn2(mtmp->mtame + 1) && mon_can_move(mtmp) && (!has_edog(mtmp) || (has_edog(mtmp) && (EDOG(mtmp)->abuse <= 0 || !rn2(EDOG(mtmp)->abuse + 2)))))
    {
        play_monster_happy_sound(mtmp, MONSTER_HAPPY_SOUND_PURR);

        switch (mtmp->data->msound)
        {
        case MS_BARK:
            pline("%s grunts softly in appreciation!", Monnam(mtmp));
            break;
        case MS_MEW:
            pline("%s purrs in appreciation!", Monnam(mtmp));
            break;
        case MS_NEIGH:
            pline("%s snorts in appreciation!", Monnam(mtmp));
            break;
        case MS_BLEAT:
            pline("%s baas in appreciation!", Monnam(mtmp));
            break;
        default:
            pline("%s seems to appreciate your gesture!", Monnam(mtmp));
            break;
        }

        if (mtmp->mtame > 0 && mtmp->mtame < 20 && !rn2(10 + mtmp->mtame))
            mtmp->mtame++;

        if (mtmp->mtame >= 15 && !mtmp->isfaithful && !rn2(max(2, 25 - mtmp->mtame)))
            mtmp->isfaithful = 1;
    }
    else if(rn2(4) && mon_can_move(mtmp))
        domonnoise(mtmp);
    else
        pline("%s does not seem to react to your gesture.", Monnam(mtmp));

    force_redraw_at(mtmp->mx, mtmp->my);
    flush_screen(1);

    return 1;
}


STATIC_OVL int
do_chat_pet_stay(mtmp)
struct monst* mtmp;
{
    if (mtmp->mtame > 5 || (mtmp->mtame > 0 && rn2(mtmp->mtame + 1)))
    {
        if (is_steed(mtmp->data))
            pline("%s looks determined not to move anywhere.", Monnam(mtmp));
        else if is_animal(mtmp->data)
            pline("%s sits down and looks determined not to move anywhere.", Monnam(mtmp));
        else if (is_speaking_monster(mtmp->data))
            pline("%s starts to hold its position.", Monnam(mtmp));
        else
            pline("%s starts to hold its position.", Monnam(mtmp));

        mtmp->mstaying = 25 + rn2(20);
        mtmp->mwantstomove = 0;
    }
    else
        pline("%s stares at you but does nothing.", Monnam(mtmp));

    return 1;
}


STATIC_OVL int
do_chat_pet_standup(mtmp)
struct monst* mtmp;
{
    if (mtmp->mtame > 0 && mtmp->mstaying)
    {
        if (is_steed(mtmp->data))
            pline("%s stops staying put.", Monnam(mtmp));
        else if is_animal(mtmp->data)
            pline("%s stands up.", Monnam(mtmp));
        else if (is_speaking_monster(mtmp->data))
            pline("%s stops holding its position.", Monnam(mtmp));
        else
            pline("%s stops holding its position.", Monnam(mtmp));

        mtmp->mstaying = 0;
        mtmp->mwantstomove = 1;
    }
    else
        pline("%s stares at you but does nothing.", Monnam(mtmp));

    return 1;
}

STATIC_OVL int
do_chat_pet_follow(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    if (speak_check())
    {
        short oldvalue = mtmp->mcomingtou;
        mtmp->mcomingtou = 100 + rnd(50);
        mtmp->yell_x = u.ux;
        mtmp->yell_y = u.uy;

        if (mtmp->mcomingtou > oldvalue)
            pline("%s is now following you more closely.", Monnam(mtmp));
        else
            pline("%s %s.", Monnam(mtmp), has_head(mtmp->data) ? "nods" : "looks perplexed");

        return 1;
    }
    return 0;
}

STATIC_OVL int
do_chat_pet_unfollow(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    if (speak_check())
    {
        short oldvalue = mtmp->mcomingtou;
        mtmp->mcomingtou = 0;
        mtmp->yell_x = 0;
        mtmp->yell_y = 0;

        if (oldvalue > 0)
            pline("%s stops following you.", Monnam(mtmp));
        else
            pline("%s looks perplexed.", Monnam(mtmp));

        return 1;
    }

    return 0;
}


STATIC_OVL int
do_chat_pet_display_inventory(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    display_monster_inventory(mtmp, FALSE);
    return 0;
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
        mtmp->mcarrying= 0;
        mtmp->mwantstodrop = 1;
        mdrop_droppable_objs(mtmp);
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

    if (mtmp->mextra && mtmp->mextra->edog)
        edog = mtmp->mextra->edog;


    if (has_edog && edog)
    {
        int itemspicked = 0;
        int shkpreaction = FALSE;
        struct obj* obj = level.objects[omx][omy];
        for (int i = 0; obj && i < 20; i++, obj = level.objects[omx][omy])
        {
            int carryamt = can_carry(mtmp, obj);
            if (carryamt > 0 && !obj->cursed && !mtmp->issummoned && !mtmp->ispartymember
                && could_reach_item(mtmp, obj->ox, obj->oy))
            {
                shkpreaction = shk_chastise_pet(mtmp, obj, FALSE);

                if (!shkpreaction)
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
                    mtmp->mcarrying = 25 + rn2(20);
                    mtmp->mwantstodrop = 0;
                }
            }
        }
        if(itemspicked == 0 && shkpreaction != 2)
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
do_chat_pet_giveitems(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    int n, i, n_given = 0;
    long cnt;
    struct obj* otmp, * otmp2;
    menu_item* pick_list;

    char qbuf[BUFSIZ] = "";
    Sprintf(qbuf, "What would you like to give to %s?", mon_nam(mtmp));

    /* should coordinate with perm invent, maybe not show worn items */
    n = query_objlist(qbuf, &invent,
        (USE_INVLET | INVORDER_SORT), &pick_list, PICK_ANY,
        allow_all, 3);
    if (n > 0) 
    {
        bypass_objlist(invent, TRUE);
        for (i = 0; i < n; i++) {
            otmp = pick_list[i].item.a_obj;
            
            for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
                if (otmp2 == otmp)
                    break;
            if (!otmp2 || !otmp2->bypass)
                continue;
            
            /* found next selected invent item */
            cnt = pick_list[i].count;
            if (cnt < otmp->quan) {
                if (welded(otmp, &youmonst)) {
                    ; /* don't split */
                }
                else if ((objects[otmp->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && otmp->cursed) {
                    /* same kludge as getobj(), for canletgo()'s use */
                    otmp->corpsenm = (int)cnt; /* don't split */
                }
                else {
                    otmp = splitobj(otmp, cnt);
                }
            }

            /* Give here */
            int carryamt = can_carry(mtmp, otmp);

            if(otmp)
            {
                if (otmp->owornmask & (W_ARMOR | W_ACCESSORY))
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    You("cannot give %s to %s. You are wearing it.", doname(otmp), mon_nam(mtmp));
                }
                else if (carryamt == 0 || carryamt < otmp->quan)
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    pline("%s cannot carry %s.", Monnam(mtmp), yname(otmp));
                }
                else
                {
                    if (release_item_from_hero_inventory(otmp))
                    {
                        n_given++;
                        
                        if (flags.verbose)
                            You("give %s to %s.", doname(otmp), mon_nam(mtmp));

                        if (*u.ushops || otmp->unpaid)
                            check_shop_obj(otmp, mtmp->mx, mtmp->my, FALSE);

                        otmp->bypass = FALSE;
                        (void)mpickobj(mtmp, otmp);
                    }
                }
            }
        }
        bypass_objlist(invent, FALSE); /* reset invent to normal */
        free((genericptr_t)pick_list);
    }
    else
    {
        pline("Nevermind.");
    }

    return (n_given > 0);
}


STATIC_OVL int
do_chat_feed(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    int n, i, n_given = 0;
    long cnt;
    struct obj* otmp, * otmp2;
    menu_item* pick_list;

    char qbuf[BUFSIZ] = "";
    Sprintf(qbuf, "What would you like to feed to %s?", mon_nam(mtmp));

    add_valid_menu_class(0); /* clear any classes already there */
    add_valid_menu_class(FOOD_CLASS);
    add_valid_menu_class(REAGENT_CLASS);

    n = query_objlist(qbuf, &invent,
        (USE_INVLET | INVORDER_SORT), &pick_list, PICK_ONE,
        allow_category, 3);

    if (n > 0)
    {
        bypass_objlist(invent, TRUE);
        for (i = 0; i < n; i++)
        {
            otmp = pick_list[i].item.a_obj;

            for (otmp2 = invent; otmp2; otmp2 = otmp2->nobj)
                if (otmp2 == otmp)
                    break;
            if (!otmp2 || !otmp2->bypass)
                continue;

            /* found next selected invent item */
            cnt = pick_list[i].count;
            /* only one food item or potion can be fed at a time*/
            if (cnt > 1)
                cnt = 1;

            int tasty = dogfood(mtmp, otmp);
            boolean foodmakesfriendly = (!is_tame(mtmp) && befriend_with_obj(mtmp->data, otmp) && tasty <= ACCFOOD);
            boolean takesfood = (!is_tame(mtmp) && tasty <= (carnivorous(mtmp->data) ? MANFOOD : ACCFOOD));
            boolean willeat = ((is_tame(mtmp) && (tasty < (objects[otmp->otyp].oc_material == MAT_VEGGY ? APPORT : MANFOOD))) || foodmakesfriendly || takesfood);

            if (cnt < otmp->quan)
            {
                
                if (welded(otmp, &youmonst)
                    || !willeat
                    || !mon_can_move(mtmp) 
                    || mtmp->meating
                    || (otmp->owornmask & (W_ARMOR | W_ACCESSORY))
                    )
                {
                    ; /* don't split */
                }
                else if ((objects[otmp->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && otmp->cursed) 
                {
                    /* same kludge as getobj(), for canletgo()'s use */
                    otmp->corpsenm = (int)cnt; /* don't split */
                }
                else 
                {
                    otmp = splitobj(otmp, cnt);
                }
            }

            /* Feed here */
            if (otmp)
            {
                if (otmp->owornmask & (W_ARMOR | W_ACCESSORY))
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    You("cannot pass %s over to %s. You are wearing it.", an(singular(otmp, cxname)), mon_nam(mtmp));
                }
                else
                {
                    You("offer %s to %s.", an(singular(otmp, cxname)), mon_nam(mtmp));
                    n_given++;
                    int releasesuccess = TRUE;
                    if (mon_can_move(mtmp) && !mtmp->meating
                        && willeat
                        && (releasesuccess = release_item_from_hero_inventory(otmp)))
                    {
                        /* dog_eat expects a floor object */
                        if (foodmakesfriendly)
                            tamedog(mtmp, otmp, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, TRUE, FALSE);
                        else if (is_tame(mtmp) && mtmp->mextra && EDOG(mtmp))
                        {
                            place_object(otmp, mtmp->mx, mtmp->my);
                            (void)dog_eat(mtmp, otmp, mtmp->mx, mtmp->my, FALSE);
                        }
                        else
                        {
                            place_object(otmp, mtmp->mx, mtmp->my);
                            pline("%s eats %s, but does not seem to appreciate it much.", Monnam(mtmp), the(cxname(otmp)));
                            dog_food_after_effect(mtmp, otmp, canseemon(mtmp));
                            if (otmp->unpaid)
                            {
                                /* edible item owned by shop has been thrown or kicked
                                   by hero and caught by tame or food-tameable monst */
                                long oprice = unpaid_cost(otmp, TRUE);
                                pline("That %s will cost you %ld %s.", cxname_singular(otmp), oprice,
                                    currency(oprice));
                                /* delobj->obfree will handle actual shop billing update */
                            }
                            delobj(otmp);
                        }
                    }
                    else
                    {
                        if (!mon_can_move(mtmp))
                            pline("%s does not seem to be able to move in order to eat %s.", Monnam(mtmp), the(singular(otmp, cxname)));
                        else if (mtmp->meating)
                            pline("%s is already eating something else.", Monnam(mtmp));
                        else if (!releasesuccess)
                            ; /* Nothing here */
                        else
                            pline("%s refuses to eat %s.", Monnam(mtmp), the(singular(otmp, cxname)));

                    }
                }
            }
        }
        bypass_objlist(invent, FALSE); /* reset invent to normal */
        free((genericptr_t)pick_list);
    }
    else
    {
        pline("Nevermind.");
    }

    return (n_given > 0);
}



int
release_item_from_hero_inventory(obj)
struct obj* obj;
{
    if (!obj)
        return 0;
    if (!canletgo(obj, "give"))
        return 0;
    if (obj == uwep) 
    {
        if (welded(uwep, &youmonst)) 
        {
            weldmsg(obj);
            return 0;
        }
        setuwep((struct obj*) 0, W_WEP);
    }
    if (obj == uarms) {
        if (welded(uarms, &youmonst)) 
        {
            weldmsg(obj);
            return 0;
        }
        setuwep((struct obj*) 0, W_ARMS);
    }
    if (obj == uquiver) 
    {
        setuqwep((struct obj*) 0);
    }
    if (obj == uswapwep) 
    {
        setuswapwep((struct obj*) 0, W_SWAPWEP);
    }
    if (obj == uswapwep2) 
    {
        setuswapwep((struct obj*) 0, W_SWAPWEP2);
    }

    if (obj->oclass == COIN_CLASS)
        context.botl = 1;

    freeinv(obj);

    return 1;
}

STATIC_OVL int
do_chat_pet_dowear(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    m_dowear(mtmp, FALSE);
    return 1;
}

STATIC_OVL int
do_chat_pet_dotakeoff(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;
    struct obj* otmp = (struct obj*)0;

    otmp = which_armor(mtmp, W_ARMC);
    if(!otmp)
        otmp = which_armor(mtmp, W_ARMO);
    if (!otmp)
        otmp = which_armor(mtmp, W_ARM);
    if (!otmp)
        otmp = which_armor(mtmp, W_ARMU);
    if (!otmp)
        otmp = which_armor(mtmp, W_ARMS);
    if (!otmp)
        otmp = which_armor(mtmp, W_ARMH);
    if (!otmp)
        otmp = which_armor(mtmp, W_ARMG);
    if (!otmp)
        otmp = which_armor(mtmp, W_ARMF);
    if (!otmp)
        otmp = which_armor(mtmp, W_ARMB);
    if (!otmp)
        otmp = which_armor(mtmp, W_AMUL);
    if (!otmp)
        otmp = which_armor(mtmp, W_MISC);
    if (!otmp)
        otmp = which_armor(mtmp, W_MISC2);
    if (!otmp)
        otmp = which_armor(mtmp, W_MISC3);
    if (!otmp)
        otmp = which_armor(mtmp, W_MISC4);
    if (!otmp)
        otmp = which_armor(mtmp, W_MISC5);
    if (!otmp)
    {
        otmp = which_armor(mtmp, W_SADDLE);
    }

    if (!otmp)
        return 0;

    if(otmp->owornmask)
    {
        if (otmp->cursed && !cursed_items_are_positive_mon(mtmp))
        {
            play_sfx_sound(SFX_GENERAL_WELDED);
            if (otmp->owornmask & W_SADDLE)
                You("try to remove %s from %s, but you can't. It's cursed!", cxname(otmp), mon_nam(mtmp));
            else
                pline("%s tries to takes off %s, but can't. It's cursed!", Monnam(mtmp), cxname(otmp));

            otmp->bknown = TRUE;
        }
        else
        {
            mtmp->worn_item_flags &= ~otmp->owornmask;
            otmp->owornmask = 0L;

            update_all_mon_statistics(mtmp, FALSE);

            if (mtmp == u.usteed && otmp->otyp == SADDLE)
                dismount_steed(DISMOUNT_FELL);

            if(otmp->owornmask & W_SADDLE)
                You("remove %s from %s.", cxname(otmp), mon_nam(mtmp));
            else
                pline("%s takes off %s.", Monnam(mtmp), cxname(otmp));
        }
    }

    return 1;
}

STATIC_OVL int
do_chat_pet_dowield_hth(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    mtmp->weapon_strategy = NEED_HTH_WEAPON;

    boolean wielded = mon_wield_item(mtmp, TRUE);
    return wielded;
}

STATIC_OVL int
do_chat_pet_dowield_ranged(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    mtmp->weapon_strategy = NEED_RANGED_WEAPON;
    boolean wielded = mon_wield_item(mtmp, TRUE);
    return wielded;
}

STATIC_OVL int
do_chat_pet_dowield_pickaxe(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    mtmp->weapon_strategy = NEED_PICK_AXE;
    boolean wielded = mon_wield_item(mtmp, TRUE);
    return wielded;

}

STATIC_OVL int
do_chat_pet_dowield_axe(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    mtmp->weapon_strategy = NEED_AXE;
    boolean wielded = mon_wield_item(mtmp, TRUE);
    return wielded;

}

STATIC_OVL int
do_chat_pet_dounwield(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    struct obj* mwep = MON_WEP(mtmp);

    if (mwep)
    {
        if (mwelded(mwep, mtmp))
        {
            play_sfx_sound(SFX_GENERAL_WELDED);
            pline("%s tries to unwield %s, but can't. It's cursed!", Monnam(mtmp), cxname(mwep));
            mwep->bknown = TRUE;
        }
        else
        {
            setmnotwielded(mtmp, mwep);
            mtmp->weapon_strategy = NEED_WEAPON;
            pline("%s unwields %s.", Monnam(mtmp), cxname(mwep));
        }
    }
    return 1;
}



STATIC_OVL int
do_chat_join_party(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    long umoney;
    int u_pay;
    long base_join_cost = 20 + 5 * mtmp->data->difficulty * mtmp->data->difficulty;
    int ucha = ACURR(A_CHA);
    long join_cost = (base_join_cost * max(10, (100 - (ucha - 8) * 5))) / 100;
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!m_general_talk_check(mtmp, "joining") || !m_speak_check(mtmp)) {
        return 0;
    }
    else if (is_tame(mtmp)) {
        if(mtmp->ispartymember)
            pline("%s is already in your party.", Monnam(mtmp));
        else
            pline("%s is already following you.", Monnam(mtmp));
        return 0;
    }

    /*
    else if (!umoney) {
        You("have no money.");
        return 0;
    }
    */
    if (is_undead(mtmp->data) || is_demon(mtmp->data) || (mtmp->data->maligntyp < 0 && mtmp->data->difficulty > 10) )
    {
        pline("%s first %s, but then says:", Monnam(mtmp), mtmp->data->msound == MS_MUMBLE ? "mumbles incomprehensibly" : "chuckles");
        Sprintf(qbuf, "\"You shall pay me a tribute of %ld %s.\" Do you yield to this demand?", join_cost, currency(join_cost));
    }
    else
    {
        pline("%s looks at you and replies:", Monnam(mtmp));
        Sprintf(qbuf, "\"I can join you for a fee of %ld %s. Acceptable?\"", join_cost, currency(join_cost));
    }
    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)join_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = join_cost;

        money2mon(mtmp, (long)u_pay);
        context.botl = 1;

        boolean success = tamedog(mtmp, (struct obj*)0, TAMEDOG_FORCE_ALL, FALSE, 0, FALSE, FALSE);
        if (success)
        {
            mtmp->ispartymember = TRUE;
            play_sfx_sound(SFX_TAMING);
            pline("%s joins your party!", Monnam(mtmp));
        }
        else if (!is_tame(mtmp))
        {
            play_sfx_sound(SFX_SURPRISE_ATTACK);
            pline("%s takes your money but refuses join your party after all!", Monnam(mtmp));
        }
        return 1;

        break;
    }

    return 0; 
}

STATIC_OVL int
do_chat_explain_statistics(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    long umoney;
    int u_pay;
    long base_explain_cost = 5 + 1 * mtmp->data->difficulty;
    int ucha = ACURR(A_CHA);
    long explain_cost = (base_explain_cost * max(10, (100 - (ucha - 8) * 5))) / 100;
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!m_general_talk_check(mtmp, "explaining") || !m_speak_check(mtmp)) {
        return 0;
    }
    else if (is_tame(mtmp)) 
    {
        monsterdescription(mtmp);
        return 0;
    }

    if (is_undead(mtmp->data) || is_demon(mtmp->data) || (mtmp->data->maligntyp < 0 && mtmp->data->difficulty > 10))
    {
        pline("%s first %s, but then says:", Monnam(mtmp), mtmp->data->msound == MS_MUMBLE ? "mumbles incomprehensibly" : "chuckles");
        Sprintf(qbuf, "\"You shall pay me %ld %s for learning my statistics.\" Do you accept?", explain_cost, currency(explain_cost));
    }
    else
    {
        pline("%s looks at you and then says:", Monnam(mtmp));
        Sprintf(qbuf, "\"I can explain my statistics to you for a fee of %ld %s. Do you accept?\"", explain_cost, currency(explain_cost));
    }
    switch (ynq(qbuf))
    {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)explain_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = explain_cost;
        play_sfx_sound(SFX_READ);
        money2mon(mtmp, (long)u_pay);
        context.botl = 1;
        monsterdescription(mtmp);
        return 1;
    }

    return 0;
}


STATIC_OVL int
do_chat_buy_items(mtmp)
struct monst* mtmp;
{
    if (!m_general_talk_check(mtmp, "doing any business") || !m_speak_check(mtmp))
        return 0;

    int sellable_item_count = 0;

    menu_item* pick_list = (menu_item*)0;
    winid win;
    anything any;

    any = zeroany;
    win = create_nhwindow(NHW_MENU);
    start_menu(win);

    
    static char def_srt_order[MAX_OBJECT_CLASSES] = {
    COIN_CLASS, AMULET_CLASS, MISCELLANEOUS_CLASS, RING_CLASS, WAND_CLASS, POTION_CLASS,
    SCROLL_CLASS, SPBOOK_CLASS, GEM_CLASS, FOOD_CLASS, REAGENT_CLASS, TOOL_CLASS,
    WEAPON_CLASS, ARMOR_CLASS, ROCK_CLASS, BALL_CLASS, CHAIN_CLASS, 0,
    };

    const char* classorder = flags.sortpack ? flags.inv_order : def_srt_order;
    boolean classhasitems[MAX_OBJECT_CLASSES] = { 0 };

    for (struct obj* otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
    {
        if (otmp->oclass > ILLOBJ_CLASS)
            classhasitems[(int)otmp->oclass] = TRUE;
    }

    for(int i = 0; i < MAX_OBJECT_CLASSES; i++)
    {
        char oclass = classorder[i];
        boolean madeheader = FALSE;

        if (flags.sortpack && !classhasitems[(int)oclass])
            continue;

        for (struct obj* otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
        {
            if ((!flags.sortpack || (flags.sortpack && otmp->oclass == oclass)) && m_sellable_item(otmp, mtmp))
            {
                if (flags.sortpack && !madeheader)
                {
                    madeheader = TRUE;
                    any = zeroany;

                    add_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings,
                        get_class_name(oclass), MENU_UNSELECTED);
                }

                any = zeroany;
                char itembuf[BUFSIZ] = "";
                Sprintf(itembuf, "%s", doname(otmp));

                long price = get_cost_of_monster_item(otmp, mtmp);
                if (price > 0L)
                    Sprintf(eos(itembuf), " (%s, %ld %s%s)", "for sale", price, currency(price), otmp->quan > 1 ? " each" : "");
                else
                    Strcat(itembuf, " (no charge)");

                any.a_obj = otmp;
//                char let = 'a' + sellable_item_count;
//                char accel = def_oc_syms[(int)otmp->oclass].sym;

                add_menu(win, obj_to_glyph(otmp, rn2_on_display_rng), &any,
                    0, 0, ATR_NONE,
                    itembuf, MENU_UNSELECTED);

                sellable_item_count++;

            }
        }
        if (!flags.sortpack)
            break;
    }

    /* Finish the menu */
    end_menu(win, "What do you want to buy?");


    if (sellable_item_count <= 0)
    {
        pline("%s doesn't have anything to sell.", Monnam(mtmp));
        destroy_nhwindow(win);
        return 0;
    }
    else
    {
        if (!Deaf && !mtmp->isshk && (is_undead(mtmp->data) || is_demon(mtmp->data) || (mtmp->data->maligntyp < 0 && mtmp->data->difficulty > 10) ))
        {
            verbalize("%sI'm willing to trade the following items.", is_undead(mtmp->data) || is_demon(mtmp->data) ? "Greetings, mortal. " : "");
            verbalize("But be quick, my patience is limited.");
        }
        else if (!Deaf && is_speaking_monster(mtmp->data))
        {
            char itembuf[BUFSZ];
            struct obj* otmp;
            if (sellable_item_count == 1 && (otmp = get_first_sellable_item(mtmp)) != 0)
                strcpy(itembuf, iflags.using_gui_sounds ? "item" : cxname(otmp));
            else
                strcpy(itembuf, "items");

            play_monster_item_trading_line(mtmp, sellable_item_count == 1 ? MONSTER_ITEM_TRADING_LINE_TRADING_MAY_I_INTEREST_YOU_IN_FOLLOWING_ITEM : MONSTER_ITEM_TRADING_LINE_TRADING_MAY_I_INTEREST_YOU_IN_FOLLOWING_ITEMS);
            verbalize("Hello, adventurer! May I interest you in the following %s?", itembuf);
        }
        else
            pline("%s shows you %s merchandise.", Monnam(mtmp), mhis(mtmp));
    }


    /* Now generate the menu */
    int pick_count = 0;
    int buy_count = 0;
    if ((pick_count = select_menu(win, PICK_ANY, &pick_list)) > 0)
    {
        boolean itemized = TRUE;
        if (pick_count > 1)
            itemized = (yn_query("Itemized billing?") == 'y');

        for (int i = 0; i < pick_count; i++)
        {
            struct obj* item_to_buy = pick_list[i].item.a_obj;
            long quan = pick_list[i].count == -1 ? item_to_buy->quan : min(item_to_buy->quan, pick_list[i].count);
            if (item_to_buy && quan > 0)
            {
                long item_cost = quan * get_cost_of_monster_item(item_to_buy, mtmp);
                
                long umoney = money_cnt(invent);
                char qbuf[QBUFSZ];
                boolean bought = FALSE;
                char ibuf[QBUFSZ];
                if(quan == 1)
                    Sprintf(ibuf, "%s", an(cxname_singular(item_to_buy)));
                else
                    Sprintf(ibuf, "%ld %s", quan, cxname(item_to_buy));

                if(itemized)
                {
                    boolean doforbreak = FALSE;
                    if (item_cost)
                    {
                        Sprintf(qbuf, "Buy %s for %ld %s?", ibuf, item_cost, currency(item_cost));
                    }
                    else
                        Sprintf(qbuf, "Take %s for no charge?", ibuf);

                    switch (ynq(qbuf)) 
                    {
                    default:
                    case 'q':
                        doforbreak = TRUE;
                        break;
                    case 'n':
                        break;
                    case 'y':
                        if (umoney < (long)item_cost) {
                            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                            You("don't have enough money for that!");
                            break; /* switch break */
                        }
                        bought = TRUE;
                        break;
                    }
                    if (doforbreak)
                        break;
                }
                else
                {
                    if (umoney < (long)item_cost) {
                        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                        You("don't have enough money for %s!", cxname(item_to_buy));
                        break; /* for break */
                    }
                    bought = TRUE;
                }

                if (bought)
                {
                    play_sfx_sound(SFX_TRANSACT_SINGLE_ITEM);
                    if (item_cost)
                        Sprintf(qbuf, "bought %s for %ld %s.", ibuf, item_cost, currency(item_cost));
                    else
                        Sprintf(qbuf, "took %s for no charge.", ibuf);

                    You("%s", qbuf);

                    money2mon(mtmp, (long)item_cost);
                    
                    if (quan < item_to_buy->quan)
                        item_to_buy = splitobj(item_to_buy, quan);
                    
                    obj_extract_self(item_to_buy);
                    hold_another_object(item_to_buy, "Oops!  %s out of your grasp!",
                        The(aobjnam(item_to_buy, "slip")),
                        (const char*)0);
                    buy_count++;
                }
            }
        }

        free((genericptr_t)pick_list);
        destroy_nhwindow(win);

        if (is_peaceful(mtmp) && buy_count > 0) 
        {
            play_sfx_sound(SFX_BUY_FROM_NPC);
            if (!Deaf && !mtmp->isshk && (is_undead(mtmp->data) || is_demon(mtmp->data) || (mtmp->data->maligntyp < 0 && mtmp->data->difficulty > 10)))
                verbalize("Use your purchase well!");
            else if (!Deaf && (is_speaking_monster(mtmp->data) || (mtmp->isshk && !muteshk(mtmp))))
            {
                play_monster_item_trading_line(mtmp, MONSTER_ITEM_TRADING_LINE_TRADING_THANK_YOU_FOR_YOUR_PURCHASE);
                verbalize("Thank you for your purchase!");
            }
            else
                pline("%s nods appreciatively at you for the purchase!", Monnam(mtmp));
        }

        if (buy_count > 0)
            return 1;
        else
            return 0;
    }
    else
        return 0;

}


STATIC_OVL int
do_chat_pet_takeitems(mtmp)
struct monst* mtmp;
{
    int item_count = 0;

    menu_item* pick_list = (menu_item*)0;
    winid win;
    anything any;

    any = zeroany;
    win = create_nhwindow(NHW_MENU);
    start_menu(win);


    static char def_srt_order[MAX_OBJECT_CLASSES] = {
    COIN_CLASS, AMULET_CLASS, MISCELLANEOUS_CLASS, RING_CLASS, WAND_CLASS, POTION_CLASS,
    SCROLL_CLASS, SPBOOK_CLASS, GEM_CLASS, FOOD_CLASS, REAGENT_CLASS, TOOL_CLASS,
    WEAPON_CLASS, ARMOR_CLASS, ROCK_CLASS, BALL_CLASS, CHAIN_CLASS, 0,
    };

    const char* classorder = flags.sortpack ? flags.inv_order : def_srt_order;
    boolean classhasitems[MAX_OBJECT_CLASSES] = { 0 };

    for (struct obj* otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
    {
        if (otmp->oclass > ILLOBJ_CLASS)
            classhasitems[(int)otmp->oclass] = TRUE;
    }

    for (int i = 0; i < MAX_OBJECT_CLASSES; i++)
    {
        char oclass = classorder[i];
        boolean madeheader = FALSE;

        if (flags.sortpack && !classhasitems[(int)oclass])
            continue;

        for (struct obj* otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
        {
            if ((!flags.sortpack || (flags.sortpack && otmp->oclass == oclass)) && otmp->owornmask == 0)
            {
                if (flags.sortpack && !madeheader)
                {
                    madeheader = TRUE;
                    any = zeroany;

                    add_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings,
                        get_class_name(oclass), MENU_UNSELECTED);
                }

                any = zeroany;
                char itembuf[BUFSIZ] = "";
                Sprintf(itembuf, "%s", doname(otmp));

                any.a_obj = otmp;
                char let = 'a' + item_count;
                char accel = def_oc_syms[(int)otmp->oclass].sym;

                add_menu(win, NO_GLYPH, &any,
                    let, accel, ATR_NONE,
                    itembuf, MENU_UNSELECTED);

                item_count++;

            }
        }
        if (!flags.sortpack)
            break;
    }

    /* Finish the menu */
    end_menu(win, "What do you want to take?");

    if (item_count <= 0)
    {
        pline("%s doesn't have any items.", Monnam(mtmp));
        destroy_nhwindow(win);
        return 0;
    }

    /* Now generate the menu */
    int pick_count = 0;
    int take_count = 0;
    if ((pick_count = select_menu(win, PICK_ANY, &pick_list)) > 0)
    {
        for (int i = 0; i < pick_count; i++)
        {
            struct obj* item_to_take = pick_list[i].item.a_obj;
            if (item_to_take)
            {
                if ((objects[item_to_take->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && item_to_take->cursed) 
                {
                    You("%s not leave %s!", Tobjnam(item_to_take, "do"), mon_nam(mtmp));
                }
                else
                {
                    if (item_to_take->quan > 1 && pick_list[i].count > 0 && pick_list[i].count < item_to_take->quan)
                        item_to_take = splitobj(item_to_take, pick_list[i].count);

                    obj_extract_self(item_to_take);

                    You("took %s from %s.", doname(item_to_take), mon_nam(mtmp));

                    hold_another_object(item_to_take, "Oops!  %s out of your grasp!",
                        The(aobjnam(item_to_take, "slip")),
                        (const char*)0);
                    take_count++;
                }
            }
        }

        free((genericptr_t)pick_list);
        destroy_nhwindow(win);

        if (take_count > 0)
            return 1;
        else
            return 0;
    }
    else
        return 0;

}

/* Returns the price of an arbitrary item per one item */
long
get_cost_of_monster_item(obj, mtmp)
register struct obj* obj;
register struct monst* mtmp;
{            
    struct obj* top;
    long cost = 0L;

    for (top = obj; top->where == OBJ_CONTAINED; top = top->ocontainer)
        continue;

    cost = get_cost(obj, mtmp);

    if (Has_contents(obj))
        cost += m_contained_cost(obj, mtmp);

    return cost;
}


long
m_contained_cost(obj, mtmp)
struct obj* obj;
struct monst* mtmp;
{
    long price = 0;
    register struct obj* otmp, * top;

    for (top = obj; top->where == OBJ_CONTAINED; top = top->ocontainer)
        continue;

    /* price of contained objects; "top" container handled by caller */
    for (otmp = obj->cobj; otmp; otmp = otmp->nobj) {
        price += get_cost(otmp, mtmp) * otmp->quan;

        if (Has_contents(otmp))
            price += m_contained_cost(otmp, mtmp);
    }

    return price;
}



int
count_sellable_items(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->minvent)
        return 0;

    int cnt = 0;
    for (struct obj* otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
    {
        if (m_sellable_item(otmp, mtmp))
            cnt++;
    }
    return cnt;
}

struct obj*
get_first_sellable_item(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->minvent)
        return (struct obj*)0;

    for (struct obj* otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
    {
        if (m_sellable_item(otmp, mtmp))
        {
            return otmp;
        }
    }
    return (struct obj*)0;
}


boolean
m_sellable_item(otmp, mtmp)
struct obj* otmp;
struct monst* mtmp;
{
    if (!otmp || !mtmp)
        return FALSE;

    if (!otmp->owornmask 
        && otmp->oclass != COIN_CLASS
        && ((otmp->speflags & SPEFLAGS_GRABBED_FROM_YOU) ||
               (otmp->oclass != WEAPON_CLASS /* monsters do not currently sell their weapons */
            && otmp->oclass != ROCK_CLASS /* or giants their boulders */
            && !(is_pick(otmp) && needspick(mtmp->data)) /* or dwarves their picks */
            && !((mtmp->data->geno & G_UNIQ)) /* or unique monsters */
            && !(is_dwarvish_obj(otmp) && is_dwarf(mtmp->data)) /* or dwarves any other of their items */
            && !mtmp->isshk
               )
           )
        && !(
            (otmp->cursed && (objects[otmp->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED))
            || (otmp->otyp == AMULET_OF_YENDOR && (mtmp->data->mflags3 & M3_WANTSAMUL))
            || (otmp->otyp == BELL_OF_OPENING && (mtmp->data->mflags3 & M3_WANTSBELL))
            || (otmp->otyp == SPE_BOOK_OF_THE_DEAD && (mtmp->data->mflags3 & M3_WANTSBOOK))
            || (otmp->otyp == CANDELABRUM_OF_INVOCATION && (mtmp->data->mflags3 & M3_WANTSCAND))
            || (is_quest_artifact(otmp) && (mtmp->data->mflags3 & M3_WANTSARTI))
            )
        )
        return TRUE;
    else
        return FALSE;
}




STATIC_OVL int
do_chat_oracle_consult(mtmp)
struct monst* mtmp;
{
    if (!m_general_talk_check(mtmp, "consulatations") || !m_speak_check(mtmp))
        return 0;

    return doconsult(mtmp);
}

STATIC_OVL int
do_chat_oracle_identify(mtmp)
struct monst* mtmp;
{
    if (!m_general_talk_check(mtmp, "identification") || !m_speak_check(mtmp))
        return 0;

    return do_oracle_identify(mtmp);
}

STATIC_OVL int
do_chat_oracle_enlightenment(mtmp)
struct monst* mtmp;
{
    if (!m_general_talk_check(mtmp, "providing any enlightenment") || !m_speak_check(mtmp))
        return 0;

    return do_oracle_enlightenment(mtmp);
}


STATIC_OVL int
do_chat_priest_blesscurse(mtmp)
struct monst* mtmp;
{

    long umoney = money_cnt(invent);
    int u_pay, 
        bless_cost = max(1, (int)((300 + 15 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))),
        curse_cost = max(1, (int)((300 + 15 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    int priest_action = 0;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to bless an item? (%d %s)", bless_cost, currency((long)bless_cost));
    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
            if (umoney < (long)bless_cost) {
                play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                You("don't have enough money for that!");
                return 0;
            }
            u_pay = bless_cost;
            priest_action = 1;
            break;
        break;
    case 'n':
        Sprintf(qbuf, "Then would you like to curse one? (%d %s)",
            curse_cost, currency((long)curse_cost));
        if (yn_query(qbuf) != 'y')
            return 0;
        if (umoney < (long)curse_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = curse_cost;
        priest_action = 2;
        break;
    }


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
    pseudo->speflags = SPEFLAGS_SERVICED_SPELL;
    boolean effect_happened = 0;
    (void)seffects(pseudo, &effect_happened);
    obfree(pseudo, (struct obj*)0);

    if (effect_happened)
    {
        money2mon(mtmp, (long)u_pay);
        context.botl = 1;
    }

    /* gnostic handled in seffects */

    return 1;
}


STATIC_OVL int
do_chat_priest_normal_healing(mtmp)
struct monst* mtmp;
{

    long umoney = money_cnt(invent);
    int u_pay, extrahealing_cost = max(1, (int)(50 * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to have a standard healing? (%d %s)", extrahealing_cost, currency((long)extrahealing_cost));
    switch (ynq(qbuf)) {
    default:
    case 'n':
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)extrahealing_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = extrahealing_cost;
        break;
    }
    money2mon(mtmp, (long)u_pay);
    context.botl = 1;

    int otyp = POT_EXTRA_HEALING;

    struct obj* pseudo = mksobj(otyp, FALSE, FALSE, FALSE);
    pseudo->blessed = pseudo->cursed = 0;
    pseudo->quan = 20L; /* do not let useup get it */
    pseudo->speflags = SPEFLAGS_SERVICED_SPELL;
    peffects(pseudo);
    obfree(pseudo, (struct obj*)0);
    u.uconduct.gnostic++;

    return 1;
}

STATIC_OVL int
do_chat_priest_full_healing(mtmp)
struct monst* mtmp;
{

    long umoney = money_cnt(invent);
    int u_pay, fullhealing_cost = max(1, (int)((250 + 5 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to have a full healing? (%d %s)", fullhealing_cost, currency((long)fullhealing_cost));
    switch (ynq(qbuf)) {
    default:
    case 'n':
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)fullhealing_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = fullhealing_cost;
        break;
    }
    money2mon(mtmp, (long)u_pay);
    context.botl = 1;

    int otyp = POT_FULL_HEALING;

    struct obj* pseudo = mksobj(otyp, FALSE, FALSE, FALSE);
    pseudo->blessed = pseudo->cursed = 0;
    pseudo->quan = 20L; /* do not let useup get it */
    pseudo->speflags = SPEFLAGS_SERVICED_SPELL;
    peffects(pseudo);
    obfree(pseudo, (struct obj*)0);
    u.uconduct.gnostic++;

    return 1;
}

STATIC_OVL int
do_chat_priest_cure_sickness(mtmp)
struct monst* mtmp;
{

    long umoney = money_cnt(invent);
    int u_pay, cure_sickness_cost = max(1, (int)(25 * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to have your sickness cured? (%d %s)", cure_sickness_cost, currency((long)cure_sickness_cost));
    switch (ynq(qbuf)) {
    default:
    case 'n':
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)cure_sickness_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = cure_sickness_cost;
        break;
    }
    money2mon(mtmp, (long)u_pay);
    context.botl = 1;

    int otyp = SPE_CURE_SICKNESS;

    struct obj* pseudo = mksobj(otyp, FALSE, FALSE, FALSE);
    pseudo->blessed = pseudo->cursed = 0;
    pseudo->quan = 20L; /* do not let useup get it */
    pseudo->speflags = SPEFLAGS_SERVICED_SPELL;
    zapyourself(pseudo, TRUE);
    obfree(pseudo, (struct obj*)0);
    u.uconduct.gnostic++;

    return 1;
}

STATIC_OVL int
do_chat_priest_chat(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    //priest_talk(mtmp);

    long umoney = money_cnt(invent);
    int u_pay,
        major_cost = max(1, (int)((2500 + 150 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))),
        minor_cost = max(1, (int)((500 + 25 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    int priest_action = 0;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "taking your donation") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to make a major contribution for the temple? (%d %s)", major_cost, currency((long)major_cost));
    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)major_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = major_cost;
        priest_action = 1;
        break;
        break;
    case 'n':
        Sprintf(qbuf, "Then would you like to make a minor donation instead? (%d %s)",
            minor_cost, currency((long)minor_cost));
        if (yn_query(qbuf) != 'y')
            return 0;
        if (umoney < (long)minor_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = minor_cost;
        priest_action = 2;
        break;
    }


    money2mon(mtmp, (long)u_pay);
    context.botl = 1;

    boolean coaligned = p_coaligned(mtmp);
    boolean strayed = (u.ualign.record < 0);

    if (priest_action == 2)
    {
        verbalize("Thou art indeed a pious individual.");
        if (coaligned && u.ualign.record <= ALGN_SINNED)
        {
            play_sfx_sound(SFX_ALTAR_ADD_ALIGNMENT);
            adjalign(1);
        }
        play_sfx_sound(SFX_PRAY_BLESS_WATER);
        verbalize("I bestow upon thee a blessing.");
        incr_itimeout(&HClairvoyant, rn1(500, 500));
    }
    else if (priest_action == 1
        /* u.ublessed is only active when Protection is
           enabled via something other than worn gear
           (theft by gremlin clears the intrinsic but not
           its former magnitude, making it recoverable) */
        && (u.ublessed == 0
            || (u.ublessed < 12
                && (u.ublessed < 8 || !rn2(u.ublessed)))))
    {
        play_sfx_sound(SFX_ALTAR_GIFT);
        verbalize("Thy devotion has been rewarded.");
        if (u.ublessed == 0)
            u.ublessed = rnd(3);
        else
            u.ublessed++;
    }
    else
    {
        verbalize("Thy selfless generosity is deeply appreciated.");
        if (coaligned)
        {
            if (strayed && (moves - u.ucleansed) > 5000L)
            {
                play_sfx_sound(SFX_ALTAR_ABSOLVED);
                u.ualign.record = 0; /* cleanse thee */
                u.ucleansed = moves;
            }
            else
            {
                play_sfx_sound(SFX_ALTAR_ADD_ALIGNMENT);
                adjalign(2);
            }
        }
    }

    find_ac();
    find_mc();

    return 1;
}

STATIC_OVL int
do_chat_priest_divination(mtmp)
struct monst* mtmp;
{
    long umoney = money_cnt(invent);
    int u_pay, divination_cost = max(1, (int)(25 * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to see your fortune? (%d %s)", divination_cost, currency((long)divination_cost));
    switch (ynq(qbuf)) {
    default:
    case 'n':
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)divination_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = divination_cost;
        break;
    }
    money2mon(mtmp, (long)u_pay);
    context.botl = 1;

    u.uconduct.gnostic++;

    verbalize("Very well, then. Let's see what your fortune is like.");

    if (can_pray(FALSE))
        verbalize("First, I see that you can safely pray.");
    else
    {
        verbalize("First, you should know that you cannot safely pray.");

        if (u.ugangr)
            verbalize("I see that %s is %sangry with you.", u_gname(), u.ugangr > 6 ? "extremely " : u.ugangr > 3 ? "very " : "");

        if (u.uprayer_timeout > 0)
        {
            verbalize("For your prayer conduct, the number %d appears before me.", u.uprayer_timeout / 10 + 1);

            if (u.uprayer_timeout > 300)
                verbalize("I can see that %s is quite tired of your constant whining.", u_gname());

            verbalize("Thus, %s wait %sbefore bothering %s again.",
                u.uprayer_timeout >= 50 ? "it would be wise to" : "you must",
                u.uprayer_timeout < 50 ? "a little longer " : u.uprayer_timeout > 200 ? "a long time " : "",
                u_gname());
        }
    }

    if (Luck < 0)
        verbalize("For your fortune, I see a number of %d. That is not good, for it is %s unlucky number.",
            abs(Luck), abs(Luck) >= 10 ? "an extremely" : abs(Luck) >= 5 ? "a very" : "an");
    else if(Luck > 0)
        verbalize("For your fortune, I see a number of %d. That is good, for it is %s lucky number.",
            Luck, Luck >= 10 ? "an extremely" : Luck >= 5 ? "a very" : "a");
    else
        verbalize("For your fortune, my vision is neutral.");

    verbalize("That's all for now. Thank you for your interest in divine matters.");

    return 1;
}




STATIC_OVL int
do_chat_shk_payitems(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    if (mtmp->isshk)
        return dopay();
    else
    {
        pline("%s is not a shopkeeper.", Monnam(mtmp));
        return 0;
    }
}


STATIC_OVL int
do_chat_shk_pricequote(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
    {
        /* Do nothing, not used */
    }

    struct obj* otmp = shop_object(u.ux, u.uy);
    if(otmp)
        price_quote(otmp);

    return 1;
}

STATIC_OVL int
do_chat_shk_chat(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    shk_chat(mtmp);
    return 1;
}

STATIC_OVL int
do_chat_shk_identify(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->mextra || !mtmp->mextra->eshk)
        return 0;

    long umoney;
    int u_pay;
    int minor_id_cost = max(1, (int)((ESHK(mtmp)->shoptype == SHOPBASE ? 150 + 10 * (double)u.ulevel : 75 + 5 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney) {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to identify %s? (%d %s)", shtypes[ESHK(mtmp)->shoptype - SHOPBASE].identified_item_description, minor_id_cost, currency((long)minor_id_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)minor_id_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = minor_id_cost;
        break;
    }

    context.shop_identify_type = ESHK(mtmp)->shoptype - SHOPBASE + 1; // shtypes[ESHK(mtmp)->shoptype - SHOPBASE].symb;

    int res = identify_pack(1, FALSE);

    context.shop_identify_type = 0;

    if (res)
    {
        money2mon(mtmp, (long)u_pay);
        context.botl = 1;
    }

    return 1; 
}

boolean
is_shop_item_type(otmp, shtype_index)
struct obj* otmp;
int shtype_index;
{
    if (!otmp)
        return FALSE;

    for (int i = 0; i < SIZE(shtypes[shtype_index].iprobs); i++)
    {
        if (shtypes[shtype_index].iprobs[i].iprob == 0)
            break;

        if (shtypes[shtype_index].iprobs[i].itype == RANDOM_CLASS)
        {
            return TRUE;
        }
        else if (shtypes[shtype_index].iprobs[i].itype > 0)
        {
            if (shtypes[shtype_index].iprobs[i].itype == VEGETARIAN_CLASS)
            {
                if (veggy_item(otmp, 0))
                    return TRUE;
            }
            else if (otmp->oclass == shtypes[shtype_index].iprobs[i].itype)
                return TRUE;
        }
        else if (shtypes[shtype_index].iprobs[i].itype < 0)
        {
            if (otmp->otyp == -shtypes[shtype_index].iprobs[i].itype)
                return TRUE;
        }
    }
    return FALSE;
}


STATIC_OVL int
do_chat_shk_reconciliation(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isshk || !mtmp->mextra || !ESHK(mtmp))
        return 0;

    long umoney;
    long u_pay;
    long reconcile_cost = max(1, (int)((1000 + u.ulevel * 100 + (mtmp->mrevived ? u.ulevel * 100 : 0) + max(0, ESHK(mtmp)->robbed + ESHK(mtmp)->debit - ESHK(mtmp)->credit)) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];
    
    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) {
        There("is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;

    play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_YOU_NEED_TO_PAY_LOT_OF_GOLD);
    if(iflags.using_gui_sounds)
        Sprintf(qbuf, "\"You need to pay a lot of gold in compensation.\" (%ld %s in fact!)  Agree?", reconcile_cost, currency(reconcile_cost));
    else
        Sprintf(qbuf, "\"You need to pay %ld %s in compensation. Agree?\"", reconcile_cost, currency(reconcile_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    context.botl = 1;

    make_happy_shk(mtmp, FALSE);
    long costapplyingtodebit = max(0, min(reconcile_cost - 1000, ESHK(mtmp)->debit));
    ESHK(mtmp)->debit -= costapplyingtodebit;

    if (is_peaceful(mtmp))
    {
        play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_THATS_A_DEAL);
        verbalize("That's a deal. Be more careful next time.");
    }
    else
    {
        play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_ON_SECOND_THOUGHT);
        verbalize("On second thought, maybe you should hang for your crimes anyway.");
    }

    return 1;
}


STATIC_OVL int
do_chat_smith_reconciliation(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long umoney;
    long u_pay;
    long reconcile_cost = max(1, (int)((1000 + u.ulevel * 100 + (mtmp->mrevived ? u.ulevel * 100 : 0)) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) {
        There("is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;


    Sprintf(qbuf, "\"You need to pay %ld %s in compensation. Agree?\"", reconcile_cost, currency(reconcile_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    context.botl = 1;

    mtmp->mpeaceful = 1;
    newsym(mtmp->mx, mtmp->my);

    if (is_peaceful(mtmp))
        verbalize("That's a deal. Be more careful next time.");
    else
        verbalize("On second thought, maybe you should hang for your crimes anyway.");

    return 1;
}

STATIC_OVL int
do_chat_smith_enchant_armor(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    int cost = max(1, (int)((1000 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));    
    return spell_service_query(mtmp, SPE_ENCHANT_ARMOR, 0, "enchant an armor", cost, "enchanting an armor");
}

STATIC_OVL int
do_chat_smith_enchant_weapon(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    int cost = max(1, (int)((1000 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SPE_ENCHANT_WEAPON, 0, "enchant a weapon", cost, "enchanting a weapon");
}

STATIC_OVL int
do_chat_smith_repair_armor(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(1, (int)((500 + 25 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return general_service_query(mtmp, repair_armor_func, "repair an armor", cost, "repairing an armor");
}

STATIC_OVL int
do_chat_smith_repair_weapon(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(1, (int)((500 + 25 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return general_service_query(mtmp, repair_weapon_func, "repair a weapon", cost, "repairing a weapon");
}


STATIC_OVL int
do_chat_smith_protect_armor(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    int cost = max(1, (int)((2000 + 100 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SPE_PROTECT_ARMOR, 0, "protect an armor", cost, "protecting an armor");
}

STATIC_OVL int
do_chat_smith_protect_weapon(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    int cost = max(1, (int)((2000 + 100 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SPE_PROTECT_WEAPON, 0, "protect a weapon", cost, "protecting a weapon");
}

STATIC_OVL int
do_chat_smith_refill_lantern(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(5, (int)((max(objects[BRASS_LANTERN].oc_cost, objects[POT_OIL].oc_cost)) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return general_service_query(mtmp, refill_lantern_func, "refill a lamp or lantern", cost, "refilling a lamp or lantern");
}

STATIC_OVL int
do_chat_smith_forge_special_armor(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    menu_item* pick_list = (menu_item*)0;
    winid win;
    anything any;

    any = zeroany;
    win = create_nhwindow(NHW_MENU);
    start_menu(win);


    any = zeroany;
    any.a_char = 1;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE,
        "Forge dragon scales into a dragon scale mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 2;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE,
        "Forge an adamantium full plate mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 3;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE,
        "Forge a mithril full plate mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 4;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE,
        "Forge an orichalcum full plate mail", MENU_UNSELECTED);

    /* Finish the menu */
    end_menu(win, "Which type of armor do you want to forge?");

    int i = 0;
    /* Now generate the menu */
    if (select_menu(win, PICK_ONE, &pick_list) > 0)
    {
        i = pick_list->item.a_char;
        free((genericptr_t)pick_list);
    }
    destroy_nhwindow(win);

    if (i < 1)
        return 0;

    int cost = 0;

    switch(i)
    {
    case 1:
        cost = max(1, (int)((1000 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query(mtmp, forge_dragon_scale_mail_func, "forge a dragon scale mail", cost, "forging a dragon scale mail");
        break;
    case 2:
        cost = max(1, (int)((500 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_components(mtmp, forge_adamantium_full_plate_mail_func, "forge an adamantium full plate mail", cost, "forging any armor", "8 nuggets of adamantium ore");
        break;
    case 3:
        cost = max(1, (int)((500 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_components(mtmp, forge_mithril_full_plate_mail_func, "forge a mithril full plate mail", cost, "forging any armor", "8 nuggets of mithril ore");
        break;
    case 4:
        cost = max(1, (int)((500 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_components(mtmp, forge_orichalcum_full_plate_mail_func, "forge an orichalcum full plate mail", cost, "forging any armor", "8 nuggets of orichalcum ore");
        break;
    default:
        pline1(Never_mind);
        break;
    }

    return 0;
}

STATIC_OVL int
do_chat_smith_identify(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->mextra || !mtmp->mextra->esmi)
        return 0;

    long umoney;
    int u_pay;
    int minor_id_cost = max(1, (int)((double)(75 + 5 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney) {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to identify a weapon or armor? (%d %s)", minor_id_cost, currency((long)minor_id_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)minor_id_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = minor_id_cost;
        break;
    }

    context.npc_identify_type = -1; /* Smith */

    int res = identify_pack(1, FALSE);

    context.npc_identify_type = 0;

    if (res)
    {
        money2mon(mtmp, (long)u_pay);
        context.botl = 1;
    }

    return 1;
}

STATIC_OVL boolean
maybe_ore(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return is_ore(otmp);
}

STATIC_OVL boolean
maybe_adamantium_ore(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return (otmp->otyp == NUGGET_OF_ADAMANTIUM_ORE);
}

STATIC_OVL boolean
maybe_mithril_ore(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return (otmp->otyp == NUGGET_OF_MITHRIL_ORE);
}

STATIC_OVL boolean
maybe_orichalcum_ore(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return (otmp->otyp == NUGGET_OF_ORICHALCUM_ORE);
}

STATIC_OVL boolean
maybe_dragon_scales(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return !!is_dragon_scales(otmp);
}

STATIC_OVL int
do_chat_smith_sell_ore(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_esmi(mtmp))
        return 0;

    const char sell_types[] = { ALLOW_COUNT, GEM_CLASS, 0 };
    int result, i = (invent) ? 0 : (SIZE(sell_types) - 1);

    result = sell_to_npc(getobj_ex(&sell_types[i], "sell", 3, "", maybe_ore), mtmp);

    if (result)
    {
        /* Do nothing at the moment */
    }

    return 1;
}

STATIC_OVL int
do_chat_npc_branch_portal(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    int service_cost = max(1, (int)((500 + 25 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));

    long umoney = money_cnt(invent);
    int u_pay;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "opening a branch portal") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to %s? (%d %s)", "open a branch portal", service_cost, currency((long)service_cost));
    switch (ynq(qbuf))
    {
    default:
    case 'n':
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)service_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = service_cost;
        break;
    }

    money2mon(mtmp, (long)u_pay);
    pline("%s opens a branch portal for you.", Monnam(mtmp));
    int portal_res = create_portal();
    if (!portal_res)
    {
        money2u(mtmp, (long)u_pay);
        pline("Nevermind.");
        return 0;
    }
    else
        mtmp = 0;

    /* Make sure mtmp is not used anywhere anymore; it is not invalid */
    return 2; 
}

boolean
is_npc_item_identification_type(otmp, npc_identification_type_index)
struct obj* otmp;
int npc_identification_type_index;
{
    if (!otmp)
        return FALSE;

    if (npc_identification_type_index == 1)
    {
        if (otmp->oclass == GEM_CLASS)
            return TRUE;
        else
            return FALSE;
    }
    else if (npc_identification_type_index == 2)
    {
        if (otmp->oclass == RING_CLASS || otmp->oclass == MISCELLANEOUS_CLASS || objects[otmp->otyp].oc_charged > CHARGED_NOT_CHARGED)
            return TRUE;
        else
            return FALSE;
    }
    else if (npc_identification_type_index == 3)
    {
        if (otmp->oclass == GEM_CLASS || objects[otmp->otyp].oc_charged > CHARGED_NOT_CHARGED)
            return TRUE;
        else
            return FALSE;
    }

    return TRUE;
}


STATIC_OVL int
do_chat_npc_reconciliation(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    long umoney;
    long u_pay;
    long reconcile_cost = max(1, (int)((1000 + u.ulevel * 100 + (mtmp->mrevived ? u.ulevel * 100 : 0)) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) {
        There("is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;


    Sprintf(qbuf, "\"You need to pay %ld %s in compensation. Agree?\"", reconcile_cost, currency(reconcile_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    context.botl = 1;

    mtmp->mpeaceful = 1;
    newsym(mtmp->mx, mtmp->my);

    play_sfx_sound(SFX_BUY_FROM_NPC);
    if (is_peaceful(mtmp))
        verbalize("That's a deal. Be more careful next time.");
    else
        verbalize("On second thought, maybe you should hang for your crimes anyway.");

    return 1;
}

STATIC_OVL int
do_chat_npc_enchant_accessory(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    int cost = max(1, (int)((1000 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SCR_ENCHANT_ACCESSORY, 0, "enchant an accessory", cost, "enchanting an accessory");
}

STATIC_OVL int
do_chat_npc_recharge(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    int cost = max(1, (int)((1200 + 60 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SCR_CHARGING, 0, "recharge an item", cost, "recharging an item");
}


STATIC_OVL int
do_chat_npc_blessed_recharge(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    int cost = max(1, (int)((4000 + 200 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SCR_CHARGING, 1, "fully recharge an item", cost, "fully recharging an item");
}

STATIC_OVL int
do_chat_npc_teach_spells(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    int spell_otyps[MAX_SPECIAL_TEACH_SPELLS + 3 + 1] = { 0 };
    int teach_num = 0;
    if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_TEACH_SPELL_LIGHTNING_BOLT)
    {
        spell_otyps[teach_num] = SPE_LIGHTNING_BOLT;
        teach_num++;
    }
    if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_TEACH_SPELL_CONE_OF_COLD)
    {
        spell_otyps[teach_num] = SPE_CONE_OF_COLD;
        teach_num++;
    }
    if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_TEACH_SPELL_FORCE_BOLT)
    {
        spell_otyps[teach_num] = SPE_FORCE_BOLT;
        teach_num++;
    }
    if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_TEACH_SPECIAL_SPELLS)
    {
        for (int i = 0; i < MAX_SPECIAL_TEACH_SPELLS && ENPC(mtmp)->special_teach_spells[i] > STRANGE_OBJECT; i++)
        {
            spell_otyps[teach_num] = ENPC(mtmp)->special_teach_spells[i];
            teach_num++;
        }
    }
    spell_otyps[teach_num] = 0;
    return spell_teaching(mtmp, spell_otyps);
}

STATIC_OVL int
do_chat_priest_teach_spells(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->ispriest || !mtmp->mextra || !EPRI(mtmp))
        return 0;

    int spell_otyps[10] = { SPE_EXTRA_HEALING, SPE_GREATER_HEALING, 0, 0, 0, 0, 0, 0, 0, 0 };

    return spell_teaching(mtmp, spell_otyps);
}


STATIC_OVL int
do_chat_watchman_reconciliation(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    long umoney;
    long u_pay;
    long reconcile_cost = max(1, (int)(500 * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) {
        There("is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;
    else if (mvitals[PM_WATCHMAN].died > 0 || mvitals[PM_WATCH_CAPTAIN].died > 0) {
        verbalize("You will hang for your crimes, scum!");
        return 0;
    }
    else if (mtmp->mhp < (3 * mtmp->mhpmax) / 4) {
        pline("%s is in no mood for talking.", Monnam(mtmp));
        return 0;
    }

    Sprintf(qbuf, "\"We can drop the case for %ld %s. Agree?\"", reconcile_cost, currency(reconcile_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    context.botl = 1;

    pacify_guards();

    play_sfx_sound(SFX_BUY_FROM_NPC);
    if(is_peaceful(mtmp))
        verbalize("Fine, it's alright now. Be more careful next time.");
    else
        verbalize("On second thought, maybe I'll hang you anyway.");

    return 1; 
}


STATIC_OVL int
do_chat_npc_identify_gems_and_stones(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_enpc(mtmp))
        return 0;

    long umoney;
    int u_pay;
    int minor_id_cost = max(1, (int)((double)(100 + 10 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney) {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to identify gems and stones? (%d %s)", minor_id_cost, currency((long)minor_id_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)minor_id_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = minor_id_cost;
        break;
    }

    context.npc_identify_type = 1;

    int res = identify_pack(1, FALSE);

    context.npc_identify_type = 0;

    if (res)
    {
        money2mon(mtmp, (long)u_pay);
        context.botl = 1;
    }

    return 1;
}


STATIC_OVL int
do_chat_npc_sell_gems_and_stones(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_enpc(mtmp))
        return 0;

    const char sell_types[] = { ALLOW_COUNT, GEM_CLASS, 0 };
    int result, i = (invent) ? 0 : (SIZE(sell_types) - 1);

    result = sell_to_npc(getobj(&sell_types[i], "sell", 3, ""), mtmp);

    if (result)
    {
        /* Do nothing at the moment */
    }

    return 1;
}

STATIC_OVL boolean
maybe_dilithium_crystal(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;
    if(objects[otmp->otyp].oc_name_known)
        return (otmp->otyp == DILITHIUM_CRYSTAL);
    else
        return (otmp->oclass == GEM_CLASS && !objects[otmp->otyp].oc_name_known && objects[otmp->otyp].oc_color == objects[DILITHIUM_CRYSTAL].oc_color);

}

STATIC_OVL int
do_chat_npc_sell_dilithium_crystals(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_enpc(mtmp))
        return 0;

    const char sell_types[] = { ALLOW_COUNT, GEM_CLASS, 0 };
    int result, i = (invent) ? 0 : (SIZE(sell_types) - 1);

    result = sell_to_npc(getobj_ex(&sell_types[i], "sell", 3, "", maybe_dilithium_crystal), mtmp);

    if (result)
    {
        /* Do nothing at the moment */
    }

    return 1;
}

STATIC_OVL int
do_chat_npc_sell_spellbooks(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_enpc(mtmp))
        return 0;

    const char sell_types[] = { ALLOW_COUNT, SPBOOK_CLASS, 0 };
    int result, i = (invent) ? 0 : (SIZE(sell_types) - 1);

    result = sell_to_npc(getobj(&sell_types[i], "sell", 3, ""), mtmp);

    if (result)
    {
        /* Do nothing at the moment */
    }

    return 1;
}

STATIC_OVL int
do_chat_npc_identify_accessories_and_charged_items(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_enpc(mtmp))
        return 0;

    long umoney;
    int u_pay;
    int minor_id_cost = max(1, (int)((double)(100 + 10 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney) {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to identify an accessory or a charged item? (%d %s)", minor_id_cost, currency((long)minor_id_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)minor_id_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = minor_id_cost;
        break;
    }

    context.npc_identify_type = 2;

    int res = identify_pack(1, FALSE);

    context.npc_identify_type = 0;

    if (res)
    {
        money2mon(mtmp, (long)u_pay);
        context.botl = 1;
    }

    return 1;
}

STATIC_OVL int
do_chat_npc_identify_gems_stones_and_charged_items(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_enpc(mtmp))
        return 0;

    long umoney;
    int u_pay;
    int minor_id_cost = max(1, (int)((double)(100 + 10 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney) {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to identify a gem, stone or a charged item? (%d %s)", minor_id_cost, currency((long)minor_id_cost));

    switch (ynq(qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)minor_id_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = minor_id_cost;
        break;
    }

    context.npc_identify_type = 3;

    int res = identify_pack(1, FALSE);

    context.npc_identify_type = 0;

    if (res)
    {
        money2mon(mtmp, (long)u_pay);
        context.botl = 1;
    }

    return 1;
}


STATIC_OVL int
sell_to_npc(obj, mtmp)
struct obj* obj;
struct monst* mtmp;
{
    if (!obj)
        return 0;

    struct obj *otmp = 0;
    int res = 0;
    boolean is_smith = mtmp->issmith && has_esmi(mtmp);
    if (!mtmp || !(has_enpc(mtmp) || is_smith))
    {
        res = 0;
        goto merge_obj_back;
    }

    if (!canletgo(obj, "let go"))
    {
        res = 0;
        goto merge_obj_back;
    }

    if (obj == uwep) 
    {
        if (welded(uwep, &youmonst)) 
        {
            weldmsg(obj);
            return 0;
        }
    }

    if (obj == uarms) 
    {
        if (welded(uarms, &youmonst)) 
        {
            weldmsg(obj);
            return 0;
        }
    }

    /* Now check if you want to sell it */

    long ltmp = 0L, offer, shkmoney;
    boolean saleitem = FALSE, container = Has_contents(obj);
    boolean isgold = (obj->oclass == COIN_CLASS);
    boolean inroom = is_smith ? inhissmithy(mtmp) : in_his_npc_room(mtmp);

    if (!inroom)
    {
        res = 0;
        goto merge_obj_back;
    }

    if (container)
    {
        /* NPCs do not currently buy containers */
        res = 0;
        goto merge_obj_back;
    }

    if (is_smith)
    {
        saleitem = is_ore(obj);
    }
    else if(has_enpc(mtmp))
    {
        if ((npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_DILITHIUM_CRYSTALS) && obj->otyp == DILITHIUM_CRYSTAL)
        {
            saleitem = TRUE;
            makeknown(DILITHIUM_CRYSTAL);
        }

        if((npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_GEMS_AND_STONES) && obj->oclass == GEM_CLASS)
            saleitem = TRUE;
        
        if ((npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_SPELLBOOKS) && obj->oclass == SPBOOK_CLASS)
            saleitem = TRUE;
    }

    if (!isgold && saleitem)
        ltmp = set_cost(obj, mtmp);

    offer = ltmp;

    /* get one case out of the way: nothing to sell, and no gold */
    if (!isgold && offer == 0L) 
    {
        play_sfx_sound(SFX_SEEMS_UNINTERESTED);
        pline("%s seems uninterested.", Monnam(mtmp));
        res = 1;
        goto merge_obj_back;
    }

    if (!saleitem
        || obj->oclass == BALL_CLASS || obj->oclass == CHAIN_CLASS
        || offer == 0L) 
    {
        play_sfx_sound(SFX_SEEMS_UNINTERESTED);
        pline("%s seems uninterested.", Monnam(mtmp));
        res = 1;
        goto merge_obj_back;
    }

    shkmoney = money_cnt(mtmp->minvent);
    if (!shkmoney) 
    {
        play_sfx_sound(SFX_CANNOT_PAY);
        pline("%s cannot pay you at present.", Monnam(mtmp));
    }
    else 
    {
        char qbuf[BUFSZ], qsfx[BUFSZ];
        boolean short_funds = (offer > shkmoney), one;

        if (short_funds)
            offer = shkmoney;

        Sprintf(qbuf, "%s offers%s %ld gold piece%s for %s ",
            Monnam(mtmp), short_funds ? " only" : "", offer,
            plur(offer),
            "your");
        one = (obj->quan == 1L);
        Sprintf(qsfx, ".  Sell %s?",
            one ? "it" : "them");
        (void)safe_qbuf(qbuf, qbuf, qsfx, obj, xname, simpleonames,
            one ? "that" : "those");

        switch (yn_query(qbuf))
        {
        case 'n':
            pline1(Never_mind);
            break;
        case 'y':
            if (release_item_from_hero_inventory(obj))
            {
                play_sfx_sound(SFX_SELL_TO_NPC);
                You("sold %s for %ld gold piece%s to %s.", doname(obj), offer, plur(offer), mon_nam(mtmp));

                if (*u.ushops || obj->unpaid)
                    check_shop_obj(obj, mtmp->mx, mtmp->my, FALSE);

                (void)mpickobj(mtmp, obj);
                money2u(mtmp, offer);
                return 1;
            }
            break;
        default:
            break;
        }
    }

    /* Merge item back */
merge_obj_back:
    if (obj->where == OBJ_INVENT)
    {
        for (otmp = invent; otmp; otmp = otmp->nobj)
            if (merged(&otmp, &obj)) 
            {
                obj = otmp;
                if (!obj)
                {
                    panic("sell_to_npc: null obj after merge");
                    return res;
                }
                break;
            }
    }

    return res;
}


STATIC_OVL int
do_chat_quest_chat(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    (void)quest_chat(mtmp); /* To avoid two consecutive hints */
    return 0;
}


double
service_cost_charisma_adjustment(cha)
int cha;
{
    if (cha < 1 || cha > 25)
        return 1.0;

    return pow(2.0, (11.0 - (double)cha) / 14.0);
}



#ifdef USER_SOUNDS

#ifdef ANDROID
extern void FDECL(load_usersound, (const char*));
#endif

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
        filename[255] = '\0';

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
#ifdef ANDROID
                load_usersound(soundmap->filename);
#endif
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





STATIC_OVL int
spell_service_query(mtmp, service_spell_id, buc, service_verb, service_cost, no_mood_string)
struct monst* mtmp;
int service_spell_id, buc, service_cost;
const char* service_verb;
const char* no_mood_string;
{

    long umoney = money_cnt(invent);
    int u_pay;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, no_mood_string) || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    Sprintf(qbuf, "Would you like to %s? (%d %s)", service_verb, service_cost, currency((long)service_cost));
    switch (ynq(qbuf)) 
    {
    default:
    case 'n':
    case 'q':
        return 0;
    case 'y':
        if (umoney < (long)service_cost) 
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = service_cost;
        break;
    }


    struct obj* pseudo = mksobj(service_spell_id, FALSE, FALSE, FALSE);
    if(buc > 0)
        pseudo->blessed = 1, pseudo->cursed = 0;
    else if (buc == 0)
        pseudo->blessed = 0, pseudo->cursed = 0;
    else if (buc < 0)
        pseudo->blessed = 0, pseudo->cursed = 1;

    pseudo->quan = 20L; /* do not let useup get it */
    pseudo->speflags = SPEFLAGS_SERVICED_SPELL;
    boolean effect_happened = 0;
    (void)seffects(pseudo, &effect_happened);
    if (effect_happened)
    {
        money2mon(mtmp, (long)u_pay);
        context.botl = 1;
    }
    obfree(pseudo, (struct obj*)0);
    /* gnostic handled in seffects */

    return 1;
}

STATIC_OVL int
general_service_query(mtmp, service_func, service_verb, service_cost, no_mood_string)
struct monst* mtmp;
int (*service_func)(struct monst*);
const char* service_verb;
long service_cost;
const char* no_mood_string;
{

    return general_service_query_with_components(mtmp, service_func, service_verb, service_cost, no_mood_string, (const char* )0);
}

STATIC_OVL int
general_service_query_with_components(mtmp, service_func, service_verb, service_cost, no_mood_string, component_string)
struct monst* mtmp;
int (*service_func)(struct monst*);
const char* service_verb;
long service_cost;
const char* no_mood_string;
const char* component_string;
{

    long umoney = money_cnt(invent);
    long u_pay;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, no_mood_string) || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You("have no money.");
        return 0;
    }

    if(component_string)
        Sprintf(qbuf, "Would you like to %s? (%ld %s, %s)", service_verb, service_cost, currency(service_cost), component_string);
    else
        Sprintf(qbuf, "Would you like to %s? (%ld %s)", service_verb, service_cost, currency(service_cost));

    switch (ynq(qbuf))
    {
    default:
    case 'n':
    case 'q':
        return 0;
    case 'y':
        if (umoney < service_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You("don't have enough money for that!");
            return 0;
        }
        u_pay = service_cost;
        break;
    }

    int res = service_func(mtmp);
    if (res)
    {
        money2mon(mtmp, u_pay);
        context.botl = 1;
    }

    return 1;
}


STATIC_OVL int
repair_armor_func(mtmp)
struct monst* mtmp;
{
    const char repair_armor_objects[] = { ALL_CLASSES, ARMOR_CLASS, 0 };
    struct obj* otmp = getobj(repair_armor_objects, "repair", 0, "");

    if (!otmp)
        return 0;

    pline("%s says: \"Let's have a look at %s.\"", Monnam(mtmp), yname(otmp));

    if (otmp && otmp->oclass != ARMOR_CLASS)
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        verbalize("Sorry, this is not an armor I can repair.");
        return 0;
    }
    else if (otmp && !erosion_matters(otmp))
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        verbalize("Sorry, I couldn't make this any better than before.");
        return 0;
    }

    play_sfx_sound(SFX_REPAIR_ITEM_SUCCESS);
    if (otmp->oeroded || otmp->oeroded2)
    {
        otmp->oeroded = otmp->oeroded2 = 0;
        pline("%s as good as new!",
            Yobjnam2(otmp, Blind ? "feel" : "look"));
    }
    else
    {
        otmp->oeroded = otmp->oeroded2 = 0;
        pline("%s as good as new, just like %s before!",
            Yobjnam2(otmp, Blind ? "feel" : "look"), otmp->quan == 1 ? "it was" : "they were");
    }
    update_inventory();
    verbalize("Thank you for using my services.");
    return 1;
}

STATIC_OVL int
repair_weapon_func(mtmp)
struct monst* mtmp;
{
    const char repair_weapon_objects[] = { ALL_CLASSES, WEAPON_CLASS, 0 };
    struct obj* otmp = getobj(repair_weapon_objects, "repair", 0, "");

    if (!otmp)
        return 0;

    pline("%s says: \"Let's have a look at %s.\"", Monnam(mtmp), yname(otmp));

    /* Check if the selection is not an appropriate weapon */
    if (otmp && !is_weapon(otmp))
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        verbalize("Sorry, this is not a weapon I can repair.");
        return 0;
    }
    else if (otmp && !erosion_matters(otmp))
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        verbalize("Sorry, I couldn't make this any better than before.");
        return 0;
    }

    play_sfx_sound(SFX_REPAIR_ITEM_SUCCESS);
    if (otmp->oeroded || otmp->oeroded2)
    {
        otmp->oeroded = otmp->oeroded2 = 0;
        pline("%s as good as new!",
            Yobjnam2(otmp, Blind ? "feel" : "look"));
    }
    else
    {
        otmp->oeroded = otmp->oeroded2 = 0;
        pline("%s as good as new, just like %s before!",
            Yobjnam2(otmp, Blind ? "feel" : "look"), otmp->quan == 1 ? "it was" : "they were");
    }
    update_inventory();
    verbalize("Thank you for using my services.");
    return 1;
}

STATIC_OVL int
refill_lantern_func(mtmp)
struct monst* mtmp;
{
    const char refill_lantern_objects[] = { ALL_CLASSES, TOOL_CLASS, 0 };
    struct obj* otmp = getobj(refill_lantern_objects, "refill", 0, "");

    if (!otmp)
        return 0;

    /* Check if the selection is appropriate */
    if (otmp && !is_refillable_with_oil(otmp))
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        verbalize("Sorry, this is not an item that I can fill with oil.");
        return 0;
    }
    else if (otmp && otmp->age > 1500L)
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        verbalize("Sorry, %s %s already full.", yname(otmp), otense(otmp, "are"));
        return 0;
    }

    if (otmp->lamplit)
        snuff_lit(otmp);

    play_sfx_sound(SFX_REPAIR_ITEM_SUCCESS);

    /* Adding oil to an empty magic lamp renders it into an oil lamp */
    if ((otmp->otyp == MAGIC_LAMP) && otmp->special_quality == 0)
    {
        otmp->otyp = OIL_LAMP;
        otmp->age = 0;
    }

    pline("%s fills %s with oil.", Monnam(mtmp), yname(otmp));

    otmp->age = 1500L;
    otmp->special_quality = 1;
    update_inventory();

    verbalize("Thank you for using my services.");
    return 1;
}

STATIC_OVL int
forge_dragon_scale_mail_func(mtmp)
struct monst* mtmp;
{
    const char forge_objects[] = { ALL_CLASSES, ARMOR_CLASS, 0 };
    struct obj* otmp = getobj_ex(forge_objects, "forge into a dragon scale mail", 0, "", maybe_dragon_scales);

    if (!otmp)
        return 0;

    pline("%s says: \"Let's have a look at %s.\"", Monnam(mtmp), yname(otmp));

    /* Check if the selection is appropriate */
    if (otmp && !maybe_dragon_scales(otmp))
    {
        play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
        verbalize("Sorry, this is not an item that I can forge into a dragon scale mail.");
        return 0;
    }

    pline("%s starts working on %s.", Monnam(mtmp), yname(otmp));

    dragon_scales_to_scale_mail(otmp, FALSE);

    verbalize("Thank you for using my services.");
    return 1;
}

STATIC_OVL int
forge_orichalcum_full_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, "forge into an orichalcum full plate mail", maybe_orichalcum_ore, GEM_CLASS, NUGGET_OF_ORICHALCUM_ORE, 8, ORICHALCUM_FULL_PLATE_MAIL);
}

STATIC_OVL int
forge_adamantium_full_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, "forge into an adamantium full plate mail", maybe_adamantium_ore, GEM_CLASS, NUGGET_OF_ADAMANTIUM_ORE, 8, ADAMANTIUM_FULL_PLATE_MAIL);
}

STATIC_OVL int
forge_mithril_full_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, "forge into an mithril full plate mail", maybe_mithril_ore, GEM_CLASS, NUGGET_OF_MITHRIL_ORE, 8, MITHRIL_FULL_PLATE_MAIL);
}

STATIC_OVL int
forge_special_func(mtmp, forge_string, forge_source_func, forge_source_class, forge_source_otyp, forge_source_quan, forge_dest_otyp)
struct monst* mtmp;
const char* forge_string;
boolean(*forge_source_func)(struct obj*);
char forge_source_class;
int forge_source_otyp;
int forge_source_quan;
int forge_dest_otyp;
{
    char forge_objects[3] = { 0, 0, 0 };
    forge_objects[0] = ALL_CLASSES;
    forge_objects[1] = forge_source_class;
    forge_objects[2] = 0;

    struct obj* otmp = getobj_ex((const char*)forge_objects, forge_string, 0, "", forge_source_func);

    if (!otmp)
        return 0;

    pline("%s says: \"Let's have a look at %s.\"", Monnam(mtmp), yname(otmp));

    int quan_needed = forge_source_quan;
    /* Check if the selection is appropriate */
    if (otmp && !forge_source_func(otmp))
    {
        play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
        verbalize("Sorry, this is not an item that I can forge into %s.", an(OBJ_NAME(objects[forge_dest_otyp])));
        return 0;
    }

    if (otmp->quan < quan_needed)
    {
        play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
        struct obj pseudo = zeroobj;
        pseudo.otyp = forge_source_otyp;
        pseudo.oclass = objects[forge_source_otyp].oc_class;
        pseudo.quan = quan_needed;
        verbalize("Sorry, you need %d %s to forge %s.", quan_needed, cxname(&pseudo), an(OBJ_NAME(objects[forge_dest_otyp])));
        return 0;
    }

    pline("%s starts working on %s.", Monnam(mtmp), yname(otmp));
    if (otmp->quan > quan_needed)
    {
        otmp->quan -= quan_needed;
        otmp->owt = weight(otmp);
    }
    else
    {
        useupall(otmp);
        otmp = 0;
    }

    struct obj* craftedobj = mksobj(forge_dest_otyp, FALSE, FALSE, 3);
    if (craftedobj)
    {
        pline("%s hands %s to you.", Monnam(mtmp), an(cxname(craftedobj)));
        hold_another_object(craftedobj, "Oops!  %s out of your grasp!",
            The(aobjnam(craftedobj, "slip")),
            (const char*)0);

        verbalize("Thank you for using my services.");
    }
    else
    {
        pline("%s stares blankly for a moment as if something is seriously amiss.", Monnam(mtmp));
    }

    return 1;
}

STATIC_OVL int
spell_teaching(mtmp, spell_otyps)
struct monst* mtmp;
int* spell_otyps;
{
    if(!mtmp || !spell_otyps)
        return 0;

    int cnt = 0, not_known_cnt = 0;
    int* spell_otyp_ptr;

    for (spell_otyp_ptr = spell_otyps; spell_otyp_ptr && *spell_otyp_ptr > STRANGE_OBJECT; spell_otyp_ptr++)
    {
        cnt++;
        int i = *spell_otyp_ptr;
        if (!already_learnt_spell_type(i))
            not_known_cnt++;
    }

    if (cnt == 0)
    {
        verbalize("Unfortunately, I cannot teach any spells at the moment.");
        return 1;
    }
    else if (not_known_cnt == 0)
    {
        verbalize("Unfortunately, I cannot teach any spells you do not already know.");
        return 1;
    }


    int spell_count = 0;

    menu_item* selected = (menu_item*)0;
    winid win;
    anything any;

    any = zeroany;
    win = create_nhwindow(NHW_MENU);
    start_menu(win);

    for(spell_otyp_ptr = spell_otyps; spell_otyp_ptr && *spell_otyp_ptr > STRANGE_OBJECT; spell_otyp_ptr++)
    {
        int i = *spell_otyp_ptr;

        if (already_learnt_spell_type(i))
            continue;

        any = zeroany;
        char spellbuf[BUFSIZ] = "";
        struct obj pseudo = zeroobj;
        pseudo.otyp = i;
        pseudo.blessed = 1;
        long cost = get_cost(&pseudo, mtmp);
        Sprintf(spellbuf, "%s (%ld %s)", OBJ_NAME(objects[i]), cost, currency(cost));
        *spellbuf = highc(*spellbuf);

        any.a_int = i;
        char let = 'a' + spell_count;
        int glyph = obj_to_glyph(&pseudo, rn2_on_display_rng);

        add_menu(win, glyph, &any,
            let, 0, ATR_NONE,
            spellbuf, MENU_UNSELECTED);

        spell_count++;
    }

    /* Finish the menu */
    end_menu(win, "Which spell do you want to learn?");

    if (spell_count <= 0)
    {
        pline("%s doesn't have any spells to teach.", Monnam(mtmp));
        destroy_nhwindow(win);
        return 0;
    }

    /* Now generate the menu */
    int pick_count = 0;
    int learn_count = 0;
    if ((pick_count = select_menu(win, PICK_ONE, &selected)) > 0)
    {
        int spell_to_learn = selected->item.a_int;
        if (spell_to_learn > 0 && objects[spell_to_learn].oc_class == SPBOOK_CLASS)
        {
            struct obj pseudo = zeroobj;
            pseudo.otyp = spell_to_learn;
            pseudo.blessed = 1;
            long cost = get_cost(&pseudo, mtmp);
            char buf[BUFSIZ] = "";
            char buf2[BUFSIZ] = "";
            Sprintf(buf, "learn the spell '%s'", OBJ_NAME(objects[spell_to_learn]));
            Sprintf(buf2, "learning the spell '%s'", OBJ_NAME(objects[spell_to_learn]));
            context.spbook.book = &pseudo;
            context.spbook.reading_result = READING_RESULT_SUCCESS;
            learn_count = general_service_query(mtmp, learn_spell_func, buf, cost, buf2);
            context.spbook.book = 0;
            context.spbook.reading_result = 0;
        }
    }
    else
        pline1(Never_mind);

    free((genericptr_t)selected);
    destroy_nhwindow(win);

    if (learn_count > 0)
        return 1;
    else
        return 0;
}

STATIC_OVL int
learn_spell_func(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !context.spbook.book)
        return 0;

    int i = 0;
    int booktype = (short)context.spbook.book->otyp;

    for (i = 0; i < MAXSPELL; i++)
        if (spellid(i) == booktype || spellid(i) == NO_SPELL)
            break;

    if (i == MAXSPELL)
    {
        impossible("Too many spells memorized!");
    }
    else if (spellid(i) == booktype)
    {
        /* Should not happen */
        if (spellknow(i) > SPELL_IS_KEEN / 10)
        {
            play_sfx_sound(SFX_SPELL_KNOWN_ALREADY);
            You("know %s quite well already.", OBJ_NAME(objects[booktype]));
        }
        else
        { /* spellknow(i) <= SPELL_IS_KEEN/10 */
            play_sfx_sound(SFX_SPELL_KEENER);
            Your("knowledge of %s is %s.", OBJ_NAME(objects[booktype]),
                spellknow(i) ? "keener" : "restored");
            incr_spell_nknow(i, 1);
        }
    }
    else
    { 
        spl_book[i].sp_id = (short)booktype;
        spl_book[i].sp_lev = (xchar)objects[booktype].oc_spell_level;
        spl_book[i].sp_matcomp = objects[booktype].oc_material_components;
        if (spl_book[i].sp_matcomp)
            spl_book[i].sp_amount = 0; //How many times material components have been mixed
        else
            spl_book[i].sp_amount = -1; //Infinite
        spl_book[i].sp_cooldownlength = objects[booktype].oc_spell_cooldown;
        spl_book[i].sp_cooldownleft = 0;
        spl_book[i].sp_skillchance = objects[booktype].oc_spell_skill_chance;

        incr_spell_nknow(i, 1);
        play_sfx_sound(SFX_SPELL_LEARN_SUCCESS);
        You(i > 0 ? "add %s to your repertoire." : "learn %s.", OBJ_NAME(objects[booktype]));
    }

    return 1;
}

/*sounds.c*/
