
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
     ENGRAVING_TYPE = 330,
     KEYTYPE_ID = 331,
     LEVER_ID = 332,
     NO_PICKUP_ID = 333,
     DIRECTION = 334,
     RANDOM_TYPE = 335,
     RANDOM_TYPE_BRACKET = 336,
     A_REGISTER = 337,
     ALIGNMENT = 338,
     LEFT_OR_RIGHT = 339,
     CENTER = 340,
     TOP_OR_BOT = 341,
     ALTAR_TYPE = 342,
     ALTAR_SUBTYPE = 343,
     UP_OR_DOWN = 344,
     ACTIVE_OR_INACTIVE = 345,
     MODRON_PORTAL_TYPE = 346,
     NPC_TYPE = 347,
     FOUNTAIN_TYPE = 348,
     SPECIAL_OBJECT_TYPE = 349,
     CMAP_TYPE = 350,
     FLOOR_SUBTYPE = 351,
     FLOOR_SUBTYPE_ID = 352,
     FLOOR_ID = 353,
     FLOOR_TYPE = 354,
     FLOOR_TYPE_ID = 355,
     ELEMENTAL_ENCHANTMENT_TYPE = 356,
     EXCEPTIONALITY_TYPE = 357,
     EXCEPTIONALITY_ID = 358,
     ELEMENTAL_ENCHANTMENT_ID = 359,
     ENCHANTMENT_ID = 360,
     SECRET_DOOR_ID = 361,
     USES_UP_KEY_ID = 362,
     MYTHIC_PREFIX_TYPE = 363,
     MYTHIC_SUFFIX_TYPE = 364,
     MYTHIC_PREFIX_ID = 365,
     MYTHIC_SUFFIX_ID = 366,
     CHARGES_ID = 367,
     SPECIAL_QUALITY_ID = 368,
     SPEFLAGS_ID = 369,
     SUBROOM_ID = 370,
     NAME_ID = 371,
     FLAGS_ID = 372,
     FLAG_TYPE = 373,
     MON_ATTITUDE = 374,
     MON_ALERTNESS = 375,
     SUBTYPE_ID = 376,
     NON_PASSDOOR_ID = 377,
     MON_APPEARANCE = 378,
     ROOMDOOR_ID = 379,
     IF_ID = 380,
     ELSE_ID = 381,
     TERRAIN_ID = 382,
     HORIZ_OR_VERT = 383,
     REPLACE_TERRAIN_ID = 384,
     LOCATION_SUBTYPE_ID = 385,
     DOOR_SUBTYPE = 386,
     BRAZIER_SUBTYPE = 387,
     SIGNPOST_SUBTYPE = 388,
     TREE_SUBTYPE = 389,
     FOREST_ID = 390,
     FOREST_TYPE = 391,
     INITIALIZE_TYPE = 392,
     EXIT_ID = 393,
     SHUFFLE_ID = 394,
     QUANTITY_ID = 395,
     BURIED_ID = 396,
     LOOP_ID = 397,
     FOR_ID = 398,
     TO_ID = 399,
     SWITCH_ID = 400,
     CASE_ID = 401,
     BREAK_ID = 402,
     DEFAULT_ID = 403,
     ERODED_ID = 404,
     TRAPPED_STATE = 405,
     RECHARGED_ID = 406,
     INVIS_ID = 407,
     GREASED_ID = 408,
     INDESTRUCTIBLE_ID = 409,
     FEMALE_ID = 410,
     MALE_ID = 411,
     WAITFORU_ID = 412,
     PROTECTOR_ID = 413,
     CANCELLED_ID = 414,
     REVIVED_ID = 415,
     AVENGE_ID = 416,
     FLEEING_ID = 417,
     BLINDED_ID = 418,
     PARALYZED_ID = 419,
     STUNNED_ID = 420,
     CONFUSED_ID = 421,
     SEENTRAPS_ID = 422,
     ALL_ID = 423,
     MONTYPE_ID = 424,
     OBJTYPE_ID = 425,
     TERTYPE_ID = 426,
     TERTYPE2_ID = 427,
     LEVER_EFFECT_TYPE = 428,
     SWITCHABLE_ID = 429,
     CONTINUOUSLY_USABLE_ID = 430,
     TARGET_ID = 431,
     TRAPTYPE_ID = 432,
     EFFECT_FLAG_ID = 433,
     GRAVE_ID = 434,
     BRAZIER_ID = 435,
     SIGNPOST_ID = 436,
     TREE_ID = 437,
     ERODEPROOF_ID = 438,
     FUNCTION_ID = 439,
     MSG_OUTPUT_TYPE = 440,
     COMPARE_TYPE = 441,
     UNKNOWN_TYPE = 442,
     rect_ID = 443,
     fillrect_ID = 444,
     line_ID = 445,
     randline_ID = 446,
     grow_ID = 447,
     selection_ID = 448,
     flood_ID = 449,
     rndcoord_ID = 450,
     circle_ID = 451,
     ellipse_ID = 452,
     filter_ID = 453,
     complement_ID = 454,
     gradient_ID = 455,
     GRADIENT_TYPE = 456,
     LIMITED = 457,
     HUMIDITY_TYPE = 458,
     STRING = 459,
     MAP_ID = 460,
     NQSTRING = 461,
     VARSTRING = 462,
     CFUNC = 463,
     CFUNC_INT = 464,
     CFUNC_STR = 465,
     CFUNC_COORD = 466,
     CFUNC_REGION = 467,
     VARSTRING_INT = 468,
     VARSTRING_INT_ARRAY = 469,
     VARSTRING_STRING = 470,
     VARSTRING_STRING_ARRAY = 471,
     VARSTRING_VAR = 472,
     VARSTRING_VAR_ARRAY = 473,
     VARSTRING_COORD = 474,
     VARSTRING_COORD_ARRAY = 475,
     VARSTRING_REGION = 476,
     VARSTRING_REGION_ARRAY = 477,
     VARSTRING_MAPCHAR = 478,
     VARSTRING_MAPCHAR_ARRAY = 479,
     VARSTRING_MONST = 480,
     VARSTRING_MONST_ARRAY = 481,
     VARSTRING_OBJ = 482,
     VARSTRING_OBJ_ARRAY = 483,
     VARSTRING_SEL = 484,
     VARSTRING_SEL_ARRAY = 485,
     METHOD_INT = 486,
     METHOD_INT_ARRAY = 487,
     METHOD_STRING = 488,
     METHOD_STRING_ARRAY = 489,
     METHOD_VAR = 490,
     METHOD_VAR_ARRAY = 491,
     METHOD_COORD = 492,
     METHOD_COORD_ARRAY = 493,
     METHOD_REGION = 494,
     METHOD_REGION_ARRAY = 495,
     METHOD_MAPCHAR = 496,
     METHOD_MAPCHAR_ARRAY = 497,
     METHOD_MONST = 498,
     METHOD_MONST_ARRAY = 499,
     METHOD_OBJ = 500,
     METHOD_OBJ_ARRAY = 501,
     METHOD_SEL = 502,
     METHOD_SEL_ARRAY = 503,
     DICE = 504
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 151 "lev_comp.y"

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
#line 340 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


