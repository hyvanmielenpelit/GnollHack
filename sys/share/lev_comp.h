
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
     MAGIC_PORTAL_ID = 326,
     MODRON_PORTAL_ID = 327,
     LEVEL_TELEPORTER_ID = 328,
     LEVEL_TELEPORT_DIRECTION_TYPE = 329,
     LEVEL_TELEPORT_END_TYPE = 330,
     POOL_ID = 331,
     SINK_ID = 332,
     NONE = 333,
     RAND_CORRIDOR_ID = 334,
     DOOR_STATE = 335,
     LIGHT_STATE = 336,
     CURSE_TYPE = 337,
     MYTHIC_TYPE = 338,
     ENGRAVING_TYPE = 339,
     KEYTYPE_ID = 340,
     LEVER_ID = 341,
     NO_PICKUP_ID = 342,
     DIRECTION = 343,
     RANDOM_TYPE = 344,
     RANDOM_TYPE_BRACKET = 345,
     A_REGISTER = 346,
     ALIGNMENT = 347,
     LEFT_OR_RIGHT = 348,
     CENTER = 349,
     TOP_OR_BOT = 350,
     ALTAR_TYPE = 351,
     ALTAR_SUBTYPE = 352,
     UP_OR_DOWN = 353,
     ACTIVE_OR_INACTIVE = 354,
     MAGIC_PORTAL_TARGET_TYPE_ID = 355,
     MAGIC_PORTAL_TARGET_TYPE = 356,
     SEEN_OR_UNSEEN = 357,
     MODRON_PORTAL_TYPE = 358,
     NPC_TYPE = 359,
     FOUNTAIN_TYPE = 360,
     SPECIAL_OBJECT_TYPE = 361,
     CMAP_TYPE = 362,
     FLOOR_SUBTYPE = 363,
     FLOOR_SUBTYPE_ID = 364,
     FLOOR_ID = 365,
     FLOOR_TYPE = 366,
     FLOOR_TYPE_ID = 367,
     DECORATION_ID = 368,
     DECORATION_TYPE = 369,
     DECORATION_DIR = 370,
     DECORATION_ITEM_STATE = 371,
     PAINTING_TYPE = 372,
     BANNER_TYPE = 373,
     WALL_SCULPTURE_TYPE = 374,
     ELEMENTAL_ENCHANTMENT_TYPE = 375,
     EXCEPTIONALITY_TYPE = 376,
     EXCEPTIONALITY_ID = 377,
     ELEMENTAL_ENCHANTMENT_ID = 378,
     ENCHANTMENT_ID = 379,
     SECRET_DOOR_ID = 380,
     USES_UP_KEY_ID = 381,
     MYTHIC_PREFIX_TYPE = 382,
     MYTHIC_SUFFIX_TYPE = 383,
     MYTHIC_PREFIX_ID = 384,
     MYTHIC_SUFFIX_ID = 385,
     MATERIAL_ID = 386,
     MATERIAL_TYPE = 387,
     CHARGES_ID = 388,
     SPECIAL_QUALITY_ID = 389,
     SPEFLAGS_ID = 390,
     LEVEL_BOSS_ID = 391,
     BOSS_HOSTILITY_ID = 392,
     HAS_BACKUP_ID = 393,
     SUBROOM_ID = 394,
     NAME_ID = 395,
     FLAGS_ID = 396,
     FLAG_TYPE = 397,
     MON_ATTITUDE = 398,
     MON_ALERTNESS = 399,
     SUBTYPE_ID = 400,
     NON_PASSDOOR_ID = 401,
     CARPET_ID = 402,
     CARPET_PIECE_ID = 403,
     CARPET_TYPE = 404,
     MON_APPEARANCE = 405,
     ROOMDOOR_ID = 406,
     IF_ID = 407,
     ELSE_ID = 408,
     TERRAIN_ID = 409,
     HORIZ_OR_VERT = 410,
     REPLACE_TERRAIN_ID = 411,
     LOCATION_SUBTYPE_ID = 412,
     DOOR_SUBTYPE = 413,
     BRAZIER_SUBTYPE = 414,
     SIGNPOST_SUBTYPE = 415,
     TREE_SUBTYPE = 416,
     FOREST_ID = 417,
     FOREST_TYPE = 418,
     INITIALIZE_TYPE = 419,
     EXIT_ID = 420,
     SHUFFLE_ID = 421,
     MANUAL_TYPE_ID = 422,
     MANUAL_TYPE = 423,
     QUANTITY_ID = 424,
     BURIED_ID = 425,
     LOOP_ID = 426,
     FOR_ID = 427,
     TO_ID = 428,
     SWITCH_ID = 429,
     CASE_ID = 430,
     BREAK_ID = 431,
     DEFAULT_ID = 432,
     ERODED_ID = 433,
     TRAPPED_STATE = 434,
     RECHARGED_ID = 435,
     INVIS_ID = 436,
     GREASED_ID = 437,
     INDESTRUCTIBLE_ID = 438,
     FEMALE_ID = 439,
     MALE_ID = 440,
     WAITFORU_ID = 441,
     PROTECTOR_ID = 442,
     CANCELLED_ID = 443,
     REVIVED_ID = 444,
     AVENGE_ID = 445,
     FLEEING_ID = 446,
     BLINDED_ID = 447,
     MAXHP_ID = 448,
     LEVEL_ADJUSTMENT_ID = 449,
     KEEP_ORIGINAL_INVENTORY_ID = 450,
     PARALYZED_ID = 451,
     STUNNED_ID = 452,
     CONFUSED_ID = 453,
     SEENTRAPS_ID = 454,
     ALL_ID = 455,
     MONTYPE_ID = 456,
     OBJTYPE_ID = 457,
     TERTYPE_ID = 458,
     TERTYPE2_ID = 459,
     LEVER_EFFECT_TYPE = 460,
     SWITCHABLE_ID = 461,
     CONTINUOUSLY_USABLE_ID = 462,
     TARGET_ID = 463,
     TRAPTYPE_ID = 464,
     EFFECT_FLAG_ID = 465,
     GRAVE_ID = 466,
     BRAZIER_ID = 467,
     SIGNPOST_ID = 468,
     TREE_ID = 469,
     ERODEPROOF_ID = 470,
     FUNCTION_ID = 471,
     MSG_OUTPUT_TYPE = 472,
     COMPARE_TYPE = 473,
     UNKNOWN_TYPE = 474,
     rect_ID = 475,
     fillrect_ID = 476,
     line_ID = 477,
     randline_ID = 478,
     grow_ID = 479,
     selection_ID = 480,
     flood_ID = 481,
     rndcoord_ID = 482,
     circle_ID = 483,
     ellipse_ID = 484,
     filter_ID = 485,
     complement_ID = 486,
     gradient_ID = 487,
     GRADIENT_TYPE = 488,
     LIMITED = 489,
     HUMIDITY_TYPE = 490,
     STRING = 491,
     MAP_ID = 492,
     NQSTRING = 493,
     VARSTRING = 494,
     CFUNC = 495,
     CFUNC_INT = 496,
     CFUNC_STR = 497,
     CFUNC_COORD = 498,
     CFUNC_REGION = 499,
     VARSTRING_INT = 500,
     VARSTRING_INT_ARRAY = 501,
     VARSTRING_STRING = 502,
     VARSTRING_STRING_ARRAY = 503,
     VARSTRING_VAR = 504,
     VARSTRING_VAR_ARRAY = 505,
     VARSTRING_COORD = 506,
     VARSTRING_COORD_ARRAY = 507,
     VARSTRING_REGION = 508,
     VARSTRING_REGION_ARRAY = 509,
     VARSTRING_MAPCHAR = 510,
     VARSTRING_MAPCHAR_ARRAY = 511,
     VARSTRING_MONST = 512,
     VARSTRING_MONST_ARRAY = 513,
     VARSTRING_OBJ = 514,
     VARSTRING_OBJ_ARRAY = 515,
     VARSTRING_SEL = 516,
     VARSTRING_SEL_ARRAY = 517,
     METHOD_INT = 518,
     METHOD_INT_ARRAY = 519,
     METHOD_STRING = 520,
     METHOD_STRING_ARRAY = 521,
     METHOD_VAR = 522,
     METHOD_VAR_ARRAY = 523,
     METHOD_COORD = 524,
     METHOD_COORD_ARRAY = 525,
     METHOD_REGION = 526,
     METHOD_REGION_ARRAY = 527,
     METHOD_MAPCHAR = 528,
     METHOD_MAPCHAR_ARRAY = 529,
     METHOD_MONST = 530,
     METHOD_MONST_ARRAY = 531,
     METHOD_OBJ = 532,
     METHOD_OBJ_ARRAY = 533,
     METHOD_SEL = 534,
     METHOD_SEL_ARRAY = 535,
     DICE = 536
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
#line 372 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


