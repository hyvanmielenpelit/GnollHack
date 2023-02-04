
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

/* GnollHack File Change Notice: This file has been changed from the original. Date of last change: 2022-08-28 */

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
extern struct opvar *FDECL(set_opvar_int, (struct opvar *, long));
extern void VDECL(add_opvars, (sp_lev *, const char *, ...));
extern void FDECL(start_level_def, (sp_lev * *, char *));

extern struct lc_funcdefs *FDECL(funcdef_new, (long,char *));
extern void FDECL(funcdef_free_all, (struct lc_funcdefs *));
extern struct lc_funcdefs *FDECL(funcdef_defined, (struct lc_funcdefs *,
                                                   char *, int));
extern char *FDECL(funcdef_paramtypes, (struct lc_funcdefs *));
extern char *FDECL(decode_parm_str, (char *));

extern struct lc_vardefs *FDECL(vardef_new, (long,char *));
extern void FDECL(vardef_free_all, (struct lc_vardefs *));
extern struct lc_vardefs *FDECL(vardef_defined, (struct lc_vardefs *,
                                                 char *, int));

extern void NDECL(break_stmt_start);
extern void FDECL(break_stmt_end, (sp_lev *));
extern void FDECL(break_stmt_new, (sp_lev *, long));

extern void FDECL(splev_add_from, (sp_lev *, sp_lev *));

extern void FDECL(check_vardef_type, (struct lc_vardefs *, char *, long));
extern void FDECL(vardef_used, (struct lc_vardefs *, char *));
extern struct lc_vardefs *FDECL(add_vardef_type, (struct lc_vardefs *,
                                                  char *, long));

extern int FDECL(reverse_jmp_opcode, (long));

struct coord {
    long x;
    long y;
};

