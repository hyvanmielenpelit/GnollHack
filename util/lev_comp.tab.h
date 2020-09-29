
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
     OBJECT_ID = 276,
     COBJECT_ID = 277,
     MONSTER_ID = 278,
     TRAP_ID = 279,
     DOOR_ID = 280,
     DRAWBRIDGE_ID = 281,
     object_ID = 282,
     monster_ID = 283,
     terrain_ID = 284,
     MAZEWALK_ID = 285,
     WALLIFY_ID = 286,
     REGION_ID = 287,
     SPECIAL_REGION_ID = 288,
     SPECIAL_LEVREGION_ID = 289,
     SPECIAL_REGION_TYPE = 290,
     NAMING_ID = 291,
     NAMING_TYPE = 292,
     FILLING = 293,
     IRREGULAR = 294,
     JOINED = 295,
     ALTAR_ID = 296,
     ANVIL_ID = 297,
     NPC_ID = 298,
     LADDER_ID = 299,
     STAIR_ID = 300,
     NON_DIGGABLE_ID = 301,
     NON_PASSWALL_ID = 302,
     ROOM_ID = 303,
     PORTAL_ID = 304,
     TELEPRT_ID = 305,
     BRANCH_ID = 306,
     LEV = 307,
     MINERALIZE_ID = 308,
     CORRIDOR_ID = 309,
     GOLD_ID = 310,
     ENGRAVING_ID = 311,
     FOUNTAIN_ID = 312,
     THRONE_ID = 313,
     MODRON_PORTAL_ID = 314,
     POOL_ID = 315,
     SINK_ID = 316,
     NONE = 317,
     RAND_CORRIDOR_ID = 318,
     DOOR_STATE = 319,
     LIGHT_STATE = 320,
     CURSE_TYPE = 321,
     ENGRAVING_TYPE = 322,
     KEY_TYPE = 323,
     KEYTYPE_ID = 324,
     DIRECTION = 325,
     RANDOM_TYPE = 326,
     RANDOM_TYPE_BRACKET = 327,
     A_REGISTER = 328,
     ALIGNMENT = 329,
     LEFT_OR_RIGHT = 330,
     CENTER = 331,
     TOP_OR_BOT = 332,
     ALTAR_TYPE = 333,
     UP_OR_DOWN = 334,
     ACTIVE_OR_INACTIVE = 335,
     MODRON_PORTAL_TYPE = 336,
     NPC_TYPE = 337,
     FOUNTAIN_TYPE = 338,
     SPECIAL_OBJECT_TYPE = 339,
     CMAP_TYPE = 340,
     FLOOR_TYPE = 341,
     FLOOR_TYPE_ID = 342,
     FLOOR_ID = 343,
     FLOOR_MAIN_TYPE = 344,
     ELEMENTAL_ENCHANTMENT_TYPE = 345,
     EXCEPTIONALITY_TYPE = 346,
     EXCEPTIONALITY_ID = 347,
     ELEMENTAL_ENCHANTMENT_ID = 348,
     ENCHANTMENT_ID = 349,
     CHARGES_ID = 350,
     SPECIAL_QUALITY_ID = 351,
     SPEFLAGS_ID = 352,
     SUBROOM_ID = 353,
     NAME_ID = 354,
     FLAGS_ID = 355,
     FLAG_TYPE = 356,
     MON_ATTITUDE = 357,
     MON_ALERTNESS = 358,
     SUBTYPE_ID = 359,
     MON_APPEARANCE = 360,
     ROOMDOOR_ID = 361,
     IF_ID = 362,
     ELSE_ID = 363,
     TERRAIN_ID = 364,
     HORIZ_OR_VERT = 365,
     REPLACE_TERRAIN_ID = 366,
     EXIT_ID = 367,
     SHUFFLE_ID = 368,
     QUANTITY_ID = 369,
     BURIED_ID = 370,
     LOOP_ID = 371,
     FOR_ID = 372,
     TO_ID = 373,
     SWITCH_ID = 374,
     CASE_ID = 375,
     BREAK_ID = 376,
     DEFAULT_ID = 377,
     ERODED_ID = 378,
     TRAPPED_STATE = 379,
     RECHARGED_ID = 380,
     INVIS_ID = 381,
     GREASED_ID = 382,
     FEMALE_ID = 383,
     WAITFORU_ID = 384,
     CANCELLED_ID = 385,
     REVIVED_ID = 386,
     AVENGE_ID = 387,
     FLEEING_ID = 388,
     BLINDED_ID = 389,
     PARALYZED_ID = 390,
     STUNNED_ID = 391,
     CONFUSED_ID = 392,
     SEENTRAPS_ID = 393,
     ALL_ID = 394,
     MONTYPE_ID = 395,
     GRAVE_ID = 396,
     ERODEPROOF_ID = 397,
     FUNCTION_ID = 398,
     MSG_OUTPUT_TYPE = 399,
     COMPARE_TYPE = 400,
     UNKNOWN_TYPE = 401,
     rect_ID = 402,
     fillrect_ID = 403,
     line_ID = 404,
     randline_ID = 405,
     grow_ID = 406,
     selection_ID = 407,
     flood_ID = 408,
     rndcoord_ID = 409,
     circle_ID = 410,
     ellipse_ID = 411,
     filter_ID = 412,
     complement_ID = 413,
     gradient_ID = 414,
     GRADIENT_TYPE = 415,
     LIMITED = 416,
     HUMIDITY_TYPE = 417,
     STRING = 418,
     MAP_ID = 419,
     NQSTRING = 420,
     VARSTRING = 421,
     CFUNC = 422,
     CFUNC_INT = 423,
     CFUNC_STR = 424,
     CFUNC_COORD = 425,
     CFUNC_REGION = 426,
     VARSTRING_INT = 427,
     VARSTRING_INT_ARRAY = 428,
     VARSTRING_STRING = 429,
     VARSTRING_STRING_ARRAY = 430,
     VARSTRING_VAR = 431,
     VARSTRING_VAR_ARRAY = 432,
     VARSTRING_COORD = 433,
     VARSTRING_COORD_ARRAY = 434,
     VARSTRING_REGION = 435,
     VARSTRING_REGION_ARRAY = 436,
     VARSTRING_MAPCHAR = 437,
     VARSTRING_MAPCHAR_ARRAY = 438,
     VARSTRING_MONST = 439,
     VARSTRING_MONST_ARRAY = 440,
     VARSTRING_OBJ = 441,
     VARSTRING_OBJ_ARRAY = 442,
     VARSTRING_SEL = 443,
     VARSTRING_SEL_ARRAY = 444,
     METHOD_INT = 445,
     METHOD_INT_ARRAY = 446,
     METHOD_STRING = 447,
     METHOD_STRING_ARRAY = 448,
     METHOD_VAR = 449,
     METHOD_VAR_ARRAY = 450,
     METHOD_COORD = 451,
     METHOD_COORD_ARRAY = 452,
     METHOD_REGION = 453,
     METHOD_REGION_ARRAY = 454,
     METHOD_MAPCHAR = 455,
     METHOD_MAPCHAR_ARRAY = 456,
     METHOD_MONST = 457,
     METHOD_MONST_ARRAY = 458,
     METHOD_OBJ = 459,
     METHOD_OBJ_ARRAY = 460,
     METHOD_SEL = 461,
     METHOD_SEL_ARRAY = 462,
     DICE = 463
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
#line 299 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


