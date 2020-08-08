
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
     MODRON_PORTAL_ID = 307,
     POOL_ID = 308,
     SINK_ID = 309,
     NONE = 310,
     RAND_CORRIDOR_ID = 311,
     DOOR_STATE = 312,
     LIGHT_STATE = 313,
     CURSE_TYPE = 314,
     ENGRAVING_TYPE = 315,
     DIRECTION = 316,
     RANDOM_TYPE = 317,
     RANDOM_TYPE_BRACKET = 318,
     A_REGISTER = 319,
     ALIGNMENT = 320,
     LEFT_OR_RIGHT = 321,
     CENTER = 322,
     TOP_OR_BOT = 323,
     ALTAR_TYPE = 324,
     UP_OR_DOWN = 325,
     ACTIVE_OR_INACTIVE = 326,
     MODRON_PORTAL_TYPE = 327,
     SUBROOM_ID = 328,
     NAME_ID = 329,
     FLAGS_ID = 330,
     FLAG_TYPE = 331,
     MON_ATTITUDE = 332,
     MON_ALERTNESS = 333,
     MON_APPEARANCE = 334,
     ROOMDOOR_ID = 335,
     IF_ID = 336,
     ELSE_ID = 337,
     TERRAIN_ID = 338,
     HORIZ_OR_VERT = 339,
     REPLACE_TERRAIN_ID = 340,
     EXIT_ID = 341,
     SHUFFLE_ID = 342,
     QUANTITY_ID = 343,
     BURIED_ID = 344,
     LOOP_ID = 345,
     FOR_ID = 346,
     TO_ID = 347,
     SWITCH_ID = 348,
     CASE_ID = 349,
     BREAK_ID = 350,
     DEFAULT_ID = 351,
     ERODED_ID = 352,
     TRAPPED_STATE = 353,
     RECHARGED_ID = 354,
     INVIS_ID = 355,
     GREASED_ID = 356,
     FEMALE_ID = 357,
     CANCELLED_ID = 358,
     REVIVED_ID = 359,
     AVENGE_ID = 360,
     FLEEING_ID = 361,
     BLINDED_ID = 362,
     PARALYZED_ID = 363,
     STUNNED_ID = 364,
     CONFUSED_ID = 365,
     SEENTRAPS_ID = 366,
     ALL_ID = 367,
     MONTYPE_ID = 368,
     GRAVE_ID = 369,
     ERODEPROOF_ID = 370,
     FUNCTION_ID = 371,
     MSG_OUTPUT_TYPE = 372,
     COMPARE_TYPE = 373,
     UNKNOWN_TYPE = 374,
     rect_ID = 375,
     fillrect_ID = 376,
     line_ID = 377,
     randline_ID = 378,
     grow_ID = 379,
     selection_ID = 380,
     flood_ID = 381,
     rndcoord_ID = 382,
     circle_ID = 383,
     ellipse_ID = 384,
     filter_ID = 385,
     complement_ID = 386,
     gradient_ID = 387,
     GRADIENT_TYPE = 388,
     LIMITED = 389,
     HUMIDITY_TYPE = 390,
     STRING = 391,
     MAP_ID = 392,
     NQSTRING = 393,
     VARSTRING = 394,
     CFUNC = 395,
     CFUNC_INT = 396,
     CFUNC_STR = 397,
     CFUNC_COORD = 398,
     CFUNC_REGION = 399,
     VARSTRING_INT = 400,
     VARSTRING_INT_ARRAY = 401,
     VARSTRING_STRING = 402,
     VARSTRING_STRING_ARRAY = 403,
     VARSTRING_VAR = 404,
     VARSTRING_VAR_ARRAY = 405,
     VARSTRING_COORD = 406,
     VARSTRING_COORD_ARRAY = 407,
     VARSTRING_REGION = 408,
     VARSTRING_REGION_ARRAY = 409,
     VARSTRING_MAPCHAR = 410,
     VARSTRING_MAPCHAR_ARRAY = 411,
     VARSTRING_MONST = 412,
     VARSTRING_MONST_ARRAY = 413,
     VARSTRING_OBJ = 414,
     VARSTRING_OBJ_ARRAY = 415,
     VARSTRING_SEL = 416,
     VARSTRING_SEL_ARRAY = 417,
     METHOD_INT = 418,
     METHOD_INT_ARRAY = 419,
     METHOD_STRING = 420,
     METHOD_STRING_ARRAY = 421,
     METHOD_VAR = 422,
     METHOD_VAR_ARRAY = 423,
     METHOD_COORD = 424,
     METHOD_COORD_ARRAY = 425,
     METHOD_REGION = 426,
     METHOD_REGION_ARRAY = 427,
     METHOD_MAPCHAR = 428,
     METHOD_MAPCHAR_ARRAY = 429,
     METHOD_MONST = 430,
     METHOD_MONST_ARRAY = 431,
     METHOD_OBJ = 432,
     METHOD_OBJ_ARRAY = 433,
     METHOD_SEL = 434,
     METHOD_SEL_ARRAY = 435,
     DICE = 436
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
#line 272 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


