
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
     DIRECTION = 323,
     RANDOM_TYPE = 324,
     RANDOM_TYPE_BRACKET = 325,
     A_REGISTER = 326,
     ALIGNMENT = 327,
     LEFT_OR_RIGHT = 328,
     CENTER = 329,
     TOP_OR_BOT = 330,
     ALTAR_TYPE = 331,
     UP_OR_DOWN = 332,
     ACTIVE_OR_INACTIVE = 333,
     MODRON_PORTAL_TYPE = 334,
     NPC_TYPE = 335,
     FOUNTAIN_TYPE = 336,
     SPECIAL_OBJECT_TYPE = 337,
     CMAP_TYPE = 338,
     FLOOR_TYPE = 339,
     FLOOR_TYPE_ID = 340,
     FLOOR_ID = 341,
     FLOOR_MAIN_TYPE = 342,
     ELEMENTAL_ENCHANTMENT_TYPE = 343,
     EXCEPTIONALITY_TYPE = 344,
     EXCEPTIONALITY_ID = 345,
     ELEMENTAL_ENCHANTMENT_ID = 346,
     ENCHANTMENT_ID = 347,
     CHARGES_ID = 348,
     SPECIAL_QUALITY_ID = 349,
     SPEFLAGS_ID = 350,
     SUBROOM_ID = 351,
     NAME_ID = 352,
     FLAGS_ID = 353,
     FLAG_TYPE = 354,
     MON_ATTITUDE = 355,
     MON_ALERTNESS = 356,
     SUBTYPE_ID = 357,
     MON_APPEARANCE = 358,
     ROOMDOOR_ID = 359,
     IF_ID = 360,
     ELSE_ID = 361,
     TERRAIN_ID = 362,
     HORIZ_OR_VERT = 363,
     REPLACE_TERRAIN_ID = 364,
     EXIT_ID = 365,
     SHUFFLE_ID = 366,
     QUANTITY_ID = 367,
     BURIED_ID = 368,
     LOOP_ID = 369,
     FOR_ID = 370,
     TO_ID = 371,
     SWITCH_ID = 372,
     CASE_ID = 373,
     BREAK_ID = 374,
     DEFAULT_ID = 375,
     ERODED_ID = 376,
     TRAPPED_STATE = 377,
     RECHARGED_ID = 378,
     INVIS_ID = 379,
     GREASED_ID = 380,
     FEMALE_ID = 381,
     WAITFORU_ID = 382,
     CANCELLED_ID = 383,
     REVIVED_ID = 384,
     AVENGE_ID = 385,
     FLEEING_ID = 386,
     BLINDED_ID = 387,
     PARALYZED_ID = 388,
     STUNNED_ID = 389,
     CONFUSED_ID = 390,
     SEENTRAPS_ID = 391,
     ALL_ID = 392,
     MONTYPE_ID = 393,
     GRAVE_ID = 394,
     ERODEPROOF_ID = 395,
     FUNCTION_ID = 396,
     MSG_OUTPUT_TYPE = 397,
     COMPARE_TYPE = 398,
     UNKNOWN_TYPE = 399,
     rect_ID = 400,
     fillrect_ID = 401,
     line_ID = 402,
     randline_ID = 403,
     grow_ID = 404,
     selection_ID = 405,
     flood_ID = 406,
     rndcoord_ID = 407,
     circle_ID = 408,
     ellipse_ID = 409,
     filter_ID = 410,
     complement_ID = 411,
     gradient_ID = 412,
     GRADIENT_TYPE = 413,
     LIMITED = 414,
     HUMIDITY_TYPE = 415,
     STRING = 416,
     MAP_ID = 417,
     NQSTRING = 418,
     VARSTRING = 419,
     CFUNC = 420,
     CFUNC_INT = 421,
     CFUNC_STR = 422,
     CFUNC_COORD = 423,
     CFUNC_REGION = 424,
     VARSTRING_INT = 425,
     VARSTRING_INT_ARRAY = 426,
     VARSTRING_STRING = 427,
     VARSTRING_STRING_ARRAY = 428,
     VARSTRING_VAR = 429,
     VARSTRING_VAR_ARRAY = 430,
     VARSTRING_COORD = 431,
     VARSTRING_COORD_ARRAY = 432,
     VARSTRING_REGION = 433,
     VARSTRING_REGION_ARRAY = 434,
     VARSTRING_MAPCHAR = 435,
     VARSTRING_MAPCHAR_ARRAY = 436,
     VARSTRING_MONST = 437,
     VARSTRING_MONST_ARRAY = 438,
     VARSTRING_OBJ = 439,
     VARSTRING_OBJ_ARRAY = 440,
     VARSTRING_SEL = 441,
     VARSTRING_SEL_ARRAY = 442,
     METHOD_INT = 443,
     METHOD_INT_ARRAY = 444,
     METHOD_STRING = 445,
     METHOD_STRING_ARRAY = 446,
     METHOD_VAR = 447,
     METHOD_VAR_ARRAY = 448,
     METHOD_COORD = 449,
     METHOD_COORD_ARRAY = 450,
     METHOD_REGION = 451,
     METHOD_REGION_ARRAY = 452,
     METHOD_MAPCHAR = 453,
     METHOD_MAPCHAR_ARRAY = 454,
     METHOD_MONST = 455,
     METHOD_MONST_ARRAY = 456,
     METHOD_OBJ = 457,
     METHOD_OBJ_ARRAY = 458,
     METHOD_SEL = 459,
     METHOD_SEL_ARRAY = 460,
     DICE = 461
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
#line 297 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


