
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
     BOUNDARY_TYPE_ID = 276,
     OBJECT_ID = 277,
     COBJECT_ID = 278,
     MONSTER_ID = 279,
     TRAP_ID = 280,
     DOOR_ID = 281,
     DRAWBRIDGE_ID = 282,
     object_ID = 283,
     monster_ID = 284,
     terrain_ID = 285,
     MAZEWALK_ID = 286,
     WALLIFY_ID = 287,
     REGION_ID = 288,
     SPECIAL_REGION_ID = 289,
     SPECIAL_LEVREGION_ID = 290,
     SPECIAL_REGION_TYPE = 291,
     NAMING_ID = 292,
     NAMING_TYPE = 293,
     FILLING = 294,
     IRREGULAR = 295,
     JOINED = 296,
     ALTAR_ID = 297,
     ANVIL_ID = 298,
     NPC_ID = 299,
     LADDER_ID = 300,
     STAIR_ID = 301,
     NON_DIGGABLE_ID = 302,
     NON_PASSWALL_ID = 303,
     ROOM_ID = 304,
     PORTAL_ID = 305,
     TELEPRT_ID = 306,
     BRANCH_ID = 307,
     LEV = 308,
     MINERALIZE_ID = 309,
     CORRIDOR_ID = 310,
     GOLD_ID = 311,
     ENGRAVING_ID = 312,
     FOUNTAIN_ID = 313,
     THRONE_ID = 314,
     MODRON_PORTAL_ID = 315,
     POOL_ID = 316,
     SINK_ID = 317,
     NONE = 318,
     RAND_CORRIDOR_ID = 319,
     DOOR_STATE = 320,
     LIGHT_STATE = 321,
     CURSE_TYPE = 322,
     ENGRAVING_TYPE = 323,
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
     FLOOR_MAIN_TYPE_ID = 346,
     ELEMENTAL_ENCHANTMENT_TYPE = 347,
     EXCEPTIONALITY_TYPE = 348,
     EXCEPTIONALITY_ID = 349,
     ELEMENTAL_ENCHANTMENT_ID = 350,
     ENCHANTMENT_ID = 351,
     CHARGES_ID = 352,
     SPECIAL_QUALITY_ID = 353,
     SPEFLAGS_ID = 354,
     SUBROOM_ID = 355,
     NAME_ID = 356,
     FLAGS_ID = 357,
     FLAG_TYPE = 358,
     MON_ATTITUDE = 359,
     MON_ALERTNESS = 360,
     SUBTYPE_ID = 361,
     MON_APPEARANCE = 362,
     ROOMDOOR_ID = 363,
     IF_ID = 364,
     ELSE_ID = 365,
     TERRAIN_ID = 366,
     HORIZ_OR_VERT = 367,
     REPLACE_TERRAIN_ID = 368,
     LOCATION_SUBTYPE_ID = 369,
     DOOR_SUBTYPE = 370,
     EXIT_ID = 371,
     SHUFFLE_ID = 372,
     QUANTITY_ID = 373,
     BURIED_ID = 374,
     LOOP_ID = 375,
     FOR_ID = 376,
     TO_ID = 377,
     SWITCH_ID = 378,
     CASE_ID = 379,
     BREAK_ID = 380,
     DEFAULT_ID = 381,
     ERODED_ID = 382,
     TRAPPED_STATE = 383,
     RECHARGED_ID = 384,
     INVIS_ID = 385,
     GREASED_ID = 386,
     INDESTRUCTIBLE_ID = 387,
     FEMALE_ID = 388,
     WAITFORU_ID = 389,
     CANCELLED_ID = 390,
     REVIVED_ID = 391,
     AVENGE_ID = 392,
     FLEEING_ID = 393,
     BLINDED_ID = 394,
     PARALYZED_ID = 395,
     STUNNED_ID = 396,
     CONFUSED_ID = 397,
     SEENTRAPS_ID = 398,
     ALL_ID = 399,
     MONTYPE_ID = 400,
     OBJTYPE_ID = 401,
     TERTYPE_ID = 402,
     TERTYPE2_ID = 403,
     LEVER_EFFECT_TYPE = 404,
     SWITCHABLE_ID = 405,
     CONTINUOUSLY_USABLE_ID = 406,
     TARGET_ID = 407,
     TRAPTYPE_ID = 408,
     EFFECT_FLAG_ID = 409,
     GRAVE_ID = 410,
     ERODEPROOF_ID = 411,
     FUNCTION_ID = 412,
     MSG_OUTPUT_TYPE = 413,
     COMPARE_TYPE = 414,
     UNKNOWN_TYPE = 415,
     rect_ID = 416,
     fillrect_ID = 417,
     line_ID = 418,
     randline_ID = 419,
     grow_ID = 420,
     selection_ID = 421,
     flood_ID = 422,
     rndcoord_ID = 423,
     circle_ID = 424,
     ellipse_ID = 425,
     filter_ID = 426,
     complement_ID = 427,
     gradient_ID = 428,
     GRADIENT_TYPE = 429,
     LIMITED = 430,
     HUMIDITY_TYPE = 431,
     STRING = 432,
     MAP_ID = 433,
     NQSTRING = 434,
     VARSTRING = 435,
     CFUNC = 436,
     CFUNC_INT = 437,
     CFUNC_STR = 438,
     CFUNC_COORD = 439,
     CFUNC_REGION = 440,
     VARSTRING_INT = 441,
     VARSTRING_INT_ARRAY = 442,
     VARSTRING_STRING = 443,
     VARSTRING_STRING_ARRAY = 444,
     VARSTRING_VAR = 445,
     VARSTRING_VAR_ARRAY = 446,
     VARSTRING_COORD = 447,
     VARSTRING_COORD_ARRAY = 448,
     VARSTRING_REGION = 449,
     VARSTRING_REGION_ARRAY = 450,
     VARSTRING_MAPCHAR = 451,
     VARSTRING_MAPCHAR_ARRAY = 452,
     VARSTRING_MONST = 453,
     VARSTRING_MONST_ARRAY = 454,
     VARSTRING_OBJ = 455,
     VARSTRING_OBJ_ARRAY = 456,
     VARSTRING_SEL = 457,
     VARSTRING_SEL_ARRAY = 458,
     METHOD_INT = 459,
     METHOD_INT_ARRAY = 460,
     METHOD_STRING = 461,
     METHOD_STRING_ARRAY = 462,
     METHOD_VAR = 463,
     METHOD_VAR_ARRAY = 464,
     METHOD_COORD = 465,
     METHOD_COORD_ARRAY = 466,
     METHOD_REGION = 467,
     METHOD_REGION_ARRAY = 468,
     METHOD_MAPCHAR = 469,
     METHOD_MAPCHAR_ARRAY = 470,
     METHOD_MONST = 471,
     METHOD_MONST_ARRAY = 472,
     METHOD_OBJ = 473,
     METHOD_OBJ_ARRAY = 474,
     METHOD_SEL = 475,
     METHOD_SEL_ARRAY = 476,
     DICE = 477
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
#line 313 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


