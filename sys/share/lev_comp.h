
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
     ARTIFACT_NAME_ID = 305,
     PORTAL_ID = 306,
     TELEPRT_ID = 307,
     BRANCH_ID = 308,
     LEV = 309,
     MINERALIZE_ID = 310,
     CORRIDOR_ID = 311,
     GOLD_ID = 312,
     ENGRAVING_ID = 313,
     FOUNTAIN_ID = 314,
     THRONE_ID = 315,
     MODRON_PORTAL_ID = 316,
     LEVEL_TELEPORTER_ID = 317,
     LEVEL_TELEPORT_DIRECTION_TYPE = 318,
     LEVEL_TELEPORT_END_TYPE = 319,
     POOL_ID = 320,
     SINK_ID = 321,
     NONE = 322,
     RAND_CORRIDOR_ID = 323,
     DOOR_STATE = 324,
     LIGHT_STATE = 325,
     CURSE_TYPE = 326,
     ENGRAVING_TYPE = 327,
     KEYTYPE_ID = 328,
     LEVER_ID = 329,
     NO_PICKUP_ID = 330,
     DIRECTION = 331,
     RANDOM_TYPE = 332,
     RANDOM_TYPE_BRACKET = 333,
     A_REGISTER = 334,
     ALIGNMENT = 335,
     LEFT_OR_RIGHT = 336,
     CENTER = 337,
     TOP_OR_BOT = 338,
     ALTAR_TYPE = 339,
     UP_OR_DOWN = 340,
     ACTIVE_OR_INACTIVE = 341,
     MODRON_PORTAL_TYPE = 342,
     NPC_TYPE = 343,
     FOUNTAIN_TYPE = 344,
     SPECIAL_OBJECT_TYPE = 345,
     CMAP_TYPE = 346,
     FLOOR_TYPE = 347,
     FLOOR_TYPE_ID = 348,
     FLOOR_ID = 349,
     FLOOR_MAIN_TYPE = 350,
     FLOOR_MAIN_TYPE_ID = 351,
     ELEMENTAL_ENCHANTMENT_TYPE = 352,
     EXCEPTIONALITY_TYPE = 353,
     EXCEPTIONALITY_ID = 354,
     ELEMENTAL_ENCHANTMENT_ID = 355,
     ENCHANTMENT_ID = 356,
     SECRET_DOOR_ID = 357,
     USES_UP_KEY_ID = 358,
     CHARGES_ID = 359,
     SPECIAL_QUALITY_ID = 360,
     SPEFLAGS_ID = 361,
     SUBROOM_ID = 362,
     NAME_ID = 363,
     FLAGS_ID = 364,
     FLAG_TYPE = 365,
     MON_ATTITUDE = 366,
     MON_ALERTNESS = 367,
     SUBTYPE_ID = 368,
     NON_PASSDOOR_ID = 369,
     MON_APPEARANCE = 370,
     ROOMDOOR_ID = 371,
     IF_ID = 372,
     ELSE_ID = 373,
     TERRAIN_ID = 374,
     HORIZ_OR_VERT = 375,
     REPLACE_TERRAIN_ID = 376,
     LOCATION_SUBTYPE_ID = 377,
     DOOR_SUBTYPE = 378,
     BRAZIER_SUBTYPE = 379,
     SIGNPOST_SUBTYPE = 380,
     EXIT_ID = 381,
     SHUFFLE_ID = 382,
     QUANTITY_ID = 383,
     BURIED_ID = 384,
     LOOP_ID = 385,
     FOR_ID = 386,
     TO_ID = 387,
     SWITCH_ID = 388,
     CASE_ID = 389,
     BREAK_ID = 390,
     DEFAULT_ID = 391,
     ERODED_ID = 392,
     TRAPPED_STATE = 393,
     RECHARGED_ID = 394,
     INVIS_ID = 395,
     GREASED_ID = 396,
     INDESTRUCTIBLE_ID = 397,
     FEMALE_ID = 398,
     WAITFORU_ID = 399,
     CANCELLED_ID = 400,
     REVIVED_ID = 401,
     AVENGE_ID = 402,
     FLEEING_ID = 403,
     BLINDED_ID = 404,
     PARALYZED_ID = 405,
     STUNNED_ID = 406,
     CONFUSED_ID = 407,
     SEENTRAPS_ID = 408,
     ALL_ID = 409,
     MONTYPE_ID = 410,
     OBJTYPE_ID = 411,
     TERTYPE_ID = 412,
     TERTYPE2_ID = 413,
     LEVER_EFFECT_TYPE = 414,
     SWITCHABLE_ID = 415,
     CONTINUOUSLY_USABLE_ID = 416,
     TARGET_ID = 417,
     TRAPTYPE_ID = 418,
     EFFECT_FLAG_ID = 419,
     GRAVE_ID = 420,
     BRAZIER_ID = 421,
     SIGNPOST_ID = 422,
     ERODEPROOF_ID = 423,
     FUNCTION_ID = 424,
     MSG_OUTPUT_TYPE = 425,
     COMPARE_TYPE = 426,
     UNKNOWN_TYPE = 427,
     rect_ID = 428,
     fillrect_ID = 429,
     line_ID = 430,
     randline_ID = 431,
     grow_ID = 432,
     selection_ID = 433,
     flood_ID = 434,
     rndcoord_ID = 435,
     circle_ID = 436,
     ellipse_ID = 437,
     filter_ID = 438,
     complement_ID = 439,
     gradient_ID = 440,
     GRADIENT_TYPE = 441,
     LIMITED = 442,
     HUMIDITY_TYPE = 443,
     STRING = 444,
     MAP_ID = 445,
     NQSTRING = 446,
     VARSTRING = 447,
     CFUNC = 448,
     CFUNC_INT = 449,
     CFUNC_STR = 450,
     CFUNC_COORD = 451,
     CFUNC_REGION = 452,
     VARSTRING_INT = 453,
     VARSTRING_INT_ARRAY = 454,
     VARSTRING_STRING = 455,
     VARSTRING_STRING_ARRAY = 456,
     VARSTRING_VAR = 457,
     VARSTRING_VAR_ARRAY = 458,
     VARSTRING_COORD = 459,
     VARSTRING_COORD_ARRAY = 460,
     VARSTRING_REGION = 461,
     VARSTRING_REGION_ARRAY = 462,
     VARSTRING_MAPCHAR = 463,
     VARSTRING_MAPCHAR_ARRAY = 464,
     VARSTRING_MONST = 465,
     VARSTRING_MONST_ARRAY = 466,
     VARSTRING_OBJ = 467,
     VARSTRING_OBJ_ARRAY = 468,
     VARSTRING_SEL = 469,
     VARSTRING_SEL_ARRAY = 470,
     METHOD_INT = 471,
     METHOD_INT_ARRAY = 472,
     METHOD_STRING = 473,
     METHOD_STRING_ARRAY = 474,
     METHOD_VAR = 475,
     METHOD_VAR_ARRAY = 476,
     METHOD_COORD = 477,
     METHOD_COORD_ARRAY = 478,
     METHOD_REGION = 479,
     METHOD_REGION_ARRAY = 480,
     METHOD_MAPCHAR = 481,
     METHOD_MAPCHAR_ARRAY = 482,
     METHOD_MONST = 483,
     METHOD_MONST_ARRAY = 484,
     METHOD_OBJ = 485,
     METHOD_OBJ_ARRAY = 486,
     METHOD_SEL = 487,
     METHOD_SEL_ARRAY = 488,
     DICE = 489
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
#line 325 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


