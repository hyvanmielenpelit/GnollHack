
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
     LADDER_ID = 291,
     STAIR_ID = 292,
     NON_DIGGABLE_ID = 293,
     NON_PASSWALL_ID = 294,
     ROOM_ID = 295,
     PORTAL_ID = 296,
     TELEPRT_ID = 297,
     BRANCH_ID = 298,
     LEV = 299,
     MINERALIZE_ID = 300,
     CORRIDOR_ID = 301,
     GOLD_ID = 302,
     ENGRAVING_ID = 303,
     FOUNTAIN_ID = 304,
     POOL_ID = 305,
     SINK_ID = 306,
     NONE = 307,
     RAND_CORRIDOR_ID = 308,
     DOOR_STATE = 309,
     LIGHT_STATE = 310,
     CURSE_TYPE = 311,
     ENGRAVING_TYPE = 312,
     DIRECTION = 313,
     RANDOM_TYPE = 314,
     RANDOM_TYPE_BRACKET = 315,
     A_REGISTER = 316,
     ALIGNMENT = 317,
     LEFT_OR_RIGHT = 318,
     CENTER = 319,
     TOP_OR_BOT = 320,
     ALTAR_TYPE = 321,
     UP_OR_DOWN = 322,
     SUBROOM_ID = 323,
     NAME_ID = 324,
     FLAGS_ID = 325,
     FLAG_TYPE = 326,
     MON_ATTITUDE = 327,
     MON_ALERTNESS = 328,
     MON_APPEARANCE = 329,
     ROOMDOOR_ID = 330,
     IF_ID = 331,
     ELSE_ID = 332,
     TERRAIN_ID = 333,
     HORIZ_OR_VERT = 334,
     REPLACE_TERRAIN_ID = 335,
     EXIT_ID = 336,
     SHUFFLE_ID = 337,
     QUANTITY_ID = 338,
     BURIED_ID = 339,
     LOOP_ID = 340,
     FOR_ID = 341,
     TO_ID = 342,
     SWITCH_ID = 343,
     CASE_ID = 344,
     BREAK_ID = 345,
     DEFAULT_ID = 346,
     ERODED_ID = 347,
     TRAPPED_STATE = 348,
     RECHARGED_ID = 349,
     INVIS_ID = 350,
     GREASED_ID = 351,
     FEMALE_ID = 352,
     CANCELLED_ID = 353,
     REVIVED_ID = 354,
     AVENGE_ID = 355,
     FLEEING_ID = 356,
     BLINDED_ID = 357,
     PARALYZED_ID = 358,
     STUNNED_ID = 359,
     CONFUSED_ID = 360,
     SEENTRAPS_ID = 361,
     ALL_ID = 362,
     MONTYPE_ID = 363,
     GRAVE_ID = 364,
     ERODEPROOF_ID = 365,
     FUNCTION_ID = 366,
     MSG_OUTPUT_TYPE = 367,
     COMPARE_TYPE = 368,
     UNKNOWN_TYPE = 369,
     rect_ID = 370,
     fillrect_ID = 371,
     line_ID = 372,
     randline_ID = 373,
     grow_ID = 374,
     selection_ID = 375,
     flood_ID = 376,
     rndcoord_ID = 377,
     circle_ID = 378,
     ellipse_ID = 379,
     filter_ID = 380,
     complement_ID = 381,
     gradient_ID = 382,
     GRADIENT_TYPE = 383,
     LIMITED = 384,
     HUMIDITY_TYPE = 385,
     STRING = 386,
     MAP_ID = 387,
     NQSTRING = 388,
     VARSTRING = 389,
     CFUNC = 390,
     CFUNC_INT = 391,
     CFUNC_STR = 392,
     CFUNC_COORD = 393,
     CFUNC_REGION = 394,
     VARSTRING_INT = 395,
     VARSTRING_INT_ARRAY = 396,
     VARSTRING_STRING = 397,
     VARSTRING_STRING_ARRAY = 398,
     VARSTRING_VAR = 399,
     VARSTRING_VAR_ARRAY = 400,
     VARSTRING_COORD = 401,
     VARSTRING_COORD_ARRAY = 402,
     VARSTRING_REGION = 403,
     VARSTRING_REGION_ARRAY = 404,
     VARSTRING_MAPCHAR = 405,
     VARSTRING_MAPCHAR_ARRAY = 406,
     VARSTRING_MONST = 407,
     VARSTRING_MONST_ARRAY = 408,
     VARSTRING_OBJ = 409,
     VARSTRING_OBJ_ARRAY = 410,
     VARSTRING_SEL = 411,
     VARSTRING_SEL_ARRAY = 412,
     METHOD_INT = 413,
     METHOD_INT_ARRAY = 414,
     METHOD_STRING = 415,
     METHOD_STRING_ARRAY = 416,
     METHOD_VAR = 417,
     METHOD_VAR_ARRAY = 418,
     METHOD_COORD = 419,
     METHOD_COORD_ARRAY = 420,
     METHOD_REGION = 421,
     METHOD_REGION_ARRAY = 422,
     METHOD_MAPCHAR = 423,
     METHOD_MAPCHAR_ARRAY = 424,
     METHOD_MONST = 425,
     METHOD_MONST_ARRAY = 426,
     METHOD_OBJ = 427,
     METHOD_OBJ_ARRAY = 428,
     METHOD_SEL = 429,
     METHOD_SEL_ARRAY = 430,
     DICE = 431
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
#line 267 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


