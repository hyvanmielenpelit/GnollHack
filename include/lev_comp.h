
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
     object_ID = 284,
     monster_ID = 285,
     terrain_ID = 286,
     MAZEWALK_ID = 287,
     WALLIFY_ID = 288,
     REGION_ID = 289,
     SPECIAL_REGION_ID = 290,
     SPECIAL_LEVREGION_ID = 291,
     SPECIAL_REGION_TYPE = 292,
     NAMING_ID = 293,
     NAMING_TYPE = 294,
     FILLING = 295,
     IRREGULAR = 296,
     JOINED = 297,
     ALTAR_ID = 298,
     ANVIL_ID = 299,
     NPC_ID = 300,
     LADDER_ID = 301,
     STAIR_ID = 302,
     NON_DIGGABLE_ID = 303,
     NON_PASSWALL_ID = 304,
     ROOM_ID = 305,
     ARTIFACT_NAME_ID = 306,
     PORTAL_ID = 307,
     TELEPRT_ID = 308,
     BRANCH_ID = 309,
     LEV = 310,
     MINERALIZE_ID = 311,
     AGE_ID = 312,
     CORRIDOR_ID = 313,
     GOLD_ID = 314,
     ENGRAVING_ID = 315,
     FOUNTAIN_ID = 316,
     THRONE_ID = 317,
     MODRON_PORTAL_ID = 318,
     LEVEL_TELEPORTER_ID = 319,
     LEVEL_TELEPORT_DIRECTION_TYPE = 320,
     LEVEL_TELEPORT_END_TYPE = 321,
     POOL_ID = 322,
     SINK_ID = 323,
     NONE = 324,
     RAND_CORRIDOR_ID = 325,
     DOOR_STATE = 326,
     LIGHT_STATE = 327,
     CURSE_TYPE = 328,
     ENGRAVING_TYPE = 329,
     KEYTYPE_ID = 330,
     LEVER_ID = 331,
     NO_PICKUP_ID = 332,
     DIRECTION = 333,
     RANDOM_TYPE = 334,
     RANDOM_TYPE_BRACKET = 335,
     A_REGISTER = 336,
     ALIGNMENT = 337,
     LEFT_OR_RIGHT = 338,
     CENTER = 339,
     TOP_OR_BOT = 340,
     ALTAR_TYPE = 341,
     UP_OR_DOWN = 342,
     ACTIVE_OR_INACTIVE = 343,
     MODRON_PORTAL_TYPE = 344,
     NPC_TYPE = 345,
     FOUNTAIN_TYPE = 346,
     SPECIAL_OBJECT_TYPE = 347,
     CMAP_TYPE = 348,
     FLOOR_TYPE = 349,
     FLOOR_TYPE_ID = 350,
     FLOOR_ID = 351,
     FLOOR_MAIN_TYPE = 352,
     FLOOR_MAIN_TYPE_ID = 353,
     ELEMENTAL_ENCHANTMENT_TYPE = 354,
     EXCEPTIONALITY_TYPE = 355,
     EXCEPTIONALITY_ID = 356,
     ELEMENTAL_ENCHANTMENT_ID = 357,
     ENCHANTMENT_ID = 358,
     SECRET_DOOR_ID = 359,
     USES_UP_KEY_ID = 360,
     MYTHIC_PREFIX_TYPE = 361,
     MYTHIC_SUFFIX_TYPE = 362,
     MYTHIC_PREFIX_ID = 363,
     MYTHIC_SUFFIX_ID = 364,
     CHARGES_ID = 365,
     SPECIAL_QUALITY_ID = 366,
     SPEFLAGS_ID = 367,
     SUBROOM_ID = 368,
     NAME_ID = 369,
     FLAGS_ID = 370,
     FLAG_TYPE = 371,
     MON_ATTITUDE = 372,
     MON_ALERTNESS = 373,
     SUBTYPE_ID = 374,
     NON_PASSDOOR_ID = 375,
     MON_APPEARANCE = 376,
     ROOMDOOR_ID = 377,
     IF_ID = 378,
     ELSE_ID = 379,
     TERRAIN_ID = 380,
     HORIZ_OR_VERT = 381,
     REPLACE_TERRAIN_ID = 382,
     LOCATION_SUBTYPE_ID = 383,
     DOOR_SUBTYPE = 384,
     BRAZIER_SUBTYPE = 385,
     SIGNPOST_SUBTYPE = 386,
     TREE_SUBTYPE = 387,
     EXIT_ID = 388,
     SHUFFLE_ID = 389,
     QUANTITY_ID = 390,
     BURIED_ID = 391,
     LOOP_ID = 392,
     FOR_ID = 393,
     TO_ID = 394,
     SWITCH_ID = 395,
     CASE_ID = 396,
     BREAK_ID = 397,
     DEFAULT_ID = 398,
     ERODED_ID = 399,
     TRAPPED_STATE = 400,
     RECHARGED_ID = 401,
     INVIS_ID = 402,
     GREASED_ID = 403,
     INDESTRUCTIBLE_ID = 404,
     FEMALE_ID = 405,
     WAITFORU_ID = 406,
     CANCELLED_ID = 407,
     REVIVED_ID = 408,
     AVENGE_ID = 409,
     FLEEING_ID = 410,
     BLINDED_ID = 411,
     PARALYZED_ID = 412,
     STUNNED_ID = 413,
     CONFUSED_ID = 414,
     SEENTRAPS_ID = 415,
     ALL_ID = 416,
     MONTYPE_ID = 417,
     OBJTYPE_ID = 418,
     TERTYPE_ID = 419,
     TERTYPE2_ID = 420,
     LEVER_EFFECT_TYPE = 421,
     SWITCHABLE_ID = 422,
     CONTINUOUSLY_USABLE_ID = 423,
     TARGET_ID = 424,
     TRAPTYPE_ID = 425,
     EFFECT_FLAG_ID = 426,
     GRAVE_ID = 427,
     BRAZIER_ID = 428,
     SIGNPOST_ID = 429,
     TREE_ID = 430,
     ERODEPROOF_ID = 431,
     FUNCTION_ID = 432,
     MSG_OUTPUT_TYPE = 433,
     COMPARE_TYPE = 434,
     UNKNOWN_TYPE = 435,
     rect_ID = 436,
     fillrect_ID = 437,
     line_ID = 438,
     randline_ID = 439,
     grow_ID = 440,
     selection_ID = 441,
     flood_ID = 442,
     rndcoord_ID = 443,
     circle_ID = 444,
     ellipse_ID = 445,
     filter_ID = 446,
     complement_ID = 447,
     gradient_ID = 448,
     GRADIENT_TYPE = 449,
     LIMITED = 450,
     HUMIDITY_TYPE = 451,
     STRING = 452,
     MAP_ID = 453,
     NQSTRING = 454,
     VARSTRING = 455,
     CFUNC = 456,
     CFUNC_INT = 457,
     CFUNC_STR = 458,
     CFUNC_COORD = 459,
     CFUNC_REGION = 460,
     VARSTRING_INT = 461,
     VARSTRING_INT_ARRAY = 462,
     VARSTRING_STRING = 463,
     VARSTRING_STRING_ARRAY = 464,
     VARSTRING_VAR = 465,
     VARSTRING_VAR_ARRAY = 466,
     VARSTRING_COORD = 467,
     VARSTRING_COORD_ARRAY = 468,
     VARSTRING_REGION = 469,
     VARSTRING_REGION_ARRAY = 470,
     VARSTRING_MAPCHAR = 471,
     VARSTRING_MAPCHAR_ARRAY = 472,
     VARSTRING_MONST = 473,
     VARSTRING_MONST_ARRAY = 474,
     VARSTRING_OBJ = 475,
     VARSTRING_OBJ_ARRAY = 476,
     VARSTRING_SEL = 477,
     VARSTRING_SEL_ARRAY = 478,
     METHOD_INT = 479,
     METHOD_INT_ARRAY = 480,
     METHOD_STRING = 481,
     METHOD_STRING_ARRAY = 482,
     METHOD_VAR = 483,
     METHOD_VAR_ARRAY = 484,
     METHOD_COORD = 485,
     METHOD_COORD_ARRAY = 486,
     METHOD_REGION = 487,
     METHOD_REGION_ARRAY = 488,
     METHOD_MAPCHAR = 489,
     METHOD_MAPCHAR_ARRAY = 490,
     METHOD_MONST = 491,
     METHOD_MONST_ARRAY = 492,
     METHOD_OBJ = 493,
     METHOD_OBJ_ARRAY = 494,
     METHOD_SEL = 495,
     METHOD_SEL_ARRAY = 496,
     DICE = 497
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
#line 333 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


