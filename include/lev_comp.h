
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
     NPC_TYPE = 329,
     SUBROOM_ID = 330,
     NAME_ID = 331,
     FLAGS_ID = 332,
     FLAG_TYPE = 333,
     MON_ATTITUDE = 334,
     MON_ALERTNESS = 335,
     MON_APPEARANCE = 336,
     ROOMDOOR_ID = 337,
     IF_ID = 338,
     ELSE_ID = 339,
     TERRAIN_ID = 340,
     HORIZ_OR_VERT = 341,
     REPLACE_TERRAIN_ID = 342,
     EXIT_ID = 343,
     SHUFFLE_ID = 344,
     QUANTITY_ID = 345,
     BURIED_ID = 346,
     LOOP_ID = 347,
     FOR_ID = 348,
     TO_ID = 349,
     SWITCH_ID = 350,
     CASE_ID = 351,
     BREAK_ID = 352,
     DEFAULT_ID = 353,
     ERODED_ID = 354,
     TRAPPED_STATE = 355,
     RECHARGED_ID = 356,
     INVIS_ID = 357,
     GREASED_ID = 358,
     FEMALE_ID = 359,
     CANCELLED_ID = 360,
     REVIVED_ID = 361,
     AVENGE_ID = 362,
     FLEEING_ID = 363,
     BLINDED_ID = 364,
     PARALYZED_ID = 365,
     STUNNED_ID = 366,
     CONFUSED_ID = 367,
     SEENTRAPS_ID = 368,
     ALL_ID = 369,
     MONTYPE_ID = 370,
     GRAVE_ID = 371,
     ERODEPROOF_ID = 372,
     FUNCTION_ID = 373,
     MSG_OUTPUT_TYPE = 374,
     COMPARE_TYPE = 375,
     UNKNOWN_TYPE = 376,
     rect_ID = 377,
     fillrect_ID = 378,
     line_ID = 379,
     randline_ID = 380,
     grow_ID = 381,
     selection_ID = 382,
     flood_ID = 383,
     rndcoord_ID = 384,
     circle_ID = 385,
     ellipse_ID = 386,
     filter_ID = 387,
     complement_ID = 388,
     gradient_ID = 389,
     GRADIENT_TYPE = 390,
     LIMITED = 391,
     HUMIDITY_TYPE = 392,
     STRING = 393,
     MAP_ID = 394,
     NQSTRING = 395,
     VARSTRING = 396,
     CFUNC = 397,
     CFUNC_INT = 398,
     CFUNC_STR = 399,
     CFUNC_COORD = 400,
     CFUNC_REGION = 401,
     VARSTRING_INT = 402,
     VARSTRING_INT_ARRAY = 403,
     VARSTRING_STRING = 404,
     VARSTRING_STRING_ARRAY = 405,
     VARSTRING_VAR = 406,
     VARSTRING_VAR_ARRAY = 407,
     VARSTRING_COORD = 408,
     VARSTRING_COORD_ARRAY = 409,
     VARSTRING_REGION = 410,
     VARSTRING_REGION_ARRAY = 411,
     VARSTRING_MAPCHAR = 412,
     VARSTRING_MAPCHAR_ARRAY = 413,
     VARSTRING_MONST = 414,
     VARSTRING_MONST_ARRAY = 415,
     VARSTRING_OBJ = 416,
     VARSTRING_OBJ_ARRAY = 417,
     VARSTRING_SEL = 418,
     VARSTRING_SEL_ARRAY = 419,
     METHOD_INT = 420,
     METHOD_INT_ARRAY = 421,
     METHOD_STRING = 422,
     METHOD_STRING_ARRAY = 423,
     METHOD_VAR = 424,
     METHOD_VAR_ARRAY = 425,
     METHOD_COORD = 426,
     METHOD_COORD_ARRAY = 427,
     METHOD_REGION = 428,
     METHOD_REGION_ARRAY = 429,
     METHOD_MAPCHAR = 430,
     METHOD_MAPCHAR_ARRAY = 431,
     METHOD_MONST = 432,
     METHOD_MONST_ARRAY = 433,
     METHOD_OBJ = 434,
     METHOD_OBJ_ARRAY = 435,
     METHOD_SEL = 436,
     METHOD_SEL_ARRAY = 437,
     DICE = 438
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
#line 274 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


