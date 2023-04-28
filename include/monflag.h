/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-28 */

/* GnollHack 4.0    monflag.h    $NHDT-Date: 1432512778 2015/05/25 00:12:58 $  $NHDT-Branch: master $:$NHDT-Revision: 1.14 $ */
/* Copyright (c) 1989 Mike Threepoint                  */
/* GnollHack may be freely redistributed.  See license for details. */

#ifndef MONFLAG_H
#define MONFLAG_H

#define MS_SILENT 0     /* makes no sound */
#define MS_BARK 1       /* if full moon, may howl */
#define MS_MEW 2        /* mews or hisses */
#define MS_ROAR 3       /* roars */
#define MS_GROWL 4      /* growls */
#define MS_SQEEK 5      /* squeaks, as a rodent */
#define MS_SQAWK 6      /* squawks, as a bird */
#define MS_HISS 7       /* hisses */
#define MS_BUZZ 8       /* buzzes (killer bee) */
#define MS_GRUNT 9      /* grunts (or speaks own language) */
#define MS_NEIGH 10     /* neighs, as an equine */
#define MS_BLEAT 11     /* bleats like a sheep or ram */
#define MS_MOO 12       /* moos like a cow */
#define MS_WAIL 13      /* wails, as a tortured soul */
#define MS_GURGLE 14    /* gurgles, as liquid or through saliva */
#define MS_BURBLE 15    /* burbles (jabberwock) */
#define MS_ANIMAL 15    /* up to here are animal noises */
#define MS_SHRIEK 17    /* wakes up others */
#define MS_BONES 18     /* rattles bones (skeleton) */
#define MS_LAUGH 19     /* grins, smiles, giggles, and laughs */
#define MS_MUMBLE 20    /* says something or other */
#define MS_IMITATE 21   /* imitates others (leocrotta) */
#define MS_ORC MS_GRUNT /* intelligent brutes */
#define MS_HUMANOID 22  /* generic traveling companion */
#define MS_ARREST 23    /* "Stop in the name of the law!" (Kops) */
#define MS_SOLDIER 24   /* army and watchmen expressions */
#define MS_GUARD 25     /* "Please drop that gold and follow me." */
#define MS_DJINNI 26    /* "Thank you for freeing me!" */
#define MS_NURSE 27     /* "Take off your shirt, please." */
#define MS_SEDUCE 28    /* "Hello, sailor." (Nymphs) */
#define MS_VAMPIRE 29   /* vampiric seduction, Vlad's exclamations */
#define MS_BRIBE 30     /* asks for money, or berates you */
#define MS_CUSS 31      /* berates (demons) or intimidates (Wiz) */
#define MS_RIDER 32     /* astral level special monsters */
#define MS_LEADER 33    /* your class leader */
#define MS_NEMESIS 34   /* your nemesis */
#define MS_GUARDIAN 35  /* your leader's guards */
#define MS_SELL 36      /* demand payment, complain about shoplifters */
#define MS_ORACLE 37    /* do a consultation */
#define MS_PRIEST 38    /* ask for contribution; do cleansing */
#define MS_SPELL 39     /* spellcaster not matching any of the above */
#define MS_WERE 40      /* lycanthrope in human form */
#define MS_BOAST 41     /* giants */
#define MS_SMITH 42     /* offer artisan services */
#define MS_NPC 43       /* offer artisan services */
#define MS_MODRON 44    /* modron talk */
#define MS_ALEAX 45     /* tells you that you are being punished by your god */
#define MS_PRISONER 46  /* "Thank you for freeing me!" */
#define MS_WATER_DEMON 47 /* "Thank you for freeing me!" */
#define MS_QUANTUM 48   /* speaks about quantum mechanics and complains about disturbing his/her underground experiments */

