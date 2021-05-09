
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

extern int FDECL(reverse_jmp_opcode, (int));

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
#line 223 "lev_comp.tab.c"

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
     FLOOR_TYPE = 349,
     FLOOR_TYPE_ID = 350,
     FLOOR_ID = 351,
     FLOOR_MAIN_TYPE = 352,
     FLOOR_MAIN_TYPE_ID = 353,
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
     EXIT_ID = 387,
     SHUFFLE_ID = 388,
     QUANTITY_ID = 389,
     BURIED_ID = 390,
     LOOP_ID = 391,
     FOR_ID = 392,
     TO_ID = 393,
     SWITCH_ID = 394,
     CASE_ID = 395,
     BREAK_ID = 396,
     DEFAULT_ID = 397,
     ERODED_ID = 398,
     TRAPPED_STATE = 399,
     RECHARGED_ID = 400,
     INVIS_ID = 401,
     GREASED_ID = 402,
     INDESTRUCTIBLE_ID = 403,
     FEMALE_ID = 404,
     WAITFORU_ID = 405,
     CANCELLED_ID = 406,
     REVIVED_ID = 407,
     AVENGE_ID = 408,
     FLEEING_ID = 409,
     BLINDED_ID = 410,
     PARALYZED_ID = 411,
     STUNNED_ID = 412,
     CONFUSED_ID = 413,
     SEENTRAPS_ID = 414,
     ALL_ID = 415,
     MONTYPE_ID = 416,
     OBJTYPE_ID = 417,
     TERTYPE_ID = 418,
     TERTYPE2_ID = 419,
     LEVER_EFFECT_TYPE = 420,
     SWITCHABLE_ID = 421,
     CONTINUOUSLY_USABLE_ID = 422,
     TARGET_ID = 423,
     TRAPTYPE_ID = 424,
     EFFECT_FLAG_ID = 425,
     GRAVE_ID = 426,
     BRAZIER_ID = 427,
     SIGNPOST_ID = 428,
     ERODEPROOF_ID = 429,
     FUNCTION_ID = 430,
     MSG_OUTPUT_TYPE = 431,
     COMPARE_TYPE = 432,
     UNKNOWN_TYPE = 433,
     rect_ID = 434,
     fillrect_ID = 435,
     line_ID = 436,
     randline_ID = 437,
     grow_ID = 438,
     selection_ID = 439,
     flood_ID = 440,
     rndcoord_ID = 441,
     circle_ID = 442,
     ellipse_ID = 443,
     filter_ID = 444,
     complement_ID = 445,
     gradient_ID = 446,
     GRADIENT_TYPE = 447,
     LIMITED = 448,
     HUMIDITY_TYPE = 449,
     STRING = 450,
     MAP_ID = 451,
     NQSTRING = 452,
     VARSTRING = 453,
     CFUNC = 454,
     CFUNC_INT = 455,
     CFUNC_STR = 456,
     CFUNC_COORD = 457,
     CFUNC_REGION = 458,
     VARSTRING_INT = 459,
     VARSTRING_INT_ARRAY = 460,
     VARSTRING_STRING = 461,
     VARSTRING_STRING_ARRAY = 462,
     VARSTRING_VAR = 463,
     VARSTRING_VAR_ARRAY = 464,
     VARSTRING_COORD = 465,
     VARSTRING_COORD_ARRAY = 466,
     VARSTRING_REGION = 467,
     VARSTRING_REGION_ARRAY = 468,
     VARSTRING_MAPCHAR = 469,
     VARSTRING_MAPCHAR_ARRAY = 470,
     VARSTRING_MONST = 471,
     VARSTRING_MONST_ARRAY = 472,
     VARSTRING_OBJ = 473,
     VARSTRING_OBJ_ARRAY = 474,
     VARSTRING_SEL = 475,
     VARSTRING_SEL_ARRAY = 476,
     METHOD_INT = 477,
     METHOD_INT_ARRAY = 478,
     METHOD_STRING = 479,
     METHOD_STRING_ARRAY = 480,
     METHOD_VAR = 481,
     METHOD_VAR_ARRAY = 482,
     METHOD_COORD = 483,
     METHOD_COORD_ARRAY = 484,
     METHOD_REGION = 485,
     METHOD_REGION_ARRAY = 486,
     METHOD_MAPCHAR = 487,
     METHOD_MAPCHAR_ARRAY = 488,
     METHOD_MONST = 489,
     METHOD_MONST_ARRAY = 490,
     METHOD_OBJ = 491,
     METHOD_OBJ_ARRAY = 492,
     METHOD_SEL = 493,
     METHOD_SEL_ARRAY = 494,
     DICE = 495
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 538 "lev_comp.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 550 "lev_comp.tab.c"

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
#define YYLAST   1265

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  258
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  185
/* YYNRULES -- Number of rules.  */
#define YYNRULES  496
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1101

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   495

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   253,   257,     2,
     197,   198,   251,   249,   195,   250,   255,   252,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   196,     2,
       2,   254,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   199,     2,   200,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   201,   256,   202,     2,     2,     2,     2,
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
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    19,    25,
      31,    37,    41,    58,    62,    66,    67,    70,    71,    74,
      75,    78,    80,    82,    83,    87,    91,    93,    94,    97,
     101,   103,   105,   107,   109,   111,   113,   115,   117,   119,
     121,   123,   125,   127,   129,   131,   133,   135,   137,   139,
     141,   143,   145,   147,   149,   151,   153,   155,   157,   159,
     161,   163,   165,   167,   169,   171,   173,   175,   177,   179,
     181,   183,   185,   187,   189,   191,   193,   195,   197,   199,
     201,   203,   205,   207,   209,   211,   213,   215,   217,   219,
     221,   223,   225,   227,   229,   231,   233,   235,   237,   239,
     241,   243,   245,   247,   249,   251,   253,   255,   257,   259,
     261,   263,   265,   269,   273,   279,   283,   289,   295,   301,
     305,   309,   315,   321,   327,   335,   343,   351,   357,   359,
     363,   365,   369,   371,   375,   377,   381,   383,   387,   389,
     393,   395,   399,   400,   401,   410,   415,   417,   418,   420,
     422,   428,   432,   433,   434,   444,   445,   448,   449,   455,
     456,   461,   463,   466,   468,   475,   476,   480,   481,   488,
     489,   494,   495,   500,   502,   503,   508,   512,   514,   518,
     522,   528,   534,   542,   547,   548,   562,   563,   579,   580,
     583,   589,   591,   597,   599,   605,   607,   613,   615,   626,
     633,   635,   637,   639,   641,   643,   647,   649,   651,   652,
     656,   660,   664,   668,   670,   672,   674,   676,   678,   686,
     692,   694,   696,   698,   700,   704,   705,   711,   716,   717,
     721,   723,   725,   727,   729,   732,   734,   736,   738,   740,
     742,   746,   750,   754,   756,   758,   762,   764,   766,   768,
     772,   776,   777,   783,   786,   787,   791,   793,   797,   799,
     803,   807,   809,   811,   815,   817,   819,   821,   825,   827,
     829,   831,   835,   839,   843,   847,   851,   855,   859,   861,
     863,   865,   869,   873,   877,   883,   891,   897,   906,   908,
     912,   916,   922,   928,   936,   944,   951,   957,   958,   961,
     967,   971,   983,   997,  1004,  1005,  1009,  1011,  1015,  1019,
    1023,  1027,  1029,  1031,  1035,  1039,  1043,  1047,  1051,  1055,
    1057,  1059,  1061,  1063,  1067,  1071,  1073,  1079,  1089,  1095,
    1099,  1103,  1109,  1119,  1125,  1131,  1137,  1138,  1152,  1153,
    1155,  1163,  1175,  1179,  1187,  1195,  1201,  1207,  1217,  1223,
    1229,  1233,  1241,  1245,  1253,  1261,  1265,  1271,  1279,  1289,
    1291,  1293,  1295,  1297,  1299,  1300,  1303,  1305,  1309,  1311,
    1313,  1315,  1316,  1320,  1322,  1323,  1327,  1329,  1330,  1334,
    1336,  1338,  1340,  1342,  1344,  1346,  1348,  1350,  1352,  1356,
    1358,  1360,  1365,  1367,  1369,  1374,  1376,  1378,  1383,  1385,
    1390,  1396,  1398,  1402,  1404,  1408,  1410,  1412,  1417,  1427,
    1429,  1431,  1436,  1438,  1444,  1446,  1448,  1453,  1455,  1457,
    1463,  1465,  1467,  1469,  1474,  1476,  1478,  1484,  1486,  1488,
    1490,  1494,  1496,  1498,  1502,  1504,  1509,  1513,  1517,  1521,
    1525,  1529,  1533,  1535,  1537,  1541,  1543,  1547,  1548,  1550,
    1552,  1554,  1556,  1560,  1561,  1563,  1565,  1568,  1571,  1576,
    1583,  1588,  1595,  1602,  1609,  1616,  1619,  1626,  1635,  1644,
    1655,  1670,  1673,  1675,  1679,  1681,  1685,  1687,  1689,  1691,
    1693,  1695,  1697,  1699,  1701,  1703,  1705,  1707,  1709,  1711,
    1713,  1715,  1717,  1719,  1721,  1723,  1734
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     259,     0,    -1,    -1,   260,    -1,   261,    -1,   261,   260,
      -1,   262,   270,   272,    -1,    16,   196,   203,    -1,    15,
     196,   203,   195,     3,    -1,    17,   196,    11,   195,   370,
      -1,    17,   196,    10,   195,     3,    -1,    17,   196,    13,
      -1,    17,   196,    12,   195,     3,   195,     3,   195,     5,
     195,     5,   195,   404,   195,   269,   268,    -1,    18,   196,
     434,    -1,    21,   196,    97,    -1,    -1,   195,   193,    -1,
      -1,   195,   411,    -1,    -1,   195,     3,    -1,     5,    -1,
      79,    -1,    -1,   115,   196,   271,    -1,   116,   195,   271,
      -1,   116,    -1,    -1,   274,   272,    -1,   201,   272,   202,
      -1,   315,    -1,   263,    -1,   264,    -1,   378,    -1,   265,
      -1,   385,    -1,   384,    -1,   382,    -1,   383,    -1,   386,
      -1,   387,    -1,   388,    -1,   389,    -1,   359,    -1,   317,
      -1,   280,    -1,   279,    -1,   373,    -1,   329,    -1,   351,
      -1,   391,    -1,   392,    -1,   361,    -1,   362,    -1,   363,
      -1,   390,    -1,   295,    -1,   305,    -1,   307,    -1,   311,
      -1,   309,    -1,   292,    -1,   302,    -1,   288,    -1,   291,
      -1,   354,    -1,   364,    -1,   336,    -1,   352,    -1,   339,
      -1,   345,    -1,   374,    -1,   369,    -1,   357,    -1,   316,
      -1,   375,    -1,   376,    -1,   377,    -1,   379,    -1,   322,
      -1,   320,    -1,   368,    -1,   372,    -1,   371,    -1,   355,
      -1,   356,    -1,   358,    -1,   350,    -1,   353,    -1,   213,
      -1,   215,    -1,   217,    -1,   219,    -1,   221,    -1,   223,
      -1,   225,    -1,   227,    -1,   229,    -1,   212,    -1,   214,
      -1,   216,    -1,   218,    -1,   220,    -1,   222,    -1,   224,
      -1,   226,    -1,   228,    -1,   275,    -1,   276,    -1,   206,
      -1,   206,    -1,   276,    -1,   133,   196,   275,    -1,   277,
     254,   423,    -1,   277,   254,   184,   196,   432,    -1,   277,
     254,   422,    -1,   277,   254,   439,   196,   416,    -1,   277,
     254,   438,   196,   418,    -1,   277,   254,   437,   196,   420,
      -1,   277,   254,   411,    -1,   277,   254,   414,    -1,   277,
     254,   201,   286,   202,    -1,   277,   254,   201,   285,   202,
      -1,   277,   254,   201,   284,   202,    -1,   277,   254,   439,
     196,   201,   283,   202,    -1,   277,   254,   438,   196,   201,
     282,   202,    -1,   277,   254,   437,   196,   201,   281,   202,
      -1,   277,   254,   201,   287,   202,    -1,   421,    -1,   281,
     195,   421,    -1,   419,    -1,   282,   195,   419,    -1,   417,
      -1,   283,   195,   417,    -1,   415,    -1,   284,   195,   415,
      -1,   412,    -1,   285,   195,   412,    -1,   423,    -1,   286,
     195,   423,    -1,   422,    -1,   287,   195,   422,    -1,    -1,
      -1,   175,   205,   197,   289,   427,   198,   290,   273,    -1,
     205,   197,   430,   198,    -1,   132,    -1,    -1,     6,    -1,
       6,    -1,   199,   423,   177,   423,   200,    -1,   199,   423,
     200,    -1,    -1,    -1,   139,   296,   199,   410,   200,   297,
     201,   298,   202,    -1,    -1,   299,   298,    -1,    -1,   140,
     435,   196,   300,   272,    -1,    -1,   142,   196,   301,   272,
      -1,   141,    -1,   255,   255,    -1,   138,    -1,   137,   278,
     254,   423,   303,   423,    -1,    -1,   304,   306,   273,    -1,
      -1,   136,   199,   410,   200,   308,   273,    -1,    -1,   294,
     196,   310,   274,    -1,    -1,   123,   294,   312,   313,    -1,
     273,    -1,    -1,   273,   314,   124,   273,    -1,    14,   196,
     422,    -1,    70,    -1,    70,   196,   435,    -1,    70,   196,
      79,    -1,    58,   196,   318,   195,   318,    -1,    58,   196,
     318,   195,   435,    -1,   197,     4,   195,    78,   195,   333,
     198,    -1,   394,   293,   195,   404,    -1,    -1,   113,   196,
     319,   195,   326,   195,   328,   395,   398,   400,   402,   321,
     273,    -1,    -1,    50,   196,   319,   195,   325,   195,   327,
     195,   328,   395,   398,   400,   402,   323,   273,    -1,    -1,
     195,     5,    -1,   197,     4,   195,     4,   198,    -1,    79,
      -1,   197,     4,   195,     4,   198,    -1,    79,    -1,   197,
     337,   195,   338,   198,    -1,    79,    -1,   197,     4,   195,
       4,   198,    -1,    79,    -1,   122,   196,   330,   195,   403,
     195,   331,   195,   333,   334,    -1,    27,   196,   403,   195,
     432,   334,    -1,     5,    -1,    79,    -1,   332,    -1,    79,
      -1,    78,    -1,    78,   256,   332,    -1,     4,    -1,    79,
      -1,    -1,   334,   195,   335,    -1,   128,   196,   129,    -1,
      75,   196,   420,    -1,   111,   196,     4,    -1,   148,    -1,
     104,    -1,   105,    -1,   120,    -1,    20,    -1,    19,   196,
     337,   195,   338,   324,   204,    -1,    19,   196,   411,   324,
     204,    -1,    83,    -1,    84,    -1,    85,    -1,    84,    -1,
      25,   196,   341,    -1,    -1,    25,   196,   341,   340,   273,
      -1,   418,   195,   411,   342,    -1,    -1,   342,   195,   343,
      -1,   422,    -1,   117,    -1,   118,    -1,   406,    -1,   121,
     422,    -1,   149,    -1,   146,    -1,   151,    -1,   152,    -1,
     153,    -1,   154,   196,   410,    -1,   155,   196,   410,    -1,
     156,   196,   410,    -1,   157,    -1,   158,    -1,   159,   196,
     344,    -1,   150,    -1,   203,    -1,   160,    -1,   203,   256,
     344,    -1,    23,   196,   347,    -1,    -1,    24,   196,   347,
     346,   273,    -1,   420,   348,    -1,    -1,   348,   195,   349,
      -1,    73,    -1,   161,   196,   418,    -1,   436,    -1,   114,
     196,   422,    -1,   134,   196,   410,    -1,   135,    -1,    72,
      -1,   143,   196,   410,    -1,   174,    -1,    71,    -1,   144,
      -1,   145,   196,   410,    -1,   146,    -1,   147,    -1,   411,
      -1,   102,   196,    99,    -1,   101,   196,   100,    -1,   103,
     196,   410,    -1,   110,   196,   410,    -1,   111,   196,   410,
      -1,   112,   196,   410,    -1,    75,   196,   420,    -1,   148,
      -1,   105,    -1,    77,    -1,   108,   196,   106,    -1,   109,
     196,   107,    -1,    57,   196,   410,    -1,    26,   196,   393,
     195,   411,    -1,    28,   196,   411,   195,    78,   195,   403,
      -1,    32,   196,   411,   195,    78,    -1,    32,   196,   411,
     195,    78,   195,     5,   268,    -1,    33,    -1,    33,   196,
     432,    -1,    33,   196,   160,    -1,    46,   196,   411,   195,
      87,    -1,    47,   196,   411,   195,    87,    -1,    47,   196,
     441,   195,   441,   195,    87,    -1,    52,   196,   441,   195,
     441,   195,   203,    -1,    53,   196,   441,   195,   441,   360,
      -1,    54,   196,   441,   195,   441,    -1,    -1,   195,    87,
      -1,    61,   196,   432,   195,    91,    -1,    62,   196,   432,
      -1,    63,   196,   411,   195,   168,   196,   411,   195,    89,
     195,    88,    -1,    63,   196,   411,   195,    64,   195,    65,
     195,    66,   195,    89,   195,    88,    -1,    76,   196,   411,
     195,   165,   365,    -1,    -1,   365,   195,   366,    -1,    88,
      -1,   161,   196,   418,    -1,   162,   196,   420,    -1,   163,
     196,   370,    -1,   164,   196,   370,    -1,   166,    -1,   167,
      -1,   168,   196,   411,    -1,   169,   196,   393,    -1,   128,
     196,   367,    -1,    95,   196,   401,    -1,   170,   196,     4,
      -1,   111,   196,     4,    -1,     4,    -1,    89,    -1,   129,
      -1,    91,    -1,    68,   196,   432,    -1,    67,   196,   432,
      -1,     3,    -1,   197,     3,   195,   404,   198,    -1,   127,
     196,   414,   195,   416,   195,   416,   195,     7,    -1,   125,
     196,   432,   195,   416,    -1,    48,   196,   414,    -1,    49,
     196,   414,    -1,    38,   196,   203,   195,    39,    -1,    38,
     196,   203,   195,    39,   195,   161,   196,   418,    -1,    35,
     196,   414,   195,    37,    -1,    36,   196,   441,   195,    37,
      -1,    22,   196,   432,   195,    93,    -1,    -1,    34,   196,
     414,   195,   404,   195,   394,   395,   398,   400,   402,   380,
     381,    -1,    -1,   273,    -1,    43,   196,   411,   195,   405,
     195,   407,    -1,    43,   196,   411,   195,   405,   195,   407,
     195,   161,   196,   418,    -1,    44,   196,   411,    -1,    44,
     196,   411,   195,   161,   196,   418,    -1,    96,   196,   432,
     195,    97,   195,    94,    -1,   119,   196,   432,   195,     4,
      -1,    45,   196,    90,   195,   411,    -1,    45,   196,    90,
     195,   411,   195,   161,   196,   418,    -1,   171,   196,   411,
     195,   422,    -1,   171,   196,   411,   195,    79,    -1,   171,
     196,   411,    -1,   172,   196,   411,   195,   130,   195,   404,
      -1,   172,   196,   411,    -1,   173,   196,   411,   195,   131,
     195,   422,    -1,   173,   196,   411,   195,   131,   195,    79,
      -1,   173,   196,   411,    -1,    59,   196,   423,   195,   411,
      -1,    60,   196,   411,   195,   440,   195,   422,    -1,    56,
     196,   410,   195,   410,   195,   410,   195,   410,    -1,    56,
      -1,   203,    -1,    79,    -1,   203,    -1,    79,    -1,    -1,
     195,   396,    -1,   397,    -1,   397,   195,   396,    -1,    40,
      -1,    41,    -1,    42,    -1,    -1,    98,   196,   399,    -1,
      97,    -1,    -1,    95,   196,   401,    -1,    94,    -1,    -1,
     161,   196,   418,    -1,    71,    -1,    79,    -1,    72,    -1,
      79,    -1,    82,    -1,   408,    -1,    79,    -1,    82,    -1,
     408,    -1,    81,   196,    79,    -1,    86,    -1,    79,    -1,
      81,   199,     4,   200,    -1,   203,    -1,   214,    -1,   215,
     199,   423,   200,    -1,   423,    -1,   412,    -1,   186,   197,
     432,   198,    -1,   218,    -1,   219,   199,   423,   200,    -1,
     197,     4,   195,     4,   198,    -1,    79,    -1,    80,   413,
     200,    -1,   194,    -1,   194,   195,   413,    -1,   415,    -1,
     220,    -1,   221,   199,   423,   200,    -1,   197,     4,   195,
       4,   195,     4,   195,     4,   198,    -1,   417,    -1,   222,
      -1,   223,   199,   423,   200,    -1,     3,    -1,   197,     3,
     195,   404,   198,    -1,   419,    -1,   224,    -1,   225,   199,
     423,   200,    -1,   203,    -1,     3,    -1,   197,     3,   195,
     203,   198,    -1,    79,    -1,   421,    -1,   226,    -1,   227,
     199,   423,   200,    -1,   203,    -1,     3,    -1,   197,     3,
     195,   203,   198,    -1,    79,    -1,    92,    -1,   409,    -1,
     422,   255,   409,    -1,     4,    -1,   433,    -1,   197,     8,
     198,    -1,   212,    -1,   213,   199,   423,   200,    -1,   423,
     249,   423,    -1,   423,   250,   423,    -1,   423,   251,   423,
      -1,   423,   252,   423,    -1,   423,   253,   423,    -1,   197,
     423,   198,    -1,   208,    -1,   209,    -1,   277,   196,   424,
      -1,   425,    -1,   426,   195,   425,    -1,    -1,   426,    -1,
     423,    -1,   422,    -1,   428,    -1,   429,   195,   428,    -1,
      -1,   429,    -1,   411,    -1,   179,   414,    -1,   180,   414,
      -1,   181,   411,   195,   411,    -1,   182,   411,   195,   411,
     195,   423,    -1,   183,   197,   432,   198,    -1,   183,   197,
     332,   195,   432,   198,    -1,   189,   197,     7,   195,   432,
     198,    -1,   189,   197,   432,   195,   432,   198,    -1,   189,
     197,   416,   195,   432,   198,    -1,   185,   411,    -1,   187,
     197,   411,   195,   423,   198,    -1,   187,   197,   411,   195,
     423,   195,    40,   198,    -1,   188,   197,   411,   195,   423,
     195,   423,   198,    -1,   188,   197,   411,   195,   423,   195,
     423,   195,    40,   198,    -1,   191,   197,   192,   195,   197,
     423,   250,   423,   266,   198,   195,   411,   267,   198,    -1,
     190,   431,    -1,   228,    -1,   197,   432,   198,    -1,   431,
      -1,   431,   257,   432,    -1,   248,    -1,    93,    -1,     4,
      -1,     8,    -1,     9,    -1,     4,    -1,     8,    -1,     9,
      -1,     4,    -1,   433,    -1,    29,    -1,    23,    -1,    30,
      -1,    25,    -1,    31,    -1,   125,    -1,    74,    -1,    79,
      -1,   442,    -1,    55,   197,     4,   195,     4,   195,     4,
     195,     4,   198,    -1,   197,     4,   195,     4,   195,     4,
     195,     4,   198,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   284,   284,   285,   288,   289,   292,   315,   320,   342,
     355,   367,   373,   402,   408,   415,   418,   425,   429,   436,
     439,   446,   447,   451,   454,   461,   465,   472,   475,   481,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   570,   571,   572,
     575,   576,   579,   595,   601,   607,   613,   619,   625,   631,
     637,   643,   653,   663,   673,   683,   693,   703,   715,   720,
     727,   732,   739,   744,   751,   755,   761,   766,   773,   777,
     783,   787,   794,   816,   793,   830,   885,   892,   895,   901,
     908,   912,   921,   925,   920,   988,   989,   993,   992,  1006,
    1005,  1020,  1030,  1031,  1034,  1072,  1071,  1106,  1105,  1136,
    1135,  1168,  1167,  1193,  1204,  1203,  1231,  1237,  1242,  1247,
    1254,  1261,  1270,  1278,  1290,  1289,  1313,  1312,  1336,  1339,
    1345,  1355,  1361,  1370,  1376,  1381,  1387,  1392,  1398,  1409,
    1415,  1416,  1419,  1420,  1423,  1427,  1433,  1434,  1438,  1444,
    1452,  1457,  1462,  1467,  1472,  1477,  1482,  1490,  1497,  1505,
    1513,  1514,  1517,  1518,  1521,  1526,  1525,  1539,  1546,  1553,
    1561,  1566,  1572,  1578,  1584,  1590,  1595,  1600,  1605,  1610,
    1615,  1620,  1625,  1630,  1635,  1640,  1646,  1653,  1662,  1666,
    1679,  1688,  1687,  1705,  1715,  1721,  1729,  1735,  1740,  1745,
    1750,  1755,  1760,  1765,  1770,  1775,  1789,  1795,  1800,  1805,
    1810,  1815,  1820,  1825,  1830,  1835,  1840,  1845,  1850,  1855,
    1860,  1865,  1870,  1875,  1882,  1888,  1917,  1922,  1930,  1936,
    1940,  1948,  1955,  1962,  1972,  1982,  1997,  2008,  2011,  2017,
    2023,  2029,  2033,  2039,  2046,  2052,  2060,  2066,  2071,  2076,
    2081,  2086,  2092,  2098,  2103,  2108,  2113,  2118,  2123,  2130,
    2130,  2130,  2130,  2133,  2139,  2145,  2150,  2157,  2164,  2170,
    2176,  2182,  2187,  2194,  2200,  2210,  2217,  2216,  2248,  2251,
    2257,  2262,  2269,  2273,  2279,  2285,  2291,  2295,  2301,  2305,
    2310,  2317,  2321,  2328,  2332,  2337,  2344,  2350,  2357,  2361,
    2368,  2376,  2379,  2389,  2393,  2396,  2402,  2406,  2413,  2417,
    2421,  2428,  2431,  2437,  2444,  2447,  2453,  2460,  2464,  2471,
    2472,  2475,  2476,  2479,  2480,  2481,  2487,  2488,  2489,  2495,
    2496,  2499,  2508,  2513,  2520,  2531,  2537,  2541,  2545,  2552,
    2562,  2569,  2573,  2579,  2583,  2591,  2595,  2602,  2612,  2625,
    2629,  2636,  2646,  2655,  2666,  2670,  2677,  2687,  2698,  2707,
    2717,  2723,  2727,  2734,  2744,  2755,  2764,  2774,  2778,  2785,
    2786,  2792,  2796,  2800,  2804,  2812,  2821,  2825,  2829,  2833,
    2837,  2841,  2844,  2851,  2860,  2893,  2894,  2897,  2898,  2901,
    2905,  2912,  2919,  2930,  2933,  2941,  2945,  2949,  2953,  2957,
    2962,  2966,  2970,  2975,  2980,  2985,  2989,  2994,  2999,  3003,
    3007,  3012,  3016,  3023,  3029,  3033,  3039,  3046,  3047,  3050,
    3051,  3052,  3055,  3059,  3063,  3067,  3073,  3074,  3077,  3078,
    3081,  3082,  3085,  3086,  3089,  3093,  3119
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
  "MONSTER_ID", "TRAP_ID", "DOOR_ID", "DRAWBRIDGE_ID", "object_ID",
  "monster_ID", "terrain_ID", "MAZEWALK_ID", "WALLIFY_ID", "REGION_ID",
  "SPECIAL_REGION_ID", "SPECIAL_LEVREGION_ID", "SPECIAL_REGION_TYPE",
  "NAMING_ID", "NAMING_TYPE", "FILLING", "IRREGULAR", "JOINED", "ALTAR_ID",
  "ANVIL_ID", "NPC_ID", "LADDER_ID", "STAIR_ID", "NON_DIGGABLE_ID",
  "NON_PASSWALL_ID", "ROOM_ID", "ARTIFACT_NAME_ID", "PORTAL_ID",
  "TELEPRT_ID", "BRANCH_ID", "LEV", "MINERALIZE_ID", "AGE_ID",
  "CORRIDOR_ID", "GOLD_ID", "ENGRAVING_ID", "FOUNTAIN_ID", "THRONE_ID",
  "MODRON_PORTAL_ID", "LEVEL_TELEPORTER_ID",
  "LEVEL_TELEPORT_DIRECTION_TYPE", "LEVEL_TELEPORT_END_TYPE", "POOL_ID",
  "SINK_ID", "NONE", "RAND_CORRIDOR_ID", "DOOR_STATE", "LIGHT_STATE",
  "CURSE_TYPE", "ENGRAVING_TYPE", "KEYTYPE_ID", "LEVER_ID", "NO_PICKUP_ID",
  "DIRECTION", "RANDOM_TYPE", "RANDOM_TYPE_BRACKET", "A_REGISTER",
  "ALIGNMENT", "LEFT_OR_RIGHT", "CENTER", "TOP_OR_BOT", "ALTAR_TYPE",
  "UP_OR_DOWN", "ACTIVE_OR_INACTIVE", "MODRON_PORTAL_TYPE", "NPC_TYPE",
  "FOUNTAIN_TYPE", "SPECIAL_OBJECT_TYPE", "CMAP_TYPE", "FLOOR_TYPE",
  "FLOOR_TYPE_ID", "FLOOR_ID", "FLOOR_MAIN_TYPE", "FLOOR_MAIN_TYPE_ID",
  "ELEMENTAL_ENCHANTMENT_TYPE", "EXCEPTIONALITY_TYPE", "EXCEPTIONALITY_ID",
  "ELEMENTAL_ENCHANTMENT_ID", "ENCHANTMENT_ID", "SECRET_DOOR_ID",
  "USES_UP_KEY_ID", "MYTHIC_PREFIX_TYPE", "MYTHIC_SUFFIX_TYPE",
  "MYTHIC_PREFIX_ID", "MYTHIC_SUFFIX_ID", "CHARGES_ID",
  "SPECIAL_QUALITY_ID", "SPEFLAGS_ID", "SUBROOM_ID", "NAME_ID", "FLAGS_ID",
  "FLAG_TYPE", "MON_ATTITUDE", "MON_ALERTNESS", "SUBTYPE_ID",
  "NON_PASSDOOR_ID", "MON_APPEARANCE", "ROOMDOOR_ID", "IF_ID", "ELSE_ID",
  "TERRAIN_ID", "HORIZ_OR_VERT", "REPLACE_TERRAIN_ID",
  "LOCATION_SUBTYPE_ID", "DOOR_SUBTYPE", "BRAZIER_SUBTYPE",
  "SIGNPOST_SUBTYPE", "EXIT_ID", "SHUFFLE_ID", "QUANTITY_ID", "BURIED_ID",
  "LOOP_ID", "FOR_ID", "TO_ID", "SWITCH_ID", "CASE_ID", "BREAK_ID",
  "DEFAULT_ID", "ERODED_ID", "TRAPPED_STATE", "RECHARGED_ID", "INVIS_ID",
  "GREASED_ID", "INDESTRUCTIBLE_ID", "FEMALE_ID", "WAITFORU_ID",
  "CANCELLED_ID", "REVIVED_ID", "AVENGE_ID", "FLEEING_ID", "BLINDED_ID",
  "PARALYZED_ID", "STUNNED_ID", "CONFUSED_ID", "SEENTRAPS_ID", "ALL_ID",
  "MONTYPE_ID", "OBJTYPE_ID", "TERTYPE_ID", "TERTYPE2_ID",
  "LEVER_EFFECT_TYPE", "SWITCHABLE_ID", "CONTINUOUSLY_USABLE_ID",
  "TARGET_ID", "TRAPTYPE_ID", "EFFECT_FLAG_ID", "GRAVE_ID", "BRAZIER_ID",
  "SIGNPOST_ID", "ERODEPROOF_ID", "FUNCTION_ID", "MSG_OUTPUT_TYPE",
  "COMPARE_TYPE", "UNKNOWN_TYPE", "rect_ID", "fillrect_ID", "line_ID",
  "randline_ID", "grow_ID", "selection_ID", "flood_ID", "rndcoord_ID",
  "circle_ID", "ellipse_ID", "filter_ID", "complement_ID", "gradient_ID",
  "GRADIENT_TYPE", "LIMITED", "HUMIDITY_TYPE", "','", "':'", "'('", "')'",
  "'['", "']'", "'{'", "'}'", "STRING", "MAP_ID", "NQSTRING", "VARSTRING",
  "CFUNC", "CFUNC_INT", "CFUNC_STR", "CFUNC_COORD", "CFUNC_REGION",
  "VARSTRING_INT", "VARSTRING_INT_ARRAY", "VARSTRING_STRING",
  "VARSTRING_STRING_ARRAY", "VARSTRING_VAR", "VARSTRING_VAR_ARRAY",
  "VARSTRING_COORD", "VARSTRING_COORD_ARRAY", "VARSTRING_REGION",
  "VARSTRING_REGION_ARRAY", "VARSTRING_MAPCHAR", "VARSTRING_MAPCHAR_ARRAY",
  "VARSTRING_MONST", "VARSTRING_MONST_ARRAY", "VARSTRING_OBJ",
  "VARSTRING_OBJ_ARRAY", "VARSTRING_SEL", "VARSTRING_SEL_ARRAY",
  "METHOD_INT", "METHOD_INT_ARRAY", "METHOD_STRING", "METHOD_STRING_ARRAY",
  "METHOD_VAR", "METHOD_VAR_ARRAY", "METHOD_COORD", "METHOD_COORD_ARRAY",
  "METHOD_REGION", "METHOD_REGION_ARRAY", "METHOD_MAPCHAR",
  "METHOD_MAPCHAR_ARRAY", "METHOD_MONST", "METHOD_MONST_ARRAY",
  "METHOD_OBJ", "METHOD_OBJ_ARRAY", "METHOD_SEL", "METHOD_SEL_ARRAY",
  "DICE", "'+'", "'-'", "'*'", "'/'", "'%'", "'='", "'.'", "'|'", "'&'",
  "$accept", "file", "levels", "level", "level_def", "lev_init",
  "tileset_detail", "boundary_type_detail", "opt_limited",
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
  "altar_detail", "anvil_detail", "floor_detail", "subtype_detail",
  "npc_detail", "grave_detail", "brazier_detail", "signpost_detail",
  "gold_detail", "engraving_detail", "mineralize", "trap_name",
  "room_type", "optroomregionflags", "roomregionflags", "roomregionflag",
  "optfloormaintype", "floormaintype", "optfloortype", "floortype",
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
     445,   446,   447,   448,   449,    44,    58,    40,    41,    91,
      93,   123,   125,   450,   451,   452,   453,   454,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,    43,
      45,    42,    47,    37,    61,    46,   124,    38
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   258,   259,   259,   260,   260,   261,   262,   262,   263,
     263,   263,   263,   264,   265,   266,   266,   267,   267,   268,
     268,   269,   269,   270,   270,   271,   271,   272,   272,   273,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   275,
     275,   275,   275,   275,   275,   275,   275,   275,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   277,   277,   277,
     278,   278,   279,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   281,   281,
     282,   282,   283,   283,   284,   284,   285,   285,   286,   286,
     287,   287,   289,   290,   288,   291,   292,   293,   293,   294,
     294,   294,   296,   297,   295,   298,   298,   300,   299,   301,
     299,   302,   303,   303,   304,   306,   305,   308,   307,   310,
     309,   312,   311,   313,   314,   313,   315,   316,   316,   316,
     317,   317,   318,   319,   321,   320,   323,   322,   324,   324,
     325,   325,   326,   326,   327,   327,   328,   328,   329,   329,
     330,   330,   331,   331,   332,   332,   333,   333,   334,   334,
     335,   335,   335,   335,   335,   335,   335,   336,   336,   336,
     337,   337,   338,   338,   339,   340,   339,   341,   342,   342,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   344,   344,   344,
     345,   346,   345,   347,   348,   348,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   350,   351,   352,   352,   353,   353,
     353,   354,   355,   356,   357,   358,   359,   360,   360,   361,
     362,   363,   363,   364,   365,   365,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   367,
     367,   367,   367,   368,   369,   370,   370,   371,   372,   373,
     374,   375,   375,   376,   377,   378,   380,   379,   381,   381,
     382,   382,   383,   383,   384,   385,   386,   386,   387,   387,
     387,   388,   388,   389,   389,   389,   390,   391,   392,   392,
     393,   393,   394,   394,   395,   395,   396,   396,   397,   397,
     397,   398,   398,   399,   400,   400,   401,   402,   402,   403,
     403,   404,   404,   405,   405,   405,   406,   406,   406,   407,
     407,   408,   409,   409,   409,   410,   411,   411,   411,   411,
     412,   412,   412,   413,   413,   414,   414,   414,   415,   416,
     416,   416,   417,   417,   418,   418,   418,   419,   419,   419,
     419,   420,   420,   420,   421,   421,   421,   421,   421,   422,
     422,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   424,   424,   425,   426,   426,   427,   427,   428,
     428,   429,   429,   430,   430,   431,   431,   431,   431,   431,
     431,   431,   431,   431,   431,   431,   431,   431,   431,   431,
     431,   431,   431,   431,   432,   432,   433,   434,   434,   435,
     435,   435,   436,   436,   436,   436,   437,   437,   438,   438,
     439,   439,   440,   440,   441,   441,   442
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     3,     5,     5,
       5,     3,    16,     3,     3,     0,     2,     0,     2,     0,
       2,     1,     1,     0,     3,     3,     1,     0,     2,     3,
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
       4,     0,     4,     1,     0,     4,     3,     1,     3,     3,
       5,     5,     7,     4,     0,    13,     0,    15,     0,     2,
       5,     1,     5,     1,     5,     1,     5,     1,    10,     6,
       1,     1,     1,     1,     1,     3,     1,     1,     0,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     7,     5,
       1,     1,     1,     1,     3,     0,     5,     4,     0,     3,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       3,     3,     3,     1,     1,     3,     1,     1,     1,     3,
       3,     0,     5,     2,     0,     3,     1,     3,     1,     3,
       3,     1,     1,     3,     1,     1,     1,     3,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     3,     3,     3,     5,     7,     5,     8,     1,     3,
       3,     5,     5,     7,     7,     6,     5,     0,     2,     5,
       3,    11,    13,     6,     0,     3,     1,     3,     3,     3,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     1,     3,     3,     1,     5,     9,     5,     3,
       3,     5,     9,     5,     5,     5,     0,    13,     0,     1,
       7,    11,     3,     7,     7,     5,     5,     9,     5,     5,
       3,     7,     3,     7,     7,     3,     5,     7,     9,     1,
       1,     1,     1,     1,     0,     2,     1,     3,     1,     1,
       1,     0,     3,     1,     0,     3,     1,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     4,     1,     1,     4,     1,     1,     4,     1,     4,
       5,     1,     3,     1,     3,     1,     1,     4,     9,     1,
       1,     4,     1,     5,     1,     1,     4,     1,     1,     5,
       1,     1,     1,     4,     1,     1,     5,     1,     1,     1,
       3,     1,     1,     3,     1,     4,     3,     3,     3,     3,
       3,     3,     1,     1,     3,     1,     3,     0,     1,     1,
       1,     1,     3,     0,     1,     1,     2,     2,     4,     6,
       4,     6,     6,     6,     6,     2,     6,     8,     8,    10,
      14,     2,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,    10,     9
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     0,     3,     4,    23,     0,     0,     1,
       5,     0,    27,     0,     7,     0,   149,     0,     0,     0,
       0,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   288,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   359,     0,     0,
       0,     0,     0,     0,     0,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,   146,     0,     0,     0,   152,
     161,     0,     0,     0,     0,     0,     0,   109,    98,    89,
      99,    90,   100,    91,   101,    92,   102,    93,   103,    94,
     104,    95,   105,    96,   106,    97,    31,    32,    34,     6,
      27,   107,   108,     0,    46,    45,    63,    64,    61,     0,
      56,    62,   165,    57,    58,    60,    59,    30,    74,    44,
      80,    79,    48,    67,    69,    70,    87,    49,    68,    88,
      65,    84,    85,    73,    86,    43,    52,    53,    54,    66,
      81,    72,    83,    82,    47,    71,    75,    76,    77,    33,
      78,    37,    38,    36,    35,    39,    40,    41,    42,    55,
      50,    51,     0,    26,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
       0,     0,     0,     0,   110,   111,     0,     0,     0,     0,
       0,     0,   431,     0,   434,     0,   476,     0,   432,   453,
      28,     0,   169,     0,     8,     0,   392,   393,     0,   429,
     176,     0,     0,     0,    11,   478,   477,    13,   401,     0,
     220,   221,     0,     0,   398,     0,     0,   188,   396,    14,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   472,   455,   474,     0,   425,   427,   428,     0,
     424,   422,     0,   250,   254,   421,   251,   418,   420,     0,
     417,   415,     0,   224,     0,   414,   361,   360,     0,   379,
     380,     0,     0,     0,   290,   289,     0,   406,     0,     0,
     405,     0,     0,     0,     0,   494,     0,     0,   342,     0,
       0,     0,     0,     0,   329,   330,   363,   362,     0,   147,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
     300,     0,   324,   323,   481,   479,   480,   179,   178,     0,
       0,     0,     0,   200,   201,     0,     0,     0,     0,   112,
       0,     0,     0,   350,   352,   355,   142,     0,     0,     0,
       0,   151,     0,     0,     0,     0,     0,   450,   449,   451,
     454,     0,   487,   489,   486,   488,   490,   491,     0,     0,
       0,   119,   120,   115,   113,     0,     0,     0,     0,    27,
     166,    25,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,   456,   457,     0,     0,     0,
     465,     0,     0,     0,   471,     0,     0,     0,     0,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   148,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   173,   172,     0,     0,   167,     0,
       0,     0,     0,     0,   447,   433,   441,     0,     0,   436,
     437,   438,   439,   440,     0,   145,     0,   431,     0,     0,
       0,     0,   136,   134,   140,   138,     0,     0,     0,   170,
       0,     0,   430,    10,   325,     0,     9,     0,     0,   402,
       0,     0,     0,   223,   222,   188,   189,   219,     0,     0,
     204,     0,     0,     0,     0,   412,     0,     0,   410,     0,
       0,   409,     0,     0,   473,   475,   335,     0,     0,     0,
     252,     0,     0,   226,   228,   284,   208,     0,   286,     0,
       0,   381,   382,     0,   333,     0,     0,   334,   331,   385,
       0,   383,     0,   384,     0,   346,   291,     0,   292,     0,
     191,     0,     0,     0,     0,   297,   296,     0,     0,   180,
     181,   356,   492,   493,     0,   299,     0,     0,   304,     0,
     193,     0,     0,   345,     0,     0,     0,   328,     0,     0,
     163,     0,     0,   153,   349,   348,     0,     0,     0,   445,
     448,     0,   435,   150,   452,   114,     0,     0,   123,     0,
     122,     0,   121,     0,   127,     0,   118,     0,   117,     0,
     116,    29,   394,     0,     0,   404,   397,     0,   399,     0,
     458,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   423,   484,   482,   483,     0,   265,
     262,   256,     0,   280,     0,     0,     0,   279,     0,     0,
       0,     0,     0,     0,     0,   261,     0,   266,     0,   268,
     269,   278,     0,   264,   255,   270,   485,   258,     0,   416,
     227,   199,     0,     0,     0,   407,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   183,     0,
       0,   295,     0,     0,     0,     0,     0,   303,     0,     0,
       0,     0,     0,     0,   168,   162,   164,     0,     0,     0,
       0,     0,   143,     0,   135,   137,   139,   141,     0,   128,
       0,   130,     0,   132,     0,     0,   400,   218,     0,   205,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   426,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   419,     0,     0,   285,    19,
       0,   364,     0,     0,     0,     0,   390,   389,   340,   343,
       0,   293,     0,   195,     0,     0,   294,   298,     0,     0,
     357,     0,     0,     0,   344,     0,   197,     0,   364,   203,
       0,   202,   175,     0,   155,   351,   354,   353,   442,   443,
     444,   446,     0,     0,   126,     0,   125,     0,   124,     0,
       0,   459,   461,     0,   466,     0,   462,     0,   411,   464,
     463,     0,   283,   277,   272,   271,   273,   281,   282,   274,
     275,   276,   259,   260,   263,   267,   257,     0,   386,   231,
     232,     0,   236,   235,   246,   237,   238,   239,     0,     0,
       0,   243,   244,     0,   229,   233,   387,   230,     0,   214,
     215,     0,   216,     0,   213,   209,     0,   287,     0,     0,
     371,     0,     0,     0,   391,     0,     0,     0,     0,     0,
       0,   206,   207,     0,     0,     0,   306,     0,     0,     0,
       0,     0,     0,     0,   311,   312,     0,     0,     0,   305,
       0,     0,   371,     0,     0,     0,     0,     0,   155,   144,
     129,   131,   133,   326,     0,     0,     0,   413,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,    20,     0,
     368,   369,   370,   365,   366,     0,   374,     0,     0,   332,
       0,   347,   190,     0,   364,   358,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   192,
       0,   374,   208,   327,     0,   159,   154,   156,     0,   467,
       0,   468,   437,   388,   240,   241,   242,   248,   247,   245,
     211,   212,   210,     0,     0,     0,     0,   377,     0,     0,
       0,     0,   371,     0,     0,   376,   316,   318,   319,   320,
     322,   321,   315,   307,   308,   309,   310,   313,   314,   317,
       0,   377,   198,   157,    27,     0,     0,     0,     0,     0,
     408,   367,   373,   372,     0,     0,   336,     0,   496,   341,
     194,   374,     0,   301,   196,   184,    27,   160,     0,   469,
      16,     0,   249,   375,     0,   338,   495,   377,     0,     0,
     158,     0,     0,   378,   339,   337,   186,   302,   185,     0,
      17,     0,    21,    22,    19,     0,     0,   187,    12,    18,
     470
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    96,    97,    98,  1048,  1096,
     897,  1094,    12,   164,    99,   400,   100,   101,   102,   103,
     216,   104,   105,   748,   750,   752,   498,   499,   500,   501,
     106,   484,   832,   107,   108,   458,   109,   110,   217,   737,
     937,   938,  1066,  1044,   111,   612,   112,   113,   233,   114,
     609,   115,   398,   116,   356,   475,   605,   117,   118,   119,
     336,   328,   120,  1079,   121,  1091,   414,   582,   602,   805,
     818,   122,   355,   820,   531,   913,   701,   895,   123,   256,
     525,   124,   435,   293,   700,   884,  1009,   125,   432,   283,
     431,   694,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   721,   136,   137,   138,   139,   727,   929,  1032,
     140,   141,   516,   142,   143,   144,   145,   146,   147,   148,
     149,   150,  1075,  1085,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   298,   329,   900,   963,   964,
     966,  1053,  1017,  1026,  1056,   301,   563,   572,   885,   798,
     573,   239,   333,   273,   258,   408,   309,   310,   540,   541,
     294,   295,   284,   285,   377,   334,   830,   619,   620,   621,
     379,   380,   381,   274,   426,   228,   247,   348,   697,   395,
     396,   397,   594,   314,   315
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -971
static const yytype_int16 yypact[] =
{
     155,   -93,   -60,   149,  -971,   155,    43,  -117,   -35,  -971,
    -971,   -17,   804,    23,  -971,   119,  -971,    57,    92,    97,
     134,  -971,   135,   137,   140,   141,   153,   154,   157,   169,
     170,   173,   175,   181,   185,   188,   189,   191,   192,   193,
     200,   203,   204,   206,   209,   223,   226,   229,   230,   232,
     241,   243,   244,   247,   253,   255,   257,   259,   277,   278,
     279,   282,    21,   284,   290,  -971,   291,    46,   681,  -971,
    -971,   293,   294,   310,   122,     9,    88,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
     804,  -971,  -971,    75,  -971,  -971,  -971,  -971,  -971,   311,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,   342,   288,  -971,   -84,   347,    52,   337,   273,
     905,   121,   121,    37,   -37,    10,   -14,   -14,   879,  -128,
    -128,   -24,   201,   -14,   -14,   418,   -14,   331,  -128,  -128,
     -21,   -24,   -24,   -24,     9,   312,     9,   -14,   905,   905,
     -14,   905,   905,    96,   -14,   905,   -21,   905,    62,  -971,
     905,  -128,   689,     9,  -971,  -971,   258,   314,   -14,   -14,
     -14,   317,  -971,    39,  -971,   319,  -971,    89,  -971,    98,
    -971,   194,  -971,   315,  -971,   119,  -971,  -971,   320,  -971,
     256,   326,   327,   329,  -971,  -971,  -971,  -971,  -971,   335,
    -971,  -971,   333,   529,  -971,   336,   348,   349,  -971,  -971,
    -128,  -128,   -14,   -14,   351,   -14,   354,   355,   356,   905,
     357,   313,  -971,  -971,   300,   363,  -971,  -971,  -971,   557,
    -971,  -971,   362,  -971,  -971,  -971,  -971,  -971,  -971,   559,
    -971,  -971,   366,   365,   372,  -971,  -971,  -971,   375,  -971,
    -971,   378,   379,   389,  -971,  -971,   581,  -971,   387,   392,
    -971,   395,   394,   589,   399,  -971,   401,   402,   405,   406,
     408,   595,   412,   413,  -971,  -971,  -971,  -971,   417,   608,
     421,   436,   456,   465,   213,   638,   467,   195,   468,   472,
    -971,   473,  -971,  -971,  -971,  -971,  -971,  -971,  -971,   476,
     481,   483,   484,  -971,  -971,   485,   315,   492,   493,  -971,
     489,     9,     9,   497,   498,   505,  -971,   509,   287,     9,
       9,  -971,     9,     9,     9,     9,     9,   256,   213,  -971,
     514,   513,  -971,  -971,  -971,  -971,  -971,  -971,   517,    68,
      34,  -971,  -971,   256,   213,   524,   527,   531,   804,   804,
    -971,  -971,     9,   -84,   722,    31,   730,   539,   536,   905,
     549,     9,   124,   740,   542,  -971,  -971,   553,   558,   855,
    -971,   -14,   -14,   447,  -971,   555,   573,   905,   662,   577,
       9,   578,   315,   580,     9,   315,   -14,   -14,   905,   696,
     699,   584,     9,     8,   743,   777,   587,   746,   745,   115,
     624,   -14,   704,   604,   713,   -24,   -44,  -971,   611,   -24,
     -24,   -24,     9,   613,    53,   -14,    58,   718,    -5,   646,
     720,   -30,   816,    10,   710,  -971,    36,    36,  -971,   183,
     635,   -29,   711,   712,   950,  -971,  -971,   218,   371,   -50,
     -50,  -971,  -971,  -971,    98,  -971,   905,   649,  -111,  -101,
     -85,   -77,  -971,  -971,   256,   213,    41,   131,   103,  -971,
     643,   404,  -971,  -971,  -971,   843,  -971,   660,   335,  -971,
     661,   857,   445,  -971,  -971,   349,  -971,  -971,   -14,   -14,
     612,   674,   672,   678,   680,  -971,   682,   579,  -971,   677,
     683,  -971,   684,   693,  -971,  -971,  -971,   686,   490,   538,
    -971,   687,   552,  -971,  -971,  -971,  -971,   697,   701,   887,
     563,  -971,  -971,   703,  -971,   716,   890,  -971,   724,  -971,
     714,  -971,   725,  -971,   726,   729,  -971,   917,  -971,   735,
    -971,   921,   737,     8,   744,   747,  -971,   751,   860,  -971,
    -971,  -971,  -971,  -971,   752,  -971,   753,   754,  -971,   756,
    -971,   945,   757,  -971,   758,   830,   952,  -971,   761,   315,
    -971,   702,     9,  -971,  -971,   256,   765,   766,   767,  -971,
     769,   764,  -971,  -971,  -971,  -971,   961,   770,  -971,    -6,
    -971,     9,  -971,   -84,  -971,    29,  -971,    80,  -971,    33,
    -971,  -971,  -971,   773,   966,  -971,  -971,   772,  -971,   768,
    -971,   776,   895,   905,  -971,     9,     9,   905,   779,     9,
     905,   905,   781,   782,  -971,  -971,  -971,  -971,   785,  -971,
    -971,  -971,   786,  -971,   787,   790,   791,  -971,   792,   793,
     794,   795,   796,   797,   798,  -971,   799,  -971,   800,  -971,
    -971,  -971,   801,  -971,  -971,  -971,  -971,  -971,   802,  -971,
     803,   806,    10,   994,   807,  -971,   -21,  1000,   810,   845,
    1003,    49,    37,   847,  -110,   924,   817,   -16,  -971,   811,
     926,  -971,     9,   820,   -84,   982,   -14,   853,   955,   856,
      -4,   133,   315,    36,  -971,  -971,   213,   849,     8,   -25,
      67,   950,  -971,   -86,  -971,  -971,   213,   256,   -26,  -971,
      -7,  -971,    -3,  -971,     8,   858,  -971,  -971,     9,  -971,
     859,   466,   208,   865,     8,   633,   873,   874,     9,  -971,
       9,   121,   954,   956,     9,   969,   949,     9,     9,     9,
     -84,     9,     9,     9,    37,  -971,   996,   377,  -971,   884,
    1076,   886,   894,  1078,   903,   900,  -971,  -971,   906,  -971,
     907,  -971,  1100,  -971,   231,   910,  -971,  -971,   911,    86,
     256,   913,   914,   626,  -971,  1106,  -971,  1107,   886,  -971,
     920,  -971,  -971,   923,   -43,  -971,  -971,   256,  -971,  -971,
    -971,  -971,   315,    29,  -971,    80,  -971,    33,  -971,   918,
    1114,   213,  -971,  1072,  -971,     9,  -971,   922,  -971,  -971,
    -971,   453,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,   256,  -971,  -971,  -971,  -971,   -42,  -971,  -971,
    -971,   -84,  -971,  -971,  -971,  -971,  -971,  -971,   925,   929,
     930,  -971,  -971,   931,  -971,  -971,  -971,   256,   932,  -971,
    -971,   933,  -971,   934,  -971,  -971,  1119,  -971,   936,   174,
    1034,  1130,   940,    37,  -971,   975,    37,   939,   943,    -4,
       9,  -971,  -971,   941,  1074,  1052,  -971,   947,   948,   962,
     963,   964,   965,   984,  -971,  -971,   985,   986,   987,  -971,
     959,   989,  1034,    86,  1178,   118,   990,   988,   -43,  -971,
    -971,  -971,  -971,  -971,   992,   991,   479,  -971,     9,  1109,
     256,     9,     9,     9,   -92,   121,  1187,  1063,  -971,  1189,
    -971,  -971,  -971,  -971,   999,  1001,  1101,  1005,  1191,  -971,
    1002,  -971,  -971,   124,   886,  -971,  -971,  1006,  1007,  1110,
    1199,    51,    37,   121,    31,    31,   -14,   -37,  1201,  -971,
    1202,  1101,  -971,  -971,  1011,  -971,  -971,  -971,  1203,  -971,
    1169,  -971,   178,  -971,  -971,  -971,  -971,  -971,   957,  -971,
    -971,  -971,  -971,  1014,   174,  1117,  1019,  1055,  1213,  1020,
      37,  1021,  1034,  1131,  1133,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    1024,  1055,   806,  -971,   804,  1028,  1026,  1032,  1029,   -92,
    -971,  -971,  -971,  -971,  1110,  1030,  -971,  1031,  -971,  -971,
    -971,  1101,  1033,  -971,  -971,  -971,   804,  -971,     8,  -971,
    -971,  1035,  -971,  -971,    37,   315,  -971,  1055,  1143,   315,
    -971,  1037,   -14,  -971,  -971,  -971,  -971,  -971,  -971,    72,
    1038,   315,  -971,  -971,   884,   -14,  1036,  -971,  -971,  -971,
    -971
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -971,  -971,  1230,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
     142,  -971,  -971,  1004,  -100,  -353,   839,  1039,  1170,  -463,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  1179,  -971,  -971,  -971,
     302,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
     778,  1040,  -971,  -971,  -971,  -971,   719,  -971,  -971,  -971,
     334,  -971,  -971,  -971,  -592,   316,   260,  -971,  -971,   441,
     274,  -971,  -971,  -971,  -971,  -971,   199,  -971,  -971,  1081,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -688,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,  -971,
    -971,  -971,  -971,  -971,  -971,   263,   548,  -790,   242,  -971,
    -884,  -971,  -913,   205,  -970,  -461,  -577,  -971,  -971,  -971,
     469,   854,  -202,  -167,  -365,   742,   163,  -364,  -462,  -610,
    -505,  -596,  -499,  -590,  -161,   -67,  -971,   520,  -971,  -971,
     771,  -971,  -971,   993,   -55,   709,  -971,  -459,  -971,  -971,
    -971,  -971,  -971,  -169,  -971
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -226
static const yytype_int16 yytable[] =
{
     230,   257,   638,   474,   240,   590,   718,   636,   227,   302,
     303,   360,   604,   222,   607,   608,   317,   318,   323,   320,
     322,   618,   330,   331,   332,   502,   503,    16,   932,   753,
     338,   312,   276,   341,   514,   580,   535,   349,   222,   535,
     287,   751,   296,   222,   276,   749,   640,   367,   991,   600,
     614,   363,   364,   365,   826,  1028,   245,   344,   326,   596,
     759,   345,   346,   803,   391,   248,   249,   353,  1007,   306,
     393,  1065,   497,   248,   249,   816,   367,  1092,  1041,   550,
     561,   299,   553,   287,   627,   793,    13,   562,   756,   300,
     911,   628,   307,   308,   629,   417,   418,   935,   420,   936,
     344,   630,   222,     7,   345,   346,   535,  1086,   277,   790,
     631,  1008,   756,   248,   249,   275,   288,   632,   633,   236,
     277,   278,   344,   305,   276,   634,   345,   346,   796,   337,
     237,   238,   592,   278,   287,   797,     8,   593,  1061,   821,
    1029,   354,  1030,   339,   340,   246,   342,   343,  1077,     9,
     350,  1093,   352,   581,   949,   357,   368,   710,    11,   288,
     480,   825,   378,   597,   394,   912,   297,   601,    14,   833,
       1,     2,   252,   313,   236,   347,   834,   839,   236,    15,
    1031,   804,   327,   253,  1022,   237,   238,   847,   835,   237,
     238,   253,   837,   817,   569,   836,   570,   571,   222,   838,
     277,   374,   375,   376,   254,   255,   223,   799,   523,   524,
     288,   530,   819,   278,   960,   961,   962,   382,   162,   383,
      75,   224,   225,   384,   385,   386,   279,   942,   515,   504,
     606,   389,   280,   606,   289,   163,   223,   236,   279,   941,
     290,   788,   635,   940,   280,   213,   224,   225,   237,   238,
     335,   224,   225,   165,   533,   534,   734,   226,   538,   539,
     587,   291,   292,   744,   745,   223,   370,   281,   282,   554,
     555,   823,   853,   248,   249,   828,   829,   289,   618,   866,
     224,   225,   226,   290,   575,   229,   579,   226,   166,   371,
     584,   585,   586,   167,   479,   223,  1035,  1036,   591,   510,
     606,   236,   487,   488,   639,   489,   490,   491,   492,   493,
     224,   225,   237,   238,   250,   251,   226,   410,   279,   387,
     615,   610,   368,   505,   280,   538,   539,   221,   289,   231,
     168,   169,   637,   170,   290,   511,   171,   172,   372,   373,
     374,   375,   376,   311,   522,   234,   226,   281,   282,   173,
     174,   324,   325,   175,   520,   291,   292,   241,   242,   243,
     244,   650,   651,   548,   532,   176,   177,   552,   542,   178,
     259,   179,   545,  1047,   358,   560,   -15,   180,   388,   822,
     252,   181,   695,   556,   182,   183,   312,   184,   185,   186,
     465,   389,   248,   249,   392,   390,   187,   236,   969,   188,
     189,   971,   190,   845,   316,   191,   224,   225,   237,   238,
     248,   249,   254,   255,   307,   308,   248,   249,   622,   192,
     250,   251,   193,   415,   416,   194,   195,   378,   196,   374,
     375,   376,   372,   373,   374,   375,   376,   197,   611,   198,
     199,   625,   226,   200,   372,   373,   374,   375,   376,   201,
     535,   202,   888,   203,   536,   204,  1010,   372,   373,   374,
     375,   376,   372,   373,   374,   375,   376,   372,   373,   374,
     375,   376,   747,   205,   206,   207,   994,  1033,   208,   939,
     210,   889,   890,   235,  1034,   486,   211,   212,   891,   218,
     219,  1081,   260,   261,   262,   263,   264,   892,   265,   252,
     266,   267,   268,   269,   270,   893,   220,   232,   319,   335,
     271,   403,   361,   362,   366,  1059,   399,   252,   369,   402,
     808,   404,   405,   252,   406,   894,   248,   249,   321,   407,
     409,   254,   255,   410,   253,   411,   372,   373,   374,   375,
     376,   272,   665,   412,   413,   736,   666,   667,   419,   254,
     255,   421,   422,   423,   425,   254,   255,   427,   428,   812,
     429,   430,   433,   810,   746,   434,  -225,   436,   852,  1083,
     437,   623,   856,   438,   439,   859,   860,   861,   827,   863,
     864,   865,   658,   410,   440,   441,   442,   443,   761,   762,
     444,   445,   765,   446,   447,   668,   448,   449,   760,   453,
     450,   451,   763,   452,   642,   766,   767,   454,   455,   669,
     670,   671,   456,   672,   457,   673,   459,   248,   249,   862,
     372,   373,   374,   375,   376,   887,   260,   261,   262,   263,
     264,   460,   265,   252,   266,   267,   268,   269,   270,   674,
     675,   676,   463,   677,   537,   648,   678,   679,   680,   681,
     682,   461,   683,   372,   373,   374,   375,   376,   248,   249,
     462,   843,   464,   466,   844,   254,   255,   467,   468,   538,
     539,   469,   684,   685,  1000,   272,   470,  1001,   471,   472,
     473,   686,   687,   688,   689,   690,   691,   476,   477,   478,
     664,   841,   481,   482,   372,   373,   374,   375,   376,   692,
     483,   851,   372,   948,   374,   375,   376,   485,   975,   494,
     950,   495,   693,   496,   916,   372,   373,   374,   375,   376,
     506,   917,  1084,   507,   252,   513,  1088,   508,   372,   373,
     374,   375,   376,   517,   518,   253,   519,   918,  1097,   372,
     373,   374,   375,   376,   521,   526,   527,   543,   528,  1004,
    1005,  1006,   699,   529,   919,   546,   254,   255,   260,   261,
     262,   263,   264,   705,   265,   252,   266,   267,   268,   269,
     270,   544,   547,   549,   557,   551,   271,   558,   946,   559,
     564,   565,   566,   567,   568,   574,   226,   920,   921,   922,
     923,   576,   924,   925,   926,   927,   928,   254,   255,   577,
     578,   372,   373,   374,   375,   376,   583,   272,   588,   595,
      16,   598,   372,   373,   374,   375,   376,   599,    17,  1037,
     603,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,   848,  -174,   613,    30,    31,    32,    33,
      34,   616,    35,   617,   626,   641,   643,    36,    37,    38,
      39,    40,    41,    42,    43,   644,    44,    45,    46,   646,
      47,   647,    48,    49,    50,    51,    52,    53,   652,   653,
     654,    54,    55,   655,    56,   656,   659,   657,   660,   661,
      57,  1002,   372,   373,   374,   375,   376,   214,   662,   663,
     698,   704,   702,    78,   708,    80,   703,    82,   706,    84,
      58,    86,    79,    88,    81,    90,    83,    92,    85,    94,
      87,   707,    89,   710,    91,  1090,    93,    59,    95,   709,
     711,   714,   712,    60,   713,   716,    61,    62,  1099,    63,
     715,    64,   717,   530,   248,   249,    65,    66,   723,   719,
      67,    68,   720,    69,  1067,    70,   722,   724,   725,   729,
     726,   728,   730,   731,   732,   658,   733,   735,   248,   249,
     738,   739,   742,   740,   741,   743,  1080,   306,   754,   755,
     756,   758,   757,   530,   764,    71,    72,    73,   768,    74,
     769,   770,   771,   772,   248,   249,   773,   774,   775,   776,
     777,   778,   779,   780,   781,   782,   783,   784,   786,   789,
     785,   787,   790,    75,   792,   793,   794,   795,   800,    76,
      77,   801,   802,   807,   806,   809,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,   260,   261,   262,   263,   264,   304,
     265,   252,   266,   267,   268,   269,   270,   811,   813,   814,
     824,   815,   271,   840,   854,   855,   858,   842,   260,   261,
     262,   263,   264,   846,   265,   252,   266,   267,   268,   269,
     270,   849,   850,   254,   255,   857,   271,   867,   868,   896,
     898,   899,   902,   272,   260,   261,   262,   263,   264,   901,
     265,   252,   266,   267,   268,   269,   270,   254,   255,   903,
     904,   905,   271,   906,   907,   909,   910,   272,   914,   915,
     930,   931,   945,   869,   870,   933,   943,   871,   934,   944,
     947,   951,   958,   254,   255,   952,   953,   954,   955,   956,
     957,   959,   965,   272,   967,   968,   970,   972,   973,   976,
     977,   978,   872,   979,   980,   873,   874,   875,   876,   877,
     878,   879,   880,   881,   882,   883,    77,   989,   981,   982,
     983,   984,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
     985,   986,   987,   988,   990,   993,   995,   998,  1003,   999,
     996,  1011,  1012,  1013,  1014,  1019,  1016,  1015,  1020,   236,
    1018,  1023,  1024,  1027,  1025,  1039,  1040,  1043,  1045,  1046,
     237,   238,  1050,  1049,  1052,  1054,  1055,  1057,  1058,  1060,
    1062,  1063,  1064,  1068,  1069,  1070,  1074,  1071,  1078,  1076,
    1082,  1087,  1089,  1095,  1100,    10,  1098,   509,   215,   401,
     997,   209,   589,   974,   649,   908,   351,  1021,  1072,   992,
    1038,   359,  1042,   286,   791,   886,  1051,   512,   696,  1073,
     645,   831,   424,     0,     0,   624
};

static const yytype_int16 yycheck[] =
{
     100,   168,   507,   356,   165,   464,   583,   506,    75,   176,
     177,   213,   473,     4,   476,   477,   183,   184,   187,   186,
     187,   484,   191,   192,   193,   390,   390,     6,   818,   639,
     197,    55,     3,   200,     3,    79,     3,   204,     4,     3,
       3,   637,    79,     4,     3,   635,   508,     8,   932,    79,
      79,   218,   219,   220,    79,     4,     4,     4,    79,    64,
     652,     8,     9,    79,   231,    79,    80,     5,   160,   197,
     231,  1041,     4,    79,    80,    79,     8,     5,   991,   432,
      72,    71,   435,     3,   195,   195,   203,    79,   198,    79,
       4,   202,   220,   221,   195,   262,   263,   140,   265,   142,
       4,   202,     4,   196,     8,     9,     3,  1077,    79,   195,
     195,   203,   198,    79,    80,   170,    79,   202,   195,   203,
      79,    92,     4,   178,     3,   202,     8,     9,    79,   196,
     214,   215,    74,    92,     3,    86,   196,    79,  1022,   731,
      89,    79,    91,   198,   199,    93,   201,   202,  1061,     0,
     205,    79,   207,   197,   196,   210,   223,   199,   115,    79,
     362,   738,   229,   168,   231,    79,   203,   197,   203,   195,
      15,    16,   186,   197,   203,    79,   202,   754,   203,   196,
     129,   197,   203,   197,   974,   214,   215,   764,   195,   214,
     215,   197,   195,   197,    79,   202,    81,    82,     4,   202,
      79,   251,   252,   253,   218,   219,   197,   712,    84,    85,
      79,    78,    79,    92,    40,    41,    42,    23,   195,    25,
     199,   212,   213,    29,    30,    31,   197,   837,   197,   390,
     197,   197,   203,   197,   197,   116,   197,   203,   197,   835,
     203,   702,   201,   833,   203,   199,   212,   213,   214,   215,
     197,   212,   213,   196,   421,   422,   609,   248,   222,   223,
     462,   224,   225,   627,   629,   197,   177,   226,   227,   436,
     437,   733,   771,    79,    80,   208,   209,   197,   741,   784,
     212,   213,   248,   203,   451,   197,   455,   248,   196,   200,
     459,   460,   461,   196,   361,   197,   984,   985,   465,   399,
     197,   203,   369,   370,   201,   372,   373,   374,   375,   376,
     212,   213,   214,   215,    83,    84,   248,     4,   197,   125,
     481,   138,   389,   390,   203,   222,   223,   205,   197,   254,
     196,   196,   201,   196,   203,   402,   196,   196,   249,   250,
     251,   252,   253,   180,   411,     3,   248,   226,   227,   196,
     196,   188,   189,   196,   409,   224,   225,    10,    11,    12,
      13,   528,   529,   430,   419,   196,   196,   434,   423,   196,
      97,   196,   427,   195,   211,   442,   198,   196,   184,   732,
     186,   196,   549,   438,   196,   196,    55,   196,   196,   196,
     195,   197,    79,    80,   231,   201,   196,   203,   903,   196,
     196,   906,   196,   195,   203,   196,   212,   213,   214,   215,
      79,    80,   218,   219,   220,   221,    79,    80,   200,   196,
      83,    84,   196,   260,   261,   196,   196,   494,   196,   251,
     252,   253,   249,   250,   251,   252,   253,   196,   255,   196,
     196,   496,   248,   196,   249,   250,   251,   252,   253,   196,
       3,   196,    75,   196,     7,   196,   955,   249,   250,   251,
     252,   253,   249,   250,   251,   252,   253,   249,   250,   251,
     252,   253,   633,   196,   196,   196,   935,   982,   196,   832,
     196,   104,   105,   195,   983,   198,   196,   196,   111,   196,
     196,  1068,   179,   180,   181,   182,   183,   120,   185,   186,
     187,   188,   189,   190,   191,   128,   196,   196,    90,   197,
     197,   255,   254,   199,   197,  1020,   201,   186,   199,   199,
     722,   195,   195,   186,   195,   148,    79,    80,   197,   194,
     197,   218,   219,     4,   197,   199,   249,   250,   251,   252,
     253,   228,     4,   195,   195,   612,     8,     9,   197,   218,
     219,   197,   197,   197,   197,   218,   219,   257,   195,   726,
       3,   199,     3,   724,   631,   199,   201,   195,   770,  1074,
     195,   200,   774,   195,   195,   777,   778,   779,   739,   781,
     782,   783,     3,     4,   195,     4,   199,   195,   655,   656,
     195,   197,   659,     4,   195,    57,   195,   195,   653,     4,
     195,   195,   657,   195,   200,   660,   661,   195,   195,    71,
      72,    73,   195,    75,     6,    77,   195,    79,    80,   780,
     249,   250,   251,   252,   253,   786,   179,   180,   181,   182,
     183,   195,   185,   186,   187,   188,   189,   190,   191,   101,
     102,   103,     4,   105,   197,   200,   108,   109,   110,   111,
     112,   195,   114,   249,   250,   251,   252,   253,    79,    80,
     195,   195,   195,   195,   198,   218,   219,   195,   195,   222,
     223,   195,   134,   135,   195,   228,   195,   198,   195,   195,
     195,   143,   144,   145,   146,   147,   148,   195,   195,   200,
     200,   758,   195,   195,   249,   250,   251,   252,   253,   161,
     195,   768,   249,   250,   251,   252,   253,   198,   910,   195,
     871,   198,   174,   196,    88,   249,   250,   251,   252,   253,
     196,    95,  1075,   196,   186,     3,  1079,   196,   249,   250,
     251,   252,   253,     3,   195,   197,   200,   111,  1091,   249,
     250,   251,   252,   253,   195,     5,   204,   192,   195,   951,
     952,   953,   200,   195,   128,    93,   218,   219,   179,   180,
     181,   182,   183,   200,   185,   186,   187,   188,   189,   190,
     191,   198,   195,   195,    78,   195,   197,    78,   845,   195,
      37,     4,   195,    37,    39,   161,   248,   161,   162,   163,
     164,    87,   166,   167,   168,   169,   170,   218,   219,   195,
      87,   249,   250,   251,   252,   253,   195,   228,   195,    91,
       6,   165,   249,   250,   251,   252,   253,    97,    14,   986,
       4,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   200,   124,   200,    32,    33,    34,    35,
      36,   130,    38,   131,   195,   202,     3,    43,    44,    45,
      46,    47,    48,    49,    50,   195,    52,    53,    54,   198,
      56,     4,    58,    59,    60,    61,    62,    63,   256,   195,
     198,    67,    68,   195,    70,   195,   199,   195,   195,   195,
      76,   948,   249,   250,   251,   252,   253,   206,   195,   203,
     203,     4,   195,   212,     4,   214,   195,   216,   195,   218,
      96,   220,   213,   222,   215,   224,   217,   226,   219,   228,
     221,   195,   223,   199,   225,  1082,   227,   113,   229,   195,
     195,     4,   196,   119,   195,     4,   122,   123,  1095,   125,
     195,   127,   195,    78,    79,    80,   132,   133,    78,   195,
     136,   137,   195,   139,  1044,   141,   195,   195,   195,     4,
     196,   195,   195,   195,   124,     3,   195,   255,    79,    80,
     195,   195,   198,   196,   195,     4,  1066,   197,   195,     3,
     198,   195,   204,    78,   195,   171,   172,   173,   197,   175,
     198,   196,   196,   196,    79,    80,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   195,     5,
     198,   195,   195,   199,     4,   195,   161,     4,   161,   205,
     206,    87,   195,    87,   203,   195,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   179,   180,   181,   182,   183,   160,
     185,   186,   187,   188,   189,   190,   191,    65,   195,    94,
     201,   195,   197,   195,   100,    99,   107,   198,   179,   180,
     181,   182,   183,   198,   185,   186,   187,   188,   189,   190,
     191,   198,   198,   218,   219,   106,   197,    81,    82,   195,
       4,   195,     4,   228,   179,   180,   181,   182,   183,   195,
     185,   186,   187,   188,   189,   190,   191,   218,   219,   196,
     200,   195,   197,   196,     4,   195,   195,   228,   195,   195,
       4,     4,    40,   117,   118,   195,   198,   121,   195,     5,
     198,   196,     3,   218,   219,   196,   196,   196,   196,   196,
     196,   195,    98,   228,     4,   195,   161,   198,   195,   198,
      66,    89,   146,   196,   196,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   206,   198,   196,   196,
     196,   196,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     196,   196,   196,   196,   195,     7,   196,   195,    79,   198,
     202,     4,   129,     4,   195,     4,    95,   196,   196,   203,
     195,   195,   195,     4,    94,     4,     4,   196,     5,    40,
     214,   215,   198,   256,    97,   196,   161,     4,   198,   198,
      89,    88,   198,   195,   198,   193,   196,   198,   195,   198,
     195,    88,   195,   195,   198,     5,  1094,   398,    68,   235,
     938,    62,   464,   909,   525,   804,   206,   973,  1049,   933,
     987,   212,   992,   172,   706,   786,  1014,   403,   549,  1054,
     518,   741,   269,    -1,    -1,   494
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    15,    16,   259,   260,   261,   262,   196,   196,     0,
     260,   115,   270,   203,   203,   196,     6,    14,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      32,    33,    34,    35,    36,    38,    43,    44,    45,    46,
      47,    48,    49,    50,    52,    53,    54,    56,    58,    59,
      60,    61,    62,    63,    67,    68,    70,    76,    96,   113,
     119,   122,   123,   125,   127,   132,   133,   136,   137,   139,
     141,   171,   172,   173,   175,   199,   205,   206,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   263,   264,   265,   272,
     274,   275,   276,   277,   279,   280,   288,   291,   292,   294,
     295,   302,   304,   305,   307,   309,   311,   315,   316,   317,
     320,   322,   329,   336,   339,   345,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   361,   362,   363,   364,
     368,   369,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   195,   116,   271,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   294,
     196,   196,   196,   199,   206,   276,   278,   296,   196,   196,
     196,   205,     4,   197,   212,   213,   248,   423,   433,   197,
     272,   254,   196,   306,     3,   195,   203,   214,   215,   409,
     422,    10,    11,    12,    13,     4,    93,   434,    79,    80,
      83,    84,   186,   197,   218,   219,   337,   411,   412,    97,
     179,   180,   181,   182,   183,   185,   187,   188,   189,   190,
     191,   197,   228,   411,   431,   432,     3,    79,    92,   197,
     203,   226,   227,   347,   420,   421,   347,     3,    79,   197,
     203,   224,   225,   341,   418,   419,    79,   203,   393,    71,
      79,   403,   411,   411,   160,   432,   197,   220,   221,   414,
     415,   414,    55,   197,   441,   442,   203,   411,   411,    90,
     411,   197,   411,   441,   414,   414,    79,   203,   319,   394,
     441,   441,   441,   410,   423,   197,   318,   423,   411,   432,
     432,   411,   432,   432,     4,     8,     9,    79,   435,   411,
     432,   319,   432,     5,    79,   330,   312,   432,   414,   275,
     410,   254,   199,   411,   411,   411,   197,     8,   423,   199,
     177,   200,   249,   250,   251,   252,   253,   422,   423,   428,
     429,   430,    23,    25,    29,    30,    31,   125,   184,   197,
     201,   411,   414,   422,   423,   437,   438,   439,   310,   201,
     273,   271,   199,   255,   195,   195,   195,   194,   413,   197,
       4,   199,   195,   195,   324,   414,   414,   411,   411,   197,
     411,   197,   197,   197,   431,   197,   432,   257,   195,     3,
     199,   348,   346,     3,   199,   340,   195,   195,   195,   195,
     195,     4,   199,   195,   195,   197,     4,   195,   195,   195,
     195,   195,   195,     4,   195,   195,   195,     6,   293,   195,
     195,   195,   195,     4,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   273,   313,   195,   195,   200,   423,
     410,   195,   195,   195,   289,   198,   198,   423,   423,   423,
     423,   423,   423,   423,   195,   198,   196,     4,   284,   285,
     286,   287,   412,   415,   422,   423,   196,   196,   196,   274,
     272,   423,   409,     3,     3,   197,   370,     3,   195,   200,
     432,   195,   423,    84,    85,   338,     5,   204,   195,   195,
      78,   332,   432,   411,   411,     3,     7,   197,   222,   223,
     416,   417,   432,   192,   198,   432,    93,   195,   423,   195,
     273,   195,   423,   273,   411,   411,   432,    78,    78,   195,
     423,    72,    79,   404,    37,     4,   195,    37,    39,    79,
      81,    82,   405,   408,   161,   411,    87,   195,    87,   441,
      79,   197,   325,   195,   441,   441,   441,   410,   195,   318,
     435,   411,    74,    79,   440,    91,    64,   168,   165,    97,
      79,   197,   326,     4,   403,   314,   197,   416,   416,   308,
     138,   255,   303,   200,    79,   422,   130,   131,   277,   425,
     426,   427,   200,   200,   428,   432,   195,   195,   202,   195,
     202,   195,   202,   195,   202,   201,   420,   201,   418,   201,
     416,   202,   200,     3,   195,   413,   198,     4,   200,   324,
     411,   411,   256,   195,   198,   195,   195,   195,     3,   199,
     195,   195,   195,   203,   200,     4,     8,     9,    57,    71,
      72,    73,    75,    77,   101,   102,   103,   105,   108,   109,
     110,   111,   112,   114,   134,   135,   143,   144,   145,   146,
     147,   148,   161,   174,   349,   411,   433,   436,   203,   200,
     342,   334,   195,   195,     4,   200,   195,   195,     4,   195,
     199,   195,   196,   195,     4,   195,     4,   195,   404,   195,
     195,   360,   195,    78,   195,   195,   196,   365,   195,     4,
     195,   195,   124,   195,   273,   255,   423,   297,   195,   195,
     196,   195,   198,     4,   415,   412,   423,   422,   281,   421,
     282,   419,   283,   417,   195,     3,   198,   204,   195,   332,
     432,   423,   423,   432,   195,   423,   432,   432,   197,   198,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   198,   195,   195,   403,     5,
     195,   394,     4,   195,   161,     4,    79,    86,   407,   418,
     161,    87,   195,    79,   197,   327,   203,    87,   410,   195,
     422,    65,   411,   195,    94,   195,    79,   197,   328,    79,
     331,   332,   273,   416,   201,   404,    79,   422,   208,   209,
     424,   425,   290,   195,   202,   195,   202,   195,   202,   404,
     195,   423,   198,   195,   198,   195,   198,   404,   200,   198,
     198,   423,   410,   420,   100,    99,   410,   106,   107,   410,
     410,   410,   422,   410,   410,   410,   418,    81,    82,   117,
     118,   121,   146,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   343,   406,   408,   422,    75,   104,
     105,   111,   120,   128,   148,   335,   195,   268,     4,   195,
     395,   195,     4,   196,   200,   195,   196,     4,   337,   195,
     195,     4,    79,   333,   195,   195,    88,    95,   111,   128,
     161,   162,   163,   164,   166,   167,   168,   169,   170,   366,
       4,     4,   395,   195,   195,   140,   142,   298,   299,   273,
     421,   419,   417,   198,     5,    40,   423,   198,   250,   196,
     422,   196,   196,   196,   196,   196,   196,   196,     3,   195,
      40,    41,    42,   396,   397,    98,   398,     4,   195,   418,
     161,   418,   198,   195,   328,   410,   198,    66,    89,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   198,
     195,   398,   333,     7,   435,   196,   202,   298,   195,   198,
     195,   198,   423,    79,   410,   410,   410,   160,   203,   344,
     420,     4,   129,     4,   195,   196,    95,   400,   195,     4,
     196,   338,   395,   195,   195,    94,   401,     4,     4,    89,
      91,   129,   367,   418,   420,   370,   370,   411,   393,     4,
       4,   400,   334,   196,   301,     5,    40,   195,   266,   256,
     198,   396,    97,   399,   196,   161,   402,     4,   198,   418,
     198,   398,    89,    88,   198,   402,   300,   272,   195,   198,
     193,   198,   344,   401,   196,   380,   198,   400,   195,   321,
     272,   404,   195,   418,   273,   381,   402,    88,   273,   195,
     411,   323,     5,    79,   269,   195,   267,   273,   268,   411,
     198
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
#line 293 "lev_comp.y"
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
#line 316 "lev_comp.y"
    {
		      start_level_def(&splev, (yyvsp[(3) - (3)].map));
		      (yyval.map) = (yyvsp[(3) - (3)].map);
		  ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 321 "lev_comp.y"
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
#line 343 "lev_comp.y"
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
#line 356 "lev_comp.y"
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
#line 368 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiiiio",
				 VA_PASS9(LVLINIT_ROGUE,0,0,0,
					  0,0,0,0, SPO_INITLEVEL));
		  ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 374 "lev_comp.y"
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
#line 403 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_TILESET));
		  ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 409 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (3)].i), SPO_BOUNDARY_TYPE));
		  ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 415 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 419 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 425 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_COPY));
		      (yyval.i) = 0;
		  ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 430 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 436 "lev_comp.y"
    {
		      (yyval.i) = -1;
		  ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 440 "lev_comp.y"
    {
		      (yyval.i) = what_map_char((char) (yyvsp[(2) - (2)].i));
		  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 451 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 455 "lev_comp.y"
    {
		      add_opvars(splev, "io",
                                 VA_PASS2((int) (yyvsp[(3) - (3)].i), SPO_LEVEL_FLAGS));
		  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 462 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 466 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 472 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 476 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 482 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(2) - (3)].i);
		  ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 580 "lev_comp.y"
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

  case 113:

/* Line 1455 of yacc.c  */
#line 596 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_INT);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 602 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_SEL);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 608 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_STRING);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 614 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MAPCHAR);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 620 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_MONST);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 626 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map), SPOVAR_OBJ);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 632 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_COORD);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 638 "lev_comp.y"
    {
		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (3)].map), SPOVAR_REGION);
		      add_opvars(splev, "iso", VA_PASS3(0, (yyvsp[(1) - (3)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (3)].map));
		  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 644 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_INT | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 654 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_COORD | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 664 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_REGION | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 674 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 684 "lev_comp.y"
    {
		      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_MONST | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 694 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(6) - (7)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (7)].map),
                                                SPOVAR_OBJ | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (7)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (7)].map));
		  ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 704 "lev_comp.y"
    {
                      int n_items = (int) (yyvsp[(4) - (5)].i);

		      vardefs = add_vardef_type(vardefs, (yyvsp[(1) - (5)].map),
                                                SPOVAR_STRING | SPOVAR_ARRAY);
		      add_opvars(splev, "iso",
				 VA_PASS3(n_items, (yyvsp[(1) - (5)].map), SPO_VAR_INIT));
		      Free((yyvsp[(1) - (5)].map));
		  ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 716 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 721 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 728 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 733 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 740 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 745 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 752 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 756 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 762 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		      (yyval.i) = 1;
		  ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 767 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(3) - (3)].i)));
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 774 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 778 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 784 "lev_comp.y"
    {
		      (yyval.i) = 1;
		  ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 788 "lev_comp.y"
    {
		      (yyval.i) = 1 + (yyvsp[(1) - (3)].i);
		  ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 794 "lev_comp.y"
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

  case 143:

/* Line 1455 of yacc.c  */
#line 816 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 820 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_RETURN));
		      splev = function_splev_backup;
		      in_function_definition--;
		      curr_function = NULL;
		      vardef_free_all(vardefs);
		      vardefs = function_tmp_var_defs;
		  ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 831 "lev_comp.y"
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

  case 146:

/* Line 1455 of yacc.c  */
#line 886 "lev_comp.y"
    {
		      add_opcode(splev, SPO_EXIT, NULL);
		  ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 892 "lev_comp.y"
    {
		      (yyval.i) = 100;
		  ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 896 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 902 "lev_comp.y"
    {
		      /* val > rn2(100) */
		      add_opvars(splev, "iio",
				 VA_PASS3((int) (yyvsp[(1) - (1)].i), 100, SPO_RN2));
		      (yyval.i) = SPO_JG;
                  ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 909 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(3) - (5)].i);
                  ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 913 "lev_comp.y"
    {
		      /* boolean, explicit foo != 0 */
		      add_opvars(splev, "i", VA_PASS1(0));
		      (yyval.i) = SPO_JNE;
                  ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 921 "lev_comp.y"
    {
		      is_inconstant_number = 0;
		  ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 925 "lev_comp.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 948 "lev_comp.y"
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

  case 157:

/* Line 1455 of yacc.c  */
#line 993 "lev_comp.y"
    {
		      if (n_switch_case_list < MAX_SWITCH_CASES) {
			  struct opvar *tmppush = New(struct opvar);

			  set_opvar_int(tmppush, splev->n_opcodes);
			  switch_case_value[n_switch_case_list] = (yyvsp[(2) - (3)].i);
			  switch_case_list[n_switch_case_list++] = tmppush;
		      } else lc_error("Too many cases in a switch.");
		  ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1003 "lev_comp.y"
    {
		  ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1006 "lev_comp.y"
    {
		      struct opvar *tmppush = New(struct opvar);

		      if (switch_default_case)
			  lc_error("Switch default case already used.");

		      set_opvar_int(tmppush, splev->n_opcodes);
		      switch_default_case = tmppush;
		  ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1016 "lev_comp.y"
    {
		  ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1021 "lev_comp.y"
    {
		      if (!allow_break_statements)
			  lc_error("Cannot use BREAK outside a statement block.");
		      else {
			  break_stmt_new(splev, splev->n_opcodes);
		      }
		  ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1035 "lev_comp.y"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 1072 "lev_comp.y"
    {
		      /* nothing */
		      break_stmt_start();
		  ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1077 "lev_comp.y"
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

  case 167:

/* Line 1455 of yacc.c  */
#line 1106 "lev_comp.y"
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

  case 168:

/* Line 1455 of yacc.c  */
#line 1120 "lev_comp.y"
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

  case 169:

/* Line 1455 of yacc.c  */
#line 1136 "lev_comp.y"
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

  case 170:

/* Line 1455 of yacc.c  */
#line 1156 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1168 "lev_comp.y"
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

  case 172:

/* Line 1455 of yacc.c  */
#line 1188 "lev_comp.y"
    {
		     /* do nothing */
		  ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1194 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;

			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush,
                                        splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?!  No start address?");
		  ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1204 "lev_comp.y"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 1222 "lev_comp.y"
    {
		      if (n_if_list > 0) {
			  struct opvar *tmppush;
			  tmppush = (struct opvar *) if_list[--n_if_list];
			  set_opvar_int(tmppush, splev->n_opcodes - tmppush->vardata.l);
		      } else lc_error("IF: Huh?! No end address?");
		  ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1232 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MESSAGE));
		  ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1238 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1,  0, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1243 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, (yyvsp[(3) - (3)].i), -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1248 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
			      VA_PASS7(-1, -1, -1, -1, -1, -1, SPO_CORRIDOR));
		  ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1255 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  (yyvsp[(5) - (5)].corpos).room, (yyvsp[(5) - (5)].corpos).door, (yyvsp[(5) - (5)].corpos).wall,
					  SPO_CORRIDOR));
		  ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1262 "lev_comp.y"
    {
		      add_opvars(splev, "iiiiiio",
				 VA_PASS7((yyvsp[(3) - (5)].corpos).room, (yyvsp[(3) - (5)].corpos).door, (yyvsp[(3) - (5)].corpos).wall,
					  -1, -1, (long)(yyvsp[(5) - (5)].i),
					  SPO_CORRIDOR));
		  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1271 "lev_comp.y"
    {
			(yyval.corpos).room = (yyvsp[(2) - (7)].i);
			(yyval.corpos).wall = (yyvsp[(4) - (7)].i);
			(yyval.corpos).door = (yyvsp[(6) - (7)].i);
		  ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1279 "lev_comp.y"
    {
		      if (((yyvsp[(2) - (4)].i) < 100) && ((yyvsp[(1) - (4)].i) == OROOM))
			  lc_error("Only typed rooms can have a chance.");
		      else {
			  add_opvars(splev, "iii",
				     VA_PASS3((long)(yyvsp[(1) - (4)].i), (long)(yyvsp[(2) - (4)].i), (long)(yyvsp[(4) - (4)].i)));
		      }
                  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1290 "lev_comp.y"
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

  case 185:

/* Line 1455 of yacc.c  */
#line 1306 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1313 "lev_comp.y"
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

  case 187:

/* Line 1455 of yacc.c  */
#line 1329 "lev_comp.y"
    {
		      break_stmt_end(splev);
		      add_opcode(splev, SPO_ENDROOM, NULL);
		  ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1336 "lev_comp.y"
    {
			(yyval.i) = 1;
		  ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1340 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1346 "lev_comp.y"
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

  case 191:

/* Line 1455 of yacc.c  */
#line 1356 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1362 "lev_comp.y"
    {
			if ( (yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0) {
			    lc_error("Invalid subroom position (%li,%li)!", (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
			} else {
			    (yyval.crd).x = (yyvsp[(2) - (5)].i);
			    (yyval.crd).y = (yyvsp[(4) - (5)].i);
			}
		  ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1371 "lev_comp.y"
    {
			(yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1377 "lev_comp.y"
    {
		      (yyval.crd).x = (yyvsp[(2) - (5)].i);
		      (yyval.crd).y = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1382 "lev_comp.y"
    {
		      (yyval.crd).x = (yyval.crd).y = ERR;
		  ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1388 "lev_comp.y"
    {
			(yyval.sze).width = (yyvsp[(2) - (5)].i);
			(yyval.sze).height = (yyvsp[(4) - (5)].i);
		  ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1393 "lev_comp.y"
    {
			(yyval.sze).height = (yyval.sze).width = ERR;
		  ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1399 "lev_comp.y"
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

  case 199:

/* Line 1455 of yacc.c  */
#line 1410 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((long)(yyvsp[(3) - (6)].i), SPO_DOOR));
		  ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1424 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1428 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1438 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_D_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1445 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("DOOR extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1453 "lev_comp.y"
    {	
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SUBTYPE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1458 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_D_V_KEY_TYPE));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1463 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_D_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1468 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1473 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_SECRET_DOOR));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1478 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_USES_UP_KEY));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1483 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_D_V_NON_PASSDOOR));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1491 "lev_comp.y"
    {
		      add_opvars(splev, "ciisiio",
				 VA_PASS7(0, 0, 1, (char *) 0, 0, 0, SPO_MAP));
		      max_x_map = COLNO-1;
		      max_y_map = ROWNO;
		  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1498 "lev_comp.y"
    {
		      add_opvars(splev, "cii",
				 VA_PASS3(SP_COORD_PACK(((yyvsp[(3) - (7)].i)), ((yyvsp[(5) - (7)].i))),
					  1, (int) (yyvsp[(6) - (7)].i)));
		      scan_map((yyvsp[(7) - (7)].map), splev);
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1506 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(2, (int) (yyvsp[(4) - (5)].i)));
		      scan_map((yyvsp[(5) - (5)].map), splev);
		      Free((yyvsp[(5) - (5)].map));
		  ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1522 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(0, SPO_MONSTER));
		  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1526 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_MONSTER));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1532 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opvars(splev, "o", VA_PASS1(SPO_END_MONINVENT));
		 ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1540 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1546 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);

		      set_opvar_int(stopit, SP_M_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00000000;
		  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1554 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("MONSTER extra info defined twice.");
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1562 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_NAME));
		      (yyval.i) = 0x00000001;
		  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1567 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_PEACEFUL));
		      (yyval.i) = 0x00000002;
		  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1573 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ASLEEP));
		      (yyval.i) = 0x00000004;
		  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1579 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_M_V_ALIGN));
		      (yyval.i) = 0x00000008;
		  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1585 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (2)].i), SP_M_V_APPEAR));
		      (yyval.i) = 0x00000010;
		  ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1591 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_FEMALE));
		      (yyval.i) = 0x00000020;
		  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1596 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_INVIS));
		      (yyval.i) = 0x00000040;
		  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1601 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CANCELLED));
		      (yyval.i) = 0x00000080;
		  ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1606 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_REVIVED));
		      (yyval.i) = 0x00000100;
		  ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1611 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_AVENGE));
		      (yyval.i) = 0x00000200;
		  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1616 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_FLEEING));
		      (yyval.i) = 0x00000400;
		  ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1621 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_BLINDED));
		      (yyval.i) = 0x00000800;
		  ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1626 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_M_V_PARALYZED));
		      (yyval.i) = 0x00001000;
		  ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1631 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_STUNNED));
		      (yyval.i) = 0x00002000;
		  ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1636 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_CONFUSED));
		      (yyval.i) = 0x00004000;
		  ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1641 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(3) - (3)].i), SP_M_V_SEENTRAPS));
		      (yyval.i) = 0x00008000;
		  ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1647 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_M_V_WAITFORU));
		      (yyval.i) = 0x00010000;
		  ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1654 "lev_comp.y"
    {
		      int token = get_trap_type((yyvsp[(1) - (1)].map));

		      if (token == ERR || token == 0)
			  lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
                      Free((yyvsp[(1) - (1)].map));
		      (yyval.i) = (1L << (token - 1));
		  ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1663 "lev_comp.y"
    {
		      (yyval.i) = (long) ~0;
		  ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1667 "lev_comp.y"
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

  case 250:

/* Line 1455 of yacc.c  */
#line 1680 "lev_comp.y"
    {
		      int cnt = 0;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1688 "lev_comp.y"
    {
		      int cnt = SP_OBJ_CONTAINER;

		      if (in_container_obj)
                          cnt |= SP_OBJ_CONTENT;
		      add_opvars(splev, "io", VA_PASS2(cnt, SPO_OBJECT));
		      in_container_obj++;
		      break_stmt_start();
		  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1698 "lev_comp.y"
    {
		     break_stmt_end(splev);
		     in_container_obj--;
		     add_opcode(splev, SPO_POP_CONTAINER, NULL);
		 ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1706 "lev_comp.y"
    {
		      if (( (yyvsp[(2) - (2)].i) & 0x4000) && in_container_obj)
                          lc_error("Object cannot have a coord when contained.");
		      else if (!( (yyvsp[(2) - (2)].i) & 0x4000) && !in_container_obj)
                          lc_error("Object needs a coord when not contained.");
		  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1715 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_O_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1722 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("OBJECT extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1730 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_CURSE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1736 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CORPSENM));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1741 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPE));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1746 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_NAME));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1751 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_QUAN));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1756 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_BURIED));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1761 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_LIT));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1766 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1771 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(-1, SP_O_V_ERODED));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1776 "lev_comp.y"
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

  case 266:

/* Line 1455 of yacc.c  */
#line 1790 "lev_comp.y"
    {
		      add_opvars(splev, "ii",
                                 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_O_V_TRAPPED));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 1796 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_RECHARGED));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1801 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INVIS));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1806 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_GREASED));
		      (yyval.i) = 0x2000;
		  ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 1811 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_COORD));
		      (yyval.i) = 0x4000;
		  ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1816 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_ELEMENTAL_ENCHANTMENT));
		      (yyval.i) = 0x8000;
		  ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1821 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_EXCEPTIONALITY));
		      (yyval.i) = 0x10000;
		  ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1826 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_ENCHANTMENT));
		      (yyval.i) = 0x20000;
		  ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 1831 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_CHARGES));
		      (yyval.i) = 0x40000;
		  ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 1836 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x80000;
		  ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1841 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_SPEFLAGS));
		      (yyval.i) = 0x100000;
		  ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1846 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_KEY_TYPE));
		      (yyval.i) = 0x200000;
		  ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1851 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_INDESTRUCTIBLE));
		      (yyval.i) = 0x400000;
		  ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1856 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_USES_UP_KEY));
		      (yyval.i) = 0x800000;
		  ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1861 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2(1, SP_O_V_NO_PICKUP));
		      (yyval.i) = 0x1000000;
		  ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 1866 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_PREFIX));
		      (yyval.i) = 0x2000000;
		  ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1871 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].i), SP_O_V_MYTHIC_SUFFIX));
		      (yyval.i) = 0x4000000;
		  ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1876 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_O_V_AGE));
		      (yyval.i) = 0x8000000;
		  ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 1883 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(3) - (5)].i), SPO_TRAP));
		  ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1889 "lev_comp.y"
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

  case 286:

