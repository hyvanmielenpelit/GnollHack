
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
     FILLING = 288,
     IRREGULAR = 289,
     JOINED = 290,
     ALTAR_ID = 291,
     ANVIL_ID = 292,
     NPC_ID = 293,
     LADDER_ID = 294,
     STAIR_ID = 295,
     NON_DIGGABLE_ID = 296,
     NON_PASSWALL_ID = 297,
     ROOM_ID = 298,
     PORTAL_ID = 299,
     TELEPRT_ID = 300,
     BRANCH_ID = 301,
     LEV = 302,
     MINERALIZE_ID = 303,
     CORRIDOR_ID = 304,
     GOLD_ID = 305,
     ENGRAVING_ID = 306,
     FOUNTAIN_ID = 307,
     THRONE_ID = 308,
     MODRON_PORTAL_ID = 309,
     POOL_ID = 310,
     SINK_ID = 311,
     NONE = 312,
     RAND_CORRIDOR_ID = 313,
     DOOR_STATE = 314,
     LIGHT_STATE = 315,
     CURSE_TYPE = 316,
     ENGRAVING_TYPE = 317,
     DIRECTION = 318,
     RANDOM_TYPE = 319,
     RANDOM_TYPE_BRACKET = 320,
     A_REGISTER = 321,
     ALIGNMENT = 322,
     LEFT_OR_RIGHT = 323,
     CENTER = 324,
     TOP_OR_BOT = 325,
     ALTAR_TYPE = 326,
     UP_OR_DOWN = 327,
     ACTIVE_OR_INACTIVE = 328,
     MODRON_PORTAL_TYPE = 329,
     NPC_TYPE = 330,
     FOUNTAIN_TYPE = 331,
     SPECIAL_OBJECT_TYPE = 332,
     CMAP_TYPE = 333,
     FLOOR_TYPE = 334,
     FLOOR_TYPE_ID = 335,
     FLOOR_ID = 336,
     FLOOR_MAIN_TYPE = 337,
     ELEMENTAL_ENCHANTMENT_TYPE = 338,
     EXCEPTIONALITY_TYPE = 339,
     EXCEPTIONALITY_ID = 340,
     ELEMENTAL_ENCHANTMENT_ID = 341,
     ENCHANTMENT_ID = 342,
     CHARGES_ID = 343,
     SPECIAL_QUALITY_ID = 344,
     SPEFLAGS_ID = 345,
     SUBROOM_ID = 346,
     NAME_ID = 347,
     FLAGS_ID = 348,
     FLAG_TYPE = 349,
     MON_ATTITUDE = 350,
     MON_ALERTNESS = 351,
     SUBTYPE_ID = 352,
     MON_APPEARANCE = 353,
     ROOMDOOR_ID = 354,
     IF_ID = 355,
     ELSE_ID = 356,
     TERRAIN_ID = 357,
     HORIZ_OR_VERT = 358,
     REPLACE_TERRAIN_ID = 359,
     EXIT_ID = 360,
     SHUFFLE_ID = 361,
     QUANTITY_ID = 362,
     BURIED_ID = 363,
     LOOP_ID = 364,
     FOR_ID = 365,
     TO_ID = 366,
     SWITCH_ID = 367,
     CASE_ID = 368,
     BREAK_ID = 369,
     DEFAULT_ID = 370,
     ERODED_ID = 371,
     TRAPPED_STATE = 372,
     RECHARGED_ID = 373,
     INVIS_ID = 374,
     GREASED_ID = 375,
     FEMALE_ID = 376,
     WAITFORU_ID = 377,
     CANCELLED_ID = 378,
     REVIVED_ID = 379,
     AVENGE_ID = 380,
     FLEEING_ID = 381,
     BLINDED_ID = 382,
     PARALYZED_ID = 383,
     STUNNED_ID = 384,
     CONFUSED_ID = 385,
     SEENTRAPS_ID = 386,
     ALL_ID = 387,
     MONTYPE_ID = 388,
     GRAVE_ID = 389,
     ERODEPROOF_ID = 390,
     FUNCTION_ID = 391,
     MSG_OUTPUT_TYPE = 392,
     COMPARE_TYPE = 393,
     UNKNOWN_TYPE = 394,
     rect_ID = 395,
     fillrect_ID = 396,
     line_ID = 397,
     randline_ID = 398,
     grow_ID = 399,
     selection_ID = 400,
     flood_ID = 401,
     rndcoord_ID = 402,
     circle_ID = 403,
     ellipse_ID = 404,
     filter_ID = 405,
     complement_ID = 406,
     gradient_ID = 407,
     GRADIENT_TYPE = 408,
     LIMITED = 409,
     HUMIDITY_TYPE = 410,
     STRING = 411,
     MAP_ID = 412,
     NQSTRING = 413,
     VARSTRING = 414,
     CFUNC = 415,
     CFUNC_INT = 416,
     CFUNC_STR = 417,
     CFUNC_COORD = 418,
     CFUNC_REGION = 419,
     VARSTRING_INT = 420,
     VARSTRING_INT_ARRAY = 421,
     VARSTRING_STRING = 422,
     VARSTRING_STRING_ARRAY = 423,
     VARSTRING_VAR = 424,
     VARSTRING_VAR_ARRAY = 425,
     VARSTRING_COORD = 426,
     VARSTRING_COORD_ARRAY = 427,
     VARSTRING_REGION = 428,
     VARSTRING_REGION_ARRAY = 429,
     VARSTRING_MAPCHAR = 430,
     VARSTRING_MAPCHAR_ARRAY = 431,
     VARSTRING_MONST = 432,
     VARSTRING_MONST_ARRAY = 433,
     VARSTRING_OBJ = 434,
     VARSTRING_OBJ_ARRAY = 435,
     VARSTRING_SEL = 436,
     VARSTRING_SEL_ARRAY = 437,
     METHOD_INT = 438,
     METHOD_INT_ARRAY = 439,
     METHOD_STRING = 440,
     METHOD_STRING_ARRAY = 441,
     METHOD_VAR = 442,
     METHOD_VAR_ARRAY = 443,
     METHOD_COORD = 444,
     METHOD_COORD_ARRAY = 445,
     METHOD_REGION = 446,
     METHOD_REGION_ARRAY = 447,
     METHOD_MAPCHAR = 448,
     METHOD_MAPCHAR_ARRAY = 449,
     METHOD_MONST = 450,
     METHOD_MONST_ARRAY = 451,
     METHOD_OBJ = 452,
     METHOD_OBJ_ARRAY = 453,
     METHOD_SEL = 454,
     METHOD_SEL_ARRAY = 455,
     DICE = 456
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
#line 292 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


