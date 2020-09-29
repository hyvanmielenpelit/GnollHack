
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
     DIRECTION = 324,
     RANDOM_TYPE = 325,
     RANDOM_TYPE_BRACKET = 326,
     A_REGISTER = 327,
     ALIGNMENT = 328,
     LEFT_OR_RIGHT = 329,
     CENTER = 330,
     TOP_OR_BOT = 331,
     ALTAR_TYPE = 332,
     UP_OR_DOWN = 333,
     ACTIVE_OR_INACTIVE = 334,
     MODRON_PORTAL_TYPE = 335,
     NPC_TYPE = 336,
     FOUNTAIN_TYPE = 337,
     SPECIAL_OBJECT_TYPE = 338,
     CMAP_TYPE = 339,
     FLOOR_TYPE = 340,
     FLOOR_TYPE_ID = 341,
     FLOOR_ID = 342,
     FLOOR_MAIN_TYPE = 343,
     ELEMENTAL_ENCHANTMENT_TYPE = 344,
     EXCEPTIONALITY_TYPE = 345,
     EXCEPTIONALITY_ID = 346,
     ELEMENTAL_ENCHANTMENT_ID = 347,
     ENCHANTMENT_ID = 348,
     CHARGES_ID = 349,
     SPECIAL_QUALITY_ID = 350,
     SPEFLAGS_ID = 351,
     SUBROOM_ID = 352,
     NAME_ID = 353,
     FLAGS_ID = 354,
     FLAG_TYPE = 355,
     MON_ATTITUDE = 356,
     MON_ALERTNESS = 357,
     SUBTYPE_ID = 358,
     MON_APPEARANCE = 359,
     ROOMDOOR_ID = 360,
     IF_ID = 361,
     ELSE_ID = 362,
     TERRAIN_ID = 363,
     HORIZ_OR_VERT = 364,
     REPLACE_TERRAIN_ID = 365,
     EXIT_ID = 366,
     SHUFFLE_ID = 367,
     QUANTITY_ID = 368,
     BURIED_ID = 369,
     LOOP_ID = 370,
     FOR_ID = 371,
     TO_ID = 372,
     SWITCH_ID = 373,
     CASE_ID = 374,
     BREAK_ID = 375,
     DEFAULT_ID = 376,
     ERODED_ID = 377,
     TRAPPED_STATE = 378,
     RECHARGED_ID = 379,
     INVIS_ID = 380,
     GREASED_ID = 381,
     FEMALE_ID = 382,
     WAITFORU_ID = 383,
     CANCELLED_ID = 384,
     REVIVED_ID = 385,
     AVENGE_ID = 386,
     FLEEING_ID = 387,
     BLINDED_ID = 388,
     PARALYZED_ID = 389,
     STUNNED_ID = 390,
     CONFUSED_ID = 391,
     SEENTRAPS_ID = 392,
     ALL_ID = 393,
     MONTYPE_ID = 394,
     GRAVE_ID = 395,
     ERODEPROOF_ID = 396,
     FUNCTION_ID = 397,
     MSG_OUTPUT_TYPE = 398,
     COMPARE_TYPE = 399,
     UNKNOWN_TYPE = 400,
     rect_ID = 401,
     fillrect_ID = 402,
     line_ID = 403,
     randline_ID = 404,
     grow_ID = 405,
     selection_ID = 406,
     flood_ID = 407,
     rndcoord_ID = 408,
     circle_ID = 409,
     ellipse_ID = 410,
     filter_ID = 411,
     complement_ID = 412,
     gradient_ID = 413,
     GRADIENT_TYPE = 414,
     LIMITED = 415,
     HUMIDITY_TYPE = 416,
     STRING = 417,
     MAP_ID = 418,
     NQSTRING = 419,
     VARSTRING = 420,
     CFUNC = 421,
     CFUNC_INT = 422,
     CFUNC_STR = 423,
     CFUNC_COORD = 424,
     CFUNC_REGION = 425,
     VARSTRING_INT = 426,
     VARSTRING_INT_ARRAY = 427,
     VARSTRING_STRING = 428,
     VARSTRING_STRING_ARRAY = 429,
     VARSTRING_VAR = 430,
     VARSTRING_VAR_ARRAY = 431,
     VARSTRING_COORD = 432,
     VARSTRING_COORD_ARRAY = 433,
     VARSTRING_REGION = 434,
     VARSTRING_REGION_ARRAY = 435,
     VARSTRING_MAPCHAR = 436,
     VARSTRING_MAPCHAR_ARRAY = 437,
     VARSTRING_MONST = 438,
     VARSTRING_MONST_ARRAY = 439,
     VARSTRING_OBJ = 440,
     VARSTRING_OBJ_ARRAY = 441,
     VARSTRING_SEL = 442,
     VARSTRING_SEL_ARRAY = 443,
     METHOD_INT = 444,
     METHOD_INT_ARRAY = 445,
     METHOD_STRING = 446,
     METHOD_STRING_ARRAY = 447,
     METHOD_VAR = 448,
     METHOD_VAR_ARRAY = 449,
     METHOD_COORD = 450,
     METHOD_COORD_ARRAY = 451,
     METHOD_REGION = 452,
     METHOD_REGION_ARRAY = 453,
     METHOD_MAPCHAR = 454,
     METHOD_MAPCHAR_ARRAY = 455,
     METHOD_MONST = 456,
     METHOD_MONST_ARRAY = 457,
     METHOD_OBJ = 458,
     METHOD_OBJ_ARRAY = 459,
     METHOD_SEL = 460,
     METHOD_SEL_ARRAY = 461,
     DICE = 462
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
#line 298 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


