/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-08-01 */

/* GnollHack 4.0    sounds.c    $NHDT-Date: 1542765362 2018/11/21 01:56:02 $  $NHDT-Branch: GnollHack-3.6.2-beta01 $:$NHDT-Revision: 1.81 $ */
/*      Copyright (c) 1989 Janet Walz, Mike Threepoint */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include <math.h>

STATIC_DCL boolean FDECL(mon_is_gecko, (struct monst *));
STATIC_DCL int FDECL(domonnoise, (struct monst *, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL int FDECL(domonnoise_with_popup, (struct monst*));
STATIC_DCL boolean NDECL(speak_check);
STATIC_DCL boolean NDECL(yell_check);
STATIC_DCL boolean FDECL(m_speak_check, (struct monst*));
STATIC_DCL boolean FDECL(m_general_talk_check, (struct monst*, const char*));
STATIC_DCL int NDECL(dochat);
STATIC_DCL int FDECL(do_chat_whoareyou, (struct monst*));
STATIC_DCL int FDECL(do_chat_rumors, (struct monst*));
STATIC_DCL struct monst* FDECL(ask_target_monster, (struct monst*));

STATIC_DCL void FDECL(hermit_talk, (struct monst*, const char**, enum ghsound_types));
#if 0
STATIC_DCL void FDECL(hermit_talk_with_startindex, (struct monst*, const char**, enum ghsound_types, UCHAR_P, int));
#endif
STATIC_DCL void FDECL(popup_talk, (struct monst*, const char**, enum ghsound_types, int, int, BOOLEAN_P, BOOLEAN_P));
STATIC_DCL void FDECL(popup_talk_core, (struct monst*, const char**, enum ghsound_types, UCHAR_P, int, int, int, BOOLEAN_P, BOOLEAN_P));

STATIC_DCL int FDECL(do_chat_hermit_dungeons, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit_quests, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit_gnomish_mines, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit_sokoban, (struct monst*));
//STATIC_DCL int FDECL(do_chat_hermit_sokoprizes, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit_further_advice, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit_castle, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit_gehennom, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit_wizard_of_yendor, (struct monst*));

STATIC_DCL int FDECL(do_chat_hermit2_castle, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit2_gehennom, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit2_wizard_of_yendor, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit2_vampire_lord, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit2_silver_bell, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit2_candelabrum, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit2_book_of_the_dead, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit2_ritual, (struct monst*));

STATIC_DCL int FDECL(do_chat_hermit3_gnomish_mines, (struct monst*));
STATIC_DCL int FDECL(do_chat_hermit3_luckstone, (struct monst*));
STATIC_DCL int FDECL(do_chat_orc_hermit3_gnomish_mines, (struct monst*));
STATIC_DCL int FDECL(do_chat_orc_hermit3_luckstone, (struct monst*));

#define hermit_told_dungeon special_talk_flag1
#define hermit_told_quests special_talk_flag2
#define hermit_told_gnomish_mines special_talk_flag3
#define hermit_told_sokoban special_talk_flag4
#define hermit_told_further_advice special_talk_flag5
#define hermit_told_wizard_of_yendor special_talk_flag6
#define hermit_told_castle special_talk_flag7
#define hermit_told_gehennom special_talk_flag8

#define hermit2_told_castle special_talk_flag1
#define hermit2_told_gehennom special_talk_flag2
#define hermit2_told_wizard_of_yendor special_talk_flag3
#define hermit2_told_vampire_lord special_talk_flag4
#define hermit2_told_candelabrum special_talk_flag5
#define hermit2_told_book_of_the_dead special_talk_flag6
#define hermit2_told_silver_bell special_talk_flag7
#define hermit2_told_ritual special_talk_flag8

#define hermit3_told_gnomish_mines special_talk_flag1

STATIC_DCL int FDECL(do_chat_quantum_experiments, (struct monst*));
STATIC_DCL int FDECL(do_chat_quantum_large_circular_dungeon, (struct monst*));
STATIC_DCL int FDECL(do_chat_quantum_special_wand, (struct monst*));
STATIC_DCL int FDECL(do_chat_quantum_disintegration_wand, (struct monst*));
STATIC_DCL int FDECL(do_chat_quantum_teleportation_wand, (struct monst*));

#define quantum_told_experiments special_talk_flag1

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
STATIC_DCL int FDECL(do_chat_quaff, (struct monst*));
STATIC_DCL int FDECL(do_chat_uncurse_items, (struct monst*));
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
STATIC_DCL int FDECL(do_chat_smith_forge_standard_armor, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_forge_special_armor, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_identify, (struct monst*));
STATIC_DCL int FDECL(do_chat_smith_sell_ore, (struct monst*));
STATIC_DCL int FDECL(do_chat_quantum_mechanic_research_support, (struct monst*));
STATIC_DCL int FDECL(do_chat_quantum_observe_position, (struct monst*));
STATIC_DCL int FDECL(do_chat_quantum_observe_speed, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_special_hints, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_sing_song, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_reconciliation, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_identify_gems_and_stones, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_identify_accessories_and_charged_items, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_identify_gems_stones_and_charged_items, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_general_identify, (struct monst*, const char*, int, long, int, int));
STATIC_DCL int FDECL(do_chat_npc_sell_gems_and_stones, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_forge_sling_bullets, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_sell_dilithium_crystals, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_sell_spellbooks, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_forge_cubic_gate, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_forge_artificial_wings, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_branch_portal, (struct monst*));
STATIC_DCL int FDECL(sell_to_npc, (struct obj*, struct monst*, int, BOOLEAN_P));
STATIC_DCL int FDECL(sell_many_to_npc, (struct monst*, boolean FDECL((*), (OBJ_P))));
STATIC_DCL int FDECL(do_chat_npc_enchant_accessory, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_recharge, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_blessed_recharge, (struct monst*));
STATIC_DCL int FDECL(do_chat_npc_teach_spells, (struct monst*));
STATIC_DCL int FDECL(do_chat_watchman_reconciliation, (struct monst*));
STATIC_DCL int FDECL(do_chat_quest_chat, (struct monst*));
STATIC_DCL int FDECL(mon_in_room, (struct monst *, int));
STATIC_DCL int FDECL(spell_service_query, (struct monst*, int, int, const char*, long, const char*, int));
STATIC_DCL int FDECL(general_service_query, (struct monst*, int (*)(struct monst*), const char*, long, const char*, int));
STATIC_DCL int FDECL(general_service_query_with_extra, (struct monst*, int (*)(struct monst*), const char*, long, const char*, int, const char*, int));
STATIC_DCL int FDECL(general_service_query_with_item_cost_adjustment_and_extra, (struct monst*, int (*)(struct monst*, struct obj*), const char*, const char*, long, long, long, const char*, int, const char*, int));
STATIC_DCL int FDECL(recharge_item_func, (struct monst*, struct obj*));
STATIC_DCL int FDECL(blessed_recharge_item_func, (struct monst*, struct obj*));
STATIC_DCL int FDECL(repair_armor_func, (struct monst*));
STATIC_DCL int FDECL(repair_weapon_func, (struct monst*));
STATIC_DCL int FDECL(refill_lantern_func, (struct monst*));
STATIC_DCL int FDECL(forge_special_func, (struct monst*, int, int, int, int, int, UCHAR_P, BOOLEAN_P));
STATIC_DCL int FDECL(forge_cubic_gate_func, (struct monst*));
STATIC_DCL int FDECL(forge_artificial_wings_func, (struct monst*));
STATIC_DCL int FDECL(forge_dragon_scale_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_shield_of_reflection_func, (struct monst*));
STATIC_DCL int FDECL(forge_crystal_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_adamantium_full_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_mithril_full_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_orichalcum_full_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_bronze_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_field_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_full_plate_mail_func, (struct monst*));
STATIC_DCL int FDECL(forge_iron_sling_bullets_func, (struct monst*));
STATIC_DCL int FDECL(forge_ex_iron_sling_bullets_func, (struct monst*));
STATIC_DCL int FDECL(forge_el_iron_sling_bullets_func, (struct monst*));
STATIC_DCL int FDECL(forge_silver_sling_bullets_func, (struct monst*));
STATIC_DCL int FDECL(forge_ex_silver_sling_bullets_func, (struct monst*));
STATIC_DCL int FDECL(forge_el_silver_sling_bullets_func, (struct monst*));
STATIC_DCL int FDECL(learn_spell_func, (struct monst*));
STATIC_DCL int FDECL(spell_teaching, (struct monst*, int*));
STATIC_DCL boolean FDECL(maybe_dilithium_crystal, (struct obj*));
STATIC_DCL boolean FDECL(maybe_forgeable_ore, (struct obj*));
STATIC_DCL boolean FDECL(maybe_gem, (struct obj*));
STATIC_DCL boolean FDECL(maybe_spellbook, (struct obj*));
STATIC_DCL boolean FDECL(maybe_dragon_scales, (struct obj*));
STATIC_DCL boolean FDECL(maybe_otyp, (struct obj*));
STATIC_VAR int otyp_for_maybe_otyp = 0;
STATIC_VAR boolean stop_chat = FALSE;

extern const struct shclass shtypes[]; /* defined in shknam.c */

#define QUERY_STYLE_COMPONENTS 0
#define QUERY_STYLE_SPELL 1

void You_ex1_popup(txt, title, attr, color, glyph, pflags)
const char* txt, *title;
int attr, color, glyph;
unsigned long pflags;
{
    if (!txt)
        return;
    You_ex1(attr, color, txt);
    char buf[BUFSZ];
    Sprintf(buf, "You %s", txt);
    display_popup_text(buf, title, POPUP_TEXT_GENERAL, attr, color, glyph, pflags);
}

int
ask_shk_reconciliation(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isshk || !has_eshk(mtmp))
        return 0;

    You("try to appease %s by offering %s some compensation.", noittame_mon_nam(mtmp), mhim(mtmp));
    pline("%s says:", noittame_Monnam(mtmp));
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
dosounds(VOID_ARGS)
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
        You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, fountain_msg[rn2(3) + hallu]);
    }
    if (level.flags.nsinks && !rn2(300)) {
        static const char *const sink_msg[3] = {
            "a slow drip.", "a gurgling noise.", "dishes being washed!",
        };
        You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, sink_msg[rn2(2) + hallu]);
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
                    You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, throne_msg[which]);
                else
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, throne_msg[2], uhis());
                return;
            }
        }
    }
    if (level.flags.has_swamp && !rn2(200)) {
        static const char *const swamp_msg[3] = {
            "hear mosquitoes!", "smell marsh gas!", /* so it's a smell...*/
            "hear Donald Duck!",
        };
        You_ex1(ATR_NONE, CLR_MSG_ATTENTION, swamp_msg[rn2(2) + hallu]);
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
                        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, !hallu
                            ? "someone counting money."
                            : "the quarterback calling the play.");
                    }
                    else
                    {
                        play_sfx_sound(SFX_LEVEL_SOMEONE_SEARCHING);
                        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "someone searching.");
                    }
                    break;
                }
            }
                /*FALLTHRU*/
            case 0:
                play_sfx_sound(SFX_LEVEL_FOOTSTEPS_OF_GUARD_PATROL);
                You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "the footsteps of a guard on patrol.");
                break;
            case 2:
                play_sfx_sound(SFX_LEVEL_EBENEZER_SCROOGE);
                You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "Ebenezer Scrooge!");
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
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a low buzzing.");
                    break;
                case 1:
                    play_sfx_sound(SFX_LEVEL_ANGRY_DRONE);
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "an angry drone.");
                    break;
                case 2:
                    play_sfx_sound(SFX_LEVEL_BEES_IN_BONNET);
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "bees in your %sbonnet!",
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
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly realize it is quiter than usual.");
                    break;
                case 1:
                    play_sfx_sound(SFX_LEVEL_SOMEONE_DEMANDING_QUIETNESS);
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "somebody demanding quietness.");
                    break;
                case 2:
                    play_sfx_sound(SFX_LEVEL_PAGES_TURNING_IN_HEAD);
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "pages turning in your head.");
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
                        You_feel_ex(ATR_NONE, CLR_MSG_WARNING, "ominously threatened.");
                    else
                        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "coins being assembled.");
                    break;
                case 1:
                    if (is_sleeping(mtmp))
                    {
                        play_sfx_sound(SFX_LEVEL_LOUD_DRAGON_SNORING);
                        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "loud snoring.");
                    }
                    else
                    {
                        play_sfx_sound(SFX_LEVEL_LOUD_DRAGON_ROAR);
                        You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "a loud roar.");
                    }
                    break;
                case 2:
                    play_sfx_sound(SFX_LEVEL_SOMEONE_CLAIMING_TO_BE_FIRE_AND_DEATH);
                    You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, "somebody claiming to be fire and death.");
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
                    You_ex(ATR_NONE, CLR_MSG_ATTENTION, "suddenly realize it is unnaturally quiet.");
                    break;
                case 1:
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s on the back of your %s %s up.", hair,
                              body_part(NECK), vtense(hair, "stand"));
                    break;
                case 2:
                    pline_The_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s on your %s %s to stand up.", hair,
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
                You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, barracks_msg[roll]);
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
                You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, armory_msg[roll]);
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
                You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, zoo_msg[roll]);
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

            You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, shop_msg[roll]);
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
                    pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, "For a moment, you thought you heard Neiman and Marcus arguing!");
                else
                {
                    if (!rn2(2))
                        pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, "For a moment, you thought you heard someone cursing.");
                    else
                    {
                        play_sfx_sound(SFX_LEVEL_FAINT_CHIME);
                        You_ex1(ATR_NONE, CLR_MSG_ATTENTION, "hear a faint chime but then it fades.");
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

            if (roll != 2 && mtmp) /* Should be in his shop, since it is tended */
            {
                context.global_minimum_volume = 0.15f;
                play_monster_special_dialogue_line(mtmp, roll == 0 ? PRIEST_SPECIAL_DIALOGUE_PRAISE_GOD : roll == 1 ? PRIEST_SPECIAL_DIALOGUE_HEAR_PRAYERS : PRIEST_SPECIAL_DIALOGUE_CONSIDER_DONATION);
                context.global_minimum_volume = 0.0f;
            }
            else
                play_sfx_sound(temple_sound[roll]);

            if (index(msg, '%'))
                You_hear_ex(ATR_NONE, CLR_MSG_ATTENTION, msg, iflags.using_gui_sounds ? (mon_aligntyp(mtmp) == A_NONE ? Moloch : "god") : halu_gname(EPRI(mtmp)->shralign));
            else
                You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, msg);
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
            You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, smithy_msg[roll]);
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
            if (!has_enpc(mtmp))
            {
                play_sfx_sound(npc_sound[roll]);
                You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, npc_msg[roll]);
            }
            else
            {
                boolean has_distant_sound_1 = (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_HAS_DISTANT_SOUND_1) != 0;
                boolean has_distant_sound_2 = (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_HAS_DISTANT_SOUND_2) != 0;
                boolean has_distant_sound_3 = (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_HAS_DISTANT_SOUND_3) != 0;
                boolean has_this_distant_sound = roll == 2 ? has_distant_sound_3 : roll == 0 ? has_distant_sound_1 : has_distant_sound_2;

                if (!has_this_distant_sound)
                {
                    play_sfx_sound(npc_sound[roll]);
                    You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, npc_msg[roll]);
                }
                else
                {
                    context.global_minimum_volume = 0.15f;
                    play_monster_special_dialogue_line(mtmp, roll == 2 ? NPC_LINE_NPC_ROOM_3 : roll == 0 ? NPC_LINE_NPC_ROOM_1 : NPC_LINE_NPC_ROOM_2);
                    context.global_minimum_volume = 0.0f;
                    if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].distant_line[roll] == 0)
                        You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, npc_msg[roll]);
                    else
                        You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, npc_subtype_definitions[ENPC(mtmp)->npc_typ].distant_line[roll]);
                }
            }
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
            if (roll == 1 || roll == 3)
            {
                context.global_minimum_volume = 0.15f;
                play_monster_special_dialogue_line(mtmp, roll == 3 ? ORACLE_LINE_NO_MORE_WOODCHUCKS : ORACLE_LINE_I_BEHOLD_THE_GREAT_WISDOM_OF_THE_ANCIENTS);
                context.global_minimum_volume = 0.0f;
            }
            else
            {
                play_sfx_sound(ora_sound[roll]);
            }
            You_hear_ex1(ATR_NONE, CLR_MSG_ATTENTION, ora_msg[roll]);
        }
        return;
    }
}

STATIC_VAR const char *const h_sounds[] = {
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
    if (!mtmp)
        return;

    if (!mon_can_move(mtmp) || is_non_eater(mtmp->data))
        return;

    /* presumably nearness and soundok checks have already been made */
    if (!is_silent(mtmp->data) && mtmp->data->msound <= MS_ANIMAL)
        (void) domonnoise(mtmp, FALSE, FALSE);
    else if (mtmp->data->msound >= MS_HUMANOID) {
        if (!canspotmon(mtmp))
            map_invisible(mtmp->mx, mtmp->my);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "I'm hungry.");
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
domonnoise_with_popup(mtmp)
register struct monst* mtmp;
{
    return domonnoise(mtmp, TRUE, TRUE);
}

STATIC_OVL int
domonnoise(mtmp, dopopup, fromchatmenu)
struct monst *mtmp;
boolean dopopup, fromchatmenu;
{
    char verbuf[BUFSZ];
    register const char *pline_msg = 0, /* Monnam(mtmp) will be prepended */
        *verbl_msg = 0,                 /* verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, ) */
        *verbl_msg_mcan = 0;            /* verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, ) if cancelled */
    struct permonst *ptr = mtmp->data;
    int msound = ptr->msound;
    char furiosbuf[BUFSZ] = "";

    /* presumably nearness and sleep checks have already been made */
    if (Deaf)
    {
        You_ex(ATR_NONE, CLR_MSG_ATTENTION, "cannot hear anything.");
        return 0;
    }
    /* leader might be poly'd; if he can still speak, give leader speech */
    if (mtmp->m_id == quest_status.leader_m_id && msound > MS_ANIMAL)
        msound = MS_LEADER;
    /* make sure it's your role's quest guardian; adjust if not */
    else if (msound == MS_GUARDIAN && ptr != &mons[urole.guardnum])
        msound = mons[genus(mtmp->mnum, 1)].msound;
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
    {
        if (has_enpc(mtmp) && npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_MAJORITY_NORMAL_HELLO)
        {
            if(!rn2(3))
                msound = MS_NPC;
        }
        else
            msound = MS_NPC;
    }
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
            play_monster_special_dialogue_line(mtmp, ORACLE_LINE_WELCOME_TO_DELPHI_ADVENTURER);
            Sprintf(verbuf, "Welcome to Delphi, adventurer!");
        }
        else
        {
            play_monster_special_dialogue_line(mtmp, ORACLE_LINE_BEGONE_YOU_FOOL);
            Sprintf(verbuf, "Begone, you fool!");
        }
        verbl_msg = verbuf;
        break;
        //return doconsult(mtmp);
    case MS_PRIEST:
    {
        const char* temple_god = iflags.using_gui_sounds ? (mon_aligntyp(mtmp) == A_NONE ? Moloch : "our almighty god") :
            ((mtmp->ispriest && has_epri(mtmp)) ? align_gname(mtmp->mextra->epri->shralign) : "our almighty god");

        if (is_peaceful(mtmp))
        {
            play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_WELCOME_TO_TEMPLE_LONG);
            Sprintf(verbuf, "Welcome to the temple of %s, adventurer!", temple_god);
        }
        else
        {
            play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_YOU_SHALL_PERISH);
            Sprintf(verbuf, "You shall perish by the divine hand of %s!", temple_god);
        }
        verbl_msg = verbuf;
        break;
        //priest_talk(mtmp);
    }
    case MS_LEADER:
    case MS_NEMESIS:
    case MS_GUARDIAN:
        quest_chat(mtmp, dopopup);
        break;
    case MS_SELL: /* pitch, pay, total */
        if (mtmp->isshk)
        {
            register struct eshk* eshkp = (struct eshk*)0;
            if (has_eshk(mtmp))
                eshkp = ESHK(mtmp);

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
                verbl_msg = verbuf;
            }
            else
            {
                pline_msg = "does not respond.";
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
            verbl_msg = verbuf;
        }

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
            play_monster_special_dialogue_line(mtmp, SMITH_LINE_WELCOME_TO_MY_SMITHY_ADVENTURER);
            Sprintf(verbuf, "Welcome to my smithy, adventurer!");
        }
        else
        {
            play_monster_special_dialogue_line(mtmp, SMITH_LINE_BEGONE_YOU_ROTTEN_VANDAL);
            Sprintf(verbuf, "Begone, you rotten vandal!");
        }
        verbl_msg = verbuf;
        break;
    case MS_NPC: /* pitch, pay, total */
        if (is_peaceful(mtmp))
        {
            if (mtmp->isnpc && has_enpc(mtmp))
            {
                int npctype = ENPC(mtmp)->npc_typ;
                play_monster_special_dialogue_line(mtmp, NPC_LINE_WELCOME_TO_MY_RESIDENCE_ADVENTURER);
                Sprintf(verbuf, "Welcome to %s %s, adventurer!", (npc_subtype_definitions[npctype].general_flags & NPC_FLAGS_NO_MY) ? "the" : "my", npc_subtype_definitions[ENPC(mtmp)->npc_typ].room_name);
            }
            else
            {
                play_monster_special_dialogue_line(mtmp, NPC_LINE_WELCOME_TO_MY_RESIDENCE_ADVENTURER);
                Sprintf(verbuf, "Welcome to my residence, adventurer!");
            }
        }
        else
        {
            play_monster_special_dialogue_line(mtmp, NPC_LINE_BEGONE_YOU_ROTTEN_VANDAL);
            Sprintf(verbuf, "Begone, you rotten vandal!");
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
                                      || u.umonnum == PM_VAMPIRE_LORD || u.umonnum == PM_VAMPIRE_KING));
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
                chat_line = isnight ? 4 : 5;
            } else {
                Sprintf(verbuf, "%s%s",
                        nightchild && !iflags.using_gui_sounds ? "Child of the night, " : "", /* Voice lines not currently available in */
                        midnight()
                         ? "I can stand this craving no longer!"
                         : isnight
                          ? "I beg you, help me satisfy this growing craving!"
                          : "I find myself growing a little weary.");
                verbl_msg = verbuf;
                chat_line = midnight() ? 6 : isnight ? 7 : 8;
            }
        } else if (is_peaceful(mtmp)) {
            if (kindred && isnight) {
                Sprintf(verbuf, "Good feeding %s!",
                        flags.female ? "sister" : "brother");
                verbl_msg = verbuf;
                chat_line = flags.female ? 9 : 10;
            } else if (nightchild && isnight) {
                Sprintf(verbuf, "How nice to hear you, child of the night!");
                verbl_msg = verbuf;
                chat_line = 12;
            }
            else
            {
                verbl_msg = "I only drink... potions.";
                chat_line = 13;
            }
        } else {
            int vampindex;
            static const char *const vampmsg[] = {
                /* These first two (0 and 1) are specially handled below */
                "I vant to suck your %s!",
                "I vill come after %s without regret!",
                /* other famous vampire quotes can follow here if desired */
            };
            if (kindred)
            {
                verbl_msg =
                    "This is my hunting ground that you dare to prowl!";
                chat_line = 14;
            }
            else if (youmonst.data == &mons[PM_SILVER_DRAGON] || youmonst.data == &mons[PM_ANCIENT_SILVER_DRAGON]
                     || youmonst.data == &mons[PM_SILVER_DRAGON_HATCHLING]) {
                /* Silver dragons are silver in color, not made of silver */
                Sprintf(verbuf, "%s!  Your silver sheen does not frighten me!",
                        youmonst.data == &mons[PM_SILVER_DRAGON] || youmonst.data == &mons[PM_ANCIENT_SILVER_DRAGON]
                            ? "Fool"
                            : "Young fool");
                verbl_msg = verbuf;
                chat_line = youmonst.data == &mons[PM_SILVER_DRAGON] || youmonst.data == &mons[PM_ANCIENT_SILVER_DRAGON] ? 16 : 17;
            } else {
                vampindex = rn2(SIZE(vampmsg));
                if (vampindex == 0) {
                    Sprintf(verbuf, vampmsg[vampindex], body_part(BLOOD));
                    verbl_msg = verbuf;
                    chat_line = 18;
                } else if (vampindex == 1) {
                    Sprintf(verbuf, vampmsg[vampindex],
                            Upolyd ? an(pm_monster_name(&mons[u.umonnum], flags.female))
                                   : an(racenoun));
                    verbl_msg = verbuf;
                    chat_line = 19;
                }
                else
                {
                    verbl_msg = vampmsg[vampindex];
                    chat_line = 18 + vampindex;
                }
            }
        }
    } break;
    case MS_WERE:
        if (flags.moonphase == FULL_MOON && (night() ^ !rn2(13))) {
            play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_HOWL);
            pline("%s throws back %s head and lets out a blood curdling %s!",
                  fromchatmenu ? noittame_Monnam(mtmp) : Monnam(mtmp), mhis(mtmp),
                  ptr == &mons[PM_HUMAN_WERERAT] ? "shriek" : ptr == &mons[PM_HUMAN_WEREBEAR] ? "growl" : "howl");
            wake_nearto(mtmp->mx, mtmp->my, 11 * 11);
        }
        else
        {
            chat_line = 1;
            pline_msg =
                "whispers inaudibly.  All you can make out is \"moon\".";
        }
        break;
    case MS_BARK:
