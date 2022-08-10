
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
     ELEMENTAL_ENCHANTMENT_TYPE = 357,
     EXCEPTIONALITY_TYPE = 358,
     EXCEPTIONALITY_ID = 359,
     ELEMENTAL_ENCHANTMENT_ID = 360,
     ENCHANTMENT_ID = 361,
     SECRET_DOOR_ID = 362,
     USES_UP_KEY_ID = 363,
     MYTHIC_PREFIX_TYPE = 364,
     MYTHIC_SUFFIX_TYPE = 365,
     MYTHIC_PREFIX_ID = 366,
     MYTHIC_SUFFIX_ID = 367,
     CHARGES_ID = 368,
     SPECIAL_QUALITY_ID = 369,
     SPEFLAGS_ID = 370,
     SUBROOM_ID = 371,
     NAME_ID = 372,
     FLAGS_ID = 373,
     FLAG_TYPE = 374,
     MON_ATTITUDE = 375,
     MON_ALERTNESS = 376,
     SUBTYPE_ID = 377,
     NON_PASSDOOR_ID = 378,
     MON_APPEARANCE = 379,
     ROOMDOOR_ID = 380,
     IF_ID = 381,
     ELSE_ID = 382,
     TERRAIN_ID = 383,
     HORIZ_OR_VERT = 384,
     REPLACE_TERRAIN_ID = 385,
     LOCATION_SUBTYPE_ID = 386,
     DOOR_SUBTYPE = 387,
     BRAZIER_SUBTYPE = 388,
     SIGNPOST_SUBTYPE = 389,
     TREE_SUBTYPE = 390,
     FOREST_ID = 391,
     FOREST_TYPE = 392,
     INITIALIZE_TYPE = 393,
     EXIT_ID = 394,
     SHUFFLE_ID = 395,
     MANUAL_TYPE_ID = 396,
     MANUAL_TYPE = 397,
     QUANTITY_ID = 398,
     BURIED_ID = 399,
     LOOP_ID = 400,
     FOR_ID = 401,
     TO_ID = 402,
     SWITCH_ID = 403,
     CASE_ID = 404,
     BREAK_ID = 405,
     DEFAULT_ID = 406,
     ERODED_ID = 407,
     TRAPPED_STATE = 408,
     RECHARGED_ID = 409,
     INVIS_ID = 410,
     GREASED_ID = 411,
     INDESTRUCTIBLE_ID = 412,
     FEMALE_ID = 413,
     MALE_ID = 414,
     WAITFORU_ID = 415,
     PROTECTOR_ID = 416,
     CANCELLED_ID = 417,
     REVIVED_ID = 418,
     AVENGE_ID = 419,
     FLEEING_ID = 420,
     BLINDED_ID = 421,
     MAXHP_ID = 422,
     LEVEL_ADJUSTMENT_ID = 423,
     KEEP_ORIGINAL_INVENTORY_ID = 424,
     PARALYZED_ID = 425,
     STUNNED_ID = 426,
     CONFUSED_ID = 427,
     SEENTRAPS_ID = 428,
     ALL_ID = 429,
     MONTYPE_ID = 430,
     OBJTYPE_ID = 431,
     TERTYPE_ID = 432,
     TERTYPE2_ID = 433,
     LEVER_EFFECT_TYPE = 434,
     SWITCHABLE_ID = 435,
     CONTINUOUSLY_USABLE_ID = 436,
     TARGET_ID = 437,
     TRAPTYPE_ID = 438,
     EFFECT_FLAG_ID = 439,
     GRAVE_ID = 440,
     BRAZIER_ID = 441,
     SIGNPOST_ID = 442,
     TREE_ID = 443,
     ERODEPROOF_ID = 444,
     FUNCTION_ID = 445,
     MSG_OUTPUT_TYPE = 446,
     COMPARE_TYPE = 447,
     UNKNOWN_TYPE = 448,
     rect_ID = 449,
     fillrect_ID = 450,
     line_ID = 451,
     randline_ID = 452,
     grow_ID = 453,
     selection_ID = 454,
     flood_ID = 455,
     rndcoord_ID = 456,
     circle_ID = 457,
     ellipse_ID = 458,
     filter_ID = 459,
     complement_ID = 460,
     gradient_ID = 461,
     GRADIENT_TYPE = 462,
     LIMITED = 463,
     HUMIDITY_TYPE = 464,
     STRING = 465,
     MAP_ID = 466,
     NQSTRING = 467,
     VARSTRING = 468,
     CFUNC = 469,
     CFUNC_INT = 470,
     CFUNC_STR = 471,
     CFUNC_COORD = 472,
     CFUNC_REGION = 473,
     VARSTRING_INT = 474,
     VARSTRING_INT_ARRAY = 475,
     VARSTRING_STRING = 476,
     VARSTRING_STRING_ARRAY = 477,
     VARSTRING_VAR = 478,
     VARSTRING_VAR_ARRAY = 479,
     VARSTRING_COORD = 480,
     VARSTRING_COORD_ARRAY = 481,
     VARSTRING_REGION = 482,
     VARSTRING_REGION_ARRAY = 483,
     VARSTRING_MAPCHAR = 484,
     VARSTRING_MAPCHAR_ARRAY = 485,
     VARSTRING_MONST = 486,
     VARSTRING_MONST_ARRAY = 487,
     VARSTRING_OBJ = 488,
     VARSTRING_OBJ_ARRAY = 489,
     VARSTRING_SEL = 490,
     VARSTRING_SEL_ARRAY = 491,
     METHOD_INT = 492,
     METHOD_INT_ARRAY = 493,
     METHOD_STRING = 494,
     METHOD_STRING_ARRAY = 495,
     METHOD_VAR = 496,
     METHOD_VAR_ARRAY = 497,
     METHOD_COORD = 498,
     METHOD_COORD_ARRAY = 499,
     METHOD_REGION = 500,
     METHOD_REGION_ARRAY = 501,
     METHOD_MAPCHAR = 502,
     METHOD_MAPCHAR_ARRAY = 503,
     METHOD_MONST = 504,
     METHOD_MONST_ARRAY = 505,
     METHOD_OBJ = 506,
     METHOD_OBJ_ARRAY = 507,
     METHOD_SEL = 508,
     METHOD_SEL_ARRAY = 509,
     DICE = 510
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
#line 346 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


