
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

extern void lc_error(const char *, ...);
extern void lc_warning(const char *, ...);
extern void yyerror(const char *);
extern void yywarning(const char *);
extern int yylex(void);
int yyparse(void);

extern int get_floor_type(char);
extern int get_room_type(char *);
extern int get_trap_type(char *);
extern int get_monster_id(char *,char);
extern int get_object_id(char *,char);
extern boolean check_monster_char(char);
extern boolean check_object_char(char);
extern char what_map_char(char);
extern void scan_map(char *, sp_lev *);
extern void add_opcode(sp_lev *, int, genericptr_t);
extern genericptr_t get_last_opcode_data1(sp_lev *, int);
extern genericptr_t get_last_opcode_data2(sp_lev *, int, int);
extern boolean check_subrooms(sp_lev *);
extern boolean write_level_file(char *,sp_lev *);
extern struct opvar *set_opvar_int(struct opvar *, int64_t);
extern void add_opvars(sp_lev *, const char *, ...);
extern void start_level_def(sp_lev * *, char *);

extern struct lc_funcdefs *funcdef_new(int64_t,char *);
extern void funcdef_free_all(struct lc_funcdefs *);
extern struct lc_funcdefs *funcdef_defined(struct lc_funcdefs *,
                                                   char *, int);
extern char *funcdef_paramtypes(struct lc_funcdefs *);
extern char *decode_parm_str(char *);

extern struct lc_vardefs *vardef_new(int64_t,char *);
extern void vardef_free_all(struct lc_vardefs *);
extern struct lc_vardefs *vardef_defined(struct lc_vardefs *,
                                                 char *, int);

extern void break_stmt_start(void);
extern void break_stmt_end(sp_lev *);
extern void break_stmt_new(sp_lev *, int64_t);

extern void splev_add_from(sp_lev *, sp_lev *);

extern void check_vardef_type(struct lc_vardefs *, char *, int64_t);
extern void vardef_used(struct lc_vardefs *, char *);
extern struct lc_vardefs *add_vardef_type(struct lc_vardefs *,
                                                  char *, int64_t);

