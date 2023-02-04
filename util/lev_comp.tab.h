
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
     MAZE_ID = 270,
     LEVEL_ID = 271,
     LEV_INIT_ID = 272,
     TILESET_ID = 273,
     GEOMETRY_ID = 274,
     NOMAP_ID = 275,
     BOUNDARY_TYPE_ID = 276,
     SPECIAL_TILESET_ID = 277,
     OBJECT_ID = 278,
     COBJECT_ID = 279,
     MONSTER_ID = 280,
     TRAP_ID = 281,
     DOOR_ID = 282,
     DRAWBRIDGE_ID = 283,
     MONSTER_GENERATION_ID = 284,
     object_ID = 285,
     monster_ID = 286,
     terrain_ID = 287,
     MAZEWALK_ID = 288,
     WALLIFY_ID = 289,
     REGION_ID = 290,
     SPECIAL_REGION_ID = 291,
     SPECIAL_LEVREGION_ID = 292,
     SPECIAL_REGION_TYPE = 293,
     NAMING_ID = 294,
     NAMING_TYPE = 295,
     FILLING = 296,
     IRREGULAR = 297,
     JOINED = 298,
     ALTAR_ID = 299,
     ANVIL_ID = 300,
     NPC_ID = 301,
     LADDER_ID = 302,
     STAIR_ID = 303,
     NON_DIGGABLE_ID = 304,
     NON_PASSWALL_ID = 305,
     ROOM_ID = 306,
     ARTIFACT_NAME_ID = 307,
     PORTAL_ID = 308,
     TELEPRT_ID = 309,
     BRANCH_ID = 310,
     LEV = 311,
     MINERALIZE_ID = 312,
     AGE_ID = 313,
     CORRIDOR_ID = 314,
     GOLD_ID = 315,
     ENGRAVING_ID = 316,
     FOUNTAIN_ID = 317,
     THRONE_ID = 318,
     MODRON_PORTAL_ID = 319,
     LEVEL_TELEPORTER_ID = 320,
     LEVEL_TELEPORT_DIRECTION_TYPE = 321,
     LEVEL_TELEPORT_END_TYPE = 322,
     POOL_ID = 323,
     SINK_ID = 324,
     NONE = 325,
     RAND_CORRIDOR_ID = 326,
     DOOR_STATE = 327,
     LIGHT_STATE = 328,
     CURSE_TYPE = 329,
     MYTHIC_TYPE = 330,
     ENGRAVING_TYPE = 331,
     KEYTYPE_ID = 332,
     LEVER_ID = 333,
     NO_PICKUP_ID = 334,
     DIRECTION = 335,
     RANDOM_TYPE = 336,
     RANDOM_TYPE_BRACKET = 337,
     A_REGISTER = 338,
     ALIGNMENT = 339,
     LEFT_OR_RIGHT = 340,
     CENTER = 341,
     TOP_OR_BOT = 342,
     ALTAR_TYPE = 343,
     ALTAR_SUBTYPE = 344,
     UP_OR_DOWN = 345,
     ACTIVE_OR_INACTIVE = 346,
     MODRON_PORTAL_TYPE = 347,
     NPC_TYPE = 348,
     FOUNTAIN_TYPE = 349,
     SPECIAL_OBJECT_TYPE = 350,
     CMAP_TYPE = 351,
     FLOOR_SUBTYPE = 352,
     FLOOR_SUBTYPE_ID = 353,
     FLOOR_ID = 354,
     FLOOR_TYPE = 355,
     FLOOR_TYPE_ID = 356,
     DECORATION_ID = 357,
     DECORATION_TYPE = 358,
     DECORATION_DIR = 359,
     DECORATION_ITEM_STATE = 360,
     ELEMENTAL_ENCHANTMENT_TYPE = 361,
     EXCEPTIONALITY_TYPE = 362,
     EXCEPTIONALITY_ID = 363,
     ELEMENTAL_ENCHANTMENT_ID = 364,
     ENCHANTMENT_ID = 365,
     SECRET_DOOR_ID = 366,
     USES_UP_KEY_ID = 367,
     MYTHIC_PREFIX_TYPE = 368,
     MYTHIC_SUFFIX_TYPE = 369,
     MYTHIC_PREFIX_ID = 370,
     MYTHIC_SUFFIX_ID = 371,
     CHARGES_ID = 372,
     SPECIAL_QUALITY_ID = 373,
     SPEFLAGS_ID = 374,
     SUBROOM_ID = 375,
     NAME_ID = 376,
     FLAGS_ID = 377,
     FLAG_TYPE = 378,
     MON_ATTITUDE = 379,
     MON_ALERTNESS = 380,
     SUBTYPE_ID = 381,
     NON_PASSDOOR_ID = 382,
     CARPET_ID = 383,
     CARPET_PIECE_ID = 384,
     CARPET_TYPE = 385,
     MON_APPEARANCE = 386,
     ROOMDOOR_ID = 387,
     IF_ID = 388,
     ELSE_ID = 389,
     TERRAIN_ID = 390,
     HORIZ_OR_VERT = 391,
     REPLACE_TERRAIN_ID = 392,
     LOCATION_SUBTYPE_ID = 393,
     DOOR_SUBTYPE = 394,
     BRAZIER_SUBTYPE = 395,
     SIGNPOST_SUBTYPE = 396,
     TREE_SUBTYPE = 397,
     FOREST_ID = 398,
     FOREST_TYPE = 399,
     INITIALIZE_TYPE = 400,
     EXIT_ID = 401,
     SHUFFLE_ID = 402,
     MANUAL_TYPE_ID = 403,
     MANUAL_TYPE = 404,
     QUANTITY_ID = 405,
     BURIED_ID = 406,
     LOOP_ID = 407,
     FOR_ID = 408,
     TO_ID = 409,
     SWITCH_ID = 410,
     CASE_ID = 411,
     BREAK_ID = 412,
     DEFAULT_ID = 413,
     ERODED_ID = 414,
     TRAPPED_STATE = 415,
     RECHARGED_ID = 416,
     INVIS_ID = 417,
     GREASED_ID = 418,
     INDESTRUCTIBLE_ID = 419,
     FEMALE_ID = 420,
     MALE_ID = 421,
     WAITFORU_ID = 422,
     PROTECTOR_ID = 423,
     CANCELLED_ID = 424,
     REVIVED_ID = 425,
     AVENGE_ID = 426,
     FLEEING_ID = 427,
     BLINDED_ID = 428,
     MAXHP_ID = 429,
     LEVEL_ADJUSTMENT_ID = 430,
     KEEP_ORIGINAL_INVENTORY_ID = 431,
     PARALYZED_ID = 432,
     STUNNED_ID = 433,
     CONFUSED_ID = 434,
     SEENTRAPS_ID = 435,
     ALL_ID = 436,
     MONTYPE_ID = 437,
     OBJTYPE_ID = 438,
     TERTYPE_ID = 439,
     TERTYPE2_ID = 440,
     LEVER_EFFECT_TYPE = 441,
     SWITCHABLE_ID = 442,
     CONTINUOUSLY_USABLE_ID = 443,
     TARGET_ID = 444,
     TRAPTYPE_ID = 445,
     EFFECT_FLAG_ID = 446,
     GRAVE_ID = 447,
     BRAZIER_ID = 448,
     SIGNPOST_ID = 449,
     TREE_ID = 450,
     ERODEPROOF_ID = 451,
     FUNCTION_ID = 452,
     MSG_OUTPUT_TYPE = 453,
     COMPARE_TYPE = 454,
     UNKNOWN_TYPE = 455,
     rect_ID = 456,
     fillrect_ID = 457,
     line_ID = 458,
     randline_ID = 459,
     grow_ID = 460,
     selection_ID = 461,
     flood_ID = 462,
     rndcoord_ID = 463,
     circle_ID = 464,
     ellipse_ID = 465,
     filter_ID = 466,
     complement_ID = 467,
     gradient_ID = 468,
     GRADIENT_TYPE = 469,
     LIMITED = 470,
     HUMIDITY_TYPE = 471,
     STRING = 472,
     MAP_ID = 473,
     NQSTRING = 474,
     VARSTRING = 475,
     CFUNC = 476,
     CFUNC_INT = 477,
     CFUNC_STR = 478,
     CFUNC_COORD = 479,
     CFUNC_REGION = 480,
     VARSTRING_INT = 481,
     VARSTRING_INT_ARRAY = 482,
     VARSTRING_STRING = 483,
     VARSTRING_STRING_ARRAY = 484,
     VARSTRING_VAR = 485,
     VARSTRING_VAR_ARRAY = 486,
     VARSTRING_COORD = 487,
     VARSTRING_COORD_ARRAY = 488,
     VARSTRING_REGION = 489,
     VARSTRING_REGION_ARRAY = 490,
     VARSTRING_MAPCHAR = 491,
     VARSTRING_MAPCHAR_ARRAY = 492,
     VARSTRING_MONST = 493,
     VARSTRING_MONST_ARRAY = 494,
     VARSTRING_OBJ = 495,
     VARSTRING_OBJ_ARRAY = 496,
     VARSTRING_SEL = 497,
     VARSTRING_SEL_ARRAY = 498,
     METHOD_INT = 499,
     METHOD_INT_ARRAY = 500,
     METHOD_STRING = 501,
     METHOD_STRING_ARRAY = 502,
     METHOD_VAR = 503,
     METHOD_VAR_ARRAY = 504,
     METHOD_COORD = 505,
     METHOD_COORD_ARRAY = 506,
     METHOD_REGION = 507,
     METHOD_REGION_ARRAY = 508,
     METHOD_MAPCHAR = 509,
     METHOD_MAPCHAR_ARRAY = 510,
     METHOD_MONST = 511,
     METHOD_MONST_ARRAY = 512,
     METHOD_OBJ = 513,
     METHOD_OBJ_ARRAY = 514,
     METHOD_SEL = 515,
     METHOD_SEL_ARRAY = 516,
     DICE = 517
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
#line 353 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


