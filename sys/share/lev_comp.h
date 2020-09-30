
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
     LEVER_ID = 325,
     DIRECTION = 326,
     RANDOM_TYPE = 327,
     RANDOM_TYPE_BRACKET = 328,
     A_REGISTER = 329,
     ALIGNMENT = 330,
     LEFT_OR_RIGHT = 331,
     CENTER = 332,
     TOP_OR_BOT = 333,
     ALTAR_TYPE = 334,
     UP_OR_DOWN = 335,
     ACTIVE_OR_INACTIVE = 336,
     MODRON_PORTAL_TYPE = 337,
     NPC_TYPE = 338,
     FOUNTAIN_TYPE = 339,
     SPECIAL_OBJECT_TYPE = 340,
     CMAP_TYPE = 341,
     FLOOR_TYPE = 342,
     FLOOR_TYPE_ID = 343,
     FLOOR_ID = 344,
     FLOOR_MAIN_TYPE = 345,
     ELEMENTAL_ENCHANTMENT_TYPE = 346,
     EXCEPTIONALITY_TYPE = 347,
     EXCEPTIONALITY_ID = 348,
     ELEMENTAL_ENCHANTMENT_ID = 349,
     ENCHANTMENT_ID = 350,
     CHARGES_ID = 351,
     SPECIAL_QUALITY_ID = 352,
     SPEFLAGS_ID = 353,
     SUBROOM_ID = 354,
     NAME_ID = 355,
     FLAGS_ID = 356,
     FLAG_TYPE = 357,
     MON_ATTITUDE = 358,
     MON_ALERTNESS = 359,
     SUBTYPE_ID = 360,
     MON_APPEARANCE = 361,
     ROOMDOOR_ID = 362,
     IF_ID = 363,
     ELSE_ID = 364,
     TERRAIN_ID = 365,
     HORIZ_OR_VERT = 366,
     REPLACE_TERRAIN_ID = 367,
     EXIT_ID = 368,
     SHUFFLE_ID = 369,
     QUANTITY_ID = 370,
     BURIED_ID = 371,
     LOOP_ID = 372,
     FOR_ID = 373,
     TO_ID = 374,
     SWITCH_ID = 375,
     CASE_ID = 376,
     BREAK_ID = 377,
     DEFAULT_ID = 378,
     ERODED_ID = 379,
     TRAPPED_STATE = 380,
     RECHARGED_ID = 381,
     INVIS_ID = 382,
     GREASED_ID = 383,
     FEMALE_ID = 384,
     WAITFORU_ID = 385,
     CANCELLED_ID = 386,
     REVIVED_ID = 387,
     AVENGE_ID = 388,
     FLEEING_ID = 389,
     BLINDED_ID = 390,
     PARALYZED_ID = 391,
     STUNNED_ID = 392,
     CONFUSED_ID = 393,
     SEENTRAPS_ID = 394,
     ALL_ID = 395,
     MONTYPE_ID = 396,
     OBJTYPE_ID = 397,
     TERTYPE_ID = 398,
     TERTYPE2_ID = 399,
     LEVER_EFFECT_TYPE = 400,
     SWITCHABLE_ID = 401,
     CONTINUOUSLY_USABLE_ID = 402,
     TARGET_ID = 403,
     GRAVE_ID = 404,
     ERODEPROOF_ID = 405,
     FUNCTION_ID = 406,
     MSG_OUTPUT_TYPE = 407,
     COMPARE_TYPE = 408,
     UNKNOWN_TYPE = 409,
     rect_ID = 410,
     fillrect_ID = 411,
     line_ID = 412,
     randline_ID = 413,
     grow_ID = 414,
     selection_ID = 415,
     flood_ID = 416,
     rndcoord_ID = 417,
     circle_ID = 418,
     ellipse_ID = 419,
     filter_ID = 420,
     complement_ID = 421,
     gradient_ID = 422,
     GRADIENT_TYPE = 423,
     LIMITED = 424,
     HUMIDITY_TYPE = 425,
     STRING = 426,
     MAP_ID = 427,
     NQSTRING = 428,
     VARSTRING = 429,
     CFUNC = 430,
     CFUNC_INT = 431,
     CFUNC_STR = 432,
     CFUNC_COORD = 433,
     CFUNC_REGION = 434,
     VARSTRING_INT = 435,
     VARSTRING_INT_ARRAY = 436,
     VARSTRING_STRING = 437,
     VARSTRING_STRING_ARRAY = 438,
     VARSTRING_VAR = 439,
     VARSTRING_VAR_ARRAY = 440,
     VARSTRING_COORD = 441,
     VARSTRING_COORD_ARRAY = 442,
     VARSTRING_REGION = 443,
     VARSTRING_REGION_ARRAY = 444,
     VARSTRING_MAPCHAR = 445,
     VARSTRING_MAPCHAR_ARRAY = 446,
     VARSTRING_MONST = 447,
     VARSTRING_MONST_ARRAY = 448,
     VARSTRING_OBJ = 449,
     VARSTRING_OBJ_ARRAY = 450,
     VARSTRING_SEL = 451,
     VARSTRING_SEL_ARRAY = 452,
     METHOD_INT = 453,
     METHOD_INT_ARRAY = 454,
     METHOD_STRING = 455,
     METHOD_STRING_ARRAY = 456,
     METHOD_VAR = 457,
     METHOD_VAR_ARRAY = 458,
     METHOD_COORD = 459,
     METHOD_COORD_ARRAY = 460,
     METHOD_REGION = 461,
     METHOD_REGION_ARRAY = 462,
     METHOD_MAPCHAR = 463,
     METHOD_MAPCHAR_ARRAY = 464,
     METHOD_MONST = 465,
     METHOD_MONST_ARRAY = 466,
     METHOD_OBJ = 467,
     METHOD_OBJ_ARRAY = 468,
     METHOD_SEL = 469,
     METHOD_SEL_ARRAY = 470,
     DICE = 471
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
#line 307 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