struct forloopdef {
    char *varname;
    long jmp_point;
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
static long switch_case_value[MAX_SWITCH_CASES];
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
     MONSTER_GENERATION_ID = 284,
     object_ID = 285,
     monster_ID = 286,
     terrain_ID = 287,
     MAZEWALK_ID = 288,
     WALLIFY_ID = 289,
     REGION_ID = 290,
     SPECIAL_REGION_ID = 291,
     SPECIAL_LEVREGION_ID = 292,
     SPECIAL_REGION_TYPE = 293,
     NAMING_ID = 294,
     NAMING_TYPE = 295,
     FILLING = 296,
     IRREGULAR = 297,
     JOINED = 298,
     ALTAR_ID = 299,
     ANVIL_ID = 300,
     NPC_ID = 301,
     LADDER_ID = 302,
     STAIR_ID = 303,
     NON_DIGGABLE_ID = 304,
     NON_PASSWALL_ID = 305,
     ROOM_ID = 306,
     ARTIFACT_NAME_ID = 307,
     PORTAL_ID = 308,
     TELEPRT_ID = 309,
     BRANCH_ID = 310,
     LEV = 311,
     MINERALIZE_ID = 312,
     AGE_ID = 313,
     CORRIDOR_ID = 314,
     GOLD_ID = 315,
     ENGRAVING_ID = 316,
     FOUNTAIN_ID = 317,
     THRONE_ID = 318,
     MODRON_PORTAL_ID = 319,
     LEVEL_TELEPORTER_ID = 320,
     LEVEL_TELEPORT_DIRECTION_TYPE = 321,
     LEVEL_TELEPORT_END_TYPE = 322,
     POOL_ID = 323,
     SINK_ID = 324,
     NONE = 325,
     RAND_CORRIDOR_ID = 326,
     DOOR_STATE = 327,
     LIGHT_STATE = 328,
     CURSE_TYPE = 329,
     MYTHIC_TYPE = 330,
     ENGRAVING_TYPE = 331,
     KEYTYPE_ID = 332,
     LEVER_ID = 333,
     NO_PICKUP_ID = 334,
     DIRECTION = 335,
     RANDOM_TYPE = 336,
     RANDOM_TYPE_BRACKET = 337,
     A_REGISTER = 338,
     ALIGNMENT = 339,
     LEFT_OR_RIGHT = 340,
     CENTER = 341,
     TOP_OR_BOT = 342,
     ALTAR_TYPE = 343,
     ALTAR_SUBTYPE = 344,
     UP_OR_DOWN = 345,
     ACTIVE_OR_INACTIVE = 346,
     MODRON_PORTAL_TYPE = 347,
     NPC_TYPE = 348,
     FOUNTAIN_TYPE = 349,
     SPECIAL_OBJECT_TYPE = 350,
     CMAP_TYPE = 351,
     FLOOR_SUBTYPE = 352,
     FLOOR_SUBTYPE_ID = 353,
     FLOOR_ID = 354,
     FLOOR_TYPE = 355,
     FLOOR_TYPE_ID = 356,
     DECORATION_ID = 357,
     DECORATION_TYPE = 358,
     DECORATION_DIR = 359,
     DECORATION_ITEM_STATE = 360,
     ELEMENTAL_ENCHANTMENT_TYPE = 361,
     EXCEPTIONALITY_TYPE = 362,
     EXCEPTIONALITY_ID = 363,
     ELEMENTAL_ENCHANTMENT_ID = 364,
     ENCHANTMENT_ID = 365,
     SECRET_DOOR_ID = 366,
     USES_UP_KEY_ID = 367,
     MYTHIC_PREFIX_TYPE = 368,
     MYTHIC_SUFFIX_TYPE = 369,
     MYTHIC_PREFIX_ID = 370,
     MYTHIC_SUFFIX_ID = 371,
     CHARGES_ID = 372,
     SPECIAL_QUALITY_ID = 373,
     SPEFLAGS_ID = 374,
     SUBROOM_ID = 375,
     NAME_ID = 376,
     FLAGS_ID = 377,
     FLAG_TYPE = 378,
     MON_ATTITUDE = 379,
     MON_ALERTNESS = 380,
     SUBTYPE_ID = 381,
     NON_PASSDOOR_ID = 382,
     CARPET_ID = 383,
     CARPET_PIECE_ID = 384,
     CARPET_TYPE = 385,
     MON_APPEARANCE = 386,
     ROOMDOOR_ID = 387,
     IF_ID = 388,
     ELSE_ID = 389,
     TERRAIN_ID = 390,
     HORIZ_OR_VERT = 391,
     REPLACE_TERRAIN_ID = 392,
     LOCATION_SUBTYPE_ID = 393,
     DOOR_SUBTYPE = 394,
     BRAZIER_SUBTYPE = 395,
     SIGNPOST_SUBTYPE = 396,
     TREE_SUBTYPE = 397,
     FOREST_ID = 398,
     FOREST_TYPE = 399,
     INITIALIZE_TYPE = 400,
     EXIT_ID = 401,
     SHUFFLE_ID = 402,
     MANUAL_TYPE_ID = 403,
     MANUAL_TYPE = 404,
     QUANTITY_ID = 405,
     BURIED_ID = 406,
     LOOP_ID = 407,
     FOR_ID = 408,
     TO_ID = 409,
     SWITCH_ID = 410,
     CASE_ID = 411,
     BREAK_ID = 412,
     DEFAULT_ID = 413,
     ERODED_ID = 414,
     TRAPPED_STATE = 415,
     RECHARGED_ID = 416,
     INVIS_ID = 417,
     GREASED_ID = 418,
     INDESTRUCTIBLE_ID = 419,
     FEMALE_ID = 420,
     MALE_ID = 421,
     WAITFORU_ID = 422,
     PROTECTOR_ID = 423,
     CANCELLED_ID = 424,
     REVIVED_ID = 425,
     AVENGE_ID = 426,
     FLEEING_ID = 427,
     BLINDED_ID = 428,
     MAXHP_ID = 429,
     LEVEL_ADJUSTMENT_ID = 430,
     KEEP_ORIGINAL_INVENTORY_ID = 431,
     PARALYZED_ID = 432,
     STUNNED_ID = 433,
     CONFUSED_ID = 434,
     SEENTRAPS_ID = 435,
     ALL_ID = 436,
     MONTYPE_ID = 437,
     OBJTYPE_ID = 438,
     TERTYPE_ID = 439,
     TERTYPE2_ID = 440,
     LEVER_EFFECT_TYPE = 441,
     SWITCHABLE_ID = 442,
     CONTINUOUSLY_USABLE_ID = 443,
     TARGET_ID = 444,
     TRAPTYPE_ID = 445,
     EFFECT_FLAG_ID = 446,
     GRAVE_ID = 447,
     BRAZIER_ID = 448,
     SIGNPOST_ID = 449,
     TREE_ID = 450,
     ERODEPROOF_ID = 451,
     FUNCTION_ID = 452,
     MSG_OUTPUT_TYPE = 453,
     COMPARE_TYPE = 454,
     UNKNOWN_TYPE = 455,
     rect_ID = 456,
     fillrect_ID = 457,
     line_ID = 458,
     randline_ID = 459,
     grow_ID = 460,
     selection_ID = 461,
     flood_ID = 462,
     rndcoord_ID = 463,
     circle_ID = 464,
     ellipse_ID = 465,
     filter_ID = 466,
     complement_ID = 467,
     gradient_ID = 468,
     GRADIENT_TYPE = 469,
     LIMITED = 470,
     HUMIDITY_TYPE = 471,
     STRING = 472,
     MAP_ID = 473,
     NQSTRING = 474,
     VARSTRING = 475,
     CFUNC = 476,
     CFUNC_INT = 477,
     CFUNC_STR = 478,
     CFUNC_COORD = 479,
     CFUNC_REGION = 480,
     VARSTRING_INT = 481,
     VARSTRING_INT_ARRAY = 482,
     VARSTRING_STRING = 483,
     VARSTRING_STRING_ARRAY = 484,
     VARSTRING_VAR = 485,
     VARSTRING_VAR_ARRAY = 486,
     VARSTRING_COORD = 487,
     VARSTRING_COORD_ARRAY = 488,
     VARSTRING_REGION = 489,
     VARSTRING_REGION_ARRAY = 490,
     VARSTRING_MAPCHAR = 491,
     VARSTRING_MAPCHAR_ARRAY = 492,
     VARSTRING_MONST = 493,
     VARSTRING_MONST_ARRAY = 494,
     VARSTRING_OBJ = 495,
     VARSTRING_OBJ_ARRAY = 496,
     VARSTRING_SEL = 497,
     VARSTRING_SEL_ARRAY = 498,
     METHOD_INT = 499,
     METHOD_INT_ARRAY = 500,
     METHOD_STRING = 501,
     METHOD_STRING_ARRAY = 502,
     METHOD_VAR = 503,
     METHOD_VAR_ARRAY = 504,
     METHOD_COORD = 505,
     METHOD_COORD_ARRAY = 506,
     METHOD_REGION = 507,
     METHOD_REGION_ARRAY = 508,
     METHOD_MAPCHAR = 509,
     METHOD_MAPCHAR_ARRAY = 510,
     METHOD_MONST = 511,
     METHOD_MONST_ARRAY = 512,
     METHOD_OBJ = 513,
     METHOD_OBJ_ARRAY = 514,
     METHOD_SEL = 515,
     METHOD_SEL_ARRAY = 516,
     DICE = 517
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 153 "lev_comp.y"

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



/* Line 214 of yacc.c  */
#line 562 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 574 "lev_comp.tab.c"

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
#define YYLAST   1336

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  280
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  191
/* YYNRULES -- Number of rules.  */
#define YYNRULES  525
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1172

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   517

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   275,   279,     2,
     219,   220,   273,   271,   217,   272,   277,   274,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   218,     2,
       2,   276,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   221,     2,   222,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   223,   278,   224,     2,     2,     2,     2,
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
     215,   216,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      31,    37,    41,    58,    62,    68,    72,    78,    82,    83,
      86,    87,    90,    91,    94,    96,    98,    99,   103,   107,
     109,   110,   113,   117,   119,   121,   123,   125,   127,   129,
     131,   133,   135,   137,   139,   141,   143,   145,   147,   149,
     151,   153,   155,   157,   159,   161,   163,   165,   167,   169,
     171,   173,   175,   177,   179,   181,   183,   185,   187,   189,
     191,   193,   195,   197,   199,   201,   203,   205,   207,   209,
     211,   213,   215,   217,   219,   221,   223,   225,   227,   229,
     231,   233,   235,   237,   239,   241,   243,   245,   247,   249,
     251,   253,   255,   257,   259,   261,   263,   265,   267,   269,
     271,   273,   275,   277,   279,   281,   283,   285,   287,   289,
     291,   293,   297,   301,   307,   311,   317,   323,   329,   333,
     337,   343,   349,   355,   363,   371,   379,   385,   387,   391,
     393,   397,   399,   403,   405,   409,   411,   415,   417,   421,
     423,   427,   428,   429,   438,   443,   445,   446,   448,   450,
     456,   460,   461,   462,   472,   473,   476,   477,   483,   484,
     489,   491,   494,   496,   503,   504,   508,   509,   516,   517,
     522,   523,   528,   530,   531,   536,   540,   542,   546,   550,
     556,   562,   570,   575,   576,   590,   591,   607,   608,   611,
     617,   619,   625,   627,   633,   635,   641,   643,   654,   661,
     663,   665,   667,   669,   671,   675,   677,   679,   680,   684,
     688,   692,   696,   698,   700,   702,   704,   706,   714,   720,
     722,   724,   726,   728,   732,   733,   739,   744,   745,   749,
     751,   753,   755,   757,   760,   762,   764,   766,   768,   770,
     772,   776,   780,   784,   786,   788,   792,   794,   796,   798,
     802,   804,   806,   808,   812,   816,   817,   823,   826,   827,
     831,   833,   837,   839,   843,   847,   849,   851,   855,   857,
     859,   861,   865,   867,   869,   871,   875,   879,   883,   887,
     891,   895,   899,   903,   905,   907,   909,   911,   915,   919,
     923,   929,   937,   943,   952,   954,   958,   962,   968,   974,
     982,   990,   997,  1003,  1004,  1007,  1013,  1017,  1029,  1043,
    1050,  1051,  1055,  1057,  1061,  1065,  1069,  1073,  1075,  1077,
    1081,  1085,  1089,  1093,  1097,  1101,  1103,  1105,  1107,  1109,
    1113,  1117,  1119,  1125,  1135,  1141,  1149,  1153,  1157,  1163,
    1173,  1179,  1185,  1191,  1192,  1206,  1207,  1209,  1217,  1229,
    1239,  1253,  1257,  1265,  1277,  1291,  1299,  1307,  1313,  1321,
    1327,  1337,  1345,  1351,  1361,  1367,  1373,  1377,  1385,  1389,
    1397,  1405,  1409,  1415,  1421,  1425,  1431,  1439,  1449,  1451,
    1453,  1455,  1457,  1459,  1460,  1463,  1465,  1469,  1471,  1473,
    1475,  1476,  1480,  1482,  1483,  1487,  1489,  1490,  1494,  1496,
    1498,  1500,  1502,  1504,  1506,  1508,  1510,  1512,  1516,  1518,
    1520,  1525,  1527,  1529,  1534,  1536,  1538,  1543,  1545,  1550,
    1556,  1558,  1562,  1564,  1568,  1570,  1572,  1577,  1587,  1589,
    1591,  1596,  1598,  1604,  1606,  1608,  1613,  1615,  1617,  1623,
    1625,  1627,  1629,  1634,  1636,  1638,  1644,  1646,  1648,  1650,
    1654,  1656,  1658,  1662,  1664,  1669,  1673,  1677,  1681,  1685,
    1689,  1693,  1695,  1697,  1701,  1703,  1707,  1708,  1710,  1712,
    1714,  1716,  1720,  1721,  1723,  1725,  1728,  1731,  1736,  1743,
    1748,  1755,  1762,  1769,  1776,  1779,  1786,  1795,  1804,  1815,
    1830,  1833,  1835,  1839,  1841,  1845,  1847,  1849,  1851,  1853,
    1855,  1857,  1859,  1861,  1863,  1865,  1867,  1869,  1871,  1873,
    1875,  1877,  1879,  1881,  1883,  1894
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     281,     0,    -1,    -1,   282,    -1,   283,    -1,   283,   282,
      -1,   284,   294,   296,    -1,    16,   218,   225,    -1,    15,
     218,   225,   217,     3,    -1,    17,   218,    11,   217,   394,
      -1,    17,   218,    10,   217,     3,    -1,    17,   218,    13,
      -1,    17,   218,    12,   217,     3,   217,     3,   217,     5,
     217,     5,   217,   432,   217,   293,   292,    -1,    18,   218,
     462,    -1,   143,   218,   144,   217,   145,    -1,   143,   218,
     144,    -1,    29,   218,     4,   217,   306,    -1,    21,   218,
     100,    -1,    -1,   217,   215,    -1,    -1,   217,   439,    -1,
      -1,   217,     3,    -1,     5,    -1,    81,    -1,    -1,   122,
     218,   295,    -1,   123,   217,   295,    -1,   123,    -1,    -1,
     298,   296,    -1,   223,   296,   224,    -1,   339,    -1,   285,
      -1,   286,    -1,   402,    -1,   289,    -1,   287,    -1,   410,
      -1,   411,    -1,   412,    -1,   288,    -1,   409,    -1,   408,
      -1,   406,    -1,   407,    -1,   413,    -1,   414,    -1,   415,
      -1,   416,    -1,   417,    -1,   383,    -1,   341,    -1,   304,
      -1,   303,    -1,   397,    -1,   353,    -1,   375,    -1,   419,
      -1,   420,    -1,   385,    -1,   386,    -1,   387,    -1,   418,
      -1,   319,    -1,   329,    -1,   331,    -1,   335,    -1,   333,
      -1,   316,    -1,   326,    -1,   312,    -1,   315,    -1,   378,
      -1,   388,    -1,   360,    -1,   376,    -1,   363,    -1,   369,
      -1,   398,    -1,   393,    -1,   381,    -1,   340,    -1,   399,
      -1,   400,    -1,   401,    -1,   403,    -1,   346,    -1,   344,
      -1,   392,    -1,   396,    -1,   395,    -1,   379,    -1,   380,
      -1,   382,    -1,   374,    -1,   377,    -1,   235,    -1,   237,
      -1,   239,    -1,   241,    -1,   243,    -1,   245,    -1,   247,
      -1,   249,    -1,   251,    -1,   234,    -1,   236,    -1,   238,
      -1,   240,    -1,   242,    -1,   244,    -1,   246,    -1,   248,
      -1,   250,    -1,   299,    -1,   300,    -1,   228,    -1,   228,
      -1,   300,    -1,   147,   218,   299,    -1,   301,   276,   451,
      -1,   301,   276,   206,   218,   460,    -1,   301,   276,   450,
      -1,   301,   276,   467,   218,   444,    -1,   301,   276,   466,
     218,   446,    -1,   301,   276,   465,   218,   448,    -1,   301,
     276,   439,    -1,   301,   276,   442,    -1,   301,   276,   223,
     310,   224,    -1,   301,   276,   223,   309,   224,    -1,   301,
     276,   223,   308,   224,    -1,   301,   276,   467,   218,   223,
     307,   224,    -1,   301,   276,   466,   218,   223,   306,   224,
      -1,   301,   276,   465,   218,   223,   305,   224,    -1,   301,
     276,   223,   311,   224,    -1,   449,    -1,   305,   217,   449,
      -1,   447,    -1,   306,   217,   447,    -1,   445,    -1,   307,
     217,   445,    -1,   443,    -1,   308,   217,   443,    -1,   440,
      -1,   309,   217,   440,    -1,   451,    -1,   310,   217,   451,
      -1,   450,    -1,   311,   217,   450,    -1,    -1,    -1,   197,
     227,   219,   313,   455,   220,   314,   297,    -1,   227,   219,
     458,   220,    -1,   146,    -1,    -1,     6,    -1,     6,    -1,
     221,   451,   199,   451,   222,    -1,   221,   451,   222,    -1,
      -1,    -1,   155,   320,   221,   438,   222,   321,   223,   322,
     224,    -1,    -1,   323,   322,    -1,    -1,   156,   463,   218,
     324,   296,    -1,    -1,   158,   218,   325,   296,    -1,   157,
      -1,   277,   277,    -1,   154,    -1,   153,   302,   276,   451,
     327,   451,    -1,    -1,   328,   330,   297,    -1,    -1,   152,
     221,   438,   222,   332,   297,    -1,    -1,   318,   218,   334,
     298,    -1,    -1,   133,   318,   336,   337,    -1,   297,    -1,
      -1,   297,   338,   134,   297,    -1,    14,   218,   450,    -1,
      71,    -1,    71,   218,   463,    -1,    71,   218,    81,    -1,
      59,   218,   342,   217,   342,    -1,    59,   218,   342,   217,
     463,    -1,   219,     4,   217,    80,   217,   357,   220,    -1,
     422,   317,   217,   432,    -1,    -1,   120,   218,   343,   217,
     350,   217,   352,   423,   426,   428,   430,   345,   297,    -1,
      -1,    51,   218,   343,   217,   349,   217,   351,   217,   352,
     423,   426,   428,   430,   347,   297,    -1,    -1,   217,     5,
      -1,   219,     4,   217,     4,   220,    -1,    81,    -1,   219,
       4,   217,     4,   220,    -1,    81,    -1,   219,   361,   217,
     362,   220,    -1,    81,    -1,   219,     4,   217,     4,   220,
      -1,    81,    -1,   132,   218,   354,   217,   431,   217,   355,
     217,   357,   358,    -1,    27,   218,   431,   217,   460,   358,
      -1,     5,    -1,    81,    -1,   356,    -1,    81,    -1,    80,
      -1,    80,   278,   356,    -1,     4,    -1,    81,    -1,    -1,
     358,   217,   359,    -1,   138,   218,   139,    -1,    77,   218,
     448,    -1,   118,   218,     4,    -1,   164,    -1,   111,    -1,
     112,    -1,   127,    -1,    20,    -1,    19,   218,   361,   217,
     362,   348,   226,    -1,    19,   218,   439,   348,   226,    -1,
      85,    -1,    86,    -1,    87,    -1,    86,    -1,    25,   218,
     365,    -1,    -1,    25,   218,   365,   364,   297,    -1,   446,
     217,   439,   366,    -1,    -1,   366,   217,   367,    -1,   450,
      -1,   124,    -1,   125,    -1,   434,    -1,   131,   450,    -1,
     165,    -1,   166,    -1,   162,    -1,   169,    -1,   170,    -1,
     171,    -1,   172,   218,   438,    -1,   173,   218,   438,    -1,
     177,   218,   438,    -1,   178,    -1,   179,    -1,   180,   218,
     368,    -1,   167,    -1,   168,    -1,   174,    -1,   175,   218,
     438,    -1,   176,    -1,   225,    -1,   181,    -1,   225,   278,
     368,    -1,    23,   218,   371,    -1,    -1,    24,   218,   371,
     370,   297,    -1,   448,   372,    -1,    -1,   372,   217,   373,
      -1,    74,    -1,   182,   218,   446,    -1,   464,    -1,   121,
     218,   450,    -1,   150,   218,   438,    -1,   151,    -1,    73,
      -1,   159,   218,   438,    -1,   196,    -1,    72,    -1,   160,
      -1,   161,   218,   438,    -1,   162,    -1,   163,    -1,   439,
      -1,   109,   218,   106,    -1,   108,   218,   107,    -1,   110,
     218,   438,    -1,   117,   218,   438,    -1,   118,   218,   438,
      -1,   148,   218,   149,    -1,   119,   218,   438,    -1,    77,
     218,   448,    -1,   164,    -1,   112,    -1,    79,    -1,    75,
      -1,   115,   218,   113,    -1,   116,   218,   114,    -1,    58,
     218,   438,    -1,    26,   218,   421,   217,   439,    -1,    28,
     218,   439,   217,    80,   217,   431,    -1,    33,   218,   439,
     217,    80,    -1,    33,   218,   439,   217,    80,   217,     5,
     292,    -1,    34,    -1,    34,   218,   460,    -1,    34,   218,
     181,    -1,    47,   218,   439,   217,    90,    -1,    48,   218,
     439,   217,    90,    -1,    48,   218,   469,   217,   469,   217,
      90,    -1,    53,   218,   469,   217,   469,   217,   225,    -1,
      54,   218,   469,   217,   469,   384,    -1,    55,   218,   469,
     217,   469,    -1,    -1,   217,    90,    -1,    62,   218,   460,
     217,    94,    -1,    63,   218,   460,    -1,    64,   218,   439,
     217,   189,   218,   439,   217,    92,   217,    91,    -1,    64,
     218,   439,   217,    65,   217,    66,   217,    67,   217,    92,
     217,    91,    -1,    78,   218,   439,   217,   186,   389,    -1,
      -1,   389,   217,   390,    -1,    91,    -1,   182,   218,   446,
      -1,   183,   218,   448,    -1,   184,   218,   394,    -1,   185,
     218,   394,    -1,   187,    -1,   188,    -1,   189,   218,   439,
      -1,   190,   218,   421,    -1,   138,   218,   391,    -1,    98,
     218,   429,    -1,   191,   218,     4,    -1,   118,   218,     4,
      -1,     4,    -1,    92,    -1,   139,    -1,    94,    -1,    69,
     218,   460,    -1,    68,   218,   460,    -1,     3,    -1,   219,
       3,   217,   432,   220,    -1,   137,   218,   442,   217,   444,
     217,   444,   217,     7,    -1,   135,   218,   460,   217,   444,
      -1,   135,   218,   460,   217,   444,   217,    97,    -1,    49,
     218,   442,    -1,    50,   218,   442,    -1,    39,   218,   225,
     217,    40,    -1,    39,   218,   225,   217,    40,   217,   182,
     218,   446,    -1,    36,   218,   442,   217,    38,    -1,    37,
     218,   469,   217,    38,    -1,    22,   218,   460,   217,    96,
      -1,    -1,    35,   218,   442,   217,   432,   217,   422,   423,
     426,   428,   430,   404,   405,    -1,    -1,   297,    -1,    44,
     218,   439,   217,   433,   217,   435,    -1,    44,   218,   439,
     217,   433,   217,   435,   217,   182,   218,   446,    -1,    44,
     218,   439,   217,   433,   217,   435,   217,    89,    -1,    44,
     218,   439,   217,   433,   217,   435,   217,    89,   217,   182,
     218,   446,    -1,    45,   218,   439,    -1,    45,   218,   439,
     217,   182,   218,   446,    -1,   102,   218,   439,   217,   103,
     217,     4,   217,   104,   217,   105,    -1,   102,   218,   439,
     217,   103,   217,     4,   217,   104,   217,   105,   217,   432,
      -1,   102,   218,   439,   217,   103,   217,   104,    -1,    99,
     218,   460,   217,   100,   217,    97,    -1,   126,   218,   460,
     217,     4,    -1,   126,   218,   460,   217,   100,   217,    97,
      -1,   128,   218,   442,   217,   130,    -1,   129,   218,   460,
     217,   130,   217,     4,   217,     4,    -1,   129,   218,   460,
     217,   130,   217,     4,    -1,    46,   218,    93,   217,   439,
      -1,    46,   218,    93,   217,   439,   217,   182,   218,   446,
      -1,   192,   218,   439,   217,   450,    -1,   192,   218,   439,
     217,    81,    -1,   192,   218,   439,    -1,   193,   218,   439,
     217,   140,   217,   432,    -1,   193,   218,   439,    -1,   194,
     218,   439,   217,   141,   217,   450,    -1,   194,   218,   439,
     217,   141,   217,    81,    -1,   194,   218,   439,    -1,   195,
     218,   439,   217,   142,    -1,   195,   218,   439,   217,   144,
      -1,   195,   218,   439,    -1,    60,   218,   451,   217,   439,
      -1,    61,   218,   439,   217,   468,   217,   450,    -1,    57,
     218,   438,   217,   438,   217,   438,   217,   438,    -1,    57,
      -1,   225,    -1,    81,    -1,   225,    -1,    81,    -1,    -1,
     217,   424,    -1,   425,    -1,   425,   217,   424,    -1,    41,
      -1,    42,    -1,    43,    -1,    -1,   101,   218,   427,    -1,
     100,    -1,    -1,    98,   218,   429,    -1,    97,    -1,    -1,
     182,   218,   446,    -1,    72,    -1,    81,    -1,    73,    -1,
      81,    -1,    84,    -1,   436,    -1,    81,    -1,    84,    -1,
     436,    -1,    83,   218,    81,    -1,    88,    -1,    81,    -1,
      83,   221,     4,   222,    -1,   225,    -1,   236,    -1,   237,
     221,   451,   222,    -1,   451,    -1,   440,    -1,   208,   219,
     460,   220,    -1,   240,    -1,   241,   221,   451,   222,    -1,
     219,     4,   217,     4,   220,    -1,    81,    -1,    82,   441,
     222,    -1,   216,    -1,   216,   217,   441,    -1,   443,    -1,
     242,    -1,   243,   221,   451,   222,    -1,   219,     4,   217,
       4,   217,     4,   217,     4,   220,    -1,   445,    -1,   244,
      -1,   245,   221,   451,   222,    -1,     3,    -1,   219,     3,
     217,   432,   220,    -1,   447,    -1,   246,    -1,   247,   221,
     451,   222,    -1,   225,    -1,     3,    -1,   219,     3,   217,
     225,   220,    -1,    81,    -1,   449,    -1,   248,    -1,   249,
     221,   451,   222,    -1,   225,    -1,     3,    -1,   219,     3,
     217,   225,   220,    -1,    81,    -1,    95,    -1,   437,    -1,
     450,   277,   437,    -1,     4,    -1,   461,    -1,   219,     8,
     220,    -1,   234,    -1,   235,   221,   451,   222,    -1,   451,
     271,   451,    -1,   451,   272,   451,    -1,   451,   273,   451,
      -1,   451,   274,   451,    -1,   451,   275,   451,    -1,   219,
     451,   220,    -1,   230,    -1,   231,    -1,   301,   218,   452,
      -1,   453,    -1,   454,   217,   453,    -1,    -1,   454,    -1,
     451,    -1,   450,    -1,   456,    -1,   457,   217,   456,    -1,
      -1,   457,    -1,   439,    -1,   201,   442,    -1,   202,   442,
      -1,   203,   439,   217,   439,    -1,   204,   439,   217,   439,
     217,   451,    -1,   205,   219,   460,   220,    -1,   205,   219,
     356,   217,   460,   220,    -1,   211,   219,     7,   217,   460,
     220,    -1,   211,   219,   460,   217,   460,   220,    -1,   211,
     219,   444,   217,   460,   220,    -1,   207,   439,    -1,   209,
     219,   439,   217,   451,   220,    -1,   209,   219,   439,   217,
     451,   217,    41,   220,    -1,   210,   219,   439,   217,   451,
     217,   451,   220,    -1,   210,   219,   439,   217,   451,   217,
     451,   217,    41,   220,    -1,   213,   219,   214,   217,   219,
     451,   272,   451,   290,   220,   217,   439,   291,   220,    -1,
     212,   459,    -1,   250,    -1,   219,   460,   220,    -1,   459,
      -1,   459,   279,   460,    -1,   270,    -1,    96,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   461,    -1,    30,    -1,    23,    -1,    31,
      -1,    25,    -1,    32,    -1,   135,    -1,    76,    -1,    81,
      -1,   470,    -1,    56,   219,     4,   217,     4,   217,     4,
     217,     4,   220,    -1,   219,     4,   217,     4,   217,     4,
     217,     4,   220,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   287,   287,   288,   291,   292,   295,   318,   323,   345,
     358,   370,   376,   405,   411,   415,   421,   427,   434,   437,
     444,   448,   455,   458,   465,   466,   470,   473,   480,   484,
     491,   494,   500,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   584,   585,   586,
     587,   588,   589,   590,   591,   592,   595,   596,   597,   600,
     601,   604,   620,   626,   632,   638,   644,   650,   656,   662,
     668,   678,   688,   698,   708,   718,   728,   740,   745,   752,
     757,   764,   769,   776,   780,   786,   791,   798,   802,   808,
     812,   819,   841,   818,   855,   910,   917,   920,   926,   933,
     937,   946,   950,   945,  1013,  1014,  1018,  1017,  1031,  1030,
    1045,  1055,  1056,  1059,  1097,  1096,  1131,  1130,  1161,  1160,
    1193,  1192,  1218,  1229,  1228,  1256,  1262,  1267,  1272,  1279,
    1286,  1295,  1303,  1315,  1314,  1338,  1337,  1361,  1364,  1370,
    1380,  1386,  1395,  1401,  1406,  1412,  1417,  1423,  1434,  1440,
    1441,  1444,  1445,  1448,  1452,  1458,  1459,  1463,  1469,  1477,
    1482,  1487,  1492,  1497,  1502,  1507,  1515,  1522,  1530,  1538,
    1539,  1542,  1543,  1546,  1551,  1550,  1564,  1571,  1578,  1586,
    1591,  1597,  1603,  1609,  1615,  1620,  1625,  1630,  1635,  1640,
    1645,  1650,  1655,  1660,  1665,  1670,  1676,  1681,  1686,  1691,
    1696,  1703,  1712,  1716,  1729,  1738,  1737,  1755,  1765,  1771,
    1779,  1785,  1790,  1795,  1800,  1805,  1810,  1815,  1820,  1825,
    1839,  1845,  1850,  1855,  1860,  1865,  1870,  1875,  1880,  1885,
    1890,  1895,  1900,  1905,  1910,  1915,  1920,  1926,  1931,  1936,
    1943,  1949,  1978,  1983,  1991,  1997,  2001,  2009,  2016,  2023,
    2033,  2043,  2059,  2070,  2073,  2079,  2085,  2091,  2095,  2101,
    2108,  2114,  2122,  2128,  2133,  2138,  2143,  2148,  2154,  2160,
    2165,  2170,  2175,  2180,  2185,  2192,  2192,  2192,  2192,  2195,
    2201,  2207,  2212,  2219,  2226,  2230,  2236,  2242,  2248,  2253,
    2260,  2266,  2276,  2283,  2282,  2314,  2317,  2323,  2328,  2333,
    2338,  2344,  2348,  2354,  2358,  2362,  2368,  2374,  2378,  2384,
    2390,  2394,  2400,  2404,  2410,  2414,  2419,  2426,  2430,  2437,
    2441,  2446,  2453,  2457,  2462,  2470,  2476,  2483,  2487,  2494,
    2502,  2505,  2515,  2519,  2522,  2528,  2532,  2539,  2543,  2547,
    2554,  2557,  2563,  2570,  2573,  2579,  2586,  2590,  2597,  2598,
    2601,  2602,  2605,  2606,  2607,  2613,  2614,  2615,  2621,  2622,
    2625,  2634,  2639,  2646,  2657,  2663,  2667,  2671,  2678,  2688,
    2695,  2699,  2705,  2709,  2717,  2721,  2728,  2738,  2751,  2755,
    2762,  2772,  2781,  2792,  2796,  2803,  2813,  2824,  2833,  2843,
    2849,  2853,  2860,  2870,  2881,  2890,  2900,  2904,  2911,  2912,
    2918,  2922,  2926,  2930,  2938,  2947,  2951,  2955,  2959,  2963,
    2967,  2970,  2977,  2986,  3019,  3020,  3023,  3024,  3027,  3031,
    3038,  3045,  3056,  3059,  3067,  3071,  3075,  3079,  3083,  3088,
    3092,  3096,  3101,  3106,  3111,  3115,  3120,  3125,  3129,  3133,
    3138,  3142,  3149,  3155,  3159,  3165,  3172,  3173,  3176,  3177,
    3178,  3181,  3185,  3189,  3193,  3199,  3200,  3203,  3204,  3207,
    3208,  3211,  3212,  3215,  3219,  3245
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHAR", "INTEGER", "BOOLEAN", "PERCENT",
  "SPERCENT", "MINUS_INTEGER", "PLUS_INTEGER", "MAZE_GRID_ID",
  "SOLID_FILL_ID", "MINES_ID", "ROGUELEV_ID", "MESSAGE_ID", "MAZE_ID",
  "LEVEL_ID", "LEV_INIT_ID", "TILESET_ID", "GEOMETRY_ID", "NOMAP_ID",
  "BOUNDARY_TYPE_ID", "SPECIAL_TILESET_ID", "OBJECT_ID", "COBJECT_ID",
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
  "DECORATION_DIR", "DECORATION_ITEM_STATE", "ELEMENTAL_ENCHANTMENT_TYPE",
  "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID", "ELEMENTAL_ENCHANTMENT_ID",
  "ENCHANTMENT_ID", "SECRET_DOOR_ID", "USES_UP_KEY_ID",
  "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE", "MYTHIC_PREFIX_ID",
  "MYTHIC_SUFFIX_ID", "CHARGES_ID", "SPECIAL_QUALITY_ID", "SPEFLAGS_ID",
  "SUBROOM_ID", "NAME_ID", "FLAGS_ID", "FLAG_TYPE", "MON_ATTITUDE",
  "MON_ALERTNESS", "SUBTYPE_ID", "NON_PASSDOOR_ID", "CARPET_ID",
  "CARPET_PIECE_ID", "CARPET_TYPE", "MON_APPEARANCE", "ROOMDOOR_ID",
  "IF_ID", "ELSE_ID", "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
  "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE", "BRAZIER_SUBTYPE",
  "SIGNPOST_SUBTYPE", "TREE_SUBTYPE", "FOREST_ID", "FOREST_TYPE",
  "INITIALIZE_TYPE", "EXIT_ID", "SHUFFLE_ID", "MANUAL_TYPE_ID",
  "MANUAL_TYPE", "QUANTITY_ID", "BURIED_ID", "LOOP_ID", "FOR_ID", "TO_ID",
  "SWITCH_ID", "CASE_ID", "BREAK_ID", "DEFAULT_ID", "ERODED_ID",
  "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID", "GREASED_ID",
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
  "optmontype", "door_state", "light_state", "alignment", "alignment_prfx",
  "altar_type", "a_register", "string_or_var", "integer_or_var",
  "coord_or_var", "encodecoord", "humidity_flags", "region_or_var",
  "encoderegion", "mapchar_or_var", "mapchar", "monster_or_var",
  "encodemonster", "object_or_var", "encodeobj", "string_expr",
  "math_expr_var", "func_param_type", "func_param_part", "func_param_list",
  "func_params_list", "func_call_param_part", "func_call_param_list",
  "func_call_params_list", "ter_selection_x", "ter_selection", "dice",
  "tileset_number", "all_integers", "all_ints_push", "objectid",
  "monsterid", "terrainid", "engraving_type", "lev_region", "region", 0
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
     465,   466,   467,   468,   469,   470,   471,    44,    58,    40,
      41,    91,    93,   123,   125,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,    43,    45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   280,   281,   281,   282,   282,   283,   284,   284,   285,
     285,   285,   285,   286,   287,   287,   288,   289,   290,   290,
     291,   291,   292,   292,   293,   293,   294,   294,   295,   295,
     296,   296,   297,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   301,   301,   301,   302,
     302,   303,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   305,   305,   306,
     306,   307,   307,   308,   308,   309,   309,   310,   310,   311,
     311,   313,   314,   312,   315,   316,   317,   317,   318,   318,
     318,   320,   321,   319,   322,   322,   324,   323,   325,   323,
     326,   327,   327,   328,   330,   329,   332,   331,   334,   333,
     336,   335,   337,   338,   337,   339,   340,   340,   340,   341,
     341,   342,   343,   345,   344,   347,   346,   348,   348,   349,
     349,   350,   350,   351,   351,   352,   352,   353,   353,   354,
     354,   355,   355,   356,   356,   357,   357,   358,   358,   359,
     359,   359,   359,   359,   359,   359,   360,   360,   360,   361,
     361,   362,   362,   363,   364,   363,   365,   366,   366,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   367,   367,   367,   367,   367,   367,   367,   367,   367,
     367,   368,   368,   368,   369,   370,   369,   371,   372,   372,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   373,   373,   373,   373,   373,
     374,   375,   376,   376,   377,   377,   377,   378,   379,   380,
     381,   382,   383,   384,   384,   385,   386,   387,   387,   388,
     389,   389,   390,   390,   390,   390,   390,   390,   390,   390,
     390,   390,   390,   390,   390,   391,   391,   391,   391,   392,
     393,   394,   394,   395,   396,   396,   397,   398,   399,   399,
     400,   401,   402,   404,   403,   405,   405,   406,   406,   406,
     406,   407,   407,   408,   408,   408,   409,   410,   410,   411,
     412,   412,   413,   413,   414,   414,   414,   415,   415,   416,
     416,   416,   417,   417,   417,   418,   419,   420,   420,   421,
     421,   422,   422,   423,   423,   424,   424,   425,   425,   425,
     426,   426,   427,   428,   428,   429,   430,   430,   431,   431,
     432,   432,   433,   433,   433,   434,   434,   434,   435,   435,
     436,   437,   437,   437,   438,   439,   439,   439,   439,   440,
     440,   440,   441,   441,   442,   442,   442,   443,   444,   444,
     444,   445,   445,   446,   446,   446,   447,   447,   447,   447,
     448,   448,   448,   449,   449,   449,   449,   449,   450,   450,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   452,   452,   453,   454,   454,   455,   455,   456,   456,
     457,   457,   458,   458,   459,   459,   459,   459,   459,   459,
     459,   459,   459,   459,   459,   459,   459,   459,   459,   459,
     459,   459,   459,   460,   460,   461,   462,   462,   463,   463,
     463,   464,   464,   464,   464,   465,   465,   466,   466,   467,
     467,   468,   468,   469,   469,   470
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     5,
       5,     3,    16,     3,     5,     3,     5,     3,     0,     2,
       0,     2,     0,     2,     1,     1,     0,     3,     3,     1,
       0,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     5,     3,     5,     5,     5,     3,     3,
       5,     5,     5,     7,     7,     7,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     0,     0,     8,     4,     1,     0,     1,     1,     5,
       3,     0,     0,     9,     0,     2,     0,     5,     0,     4,
       1,     2,     1,     6,     0,     3,     0,     6,     0,     4,
       0,     4,     1,     0,     4,     3,     1,     3,     3,     5,
       5,     7,     4,     0,    13,     0,    15,     0,     2,     5,
       1,     5,     1,     5,     1,     5,     1,    10,     6,     1,
       1,     1,     1,     1,     3,     1,     1,     0,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     7,     5,     1,
       1,     1,     1,     3,     0,     5,     4,     0,     3,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     3,     3,     0,     5,     2,     0,     3,
       1,     3,     1,     3,     3,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     3,     3,
       5,     7,     5,     8,     1,     3,     3,     5,     5,     7,
       7,     6,     5,     0,     2,     5,     3,    11,    13,     6,
       0,     3,     1,     3,     3,     3,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     3,
       3,     1,     5,     9,     5,     7,     3,     3,     5,     9,
       5,     5,     5,     0,    13,     0,     1,     7,    11,     9,
      13,     3,     7,    11,    13,     7,     7,     5,     7,     5,
       9,     7,     5,     9,     5,     5,     3,     7,     3,     7,
       7,     3,     5,     5,     3,     5,     7,     9,     1,     1,
       1,     1,     1,     0,     2,     1,     3,     1,     1,     1,
       0,     3,     1,     0,     3,     1,     0,     3,     1,     1,
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
       2,     0,     0,     0,     3,     4,    26,     0,     0,     1,
       5,     0,    30,     0,     7,     0,   158,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   388,     0,
       0,     0,     0,     0,     0,     0,     0,   186,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     155,     0,     0,     0,   161,   170,     0,     0,     0,     0,
       0,     0,     0,   118,   107,    98,   108,    99,   109,   100,
     110,   101,   111,   102,   112,   103,   113,   104,   114,   105,
     115,   106,    34,    35,    38,    42,    37,     6,    30,   116,
     117,     0,    55,    54,    72,    73,    70,     0,    65,    71,
     174,    66,    67,    69,    68,    33,    83,    53,    89,    88,
      57,    76,    78,    79,    96,    58,    77,    97,    74,    93,
      94,    82,    95,    52,    61,    62,    63,    75,    90,    81,
      92,    91,    56,    80,    84,    85,    86,    36,    87,    45,
      46,    44,    43,    39,    40,    41,    47,    48,    49,    50,
      51,    64,    59,    60,     0,    29,    27,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   180,     0,     0,     0,     0,
       0,   119,   120,     0,     0,     0,     0,     0,     0,     0,
     460,     0,   463,     0,   505,     0,   461,   482,    31,     0,
     178,     0,     8,     0,   421,   422,     0,   458,   185,     0,
       0,     0,    11,   507,   506,    13,   430,     0,   229,   230,
       0,     0,   427,     0,     0,   197,   425,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     501,   484,   503,     0,   454,   456,   457,     0,   453,   451,
       0,   264,   268,   450,   265,   447,   449,     0,   446,   444,
       0,   233,     0,   443,   390,   389,     0,   408,   409,     0,
       0,     0,     0,   306,   305,     0,   435,     0,     0,   434,
       0,     0,     0,     0,   523,     0,     0,   361,     0,     0,
       0,     0,     0,   346,   347,   392,   391,     0,   156,     0,
       0,     0,     0,   424,     0,     0,     0,     0,     0,   316,
       0,   340,   339,   510,   508,   509,   188,   187,     0,     0,
       0,     0,     0,     0,     0,   209,   210,     0,     0,     0,
       0,    15,   121,     0,     0,     0,   376,   378,   381,   384,
     151,     0,     0,     0,     0,   160,     0,     0,     0,     0,
       0,   479,   478,   480,   483,     0,   516,   518,   515,   517,
     519,   520,     0,     0,     0,   128,   129,   124,   122,     0,
       0,     0,     0,    30,   175,    28,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,   485,
     486,     0,     0,     0,   494,     0,     0,     0,   500,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   157,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   182,   181,     0,     0,     0,   176,     0,     0,
       0,     0,     0,     0,   476,   462,   470,     0,     0,   465,
     466,   467,   468,   469,     0,   154,     0,   460,     0,     0,
       0,     0,   145,   143,   149,   147,     0,     0,     0,   179,
       0,     0,   459,    10,   341,     0,     9,     0,     0,   431,
       0,     0,     0,   232,   231,   197,   198,   228,     0,     0,
     213,     0,     0,     0,     0,   441,     0,     0,   439,     0,
       0,   438,     0,     0,   502,   504,   352,     0,     0,     0,
     266,     0,     0,   235,   237,   300,   217,     0,    16,   139,
     302,     0,     0,   410,   411,     0,   350,     0,     0,   351,
     348,   414,     0,   412,     0,   413,     0,   372,   307,     0,
     308,     0,   200,     0,     0,     0,     0,   313,   312,     0,
       0,   189,   190,   385,   521,   522,     0,   315,     0,     0,
     320,     0,     0,   202,     0,     0,   367,     0,   369,     0,
       0,     0,     0,   344,     0,    14,     0,   172,     0,     0,
     162,   375,   374,     0,     0,   382,   383,     0,   474,   477,
       0,   464,   159,   481,   123,     0,     0,   132,     0,   131,
       0,   130,     0,   136,     0,   127,     0,   126,     0,   125,
      32,   423,     0,     0,   433,   426,     0,   428,     0,   487,
       0,     0,     0,   489,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   452,   513,   511,   512,     0,   279,   276,
     270,   296,     0,   295,     0,     0,     0,   294,     0,     0,
       0,     0,     0,     0,     0,     0,   275,     0,   280,     0,
     282,   283,   293,     0,   278,   269,   284,   514,   272,     0,
     445,   236,   208,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     192,     0,     0,   311,     0,     0,     0,     0,     0,   319,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     177,   171,   173,     0,     0,     0,     0,     0,   152,     0,
     144,   146,   148,   150,     0,   137,     0,     0,   141,     0,
       0,   429,   227,     0,   214,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   448,     0,     0,   301,   140,    22,     0,   393,     0,
       0,     0,     0,   419,   418,   357,   362,     0,   309,     0,
     204,     0,     0,   310,   314,     0,     0,   386,     0,     0,
       0,   366,     0,   365,     0,   206,     0,   393,   368,   371,
     212,     0,   211,   184,   345,     0,   164,   377,   380,   379,
     471,   472,   473,   475,     0,     0,   135,   134,     0,   133,
       0,     0,   488,   490,     0,   495,     0,   491,     0,   440,
     493,   492,     0,   299,   292,   286,   285,   287,   297,   298,
     288,   289,   291,   273,   290,   274,   277,   281,   271,     0,
     415,   240,   241,     0,   246,   244,   245,   256,   257,   247,
     248,   249,     0,     0,   258,     0,   260,     0,   253,   254,
       0,   238,   242,   416,   239,     0,   223,   224,     0,   225,
       0,   222,   218,     0,   303,     0,     0,   400,     0,     0,
       0,   420,     0,     0,     0,     0,     0,     0,   215,   216,
       0,     0,     0,   322,     0,     0,     0,     0,     0,     0,
       0,   327,   328,     0,     0,     0,   321,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,   164,   153,   138,
     142,   342,     0,     0,     0,   442,     0,     0,   243,     0,
       0,     0,     0,     0,     0,     0,     0,    23,     0,   397,
     398,   399,   394,   395,     0,   403,     0,     0,   349,   359,
       0,   373,   199,     0,   393,   387,   191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     201,     0,   403,   370,   217,   343,     0,   168,   163,   165,
       0,   496,     0,   497,   466,   417,   250,   251,   259,   252,
     262,   261,   255,   220,   221,   219,     0,     0,     0,     0,
     406,     0,     0,     0,     0,     0,   400,     0,     0,   405,
     332,   334,   335,   336,   338,   337,   331,   323,   324,   325,
     326,   329,   330,   333,     0,     0,   406,   207,   166,    30,
       0,     0,     0,     0,     0,   437,   396,   402,   401,     0,
       0,   353,     0,   525,     0,   358,   203,   403,     0,   317,
     363,   205,   193,    30,   169,     0,   498,    19,     0,   263,
     404,     0,   355,   524,     0,   406,     0,     0,     0,   167,
       0,     0,   407,   356,   354,   360,   195,   318,   364,   194,
       0,    20,     0,    24,    25,    22,     0,     0,   196,    12,
      21,   499
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,   102,   103,   104,   105,   106,
    1113,  1167,   954,  1165,    12,   176,   107,   424,   108,   109,
     110,   111,   233,   112,   113,   794,   588,   797,   528,   529,
     530,   531,   114,   514,   884,   115,   116,   483,   117,   118,
     234,   783,   996,   997,  1133,  1109,   119,   649,   120,   121,
     251,   122,   646,   123,   422,   124,   378,   503,   641,   125,
     126,   127,   355,   347,   128,  1148,   129,  1162,   438,   614,
     635,   852,   867,   130,   377,   871,   561,   970,   742,   952,
     131,   274,   555,   132,   459,   311,   741,   941,  1072,   133,
     456,   301,   455,   735,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   763,   144,   145,   146,   147,   769,
     986,  1096,   148,   149,   546,   150,   151,   152,   153,   154,
     155,   156,   157,   158,  1142,  1154,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   316,   348,   957,  1022,  1023,  1025,  1118,  1080,  1090,
    1121,   319,   595,   604,   942,   845,   605,   257,   352,   291,
     276,   432,   328,   329,   570,   571,   312,   313,   302,   303,
     401,   353,   882,   658,   659,   660,   403,   404,   405,   292,
     450,   246,   265,   367,   738,   419,   420,   421,   626,   333,
     334
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1025
static const yytype_int16 yypact[] =
{
     271,  -119,  -113,   129, -1025,   271,    11,   -84,   -81, -1025,
   -1025,   -61,   811,   -55, -1025,    51, -1025,   -34,   -24,    17,
      61, -1025,    73,    92,   111,   113,   117,   122,   132,   141,
     143,   155,   158,   160,   176,   197,   198,   209,   210,   211,
     213,   214,   217,   218,   219,   222,   228,   233,   234,   236,
     237,   244,   245,   246,   252,   263,   266,   267,   269,   279,
     280,   281,   283,   284,   285,   288,    16,   289,   291,   295,
   -1025,   298,   -39,   883, -1025, -1025,   300,   305,   306,   309,
     -17,    90,    32, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,   811, -1025,
   -1025,   -18, -1025, -1025, -1025, -1025, -1025,   310, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025,   477,    52, -1025,  -140,   399,    72,
     363,   382,   692,   114,   114,   123,   -36,    26,   270,   484,
     270,   772,  -130,  -130,   -12,   311,   270,   270,   432,   270,
      -7,  -130,  -130,   -27,   -12,   -12,   -12,    90,   316,    90,
     270,   692,   692,   270,   692,   692,    84,   270,   692,   270,
     -27,   692,  -130,   692,    74, -1025,   692,  -130,   385,   875,
      90, -1025, -1025,   282,   318,   270,   270,   270,   270,   321,
   -1025,    42, -1025,   325, -1025,   185, -1025,    19, -1025,   183,
   -1025,   332, -1025,    51, -1025, -1025,   336, -1025,   286,   343,
     345,   347, -1025, -1025, -1025, -1025, -1025,   351, -1025, -1025,
     349,   565, -1025,   357,   353,   362, -1025, -1025,  -130,  -130,
     270,   270,   364,   270,   367,   368,   372,   692,   379,   264,
   -1025, -1025,   320,   393, -1025, -1025, -1025,   609, -1025, -1025,
     398, -1025, -1025, -1025, -1025, -1025, -1025,   619, -1025, -1025,
     404,   403,   410, -1025, -1025, -1025,   411, -1025, -1025,   416,
     437,   438,   442, -1025, -1025,   652, -1025,   436,   444, -1025,
     447,   449,   667,   460, -1025,   480,   481,   487,   489,   492,
     718,   507,   508, -1025, -1025, -1025, -1025,   510,   717,   517,
     518,   520,   524,   458,   738,   527,   420,   533,   535, -1025,
     541, -1025, -1025, -1025, -1025, -1025, -1025, -1025,   547,   558,
     582,   583,   584,   590,   598, -1025, -1025,   599,   332,   604,
     605,   606, -1025,   596,    90,    90,   607,   626,   632,   635,
   -1025,   643,   259,    90,    90, -1025,    90,    90,    90,    90,
      90,   286,   458, -1025,   659,   657, -1025, -1025, -1025, -1025,
   -1025, -1025,   608,    64,    38, -1025, -1025,   286,   458,   660,
     663,   665,   811,   811, -1025, -1025,    90,  -140,   881,    29,
     882,   669,   666,   692,   670,    90,   227,   885,   672, -1025,
   -1025,   674,   675,   601, -1025,   270,   270,   340, -1025,   693,
     686,   692,   812,   695,    90,   697,   332,   698,    90,   332,
     270,   270,   692,   829,    22,   836,   700,    90,    14,   886,
     919,   708,   888,   887,   304,   746,   270,   840,   719,   844,
     -12,   -33, -1025,   721,   -12,   -12,   -12,    90,   724,    43,
     270,    33,   841,   -10,   759,   847,   846,   -28,    60,   820,
     821,    26,   818, -1025,    40,    40,   810, -1025,   -54,   734,
     -44,   819,   824,    36,   858, -1025, -1025,   334,   413,    91,
      91, -1025, -1025, -1025,    19, -1025,   692,   743,  -106,  -101,
     -90,   -87, -1025, -1025,   286,   458,   107,   149,    83, -1025,
     737,   440, -1025, -1025, -1025,   959, -1025,   750,   351, -1025,
     749,   966,   474, -1025, -1025,   362, -1025, -1025,   270,   270,
     712,   754,   758,   769,   775, -1025,   776,   439, -1025,   773,
     778, -1025,   779,   780, -1025, -1025, -1025,   774,   506,   557,
   -1025,   777,   516, -1025, -1025, -1025, -1025,   781,   783, -1025,
     790,  1005,   521, -1025, -1025,   793, -1025,   794,  1010, -1025,
     798, -1025,   795, -1025,   800, -1025,   801,   803, -1025,  1014,
   -1025,   804, -1025,  1019,   807,    14,   809,   814, -1025,   816,
     947, -1025, -1025, -1025, -1025, -1025,   817, -1025,   823,   825,
   -1025,   827,   848, -1025,  1024,   849, -1025,   851, -1025,   852,
     870,   901,  1034,   871,   872, -1025,   332, -1025,   764,    90,
   -1025, -1025,   286,   873,   874, -1025, -1025,   845, -1025,   896,
     822, -1025, -1025, -1025, -1025,  1060,   909, -1025,   -20, -1025,
      90, -1025,  -140, -1025,    27, -1025,    22, -1025,    30, -1025,
   -1025, -1025,   898,  1129, -1025, -1025,   914, -1025,   910, -1025,
     918,  1057,   692, -1025,    90,    90,   692,   921,    90,   692,
     692,   920,   923, -1025, -1025, -1025, -1025,   922, -1025, -1025,
   -1025, -1025,   926, -1025,   927,   928,   929, -1025,   930,   931,
     932,   933,   934,   935,   936,   937, -1025,   938, -1025,   939,
   -1025, -1025, -1025,   940, -1025, -1025, -1025, -1025, -1025,   941,
   -1025,   942,   943,    26,    22,  1157,   946, -1025,   -27,  1160,
     948,   984,  1163,    68,   123,   986,   -74,  1079,   953,   -14,
   -1025,   949,  1081, -1025,    90,   955,  -140,  1107,   270,   958,
    1080,    59,   961,    -8,  1082,  1172,   273,   332,  1083,    40,
   -1025, -1025,   458,   960,    14,   -40,   152,   858, -1025,    23,
   -1025, -1025,   458,   286,   -51, -1025,   -41,     5, -1025,    14,
     964, -1025, -1025,    90, -1025,   962,   221,   428,   965,    14,
     647,   967,   968,    90, -1025,    90,   114,  1077,  1084,    90,
    1073,  1075,    90,    90,    90,  -140,  1042,    90,    90,    90,
     123, -1025,   905,    13, -1025, -1025,   975,  1189,   977,   978,
    1192,   979,   976, -1025, -1025,   982, -1025,   983, -1025,  1196,
   -1025,   307,   985, -1025, -1025,   987,    67,   286,   988,   989,
     572, -1025,   990, -1025,  1199, -1025,  1204,   977, -1025,   992,
   -1025,   993, -1025, -1025, -1025,   994,    70, -1025, -1025,   286,
   -1025, -1025, -1025, -1025,   332,    27, -1025, -1025,    30, -1025,
     995,  1207,   458, -1025,  1173, -1025,    90, -1025,   996, -1025,
   -1025, -1025,   496, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025,   286, -1025, -1025, -1025, -1025, -1025,    18,
   -1025, -1025, -1025,  -140, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025,   999,  1000, -1025,  1001, -1025,  1002, -1025, -1025,
    1003, -1025, -1025, -1025,   286,  1004, -1025, -1025,  1006, -1025,
    1007, -1025, -1025,  1210, -1025,  1009,   262,  1122,  1223,  1011,
     123, -1025,   -23,   123,  1012,  1013,    -8,    90, -1025, -1025,
    1015,  1162,  1139, -1025,  1016,  1018,  1020,  1021,  1022,  1023,
    1025, -1025, -1025,  1026,  1027,  1028, -1025,  1133,  1029,  1030,
    1122,  1229,    67,  1235,   223,  1032,  1031,    70, -1025, -1025,
   -1025, -1025,  1035,  1033,   302, -1025,    90,  1167,   286,    90,
      90,    90,    90,   -97,   114,  1247,  1115, -1025,  1252, -1025,
   -1025, -1025, -1025,  1040,  1041,  1164,  1043,  1254, -1025,  1044,
    1045, -1025, -1025,   227,   977, -1025, -1025,  1047,  1048,  1169,
    1263,    77,   123,   114,    29,    29,   270,   -36,  1264,  1052,
   -1025,  1266,  1164, -1025, -1025, -1025,  1053, -1025, -1025, -1025,
    1267, -1025,  1232, -1025,   124, -1025, -1025, -1025, -1025, -1025,
   -1025,   997, -1025, -1025, -1025, -1025,  1054,   262,  1176,  1059,
    1096,  1275,  1061,  1098,   123,  1062,  1122,  1191,  1193, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025,  1180,  1066,  1096,   943, -1025,   811,
    1070,  1068,  1074,  1071,   -97, -1025, -1025, -1025, -1025,  1169,
    1072, -1025,  1076, -1025,  1085, -1025, -1025,  1164,  1078, -1025,
    1087, -1025, -1025,   811, -1025,    14, -1025, -1025,  1088, -1025,
   -1025,   123,   332, -1025,   123,  1096,  1201,    14,   332, -1025,
    1089,   270, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
      86,  1090,   332, -1025, -1025,   975,   270,  1091, -1025, -1025,
   -1025, -1025
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1025, -1025,  1288, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025,   133, -1025, -1025,  1046,  -108,  -376,   878,  1065,
    1224,  -495, -1025, -1025, -1025, -1025,   625, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,  1236, -1025,
   -1025, -1025,   312, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025,   826,  1092, -1025, -1025, -1025, -1025,   753, -1025,
   -1025, -1025,   344, -1025, -1025, -1025,  -622,   322,   265, -1025,
   -1025,   462,   287, -1025, -1025, -1025, -1025, -1025,   202, -1025,
   -1025,  1134, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025,  -641, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025, -1025,
   -1025,   274,   569,  -831,   247, -1025,  -925, -1025,  -974,   203,
   -1024,  -472,  -609, -1025, -1025, -1025,   491,   899,  -227,  -179,
    -390,   782,   212,  -386,  -478,  -643,  -530,  -451,  -528,  -635,
    -172,   -77, -1025,   538, -1025, -1025,   805, -1025, -1025,  1049,
     -76,   748, -1025,  -477, -1025, -1025, -1025, -1025, -1025,  -190,
   -1025
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -235
static const yytype_int16 yytable[] =
{
     248,   275,   502,   383,   245,   258,   760,   677,   675,   320,
     342,   322,   622,   589,   349,   350,   351,   336,   337,   657,
     339,   341,    16,   240,   532,   305,   643,   644,   533,   640,
     294,   357,   544,   565,   360,   798,   990,   651,   368,   795,
     370,   878,   240,   565,   331,   314,   240,   363,   612,   331,
     391,   364,   365,   633,   345,   628,   386,   387,   388,   389,
     679,   266,   267,   862,   636,  1052,  1029,   850,   527,   804,
     415,   968,   391,   865,   266,   267,   263,   417,  1106,   375,
     580,  1092,  1132,   583,  1070,   254,   565,   593,   363,   325,
     945,  1163,   364,   365,   240,   594,   255,   256,   317,     7,
     647,   441,   442,   306,   444,     8,   293,   318,   295,   624,
     294,   666,   326,   327,   625,   324,   668,   294,   667,   266,
     267,  1156,   296,   669,   946,   947,   305,   670,  1071,     9,
     672,   948,   356,    11,   671,   358,   359,   673,   361,   362,
     949,    13,   369,   840,    14,   372,   801,   374,   969,   843,
     379,   950,   305,  1145,   872,   376,   844,    15,   509,  1030,
     637,  1127,   174,   863,   392,   366,   885,  1164,   264,  1093,
     402,  1094,   418,   886,   175,   877,   744,   951,   655,   629,
     656,   254,   230,   887,   177,   254,   613,   240,   295,   315,
     890,   634,   255,   256,   178,   295,   255,   256,   346,   271,
     898,   270,   296,  1086,   306,   851,   406,   332,   407,   296,
     239,   866,   340,   408,   409,   410,  1095,   396,   397,   398,
     399,   400,   888,   648,   846,   589,   994,   363,   995,   889,
     306,   364,   365,   272,   273,   179,  1007,    81,   241,   752,
     837,   307,   534,   801,   254,  1000,   297,   308,   545,   642,
     999,   247,   298,   242,   243,   255,   256,   413,   249,   642,
     619,   241,   354,   254,   266,   267,   563,   564,   434,   253,
     780,   834,   242,   243,   255,   256,   242,   243,   791,   180,
     790,   584,   585,   241,   568,   569,     1,     2,   904,   244,
     611,   181,   657,   835,   616,   617,   618,   607,   242,   243,
     918,   875,   642,  1019,  1020,  1021,   678,   508,   244,   241,
     182,   623,   244,   553,   554,   540,   517,   518,   411,   519,
     520,   521,   522,   523,   242,   243,   297,   568,   569,   183,
     674,   184,   298,   297,   244,   185,   392,   535,   652,   298,
     186,  1112,   307,   565,   -18,   266,   267,   566,   308,   541,
     187,   266,   267,   560,   870,   299,   300,   550,   552,   188,
     244,   189,   299,   300,   398,   399,   400,   562,   307,   309,
     310,   572,   676,   190,   308,   575,   191,   578,   192,   689,
     690,   582,   880,   881,   394,   601,   586,   602,   603,   412,
     592,   270,   268,   269,   193,   309,   310,   398,   399,   400,
     736,   873,   413,  1099,  1100,   330,   414,   395,   254,   259,
     260,   261,   262,   343,   344,   194,   195,   242,   243,   255,
     256,   266,   267,   272,   273,   326,   327,   196,   197,   198,
    1028,   199,   200,  1031,   373,   201,   202,   203,   894,   380,
     204,   895,   697,   434,   266,   267,   205,   402,   268,   269,
     664,   206,   207,   244,   208,   209,   396,   397,   398,   399,
     400,   416,   210,   211,   212,   278,   279,   280,   281,   282,
     213,   283,   270,   284,   285,   286,   287,   288,   270,   516,
     252,   214,   277,   289,   215,   216,  1073,   217,   321,   271,
     439,   440,   396,   397,   398,   399,   400,   218,   219,   220,
     793,   221,   222,   223,   272,   273,   224,   226,   998,   227,
     272,   273,  1097,   228,   290,  1098,   229,  1056,   235,  1062,
     266,   267,  1063,   236,   237,   338,  1150,   238,   250,   381,
     396,   397,   398,   399,   400,   354,   335,   855,  1158,   385,
     390,   278,   279,   280,   281,   282,   393,   283,   270,   284,
     285,   286,   287,   288,  1125,   423,   661,   426,   384,   567,
     428,   704,   429,   427,   430,   705,   706,   431,   433,   434,
     436,   270,   782,   396,   397,   398,   399,   400,   435,   437,
     272,   273,   271,   443,   568,   569,   445,   446,   903,   859,
     290,   447,   907,   792,   857,   910,   911,   912,   449,   451,
     915,   916,   917,   272,   273,   396,   397,   398,   399,   400,
     452,  1152,   453,   879,  1155,   707,   805,   806,   807,   454,
     808,   810,   457,   811,   812,   458,  -234,   460,   461,   708,
     709,   710,   711,   462,   712,   662,   713,   490,   266,   267,
     278,   279,   280,   281,   282,   896,   283,   270,   284,   285,
     286,   287,   288,   913,   463,   464,   466,   467,   289,   465,
     944,   468,   681,   973,   469,   714,   715,   716,   470,   717,
     974,   471,   718,   719,   720,   721,   722,   472,   723,   272,
     273,   560,   266,   267,   396,   397,   398,   399,   400,   290,
     975,   396,   397,   398,   399,   400,   687,   473,   474,   396,
     397,   398,   399,   400,   475,   724,   476,   725,   726,   477,
     976,   396,   397,   398,   399,   400,   727,   728,   729,   730,
     731,   732,   478,   482,   479,   480,   892,   481,   703,   396,
     397,   398,   399,   400,   484,   485,   902,   486,   740,   733,
    1035,   487,   488,   747,   489,   396,   397,   398,   399,   400,
     491,  1008,   492,   734,   977,   978,   979,   980,   493,   981,
     982,   983,   984,   985,   494,   270,  1153,   396,  1006,   398,
     399,   400,  1159,   266,   267,   495,   271,   396,   397,   398,
     399,   400,  1066,  1067,  1068,  1069,  1168,   396,   397,   398,
     399,   400,   396,   397,   398,   399,   400,   272,   273,   496,
     497,   498,   278,   279,   280,   281,   282,   499,   283,   270,
     284,   285,   286,   287,   288,   500,   501,    16,   507,  1004,
     289,   504,   505,   506,   510,    17,   526,   244,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   272,   273,   511,    31,    32,    33,    34,    35,   512,
      36,   290,   513,   266,   267,    37,    38,    39,    40,    41,
      42,    43,    44,   515,    45,    46,    47,  1101,    48,   899,
      49,    50,    51,    52,    53,    54,   524,   525,   536,    55,
      56,   537,    57,   538,   543,   547,   548,   551,   549,    58,
     556,   558,   559,   278,   279,   280,   281,   282,   557,   283,
     270,   284,   285,   286,   287,   288,   574,   573,   576,   587,
      59,   289,   577,    60,   579,   581,   590,   591,   396,   397,
     398,   399,   400,   597,   596,   598,   599,   600,   606,  1064,
     608,    61,   272,   273,   610,   627,   609,    62,   615,    63,
      64,   620,   290,    65,    66,   630,    67,   631,    68,   632,
     638,   639,  -183,   323,    69,   645,   650,    70,    71,   653,
     665,   680,   682,    72,    73,   654,    74,   683,    75,   685,
     686,   692,  1161,   278,   279,   280,   281,   282,   693,   283,
     270,   284,   285,   286,   287,   288,   694,  1170,   919,   920,
     691,   289,   695,   696,   698,   699,   700,   701,   743,   702,
     744,  1134,   739,    76,    77,    78,    79,   745,    80,   746,
     748,   749,   272,   273,   750,   751,   752,   753,   756,   754,
     755,   757,   290,   758,   759,  1149,   761,   765,   772,   921,
     922,   762,    81,   764,   766,   777,   923,   697,    82,    83,
     767,   781,   788,   768,   770,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   786,   789,   771,   773,   924,   774,   775,
     925,   926,   927,   928,   929,   930,   931,   932,   933,   934,
     935,   936,   937,   938,   939,   940,    83,   776,   778,   779,
     784,   785,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      85,   231,    87,   787,    89,   799,    91,    84,    93,    86,
      95,    88,    97,    90,    99,    92,   101,    94,   325,    96,
     254,    98,   800,   100,   801,   803,   802,   560,   809,   813,
     815,   255,   256,   814,   816,   817,   818,   819,   820,   821,
     822,   823,   824,   825,   826,   827,   828,   829,   830,   832,
     833,   831,   836,   837,   839,   840,   841,   842,   847,   848,
     849,   854,   856,   858,   853,   860,   869,   861,   864,   868,
     874,   891,   893,   876,   905,   897,   908,   900,   901,   909,
     906,   914,   953,   955,   956,   958,   959,   960,   961,   962,
     964,   963,   966,   988,   967,   971,   972,   987,   989,   991,
     992,   993,  1002,  1017,  1003,  1001,  1005,  1009,  1010,  1011,
    1012,  1013,  1014,  1024,  1015,  1016,  1018,  1026,  1027,  1037,
    1033,  1038,  1032,  1053,  1039,  1036,  1040,  1049,  1041,  1042,
    1043,  1044,  1055,  1045,  1046,  1047,  1048,  1051,  1065,  1050,
    1057,  1074,  1060,  1061,  1075,  1058,  1076,  1077,  1082,  1078,
    1081,  1083,  1079,  1084,  1087,  1088,  1089,  1091,  1103,  1104,
    1105,  1108,  1110,  1111,  1115,  1114,  1117,  1119,  1120,  1122,
    1124,  1123,  1126,  1128,  1129,  1130,  1131,  1135,  1136,  1137,
    1141,  1138,  1157,    10,   382,  1146,  1143,   232,  1169,   425,
     539,   796,   225,  1144,  1147,  1151,  1160,  1166,   688,  1059,
    1034,  1171,   371,   965,  1054,   621,  1139,   838,   304,  1107,
    1085,  1102,  1140,   943,  1116,   883,   542,   737,     0,   663,
     684,     0,     0,     0,     0,     0,   448
};

static const yytype_int16 yycheck[] =
{
     108,   180,   378,   230,    81,   177,   615,   537,   536,   188,
     200,   190,   489,   464,   204,   205,   206,   196,   197,   514,
     199,   200,     6,     4,   414,     3,   504,   505,   414,   501,
       3,   210,     3,     3,   213,   678,   867,    81,   217,   674,
     219,    81,     4,     3,    56,    81,     4,     4,    81,    56,
       8,     8,     9,    81,    81,    65,   235,   236,   237,   238,
     538,    81,    82,     4,     4,   990,    89,    81,     4,   691,
     249,     4,     8,    81,    81,    82,     4,   249,  1052,     5,
     456,     4,  1106,   459,   181,   225,     3,    73,     4,   219,
      77,     5,     8,     9,     4,    81,   236,   237,    72,   218,
     154,   280,   281,    81,   283,   218,   182,    81,    81,    76,
       3,   217,   242,   243,    81,   191,   217,     3,   224,    81,
      82,  1145,    95,   224,   111,   112,     3,   217,   225,     0,
     217,   118,   209,   122,   224,   211,   212,   224,   214,   215,
     127,   225,   218,   217,   225,   221,   220,   223,    81,    81,
     226,   138,     3,  1127,   776,    81,    88,   218,   385,   182,
     100,  1086,   217,   104,   241,    81,   217,    81,    96,    92,
     247,    94,   249,   224,   123,   784,   217,   164,   142,   189,
     144,   225,   221,   224,   218,   225,   219,     4,    81,   225,
     799,   219,   236,   237,   218,    81,   236,   237,   225,   219,
     809,   208,    95,  1034,    81,   219,    23,   219,    25,    95,
     227,   219,   219,    30,    31,    32,   139,   271,   272,   273,
     274,   275,   217,   277,   754,   676,   156,     4,   158,   224,
      81,     8,     9,   240,   241,   218,   218,   221,   219,   221,
     217,   219,   414,   220,   225,   888,   219,   225,   219,   219,
     885,   219,   225,   234,   235,   236,   237,   219,   276,   219,
     487,   219,   219,   225,    81,    82,   445,   446,     4,   217,
     646,   743,   234,   235,   236,   237,   234,   235,   668,   218,
     666,   460,   461,   219,   244,   245,    15,    16,   816,   270,
     480,   218,   787,   744,   484,   485,   486,   476,   234,   235,
     830,   779,   219,    41,    42,    43,   223,   384,   270,   219,
     218,   490,   270,    86,    87,   423,   393,   394,   135,   396,
     397,   398,   399,   400,   234,   235,   219,   244,   245,   218,
     223,   218,   225,   219,   270,   218,   413,   414,   510,   225,
     218,   217,   219,     3,   220,    81,    82,     7,   225,   426,
     218,    81,    82,    80,    81,   248,   249,   433,   435,   218,
     270,   218,   248,   249,   273,   274,   275,   443,   219,   246,
     247,   447,   223,   218,   225,   451,   218,   454,   218,   558,
     559,   458,   230,   231,   199,    81,   462,    83,    84,   206,
     467,   208,    85,    86,   218,   246,   247,   273,   274,   275,
     579,   777,   219,  1044,  1045,   193,   223,   222,   225,    10,
      11,    12,    13,   201,   202,   218,   218,   234,   235,   236,
     237,    81,    82,   240,   241,   242,   243,   218,   218,   218,
     960,   218,   218,   963,   222,   218,   218,   218,   217,   227,
     218,   220,     3,     4,    81,    82,   218,   524,    85,    86,
     526,   218,   218,   270,   218,   218,   271,   272,   273,   274,
     275,   249,   218,   218,   218,   201,   202,   203,   204,   205,
     218,   207,   208,   209,   210,   211,   212,   213,   208,   220,
       3,   218,   100,   219,   218,   218,  1014,   218,     4,   219,
     278,   279,   271,   272,   273,   274,   275,   218,   218,   218,
     672,   218,   218,   218,   240,   241,   218,   218,   884,   218,
     240,   241,  1042,   218,   250,  1043,   218,   994,   218,   217,
      81,    82,   220,   218,   218,    93,  1135,   218,   218,   144,
     271,   272,   273,   274,   275,   219,   225,   764,  1147,   221,
     219,   201,   202,   203,   204,   205,   221,   207,   208,   209,
     210,   211,   212,   213,  1084,   223,   222,   221,   276,   219,
     217,     4,   217,   277,   217,     8,     9,   216,   219,     4,
     217,   208,   649,   271,   272,   273,   274,   275,   221,   217,
     240,   241,   219,   219,   244,   245,   219,   219,   815,   768,
     250,   219,   819,   670,   766,   822,   823,   824,   219,   279,
     827,   828,   829,   240,   241,   271,   272,   273,   274,   275,
     217,  1141,     3,   785,  1144,    58,   692,   694,   695,   221,
     696,   698,     3,   699,   700,   221,   223,   217,   217,    72,
      73,    74,    75,   217,    77,   222,    79,   217,    81,    82,
     201,   202,   203,   204,   205,   217,   207,   208,   209,   210,
     211,   212,   213,   825,   217,   217,     4,   221,   219,   217,
     832,   217,   222,    91,   217,   108,   109,   110,   219,   112,
      98,     4,   115,   116,   117,   118,   119,   217,   121,   240,
     241,    80,    81,    82,   271,   272,   273,   274,   275,   250,
     118,   271,   272,   273,   274,   275,   222,   217,   217,   271,
     272,   273,   274,   275,   217,   148,   217,   150,   151,   217,
     138,   271,   272,   273,   274,   275,   159,   160,   161,   162,
     163,   164,     4,     6,   217,   217,   803,   217,   222,   271,
     272,   273,   274,   275,   217,   217,   813,   217,   222,   182,
     967,   217,     4,   222,   217,   271,   272,   273,   274,   275,
     217,   923,   217,   196,   182,   183,   184,   185,   217,   187,
     188,   189,   190,   191,   217,   208,  1142,   271,   272,   273,
     274,   275,  1148,    81,    82,   217,   219,   271,   272,   273,
     274,   275,  1009,  1010,  1011,  1012,  1162,   271,   272,   273,
     274,   275,   271,   272,   273,   274,   275,   240,   241,   217,
     217,   217,   201,   202,   203,   204,   205,   217,   207,   208,
     209,   210,   211,   212,   213,   217,   217,     6,   222,   896,
     219,   217,   217,   217,   217,    14,   218,   270,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,   240,   241,   217,    33,    34,    35,    36,    37,   217,
      39,   250,   217,    81,    82,    44,    45,    46,    47,    48,
      49,    50,    51,   220,    53,    54,    55,  1046,    57,   222,
      59,    60,    61,    62,    63,    64,   217,   220,   218,    68,
      69,   218,    71,   218,     3,     3,   217,   217,   222,    78,
       5,   217,   217,   201,   202,   203,   204,   205,   226,   207,
     208,   209,   210,   211,   212,   213,   220,   214,    96,    80,
      99,   219,   217,   102,   217,   217,    80,   217,   271,   272,
     273,   274,   275,     4,    38,   217,    38,    40,   182,  1006,
      90,   120,   240,   241,    90,    94,   217,   126,   217,   128,
     129,   217,   250,   132,   133,   186,   135,   100,   137,   103,
     130,   130,   134,   181,   143,   145,   222,   146,   147,   140,
     217,   224,     3,   152,   153,   141,   155,   217,   157,   220,
       4,   217,  1151,   201,   202,   203,   204,   205,   220,   207,
     208,   209,   210,   211,   212,   213,   217,  1166,    83,    84,
     278,   219,   217,   217,   221,   217,   217,   217,   217,   225,
     217,  1109,   225,   192,   193,   194,   195,   217,   197,     4,
     217,   217,   240,   241,     4,   217,   221,   217,     4,   218,
     217,   217,   250,     4,   217,  1133,   217,    80,     4,   124,
     125,   217,   221,   217,   217,   134,   131,     3,   227,   228,
     217,   277,   220,   218,   217,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   218,     4,   217,   217,   162,   217,   217,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   228,   217,   217,   217,
     217,   217,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     235,   228,   237,   217,   239,   217,   241,   234,   243,   236,
     245,   238,   247,   240,   249,   242,   251,   244,   219,   246,
     225,   248,     3,   250,   220,   217,   226,    80,   217,   219,
     218,   236,   237,   220,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   217,
     217,   220,     5,   217,     4,   217,   182,     4,   182,    90,
     217,    90,   217,    66,   225,   217,     4,    97,   217,    97,
      97,   217,   220,   223,   107,   220,   113,   220,   220,   114,
     106,   149,   217,     4,   217,   217,     4,   218,   222,   217,
       4,   218,   217,     4,   217,   217,   217,   217,     4,   217,
     217,   217,     5,     3,    41,   220,   220,   218,   218,   218,
     218,   218,   218,   101,   218,   218,   217,     4,   217,    67,
     217,    92,   220,     4,   218,   220,   218,   104,   218,   218,
     218,   218,     7,   218,   218,   218,   218,   217,    81,   220,
     218,     4,   217,   220,   139,   224,     4,   217,     4,   218,
     217,   217,    98,   218,   217,   217,    97,     4,     4,   217,
       4,   218,     5,    41,   220,   278,   100,   218,   182,     4,
     182,   220,   220,    92,    91,   105,   220,   217,   220,   215,
     218,   220,    91,     5,   229,   217,   220,    73,  1165,   253,
     422,   676,    66,   218,   217,   217,   217,   217,   555,   997,
     966,   220,   220,   851,   992,   489,  1114,   748,   184,  1054,
    1033,  1047,  1119,   832,  1077,   787,   427,   579,    -1,   524,
     548,    -1,    -1,    -1,    -1,    -1,   287
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   281,   282,   283,   284,   218,   218,     0,
     282,   122,   294,   225,   225,   218,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    33,    34,    35,    36,    37,    39,    44,    45,    46,
      47,    48,    49,    50,    51,    53,    54,    55,    57,    59,
      60,    61,    62,    63,    64,    68,    69,    71,    78,    99,
     102,   120,   126,   128,   129,   132,   133,   135,   137,   143,
     146,   147,   152,   153,   155,   157,   192,   193,   194,   195,
     197,   221,   227,   228,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   285,   286,   287,   288,   289,   296,   298,   299,
     300,   301,   303,   304,   312,   315,   316,   318,   319,   326,
     328,   329,   331,   333,   335,   339,   340,   341,   344,   346,
     353,   360,   363,   369,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   385,   386,   387,   388,   392,   393,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   406,
     407,   408,   409,   410,   411,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   217,   123,   295,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   318,   218,   218,   218,   218,
     221,   228,   300,   302,   320,   218,   218,   218,   218,   227,
       4,   219,   234,   235,   270,   451,   461,   219,   296,   276,
     218,   330,     3,   217,   225,   236,   237,   437,   450,    10,
      11,    12,    13,     4,    96,   462,    81,    82,    85,    86,
     208,   219,   240,   241,   361,   439,   440,   100,   201,   202,
     203,   204,   205,   207,   209,   210,   211,   212,   213,   219,
     250,   439,   459,   460,     3,    81,    95,   219,   225,   248,
     249,   371,   448,   449,   371,     3,    81,   219,   225,   246,
     247,   365,   446,   447,    81,   225,   421,    72,    81,   431,
     439,     4,   439,   181,   460,   219,   242,   243,   442,   443,
     442,    56,   219,   469,   470,   225,   439,   439,    93,   439,
     219,   439,   469,   442,   442,    81,   225,   343,   422,   469,
     469,   469,   438,   451,   219,   342,   451,   439,   460,   460,
     439,   460,   460,     4,     8,     9,    81,   463,   439,   460,
     439,   343,   460,   442,   460,     5,    81,   354,   336,   460,
     442,   144,   299,   438,   276,   221,   439,   439,   439,   439,
     219,     8,   451,   221,   199,   222,   271,   272,   273,   274,
     275,   450,   451,   456,   457,   458,    23,    25,    30,    31,
      32,   135,   206,   219,   223,   439,   442,   450,   451,   465,
     466,   467,   334,   223,   297,   295,   221,   277,   217,   217,
     217,   216,   441,   219,     4,   221,   217,   217,   348,   442,
     442,   439,   439,   219,   439,   219,   219,   219,   459,   219,
     460,   279,   217,     3,   221,   372,   370,     3,   221,   364,
     217,   217,   217,   217,   217,   217,     4,   221,   217,   217,
     219,     4,   217,   217,   217,   217,   217,   217,     4,   217,
     217,   217,     6,   317,   217,   217,   217,   217,     4,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   297,   337,   217,   217,   217,   222,   451,   438,
     217,   217,   217,   217,   313,   220,   220,   451,   451,   451,
     451,   451,   451,   451,   217,   220,   218,     4,   308,   309,
     310,   311,   440,   443,   450,   451,   218,   218,   218,   298,
     296,   451,   437,     3,     3,   219,   394,     3,   217,   222,
     460,   217,   451,    86,    87,   362,     5,   226,   217,   217,
      80,   356,   460,   439,   439,     3,     7,   219,   244,   245,
     444,   445,   460,   214,   220,   460,    96,   217,   451,   217,
     297,   217,   451,   297,   439,   439,   460,    80,   306,   447,
      80,   217,   451,    73,    81,   432,    38,     4,   217,    38,
      40,    81,    83,    84,   433,   436,   182,   439,    90,   217,
      90,   469,    81,   219,   349,   217,   469,   469,   469,   438,
     217,   342,   463,   439,    76,    81,   468,    94,    65,   189,
     186,   100,   103,    81,   219,   350,     4,   100,   130,   130,
     431,   338,   219,   444,   444,   145,   332,   154,   277,   327,
     222,    81,   450,   140,   141,   142,   144,   301,   453,   454,
     455,   222,   222,   456,   460,   217,   217,   224,   217,   224,
     217,   224,   217,   224,   223,   448,   223,   446,   223,   444,
     224,   222,     3,   217,   441,   220,     4,   222,   348,   439,
     439,   278,   217,   220,   217,   217,   217,     3,   221,   217,
     217,   217,   225,   222,     4,     8,     9,    58,    72,    73,
      74,    75,    77,    79,   108,   109,   110,   112,   115,   116,
     117,   118,   119,   121,   148,   150,   151,   159,   160,   161,
     162,   163,   164,   182,   196,   373,   439,   461,   464,   225,
     222,   366,   358,   217,   217,   217,     4,   222,   217,   217,
       4,   217,   221,   217,   218,   217,     4,   217,     4,   217,
     432,   217,   217,   384,   217,    80,   217,   217,   218,   389,
     217,   217,     4,   217,   217,   217,   217,   134,   217,   217,
     297,   277,   451,   321,   217,   217,   218,   217,   220,     4,
     443,   440,   451,   450,   305,   449,   306,   307,   445,   217,
       3,   220,   226,   217,   356,   460,   451,   451,   460,   217,
     451,   460,   460,   219,   220,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   220,   217,   217,   431,   447,     5,   217,   422,     4,
     217,   182,     4,    81,    88,   435,   446,   182,    90,   217,
      81,   219,   351,   225,    90,   438,   217,   450,    66,   439,
     217,    97,     4,   104,   217,    81,   219,   352,    97,     4,
      81,   355,   356,   297,    97,   444,   223,   432,    81,   450,
     230,   231,   452,   453,   314,   217,   224,   224,   217,   224,
     432,   217,   451,   220,   217,   220,   217,   220,   432,   222,
     220,   220,   451,   438,   448,   107,   106,   438,   113,   114,
     438,   438,   438,   450,   149,   438,   438,   438,   446,    83,
      84,   124,   125,   131,   162,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   367,   434,   436,   450,    77,   111,   112,   118,   127,
     138,   164,   359,   217,   292,     4,   217,   423,   217,     4,
     218,   222,   217,   218,     4,   361,   217,   217,     4,    81,
     357,   217,   217,    91,    98,   118,   138,   182,   183,   184,
     185,   187,   188,   189,   190,   191,   390,   217,     4,     4,
     423,   217,   217,   217,   156,   158,   322,   323,   297,   449,
     445,   220,     5,    41,   451,   220,   272,   218,   450,   218,
     218,   218,   218,   218,   218,   218,   218,     3,   217,    41,
      42,    43,   424,   425,   101,   426,     4,   217,   446,    89,
     182,   446,   220,   217,   352,   438,   220,    67,    92,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   104,
     220,   217,   426,     4,   357,     7,   463,   218,   224,   322,
     217,   220,   217,   220,   451,    81,   438,   438,   438,   438,
     181,   225,   368,   448,     4,   139,     4,   217,   218,    98,
     428,   217,     4,   217,   218,   362,   423,   217,   217,    97,
     429,     4,     4,    92,    94,   139,   391,   446,   448,   394,
     394,   439,   421,     4,   217,     4,   428,   358,   218,   325,
       5,    41,   217,   290,   278,   220,   424,   100,   427,   218,
     182,   430,     4,   220,   182,   446,   220,   426,    92,    91,
     105,   220,   430,   324,   296,   217,   220,   215,   220,   368,
     429,   218,   404,   220,   218,   428,   217,   217,   345,   296,
     432,   217,   446,   297,   405,   446,   430,    91,   432,   297,
     217,   439,   347,     5,    81,   293,   217,   291,   297,   292,
     439,   220
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
#line 346 "lev_comp.y"
    {
		      int filling = (int) (yyvsp[(5) - (5)].terr).ter;

		      if (filling == INVALID_TYPE || filling >= MAX_TYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_SOLIDFILL, filling,
                                          0, (int) (yyvsp[(5) - (5)].terr).lit,
                                          0,0,0,0, SPO_INITLEVEL));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 359 "lev_comp.y"
    {
		      int filling = (int) what_map_char((char) (yyvsp[(5) - (5)].i));

		      if (filling == INVALID_TYPE || filling >= MAX_TYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");
                      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_MAZEGRID, filling, 0,0,
					  0,0,0,0, SPO_INITLEVEL));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 371 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 377 "lev_comp.y"
    {
                      int fg = (int) what_map_char((char) (yyvsp[(5) - (16)].i)),
                          bg = (int) what_map_char((char) (yyvsp[(7) - (16)].i));
                      int smoothed = (int) (yyvsp[(9) - (16)].i),
                          joined = (int) (yyvsp[(11) - (16)].i),
                          lit = (int) (yyvsp[(13) - (16)].i),
                          walled = (int) (yyvsp[(15) - (16)].i),
                          filling = (int) (yyvsp[(16) - (16)].i);

		      if (fg == INVALID_TYPE || fg >= MAX_TYPE)
			  lc_error("INIT_MAP: Invalid foreground type.");
		      if (bg == INVALID_TYPE || bg >= MAX_TYPE)
			  lc_error("INIT_MAP: Invalid background type.");
		      if (joined && fg != CORR && fg != ROOM && fg != GRASS && fg != GROUND && fg != AIR && fg != CLOUD)
			  lc_error("INIT_MAP: Invalid foreground type for joined map.");

		      if (filling == INVALID_TYPE)
			  lc_error("INIT_MAP: Invalid fill char type.");

		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_MINES, filling, walled, lit,
					  joined, smoothed, bg, fg,
					  SPO_INITLEVEL));
			max_x_map = COLNO-1;
			max_y_map = ROWNO;
		  ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 406 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 412 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_FOREST));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 416 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(0, (int)(yyvsp[(3) - (3)].i), SPO_FOREST));
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 422 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), (int)(yyvsp[(3) - (5)].i), SPO_MONSTER_GENERATION));
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 428 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 434 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 438 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 444 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 449 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 455 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 459 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 470 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 474 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 481 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 485 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 491 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 495 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 501 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 605 "lev_comp.y"
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

  case 122:

