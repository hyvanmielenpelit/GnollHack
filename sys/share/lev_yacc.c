
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
     MODRON_PORTAL_ID = 326,
     LEVEL_TELEPORTER_ID = 327,
     LEVEL_TELEPORT_DIRECTION_TYPE = 328,
     LEVEL_TELEPORT_END_TYPE = 329,
     POOL_ID = 330,
     SINK_ID = 331,
     NONE = 332,
     RAND_CORRIDOR_ID = 333,
     DOOR_STATE = 334,
     LIGHT_STATE = 335,
     CURSE_TYPE = 336,
     MYTHIC_TYPE = 337,
     ENGRAVING_TYPE = 338,
     KEYTYPE_ID = 339,
     LEVER_ID = 340,
     NO_PICKUP_ID = 341,
     DIRECTION = 342,
     RANDOM_TYPE = 343,
     RANDOM_TYPE_BRACKET = 344,
     A_REGISTER = 345,
     ALIGNMENT = 346,
     LEFT_OR_RIGHT = 347,
     CENTER = 348,
     TOP_OR_BOT = 349,
     ALTAR_TYPE = 350,
     ALTAR_SUBTYPE = 351,
     UP_OR_DOWN = 352,
     ACTIVE_OR_INACTIVE = 353,
     MODRON_PORTAL_TYPE = 354,
     NPC_TYPE = 355,
     FOUNTAIN_TYPE = 356,
     SPECIAL_OBJECT_TYPE = 357,
     CMAP_TYPE = 358,
     FLOOR_SUBTYPE = 359,
     FLOOR_SUBTYPE_ID = 360,
     FLOOR_ID = 361,
     FLOOR_TYPE = 362,
     FLOOR_TYPE_ID = 363,
     DECORATION_ID = 364,
     DECORATION_TYPE = 365,
     DECORATION_DIR = 366,
     DECORATION_ITEM_STATE = 367,
     PAINTING_TYPE = 368,
     BANNER_TYPE = 369,
     WALL_SCULPTURE_TYPE = 370,
     ELEMENTAL_ENCHANTMENT_TYPE = 371,
     EXCEPTIONALITY_TYPE = 372,
     EXCEPTIONALITY_ID = 373,
     ELEMENTAL_ENCHANTMENT_ID = 374,
     ENCHANTMENT_ID = 375,
     SECRET_DOOR_ID = 376,
     USES_UP_KEY_ID = 377,
     MYTHIC_PREFIX_TYPE = 378,
     MYTHIC_SUFFIX_TYPE = 379,
     MYTHIC_PREFIX_ID = 380,
     MYTHIC_SUFFIX_ID = 381,
     MATERIAL_ID = 382,
     MATERIAL_TYPE = 383,
     CHARGES_ID = 384,
     SPECIAL_QUALITY_ID = 385,
     SPEFLAGS_ID = 386,
     LEVEL_BOSS_ID = 387,
     BOSS_HOSTILITY_ID = 388,
     HAS_BACKUP_ID = 389,
     SUBROOM_ID = 390,
     NAME_ID = 391,
     FLAGS_ID = 392,
     FLAG_TYPE = 393,
     MON_ATTITUDE = 394,
     MON_ALERTNESS = 395,
     SUBTYPE_ID = 396,
     NON_PASSDOOR_ID = 397,
     CARPET_ID = 398,
     CARPET_PIECE_ID = 399,
     CARPET_TYPE = 400,
     MON_APPEARANCE = 401,
     ROOMDOOR_ID = 402,
     IF_ID = 403,
     ELSE_ID = 404,
     TERRAIN_ID = 405,
     HORIZ_OR_VERT = 406,
     REPLACE_TERRAIN_ID = 407,
     LOCATION_SUBTYPE_ID = 408,
     DOOR_SUBTYPE = 409,
     BRAZIER_SUBTYPE = 410,
     SIGNPOST_SUBTYPE = 411,
     TREE_SUBTYPE = 412,
     FOREST_ID = 413,
     FOREST_TYPE = 414,
     INITIALIZE_TYPE = 415,
     EXIT_ID = 416,
     SHUFFLE_ID = 417,
     MANUAL_TYPE_ID = 418,
     MANUAL_TYPE = 419,
     QUANTITY_ID = 420,
     BURIED_ID = 421,
     LOOP_ID = 422,
     FOR_ID = 423,
     TO_ID = 424,
     SWITCH_ID = 425,
     CASE_ID = 426,
     BREAK_ID = 427,
     DEFAULT_ID = 428,
     ERODED_ID = 429,
     TRAPPED_STATE = 430,
     RECHARGED_ID = 431,
     INVIS_ID = 432,
     GREASED_ID = 433,
     INDESTRUCTIBLE_ID = 434,
     FEMALE_ID = 435,
     MALE_ID = 436,
     WAITFORU_ID = 437,
     PROTECTOR_ID = 438,
     CANCELLED_ID = 439,
     REVIVED_ID = 440,
     AVENGE_ID = 441,
     FLEEING_ID = 442,
     BLINDED_ID = 443,
     MAXHP_ID = 444,
     LEVEL_ADJUSTMENT_ID = 445,
     KEEP_ORIGINAL_INVENTORY_ID = 446,
     PARALYZED_ID = 447,
     STUNNED_ID = 448,
     CONFUSED_ID = 449,
     SEENTRAPS_ID = 450,
     ALL_ID = 451,
     MONTYPE_ID = 452,
     OBJTYPE_ID = 453,
     TERTYPE_ID = 454,
     TERTYPE2_ID = 455,
     LEVER_EFFECT_TYPE = 456,
     SWITCHABLE_ID = 457,
     CONTINUOUSLY_USABLE_ID = 458,
     TARGET_ID = 459,
     TRAPTYPE_ID = 460,
     EFFECT_FLAG_ID = 461,
     GRAVE_ID = 462,
     BRAZIER_ID = 463,
     SIGNPOST_ID = 464,
     TREE_ID = 465,
     ERODEPROOF_ID = 466,
     FUNCTION_ID = 467,
     MSG_OUTPUT_TYPE = 468,
     COMPARE_TYPE = 469,
     UNKNOWN_TYPE = 470,
     rect_ID = 471,
     fillrect_ID = 472,
     line_ID = 473,
     randline_ID = 474,
     grow_ID = 475,
     selection_ID = 476,
     flood_ID = 477,
     rndcoord_ID = 478,
     circle_ID = 479,
     ellipse_ID = 480,
     filter_ID = 481,
     complement_ID = 482,
     gradient_ID = 483,
     GRADIENT_TYPE = 484,
     LIMITED = 485,
     HUMIDITY_TYPE = 486,
     STRING = 487,
     MAP_ID = 488,
     NQSTRING = 489,
     VARSTRING = 490,
     CFUNC = 491,
     CFUNC_INT = 492,
     CFUNC_STR = 493,
     CFUNC_COORD = 494,
     CFUNC_REGION = 495,
     VARSTRING_INT = 496,
     VARSTRING_INT_ARRAY = 497,
     VARSTRING_STRING = 498,
     VARSTRING_STRING_ARRAY = 499,
     VARSTRING_VAR = 500,
     VARSTRING_VAR_ARRAY = 501,
     VARSTRING_COORD = 502,
     VARSTRING_COORD_ARRAY = 503,
     VARSTRING_REGION = 504,
     VARSTRING_REGION_ARRAY = 505,
     VARSTRING_MAPCHAR = 506,
     VARSTRING_MAPCHAR_ARRAY = 507,
     VARSTRING_MONST = 508,
     VARSTRING_MONST_ARRAY = 509,
     VARSTRING_OBJ = 510,
     VARSTRING_OBJ_ARRAY = 511,
     VARSTRING_SEL = 512,
     VARSTRING_SEL_ARRAY = 513,
     METHOD_INT = 514,
     METHOD_INT_ARRAY = 515,
     METHOD_STRING = 516,
     METHOD_STRING_ARRAY = 517,
     METHOD_VAR = 518,
     METHOD_VAR_ARRAY = 519,
     METHOD_COORD = 520,
     METHOD_COORD_ARRAY = 521,
     METHOD_REGION = 522,
     METHOD_REGION_ARRAY = 523,
     METHOD_MAPCHAR = 524,
     METHOD_MAPCHAR_ARRAY = 525,
     METHOD_MONST = 526,
     METHOD_MONST_ARRAY = 527,
     METHOD_OBJ = 528,
     METHOD_OBJ_ARRAY = 529,
     METHOD_SEL = 530,
     METHOD_SEL_ARRAY = 531,
     DICE = 532
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
#line 577 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 589 "lev_comp.tab.c"

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
#define YYLAST   1387

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  295
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  541
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1218

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   532

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   290,   294,     2,
     234,   235,   288,   286,   232,   287,   292,   289,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   233,     2,
       2,   291,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   236,     2,   237,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   238,   293,   239,     2,     2,     2,     2,
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
     225,   226,   227,   228,   229,   230,   231,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285
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
     297,   299,   301,   305,   309,   315,   319,   325,   331,   337,
     341,   345,   351,   357,   363,   371,   379,   387,   393,   395,
     399,   401,   405,   407,   411,   413,   417,   419,   423,   425,
     429,   431,   435,   436,   437,   446,   451,   453,   454,   456,
     458,   464,   468,   469,   470,   480,   481,   484,   485,   491,
     492,   497,   499,   502,   504,   511,   512,   516,   517,   524,
     525,   530,   531,   536,   538,   539,   544,   548,   560,   578,
     580,   584,   588,   594,   600,   608,   613,   614,   630,   631,
     649,   650,   653,   659,   661,   667,   669,   675,   677,   683,
     685,   696,   703,   705,   707,   709,   711,   713,   717,   719,
     721,   722,   726,   730,   734,   738,   740,   742,   744,   746,
     748,   756,   762,   764,   766,   768,   770,   774,   775,   781,
     786,   787,   791,   793,   795,   797,   799,   802,   804,   806,
     808,   810,   812,   816,   818,   822,   826,   830,   832,   834,
     838,   840,   842,   844,   846,   850,   852,   854,   856,   858,
     860,   862,   866,   870,   871,   877,   880,   881,   885,   887,
     891,   893,   897,   901,   903,   905,   909,   911,   913,   915,
     919,   921,   923,   925,   929,   933,   937,   941,   945,   949,
     953,   957,   959,   961,   963,   965,   969,   973,   977,   981,
     987,   995,  1001,  1010,  1012,  1016,  1020,  1026,  1032,  1040,
    1048,  1055,  1061,  1062,  1065,  1071,  1075,  1087,  1101,  1108,
    1109,  1113,  1115,  1119,  1123,  1127,  1131,  1133,  1135,  1139,
    1143,  1147,  1151,  1155,  1159,  1161,  1163,  1165,  1167,  1171,
    1175,  1177,  1183,  1193,  1199,  1207,  1211,  1215,  1221,  1231,
    1237,  1243,  1249,  1250,  1266,  1267,  1269,  1277,  1289,  1299,
    1313,  1317,  1325,  1337,  1351,  1361,  1371,  1381,  1389,  1397,
    1403,  1411,  1417,  1427,  1435,  1441,  1451,  1457,  1463,  1467,
    1475,  1479,  1487,  1495,  1499,  1505,  1511,  1515,  1521,  1529,
    1539,  1541,  1543,  1545,  1547,  1549,  1550,  1553,  1555,  1559,
    1561,  1563,  1565,  1566,  1570,  1572,  1573,  1577,  1579,  1580,
    1584,  1585,  1589,  1590,  1594,  1596,  1598,  1600,  1602,  1604,
    1606,  1608,  1610,  1612,  1616,  1618,  1620,  1625,  1627,  1629,
    1634,  1636,  1638,  1643,  1645,  1650,  1656,  1658,  1662,  1664,
    1668,  1670,  1672,  1677,  1687,  1689,  1691,  1696,  1698,  1704,
    1706,  1708,  1713,  1715,  1717,  1723,  1725,  1727,  1729,  1734,
    1736,  1738,  1744,  1746,  1748,  1750,  1754,  1756,  1758,  1762,
    1764,  1769,  1773,  1777,  1781,  1785,  1789,  1793,  1795,  1797,
    1801,  1803,  1807,  1808,  1810,  1812,  1814,  1816,  1820,  1821,
    1823,  1825,  1828,  1831,  1836,  1843,  1848,  1855,  1862,  1869,
    1876,  1879,  1886,  1895,  1904,  1915,  1930,  1933,  1935,  1939,
    1941,  1945,  1947,  1949,  1951,  1953,  1955,  1957,  1959,  1961,
    1963,  1965,  1967,  1969,  1971,  1973,  1975,  1977,  1979,  1981,
    1983,  1994
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     296,     0,    -1,    -1,   297,    -1,   298,    -1,   298,   297,
      -1,   299,   309,   311,    -1,    21,   233,   240,    -1,    20,
     233,   240,   232,     3,    -1,    20,   233,   240,   232,     3,
     232,   449,    -1,    22,   233,    11,   232,   409,    -1,    22,
     233,    10,   232,     3,    -1,    22,   233,    13,    -1,    22,
     233,    12,   232,     3,   232,     3,   232,     5,   232,     5,
     232,   449,   232,   308,   307,    -1,    23,   233,   479,    -1,
     158,   233,   159,   232,   160,    -1,   158,   233,   159,    -1,
      36,   233,     4,   232,   321,    -1,    26,   233,   107,    -1,
      -1,   232,   230,    -1,    -1,   232,   456,    -1,    -1,   232,
       3,    -1,     5,    -1,    88,    -1,    -1,   137,   233,   310,
      -1,   138,   232,   310,    -1,   138,    -1,    -1,   313,   311,
      -1,   238,   311,   239,    -1,   354,    -1,   300,    -1,   301,
      -1,   417,    -1,   304,    -1,   302,    -1,   425,    -1,   426,
      -1,   427,    -1,   303,    -1,   424,    -1,   423,    -1,   421,
      -1,   422,    -1,   428,    -1,   429,    -1,   430,    -1,   431,
      -1,   432,    -1,   398,    -1,   356,    -1,   319,    -1,   318,
      -1,   412,    -1,   368,    -1,   390,    -1,   434,    -1,   435,
      -1,   400,    -1,   401,    -1,   402,    -1,   433,    -1,   334,
      -1,   344,    -1,   346,    -1,   350,    -1,   348,    -1,   331,
      -1,   341,    -1,   327,    -1,   330,    -1,   393,    -1,   403,
      -1,   375,    -1,   391,    -1,   378,    -1,   384,    -1,   413,
      -1,   408,    -1,   396,    -1,   355,    -1,   414,    -1,   415,
      -1,   416,    -1,   418,    -1,   361,    -1,   359,    -1,   407,
      -1,   411,    -1,   410,    -1,   394,    -1,   395,    -1,   397,
      -1,   389,    -1,   392,    -1,   250,    -1,   252,    -1,   254,
      -1,   256,    -1,   258,    -1,   260,    -1,   262,    -1,   264,
      -1,   266,    -1,   249,    -1,   251,    -1,   253,    -1,   255,
      -1,   257,    -1,   259,    -1,   261,    -1,   263,    -1,   265,
      -1,   314,    -1,   315,    -1,   243,    -1,   243,    -1,   315,
      -1,   162,   233,   314,    -1,   316,   291,   468,    -1,   316,
     291,   221,   233,   477,    -1,   316,   291,   467,    -1,   316,
     291,   484,   233,   461,    -1,   316,   291,   483,   233,   463,
      -1,   316,   291,   482,   233,   465,    -1,   316,   291,   456,
      -1,   316,   291,   459,    -1,   316,   291,   238,   325,   239,
      -1,   316,   291,   238,   324,   239,    -1,   316,   291,   238,
     323,   239,    -1,   316,   291,   484,   233,   238,   322,   239,
      -1,   316,   291,   483,   233,   238,   321,   239,    -1,   316,
     291,   482,   233,   238,   320,   239,    -1,   316,   291,   238,
     326,   239,    -1,   466,    -1,   320,   232,   466,    -1,   464,
      -1,   321,   232,   464,    -1,   462,    -1,   322,   232,   462,
      -1,   460,    -1,   323,   232,   460,    -1,   457,    -1,   324,
     232,   457,    -1,   468,    -1,   325,   232,   468,    -1,   467,
      -1,   326,   232,   467,    -1,    -1,    -1,   212,   242,   234,
     328,   472,   235,   329,   312,    -1,   242,   234,   475,   235,
      -1,   161,    -1,    -1,     6,    -1,     6,    -1,   236,   468,
     214,   468,   237,    -1,   236,   468,   237,    -1,    -1,    -1,
     170,   335,   236,   455,   237,   336,   238,   337,   239,    -1,
      -1,   338,   337,    -1,    -1,   171,   480,   233,   339,   311,
      -1,    -1,   173,   233,   340,   311,    -1,   172,    -1,   292,
     292,    -1,   169,    -1,   168,   317,   291,   468,   342,   468,
      -1,    -1,   343,   345,   312,    -1,    -1,   167,   236,   455,
     237,   347,   312,    -1,    -1,   333,   233,   349,   313,    -1,
      -1,   148,   333,   351,   352,    -1,   312,    -1,    -1,   312,
     353,   149,   312,    -1,    14,   233,   467,    -1,    14,   233,
     467,   232,    15,   232,    16,   232,    17,   232,     4,    -1,
      14,   233,   467,   232,    15,   232,    16,   232,    17,   232,
      18,   232,    19,   232,     4,   232,     4,    -1,    78,    -1,
      78,   233,   480,    -1,    78,   233,    88,    -1,    66,   233,
     357,   232,   357,    -1,    66,   233,   357,   232,   480,    -1,
     234,     4,   232,    87,   232,   372,   235,    -1,   437,   332,
     232,   449,    -1,    -1,   135,   233,   358,   232,   365,   232,
     367,   438,   441,   443,   446,   447,   445,   360,   312,    -1,
      -1,    58,   233,   358,   232,   364,   232,   366,   232,   367,
     438,   441,   443,   446,   447,   445,   362,   312,    -1,    -1,
     232,     5,    -1,   234,     4,   232,     4,   235,    -1,    88,
      -1,   234,     4,   232,     4,   235,    -1,    88,    -1,   234,
     376,   232,   377,   235,    -1,    88,    -1,   234,     4,   232,
       4,   235,    -1,    88,    -1,   147,   233,   369,   232,   448,
     232,   370,   232,   372,   373,    -1,    34,   233,   448,   232,
     477,   373,    -1,     5,    -1,    88,    -1,   371,    -1,    88,
      -1,    87,    -1,    87,   293,   371,    -1,     4,    -1,    88,
      -1,    -1,   373,   232,   374,    -1,   153,   233,   154,    -1,
      84,   233,   465,    -1,   130,   233,     4,    -1,   179,    -1,
     121,    -1,   122,    -1,   142,    -1,    25,    -1,    24,   233,
     376,   232,   377,   363,   241,    -1,    24,   233,   456,   363,
     241,    -1,    92,    -1,    93,    -1,    94,    -1,    93,    -1,
      32,   233,   380,    -1,    -1,    32,   233,   380,   379,   312,
      -1,   463,   232,   456,   381,    -1,    -1,   381,   232,   382,
      -1,   467,    -1,   139,    -1,   140,    -1,   451,    -1,   146,
     467,    -1,   180,    -1,   181,    -1,   177,    -1,   184,    -1,
     185,    -1,   185,   233,   455,    -1,   186,    -1,   187,   233,
     455,    -1,   188,   233,   455,    -1,   192,   233,   455,    -1,
     193,    -1,   194,    -1,   195,   233,   383,    -1,   195,    -1,
     182,    -1,   183,    -1,   189,    -1,   190,   233,   455,    -1,
     191,    -1,   132,    -1,   133,    -1,   134,    -1,   240,    -1,
     196,    -1,   240,   293,   383,    -1,    30,   233,   386,    -1,
      -1,    31,   233,   386,   385,   312,    -1,   465,   387,    -1,
      -1,   387,   232,   388,    -1,    81,    -1,   197,   233,   463,
      -1,   481,    -1,   136,   233,   467,    -1,   165,   233,   455,
      -1,   166,    -1,    80,    -1,   174,   233,   455,    -1,   211,
      -1,    79,    -1,   175,    -1,   176,   233,   455,    -1,   177,
      -1,   178,    -1,   456,    -1,   119,   233,   116,    -1,   118,
     233,   117,    -1,   120,   233,   455,    -1,   129,   233,   455,
      -1,   130,   233,   455,    -1,   163,   233,   164,    -1,   131,
     233,   455,    -1,    84,   233,   465,    -1,   179,    -1,   122,
      -1,    86,    -1,    82,    -1,   125,   233,   123,    -1,   126,
     233,   124,    -1,    65,   233,   455,    -1,   127,   233,   128,
      -1,    33,   233,   436,   232,   456,    -1,    35,   233,   456,
     232,    87,   232,   448,    -1,    40,   233,   456,   232,    87,
      -1,    40,   233,   456,   232,    87,   232,     5,   307,    -1,
      41,    -1,    41,   233,   477,    -1,    41,   233,   196,    -1,
      54,   233,   456,   232,    97,    -1,    55,   233,   456,   232,
      97,    -1,    55,   233,   486,   232,   486,   232,    97,    -1,
      60,   233,   486,   232,   486,   232,   240,    -1,    61,   233,
     486,   232,   486,   399,    -1,    62,   233,   486,   232,   486,
      -1,    -1,   232,    97,    -1,    69,   233,   477,   232,   101,
      -1,    70,   233,   477,    -1,    71,   233,   456,   232,   204,
     233,   456,   232,    99,   232,    98,    -1,    71,   233,   456,
     232,    72,   232,    73,   232,    74,   232,    99,   232,    98,
      -1,    85,   233,   456,   232,   201,   404,    -1,    -1,   404,
     232,   405,    -1,    98,    -1,   197,   233,   463,    -1,   198,
     233,   465,    -1,   199,   233,   409,    -1,   200,   233,   409,
      -1,   202,    -1,   203,    -1,   204,   233,   456,    -1,   205,
     233,   436,    -1,   153,   233,   406,    -1,   105,   233,   444,
      -1,   206,   233,     4,    -1,   130,   233,     4,    -1,     4,
      -1,    99,    -1,   154,    -1,   101,    -1,    76,   233,   477,
      -1,    75,   233,   477,    -1,     3,    -1,   234,     3,   232,
     449,   235,    -1,   152,   233,   459,   232,   461,   232,   461,
     232,     7,    -1,   150,   233,   477,   232,   461,    -1,   150,
     233,   477,   232,   461,   232,   104,    -1,    56,   233,   459,
      -1,    57,   233,   459,    -1,    46,   233,   240,   232,    47,
      -1,    46,   233,   240,   232,    47,   232,   197,   233,   463,
      -1,    43,   233,   459,   232,    45,    -1,    44,   233,   486,
     232,    45,    -1,    27,   233,   477,   232,   103,    -1,    -1,
      42,   233,   459,   232,   449,   232,   437,   438,   441,   443,
     446,   447,   445,   419,   420,    -1,    -1,   312,    -1,    51,
     233,   456,   232,   450,   232,   452,    -1,    51,   233,   456,
     232,   450,   232,   452,   232,   197,   233,   463,    -1,    51,
     233,   456,   232,   450,   232,   452,   232,    96,    -1,    51,
     233,   456,   232,   450,   232,   452,   232,    96,   232,   197,
     233,   463,    -1,    52,   233,   456,    -1,    52,   233,   456,
     232,   197,   233,   463,    -1,   109,   233,   456,   232,   110,
     232,     4,   232,   111,   232,   112,    -1,   109,   233,   456,
     232,   110,   232,     4,   232,   111,   232,   112,   232,   449,
      -1,   109,   233,   456,   232,   110,   232,   113,   232,   111,
      -1,   109,   233,   456,   232,   110,   232,   114,   232,   111,
      -1,   109,   233,   456,   232,   110,   232,   115,   232,   111,
      -1,   109,   233,   456,   232,   110,   232,   111,    -1,   106,
     233,   477,   232,   107,   232,   104,    -1,   141,   233,   477,
     232,     4,    -1,   141,   233,   477,   232,   107,   232,   104,
      -1,   143,   233,   459,   232,   145,    -1,   144,   233,   477,
     232,   145,   232,     4,   232,     4,    -1,   144,   233,   477,
     232,   145,   232,     4,    -1,    53,   233,   100,   232,   456,
      -1,    53,   233,   100,   232,   456,   232,   197,   233,   463,
      -1,   207,   233,   456,   232,   467,    -1,   207,   233,   456,
     232,    88,    -1,   207,   233,   456,    -1,   208,   233,   456,
     232,   155,   232,   449,    -1,   208,   233,   456,    -1,   209,
     233,   456,   232,   156,   232,   467,    -1,   209,   233,   456,
     232,   156,   232,    88,    -1,   209,   233,   456,    -1,   210,
     233,   456,   232,   157,    -1,   210,   233,   456,   232,   159,
      -1,   210,   233,   456,    -1,    67,   233,   468,   232,   456,
      -1,    68,   233,   456,   232,   485,   232,   467,    -1,    64,
     233,   455,   232,   455,   232,   455,   232,   455,    -1,    64,
      -1,   240,    -1,    88,    -1,   240,    -1,    88,    -1,    -1,
     232,   439,    -1,   440,    -1,   440,   232,   439,    -1,    48,
      -1,    49,    -1,    50,    -1,    -1,   108,   233,   442,    -1,
     107,    -1,    -1,   105,   233,   444,    -1,   104,    -1,    -1,
     197,   233,   463,    -1,    -1,    28,   233,   103,    -1,    -1,
      29,   233,     4,    -1,    79,    -1,    88,    -1,    80,    -1,
      88,    -1,    91,    -1,   453,    -1,    88,    -1,    91,    -1,
     453,    -1,    90,   233,    88,    -1,    95,    -1,    88,    -1,
      90,   236,     4,   237,    -1,   240,    -1,   251,    -1,   252,
     236,   468,   237,    -1,   468,    -1,   457,    -1,   223,   234,
     477,   235,    -1,   255,    -1,   256,   236,   468,   237,    -1,
     234,     4,   232,     4,   235,    -1,    88,    -1,    89,   458,
     237,    -1,   231,    -1,   231,   232,   458,    -1,   460,    -1,
     257,    -1,   258,   236,   468,   237,    -1,   234,     4,   232,
       4,   232,     4,   232,     4,   235,    -1,   462,    -1,   259,
      -1,   260,   236,   468,   237,    -1,     3,    -1,   234,     3,
     232,   449,   235,    -1,   464,    -1,   261,    -1,   262,   236,
     468,   237,    -1,   240,    -1,     3,    -1,   234,     3,   232,
     240,   235,    -1,    88,    -1,   466,    -1,   263,    -1,   264,
     236,   468,   237,    -1,   240,    -1,     3,    -1,   234,     3,
     232,   240,   235,    -1,    88,    -1,   102,    -1,   454,    -1,
     467,   292,   454,    -1,     4,    -1,   478,    -1,   234,     8,
     235,    -1,   249,    -1,   250,   236,   468,   237,    -1,   468,
     286,   468,    -1,   468,   287,   468,    -1,   468,   288,   468,
      -1,   468,   289,   468,    -1,   468,   290,   468,    -1,   234,
     468,   235,    -1,   245,    -1,   246,    -1,   316,   233,   469,
      -1,   470,    -1,   471,   232,   470,    -1,    -1,   471,    -1,
     468,    -1,   467,    -1,   473,    -1,   474,   232,   473,    -1,
      -1,   474,    -1,   456,    -1,   216,   459,    -1,   217,   459,
      -1,   218,   456,   232,   456,    -1,   219,   456,   232,   456,
     232,   468,    -1,   220,   234,   477,   235,    -1,   220,   234,
     371,   232,   477,   235,    -1,   226,   234,     7,   232,   477,
     235,    -1,   226,   234,   477,   232,   477,   235,    -1,   226,
     234,   461,   232,   477,   235,    -1,   222,   456,    -1,   224,
     234,   456,   232,   468,   235,    -1,   224,   234,   456,   232,
     468,   232,    48,   235,    -1,   225,   234,   456,   232,   468,
     232,   468,   235,    -1,   225,   234,   456,   232,   468,   232,
     468,   232,    48,   235,    -1,   228,   234,   229,   232,   234,
     468,   287,   468,   305,   235,   232,   456,   306,   235,    -1,
     227,   476,    -1,   265,    -1,   234,   477,   235,    -1,   476,
      -1,   476,   294,   477,    -1,   285,    -1,   103,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   478,    -1,    37,    -1,    30,    -1,    38,
      -1,    32,    -1,    39,    -1,   150,    -1,    83,    -1,    88,
      -1,   487,    -1,    63,   234,     4,   232,     4,   232,     4,
     232,     4,   235,    -1,   234,     4,   232,     4,   232,     4,
     232,     4,   235,    -1
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
     583,   584,   585,   586,   587,   588,   589,   590,   591,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   605,   606,
     607,   608,   609,   610,   611,   612,   613,   616,   617,   618,
     621,   622,   625,   641,   647,   653,   659,   665,   671,   677,
     683,   689,   699,   709,   719,   729,   739,   749,   761,   766,
     773,   778,   785,   790,   797,   801,   807,   812,   819,   823,
     829,   833,   840,   862,   839,   876,   931,   938,   941,   947,
     954,   958,   967,   971,   966,  1034,  1035,  1039,  1038,  1052,
    1051,  1066,  1076,  1077,  1080,  1118,  1117,  1152,  1151,  1182,
    1181,  1214,  1213,  1239,  1250,  1249,  1277,  1281,  1285,  1291,
    1296,  1301,  1308,  1315,  1324,  1332,  1344,  1343,  1369,  1368,
    1394,  1397,  1403,  1413,  1419,  1428,  1434,  1439,  1445,  1450,
    1456,  1467,  1473,  1474,  1477,  1478,  1481,  1485,  1491,  1492,
    1496,  1502,  1510,  1515,  1520,  1525,  1530,  1535,  1540,  1548,
    1555,  1563,  1571,  1572,  1575,  1576,  1579,  1584,  1583,  1597,
    1604,  1611,  1619,  1624,  1630,  1636,  1642,  1648,  1653,  1658,
    1663,  1668,  1673,  1678,  1683,  1688,  1693,  1698,  1703,  1708,
    1714,  1720,  1725,  1730,  1735,  1740,  1745,  1750,  1755,  1762,
    1771,  1775,  1788,  1797,  1796,  1814,  1824,  1830,  1838,  1844,
    1849,  1854,  1859,  1864,  1869,  1874,  1879,  1884,  1898,  1904,
    1909,  1914,  1919,  1924,  1929,  1934,  1939,  1944,  1949,  1954,
    1959,  1964,  1969,  1974,  1979,  1985,  1990,  1995,  2000,  2007,
    2013,  2042,  2047,  2055,  2061,  2065,  2073,  2080,  2087,  2097,
    2107,  2123,  2134,  2137,  2143,  2149,  2155,  2159,  2165,  2172,
    2178,  2186,  2192,  2197,  2202,  2207,  2212,  2218,  2224,  2229,
    2234,  2239,  2244,  2249,  2256,  2256,  2256,  2256,  2259,  2265,
    2271,  2276,  2283,  2290,  2294,  2300,  2306,  2312,  2317,  2324,
    2330,  2340,  2347,  2346,  2380,  2383,  2389,  2394,  2399,  2404,
    2410,  2414,  2420,  2424,  2428,  2432,  2436,  2440,  2446,  2452,
    2456,  2462,  2468,  2472,  2478,  2482,  2488,  2492,  2497,  2504,
    2508,  2515,  2519,  2524,  2531,  2535,  2540,  2548,  2554,  2561,
    2565,  2572,  2580,  2583,  2593,  2597,  2600,  2606,  2610,  2617,
    2621,  2625,  2632,  2635,  2641,  2648,  2651,  2657,  2664,  2668,
    2675,  2678,  2685,  2688,  2694,  2695,  2698,  2699,  2702,  2703,
    2704,  2710,  2711,  2712,  2718,  2719,  2722,  2731,  2736,  2743,
    2754,  2760,  2764,  2768,  2775,  2785,  2792,  2796,  2802,  2806,
    2814,  2818,  2825,  2835,  2848,  2852,  2859,  2869,  2878,  2889,
    2893,  2900,  2910,  2921,  2930,  2940,  2946,  2950,  2957,  2967,
    2978,  2987,  2997,  3001,  3008,  3009,  3015,  3019,  3023,  3027,
    3035,  3044,  3048,  3052,  3056,  3060,  3064,  3067,  3074,  3083,
    3116,  3117,  3120,  3121,  3124,  3128,  3135,  3142,  3153,  3156,
    3164,  3168,  3172,  3176,  3180,  3185,  3189,  3193,  3198,  3203,
    3208,  3212,  3217,  3222,  3226,  3230,  3235,  3239,  3246,  3252,
    3256,  3262,  3269,  3270,  3273,  3274,  3275,  3278,  3282,  3286,
    3290,  3296,  3297,  3300,  3301,  3304,  3305,  3308,  3309,  3312,
    3316,  3342
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
  "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID", "MODRON_PORTAL_ID",
  "LEVEL_TELEPORTER_ID", "LEVEL_TELEPORT_DIRECTION_TYPE",
  "LEVEL_TELEPORT_END_TYPE", "POOL_ID", "SINK_ID", "NONE",
  "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE", "CURSE_TYPE",
  "MYTHIC_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID",
  "NO_PICKUP_ID", "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET",
  "A_REGISTER", "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT",
  "ALTAR_TYPE", "ALTAR_SUBTYPE", "UP_OR_DOWN", "ACTIVE_OR_INACTIVE",
  "MODRON_PORTAL_TYPE", "NPC_TYPE", "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE",
  "CMAP_TYPE", "FLOOR_SUBTYPE", "FLOOR_SUBTYPE_ID", "FLOOR_ID",
  "FLOOR_TYPE", "FLOOR_TYPE_ID", "DECORATION_ID", "DECORATION_TYPE",
  "DECORATION_DIR", "DECORATION_ITEM_STATE", "PAINTING_TYPE",
  "BANNER_TYPE", "WALL_SCULPTURE_TYPE", "ELEMENTAL_ENCHANTMENT_TYPE",
  "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID",
  "ENCHANTMENT_ID", "SECRET_DOOR_ID", "USES_UP_KEY_ID",
  "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE", "MYTHIC_PREFIX_ID",
  "MYTHIC_SUFFIX_ID", "MATERIAL_ID", "MATERIAL_TYPE", "CHARGES_ID",
  "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "LEVEL_BOSS_ID",
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
  "throne_detail", "modron_portal_detail", "lever_detail", "lever_infos",
  "lever_info", "valid_subtype", "sink_detail", "pool_detail",
  "terrain_type", "replace_terrain_detail", "terrain_detail",
  "diggable_detail", "passwall_detail", "naming_detail",
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
     485,   486,    44,    58,    40,    41,    91,    93,   123,   125,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,    43,    45,    42,    47,
      37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   295,   296,   296,   297,   297,   298,   299,   299,   299,
     300,   300,   300,   300,   301,   302,   302,   303,   304,   305,
     305,   306,   306,   307,   307,   308,   308,   309,   309,   310,
     310,   311,   311,   312,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   316,   316,   316,
     317,   317,   318,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   320,   320,
     321,   321,   322,   322,   323,   323,   324,   324,   325,   325,
     326,   326,   328,   329,   327,   330,   331,   332,   332,   333,
     333,   333,   335,   336,   334,   337,   337,   339,   338,   340,
     338,   341,   342,   342,   343,   345,   344,   347,   346,   349,
     348,   351,   350,   352,   353,   352,   354,   354,   354,   355,
     355,   355,   356,   356,   357,   358,   360,   359,   362,   361,
     363,   363,   364,   364,   365,   365,   366,   366,   367,   367,
     368,   368,   369,   369,   370,   370,   371,   371,   372,   372,
     373,   373,   374,   374,   374,   374,   374,   374,   374,   375,
     375,   375,   376,   376,   377,   377,   378,   379,   378,   380,
     381,   381,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   383,
     383,   383,   384,   385,   384,   386,   387,   387,   388,   388,
     388,   388,   388,   388,   388,   388,   388,   388,   388,   388,
     388,   388,   388,   388,   388,   388,   388,   388,   388,   388,
     388,   388,   388,   388,   388,   388,   388,   388,   388,   389,
     390,   391,   391,   392,   392,   392,   393,   394,   395,   396,
     397,   398,   399,   399,   400,   401,   402,   402,   403,   404,
     404,   405,   405,   405,   405,   405,   405,   405,   405,   405,
     405,   405,   405,   405,   406,   406,   406,   406,   407,   408,
     409,   409,   410,   411,   411,   412,   413,   414,   414,   415,
     416,   417,   419,   418,   420,   420,   421,   421,   421,   421,
     422,   422,   423,   423,   423,   423,   423,   423,   424,   425,
     425,   426,   427,   427,   428,   428,   429,   429,   429,   430,
     430,   431,   431,   431,   432,   432,   432,   433,   434,   435,
     435,   436,   436,   437,   437,   438,   438,   439,   439,   440,
     440,   440,   441,   441,   442,   443,   443,   444,   445,   445,
     446,   446,   447,   447,   448,   448,   449,   449,   450,   450,
     450,   451,   451,   451,   452,   452,   453,   454,   454,   454,
     455,   456,   456,   456,   456,   457,   457,   457,   458,   458,
     459,   459,   459,   460,   461,   461,   461,   462,   462,   463,
     463,   463,   464,   464,   464,   464,   465,   465,   465,   466,
     466,   466,   466,   466,   467,   467,   468,   468,   468,   468,
     468,   468,   468,   468,   468,   468,   468,   469,   469,   470,
     471,   471,   472,   472,   473,   473,   474,   474,   475,   475,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   477,
     477,   478,   479,   479,   480,   480,   480,   481,   481,   481,
     481,   482,   482,   483,   483,   484,   484,   485,   485,   486,
     486,   487
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
       1,     1,     3,     3,     5,     3,     5,     5,     5,     3,
       3,     5,     5,     5,     7,     7,     7,     5,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     0,     0,     8,     4,     1,     0,     1,     1,
       5,     3,     0,     0,     9,     0,     2,     0,     5,     0,
       4,     1,     2,     1,     6,     0,     3,     0,     6,     0,
       4,     0,     4,     1,     0,     4,     3,    11,    17,     1,
       3,     3,     5,     5,     7,     4,     0,    15,     0,    17,
       0,     2,     5,     1,     5,     1,     5,     1,     5,     1,
      10,     6,     1,     1,     1,     1,     1,     3,     1,     1,
       0,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       7,     5,     1,     1,     1,     1,     3,     0,     5,     4,
       0,     3,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     3,     1,     3,     3,     3,     1,     1,     3,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     3,     3,     0,     5,     2,     0,     3,     1,     3,
       1,     3,     3,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     1,     1,     3,     3,     3,     3,     5,
       7,     5,     8,     1,     3,     3,     5,     5,     7,     7,
       6,     5,     0,     2,     5,     3,    11,    13,     6,     0,
       3,     1,     3,     3,     3,     3,     1,     1,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     3,     3,
       1,     5,     9,     5,     7,     3,     3,     5,     9,     5,
       5,     5,     0,    15,     0,     1,     7,    11,     9,    13,
       3,     7,    11,    13,     9,     9,     9,     7,     7,     5,
       7,     5,     9,     7,     5,     9,     5,     5,     3,     7,
       3,     7,     7,     3,     5,     5,     3,     5,     7,     9,
       1,     1,     1,     1,     1,     0,     2,     1,     3,     1,
       1,     1,     0,     3,     1,     0,     3,     1,     0,     3,
       0,     3,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     4,     1,     1,     4,
       1,     1,     4,     1,     4,     5,     1,     3,     1,     3,
       1,     1,     4,     9,     1,     1,     4,     1,     5,     1,
       1,     4,     1,     1,     5,     1,     1,     1,     4,     1,
       1,     5,     1,     1,     1,     3,     1,     1,     3,     1,
       4,     3,     3,     3,     3,     3,     3,     1,     1,     3,
       1,     3,     0,     1,     1,     1,     1,     3,     0,     1,
       1,     2,     2,     4,     6,     4,     6,     6,     6,     6,
       2,     6,     8,     8,    10,    14,     2,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
      10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    27,     0,     0,     1,
       5,     0,    31,     0,     7,     0,   159,     0,     0,     0,
       0,   229,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     156,     0,     0,     0,   162,   171,     0,     0,     0,     0,
       0,     0,     0,   119,   108,    99,   109,   100,   110,   101,
     111,   102,   112,   103,   113,   104,   114,   105,   115,   106,
     116,   107,    35,    36,    39,    43,    38,     6,    31,   117,
     118,     0,    56,    55,    73,    74,    71,     0,    66,    72,
     175,    67,    68,    70,    69,    34,    84,    54,    90,    89,
      58,    77,    79,    80,    97,    59,    78,    98,    75,    94,
      95,    83,    96,    53,    62,    63,    64,    76,    91,    82,
      93,    92,    57,    81,    85,    86,    87,    37,    88,    46,
      47,    45,    44,    40,    41,    42,    48,    49,    50,    51,
      52,    65,    60,    61,     0,    30,    28,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
       0,   120,   121,     0,     0,     0,     0,     0,     0,     0,
     476,     0,   479,     0,   521,     0,   477,   498,    32,     0,
     179,     0,     8,     0,   437,   438,     0,   474,   186,     0,
       0,     0,    12,   523,   522,    14,   446,     0,   232,   233,
       0,     0,   443,     0,     0,   200,   441,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     517,   500,   519,     0,   470,   472,   473,     0,   469,   467,
       0,   272,   276,   466,   273,   463,   465,     0,   462,   460,
       0,   236,     0,   459,   402,   401,     0,   424,   425,     0,
       0,     0,     0,   315,   314,     0,   451,     0,     0,   450,
       0,     0,     0,     0,   539,     0,     0,   370,     0,     0,
       0,     0,     0,   355,   356,   404,   403,     0,   157,     0,
       0,     0,     0,   440,     0,     0,     0,     0,     0,   325,
       0,   349,   348,   526,   524,   525,   191,   190,     0,     0,
       0,     0,     0,     0,     0,   212,   213,     0,     0,     0,
       0,    16,   122,     0,     0,     0,   388,   390,   393,   396,
     152,     0,     0,     0,     0,   161,     0,     0,     0,     0,
       0,   495,   494,   496,   499,     0,   532,   534,   531,   533,
     535,   536,     0,     0,     0,   129,   130,   125,   123,     0,
       0,     0,     0,    31,   176,     0,    29,     0,     0,     0,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,   501,   502,     0,     0,     0,   510,     0,     0,     0,
     516,     0,     0,     0,     0,     0,     0,   275,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   182,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,   492,   478,   486,     0,
       0,   481,   482,   483,   484,   485,     0,   155,     0,   476,
       0,     0,     0,     0,   146,   144,   150,   148,     0,     0,
       0,   180,     0,   426,   427,     9,     0,     0,   475,    11,
     350,     0,    10,     0,     0,   447,     0,     0,     0,   235,
     234,   200,   201,   231,     0,     0,   216,     0,     0,     0,
       0,   457,     0,     0,   455,     0,     0,   454,     0,     0,
     518,   520,   361,     0,     0,     0,   274,     0,     0,   238,
     240,   309,   220,     0,    17,   140,   311,     0,     0,     0,
     359,     0,     0,   360,   357,   430,     0,   428,     0,   429,
       0,   384,   316,     0,   317,     0,   203,     0,     0,     0,
       0,   322,   321,     0,     0,   192,   193,   397,   537,   538,
       0,   324,     0,     0,   329,     0,     0,   205,     0,     0,
     379,     0,   381,     0,     0,     0,     0,   353,     0,    15,
       0,   173,     0,     0,   163,   387,   386,     0,     0,   394,
     395,     0,   490,   493,     0,   480,   160,   497,   124,     0,
       0,   133,     0,   132,     0,   131,     0,   137,     0,   128,
       0,   127,     0,   126,    33,   439,     0,     0,     0,   449,
     442,     0,   444,     0,   503,     0,     0,     0,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   468,   529,
     527,   528,     0,   287,   284,   278,   304,     0,   303,     0,
       0,     0,   302,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   283,     0,   288,     0,   290,   291,   301,     0,
     286,   277,   292,   530,   280,     0,   461,   239,   211,     0,
       0,     0,     0,   452,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   195,     0,     0,   320,
       0,     0,     0,     0,     0,   328,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,   172,   174,     0,
       0,     0,     0,     0,   153,     0,   145,   147,   149,   151,
       0,   138,     0,     0,   142,     0,     0,     0,   445,   230,
       0,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   464,
       0,     0,   310,   141,    23,     0,   405,     0,     0,     0,
       0,   435,   434,   366,   371,     0,   318,     0,   207,     0,
       0,   319,   323,     0,     0,   398,     0,     0,     0,   378,
       0,   377,     0,     0,     0,     0,   209,     0,   405,   380,
     383,   215,     0,   214,   185,   354,     0,   165,   389,   392,
     391,   487,   488,   489,   491,     0,     0,   136,   135,     0,
     134,     0,     0,     0,   504,   506,     0,   511,     0,   507,
       0,   456,   509,   508,     0,   307,   300,   294,   293,   295,
     305,   306,   308,   296,   297,   299,   281,   298,   282,   285,
     289,   279,     0,   431,   266,   267,   268,   243,   244,     0,
     249,   247,   248,   261,   262,   250,   251,   253,     0,     0,
     263,     0,   265,     0,   257,   258,   260,   241,   245,   432,
     242,     0,   226,   227,     0,   228,     0,   225,   221,     0,
     312,     0,     0,   412,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,   218,   219,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,   336,   337,     0,
       0,     0,   330,     0,     0,     0,     0,     0,     0,   412,
       0,     0,     0,     0,     0,     0,   165,   154,   139,   143,
       0,   351,     0,     0,     0,   458,     0,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
     409,   410,   411,   406,   407,     0,   415,     0,     0,   358,
     368,     0,   385,   202,     0,   405,   399,   194,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   374,   375,   376,   204,     0,   415,   382,   220,   352,
       0,   169,   164,   166,     0,     0,   512,     0,   513,   482,
     433,   252,   254,   255,   264,   256,   270,   269,   259,   223,
     224,   222,     0,     0,     0,     0,   420,     0,     0,     0,
       0,     0,   412,     0,     0,   417,   341,   343,   344,   345,
     347,   346,   340,   332,   333,   334,   335,   338,   339,   342,
       0,     0,   420,   210,   167,    31,   187,     0,     0,     0,
       0,     0,     0,   453,   408,   414,   413,     0,     0,   422,
       0,   541,     0,   367,   206,   415,     0,   326,   372,   208,
     422,    31,   170,     0,     0,   514,    20,     0,   271,   416,
       0,     0,   418,   540,     0,   420,     0,     0,   418,   168,
       0,     0,     0,   421,     0,     0,   362,   369,   422,   327,
     373,   196,     0,     0,    21,   423,     0,   364,   418,     0,
       0,    25,    26,    23,     0,     0,   419,   365,   363,   198,
     197,     0,    13,    22,   515,     0,   188,   199
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1141,  1205,   970,  1203,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   800,   594,   803,   530,   531,
     532,   533,   114,   516,   895,   115,   116,   485,   117,   118,
     234,   789,  1015,  1016,  1161,  1135,   119,   653,   120,   121,
     251,   122,   650,   123,   422,   124,   378,   505,   645,   125,
     126,   127,   355,   347,   128,  1199,   129,  1215,   440,   618,
     639,   860,   878,   130,   377,   882,   567,   986,   748,   968,
     131,   274,   561,   132,   461,   311,   747,   957,  1098,   133,
     458,   301,   457,   741,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   769,   144,   145,   146,   147,   775,
    1002,  1122,   148,   149,   552,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1197,  1208,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   973,  1043,  1044,  1046,  1146,  1106,  1116,
    1186,  1149,  1172,   319,   545,   608,   958,   853,   609,   257,
     352,   291,   276,   434,   328,   329,   576,   577,   312,   313,
     302,   303,   401,   353,   893,   662,   663,   664,   403,   404,
     405,   292,   452,   246,   265,   367,   744,   419,   420,   421,
     630,   333,   334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1076
