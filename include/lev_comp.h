
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
     MON_APPEARANCE = 383,
     ROOMDOOR_ID = 384,
     IF_ID = 385,
     ELSE_ID = 386,
     TERRAIN_ID = 387,
     HORIZ_OR_VERT = 388,
     REPLACE_TERRAIN_ID = 389,
     LOCATION_SUBTYPE_ID = 390,
     DOOR_SUBTYPE = 391,
     BRAZIER_SUBTYPE = 392,
     SIGNPOST_SUBTYPE = 393,
     TREE_SUBTYPE = 394,
     FOREST_ID = 395,
     FOREST_TYPE = 396,
     INITIALIZE_TYPE = 397,
     EXIT_ID = 398,
     SHUFFLE_ID = 399,
     MANUAL_TYPE_ID = 400,
     MANUAL_TYPE = 401,
     QUANTITY_ID = 402,
     BURIED_ID = 403,
     LOOP_ID = 404,
     FOR_ID = 405,
     TO_ID = 406,
     SWITCH_ID = 407,
     CASE_ID = 408,
     BREAK_ID = 409,
     DEFAULT_ID = 410,
     ERODED_ID = 411,
     TRAPPED_STATE = 412,
     RECHARGED_ID = 413,
     INVIS_ID = 414,
     GREASED_ID = 415,
     INDESTRUCTIBLE_ID = 416,
     FEMALE_ID = 417,
     MALE_ID = 418,
     WAITFORU_ID = 419,
     PROTECTOR_ID = 420,
     CANCELLED_ID = 421,
     REVIVED_ID = 422,
     AVENGE_ID = 423,
     FLEEING_ID = 424,
     BLINDED_ID = 425,
     MAXHP_ID = 426,
     LEVEL_ADJUSTMENT_ID = 427,
     KEEP_ORIGINAL_INVENTORY_ID = 428,
     PARALYZED_ID = 429,
     STUNNED_ID = 430,
     CONFUSED_ID = 431,
     SEENTRAPS_ID = 432,
     ALL_ID = 433,
     MONTYPE_ID = 434,
     OBJTYPE_ID = 435,
     TERTYPE_ID = 436,
     TERTYPE2_ID = 437,
     LEVER_EFFECT_TYPE = 438,
     SWITCHABLE_ID = 439,
     CONTINUOUSLY_USABLE_ID = 440,
     TARGET_ID = 441,
     TRAPTYPE_ID = 442,
     EFFECT_FLAG_ID = 443,
     GRAVE_ID = 444,
     BRAZIER_ID = 445,
     SIGNPOST_ID = 446,
     TREE_ID = 447,
     ERODEPROOF_ID = 448,
     FUNCTION_ID = 449,
     MSG_OUTPUT_TYPE = 450,
     COMPARE_TYPE = 451,
     UNKNOWN_TYPE = 452,
     rect_ID = 453,
     fillrect_ID = 454,
     line_ID = 455,
     randline_ID = 456,
     grow_ID = 457,
     selection_ID = 458,
     flood_ID = 459,
     rndcoord_ID = 460,
     circle_ID = 461,
     ellipse_ID = 462,
     filter_ID = 463,
     complement_ID = 464,
     gradient_ID = 465,
     GRADIENT_TYPE = 466,
     LIMITED = 467,
     HUMIDITY_TYPE = 468,
     STRING = 469,
     MAP_ID = 470,
     NQSTRING = 471,
     VARSTRING = 472,
     CFUNC = 473,
     CFUNC_INT = 474,
     CFUNC_STR = 475,
     CFUNC_COORD = 476,
     CFUNC_REGION = 477,
     VARSTRING_INT = 478,
     VARSTRING_INT_ARRAY = 479,
     VARSTRING_STRING = 480,
     VARSTRING_STRING_ARRAY = 481,
     VARSTRING_VAR = 482,
     VARSTRING_VAR_ARRAY = 483,
     VARSTRING_COORD = 484,
     VARSTRING_COORD_ARRAY = 485,
     VARSTRING_REGION = 486,
     VARSTRING_REGION_ARRAY = 487,
     VARSTRING_MAPCHAR = 488,
     VARSTRING_MAPCHAR_ARRAY = 489,
     VARSTRING_MONST = 490,
     VARSTRING_MONST_ARRAY = 491,
     VARSTRING_OBJ = 492,
     VARSTRING_OBJ_ARRAY = 493,
     VARSTRING_SEL = 494,
     VARSTRING_SEL_ARRAY = 495,
     METHOD_INT = 496,
     METHOD_INT_ARRAY = 497,
     METHOD_STRING = 498,
     METHOD_STRING_ARRAY = 499,
     METHOD_VAR = 500,
     METHOD_VAR_ARRAY = 501,
     METHOD_COORD = 502,
     METHOD_COORD_ARRAY = 503,
     METHOD_REGION = 504,
     METHOD_REGION_ARRAY = 505,
     METHOD_MAPCHAR = 506,
     METHOD_MAPCHAR_ARRAY = 507,
     METHOD_MONST = 508,
     METHOD_MONST_ARRAY = 509,
     METHOD_OBJ = 510,
     METHOD_OBJ_ARRAY = 511,
     METHOD_SEL = 512,
     METHOD_SEL_ARRAY = 513,
     DICE = 514
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
#line 350 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


