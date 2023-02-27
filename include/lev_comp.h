
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
     OBJECT_ID = 280,
     COBJECT_ID = 281,
     MONSTER_ID = 282,
     TRAP_ID = 283,
     DOOR_ID = 284,
     DRAWBRIDGE_ID = 285,
     MONSTER_GENERATION_ID = 286,
     object_ID = 287,
     monster_ID = 288,
     terrain_ID = 289,
     MAZEWALK_ID = 290,
     WALLIFY_ID = 291,
     REGION_ID = 292,
     SPECIAL_REGION_ID = 293,
     SPECIAL_LEVREGION_ID = 294,
     SPECIAL_REGION_TYPE = 295,
     NAMING_ID = 296,
     NAMING_TYPE = 297,
     FILLING = 298,
     IRREGULAR = 299,
     JOINED = 300,
     ALTAR_ID = 301,
     ANVIL_ID = 302,
     NPC_ID = 303,
     LADDER_ID = 304,
     STAIR_ID = 305,
     NON_DIGGABLE_ID = 306,
     NON_PASSWALL_ID = 307,
     ROOM_ID = 308,
     ARTIFACT_NAME_ID = 309,
     PORTAL_ID = 310,
     TELEPRT_ID = 311,
     BRANCH_ID = 312,
     LEV = 313,
     MINERALIZE_ID = 314,
     AGE_ID = 315,
     CORRIDOR_ID = 316,
     GOLD_ID = 317,
     ENGRAVING_ID = 318,
     FOUNTAIN_ID = 319,
     THRONE_ID = 320,
     MODRON_PORTAL_ID = 321,
     LEVEL_TELEPORTER_ID = 322,
     LEVEL_TELEPORT_DIRECTION_TYPE = 323,
     LEVEL_TELEPORT_END_TYPE = 324,
     POOL_ID = 325,
     SINK_ID = 326,
     NONE = 327,
     RAND_CORRIDOR_ID = 328,
     DOOR_STATE = 329,
     LIGHT_STATE = 330,
     CURSE_TYPE = 331,
     MYTHIC_TYPE = 332,
     ENGRAVING_TYPE = 333,
     KEYTYPE_ID = 334,
     LEVER_ID = 335,
     NO_PICKUP_ID = 336,
     DIRECTION = 337,
     RANDOM_TYPE = 338,
     RANDOM_TYPE_BRACKET = 339,
     A_REGISTER = 340,
     ALIGNMENT = 341,
     LEFT_OR_RIGHT = 342,
     CENTER = 343,
     TOP_OR_BOT = 344,
     ALTAR_TYPE = 345,
     ALTAR_SUBTYPE = 346,
     UP_OR_DOWN = 347,
     ACTIVE_OR_INACTIVE = 348,
     MODRON_PORTAL_TYPE = 349,
     NPC_TYPE = 350,
     FOUNTAIN_TYPE = 351,
     SPECIAL_OBJECT_TYPE = 352,
     CMAP_TYPE = 353,
     FLOOR_SUBTYPE = 354,
     FLOOR_SUBTYPE_ID = 355,
     FLOOR_ID = 356,
     FLOOR_TYPE = 357,
     FLOOR_TYPE_ID = 358,
     DECORATION_ID = 359,
     DECORATION_TYPE = 360,
     DECORATION_DIR = 361,
     DECORATION_ITEM_STATE = 362,
     ELEMENTAL_ENCHANTMENT_TYPE = 363,
     EXCEPTIONALITY_TYPE = 364,
     EXCEPTIONALITY_ID = 365,
     ELEMENTAL_ENCHANTMENT_ID = 366,
     ENCHANTMENT_ID = 367,
     SECRET_DOOR_ID = 368,
     USES_UP_KEY_ID = 369,
     MYTHIC_PREFIX_TYPE = 370,
     MYTHIC_SUFFIX_TYPE = 371,
     MYTHIC_PREFIX_ID = 372,
     MYTHIC_SUFFIX_ID = 373,
     MATERIAL_ID = 374,
     MATERIAL_TYPE = 375,
     CHARGES_ID = 376,
     SPECIAL_QUALITY_ID = 377,
     SPEFLAGS_ID = 378,
     SUBROOM_ID = 379,
     NAME_ID = 380,
     FLAGS_ID = 381,
     FLAG_TYPE = 382,
     MON_ATTITUDE = 383,
     MON_ALERTNESS = 384,
     SUBTYPE_ID = 385,
     NON_PASSDOOR_ID = 386,
     CARPET_ID = 387,
     CARPET_PIECE_ID = 388,
     CARPET_TYPE = 389,
     MON_APPEARANCE = 390,
     ROOMDOOR_ID = 391,
     IF_ID = 392,
     ELSE_ID = 393,
     TERRAIN_ID = 394,
     HORIZ_OR_VERT = 395,
     REPLACE_TERRAIN_ID = 396,
     LOCATION_SUBTYPE_ID = 397,
     DOOR_SUBTYPE = 398,
     BRAZIER_SUBTYPE = 399,
     SIGNPOST_SUBTYPE = 400,
     TREE_SUBTYPE = 401,
     FOREST_ID = 402,
     FOREST_TYPE = 403,
     INITIALIZE_TYPE = 404,
     EXIT_ID = 405,
     SHUFFLE_ID = 406,
     MANUAL_TYPE_ID = 407,
     MANUAL_TYPE = 408,
     QUANTITY_ID = 409,
     BURIED_ID = 410,
     LOOP_ID = 411,
     FOR_ID = 412,
     TO_ID = 413,
     SWITCH_ID = 414,
     CASE_ID = 415,
     BREAK_ID = 416,
     DEFAULT_ID = 417,
     ERODED_ID = 418,
     TRAPPED_STATE = 419,
     RECHARGED_ID = 420,
     INVIS_ID = 421,
     GREASED_ID = 422,
     INDESTRUCTIBLE_ID = 423,
     FEMALE_ID = 424,
     MALE_ID = 425,
     WAITFORU_ID = 426,
     PROTECTOR_ID = 427,
     CANCELLED_ID = 428,
     REVIVED_ID = 429,
     AVENGE_ID = 430,
     FLEEING_ID = 431,
     BLINDED_ID = 432,
     MAXHP_ID = 433,
     LEVEL_ADJUSTMENT_ID = 434,
     KEEP_ORIGINAL_INVENTORY_ID = 435,
     PARALYZED_ID = 436,
     STUNNED_ID = 437,
     CONFUSED_ID = 438,
     SEENTRAPS_ID = 439,
     ALL_ID = 440,
     MONTYPE_ID = 441,
     OBJTYPE_ID = 442,
     TERTYPE_ID = 443,
     TERTYPE2_ID = 444,
     LEVER_EFFECT_TYPE = 445,
     SWITCHABLE_ID = 446,
     CONTINUOUSLY_USABLE_ID = 447,
     TARGET_ID = 448,
     TRAPTYPE_ID = 449,
     EFFECT_FLAG_ID = 450,
     GRAVE_ID = 451,
     BRAZIER_ID = 452,
     SIGNPOST_ID = 453,
     TREE_ID = 454,
     ERODEPROOF_ID = 455,
     FUNCTION_ID = 456,
     MSG_OUTPUT_TYPE = 457,
     COMPARE_TYPE = 458,
     UNKNOWN_TYPE = 459,
     rect_ID = 460,
     fillrect_ID = 461,
     line_ID = 462,
     randline_ID = 463,
     grow_ID = 464,
     selection_ID = 465,
     flood_ID = 466,
     rndcoord_ID = 467,
     circle_ID = 468,
     ellipse_ID = 469,
     filter_ID = 470,
     complement_ID = 471,
     gradient_ID = 472,
     GRADIENT_TYPE = 473,
     LIMITED = 474,
     HUMIDITY_TYPE = 475,
     STRING = 476,
     MAP_ID = 477,
     NQSTRING = 478,
     VARSTRING = 479,
     CFUNC = 480,
     CFUNC_INT = 481,
     CFUNC_STR = 482,
     CFUNC_COORD = 483,
     CFUNC_REGION = 484,
     VARSTRING_INT = 485,
     VARSTRING_INT_ARRAY = 486,
     VARSTRING_STRING = 487,
     VARSTRING_STRING_ARRAY = 488,
     VARSTRING_VAR = 489,
     VARSTRING_VAR_ARRAY = 490,
     VARSTRING_COORD = 491,
     VARSTRING_COORD_ARRAY = 492,
     VARSTRING_REGION = 493,
     VARSTRING_REGION_ARRAY = 494,
     VARSTRING_MAPCHAR = 495,
     VARSTRING_MAPCHAR_ARRAY = 496,
     VARSTRING_MONST = 497,
     VARSTRING_MONST_ARRAY = 498,
     VARSTRING_OBJ = 499,
     VARSTRING_OBJ_ARRAY = 500,
     VARSTRING_SEL = 501,
     VARSTRING_SEL_ARRAY = 502,
     METHOD_INT = 503,
     METHOD_INT_ARRAY = 504,
     METHOD_STRING = 505,
     METHOD_STRING_ARRAY = 506,
     METHOD_VAR = 507,
     METHOD_VAR_ARRAY = 508,
     METHOD_COORD = 509,
     METHOD_COORD_ARRAY = 510,
     METHOD_REGION = 511,
     METHOD_REGION_ARRAY = 512,
     METHOD_MAPCHAR = 513,
     METHOD_MAPCHAR_ARRAY = 514,
     METHOD_MONST = 515,
     METHOD_MONST_ARRAY = 516,
     METHOD_OBJ = 517,
     METHOD_OBJ_ARRAY = 518,
     METHOD_SEL = 519,
     METHOD_SEL_ARRAY = 520,
     DICE = 521
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
#line 357 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