/* resistances, and all intrinsics and extrinsics the monster can have */
#define MR_NONE                    0x00000000UL
#define MR_FIRE                    0x00000001UL   /* resists fire */
#define MR_COLD                    0x00000002UL   /* resists cold */
#define MR_SLEEP                   0x00000004UL   /* resists sleep */
#define MR_DISINT                  0x00000008UL   /* resists disintegration */
#define MR_ELEC                    0x00000010UL   /* resists electricity */
#define MR_POISON                  0x00000020UL   /* resists poison */
#define MR_ACID                    0x00000040UL   /* resists acid */
#define MR_STONE                   0x00000080UL   /* resists petrification */
#define MR_DEATH                   0x00000100UL   /* resists death */
#define MR_LYCANTHROPY             0x00000200UL   /* resists lycanthropy */
#define MR_MAGIC_MISSILE           0x00000400UL   /* resists magic missiles */
#define MR_MAGIC                   0x00000800UL   /* resists magic */
#define MR_CHARM                   0x00001000UL   /* resists charm */
#define MR_DRAIN                   0x00002000UL   /* resists life draining */
#define MR_FLASH                   0x00004000UL   /* resists flash */
#define MR_REFLECTING              0x00008000UL   /* reflecting */
#define MR_INVISIBLE               0x00010000UL   /* invisible */
#define MR_SEE_INVISIBLE           0x00020000UL   /* see invisible */
#define MR_REGENERATION            0x00040000UL        
#define MR_TELEPORT                0x00080000UL        
#define MR_TELEPORT_CONTROL        0x00100000UL        
#define MR_BLIND_TELEPATHY         0x00200000UL        
#define MR_TELEPATHY               0x00400000UL        
#define MR_FEAR                    0x00800000UL   /* immune to fear */
#define MR_SICK                    0x01000000UL   /* immune to sickness */
#define MR_FREE_ACTION             0x02000000UL   /* immune to paralysis */
#define MR_STUN                    0x04000000UL   /* immune to stun */
#define MR_LEVITATION_CONTROL      0x08000000UL        
#define MR_FIRE_VULNERABLITY       0x10000000UL        
#define MR_COLD_VULNERABLITY       0x20000000UL        
#define MR_SHOCK_VULNERABLITY      0x40000000UL        
#define MR_MAGIC_MISSILE_VULNERABLITY      0x80000000UL        

#define MR2_NONE                           0x00000000UL
#define MR2_FIRE_RESISTANCE                0x00000001UL        
#define MR2_COLD_RESISTANCE                0x00000002UL        
#define MR2_SHOCK_RESISTANCE               0x00000004UL        
#define MR2_MAGIC_MISSILE_RESISTANCE       0x00000008UL        
#define MR2_ACID_RESISTANCE                0x00000010UL        
#define MR2_CANCELLATION_RESISTANCE        0x00000020UL        
#define MR2_BISECTION_RESISTANCE           0x00000040UL        
#define MR2_SLIME_RESISTANCE               0x00000080UL        
#define MR2_ENERGY_REGENERATION            0x00000100UL        

