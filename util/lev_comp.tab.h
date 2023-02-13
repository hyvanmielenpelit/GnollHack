
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
     OBJECT_ID = 279,
     COBJECT_ID = 280,
     MONSTER_ID = 281,
     TRAP_ID = 282,
     DOOR_ID = 283,
     DRAWBRIDGE_ID = 284,
     MONSTER_GENERATION_ID = 285,
     object_ID = 286,
     monster_ID = 287,
     terrain_ID = 288,
     MAZEWALK_ID = 289,
     WALLIFY_ID = 290,
     REGION_ID = 291,
     SPECIAL_REGION_ID = 292,
     SPECIAL_LEVREGION_ID = 293,
     SPECIAL_REGION_TYPE = 294,
     NAMING_ID = 295,
     NAMING_TYPE = 296,
     FILLING = 297,
     IRREGULAR = 298,
     JOINED = 299,
     ALTAR_ID = 300,
     ANVIL_ID = 301,
     NPC_ID = 302,
     LADDER_ID = 303,
     STAIR_ID = 304,
     NON_DIGGABLE_ID = 305,
     NON_PASSWALL_ID = 306,
     ROOM_ID = 307,
     ARTIFACT_NAME_ID = 308,
     PORTAL_ID = 309,
     TELEPRT_ID = 310,
     BRANCH_ID = 311,
     LEV = 312,
     MINERALIZE_ID = 313,
     AGE_ID = 314,
     CORRIDOR_ID = 315,
     GOLD_ID = 316,
     ENGRAVING_ID = 317,
     FOUNTAIN_ID = 318,
     THRONE_ID = 319,
     MODRON_PORTAL_ID = 320,
     LEVEL_TELEPORTER_ID = 321,
     LEVEL_TELEPORT_DIRECTION_TYPE = 322,
     LEVEL_TELEPORT_END_TYPE = 323,
     POOL_ID = 324,
     SINK_ID = 325,
     NONE = 326,
     RAND_CORRIDOR_ID = 327,
     DOOR_STATE = 328,
     LIGHT_STATE = 329,
     CURSE_TYPE = 330,
     MYTHIC_TYPE = 331,
     ENGRAVING_TYPE = 332,
     KEYTYPE_ID = 333,
     LEVER_ID = 334,
     NO_PICKUP_ID = 335,
     DIRECTION = 336,
     RANDOM_TYPE = 337,
     RANDOM_TYPE_BRACKET = 338,
     A_REGISTER = 339,
     ALIGNMENT = 340,
     LEFT_OR_RIGHT = 341,
     CENTER = 342,
     TOP_OR_BOT = 343,
     ALTAR_TYPE = 344,
     ALTAR_SUBTYPE = 345,
     UP_OR_DOWN = 346,
     ACTIVE_OR_INACTIVE = 347,
     MODRON_PORTAL_TYPE = 348,
     NPC_TYPE = 349,
     FOUNTAIN_TYPE = 350,
     SPECIAL_OBJECT_TYPE = 351,
     CMAP_TYPE = 352,
     FLOOR_SUBTYPE = 353,
     FLOOR_SUBTYPE_ID = 354,
     FLOOR_ID = 355,
     FLOOR_TYPE = 356,
     FLOOR_TYPE_ID = 357,
     DECORATION_ID = 358,
     DECORATION_TYPE = 359,
     DECORATION_DIR = 360,
     DECORATION_ITEM_STATE = 361,
     ELEMENTAL_ENCHANTMENT_TYPE = 362,
     EXCEPTIONALITY_TYPE = 363,
     EXCEPTIONALITY_ID = 364,
     ELEMENTAL_ENCHANTMENT_ID = 365,
     ENCHANTMENT_ID = 366,
     SECRET_DOOR_ID = 367,
     USES_UP_KEY_ID = 368,
     MYTHIC_PREFIX_TYPE = 369,
     MYTHIC_SUFFIX_TYPE = 370,
     MYTHIC_PREFIX_ID = 371,
     MYTHIC_SUFFIX_ID = 372,
     MATERIAL_ID = 373,
     MATERIAL_TYPE = 374,
     CHARGES_ID = 375,
     SPECIAL_QUALITY_ID = 376,
     SPEFLAGS_ID = 377,
     SUBROOM_ID = 378,
     NAME_ID = 379,
     FLAGS_ID = 380,
     FLAG_TYPE = 381,
     MON_ATTITUDE = 382,
     MON_ALERTNESS = 383,
     SUBTYPE_ID = 384,
     NON_PASSDOOR_ID = 385,
     CARPET_ID = 386,
     CARPET_PIECE_ID = 387,
     CARPET_TYPE = 388,
     MON_APPEARANCE = 389,
     ROOMDOOR_ID = 390,
     IF_ID = 391,
     ELSE_ID = 392,
     TERRAIN_ID = 393,
     HORIZ_OR_VERT = 394,
     REPLACE_TERRAIN_ID = 395,
     LOCATION_SUBTYPE_ID = 396,
     DOOR_SUBTYPE = 397,
     BRAZIER_SUBTYPE = 398,
     SIGNPOST_SUBTYPE = 399,
     TREE_SUBTYPE = 400,
     FOREST_ID = 401,
     FOREST_TYPE = 402,
     INITIALIZE_TYPE = 403,
     EXIT_ID = 404,
     SHUFFLE_ID = 405,
     MANUAL_TYPE_ID = 406,
     MANUAL_TYPE = 407,
     QUANTITY_ID = 408,
     BURIED_ID = 409,
     LOOP_ID = 410,
     FOR_ID = 411,
     TO_ID = 412,
     SWITCH_ID = 413,
     CASE_ID = 414,
     BREAK_ID = 415,
     DEFAULT_ID = 416,
     ERODED_ID = 417,
     TRAPPED_STATE = 418,
     RECHARGED_ID = 419,
     INVIS_ID = 420,
     GREASED_ID = 421,
     INDESTRUCTIBLE_ID = 422,
     FEMALE_ID = 423,
     MALE_ID = 424,
     WAITFORU_ID = 425,
     PROTECTOR_ID = 426,
     CANCELLED_ID = 427,
     REVIVED_ID = 428,
     AVENGE_ID = 429,
     FLEEING_ID = 430,
     BLINDED_ID = 431,
     MAXHP_ID = 432,
     LEVEL_ADJUSTMENT_ID = 433,
     KEEP_ORIGINAL_INVENTORY_ID = 434,
     PARALYZED_ID = 435,
     STUNNED_ID = 436,
     CONFUSED_ID = 437,
     SEENTRAPS_ID = 438,
     ALL_ID = 439,
     MONTYPE_ID = 440,
     OBJTYPE_ID = 441,
     TERTYPE_ID = 442,
     TERTYPE2_ID = 443,
     LEVER_EFFECT_TYPE = 444,
     SWITCHABLE_ID = 445,
     CONTINUOUSLY_USABLE_ID = 446,
     TARGET_ID = 447,
     TRAPTYPE_ID = 448,
     EFFECT_FLAG_ID = 449,
     GRAVE_ID = 450,
     BRAZIER_ID = 451,
     SIGNPOST_ID = 452,
     TREE_ID = 453,
     ERODEPROOF_ID = 454,
     FUNCTION_ID = 455,
     MSG_OUTPUT_TYPE = 456,
     COMPARE_TYPE = 457,
     UNKNOWN_TYPE = 458,
     rect_ID = 459,
     fillrect_ID = 460,
     line_ID = 461,
     randline_ID = 462,
     grow_ID = 463,
     selection_ID = 464,
     flood_ID = 465,
     rndcoord_ID = 466,
     circle_ID = 467,
     ellipse_ID = 468,
     filter_ID = 469,
     complement_ID = 470,
     gradient_ID = 471,
     GRADIENT_TYPE = 472,
     LIMITED = 473,
     HUMIDITY_TYPE = 474,
     STRING = 475,
     MAP_ID = 476,
     NQSTRING = 477,
     VARSTRING = 478,
     CFUNC = 479,
     CFUNC_INT = 480,
     CFUNC_STR = 481,
     CFUNC_COORD = 482,
     CFUNC_REGION = 483,
     VARSTRING_INT = 484,
     VARSTRING_INT_ARRAY = 485,
     VARSTRING_STRING = 486,
     VARSTRING_STRING_ARRAY = 487,
     VARSTRING_VAR = 488,
     VARSTRING_VAR_ARRAY = 489,
     VARSTRING_COORD = 490,
     VARSTRING_COORD_ARRAY = 491,
     VARSTRING_REGION = 492,
     VARSTRING_REGION_ARRAY = 493,
     VARSTRING_MAPCHAR = 494,
     VARSTRING_MAPCHAR_ARRAY = 495,
     VARSTRING_MONST = 496,
     VARSTRING_MONST_ARRAY = 497,
     VARSTRING_OBJ = 498,
     VARSTRING_OBJ_ARRAY = 499,
     VARSTRING_SEL = 500,
     VARSTRING_SEL_ARRAY = 501,
     METHOD_INT = 502,
     METHOD_INT_ARRAY = 503,
     METHOD_STRING = 504,
     METHOD_STRING_ARRAY = 505,
     METHOD_VAR = 506,
     METHOD_VAR_ARRAY = 507,
     METHOD_COORD = 508,
     METHOD_COORD_ARRAY = 509,
     METHOD_REGION = 510,
     METHOD_REGION_ARRAY = 511,
     METHOD_MAPCHAR = 512,
     METHOD_MAPCHAR_ARRAY = 513,
     METHOD_MONST = 514,
     METHOD_MONST_ARRAY = 515,
     METHOD_OBJ = 516,
     METHOD_OBJ_ARRAY = 517,
     METHOD_SEL = 518,
     METHOD_SEL_ARRAY = 519,
     DICE = 520
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
#line 356 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