static const yytype_int16 yypact[] =
{
     134,  -138,  -123,   130, -1076,   134,     3,   -65,   -50, -1076,
   -1076,   -12,   920,   -15, -1076,    81, -1076,    -8,     5,    70,
      72, -1076,    99,   101,   119,   125,   135,   139,   143,   154,
     156,   158,   172,   180,   186,   206,   209,   211,   234,   235,
     241,   255,   269,   270,   280,   284,   288,   291,   292,   293,
     297,   306,   307,   308,   309,   312,   314,   320,   321,   324,
     325,   326,   330,   337,   338,   339,     7,   340,   342,   346,
   -1076,   349,     0,   781, -1076, -1076,   352,   358,   360,   361,
      88,    48,    20, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,   920, -1076,
   -1076,    23, -1076, -1076, -1076, -1076, -1076,   366, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076,   378,   165, -1076,  -132,   449,    75,
     -25,   296,   447,   164,   164,   322,   -47,    37,   -16,   502,
     -16,   528,  -140,  -140,   -24,   315,   -16,   -16,   431,   -16,
     -28,  -140,  -140,   -46,   -24,   -24,   -24,    48,   299,    48,
     -16,   447,   447,   -16,   447,   447,    83,   -16,   447,   -16,
     -46,   447,  -140,   447,    85, -1076,   447,  -140,   419,   783,
      48, -1076, -1076,   305,   365,   -16,   -16,   -16,   -16,   369,
   -1076,    21, -1076,   368, -1076,   300, -1076,    39, -1076,   144,
   -1076,   367,   377,    81, -1076, -1076,   374, -1076,  -146,   382,
     383,   388, -1076, -1076, -1076, -1076, -1076,   393, -1076, -1076,
     402,   623, -1076,   394,   405,   421, -1076, -1076,  -140,  -140,
     -16,   -16,   422,   -16,   428,   442,   443,   447,   446,   273,
   -1076, -1076,   395,   455, -1076, -1076, -1076,   687, -1076, -1076,
     456, -1076, -1076, -1076, -1076, -1076, -1076,   688, -1076, -1076,
     462,   461,   468, -1076, -1076, -1076,   472, -1076, -1076,   473,
     479,   481,   489, -1076, -1076,   718, -1076,   487,   493, -1076,
     494,   501,   723,   496, -1076,   505,   508,   509,   510,   525,
     754,   529,   531, -1076, -1076, -1076, -1076,   533,   760,   541,
     542,   546,   550,   396,   756,   555,   148,   556,   557, -1076,
     558, -1076, -1076, -1076, -1076, -1076, -1076, -1076,   559,   560,
     562,   563,   565,   572,   578, -1076, -1076,   579,   367,   580,
     582,   585, -1076,   581,    48,    48,   588,   590,   600,   608,
   -1076,   609,   196,    48,    48, -1076,    48,    48,    48,    48,
      48,   553,   396, -1076,   614,   615, -1076, -1076, -1076, -1076,
   -1076, -1076,   616,   105,    10, -1076, -1076,   553,   396,   618,
     624,   625,   920,   920, -1076,    69, -1076,    48,   844,  -132,
     857,    33,   871,   643,   639,   447,   645,    48,   354,   873,
     644, -1076, -1076,   654,   655,   423, -1076,   -16,   -16,   198,
   -1076,   661,   656,   447,   799,   671,    48,   672,   367,   675,
      48,   367,   -16,   -16,   447,   821,   104,   822,   678,    48,
      69,   866,   908,   681,   869,   868,   238,   719,   -16,   820,
     689,   823,   -24,   -43, -1076,   690,   -24,   -24,   -24,    48,
     691,    46,   -16,   170,   824,    12,   726,   817,   818,   -37,
      45,   784,   785,    37,   782, -1076,    41,    41,   772, -1076,
     -41,   696,   -40,   786,   780,   -33,   757, -1076, -1076,   278,
     407,   -86,   -86, -1076, -1076, -1076,    39, -1076,   447,   705,
    -127,  -110,  -105,   -88, -1076, -1076,   553,   396,   111,   282,
      97, -1076,   699, -1076, -1076, -1076,   512,   707, -1076, -1076,
   -1076,   937, -1076,   716,   393, -1076,   722,   954,   548, -1076,
   -1076,   421, -1076, -1076,   -16,   -16,   674,   727,   730,   736,
     737, -1076,   738,   253, -1076,   743,   751, -1076,   753,   761,
   -1076, -1076, -1076,   752,   566,   650, -1076,   759,   593, -1076,
   -1076, -1076, -1076,   762,   765, -1076,   769,   998,   606,   796,
   -1076,   816,  1000, -1076,   819, -1076,   795, -1076,   825, -1076,
     826,   828, -1076,  1046, -1076,   830, -1076,  1048,   833,    69,
     834,   837, -1076,   839,   967, -1076, -1076, -1076, -1076, -1076,
     841, -1076,   842,   843, -1076,   845,   847, -1076,  1052,   848,
   -1076,   851, -1076,   852,   853,   909,  1072,   854,   859, -1076,
     367, -1076,   797,    48, -1076, -1076,   553,   861,   862, -1076,
   -1076,   863, -1076,   865,   860, -1076, -1076, -1076, -1076,  1094,
     870, -1076,   -14, -1076,    48, -1076,  -132, -1076,    78, -1076,
     104, -1076,    44, -1076, -1076, -1076,  1083,   874,  1097, -1076,
   -1076,   872, -1076,   864, -1076,   876,  1016,   447, -1076,    48,
      48,   447,   877,    48,   447,   447,   878,   875, -1076, -1076,
   -1076, -1076,   880, -1076, -1076, -1076, -1076,   881, -1076,   882,
     883,   884, -1076,   885,   886,   887,   888,   889,   890,   891,
     892,   893, -1076,   898, -1076,   900, -1076, -1076, -1076,   901,
   -1076, -1076, -1076, -1076, -1076,   902, -1076,   879,   903,    37,
     104,  1131,   906, -1076,   -46,  1135,   910,   943,  1137,    65,
     322,   949,   131,  1050,   916,   -26, -1076,   912,  1056, -1076,
      48,   917,  -132,  1081,   -16,   923,  1054,    74,   927,   -19,
    1057,  1156,   363,   367,  1060,    41, -1076, -1076,   396,   928,
      69,   -17,   -83,   757, -1076,   177, -1076, -1076,   396,   553,
     -74, -1076,   -10,   132, -1076,   933,    69,   935, -1076, -1076,
      48, -1076,   952,   345,   262,   955,    69,   611,   972,   973,
      48, -1076,    48,   164,  1051,  1073,    48,  1086,  1087,  1082,
      48,    48,    48,  -132,  1049,    48,    48,    48,   322, -1076,
    1011,   439, -1076, -1076,   980,  1210,   983,   984,  1213,   985,
     982, -1076, -1076,   988, -1076,   989, -1076,  1217, -1076,   171,
     991, -1076, -1076,   992,    49,   553,   993,   994,   666, -1076,
     995, -1076,   996,   997,   999,  1226, -1076,  1228,   983, -1076,
    1001, -1076,  1002, -1076, -1076, -1076,  1003,    13, -1076, -1076,
     553, -1076, -1076, -1076, -1076,   367,    78, -1076, -1076,    44,
   -1076,  1219,  1004,  1232,   396, -1076,  1190, -1076,    48, -1076,
    1005, -1076, -1076, -1076,   430, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076,   553, -1076, -1076, -1076,
   -1076, -1076,   197, -1076, -1076, -1076, -1076, -1076, -1076,  -132,
   -1076, -1076, -1076, -1076, -1076, -1076,  1008, -1076,  1009,  1010,
   -1076,  1012, -1076,  1013, -1076, -1076,  1014, -1076, -1076, -1076,
     553,  1015, -1076, -1076,  1017, -1076,  1019, -1076, -1076,  1241,
   -1076,  1021,   298,  1141,  1250,  1023,   322, -1076,   -20,   322,
    1022,  1024,   -19,    48, -1076, -1076,  1025,  1184,  1160, -1076,
    1028,  1031,  1032,  1033,  1034,  1035,  1036, -1076, -1076,  1037,
    1038,  1039, -1076,  1162,  1163,  1164,  1165,  1042,  1047,  1141,
    1274,    49,  1273,   437,  1053,  1043,    13, -1076, -1076, -1076,
    1055, -1076,  1058,  1059,   420, -1076,    48,  1193,   553,    48,
      48,    48,    48,    48,  -107,   164,  1279,  1130, -1076,  1281,
   -1076, -1076, -1076, -1076,  1061,  1062,  1183,  1064,  1285, -1076,
    1065,  1066, -1076, -1076,   354,   983, -1076, -1076,  1068,  1069,
    1187,  1288,    42,   322,   164,    33,    33,   -16,   -47,  1294,
    1070, -1076, -1076, -1076, -1076,  1299,  1183, -1076, -1076, -1076,
    1071, -1076, -1076, -1076,    93,  1300, -1076,  1258, -1076,   176,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076,  1018, -1076, -1076,
   -1076, -1076,  1074,   298,  1200,  1075,  1282,  1308,  1078,  1117,
     322,  1080,  1141,  1218,  1220, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
    1204,  1084,  1282,   903, -1076,   920, -1076,  1088,  1089,  1090,
    1092,  1091,  -107, -1076, -1076, -1076, -1076,  1187,  1095,  1295,
    1096, -1076,  1099, -1076, -1076,  1183,  1098, -1076,  1101, -1076,
    1295,   920, -1076,  1304,    69, -1076, -1076,  1102, -1076, -1076,
    1224,  1103,  1132, -1076,   322,  1282,  1237,    69,  1132, -1076,
    1105,  1106,   -16, -1076,  1335,  1107, -1076, -1076,  1295, -1076,
   -1076, -1076,  1337,    91,  1110, -1076,   322,   367,  1132,   367,
    1111, -1076, -1076,   980,   -16,  1109, -1076, -1076, -1076, -1076,
   -1076,  1341, -1076, -1076, -1076,   367, -1076, -1076
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1076, -1076,  1342, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076,   145, -1076, -1076,  1093,  -108,  -376,   929,  1120,
    1277,  -501, -1076, -1076, -1076, -1076,   673, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,  1286, -1076,
   -1076, -1076,   341, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076,   867,  1134, -1076, -1076, -1076, -1076,   794, -1076,
   -1076, -1076,   379, -1076, -1076, -1076,  -613,   348,   285, -1076,
   -1076,   497,   310, -1076, -1076, -1076, -1076, -1076,   218, -1076,
   -1076,  1178, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076,  -610, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076, -1076,
   -1076,   301,   612,  -841,   264, -1076,  -944, -1076,  -996,   221,
   -1075, -1044, -1067,  -473,  -463, -1076, -1076, -1076,   530,   936,
    -224,  -179,  -391,   827,   640,  -398,  -474,  -658,  -536,  -454,
    -530,  -659,  -172,   -77, -1076,   583, -1076, -1076,   846, -1076,
   -1076,  1100,   -76,   788, -1076,  -481, -1076, -1076, -1076, -1076,
   -1076,  -178, -1076
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -238
static const yytype_int16 yytable[] =
{
     248,   275,   504,   681,   245,   258,   383,   599,   679,   320,
     626,   322,   595,    16,   240,   661,   535,   336,   337,   801,
     339,   341,   342,   534,   804,   240,   349,   350,   351,   391,
     644,   357,   647,   648,   360,   331,   550,  1009,   368,   331,
     370,   314,   345,   240,   571,   616,  1118,   571,   655,   640,
     363,   637,   240,   984,   364,   365,   386,   387,   388,   389,
     266,   267,   858,   266,   267,  1076,   683,   268,   269,   876,
     415,   889,   266,   267,   266,   267,  1050,   417,   870,   263,
    1132,   294,   586,   811,   632,   589,   428,   363,  1160,  1096,
     375,   364,   365,  1178,   325,     7,  1201,  1136,   266,   267,
     571,   443,   444,  1191,   446,   670,   293,   305,   254,   529,
       8,  1137,   671,   391,   294,   324,   317,   326,   327,   255,
     256,  1198,   672,  1209,   659,   318,   660,   674,   651,   673,
       9,  1188,   356,  1097,   675,   358,   359,   985,   361,   362,
      11,  1119,   369,  1120,   676,   372,   429,   374,   240,   543,
     379,   677,   641,   851,     1,     2,   766,   544,   896,  1175,
     852,   511,   891,   892,   392,   897,   295,   294,  1155,   883,
     402,   366,   418,   376,   406,    13,   407,  1051,   264,  1202,
     296,   408,   409,   410,  1013,   871,  1014,   872,   873,   874,
      14,   617,   306,   315,   346,   270,  1121,   638,   270,   295,
     254,   571,   398,   399,   400,   572,   340,   270,   859,   271,
     332,   255,   256,   296,  1112,   877,   633,   174,   271,   175,
     271,    15,   750,   254,   854,   177,   595,   272,   273,   898,
     272,   273,   266,   267,   255,   256,   230,  1018,   178,   272,
     273,  1019,   536,    81,   413,   396,   397,   398,   399,   400,
     254,   652,   295,   628,   247,   241,   702,   436,   629,   242,
     243,   255,   256,   268,   269,   623,   296,   551,   569,   570,
     242,   243,   796,   241,   786,   646,   842,   436,   646,   254,
     354,   797,   241,   590,   591,   305,   266,   267,   242,   243,
     255,   256,   661,   916,   411,   244,   843,   242,   243,   611,
     574,   575,   931,   179,   615,   180,   244,   510,   620,   621,
     622,   886,   297,   627,   249,   542,   519,   520,   298,   521,
     522,   523,   524,   525,   244,   305,   605,   888,   606,   607,
     239,   646,   181,   244,   182,   682,   392,   537,   307,   241,
     656,   266,   267,   902,   308,   297,  1040,  1041,  1042,   678,
     546,   298,   183,   910,   242,   243,   574,   575,   184,   556,
     558,   266,   267,   848,   899,   412,   808,   270,   185,   568,
     306,   900,   186,   578,   299,   300,   187,   581,   413,   584,
     492,   252,   414,   588,   254,   694,   695,   188,   592,   189,
     244,   190,   598,   242,   243,   255,   256,   253,   297,   272,
     273,   326,   327,   277,   298,   191,   742,   884,  1140,   845,
     306,   -19,   808,   192,   278,   279,   280,   281,   282,   193,
     283,   270,   284,   285,   286,   287,   288,   299,   300,   244,
    1027,   518,   573,   758,   396,   397,   398,   399,   400,   194,
    1049,   363,   195,  1052,   196,   364,   365,   559,   560,   402,
     566,   881,   668,   272,   273,  1125,  1126,   574,   575,   259,
     260,   261,   262,   290,   398,   399,   400,   197,   198,   278,
     279,   280,   281,   282,   199,   283,   270,   284,   285,   286,
     287,   288,   396,   397,   398,   399,   400,   289,   200,   278,
     279,   280,   281,   282,   908,   283,   270,   284,   285,   286,
     287,   288,   201,   202,   799,  1099,   321,   289,   272,   273,
     566,   266,   267,   203,   394,   665,   307,   204,   290,  1017,
     680,   205,   308,   961,   206,   207,   208,  1123,   272,   273,
     209,   338,  1080,   354,  1124,   266,   267,   395,   290,   210,
     211,   212,   213,   309,   310,   214,   863,   215,   396,   397,
     398,   399,   400,   216,   217,   335,   307,   218,   219,   220,
     962,   963,   308,   221,   396,   397,   398,   399,   400,   964,
     222,   223,   224,   226,  1153,   227,   788,   906,   381,   228,
     907,   965,   229,   309,   310,   235,   396,   397,   398,   399,
     400,   236,   966,   237,   238,   867,   384,   798,   915,   250,
     865,   385,   919,   390,   393,   423,   923,   924,   925,   425,
     427,   928,   929,   930,   430,   431,   266,   267,   967,   890,
     432,   812,   813,   814,   433,   815,   817,   436,   818,   819,
     437,   396,   397,   398,   399,   400,   435,   438,  1187,   278,
     279,   280,   281,   282,   666,   283,   270,   284,   285,   286,
     287,   288,  1087,   439,   709,  1088,   445,   289,   710,   711,
    1206,   926,   447,   278,   279,   280,   281,   282,   960,   283,
     270,   284,   285,   286,   287,   288,   448,   449,   272,   273,
     451,   289,   396,   397,   398,   399,   400,   454,   290,   453,
     455,   459,   456,   396,   397,   398,   399,   400,   460,  -237,
     462,  1181,   272,   273,   463,   464,   396,   397,   398,   399,
     400,   465,   290,   466,  1190,   712,   396,  1026,   398,   399,
     400,   467,   468,   469,   323,   470,   471,   473,   474,   713,
     714,   715,   716,   904,   717,   472,   718,   475,   266,   267,
     476,   477,   478,   914,   278,   279,   280,   281,   282,   685,
     283,   270,   284,   285,   286,   287,   288,   479,   480,  1056,
     490,   481,   289,   482,   989,   483,   484,  1028,   719,   720,
     721,   990,   722,   486,   487,   723,   724,   725,   488,   726,
     727,   728,   489,   272,   273,   692,   729,   491,   493,   494,
     495,   496,   497,   290,   498,   499,   991,   500,   396,   397,
     398,   399,   400,   708,   501,  1091,  1092,  1093,  1094,  1095,
     502,   503,   506,   730,   507,   731,   732,   508,   509,   992,
     512,  1207,   513,  1210,   733,   734,   735,   736,   737,   738,
     746,  1024,   514,   330,   396,   397,   398,   399,   400,  1217,
     515,   343,   344,   753,   517,   429,   526,   739,   911,   528,
     527,   538,   396,   397,   398,   399,   400,   539,   540,   547,
     549,   740,   373,   993,   994,   995,   996,   380,   997,   998,
     999,  1000,  1001,   270,   553,   554,   555,   557,   562,   396,
     397,   398,   399,   400,   271,   563,   564,   565,  1127,   416,
     579,   580,   396,   397,   398,   399,   400,   396,   397,   398,
     399,   400,   582,   583,   585,   272,   273,   587,   593,   596,
     597,   600,   601,   602,   603,   604,   610,   612,   441,   442,
     614,   613,   619,   624,   635,   631,    16,   634,   636,   642,
     643,  -184,   649,   654,    17,   244,   658,   669,   684,   686,
     687,   657,    18,    19,    20,    21,    22,    23,   688,  1089,
      24,    25,    26,    27,    28,    29,    30,   690,   691,   697,
      31,    32,    33,    34,    35,   698,    36,   696,   699,   700,
     701,    37,    38,    39,    40,    41,    42,    43,    44,   703,
      45,    46,    47,   704,    48,   705,    49,    50,    51,    52,
      53,    54,   707,   706,   749,    55,    56,   750,    57,   745,
      83,   751,   752,  1194,   756,    58,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   231,  1213,    59,  1162,   754,    60,
      84,   758,    86,    85,    88,    87,    90,    89,    92,    91,
      94,    93,    96,    95,    98,    97,   100,    99,   755,   101,
     762,   757,   764,  1179,   771,    61,   778,   759,   783,   760,
     761,    62,   763,    63,    64,   765,   767,    65,    66,   768,
      67,   770,    68,   772,   773,   702,   774,   776,    69,   777,
     779,    70,    71,   780,   781,   782,   784,    72,    73,   787,
      74,   785,    75,   790,   791,   794,   792,   793,   795,   805,
     807,   932,   933,   566,   325,   809,   806,   808,   810,   816,
     821,   840,   820,   822,   823,   824,   825,   826,   827,   828,
     829,   830,   831,   832,   833,   834,   835,    76,    77,    78,
      79,   836,    80,   837,   838,   841,   844,   839,   845,   847,
     849,   850,   848,   934,   935,   936,   855,   856,   857,   864,
     937,   938,   861,   862,   866,   868,    81,   939,   869,   875,
     880,   879,    82,    83,   885,   901,   887,   903,   917,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   905,   940,   918,
     909,   941,   942,   943,   944,   945,   946,   947,   948,   949,
     950,   951,   952,   953,   954,   955,   956,   912,   913,   920,
     922,   921,   969,   927,   971,   972,   974,   975,   976,   977,
     978,   980,   979,   982,   983,   987,   988,  1003,  1004,  1005,
    1007,  1006,  1008,  1010,  1011,  1012,  1020,  1022,  1023,  1021,
    1025,  1029,  1030,  1031,  1038,  1032,  1033,  1034,  1035,  1045,
    1036,   254,  1037,  1039,  1047,  1048,  1054,  1053,  1058,  1059,
    1057,  1060,   255,   256,  1061,  1062,  1063,  1064,  1065,  1066,
    1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,  1077,  1075,
    1079,  1090,  1082,  1100,  1101,  1102,  1081,  1084,  1105,  1108,
    1085,  1115,  1117,  1103,  1086,  1104,  1107,  1109,  1129,  1110,
    1113,  1114,  1130,  1131,  1134,  1138,  1139,  1145,  1147,  1143,
    1148,  1142,  1150,  1151,  1152,  1154,  1158,  1156,  1157,  1159,
    1163,  1164,  1166,  1180,  1171,  1165,  1167,  1183,  1170,  1185,
    1176,  1173,  1174,  1177,  1182,  1189,  1184,  1192,  1193,  1195,
    1196,  1200,  1204,  1211,  1214,  1216,   426,    10,  1212,   382,
     232,   541,   225,   802,   371,   693,   981,  1083,   625,  1078,
    1168,  1055,   304,  1133,  1111,   548,   846,  1144,  1169,  1128,
     959,     0,   667,   743,     0,     0,   894,     0,     0,     0,
       0,   689,     0,     0,     0,     0,     0,   450
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   539,    81,   177,   230,   470,   538,   188,
     491,   190,   466,     6,     4,   516,   414,   196,   197,   678,
     199,   200,   200,   414,   682,     4,   204,   205,   206,     8,
     503,   210,   506,   507,   213,    63,     3,   878,   217,    63,
     219,    88,    88,     4,     3,    88,     4,     3,    88,     4,
       4,    88,     4,     4,     8,     9,   235,   236,   237,   238,
      88,    89,    88,    88,    89,  1009,   540,    92,    93,    88,
     249,    88,    88,    89,    88,    89,    96,   249,     4,     4,
    1076,     3,   458,   696,    72,   461,   232,     4,  1132,   196,
       5,     8,     9,  1160,   234,   233,     5,     4,    88,    89,
       3,   280,   281,  1178,   283,   232,   182,     3,   240,     4,
     233,    18,   239,     8,     3,   191,    79,   257,   258,   251,
     252,  1188,   232,  1198,   157,    88,   159,   232,   169,   239,
       0,  1175,   209,   240,   239,   211,   212,    88,   214,   215,
     137,    99,   218,   101,   232,   221,   292,   223,     4,    80,
     226,   239,   107,    88,    20,    21,   619,    88,   232,  1155,
      95,   385,   245,   246,   241,   239,    88,     3,  1112,   782,
     247,    88,   249,    88,    30,   240,    32,   197,   103,    88,
     102,    37,    38,    39,   171,   111,   173,   113,   114,   115,
     240,   234,    88,   240,   240,   223,   154,   234,   223,    88,
     240,     3,   288,   289,   290,     7,   234,   223,   234,   234,
     234,   251,   252,   102,  1055,   234,   204,   232,   234,   138,
     234,   233,   232,   240,   760,   233,   680,   255,   256,   239,
     255,   256,    88,    89,   251,   252,   236,   896,   233,   255,
     256,   899,   414,   236,   234,   286,   287,   288,   289,   290,
     240,   292,    88,    83,   234,   234,     3,     4,    88,   249,
     250,   251,   252,    92,    93,   489,   102,   234,   447,   448,
     249,   250,   670,   234,   650,   234,   749,     4,   234,   240,
     234,   672,   234,   462,   463,     3,    88,    89,   249,   250,
     251,   252,   793,   823,   150,   285,   750,   249,   250,   478,
     259,   260,   838,   233,   482,   233,   285,   384,   486,   487,
     488,   785,   234,   492,   291,   423,   393,   394,   240,   396,
     397,   398,   399,   400,   285,     3,    88,   790,    90,    91,
     242,   234,   233,   285,   233,   238,   413,   414,   234,   234,
     512,    88,    89,   806,   240,   234,    48,    49,    50,   238,
     427,   240,   233,   816,   249,   250,   259,   260,   233,   435,
     437,    88,    89,   232,   232,   221,   235,   223,   233,   445,
      88,   239,   233,   449,   263,   264,   233,   453,   234,   456,
     232,     3,   238,   460,   240,   564,   565,   233,   464,   233,
     285,   233,   469,   249,   250,   251,   252,   232,   234,   255,
     256,   257,   258,   107,   240,   233,   585,   783,   232,   232,
      88,   235,   235,   233,   216,   217,   218,   219,   220,   233,
     222,   223,   224,   225,   226,   227,   228,   263,   264,   285,
     233,   235,   234,   236,   286,   287,   288,   289,   290,   233,
     976,     4,   233,   979,   233,     8,     9,    93,    94,   526,
      87,    88,   528,   255,   256,  1065,  1066,   259,   260,    10,
      11,    12,    13,   265,   288,   289,   290,   233,   233,   216,
     217,   218,   219,   220,   233,   222,   223,   224,   225,   226,
     227,   228,   286,   287,   288,   289,   290,   234,   233,   216,
     217,   218,   219,   220,   232,   222,   223,   224,   225,   226,
     227,   228,   233,   233,   676,  1035,     4,   234,   255,   256,
      87,    88,    89,   233,   214,   237,   234,   233,   265,   895,
     238,   233,   240,    84,   233,   233,   233,  1063,   255,   256,
     233,   100,  1013,   234,  1064,    88,    89,   237,   265,   233,
     233,   233,   233,   261,   262,   233,   770,   233,   286,   287,
     288,   289,   290,   233,   233,   240,   234,   233,   233,   233,
     121,   122,   240,   233,   286,   287,   288,   289,   290,   130,
     233,   233,   233,   233,  1110,   233,   653,   232,   159,   233,
     235,   142,   233,   261,   262,   233,   286,   287,   288,   289,
     290,   233,   153,   233,   233,   774,   291,   674,   822,   233,
     772,   236,   826,   234,   236,   238,   830,   831,   832,   232,
     236,   835,   836,   837,   232,   232,    88,    89,   179,   791,
     232,   697,   699,   700,   231,   701,   703,     4,   704,   705,
     236,   286,   287,   288,   289,   290,   234,   232,  1174,   216,
     217,   218,   219,   220,   237,   222,   223,   224,   225,   226,
     227,   228,   232,   232,     4,   235,   234,   234,     8,     9,
    1196,   833,   234,   216,   217,   218,   219,   220,   840,   222,
     223,   224,   225,   226,   227,   228,   234,   234,   255,   256,
     234,   234,   286,   287,   288,   289,   290,   232,   265,   294,
       3,     3,   236,   286,   287,   288,   289,   290,   236,   238,
     232,  1164,   255,   256,   232,   232,   286,   287,   288,   289,
     290,   232,   265,   232,  1177,    65,   286,   287,   288,   289,
     290,   232,     4,   236,   196,   232,   232,     4,   232,    79,
      80,    81,    82,   810,    84,   234,    86,   232,    88,    89,
     232,   232,   232,   820,   216,   217,   218,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   232,     4,   983,
       4,   232,   234,   232,    98,   232,     6,   939,   118,   119,
     120,   105,   122,   232,   232,   125,   126,   127,   232,   129,
     130,   131,   232,   255,   256,   237,   136,   232,   232,   232,
     232,   232,   232,   265,   232,   232,   130,   232,   286,   287,
     288,   289,   290,   237,   232,  1029,  1030,  1031,  1032,  1033,
     232,   232,   232,   163,   232,   165,   166,   232,   237,   153,
     232,  1197,   232,  1199,   174,   175,   176,   177,   178,   179,
     237,   908,   232,   193,   286,   287,   288,   289,   290,  1215,
     232,   201,   202,   237,   235,   292,   232,   197,   237,   233,
     235,   233,   286,   287,   288,   289,   290,   233,   233,    15,
       3,   211,   222,   197,   198,   199,   200,   227,   202,   203,
     204,   205,   206,   223,     3,   232,   237,   232,     5,   286,
     287,   288,   289,   290,   234,   241,   232,   232,  1067,   249,
     229,   235,   286,   287,   288,   289,   290,   286,   287,   288,
     289,   290,   103,   232,   232,   255,   256,   232,    87,    87,
     232,    45,     4,   232,    45,    47,   197,    97,   278,   279,
      97,   232,   232,   232,   107,   101,     6,   201,   110,   145,
     145,   149,   160,   237,    14,   285,   156,   232,   239,   232,
       3,   155,    22,    23,    24,    25,    26,    27,   232,  1026,
      30,    31,    32,    33,    34,    35,    36,   235,     4,   232,
      40,    41,    42,    43,    44,   235,    46,   293,   232,   232,
     232,    51,    52,    53,    54,    55,    56,    57,    58,   236,
      60,    61,    62,   232,    64,   232,    66,    67,    68,    69,
      70,    71,   240,   232,   232,    75,    76,   232,    78,   240,
     243,   232,     4,  1182,     4,    85,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   243,  1204,   106,  1135,   232,   109,
     249,   236,   251,   250,   253,   252,   255,   254,   257,   256,
     259,   258,   261,   260,   263,   262,   265,   264,   232,   266,
       4,   232,     4,  1161,    87,   135,     4,   232,   149,   233,
     232,   141,   232,   143,   144,   232,   232,   147,   148,   232,
     150,   232,   152,   232,   232,     3,   233,   232,   158,   232,
     232,   161,   162,   232,   232,   232,   232,   167,   168,   292,
     170,   232,   172,   232,   232,   235,   233,   232,     4,    16,
       3,    90,    91,    87,   234,   241,   232,   235,   232,   232,
     235,   232,   234,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   207,   208,   209,
     210,   233,   212,   233,   233,   232,     5,   235,   232,     4,
     197,     4,   232,   132,   133,   134,   197,    97,   232,   232,
     139,   140,   240,    97,    73,   232,   236,   146,   104,   232,
       4,   104,   242,   243,   104,   232,   238,   232,   117,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   235,   177,   116,
     235,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   235,   235,   123,
     128,   124,   232,   164,     4,   232,   232,     4,   233,   237,
     232,     4,   233,   232,   232,   232,   232,   232,   232,   232,
       4,   232,     4,   232,   232,   232,    17,     5,    48,   235,
     235,   233,   233,   233,     3,   233,   233,   233,   233,   108,
     233,   240,   233,   232,     4,   232,   232,   235,    74,    99,
     235,   233,   251,   252,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   111,   111,   111,   111,   235,     4,   232,
       7,    88,   239,     4,   154,     4,   233,   232,   105,     4,
     232,   104,     4,   232,   235,   233,   232,   232,     4,   233,
     232,   232,   232,     4,   233,     5,    48,   107,   233,   235,
      28,   293,     4,   235,   197,   235,   112,    99,    98,   235,
     232,   232,   230,    19,    29,   235,   235,   103,   233,   197,
     232,   235,   233,   232,   232,    98,   233,   232,   232,     4,
     233,     4,   232,   232,   235,     4,   253,     5,  1203,   229,
      73,   422,    66,   680,   220,   561,   859,  1016,   491,  1011,
    1142,   982,   184,  1078,  1054,   429,   754,  1103,  1147,  1068,
     840,    -1,   526,   585,    -1,    -1,   793,    -1,    -1,    -1,
      -1,   554,    -1,    -1,    -1,    -1,    -1,   287
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    20,    21,   296,   297,   298,   299,   233,   233,     0,
     297,   137,   309,   240,   240,   233,     6,    14,    22,    23,
      24,    25,    26,    27,    30,    31,    32,    33,    34,    35,
      36,    40,    41,    42,    43,    44,    46,    51,    52,    53,
      54,    55,    56,    57,    58,    60,    61,    62,    64,    66,
      67,    68,    69,    70,    71,    75,    76,    78,    85,   106,
     109,   135,   141,   143,   144,   147,   148,   150,   152,   158,
     161,   162,   167,   168,   170,   172,   207,   208,   209,   210,
     212,   236,   242,   243,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   300,   301,   302,   303,   304,   311,   313,   314,
     315,   316,   318,   319,   327,   330,   331,   333,   334,   341,
     343,   344,   346,   348,   350,   354,   355,   356,   359,   361,
     368,   375,   378,   384,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   400,   401,   402,   403,   407,   408,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   232,   138,   310,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   333,   233,   233,   233,   233,
     236,   243,   315,   317,   335,   233,   233,   233,   233,   242,
       4,   234,   249,   250,   285,   468,   478,   234,   311,   291,
     233,   345,     3,   232,   240,   251,   252,   454,   467,    10,
      11,    12,    13,     4,   103,   479,    88,    89,    92,    93,
     223,   234,   255,   256,   376,   456,   457,   107,   216,   217,
     218,   219,   220,   222,   224,   225,   226,   227,   228,   234,
     265,   456,   476,   477,     3,    88,   102,   234,   240,   263,
     264,   386,   465,   466,   386,     3,    88,   234,   240,   261,
     262,   380,   463,   464,    88,   240,   436,    79,    88,   448,
     456,     4,   456,   196,   477,   234,   257,   258,   459,   460,
     459,    63,   234,   486,   487,   240,   456,   456,   100,   456,
     234,   456,   486,   459,   459,    88,   240,   358,   437,   486,
     486,   486,   455,   468,   234,   357,   468,   456,   477,   477,
     456,   477,   477,     4,     8,     9,    88,   480,   456,   477,
     456,   358,   477,   459,   477,     5,    88,   369,   351,   477,
     459,   159,   314,   455,   291,   236,   456,   456,   456,   456,
     234,     8,   468,   236,   214,   237,   286,   287,   288,   289,
     290,   467,   468,   473,   474,   475,    30,    32,    37,    38,
      39,   150,   221,   234,   238,   456,   459,   467,   468,   482,
     483,   484,   349,   238,   312,   232,   310,   236,   232,   292,
     232,   232,   232,   231,   458,   234,     4,   236,   232,   232,
     363,   459,   459,   456,   456,   234,   456,   234,   234,   234,
     476,   234,   477,   294,   232,     3,   236,   387,   385,     3,
     236,   379,   232,   232,   232,   232,   232,   232,     4,   236,
     232,   232,   234,     4,   232,   232,   232,   232,   232,   232,
       4,   232,   232,   232,     6,   332,   232,   232,   232,   232,
       4,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   312,   352,   232,   232,   232,   237,
     468,   455,   232,   232,   232,   232,   328,   235,   235,   468,
     468,   468,   468,   468,   468,   468,   232,   235,   233,     4,
     323,   324,   325,   326,   457,   460,   467,   468,   233,   233,
     233,   313,   311,    80,    88,   449,   468,    15,   454,     3,
       3,   234,   409,     3,   232,   237,   477,   232,   468,    93,
      94,   377,     5,   241,   232,   232,    87,   371,   477,   456,
     456,     3,     7,   234,   259,   260,   461,   462,   477,   229,
     235,   477,   103,   232,   468,   232,   312,   232,   468,   312,
     456,   456,   477,    87,   321,   464,    87,   232,   468,   449,
      45,     4,   232,    45,    47,    88,    90,    91,   450,   453,
     197,   456,    97,   232,    97,   486,    88,   234,   364,   232,
     486,   486,   486,   455,   232,   357,   480,   456,    83,    88,
     485,   101,    72,   204,   201,   107,   110,    88,   234,   365,
       4,   107,   145,   145,   448,   353,   234,   461,   461,   160,
     347,   169,   292,   342,   237,    88,   467,   155,   156,   157,
     159,   316,   470,   471,   472,   237,   237,   473,   477,   232,
     232,   239,   232,   239,   232,   239,   232,   239,   238,   465,
     238,   463,   238,   461,   239,   237,   232,     3,   232,   458,
     235,     4,   237,   363,   456,   456,   293,   232,   235,   232,
     232,   232,     3,   236,   232,   232,   232,   240,   237,     4,
       8,     9,    65,    79,    80,    81,    82,    84,    86,   118,
     119,   120,   122,   125,   126,   127,   129,   130,   131,   136,
     163,   165,   166,   174,   175,   176,   177,   178,   179,   197,
     211,   388,   456,   478,   481,   240,   237,   381,   373,   232,
     232,   232,     4,   237,   232,   232,     4,   232,   236,   232,
     233,   232,     4,   232,     4,   232,   449,   232,   232,   399,
     232,    87,   232,   232,   233,   404,   232,   232,     4,   232,
     232,   232,   232,   149,   232,   232,   312,   292,   468,   336,
     232,   232,   233,   232,   235,     4,   460,   457,   468,   467,
     320,   466,   321,   322,   462,    16,   232,     3,   235,   241,
     232,   371,   477,   468,   468,   477,   232,   468,   477,   477,
     234,   235,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   235,
     232,   232,   448,   464,     5,   232,   437,     4,   232,   197,
       4,    88,    95,   452,   463,   197,    97,   232,    88,   234,
     366,   240,    97,   455,   232,   467,    73,   456,   232,   104,
       4,   111,   113,   114,   115,   232,    88,   234,   367,   104,
       4,    88,   370,   371,   312,   104,   461,   238,   449,    88,
     467,   245,   246,   469,   470,   329,   232,   239,   239,   232,
     239,   232,   449,   232,   468,   235,   232,   235,   232,   235,
     449,   237,   235,   235,   468,   455,   465,   117,   116,   455,
     123,   124,   128,   455,   455,   455,   467,   164,   455,   455,
     455,   463,    90,    91,   132,   133,   134,   139,   140,   146,
     177,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   382,   451,   453,
     467,    84,   121,   122,   130,   142,   153,   179,   374,   232,
     307,     4,   232,   438,   232,     4,   233,   237,   232,   233,
       4,   376,   232,   232,     4,    88,   372,   232,   232,    98,
     105,   130,   153,   197,   198,   199,   200,   202,   203,   204,
     205,   206,   405,   232,   232,   232,   232,     4,     4,   438,
     232,   232,   232,   171,   173,   337,   338,   312,   466,   462,
      17,   235,     5,    48,   468,   235,   287,   233,   467,   233,
     233,   233,   233,   233,   233,   233,   233,   233,     3,   232,
      48,    49,    50,   439,   440,   108,   441,     4,   232,   463,
      96,   197,   463,   235,   232,   367,   455,   235,    74,    99,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     111,   111,   111,   111,   235,   232,   441,     4,   372,     7,
     480,   233,   239,   337,   232,   232,   235,   232,   235,   468,
      88,   455,   455,   455,   455,   455,   196,   240,   383,   465,
       4,   154,     4,   232,   233,   105,   443,   232,     4,   232,
     233,   377,   438,   232,   232,   104,   444,     4,     4,    99,
     101,   154,   406,   463,   465,   409,   409,   456,   436,     4,
     232,     4,   443,   373,   233,   340,     4,    18,     5,    48,
     232,   305,   293,   235,   439,   107,   442,   233,    28,   446,
       4,   235,   197,   463,   235,   441,    99,    98,   112,   235,
     446,   339,   311,   232,   232,   235,   230,   235,   383,   444,
     233,    29,   447,   235,   233,   443,   232,   232,   447,   311,
      19,   449,   232,   103,   233,   197,   445,   463,   446,    98,
     449,   445,   232,   232,   456,     4,   233,   419,   447,   360,
       4,     5,    88,   308,   232,   306,   463,   312,   420,   445,
     312,   232,   307,   456,   235,   362,     4,   312
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

  case 122:

/* Line 1455 of yacc.c  */
#line 626 "lev_comp.y"
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

  case 123:

/* Line 1455 of yacc.c  */
#line 642 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 648 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 654 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 660 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 666 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 672 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 678 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 684 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 690 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 700 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 710 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 720 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 730 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 740 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 750 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 762 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 767 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 774 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 779 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 786 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 791 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 798 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 802 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 808 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 813 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 820 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 824 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 830 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 834 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 840 "lev_comp.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 862 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 866 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 877 "lev_comp.y"
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

  case 156:

/* Line 1455 of yacc.c  */
#line 932 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 938 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 942 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 948 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 955 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 959 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 967 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 971 "lev_comp.y"
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

  case 164:

/* Line 1455 of yacc.c  */
#line 994 "lev_comp.y"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 1039 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1049 "lev_comp.y"
    {
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1052 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1062 "lev_comp.y"
    {
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1067 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1081 "lev_comp.y"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 1118 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1123 "lev_comp.y"
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

  case 177:

/* Line 1455 of yacc.c  */
#line 1152 "lev_comp.y"
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

  case 178:

/* Line 1455 of yacc.c  */
#line 1166 "lev_comp.y"
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

  case 179:

/* Line 1455 of yacc.c  */
#line 1182 "lev_comp.y"
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

  case 180:

/* Line 1455 of yacc.c  */
#line 1202 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1214 "lev_comp.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1234 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1240 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1250 "lev_comp.y"
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

  case 185:

/* Line 1455 of yacc.c  */
#line 1268 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1278 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiio", VA_PASS8(0, 0, 0, 0, NO_COLOR, ATR_NONE, SPLEV_MESSAGE_TYPE_MESSAGE, SPO_MESSAGE));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1282 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiio", VA_PASS8((int)(yyvsp[(11) - (11)].i), 0, 0, 0, (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_MESSAGE));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1286 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiio", VA_PASS8((int)(yyvsp[(17) - (17)].i), (int)(yyvsp[(15) - (17)].i), (int)(yyvsp[(13) - (17)].i), (int)(yyvsp[(11) - (17)].i), (int)(yyvsp[(9) - (17)].i), (int)(yyvsp[(7) - (17)].i), (int)(yyvsp[(5) - (17)].i), SPO_MESSAGE));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1292 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1297 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1302 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1309 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1316 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (int64_t)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1325 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1333 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((int64_t)(yyvsp[(1) - (4)].i), (int64_t)(yyvsp[(2) - (4)].i), (int64_t)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1344 "lev_comp.y"
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

  case 197:

/* Line 1455 of yacc.c  */
#line 1362 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1369 "lev_comp.y"
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

  case 199:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1394 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1398 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1404 "lev_comp.y"
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

  case 203:

/* Line 1455 of yacc.c  */
#line 1414 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1420 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1429 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1435 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1440 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1446 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1451 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1457 "lev_comp.y"
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

  case 211:

/* Line 1455 of yacc.c  */
#line 1468 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int64_t)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1482 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1486 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1496 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1511 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1521 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1536 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1541 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1549 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1556 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1564 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1580 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1584 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1590 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1598 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1604 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1612 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1620 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1625 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1631 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1637 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1643 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1649 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1659 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1664 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1669 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1674 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1679 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1684 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1689 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1694 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1699 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1709 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1715 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2(0xFFFFFFFFL, SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1721 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1726 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1731 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1736 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1741 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1746 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_LEVEL_BOSS));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1751 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_BOSS_HOSTILITY));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1756 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_HAS_BACKUP));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1763 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = ((int64_t)1 << (token - 1));
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      (yyval.i) = (int64_t) ~0;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1776 "lev_comp.y"
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

  case 272:

/* Line 1455 of yacc.c  */
#line 1789 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1797 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1807 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1815 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1824 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1831 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1839 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1850 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1855 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1860 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1865 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1870 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1875 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1880 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1885 "lev_comp.y"
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

  case 288:

/* Line 1455 of yacc.c  */
#line 1899 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1910 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1915 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1920 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1925 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1930 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1935 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1940 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1945 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1955 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1960 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1965 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1970 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1975 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1980 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1986 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1991 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 1996 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2001 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2008 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2014 "lev_comp.y"
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

  case 311:

/* Line 1455 of yacc.c  */
#line 2043 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2048 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2056 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2062 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2066 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2074 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2081 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2088 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (int64_t) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2098 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2108 "lev_comp.y"
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

  case 321:

/* Line 1455 of yacc.c  */
#line 2124 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (int64_t) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2134 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2138 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2144 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2150 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2156 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2160 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2179 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2187 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2193 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2198 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2203 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2208 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2213 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2219 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2225 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2230 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2235 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2240 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2245 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2250 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2260 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2266 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2272 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2277 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2284 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2291 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2295 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2301 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2307 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2313 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2318 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2325 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2331 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (int64_t) LR_SPECIAL_MAP_SEEN : (int64_t) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2341 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2347 "lev_comp.y"
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

  case 363:

/* Line 1455 of yacc.c  */
#line 2368 "lev_comp.y"
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

  case 364:

/* Line 1455 of yacc.c  */
#line 2380 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2384 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2390 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (int64_t)(yyvsp[(7) - (7)].i), (int64_t)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (int64_t)(yyvsp[(7) - (11)].i), (int64_t)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2400 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (int64_t)(yyvsp[(9) - (9)].i), (int64_t)(yyvsp[(7) - (9)].i), (int64_t)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2405 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int64_t)(yyvsp[(9) - (13)].i), (int64_t)(yyvsp[(7) - (13)].i), (int64_t)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2411 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2415 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2421 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2425 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2429 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2433 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2437 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2441 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2453 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2457 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2463 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2469 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2473 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2479 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2483 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2489 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2493 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2498 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2505 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2509 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2516 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2520 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2525 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2532 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2536 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2541 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2549 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2555 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int64_t)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2562 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2566 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2573 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2584 "lev_comp.y"
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

  case 405:

/* Line 1455 of yacc.c  */
#line 2597 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2607 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2611 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2618 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 0);
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2622 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 1);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 2);
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2632 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2636 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2642 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2648 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2652 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2658 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2664 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2675 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2679 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2685 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2689 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2705 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2713 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2723 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2732 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2737 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2744 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2755 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2761 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2765 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2769 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2776 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2786 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2793 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2797 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2803 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2807 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2815 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2819 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2826 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2836 "lev_comp.y"
    {
		      int64_t r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2849 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2853 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2860 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2870 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2879 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2890 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2894 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2901 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2911 "lev_comp.y"
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

  case 463:

/* Line 1455 of yacc.c  */
#line 2922 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2931 "lev_comp.y"
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

  case 465:

/* Line 1455 of yacc.c  */
#line 2941 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2947 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2951 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2958 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2968 "lev_comp.y"
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

  case 470:

/* Line 1455 of yacc.c  */
#line 2979 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2988 "lev_comp.y"
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

  case 472:

/* Line 1455 of yacc.c  */
#line 2998 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3002 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3008 "lev_comp.y"
    { ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3010 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3016 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3020 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3024 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3028 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3036 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3045 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3049 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3053 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3057 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3061 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3064 "lev_comp.y"
    { ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3068 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3075 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3084 "lev_comp.y"
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

  case 494:

/* Line 1455 of yacc.c  */
#line 3125 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3129 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3136 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3143 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3153 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3157 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3165 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3169 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3173 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3177 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3181 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3186 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3190 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3194 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3199 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3204 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3209 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3213 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3218 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3223 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3227 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3231 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3236 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3240 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3247 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3253 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3257 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3263 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3279 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 3283 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3287 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 3291 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 539:

/* Line 1455 of yacc.c  */
#line 3313 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 540:

/* Line 1455 of yacc.c  */
#line 3317 "lev_comp.y"
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

  case 541:

/* Line 1455 of yacc.c  */
#line 3343 "lev_comp.y"
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
#line 7613 "lev_comp.tab.c"
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
#line 3371 "lev_comp.y"


/*lev_comp.y*/