/* conveyances: resistances, teleport, teleport control, telepathy, as well as other properties from eating the corpse */
#define MC_NONE            0x00000000UL
#define MC_FIRE            0x00000001UL   /* confers weak fire resistance */
#define MC_COLD            0x00000002UL   /* confers weak cold resistance */
#define MC_SLEEP           0x00000004UL   /* confers sleep resistance */
#define MC_DISINT          0x00000008UL   /* confers disintegration resistance */
#define MC_ELEC            0x00000010UL   /* confers weak shock resistance */
#define MC_POISON          0x00000020UL   /* confers poison resistance */
#define MC_ACID            0x00000040UL   /* confers weak acid resistance */
#define MC_STONE           0x00000080UL   /* confers petrification resistance */
#define MC_DEATH           0x00000100UL   /* confers death resistance */
#define MC_DRAIN           0x00000200UL   /* confers life draining resistance */
#define MC_CHARM           0x00000400UL   /* confers charm resistance */
#define MC_SICK            0x00000800UL   /* confers sickness resistance */
#define MC_FREE_ACTION     0x00001000UL   /* confers paralysis resistance */
#define MC_INVISIBILITY    0x00002000UL   /* confers invisibility */
#define MC_SEE_INVISIBLE   0x00004000UL   /* confers see invisible */
#define MC_STRENGTH        0x00008000UL   /* confers strength */
#define MC_DEXTERITY       0x00010000UL   /* confers dexterity */
#define MC_CONSTITUTION    0x00020000UL   /* confers constitution */
#define MC_INTELLIGENCE    0x00040000UL   /* confers intelligence */
#define MC_WISDOM          0x00080000UL   /* confers wisdom */
#define MC_CHARISMA        0x00100000UL   /* confers charisma */
#define MC_LEVEL_GAIN      0x00200000UL   /* confers experience levels */
#define MC_TELEPORT            0x00400000UL   /* confers teleportiis */
#define MC_TELEPORT_CONTROL    0x00800000UL   /* confers teleport control */
#define MC_BLIND_TELEPATHY     0x01000000UL   /* confers blind telepathy */
#define MC_TELEPATHY           0x02000000UL   /* confers telepathy */
/* edible effects */
#define MC_STUNNING_CORPSE         0x04000000UL    /* the corpse is stunning to eat */
#define MC_POISONOUS_CORPSE        0x08000000UL    /* the corpse is poisonous to eat */
#define MC_ACIDIC_CORPSE           0x10000000UL    /* the corpse is acidic to eat */
#define MC_HALLUCINATING_CORPSE    0x20000000UL    /* the corpse is hallucinating to eat */
#define MC_SICKENING_CORPSE        0x40000000UL    /* the corpse is sickening to eat */
#define MC_MUMMY_ROTTED_CORPSE     0x80000000UL    /* the corpse is infected with mummy rot */


#define M1_NONE            0x00000000UL
#define M1_FLY             0x00000001UL   /* can fly or float */
#define M1_SWIM            0x00000002UL   /* can traverse water */
#define M1_AMORPHOUS       0x00000004UL   /* can flow under doors */
#define M1_WALLWALK        0x00000008UL   /* can phase thru rock */
#define M1_CLING           0x00000010UL   /* can cling to ceiling */
#define M1_TUNNEL          0x00000020UL   /* can tunnel thru rock */
#define M1_NEEDPICK        0x00000040UL   /* needs pick to tunnel */
#define M1_CONCEAL         0x00000080UL   /* hides under objects */
#define M1_HIDE            0x00000100UL   /* mimics, blends in with ceiling */
#define M1_AMPHIBIOUS      0x00000200UL   /* can survive underwater */
#define M1_BREATHLESS      0x00000400UL   /* doesn't need to breathe */
#define M1_NOTAKE          0x00000800UL   /* cannot pick up objects */
#define M1_NOEYES          0x00001000UL   /* no eyes to gaze into or blind */
#define M1_NOHANDS         0x00002000UL   /* no hands to handle things */
#define M1_NOLIMBS         0x00006000UL   /* TWO BITS: no arms/legs to kick/wear on */
#define M1_NONECK          0x00008000UL   /* no head to behead */
#define M1_MINDLESS        0x00010000UL   /* has no mind--golem, zombie, mold */
#define M1_HUMANOID        0x00020000UL   /* has humanoid head/arms/torso */
#define M1_ANIMAL          0x00040000UL   /* has animal body */
#define M1_SLITHY          0x00080000UL   /* has serpent body */
#define M1_UNSOLID         0x00100000UL   /* has no solid or liquid body */
#define M1_THICK_HIDE      0x00200000UL   /* has thick hide or scales */
#define M1_OVIPAROUS       0x00400000UL   /* can lay eggs */
#define M1_FLOATER         0x00800000UL   /* can float */
#define M1_TELEKINETIC_OPERATOR \
                           0x01000000UL   /* can use telekinesis as hands for certain purposes (for gazer opening doors etc.) */
