
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
     NPC_ID = 292,
     LADDER_ID = 293,
     STAIR_ID = 294,
     NON_DIGGABLE_ID = 295,
     NON_PASSWALL_ID = 296,
     ROOM_ID = 297,
     PORTAL_ID = 298,
     TELEPRT_ID = 299,
     BRANCH_ID = 300,
     LEV = 301,
     MINERALIZE_ID = 302,
     CORRIDOR_ID = 303,
     GOLD_ID = 304,
     ENGRAVING_ID = 305,
     FOUNTAIN_ID = 306,
     THRONE_ID = 307,
     MODRON_PORTAL_ID = 308,
     POOL_ID = 309,
     SINK_ID = 310,
     NONE = 311,
     RAND_CORRIDOR_ID = 312,
     DOOR_STATE = 313,
     LIGHT_STATE = 314,
     CURSE_TYPE = 315,
     ENGRAVING_TYPE = 316,
     DIRECTION = 317,
     RANDOM_TYPE = 318,
     RANDOM_TYPE_BRACKET = 319,
     A_REGISTER = 320,
     ALIGNMENT = 321,
     LEFT_OR_RIGHT = 322,
     CENTER = 323,
     TOP_OR_BOT = 324,
     ALTAR_TYPE = 325,
     UP_OR_DOWN = 326,
     ACTIVE_OR_INACTIVE = 327,
     MODRON_PORTAL_TYPE = 328,
     SUBROOM_ID = 329,
     NAME_ID = 330,
     FLAGS_ID = 331,
     FLAG_TYPE = 332,
     MON_ATTITUDE = 333,
     MON_ALERTNESS = 334,
     MON_APPEARANCE = 335,
     ROOMDOOR_ID = 336,
     IF_ID = 337,
     ELSE_ID = 338,
     TERRAIN_ID = 339,
     HORIZ_OR_VERT = 340,
     REPLACE_TERRAIN_ID = 341,
     EXIT_ID = 342,
     SHUFFLE_ID = 343,
     QUANTITY_ID = 344,
     BURIED_ID = 345,
     LOOP_ID = 346,
     FOR_ID = 347,
     TO_ID = 348,
     SWITCH_ID = 349,
     CASE_ID = 350,
     BREAK_ID = 351,
     DEFAULT_ID = 352,
     ERODED_ID = 353,
     TRAPPED_STATE = 354,
     RECHARGED_ID = 355,
     INVIS_ID = 356,
     GREASED_ID = 357,
     FEMALE_ID = 358,
     CANCELLED_ID = 359,
     REVIVED_ID = 360,
     AVENGE_ID = 361,
     FLEEING_ID = 362,
     BLINDED_ID = 363,
     PARALYZED_ID = 364,
     STUNNED_ID = 365,
     CONFUSED_ID = 366,
     SEENTRAPS_ID = 367,
     ALL_ID = 368,
     MONTYPE_ID = 369,
     GRAVE_ID = 370,
     ERODEPROOF_ID = 371,
     FUNCTION_ID = 372,
     MSG_OUTPUT_TYPE = 373,
     COMPARE_TYPE = 374,
     UNKNOWN_TYPE = 375,
     rect_ID = 376,
     fillrect_ID = 377,
     line_ID = 378,
     randline_ID = 379,
     grow_ID = 380,
     selection_ID = 381,
     flood_ID = 382,
     rndcoord_ID = 383,
     circle_ID = 384,
     ellipse_ID = 385,
     filter_ID = 386,
     complement_ID = 387,
     gradient_ID = 388,
     GRADIENT_TYPE = 389,
     LIMITED = 390,
     HUMIDITY_TYPE = 391,
     STRING = 392,
     MAP_ID = 393,
     NQSTRING = 394,
     VARSTRING = 395,
     CFUNC = 396,
     CFUNC_INT = 397,
     CFUNC_STR = 398,
     CFUNC_COORD = 399,
     CFUNC_REGION = 400,
     VARSTRING_INT = 401,
     VARSTRING_INT_ARRAY = 402,
     VARSTRING_STRING = 403,
     VARSTRING_STRING_ARRAY = 404,
     VARSTRING_VAR = 405,
     VARSTRING_VAR_ARRAY = 406,
     VARSTRING_COORD = 407,
     VARSTRING_COORD_ARRAY = 408,
     VARSTRING_REGION = 409,
     VARSTRING_REGION_ARRAY = 410,
     VARSTRING_MAPCHAR = 411,
     VARSTRING_MAPCHAR_ARRAY = 412,
     VARSTRING_MONST = 413,
     VARSTRING_MONST_ARRAY = 414,
     VARSTRING_OBJ = 415,
     VARSTRING_OBJ_ARRAY = 416,
     VARSTRING_SEL = 417,
     VARSTRING_SEL_ARRAY = 418,
     METHOD_INT = 419,
     METHOD_INT_ARRAY = 420,
     METHOD_STRING = 421,
     METHOD_STRING_ARRAY = 422,
     METHOD_VAR = 423,
     METHOD_VAR_ARRAY = 424,
     METHOD_COORD = 425,
     METHOD_COORD_ARRAY = 426,
     METHOD_REGION = 427,
     METHOD_REGION_ARRAY = 428,
     METHOD_MAPCHAR = 429,
     METHOD_MAPCHAR_ARRAY = 430,
     METHOD_MONST = 431,
     METHOD_MONST_ARRAY = 432,
     METHOD_OBJ = 433,
     METHOD_OBJ_ARRAY = 434,
     METHOD_SEL = 435,
     METHOD_SEL_ARRAY = 436,
     DICE = 437
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
#line 273 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


