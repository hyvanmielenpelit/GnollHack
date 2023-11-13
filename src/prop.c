/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2023-05-22 */

/* GnollHack 4.0 prop.c */
/* Copyright (c) Janne Gustafsson, 2021.                            */
/* GnollHack may be freely redistributed.  See license for details. */

#include "hack.h"
#define PCLR_BLACK {0, 0, 0}
#define PCLR_NONE  {0, 0, 0}
#define PCLR_WHITE {255, 255, 255}

/* Moved from timeout.c */
const struct propname propertynames[] = {
    { INVULNERABLE, "invulnerable", "invulnerability" },
    { STONED, "petrifying", "petrification" },
    { SLIMED, "becoming slime", "transformation into slime" },
    { STRANGLED, "being strangled", "strangulation" },
    { SICK, "fatally sick", "fatal sickness" },
    { STUNNED, "stunned", "stunning" },
    { CONFUSION, "confused", "confusion" },
    { HALLUC, "hallucinating", "hallucination" },
    { BLINDED, "blinded", "blindness" },
    { DEAF, "deafened", "deafness" },
    { VOMITING, "vomiting", "vomiting" },
    { GLIB, "slippery fingers", "slippery fingers" },
    { WOUNDED_LEGS, "wounded legs", "wounded legs" },
    { SLEEPY, "sleepy", "sleepiness" },
    { TELEPORT, "teleporting", "teleportation" },
    { POLYMORPH, "polymorphing", "polymorph" },
    { LEVITATION, "levitating", "levitation" },
    { FAST, "fast", "fast speed" },
    { CLAIRVOYANT, "clairvoyant", "clairvoyance" },
    { DETECT_MONSTERS, "detecting monsters", "monster detection" },
    { SEE_INVISIBLE, "seeing invisible", "see invisible" },
    { INVISIBILITY, "invisible", "invisibility" },
    /* properties beyond here don't have timed values during normal play,
       so there's not much point in trying to order them sensibly;
       they're either on or off based on equipment, role, actions, &c */
    { FIRE_IMMUNITY, "fully resistant to fire", "full fire resistance" },
    { COLD_IMMUNITY, "fully resistant to cold", "full cold resistance" },
    { SLEEP_RESISTANCE, "resistant to sleep", "sleep resistance" },
    { DISINTEGRATION_RESISTANCE, "resistant to disintegration", "disintegration resistance" },
    { SHOCK_IMMUNITY, "fully resistant to shock", "full shock resistance" },
    { POISON_RESISTANCE, "resistant to poison", "poison resistance" },
    { ACID_IMMUNITY, "fully resistant to acid", "full acid resistance" },
    { STONE_RESISTANCE, "resistant to stoning", "stoning resistance" },
    { DRAIN_RESISTANCE, "resistant to drain", "drain resistance" },
    { SICK_RESISTANCE, "resistant to sickness", "sickness resistance" },
    { ANTIMAGIC, "resistant to magic", "magic resistance" },
    { HALLUC_RES, "resistant to hallucination", "hallucination resistance" },
    { FUMBLING, "fumbling", "fumbling" },
    { HUNGER, "voraciously hungry", "voracious hunger" },
    { TELEPAT, "telepathic", "telepathy" },
    { WARNING, "warned", "warning" },
    { WARN_OF_MON, "warned of monster type or class", "warning of monster type or class" },
    { WARN_UNDEAD, "warned of undead", "warning of undead" },
    { SEARCHING, "searching", "searching" },
    { INFRAVISION, "having infravision", "infravision" },
    { ADORNED, "adorned", "adornment" },
    { DISPLACED, "displaced", "displacement" },
    { STEALTH, "stealthy", "stealth" },
    { AGGRAVATE_MONSTER, "monster aggravation", "monster aggravation" },
    { CONFLICT, "causing conflict", "conflict" },
    { JUMPING, "jumping", "jumping" },
    { TELEPORT_CONTROL, " controlling teleportation", "teleport control" },
    { FLYING, "flying", "flying" },
    { WATER_WALKING, "water walking", "water walking" },
    { SWIMMING, "swimming", "swimming" },
    { MAGICAL_BREATHING, "magical breathing", "magical breathing" },
    { PASSES_WALLS, "passing thru walls", "pass thru walls" },
    { SLOW_DIGESTION, "digesting slowly", "slow digestion" },
    { HALF_SPELL_DAMAGE, "receiving half normal spell damage", "half received spell damage" },
    { HALF_PHYSICAL_DAMAGE, "receiving half normal physical damage", "half received physical damage" },
    { REGENERATION, "regenerating", "regeneration" },
    { ENERGY_REGENERATION, "regenerating mana", "mana regeneration" },
    { MAGICAL_PROTECTION, "magically protected", "magical protection" ,"-3 AC, +1 MC"},
    { PROT_FROM_SHAPE_CHANGERS, "protected from shape changers", "protection from shape changers" },
    { POLYMORPH_CONTROL, "controlling polymorphing", "polymorph control" },
    { UNCHANGING, "unchanging", "unchange" },
    { REFLECTING, "reflecting", "reflection" },
    { FREE_ACTION, "resistant to paralysis", "paralysis resistance" },
    { FIXED_ABIL, "having fixed abilities", "fixed abilities" },
    { LIFESAVED, "life will be saved", "life saving" },
    { DEATH_RESISTANCE, "resistant to death", "death resistance" },
    { LYCANTHROPY_RESISTANCE, "resistant to lycanthropy", "lycanthropy resistance" },
    { CURSE_RESISTANCE, "resistant to curses", "curse resistance" },
    { BLIND_TELEPATHY, "telepathic when blind", "blind telepathy" },
    { ENHANCED_VISION, "enhanced vision", "enhanced vision" },
    { FLASH_RESISTANCE, "resistant to flashes", "flash resistance" },
    { EYE_PROTECTION, "protected from eye damage", "protection from eye damage" },
    { BRAIN_PROTECTION, "protected from brain damage", "protection from brain damage" },
    { WARN_DEMON, "warned of demons", "warning of demons" },
    { WARN_ORC, "warned of orcs", "warning of orcs" },
    { WARN_TROLL, "warned of trolls", "warning of trolls" },
    { WARN_GIANT, "warned of giants", "warning of giants" },
    { WARN_DRAGON, "warned of dragons", "warning of dragons" },
    { WARN_ELF, "warned of elves", "warning of elves" },
    { WARN_DWARF, "warned of dwarves", "warning of dwarves" },
    { WARN_GNOLL, "warned of gnolls", "warning of gnolls" },
    { WARN_HUMAN, "warned of human beings", "warning of human beings" },
    { WARN_LYCANTHROPE, "warned of lycanthropes", "warning of lycanthropes" },
    { WARN_ANGEL, "warned of angels", "warning of angels" },
    { WARN_OGRE, "warned of ogres", "warning of ogres" },
    { WARN_OGRE, "warned of gnomes", "warning of gnomes" },
    { CHARM_RESISTANCE, "resistant to charm", "charm resistance" },
    { MIND_SHIELDING, "mind shielded", "mind shielding" },
    { ODD_IDEAS, "having visionary ideas", "visionary ideas" },
    { MAGICAL_KICKING, "kicking magically", "magical kicking" },
    { BLOCKS_INVISIBILITY, "blocking invisibility", "blocks invisibility" },
    { BLOCKS_BLINDNESS, "blocking blindness", "blocks blindness"  },
    { BLOCKS_CLAIRVOYANCE, "blocking clairvoyance", "blocks clairvoyance" },
    { HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS, "receiving half physical damage from undead and demons", "half physical damage from undead and demons" },
    { MAGICAL_SHIELDING, "magically shielded", "magical shielding", "-6 AC, +2 MC" },
    { MAGICAL_BARKSKIN, "magically barkskinned", "magical barkskin", "-12 AC, +4 MC" },
    { MAGICAL_STONESKIN, "magically stoneskinned", "magical stoneskin", "-18 AC, +6 MC" },
    { XRAY_VISION, "having X-ray vision", "X-ray vision" },
    { VERY_FAST, "very fast", "very fast speed" },
    { SLOWED, "slowed", "slow speed" },
    { CANCELLED, "cancelled", "cancellation" },
    { HALVED_MAGIC_RESISTANCE, "having halved magic resistance", "halved magic resistance" },
    { NO_MAGIC_RESISTANCE, "lacking magic resistance", "no magic resistance" },
    { CHARMED, "charmed", "charm" },
    { PARALYZED, "paralyzed", "paralysis" },
    { FEARFUL, "unnaturally frightened", "magically induced fear" },
    { FEAR_RESISTANCE, "resistant to fear", "fear resistance" },
    { SLEEPING, "sleeping unnaturally deeply", "magically induced sleep" },
    { SUMMON_FORBIDDEN, "having summoning forbidden", "summoning forbidden" },
    { SILENCED, "silenced", "silence" },
    { LEVITATION_CONTROL, "controlling levitation", "levitation control" },
    { FIRE_VULNERABILITY, "vulnerable to fire", "fire vulnerability" },
    { COLD_VULNERABILITY, "vulnerable to cold", "cold vulnerability" },
    { SHOCK_VULNERABILITY, "vulnerable to electricity", "electricity vulnerability" },
    { MAGIC_MISSILE_VULNERABILITY, "vulnerable to magic damage", "magic damage vulnerability" },
    { DOUBLE_PHYSICAL_DAMAGE, "receiving double physical damage", "double physical damage" },
    { DOUBLE_SPELL_DAMAGE, "receiving double spell damage", "double spell damage" },
    { ENHANCED_UNTRAP, "enhanced in untrapping", "enhanced untrapping" },
    { BLOCKS_LEVITATION, "blocking levitation", "blocks levitation" },
    { BLOCKS_FLYING, "blocking flying", "blocks flying" },
    { ONE_FOURTH_MAGIC_RESISTANCE, "having 25% of normal magic resistance", "25% of normal magic resistance" },
    { THREE_FOURTHS_MAGIC_RESISTANCE, "having 75% of normal magic resistance", "75% of normal magic resistance" },
    { BLINDFOLDED, "blindfolded", "blindness due to a blindfold" },
    { TITAN_STRENGTH, "as strong as a titan", "titan strength" },
    { MAGIC_MISSILE_IMMUNITY, "fully resistant to magic missiles", "full magic missile resistance" },
    { STUN_RESISTANCE, "stun resistant", "stun resistance" },
    { FOOD_POISONED, "fatally food poisoned", "fatal food poisoning" },
    { BISECTION_RESISTANCE, "protected from bisection", "protection from bisection" },
    { DIVINE_ENDURANCE, "having as high constitution as a demigod", "constitution of a demigod" },
    { DIVINE_DEXTERITY, "having as high dexterity as a demigod", "dexterity of a demigod" },
    { DIVINE_INTELLECT, "having as high intelligence as a demigod", "intelligence of a demigod" },
    { DIVINE_WISDOM, "having as high wisdom as a demigod", "wisdom of a demigod" },
    { DIVINE_CHARISMA, "having as high charisma as a demigod", "charisma of a demigod" },
    { MUMMY_ROT, "contracted with mummy rot", "mummy rot" },
    { FIRE_RESISTANCE, "50% resistant to fire", "50% fire resistance" },
    { COLD_RESISTANCE, "50% resistant to cold", "50% cold resistance" },
    { SHOCK_RESISTANCE, "50% resistant to shock", "50% shock resistance" },
    { MAGIC_MISSILE_RESISTANCE, "50% resistant to magic missiles", "50% magic missile resistance" },
    { ACID_RESISTANCE, "50% resistant to acid", "50% acid resistance" },
    { ULTRA_FAST, "ultra fast", "ultra fast speed" },
    { SUPER_FAST, "super fast", "super fast speed" },
    { LIGHTNING_FAST, "lightning fast", "lightning fast speed" },
    { HEROISM, "heroic", "heroism" },
    { SUPER_HEROISM, "super-heroic", "super-heroism" },
    { RAPID_REGENERATION, "rapidly regenerating", "rapid regeneration" },
    { RAPIDER_REGENERATION, "very rapidly regenerating", "very rapid regeneration" },
    { RAPIDEST_REGENERATION, "very very rapidly regenerating", "very very rapid regeneration" },
    { RAPID_ENERGY_REGENERATION, "rapidly regenerating mana", "rapid mana regeneration" },
    { RAPIDER_ENERGY_REGENERATION, "very rapidly regenerating mana", "very rapid mana regeneration" },
    { RAPIDEST_ENERGY_REGENERATION, "very very rapidly regenerating mana", "very very rapid mana regeneration" },
    { MELEE_LIFE_LEECH, "having melee life leech", "melee life leech" },
    { CRAZED, "crazed", "crazedness" },
    { DIVINE_REGENERATION, "extremely rapidly regenerating", "extremely rapid regeneration" },
    { IMPROVED_FIRE_RESISTANCE, "75% resistant to fire", "75% fire resistance" },
    { IMPROVED_COLD_RESISTANCE, "75% resistant to cold", "75% cold resistance" },
    { IMPROVED_SHOCK_RESISTANCE, "75% resistant to shock", "75% shock resistance" },
    { IMPROVED_MAGIC_MISSILE_RESISTANCE, "75% resistant to magic missiles", "75% magic missile resistance" },
    { IMPROVED_ACID_RESISTANCE, "75% resistant to acid", "75% acid resistance" },
    { UNDEAD_IMMOBILITY, "held magically immobile", "magically induced undead immobility" },
    { UNDEAD_CONTROL, "controlled as undead", "undead control" },
    { CANCELLATION_RESISTANCE, "resistant to cancellation", "cancellation resistance" },
    { HALF_SLOW_DIGESTION, "digesting half slower than normal", "half slower digestion" },
    { SLIME_RESISTANCE, "resistant to green slime", "slime resistance" },
    { LAUGHING, "laughing uncontrollably", "uncontrollable laughter" },
    { TOTTERING, "shaking and tottering", "shaking and tottering" },
    { MARTIAL_PROWESS, "imbued with martial prowess", "martial prowess" },
    {  0, 0, 0 },
};

