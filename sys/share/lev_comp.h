
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
     MESSAGE_ATTR = 270,
     MESSAGE_COLOR = 271,
     MESSAGE_SOUND_TYPE = 272,
     MESSAGE_SOUND_ID = 273,
     MAZE_ID = 274,
     LEVEL_ID = 275,
     LEV_INIT_ID = 276,
     TILESET_ID = 277,
     GEOMETRY_ID = 278,
     NOMAP_ID = 279,
     BOUNDARY_TYPE_ID = 280,
     SPECIAL_TILESET_ID = 281,
     TILESET_PARAM_ID = 282,
     DECOTYP_ID = 283,
     OBJECT_ID = 284,
     COBJECT_ID = 285,
     MONSTER_ID = 286,
     TRAP_ID = 287,
     DOOR_ID = 288,
     DRAWBRIDGE_ID = 289,
     MONSTER_GENERATION_ID = 290,
     object_ID = 291,
     monster_ID = 292,
     terrain_ID = 293,
     MAZEWALK_ID = 294,
     WALLIFY_ID = 295,
     REGION_ID = 296,
     SPECIAL_REGION_ID = 297,
     SPECIAL_LEVREGION_ID = 298,
     SPECIAL_REGION_TYPE = 299,
     NAMING_ID = 300,
     NAMING_TYPE = 301,
     FILLING = 302,
     IRREGULAR = 303,
     JOINED = 304,
     ALTAR_ID = 305,
     ANVIL_ID = 306,
     NPC_ID = 307,
     LADDER_ID = 308,
     STAIR_ID = 309,
     NON_DIGGABLE_ID = 310,
     NON_PASSWALL_ID = 311,
     ROOM_ID = 312,
     ARTIFACT_NAME_ID = 313,
     PORTAL_ID = 314,
     TELEPRT_ID = 315,
     BRANCH_ID = 316,
     LEV = 317,
     MINERALIZE_ID = 318,
     AGE_ID = 319,
     CORRIDOR_ID = 320,
     GOLD_ID = 321,
     ENGRAVING_ID = 322,
     FOUNTAIN_ID = 323,
     THRONE_ID = 324,
     MODRON_PORTAL_ID = 325,
     LEVEL_TELEPORTER_ID = 326,
     LEVEL_TELEPORT_DIRECTION_TYPE = 327,
     LEVEL_TELEPORT_END_TYPE = 328,
     POOL_ID = 329,
     SINK_ID = 330,
     NONE = 331,
     RAND_CORRIDOR_ID = 332,
     DOOR_STATE = 333,
     LIGHT_STATE = 334,
     CURSE_TYPE = 335,
     MYTHIC_TYPE = 336,
     ENGRAVING_TYPE = 337,
     KEYTYPE_ID = 338,
     LEVER_ID = 339,
     NO_PICKUP_ID = 340,
     DIRECTION = 341,
     RANDOM_TYPE = 342,
     RANDOM_TYPE_BRACKET = 343,
     A_REGISTER = 344,
     ALIGNMENT = 345,
     LEFT_OR_RIGHT = 346,
     CENTER = 347,
     TOP_OR_BOT = 348,
     ALTAR_TYPE = 349,
     ALTAR_SUBTYPE = 350,
     UP_OR_DOWN = 351,
     ACTIVE_OR_INACTIVE = 352,
     MODRON_PORTAL_TYPE = 353,
     NPC_TYPE = 354,
     FOUNTAIN_TYPE = 355,
     SPECIAL_OBJECT_TYPE = 356,
     CMAP_TYPE = 357,
     FLOOR_SUBTYPE = 358,
     FLOOR_SUBTYPE_ID = 359,
     FLOOR_ID = 360,
     FLOOR_TYPE = 361,
     FLOOR_TYPE_ID = 362,
     DECORATION_ID = 363,
     DECORATION_TYPE = 364,
     DECORATION_DIR = 365,
     DECORATION_ITEM_STATE = 366,
     PAINTING_TYPE = 367,
     BANNER_TYPE = 368,
     WALL_SCULPTURE_TYPE = 369,
     ELEMENTAL_ENCHANTMENT_TYPE = 370,
     EXCEPTIONALITY_TYPE = 371,
     EXCEPTIONALITY_ID = 372,
     ELEMENTAL_ENCHANTMENT_ID = 373,
     ENCHANTMENT_ID = 374,
     SECRET_DOOR_ID = 375,
     USES_UP_KEY_ID = 376,
     MYTHIC_PREFIX_TYPE = 377,
     MYTHIC_SUFFIX_TYPE = 378,
     MYTHIC_PREFIX_ID = 379,
     MYTHIC_SUFFIX_ID = 380,
     MATERIAL_ID = 381,
     MATERIAL_TYPE = 382,
     CHARGES_ID = 383,
     SPECIAL_QUALITY_ID = 384,
     SPEFLAGS_ID = 385,
     SUBROOM_ID = 386,
     NAME_ID = 387,
     FLAGS_ID = 388,
     FLAG_TYPE = 389,
     MON_ATTITUDE = 390,
     MON_ALERTNESS = 391,
     SUBTYPE_ID = 392,
     NON_PASSDOOR_ID = 393,
     CARPET_ID = 394,
     CARPET_PIECE_ID = 395,
     CARPET_TYPE = 396,
     MON_APPEARANCE = 397,
     ROOMDOOR_ID = 398,
     IF_ID = 399,
     ELSE_ID = 400,
     TERRAIN_ID = 401,
     HORIZ_OR_VERT = 402,
     REPLACE_TERRAIN_ID = 403,
     LOCATION_SUBTYPE_ID = 404,
     DOOR_SUBTYPE = 405,
     BRAZIER_SUBTYPE = 406,
     SIGNPOST_SUBTYPE = 407,
     TREE_SUBTYPE = 408,
     FOREST_ID = 409,
     FOREST_TYPE = 410,
     INITIALIZE_TYPE = 411,
     EXIT_ID = 412,
     SHUFFLE_ID = 413,
     MANUAL_TYPE_ID = 414,
     MANUAL_TYPE = 415,
     QUANTITY_ID = 416,
     BURIED_ID = 417,
     LOOP_ID = 418,
     FOR_ID = 419,
     TO_ID = 420,
     SWITCH_ID = 421,
     CASE_ID = 422,
     BREAK_ID = 423,
     DEFAULT_ID = 424,
     ERODED_ID = 425,
     TRAPPED_STATE = 426,
     RECHARGED_ID = 427,
     INVIS_ID = 428,
     GREASED_ID = 429,
     INDESTRUCTIBLE_ID = 430,
     FEMALE_ID = 431,
     MALE_ID = 432,
     WAITFORU_ID = 433,
     PROTECTOR_ID = 434,
     CANCELLED_ID = 435,
     REVIVED_ID = 436,
     AVENGE_ID = 437,
     FLEEING_ID = 438,
     BLINDED_ID = 439,
     MAXHP_ID = 440,
     LEVEL_ADJUSTMENT_ID = 441,
     KEEP_ORIGINAL_INVENTORY_ID = 442,
     PARALYZED_ID = 443,
     STUNNED_ID = 444,
     CONFUSED_ID = 445,
     SEENTRAPS_ID = 446,
     ALL_ID = 447,
     MONTYPE_ID = 448,
     OBJTYPE_ID = 449,
     TERTYPE_ID = 450,
     TERTYPE2_ID = 451,
     LEVER_EFFECT_TYPE = 452,
     SWITCHABLE_ID = 453,
     CONTINUOUSLY_USABLE_ID = 454,
     TARGET_ID = 455,
     TRAPTYPE_ID = 456,
     EFFECT_FLAG_ID = 457,
     GRAVE_ID = 458,
     BRAZIER_ID = 459,
     SIGNPOST_ID = 460,
     TREE_ID = 461,
     ERODEPROOF_ID = 462,
     FUNCTION_ID = 463,
     MSG_OUTPUT_TYPE = 464,
     COMPARE_TYPE = 465,
     UNKNOWN_TYPE = 466,
     rect_ID = 467,
     fillrect_ID = 468,
     line_ID = 469,
     randline_ID = 470,
     grow_ID = 471,
     selection_ID = 472,
     flood_ID = 473,
     rndcoord_ID = 474,
     circle_ID = 475,
     ellipse_ID = 476,
     filter_ID = 477,
     complement_ID = 478,
     gradient_ID = 479,
     GRADIENT_TYPE = 480,
     LIMITED = 481,
     HUMIDITY_TYPE = 482,
     STRING = 483,
     MAP_ID = 484,
     NQSTRING = 485,
     VARSTRING = 486,
     CFUNC = 487,
     CFUNC_INT = 488,
     CFUNC_STR = 489,
     CFUNC_COORD = 490,
     CFUNC_REGION = 491,
     VARSTRING_INT = 492,
     VARSTRING_INT_ARRAY = 493,
     VARSTRING_STRING = 494,
     VARSTRING_STRING_ARRAY = 495,
     VARSTRING_VAR = 496,
     VARSTRING_VAR_ARRAY = 497,
     VARSTRING_COORD = 498,
     VARSTRING_COORD_ARRAY = 499,
     VARSTRING_REGION = 500,
     VARSTRING_REGION_ARRAY = 501,
     VARSTRING_MAPCHAR = 502,
     VARSTRING_MAPCHAR_ARRAY = 503,
     VARSTRING_MONST = 504,
     VARSTRING_MONST_ARRAY = 505,
     VARSTRING_OBJ = 506,
     VARSTRING_OBJ_ARRAY = 507,
     VARSTRING_SEL = 508,
     VARSTRING_SEL_ARRAY = 509,
     METHOD_INT = 510,
     METHOD_INT_ARRAY = 511,
     METHOD_STRING = 512,
     METHOD_STRING_ARRAY = 513,
     METHOD_VAR = 514,
     METHOD_VAR_ARRAY = 515,
     METHOD_COORD = 516,
     METHOD_COORD_ARRAY = 517,
     METHOD_REGION = 518,
     METHOD_REGION_ARRAY = 519,
     METHOD_MAPCHAR = 520,
     METHOD_MAPCHAR_ARRAY = 521,
     METHOD_MONST = 522,
     METHOD_MONST_ARRAY = 523,
     METHOD_OBJ = 524,
     METHOD_OBJ_ARRAY = 525,
     METHOD_SEL = 526,
     METHOD_SEL_ARRAY = 527,
     DICE = 528
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 153 "lev_comp.y"

    long    i;
    char    *map;
    struct {
        long room;
        long wall;
        long door;
    } corpos;
    struct {
        long area;
        long x1;
        long y1;
        long x2;
        long y2;
    } lregn;
    struct {
        long x;
        long y;
    } crd;
    struct {
        long ter;
        long lit;
    } terr;
    struct {
        long height;
        long width;
    } sze;
    struct {
        long die;
        long num;
    } dice;
    struct {
        long cfunc;
        char *varstr;
    } meth;



/* Line 1676 of yacc.c  */
#line 364 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


