
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
     POOL_ID = 318,
     SINK_ID = 319,
     NONE = 320,
     RAND_CORRIDOR_ID = 321,
     DOOR_STATE = 322,
     LIGHT_STATE = 323,
     CURSE_TYPE = 324,
     ENGRAVING_TYPE = 325,
     KEYTYPE_ID = 326,
     LEVER_ID = 327,
     DIRECTION = 328,
     RANDOM_TYPE = 329,
     RANDOM_TYPE_BRACKET = 330,
     A_REGISTER = 331,
     ALIGNMENT = 332,
     LEFT_OR_RIGHT = 333,
     CENTER = 334,
     TOP_OR_BOT = 335,
     ALTAR_TYPE = 336,
     UP_OR_DOWN = 337,
     ACTIVE_OR_INACTIVE = 338,
     MODRON_PORTAL_TYPE = 339,
     NPC_TYPE = 340,
     FOUNTAIN_TYPE = 341,
     SPECIAL_OBJECT_TYPE = 342,
     CMAP_TYPE = 343,
     FLOOR_TYPE = 344,
     FLOOR_TYPE_ID = 345,
     FLOOR_ID = 346,
     FLOOR_MAIN_TYPE = 347,
     FLOOR_MAIN_TYPE_ID = 348,
     ELEMENTAL_ENCHANTMENT_TYPE = 349,
     EXCEPTIONALITY_TYPE = 350,
     EXCEPTIONALITY_ID = 351,
     ELEMENTAL_ENCHANTMENT_ID = 352,
     ENCHANTMENT_ID = 353,
     CHARGES_ID = 354,
     SPECIAL_QUALITY_ID = 355,
     SPEFLAGS_ID = 356,
     SUBROOM_ID = 357,
     NAME_ID = 358,
     FLAGS_ID = 359,
     FLAG_TYPE = 360,
     MON_ATTITUDE = 361,
     MON_ALERTNESS = 362,
     SUBTYPE_ID = 363,
     MON_APPEARANCE = 364,
     ROOMDOOR_ID = 365,
     IF_ID = 366,
     ELSE_ID = 367,
     TERRAIN_ID = 368,
     HORIZ_OR_VERT = 369,
     REPLACE_TERRAIN_ID = 370,
     LOCATION_SUBTYPE_ID = 371,
     DOOR_SUBTYPE = 372,
     EXIT_ID = 373,
     SHUFFLE_ID = 374,
     QUANTITY_ID = 375,
     BURIED_ID = 376,
     LOOP_ID = 377,
     FOR_ID = 378,
     TO_ID = 379,
     SWITCH_ID = 380,
     CASE_ID = 381,
     BREAK_ID = 382,
     DEFAULT_ID = 383,
     ERODED_ID = 384,
     TRAPPED_STATE = 385,
     RECHARGED_ID = 386,
     INVIS_ID = 387,
     GREASED_ID = 388,
     INDESTRUCTIBLE_ID = 389,
     FEMALE_ID = 390,
     WAITFORU_ID = 391,
     CANCELLED_ID = 392,
     REVIVED_ID = 393,
     AVENGE_ID = 394,
     FLEEING_ID = 395,
     BLINDED_ID = 396,
     PARALYZED_ID = 397,
     STUNNED_ID = 398,
     CONFUSED_ID = 399,
     SEENTRAPS_ID = 400,
     ALL_ID = 401,
     MONTYPE_ID = 402,
     OBJTYPE_ID = 403,
     TERTYPE_ID = 404,
     TERTYPE2_ID = 405,
     LEVER_EFFECT_TYPE = 406,
     SWITCHABLE_ID = 407,
     CONTINUOUSLY_USABLE_ID = 408,
     TARGET_ID = 409,
     TRAPTYPE_ID = 410,
     EFFECT_FLAG_ID = 411,
     GRAVE_ID = 412,
     ERODEPROOF_ID = 413,
     FUNCTION_ID = 414,
     MSG_OUTPUT_TYPE = 415,
     COMPARE_TYPE = 416,
     UNKNOWN_TYPE = 417,
     rect_ID = 418,
     fillrect_ID = 419,
     line_ID = 420,
     randline_ID = 421,
     grow_ID = 422,
     selection_ID = 423,
     flood_ID = 424,
     rndcoord_ID = 425,
     circle_ID = 426,
     ellipse_ID = 427,
     filter_ID = 428,
     complement_ID = 429,
     gradient_ID = 430,
     GRADIENT_TYPE = 431,
     LIMITED = 432,
     HUMIDITY_TYPE = 433,
     STRING = 434,
     MAP_ID = 435,
     NQSTRING = 436,
     VARSTRING = 437,
     CFUNC = 438,
     CFUNC_INT = 439,
     CFUNC_STR = 440,
     CFUNC_COORD = 441,
     CFUNC_REGION = 442,
     VARSTRING_INT = 443,
     VARSTRING_INT_ARRAY = 444,
     VARSTRING_STRING = 445,
     VARSTRING_STRING_ARRAY = 446,
     VARSTRING_VAR = 447,
     VARSTRING_VAR_ARRAY = 448,
     VARSTRING_COORD = 449,
     VARSTRING_COORD_ARRAY = 450,
     VARSTRING_REGION = 451,
     VARSTRING_REGION_ARRAY = 452,
     VARSTRING_MAPCHAR = 453,
     VARSTRING_MAPCHAR_ARRAY = 454,
     VARSTRING_MONST = 455,
     VARSTRING_MONST_ARRAY = 456,
     VARSTRING_OBJ = 457,
     VARSTRING_OBJ_ARRAY = 458,
     VARSTRING_SEL = 459,
     VARSTRING_SEL_ARRAY = 460,
     METHOD_INT = 461,
     METHOD_INT_ARRAY = 462,
     METHOD_STRING = 463,
     METHOD_STRING_ARRAY = 464,
     METHOD_VAR = 465,
     METHOD_VAR_ARRAY = 466,
     METHOD_COORD = 467,
     METHOD_COORD_ARRAY = 468,
     METHOD_REGION = 469,
     METHOD_REGION_ARRAY = 470,
     METHOD_MAPCHAR = 471,
     METHOD_MAPCHAR_ARRAY = 472,
     METHOD_MONST = 473,
     METHOD_MONST_ARRAY = 474,
     METHOD_OBJ = 475,
     METHOD_OBJ_ARRAY = 476,
     METHOD_SEL = 477,
     METHOD_SEL_ARRAY = 478,
     DICE = 479
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
#line 315 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


