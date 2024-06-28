
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
     SUBROOM_ID = 389,
     NAME_ID = 390,
     FLAGS_ID = 391,
     FLAG_TYPE = 392,
     MON_ATTITUDE = 393,
     MON_ALERTNESS = 394,
     SUBTYPE_ID = 395,
     NON_PASSDOOR_ID = 396,
     CARPET_ID = 397,
     CARPET_PIECE_ID = 398,
     CARPET_TYPE = 399,
     MON_APPEARANCE = 400,
     ROOMDOOR_ID = 401,
     IF_ID = 402,
     ELSE_ID = 403,
     TERRAIN_ID = 404,
     HORIZ_OR_VERT = 405,
     REPLACE_TERRAIN_ID = 406,
     LOCATION_SUBTYPE_ID = 407,
     DOOR_SUBTYPE = 408,
     BRAZIER_SUBTYPE = 409,
     SIGNPOST_SUBTYPE = 410,
     TREE_SUBTYPE = 411,
     FOREST_ID = 412,
     FOREST_TYPE = 413,
     INITIALIZE_TYPE = 414,
     EXIT_ID = 415,
     SHUFFLE_ID = 416,
     MANUAL_TYPE_ID = 417,
     MANUAL_TYPE = 418,
     QUANTITY_ID = 419,
     BURIED_ID = 420,
     LOOP_ID = 421,
     FOR_ID = 422,
     TO_ID = 423,
     SWITCH_ID = 424,
     CASE_ID = 425,
     BREAK_ID = 426,
     DEFAULT_ID = 427,
     ERODED_ID = 428,
     TRAPPED_STATE = 429,
     RECHARGED_ID = 430,
     INVIS_ID = 431,
     GREASED_ID = 432,
     INDESTRUCTIBLE_ID = 433,
     FEMALE_ID = 434,
     MALE_ID = 435,
     WAITFORU_ID = 436,
     PROTECTOR_ID = 437,
     CANCELLED_ID = 438,
     REVIVED_ID = 439,
     AVENGE_ID = 440,
     FLEEING_ID = 441,
     BLINDED_ID = 442,
     MAXHP_ID = 443,
     LEVEL_ADJUSTMENT_ID = 444,
     KEEP_ORIGINAL_INVENTORY_ID = 445,
     PARALYZED_ID = 446,
     STUNNED_ID = 447,
     CONFUSED_ID = 448,
     SEENTRAPS_ID = 449,
     ALL_ID = 450,
     MONTYPE_ID = 451,
     OBJTYPE_ID = 452,
     TERTYPE_ID = 453,
     TERTYPE2_ID = 454,
     LEVER_EFFECT_TYPE = 455,
     SWITCHABLE_ID = 456,
     CONTINUOUSLY_USABLE_ID = 457,
     TARGET_ID = 458,
     TRAPTYPE_ID = 459,
     EFFECT_FLAG_ID = 460,
     GRAVE_ID = 461,
     BRAZIER_ID = 462,
     SIGNPOST_ID = 463,
     TREE_ID = 464,
     ERODEPROOF_ID = 465,
     FUNCTION_ID = 466,
     MSG_OUTPUT_TYPE = 467,
     COMPARE_TYPE = 468,
     UNKNOWN_TYPE = 469,
     rect_ID = 470,
     fillrect_ID = 471,
     line_ID = 472,
     randline_ID = 473,
     grow_ID = 474,
     selection_ID = 475,
     flood_ID = 476,
     rndcoord_ID = 477,
     circle_ID = 478,
     ellipse_ID = 479,
     filter_ID = 480,
     complement_ID = 481,
     gradient_ID = 482,
     GRADIENT_TYPE = 483,
     LIMITED = 484,
     HUMIDITY_TYPE = 485,
     STRING = 486,
     MAP_ID = 487,
     NQSTRING = 488,
     VARSTRING = 489,
     CFUNC = 490,
     CFUNC_INT = 491,
     CFUNC_STR = 492,
     CFUNC_COORD = 493,
     CFUNC_REGION = 494,
     VARSTRING_INT = 495,
     VARSTRING_INT_ARRAY = 496,
     VARSTRING_STRING = 497,
     VARSTRING_STRING_ARRAY = 498,
     VARSTRING_VAR = 499,
     VARSTRING_VAR_ARRAY = 500,
     VARSTRING_COORD = 501,
     VARSTRING_COORD_ARRAY = 502,
     VARSTRING_REGION = 503,
     VARSTRING_REGION_ARRAY = 504,
     VARSTRING_MAPCHAR = 505,
     VARSTRING_MAPCHAR_ARRAY = 506,
     VARSTRING_MONST = 507,
     VARSTRING_MONST_ARRAY = 508,
     VARSTRING_OBJ = 509,
     VARSTRING_OBJ_ARRAY = 510,
     VARSTRING_SEL = 511,
     VARSTRING_SEL_ARRAY = 512,
     METHOD_INT = 513,
     METHOD_INT_ARRAY = 514,
     METHOD_STRING = 515,
     METHOD_STRING_ARRAY = 516,
     METHOD_VAR = 517,
     METHOD_VAR_ARRAY = 518,
     METHOD_COORD = 519,
     METHOD_COORD_ARRAY = 520,
     METHOD_REGION = 521,
     METHOD_REGION_ARRAY = 522,
     METHOD_MAPCHAR = 523,
     METHOD_MAPCHAR_ARRAY = 524,
     METHOD_MONST = 525,
     METHOD_MONST_ARRAY = 526,
     METHOD_OBJ = 527,
     METHOD_OBJ_ARRAY = 528,
     METHOD_SEL = 529,
     METHOD_SEL_ARRAY = 530,
     DICE = 531
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
#line 367 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