/* Line 1455 of yacc.c  */
#line 621 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 627 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 633 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 639 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 645 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 651 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 657 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 663 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 669 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 679 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 689 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 699 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 709 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 719 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 729 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 741 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 746 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 753 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 758 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 765 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 770 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 777 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 781 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 787 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 792 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 799 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 803 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 809 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 813 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 819 "lev_comp.y"
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

  case 152:

/* Line 1455 of yacc.c  */
#line 841 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 845 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 856 "lev_comp.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 911 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 917 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 927 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 934 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 938 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 946 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 950 "lev_comp.y"
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

  case 163:

/* Line 1455 of yacc.c  */
#line 973 "lev_comp.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1018 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1028 "lev_comp.y"
    {
		  ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1031 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1041 "lev_comp.y"
    {
		  ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1046 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1060 "lev_comp.y"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1097 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1102 "lev_comp.y"
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

  case 176:

/* Line 1455 of yacc.c  */
#line 1131 "lev_comp.y"
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

  case 177:

/* Line 1455 of yacc.c  */
#line 1145 "lev_comp.y"
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

  case 178:

/* Line 1455 of yacc.c  */
#line 1161 "lev_comp.y"
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

  case 179:

/* Line 1455 of yacc.c  */
#line 1181 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1193 "lev_comp.y"
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

  case 181:

/* Line 1455 of yacc.c  */
#line 1213 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1219 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1229 "lev_comp.y"
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

  case 184:

/* Line 1455 of yacc.c  */
#line 1247 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1257 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1263 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1268 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1273 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1280 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1287 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1296 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1304 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1315 "lev_comp.y"
    {
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags, ERR, ERR,
					  (yyvsp[(5) - (11)].crd).x, (yyvsp[(5) - (11)].crd).y, (yyvsp[(7) - (11)].sze).width, (yyvsp[(7) - (11)].sze).height,
					  SPO_SUBROOM));
		      break_stmt_start();
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1331 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1338 "lev_comp.y"
    {
		      long rflags = (yyvsp[(10) - (13)].i);
		      long flmt = (long)(yyvsp[(11) - (13)].i);
		      long flt = (long)(yyvsp[(12) - (13)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = ROOM;
		      //if (flt == -1) flt = 0;

		      add_opvars(splev, "iiiiiiiiio",
				 VA_PASS10(flt, flmt, rflags,
					  (yyvsp[(7) - (13)].crd).x, (yyvsp[(7) - (13)].crd).y, (yyvsp[(5) - (13)].crd).x, (yyvsp[(5) - (13)].crd).y,
					  (yyvsp[(9) - (13)].sze).width, (yyvsp[(9) - (13)].sze).height, SPO_ROOM));
		      break_stmt_start();
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1354 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1361 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1365 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1371 "lev_comp.y"
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

  case 200:

/* Line 1455 of yacc.c  */
#line 1381 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1387 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1396 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1402 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1407 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1413 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1418 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1424 "lev_comp.y"
    {
			/* ERR means random here */
			if ((yyvsp[(7) - (10)].i) == ERR && (yyvsp[(9) - (10)].i) != ERR) {
			    lc_error("If the door wall is random, so must be its pos!");
			} else {
			    add_opvars(splev, "iiiio",
				       VA_PASS5((long)(yyvsp[(9) - (10)].i), (long)(yyvsp[(5) - (10)].i), (long)(yyvsp[(3) - (10)].i),
						(long)(yyvsp[(7) - (10)].i), SPO_ROOM_DOOR));
			}
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1435 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1449 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1453 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1463 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1470 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1478 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1483 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1488 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1493 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1498 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1503 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1508 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1516 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1523 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1531 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1547 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1551 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1557 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1565 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1571 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1579 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1587 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1592 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1598 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1604 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1610 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1616 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(0, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1631 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1636 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1641 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1646 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1651 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1656 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1661 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1666 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1671 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1677 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1682 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_PROTECTOR));
		      (yyval.i) = 0x00020000;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1687 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_MAXHP));
		      (yyval.i) = 0x00040000;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1692 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_LEVEL_ADJUSTMENT));
		      (yyval.i) = 0x00080000;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1697 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_KEEP_ORIGINAL_INVENTORY));
		      (yyval.i) = 0x00100000;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1704 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1713 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1717 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (3)].map));
		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (3)].map));

		      if ((1L << (token - 1)) & (yyvsp[(3) - (3)].i))
			  lc_error("Monster seen_traps, trap '%s' listed twice.", (yyvsp[(1) - (3)].map));
                      Free((yyvsp[(1) - (3)].map));
		      (yyval.i) = ((1L << (token - 1)) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1730 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1738 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1748 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1756 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1765 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1772 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1780 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1786 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1791 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1796 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1801 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1806 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1811 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1816 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1821 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1826 "lev_comp.y"
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

  case 280:

/* Line 1455 of yacc.c  */
#line 1840 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1846 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1851 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1856 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1861 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1866 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1871 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1881 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1886 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1891 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1896 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1901 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1906 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1911 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1916 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1921 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_MYTHIC_TYPE));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1927 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1932 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1937 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x10000000;
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 1944 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 1950 "lev_comp.y"
    {
		       long dir, state = 0;

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

  case 302:

/* Line 1455 of yacc.c  */
#line 1979 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 1984 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 1992 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2002 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2010 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2017 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2024 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2034 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2044 "lev_comp.y"
    {
		      long rtyp = 0;
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

  case 312:

/* Line 1455 of yacc.c  */
#line 2060 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2070 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2074 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2080 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2086 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2092 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2096 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2102 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2108 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2115 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2123 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2129 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2134 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2139 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2144 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2149 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2155 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2161 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2166 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2176 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2181 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2186 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2196 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2202 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2208 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2213 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2220 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2227 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2(-1, SPO_TERRAIN));
		 ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2231 "lev_comp.y"
    {
		     add_opvars(splev, "io", VA_PASS2((yyvsp[(7) - (7)].i), SPO_TERRAIN));
		 ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2237 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2243 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2249 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2254 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2261 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2267 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2277 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2283 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (11)].i);
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      //if (flmt == -1) flmt = 0;
		      //if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiio",
				 VA_PASS6((long)(yyvsp[(5) - (11)].i), rt, rflags, flmt, flt, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2302 "lev_comp.y"
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

  case 355:

/* Line 1455 of yacc.c  */
#line 2314 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2318 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2324 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, 0, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2329 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4(0, (long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2334 "lev_comp.y"
    {
		      add_opvars(splev, "Miiio",
				 VA_PASS5(-1, (long)(yyvsp[(9) - (9)].i), (long)(yyvsp[(7) - (9)].i), (long)(yyvsp[(5) - (9)].i), SPO_ALTAR));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2339 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((long)(yyvsp[(9) - (13)].i), (long)(yyvsp[(7) - (13)].i), (long)(yyvsp[(5) - (13)].i), SPO_ALTAR));
		  ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2349 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2355 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, (int)(yyvsp[(11) - (11)].i), (int)(yyvsp[(9) - (11)].i), (int)(yyvsp[(7) - (11)].i), (int)(yyvsp[(5) - (11)].i), SPO_DECORATION));
		  ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2359 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6((int)(yyvsp[(13) - (13)].i), (int)(yyvsp[(11) - (13)].i), (int)(yyvsp[(9) - (13)].i), (int)(yyvsp[(7) - (13)].i), (int)(yyvsp[(5) - (13)].i), SPO_DECORATION));
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2363 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiio", VA_PASS6(1, 1, (int)(yyvsp[(7) - (7)].i), 0, (int)(yyvsp[(5) - (7)].i), SPO_DECORATION));
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2369 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2375 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_SUBTYPE));
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2379 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2385 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_CARPET));
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2391 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4((int)(yyvsp[(9) - (9)].i), (int)(yyvsp[(7) - (9)].i), (int)(yyvsp[(5) - (9)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2395 "lev_comp.y"
    {
		      add_opvars(splev, "iiio", VA_PASS4(0, (int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_CARPET_PIECE));
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2401 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2405 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2411 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2415 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2420 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2427 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2431 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2442 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2447 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(-1, (int)(yyvsp[(5) - (5)].i), SPO_TREE));
		  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2458 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((int)(yyvsp[(5) - (5)].i), -1, SPO_TREE));
		  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 2463 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(-1, -1, SPO_TREE));
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2471 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2477 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2484 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2488 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2495 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2506 "lev_comp.y"
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

  case 393:

/* Line 1455 of yacc.c  */
#line 2519 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2523 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2529 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2533 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2540 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2544 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2548 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2554 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2558 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2564 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2570 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2574 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2580 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2586 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2591 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2608 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2616 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2635 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2640 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2658 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 2664 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2668 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 2672 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2679 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2689 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2696 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2700 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2706 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2710 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2718 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2722 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2729 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2739 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2752 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2763 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2773 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2782 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2793 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2797 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2804 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2814 "lev_comp.y"
    {
                      long m = get_monster_id((yyvsp[(1) - (1)].map), (char)0);
                      if (m == ERR) {
                          lc_error("Unknown monster \"%s\"!", (yyvsp[(1) - (1)].map));
                          (yyval.i) = -1;
                      } else
                          (yyval.i) = SP_MONST_PACK(m,
                                         def_monsyms[(int) mons[m].mlet].sym);
                      Free((yyvsp[(1) - (1)].map));
                  ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2825 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2834 "lev_comp.y"
    {
                      long m = get_monster_id((yyvsp[(4) - (5)].map), (char) (yyvsp[(2) - (5)].i));
                      if (m == ERR) {
                          lc_error("Unknown monster ('%c', \"%s\")!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].map));
                          (yyval.i) = -1;
                      } else
                          (yyval.i) = SP_MONST_PACK(m, (yyvsp[(2) - (5)].i));
                      Free((yyvsp[(4) - (5)].map));
                  ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2844 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2850 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2854 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2861 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2871 "lev_comp.y"
    {
		      long m = get_object_id((yyvsp[(1) - (1)].map), (char)0);
		      if (m == ERR) {
			  lc_error("Unknown object \"%s\"!", (yyvsp[(1) - (1)].map));
			  (yyval.i) = -1;
		      } else
                          /* obj class != 0 to force generation of a specific item */
                          (yyval.i) = SP_OBJ_PACK(m, 1);
                      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2882 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2891 "lev_comp.y"
    {
		      long m = get_object_id((yyvsp[(4) - (5)].map), (char) (yyvsp[(2) - (5)].i));
		      if (m == ERR) {
			  lc_error("Unknown object ('%c', \"%s\")!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].map));
			  (yyval.i) = -1;
		      } else
			  (yyval.i) = SP_OBJ_PACK(m, (yyvsp[(2) - (5)].i));
                      Free((yyvsp[(4) - (5)].map));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2901 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2905 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2911 "lev_comp.y"
    { ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2913 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2919 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2923 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2927 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2931 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2939 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2948 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2952 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2956 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2960 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2964 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2967 "lev_comp.y"
    { ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2971 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2978 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2987 "lev_comp.y"
    {
		      struct lc_funcdefs_parm *tmp = New(struct lc_funcdefs_parm);

		      if (!curr_function) {
			  lc_error("Function parameters outside function definition.");
		      } else if (!tmp) {
			  lc_error("Could not alloc function params.");
		      } else {
			  long vt = SPOVAR_NULL;

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

  case 478:

/* Line 1455 of yacc.c  */
#line 3028 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3032 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3039 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3046 "lev_comp.y"
    {
			      size_t len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3056 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3060 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3068 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3072 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3076 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3080 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3084 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3089 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3093 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3097 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3102 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3107 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3112 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3116 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3121 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3126 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3130 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3134 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3139 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3143 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3150 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3156 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3160 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3166 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3182 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3186 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3190 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3194 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3216 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3220 "lev_comp.y"
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

  case 525:

/* Line 1455 of yacc.c  */
#line 3246 "lev_comp.y"
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
#line 7375 "lev_comp.tab.c"
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
#line 3274 "lev_comp.y"


/*lev_comp.y*/

