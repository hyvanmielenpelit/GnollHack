
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
     GEOMETRY_ID = 273,
     NOMAP_ID = 274,
     OBJECT_ID = 275,
     COBJECT_ID = 276,
     MONSTER_ID = 277,
     TRAP_ID = 278,
     DOOR_ID = 279,
     DRAWBRIDGE_ID = 280,
     object_ID = 281,
     monster_ID = 282,
     terrain_ID = 283,
     MAZEWALK_ID = 284,
     WALLIFY_ID = 285,
     REGION_ID = 286,
     FILLING = 287,
     IRREGULAR = 288,
     JOINED = 289,
     ALTAR_ID = 290,
     ANVIL_ID = 291,
     LADDER_ID = 292,
     STAIR_ID = 293,
     NON_DIGGABLE_ID = 294,
     NON_PASSWALL_ID = 295,
     ROOM_ID = 296,
     PORTAL_ID = 297,
     TELEPRT_ID = 298,
     BRANCH_ID = 299,
     LEV = 300,
     MINERALIZE_ID = 301,
     CORRIDOR_ID = 302,
     GOLD_ID = 303,
     ENGRAVING_ID = 304,
     FOUNTAIN_ID = 305,
     THRONE_ID = 306,
     POOL_ID = 307,
     SINK_ID = 308,
     NONE = 309,
     RAND_CORRIDOR_ID = 310,
     DOOR_STATE = 311,
     LIGHT_STATE = 312,
     CURSE_TYPE = 313,
     ENGRAVING_TYPE = 314,
     DIRECTION = 315,
     RANDOM_TYPE = 316,
     RANDOM_TYPE_BRACKET = 317,
     A_REGISTER = 318,
     ALIGNMENT = 319,
     LEFT_OR_RIGHT = 320,
     CENTER = 321,
     TOP_OR_BOT = 322,
     ALTAR_TYPE = 323,
     UP_OR_DOWN = 324,
     SUBROOM_ID = 325,
     NAME_ID = 326,
     FLAGS_ID = 327,
     FLAG_TYPE = 328,
     MON_ATTITUDE = 329,
     MON_ALERTNESS = 330,
     MON_APPEARANCE = 331,
     ROOMDOOR_ID = 332,
     IF_ID = 333,
     ELSE_ID = 334,
     TERRAIN_ID = 335,
     HORIZ_OR_VERT = 336,
     REPLACE_TERRAIN_ID = 337,
     EXIT_ID = 338,
     SHUFFLE_ID = 339,
     QUANTITY_ID = 340,
     BURIED_ID = 341,
     LOOP_ID = 342,
     FOR_ID = 343,
     TO_ID = 344,
     SWITCH_ID = 345,
     CASE_ID = 346,
     BREAK_ID = 347,
     DEFAULT_ID = 348,
     ERODED_ID = 349,
     TRAPPED_STATE = 350,
     RECHARGED_ID = 351,
     INVIS_ID = 352,
     GREASED_ID = 353,
     FEMALE_ID = 354,
     CANCELLED_ID = 355,
     REVIVED_ID = 356,
     AVENGE_ID = 357,
     FLEEING_ID = 358,
     BLINDED_ID = 359,
     PARALYZED_ID = 360,
     STUNNED_ID = 361,
     CONFUSED_ID = 362,
     SEENTRAPS_ID = 363,
     ALL_ID = 364,
     MONTYPE_ID = 365,
     GRAVE_ID = 366,
     ERODEPROOF_ID = 367,
     FUNCTION_ID = 368,
     MSG_OUTPUT_TYPE = 369,
     COMPARE_TYPE = 370,
     UNKNOWN_TYPE = 371,
     rect_ID = 372,
     fillrect_ID = 373,
     line_ID = 374,
     randline_ID = 375,
     grow_ID = 376,
     selection_ID = 377,
     flood_ID = 378,
     rndcoord_ID = 379,
     circle_ID = 380,
     ellipse_ID = 381,
     filter_ID = 382,
     complement_ID = 383,
     gradient_ID = 384,
     GRADIENT_TYPE = 385,
     LIMITED = 386,
     HUMIDITY_TYPE = 387,
     STRING = 388,
     MAP_ID = 389,
     NQSTRING = 390,
     VARSTRING = 391,
     CFUNC = 392,
     CFUNC_INT = 393,
     CFUNC_STR = 394,
     CFUNC_COORD = 395,
     CFUNC_REGION = 396,
     VARSTRING_INT = 397,
     VARSTRING_INT_ARRAY = 398,
     VARSTRING_STRING = 399,
     VARSTRING_STRING_ARRAY = 400,
     VARSTRING_VAR = 401,
     VARSTRING_VAR_ARRAY = 402,
     VARSTRING_COORD = 403,
     VARSTRING_COORD_ARRAY = 404,
     VARSTRING_REGION = 405,
     VARSTRING_REGION_ARRAY = 406,
     VARSTRING_MAPCHAR = 407,
     VARSTRING_MAPCHAR_ARRAY = 408,
     VARSTRING_MONST = 409,
     VARSTRING_MONST_ARRAY = 410,
     VARSTRING_OBJ = 411,
     VARSTRING_OBJ_ARRAY = 412,
     VARSTRING_SEL = 413,
     VARSTRING_SEL_ARRAY = 414,
     METHOD_INT = 415,
     METHOD_INT_ARRAY = 416,
     METHOD_STRING = 417,
     METHOD_STRING_ARRAY = 418,
     METHOD_VAR = 419,
     METHOD_VAR_ARRAY = 420,
     METHOD_COORD = 421,
     METHOD_COORD_ARRAY = 422,
     METHOD_REGION = 423,
     METHOD_REGION_ARRAY = 424,
     METHOD_MAPCHAR = 425,
     METHOD_MAPCHAR_ARRAY = 426,
     METHOD_MONST = 427,
     METHOD_MONST_ARRAY = 428,
     METHOD_OBJ = 429,
     METHOD_OBJ_ARRAY = 430,
     METHOD_SEL = 431,
     METHOD_SEL_ARRAY = 432,
     DICE = 433
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
#line 269 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


