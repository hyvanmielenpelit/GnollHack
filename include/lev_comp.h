
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
     SPECIAL_REGION_TYPE = 289,
     NAMING_ID = 290,
     NAMING_TYPE = 291,
     FILLING = 292,
     IRREGULAR = 293,
     JOINED = 294,
     ALTAR_ID = 295,
     ANVIL_ID = 296,
     NPC_ID = 297,
     LADDER_ID = 298,
     STAIR_ID = 299,
     NON_DIGGABLE_ID = 300,
     NON_PASSWALL_ID = 301,
     ROOM_ID = 302,
     PORTAL_ID = 303,
     TELEPRT_ID = 304,
     BRANCH_ID = 305,
     LEV = 306,
     MINERALIZE_ID = 307,
     CORRIDOR_ID = 308,
     GOLD_ID = 309,
     ENGRAVING_ID = 310,
     FOUNTAIN_ID = 311,
     THRONE_ID = 312,
     MODRON_PORTAL_ID = 313,
     POOL_ID = 314,
     SINK_ID = 315,
     NONE = 316,
     RAND_CORRIDOR_ID = 317,
     DOOR_STATE = 318,
     LIGHT_STATE = 319,
     CURSE_TYPE = 320,
     ENGRAVING_TYPE = 321,
     DIRECTION = 322,
     RANDOM_TYPE = 323,
     RANDOM_TYPE_BRACKET = 324,
     A_REGISTER = 325,
     ALIGNMENT = 326,
     LEFT_OR_RIGHT = 327,
     CENTER = 328,
     TOP_OR_BOT = 329,
     ALTAR_TYPE = 330,
     UP_OR_DOWN = 331,
     ACTIVE_OR_INACTIVE = 332,
     MODRON_PORTAL_TYPE = 333,
     NPC_TYPE = 334,
     FOUNTAIN_TYPE = 335,
     SPECIAL_OBJECT_TYPE = 336,
     CMAP_TYPE = 337,
     FLOOR_TYPE = 338,
     FLOOR_TYPE_ID = 339,
     FLOOR_ID = 340,
     FLOOR_MAIN_TYPE = 341,
     ELEMENTAL_ENCHANTMENT_TYPE = 342,
     EXCEPTIONALITY_TYPE = 343,
     EXCEPTIONALITY_ID = 344,
     ELEMENTAL_ENCHANTMENT_ID = 345,
     ENCHANTMENT_ID = 346,
     CHARGES_ID = 347,
     SPECIAL_QUALITY_ID = 348,
     SPEFLAGS_ID = 349,
     SUBROOM_ID = 350,
     NAME_ID = 351,
     FLAGS_ID = 352,
     FLAG_TYPE = 353,
     MON_ATTITUDE = 354,
     MON_ALERTNESS = 355,
     SUBTYPE_ID = 356,
     MON_APPEARANCE = 357,
     ROOMDOOR_ID = 358,
     IF_ID = 359,
     ELSE_ID = 360,
     TERRAIN_ID = 361,
     HORIZ_OR_VERT = 362,
     REPLACE_TERRAIN_ID = 363,
     EXIT_ID = 364,
     SHUFFLE_ID = 365,
     QUANTITY_ID = 366,
     BURIED_ID = 367,
     LOOP_ID = 368,
     FOR_ID = 369,
     TO_ID = 370,
     SWITCH_ID = 371,
     CASE_ID = 372,
     BREAK_ID = 373,
     DEFAULT_ID = 374,
     ERODED_ID = 375,
     TRAPPED_STATE = 376,
     RECHARGED_ID = 377,
     INVIS_ID = 378,
     GREASED_ID = 379,
     FEMALE_ID = 380,
     WAITFORU_ID = 381,
     CANCELLED_ID = 382,
     REVIVED_ID = 383,
     AVENGE_ID = 384,
     FLEEING_ID = 385,
     BLINDED_ID = 386,
     PARALYZED_ID = 387,
     STUNNED_ID = 388,
     CONFUSED_ID = 389,
     SEENTRAPS_ID = 390,
     ALL_ID = 391,
     MONTYPE_ID = 392,
     GRAVE_ID = 393,
     ERODEPROOF_ID = 394,
     FUNCTION_ID = 395,
     MSG_OUTPUT_TYPE = 396,
     COMPARE_TYPE = 397,
     UNKNOWN_TYPE = 398,
     rect_ID = 399,
     fillrect_ID = 400,
     line_ID = 401,
     randline_ID = 402,
     grow_ID = 403,
     selection_ID = 404,
     flood_ID = 405,
     rndcoord_ID = 406,
     circle_ID = 407,
     ellipse_ID = 408,
     filter_ID = 409,
     complement_ID = 410,
     gradient_ID = 411,
     GRADIENT_TYPE = 412,
     LIMITED = 413,
     HUMIDITY_TYPE = 414,
     STRING = 415,
     MAP_ID = 416,
     NQSTRING = 417,
     VARSTRING = 418,
     CFUNC = 419,
     CFUNC_INT = 420,
     CFUNC_STR = 421,
     CFUNC_COORD = 422,
     CFUNC_REGION = 423,
     VARSTRING_INT = 424,
     VARSTRING_INT_ARRAY = 425,
     VARSTRING_STRING = 426,
     VARSTRING_STRING_ARRAY = 427,
     VARSTRING_VAR = 428,
     VARSTRING_VAR_ARRAY = 429,
     VARSTRING_COORD = 430,
     VARSTRING_COORD_ARRAY = 431,
     VARSTRING_REGION = 432,
     VARSTRING_REGION_ARRAY = 433,
     VARSTRING_MAPCHAR = 434,
     VARSTRING_MAPCHAR_ARRAY = 435,
     VARSTRING_MONST = 436,
     VARSTRING_MONST_ARRAY = 437,
     VARSTRING_OBJ = 438,
     VARSTRING_OBJ_ARRAY = 439,
     VARSTRING_SEL = 440,
     VARSTRING_SEL_ARRAY = 441,
     METHOD_INT = 442,
     METHOD_INT_ARRAY = 443,
     METHOD_STRING = 444,
     METHOD_STRING_ARRAY = 445,
     METHOD_VAR = 446,
     METHOD_VAR_ARRAY = 447,
     METHOD_COORD = 448,
     METHOD_COORD_ARRAY = 449,
     METHOD_REGION = 450,
     METHOD_REGION_ARRAY = 451,
     METHOD_MAPCHAR = 452,
     METHOD_MAPCHAR_ARRAY = 453,
     METHOD_MONST = 454,
     METHOD_MONST_ARRAY = 455,
     METHOD_OBJ = 456,
     METHOD_OBJ_ARRAY = 457,
     METHOD_SEL = 458,
     METHOD_SEL_ARRAY = 459,
     DICE = 460
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
#line 296 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


