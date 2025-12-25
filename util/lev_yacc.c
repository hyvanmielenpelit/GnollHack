
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "lev_comp.y"

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2024-08-11 */

/* GnollHack 4.0  lev_comp.y	$NHDT-Date: 1543371691 2018/11/28 02:21:31 $  $NHDT-Branch: NetHack-3.6.2-beta01 $:$NHDT-Revision: 1.22 $ */
/*      Copyright (c) 1989 by Jean-Christophe Collet */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * This file contains the Level Compiler code
 * It may handle special mazes & special room-levels
 */

/* In case we're using bison in AIX.  This definition must be
 * placed before any other C-language construct in the file
 * excluding comments and preprocessor directives (thanks IBM
 * for this wonderful feature...).
 *
 * Note: some cpps barf on this 'undefined control' (#pragma).
 * Addition of the leading space seems to prevent barfage for now,
 * and AIX will still see the directive.
 */
#ifdef _AIX
 #pragma alloca         /* keep leading space! */
#endif

#define SPEC_LEV    /* for USE_OLDARGS (sp_lev.h) */
#include "hack.h"
#include "sp_lev.h"

#define ERR             (-1)
/* many types of things are put in chars for transference to NetHack.
 * since some systems will use signed chars, limit everybody to the
 * same number for portability.
 */
#define MAX_OF_TYPE     128

#define MAX_NESTED_IFS   20
#define MAX_SWITCH_CASES 20

#define New(type) \
        (type *) memset((genericptr_t) alloc(sizeof (type)), 0, sizeof (type))
#define NewTab(type, size)      (type **) alloc(sizeof (type *) * size)
#define Free(ptr)               free((genericptr_t) ptr)

extern void VDECL(lc_error, (const char *, ...));
extern void VDECL(lc_warning, (const char *, ...));
extern void FDECL(yyerror, (const char *));
extern void FDECL(yywarning, (const char *));
extern int NDECL(yylex);
int NDECL(yyparse);

extern int FDECL(get_floor_type, (CHAR_P));
extern int FDECL(get_room_type, (char *));
extern int FDECL(get_trap_type, (char *));
extern int FDECL(get_monster_id, (char *,CHAR_P));
extern int FDECL(get_object_id, (char *,CHAR_P));
extern boolean FDECL(check_monster_char, (CHAR_P));
extern boolean FDECL(check_object_char, (CHAR_P));
extern char FDECL(what_map_char, (CHAR_P));
extern void FDECL(scan_map, (char *, sp_lev *));
extern void FDECL(add_opcode, (sp_lev *, int, genericptr_t));
extern genericptr_t FDECL(get_last_opcode_data1, (sp_lev *, int));
extern genericptr_t FDECL(get_last_opcode_data2, (sp_lev *, int, int));
extern boolean FDECL(check_subrooms, (sp_lev *));
extern boolean FDECL(write_level_file, (char *,sp_lev *));
extern struct opvar *FDECL(set_opvar_int, (struct opvar *, int64_t));
extern void VDECL(add_opvars, (sp_lev *, const char *, ...));
extern void FDECL(start_level_def, (sp_lev * *, char *));

extern struct lc_funcdefs *FDECL(funcdef_new, (int64_t,char *));
extern void FDECL(funcdef_free_all, (struct lc_funcdefs *));
extern struct lc_funcdefs *FDECL(funcdef_defined, (struct lc_funcdefs *,
                                                   char *, int));
extern char *FDECL(funcdef_paramtypes, (struct lc_funcdefs *));
extern char *FDECL(decode_parm_str, (char *));

extern struct lc_vardefs *FDECL(vardef_new, (int64_t,char *));
extern void FDECL(vardef_free_all, (struct lc_vardefs *));
extern struct lc_vardefs *FDECL(vardef_defined, (struct lc_vardefs *,
                                                 char *, int));

extern void NDECL(break_stmt_start);
extern void FDECL(break_stmt_end, (sp_lev *));
extern void FDECL(break_stmt_new, (sp_lev *, int64_t));

extern void FDECL(splev_add_from, (sp_lev *, sp_lev *));

extern void FDECL(check_vardef_type, (struct lc_vardefs *, char *, int64_t));
extern void FDECL(vardef_used, (struct lc_vardefs *, char *));
extern struct lc_vardefs *FDECL(add_vardef_type, (struct lc_vardefs *,
                                                  char *, int64_t));

extern int FDECL(reverse_jmp_opcode, (int64_t));

struct coord {
    int64_t x;
    int64_t y;
};

struct forloopdef {
    char *varname;
    int64_t jmp_point;
};
static struct forloopdef forloop_list[MAX_NESTED_IFS];
static short n_forloops = 0;


sp_lev *splev = NULL;

static struct opvar *if_list[MAX_NESTED_IFS];

static short n_if_list = 0;

unsigned int max_x_map, max_y_map;
int obj_containment = 0;

int in_container_obj = 0;

/* integer value is possibly an inconstant value (eg. dice notation
   or a variable) */
int is_inconstant_number = 0;

int in_switch_statement = 0;
static struct opvar *switch_check_jump = NULL;
static struct opvar *switch_default_case = NULL;
static struct opvar *switch_case_list[MAX_SWITCH_CASES];
static int64_t switch_case_value[MAX_SWITCH_CASES];
int n_switch_case_list = 0;

int allow_break_statements = 0;
struct lc_breakdef *break_list = NULL;

extern struct lc_vardefs *vardefs; /* variable definitions */


struct lc_vardefs *function_tmp_var_defs = NULL;
extern struct lc_funcdefs *function_definitions;
struct lc_funcdefs *curr_function = NULL;
struct lc_funcdefs_parm * curr_function_param = NULL;
int in_function_definition = 0;
sp_lev *function_splev_backup = NULL;

extern int fatal_error;
extern int got_errors;
extern int line_number;
extern const char *fname;

extern char curr_token[512];



/* Line 189 of yacc.c  */
#line 225 "lev_comp.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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
     MESSAGE_TYPE = 270,
     MESSAGE_ATTR = 271,
     MESSAGE_COLOR = 272,
     MESSAGE_SOUND_TYPE = 273,
     MESSAGE_SOUND_ID = 274,
     MAZE_ID = 275,
     LEVEL_ID = 276,
     LEV_INIT_ID = 277,
     TILESET_ID = 278,
     GEOMETRY_ID = 279,
     NOMAP_ID = 280,
     BOUNDARY_TYPE_ID = 281,
     SPECIAL_TILESET_ID = 282,
     TILESET_PARAM_ID = 283,
     DECOTYP_ID = 284,
     OBJECT_ID = 285,
     COBJECT_ID = 286,
     MONSTER_ID = 287,
     TRAP_ID = 288,
     DOOR_ID = 289,
     DRAWBRIDGE_ID = 290,
     MONSTER_GENERATION_ID = 291,
     object_ID = 292,
     monster_ID = 293,
     terrain_ID = 294,
     MAZEWALK_ID = 295,
     WALLIFY_ID = 296,
     REGION_ID = 297,
     SPECIAL_REGION_ID = 298,
     SPECIAL_LEVREGION_ID = 299,
     SPECIAL_REGION_TYPE = 300,
     NAMING_ID = 301,
     NAMING_TYPE = 302,
     FILLING = 303,
     IRREGULAR = 304,
     JOINED = 305,
     ALTAR_ID = 306,
     ANVIL_ID = 307,
     NPC_ID = 308,
     LADDER_ID = 309,
     STAIR_ID = 310,
     NON_DIGGABLE_ID = 311,
     NON_PASSWALL_ID = 312,
     ROOM_ID = 313,
     ARTIFACT_NAME_ID = 314,
     PORTAL_ID = 315,
     TELEPRT_ID = 316,
     BRANCH_ID = 317,
     LEV = 318,
     MINERALIZE_ID = 319,
     AGE_ID = 320,
     CORRIDOR_ID = 321,
     GOLD_ID = 322,
     ENGRAVING_ID = 323,
     FOUNTAIN_ID = 324,
     THRONE_ID = 325,
     MAGIC_PORTAL_ID = 326,
     MODRON_PORTAL_ID = 327,
     LEVEL_TELEPORTER_ID = 328,
     LEVEL_TELEPORT_DIRECTION_TYPE = 329,
     LEVEL_TELEPORT_END_TYPE = 330,
     POOL_ID = 331,
     SINK_ID = 332,
     NONE = 333,
     RAND_CORRIDOR_ID = 334,
     DOOR_STATE = 335,
     LIGHT_STATE = 336,
     CURSE_TYPE = 337,
     MYTHIC_TYPE = 338,
     ENGRAVING_TYPE = 339,
     KEYTYPE_ID = 340,
     LEVER_ID = 341,
     NO_PICKUP_ID = 342,
     DIRECTION = 343,
     RANDOM_TYPE = 344,
     RANDOM_TYPE_BRACKET = 345,
     A_REGISTER = 346,
     ALIGNMENT = 347,
     LEFT_OR_RIGHT = 348,
     CENTER = 349,
     TOP_OR_BOT = 350,
     ALTAR_TYPE = 351,
     ALTAR_SUBTYPE = 352,
     UP_OR_DOWN = 353,
     ACTIVE_OR_INACTIVE = 354,
     MAGIC_PORTAL_TARGET_TYPE_ID = 355,
     MAGIC_PORTAL_TARGET_TYPE = 356,
     SEEN_OR_UNSEEN = 357,
     MODRON_PORTAL_TYPE = 358,
     NPC_TYPE = 359,
     FOUNTAIN_TYPE = 360,
     SPECIAL_OBJECT_TYPE = 361,
     CMAP_TYPE = 362,
     FLOOR_SUBTYPE = 363,
     FLOOR_SUBTYPE_ID = 364,
     FLOOR_ID = 365,
     FLOOR_TYPE = 366,
     FLOOR_TYPE_ID = 367,
     DECORATION_ID = 368,
     DECORATION_TYPE = 369,
     DECORATION_DIR = 370,
     DECORATION_ITEM_STATE = 371,
     PAINTING_TYPE = 372,
     BANNER_TYPE = 373,
     WALL_SCULPTURE_TYPE = 374,
     ELEMENTAL_ENCHANTMENT_TYPE = 375,
     EXCEPTIONALITY_TYPE = 376,
     EXCEPTIONALITY_ID = 377,
     ELEMENTAL_ENCHANTMENT_ID = 378,
     ENCHANTMENT_ID = 379,
     SECRET_DOOR_ID = 380,
     USES_UP_KEY_ID = 381,
     MYTHIC_PREFIX_TYPE = 382,
     MYTHIC_SUFFIX_TYPE = 383,
     MYTHIC_PREFIX_ID = 384,
     MYTHIC_SUFFIX_ID = 385,
     MATERIAL_ID = 386,
     MATERIAL_TYPE = 387,
     CHARGES_ID = 388,
     SPECIAL_QUALITY_ID = 389,
     SPEFLAGS_ID = 390,
     LEVEL_BOSS_ID = 391,
     BOSS_HOSTILITY_ID = 392,
     HAS_BACKUP_ID = 393,
     SUBROOM_ID = 394,
     NAME_ID = 395,
     FLAGS_ID = 396,
     FLAG_TYPE = 397,
     MON_ATTITUDE = 398,
     MON_ALERTNESS = 399,
     SUBTYPE_ID = 400,
     NON_PASSDOOR_ID = 401,
     CARPET_ID = 402,
     CARPET_PIECE_ID = 403,
     CARPET_TYPE = 404,
     MON_APPEARANCE = 405,
     ROOMDOOR_ID = 406,
     IF_ID = 407,
     ELSE_ID = 408,
     TERRAIN_ID = 409,
     HORIZ_OR_VERT = 410,
     REPLACE_TERRAIN_ID = 411,
     LOCATION_SUBTYPE_ID = 412,
     DOOR_SUBTYPE = 413,
     BRAZIER_SUBTYPE = 414,
     SIGNPOST_SUBTYPE = 415,
     TREE_SUBTYPE = 416,
     FOREST_ID = 417,
     FOREST_TYPE = 418,
     INITIALIZE_TYPE = 419,
     EXIT_ID = 420,
     SHUFFLE_ID = 421,
     MANUAL_TYPE_ID = 422,
     MANUAL_TYPE = 423,
     QUANTITY_ID = 424,
     BURIED_ID = 425,
     LOOP_ID = 426,
     FOR_ID = 427,
     TO_ID = 428,
     SWITCH_ID = 429,
     CASE_ID = 430,
     BREAK_ID = 431,
     DEFAULT_ID = 432,
     ERODED_ID = 433,
     TRAPPED_STATE = 434,
     RECHARGED_ID = 435,
     INVIS_ID = 436,
     GREASED_ID = 437,
     INDESTRUCTIBLE_ID = 438,
     FEMALE_ID = 439,
     MALE_ID = 440,
     WAITFORU_ID = 441,
     PROTECTOR_ID = 442,
     CANCELLED_ID = 443,
     REVIVED_ID = 444,
     AVENGE_ID = 445,
     FLEEING_ID = 446,
     BLINDED_ID = 447,
     MAXHP_ID = 448,
     LEVEL_ADJUSTMENT_ID = 449,
     KEEP_ORIGINAL_INVENTORY_ID = 450,
     PARALYZED_ID = 451,
     STUNNED_ID = 452,
     CONFUSED_ID = 453,
     SEENTRAPS_ID = 454,
     ALL_ID = 455,
     MONTYPE_ID = 456,
     OBJTYPE_ID = 457,
     TERTYPE_ID = 458,
     TERTYPE2_ID = 459,
     LEVER_EFFECT_TYPE = 460,
     SWITCHABLE_ID = 461,
     CONTINUOUSLY_USABLE_ID = 462,
     TARGET_ID = 463,
     TRAPTYPE_ID = 464,
     EFFECT_FLAG_ID = 465,
     GRAVE_ID = 466,
     BRAZIER_ID = 467,
     SIGNPOST_ID = 468,
     TREE_ID = 469,
     ERODEPROOF_ID = 470,
     FUNCTION_ID = 471,
     MSG_OUTPUT_TYPE = 472,
     COMPARE_TYPE = 473,
     UNKNOWN_TYPE = 474,
     rect_ID = 475,
     fillrect_ID = 476,
     line_ID = 477,
     randline_ID = 478,
     grow_ID = 479,
     selection_ID = 480,
     flood_ID = 481,
     rndcoord_ID = 482,
     circle_ID = 483,
     ellipse_ID = 484,
     filter_ID = 485,
     complement_ID = 486,
     gradient_ID = 487,
     GRADIENT_TYPE = 488,
     LIMITED = 489,
     HUMIDITY_TYPE = 490,
     STRING = 491,
     MAP_ID = 492,
     NQSTRING = 493,
     VARSTRING = 494,
     CFUNC = 495,
     CFUNC_INT = 496,
     CFUNC_STR = 497,
     CFUNC_COORD = 498,
     CFUNC_REGION = 499,
     VARSTRING_INT = 500,
     VARSTRING_INT_ARRAY = 501,
     VARSTRING_STRING = 502,
     VARSTRING_STRING_ARRAY = 503,
     VARSTRING_VAR = 504,
     VARSTRING_VAR_ARRAY = 505,
     VARSTRING_COORD = 506,
     VARSTRING_COORD_ARRAY = 507,
     VARSTRING_REGION = 508,
     VARSTRING_REGION_ARRAY = 509,
     VARSTRING_MAPCHAR = 510,
     VARSTRING_MAPCHAR_ARRAY = 511,
     VARSTRING_MONST = 512,
     VARSTRING_MONST_ARRAY = 513,
     VARSTRING_OBJ = 514,
     VARSTRING_OBJ_ARRAY = 515,
     VARSTRING_SEL = 516,
     VARSTRING_SEL_ARRAY = 517,
     METHOD_INT = 518,
     METHOD_INT_ARRAY = 519,
     METHOD_STRING = 520,
     METHOD_STRING_ARRAY = 521,
     METHOD_VAR = 522,
     METHOD_VAR_ARRAY = 523,
     METHOD_COORD = 524,
     METHOD_COORD_ARRAY = 525,
     METHOD_REGION = 526,
     METHOD_REGION_ARRAY = 527,
     METHOD_MAPCHAR = 528,
     METHOD_MAPCHAR_ARRAY = 529,
     METHOD_MONST = 530,
     METHOD_MONST_ARRAY = 531,
     METHOD_OBJ = 532,
     METHOD_OBJ_ARRAY = 533,
     METHOD_SEL = 534,
     METHOD_SEL_ARRAY = 535,
     DICE = 536
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 153 "lev_comp.y"

    int64_t    i;
    char    *map;
    struct {
        int64_t room;
        int64_t wall;
        int64_t door;
    } corpos;
    struct {
        int64_t area;
        int64_t x1;
        int64_t y1;
        int64_t x2;
        int64_t y2;
    } lregn;
    struct {
        int64_t x;
        int64_t y;
    } crd;
    struct {
        int64_t ter;
        int64_t lit;
    } terr;
    struct {
        int64_t height;
        int64_t width;
    } sze;
    struct {
        int64_t die;
        int64_t num;
    } dice;
    struct {
        int64_t cfunc;
        char *varstr;
    } meth;