#define M1_TOUCH_PETRIFIES 0x02000000UL   /* touching the corpse petrifies */
#define M1_ROCKTHROW       0x04000000UL   /* throws boulders */
#define M1_STEED           0x08000000UL   /* can be saddled and ridden */
#define M1_RUST_CAUSING_IRONVORE \
                           0x10000000UL   /* eats iron and rustable materials only; requires M1_METALLIVORE to be set as well */
#define M1_CARNIVORE       0x20000000UL   /* eats corpses */
#define M1_HERBIVORE       0x40000000UL   /* eats fruits */
#define M1_OMNIVORE        0x60000000UL   /* eats both */
#ifdef NHSTDC
#define M1_METALLIVORE     0x80000000UL   /* eats metal */
#else
#define M1_METALLIVORE 0x80000000L /* eats metal */
#endif

#define M2_NONE            0x00000000UL
#define M2_NOPOLY          0x00000001UL   /* players mayn't poly into one */
#define M2_UNDEAD          0x00000002UL   /* is walking dead */
#define M2_WERE            0x00000004UL   /* is a lycanthrope */
#define M2_HUMAN           0x00000008UL   /* is a human */ /* THIS MUST START HERE BECAUSE OF MH_HUMAN */
#define M2_ELF             0x00000010UL   /* is an elf */
#define M2_DWARF           0x00000020UL   /* is a dwarf */
#define M2_GNOLL           0x00000040UL   /* is a gnoll */
#define M2_ORC             0x00000080UL   /* is an orc */
#define M2_GNOME           0x00000100UL   /* is a gnome */
#define M2_DEMON           0x00000200UL   /* is a demon */
#define M2_MODRON          0x00000400UL   /* is a modron */
#define M2_MIMIC           0x00000800UL   /* is a mimic */
#define M2_ELEMENTAL       0x00001000UL   /* is an elemental being */
#define M2_COCKATRICE      0x00002000UL   /* is a cockatrice */
#define M2_MINION          0x00004000UL   /* is a minion of a deity */
#define M2_GIANT           0x00008000UL   /* is a giant */
#define M2_SHAPESHIFTER    0x00010000UL   /* is a shapeshifting species */
#define M2_TENTACLED_ONE   0x00020000UL   /* is a tentacled one */
#define M2_ANGEL           0x00040000UL   /* is an angelic being */
#define M2_MALE            0x00080000UL   /* always male */
#define M2_FEMALE          0x00100000UL   /* always female */
#define M2_NEUTER          0x00200000UL   /* neither male nor female */
#define M2_HOSTILE         0x00400000UL   /* always starts hostile */
#define M2_PEACEFUL        0x00800000UL   /* always starts peaceful */
#define M2_DOMESTIC        0x01000000UL   /* can be tamed by feeding */
#define M2_WANDER          0x02000000UL   /* wanders randomly */
#define M2_STALK           0x04000000UL   /* follows you to other levels */
#define M2_NASTY           0x08000000UL   /* extra-nasty monster (more xp) */
#define M2_GREEDY          0x10000000UL   /* likes gold */
#define M2_JEWELS          0x20000000UL   /* likes gems */
#define M2_COLLECT         0x40000000UL   /* picks up weapons and food */
#ifdef NHSTDC
#define M2_MAGIC           0x80000000UL   /* picks up magic items */
#else
#define M2_MAGIC 0x80000000UL   /* picks up magic items */
#endif

#define M3_NONE                                        0x00000000UL
#define M3_WANTSAMUL                                   0x00000001UL   /* would like to steal the amulet */
#define M3_WANTSBELL                                   0x00000002UL   /* wants the bell */
#define M3_WANTSBOOK                                   0x00000004UL   /* wants the book */
#define M3_WANTSCAND                                   0x00000008UL   /* wants the candelabrum */
#define M3_WANTSARTI                                   0x00000010UL   /* wants the quest artifact */
#define M3_WANTSALL                                    0x0000001fUL   /* wants any major artifact */
#define M3_WAITFORU                                    0x00000040UL   /* waits to see you or get attacked */
#define M3_CLOSE                                       0x00000080UL   /* lets you close unless attacked */

