
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
     MATERIAL_ID = 372,
     MATERIAL_TYPE = 373,
     CHARGES_ID = 374,
     SPECIAL_QUALITY_ID = 375,
     SPEFLAGS_ID = 376,
     SUBROOM_ID = 377,
     NAME_ID = 378,
     FLAGS_ID = 379,
     FLAG_TYPE = 380,
     MON_ATTITUDE = 381,
     MON_ALERTNESS = 382,
     SUBTYPE_ID = 383,
     NON_PASSDOOR_ID = 384,
     CARPET_ID = 385,
     CARPET_PIECE_ID = 386,
     CARPET_TYPE = 387,
     MON_APPEARANCE = 388,
     ROOMDOOR_ID = 389,
     IF_ID = 390,
     ELSE_ID = 391,
     TERRAIN_ID = 392,
     HORIZ_OR_VERT = 393,
     REPLACE_TERRAIN_ID = 394,
     LOCATION_SUBTYPE_ID = 395,
     DOOR_SUBTYPE = 396,
     BRAZIER_SUBTYPE = 397,
     SIGNPOST_SUBTYPE = 398,
     TREE_SUBTYPE = 399,
     FOREST_ID = 400,
     FOREST_TYPE = 401,
     INITIALIZE_TYPE = 402,
     EXIT_ID = 403,
     SHUFFLE_ID = 404,
     MANUAL_TYPE_ID = 405,
     MANUAL_TYPE = 406,
     QUANTITY_ID = 407,
     BURIED_ID = 408,
     LOOP_ID = 409,
     FOR_ID = 410,
     TO_ID = 411,
     SWITCH_ID = 412,
     CASE_ID = 413,
     BREAK_ID = 414,
     DEFAULT_ID = 415,
     ERODED_ID = 416,
     TRAPPED_STATE = 417,
     RECHARGED_ID = 418,
     INVIS_ID = 419,
     GREASED_ID = 420,
     INDESTRUCTIBLE_ID = 421,
     FEMALE_ID = 422,
     MALE_ID = 423,
     WAITFORU_ID = 424,
     PROTECTOR_ID = 425,
     CANCELLED_ID = 426,
     REVIVED_ID = 427,
     AVENGE_ID = 428,
     FLEEING_ID = 429,
     BLINDED_ID = 430,
     MAXHP_ID = 431,
     LEVEL_ADJUSTMENT_ID = 432,
     KEEP_ORIGINAL_INVENTORY_ID = 433,
     PARALYZED_ID = 434,
     STUNNED_ID = 435,
     CONFUSED_ID = 436,
     SEENTRAPS_ID = 437,
     ALL_ID = 438,
     MONTYPE_ID = 439,
     OBJTYPE_ID = 440,
     TERTYPE_ID = 441,
     TERTYPE2_ID = 442,
     LEVER_EFFECT_TYPE = 443,
     SWITCHABLE_ID = 444,
     CONTINUOUSLY_USABLE_ID = 445,
     TARGET_ID = 446,
     TRAPTYPE_ID = 447,
     EFFECT_FLAG_ID = 448,
     GRAVE_ID = 449,
     BRAZIER_ID = 450,
     SIGNPOST_ID = 451,
     TREE_ID = 452,
     ERODEPROOF_ID = 453,
     FUNCTION_ID = 454,
     MSG_OUTPUT_TYPE = 455,
     COMPARE_TYPE = 456,
     UNKNOWN_TYPE = 457,
     rect_ID = 458,
     fillrect_ID = 459,
     line_ID = 460,
     randline_ID = 461,
     grow_ID = 462,
     selection_ID = 463,
     flood_ID = 464,
     rndcoord_ID = 465,
     circle_ID = 466,
     ellipse_ID = 467,
     filter_ID = 468,
     complement_ID = 469,
     gradient_ID = 470,
     GRADIENT_TYPE = 471,
     LIMITED = 472,
     HUMIDITY_TYPE = 473,
     STRING = 474,
     MAP_ID = 475,
     NQSTRING = 476,
     VARSTRING = 477,
     CFUNC = 478,
     CFUNC_INT = 479,
     CFUNC_STR = 480,
     CFUNC_COORD = 481,
     CFUNC_REGION = 482,
     VARSTRING_INT = 483,
     VARSTRING_INT_ARRAY = 484,
     VARSTRING_STRING = 485,
     VARSTRING_STRING_ARRAY = 486,
     VARSTRING_VAR = 487,
     VARSTRING_VAR_ARRAY = 488,
     VARSTRING_COORD = 489,
     VARSTRING_COORD_ARRAY = 490,
     VARSTRING_REGION = 491,
     VARSTRING_REGION_ARRAY = 492,
     VARSTRING_MAPCHAR = 493,
     VARSTRING_MAPCHAR_ARRAY = 494,
     VARSTRING_MONST = 495,
     VARSTRING_MONST_ARRAY = 496,
     VARSTRING_OBJ = 497,
     VARSTRING_OBJ_ARRAY = 498,
     VARSTRING_SEL = 499,
     VARSTRING_SEL_ARRAY = 500,
     METHOD_INT = 501,
     METHOD_INT_ARRAY = 502,
     METHOD_STRING = 503,
     METHOD_STRING_ARRAY = 504,
     METHOD_VAR = 505,
     METHOD_VAR_ARRAY = 506,
     METHOD_COORD = 507,
     METHOD_COORD_ARRAY = 508,
     METHOD_REGION = 509,
     METHOD_REGION_ARRAY = 510,
     METHOD_MAPCHAR = 511,
     METHOD_MAPCHAR_ARRAY = 512,
     METHOD_MONST = 513,
     METHOD_MONST_ARRAY = 514,
     METHOD_OBJ = 515,
     METHOD_OBJ_ARRAY = 516,
     METHOD_SEL = 517,
     METHOD_SEL_ARRAY = 518,
     DICE = 519
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
#line 355 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


