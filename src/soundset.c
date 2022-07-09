/* GnollHack 4.1.0 soundset.c */
/* Copyright (c) Janne Gustafsson, 2020. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h" /* for checking save modes */
#include "qtext.h"

 /* flags */
#define SSF_SHOW 0x1        /* display the sound source */
#define SSF_NEEDS_FIXUP 0x2 /* need oid fixup */
#define SSF_SILENCE_SOURCE 0x4 /* emits silence rather than sound */

NEARDATA struct soundsource_t* sound_base = 0;
STATIC_DCL void FDECL(set_hearing_array, (int, int, double, int));

NEARDATA const struct ghsound_eventmapping ghsound2event[MAX_GHSOUNDS] = {
    { SOUND_BANK_NONE, "", 0.0f },

    { SOUND_BANK_MASTER, "event:/Music/Start/Splash", 0.5f},
    { SOUND_BANK_MASTER, "event:/Music/Start/Player Selection", 0.3f},
    { SOUND_BANK_MASTER, "event:/Music/Start/Intro", BACKGROUND_MUSIC_VOLUME * 1.5f},
    { SOUND_BANK_MASTER, "event:/Music/Music-Normal-1", BACKGROUND_MUSIC_VOLUME},
    { SOUND_BANK_MASTER, "event:/Music/Music-Oracle", BACKGROUND_MUSIC_VOLUME * 4.0f},
    { SOUND_BANK_MASTER, "event:/Music/Music-Temple", BACKGROUND_MUSIC_VOLUME},
    { SOUND_BANK_MASTER, "event:/Music/Music-Medusa", BACKGROUND_MUSIC_VOLUME},
    { SOUND_BANK_MASTER, "event:/Music/Music-Castle", BACKGROUND_MUSIC_VOLUME},
    { SOUND_BANK_MASTER, "event:/Music/Music-Gnomish-Mines-Normal", BACKGROUND_MUSIC_VOLUME * 2.5f},
    { SOUND_BANK_MASTER, "event:/Music/Music-Gnomish-Mines-Town", BACKGROUND_MUSIC_VOLUME * 2.5f},

    { SOUND_BANK_MASTER, "event:/Music/Music-Sokoban-Normal", BACKGROUND_MUSIC_VOLUME * 2.5f},
    { SOUND_BANK_MASTER, "event:/Music/Music-Gehennom-Normal", BACKGROUND_MUSIC_VOLUME * 3.0f},
    { SOUND_BANK_MASTER, "event:/Music/Music-Gehennom-Valley", BACKGROUND_MUSIC_VOLUME * 2.0f},
    { SOUND_BANK_MASTER, "event:/Music/Music-Gehennom-Sanctum", BACKGROUND_MUSIC_VOLUME * 2.0f},
    { SOUND_BANK_MASTER, "event:/Music/Finish/Game Over", BACKGROUND_MUSIC_VOLUME * 2.5f},
    { SOUND_BANK_MASTER, "event:/Music/Shop/Normal", BACKGROUND_MUSIC_VOLUME * 2.0f},
    { SOUND_BANK_MASTER, "event:/Music/Shop/Angry", BACKGROUND_MUSIC_VOLUME},
    { SOUND_BANK_MASTER, "event:/Music/Shop/Dead", BACKGROUND_MUSIC_VOLUME},
    { SOUND_BANK_MASTER, "event:/Music/Shop/Deserted", BACKGROUND_MUSIC_VOLUME},
    { SOUND_BANK_MASTER, "event:/Location/Fountain/Fountain Ambient", 0.75f},

    { SOUND_BANK_MASTER, "event:/Monster/Bee/Bee Ambient", 1.0f},
    { SOUND_BANK_MASTER, "event:/Location/Altar/Fire Ambient", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Quarterstaff/Quarterstaff Swing", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Quarterstaff/Quarterstaff Hit", 1.0f},
    { SOUND_BANK_MASTER, "event:/Ambience/Level/Valley Ambient", 0.10f},
    { SOUND_BANK_MASTER, "event:/Ambience/Room/Morgue Ambient", 0.10f},
    { SOUND_BANK_MASTER, "event:/Ambience/Room/Garden Ambient", 0.03f},
    { SOUND_BANK_MASTER, "event:/UI/Button Down", 0.15f},
    { SOUND_BANK_MASTER, "event:/UI/Menu Select", 0.15f},
    { SOUND_BANK_MASTER, "event:/SFX/Action/Quaff", 1.0f},

    { SOUND_BANK_MASTER, "event:/Location/Poison Gas/Poison Gas", 1.0f},
    { SOUND_BANK_MASTER, "event:/Ray/Electricity/Ray Electricity Ambient", 1.0f},
    { SOUND_BANK_MASTER, "event:/Ray/Electricity/Ray Electricity Create", 1.0f},
    { SOUND_BANK_MASTER, "event:/Ray/Electricity/Ray Electricity Destroy", 1.0f},
    { SOUND_BANK_MASTER, "event:/Ray/Electricity/Ray Electricity Bounce", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Bow/Bow Fire", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Arrow/Arrow Hit", 4.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Sword/Sword Swing", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Sword/Sword Hit", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Axe/Axe Swing", 1.0f},

    { SOUND_BANK_MASTER, "event:/Object/Weapon/Axe/Axe Hit", 1.0f},
    { SOUND_BANK_MASTER, "event:/Monster/Goblin/Goblin Death", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Dagger/Dagger Swing", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Dagger/Dagger Hit", 1.0f},
    { SOUND_BANK_MASTER, "event:/Monster/Human/Barehanded/Human Barehanded Swing", 1.0f},
    { SOUND_BANK_MASTER, "event:/Monster/Human/Barehanded/Human Barehanded Hit", 1.0f},
    { SOUND_BANK_MASTER, "event:/Monster/Human/Barefooted/Human Kick Swing", 1.0f},
    { SOUND_BANK_MASTER, "event:/Monster/Human/Barefooted/Human Kick Hit", 1.0f},
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Wham", 1.0f},
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Break", 1.0f},

    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Open", 0.1f},
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Close", 0.5f},
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Unlock", 0.5f},
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Lock", 0.5f},
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Resists", 0.5f},
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Try Locked", 0.5f},
    { SOUND_BANK_MASTER, "event:/Object/Tool/Chest/Chest Unlock", 0.5f},
    { SOUND_BANK_MASTER, "event:/Object/Tool/Chest/Chest Lock", 0.5f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Crossbow/Crossbow Fire", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/General/Coin/Coins Pick Up", 1.0f},

    { SOUND_BANK_MASTER, "event:/Object/General/Coin/Coins Drop", 1.0f},
    { SOUND_BANK_MASTER, "event:/Monster/Horse/Barefooted/Horse Footsteps", 0.3f},
    { SOUND_BANK_MASTER, "event:/Object/General/Boulder/Push Boulder", 1.0f},
    { SOUND_BANK_MASTER, "event:/Monster/Human/Male/Push Effort", 0.25f},
    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/Cause Paralysis", 1.0f},
    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/Cause Sleeping", 1.0f},
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Monster Death", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Tool/Chest/Chest Lock Break", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Tool/Chest/Chest Lid Slam", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Pick Up", 0.3f},

    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Drop", 0.5f},
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Throw", 0.5f},
    { SOUND_BANK_MASTER, "event:/Object/Armor/Cotton Slippers/Cotton Slipper Footsteps", 0.1f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Sword/Sword Drop", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Dagger/Dagger Drop", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Fly", 0.4f},
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Levitate", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Discard", 0.5f},
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Sparks Fly", 1.0f},
    { SOUND_BANK_MASTER, "event:/Object/Food/Apple/Apple Eat", 0.5f},

    { SOUND_BANK_MASTER, "event:/Object/Food/Carrot/Carrot Eat", 0.5f},
    { SOUND_BANK_MASTER, "event:/Monster/Human/Barefooted/Player Footsteps", 1.0f},
    { SOUND_BANK_MASTER, "event:/Occupation/Eating/Eating Ambient", 0.4f},
    { SOUND_BANK_MASTER, "event:/Occupation/Eating/Eating Start", 0.4f},
    { SOUND_BANK_MASTER, "event:/Occupation/Eating/Eating Finish", 0.3f},
    { SOUND_BANK_MASTER, "event:/Occupation/Eating/Eating Interrupted", 0.3f},
    { SOUND_BANK_MASTER, "event:/Occupation/Eating/Eating Resume", 0.4f},
    { SOUND_BANK_MASTER, "event:/Occupation/Digging/Digging Ambient", 0.3f },
    { SOUND_BANK_MASTER, "event:/Occupation/Digging/Digging Start", 0.3f },
    { SOUND_BANK_MASTER, "event:/Occupation/Digging/Digging Finish", 0.3f },

    { SOUND_BANK_MASTER, "event:/Occupation/Digging/Digging Interrupted", 0.3f },
    { SOUND_BANK_MASTER, "event:/Occupation/Digging/Digging Resume", 0.3f },
    { SOUND_BANK_MASTER, "event:/Occupation/Digging Ground/Digging Ground Ambient", 0.3f},
    { SOUND_BANK_MASTER, "event:/Occupation/Digging Ground/Digging Ground Start", 0.3f},
    { SOUND_BANK_MASTER, "event:/Occupation/Digging Ground/Digging Ground Finish", 0.3f},
    { SOUND_BANK_MASTER, "event:/Occupation/Digging Ground/Digging Ground Interrupted", 0.3f},
    { SOUND_BANK_MASTER, "event:/Occupation/Digging Ground/Digging Ground Resume", 0.3f},
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Desecrate Altar", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Summon Demon", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Sacrifice Pet", 0.3f },

    { SOUND_BANK_MASTER, "event:/SFX/Altar/Sacrifice Coaligned Unicorn", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Add Alignment", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Environment/Thunderclap", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Environment/Charged Air", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Accept Sacrifice", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Reject Sacrifice", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Power Increase", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Power Decrease", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/God Mollified", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Inadequacy", 0.3f },

    { SOUND_BANK_MASTER, "event:/SFX/Altar/Absolved", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Hopeful Feeling", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Reconciliation", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Gift", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Four-Leaf Clover", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Aura Glow", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Offering Burst of Flame", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Altar/Offering Vanish", 0.3f },
    { SOUND_BANK_MASTER, "event:/Monster/Dog/Dog Unhappy", 0.7f },

    { SOUND_BANK_MASTER, "event:/Monster/Cat/Cat Unhappy", 0.7f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Unhappy", 0.3f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Pray", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Fail", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Pleased", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Fix All Troubles", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Fix One Trouble", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Repair", 0.3f },

    { SOUND_BANK_MASTER, "event:/SFX/Pray/Healing", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Uncurse", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Gift", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Crowning", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Spellbook", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Bless Water", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Curse Water", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Pray/Shimmering Light", 0.3f },
    { SOUND_BANK_MASTER, "event:/Ray/Camera Flash/Camera Flash Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Camera Flash/Camera Flash Create", 1.0f },

    { SOUND_BANK_MASTER, "event:/Music/Quest-Normal", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/Vlad-Tower-Normal", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/Modron-Normal", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/Bovine-Normal", BACKGROUND_MUSIC_VOLUME * 1.5f },
    { SOUND_BANK_MASTER, "event:/Music/Large-Circular-Dungeon-Normal", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/End Game/Normal", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/End Game/Earth", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/End Game/Air", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/End Game/Water", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/End Game/Fire", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/End Game/Astral", BACKGROUND_MUSIC_VOLUME * 2.0f },

    { SOUND_BANK_MASTER, "event:/Music/Finish/Victory", BACKGROUND_MUSIC_VOLUME * 3.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Blessed Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Uncurse and Other", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Negative", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Violent Glow", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Vibrate Warning", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Vibrate and Destroy", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Special Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/Special Negative", 0.3f },

    { SOUND_BANK_MASTER, "event:/SFX/Spell/Enchant Item/General Fail", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Hands Itch", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Protect Item/Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Protect Item/Fail", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Repair Item/Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Repair Item/Fail", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Bless Item/Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Bless Item/Uncurse", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Curse Item/Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Curse Item/Unbless", 0.3f },

    { SOUND_BANK_MASTER, "event:/Music/Smith/Normal", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/Music/NPC/Normal", BACKGROUND_MUSIC_VOLUME * 2.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Disintegrate", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Life Saved", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Healing/Healing", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Healing/Full Healing", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Healing/Cure Disease", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Healing/Cure Ailment", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Mana/Gain Energy", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Mana/Full Energy", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Gain Level", 0.3f },

    { SOUND_BANK_MASTER, "event:/SFX/General/Gain Skill", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Lose Level", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Mana/Lose Energy", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Muffled Shatter", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Muffled Cracking", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Muffled Crash", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Muffled Splat", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Skill Advanced", 0.3f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Cast", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ambience/Environment/Deaf", 0.4f },

    { SOUND_BANK_MASTER, "event:/Ambience/Environment/Underwater", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Dust Vortex/Swallow Ambient", 0.5f },
    { SOUND_BANK_MASTER, "event:/Ray/Fire/Ray Fire Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Fire/Ray Fire Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Fire/Ray Fire Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Fire/Ray Fire Bounce", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Cold/Ray Cold Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Cold/Ray Cold Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Cold/Ray Cold Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Cold/Ray Cold Bounce", 1.0f },

    { SOUND_BANK_MASTER, "event:/Ray/Death/Ray Death Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Death/Ray Death Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Death/Ray Death Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Death/Ray Death Bounce", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/General Spell/General Spell Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/General Spell/General Spell Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Magic Missile/Ray Magic Missile Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Magic Missile/Ray Magic Missile Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Magic Missile/Ray Magic Missile Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Magic Missile/Ray Magic Missile Bounce", 1.0f },

    { SOUND_BANK_MASTER, "event:/Ray/Disintegration/Ray Disintegration Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Disintegration/Ray Disintegration Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Disintegration/Ray Disintegration Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Disintegration/Ray Disintegration Bounce", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Sleep/Ray Sleep Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Sleep/Ray Sleep Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Sleep/Ray Sleep Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Sleep/Ray Sleep Bounce", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Poison Gas/Ray Poison Gas Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Poison Gas/Ray Poison Gas Create", 1.0f },

    { SOUND_BANK_MASTER, "event:/Ray/Poison Gas/Ray Poison Gas Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Poison Gas/Ray Poison Gas Bounce", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Acid/Ray Acid Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Acid/Ray Acid Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Acid/Ray Acid Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Acid/Ray Acid Bounce", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Petrification/Ray Petrification Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Petrification/Ray Petrification Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Petrification/Ray Petrification Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Petrification/Ray Petrification Bounce", 1.0f },

    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Dark", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Noxious", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Muddy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Wet", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Magical", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Fiery", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Frosty", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/Surprise Attack", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Statue Becomes Alive", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/Was Hiding", 0.5f },

    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/Piercer Drops", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Trap Found", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Hidden Door Found", 0.2f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Unearthed Object Found", 0.2f },
    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/You Surprise Monster", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Arrow Trap Fire", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Dart Trap Fire", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Hole Opens", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Trap Door Opens", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Fall into Pit", 0.3f },

    { SOUND_BANK_MASTER, "event:/SFX/Trap/Land on Spikes", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Squeaky Board", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Falling Rock Trap Trigger", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Rolling Boulder Trap Trigger", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Teleport", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Bear Trap Closes", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Enveloped in Cloud of Gas", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Gush of Water Hits", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Steamy Bubbles", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Tower of Flame Erupts", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Trap/Teleport Trap Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Level Teleport Trap Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Level Teleport", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Shudder for Moment", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Wrenching Sensation", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Burn Spider Web", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Caught in Web", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Tear through Web", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Flash and Roar", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Shiver Runs Down Spine", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Trap/Distant Howling", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Magic Trap Weird Effect", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Pack Shaking", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Taming", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Magic Trap Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Anti Magic Trap Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Land Mine Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Polymorph Trap Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Magic Portal Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Vibrating Square/Vibrating Square Ambient", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Trap/Modron Glimmer Surrounds", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Modron Teleport Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Drain Energy", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Feeling Lethargic", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/You Rust", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/You Split", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Rock Hits Hard Helmet", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Rock Hits You on Head", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/General/Gain Intrinsic", 0.5f },

    { SOUND_BANK_MASTER, "event:/SFX/Trap/Generic Physical Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Generic Magical Activate", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ambience/Effect/Rolling Boulder", 1.0f },
    { SOUND_BANK_MASTER, "event:/UI/Knapsack Full", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Environment/Fall into Water", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Pet in Way", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Pet Does Not Move", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Fountain/Bubbling Water", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Fountain/Water Falling on Coins", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Fountain/Splashing of Naiad", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Level/Fountain/Soda Fountain", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Sink/Slow Drip", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Sink/Gurgling Noise", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Sink/Dishes Being Washed", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Throne/Courtly Conversation", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Throne/Sceptre Pounded", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Throne/Off With Head", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Throne/Queens Cats", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Swamp/Mosquitoes", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Swamp/Marsh Gas", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Level/Swamp/Donald Duck", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Vault/Someone Counting Money", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Vault/Quarterback Calling Play", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Vault/Someone Searching", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Vault/Footsteps of Guard Patrol", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Vault/Ebenezer Scrooge", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Beehive/Low Buzzing", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Beehive/Angry Drone", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Beehive/Bees in Bonnet", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Library/Someone Demanding Quietness", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Level/Library/Pages Turning in Head", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Dragon Lair/Coins Being Assembled", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Dragon Lair/Loud Dragon Snoring", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Dragon Lair/Loud Dragon Roar", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Dragon Lair/Fire and Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Barracks/Blades Being Honed", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Barracks/Loud Human Snoring", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Barracks/Dice Being Thrown", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Barracks/General McArthur", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Zoo/Elephant Stepping on Peanut", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Level/Zoo/Seal Breaking", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Zoo/Doctor Dolittle", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Shop/Someone Cursing Shoplifters", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Shop/Chime of Cash Register", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Shop/Neiman and Marcus Arguing", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Deserted Shop/Faint Chime", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Temple/Someone Praising God", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Temple/Someone Beseeching", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Temple/Animal Carcass Being Offered", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Temple/Plea For Donations", 0.3f },

    { SOUND_BANK_MASTER, "event:/SFX/Level/Smithy/Iron Being Forged", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Smithy/Loud Clanging", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Smithy/Water Being Vaporized", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/NPC/Someone Mumbling", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/NPC/Distant Chitchat", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/NPC/Footsteps at Distance", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Oracle/Strange Wind", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Oracle/Convulsive Ravings", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Oracle/Snoring Snakes", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Oracle/No More Woodchucks", 0.3f },

    { SOUND_BANK_MASTER, "event:/SFX/Level/Oracle/Loud Zot", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Armory/Swords Being Unsheated", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Armory/Armors Being Worn", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Armory/Weapons Being Assembled", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Level/Armory/Master Chief", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/Lightning Strikes", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Stoning", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Sliming", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Strangulation", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Suffocation", 0.5f },

    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Food Poisoning", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Terminal Illness", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Blindness", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Confusion", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Stun", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Hallucination", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Slow", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Paralysis", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Fear", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Sleep", 0.5f },

    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Cancellation", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Silence", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Grab", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Mummy Rot", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Condition/Acquired/Lycanthropy", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Gain Ability", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Lose Ability", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Restore Ability", 0.5f },
    { SOUND_BANK_MASTER, "event:/Explosion/Blinding Flash", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Clerical Monster/Scales Cover Eyes", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/General/Weapon Swapped", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Start Two Weapon Combat", 0.4f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Stop Two Weapon Combat", 0.4f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Electric Shock", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Malignant Aura Resisted", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Malignant Aura Surrounds", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Item Resists Negative Effect", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Summon Monster", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Vanishes in Puff of Smoke", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Disarm Trap Success", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Trap/Disarm Trap Fail", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/No Traps Found", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Cannot Disarm Trap", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/Stumble on Mimic", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Chest Trap Trigger", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Flame Fizzles Out", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Needle Misses", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Cloud Blows Away", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Needle Pricks", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Blasted by Power", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Action/Read", 0.5f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Identify Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Recharge/Recharge Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Recharge/Blessed Recharge Success", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Recharge/Strip Charges", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Recharge/Charges at Maximum", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Recharge/Recharge Fail", 0.3f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Wizard Lock Klunk", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Knock Klick", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Jump", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Trap/Lever Switch Fail", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Lever Switch Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Lever Switch On Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Lever Switch Off Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Digging Beam/Digging Beam Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Digging Beam/Digging Beam Create", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Digging Beam/Digging Beam Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Digging Beam/Digging Beam Hit Location", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Evaporation Beam/Evaporation Beam Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Evaporation Beam/Evaporation Beam Create", 1.0f },

    { SOUND_BANK_MASTER, "event:/Ray/Evaporation Beam/Evaporation Beam Destroy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ray/Evaporation Beam/Evaporation Beam Hit Location", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Wall Glows Then Fades", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Tree/Tree Break", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Stone/Stone Break", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Water/Water Break", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Consumed in Flames", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Door Freezes and Shatters", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Petrify", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Door/Generic/Electricity Hits Door", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Burnt", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Frozen", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Electrocuted", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Fountain/Fountain Dries Up", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Water/Some Water Evaporates", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Water/Water is Frozen", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Water/Water Grounds Electricity", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Water/Water Boils", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Water/Water Dries Up", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Chest/Chest Try Locked", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Chest/Chest Resists Open", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Key Does Not Fit", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Ice/Ice Melts", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Lava/Lava Hardens", 1.0f },
    { SOUND_BANK_MASTER, "event:/Ambience/Environment/Underwater", 0.2f },
    { SOUND_BANK_MASTER, "event:/Monster/Dust Vortex/Swallow Ambient", 0.04f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Item Vanishes", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Genocide", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Item Crumbles To Dust", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Laughter", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Generic/Evil Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Demonic Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Sinister Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Maniacal Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Hyena/Hyena Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Horse/Horse Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Ram/Ram Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Horse/Horse Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Horse/Horse Chat", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Ram/Ram Chat", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Dog/Dog Chat", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Cat/Cat Chat", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Ram/Ram Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Commerce/Sell to NPC", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Commerce/Buy from NPC", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Commerce/Sell to Shopkeeper", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Commerce/Buy from Shopkeeper", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Commerce/Seems Uninterested", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Commerce/Cannot Pay", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Commerce/Not Enough Money", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Commerce/Transact Single Item", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/You Cannot", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Wand/Generic/Wand Break", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Wand/Generic/Wand Pick Up", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Wand/Generic/Wand Drop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Fog Cloud/Fog Cloud Swallow Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Jelly/Jelly Swallow Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Luggage/Luggage Swallow Ambient", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/General Swallow Ambient", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Engrave", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Gem/Generic/Stone Engrave", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Bear/Bear Grunt", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Bear/Bear Chat", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Bear/Bear Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Human/Female/Push Effort", 0.25f },
    { SOUND_BANK_MASTER, "event:/SFX/Action/Zap", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Zap", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Sink/Clanking from Pipes", 1.0f },

    { SOUND_BANK_MASTER, "event:/Location/Sink/Snatches of Song", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Generic/Generic Bump Into", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Door/Wooden/Wooden Door Bump Into", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Male Flounder", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Female Flounder", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Male Ouch", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Female Ouch", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Male Search", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Female Search", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Male Yell", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Generic/Generic Female Yell", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Sex Change", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Unknown Odd Effect", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Celestial Summon Start", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Infernal Summon Start", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Neutral Summon Start", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/General Summon Start", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Summon Nasty Start", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Summon Undead Start", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Animate Dead Start", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Smoke Summon Start", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Polymorph Fail", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Polymorph Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Polymorph Scales Merge", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/General/Polymorph Scales Revert", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Polymorph New Man", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Wish Fulfilled", 1.0f },
    { SOUND_BANK_MASTER, "event:/Player/Generic/Male/Generic Male Shudder", 1.0f },
    { SOUND_BANK_MASTER, "event:/Player/Generic/Female/Generic Female Shudder", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Potion/Generic/Potion Break", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Wish for Object", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Stuck in Trap", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Wriggle Free", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Scroll/Rumbling Earth", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/General/Rumble of Distant Thunder", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Studio Audience Applauds", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Whistle/Whistle Normal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Whistle/Whistle Cursed", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Whistle/Whistle Underwater", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Magic Whistle/Magic Whistle Normal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Magic Whistle/Magic Whistle Cursed", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Magic Whistle/Magic Whistle Underwater", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Improvise Normal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Improvise Stunned", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Improvise Confused", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Improvise Hallucinated", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Improvise Far From Music", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Improvise Normal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Improvise Stunned", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Improvise Confused", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Improvise Hallucinated", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Improvise Far From Music", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Improvise Normal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Improvise Stunned", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Improvise Confused", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Improvise Hallucinated", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Improvise Far From Music", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Improvise Normal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Improvise Stunned", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Improvise Confused", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Improvise Hallucinated", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Improvise Far From Music", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Note A", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Note B", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Note C", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Note D", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Note E", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Note F", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Drum/Drum Note G", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Note A", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Note B", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Note C", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Note D", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Note E", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Note F", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Flute/Flute Note G", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Note A", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Note B", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Note C", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Note D", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Note E", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Note F", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Harp/Harp Note G", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Note A", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Note B", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Note C", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Note D", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Note E", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Note F", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Horn/Horn Note G", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Puzzle/Gear Turn", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Puzzle/Tumbler Click", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Drawbridge/Open Drawbridge", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Drawbridge/Close Drawbridge", 1.0f },

    { SOUND_BANK_MASTER, "event:/Location/Drawbridge/Drawbridge Starts to Rise", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Drawbridge/Drawbridge Gets Blocked", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/General Unaffected", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/General Resists", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Steed Refuses", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/General Reflects", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Drawbridge/Drawbridge Loud Splash", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Drawbridge/Drawbridge Loud Crash", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Food/Egg/Egg Hatch", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Drop After Throw", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Wear", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Take Off", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Wield", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Sword/Sword Wield", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Axe/Axe Wield", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Dagger/Dagger Wield", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Bow/Bow Wield", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Weapon/Crossbow/Crossbow Wield", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Weapon Swapping in Right Hand Only", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Weapon Swapping in Both Hands", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Unwield", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Quiver", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Unquiver", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Boulder/Boulder Drop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Boulder/Boulder Drop After Throw", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Rusts", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Gets Burnt", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Corrodes", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Item Rots", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Scroll Fades", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/General/Potion Dilutes", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Poison Dissolves", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Flames Extinquished", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Electricity Crackles", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Icy Coating Forms", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Magic Arrow Hit", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Mixing/Mixing Not Enough Components", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Mixing/Mixing Flares Up", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Mixing/Mixing Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Mixing/Mixing Failure", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/General/General Unharmed", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Burp", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Monster on Fire", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Monster Covered in Frost", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Monster Gets Zapped", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Monster Gets Splashed by Acid", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Sharpness Slice", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Steal Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Drain Life", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Steal Item", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/General/Monster is Poisoned", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Helmet Blocks Attack", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Life Leech", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Hail of Magic Missiles Misses", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Hail of Magic Missiles Hits", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/General/Not Enough Mana", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/General/Fail to Cast Correctly", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/General/Magical Energy Released", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Leash/Put on Leash", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Leash/Remove Leash", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Leash/Pulls Free of Leash", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Leash/Leash Snaps Loose", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Leash/Leash Goes Slack", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Leash/Fails to Leash", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Horse/Horse Unhappy", 0.7f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Learning/Spell Learn Fail", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Learning/Spell Learn Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Learning/Too Faint", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Learning/Spell Known Already", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Learning/Spell Keener", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Special Attack/Cancellation in Force", 1.0f },
    { SOUND_BANK_MASTER, "event:/Music/Start/Main Menu", 0.3f },

    { SOUND_BANK_MASTER, "event:/Location/Sink/Open Tap", 1.0f },
    { SOUND_BANK_MASTER, "event:/Location/Sink/Klunk", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Scolded", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Lava Burnt", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Hidden Treasure Found", 0.2f },
    { SOUND_BANK_MASTER, "event:/Location/Sink/Sink Break", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/You Do Not Know How", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Already Done", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Something Interesting There", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Nothing There", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Error/Does Not Fit", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Welded", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Bell/Bell Ring", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Bell/Muffled Bell Ring", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Out of Charges", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Bell of Opening/Cursed Bell of Opening Effect", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Bell of Opening/Uncursed Bell of Opening Effect", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Bell of Opening/Blessed Bell of Opening Effect", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Bell of Opening/Unsettling Shrill Sound", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Buff/Acquired/Haste", 0.5f },

    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Flaming Sphere", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Freezing Sphere", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Explosion Shocking Sphere", 1.0f },
    { SOUND_BANK_MASTER, "event:/Explosion/Hallucinating Flash", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Currently Unable to Do", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Monster Does Not Notice", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/That Did Nothing", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/That Is Silly", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Monster Is Busy", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Not At Right Location", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Error/Another Action Needed", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Not a Good Idea", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Cannot Reach", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Too Close", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Too Far", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Cannot See Spot", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Not Enough Leverage", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Current Form Does Not Allow", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Not Skilled Enough", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Too Much Encumbrance", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Error/Monster Does Not Allow", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Not Enough Stamina", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Item Welds", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Crunching Sound", 1.0f },
    { SOUND_BANK_MASTER, "event:/Player/Generic/Male/Generic Male Pray", 1.0f },
    { SOUND_BANK_MASTER, "event:/Player/Generic/Female/Generic Female Pray", 1.0f },
    { SOUND_BANK_MASTER, "event:/Player/Generic/Male/Generic Male Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Player/Generic/Female/Generic Female Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Monster Is Hit with Death Magic", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Weapon No Longer Poisoned", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/General/Weapon No Longer Enchanted", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Unluckily Your Touch Did Not Work", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Luckily Its Touch Did Not Work", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/You Resistance Success STR", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/You Resistance Success DEX", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/You Resistance Success CON", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/You Resistance Success INT", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/You Resistance Success WIS", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/You Resistance Success CHA", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/Enemy Resistance Success STR", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Resistance/Enemy Resistance Success DEX", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/Enemy Resistance Success CON", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/Enemy Resistance Success INT", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/Enemy Resistance Success WIS", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Resistance/Enemy Resistance Success CHA", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Vampire Transforms", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Gem/Glass/Glass Break", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Monster Is Hit with Celestial Magic", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Body Crumbles To Dust", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Error/Tried Action But It Failed", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Lamp/Lamp Turn On", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Lamp/Lamp Turn Off", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Lantern/Lantern Turn On", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Lantern/Lantern Turn Off", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Candle/Candle Light Up", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Candle/Candle Snuff Out", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Candelabrum/Attach Candle", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Not in the Right Condition", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Land Mine Set Up", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Bear Trap Set Up", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Alarm Sounds", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Trap/Escape Trap", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Coffin/Coffin Open", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Sarcophagus/Sarcophagus Open", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Nothing Found", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Disoriented for Moment", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Mysterious Force Prevents", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Armor/Helm of Opposite Alignment/Blocked Alignment Change", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Armor/Helm of Opposite Alignment/Alignment Change Conversion", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Armor/Helm of Opposite Alignment/Alignment Change Helm On", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Armor/Helm of Opposite Alignment/Alignment Change Helm Off", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Too Much to Comprehend", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Air Crackles", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Miscellaneous/Aggravate Monster", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Cat/Cat Happy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Dog/Dog Happy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Horse/Horse Happy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Ram/Ram Happy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Ram/Ram Unhappy", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Boulder/Tumble Downwards", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Boulder/Fills Pit", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Boulder/Plugs Hole", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Tentacled One/Wave of Psychic Energy", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Boss Fight", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Achievement", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Mount/Mount Fail Slip Away", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Mount/Mount Fail and Slip", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Mount/Mount Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Saddle/Saddle Apply", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Saddle/Saddle Drop", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Blind Telepathy", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Fire Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Cold Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Shock Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Poison Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Stone Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Sleep Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Acid Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Disintegration Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Death Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Charm Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Fear Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Mind Shielding", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Curse Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Teleport", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Teleport Control", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Telepathy", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/Lycanthropy Resistance", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Intrinsic/Acquired/General", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Potion/Gain Level/Cursed Effect", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Action/Rub", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Tinning Kit/Apply", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Lamp/Fill Oil Up", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/General/Generic/Generic Give", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Can of Grease/Apply", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Thump Hit", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Splash Hit", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Guilty", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Caitiff", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Warning", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Hint", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Revival", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Error/Not Ready Yet", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Artifact Name Known", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Action/Sit", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Action/Sit in Air", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Action/Tumble in Air", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/General/Protection End Warning", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/General/Negative Effect Ended", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Enter/Zoo", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Swamp", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Court", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Leprechaun Hall", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Morgue", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Morgue Midnight", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Beehive", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Garden", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Deserted Shop", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Library", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Dragon Lair", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Cockatrice Nest", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Anthole", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Barracks", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Abandoned Barracks", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Enter/Armory", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Intro/Intro Text", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Quests/Com Pager", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Quests/Quest Pager", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Quests/Quest Leader Who Are You", 1.0f },

    { SOUND_BANK_MASTER, "event:/SFX/Spell/Circle/Fire", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Circle/Frost", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Circle/Lightning", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Circle/Magic", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/Spell/Circle/Radiance", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Magic Marker/Write", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Magic Marker/Dry Out", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Magic Marker/Drop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Magic Marker/Pick Up", 1.0f },

    { SOUND_BANK_MASTER, "event:/Object/Tool/Crystal Ball/Use Success", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Crystal Ball/Use Fail", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Crystal Ball/Use Hallucinated", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Crystal Ball/Use Fail Hallucinated", 1.0f },
    { SOUND_BANK_MASTER, "event:/Object/Tool/Crystal Ball/Select Symbol", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Male/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Male/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Male/Item Trading", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Female/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Female/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Female/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Female/Item Trading", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Undead/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Undead/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Undead/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Undead/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Undead/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Shopkeeper/Undead/Item Trading", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Angel/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Angel/Male/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Angel/Female/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Angel/Female/Special Dialogue", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Imp/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Imp/Female/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Demon/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Demon/Female/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Wizard of Yendor/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Maledictions/Male Angel Malediction", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Maledictions/Female Angel Malediction", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Maledictions/Marilith Malediction", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Maledictions/Sandestin Malediction", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Maledictions/Male Imp Malediction", 0.65f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Maledictions/Female Imp Malediction", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor Malediction", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor Ill Be Back", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor I Shall Return", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor Destroy the Thief My Pet", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor Destroy the Thief My Pets", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor Double Trouble", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor So Thou Thought Thou Couldst Kill Me", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor So Thou Thought Thou Couldst Elude Me", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Wizard of Yendor/Wizard of Yendor Evil Laughter", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Shopkeeper Male Welcome", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Shopkeeper Male Welcome Back", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Shopkeeper Male Thank You for Shopping in My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Shopkeeper Male Invisible Customers Are Not Welcome", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Drop That Now", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Stay Away From Those", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Stay Away From That", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Cursing Shoplifters", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Id Hang onto That", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/I Wont Stock That", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Shopkeeper Female Welcome", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Shopkeeper Female Welcome Back", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Shopkeeper Female Thank You for Shopping in My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Shopkeeper Female Invisible Customers Are Not Welcome", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Drop That Now", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Stay Away From Those", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Stay Away From That", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Cursing Shoplifters", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Id Hang onto That", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/I Wont Stock That", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Shopkeeper Undead Welcome", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Shopkeeper Undead Welcome Back", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Shopkeeper Undead Thank You for Shopping in My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Shopkeeper Undead Invisible Customers Are Not Welcome", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Drop That Now", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Stay Away From Those", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Stay Away From That", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Cursing Shoplifters", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Id Hang onto That", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/I Wont Stock That", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Leave Stuff Outside/Leave Stuff Outside", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Leave Stuff Outside/Leave Stuff Outside", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Leave Stuff Outside/Leave Stuff Outside", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Leave Stuff Outside/Will You Please Leave Stuff Outside", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Leave Stuff Outside/Will You Please Leave Stuff Outside", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Leave Stuff Outside/Will You Please Leave Stuff Outside", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Cad/Sneaky", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Cad/Sneaky", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Cad/Sneaky", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Candles/More Candles", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Candles/More Candles", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Candles/More Candles", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Thank You Scum", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Thank You for Your Contribution", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Out of My Way Scum", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Get Your Junk Out of My Wall", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/I Was Looking for Someone Else", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Didnt You Forget to Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Adventurer Didnt You Forget to Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Be Careful Sir Fall Through the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Be Careful Madam Fall Through the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Sir Do Not Damage the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Madam Do Not Damage the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Please Pay Before Leaving", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Dont You Leave Before Paying", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Dare to Return to My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Pay for Other/Pay for the Other Item Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Pay for Other/Pay for the Other Item Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Pay for Other/Pay for the Other Items Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Pay for Other/Pay for the Other Items Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Pay for Other/Please Pay for the Other Item Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Pay for Other/Please Pay for the Other Item Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Pay for Other/Please Pay for the Other Items Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Pay for Other/Please Pay for the Other Items Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Welcome to My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Welcome Back to My Store", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Thank You Scum", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Thank You for Your Contribution", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Out of My Way Scum", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Get Your Junk Out of My Wall", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/I Was Looking for Someone Else", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Didnt You Forget to Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Adventurer Didnt You Forget to Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Be Careful Sir Fall Through the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Be Careful Madam Fall Through the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Sir Do Not Damage the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Madam Do Not Damage the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Please Pay Before Leaving", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Dont You Leave Before Paying", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Dare to Return to My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Pay for Other/Pay for the Other Item Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Pay for Other/Pay for the Other Item Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Pay for Other/Pay for the Other Items Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Pay for Other/Pay for the Other Items Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Pay for Other/Please Pay for the Other Item Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Pay for Other/Please Pay for the Other Item Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Pay for Other/Please Pay for the Other Items Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Pay for Other/Please Pay for the Other Items Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Welcome to My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Welcome Back to My Store", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Thank You Scum", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Thank You for Your Contribution", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Out of My Way Scum", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Get Your Junk Out of My Wall", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/I Was Looking for Someone Else", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Didnt You Forget to Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Adventurer Didnt You Forget to Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Be Careful Sir Fall Through the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Be Careful Madam Fall Through the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Sir Do Not Damage the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Madam Do Not Damage the Floor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Please Pay Before Leaving", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Dont You Leave Before Paying", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Dare to Return to My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Pay for Other/Pay for the Other Item Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Pay for Other/Pay for the Other Item Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Pay for Other/Pay for the Other Items Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Pay for Other/Pay for the Other Items Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Pay for Other/Please Pay for the Other Item Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Pay for Other/Please Pay for the Other Item Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Pay for Other/Please Pay for the Other Items Before This One", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Pay for Other/Please Pay for the Other Items Before These", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Welcome to My Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Welcome Back to My Store", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/How Dare You Damage/How Dare You Damage Shop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/How Dare You Damage/How Dare You Damage Door", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/How Dare You Damage/Who Dared Damage Shop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/How Dare You Damage/Who Dared Damage Door", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/How Dare You Damage/How Dare You Damage Shop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/How Dare You Damage/How Dare You Damage Door", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/How Dare You Damage/Who Dared Damage Shop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/How Dare You Damage/Who Dared Damage Door", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/How Dare You Damage/How Dare You Damage Shop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/How Dare You Damage/How Dare You Damage Door", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/How Dare You Damage/Who Dared Damage Shop", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/How Dare You Damage/Who Dared Damage Door", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Oh Yes Youll Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Didnt You Forget to Pay 2", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Whoa", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Watch It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Hey", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Ahem", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Owe Me Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Owe Me Some Additional Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/That Will Cost You Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Emptying That Will Cost You Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Usage Fee Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Did Lot of Damage", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Oh Yes Youll Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Didnt You Forget to Pay 2", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Whoa", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Watch It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Hey", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Ahem", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Owe Me Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Owe Me Some Additional Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/That Will Cost You Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Emptying That Will Cost You Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Usage Fee Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Did Lot of Damage", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Oh Yes Youll Pay", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Didnt You Forget to Pay 2", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Whoa", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Watch It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Hey", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Ahem", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Owe Me Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Owe Me Some Additional Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/That Will Cost You Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Emptying That Will Cost You Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Usage Fee Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Did Lot of Damage", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Cad/No Free Library", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Cad/No Free Library", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Cad/No Free Library", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Cad/Lot of Damage", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Close Sesame", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Need to Pay Lot of Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Damn It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Cursed Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Cursed Vandal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Dare to Return", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Pay Gold in Compensation", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Dislike Nonpaying Customers", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Dislike Rude Customers", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Was Robbed", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Sir Owe Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Madam Owe Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Using Outstanding Credit", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Business is Bad", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Business is Good", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Problem with Shoplifters", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Im the Shopkeeper", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/I Run This Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Izchak Talks", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/For You/Peaceful", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/For You/Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Thats a Deal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/On Second Thought", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Your Bill Comes To This Amount", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Burn It Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Burn Them Bought Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Use It Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Use Them Bought Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Seen Untended Shops", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Tin It You Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/In Addition to Cost of Item Itself", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/In Addition to Cost of Items Themselves", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/In Addition to Cost of Potion", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/No Checks No Credit No Problem", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Alter That You Pay for It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/You Alter Those You Pay for Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/General/Ill Add That to Your Bill", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Male/Costly Alteration/Alter Pay", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Cad/Lot of Damage", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Close Sesame", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Need to Pay Lot of Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Damn It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Cursed Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Cursed Vandal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Dare to Return", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Pay Gold in Compensation", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Dislike Nonpaying Customers", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Dislike Rude Customers", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Was Robbed", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Sir Owe Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Madam Owe Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Using Outstanding Credit", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Business is Bad", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Business is Good", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Problem with Shoplifters", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Im the Shopkeeper", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/I Run This Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Izchak Talks", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/For You/Peaceful", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/For You/Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Thats a Deal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/On Second Thought", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Your Bill Comes To This Amount", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Burn It Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Burn Them Bought Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Use It Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Use Them Bought Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Seen Untended Shops", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Tin It You Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/In Addition to Cost of Item Itself", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/In Addition to Cost of Items Themselves", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/In Addition to Cost of Potion", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/No Checks No Credit No Problem", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Alter That You Pay for It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/You Alter Those You Pay for Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/General/Ill Add That to Your Bill", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Female/Costly Alteration/Alter Pay", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Cad/Lot of Damage", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Close Sesame", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Need to Pay Lot of Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Damn It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Cursed Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Cursed Vandal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Thief", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Dare to Return", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Pay Gold in Compensation", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Dislike Nonpaying Customers", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Dislike Rude Customers", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Was Robbed", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Sir Owe Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Madam Owe Some Gold", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Using Outstanding Credit", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Business is Bad", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Business is Good", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Problem with Shoplifters", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Im the Shopkeeper", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/I Run This Store", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Izchak Talks", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/For You/Peaceful", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/For You/Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Thats a Deal", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/On Second Thought", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Your Bill Comes To This Amount", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Burn It Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Burn Them Bought Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Use It Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Use Them Bought Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Seen Untended Shops", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Tin It You Bought It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/In Addition to Cost of Item Itself", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/In Addition to Cost of Items Themselves", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/In Addition to Cost of Potion", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/No Checks No Credit No Problem", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Alter That You Pay for It", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/You Alter Those You Pay for Them", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/General/Ill Add That to Your Bill", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Shopkeeper/Undead/Costly Alteration/Alter Pay", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Student/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Student/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Student/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Student/Female/Advice", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Chieftain/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Chieftain/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Chieftain/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Chieftain/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Chieftain/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Chieftain/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Neanderthal/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Neanderthal/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Neanderthal/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Neanderthal/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Neanderthal/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Neanderthal/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Attendant/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Attendant/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Attendant/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Attendant/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Attendant/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Attendant/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Page/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Page/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Page/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Page/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Page/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Page/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Abbot/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Abbot/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Abbot/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Abbot/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Abbot/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Abbot/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Acolyte/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Acolyte/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Acolyte/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Acolyte/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Acolyte/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Acolyte/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Hunter/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Hunter/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Hunter/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Hunter/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Hunter/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Hunter/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Thug/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Thug/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Thug/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Thug/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Thug/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Thug/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Roshi/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Roshi/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Roshi/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Roshi/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Roshi/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Roshi/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Guide/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Guide/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Guide/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Guide/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Guide/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Guide/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Warrior/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Warrior/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Warrior/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Warrior/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Warrior/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Warrior/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Apprentice/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Apprentice/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Apprentice/Male/Death", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Apprentice/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Apprentice/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Apprentice/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Male/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Male/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Male/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Male/Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Male/Mumbled Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Male/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Female/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Female/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Female/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Female/Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Female/Mumbled Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Moloch/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Male/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Male/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Male/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Male/Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Male/Mumbled Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Male/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Female/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Female/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Female/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Female/Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Female/Mumbled Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Priest/Normal/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/God/Male/God Speaks", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/God/Female/God Speaks", 1.0f },
    { SOUND_BANK_MASTER, "event:/SFX/General/Invisible Choir Sings", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Male/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Male/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Male/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Male/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Male/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Female/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Female/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Female/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Female/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Flind/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Flind/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Flind/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Flind/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Flind/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Flind/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Flind/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Undead/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Undead/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Undead/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Undead/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Undead/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Undead/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Smith/Undead/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Mumbled Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Male/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Mumbled Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Mumbled Curse", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Artificer/Undead/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Male/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Male/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Male/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Male/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Male/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Female/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Female/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Female/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Female/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Undead/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Undead/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Undead/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Undead/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Undead/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Undead/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Geologist/Undead/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Djinn/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Djinn/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Djinn/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Djinn/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Male/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Male/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Male/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Male/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Female/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Female/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Female/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Watchman/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Oracle/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Oracle/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Oracle/Major Consultation", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Oracle/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Oracle/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Oracle/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Keystone Kop/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Keystone Kop/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Keystone Kop/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Male/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Male/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Male/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Male/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Male/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Male/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Female/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Female/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Female/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Female/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Female/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Neutral/Vault Guard/Female/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/Throne Room/Audience Summoned Dame", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Throne Room/Audience Summoned Sire", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Throne Room/By Thine Imperious Order Dame", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Throne Room/By Thine Imperious Order Sire", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Throne Room/Curse Upon Thee", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/Throne Room/Thank You for Your Contribution", 1.0f },

    { SOUND_BANK_MASTER, "event:/Music/Elven Bard/A Elbereth Gilthoniel", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Dungeons", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Quests", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Gnomish Mines", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Sokoban", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Castle", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Gehennom", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Wizard of Yendor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Further Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit1/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Castle", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Gehennom", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Wizard of Yendor", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Vampire Lord", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Silver Bell", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Candelabrum", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Book of the Dead", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Ritual", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit2/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit3/Gnomish Mines", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit3/Luckstone", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit3/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit3/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit3/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/Hermit3/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/OrcHermit3/Gnomish Mines", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/OrcHermit3/Luckstone", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/OrcHermit3/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/OrcHermit3/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/OrcHermit3/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Hermits/OrcHermit3/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Elven Bard/Elbereth", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Elven Bard/Hear This Song", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Elven Bard/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Elven Bard/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Elven Bard/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Elven Bard/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Teleportation", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Quantum Experiments", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Large Circular Dungeon", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Special Wand", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Disintegration Wand", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Teleportation Wand", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Item Trading", 1.0f },
    { SOUND_BANK_MASTER, "event:/Voice Acting/NPCs/Quantum Mechanic/Death", 1.0f },

    { SOUND_BANK_MASTER, "event:/Monster/Yeenaghu/Standard Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Yeenaghu/Special Dialogue", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Yeenaghu/Get Angry", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Yeenaghu/Mollified", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Yeenaghu/Advice", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Yeenaghu/Laughter", 1.0f },
    { SOUND_BANK_MASTER, "event:/Monster/Yeenaghu/Death", 1.0f },
};
/*

    GHSOUND_HERMIT3_GNOMISH_MINES,
    GHSOUND_HERMIT3_LUCKSTONE,
    GHSOUND_HERMIT3_STANDARD_DIALOGUE,
    GHSOUND_HERMIT3_SPECIAL_DIALOGUE,
    GHSOUND_HERMIT3_ITEM_TRADING,
    GHSOUND_HERMIT3_DEATH,

    GHSOUND_ORC_HERMIT3_GNOMISH_MINES,
    GHSOUND_ORC_HERMIT3_LUCKSTONE,
    GHSOUND_ORC_HERMIT3_STANDARD_DIALOGUE,
    GHSOUND_ORC_HERMIT3_SPECIAL_DIALOGUE,
    GHSOUND_ORC_HERMIT3_ITEM_TRADING,
    GHSOUND_ORC_HERMIT3_DEATH,
*/

NEARDATA struct player_soundset_definition player_soundsets[MAX_PLAYER_SOUNDSETS] =
{
    {
        "",
        PLAYER_SOUNDSET_NONE,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},{GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},  {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},  {GHSOUND_NONE, 0.0f},{GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE},
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},  {GHSOUND_NONE, 0.0f},  {GHSOUND_NONE, 0.0f}}
    },
    {
        "Generic Male",
        PLAYER_SOUNDSET_NONE,
        {
            {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_PUSH_EFFORT, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_OUCH_MALE, 1.0f}, {GHSOUND_GENERIC_FLOUNDER_MALE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f},
            {GHSOUND_NONE, 1.0f}, {GHSOUND_GENERIC_BUMP_INTO, 1.0f}, {GHSOUND_GENERIC_SEARCH_MALE, 1.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 1.0f},
            {GHSOUND_GENERIC_LAUGHTER, 10.0f},  {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_MALE, 1.0f}, {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_MALE_SHUDDER, 1.0f}, {GHSOUND_GENERIC_PLAYER_MALE_DEATH, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_HUMAN_BAREFOOTED},
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},  {GHSOUND_NONE, 0.0f},  {GHSOUND_GENERIC_PLAYER_MALE_PRAY, 1.0f}}
    },
    {
        "Generic Female",
        PLAYER_SOUNDSET_GENERAL,
        {
            {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_PUSH_EFFORT_FEMALE, 1.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_OUCH_FEMALE, 1.0f}, {GHSOUND_GENERIC_FLOUNDER_FEMALE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f},
            {GHSOUND_NONE, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_SEARCH_FEMALE, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_FEMALE, 1.0f}, {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_FEMALE_SHUDDER, 1.0f}, {GHSOUND_GENERIC_PLAYER_FEMALE_DEATH, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_HUMAN_BAREFOOTED},
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},  {GHSOUND_NONE, 0.0f},  {GHSOUND_GENERIC_PLAYER_FEMALE_PRAY, 1.0f}}
    }
};


NEARDATA struct monster_soundset_definition monster_soundsets[MAX_MONSTER_SOUNDSETS] =
{
    {
        "",
        MONSTER_SOUNDSET_NONE,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},{GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE, OBJECT_SOUNDSET_NONE}
    },
    {
        "Generic",
        MONSTER_SOUNDSET_NONE,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_FLOUNDER_MALE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_GENERIC_BUMP_INTO, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_LAUGHTER, 10.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_MALE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_MONSTER_DEATH_GENERIC, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Human Male",
        MONSTER_SOUNDSET_GENERIC,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_FLOUNDER_MALE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_GENERIC_BUMP_INTO, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_GENERIC_LAUGHTER, 10.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_MALE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_PLAYER_MALE_DEATH, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Human Female",
        MONSTER_SOUNDSET_GENERIC,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f},  {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_FLOUNDER_FEMALE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_GENERIC_BUMP_INTO, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 10.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_FEMALE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_PLAYER_FEMALE_DEATH, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Goblin",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_GENERIC_LAUGHTER, 10.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {GHSOUND_GOBLIN_DEATH, 2.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Dragon",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_DEMONIC_LAUGHTER, 10.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {GHSOUND_MONSTER_DEATH_GENERIC, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Dracolich",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_DEMONIC_LAUGHTER, 10.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {GHSOUND_MONSTER_DEATH_GENERIC, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}    },
    {
        "Bee",
        MONSTER_SOUNDSET_GENERIC,
        {{GHSOUND_BEE, 1.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {GHSOUND_MONSTER_DEATH_GENERIC, 2.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Horse",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_HORSE_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_HORSE_HAPPY, 1.0f}, {GHSOUND_HORSE_LAUGHTER, 1.0f}, {GHSOUND_HORSE_CHAT, 1.0f}, {GHSOUND_HORSE_CHAT, 0.0f},{GHSOUND_HORSE_CHAT, 1.0f}, {GHSOUND_HORSE_CHAT, 1.0f}, {GHSOUND_HORSE_CHAT, 1.0f}, {GHSOUND_HORSE_CHAT, 1.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {GHSOUND_HORSE_DEATH, 3.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HORSE_BAREFOOTED}
    },
    {
        "Dog",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_DOG_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_DOG_HAPPY, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_DOG_CHAT, 1.0f}, {GHSOUND_DOG_CHAT, 1.0f}, {GHSOUND_DOG_CHAT, 1.0f}, {GHSOUND_DOG_CHAT, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {GHSOUND_MONSTER_DEATH_GENERIC, 3.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Cat",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_CAT_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_CAT_HAPPY, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_CAT_CHAT, 1.0f}, {GHSOUND_CAT_CHAT, 1.0f}, {GHSOUND_CAT_CHAT, 1.0f}, {GHSOUND_CAT_CHAT, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {GHSOUND_MONSTER_DEATH_GENERIC, 2.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Ram",
        MONSTER_SOUNDSET_HORSE,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_RAM_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_RAM_HAPPY, 1.0f}, {GHSOUND_RAM_CHAT, 1.0f}, {GHSOUND_RAM_CHAT, 1.0f}, {GHSOUND_RAM_CHAT, 1.0f}, {GHSOUND_RAM_CHAT, 1.0f}, {GHSOUND_RAM_CHAT, 1.0f}, {GHSOUND_RAM_CHAT, 1.0f}, {GHSOUND_RAM_CHAT, 1.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {GHSOUND_RAM_DEATH, 2.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HORSE_BAREFOOTED}
    },
    {
        "Hyena",
        MONSTER_SOUNDSET_DOG,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_HYENA_LAUGHTER, 8.0f}, {GHSOUND_HYENA_LAUGHTER, 1.0f}, {GHSOUND_HYENA_LAUGHTER, 1.0f}, {GHSOUND_HYENA_LAUGHTER, 1.0f}, {GHSOUND_HYENA_LAUGHTER, 1.0f}, {GHSOUND_HYENA_LAUGHTER, 1.0f}, {GHSOUND_HYENA_LAUGHTER, 1.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Wizard of Yendor",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_VOICE_WIZARD_OF_YENDOR_EVIL_LAUGHTER, 10.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {GHSOUND_WIZARD_OF_YENDOR_DEATH, 4.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Human-like demon",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_VOICE_SANDESTIN_MALEDICTION, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_DEMONIC_LAUGHTER, 10.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {GHSOUND_DEMON_DEATH, 4.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Human-like demoness",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_VOICE_MARILITH_MALEDICTION, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_DEMONIC_LAUGHTER, 10.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {GHSOUND_DEMONESS_DEATH, 4.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Vortex",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {GHSOUND_DUST_VORTEX_SWALLOW_AMBIENT, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Fog cloud",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {GHSOUND_FOG_CLOUD_SWALLOW_AMBIENT, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Jelly",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {GHSOUND_JELLY_SWALLOW_AMBIENT, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Luggage",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {GHSOUND_LUGGAGE_SWALLOW_AMBIENT, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "General swallower",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Bear",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_BEAR_LAUGHTER, 1.0f}, {GHSOUND_BEAR_LAUGHTER, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_BEAR_GRUNT, 1.0f}, {GHSOUND_BEAR_CHAT, 1.0f}, {GHSOUND_BEAR_GRUNT, 1.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Angel Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_VOICE_MALE_ANGEL_MALEDICTION_WITH_GOD_NAME, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_MALE_ANGEL_SPECIAL_DIALOGUE, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_FLOUNDER_MALE, 1.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_BUMP_INTO, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_GENERIC_LAUGHTER, 10.0f}, {GHSOUND_NONE, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_MALE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_PLAYER_MALE_DEATH, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Angel Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_VOICE_FEMALE_ANGEL_MALEDICTION_WITH_GOD_NAME, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_FEMALE_ANGEL_SPECIAL_DIALOGUE, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_FLOUNDER_FEMALE, 1.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_BUMP_INTO, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 10.0f}, {GHSOUND_NONE, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_FEMALE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_PLAYER_FEMALE_DEATH, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Male Imp",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_VOICE_MALE_IMP_MALEDICTION, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_DEMONIC_LAUGHTER, 10.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {GHSOUND_MALE_IMP_DEATH, 4.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Female Imp",
        MONSTER_SOUNDSET_GENERIC,
        {{MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_VOICE_FEMALE_IMP_MALEDICTION, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {MAX_GHSOUNDS, 1.0f}, {GHSOUND_DEMONIC_LAUGHTER, 10.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f},  {MAX_GHSOUNDS, 0.0f}, {GHSOUND_FEMALE_IMP_DEATH, 4.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Shopkeeper Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_SHOPKEEPER_MALE_ITEM_TRADING, 1.0f}, {GHSOUND_NONE, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_SHOPKEEPER_MALE_GET_ANGRY, 1.0f}, {GHSOUND_SHOPKEEPER_MALE_MOLLIFIED, 1.0f}, {GHSOUND_SHOPKEEPER_MALE_ADVICE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_FLOUNDER_MALE, 1.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_BUMP_INTO, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_GENERIC_LAUGHTER, 10.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_MALE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_SHOPKEEPER_MALE_DEATH, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Shopkeeper Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {{GHSOUND_NONE, 0.0f}, {GHSOUND_GENERAL_SWALLOW_AMBIENT, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_SHOPKEEPER_FEMALE_ITEM_TRADING, 1.0f}, {GHSOUND_NONE, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_SHOPKEEPER_FEMALE_GET_ANGRY, 1.0f}, {GHSOUND_SHOPKEEPER_FEMALE_MOLLIFIED, 1.0f}, {GHSOUND_SHOPKEEPER_FEMALE_ADVICE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_UNHAPPY, 1.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_FLOUNDER_FEMALE, 1.0f},  {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {MAX_GHSOUNDS, 0.0f}, {GHSOUND_GENERIC_BUMP_INTO, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_CAST, 1.0f}, {GHSOUND_NONE, 1.0f}, {GHSOUND_NONE, 10.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_GENERIC_YELL_FEMALE, 1.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_NONE, 0.0f}, {GHSOUND_SHOPKEEPER_FEMALE_DEATH, 5.0f}},
        SOUNDSOURCE_AMBIENT_GENERAL,
        {OBJECT_SOUNDSET_HUMAN_BAREHANDED, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_GENERIC, OBJECT_SOUNDSET_HUMAN_BAREFOOTED}
    },
    {
        "Shopkeeper Undead",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_SHOPKEEPER_UNDEAD_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_SHOPKEEPER_UNDEAD_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_SHOPKEEPER_UNDEAD_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_SHOPKEEPER_UNDEAD_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_SHOPKEEPER_UNDEAD_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Smith Male",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_SMITH_MALE_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_SMITH_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_SMITH_MALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_SMITH_MALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_SMITH_MALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_SMITH_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_SMITH_MALE_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Smith Female",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_SMITH_FEMALE_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_SMITH_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_SMITH_FEMALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_SMITH_FEMALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_SMITH_FEMALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_SMITH_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_SMITH_FEMALE_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Smith Undead",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_SMITH_UNDEAD_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_SMITH_UNDEAD_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_SMITH_UNDEAD_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_SMITH_UNDEAD_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_SMITH_UNDEAD_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_SMITH_UNDEAD_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_SMITH_UNDEAD_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Smith Flind",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_SMITH_FLIND_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_SMITH_FLIND_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_SMITH_FLIND_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_SMITH_FLIND_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_SMITH_FLIND_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_SMITH_FLIND_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_SMITH_FLIND_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Student Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_STUDENT_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_STUDENT_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {MAX_GHSOUNDS, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Student Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_STUDENT_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_STUDENT_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {MAX_GHSOUNDS, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Chieftain Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_CHIEFTAIN_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_CHIEFTAIN_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_CHIEFTAIN_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Chieftain Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_CHIEFTAIN_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_CHIEFTAIN_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_CHIEFTAIN_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Neanderthal Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_NEANDERTHAL_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_NEANDERTHAL_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_NEANDERTHAL_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Neanderthal Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_NEANDERTHAL_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_NEANDERTHAL_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_NEANDERTHAL_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Attendant Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ATTENDANT_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ATTENDANT_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ATTENDANT_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Attendant Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ATTENDANT_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ATTENDANT_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ATTENDANT_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Page Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_PAGE_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_PAGE_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_PAGE_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Page Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_PAGE_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_PAGE_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_PAGE_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Abbot Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ABBOT_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ABBOT_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ABBOT_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Abbot Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ABBOT_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ABBOT_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ABBOT_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Acolyte Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ACOLYTE_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ACOLYTE_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ACOLYTE_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Acolyte Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ACOLYTE_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ACOLYTE_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ACOLYTE_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Hunter Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_HUNTER_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_HUNTER_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_HUNTER_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Hunter Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_HUNTER_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_HUNTER_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_HUNTER_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Thug Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_THUG_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_THUG_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_THUG_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Thug Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_THUG_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_THUG_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_THUG_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Roshi Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ROSHI_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ROSHI_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ROSHI_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Roshi Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ROSHI_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ROSHI_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ROSHI_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Guide Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_GUIDE_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_GUIDE_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_GUIDE_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Guide Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_GUIDE_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_GUIDE_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_GUIDE_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Warrior Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_WARRIOR_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_WARRIOR_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_WARRIOR_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Warrior Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_WARRIOR_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_WARRIOR_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_WARRIOR_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Apprentice Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_APPRENTICE_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_APPRENTICE_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_APPRENTICE_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Apprentice Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_APPRENTICE_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_APPRENTICE_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_APPRENTICE_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Aligned Priest",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ALIGNED_PRIEST_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_ALIGNED_PRIEST_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_ALIGNED_PRIEST_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_ALIGNED_PRIEST_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ALIGNED_PRIEST_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {GHSOUND_VOICE_ALIGNED_PRIEST_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {GHSOUND_VOICE_ALIGNED_PRIEST_MUMBLED_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ALIGNED_PRIEST_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Aligned Priestess",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ALIGNED_PRIESTESS_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_ALIGNED_PRIESTESS_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_ALIGNED_PRIESTESS_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_ALIGNED_PRIESTESS_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ALIGNED_PRIESTESS_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {GHSOUND_VOICE_ALIGNED_PRIESTESS_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {GHSOUND_VOICE_ALIGNED_PRIESTESS_MUMBLED_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ALIGNED_PRIESTESS_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Priest of Moloch",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_PRIEST_OF_MOLOCH_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_PRIEST_OF_MOLOCH_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_PRIEST_OF_MOLOCH_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_PRIEST_OF_MOLOCH_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_PRIEST_OF_MOLOCH_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {GHSOUND_VOICE_PRIEST_OF_MOLOCH_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {GHSOUND_VOICE_PRIEST_OF_MOLOCH_MUMBLED_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_PRIEST_OF_MOLOCH_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */

        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Priestess of Moloch",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_PRIESTESS_OF_MOLOCH_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_PRIESTESS_OF_MOLOCH_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_PRIESTESS_OF_MOLOCH_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_PRIESTESS_OF_MOLOCH_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_PRIESTESS_OF_MOLOCH_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {GHSOUND_VOICE_PRIESTESS_OF_MOLOCH_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {GHSOUND_VOICE_PRIESTESS_OF_MOLOCH_MUMBLED_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_PRIESTESS_OF_MOLOCH_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Artificer Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_ARTIFICER_MALE_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ARTIFICER_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_ARTIFICER_MALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_ARTIFICER_MALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_ARTIFICER_MALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ARTIFICER_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {GHSOUND_VOICE_ARTIFICER_MALE_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {GHSOUND_VOICE_ARTIFICER_MALE_MUMBLED_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ARTIFICER_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Artificer Female",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_MUMBLED_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ARTIFICER_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Artificer Undead",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_MUMBLED_CURSE, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ARTIFICER_UNDEAD_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    }, 
    {
        "Geologist Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_GEOLOGIST_MALE_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_GEOLOGIST_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_GEOLOGIST_MALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_GEOLOGIST_MALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_GEOLOGIST_MALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_GEOLOGIST_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_GEOLOGIST_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Geologist Female",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_GEOLOGIST_FEMALE_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_GEOLOGIST_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_GEOLOGIST_FEMALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_GEOLOGIST_FEMALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_GEOLOGIST_FEMALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_GEOLOGIST_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_GEOLOGIST_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Geologist Undead",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_VOICE_GEOLOGIST_UNDEAD_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_GEOLOGIST_UNDEAD_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_GEOLOGIST_UNDEAD_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_GEOLOGIST_UNDEAD_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_GEOLOGIST_UNDEAD_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_GEOLOGIST_UNDEAD_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_GEOLOGIST_UNDEAD_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Djinn",
        MONSTER_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_DJINN_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_DJINN_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_DJINN_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_DJINN_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Watchman Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_WATCHMAN_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_WATCHMAN_MALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_WATCHMAN_MALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_WATCHMAN_MALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_WATCHMAN_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_WATCHMAN_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Watchman Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_WATCHMAN_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_WATCHMAN_FEMALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_WATCHMAN_FEMALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_WATCHMAN_FEMALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_WATCHMAN_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_WATCHMAN_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Oracle",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_ORACLE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_ORACLE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_ORACLE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_ORACLE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_ORACLE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Keystone Kop",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_KEYSTONE_KOP_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_KEYSTONE_KOP_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_KEYSTONE_KOP_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Vault Guard Male",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_VAULT_GUARD_MALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_VAULT_GUARD_MALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_VAULT_GUARD_MALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_VAULT_GUARD_MALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_VAULT_GUARD_MALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_VAULT_GUARD_MALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Vault Guard Female",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_VOICE_VAULT_GUARD_FEMALE_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_VOICE_VAULT_GUARD_FEMALE_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_VOICE_VAULT_GUARD_FEMALE_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_VOICE_VAULT_GUARD_FEMALE_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_VOICE_VAULT_GUARD_FEMALE_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_VOICE_VAULT_GUARD_FEMALE_DEATH, 1.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Hermit1",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_HERMIT1_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_HERMIT1_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_HERMIT1_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_HERMIT1_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Hermit2",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_HERMIT2_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_HERMIT2_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_HERMIT2_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_HERMIT2_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    }, 
    {
        "Hermit3",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_HERMIT3_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_HERMIT3_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_HERMIT3_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_HERMIT3_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    }, 
    {
        "OrcHermit3",
        MONSTER_SOUNDSET_SHOPKEEPER_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_ORC_HERMIT3_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_ORC_HERMIT3_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_ORC_HERMIT3_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_ORC_HERMIT3_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Elven Bard",
        MONSTER_SOUNDSET_HUMAN_FEMALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_ELVEN_BARD_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_ELVEN_BARD_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_ELVEN_BARD_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_ELVEN_BARD_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Professor of Quantum Mechanics",
        MONSTER_SOUNDSET_HUMAN_MALE,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {GHSOUND_QUANTUM_ITEM_TRADING, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_QUANTUM_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_QUANTUM_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_QUANTUM_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
    {
        "Yeenaghu",
        MONSTER_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SWALLOW_AMBIENT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME */
            {MAX_GHSOUNDS, 1.0f}, /* MONSTER_SOUND_TYPE_ITEM_TRADING */
            {GHSOUND_YEENAGHU_STANDARD_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE */
            {GHSOUND_YEENAGHU_SPECIAL_DIALOGUE, 1.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_STANDARD_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SPECIAL_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CREATION */
            {GHSOUND_YEENAGHU_GET_ANGRY, 1.0f}, /* MONSTER_SOUND_TYPE_GET_ANGRY */
            {GHSOUND_YEENAGHU_MOLLIFIED, 1.0f}, /* MONSTER_SOUND_TYPE_MOLLIFIED */
            {GHSOUND_YEENAGHU_ADVICE, 1.0f}, /* MONSTER_SOUND_TYPE_ADVICE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_COUGH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_PUSH_EFFORT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_UNHAPPY */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_1 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_2 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_3 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_OUCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_FLOUNDER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_4 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_5 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_6 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_BUMP_INTO */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SEARCH */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CAST */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_HAPPY */
            {GHSOUND_YEENAGHU_LAUGHTER, 1.0f}, /* MONSTER_SOUND_TYPE_LAUGHTER */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RANDOM_SOUND */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_RESERVED_7 */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_MUMBLED_CURSE */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_YELL */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_CHAT */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_START_ATTACK */
            {MAX_GHSOUNDS, 0.0f}, /* MONSTER_SOUND_TYPE_SHUDDER */
            {GHSOUND_YEENAGHU_DEATH, 5.0f} /* MONSTER_SOUND_TYPE_DEATH */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OBJECT_SOUNDSET_HUMAN_BAREHANDED,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_GENERIC,
            OBJECT_SOUNDSET_HUMAN_BAREFOOTED
        }
    },
};

NEARDATA struct object_soundset_definition object_soundsets[MAX_OBJECT_SOUNDSETS] =
{
    {
        "",
        OBJECT_SOUNDSET_NONE,
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},

            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},

            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},

            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_NONE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "generic",
        OBJECT_SOUNDSET_GENERIC,
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {GHSOUND_BOW_FIRE, 1.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {GHSOUND_GENERIC_ITEM_WEAR, 1.0f},
            {GHSOUND_GENERIC_ITEM_TAKE_OFF, 1.0f},
            {GHSOUND_GENERIC_ITEM_WIELD, 1.0f},
            {GHSOUND_GENERIC_ITEM_UNWIELD, 1.0f},
            {GHSOUND_GENERIC_ITEM_QUIVER, 1.0f},
            {GHSOUND_GENERIC_ITEM_UNQUIVER, 1.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_SFX_QUAFF, 1.0f},
            {GHSOUND_SFX_READ, 1.0f},
            {GHSOUND_GENERIC_ENGRAVE, 1.0f},
            {GHSOUND_GENERIC_ZAP, 1.0f},

            {GHSOUND_NONE, 0.0f},
            {GHSOUND_BURNT_GENERIC, 1.0f},
            {GHSOUND_FROZEN_GENERIC, 1.0f},
            {GHSOUND_ELECTROCUTED_GENERIC, 1.0f},
            {GHSOUND_GENERIC_ITEM_RUSTS, 1.0f},
            {GHSOUND_GENERIC_ITEM_GETS_BURNT, 1.0f},
            {GHSOUND_GENERIC_ITEM_CORRODES, 1.0f},
            {GHSOUND_GENERIC_ITEM_ROTS, 1.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_GENERIC_BUMP_INTO, 1.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_GENERIC_ITEM_GIVE, 1.0f},

            {GHSOUND_PLAYER_FOOTSTEPS_NORMAL, 1.0f},
            {GHSOUND_GENERIC_FLY, 1.0f},
            {GHSOUND_GENERIC_LEVITATE, 1.0f},
            {GHSOUND_NONE, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_GENERIC
    },
    {
        "candle",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_CANDLE_LIGHT_UP, 1.0f},
            {GHSOUND_CANDLE_SNUFF_OUT, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "quarterstaff",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "bow",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {GHSOUND_BOW_FIRE, 1.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_BOW_WIELD, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "crossbow",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {GHSOUND_CROSSBOW_FIRE, 1.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_CROSSBOW_WIELD, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "arrow",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "sword",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_SWORD_SWING, 1.0f},
            {GHSOUND_SWORD_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_SWORD_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_SWORD_WIELD, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "axe",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_AXE_SWING, 1.0f},
            {GHSOUND_AXE_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_SWORD_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_AXE_WIELD, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "dagger",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DAGGER_SWING, 1.0f},
            {GHSOUND_DAGGER_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_DAGGER_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DAGGER_WIELD, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "bone dagger",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DAGGER_WIELD, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "human barehanded",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_HUMAN_BAREHANDED_SWING, 1.0f},
            {GHSOUND_HUMAN_BAREHANDED_HIT, 1.0f},
            {GHSOUND_NONE, +.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "human barefooted",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_HUMAN_KICK_SWING, 1.0f},
            {GHSOUND_HUMAN_KICK_HIT, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_PLAYER_FOOTSTEPS_NORMAL, 1.0f},
            {GHSOUND_GENERIC_FLY, 1.0f},
            {GHSOUND_GENERIC_LEVITATE, 1.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "horse barefooted",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_HUMAN_KICK_SWING, 1.0f},
            {GHSOUND_HUMAN_KICK_HIT, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_HORSE_FOOTSTEPS, 1.0f},
            {GHSOUND_GENERIC_FLY, 1.0f},
            {GHSOUND_GENERIC_LEVITATE, 1.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "cotton slippers",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_HUMAN_KICK_SWING, 1.0f},
            {GHSOUND_HUMAN_KICK_HIT, 0.5f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_FOOTSTEPS_COTTON_SLIPPERS, 1.0f},
            {GHSOUND_GENERIC_FLY, 1.0f},
            {GHSOUND_GENERIC_LEVITATE, 1.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "chest",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_CHEST
    },
    {
        "coins",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_PICKUP_COINS, 1.0f},
            {GHSOUND_DROP_COINS, 1.0f},
            {GHSOUND_DROP_COINS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "boulder",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_BOULDER_DROP, 1.0f},
            {GHSOUND_BOULDER_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STONE_ENGRAVE, 1.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_STONE_BREAK, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {GHSOUND_PUSH_BOULDER, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_BOULDER_TUMBLE_DOWNWARDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_ROLLING_BOULDER,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "camera",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STAFF_SWING, 1.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_GENERIC_ITEM_THROW, 1.0f},
            {GHSOUND_BOW_FIRE, 1.0f},
            {GHSOUND_ARROW_HIT, 1.0f},

            {GHSOUND_GENERIC_ITEM_PICK_UP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP, 1.0f},
            {GHSOUND_GENERIC_ITEM_DROP_AFTER_THROW, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STONE_ENGRAVE, 1.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DISCARD_GENERIC, 1.0f},
            {GHSOUND_SPARKS_FLY_GENERIC, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_PLAYER_FOOTSTEPS_NORMAL, 1.0f},
            {GHSOUND_GENERIC_FLY, 1.0f},
            {GHSOUND_GENERIC_LEVITATE, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_CAMERA,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "wand",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {GHSOUND_WAND_PICK_UP, 1.0f},
            {GHSOUND_WAND_DROP, 1.0f},
            {GHSOUND_WAND_DROP, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_GENERIC_ZAP, 1.0f},

            {GHSOUND_WAND_BREAK, 1.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {GHSOUND_CRYSTAL_BALL_USE_SUCCESS, 1.0f}, /* General effect */
            {GHSOUND_CRYSTAL_BALL_USE_FAIL, 1.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "stone",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STONE_ENGRAVE, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STONE_ENGRAVE, 1.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_STONE_BREAK, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "potion",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f}, /* Pick up */
            {MAX_GHSOUNDS, 1.0f}, /* Drop */
            {MAX_GHSOUNDS, 1.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {GHSOUND_CANDLE_LIGHT_UP, 1.0f},
            {GHSOUND_CANDLE_SNUFF_OUT, 1.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 1.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {MAX_GHSOUNDS, 1.0f}, /* Zap */

            {GHSOUND_POTION_BREAK, 1.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {GHSOUND_CRYSTAL_BALL_USE_SUCCESS, 1.0f}, /* General effect */
            {GHSOUND_CRYSTAL_BALL_USE_FAIL, 1.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "drum",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_DRUM,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "harp",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_HARP,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "flute",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_FLUTE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "horn",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_HORN,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "gem",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STONE_ENGRAVE, 1.0f},
            {MAX_GHSOUNDS, 0.0f},

            {GHSOUND_GLASS_BREAK, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "lamp",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_LAMP_TURN_ON, 1.0f},
            {GHSOUND_LAMP_TURN_OFF, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "lantern",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_LANTERN_TURN_ON, 1.0f},
            {GHSOUND_LANTERN_TURN_OFF, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "candelabrum",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_CANDLE_LIGHT_UP, 1.0f},
            {GHSOUND_CANDLE_SNUFF_OUT, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_GENERIC,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "coffin",
        OBJECT_SOUNDSET_CHEST,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_COFFIN
    },
    {
        "sarcophagus",
        OBJECT_SOUNDSET_CHEST,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},

            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_SARCOPHAGUS
    },
    {
        "magic marker",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* Ambient */
            {MAX_GHSOUNDS, 1.0f}, /* Melee Swing */
            {MAX_GHSOUNDS, 1.0f}, /* Melee Hit */
            {MAX_GHSOUNDS, 1.0f}, /* Throw */
            {MAX_GHSOUNDS, 0.0f}, /* Fire */
            {MAX_GHSOUNDS, 1.0f}, /* Ranged Hit */

            {GHSOUND_MAGIC_MARKER_PICK_UP, 1.0f}, /* Pick up */
            {GHSOUND_MAGIC_MARKER_DROP, 1.0f}, /* Drop */
            {MAX_GHSOUNDS, 1.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {GHSOUND_MAGIC_MARKER_WRITE, 1.0f}, /* Apply */
            {GHSOUND_MAGIC_MARKER_DRY_OUT, 1.0f}, /* Apply 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {MAX_GHSOUNDS, 0.0f}, /* Zap */

            {MAX_GHSOUNDS, 0.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {MAX_GHSOUNDS, 1.0f}, /* General effect */
            {MAX_GHSOUNDS, 1.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f}, /* Walk */
            {MAX_GHSOUNDS, 0.0f}, /* Fly */
            {MAX_GHSOUNDS, 0.0f}, /* Levitation */
            {MAX_GHSOUNDS, 0.0f}  /* Swim */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "crystal ball",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* Ambient */
            {MAX_GHSOUNDS, 1.0f}, /* Melee Swing */
            {MAX_GHSOUNDS, 1.0f}, /* Melee Hit */
            {MAX_GHSOUNDS, 1.0f}, /* Throw */
            {MAX_GHSOUNDS, 0.0f}, /* Fire */
            {MAX_GHSOUNDS, 1.0f}, /* Ranged Hit */

            {MAX_GHSOUNDS, 1.0f}, /* Pick up */
            {MAX_GHSOUNDS, 1.0f}, /* Drop */
            {MAX_GHSOUNDS, 1.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {GHSOUND_CRYSTAL_BALL_USE_HALLUCINATED, 1.0f}, /* Apply */
            {GHSOUND_CRYSTAL_BALL_USE_FAIL_HALLUCINATED, 1.0f}, /* Apply 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {GHSOUND_CRYSTAL_BALL_SELECT_SYMBOL, 1.0f}, /* Zap */

            {MAX_GHSOUNDS, 0.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {GHSOUND_CRYSTAL_BALL_USE_SUCCESS, 1.0f}, /* General effect */
            {GHSOUND_CRYSTAL_BALL_USE_FAIL, 1.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f}, /* Walk */
            {MAX_GHSOUNDS, 0.0f}, /* Fly */
            {MAX_GHSOUNDS, 0.0f}, /* Levitation */
            {MAX_GHSOUNDS, 0.0f}  /* Swim */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "scroll",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* Ambient */
            {MAX_GHSOUNDS, 1.0f}, /* Melee Swing */
            {MAX_GHSOUNDS, 1.0f}, /* Melee Hit */
            {MAX_GHSOUNDS, 1.0f}, /* Throw */
            {MAX_GHSOUNDS, 0.0f}, /* Fire */
            {MAX_GHSOUNDS, 1.0f}, /* Ranged Hit */

            {MAX_GHSOUNDS, 1.0f}, /* Pick up */
            {MAX_GHSOUNDS, 1.0f}, /* Drop */
            {MAX_GHSOUNDS, 1.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {GHSOUND_CRYSTAL_BALL_USE_SUCCESS, 1.0f}, /* Apply */
            {GHSOUND_CRYSTAL_BALL_USE_FAIL, 1.0f}, /* Apply 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 1.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {MAX_GHSOUNDS, 1.0f}, /* Zap */

            {MAX_GHSOUNDS, 0.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {GHSOUND_CRYSTAL_BALL_USE_SUCCESS, 1.0f}, /* General effect */
            {GHSOUND_CRYSTAL_BALL_USE_FAIL, 1.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f}, /* Walk */
            {MAX_GHSOUNDS, 0.0f}, /* Fly */
            {MAX_GHSOUNDS, 0.0f}, /* Levitation */
            {MAX_GHSOUNDS, 0.0f}  /* Swim */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "spellbook",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* Ambient */
            {MAX_GHSOUNDS, 1.0f}, /* Melee Swing */
            {MAX_GHSOUNDS, 1.0f}, /* Melee Hit */
            {MAX_GHSOUNDS, 1.0f}, /* Throw */
            {MAX_GHSOUNDS, 0.0f}, /* Fire */
            {MAX_GHSOUNDS, 1.0f}, /* Ranged Hit */

            {MAX_GHSOUNDS, 1.0f}, /* Pick up */
            {MAX_GHSOUNDS, 1.0f}, /* Drop */
            {MAX_GHSOUNDS, 1.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {MAX_GHSOUNDS, 1.0f}, /* Apply */
            {MAX_GHSOUNDS, 1.0f}, /* Apply 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {MAX_GHSOUNDS, 0.0f}, /* Zap */

            {MAX_GHSOUNDS, 0.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {GHSOUND_CRYSTAL_BALL_USE_SUCCESS, 1.0f}, /* General effect */
            {GHSOUND_CRYSTAL_BALL_USE_FAIL, 1.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f}, /* Walk */
            {MAX_GHSOUNDS, 0.0f}, /* Fly */
            {MAX_GHSOUNDS, 0.0f}, /* Levitation */
            {MAX_GHSOUNDS, 0.0f}  /* Swim */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_NONE
    },
    {
        "bell",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* Ambient */
            {MAX_GHSOUNDS, 0.0f}, /* Melee Swing */
            {MAX_GHSOUNDS, 0.0f}, /* Melee Hit */
            {MAX_GHSOUNDS, 0.0f}, /* Throw */
            {MAX_GHSOUNDS, 0.0f}, /* Fire */
            {MAX_GHSOUNDS, 0.0f}, /* Ranged Hit */

            {MAX_GHSOUNDS, 0.0f}, /* Pick up */
            {MAX_GHSOUNDS, 0.0f}, /* Drop */
            {MAX_GHSOUNDS, 0.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {GHSOUND_BELL_RING, 1.0f}, /* Apply */
            {MAX_GHSOUNDS, 0.0f}, /* Apply 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {MAX_GHSOUNDS, 0.0f}, /* Zap */

            {MAX_GHSOUNDS, 0.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {MAX_GHSOUNDS, 0.0f}, /* General effect */
            {MAX_GHSOUNDS, 0.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f}, /* Walk */
            {MAX_GHSOUNDS, 0.0f}, /* Fly */
            {MAX_GHSOUNDS, 0.0f}, /* Levitation */
            {MAX_GHSOUNDS, 0.0f}  /* Swim */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_GENERIC
    },
    {
        "saddle",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* Ambient */
            {MAX_GHSOUNDS, 0.0f}, /* Melee Swing */
            {MAX_GHSOUNDS, 0.0f}, /* Melee Hit */
            {MAX_GHSOUNDS, 0.0f}, /* Throw */
            {MAX_GHSOUNDS, 0.0f}, /* Fire */
            {MAX_GHSOUNDS, 0.0f}, /* Ranged Hit */

            {MAX_GHSOUNDS, 0.0f}, /* Pick up */
            {GHSOUND_SADDLE_DROP, 1.0f}, /* Drop */
            {GHSOUND_SADDLE_DROP, 1.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {GHSOUND_SADDLE_APPLY, 1.0f}, /* Apply */
            {MAX_GHSOUNDS, 0.0f}, /* Apply 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {MAX_GHSOUNDS, 0.0f}, /* Zap */

            {MAX_GHSOUNDS, 0.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {MAX_GHSOUNDS, 0.0f}, /* General effect */
            {MAX_GHSOUNDS, 0.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f}, /* Walk */
            {MAX_GHSOUNDS, 0.0f}, /* Fly */
            {MAX_GHSOUNDS, 0.0f}, /* Levitation */
            {MAX_GHSOUNDS, 0.0f}  /* Swim */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_GENERIC
    },
    {
        "can of grease",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* Ambient */
            {MAX_GHSOUNDS, 0.0f}, /* Melee Swing */
            {MAX_GHSOUNDS, 0.0f}, /* Melee Hit */
            {MAX_GHSOUNDS, 0.0f}, /* Throw */
            {MAX_GHSOUNDS, 0.0f}, /* Fire */
            {MAX_GHSOUNDS, 0.0f}, /* Ranged Hit */

            {MAX_GHSOUNDS, 0.0f}, /* Pick up */
            {MAX_GHSOUNDS, 0.0f}, /* Drop */
            {MAX_GHSOUNDS, 0.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {GHSOUND_CAN_OF_GREASE_APPLY, 1.0f}, /* Apply */
            {MAX_GHSOUNDS, 0.0f}, /* Apply 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {MAX_GHSOUNDS, 0.0f}, /* Zap */

            {MAX_GHSOUNDS, 0.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {MAX_GHSOUNDS, 0.0f}, /* General effect */
            {MAX_GHSOUNDS, 0.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f}, /* Walk */
            {MAX_GHSOUNDS, 0.0f}, /* Fly */
            {MAX_GHSOUNDS, 0.0f}, /* Levitation */
            {MAX_GHSOUNDS, 0.0f}  /* Swim */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_GENERIC
    },
    {
        "ring of conflict",
        OBJECT_SOUNDSET_GENERIC,
        {
            {MAX_GHSOUNDS, 0.0f}, /* Ambient */
            {MAX_GHSOUNDS, 0.0f}, /* Melee Swing */
            {MAX_GHSOUNDS, 0.0f}, /* Melee Hit */
            {MAX_GHSOUNDS, 0.0f}, /* Throw */
            {MAX_GHSOUNDS, 0.0f}, /* Fire */
            {MAX_GHSOUNDS, 0.0f}, /* Ranged Hit */

            {MAX_GHSOUNDS, 0.0f}, /* Pick up */
            {MAX_GHSOUNDS, 0.0f}, /* Drop */
            {MAX_GHSOUNDS, 0.0f}, /* Drop after throw */
            {MAX_GHSOUNDS, 0.0f}, /* Wear */
            {MAX_GHSOUNDS, 0.0f}, /* Take off */
            {MAX_GHSOUNDS, 0.0f}, /* Wield */
            {MAX_GHSOUNDS, 0.0f}, /* Unwield */
            {MAX_GHSOUNDS, 0.0f}, /* Quiver */
            {MAX_GHSOUNDS, 0.0f}, /* Unquiver */
            {MAX_GHSOUNDS, 1.0f}, /* Apply */
            {MAX_GHSOUNDS, 0.0f}, /* Apply 2 */
            {GHSOUND_AGGRAVATE_MONSTER, 1.0f}, /* Invoke */
            {MAX_GHSOUNDS, 0.0f}, /* Invoke 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Quaff */
            {MAX_GHSOUNDS, 0.0f}, /* Read */
            {MAX_GHSOUNDS, 0.0f}, /* Engrave */
            {MAX_GHSOUNDS, 0.0f}, /* Zap */

            {MAX_GHSOUNDS, 0.0f}, /* Break */
            {MAX_GHSOUNDS, 0.0f}, /* Burnt */
            {MAX_GHSOUNDS, 0.0f}, /* Frozen */
            {MAX_GHSOUNDS, 0.0f}, /* Electrocuted */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rusts */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Burns */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Corrodes */
            {MAX_GHSOUNDS, 0.0f}, /* Erosion - Rots */
            {MAX_GHSOUNDS, 0.0f}, /* General effect */
            {MAX_GHSOUNDS, 0.0f}, /* General effect 2 */
            {MAX_GHSOUNDS, 0.0f}, /* Push */
            {MAX_GHSOUNDS, 0.0f}, /* Bump into */
            {MAX_GHSOUNDS, 0.0f}, /* Tumble downwards */
            {MAX_GHSOUNDS, 0.0f}, /* Give */

            {MAX_GHSOUNDS, 0.0f}, /* Walk */
            {MAX_GHSOUNDS, 0.0f}, /* Fly */
            {MAX_GHSOUNDS, 0.0f}, /* Levitation */
            {MAX_GHSOUNDS, 0.0f}  /* Swim */
        },
        SOUNDSOURCE_AMBIENT_GENERAL,
        {
            OCCUPATION_SOUNDSET_NONE,
            OCCUPATION_SOUNDSET_GENERIC_EATING,
            OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK,
            OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP,
            OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF,
            OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK,
            OCCUPATION_SOUNDSET_GENERIC_STUDYING,
            OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND,
            OCCUPATION_SOUNDSET_GENERIC_SWINGING,
            OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR,
            OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE
        },
        OBJECT_RAY_SOUNDSET_NONE,
        OBJECT_INSTRUMENT_SOUNDSET_NONE,
        OBJECT_CONTAINER_SOUNDSET_GENERIC
    }, 
};

NEARDATA struct occupation_soundset_definition occupation_soundset_definitions[MAX_OCCUPATION_SOUNDSETS] =
{
    {
        "",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_EATING",
        {
            {GHSOUND_EAT_GENERIC_AMBIENT, 1.0f},
            {GHSOUND_EAT_GENERIC_START, 1.0f},
            {GHSOUND_EAT_GENERIC_FINISH, 1.0f},
            {GHSOUND_EAT_GENERIC_INTERRUPTED, 1.0f},
            {GHSOUND_EAT_GENERIC_RESUME, 1.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_OPENING_TIN",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_DIGGING_ROCK",
        {
            {GHSOUND_DIGGING_GENERIC_AMBIENT, 1.0f},
            {GHSOUND_DIGGING_GENERIC_START, 1.0f},
            {GHSOUND_DIGGING_GENERIC_FINISH, 1.0f},
            {GHSOUND_DIGGING_GENERIC_INTERRUPTED, 1.0f},
            {GHSOUND_DIGGING_GENERIC_RESUME, 1.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_SETTING_TRAP",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_WIPING_OFF",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_TAKING_OFF",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_PICKING_LOCK",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_FORCING_LOCK",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_STUDYING",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_DIGGING_GROUND",
        {
            {GHSOUND_DIGGING_GROUND_GENERIC_AMBIENT, 1.0f},
            {GHSOUND_DIGGING_GROUND_GENERIC_START, 1.0f},
            {GHSOUND_DIGGING_GROUND_GENERIC_FINISH, 1.0f},
            {GHSOUND_DIGGING_GROUND_GENERIC_INTERRUPTED, 1.0f},
            {GHSOUND_DIGGING_GROUND_GENERIC_RESUME, 1.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_SWINGING",
        {
            {GHSOUND_DIGGING_GENERIC_AMBIENT, 1.0f},
            {GHSOUND_DIGGING_GENERIC_START, 1.0f},
            {GHSOUND_DIGGING_GENERIC_FINISH, 1.0f},
            {GHSOUND_DIGGING_GENERIC_INTERRUPTED, 1.0f},
            {GHSOUND_DIGGING_GENERIC_RESUME, 1.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_HITTING_DOOR",
        {
            {GHSOUND_DIGGING_GENERIC_AMBIENT, 1.0f},
            {GHSOUND_DIGGING_GENERIC_START, 1.0f},
            {GHSOUND_DIGGING_GENERIC_FINISH, 1.0f},
            {GHSOUND_DIGGING_GENERIC_INTERRUPTED, 1.0f},
            {GHSOUND_DIGGING_GENERIC_RESUME, 1.0f}
        }
    },
    {
        "OCCUPATION_SOUNDSET_GENERIC_CUTTING_TREE",
        {
            {GHSOUND_DIGGING_GENERIC_AMBIENT, 1.0f},
            {GHSOUND_DIGGING_GENERIC_START, 1.0f},
            {GHSOUND_DIGGING_GENERIC_FINISH, 1.0f},
            {GHSOUND_DIGGING_GENERIC_INTERRUPTED, 1.0f},
            {GHSOUND_DIGGING_GENERIC_RESUME, 1.0f}
        }
    },
};

NEARDATA struct location_soundset_definition location_soundsets[MAX_LOCATION_SOUNDSETS] =
{
    {
        "",
        LOCATION_SOUNDSET_NONE,
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},

            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},

            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},

            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "general",
        LOCATION_SOUNDSET_NONE,
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_STONE_BREAK, 20.0f},
            {GHSOUND_DOOR_CONSUMED_IN_FLAMES, 10.0f},
            {GHSOUND_WATER_IS_FROZEN, 5.0f},

            {GHSOUND_ELECTRICITY_HITS_DOOR, 1.0f},
            {GHSOUND_SOME_WATER_EVAPORATES, 1.0f},
            {GHSOUND_WATER_IS_FROZEN, 1.0f},
            {GHSOUND_WATER_GROUNDS_ELECTRICITY, 1.0f},
            {GHSOUND_DOOR_OPEN, 1.0f},

            {GHSOUND_DOOR_CLOSE, 1.0f},
            {GHSOUND_DOOR_UNLOCK, 1.0f},
            {GHSOUND_DOOR_LOCK, 1.0f},
            {GHSOUND_DOOR_RESISTS, 1.0f},
            {GHSOUND_DOOR_TRY_LOCKED, 1.0f},

            {GHSOUND_GENERIC_BUMP_INTO, 1.0f},
            {GHSOUND_FOUNTAIN_DRIES_UP, 1.0f},
            {GHSOUND_CANDLE_LIGHT_UP, 1.0f},
            {GHSOUND_CANDLE_SNUFF_OUT, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "fountain",
        LOCATION_SOUNDSET_GENERAL,
        {
            {GHSOUND_FOUNTAIN, 0.75f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STONE_BREAK, 10.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_FOUNTAIN_DRIES_UP, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "altar",
        LOCATION_SOUNDSET_GENERAL,
        {
            {GHSOUND_FIRE, 0.5f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STONE_BREAK, 10.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_LIT
    },
    {
        "door",
        LOCATION_SOUNDSET_GENERAL,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_DOOR_WHAM, 1.0f},
            {GHSOUND_DOOR_BREAK, 10.0f},
            {GHSOUND_DOOR_CONSUMED_IN_FLAMES, 1.0f},
            {GHSOUND_DOOR_FREEZES_AND_SHATTERS, 1.0f},

            {GHSOUND_ELECTRICITY_HITS_DOOR, 10.0f},
            {GHSOUND_SOME_WATER_EVAPORATES, 1.0f},
            {GHSOUND_WATER_IS_FROZEN, 1.0f},
            {GHSOUND_WATER_GROUNDS_ELECTRICITY, 1.0f},
            {GHSOUND_DOOR_OPEN, 1.0f},

            {GHSOUND_DOOR_CLOSE, 1.0f},
            {GHSOUND_DOOR_UNLOCK, 1.0f},
            {GHSOUND_DOOR_LOCK, 1.0f},
            {GHSOUND_DOOR_RESISTS, 1.0f},
            {GHSOUND_DOOR_TRY_LOCKED, 1.0f},

            {GHSOUND_DOOR_WOODEN_BUMP_INTO, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "tree",
        LOCATION_SOUNDSET_GENERAL,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_TREE_BREAK, 20.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "stone",
        LOCATION_SOUNDSET_GENERAL,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_STONE_BREAK, 20.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "water",
        LOCATION_SOUNDSET_GENERAL,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_WATER_BREAK, 5.0f},
            {GHSOUND_WATER_BOILS, 5.0f},
            {GHSOUND_WATER_IS_FROZEN, 5.0f},

            {GHSOUND_WATER_GROUNDS_ELECTRICITY, 5.0f},
            {GHSOUND_SOME_WATER_EVAPORATES, 5.0f},
            {GHSOUND_WATER_IS_FROZEN, 5.0f},
            {GHSOUND_WATER_GROUNDS_ELECTRICITY, 5.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_WATER_DRIES_UP, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "ice",
        LOCATION_SOUNDSET_GENERAL,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ICE_MELTS, 5.0f},
            {GHSOUND_ICE_MELTS, 5.0f},
            {GHSOUND_WATER_IS_FROZEN, 5.0f},

            {GHSOUND_WATER_GROUNDS_ELECTRICITY, 5.0f},
            {GHSOUND_ICE_MELTS, 5.0f},
            {GHSOUND_WATER_IS_FROZEN, 5.0f},
            {GHSOUND_WATER_GROUNDS_ELECTRICITY, 5.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_ICE_MELTS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "lava",
        LOCATION_SOUNDSET_GENERAL,
        {
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_LAVA_HARDENS, 5.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_LAVA_HARDENS, 5.0f},

            {GHSOUND_WATER_GROUNDS_ELECTRICITY, 5.0f},
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_LAVA_HARDENS, 5.0f},
            {GHSOUND_WATER_GROUNDS_ELECTRICITY, 5.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_LAVA_HARDENS, 1.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "sink",
        LOCATION_SOUNDSET_GENERAL,
        {
            {MAX_GHSOUNDS, 0.0f},
            {GHSOUND_SINK_KLUNK, 1.0f},
            {GHSOUND_SINK_BREAK, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "brazier",
        LOCATION_SOUNDSET_GENERAL,
        {
            {GHSOUND_FIRE, 1.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 10.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},

            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 0.0f},
            {MAX_GHSOUNDS, 1.0f},
            {MAX_GHSOUNDS, 1.0f}
        },
        SOUNDSOURCE_AMBIENT_LIT
    },
};


NEARDATA struct region_soundset_definition region_soundsets[MAX_REGION_SOUNDSETS] =
{
    {
        "",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_GENERAL
    },
    {
        "poison gas",
        {
            {GHSOUND_POISON_GAS_INSIDE, 1.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        },
        SOUNDSOURCE_AMBIENT_INSIDE_REGION
    }
};


NEARDATA struct effect_sound_definition ui_sounds[MAX_UI_SOUND_TYPES] =
{
    {
        "illegal",
        {GHSOUND_NONE, 0.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "button-down",
        {GHSOUND_UI_BUTTON_DOWN, 1.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "menu-select",
        {GHSOUND_UI_MENU_SELECT, 1.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "",
        {GHSOUND_NONE, 0.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "",
        {GHSOUND_NONE, 0.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "",
        {GHSOUND_NONE, 0.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "knapsack-full",
        {GHSOUND_KNAPSACK_FULL, 1.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "weapon-swapped",
        {GHSOUND_WEAPON_SWAPPED, 1.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "start-two-weapon-combat",
        {GHSOUND_START_TWO_WEAPON_COMBAT, 1.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "stop-two-weapon-combat",
        {GHSOUND_STOP_TWO_WEAPON_COMBAT, 1.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "weapon-swapping-in-right-hand-only",
        {GHSOUND_WEAPON_SWAPPING_IN_RIGHT_HAND_ONLY, 1.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
    {
        "weapon-swapping-in-both-hands",
        {GHSOUND_WEAPON_SWAPPING_IN_BOTH_HANDS, 1.0f},
        FALSE,
        SOUND_PLAY_GROUP_NORMAL
    },
};


NEARDATA struct effect_sound_definition sfx_sounds[MAX_SFX_SOUND_TYPES] =
{
    {
        "",
        {GHSOUND_NONE, 0.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "read",
        {GHSOUND_SFX_READ, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "quaff",
        {GHSOUND_SFX_QUAFF, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "camera click",
        {GHSOUND_CAMERA_FLASH_CREATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "zap",
        {GHSOUND_SFX_ZAP, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "",
        {GHSOUND_NONE, 0.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "",
        {GHSOUND_NONE, 0.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "shield effect",
        {GHSOUND_NONE, 0.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "talk effect",
        {GHSOUND_NONE, 0.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "push boulder",
        {GHSOUND_PUSH_BOULDER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "paralyzed",
        {GHSOUND_CAUSE_PARALYSIS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "sleeping",
        {GHSOUND_CAUSE_SLEEPING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "desecrate altar",
        {GHSOUND_DESECRATE_ALTAR, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "summon demon",
        {GHSOUND_SUMMON_DEMON, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "sacrifice pet",
        {GHSOUND_SACRIFICE_PET, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "sacrifice coaligned unicorn",
        {GHSOUND_SACRIFICE_COALIGNED_UNICORN, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar add alignment",
        {GHSOUND_ALTAR_ADD_ALIGNMENT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar thunderclap",
        {GHSOUND_ALTAR_THUNDERCLAP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar charged air",
        {GHSOUND_ALTAR_CHARGED_AIR, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar accepts sacrifice",
        {GHSOUND_ALTAR_ANGRY_ACCEPTS_SACRIFICE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar rejects sacrifice",
        {GHSOUND_ALTAR_ANGRY_REJECTS_SACRIFICE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar power increase",
        {GHSOUND_ALTAR_POWER_INCREASE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar power decrease",
        {GHSOUND_ALTAR_POWER_DECREASE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar god mollified",
        {GHSOUND_ALTAR_GOD_MOLLIFIED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar inadequacy",
        {GHSOUND_ALTAR_INADEQUACY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar absolved",
        {GHSOUND_ALTAR_ABSOLVED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar hopeful feeling",
        {GHSOUND_ALTAR_HOPEFUL_FEELING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar reconciliation",
        {GHSOUND_ALTAR_RECONCILIATION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar gift",
        {GHSOUND_ALTAR_GIFT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar four-leaf clover",
        {GHSOUND_ALTAR_FOUR_LEAF_CLOVER, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "aura glow",
        {GHSOUND_AURA_GLOW, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar burst of flame",
        {GHSOUND_ALTAR_BURST_OF_FLAME, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "altar offering vanish",
        {GHSOUND_ALTAR_OFFERING_VANISH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray fail",
        {GHSOUND_PRAY_FAIL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray pleased",
        {GHSOUND_PRAY_PLEASED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray fix all troubles",
        {GHSOUND_PRAY_FIX_ALL_TROUBLES, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray fix one trouble",
        {GHSOUND_PRAY_FIX_ONE_TROUBLE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray repair",
        {GHSOUND_PRAY_REPAIR, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray healing",
        {GHSOUND_PRAY_HEALING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray uncurse",
        {GHSOUND_PRAY_UNCURSE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray gift",
        {GHSOUND_PRAY_GIFT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray crowning",
        {GHSOUND_PRAY_CROWNING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray spellbook",
        {GHSOUND_PRAY_SPELLBOOK, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray bless water",
        {GHSOUND_PRAY_BLESS_WATER, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray curse water",
        {GHSOUND_PRAY_CURSE_WATER, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "pray shimmering light",
        {GHSOUND_PRAY_SHIMMERING_LIGHT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_SUCCESS",
        {GHSOUND_ENCHANT_ITEM_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_BLESSED_SUCCESS",
        {GHSOUND_ENCHANT_ITEM_BLESSED_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_UNCURSE_AND_OTHER",
        {GHSOUND_ENCHANT_ITEM_UNCURSE_AND_OTHER, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_NEGATIVE",
        {GHSOUND_ENCHANT_ITEM_NEGATIVE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_VIOLENT_GLOW",
        {GHSOUND_ENCHANT_ITEM_VIOLENT_GLOW, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_VIBRATE_WARNING",
        {GHSOUND_ENCHANT_ITEM_VIBRATE_WARNING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_VIBRATE_AND_DESTROY",
        {GHSOUND_ENCHANT_ITEM_VIBRATE_AND_DESTROY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_SPECIAL_SUCCESS",
        {GHSOUND_ENCHANT_ITEM_SPECIAL_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_SPECIAL_NEGATIVE",
        {GHSOUND_ENCHANT_ITEM_SPECIAL_NEGATIVE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENCHANT_ITEM_GENERAL_FAIL",
        {GHSOUND_ENCHANT_ITEM_GENERAL_FAIL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_HANDS_ITCH",
        {GHSOUND_HANDS_ITCH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PROTECT_ITEM_SUCCESS",
        {GHSOUND_PROTECT_ITEM_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PROTECT_ITEM_FAIL",
        {GHSOUND_PROTECT_ITEM_FAIL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_REPAIR_ITEM_SUCCESS",
        {GHSOUND_REPAIR_ITEM_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_REPAIR_ITEM_FAIL",
        {GHSOUND_REPAIR_ITEM_FAIL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BLESS_ITEM_SUCCESS",
        {GHSOUND_BLESS_ITEM_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_UNCURSE_ITEM_SUCCESS",
        {GHSOUND_UNCURSE_ITEM_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CURSE_ITEM_SUCCESS",
        {GHSOUND_CURSE_ITEM_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_UNBLESS_ITEM_SUCCESS",
        {GHSOUND_UNBLESS_ITEM_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DISINTEGRATE",
        {GHSOUND_DISINTEGRATE, 10.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LIFE_SAVED",
        {GHSOUND_LIFE_SAVED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_HEALING",
        {GHSOUND_HEALING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FULL_HEALING",
        {GHSOUND_FULL_HEALING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CURE_DISEASE",
        {GHSOUND_CURE_DISEASE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CURE_AILMENT",
        {GHSOUND_CURE_AILMENT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GAIN_ENERGY",
        {GHSOUND_GAIN_ENERGY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FULL_ENERGY",
        {GHSOUND_FULL_ENERGY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GAIN_LEVEL",
        {GHSOUND_GAIN_LEVEL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GAIN_SKILL",
        {GHSOUND_GAIN_SKILL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LOSE_LEVEL",
        {GHSOUND_LOSE_LEVEL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LOSE_ENERGY",
        {GHSOUND_LOSE_ENERGY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MUFFLED_SHATTER",
        {GHSOUND_MUFFLED_SHATTER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MUFFLED_CRACKING",
        {GHSOUND_MUFFLED_CRACKING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MUFFLED_CRASH",
        {GHSOUND_MUFFLED_CRASH, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MUFFLED_SPLAT",
        {GHSOUND_MUFFLED_SPLAT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SKILL_ADVANCED",
        {GHSOUND_SKILL_ADVANCED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_EXPLOSION_DARK",
        {GHSOUND_EXPLOSION_DARK, 200.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EXPLOSION_NOXIOUS",
        {GHSOUND_EXPLOSION_NOXIOUS, 200.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EXPLOSION_MUDDY",
        {GHSOUND_EXPLOSION_MUDDY, 200.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EXPLOSION_WET",
        {GHSOUND_EXPLOSION_WET, 200.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EXPLOSION_MAGICAL",
        {GHSOUND_EXPLOSION_MAGICAL, 200.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EXPLOSION_FIERY",
        {GHSOUND_EXPLOSION_FIERY, 200.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EXPLOSION_FROSTY",
        {GHSOUND_EXPLOSION_FROSTY, 200.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_SURPRISE_ATTACK",
        {GHSOUND_SURPRISE_ATTACK, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_STATUE_BECOMES_ALIVE",
        {GHSOUND_STATUE_BECOMES_ALIVE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_WAS_HIDING",
        {GHSOUND_WAS_HIDING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PIERCER_DROPS",
        {GHSOUND_PIERCER_DROPS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TRAP_FOUND",
        {GHSOUND_TRAP_FOUND, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_HIDDEN_DOOR_FOUND",
        {GHSOUND_HIDDEN_DOOR_FOUND, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_UNEARTHED_OBJECT_FOUND",
        {GHSOUND_UNEARTHED_OBJECT_FOUND, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_YOU_SURPRISE_MONSTER",
        {GHSOUND_YOU_SURPRISE_MONSTER, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ARROW_TRAP_FIRE",
        {GHSOUND_ARROW_TRAP_FIRE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DART_TRAP_FIRE",
        {GHSOUND_DART_TRAP_FIRE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_HOLE_OPENS",
        {GHSOUND_HOLE_OPENS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TRAP_DOOR_OPENS",
        {GHSOUND_TRAP_DOOR_OPENS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FALL_INTO_PIT",
        {GHSOUND_FALL_INTO_PIT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LAND_ON_SPIKES",
        {GHSOUND_LAND_ON_SPIKES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SQUEAKY_BOARD",
        {GHSOUND_SQUEAKY_BOARD, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FALLING_ROCK_TRAP_TRIGGER",
        {GHSOUND_FALLING_ROCK_TRAP_TRIGGER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ROLLING_BOOULDER_TRAP_TRIGGER",
        {GHSOUND_ROLLING_BOOULDER_TRAP_TRIGGER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TELEPORT",
        {GHSOUND_TELEPORT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BEAR_TRAP_CLOSES",
        {GHSOUND_BEAR_TRAP_CLOSES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ENVELOPED_IN_CLOUD_OF_GAS",
        {GHSOUND_ENVELOPED_IN_CLOUD_OF_GAS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GUSH_OF_WATER_HITS",
        {GHSOUND_GUSH_OF_WATER_HITS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_STEAMY_BUBBLES",
        {GHSOUND_STEAMY_BUBBLES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TOWER_OF_FLAME_ERUPTS",
        {GHSOUND_TOWER_OF_FLAME_ERUPTS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TELEPORT_TRAP_ACTIVATE",
        {GHSOUND_TELEPORT_TRAP_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEVEL_TELEPORT_TRAP_ACTIVATE",
        {GHSOUND_LEVEL_TELEPORT_TRAP_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEVEL_TELEPORT",
        {GHSOUND_LEVEL_TELEPORT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SHUDDER_FOR_MOMENT",
        {GHSOUND_SHUDDER_FOR_MOMENT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_WRENCHING_SENSATION",
        {GHSOUND_WRENCHING_SENSATION, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BURN_SPIDER_WEB",
        {GHSOUND_BURN_SPIDER_WEB, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CAUGHT_IN_WEB",
        {GHSOUND_CAUGHT_IN_WEB, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TEAR_THROUGH_WEB",
        {GHSOUND_TEAR_THROUGH_WEB, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FLASH_AND_ROAR",
        {GHSOUND_FLASH_AND_ROAR, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SHIVER_RUNS_DOWN_SPINE",
        {GHSOUND_SHIVER_RUNS_DOWN_SPINE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DISTANT_HOWLING",
        {GHSOUND_DISTANT_HOWLING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_MAGIC_TRAP_WEIRD_EFFECT",
        {GHSOUND_MAGIC_TRAP_WEIRD_EFFECT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PACK_SHAKING",
        {GHSOUND_PACK_SHAKING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TAMING",
        {GHSOUND_TAMING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MAGIC_TRAP_ACTIVATE",
        {GHSOUND_MAGIC_TRAP_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ANTI_MAGIC_TRAP_ACTIVATE",
        {GHSOUND_ANTI_MAGIC_TRAP_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LAND_MINE_ACTIVATE",
        {GHSOUND_LAND_MINE_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_POLYMORPH_ACTIVATE",
        {GHSOUND_POLYMORPH_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MAGIC_PORTAL_ACTIVATE",
        {GHSOUND_MAGIC_PORTAL_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_AT_VIBRATING_SQUARE",
        {GHSOUND_AT_VIBRATING_SQUARE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_MODRON_GLIMMER_SURROUNDS",
        {GHSOUND_MODRON_GLIMMER_SURROUNDS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MODRON_TELEPORT_SUCCESS",
        {GHSOUND_MODRON_TELEPORT_SUCCESS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DRAIN_ENERGY",
        {GHSOUND_DRAIN_ENERGY, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FEELING_LETHARGIC",
        {GHSOUND_FEELING_LETHARGIC, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_YOU_RUST",
        {GHSOUND_YOU_RUST, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_YOU_SPLIT",
        {GHSOUND_YOU_SPLIT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ROCK_HITS_HARD_HELMET",
        {GHSOUND_ROCK_HITS_HARD_HELMET, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ROCK_HITS_YOU_ON_HEAD",
        {GHSOUND_ROCK_HITS_YOU_ON_HEAD, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_GAIN_ABILITY_SPELL",
        {GHSOUND_GENERAL_GAIN_ABILITY_SPELL, 0.5f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERIC_PHYSICAL_TRAP_ACTIVATE",
        {GHSOUND_GENERIC_PHYSICAL_TRAP_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERIC_MAGICAL_TRAP_ACTIVATE",
        {GHSOUND_GENERIC_MAGICAL_TRAP_ACTIVATE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FALL_INTO_WATER",
        {GHSOUND_FALL_INTO_WATER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PET_IN_WAY",
        {GHSOUND_PET_IN_WAY, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PET_DOES_NOT_MOVE",
        {GHSOUND_PET_DOES_NOT_MOVE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEVEL_BUBBLING_WATER",
        {GHSOUND_LEVEL_BUBBLING_WATER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_WATER_FALLING_ON_COINS",
        {GHSOUND_LEVEL_WATER_FALLING_ON_COINS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SPLASHING_OF_NAIAD",
        {GHSOUND_LEVEL_SPLASHING_OF_NAIAD, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SODA_FOUNTAIN",
        {GHSOUND_LEVEL_SODA_FOUNTAIN, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SLOW_DRIP",
        {GHSOUND_LEVEL_SLOW_DRIP, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_GURGLING_NOISE",
        {GHSOUND_LEVEL_GURGLING_NOISE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_DISHES_BEING_WASHED",
        {GHSOUND_LEVEL_DISHES_BEING_WASHED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_COURTLY_CONVERSATION",
        {GHSOUND_LEVEL_COURTLY_CONVERSATION, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SCEPTRE_POUNDED",
        {GHSOUND_LEVEL_SCEPTRE_POUNDED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_OFF_WITH_HEAD",
        {GHSOUND_LEVEL_OFF_WITH_HEAD, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_QUEENS_CATS",
        {GHSOUND_LEVEL_QUEENS_CATS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_MOSQUITOES",
        {GHSOUND_LEVEL_MOSQUITOES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_MARSH_GAS",
        {GHSOUND_LEVEL_MARSH_GAS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_DONALD_DUCK",
        {GHSOUND_LEVEL_DONALD_DUCK, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SOMEONE_COUNTING_MONEY",
        {GHSOUND_LEVEL_SOMEONE_COUNTING_MONEY, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_QUARTERBACK_CALLING_PLAY",
        {GHSOUND_LEVEL_QUARTERBACK_CALLING_PLAY, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SOMEONE_SEARCHING",
        {GHSOUND_LEVEL_SOMEONE_SEARCHING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_FOOTSTEPS_OF_GUARD_PATROL",
        {GHSOUND_LEVEL_FOOTSTEPS_OF_GUARD_PATROL, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_EBENEZER_SCROOGE",
        {GHSOUND_LEVEL_EBENEZER_SCROOGE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_LOW_BUZZING",
        {GHSOUND_LEVEL_LOW_BUZZING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_ANGRY_DRONE",
        {GHSOUND_LEVEL_ANGRY_DRONE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_BEES_IN_BONNET",
        {GHSOUND_LEVEL_BEES_IN_BONNET, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SOMEONE_DEMANDING_QUIETNESS",
        {GHSOUND_LEVEL_SOMEONE_DEMANDING_QUIETNESS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_PAGES_TURNING_IN_HEAD",
        {GHSOUND_LEVEL_PAGES_TURNING_IN_HEAD, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_COINS_BEING_ASSEMBLED",
        {GHSOUND_LEVEL_COINS_BEING_ASSEMBLED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_LOUD_DRAGON_SNORING",
        {GHSOUND_LEVEL_LOUD_DRAGON_SNORING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_LOUD_ROAR",
        {GHSOUND_LEVEL_LOUD_ROAR, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SOMEONE_CLAIMING_TO_BE_FIRE_AND_DEATH",
        {GHSOUND_LEVEL_SOMEONE_CLAIMING_TO_BE_FIRE_AND_DEATH, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_BLADES_BEING_HONED",
        {GHSOUND_LEVEL_BLADES_BEING_HONED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_LOUD_HUMAN_SNORING",
        {GHSOUND_LEVEL_LOUD_HUMAN_SNORING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_DICE_BEING_THROWN",
        {GHSOUND_LEVEL_DICE_BEING_THROWN, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_GENERAL_MCARTHUR",
        {GHSOUND_LEVEL_GENERAL_MCARTHUR, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_ELEPHANT_STEPPING_ON_PEANUT",
        {GHSOUND_LEVEL_ELEPHANT_STEPPING_ON_PEANUT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SEAL_BREAKING",
        {GHSOUND_LEVEL_SEAL_BREAKING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_DOCTOR_DOLITTLE",
        {GHSOUND_LEVEL_DOCTOR_DOLITTLE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SOMEONE_CURSING_SHOPLIFTERS",
        {GHSOUND_LEVEL_SOMEONE_CURSING_SHOPLIFTERS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_CHIME_OF_CASH_REGISTER",
        {GHSOUND_LEVEL_CHIME_OF_CASH_REGISTER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_NEIMAN_AND_MARCUS_ARGUING",
        {GHSOUND_LEVEL_NEIMAN_AND_MARCUS_ARGUING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_FAINT_CHIME",
        {GHSOUND_LEVEL_FAINT_CHIME, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SOMEONE_PRAISING_GOD",
        {GHSOUND_LEVEL_SOMEONE_PRAISING_GOD, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SOMEONE_BESEECHING",
        {GHSOUND_LEVEL_SOMEONE_BESEECHING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_ANIMAL_CARCASS_BEING_OFFERED",
        {GHSOUND_LEVEL_ANIMAL_CARCASS_BEING_OFFERED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_PLEA_FOR_DONATIONS",
        {GHSOUND_LEVEL_PLEA_FOR_DONATIONS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_IRON_BEING_FORGED",
        {GHSOUND_LEVEL_IRON_BEING_FORGED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_LOUD_CLANGING",
        {GHSOUND_LEVEL_LOUD_CLANGING, 0.3f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_WATER_BEING_VAPORIZED",
        {GHSOUND_LEVEL_WATER_BEING_VAPORIZED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SOMEONE_MUMBLING",
        {GHSOUND_LEVEL_SOMEONE_MUMBLING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_DISTANT_CHITCHAT",
        {GHSOUND_LEVEL_DISTANT_CHITCHAT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_FOOTSTEPS_AT_DISTANCE",
        {GHSOUND_LEVEL_FOOTSTEPS_AT_DISTANCE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_STRANGE_WIND",
        {GHSOUND_LEVEL_STRANGE_WIND, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_CONVULSIVE_RAVINGS",
        {GHSOUND_LEVEL_CONVULSIVE_RAVINGS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SNORING_SNAKES",
        {GHSOUND_LEVEL_SNORING_SNAKES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_NO_MORE_WOODCHUCKS",
        {GHSOUND_LEVEL_NO_MORE_WOODCHUCKS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_LOUD_ZOT",
        {GHSOUND_LEVEL_LOUD_ZOT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_SWORDS_BEING_UNSHEATED",
        {GHSOUND_LEVEL_SWORDS_BEING_UNSHEATED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_ARMORS_BEING_WORN",
        {GHSOUND_LEVEL_ARMORS_BEING_WORN, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_WEAPONS_BEING_ASSEMBLED",
        {GHSOUND_LEVEL_WEAPONS_BEING_ASSEMBLED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_MASTER_CHIEF",
        {GHSOUND_LEVEL_MASTER_CHIEF, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LIGHTNING_STRIKES",
        {GHSOUND_LIGHTNING_STRIKES, 10.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_START_STONING",
        {GHSOUND_START_STONING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_START_SLIMING",
        {GHSOUND_START_SLIMING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_START_STRANGULATION",
        {GHSOUND_START_STRANGULATION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_START_SUFFOCATION",
        {GHSOUND_START_SUFFOCATION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CATCH_FOOD_POISONING",
        {GHSOUND_CATCH_FOOD_POISONING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CATCH_TERMINAL_ILLNESS",
        {GHSOUND_CATCH_TERMINAL_ILLNESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_BLINDNESS",
        {GHSOUND_ACQUIRE_BLINDNESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_CONFUSION",
        {GHSOUND_ACQUIRE_CONFUSION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_STUN",
        {GHSOUND_ACQUIRE_STUN, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_HALLUCINATION",
        {GHSOUND_ACQUIRE_HALLUCINATION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_SLOW",
        {GHSOUND_ACQUIRE_SLOW, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_PARALYSIS",
        {GHSOUND_ACQUIRE_PARALYSIS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ACQUIRE_FEAR",
        {GHSOUND_ACQUIRE_FEAR, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_SLEEP",
        {GHSOUND_ACQUIRE_SLEEP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ACQUIRE_CANCELLATION",
        {GHSOUND_ACQUIRE_CANCELLATION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_SILENCE",
        {GHSOUND_ACQUIRE_SILENCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ACQUIRE_GRAB",
        {GHSOUND_ACQUIRE_GRAB, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CATCH_MUMMY_ROT",
        {GHSOUND_CATCH_MUMMY_ROT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CATCH_LYCANTHROPY",
        {GHSOUND_CATCH_LYCANTHROPY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GAIN_ABILITY",
        {GHSOUND_GAIN_ABILITY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LOSE_ABILITY",
        {GHSOUND_LOSE_ABILITY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_RESTORE_ABILITY",
        {GHSOUND_RESTORE_ABILITY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BLINDING_FLASH",
        {GHSOUND_BLINDING_FLASH, 10.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_SCALES_COVER_EYES",
        {GHSOUND_SCALES_COVER_EYES, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ELECTRIC_SHOCK",
        {GHSOUND_ELECTRIC_SHOCK, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MALIGNANT_AURA_RESISTED",
        {GHSOUND_MALIGNANT_AURA_RESISTED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MALIGNANT_AURA_SURROUNDS",
        {GHSOUND_MALIGNANT_AURA_SURROUNDS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ITEM_RESISTS_NEGATIVE_EFFECT",
        {GHSOUND_ITEM_RESISTS_NEGATIVE_EFFECT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SUMMON_MONSTER",
        {GHSOUND_SUMMON_MONSTER, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_VANISHES_IN_PUFF_OF_SMOKE",
        {GHSOUND_VANISHES_IN_PUFF_OF_SMOKE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DISARM_TRAP_SUCCESS",
        {GHSOUND_DISARM_TRAP_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DISARM_TRAP_FAIL",
        {GHSOUND_DISARM_TRAP_FAIL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_NO_TRAPS_FOUND",
        {GHSOUND_NO_TRAPS_FOUND, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CANNOT_DISARM_TRAP",
        {GHSOUND_CANNOT_DISARM_TRAP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_STUMBLE_ON_MIMIC",
        {GHSOUND_STUMBLE_ON_MIMIC, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CHEST_TRAP_TRIGGER",
        {GHSOUND_CHEST_TRAP_TRIGGER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TRAP_FLAME_FIZZLES_OUT",
        {GHSOUND_TRAP_FLAME_FIZZLES_OUT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TRAP_NEEDLE_MISSES",
        {GHSOUND_TRAP_NEEDLE_MISSES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TRAP_CLOUD_BLOWS_AWAY",
        {GHSOUND_TRAP_CLOUD_BLOWS_AWAY, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TRAP_NEEDLE_PRICKS",
        {GHSOUND_TRAP_NEEDLE_PRICKS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BLASTED_BY_POWER",
        {GHSOUND_BLASTED_BY_POWER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_IDENTIFY_SUCCESS",
        {GHSOUND_IDENTIFY_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_RECHARGE_SUCCESS",
        {GHSOUND_RECHARGE_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BLESSED_RECHARGE_SUCCESS",
        {GHSOUND_BLESSED_RECHARGE_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_STRIP_CHARGES",
        {GHSOUND_STRIP_CHARGES, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CHARGES_AT_MAXIMUM",
        {GHSOUND_CHARGES_AT_MAXIMUM, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_RECHARGE_FAIL",
        {GHSOUND_RECHARGE_FAIL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_WIZARD_LOCK_KLUNK",
        {GHSOUND_WIZARD_LOCK_KLUNK, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_KNOCK_KLICK",
        {GHSOUND_KNOCK_KLICK, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_JUMP",
        {GHSOUND_JUMP, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEVER_SWITCH_FAIL",
        {GHSOUND_LEVER_SWITCH_FAIL, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEVER_SWITCH_SUCCESS",
        {GHSOUND_LEVER_SWITCH_SUCCESS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEVER_SWITCH_ON_SUCCESS",
        {GHSOUND_LEVER_SWITCH_ON_SUCCESS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEVER_SWITCH_OFF_SUCCESS",
        {GHSOUND_LEVER_SWITCH_OFF_SUCCESS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_WALL_GLOWS_THEN_FADES",
        {GHSOUND_WALL_GLOWS_THEN_FADES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PETRIFY",
        {GHSOUND_PETRIFY, 5.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_KEY_DOES_NOT_FIT",
        {GHSOUND_KEY_DOES_NOT_FIT, 5.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ITEM_VANISHES",
        {GHSOUND_ITEM_VANISHES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENOCIDE",
        {GHSOUND_GENOCIDE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ITEM_CRUMBLES_TO_DUST",
        {GHSOUND_ITEM_CRUMBLES_TO_DUST, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SINISTER_LAUGHTER",
        {GHSOUND_SINISTER_LAUGHTER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_MANIACAL_LAUGHTER",
        {GHSOUND_MANIACAL_LAUGHTER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_SELL_TO_NPC",
        {GHSOUND_SELL_TO_NPC, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BUY_FROM_NPC",
        {GHSOUND_BUY_FROM_NPC, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SELL_TO_SHOPKEEPER",
        {GHSOUND_SELL_TO_SHOPKEEPER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BUY_FROM_SHOPKEEPER",
        {GHSOUND_BUY_FROM_SHOPKEEPER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SEEMS_UNINTERESTED",
        {GHSOUND_SEEMS_UNINTERESTED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CANNOT_PAY",
        {GHSOUND_CANNOT_PAY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_NOT_ENOUGH_MONEY",
        {GHSOUND_NOT_ENOUGH_MONEY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TRANSACT_SINGLE_ITEM",
        {GHSOUND_TRANSACT_SINGLE_ITEM, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_CANNOT",
        {GHSOUND_GENERAL_CANNOT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SINK_CLANKING_FROM_PIPES",
        {GHSOUND_SINK_CLANKING_FROM_PIPES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_SINK_SNATCHES_OF_SONG",
        {GHSOUND_SINK_SNATCHES_OF_SONG, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_SEX_CHANGE",
        {GHSOUND_SEX_CHANGE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_UNKNOWN_ODD_EFFECT",
        {GHSOUND_UNKNOWN_ODD_EFFECT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CELESTIAL_SUMMON_START",
        {GHSOUND_CELESTIAL_SUMMON_START, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_INFERNAL_SUMMON_START",
        {GHSOUND_INFERNAL_SUMMON_START, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_NEUTRAL_SUMMON_START",
        {GHSOUND_NEUTRAL_SUMMON_START, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_SUMMON_START",
        {GHSOUND_GENERAL_SUMMON_START, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SUMMON_NASTY_START",
        {GHSOUND_SUMMON_NASTY_START, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SUMMON_UNDEAD_START",
        {GHSOUND_SUMMON_UNDEAD_START, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ANIMATE_DEAD_START",
        {GHSOUND_ANIMATE_DEAD_START, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SMOKE_SUMMON_START",
        {GHSOUND_SMOKE_SUMMON_START, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_POLYMORPH_FAIL",
        {GHSOUND_POLYMORPH_FAIL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_POLYMORPH_SUCCESS",
        {GHSOUND_POLYMORPH_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_POLYMORPH_SCALES_MERGE",
        {GHSOUND_POLYMORPH_SCALES_MERGE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_POLYMORPH_SCALES_REVERT",
        {GHSOUND_POLYMORPH_SCALES_REVERT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_POLYMORPH_NEW_MAN",
        {GHSOUND_POLYMORPH_NEW_MAN, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_WISH_FULFILLED",
        {GHSOUND_WISH_FULFILLED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_WISH_FOR_OBJECT",
        {GHSOUND_WISH_FOR_OBJECT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_STUCK_IN_TRAP",
        {GHSOUND_STUCK_IN_TRAP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_WRIGGLE_FREE",
        {GHSOUND_WRIGGLE_FREE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_RUMBLING_EARTH",
        {GHSOUND_RUMBLING_EARTH, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_RUMBLE_OF_DISTANT_THUNDER",
        {GHSOUND_RUMBLE_OF_DISTANT_THUNDER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_STUDIO_AUDIENCE_APPLAUDS",
        {GHSOUND_STUDIO_AUDIENCE_APPLAUDS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_WHISTLE",
        {GHSOUND_WHISTLE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CURSED_WHISTLE",
        {GHSOUND_CURSED_WHISTLE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_WHISTLE_UNDERWATER",
        {GHSOUND_WHISTLE_UNDERWATER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_MAGIC_WHISTLE",
        {GHSOUND_MAGIC_WHISTLE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CURSED_MAGIC_WHISTLE",
        {GHSOUND_CURSED_MAGIC_WHISTLE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_MAGIC_WHISTLE_UNDERWATER",
        {GHSOUND_MAGIC_WHISTLE_UNDERWATER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_GEAR_TURN",
        {GHSOUND_GEAR_TURN, 20.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TUMBLER_CLICK",
        {GHSOUND_TUMBLER_CLICK, 20.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_OPEN_DRAWBRIDGE",
        {GHSOUND_OPEN_DRAWBRIDGE, 20.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CLOSE_DRAWBRIDGE",
        {GHSOUND_CLOSE_DRAWBRIDGE, 20.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_DRAWBRIDGE_STARTS_TO_RISE",
        {GHSOUND_DRAWBRIDGE_STARTS_TO_RISE, 10.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_DRAWBRIDGE_GETS_BLOCKED",
        {GHSOUND_DRAWBRIDGE_GETS_BLOCKED, 10.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_GENERAL_UNAFFECTED",
        {GHSOUND_GENERAL_UNAFFECTED, 5.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_RESISTS",
        {GHSOUND_GENERAL_RESISTS, 5.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_STEED_REFUSES",
        {GHSOUND_STEED_REFUSES, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_REFLECTS",
        {GHSOUND_GENERAL_REFLECTS, 5.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DRAWBRIDGE_LOUD_SPLASH",
        {GHSOUND_DRAWBRIDGE_LOUD_SPLASH, 20.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_DRAWBRIDGE_LOUD_CRASH",
        {GHSOUND_DRAWBRIDGE_LOUD_CRASH, 20.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EGG_HATCH",
        {GHSOUND_EGG_HATCH, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SCROLL_FADES",
        {GHSOUND_SCROLL_FADES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_POTION_DILUTES",
        {GHSOUND_POTION_DILUTES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_POISON_DISSOLVES",
        {GHSOUND_POISON_DISSOLVES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FLAMES_EXTINQUISHED",
        {GHSOUND_FLAMES_EXTINQUISHED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ELECTRICITY_CRACKLES",
        {GHSOUND_ELECTRICITY_CRACKLES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ICY_COATING_FORMS",
        {GHSOUND_ICY_COATING_FORMS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MAGIC_ARROW_HIT",
        {GHSOUND_MAGIC_ARROW_HIT, 10.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MIXING_NOT_ENOUGH_COMPONENTS",
        {GHSOUND_MIXING_NOT_ENOUGH_COMPONENTS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MIXING_FLARES_UP",
        {GHSOUND_MIXING_FLARES_UP, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MIXING_SUCCESS",
        {GHSOUND_MIXING_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MIXING_FAILURE",
        {GHSOUND_MIXING_FAILURE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_UNHARMED",
        {GHSOUND_GENERAL_UNHARMED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BURP",
        {GHSOUND_BURP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MONSTER_ON_FIRE",
        {GHSOUND_MONSTER_ON_FIRE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MONSTER_COVERED_IN_FROST",
        {GHSOUND_MONSTER_COVERED_IN_FROST, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MONSTER_GETS_ZAPPED",
        {GHSOUND_MONSTER_GETS_ZAPPED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MONSTER_GETS_SPLASHED_BY_ACID",
        {GHSOUND_MONSTER_GETS_SPLASHED_BY_ACID, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SHARPNESS_SLICE",
        {GHSOUND_SHARPNESS_SLICE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_STEAL_GOLD",
        {GHSOUND_STEAL_GOLD, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DRAIN_LIFE",
        {GHSOUND_DRAIN_LIFE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_STEAL_ITEM",
        {GHSOUND_STEAL_ITEM, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MONSTER_IS_POISONED",
        {GHSOUND_MONSTER_IS_POISONED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_HELMET_BLOCKS_ATTACK",
        {GHSOUND_HELMET_BLOCKS_ATTACK, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LIFE_LEECH",
        {GHSOUND_LIFE_LEECH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_HAIL_OF_MAGIC_MISSILES_MISSES",
        {GHSOUND_HAIL_OF_MAGIC_MISSILES_MISSES, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_HAIL_OF_MAGIC_MISSILES_HITS",
        {GHSOUND_HAIL_OF_MAGIC_MISSILES_HITS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_NOT_ENOUGH_MANA",
        {GHSOUND_NOT_ENOUGH_MANA, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FAIL_TO_CAST_CORRECTLY",
        {GHSOUND_FAIL_TO_CAST_CORRECTLY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MAGICAL_ENERGY_RELEASED",
        {GHSOUND_MAGICAL_ENERGY_RELEASED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PUT_ON_LEASH",
        {GHSOUND_PUT_ON_LEASH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_REMOVE_LEASH",
        {GHSOUND_REMOVE_LEASH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PULLS_FREE_OF_LEASH",
        {GHSOUND_PULLS_FREE_OF_LEASH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEASH_SNAPS_LOOSE",
        {GHSOUND_LEASH_SNAPS_LOOSE, 10.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LEASH_GOES_SLACK",
        {GHSOUND_LEASH_GOES_SLACK, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_FAILS_TO_LEASH",
        {GHSOUND_FAILS_TO_LEASH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SPELL_LEARN_FAIL",
        {GHSOUND_SPELL_LEARN_FAIL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SPELL_LEARN_SUCCESS",
        {GHSOUND_SPELL_LEARN_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SPELL_TOO_FAINT",
        {GHSOUND_SPELL_TOO_FAINT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SPELL_KNOWN_ALREADY",
        {GHSOUND_SPELL_KNOWN_ALREADY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SPELL_KEENER",
        {GHSOUND_SPELL_KEENER, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CANCELLATION_IN_FORCE",
        {GHSOUND_CANCELLATION_IN_FORCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SINK_OPEN_TAP",
        {GHSOUND_SINK_OPEN_TAP, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_SINK_KLUNK",
        {GHSOUND_SINK_KLUNK, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SCOLDED",
        {GHSOUND_SCOLDED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LAVA_BURNT",
        {GHSOUND_LAVA_BURNT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_HIDDEN_TREASURE_FOUND",
        {GHSOUND_HIDDEN_TREASURE_FOUND, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_DO_NOT_KNOW_HOW",
        {GHSOUND_GENERAL_DO_NOT_KNOW_HOW, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_ALREADY_DONE",
        {GHSOUND_GENERAL_ALREADY_DONE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_SOMETHING_INTERESTING_THERE",
        {GHSOUND_GENERAL_SOMETHING_INTERESTING_THERE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_NOTHING_THERE",
        {GHSOUND_GENERAL_NOTHING_THERE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_DOES_NOT_FIT",
        {GHSOUND_GENERAL_DOES_NOT_FIT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_WELDED",
        {GHSOUND_GENERAL_WELDED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MUFFLED_BELL_RING",
        {GHSOUND_MUFFLED_BELL_RING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_OUT_OF_CHARGES",
        {GHSOUND_GENERAL_OUT_OF_CHARGES, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_CURSED_BELL_OF_OPENING_EFFECT",
        {GHSOUND_CURSED_BELL_OF_OPENING_EFFECT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_UNCURSED_BELL_OF_OPENING_EFFECT",
        {GHSOUND_UNCURSED_BELL_OF_OPENING_EFFECT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_BLESSED_BELL_OF_OPENING_EFFECT",
        {GHSOUND_BLESSED_BELL_OF_OPENING_EFFECT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_BELL_OF_OPENING_UNSETTLING_SHRILL_SOUND",
        {GHSOUND_BELL_OF_OPENING_UNSETTLING_SHRILL_SOUND, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ACQUIRE_HASTE",
        {GHSOUND_ACQUIRE_HASTE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_EXPLOSION_FLAMING_SPHERE",
        {GHSOUND_EXPLOSION_FLAMING_SPHERE, 20.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EXPLOSION_FREEZING_SPHERE",
        {GHSOUND_EXPLOSION_FREEZING_SPHERE, 20.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_EXPLOSION_SHOCKING_SPHERE",
        {GHSOUND_EXPLOSION_SHOCKING_SPHERE, 20.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_HALLUCINATING_FLASH",
        {GHSOUND_HALLUCINATING_FLASH, 10.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_GENERAL_CURRENTLY_UNABLE_TO_DO",
        {GHSOUND_GENERAL_CURRENTLY_UNABLE_TO_DO, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MONSTER_DOES_NOT_NOTICE",
        {GHSOUND_MONSTER_DOES_NOT_NOTICE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_THAT_DID_NOTHING",
        {GHSOUND_GENERAL_THAT_DID_NOTHING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_THATS_SILLY",
        {GHSOUND_GENERAL_THATS_SILLY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MONSTER_IS_BUSY",
        {GHSOUND_MONSTER_IS_BUSY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_NOT_AT_RIGHT_LOCATION",
        {GHSOUND_GENERAL_NOT_AT_RIGHT_LOCATION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_ANOTHER_ACTION_NEEDED",
        {GHSOUND_GENERAL_ANOTHER_ACTION_NEEDED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_NOT_A_GOOD_IDEA",
        {GHSOUND_GENERAL_NOT_A_GOOD_IDEA, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_CANNOT_REACH",
        {GHSOUND_GENERAL_CANNOT_REACH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_TOO_CLOSE",
        {GHSOUND_GENERAL_TOO_CLOSE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_TOO_FAR",
        {GHSOUND_GENERAL_TOO_FAR, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_CANNOT_SEE_SPOT",
        {GHSOUND_GENERAL_CANNOT_SEE_SPOT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_NOT_ENOUGH_LEVERAGE",
        {GHSOUND_GENERAL_NOT_ENOUGH_LEVERAGE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW",
        {GHSOUND_GENERAL_CURRENT_FORM_DOES_NOT_ALLOW, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_NOT_SKILLED_ENOUGH",
        {GHSOUND_GENERAL_NOT_SKILLED_ENOUGH, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_TOO_MUCH_ENCUMBRANCE",
        {GHSOUND_GENERAL_TOO_MUCH_ENCUMBRANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MONSTER_DOES_NOT_ALLOW",
        {GHSOUND_MONSTER_DOES_NOT_ALLOW, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_NOT_ENOUGH_STAMINA",
        {GHSOUND_GENERAL_NOT_ENOUGH_STAMINA, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ITEM_WELDS",
        {GHSOUND_ITEM_WELDS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CRUNCHING_SOUND",
        {GHSOUND_CRUNCHING_SOUND, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_MONSTER_IS_HIT_WITH_DEATH_MAGIC",
        {GHSOUND_MONSTER_IS_HIT_WITH_DEATH_MAGIC, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_WEAPON_NO_LONGER_POISONED",
        {GHSOUND_WEAPON_NO_LONGER_POISONED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_WEAPON_NO_LONGER_ENCHANTED",
        {GHSOUND_WEAPON_NO_LONGER_ENCHANTED, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_UNLUCKILY_YOUR_TOUCH_DID_NOT_WORK",
        {GHSOUND_UNLUCKILY_YOUR_TOUCH_DID_NOT_WORK, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LUCKILY_ITS_TOUCH_DID_NOT_WORK",
        {GHSOUND_LUCKILY_ITS_TOUCH_DID_NOT_WORK, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_YOU_RESISTANCE_SUCCESS_STR",
        {GHSOUND_YOU_RESISTANCE_SUCCESS_STR, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_YOU_RESISTANCE_SUCCESS_DEX",
        {GHSOUND_YOU_RESISTANCE_SUCCESS_DEX, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_YOU_RESISTANCE_SUCCESS_CON",
        {GHSOUND_YOU_RESISTANCE_SUCCESS_CON, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_YOU_RESISTANCE_SUCCESS_INT",
        {GHSOUND_YOU_RESISTANCE_SUCCESS_INT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_YOU_RESISTANCE_SUCCESS_WIS",
        {GHSOUND_YOU_RESISTANCE_SUCCESS_WIS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_YOU_RESISTANCE_SUCCESS_CHA",
        {GHSOUND_YOU_RESISTANCE_SUCCESS_CHA, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENEMY_RESISTANCE_SUCCESS_STR",
        {GHSOUND_ENEMY_RESISTANCE_SUCCESS_STR, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENEMY_RESISTANCE_SUCCESS_DEX",
        {GHSOUND_ENEMY_RESISTANCE_SUCCESS_DEX, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENEMY_RESISTANCE_SUCCESS_CON",
        {GHSOUND_ENEMY_RESISTANCE_SUCCESS_CON, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENEMY_RESISTANCE_SUCCESS_INT",
        {GHSOUND_ENEMY_RESISTANCE_SUCCESS_INT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENEMY_RESISTANCE_SUCCESS_WIS",
        {GHSOUND_ENEMY_RESISTANCE_SUCCESS_WIS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENEMY_RESISTANCE_SUCCESS_CHA",
        {GHSOUND_ENEMY_RESISTANCE_SUCCESS_CHA, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_VAMPIRE_TRANSFORMS",
        {GHSOUND_VAMPIRE_TRANSFORMS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_MONSTER_IS_HIT_WITH_CELESTIAL_MAGIC",
        {GHSOUND_MONSTER_IS_HIT_WITH_CELESTIAL_MAGIC, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_BODY_CRUMBLES_TO_DUST",
        {GHSOUND_BODY_CRUMBLES_TO_DUST, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_TRIED_ACTION_BUT_IT_FAILED",
        {GHSOUND_GENERAL_TRIED_ACTION_BUT_IT_FAILED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ATTACH_CANDLE",
        {GHSOUND_ATTACH_CANDLE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GENERAL_NOT_IN_THE_RIGHT_CONDITION",
        {GHSOUND_GENERAL_NOT_IN_THE_RIGHT_CONDITION, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_LAND_MINE_SET_UP",
        {GHSOUND_LAND_MINE_SET_UP, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BEAR_TRAP_SET_UP",
        {GHSOUND_BEAR_TRAP_SET_UP, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ALARM_SOUNDS",
        {GHSOUND_ALARM_SOUNDS, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ESCAPE_TRAP",
        {GHSOUND_ESCAPE_TRAP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_NOTHING_FOUND",
        {GHSOUND_NOTHING_FOUND, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_DISORIENTED_FOR_MOMENT",
        {GHSOUND_DISORIENTED_FOR_MOMENT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MYSTERIOUS_FORCE_PREVENTS",
        {GHSOUND_MYSTERIOUS_FORCE_PREVENTS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_BLOCKED_ALIGNMENT_CHANGE",
        {GHSOUND_BLOCKED_ALIGNMENT_CHANGE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ALIGNMENT_CHANGE_CONVERSION",
        {GHSOUND_ALIGNMENT_CHANGE_CONVERSION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ALIGNMENT_CHANGE_HELM_ON",
        {GHSOUND_ALIGNMENT_CHANGE_HELM_ON, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ALIGNMENT_CHANGE_HELM_OFF",
        {GHSOUND_ALIGNMENT_CHANGE_HELM_OFF, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_TOO_MUCH_TO_COMPREHEND",
        {GHSOUND_TOO_MUCH_TO_COMPREHEND, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_AIR_CRACKLES",
        {GHSOUND_AIR_CRACKLES, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_AGGRAVATE_MONSTER",
        {GHSOUND_AGGRAVATE_MONSTER, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CIRCLE_OF_FIRE",
        {GHSOUND_CIRCLE_OF_FIRE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CIRCLE_OF_FROST",
        {GHSOUND_CIRCLE_OF_FROST, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CIRCLE_OF_LIGHTNING",
        {GHSOUND_CIRCLE_OF_LIGHTNING, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CIRCLE_OF_MAGIC",
        {GHSOUND_CIRCLE_OF_MAGIC, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CIRCLE_OF_RADIANCE",
        {GHSOUND_CIRCLE_OF_RADIANCE, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_LEVEL_LOUD_CLANGING",
        {GHSOUND_LEVEL_LOUD_CLANGING, 2.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_BOULDER_FILLS_PIT",
        {GHSOUND_BOULDER_FILLS_PIT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_BOULDER_PLUGS_HOLE_OR_TRAPDOOR",
        {GHSOUND_BOULDER_PLUGS_HOLE_OR_TRAPDOOR, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_INVISIBLE_CHOIR_SINGS",
        {GHSOUND_INVISIBLE_CHOIR_SINGS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_WAVE_OF_PSYCHIC_ENERGY",
        {GHSOUND_WAVE_OF_PSYCHIC_ENERGY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_WAVE_OF_PSYCHIC_ENERGY",
        {GHSOUND_WAVE_OF_PSYCHIC_ENERGY, 0.25f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_BOSS_FIGHT",
        {GHSOUND_BOSS_FIGHT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_VOICE_THRONE_ROOM_AUDIENCE_SUMMONED_DAME",
        {GHSOUND_VOICE_THRONE_ROOM_AUDIENCE_SUMMONED_DAME, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_VOICE_THRONE_ROOM_AUDIENCE_SUMMONED_SIRE",
        {GHSOUND_VOICE_THRONE_ROOM_AUDIENCE_SUMMONED_SIRE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_VOICE_THRONE_ROOM_BY_THINE_IMPERIOUS_ORDER_DAME",
        {GHSOUND_VOICE_THRONE_ROOM_BY_THINE_IMPERIOUS_ORDER_DAME, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_VOICE_THRONE_ROOM_BY_THINE_IMPERIOUS_ORDER_SIRE",
        {GHSOUND_VOICE_THRONE_ROOM_BY_THINE_IMPERIOUS_ORDER_SIRE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_VOICE_THRONE_ROOM_CURSE_UPON_THEE",
        {GHSOUND_VOICE_THRONE_ROOM_CURSE_UPON_THEE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_VOICE_THRONE_ROOM_THANK_YOU_FOR_YOUR_CONTRIBUTION",
        {GHSOUND_VOICE_THRONE_ROOM_THANK_YOU_FOR_YOUR_CONTRIBUTION, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ACHIEVEMENT",
        {GHSOUND_ACHIEVEMENT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MOUNT_FAIL_SLIP_AWAY",
        {GHSOUND_MOUNT_FAIL_SLIP_AWAY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MOUNT_FAIL_AND_SLIP",
        {GHSOUND_MOUNT_FAIL_AND_SLIP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_MOUNT_SUCCESS",
        {GHSOUND_MOUNT_SUCCESS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_BLIND_TELEPATHY",
        {GHSOUND_INTRINSIC_ACQUIRED_BLIND_TELEPATHY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_FIRE_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_FIRE_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_COLD_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_COLD_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_SHOCK_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_SHOCK_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_POISON_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_POISON_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_STONE_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_STONE_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_SLEEP_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_SLEEP_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_ACID_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_ACID_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_DISINTEGRATION_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_DISINTEGRATION_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_DEATH_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_DEATH_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_CHARM_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_CHARM_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_FEAR_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_FEAR_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_MIND_SHIELDING",
        {GHSOUND_INTRINSIC_ACQUIRED_MIND_SHIELDING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_CURSE_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_CURSE_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_TELEPORT",
        {GHSOUND_INTRINSIC_ACQUIRED_TELEPORT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_TELEPORT_CONTROL",
        {GHSOUND_INTRINSIC_ACQUIRED_TELEPORT_CONTROL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_TELEPATHY",
        {GHSOUND_INTRINSIC_ACQUIRED_TELEPATHY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_LYCANTHROPY_RESISTANCE",
        {GHSOUND_INTRINSIC_ACQUIRED_LYCANTHROPY_RESISTANCE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_INTRINSIC_ACQUIRED_GENERAL",
        {GHSOUND_INTRINSIC_ACQUIRED_GENERAL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CURSED_GAIN_LEVEL",
        {GHSOUND_CURSED_GAIN_LEVEL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_RUB",
        {GHSOUND_RUB, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_APPLY_TINNING_KIT",
        {GHSOUND_APPLY_TINNING_KIT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_FILL_OIL_UP",
        {GHSOUND_FILL_OIL_UP, 1.0f},
        TRUE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_THUMP_HIT",
        {GHSOUND_THUMP_HIT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SPLASH_HIT",
        {GHSOUND_SPLASH_HIT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_GUILTY",
        {GHSOUND_GUILTY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_CAITIFF",
        {GHSOUND_CAITIFF, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_WARNING",
        {GHSOUND_WARNING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_HINT",
        {GHSOUND_HINT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_REVIVAL",
        {GHSOUND_REVIVAL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_NOT_READY_YET",
        {GHSOUND_NOT_READY_YET, 1.0f},
        FALSE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_ARTIFACT_NAME_KNOWN",
        {GHSOUND_ARTIFACT_NAME_KNOWN, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_ZOO",
        {GHSOUND_ENTER_ZOO, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_SWAMP",
        {GHSOUND_ENTER_SWAMP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_COURT",
        {GHSOUND_ENTER_COURT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_LEPRECHAUN_HALL",
        {GHSOUND_ENTER_LEPRECHAUN_HALL, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_MORGUE",
        {GHSOUND_ENTER_MORGUE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_MORGUE_MIDNIGHT",
        {GHSOUND_ENTER_MORGUE_MIDNIGHT, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_BEEHIVE",
        {GHSOUND_ENTER_BEEHIVE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_GARDEN",
        {GHSOUND_ENTER_GARDEN, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_DESERTED_SHOP",
        {GHSOUND_ENTER_DESERTED_SHOP, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_LIBRARY",
        {GHSOUND_ENTER_LIBRARY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_DRAGON_LAIR",
        {GHSOUND_ENTER_DRAGON_LAIR, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_COCKATRICE_NEST",
        {GHSOUND_ENTER_COCKATRICE_NEST, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_ANTHOLE",
        {GHSOUND_ENTER_ANTHOLE, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_BARRACKS",
        {GHSOUND_ENTER_BARRACKS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_ABANDONED_BARRACKS",
        {GHSOUND_ENTER_ABANDONED_BARRACKS, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_ENTER_ARMORY",
        {GHSOUND_ENTER_ARMORY, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_SIT",
        {GHSOUND_SIT, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_SIT_IN_AIR",
        {GHSOUND_SIT_IN_AIR, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_TUMBLE_IN_AIR",
        {GHSOUND_TUMBLE_IN_AIR, 1.0f},
        TRUE, SOUND_PLAY_GROUP_NORMAL
    },
    {
        "GHSOUND_PROTECTION_END_WARNING",
        {GHSOUND_PROTECTION_END_WARNING, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
    {
        "GHSOUND_NEGATIVE_EFFECT_ENDED",
        {GHSOUND_NEGATIVE_EFFECT_ENDED, 1.0f},
        FALSE, SOUND_PLAY_GROUP_LONG
    },
};

struct ray_soundset_definition ray_soundsets[MAX_RAY_SOUNDSETS] =
{
    {
        "ray of magic missiles",
        {
            {GHSOUND_RAY_MAGIC_MISSILE_AMBIENT, 1.0f},
            {GHSOUND_RAY_MAGIC_MISSILE_CREATE, 100.0f},
            {GHSOUND_RAY_MAGIC_MISSILE_DESTROY, 10.0f},
            {GHSOUND_RAY_MAGIC_MISSILE_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of fire",
        {
            {GHSOUND_RAY_FIRE_AMBIENT, 1.0f},
            {GHSOUND_RAY_FIRE_CREATE, 100.0f},
            {GHSOUND_RAY_FIRE_DESTROY, 10.0f},
            {GHSOUND_RAY_FIRE_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of frost",
        {
            {GHSOUND_RAY_COLD_AMBIENT, 1.0f},
            {GHSOUND_RAY_COLD_CREATE, 100.0f},
            {GHSOUND_RAY_COLD_DESTROY, 10.0f},
            {GHSOUND_RAY_COLD_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of sleep",
        {
            {GHSOUND_RAY_SLEEP_AMBIENT, 1.0f},
            {GHSOUND_RAY_SLEEP_CREATE, 100.0f},
            {GHSOUND_RAY_SLEEP_DESTROY, 10.0f},
            {GHSOUND_RAY_SLEEP_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of disintegration",
        {
            {GHSOUND_RAY_DISINTEGRATION_AMBIENT, 1.0f},
            {GHSOUND_RAY_DISINTEGRATION_CREATE, 100.0f},
            {GHSOUND_RAY_DISINTEGRATION_DESTROY, 10.0f},
            {GHSOUND_RAY_DISINTEGRATION_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of electricity",
        {
            {GHSOUND_RAY_ELEC_AMBIENT, 1.0f},
            {GHSOUND_RAY_ELEC_CREATE, 100.0f},
            {GHSOUND_RAY_ELEC_DESTROY, 10.0f},
            {GHSOUND_RAY_ELEC_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of poison gas",
        {
            {GHSOUND_RAY_POISON_GAS_AMBIENT, 1.0f},
            {GHSOUND_RAY_POISON_GAS_CREATE, 100.0f},
            {GHSOUND_RAY_POISON_GAS_DESTROY, 10.0f},
            {GHSOUND_RAY_POISON_GAS_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of acid",
        {
            {GHSOUND_RAY_ACID_AMBIENT, 1.0f},
            {GHSOUND_RAY_ACID_CREATE, 100.0f},
            {GHSOUND_RAY_ACID_DESTROY, 10.0f},
            {GHSOUND_RAY_ACID_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of death",
        {
            {GHSOUND_RAY_DEATH_AMBIENT, 1.0f},
            {GHSOUND_RAY_DEATH_CREATE, 100.0f},
            {GHSOUND_RAY_DEATH_DESTROY, 10.0f},
            {GHSOUND_RAY_DEATH_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "ray of petrification",
        {
            {GHSOUND_RAY_PETRIFICATION_AMBIENT, 1.0f},
            {GHSOUND_RAY_PETRIFICATION_CREATE, 100.0f},
            {GHSOUND_RAY_PETRIFICATION_DESTROY, 10.0f},
            {GHSOUND_RAY_PETRIFICATION_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of magic missiles",
        {
            {GHSOUND_RAY_MAGIC_MISSILE_AMBIENT, 1.0f},
            {GHSOUND_RAY_MAGIC_MISSILE_CREATE, 100.0f},
            {GHSOUND_RAY_MAGIC_MISSILE_DESTROY, 10.0f},
            {GHSOUND_RAY_MAGIC_MISSILE_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of fire",
        {
            {GHSOUND_RAY_FIRE_AMBIENT, 1.0f},
            {GHSOUND_RAY_FIRE_CREATE, 100.0f},
            {GHSOUND_RAY_FIRE_DESTROY, 10.0f},
            {GHSOUND_RAY_FIRE_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of frost",
        {
            {GHSOUND_RAY_COLD_AMBIENT, 1.0f},
            {GHSOUND_RAY_COLD_CREATE, 100.0f},
            {GHSOUND_RAY_COLD_DESTROY, 10.0f},
            {GHSOUND_RAY_COLD_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of sleep",
        {
            {GHSOUND_RAY_SLEEP_AMBIENT, 1.0f},
            {GHSOUND_RAY_SLEEP_CREATE, 100.0f},
            {GHSOUND_RAY_SLEEP_DESTROY, 10.0f},
            {GHSOUND_RAY_SLEEP_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of disintegration",
        {
            {GHSOUND_RAY_DISINTEGRATION_AMBIENT, 1.0f},
            {GHSOUND_RAY_DISINTEGRATION_CREATE, 100.0f},
            {GHSOUND_RAY_DISINTEGRATION_DESTROY, 10.0f},
            {GHSOUND_RAY_DISINTEGRATION_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of electricity",
        {
            {GHSOUND_RAY_ELEC_AMBIENT, 1.0f},
            {GHSOUND_RAY_ELEC_CREATE, 100.0f},
            {GHSOUND_RAY_ELEC_DESTROY, 10.0f},
            {GHSOUND_RAY_ELEC_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of poison gas",
        {
            {GHSOUND_RAY_POISON_GAS_AMBIENT, 1.0f},
            {GHSOUND_RAY_POISON_GAS_CREATE, 100.0f},
            {GHSOUND_RAY_POISON_GAS_DESTROY, 10.0f},
            {GHSOUND_RAY_POISON_GAS_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of acid",
        {
            {GHSOUND_RAY_ACID_AMBIENT, 1.0f},
            {GHSOUND_RAY_ACID_CREATE, 100.0f},
            {GHSOUND_RAY_ACID_DESTROY, 10.0f},
            {GHSOUND_RAY_ACID_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of death",
        {
            {GHSOUND_RAY_DEATH_AMBIENT, 1.0f},
            {GHSOUND_RAY_DEATH_CREATE, 100.0f},
            {GHSOUND_RAY_DEATH_DESTROY, 10.0f},
            {GHSOUND_RAY_DEATH_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "blast of petrification",
        {
            {GHSOUND_RAY_PETRIFICATION_AMBIENT, 1.0f},
            {GHSOUND_RAY_PETRIFICATION_CREATE, 100.0f},
            {GHSOUND_RAY_PETRIFICATION_DESTROY, 10.0f},
            {GHSOUND_RAY_PETRIFICATION_BOUNCE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "digging beam",
        {
            {GHSOUND_BEAM_DIGGING_AMBIENT, 5.0f},
            {GHSOUND_BEAM_DIGGING_CREATE, 100.0f},
            {GHSOUND_BEAM_DIGGING_DESTROY, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_BEAM_DIGGING_HIT_LOCATION, 100.0f}
        }
    },
    {
        "evaportation beam",
        {
            {GHSOUND_BEAM_EVAPORATION_AMBIENT, 1.0f},
            {GHSOUND_BEAM_EVAPORATION_CREATE, 100.0f},
            {GHSOUND_BEAM_EVAPORATION_DESTROY, 10.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_BEAM_EVAPORATION_HIT_LOCATION, 10.0f}
        }
    },
    {
        "camera",
        {
            {GHSOUND_CAMERA_FLASH_AMBIENT, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },

    {
        "no object ray soundset",
        {
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "generic object ray soundset",
        {
            {GHSOUND_GENERAL_SPELL_AMBIENT, 1.0f},
            {GHSOUND_GENERAL_SPELL_CREATE, 100.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },
    {
        "rolling boulder",
        {
            {GHSOUND_ROLLING_BOULDER_AMBIENT, 1.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_STAFF_HIT, 1.0f},
            {GHSOUND_NONE, 0.0f},
            {GHSOUND_NONE, 0.0f}
        }
    },

};


struct instrument_soundset_definition instrument_soundsets[MAX_INSTRUMENT_SOUNDSETS] =
{
    {
        "",
        {
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Stunned */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Confused */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Hallucinated */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Far from music */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note A */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note B */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note C */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note D */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note E */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note F */
            {GHSOUND_NONE, 0.0f, SOUND_PLAY_GROUP_NORMAL}  /* Note G */
        },
        0
    },
    {
        "drum",
        {
            {GHSOUND_DRUM_IMPROVISE_NORMAL, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise */
            {GHSOUND_DRUM_IMPROVISE_STUNNED, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Stunned */
            {GHSOUND_DRUM_IMPROVISE_CONFUSED, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Confused */
            {GHSOUND_DRUM_IMPROVISE_HALLUCINATION, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Hallucinated */
            {GHSOUND_DRUM_IMPROVISE_FAR_FROM_MUSIC, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Far from music */
            {GHSOUND_DRUM_NOTE_A, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note A */
            {GHSOUND_DRUM_NOTE_B, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note B */
            {GHSOUND_DRUM_NOTE_C, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note C */
            {GHSOUND_DRUM_NOTE_D, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note D */
            {GHSOUND_DRUM_NOTE_E, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note E */
            {GHSOUND_DRUM_NOTE_F, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note F */
            {GHSOUND_DRUM_NOTE_G, 1.0f, SOUND_PLAY_GROUP_NORMAL}  /* Note G */
        },
        200
    },
    {
        "harp",
        {
            {GHSOUND_HARP_IMPROVISE_NORMAL, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise */
            {GHSOUND_HARP_IMPROVISE_STUNNED, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Stunned */
            {GHSOUND_HARP_IMPROVISE_CONFUSED, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Confused */
            {GHSOUND_HARP_IMPROVISE_HALLUCINATION, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Hallucinated */
            {GHSOUND_HARP_IMPROVISE_FAR_FROM_MUSIC, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Far from music */
            {GHSOUND_HARP_NOTE_A, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note A */
            {GHSOUND_HARP_NOTE_B, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note B */
            {GHSOUND_HARP_NOTE_C, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note C */
            {GHSOUND_HARP_NOTE_D, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note D */
            {GHSOUND_HARP_NOTE_E, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note E */
            {GHSOUND_HARP_NOTE_F, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note F */
            {GHSOUND_HARP_NOTE_G, 1.0f, SOUND_PLAY_GROUP_NORMAL}  /* Note G */
        },
        1000
    },
    {
        "flute",
        {
            {GHSOUND_FLUTE_IMPROVISE_NORMAL, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise */
            {GHSOUND_FLUTE_IMPROVISE_STUNNED, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Stunned */
            {GHSOUND_FLUTE_IMPROVISE_CONFUSED, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Confused */
            {GHSOUND_FLUTE_IMPROVISE_HALLUCINATION, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Hallucinated */
            {GHSOUND_FLUTE_IMPROVISE_FAR_FROM_MUSIC, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Far from music */
            {GHSOUND_FLUTE_NOTE_A, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note A */
            {GHSOUND_FLUTE_NOTE_B, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note B */
            {GHSOUND_FLUTE_NOTE_C, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note C */
            {GHSOUND_FLUTE_NOTE_D, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note D */
            {GHSOUND_FLUTE_NOTE_E, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note E */
            {GHSOUND_FLUTE_NOTE_F, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note F */
            {GHSOUND_FLUTE_NOTE_G, 1.0f, SOUND_PLAY_GROUP_NORMAL}  /* Note G */
        },
        2000
    },
    {
        "horn",
        {
            {GHSOUND_HORN_IMPROVISE_NORMAL, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise */
            {GHSOUND_HORN_IMPROVISE_STUNNED, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Stunned */
            {GHSOUND_HORN_IMPROVISE_CONFUSED, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Confused */
            {GHSOUND_HORN_IMPROVISE_HALLUCINATION, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Hallucinated */
            {GHSOUND_HORN_IMPROVISE_FAR_FROM_MUSIC, 1.0f, SOUND_PLAY_GROUP_LONG}, /* Improvise Far from music */
            {GHSOUND_HORN_NOTE_A, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note A */
            {GHSOUND_HORN_NOTE_B, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note B */
            {GHSOUND_HORN_NOTE_C, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note C */
            {GHSOUND_HORN_NOTE_D, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note D */
            {GHSOUND_HORN_NOTE_E, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note E */
            {GHSOUND_HORN_NOTE_F, 1.0f, SOUND_PLAY_GROUP_NORMAL}, /* Note F */
            {GHSOUND_HORN_NOTE_G, 1.0f, SOUND_PLAY_GROUP_NORMAL}  /* Note G */
        },
        650
    },
};


struct container_soundset_definition container_soundsets[MAX_CONTAINER_SOUNDSETS] =
{
    {
        "",
        {
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_OPEN */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_CLOSE */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_UNLOCK */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_LOCK */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_BREAK_LOCK */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_LID_SLAM */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_RESIST_OPEN */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_TRY_LOCKED */
        }
    },
    {
        "generic",
        {
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_OPEN */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_CLOSE */
            {GHSOUND_CHEST_UNLOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_UNLOCK */
            {GHSOUND_CHEST_LOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_LOCK */
            {GHSOUND_CHEST_BREAK_LOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_BREAK_LOCK */
            {GHSOUND_CHEST_LID_SLAM, 1.0f}, /* CONTAINER_SOUND_TYPE_LID_SLAM */
            {GHSOUND_CHEST_RESISTS_OPEN, 1.0f}, /* CONTAINER_SOUND_TYPE_RESIST_OPEN */
            {GHSOUND_CHEST_TRY_LOCKED, 1.0f}, /* CONTAINER_SOUND_TYPE_TRY_LOCKED */
        }
    },
    {
        "chest",
        {
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_OPEN */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_CLOSE */
            {GHSOUND_CHEST_UNLOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_UNLOCK */
            {GHSOUND_CHEST_LOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_LOCK */
            {GHSOUND_CHEST_BREAK_LOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_BREAK_LOCK */
            {GHSOUND_CHEST_LID_SLAM, 1.0f}, /* CONTAINER_SOUND_TYPE_LID_SLAM */
            {GHSOUND_CHEST_RESISTS_OPEN, 1.0f}, /* CONTAINER_SOUND_TYPE_RESIST_OPEN */
            {GHSOUND_CHEST_TRY_LOCKED, 1.0f}, /* CONTAINER_SOUND_TYPE_TRY_LOCKED */
        }
    },
    {
        "coffin",
        {
            {GHSOUND_COFFIN_OPEN, 1.0f}, /* CONTAINER_SOUND_TYPE_OPEN */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_CLOSE */
            {GHSOUND_CHEST_UNLOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_UNLOCK */
            {GHSOUND_CHEST_LOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_LOCK */
            {GHSOUND_CHEST_BREAK_LOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_BREAK_LOCK */
            {GHSOUND_CHEST_LID_SLAM, 1.0f}, /* CONTAINER_SOUND_TYPE_LID_SLAM */
            {GHSOUND_CHEST_RESISTS_OPEN, 1.0f}, /* CONTAINER_SOUND_TYPE_RESIST_OPEN */
            {GHSOUND_CHEST_TRY_LOCKED, 1.0f}, /* CONTAINER_SOUND_TYPE_TRY_LOCKED */
        }
    },
    {
        "sarcophagus",
        {
            {GHSOUND_SARCOPHAGUS_OPEN, 1.0f}, /* CONTAINER_SOUND_TYPE_OPEN */
            {GHSOUND_NONE, 0.0f}, /* CONTAINER_SOUND_TYPE_CLOSE */
            {GHSOUND_CHEST_UNLOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_UNLOCK */
            {GHSOUND_CHEST_LOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_LOCK */
            {GHSOUND_CHEST_BREAK_LOCK, 1.0f}, /* CONTAINER_SOUND_TYPE_BREAK_LOCK */
            {GHSOUND_CHEST_LID_SLAM, 1.0f}, /* CONTAINER_SOUND_TYPE_LID_SLAM */
            {GHSOUND_CHEST_RESISTS_OPEN, 1.0f}, /* CONTAINER_SOUND_TYPE_RESIST_OPEN */
            {GHSOUND_CHEST_TRY_LOCKED, 1.0f}, /* CONTAINER_SOUND_TYPE_TRY_LOCKED */
        }
    },
};


void
update_game_music()
{
    struct ghsound_music_info musicinfo = { 0 };
    musicinfo.volume = 1.0f;
    musicinfo.ghsound = GHSOUND_NONE;

    if (context.game_started == FALSE)
    {
        musicinfo.ghsound = GHSOUND_MUSIC_INTRO;
    }
    else if (program_state.gameover == 1)
    {
        musicinfo.ghsound = GHSOUND_MUSIC_GAMEOVER;
    }
    else if (program_state.gameover == 2)
    {
        musicinfo.ghsound = GHSOUND_MUSIC_VICTORY;
    }
    else
    {
        struct mkroom* room_ptr = which_room(u.ux, u.uy);
        if (!room_ptr)
            musicinfo.ghsound = get_level_music(&u.uz);
        else
            musicinfo.ghsound = get_room_music(room_ptr);
    }

    /* play_ghsound_music will check if the music is currently playing and then do nothing if this is the case */
    /* GHSOUND_NONE will stop music */
    play_ghsound_music(musicinfo);
}

void
play_level_ambient_sounds()
{
    struct ghsound_level_ambient_info lainfo = { 0 };
    lainfo.volume = 1.0f;
    lainfo.ghsound = GHSOUND_NONE;

    if (context.game_started == FALSE || program_state.gameover > 0)
    {
        lainfo.ghsound = GHSOUND_NONE;
    }
    else
    {
        /* First special square ambients, if any, otherwise a room or level ambient */
        if (In_modron_level(&u.uz) && (levl[u.ux][u.uy].typ == AIR || levl[u.ux][u.uy].typ == CLOUD))
        {
            lainfo.ghsound = GHSOUND_AIR_LEVEL_AMBIENT;
        }
        else
        {
            struct mkroom* room_ptr = which_room(u.ux, u.uy);
            if (!room_ptr)
                lainfo.ghsound = get_level_ambient_sounds(&u.uz);
            else
                lainfo.ghsound = get_room_ambient_sounds(room_ptr);
        }
    }

    /* play_ghsound_level_ambient will check if the ambient sound is currently playing and then do nothing if this is the case */
    /* GHSOUND_NONE will stop ambient sounds */
    play_ghsound_level_ambient(lainfo);
}

void
play_environment_ambient_sounds()
{
    struct ghsound_environment_ambient_info eainfo = { 0 };
    eainfo.volume = 1.0f;
    eainfo.ghsound = GHSOUND_NONE;

    if (context.game_started == FALSE || program_state.gameover > 0)
    {
        eainfo.ghsound = GHSOUND_NONE;
    }
    else
    {
        eainfo.ghsound = get_environment_ambient_sounds();
    }

    /* play_ghsound_level_ambient will check if the ambient sound is currently playing and then do nothing if this is the case */
    /* GHSOUND_NONE will stop ambient sounds */
    play_ghsound_environment_ambient(eainfo);
}

enum floor_surface_types
    get_floor_surface_type(x, y, is_underwater)
    int x, y;
boolean is_underwater;
{
    if (!isok(x, y))
        return FLOOR_SURFACE_NONE;

    struct rm* lev = &levl[x][y];

    enum floor_surface_types floorid = FLOOR_SURFACE_NONE;
    int ftyp = IS_FLOOR(lev->typ) || (IS_POOL(lev->typ) && !is_underwater) ? lev->typ : lev->floortyp;
    int fsubtyp = IS_FLOOR(lev->typ) || (IS_POOL(lev->typ) && !is_underwater) ? lev->subtyp : lev->floorsubtyp;

    enum obj_material_types floor_material = get_location_type_material(ftyp, fsubtyp);
    floorid = material_definitions[floor_material].floor_surface_mapping;

    return floorid;
}

void
play_movement_sound(mtmp, climbingid)
struct monst* mtmp;
enum climbing_types climbingid;
{
    if (!mtmp)
        return;

    if (Deaf)
        return;

    boolean isyou = (mtmp == &youmonst);
    xchar ox = isyou ? u.ux : mtmp->mx, oy = isyou ? u.uy : mtmp->my;
    if (!isok(ox, oy))
        return;

    if (!isyou && distu(ox, oy) > MINIMUM_PLAYABLE_MOVEMENT_SOUND_DISTANCE * MINIMUM_PLAYABLE_MOVEMENT_SOUND_DISTANCE)
        return;

    boolean is_underwater = isyou ? Underwater : is_swimmer(mtmp->data) && is_pool(ox, oy);
    struct ghsound_immediate_info immediateinfo = { 0 };

    /* Parameters */
    enum floor_surface_types floorid = get_floor_surface_type(ox, oy, is_underwater); /* Set the appropriate floor here */
    enum floor_treading_types treadingid = FLOOR_TREADING_TYPE_NORMAL;
    enum location_passing_types passingid = LOCATION_PASSING_TYPE_NORMAL;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f, base_volume = isyou || mtmp == u.usteed ? 1.0f : 0.25f;
    float weight = max(0.0f, min(10000.0f, (float)mtmp->data->cwt));

    enum monster_soundset_types mss = get_monster_soundset(mtmp);
    enum object_soundset_types oss = monster_soundsets[mss].attack_soundsets[BAREFOOTED_ATTACK_NUMBER];
    enum object_sound_types sound_type = OBJECT_SOUND_TYPE_WALK;

    if (isyou)
    {
        if (!isok(u.ux, u.uy))
            return;

        if (Wwalking && floorid == FLOOR_SURFACE_LIQUID)
            passingid = LOCATION_PASSING_TYPE_WATERWALKING;
        else if (!Swimming && floorid == FLOOR_SURFACE_LIQUID)
            passingid = LOCATION_PASSING_TYPE_SUBMERGED;
        else if (Passes_walls && IS_ROCK(levl[u.ux][u.uy].typ))
            passingid = LOCATION_PASSING_TYPE_THROUGH_SOLID_WALLS;

        treadingid = Stealth ? FLOOR_TREADING_TYPE_STEALTH : FLOOR_TREADING_TYPE_NORMAL;

        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);

        //        soundid = object_soundsets[oss].sounds[sound_type].ghsound;
        //        volume = object_soundsets[oss].sounds[sound_type].volume;

        if (is_flyer(mtmp->data) || Flying)
        {
            sound_type = OBJECT_SOUND_TYPE_FLY;
            struct obj* otmp = what_gives(FLYING);
            if (otmp)
            {
                enum object_soundset_types oss2 = objects[otmp->otyp].oc_soundset;
                set_simple_object_sound_id_and_volume(oss2, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss2].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss2].sounds[sound_type].volume;
            }
            else
            {
                set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss].sounds[sound_type].volume;
            }
        }
        else if (is_floater(mtmp->data) || Levitation)
        {
            sound_type = OBJECT_SOUND_TYPE_LEVITATION;
            struct obj* otmp = what_gives(LEVITATION);
            if (otmp)
            {
                enum object_soundset_types oss2 = objects[otmp->otyp].oc_soundset;
                set_simple_object_sound_id_and_volume(oss2, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss2].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss2].sounds[sound_type].volume;
            }
            else
            {
                set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss].sounds[sound_type].volume;
            }
        }
        else if ((is_swimmer(mtmp->data) || amphibious(mtmp->data) || Swimming) && floorid == FLOOR_SURFACE_LIQUID)
        {
            sound_type = OBJECT_SOUND_TYPE_SWIM;
            struct obj* otmp = what_gives(SWIMMING);
            if (otmp)
            {
                enum object_soundset_types oss2 = objects[otmp->otyp].oc_soundset;
                set_simple_object_sound_id_and_volume(oss2, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss2].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss2].sounds[sound_type].volume;
            }
            else
            {
                set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss].sounds[sound_type].volume;
            }
        }
        else if (uarmf)
        {
            enum object_soundset_types oss_boots = objects[uarmf->otyp].oc_soundset;
            set_simple_object_sound_id_and_volume(oss_boots, sound_type, &soundid, &volume);
            //            soundid = object_soundsets[oss_boots].sounds[sound_type].ghsound;
            //            volume = object_soundsets[oss_boots].sounds[sound_type].volume;
        }
    }
    else
    {
        if (!isok(mtmp->mx, mtmp->my))
            return;

        if (mtmp->mprops[WATER_WALKING] != 0 && floorid == FLOOR_SURFACE_LIQUID)
            passingid = LOCATION_PASSING_TYPE_WATERWALKING;
        else if (mtmp->mprops[SWIMMING] == 0 && floorid == FLOOR_SURFACE_LIQUID)
            passingid = LOCATION_PASSING_TYPE_SUBMERGED;
        else if (mtmp->mprops[PASSES_WALLS] != 0 && IS_ROCK(levl[mtmp->mx][mtmp->my].typ))
            passingid = LOCATION_PASSING_TYPE_THROUGH_SOLID_WALLS;

        treadingid = mtmp->mprops[STEALTH] != 0 ? FLOOR_TREADING_TYPE_STEALTH : FLOOR_TREADING_TYPE_NORMAL;

        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
        //        soundid = object_soundsets[oss].sounds[sound_type].ghsound;
        //        volume = object_soundsets[oss].sounds[sound_type].volume;

        if (is_flying(mtmp))
        {
            sound_type = OBJECT_SOUND_TYPE_FLY;
            struct obj* otmp = what_gives_monster(mtmp, FLYING);
            if (otmp)
            {
                enum object_soundset_types oss2 = objects[otmp->otyp].oc_soundset;
                set_simple_object_sound_id_and_volume(oss2, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss2].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss2].sounds[sound_type].volume;
            }
            else
            {
                set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss].sounds[sound_type].volume;
            }
        }
        else if (is_levitating(mtmp))
        {
            sound_type = OBJECT_SOUND_TYPE_LEVITATION;
            struct obj* otmp = what_gives_monster(mtmp, LEVITATION);
            if (otmp)
            {
                enum object_soundset_types oss2 = objects[otmp->otyp].oc_soundset;
                set_simple_object_sound_id_and_volume(oss2, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss2].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss2].sounds[sound_type].volume;
            }
            else
            {
                set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss].sounds[sound_type].volume;
            }
        }
        else if ((has_swimming(mtmp) || amphibious(mtmp->data)) && floorid == FLOOR_SURFACE_LIQUID)
        {
            sound_type = OBJECT_SOUND_TYPE_SWIM;
            struct obj* otmp = what_gives_monster(mtmp, SWIMMING);
            if (otmp)
            {
                enum object_soundset_types oss2 = objects[otmp->otyp].oc_soundset;
                set_simple_object_sound_id_and_volume(oss2, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss2].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss2].sounds[sound_type].volume;
            }
            else
            {
                set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss].sounds[sound_type].volume;
            }
        }
        else if (mtmp->worn_item_flags & W_ARMF)
        {
            struct obj* omtp_boots = which_armor(mtmp, W_ARMF);
            if (omtp_boots)
            {
                enum object_soundset_types oss_boots = objects[omtp_boots->otyp].oc_soundset;
                set_simple_object_sound_id_and_volume(oss_boots, sound_type, &soundid, &volume);
                //                soundid = object_soundsets[oss_boots].sounds[sound_type].ghsound;
                //                volume = object_soundsets[oss_boots].sounds[sound_type].volume;
            }
        }

        float hearing = hearing_array[mtmp->mx][mtmp->my];
        if (hearing == 0.0f && context.global_minimum_volume <= 0.0f)
            return;
        else
            volume *= hearing_array[mtmp->mx][mtmp->my];
    }

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, base_volume * volume));
    immediateinfo.parameter_names[0] = "FloorSurface";
    immediateinfo.parameter_values[0] = (float)floorid;
    immediateinfo.parameter_names[1] = "Weight";
    immediateinfo.parameter_values[1] = weight;
    immediateinfo.parameter_names[2] = "TreadingType";
    immediateinfo.parameter_values[2] = (float)treadingid;
    immediateinfo.parameter_names[3] = "ClimbingType";
    immediateinfo.parameter_values[3] = (float)climbingid;
    immediateinfo.parameter_names[4] = "PassingType";
    immediateinfo.parameter_values[4] = (float)passingid;
    immediateinfo.parameter_names[5] = (char*)0;
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > MINIMUM_PLAYABLE_MOVEMENT_SOUND_VOLUME)
        play_immediate_ghsound(immediateinfo);
}

void
play_simple_object_sound(obj, sound_type)
struct obj* obj;
enum object_sound_types sound_type;
{
    /* Do not use for hit sounds */

    if (!obj)
        return;

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    struct ghsound_immediate_info immediateinfo = { 0 };

    if (obj->oartifact && artilist[obj->oartifact].soundset > OBJECT_SOUNDSET_NONE)
    {
        enum object_soundset_types oss = artilist[obj->oartifact].soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }
    else
    {
        enum object_soundset_types oss = objects[obj->otyp].oc_soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }

    xchar x = 0, y = 0;
    get_obj_location(obj, &x, &y, 0);

    if (isok(x, y))
    {
        float hearing = hearing_array[x][y];
        if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
            return;
        else
            volume *= hearing_array[x][y];
    }
    else
        return;

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
play_simple_object_sound_at_location(obj, x, y, sound_type)
struct obj* obj;
int x, y;
enum object_sound_types sound_type;
{
    /* Do not use for hit sounds */

    if (!obj || !isok(x, y))
        return;

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    struct ghsound_immediate_info immediateinfo = { 0 };

    if (obj->oartifact && artilist[obj->oartifact].soundset > OBJECT_SOUNDSET_NONE)
    {
        enum object_soundset_types oss = artilist[obj->oartifact].soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }
    else
    {
        enum object_soundset_types oss = objects[obj->otyp].oc_soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }

    float hearing = hearing_array[x][y];
    if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
        return;
    else
        volume *= hearing_array[x][y];

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
set_simple_object_sound_id_and_volume(oss, sound_type, sound_id_ptr, volume_ptr)
enum object_soundset_types oss;
enum object_sound_types sound_type;
enum ghsound_types* sound_id_ptr;
float* volume_ptr;
{
    if (!sound_id_ptr || !volume_ptr)
        return;

    enum ghsound_types soundid = object_soundsets[oss].sounds[sound_type].ghsound;
    float volume = object_soundsets[oss].sounds[sound_type].volume;

    if (soundid < MAX_GHSOUNDS || object_soundsets[oss].base_soundset == OBJECT_SOUNDSET_NONE || object_soundsets[oss].base_soundset == oss)
    {
        if (soundid >= MAX_GHSOUNDS)
        {
            *sound_id_ptr = GHSOUND_NONE;
            *volume_ptr = 0.0f;
        }
        else
        {
            *sound_id_ptr = soundid;
            *volume_ptr = volume;
        }
    }
    else
    {
        enum object_soundset_types base_oss = object_soundsets[oss].base_soundset;
        set_simple_object_sound_id_and_volume(base_oss, sound_type, sound_id_ptr, volume_ptr);
    }
}


void
play_simple_container_sound(obj, sound_type)
struct obj* obj;
enum container_sound_types sound_type;
{
    /* Do not use for hit sounds */

    if (!obj)
        return;

    if (Deaf)
        return;

    xchar x = 0, y = 0;
    get_obj_location(obj, &x, &y, 0);

    play_simple_container_sound_at_location(obj, x, y, sound_type);
}

void
play_simple_container_sound_at_location(obj, x, y, sound_type)
struct obj* obj;
int x, y;
enum container_sound_types sound_type;
{
    /* Do not use for hit sounds */

    if (!obj || !isok(x, y))
        return;

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    struct ghsound_immediate_info immediateinfo = { 0 };

    if (obj->oartifact && artilist[obj->oartifact].soundset > OBJECT_SOUNDSET_NONE)
    {
        enum object_soundset_types oss = artilist[obj->oartifact].soundset;
        set_simple_container_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }
    else
    {
        enum object_soundset_types oss = objects[obj->otyp].oc_soundset;
        set_simple_container_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }

    float hearing = hearing_array[x][y];
    if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
        return;
    else
        volume *= hearing_array[x][y];

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
set_simple_container_sound_id_and_volume(oss, sound_type, sound_id_ptr, volume_ptr)
enum object_soundset_types oss;
enum container_sound_types sound_type;
enum ghsound_types* sound_id_ptr;
float* volume_ptr;
{
    if (!sound_id_ptr || !volume_ptr)
        return;

    enum ghsound_types soundid = container_soundsets[object_soundsets[oss].container_soundset].sounds[sound_type].ghsound;
    float volume = container_soundsets[object_soundsets[oss].container_soundset].sounds[sound_type].volume;

    if (soundid < MAX_GHSOUNDS || object_soundsets[oss].base_soundset == OBJECT_SOUNDSET_NONE || object_soundsets[oss].base_soundset == oss)
    {
        if (soundid >= MAX_GHSOUNDS)
        {
            *sound_id_ptr = GHSOUND_NONE;
            *volume_ptr = 0.0f;
        }
        else
        {
            *sound_id_ptr = soundid;
            *volume_ptr = volume;
        }
    }
    else
    {
        enum object_soundset_types base_oss = object_soundsets[oss].base_soundset;
        set_simple_container_sound_id_and_volume(base_oss, sound_type, sound_id_ptr, volume_ptr);
    }
}


void
set_simple_location_sound_id_and_volume(lss, sound_type, sound_id_ptr, volume_ptr)
enum location_soundset_types lss;
enum location_sound_types sound_type;
enum ghsound_types* sound_id_ptr;
float* volume_ptr;
{
    if (!sound_id_ptr || !volume_ptr)
        return;

    enum ghsound_types soundid = location_soundsets[lss].sounds[sound_type].ghsound;
    float volume = location_soundsets[lss].sounds[sound_type].volume;

    if (soundid < MAX_GHSOUNDS || location_soundsets[lss].base_soundset == LOCATION_SOUNDSET_NONE || location_soundsets[lss].base_soundset == lss)
    {
        if (soundid >= MAX_GHSOUNDS)
        {
            *sound_id_ptr = GHSOUND_NONE;
            *volume_ptr = 0.0f;
        }
        else
        {
            *sound_id_ptr = soundid;
            *volume_ptr = volume;
        }
    }
    else
    {
        enum location_soundset_types base_lss = location_soundsets[lss].base_soundset;
        set_simple_location_sound_id_and_volume(base_lss, sound_type, sound_id_ptr, volume_ptr);
    }
}



void
set_simple_player_sound_id_and_volume(pss, sound_type, sound_id_ptr, volume_ptr)
enum player_soundset_types pss;
enum monster_sound_types sound_type;
enum ghsound_types* sound_id_ptr;
float* volume_ptr;
{
    if (!sound_id_ptr || !volume_ptr)
        return;

    enum ghsound_types soundid = player_soundsets[pss].sounds[sound_type].ghsound;
    float volume = player_soundsets[pss].sounds[sound_type].volume;

    if (soundid < MAX_GHSOUNDS || player_soundsets[pss].base_soundset == PLAYER_SOUNDSET_NONE || player_soundsets[pss].base_soundset == pss)
    {
        if (soundid >= MAX_GHSOUNDS)
        {
            *sound_id_ptr = GHSOUND_NONE;
            *volume_ptr = 0.0f;
        }
        else
        {
            *sound_id_ptr = soundid;
            *volume_ptr = volume;
        }
    }
    else
    {
        enum player_soundset_types base_pss = player_soundsets[pss].base_soundset;
        set_simple_player_sound_id_and_volume(base_pss, sound_type, sound_id_ptr, volume_ptr);
    }
}

void
set_simple_player_voice_id_and_volume(pss, sound_type, sound_id_ptr, volume_ptr)
enum player_soundset_types pss;
enum player_voice_types sound_type;
enum ghsound_types* sound_id_ptr;
float* volume_ptr;
{
    if (!sound_id_ptr || !volume_ptr)
        return;

    enum ghsound_types soundid = player_soundsets[pss].voices[sound_type].ghsound;
    float volume = player_soundsets[pss].voices[sound_type].volume;

    if (soundid < MAX_GHSOUNDS || player_soundsets[pss].base_soundset == PLAYER_SOUNDSET_NONE || player_soundsets[pss].base_soundset == pss)
    {
        if (soundid >= MAX_GHSOUNDS)
        {
            *sound_id_ptr = GHSOUND_NONE;
            *volume_ptr = 0.0f;
        }
        else
        {
            *sound_id_ptr = soundid;
            *volume_ptr = volume;
        }
    }
    else
    {
        enum player_soundset_types base_pss = player_soundsets[pss].base_soundset;
        set_simple_player_voice_id_and_volume(base_pss, sound_type, sound_id_ptr, volume_ptr);
    }
}

void
set_simple_monster_sound_id_and_volume(mss, sound_type, sound_id_ptr, volume_ptr)
enum monster_soundset_types mss;
enum monster_sound_types sound_type;
enum ghsound_types* sound_id_ptr;
float* volume_ptr;
{
    if (!sound_id_ptr || !volume_ptr)
        return;

    enum ghsound_types soundid = monster_soundsets[mss].sounds[sound_type].ghsound;
    float volume = monster_soundsets[mss].sounds[sound_type].volume;

    if (soundid < MAX_GHSOUNDS || monster_soundsets[mss].base_soundset == MONSTER_SOUNDSET_NONE || monster_soundsets[mss].base_soundset == mss)
    {
        if (soundid >= MAX_GHSOUNDS)
        {
            *sound_id_ptr = GHSOUND_NONE;
            *volume_ptr = 0.0f;
        }
        else
        {
            *sound_id_ptr = soundid;
            *volume_ptr = volume;
        }
    }
    else
    {
        enum monster_soundset_types base_mss = monster_soundsets[mss].base_soundset;
        set_simple_monster_sound_id_and_volume(base_mss, sound_type, sound_id_ptr, volume_ptr);
    }
}

enum ghsound_types
    get_monster_ambient_sound_id(mss)
    enum monster_soundset_types mss;
{
    enum ghsound_types sound_id = GHSOUND_NONE;
    float volume = 0.0f;
    set_simple_monster_sound_id_and_volume(mss, MONSTER_SOUND_TYPE_AMBIENT, &sound_id, &volume);

    return sound_id;
}

float
get_monster_ambient_sound_volume(mss)
enum monster_soundset_types mss;
{
    enum ghsound_types sound_id = GHSOUND_NONE;
    float volume = 0.0f;
    set_simple_monster_sound_id_and_volume(mss, MONSTER_SOUND_TYPE_AMBIENT, &sound_id, &volume);

    return volume;
}


void
play_object_floor_sound_at_location(obj, sound_type, x, y, is_underwater)
struct obj* obj;
enum object_sound_types sound_type;
int x, y;
boolean is_underwater;
{
    if (!obj)
        return;

    if (Deaf)
        return;

    xchar ox = x, oy = y;

    if (!isok(ox, oy))
        return;

    struct ghsound_immediate_info immediateinfo = { 0 };
    enum floor_surface_types floorid = get_floor_surface_type(ox, oy, is_underwater);
    float weight = (float)obj->owt;
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;

    if (obj->oartifact && artilist[obj->oartifact].soundset > OBJECT_SOUNDSET_NONE)
    {
        enum object_soundset_types oss = artilist[obj->oartifact].soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }
    else
    {
        enum object_soundset_types oss = objects[obj->otyp].oc_soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }

    float hearing = hearing_array[ox][oy];
    if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
        return;
    else
        volume *= hearing_array[ox][oy];

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.parameter_names[0] = "FloorSurface";
    immediateinfo.parameter_values[0] = (float)floorid;
    immediateinfo.parameter_names[1] = "Weight";
    immediateinfo.parameter_values[1] = weight;
    immediateinfo.parameter_names[2] = (char*)0;
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);
}


void
play_object_floor_sound(obj, sound_type, is_underwater)
struct obj* obj;
enum object_sound_types sound_type;
boolean is_underwater;
{
    if (!obj)
        return;

    if (Deaf)
        return;

    xchar ox = 0, oy = 0;
    if (!get_obj_location(obj, &ox, &oy, 0))
        return;

    if (!isok(ox, oy))
        return;

    play_object_floor_sound_at_location(obj, sound_type, (int)ox, (int)oy, is_underwater);
}

void
play_object_container_in_sound(obj, container)
struct obj* obj;
struct obj* container;
{
    if (!obj || !container)
        return;

    if (Deaf)
        return;

    enum object_sound_types sound_type = OBJECT_SOUND_TYPE_DROP;
    struct ghsound_immediate_info immediateinfo = { 0 };

    enum obj_material_types container_material = objects[container->otyp].oc_material;
    enum floor_surface_types floorid = material_definitions[container_material].floor_surface_mapping;
    float weight = (float)obj->owt;
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;

    if (obj->oartifact && artilist[obj->oartifact].soundset > OBJECT_SOUNDSET_NONE)
    {
        enum object_soundset_types oss = artilist[obj->oartifact].soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }
    else
    {
        enum object_soundset_types oss = objects[obj->otyp].oc_soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.parameter_names[0] = "FloorSurface";
    immediateinfo.parameter_values[0] = (float)floorid;
    immediateinfo.parameter_names[1] = "Weight";
    immediateinfo.parameter_values[1] = weight;
    immediateinfo.parameter_names[2] = (char*)0;
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);
}


void
play_simple_location_sound(x, y, sound_type)
xchar x, y;
enum location_sound_types sound_type;
{
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    struct ghsound_immediate_info immediateinfo = { 0 };

    if (!isok(x, y))
        return;

    if (Deaf)
        return;

    int ltyp = levl[x][y].typ;
    int lsubtyp = levl[x][y].subtyp;

    enum location_soundset_types lss = 0;

    /* Doors use subtype soundsets rather than location soundset */
    if (IS_DOOR_OR_SDOOR(ltyp) && lsubtyp >= 0 && lsubtyp < MAX_DOOR_SUBTYPES)
        lss = door_subtype_definitions[lsubtyp].soundset;
    else
        lss = location_type_definitions[ltyp].soundset;

    set_simple_location_sound_id_and_volume(lss, sound_type, &soundid, &volume);
    //soundid = location_soundsets[lss].sounds[sound_type].ghsound;
    //volume = location_soundsets[lss].sounds[sound_type].volume;

    /* Move one square towards the player, since the square itself may be rock etc. */
    xchar hear_x = x + sgn(u.ux - x);
    xchar hear_y = y + sgn(u.uy - y);

    if (!isok(hear_x, hear_y))
        hear_x = x, hear_y = y;

    float hearing = hearing_array[hear_x][hear_y];
    if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
        return;
    else
        volume *= hearing_array[hear_x][hear_y];

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
play_simple_player_sound(sound_type)
enum monster_sound_types sound_type;
{
    play_simple_player_sound_with_flags(sound_type, 0UL);
}

void
play_simple_player_sound_with_flags(sound_type, play_flags)
enum monster_sound_types sound_type;
unsigned long play_flags;
{
    /* Do not use for hit sounds */

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    struct ghsound_immediate_info immediateinfo = { 0 };

    if (Upolyd)
    {
        enum monster_soundset_types mss = get_monster_soundset(&youmonst);
        set_simple_monster_sound_id_and_volume(mss, sound_type, &soundid, &volume);
    }
    else
    {
        enum player_soundset_types pss = get_player_soundset();
        set_simple_player_sound_id_and_volume(pss, sound_type, &soundid, &volume);
    }

    xchar x = u.ux, y = u.uy;

    if (isok(x, y))
    {
        float hearing = hearing_array[x][y];
        if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
            return;
        else
            volume *= hearing_array[x][y];
    }
    else
        return;

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;
    immediateinfo.play_flags = play_flags;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
play_player_line_indexed_sound(sound_type, line_id, play_group, sfx_sound_type)
enum monster_sound_types sound_type;
int line_id;
enum sound_play_groups play_group;
enum immediate_sound_types sfx_sound_type;
{
    /* Do not use for hit sounds */

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    struct ghsound_immediate_info immediateinfo = { 0 };

    if (Upolyd)
    {
        enum monster_soundset_types mss = get_monster_soundset(&youmonst);
        set_simple_monster_sound_id_and_volume(mss, sound_type, &soundid, &volume);
    }
    else
    {
        enum player_soundset_types pss = get_player_soundset();
        set_simple_player_sound_id_and_volume(pss, sound_type, &soundid, &volume);
    }

    immediateinfo.ghsound = soundid;
    immediateinfo.parameter_names[0] = "LineIndex";
    immediateinfo.parameter_values[0] = (float)line_id;
    immediateinfo.parameter_names[1] = (char*)0;

    xchar x = u.ux, y = u.uy;
    if (isok(x, y))
    {
        float hearing = hearing_array[x][y];
        if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
            return;
        else
            volume *= hearing_array[x][y];
    }
    else
        return;

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = sfx_sound_type;
    immediateinfo.play_group = play_group;
    immediateinfo.dialogue_mid = youmonst.m_id;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}


void
play_player_ouch_sound(line_id)
enum monster_ouch_sounds line_id;
{
    play_player_line_indexed_sound(MONSTER_SOUND_TYPE_OUCH, line_id, SOUND_PLAY_GROUP_NORMAL, IMMEDIATE_SOUND_SFX);
}


void
play_simple_player_voice(sound_type)
enum player_voice_types sound_type;
{
    /* Do not use for hit sounds */

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    struct ghsound_immediate_info immediateinfo = { 0 };

    enum player_soundset_types pss = get_player_soundset();
    set_simple_player_voice_id_and_volume(pss, sound_type, &soundid, &volume);

    xchar x = u.ux, y = u.uy;

    if (isok(x, y))
    {
        float hearing = hearing_array[x][y];
        if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
            return;
        else
            volume *= hearing_array[x][y];
    }
    else
        return;

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_LONG;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
play_simple_monster_sound(mon, sound_type)
struct monst* mon;
enum monster_sound_types sound_type;
{
    play_simple_monster_sound_with_flags(mon, sound_type, PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING_OR_QUEUED);
}

void
play_simple_monster_sound_with_flags(mon, sound_type, play_flags)
struct monst* mon;
enum monster_sound_types sound_type;
unsigned long play_flags;
{
    /* Do not use for hit sounds */
    boolean isyou = (mon == &youmonst);

    if (!mon)
        return;

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    struct ghsound_immediate_info immediateinfo = { 0 };
    enum immediate_sound_types sfxtype = IMMEDIATE_SOUND_SFX;
    enum sound_play_groups playgroup = SOUND_PLAY_GROUP_NORMAL;
    enum monster_soundset_types mss = get_monster_soundset(mon);
    set_simple_monster_sound_id_and_volume(mss, sound_type, &soundid, &volume);

    if (!Upolyd && isyou)
    {
        enum player_soundset_types pss = get_player_soundset();
        set_simple_player_sound_id_and_volume(pss, sound_type, &soundid, &volume);
    }

    /* Treat potential spoken lines as queueable dialogue */
    if ((humanoid(mon->data) || is_speaking_monster(mon->data)) && (
        sound_type == MONSTER_SOUND_TYPE_CREATION
        || sound_type == MONSTER_SOUND_TYPE_GET_ANGRY
        || sound_type == MONSTER_SOUND_TYPE_MOLLIFIED
        || sound_type == MONSTER_SOUND_TYPE_CURSE
        || sound_type == MONSTER_SOUND_TYPE_MUMBLED_CURSE
        || sound_type == MONSTER_SOUND_TYPE_YELL
        )
        )
    {
        sfxtype = IMMEDIATE_SOUND_DIALOGUE;
        playgroup = SOUND_PLAY_GROUP_LONG;
    }

    xchar x = isyou ? u.ux : mon->mx, y = isyou ? u.uy : mon->my;

    if (isok(x, y))
    {
        float hearing = hearing_array[x][y];
        if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
            return;
        else
            volume *= hearing_array[x][y];
    }
    else
        return;

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = sfxtype;
    immediateinfo.play_group = playgroup;
    immediateinfo.dialogue_mid = mon->m_id;
    immediateinfo.play_flags = play_flags;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
play_monster_simple_weapon_sound(magr, attack_number, weapon, sound_type)
struct monst* magr;
int attack_number;
struct obj* weapon;
enum object_sound_types sound_type;
{
    /* Do not use for hit sounds */

    if (!magr)
        return;

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    boolean you_attack = (magr == &youmonst);
    struct ghsound_immediate_info immediateinfo = { 0 };

    if (weapon)
    {
        if (weapon->oartifact && artilist[weapon->oartifact].soundset > OBJECT_SOUNDSET_NONE)
        {
            enum object_soundset_types oss = artilist[weapon->oartifact].soundset;
            set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
        }
        else
        {
            enum object_soundset_types oss = objects[weapon->otyp].oc_soundset;
            set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
        }
    }
    else
    {
        if (!Upolyd && you_attack)
        {
            enum player_soundset_types pss = get_player_soundset();
            enum object_soundset_types oss = player_soundsets[pss].attack_soundsets[PLAYER_ATTACK_SOUNDSET_BAREHANDED];
            set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
        }
        else
        {
            enum monster_soundset_types mss = get_monster_soundset(magr);
            enum object_soundset_types oss = monster_soundsets[mss].attack_soundsets[attack_number];
            set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
        }
    }

    if (!you_attack)
    {
        if (isok(magr->mx, magr->my))
        {
            float hearing = hearing_array[magr->mx][magr->my];
            if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
                return;
            else
                volume *= hearing_array[magr->mx][magr->my];
        }
        else
            return;
    }

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
play_monster_attack_floor_sound(mtmp, attack_number, sound_type)
struct monst* mtmp;
int attack_number;
enum object_sound_types sound_type;
{
    if (Deaf)
        return;

    boolean isyou = (mtmp == &youmonst);
    xchar ox = isyou ? u.ux : mtmp->mx, oy = isyou ? u.uy : mtmp->my;

    if (!isok(ox, oy))
        return;

    boolean is_underwater = isyou ? Underwater : is_swimmer(mtmp->data) && is_pool(ox, oy);
    struct ghsound_immediate_info immediateinfo = { 0 };
    enum floor_surface_types floorid = get_floor_surface_type(ox, oy, is_underwater);
    float weight = 1.0f;
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;

    if (!Upolyd && isyou)
    {
        enum player_soundset_types pss = get_player_soundset();
        enum object_soundset_types oss = player_soundsets[pss].attack_soundsets[attack_number == NATTK ? PLAYER_ATTACK_SOUNDSET_BAREFOOTED : PLAYER_ATTACK_SOUNDSET_BAREHANDED];
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }
    else
    {
        enum monster_soundset_types mss = get_monster_soundset(mtmp);
        enum object_soundset_types oss = monster_soundsets[mss].attack_soundsets[attack_number];
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }

    float hearing = hearing_array[ox][oy];
    if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
        return;
    else
        volume *= hearing_array[ox][oy];

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.parameter_names[0] = "FloorSurface";
    immediateinfo.parameter_values[0] = (float)floorid;
    immediateinfo.parameter_names[1] = "Weight";
    immediateinfo.parameter_values[1] = weight;
    immediateinfo.parameter_names[2] = (char*)0;
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);
}

void
get_hit_location(surface_type, surface_source_ptr, defx_ptr, defy_ptr)
enum hit_surface_source_types surface_type;
anything* surface_source_ptr;
xchar* defx_ptr, * defy_ptr;
{
    if (!surface_source_ptr || !defx_ptr || !defy_ptr)
        return;

    boolean you_defend = FALSE;
    anything surface_source = *surface_source_ptr;

    struct monst* mdef = 0;
    struct obj* obj = 0;
    coord cc = { 0 };

    *defx_ptr = 0;
    *defy_ptr = 0;

    if (surface_type == HIT_SURFACE_SOURCE_MONSTER)
    {
        mdef = surface_source.a_monst;
        if (!mdef)
            return;

        you_defend = (mdef == &youmonst);

        if (you_defend)
        {
            *defx_ptr = u.ux;
            *defy_ptr = u.uy;
        }
        else
        {
            *defx_ptr = mdef->mx;
            *defy_ptr = mdef->my;
        }
    }
    else if (surface_type == HIT_SURFACE_SOURCE_OBJECT)
    {
        obj = surface_source.a_obj;
        if (!obj)
            return;
        get_obj_location(obj, defx_ptr, defy_ptr, 0);
    }
    else if (surface_type == HIT_SURFACE_SOURCE_LOCATION)
    {
        cc.x = surface_source.a_coord.x;
        cc.y = surface_source.a_coord.y;
        if (!isok(cc.x, cc.y))
            return;

        /* Move one square closer to the player */
        xchar hear_x = cc.x + sgn(u.ux - cc.x);
        xchar hear_y = cc.y + sgn(u.uy - cc.y);

        if (isok(hear_x, hear_y))
        {
            *defx_ptr = hear_x;
            *defy_ptr = hear_y;
        }
        else
        {
            *defx_ptr = cc.x;
            *defy_ptr = cc.y;
        }
    }
    else if (surface_type == HIT_SURFACE_SOURCE_TRAP)
    {
        struct trap* t = surface_source.a_trap;
        if (!t)
            return;
        *defx_ptr = t->tx;
        *defy_ptr = t->ty;
    }

}



enum hit_surface_types
    get_hit_surface_type(surface_type, surface_source_ptr)
    enum hit_surface_source_types surface_type;
anything* surface_source_ptr;
{
    enum hit_surface_types surfaceid = HIT_SURFACE_NONE;
    enum obj_material_types surface_material = MAT_NONE;
    if (!surface_source_ptr)
        return surfaceid;

    anything surface_source = *surface_source_ptr;

    if (surface_type == HIT_SURFACE_SOURCE_MONSTER)
    {
        struct monst* mdef = surface_source.a_monst;
        if (!mdef)
            return surfaceid;

        boolean you_defend = (mdef == &youmonst);

        surface_material = mdef->data->natural_armor_material_type;
        if (you_defend)
        {
            /* Maybe change if you are wearing armor */
        }
        else if (mdef)
        {
            /* Maybe change if monster is wearing armor */
        }
    }
    else if (surface_type == HIT_SURFACE_SOURCE_OBJECT)
    {
        struct obj* obj = surface_source.a_obj;
        if (!obj)
            return surfaceid;
        surface_material = objects[obj->otyp].oc_material;
    }
    else if (surface_type == HIT_SURFACE_SOURCE_LOCATION)
    {
        coord cc = { 0 };
        cc.x = surface_source.a_coord.x;
        cc.y = surface_source.a_coord.y;
        if (!isok(cc.x, cc.y))
            return surfaceid;
        surface_material = location_type_definitions[levl[cc.x][cc.y].typ].material;
    }
    else if (surface_type == HIT_SURFACE_SOURCE_TRAP)
    {
        struct trap* t = surface_source.a_trap;
        if (!t)
            return surfaceid;
        surface_material = trap_type_definitions[t->ttyp].material;
    }

    surfaceid = material_definitions[surface_material].hit_surface_mapping;

    return surfaceid;
}

void
play_monster_weapon_hit_sound(magr, surface_type, surface_source_ptr, attack_number, weapon, damage, thrown)
struct monst* magr;
enum hit_surface_source_types surface_type;
anything* surface_source_ptr;
int attack_number; /* attack_number == NATTK indicates kicking */
struct obj* weapon;
double damage;
enum hmon_atkmode_types thrown;
{
    if (!magr || !surface_source_ptr)
        return;

    if (Deaf)
        return;

    xchar defx = 0, defy = 0;
    get_hit_location(surface_type, surface_source_ptr, &defx, &defy);
    if (!isok(defx, defy))
        return;

    play_monster_weapon_hit_sound_at_location(magr, surface_type, surface_source_ptr, attack_number, weapon, damage, thrown, defx, defy);
}

void
play_monster_weapon_hit_sound_at_location(magr, surface_type, surface_source_ptr, attack_number, weapon, damage, thrown, x, y)
struct monst* magr;
enum hit_surface_source_types surface_type;
anything* surface_source_ptr;
int attack_number; /* attack_number == NATTK indicates kicking */
struct obj* weapon;
double damage;
enum hmon_atkmode_types thrown;
xchar x, y;
{
    if (!magr || !surface_source_ptr)
        return;

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    enum object_sound_types sound_type = (thrown == HMON_MELEE ? OBJECT_SOUND_TYPE_HIT_MELEE : OBJECT_SOUND_TYPE_HIT_THROW);
    struct ghsound_immediate_info immediateinfo = { 0 };

    xchar defx = x, defy = y;
    if (!isok(defx, defy))
        return;

    if (weapon)
    {
        if (weapon->oartifact && artilist[weapon->oartifact].soundset > OBJECT_SOUNDSET_NONE)
        {
            enum object_soundset_types oss = artilist[weapon->oartifact].soundset;
            set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
        }
        else
        {
            enum object_soundset_types oss = objects[weapon->otyp].oc_soundset;
            set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
        }
    }
    else
    {
        enum monster_soundset_types mss = get_monster_soundset(magr);
        enum object_soundset_types oss = monster_soundsets[mss].attack_soundsets[attack_number];
        soundid = object_soundsets[oss].sounds[sound_type].ghsound;
        volume = object_soundsets[oss].sounds[sound_type].volume;
    }

    /* Hit sound is based on the defender's location */
    if (isok(defx, defy))
    {
        float hearing = hearing_array[defx][defy];
        if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
            return;
        else
            volume *= hearing_array[defx][defy];
    }

    enum hit_surface_types surfaceid = get_hit_surface_type(surface_type, surface_source_ptr);

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.parameter_names[0] = "HitSurface";
    immediateinfo.parameter_values[0] = (float)surfaceid;
    immediateinfo.parameter_names[1] = "Damage";
    immediateinfo.parameter_values[1] = (float)damage;
    immediateinfo.parameter_names[2] = (char*)0;
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
play_object_hit_sound(obj, surface_type, surface_source_ptr, damage, thrown)
struct obj* obj;
enum hit_surface_source_types surface_type;
anything* surface_source_ptr;
double damage;
enum hmon_atkmode_types thrown;
{
    if (!surface_source_ptr || !obj)
        return;

    if (Deaf)
        return;

    xchar defx = 0, defy = 0;
    get_hit_location(surface_type, surface_source_ptr, &defx, &defy);
    if (!isok(defx, defy))
        return;

    play_object_hit_sound_at_location(obj, surface_type, surface_source_ptr, damage, thrown, defx, defy);
}

void
play_object_hit_sound_at_location(obj, surface_type, surface_source_ptr, damage, thrown, x, y)
struct obj* obj;
enum hit_surface_source_types surface_type;
anything* surface_source_ptr;
double damage;
enum hmon_atkmode_types thrown;
xchar x, y;
{
    if (!surface_source_ptr || !obj)
        return;

    if (Deaf)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    enum object_sound_types sound_type = (thrown == HMON_MELEE ? OBJECT_SOUND_TYPE_HIT_MELEE : OBJECT_SOUND_TYPE_HIT_THROW);
    struct ghsound_immediate_info immediateinfo = { 0 };

    if (!isok(x, y))
        return;

    if (obj->oartifact && artilist[obj->oartifact].soundset > OBJECT_SOUNDSET_NONE)
    {
        enum object_soundset_types oss = artilist[obj->oartifact].soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }
    else
    {
        enum object_soundset_types oss = objects[obj->otyp].oc_soundset;
        set_simple_object_sound_id_and_volume(oss, sound_type, &soundid, &volume);
    }

    if (isok(x, y))
    {
        float hearing = hearing_array[x][y];
        if (hearing == 0.0f && context.global_minimum_volume == 0.0f)
            return;
        else
            volume *= hearing_array[x][y];
    }
    else
        return;


    enum hit_surface_types surfaceid = get_hit_surface_type(surface_type, surface_source_ptr);

    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume));
    immediateinfo.parameter_names[0] = "HitSurface";
    immediateinfo.parameter_values[0] = (float)surfaceid;
    immediateinfo.parameter_names[1] = "Damage";
    immediateinfo.parameter_values[1] = (float)damage;
    immediateinfo.parameter_names[2] = (char*)0;
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);

}

void
play_sfx_sound(sfx_sound_id)
enum sfx_sound_types sfx_sound_id;
{
    play_sfx_sound_with_flags(sfx_sound_id, 0UL);
}

void
play_sfx_sound_with_flags(sfx_sound_id, play_flags)
enum sfx_sound_types sfx_sound_id;
unsigned long play_flags;
{
    if (sfx_sound_id < 0 || sfx_sound_id >= MAX_SFX_SOUND_TYPES)
        return;

    if (Deaf && sfx_sounds[sfx_sound_id].affected_by_deafness)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;

    soundid = sfx_sounds[sfx_sound_id].sound.ghsound;
    volume = min(1.0f, sfx_sounds[sfx_sound_id].sound.volume);

    struct ghsound_immediate_info immediateinfo = { 0 };
    immediateinfo.ghsound = soundid;
    immediateinfo.volume = volume;
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = sfx_sounds[sfx_sound_id].play_group;
    immediateinfo.play_flags = play_flags;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);
}

void
play_ui_sound(ui_sound_id)
enum ui_sound_types ui_sound_id;
{

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;

    soundid = ui_sounds[ui_sound_id].sound.ghsound;
    volume = ui_sounds[ui_sound_id].sound.volume;

    struct ghsound_immediate_info immediateinfo = { 0 };
    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, volume);
    immediateinfo.sound_type = IMMEDIATE_SOUND_UI;
    immediateinfo.play_group = ui_sounds[ui_sound_id].play_group;

    if (soundid > GHSOUND_NONE && volume > 0.0f)
        play_immediate_ghsound(immediateinfo);
}

void
play_sfx_sound_at_location(sfx_sound_id, x, y)
enum sfx_sound_types sfx_sound_id;
int x, y;
{
    play_sfx_sound_at_location_with_minimum_volume(sfx_sound_id, x, y, 0.0);
}

void
play_sfx_sound_at_location_with_minimum_volume(sfx_sound_id, x, y, min_volume_d)
enum sfx_sound_types sfx_sound_id;
int x, y;
double min_volume_d;
{
    if (sfx_sound_id < 0 || sfx_sound_id >= MAX_SFX_SOUND_TYPES)
        return;

    float min_volume = (float)min_volume_d;

    if (min_volume < 0.0f)
        min_volume = 0.0f;

    if (!isok(x, y) || (hearing_array[x][y] == 0.0f && min_volume == 0.0f && context.global_minimum_volume == 0.0f))
        return;

    if (Deaf && sfx_sounds[sfx_sound_id].affected_by_deafness)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;

    soundid = sfx_sounds[sfx_sound_id].sound.ghsound;
    volume = sfx_sounds[sfx_sound_id].sound.volume;

    struct ghsound_immediate_info immediateinfo = { 0 };
    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, max((float)min_volume, (float)(volume * hearing_array[x][y]))));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = sfx_sounds[sfx_sound_id].play_group;

    if (immediateinfo.ghsound > GHSOUND_NONE && immediateinfo.volume > 0.0f)
        play_immediate_ghsound(immediateinfo);
}


void
play_immediate_instrument_sound(instrument_soundset_id, instrument_sound_type)
enum object_instrument_soundset_types instrument_soundset_id;
enum instrument_sound_types instrument_sound_type;
{
    play_immediate_instrument_sound_at_location(instrument_soundset_id, instrument_sound_type, u.ux, u.uy);
}


void
play_immediate_instrument_sound_at_location(instrument_soundset_id, instrument_sound_type, x, y)
enum object_instrument_soundset_types instrument_soundset_id;
enum instrument_sound_types instrument_sound_type;
int x, y;
{
    if (!isok(x, y) || (hearing_array[x][y] == 0.0f && context.global_minimum_volume == 0.0f))
        return;

    if (Deaf)
        return;

    enum object_instrument_soundset_types typ = instrument_soundset_id;
    if (instrument_soundsets[typ].sounds[instrument_sound_type].ghsound == GHSOUND_NONE)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;

    soundid = instrument_soundsets[typ].sounds[instrument_sound_type].ghsound;
    volume = instrument_soundsets[typ].sounds[instrument_sound_type].volume;

    struct ghsound_immediate_info immediateinfo = { 0 };
    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume * hearing_array[x][y]));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = instrument_soundsets[typ].sounds[instrument_sound_type].play_group;

    if (immediateinfo.ghsound > GHSOUND_NONE && immediateinfo.volume > 0.0f)
        play_immediate_ghsound(immediateinfo);
}

void
play_immediate_ray_sound_at_location(ray_soundset_id, ray_sound_type, x, y)
int ray_soundset_id;
enum ray_sound_types ray_sound_type;
int x, y;
{
    if (!isok(x, y) || hearing_array[x][y] == 0.0f)
        return;

    if (Deaf)
        return;

    enum object_ray_soundset_types typ = ray_soundset_id;
    if (ray_soundsets[typ].sounds[ray_sound_type].ghsound == GHSOUND_NONE)
        return;

    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;

    soundid = ray_soundsets[typ].sounds[ray_sound_type].ghsound;
    volume = ray_soundsets[typ].sounds[ray_sound_type].volume;

    struct ghsound_immediate_info immediateinfo = { 0 };
    immediateinfo.ghsound = soundid;
    immediateinfo.volume = min(1.0f, max((float)context.global_minimum_volume, volume * hearing_array[x][y]));
    immediateinfo.sound_type = IMMEDIATE_SOUND_SFX;
    immediateinfo.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (immediateinfo.ghsound > GHSOUND_NONE && immediateinfo.volume > 0.0f)
        play_immediate_ghsound(immediateinfo);
}

void
start_ambient_ray_sound_at_location(ray_type, x, y)
int ray_type;
int x, y;
{
    int typ = ray_type;
    if (ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].ghsound == GHSOUND_NONE || ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].ghsound == MAX_GHSOUNDS)
        return;

    float hearing_volume = 0.0f;
    if (isok(x, y))
        hearing_volume = hearing_array[x][y];

    struct ghsound_effect_ambient_info info = { 0 };
    info.ghsound = ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].ghsound;
    info.volume = min(1.0f, max((float)context.global_minimum_volume, ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].volume * hearing_volume));

    play_ghsound_effect_ambient(info);
}

void
update_ambient_ray_sound_to_location(ray_type, x, y)
int ray_type;
int x, y;
{
    int typ = ray_type;
    if (ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].ghsound == GHSOUND_NONE || ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].ghsound == MAX_GHSOUNDS)
        return;

    float hearing_volume = 0.0f;
    if (isok(x, y))
        hearing_volume = hearing_array[x][y];

    struct effect_ambient_volume_info vinfo = { 0 };
    vinfo.volume = min(1.0f, max((float)context.global_minimum_volume, ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].volume * hearing_volume));

    set_effect_ambient_volume(vinfo);
}

void
stop_ambient_ray_sound(ray_type)
int ray_type;
{
    int typ = ray_type;

    if (ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].ghsound == GHSOUND_NONE || ray_soundsets[typ].sounds[RAY_SOUND_TYPE_AMBIENT].ghsound == MAX_GHSOUNDS)
        return;

    struct effect_ambient_volume_info vinfo = { 0 };
    vinfo.volume = 0.0f;

    set_effect_ambient_volume(vinfo);
}


void
start_occupation_ambient_sound(object_soundset_id, occupation_type)
enum object_soundset_types object_soundset_id;
enum object_occupation_types occupation_type;
{
    if (occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[OCCUPATION_SOUND_TYPE_AMBIENT].ghsound == GHSOUND_NONE
        || occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[OCCUPATION_SOUND_TYPE_AMBIENT].ghsound == MAX_GHSOUNDS)
        return;

    struct ghsound_occupation_ambient_info info = { 0 };
    info.ghsound = occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[OCCUPATION_SOUND_TYPE_AMBIENT].ghsound;
    info.volume = min(1.0f, occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[OCCUPATION_SOUND_TYPE_AMBIENT].volume);

    play_ghsound_occupation_ambient(info);
}

void
stop_occupation_ambient_sound(object_soundset_id, occupation_type)
enum object_soundset_types object_soundset_id;
enum object_occupation_types occupation_type;
{
    if (occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[OCCUPATION_SOUND_TYPE_AMBIENT].ghsound == GHSOUND_NONE
        || occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[OCCUPATION_SOUND_TYPE_AMBIENT].ghsound == MAX_GHSOUNDS)
        return;

    struct ghsound_occupation_ambient_info info = { 0 };
    info.volume = 0.0f;

    play_ghsound_occupation_ambient(info);

}

void
play_occupation_immediate_sound(object_soundset_id, occupation_type, sound_type)
enum object_soundset_types object_soundset_id;
enum object_occupation_types occupation_type;
enum occupation_sound_types sound_type;
{

    if (occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[sound_type].ghsound == GHSOUND_NONE
        || occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[sound_type].ghsound == MAX_GHSOUNDS)
        return;

    struct ghsound_immediate_info info = { 0 };
    info.ghsound = occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[sound_type].ghsound;
    info.volume = min(1.0f, occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[sound_type].volume);
    info.play_group = SOUND_PLAY_GROUP_NORMAL;

    play_immediate_ghsound(info);
}

void
play_occupation_immediate_sound_at_location(object_soundset_id, occupation_type, sound_type, x, y)
enum object_soundset_types object_soundset_id;
enum object_occupation_types occupation_type;
enum occupation_sound_types sound_type;
int x, y;
{

    if (!isok(x, y) || occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[sound_type].ghsound == GHSOUND_NONE
        || occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[sound_type].ghsound == MAX_GHSOUNDS)
        return;

    float hearing_volume = hearing_array[x][y];
    struct ghsound_immediate_info info = { 0 };
    info.ghsound = occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[sound_type].ghsound;
    info.volume = min(1.0f, occupation_soundset_definitions[object_soundsets[object_soundset_id].occupation_soundsets[occupation_type]].sounds[sound_type].volume * hearing_volume);
    info.play_group = SOUND_PLAY_GROUP_NORMAL;

    if (info.volume > 0.0f)
        play_immediate_ghsound(info);
}

enum player_soundset_types
    get_player_soundset()
{
    if (flags.female)
        return PLAYER_SOUNDSET_FEMALE;
    else
        return PLAYER_SOUNDSET_GENERAL;
}

void
dosetsoundvolume()
{
    adjust_ghsound_general_volumes();
}

static float prev_hearing_array[COLNO][ROWNO];

void
update_hearing_array(mode)
int mode; /* 0 = normal, 1 = clear */
{
    /* Clear array*/
    memset(hearing_array, 0, sizeof(hearing_array));
    memset(prev_hearing_array, 0, sizeof(prev_hearing_array));

    /* Can't hear anything */
    if (Deaf || mode == 1)
        return;

    int hear_distance = get_max_hearing_distance();
    /* Fill the array */
    hearing_array[u.ux][u.uy] = prev_hearing_array[u.ux][u.uy] = 1.0f;

    int r, i, y, prev_y, x, prev_x;
    for (r = 1; r <= hear_distance; r++)
    {
        int x_min = u.ux - r;
        int x_max = u.ux + r;
        int x_min_adjusted = max(1, x_min);
        int x_max_adjusted = min(COLNO - 1, x_max);

        int y_min = u.uy - r;
        int y_max = u.uy + r;
        int y_min_adjusted = max(0, y_min);
        int y_max_adjusted = min(ROWNO - 1, y_max);

        boolean horizontal_min_done = FALSE;
        boolean horizontal_max_done = FALSE;

        /* 1. Horizontal lines, left to right */
        for (i = 0; i <= 1; i++)
        {
            if (i == 0)
            {
                if (y_min_adjusted != y_min)
                    continue;

                horizontal_min_done = TRUE;
            }
            else if (i == 1)
            {
                if (y_max_adjusted != y_max)
                    continue;

                horizontal_max_done = TRUE;
            }

            y = (i == 0 ? y_min_adjusted : y_max_adjusted);

            for (x = x_min_adjusted; x <= x_max_adjusted; x++)
            {
                float prev_hearing = 0.0f;
                prev_y = (i == 0) ? y + 1 : y - 1;

                if (r == 1)
                {
                    prev_hearing = 1.0f;
                }
                else if (prev_y > y_max_adjusted || prev_y < y_min_adjusted)
                {
                    //Nothing
                }
                else
                {
                    float maximum = 0.0f;

                    /* Take maximum from above or below from the previous round */
                    for (prev_x = max(x_min_adjusted + 1, x - 1); prev_x <= min(x_max_adjusted - 1, x + 1); prev_x++)
                    {
                        maximum = max(maximum, prev_hearing_array[prev_x][prev_y]);
                    }

                    /* Take also previous from the same line */
                    if (x > x_min_adjusted)
                        maximum = max(maximum, prev_hearing_array[x - 1][y]);

                    prev_hearing = maximum;
                }

                if (prev_hearing == 0.0f)
                {
                    continue;
                    /* Current hearing is 0, too */
                }
                else
                {
                    set_hearing_array(x, y, prev_hearing, r);
                }
            }
        }

        /* Vertical lines, top to bottom */
        for (i = 0; i <= 1; i++)
        {
            if (i == 0 && x_min_adjusted != x_min)
                continue;
            if (i == 1 && x_max_adjusted != x_max)
                continue;

            x = (i == 0 ? x_min_adjusted : x_max_adjusted);
            int upper_y_limit = y_max_adjusted - (horizontal_max_done ? 1 : 0);

            for (y = y_min_adjusted + (horizontal_min_done ? 1 : 0); y <= upper_y_limit; y++)
            {
                float prev_hearing = 0.0f;
                prev_x = (i == 0) ? x + 1 : x - 1;

                if (r == 1)
                {
                    prev_hearing = 1.0f;
                }
                else if (prev_x > x_max_adjusted || prev_x < x_min_adjusted)
                {
                    //Nothing
                }
                else
                {
                    float maximum = 0.0f;
                    for (prev_y = max(y_min_adjusted + 1, y - 1); prev_y <= min(y_max_adjusted - 1, y + 1); prev_y++)
                    {
                        maximum = max(maximum, prev_hearing_array[prev_x][prev_y]);
                    }

                    /* Take also previous from the same line */
                    if (y > y_min_adjusted)
                        maximum = max(maximum, prev_hearing_array[x][y - 1]);

                    prev_hearing = maximum;
                }

                if (prev_hearing == 0.0f)
                {
                    continue;
                    /* Current hearing is 0, too */
                }
                else
                {
                    set_hearing_array(x, y, prev_hearing, r);
                }
            }
        }

        /* Vertical lines, bottom to top */
        for (i = 0; i <= 1; i++)
        {
            if (i == 0 && x_max_adjusted != x_max)
                continue;
            if (i == 1 && x_min_adjusted != x_min)
                continue;

            x = (i == 0 ? x_max_adjusted : x_min_adjusted);

            for (y = y_max_adjusted - 1; y >= y_min_adjusted; y--)
            {
                if (prev_hearing_array[x][y + 1] > 0.0f)
                    set_hearing_array(x, y, prev_hearing_array[x][y + 1], r);
            }
        }

        /* Horizontal lines, right to left */
        for (i = 0; i <= 1; i++)
        {
            if (i == 0 && y_max_adjusted != y_max)
                continue;
            if (i == 1 && y_min_adjusted != y_min)
                continue;

            y = (i == 0 ? y_max_adjusted : y_min_adjusted);

            for (x = x_max_adjusted - 1; x >= x_min_adjusted; x--)
            {
                if (prev_hearing_array[x + 1][y] > 0.0f)
                    set_hearing_array(x, y, prev_hearing_array[x + 1][y], r);
            }
        }
    }

}

STATIC_OVL
void set_hearing_array(x, y, prev_hearing, radius)
int x, y;
double prev_hearing;
int radius;
{
    float multiplier = 1.0f;
    if (radius <= 0)
    {
        hearing_array[x][y] = 1.0f;
        prev_hearing_array[x][y] = 1.0f;
        return;
    }
    else if (radius == 1)
    {
        multiplier = 0.75f; /* Specially adjusted value */
    }
    else if (radius < 5)
    {
        multiplier = 0.65f; /* Specially adjusted value */
    }
    else if (radius >= 5)
    {
        float prev_factor = 1.0f / ((float)(radius - 1) * (float)(radius - 1));
        float curr_factor = 1.0f / ((float)radius * (float)radius);
        multiplier = curr_factor / prev_factor;
    }

    struct monst* mtmp = m_at(x, y);
    if ((IS_ROCK(levl[x][y].typ) && !IS_TREE(levl[x][y].typ))
        || (mtmp && is_lightblocker_mappear(mtmp)))
    {
        if (1 /*mtmp*/)
        {
            /* Make sure that monsters walking through in walls etc. produce a sound when hit etc. */
            float new_prev_hearing = (float)max(0.0f, min(1.0f, multiplier * ((float)prev_hearing) / (20.0f)));
            float new_hearing = (float)max(0.0f, min(1.0f, multiplier * ((float)prev_hearing)));
            if (new_prev_hearing > prev_hearing_array[x][y])
                prev_hearing_array[x][y] = new_prev_hearing;
            if (new_hearing > hearing_array[x][y])
                hearing_array[x][y] = new_hearing;
        }
    }
    else if (IS_DOOR(levl[x][y].typ) && ((levl[x][y].doormask & D_MASK) != 0 && (levl[x][y].doormask & (D_NODOOR | D_ISOPEN | D_BROKEN | D_PORTCULLIS)) == 0))
    {
        float new_prev_hearing = (float)max(0.0f, min(1.0f, multiplier * ((float)prev_hearing) / (10.0f)));
        float new_hearing = (float)max(0.0f, min(1.0f, multiplier * ((float)prev_hearing)));
        if (new_prev_hearing > prev_hearing_array[x][y])
            prev_hearing_array[x][y] = new_prev_hearing;
        if (new_hearing > hearing_array[x][y])
            hearing_array[x][y] = new_hearing;
    }
    else
    {
        float new_prev_hearing = (float)max(0.0f, min(1.0f, multiplier * (float)prev_hearing));
        float new_hearing = (float)max(0.0f, min(1.0f, multiplier * (float)prev_hearing));
        if (new_prev_hearing > prev_hearing_array[x][y])
            prev_hearing_array[x][y] = new_prev_hearing;
        if (new_hearing > hearing_array[x][y])
            hearing_array[x][y] = new_hearing;
    }

}

void
update_ambient_sounds()
{
    struct soundsource_t* curr;
    for (curr = sound_base; curr; curr = curr->next)
    {
        if (!isok(curr->x, curr->y))
            continue;

        if (curr->flags & SSF_NEEDS_FIXUP)
            continue;

        boolean lit = FALSE;
        boolean insidereg = FALSE;

        /* Update sound source location */
        if (curr->type == SOUNDSOURCE_OBJECT)
        {
            if (get_obj_location(curr->id.a_obj, &curr->x, &curr->y, 0))
            {
                /* Do nothing */
            }

            if (curr->id.a_obj)
                lit = curr->id.a_obj->lamplit;
        }
        else if (curr->type == SOUNDSOURCE_MONSTER)
        {
            if (get_mon_location(curr->id.a_monst, &curr->x, &curr->y, 0))
            {
                /* Do nothing */
            }

            if (curr->id.a_monst)
                lit = curr->id.a_monst->data->lightrange;
        }
        else if (curr->type == SOUNDSOURCE_LOCATION)
        {
            curr->x = curr->id.a_coord.x;
            curr->y = curr->id.a_coord.y;

            if (isok(curr->id.a_coord.x, curr->id.a_coord.y))
                lit = levl[curr->id.a_coord.x][curr->id.a_coord.y].lamplit;
        }
        else if (curr->type == SOUNDSOURCE_REGION)
        {
            if (get_region_location(curr->id.a_nhregion, &curr->x, &curr->y, 0))
            {
                /* Do nothing */
            }

            if (curr->id.a_nhregion)
            {
                lit = curr->id.a_nhregion->lamplit;
                insidereg = hero_inside(curr->id.a_nhregion);
            }
        }

        /* Update sound source heard volume */
        float old_heard_volume = curr->heard_volume;
        float hearing_volume = hearing_array[curr->x][curr->y];
        float total_volume = 0.0f;
        if (hearing_volume > 0.0f)
        {
            float source_volume = curr->source_volume;
            total_volume = source_volume * hearing_volume;
            if (curr->subtype == SOUNDSOURCE_AMBIENT_LIT && !lit)
                total_volume = 0.0f;
            else if (curr->subtype == SOUNDSOURCE_AMBIENT_INSIDE_REGION && !insidereg)
                total_volume = 0.0f;
        }

        if (total_volume < SOUND_HEARING_THRESHOLD)
            total_volume = 0.0f; /* turn off if too quiet */

        curr->heard_volume = total_volume;
        if (curr->heard_volume != old_heard_volume)
            set_ambient_ghsound_volume(curr);
    }
}

void
update_hearing_array_and_ambient_sounds()
{
    update_hearing_array(0);
    update_ambient_sounds();
}

void
clear_hearing_array_and_ambient_sounds()
{
    update_hearing_array(1);
    update_ambient_sounds();
}

int get_max_hearing_distance()
{
    return (u.uswallow ? 1 : Underwater ? 2 : MAX_HEARING_DISTANCE);
}

void
update_hearing_array_and_ambient_sounds_if_point_within_hearing_range(px, py)
int px, py;
{
    int max_hear_dist = get_max_hearing_distance();
    if (abs(u.ux - px) <= max_hear_dist && abs(u.uy - py) <= max_hear_dist)
        update_hearing_array_and_ambient_sounds();
}

void
unblock_vision_and_hearing_at_point(x, y)
int x, y;
{
    unblock_point(x, y);
    update_hearing_array_and_ambient_sounds_if_point_within_hearing_range(x, y);
}

void
block_vision_and_hearing_at_point(x, y)
int x, y;
{
    block_point(x, y);
    update_hearing_array_and_ambient_sounds_if_point_within_hearing_range(x, y);
}




/*
 * Ambient sound sources.
 *
 * This implementation minimizes memory at the expense of extra
 * recalculations.
 *
 * Sound sources are "things" that have a physical position and volume.
 * They have a type, which gives us information about them.  Currently
 * they are only attached to objects, monsters, and locations.  Note:  the
 * polymorphed-player handling assumes that both youmonst.m_id and
 * youmonst.mx will always remain 0.
 *
 * Sound sources, like timers, either follow game play (RANGE_GLOBAL) or
 * stay on a level (RANGE_LEVEL).  Sound sources are unique by their
 * (type, id) pair.  For sound sources attached to objects, this id
 * is a pointer to the object.
 *
 * The structure of the save/restore mechanism is amazingly similar to
 * the timer save/restore.  This is because they both have the same
 * principals of having pointers into objects that must be recalculated
 * across saves and restores.
 */

STATIC_DCL void FDECL(write_soundsource, (int, sound_source*));
STATIC_DCL int FDECL(maybe_write_soundsource, (int, int, BOOLEAN_P));


/* Create a new sound source.  */
void
new_sound_source(x, y, ghsound, volume, type, subtype, id)
xchar x, y;
enum ghsound_types ghsound;
double volume;
enum soundsource_types type;
enum soundsource_ambient_subtypes subtype;
anything* id;
{
    sound_source* ss;
    double absvolume = volume > 0.0 ? volume : -volume;
    if (absvolume > 1.0 || absvolume < 0.0)
    {
        impossible("new_sound_source:  illegal volume %f", volume);
        return;
    }

    ss = (sound_source*)alloc(sizeof(sound_source));

    ss->next = sound_base;
    ss->x = x;
    ss->y = y;
    ss->ghsound = ghsound;
    ss->source_volume = ((float)absvolume);
    ss->heard_volume = ss->source_volume * hearing_array[x][y];
    ss->type = type;
    ss->subtype = subtype;
    ss->id = *id;
    ss->flags = volume < 0 ? SSF_SILENCE_SOURCE : 0;
    sound_base = ss;

    add_ambient_ghsound(ss);

    hearing_full_recalc = 1; /* make the source show up */
}

/*
 * Delete a sound source. This assumes only one sound source is attached
 * to an object at a time.
 */
void
del_sound_source(type, id)
enum soundsource_types type;
anything* id;
{
    sound_source* curr, * prev;
    anything tmp_id;

    tmp_id = zeroany;
    /* need to be prepared for dealing a with sound source which
       has only been partially restored during a level change
       (in particular: chameleon vs prot. from shape changers) */
    switch (type)
    {
    case SOUNDSOURCE_OBJECT:
        tmp_id.a_uint = id->a_obj->o_id;
        break;
    case SOUNDSOURCE_MONSTER:
        tmp_id.a_uint = id->a_monst->m_id;
        break;
    case SOUNDSOURCE_LOCATION:
        tmp_id = zeroany;
        tmp_id.a_coord.x = id->a_coord.x;
        tmp_id.a_coord.y = id->a_coord.y;
        break;
    case SOUNDSOURCE_REGION:
        tmp_id.a_uint = get_rid(id->a_nhregion);
        break;
    default:
        tmp_id.a_uint = 0;
        break;
    }

    for (prev = 0, curr = sound_base; curr; prev = curr, curr = curr->next)
    {
        if (curr->type != type)
            continue;

        if ((type == SOUNDSOURCE_LOCATION && curr->id.a_coord.x == tmp_id.a_coord.x && curr->id.a_coord.y == tmp_id.a_coord.y)
            || (type != SOUNDSOURCE_LOCATION && curr->id.a_obj == ((curr->flags & SSF_NEEDS_FIXUP) ? tmp_id.a_obj : id->a_obj))
            )
        {
            if (prev)
                prev->next = curr->next;
            else
                sound_base = curr->next;

            if (delete_ambient_ghsound != 0)
                delete_ambient_ghsound(curr);

            free((genericptr_t)curr);
            hearing_full_recalc = 1;
            return;
        }
    }
    impossible("del_sound_source: not found type=%d, id=%s", type,
        fmt_ptr((genericptr_t)id->a_obj));
}


/* Save all sound sources of the given volume. */
void
save_sound_sources(fd, mode, volume)
int fd, mode, volume;
{
    int count, actual, is_global;
    sound_source** prev, * curr;

    if (perform_bwrite(mode))
    {
        count = maybe_write_soundsource(fd, volume, FALSE);
        bwrite(fd, (genericptr_t)&count, sizeof count);
        actual = maybe_write_soundsource(fd, volume, TRUE);
        if (actual != count)
        {
            panic("counted %d sound sources, wrote %d! [volume=%d]", count,
                actual, volume);
            return;
        }
    }

    if (release_data(mode))
    {
        for (prev = &sound_base; (curr = *prev) != 0;)
        {
            if (!curr->id.a_monst)
            {
                impossible("save_sound_sources: no id! [volume=%d]", volume);
                is_global = 0;
            }
            else
                switch (curr->type)
                {
                case SOUNDSOURCE_OBJECT:
                    is_global = !obj_is_local(curr->id.a_obj);
                    break;
                case SOUNDSOURCE_MONSTER:
                    is_global = !mon_is_local_mx(curr->id.a_monst);
                    break;
                case SOUNDSOURCE_LOCATION:
                    is_global = 0; /* always local by definition */
                    break;
                case SOUNDSOURCE_REGION:
                    is_global = 0; /* always local by definition */
                    break;
                default:
                    is_global = 0;
                    impossible("save_sound_sources: bad type (%d) [volume=%d]",
                        curr->type, volume);
                    break;
                }
            /* if global and not doing local, or vice versa, remove it */
            if (is_global ^ (volume == RANGE_LEVEL))
            {
                *prev = curr->next;
                if (delete_ambient_ghsound != 0)
                    delete_ambient_ghsound(curr);
                free((genericptr_t)curr);
            }
            else
            {
                prev = &(*prev)->next;
            }
        }
    }
}

/*
 * Pull in the structures from disk, but don't recalculate the object
 * pointers.
 */
void
restore_sound_sources(fd)
int fd;
{
    int count;
    sound_source* ss;

    /* restore elements */
    mread(fd, (genericptr_t)&count, sizeof count);

    while (count-- > 0) {
        ss = (sound_source*)alloc(sizeof(sound_source));
        mread(fd, (genericptr_t)ss, sizeof(sound_source));
        ss->next = sound_base;
        sound_base = ss;
        if (add_ambient_ghsound != 0)
            add_ambient_ghsound(ss);
    }
}

/* to support '#stats' wizard-mode command */
void
sound_stats(hdrfmt, hdrbuf, count, size)
const char* hdrfmt;
char* hdrbuf;
long* count;
size_t* size;
{
    sound_source* ss;

    Sprintf(hdrbuf, hdrfmt, sizeof(sound_source));
    *count = *size = 0L;
    for (ss = sound_base; ss; ss = ss->next) {
        ++* count;
        *size += sizeof * ss;
    }
}

/* Relink all sounds that are so marked. */
void
relink_sound_sources(ghostly)
boolean ghostly;
{
    char which;
    unsigned nid;
    sound_source* ss;

    for (ss = sound_base; ss; ss = ss->next)
    {
        if (ss->flags & SSF_NEEDS_FIXUP)
        {
            if (ss->type == SOUNDSOURCE_OBJECT || ss->type == SOUNDSOURCE_MONSTER || ss->type == SOUNDSOURCE_LOCATION || ss->type == SOUNDSOURCE_REGION)
            {
                if (ghostly && ss->type != SOUNDSOURCE_LOCATION && ss->type != SOUNDSOURCE_REGION)
                {
                    if (!lookup_id_mapping(ss->id.a_uint, &nid))
                        impossible("relink_sound_sources: no id mapping");
                }
                else
                    nid = ss->id.a_uint;

                if (ss->type == SOUNDSOURCE_OBJECT)
                {
                    which = 'o';
                    ss->id.a_obj = find_oid(nid);
                }
                else if (ss->type == SOUNDSOURCE_MONSTER)
                {
                    which = 'm';
                    ss->id.a_monst = find_mid(nid, FM_EVERYWHERE);
                }
                else if (ss->type == SOUNDSOURCE_LOCATION)
                {
                    which = 'l';
                    ss->id = zeroany;
                    ss->id.a_coord.x = ss->x;
                    ss->id.a_coord.y = ss->y;
                }
                else if (ss->type == SOUNDSOURCE_REGION)
                {
                    which = 'r';
                    ss->id.a_nhregion = find_rid(nid);
                }

                if (!ss->id.a_monst)
                    impossible("relink_sound_sources: cant find %c_id %d", which, nid);
            }
            else
                impossible("relink_sound_sources: bad type (%d)", ss->type);

            ss->flags &= ~SSF_NEEDS_FIXUP;
        }
    }
}

/*
 * Part of the sound source save routine.  Count up the number of sound
 * sources that would be written.  If write_it is true, actually write
 * the sound source out.
 */
STATIC_OVL int
maybe_write_soundsource(fd, volume, write_it)
int fd, volume;
boolean write_it;
{
    int count = 0, is_global;
    sound_source* ss;

    for (ss = sound_base; ss; ss = ss->next)
    {
        if (!ss->id.a_monst)
        {
            impossible("maybe_write_soundsource: no id! [volume=%d]", volume);
            continue;
        }

        switch (ss->type)
        {
        case SOUNDSOURCE_OBJECT:
            is_global = !obj_is_local(ss->id.a_obj);
            break;
        case SOUNDSOURCE_MONSTER:
            is_global = !mon_is_local_mx(ss->id.a_monst);
            break;
        case SOUNDSOURCE_LOCATION:
            is_global = 0; /* always local */
            break;
        case SOUNDSOURCE_REGION:
            is_global = 0; /* always local */
            break;
        default:
            is_global = 0;
            impossible("maybe_write_soundsource: bad type (%d) [volume=%d]", ss->type, volume);
            break;
        }

        /* if global and not doing local, or vice versa, count it */
        if (is_global ^ (volume == RANGE_LEVEL))
        {
            count++;
            if (write_it)
                write_soundsource(fd, ss);
        }
    }

    return count;
}

void
sound_sources_sanity_check()
{
    sound_source* ss;
    struct monst* mtmp;
    struct obj* otmp;
    struct nhregion* reg;
    unsigned int auint = 0;

    for (ss = sound_base; ss; ss = ss->next)
    {
        if (!ss->id.a_monst)
        {
            panic("insane sound source: no id!");
            return;
        }
        if (ss->type == SOUNDSOURCE_OBJECT)
        {
            otmp = (struct obj*)ss->id.a_obj;
            if (otmp)
                auint = otmp->o_id;

            if (find_oid(auint) != otmp)
            {
                panic("insane sound source: can't find obj #%u!", auint);
                return;
            }
        }
        else if (ss->type == SOUNDSOURCE_MONSTER)
        {
            mtmp = (struct monst*)ss->id.a_monst;
            if (mtmp)
                auint = mtmp->m_id;

            if (find_mid(auint, FM_EVERYWHERE) != mtmp)
            {
                panic("insane sound source: can't find mon #%u!", auint);
                return;
            }
        }
        else if (ss->type == SOUNDSOURCE_LOCATION)
        {
            coord c = ss->id.a_coord;
            if (!isok(c.x, c.y))
            {
                panic("insane sound source: invalid location coordinates (%d, %d)!", c.x, c.y);
                return;
            }
        }
        else if (ss->type == SOUNDSOURCE_REGION)
        {
            reg = (struct nhregion*)ss->id.a_nhregion;
            if (reg)
                auint = get_rid(reg);

            if (find_rid(auint) != reg)
            {
                panic("insane sound source: can't find nhregion #%u!", auint);
                return;
            }
        }
        else
        {
            panic("insane sound source: bad ss type %d", ss->type);
            return;
        }
    }
}

/* Write a sound source structure to disk. */
STATIC_OVL void
write_soundsource(fd, ss)
int fd;
sound_source* ss;
{
    anything arg_save;
    struct obj* otmp;
    struct monst* mtmp;
    struct nhregion* reg;

    if (ss->type == SOUNDSOURCE_OBJECT || ss->type == SOUNDSOURCE_MONSTER || ss->type == SOUNDSOURCE_LOCATION || ss->type == SOUNDSOURCE_REGION)
    {
        if (ss->flags & SSF_NEEDS_FIXUP)
        {
            bwrite(fd, (genericptr_t)ss, sizeof(sound_source));
        }
        else
        {
            /* replace object pointer with id for write, then put back */
            arg_save = ss->id;
            if (ss->type == SOUNDSOURCE_OBJECT)
            {
                otmp = ss->id.a_obj;
                ss->id = zeroany;
                ss->id.a_uint = otmp->o_id;
                if (find_oid((unsigned)ss->id.a_uint) != otmp)
                    impossible("write_soundsource: can't find obj #%u!",
                        ss->id.a_uint);
            }
            else if (ss->type == SOUNDSOURCE_MONSTER)
            {
                mtmp = (struct monst*)ss->id.a_monst;
                ss->id = zeroany;
                ss->id.a_uint = mtmp->m_id;
                if (find_mid((unsigned)ss->id.a_uint, FM_EVERYWHERE) != mtmp)
                    impossible("write_soundsource: can't find mon #%u!",
                        ss->id.a_uint);
            }
            else if (ss->type == SOUNDSOURCE_LOCATION)
            {
                /* No need to do anything, coord can be written to disk as is */
            }
            else if (ss->type == SOUNDSOURCE_REGION)
            {
                reg = (struct nhregion*)ss->id.a_nhregion;
                ss->id = zeroany;
                ss->id.a_uint = get_rid(reg);
                if (find_rid((unsigned)ss->id.a_uint) != reg)
                    impossible("write_soundsource: can't find nhregion #%u!",
                        ss->id.a_uint);
            }

            ss->flags |= SSF_NEEDS_FIXUP;
            bwrite(fd, (genericptr_t)ss, sizeof(sound_source));
            ss->id = arg_save;
            ss->flags &= ~SSF_NEEDS_FIXUP;
        }
    }
    else
    {
        impossible("write_soundsource: bad type (%d)", ss->type);
    }
}

/* Change sound source's ID from src to dest. */
void
obj_move_sound_source(src, dest)
struct obj* src, * dest;
{
    sound_source* ss;

    for (ss = sound_base; ss; ss = ss->next)
    {
        if (ss->type == SOUNDSOURCE_OBJECT && ss->id.a_obj == src)
            ss->id.a_obj = dest;
    }

    src->makingsound = 0;
    dest->makingsound = 1;
}

/* return true if there exist any sound sources */
boolean
any_sound_source()
{
    return (boolean)(sound_base != (sound_source*)0);
}

/*
 * Snuff an object sound source if at (x,y).  This currently works
 * only for burning sound sources.
 */
void
snuff_sound_source(x, y)
int x, y;
{
    sound_source* ss;
    struct obj* obj;

    for (ss = sound_base; ss; ss = ss->next)
        /*
         * Is this position check valid??? Can I assume that the positions
         * will always be correct because the objects would have been
         * updated with the last vision update?  [Is that recent enough???]
         */
        if (ss->type == SOUNDSOURCE_OBJECT && ss->x == x && ss->y == y) {
            obj = ss->id.a_obj;
            //if (obj_is_burning(obj)) // Always true
            {
                del_sound_source(SOUNDSOURCE_OBJECT, obj_to_any(obj));
                end_sound(obj, FALSE);
                /*
                 * The current ss element has just been removed (and
                 * ss->next is now invalid).  Return assuming that there
                 * is only one sound source attached to each object.
                 */
                return;
            }
        }
        else if (ss->type == SOUNDSOURCE_LOCATION && ss->x == x && ss->y == y)
        {
            if (levl[x][y].makingsound)
            {
                levl[x][y].makingsound = 0;
                del_sound_source(SOUNDSOURCE_LOCATION, xy_to_any(x, y));
                newsym(x, y);
                return;
            }
        }

}


/* copy the sound source(s) attached to src, and attach it/them to dest */
void
obj_split_sound_source(src, dest)
struct obj* src, * dest;
{
    sound_source* ss, * new_ss;

    for (ss = sound_base; ss; ss = ss->next)
        if (ss->type == SOUNDSOURCE_OBJECT && ss->id.a_obj == src)
        {
            /*
             * Insert the new source at beginning of list.  This will
             * never interfere us walking down the list - we are already
             * past the insertion point.
             */
            new_ss = (sound_source*)alloc(sizeof(sound_source));
            *new_ss = *ss;
            new_ss->id.a_obj = dest;
            new_ss->next = sound_base;
            sound_base = new_ss;
            //dest->lamplit = 1; /* now an active sound source */
        }
}

/* sound source `src' has been folded into sound source `dest';
   used for merging lit candles and adding candle(s) to lit candelabrum */
void
obj_merge_sound_sources(src, dest)
struct obj* src, * dest;
{
    sound_source* ss;

    /* src == dest implies adding to candelabrum */
    if (src != dest)
        del_sound_source(SOUNDSOURCE_OBJECT, obj_to_any(src));
    //end_burn(src, TRUE); /* extinguish candles */


    for (ss = sound_base; ss; ss = ss->next)
        if (ss->type == SOUNDSOURCE_OBJECT && ss->id.a_obj == dest)
        {
            //ss->volume = candle_sound_range(dest);
            hearing_full_recalc = 1; /* in case volume changed */
            break;
        }
}

/* sound source `obj' is being made brighter or dimmer */
void
obj_adjust_sound_volume(obj, new_volume)
struct obj* obj;
double new_volume;
{
    sound_source* ss;

    for (ss = sound_base; ss; ss = ss->next)
        if (ss->type == SOUNDSOURCE_OBJECT && ss->id.a_obj == obj) {
            if ((float)new_volume != ss->source_volume)
                hearing_full_recalc = 1;
            ss->source_volume = (float)new_volume;
            return;
        }
    impossible("obj_adjust_sound_volume: can't find %s", xname(obj));
}

void
begin_sound(obj, already_making_noise)
struct obj* obj;
boolean already_making_noise;
{
    long turns = 0;
    boolean do_timer = TRUE;

    obj->makingsound = 1;
    do_timer = FALSE;
    turns = 0;

    if (do_timer)
    {
        if (start_timer(turns, TIMER_OBJECT, MAKE_SOUND_OBJECT, obj_to_any(obj)))
        {
            obj->makingsound = 1;
            obj->age -= turns; /* Needs own timer, otherwise possible conflict with light sources */
            if (carried(obj) && !already_making_noise)
                update_inventory();
        }
        else
        {
            obj->makingsound = 0;
        }
    }
    else
    {
        if (carried(obj) && !already_making_noise)
            update_inventory();
    }

    if (obj->makingsound && !already_making_noise)
    {
        xchar x, y;
        enum object_soundset_types objsoundset = objects[obj->otyp].oc_soundset;
        enum ghsound_types ghsound = GHSOUND_NONE; // object_soundsets[objsoundset].sounds[OBJECT_SOUND_TYPE_AMBIENT].ghsound;
        float volume = 0.0f; // object_soundsets[objsoundset].sounds[OBJECT_SOUND_TYPE_AMBIENT].volume;
        set_simple_object_sound_id_and_volume(objsoundset, OBJECT_SOUND_TYPE_AMBIENT, &ghsound, &volume);
        enum soundsource_ambient_subtypes subtype = object_soundsets[objsoundset].ambient_subtype;

        if (get_obj_location(obj, &x, &y, CONTAINED_TOO | BURIED_TOO))
            new_sound_source(x, y, ghsound, (double)volume, SOUNDSOURCE_OBJECT, subtype, obj_to_any(obj));
        else
            impossible("begin_sound: can't get obj position");
    }
}

/*
 * Stop a burn timeout on the given object if timer attached.  Darken
 * light source.
 */
void
end_sound(obj, timer_attached)
struct obj* obj;
boolean timer_attached;
{
    if (!obj->makingsound)
    {
        impossible("end_sound: obj %s not making sound", xname(obj));
        return;
    }

    //if (1) //obj->otyp == MAGIC_LAMP || obj->otyp == MAGIC_CANDLE || artifact_light(obj) || obj_shines_magical_light(obj))
    timer_attached = FALSE;

    if (!timer_attached) {
        /* [DS] Cleanup explicitly, since timer cleanup won't happen */
        del_sound_source(SOUNDSOURCE_OBJECT, obj_to_any(obj));
        obj->makingsound = 0;
        if (obj->where == OBJ_INVENT)
            update_inventory();
    }
    else if (!stop_timer(MAKE_SOUND_OBJECT, obj_to_any(obj)))
        impossible("end_sound: obj %s not timed!", xname(obj));
}

boolean
obj_has_sound_source(obj)
struct obj* obj;
{
    return (obj->makingsound == TRUE);
}

enum ghsound_types
    obj_ambient_sound(obj)
    struct obj* obj;
{
    if (!obj || objects[obj->otyp].oc_soundset == OBJECT_SOUNDSET_NONE)
        return GHSOUND_NONE;

    enum object_soundset_types objsoundset = objects[obj->otyp].oc_soundset;
    enum ghsound_types ghsound = GHSOUND_NONE;
    float volume = 0.0f;
    set_simple_object_sound_id_and_volume(objsoundset, OBJECT_SOUND_TYPE_AMBIENT, &ghsound, &volume);

    return ghsound; // object_soundsets[objects[obj->otyp].oc_soundset].sounds[OBJECT_SOUND_TYPE_AMBIENT].ghsound;
}

double
obj_ambient_sound_volume(obj)
struct obj* obj;
{
    if (!obj || objects[obj->otyp].oc_soundset == OBJECT_SOUNDSET_NONE)
        return 0.0;

    enum object_soundset_types objsoundset = objects[obj->otyp].oc_soundset;
    enum ghsound_types ghsound = GHSOUND_NONE;
    float volume = 0.0f;
    set_simple_object_sound_id_and_volume(objsoundset, OBJECT_SOUND_TYPE_AMBIENT, &ghsound, &volume);

    return (double)volume; // object_soundsets[objects[obj->otyp].oc_soundset].sounds[OBJECT_SOUND_TYPE_AMBIENT].volume;
}

enum ghsound_types
get_location_ambient_sound_type(x, y, volume_ptr, subtype_ptr)
xchar x, y;
double* volume_ptr;
enum soundsource_ambient_subtypes* subtype_ptr;
{
    if (!isok(x, y))
        return GHSOUND_NONE;

    struct rm* lev = &levl[x][y];

    enum location_soundset_types lsoundset = location_type_definitions[lev->typ].soundset;
    if (lsoundset == LOCATION_SOUNDSET_NONE)
        return GHSOUND_NONE;

    enum ghsound_types ghsound = GHSOUND_NONE;
    float volume = 0.0f;
    set_simple_location_sound_id_and_volume(lsoundset, LOCATION_SOUND_TYPE_AMBIENT, &ghsound, &volume);


    if (volume_ptr)
        *volume_ptr = (double)volume; // location_soundsets[lsoundset].sounds[LOCATION_SOUND_TYPE_AMBIENT].volume;

    if (subtype_ptr)
        *subtype_ptr = location_soundsets[lsoundset].ambient_subtype;

    return ghsound; // location_soundsets[lsoundset].sounds[LOCATION_SOUND_TYPE_AMBIENT].ghsound;
}

enum ghsound_types
get_dungeon_music(dnum)
int dnum;
{
    enum ghsound_types res = GHSOUND_NONE;

    if (dnum == mines_dnum)
        res = GHSOUND_GNOMISH_MINES_MUSIC_NORMAL;
    else if (dnum == sokoban_dnum)
        res = GHSOUND_SOKOBAN_MUSIC_NORMAL;
    else if (dnum == quest_dnum)
        res = GHSOUND_QUEST_MUSIC_NORMAL;
    else if (dnum == tower_dnum)
        res = GHSOUND_VLAD_TOWER_MUSIC_NORMAL;
    else if (dnum == modron_dnum)
        res = GHSOUND_MODRON_MUSIC_NORMAL;
    else if (dnum == bovine_dnum)
        res = GHSOUND_BOVINE_MUSIC_NORMAL;
    else if (dnum == large_circular_dgn_dnum)
        res = GHSOUND_LARGE_CIRCULAR_DUNGEON_MUSIC_NORMAL;
    else if (Inhell)
        res = GHSOUND_GEHENNOM_MUSIC_NORMAL;
    else if (In_endgame(&u.uz))
        res = GHSOUND_ENDGAME_MUSIC_NORMAL;
    else
        res = GHSOUND_DUNGEON_NORMAL_MUSIC_NORMAL;

    return res;
}

enum ghsound_types
get_level_music(dlvl)
struct d_level* dlvl;
{
    if (!dlvl)
        return GHSOUND_NONE;

    int dnum = dlvl->dnum;

    if (Is_valley(dlvl))
        return GHSOUND_GEHENNOM_MUSIC_VALLEY;
    else if (Is_sanctum(dlvl))
        return GHSOUND_GEHENNOM_MUSIC_SANCTUM;
    else if (Is_stronghold(dlvl))
        return GHSOUND_DUNGEON_NORMAL_MUSIC_CASTLE;
    else if (Is_medusa_level(dlvl))
        return GHSOUND_DUNGEON_NORMAL_MUSIC_MEDUSA;
    else if (Is_dispater_level(dlvl))
        return GHSOUND_GEHENNOM_MUSIC_NORMAL;
    else if (Is_baal_level(dlvl))
        return GHSOUND_GEHENNOM_MUSIC_NORMAL;
    else if (Is_yeenaghu_level(dlvl))
        return GHSOUND_GEHENNOM_MUSIC_NORMAL;
    else if (Is_tarrasque_level(dlvl))
        return GHSOUND_GEHENNOM_MUSIC_NORMAL;
    else if (Is_orcus_level(dlvl))
        return GHSOUND_GEHENNOM_MUSIC_VALLEY;
    else if (Is_minetown_level(dlvl))
        return GHSOUND_GNOMISH_MINES_MUSIC_TOWN;
    else if (Is_earthlevel(dlvl))
        return GHSOUND_ENDGAME_MUSIC_EARTH;
    else if (Is_airlevel(dlvl))
        return GHSOUND_ENDGAME_MUSIC_AIR;
    else if (Is_waterlevel(dlvl))
        return GHSOUND_ENDGAME_MUSIC_WATER;
    else if (Is_firelevel(dlvl))
        return GHSOUND_ENDGAME_MUSIC_FIRE;
    else if (Is_astralevel(dlvl))
        return GHSOUND_ENDGAME_MUSIC_ASTRAL;
    else
        return get_dungeon_music(dnum);
}

enum ghsound_types
get_room_music(room)
struct mkroom* room;
{
    enum roomtype_types rtype = room->orig_rtype;
    enum ghsound_types res = get_level_music(&u.uz);

    if (rtype >= SHOPBASE)
    {
        if (room->resident && room->resident->isshk && room->resident->mextra && ESHK(room->resident))
        {
            if (is_peaceful(room->resident))
            {
                switch (rtype)
                {
                case SHOPBASE:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case ARMORSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case SCROLLSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case POTIONSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case WEAPONSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case FOODSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case RINGSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case WANDSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case TOOLSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case BOOKSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case REAGENTSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case MODRONSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case FODDERSHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                case CANDLESHOP:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_NORMAL;
                    break;
                default:
                    break;
                }
            }
            else
            {
                res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_SHOPKEEPER_ANGRY;
            }
        }
        else
        {
            res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_SHOPKEEPER_DEAD;
        }
    }
    else
    {
        switch (rtype)
        {
        case OROOM:
            break;
        case COURT:
            break;
        case SWAMP:
            break;
        case VAULT:
            break;
        case BEEHIVE:
            break;
        case DRAGONLAIR:
            break;
        case LIBRARY:
            break;
        case GARDEN:
            break;
        case MORGUE:
            break;
        case BARRACKS:
            break;
        case ARMORY:
            break;
        case ZOO:
            break;
        case DELPHI:
            res = GHSOUND_DUNGEON_NORMAL_MUSIC_ORACLE;
            break;
        case TEMPLE:
            res = GHSOUND_DUNGEON_NORMAL_MUSIC_TEMPLE;
            break;
        case SMITHY:
            if (Inhell)
                res = GHSOUND_DUNGEON_NORMAL_MUSIC_NPC_NORMAL;
            else
                res = GHSOUND_DUNGEON_NORMAL_MUSIC_NPC_NORMAL;
            break;
        case NPCROOM:
            if (Inhell)
            {
                switch (room->rsubtype)
                {
                case NPC_GEOLOGIST:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_NPC_NORMAL;
                    break;
                case NPC_HERMIT:
                    break;
                default:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_NPC_NORMAL;
                    break;
                }
            }
            else if(Is_minetown_level(&u.uz))
            {
                switch (room->rsubtype)
                {
                case NPC_GEOLOGIST:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_NPC_NORMAL;
                    break;
                case NPC_HERMIT:
                    break;
                default:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_NPC_NORMAL;
                    break;
                }
            }
            else
            {
                switch (room->rsubtype)
                {
                case NPC_HERMIT:
                    break;
                default:
                    res = GHSOUND_DUNGEON_NORMAL_MUSIC_NPC_NORMAL;
                    break;
                }
            }
            break;
        case LEPREHALL:
            break;
        case COCKNEST:
            break;
        case ANTHOLE:
            break;
        case DESERTEDSHOP:
            res = GHSOUND_DUNGEON_NORMAL_MUSIC_SHOP_DESERTED;
            break;
        default:
            break;
        }
    }

    return res;
}


enum ghsound_types
    get_level_ambient_sounds(dlvl)
    struct d_level* dlvl;
{
    if (!dlvl)
        return GHSOUND_NONE;

    if (Is_airlevel(dlvl))
        return GHSOUND_AIR_LEVEL_AMBIENT;
    else if (Is_waterlevel(dlvl))
        return GHSOUND_WATER_LEVEL_AMBIENT;
    else if (Is_valley(dlvl))
        return GHSOUND_GEHENNOM_VALLEY_AMBIENT;
    else
        return GHSOUND_NONE;
}

enum ghsound_types
    get_room_ambient_sounds(room)
    struct mkroom* room;
{
    enum roomtype_types rtype = room->orig_rtype;
    enum ghsound_types res = get_level_ambient_sounds(&u.uz);

    switch (rtype)
    {
    case OROOM:
        break;
    case COURT:
        break;
    case SWAMP:
        break;
    case VAULT:
        break;
    case BEEHIVE:
        break;
    case DRAGONLAIR:
        break;
    case LIBRARY:
        break;
    case GARDEN:
        res = GHSOUND_GARDEN;
        break;
    case MORGUE:
        res = GHSOUND_MORGUE;
        break;
    case BARRACKS:
        break;
    case ARMORY:
        break;
    case ZOO:
        break;
    case DELPHI:
        break;
    case TEMPLE:
        break;
    case SMITHY:
        break;
    case LEPREHALL:
        break;
    case COCKNEST:
        break;
    case ANTHOLE:
        break;
    case DESERTEDSHOP:
        res = GHSOUND_MORGUE;
        break;
    case ARMORSHOP:
        break;
    case SHOPBASE:
        break;
    case SCROLLSHOP:
        break;
    case POTIONSHOP:
        break;
    case WEAPONSHOP:
        break;
    case FOODSHOP:
        break;
    case RINGSHOP:
        break;
    case WANDSHOP:
        break;
    case TOOLSHOP:
        break;
    case BOOKSHOP:
        break;
    case REAGENTSHOP:
        break;
    case MODRONSHOP:
        break;
    case FODDERSHOP:
        break;
    case CANDLESHOP:
        break;
    default:
        break;
    }

    return res;
}

enum ghsound_types
    get_environment_ambient_sounds()
{
    enum ghsound_types res = GHSOUND_NONE;

    if (Deaf)
        return GHSOUND_DEAF_AMBIENT;
    else if (u.uswallow && u.ustuck)
    {
        int soundset = u.ustuck->female ? u.ustuck->data->female_soundset : u.ustuck->data->soundset;
        return monster_soundsets[soundset].sounds[MONSTER_SOUND_TYPE_SWALLOW_AMBIENT].ghsound;
    }
    else if (Underwater)
        return GHSOUND_UNDERWATER_AMBIENT;
    else
        return GHSOUND_NONE;

    return res;
}

void
get_god_indices(godindex_ptr, titleandgodindex_ptr)
int* godindex_ptr;
int* titleandgodindex_ptr;
{
    if (!godindex_ptr || !titleandgodindex_ptr)
        return;

    *godindex_ptr = 7;
    *titleandgodindex_ptr = 9;
    enum role_types yourrole = urole.rolenum;

    switch (yourrole)
    {
    case ROLE_ARCHAEOLOGIST:
        if (u.ualign.type == A_LAWFUL)
        {
            *godindex_ptr = 0;
            *titleandgodindex_ptr = 0;
        }
        else if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 1;
            *titleandgodindex_ptr = 1;
        }
        break;
    case ROLE_BARBARIAN:
        if (u.ualign.type == A_CHAOTIC)
        {
            *godindex_ptr = 2;
            *titleandgodindex_ptr = flags.female ? 2 : 3;
        }
        else if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 3;
            *titleandgodindex_ptr = flags.female ? 4 : 5;
        }
        break;
    case ROLE_CAVEMAN:
        if (u.ualign.type == A_LAWFUL)
        {
            *godindex_ptr = 4;
            *titleandgodindex_ptr = 6;
        }
        else if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 5;
            *titleandgodindex_ptr = 7;
        }
        break;
    case ROLE_HEALER:
        if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 8;
            *titleandgodindex_ptr = 10;
        }
        break;
    case ROLE_KNIGHT:
        if (u.ualign.type == A_LAWFUL)
        {
            *godindex_ptr = 9;
            *titleandgodindex_ptr = 11;
        }
        break;
    case ROLE_MONK:
        if (u.ualign.type == A_CHAOTIC)
        {
            *godindex_ptr = 10;
            *titleandgodindex_ptr = 12;
        }
        else if (u.ualign.type == A_LAWFUL)
        {
            *godindex_ptr = 11;
            *titleandgodindex_ptr = 13;
        }
        else if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 12;
            *titleandgodindex_ptr = 14;
        }
        break;
    case ROLE_PRIEST:
    {
        enum role_types yourpantheon = flags.pantheon;
        if (u.ualign.type == A_CHAOTIC)
        {
            switch (yourpantheon)
            {
            case ROLE_ARCHAEOLOGIST:
                *godindex_ptr = 13;
                *titleandgodindex_ptr = 15;
                break;
            case ROLE_BARBARIAN:
                *godindex_ptr = 2;
                *titleandgodindex_ptr = 16;
                break;
            case ROLE_CAVEMAN:
                *godindex_ptr = 14;
                *titleandgodindex_ptr = 17;
                break;
            case ROLE_HEALER:
                *godindex_ptr = 15;
                *titleandgodindex_ptr = 18;
                break;
            case ROLE_KNIGHT:
                *godindex_ptr = 16;
                *titleandgodindex_ptr = 19;
                break;
            case ROLE_MONK:
                *godindex_ptr = 10;
                *titleandgodindex_ptr = 20;
                break;
            case ROLE_PRIEST:
                break;
            case ROLE_ROGUE:
                *godindex_ptr = 31;
                *titleandgodindex_ptr = 22;
                break;
            case ROLE_RANGER:
                *godindex_ptr = 29;
                *titleandgodindex_ptr = 21;
                break;
            case ROLE_SAMURAI:
                *godindex_ptr = 17;
                *titleandgodindex_ptr = 23;
                break;
            case ROLE_TOURIST:
                *godindex_ptr = 18;
                *titleandgodindex_ptr = 24;
                break;
            case ROLE_VALKYRIE:
                *godindex_ptr = 19;
                *titleandgodindex_ptr = 25;
                break;
            case ROLE_WIZARD:
                *godindex_ptr = 36;
                *titleandgodindex_ptr = 26;
                break;
            case NUM_ROLES:
                break;
            default:
                break;
            }
        }
        else if (u.ualign.type == A_LAWFUL)
        {
            switch (yourpantheon)
            {
            case ROLE_ARCHAEOLOGIST:
                *godindex_ptr = 0;
                *titleandgodindex_ptr = 27;
                break;
            case ROLE_BARBARIAN:
                *godindex_ptr = 20;
                *titleandgodindex_ptr = 28;
                break;
            case ROLE_CAVEMAN:
                *godindex_ptr = 4;
                *titleandgodindex_ptr = 29;
                break;
            case ROLE_HEALER:
                *godindex_ptr = 21;
                *titleandgodindex_ptr = 30;
                break;
            case ROLE_KNIGHT:
                *godindex_ptr = 9;
                *titleandgodindex_ptr = 31;
                break;
            case ROLE_MONK:
                *godindex_ptr = 11;
                *titleandgodindex_ptr = 32;
                break;
            case ROLE_PRIEST:
                break;
            case ROLE_ROGUE:
                *godindex_ptr = 23;
                *titleandgodindex_ptr = 34;
                break;
            case ROLE_RANGER:
                *godindex_ptr = 22;
                *titleandgodindex_ptr = 33;
                break;
            case ROLE_SAMURAI:
                *godindex_ptr = 17;
                *titleandgodindex_ptr = 35;
                break;
            case ROLE_TOURIST:
                *godindex_ptr = 24;
                *titleandgodindex_ptr = 36;
                break;
            case ROLE_VALKYRIE:
                *godindex_ptr = 34;
                *titleandgodindex_ptr = 37;
                break;
            case ROLE_WIZARD:
                *godindex_ptr = 25;
                *titleandgodindex_ptr = 38;
                break;
            case NUM_ROLES:
                break;
            default:
                break;
            }
        }
        else if (u.ualign.type == A_NEUTRAL)
        {
            switch (yourpantheon)
            {
            case ROLE_ARCHAEOLOGIST:
                *godindex_ptr = 1;
                *titleandgodindex_ptr = 39;
                break;
            case ROLE_BARBARIAN:
                *godindex_ptr = 3;
                *titleandgodindex_ptr = 40;
                break;
            case ROLE_CAVEMAN:
                *godindex_ptr = 5;
                *titleandgodindex_ptr = 41;
                break;
            case ROLE_HEALER:
                *godindex_ptr = 8;
                *titleandgodindex_ptr = 42;
                break;
            case ROLE_KNIGHT:
                *godindex_ptr = 26;
                *titleandgodindex_ptr = 43;
                break;
            case ROLE_MONK:
                *godindex_ptr = 12;
                *titleandgodindex_ptr = 44;
                break;
            case ROLE_PRIEST:
                break;
            case ROLE_ROGUE:
                *godindex_ptr = 27;
                *titleandgodindex_ptr = 46;
                break;
            case ROLE_RANGER:
                *godindex_ptr = 30;
                *titleandgodindex_ptr = 45;
                break;
            case ROLE_SAMURAI:
                *godindex_ptr = 28;
                *titleandgodindex_ptr = 47;
                break;
            case ROLE_TOURIST:
                *godindex_ptr = 33;
                *titleandgodindex_ptr = 48;
                break;
            case ROLE_VALKYRIE:
                *godindex_ptr = 35;
                *titleandgodindex_ptr = 49;
                break;
            case ROLE_WIZARD:
                *godindex_ptr = 37;
                *titleandgodindex_ptr = 50;
                break;
            case NUM_ROLES:
                break;
            default:
                break;
            }
        }

        break;
    }
    case ROLE_ROGUE:
        if (u.ualign.type == A_CHAOTIC)
        {
            *godindex_ptr = 31;
            *titleandgodindex_ptr = 53;
        }
        break;
    case ROLE_RANGER:
        if (u.ualign.type == A_CHAOTIC)
        {
            *godindex_ptr = 29;
            *titleandgodindex_ptr = 51;
        }
        else if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 30;
            *titleandgodindex_ptr = 52;
        }
        break;
    case ROLE_SAMURAI:
        if (u.ualign.type == A_LAWFUL)
        {
            *godindex_ptr = 32;
            *titleandgodindex_ptr = 54;
        }
        break;
    case ROLE_TOURIST:
        if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 33;
            *titleandgodindex_ptr = 55;
        }
        break;
    case ROLE_VALKYRIE:
        if (u.ualign.type == A_LAWFUL)
        {
            *godindex_ptr = 34;
            *titleandgodindex_ptr = 56;
        }
        else if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 35;
            *titleandgodindex_ptr = 57;
        }
        break;
    case ROLE_WIZARD:
        if (u.ualign.type == A_CHAOTIC)
        {
            *godindex_ptr = 36;
            *titleandgodindex_ptr = 58;
        }
        else if (u.ualign.type == A_NEUTRAL)
        {
            *godindex_ptr = 37;
            *titleandgodindex_ptr = 59;
        }
        break;
    case NUM_ROLES:
        break;
    default:
        break;
    }
}

void
play_intro_text()
{
    int godindex = 7;
    int titleandgodindex = 9;

    get_god_indices(&godindex, &titleandgodindex);

    struct ghsound_immediate_info info = { 0 };
    info.ghsound = GHSOUND_INTRO_TEXT;
    info.volume = 1.0f;
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.parameter_names[0] = "GodIndex";
    info.parameter_values[0] = (float)godindex;
    info.parameter_names[1] = "TitleAndGodIndex";
    info.parameter_values[1] = (float)titleandgodindex;
    info.parameter_names[2] = (char*)0;

    play_immediate_ghsound(info);
}

void
play_voice_com_pager(mon, msgnum, via_pline)
struct monst* mon;
int msgnum;
boolean via_pline;
{
    /* Special cases first */
    if (msgnum >= QT_ANGELIC && msgnum <= QT_ANGELIC + QTN_ANGELIC - 1)
    {
        play_voice_monster_cuss_with_god_name(mon, msgnum - QT_ANGELIC + 1);
        return;
    }
    else if (msgnum >= QT_DEMONIC && msgnum <= QT_DEMONIC + QTN_DEMONIC - 1)
    {
        play_voice_monster_cuss(mon, msgnum - QT_DEMONIC + 1);
        return;
    }

    struct ghsound_immediate_info info = { 0 };
    info.ghsound = GHSOUND_COM_PAGER;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE; //  is_dialogue ? IMMEDIATE_SOUND_DIALOGUE : IMMEDIATE_SOUND_SFX;
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.dialogue_mid = 0;

    if (via_pline)
    {
        float volume = 1.0f;
        if (mon && isok(mon->mx, mon->my))
        {
            volume = max(0.15f, max(context.global_minimum_volume, volume * hearing_array[mon->mx][mon->my]));
        }

        info.volume = min(1.0f, volume);
    }
    else
        info.volume = 1.0f;

    info.parameter_names[0] = "RoleIndex";
    info.parameter_values[0] = (float)urole.rolenum;
    info.parameter_names[1] = "MsgIndex";
    info.parameter_values[1] = (float)msgnum;
    info.parameter_names[2] = "OriginalAlignmentIndex";
    info.parameter_values[2] = (float)u.ualignbase[A_ORIGINAL];
    info.parameter_names[3] = "OriginalGodGenderIndex";
    info.parameter_values[3] = (float)align_gfemale(u.ualignbase[A_ORIGINAL]);
    info.parameter_names[4] = (char*)0;

    play_immediate_ghsound(info);
}

void
play_voice_quest_pager(mon, msgnum, via_pline)
struct monst* mon;
int msgnum;
boolean via_pline;
{
    struct ghsound_immediate_info info = { 0 };
    info.ghsound = GHSOUND_QUEST_PAGER;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE; //  is_dialogue ? IMMEDIATE_SOUND_DIALOGUE : IMMEDIATE_SOUND_SFX;
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.dialogue_mid = mon ? mon->m_id : 0;

    info.parameter_names[0] = "RoleIndex";
    info.parameter_values[0] = (float)urole.rolenum;
    info.parameter_names[1] = "MsgIndex";
    info.parameter_values[1] = (float)msgnum;
    info.parameter_names[2] = "AlignmentIndex";
    info.parameter_values[2] = (float)u.ualign.type;
    info.parameter_names[3] = "OriginalAlignmentIndex";
    info.parameter_values[3] = (float)u.ualignbase[A_ORIGINAL];
    info.parameter_names[4] = "TitleIndex";
    info.parameter_values[4] = (float)xlev_to_rank(u.ulevel);
    info.parameter_names[5] = "GenderIndex";
    info.parameter_values[5] = mon ? (float)mon->female : 0;
    info.parameter_names[6] = "YourGenderIndex";
    info.parameter_values[6] = (float)flags.female;
    info.parameter_names[7] = "YouBlind";
    info.parameter_values[7] = (float)Blind;
    info.parameter_names[8] = "OriginalGodGenderIndex";
    info.parameter_values[8] = (float)align_gfemale(u.ualignbase[A_ORIGINAL]);
    info.parameter_names[9] = (char*)0;

    if (via_pline)
    {
        float volume = 1.0f;
        if (mon && isok(mon->mx, mon->my))
        {
            volume = max(0.15f, max(context.global_minimum_volume, volume * hearing_array[mon->mx][mon->my]));
        }

        info.volume = min(1.0f, volume);
    }
    else
        info.volume = 1.0f;

    play_immediate_ghsound(info);
}

void
stop_all_immediate_sounds()
{
    struct stop_all_info info = { 0 };
    info.stop_flags = STOP_SOUNDS_FLAGS_IMMEDIATE_ALL;
    stop_all_sounds(info);
}

void
stop_all_long_immediate_sounds()
{
    struct stop_all_info info = { 0 };
    info.stop_flags = STOP_SOUNDS_FLAGS_IMMEDIATE_LONG;
    stop_all_sounds(info);
}

void
stop_all_dialogue_of_mon(mon)
struct monst* mon;
{
    if (!mon)
        return;

    struct stop_all_info info = { 0 };
    info.stop_flags = STOP_SOUNDS_FLAGS_IMMEDIATE_LONG | STOP_SOUNDS_FLAGS_ONLY_DIALOGUE_MID;
    info.dialogue_mid = mon->m_id;
    stop_all_sounds(info);
}

void
play_voice_shopkeeper_welcome(shkp, rt)
struct monst* shkp;
int rt;
{
    if (!shkp || !shkp->mextra || !ESHK(shkp) || Deaf)
        return;

    int shoptype = (rt - SHOPBASE);
    enum role_types yourrole = urole.rolenum;
    struct eshk* eshkp = ESHK(shkp);

    struct ghsound_immediate_info info = { 0 };
    if (eshkp->visitct == 0)
    {
        info.ghsound = is_undead(shkp->data) || is_demon(shkp->data) ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WELCOME : shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WELCOME : GHSOUND_VOICE_SHOPKEEPER_MALE_WELCOME;
        info.parameter_names[0] = "RoleIndex";
        info.parameter_values[0] = (float)yourrole;
        info.parameter_names[1] = "ShopType";
        info.parameter_values[1] = (float)shoptype;
        info.parameter_names[2] = (char*)0;
    }
    else
    {
        info.ghsound = is_undead(shkp->data) || is_demon(shkp->data) ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WELCOME_BACK : shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WELCOME_BACK : GHSOUND_VOICE_SHOPKEEPER_MALE_WELCOME_BACK;
        info.parameter_names[0] = "RoleIndex";
        info.parameter_values[0] = (float)yourrole;
        info.parameter_names[1] = (char*)0;
    }

    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}


void
play_voice_shopkeeper_simple_line(shkp, line_idx)
struct monst* shkp;
enum shopkeeper_lines line_idx;
{
    if (!shkp || (!has_eshk(shkp) && line_idx != SHOPKEEPER_LINE_SEEN_UNTENDED_SHOPS) || line_idx == SHOPKEEPER_LINE_NONE || Deaf)
        return;

    enum role_types yourrole = urole.rolenum;
    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);
    float special_minimum_volume = -1.0f;

    struct ghsound_immediate_info info = { 0 };

    switch (line_idx)
    {
    case SHOPKEEPER_LINE_NONE:
        break;
    case SHOPKEEPER_LINE_THANK_YOU_SHOPPING_IN_MY_STORE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_THANK_YOU_SHOPPING_IN_MY_STORE :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_THANK_YOU_SHOPPING_IN_MY_STORE :
            GHSOUND_VOICE_SHOPKEEPER_MALE_THANK_YOU_SHOPPING_IN_MY_STORE;
        break;
    case SHOPKEEPER_LINE_INVISIBLE_CUSTOMERS_NOT_WELCOME:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_INVISIBLE_CUSTOMERS_NOT_WELCOME :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_INVISIBLE_CUSTOMERS_NOT_WELCOME :
            GHSOUND_VOICE_SHOPKEEPER_MALE_INVISIBLE_CUSTOMERS_NOT_WELCOME;
        break;
    case SHOPKEEPER_LINE_DROP_THAT_NOW:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_DROP_THAT_NOW :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_DROP_THAT_NOW :
            GHSOUND_VOICE_SHOPKEEPER_MALE_DROP_THAT_NOW;
        break;
    case SHOPKEEPER_LINE_CURSING_SHOPLIFTERS:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_CURSING_SHOPLIFTERS :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_CURSING_SHOPLIFTERS :
            GHSOUND_VOICE_SHOPKEEPER_MALE_CURSING_SHOPLIFTERS;
        break;
    case SHOPKEEPER_LINE_STAY_AWAY_FROM_THOSE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_STAY_AWAY_FROM_THOSE :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_STAY_AWAY_FROM_THOSE :
            GHSOUND_VOICE_SHOPKEEPER_MALE_STAY_AWAY_FROM_THOSE;
        break;
    case SHOPKEEPER_LINE_STAY_AWAY_FROM_THAT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_STAY_AWAY_FROM_THAT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_STAY_AWAY_FROM_THAT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_STAY_AWAY_FROM_THAT;
        break;
    case SHOPKEEPER_LINE_ID_HANG_ONTO_THAT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_ID_HANG_ONTO_THAT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_ID_HANG_ONTO_THAT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_ID_HANG_ONTO_THAT;
        break;
    case SHOPKEEPER_LINE_I_WONT_STOCK_THAT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_I_WONT_STOCK_THAT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_I_WONT_STOCK_THAT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_I_WONT_STOCK_THAT;
        break;
    case SHOPKEEPER_LINE_THANK_YOU_SCUM:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_THANK_YOU_SCUM :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_THANK_YOU_SCUM :
            GHSOUND_VOICE_SHOPKEEPER_MALE_THANK_YOU_SCUM;
        break;
    case SHOPKEEPER_LINE_OUT_OF_MY_WAY_SCUM:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_OUT_OF_MY_WAY_SCUM :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_OUT_OF_MY_WAY_SCUM :
            GHSOUND_VOICE_SHOPKEEPER_MALE_OUT_OF_MY_WAY_SCUM;
        break;
    case SHOPKEEPER_LINE_GET_YOUR_JUNK_OUT_OF_MY_WALL:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_GET_YOUR_JUNK_OUT_OF_MY_WALL :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_GET_YOUR_JUNK_OUT_OF_MY_WALL :
            GHSOUND_VOICE_SHOPKEEPER_MALE_GET_YOUR_JUNK_OUT_OF_MY_WALL;
        break;
    case SHOPKEEPER_LINE_I_WAS_LOOKING_FOR_SOMEONE_ELSE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_I_WAS_LOOKING_FOR_SOMEONE_ELSE :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_I_WAS_LOOKING_FOR_SOMEONE_ELSE :
            GHSOUND_VOICE_SHOPKEEPER_MALE_I_WAS_LOOKING_FOR_SOMEONE_ELSE;
        info.parameter_names[0] = "RoleIndex";
        info.parameter_values[0] = (float)yourrole;
        info.parameter_names[1] = (char*)0;
        break;
    case SHOPKEEPER_LINE_DIDNT_YOU_FORGET_TO_PAY:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_DIDNT_YOU_FORGET_TO_PAY :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_DIDNT_YOU_FORGET_TO_PAY :
            GHSOUND_VOICE_SHOPKEEPER_MALE_DIDNT_YOU_FORGET_TO_PAY;
        info.parameter_names[0] = "RoleIndex";
        info.parameter_values[0] = (float)yourrole;
        info.parameter_names[1] = (char*)0;
        break;
    case SHOPKEEPER_LINE_ADVENTURER_DIDNT_YOU_FORGET_TO_PAY:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_ADVENTURER_DIDNT_YOU_FORGET_TO_PAY :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_ADVENTURER_DIDNT_YOU_FORGET_TO_PAY :
            GHSOUND_VOICE_SHOPKEEPER_MALE_ADVENTURER_DIDNT_YOU_FORGET_TO_PAY;
        break;
    case SHOPKEEPER_LINE_BE_CAREFUL_SIR_FALL_THROUGH_THE_FLOOR:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_BE_CAREFUL_SIR_FALL_THROUGH_THE_FLOOR :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_BE_CAREFUL_SIR_FALL_THROUGH_THE_FLOOR :
            GHSOUND_VOICE_SHOPKEEPER_MALE_BE_CAREFUL_SIR_FALL_THROUGH_THE_FLOOR;
        break;
    case SHOPKEEPER_LINE_BE_CAREFUL_MADAM_FALL_THROUGH_THE_FLOOR:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_BE_CAREFUL_MADAM_FALL_THROUGH_THE_FLOOR :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_BE_CAREFUL_MADAM_FALL_THROUGH_THE_FLOOR :
            GHSOUND_VOICE_SHOPKEEPER_MALE_BE_CAREFUL_MADAM_FALL_THROUGH_THE_FLOOR;
        break;
    case SHOPKEEPER_LINE_SIR_DO_NOT_DAMAGE_FLOOR:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_SIR_DO_NOT_DAMAGE_FLOOR :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_SIR_DO_NOT_DAMAGE_FLOOR :
            GHSOUND_VOICE_SHOPKEEPER_MALE_SIR_DO_NOT_DAMAGE_FLOOR;
        break;
    case SHOPKEEPER_LINE_MADAM_DO_NOT_DAMAGE_FLOOR:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_MADAM_DO_NOT_DAMAGE_FLOOR :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_MADAM_DO_NOT_DAMAGE_FLOOR :
            GHSOUND_VOICE_SHOPKEEPER_MALE_MADAM_DO_NOT_DAMAGE_FLOOR;
        break;
    case SHOPKEEPER_LINE_PLEASE_PAY_BEFORE_LEAVING:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PLEASE_PAY_BEFORE_LEAVING :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PLEASE_PAY_BEFORE_LEAVING :
            GHSOUND_VOICE_SHOPKEEPER_MALE_PLEASE_PAY_BEFORE_LEAVING;
        break;
    case SHOPKEEPER_LINE_DONT_YOU_LEAVE_BEFORE_PAYING:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_DONT_YOU_LEAVE_BEFORE_PAYING :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_DONT_YOU_LEAVE_BEFORE_PAYING :
            GHSOUND_VOICE_SHOPKEEPER_MALE_DONT_YOU_LEAVE_BEFORE_PAYING;
        break;
    case SHOPKEEPER_LINE_YOU_DARE_TO_RETURN_TO_MY_STORE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOU_DARE_TO_RETURN_TO_MY_STORE :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOU_DARE_TO_RETURN_TO_MY_STORE :
            GHSOUND_VOICE_SHOPKEEPER_MALE_YOU_DARE_TO_RETURN_TO_MY_STORE;
        break;
    case SHOPKEEPER_LINE_OH_YES_YOULL_PAY:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_OH_YES_YOULL_PAY :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_OH_YES_YOULL_PAY :
            GHSOUND_VOICE_SHOPKEEPER_MALE_OH_YES_YOULL_PAY;
        break;
    case SHOPKEEPER_LINE_DIDNT_YOU_FORGET_TO_PAY2:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_DIDNT_YOU_FORGET_TO_PAY2 :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_DIDNT_YOU_FORGET_TO_PAY2 :
            GHSOUND_VOICE_SHOPKEEPER_MALE_DIDNT_YOU_FORGET_TO_PAY2;
        break;
    case SHOPKEEPER_LINE_WHOA:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WHOA :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WHOA :
            GHSOUND_VOICE_SHOPKEEPER_MALE_WHOA;
        break;
    case SHOPKEEPER_LINE_WATCH_IT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WATCH_IT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WATCH_IT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_WATCH_IT;
        break;
    case SHOPKEEPER_LINE_HEY:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_HEY :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_HEY :
            GHSOUND_VOICE_SHOPKEEPER_MALE_HEY;
        break;
    case SHOPKEEPER_LINE_AHEM:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_AHEM :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_AHEM :
            GHSOUND_VOICE_SHOPKEEPER_MALE_AHEM;
        break;
    case SHOPKEEPER_LINE_YOU_OWE_ME_SOME_GOLD:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOU_OWE_ME_SOME_GOLD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOU_OWE_ME_SOME_GOLD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_YOU_OWE_ME_SOME_GOLD;
        break;
    case SHOPKEEPER_LINE_YOU_OWE_ME_SOME_ADDITIONAL_GOLD:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOU_OWE_ME_SOME_ADDITIONAL_GOLD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOU_OWE_ME_SOME_ADDITIONAL_GOLD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_YOU_OWE_ME_SOME_ADDITIONAL_GOLD;
        break;
    case SHOPKEEPER_LINE_THAT_WILL_COST_YOU_SOME_GOLD:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_THAT_WILL_COST_YOU_SOME_GOLD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_THAT_WILL_COST_YOU_SOME_GOLD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_THAT_WILL_COST_YOU_SOME_GOLD;
        break;
    case SHOPKEEPER_LINE_EMPTYING_THAT_WILL_COST_YOU_SOME_GOLD:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_EMPTYING_THAT_WILL_COST_YOU_SOME_GOLD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_EMPTYING_THAT_WILL_COST_YOU_SOME_GOLD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_EMPTYING_THAT_WILL_COST_YOU_SOME_GOLD;
        break;
    case SHOPKEEPER_LINE_USAGE_FEE_SOME_GOLD:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_USAGE_FEE_SOME_GOLD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_USAGE_FEE_SOME_GOLD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_USAGE_FEE_SOME_GOLD;
        break;
    case SHOPKEEPER_LINE_WELCOME_TO_MY_STORE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WELCOME_TO_MY_STORE_SHORT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WELCOME_TO_MY_STORE_SHORT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_WELCOME_TO_MY_STORE_SHORT;
        info.parameter_names[0] = "ShopType";
        info.parameter_values[0] = max(0, (float)(ESHK(shkp)->shoptype - SHOPBASE));
        info.parameter_names[1] = (char*)0;
        break;
    case SHOPKEEPER_LINE_WELCOME_BACK_TO_MY_STORE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WELCOME_BACK_TO_MY_STORE_SHORT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WELCOME_BACK_TO_MY_STORE_SHORT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_WELCOME_BACK_TO_MY_STORE_SHORT;
        break;
    case SHOPKEEPER_LINE_YOU_DID_LOT_OF_DAMAGE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOU_DID_LOT_OF_DAMAGE_SIMPLE :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOU_DID_LOT_OF_DAMAGE_SIMPLE :
            GHSOUND_VOICE_SHOPKEEPER_MALE_YOU_DID_LOT_OF_DAMAGE_SIMPLE;
        break;
    case SHOPKEEPER_LINE_CLOSE_SESAME:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_CLOSE_SESAME :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_CLOSE_SESAME :
            GHSOUND_VOICE_SHOPKEEPER_MALE_CLOSE_SESAME;
        special_minimum_volume = 0.0f;
        break;
    case SHOPKEEPER_LINE_YOU_NEED_TO_PAY_LOT_OF_GOLD:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOU_NEED_TO_PAY_LOT_OF_GOLD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOU_NEED_TO_PAY_LOT_OF_GOLD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_YOU_NEED_TO_PAY_LOT_OF_GOLD;
        break;
    case SHOPKEEPER_LINE_DAMN_IT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_DAMN_IT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_DAMN_IT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_DAMN_IT;
        break;
    case SHOPKEEPER_LINE_CURSED_THIEF:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_CURSED_THIEF :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_CURSED_THIEF :
            GHSOUND_VOICE_SHOPKEEPER_MALE_CURSED_THIEF;
        break;
    case SHOPKEEPER_LINE_CURSED_VANDAL:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_CURSED_VANDAL :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_CURSED_VANDAL :
            GHSOUND_VOICE_SHOPKEEPER_MALE_CURSED_VANDAL;
        break;
    case SHOPKEEPER_LINE_YOU_THIEF:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOU_THIEF :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOU_THIEF :
            GHSOUND_VOICE_SHOPKEEPER_MALE_YOU_THIEF;
        break;
    case SHOPKEEPER_LINE_THIEF:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_THIEF :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_THIEF :
            GHSOUND_VOICE_SHOPKEEPER_MALE_THIEF;
        break;
    case SHOPKEEPER_LINE_YOU_DARE_TO_RETURN:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOU_DARE_TO_RETURN :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOU_DARE_TO_RETURN :
            GHSOUND_VOICE_SHOPKEEPER_MALE_YOU_DARE_TO_RETURN;
        break;
    case SHOPKEEPER_LINE_PAY_GOLD_IN_COMPENSATION:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PAY_GOLD_IN_COMPENSATION :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PAY_GOLD_IN_COMPENSATION :
            GHSOUND_VOICE_SHOPKEEPER_MALE_PAY_GOLD_IN_COMPENSATION;
        break;
    case SHOPKEEPER_LINE_DISLIKE_NON_PAYING_CUSTOMERS:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_DISLIKE_NON_PAYING_CUSTOMERS :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_DISLIKE_NON_PAYING_CUSTOMERS :
            GHSOUND_VOICE_SHOPKEEPER_MALE_DISLIKE_NON_PAYING_CUSTOMERS;
        break;
    case SHOPKEEPER_LINE_DISLIKE_RUDE_CUSTOMERS:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_DISLIKE_RUDE_CUSTOMERS :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_DISLIKE_RUDE_CUSTOMERS :
            GHSOUND_VOICE_SHOPKEEPER_MALE_DISLIKE_RUDE_CUSTOMERS;
        break;
    case SHOPKEEPER_LINE_WAS_ROBBED:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WAS_ROBBED :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WAS_ROBBED :
            GHSOUND_VOICE_SHOPKEEPER_MALE_WAS_ROBBED;
        break;
    case SHOPKEEPER_LINE_OWE_SOME_GOLD:
        if (flags.female)
            info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_MADAM_OWE_SOME_GOLD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_MADAM_OWE_SOME_GOLD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_MADAM_OWE_SOME_GOLD;
        else
            info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_SIR_OWE_SOME_GOLD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_SIR_OWE_SOME_GOLD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_SIR_OWE_SOME_GOLD;
        break;
    case SHOPKEEPER_LINE_USING_OUTSTANDING_CREDIT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_USING_OUTSTANDING_CREDIT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_USING_OUTSTANDING_CREDIT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_USING_OUTSTANDING_CREDIT;
        break;
    case SHOPKEEPER_LINE_BUSINESS_IS_BAD:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_BUSINESS_IS_BAD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_BUSINESS_IS_BAD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_BUSINESS_IS_BAD;
        break;
    case SHOPKEEPER_LINE_BUSINESS_IS_GOOD:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_BUSINESS_IS_GOOD :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_BUSINESS_IS_GOOD :
            GHSOUND_VOICE_SHOPKEEPER_MALE_BUSINESS_IS_GOOD;
        break;
    case SHOPKEEPER_LINE_PROBLEM_WITH_SHOPLIFTERS:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PROBLEM_WITH_SHOPLIFTERS :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PROBLEM_WITH_SHOPLIFTERS :
            GHSOUND_VOICE_SHOPKEEPER_MALE_PROBLEM_WITH_SHOPLIFTERS;
        break;
    case SHOPKEEPER_LINE_IM_THE_SHOPKEEPER:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_IM_THE_SHOPKEEPER :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_IM_THE_SHOPKEEPER :
            GHSOUND_VOICE_SHOPKEEPER_MALE_IM_THE_SHOPKEEPER;
        break;
    case SHOPKEEPER_LINE_I_RUN_THIS_STORE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_I_RUN_THIS_STORE :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_I_RUN_THIS_STORE :
            GHSOUND_VOICE_SHOPKEEPER_MALE_I_RUN_THIS_STORE;
        info.parameter_names[0] = "ShopType";
        info.parameter_values[0] = max(0, (float)(ESHK(shkp)->shoptype - SHOPBASE));
        info.parameter_names[1] = (char*)0;
        break;
    case SHOPKEEPER_LINE_THATS_A_DEAL:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_THATS_A_DEAL :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_THATS_A_DEAL :
            GHSOUND_VOICE_SHOPKEEPER_MALE_THATS_A_DEAL;
        break;
    case SHOPKEEPER_LINE_ON_SECOND_THOUGHT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_ON_SECOND_THOUGHT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_ON_SECOND_THOUGHT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_ON_SECOND_THOUGHT;
        break;
    case SHOPKEEPER_LINE_YOUR_BILL_COMES_TO_THIS_AMOUNT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOUR_BILL_COMES_TO_THIS_AMOUNT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOUR_BILL_COMES_TO_THIS_AMOUNT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_YOUR_BILL_COMES_TO_THIS_AMOUNT;
        break;
    case SHOPKEEPER_LINE_BURN_IT_BOUGHT_IT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_BURN_IT_BOUGHT_IT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_BURN_IT_BOUGHT_IT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_BURN_IT_BOUGHT_IT;
        break;
    case SHOPKEEPER_LINE_BURN_THEM_BOUGHT_THEM:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_BURN_THEM_BOUGHT_THEM :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_BURN_THEM_BOUGHT_THEM :
            GHSOUND_VOICE_SHOPKEEPER_MALE_BURN_THEM_BOUGHT_THEM;
        break;
    case SHOPKEEPER_LINE_USE_IT_BOUGHT_IT:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_USE_IT_BOUGHT_IT :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_USE_IT_BOUGHT_IT :
            GHSOUND_VOICE_SHOPKEEPER_MALE_USE_IT_BOUGHT_IT;
        break;
    case SHOPKEEPER_LINE_USE_THEM_BOUGHT_THEM:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_USE_THEM_BOUGHT_THEM :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_USE_THEM_BOUGHT_THEM :
            GHSOUND_VOICE_SHOPKEEPER_MALE_USE_THEM_BOUGHT_THEM;
        break;
    case SHOPKEEPER_LINE_SEEN_UNTENDED_SHOPS:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_SEEN_UNTENDED_SHOPS :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_SEEN_UNTENDED_SHOPS :
            GHSOUND_VOICE_SHOPKEEPER_MALE_SEEN_UNTENDED_SHOPS;
        break;
    case SHOPKEEPER_LINE_YOU_TIN_IT_YOU_BOUGHT_IT:
    case SHOPKEEPER_LINE_IN_ADDITION_TO_COST_OF_ITEM_ITSELF:
    case SHOPKEEPER_LINE_IN_ADDITION_TO_COST_OF_ITEMS_THEMSELVES:
    case SHOPKEEPER_LINE_IN_ADDITION_TO_COST_OF_POTION:
    case SHOPKEEPER_LINE_NO_CHECKS_NO_CREDIT_NO_PROBLEM:
    case SHOPKEEPER_LINE_YOU_ALTER_THAT_YOU_PAY_FOR_IT:
    case SHOPKEEPER_LINE_YOU_ALTER_THOSE_YOU_PAY_FOR_THEM:
    case SHOPKEEPER_LINE_ILL_ADD_THAT_TO_YOUR_BILL:
    {
        int dif = line_idx - SHOPKEEPER_LINE_YOU_TIN_IT_YOU_BOUGHT_IT;
        info.ghsound = is_undead_shk ? dif + GHSOUND_VOICE_SHOPKEEPER_UNDEAD_YOU_TIN_IT_YOU_BOUGHT_IT :
            shkp->female ? dif + GHSOUND_VOICE_SHOPKEEPER_FEMALE_YOU_TIN_IT_YOU_BOUGHT_IT :
            dif + GHSOUND_VOICE_SHOPKEEPER_MALE_YOU_TIN_IT_YOU_BOUGHT_IT;
        break;
    }
    default:
        break;
    }
    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (special_minimum_volume != -1.0f)
            volume = max(special_minimum_volume, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
        else
        {
            if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
                volume = SHOPKEEPER_DISTANT_VOLUME;
            else
                volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
        }
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE && info.volume >= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
        play_immediate_ghsound(info);

}


void
play_voice_shopkeeper_leave_pick_outside(shkp, tool_str, cnt, is_angry)
struct monst* shkp;
const char* tool_str;
int cnt;
boolean is_angry;
{
    if (!shkp || !shkp->mextra || !ESHK(shkp) || Deaf)
        return;

    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);
    int pickindex = 0;
    int howmanyindex = cnt > 1 ? 2 : 1;

    struct ghsound_immediate_info info = { 0 };
    if (is_angry)
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_LEAVE_STUFF_OUTSIDE :
        shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_LEAVE_STUFF_OUTSIDE :
        GHSOUND_VOICE_SHOPKEEPER_MALE_LEAVE_STUFF_OUTSIDE;
    else
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WILL_YOU_PLEASE_LEAVE_STUFF_OUTSIDE :
        shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WILL_YOU_PLEASE_LEAVE_STUFF_OUTSIDE :
        GHSOUND_VOICE_SHOPKEEPER_MALE_WILL_YOU_PLEASE_LEAVE_STUFF_OUTSIDE;

    if (!strcmp(tool_str, "pick") || !strcmp(tool_str, "pick-axe"))
    {
        pickindex = 0;
    }
    else if (!strcmp(tool_str, "mattock"))
    {
        pickindex = 1;
    }
    else if (!strcmp(tool_str, "spade"))
    {
        pickindex = 2;
    }
    else if (!strcmp(tool_str, "digging tool"))
    {
        pickindex = 3;
    }
    else if (!strcmp(tool_str, "steed"))
    {
        pickindex = 4;
    }

    info.parameter_names[0] = "HowManyIndex";
    info.parameter_values[0] = (float)howmanyindex;
    info.parameter_names[1] = "PickIndex";
    info.parameter_values[1] = (float)pickindex;
    info.parameter_names[2] = (char*)0;

    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}

void
play_voice_shopkeeper_cad_line(shkp, line_id, cad_str)
struct monst* shkp;
enum shopkeeper_cad_lines line_id;
const char* cad_str;
{
    if (!shkp || !shkp->mextra || !ESHK(shkp) || Deaf || line_id == SHOPKEEPER_CAD_LINE_NONE)
        return;

    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);
    int cad_id = 0;

    struct ghsound_immediate_info info = { 0 };

    switch (line_id)
    {
    case SHOPKEEPER_CAD_LINE_NONE:
        break;
    case SHOPKEEPER_CAD_LINE_YOU_SNEAKY:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_SNEAKY :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_SNEAKY :
            GHSOUND_VOICE_SHOPKEEPER_MALE_SNEAKY;
        break;
    case SHOPKEEPER_CAD_LINE_NO_FREE_LIBRARY:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_NO_FREE_LIBRARY :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_NO_FREE_LIBRARY :
            GHSOUND_VOICE_SHOPKEEPER_MALE_NO_FREE_LIBRARY;
        break;
    case SHOPKEEPER_CAD_LINE_LOT_OF_DAMAGE:
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_LOT_OF_DAMAGE :
            shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_LOT_OF_DAMAGE :
            GHSOUND_VOICE_SHOPKEEPER_MALE_LOT_OF_DAMAGE;
        break;
    default:
        break;
    }

    if (!strcmp(cad_str, "cad"))
    {
        cad_id = 0;
    }
    else if (!strcmp(cad_str, "minx"))
    {
        cad_id = 1;
    }
    else if (!strcmp(cad_str, "fiend"))
    {
        cad_id = 2;
    }
    else if (!strcmp(cad_str, "beast"))
    {
        cad_id = 3;
    }
    else if (!strcmp(cad_str, "thing"))
    {
        cad_id = 4;
    }

    info.parameter_names[0] = "CadIndex";
    info.parameter_values[0] = (float)cad_id;
    info.parameter_names[1] = (char*)0;

    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}


void
play_voice_shopkeeper_candelabrum_candles(shkp, candelabrum)
struct monst* shkp;
struct obj* candelabrum;
{
    if (!shkp || !shkp->mextra || !ESHK(shkp) || !candelabrum || Deaf)
        return;

    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);

    struct ghsound_immediate_info info = { 0 };
    int howmanymore = (7 - candelabrum->special_quality);

    info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_MORE_CANDLES : shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_MORE_CANDLES : GHSOUND_VOICE_SHOPKEEPER_MALE_MORE_CANDLES;

    info.parameter_names[0] = "HowManyIndex";
    info.parameter_values[0] = (float)howmanymore;
    info.parameter_names[1] = (char*)0;


    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}


void
play_voice_shopkeeper_izchak_talks(shkp, line_idx)
struct monst* shkp;
int line_idx;
{
    if (!shkp || !shkp->mextra || !ESHK(shkp) || Deaf)
        return;

    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);

    struct ghsound_immediate_info info = { 0 };

    info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_IZCHAK_TALKS : shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_IZCHAK_TALKS : GHSOUND_VOICE_SHOPKEEPER_MALE_IZCHAK_TALKS;

    info.parameter_names[0] = "LineIndex";
    info.parameter_values[0] = (float)line_idx;
    info.parameter_names[1] = (char*)0;


    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}


void
play_voice_shopkeeper_for_you(shkp, honorific_idx, base_line_idx, quan)
struct monst* shkp;
int honorific_idx, base_line_idx;
long quan;
{
    if (!shkp || !shkp->mextra || !ESHK(shkp) || Deaf)
        return;

    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);
    struct ghsound_immediate_info info = { 0 };

    if (ANGRY(shkp))
    {
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_FOR_YOU_ANGRY : shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_FOR_YOU_ANGRY : GHSOUND_VOICE_SHOPKEEPER_MALE_FOR_YOU_ANGRY;
        info.parameter_names[0] = "ItemPriceDescriptionIndex";
        info.parameter_values[0] = (float)(base_line_idx + (quan > 1) * 8);
        info.parameter_names[1] = (char*)0;
    }
    else
    {
        info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_FOR_YOU_PEACEFUL : shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_FOR_YOU_PEACEFUL : GHSOUND_VOICE_SHOPKEEPER_MALE_FOR_YOU_PEACEFUL;
        info.parameter_names[0] = "GenderIndex";
        info.parameter_values[0] = (float)(!humanoid(youmonst.data) ? 2 : flags.female);
        info.parameter_names[1] = "HonorificIndex";
        info.parameter_values[1] = (float)honorific_idx;
        info.parameter_names[2] = "ItemPriceDescriptionIndex";
        info.parameter_values[2] = (float)(base_line_idx + (quan > 1) * 8);
        info.parameter_names[3] = (char*)0;
    }

    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;
    info.play_flags = PLAY_FLAGS_NO_PLAY_IF_ALREADY_PLAYING_OR_QUEUED; /* Reduce tautophony in shops */

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}


void
play_voice_shopkeeper_pay_before_buying(shkp, obj_quan, save_quan)
struct monst* shkp;
long obj_quan, save_quan;
{
    if (!shkp || !shkp->mextra || !ESHK(shkp) || Deaf)
        return;

    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);

    struct ghsound_immediate_info info = { 0 };

    if (ANGRY(shkp))
    {
        if (obj_quan != 1)
        {
            if (save_quan > 1)
            {
                info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THESE :
                    shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THESE :
                    GHSOUND_VOICE_SHOPKEEPER_MALE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THESE;
            }
            else
            {
                info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THIS_ONE :
                    shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THIS_ONE :
                    GHSOUND_VOICE_SHOPKEEPER_MALE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THIS_ONE;
            }
        }
        else
        {
            if (save_quan > 1)
            {
                info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THESE :
                    shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THESE :
                    GHSOUND_VOICE_SHOPKEEPER_MALE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THESE;
            }
            else
            {
                info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THIS_ONE :
                    shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THIS_ONE :
                    GHSOUND_VOICE_SHOPKEEPER_MALE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THIS_ONE;
            }
        }
    }
    else
    {
        if (obj_quan != 1)
        {
            if (save_quan > 1)
            {
                info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PLEASE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THESE :
                    shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PLEASE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THESE :
                    GHSOUND_VOICE_SHOPKEEPER_MALE_PLEASE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THESE;
            }
            else
            {
                info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PLEASE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THIS_ONE :
                    shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PLEASE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THIS_ONE :
                    GHSOUND_VOICE_SHOPKEEPER_MALE_PLEASE_PAY_FOR_OTHER_ITEMS_BEFORE_BUYING_THIS_ONE;
            }
        }
        else
        {
            if (save_quan > 1)
            {
                info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PLEASE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THESE :
                    shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PLEASE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THESE :
                    GHSOUND_VOICE_SHOPKEEPER_MALE_PLEASE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THESE;
            }
            else
            {
                info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_PLEASE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THIS_ONE :
                    shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_PLEASE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THIS_ONE :
                    GHSOUND_VOICE_SHOPKEEPER_MALE_PLEASE_PAY_FOR_OTHER_ITEM_BEFORE_BUYING_THIS_ONE;
            }
        }
    }


    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}


void
play_voice_shopkeeper_how_dare_you_damage(shkp, style, dmg_str, is_shop)
struct monst* shkp;
uchar style;
const char* dmg_str;
boolean is_shop;
{
    if (!shkp || !shkp->mextra || !ESHK(shkp) || Deaf)
        return;

    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);

    struct ghsound_immediate_info info = { 0 };
    int damagetype = 0;

    if (style == 0)
    {
        if (is_shop)
        {
            info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WHO_DARED_DAMAGE_SHOP :
                shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WHO_DARED_DAMAGE_SHOP :
                GHSOUND_VOICE_SHOPKEEPER_MALE_WHO_DARED_DAMAGE_SHOP;
        }
        else
        {
            info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_WHO_DARED_DAMAGE_DOOR :
                shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_WHO_DARED_DAMAGE_DOOR :
                GHSOUND_VOICE_SHOPKEEPER_MALE_WHO_DARED_DAMAGE_DOOR;
        }
    }
    else
    {
        if (is_shop)
        {
            info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_HOW_DARE_YOU_DAMAGE_SHOP :
                shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_HOW_DARE_YOU_DAMAGE_SHOP :
                GHSOUND_VOICE_SHOPKEEPER_MALE_HOW_DARE_YOU_DAMAGE_SHOP;
        }
        else
        {
            info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_HOW_DARE_YOU_DAMAGE_DOOR :
                shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_HOW_DARE_YOU_DAMAGE_DOOR :
                GHSOUND_VOICE_SHOPKEEPER_MALE_HOW_DARE_YOU_DAMAGE_DOOR;
        }
    }

    if (!strcmp(dmg_str, "ruin"))
    {
        damagetype = 0;
    }
    else if (!strcmp(dmg_str, "dig into"))
    {
        damagetype = 1;
    }
    else if (!strcmp(dmg_str, "destroy"))
    {
        damagetype = 2;
    }
    else if (!strcmp(dmg_str, "break"))
    {
        damagetype = 3;
    }
    else if (!strcmp(dmg_str, "damage"))
    {
        damagetype = 4;
    }
    else if (!strcmp(dmg_str, "shatter"))
    {
        damagetype = 5;
    }
    else if (!strcmp(dmg_str, "disintegrate"))
    {
        damagetype = 6;
    }

    info.parameter_names[0] = "DamageType";
    info.parameter_values[0] = (float)damagetype;
    info.parameter_names[1] = (char*)0;

    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}

void
play_voice_shopkeeper_costly_alteration(shkp, otmp, alteration_type)
struct monst* shkp;
struct obj* otmp;
enum cost_alteration_types alteration_type;
{
    if (!shkp || !has_eshk(shkp) || Deaf || !otmp)
        return;

    boolean is_undead_shk = is_undead(shkp->data) || is_demon(shkp->data);

    struct ghsound_immediate_info info = { 0 };

    info.ghsound = is_undead_shk ? GHSOUND_VOICE_SHOPKEEPER_UNDEAD_COSTLY_ALTERATION :
        shkp->female ? GHSOUND_VOICE_SHOPKEEPER_FEMALE_COSTLY_ALTERATION :
        GHSOUND_VOICE_SHOPKEEPER_MALE_COSTLY_ALTERATION;
    info.parameter_names[0] = "IsMany";
    info.parameter_values[0] = (float)(otmp->quan > 1);
    info.parameter_names[1] = "CostlyType";
    info.parameter_values[1] = (float)alteration_type;
    info.parameter_names[2] = (char*)0;

    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(shkp->mx, shkp->my))
    {
        float hearing = hearing_array[shkp->mx][shkp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[shkp->mx][shkp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = shkp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}

void
play_voice_quest_leader_whoareyou(mtmp)
struct monst* mtmp;
{
    if (!mtmp || Deaf)
        return;

    struct ghsound_immediate_info info = { 0 };

    info.ghsound = GHSOUND_VOICE_QUEST_LEADER_WHOAREYOU;
    info.parameter_names[0] = "RoleIndex";
    info.parameter_values[0] = (float)urole.rolenum;
    info.parameter_names[1] = (char*)0;


    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing_array[mtmp->mx][mtmp->my]));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = mtmp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}

/* cuss_id = 0 is aspirations on your ancestry or something general; positive integers indicate lines from pager */
void
play_voice_monster_cuss(mtmp, cuss_id)
struct monst* mtmp;
int cuss_id;
{
    if (!mtmp || Deaf)
        return;

    struct ghsound_immediate_info info = { 0 };
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    enum monster_soundset_types mss = get_monster_soundset(mtmp);
    set_simple_monster_sound_id_and_volume(mss, MONSTER_SOUND_TYPE_MALEDICTION, &soundid, &volume);
    info.ghsound = soundid;
    info.parameter_names[0] = "MaledictionIndex";
    info.parameter_values[0] = (float)cuss_id;
    info.parameter_names[1] = (char*)0;

    /* Maledictions start at louder level than normal */
    volume *= 3.0;

    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        volume = max(0.15f, max((float)context.global_minimum_volume, volume * hearing));
    }
    else
        return;

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = mtmp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);
}

/* cuss_id = 0 is aspirations on your ancestry or something general; positive integers indicate lines from pager */
void
play_voice_monster_cuss_with_god_name(mtmp, cuss_id)
struct monst* mtmp;
int cuss_id;
{
    if (!mtmp || Deaf)
        return;

    int godindex = 7;
    int titleandgodindex = 9;

    get_god_indices(&godindex, &titleandgodindex);

    struct ghsound_immediate_info info = { 0 };
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    enum monster_soundset_types mss = get_monster_soundset(mtmp);
    set_simple_monster_sound_id_and_volume(mss, MONSTER_SOUND_TYPE_MALEDICTION_WITH_GOD_NAME, &soundid, &volume);
    info.ghsound = soundid;
    info.parameter_names[0] = "MaledictionIndex";
    info.parameter_values[0] = (float)cuss_id;
    info.parameter_names[1] = "GodIndex";
    info.parameter_values[1] = (float)godindex;
    info.parameter_names[2] = (char*)0;

    /* Maledictions start at louder level than normal */
    volume *= 3.0;

    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        volume = max(0.15f, max((float)context.global_minimum_volume, volume * hearing));
    }
    else
        return;

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = mtmp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);
}

void
play_voice_monster_advice(mtmp, has_advice)
struct monst* mtmp;
boolean has_advice;
{
    if (!mtmp || Deaf)
        return;

    struct ghsound_immediate_info info = { 0 };
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    enum monster_soundset_types mss = get_monster_soundset(mtmp);
    set_simple_monster_sound_id_and_volume(mss, MONSTER_SOUND_TYPE_ADVICE, &soundid, &volume);
    info.ghsound = soundid;
    info.parameter_names[0] = "LineIndex";
    info.parameter_values[0] = (float)(has_advice ? (mtmp->told_rumor ? 2 : 3) : (mtmp->told_rumor ? 1 : 0));
    info.parameter_names[1] = (char*)0;

    volume *= 1.5;

    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        volume = max(0.15f, max((float)context.global_minimum_volume, volume * hearing));
    }
    else
        return;

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = mtmp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);
}

void
play_voice_wizard_of_yendor_cuss(mtmp, malediction_id, insult_id)
struct monst* mtmp;
int malediction_id, insult_id;
{
    if (!mtmp || Deaf)
        return;

    struct ghsound_immediate_info info = { 0 };
    enum ghsound_types soundid = GHSOUND_VOICE_WIZARD_OF_YENDOR_MALEDICTION;
    float volume = 1.0f;
    info.ghsound = soundid;
    info.parameter_names[0] = "MaledictionIndex";
    info.parameter_values[0] = (float)malediction_id;
    info.parameter_names[1] = "InsultIndex";
    info.parameter_values[1] = (float)insult_id;
    info.parameter_names[2] = (char*)0;

    /* Maledictions start at louder level than normal */
    volume *= 3.0;

    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        volume = max(0.15f, max((float)context.global_minimum_volume, volume * hearing));
    }
    else
        return;

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = mtmp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);
}


void
play_voice_wizard_of_yendor_simple_line(mtmp, line_id)
struct monst* mtmp;
enum wizard_of_yendor_simple_lines line_id;
{
    if (!mtmp || Deaf)
        return;

    struct ghsound_immediate_info info = { 0 };
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    info.parameter_names[0] = (char*)0;

    switch (line_id)
    {
    case WIZARD_OF_YENDOR_LINE_NONE:
        break;
    case WIZARD_OF_YENDOR_LINE_ILL_BE_BACK:
        soundid = GHSOUND_VOICE_WIZARD_OF_YENDOR_ILL_BE_BACK;
        break;
    case WIZARD_OF_YENDOR_LINE_I_SHALL_RETURN:
        soundid = GHSOUND_VOICE_WIZARD_OF_YENDOR_I_SHALL_RETURN;
        break;
    case WIZARD_OF_YENDOR_LINE_DESTROY_THE_THIEF_MY_PET:
        soundid = GHSOUND_VOICE_WIZARD_OF_YENDOR_DESTROY_THE_THIEF_MY_PET;
        break;
    case WIZARD_OF_YENDOR_LINE_DESTROY_THE_THIEF_MY_PETS:
        soundid = GHSOUND_VOICE_WIZARD_OF_YENDOR_DESTROY_THE_THIEF_MY_PETS;
        break;
    case WIZARD_OF_YENDOR_LINE_DOUBLE_TROUBLE:
        soundid = GHSOUND_VOICE_WIZARD_OF_YENDOR_DOUBLE_TROUBLE;
        break;
    case WIZARD_OF_YENDOR_LINE_SO_THOU_THOUGHT_THOU_COULDST_KILL_ME:
        soundid = GHSOUND_VOICE_WIZARD_OF_YENDOR_SO_THOU_THOUGHT_THOU_COULDST_KILL_ME;
        break;
    case WIZARD_OF_YENDOR_LINE_SO_THOU_THOUGHT_THOU_COULDST_ELUDE_ME:
        soundid = GHSOUND_VOICE_WIZARD_OF_YENDOR_SO_THOU_THOUGHT_THOU_COULDST_ELUDE_ME;
        break;
    default:
        break;
    }

    /* Maledictions start at louder level than normal */
    volume *= 3.0;

    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        volume = max(0.30f, max((float)context.global_minimum_volume, volume * hearing));
    }
    else
    {
        volume = 0.30f;
    }

    info.ghsound = soundid;
    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = mtmp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);
}

void
play_monster_line_indexed_sound(mtmp, sound_id, line_id, play_group, sfx_sound_type, minimum_volume, pflags)
struct monst* mtmp;
enum monster_sound_types sound_id;
int line_id;
enum sound_play_groups play_group;
enum immediate_sound_types sfx_sound_type;
double minimum_volume;
unsigned long pflags;
{
    play_monster_simply_indexed_sound(mtmp, sound_id, line_id, play_group, sfx_sound_type, minimum_volume, "LineIndex", pflags);
}

void
play_monster_msg_indexed_sound(mtmp, sound_id, line_id, play_group, sfx_sound_type, minimum_volume, pflags)
struct monst* mtmp;
enum monster_sound_types sound_id;
int line_id;
enum sound_play_groups play_group;
enum immediate_sound_types sfx_sound_type;
double minimum_volume;
unsigned long pflags;
{
    play_monster_simply_indexed_sound(mtmp, sound_id, line_id, play_group, sfx_sound_type, minimum_volume, "MsgIndex", pflags);
}


void
play_monster_simply_indexed_sound(mtmp, sound_id, line_id, play_group, sfx_sound_type, minimum_volume, index_name, pflags)
struct monst* mtmp;
enum monster_sound_types sound_id;
int line_id;
enum sound_play_groups play_group;
enum immediate_sound_types sfx_sound_type;
double minimum_volume;
const char* index_name;
unsigned long pflags;
{
    if (!mtmp || Deaf)
        return;

    struct ghsound_immediate_info info = { 0 };
    enum ghsound_types soundid = GHSOUND_NONE;
    float volume = 1.0f;
    enum monster_soundset_types mss = get_monster_soundset(mtmp);
    set_simple_monster_sound_id_and_volume(mss, sound_id, &soundid, &volume);
    info.ghsound = soundid;
    info.parameter_names[0] = index_name;
    info.parameter_values[0] = (float)line_id;
    info.parameter_names[1] = (char*)0;

    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        volume = max((float)minimum_volume, max((float)context.global_minimum_volume, volume * hearing));
    }
    else
        return;

    info.volume = min(1.0f, volume);
    info.play_group = play_group;
    info.sound_type = sfx_sound_type;
    info.dialogue_mid = mtmp->m_id;
    info.play_flags = pflags;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);
}

void
play_monster_standard_dialogue_line(mtmp, line_id)
struct monst* mtmp;
enum monster_standard_dialogue_lines line_id;
{
    play_monster_line_indexed_sound(mtmp, MONSTER_SOUND_TYPE_STANDARD_DIALOGUE_LINE, line_id, SOUND_PLAY_GROUP_LONG, IMMEDIATE_SOUND_DIALOGUE, 0.15f, 0UL);
}

void
play_monster_item_trading_line(mtmp, line_id)
struct monst* mtmp;
enum monster_item_trading_lines line_id;
{
    play_monster_line_indexed_sound(mtmp, MONSTER_SOUND_TYPE_ITEM_TRADING, line_id, SOUND_PLAY_GROUP_LONG, IMMEDIATE_SOUND_DIALOGUE, 0.15f, 0UL);
}


void
play_monster_special_dialogue_line(mtmp, line_id)
struct monst* mtmp;
int line_id;
{
    play_monster_special_dialogue_line_with_flags(mtmp, line_id, 0UL);
}

void
play_monster_special_dialogue_line_with_flags(mtmp, line_id, pflags)
struct monst* mtmp;
int line_id;
unsigned long pflags;
{
    play_monster_msg_indexed_sound(mtmp, MONSTER_SOUND_TYPE_SPECIAL_DIALOGUE_LINE, line_id, SOUND_PLAY_GROUP_LONG, IMMEDIATE_SOUND_DIALOGUE, 0.15f, pflags);
}

void
play_monster_standard_sound(mtmp, line_id)
struct monst* mtmp;
enum monster_standard_sounds line_id;
{
    play_monster_line_indexed_sound(mtmp, MONSTER_SOUND_TYPE_STANDARD_SOUND, line_id, SOUND_PLAY_GROUP_NORMAL, IMMEDIATE_SOUND_SFX, 0.0f, 0UL);
}

void
play_monster_special_sound(mtmp, line_id)
struct monst* mtmp;
int line_id;
{
    play_monster_special_sound_with_flags(mtmp, line_id, 0UL);
}

void
play_monster_special_sound_with_flags(mtmp, line_id, pflags)
struct monst* mtmp;
int line_id;
unsigned long pflags;
{
    play_monster_msg_indexed_sound(mtmp, MONSTER_SOUND_TYPE_SPECIAL_SOUND, line_id, SOUND_PLAY_GROUP_NORMAL, IMMEDIATE_SOUND_SFX, 0.0f, pflags);
}


void
play_monster_ouch_sound(mtmp, line_id)
struct monst* mtmp;
enum monster_ouch_sounds line_id;
{
    play_monster_line_indexed_sound(mtmp, MONSTER_SOUND_TYPE_OUCH, line_id, SOUND_PLAY_GROUP_NORMAL, IMMEDIATE_SOUND_SFX, 0.0f, 0UL);
}

void
play_monster_happy_sound(mtmp, line_id)
struct monst* mtmp;
enum monster_happy_sounds line_id;
{
    play_monster_line_indexed_sound(mtmp, MONSTER_SOUND_TYPE_HAPPY, line_id, SOUND_PLAY_GROUP_NORMAL, IMMEDIATE_SOUND_SFX, 0.0f, 0UL);
}

void
play_monster_unhappy_sound(mtmp, line_id)
struct monst* mtmp;
enum monster_unhappy_sounds line_id;
{
    play_monster_line_indexed_sound(mtmp, MONSTER_SOUND_TYPE_UNHAPPY, line_id, SOUND_PLAY_GROUP_NORMAL, IMMEDIATE_SOUND_SFX, 0.0f, 0UL);
}

void
play_monster_flounder_sound(mtmp, line_id)
struct monst* mtmp;
enum monster_flounder_sounds line_id;
{
    play_monster_line_indexed_sound(mtmp, MONSTER_SOUND_TYPE_FLOUNDER, line_id, SOUND_PLAY_GROUP_NORMAL, IMMEDIATE_SOUND_SFX, 0.0f, 0UL);
}

void
play_monster_chat_sound(mtmp, line_id)
struct monst* mtmp;
int line_id;
{
    play_monster_line_indexed_sound(mtmp, MONSTER_SOUND_TYPE_CHAT, line_id, SOUND_PLAY_GROUP_LONG, IMMEDIATE_SOUND_DIALOGUE, 0.0f, 0UL);
}

void
play_player_flounder_sound(line_id)
enum monster_flounder_sounds line_id;
{
    play_player_line_indexed_sound(MONSTER_SOUND_TYPE_FLOUNDER, line_id, SOUND_PLAY_GROUP_NORMAL, IMMEDIATE_SOUND_SFX);
}


enum monster_soundset_types
get_monster_soundset(mtmp)
struct monst* mtmp;
{
    if (!mtmp)
        return MONSTER_SOUNDSET_NONE;

    boolean isyou = (mtmp == &youmonst);
    boolean isfemale = isyou ? flags.female : mtmp->female;

    if (mtmp->issmith)
        return is_gnoll(mtmp->data) ? MONSTER_SOUNDSET_SMITH_FLIND : is_undead(mtmp->data) ? MONSTER_SOUNDSET_SMITH_UNDEAD : isfemale ? MONSTER_SOUNDSET_SMITH_FEMALE : MONSTER_SOUNDSET_SMITH_MALE;
    else if (mtmp->isshk)
        return is_undead(mtmp->data) ? MONSTER_SOUNDSET_SHOPKEEPER_UNDEAD : isfemale ? MONSTER_SOUNDSET_SHOPKEEPER_FEMALE : MONSTER_SOUNDSET_SHOPKEEPER_MALE;
    else if (mtmp->isnpc && has_enpc(mtmp))
    {
        switch (ENPC(mtmp)->npc_typ)
        {
        case NPC_ARTIFICER:
            return is_undead(mtmp->data) ? MONSTER_SOUNDSET_ARTIFICER_UNDEAD : isfemale ? MONSTER_SOUNDSET_ARTIFICER_FEMALE : MONSTER_SOUNDSET_ARTIFICER_MALE;
            break;
        case NPC_GEOLOGIST:
            return is_undead(mtmp->data) ? MONSTER_SOUNDSET_GEOLOGIST_UNDEAD : isfemale ? MONSTER_SOUNDSET_GEOLOGIST_FEMALE : MONSTER_SOUNDSET_GEOLOGIST_MALE;
            break;
        case NPC_HERMIT:
            return MONSTER_SOUNDSET_HERMIT1;
            break;
        case NPC_HERMIT2:
            return MONSTER_SOUNDSET_HERMIT2;
            break;
        case NPC_HERMIT3:
            return MONSTER_SOUNDSET_HERMIT3;
            break;
        case NPC_ORC_HERMIT3:
            return MONSTER_SOUNDSET_ORC_HERMIT3;
            break;
        case NPC_ELVEN_BARD:
            return MONSTER_SOUNDSET_ELVEN_BARD;
            break;
        case NPC_QUANTUM_MECHANIC:
            return MONSTER_SOUNDSET_PROFESSOR_OF_QUANTUM_MECHANICS;
            break;
        case NPC_WARP_ENGINEER:
            break;
        default:
            break;
        }
        return isfemale ? mtmp->data->female_soundset : mtmp->data->soundset;
    }
    else if ((mtmp->mnum == PM_ALIGNED_PRIEST || mtmp->mnum == PM_HIGH_PRIEST) && mon_aligntyp(mtmp) == A_NONE)
        return isfemale ? MONSTER_SOUNDSET_PRIESTESS_OF_MOLOCH : MONSTER_SOUNDSET_PRIEST_OF_MOLOCH;
    else if ((mtmp->mnum == PM_ANGEL) && mon_aligntyp(mtmp) == A_NONE)
        return isfemale ? MONSTER_SOUNDSET_HUMANLIKE_DEMONESS : MONSTER_SOUNDSET_HUMANLIKE_DEMON;
    else
        return isfemale ? mtmp->data->female_soundset : mtmp->data->soundset;
}

void
play_hermit_dialogue_line(mtmp, soundid, soundindextype, lineid)
struct monst* mtmp;
enum ghsound_types soundid;
uchar soundindextype;
int lineid;
{
    if (!mtmp || Deaf)
        return;

    struct ghsound_immediate_info info = { 0 };
    float volume = 1.0f;
    info.ghsound = soundid;
    int paramcnt = 0;
    switch (soundindextype)
    {
    default:
    case 0:
        info.parameter_names[paramcnt] = "LineIndex";
        info.parameter_values[paramcnt] = (float)lineid;
        paramcnt++;
        break;
    case 1:
        info.parameter_names[paramcnt] = "MsgIndex";
        info.parameter_values[paramcnt] = (float)lineid;
        paramcnt++;
        break;
    case 2:
        /* Nothing, lineid is ignored */
        break;
    }
    info.parameter_names[paramcnt] = (char*)0;

    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        volume = max(0.30f, max((float)context.global_minimum_volume, volume * hearing));
    }
    else
        return;

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = mtmp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);
}

void
play_voice_god_simple_line_at(x, y, line_id)
xchar x, y;
enum god_simple_lines line_id;
{
    if (!IS_ALTAR(levl[x][y].typ))
        return; /* No altar */

    aligntyp godalign = 0;
    if (levl[x][y].subtyp == ALTAR_SUBTYPE_MOLOCH)
        godalign = A_NONE; /* Altar of Moloch */
    else if (a_align(x, y) == A_NONE)
        return; /* No god is associated with altar */
    else
        godalign = a_align(x, y);

    play_voice_god_simple_line_by_align(godalign, line_id);
}

void
play_voice_god_simple_line_by_align(godalign, line_id)
aligntyp godalign;
enum god_simple_lines line_id;
{
    /* You will hear the god even if you are deaf */

    boolean isfemale = align_gfemale(godalign);
    struct ghsound_immediate_info info = { 0 };
    enum ghsound_types soundid = isfemale ? GHSOUND_VOICE_GOD_FEMALE_SPEAKS : GHSOUND_VOICE_GOD_MALE_SPEAKS;
    float volume = 1.0f;
    info.parameter_names[0] = "MsgIndex";
    info.parameter_values[0] = (float)line_id;
    info.parameter_names[1] = (char*)0;

    info.ghsound = soundid;
    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = 0;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);
}

void
play_voice_oracle_major_consultation(mtmp, line_idx)
struct monst* mtmp;
int line_idx;
{
    if (!mtmp || Deaf)
        return;

    struct ghsound_immediate_info info = { 0 };

    info.ghsound = GHSOUND_VOICE_ORACLE_MAJOR_CONSULTATION;

    info.parameter_names[0] = "MsgIndex";
    info.parameter_values[0] = (float)line_idx;
    info.parameter_names[1] = (char*)0;


    float volume = SHOPKEEPER_BASE_VOLUME;
    if (isok(mtmp->mx, mtmp->my))
    {
        float hearing = hearing_array[mtmp->mx][mtmp->my];
        if (max(hearing, context.global_minimum_volume) <= SHOPKEEPER_DISTANT_VOLUME_THRESHOLD)
            volume = SHOPKEEPER_DISTANT_VOLUME;
        else
            volume = max(SHOPKEEPER_NEARBY_MINIMUM_VOLUME, max((float)context.global_minimum_volume, volume * hearing));
    }

    info.volume = min(1.0f, volume);
    info.play_group = SOUND_PLAY_GROUP_LONG;
    info.sound_type = IMMEDIATE_SOUND_DIALOGUE;
    info.dialogue_mid = mtmp->m_id;

    if (info.ghsound > GHSOUND_NONE)
        play_immediate_ghsound(info);

}

/* soundset.c */