/* Line 214 of yacc.c  */
#line 581 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 593 "lev_comp.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1393

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  299
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  194
/* YYNRULES -- Number of rules.  */
#define YYNRULES  544
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1231

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   536

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   294,   298,     2,
     238,   239,   292,   290,   236,   291,   296,   293,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   237,     2,
       2,   295,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   240,     2,   241,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   242,   297,   243,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      33,    39,    45,    49,    66,    70,    76,    80,    86,    90,
      91,    94,    95,    98,    99,   102,   104,   106,   107,   111,
     115,   117,   118,   121,   125,   127,   129,   131,   133,   135,
     137,   139,   141,   143,   145,   147,   149,   151,   153,   155,
     157,   159,   161,   163,   165,   167,   169,   171,   173,   175,
     177,   179,   181,   183,   185,   187,   189,   191,   193,   195,
     197,   199,   201,   203,   205,   207,   209,   211,   213,   215,
     217,   219,   221,   223,   225,   227,   229,   231,   233,   235,
     237,   239,   241,   243,   245,   247,   249,   251,   253,   255,
     257,   259,   261,   263,   265,   267,   269,   271,   273,   275,
     277,   279,   281,   283,   285,   287,   289,   291,   293,   295,
     297,   299,   301,   303,   307,   311,   317,   321,   327,   333,
     339,   343,   347,   353,   359,   365,   373,   381,   389,   395,
     397,   401,   403,   407,   409,   413,   415,   419,   421,   425,
     427,   431,   433,   437,   438,   439,   448,   453,   455,   456,
     458,   460,   466,   470,   471,   472,   482,   483,   486,   487,
     493,   494,   499,   501,   504,   506,   513,   514,   518,   519,
     526,   527,   532,   533,   538,   540,   541,   546,   550,   562,
     580,   582,   586,   590,   596,   602,   610,   615,   616,   632,
     633,   651,   652,   655,   661,   663,   669,   671,   677,   679,
     685,   687,   698,   705,   707,   709,   711,   713,   715,   719,
     721,   723,   724,   728,   732,   736,   740,   742,   744,   746,
     748,   750,   758,   764,   766,   768,   770,   772,   776,   777,
     783,   788,   789,   793,   795,   797,   799,   801,   804,   806,
     808,   810,   812,   814,   818,   820,   824,   828,   832,   834,
     836,   840,   842,   844,   846,   848,   852,   854,   856,   858,
     860,   862,   864,   868,   872,   873,   879,   882,   883,   887,
     889,   893,   895,   899,   903,   905,   907,   911,   913,   915,
     917,   921,   923,   925,   927,   931,   935,   939,   943,   947,
     951,   955,   959,   961,   963,   965,   967,   971,   975,   979,
     983,   989,   997,  1003,  1012,  1014,  1018,  1022,  1028,  1034,
    1042,  1050,  1057,  1063,  1064,  1067,  1073,  1077,  1089,  1103,
    1113,  1120,  1121,  1125,  1127,  1131,  1135,  1139,  1143,  1145,
    1147,  1151,  1155,  1159,  1163,  1167,  1171,  1175,  1177,  1179,
    1181,  1183,  1187,  1191,  1193,  1199,  1209,  1215,  1223,  1227,
    1231,  1237,  1247,  1253,  1259,  1265,  1266,  1282,  1283,  1285,
    1293,  1305,  1315,  1329,  1333,  1341,  1353,  1367,  1377,  1387,
    1397,  1405,  1413,  1419,  1427,  1433,  1443,  1451,  1457,  1467,
    1473,  1479,  1483,  1491,  1495,  1503,  1511,  1515,  1521,  1527,
    1531,  1537,  1545,  1555,  1557,  1559,  1561,  1563,  1565,  1566,
    1569,  1571,  1575,  1577,  1579,  1581,  1582,  1586,  1588,  1589,
    1593,  1595,  1596,  1600,  1601,  1605,  1606,  1610,  1612,  1614,
    1616,  1618,  1620,  1622,  1624,  1626,  1628,  1632,  1634,  1636,
    1641,  1643,  1645,  1650,  1652,  1654,  1659,  1661,  1666,  1672,
    1674,  1678,  1680,  1684,  1686,  1688,  1693,  1703,  1705,  1707,
    1712,  1714,  1720,  1722,  1724,  1729,  1731,  1733,  1739,  1741,
    1743,  1745,  1750,  1752,  1754,  1760,  1762,  1764,  1766,  1770,
    1772,  1774,  1778,  1780,  1785,  1789,  1793,  1797,  1801,  1805,
    1809,  1811,  1813,  1817,  1819,  1823,  1824,  1826,  1828,  1830,
    1832,  1836,  1837,  1839,  1841,  1844,  1847,  1852,  1859,  1864,
    1871,  1878,  1885,  1892,  1895,  1902,  1911,  1920,  1931,  1946,
    1949,  1951,  1955,  1957,  1961,  1963,  1965,  1967,  1969,  1971,
    1973,  1975,  1977,  1979,  1981,  1983,  1985,  1987,  1989,  1991,
    1993,  1995,  1997,  1999,  2010
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     300,     0,    -1,    -1,   301,    -1,   302,    -1,   302,   301,
      -1,   303,   313,   315,    -1,    21,   237,   244,    -1,    20,
     237,   244,   236,     3,    -1,    20,   237,   244,   236,     3,
     236,   454,    -1,    22,   237,    11,   236,   414,    -1,    22,
     237,    10,   236,     3,    -1,    22,   237,    13,    -1,    22,
     237,    12,   236,     3,   236,     3,   236,     5,   236,     5,
     236,   454,   236,   312,   311,    -1,    23,   237,   484,    -1,
     162,   237,   163,   236,   164,    -1,   162,   237,   163,    -1,
      36,   237,     4,   236,   325,    -1,    26,   237,   111,    -1,
      -1,   236,   234,    -1,    -1,   236,   461,    -1,    -1,   236,
       3,    -1,     5,    -1,    89,    -1,    -1,   141,   237,   314,
      -1,   142,   236,   314,    -1,   142,    -1,    -1,   317,   315,
      -1,   242,   315,   243,    -1,   358,    -1,   304,    -1,   305,
      -1,   422,    -1,   308,    -1,   306,    -1,   430,    -1,   431,
      -1,   432,    -1,   307,    -1,   429,    -1,   428,    -1,   426,
      -1,   427,    -1,   433,    -1,   434,    -1,   435,    -1,   436,
      -1,   437,    -1,   402,    -1,   360,    -1,   323,    -1,   322,
      -1,   417,    -1,   372,    -1,   394,    -1,   439,    -1,   440,
      -1,   404,    -1,   405,    -1,   407,    -1,   406,    -1,   438,
      -1,   338,    -1,   348,    -1,   350,    -1,   354,    -1,   352,
      -1,   335,    -1,   345,    -1,   331,    -1,   334,    -1,   397,
      -1,   408,    -1,   379,    -1,   395,    -1,   382,    -1,   388,
      -1,   418,    -1,   413,    -1,   400,    -1,   359,    -1,   419,
      -1,   420,    -1,   421,    -1,   423,    -1,   365,    -1,   363,
      -1,   412,    -1,   416,    -1,   415,    -1,   398,    -1,   399,
      -1,   401,    -1,   393,    -1,   396,    -1,   254,    -1,   256,
      -1,   258,    -1,   260,    -1,   262,    -1,   264,    -1,   266,
      -1,   268,    -1,   270,    -1,   253,    -1,   255,    -1,   257,
      -1,   259,    -1,   261,    -1,   263,    -1,   265,    -1,   267,
      -1,   269,    -1,   318,    -1,   319,    -1,   247,    -1,   247,
      -1,   319,    -1,   166,   237,   318,    -1,   320,   295,   473,
      -1,   320,   295,   225,   237,   482,    -1,   320,   295,   472,
      -1,   320,   295,   489,   237,   466,    -1,   320,   295,   488,
     237,   468,    -1,   320,   295,   487,   237,   470,    -1,   320,
     295,   461,    -1,   320,   295,   464,    -1,   320,   295,   242,
     329,   243,    -1,   320,   295,   242,   328,   243,    -1,   320,
     295,   242,   327,   243,    -1,   320,   295,   489,   237,   242,
     326,   243,    -1,   320,   295,   488,   237,   242,   325,   243,
      -1,   320,   295,   487,   237,   242,   324,   243,    -1,   320,
     295,   242,   330,   243,    -1,   471,    -1,   324,   236,   471,
      -1,   469,    -1,   325,   236,   469,    -1,   467,    -1,   326,
     236,   467,    -1,   465,    -1,   327,   236,   465,    -1,   462,
      -1,   328,   236,   462,    -1,   473,    -1,   329,   236,   473,
      -1,   472,    -1,   330,   236,   472,    -1,    -1,    -1,   216,
     246,   238,   332,   477,   239,   333,   316,    -1,   246,   238,
     480,   239,    -1,   165,    -1,    -1,     6,    -1,     6,    -1,
     240,   473,   218,   473,   241,    -1,   240,   473,   241,    -1,
      -1,    -1,   174,   339,   240,   460,   241,   340,   242,   341,
     243,    -1,    -1,   342,   341,    -1,    -1,   175,   485,   237,
     343,   315,    -1,    -1,   177,   237,   344,   315,    -1,   176,
      -1,   296,   296,    -1,   173,    -1,   172,   321,   295,   473,
     346,   473,    -1,    -1,   347,   349,   316,    -1,    -1,   171,
     240,   460,   241,   351,   316,    -1,    -1,   337,   237,   353,
     317,    -1,    -1,   152,   337,   355,   356,    -1,   316,    -1,
      -1,   316,   357,   153,   316,    -1,    14,   237,   472,    -1,
      14,   237,   472,   236,    15,   236,    16,   236,    17,   236,
       4,    -1,    14,   237,   472,   236,    15,   236,    16,   236,
      17,   236,    18,   236,    19,   236,     4,   236,     4,    -1,
      79,    -1,    79,   237,   485,    -1,    79,   237,    89,    -1,
      66,   237,   361,   236,   361,    -1,    66,   237,   361,   236,
     485,    -1,   238,     4,   236,    88,   236,   376,   239,    -1,
     442,   336,   236,   454,    -1,    -1,   139,   237,   362,   236,
     369,   236,   371,   443,   446,   448,   451,   452,   450,   364,
     316,    -1,    -1,    58,   237,   362,   236,   368,   236,   370,
     236,   371,   443,   446,   448,   451,   452,   450,   366,   316,
      -1,    -1,   236,     5,    -1,   238,     4,   236,     4,   239,
      -1,    89,    -1,   238,     4,   236,     4,   239,    -1,    89,
      -1,   238,   380,   236,   381,   239,    -1,    89,    -1,   238,
       4,   236,     4,   239,    -1,    89,    -1,   151,   237,   373,
     236,   453,   236,   374,   236,   376,   377,    -1,    34,   237,
     453,   236,   482,   377,    -1,     5,    -1,    89,    -1,   375,
      -1,    89,    -1,    88,    -1,    88,   297,   375,    -1,     4,
      -1,    89,    -1,    -1,   377,   236,   378,    -1,   157,   237,
     158,    -1,    85,   237,   470,    -1,   134,   237,     4,    -1,
     183,    -1,   125,    -1,   126,    -1,   146,    -1,    25,    -1,
      24,   237,   380,   236,   381,   367,   245,    -1,    24,   237,
     461,   367,   245,    -1,    93,    -1,    94,    -1,    95,    -1,
      94,    -1,    32,   237,   384,    -1,    -1,    32,   237,   384,
     383,   316,    -1,   468,   236,   461,   385,    -1,    -1,   385,
     236,   386,    -1,   472,    -1,   143,    -1,   144,    -1,   456,
      -1,   150,   472,    -1,   184,    -1,   185,    -1,   181,    -1,
     188,    -1,   189,    -1,   189,   237,   460,    -1,   190,    -1,
     191,   237,   460,    -1,   192,   237,   460,    -1,   196,   237,
     460,    -1,   197,    -1,   198,    -1,   199,   237,   387,    -1,
     199,    -1,   186,    -1,   187,    -1,   193,    -1,   194,   237,
     460,    -1,   195,    -1,   136,    -1,   137,    -1,   138,    -1,
     244,    -1,   200,    -1,   244,   297,   387,    -1,    30,   237,
     390,    -1,    -1,    31,   237,   390,   389,   316,    -1,   470,
     391,    -1,    -1,   391,   236,   392,    -1,    82,    -1,   201,
     237,   468,    -1,   486,    -1,   140,   237,   472,    -1,   169,
     237,   460,    -1,   170,    -1,    81,    -1,   178,   237,   460,
      -1,   215,    -1,    80,    -1,   179,    -1,   180,   237,   460,
      -1,   181,    -1,   182,    -1,   461,    -1,   123,   237,   120,
      -1,   122,   237,   121,    -1,   124,   237,   460,    -1,   133,
     237,   460,    -1,   134,   237,   460,    -1,   167,   237,   168,
      -1,   135,   237,   460,    -1,    85,   237,   470,    -1,   183,
      -1,   126,    -1,    87,    -1,    83,    -1,   129,   237,   127,
      -1,   130,   237,   128,    -1,    65,   237,   460,    -1,   131,
     237,   132,    -1,    33,   237,   441,   236,   461,    -1,    35,
     237,   461,   236,    88,   236,   453,    -1,    40,   237,   461,
     236,    88,    -1,    40,   237,   461,   236,    88,   236,     5,
     311,    -1,    41,    -1,    41,   237,   482,    -1,    41,   237,
     200,    -1,    54,   237,   461,   236,    98,    -1,    55,   237,
     461,   236,    98,    -1,    55,   237,   491,   236,   491,   236,
      98,    -1,    60,   237,   491,   236,   491,   236,   244,    -1,
      61,   237,   491,   236,   491,   403,    -1,    62,   237,   491,
     236,   491,    -1,    -1,   236,    98,    -1,    69,   237,   482,
     236,   105,    -1,    70,   237,   482,    -1,    72,   237,   461,
     236,   208,   237,   461,   236,   103,   236,    99,    -1,    72,
     237,   461,   236,    73,   236,    74,   236,    75,   236,   103,
     236,    99,    -1,    71,   237,   461,   236,   101,   236,   102,
     236,    99,    -1,    86,   237,   461,   236,   205,   409,    -1,
      -1,   409,   236,   410,    -1,    99,    -1,   201,   237,   468,
      -1,   202,   237,   470,    -1,   203,   237,   414,    -1,   204,
     237,   414,    -1,   206,    -1,   207,    -1,   208,   237,   461,
      -1,   209,   237,   441,    -1,   157,   237,   411,    -1,   109,
     237,   449,    -1,   210,   237,     4,    -1,   134,   237,     4,
      -1,   100,   237,   101,    -1,     4,    -1,   103,    -1,   158,
      -1,   105,    -1,    77,   237,   482,    -1,    76,   237,   482,
      -1,     3,    -1,   238,     3,   236,   454,   239,    -1,   156,
     237,   464,   236,   466,   236,   466,   236,     7,    -1,   154,
     237,   482,   236,   466,    -1,   154,   237,   482,   236,   466,
     236,   108,    -1,    56,   237,   464,    -1,    57,   237,   464,
      -1,    46,   237,   244,   236,    47,    -1,    46,   237,   244,
     236,    47,   236,   201,   237,   468,    -1,    43,   237,   464,
     236,    45,    -1,    44,   237,   491,   236,    45,    -1,    27,
     237,   482,   236,   107,    -1,    -1,    42,   237,   464,   236,
     454,   236,   442,   443,   446,   448,   451,   452,   450,   424,
     425,    -1,    -1,   316,    -1,    51,   237,   461,   236,   455,
     236,   457,    -1,    51,   237,   461,   236,   455,   236,   457,
     236,   201,   237,   468,    -1,    51,   237,   461,   236,   455,
     236,   457,   236,    97,    -1,    51,   237,   461,   236,   455,
     236,   457,   236,    97,   236,   201,   237,   468,    -1,    52,
     237,   461,    -1,    52,   237,   461,   236,   201,   237,   468,
      -1,   113,   237,   461,   236,   114,   236,     4,   236,   115,
     236,   116,    -1,   113,   237,   461,   236,   114,   236,     4,
     236,   115,   236,   116,   236,   454,    -1,   113,   237,   461,
     236,   114,   236,   117,   236,   115,    -1,   113,   237,   461,
     236,   114,   236,   118,   236,   115,    -1,   113,   237,   461,
     236,   114,   236,   119,   236,   115,    -1,   113,   237,   461,
     236,   114,   236,   115,    -1,   110,   237,   482,   236,   111,
     236,   108,    -1,   145,   237,   482,   236,     4,    -1,   145,
     237,   482,   236,   111,   236,   108,    -1,   147,   237,   464,
     236,   149,    -1,   148,   237,   482,   236,   149,   236,     4,
     236,     4,    -1,   148,   237,   482,   236,   149,   236,     4,
      -1,    53,   237,   104,   236,   461,    -1,    53,   237,   104,
     236,   461,   236,   201,   237,   468,    -1,   211,   237,   461,
     236,   472,    -1,   211,   237,   461,   236,    89,    -1,   211,
     237,   461,    -1,   212,   237,   461,   236,   159,   236,   454,
      -1,   212,   237,   461,    -1,   213,   237,   461,   236,   160,
     236,   472,    -1,   213,   237,   461,   236,   160,   236,    89,
      -1,   213,   237,   461,    -1,   214,   237,   461,   236,   161,
      -1,   214,   237,   461,   236,   163,    -1,   214,   237,   461,
      -1,    67,   237,   473,   236,   461,    -1,    68,   237,   461,
     236,   490,   236,   472,    -1,    64,   237,   460,   236,   460,
     236,   460,   236,   460,    -1,    64,    -1,   244,    -1,    89,
      -1,   244,    -1,    89,    -1,    -1,   236,   444,    -1,   445,
      -1,   445,   236,   444,    -1,    48,    -1,    49,    -1,    50,
      -1,    -1,   112,   237,   447,    -1,   111,    -1,    -1,   109,
     237,   449,    -1,   108,    -1,    -1,   201,   237,   468,    -1,
      -1,    28,   237,   107,    -1,    -1,    29,   237,     4,    -1,
      80,    -1,    89,    -1,    81,    -1,    89,    -1,    92,    -1,
     458,    -1,    89,    -1,    92,    -1,   458,    -1,    91,   237,
      89,    -1,    96,    -1,    89,    -1,    91,   240,     4,   241,
      -1,   244,    -1,   255,    -1,   256,   240,   473,   241,    -1,
     473,    -1,   462,    -1,   227,   238,   482,   239,    -1,   259,
      -1,   260,   240,   473,   241,    -1,   238,     4,   236,     4,
     239,    -1,    89,    -1,    90,   463,   241,    -1,   235,    -1,
     235,   236,   463,    -1,   465,    -1,   261,    -1,   262,   240,
     473,   241,    -1,   238,     4,   236,     4,   236,     4,   236,
       4,   239,    -1,   467,    -1,   263,    -1,   264,   240,   473,
     241,    -1,     3,    -1,   238,     3,   236,   454,   239,    -1,
     469,    -1,   265,    -1,   266,   240,   473,   241,    -1,   244,
      -1,     3,    -1,   238,     3,   236,   244,   239,    -1,    89,
      -1,   471,    -1,   267,    -1,   268,   240,   473,   241,    -1,
     244,    -1,     3,    -1,   238,     3,   236,   244,   239,    -1,
      89,    -1,   106,    -1,   459,    -1,   472,   296,   459,    -1,
       4,    -1,   483,    -1,   238,     8,   239,    -1,   253,    -1,
     254,   240,   473,   241,    -1,   473,   290,   473,    -1,   473,
     291,   473,    -1,   473,   292,   473,    -1,   473,   293,   473,
      -1,   473,   294,   473,    -1,   238,   473,   239,    -1,   249,
      -1,   250,    -1,   320,   237,   474,    -1,   475,    -1,   476,
     236,   475,    -1,    -1,   476,    -1,   473,    -1,   472,    -1,
     478,    -1,   479,   236,   478,    -1,    -1,   479,    -1,   461,
      -1,   220,   464,    -1,   221,   464,    -1,   222,   461,   236,
     461,    -1,   223,   461,   236,   461,   236,   473,    -1,   224,
     238,   482,   239,    -1,   224,   238,   375,   236,   482,   239,
      -1,   230,   238,     7,   236,   482,   239,    -1,   230,   238,
     482,   236,   482,   239,    -1,   230,   238,   466,   236,   482,
     239,    -1,   226,   461,    -1,   228,   238,   461,   236,   473,
     239,    -1,   228,   238,   461,   236,   473,   236,    48,   239,
      -1,   229,   238,   461,   236,   473,   236,   473,   239,    -1,
     229,   238,   461,   236,   473,   236,   473,   236,    48,   239,
      -1,   232,   238,   233,   236,   238,   473,   291,   473,   309,
     239,   236,   461,   310,   239,    -1,   231,   481,    -1,   269,
      -1,   238,   482,   239,    -1,   481,    -1,   481,   298,   482,
      -1,   289,    -1,   107,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,     8,    -1,     9,    -1,     4,    -1,   483,
      -1,    37,    -1,    30,    -1,    38,    -1,    32,    -1,    39,
      -1,   154,    -1,    84,    -1,    89,    -1,   492,    -1,    63,
     238,     4,   236,     4,   236,     4,   236,     4,   239,    -1,
     238,     4,   236,     4,   236,     4,   236,     4,   239,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   287,   287,   288,   291,   292,   295,   318,   323,   343,
     366,   379,   391,   397,   426,   432,   436,   442,   448,   455,
     458,   465,   469,   476,   479,   486,   487,   491,   494,   501,
     505,   512,   515,   521,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   606,
     607,   608,   609,   610,   611,   612,   613,   614,   617,   618,
     619,   622,   623,   626,   642,   648,   654,   660,   666,   672,
     678,   684,   690,   700,   710,   720,   730,   740,   750,   762,
     767,   774,   779,   786,   791,   798,   802,   808,   813,   820,
     824,   830,   834,   841,   863,   840,   877,   932,   939,   942,
     948,   955,   959,   968,   972,   967,  1035,  1036,  1040,  1039,
    1053,  1052,  1067,  1077,  1078,  1081,  1119,  1118,  1153,  1152,
    1183,  1182,  1215,  1214,  1240,  1251,  1250,  1278,  1282,  1286,
    1292,  1297,  1302,  1309,  1316,  1325,  1333,  1345,  1344,  1370,
    1369,  1395,  1398,  1404,  1414,  1420,  1429,  1435,  1440,  1446,
    1451,  1457,  1468,  1474,  1475,  1478,  1479,  1482,  1486,  1492,
    1493,  1497,  1503,  1511,  1516,  1521,  1526,  1531,  1536,  1541,
    1549,  1556,  1564,  1572,  1573,  1576,  1577,  1580,  1585,  1584,
    1598,  1605,  1612,  1620,  1625,  1631,  1637,  1643,  1649,  1654,
    1659,  1664,  1669,  1674,  1679,  1684,  1689,  1694,  1699,  1704,
    1709,  1715,  1721,  1726,  1731,  1736,  1741,  1746,  1751,  1756,
    1763,  1772,  1776,  1789,  1798,  1797,  1815,  1825,  1831,  1839,
    1845,  1850,  1855,  1860,  1865,  1870,  1875,  1880,  1885,  1899,
    1905,  1910,  1915,  1920,  1925,  1930,  1935,  1940,  1945,  1950,
    1955,  1960,  1965,  1970,  1975,  1980,  1986,  1991,  1996,  2001,
    2008,  2014,  2043,  2048,  2056,  2062,  2066,  2074,  2081,  2088,
    2098,  2108,  2124,  2135,  2138,  2144,  2150,  2156,  2160,  2166,
    2172,  2179,  2185,  2193,  2199,  2204,  2209,  2214,  2219,  2225,
    2231,  2236,  2241,  2246,  2251,  2256,  2261,  2268,  2268,  2268,
    2268,  2271,  2277,  2283,  2288,  2295,  2302,  2306,  2312,  2318,
    2324,  2329,  2336,  2342,  2352,  2359,  2358,  2392,  2395,  2401,
    2406,  2411,  2416,  2422,  2426,  2432,  2436,  2440,  2444,  2448,
    2452,  2458,  2464,  2468,  2474,  2480,  2484,  2490,  2494,  2500,
    2504,  2509,  2516,  2520,  2527,  2531,  2536,  2543,  2547,  2552,
    2560,  2566,  2573,  2577,  2584,  2592,  2595,  2605,  2609,  2612,
    2618,  2622,  2629,  2633,  2637,  2644,  2647,  2653,  2660,  2663,
    2669,  2676,  2680,  2687,  2690,  2697,  2700,  2706,  2707,  2710,
    2711,  2714,  2715,  2716,  2722,  2723,  2724,  2730,  2731,  2734,
    2743,  2748,  2755,  2766,  2772,  2776,  2780,  2787,  2797,  2804,
    2808,  2814,  2818,  2826,  2830,  2837,  2847,  2860,  2864,  2871,
    2881,  2890,  2901,  2905,  2912,  2922,  2933,  2942,  2952,  2958,
    2962,  2969,  2979,  2990,  2999,  3009,  3013,  3020,  3021,  3027,
    3031,  3035,  3039,  3047,  3056,  3060,  3064,  3068,  3072,  3076,
    3079,  3086,  3095,  3128,  3129,  3132,  3133,  3136,  3140,  3147,
    3154,  3165,  3168,  3176,  3180,  3184,  3188,  3192,  3197,  3201,
    3205,  3210,  3215,  3220,  3224,  3229,  3234,  3238,  3242,  3247,
    3251,  3258,  3264,  3268,  3274,  3281,  3282,  3285,  3286,  3287,
    3290,  3294,  3298,  3302,  3308,  3309,  3312,  3313,  3316,  3317,
    3320,  3321,  3324,  3328,  3354
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHAR", "INTEGER", "BOOLEAN", "PERCENT",
  "SPERCENT", "MINUS_INTEGER", "PLUS_INTEGER", "MAZE_GRID_ID",
  "SOLID_FILL_ID", "MINES_ID", "ROGUELEV_ID", "MESSAGE_ID", "MESSAGE_TYPE",
  "MESSAGE_ATTR", "MESSAGE_COLOR", "MESSAGE_SOUND_TYPE",
  "MESSAGE_SOUND_ID", "MAZE_ID", "LEVEL_ID", "LEV_INIT_ID", "TILESET_ID",
  "GEOMETRY_ID", "NOMAP_ID", "BOUNDARY_TYPE_ID", "SPECIAL_TILESET_ID",
  "TILESET_PARAM_ID", "DECOTYP_ID", "OBJECT_ID", "COBJECT_ID",
  "MONSTER_ID", "TRAP_ID", "DOOR_ID", "DRAWBRIDGE_ID",
  "MONSTER_GENERATION_ID", "object_ID", "monster_ID", "terrain_ID",
  "MAZEWALK_ID", "WALLIFY_ID", "REGION_ID", "SPECIAL_REGION_ID",
  "SPECIAL_LEVREGION_ID", "SPECIAL_REGION_TYPE", "NAMING_ID",
  "NAMING_TYPE", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID", "ANVIL_ID",
  "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID", "NON_PASSWALL_ID",
  "ROOM_ID", "ARTIFACT_NAME_ID", "PORTAL_ID", "TELEPRT_ID", "BRANCH_ID",
  "LEV", "MINERALIZE_ID", "AGE_ID", "CORRIDOR_ID", "GOLD_ID",
  "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID", "MAGIC_PORTAL_ID",
  "MODRON_PORTAL_ID", "LEVEL_TELEPORTER_ID",
  "LEVEL_TELEPORT_DIRECTION_TYPE", "LEVEL_TELEPORT_END_TYPE", "POOL_ID",
  "SINK_ID", "NONE", "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE",
  "CURSE_TYPE", "MYTHIC_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID",
  "NO_PICKUP_ID", "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET",
  "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT",
  "ALTAR_TYPE", "ALTAR_SUBTYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE",
  "MAGIC_PORTAL_TARGET_TYPE_ID", "MAGIC_PORTAL_TARGET_TYPE",
  "SEEN_OR_UNSEEN", "MODRON_PORTAL_TYPE", "NPC_TYPE", "FOUNTAIN_TYPE",
  "SPECIAL_OBJECT_TYPE", "CMAP_TYPE", "FLOOR_SUBTYPE", "FLOOR_SUBTYPE_ID",
  "FLOOR_ID", "FLOOR_TYPE", "FLOOR_TYPE_ID", "DECORATION_ID",
  "DECORATION_TYPE", "DECORATION_DIR", "DECORATION_ITEM_STATE",
  "PAINTING_TYPE", "BANNER_TYPE", "WALL_SCULPTURE_TYPE",
  "ELEMENTAL_ENCHANTMENT_TYPE", "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID",
  "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID", "SECRET_DOOR_ID",
  "USES_UP_KEY_ID", "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE",
  "MYTHIC_PREFIX_ID", "MYTHIC_SUFFIX_ID", "MATERIAL_ID", "MATERIAL_TYPE",
  "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "LEVEL_BOSS_ID",
  "BOSS_HOSTILITY_ID", "HAS_BACKUP_ID", "SUBROOM_ID", "NAME_ID",
  "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS", "SUBTYPE_ID",
  "NON_PASSDOOR_ID", "CARPET_ID", "CARPET_PIECE_ID", "CARPET_TYPE",
  "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID", "TERRAIN_ID",
  "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID", "LOCATION_SUBTYPE_ID",
  "DOOR_SUBTYPE", "BRAZIER_SUBTYPE", "SIGNPOST_SUBTYPE", "TREE_SUBTYPE",
  "FOREST_ID", "FOREST_TYPE", "INITIALIZE_TYPE", "EXIT_ID", "SHUFFLE_ID",
  "MANUAL_TYPE_ID", "MANUAL_TYPE", "QUANTITY_ID", "BURIED_ID", "LOOP_ID",
  "FOR_ID", "TO_ID", "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID",
  "ERODED_ID", "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID",
  "INDESTRUCTIBLE_ID", "FEMALE_ID", "MALE_ID", "WAITFORU_ID",
  "PROTECTOR_ID", "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID",
  "BLINDED_ID", "MAXHP_ID", "LEVEL_ADJUSTMENT_ID",
  "KEEP_ORIGINAL_INVENTORY_ID", "PARALYZED_ID", "STUNNED_ID",
  "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID", "MONTYPE_ID", "OBJTYPE_ID",
  "TERTYPE_ID", "TERTYPE2_ID", "LEVER_EFFECT_TYPE", "SWITCHABLE_ID",
  "CONTINUOUSLY_USABLE_ID", "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID",
  "GRAVE_ID", "BRAZIER_ID", "SIGNPOST_ID", "TREE_ID", "ERODEPROOF_ID",
  "FUNCTION_ID", "MSG_OUTPUT_TYPE", "COMPARE_TYPE", "UNKNOWN_TYPE",
  "rect_ID", "fillrect_ID", "line_ID", "randline_ID", "grow_ID",
  "selection_ID", "flood_ID", "rndcoord_ID", "circle_ID", "ellipse_ID",
  "filter_ID", "complement_ID", "gradient_ID", "GRADIENT_TYPE", "LIMITED",
  "HUMIDITY_TYPE", "','", "':'", "'('", "')'", "'['", "']'", "'{'", "'}'",
  "STRING", "MAP_ID", "NQSTRING", "VARSTRING", "CFUNC", "CFUNC_INT",
  "CFUNC_STR", "CFUNC_COORD", "CFUNC_REGION", "VARSTRING_INT",
  "VARSTRING_INT_ARRAY", "VARSTRING_STRING", "VARSTRING_STRING_ARRAY",
  "VARSTRING_VAR", "VARSTRING_VAR_ARRAY", "VARSTRING_COORD",
  "VARSTRING_COORD_ARRAY", "VARSTRING_REGION", "VARSTRING_REGION_ARRAY",
  "VARSTRING_MAPCHAR", "VARSTRING_MAPCHAR_ARRAY", "VARSTRING_MONST",
  "VARSTRING_MONST_ARRAY", "VARSTRING_OBJ", "VARSTRING_OBJ_ARRAY",
  "VARSTRING_SEL", "VARSTRING_SEL_ARRAY", "METHOD_INT", "METHOD_INT_ARRAY",
  "METHOD_STRING", "METHOD_STRING_ARRAY", "METHOD_VAR", "METHOD_VAR_ARRAY",
  "METHOD_COORD", "METHOD_COORD_ARRAY", "METHOD_REGION",
  "METHOD_REGION_ARRAY", "METHOD_MAPCHAR", "METHOD_MAPCHAR_ARRAY",
  "METHOD_MONST", "METHOD_MONST_ARRAY", "METHOD_OBJ", "METHOD_OBJ_ARRAY",
  "METHOD_SEL", "METHOD_SEL_ARRAY", "DICE", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'='", "'.'", "'|'", "'&'", "$accept", "file", "levels", "level",
  "level_def", "lev_init", "tileset_detail", "forest_detail",
  "monster_generation_detail", "boundary_type_detail", "opt_limited",
  "opt_coord_or_var", "opt_fillchar", "walled", "flags", "flag_list",
  "levstatements", "stmt_block", "levstatement", "any_var_array",
  "any_var", "any_var_or_arr", "any_var_or_unk", "shuffle_detail",
  "variable_define", "encodeobj_list", "encodemonster_list",
  "mapchar_list", "encoderegion_list", "encodecoord_list", "integer_list",
  "string_list", "function_define", "$@1", "$@2", "function_call",
  "exitstatement", "opt_percent", "comparestmt", "switchstatement", "$@3",
  "$@4", "switchcases", "switchcase", "$@5", "$@6", "breakstatement",
  "for_to_span", "forstmt_start", "forstatement", "$@7", "loopstatement",
  "$@8", "chancestatement", "$@9", "ifstatement", "$@10", "if_ending",
  "$@11", "message", "random_corridors", "corridor", "corr_spec",
  "room_begin", "subroom_def", "$@12", "room_def", "$@13", "roomfill",
  "room_pos", "subroom_pos", "room_align", "room_size", "door_detail",
  "secret", "door_wall", "dir_list", "door_pos", "door_infos", "door_info",
  "map_definition", "h_justif", "v_justif", "monster_detail", "$@14",
  "monster_desc", "monster_infos", "monster_info", "seen_trap_mask",
  "object_detail", "$@15", "object_desc", "object_infos", "object_info",
  "trap_detail", "drawbridge_detail", "mazewalk_detail", "wallify_detail",
  "ladder_detail", "stair_detail", "stair_region", "portal_region",
  "teleprt_region", "branch_region", "teleprt_detail", "fountain_detail",
  "throne_detail", "modron_portal_detail", "magic_portal_detail",
  "lever_detail", "lever_infos", "lever_info", "valid_subtype",
  "sink_detail", "pool_detail", "terrain_type", "replace_terrain_detail",
  "terrain_detail", "diggable_detail", "passwall_detail", "naming_detail",
  "special_region_detail", "special_levregion_detail",
  "special_tileset_detail", "region_detail", "@16", "region_detail_end",
  "altar_detail", "anvil_detail", "decoration_detail", "floor_detail",
  "subtype_detail", "carpet_detail", "carpet_piece_detail", "npc_detail",
  "grave_detail", "brazier_detail", "signpost_detail", "tree_detail",
  "gold_detail", "engraving_detail", "mineralize", "trap_name",
  "room_type", "optroomregionflags", "roomregionflags", "roomregionflag",
  "optfloortype", "floortype", "optfloorsubtype", "floorsubtype",
  "optmontype", "opttileset", "optdecotyp", "door_state", "light_state",
  "alignment", "alignment_prfx", "altar_type", "a_register",
  "string_or_var", "integer_or_var", "coord_or_var", "encodecoord",
  "humidity_flags", "region_or_var", "encoderegion", "mapchar_or_var",
  "mapchar", "monster_or_var", "encodemonster", "object_or_var",
  "encodeobj", "string_expr", "math_expr_var", "func_param_type",
  "func_param_part", "func_param_list", "func_params_list",
  "func_call_param_part", "func_call_param_list", "func_call_params_list",
  "ter_selection_x", "ter_selection", "dice", "tileset_number",
  "all_integers", "all_ints_push", "objectid", "monsterid", "terrainid",
  "engraving_type", "lev_region", "region", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,    44,    58,    40,    41,
      91,    93,   123,   125,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
      43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   299,   300,   300,   301,   301,   302,   303,   303,   303,
     304,   304,   304,   304,   305,   306,   306,   307,   308,   309,
     309,   310,   310,   311,   311,   312,   312,   313,   313,   314,
     314,   315,   315,   316,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   320,   320,
     320,   321,   321,   322,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   324,
     324,   325,   325,   326,   326,   327,   327,   328,   328,   329,
     329,   330,   330,   332,   333,   331,   334,   335,   336,   336,
     337,   337,   337,   339,   340,   338,   341,   341,   343,   342,
     344,   342,   345,   346,   346,   347,   349,   348,   351,   350,
     353,   352,   355,   354,   356,   357,   356,   358,   358,   358,
     359,   359,   359,   360,   360,   361,   362,   364,   363,   366,
     365,   367,   367,   368,   368,   369,   369,   370,   370,   371,
     371,   372,   372,   373,   373,   374,   374,   375,   375,   376,
     376,   377,   377,   378,   378,   378,   378,   378,   378,   378,
     379,   379,   379,   380,   380,   381,   381,   382,   383,   382,
     384,   385,   385,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     387,   387,   387,   388,   389,   388,   390,   391,   391,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     392,   392,   392,   392,   392,   392,   392,   392,   392,   392,
     393,   394,   395,   395,   396,   396,   396,   397,   398,   399,
     400,   401,   402,   403,   403,   404,   405,   406,   406,   407,
     408,   409,   409,   410,   410,   410,   410,   410,   410,   410,
     410,   410,   410,   410,   410,   410,   410,   411,   411,   411,
     411,   412,   413,   414,   414,   415,   416,   416,   417,   418,
     419,   419,   420,   421,   422,   424,   423,   425,   425,   426,
     426,   426,   426,   427,   427,   428,   428,   428,   428,   428,
     428,   429,   430,   430,   431,   432,   432,   433,   433,   434,
     434,   434,   435,   435,   436,   436,   436,   437,   437,   437,
     438,   439,   440,   440,   441,   441,   442,   442,   443,   443,
     444,   444,   445,   445,   445,   446,   446,   447,   448,   448,
     449,   450,   450,   451,   451,   452,   452,   453,   453,   454,
     454,   455,   455,   455,   456,   456,   456,   457,   457,   458,
     459,   459,   459,   460,   461,   461,   461,   461,   462,   462,
     462,   463,   463,   464,   464,   464,   465,   466,   466,   466,
     467,   467,   468,   468,   468,   469,   469,   469,   469,   470,
     470,   470,   471,   471,   471,   471,   471,   472,   472,   473,
     473,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     474,   474,   475,   476,   476,   477,   477,   478,   478,   479,
     479,   480,   480,   481,   481,   481,   481,   481,   481,   481,
     481,   481,   481,   481,   481,   481,   481,   481,   481,   481,
     481,   481,   482,   482,   483,   484,   484,   485,   485,   485,
     486,   486,   486,   486,   487,   487,   488,   488,   489,   489,
     490,   490,   491,   491,   492
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     7,
       5,     5,     3,    16,     3,     5,     3,     5,     3,     0,
       2,     0,     2,     0,     2,     1,     1,     0,     3,     3,
       1,     0,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     5,     3,     5,     5,     5,
       3,     3,     5,     5,     5,     7,     7,     7,     5,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     0,     0,     8,     4,     1,     0,     1,
       1,     5,     3,     0,     0,     9,     0,     2,     0,     5,
       0,     4,     1,     2,     1,     6,     0,     3,     0,     6,
       0,     4,     0,     4,     1,     0,     4,     3,    11,    17,
       1,     3,     3,     5,     5,     7,     4,     0,    15,     0,
      17,     0,     2,     5,     1,     5,     1,     5,     1,     5,
       1,    10,     6,     1,     1,     1,     1,     1,     3,     1,
       1,     0,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     7,     5,     1,     1,     1,     1,     3,     0,     5,
       4,     0,     3,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     3,     1,     3,     3,     3,     1,     1,
       3,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     3,     3,     0,     5,     2,     0,     3,     1,
       3,     1,     3,     3,     1,     1,     3,     1,     1,     1,
       3,     1,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     3,     3,     3,     3,
       5,     7,     5,     8,     1,     3,     3,     5,     5,     7,
       7,     6,     5,     0,     2,     5,     3,    11,    13,     9,
       6,     0,     3,     1,     3,     3,     3,     3,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     3,     3,     1,     5,     9,     5,     7,     3,     3,
       5,     9,     5,     5,     5,     0,    15,     0,     1,     7,
      11,     9,    13,     3,     7,    11,    13,     9,     9,     9,
       7,     7,     5,     7,     5,     9,     7,     5,     9,     5,
       5,     3,     7,     3,     7,     7,     3,     5,     5,     3,
       5,     7,     9,     1,     1,     1,     1,     1,     0,     2,
       1,     3,     1,     1,     1,     0,     3,     1,     0,     3,
       1,     0,     3,     0,     3,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     4,
       1,     1,     4,     1,     1,     4,     1,     4,     5,     1,
       3,     1,     3,     1,     1,     4,     9,     1,     1,     4,
       1,     5,     1,     1,     4,     1,     1,     5,     1,     1,
       1,     4,     1,     1,     5,     1,     1,     1,     3,     1,
       1,     3,     1,     4,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     1,     3,     0,     1,     1,     1,     1,
       3,     0,     1,     1,     2,     2,     4,     6,     4,     6,
       6,     6,     6,     2,     6,     8,     8,    10,    14,     2,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    27,     0,     0,     1,
       5,     0,    31,     0,     7,     0,   160,     0,     0,     0,
       0,   230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   314,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   157,     0,     0,     0,   163,   172,     0,     0,     0,
       0,     0,     0,     0,   120,   109,   100,   110,   101,   111,
     102,   112,   103,   113,   104,   114,   105,   115,   106,   116,
     107,   117,   108,    35,    36,    39,    43,    38,     6,    31,
     118,   119,     0,    56,    55,    74,    75,    72,     0,    67,
      73,   176,    68,    69,    71,    70,    34,    85,    54,    91,
      90,    58,    78,    80,    81,    98,    59,    79,    99,    76,
      95,    96,    84,    97,    53,    62,    63,    65,    64,    77,
      92,    83,    94,    93,    57,    82,    86,    87,    88,    37,
      89,    46,    47,    45,    44,    40,    41,    42,    48,    49,
      50,    51,    52,    66,    60,    61,     0,    30,    28,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   182,     0,
       0,     0,     0,     0,   121,   122,     0,     0,     0,     0,
       0,     0,     0,   479,     0,   482,     0,   524,     0,   480,
     501,    32,     0,   180,     0,     8,     0,   440,   441,     0,
     477,   187,     0,     0,     0,    12,   526,   525,    14,   449,
       0,   233,   234,     0,     0,   446,     0,     0,   201,   444,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   520,   503,   522,     0,   473,   475,   476,
       0,   472,   470,     0,   273,   277,   469,   274,   466,   468,
       0,   465,   463,     0,   237,     0,   462,   405,   404,     0,
     427,   428,     0,     0,     0,     0,   316,   315,     0,   454,
       0,     0,   453,     0,     0,     0,     0,   542,     0,     0,
     373,     0,     0,     0,     0,     0,   358,   359,   407,   406,
       0,   158,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,   326,     0,     0,   352,   351,   529,   527,   528,
     192,   191,     0,     0,     0,     0,     0,     0,     0,   213,
     214,     0,     0,     0,     0,    16,   123,     0,     0,     0,
     391,   393,   396,   399,   153,     0,     0,     0,     0,   162,
       0,     0,     0,     0,     0,   498,   497,   499,   502,     0,
     535,   537,   534,   536,   538,   539,     0,     0,     0,   130,
     131,   126,   124,     0,     0,     0,     0,    31,   177,     0,
      29,     0,     0,     0,     0,     0,     0,   451,     0,     0,
       0,     0,     0,     0,     0,   504,   505,     0,     0,     0,
     513,     0,     0,     0,   519,     0,     0,     0,     0,     0,
       0,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   159,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   184,
     183,     0,     0,     0,   178,     0,     0,     0,     0,     0,
       0,   495,   481,   489,     0,     0,   484,   485,   486,   487,
     488,     0,   156,     0,   479,     0,     0,     0,     0,   147,
     145,   151,   149,     0,     0,     0,   181,     0,   429,   430,
       9,     0,     0,   478,    11,   353,     0,    10,     0,     0,
     450,     0,     0,     0,   236,   235,   201,   202,   232,     0,
       0,   217,     0,     0,     0,     0,   460,     0,     0,   458,
       0,     0,   457,     0,     0,   521,   523,   364,     0,     0,
       0,   275,     0,     0,   239,   241,   310,   221,     0,    17,
     141,   312,     0,     0,     0,   362,     0,     0,   363,   360,
     433,     0,   431,     0,   432,     0,   387,   317,     0,   318,
       0,   204,     0,     0,     0,     0,   323,   322,     0,     0,
     193,   194,   400,   540,   541,     0,   325,     0,     0,     0,
     331,     0,     0,   206,     0,     0,   382,     0,   384,     0,
       0,     0,     0,   356,     0,    15,     0,   174,     0,     0,
     164,   390,   389,     0,     0,   397,   398,     0,   493,   496,
       0,   483,   161,   500,   125,     0,     0,   134,     0,   133,
       0,   132,     0,   138,     0,   129,     0,   128,     0,   127,
      33,   442,     0,     0,     0,   452,   445,     0,   447,     0,
     506,     0,     0,     0,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   471,   532,   530,   531,     0,   288,
     285,   279,   305,     0,   304,     0,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   284,     0,
     289,     0,   291,   292,   302,     0,   287,   278,   293,   533,
     281,     0,   464,   240,   212,     0,     0,     0,     0,   455,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,   321,     0,     0,     0,     0,
       0,     0,   330,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,   173,   175,     0,     0,     0,     0,
       0,   154,     0,   146,   148,   150,   152,     0,   139,     0,
       0,   143,     0,     0,     0,   448,   231,     0,   218,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   467,     0,     0,   311,
     142,    23,     0,   408,     0,     0,     0,     0,   438,   437,
     369,   374,     0,   319,     0,   208,     0,     0,   320,   324,
       0,     0,   401,     0,     0,     0,     0,   381,     0,   380,
       0,     0,     0,     0,   210,     0,   408,   383,   386,   216,
       0,   215,   186,   357,     0,   166,   392,   395,   394,   490,
     491,   492,   494,     0,     0,   137,   136,     0,   135,     0,
       0,     0,   507,   509,     0,   514,     0,   510,     0,   459,
     512,   511,     0,   308,   301,   295,   294,   296,   306,   307,
     309,   297,   298,   300,   282,   299,   283,   286,   290,   280,
       0,   434,   267,   268,   269,   244,   245,     0,   250,   248,
     249,   262,   263,   251,   252,   254,     0,     0,   264,     0,
     266,     0,   258,   259,   261,   242,   246,   435,   243,     0,
     227,   228,     0,   229,     0,   226,   222,     0,   313,     0,
       0,   415,     0,     0,     0,   439,     0,     0,     0,     0,
       0,     0,   219,   220,     0,     0,     0,     0,   333,     0,
       0,     0,     0,     0,     0,     0,     0,   338,   339,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,   415,
       0,     0,     0,     0,     0,     0,   166,   155,   140,   144,
       0,   354,     0,     0,     0,   461,     0,     0,   247,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
     412,   413,   414,   409,   410,     0,   418,     0,     0,   361,
     371,     0,   388,   203,     0,   408,   402,   195,   329,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   377,   378,   379,   205,     0,   418,   385,
     221,   355,     0,   170,   165,   167,     0,     0,   515,     0,
     516,   485,   436,   253,   255,   256,   265,   257,   271,   270,
     260,   224,   225,   223,     0,     0,     0,     0,   423,     0,
       0,     0,     0,     0,   415,     0,     0,   346,   420,   343,
     345,   347,   348,   350,   349,   342,   334,   335,   336,   337,
     340,   341,   344,     0,     0,   423,   211,   168,    31,   188,
       0,     0,     0,     0,     0,     0,   456,   411,   417,   416,
       0,     0,   425,     0,   544,     0,   370,   207,   418,     0,
     327,   375,   209,   425,    31,   171,     0,     0,   517,    20,
       0,   272,   419,     0,     0,   421,   543,     0,   423,     0,
       0,   421,   169,     0,     0,     0,   424,     0,     0,   365,
     372,   425,   328,   376,   197,     0,     0,    21,   426,     0,
     367,   421,     0,     0,    25,    26,    23,     0,     0,   422,
     368,   366,   199,   198,     0,    13,    22,   518,     0,   189,
     200
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   103,   104,   105,   106,   107,
    1154,  1218,   978,  1216,    12,   178,   108,   428,   109,   110,
     111,   112,   236,   113,   114,   807,   599,   810,   535,   536,
     537,   538,   115,   521,   903,   116,   117,   489,   118,   119,
     237,   796,  1025,  1026,  1174,  1148,   120,   659,   121,   122,
     254,   123,   656,   124,   426,   125,   382,   510,   651,   126,
     127,   128,   358,   350,   129,  1212,   130,  1228,   444,   623,
     645,   867,   886,   131,   381,   890,   572,   994,   754,   976,
     132,   277,   566,   133,   465,   314,   753,   965,  1110,   134,
     462,   304,   461,   747,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   775,   145,   146,   147,   148,   149,
     782,  1012,  1135,   150,   151,   557,   152,   153,   154,   155,
     156,   157,   158,   159,   160,  1210,  1221,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   319,   351,   981,  1053,  1054,  1056,  1159,  1118,
    1129,  1199,  1162,  1185,   322,   550,   613,   966,   860,   614,
     260,   355,   294,   279,   438,   331,   332,   581,   582,   315,
     316,   305,   306,   405,   356,   901,   668,   669,   670,   407,
     408,   409,   295,   456,   249,   268,   371,   750,   423,   424,
     425,   635,   336,   337
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1095
static const yytype_int16 yypact[] =
{
     171,  -144,   -37,   109, -1095,   171,    70,   -16,    25, -1095,
   -1095,   -32,   861,    39, -1095,   155, -1095,    79,    98,   107,
     119, -1095,   130,   141,   143,   160,   166,   180,   182,   206,
     211,   214,   221,   224,   233,   235,   239,   242,   248,   249,
     252,   253,   255,   261,   264,   280,   283,   285,   287,   288,
     290,   298,   299,   302,   310,   312,   313,   314,   316,   317,
     322,   331,   332,   345,   346,   347,   354,    56,   364,   368,
     369, -1095,   373,   187,   501, -1095, -1095,   379,   383,   384,
     392,   324,    20,   149, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,   861,
   -1095, -1095,   165, -1095, -1095, -1095, -1095, -1095,   393, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095,   516,   297, -1095,   -17,
     567,    82,    -6,   522,   820,    92,    92,   108,   -42,    30,
     152,   630,   152,   760,   163,   163,   -15,   394,   152,   152,
     531,   152,   -26,   163,   163,   -40,   -15,   -15,   -15,    20,
     399,    20,   152,   820,   820,   152,   152,   820,   820,    72,
     152,   820,   152,   -40,   820,   163,   820,    74, -1095,   820,
     163,   476,   537,    20, -1095, -1095,   348,   401,   152,   152,
     152,   152,   406, -1095,    96, -1095,   405, -1095,   203, -1095,
      28, -1095,   398, -1095,   404,   413,   155, -1095, -1095,   415,
   -1095,  -146,   427,   428,   429, -1095, -1095, -1095, -1095, -1095,
     421, -1095, -1095,   424,   664, -1095,   430,   433,   435, -1095,
   -1095,   163,   163,   152,   152,   436,   152,   442,   448,   463,
     820,   466,   613, -1095, -1095,   407,   471, -1095, -1095, -1095,
     705, -1095, -1095,   475, -1095, -1095, -1095, -1095, -1095, -1095,
     718, -1095, -1095,   483,   482,   489, -1095, -1095, -1095,   490,
   -1095, -1095,   493,   497,   498,   499, -1095, -1095,   726, -1095,
     496,   502, -1095,   504,   503,   743,   514, -1095,   515,   517,
     519,   521,   523,   748,   527,   533, -1095, -1095, -1095, -1095,
     535,   759,   536,   538,   539,   540,   529,   763,   541,   238,
     547,   556, -1095,   564,   566, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095,   568,   570,   572,   578,   579,   580,   588, -1095,
   -1095,   589,   404,   590,   592,   593, -1095,   553,    20,    20,
     595,   610,   611,   621, -1095,   559,  -145,    20,    20, -1095,
      20,    20,    20,    20,    20,   563,   529, -1095,   622,   623,
   -1095, -1095, -1095, -1095, -1095, -1095,   624,   116,    24, -1095,
   -1095,   563,   529,   626,   627,   628,   861,   861, -1095,    17,
   -1095,    20,   845,   -17,   863,    22,   865,   633,   629,   820,
     635,    20,   162,   869,   631, -1095, -1095,   641,   642,   -44,
   -1095,   152,   152,   468, -1095,   646,   650,   820,   773,   645,
      20,   654,   404,   663,    20,   404,   152,   152,   820,   812,
     133,   818,   672,    20,    17,   866,   916,   688,   881,   887,
     292,   734,   152,   838,   703,   843,   -15,   -35, -1095,   706,
     -15,   -15,   -15,    20,   707,    66,   152,    55,   839,   844,
     -18,   741,   837,   835,   -28,    62,   801,   802,    30,   799,
   -1095,    69,    69,   789, -1095,   -45,   713,   -49,   796,   797,
      -9,   915, -1095, -1095,  -123,   296,   146,   146, -1095, -1095,
   -1095,    28, -1095,   820,   720,  -128,  -105,   -85,   -12, -1095,
   -1095,   563,   529,   124,    23,    64, -1095,   716, -1095, -1095,
   -1095,   304,   725, -1095, -1095, -1095,   959, -1095,   727,   421,
   -1095,   728,   960,   385, -1095, -1095,   435, -1095, -1095,   152,
     152,   668,   730,   729,   733,   736, -1095,   737,   558, -1095,
     735,   740, -1095,   742,   749, -1095, -1095, -1095,   750,   391,
     333, -1095,   751,   420, -1095, -1095, -1095, -1095,   757,   761,
   -1095,   765,   966,   426,   766, -1095,   767,   973, -1095,   768,
   -1095,   739, -1095,   769, -1095,   762,   771, -1095,   992, -1095,
     774, -1095,  1007,   780,    17,   782,   785, -1095,   786,   936,
   -1095, -1095, -1095, -1095, -1095,   792, -1095,   794,   795,   788,
   -1095,   798,   809, -1095,  1034,   819, -1095,   821, -1095,   823,
     824,   903,  1058,   826,   827, -1095,   404, -1095,   770,    20,
   -1095, -1095,   563,   828,   831, -1095, -1095,   832, -1095,   834,
     829, -1095, -1095, -1095, -1095,  1067,   840, -1095,   -39, -1095,
      20, -1095,   -17, -1095,   120, -1095,   133, -1095,    26, -1095,
   -1095, -1095,  1060,   846,  1078, -1095, -1095,   847, -1095,   842,
   -1095,   848,   995,   820, -1095,    20,    20,   820,   849,    20,
     820,   820,   850,   851, -1095, -1095, -1095, -1095,   854, -1095,
   -1095, -1095, -1095,   855, -1095,   856,   857,   858, -1095,   859,
     860,   867,   872,   873,   874,   876,   897,   898, -1095,   899,
   -1095,   900, -1095, -1095, -1095,   901, -1095, -1095, -1095, -1095,
   -1095,   864, -1095,   904,   905,    30,   133,  1097,   908, -1095,
     -40,  1135,   909,   963,  1159,   147,   108,   964,     1,  1068,
     950,   -24, -1095,   943,  1090, -1095,    20,   953,   -17,  1089,
    1118,   152,   957,  1086,   289,   961,   -20,  1087,  1192,   223,
     404,  1091,    69, -1095, -1095,   529,   956,    17,   -36,   150,
     915, -1095,   136, -1095, -1095,   529,   563,    16, -1095,    78,
      95, -1095,   965,    17,   967, -1095, -1095,    20, -1095,   968,
     190,   273,   969,    17,   452,   970,   971,    20, -1095,    20,
      92,  1079,  1082,    20,  1077,  1083,  1073,    20,    20,    20,
     -17,  1044,    20,    20,    20,   108, -1095,   962,   308, -1095,
   -1095,   977,  1210,   979,   980,  1215,   983,   981, -1095, -1095,
     985, -1095,   986, -1095,  1220, -1095,   338,   989, -1095, -1095,
     990,    73,   563,   991,   993,   994,  1033, -1095,   996, -1095,
     997,  1002,  1008,  1224, -1095,  1227,   979, -1095,  1009, -1095,
    1010, -1095, -1095, -1095,  1011,   219, -1095, -1095,   563, -1095,
   -1095, -1095, -1095,   404,   120, -1095, -1095,    26, -1095,  1231,
    1012,  1244,   529, -1095,  1202, -1095,    20, -1095,  1013, -1095,
   -1095, -1095,   562, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095,   563, -1095, -1095, -1095, -1095, -1095,
     111, -1095, -1095, -1095, -1095, -1095, -1095,   -17, -1095, -1095,
   -1095, -1095, -1095, -1095,  1016, -1095,  1017,  1018, -1095,  1019,
   -1095,  1020, -1095, -1095,  1021, -1095, -1095, -1095,   563,  1022,
   -1095, -1095,  1023, -1095,  1024, -1095, -1095,  1259, -1095,  1027,
     494,  1152,  1261,  1030,   108, -1095,    -8,   108,  1028,  1032,
     -20,    20, -1095, -1095,  1031,  1170,  1196,  1169, -1095,  1036,
    1037,  1038,  1039,  1040,  1041,  1042,  1043, -1095, -1095,  1045,
    1046,  1047, -1095,  1166,  1171,  1172,  1173,  1050,  1049,  1152,
    1286,    73,  1284,   156,  1055,  1051,   219, -1095, -1095, -1095,
    1057, -1095,  1059,  1061,   282, -1095,    20,  1207,   563,    20,
      20,    20,    20,    20,  -104,    92,  1293,  1140, -1095,  1295,
   -1095, -1095, -1095, -1095,  1065,  1066,  1193,  1069,  1300, -1095,
    1070,  1071, -1095, -1095,   162,   979, -1095, -1095, -1095,  1074,
    1075,  1206,  1201,  1308,    38,   108,    92,    22,    22,   152,
     -42,  1309,  1080, -1095, -1095, -1095, -1095,  1310,  1193, -1095,
   -1095, -1095,  1081, -1095, -1095, -1095,   112,  1312, -1095,  1267,
   -1095,   213, -1095, -1095, -1095, -1095, -1095, -1095, -1095,  1025,
   -1095, -1095, -1095, -1095,  1084,   494,  1208,  1088,  1292,  1317,
    1085,  1125,   108,  1092,  1152,  1225,  1228, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095,  1213,  1093,  1292,   905, -1095,   861, -1095,
    1094,  1098,  1096,  1099,  1100,  -104, -1095, -1095, -1095, -1095,
    1201,  1101,  1307,  1102, -1095,  1103, -1095, -1095,  1193,  1106,
   -1095,  1107, -1095,  1307,   861, -1095,  1318,    17, -1095, -1095,
    1108, -1095, -1095,  1238,  1109,  1146, -1095,   108,  1292,  1249,
      17,  1146, -1095,  1113,  1114,   152, -1095,  1347,  1115, -1095,
   -1095,  1307, -1095, -1095, -1095,  1349,    86,  1119, -1095,   108,
     404,  1146,   404,  1120, -1095, -1095,   977,   152,  1121, -1095,
   -1095, -1095, -1095, -1095,  1350, -1095, -1095, -1095,   404, -1095,
   -1095
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1095, -1095,  1352, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095,   142, -1095, -1095,  1105,  -109,  -380,   933,  1130,
    1289,  -508, -1095, -1095, -1095, -1095,   678, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,  1298, -1095,
   -1095, -1095,   340, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095,   875,  1144, -1095, -1095, -1095, -1095,   803, -1095,
   -1095, -1095,   378, -1095, -1095, -1095,  -634,   350,   284, -1095,
   -1095,   506,   309, -1095, -1095, -1095, -1095, -1095,   220, -1095,
   -1095,  1190, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095,  -632, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095, -1095,
   -1095, -1095,   300,   617,  -848,   263, -1095,  -967, -1095, -1015,
     222, -1094, -1046, -1066,  -465,  -468, -1095, -1095, -1095,   532,
     948,  -230,  -181,  -391,   825,   274,  -385,  -489,  -652,  -537,
    -451,  -535,  -654,  -174,   -78, -1095,   583, -1095, -1095,   862,
   -1095, -1095,  1095,   -92,   800, -1095,  -483, -1095, -1095, -1095,
   -1095, -1095,  -192, -1095
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -239
static const yytype_int16 yytable[] =
{
     251,   278,   509,   387,   248,   261,   604,   687,   685,   323,
     345,   325,   631,   667,   352,   353,   354,   339,   340,   600,
     342,   344,   653,   654,   243,   555,   308,   539,   243,   576,
     808,   360,   243,   540,   363,   364,   811,   334,  1019,   372,
     661,   374,  1131,   650,   571,   269,   270,   317,   334,   348,
     269,   270,  1088,   897,   621,   638,   689,   390,   391,   392,
     393,   643,    16,   269,   270,   865,   646,   576,   818,   884,
     367,   419,   576,  1145,   368,   369,   367,   992,   421,   379,
     368,   369,   591,   269,   270,   594,   266,   271,   272,  1060,
     432,  1214,   296,     7,   523,   297,  1108,  1204,   548,  1173,
     243,   327,   447,   448,   395,   450,   549,  1191,   676,     9,
     320,   308,   309,   269,   270,   677,  1149,  1222,   671,   321,
     534,   361,   362,   297,   395,   365,   366,   297,   657,   373,
    1150,   678,   376,   359,   378,  1211,   308,   383,   679,   633,
    1109,  1132,  1201,  1133,   634,   400,   401,   402,   403,   404,
     433,   680,   665,  1188,   666,   891,   772,  1168,   681,   516,
     367,   370,   993,   380,   368,   369,   396,   400,   401,   402,
     403,   404,   406,   647,   422,  1215,   281,   282,   283,   284,
     285,   298,   286,   273,   287,   288,   289,   290,   291,   267,
     639,     1,     2,  1061,   292,   257,  1134,   309,   299,   274,
       8,   273,   318,   622,   349,    15,   258,   259,   257,   298,
     644,    11,   343,   298,   866,   275,   276,  1124,   885,   258,
     259,   273,   309,   335,   682,   293,   299,   257,    13,   861,
     299,   683,   274,   275,   276,   600,   858,   855,   258,   259,
     815,   269,   270,   859,   541,   400,   401,   402,   403,   404,
    1028,   658,   904,   275,   276,  1029,   564,   565,   244,   905,
     556,   310,   417,   628,   652,   686,   244,   311,   257,    14,
     574,   575,   257,   245,   246,   176,   793,   245,   246,   258,
     259,   245,   246,   258,   259,   595,   596,   804,   312,   313,
     849,   803,   667,   878,   620,   924,    82,   177,   625,   626,
     627,   616,   652,   894,   357,   850,   688,   652,   939,   247,
     515,   571,   889,   247,   756,   632,   179,   247,   547,   524,
     525,   906,   526,   527,   528,   529,   530,   579,   580,   896,
     300,   907,   579,   580,   244,   180,   301,   715,   908,   396,
     542,   716,   717,   662,   181,   910,   310,   561,  1037,   245,
     246,   764,   311,   551,   244,   918,   182,   573,   300,   302,
     303,   583,   300,   563,   301,   586,   684,   183,   301,   245,
     246,   310,   852,   312,   313,   815,   597,   311,   184,   273,
     185,   610,   589,   611,   612,   247,   593,   250,   700,   701,
     274,   302,   303,   969,  1023,   603,  1024,   186,   718,   899,
     900,   328,   243,   187,   879,   247,   880,   881,   882,   748,
     892,   275,   276,   719,   720,   721,   722,   188,   723,   189,
     724,   398,   269,   270,   329,   330,   914,   233,   410,   915,
     411,   271,   272,   970,   971,   412,   413,   414,   402,   403,
     404,   674,   972,   190,   399,  1138,  1139,  1059,   191,  1153,
    1062,   192,   -19,   406,   973,   725,   726,   727,   193,   728,
     252,   194,   729,   730,   731,   974,   732,   733,   734,   333,
     195,   576,   196,   735,   496,   577,   197,   346,   347,   198,
     400,   401,   402,   403,   404,   199,   200,   269,   270,   201,
     202,   975,   203,   400,   401,   402,   403,   404,   204,   377,
     736,   205,   737,   738,   384,   402,   403,   404,   806,   916,
    1111,   739,   740,   741,   742,   743,   744,   206,  1099,   255,
     207,  1100,   208,  1027,   209,   210,   420,   211,   400,   401,
     402,   403,   404,   256,   745,   212,   213,   672,  1136,   214,
    1092,  1137,  1050,  1051,  1052,   691,   870,   215,   746,   216,
     217,   218,   415,   219,   220,   445,   446,   269,   270,   221,
     273,   708,   440,   400,   401,   402,   403,   404,   222,   223,
     242,   274,   400,   401,   402,   403,   404,   262,   263,   264,
     265,   795,   224,   225,   226,  1166,   400,   401,   402,   403,
     404,   227,   275,   276,   400,   401,   402,   403,   404,   923,
     875,   229,   805,   927,   872,   230,   231,   931,   932,   933,
     232,   819,   936,   937,   938,   822,   238,   440,   825,   826,
     239,   240,   247,   416,   898,   273,   698,   820,   821,   241,
     253,   824,   714,   280,   324,   341,   417,   357,   338,   385,
     418,   389,   257,   388,   394,   397,   427,   269,   270,   429,
    1200,   245,   246,   258,   259,   431,   437,   275,   276,   329,
     330,   752,   439,   434,   435,   436,   934,   759,   440,   442,
     441,   443,  1219,   968,   449,   400,   401,   402,   403,   404,
     451,   400,   401,   402,   403,   404,   452,   247,   281,   282,
     283,   284,   285,   919,   286,   273,   287,   288,   289,   290,
     291,   453,   269,   270,   455,   457,   578,   458,   459,  1194,
     400,   401,   402,   403,   404,   460,   400,   401,   402,   403,
     404,   463,  1203,   464,  -238,   466,   467,   275,   276,   468,
     472,   579,   580,   469,   470,   471,   473,   293,   474,   912,
     475,   476,   400,   401,   402,   403,   404,   477,   234,   922,
     478,   479,   484,   480,    85,   481,    87,   482,    89,   483,
      91,  1066,    93,   485,    95,   488,    97,   494,    99,   486,
     101,   487,   490,  1038,   491,   492,   493,   495,   281,   282,
     283,   284,   285,   497,   286,   273,   287,   288,   289,   290,
     291,    86,   498,    88,   514,    90,   292,    92,   522,    94,
     499,    96,   500,    98,   501,   100,   502,   102,   503,  1103,
    1104,  1105,  1106,  1107,   504,   505,   506,   275,   276,   400,
     401,   402,   403,   404,   507,   508,   511,   293,   512,   513,
    1220,   517,  1223,   281,   282,   283,   284,   285,  1034,   286,
     273,   287,   288,   289,   290,   291,   518,   519,  1230,   269,
     270,   292,   400,  1036,   402,   403,   404,   520,   531,   433,
     552,   533,   532,   543,   544,   545,   554,    16,   558,   559,
     560,   562,   275,   276,   567,    17,   568,   569,   570,   584,
     587,   588,   293,    18,    19,    20,    21,    22,    23,   585,
     590,    24,    25,    26,    27,    28,    29,    30,  1140,   592,
     598,    31,    32,    33,    34,    35,   601,    36,   602,   269,
     270,   605,    37,    38,    39,    40,    41,    42,    43,    44,
     606,    45,    46,    47,   607,    48,   608,    49,    50,    51,
      52,    53,    54,    55,   609,   615,   617,    56,    57,   618,
      58,   619,   624,   629,   636,   637,   640,    59,   641,   642,
     648,   649,  -185,   655,   660,   663,   675,   664,  1101,   690,
     326,   692,   693,   694,   697,   702,   703,   696,   704,   705,
     758,    60,   706,   707,    61,   709,   710,   762,   711,   764,
     281,   282,   283,   284,   285,   712,   286,   273,   287,   288,
     289,   290,   291,   755,   713,   751,   768,   756,   292,   766,
      62,   757,   760,   761,   763,   765,    63,   767,    64,    65,
     769,   770,    66,    67,  1207,    68,   771,    69,   773,   275,
     276,   774,   776,    70,   777,   781,    71,    72,   778,   293,
     779,   780,    73,    74,   783,    75,  1226,    76,   785,  1175,
     281,   282,   283,   284,   285,   784,   286,   273,   287,   288,
     289,   290,   291,   940,   941,   786,   790,   787,   292,   788,
     789,   708,   791,   792,   797,  1192,   794,   798,   801,   799,
     800,   802,    77,    78,    79,    80,   812,    81,   328,   275,
     276,   814,   813,   571,   817,   823,   815,   816,   827,   293,
     828,   829,   830,   831,   832,   833,   834,   835,   942,   943,
     944,    82,   851,   846,   836,   945,   946,    83,    84,   837,
     838,   839,   947,   840,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   998,   999,   841,   842,   843,   844,   845,   854,
     847,   848,  1000,   948,   852,   855,   949,   950,   951,   952,
     953,   954,   955,   956,   957,   958,   959,   960,   961,   962,
     963,   964,    84,   857,   856,   862,   863,  1001,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   864,   868,   869,   871,
    1002,   873,   874,   876,   877,   887,   888,   883,   895,   893,
     925,   909,   926,   911,   928,   930,   257,   913,   917,   920,
     921,   929,   935,   977,   979,   980,   982,   258,   259,   983,
     984,   986,   985,   987,   988,   990,   991,   995,  1017,   996,
     997,  1018,  1013,  1014,  1003,  1004,  1005,  1006,  1015,  1007,
    1008,  1009,  1010,  1011,  1016,  1020,  1021,  1022,  1030,  1032,
    1033,  1031,  1035,  1039,  1040,  1041,  1042,  1043,  1044,  1045,
    1046,  1047,  1048,  1049,  1055,  1057,  1058,  1063,  1064,  1068,
    1067,  1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,
    1078,  1082,  1079,  1080,  1081,  1087,  1083,  1084,  1085,  1086,
    1089,  1091,  1093,  1096,  1094,  1097,  1102,  1112,  1113,  1114,
    1098,  1115,  1117,  1116,  1120,  1119,  1121,  1127,  1122,  1128,
    1125,  1126,  1130,  1142,  1144,  1152,  1143,  1151,  1147,  1158,
    1161,  1163,  1155,  1156,  1164,  1160,  1165,  1170,  1169,  1171,
    1176,  1167,  1172,  1179,  1177,  1178,  1184,  1193,  1183,  1180,
    1187,  1186,  1189,  1190,  1195,  1196,  1197,  1198,  1202,  1205,
    1206,  1208,  1209,  1213,  1229,  1217,  1224,    10,  1225,   546,
    1227,   430,   386,   235,   809,   228,  1095,   375,  1065,   699,
     630,  1090,   989,  1123,  1146,  1181,   307,   853,  1157,   967,
    1141,   553,  1182,   902,   695,   454,     0,     0,     0,     0,
     749,     0,     0,   673
};

static const yytype_int16 yycheck[] =
{
     109,   182,   382,   233,    82,   179,   474,   544,   543,   190,
     202,   192,   495,   521,   206,   207,   208,   198,   199,   470,
     201,   202,   511,   512,     4,     3,     3,   418,     4,     3,
     684,   212,     4,   418,   215,   216,   688,    63,   886,   220,
      89,   222,     4,   508,    88,    89,    90,    89,    63,    89,
      89,    90,  1019,    89,    89,    73,   545,   238,   239,   240,
     241,    89,     6,    89,    90,    89,     4,     3,   702,    89,
       4,   252,     3,  1088,     8,     9,     4,     4,   252,     5,
       8,     9,   462,    89,    90,   465,     4,    93,    94,    97,
     236,     5,   184,   237,   239,     3,   200,  1191,    81,  1145,
       4,   193,   283,   284,     8,   286,    89,  1173,   236,     0,
      80,     3,    89,    89,    90,   243,     4,  1211,   241,    89,
       4,   213,   214,     3,     8,   217,   218,     3,   173,   221,
      18,   236,   224,   211,   226,  1201,     3,   229,   243,    84,
     244,   103,  1188,   105,    89,   290,   291,   292,   293,   294,
     296,   236,   161,  1168,   163,   789,   624,  1124,   243,   389,
       4,    89,    89,    89,     8,     9,   244,   290,   291,   292,
     293,   294,   250,   111,   252,    89,   220,   221,   222,   223,
     224,    89,   226,   227,   228,   229,   230,   231,   232,   107,
     208,    20,    21,   201,   238,   244,   158,    89,   106,   238,
     237,   227,   244,   238,   244,   237,   255,   256,   244,    89,
     238,   141,   238,    89,   238,   259,   260,  1065,   238,   255,
     256,   227,    89,   238,   236,   269,   106,   244,   244,   766,
     106,   243,   238,   259,   260,   686,    89,   236,   255,   256,
     239,    89,    90,    96,   418,   290,   291,   292,   293,   294,
     904,   296,   236,   259,   260,   907,    94,    95,   238,   243,
     238,   238,   238,   493,   238,   242,   238,   244,   244,   244,
     451,   452,   244,   253,   254,   236,   656,   253,   254,   255,
     256,   253,   254,   255,   256,   466,   467,   678,   265,   266,
     755,   676,   800,     4,   486,   830,   240,   142,   490,   491,
     492,   482,   238,   792,   238,   756,   242,   238,   845,   289,
     388,    88,    89,   289,   236,   496,   237,   289,   427,   397,
     398,   243,   400,   401,   402,   403,   404,   263,   264,   797,
     238,   236,   263,   264,   238,   237,   244,     4,   243,   417,
     418,     8,     9,   517,   237,   813,   238,   439,   237,   253,
     254,   240,   244,   431,   238,   823,   237,   449,   238,   267,
     268,   453,   238,   441,   244,   457,   242,   237,   244,   253,
     254,   238,   236,   265,   266,   239,   468,   244,   237,   227,
     237,    89,   460,    91,    92,   289,   464,   238,   569,   570,
     238,   267,   268,    85,   175,   473,   177,   237,    65,   249,
     250,   238,     4,   237,   115,   289,   117,   118,   119,   590,
     790,   259,   260,    80,    81,    82,    83,   237,    85,   237,
      87,   218,    89,    90,   261,   262,   236,   240,    30,   239,
      32,    93,    94,   125,   126,    37,    38,    39,   292,   293,
     294,   533,   134,   237,   241,  1077,  1078,   984,   237,   236,
     987,   237,   239,   531,   146,   122,   123,   124,   237,   126,
     295,   237,   129,   130,   131,   157,   133,   134,   135,   195,
     237,     3,   237,   140,   236,     7,   237,   203,   204,   237,
     290,   291,   292,   293,   294,   237,   237,    89,    90,   237,
     237,   183,   237,   290,   291,   292,   293,   294,   237,   225,
     167,   237,   169,   170,   230,   292,   293,   294,   682,   236,
    1045,   178,   179,   180,   181,   182,   183,   237,   236,     3,
     237,   239,   237,   903,   237,   237,   252,   237,   290,   291,
     292,   293,   294,   236,   201,   237,   237,   241,  1075,   237,
    1023,  1076,    48,    49,    50,   241,   776,   237,   215,   237,
     237,   237,   154,   237,   237,   281,   282,    89,    90,   237,
     227,     3,     4,   290,   291,   292,   293,   294,   237,   237,
     246,   238,   290,   291,   292,   293,   294,    10,    11,    12,
      13,   659,   237,   237,   237,  1122,   290,   291,   292,   293,
     294,   237,   259,   260,   290,   291,   292,   293,   294,   829,
     781,   237,   680,   833,   778,   237,   237,   837,   838,   839,
     237,   703,   842,   843,   844,   707,   237,     4,   710,   711,
     237,   237,   289,   225,   798,   227,   241,   705,   706,   237,
     237,   709,   241,   111,     4,   104,   238,   238,   244,   163,
     242,   240,   244,   295,   238,   240,   242,    89,    90,   236,
    1187,   253,   254,   255,   256,   240,   235,   259,   260,   261,
     262,   241,   238,   236,   236,   236,   840,   241,     4,   236,
     240,   236,  1209,   847,   238,   290,   291,   292,   293,   294,
     238,   290,   291,   292,   293,   294,   238,   289,   220,   221,
     222,   223,   224,   241,   226,   227,   228,   229,   230,   231,
     232,   238,    89,    90,   238,   298,   238,   236,     3,  1177,
     290,   291,   292,   293,   294,   240,   290,   291,   292,   293,
     294,     3,  1190,   240,   242,   236,   236,   259,   260,   236,
       4,   263,   264,   236,   236,   236,   240,   269,   236,   817,
     236,   238,   290,   291,   292,   293,   294,     4,   247,   827,
     236,   236,     4,   236,   253,   236,   255,   236,   257,   236,
     259,   991,   261,   236,   263,     6,   265,     4,   267,   236,
     269,   236,   236,   947,   236,   236,   236,   236,   220,   221,
     222,   223,   224,   236,   226,   227,   228,   229,   230,   231,
     232,   254,   236,   256,   241,   258,   238,   260,   239,   262,
     236,   264,   236,   266,   236,   268,   236,   270,   236,  1039,
    1040,  1041,  1042,  1043,   236,   236,   236,   259,   260,   290,
     291,   292,   293,   294,   236,   236,   236,   269,   236,   236,
    1210,   236,  1212,   220,   221,   222,   223,   224,   916,   226,
     227,   228,   229,   230,   231,   232,   236,   236,  1228,    89,
      90,   238,   290,   291,   292,   293,   294,   236,   236,   296,
      15,   237,   239,   237,   237,   237,     3,     6,     3,   236,
     241,   236,   259,   260,     5,    14,   245,   236,   236,   233,
     107,   236,   269,    22,    23,    24,    25,    26,    27,   239,
     236,    30,    31,    32,    33,    34,    35,    36,  1079,   236,
      88,    40,    41,    42,    43,    44,    88,    46,   236,    89,
      90,    45,    51,    52,    53,    54,    55,    56,    57,    58,
       4,    60,    61,    62,   236,    64,    45,    66,    67,    68,
      69,    70,    71,    72,    47,   201,    98,    76,    77,   236,
      79,    98,   236,   236,   105,   101,   205,    86,   111,   114,
     149,   149,   153,   164,   241,   159,   236,   160,  1036,   243,
     200,   236,     3,   236,     4,   297,   236,   239,   239,   236,
       4,   110,   236,   236,   113,   240,   236,     4,   236,   240,
     220,   221,   222,   223,   224,   236,   226,   227,   228,   229,
     230,   231,   232,   236,   244,   244,     4,   236,   238,   237,
     139,   236,   236,   236,   236,   236,   145,   236,   147,   148,
     236,     4,   151,   152,  1195,   154,   236,   156,   236,   259,
     260,   236,   236,   162,    88,   237,   165,   166,   236,   269,
     236,   236,   171,   172,   236,   174,  1217,   176,     4,  1148,
     220,   221,   222,   223,   224,   236,   226,   227,   228,   229,
     230,   231,   232,    91,    92,   236,   153,   236,   238,   236,
     236,     3,   236,   236,   236,  1174,   296,   236,   239,   237,
     236,     4,   211,   212,   213,   214,    16,   216,   238,   259,
     260,     3,   236,    88,   236,   236,   239,   245,   238,   269,
     239,   237,   237,   237,   237,   237,   237,   237,   136,   137,
     138,   240,     5,   239,   237,   143,   144,   246,   247,   237,
     237,   237,   150,   237,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,    99,   100,   237,   237,   237,   237,   237,     4,
     236,   236,   109,   181,   236,   236,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   247,     4,   201,   201,    98,   134,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   236,   244,    98,   236,
     157,   102,    74,   236,   108,   108,     4,   236,   242,   108,
     121,   236,   120,   236,   127,   132,   244,   239,   239,   239,
     239,   128,   168,   236,     4,   236,   236,   255,   256,     4,
     237,   236,   241,   237,     4,   236,   236,   236,     4,   236,
     236,     4,   236,   236,   201,   202,   203,   204,   236,   206,
     207,   208,   209,   210,   236,   236,   236,   236,    17,     5,
      48,   239,   239,   237,   237,   237,   237,   237,   237,   237,
     237,   237,     3,   236,   112,     4,   236,   239,   236,    99,
     239,    75,   103,   237,   237,   237,   237,   237,   237,   237,
     237,   115,   237,   237,   237,   236,   115,   115,   115,   239,
       4,     7,   237,   236,   243,   236,    89,     4,   158,     4,
     239,   236,   109,   237,     4,   236,   236,   101,   237,   108,
     236,   236,     4,     4,     4,    48,   236,     5,   237,   111,
      28,     4,   297,   239,   239,   237,   201,    99,   103,   116,
     236,   239,   239,   234,   236,   239,    29,    19,   237,   239,
     237,   239,   236,   236,   236,   107,   237,   201,    99,   236,
     236,     4,   237,     4,     4,   236,   236,     5,  1216,   426,
     239,   256,   232,    74,   686,    67,  1026,   223,   990,   566,
     495,  1021,   866,  1064,  1090,  1155,   186,   760,  1115,   847,
    1080,   433,  1160,   800,   559,   290,    -1,    -1,    -1,    -1,
     590,    -1,    -1,   531
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    20,    21,   300,   301,   302,   303,   237,   237,     0,
     301,   141,   313,   244,   244,   237,     6,    14,    22,    23,
      24,    25,    26,    27,    30,    31,    32,    33,    34,    35,
      36,    40,    41,    42,    43,    44,    46,    51,    52,    53,
      54,    55,    56,    57,    58,    60,    61,    62,    64,    66,
      67,    68,    69,    70,    71,    72,    76,    77,    79,    86,
     110,   113,   139,   145,   147,   148,   151,   152,   154,   156,
     162,   165,   166,   171,   172,   174,   176,   211,   212,   213,
     214,   216,   240,   246,   247,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   304,   305,   306,   307,   308,   315,   317,
     318,   319,   320,   322,   323,   331,   334,   335,   337,   338,
     345,   347,   348,   350,   352,   354,   358,   359,   360,   363,
     365,   372,   379,   382,   388,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   404,   405,   406,   407,   408,
     412,   413,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   236,   142,   314,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   337,   237,
     237,   237,   237,   240,   247,   319,   321,   339,   237,   237,
     237,   237,   246,     4,   238,   253,   254,   289,   473,   483,
     238,   315,   295,   237,   349,     3,   236,   244,   255,   256,
     459,   472,    10,    11,    12,    13,     4,   107,   484,    89,
      90,    93,    94,   227,   238,   259,   260,   380,   461,   462,
     111,   220,   221,   222,   223,   224,   226,   228,   229,   230,
     231,   232,   238,   269,   461,   481,   482,     3,    89,   106,
     238,   244,   267,   268,   390,   470,   471,   390,     3,    89,
     238,   244,   265,   266,   384,   468,   469,    89,   244,   441,
      80,    89,   453,   461,     4,   461,   200,   482,   238,   261,
     262,   464,   465,   464,    63,   238,   491,   492,   244,   461,
     461,   104,   461,   238,   461,   491,   464,   464,    89,   244,
     362,   442,   491,   491,   491,   460,   473,   238,   361,   473,
     461,   482,   482,   461,   461,   482,   482,     4,     8,     9,
      89,   485,   461,   482,   461,   362,   482,   464,   482,     5,
      89,   373,   355,   482,   464,   163,   318,   460,   295,   240,
     461,   461,   461,   461,   238,     8,   473,   240,   218,   241,
     290,   291,   292,   293,   294,   472,   473,   478,   479,   480,
      30,    32,    37,    38,    39,   154,   225,   238,   242,   461,
     464,   472,   473,   487,   488,   489,   353,   242,   316,   236,
     314,   240,   236,   296,   236,   236,   236,   235,   463,   238,
       4,   240,   236,   236,   367,   464,   464,   461,   461,   238,
     461,   238,   238,   238,   481,   238,   482,   298,   236,     3,
     240,   391,   389,     3,   240,   383,   236,   236,   236,   236,
     236,   236,     4,   240,   236,   236,   238,     4,   236,   236,
     236,   236,   236,   236,     4,   236,   236,   236,     6,   336,
     236,   236,   236,   236,     4,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   316,
     356,   236,   236,   236,   241,   473,   460,   236,   236,   236,
     236,   332,   239,   239,   473,   473,   473,   473,   473,   473,
     473,   236,   239,   237,     4,   327,   328,   329,   330,   462,
     465,   472,   473,   237,   237,   237,   317,   315,    81,    89,
     454,   473,    15,   459,     3,     3,   238,   414,     3,   236,
     241,   482,   236,   473,    94,    95,   381,     5,   245,   236,
     236,    88,   375,   482,   461,   461,     3,     7,   238,   263,
     264,   466,   467,   482,   233,   239,   482,   107,   236,   473,
     236,   316,   236,   473,   316,   461,   461,   482,    88,   325,
     469,    88,   236,   473,   454,    45,     4,   236,    45,    47,
      89,    91,    92,   455,   458,   201,   461,    98,   236,    98,
     491,    89,   238,   368,   236,   491,   491,   491,   460,   236,
     361,   485,   461,    84,    89,   490,   105,   101,    73,   208,
     205,   111,   114,    89,   238,   369,     4,   111,   149,   149,
     453,   357,   238,   466,   466,   164,   351,   173,   296,   346,
     241,    89,   472,   159,   160,   161,   163,   320,   475,   476,
     477,   241,   241,   478,   482,   236,   236,   243,   236,   243,
     236,   243,   236,   243,   242,   470,   242,   468,   242,   466,
     243,   241,   236,     3,   236,   463,   239,     4,   241,   367,
     461,   461,   297,   236,   239,   236,   236,   236,     3,   240,
     236,   236,   236,   244,   241,     4,     8,     9,    65,    80,
      81,    82,    83,    85,    87,   122,   123,   124,   126,   129,
     130,   131,   133,   134,   135,   140,   167,   169,   170,   178,
     179,   180,   181,   182,   183,   201,   215,   392,   461,   483,
     486,   244,   241,   385,   377,   236,   236,   236,     4,   241,
     236,   236,     4,   236,   240,   236,   237,   236,     4,   236,
       4,   236,   454,   236,   236,   403,   236,    88,   236,   236,
     236,   237,   409,   236,   236,     4,   236,   236,   236,   236,
     153,   236,   236,   316,   296,   473,   340,   236,   236,   237,
     236,   239,     4,   465,   462,   473,   472,   324,   471,   325,
     326,   467,    16,   236,     3,   239,   245,   236,   375,   482,
     473,   473,   482,   236,   473,   482,   482,   238,   239,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   239,   236,   236,   453,
     469,     5,   236,   442,     4,   236,   201,     4,    89,    96,
     457,   468,   201,    98,   236,    89,   238,   370,   244,    98,
     460,   236,   472,   102,    74,   461,   236,   108,     4,   115,
     117,   118,   119,   236,    89,   238,   371,   108,     4,    89,
     374,   375,   316,   108,   466,   242,   454,    89,   472,   249,
     250,   474,   475,   333,   236,   243,   243,   236,   243,   236,
     454,   236,   473,   239,   236,   239,   236,   239,   454,   241,
     239,   239,   473,   460,   470,   121,   120,   460,   127,   128,
     132,   460,   460,   460,   472,   168,   460,   460,   460,   468,
      91,    92,   136,   137,   138,   143,   144,   150,   181,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   386,   456,   458,   472,    85,
     125,   126,   134,   146,   157,   183,   378,   236,   311,     4,
     236,   443,   236,     4,   237,   241,   236,   237,     4,   380,
     236,   236,     4,    89,   376,   236,   236,   236,    99,   100,
     109,   134,   157,   201,   202,   203,   204,   206,   207,   208,
     209,   210,   410,   236,   236,   236,   236,     4,     4,   443,
     236,   236,   236,   175,   177,   341,   342,   316,   471,   467,
      17,   239,     5,    48,   473,   239,   291,   237,   472,   237,
     237,   237,   237,   237,   237,   237,   237,   237,     3,   236,
      48,    49,    50,   444,   445,   112,   446,     4,   236,   468,
      97,   201,   468,   239,   236,   371,   460,   239,    99,    75,
     103,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   115,   115,   115,   115,   239,   236,   446,     4,
     376,     7,   485,   237,   243,   341,   236,   236,   239,   236,
     239,   473,    89,   460,   460,   460,   460,   460,   200,   244,
     387,   470,     4,   158,     4,   236,   237,   109,   448,   236,
       4,   236,   237,   381,   443,   236,   236,   101,   108,   449,
       4,     4,   103,   105,   158,   411,   468,   470,   414,   414,
     461,   441,     4,   236,     4,   448,   377,   237,   344,     4,
      18,     5,    48,   236,   309,   297,   239,   444,   111,   447,
     237,    28,   451,     4,   239,   201,   468,   239,   446,   103,
      99,   116,   239,   451,   343,   315,   236,   236,   239,   234,
     239,   387,   449,   237,    29,   452,   239,   237,   448,   236,
     236,   452,   315,    19,   454,   236,   107,   237,   201,   450,
     468,   451,    99,   454,   450,   236,   236,   461,     4,   237,
     424,   452,   364,     4,     5,    89,   312,   236,   310,   468,
     316,   425,   450,   316,   236,   311,   461,   239,   366,     4,
     316
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:

/* Line 1455 of yacc.c  */
#line 296 "lev_comp.y"
    {
			if (fatal_error > 0) {
				(void) fprintf(stderr,
              "%s: %d errors detected for level \"%s\". No output created!\n",
					       fname, fatal_error, (yyvsp[(1) - (3)].map));
				fatal_error = 0;
				got_errors++;
			} else if (!got_errors) {
				if (!write_level_file((yyvsp[(1) - (3)].map), splev)) {
                                    lc_error("Can't write output file for '%s'!",
                                             (yyvsp[(1) - (3)].map));
				    exit(EXIT_FAILURE);
				}
			}
			Free((yyvsp[(1) - (3)].map));
			Free(splev);
			splev = NULL;
			vardef_free_all(vardefs);
			vardefs = NULL;
		  ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 319 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 324 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (5)].map));
		      if ((yyvsp[(5) - (5)].i) == -1) {
			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_MAZEGRID, HWALL, 0,0,
					      0,0,0,0, SPO_INITLEVEL));
		      } else {
			  int bg = (int)what_map_char((char) (yyvsp[(5) - (5)].i));

			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_SOLIDFILL, bg, 0, 0,
					      0,0,0,0, SPO_INITLEVEL));
		      }
		      add_opvars(splev, "io",
				 VA_PASS2(MAZELEVEL, SPO_LEVEL_FLAGS));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		      (yyval.map) = (yyvsp[(3) - (5)].map);
		  ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 344 "lev_comp.y"
    {
              int lit = (int) (yyvsp[(7) - (7)].i);
		      start_level_def(&splev, (yyvsp[(3) - (7)].map));
		      if ((yyvsp[(5) - (7)].i) == -1) {
			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_MAZEGRID, HWALL, 0, lit,
					      0,0,0,0, SPO_INITLEVEL));
		      } else {
			  int bg = (int)what_map_char((char) (yyvsp[(5) - (7)].i));

			  add_opvars(splev, "iiiiiiiio",
				     VA_PASS9(LVLINIT_SOLIDFILL, bg, 0, lit,
					      0,0,0,0, SPO_INITLEVEL));
		      }
		      add_opvars(splev, "io",
				 VA_PASS2(MAZELEVEL, SPO_LEVEL_FLAGS));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		      (yyval.map) = (yyvsp[(3) - (7)].map);
		  ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 367 "lev_comp.y"
    {
		      int filling = (int) (yyvsp[(5) - (5)].terr).ter;

		      if (filling == INVALID_LEVTYPE || filling >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_SOLIDFILL, filling,
                                          0, (int) (yyvsp[(5) - (5)].terr).lit,
                                          0,0,0,0, SPO_INITLEVEL));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 380 "lev_comp.y"
    {
		      int filling = (int) what_map_char((char) (yyvsp[(5) - (5)].i));

		      if (filling == INVALID_LEVTYPE || filling >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");
                      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_MAZEGRID, filling, 0,0,
					  0,0,0,0, SPO_INITLEVEL));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 392 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 398 "lev_comp.y"
    {
                      int fg = (int) what_map_char((char) (yyvsp[(5) - (16)].i)),
                          bg = (int) what_map_char((char) (yyvsp[(7) - (16)].i));
                      int smoothed = (int) (yyvsp[(9) - (16)].i),
                          joined = (int) (yyvsp[(11) - (16)].i),
                          lit = (int) (yyvsp[(13) - (16)].i),
                          walled = (int) (yyvsp[(15) - (16)].i),
                          filling = (int) (yyvsp[(16) - (16)].i);

		      if (fg == INVALID_LEVTYPE || fg >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid foreground type.");
		      if (bg == INVALID_LEVTYPE || bg >= MAX_LEVTYPE)
			  lc_error("INIT_MAP: Invalid background type.");
		      if (joined && fg != CORR && fg != ROOM && fg != GRASS && fg != GROUND && fg != AIR && fg != CLOUD)
			  lc_error("INIT_MAP: Invalid foreground type for joined map.");

		      if (filling == INVALID_LEVTYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");

		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_MINES, filling, walled, lit,
					  joined, smoothed, bg, fg,
					  SPO_INITLEVEL));
			max_x_map = COLNO-1;
			max_y_map = ROWNO;
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 427 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 433 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_FOREST));
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 437 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(0, (int)(yyvsp[(3) - (3)].i), SPO_FOREST));
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 443 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_MONSTER_GENERATION));
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 449 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 455 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 459 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 465 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 470 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 476 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 480 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 491 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 495 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 502 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 506 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 512 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 516 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 522 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 627 "lev_comp.y"
    {
		      struct lc_vardefs *vd;

		      if ((vd = vardef_defined(vardefs, (yyvsp[(3) - (3)].map), 1))) {
			  if (!(vd->var_type & SPOVAR_ARRAY))
			      lc_error("Trying to shuffle non-array variable '%s'",
                                       (yyvsp[(3) - (3)].map));
		      } else
                          lc_error("Trying to shuffle undefined variable '%s'",
                                   (yyvsp[(3) - (3)].map));
		      add_opvars(splev, "so", VA_PASS2((yyvsp[(3) - (3)].map), SPO_SHUFFLE_ARRAY));
		      Free((yyvsp[(3) - (3)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 643 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 649 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 655 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 661 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 667 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 673 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 679 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 685 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 691 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 701 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 711 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 721 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 731 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 751 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 763 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 768 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 775 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 780 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 787 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 792 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 799 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 803 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 809 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 814 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 821 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 825 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 831 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 835 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 841 "lev_comp.y"
    {
		      struct lc_funcdefs *funcdef;

		      if (in_function_definition)
			  lc_error("Recursively defined functions not allowed (function %s).", (yyvsp[(2) - (3)].map));

		      in_function_definition++;

		      if (funcdef_defined(function_definitions, (yyvsp[(2) - (3)].map), 1))
			  lc_error("Function '%s' already defined once.", (yyvsp[(2) - (3)].map));

		      funcdef = funcdef_new(-1, (yyvsp[(2) - (3)].map));
		      funcdef->next = function_definitions;
		      function_definitions = funcdef;
		      function_splev_backup = splev;
		      splev = &(funcdef->code);
		      Free((yyvsp[(2) - (3)].map));
		      curr_function = funcdef;
		      function_tmp_var_defs = vardefs;
		      vardefs = NULL;
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 863 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 867 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 878 "lev_comp.y"
    {
		      struct lc_funcdefs *tmpfunc;

		      tmpfunc = funcdef_defined(function_definitions, (yyvsp[(1) - (4)].map), 1);
		      if (tmpfunc) {
			  int l;
			  int nparams = (int) strlen((yyvsp[(3) - (4)].map));
			  char *fparamstr = funcdef_paramtypes(tmpfunc);

			  if (strcmp((yyvsp[(3) - (4)].map), fparamstr)) {
			      char *tmps = strdup(decode_parm_str(fparamstr));

			      lc_error("Function '%s' requires params '%s', got '%s' instead.",
                                       (yyvsp[(1) - (4)].map), tmps, decode_parm_str((yyvsp[(3) - (4)].map)));
			      Free(tmps);
			  }
			  Free(fparamstr);
			  Free((yyvsp[(3) - (4)].map));
			  if (!(tmpfunc->n_called)) {
			      /* we haven't called the function yet, so insert it in the code */
			      struct opvar *jmp = New(struct opvar);

			      set_opvar_int(jmp, splev->n_opcodes+1);
			      add_opcode(splev, SPO_PUSH, jmp);
                              /* we must jump past it first, then CALL it, due to RETURN. */
			      add_opcode(splev, SPO_JMP, NULL);

			      tmpfunc->addr = splev->n_opcodes;

			      { /* init function parameter variables */
				  struct lc_funcdefs_parm *tfp = tmpfunc->params;
				  while (tfp) {
				      add_opvars(splev, "iso",
						 VA_PASS3(0, tfp->name,
							  SPO_VAR_INIT));
				      tfp = tfp->next;
				  }
			      }

			      splev_add_from(splev, &(tmpfunc->code));
			      set_opvar_int(jmp,
                                            splev->n_opcodes - jmp->vardata.l);
			  }
			  l = (int) (tmpfunc->addr - splev->n_opcodes - 2);
			  add_opvars(splev, "iio",
				     VA_PASS3(nparams, l, SPO_CALL));
			  tmpfunc->n_called++;
		      } else {
			  lc_error("Function '%s' not defined.", (yyvsp[(1) - (4)].map));
		      }
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 933 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 939 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 943 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 949 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 956 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 960 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 968 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 972 "lev_comp.y"
    {
		      struct opvar *chkjmp;

		      if (in_switch_statement > 0)
			  lc_error("Cannot nest switch-statements.");

		      in_switch_statement++;

		      n_switch_case_list = 0;
		      switch_default_case = NULL;

		      if (!is_inconstant_number)
			  add_opvars(splev, "o", VA_PASS1(SPO_RN2));
		      is_inconstant_number = 0;

		      chkjmp = New(struct opvar);
		      set_opvar_int(chkjmp, splev->n_opcodes+1);
		      switch_check_jump = chkjmp;
		      add_opcode(splev, SPO_PUSH, chkjmp);
		      add_opcode(splev, SPO_JMP, NULL);
		      break_stmt_start();
		  ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 995 "lev_comp.y"
    {
		      struct opvar *endjump = New(struct opvar);
		      int i;

		      set_opvar_int(endjump, splev->n_opcodes+1);

		      add_opcode(splev, SPO_PUSH, endjump);
		      add_opcode(splev, SPO_JMP, NULL);

		      set_opvar_int(switch_check_jump,
			     splev->n_opcodes - switch_check_jump->vardata.l);

		      for (i = 0; i < n_switch_case_list; i++) {
			  add_opvars(splev, "oio",
				     VA_PASS3(SPO_COPY,
					      switch_case_value[i], SPO_CMP));
			  set_opvar_int(switch_case_list[i],
			 switch_case_list[i]->vardata.l - splev->n_opcodes-1);
			  add_opcode(splev, SPO_PUSH, switch_case_list[i]);
			  add_opcode(splev, SPO_JE, NULL);
		      }

		      if (switch_default_case) {
			  set_opvar_int(switch_default_case,
			 switch_default_case->vardata.l - splev->n_opcodes-1);
			  add_opcode(splev, SPO_PUSH, switch_default_case);
			  add_opcode(splev, SPO_JMP, NULL);
		      }

		      set_opvar_int(endjump, splev->n_opcodes - endjump->vardata.l);

		      break_stmt_end(splev);

		      add_opcode(splev, SPO_POP, NULL); /* get rid of the value in stack */
		      in_switch_statement--;


		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1040 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1050 "lev_comp.y"
    {
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1053 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1063 "lev_comp.y"
    {
		  ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1068 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1082 "lev_comp.y"
    {
		      char buf[256], buf2[256];

		      if (n_forloops >= MAX_NESTED_IFS) {
			  lc_error("FOR: Too deeply nested loops.");
			  n_forloops = MAX_NESTED_IFS - 1;
		      }

		      /* first, define a variable for the for-loop end value */
		      Sprintf(buf, "%s end", (yyvsp[(2) - (6)].map));
		      /* the value of which is already in stack (the 2nd math_expr) */
		      add_opvars(splev, "iso", VA_PASS3(0, buf, SPO_VAR_INIT));

		      vardefs = add_vardef_type(vardefs, (yyvsp[(2) - (6)].map), SPOVAR_INT);
		      /* define the for-loop variable. value is in stack (1st math_expr) */
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(2) - (6)].map), SPO_VAR_INIT));

		      /* calculate value for the loop "step" variable */
		      Sprintf(buf2, "%s step", (yyvsp[(2) - (6)].map));
		      /* end - start */
		      add_opvars(splev, "vvo",
				 VA_PASS3(buf, (yyvsp[(2) - (6)].map), SPO_MATH_SUB));
		      /* sign of that */
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SIGN));
		      /* save the sign into the step var */
		      add_opvars(splev, "iso",
				 VA_PASS3(0, buf2, SPO_VAR_INIT));

		      forloop_list[n_forloops].varname = strdup((yyvsp[(2) - (6)].map));
		      forloop_list[n_forloops].jmp_point = splev->n_opcodes;

		      n_forloops++;
		      Free((yyvsp[(2) - (6)].map));
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1119 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1124 "lev_comp.y"
    {
                      int l;
		      char buf[256], buf2[256];

		      n_forloops--;
		      Sprintf(buf, "%s step", forloop_list[n_forloops].varname);
		      Sprintf(buf2, "%s end", forloop_list[n_forloops].varname);
		      /* compare for-loop var to end value */
		      add_opvars(splev, "vvo",
				 VA_PASS3(forloop_list[n_forloops].varname,
					  buf2, SPO_CMP));
		      /* var + step */
		      add_opvars(splev, "vvo",
				VA_PASS3(buf, forloop_list[n_forloops].varname,
					 SPO_MATH_ADD));
		      /* for-loop var = (for-loop var + step) */
		      add_opvars(splev, "iso",
				 VA_PASS3(0, forloop_list[n_forloops].varname,
					  SPO_VAR_INIT));
		      /* jump back if compared values were not equal */
                      l = (int) (forloop_list[n_forloops].jmp_point
                                 - splev->n_opcodes - 1);
		      add_opvars(splev, "io", VA_PASS2(l, SPO_JNE));
		      Free(forloop_list[n_forloops].varname);
		      break_stmt_end(splev);
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1153 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (n_if_list >= MAX_NESTED_IFS) {
			  lc_error("LOOP: Too deeply nested conditionals.");
			  n_if_list = MAX_NESTED_IFS - 1;
		      }
		      set_opvar_int(tmppush, splev->n_opcodes);
		      if_list[n_if_list++] = tmppush;

		      add_opvars(splev, "o", VA_PASS1(SPO_DEC));
		      break_stmt_start();
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1167 "lev_comp.y"
    {
		      struct opvar *tmppush;

		      add_opvars(splev, "oio", VA_PASS3(SPO_COPY, 0, SPO_CMP));

		      tmppush = (struct opvar *) if_list[--n_if_list];
		      set_opvar_int(tmppush,
                                    tmppush->vardata.l - splev->n_opcodes-1);
		      add_opcode(splev, SPO_PUSH, tmppush);
		      add_opcode(splev, SPO_JG, NULL);
		      add_opcode(splev, SPO_POP, NULL); /* discard count */
		      break_stmt_end(splev);
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1183 "lev_comp.y"
    {
		      struct opvar *tmppush2 = New(struct opvar);

		      if (n_if_list >= MAX_NESTED_IFS) {
			  lc_error("IF: Too deeply nested conditionals.");
			  n_if_list = MAX_NESTED_IFS - 1;
		      }

		      add_opcode(splev, SPO_CMP, NULL);

		      set_opvar_int(tmppush2, splev->n_opcodes+1);

		      if_list[n_if_list++] = tmppush2;

		      add_opcode(splev, SPO_PUSH, tmppush2);

		      add_opcode(splev, reverse_jmp_opcode( (yyvsp[(1) - (2)].i) ), NULL);

		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1203 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1215 "lev_comp.y"
    {
		      struct opvar *tmppush2 = New(struct opvar);

		      if (n_if_list >= MAX_NESTED_IFS) {
			  lc_error("IF: Too deeply nested conditionals.");
			  n_if_list = MAX_NESTED_IFS - 1;
		      }

		      add_opcode(splev, SPO_CMP, NULL);

		      set_opvar_int(tmppush2, splev->n_opcodes+1);

		      if_list[n_if_list++] = tmppush2;

		      add_opcode(splev, SPO_PUSH, tmppush2);

		      add_opcode(splev, reverse_jmp_opcode( (yyvsp[(2) - (2)].i) ), NULL);

		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1235 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1241 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1251 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush = New(struct opvar);
			  struct opvar *tmppush2;

			  set_opvar_int(tmppush, splev->n_opcodes+1);
			  add_opcode(splev, SPO_PUSH, tmppush);

			  add_opcode(splev, SPO_JMP, NULL);

			  tmppush2 = (struct opvar *) if_list[--n_if_list];

			  set_opvar_int(tmppush2,
                                      splev->n_opcodes - tmppush2->vardata.l);
			  if_list[n_if_list++] = tmppush;
		      } else lc_error("IF: Huh?!  No else-part address?");
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1269 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1279 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiio", VA_PASS8(0, 0, 0, 0, NO_COLOR, ATR_NONE, SPLEV_MESSAGE_TYPE_MESSAGE, SPO_MESSAGE));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1283 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiio", VA_PASS8((int)(yyvsp[(11) - (11)].i), 0, 0, 0, (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_MESSAGE));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1287 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiio", VA_PASS8((int)(yyvsp[(17) - (17)].i), (int)(yyvsp[(15) - (17)].i), (int)(yyvsp[(13) - (17)].i), (int)(yyvsp[(11) - (17)].i), (int)(yyvsp[(9) - (17)].i), (int)(yyvsp[(7) - (17)].i), (int)(yyvsp[(5) - (17)].i), SPO_MESSAGE));
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1293 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1298 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1303 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1310 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1317 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (int64_t)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1326 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1334 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((int64_t)(yyvsp[(1) - (4)].i), (int64_t)(yyvsp[(2) - (4)].i), (int64_t)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1345 "lev_comp.y"
    {
		      int64_t rflags = (yyvsp[(8) - (13)].i);
		      int64_t flmt = (int64_t)(yyvsp[(9) - (13)].i);
		      int64_t flt = (int64_t)(yyvsp[(10) - (13)].i);
		      int64_t tlset = (int64_t)(yyvsp[(11) - (13)].i);
		      int64_t decotyp = (int64_t)(yyvsp[(12) - (13)].i);

		      if (rflags == -1) rflags = ((int64_t)1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiiiio",
				 VA_PASS12(decotyp, tlset, flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (13)].crd).x, (yyvsp[(5) - (13)].crd).y, (yyvsp[(7) - (13)].sze).width, (yyvsp[(7) - (13)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1363 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1370 "lev_comp.y"
    {
		      int64_t rflags = (yyvsp[(10) - (15)].i);
		      int64_t flmt = (int64_t)(yyvsp[(11) - (15)].i);
		      int64_t flt = (int64_t)(yyvsp[(12) - (15)].i);
		      int64_t tlset = (int64_t)(yyvsp[(13) - (15)].i);
		      int64_t decotyp = (int64_t)(yyvsp[(14) - (15)].i);

		      if (rflags == -1) rflags = ((int64_t)1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiiiio",
				 VA_PASS12(decotyp, tlset, flt, flmt, rflags,
					  (yyvsp[(7) - (15)].crd).x, (yyvsp[(7) - (15)].crd).y, (yyvsp[(5) - (15)].crd).x, (yyvsp[(5) - (15)].crd).y,
					  (yyvsp[(9) - (15)].sze).width, (yyvsp[(9) - (15)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1388 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1395 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1399 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1405 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 1 || (yyvsp[(2) - (5)].i) > 5 ||
			    (yyvsp[(4) - (5)].i) < 1 || (yyvsp[(4) - (5)].i) > 5 ) {
			    lc_error("Room positions should be between 1-5: (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1415 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1421 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1430 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1436 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1441 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1447 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1452 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1458 "lev_comp.y"
    {
			/* ERR means random here */
			if ((yyvsp[(7) - (10)].i) == ERR && (yyvsp[(9) - (10)].i) != ERR) {
			    lc_error("If the door wall is random, so must be its pos!");
			} else {
			    add_opvars(splev, "iiiio",
				       VA_PASS5((int64_t)(yyvsp[(9) - (10)].i), (int64_t)(yyvsp[(5) - (10)].i), (int64_t)(yyvsp[(3) - (10)].i),
						(int64_t)(yyvsp[(7) - (10)].i), SPO_ROOM_DOOR));
			}
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1469 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int64_t)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1483 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1487 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1497 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1504 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1512 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1517 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1522 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1527 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1532 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1537 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1542 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1550 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1557 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1581 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1591 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1599 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1605 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1613 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1632 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1638 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1644 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1650 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1655 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1660 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1665 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1670 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1675 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1680 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1685 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1690 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1695 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1700 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1705 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1710 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1716 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2(0xFFFFFFFFL, SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1722 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1727 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1732 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1737 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1742 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1747 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_LEVEL_BOSS));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1752 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_BOSS_HOSTILITY));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1757 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_HAS_BACKUP));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1764 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = ((int64_t)1 << (token - 1));
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1773 "lev_comp.y"
    {
		      (yyval.i) = (int64_t) ~0;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1777 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (3)].map));
		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (3)].map));

		      if (((int64_t)1 << (token - 1)) & (int64_t)((yyvsp[(3) - (3)].i)))
			  lc_error("Monster seen_traps, trap '%s' listed twice.", (yyvsp[(1) - (3)].map));
                      Free((yyvsp[(1) - (3)].map));
		      (yyval.i) = (((int64_t)1 << (token - 1)) | (int64_t)((yyvsp[(3) - (3)].i)));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1790 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1798 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1808 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1816 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1825 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1832 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1840 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1846 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1851 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1856 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1861 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1866 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1871 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1881 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1886 "lev_comp.y"
    {
		      if ((yyvsp[(1) - (1)].i) == D_LOCKED) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_LOCKED));
			  (yyval.i) = 0x0100;
		      } else if ((yyvsp[(1) - (1)].i) == D_BROKEN) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BROKEN));
			  (yyval.i) = 0x0200;
		      } else if ((yyvsp[(1) - (1)].i) == D_ISOPEN) {
			  add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_OPEN));
			  (yyval.i) = 0x2000000;
		      } else
			  lc_error("DOOR state can only be locked or broken.");
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1900 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1906 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1916 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1921 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1926 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1931 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1936 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1941 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1946 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1951 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1956 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1961 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1966 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1971 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1976 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1981 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1987 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 1992 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 1997 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2002 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2009 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2015 "lev_comp.y"
    {
		       int64_t dir, state = 0;

		       /* convert dir from a DIRECTION to a DB_DIR */
		       dir = (yyvsp[(5) - (7)].i);
		       switch (dir) {
		       case W_NORTH: dir = DB_NORTH; break;
		       case W_SOUTH: dir = DB_SOUTH; break;
		       case W_EAST:  dir = DB_EAST;  break;
		       case W_WEST:  dir = DB_WEST;  break;
		       default:
			   lc_error("Invalid drawbridge direction.");
			   break;
		       }

		       if ( (yyvsp[(7) - (7)].i) == D_ISOPEN )
			   state = 1;
		       else if ( (yyvsp[(7) - (7)].i) == D_CLOSED )
			   state = 0;
		       else if ( (yyvsp[(7) - (7)].i) == -1 )
			   state = -1;
		       else
			   lc_error("A drawbridge can only be open, closed or random!");
		       add_opvars(splev, "iio",
				  VA_PASS3(state, dir, SPO_DRAWBRIDGE));
		   ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2044 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2049 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2057 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2063 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2067 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2075 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2089 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (int64_t) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2109 "lev_comp.y"
    {
		      int64_t rtyp = 0;
		      switch((yyvsp[(6) - (6)].i)) {
		      case -1: rtyp = LR_TELE; break;
		      case  0: rtyp = LR_DOWNTELE; break;
		      case  1: rtyp = LR_UPTELE; break;
		      case  2: rtyp = LR_NOTELE; break;
		      }
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (6)].lregn).x1, (yyvsp[(3) - (6)].lregn).y1, (yyvsp[(3) - (6)].lregn).x2, (yyvsp[(3) - (6)].lregn).y2, (yyvsp[(3) - (6)].lregn).area,
					   (yyvsp[(5) - (6)].lregn).x1, (yyvsp[(5) - (6)].lregn).y1, (yyvsp[(5) - (6)].lregn).x2, (yyvsp[(5) - (6)].lregn).y2, (yyvsp[(5) - (6)].lregn).area,
					   rtyp, 0, (char *)0, SPO_LEVREGION));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2125 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (int64_t) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2135 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2145 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2157 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2161 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2167 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int) (yyvsp[(5) - (9)].i), (int) (yyvsp[(7) - (9)].i), (int) (yyvsp[(9) - (9)].i), SPO_MAGIC_PORTAL));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2173 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2179 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2186 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2194 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2200 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2215 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2220 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2226 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2232 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2237 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2247 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2257 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_L_V_PORTAL_TYPE));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2272 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2278 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2289 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2296 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2303 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2307 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2313 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2319 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2325 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2337 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2343 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (int64_t) LR_SPECIAL_MAP_SEEN : (int64_t) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2353 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2359 "lev_comp.y"
    {
		      int64_t irr;
		      int64_t rt = (yyvsp[(7) - (13)].i);
		      int64_t rflags = (yyvsp[(8) - (13)].i);
		      int64_t flmt = (int64_t)(yyvsp[(9) - (13)].i);
		      int64_t flt = (int64_t)(yyvsp[(10) - (13)].i);
		      int64_t tlset = (int64_t)(yyvsp[(11) - (13)].i);
		      int64_t decotyp = (int64_t)(yyvsp[(12) - (13)].i);

		      if (rflags == -1) rflags = ((int64_t)1 << 0);
		      //if (flmt == -1) flmt = 0;
		      //if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiiiio",
				 VA_PASS8((int64_t)(yyvsp[(5) - (13)].i), rt, rflags, flmt, flt, tlset, decotyp, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2380 "lev_comp.y"
    {
		      break_stmt_end(splev);
			  add_opcode(splev, SPO_ENDROOM, NULL);
		      /*if ( $<i>9 ||  $<i>10 ||  $<i>11 ) {
			  	add_opcode(splev, SPO_ENDROOM, NULL);
		       } else if ( $<i>12 )
			  	lc_error("Cannot use lev statements in non-permanent REGION");
		      */
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2392 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2396 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2402 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (int64_t)(yyvsp[(7) - (7)].i), (int64_t)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (int64_t)(yyvsp[(7) - (11)].i), (int64_t)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2412 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (int64_t)(yyvsp[(9) - (9)].i), (int64_t)(yyvsp[(7) - (9)].i), (int64_t)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int64_t)(yyvsp[(9) - (13)].i), (int64_t)(yyvsp[(7) - (13)].i), (int64_t)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2423 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2433 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2441 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2445 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2449 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2453 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2459 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2469 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2475 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2481 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2485 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2491 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2495 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2501 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2510 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2517 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2528 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2532 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2537 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2544 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2548 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2553 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2561 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2567 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int64_t)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2574 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2578 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2585 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
			int token = get_room_type((yyvsp[(1) - (1)].map));
			if (token == ERR) {
			    lc_warning("Unknown room type \"%s\"!  Making ordinary room...", (yyvsp[(1) - (1)].map));
				(yyval.i) = OROOM;
			} else
				(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2609 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2619 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2623 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 0);
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2634 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 1);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2638 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 2);
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2644 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2648 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2654 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2660 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2664 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2670 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2676 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2681 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2687 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2691 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2697 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2701 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2717 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2725 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2735 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2749 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2767 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2773 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2777 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2781 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2788 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2798 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2809 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2815 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2827 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2831 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2838 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2848 "lev_comp.y"
    {
		      int64_t r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2861 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2865 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2872 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2882 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2891 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2902 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2906 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2913 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2923 "lev_comp.y"
    {
                      int64_t m = get_monster_id((yyvsp[(1) - (1)].map), (char)0);
                      if (m == ERR) {
                          lc_error("Unknown monster \"%s\"!", (yyvsp[(1) - (1)].map));
                          (yyval.i) = -1;
                      } else
                          (yyval.i) = SP_MONST_PACK(m,
                                         def_monsyms[(int) mons[m].mlet].sym);
                      Free((yyvsp[(1) - (1)].map));
                  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2934 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2943 "lev_comp.y"
    {
                      int64_t m = get_monster_id((yyvsp[(4) - (5)].map), (char) (yyvsp[(2) - (5)].i));
                      if (m == ERR) {
                          lc_error("Unknown monster ('%c', \"%s\")!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].map));
                          (yyval.i) = -1;
                      } else
                          (yyval.i) = SP_MONST_PACK(m, (yyvsp[(2) - (5)].i));
                      Free((yyvsp[(4) - (5)].map));
                  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2953 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2959 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2963 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2970 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2980 "lev_comp.y"
    {
		      int64_t m = get_object_id((yyvsp[(1) - (1)].map), (char)0);
		      if (m == ERR) {
			  lc_error("Unknown object \"%s\"!", (yyvsp[(1) - (1)].map));
			  (yyval.i) = -1;
		      } else
                          /* obj class != 0 to force generation of a specific item */
                          (yyval.i) = SP_OBJ_PACK(m, 1);
                      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2991 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3000 "lev_comp.y"
    {
		      int64_t m = get_object_id((yyvsp[(4) - (5)].map), (char) (yyvsp[(2) - (5)].i));
		      if (m == ERR) {
			  lc_error("Unknown object ('%c', \"%s\")!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].map));
			  (yyval.i) = -1;
		      } else
			  (yyval.i) = SP_OBJ_PACK(m, (yyvsp[(2) - (5)].i));
                      Free((yyvsp[(4) - (5)].map));
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3010 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3014 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3020 "lev_comp.y"
    { ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3022 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3028 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3032 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3036 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3040 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3048 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3057 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3061 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3065 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3069 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3073 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3076 "lev_comp.y"
    { ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3080 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3087 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3096 "lev_comp.y"
    {
		      struct lc_funcdefs_parm *tmp = New(struct lc_funcdefs_parm);

		      if (!curr_function) {
			  lc_error("Function parameters outside function definition.");
		      } else if (!tmp) {
			  lc_error("Could not alloc function params.");
		      } else {
			  int64_t vt = SPOVAR_NULL;

			  tmp->name = strdup((yyvsp[(1) - (3)].map));
			  tmp->parmtype = (char) (yyvsp[(3) - (3)].i);
			  tmp->next = curr_function->params;
			  curr_function->params = tmp;
			  curr_function->n_params++;
			  switch (tmp->parmtype) {
			  case 'i':
                              vt = SPOVAR_INT;
                              break;
			  case 's':
                              vt = SPOVAR_STRING;
                              break;
			  default:
                              lc_error("Unknown func param conversion.");
                              break;
			  }
			  vardefs = add_vardef_type( vardefs, (yyvsp[(1) - (3)].map), vt);
		      }
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3137 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3141 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3148 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3155 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3165 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3169 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3177 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3181 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3185 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3189 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3193 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3198 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3202 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3206 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3211 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3216 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3221 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3225 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3230 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3235 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3239 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3243 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3248 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3252 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3259 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3265 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3269 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3275 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 3291 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3295 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3299 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3303 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 542:

/* Line 1455 of yacc.c  */
#line 3325 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 543:

/* Line 1455 of yacc.c  */
#line 3329 "lev_comp.y"
    {
			if ((yyvsp[(3) - (10)].i) <= 0 || (yyvsp[(3) - (10)].i) >= COLNO)
			    lc_error(
                          "Region (%ld,%ld,%ld,%ld) out of level range (x1)!",
                                     (yyvsp[(3) - (10)].i), (yyvsp[(5) - (10)].i), (yyvsp[(7) - (10)].i), (yyvsp[(9) - (10)].i));
			else if ((yyvsp[(5) - (10)].i) < 0 || (yyvsp[(5) - (10)].i) >= ROWNO)
			    lc_error(
                          "Region (%ld,%ld,%ld,%ld) out of level range (y1)!",
                                     (yyvsp[(3) - (10)].i), (yyvsp[(5) - (10)].i), (yyvsp[(7) - (10)].i), (yyvsp[(9) - (10)].i));
			else if ((yyvsp[(7) - (10)].i) <= 0 || (yyvsp[(7) - (10)].i) >= COLNO)
			    lc_error(
                          "Region (%ld,%ld,%ld,%ld) out of level range (x2)!",
                                     (yyvsp[(3) - (10)].i), (yyvsp[(5) - (10)].i), (yyvsp[(7) - (10)].i), (yyvsp[(9) - (10)].i));
			else if ((yyvsp[(9) - (10)].i) < 0 || (yyvsp[(9) - (10)].i) >= ROWNO)
			    lc_error(
                          "Region (%ld,%ld,%ld,%ld) out of level range (y2)!",
                                     (yyvsp[(3) - (10)].i), (yyvsp[(5) - (10)].i), (yyvsp[(7) - (10)].i), (yyvsp[(9) - (10)].i));
			(yyval.lregn).x1 = (yyvsp[(3) - (10)].i);
			(yyval.lregn).y1 = (yyvsp[(5) - (10)].i);
			(yyval.lregn).x2 = (yyvsp[(7) - (10)].i);
			(yyval.lregn).y2 = (yyvsp[(9) - (10)].i);
			(yyval.lregn).area = 1;
		  ;}
    break;

  case 544:

/* Line 1455 of yacc.c  */
#line 3355 "lev_comp.y"
    {
/* This series of if statements is a hack for MSC 5.1.  It seems that its
   tiny little brain cannot compile if these are all one big if statement. */
			if ((yyvsp[(2) - (9)].i) < 0 || (yyvsp[(2) - (9)].i) > (int) max_x_map)
			    lc_error(
                            "Region (%ld,%ld,%ld,%ld) out of map range (x1)!",
                                     (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));
			else if ((yyvsp[(4) - (9)].i) < 0 || (yyvsp[(4) - (9)].i) > (int) max_y_map)
			    lc_error(
                            "Region (%ld,%ld,%ld,%ld) out of map range (y1)!",
                                     (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));
			else if ((yyvsp[(6) - (9)].i) < 0 || (yyvsp[(6) - (9)].i) > (int) max_x_map)
			    lc_error(
                            "Region (%ld,%ld,%ld,%ld) out of map range (x2)!",
                                     (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));
			else if ((yyvsp[(8) - (9)].i) < 0 || (yyvsp[(8) - (9)].i) > (int) max_y_map)
			    lc_error(
                            "Region (%ld,%ld,%ld,%ld) out of map range (y2)!",
                                     (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));
			(yyval.lregn).area = 0;
			(yyval.lregn).x1 = (yyvsp[(2) - (9)].i);
			(yyval.lregn).y1 = (yyvsp[(4) - (9)].i);
			(yyval.lregn).x2 = (yyvsp[(6) - (9)].i);
			(yyval.lregn).y2 = (yyvsp[(8) - (9)].i);
		  ;}
    break;



/* Line 1455 of yacc.c  */
#line 7646 "lev_comp.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 3383 "lev_comp.y"


/*lev_comp.y*/

