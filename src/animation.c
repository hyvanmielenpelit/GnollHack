/*
 * Copyright (c) Janne Gustafsson, 2020
 */
#include "hack.h"

NEARDATA struct animation_definition animations[NUM_ANIMATIONS + 1] =
{
    { "", 0,
      0, 0, 0,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },

    { "handcrafted-candle-lit-animation",  1,
      HANDCRAFTED_CANDLE_ANIMATION_FRAMES, HANDCRAFTED_CANDLE_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "elf_wizard_female_cast-animation", 1,
      PLAYER_ELF_FEMALE_WIZARD_CAST_ANIMATION_FRAMES, PLAYER_ELF_FEMALE_WIZARD_CAST_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "brass-lantern-lit-animation",  1,
      BRASS_LANTERN_LIT_ANIMATION_FRAMES, BRASS_LANTERN_LIT_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "lawful-altar-animation",  1,
      LAWFUL_ALTAR_ANIMATION_FRAMES, LAWFUL_ALTAR_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "neutral-altar-animation",  1,
      NEUTRAL_ALTAR_ANIMATION_FRAMES, NEUTRAL_ALTAR_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "chaotic-altar-animation",  1,
      CHAOTIC_ALTAR_ANIMATION_FRAMES, CHAOTIC_ALTAR_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "look-cursor-animation",  2,
      LOOK_CURSOR_ANIMATION_FRAMES, LOOK_CURSOR_ANIMATION_OFF, 7,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, -1, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "kobold-animation",  1,
      KOBOLD_ANIMATION_FRAMES, KOBOLD_ANIMATION_OFF, 2,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "twisted-candle-lit-animation", 1,
      TWISTED_CANDLE_ANIMATION_FRAMES, TWISTED_CANDLE_ANIMATION_OFF, 3,
      ANIMATION_MAIN_TILE_USE_FIRST,
      AUTODRAW_NONE,
      { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    }
};

NEARDATA struct enlargement_definition enlargements[NUM_ENLARGEMENTS + 1] =
{
    { "", 0,
      0, 0,
      0, 0, 0,
      { 0, 1, 2, 3, 4 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    },

    { "dracolich-enlargement", 5,
      DRACOLICH_ENLARGEMENT_FRAMES, DRACOLICH_ENLARGEMENT_OFF,
      3, 2, 0,
      { 0, 1, 2, 3, 4 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    },
    { "dracolich-statue-enlargement", 5,
      DRACOLICH_STATUE_ENLARGEMENT_FRAMES, DRACOLICH_STATUE_ENLARGEMENT_OFF,
      3, 2, 0,
      { 0, 1, 2, 3, 4 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    },
    { "tree-enlargement", 5,
      TREE_ENLARGEMENT_FRAMES, TREE_ENLARGEMENT_OFF,
      3, 2, 0,
      { 0, 1, 2, 3, 4 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-upstairs-enlargement", 1,
      UPSTAIRS_ENLARGEMENT_FRAMES, UPSTAIRS_ENLARGEMENT_OFF,
      1, 2, 0,
      { -1, 0, -1, -1, -1 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-branch-upstairs-enlargement", 1,
      BRANCH_UPSTAIRS_ENLARGEMENT_FRAMES, BRANCH_UPSTAIRS_ENLARGEMENT_OFF,
      1, 2, 0,
      { -1, 0, -1, -1, -1 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-special-branch-upstairs-enlargement", 1,
      SPECIAL_BRANCH_UPSTAIRS_ENLARGEMENT_FRAMES, SPECIAL_BRANCH_UPSTAIRS_ENLARGEMENT_OFF,
      1, 2, 0,
      { -1, 0, -1, -1, -1 },
      { 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0 }
    }
};

NEARDATA struct replacement_definition replacements[NUM_REPLACEMENTS + 1] =
{
    { "",
      0, 0,
      0UL,
      REPLACEMENT_ACTION_NO_ACTION,
      AUTODRAW_NONE,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-stone-replacement",
      DUNGEON_NORMAL_STONE_REPLACEMENT_TILES, DUNGEON_NORMAL_STONE_REPLACEMENT_OFF,
      REPLACEMENT_EVENT_UPDATE_FROM_BELOW,
      REPLACEMENT_ACTION_BOTTOM_TILE,
      AUTODRAW_NONE,
      { "bottom-end", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-vwall-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_BELOW | REPLACEMENT_EVENT_UPDATE_FROM_TOP,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_UP_DOWN,
      { "bottom-end", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-crwall-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_BELOW | REPLACEMENT_EVENT_UPDATE_FROM_TOP | REPLACEMENT_EVENT_UPDATE_FROM_LEFT | REPLACEMENT_EVENT_UPDATE_FROM_RIGHT,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_ALL,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-trwall-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_BELOW | REPLACEMENT_EVENT_UPDATE_FROM_TOP | REPLACEMENT_EVENT_UPDATE_FROM_RIGHT,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_UP_DOWN_RIGHT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-tlwall-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_BELOW | REPLACEMENT_EVENT_UPDATE_FROM_TOP | REPLACEMENT_EVENT_UPDATE_FROM_LEFT,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_UP_DOWN_LEFT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-tuwall-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_LEFT | REPLACEMENT_EVENT_UPDATE_FROM_TOP | REPLACEMENT_EVENT_UPDATE_FROM_RIGHT,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_UP_LEFT_RIGHT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-tdwall-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_BELOW | REPLACEMENT_EVENT_UPDATE_FROM_LEFT | REPLACEMENT_EVENT_UPDATE_FROM_RIGHT,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_DOWN_LEFT_RIGHT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-hwall-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_LEFT | REPLACEMENT_EVENT_UPDATE_FROM_RIGHT,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_LEFT_RIGHT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-tlcorn-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_BELOW | REPLACEMENT_EVENT_UPDATE_FROM_RIGHT,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_DOWN_RIGHT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-trcorn-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_LEFT | REPLACEMENT_EVENT_UPDATE_FROM_BELOW,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_DOWN_LEFT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-blcorn-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_TOP | REPLACEMENT_EVENT_UPDATE_FROM_RIGHT,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_UP_RIGHT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "dungeon-normal-brcorn-replacement",
      0, 0,
      REPLACEMENT_EVENT_UPDATE_FROM_LEFT | REPLACEMENT_EVENT_UPDATE_FROM_TOP,
      REPLACEMENT_ACTION_AUTODRAW,
      AUTODRAW_DUNGEON_NORMAL_REPLACE_WALL_ENDS_UP_LEFT,
      { "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "coin-replacement",
      COIN_REPLACEMENT_TILES, COIN_REPLACEMENT_OFF,
      REPLACEMENT_EVENT_NO_EVENT,
      REPLACEMENT_ACTION_COIN_QUANTITY,
      AUTODRAW_NONE,
      { "few", "many", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "handcrafted-candle-replacement",
      HANDCRAFTED_CANDLE_LIT_TILES, HANDCRAFTED_CANDLE_LIT_OFF,
      REPLACEMENT_EVENT_NO_EVENT,
      REPLACEMENT_ACTION_LIT,
      AUTODRAW_NONE,
      { "lit", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { HANDCRAFTED_CANDLE_LIT_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "chest-replacement",
      CHEST_REPLACEMENT_TILES, CHEST_REPLACEMENT_OFF,
      REPLACEMENT_EVENT_NO_EVENT,
      REPLACEMENT_ACTION_CHEST,
      AUTODRAW_NONE,
      { "locked", "broken", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    },
    { "brass-lantern-replacement",
      BRASS_LANTERN_LIT_TILES, BRASS_LANTERN_LIT_OFF,
      REPLACEMENT_EVENT_NO_EVENT,
      REPLACEMENT_ACTION_LIT,
      AUTODRAW_NONE,
      { "lit", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
      { BRASS_LANTERN_LIT_ANIMATION, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    }
};


NEARDATA struct autodraw_definition autodraws[NUM_AUTODRAWS + 1] =
{
    {"", 0, 0, 0, 0, 0},
    {
        "dungeon-normal-wall-autodraw-all",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-up-down",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-up-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-up-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-up-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-down-left-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-down-down-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_DOWN,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-down-down-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_DOWN | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-down-up-right",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_RIGHT | AUTODRAW_DIR_UP,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
    {
        "dungeon-normal-wall-autodraw-down-up-left",
        AUTODRAW_DRAW_REPLACE_WALL_ENDS,
        AUTODRAW_DIR_UP | AUTODRAW_DIR_LEFT,
        0 + CRWALL_VARIATION_OFFSET + GLYPH_CMAP_VARIATION_OFF,
        S_vwall + GLYPH_CMAP_OFF,
        S_hwall + GLYPH_CMAP_OFF
    },
};


/* Game Cursors */
NEARDATA struct game_cursor_definition game_cursors[MAX_CURSORS] =
{
    {"generic", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT},
    {"look", NO_REPLACEMENT, LOOK_CURSOR_ANIMATION, NO_ENLARGEMENT },
    {"travel", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT},
    {"name", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT},
    {"teleport", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT},
    {"jump", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT},
    {"polearm", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT},
    {"grapple", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT},
    {"spell", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT},
    {"pay", NO_REPLACEMENT, NO_ANIMATION, NO_ENLARGEMENT}
};






void
init_animations()
{
    /* force linkage */
    return;
}

int
get_player_action_glyph_offset(action)
enum action_tile_types action;
{
    /* relies on aligned ordering of glyph offsets and action_tile_types */
    int res = GLYPH_PLAYER_OFF + action * NUM_PLAYER_CHARACTERS;
    return res;
}

int
get_monster_action_glyph_offset(action, genderidx)
enum action_tile_types action;
int genderidx;
{
    /* relies on aligned ordering of glyph offsets and action_tile_types */
    int res = (genderidx == 0 ? GLYPH_MON_OFF : GLYPH_FEMALE_MON_OFF) + action * NUM_MONSTERS;
    return res;
}

short
get_player_replacement(action, roleidx, raceidx, genderidx, alignmentidx, levelidx)
enum action_tile_types action;
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right replacement for the combination that has a replacement */
    switch (action)
    {
    case ACTION_TILE_NO_ACTION:
        break;
    case ACTION_TILE_ATTACK:
        break;
    case ACTION_TILE_THROW:
        break;
    case ACTION_TILE_FIRE:
        break;
    case ACTION_TILE_CAST:
        break;
    case ACTION_TILE_SPECIAL_ATTACK:
        break;
    case ACTION_TILE_KICK:
        break;
    case ACTION_TILE_ITEM_USE:
        break;
    case ACTION_TILE_DOOR_USE:
        break;
    case ACTION_TILE_DEATH:
        break;
    case MAX_ACTION_TILES:
        break;
    default:
        break;
    }
    return 0;
}

short
get_player_animation(action, roleidx, raceidx, genderidx, alignmentidx, levelidx)
enum action_tile_types action;
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right animation for the combination that has an animation */
    switch (action)
    {
    case ACTION_TILE_NO_ACTION:
        break;
    case ACTION_TILE_ATTACK:
        break;
    case ACTION_TILE_THROW:
        break;
    case ACTION_TILE_FIRE:
        break;
    case ACTION_TILE_CAST:
        if (roleidx == ROLE_WIZARD && raceidx == RACE_ELF && genderidx == GENDER_FEMALE)
            return PLAYER_ELF_FEMALE_WIZARD_CAST_ANIMATION;
        break;
    case ACTION_TILE_SPECIAL_ATTACK:
        break;
    case ACTION_TILE_KICK:
        break;
    case ACTION_TILE_ITEM_USE:
        break;
    case ACTION_TILE_DOOR_USE:
        break;
    case ACTION_TILE_DEATH:
        break;
    case MAX_ACTION_TILES:
        break;
    default:
        break;
    }
    return 0;
}

struct replacement_info
obj_to_replacement_info(otmp)
struct obj* otmp;
{
    struct replacement_info info = { 0 };
    info.object = otmp;

    return info;
}

short
maybe_get_replaced_tile(ntile, x, y, info, autodraw_ptr)
int x, y;
short ntile;
struct replacement_info info;
enum autodraw_types* autodraw_ptr;
{
#ifdef USE_TILES
    struct obj* otmp = info.object;
    short replacement_idx = tile2replacement[ntile];
    if (replacement_idx > 0)
    {
        int action_id = replacements[replacement_idx].replacement_action;
        switch (replacements[replacement_idx].replacement_action)
        {
        case REPLACEMENT_ACTION_BOTTOM_TILE:
        {
            int below_y = y + 1;
            if (!isok(x, below_y) || levl[x][below_y].hero_memory_layers.glyph == cmap_to_glyph(S_unexplored) || (IS_DOORJOIN(levl[x][below_y].typ) && !IS_TREE(levl[x][below_y].typ)) || levl[x][below_y].typ == DOOR || levl[x][below_y].typ == UNEXPLORED || (levl[x][y].seenv & (SV3 | SV4 | SV5 | SV6 | SV7)) == 0)
            {
                /* No action */
            }
            else
            {
                if(autodraw_ptr)
                    *autodraw_ptr = replacements[replacement_idx].general_autodraw;

                if (replacements[replacement_idx].number_of_tiles < 1)
                    return ntile;

                /* Return the first tile with index 0 */
                if (autodraw_ptr)
                    *autodraw_ptr = replacements[replacement_idx].tile_autodraw[0];
                return glyph2tile[0 + replacements[replacement_idx].glyph_offset + GLYPH_REPLACEMENT_OFF];
            }

            break;
        }
        case REPLACEMENT_ACTION_LIT:
        {
            if (!otmp)
                return ntile;

            if (is_obj_activated(otmp))
            {
                if (autodraw_ptr)
                    *autodraw_ptr = replacements[replacement_idx].general_autodraw;

                if (replacements[replacement_idx].number_of_tiles < 1)
                    return ntile;

                /* Return the first tile with index 0 */
                if (autodraw_ptr)
                    *autodraw_ptr = replacements[replacement_idx].tile_autodraw[0];
                return glyph2tile[0 + replacements[replacement_idx].glyph_offset + GLYPH_REPLACEMENT_OFF];
            }
            break;
        }
        case REPLACEMENT_ACTION_COIN_QUANTITY:
        {
            if (!otmp)
                return ntile;

            if (autodraw_ptr)
                *autodraw_ptr = replacements[replacement_idx].general_autodraw;

            if (replacements[replacement_idx].number_of_tiles < 1)
                return ntile;

            if (otmp->quan > 1)
            {
                int glyph_idx = (otmp->quan <= 6 ? 0 : 1);
                if (autodraw_ptr)
                    *autodraw_ptr = replacements[replacement_idx].tile_autodraw[glyph_idx];
                return glyph2tile[glyph_idx + replacements[replacement_idx].glyph_offset + GLYPH_REPLACEMENT_OFF];
            }
            break;
        }
        case REPLACEMENT_ACTION_CHEST:
        {
            if (!otmp)
                return ntile;

            if (autodraw_ptr)
                *autodraw_ptr = replacements[replacement_idx].general_autodraw;

            if (replacements[replacement_idx].number_of_tiles < 1)
                return ntile;

            if (otmp->olocked)
            {
                int glyph_idx = 0;
                if (autodraw_ptr)
                    *autodraw_ptr = replacements[replacement_idx].tile_autodraw[glyph_idx];
                return glyph2tile[glyph_idx + replacements[replacement_idx].glyph_offset + GLYPH_REPLACEMENT_OFF];
            }
            else if (otmp->obroken)
            {
                int glyph_idx = 1;
                if (autodraw_ptr)
                    *autodraw_ptr = replacements[replacement_idx].tile_autodraw[glyph_idx];
                return glyph2tile[glyph_idx + replacements[replacement_idx].glyph_offset + GLYPH_REPLACEMENT_OFF];
            }
            break;
        }
        case REPLACEMENT_ACTION_AUTODRAW:
        {
            if (autodraw_ptr)
                *autodraw_ptr = replacements[replacement_idx].general_autodraw;
            break;
        }
        default:
            break;
        }
    }
#endif
    return ntile;
}



short
maybe_get_animated_tile(ntile, interval_counter, mapAnimated, autodraw_ptr)
short ntile;
unsigned long interval_counter;
boolean* mapAnimated;
enum autodraw_types* autodraw_ptr;
{
#ifdef USE_TILES
    short animation_idx = tile2animation[ntile];
    if (animation_idx > 0)
    {
        if (autodraw_ptr)
            *autodraw_ptr = animations[animation_idx].main_tile_autodraw;
        if (animations[animation_idx].number_of_frames < 1)
            return ntile;

        if (mapAnimated)
            *mapAnimated = TRUE;

        char main_tile_frame_position = -1; /* ignore */
        if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_USE_FIRST)
        {
            main_tile_frame_position = 0;
        }
        else if (animations[animation_idx].main_tile_use_style == ANIMATION_MAIN_TILE_USE_LAST)
        {
            main_tile_frame_position = animations[animation_idx].number_of_frames;
        }
        char additional_tile_num = (main_tile_frame_position > -1 ? 1 : 0);
        char animation_tile_offset = (main_tile_frame_position == 0 ? 1 : 0);
        unsigned long numframes = (unsigned long)(animations[animation_idx].number_of_frames + additional_tile_num); /* add original tile as the first tile and frame */
        char current_animation_frame = (char)((interval_counter / (unsigned long)animations[animation_idx].intervals_between_frames) % numframes);
        
        if (current_animation_frame > main_tile_frame_position) /* 0 is the original picture */
        {
            char animation_frame_index = current_animation_frame - animation_tile_offset;
            if (animation_frame_index >= animations[animation_idx].number_of_frames)
            {
                return ntile; /* original tile is the last if number_of_frames is exceeded by numframes */
            }

            if (autodraw_ptr)
                *autodraw_ptr = animations[animation_idx].frame_autodraw[animation_frame_index];

            int animation_glyph = (int)animation_frame_index + animations[animation_idx].glyph_offset + GLYPH_ANIMATION_OFF;
            short res = glyph2tile[animation_glyph]; /* animated version selected */
            return res;
        }
    }
#endif
    return ntile;
}




short
get_player_enlargement(action, roleidx, raceidx, genderidx, alignmentidx, levelidx)
enum action_tile_types action;
int roleidx, raceidx, genderidx, alignmentidx, levelidx;
{
    /* Write here the code that returns the right enlargement for the combination that has an enlargement */
    switch (action)
    {
    case ACTION_TILE_NO_ACTION:
        break;
    case ACTION_TILE_ATTACK:
        break;
    case ACTION_TILE_THROW:
        break;
    case ACTION_TILE_FIRE:
        break;
    case ACTION_TILE_CAST:
        break;
    case ACTION_TILE_SPECIAL_ATTACK:
        break;
    case ACTION_TILE_KICK:
        break;
    case ACTION_TILE_ITEM_USE:
        break;
    case ACTION_TILE_DOOR_USE:
        break;
    case ACTION_TILE_DEATH:
        break;
    case MAX_ACTION_TILES:
        break;
    default:
        break;
    }
    return 0;
}


short
get_animation_base_tile(animidx)
short animidx;
{
    for (int i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].animation.actions[action] == animidx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 0)];
        }
        if (mons[i].animation.statue == animidx)
            return glyph2tile[i + GLYPH_STATUE_OFF];
        if (mons[i].animation.corpse == animidx)
            return glyph2tile[i + GLYPH_BODY_OFF];

        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].female_animation.actions[action] == animidx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 1)];
        }
        if (mons[i].female_animation.corpse == animidx)
            return glyph2tile[i + GLYPH_FEMALE_BODY_OFF];
        if (mons[i].female_animation.statue == animidx)
            return glyph2tile[i + GLYPH_FEMALE_STATUE_OFF];
    }

    for (int i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
    {
        if(obj_descr[objects[i].oc_descr_idx].stand_animation == animidx)
            return glyph2tile[i + GLYPH_OBJ_OFF];
    }

    for (int cmap_idx = 0; cmap_idx < CMAP_TYPE_MAX; cmap_idx++)
    {
        for (int i = 0; i < MAX_CMAPPED_CHARS; i++)
        {
            if (defsyms[i].stand_animation[cmap_idx] == animidx)
                return glyph2tile[i + GLYPH_CMAP_OFF];
        }

        for (int i = 0; i < MAX_VARIATIONS; i++)
        {
            if (defsym_variations[i].stand_animation[cmap_idx] == animidx)
                return glyph2tile[i + GLYPH_CMAP_VARIATION_OFF];
        }
    }

    for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
    {
        for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
        {
            for (int genderidx = 0; genderidx <= 1; genderidx++)
            {
                for (int alignment = -1; alignment <= 1; alignment++)
                {
                    for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                    {
                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, genderidx, alignment, level);
                        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                        {
                            if (get_player_animation(action, roleidx, raceidx, genderidx, alignment, level) == animidx)
                                return glyph2tile[player_glyph_index + get_player_action_glyph_offset(action)];
                        }
                    }
                }
            }
        }
    }

    /* Cursors */
    for (enum game_cursor_types i = CURSOR_STYLE_GENERIC_CURSOR; i < MAX_CURSORS; i++)
    {
        if (game_cursors[i].animation == animidx)
            return glyph2tile[i + GLYPH_CURSOR_OFF];
    }

    /* UI Tiles */
    for (enum game_ui_tile_types i = DEATH_TILE; i < MAX_UI_TILES; i++)
    {
        if (ui_tile_component_array[i].animation == animidx)
            return glyph2tile[i + GLYPH_UI_TILE_OFF];
    }

    /* Replacement */
    for (int i = 1; i <= NUM_REPLACEMENTS; i++)
    {
        for (int j = 0; j < replacements[i].number_of_tiles; j++)
        {
            if (replacements[i].tile_animation[j] == animidx)
                return glyph2tile[j + replacements[i].glyph_offset + GLYPH_REPLACEMENT_OFF];
        }
    }

    return -1;
}


short
get_enlargement_base_tile(enlidx)
short enlidx;
{
    for (int i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].enlargement.actions[action] == enlidx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 0)];
        }
        if (mons[i].enlargement.statue == enlidx)
            return glyph2tile[i + GLYPH_STATUE_OFF];
        if (mons[i].enlargement.corpse == enlidx)
            return glyph2tile[i + GLYPH_BODY_OFF];

        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].female_enlargement.actions[action] == enlidx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 1)];
        }
        if (mons[i].female_enlargement.statue == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_STATUE_OFF];
        if (mons[i].female_enlargement.corpse == enlidx)
            return glyph2tile[i + GLYPH_FEMALE_BODY_OFF];
    }

    for (int i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
    {
        if (obj_descr[objects[i].oc_descr_idx].enlargement == enlidx)
            return glyph2tile[i + GLYPH_OBJ_OFF];
    }

    for (int cmap_idx = 0; cmap_idx < CMAP_TYPE_MAX; cmap_idx++)
    {

        for (int i = 0; i < MAX_CMAPPED_CHARS; i++)
        {
            if (defsyms[i].enlargement[cmap_idx] == enlidx)
                return glyph2tile[i + GLYPH_CMAP_OFF];
        }

        for (int i = 0; i < MAX_VARIATIONS; i++)
        {
            if (defsym_variations[i].enlargement[cmap_idx] == enlidx)
                return glyph2tile[i + GLYPH_CMAP_VARIATION_OFF];
        }
    }
    for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
    {
        for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
        {
            for (int genderidx = 0; genderidx <= 1; genderidx++)
            {
                for (int alignment = -1; alignment <= 1; alignment++)
                {
                    for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                    {
                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, genderidx, alignment, level);
                        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                        {
                            if (get_player_enlargement(action, roleidx, raceidx, genderidx, alignment, level) == enlidx)
                                return glyph2tile[player_glyph_index + get_player_action_glyph_offset(action)];
                        }
                    }
                }
            }
        }
    }

    for (enum game_cursor_types i = CURSOR_STYLE_GENERIC_CURSOR; i < MAX_CURSORS; i++)
    {
        if (game_cursors[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_CURSOR_OFF];
    }

    for (enum game_ui_tile_types i = DEATH_TILE; i < MAX_UI_TILES; i++)
    {
        if (ui_tile_component_array[i].enlargement == enlidx)
            return glyph2tile[i + GLYPH_UI_TILE_OFF];
    }


    /* Replacement */
    for (int i = 1; i <= NUM_REPLACEMENTS; i++)
    {
        for (int j = 0; j < replacements[i].number_of_tiles; j++)
        {
            if (replacements[i].tile_enlargement[j] == enlidx)
                return glyph2tile[j + replacements[i].glyph_offset + GLYPH_REPLACEMENT_OFF];
        }
    }

    /* Animation */
    for (int i = 1; i <= NUM_ANIMATIONS; i++)
    {
        for (int j = 0; j < animations[i].number_of_tiles; j++)
        {
            if (animations[i].tile_enlargement[j] == enlidx)
                return glyph2tile[j + animations[i].glyph_offset + GLYPH_ANIMATION_OFF];
        }
    }


    return -1;
}