extern int reverse_jmp_opcode(int64_t);

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
     QUEST_COMPANION_ID = 449,
     LEVEL_ADJUSTMENT_ID = 450,
     KEEP_ORIGINAL_INVENTORY_ID = 451,
     PARALYZED_ID = 452,
     STUNNED_ID = 453,
     CONFUSED_ID = 454,
     SEENTRAPS_ID = 455,
     ALL_ID = 456,
     MONTYPE_ID = 457,
     OBJTYPE_ID = 458,
     TERTYPE_ID = 459,
     TERTYPE2_ID = 460,
     LEVER_EFFECT_TYPE = 461,
     SWITCHABLE_ID = 462,
     CONTINUOUSLY_USABLE_ID = 463,
     TARGET_ID = 464,
     TRAPTYPE_ID = 465,
     EFFECT_FLAG_ID = 466,
     GRAVE_ID = 467,
     BRAZIER_ID = 468,
     SIGNPOST_ID = 469,
     TREE_ID = 470,
     ERODEPROOF_ID = 471,
     FUNCTION_ID = 472,
     MSG_OUTPUT_TYPE = 473,
     COMPARE_TYPE = 474,
     UNKNOWN_TYPE = 475,
     rect_ID = 476,
     fillrect_ID = 477,
     line_ID = 478,
     randline_ID = 479,
     grow_ID = 480,
     selection_ID = 481,
     flood_ID = 482,
     rndcoord_ID = 483,
     circle_ID = 484,
     ellipse_ID = 485,
     filter_ID = 486,
     complement_ID = 487,
     gradient_ID = 488,
     GRADIENT_TYPE = 489,
     LIMITED = 490,
     HUMIDITY_TYPE = 491,
     STRING = 492,
     MAP_ID = 493,
     NQSTRING = 494,
     VARSTRING = 495,
     CFUNC = 496,
     CFUNC_INT = 497,
     CFUNC_STR = 498,
     CFUNC_COORD = 499,
     CFUNC_REGION = 500,
     VARSTRING_INT = 501,
     VARSTRING_INT_ARRAY = 502,
     VARSTRING_STRING = 503,
     VARSTRING_STRING_ARRAY = 504,
     VARSTRING_VAR = 505,
     VARSTRING_VAR_ARRAY = 506,
     VARSTRING_COORD = 507,
     VARSTRING_COORD_ARRAY = 508,
     VARSTRING_REGION = 509,
     VARSTRING_REGION_ARRAY = 510,
     VARSTRING_MAPCHAR = 511,
     VARSTRING_MAPCHAR_ARRAY = 512,
     VARSTRING_MONST = 513,
     VARSTRING_MONST_ARRAY = 514,
     VARSTRING_OBJ = 515,
     VARSTRING_OBJ_ARRAY = 516,
     VARSTRING_SEL = 517,
     VARSTRING_SEL_ARRAY = 518,
     METHOD_INT = 519,
     METHOD_INT_ARRAY = 520,
     METHOD_STRING = 521,
     METHOD_STRING_ARRAY = 522,
     METHOD_VAR = 523,
     METHOD_VAR_ARRAY = 524,
     METHOD_COORD = 525,
     METHOD_COORD_ARRAY = 526,
     METHOD_REGION = 527,
     METHOD_REGION_ARRAY = 528,
     METHOD_MAPCHAR = 529,
     METHOD_MAPCHAR_ARRAY = 530,
     METHOD_MONST = 531,
     METHOD_MONST_ARRAY = 532,
     METHOD_OBJ = 533,
     METHOD_OBJ_ARRAY = 534,
     METHOD_SEL = 535,
     METHOD_SEL_ARRAY = 536,
     DICE = 537
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
#line 582 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 594 "lev_comp.tab.c"

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
#define YYLAST   1401

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  300
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  194
/* YYNRULES -- Number of rules.  */
#define YYNRULES  545
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1232

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   537

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   295,   299,     2,
     239,   240,   293,   291,   237,   292,   297,   294,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   238,     2,
       2,   296,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   241,     2,   242,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   243,   298,   244,     2,     2,     2,     2,
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
     235,   236,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290
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
     860,   862,   864,   866,   870,   874,   875,   881,   884,   885,
     889,   891,   895,   897,   901,   905,   907,   909,   913,   915,
     917,   919,   923,   925,   927,   929,   933,   937,   941,   945,
     949,   953,   957,   961,   963,   965,   967,   969,   973,   977,
     981,   985,   991,   999,  1005,  1014,  1016,  1020,  1024,  1030,
    1036,  1044,  1052,  1059,  1065,  1066,  1069,  1075,  1079,  1091,
    1105,  1115,  1122,  1123,  1127,  1129,  1133,  1137,  1141,  1145,
    1147,  1149,  1153,  1157,  1161,  1165,  1169,  1173,  1177,  1179,
    1181,  1183,  1185,  1189,  1193,  1195,  1201,  1211,  1217,  1225,
    1229,  1233,  1239,  1249,  1255,  1261,  1267,  1268,  1284,  1285,
    1287,  1295,  1307,  1317,  1331,  1335,  1343,  1355,  1369,  1379,
    1389,  1399,  1407,  1415,  1421,  1429,  1435,  1445,  1453,  1459,
    1469,  1475,  1481,  1485,  1493,  1497,  1505,  1513,  1517,  1523,
    1529,  1533,  1539,  1547,  1557,  1559,  1561,  1563,  1565,  1567,
    1568,  1571,  1573,  1577,  1579,  1581,  1583,  1584,  1588,  1590,
    1591,  1595,  1597,  1598,  1602,  1603,  1607,  1608,  1612,  1614,
    1616,  1618,  1620,  1622,  1624,  1626,  1628,  1630,  1634,  1636,
    1638,  1643,  1645,  1647,  1652,  1654,  1656,  1661,  1663,  1668,
    1674,  1676,  1680,  1682,  1686,  1688,  1690,  1695,  1705,  1707,
    1709,  1714,  1716,  1722,  1724,  1726,  1731,  1733,  1735,  1741,
    1743,  1745,  1747,  1752,  1754,  1756,  1762,  1764,  1766,  1768,
    1772,  1774,  1776,  1780,  1782,  1787,  1791,  1795,  1799,  1803,
    1807,  1811,  1813,  1815,  1819,  1821,  1825,  1826,  1828,  1830,
    1832,  1834,  1838,  1839,  1841,  1843,  1846,  1849,  1854,  1861,
    1866,  1873,  1880,  1887,  1894,  1897,  1904,  1913,  1922,  1933,
    1948,  1951,  1953,  1957,  1959,  1963,  1965,  1967,  1969,  1971,
    1973,  1975,  1977,  1979,  1981,  1983,  1985,  1987,  1989,  1991,
    1993,  1995,  1997,  1999,  2001,  2012
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     301,     0,    -1,    -1,   302,    -1,   303,    -1,   303,   302,
      -1,   304,   314,   316,    -1,    21,   238,   245,    -1,    20,
     238,   245,   237,     3,    -1,    20,   238,   245,   237,     3,
     237,   455,    -1,    22,   238,    11,   237,   415,    -1,    22,
     238,    10,   237,     3,    -1,    22,   238,    13,    -1,    22,
     238,    12,   237,     3,   237,     3,   237,     5,   237,     5,
     237,   455,   237,   313,   312,    -1,    23,   238,   485,    -1,
     162,   238,   163,   237,   164,    -1,   162,   238,   163,    -1,
      36,   238,     4,   237,   326,    -1,    26,   238,   111,    -1,
      -1,   237,   235,    -1,    -1,   237,   462,    -1,    -1,   237,
       3,    -1,     5,    -1,    89,    -1,    -1,   141,   238,   315,
      -1,   142,   237,   315,    -1,   142,    -1,    -1,   318,   316,
      -1,   243,   316,   244,    -1,   359,    -1,   305,    -1,   306,
      -1,   423,    -1,   309,    -1,   307,    -1,   431,    -1,   432,
      -1,   433,    -1,   308,    -1,   430,    -1,   429,    -1,   427,
      -1,   428,    -1,   434,    -1,   435,    -1,   436,    -1,   437,
      -1,   438,    -1,   403,    -1,   361,    -1,   324,    -1,   323,
      -1,   418,    -1,   373,    -1,   395,    -1,   440,    -1,   441,
      -1,   405,    -1,   406,    -1,   408,    -1,   407,    -1,   439,
      -1,   339,    -1,   349,    -1,   351,    -1,   355,    -1,   353,
      -1,   336,    -1,   346,    -1,   332,    -1,   335,    -1,   398,
      -1,   409,    -1,   380,    -1,   396,    -1,   383,    -1,   389,
      -1,   419,    -1,   414,    -1,   401,    -1,   360,    -1,   420,
      -1,   421,    -1,   422,    -1,   424,    -1,   366,    -1,   364,
      -1,   413,    -1,   417,    -1,   416,    -1,   399,    -1,   400,
      -1,   402,    -1,   394,    -1,   397,    -1,   255,    -1,   257,
      -1,   259,    -1,   261,    -1,   263,    -1,   265,    -1,   267,
      -1,   269,    -1,   271,    -1,   254,    -1,   256,    -1,   258,
      -1,   260,    -1,   262,    -1,   264,    -1,   266,    -1,   268,
      -1,   270,    -1,   319,    -1,   320,    -1,   248,    -1,   248,
      -1,   320,    -1,   166,   238,   319,    -1,   321,   296,   474,
      -1,   321,   296,   226,   238,   483,    -1,   321,   296,   473,
      -1,   321,   296,   490,   238,   467,    -1,   321,   296,   489,
     238,   469,    -1,   321,   296,   488,   238,   471,    -1,   321,
     296,   462,    -1,   321,   296,   465,    -1,   321,   296,   243,
     330,   244,    -1,   321,   296,   243,   329,   244,    -1,   321,
     296,   243,   328,   244,    -1,   321,   296,   490,   238,   243,
     327,   244,    -1,   321,   296,   489,   238,   243,   326,   244,
      -1,   321,   296,   488,   238,   243,   325,   244,    -1,   321,
     296,   243,   331,   244,    -1,   472,    -1,   325,   237,   472,
      -1,   470,    -1,   326,   237,   470,    -1,   468,    -1,   327,
     237,   468,    -1,   466,    -1,   328,   237,   466,    -1,   463,
      -1,   329,   237,   463,    -1,   474,    -1,   330,   237,   474,
      -1,   473,    -1,   331,   237,   473,    -1,    -1,    -1,   217,
     247,   239,   333,   478,   240,   334,   317,    -1,   247,   239,
     481,   240,    -1,   165,    -1,    -1,     6,    -1,     6,    -1,
     241,   474,   219,   474,   242,    -1,   241,   474,   242,    -1,
      -1,    -1,   174,   340,   241,   461,   242,   341,   243,   342,
     244,    -1,    -1,   343,   342,    -1,    -1,   175,   486,   238,
     344,   316,    -1,    -1,   177,   238,   345,   316,    -1,   176,
      -1,   297,   297,    -1,   173,    -1,   172,   322,   296,   474,
     347,   474,    -1,    -1,   348,   350,   317,    -1,    -1,   171,
     241,   461,   242,   352,   317,    -1,    -1,   338,   238,   354,
     318,    -1,    -1,   152,   338,   356,   357,    -1,   317,    -1,
      -1,   317,   358,   153,   317,    -1,    14,   238,   473,    -1,
      14,   238,   473,   237,    15,   237,    16,   237,    17,   237,
       4,    -1,    14,   238,   473,   237,    15,   237,    16,   237,
      17,   237,    18,   237,    19,   237,     4,   237,     4,    -1,
      79,    -1,    79,   238,   486,    -1,    79,   238,    89,    -1,
      66,   238,   362,   237,   362,    -1,    66,   238,   362,   237,
     486,    -1,   239,     4,   237,    88,   237,   377,   240,    -1,
     443,   337,   237,   455,    -1,    -1,   139,   238,   363,   237,
     370,   237,   372,   444,   447,   449,   452,   453,   451,   365,
     317,    -1,    -1,    58,   238,   363,   237,   369,   237,   371,
     237,   372,   444,   447,   449,   452,   453,   451,   367,   317,
      -1,    -1,   237,     5,    -1,   239,     4,   237,     4,   240,
      -1,    89,    -1,   239,     4,   237,     4,   240,    -1,    89,
      -1,   239,   381,   237,   382,   240,    -1,    89,    -1,   239,
       4,   237,     4,   240,    -1,    89,    -1,   151,   238,   374,
     237,   454,   237,   375,   237,   377,   378,    -1,    34,   238,
     454,   237,   483,   378,    -1,     5,    -1,    89,    -1,   376,
      -1,    89,    -1,    88,    -1,    88,   298,   376,    -1,     4,
      -1,    89,    -1,    -1,   378,   237,   379,    -1,   157,   238,
     158,    -1,    85,   238,   471,    -1,   134,   238,     4,    -1,
     183,    -1,   125,    -1,   126,    -1,   146,    -1,    25,    -1,
      24,   238,   381,   237,   382,   368,   246,    -1,    24,   238,
     462,   368,   246,    -1,    93,    -1,    94,    -1,    95,    -1,
      94,    -1,    32,   238,   385,    -1,    -1,    32,   238,   385,
     384,   317,    -1,   469,   237,   462,   386,    -1,    -1,   386,
     237,   387,    -1,   473,    -1,   143,    -1,   144,    -1,   457,
      -1,   150,   473,    -1,   184,    -1,   185,    -1,   181,    -1,
     188,    -1,   189,    -1,   189,   238,   461,    -1,   190,    -1,
     191,   238,   461,    -1,   192,   238,   461,    -1,   197,   238,
     461,    -1,   198,    -1,   199,    -1,   200,   238,   388,    -1,
     200,    -1,   186,    -1,   187,    -1,   193,    -1,   195,   238,
     461,    -1,   196,    -1,   136,    -1,   137,    -1,   138,    -1,
     194,    -1,   245,    -1,   201,    -1,   245,   298,   388,    -1,
      30,   238,   391,    -1,    -1,    31,   238,   391,   390,   317,
      -1,   471,   392,    -1,    -1,   392,   237,   393,    -1,    82,
      -1,   202,   238,   469,    -1,   487,    -1,   140,   238,   473,
      -1,   169,   238,   461,    -1,   170,    -1,    81,    -1,   178,
     238,   461,    -1,   216,    -1,    80,    -1,   179,    -1,   180,
     238,   461,    -1,   181,    -1,   182,    -1,   462,    -1,   123,
     238,   120,    -1,   122,   238,   121,    -1,   124,   238,   461,
      -1,   133,   238,   461,    -1,   134,   238,   461,    -1,   167,
     238,   168,    -1,   135,   238,   461,    -1,    85,   238,   471,
      -1,   183,    -1,   126,    -1,    87,    -1,    83,    -1,   129,
     238,   127,    -1,   130,   238,   128,    -1,    65,   238,   461,
      -1,   131,   238,   132,    -1,    33,   238,   442,   237,   462,
      -1,    35,   238,   462,   237,    88,   237,   454,    -1,    40,
     238,   462,   237,    88,    -1,    40,   238,   462,   237,    88,
     237,     5,   312,    -1,    41,    -1,    41,   238,   483,    -1,
      41,   238,   201,    -1,    54,   238,   462,   237,    98,    -1,
      55,   238,   462,   237,    98,    -1,    55,   238,   492,   237,
     492,   237,    98,    -1,    60,   238,   492,   237,   492,   237,
     245,    -1,    61,   238,   492,   237,   492,   404,    -1,    62,
     238,   492,   237,   492,    -1,    -1,   237,    98,    -1,    69,
     238,   483,   237,   105,    -1,    70,   238,   483,    -1,    72,
     238,   462,   237,   209,   238,   462,   237,   103,   237,    99,
      -1,    72,   238,   462,   237,    73,   237,    74,   237,    75,
     237,   103,   237,    99,    -1,    71,   238,   462,   237,   101,
     237,   102,   237,    99,    -1,    86,   238,   462,   237,   206,
     410,    -1,    -1,   410,   237,   411,    -1,    99,    -1,   202,
     238,   469,    -1,   203,   238,   471,    -1,   204,   238,   415,
      -1,   205,   238,   415,    -1,   207,    -1,   208,    -1,   209,
     238,   462,    -1,   210,   238,   442,    -1,   157,   238,   412,
      -1,   109,   238,   450,    -1,   211,   238,     4,    -1,   134,
     238,     4,    -1,   100,   238,   101,    -1,     4,    -1,   103,
      -1,   158,    -1,   105,    -1,    77,   238,   483,    -1,    76,
     238,   483,    -1,     3,    -1,   239,     3,   237,   455,   240,
      -1,   156,   238,   465,   237,   467,   237,   467,   237,     7,
      -1,   154,   238,   483,   237,   467,    -1,   154,   238,   483,
     237,   467,   237,   108,    -1,    56,   238,   465,    -1,    57,
     238,   465,    -1,    46,   238,   245,   237,    47,    -1,    46,
     238,   245,   237,    47,   237,   202,   238,   469,    -1,    43,
     238,   465,   237,    45,    -1,    44,   238,   492,   237,    45,
      -1,    27,   238,   483,   237,   107,    -1,    -1,    42,   238,
     465,   237,   455,   237,   443,   444,   447,   449,   452,   453,
     451,   425,   426,    -1,    -1,   317,    -1,    51,   238,   462,
     237,   456,   237,   458,    -1,    51,   238,   462,   237,   456,
     237,   458,   237,   202,   238,   469,    -1,    51,   238,   462,
     237,   456,   237,   458,   237,    97,    -1,    51,   238,   462,
     237,   456,   237,   458,   237,    97,   237,   202,   238,   469,
      -1,    52,   238,   462,    -1,    52,   238,   462,   237,   202,
     238,   469,    -1,   113,   238,   462,   237,   114,   237,     4,
     237,   115,   237,   116,    -1,   113,   238,   462,   237,   114,
     237,     4,   237,   115,   237,   116,   237,   455,    -1,   113,
     238,   462,   237,   114,   237,   117,   237,   115,    -1,   113,
     238,   462,   237,   114,   237,   118,   237,   115,    -1,   113,
     238,   462,   237,   114,   237,   119,   237,   115,    -1,   113,
     238,   462,   237,   114,   237,   115,    -1,   110,   238,   483,
     237,   111,   237,   108,    -1,   145,   238,   483,   237,     4,
      -1,   145,   238,   483,   237,   111,   237,   108,    -1,   147,
     238,   465,   237,   149,    -1,   148,   238,   483,   237,   149,
     237,     4,   237,     4,    -1,   148,   238,   483,   237,   149,
     237,     4,    -1,    53,   238,   104,   237,   462,    -1,    53,
     238,   104,   237,   462,   237,   202,   238,   469,    -1,   212,
     238,   462,   237,   473,    -1,   212,   238,   462,   237,    89,
      -1,   212,   238,   462,    -1,   213,   238,   462,   237,   159,
     237,   455,    -1,   213,   238,   462,    -1,   214,   238,   462,
     237,   160,   237,   473,    -1,   214,   238,   462,   237,   160,
     237,    89,    -1,   214,   238,   462,    -1,   215,   238,   462,
     237,   161,    -1,   215,   238,   462,   237,   163,    -1,   215,
     238,   462,    -1,    67,   238,   474,   237,   462,    -1,    68,
     238,   462,   237,   491,   237,   473,    -1,    64,   238,   461,
     237,   461,   237,   461,   237,   461,    -1,    64,    -1,   245,
      -1,    89,    -1,   245,    -1,    89,    -1,    -1,   237,   445,
      -1,   446,    -1,   446,   237,   445,    -1,    48,    -1,    49,
      -1,    50,    -1,    -1,   112,   238,   448,    -1,   111,    -1,
      -1,   109,   238,   450,    -1,   108,    -1,    -1,   202,   238,
     469,    -1,    -1,    28,   238,   107,    -1,    -1,    29,   238,
       4,    -1,    80,    -1,    89,    -1,    81,    -1,    89,    -1,
      92,    -1,   459,    -1,    89,    -1,    92,    -1,   459,    -1,
      91,   238,    89,    -1,    96,    -1,    89,    -1,    91,   241,
       4,   242,    -1,   245,    -1,   256,    -1,   257,   241,   474,
     242,    -1,   474,    -1,   463,    -1,   228,   239,   483,   240,
      -1,   260,    -1,   261,   241,   474,   242,    -1,   239,     4,
     237,     4,   240,    -1,    89,    -1,    90,   464,   242,    -1,
     236,    -1,   236,   237,   464,    -1,   466,    -1,   262,    -1,
     263,   241,   474,   242,    -1,   239,     4,   237,     4,   237,
       4,   237,     4,   240,    -1,   468,    -1,   264,    -1,   265,
     241,   474,   242,    -1,     3,    -1,   239,     3,   237,   455,
     240,    -1,   470,    -1,   266,    -1,   267,   241,   474,   242,
      -1,   245,    -1,     3,    -1,   239,     3,   237,   245,   240,
      -1,    89,    -1,   472,    -1,   268,    -1,   269,   241,   474,
     242,    -1,   245,    -1,     3,    -1,   239,     3,   237,   245,
     240,    -1,    89,    -1,   106,    -1,   460,    -1,   473,   297,
     460,    -1,     4,    -1,   484,    -1,   239,     8,   240,    -1,
     254,    -1,   255,   241,   474,   242,    -1,   474,   291,   474,
      -1,   474,   292,   474,    -1,   474,   293,   474,    -1,   474,
     294,   474,    -1,   474,   295,   474,    -1,   239,   474,   240,
      -1,   250,    -1,   251,    -1,   321,   238,   475,    -1,   476,
      -1,   477,   237,   476,    -1,    -1,   477,    -1,   474,    -1,
     473,    -1,   479,    -1,   480,   237,   479,    -1,    -1,   480,
      -1,   462,    -1,   221,   465,    -1,   222,   465,    -1,   223,
     462,   237,   462,    -1,   224,   462,   237,   462,   237,   474,
      -1,   225,   239,   483,   240,    -1,   225,   239,   376,   237,
     483,   240,    -1,   231,   239,     7,   237,   483,   240,    -1,
     231,   239,   483,   237,   483,   240,    -1,   231,   239,   467,
     237,   483,   240,    -1,   227,   462,    -1,   229,   239,   462,
     237,   474,   240,    -1,   229,   239,   462,   237,   474,   237,
      48,   240,    -1,   230,   239,   462,   237,   474,   237,   474,
     240,    -1,   230,   239,   462,   237,   474,   237,   474,   237,
      48,   240,    -1,   233,   239,   234,   237,   239,   474,   292,
     474,   310,   240,   237,   462,   311,   240,    -1,   232,   482,
      -1,   270,    -1,   239,   483,   240,    -1,   482,    -1,   482,
     299,   483,    -1,   290,    -1,   107,    -1,     4,    -1,     8,
      -1,     9,    -1,     4,    -1,     8,    -1,     9,    -1,     4,
      -1,   484,    -1,    37,    -1,    30,    -1,    38,    -1,    32,
      -1,    39,    -1,   154,    -1,    84,    -1,    89,    -1,   493,
      -1,    63,   239,     4,   237,     4,   237,     4,   237,     4,
     240,    -1,   239,     4,   237,     4,   237,     4,   237,     4,
     240,    -1
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
    1761,  1768,  1777,  1781,  1794,  1803,  1802,  1820,  1830,  1836,
    1844,  1850,  1855,  1860,  1865,  1870,  1875,  1880,  1885,  1890,
    1904,  1910,  1915,  1920,  1925,  1930,  1935,  1940,  1945,  1950,
    1955,  1960,  1965,  1970,  1975,  1980,  1985,  1991,  1996,  2001,
    2006,  2013,  2019,  2048,  2053,  2061,  2067,  2071,  2079,  2086,
    2093,  2103,  2113,  2129,  2140,  2143,  2149,  2155,  2161,  2165,
    2171,  2177,  2184,  2190,  2198,  2204,  2209,  2214,  2219,  2224,
    2230,  2236,  2241,  2246,  2251,  2256,  2261,  2266,  2273,  2273,
    2273,  2273,  2276,  2282,  2288,  2293,  2300,  2307,  2311,  2317,
    2323,  2329,  2334,  2341,  2347,  2357,  2364,  2363,  2397,  2400,
    2406,  2411,  2416,  2421,  2427,  2431,  2437,  2441,  2445,  2449,
    2453,  2457,  2463,  2469,  2473,  2479,  2485,  2489,  2495,  2499,
    2505,  2509,  2514,  2521,  2525,  2532,  2536,  2541,  2548,  2552,
    2557,  2565,  2571,  2578,  2582,  2589,  2597,  2600,  2610,  2614,
    2617,  2623,  2627,  2634,  2638,  2642,  2649,  2652,  2658,  2665,
    2668,  2674,  2681,  2685,  2692,  2695,  2702,  2705,  2711,  2712,
    2715,  2716,  2719,  2720,  2721,  2727,  2728,  2729,  2735,  2736,
    2739,  2748,  2753,  2760,  2771,  2777,  2781,  2785,  2792,  2802,
    2809,  2813,  2819,  2823,  2831,  2835,  2842,  2852,  2865,  2869,
    2876,  2886,  2895,  2906,  2910,  2917,  2927,  2938,  2947,  2957,
    2963,  2967,  2974,  2984,  2995,  3004,  3014,  3018,  3025,  3026,
    3032,  3036,  3040,  3044,  3052,  3061,  3065,  3069,  3073,  3077,
    3081,  3084,  3091,  3100,  3133,  3134,  3137,  3138,  3141,  3145,
    3152,  3159,  3170,  3173,  3181,  3185,  3189,  3193,  3197,  3202,
    3206,  3210,  3215,  3220,  3225,  3229,  3234,  3239,  3243,  3247,
    3252,  3256,  3263,  3269,  3273,  3279,  3286,  3287,  3290,  3291,
    3292,  3295,  3299,  3303,  3307,  3313,  3314,  3317,  3318,  3321,
    3322,  3325,  3326,  3329,  3333,  3359
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
  "BLINDED_ID", "MAXHP_ID", "QUEST_COMPANION_ID", "LEVEL_ADJUSTMENT_ID",
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
     485,   486,   487,   488,   489,   490,   491,    44,    58,    40,
      41,    91,    93,   123,   125,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,    43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   300,   301,   301,   302,   302,   303,   304,   304,   304,
     305,   305,   305,   305,   306,   307,   307,   308,   309,   310,
     310,   311,   311,   312,   312,   313,   313,   314,   314,   315,
     315,   316,   316,   317,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   321,   321,
     321,   322,   322,   323,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   325,
     325,   326,   326,   327,   327,   328,   328,   329,   329,   330,
     330,   331,   331,   333,   334,   332,   335,   336,   337,   337,
     338,   338,   338,   340,   341,   339,   342,   342,   344,   343,
     345,   343,   346,   347,   347,   348,   350,   349,   352,   351,
     354,   353,   356,   355,   357,   358,   357,   359,   359,   359,
     360,   360,   360,   361,   361,   362,   363,   365,   364,   367,
     366,   368,   368,   369,   369,   370,   370,   371,   371,   372,
     372,   373,   373,   374,   374,   375,   375,   376,   376,   377,
     377,   378,   378,   379,   379,   379,   379,   379,   379,   379,
     380,   380,   380,   381,   381,   382,   382,   383,   384,   383,
     385,   386,   386,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   388,   388,   388,   389,   390,   389,   391,   392,   392,
     393,   393,   393,   393,   393,   393,   393,   393,   393,   393,
     393,   393,   393,   393,   393,   393,   393,   393,   393,   393,
     393,   393,   393,   393,   393,   393,   393,   393,   393,   393,
     393,   394,   395,   396,   396,   397,   397,   397,   398,   399,
     400,   401,   402,   403,   404,   404,   405,   406,   407,   407,
     408,   409,   410,   410,   411,   411,   411,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   412,   412,
     412,   412,   413,   414,   415,   415,   416,   417,   417,   418,
     419,   420,   420,   421,   422,   423,   425,   424,   426,   426,
     427,   427,   427,   427,   428,   428,   429,   429,   429,   429,
     429,   429,   430,   431,   431,   432,   433,   433,   434,   434,
     435,   435,   435,   436,   436,   437,   437,   437,   438,   438,
     438,   439,   440,   441,   441,   442,   442,   443,   443,   444,
     444,   445,   445,   446,   446,   446,   447,   447,   448,   449,
     449,   450,   451,   451,   452,   452,   453,   453,   454,   454,
     455,   455,   456,   456,   456,   457,   457,   457,   458,   458,
     459,   460,   460,   460,   461,   462,   462,   462,   462,   463,
     463,   463,   464,   464,   465,   465,   465,   466,   467,   467,
     467,   468,   468,   469,   469,   469,   470,   470,   470,   470,
     471,   471,   471,   472,   472,   472,   472,   472,   473,   473,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   475,   475,   476,   477,   477,   478,   478,   479,   479,
     480,   480,   481,   481,   482,   482,   482,   482,   482,   482,
     482,   482,   482,   482,   482,   482,   482,   482,   482,   482,
     482,   482,   482,   483,   483,   484,   485,   485,   486,   486,
     486,   487,   487,   487,   487,   488,   488,   489,   489,   490,
     490,   491,   491,   492,   492,   493
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
       1,     1,     1,     3,     3,     0,     5,     2,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     3,     3,
       3,     5,     7,     5,     8,     1,     3,     3,     5,     5,
       7,     7,     6,     5,     0,     2,     5,     3,    11,    13,
       9,     6,     0,     3,     1,     3,     3,     3,     3,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     3,     3,     1,     5,     9,     5,     7,     3,
       3,     5,     9,     5,     5,     5,     0,    15,     0,     1,
       7,    11,     9,    13,     3,     7,    11,    13,     9,     9,
       9,     7,     7,     5,     7,     5,     9,     7,     5,     9,
       5,     5,     3,     7,     3,     7,     7,     3,     5,     5,
       3,     5,     7,     9,     1,     1,     1,     1,     1,     0,
       2,     1,     3,     1,     1,     1,     0,     3,     1,     0,
       3,     1,     0,     3,     0,     3,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       4,     1,     1,     4,     1,     1,     4,     1,     4,     5,
       1,     3,     1,     3,     1,     1,     4,     9,     1,     1,
       4,     1,     5,     1,     1,     4,     1,     1,     5,     1,
       1,     1,     4,     1,     1,     5,     1,     1,     1,     3,
       1,     1,     3,     1,     4,     3,     3,     3,     3,     3,
       3,     1,     1,     3,     1,     3,     0,     1,     1,     1,
       1,     3,     0,     1,     1,     2,     2,     4,     6,     4,
       6,     6,     6,     6,     2,     6,     8,     8,    10,    14,
       2,     1,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    27,     0,     0,     1,
       5,     0,    31,     0,     7,     0,   160,     0,     0,     0,
       0,   230,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   315,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,     0,
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
       0,     0,     0,   480,     0,   483,     0,   525,     0,   481,
     502,    32,     0,   180,     0,     8,     0,   441,   442,     0,
     478,   187,     0,     0,     0,    12,   527,   526,    14,   450,
       0,   233,   234,     0,     0,   447,     0,     0,   201,   445,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   504,   523,     0,   474,   476,   477,
       0,   473,   471,     0,   274,   278,   470,   275,   467,   469,
       0,   466,   464,     0,   237,     0,   463,   406,   405,     0,
     428,   429,     0,     0,     0,     0,   317,   316,     0,   455,
       0,     0,   454,     0,     0,     0,     0,   543,     0,     0,
     374,     0,     0,     0,     0,     0,   359,   360,   408,   407,
       0,   158,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,   327,     0,     0,   353,   352,   530,   528,   529,
     192,   191,     0,     0,     0,     0,     0,     0,     0,   213,
     214,     0,     0,     0,     0,    16,   123,     0,     0,     0,
     392,   394,   397,   400,   153,     0,     0,     0,     0,   162,
       0,     0,     0,     0,     0,   499,   498,   500,   503,     0,
     536,   538,   535,   537,   539,   540,     0,     0,     0,   130,
     131,   126,   124,     0,     0,     0,     0,    31,   177,     0,
      29,     0,     0,     0,     0,     0,     0,   452,     0,     0,
       0,     0,     0,     0,     0,   505,   506,     0,     0,     0,
     514,     0,     0,     0,   520,     0,     0,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   159,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   184,
     183,     0,     0,     0,   178,     0,     0,     0,     0,     0,
       0,   496,   482,   490,     0,     0,   485,   486,   487,   488,
     489,     0,   156,     0,   480,     0,     0,     0,     0,   147,
     145,   151,   149,     0,     0,     0,   181,     0,   430,   431,
       9,     0,     0,   479,    11,   354,     0,    10,     0,     0,
     451,     0,     0,     0,   236,   235,   201,   202,   232,     0,
       0,   217,     0,     0,     0,     0,   461,     0,     0,   459,
       0,     0,   458,     0,     0,   522,   524,   365,     0,     0,
       0,   276,     0,     0,   239,   241,   311,   221,     0,    17,
     141,   313,     0,     0,     0,   363,     0,     0,   364,   361,
     434,     0,   432,     0,   433,     0,   388,   318,     0,   319,
       0,   204,     0,     0,     0,     0,   324,   323,     0,     0,
     193,   194,   401,   541,   542,     0,   326,     0,     0,     0,
     332,     0,     0,   206,     0,     0,   383,     0,   385,     0,
       0,     0,     0,   357,     0,    15,     0,   174,     0,     0,
     164,   391,   390,     0,     0,   398,   399,     0,   494,   497,
       0,   484,   161,   501,   125,     0,     0,   134,     0,   133,
       0,   132,     0,   138,     0,   129,     0,   128,     0,   127,
      33,   443,     0,     0,     0,   453,   446,     0,   448,     0,
     507,     0,     0,     0,   509,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   472,   533,   531,   532,     0,   289,
     286,   280,   306,     0,   305,     0,     0,     0,   304,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   285,     0,
     290,     0,   292,   293,   303,     0,   288,   279,   294,   534,
     282,     0,   465,   240,   212,     0,     0,     0,     0,   456,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,   322,     0,     0,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,   173,   175,     0,     0,     0,     0,
       0,   154,     0,   146,   148,   150,   152,     0,   139,     0,
       0,   143,     0,     0,     0,   449,   231,     0,   218,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   475,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   468,     0,     0,   312,
     142,    23,     0,   409,     0,     0,     0,     0,   439,   438,
     370,   375,     0,   320,     0,   208,     0,     0,   321,   325,
       0,     0,   402,     0,     0,     0,     0,   382,     0,   381,
       0,     0,     0,     0,   210,     0,   409,   384,   387,   216,
       0,   215,   186,   358,     0,   166,   393,   396,   395,   491,
     492,   493,   495,     0,     0,   137,   136,     0,   135,     0,
       0,     0,   508,   510,     0,   515,     0,   511,     0,   460,
     513,   512,     0,   309,   302,   296,   295,   297,   307,   308,
     310,   298,   299,   301,   283,   300,   284,   287,   291,   281,
       0,   435,   267,   268,   269,   244,   245,     0,   250,   248,
     249,   262,   263,   251,   252,   254,     0,     0,   264,   270,
       0,   266,     0,   258,   259,   261,   242,   246,   436,   243,
       0,   227,   228,     0,   229,     0,   226,   222,     0,   314,
       0,     0,   416,     0,     0,     0,   440,     0,     0,     0,
       0,     0,     0,   219,   220,     0,     0,     0,     0,   334,
       0,     0,     0,     0,     0,     0,     0,     0,   339,   340,
       0,     0,     0,   333,     0,     0,     0,     0,     0,     0,
     416,     0,     0,     0,     0,     0,     0,   166,   155,   140,
     144,     0,   355,     0,     0,     0,   462,     0,     0,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,   413,   414,   415,   410,   411,     0,   419,     0,     0,
     362,   372,     0,   389,   203,     0,   409,   403,   195,   330,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   378,   379,   380,   205,     0,   419,
     386,   221,   356,     0,   170,   165,   167,     0,     0,   516,
       0,   517,   486,   437,   253,   255,   256,   265,   257,   272,
     271,   260,   224,   225,   223,     0,     0,     0,     0,   424,
       0,     0,     0,     0,     0,   416,     0,     0,   347,   421,
     344,   346,   348,   349,   351,   350,   343,   335,   336,   337,
     338,   341,   342,   345,     0,     0,   424,   211,   168,    31,
     188,     0,     0,     0,     0,     0,     0,   457,   412,   418,
     417,     0,     0,   426,     0,   545,     0,   371,   207,   419,
       0,   328,   376,   209,   426,    31,   171,     0,     0,   518,
      20,     0,   273,   420,     0,     0,   422,   544,     0,   424,
       0,     0,   422,   169,     0,     0,     0,   425,     0,     0,
     366,   373,   426,   329,   377,   197,     0,     0,    21,   427,
       0,   368,   422,     0,     0,    25,    26,    23,     0,     0,
     423,   369,   367,   199,   198,     0,    13,    22,   519,     0,
     189,   200
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   103,   104,   105,   106,   107,
    1155,  1219,   979,  1217,    12,   178,   108,   428,   109,   110,
     111,   112,   236,   113,   114,   807,   599,   810,   535,   536,
     537,   538,   115,   521,   903,   116,   117,   489,   118,   119,
     237,   796,  1026,  1027,  1175,  1149,   120,   659,   121,   122,
     254,   123,   656,   124,   426,   125,   382,   510,   651,   126,
     127,   128,   358,   350,   129,  1213,   130,  1229,   444,   623,
     645,   867,   886,   131,   381,   890,   572,   995,   754,   977,
     132,   277,   566,   133,   465,   314,   753,   966,  1111,   134,
     462,   304,   461,   747,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   775,   145,   146,   147,   148,   149,
     782,  1013,  1136,   150,   151,   557,   152,   153,   154,   155,
     156,   157,   158,   159,   160,  1211,  1222,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   319,   351,   982,  1054,  1055,  1057,  1160,  1119,
    1130,  1200,  1163,  1186,   322,   550,   613,   967,   860,   614,
     260,   355,   294,   279,   438,   331,   332,   581,   582,   315,
     316,   305,   306,   405,   356,   901,   668,   669,   670,   407,
     408,   409,   295,   456,   249,   268,   371,   750,   423,   424,
     425,   635,   336,   337
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1100
static const yytype_int16 yypact[] =
{
      89,  -123,  -114,   163, -1100,    89,    28,   -59,   -38, -1100,
   -1100,   -35,   862,   -21, -1100,    99, -1100,    78,    83,   111,
     116, -1100,   118,   124,   128,   139,   149,   156,   160,   175,
     184,   186,   196,   197,   202,   205,   209,   210,   213,   218,
     219,   220,   226,   227,   229,   230,   237,   238,   240,   243,
     249,   263,   266,   267,   273,   274,   278,   286,   289,   290,
     296,   299,   300,   301,   305,   307,   313,    26,   319,   320,
     324, -1100,   327,    -6,   696, -1100, -1100,   328,   340,   341,
     342,   -26,    48,     9, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,   862,
   -1100, -1100,    -7, -1100, -1100, -1100, -1100, -1100,   353, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100,   338,   164, -1100,  -106,
     321,    76,   -43,   216,   932,   103,   103,   145,   -51,    31,
     -28,   369,   -28,   913,  -116,  -116,   -20,   235,   -28,   -28,
     310,   -28,   136,  -116,  -116,   -41,   -20,   -20,   -20,    48,
     203,    48,   -28,   932,   932,   -28,   -28,   932,   932,    87,
     -28,   932,   -28,   -41,   932,  -116,   932,    69, -1100,   932,
    -116,   352,   565,    48, -1100, -1100,   207,   279,   -28,   -28,
     -28,   -28,   359, -1100,    18, -1100,   288, -1100,   204, -1100,
     113, -1100,   176, -1100,   349,   364,    99, -1100, -1100,   365,
   -1100,  -148,   379,   380,   385, -1100, -1100, -1100, -1100, -1100,
     387, -1100, -1100,   382,   621, -1100,   388,   389,   393, -1100,
   -1100,  -116,  -116,   -28,   -28,   394,   -28,   400,   402,   404,
     932,   410,   630, -1100, -1100,   333,   413, -1100, -1100, -1100,
     648, -1100, -1100,   424, -1100, -1100, -1100, -1100, -1100, -1100,
     669, -1100, -1100,   434,   433,   443, -1100, -1100, -1100,   445,
   -1100, -1100,   446,   450,   455,   456, -1100, -1100,   690, -1100,
     466,   459, -1100,   471,   470,   707,   475, -1100,   481,   484,
     494,   495,   508,   742,   510,   513, -1100, -1100, -1100, -1100,
     514,   746,   516,   517,   518,   519,   376,   754,   522,   179,
     523,   524, -1100,   526,   527, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100,   528,   529,   530,   531,   532,   533,   535, -1100,
   -1100,   537,   349,   538,   539,   540, -1100,   549,    48,    48,
     556,   558,   566,   567, -1100,   552,   302,    48,    48, -1100,
      48,    48,    48,    48,    48,   505,   376, -1100,   568,   569,
   -1100, -1100, -1100, -1100, -1100, -1100,   570,    45,    38, -1100,
   -1100,   505,   376,   581,   583,   585,   862,   862, -1100,    23,
   -1100,    48,   791,  -106,   804,    13,   822,   592,   593,   932,
     600,    48,   265,   834,   594, -1100, -1100,   604,   605,   821,
   -1100,   -28,   -28,   261, -1100,   609,   606,   932,   737,   610,
      48,   611,   349,   613,    48,   349,   -28,   -28,   932,   768,
      51,   776,   628,    48,    23,   825,   863,   629,   826,   827,
     169,   670,   -28,   775,   638,   779,   -20,   -34, -1100,   641,
     -20,   -20,   -20,    48,   642,    75,   -28,    46,   777,   780,
      -8,   674,   772,   787,   -12,    59,   758,   763,    31,   774,
   -1100,    41,    41,   757, -1100,  -104,   683,   -33,   778,   782,
     183,   722, -1100, -1100,   291,   363,    98,    98, -1100, -1100,
   -1100,   113, -1100,   932,   698,  -130,   -84,   -82,   -80, -1100,
   -1100,   505,   376,    10,   140,   135, -1100,   692, -1100, -1100,
   -1100,   368,   703, -1100, -1100, -1100,   940, -1100,   708,   387,
   -1100,   706,   943,   406, -1100, -1100,   393, -1100, -1100,   -28,
     -28,   651,   714,   713,   718,   720, -1100,   724,   557, -1100,
     726,   728, -1100,   731,   732, -1100, -1100, -1100,   729,   411,
     555, -1100,   749,   422, -1100, -1100, -1100, -1100,   734,   736,
   -1100,   759,   959,   449,   760, -1100,   761,   991, -1100,   762,
   -1100,   764, -1100,   767, -1100,   770,   769, -1100,   996, -1100,
     783, -1100,  1007,   786,    23,   788,   789, -1100,   792,   924,
   -1100, -1100, -1100, -1100, -1100,   793, -1100,   794,   795,   781,
   -1100,   798,   802, -1100,  1013,   810, -1100,   818, -1100,   819,
     820,   888,  1055,   824,   828, -1100,   349, -1100,   765,    48,
   -1100, -1100,   505,   830,   831, -1100, -1100,   832, -1100,   835,
     823, -1100, -1100, -1100, -1100,  1060,   839, -1100,   -17, -1100,
      48, -1100,  -106, -1100,    30, -1100,    51, -1100,    42, -1100,
   -1100, -1100,  1043,   836,  1066, -1100, -1100,   840, -1100,   837,
   -1100,   847,   983,   932, -1100,    48,    48,   932,   848,    48,
     932,   932,   849,   846, -1100, -1100, -1100, -1100,   851, -1100,
   -1100, -1100, -1100,   852, -1100,   854,   855,   856, -1100,   857,
     858,   859,   860,   861,   864,   866,   867,   868, -1100,   869,
   -1100,   870, -1100, -1100, -1100,   873, -1100, -1100, -1100, -1100,
   -1100,   872, -1100,   850,   876,    31,    51,  1095,   878, -1100,
     -41,  1097,   902,   945,  1144,    56,   145,   947,   -96,  1052,
     914,    -3, -1100,   923,  1071, -1100,    48,   921,  -106,  1068,
    1098,   -28,   938,  1069,    60,   939,    -2,  1070,  1175,   109,
     349,  1072,    41, -1100, -1100,   376,   941,    23,   -14,    86,
     722, -1100,   -69, -1100, -1100,   376,   505,   -42, -1100,     1,
       3, -1100,   944,    23,   948, -1100, -1100,    48, -1100,   942,
     168,   282,   946,    23,   506,   949,   950,    48, -1100,    48,
     103,  1067,  1074,    48,  1064,  1059,  1063,    48,    48,    48,
    -106,  1028,    48,    48,    48,   145, -1100,  1075,   384, -1100,
   -1100,   960,  1194,   962,   963,  1197,   965,   964, -1100, -1100,
     967, -1100,   969, -1100,  1201, -1100,   287,   971, -1100, -1100,
     972,    93,   505,   973,   977,   978,   345, -1100,   979, -1100,
     980,   984,   985,  1216, -1100,  1219,   962, -1100,   987, -1100,
     989, -1100, -1100, -1100,   990,    79, -1100, -1100,   505, -1100,
   -1100, -1100, -1100,   349,    30, -1100, -1100,    42, -1100,  1211,
     992,  1224,   376, -1100,  1182, -1100,    48, -1100,   993, -1100,
   -1100, -1100,   435, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100,   505, -1100, -1100, -1100, -1100, -1100,
      50, -1100, -1100, -1100, -1100, -1100, -1100,  -106, -1100, -1100,
   -1100, -1100, -1100, -1100,   997, -1100,   998,   999, -1100, -1100,
    1000, -1100,  1001, -1100, -1100,  1002, -1100, -1100, -1100,   505,
    1003, -1100, -1100,  1004, -1100,  1005, -1100, -1100,  1228, -1100,
    1008,   377,  1122,  1240,  1009,   145, -1100,   -29,   145,  1010,
    1011,    -2,    48, -1100, -1100,  1012,  1148,  1174,  1150, -1100,
    1016,  1017,  1019,  1020,  1038,  1039,  1040,  1041, -1100, -1100,
    1042,  1044,  1045, -1100,  1136,  1166,  1169,  1170,  1046,  1050,
    1122,  1284,    93,  1282,   242,  1053,  1048,    79, -1100, -1100,
   -1100,  1056, -1100,  1057,  1058,   277, -1100,    48,  1206,   505,
      48,    48,    48,    48,    48,  -103,   103,  1286,  1138, -1100,
    1293, -1100, -1100, -1100, -1100,  1062,  1065,  1191,  1073,  1297,
   -1100,  1076,  1077, -1100, -1100,   265,   962, -1100, -1100, -1100,
    1079,  1080,  1203,  1198,  1298,    62,   145,   103,    13,    13,
     -28,   -51,  1301,  1081, -1100, -1100, -1100, -1100,  1303,  1191,
   -1100, -1100, -1100,  1083, -1100, -1100, -1100,    90,  1304, -1100,
    1260, -1100,   295, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
    1014, -1100, -1100, -1100, -1100,  1082,   377,  1200,  1085,  1291,
    1310,  1084,  1123,   145,  1086,  1122,  1225,  1230, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100,  1214,  1087,  1291,   876, -1100,   862,
   -1100,  1096,  1099,  1094,  1100,  1101,  -103, -1100, -1100, -1100,
   -1100,  1198,  1102,  1308,  1103, -1100,  1104, -1100, -1100,  1191,
    1107, -1100,  1108, -1100,  1308,   862, -1100,  1319,    23, -1100,
   -1100,  1109, -1100, -1100,  1232,  1110,  1145, -1100,   145,  1291,
    1250,    23,  1145, -1100,  1113,  1114,   -28, -1100,  1348,  1115,
   -1100, -1100,  1308, -1100, -1100, -1100,  1350,    95,  1118, -1100,
     145,   349,  1145,   349,  1119, -1100, -1100,   960,   -28,  1117,
   -1100, -1100, -1100, -1100, -1100,  1354, -1100, -1100, -1100,   349,
   -1100, -1100
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1100, -1100,  1355, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100,   142, -1100, -1100,  1105,  -109,  -380,   936,  1131,
    1290,  -496, -1100, -1100, -1100, -1100,   679, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,  1299, -1100,
   -1100, -1100,   343, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100,   874,  1149, -1100, -1100, -1100, -1100,   801, -1100,
   -1100, -1100,   383, -1100, -1100, -1100,  -635,   346,   280, -1100,
   -1100,   507,   311, -1100, -1100, -1100, -1100, -1100,   221, -1100,
   -1100,  1189, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100,  -661, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100, -1100,
   -1100, -1100,   297,   619,  -856,   264, -1100,  -944, -1100, -1008,
     222, -1099, -1058, -1084,  -493,  -468, -1100, -1100, -1100,   534,
     951,  -230,  -181,  -404,   829,   225,  -394,  -475,  -648,  -536,
    -458,  -533,  -665,  -174,   -78, -1100,   582, -1100, -1100,   865,
   -1100, -1100,  1111,   -92,   796, -1100,  -488, -1100, -1100, -1100,
   -1100, -1100,  -179, -1100
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -239
static const yytype_int16 yytable[] =
{
     251,   278,   509,   387,   248,   261,   604,   631,   687,   323,
     685,   325,   600,   297,   539,   650,   555,   339,   340,   808,
     342,   344,   243,   345,   540,   667,   395,   352,   353,   354,
    1020,   360,    16,   297,   363,   364,   653,   654,   317,   372,
     811,   374,   243,   334,   576,   576,   269,   270,   348,   534,
     271,   272,   243,   395,   308,   621,   661,   390,   391,   392,
     393,   269,   270,   646,   878,   638,  1132,   818,  1061,   657,
     689,   419,   269,   270,   379,   897,  1089,   643,   421,   367,
     266,  1146,   591,   368,   369,   594,   865,   884,  1174,   432,
    1192,   367,   296,  1205,  1150,   368,   369,   993,  1109,   298,
    1215,   327,   447,   448,   548,   450,   297,   676,  1151,     1,
       2,   320,   549,  1223,   677,     7,   299,   243,  1212,   298,
     321,   361,   362,   328,     8,   365,   366,   269,   270,   373,
     633,  1202,   376,   359,   378,   634,   299,   383,   576,   257,
     309,   855,  1110,   308,   815,   858,   329,   330,   308,   433,
     258,   259,   859,   678,   891,   680,   772,   682,   380,   516,
     679,  1189,   681,     9,   683,  1133,   396,  1134,   852,    11,
     647,   815,   406,  1062,   422,   879,   370,   880,   881,   882,
     243,  1169,   994,   267,  1216,   273,    13,   400,   401,   402,
     403,   404,   298,   658,   318,   904,   274,   571,   889,   334,
     273,   639,   905,    15,   349,   622,   410,    14,   411,   299,
    1125,   274,   257,   412,   413,   414,   176,   275,   276,   335,
    1135,   242,   274,   258,   259,   269,   270,   644,   600,   309,
     861,   257,   275,   276,   309,   233,   866,   885,   756,  1029,
     907,   177,   258,   259,   541,   906,   367,   908,   250,   300,
     368,   369,   556,   684,  1024,   301,  1025,   244,   610,  1030,
     611,   612,   849,   628,   576,   269,   270,    82,   577,   300,
     574,   575,   245,   246,   804,   301,   793,   417,   302,   303,
     652,   652,   803,   257,   244,   595,   596,   244,  1038,   252,
     310,   764,   245,   246,   258,   259,   311,   924,   850,   245,
     246,   616,   245,   246,   667,   579,   580,   620,   247,   939,
     515,   625,   626,   627,   357,   632,   179,   894,   547,   524,
     525,   180,   526,   527,   528,   529,   530,   280,   247,   896,
     415,   262,   263,   264,   265,   247,   899,   900,   247,   396,
     542,   255,   300,   662,   665,   910,   666,   561,   301,   181,
     269,   270,   244,   551,   182,   918,   183,   573,   257,   564,
     565,   583,   184,   563,   273,   586,   185,   245,   246,   258,
     259,   302,   303,   324,   652,   343,   597,   186,   688,   310,
     271,   272,   589,   686,   310,   311,   593,   187,   700,   701,
     311,   402,   403,   404,   188,   603,   275,   276,   189,   579,
     580,   256,   416,   247,   273,   914,   312,   313,   915,   748,
     892,   312,   313,   190,   341,   417,   496,  1139,  1140,   418,
     333,   257,   191,   398,   192,  1051,  1052,  1053,   346,   347,
     245,   246,   258,   259,   193,   194,   275,   276,   329,   330,
     195,   674,   357,   196,   999,  1000,   399,   197,   198,  1060,
     377,   199,  1063,   406,  1001,   384,   200,   201,   202,   400,
     401,   402,   403,   404,   203,   204,   247,   205,   206,   970,
     400,   401,   402,   403,   404,   207,   208,   420,   209,  1002,
     338,   210,   281,   282,   283,   284,   285,   211,   286,   273,
     287,   288,   289,   290,   291,   400,   401,   402,   403,   404,
     578,   212,  1003,   388,   213,   214,   445,   446,   806,   971,
     972,   215,   216,  1112,  1100,   385,   217,  1101,   973,   916,
     389,   275,   276,  1028,   218,   579,   580,   219,   220,   397,
     974,   293,  1154,   671,   221,   -19,  1093,   222,   223,   224,
    1137,   975,   523,   225,  1138,   226,   870,  1004,  1005,  1006,
    1007,   227,  1008,  1009,  1010,  1011,  1012,   229,   230,   715,
     708,   440,   231,   716,   717,   232,   238,   976,   400,   401,
     402,   403,   404,   400,   401,   402,   403,   404,   239,   240,
     241,   795,   400,   401,   402,   403,   404,  1167,   402,   403,
     404,   253,   427,   400,   401,   402,   403,   404,   394,   923,
     875,   429,   805,   927,   872,   672,   431,   931,   932,   933,
     691,   819,   936,   937,   938,   822,   434,   435,   825,   826,
     718,   439,   436,   437,   898,   440,   442,   820,   821,   441,
     443,   824,   457,   449,   440,   719,   720,   721,   722,   451,
     723,   452,   724,   453,   269,   270,   269,   270,   698,   455,
     458,   459,  1201,   714,   400,   401,   402,   403,   404,   400,
     401,   402,   403,   404,   752,   460,   934,   400,   401,   402,
     403,   404,   463,   969,  1220,   464,  -238,   725,   726,   727,
     466,   728,   467,   468,   729,   730,   731,   469,   732,   733,
     734,   759,   470,   471,   472,   735,   474,   400,   401,   402,
     403,   404,   400,   401,   402,   403,   404,   473,   475,   476,
    1195,   477,   478,   400,   401,   402,   403,   404,   479,   269,
     270,   480,   736,  1204,   737,   738,   400,  1037,   402,   403,
     404,   481,   482,   739,   740,   741,   742,   743,   744,   912,
     400,   401,   402,   403,   404,   483,   484,   485,   919,   922,
     486,   487,   488,   490,   491,   492,   493,   745,   494,   495,
     497,   498,  1067,   499,   500,   501,   502,   503,   504,   505,
     506,   746,   507,  1039,   508,   511,   512,   513,   281,   282,
     283,   284,   285,   273,   286,   273,   287,   288,   289,   290,
     291,   514,   522,   517,   274,   518,   292,   400,   401,   402,
     403,   404,   433,   519,   520,   531,   552,   554,   533,   532,
    1104,  1105,  1106,  1107,  1108,   275,   276,   275,   276,   543,
      86,   544,    88,   545,    90,   558,    92,   293,    94,   559,
      96,  1221,    98,  1224,   100,   560,   102,   562,  1035,   567,
     568,   569,   570,   584,   587,   247,   585,   588,   590,  1231,
     592,   281,   282,   283,   284,   285,   598,   286,   273,   287,
     288,   289,   290,   291,   601,   602,   607,   606,    16,   292,
     605,   608,   615,   617,   609,   618,    17,   619,   624,   629,
     640,   637,   636,   641,    18,    19,    20,    21,    22,    23,
     275,   276,    24,    25,    26,    27,    28,    29,    30,  1141,
     293,   642,    31,    32,    33,    34,    35,   648,    36,   571,
     269,   270,   649,    37,    38,    39,    40,    41,    42,    43,
      44,   655,    45,    46,    47,   660,    48,  -185,    49,    50,
      51,    52,    53,    54,    55,   675,   690,   663,    56,    57,
     692,    58,   664,   693,   234,   694,   696,   697,    59,   702,
      85,   703,    87,   704,    89,   705,    91,   706,    93,  1102,
      95,   707,    97,   758,    99,   710,   101,   709,   711,   712,
      84,   755,    60,   756,   713,    61,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   751,   762,   757,   760,   761,   763,
     768,    62,   269,   270,   765,   764,   767,    63,   766,    64,
      65,   770,   777,    66,    67,  1208,    68,   785,    69,   781,
     769,   269,   270,   771,    70,   773,   774,    71,    72,   776,
     778,   779,   780,    73,    74,   783,    75,  1227,    76,   784,
    1176,   790,   281,   282,   283,   284,   285,   786,   286,   273,
     287,   288,   289,   290,   291,   787,   788,   789,   708,   812,
     292,   791,   794,   801,   802,   792,  1193,   797,   798,   814,
     799,   571,   800,   813,    77,    78,    79,    80,   328,    81,
     815,   275,   276,   816,   817,   823,   828,   847,   827,   829,
     830,   293,   831,   832,   833,   834,   835,   836,   837,   838,
     851,   854,   839,    82,   840,   841,   842,   843,   844,    83,
      84,   845,   846,   848,   326,   852,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   281,   282,   283,   284,   285,   855,
     286,   273,   287,   288,   289,   290,   291,   856,   857,   862,
     863,   864,   292,   281,   282,   283,   284,   285,   871,   286,
     273,   287,   288,   289,   290,   291,   940,   941,   868,   869,
     873,   292,   874,   275,   276,   876,   883,   877,   887,   888,
     893,   909,   913,   293,   895,   911,   917,   929,   925,   920,
     921,   928,   275,   276,   926,   930,   935,   978,   980,   981,
     983,   984,   293,   985,   987,   989,   986,   988,   991,   992,
     996,   942,   943,   944,   997,   998,  1014,  1015,   945,   946,
    1018,  1016,  1017,  1019,  1021,   947,  1022,  1023,  1031,  1033,
    1034,  1049,  1032,  1036,  1056,  1040,  1041,  1042,  1043,  1044,
    1045,  1046,  1047,  1048,  1058,  1050,  1059,  1069,  1065,  1070,
    1064,  1083,  1068,  1071,  1072,  1073,   948,  1074,  1075,   949,
     950,   951,   952,   953,   954,   955,   956,   957,   958,   959,
     960,   961,   962,   963,   964,   965,  1076,  1077,  1078,  1079,
    1080,  1084,  1081,  1082,  1085,  1086,  1087,  1088,  1090,  1092,
    1113,  1094,  1095,  1097,  1098,  1103,  1114,  1115,  1099,  1116,
    1118,  1121,  1131,  1117,  1128,  1143,  1129,  1145,  1153,  1152,
    1120,  1159,  1156,  1122,  1164,  1123,  1126,  1127,  1144,  1162,
     257,  1148,  1157,  1161,  1165,  1166,  1168,  1173,  1170,  1171,
    1172,   258,   259,  1177,  1179,  1180,  1178,  1185,  1194,  1197,
    1184,  1181,  1188,  1187,  1190,  1191,  1196,  1199,  1198,  1203,
    1206,  1207,  1209,  1210,  1214,  1218,  1225,  1228,  1230,  1226,
      10,   430,   546,   386,   235,   809,   228,   699,  1091,   630,
    1096,  1147,   375,   990,  1066,   307,  1124,  1182,  1142,   853,
    1158,   968,   902,  1183,   553,     0,   749,     0,   695,     0,
       0,     0,     0,     0,     0,     0,   673,     0,     0,     0,
       0,   454
};

static const yytype_int16 yycheck[] =
{
     109,   182,   382,   233,    82,   179,   474,   495,   544,   190,
     543,   192,   470,     3,   418,   508,     3,   198,   199,   684,
     201,   202,     4,   202,   418,   521,     8,   206,   207,   208,
     886,   212,     6,     3,   215,   216,   511,   512,    89,   220,
     688,   222,     4,    63,     3,     3,    89,    90,    89,     4,
      93,    94,     4,     8,     3,    89,    89,   238,   239,   240,
     241,    89,    90,     4,     4,    73,     4,   702,    97,   173,
     545,   252,    89,    90,     5,    89,  1020,    89,   252,     4,
       4,  1089,   462,     8,     9,   465,    89,    89,  1146,   237,
    1174,     4,   184,  1192,     4,     8,     9,     4,   201,    89,
       5,   193,   283,   284,    81,   286,     3,   237,    18,    20,
      21,    80,    89,  1212,   244,   238,   106,     4,  1202,    89,
      89,   213,   214,   239,   238,   217,   218,    89,    90,   221,
      84,  1189,   224,   211,   226,    89,   106,   229,     3,   245,
      89,   237,   245,     3,   240,    89,   262,   263,     3,   297,
     256,   257,    96,   237,   789,   237,   624,   237,    89,   389,
     244,  1169,   244,     0,   244,   103,   244,   105,   237,   141,
     111,   240,   250,   202,   252,   115,    89,   117,   118,   119,
       4,  1125,    89,   107,    89,   228,   245,   291,   292,   293,
     294,   295,    89,   297,   245,   237,   239,    88,    89,    63,
     228,   209,   244,   238,   245,   239,    30,   245,    32,   106,
    1066,   239,   245,    37,    38,    39,   237,   260,   261,   239,
     158,   247,   239,   256,   257,    89,    90,   239,   686,    89,
     766,   245,   260,   261,    89,   241,   239,   239,   237,   904,
     237,   142,   256,   257,   418,   244,     4,   244,   239,   239,
       8,     9,   239,   243,   175,   245,   177,   239,    89,   907,
      91,    92,   755,   493,     3,    89,    90,   241,     7,   239,
     451,   452,   254,   255,   678,   245,   656,   239,   268,   269,
     239,   239,   676,   245,   239,   466,   467,   239,   238,   296,
     239,   241,   254,   255,   256,   257,   245,   830,   756,   254,
     255,   482,   254,   255,   800,   264,   265,   486,   290,   845,
     388,   490,   491,   492,   239,   496,   238,   792,   427,   397,
     398,   238,   400,   401,   402,   403,   404,   111,   290,   797,
     154,    10,    11,    12,    13,   290,   250,   251,   290,   417,
     418,     3,   239,   517,   161,   813,   163,   439,   245,   238,
      89,    90,   239,   431,   238,   823,   238,   449,   245,    94,
      95,   453,   238,   441,   228,   457,   238,   254,   255,   256,
     257,   268,   269,     4,   239,   239,   468,   238,   243,   239,
      93,    94,   460,   243,   239,   245,   464,   238,   569,   570,
     245,   293,   294,   295,   238,   473,   260,   261,   238,   264,
     265,   237,   226,   290,   228,   237,   266,   267,   240,   590,
     790,   266,   267,   238,   104,   239,   237,  1078,  1079,   243,
     195,   245,   238,   219,   238,    48,    49,    50,   203,   204,
     254,   255,   256,   257,   238,   238,   260,   261,   262,   263,
     238,   533,   239,   238,    99,   100,   242,   238,   238,   985,
     225,   238,   988,   531,   109,   230,   238,   238,   238,   291,
     292,   293,   294,   295,   238,   238,   290,   238,   238,    85,
     291,   292,   293,   294,   295,   238,   238,   252,   238,   134,
     245,   238,   221,   222,   223,   224,   225,   238,   227,   228,
     229,   230,   231,   232,   233,   291,   292,   293,   294,   295,
     239,   238,   157,   296,   238,   238,   281,   282,   682,   125,
     126,   238,   238,  1046,   237,   163,   238,   240,   134,   237,
     241,   260,   261,   903,   238,   264,   265,   238,   238,   241,
     146,   270,   237,   242,   238,   240,  1024,   238,   238,   238,
    1076,   157,   240,   238,  1077,   238,   776,   202,   203,   204,
     205,   238,   207,   208,   209,   210,   211,   238,   238,     4,
       3,     4,   238,     8,     9,   238,   238,   183,   291,   292,
     293,   294,   295,   291,   292,   293,   294,   295,   238,   238,
     238,   659,   291,   292,   293,   294,   295,  1123,   293,   294,
     295,   238,   243,   291,   292,   293,   294,   295,   239,   829,
     781,   237,   680,   833,   778,   242,   241,   837,   838,   839,
     242,   703,   842,   843,   844,   707,   237,   237,   710,   711,
      65,   239,   237,   236,   798,     4,   237,   705,   706,   241,
     237,   709,   299,   239,     4,    80,    81,    82,    83,   239,
      85,   239,    87,   239,    89,    90,    89,    90,   242,   239,
     237,     3,  1188,   242,   291,   292,   293,   294,   295,   291,
     292,   293,   294,   295,   242,   241,   840,   291,   292,   293,
     294,   295,     3,   847,  1210,   241,   243,   122,   123,   124,
     237,   126,   237,   237,   129,   130,   131,   237,   133,   134,
     135,   242,   237,   237,     4,   140,   237,   291,   292,   293,
     294,   295,   291,   292,   293,   294,   295,   241,   237,   239,
    1178,     4,   237,   291,   292,   293,   294,   295,   237,    89,
      90,   237,   167,  1191,   169,   170,   291,   292,   293,   294,
     295,   237,   237,   178,   179,   180,   181,   182,   183,   817,
     291,   292,   293,   294,   295,   237,     4,   237,   242,   827,
     237,   237,     6,   237,   237,   237,   237,   202,     4,   237,
     237,   237,   992,   237,   237,   237,   237,   237,   237,   237,
     237,   216,   237,   947,   237,   237,   237,   237,   221,   222,
     223,   224,   225,   228,   227,   228,   229,   230,   231,   232,
     233,   242,   240,   237,   239,   237,   239,   291,   292,   293,
     294,   295,   297,   237,   237,   237,    15,     3,   238,   240,
    1040,  1041,  1042,  1043,  1044,   260,   261,   260,   261,   238,
     255,   238,   257,   238,   259,     3,   261,   270,   263,   237,
     265,  1211,   267,  1213,   269,   242,   271,   237,   916,     5,
     246,   237,   237,   234,   107,   290,   240,   237,   237,  1229,
     237,   221,   222,   223,   224,   225,    88,   227,   228,   229,
     230,   231,   232,   233,    88,   237,   237,     4,     6,   239,
      45,    45,   202,    98,    47,   237,    14,    98,   237,   237,
     206,   101,   105,   111,    22,    23,    24,    25,    26,    27,
     260,   261,    30,    31,    32,    33,    34,    35,    36,  1080,
     270,   114,    40,    41,    42,    43,    44,   149,    46,    88,
      89,    90,   149,    51,    52,    53,    54,    55,    56,    57,
      58,   164,    60,    61,    62,   242,    64,   153,    66,    67,
      68,    69,    70,    71,    72,   237,   244,   159,    76,    77,
     237,    79,   160,     3,   248,   237,   240,     4,    86,   298,
     254,   237,   256,   240,   258,   237,   260,   237,   262,  1037,
     264,   237,   266,     4,   268,   237,   270,   241,   237,   237,
     248,   237,   110,   237,   245,   113,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   245,     4,   237,   237,   237,   237,
       4,   139,    89,    90,   237,   241,   237,   145,   238,   147,
     148,     4,    88,   151,   152,  1196,   154,     4,   156,   238,
     237,    89,    90,   237,   162,   237,   237,   165,   166,   237,
     237,   237,   237,   171,   172,   237,   174,  1218,   176,   237,
    1149,   153,   221,   222,   223,   224,   225,   237,   227,   228,
     229,   230,   231,   232,   233,   237,   237,   237,     3,    16,
     239,   237,   297,   240,     4,   237,  1175,   237,   237,     3,
     238,    88,   237,   237,   212,   213,   214,   215,   239,   217,
     240,   260,   261,   246,   237,   237,   240,   237,   239,   238,
     238,   270,   238,   238,   238,   238,   238,   238,   238,   238,
       5,     4,   238,   241,   238,   238,   238,   238,   238,   247,
     248,   238,   240,   237,   201,   237,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   221,   222,   223,   224,   225,   237,
     227,   228,   229,   230,   231,   232,   233,   202,     4,   202,
      98,   237,   239,   221,   222,   223,   224,   225,   237,   227,
     228,   229,   230,   231,   232,   233,    91,    92,   245,    98,
     102,   239,    74,   260,   261,   237,   237,   108,   108,     4,
     108,   237,   240,   270,   243,   237,   240,   128,   121,   240,
     240,   127,   260,   261,   120,   132,   168,   237,     4,   237,
     237,     4,   270,   238,   237,     4,   242,   238,   237,   237,
     237,   136,   137,   138,   237,   237,   237,   237,   143,   144,
       4,   237,   237,     4,   237,   150,   237,   237,    17,     5,
      48,     3,   240,   240,   112,   238,   238,   238,   238,   238,
     238,   238,   238,   238,     4,   237,   237,    99,   237,    75,
     240,   115,   240,   103,   238,   238,   181,   238,   238,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   238,   238,   238,   238,
     238,   115,   238,   238,   115,   115,   240,   237,     4,     7,
       4,   238,   244,   237,   237,    89,   158,     4,   240,   237,
     109,     4,     4,   238,   101,     4,   108,     4,    48,     5,
     237,   111,   298,   237,     4,   238,   237,   237,   237,    28,
     245,   238,   240,   238,   240,   202,   240,   240,   103,    99,
     116,   256,   257,   237,   240,   235,   237,    29,    19,   107,
     238,   240,   238,   240,   237,   237,   237,   202,   238,    99,
     237,   237,     4,   238,     4,   237,   237,   240,     4,  1217,
       5,   256,   426,   232,    74,   686,    67,   566,  1022,   495,
    1027,  1091,   223,   866,   991,   186,  1065,  1156,  1081,   760,
    1116,   847,   800,  1161,   433,    -1,   590,    -1,   559,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   531,    -1,    -1,    -1,
      -1,   290
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    20,    21,   301,   302,   303,   304,   238,   238,     0,
     302,   141,   314,   245,   245,   238,     6,    14,    22,    23,
      24,    25,    26,    27,    30,    31,    32,    33,    34,    35,
      36,    40,    41,    42,    43,    44,    46,    51,    52,    53,
      54,    55,    56,    57,    58,    60,    61,    62,    64,    66,
      67,    68,    69,    70,    71,    72,    76,    77,    79,    86,
     110,   113,   139,   145,   147,   148,   151,   152,   154,   156,
     162,   165,   166,   171,   172,   174,   176,   212,   213,   214,
     215,   217,   241,   247,   248,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   305,   306,   307,   308,   309,   316,   318,
     319,   320,   321,   323,   324,   332,   335,   336,   338,   339,
     346,   348,   349,   351,   353,   355,   359,   360,   361,   364,
     366,   373,   380,   383,   389,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   403,   405,   406,   407,   408,   409,
     413,   414,   416,   417,   418,   419,   420,   421,   422,   423,
     424,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   237,   142,   315,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   338,   238,
     238,   238,   238,   241,   248,   320,   322,   340,   238,   238,
     238,   238,   247,     4,   239,   254,   255,   290,   474,   484,
     239,   316,   296,   238,   350,     3,   237,   245,   256,   257,
     460,   473,    10,    11,    12,    13,     4,   107,   485,    89,
      90,    93,    94,   228,   239,   260,   261,   381,   462,   463,
     111,   221,   222,   223,   224,   225,   227,   229,   230,   231,
     232,   233,   239,   270,   462,   482,   483,     3,    89,   106,
     239,   245,   268,   269,   391,   471,   472,   391,     3,    89,
     239,   245,   266,   267,   385,   469,   470,    89,   245,   442,
      80,    89,   454,   462,     4,   462,   201,   483,   239,   262,
     263,   465,   466,   465,    63,   239,   492,   493,   245,   462,
     462,   104,   462,   239,   462,   492,   465,   465,    89,   245,
     363,   443,   492,   492,   492,   461,   474,   239,   362,   474,
     462,   483,   483,   462,   462,   483,   483,     4,     8,     9,
      89,   486,   462,   483,   462,   363,   483,   465,   483,     5,
      89,   374,   356,   483,   465,   163,   319,   461,   296,   241,
     462,   462,   462,   462,   239,     8,   474,   241,   219,   242,
     291,   292,   293,   294,   295,   473,   474,   479,   480,   481,
      30,    32,    37,    38,    39,   154,   226,   239,   243,   462,
     465,   473,   474,   488,   489,   490,   354,   243,   317,   237,
     315,   241,   237,   297,   237,   237,   237,   236,   464,   239,
       4,   241,   237,   237,   368,   465,   465,   462,   462,   239,
     462,   239,   239,   239,   482,   239,   483,   299,   237,     3,
     241,   392,   390,     3,   241,   384,   237,   237,   237,   237,
     237,   237,     4,   241,   237,   237,   239,     4,   237,   237,
     237,   237,   237,   237,     4,   237,   237,   237,     6,   337,
     237,   237,   237,   237,     4,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   317,
     357,   237,   237,   237,   242,   474,   461,   237,   237,   237,
     237,   333,   240,   240,   474,   474,   474,   474,   474,   474,
     474,   237,   240,   238,     4,   328,   329,   330,   331,   463,
     466,   473,   474,   238,   238,   238,   318,   316,    81,    89,
     455,   474,    15,   460,     3,     3,   239,   415,     3,   237,
     242,   483,   237,   474,    94,    95,   382,     5,   246,   237,
     237,    88,   376,   483,   462,   462,     3,     7,   239,   264,
     265,   467,   468,   483,   234,   240,   483,   107,   237,   474,
     237,   317,   237,   474,   317,   462,   462,   483,    88,   326,
     470,    88,   237,   474,   455,    45,     4,   237,    45,    47,
      89,    91,    92,   456,   459,   202,   462,    98,   237,    98,
     492,    89,   239,   369,   237,   492,   492,   492,   461,   237,
     362,   486,   462,    84,    89,   491,   105,   101,    73,   209,
     206,   111,   114,    89,   239,   370,     4,   111,   149,   149,
     454,   358,   239,   467,   467,   164,   352,   173,   297,   347,
     242,    89,   473,   159,   160,   161,   163,   321,   476,   477,
     478,   242,   242,   479,   483,   237,   237,   244,   237,   244,
     237,   244,   237,   244,   243,   471,   243,   469,   243,   467,
     244,   242,   237,     3,   237,   464,   240,     4,   242,   368,
     462,   462,   298,   237,   240,   237,   237,   237,     3,   241,
     237,   237,   237,   245,   242,     4,     8,     9,    65,    80,
      81,    82,    83,    85,    87,   122,   123,   124,   126,   129,
     130,   131,   133,   134,   135,   140,   167,   169,   170,   178,
     179,   180,   181,   182,   183,   202,   216,   393,   462,   484,
     487,   245,   242,   386,   378,   237,   237,   237,     4,   242,
     237,   237,     4,   237,   241,   237,   238,   237,     4,   237,
       4,   237,   455,   237,   237,   404,   237,    88,   237,   237,
     237,   238,   410,   237,   237,     4,   237,   237,   237,   237,
     153,   237,   237,   317,   297,   474,   341,   237,   237,   238,
     237,   240,     4,   466,   463,   474,   473,   325,   472,   326,
     327,   468,    16,   237,     3,   240,   246,   237,   376,   483,
     474,   474,   483,   237,   474,   483,   483,   239,   240,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   240,   237,   237,   454,
     470,     5,   237,   443,     4,   237,   202,     4,    89,    96,
     458,   469,   202,    98,   237,    89,   239,   371,   245,    98,
     461,   237,   473,   102,    74,   462,   237,   108,     4,   115,
     117,   118,   119,   237,    89,   239,   372,   108,     4,    89,
     375,   376,   317,   108,   467,   243,   455,    89,   473,   250,
     251,   475,   476,   334,   237,   244,   244,   237,   244,   237,
     455,   237,   474,   240,   237,   240,   237,   240,   455,   242,
     240,   240,   474,   461,   471,   121,   120,   461,   127,   128,
     132,   461,   461,   461,   473,   168,   461,   461,   461,   469,
      91,    92,   136,   137,   138,   143,   144,   150,   181,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   387,   457,   459,   473,
      85,   125,   126,   134,   146,   157,   183,   379,   237,   312,
       4,   237,   444,   237,     4,   238,   242,   237,   238,     4,
     381,   237,   237,     4,    89,   377,   237,   237,   237,    99,
     100,   109,   134,   157,   202,   203,   204,   205,   207,   208,
     209,   210,   211,   411,   237,   237,   237,   237,     4,     4,
     444,   237,   237,   237,   175,   177,   342,   343,   317,   472,
     468,    17,   240,     5,    48,   474,   240,   292,   238,   473,
     238,   238,   238,   238,   238,   238,   238,   238,   238,     3,
     237,    48,    49,    50,   445,   446,   112,   447,     4,   237,
     469,    97,   202,   469,   240,   237,   372,   461,   240,    99,
      75,   103,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   115,   115,   115,   115,   240,   237,   447,
       4,   377,     7,   486,   238,   244,   342,   237,   237,   240,
     237,   240,   474,    89,   461,   461,   461,   461,   461,   201,
     245,   388,   471,     4,   158,     4,   237,   238,   109,   449,
     237,     4,   237,   238,   382,   444,   237,   237,   101,   108,
     450,     4,     4,   103,   105,   158,   412,   469,   471,   415,
     415,   462,   442,     4,   237,     4,   449,   378,   238,   345,
       4,    18,     5,    48,   237,   310,   298,   240,   445,   111,
     448,   238,    28,   452,     4,   240,   202,   469,   240,   447,
     103,    99,   116,   240,   452,   344,   316,   237,   237,   240,
     235,   240,   388,   450,   238,    29,   453,   240,   238,   449,
     237,   237,   453,   316,    19,   455,   237,   107,   238,   202,
     451,   469,   452,    99,   455,   451,   237,   237,   462,     4,
     238,   425,   453,   365,     4,     5,    89,   313,   237,   311,
     469,   317,   426,   451,   317,   237,   312,   462,   240,   367,
       4,   317
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
#line 1762 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_QUEST_COMPANION));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1769 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = ((int64_t)1 << (token - 1));
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1778 "lev_comp.y"
    {
		      (yyval.i) = (int64_t) ~0;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1782 "lev_comp.y"
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

  case 274:

/* Line 1455 of yacc.c  */
#line 1795 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1803 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1813 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1821 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1830 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1837 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1845 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1851 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1856 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1861 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1866 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1871 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1881 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1886 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1891 "lev_comp.y"
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

  case 290:

/* Line 1455 of yacc.c  */
#line 1905 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1916 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1921 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1926 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1931 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1936 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1941 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1946 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1951 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1956 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1961 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 1966 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x00200000;
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1971 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x00400000;
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1976 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x00800000;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1981 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x01000000;
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 1986 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x02000000;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 1992 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x04000000;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 1997 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x08000000;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2002 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2007 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MATERIAL));
		      (yyval.i) = 0x20000000;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2014 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2020 "lev_comp.y"
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

  case 313:

/* Line 1455 of yacc.c  */
#line 2049 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2054 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2062 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2068 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2072 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2080 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2087 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2094 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (int64_t) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2104 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2114 "lev_comp.y"
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

  case 323:

/* Line 1455 of yacc.c  */
#line 2130 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (int64_t) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2140 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2144 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2150 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2156 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2162 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2172 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int) (yyvsp[(5) - (9)].i), (int) (yyvsp[(7) - (9)].i), (int) (yyvsp[(9) - (9)].i), SPO_MAGIC_PORTAL));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2178 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2184 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2191 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2199 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2205 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2210 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2215 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2220 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2225 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2231 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2237 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2242 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2247 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2257 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2262 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_L_V_PORTAL_TYPE));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2277 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2283 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2289 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2294 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2301 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2308 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2312 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2318 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2324 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2330 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2335 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2342 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2348 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (int64_t) LR_SPECIAL_MAP_SEEN : (int64_t) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2358 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2364 "lev_comp.y"
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

  case 367:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
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

  case 368:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2401 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (int64_t)(yyvsp[(7) - (7)].i), (int64_t)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2412 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (int64_t)(yyvsp[(7) - (11)].i), (int64_t)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2417 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (int64_t)(yyvsp[(9) - (9)].i), (int64_t)(yyvsp[(7) - (9)].i), (int64_t)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int64_t)(yyvsp[(9) - (13)].i), (int64_t)(yyvsp[(7) - (13)].i), (int64_t)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2442 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2446 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2450 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(0, 1, (int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_DECORATION));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2464 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2470 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2474 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2480 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2486 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2496 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2506 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2510 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2515 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2522 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2526 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2533 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2537 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2549 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2553 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2558 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2566 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2572 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int64_t)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2579 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2583 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2590 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2601 "lev_comp.y"
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

  case 409:

