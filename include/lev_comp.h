
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
     SECRET_DOOR_ID = 355,
     USES_UP_KEY_ID = 356,
     CHARGES_ID = 357,
     SPECIAL_QUALITY_ID = 358,
     SPEFLAGS_ID = 359,
     SUBROOM_ID = 360,
     NAME_ID = 361,
     FLAGS_ID = 362,
     FLAG_TYPE = 363,
     MON_ATTITUDE = 364,
     MON_ALERTNESS = 365,
     SUBTYPE_ID = 366,
     MON_APPEARANCE = 367,
     ROOMDOOR_ID = 368,
     IF_ID = 369,
     ELSE_ID = 370,
     TERRAIN_ID = 371,
     HORIZ_OR_VERT = 372,
     REPLACE_TERRAIN_ID = 373,
     LOCATION_SUBTYPE_ID = 374,
     DOOR_SUBTYPE = 375,
     EXIT_ID = 376,
     SHUFFLE_ID = 377,
     QUANTITY_ID = 378,
     BURIED_ID = 379,
     LOOP_ID = 380,
     FOR_ID = 381,
     TO_ID = 382,
     SWITCH_ID = 383,
     CASE_ID = 384,
     BREAK_ID = 385,
     DEFAULT_ID = 386,
     ERODED_ID = 387,
     TRAPPED_STATE = 388,
     RECHARGED_ID = 389,
     INVIS_ID = 390,
     GREASED_ID = 391,
     INDESTRUCTIBLE_ID = 392,
     FEMALE_ID = 393,
     WAITFORU_ID = 394,
     CANCELLED_ID = 395,
     REVIVED_ID = 396,
     AVENGE_ID = 397,
     FLEEING_ID = 398,
     BLINDED_ID = 399,
     PARALYZED_ID = 400,
     STUNNED_ID = 401,
     CONFUSED_ID = 402,
     SEENTRAPS_ID = 403,
     ALL_ID = 404,
     MONTYPE_ID = 405,
     OBJTYPE_ID = 406,
     TERTYPE_ID = 407,
     TERTYPE2_ID = 408,
     LEVER_EFFECT_TYPE = 409,
     SWITCHABLE_ID = 410,
     CONTINUOUSLY_USABLE_ID = 411,
     TARGET_ID = 412,
     TRAPTYPE_ID = 413,
     EFFECT_FLAG_ID = 414,
     GRAVE_ID = 415,
     ERODEPROOF_ID = 416,
     FUNCTION_ID = 417,
     MSG_OUTPUT_TYPE = 418,
     COMPARE_TYPE = 419,
     UNKNOWN_TYPE = 420,
     rect_ID = 421,
     fillrect_ID = 422,
     line_ID = 423,
     randline_ID = 424,
     grow_ID = 425,
     selection_ID = 426,
     flood_ID = 427,
     rndcoord_ID = 428,
     circle_ID = 429,
     ellipse_ID = 430,
     filter_ID = 431,
     complement_ID = 432,
     gradient_ID = 433,
     GRADIENT_TYPE = 434,
     LIMITED = 435,
     HUMIDITY_TYPE = 436,
     STRING = 437,
     MAP_ID = 438,
     NQSTRING = 439,
     VARSTRING = 440,
     CFUNC = 441,
     CFUNC_INT = 442,
     CFUNC_STR = 443,
     CFUNC_COORD = 444,
     CFUNC_REGION = 445,
     VARSTRING_INT = 446,
     VARSTRING_INT_ARRAY = 447,
     VARSTRING_STRING = 448,
     VARSTRING_STRING_ARRAY = 449,
     VARSTRING_VAR = 450,
     VARSTRING_VAR_ARRAY = 451,
     VARSTRING_COORD = 452,
     VARSTRING_COORD_ARRAY = 453,
     VARSTRING_REGION = 454,
     VARSTRING_REGION_ARRAY = 455,
     VARSTRING_MAPCHAR = 456,
     VARSTRING_MAPCHAR_ARRAY = 457,
     VARSTRING_MONST = 458,
     VARSTRING_MONST_ARRAY = 459,
     VARSTRING_OBJ = 460,
     VARSTRING_OBJ_ARRAY = 461,
     VARSTRING_SEL = 462,
     VARSTRING_SEL_ARRAY = 463,
     METHOD_INT = 464,
     METHOD_INT_ARRAY = 465,
     METHOD_STRING = 466,
     METHOD_STRING_ARRAY = 467,
     METHOD_VAR = 468,
     METHOD_VAR_ARRAY = 469,
     METHOD_COORD = 470,
     METHOD_COORD_ARRAY = 471,
     METHOD_REGION = 472,
     METHOD_REGION_ARRAY = 473,
     METHOD_MAPCHAR = 474,
     METHOD_MAPCHAR_ARRAY = 475,
     METHOD_MONST = 476,
     METHOD_MONST_ARRAY = 477,
     METHOD_OBJ = 478,
     METHOD_OBJ_ARRAY = 479,
     METHOD_SEL = 480,
     METHOD_SEL_ARRAY = 481,
     DICE = 482
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
#line 318 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


