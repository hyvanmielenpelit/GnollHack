
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
     EXIT_ID = 387,
     SHUFFLE_ID = 388,
     QUANTITY_ID = 389,
     BURIED_ID = 390,
     LOOP_ID = 391,
     FOR_ID = 392,
     TO_ID = 393,
     SWITCH_ID = 394,
     CASE_ID = 395,
     BREAK_ID = 396,
     DEFAULT_ID = 397,
     ERODED_ID = 398,
     TRAPPED_STATE = 399,
     RECHARGED_ID = 400,
     INVIS_ID = 401,
     GREASED_ID = 402,
     INDESTRUCTIBLE_ID = 403,
     FEMALE_ID = 404,
     WAITFORU_ID = 405,
     CANCELLED_ID = 406,
     REVIVED_ID = 407,
     AVENGE_ID = 408,
     FLEEING_ID = 409,
     BLINDED_ID = 410,
     PARALYZED_ID = 411,
     STUNNED_ID = 412,
     CONFUSED_ID = 413,
     SEENTRAPS_ID = 414,
     ALL_ID = 415,
     MONTYPE_ID = 416,
     OBJTYPE_ID = 417,
     TERTYPE_ID = 418,
     TERTYPE2_ID = 419,
     LEVER_EFFECT_TYPE = 420,
     SWITCHABLE_ID = 421,
     CONTINUOUSLY_USABLE_ID = 422,
     TARGET_ID = 423,
     TRAPTYPE_ID = 424,
     EFFECT_FLAG_ID = 425,
     GRAVE_ID = 426,
     BRAZIER_ID = 427,
     SIGNPOST_ID = 428,
     ERODEPROOF_ID = 429,
     FUNCTION_ID = 430,
     MSG_OUTPUT_TYPE = 431,
     COMPARE_TYPE = 432,
     UNKNOWN_TYPE = 433,
     rect_ID = 434,
     fillrect_ID = 435,
     line_ID = 436,
     randline_ID = 437,
     grow_ID = 438,
     selection_ID = 439,
     flood_ID = 440,
     rndcoord_ID = 441,
     circle_ID = 442,
     ellipse_ID = 443,
     filter_ID = 444,
     complement_ID = 445,
     gradient_ID = 446,
     GRADIENT_TYPE = 447,
     LIMITED = 448,
     HUMIDITY_TYPE = 449,
     STRING = 450,
     MAP_ID = 451,
     NQSTRING = 452,
     VARSTRING = 453,
     CFUNC = 454,
     CFUNC_INT = 455,
     CFUNC_STR = 456,
     CFUNC_COORD = 457,
     CFUNC_REGION = 458,
     VARSTRING_INT = 459,
     VARSTRING_INT_ARRAY = 460,
     VARSTRING_STRING = 461,
     VARSTRING_STRING_ARRAY = 462,
     VARSTRING_VAR = 463,
     VARSTRING_VAR_ARRAY = 464,
     VARSTRING_COORD = 465,
     VARSTRING_COORD_ARRAY = 466,
     VARSTRING_REGION = 467,
     VARSTRING_REGION_ARRAY = 468,
     VARSTRING_MAPCHAR = 469,
     VARSTRING_MAPCHAR_ARRAY = 470,
     VARSTRING_MONST = 471,
     VARSTRING_MONST_ARRAY = 472,
     VARSTRING_OBJ = 473,
     VARSTRING_OBJ_ARRAY = 474,
     VARSTRING_SEL = 475,
     VARSTRING_SEL_ARRAY = 476,
     METHOD_INT = 477,
     METHOD_INT_ARRAY = 478,
     METHOD_STRING = 479,
     METHOD_STRING_ARRAY = 480,
     METHOD_VAR = 481,
     METHOD_VAR_ARRAY = 482,
     METHOD_COORD = 483,
     METHOD_COORD_ARRAY = 484,
     METHOD_REGION = 485,
     METHOD_REGION_ARRAY = 486,
     METHOD_MAPCHAR = 487,
     METHOD_MAPCHAR_ARRAY = 488,
     METHOD_MONST = 489,
     METHOD_MONST_ARRAY = 490,
     METHOD_OBJ = 491,
     METHOD_OBJ_ARRAY = 492,
     METHOD_SEL = 493,
     METHOD_SEL_ARRAY = 494,
     DICE = 495
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
#line 331 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


