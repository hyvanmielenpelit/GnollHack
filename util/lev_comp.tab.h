
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
     OBJECT_ID = 277,
     COBJECT_ID = 278,
     MONSTER_ID = 279,
     TRAP_ID = 280,
     DOOR_ID = 281,
     DRAWBRIDGE_ID = 282,
     object_ID = 283,
     monster_ID = 284,
     terrain_ID = 285,
     MAZEWALK_ID = 286,
     WALLIFY_ID = 287,
     REGION_ID = 288,
     SPECIAL_REGION_ID = 289,
     SPECIAL_LEVREGION_ID = 290,
     SPECIAL_REGION_TYPE = 291,
     NAMING_ID = 292,
     NAMING_TYPE = 293,
     FILLING = 294,
     IRREGULAR = 295,
     JOINED = 296,
     ALTAR_ID = 297,
     ANVIL_ID = 298,
     NPC_ID = 299,
     LADDER_ID = 300,
     STAIR_ID = 301,
     NON_DIGGABLE_ID = 302,
     NON_PASSWALL_ID = 303,
     ROOM_ID = 304,
     ARTIFACT_NAME_ID = 305,
     PORTAL_ID = 306,
     TELEPRT_ID = 307,
     BRANCH_ID = 308,
     LEV = 309,
     MINERALIZE_ID = 310,
     CORRIDOR_ID = 311,
     GOLD_ID = 312,
     ENGRAVING_ID = 313,
     FOUNTAIN_ID = 314,
     THRONE_ID = 315,
     MODRON_PORTAL_ID = 316,
     LEVEL_TELEPORTER_ID = 317,
     LEVEL_TELEPORT_DIRECTION_TYPE = 318,
     LEVEL_TELEPORT_END_TYPE = 319,
     POOL_ID = 320,
     SINK_ID = 321,
     NONE = 322,
     RAND_CORRIDOR_ID = 323,
     DOOR_STATE = 324,
     LIGHT_STATE = 325,
     CURSE_TYPE = 326,
     ENGRAVING_TYPE = 327,
     KEYTYPE_ID = 328,
     LEVER_ID = 329,
     NO_PICKUP_ID = 330,
     DIRECTION = 331,
     RANDOM_TYPE = 332,
     RANDOM_TYPE_BRACKET = 333,
     A_REGISTER = 334,
     ALIGNMENT = 335,
     LEFT_OR_RIGHT = 336,
     CENTER = 337,
     TOP_OR_BOT = 338,
     ALTAR_TYPE = 339,
     UP_OR_DOWN = 340,
     ACTIVE_OR_INACTIVE = 341,
     MODRON_PORTAL_TYPE = 342,
     NPC_TYPE = 343,
     FOUNTAIN_TYPE = 344,
     SPECIAL_OBJECT_TYPE = 345,
     CMAP_TYPE = 346,
     FLOOR_TYPE = 347,
     FLOOR_TYPE_ID = 348,
     FLOOR_ID = 349,
     FLOOR_MAIN_TYPE = 350,
     FLOOR_MAIN_TYPE_ID = 351,
     ELEMENTAL_ENCHANTMENT_TYPE = 352,
     EXCEPTIONALITY_TYPE = 353,
     EXCEPTIONALITY_ID = 354,
     ELEMENTAL_ENCHANTMENT_ID = 355,
     ENCHANTMENT_ID = 356,
     SECRET_DOOR_ID = 357,
     USES_UP_KEY_ID = 358,
     MYTHIC_PREFIX_TYPE = 359,
     MYTHIC_SUFFIX_TYPE = 360,
     MYTHIC_PREFIX_ID = 361,
     MYTHIC_SUFFIX_ID = 362,
     CHARGES_ID = 363,
     SPECIAL_QUALITY_ID = 364,
     SPEFLAGS_ID = 365,
     SUBROOM_ID = 366,
     NAME_ID = 367,
     FLAGS_ID = 368,
     FLAG_TYPE = 369,
     MON_ATTITUDE = 370,
     MON_ALERTNESS = 371,
     SUBTYPE_ID = 372,
     NON_PASSDOOR_ID = 373,
     MON_APPEARANCE = 374,
     ROOMDOOR_ID = 375,
     IF_ID = 376,
     ELSE_ID = 377,
     TERRAIN_ID = 378,
     HORIZ_OR_VERT = 379,
     REPLACE_TERRAIN_ID = 380,
     LOCATION_SUBTYPE_ID = 381,
     DOOR_SUBTYPE = 382,
     BRAZIER_SUBTYPE = 383,
     SIGNPOST_SUBTYPE = 384,
     EXIT_ID = 385,
     SHUFFLE_ID = 386,
     QUANTITY_ID = 387,
     BURIED_ID = 388,
     LOOP_ID = 389,
     FOR_ID = 390,
     TO_ID = 391,
     SWITCH_ID = 392,
     CASE_ID = 393,
     BREAK_ID = 394,
     DEFAULT_ID = 395,
     ERODED_ID = 396,
     TRAPPED_STATE = 397,
     RECHARGED_ID = 398,
     INVIS_ID = 399,
     GREASED_ID = 400,
     INDESTRUCTIBLE_ID = 401,
     FEMALE_ID = 402,
     WAITFORU_ID = 403,
     CANCELLED_ID = 404,
     REVIVED_ID = 405,
     AVENGE_ID = 406,
     FLEEING_ID = 407,
     BLINDED_ID = 408,
     PARALYZED_ID = 409,
     STUNNED_ID = 410,
     CONFUSED_ID = 411,
     SEENTRAPS_ID = 412,
     ALL_ID = 413,
     MONTYPE_ID = 414,
     OBJTYPE_ID = 415,
     TERTYPE_ID = 416,
     TERTYPE2_ID = 417,
     LEVER_EFFECT_TYPE = 418,
     SWITCHABLE_ID = 419,
     CONTINUOUSLY_USABLE_ID = 420,
     TARGET_ID = 421,
     TRAPTYPE_ID = 422,
     EFFECT_FLAG_ID = 423,
     GRAVE_ID = 424,
     BRAZIER_ID = 425,
     SIGNPOST_ID = 426,
     ERODEPROOF_ID = 427,
     FUNCTION_ID = 428,
     MSG_OUTPUT_TYPE = 429,
     COMPARE_TYPE = 430,
     UNKNOWN_TYPE = 431,
     rect_ID = 432,
     fillrect_ID = 433,
     line_ID = 434,
     randline_ID = 435,
     grow_ID = 436,
     selection_ID = 437,
     flood_ID = 438,
     rndcoord_ID = 439,
     circle_ID = 440,
     ellipse_ID = 441,
     filter_ID = 442,
     complement_ID = 443,
     gradient_ID = 444,
     GRADIENT_TYPE = 445,
     LIMITED = 446,
     HUMIDITY_TYPE = 447,
     STRING = 448,
     MAP_ID = 449,
     NQSTRING = 450,
     VARSTRING = 451,
     CFUNC = 452,
     CFUNC_INT = 453,
     CFUNC_STR = 454,
     CFUNC_COORD = 455,
     CFUNC_REGION = 456,
     VARSTRING_INT = 457,
     VARSTRING_INT_ARRAY = 458,
     VARSTRING_STRING = 459,
     VARSTRING_STRING_ARRAY = 460,
     VARSTRING_VAR = 461,
     VARSTRING_VAR_ARRAY = 462,
     VARSTRING_COORD = 463,
     VARSTRING_COORD_ARRAY = 464,
     VARSTRING_REGION = 465,
     VARSTRING_REGION_ARRAY = 466,
     VARSTRING_MAPCHAR = 467,
     VARSTRING_MAPCHAR_ARRAY = 468,
     VARSTRING_MONST = 469,
     VARSTRING_MONST_ARRAY = 470,
     VARSTRING_OBJ = 471,
     VARSTRING_OBJ_ARRAY = 472,
     VARSTRING_SEL = 473,
     VARSTRING_SEL_ARRAY = 474,
     METHOD_INT = 475,
     METHOD_INT_ARRAY = 476,
     METHOD_STRING = 477,
     METHOD_STRING_ARRAY = 478,
     METHOD_VAR = 479,
     METHOD_VAR_ARRAY = 480,
     METHOD_COORD = 481,
     METHOD_COORD_ARRAY = 482,
     METHOD_REGION = 483,
     METHOD_REGION_ARRAY = 484,
     METHOD_MAPCHAR = 485,
     METHOD_MAPCHAR_ARRAY = 486,
     METHOD_MONST = 487,
     METHOD_MONST_ARRAY = 488,
     METHOD_OBJ = 489,
     METHOD_OBJ_ARRAY = 490,
     METHOD_SEL = 491,
     METHOD_SEL_ARRAY = 492,
     DICE = 493
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
#line 329 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