#define M3_COVETOUS                                    0x0000000fUL   /* wants something (earlier the same as WANTSALL) */
#define M3_WAITMASK                                    0x000000c0UL   /* waiting... */

/* Infravision is currently implemented for players only */
#define M3_INFRAVISION                                 0x00000100UL   /* has infravision */
#define M3_INFRAVISIBLE                                0x00000200UL   /* visible by infravision */

#define M3_DISPLACES                                   0x00000400UL   /* moves monsters out of its way */
#define M3_INCORPOREAL                                 0x00000800UL   /* is incorporeal, different from unsolid in the sense that
                                                                         an unsolid monster can be disintegrated, but is_incorporeal not */
#define M3_NONLIVING                                   0x00001000UL   /* is nonliving, such as a golem or other construct or a vortex (unaffected by death attacks)*/
#define M3_MULTIWEAPON                                 0x00002000UL   /* fights with more than one weapon (i.e., the wielded weapon), if available*/
#define M3_CONSTRICTOR                                 0x00004000UL   /* grabbing / sticking itself causes constriction */
#define M3_STRANGLES_NECK_BY_HAND                      0x00008000UL   /* rope golem's strangulation attack */

#define M3_BURNS_COMPLETELY                            0x00010000UL   /* burns completely */

#define M3_SPEAKING                                    0x00020000UL   /* can be spoken to using the chat command */
#define M3_CHATTY                                      0x00040000UL   /* speaks even if hostile */
#define M3_CHAT_CAN_JOIN_PARTY                         0x00080000UL   /* can join the player's party */
/* free bit */
/* free bit */
/* free bit */
/* free bit */
#define M3_REGENERATES_LOST_BODY_PARTS                 0x01000000UL   /* does not lose max hitpoint because of sharpness or bisection */
#define M3_REVIVES_FROM_DEAD                           0x02000000UL   /* is resurrects itself after a while */

#define M3_WEBMAKER                                    0x04000000UL   /* is a webmaker */
#define M3_KNOWS_TRAPS                                 0x08000000UL   /* knows traps on the level */
#define M3_KNOWS_PITS_AND_HOLES                        0x10000000UL   /* knows pits and holes on the level */

#define M3_RANDOM_CORPSE                               0x20000000UL   /* leaves a corpse at random intervals */
#define M3_CORPSE_CRUMBLES_TO_DUST                     0x40000000UL   /* corpse crumbles to dust, also leaving no corpse */
#define M3_HUG_HITS_IF_TWO_FIRST_ATTACKS_SUCCEEDED     0x80000000UL   /* corpse crumbles to dust, also leaving no corpse */


#define M4_NONE                        0x00000000UL
#define M4_BRAVE                       0x00000001UL   /* disregards enemy strength */
#define M4_FEARLESS                    0x00000002UL   /* disregards own health */
#define M4_BLOODLUST                   0x00000004UL   /* attacks peaceful */

#define M4_SPLITS_UPON_HIT             0x00000008UL   /* brown and black puddings */
#define M4_VEGETARIAN_FOOD             0x00000010UL   /* puddings and other monsters for a vegetarian */
#define M4_VEGAN_FOOD                  0x00000020UL   /* monsters for a vegan */
#define M4_NONROTTING_CORPSE           0x00000040UL   /* corpse does not rot */

