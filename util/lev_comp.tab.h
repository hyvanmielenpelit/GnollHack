
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
     ALTAR_SUBTYPE = 342,
     UP_OR_DOWN = 343,
     ACTIVE_OR_INACTIVE = 344,
     MODRON_PORTAL_TYPE = 345,
     NPC_TYPE = 346,
     FOUNTAIN_TYPE = 347,
     SPECIAL_OBJECT_TYPE = 348,
     CMAP_TYPE = 349,
     FLOOR_SUBTYPE = 350,
     FLOOR_SUBTYPE_ID = 351,
     FLOOR_ID = 352,
     FLOOR_TYPE = 353,
     FLOOR_TYPE_ID = 354,
     ELEMENTAL_ENCHANTMENT_TYPE = 355,
     EXCEPTIONALITY_TYPE = 356,
     EXCEPTIONALITY_ID = 357,
     ELEMENTAL_ENCHANTMENT_ID = 358,
     ENCHANTMENT_ID = 359,
     SECRET_DOOR_ID = 360,
     USES_UP_KEY_ID = 361,
     MYTHIC_PREFIX_TYPE = 362,
     MYTHIC_SUFFIX_TYPE = 363,
     MYTHIC_PREFIX_ID = 364,
     MYTHIC_SUFFIX_ID = 365,
     CHARGES_ID = 366,
     SPECIAL_QUALITY_ID = 367,
     SPEFLAGS_ID = 368,
     SUBROOM_ID = 369,
     NAME_ID = 370,
     FLAGS_ID = 371,
     FLAG_TYPE = 372,
     MON_ATTITUDE = 373,
     MON_ALERTNESS = 374,
     SUBTYPE_ID = 375,
     NON_PASSDOOR_ID = 376,
     MON_APPEARANCE = 377,
     ROOMDOOR_ID = 378,
     IF_ID = 379,
     ELSE_ID = 380,
     TERRAIN_ID = 381,
     HORIZ_OR_VERT = 382,
     REPLACE_TERRAIN_ID = 383,
     LOCATION_SUBTYPE_ID = 384,
     DOOR_SUBTYPE = 385,
     BRAZIER_SUBTYPE = 386,
     SIGNPOST_SUBTYPE = 387,
     TREE_SUBTYPE = 388,
     FOREST_ID = 389,
     FOREST_TYPE = 390,
     INITIALIZE_TYPE = 391,
     EXIT_ID = 392,
     SHUFFLE_ID = 393,
     QUANTITY_ID = 394,
     BURIED_ID = 395,
     LOOP_ID = 396,
     FOR_ID = 397,
     TO_ID = 398,
     SWITCH_ID = 399,
     CASE_ID = 400,
     BREAK_ID = 401,
     DEFAULT_ID = 402,
     ERODED_ID = 403,
     TRAPPED_STATE = 404,
     RECHARGED_ID = 405,
     INVIS_ID = 406,
     GREASED_ID = 407,
     INDESTRUCTIBLE_ID = 408,
     FEMALE_ID = 409,
     MALE_ID = 410,
     WAITFORU_ID = 411,
     PROTECTOR_ID = 412,
     CANCELLED_ID = 413,
     REVIVED_ID = 414,
     AVENGE_ID = 415,
     FLEEING_ID = 416,
     BLINDED_ID = 417,
     PARALYZED_ID = 418,
     STUNNED_ID = 419,
     CONFUSED_ID = 420,
     SEENTRAPS_ID = 421,
     ALL_ID = 422,
     MONTYPE_ID = 423,
     OBJTYPE_ID = 424,
     TERTYPE_ID = 425,
     TERTYPE2_ID = 426,
     LEVER_EFFECT_TYPE = 427,
     SWITCHABLE_ID = 428,
     CONTINUOUSLY_USABLE_ID = 429,
     TARGET_ID = 430,
     TRAPTYPE_ID = 431,
     EFFECT_FLAG_ID = 432,
     GRAVE_ID = 433,
     BRAZIER_ID = 434,
     SIGNPOST_ID = 435,
     TREE_ID = 436,
     ERODEPROOF_ID = 437,
     FUNCTION_ID = 438,
     MSG_OUTPUT_TYPE = 439,
     COMPARE_TYPE = 440,
     UNKNOWN_TYPE = 441,
     rect_ID = 442,
     fillrect_ID = 443,
     line_ID = 444,
     randline_ID = 445,
     grow_ID = 446,
     selection_ID = 447,
     flood_ID = 448,
     rndcoord_ID = 449,
     circle_ID = 450,
     ellipse_ID = 451,
     filter_ID = 452,
     complement_ID = 453,
     gradient_ID = 454,
     GRADIENT_TYPE = 455,
     LIMITED = 456,
     HUMIDITY_TYPE = 457,
     STRING = 458,
     MAP_ID = 459,
     NQSTRING = 460,
     VARSTRING = 461,
     CFUNC = 462,
     CFUNC_INT = 463,
     CFUNC_STR = 464,
     CFUNC_COORD = 465,
     CFUNC_REGION = 466,
     VARSTRING_INT = 467,
     VARSTRING_INT_ARRAY = 468,
     VARSTRING_STRING = 469,
     VARSTRING_STRING_ARRAY = 470,
     VARSTRING_VAR = 471,
     VARSTRING_VAR_ARRAY = 472,
     VARSTRING_COORD = 473,
     VARSTRING_COORD_ARRAY = 474,
     VARSTRING_REGION = 475,
     VARSTRING_REGION_ARRAY = 476,
     VARSTRING_MAPCHAR = 477,
     VARSTRING_MAPCHAR_ARRAY = 478,
     VARSTRING_MONST = 479,
     VARSTRING_MONST_ARRAY = 480,
     VARSTRING_OBJ = 481,
     VARSTRING_OBJ_ARRAY = 482,
     VARSTRING_SEL = 483,
     VARSTRING_SEL_ARRAY = 484,
     METHOD_INT = 485,
     METHOD_INT_ARRAY = 486,
     METHOD_STRING = 487,
     METHOD_STRING_ARRAY = 488,
     METHOD_VAR = 489,
     METHOD_VAR_ARRAY = 490,
     METHOD_COORD = 491,
     METHOD_COORD_ARRAY = 492,
     METHOD_REGION = 493,
     METHOD_REGION_ARRAY = 494,
     METHOD_MAPCHAR = 495,
     METHOD_MAPCHAR_ARRAY = 496,
     METHOD_MONST = 497,
     METHOD_MONST_ARRAY = 498,
     METHOD_OBJ = 499,
     METHOD_OBJ_ARRAY = 500,
     METHOD_SEL = 501,
     METHOD_SEL_ARRAY = 502,
     DICE = 503
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
#line 339 "lev_comp.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