short
get_replacement_base_tile(replacement_idx)
short replacement_idx;
{
    for (int i = LOW_PM; i < NUM_MONSTERS; i++)
    {
        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].replacement.actions[action] == replacement_idx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 0)];
        }
        if (mons[i].replacement.statue == replacement_idx)
            return glyph2tile[i + GLYPH_STATUE_OFF];
        if (mons[i].replacement.corpse == replacement_idx)
            return glyph2tile[i + GLYPH_BODY_OFF];

        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
        {
            if (mons[i].female_replacement.actions[action] == replacement_idx)
                return glyph2tile[i + get_monster_action_glyph_offset(action, 1)];
        }
        if (mons[i].female_replacement.corpse == replacement_idx)
            return glyph2tile[i + GLYPH_FEMALE_BODY_OFF];
        if (mons[i].female_replacement.statue == replacement_idx)
            return glyph2tile[i + GLYPH_FEMALE_STATUE_OFF];
    }

    for (int i = STRANGE_OBJECT; i < NUM_OBJECTS; i++)
    {
        if (obj_descr[objects[i].oc_descr_idx].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_OBJ_OFF];
    }

    for (int cmap_idx = 0; cmap_idx < CMAP_TYPE_MAX; cmap_idx++)
    {
        for (int i = 0; i < MAX_CMAPPED_CHARS; i++)
        {
            if (defsyms[i].replacement[cmap_idx] == replacement_idx)
                return glyph2tile[i + GLYPH_CMAP_OFF];
        }

        for (int i = 0; i < MAX_VARIATIONS; i++)
        {
            if (defsym_variations[i].replacement[cmap_idx] == replacement_idx)
                return glyph2tile[i + GLYPH_CMAP_VARIATION_OFF];
        }
    }

    for (int roleidx = 0; roleidx < NUM_ROLES; roleidx++)
    {
        for (int raceidx = 0; raceidx < NUM_RACES; raceidx++)
        {
            for (int genderidx = 0; genderidx <= 1; genderidx++)
            {
                for (int alignment = -1; alignment <= 1; alignment++)
                {
                    for (int level = 0; level < NUM_PLAYER_GLYPH_LEVELS; level++)
                    {
                        int player_glyph_index = player_to_glyph_index(roleidx, raceidx, genderidx, alignment, level);
                        for (enum action_tile_types action = ACTION_TILE_NO_ACTION; action < MAX_ACTION_TILES; action++)
                        {
                            if (get_player_replacement(action, roleidx, raceidx, genderidx, alignment, level) == replacement_idx)
                                return glyph2tile[player_glyph_index + get_player_action_glyph_offset(action)];
                        }
                    }
                }
            }
        }
    }

    for (enum game_cursor_types i = CURSOR_STYLE_GENERIC_CURSOR; i < MAX_CURSORS; i++)
    {
        if (game_cursors[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_CURSOR_OFF];
    }

    for (enum game_ui_tile_types i = DEATH_TILE; i < MAX_UI_TILES; i++)
    {
        if (ui_tile_component_array[i].replacement == replacement_idx)
            return glyph2tile[i + GLYPH_UI_TILE_OFF];
    }

    return -1;
}




/* animation.c */