#define M4_SMELLS_BURIED_SEARCHABLE    0x00000080UL   /* can smell buried objects close to the surface of the ground such as roots */
#define M4_PITWALK                     0x00000100UL   /* can walk in and out of pits without trouble */
#define M4_IGNORES_ELBERETH            0x00000200UL   /* is unaffected by Elbereth like a minotaur */
#define M4_SLURPS_ITEMS                0x00000400UL   /* hoovers up items like a gelatinous cube */
#define M4_STONY                       0x00000800UL   /* is made of stone, sand or the like, gets double damage from spade of colossal excavation */
#define M4_WOODEN                      0x00001000UL   /* is made of wooden, extra damage from axes */
#define M4_FLAMING                     0x00002000UL   /* a flaming monster */
#define M4_WHIRLY                      0x00004000UL   /* a whirly monster */
#define M4_BAT                         0x00008000UL   /* a bat */
#define M4_LIKES_LAVA                  0x00010000UL   /* likes lava */
#define M4_LIKES_FIRE                  0x00020000UL   /* likes fire */
#define M4_VORPAL_VULNERABILITY        0x00040000UL   /* vorpal weapons always cut off head */
#define M4_LOVES_BANANAS               0x00080000UL   /* tamed with bananas */
#define M4_PAWED                       0x00100000UL   /* has paws instead of hands */
#define M4_WATERY                      0x00200000UL   /* composed of water like a water elemental */
#define M4_FAITHFUL                    0x00400000UL   /* always faithful is tame */
#define M4_BEAR                        0x00800000UL   /* a bear or bear-like monster */
#define M4_BIRD                        0x01000000UL   /* a bird */
#define M4_PROPER_NAME                 0x02000000UL   /* monster name is a proper name */
#define M4_MERC                        0x04000000UL   /* is a guard or soldier */
#define M4_LORD                        0x08000000UL   /* is a lord to its kind */
#define M4_PRINCE                      0x10000000UL   /* is an overlord to its kind */
#define M4_PACKMULE                    0x20000000UL   /* carries items for the player */
#define M4_IRON                        0x40000000UL   /* is made of iron (can rust) */
#define M4_METAL                       0x80000000UL   /* is made of metal other than iron */

#define M5_NONE                        0x00000000UL
/* Must be in the same order as enum action_tile_types */
#define M5_ATTACK_TILE                 0x00000001UL
#define M5_THROW_TILE                  0x00000002UL
#define M5_FIRE_TILE                   0x00000004UL
#define M5_CAST_NODIR_TILE             0x00000008UL
#define M5_CAST_DIR_TILE               0x00000010UL
#define M5_SPECIAL_ATTACK_TILE         0x00000020UL
#define M5_KICK_TILE                   0x00000040UL
#define M5_PASSIVE_DEFENSE_TILE        0x00000080UL
#define M5_SPECIAL_ATTACK_2_TILE       0x00000100UL
#define M5_SPECIAL_ATTACK_3_TILE       0x00000200UL
#define M5_ITEM_USE_TILE               0x00000400UL
#define M5_DOOR_USE_TILE               0x00000800UL
#define M5_DEATH_TILE                  0x00001000UL
/* Other tile flags */
#define M5_STATUE_TILE                 0x00002000UL   /* Unused so far */
#define M5_CORPSE_TILE                 0x00004000UL
#define M5_FEMALE_TILE                 0x00008000UL

/* Some special tile flags */
#define M5_FULL_SIZED_CORPSE_TILE           0x00010000UL   /* Default is half size */
#define M5_FULL_SIZED_FEMALE_CORPSE_TILE    0x00020000UL   /* Default is half size */
#define M5_HALF_SIZED_MONSTER_TILE          0x00040000UL   /* Upper half of the tile is empty, so one can draw in menus just the lower half */
#define M5_TWO_WIDE_CENTERED_MONSTER_TILE   0x00080000UL   /* The monster covers three tiles, but is in fact only two tiles wide, leaving half tile from left and right empty, so one can draw just the center tile, right half of left tile, and left half of right tile. */
#define M5_TWO_WIDE_CENTERED_CORPSE_TILE    0x00100000UL   /* The corpse covers three tiles, but is in fact only two tiles wide, leaving half tile from left and right empty, so one can draw just the center tile, right half of left tile, and left half of right tile. */
/* Free bit */

