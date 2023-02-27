
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
     ELEMENTAL_ENCHANTMENT_TYPE = 364,
     EXCEPTIONALITY_TYPE = 365,
     EXCEPTIONALITY_ID = 366,
     ELEMENTAL_ENCHANTMENT_ID = 367,
     ENCHANTMENT_ID = 368,
     SECRET_DOOR_ID = 369,
     USES_UP_KEY_ID = 370,
     MYTHIC_PREFIX_TYPE = 371,
     MYTHIC_SUFFIX_TYPE = 372,
     MYTHIC_PREFIX_ID = 373,
     MYTHIC_SUFFIX_ID = 374,
     MATERIAL_ID = 375,
     MATERIAL_TYPE = 376,
     CHARGES_ID = 377,
     SPECIAL_QUALITY_ID = 378,
     SPEFLAGS_ID = 379,
     SUBROOM_ID = 380,
     NAME_ID = 381,
     FLAGS_ID = 382,
     FLAG_TYPE = 383,
     MON_ATTITUDE = 384,
     MON_ALERTNESS = 385,
     SUBTYPE_ID = 386,
     NON_PASSDOOR_ID = 387,
     CARPET_ID = 388,
     CARPET_PIECE_ID = 389,
     CARPET_TYPE = 390,
     MON_APPEARANCE = 391,
     ROOMDOOR_ID = 392,
     IF_ID = 393,
     ELSE_ID = 394,
     TERRAIN_ID = 395,
     HORIZ_OR_VERT = 396,
     REPLACE_TERRAIN_ID = 397,
     LOCATION_SUBTYPE_ID = 398,
     DOOR_SUBTYPE = 399,
     BRAZIER_SUBTYPE = 400,
     SIGNPOST_SUBTYPE = 401,
     TREE_SUBTYPE = 402,
     FOREST_ID = 403,
     FOREST_TYPE = 404,
     INITIALIZE_TYPE = 405,
     EXIT_ID = 406,
     SHUFFLE_ID = 407,
     MANUAL_TYPE_ID = 408,
     MANUAL_TYPE = 409,
     QUANTITY_ID = 410,
     BURIED_ID = 411,
     LOOP_ID = 412,
     FOR_ID = 413,
     TO_ID = 414,
     SWITCH_ID = 415,
     CASE_ID = 416,
     BREAK_ID = 417,
     DEFAULT_ID = 418,
     ERODED_ID = 419,
     TRAPPED_STATE = 420,
     RECHARGED_ID = 421,
     INVIS_ID = 422,
     GREASED_ID = 423,
     INDESTRUCTIBLE_ID = 424,
     FEMALE_ID = 425,
     MALE_ID = 426,
     WAITFORU_ID = 427,
     PROTECTOR_ID = 428,
     CANCELLED_ID = 429,
     REVIVED_ID = 430,
     AVENGE_ID = 431,
     FLEEING_ID = 432,
     BLINDED_ID = 433,
     MAXHP_ID = 434,
     LEVEL_ADJUSTMENT_ID = 435,
     KEEP_ORIGINAL_INVENTORY_ID = 436,
     PARALYZED_ID = 437,
     STUNNED_ID = 438,
     CONFUSED_ID = 439,
     SEENTRAPS_ID = 440,
     ALL_ID = 441,
     MONTYPE_ID = 442,
     OBJTYPE_ID = 443,
     TERTYPE_ID = 444,
     TERTYPE2_ID = 445,
     LEVER_EFFECT_TYPE = 446,
     SWITCHABLE_ID = 447,
     CONTINUOUSLY_USABLE_ID = 448,
     TARGET_ID = 449,
     TRAPTYPE_ID = 450,
     EFFECT_FLAG_ID = 451,
     GRAVE_ID = 452,
     BRAZIER_ID = 453,
     SIGNPOST_ID = 454,
     TREE_ID = 455,
     ERODEPROOF_ID = 456,
     FUNCTION_ID = 457,
     MSG_OUTPUT_TYPE = 458,
     COMPARE_TYPE = 459,
     UNKNOWN_TYPE = 460,
     rect_ID = 461,
     fillrect_ID = 462,
     line_ID = 463,
     randline_ID = 464,
     grow_ID = 465,
     selection_ID = 466,
     flood_ID = 467,
     rndcoord_ID = 468,
     circle_ID = 469,
     ellipse_ID = 470,
     filter_ID = 471,
     complement_ID = 472,
     gradient_ID = 473,
     GRADIENT_TYPE = 474,
     LIMITED = 475,
     HUMIDITY_TYPE = 476,
     STRING = 477,
     MAP_ID = 478,
     NQSTRING = 479,
     VARSTRING = 480,
     CFUNC = 481,
     CFUNC_INT = 482,
     CFUNC_STR = 483,
     CFUNC_COORD = 484,
     CFUNC_REGION = 485,
     VARSTRING_INT = 486,
     VARSTRING_INT_ARRAY = 487,
     VARSTRING_STRING = 488,
     VARSTRING_STRING_ARRAY = 489,
     VARSTRING_VAR = 490,
     VARSTRING_VAR_ARRAY = 491,
     VARSTRING_COORD = 492,
     VARSTRING_COORD_ARRAY = 493,
     VARSTRING_REGION = 494,
     VARSTRING_REGION_ARRAY = 495,
     VARSTRING_MAPCHAR = 496,
     VARSTRING_MAPCHAR_ARRAY = 497,
     VARSTRING_MONST = 498,
     VARSTRING_MONST_ARRAY = 499,
     VARSTRING_OBJ = 500,
     VARSTRING_OBJ_ARRAY = 501,
     VARSTRING_SEL = 502,
     VARSTRING_SEL_ARRAY = 503,
     METHOD_INT = 504,
     METHOD_INT_ARRAY = 505,
     METHOD_STRING = 506,
     METHOD_STRING_ARRAY = 507,
     METHOD_VAR = 508,
     METHOD_VAR_ARRAY = 509,
     METHOD_COORD = 510,
     METHOD_COORD_ARRAY = 511,
     METHOD_REGION = 512,
     METHOD_REGION_ARRAY = 513,
     METHOD_MAPCHAR = 514,
     METHOD_MAPCHAR_ARRAY = 515,
     METHOD_MONST = 516,
     METHOD_MONST_ARRAY = 517,
     METHOD_OBJ = 518,
     METHOD_OBJ_ARRAY = 519,
     METHOD_SEL = 520,
     METHOD_SEL_ARRAY = 521,
     DICE = 522
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
#line 358 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


