
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
     SPECIAL_TILESET_ID = 277,
     OBJECT_ID = 278,
     COBJECT_ID = 279,
     MONSTER_ID = 280,
     TRAP_ID = 281,
     DOOR_ID = 282,
     DRAWBRIDGE_ID = 283,
     object_ID = 284,
     monster_ID = 285,
     terrain_ID = 286,
     MAZEWALK_ID = 287,
     WALLIFY_ID = 288,
     REGION_ID = 289,
     SPECIAL_REGION_ID = 290,
     SPECIAL_LEVREGION_ID = 291,
     SPECIAL_REGION_TYPE = 292,
     NAMING_ID = 293,
     NAMING_TYPE = 294,
     FILLING = 295,
     IRREGULAR = 296,
     JOINED = 297,
     ALTAR_ID = 298,
     ANVIL_ID = 299,
     NPC_ID = 300,
     LADDER_ID = 301,
     STAIR_ID = 302,
     NON_DIGGABLE_ID = 303,
     NON_PASSWALL_ID = 304,
     ROOM_ID = 305,
     ARTIFACT_NAME_ID = 306,
     PORTAL_ID = 307,
     TELEPRT_ID = 308,
     BRANCH_ID = 309,
     LEV = 310,
     MINERALIZE_ID = 311,
     AGE_ID = 312,
     CORRIDOR_ID = 313,
     GOLD_ID = 314,
     ENGRAVING_ID = 315,
     FOUNTAIN_ID = 316,
     THRONE_ID = 317,
     MODRON_PORTAL_ID = 318,
     LEVEL_TELEPORTER_ID = 319,
     LEVEL_TELEPORT_DIRECTION_TYPE = 320,
     LEVEL_TELEPORT_END_TYPE = 321,
     POOL_ID = 322,
     SINK_ID = 323,
     NONE = 324,
     RAND_CORRIDOR_ID = 325,
     DOOR_STATE = 326,
     LIGHT_STATE = 327,
     CURSE_TYPE = 328,
     ENGRAVING_TYPE = 329,
     KEYTYPE_ID = 330,
     LEVER_ID = 331,
     NO_PICKUP_ID = 332,
     DIRECTION = 333,
     RANDOM_TYPE = 334,
     RANDOM_TYPE_BRACKET = 335,
     A_REGISTER = 336,
     ALIGNMENT = 337,
     LEFT_OR_RIGHT = 338,
     CENTER = 339,
     TOP_OR_BOT = 340,
     ALTAR_TYPE = 341,
     UP_OR_DOWN = 342,
     ACTIVE_OR_INACTIVE = 343,
     MODRON_PORTAL_TYPE = 344,
     NPC_TYPE = 345,
     FOUNTAIN_TYPE = 346,
     SPECIAL_OBJECT_TYPE = 347,
     CMAP_TYPE = 348,
     FLOOR_SUBTYPE = 349,
     FLOOR_SUBTYPE_ID = 350,
     FLOOR_ID = 351,
     FLOOR_TYPE = 352,
     FLOOR_TYPE_ID = 353,
     ELEMENTAL_ENCHANTMENT_TYPE = 354,
     EXCEPTIONALITY_TYPE = 355,
     EXCEPTIONALITY_ID = 356,
     ELEMENTAL_ENCHANTMENT_ID = 357,
     ENCHANTMENT_ID = 358,
     SECRET_DOOR_ID = 359,
     USES_UP_KEY_ID = 360,
     MYTHIC_PREFIX_TYPE = 361,
     MYTHIC_SUFFIX_TYPE = 362,
     MYTHIC_PREFIX_ID = 363,
     MYTHIC_SUFFIX_ID = 364,
     CHARGES_ID = 365,
     SPECIAL_QUALITY_ID = 366,
     SPEFLAGS_ID = 367,
     SUBROOM_ID = 368,
     NAME_ID = 369,
     FLAGS_ID = 370,
     FLAG_TYPE = 371,
     MON_ATTITUDE = 372,
     MON_ALERTNESS = 373,
     SUBTYPE_ID = 374,
     NON_PASSDOOR_ID = 375,
     MON_APPEARANCE = 376,
     ROOMDOOR_ID = 377,
     IF_ID = 378,
     ELSE_ID = 379,
     TERRAIN_ID = 380,
     HORIZ_OR_VERT = 381,
     REPLACE_TERRAIN_ID = 382,
     LOCATION_SUBTYPE_ID = 383,
     DOOR_SUBTYPE = 384,
     BRAZIER_SUBTYPE = 385,
     SIGNPOST_SUBTYPE = 386,
     TREE_SUBTYPE = 387,
     FOREST_ID = 388,
     FOREST_TYPE = 389,
     INITIALIZE_TYPE = 390,
     EXIT_ID = 391,
     SHUFFLE_ID = 392,
     QUANTITY_ID = 393,
     BURIED_ID = 394,
     LOOP_ID = 395,
     FOR_ID = 396,
     TO_ID = 397,
     SWITCH_ID = 398,
     CASE_ID = 399,
     BREAK_ID = 400,
     DEFAULT_ID = 401,
     ERODED_ID = 402,
     TRAPPED_STATE = 403,
     RECHARGED_ID = 404,
     INVIS_ID = 405,
     GREASED_ID = 406,
     INDESTRUCTIBLE_ID = 407,
     FEMALE_ID = 408,
     WAITFORU_ID = 409,
     CANCELLED_ID = 410,
     REVIVED_ID = 411,
     AVENGE_ID = 412,
     FLEEING_ID = 413,
     BLINDED_ID = 414,
     PARALYZED_ID = 415,
     STUNNED_ID = 416,
     CONFUSED_ID = 417,
     SEENTRAPS_ID = 418,
     ALL_ID = 419,
     MONTYPE_ID = 420,
     OBJTYPE_ID = 421,
     TERTYPE_ID = 422,
     TERTYPE2_ID = 423,
     LEVER_EFFECT_TYPE = 424,
     SWITCHABLE_ID = 425,
     CONTINUOUSLY_USABLE_ID = 426,
     TARGET_ID = 427,
     TRAPTYPE_ID = 428,
     EFFECT_FLAG_ID = 429,
     GRAVE_ID = 430,
     BRAZIER_ID = 431,
     SIGNPOST_ID = 432,
     TREE_ID = 433,
     ERODEPROOF_ID = 434,
     FUNCTION_ID = 435,
     MSG_OUTPUT_TYPE = 436,
     COMPARE_TYPE = 437,
     UNKNOWN_TYPE = 438,
     rect_ID = 439,
     fillrect_ID = 440,
     line_ID = 441,
     randline_ID = 442,
     grow_ID = 443,
     selection_ID = 444,
     flood_ID = 445,
     rndcoord_ID = 446,
     circle_ID = 447,
     ellipse_ID = 448,
     filter_ID = 449,
     complement_ID = 450,
     gradient_ID = 451,
     GRADIENT_TYPE = 452,
     LIMITED = 453,
     HUMIDITY_TYPE = 454,
     STRING = 455,
     MAP_ID = 456,
     NQSTRING = 457,
     VARSTRING = 458,
     CFUNC = 459,
     CFUNC_INT = 460,
     CFUNC_STR = 461,
     CFUNC_COORD = 462,
     CFUNC_REGION = 463,
     VARSTRING_INT = 464,
     VARSTRING_INT_ARRAY = 465,
     VARSTRING_STRING = 466,
     VARSTRING_STRING_ARRAY = 467,
     VARSTRING_VAR = 468,
     VARSTRING_VAR_ARRAY = 469,
     VARSTRING_COORD = 470,
     VARSTRING_COORD_ARRAY = 471,
     VARSTRING_REGION = 472,
     VARSTRING_REGION_ARRAY = 473,
     VARSTRING_MAPCHAR = 474,
     VARSTRING_MAPCHAR_ARRAY = 475,
     VARSTRING_MONST = 476,
     VARSTRING_MONST_ARRAY = 477,
     VARSTRING_OBJ = 478,
     VARSTRING_OBJ_ARRAY = 479,
     VARSTRING_SEL = 480,
     VARSTRING_SEL_ARRAY = 481,
     METHOD_INT = 482,
     METHOD_INT_ARRAY = 483,
     METHOD_STRING = 484,
     METHOD_STRING_ARRAY = 485,
     METHOD_VAR = 486,
     METHOD_VAR_ARRAY = 487,
     METHOD_COORD = 488,
     METHOD_COORD_ARRAY = 489,
     METHOD_REGION = 490,
     METHOD_REGION_ARRAY = 491,
     METHOD_MAPCHAR = 492,
     METHOD_MAPCHAR_ARRAY = 493,
     METHOD_MONST = 494,
     METHOD_MONST_ARRAY = 495,
     METHOD_OBJ = 496,
     METHOD_OBJ_ARRAY = 497,
     METHOD_SEL = 498,
     METHOD_SEL_ARRAY = 499,
     DICE = 500
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
#line 336 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