/* Line 1455 of yacc.c  */
#line 2614 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2618 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2624 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2628 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2635 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 0);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2639 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 1);
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2643 "lev_comp.y"
    {
		      (yyval.i) = ((int64_t)((yyvsp[(1) - (1)].i)) << 2);
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2649 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2653 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2659 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2665 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2669 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2675 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2681 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2686 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2692 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2696 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2702 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2730 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2740 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2749 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2754 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2761 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2772 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2778 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2782 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2786 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2793 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2803 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2810 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2814 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2820 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2824 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2832 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2836 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2843 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2853 "lev_comp.y"
    {
		      int64_t r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2866 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2870 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2877 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2887 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2896 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_LEVTYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2907 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2911 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2918 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2928 "lev_comp.y"
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

  case 467:

/* Line 1455 of yacc.c  */
#line 2939 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2948 "lev_comp.y"
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

  case 469:

/* Line 1455 of yacc.c  */
#line 2958 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2964 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2968 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2975 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2985 "lev_comp.y"
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

  case 474:

/* Line 1455 of yacc.c  */
#line 2996 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3005 "lev_comp.y"
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

  case 476:

/* Line 1455 of yacc.c  */
#line 3015 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3019 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3025 "lev_comp.y"
    { ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3027 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3033 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3037 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3041 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3045 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3053 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3062 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3066 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3070 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3074 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3078 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3081 "lev_comp.y"
    { ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3085 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3092 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3101 "lev_comp.y"
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

  case 498:

/* Line 1455 of yacc.c  */
#line 3142 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3146 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3153 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3160 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3170 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3174 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3182 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3186 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3190 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3194 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3198 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3203 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3207 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3211 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3216 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3221 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3226 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3230 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3235 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3240 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3244 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3248 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3253 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3257 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3264 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3270 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3274 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3280 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3296 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3300 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3304 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 3308 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 543:

/* Line 1455 of yacc.c  */
#line 3330 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 544:

/* Line 1455 of yacc.c  */
#line 3334 "lev_comp.y"
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

  case 545:

/* Line 1455 of yacc.c  */
#line 3360 "lev_comp.y"
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
#line 7660 "lev_comp.tab.c"
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
#line 3388 "lev_comp.y"


/*lev_comp.y*/

