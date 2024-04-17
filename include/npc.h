/*
 * npc.h
 * Copyright (c) Janne Gustafsson, 2020
 */
#ifndef NPC_H
#define NPC_H

enum npc_gehennom_types {
    NPC_GEHENNOM_STANDARD = 0,
    NPC_GEHENNOM_UNDEAD_SPELLCASTER,
    NPC_GEHENNOM_DWARF_MUMMY,
    MAX_NPC_GEHENNOM_TYPES
};

enum npc_modron_plane_types {
    NPC_MODRON_PLANE_STANDARD = 0,
    NPC_MODRON_PLANE_MONK,
    NPC_MODRON_PLANE_ABBOT,
    MAX_NPC_MODRON_PLANE_TYPES
};

enum npc_subtypes {
    NPC_ARTIFICER = 0,
    NPC_GEOLOGIST,
    NPC_WARP_ENGINEER,
    NPC_HERMIT,
    NPC_HERMIT2,
    NPC_HERMIT3,
    NPC_ORC_HERMIT3,
    NPC_QUANTUM_MECHANIC,
    NPC_ELVEN_BARD,
    MAX_NPC_SUBTYPES
};

#define DESERTED_NPC_ROOM MAX_NPC_SUBTYPES
#define UNSPECIFIED_NPC_ROOM MAX_NPC_SUBTYPES + 1

struct npc_subtype_definition {
    int mnum;
    enum npc_gehennom_types npc_gehennom_type;
    const char* npc_role_name;
    const char* room_name;
    const char* npc_fixed_name;
    const char* npc_fixed_explanation;
    const char* revival_line;
    const char* distant_line[3];
    schar min_appearance_depth;
    schar max_appearance_depth;
    int start_money_d, start_money_n, start_money_p;
    boolean has_special_tileset;
    uchar special_tileset;
    unsigned long service_flags;
    unsigned long general_flags;
};

extern struct npc_subtype_definition npc_subtype_definitions[MAX_NPC_SUBTYPES];

#define NPC_SERVICE_ENCHANT_ACCESSORY                       0x00000001UL
#define NPC_SERVICE_RECHARGING                              0x00000002UL
#define NPC_SERVICE_BUY_GEMS_AND_STONES                     0x00000004UL
#define NPC_SERVICE_IDENTIFY_GEMS_AND_STONES                0x00000008UL
#define NPC_SERVICE_IDENTIFY_ACCESSORIES_AND_CHARGED_ITEMS  0x00000010UL
#define NPC_SERVICE_BUY_SPELLBOOKS                          0x00000020UL
#define NPC_SERVICE_BLESSED_RECHARGING                      0x00000040UL
#define NPC_SERVICE_BUY_DILITHIUM_CRYSTALS                  0x00000080UL
#define NPC_SERVICE_IDENTIFY_GEMS_STONES_AND_CHARGED_ITEMS  0x00000100UL
#define NPC_SERVICE_BRANCH_PORTAL                           0x00000200UL
#define NPC_SERVICE_FORGE_SLING_BULLETS                     0x00000400UL
#define NPC_SERVICE_FORGE_CUBIC_GATE                        0x00000800UL
/* free spell bit */
/* free spell bit */
/* free spell bit */
#define NPC_SERVICE_TEACH_SPECIAL_SPELLS                    0x00008000UL
#define NPC_SERVICE_TEACH_WIZARD_SPELLS                     0x00010000UL
/* free spell bit */
/* free spell bit */
/* free spell bit */
#define NPC_SERVICE_TEACH_SPELLS_MASK                       0x000FF000UL
#define NPC_SERVICE_GIVE_STARTING_QUESTS                    0x00100000UL
#define NPC_SERVICE_GIVE_ADVANCED_QUESTS                    0x00200000UL
#define NPC_SERVICE_GIVE_GNOMISH_QUESTS                     0x00400000UL
#define NPC_SERVICE_GIVE_ORCISH_QUESTS                      0x00800000UL
#define NPC_SERVICE_SPECIAL_NPC_HINTS                       0x01000000UL
#define NPC_SERVICE_SING_SONGS                              0x02000000UL
#define NPC_SERVICE_GIVE_QUANTUM_QUESTS                     0x04000000UL
#define NPC_SERVICE_FORGE_ARTIFICIAL_WINGS                  0x08000000UL

#define NPC_FLAGS_PARQUET_FLOOR             0x00000001UL
#define NPC_FLAGS_MARBLE_FLOOR              0x00000002UL
#define NPC_FLAGS_DOORS_CLOSED              0x00000004UL
#define NPC_FLAGS_LIGHTS_ON                 0x00000008UL
#define NPC_FLAGS_DOUBLE_MONEY_IN_HELL      0x00000010UL
#define NPC_FLAGS_NO_GENERATION             0x00000020UL
/* free flag */
#define NPC_FLAGS_NO_MY                     0x00000080UL
#define NPC_FLAGS_NO_ADVICE                 0x00000100UL
#define NPC_FLAGS_NO_ITEMS                  0x00000200UL
#define NPC_FLAGS_DISPLAY_NAME_ONLY         0x00000400UL /* When name is known */
#define NPC_FLAGS_COMMENTS_ON_REVIVAL       0x00000800UL
#define NPC_FLAGS_HAS_TRUE_RUMORS           0x00001000UL
#define NPC_FLAGS_NO_TITLE_ARTICLE          0x00002000UL
#define NPC_FLAGS_ALSO_NORMAL_HELLO         0x00004000UL
#define NPC_FLAGS_MALE                      0x00008000UL
#define NPC_FLAGS_FEMALE                    0x00010000UL
#define NPC_FLAGS_HAS_DISTANT_SOUND_1       0x00020000UL
#define NPC_FLAGS_HAS_DISTANT_SOUND_2       0x00040000UL
#define NPC_FLAGS_HAS_DISTANT_SOUND_3       0x00080000UL
#define NPC_FLAGS_HAS_STANDARD_DISTANT_SOUNDS       (NPC_FLAGS_HAS_DISTANT_SOUND_1 | NPC_FLAGS_HAS_DISTANT_SOUND_2)
#define NPC_FLAGS_HAS_ALL_DISTANT_SOUNDS            (NPC_FLAGS_HAS_DISTANT_SOUND_1 | NPC_FLAGS_HAS_DISTANT_SOUND_2 | NPC_FLAGS_HAS_DISTANT_SOUND_3)
#define NPC_FLAGS_MAY_HAVE_PAINTINGS        0x00100000UL
#define NPC_FLAGS_ALWAYS_HAS_PAINTING       0x00200000UL



#endif /* NPC_H */