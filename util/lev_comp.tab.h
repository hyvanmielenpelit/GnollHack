
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
     NO_PICKUP_ID = 329,
     DIRECTION = 330,
     RANDOM_TYPE = 331,
     RANDOM_TYPE_BRACKET = 332,
     A_REGISTER = 333,
     ALIGNMENT = 334,
     LEFT_OR_RIGHT = 335,
     CENTER = 336,
     TOP_OR_BOT = 337,
     ALTAR_TYPE = 338,
     UP_OR_DOWN = 339,
     ACTIVE_OR_INACTIVE = 340,
     MODRON_PORTAL_TYPE = 341,
     NPC_TYPE = 342,
     FOUNTAIN_TYPE = 343,
     SPECIAL_OBJECT_TYPE = 344,
     CMAP_TYPE = 345,
     FLOOR_TYPE = 346,
     FLOOR_TYPE_ID = 347,
     FLOOR_ID = 348,
     FLOOR_MAIN_TYPE = 349,
     FLOOR_MAIN_TYPE_ID = 350,
     ELEMENTAL_ENCHANTMENT_TYPE = 351,
     EXCEPTIONALITY_TYPE = 352,
     EXCEPTIONALITY_ID = 353,
     ELEMENTAL_ENCHANTMENT_ID = 354,
     ENCHANTMENT_ID = 355,
     SECRET_DOOR_ID = 356,
     USES_UP_KEY_ID = 357,
     CHARGES_ID = 358,
     SPECIAL_QUALITY_ID = 359,
     SPEFLAGS_ID = 360,
     SUBROOM_ID = 361,
     NAME_ID = 362,
     FLAGS_ID = 363,
     FLAG_TYPE = 364,
     MON_ATTITUDE = 365,
     MON_ALERTNESS = 366,
     SUBTYPE_ID = 367,
     NON_PASSDOOR_ID = 368,
     MON_APPEARANCE = 369,
     ROOMDOOR_ID = 370,
     IF_ID = 371,
     ELSE_ID = 372,
     TERRAIN_ID = 373,
     HORIZ_OR_VERT = 374,
     REPLACE_TERRAIN_ID = 375,
     LOCATION_SUBTYPE_ID = 376,
     DOOR_SUBTYPE = 377,
     EXIT_ID = 378,
     SHUFFLE_ID = 379,
     QUANTITY_ID = 380,
     BURIED_ID = 381,
     LOOP_ID = 382,
     FOR_ID = 383,
     TO_ID = 384,
     SWITCH_ID = 385,
     CASE_ID = 386,
     BREAK_ID = 387,
     DEFAULT_ID = 388,
     ERODED_ID = 389,
     TRAPPED_STATE = 390,
     RECHARGED_ID = 391,
     INVIS_ID = 392,
     GREASED_ID = 393,
     INDESTRUCTIBLE_ID = 394,
     FEMALE_ID = 395,
     WAITFORU_ID = 396,
     CANCELLED_ID = 397,
     REVIVED_ID = 398,
     AVENGE_ID = 399,
     FLEEING_ID = 400,
     BLINDED_ID = 401,
     PARALYZED_ID = 402,
     STUNNED_ID = 403,
     CONFUSED_ID = 404,
     SEENTRAPS_ID = 405,
     ALL_ID = 406,
     MONTYPE_ID = 407,
     OBJTYPE_ID = 408,
     TERTYPE_ID = 409,
     TERTYPE2_ID = 410,
     LEVER_EFFECT_TYPE = 411,
     SWITCHABLE_ID = 412,
     CONTINUOUSLY_USABLE_ID = 413,
     TARGET_ID = 414,
     TRAPTYPE_ID = 415,
     EFFECT_FLAG_ID = 416,
     GRAVE_ID = 417,
     ERODEPROOF_ID = 418,
     FUNCTION_ID = 419,
     MSG_OUTPUT_TYPE = 420,
     COMPARE_TYPE = 421,
     UNKNOWN_TYPE = 422,
     rect_ID = 423,
     fillrect_ID = 424,
     line_ID = 425,
     randline_ID = 426,
     grow_ID = 427,
     selection_ID = 428,
     flood_ID = 429,
     rndcoord_ID = 430,
     circle_ID = 431,
     ellipse_ID = 432,
     filter_ID = 433,
     complement_ID = 434,
     gradient_ID = 435,
     GRADIENT_TYPE = 436,
     LIMITED = 437,
     HUMIDITY_TYPE = 438,
     STRING = 439,
     MAP_ID = 440,
     NQSTRING = 441,
     VARSTRING = 442,
     CFUNC = 443,
     CFUNC_INT = 444,
     CFUNC_STR = 445,
     CFUNC_COORD = 446,
     CFUNC_REGION = 447,
     VARSTRING_INT = 448,
     VARSTRING_INT_ARRAY = 449,
     VARSTRING_STRING = 450,
     VARSTRING_STRING_ARRAY = 451,
     VARSTRING_VAR = 452,
     VARSTRING_VAR_ARRAY = 453,
     VARSTRING_COORD = 454,
     VARSTRING_COORD_ARRAY = 455,
     VARSTRING_REGION = 456,
     VARSTRING_REGION_ARRAY = 457,
     VARSTRING_MAPCHAR = 458,
     VARSTRING_MAPCHAR_ARRAY = 459,
     VARSTRING_MONST = 460,
     VARSTRING_MONST_ARRAY = 461,
     VARSTRING_OBJ = 462,
     VARSTRING_OBJ_ARRAY = 463,
     VARSTRING_SEL = 464,
     VARSTRING_SEL_ARRAY = 465,
     METHOD_INT = 466,
     METHOD_INT_ARRAY = 467,
     METHOD_STRING = 468,
     METHOD_STRING_ARRAY = 469,
     METHOD_VAR = 470,
     METHOD_VAR_ARRAY = 471,
     METHOD_COORD = 472,
     METHOD_COORD_ARRAY = 473,
     METHOD_REGION = 474,
     METHOD_REGION_ARRAY = 475,
     METHOD_MAPCHAR = 476,
     METHOD_MAPCHAR_ARRAY = 477,
     METHOD_MONST = 478,
     METHOD_MONST_ARRAY = 479,
     METHOD_OBJ = 480,
     METHOD_OBJ_ARRAY = 481,
     METHOD_SEL = 482,
     METHOD_SEL_ARRAY = 483,
     DICE = 484
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
#line 320 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


