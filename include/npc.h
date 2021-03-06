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
    MAX_NPC_SUBTYPES
};

#define DESERTED_NPC_ROOM MAX_NPC_SUBTYPES
#define UNSPECIFIED_NPC_ROOM MAX_NPC_SUBTYPES + 1

struct npc_subtype_definition {
    int mnum;
    enum npc_gehennom_types npc_gehennom_type;
    const char* npc_role_name;
    const char* room_name;
    schar min_appearance_depth;
    schar max_appearance_depth;
    int start_money_d, start_money_n, start_money_p;
    unsigned long service_flags;
    unsigned long general_flags;
};

extern struct npc_subtype_definition npc_subtype_definitions[MAX_NPC_SUBTYPES];

#define NPC_SERVICE_ENCHANT_ACCESSORY        0x00000001UL
#define NPC_SERVICE_RECHARGING               0x00000002UL
#define NPC_SERVICE_BUY_GEMS_AND_STONES      0x00000004UL
#define NPC_SERVICE_IDENTIFY_GEMS_AND_STONES 0x00000008UL
#define NPC_SERVICE_IDENTIFY_ACCESSORIES_AND_CHARGED_ITEMS  0x00000010UL
#define NPC_SERVICE_BUY_SPELLBOOKS           0x00000020UL
#define NPC_SERVICE_BLESSED_RECHARGING       0x00000040UL
#define NPC_SERVICE_BUY_DILITHIUM_CRYSTALS   0x00000080UL
#define NPC_SERVICE_IDENTIFY_GEMS_STONES_AND_CHARGED_ITEMS  0x00000100UL
#define NPC_SERVICE_BRANCH_PORTAL            0x00000200UL
#define NPC_SERVICE_TEACH_SPELL_LIGHTNING_BOLT              0x00001000UL
#define NPC_SERVICE_TEACH_SPELL_CONE_OF_COLD                0x00002000UL
#define NPC_SERVICE_TEACH_SPELL_FORCE_BOLT                  0x00004000UL
#define NPC_SERVICE_TEACH_SPECIAL_SPELLS                    0x00008000UL
#define NPC_SERVICE_TEACH_RANDOM_ARCANE_SPELLS              0x00010000UL
/*free bit */
/*free bit */
/*free bit */
#define NPC_SERVICE_TEACH_SPELLS_MASK                       0x000FF000UL

#define NPC_FLAGS_PARQUET_FLOOR             0x00000001UL
#define NPC_FLAGS_MARBLE_FLOOR              0x00000002UL
#define NPC_FLAGS_DOORS_CLOSED              0x00000004UL
#define NPC_FLAGS_LIGHTS_ON                 0x00000008UL
#define NPC_FLAGS_DOUBLE_MONEY_IN_HELL      0x00000010UL
#define NPC_FLAGS_NO_GENERATION             0x00000020UL
#define NPC_FLAGS_GEOLOGIST_ITEMS           0x00000040UL



#endif /* NPC_H */