bark_here:
        if (flags.moonphase == FULL_MOON && night())
        {
            play_simple_monster_sound(mtmp, MONSTER_SOUND_TYPE_HOWL);
            pline_msg = "howls.";
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
        {
            pline_msg = "squawks.";
            chat_line = 0;
        }
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
    case MS_ALEAX:
        if (u.ualignbase[A_ORIGINAL] != u.ualign.type)
        {
            switch (mtmp->talkstate_special)
            {
            default:
            case 0:
                verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "Thou hast strayed from the %s path, %s.", align_str(u.ualignbase[A_ORIGINAL]), is_living(youmonst.data) ? "mortal" : "creature");
                break;
            case 1:
                verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "I have been sent by %s to punish thee for thine insolence.", align_gname(u.ualignbase[A_ORIGINAL]));
                break;
            case 2:
                verbalize_angry1("The punishment for such insolence is death.");
                break;
            }
            mtmp->talkstate_special++;
            if (mtmp->talkstate_special == 3)
                mtmp->talkstate_special = 0;
            chat_line = 0;
        }
        else
        {
            pline_msg = "looks perplexed.";
            chat_line = 0;
        }
        break;
    case MS_BONES:
        pline("%s rattles noisily.", fromchatmenu ? noittame_Monnam(mtmp) : Monnam(mtmp));
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
        if (is_tame(mtmp)) 
        {
            play_monster_special_dialogue_line(mtmp, DJINN_LINE_SORRY_OUT_OF_WISHES);
            verbl_msg = "Sorry, I'm all out of wishes.";
        }
        else if (is_peaceful(mtmp)) 
        {
            play_monster_special_dialogue_line(mtmp, DJINN_LINE_IM_FREE);
            verbl_msg = "I'm free!";
        }
        else 
        {
            play_monster_special_dialogue_line(mtmp, DJINN_LINE_THIS_WILL_TEACH_YOU);
            verbl_msg = "This will teach you not to disturb me!";
        }
        break;
    case MS_PRISONER:
        play_monster_special_dialogue_line(mtmp, DJINN_LINE_IM_FREE);
        verbl_msg = "I'm free!";
        break;
    case MS_WATER_DEMON:
        play_monster_special_dialogue_line(mtmp, DJINN_LINE_GURGLES);
        pline_msg = "gurgles.";
        break;
    case MS_BOAST: /* giants */
        if (!is_peaceful(mtmp)) {
            switch (rn2(4)) {
            case 0:
                pline("%s boasts about %s gem collection.", fromchatmenu ? noittame_Monnam(mtmp) : Monnam(mtmp),
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
            chat_line = 11;
        }
        else if (is_dwarf(ptr))
        {
            pline_msg = "talks about mining.";
            chat_line = 11;
        }
        else if (likes_magic(ptr))
        {
            pline_msg = "talks about spellcraft.";
            chat_line = 10;
        }
        else if (ptr->mlet == S_CENTAUR)
        {
            pline_msg = "discusses hunting.";
            chat_line = 11;
        }
        else
            switch (mtmp->mnum)
            {
            case PM_HALFLING:
                pline_msg =
                    (mtmp->mhpmax - mtmp->mhp >= 10)
                        ? "complains about unpleasant dungeon conditions."
                        : "asks you about the Ruling Ring of Yendor.";

                chat_line = 11 + ((mtmp->mhpmax - mtmp->mhp >= 10) ? 1 : 0);
                break;
            case PM_ARCHAEOLOGIST:
                pline_msg =
                "describes a recent article in \"Spelunker Today\" magazine.";
                chat_line = 11;
                break;
            case PM_TOURIST:
                verbl_msg = "Aloha.";
                chat_line = 11;
                break;
            case PM_ABBOT:
            case PM_MONK:
                if (In_modron_level(&u.uz))
                {
                    pline_msg = "discusses the ways of the Modron.";
                    chat_line = 11;
                }
                else
                {
                    pline_msg = "advises you on meditation.";
                    chat_line = 12;
                }
                break;
            default:
                pline_msg = "greets you.";
                chat_line = 11;
                break;
            }
        break;
    case MS_QUANTUM:
        if (!is_peaceful(mtmp)) 
        {
            Sprintf(furiosbuf, "is furious about you ruining %s delicate experiment.", mhis(mtmp));
            pline_msg = furiosbuf;
            chat_line = 0;
            break;
        }
        /* Generic peaceful quantum mechanic behaviour. */
        if (is_fleeing(mtmp))
        {
            pline_msg = "talks something about quantum tunneling.";
            chat_line = 1;
        }
        else if (is_confused(mtmp) || is_stunned(mtmp))
        {
            pline_msg = "babbles something about interpretations of quantum mechanics.";
            chat_line = 2;
        }
        else if (mtmp->mtrapped)
        {
            pline_msg = "curses something about bad luck with quantum tunneling.";
            chat_line = 3;
        }
        else if (mtmp->mtame && !mtmp->isminion
            && moves > EDOG(mtmp)->hungrytime)
        {
            verbl_msg = "I'm so hungry I cannot think straight.";
            chat_line = 4;
        }
        else
        {
            const char* normal_msg[6] = { "discusses the Schroedinger equation.", 
                "explains the benefits of Feynman diagrams.", 
                "talks about the Heisenberg uncertainty principle.",
                "discusses an experiment in which they achieved a quantum superposition on a cat.",
                "explains that God plays dice with the universe, not unlike as in role-playing games.",
                "wonders if the universe is just one big roguelike simulation."};

            int roll = rn2(6);
            pline_msg = normal_msg[roll];
            chat_line = 5 + roll;
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
            play_monster_special_dialogue_line(mtmp, flags.female ? KOP_LINE_JUST_THE_FACTS_MAAM : KOP_LINE_JUST_THE_FACTS_SIR);
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "Just the facts, %s.", flags.female ? "Ma'am" : "Sir");
        }
        else 
        {
            static const char *const arrest_msg[3] = {
                "Anything you say can be used against you.",
                "You're under arrest!", "Stop in the name of the Law!",
            };
            int roll = rn2(3);
            play_monster_special_dialogue_line(mtmp, roll + KOP_LINE_ANYTHING_YOU_SAY_CAN_BE_USED);
            verbl_msg = arrest_msg[roll];
        }
        break;
    case MS_BRIBE:
        if (is_peaceful(mtmp) && !is_tame(mtmp)) {
            (void) demon_talk(mtmp, &stop_chat);
            break;
        }
        if (is_gnoll(mtmp->data))
            goto bark_here;
    /* fall through */
    case MS_CUSS:
        if (!is_peaceful(mtmp))
            cuss(mtmp);
        else if (is_lminion(mtmp))
        {
            verbl_msg = "It's not too late.";
            chat_line = 16;
        }
        else
        {
            verbl_msg = "We're all doomed.";
            chat_line = 17;
        }
        break;
    case MS_SPELL:
        /* deliberately vague, since it's not actually casting any spell */
        pline_msg = "seems to mutter a cantrip.";
        chat_line = 15;
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
            play_monster_special_dialogue_line(mtmp, VAULT_GUARD_LINE_PLEASE_DROP_THAT_GOLD_AND_FOLLOW_ME);
            verbl_msg = "Please drop that gold and follow me.";
        }
        else
        {
            play_monster_special_dialogue_line(mtmp, VAULT_GUARD_LINE_PLEASE_FOLLOW_ME);
            verbl_msg = "Please follow me.";
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
        int dialogue_line = (is_watch(mtmp->data) ? WATCHMAN_LINE_RESISTANCE_IS_USELESS : 1)  + (is_peaceful(mtmp) ? roll + 3 : roll);
        play_monster_special_dialogue_line(mtmp, dialogue_line);
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
            if ((tribtitle = noveltitle(&book->novelidx, 0UL, 0UL)) != 0)
            {
                if (iflags.using_gui_sounds)
                {
                    Strcpy(verbuf, "Ah, so you have a copy of a Discworld novel.");
                    Strcat(verbuf, "  I may have been misquoted there.");
                }
                else
                {
                    Sprintf(verbuf, "Ah, so you have a copy of /%s/.", tribtitle);
                    /* no Death featured in these two, so exclude them */
                    if (strcmpi(tribtitle, "Snuff")
                        && strcmpi(tribtitle, "The Wee Free Men"))
                        Strcat(verbuf, "  I may have been misquoted there.");
                }
                verbl_msg = verbuf;
                chat_line = 2;
            }
            context.tribute.Deathnotice = 1;
        } 
        else if (ms_Death && !iflags.using_gui_sounds && rn2(3) && Death_quote(verbuf, sizeof verbuf)) 
        {
            verbl_msg = verbuf;
            /* end of tribute addition */
            chat_line = -1; /* This needs to be activated later */
        }
        else if (ms_Death && !rn2(10)) 
        {
            pline_msg = "is busy reading a copy of Sandman #8.";
            chat_line = -1;
        }
        else
        {
            verbl_msg = "Who do you think you are, War?";
            chat_line = 3;
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
        char pbuf[BUFSZ];
        Sprintf(pbuf, "%s %s", fromchatmenu ? noittame_Monnam(mtmp) : Monnam(mtmp), pline_msg);
        if(dopopup)
            popup_talk_line_ex(mtmp, pbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        else
            pline1(pbuf);
    }
    else if (is_cancelled(mtmp) && verbl_msg_mcan)
    {
        if (chat_line >= 0)
            play_monster_chat_sound(mtmp, chat_line);
        if (dopopup)
            popup_talk_line(mtmp, verbl_msg_mcan);
        else
            verbalize_talk1(verbl_msg_mcan);
    }
    else if (verbl_msg) 
    {
        /* more 3.6 tribute */
        if (ptr == &mons[PM_DEATH]) 
        {
            /* Death talks in CAPITAL LETTERS
               and without quotation marks */
            char tmpbuf[BUFSZ] = "";
            (void)ucase(strcpy(tmpbuf, verbl_msg));
            if(dopopup)
                popup_talk_line_ex(mtmp, tmpbuf, ATR_NONE, CLR_MSG_GOD, TRUE, FALSE);
            else
                pline_ex1(ATR_NONE, CLR_MSG_GOD, tmpbuf);
        }
        else 
        {
            popup_talk_line(mtmp, verbl_msg);
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
        struct monst* mtmp;
        for (mtmp = fmon; mtmp; mtmp = mtmp->nmon)
        {
            if (!DEADMONSTER(mtmp) && is_tame(mtmp))
            {
                petfound = TRUE;
                if (!is_deaf(mtmp))
                {
                    mtmp->mcomingtou = 100 + rnd(50);
                    mtmp->notraveltimer = 0;
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
dotalk(VOID_ARGS)
{
    int result;

    result = dochat();
    return result;
}

int
dotalksteed(VOID_ARGS)
{
    if (!u.usteed)
        return 0;
    else
        return dochatmon(u.usteed);
}

int
dotalknearby(VOID_ARGS)
{
    int x, y;
    int nummonfound = 0;
    struct monst* selected_mtmp = 0;
    if (!Hallucination)
    {
        struct monst* mtmp = 0;
        for (x = u.ux - 1; x <= u.ux + 1; x++)
        {
            for (y = u.uy - 1; y <= u.uy + 1; y++)
                if (!(x == u.ux && y == u.uy) && isok(x, y))
                {
                    mtmp = m_at(x, y);
                    if (mtmp && !DEADMONSTER(mtmp) && monster_invokes_context_chat(mtmp))
                    {
                        nummonfound++;
                        selected_mtmp = mtmp;
                    }
                }
        }
    }

    if (!selected_mtmp || nummonfound != 1)
        return dotalk();
    else if (selected_mtmp)
        return dochatmon(selected_mtmp);
    else
        return 0; /* Should not happen */
}

STATIC_OVL boolean
speak_check(VOID_ARGS)
{
    if (is_silent(youmonst.data) || !can_speak_language(youmonst.data))
    {
        play_sfx_sound(SFX_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "As %s, you cannot speak.", an(mon_monster_name(&youmonst)));
        return 0;
    }
    if (Strangled)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "speak.  You're choking!");
        return 0;
    }
    if (Silenced)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "speak.  Your voice is gone!");
        return 0;
    }
    if (Underwater)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        Your_ex(ATR_NONE, CLR_MSG_FAIL, "speech is unintelligible underwater.");
        return 0;
    }
    if (Deaf)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "How can you hold a conversation when you cannot hear?");
        return 0;
    }
    return 1;
}

STATIC_OVL boolean
yell_check(VOID_ARGS)
{
    if (is_silent(youmonst.data) || !can_speak_language(youmonst.data))
    {
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "As %s, you cannot yell.", an(mon_monster_name(&youmonst)));
        return 0;
    }
    if (Strangled)
    {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "yell.  You're choking!");
        return 0;
    }
    if (Silenced)
    {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "yell.  Your voice is gone!");
        return 0;
    }
    if (Underwater)
    {
        You_cant_ex(ATR_NONE, CLR_MSG_FAIL, "yell underwater.");
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
        Strcpy(Mhis, mhis(mtmp));
        *Mhis = highc(*Mhis);
        pline("%s cannot answer you. %s voice is gone!", noittame_Monnam(mtmp), Mhis);
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
        play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        There_ex(ATR_NONE, CLR_MSG_FAIL, "is no one here to talk to.");
        return 0;
    }
    else if (!is_peaceful(mtmp) && !is_quantum_mechanic(mtmp->data) && !is_rider(mtmp->data))
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "%s is in no mood for %s.", noittame_Monnam(mtmp), nomoodstr);
        return 0;
    }

    return 1;
}

void
genl_chat_message(VOID_ARGS)
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
dochat(VOID_ARGS)
{
    struct monst* mtmp;
    int tx, ty;
    struct obj* otmp;
    boolean target_is_steed = FALSE;

    if (!getdir("Talk to whom? (in what direction)"))
    {
        /* decided not to chat */
        return 0;
    }

    if (u.dx == 0 && u.dy == 0 && u.dz == 0)
    {
        struct special_view_info info = { 0 };
        info.viewtype = SPECIAL_VIEW_CHAT_MESSAGE;

        /* Chat message functionality */
        (void)open_special_view(info);
        return 0;
    }

    if (u.uswallow)
    {
        play_sfx_sound(SFX_GENERAL_CURRENTLY_UNABLE_TO_DO);
        pline_ex1(ATR_NONE, CLR_MSG_FAIL, "They won't hear you out there.");
        return 0;
    }

    if (u.usteed && u.dx == 0 && u.dy == 0 && u.dz > 0)
    {
        if (!mon_can_move(u.usteed))
        {
            play_sfx_sound(SFX_MONSTER_DOES_NOT_NOTICE);
            pline("%s seems not to notice you.", noittame_Monnam(u.usteed));
            return 1;
        }
        else
            target_is_steed = TRUE; // return domonnoise(u.usteed, FALSE, TRUE);
    }
    else if (u.dz)
    {
        play_sfx_sound(SFX_GENERAL_THAT_DID_NOTHING);
        pline_ex(ATR_NONE, CLR_MSG_FAIL, "They won't hear you %s there.", u.dz < 0 ? "up" : "down");
        return 0;
    }
    else if (u.dx == 0 && u.dy == 0)
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
        pline_ex1(ATR_NONE, CLR_MSG_WARNING, "Talking to yourself is a bad habit for a dungeoneer.");
        return 0;
    }

    update_u_facing(2);

    tx = u.ux + u.dx;
    ty = u.uy + u.dy;

    if (!isok(tx, ty))
        return 0;

    mtmp = target_is_steed ? u.usteed : m_at(tx, ty);

    if ((!mtmp || mtmp->mundetected)
        && (otmp = vobj_at(tx, ty)) != 0 && otmp->otyp == STATUE)
    {
        /* Talking to a statue */
        if (!Blind)
        {
            play_sfx_sound(SFX_MONSTER_DOES_NOT_NOTICE);
            pline_The("%s seems not to notice you.",
                /* if hallucinating, you can't tell it's a statue */
                Hallucination ? rndmonnam((char*)0) : "statue");
        }
        return 0;
    }

    return dochatmon(mtmp);
}

int
dochatmon(mtmp)
struct monst* mtmp;
{
    if (!mtmp || 
        ((!canspotmon(mtmp) || mtmp->mundetected || M_AP_TYPE(mtmp) == M_AP_FURNITURE || M_AP_TYPE(mtmp) == M_AP_OBJECT) && !is_tame(mtmp)))
    {
        play_sfx_sound(SFX_GENERAL_NOTHING_THERE);
        pline_ex1(ATR_NONE, CLR_MSG_FAIL, Blind ? "You cannot see there anyone to talk to." : "There is no-one to talk to.");
        return 0;
    }

    unsigned was_ritual_known = u.uevent.invocation_ritual_known;
    unsigned was_elbereth_known = u.uevent.elbereth_known;

    if (!canspotmon(mtmp) && is_tame(mtmp))
        pline("You cannot see anyone but then you feel %s's familiar presence there.", noit_mon_nam(mtmp));

#if 0
    /* Non-speaking monster */
    if (!is_speaking(mtmp->data) && !is_tame(mtmp))
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
        msound = mons[genus(mtmp->mnum, 1)].msound;
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

#define MAXCHATNUM 50
    struct available_chat_item
    {
        int charnum;
        char name[80];
        boolean stops_dialogue;
        int (*function_ptr)();
    };

    int i = '\0';
    int result = 0;
    boolean stopsdialogue = FALSE;
    stop_chat = FALSE;
    do
    {
        i = '\0';

        menu_item* pick_list = (menu_item*)0;
        winid win;
        anything any;

        any = zeroany;
        win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_CHAT_MENU, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
        start_menu_ex(win, GHMENU_STYLE_CHAT);

        struct available_chat_item available_chat_list[MAXCHATNUM] = { {0} };
        int chatnum = 0;

        any = zeroany;

        /* Hello! This is the old chat, i.e., domonnoise function */
        Strcpy(available_chat_list[chatnum].name, "\"Hello there!\"");
        available_chat_list[chatnum].function_ptr = &domonnoise_with_popup;
        available_chat_list[chatnum].charnum = 'a' + chatnum;

        any = zeroany;
        any.a_char = available_chat_list[chatnum].charnum;

        add_menu(win, NO_GLYPH, &any,
            any.a_char, 0, ATR_NONE, NO_COLOR,
            available_chat_list[chatnum].name, MENU_UNSELECTED);

        chatnum++;

        if (is_speaking(mtmp->data) && is_mon_talkative(mtmp))
        {
            /* Who are you? */
            Strcpy(available_chat_list[chatnum].name, "\"Who are you?\"");
            available_chat_list[chatnum].function_ptr = &do_chat_whoareyou;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        boolean non_advicing_npc = has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_NO_ADVICE) != 0;

        if(is_speaking(mtmp->data) && is_peaceful(mtmp) && !non_advicing_npc)
        {
            if (!mtmp->isgd && !(mtmp->data->mflags6 & M6_HATCHLING) && (is_izchak(mtmp, TRUE) || mtmp->rumorsleft >= 0))
            {
                if(is_izchak(mtmp, TRUE))
                    Strcpy(available_chat_list[chatnum].name, mtmp->told_rumor ? "Ask what is further on his mind" : "Ask what is on his mind");
                else
                    Strcpy(available_chat_list[chatnum].name, mtmp->told_rumor ? "Ask for further adventuring advice" : "Ask for adventuring advice");

                available_chat_list[chatnum].function_ptr = &do_chat_rumors;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
        }

        if (is_speaking(mtmp->data) && is_peaceful(mtmp) && has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_GIVE_QUANTUM_QUESTS) != 0)
        {
            /* Endicott - Quantum Quests */
            if (mtmp->u_know_mname)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about experiments");
                available_chat_list[chatnum].function_ptr = &do_chat_quantum_experiments;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->quantum_told_experiments)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Large Circular Dungeon");
                available_chat_list[chatnum].function_ptr = &do_chat_quantum_large_circular_dungeon;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            struct obj* tpwand = carrying(WAN_TOWN_PORTAL);
            if (tpwand && !objects[WAN_TOWN_PORTAL].oc_name_known)
            {
                Sprintf(available_chat_list[chatnum].name, "Ask about %s", thesimpleoname(tpwand));
                available_chat_list[chatnum].function_ptr = &do_chat_quantum_special_wand;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            struct obj* diswand = carrying(WAN_DISINTEGRATION);
            if (diswand && !objects[WAN_DISINTEGRATION].oc_name_known)
            {
                Sprintf(available_chat_list[chatnum].name, "Ask about %s", thesimpleoname(diswand));
                available_chat_list[chatnum].function_ptr = &do_chat_quantum_disintegration_wand;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            struct obj* telewand = carrying(WAN_TELEPORTATION);
            if (telewand && !objects[WAN_TELEPORTATION].oc_name_known)
            {
                Sprintf(available_chat_list[chatnum].name, "Ask about %s", thesimpleoname(telewand));
                available_chat_list[chatnum].function_ptr = &do_chat_quantum_teleportation_wand;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
        }

        if (is_speaking(mtmp->data) && is_peaceful(mtmp) && has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_GIVE_STARTING_QUESTS) != 0)
        {
            /* Hermit - Starting Quests */
            Strcpy(available_chat_list[chatnum].name, "Ask about the Dungeons of Doom");
            available_chat_list[chatnum].function_ptr = &do_chat_hermit_dungeons;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Ask about the Amulet of Yendor");
            available_chat_list[chatnum].function_ptr = &do_chat_hermit_quests;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Ask about further advice");
            available_chat_list[chatnum].function_ptr = &do_chat_hermit_further_advice;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            if (mtmp->hermit_told_dungeon)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Gnomish Mines");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit_gnomish_mines;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Strcpy(available_chat_list[chatnum].name, "Ask about the Sokoban");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit_sokoban;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if(mtmp->hermit_told_quests || mtmp->hermit_told_castle)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Wizard of Yendor");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit_wizard_of_yendor;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->hermit_told_quests)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Castle");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit_castle;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Strcpy(available_chat_list[chatnum].name, "Ask about Under World");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit_gehennom;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            //if (mtmp->hermit_told_sokoban)
            //{
            //    Strcpy(available_chat_list[chatnum].name, "Ask about the prizes in Sokoban");
            //    available_chat_list[chatnum].function_ptr = &do_chat_hermit_sokoprizes;
            //    available_chat_list[chatnum].charnum = 'a' + chatnum;

            //    any = zeroany;
            //    any.a_char = available_chat_list[chatnum].charnum;

            //    add_menu(win, NO_GLYPH, &any,
            //        any.a_char, 0, ATR_NONE, NO_COLOR,
            //        available_chat_list[chatnum].name, MENU_UNSELECTED);

            //    chatnum++;
            //}

        }

