
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
     MAZE_ID = 271,
     LEVEL_ID = 272,
     LEV_INIT_ID = 273,
     TILESET_ID = 274,
     GEOMETRY_ID = 275,
     NOMAP_ID = 276,
     BOUNDARY_TYPE_ID = 277,
     SPECIAL_TILESET_ID = 278,
     TILESET_PARAM_ID = 279,
     DECOTYP_ID = 280,
     OBJECT_ID = 281,
     COBJECT_ID = 282,
     MONSTER_ID = 283,
     TRAP_ID = 284,
     DOOR_ID = 285,
     DRAWBRIDGE_ID = 286,
     MONSTER_GENERATION_ID = 287,
     object_ID = 288,
     monster_ID = 289,
     terrain_ID = 290,
     MAZEWALK_ID = 291,
     WALLIFY_ID = 292,
     REGION_ID = 293,
     SPECIAL_REGION_ID = 294,
     SPECIAL_LEVREGION_ID = 295,
     SPECIAL_REGION_TYPE = 296,
     NAMING_ID = 297,
     NAMING_TYPE = 298,
     FILLING = 299,
     IRREGULAR = 300,
     JOINED = 301,
     ALTAR_ID = 302,
     ANVIL_ID = 303,
     NPC_ID = 304,
     LADDER_ID = 305,
     STAIR_ID = 306,
     NON_DIGGABLE_ID = 307,
     NON_PASSWALL_ID = 308,
     ROOM_ID = 309,
     ARTIFACT_NAME_ID = 310,
     PORTAL_ID = 311,
     TELEPRT_ID = 312,
     BRANCH_ID = 313,
     LEV = 314,
     MINERALIZE_ID = 315,
     AGE_ID = 316,
     CORRIDOR_ID = 317,
     GOLD_ID = 318,
     ENGRAVING_ID = 319,
     FOUNTAIN_ID = 320,
     THRONE_ID = 321,
     MODRON_PORTAL_ID = 322,
     LEVEL_TELEPORTER_ID = 323,
     LEVEL_TELEPORT_DIRECTION_TYPE = 324,
     LEVEL_TELEPORT_END_TYPE = 325,
     POOL_ID = 326,
     SINK_ID = 327,
     NONE = 328,
     RAND_CORRIDOR_ID = 329,
     DOOR_STATE = 330,
     LIGHT_STATE = 331,
     CURSE_TYPE = 332,
     MYTHIC_TYPE = 333,
     ENGRAVING_TYPE = 334,
     KEYTYPE_ID = 335,
     LEVER_ID = 336,
     NO_PICKUP_ID = 337,
     DIRECTION = 338,
     RANDOM_TYPE = 339,
     RANDOM_TYPE_BRACKET = 340,
     A_REGISTER = 341,
     ALIGNMENT = 342,
     LEFT_OR_RIGHT = 343,
     CENTER = 344,
     TOP_OR_BOT = 345,
     ALTAR_TYPE = 346,
     ALTAR_SUBTYPE = 347,
     UP_OR_DOWN = 348,
     ACTIVE_OR_INACTIVE = 349,
     MODRON_PORTAL_TYPE = 350,
     NPC_TYPE = 351,
     FOUNTAIN_TYPE = 352,
     SPECIAL_OBJECT_TYPE = 353,
     CMAP_TYPE = 354,
     FLOOR_SUBTYPE = 355,
     FLOOR_SUBTYPE_ID = 356,
     FLOOR_ID = 357,
     FLOOR_TYPE = 358,
     FLOOR_TYPE_ID = 359,
     DECORATION_ID = 360,
     DECORATION_TYPE = 361,
     DECORATION_DIR = 362,
     DECORATION_ITEM_STATE = 363,
     PAINTING_TYPE = 364,
     BANNER_TYPE = 365,
     WALL_SCULPTURE_TYPE = 366,
     ELEMENTAL_ENCHANTMENT_TYPE = 367,
     EXCEPTIONALITY_TYPE = 368,
     EXCEPTIONALITY_ID = 369,
     ELEMENTAL_ENCHANTMENT_ID = 370,
     ENCHANTMENT_ID = 371,
     SECRET_DOOR_ID = 372,
     USES_UP_KEY_ID = 373,
     MYTHIC_PREFIX_TYPE = 374,
     MYTHIC_SUFFIX_TYPE = 375,
     MYTHIC_PREFIX_ID = 376,
     MYTHIC_SUFFIX_ID = 377,
     MATERIAL_ID = 378,
     MATERIAL_TYPE = 379,
     CHARGES_ID = 380,
     SPECIAL_QUALITY_ID = 381,
     SPEFLAGS_ID = 382,
     SUBROOM_ID = 383,
     NAME_ID = 384,
     FLAGS_ID = 385,
     FLAG_TYPE = 386,
     MON_ATTITUDE = 387,
     MON_ALERTNESS = 388,
     SUBTYPE_ID = 389,
     NON_PASSDOOR_ID = 390,
     CARPET_ID = 391,
     CARPET_PIECE_ID = 392,
     CARPET_TYPE = 393,
     MON_APPEARANCE = 394,
     ROOMDOOR_ID = 395,
     IF_ID = 396,
     ELSE_ID = 397,
     TERRAIN_ID = 398,
     HORIZ_OR_VERT = 399,
     REPLACE_TERRAIN_ID = 400,
     LOCATION_SUBTYPE_ID = 401,
     DOOR_SUBTYPE = 402,
     BRAZIER_SUBTYPE = 403,
     SIGNPOST_SUBTYPE = 404,
     TREE_SUBTYPE = 405,
     FOREST_ID = 406,
     FOREST_TYPE = 407,
     INITIALIZE_TYPE = 408,
     EXIT_ID = 409,
     SHUFFLE_ID = 410,
     MANUAL_TYPE_ID = 411,
     MANUAL_TYPE = 412,
     QUANTITY_ID = 413,
     BURIED_ID = 414,
     LOOP_ID = 415,
     FOR_ID = 416,
     TO_ID = 417,
     SWITCH_ID = 418,
     CASE_ID = 419,
     BREAK_ID = 420,
     DEFAULT_ID = 421,
     ERODED_ID = 422,
     TRAPPED_STATE = 423,
     RECHARGED_ID = 424,
     INVIS_ID = 425,
     GREASED_ID = 426,
     INDESTRUCTIBLE_ID = 427,
     FEMALE_ID = 428,
     MALE_ID = 429,
     WAITFORU_ID = 430,
     PROTECTOR_ID = 431,
     CANCELLED_ID = 432,
     REVIVED_ID = 433,
     AVENGE_ID = 434,
     FLEEING_ID = 435,
     BLINDED_ID = 436,
     MAXHP_ID = 437,
     LEVEL_ADJUSTMENT_ID = 438,
     KEEP_ORIGINAL_INVENTORY_ID = 439,
     PARALYZED_ID = 440,
     STUNNED_ID = 441,
     CONFUSED_ID = 442,
     SEENTRAPS_ID = 443,
     ALL_ID = 444,
     MONTYPE_ID = 445,
     OBJTYPE_ID = 446,
     TERTYPE_ID = 447,
     TERTYPE2_ID = 448,
     LEVER_EFFECT_TYPE = 449,
     SWITCHABLE_ID = 450,
     CONTINUOUSLY_USABLE_ID = 451,
     TARGET_ID = 452,
     TRAPTYPE_ID = 453,
     EFFECT_FLAG_ID = 454,
     GRAVE_ID = 455,
     BRAZIER_ID = 456,
     SIGNPOST_ID = 457,
     TREE_ID = 458,
     ERODEPROOF_ID = 459,
     FUNCTION_ID = 460,
     MSG_OUTPUT_TYPE = 461,
     COMPARE_TYPE = 462,
     UNKNOWN_TYPE = 463,
     rect_ID = 464,
     fillrect_ID = 465,
     line_ID = 466,
     randline_ID = 467,
     grow_ID = 468,
     selection_ID = 469,
     flood_ID = 470,
     rndcoord_ID = 471,
     circle_ID = 472,
     ellipse_ID = 473,
     filter_ID = 474,
     complement_ID = 475,
     gradient_ID = 476,
     GRADIENT_TYPE = 477,
     LIMITED = 478,
     HUMIDITY_TYPE = 479,
     STRING = 480,
     MAP_ID = 481,
     NQSTRING = 482,
     VARSTRING = 483,
     CFUNC = 484,
     CFUNC_INT = 485,
     CFUNC_STR = 486,
     CFUNC_COORD = 487,
     CFUNC_REGION = 488,
     VARSTRING_INT = 489,
     VARSTRING_INT_ARRAY = 490,
     VARSTRING_STRING = 491,
     VARSTRING_STRING_ARRAY = 492,
     VARSTRING_VAR = 493,
     VARSTRING_VAR_ARRAY = 494,
     VARSTRING_COORD = 495,
     VARSTRING_COORD_ARRAY = 496,
     VARSTRING_REGION = 497,
     VARSTRING_REGION_ARRAY = 498,
     VARSTRING_MAPCHAR = 499,
     VARSTRING_MAPCHAR_ARRAY = 500,
     VARSTRING_MONST = 501,
     VARSTRING_MONST_ARRAY = 502,
     VARSTRING_OBJ = 503,
     VARSTRING_OBJ_ARRAY = 504,
     VARSTRING_SEL = 505,
     VARSTRING_SEL_ARRAY = 506,
     METHOD_INT = 507,
     METHOD_INT_ARRAY = 508,
     METHOD_STRING = 509,
     METHOD_STRING_ARRAY = 510,
     METHOD_VAR = 511,
     METHOD_VAR_ARRAY = 512,
     METHOD_COORD = 513,
     METHOD_COORD_ARRAY = 514,
     METHOD_REGION = 515,
     METHOD_REGION_ARRAY = 516,
     METHOD_MAPCHAR = 517,
     METHOD_MAPCHAR_ARRAY = 518,
     METHOD_MONST = 519,
     METHOD_MONST_ARRAY = 520,
     METHOD_OBJ = 521,
     METHOD_OBJ_ARRAY = 522,
     METHOD_SEL = 523,
     METHOD_SEL_ARRAY = 524,
     DICE = 525
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
#line 361 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