/* Line 1455 of yacc.c  */
#line 1918 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (5)].i), 1, 0, SPO_MAZEWALK));
		  ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1923 "lev_comp.y"
    {
		      add_opvars(splev, "iiio",
				 VA_PASS4((int) (yyvsp[(5) - (8)].i), (int) (yyvsp[(7) - (8)].i),
					  (int) (yyvsp[(8) - (8)].i), SPO_MAZEWALK));
		  ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1931 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  0, SPO_WALLIFY));
		  ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1937 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_WALLIFY));
		  ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1941 "lev_comp.y"
    {
		      add_opvars(splev, "rio",
				 VA_PASS3(SP_REGION_PACK(-1,-1,-1,-1),
					  2, SPO_WALLIFY));
		  ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1949 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_LADDER));
		  ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1956 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((int) (yyvsp[(5) - (5)].i), SPO_STAIR));
		  ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1963 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
				     (long) (((yyvsp[(7) - (7)].i)) ? LR_UPSTAIR : LR_DOWNSTAIR),
					   0, (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1973 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (7)].lregn).x1, (yyvsp[(3) - (7)].lregn).y1, (yyvsp[(3) - (7)].lregn).x2, (yyvsp[(3) - (7)].lregn).y2, (yyvsp[(3) - (7)].lregn).area,
					   (yyvsp[(5) - (7)].lregn).x1, (yyvsp[(5) - (7)].lregn).y1, (yyvsp[(5) - (7)].lregn).x2, (yyvsp[(5) - (7)].lregn).y2, (yyvsp[(5) - (7)].lregn).area,
					   LR_PORTAL, 0, (yyvsp[(7) - (7)].map), SPO_LEVREGION));
		      Free((yyvsp[(7) - (7)].map));
		  ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1983 "lev_comp.y"
    {
		      long rtyp = 0;
		      switch((yyvsp[(6) - (6)].i)) {
		      case -1: rtyp = LR_TELE; break;
		      case  0: rtyp = LR_DOWNTELE; break;
		      case  1: rtyp = LR_UPTELE; break;
		      }
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (6)].lregn).x1, (yyvsp[(3) - (6)].lregn).y1, (yyvsp[(3) - (6)].lregn).x2, (yyvsp[(3) - (6)].lregn).y2, (yyvsp[(3) - (6)].lregn).area,
					   (yyvsp[(5) - (6)].lregn).x1, (yyvsp[(5) - (6)].lregn).y1, (yyvsp[(5) - (6)].lregn).x2, (yyvsp[(5) - (6)].lregn).y2, (yyvsp[(5) - (6)].lregn).area,
					   rtyp, 0, (char *)0, SPO_LEVREGION));
		  ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1998 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   (yyvsp[(5) - (5)].lregn).x1, (yyvsp[(5) - (5)].lregn).y1, (yyvsp[(5) - (5)].lregn).x2, (yyvsp[(5) - (5)].lregn).y2, (yyvsp[(5) - (5)].lregn).area,
					   (long) LR_BRANCH, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2008 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2012 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2018 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_FOUNTAIN));
		  ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2024 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_THRONE));
		  ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2030 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int) (yyvsp[(9) - (11)].i), (int) (yyvsp[(11) - (11)].i), SPO_MODRON_PORTAL));
		  ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2034 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio", VA_PASS5((int) (yyvsp[(7) - (13)].i), (int) (yyvsp[(9) - (13)].i), (int) (yyvsp[(11) - (13)].i), (int) (yyvsp[(13) - (13)].i), SPO_MODRON_LEVEL_TELEPORTER));
		  ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2040 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int) (yyvsp[(5) - (6)].i), SPO_LEVER));
		  ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2046 "lev_comp.y"
    {
		      struct opvar *stopit = New(struct opvar);
		      set_opvar_int(stopit, SP_L_V_END);
		      add_opcode(splev, SPO_PUSH, stopit);
		      (yyval.i) = 0x00;
		  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2053 "lev_comp.y"
    {
		      if (( (yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i) ))
			  lc_error("LEVER extra info '%s' defined twice.", curr_token);
		      (yyval.i) = ( (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i) );
		  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2061 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2((int) (yyvsp[(1) - (1)].i), SP_L_V_ACTIVE));
		      (yyval.i) = 0x0001;
		  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2067 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_MONSTER));
		      (yyval.i) = 0x0002;
		  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2072 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_OBJECT));
		      (yyval.i) = 0x0004;
		  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2077 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN));
		      (yyval.i) = 0x0008;
		  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2082 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((int)(yyvsp[(3) - (3)].terr).ter, SP_L_V_TERRAIN2));
		      (yyval.i) = 0x0010;
		  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2087 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_SWITCHABLE));
		      (yyval.i) = 0x0020;
		  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2093 "lev_comp.y"
    {	
		      add_opvars(splev, "ii",
				 VA_PASS2(1, SP_L_V_CONTINUOUS));
		      (yyval.i) = 0x0040;
		  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2099 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1(SP_L_V_COORD));
		      (yyval.i) = 0x0080;
		  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2104 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_TRAP));
		      (yyval.i) = 0x0100;
		  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2109 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SUBTYPE));
		      (yyval.i) = 0x0200;
		  ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2114 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_FLOOR_SUBTYPE));
		      (yyval.i) = 0x0400;
		  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2119 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_EFFECT_FLAG));
		      (yyval.i) = 0x0800;
		  ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2124 "lev_comp.y"
    {
		      add_opvars(splev, "ii", VA_PASS2((yyvsp[(3) - (3)].i), SP_L_V_SPECIAL_QUALITY));
		      (yyval.i) = 0x1000;
		  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2134 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SINK));
		  ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2140 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_POOL));
		  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2146 "lev_comp.y"
    {
		      (yyval.terr).lit = -2;
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(1) - (1)].i));
		  ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2151 "lev_comp.y"
    {
		      (yyval.terr).lit = (yyvsp[(4) - (5)].i);
		      (yyval.terr).ter = what_map_char((char) (yyvsp[(2) - (5)].i));
		  ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2158 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((yyvsp[(9) - (9)].i), SPO_REPLACETERRAIN));
		  ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2165 "lev_comp.y"
    {
		     add_opvars(splev, "o", VA_PASS1(SPO_TERRAIN));
		 ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2171 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_DIGGABLE));
		  ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2177 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_NON_PASSWALL));
		  ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2183 "lev_comp.y"
    {
		      add_opvars(splev, "Miso", VA_PASS4(-1, (yyvsp[(5) - (5)].i), (yyvsp[(3) - (5)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (5)].map));
		  ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2188 "lev_comp.y"
    {
		      add_opvars(splev, "iso", VA_PASS3((yyvsp[(5) - (9)].i), (yyvsp[(3) - (9)].map), SPO_NAMING));
		      Free((yyvsp[(3) - (9)].map));
		  ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2195 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_REGION));
		  ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2201 "lev_comp.y"
    {
		      add_opvars(splev, "iiiii iiiii iiso",
				 VA_PASS14((yyvsp[(3) - (5)].lregn).x1, (yyvsp[(3) - (5)].lregn).y1, (yyvsp[(3) - (5)].lregn).x2, (yyvsp[(3) - (5)].lregn).y2, (yyvsp[(3) - (5)].lregn).area,
					   0, 0, 0, 0, 1,
					   (yyvsp[(5) - (5)].i) == REGION_SPECIAL_LEVEL_SEEN ? (long) LR_SPECIAL_MAP_SEEN : (long) LR_SPECIAL_MAP_NAME_REVEALED, 0,
					   (char *) 0, SPO_LEVREGION));
		  ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2211 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(5) - (5)].i), SPO_SPECIAL_TILESET));
		  ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 2217 "lev_comp.y"
    {
		      long irr;
		      long rt = (yyvsp[(7) - (11)].i);
		      long rflags = (yyvsp[(8) - (11)].i);
		      long flmt = (long)(yyvsp[(9) - (11)].i);
		      long flt = (long)(yyvsp[(10) - (11)].i);

		      if (rflags == -1) rflags = (1 << 0);
		      if (flmt == -1) flmt = 0;
		      if (flt == -1) flt = 0;

		      if (!(rflags & 1)) rt += MAXRTYPE+1;
		      irr = ((rflags & 2) != 0);
		      add_opvars(splev, "iiiiio",
				 VA_PASS6((long)(yyvsp[(5) - (11)].i), rt, rflags, flmt, flt, SPO_REGION));
		      (yyval.i) = (irr || (rflags & 1) || rt != OROOM);
		      break_stmt_start();
		  ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2236 "lev_comp.y"
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

  case 338:

/* Line 1455 of yacc.c  */
#line 2248 "lev_comp.y"
    {
		      (yyval.i) = 0;
		  ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2252 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2258 "lev_comp.y"
    {
		      add_opvars(splev, "Miio",
				 VA_PASS4(-1, (long)(yyvsp[(7) - (7)].i), (long)(yyvsp[(5) - (7)].i), SPO_ALTAR));
		  ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2263 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((long)(yyvsp[(7) - (11)].i), (long)(yyvsp[(5) - (11)].i), SPO_ALTAR));
		  ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2270 "lev_comp.y"
    {
		      add_opvars(splev, "Mo", VA_PASS2(-1, SPO_ANVIL));
		  ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2274 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_ANVIL));
		  ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2280 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_FLOOR));
		  ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2286 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(5) - (5)].i), SPO_SUBTYPE));
		  ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2292 "lev_comp.y"
    {
		      add_opvars(splev, "Mio", VA_PASS3(-1, (int)(yyvsp[(3) - (5)].i), SPO_NPC));
		  ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2296 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((int)(yyvsp[(3) - (9)].i), SPO_NPC));
		  ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2302 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(2, SPO_GRAVE));
		  ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2306 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 1, SPO_GRAVE));
		  ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2311 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS3((char *)0, 0, SPO_GRAVE));
		  ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2318 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3((int)(yyvsp[(7) - (7)].i), (int)(yyvsp[(5) - (7)].i), SPO_BRAZIER));
		  ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2322 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3(1, 0, SPO_BRAZIER));
		  ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2329 "lev_comp.y"
    {
		      add_opvars(splev, "iio", VA_PASS3(2, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2333 "lev_comp.y"
    {
		      add_opvars(splev, "siio",
				 VA_PASS4((char *)0, 1, (int)(yyvsp[(5) - (7)].i), SPO_SIGNPOST));
		  ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2338 "lev_comp.y"
    {
		      add_opvars(splev, "sio",
				 VA_PASS4((char *)0, 0, 0, SPO_SIGNPOST));
		  ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2345 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_GOLD));
		  ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2351 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2((long)(yyvsp[(5) - (7)].i), SPO_ENGRAVING));
		  ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2358 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MINERALIZE));
		  ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2362 "lev_comp.y"
    {
		      add_opvars(splev, "iiiio",
				 VA_PASS5(-1L, -1L, -1L, -1L, SPO_MINERALIZE));
		  ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2369 "lev_comp.y"
    {
			int token = get_trap_type((yyvsp[(1) - (1)].map));
			if (token == ERR)
			    lc_error("Unknown trap type '%s'!", (yyvsp[(1) - (1)].map));
			(yyval.i) = token;
			Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2380 "lev_comp.y"
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

  case 364:

/* Line 1455 of yacc.c  */
#line 2393 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2397 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(2) - (2)].i);
		  ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2403 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2407 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2414 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 0);
		  ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2418 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 1);
		  ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2422 "lev_comp.y"
    {
		      (yyval.i) = ((yyvsp[(1) - (1)].i) << 2);
		  ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2428 "lev_comp.y"
    {
			(yyval.i) = -1;
		  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2432 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2438 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2444 "lev_comp.y"
    {
			(yyval.i) = 0;
		  ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2448 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2454 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2460 "lev_comp.y"
    {
			add_opvars(splev, "M", VA_PASS1(-1));
			(yyval.i) = -1;
		  ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 2465 "lev_comp.y"
    {
			(yyval.i) = (yyvsp[(3) - (3)].i);
		  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2482 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2490 "lev_comp.y"
    {
			(yyval.i) = - MAX_REGISTERS - 1;
		  ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2500 "lev_comp.y"
    {
			if ( (yyvsp[(3) - (4)].i) >= 3 )
				lc_error("Register Index overflow!");
			else
				(yyval.i) = - (yyvsp[(3) - (4)].i) - 1;
		  ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2509 "lev_comp.y"
    {
		      add_opvars(splev, "s", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2514 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_STRING);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2521 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_STRING | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2532 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2538 "lev_comp.y"
    {
		      add_opvars(splev, "c", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 2542 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDCOORD));
		  ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2546 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_COORD);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2553 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_COORD | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2563 "lev_comp.y"
    {
		      if ((yyvsp[(2) - (5)].i) < 0 || (yyvsp[(4) - (5)].i) < 0 || (yyvsp[(2) - (5)].i) >= COLNO || (yyvsp[(4) - (5)].i) >= ROWNO)
                          lc_error("Coordinates (%li,%li) out of map range!",
                                   (yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		      (yyval.i) = SP_COORD_PACK((yyvsp[(2) - (5)].i), (yyvsp[(4) - (5)].i));
		  ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2570 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM(0);
		  ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2574 "lev_comp.y"
    {
		      (yyval.i) = SP_COORD_PACK_RANDOM((yyvsp[(2) - (3)].i));
		  ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2580 "lev_comp.y"
    {
		      (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2584 "lev_comp.y"
    {
		      if (((yyvsp[(1) - (3)].i) & (yyvsp[(3) - (3)].i)))
			  lc_warning("Humidity flag used twice.");
		      (yyval.i) = ((yyvsp[(1) - (3)].i) | (yyvsp[(3) - (3)].i));
		  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2592 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2596 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_REGION);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2603 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_REGION | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2613 "lev_comp.y"
    {
		      long r = SP_REGION_PACK((yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      if ((yyvsp[(2) - (9)].i) > (yyvsp[(6) - (9)].i) || (yyvsp[(4) - (9)].i) > (yyvsp[(8) - (9)].i))
			  lc_error("Region start > end: (%ld,%ld,%ld,%ld)!",
                                   (yyvsp[(2) - (9)].i), (yyvsp[(4) - (9)].i), (yyvsp[(6) - (9)].i), (yyvsp[(8) - (9)].i));

		      add_opvars(splev, "r", VA_PASS1(r));
		      (yyval.i) = r;
		  ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2626 "lev_comp.y"
    {
		      add_opvars(splev, "m", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2630 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MAPCHAR);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2637 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MAPCHAR | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 2647 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(1) - (1)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(1) - (1)].i)), -2);
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(1) - (1)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, -2);
		      }
		  ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2656 "lev_comp.y"
    {
		      if (what_map_char((char) (yyvsp[(2) - (5)].i)) != INVALID_TYPE)
			  (yyval.i) = SP_MAPCHAR_PACK(what_map_char((char) (yyvsp[(2) - (5)].i)), (yyvsp[(4) - (5)].i));
		      else {
			  lc_error("Unknown map char type '%c'!", (yyvsp[(2) - (5)].i));
			  (yyval.i) = SP_MAPCHAR_PACK(STONE, (yyvsp[(4) - (5)].i));
		      }
		  ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2667 "lev_comp.y"
    {
		      add_opvars(splev, "M", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2671 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_MONST);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2678 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_MONST | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2688 "lev_comp.y"
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

  case 418:

/* Line 1455 of yacc.c  */
#line 2699 "lev_comp.y"
    {
                        if (check_monster_char((char) (yyvsp[(1) - (1)].i)))
                            (yyval.i) = SP_MONST_PACK(-1, (yyvsp[(1) - (1)].i));
                        else {
                            lc_error("Unknown monster class '%c'!", (yyvsp[(1) - (1)].i));
                            (yyval.i) = -1;
                        }
                  ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2708 "lev_comp.y"
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

  case 420:

/* Line 1455 of yacc.c  */
#line 2718 "lev_comp.y"
    {
                      (yyval.i) = -1;
                  ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 2724 "lev_comp.y"
    {
		      add_opvars(splev, "O", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 2728 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_OBJ);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 2735 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
                                        SPOVAR_OBJ | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		  ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 2745 "lev_comp.y"
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

  case 425:

/* Line 1455 of yacc.c  */
#line 2756 "lev_comp.y"
    {
			if (check_object_char((char) (yyvsp[(1) - (1)].i)))
			    (yyval.i) = SP_OBJ_PACK(-1, (yyvsp[(1) - (1)].i));
			else {
			    lc_error("Unknown object class '%c'!", (yyvsp[(1) - (1)].i));
			    (yyval.i) = -1;
			}
		  ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2765 "lev_comp.y"
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

  case 427:

/* Line 1455 of yacc.c  */
#line 2775 "lev_comp.y"
    {
		      (yyval.i) = OBJECT_SPECIAL_CREATE_TYPE_RANDOM;
		  ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 2779 "lev_comp.y"
    {
  			  (yyval.i) = (yyvsp[(1) - (1)].i);
		  ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2785 "lev_comp.y"
    { ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2787 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2793 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2797 "lev_comp.y"
    {
		      is_inconstant_number = 1;
		  ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2801 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(2) - (3)].i)));
		  ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2805 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_INT);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2813 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (4)].map),
					SPOVAR_INT | SPOVAR_ARRAY);
		      vardef_used(vardefs, (yyvsp[(1) - (4)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (4)].map)));
		      Free((yyvsp[(1) - (4)].map));
		      is_inconstant_number = 1;
		  ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2822 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_ADD));
		  ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2826 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_SUB));
		  ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2830 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MUL));
		  ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2834 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_DIV));
		  ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2838 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_MATH_MOD));
		  ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2841 "lev_comp.y"
    { ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2845 "lev_comp.y"
    {
		      if (!strcmp("int", (yyvsp[(1) - (1)].map)) || !strcmp("integer", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'i';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2852 "lev_comp.y"
    {
		      if (!strcmp("str", (yyvsp[(1) - (1)].map)) || !strcmp("string", (yyvsp[(1) - (1)].map))) {
			  (yyval.i) = (int)'s';
		      } else
			  lc_error("Unknown function parameter type '%s'", (yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2861 "lev_comp.y"
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

  case 449:

/* Line 1455 of yacc.c  */
#line 2902 "lev_comp.y"
    {
			      (yyval.i) = (int)'i';
			  ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2906 "lev_comp.y"
    {
			      (yyval.i) = (int)'s';
			  ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2913 "lev_comp.y"
    {
			      char tmpbuf[2];
			      tmpbuf[0] = (char) (yyvsp[(1) - (1)].i);
			      tmpbuf[1] = '\0';
			      (yyval.map) = strdup(tmpbuf);
			  ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2920 "lev_comp.y"
    {
			      long len = strlen( (yyvsp[(1) - (3)].map) );
			      char *tmp = (char *) alloc(len + 2);
			      sprintf(tmp, "%c%s", (char) (yyvsp[(3) - (3)].i), (yyvsp[(1) - (3)].map) );
			      Free( (yyvsp[(1) - (3)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2930 "lev_comp.y"
    {
			      (yyval.map) = strdup("");
			  ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2934 "lev_comp.y"
    {
			      char *tmp = strdup( (yyvsp[(1) - (1)].map) );
			      Free( (yyvsp[(1) - (1)].map) );
			      (yyval.map) = tmp;
			  ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2942 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_POINT));
		  ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2946 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RECT));
		  ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2950 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FILLRECT));
		  ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2954 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_LINE));
		  ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2958 "lev_comp.y"
    {
		      /* randline (x1,y1),(x2,y2), roughness */
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_RNDLINE));
		  ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2963 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(W_ANY, SPO_SEL_GROW));
		  ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2967 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(3) - (6)].i), SPO_SEL_GROW));
		  ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2971 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
			     VA_PASS3((yyvsp[(3) - (6)].i), SPOFILTER_PERCENT, SPO_SEL_FILTER));
		  ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2976 "lev_comp.y"
    {
		      add_opvars(splev, "io",
			       VA_PASS2(SPOFILTER_SELECTION, SPO_SEL_FILTER));
		  ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2981 "lev_comp.y"
    {
		      add_opvars(splev, "io",
				 VA_PASS2(SPOFILTER_MAPCHAR, SPO_SEL_FILTER));
		  ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2986 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_FLOOD));
		  ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2990 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, 1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2995 "lev_comp.y"
    {
		      add_opvars(splev, "oio",
				 VA_PASS3(SPO_COPY, (yyvsp[(7) - (8)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 3000 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2(1, SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3004 "lev_comp.y"
    {
		      add_opvars(splev, "io", VA_PASS2((yyvsp[(9) - (10)].i), SPO_SEL_ELLIPSE));
		  ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3008 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(9) - (14)].i), (yyvsp[(3) - (14)].i), SPO_SEL_GRADIENT));
		  ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3013 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_COMPLEMENT));
		  ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3017 "lev_comp.y"
    {
		      check_vardef_type(vardefs, (yyvsp[(1) - (1)].map), SPOVAR_SEL);
		      vardef_used(vardefs, (yyvsp[(1) - (1)].map));
		      add_opvars(splev, "v", VA_PASS1((yyvsp[(1) - (1)].map)));
		      Free((yyvsp[(1) - (1)].map));
		  ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3024 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3030 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3034 "lev_comp.y"
    {
		      add_opvars(splev, "o", VA_PASS1(SPO_SEL_ADD));
		  ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3040 "lev_comp.y"
    {
		      add_opvars(splev, "iio",
				 VA_PASS3((yyvsp[(1) - (1)].dice).num, (yyvsp[(1) - (1)].dice).die, SPO_DICE));
		  ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3056 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3060 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3064 "lev_comp.y"
    {
		      add_opvars(splev, "i", VA_PASS1((yyvsp[(1) - (1)].i)));
		  ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3068 "lev_comp.y"
    {
		      /* nothing */
		  ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3090 "lev_comp.y"
    {
			(yyval.lregn) = (yyvsp[(1) - (1)].lregn);
		  ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3094 "lev_comp.y"
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

  case 496:

/* Line 1455 of yacc.c  */
#line 3120 "lev_comp.y"
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
#line 7054 "lev_comp.tab.c"
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
#line 3148 "lev_comp.y"


/*lev_comp.y*/