        if (is_speaking(mtmp->data) && is_peaceful(mtmp) && has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_GIVE_ADVANCED_QUESTS) != 0)
        {
            /* Hermit - Advanced Quests */
            Strcpy(available_chat_list[chatnum].name, "Ask about the Castle");
            available_chat_list[chatnum].function_ptr = &do_chat_hermit2_castle;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Ask about Gehennom");
            available_chat_list[chatnum].function_ptr = &do_chat_hermit2_gehennom;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Ask about the Wizard of Yendor");
            available_chat_list[chatnum].function_ptr = &do_chat_hermit2_wizard_of_yendor;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            if (mtmp->hermit2_told_gehennom)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Vampire Lord");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit2_vampire_lord;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->hermit2_told_vampire_lord || mtmp->hermit2_told_ritual)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Candelabrum of Invocation");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit2_candelabrum;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->hermit2_told_wizard_of_yendor || mtmp->hermit2_told_ritual)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Book of the Dead");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit2_book_of_the_dead;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->hermit2_told_ritual)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Silver Bell");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit2_silver_bell;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->hermit2_told_wizard_of_yendor || mtmp->hermit2_told_vampire_lord || mtmp->hermit2_told_candelabrum || mtmp->hermit2_told_silver_bell || mtmp->hermit2_told_book_of_the_dead)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Passage to Amulet");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit2_ritual;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

        }

        if (is_speaking(mtmp->data) && is_peaceful(mtmp) && has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_GIVE_GNOMISH_QUESTS) != 0)
        {
            /* Hermit - Gnomish Quests */
            Strcpy(available_chat_list[chatnum].name, "Ask about the Gnomish Mines");
            available_chat_list[chatnum].function_ptr = &do_chat_hermit3_gnomish_mines;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            if (mtmp->hermit3_told_gnomish_mines)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Gladstone");
                available_chat_list[chatnum].function_ptr = &do_chat_hermit3_luckstone;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

        }

        if (is_speaking(mtmp->data) && is_peaceful(mtmp) && has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_GIVE_ORCISH_QUESTS) != 0)
        {
            /* Hermit - Orcish Quests */
            Strcpy(available_chat_list[chatnum].name, "Ask about the Gnomish Mines");
            available_chat_list[chatnum].function_ptr = &do_chat_orc_hermit3_gnomish_mines;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            if (mtmp->hermit3_told_gnomish_mines)
            {
                Strcpy(available_chat_list[chatnum].name, "Ask about the Gladstone");
                available_chat_list[chatnum].function_ptr = &do_chat_orc_hermit3_luckstone;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
        }


        if (is_speaking(mtmp->data) && is_peaceful(mtmp) && has_enpc(mtmp))
        {
            if ((npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_SING_SONGS) != 0)
            {
                /* Special hints about game mechanics */
                Strcpy(available_chat_list[chatnum].name, "Ask to sing a song");
                available_chat_list[chatnum].function_ptr = &do_chat_npc_sing_song;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            if ((npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_SPECIAL_NPC_HINTS) != 0)
            {
                /* Special hints about game mechanics */
                Strcpy(available_chat_list[chatnum].name, "Ask about advanced adventuring tactics");
                available_chat_list[chatnum].function_ptr = &do_chat_npc_special_hints;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
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
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->data->mlet == S_DOG && !mtmp->mstaying && mtmp->mwantstomove)
            {
                Strcpy(available_chat_list[chatnum].name, "Command to sit down");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_sit;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->data->mlet == S_DOG)
            {
                Strcpy(available_chat_list[chatnum].name, "Command to give paw");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_givepaw;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

            }

            if (!mtmp->mstaying && mtmp->mwantstomove)
            {

                if (is_animal(mtmp->data))
                    Strcpy(available_chat_list[chatnum].name, "Command to stay put");
                else if (is_speaking(mtmp->data))
                    Strcpy(available_chat_list[chatnum].name, "Command to hold position");
                else
                    Strcpy(available_chat_list[chatnum].name, "Command to hold position");

                available_chat_list[chatnum].function_ptr = &do_chat_pet_stay;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }


            if (mtmp->mstaying || !mtmp->mwantstomove)
            {
                if (is_animal(mtmp->data))
                    Strcpy(available_chat_list[chatnum].name, "Command to stop staying put");
                else if (is_speaking(mtmp->data))
                    Strcpy(available_chat_list[chatnum].name, "Command to stop holding position");
                else
                    Strcpy(available_chat_list[chatnum].name, "Command to stop holding position");

                available_chat_list[chatnum].function_ptr = &do_chat_pet_standup;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }


            if (!mtmp->mcomingtou)
            {

                Strcpy(available_chat_list[chatnum].name, "Command to follow you");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_follow;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->mcomingtou)
            {
                Strcpy(available_chat_list[chatnum].name, "Command to stop following you");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_unfollow;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }



            if (mtmp->minvent)
            {
                Strcpy(available_chat_list[chatnum].name, "Display inventory");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_display_inventory;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (mtmp->minvent && droppables(mtmp) && !mtmp->issummoned && !mtmp->ispartymember)
            {
                Strcpy(available_chat_list[chatnum].name, "Command to drop items");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_dropitems;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (OBJ_AT(mtmp->mx, mtmp->my) && !mtmp->issummoned && !mtmp->ispartymember)
            {
            
                Strcpy(available_chat_list[chatnum].name, "Command to pick the items on the ground");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_pickitems;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

        }

        /* These are available also for hostile creatures */
        if (is_animal(mtmp->data))
        {
            /* Petting */
            Sprintf(available_chat_list[chatnum].name, "Pet %s", noittame_mon_nam(mtmp));
            available_chat_list[chatnum].function_ptr = &do_chat_pet_pet;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if ((is_domestic(mtmp->data) || mtmp->data->mlet == S_DOG || mtmp->data->mlet == S_FELINE || mtmp->data->mlet == S_YETI || mtmp->data->mlet == S_UNICORN || is_tame(mtmp)) && !is_non_eater(mtmp->data))
        {
            /* Feeding */
            if(humanoid(mtmp->data))
                Sprintf(available_chat_list[chatnum].name, "Give food to %s", noittame_mon_nam(mtmp));
            else
                Sprintf(available_chat_list[chatnum].name, "Feed %s", noittame_mon_nam(mtmp));
            available_chat_list[chatnum].function_ptr = &do_chat_feed;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Sprintf(available_chat_list[chatnum].name, "Give a potion to %s to drink", noittame_mon_nam(mtmp));
            available_chat_list[chatnum].function_ptr = &do_chat_quaff;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        if (is_tame(mtmp) && invent && is_peaceful(mtmp)) /*  && !mtmp->issummoned */
        {
            Sprintf(available_chat_list[chatnum].name, "Give items to %s", noittame_mon_nam(mtmp));
            available_chat_list[chatnum].function_ptr = &do_chat_pet_giveitems;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = FALSE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            if (is_packmule(mtmp->data) && mtmp->minvent)
            {
                Sprintf(available_chat_list[chatnum].name, "Take items from %s", noittame_mon_nam(mtmp));
                available_chat_list[chatnum].function_ptr = &do_chat_pet_takeitems;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

            }
        }

        if (is_tame(mtmp) && mtmp->minvent && is_peaceful(mtmp)) /*  && !mtmp->issummoned */
        {
            Sprintf(available_chat_list[chatnum].name, "Uncurse or bless %s items", s_suffix(noittame_mon_nam(mtmp)));
            available_chat_list[chatnum].function_ptr = &do_chat_uncurse_items;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;


            if(m_has_wearable_armor_or_accessory(mtmp))
            {
                Sprintf(available_chat_list[chatnum].name, "Ask to wear a piece of armor or accessory");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_dowear;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (m_has_worn_armor_or_accessory(mtmp))
            {
                Sprintf(available_chat_list[chatnum].name, "Ask to take off a piece of armor or accessory");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_dotakeoff;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;
                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
        }

        if (is_tame(mtmp) && mtmp->minvent && can_operate_objects(mtmp->data) && attacktype(mtmp->data, AT_WEAP) && is_peaceful(mtmp)) /*  && !mtmp->issummoned */
        {
            if (select_hwep(mtmp, TRUE, 0, 0))
            {
                Sprintf(available_chat_list[chatnum].name, "Ask to wield a hand-to-hand weapon");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_dowield_hth;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            if (select_rwep(mtmp) && !(MON_WEP(mtmp) && is_launcher(MON_WEP(mtmp))))
            {
                Sprintf(available_chat_list[chatnum].name, "Ask to wield a ranged weapon");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_dowield_ranged;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (m_carrying(mtmp, PICK_AXE) && !(MON_WEP(mtmp) && MON_WEP(mtmp)->otyp == PICK_AXE))
            {
                Sprintf(available_chat_list[chatnum].name, "Ask to wield a pick axe");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_dowield_pickaxe;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            if(m_carrying(mtmp, AXE) || ((m_carrying(mtmp, BATTLE_AXE)) && !which_armor(mtmp, W_ARMS)))
            {
                Sprintf(available_chat_list[chatnum].name, "Ask to wield an axe");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_dowield_axe;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            if(MON_WEP(mtmp))
            {
                Sprintf(available_chat_list[chatnum].name, "Ask to unwield the current weapon");
                available_chat_list[chatnum].function_ptr = &do_chat_pet_dounwield;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
        }


        /* Peaceful monster with sellable items */
        if (is_peaceful(mtmp) && !(is_tame(mtmp) && !mtmp->ispartymember)
            && !mtmp->isgd
            //&& !mtmp->ispriest
            && !is_watch(mtmp->data)
            && msound != MS_ORACLE
            && msound != MS_ARREST
            && !(msound == MS_LEADER || msound == MS_GUARDIAN || msound == MS_NEMESIS)
            && mtmp->minvent && count_sellable_items(mtmp) > 0)
        {
            Strcpy(available_chat_list[chatnum].name, "Check items for sale");
            available_chat_list[chatnum].function_ptr = &do_chat_buy_items;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
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
            Strcpy(available_chat_list[chatnum].name, "Ask to join the party");
            available_chat_list[chatnum].function_ptr = &do_chat_join_party;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = FALSE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Explain current statistics");
            available_chat_list[chatnum].function_ptr = &do_chat_explain_statistics;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }


        /* Oracle */
        if (is_peaceful(mtmp) && msound == MS_ORACLE)
        {
            Strcpy(available_chat_list[chatnum].name, "Consultation");
            available_chat_list[chatnum].function_ptr = &do_chat_oracle_consult;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Identify items");
            available_chat_list[chatnum].function_ptr = &do_chat_oracle_identify;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Enlightenment");
            available_chat_list[chatnum].function_ptr = &do_chat_oracle_enlightenment;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        /* Priest */
        if (is_peaceful(mtmp) && (msound == MS_PRIEST || mtmp->ispriest))
        {
            Strcpy(available_chat_list[chatnum].name, "Standard healing");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_normal_healing;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Full healing");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_full_healing;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Cure sickness");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_cure_sickness;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Bless or curse an item");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_blesscurse;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = FALSE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Divination");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_divination;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Teach spells");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_teach_spells;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            if (mtmp->ispriest && inhistemple(mtmp))
            {
                Strcpy(available_chat_list[chatnum].name, "Chat about a monetary contribution to the temple");
                available_chat_list[chatnum].function_ptr = &do_chat_priest_chat;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

        }
        else if (is_peaceful(mtmp) && is_priest(mtmp->data) && msound != MS_ORACLE 
            && msound != MS_LEADER && msound != MS_GUARDIAN)
        {
            /* Non-priest monster priests here */
            Strcpy(available_chat_list[chatnum].name, "Healing");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_normal_healing;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

            Strcpy(available_chat_list[chatnum].name, "Cure sickness");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_cure_sickness;
            available_chat_list[chatnum].charnum = 'a' + chatnum;
            available_chat_list[chatnum].stops_dialogue = TRUE;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;


            Strcpy(available_chat_list[chatnum].name, "Divination");
            available_chat_list[chatnum].function_ptr = &do_chat_priest_divination;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        /* Quest */
        if ((is_peaceful(mtmp) && (msound == MS_LEADER || msound == MS_GUARDIAN)) || msound == MS_NEMESIS)
        {
            Strcpy(available_chat_list[chatnum].name, "Talk about your quest");
            available_chat_list[chatnum].function_ptr = &do_chat_quest_chat;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;

        }

        /* Shopkeeper */
        if (msound == MS_SELL || mtmp->isshk)
        {
            if(1)
            {
                if(is_peaceful(mtmp))
                    Strcpy(available_chat_list[chatnum].name, "Ask about the state of business");
                else
                    Sprintf(available_chat_list[chatnum].name, "Ask about what's getting on %s nerves", mhis(mtmp));
                available_chat_list[chatnum].function_ptr = &do_chat_shk_chat;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            if (!is_peaceful(mtmp))
            {
                Strcpy(available_chat_list[chatnum].name, "Ask for reconciliation");
                available_chat_list[chatnum].function_ptr = &do_chat_shk_reconciliation;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            if(invent && count_unpaid(invent, 0, FALSE))
            {
                Strcpy(available_chat_list[chatnum].name, "Pay items");
                available_chat_list[chatnum].function_ptr = &do_chat_shk_payitems;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }


            int shp_indx = 0;
            if(is_peaceful(mtmp) && has_eshk(mtmp))
            {
                shp_indx = ESHK(mtmp)->shoptype - SHOPBASE;
                const struct shclass* shp = &shtypes[shp_indx];
                char itembuf[BUFSIZ] = "";
                Strcpy(itembuf, shp->identified_item_description_plural);
            
                Sprintf(available_chat_list[chatnum].name, "Identify %s", itembuf);
                available_chat_list[chatnum].function_ptr = &do_chat_shk_identify;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            struct obj* otmp;
            if (is_peaceful(mtmp) && !Blind && (otmp = shop_object(u.ux, u.uy)) != (struct obj*) 0)
            {
                /* standing on something in a shop and chatting causes the shopkeeper
                   to describe the price(s).  This can inhibit other chatting inside
                   a shop, but that shouldn't matter much.  shop_object() returns an
                   object iff inside a shop and the shopkeeper is present and willing
                   (not angry) and able (not asleep) to speak and the position
                   contains any objects other than just gold.
                */
                Strcpy(available_chat_list[chatnum].name, "Quote items");
                available_chat_list[chatnum].function_ptr = &do_chat_shk_pricequote;
                available_chat_list[chatnum].charnum = 'a' + chatnum;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
        }

        /* Smith */
        if (msound == MS_SMITH || mtmp->issmith)
        {
            if (!is_peaceful(mtmp))
            {
                Strcpy(available_chat_list[chatnum].name, "Ask for reconciliation");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_reconciliation;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }

            if (is_peaceful(mtmp) && has_esmi(mtmp) &&!mtmp->mrevived) /* no mrivived here to prevent abuse*/
            {
                Sprintf(available_chat_list[chatnum].name, "Forge a plate armor");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_forge_standard_armor;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Sprintf(available_chat_list[chatnum].name, "Forge a special armor");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_forge_special_armor;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Strcpy(available_chat_list[chatnum].name, "Identify weapons and armor");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_identify;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                char sbuf[BUFSIZ];
                Sprintf(sbuf, "Sell nuggets of armor ore to %s", noittame_mon_nam(mtmp));
                Strcpy(available_chat_list[chatnum].name, sbuf);
                available_chat_list[chatnum].function_ptr = &do_chat_smith_sell_ore;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Sprintf(available_chat_list[chatnum].name, "Enchant a piece of armor");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_enchant_armor;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Sprintf(available_chat_list[chatnum].name, "Enchant a weapon");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_enchant_weapon;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Sprintf(available_chat_list[chatnum].name, "Repair a piece of armor");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_repair_armor;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Sprintf(available_chat_list[chatnum].name, "Repair a weapon");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_repair_weapon;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Sprintf(available_chat_list[chatnum].name, "Protect a piece of armor");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_protect_armor;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Sprintf(available_chat_list[chatnum].name, "Protect a weapon");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_protect_weapon;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;


                Sprintf(available_chat_list[chatnum].name, "Refill oil for a lamp or lantern");
                available_chat_list[chatnum].function_ptr = &do_chat_smith_refill_lantern;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

            }

        }

        if (msound == MS_QUANTUM)
        {
            if (!is_peaceful(mtmp))
            {
                Strcpy(available_chat_list[chatnum].name, "Offer research support");
                available_chat_list[chatnum].function_ptr = &do_chat_quantum_mechanic_research_support;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = FALSE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
            else
            {
                Strcpy(available_chat_list[chatnum].name, "Ask to observe your position");
                available_chat_list[chatnum].function_ptr = &do_chat_quantum_observe_position;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;

                Strcpy(available_chat_list[chatnum].name, "Ask to observe your speed");
                available_chat_list[chatnum].function_ptr = &do_chat_quantum_observe_speed;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
        }

        /* NPCs */
        if (msound == MS_NPC || mtmp->isnpc)
        {
            if (!is_peaceful(mtmp))
            {
                Strcpy(available_chat_list[chatnum].name, "Ask for reconciliation");
                available_chat_list[chatnum].function_ptr = &do_chat_npc_reconciliation;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
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
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;

                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_RECHARGING)
                {
                    Sprintf(available_chat_list[chatnum].name, "Recharge an item");
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_recharge;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BLESSED_RECHARGING)
                {
                    Sprintf(available_chat_list[chatnum].name, "Fully recharge an item");
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_blessed_recharge;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_IDENTIFY_GEMS_AND_STONES)
                {
                    char sbuf[BUFSIZ];
                    Sprintf(sbuf, "Identify gems and stones");
                    Strcpy(available_chat_list[chatnum].name, sbuf);
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_identify_gems_and_stones;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_GEMS_AND_STONES)
                {
                    char sbuf[BUFSIZ];
                    Sprintf(sbuf, "Sell gems and stones to %s", noittame_mon_nam(mtmp));
                    Strcpy(available_chat_list[chatnum].name, sbuf);
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_sell_gems_and_stones;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_FORGE_SLING_BULLETS)
                {
                    Sprintf(available_chat_list[chatnum].name, "Forge sling-bullets");
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_forge_sling_bullets;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_FORGE_CUBIC_GATE)
                {
                    Sprintf(available_chat_list[chatnum].name, "Forge a cubic gate");
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_forge_cubic_gate;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_FORGE_ARTIFICIAL_WINGS)
                {
                    Sprintf(available_chat_list[chatnum].name, "Forge a pair of artificial wings");
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_forge_artificial_wings;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_DILITHIUM_CRYSTALS)
                {
                    char sbuf[BUFSIZ];
                    Sprintf(sbuf, "Sell dilithium crystals and other gems to %s", noittame_mon_nam(mtmp));
                    Strcpy(available_chat_list[chatnum].name, sbuf);
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_sell_dilithium_crystals;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BUY_SPELLBOOKS)
                {
                    char sbuf[BUFSIZ];
                    Sprintf(sbuf, "Sell spellbooks to %s", noittame_mon_nam(mtmp));
                    Strcpy(available_chat_list[chatnum].name, sbuf);
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_sell_spellbooks;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_IDENTIFY_ACCESSORIES_AND_CHARGED_ITEMS)
                {
                    char sbuf[BUFSIZ];
                    Sprintf(sbuf, "Identify accessories and charged items");
                    Strcpy(available_chat_list[chatnum].name, sbuf);
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_identify_accessories_and_charged_items;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_IDENTIFY_GEMS_STONES_AND_CHARGED_ITEMS)
                {
                    char sbuf[BUFSIZ];
                    Sprintf(sbuf, "Identify gems, stones and charged items");
                    Strcpy(available_chat_list[chatnum].name, sbuf);
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_identify_gems_stones_and_charged_items;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = FALSE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_BRANCH_PORTAL)
                {
                    Sprintf(available_chat_list[chatnum].name, "Open a branch portal");
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_branch_portal;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;
                    available_chat_list[chatnum].stops_dialogue = TRUE;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
                        available_chat_list[chatnum].name, MENU_UNSELECTED);

                    chatnum++;
                }

                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_TEACH_SPELLS_MASK)
                {
                    char sbuf[BUFSIZ];
                    Sprintf(sbuf, "Teach spells");
                    Strcpy(available_chat_list[chatnum].name, sbuf);
                    available_chat_list[chatnum].function_ptr = &do_chat_npc_teach_spells;
                    available_chat_list[chatnum].charnum = 'a' + chatnum;

                    any = zeroany;
                    any.a_char = available_chat_list[chatnum].charnum;

                    add_menu(win, NO_GLYPH, &any,
                        any.a_char, 0, ATR_NONE, NO_COLOR,
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
                Strcpy(available_chat_list[chatnum].name, "Ask for reconciliation");
                available_chat_list[chatnum].function_ptr = &do_chat_watchman_reconciliation;
                available_chat_list[chatnum].charnum = 'a' + chatnum;
                available_chat_list[chatnum].stops_dialogue = TRUE;

                any = zeroany;
                any.a_char = available_chat_list[chatnum].charnum;

                add_menu(win, NO_GLYPH, &any,
                    any.a_char, 0, ATR_NONE, NO_COLOR,
                    available_chat_list[chatnum].name, MENU_UNSELECTED);

                chatnum++;
            }
        }

        if (is_tame(mtmp))
        {
            Strcpy(available_chat_list[chatnum].name, "Display statistics");
            available_chat_list[chatnum].function_ptr = &monsterdescription;
            available_chat_list[chatnum].charnum = 'a' + chatnum;

            any = zeroany;
            any.a_char = available_chat_list[chatnum].charnum;

            add_menu(win, NO_GLYPH, &any,
                any.a_char, 0, ATR_NONE, NO_COLOR,
                available_chat_list[chatnum].name, MENU_UNSELECTED);

            chatnum++;
        }

        /* Finish the menu */
        end_menu(win, "What do you want to say?");


        if (chatnum <= 0)
        {
            You("don't have anything to talk about.");
            destroy_nhwindow(win);
            break; //Out of while loop
        }


        /* Now generate the menu */
        if (select_menu(win, PICK_ONE, &pick_list) > 0) 
        {
            i = pick_list->item.a_char;
            free((genericptr_t)pick_list);
        }
        destroy_nhwindow(win);

        if (i == '\0')
            break; //Out of while loop

        int res = 0;
        int j;
        stopsdialogue = FALSE;
        for (j = 0; j < chatnum; j++)
        {
            if (available_chat_list[j].charnum == i)
            {
                if (i != '\0')
                {
                    stopsdialogue = available_chat_list[j].stops_dialogue;
                    res = (available_chat_list[j].function_ptr)(mtmp);
                    bot();
                    if (res == 2) /* Changed level or the like and mtmp does not exist anymore */
                    {
                        result = 1;
                        goto end_of_chat_here;
                    }

                    if(res != 0)
                        result = 1;

                    if(mtmp->talkstate_item_trading == 0)
                        mtmp->talkstate_item_trading = 1;
                    mtmp->notalktimer = 100 + rnd(200);
                }
                break;
            }
        }
        if (!is_peaceful(mtmp))
            stopsdialogue = TRUE;
    } while (i > 0 && !stopsdialogue && !stop_chat);
    
end_of_chat_here:
    if (!was_elbereth_known && u.uevent.elbereth_known)
    {
        standard_hint("You can engrave \'Elbereth\' on the ground to protect yourself against attacking monsters.", &u.uhint.elbereth);
    }
    
    if (!was_ritual_known && u.uevent.invocation_ritual_known && !u.uevent.invoked && !(u.uachieve.bell && u.uachieve.book && u.uachieve.menorah))
    {
        if (!was_elbereth_known && u.uevent.elbereth_known)
            delay_output_milliseconds(500);
        play_sfx_sound(SFX_HINT);
        custompline_ex_prefix(ATR_NONE, CLR_MSG_HINT, "QUEST UPDATE", ATR_NONE, NO_COLOR, " - ", ATR_BOLD, CLR_WHITE, 0, "Find the Candelabrum of Invocation, Silver Bell, and the Book of the Dead");
    }
    return result;
}



STATIC_OVL int
do_chat_whoareyou(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    if (!m_general_talk_check(mtmp, "chatting") || !m_speak_check(mtmp))
        return 0;

    if (Deaf)
    {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s says something but you cannot hear anything.", noittame_Monnam(mtmp));
        return 1;
    }

    char ansbuf[BUFSZ];
    int msound = mtmp->data->msound;

    if (mtmp->isshk)
    {
        register struct eshk* eshkp = (struct eshk*)0;
        if (has_eshk(mtmp))
            eshkp = ESHK(mtmp);

        if (iflags.using_gui_sounds)
        {
            play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_IM_THE_SHOPKEEPER);
            Sprintf(ansbuf, "I'm the shopkeeper.");
        }
        else
            Sprintf(ansbuf, "My name is %s.", shkname(mtmp));

        if (eshkp)
        {
            if (muteshk(mtmp))
            {
                pline("%s does not respond.", noittame_Monnam(mtmp));
                return 1;
            }

            char shopbuf[BUFSZ] = "";
            Sprintf(shopbuf, "this %s", shoptypename(eshkp->shoptype));
            Sprintf(eos(ansbuf), " I run %s.", shopbuf);
            play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_I_RUN_THIS_STORE);
        }
        if (iflags.using_gui_sounds && !mtmp->u_know_mname)
            Sprintf(eos(ansbuf), " (The name tag shows that %s name is %s.)", mhis(mtmp), shkname(mtmp));
        popup_talk_line(mtmp, ansbuf);
        mtmp->u_know_mname = 1;
    }
    else if (mtmp->ispriest || msound == MS_PRIEST)
    {
        boolean saved_know_mname = mtmp->u_know_mname;
        if (iflags.using_gui_sounds)
        {
            play_monster_standard_dialogue_line(mtmp, mtmp->mnum == PM_HIGH_PRIEST ? MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU_SECONDARY : MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
            if (has_mname(mtmp))
            {
                if (mon_aligntyp(mtmp) == A_NONE)
                    Sprintf(ansbuf, "I am a %s%s of %s.", mtmp->mnum == PM_HIGH_PRIEST ? "high " : "", mtmp->female ? "priestess" : "priest", Moloch);
                else
                    Sprintf(ansbuf, "I am a local %s%s. (The name tag indicates that %s name is %s.)", mtmp->mnum == PM_HIGH_PRIEST ? "high " : "", mtmp->female ? "priestess" : "priest", mhis(mtmp), MNAME(mtmp));
            }
            else
            {
                Sprintf(ansbuf, "I am a local %s%s.", mtmp->mnum == PM_HIGH_PRIEST ? "high " : "", mtmp->female ? "priestess" : "priest");
            }
        }
        else
        {
            if (has_mname(mtmp))
            {
                Sprintf(ansbuf, "I am %s, %s.", MNAME(mtmp), noit_mon_nam(mtmp));
            }
            else
            {
                Sprintf(ansbuf, "I am %s.", noit_mon_nam(mtmp));
            }
        }
        popup_talk_line_with_know_mname(mtmp, ansbuf, saved_know_mname);
        if (has_mname(mtmp))
            mtmp->u_know_mname = 1;
    }
    else if (mtmp->mnum == PM_ORACLE || msound == MS_ORACLE)
    {
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
        Sprintf(ansbuf, "I am the Oracle of Delphi.");
        if (has_mname(mtmp))
        {
            Sprintf(eos(ansbuf), " (The name tag indicates %s name is %s.)", mhis(mtmp), MNAME(mtmp));
        }
        popup_talk_line(mtmp, ansbuf);
        if (has_mname(mtmp))
            mtmp->u_know_mname = 1;
    }
    else if (msound == MS_ARREST)
    {
        if(!is_peaceful(mtmp))
        {
            play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
            Sprintf(ansbuf, "Hah, I'm the DDPD officer who is going to arrest you, scum!");
            popup_talk_line(mtmp, ansbuf);
        }
        else
        {
            play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU_SECONDARY);
            Sprintf(ansbuf, "I work for the DDPD.");
            if (has_mname(mtmp))
            {
                Sprintf(eos(ansbuf), " (The name tag indicate that %s name is %s.)", mhis(mtmp), MNAME(mtmp));
            }
            popup_talk_line(mtmp, ansbuf);
            if (has_mname(mtmp))
                mtmp->u_know_mname = 1;
        }
    }
    else if (is_watch(mtmp->data))
    {
        if (!is_peaceful(mtmp))
        {
            if (uwep && is_wieldable_weapon(uwep))
            {
                play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_HAH_DROP_YOUR_WEAPON_FIRST_SCUM);
                Sprintf(ansbuf, "Hah, drop your weapon first, scum!");
            }
            else
            {
                play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_THE_QUESTION_IS_WHO_ARE_YOU_SCUM);
                Sprintf(ansbuf, "The question is who are you, scum?");
            }
            popup_talk_line(mtmp, ansbuf);
        }
        else
        {
            if (iflags.using_gui_sounds)
            {
                play_monster_standard_dialogue_line(mtmp, mtmp->mnum == PM_WATCH_CAPTAIN ? MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU_SECONDARY : MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
                if (has_mname(mtmp))
                    Sprintf(ansbuf, "I am a local %s. (The name tag indicates that %s name is %s.)", mon_monster_name(mtmp), mhis(mtmp), MNAME(mtmp));
                else
                    Sprintf(ansbuf, "I am a local %s.", mon_monster_name(mtmp));
            }
            else
            {
                if (has_mname(mtmp))
                    Sprintf(ansbuf, "I am %s, a local %s.", MNAME(mtmp), mon_monster_name(mtmp));
                else
                    Sprintf(ansbuf, "I am a local %s.", mon_monster_name(mtmp));
            }
            popup_talk_line(mtmp, ansbuf);
            if (has_mname(mtmp))
                mtmp->u_know_mname = 1;
        }
    }
    else if (mtmp->m_id == quest_status.leader_m_id && msound > MS_ANIMAL)
    {
        if (mtmp->data->mtitle && *mtmp->data->mtitle)
            Sprintf(ansbuf, "I am %s, %s.", noit_mon_nam(mtmp), mtmp->data->mtitle);
        else
            Sprintf(ansbuf, "I am %s, your quest leader.", noit_mon_nam(mtmp));
    
        play_voice_quest_leader_whoareyou(mtmp);
        popup_talk_line(mtmp, ansbuf);
    }
    else if (mtmp->issmith)
    {
        boolean saved_know_mname = mtmp->u_know_mname;
        if (iflags.using_gui_sounds)
        {
            play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
            if (has_mname(mtmp))
            {
                Sprintf(ansbuf, "I am a local smith. (The name tag indicates that %s name is %s.)", mhis(mtmp), MNAME(mtmp));
            }
            else
            {
                Sprintf(ansbuf, "I am a local smith.");
            }
        }
        else
        {
            if (has_mname(mtmp))
            {
                Sprintf(ansbuf, "I am %s, a local smith.", MNAME(mtmp));
            }
            else
            {
                Sprintf(ansbuf, "I am a local smith.");
            }
        }
        popup_talk_line_with_know_mname(mtmp, ansbuf, saved_know_mname);
        if (has_mname(mtmp))
            mtmp->u_know_mname = 1;
    }
    else if (mtmp->isgd)
    {
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
        Sprintf(ansbuf, "I am the vault guard.");
        popup_talk_line(mtmp, ansbuf);
    }
    else if (mtmp->isnpc && has_enpc(mtmp))
    {
        boolean saved_know_mname = mtmp->u_know_mname;
        if (iflags.using_gui_sounds)
        {
            play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
            if (has_mname(mtmp))
            {
                if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_fixed_name != 0)
                {
                    char titlebuf[BUFSZ];
                    if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_NO_TITLE_ARTICLE)
                    {
                        Strcpy(titlebuf, npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_role_name);
                    }
                    else
                    {
                        Strcpy(titlebuf, an(npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_role_name));
                    }

                    Sprintf(ansbuf, "I am %s, %s.", MNAME(mtmp), titlebuf);
                }
                else
                    Sprintf(ansbuf, "I am a local %s. (The name tag indicates that %s name is %s.)", 
                        npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_role_name, mhis(mtmp), MNAME(mtmp));
            }
            else
            {
                Sprintf(ansbuf, "I am a local %s.", npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_role_name);
            }
        }
        else
        {
            if (has_mname(mtmp))
            {
                Sprintf(ansbuf, "I am %s, %s.", MNAME(mtmp), an(npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_role_name));
            }
            else
                Sprintf(ansbuf, "I am a local %s.", npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_role_name);

        }

        if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_fixed_explanation != 0)
            Sprintf(eos(ansbuf), "  %s", npc_subtype_definitions[ENPC(mtmp)->npc_typ].npc_fixed_explanation);

        popup_talk_line_with_know_mname(mtmp, ansbuf, saved_know_mname);
        if (has_mname(mtmp))
            mtmp->u_know_mname = 1;

    }
    else if (msound == MS_GUARDIAN)
    {
        char namebuf[BUFSIZ] = "";
        char endbuf[BUFSIZ] = "";
        char titlebuf[BUFSIZ] = "";
        switch (mtmp->mnum)
        {
        case PM_STUDENT:
            Strcpy(titlebuf, "a student at the College of Archaeology");
            break;
        case PM_CHIEFTAIN:
        case PM_NEANDERTHAL:
        case PM_PAGE:
        case PM_ATTENDANT:
        case PM_ABBOT:
        case PM_ACOLYTE:
        case PM_THUG:
        case PM_HUNTER:
        case PM_ROSHI:
        case PM_GUIDE:
        case PM_WARRIOR:
        case PM_APPRENTICE:
            Sprintf(titlebuf, "a local %s", pm_monster_name(mtmp->data, mtmp->female));
            break;
        default:
            if (mtmp->mnum == urole.guardnum)
                Strcpy(titlebuf, "your quest guardian");
            else
                Strcpy(titlebuf, "a quest guardian");
            break;

        }

        if (has_mname(mtmp))
        {
            if (!iflags.using_gui_sounds)
                Sprintf(namebuf, "%s, ", MNAME(mtmp));
            else
                Sprintf(endbuf, " (The name tag shows %s name is %s.)", mhis(mtmp), MNAME(mtmp));
        }

        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
        Sprintf(ansbuf, "I am %s%s.%s", namebuf, titlebuf, endbuf);
        popup_talk_line(mtmp, ansbuf);
        if (has_mname(mtmp))
            mtmp->u_know_mname = 1;
    }
    else if (msound == MS_NEMESIS)
    {
        Sprintf(ansbuf, "I am %s, your quest nemesis. Tremble before me!", noit_mon_nam(mtmp));
        popup_talk_line(mtmp, ansbuf);
    }
    else if (msound == MS_DJINNI)
    {
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
        Sprintf(ansbuf, "I am a genie from the Elemental Plane of Air.");
        popup_talk_line(mtmp, ansbuf);
    }
    else if (msound == MS_QUANTUM)
    {
        play_monster_standard_dialogue_line(mtmp, !is_peaceful(mtmp) ? MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU_SCUM : MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);
        if (has_mname(mtmp))
        {
            if(iflags.using_gui_sounds)
                Sprintf(ansbuf, "I am %s at the University of Yendor%s. (The name tag indicates %s name is %s.)", an(mtmp->data->mname), !is_peaceful(mtmp) ? ", scum" : "", mhis(mtmp), MNAME(mtmp));
            else
                Sprintf(ansbuf, "I am %s, %s at the University of Yendor%s.", MNAME(mtmp), an(mtmp->data->mname), !is_peaceful(mtmp) ? ", scum" : "");
            popup_talk_line(mtmp, ansbuf);
            mtmp->u_know_mname = 1;
        }
        else
        {
            Sprintf(ansbuf, "I am %s at the University of Yendor%s.", an(mtmp->data->mname), !is_peaceful(mtmp) ? ", scum" : "");
            popup_talk_line(mtmp, ansbuf);
        }
    }
    else if (is_mname_proper_name(mtmp->data))
    {
        char titlebuf[BUFSZ];
        Strcpy(titlebuf, "");
        if (!is_rider(mtmp->data) && mtmp->data->mtitle && strcmp(mtmp->data->mtitle, ""))
            Sprintf(titlebuf, ", %s", mtmp->data->mtitle);

        play_monster_switchable_who_sound(mtmp, MONSTER_WHO_SOUND_ANSWER_WHO_ARE_YOU, MONSTER_STANDARD_DIALOGUE_LINE_ANSWER_WHO_ARE_YOU);

        Sprintf(ansbuf, "I am %s%s.", mon_monster_name(mtmp), titlebuf);
        if (mtmp->mnum == PM_DEATH)
        {
            (void)ucase(ansbuf);
            popup_talk_line_ex(mtmp, ansbuf, ATR_NONE, CLR_MSG_GOD, TRUE, FALSE);
        }
        else
        {
            popup_talk_line(mtmp, ansbuf);
        }


    }
    else if (has_mname(mtmp))
    {
        play_monster_who_sound(mtmp, MONSTER_WHO_SOUND_MY_NAME_IS);
        Sprintf(ansbuf, "My name is %s.", MNAME(mtmp));
        popup_talk_line(mtmp, ansbuf);
        mtmp->u_know_mname = 1;
    }
    else
    {
        if (mtmp->mon_flags & MON_FLAGS_YOUR_CHILD)
        {
            play_monster_who_sound(mtmp, flags.female ? MONSTER_WHO_SOUND_I_DONT_KNOW_MY_NAME_MOMMY : MONSTER_WHO_SOUND_I_DONT_KNOW_MY_NAME_DADDY);
            Sprintf(ansbuf, "I don't know my name%s.", flags.female ? ", mommy" : ", daddy");
        }
        else
        {
            play_monster_switchable_who_sound(mtmp, MONSTER_WHO_SOUND_MY_NAME_IS_NONE_OF_YOUR_BUSINESS, MONSTER_STANDARD_DIALOGUE_LINE_NAME_NONE_OF_YOUR_BUSINESS);
            Sprintf(ansbuf, "My name is none of your business.");
        }
        popup_talk_line(mtmp, ansbuf);
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

    if (Deaf)
    {
        pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s says something but you cannot hear anything.", noittame_Monnam(mtmp));
        return 1;
    }

    if (mtmp->data->msound == MS_ORACLE || mtmp->data == &mons[PM_ORACLE])
    {
        char ansbuf[BUFSZ];
        play_monster_special_dialogue_line(mtmp, ORACLE_LINE_THE_WISDOM_OF_DELPHI_SHALL_BE_CONVEYED_TO_THEE_BY_CONSULTATION);
        pline("%s answers:", noittame_Monnam(mtmp));
        Sprintf(ansbuf, "The wisdom of Delphi shall be conveyed to thee by consultation.");
        popup_talk_line(mtmp, ansbuf);
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
    char rumorbuf[BUFSZ] = "";
    int truth_core = mtmp->isshk || mtmp->issmith || (mtmp->data->mflags6 & M6_ELDER) || is_angel(mtmp->data) || (mtmp->isnpc && has_enpc(mtmp) && (npc_subtype_definitions[ENPC(mtmp)->npc_typ].general_flags & NPC_FLAGS_HAS_TRUE_RUMORS) != 0) ? 1
        : (mtmp->ispriest && has_epri(mtmp) && EPRI(mtmp)->shralign == A_NONE) || (mtmp->isminion && has_emin(mtmp) && EMIN(mtmp)->min_align == A_NONE) || is_demon(mtmp->data) ? -1
        : mtmp->ispriest || mtmp->isminion ? 1 : 0;
    int truth = min(is_peaceful(mtmp), truth_core);
    boolean is_death = mtmp->mnum == PM_DEATH;

    (void)getrumor(truth, rumorbuf, TRUE);
    if (*rumorbuf)
        (void) strip_newline(rumorbuf);
        
    if (mtmp->rumorsleft == 0 || !*rumorbuf)
    {
        play_voice_monster_advice(mtmp, FALSE);
        pline("%s answers:", noittame_Monnam(mtmp));
        Sprintf(ansbuf, "Unfortunately, I don't have any %s advice for you.", mtmp->told_rumor ? "further" : "useful");
        if (is_death)
        {
            (void)ucase(ansbuf);
            popup_talk_line_ex(mtmp, ansbuf, ATR_NONE, CLR_MSG_GOD, TRUE, FALSE);
        }
        else
            popup_talk_line(mtmp, ansbuf);
        mtmp->rumorsleft = 0;
    }
    else
    {
        play_voice_monster_advice(mtmp, TRUE);
        pline("%s answers:", noittame_Monnam(mtmp));
        if (mtmp->told_rumor)
            Sprintf(ansbuf, "Let me think. Maybe keep this in mind%s", iflags.using_gui_sounds || Deaf ? "." : ":");
        else
            Sprintf(ansbuf, "Yes, here's a piece of advice for you%s", iflags.using_gui_sounds || Deaf ? "." : ":");
        
        if (is_death)
        {
            (void)ucase(ansbuf);
            popup_talk_line_ex(mtmp, ansbuf, ATR_NONE, CLR_MSG_GOD, TRUE, FALSE);
        }
        else
            popup_talk_line(mtmp, ansbuf);

        /* Tell a rumor */
        boolean isspeaking = TRUE;
        if (iflags.using_gui_sounds || Deaf)
        {
            isspeaking = FALSE;
            pline("(%s hands a note over to you.)  It reads:", noittame_Monnam(mtmp));
            if (!u.uconduct.literate++)
                livelog_printf(LL_CONDUCT, "became literate by reading a note handed over by %s.", noittame_mon_nam(mtmp));
        }
        if (is_death && isspeaking)
        {
            (void)ucase(rumorbuf);
            popup_talk_line_ex(mtmp, rumorbuf, ATR_NONE, CLR_MSG_GOD, TRUE, FALSE);
        }
        else
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "%s", rumorbuf);

        display_popup_text(rumorbuf, "Advice", POPUP_TEXT_ADVICE, ATR_NONE, is_death ? CLR_MSG_GOD : NO_COLOR, NO_GLYPH, POPUP_FLAGS_ADD_QUOTES);

        mtmp->told_rumor = TRUE;
    }

    mtmp->rumorsleft--;

    return 1;
}


STATIC_OVL int
do_chat_pet_sit(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    char pbuf[BUFSZ] = "";
    if (mtmp->mtame > 5 || (mtmp->mtame > 0 && rn2(mtmp->mtame + 1)))
    {
        Sprintf(pbuf, "%s sits down!", noittame_Monnam(mtmp));
        mtmp->mstaying = 2 + rn2(5);
        mtmp->mwantstomove = 0;
        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_POSITIVE, TRUE, FALSE);
    }
    else
    {
        Sprintf(pbuf, "%s stares at you but does nothing.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
    }

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

    char pbuf[BUFSZ] = "";
    if (givepawsuccess)
    {
        boolean tamenessadded = FALSE;
        if (mtmp->mtame > 0 && mtmp->mtame < 20 && !rn2(mtmp->mtame + 20))
        {
            mtmp->mtame++;
            tamenessadded = TRUE;
        }
        Sprintf(pbuf, "%s gives you the paw%s!", noittame_Monnam(mtmp), tamenessadded ? ", and seems happier than before" : "");
        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, tamenessadded ? CLR_MSG_POSITIVE : CLR_MSG_SUCCESS, TRUE, FALSE);

        if (touch_petrifies(mtmp->data) && !Stone_resistance && !uarmg)
        {
            char kbuf[BUFSZ];
            if (poly_when_stoned(youmonst.data))
                You_ex(ATR_NONE, CLR_MSG_WARNING, "touch %s %s without wearing gloves.", s_suffix(mon_nam(mtmp)), body_part(HAND));
            else
            {
                pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Touching %s %s without wearing gloves is a fatal mistake...", s_suffix(mon_nam(mtmp)), body_part(HAND));
                Sprintf(kbuf, "touching %s %s without gloves", s_suffix(mon_nam(mtmp)), body_part(HAND));
            }
            killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE;
            instapetrify(kbuf);
        }
    }
    else
    {
        Sprintf(pbuf, "%s stares at you but does nothing.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
    }

    return 1;
}

STATIC_OVL int
do_chat_pet_good_boy_girl(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    char pbuf[BUFSZ] = "";
    if (is_animal(mtmp->data) && mtmp->mtame > 2 && rn2(mtmp->mtame) && mon_can_move(mtmp) && (!has_edog(mtmp) || (has_edog(mtmp) && (EDOG(mtmp)->abuse <= 0 || !rn2(EDOG(mtmp)->abuse + 2)))))
    {
        play_monster_happy_sound(mtmp, MONSTER_HAPPY_SOUND_NORMAL);
        boolean tamenessadded = FALSE, becamefaithful = FALSE;
        if (mtmp->mtame > 0 && mtmp->mtame < 20 && !rn2(10 + mtmp->mtame))
        {
            mtmp->mtame++;
            tamenessadded = TRUE;
        }
        if (mtmp->mtame >= 15 && !mtmp->isfaithful && !rn2(max(2, 25 - mtmp->mtame)))
        {
            mtmp->isfaithful = 1;
            becamefaithful = TRUE;
        }

        const char* adjective = tamenessadded ? "very softly" : "softly";
        const char* inwhat = becamefaithful ? "admiration" : "appreciation";
        switch (mtmp->data->msound)
        {
        case MS_BARK:
            Sprintf(pbuf, "%s woofs %s in %s!", noittame_Monnam(mtmp), adjective, inwhat);
            break;
        case MS_MEW:
            Sprintf(pbuf, "%s mews %s in %s!", noittame_Monnam(mtmp), adjective, inwhat);
            break;
        case MS_NEIGH:
            Sprintf(pbuf, "%s snorts %s in %s!", noittame_Monnam(mtmp), adjective, inwhat);
            break;
        case MS_BLEAT:
            Sprintf(pbuf, "%s baas %s in %s!", noittame_Monnam(mtmp), adjective, inwhat);
            break;
        default:
            Sprintf(pbuf, "%s seems to %s your kind words%s!", noittame_Monnam(mtmp),
                becamefaithful ? "admire" : "appreciate", tamenessadded ? " very much" : "");
            break;
        }

        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, tamenessadded || becamefaithful ? CLR_MSG_POSITIVE : CLR_MSG_SUCCESS, TRUE, FALSE);
    }
    else if(rn2(4) && mon_can_move(mtmp))
        domonnoise(mtmp, FALSE, TRUE);
    else
    {
        Sprintf(pbuf, "%s does not seem to react to your words.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
    }
    return 1;
}

STATIC_OVL int
do_chat_pet_pet(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    char pbuf[BUFSZ] = "";
    if (touch_petrifies(mtmp->data) && !Stone_resistance && !uarmg)
    {
        char kbuf[BUFSZ];
        if (poly_when_stoned(youmonst.data))
            You_ex(ATR_NONE, CLR_MSG_WARNING, "touch %s without wearing gloves.", mon_nam(mtmp));
        else 
        {
            pline_ex(ATR_NONE, CLR_MSG_NEGATIVE, "Touching %s without wearing gloves is a fatal mistake...", mon_nam(mtmp));
            Sprintf(kbuf, "trying to pet %s without gloves", mon_nam(mtmp));
        }
        killer.hint_idx = HINT_KILLED_TOUCHED_COCKATRICE;
        instapetrify(kbuf);
    }

    if (is_animal(mtmp->data) && !is_peaceful(mtmp))
    {
        int color = NO_COLOR;
        if (!rn2(is_domestic(mtmp->data) ? 20 : 200) && !(mtmp->data->mflags2 & M2_HOSTILE) && !(mtmp->data->geno & G_UNIQ) && !mtmp->iswiz && mtmp->cham < LOW_PM)
        {
            mtmp->mpeaceful = 1;
            Sprintf(pbuf, "%s seems to appreciate your gesture!", noittame_Monnam(mtmp));
            color = CLR_MSG_POSITIVE;
        }
        else
        {
            Sprintf(pbuf, "%s does not seem to appreciate your gesture!", noittame_Monnam(mtmp));
            color = CLR_MSG_NEGATIVE;
        }

        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, color, TRUE, FALSE);
    }
    else if (is_animal(mtmp->data) && is_peaceful(mtmp) && !is_tame(mtmp))
    {
        if (!rn2(is_domestic(mtmp->data) ? 20 : 200) && !(mtmp->data->mflags2 & M2_HOSTILE) && !(mtmp->data->geno & G_UNIQ) && !mtmp->iswiz && mtmp->cham < LOW_PM)
        {
            mtmp->mpeaceful = 1;
            tamedog(mtmp, (struct obj*)0, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, TRUE, FALSE);
            Sprintf(pbuf, "%s seems to appreciate your gesture!", noittame_Monnam(mtmp));
            popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_POSITIVE, TRUE, FALSE);
        }
        else
        {
            if (!rn2(is_domestic(mtmp->data) ? 40 : 10) && !(mtmp->data->mflags2 & M2_PEACEFUL))
            {
                Sprintf(pbuf, "%s does not seem to appreciate your gesture!", noittame_Monnam(mtmp));
                setmangry(mtmp, FALSE);
                popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_NEGATIVE, TRUE, FALSE);
            }
            else
            {
                Sprintf(pbuf, "%s seems to ignore your gesture!", noittame_Monnam(mtmp));
                popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
            }
        }
    }
    else if (is_animal(mtmp->data) && mtmp->mtame > 0 && rn2(mtmp->mtame + 1) && mon_can_move(mtmp) && (!has_edog(mtmp) || (has_edog(mtmp) && (EDOG(mtmp)->abuse <= 0 || !rn2(EDOG(mtmp)->abuse + 2)))))
    {
        play_monster_happy_sound(mtmp, MONSTER_HAPPY_SOUND_PURR);
        boolean tamenessadded = FALSE, becamefaithful = FALSE;
        if (mtmp->mtame > 0 && mtmp->mtame < 20 && !rn2(10 + mtmp->mtame))
        {
            mtmp->mtame++;
            tamenessadded = TRUE;
        }
        if (mtmp->mtame >= 15 && !mtmp->isfaithful && !rn2(max(2, 25 - mtmp->mtame)))
        {
            mtmp->isfaithful = 1;
            becamefaithful = TRUE;
        }

        const char* adjective = tamenessadded ? "very softly" : "softly";
        const char* inwhat = becamefaithful ? "admiration" : "appreciation";
        switch (mtmp->data->msound)
        {
        case MS_BARK:
            Sprintf(pbuf, "%s grunts %s in %s!", noittame_Monnam(mtmp), adjective, inwhat);
            break;
        case MS_MEW:
            Sprintf(pbuf, "%s purrs %s in %s!", noittame_Monnam(mtmp), adjective, inwhat);
            break;
        case MS_NEIGH:
            Sprintf(pbuf, "%s snorts %s in %s!", noittame_Monnam(mtmp), adjective, inwhat);
            break;
        case MS_BLEAT:
            Sprintf(pbuf, "%s baas %s in %s!", noittame_Monnam(mtmp), adjective, inwhat);
            break;
        default:
            Sprintf(pbuf, "%s seems to %s your gesture%s!", noittame_Monnam(mtmp),
                becamefaithful ? "admire" : "appreciate", tamenessadded ? " very much" : "");
            break;
        }

        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, tamenessadded || becamefaithful ? CLR_MSG_POSITIVE : CLR_MSG_SUCCESS, TRUE, FALSE);
    }
    else if(rn2(4) && mon_can_move(mtmp))
        domonnoise(mtmp, FALSE, TRUE);
    else
    {
        Sprintf(pbuf, "%s does not seem to react to your gesture.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
    }
    newsym(mtmp->mx, mtmp->my);
    flush_screen(1);

    return 1;
}


STATIC_OVL int
do_chat_pet_stay(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    char pbuf[BUFSZ] = "";
    if (mtmp->mtame > 5 || (mtmp->mtame > 0 && rn2(mtmp->mtame + 1)))
    {
        if (is_steed(mtmp->data))
            Sprintf(pbuf, "%s looks determined not to move anywhere.", noittame_Monnam(mtmp));
        else if is_animal(mtmp->data)
            Sprintf(pbuf, "%s sits down and looks determined not to move anywhere.", noittame_Monnam(mtmp));
        else if (is_speaking(mtmp->data))
            Sprintf(pbuf, "%s starts to hold its position.", noittame_Monnam(mtmp));
        else
            Sprintf(pbuf, "%s starts to hold its position.", noittame_Monnam(mtmp));

        mtmp->mstaying = 25 + rn2(20);
        mtmp->mwantstomove = 0;
    }
    else
        Sprintf(pbuf, "%s stares at you but does nothing.", noittame_Monnam(mtmp));

    popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);

    return 1;
}


STATIC_OVL int
do_chat_pet_standup(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    char pbuf[BUFSZ] = "";
    if (mtmp->mtame > 0 && mtmp->mstaying)
    {
        if (is_steed(mtmp->data))
            Sprintf(pbuf, "%s stops staying put.", noittame_Monnam(mtmp));
        else if is_animal(mtmp->data)
            Sprintf(pbuf, "%s stands up.", noittame_Monnam(mtmp));
        else if (is_speaking(mtmp->data))
            Sprintf(pbuf, "%s stops holding its position.", noittame_Monnam(mtmp));
        else
            Sprintf(pbuf, "%s stops holding its position.", noittame_Monnam(mtmp));

        mtmp->mstaying = 0;
        mtmp->mwantstomove = 1;
    }
    else
        Sprintf(pbuf, "%s stares at you but does nothing.", noittame_Monnam(mtmp));

    popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);

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
        char pbuf[BUFSZ] = "";
        short oldvalue = mtmp->mcomingtou;
        mtmp->mcomingtou = 100 + rnd(50);
        mtmp->notraveltimer = 0;
        mtmp->yell_x = u.ux;
        mtmp->yell_y = u.uy;

        if (mtmp->mcomingtou > oldvalue)
            Sprintf(pbuf, "%s is now following you more closely.", noittame_Monnam(mtmp));
        else
            Sprintf(pbuf, "%s %s.", noittame_Monnam(mtmp), has_head(mtmp->data) ? "nods" : "looks perplexed");

        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);

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
        char pbuf[BUFSZ] = "";
        short oldvalue = mtmp->mcomingtou;
        mtmp->mcomingtou = 0;
        mtmp->notraveltimer = 0;
        mtmp->yell_x = 0;
        mtmp->yell_y = 0;

        if (oldvalue > 0)
            Sprintf(pbuf, "%s stops following you.", noittame_Monnam(mtmp));
        else
            Sprintf(pbuf, "%s looks perplexed.", noittame_Monnam(mtmp));

        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);

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

    if(has_edog(mtmp))
        edog = EDOG(mtmp);

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
        char pbuf[BUFSZ] = "";
        Sprintf(pbuf, "%s stares at you but does nothing.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
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
    char pbuf[BUFSZ] = "";

    int omx = mtmp->mx;
    int omy = mtmp->my;

    if (has_edog(mtmp))
        edog = EDOG(mtmp);

    if (has_edog && edog)
    {
        int itemspicked = 0;
        int shkpreaction = FALSE;
        struct obj* obj = level.objects[omx][omy];
        int i;
        for (i = 0; obj && i < 20; i++, obj = level.objects[omx][omy])
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
                        pline("%s picks up %s.", noittame_Monnam(mtmp),
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
            Sprintf(pbuf, "%s stares at you but does nothing.", noittame_Monnam(mtmp));
            popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
        }
    }
    else
    {
        Sprintf(pbuf, "%s stares at you but does nothing.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, pbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
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
    char pbuf[BUFSIZ] = "";
    Sprintf(qbuf, "What would you like to give to %s?", noittame_mon_nam(mtmp));

    /* should coordinate with perm invent, maybe not show worn items */
    n = query_objlist(qbuf, &invent,
        (USE_INVLET | INVORDER_SORT | OBJECT_COMPARISON), &pick_list, PICK_ANY,
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
                    Sprintf(pbuf, "You cannot give %s to %s. You are wearing it.", doname(otmp), noittame_mon_nam(mtmp));
                    pline_ex1_popup(ATR_NONE, CLR_MSG_FAIL, pbuf, "Cannot Give Item", TRUE);
                }
                else if (carryamt == 0 || carryamt < otmp->quan)
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    Sprintf(pbuf, "%s cannot carry %s.", noittame_Monnam(mtmp), yname(otmp));
                    pline_ex1_popup(ATR_NONE, CLR_MSG_FAIL, pbuf, "Cannot Carry Item", TRUE);
                }
                else
                {
                    if (release_item_from_hero_inventory(otmp))
                    {
                        n_given++;
                        
                        if (flags.verbose)
                        {
                            play_simple_object_sound_at_location(otmp, u.ux, u.uy, OBJECT_SOUND_TYPE_GIVE);
                            Sprintf(pbuf, "You give %s to %s.", doname(otmp), noittame_mon_nam(mtmp));
                            pline_ex1_popup(ATR_NONE, NO_COLOR, pbuf, "Item Given", TRUE);
                        }

                        boolean abort_pickup = FALSE;
                        if (*u.ushops && otmp->unpaid)
                        {
                            check_shop_obj(otmp, mtmp->mx, mtmp->my, FALSE, FALSE);
                            if (costly_spot(mtmp->mx, mtmp->my))
                            {
                                struct monst* shkp = shop_keeper(inside_shop(mtmp->mx, mtmp->my));
                                if (shkp)
                                {
                                    play_voice_shopkeeper_simple_line(shkp, SHOPKEEPER_LINE_DROP_THAT_NOW);
                                    pline("%s shouts:", Monnam(shkp));
                                    verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "Drop that, now!");
                                    if (iflags.using_gui_sounds)
                                        delay_output_milliseconds(1200);
                                    play_monster_unhappy_sound(mtmp, MONSTER_UNHAPPY_SOUND_WHIMPER);
                                    if (iflags.using_gui_sounds)
                                        delay_output_milliseconds(200);
                                    play_object_floor_sound(otmp, OBJECT_SOUND_TYPE_DROP, FALSE);
                                    pline("%s drops %s.", Monnam(mtmp), the(cxname(otmp)));
                                    place_object(otmp, mtmp->mx, mtmp->my);
                                    abort_pickup = TRUE;
                                }
                            }
                        }
                        if (abort_pickup)
                            break;
                        else
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
        pline1(Never_mind);
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

    boolean appritemfound = FALSE;
    for (otmp = invent; otmp; otmp = otmp->nobj)
    {
        if (is_obj_normally_edible(otmp))
        {
            appritemfound = TRUE;
            break;
        }
    }
    if (!appritemfound)
    {
        char fbuf[BUFSZ];
        Sprintf(fbuf, "don't have anything suitable to feed to %s.", mon_nam(mtmp));
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1_popup(fbuf, "Nothing to Feed", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    char qbuf[BUFSIZ] = "";
    Sprintf(qbuf, "What would you like to feed to %s?", noittame_mon_nam(mtmp));

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
            boolean willeat = ((is_tame(mtmp) && (tasty < (otmp->material == MAT_VEGGY ? APPORT : MANFOOD))) || foodmakesfriendly || takesfood);

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
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot pass %s over to %s. You are wearing it.", an(singular(otmp, cxname)), noittame_mon_nam(mtmp));
                }
                else
                {
                    You("offer %s to %s.", an(singular(otmp, cxname)), noittame_mon_nam(mtmp));
                    n_given++;
                    int releasesuccess = TRUE;
                    if (mon_can_move(mtmp) && !mtmp->meating
                        && willeat
                        && (releasesuccess = release_item_from_hero_inventory(otmp)))
                    {
                        /* dog_eat expects a floor object */
                        if (foodmakesfriendly)
                        {
                            if (tamedog(mtmp, otmp, TAMEDOG_NO_FORCED_TAMING, FALSE, 0, TRUE, FALSE))
                                otmp = 0; /* It is gone */
                        }
                        else if (is_tame(mtmp) && has_edog(mtmp))
                        {
                            place_object(otmp, mtmp->mx, mtmp->my);
                            (void)dog_eat(mtmp, otmp, mtmp->mx, mtmp->my, FALSE);
                        }
                        else
                        {
                            place_object(otmp, mtmp->mx, mtmp->my);
                            pline("%s eats %s, but does not seem to appreciate it much.", noittame_Monnam(mtmp), the(cxname(otmp)));
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
                            pline("%s does not seem to be able to move in order to eat %s.", noittame_Monnam(mtmp), the(singular(otmp, cxname)));
                        else if (mtmp->meating)
                            pline("%s is already eating something else.", noittame_Monnam(mtmp));
                        else if (!releasesuccess)
                            ; /* Nothing here */
                        else
                            pline("%s refuses to eat %s.", noittame_Monnam(mtmp), the(singular(otmp, cxname)));

                    }
                }
            }
        }
        bypass_objlist(invent, FALSE); /* reset invent to normal */
        free((genericptr_t)pick_list);
    }
    else
    {
        pline1(Never_mind);
    }

    return (n_given > 0);
}

STATIC_OVL int
do_chat_uncurse_items(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    char nbuf[BUFSIZ];
    menu_item* pick_list = (menu_item*)0;
    winid win;
    anything any;
    int cnt = 0;

    any = zeroany;
    win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_CHAT_MENU, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
    start_menu_ex(win, GHMENU_STYLE_CHAT);

    boolean itemfound = FALSE;
    struct obj* otmp;
    for (otmp = invent; otmp; otmp = otmp->nobj)
    {
        if (otmp->otyp == POT_WATER)
        {
            itemfound = TRUE;
            break;
        }
    }

    if (itemfound)
    {
        any = zeroany;
        any.a_int = 1;
        Sprintf(nbuf, "Cast a bless on %s item", s_suffix(mon_nam(mtmp)));
        add_menu(win, POT_WATER + GLYPH_OBJ_OFF, &any, 0, 0, ATR_NONE, NO_COLOR, "Dip an item in a potion of water", MENU_UNSELECTED);
        cnt++;
    }

    itemfound = FALSE;
    for (otmp = invent; otmp; otmp = otmp->nobj)
    {
        if (otmp->otyp == SCR_REMOVE_CURSE)
        {
            itemfound = TRUE;
            break;
        }
    }

    if (objects[SCR_REMOVE_CURSE].oc_name_known && itemfound && mon_is_literate(mtmp))
    {
        any = zeroany;
        any.a_int = 2;
        Sprintf(nbuf, "Ask %s to read a scroll of remove curse", mon_nam(mtmp));
        add_menu(win, SCR_REMOVE_CURSE + GLYPH_OBJ_OFF, &any, 0, 0, ATR_NONE, NO_COLOR, nbuf, MENU_UNSELECTED);
        cnt++;
    }

    int spellid = -1;
    int i;
    for (i = 0; i < MAXSPELL; i++)
        if (spellid(i) == NO_SPELL)
            break;
        else if (spellid(i) == SPE_BLESS)
        {
            spellid = i;
            break;
        }

    if (spellid >= 0)
    {
        any = zeroany;
        any.a_int = 3;
        Sprintf(nbuf, "Cast a bless spell on %s item", s_suffix(mon_nam(mtmp)));
        add_menu(win, SPE_BLESS - FIRST_SPELL + GLYPH_SPELL_TILE_OFF, &any, 0, 0, ATR_NONE, NO_COLOR, nbuf, MENU_UNSELECTED);
        cnt++;
    }

    struct monst* priest = 0;
    int x, y;
    for (x = u.ux - 1; x <= u.ux + 1; x++)
    {
        for (y = u.uy - 1; y <= u.uy + 1; y++)
        {
            if (x == u.ux && y == u.uy)
                continue;

            if (isok(x, y))
            {
                struct monst* mon = m_at(x, y);
                if (mon && !DEADMONSTER(mon))
                {
                    if (mon->ispriest && has_epri(mon) && is_peaceful(mon))
                    {
                        priest = mon;
                    }
                }
            }
            if (priest)
                break;
        }
        if (priest)
            break;
    }

    if (priest)
    {
        any = zeroany;
        any.a_int = 4;
        Sprintf(nbuf, "Ask %s to bless %s item", mon_nam(priest), s_suffix(mon_nam(mtmp)));
        add_menu(win, any_mon_to_glyph(priest, rn2_on_display_rng), &any, 0, 0, ATR_NONE, NO_COLOR, nbuf, MENU_UNSELECTED);
        cnt++;
    }


    /* Finish the menu */
    end_menu(win, "How do you want to uncurse or bless items?");


    if (cnt <= 0)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        char errbuf[BUFSZ];
        Sprintf(errbuf, "You don't have any means to uncurse or bless %s items.", s_suffix(mon_nam(mtmp)));
        pline_ex1_popup(ATR_NONE, CLR_MSG_FAIL, errbuf, "Nothing to Uncurse or Bless", TRUE);
        destroy_nhwindow(win);
        return 0;
    }

    int selidx = 0;
    /* Now generate the menu */
    if (select_menu(win, PICK_ONE, &pick_list) > 0)
    {
        selidx = pick_list->item.a_int;
        free((genericptr_t)pick_list);
        pick_list = 0;
    }
    destroy_nhwindow(win);

    char qbuf[BUFSZ];
    int n;
    switch (selidx)
    {
    case 1:
        Strcpy(qbuf, "Which potion would you like to dip items into?");
        n = query_objlist(qbuf, &invent, (SIGNAL_NOMENU | SIGNAL_ESCAPE | USE_INVLET | INVORDER_SORT),
            &pick_list, PICK_ONE, is_potion_of_water, 0);
        if (n && pick_list && pick_list[0].item.a_obj)
        {
            otmp = pick_list[0].item.a_obj;
            free((genericptr_t)pick_list);
            pick_list = 0;
            Sprintf(qbuf, "What would you like to dip into %s?", the(cxname(otmp)));
            n = query_objlist(qbuf, &mtmp->minvent, (SIGNAL_NOMENU | SIGNAL_ESCAPE | USE_INVLET | INVORDER_SORT),
                &pick_list, PICK_ONE, allow_all, 0);
            if (n && pick_list && pick_list[0].item.a_obj)
            {
                struct obj* obj = pick_list[0].item.a_obj;
                free((genericptr_t)pick_list);
                pick_list = 0;
                const char* obj_glows = Yobjnam2(obj, "glow");
                if (H2Opotion_dip(otmp, obj, TRUE, obj_glows))
                {
                    useup(otmp);
                    return 1;
                }
            }
        }
        break;
    case 2:
        Sprintf(qbuf, "Which scroll would you like to have %s read?", mon_nam(mtmp));
        n = query_objlist(qbuf, &invent, (SIGNAL_NOMENU | SIGNAL_ESCAPE | USE_INVLET | INVORDER_SORT),
            &pick_list, PICK_ONE, is_scroll_of_remove_curse, 0);
        if (n && pick_list && pick_list[0].item.a_obj)
        {
            otmp = pick_list[0].item.a_obj;
            free((genericptr_t)pick_list);
            pick_list = 0;
            if (otmp->otyp == SCR_REMOVE_CURSE)
            {
                (void)remove_curse(otmp, mtmp, !!mtmp->mprops[CONFUSION]);
                useup(otmp);
                otmp = 0;
            }
        }
        break;
    case 3:
        if(spellid >= 0)
            spelleffects(spellid, FALSE, mtmp);
        break;
    case 4:
        if (priest)
        {
            iflags.spell_target_monster = mtmp;
            do_chat_priest_blesscurse(priest);
            iflags.spell_target_monster = 0;
        }
        break;
    default:
        break;
    }

    return 0;
}
STATIC_OVL int
do_chat_quaff(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    int n, i, n_given = 0;
    long cnt;
    struct obj* otmp, * otmp2;
    menu_item* pick_list;

    boolean appritemfound = FALSE;
    for (otmp = invent; otmp; otmp = otmp->nobj)
    {
        if (otmp->oclass == POTION_CLASS)
        {
            appritemfound = TRUE;
            break;
        }
    }
    if (!appritemfound)
    {
        char fbuf[BUFSZ];
        Sprintf(fbuf, "don't have anything suitable to give to %s to drink.", mon_nam(mtmp));
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1_popup(fbuf, "Nothing to Give to Drink", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    char qbuf[BUFSIZ] = "";
    Sprintf(qbuf, "What would you like to give to %s to drink?", noittame_mon_nam(mtmp));

    add_valid_menu_class(0); /* clear any classes already there */
    add_valid_menu_class(POTION_CLASS);

    n = query_objlist(qbuf, &invent,
        (USE_INVLET | INVORDER_SORT), &pick_list, PICK_ONE,
        allow_category, 3);

    boolean res = 0;
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

            int tasty = 0;
            if (set_defensive_potion(mtmp, otmp))
            {
                tasty = 1;
            }
            else if (set_misc_potion(mtmp, otmp))
            {
                tasty = 2;
            }
            boolean willquaff = tasty > 0;
            if (cnt < otmp->quan)
            {
                if (welded(otmp, &youmonst)
                    || !willquaff
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

                    //Need to reset set_defensive/misc
                    if (tasty == 1)
                    {
                        (void)set_defensive_potion(mtmp, otmp);
                    }
                    else if (tasty == 2)
                    {
                        (void)set_misc_potion(mtmp, otmp);
                    }
                }
            }

            /* Drink here */
            if (otmp)
            {
                if (otmp->owornmask & (W_ARMOR | W_ACCESSORY))
                {
                    play_sfx_sound(SFX_GENERAL_CANNOT);
                    You_ex(ATR_NONE, CLR_MSG_FAIL, "cannot pass %s over to %s. You are wearing it.", an(singular(otmp, cxname)), noittame_mon_nam(mtmp));
                }
                else
                {
                    You("offer %s to %s.", an(singular(otmp, cxname)), noittame_mon_nam(mtmp));
                    n_given++;
                    int releasesuccess = TRUE;
                    if (mon_can_move(mtmp) && !mtmp->meating
                        && willquaff
                        && (releasesuccess = release_item_from_hero_inventory(otmp)))
                    {
                        if (*u.ushops || otmp->unpaid)
                            check_shop_obj(otmp, mtmp->mx, mtmp->my, FALSE, FALSE);
                        (void)mpickobj(mtmp, otmp);
                        if (tasty == 1)
                            res |= use_defensive(mtmp);
                        else if (tasty == 2)
                            res |= use_misc(mtmp);

                        /* otmp is now gone! */
                        otmp = 0;
                    }
                    else
                    {
                        if (!mon_can_move(mtmp))
                            pline("%s does not seem to be able to move in order to drink %s.", noittame_Monnam(mtmp), the(singular(otmp, cxname)));
                        else if (mtmp->meating)
                            pline("%s is eating something and cannot drink at the same time.", noittame_Monnam(mtmp));
                        else if (!releasesuccess)
                            ; /* Nothing here */
                        else
                            pline("%s refuses to drink %s.", noittame_Monnam(mtmp), the(singular(otmp, cxname)));

                    }
                }
            }
        }
        bypass_objlist(invent, FALSE); /* reset invent to normal */
        free((genericptr_t)pick_list);
    }
    else
    {
        pline1(Never_mind);
    }

    return (n_given > 0) && res;
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
                You_ex(ATR_NONE, CLR_MSG_WARNING, "try to remove %s from %s, but you can't. It's cursed!", cxname(otmp), noittame_mon_nam(mtmp));
            else
                pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s tries to takes off %s, but can't. It's cursed!", noittame_Monnam(mtmp), cxname(otmp));

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
                You("remove %s from %s.", cxname(otmp), noittame_mon_nam(mtmp));
            else
                pline("%s takes off %s.", noittame_Monnam(mtmp), cxname(otmp));
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

    boolean wielded = mon_wield_item(mtmp, TRUE, 0, 0);
    return wielded;
}

STATIC_OVL int
do_chat_pet_dowield_ranged(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    mtmp->weapon_strategy = NEED_RANGED_WEAPON;
    boolean wielded = mon_wield_item(mtmp, TRUE, 0, 0);
    return wielded;
}

STATIC_OVL int
do_chat_pet_dowield_pickaxe(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    mtmp->weapon_strategy = NEED_PICK_AXE;
    boolean wielded = mon_wield_item(mtmp, TRUE, 0, 0);
    return wielded;

}

STATIC_OVL int
do_chat_pet_dowield_axe(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    mtmp->weapon_strategy = NEED_AXE;
    boolean wielded = mon_wield_item(mtmp, TRUE, 0, 0);
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
            pline_ex(ATR_NONE, CLR_MSG_WARNING, "%s tries to unwield %s, but can't. It's cursed!", noittame_Monnam(mtmp), cxname(mwep));
            mwep->bknown = TRUE;
        }
        else
        {
            setmnotwielded(mtmp, mwep);
            mtmp->weapon_strategy = NEED_WEAPON;
            pline("%s unwields %s.", noittame_Monnam(mtmp), cxname(mwep));
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
    long u_pay;
    long base_join_cost = 20 + 5 * mtmp->data->difficulty * mtmp->data->difficulty;
    int ucha = ACURR(A_CHA);
    long join_cost = (base_join_cost * max(10, (100 - (ucha - 8) * 5))) / 100;
    char qbuf[QBUFSZ];

    if((mtmp->data->mflags6 & M6_PEACEFUL_AUTO_JOIN) != 0 && is_peaceful(mtmp))
        join_cost = 0;

    multi = 0;
    umoney = money_cnt(invent);


    if (!m_general_talk_check(mtmp, "joining") || !m_speak_check(mtmp)) 
    {
        return 0;
    }
    else if (is_tame(mtmp)) 
    {
        if(mtmp->ispartymember)
            pline("%s is already in your party.", noittame_Monnam(mtmp));
        else
            pline("%s is already following you.", noittame_Monnam(mtmp));
        return 0;
    }

    if (Deaf)
    {
        char jbuf[BUFSZ];
        Sprintf(jbuf, "%s says something but you cannot hear anything.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, jbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
        return 1;
    }

    /*
    else if (!umoney) {
        You("have no money.");
        return 0;
    }
    */
    if (!join_cost)
    {
        Sprintf(qbuf, "%s is willing to join you for free. Do you accept?", noittame_Monnam(mtmp));
    }
    else if (is_undead(mtmp->data) || is_demon(mtmp->data) || (mtmp->data->maligntyp < 0 && mtmp->data->difficulty > 10) )
    {
        pline("%s first %s, but then says:", noittame_Monnam(mtmp), mtmp->data->msound == MS_MUMBLE ? "mumbles incomprehensibly" : "chuckles");
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_I_CAN_JOIN_YOU_AGGRESSIVE);
        Sprintf(qbuf, "\"You shall pay me a tribute of %ld %s.\" Do you yield to this demand?", join_cost, currency(join_cost));
    }
    else
    {
        pline("%s looks at you and replies:", noittame_Monnam(mtmp));
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_I_CAN_JOIN_YOU);
        Sprintf(qbuf, "\"I can join you for a fee of %ld %s. Is this acceptable?\"", join_cost, currency(join_cost));
    }
    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (join_cost > 0)
        {
            if (umoney < join_cost) 
            {
                play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
                return 0;
            }
            u_pay = join_cost;

            money2mon(mtmp, u_pay);
            bot();
        }

        boolean success = tamedog(mtmp, (struct obj*)0, TAMEDOG_FORCE_ALL, FALSE, 0, FALSE, FALSE);
        if (success)
        {
            mtmp->ispartymember = TRUE;
            play_sfx_sound(SFX_TAMING);
            char jbuf[BUFSZ];
            Sprintf(jbuf, "%s joins your party!", noittame_Monnam(mtmp));
            popup_talk_line_ex(mtmp, jbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        }
        else if (!is_tame(mtmp))
        {
            play_sfx_sound(SFX_SURPRISE_ATTACK);
            char jbuf[BUFSZ];
            Sprintf(jbuf, "%s takes your money but refuses join your party after all!", noittame_Monnam(mtmp));
            popup_talk_line_ex(mtmp, jbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
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
    long u_pay;
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

    if (Deaf)
    {
        char jbuf[BUFSZ];
        Sprintf(jbuf, "%s says something but you cannot hear anything.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, jbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
        return 1;
    }

    if (is_undead(mtmp->data) || is_demon(mtmp->data) || (mtmp->data->maligntyp < 0 && mtmp->data->difficulty > 10))
    {
        pline("%s first %s, but then says:", noittame_Monnam(mtmp), mtmp->data->msound == MS_MUMBLE ? "mumbles incomprehensibly" : "chuckles");
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_I_CAN_EXPLAIN_MY_STATISTICS_AGGRESSIVE);
        Sprintf(qbuf, "\"You shall pay me %ld %s for learning my statistics.\" Do you accept?", explain_cost, currency(explain_cost));
    }
    else
    {
        pline("%s looks at you and then says:", noittame_Monnam(mtmp));
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_LINE_I_CAN_EXPLAIN_MY_STATISTICS);
        Sprintf(qbuf, "\"I can explain my statistics to you for a fee of %ld %s.\" Do you accept?", explain_cost, currency(explain_cost));
    }
    switch (yn_query_mon(mtmp, qbuf))
    {
    default:
        return 0;
    case 'y':
        if (umoney < explain_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = explain_cost;
        play_sfx_sound(SFX_READ);
        money2mon(mtmp, u_pay);
        bot();
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
    struct obj* otmp;
    int i;

    menu_item* pick_list = (menu_item*)0;
    winid win;
    anything any;

    any = zeroany;
    win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_CHAT_ITEM_MENU, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
    start_menu_ex(win, GHMENU_STYLE_OTHERS_INVENTORY);

    
    static const char def_srt_order[MAX_OBJECT_CLASSES] = {
    COIN_CLASS, AMULET_CLASS, MISCELLANEOUS_CLASS, RING_CLASS, WAND_CLASS, POTION_CLASS,
    SCROLL_CLASS, SPBOOK_CLASS, GEM_CLASS, FOOD_CLASS, REAGENT_CLASS, TOOL_CLASS,
    WEAPON_CLASS, ARMOR_CLASS, ART_CLASS, ROCK_CLASS, BALL_CLASS, CHAIN_CLASS, 0,
    };

    const char* classorder = flags.sortpack ? flags.inv_order : def_srt_order;
    boolean classhasitems[MAX_OBJECT_CLASSES] = { 0 };

    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
    {
        if (otmp->oclass > ILLOBJ_CLASS)
            classhasitems[(int)otmp->oclass] = TRUE;
    }

    for(i = 0; i < MAX_OBJECT_CLASSES; i++)
    {
        char oclass = classorder[i];
        boolean madeheader = FALSE;

        if (flags.sortpack && !classhasitems[(int)oclass])
            continue;

        for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
        {
            if ((!flags.sortpack || (flags.sortpack && otmp->oclass == oclass)) && m_sellable_item(otmp, mtmp))
            {
                if (flags.sortpack && !madeheader)
                {
                    madeheader = TRUE;
                    any = zeroany;

                    add_extended_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings, NO_COLOR,
                        get_class_name(oclass), MENU_UNSELECTED, menu_group_heading_info(def_oc_syms[(int)oclass].sym));
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

                int glyph = obj_to_glyph(otmp, rn2_on_display_rng);
                int gui_glyph = maybe_get_replaced_glyph(glyph, mtmp->mx, mtmp->my, data_to_replacement_info(glyph, LAYER_OBJECT, otmp, (struct monst*)0, 0UL, 0UL, 0UL, MAT_NONE, 0));
                add_extended_menu(win, gui_glyph, & any, 
                    0, 0, ATR_NONE, NO_COLOR,
                    itembuf, MENU_UNSELECTED, obj_to_extended_menu_info(otmp));

                sellable_item_count++;

            }
        }
        if (!flags.sortpack)
            break;
    }

    /* Finish the menu */
    char moneybuf[BUFSZ];
    long umoney = money_cnt(invent);
    Sprintf(moneybuf, "You have %ld %s.", umoney, currency(umoney));
    char* txt = 0;
#ifdef GNH_MOBILE
    txt = moneybuf;
#endif
    end_menu_ex(win, "What do you want to buy?", txt);

    if (sellable_item_count <= 0)
    {
        pline("%s doesn't have anything to sell.", noittame_Monnam(mtmp));
        destroy_nhwindow(win);
        return 0;
    }
    else
    {
        if (!Deaf && is_speaking(mtmp->data) && !mtmp->isshk && (is_undead(mtmp->data) || is_demon(mtmp->data) || (mtmp->data->maligntyp < 0 && mtmp->data->difficulty > 10) ))
        {
            if(is_undead(mtmp->data) || is_demon(mtmp->data))
                play_monster_item_trading_line(mtmp, MONSTER_ITEM_TRADING_LINE_TRADING_GREETINGS_MORTAL);
            play_monster_item_trading_line(mtmp, sellable_item_count == 1 ? MONSTER_ITEM_TRADING_LINE_TRADING_IM_WILLING_TO_TRADE_THE_FOLLOWING_ITEM : MONSTER_ITEM_TRADING_LINE_TRADING_IM_WILLING_TO_TRADE_THE_FOLLOWING_ITEMS);
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "%sI'm willing to trade the following item%s.", is_undead(mtmp->data) || is_demon(mtmp->data) ? "Greetings, mortal. " : "", plur(sellable_item_count));
            play_monster_item_trading_line(mtmp, MONSTER_ITEM_TRADING_LINE_BUT_BE_QUICK_MY_PATIENCE_IS_LIMITED);
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "But be quick, my patience is limited.");
        }
        else if (!Deaf && is_speaking(mtmp->data))
        {
            char itembuf[BUFSZ];
            if (sellable_item_count == 1 && !iflags.using_gui_sounds && (otmp = get_first_sellable_item(mtmp)) != 0)
                Strcpy(itembuf, cxname(otmp));
            else if (sellable_item_count == 1)
                Strcpy(itembuf,"item");
            else
                Strcpy(itembuf, "items");

            play_monster_item_trading_line(mtmp, sellable_item_count == 1 ? MONSTER_ITEM_TRADING_LINE_TRADING_MAY_I_INTEREST_YOU_IN_FOLLOWING_ITEM : MONSTER_ITEM_TRADING_LINE_TRADING_MAY_I_INTEREST_YOU_IN_FOLLOWING_ITEMS);
            verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "Hello, adventurer! May I interest you in the following %s?", itembuf);
        }
        else
            pline("%s shows you %s merchandise.", noittame_Monnam(mtmp), mhis(mtmp));
    }

    /* Now generate the menu */
    int pick_count = 0;
    int buy_count = 0;
    if ((pick_count = select_menu(win, PICK_ANY, &pick_list)) > 0)
    {
        boolean itemized = TRUE;
        if (pick_count > 1)
            itemized = (yn_query("Itemized billing?") == 'y');

        for (i = 0; i < pick_count; i++)
        {
            struct obj* item_to_buy = pick_list[i].item.a_obj;
            long quan = pick_list[i].count == -1 ? item_to_buy->quan : min(item_to_buy->quan, pick_list[i].count);
            if (item_to_buy && quan > 0)
            {
                long item_cost = quan * get_cost_of_monster_item(item_to_buy, mtmp);
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
                        if (umoney < item_cost) {
                            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
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
                    if (umoney < item_cost) {
                        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                        You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough money for %s!", cxname(item_to_buy));
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

                    money2mon(mtmp, item_cost);
                    umoney = money_cnt(invent);
                    if(itemized)
                        bot();

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
        if(!itemized)
            bot();

        if (is_peaceful(mtmp) && buy_count > 0) 
        {
            play_sfx_sound(SFX_BUY_FROM_NPC);
            if (!Deaf && !mtmp->isshk && (is_undead(mtmp->data) || is_demon(mtmp->data) || (mtmp->data->maligntyp < 0 && mtmp->data->difficulty > 10)))
            {
                play_monster_item_trading_line(mtmp, MONSTER_ITEM_TRADING_LINE_TRADING_USE_YOUR_PURCHASE_WELL);
                popup_talk_line_ex(mtmp, "Use your purchase well!", ATR_NONE, NO_COLOR, TRUE, TRUE);
                //verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "Use your purchase well!");
            }
            else if (!Deaf && (is_speaking(mtmp->data) || (mtmp->isshk && !muteshk(mtmp))))
            {
                play_monster_item_trading_line(mtmp, MONSTER_ITEM_TRADING_LINE_TRADING_THANK_YOU_FOR_YOUR_PURCHASE);
                popup_talk_line_ex(mtmp, "Thank you for your purchase!", ATR_NONE, NO_COLOR, TRUE, TRUE);
                //verbalize_ex(ATR_NONE, CLR_MSG_TALK_NORMAL, "Thank you for your purchase!");
            }
            else
            {
                char tbuf[BUFSZ];
                Sprintf(tbuf, "%s nods appreciatively at you for the purchase!", noittame_Monnam(mtmp));
                popup_talk_line_ex(mtmp, tbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
                //pline("%s nods appreciatively at you for the purchase!", Monnam(mtmp));
            }
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
    win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_CHAT_ITEM_MENU, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
    start_menu_ex(win, GHMENU_STYLE_OTHERS_INVENTORY);


    static const char def_srt_order[MAX_OBJECT_CLASSES] = {
    COIN_CLASS, AMULET_CLASS, ART_CLASS, MISCELLANEOUS_CLASS, RING_CLASS, WAND_CLASS, POTION_CLASS,
    SCROLL_CLASS, SPBOOK_CLASS, GEM_CLASS, FOOD_CLASS, REAGENT_CLASS, TOOL_CLASS,
    WEAPON_CLASS, ARMOR_CLASS, ROCK_CLASS, BALL_CLASS, CHAIN_CLASS, 0,
    };

    const char* classorder = flags.sortpack ? flags.inv_order : def_srt_order;
    boolean classhasitems[MAX_OBJECT_CLASSES] = { 0 };
    struct obj* otmp;

    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
    {
        if (otmp->oclass > ILLOBJ_CLASS)
            classhasitems[(int)otmp->oclass] = TRUE;
    }

    int i;
    for (i = 0; i < MAX_OBJECT_CLASSES; i++)
    {
        char oclass = classorder[i];
        boolean madeheader = FALSE;

        if (flags.sortpack && !classhasitems[(int)oclass])
            continue;

        for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
        {
            if ((!flags.sortpack || (flags.sortpack && otmp->oclass == oclass)) && otmp->owornmask == 0)
            {
                if (flags.sortpack && !madeheader)
                {
                    madeheader = TRUE;
                    any = zeroany;

                    add_extended_menu(win, NO_GLYPH, &any, 0, 0, iflags.menu_headings, NO_COLOR,
                        get_class_name(oclass), MENU_UNSELECTED, menu_group_heading_info(def_oc_syms[(int)oclass].sym));
                }

                any = zeroany;
                char itembuf[BUFSIZ] = "";
                Sprintf(itembuf, "%s", doname(otmp));

                any.a_obj = otmp;
                char let = 'a' + item_count;
                char accel = def_oc_syms[(int)otmp->oclass].sym;

                add_menu(win, NO_GLYPH, &any,
                    let, accel, ATR_NONE, NO_COLOR,
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
        pline("%s doesn't have any items.", noittame_Monnam(mtmp));
        destroy_nhwindow(win);
        return 0;
    }

    /* Now generate the menu */
    int pick_count = 0;
    int take_count = 0;
    if ((pick_count = select_menu(win, PICK_ANY, &pick_list)) > 0)
    {
        for (i = 0; i < pick_count; i++)
        {
            struct obj* item_to_take = pick_list[i].item.a_obj;
            if (item_to_take)
            {
                if ((objects[item_to_take->otyp].oc_flags & O1_CANNOT_BE_DROPPED_IF_CURSED) && item_to_take->cursed) 
                {
                    play_sfx_sound(SFX_GENERAL_WELDED);
                    pline_ex(ATR_NONE, CLR_MSG_ATTENTION, "%s not leave %s!", Tobjnam(item_to_take, "do"), noittame_mon_nam(mtmp));
                }
                else
                {
                    if (item_to_take->quan > 1 && pick_list[i].count > 0 && pick_list[i].count < item_to_take->quan)
                        item_to_take = splitobj(item_to_take, pick_list[i].count);

                    obj_extract_self(item_to_take);

                    play_simple_object_sound_at_location(item_to_take, mtmp->mx, mtmp->my, OBJECT_SOUND_TYPE_GIVE);
                    You("took %s from %s.", doname(item_to_take), noittame_mon_nam(mtmp));

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
    struct obj* otmp;
    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
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

    struct obj* otmp;
    for (otmp = mtmp->minvent; otmp; otmp = otmp->nobj)
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
        && ((otmp->speflags & SPEFLAGS_GRABBED_FROM_YOU) || mtmp->isnpc || mtmp->issmith ||
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
    long u_pay, 
        bless_cost = max(1L, (long)((300 + 15 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))),
        curse_cost = max(1L, (long)((300 + 15 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    int priest_action = 0;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    struct monst* targetmonst = ask_target_monster(mtmp);
    if(!targetmonst)
        return 0;

    play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_BLESS_AN_ITEM);
    Sprintf(qbuf, "Would you like to bless an item? (%ld %s)", bless_cost, currency(bless_cost));
    switch (ynq_mon(mtmp, qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
            if (umoney < bless_cost) {
                play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
                return 0;
            }
            u_pay = bless_cost;
            priest_action = 1;
            break;
        break;
    case 'n':
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_CURSE_AN_ITEM);
        Sprintf(qbuf, "Then would you like to curse one? (%ld %s)",
            curse_cost, currency(curse_cost));
        if (yn_query_mon(mtmp, qbuf) != 'y')
            return 0;
        if (umoney < curse_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
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

    struct obj pseudo = { 0 };
    pseudo.otyp = otyp;
    pseudo.quan = 20L;
    pseudo.speflags = SPEFLAGS_SERVICED_SPELL;

    boolean effect_happened = 0;
    (void)seffects(&pseudo, &effect_happened, targetmonst);

    if (effect_happened)
    {
        money2mon(mtmp, u_pay);
        bot();
    }

    /* gnostic handled in seffects */

    return 1;
}


STATIC_OVL int
do_chat_priest_normal_healing(mtmp)
struct monst* mtmp;
{

    long umoney = money_cnt(invent);
    long u_pay, extrahealing_cost = max(1L, (long)(50 * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    struct monst* targetmonst = ask_target_monster(mtmp);
    if (!targetmonst)
        return 0;

    play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_STANDARD_HEALING);
    Sprintf(qbuf, "Would you like to have a standard healing? (%ld %s)", extrahealing_cost, currency(extrahealing_cost));
    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < extrahealing_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough money for that!");
            return 0;
        }
        u_pay = extrahealing_cost;
        break;
    }
    money2mon(mtmp, u_pay);
    bot();

    struct obj pseudo = { 0 };
    pseudo.otyp = SPE_EXTRA_HEALING;
    pseudo.quan = 1L;
    pseudo.speflags = SPEFLAGS_SERVICED_SPELL;
    if (targetmonst == &youmonst)
    {
        zapyourself(&pseudo, TRUE);
        u.uconduct.gnostic++;
    }
    else
        bhitm(targetmonst, &pseudo, mtmp);

    return 1;
}

STATIC_OVL int
do_chat_priest_full_healing(mtmp)
struct monst* mtmp;
{

    long umoney = money_cnt(invent);
    long u_pay, fullhealing_cost = max(1L, (long)((250 + 5 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    struct monst* targetmonst = ask_target_monster(mtmp);
    if (!targetmonst)
        return 0;

    play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_FULL_HEALING);
    Sprintf(qbuf, "Would you like to have a full healing? (%ld %s)", fullhealing_cost, currency(fullhealing_cost));
    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < fullhealing_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = fullhealing_cost;
        break;
    }
    money2mon(mtmp, u_pay);
    bot();

    struct obj pseudo = { 0 };
    pseudo.otyp = SPE_FULL_HEALING;
    pseudo.quan = 1L;
    pseudo.speflags = SPEFLAGS_SERVICED_SPELL;
    if (targetmonst == &youmonst)
    {
        zapyourself(&pseudo, TRUE);
        u.uconduct.gnostic++;
    }
    else
        bhitm(targetmonst, &pseudo, mtmp);

    return 1;
}

STATIC_OVL int
do_chat_priest_cure_sickness(mtmp)
struct monst* mtmp;
{

    long umoney = money_cnt(invent);
    long u_pay, cure_sickness_cost = max(1L, (long)(100 * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    struct monst* targetmonst = ask_target_monster(mtmp);
    if (!targetmonst)
        return 0;

    play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_SICKNESS_CURED);
    Sprintf(qbuf, "Would you like to have your sickness cured? (%ld %s)", cure_sickness_cost, currency(cure_sickness_cost));
    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < cure_sickness_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = cure_sickness_cost;
        break;
    }
    money2mon(mtmp, u_pay);
    bot();

    struct obj pseudo = { 0 };
    pseudo.otyp = SPE_CURE_SICKNESS;
    pseudo.quan = 1L;
    pseudo.speflags = SPEFLAGS_SERVICED_SPELL;
    if (targetmonst == &youmonst)
    {
        zapyourself(&pseudo, TRUE);
        u.uconduct.gnostic++;
    }
    else
        bhitm(targetmonst, &pseudo, mtmp);

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
    long u_pay,
        major_cost = max(1L, (long)((2500 + 150 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))),
        minor_cost = max(1L, (long)((500 + 25 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    int priest_action = 0;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "taking your donation") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_MAJOR_CONTRIBUTION);
    Sprintf(qbuf, "Would you like to make a major contribution for the temple? (%ld %s)", major_cost, currency(major_cost));
    switch (ynq_mon(mtmp, qbuf)) {
    default:
    case 'q':
        return 0;
    case 'y':
        if (umoney < major_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = major_cost;
        priest_action = 1;
        break;
        break;
    case 'n':
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_MINOR_DONATION);
        Sprintf(qbuf, "Then would you like to make a minor donation instead? (%ld %s)",
            minor_cost, currency(minor_cost));
        if (yn_query_mon(mtmp, qbuf) != 'y')
            return 0;
        if (umoney < minor_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = minor_cost;
        priest_action = 2;
        break;
    }


    money2mon(mtmp, u_pay);
    bot();

    boolean coaligned = p_coaligned(mtmp);
    boolean strayed = (u.ualign.record < 0);

    if (priest_action == 2)
    {
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_PIOUS_INDIVIDUAL);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "Thou art indeed a pious individual.");
        if (coaligned && u.ualign.record <= ALGN_SINNED)
        {
            play_sfx_sound(SFX_ALTAR_ADD_ALIGNMENT);
            adjalign(1);
        }
        play_sfx_sound(SFX_PRAY_BLESS_WATER);
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_BESTOW_BLESSING);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "I bestow upon thee a blessing.");
        incr_itimeout(&HClairvoyant, rn1(500, 500));
        refresh_u_tile_gui_info(TRUE);
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
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_DEVOTION_REWARDED);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "Thy devotion has been rewarded.");
        if (u.ublessed == 0)
            u.ublessed = rnd(3);
        else
            u.ublessed++;
    }
    else
    {
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_GENEROSITY_APPRECIATED);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "Thy selfless generosity is deeply appreciated.");
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
    long u_pay, divination_cost = max(1L, (long)(25 * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_WOULD_YOU_LIKE_TO_SEE_YOUR_FORTUNE);
    Sprintf(qbuf, "Would you like to see your fortune? (%ld %s)", divination_cost, currency(divination_cost));
    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < divination_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = divination_cost;
        break;
    }
    money2mon(mtmp, u_pay);
    bot();

    u.uconduct.gnostic++;

    char talkbuf[BUFSZ];
    play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_FORTUNE_IS_LIKE);
    popup_talk_line(mtmp, "Very well, then. Let's see what your fortune is like.");

    if (can_pray(FALSE))
    {
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_CAN_SAFELY_PRAY);
        popup_talk_line(mtmp, "First, I see that you can safely pray.");
    }
    else
    {
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_CANNOT_SAFELY_PRAY);
        popup_talk_line(mtmp, "First, you should know that you cannot safely pray.");
        if (u.ugangr)
        {
            play_monster_special_dialogue_line(mtmp, u.ugangr > 6 ? PRIEST_SPECIAL_DIALOGUE_GOD_EXTREMELY_ANGRY : u.ugangr > 3 ? PRIEST_SPECIAL_DIALOGUE_GOD_VERY_ANGRY : PRIEST_SPECIAL_DIALOGUE_GOD_ANGRY );
            Sprintf(talkbuf, "I see that %s is %sangry with you.", iflags.using_gui_sounds ? "your god" : u_gname(), u.ugangr > 6 ? "extremely " : u.ugangr > 3 ? "very " : "");
            popup_talk_line(mtmp, talkbuf);
        }

        if (u.uprayer_timeout > 0)
        {
            if (iflags.using_gui_sounds)
            {
                play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_PRAYER_CONDUCT_NUMBER);
                Sprintf(talkbuf, "For your prayer conduct, a number appears before me. (The number appears to be %d.)", u.uprayer_timeout / 10 + 1);
                popup_talk_line(mtmp, talkbuf);
            }
            else
            {
                Sprintf(talkbuf, "For your prayer conduct, the number %d appears before me.", u.uprayer_timeout / 10 + 1);
                popup_talk_line(mtmp, talkbuf);
            }

            if (u.uprayer_timeout > 300)
            {
                play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_GOD_TIRED_OF_WHINING);
                Sprintf(talkbuf, "I can see that %s is quite tired of your constant whining.", iflags.using_gui_sounds ? "your god" : u_gname());
                popup_talk_line(mtmp, talkbuf);
            }

            if(u.uprayer_timeout >= 50)
                play_monster_special_dialogue_line(mtmp, u.uprayer_timeout < 50 ? PRIEST_SPECIAL_DIALOGUE_WISE_TO_WAIT_LITTLE_LONGER : u.uprayer_timeout > 200 ? PRIEST_SPECIAL_DIALOGUE_WISE_TO_WAIT_LONG_TIME : PRIEST_SPECIAL_DIALOGUE_WISE_TO_WAIT);
            else
                play_monster_special_dialogue_line(mtmp, u.uprayer_timeout < 50 ? PRIEST_SPECIAL_DIALOGUE_MUST_WAIT_LITTLE_LONGER : u.uprayer_timeout > 200 ? PRIEST_SPECIAL_DIALOGUE_MUST_WAIT_LONG_TIME : PRIEST_SPECIAL_DIALOGUE_MUST_WAIT);

            Sprintf(talkbuf, "Thus, %s wait %sbefore bothering %s again.",
                u.uprayer_timeout >= 50 ? "it would be wise to" : "you must",
                u.uprayer_timeout < 50 ? "a little longer " : u.uprayer_timeout > 200 ? "a long time " : "",
                iflags.using_gui_sounds ? "your god" : u_gname());
            popup_talk_line(mtmp, talkbuf);
        }
    }

    char buf1[BUFSZ] = "";
    char buf2[BUFSZ] = "";
    if (!iflags.using_gui_sounds)
        Sprintf(buf1, " of %d", abs(Luck));
    if (iflags.using_gui_sounds)
        Sprintf(buf2, " (This number seems to be %d.)", abs(Luck));

    if (Luck < 0)
    {
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_FORTUNE_NUMBER);
        play_monster_special_dialogue_line(mtmp, abs(Luck) >= 10 ? PRIEST_SPECIAL_DIALOGUE_EXTREMELY_UNLUCKY_NUMBER : abs(Luck) >= 5 ? PRIEST_SPECIAL_DIALOGUE_VERY_UNLUCKY_NUMBER : PRIEST_SPECIAL_DIALOGUE_UNLUCKY_NUMBER);

        Sprintf(talkbuf, "For your fortune, I see a number%s. That is not good, for it is %s unlucky number.%s",
            buf1, abs(Luck) >= 10 ? "an extremely" : abs(Luck) >= 5 ? "a very" : "an", buf2);
        popup_talk_line(mtmp, talkbuf);
    }
    else if (Luck > 0)
    {

        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_FORTUNE_NUMBER);
        play_monster_special_dialogue_line(mtmp, abs(Luck) >= 10 ? PRIEST_SPECIAL_DIALOGUE_EXTREMELY_LUCKY_NUMBER : abs(Luck) >= 5 ? PRIEST_SPECIAL_DIALOGUE_VERY_LUCKY_NUMBER : PRIEST_SPECIAL_DIALOGUE_LUCKY_NUMBER);

        Sprintf(talkbuf, "For your fortune, I see a number%s. That is good, for it is %s lucky number.%s",
            buf1, abs(Luck) >= 10 ? "an extremely" : abs(Luck) >= 5 ? "a very" : "an", buf2);
        popup_talk_line(mtmp, talkbuf);
    }
    else
    {
        play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_FORTUNE_NEUTRAL);
        popup_talk_line(mtmp, "For your fortune, my vision is neutral.");
    }

    play_monster_special_dialogue_line(mtmp, PRIEST_SPECIAL_DIALOGUE_THANK_YOU_FOR_YOUR_INTEREST);
    popup_talk_line(mtmp, "That's all for now. Thank you for your interest in divine matters.");

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
        pline("%s is not a shopkeeper.", noittame_Monnam(mtmp));
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
    if (!mtmp || !has_eshk(mtmp))
        return 0;

    long umoney;
    long minor_id_cost = max(1L, (long)((ESHK(mtmp)->shoptype == SHOPBASE ? 150 + 10 * (double)u.ulevel : 75 + 5 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney) 
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    int res = 0;

    context.shop_identify_type = ESHK(mtmp)->shoptype - SHOPBASE + 1;
    if (count_unidentified(invent, 0, FALSE) == 0)
    {
        context.shop_identify_type = 0;
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1_popup("have nothing to identify.", "Nothing to Identify", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }
    context.shop_identify_type = 0;

    Sprintf(qbuf, "Would you like to identify %s? (%ld %s)", an(shtypes[ESHK(mtmp)->shoptype - SHOPBASE].identified_item_description), minor_id_cost, currency(minor_id_cost));
    switch (yn_query_mon(mtmp, qbuf)) 
    {
    default:
        return 0;
    case 'y':
        if (umoney < minor_id_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        break;
    }

    context.shop_identify_type = ESHK(mtmp)->shoptype - SHOPBASE + 1;
    res = service_identify(mtmp, minor_id_cost);
    context.shop_identify_type = 0;

    //int u_pay;
    //int cnt = 0, unided = 0;
    //do
    //{
    //    if(!cnt)
    //        Sprintf(qbuf, "Would you like to identify %s? (%d %s)", an(shtypes[ESHK(mtmp)->shoptype - SHOPBASE].identified_item_description), minor_id_cost, currency((long)minor_id_cost));
    //    else
    //        Sprintf(qbuf, "Would you like to identify one more %s? (%d %s)", shtypes[ESHK(mtmp)->shoptype - SHOPBASE].identified_item_description, minor_id_cost, currency((long)minor_id_cost));

    //    switch (yn_query_mon(mtmp, qbuf)) {
    //    default:
    //        return 0;
    //    case 'y':
    //        if (umoney < (long)minor_id_cost) {
    //            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
    //            You("don't have enough money for that!");
    //            return 0;
    //        }
    //        u_pay = minor_id_cost;
    //        break;
    //    }

    //    context.shop_identify_type = ESHK(mtmp)->shoptype - SHOPBASE + 1; // shtypes[ESHK(mtmp)->shoptype - SHOPBASE].symb;

    //    res = identify_pack(1, FALSE);
    //    unided = count_unidentified(invent, 0, FALSE);

    //    context.shop_identify_type = 0;

    //    if (res)
    //    {
    //        money2mon(mtmp, (long)u_pay);
    //        context.botl = 1;
    //        umoney = money_cnt(invent);
    //        cnt += res;
    //    }
    //} while (res > 0 && unided > 0 && umoney >= (long)minor_id_cost && cnt < 100); /* Paranoid limit */

    return (res > 0);
}

boolean
is_shop_item_type(otmp, shtype_index)
struct obj* otmp;
int shtype_index;
{
    if (!otmp)
        return FALSE;

    int i;
    for (i = 0; i < SIZE(shtypes[shtype_index].iprobs); i++)
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
    if (!mtmp || !mtmp->isshk || !has_eshk(mtmp))
        return 0;

    long umoney;
    long u_pay;
    long reconcile_cost = max(1L, (long)((1000 + u.ulevel * 100 + (mtmp->mrevived ? u.ulevel * 100 : 0) + max(0, ESHK(mtmp)->robbed + ESHK(mtmp)->debit - ESHK(mtmp)->credit)) * service_cost_charisma_adjustment(ACURR(A_CHA))));
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

    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    bot();

    make_happy_shk(mtmp, FALSE);
    long costapplyingtodebit = max(0, min(reconcile_cost - 1000, ESHK(mtmp)->debit));
    ESHK(mtmp)->debit -= costapplyingtodebit;

    if (is_peaceful(mtmp))
    {
        play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_THATS_A_DEAL);
        popup_talk_line(mtmp, "That's a deal. Be more careful next time.");
    }
    else
    {
        play_voice_shopkeeper_simple_line(mtmp, SHOPKEEPER_LINE_ON_SECOND_THOUGHT);
        popup_talk_line(mtmp, "On second thought, maybe you should hang for your crimes anyway.");
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
    long reconcile_cost = max(1L, (long)((1000 + u.ulevel * 100 + (mtmp->mrevived ? u.ulevel * 100 : 0)) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) {
        There("is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;

    play_monster_special_dialogue_line(mtmp, SMITH_LINE_YOU_NEED_TO_PAY_A_LOT_OF_GOLD_IN_COMPENSATION);
    if (iflags.using_gui_sounds)
        Sprintf(qbuf, "\"You need to pay a lot of gold in compensation.\" (%ld %s in fact!)  Agree?", reconcile_cost, currency(reconcile_cost));
    else
        Sprintf(qbuf, "\"You need to pay %ld %s in compensation. Agree?\"", reconcile_cost, currency(reconcile_cost));

    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    bot();

    mtmp->mpeaceful = 1;
    newsym(mtmp->mx, mtmp->my);

    if (is_peaceful(mtmp))
    {
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_THATS_A_DEAL_BE_MORE_CAREFUL_NEXT_TIME);
        popup_talk_line(mtmp, "That's a deal. Be more careful next time.");
    }
    else
    {
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_ON_SECOND_THOUGHT_MAYBE_YOU_SHOULD_HANG_FOR_YOUR_CRIMES_ANYWAY);
        popup_talk_line(mtmp, "On second thought, maybe you should hang for your crimes anyway.");
    }

    return 1;
}

STATIC_OVL int
do_chat_smith_enchant_armor(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(1L, (long)((1000 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SPE_ENCHANT_ARMOR, 0, "enchant an armor", cost, "enchanting an armor", SMITH_LINE_WOULD_YOU_LIKE_TO_ENCHANT_AN_ARMOR);
}

STATIC_OVL int
do_chat_smith_enchant_weapon(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(1L, (long)((1000 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SPE_ENCHANT_WEAPON, 0, "enchant a weapon", cost, "enchanting a weapon", SMITH_LINE_WOULD_YOU_LIKE_TO_ENCHANT_A_WEAPON);
}

STATIC_OVL int
do_chat_smith_repair_armor(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(1L, (long)((500 + 25 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return general_service_query(mtmp, repair_armor_func, "repair an armor", cost, "repairing an armor", SMITH_LINE_WOULD_YOU_LIKE_TO_REPAIR_AN_ARMOR);
}

STATIC_OVL int
do_chat_smith_repair_weapon(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(1L, (long)((500 + 25 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return general_service_query(mtmp, repair_weapon_func, "repair a weapon", cost, "repairing a weapon", SMITH_LINE_WOULD_YOU_LIKE_TO_REPAIR_A_WEAPON);
}


STATIC_OVL int
do_chat_smith_protect_armor(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(1L, (long)((2000 + 100 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SPE_PROTECT_ARMOR, 0, "protect an armor", cost, "protecting an armor", SMITH_LINE_WOULD_YOU_LIKE_TO_PROTECT_AN_ARMOR);
}

STATIC_OVL int
do_chat_smith_protect_weapon(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(1L, (long)((2000 + 100 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SPE_PROTECT_WEAPON, 0, "protect a weapon", cost, "protecting a weapon", SMITH_LINE_WOULD_YOU_LIKE_TO_PROTECT_A_WEAPON);
}

STATIC_OVL int
do_chat_smith_refill_lantern(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    long cost = max(5L, (long)((max(objects[BRASS_LANTERN].oc_cost, objects[POT_OIL].oc_cost)) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return general_service_query(mtmp, refill_lantern_func, "refill a lamp or lantern", cost, "refilling a lamp or lantern", SMITH_LINE_WOULD_YOU_LIKE_TO_REFILL_A_LAMP_OR_LANTERN);
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
    win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_CHAT_ITEM_MENU, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
    start_menu_ex(win, GHMENU_STYLE_CHAT_CHOOSE_ITEM);

    any = zeroany;
    any.a_char = 1;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge dragon scales into a dragon scale mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 2;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge a shield of reflection", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 3;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge a crystal plate mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 4;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge an adamantium full plate mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 5;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge a mithril full plate mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 6;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge an orichalcum full plate mail", MENU_UNSELECTED);

    /* Finish the menu */
    end_menu(win, "Which type of armor do you want to forge?");

    int i = 0;
    /* Now generate the menu */
    if (select_menu(win, PICK_ONE, &pick_list) > 0)
    {
        i = (int)pick_list->item.a_char;
        free((genericptr_t)pick_list);
    }
    destroy_nhwindow(win);

    if (i < 1)
        return 0;

    long cost = 0;

    switch(i)
    {
    case 1:
        cost = max(1L, (long)((1000 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query(mtmp, forge_dragon_scale_mail_func, "forge a dragon scale mail", cost, "forging a dragon scale mail", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_A_DRAGON_SCALE_MAIL);
        break;
    case 2:
        cost = max(1L, (long)((800 + 80 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_shield_of_reflection_func, "forge a shield of reflection", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "8 nuggets of silver ore", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_A_SHIELD_OF_REFLECTION);
        break;
    case 3:
        cost = max(1L, (long)((600 + 60 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_crystal_plate_mail_func, "forge a crystal plate mail", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "2 dilithium crystals", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_A_CRYSTAL_PLATE_MAIL);
        break;
    case 4:
        cost = max(1L, (long)((600 + 60 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_adamantium_full_plate_mail_func, "forge an adamantium full plate mail", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "4 nuggets of adamantium ore", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_AN_ADAMANTIUM_FULL_PLATE_MAIL);
        break;
    case 5:
        cost = max(1L, (long)((600 + 60 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_mithril_full_plate_mail_func, "forge a mithril full plate mail", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "4 nuggets of mithril ore", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_A_MITHRIL_FULL_PLATE_MAIL);
        break;
    case 6:
        cost = max(1L, (long)((600 + 60 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_orichalcum_full_plate_mail_func, "forge an orichalcum full plate mail", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "4 nuggets of orichalcum ore", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_AN_ORICHALCUM_FULL_PLATE_MAIL);
        break;
    default:
        pline1(Never_mind);
        break;
    }

    return 0;
}

STATIC_OVL int
do_chat_smith_forge_standard_armor(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->issmith || !mtmp->mextra || !ESMI(mtmp))
        return 0;

    menu_item* pick_list = (menu_item*)0;
    winid win;
    anything any;

    any = zeroany;
    win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_GENERAL, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
    start_menu_ex(win, GHMENU_STYLE_CHAT_CHOOSE_ITEM);


    any = zeroany;
    any.a_char = 1;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge a plate mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 2;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge a bronze plate mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 3;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge a field plate mail", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 4;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge a full plate mail", MENU_UNSELECTED);

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

    long cost = 0;

    switch (i)
    {
    case 1:
        cost = max(1L, (long)((100 + 10 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_plate_mail_func, "forge a plate mail", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "4 nuggets of iron ore", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_A_PLATE_MAIL);
        break;
    case 2:
        cost = max(1L, (long)((50 + 5 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_bronze_plate_mail_func, "forge a bronze plate mail", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "4 nuggets of copper ore", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_A_BRONZE_PLATE_MAIL);
        break;
    case 3:
        cost = max(1L, (long)((200 + 20 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_field_plate_mail_func, "forge a field plate mail", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "6 nuggets of iron ore", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_A_FIELD_PLATE_MAIL);
        break;
    case 4:
        cost = max(1L, (long)((400 + 40 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_full_plate_mail_func, "forge a full plate mail", cost, "forging any armor", QUERY_STYLE_COMPONENTS, "8 nuggets of iron ore", SMITH_LINE_WOULD_YOU_LIKE_TO_FORGE_A_FULL_PLATE_MAIL);
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
    return do_chat_npc_general_identify(mtmp, "weapon or armor", -1, max(1L, (long)((double)(75 + 5 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))), SMITH_LINE_WOULD_YOU_LIKE_TO_IDENTIFY_A_WEAPON_OR_ARMOR, SMITH_LINE_WOULD_YOU_LIKE_TO_IDENTIFY_ONE_MORE_WEAPON_OR_ARMOR);
}


STATIC_OVL int
do_chat_npc_forge_sling_bullets(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    menu_item* pick_list = (menu_item*)0;
    winid win;
    anything any;

    any = zeroany;
    win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_GENERAL, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
    start_menu_ex(win, GHMENU_STYLE_CHAT_CHOOSE_ITEM);


    any = zeroany;
    any.a_char = 1;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge 10 iron sling-bullets", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 2;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge 10 exceptional iron sling-bullets", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 3;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge 10 elite iron sling-bullets", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 4;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge 10 silver sling-bullets", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 5;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge 10 exceptional silver sling-bullets", MENU_UNSELECTED);

    any = zeroany;
    any.a_char = 6;

    add_menu(win, NO_GLYPH, &any,
        0, 0, ATR_NONE, NO_COLOR,
        "Forge 10 elite silver sling-bullets", MENU_UNSELECTED);

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

    long cost = 0;

    switch (i)
    {
    case 1:
        cost = max(1L, (long)((double)(objects[SLING_BULLET].oc_cost) * 3 * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_iron_sling_bullets_func, "forge 10 iron sling-bullets", cost, "forging any sling-bullets", QUERY_STYLE_COMPONENTS, "2 nuggets of iron ore", NPC_LINE_WOULD_YOU_LIKE_TO_FORGE_10_IRON_SLING_BULLETS);
        break;
    case 2:
        cost = max(1L, (long)((double)(objects[SLING_BULLET].oc_cost) * 12 * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_ex_iron_sling_bullets_func, "forge 10 exceptional iron sling-bullets", cost, "forging any sling-bullets", QUERY_STYLE_COMPONENTS, "3 nuggets of iron ore", NPC_LINE_WOULD_YOU_LIKE_TO_FORGE_10_EXCEPTIONAL_IRON_SLING_BULLETS);
        break;
    case 3:
        cost = max(1L, (long)((double)(objects[SLING_BULLET].oc_cost) * 48 * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_el_iron_sling_bullets_func, "forge 10 elite iron sling-bullets", cost, "forging any sling-bullets", QUERY_STYLE_COMPONENTS, "4 nuggets of iron ore", NPC_LINE_WOULD_YOU_LIKE_TO_FORGE_10_ELITE_IRON_SLING_BULLETS);
        break;
    case 4:
        cost = max(1L, (long)(((double)objects[SLING_BULLET].oc_cost * material_definitions[MAT_SILVER].cost_multiplier) * 3 * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_silver_sling_bullets_func, "forge 10 silver sling-bullets", cost, "forging any sling-bullets", QUERY_STYLE_COMPONENTS, "2 nuggets of silver ore", NPC_LINE_WOULD_YOU_LIKE_TO_FORGE_10_SILVER_SLING_BULLETS);
        break;
    case 5:
        cost = max(1L, (long)(((double)objects[SLING_BULLET].oc_cost * material_definitions[MAT_SILVER].cost_multiplier) * 12 * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_ex_silver_sling_bullets_func, "forge 10 exceptional silver sling-bullets", cost, "forging any sling-bullets", QUERY_STYLE_COMPONENTS, "3 nuggets of silver ore", NPC_LINE_WOULD_YOU_LIKE_TO_FORGE_10_EXCEPTIONAL_SILVER_SLING_BULLETS);
        break;
    case 6:
        cost = max(1L, (long)(((double)objects[SLING_BULLET].oc_cost * material_definitions[MAT_SILVER].cost_multiplier) * 48 * service_cost_charisma_adjustment(ACURR(A_CHA))));
        return general_service_query_with_extra(mtmp, forge_el_silver_sling_bullets_func, "forge 10 elite silver sling-bullets", cost, "forging any sling-bullets", QUERY_STYLE_COMPONENTS, "4 nuggets of silver ore", NPC_LINE_WOULD_YOU_LIKE_TO_FORGE_10_ELITE_SILVER_SLING_BULLETS);
        break;
    default:
        pline1(Never_mind);
        break;
    }

    return 0;
}

STATIC_OVL int
do_chat_npc_forge_cubic_gate(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    long cost = max(1L, (long)((50 + 5 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return general_service_query_with_extra(mtmp, forge_cubic_gate_func, "forge a cubic gate", cost, "forging any cubic gates", QUERY_STYLE_COMPONENTS, "a dilithium crystal", NPC_LINE_WOULD_YOU_LIKE_TO_FORGE_A_CUBIC_GATE);
}

STATIC_OVL int
do_chat_npc_forge_artificial_wings(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    long cost = max(1L, (long)((500 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return general_service_query_with_extra(mtmp, forge_artificial_wings_func, "forge a pair of artificial wings", cost, "forging any artificial wings", QUERY_STYLE_COMPONENTS, "12 feathers", NPC_LINE_WOULD_YOU_LIKE_TO_FORGE_A_PAIR_OF_ARTIFICIAL_WINGS);
}

STATIC_OVL boolean
maybe_forgeable_ore(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return is_ore(otmp) && otmp->otyp != NUGGET_OF_GOLD_ORE && otmp->otyp != NUGGET_OF_PLATINUM_ORE;
}

STATIC_OVL boolean
maybe_gem(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return (otmp->oclass == GEM_CLASS);
}

STATIC_OVL boolean
maybe_spellbook(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return (otmp->oclass == SPBOOK_CLASS);
}

STATIC_OVL boolean
maybe_dragon_scales(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return !!is_dragon_scales(otmp);
}

boolean
maybe_refillable_with_oil(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return !!is_refillable_with_oil(otmp);
}

STATIC_OVL boolean
maybe_otyp(otmp)
struct obj* otmp;
{
    if (!otmp)
        return FALSE;

    return (otmp->otyp == otyp_for_maybe_otyp);
}


STATIC_OVL int
sell_many_to_npc(mtmp, allow)
struct monst* mtmp;
boolean FDECL((*allow), (OBJ_P)); /* allow function */
{
    int n, n_sold = 0, i;
    long cnt;
    struct obj* otmp, * otmp2;
    menu_item* pick_list = (menu_item*)0;
    boolean all_pressed = FALSE;

    /* should coordinate with perm invent, maybe not show worn items */
    n = query_objlist("What would you like to sell?", &invent,
        (USE_INVLET | INVORDER_SORT | OBJECT_COMPARISON), &pick_list, PICK_ANY, allow, 3);

    if (n > 0 && pick_list)
    {
        /*
         * picklist[] contains a set of pointers into inventory, but
         * as soon as something gets dropped, they might become stale
         * (see the drop_everything code above for an explanation).
         * Just checking to see whether one is still in the invent
         * chain is not sufficient validation since destroyed items
         * will be freed and items we've split here might have already
         * reused that memory and put the same pointer value back into
         * invent.  Ditto for using invlet to validate.  So we start
         * by setting bypass on all of invent, then check each pointer
         * to verify that it is in invent and has that bit set.
         */
        bypass_objlist(invent, TRUE);
        if (n > 1)
            all_pressed = (yn_query("Itemized selling?") != 'y');
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
            if (cnt < otmp->quan)
            {
                if (welded(otmp, &youmonst))
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
            int sell_res = sell_to_npc(otmp, mtmp, n - i, all_pressed);
            if(sell_res > 0 || sell_res == -1)
                n_sold += 1;
            if (sell_res == -1 || sell_res == -2)
                all_pressed = TRUE;
            if (sell_res == -3)
                break;
        }
        bypass_objlist(invent, FALSE); /* reset invent to normal */
        free((genericptr_t)pick_list);
    }
    return n_sold;
}

STATIC_OVL int
do_chat_smith_sell_ore(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_esmi(mtmp))
        return 0;

    int result = sell_many_to_npc(mtmp, maybe_forgeable_ore);
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

    long service_cost = max(1L, (long)((500 + 25 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));

    long umoney = money_cnt(invent);
    long u_pay;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, "opening a branch portal") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    play_monster_special_dialogue_line(mtmp, NPC_LINE_WOULD_YOU_LIKE_TO_OPEN_A_BRANCH_PORTAL);
    Sprintf(qbuf, "Would you like to %s? (%ld %s)", "open a branch portal", service_cost, currency(service_cost));
    switch (yn_query_mon(mtmp, qbuf))
    {
    default:
        return 0;
    case 'y':
        if (umoney < service_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = service_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    pline("%s opens a branch portal for you.", noittame_Monnam(mtmp));
    int portal_res = create_portal();
    if (!portal_res)
    {
        money2u(mtmp, u_pay);
        pline1(Never_mind);
        return 0;
    }
    else
        mtmp = 0;

    bot();

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
        if (otmp->oclass == AMULET_CLASS || otmp->oclass == RING_CLASS || otmp->oclass == MISCELLANEOUS_CLASS || objects[otmp->otyp].oc_charged > CHARGED_NOT_CHARGED)
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
do_chat_npc_sing_song(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !m_speak_check(mtmp) || !mtmp->isnpc || !has_enpc(mtmp))
        return 0;

    const char* linearray[2] = {
        "Hear this song to Elbereth Gilthoniel:",
        0 };

    hermit_talk(mtmp, linearray, GHSOUND_ELVEN_BARD_HEAR_THIS_SONG);

    pline("%s magically summons a large elven harp, and then starts playing.", noittame_Monnam(mtmp));

    struct stop_all_info sainfo = { 0 };
    stop_all_sounds(sainfo);

    struct ghsound_immediate_info immediateinfo = { 0 };
    immediateinfo.ghsound = GHSOUND_SONG_A_ELBERETH_GILTHONIEL;
    immediateinfo.volume = 1.0f;
    immediateinfo.dialogue_mid = mtmp->m_id;
    immediateinfo.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    immediateinfo.play_group = SOUND_PLAY_GROUP_LONG;

    play_immediate_ghsound(immediateinfo);

    winid datawin = create_nhwindow_ex(NHW_TEXT, GHWINDOW_STYLE_PAGER_KEEP_LINE_BREAKS, NO_GLYPH, zerocreatewindowinfo);

    putstr(datawin, 0, "A Elbereth Gilthoniel");
    putstr(datawin, 0, "silivren penna miriel");
    putstr(datawin, 0, "o menel aglar elenath");
    putstr(datawin, 0, "Gilthoniel, A Elbereth");
    putstr(datawin, 0, "");
    putstr(datawin, 0, "A Elbereth Gilthoniel");
    putstr(datawin, 0, "We still remember we who dwell");
    putstr(datawin, 0, "In this far land beneath the trees");
    putstr(datawin, 0, "The starlight on the Western Seas");
    putstr(datawin, 0, "");
    putstr(datawin, 0, "A Elbereth Gilthoniel");
    putstr(datawin, 0, "o menel palan-diriel");
    putstr(datawin, 0, "le nallon si di'n guruthos!");
    putstr(datawin, 0, "A tiro nin, Fanuilos!");
    putstr(datawin, 0, "");
    putstr(datawin, 0, "A Elbereth Gilthoniel");
    putstr(datawin, 0, "We still remember we who dwell");
    putstr(datawin, 0, "In this far land beneath the trees");
    putstr(datawin, 0, "The starlight on the Western Seas");

    display_nhwindow(datawin, TRUE);
    destroy_nhwindow(datawin);
    putmsghistory("[A Elbereth Gilthoniel]", FALSE);

    stop_all_sounds(sainfo);
    update_game_music();

    int distance = mtmp->m_lev * 3;
    struct monst* mtmp2;
    for (mtmp2 = fmon; mtmp2; mtmp2 = mtmp2->nmon) 
    {
        if (DEADMONSTER(mtmp2))
            continue;
        if (dist2(mtmp->mx, mtmp->my, mtmp2->mx, mtmp2->my) < distance && mtmp2->mcanmove)
        {
            if (mtmp2->data->mlet == S_NYMPH && mtmp2->mcanmove
                && dist2(mtmp->mx, mtmp->my, mtmp2->mx, mtmp2->my) < distance)
            {
                mtmp2->msleeping = 0;
                mtmp2->mpeaceful = 1;
                mtmp2->mavenge = 0;
                mtmp2->mstrategy &= ~STRAT_WAITMASK;
                newsym(mtmp2->mx, mtmp2->my);
                if (canseemon(mtmp2))
                    pline(
                        "%s listens cheerfully to the song, then seems quieter.",
                        Monnam(mtmp2));
            }
            else if (!is_peaceful(mtmp2) && !is_tame(mtmp2) && mtmp2->data->mlet != S_HUMAN && !(mtmp2->data->geno & G_UNIQ))
            {
                monflee(mtmp2, 0, FALSE, TRUE);
            }
        }
    }
    return 1;
}

STATIC_OVL int
do_chat_npc_special_hints(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !m_speak_check(mtmp) || !mtmp->isnpc || !has_enpc(mtmp))
        return 0;

    int npctyp = ENPC(mtmp)->npc_typ;
    switch (npctyp)
    {
    case NPC_QUANTUM_MECHANIC:
    {
        const char* linearray[5] = {
            "After years of study, we have come up with the following research results:",
            "First, it is elementary that a wand of teleportation can be used to teleport monsters away.",
            "However, it is less well-known that this works even on a non-teleport level, on which you cannot teleport yourself.",
            "Second, our results indicate that the Amulet of Yendor will prevent its owner from teleporting across dungeon levels.",
            0 };

        hermit_talk(mtmp, linearray, GHSOUND_QUANTUM_TELEPORTATION);
        break;
    }
    case NPC_ELVEN_BARD:
    {
        const char* linearray[6] = {
            "Brave adventurer, I seek to convey to you a great word of protection.",
            "If you engrave the name of the great elven goddess Elbereth, the Starkindler, on the ground, you shall be under her protection.",
            "It takes but a mere round to engrave her name with a finger, athame, or wand.",
            "But be warned: The protection provided by her name will vanish should you attack others.",
            "Also, so great is the power of Moloch in the Under World that she cannot assist you during your journeys there, nor is she willing to grant her protection against elves or humans.",
            0 };
        
        hermit_talk(mtmp, linearray, GHSOUND_ELVEN_BARD_ELBERETH);
        u.uevent.elbereth_known = 1;
        break;
    }
    }
    return 1;
}


STATIC_OVL int
do_chat_quantum_mechanic_research_support(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    long umoney;
    long u_pay;
    long reconcile_cost = max(1L, (long)((mtmp->m_lev * 5 + u.ulevel * 5) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) {
        There("is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;

    play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_RESEARCH_SUPPORT);
    Sprintf(qbuf, "%s asks for a research support of %ld %s.  Agree?", noittame_Monnam(mtmp), reconcile_cost, currency(reconcile_cost));

    switch (yn_query(qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    bot();

    mtmp->mpeaceful = 1;
    newsym(mtmp->mx, mtmp->my);

    play_sfx_sound(SFX_BUY_FROM_NPC);
    if (is_peaceful(mtmp))
    {
        play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_THANK_YOU_FOR_YOUR_SUPPOORT);
        pline("%s thanks you for your support.", noittame_Monnam(mtmp));

    }
    else
    {
        play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_THATS_NOT_WHAT_I_WAS_EXPECTING);
        pline("%s seems mysteriously disappointed.", noittame_Monnam(mtmp));
    }

    return 1;
}

STATIC_OVL int
do_chat_quantum_observe_position(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    long umoney;
    long u_pay;
    long observe_cost = max(1L, (long)((20 + u.ulevel * 5) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) 
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        There_ex(ATR_NONE, CLR_MSG_FAIL, "is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;

    if (mtmp->mspec_used)
    {
        play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_WAVE_FUNCTION_ALREADY_COLLAPSED);
        pline("%s explains something about your wave function having already collapsed.", noittame_Monnam(mtmp));
        pline1("It all sounds pretty serious!");
        return 0;
    }

    if (!is_tame(mtmp))
    {
        play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_OBSERVE_POSITION);
        Sprintf(qbuf, "%s asks for %ld %s to observe your exact position.  Agree?", noittame_Monnam(mtmp), observe_cost, currency(observe_cost));

        switch (yn_query(qbuf)) {
        default:
            return 0;
        case 'y':
            if (umoney < observe_cost) {
                play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
                return 0;
            }
            u_pay = observe_cost;
            break;
        }

        money2mon(mtmp, u_pay);
        bot();
    }

    mtmp->mspec_used = 100;
    refresh_m_tile_gui_info(mtmp, TRUE);
    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "velocity suddenly seems very uncertain!");
    if (rn2(2)) 
    {
        play_sfx_sound(SFX_ACQUIRE_SLOW);
        incr_itimeout(&HSlowed, 50 + d(1, 50));
        refresh_u_tile_gui_info(TRUE);
        You_ex(ATR_NONE, CLR_MSG_NEGATIVE, "seem slower.");
    }
    else 
    {
        play_sfx_sound(SFX_ACQUIRE_HASTE);
        incr_itimeout(!rn2(3) ? &HUltra_fast : !rn2(2) ? &HVery_fast  : &HFast, 50 + d(1, 50));
        refresh_u_tile_gui_info(TRUE);
        You_ex(ATR_NONE, CLR_MSG_POSITIVE, "seem faster.");
    }
    newsym(u.ux, u.uy);

    if (!is_tame(mtmp))
    {
        play_sfx_sound(SFX_BUY_FROM_NPC);
    }
    play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_POSITION_EXACTLY_WHERE_YOU_ARE);
    pline("%s tells that your position was observed to be exactly where you are.", noittame_Monnam(mtmp));

    return 1;
}

STATIC_OVL int
do_chat_quantum_observe_speed(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return 0;

    long umoney;
    long u_pay;
    long observe_cost = max(1L, (long)((20 + u.ulevel * 5) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        There_ex(ATR_NONE, CLR_MSG_FAIL, "is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;

    if (mtmp->mspec_used)
    {
        play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_WAVE_FUNCTION_ALREADY_COLLAPSED);
        pline("%s explains something about your wave function having already collapsed.", noittame_Monnam(mtmp));
        pline1("It all sounds pretty serious!");
        return 0;
    }

    if (!is_tame(mtmp))
    {
        play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_OBSERVE_SPEED);
        Sprintf(qbuf, "%s asks for %ld %s to observe your exact speed.  Agree?", noittame_Monnam(mtmp), observe_cost, currency(observe_cost));

        switch (yn_query(qbuf)) {
        default:
            return 0;
        case 'y':
            if (umoney < observe_cost) {
                play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
                return 0;
            }
            u_pay = observe_cost;
            break;
        }

        money2mon(mtmp, u_pay);
        bot();
    }

    mtmp->mspec_used = 100;
    refresh_m_tile_gui_info(mtmp, TRUE);

    Your_ex(ATR_NONE, CLR_MSG_ATTENTION, "position suddenly seems %suncertain!",
        (Teleport_control && !Stunned && !unconscious()) ? ""
        : "very ");
    tele();

    if (!is_tame(mtmp))
    {
        play_sfx_sound(SFX_BUY_FROM_NPC);
    }
    if (canspotmon(mtmp) && m_canseeu(mtmp))
    {
        play_monster_special_dialogue_line(mtmp, QUANTUM_MECHANIC_LINE_SPEED_IS_ZERO);
        pline("%s tells that your speed was observed to be zero.", noittame_Monnam(mtmp));
    }

    return 1;
}


STATIC_OVL int
do_chat_npc_reconciliation(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    long umoney;
    long u_pay;
    long reconcile_cost = max(1L, (long)((1000 + u.ulevel * 100 + (mtmp->mrevived ? u.ulevel * 100 : 0)) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    char qbuf[QBUFSZ];

    multi = 0;
    umoney = money_cnt(invent);


    if (!mtmp) {
        There("is no one here to talk to.");
        return 0;
    }
    else if (!m_speak_check(mtmp))
        return 0;

    if (iflags.using_gui_sounds)
    {
        play_monster_special_dialogue_line(mtmp, NPC_LINE_YOU_NEED_TO_PAY_A_LOT_OF_GOLD_IN_COMPENSATION);
        Sprintf(qbuf, "\"You need to pay a lot of gold in compensation.\" (%ld %s in fact!)  Agree?", reconcile_cost, currency(reconcile_cost));
    }
    else
    {
        Sprintf(qbuf, "\"You need to pay %ld %s in compensation. Agree?\"", reconcile_cost, currency(reconcile_cost));
    }

    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    bot();

    mtmp->mpeaceful = 1;
    newsym(mtmp->mx, mtmp->my);

    play_sfx_sound(SFX_BUY_FROM_NPC);
    if (is_peaceful(mtmp))
    {
        play_monster_special_dialogue_line(mtmp, NPC_LINE_THATS_A_DEAL_BE_MORE_CAREFUL_NEXT_TIME);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "That's a deal. Be more careful next time.");

    }
    else
    {
        play_monster_special_dialogue_line(mtmp, NPC_LINE_ON_SECOND_THOUGHT_MAYBE_YOU_SHOULD_HANG_FOR_YOUR_CRIMES_ANYWAY);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "On second thought, maybe you should hang for your crimes anyway.");
    }

    return 1;
}

STATIC_OVL int
do_chat_npc_enchant_accessory(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    long cost = max(1L, (long)((1000 + 50 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    return spell_service_query(mtmp, SCR_ENCHANT_ACCESSORY, 0, "enchant an accessory", cost, "enchanting an accessory", NPC_LINE_WOULD_YOU_LIKE_TO_ENCHANT_AN_ACCESSORY);
}

STATIC_OVL int
do_chat_npc_recharge(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    long cost = max(1L, (long)((1200 + 60 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    //return spell_service_query(mtmp, SCR_CHARGING, 0, "recharge an item", cost, "recharging an item", NPC_LINE_WOULD_YOU_LIKE_TO_RECHARGE_AN_ITEM);
    return general_service_query_with_item_cost_adjustment_and_extra(mtmp, recharge_item_func, "charge", "recharging", cost, objects[WAN_WISHING].oc_cost, 50L, "recharging an item", QUERY_STYLE_COMPONENTS, (char*)0, NPC_LINE_WHAT_WOULD_YOU_LIKE_TO_CHARGE);
}


STATIC_OVL int
do_chat_npc_blessed_recharge(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    long cost = max(1L, (long)((4000 + 200 * (double)u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA))));
    //return spell_service_query(mtmp, SCR_CHARGING, 1, "fully recharge an item", cost, "fully recharging an item", NPC_LINE_WOULD_YOU_LIKE_TO_FULLY_RECHARGE_AN_ITEM);
    return general_service_query_with_item_cost_adjustment_and_extra(mtmp, blessed_recharge_item_func, "charge", "fully recharging", cost, objects[WAN_WISHING].oc_cost, 50L, "recharging an item", QUERY_STYLE_COMPONENTS, (char*)0, NPC_LINE_WHAT_WOULD_YOU_LIKE_TO_CHARGE);
}

STATIC_OVL int
do_chat_npc_teach_spells(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !mtmp->isnpc || !mtmp->mextra || !ENPC(mtmp))
        return 0;

    int spell_otyps[MAX_SPECIAL_TEACH_SPELLS + 8 + 1] = { 0 };
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
    /* Spells that are always present are here; random ones are in ENPC(mtmp)->special_teach_spells */
    if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & NPC_SERVICE_TEACH_SPECIAL_SPELLS)
    {
        switch (ENPC(mtmp)->npc_typ)
        {
        case NPC_QUANTUM_MECHANIC:
            spell_otyps[teach_num] = SPE_MAGICAL_IMPLOSION;
            teach_num++;
            spell_otyps[teach_num] = SPE_DISINTEGRATE;
            teach_num++;
            spell_otyps[teach_num] = SPE_SLOW_MONSTER;
            teach_num++;
            break;
        default:
            break;
        }
    }
    if (npc_subtype_definitions[ENPC(mtmp)->npc_typ].service_flags & (NPC_SERVICE_TEACH_SPECIAL_SPELLS | NPC_SERVICE_TEACH_RANDOM_ARCANE_SPELLS))
    {
        int i;
        for (i = 0; i < MAX_SPECIAL_TEACH_SPELLS && ENPC(mtmp)->special_teach_spells[i] > STRANGE_OBJECT; i++)
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

    int spell_otyps[10] = { SPE_MINOR_HEALING, SPE_HEALING, SPE_EXTRA_HEALING, SPE_GREATER_HEALING, 0, 0, 0, 0, 0, 0 };
    switch (EPRI(mtmp)->shralign)
    {
    case A_NONE:
        spell_otyps[4] = SPE_RAISE_SKELETON_WARRIOR;
        spell_otyps[5] = SPE_DRAIN_LEVEL;
        spell_otyps[6] = SPE_SUMMON_DEMON;
        break;
    case A_LAWFUL:
        spell_otyps[4] = SPE_CURE_SICKNESS;
        spell_otyps[5] = SPE_CURE_BLINDNESS;
        spell_otyps[6] = SPE_CELESTIAL_DOVE;
        break;
    case A_NEUTRAL:
        spell_otyps[4] = SPE_STICK_TO_SNAKE;
        spell_otyps[5] = SPE_CREATE_FOOD;
        spell_otyps[6] = SPE_CREATE_CLAY_GOLEM;
        break;
    case A_CHAOTIC:
        spell_otyps[4] = SPE_RAISE_MINOR_ZOMBIE;
        spell_otyps[5] = SPE_RAISE_SKELETON;
        spell_otyps[6] = SPE_REPLENISH_UNDEATH;
        break;
    default:
        break;
    }

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
    long reconcile_cost = max(1L, (long)(500 * service_cost_charisma_adjustment(ACURR(A_CHA))));
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
        play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_YOU_WILL_HANG_FOR_YOUR_CRIMES_SCUM);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "You will hang for your crimes, scum!");
        return 0;
    }
    else if (mtmp->mhp < (3 * mtmp->mhpmax) / 4) {
        pline("%s is in no mood for talking.", noittame_Monnam(mtmp));
        return 0;
    }

    if (iflags.using_gui_sounds)
    {
        play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_WE_CAN_DROP_THE_CASE_FOR_THIS_AMOUNT_OF_GOLD);
        Sprintf(qbuf, "\"We can drop the case for this amount of gold.\" (%ld %s in fact!) Agree?", reconcile_cost, currency(reconcile_cost));
    }
    else
    {
        Sprintf(qbuf, "\"We can drop the case for %ld %s. Agree?\"", reconcile_cost, currency(reconcile_cost));
    }
    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < reconcile_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        u_pay = reconcile_cost;
        break;
    }

    money2mon(mtmp, u_pay);
    bot();

    pacify_guards();

    play_sfx_sound(SFX_BUY_FROM_NPC);
    if (is_peaceful(mtmp))
    {
        play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_FINE_ITS_ALRIGHT_NOW_BE_MORE_CAREFUL_NEXT_TIME);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_HAPPY, "Fine, it's alright now. Be more careful next time.");
    }
    else
    {
        play_monster_special_dialogue_line(mtmp, WATCHMAN_LINE_ON_SECOND_THOUGHT_MAYBE_ILL_HANG_YOU_ANYWAY);
        verbalize_ex(ATR_NONE, CLR_MSG_TALK_ANGRY, "On second thought, maybe I'll hang you anyway.");
    }

    return 1; 
}


STATIC_OVL int
do_chat_npc_identify_gems_and_stones(mtmp)
struct monst* mtmp;
{
    return do_chat_npc_general_identify(mtmp, "gem or stone", 1, 
        max(1L, (long)((double)(100 + 10 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))),
        NPC_LINE_WOULD_YOU_LIKE_TO_IDENTIFY_A_GEM_OR_STONE, NPC_LINE_WOULD_YOU_LIKE_TO_IDENTIFY_ONE_MORE_GEM_OR_STONE);
}


STATIC_OVL int
do_chat_npc_sell_gems_and_stones(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_enpc(mtmp))
        return 0;

    int result = sell_many_to_npc(mtmp, maybe_gem);
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

    //if(objects[otmp->otyp].oc_name_known)
    //    return (otmp->otyp == DILITHIUM_CRYSTAL);

    return maybe_gem(otmp);
}

STATIC_OVL int
do_chat_npc_sell_dilithium_crystals(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !has_enpc(mtmp))
        return 0;

    int result = sell_many_to_npc(mtmp, maybe_dilithium_crystal);
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

    int result = sell_many_to_npc(mtmp, maybe_spellbook);
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
    return do_chat_npc_general_identify(mtmp, "accessory or charged item", 2,
        max(1L, (long)((double)(100 + 10 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))),
        NPC_LINE_WOULD_YOU_LIKE_TO_IDENTIFY_AN_ACCESSORY_OR_CHARGED_ITEM, NPC_LINE_WOULD_YOU_LIKE_TO_IDENTIFY_ONE_MORE_ACCESSORY_OR_CHARGED_ITEM);
}

STATIC_OVL int
do_chat_npc_identify_gems_stones_and_charged_items(mtmp)
struct monst* mtmp;
{

    return do_chat_npc_general_identify(mtmp, "gem, stone or a charged item", 3,
        max(1L, (long)((double)(100 + 10 * u.ulevel) * service_cost_charisma_adjustment(ACURR(A_CHA)))),
        NPC_LINE_WOULD_YOU_LIKE_TO_IDENTIFY_A_GEM_STONE_OR_CHARGED_ITEM, NPC_LINE_WOULD_YOU_LIKE_TO_IDENTIFY_ONE_MORE_GEM_STONE_OR_CHARGED_ITEM);
}

STATIC_OVL int
do_chat_npc_general_identify(mtmp, identify_item_str, id_idx, minor_id_cost, spdialogue1, spdialogue2)
struct monst* mtmp;
const char* identify_item_str;
int id_idx, spdialogue1, spdialogue2 UNUSED;
long minor_id_cost;
{
    if (!mtmp)
        return 0;

    long umoney = money_cnt(invent);

    if (!m_general_talk_check(mtmp, "doing any services") || !m_speak_check(mtmp))
        return 0;
    else if (!umoney) 
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    context.npc_identify_type = id_idx;
    if (count_unidentified(invent, 0, FALSE) == 0)
    {
        play_sfx_sound(SFX_GENERAL_CANNOT);
        You_ex1_popup("have nothing to identify.", "Nothing to Identify", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
        context.npc_identify_type = 0;
        return 0;
    }
    context.npc_identify_type = 0;

    char qbuf[QBUFSZ];
    int res = 0;
    play_monster_special_dialogue_line(mtmp, spdialogue1);
    Sprintf(qbuf, "Would you like to identify %s? (%ld %s)", an(identify_item_str), minor_id_cost, currency(minor_id_cost));

    switch (yn_query_mon(mtmp, qbuf)) {
    default:
        return 0;
    case 'y':
        if (umoney < minor_id_cost) {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
            return 0;
        }
        break;
    }

    context.npc_identify_type = id_idx;
    res = service_identify(mtmp, minor_id_cost);
    context.npc_identify_type = 0;


    //int cnt = 0, unided = 0;

    //do
    //{
        //if (!cnt)
        //{
            //play_monster_special_dialogue_line(mtmp, spdialogue1);
            //Sprintf(qbuf, "Would you like to identify %s? (%d %s)", an(identify_item_str), minor_id_cost, currency((long)minor_id_cost));
        //}
        //else
        //{
        //    play_monster_special_dialogue_line_with_flags(mtmp, spdialogue2, PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING_OR_QUEUED);
        //    Sprintf(qbuf, "Would you like to identify one more %s? (%d %s)", identify_item_str, minor_id_cost, currency((long)minor_id_cost));
        //}

        //switch (yn_query_mon(mtmp, qbuf)) {
        //default:
        //    return 0;
        //case 'y':
        //    if (umoney < (long)minor_id_cost) {
        //        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        //        You("don't have enough money for that!");
        //        return 0;
        //    }
        //    break;
        //}

        //context.npc_identify_type = id_idx;
        //res = identify_pack(1, FALSE);
        //context.npc_identify_type = 0;

    //} while (res > 0 && unided > 0 && umoney >= (long)minor_id_cost && cnt < 100); /* Paranoid limit */

    return (res > 0);
}



/* menu of unidentified objects */
int
service_identify(mtmp, id_cost)
struct monst* mtmp;
long id_cost;
{
    menu_item* pick_list;
    int n, i;
    char buf[BUFSZ];
    int res = 0, id_res = 0;
    long umoney = money_cnt(invent);

    Strcpy(buf, "What would you like to identify?");

    n = query_objlist(buf, &invent, (SIGNAL_NOMENU | SIGNAL_ESCAPE
        | USE_INVLET | INVORDER_SORT | OBJECT_COMPARISON),
        &pick_list, PICK_ANY, not_fully_identified, 0);

    if (n > 0)
    {
        boolean itemize = FALSE;
        int iprompt = 'n';
        if (n > 1)
        {
            iprompt = ynq("Itemized billing for identification?");
            itemize = (iprompt == 'y');
        }

        if (iprompt == 'q')
        {
            pline1(Never_mind);
            return 0;
        }

        char qbuf[BUFSZ];
        char qendbuf[BUFSZ];
        for (i = 0; i < n; i++)
        {
            int ans = 'y';
            struct obj* otmp = pick_list[i].item.a_obj;
            if (itemize)
            {
                Sprintf(qendbuf, " for %ld %s?", id_cost, currency(id_cost));
                (void)safe_qbuf(qbuf, "Identify ", qendbuf, otmp,
                    doname, thesimpleoname,
                    (otmp->quan == 1L) ? "that" : "those");

                ans = n == 1 ? yn_query(qbuf) : ynaq(qbuf);
                switch (ans)
                {
                default:
                case 'n':
                    continue;
                case 'a':
                    itemize = FALSE;
                    break;
                case 'y':
                    break;
                case 'q':
                    return res;
                }
            }

            if (umoney < id_cost)
            {
                play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
                You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
                if(!itemize && res > 0)
                    update_inventory();
                return res;
            }

            play_sfx_sound(SFX_IDENTIFY_SUCCESS);
            money2mon(mtmp, id_cost);
            umoney = money_cnt(invent);
            bot();
            id_res = identify(otmp);
            res += id_res;
            if (itemize)
                update_inventory();
        }
        free((genericptr_t)pick_list);
        mark_synch(); /* Before we loop to pop open another menu */
        if (!itemize && res > 0)
            update_inventory();
    }
    return res;
}


STATIC_OVL int
sell_to_npc(obj, mtmp, items_left_in_list, auto_yes)
struct obj* obj;
struct monst* mtmp;
int items_left_in_list;
boolean auto_yes;
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
        saleitem = maybe_forgeable_ore(obj);
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
        pline("%s seems uninterested.", noittame_Monnam(mtmp));
        res = 1;
        goto merge_obj_back;
    }

    if (!saleitem
        || obj->oclass == BALL_CLASS || obj->oclass == CHAIN_CLASS
        || offer == 0L) 
    {
        play_sfx_sound(SFX_SEEMS_UNINTERESTED);
        pline("%s seems uninterested.", noittame_Monnam(mtmp));
        res = 1;
        goto merge_obj_back;
    }

    shkmoney = money_cnt(mtmp->minvent);
    if (!shkmoney) 
    {
        play_sfx_sound(SFX_CANNOT_PAY);
        pline("%s cannot pay you at present.", noittame_Monnam(mtmp));
    }
    else 
    {
        char qbuf[BUFSZ], qsfx[BUFSZ];
        boolean short_funds = (offer > shkmoney), one;

        if (short_funds)
            offer = shkmoney;

        Sprintf(qbuf, "%s offers%s %ld gold piece%s for %s ",
            noittame_Monnam(mtmp), short_funds ? " only" : "", offer,
            plur(offer),
            "your");
        one = (obj->quan == 1L);
        Sprintf(qsfx, ".  Sell %s?",
            one ? "it" : "them");
        (void)safe_qbuf(qbuf, qbuf, qsfx, obj, xname, simpleonames,
            one ? "that" : "those");

        char ans = auto_yes ? 'y' : items_left_in_list > 1 ? ynaq(qbuf) : yn_query(qbuf);

        switch (ans)
        {
        case 'q':
        case 'n':
            pline1(Never_mind);
            if (ans == 'q')
                res = -3;
            break;
        case 'a':
        case 'y':
            if (release_item_from_hero_inventory(obj))
            {
                play_sfx_sound(SFX_SELL_TO_NPC);
                You("sold %s for %ld gold piece%s to %s.", doname(obj), offer, plur(offer), noittame_mon_nam(mtmp));

                if (*u.ushops || obj->unpaid)
                    check_shop_obj(obj, mtmp->mx, mtmp->my, FALSE, FALSE);

                (void)mpickobj(mtmp, obj);
                money2u(mtmp, offer);
                bot();
                return ans == 'a' ? -1 : 1;
            }
            else
            {
                if (ans == 'a')
                    res = -2;
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
                    impossible("sell_to_npc: null obj after merge");
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

    (void)quest_chat(mtmp, TRUE); /* To avoid two consecutive hints */
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

void
popup_talk_lines(mtmp, linearray)
struct monst* mtmp;
const char** linearray;
{
    hermit_talk(mtmp, linearray, GHSOUND_NONE);
}

void
popup_talk_line(mtmp, line)
struct monst* mtmp;
const char* line;
{
    const char* linearray[2] = { 0, 0 };
    linearray[0] = line;
    hermit_talk(mtmp, linearray, GHSOUND_NONE);
}

void
popup_talk_line_noquotes(mtmp, line)
struct monst* mtmp;
const char* line;
{
    const char* linearray[2] = { 0, 0 };
    linearray[0] = line;
    popup_talk(mtmp, linearray, GHSOUND_NONE, ATR_NONE, NO_COLOR, TRUE, FALSE);
}


void
popup_talk_line_ex(mtmp, line, attr, color, printtext, addquotes)
struct monst* mtmp;
const char* line;
int attr, color;
boolean printtext, addquotes;
{
    const char* linearray[2] = { 0, 0 };
    linearray[0] = line;
    popup_talk(mtmp, linearray, GHSOUND_NONE, attr, color, printtext, addquotes);
}

void
popup_talk_line_with_know_mname(mtmp, line, know_mname)
struct monst* mtmp;
const char* line;
boolean know_mname;
{
    boolean saved_know_mname = mtmp->u_know_mname;
    mtmp->u_know_mname = know_mname;
    popup_talk_line(mtmp, line);
    mtmp->u_know_mname = saved_know_mname;
}

STATIC_OVL void
hermit_talk(mtmp, linearray, soundid)
struct monst* mtmp;
const char** linearray;
enum ghsound_types soundid;
{
    popup_talk(mtmp, linearray, soundid, ATR_NONE, CLR_MSG_TALK_NORMAL, TRUE, TRUE);
}

#if 0
STATIC_OVL void
hermit_talk_with_startindex(mtmp, linearray, soundid, soundindextype, startindex)
struct monst* mtmp;
const char** linearray;
enum ghsound_types soundid;
uchar soundindextype; /* 0 = LineIndex, 1 = MsgIndex */
int startindex;
{
    popup_talk_core(mtmp, linearray, soundid, soundindextype, startindex, ATR_NONE, NO_COLOR, TRUE, TRUE);
}
#endif

STATIC_OVL void
popup_talk(mtmp, linearray, soundid, attr, color, printtext, addquotes)
struct monst* mtmp;
const char** linearray;
enum ghsound_types soundid;
int attr, color;
boolean printtext;
boolean addquotes;
{
    popup_talk_core(mtmp, linearray, soundid, 0, 0, attr, color, printtext, addquotes);
}

STATIC_OVL void
popup_talk_core(mtmp, linearray, soundid, soundindextype, startlineidx, attr, color, printtext, addquotes)
struct monst* mtmp;
const char** linearray;
enum ghsound_types soundid;
uchar soundindextype; /* 0 = line indexed, 1 = msg indexed, 2 = unindexed (ignore startlineidx) */
int startlineidx; /* Index number to start */
int attr, color;
boolean printtext;
boolean addquotes;
{
    if (!mtmp || !linearray)
        return;

    int glyph = get_seen_monster_glyph(mtmp);
    const char* hermit_txt = 0;
    char namebuf[BUFSZ];
    strcpy_capitalized_for_title(namebuf, noittame_Monnam(mtmp));

    int idx = 0;
    while(linearray[idx] != 0)
    {
        if (linearray[idx])
        {
            hermit_txt = linearray[idx];
            if (soundid != GHSOUND_NONE)
                play_hermit_dialogue_line(mtmp, soundid, soundindextype, startlineidx + idx);
            if (printtext)
            {
                if(addquotes)
                    verbalize_ex1(attr, color, hermit_txt);
                else
                    pline_ex1(attr, color, hermit_txt);
            }
            display_popup_text(hermit_txt, namebuf, POPUP_TEXT_DIALOGUE, attr, color, glyph, addquotes ? POPUP_FLAGS_ADD_QUOTES : 0);
        }
        idx++;
    }
}

STATIC_OVL int
do_chat_quantum_experiments(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "Deep underneath this town, my colleagues are conducting delicate quantum experiments in the Large Circular Dungeon.",
        "But beware, it takes but a minuscule disturbance to ruin their carefully constructed entanglements.",
        "Nothing would anger them more than someone making them lose years of research.",
        0 };

    hermit_talk(mtmp, linearray, GHSOUND_QUANTUM_EXPERIMENTS);
    mtmp->quantum_told_experiments = 1;
    return 1;
}

STATIC_OVL int
do_chat_quantum_large_circular_dungeon(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "The Large Circular Dungeon is a massive circular hallway constructed to conduct our most demanding quantum experiments.",
        "The entire dungeon is one gigantic quantum apparatus, originally carved into its great size by tamed purple worms.",
        "Since its completion, it has provided us with invaluable insights into teleportation and formation of planar rifts, among other discoveries.",
        0 };

    hermit_talk(mtmp, linearray, GHSOUND_QUANTUM_LARGE_CIRCULAR_DUNGEON);
    return 1;
}


STATIC_OVL int
do_chat_quantum_special_wand(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "That is a wand of town portal.",
        "It enables you to teleport back and forth between this town and your original location.",
        "I supply them to local inhabitants and travellers alike.",
        0 };

    hermit_talk(mtmp, linearray, GHSOUND_QUANTUM_SPECIAL_WAND);
    makeknown(WAN_TOWN_PORTAL);

    return 1;
}

STATIC_OVL int
do_chat_quantum_disintegration_wand(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !m_speak_check(mtmp))
        return 0;

    const char* linearray[3] = {
        "That is a wand of disintegration.",
        "It has been constructed using our research on planar rifts and other holes in the fabric of reality.",
        0 };

    hermit_talk(mtmp, linearray, GHSOUND_QUANTUM_DISINTEGRATION_WAND);
    makeknown(WAN_DISINTEGRATION);

    return 1;
}

STATIC_OVL int
do_chat_quantum_teleportation_wand(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !m_speak_check(mtmp))
        return 0;

    const char* linearray[3] = {
        "That is a wand of teleportation.",
        "You can use it to teleport yourself and others to another location.",
        0 };

    hermit_talk(mtmp, linearray, GHSOUND_QUANTUM_TELEPORTATION_WAND);
    makeknown(WAN_TELEPORTATION);

    return 1;
}


STATIC_OVL int 
do_chat_hermit_dungeons(mtmp)
struct monst* mtmp;
{
    if (!mtmp || !m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = { 
        "The Dungeons of Doom are an ancient complex of dungeons, some thirty levels deep.",
        "A few levels below the surface, the Dungeons branch into Gnomish Mines, home to the mysterious Gnomes of Yendor.",
        "The Dungeons also feature Sokoban, a maze-like underground tower erected by one of the local mad wizards.",
        0 };

    hermit_talk(mtmp, linearray, GHSOUND_HERMIT1_DUNGEONS);
    mtmp->hermit_told_dungeon = 1;
    return 1;
}

STATIC_OVL int
do_chat_hermit_quests(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "The Amulet has been taken to Gehennom, the Under World, where Moloch's minions are hiding.",
        "To get there, you will need to descend to the bottom of this dungeon and find an underground castle there.",
        "From there, you can access the Under World, just as the Wizard of Yendor himself once did.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT1_QUESTS);

    mtmp->hermit_told_quests = 1;
    return 1;
}
STATIC_OVL int
do_chat_hermit_gnomish_mines(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[5] = {
        "The Gnomish Mines are a complex of ancient mines, some eight levels deep, inhabited by Gnomes of Yendor.",
        "There, on its bottom level, they worship a magical stone, the Gladstone, through strange ceremonies.",
        "There is also a trading outpost, called Mine Town, which is located somewhere in the middle of the complex.",
        "Seek out Herbert Reed there. He knows more about the gnomes and their mysterious artifact.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT1_GNOMISH_MINES);

    mtmp->hermit_told_gnomish_mines = 1;
    return 1;
}


STATIC_OVL int
do_chat_hermit3_gnomish_mines(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[3] = {
        "These mines have been inhabited by the Gnomes of Yendor as long as I can remember.",
        "Their most precious treasures, including the famed Gladstone, are located on the bottom level of the complex, a few levels down from here.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT3_GNOMISH_MINES);

    mtmp->hermit3_told_gnomish_mines = 1;
    return 1;
}

STATIC_OVL int
do_chat_hermit3_luckstone(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[5] = {
        "The Gladstone is a magnificent magical stone that is known to bestow unparalleled good luck on the wearer.",
        "They also say that it can grant protection from poison and heal the bearer upon invocation.",
        "It has been worshipped by the gnomes as long as anyone can remember, and they will jealously guard it from anyone attempting to take it from them.",
        "However, such a powerful artifact can greatly help you in your quest for the Amulet of Yendor.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT3_LUCKSTONE);

    return 1;
}

STATIC_OVL int
do_chat_orc_hermit3_gnomish_mines(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "We came to these mines to seek the treasures of the gnomes, especially the famed Gladstone.",
        "So far we have just been able to overrun this town; the human guards were but mere weaklings and faltered to our horde with little resistance.",
        "However, the gnomes are putting up a fierce fight on the bottom level of the complex, and we have been unable to take the Gladstone from them.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_ORC_HERMIT3_GNOMISH_MINES);

    mtmp->hermit3_told_gnomish_mines = 1;
    return 1;
}

STATIC_OVL int
do_chat_orc_hermit3_luckstone(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "The Gladstone is a great magical stone that is known to bestow unparalleled good luck on the wearer.",
        "They also say that it can may grant protection from poison and heal the bearer upon invocation.",
        "It still angers me that our warriors have failed to bring such a great artifact to me for closer examination.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_ORC_HERMIT3_LUCKSTONE);

    return 1;
}


STATIC_OVL int
do_chat_hermit_sokoban(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "Sokoban is a tower inlaid with puzzles devised by a mad wizard, who resides on the tower's topmost level.",
        "Great treasures have been promised to the one who can solve the wizard's puzzles.",
        "They say that the wizard will gift either an amulet of reflection or a bag of holding to whomever passes the tests.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT1_SOKOBAN);

    mtmp->hermit_told_sokoban = 1;
    return 1;
}


//STATIC_OVL int
//do_chat_hermit_sokoprizes(mtmp)
//struct monst* mtmp;
//{
//    if (!m_speak_check(mtmp))
//        return 0;
//
//    const char* linearray[2] = {
//        "They say that the wizard will gift either an amulet of reflection or a bag of holding to whomever passes the tests.",
//        0 };
//    hermit_talk(mtmp, linearray);
//
//    return 1;
//}


STATIC_OVL int
do_chat_hermit_castle(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[5] = {
    "A great castle resides at the bottom of the Dungeons of Doom.",
    "It was once a memorial place for the dead kings of Yendor and the last stop before the Under World.",
    "However, it has been overrun by monstrous forces loyal to the Wizard of Yendor.",
    "Only from there, and with the right tune they say, you can track down the Wizard and the Amulet you seek.",
    0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT1_CASTLE);

    return 1;
}
STATIC_OVL int
do_chat_hermit_gehennom(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[3] = {
    "Gehennom, the Under World, can be accessed through the castle at the bottom of the dungeon.",
    "Seek out a hermit, Eduard, at the castle. He knows more about the Under World and what you must do.",
    0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT1_GEHENNOM);

    return 1;
}
STATIC_OVL int
do_chat_hermit_wizard_of_yendor(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[3] = {
        "The Wizard of Yendor is a great magician who stepped into the Under World to study the secrets of life and death.",
        "He is said to possess vast arcane powers and be almost immortal.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT1_WIZARD_OF_YENDOR);

    mtmp->hermit_told_wizard_of_yendor = 1;
    return 1;
}

STATIC_OVL int
do_chat_hermit2_castle(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[3] = {
        "This castle was built to honor the dead kings of Yendor.",
        "Their remains are held in the Tomb of the Kings, just behind this castle.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT2_CASTLE);

    mtmp->hermit2_told_castle = 1;
    return 1;
}

STATIC_OVL int
do_chat_hermit2_gehennom(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
    "Gehennom, the Under World, can be accessed through this castle.",
    "Trap doors in the castle will lead you straight there, as does the blocked entrance at the Tomb of the Kings.",
    "There, you must track down the Wizard of Yendor and find Vlad the Impaler, the Lord of the Vampires.",
    0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT2_GEHENNOM);

    mtmp->hermit2_told_gehennom = 1;
    return 1;
}

STATIC_OVL int
do_chat_hermit2_vampire_lord(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
    "Vlad the Impaler, the Lord of the Vampires, resides in a tower in Gehennom.",
    "You must defeat him and take the Candelabrum of Invocation from him.",
    "The Candelabrum will light your way to Moloch's Sanctum, where the Amulet is hidden.",
    0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT2_VAMPIRE_LORD);

    mtmp->hermit2_told_vampire_lord = 1;
    return 1;
}
STATIC_OVL int
do_chat_hermit2_wizard_of_yendor(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "The Wizard of Yendor is studying a tome of ancient evil, the Book of the Dead, in his tower in Gehennom.",
        "You must defeat him and obtain his grimoire.",
        "Only its tenebrous words are powerful enough to crack open the hidden passage to the sanctum where Moloch's minions guard the Amulet.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT2_WIZARD_OF_YENDOR);

    mtmp->hermit2_told_wizard_of_yendor = 1;
    return 1;
}
STATIC_OVL int
do_chat_hermit2_silver_bell(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "The Silver Bell is an artifact necessary for gaining access to Moloch's Sanctum.",
        "The bell is held by a great enemy of yours near your home.",
        "Your friends have called for your assistance. Heed their call.",
        0 };

    if (u.uachieve.bell)
    {
        linearray[1] = 0;
    }

    hermit_talk(mtmp, linearray, GHSOUND_HERMIT2_SILVER_BELL);

    mtmp->hermit2_told_silver_bell = 1;
    return 1;
}
STATIC_OVL int
do_chat_hermit2_candelabrum(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[3] = {
    "The Candelabrum of Invocation is a powerful artifact that can light your way to Moloch's Sanctum.",
    "It is held by Vlad the Impaler, the Lord of the Vampires, at his tower in Gehennom.",
    0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT2_CANDELABRUM);

    mtmp->hermit2_told_candelabrum = 1;
    return 1;
}
STATIC_OVL int
do_chat_hermit2_book_of_the_dead(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[3] = {
        "The Book of the Dead is a terrible tome of great power that can even raise the dead.",
        "The Wizard of Yendor has been studying it to learn the secrets of life and death in his quest for immortality.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT2_BOOK_OF_THE_DEAD);

    mtmp->hermit2_told_book_of_the_dead = 1;
    return 1;
}
STATIC_OVL int
do_chat_hermit2_ritual(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "To open the passage to Moloch's Sanctum, where the Amulet is hidden, you must have three artifacts.",
        "These are the Candelabrum of Invocation, the Silver Bell, and the Book of the Dead.",
        "How and where these must be used, you must find out. Perhaps the Oracle of Delphi can see what must be done.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT2_RITUAL);

    mtmp->hermit2_told_ritual = 1;
    return 1;
}
STATIC_OVL int
do_chat_hermit_further_advice(mtmp)
struct monst* mtmp;
{
    if (!m_speak_check(mtmp))
        return 0;

    const char* linearray[4] = {
        "Please consult with the Oracle of Delphi a few levels down from here.",
        "Also, Izchak and Herbert at the Mine Town may be able to help you.",
        "Finally, if you reach the bottom of this dungeon, speak with Eduard, who is confined at the castle there.",
        0 };
    hermit_talk(mtmp, linearray, GHSOUND_HERMIT1_FURTHER_ADVICE);

    mtmp->hermit_told_further_advice = 1;
    return 1;
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

STATIC_VAR audio_mapping *soundmap = 0;

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
spell_service_query(mtmp, service_spell_id, buc, service_verb, service_cost, no_mood_string, special_dialogue_sound_id)
struct monst* mtmp;
int service_spell_id, buc;
long service_cost;
const char* service_verb;
const char* no_mood_string;
int special_dialogue_sound_id;
{

    long umoney = money_cnt(invent);
    long  u_pay;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, no_mood_string) || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    if (special_dialogue_sound_id > 0)
        play_monster_special_dialogue_line(mtmp, special_dialogue_sound_id);

    Sprintf(qbuf, "Would you like to %s? (%ld %s)", service_verb, service_cost, currency(service_cost));
    switch (yn_query_mon(mtmp, qbuf))
    {
    default:
        return 0;
    case 'y':
        if (umoney < service_cost) 
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex1_popup("don't have enough money for that!", "Not Enough Money", ATR_NONE, CLR_MSG_FAIL, NO_GLYPH, POPUP_FLAGS_NONE);
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
    (void)seffects(pseudo, &effect_happened, &youmonst);
    if (effect_happened)
    {
        money2mon(mtmp, u_pay);
        bot();
    }
    obfree(pseudo, (struct obj*)0);
    /* gnostic handled in seffects */

    return 1;
}

STATIC_OVL int
general_service_query(mtmp, service_func, service_verb, service_cost, no_mood_string, special_dialogue_sound_id)
struct monst* mtmp;
int (*service_func)(struct monst*);
const char* service_verb;
long service_cost;
const char* no_mood_string;
int special_dialogue_sound_id;
{

    return general_service_query_with_extra(mtmp, service_func, service_verb, service_cost, no_mood_string, QUERY_STYLE_COMPONENTS, (const char* )0, special_dialogue_sound_id);
}

STATIC_OVL int
general_service_query_with_extra(mtmp, service_func, service_verb, service_cost, no_mood_string, query_style, extra_string, special_dialogue_sound_id)
struct monst* mtmp;
int (*service_func)(struct monst*);
const char* service_verb;
long service_cost;
const char* no_mood_string;
int query_style;
const char* extra_string;
int special_dialogue_sound_id;
{

    long umoney = money_cnt(invent);
    long u_pay;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, no_mood_string) || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }

    if (special_dialogue_sound_id > 0)
    {
        if(query_style == QUERY_STYLE_SPELL)
            play_monster_standard_dialogue_line(mtmp, special_dialogue_sound_id);
        else
            play_monster_special_dialogue_line(mtmp, special_dialogue_sound_id);
    }

    if (extra_string)
    {
        if(query_style == QUERY_STYLE_COMPONENTS)
            Sprintf(qbuf, "Would you like to %s? (%ld %s, %s)", service_verb, service_cost, currency(service_cost), extra_string);
        else
            Sprintf(qbuf, "Would you like to %s? (%s for %ld %s)", service_verb, extra_string, service_cost, currency(service_cost));
    }
    else
        Sprintf(qbuf, "Would you like to %s? (%ld %s)", service_verb, service_cost, currency(service_cost));

    switch (yn_query_mon(mtmp, qbuf))
    {
    default:
        return 0;
    case 'y':
        if (umoney < service_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough money for that!");
            return 0;
        }
        u_pay = service_cost;
        break;
    }

    int res = service_func(mtmp);
    if (res)
    {
        money2mon(mtmp, u_pay);
        bot();
    }

    return 1;
}

STATIC_OVL int
general_service_query_with_item_cost_adjustment_and_extra(mtmp, service_item_func, service_verb, service_verb_ing, service_base_cost, item_cost_at_base, minimum_cost, no_mood_string, query_style, extra_string, special_dialogue_sound_id)
struct monst* mtmp;
int (*service_item_func)(struct monst*, struct obj*);
const char* service_verb, *service_verb_ing;
long service_base_cost, item_cost_at_base, minimum_cost;
const char* no_mood_string;
int query_style;
const char* extra_string;
int special_dialogue_sound_id;
{

    long umoney = money_cnt(invent);
    long u_pay;
    char qbuf[QBUFSZ];

    if (!m_general_talk_check(mtmp, no_mood_string) || !m_speak_check(mtmp))
        return 0;
    else if (!umoney)
    {
        play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
        You_ex1_popup("have no money.", "No Money", ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        return 0;
    }
    if (special_dialogue_sound_id > 0)
    {
        if (query_style == QUERY_STYLE_SPELL)
            play_monster_standard_dialogue_line(mtmp, special_dialogue_sound_id);
        else
            play_monster_special_dialogue_line(mtmp, special_dialogue_sound_id);
    }
    struct obj* otmp = getobj(getobj_all_count, service_verb, 0, "");
    if (!otmp)
        return 0;

    long service_cost = !item_cost_at_base ? service_base_cost : max(minimum_cost, (service_base_cost * get_object_base_value(otmp)) / max(1L, item_cost_at_base));
    char ingbuf[BUFSZ] = "";
    Strcpy(ingbuf, service_verb_ing);
    *ingbuf = highc(*ingbuf);

    if (extra_string)
        Sprintf(qbuf, "%s %s costs %ld %s and %s. Proceed?", ingbuf, the(cxname(otmp)), service_cost, currency(service_cost), extra_string);
    else
        Sprintf(qbuf, "%s %s costs %ld %s. Proceed?", ingbuf, the(cxname(otmp)), service_cost, currency(service_cost));

    switch (yn_query_mon(mtmp, qbuf))
    {
    default:
        return 0;
    case 'y':
        if (umoney < service_cost)
        {
            play_sfx_sound(SFX_NOT_ENOUGH_MONEY);
            You_ex(ATR_NONE, CLR_MSG_FAIL, "don't have enough money for that!");
            return 0;
        }
        u_pay = service_cost;
        break;
    }

    int res = service_item_func(mtmp, otmp);
    if (res)
    {
        money2mon(mtmp, u_pay);
        bot();
    }

    return 1;
}

STATIC_OVL int
recharge_item_func(mtmp, otmp)
struct monst* mtmp UNUSED;
struct obj* otmp;
{

    if (otmp)
        recharge(otmp, 0, TRUE, "Recharging", TRUE);
    else
        return 0;

    return 1;
}

STATIC_OVL int
blessed_recharge_item_func(mtmp, otmp)
struct monst* mtmp UNUSED;
struct obj* otmp;
{

    if (otmp)
        recharge(otmp, 1, TRUE, "Recharging", TRUE);
    else
        return 0;

    return 1;
}


STATIC_OVL int
repair_armor_func(mtmp)
struct monst* mtmp;
{
    const char repair_armor_objects[] = { ALL_CLASSES, ARMOR_CLASS, 0 };
    struct obj* otmp = getobj(repair_armor_objects, "repair", 0, "");
    char talkbuf[BUFSZ];

    if (!otmp)
        return 0;

    play_monster_special_dialogue_line(mtmp, SMITH_LINE_LETS_HAVE_A_LOOK);
    if(iflags.using_gui_sounds)
        Sprintf(talkbuf, "%s says: \"Let's have a look.\"", noittame_Monnam(mtmp));
    else
        Sprintf(talkbuf, "%s says: \"Let's have a look at %s.\"", noittame_Monnam(mtmp), yname(otmp));
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);

    if (otmp && otmp->oclass != ARMOR_CLASS)
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_SORRY_THIS_IS_NOT_AN_ARMOR_I_CAN_REPAIR);
        Strcpy(talkbuf, "Sorry, this is not an armor I can repair.");
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 0;
    }
    else if (otmp && !erosion_matters(otmp))
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_SORRY_I_COULDNT_MAKE_THIS_ANY_BETTER_THAN_BEFORE);
        Strcpy(talkbuf, "Sorry, I couldn't make this any better than before.");
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 0;
    }

    play_sfx_sound(SFX_REPAIR_ITEM_SUCCESS);
    if (otmp->oeroded || otmp->oeroded2)
    {
        otmp->oeroded = otmp->oeroded2 = 0;
        Sprintf(talkbuf, "%s as good as new!",
            Yobjnam2(otmp, Blind ? "feel" : "look"));
        pline_ex1_popup(ATR_NONE, NO_COLOR, talkbuf, "Repair Armor", TRUE);
    }
    else
    {
        otmp->oeroded = otmp->oeroded2 = 0;
        Sprintf(talkbuf, "%s as good as new, just like %s before!",
            Yobjnam2(otmp, Blind ? "feel" : "look"), otmp->quan == 1 ? "it was" : "they were");
        pline_ex1_popup(ATR_NONE, NO_COLOR, talkbuf, "Repair Armor", TRUE);
    }
    update_inventory();
    play_monster_special_dialogue_line(mtmp, SMITH_LINE_THANK_YOU_FOR_USING_MY_SERVICES);
    Strcpy(talkbuf, "Thank you for using my services.");
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
    return 1;
}

STATIC_OVL int
repair_weapon_func(mtmp)
struct monst* mtmp;
{
    const char repair_weapon_objects[] = { ALL_CLASSES, WEAPON_CLASS, 0 };
    struct obj* otmp = getobj(repair_weapon_objects, "repair", 0, "");
    char talkbuf[BUFSZ];

    if (!otmp)
        return 0;

    play_monster_special_dialogue_line(mtmp, SMITH_LINE_LETS_HAVE_A_LOOK);
    if (iflags.using_gui_sounds)
        Sprintf(talkbuf, "%s says: \"Let's have a look.\"", noittame_Monnam(mtmp));
    else
        Sprintf(talkbuf, "%s says: \"Let's have a look at %s.\"", noittame_Monnam(mtmp), yname(otmp));
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);

    /* Check if the selection is not an appropriate weapon */
    if (otmp && !is_weapon(otmp))
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_SORRY_THIS_IS_NOT_A_WEAPON_I_CAN_REPAIR);
        Strcpy(talkbuf, "Sorry, this is not a weapon I can repair.");
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 0;
    }
    else if (otmp && !erosion_matters(otmp))
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_SORRY_I_COULDNT_MAKE_THIS_ANY_BETTER_THAN_BEFORE);
        Strcpy(talkbuf, "Sorry, I couldn't make this any better than before.");
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 0;
    }

    play_sfx_sound(SFX_REPAIR_ITEM_SUCCESS);
    if (otmp->oeroded || otmp->oeroded2)
    {
        otmp->oeroded = otmp->oeroded2 = 0;
        Sprintf(talkbuf, "%s as good as new!",
            Yobjnam2(otmp, Blind ? "feel" : "look"));
        pline_ex1_popup(ATR_NONE, NO_COLOR, talkbuf, "Repair Weapon", TRUE);
    }
    else
    {
        otmp->oeroded = otmp->oeroded2 = 0;
        Sprintf(talkbuf, "%s as good as new, just like %s before!",
            Yobjnam2(otmp, Blind ? "feel" : "look"), otmp->quan == 1 ? "it was" : "they were");
        pline_ex1_popup(ATR_NONE, NO_COLOR, talkbuf, "Repair Weapon", TRUE);
    }
    update_inventory();
    play_monster_special_dialogue_line(mtmp, SMITH_LINE_THANK_YOU_FOR_USING_MY_SERVICES);
    Strcpy(talkbuf, "Thank you for using my services.");
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
    return 1;
}

STATIC_OVL int
refill_lantern_func(mtmp)
struct monst* mtmp;
{
    const char refill_lantern_objects[] = { ALL_CLASSES, TOOL_CLASS, 0 };
    struct obj* otmp = getobj_ex(refill_lantern_objects, "refill", 0, "", maybe_refillable_with_oil);
    char talkbuf[BUFSZ];

    if (!otmp)
        return 0;

    /* Check if the selection is appropriate */
    if (otmp && !is_refillable_with_oil(otmp))
    {
//        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_SORRY_THIS_IS_NOT_AN_ITEM_THAT_I_CAN_FILL_WITH_OIL);
        Strcpy(talkbuf, "Sorry, this is not an item that I can fill with oil.");
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 0;
    }
    else if (otmp && otmp->age > 1500L)
    {
        play_sfx_sound(SFX_REPAIR_ITEM_FAIL);
        if (iflags.using_gui_sounds)
        {
            if (otmp->otyp == BRASS_LANTERN)
            {
                play_monster_special_dialogue_line(mtmp, SMITH_LINE_SORRY_YOUR_LANTERN_IS_ALREADY_FULL);
                Strcpy(talkbuf, "Sorry, your lantern is already full.");
                popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
            }
            else
            {
                play_monster_special_dialogue_line(mtmp, SMITH_LINE_SORRY_YOUR_LAMP_IS_ALREADY_FULL);
                Strcpy(talkbuf, "Sorry, your lamp is already full.");
                popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
            }
        }
        else
        {
            Sprintf(talkbuf, "Sorry, %s %s already full.", yname(otmp), otense(otmp, "are"));
            popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        }

        return 0;
    }

    if (otmp->lamplit)
        snuff_lit(otmp);

    play_sfx_sound(SFX_FILL_OIL_UP);

    /* Adding oil to an empty magic lamp renders it into an oil lamp */
    if ((otmp->otyp == MAGIC_LAMP) && otmp->special_quality == 0)
    {
        otmp->otyp = OIL_LAMP;
        otmp->age = 0;
    }

    Sprintf(talkbuf, "%s fills %s with oil.", noittame_Monnam(mtmp), yname(otmp));
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);

    otmp->age = MAX_OIL_IN_LAMP;
    otmp->special_quality = 1;
    update_inventory();

    play_monster_special_dialogue_line(mtmp, SMITH_LINE_THANK_YOU_FOR_USING_MY_SERVICES);
    Strcpy(talkbuf, "Thank you for using my services.");
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
    return 1;
}

STATIC_OVL int
forge_cubic_gate_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, DILITHIUM_CRYSTAL, 1, CUBIC_GATE, 0, 0, MAT_NONE,TRUE);
}

STATIC_OVL int
forge_artificial_wings_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, FEATHER, 12, WINGS_OF_FLYING, 0, 0, MAT_NONE, FALSE);
}

STATIC_OVL int
forge_dragon_scale_mail_func(mtmp)
struct monst* mtmp;
{
    const char forge_objects[] = { ALL_CLASSES, ARMOR_CLASS, 0 };
    struct obj* otmp = getobj_ex(forge_objects, "forge into a dragon scale mail", 0, "", maybe_dragon_scales);
    char talkbuf[BUFSZ];

    if (!otmp)
        return 0;

    if (iflags.using_gui_sounds)
    {
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_LETS_HAVE_A_LOOK);
        Sprintf(talkbuf, "%s says: \"Let's have a look.\"", noittame_Monnam(mtmp));
    }
    else
    {
        Sprintf(talkbuf, "%s says: \"Let's have a look at %s.\"", noittame_Monnam(mtmp), yname(otmp));
    }
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);

    /* Check if the selection is appropriate */
    if (otmp && !maybe_dragon_scales(otmp))
    {
        //play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
        play_monster_special_dialogue_line(mtmp, SMITH_LINE_SORRY_THIS_IS_NOT_AN_ITEM_THAT_I_CAN_FORGE_INTO_A_DRAGON_SCALE_MAIL);
        Strcpy(talkbuf, "Sorry, this is not an item that I can forge into a dragon scale mail.");
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 0;
    }

    play_sfx_sound(SFX_NEARBY_LOUD_CLANGING);
    Sprintf(talkbuf, "%s starts working on %s.", noittame_Monnam(mtmp), yname(otmp));
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
#ifndef GNH_MOBILE
    if (iflags.using_gui_sounds)
        delay_output_milliseconds(2000);
#endif

    dragon_scales_to_scale_mail(otmp, FALSE, TRUE);

    play_monster_special_dialogue_line(mtmp, SMITH_LINE_THANK_YOU_FOR_USING_MY_SERVICES);
    Strcpy(talkbuf, "Thank you for using my services.");
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
    return 1;
}

STATIC_OVL int
forge_orichalcum_full_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_ORICHALCUM_ORE, 4, FULL_PLATE_MAIL, 0, 0, MAT_ORICHALCUM, FALSE);
}

STATIC_OVL int
forge_crystal_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, DILITHIUM_CRYSTAL, 2, PLATE_MAIL, 0, 0, MAT_HARD_CRYSTAL, FALSE);
}

STATIC_OVL int
forge_shield_of_reflection_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_SILVER_ORE, 8, SHIELD_OF_REFLECTION, 0, 0, MAT_NONE, FALSE);
}

STATIC_OVL int
forge_adamantium_full_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_ADAMANTIUM_ORE, 4, FULL_PLATE_MAIL, 0, 0, MAT_ADAMANTIUM, FALSE);
}

STATIC_OVL int
forge_mithril_full_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_MITHRIL_ORE, 4, FULL_PLATE_MAIL, 0, 0, MAT_MITHRIL, FALSE);
}

STATIC_OVL int
forge_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_IRON_ORE, 4, PLATE_MAIL, 0, 0, MAT_NONE, FALSE);
}

STATIC_OVL int
forge_bronze_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_COPPER_ORE, 4, PLATE_MAIL, 0, 0, MAT_BRONZE, FALSE);
}

STATIC_OVL int
forge_field_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_IRON_ORE, 6, FIELD_PLATE_MAIL, 0, 0, MAT_NONE, FALSE);
}

STATIC_OVL int
forge_full_plate_mail_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_IRON_ORE, 8, FULL_PLATE_MAIL, 0, 0, MAT_NONE, FALSE);
}

STATIC_OVL int
forge_iron_sling_bullets_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_IRON_ORE, 2, SLING_BULLET, 10, EXCEPTIONALITY_NORMAL, MAT_NONE, FALSE);
}

STATIC_OVL int
forge_ex_iron_sling_bullets_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_IRON_ORE, 3, SLING_BULLET, 10, EXCEPTIONALITY_EXCEPTIONAL, MAT_NONE, FALSE);
}

STATIC_OVL int
forge_el_iron_sling_bullets_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_IRON_ORE, 4, SLING_BULLET, 10, EXCEPTIONALITY_ELITE, MAT_NONE, FALSE);
}

STATIC_OVL int
forge_silver_sling_bullets_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_SILVER_ORE, 2, SLING_BULLET, 10, EXCEPTIONALITY_NORMAL, MAT_SILVER, FALSE);
}

STATIC_OVL int
forge_ex_silver_sling_bullets_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_SILVER_ORE, 3, SLING_BULLET, 10, EXCEPTIONALITY_EXCEPTIONAL, MAT_SILVER, FALSE);
}

STATIC_OVL int
forge_el_silver_sling_bullets_func(mtmp)
struct monst* mtmp;
{
    return forge_special_func(mtmp, NUGGET_OF_SILVER_ORE, 4, SLING_BULLET, 10, EXCEPTIONALITY_ELITE, MAT_SILVER, FALSE);
}


STATIC_OVL int
forge_special_func(mtmp, forge_source_otyp, forge_source_quan, forge_dest_otyp, quan, exceptionality, material, initialize)
struct monst* mtmp;
int forge_source_otyp, forge_source_quan, forge_dest_otyp, quan, exceptionality;
uchar material;
boolean initialize;
{
    char talkbuf[BUFSZ];
    char forge_objects[3] = { 0, 0, 0 };
    forge_objects[0] = ALL_CLASSES;
    forge_objects[1] = objects[forge_source_otyp].oc_class;
    forge_objects[2] = 0;

    char forge_string[BUFSZ];
    Sprintf(forge_string, "forge into %s", an(OBJ_NAME(objects[forge_dest_otyp])));

    otyp_for_maybe_otyp = forge_source_otyp;
    struct obj* otmp = getobj_ex((const char*)forge_objects, forge_string, 0, "", maybe_otyp);

    if (!otmp)
        return 0;

    if (iflags.using_gui_sounds)
    {
        play_monster_special_dialogue_line(mtmp, mtmp->issmith ? SMITH_LINE_LETS_HAVE_A_LOOK : NPC_LINE_LETS_HAVE_A_LOOK);
        Sprintf(talkbuf, "%s says: \"Let's have a look.\"", noittame_Monnam(mtmp));
        delay_output_milliseconds(750);
    }
    else
    {
        Sprintf(talkbuf, "%s says: \"Let's have a look at %s.\"", noittame_Monnam(mtmp), yname(otmp));
    }
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);

    int quan_needed = forge_source_quan;
    /* Check if the selection is appropriate */
    if (otmp && !maybe_otyp(otmp))
    {
        //play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
        play_monster_special_dialogue_line(mtmp, mtmp->issmith ? SMITH_LINE_SORRY_THIS_IS_NOT_AN_ITEM_THAT_I_CAN_FORGE_INTO_THE_REQUESTED_ITEM : NPC_LINE_SORRY_THIS_IS_NOT_A_COMPONENT_THAT_I_CAN_FORGE_INTO_THE_REQUESTED_ITEM);
        Sprintf(talkbuf, "Sorry, this is not an item that I can forge into %s.", an(OBJ_NAME(objects[forge_dest_otyp])));
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 0;
    }

    otyp_for_maybe_otyp = 0;

    if (otmp->quan < quan_needed)
    {
        play_sfx_sound(SFX_ENCHANT_ITEM_GENERAL_FAIL);
        struct obj pseudo = zeroobj;
        pseudo.otyp = forge_source_otyp;
        pseudo.oclass = objects[forge_source_otyp].oc_class;
        pseudo.quan = quan_needed;
        if (iflags.using_gui_sounds)
        {
            play_monster_special_dialogue_line(mtmp, mtmp->issmith ? SMITH_LINE_SORRY_YOU_NEED_MORE_COMPONENTS_TO_FORGE_THE_REQUESTED_ITEM : NPC_LINE_SORRY_YOU_NEED_MORE_COMPONENTS_TO_FORGE_THE_REQUESTED_ITEM);
            Sprintf(talkbuf, "\"Sorry, you need more components to forge the requested item.\" (You need %d %s for %s.) ", quan_needed, cxname(&pseudo), an(OBJ_NAME(objects[forge_dest_otyp])));
            popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        }
        else
        {
            Sprintf(talkbuf, "Sorry, you need %d %s to forge %s.", quan_needed, cxname(&pseudo), an(OBJ_NAME(objects[forge_dest_otyp])));
            popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        }
        return 0;
    }

    play_sfx_sound(SFX_NEARBY_LOUD_CLANGING);
    Sprintf(talkbuf, "%s starts working on %s.", noittame_Monnam(mtmp), yname(otmp));
    popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
#ifndef GNH_MOBILE
    if (iflags.using_gui_sounds)
        delay_output_milliseconds(2000);
#endif

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

    struct obj* craftedobj = mksobj_with_flags(forge_dest_otyp, initialize, FALSE, 3, (struct monst*)0, material, 0L, 0L, MKOBJ_FLAGS_FORCE_BASE_MATERIAL);
    if (craftedobj)
    {
        if (quan > 0)
        {
            craftedobj->quan = quan;
            craftedobj->owt = weight(craftedobj);
        }
        if (exceptionality > 0)
        {
            craftedobj->exceptionality = exceptionality;
        }
        craftedobj->cursed = craftedobj->blessed = 0;
        fully_identify_obj(craftedobj);
        Sprintf(talkbuf, "%s hands %s to you.", noittame_Monnam(mtmp), acxname(craftedobj));
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        hold_another_object(craftedobj, "Oops!  %s out of your grasp!",
            The(aobjnam(craftedobj, "slip")),
            (const char*)0);

        stop_all_immediate_sounds();
        play_sfx_sound(SFX_BUY_FROM_NPC);
        play_monster_special_dialogue_line(mtmp, mtmp->issmith ? SMITH_LINE_THANK_YOU_FOR_USING_MY_SERVICES : NPC_LINE_THANK_YOU_FOR_USING_MY_SERVICES);
        Strcpy(talkbuf, "Thank you for using my services.");
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
    }
    else
    {
        Sprintf(talkbuf, "%s stares blankly for a moment as if something is seriously amiss.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, talkbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
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

    char speakbuf[BUFSZ] = "";
    if (Deaf)
    {
        Sprintf(speakbuf, "%s says something but you cannot hear anything.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, speakbuf, ATR_NONE, CLR_MSG_ATTENTION, TRUE, FALSE);
        return 1;
    }

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
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_DIALOGUE_CANNOT_TEACH_SPELLS);
        Strcpy(speakbuf, "Unfortunately, I cannot teach any spells at the moment.");
        popup_talk_line_ex(mtmp, speakbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 1;
    }
    else if (not_known_cnt == 0)
    {
        play_monster_standard_dialogue_line(mtmp, MONSTER_STANDARD_CANNOT_TEACH_SPELLS_YOU_DONT_KNOW);
        Strcpy(speakbuf, "Unfortunately, I cannot teach any spells you do not already know.");
        popup_talk_line_ex(mtmp, speakbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
        return 1;
    }


    int spell_count = 0;

    menu_item* selected = (menu_item*)0;
    winid win;
    anything any;

    any = zeroany;
    win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_CHAT_ITEM_MENU, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
    start_menu_ex(win, GHMENU_STYLE_CHAT_CHOOSE_ITEM);

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
        int gui_glyph = maybe_get_replaced_glyph(glyph, mtmp->mx, mtmp->my, data_to_replacement_info(glyph, LAYER_OBJECT, &pseudo, (struct monst*)0, 0UL, 0UL, 0UL, MAT_NONE, 0));

        add_menu(win, gui_glyph, &any,
            let, 0, ATR_NONE, NO_COLOR,
            spellbuf, MENU_UNSELECTED);

        spell_count++;
    }

    /* Finish the menu */
    end_menu(win, "Which spell do you want to learn?");

    if (spell_count <= 0)
    {
        destroy_nhwindow(win);
        play_sfx_sound(SFX_GENERAL_CANNOT);
        Sprintf(speakbuf, "%s doesn't have any spells to teach.", noittame_Monnam(mtmp));
        popup_talk_line_ex(mtmp, speakbuf, ATR_NONE, NO_COLOR, TRUE, FALSE);
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
            char* txt = 0;
            struct obj pseudo = zeroobj;
            pseudo.otyp = spell_to_learn;
            pseudo.blessed = 1;
            long cost = get_cost(&pseudo, mtmp);
            char buf[BUFSIZ] = "";
            char buf2[BUFSIZ] = "";
            char bufc[BUFSIZ] = "";
            if (iflags.using_gui_sounds)
            {
                Sprintf(buf, "learn a new spell");
                Sprintf(buf2, "learning the spell '%s'", OBJ_NAME(objects[spell_to_learn]));
                Sprintf(bufc, "'%s'", OBJ_NAME(objects[spell_to_learn]));
                *(bufc + 1) = highc(*(bufc + 1));
                txt = bufc;
            }
            else
            {
                Sprintf(buf, "learn the spell '%s'", OBJ_NAME(objects[spell_to_learn]));
                Sprintf(buf2, "learning the spell '%s'", OBJ_NAME(objects[spell_to_learn]));
                txt = 0;
            }
            context.spbook.book = &pseudo;
            context.spbook.reading_result = READING_RESULT_SUCCESS;
            learn_count = general_service_query_with_extra(mtmp, learn_spell_func, buf, cost, buf2, QUERY_STYLE_SPELL, txt, MONSTER_STANDARD_WOULD_YOU_LIKE_TO_LEARN_A_NEW_SPELL);
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
    char learnbuf[BUFSZ] = "";

    char splname[BUFSZ];
    Sprintf(splname, "%s", OBJ_NAME(objects[booktype]));

    for (i = 0; i < MAXSPELL; i++)
        if (spellid(i) == booktype || spellid(i) == NO_SPELL)
            break;

    int initialamount = i < MAXSPELL ? spl_book[i].sp_amount : 0;
    int addedamount = 0;

    if (i == MAXSPELL)
    {
        impossible("Too many spells memorized!");
    }
    else if (spellid(i) == booktype)
    {
        /* Should not happen */
        //if (spellknow(i) > SPELL_IS_KEEN / 10)
        //{
        //    play_sfx_sound(SFX_SPELL_KNOWN_ALREADY);
        //    Sprintf(learnbuf, "You know %s quite well already.", OBJ_NAME(objects[booktype]));
        //    pline_ex1(ATR_NONE, CLR_MSG_ATTENTION, learnbuf);
        //    display_popup_text(learnbuf, "Spell Known Already", POPUP_TEXT_GENERAL, ATR_NONE, CLR_MSG_ATTENTION, NO_GLYPH, POPUP_FLAGS_NONE);
        //}
        //else
        { /* spellknow(i) <= SPELL_IS_KEEN/10 */
            incr_spell_nknow(i, 1);
            play_sfx_sound(SFX_SPELL_KEENER);
            Sprintf(learnbuf, "Your knowledge of \'%s\' is %s.", splname, spellknow(i) ? "keener" : "restored");
            int multicolors[2] = { CLR_BRIGHT_GREEN, NO_COLOR };
            Your_multi_ex(ATR_NONE, CLR_MSG_POSITIVE, no_multiattrs, multicolors, "knowledge of \'%s\' is %s.", splname, spellknow(i) ? "keener" : "restored");
            display_popup_text(learnbuf, "Knowledge Keener", POPUP_TEXT_GENERAL, ATR_NONE, CLR_MSG_POSITIVE, NO_GLYPH, POPUP_FLAGS_NONE);

            if (spl_book[i].sp_matcomp > 0)
            {
                addedamount = matlists[spl_book[i].sp_matcomp].spellsgained;
                spl_book[i].sp_amount += addedamount;
            }
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
        spl_book[i].sp_cooldownlength = (int)objects[booktype].oc_spell_cooldown;
        spl_book[i].sp_cooldownleft = 0;
        spl_book[i].sp_skillchance = (int)objects[booktype].oc_spell_skill_chance;

        incr_spell_nknow(i, 1);

        if (spl_book[i].sp_matcomp > 0)
        {
            addedamount = matlists[spl_book[i].sp_matcomp].spellsgained;
            spl_book[i].sp_amount += addedamount;
        }

        sortspells();

        play_sfx_sound(SFX_SPELL_LEARN_SUCCESS);
        Sprintf(learnbuf, i > 0 ? "You add \'%s\' to your repertoire." : "You learn \'%s\'.", splname);
        int multicolors[1] = { CLR_MSG_SPELL };
        You_multi_ex(ATR_NONE, CLR_MSG_SUCCESS, no_multiattrs, multicolors, i > 0 ? "add \'%s\' to your repertoire." : "learn \'%s\'.", splname);
        display_popup_text(learnbuf, "New Spell Learnt", POPUP_TEXT_GENERAL, ATR_NONE, CLR_MSG_POSITIVE, NO_GLYPH, POPUP_FLAGS_NONE);
    }

    if (addedamount > 0)
    {
        int multicolors[3] = { CLR_BRIGHT_CYAN, NO_COLOR, CLR_BRIGHT_GREEN };
        if (addedamount == 1)
        {
            You_multi_ex(ATR_NONE, CLR_MSG_SUCCESS, no_multiattrs, multicolors, "now have %s %scasting of \'%s\' prepared.", "one", !initialamount ? "" : "more ", splname);
        }
        else
        {
            You_multi_ex(ATR_NONE, CLR_MSG_SUCCESS, no_multiattrs, multicolors, "now have %d %scastings of \'%s\' prepared.", addedamount, !initialamount ? "" : "more ", splname);
        }
    }

    return 1;
}

STATIC_OVL
struct monst*
ask_target_monster(mtmp)
struct monst* mtmp;
{
    if (iflags.spell_target_monster)
        return iflags.spell_target_monster;

    if (!mtmp || !isok(mtmp->mx, mtmp->my))
        return 0;

    struct monst* nearbymon[10] = { 0 };
    int cnt = 0;
    int x, y;
    nearbymon[0] = &youmonst;
    cnt++;
    for (x = mtmp->mx - 1; x <= mtmp->mx + 1; x++)
    {
        for (y = mtmp->my - 1; y <= mtmp->my + 1; y++)
        {
            if (isok(x, y))
            {
                struct monst* mon = m_at(x, y);
                if (mon && !DEADMONSTER(mon))
                {
                    if (is_tame(mon))
                    {
                        nearbymon[cnt] = mon;
                        cnt++;
                    }
                }
            }
        }
    }

    if (cnt > 1)
    {
        menu_item* pick_list = (menu_item*)0;
        winid win;
        anything any;
        char nbuf[BUFSIZ];

        any = zeroany;
        win = create_nhwindow_ex(NHW_MENU, GHWINDOW_STYLE_CHAT_MENU, get_seen_monster_glyph(mtmp), extended_create_window_info_from_mon(mtmp));
        start_menu_ex(win, GHMENU_STYLE_CHAT);

        int i;
        for (i = 0; i < cnt; i++)
        {
            boolean isyou = nearbymon[i] == &youmonst;
            any = zeroany;
            any.a_monst = nearbymon[i];
            if (isyou)
                Strcpy(nbuf, "You");
            else if (nearbymon[i])
                Strcpy(nbuf, Monnam(nearbymon[i]));
            else
                continue;
            add_menu(win, any_mon_to_glyph(nearbymon[i], rn2_on_display_rng), &any, 0, 0, ATR_NONE, NO_COLOR, nbuf, MENU_UNSELECTED);
        }

        /* Finish the menu */
        end_menu(win, "Choose target");

        if (cnt <= 0)
            return &youmonst;

        struct monst* selmon = 0;
        /* Now generate the menu */
        if (select_menu(win, PICK_ONE, &pick_list) > 0)
        {
            selmon = pick_list->item.a_monst;
            free((genericptr_t)pick_list);
            pick_list = 0;
        }
        destroy_nhwindow(win);

        return selmon;
    }
    else
        return &youmonst;
}

/*sounds.c*/
