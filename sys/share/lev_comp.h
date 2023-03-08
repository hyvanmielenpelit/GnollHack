
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
     ELEMENTAL_ENCHANTMENT_TYPE = 365,
     EXCEPTIONALITY_TYPE = 366,
     EXCEPTIONALITY_ID = 367,
     ELEMENTAL_ENCHANTMENT_ID = 368,
     ENCHANTMENT_ID = 369,
     SECRET_DOOR_ID = 370,
     USES_UP_KEY_ID = 371,
     MYTHIC_PREFIX_TYPE = 372,
     MYTHIC_SUFFIX_TYPE = 373,
     MYTHIC_PREFIX_ID = 374,
     MYTHIC_SUFFIX_ID = 375,
     MATERIAL_ID = 376,
     MATERIAL_TYPE = 377,
     CHARGES_ID = 378,
     SPECIAL_QUALITY_ID = 379,
     SPEFLAGS_ID = 380,
     SUBROOM_ID = 381,
     NAME_ID = 382,
     FLAGS_ID = 383,
     FLAG_TYPE = 384,
     MON_ATTITUDE = 385,
     MON_ALERTNESS = 386,
     SUBTYPE_ID = 387,
     NON_PASSDOOR_ID = 388,
     CARPET_ID = 389,
     CARPET_PIECE_ID = 390,
     CARPET_TYPE = 391,
     MON_APPEARANCE = 392,
     ROOMDOOR_ID = 393,
     IF_ID = 394,
     ELSE_ID = 395,
     TERRAIN_ID = 396,
     HORIZ_OR_VERT = 397,
     REPLACE_TERRAIN_ID = 398,
     LOCATION_SUBTYPE_ID = 399,
     DOOR_SUBTYPE = 400,
     BRAZIER_SUBTYPE = 401,
     SIGNPOST_SUBTYPE = 402,
     TREE_SUBTYPE = 403,
     FOREST_ID = 404,
     FOREST_TYPE = 405,
     INITIALIZE_TYPE = 406,
     EXIT_ID = 407,
     SHUFFLE_ID = 408,
     MANUAL_TYPE_ID = 409,
     MANUAL_TYPE = 410,
     QUANTITY_ID = 411,
     BURIED_ID = 412,
     LOOP_ID = 413,
     FOR_ID = 414,
     TO_ID = 415,
     SWITCH_ID = 416,
     CASE_ID = 417,
     BREAK_ID = 418,
     DEFAULT_ID = 419,
     ERODED_ID = 420,
     TRAPPED_STATE = 421,
     RECHARGED_ID = 422,
     INVIS_ID = 423,
     GREASED_ID = 424,
     INDESTRUCTIBLE_ID = 425,
     FEMALE_ID = 426,
     MALE_ID = 427,
     WAITFORU_ID = 428,
     PROTECTOR_ID = 429,
     CANCELLED_ID = 430,
     REVIVED_ID = 431,
     AVENGE_ID = 432,
     FLEEING_ID = 433,
     BLINDED_ID = 434,
     MAXHP_ID = 435,
     LEVEL_ADJUSTMENT_ID = 436,
     KEEP_ORIGINAL_INVENTORY_ID = 437,
     PARALYZED_ID = 438,
     STUNNED_ID = 439,
     CONFUSED_ID = 440,
     SEENTRAPS_ID = 441,
     ALL_ID = 442,
     MONTYPE_ID = 443,
     OBJTYPE_ID = 444,
     TERTYPE_ID = 445,
     TERTYPE2_ID = 446,
     LEVER_EFFECT_TYPE = 447,
     SWITCHABLE_ID = 448,
     CONTINUOUSLY_USABLE_ID = 449,
     TARGET_ID = 450,
     TRAPTYPE_ID = 451,
     EFFECT_FLAG_ID = 452,
     GRAVE_ID = 453,
     BRAZIER_ID = 454,
     SIGNPOST_ID = 455,
     TREE_ID = 456,
     ERODEPROOF_ID = 457,
     FUNCTION_ID = 458,
     MSG_OUTPUT_TYPE = 459,
     COMPARE_TYPE = 460,
     UNKNOWN_TYPE = 461,
     rect_ID = 462,
     fillrect_ID = 463,
     line_ID = 464,
     randline_ID = 465,
     grow_ID = 466,
     selection_ID = 467,
     flood_ID = 468,
     rndcoord_ID = 469,
     circle_ID = 470,
     ellipse_ID = 471,
     filter_ID = 472,
     complement_ID = 473,
     gradient_ID = 474,
     GRADIENT_TYPE = 475,
     LIMITED = 476,
     HUMIDITY_TYPE = 477,
     STRING = 478,
     MAP_ID = 479,
     NQSTRING = 480,
     VARSTRING = 481,
     CFUNC = 482,
     CFUNC_INT = 483,
     CFUNC_STR = 484,
     CFUNC_COORD = 485,
     CFUNC_REGION = 486,
     VARSTRING_INT = 487,
     VARSTRING_INT_ARRAY = 488,
     VARSTRING_STRING = 489,
     VARSTRING_STRING_ARRAY = 490,
     VARSTRING_VAR = 491,
     VARSTRING_VAR_ARRAY = 492,
     VARSTRING_COORD = 493,
     VARSTRING_COORD_ARRAY = 494,
     VARSTRING_REGION = 495,
     VARSTRING_REGION_ARRAY = 496,
     VARSTRING_MAPCHAR = 497,
     VARSTRING_MAPCHAR_ARRAY = 498,
     VARSTRING_MONST = 499,
     VARSTRING_MONST_ARRAY = 500,
     VARSTRING_OBJ = 501,
     VARSTRING_OBJ_ARRAY = 502,
     VARSTRING_SEL = 503,
     VARSTRING_SEL_ARRAY = 504,
     METHOD_INT = 505,
     METHOD_INT_ARRAY = 506,
     METHOD_STRING = 507,
     METHOD_STRING_ARRAY = 508,
     METHOD_VAR = 509,
     METHOD_VAR_ARRAY = 510,
     METHOD_COORD = 511,
     METHOD_COORD_ARRAY = 512,
     METHOD_REGION = 513,
     METHOD_REGION_ARRAY = 514,
     METHOD_MAPCHAR = 515,
     METHOD_MAPCHAR_ARRAY = 516,
     METHOD_MONST = 517,
     METHOD_MONST_ARRAY = 518,
     METHOD_OBJ = 519,
     METHOD_OBJ_ARRAY = 520,
     METHOD_SEL = 521,
     METHOD_SEL_ARRAY = 522,
     DICE = 523
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
#line 359 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