/* Remaining bits are normal monster flags */
#define M5_ESCHEWS_CURSED_ITEMS     0x00400000UL
#define M5_ESCHEWS_BLESSED_ITEMS    0x00800000UL
#define M5_ESCHEWS_SILVER_ITEMS     0x01000000UL
#define M5_HATES_LIGHT              0x02000000UL   /* Gremlin only */
#define M5_HATES_SILVER             0x04000000UL   /* In addition to lycanthropes, demons, and vampires, e.g. non-tengu imps and shade */
#define M5_MAKES_SOUND_RANDOMLY     0x08000000UL
#define M5_SEMI_TRANSPARENT         0x10000000UL   /* Tile drawn with some transparency (default: 50% constant) */
#define M5_RADIAL_TRANSPARENCY      0x20000000UL   /* Tile drawn with radial transparency (0% at center and 96% at radius of 32 and 96% thereafter) */
#define M5_GLASS_TRANSPARENT        0x40000000UL   /* Tile drawn with 35% transparency */
/* Free bit */

#define M6_NONE                     0x00000000UL
#define M6_NON_EATER                0x00000001UL   /* Living beings like elementals */
#define M6_CORPSE_EATER             0x00000002UL   /* Overrides undead or non_eater flags for corpses, e.g., ghoul */
#define M6_LONG_WORM                0x00000004UL   /* Any sort of long worm (tailed or not, including tail) */
#define M6_TAILED_LONG_WORM         0x00000008UL   /* Multi-square long worm, a head with a tail */
#define M6_PURPLE_WORM              0x00000010UL   /* Any sort of purple worm (baby or normal) */
#define M6_PEACEFUL_AUTO_JOIN       0x00000020UL
#define M6_PEACEFUL_SPECIAL_SHOUT   0x00000040UL
#define M6_HATCHLING                0x00000080UL   /* Elder version of the base monster; for eggs do little_to_big */
#define M6_ELDER                    0x00000100UL   /* Elder version of the base monster; for eggs do big_to_little */
#define M6_QUANTUM_MECHANIC         0x00000200UL   /* Is a quantum mechanic */
#define M6_LONG_WORM_TAIL           0x00000400UL   /* Tail of a long worm */
#define M6_HELL_HOUND               0x00000800UL   /* Is a hell hound */
#define M6_BOSS_MONSTER             0x00001000UL   /* Seeing the monster triggers a boss fight */
#define M6_MAY_START_WITH_SADDLE    0x00002000UL   /* If steed, then may be generated with saddle */
#define M6_SHADE                    0x00004000UL   /* Shade only */
#define M6_TELEPORT_HEAL_TACTICS    0x00008000UL   /* Arch-lich, Wizard of Yendor, Demogorgon, and various other monsters */
#define M6_NON_TINNABLE             0x00010000UL   /* Corpse cannot be tinned similar to riders */
#define M6_NON_EDIBLE               0x00020000UL   /* Corpse cannot be eaten similar to riders */
#define M6_REVIVES_UPON_MEDDLING    0x00040000UL   /* Revive upon corpse meddling */
#define M6_LITHOVORE                0x00080000UL   /* Eats rock */
#define M6_USES_DOG_SUBTYPES        0x00100000UL   /* Can have dog breeds */

#define M7_NONE                     0x00000000UL
#define M7_ARCHAEOLOGIST            0x00000001UL   /* monster can use archaeologist items */
#define M7_BARBARIAN                0x00000002UL   /* monster can use barbarian items */
#define M7_CAVEMAN                  0x00000004UL   /* monster can use caveman items */
#define M7_HEALER                   0x00000008UL   /* monster can use healer items */
#define M7_KNIGHT                   0x00000010UL   /* monster can use knight items */
#define M7_MONK                     0x00000020UL   /* monster can use monk items */
#define M7_PRIEST                   0x00000040UL   /* monster can use priest items */
#define M7_ROGUE                    0x00000080UL   /* monster can use rogue items */
#define M7_RANGER                   0x00000100UL   /* monster can use ranger items */
#define M7_SAMURAI                  0x00000200UL   /* monster can use samurai items */
#define M7_TOURIST                  0x00000400UL   /* monster can use tourist items */
#define M7_VALKYRIE                 0x00000800UL   /* monster can use valkyrie items */
#define M7_WIZARD                   0x00001000UL   /* monster can use wizard items */

