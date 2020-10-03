
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
     KEYTYPE_ID = 323,
     LEVER_ID = 324,
     DIRECTION = 325,
     RANDOM_TYPE = 326,
     RANDOM_TYPE_BRACKET = 327,
     A_REGISTER = 328,
     ALIGNMENT = 329,
     LEFT_OR_RIGHT = 330,
     CENTER = 331,
     TOP_OR_BOT = 332,
     ALTAR_TYPE = 333,
     UP_OR_DOWN = 334,
     ACTIVE_OR_INACTIVE = 335,
     MODRON_PORTAL_TYPE = 336,
     NPC_TYPE = 337,
     FOUNTAIN_TYPE = 338,
     SPECIAL_OBJECT_TYPE = 339,
     CMAP_TYPE = 340,
     FLOOR_TYPE = 341,
     FLOOR_TYPE_ID = 342,
     FLOOR_ID = 343,
     FLOOR_MAIN_TYPE = 344,
     FLOOR_MAIN_TYPE_ID = 345,
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
     LOCATION_SUBTYPE_ID = 368,
     DOOR_SUBTYPE = 369,
     EXIT_ID = 370,
     SHUFFLE_ID = 371,
     QUANTITY_ID = 372,
     BURIED_ID = 373,
     LOOP_ID = 374,
     FOR_ID = 375,
     TO_ID = 376,
     SWITCH_ID = 377,
     CASE_ID = 378,
     BREAK_ID = 379,
     DEFAULT_ID = 380,
     ERODED_ID = 381,
     TRAPPED_STATE = 382,
     RECHARGED_ID = 383,
     INVIS_ID = 384,
     GREASED_ID = 385,
     INDESTRUCTIBLE_ID = 386,
     FEMALE_ID = 387,
     WAITFORU_ID = 388,
     CANCELLED_ID = 389,
     REVIVED_ID = 390,
     AVENGE_ID = 391,
     FLEEING_ID = 392,
     BLINDED_ID = 393,
     PARALYZED_ID = 394,
     STUNNED_ID = 395,
     CONFUSED_ID = 396,
     SEENTRAPS_ID = 397,
     ALL_ID = 398,
     MONTYPE_ID = 399,
     OBJTYPE_ID = 400,
     TERTYPE_ID = 401,
     TERTYPE2_ID = 402,
     LEVER_EFFECT_TYPE = 403,
     SWITCHABLE_ID = 404,
     CONTINUOUSLY_USABLE_ID = 405,
     TARGET_ID = 406,
     TRAPTYPE_ID = 407,
     EFFECT_FLAG_ID = 408,
     GRAVE_ID = 409,
     ERODEPROOF_ID = 410,
     FUNCTION_ID = 411,
     MSG_OUTPUT_TYPE = 412,
     COMPARE_TYPE = 413,
     UNKNOWN_TYPE = 414,
     rect_ID = 415,
     fillrect_ID = 416,
     line_ID = 417,
     randline_ID = 418,
     grow_ID = 419,
     selection_ID = 420,
     flood_ID = 421,
     rndcoord_ID = 422,
     circle_ID = 423,
     ellipse_ID = 424,
     filter_ID = 425,
     complement_ID = 426,
     gradient_ID = 427,
     GRADIENT_TYPE = 428,
     LIMITED = 429,
     HUMIDITY_TYPE = 430,
     STRING = 431,
     MAP_ID = 432,
     NQSTRING = 433,
     VARSTRING = 434,
     CFUNC = 435,
     CFUNC_INT = 436,
     CFUNC_STR = 437,
     CFUNC_COORD = 438,
     CFUNC_REGION = 439,
     VARSTRING_INT = 440,
     VARSTRING_INT_ARRAY = 441,
     VARSTRING_STRING = 442,
     VARSTRING_STRING_ARRAY = 443,
     VARSTRING_VAR = 444,
     VARSTRING_VAR_ARRAY = 445,
     VARSTRING_COORD = 446,
     VARSTRING_COORD_ARRAY = 447,
     VARSTRING_REGION = 448,
     VARSTRING_REGION_ARRAY = 449,
     VARSTRING_MAPCHAR = 450,
     VARSTRING_MAPCHAR_ARRAY = 451,
     VARSTRING_MONST = 452,
     VARSTRING_MONST_ARRAY = 453,
     VARSTRING_OBJ = 454,
     VARSTRING_OBJ_ARRAY = 455,
     VARSTRING_SEL = 456,
     VARSTRING_SEL_ARRAY = 457,
     METHOD_INT = 458,
     METHOD_INT_ARRAY = 459,
     METHOD_STRING = 460,
     METHOD_STRING_ARRAY = 461,
     METHOD_VAR = 462,
     METHOD_VAR_ARRAY = 463,
     METHOD_COORD = 464,
     METHOD_COORD_ARRAY = 465,
     METHOD_REGION = 466,
     METHOD_REGION_ARRAY = 467,
     METHOD_MAPCHAR = 468,
     METHOD_MAPCHAR_ARRAY = 469,
     METHOD_MONST = 470,
     METHOD_MONST_ARRAY = 471,
     METHOD_OBJ = 472,
     METHOD_OBJ_ARRAY = 473,
     METHOD_SEL = 474,
     METHOD_SEL_ARRAY = 475,
     DICE = 476
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
#line 312 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


