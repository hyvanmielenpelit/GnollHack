
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CHAR = 258,
     INTEGER = 259,
     BOOLEAN = 260,
     PERCENT = 261,
     SPERCENT = 262,
     MINUS_INTEGER = 263,
     PLUS_INTEGER = 264,
     MAZE_GRID_ID = 265,
     SOLID_FILL_ID = 266,
     MINES_ID = 267,
     ROGUELEV_ID = 268,
     MESSAGE_ID = 269,
     MESSAGE_TYPE = 270,
     MESSAGE_ATTR = 271,
     MESSAGE_COLOR = 272,
     MESSAGE_SOUND_TYPE = 273,
     MESSAGE_SOUND_ID = 274,
     MAZE_ID = 275,
     LEVEL_ID = 276,
     LEV_INIT_ID = 277,
     TILESET_ID = 278,
     GEOMETRY_ID = 279,
     NOMAP_ID = 280,
     BOUNDARY_TYPE_ID = 281,
     SPECIAL_TILESET_ID = 282,
     TILESET_PARAM_ID = 283,
     DECOTYP_ID = 284,
     OBJECT_ID = 285,
     COBJECT_ID = 286,
     MONSTER_ID = 287,
     TRAP_ID = 288,
     DOOR_ID = 289,
     DRAWBRIDGE_ID = 290,
     MONSTER_GENERATION_ID = 291,
     object_ID = 292,
     monster_ID = 293,
     terrain_ID = 294,
     MAZEWALK_ID = 295,
     WALLIFY_ID = 296,
     REGION_ID = 297,
     SPECIAL_REGION_ID = 298,
     SPECIAL_LEVREGION_ID = 299,
     SPECIAL_REGION_TYPE = 300,
     NAMING_ID = 301,
     NAMING_TYPE = 302,
     FILLING = 303,
     IRREGULAR = 304,
     JOINED = 305,
     ALTAR_ID = 306,
     ANVIL_ID = 307,
     NPC_ID = 308,
     LADDER_ID = 309,
     STAIR_ID = 310,
     NON_DIGGABLE_ID = 311,
     NON_PASSWALL_ID = 312,
     ROOM_ID = 313,
     ARTIFACT_NAME_ID = 314,
     PORTAL_ID = 315,
     TELEPRT_ID = 316,
     BRANCH_ID = 317,
     LEV = 318,
     MINERALIZE_ID = 319,
     AGE_ID = 320,
     CORRIDOR_ID = 321,
     GOLD_ID = 322,
     ENGRAVING_ID = 323,
     FOUNTAIN_ID = 324,
     THRONE_ID = 325,
     MODRON_PORTAL_ID = 326,
     LEVEL_TELEPORTER_ID = 327,
     LEVEL_TELEPORT_DIRECTION_TYPE = 328,
     LEVEL_TELEPORT_END_TYPE = 329,
     POOL_ID = 330,
     SINK_ID = 331,
     NONE = 332,
     RAND_CORRIDOR_ID = 333,
     DOOR_STATE = 334,
     LIGHT_STATE = 335,
     CURSE_TYPE = 336,
     MYTHIC_TYPE = 337,
     ENGRAVING_TYPE = 338,
     KEYTYPE_ID = 339,
     LEVER_ID = 340,
     NO_PICKUP_ID = 341,
     DIRECTION = 342,
     RANDOM_TYPE = 343,
     RANDOM_TYPE_BRACKET = 344,
     A_REGISTER = 345,
     ALIGNMENT = 346,
     LEFT_OR_RIGHT = 347,
     CENTER = 348,
     TOP_OR_BOT = 349,
     ALTAR_TYPE = 350,
     ALTAR_SUBTYPE = 351,
     UP_OR_DOWN = 352,
     ACTIVE_OR_INACTIVE = 353,
     MODRON_PORTAL_TYPE = 354,
     NPC_TYPE = 355,
     FOUNTAIN_TYPE = 356,
     SPECIAL_OBJECT_TYPE = 357,
     CMAP_TYPE = 358,
     FLOOR_SUBTYPE = 359,
     FLOOR_SUBTYPE_ID = 360,
     FLOOR_ID = 361,
     FLOOR_TYPE = 362,
     FLOOR_TYPE_ID = 363,
     DECORATION_ID = 364,
     DECORATION_TYPE = 365,
     DECORATION_DIR = 366,
     DECORATION_ITEM_STATE = 367,
     PAINTING_TYPE = 368,
     BANNER_TYPE = 369,
     WALL_SCULPTURE_TYPE = 370,
     ELEMENTAL_ENCHANTMENT_TYPE = 371,
     EXCEPTIONALITY_TYPE = 372,
     EXCEPTIONALITY_ID = 373,
     ELEMENTAL_ENCHANTMENT_ID = 374,
     ENCHANTMENT_ID = 375,
     SECRET_DOOR_ID = 376,
     USES_UP_KEY_ID = 377,
     MYTHIC_PREFIX_TYPE = 378,
     MYTHIC_SUFFIX_TYPE = 379,
     MYTHIC_PREFIX_ID = 380,
     MYTHIC_SUFFIX_ID = 381,
     MATERIAL_ID = 382,
     MATERIAL_TYPE = 383,
     CHARGES_ID = 384,
     SPECIAL_QUALITY_ID = 385,
     SPEFLAGS_ID = 386,
     LEVEL_BOSS_ID = 387,
     BOSS_HOSTILITY_ID = 388,
     HAS_BACKUP_ID = 389,
     SUBROOM_ID = 390,
     NAME_ID = 391,
     FLAGS_ID = 392,
     FLAG_TYPE = 393,
     MON_ATTITUDE = 394,
     MON_ALERTNESS = 395,
     SUBTYPE_ID = 396,
     NON_PASSDOOR_ID = 397,
     CARPET_ID = 398,
     CARPET_PIECE_ID = 399,
     CARPET_TYPE = 400,
     MON_APPEARANCE = 401,
     ROOMDOOR_ID = 402,
     IF_ID = 403,
     ELSE_ID = 404,
     TERRAIN_ID = 405,
     HORIZ_OR_VERT = 406,
     REPLACE_TERRAIN_ID = 407,
     LOCATION_SUBTYPE_ID = 408,
     DOOR_SUBTYPE = 409,
     BRAZIER_SUBTYPE = 410,
     SIGNPOST_SUBTYPE = 411,
     TREE_SUBTYPE = 412,
     FOREST_ID = 413,
     FOREST_TYPE = 414,
     INITIALIZE_TYPE = 415,
     EXIT_ID = 416,
     SHUFFLE_ID = 417,
     MANUAL_TYPE_ID = 418,
     MANUAL_TYPE = 419,
     QUANTITY_ID = 420,
     BURIED_ID = 421,
     LOOP_ID = 422,
     FOR_ID = 423,
     TO_ID = 424,
     SWITCH_ID = 425,
     CASE_ID = 426,
     BREAK_ID = 427,
     DEFAULT_ID = 428,
     ERODED_ID = 429,
     TRAPPED_STATE = 430,
     RECHARGED_ID = 431,
     INVIS_ID = 432,
     GREASED_ID = 433,
     INDESTRUCTIBLE_ID = 434,
     FEMALE_ID = 435,
     MALE_ID = 436,
     WAITFORU_ID = 437,
     PROTECTOR_ID = 438,
     CANCELLED_ID = 439,
     REVIVED_ID = 440,
     AVENGE_ID = 441,
     FLEEING_ID = 442,
     BLINDED_ID = 443,
     MAXHP_ID = 444,
     LEVEL_ADJUSTMENT_ID = 445,
     KEEP_ORIGINAL_INVENTORY_ID = 446,
     PARALYZED_ID = 447,
     STUNNED_ID = 448,
     CONFUSED_ID = 449,
     SEENTRAPS_ID = 450,
     ALL_ID = 451,
     MONTYPE_ID = 452,
     OBJTYPE_ID = 453,
     TERTYPE_ID = 454,
     TERTYPE2_ID = 455,
     LEVER_EFFECT_TYPE = 456,
     SWITCHABLE_ID = 457,
     CONTINUOUSLY_USABLE_ID = 458,
     TARGET_ID = 459,
     TRAPTYPE_ID = 460,
     EFFECT_FLAG_ID = 461,
     GRAVE_ID = 462,
     BRAZIER_ID = 463,
     SIGNPOST_ID = 464,
     TREE_ID = 465,
     ERODEPROOF_ID = 466,
     FUNCTION_ID = 467,
     MSG_OUTPUT_TYPE = 468,
     COMPARE_TYPE = 469,
     UNKNOWN_TYPE = 470,
     rect_ID = 471,
     fillrect_ID = 472,
     line_ID = 473,
     randline_ID = 474,
     grow_ID = 475,
     selection_ID = 476,
     flood_ID = 477,
     rndcoord_ID = 478,
     circle_ID = 479,
     ellipse_ID = 480,
     filter_ID = 481,
     complement_ID = 482,
     gradient_ID = 483,
     GRADIENT_TYPE = 484,
     LIMITED = 485,
     HUMIDITY_TYPE = 486,
     STRING = 487,
     MAP_ID = 488,
     NQSTRING = 489,
     VARSTRING = 490,
     CFUNC = 491,
     CFUNC_INT = 492,
     CFUNC_STR = 493,
     CFUNC_COORD = 494,
     CFUNC_REGION = 495,
     VARSTRING_INT = 496,
     VARSTRING_INT_ARRAY = 497,
     VARSTRING_STRING = 498,
     VARSTRING_STRING_ARRAY = 499,
     VARSTRING_VAR = 500,
     VARSTRING_VAR_ARRAY = 501,
     VARSTRING_COORD = 502,
     VARSTRING_COORD_ARRAY = 503,
     VARSTRING_REGION = 504,
     VARSTRING_REGION_ARRAY = 505,
     VARSTRING_MAPCHAR = 506,
     VARSTRING_MAPCHAR_ARRAY = 507,
     VARSTRING_MONST = 508,
     VARSTRING_MONST_ARRAY = 509,
     VARSTRING_OBJ = 510,
     VARSTRING_OBJ_ARRAY = 511,
     VARSTRING_SEL = 512,
     VARSTRING_SEL_ARRAY = 513,
     METHOD_INT = 514,
     METHOD_INT_ARRAY = 515,
     METHOD_STRING = 516,
     METHOD_STRING_ARRAY = 517,
     METHOD_VAR = 518,
     METHOD_VAR_ARRAY = 519,
     METHOD_COORD = 520,
     METHOD_COORD_ARRAY = 521,
     METHOD_REGION = 522,
     METHOD_REGION_ARRAY = 523,
     METHOD_MAPCHAR = 524,
     METHOD_MAPCHAR_ARRAY = 525,
     METHOD_MONST = 526,
     METHOD_MONST_ARRAY = 527,
     METHOD_OBJ = 528,
     METHOD_OBJ_ARRAY = 529,
     METHOD_SEL = 530,
     METHOD_SEL_ARRAY = 531,
     DICE = 532
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 153 "lev_comp.y"

    int64_t    i;
    char    *map;
    struct {
        int64_t room;
        int64_t wall;
        int64_t door;
    } corpos;
    struct {
        int64_t area;
        int64_t x1;
        int64_t y1;
        int64_t x2;
        int64_t y2;
    } lregn;
    struct {
        int64_t x;
        int64_t y;
    } crd;
    struct {
        int64_t ter;
        int64_t lit;
    } terr;
    struct {
        int64_t height;
        int64_t width;
    } sze;
    struct {
        int64_t die;
        int64_t num;
    } dice;
    struct {
        int64_t cfunc;
        char *varstr;
    } meth;



/* Line 1676 of yacc.c  */
#line 368 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


