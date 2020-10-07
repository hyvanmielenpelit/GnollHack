
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
     LEVEL_TELEPORTER_ID = 316,
     LEVEL_TELEPORT_DIRECTION_TYPE = 317,
     LEVEL_TELEPORT_END_TYPE = 318,
     POOL_ID = 319,
     SINK_ID = 320,
     NONE = 321,
     RAND_CORRIDOR_ID = 322,
     DOOR_STATE = 323,
     LIGHT_STATE = 324,
     CURSE_TYPE = 325,
     ENGRAVING_TYPE = 326,
     KEYTYPE_ID = 327,
     LEVER_ID = 328,
     DIRECTION = 329,
     RANDOM_TYPE = 330,
     RANDOM_TYPE_BRACKET = 331,
     A_REGISTER = 332,
     ALIGNMENT = 333,
     LEFT_OR_RIGHT = 334,
     CENTER = 335,
     TOP_OR_BOT = 336,
     ALTAR_TYPE = 337,
     UP_OR_DOWN = 338,
     ACTIVE_OR_INACTIVE = 339,
     MODRON_PORTAL_TYPE = 340,
     NPC_TYPE = 341,
     FOUNTAIN_TYPE = 342,
     SPECIAL_OBJECT_TYPE = 343,
     CMAP_TYPE = 344,
     FLOOR_TYPE = 345,
     FLOOR_TYPE_ID = 346,
     FLOOR_ID = 347,
     FLOOR_MAIN_TYPE = 348,
     FLOOR_MAIN_TYPE_ID = 349,
     ELEMENTAL_ENCHANTMENT_TYPE = 350,
     EXCEPTIONALITY_TYPE = 351,
     EXCEPTIONALITY_ID = 352,
     ELEMENTAL_ENCHANTMENT_ID = 353,
     ENCHANTMENT_ID = 354,
     CHARGES_ID = 355,
     SPECIAL_QUALITY_ID = 356,
     SPEFLAGS_ID = 357,
     SUBROOM_ID = 358,
     NAME_ID = 359,
     FLAGS_ID = 360,
     FLAG_TYPE = 361,
     MON_ATTITUDE = 362,
     MON_ALERTNESS = 363,
     SUBTYPE_ID = 364,
     MON_APPEARANCE = 365,
     ROOMDOOR_ID = 366,
     IF_ID = 367,
     ELSE_ID = 368,
     TERRAIN_ID = 369,
     HORIZ_OR_VERT = 370,
     REPLACE_TERRAIN_ID = 371,
     LOCATION_SUBTYPE_ID = 372,
     DOOR_SUBTYPE = 373,
     EXIT_ID = 374,
     SHUFFLE_ID = 375,
     QUANTITY_ID = 376,
     BURIED_ID = 377,
     LOOP_ID = 378,
     FOR_ID = 379,
     TO_ID = 380,
     SWITCH_ID = 381,
     CASE_ID = 382,
     BREAK_ID = 383,
     DEFAULT_ID = 384,
     ERODED_ID = 385,
     TRAPPED_STATE = 386,
     RECHARGED_ID = 387,
     INVIS_ID = 388,
     GREASED_ID = 389,
     INDESTRUCTIBLE_ID = 390,
     FEMALE_ID = 391,
     WAITFORU_ID = 392,
     CANCELLED_ID = 393,
     REVIVED_ID = 394,
     AVENGE_ID = 395,
     FLEEING_ID = 396,
     BLINDED_ID = 397,
     PARALYZED_ID = 398,
     STUNNED_ID = 399,
     CONFUSED_ID = 400,
     SEENTRAPS_ID = 401,
     ALL_ID = 402,
     MONTYPE_ID = 403,
     OBJTYPE_ID = 404,
     TERTYPE_ID = 405,
     TERTYPE2_ID = 406,
     LEVER_EFFECT_TYPE = 407,
     SWITCHABLE_ID = 408,
     CONTINUOUSLY_USABLE_ID = 409,
     TARGET_ID = 410,
     TRAPTYPE_ID = 411,
     EFFECT_FLAG_ID = 412,
     GRAVE_ID = 413,
     ERODEPROOF_ID = 414,
     FUNCTION_ID = 415,
     MSG_OUTPUT_TYPE = 416,
     COMPARE_TYPE = 417,
     UNKNOWN_TYPE = 418,
     rect_ID = 419,
     fillrect_ID = 420,
     line_ID = 421,
     randline_ID = 422,
     grow_ID = 423,
     selection_ID = 424,
     flood_ID = 425,
     rndcoord_ID = 426,
     circle_ID = 427,
     ellipse_ID = 428,
     filter_ID = 429,
     complement_ID = 430,
     gradient_ID = 431,
     GRADIENT_TYPE = 432,
     LIMITED = 433,
     HUMIDITY_TYPE = 434,
     STRING = 435,
     MAP_ID = 436,
     NQSTRING = 437,
     VARSTRING = 438,
     CFUNC = 439,
     CFUNC_INT = 440,
     CFUNC_STR = 441,
     CFUNC_COORD = 442,
     CFUNC_REGION = 443,
     VARSTRING_INT = 444,
     VARSTRING_INT_ARRAY = 445,
     VARSTRING_STRING = 446,
     VARSTRING_STRING_ARRAY = 447,
     VARSTRING_VAR = 448,
     VARSTRING_VAR_ARRAY = 449,
     VARSTRING_COORD = 450,
     VARSTRING_COORD_ARRAY = 451,
     VARSTRING_REGION = 452,
     VARSTRING_REGION_ARRAY = 453,
     VARSTRING_MAPCHAR = 454,
     VARSTRING_MAPCHAR_ARRAY = 455,
     VARSTRING_MONST = 456,
     VARSTRING_MONST_ARRAY = 457,
     VARSTRING_OBJ = 458,
     VARSTRING_OBJ_ARRAY = 459,
     VARSTRING_SEL = 460,
     VARSTRING_SEL_ARRAY = 461,
     METHOD_INT = 462,
     METHOD_INT_ARRAY = 463,
     METHOD_STRING = 464,
     METHOD_STRING_ARRAY = 465,
     METHOD_VAR = 466,
     METHOD_VAR_ARRAY = 467,
     METHOD_COORD = 468,
     METHOD_COORD_ARRAY = 469,
     METHOD_REGION = 470,
     METHOD_REGION_ARRAY = 471,
     METHOD_MAPCHAR = 472,
     METHOD_MAPCHAR_ARRAY = 473,
     METHOD_MONST = 474,
     METHOD_MONST_ARRAY = 475,
     METHOD_OBJ = 476,
     METHOD_OBJ_ARRAY = 477,
     METHOD_SEL = 478,
     METHOD_SEL_ARRAY = 479,
     DICE = 480
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
#line 316 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