#define M8_NONE                     0x00000000UL

#define MZ_TINY 0          /* < 2' */
#define MZ_SMALL 1         /* 2-4' */
#define MZ_MEDIUM 2        /* 4-7' */
#define MZ_HUMAN MZ_MEDIUM /* human-sized */
#define MZ_LARGE 3         /* 7-12' */
#define MZ_HUGE 4          /* 12-25' */
#define MZ_GIGANTIC 7      /* off the scale */

/* Monster races -- must stay within ROLE_RACEMASK */
/* Eventually this may become its own field */
#define MH_HUMAN M2_HUMAN
#define MH_ELF M2_ELF
#define MH_DWARF M2_DWARF
#define MH_GNOLL M2_GNOLL
#define MH_ORC M2_ORC
#define MH_GNOME M2_GNOME

/* for mons[].geno (constant during game) */
#define G_GENDER_MALE_RARE   0x80000000UL   /* female more frequent */
#define G_GENDER_ONE_256     0x70000000UL
#define G_GENDER_ONE_128     0x60000000UL
#define G_GENDER_ONE_64      0x50000000UL
#define G_GENDER_ONE_32      0x40000000UL
#define G_GENDER_ONE_16      0x30000000UL
#define G_GENDER_ONE_EIGTH   0x20000000UL
#define G_GENDER_ONE_FOURTH  0x10000000UL
#define G_GENDER_GEN_MASK    0x70000000UL   /* gender distribution mask, bits 0x10000000 - 0x40000000 give relative rarity: one in 2^(1 + number) is female or male */

#define G_NOMINES   0x00008000UL     /* not generated in Gnomish Mines */
#define G_YACC      0x00004000UL     /* generated on hellish pastures regardless of other flags */
#define G_MODRON    0x00002000UL     /* generated on modron plane regardless of other flags */
#define G_UNIQ      0x00001000UL     /* generated only once */
#define G_NOHELL    0x00000800UL     /* not generated in "hell" */
#define G_HELL      0x00000400UL     /* generated only in "hell" */
#define G_NOGEN     0x00000200UL     /* generated only specially */
#define G_STRAYED   0x00000100UL     /* generated only when the player has strayed from the his/her original alignment path */
#define G_GENO      0x00000080UL     /* can be genocided */
#define G_NOCORPSE  0x00000040UL     /* no corpse left ever */
/* Reserved */
/* Reserved */
#define G_FREQ      0x00000007UL     /* creation frequency mask */

/* OBSOLETE --JG */
#define G_SGROUP    0x00000000UL     /* 0x00000080UL appear in small groups normally */
#define G_LGROUP    0x00000000UL     /* 0x00000040UL appear in large groups normally */

/* for mvitals[].mvflags (variant during game) */
#define MV_EXTINCT      0x01 /* have been extinguished as population control */
#define MV_GENOCIDED    0x02 /* have been genocided */
#define MV_GONE (MV_GENOCIDED | MV_EXTINCT)
#define MV_KNOWN        0x04 /* have been encountered */
#define MV_KNOWS_EGG    0x08 /* player recognizes egg of this monster type */
#define MV_KNOWS_CORPSE 0x10 /* player recognizes corpse of this monster type */
#define MV_SELFIE_TAKEN 0x20 /* player has taken a selfie with this monster type */
#define MV_NOCORPSE     0x40 /* no corpse left ever, the same bit as G_NOCORPSE just in case */

#endif /* MONFLAG_H */
