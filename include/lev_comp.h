
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
     CORRIDOR_ID = 312,
     GOLD_ID = 313,
     ENGRAVING_ID = 314,
     FOUNTAIN_ID = 315,
     THRONE_ID = 316,
     MODRON_PORTAL_ID = 317,
     LEVEL_TELEPORTER_ID = 318,
     LEVEL_TELEPORT_DIRECTION_TYPE = 319,
     LEVEL_TELEPORT_END_TYPE = 320,
     POOL_ID = 321,
     SINK_ID = 322,
     NONE = 323,
     RAND_CORRIDOR_ID = 324,
     DOOR_STATE = 325,
     LIGHT_STATE = 326,
     CURSE_TYPE = 327,
     ENGRAVING_TYPE = 328,
     KEYTYPE_ID = 329,
     LEVER_ID = 330,
     NO_PICKUP_ID = 331,
     DIRECTION = 332,
     RANDOM_TYPE = 333,
     RANDOM_TYPE_BRACKET = 334,
     A_REGISTER = 335,
     ALIGNMENT = 336,
     LEFT_OR_RIGHT = 337,
     CENTER = 338,
     TOP_OR_BOT = 339,
     ALTAR_TYPE = 340,
     UP_OR_DOWN = 341,
     ACTIVE_OR_INACTIVE = 342,
     MODRON_PORTAL_TYPE = 343,
     NPC_TYPE = 344,
     FOUNTAIN_TYPE = 345,
     SPECIAL_OBJECT_TYPE = 346,
     CMAP_TYPE = 347,
     FLOOR_TYPE = 348,
     FLOOR_TYPE_ID = 349,
     FLOOR_ID = 350,
     FLOOR_MAIN_TYPE = 351,
     FLOOR_MAIN_TYPE_ID = 352,
     ELEMENTAL_ENCHANTMENT_TYPE = 353,
     EXCEPTIONALITY_TYPE = 354,
     EXCEPTIONALITY_ID = 355,
     ELEMENTAL_ENCHANTMENT_ID = 356,
     ENCHANTMENT_ID = 357,
     SECRET_DOOR_ID = 358,
     USES_UP_KEY_ID = 359,
     MYTHIC_PREFIX_TYPE = 360,
     MYTHIC_SUFFIX_TYPE = 361,
     MYTHIC_PREFIX_ID = 362,
     MYTHIC_SUFFIX_ID = 363,
     CHARGES_ID = 364,
     SPECIAL_QUALITY_ID = 365,
     SPEFLAGS_ID = 366,
     SUBROOM_ID = 367,
     NAME_ID = 368,
     FLAGS_ID = 369,
     FLAG_TYPE = 370,
     MON_ATTITUDE = 371,
     MON_ALERTNESS = 372,
     SUBTYPE_ID = 373,
     NON_PASSDOOR_ID = 374,
     MON_APPEARANCE = 375,
     ROOMDOOR_ID = 376,
     IF_ID = 377,
     ELSE_ID = 378,
     TERRAIN_ID = 379,
     HORIZ_OR_VERT = 380,
     REPLACE_TERRAIN_ID = 381,
     LOCATION_SUBTYPE_ID = 382,
     DOOR_SUBTYPE = 383,
     BRAZIER_SUBTYPE = 384,
     SIGNPOST_SUBTYPE = 385,
     EXIT_ID = 386,
     SHUFFLE_ID = 387,
     QUANTITY_ID = 388,
     BURIED_ID = 389,
     LOOP_ID = 390,
     FOR_ID = 391,
     TO_ID = 392,
     SWITCH_ID = 393,
     CASE_ID = 394,
     BREAK_ID = 395,
     DEFAULT_ID = 396,
     ERODED_ID = 397,
     TRAPPED_STATE = 398,
     RECHARGED_ID = 399,
     INVIS_ID = 400,
     GREASED_ID = 401,
     INDESTRUCTIBLE_ID = 402,
     FEMALE_ID = 403,
     WAITFORU_ID = 404,
     CANCELLED_ID = 405,
     REVIVED_ID = 406,
     AVENGE_ID = 407,
     FLEEING_ID = 408,
     BLINDED_ID = 409,
     PARALYZED_ID = 410,
     STUNNED_ID = 411,
     CONFUSED_ID = 412,
     SEENTRAPS_ID = 413,
     ALL_ID = 414,
     MONTYPE_ID = 415,
     OBJTYPE_ID = 416,
     TERTYPE_ID = 417,
     TERTYPE2_ID = 418,
     LEVER_EFFECT_TYPE = 419,
     SWITCHABLE_ID = 420,
     CONTINUOUSLY_USABLE_ID = 421,
     TARGET_ID = 422,
     TRAPTYPE_ID = 423,
     EFFECT_FLAG_ID = 424,
     GRAVE_ID = 425,
     BRAZIER_ID = 426,
     SIGNPOST_ID = 427,
     ERODEPROOF_ID = 428,
     FUNCTION_ID = 429,
     MSG_OUTPUT_TYPE = 430,
     COMPARE_TYPE = 431,
     UNKNOWN_TYPE = 432,
     rect_ID = 433,
     fillrect_ID = 434,
     line_ID = 435,
     randline_ID = 436,
     grow_ID = 437,
     selection_ID = 438,
     flood_ID = 439,
     rndcoord_ID = 440,
     circle_ID = 441,
     ellipse_ID = 442,
     filter_ID = 443,
     complement_ID = 444,
     gradient_ID = 445,
     GRADIENT_TYPE = 446,
     LIMITED = 447,
     HUMIDITY_TYPE = 448,
     STRING = 449,
     MAP_ID = 450,
     NQSTRING = 451,
     VARSTRING = 452,
     CFUNC = 453,
     CFUNC_INT = 454,
     CFUNC_STR = 455,
     CFUNC_COORD = 456,
     CFUNC_REGION = 457,
     VARSTRING_INT = 458,
     VARSTRING_INT_ARRAY = 459,
     VARSTRING_STRING = 460,
     VARSTRING_STRING_ARRAY = 461,
     VARSTRING_VAR = 462,
     VARSTRING_VAR_ARRAY = 463,
     VARSTRING_COORD = 464,
     VARSTRING_COORD_ARRAY = 465,
     VARSTRING_REGION = 466,
     VARSTRING_REGION_ARRAY = 467,
     VARSTRING_MAPCHAR = 468,
     VARSTRING_MAPCHAR_ARRAY = 469,
     VARSTRING_MONST = 470,
     VARSTRING_MONST_ARRAY = 471,
     VARSTRING_OBJ = 472,
     VARSTRING_OBJ_ARRAY = 473,
     VARSTRING_SEL = 474,
     VARSTRING_SEL_ARRAY = 475,
     METHOD_INT = 476,
     METHOD_INT_ARRAY = 477,
     METHOD_STRING = 478,
     METHOD_STRING_ARRAY = 479,
     METHOD_VAR = 480,
     METHOD_VAR_ARRAY = 481,
     METHOD_COORD = 482,
     METHOD_COORD_ARRAY = 483,
     METHOD_REGION = 484,
     METHOD_REGION_ARRAY = 485,
     METHOD_MAPCHAR = 486,
     METHOD_MAPCHAR_ARRAY = 487,
     METHOD_MONST = 488,
     METHOD_MONST_ARRAY = 489,
     METHOD_OBJ = 490,
     METHOD_OBJ_ARRAY = 491,
     METHOD_SEL = 492,
     METHOD_SEL_ARRAY = 493,
     DICE = 494
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
#line 330 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