NEARDATA struct prop_info property_definitions[MAX_PROPS] =
{
    { "",                             0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* = 0 */
    { "fire-immunity",                1, 1, 0,  0, 0,  PCLR_BLACK,  { 255, 255, 192 }, PROPFLAGS_BUFF_CANCELLABLE}, /* FIRE_IMMUNITY = 1 */
    { "cold-immunity",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* COLD_IMMUNITY = 2 */
    { "sleep-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SLEEP_RESISTANCE = 3 */
    { "disintegration-resistance",    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DISINTEGRATION_RESISTANCE = 4 */
    { "shock-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SHOCK_IMMUNITY = 5 */
    { "poison-resistance",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* POISON_RESISTANCE = 6 */
    { "acid-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ACID_IMMUNITY = 7 */
    { "stone-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* STONE_RESISTANCE = 8 */
    { "drain-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DRAIN_RESISTANCE = 9 */
    { "sick-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SICK_RESISTANCE = 10 */
    { "invulnerable",                 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* INVULNERABLE = 11 */
    { "antimagic",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ANTIMAGIC = 12 */
    { "stunned",                      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* STUNNED = 13 */
    { "confusion",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* CONFUSION = 14 */
    { "blinded",                      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLINDED = 15 */
    { "deaf",                         0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* DEAF = 16 */
    { "sick",                         0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SICK = 17 */
    { "stoned",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* STONED = 18 */
    { "strangled",                    0, 0, 1,  6, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* STRANGLED = 19 */
    { "vomiting",                     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* VOMITING = 20 */
    { "glib",                         0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* GLIB = 21 */
    { "slimed",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SLIMED = 22 */
    { "hallucination",                0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* HALLUC = 23 */
    { "hallucination-resistance",     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALLUC_RES = 24 */
    { "fumbling",                     0, 0, 1,  0, 20,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* FUMBLING = 25 */
    { "wounded-legs",                 0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* WOUNDED_LEGS = 26 */
    { "sleepy",                       0, 0, 1,  0, 100,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SLEEPY = 27 */
    { "hunger",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* HUNGER = 28 */
    { "see-invisible",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SEE_INVISIBLE = 29 */
    { "telepathy",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* TELEPAT = 30 */ 
    { "warning",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARNING = 31 */
    { "warn-of-mon",                  0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_OF_MON = 32 */
    { "warn-undead",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_UNDEAD = 33 */
    { "searching",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SEARCHING = 34 */
    { "clairvoyant",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CLAIRVOYANT = 35 */
    { "infravision",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* INFRAVISION = 36 */
    { "detect-monsters",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DETECT_MONSTERS = 37 */
    { "adorned",                      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* ADORNED = 38 */
    { "invisibility",                 0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* INVISIBILITY = 39 */
    { "displaced",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DISPLACED = 40 */
    { "stealth",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* STEALTH = 41 */
    { "aggravate-monster",            0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* AGGRAVATE_MONSTER = 42 */
    { "conflict",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* CONFLICT = 43 */
    { "jumping",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* JUMPING = 44 */
    { "teleport",                     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* TELEPORT = 45 */
    { "teleport-control",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* TELEPORT_CONTROL = 46 */
    { "levitation",                   0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LEVITATION = 47 */
    { "flying",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FLYING = 48 */
    { "water-walking",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WATER_WALKING = 49 */
    { "swimming",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SWIMMING = 50 */
    { "magical-breathing",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_BREATHING = 51 */
    { "passes-walls",                 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* PASSES_WALLS = 52 */
    { "slow-digestion",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SLOW_DIGESTION = 53 */
    { "half-spell-damage",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALF_SPELL_DAMAGE = 54 */
    { "half-physical-damage",         1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALF_PHYSICAL_DAMAGE = 55 */
    { "regeneration",                 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* REGENERATION = 56 */
    { "energy-regeneration",          1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ENERGY_REGENERATION = 57 */
    { "magical-protection",           1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_PROTECTION = 58 */
    { "prot-from-shapechangers",      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* PROT_FROM_SHAPE_CHANGERS = 59 */
    { "polymorph",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* POLYMORPH = 60 */
    { "polymorph-control",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* POLYMORPH_CONTROL = 61 */
    { "unchanging",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* UNCHANGING = 62 */
    { "fast",                         1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FAST = 63 */
    { "reflecting",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* REFLECTING = 64 */
    { "free-action",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FREE_ACTION = 65 */
    { "fixed-ability",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FIXED_ABIL = 66 */
    { "life-saved",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LIFESAVED = 67 */
    { "death-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DEATH_RESISTANCE = 68 */
    { "lycanthropy-resistance",       1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LYCANTHROPY_RESISTANCE = 69 */
    { "curse-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CURSE_RESISTANCE = 70 */
    { "blind-telepathy",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* BLIND_TELEPATHY = 71 */
    { "enhanced-vision",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ENHANCED_VISION = 72 */
    { "flash-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FLASH_RESISTANCE = 73 */
    { "eye-protection",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* EYE_PROTECTION = 74 */
    { "brain-protection",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* BRAIN_PROTECTION = 75 */
    { "warn-demon",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_DEMON = 76 */
    { "warn-orc",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_ORC = 77 */
    { "warn-troll",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_TROLL = 78 */
    { "warn-giant",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_GIANT = 79 */
    { "warn-dragon",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_DRAGON = 80 */
    { "warn-elf",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_ELF = 81 */
    { "warn-dwarf",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_DWARF = 82 */
    { "warn-gnoll",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_GNOLL = 83 */
    { "warn-human",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_HUMAN = 84 */
    { "warn-lycanthrope",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_LYCANTHROPE = 85 */
    { "warn-angel",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_ANGEL = 86 */
    { "warn-ogre",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_OGRE = 87 */
    { "warn-gnome",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* WARN_GNOME = 88 */
    { "charm-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CHARM_RESISTANCE = 89 */
    { "mind-shielding",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MIND_SHIELDING = 90 */
    { "odd-ideas",                    0, 0, 1,  100, 150,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ODD_IDEAS = 91 */
    { "airless-environment",          0, 0, 1,  6, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* AIRLESS_ENVIRONMENT = 92 */
    { "magical-kicking",              0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_KICKING = 93 */
    { "blocks-invisibility",          0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_INVISIBILITY = 94 */
    { "blocks-blindness",             0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_BLINDNESS = 95 */
    { "blocks-clairvoyance",          0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_CLAIRVOYANCE = 96 */
    { "half-physical-damage-against-undead-and-demons",  0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALF_PHYSICAL_DAMAGE_AGAINST_UNDEAD_AND_DEMONS = 97 */
    { "magical-shielding",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_SHIELDING = 98 */
    { "magical-barkskin",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_BARKSKIN = 99 */
    { "magical-stoneskin",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGICAL_STONESKIN = 100 */
    { "xray-vision",                  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* XRAY_VISION = 101 */
    { "very-fast",                    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* VERY_FAST = 102 */
    { "slowed",                       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SLOWED = 103 */
    { "cancelled",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* CANCELLED = 104 */
    { "halved-magic-resistance",      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALVED_MAGIC_RESISTANCE = 105 */
    { "no-magic-resistance",          1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* NO_MAGIC_RESISTANCE = 106 */
    { "charmed",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CHARMED = 107 */
    { "paralyzed",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* PARALYZED = 108 */
    { "fearful",                      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* FEARFUL = 109 */
    { "fear-resistance",              0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FEAR_RESISTANCE = 110 */
    { "sleeping",                     0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SLEEPING = 111 */
    { "summon-forbidden",             0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* SUMMON_FORBIDDEN = 112 */
    { "silenced",                     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SILENCED = 113 */
    { "levitation-control",           1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LEVITATION_CONTROL = 114 */
    { "fire-vulnerability",           0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* FIRE_VULNERABILITY = 115 */
    { "cold-vulnerability",           0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* COLD_VULNERABILITY = 116 */
    { "shock-vulnerability",          0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SHOCK_VULNERABILITY = 117 */
    { "magic-missile-vulnerability",  0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGIC_MISSILE_VULNERABILITY = 118 */
    { "double-physical-damage",       0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DOUBLE_PHYSICAL_DAMAGE = 119 */
    { "double-spell-damage",          0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DOUBLE_SPELL_DAMAGE = 120 */
    { "enhanced-untrap",              0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ENHANCED_UNTRAP = 121 */
    { "blocks-levitation",            0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_LEVITATION = 122 */
    { "blocks-flying",                0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLOCKS_FLYING = 123 */
    { "one-fourth-magic-resistance",  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ONE_FOURTH_MAGIC_RESISTANCE = 124 */
    { "three-fourths-magic-resistance", 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* THREE_FOURTHS_MAGIC_RESISTANCE = 125 */
    { "blindfolded",                  0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* BLINDFOLDED = 126 */
    { "titan-strength",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* TITAN_STRENGTH = 127 */
    { "magic-missile-immunity",       1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGIC_MISSILE_IMMUNITY = 128 */
    { "stun-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* STUN_RESISTANCE = 129 */
    { "food-poisoned",                0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* FOOD_POISONED = 130 */
    { "bisection-resistance",         0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* BISECTION_RESISTANCE = 131 */
    { "divine-endurance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_ENDURANCE = 132 */
    { "divine-intellect",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_INTELLECT = 133 */
    { "divine-wisdon",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_WISDOM = 134 */
    { "divine-dexterity",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_DEXTERITY = 135 */
    { "divine-charisma",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_CHARISMA = 136 */
    { "mummy-rot",                    0, 0, 1, 50, 50,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MUMMY_ROT = 137 */
    { "fire-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* FIRE_RESISTANCE = 138 */
    { "cold-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* COLD_RESISTANCE = 139 */
    { "shock-resistance",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SHOCK_RESISTANCE = 140 */
    { "magic-missile-resistance",     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MAGIC_MISSILE_RESISTANCE = 141 */
    { "acid-resistance",              1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ACID_RESISTANCE = 142 */
    { "ultra-fast",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* ULTRA_FAST = 143 */
    { "super-fast",                   1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SUPER_FAST = 144 */
    { "lightning-fast",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* LIGHTNING_FAST = 145 */
    { "heroism",                      1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HEROISM = 146 */
    { "super-heroism",                1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* SUPER_HEROISM = 147 */
    { "rapid-regeneration",           1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPID_REGENERATION = 148 */
    { "rapider-regeneration",         1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPIDER_REGENERATION = 149 */
    { "rapidest-regeneration",        1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPIDEST_REGENERATION = 150 */
    { "rapid-energy-regeneration",    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPID_ENERGY_REGENERATION = 151 */
    { "rapider-energy-regeneration",  1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPIDER_ENERGY_REGENERATION = 152 */
    { "rapidest-energy-regeneration", 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* RAPIDEST_ENERGY_REGENERATION = 153 */
    { "melee-life-leech",             1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* MELEE_LIFE_LEECH = 154 */
    { "crazed",                       1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* CRAZED = 155 */
    { "divine-regeneration",          1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* DIVINE_REGENERATION = 156 */
    { "improved-fire-resistance",     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_FIRE_RESISTANCE = 157 */
    { "improved-cold-resistance",     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_COLD_RESISTANCE = 158 */
    { "improved-shock-resistance",    1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_SHOCK_RESISTANCE = 159 */
    { "improved-magic-missile-resistance", 1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_MAGIC_MISSILE_RESISTANCE = 160 */
    { "improved-acid-resistance",     1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* IMPROVED_ACID_RESISTANCE = 161 */
    { "undead-immobility",            1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* UNDEAD_IMMOBILITY = 162 */
    { "undead-control",               1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* UNDEAD_CONTROL = 163 */
    { "cancellation-resistance",      0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* CANCELLATION_RESISTANCE = 164 */
    { "half-slow-digestion",          1, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE},  /* HALF_SLOW_DIGESTION = 165 */
    { "sliming-resistance",           0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_BUFF_CANCELLABLE },  /* SLIME_RESISTANCE = 166 */
    { "laughing",                     0, 0, 1,  0, 20,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE},  /* LAUGHING = 167 */
    { "tottering",                    0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE },  /* TOTTERING = 168 */
    { "martial-prowess",              0, 0, 0,  0, 0,  PCLR_WHITE, PCLR_NONE, PROPFLAGS_NONE },  /* MARTIAL_PROWESS = 169 */
};

const char* condition_names[NUM_BL_CONDITIONS] = {
    "Petrifying",
    "Slimed",
    "Being strangled",
    "Suffocating",
    "Food poisoned",
    "Terminally ill",
    "Blind",
    "Deaf",
    "Stun",
    "Confused",
    "Hallucinating",
    "Levitating",
    "Flying",
    "Riding",
    "Slowed",
    "Paralyzed",
    "Frightened",
    "Sleeping",
    "Cancelled",
    "Silenced",
    "Grabbed",
    "Mummy rot",
    "Lycanthropy",
};

const char* status_names[MAX_STATUS_MARKS] = {
    "Tame",
    "Peaceful",
    "Detected",
    "Object pile",
    "Satiated",
    "Hungry",
    "Weak",
    "Fainting",
    "Burdended",
    "Stressed",
    "Strained",
    "Overtaxed",
    "Overloaded",
    "Two-weapon fighting",
    "Skill available",
    "Saddled",
    "Low hit points",
    "Critical hit points",
    "Cooldown",
    "Trapped",
    "Grabber",
    "Carrying object",
    "Peaceful townguard",
    "Hostile townguard",
};

enum game_ui_status_mark_types statusmarkorder[MAX_STATUS_MARKS] = { STATUS_MARK_TOWNGUARD_PEACEFUL, STATUS_MARK_TOWNGUARD_HOSTILE, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };

void
props_init()
{
    /* Force linkage */
    return;
}

const char* 
get_property_name(prop_index)
int prop_index;
{
    int idx;
    for (idx = 0; propertynames[idx].prop_num; idx++)
    {
        if (propertynames[idx].prop_num == prop_index)
        {
            return propertynames[idx].prop_noun;
        }
    }

    return "";
}

struct propname
get_property_name_ex(prop_index)
int prop_index;
{
    int idx;
    for (idx = 0; propertynames[idx].prop_num; idx++)
    {
        if (propertynames[idx].prop_num == prop_index)
        {
            return propertynames[idx];
        }
    }

    struct propname res = { 0 };
    return res;
}


const char*
get_status_name(mtmp, status_index)
struct monst* mtmp;
int status_index;
{
    switch (status_index)
    {
    case STATUS_MARK_PET:
        if (!call_mon_tame(mtmp))
            return "Ally";
        break;
    default:
        break;
    }
    return status_names[status_index];
}

/* prop.c */
