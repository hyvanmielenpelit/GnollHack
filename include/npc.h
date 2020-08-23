/*
 * npc.h
 * Copyright 2020 by Janne Gustafsson
 */
#ifndef NPC_H
#define NPC_H

enum npc_gehennom_types {
    NPC_GEHENNOM_STANDARD = 0,
    NPC_GEHENNOM_UNDEAD_SPELLCASTER,
    NPC_GEHENNOM_DWARF_MUMMY,
    MAX_NPC_GEHENNOM_TYPES
};

enum npc_subtypes {
    NPC_ARTIFICER = 0,
    NPC_GEOLOGIST,
    MAX_NPC_SUBTYPES
};

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

#define NPC_FLAGS_PARQUET_FLOOR             0x00000001UL
#define NPC_FLAGS_MARBLE_FLOOR              0x00000002UL
#define NPC_FLAGS_DOORS_CLOSED              0x00000004UL
#define NPC_FLAGS_LIGHTS_ON                 0x00000008UL
#define NPC_FLAGS_DOUBLE_MONEY_IN_HELL      0x00000010UL



#endif /* NPC_H */