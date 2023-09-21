
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
     MESSAGE_ATTR = 271,
     MESSAGE_COLOR = 272,
     MESSAGE_SOUND_TYPE = 273,
     MESSAGE_SOUND_ID = 274,
     MAZE_ID = 275,
     LEVEL_ID = 276,
     LEV_INIT_ID = 277,
     TILESET_ID = 278,
     GEOMETRY_ID = 279,
     NOMAP_ID = 280,
     BOUNDARY_TYPE_ID = 281,
     SPECIAL_TILESET_ID = 282,
     TILESET_PARAM_ID = 283,
     DECOTYP_ID = 284,
     OBJECT_ID = 285,
     COBJECT_ID = 286,
     MONSTER_ID = 287,
     TRAP_ID = 288,
     DOOR_ID = 289,
     DRAWBRIDGE_ID = 290,
     MONSTER_GENERATION_ID = 291,
     object_ID = 292,
     monster_ID = 293,
     terrain_ID = 294,
     MAZEWALK_ID = 295,
     WALLIFY_ID = 296,
     REGION_ID = 297,
     SPECIAL_REGION_ID = 298,
     SPECIAL_LEVREGION_ID = 299,
     SPECIAL_REGION_TYPE = 300,
     NAMING_ID = 301,
     NAMING_TYPE = 302,
     FILLING = 303,
     IRREGULAR = 304,
     JOINED = 305,
     ALTAR_ID = 306,
     ANVIL_ID = 307,
     NPC_ID = 308,
     LADDER_ID = 309,
     STAIR_ID = 310,
     NON_DIGGABLE_ID = 311,
     NON_PASSWALL_ID = 312,
     ROOM_ID = 313,
     ARTIFACT_NAME_ID = 314,
     PORTAL_ID = 315,
     TELEPRT_ID = 316,
     BRANCH_ID = 317,
     LEV = 318,
     MINERALIZE_ID = 319,
     AGE_ID = 320,
     CORRIDOR_ID = 321,
     GOLD_ID = 322,
     ENGRAVING_ID = 323,
     FOUNTAIN_ID = 324,
     THRONE_ID = 325,
     MODRON_PORTAL_ID = 326,
     LEVEL_TELEPORTER_ID = 327,
     LEVEL_TELEPORT_DIRECTION_TYPE = 328,
     LEVEL_TELEPORT_END_TYPE = 329,
     POOL_ID = 330,
     SINK_ID = 331,
     NONE = 332,
     RAND_CORRIDOR_ID = 333,
     DOOR_STATE = 334,
     LIGHT_STATE = 335,
     CURSE_TYPE = 336,
     MYTHIC_TYPE = 337,
     ENGRAVING_TYPE = 338,
     KEYTYPE_ID = 339,
     LEVER_ID = 340,
     NO_PICKUP_ID = 341,
     DIRECTION = 342,
     RANDOM_TYPE = 343,
     RANDOM_TYPE_BRACKET = 344,
     A_REGISTER = 345,
     ALIGNMENT = 346,
     LEFT_OR_RIGHT = 347,
     CENTER = 348,
     TOP_OR_BOT = 349,
     ALTAR_TYPE = 350,
     ALTAR_SUBTYPE = 351,
     UP_OR_DOWN = 352,
     ACTIVE_OR_INACTIVE = 353,
     MODRON_PORTAL_TYPE = 354,
     NPC_TYPE = 355,
     FOUNTAIN_TYPE = 356,
     SPECIAL_OBJECT_TYPE = 357,
     CMAP_TYPE = 358,
     FLOOR_SUBTYPE = 359,
     FLOOR_SUBTYPE_ID = 360,
     FLOOR_ID = 361,
     FLOOR_TYPE = 362,
     FLOOR_TYPE_ID = 363,
     DECORATION_ID = 364,
     DECORATION_TYPE = 365,
     DECORATION_DIR = 366,
     DECORATION_ITEM_STATE = 367,
     PAINTING_TYPE = 368,
     BANNER_TYPE = 369,
     WALL_SCULPTURE_TYPE = 370,
     ELEMENTAL_ENCHANTMENT_TYPE = 371,
     EXCEPTIONALITY_TYPE = 372,
     EXCEPTIONALITY_ID = 373,
     ELEMENTAL_ENCHANTMENT_ID = 374,
     ENCHANTMENT_ID = 375,
     SECRET_DOOR_ID = 376,
     USES_UP_KEY_ID = 377,
     MYTHIC_PREFIX_TYPE = 378,
     MYTHIC_SUFFIX_TYPE = 379,
     MYTHIC_PREFIX_ID = 380,
     MYTHIC_SUFFIX_ID = 381,
     MATERIAL_ID = 382,
     MATERIAL_TYPE = 383,
     CHARGES_ID = 384,
     SPECIAL_QUALITY_ID = 385,
     SPEFLAGS_ID = 386,
     SUBROOM_ID = 387,
     NAME_ID = 388,
     FLAGS_ID = 389,
     FLAG_TYPE = 390,
     MON_ATTITUDE = 391,
     MON_ALERTNESS = 392,
     SUBTYPE_ID = 393,
     NON_PASSDOOR_ID = 394,
     CARPET_ID = 395,
     CARPET_PIECE_ID = 396,
     CARPET_TYPE = 397,
     MON_APPEARANCE = 398,
     ROOMDOOR_ID = 399,
     IF_ID = 400,
     ELSE_ID = 401,
     TERRAIN_ID = 402,
     HORIZ_OR_VERT = 403,
     REPLACE_TERRAIN_ID = 404,
     LOCATION_SUBTYPE_ID = 405,
     DOOR_SUBTYPE = 406,
     BRAZIER_SUBTYPE = 407,
     SIGNPOST_SUBTYPE = 408,
     TREE_SUBTYPE = 409,
     FOREST_ID = 410,
     FOREST_TYPE = 411,
     INITIALIZE_TYPE = 412,
     EXIT_ID = 413,
     SHUFFLE_ID = 414,
     MANUAL_TYPE_ID = 415,
     MANUAL_TYPE = 416,
     QUANTITY_ID = 417,
     BURIED_ID = 418,
     LOOP_ID = 419,
     FOR_ID = 420,
     TO_ID = 421,
     SWITCH_ID = 422,
     CASE_ID = 423,
     BREAK_ID = 424,
     DEFAULT_ID = 425,
     ERODED_ID = 426,
     TRAPPED_STATE = 427,
     RECHARGED_ID = 428,
     INVIS_ID = 429,
     GREASED_ID = 430,
     INDESTRUCTIBLE_ID = 431,
     FEMALE_ID = 432,
     MALE_ID = 433,
     WAITFORU_ID = 434,
     PROTECTOR_ID = 435,
     CANCELLED_ID = 436,
     REVIVED_ID = 437,
     AVENGE_ID = 438,
     FLEEING_ID = 439,
     BLINDED_ID = 440,
     MAXHP_ID = 441,
     LEVEL_ADJUSTMENT_ID = 442,
     KEEP_ORIGINAL_INVENTORY_ID = 443,
     PARALYZED_ID = 444,
     STUNNED_ID = 445,
     CONFUSED_ID = 446,
     SEENTRAPS_ID = 447,
     ALL_ID = 448,
     MONTYPE_ID = 449,
     OBJTYPE_ID = 450,
     TERTYPE_ID = 451,
     TERTYPE2_ID = 452,
     LEVER_EFFECT_TYPE = 453,
     SWITCHABLE_ID = 454,
     CONTINUOUSLY_USABLE_ID = 455,
     TARGET_ID = 456,
     TRAPTYPE_ID = 457,
     EFFECT_FLAG_ID = 458,
     GRAVE_ID = 459,
     BRAZIER_ID = 460,
     SIGNPOST_ID = 461,
     TREE_ID = 462,
     ERODEPROOF_ID = 463,
     FUNCTION_ID = 464,
     MSG_OUTPUT_TYPE = 465,
     COMPARE_TYPE = 466,
     UNKNOWN_TYPE = 467,
     rect_ID = 468,
     fillrect_ID = 469,
     line_ID = 470,
     randline_ID = 471,
     grow_ID = 472,
     selection_ID = 473,
     flood_ID = 474,
     rndcoord_ID = 475,
     circle_ID = 476,
     ellipse_ID = 477,
     filter_ID = 478,
     complement_ID = 479,
     gradient_ID = 480,
     GRADIENT_TYPE = 481,
     LIMITED = 482,
     HUMIDITY_TYPE = 483,
     STRING = 484,
     MAP_ID = 485,
     NQSTRING = 486,
     VARSTRING = 487,
     CFUNC = 488,
     CFUNC_INT = 489,
     CFUNC_STR = 490,
     CFUNC_COORD = 491,
     CFUNC_REGION = 492,
     VARSTRING_INT = 493,
     VARSTRING_INT_ARRAY = 494,
     VARSTRING_STRING = 495,
     VARSTRING_STRING_ARRAY = 496,
     VARSTRING_VAR = 497,
     VARSTRING_VAR_ARRAY = 498,
     VARSTRING_COORD = 499,
     VARSTRING_COORD_ARRAY = 500,
     VARSTRING_REGION = 501,
     VARSTRING_REGION_ARRAY = 502,
     VARSTRING_MAPCHAR = 503,
     VARSTRING_MAPCHAR_ARRAY = 504,
     VARSTRING_MONST = 505,
     VARSTRING_MONST_ARRAY = 506,
     VARSTRING_OBJ = 507,
     VARSTRING_OBJ_ARRAY = 508,
     VARSTRING_SEL = 509,
     VARSTRING_SEL_ARRAY = 510,
     METHOD_INT = 511,
     METHOD_INT_ARRAY = 512,
     METHOD_STRING = 513,
     METHOD_STRING_ARRAY = 514,
     METHOD_VAR = 515,
     METHOD_VAR_ARRAY = 516,
     METHOD_COORD = 517,
     METHOD_COORD_ARRAY = 518,
     METHOD_REGION = 519,
     METHOD_REGION_ARRAY = 520,
     METHOD_MAPCHAR = 521,
     METHOD_MAPCHAR_ARRAY = 522,
     METHOD_MONST = 523,
     METHOD_MONST_ARRAY = 524,
     METHOD_OBJ = 525,
     METHOD_OBJ_ARRAY = 526,
     METHOD_SEL = 527,
     METHOD_SEL_ARRAY = 528,
     DICE = 529
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
#line 365 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


